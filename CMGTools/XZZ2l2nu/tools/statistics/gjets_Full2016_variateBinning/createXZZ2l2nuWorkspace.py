
import ROOT, os, string
import math
from math import *

from ROOT import *
from array import array

import sys, os, pwd, commands
from subprocess import *
import optparse, shlex, re

# load signal modules
sys.path.append('./SigInputs')

grootargs = []
def callback_rootargs(option, opt, value, parser):
    grootargs.append(opt)

def parseOptions():

    usage = ('usage: %prog [options]\n'
             + '%prog -h for help')
    parser = optparse.OptionParser(usage)

    parser.add_option("--cutChain", dest="CutChain",default='zpt100met50',help="kinematic cuts on zpt and met")
    parser.add_option("--dyMC",action="store_true", dest="DyMC", default=False,help="whether use DY MC to predict Z+jets MT spectrum")
    parser.add_option("--perBinStatUnc",action="store_true", dest="PerBinStatUnc", default=False,help="whether do per-bin statistical uncertainty")
    #parser.add_option("--sigType", dest='SigType', type='string',default='BulkGrav_narrow',help='signal type')
    parser.add_option("--runLimits",action="store_true", dest="RunLimits", default=False,help="whether run limits")


    parser.add_option("-l",action="callback",callback=callback_rootargs)
    parser.add_option("-q",action="callback",callback=callback_rootargs)
    parser.add_option("-b",action="callback",callback=callback_rootargs)

    #(option, args) = parser.parse_args()
    return parser 

def createXSworkspace(sigType,MX, channel,cat, parser):
    
    pedestal = 1e-10
    (options,args) = parser.parse_args()

    cut=options.CutChain
    perBinStatUnc=options.PerBinStatUnc
    isDyMC=options.DyMC
    isGJets= (not isDyMC)

    zjetsMethod = 'mczjet'
    if( not isDyMC) :
      zjetsMethod = 'gjet'

    fs=""
    if(channel=="mm") :
      fs = "mu"
    elif(channel=="ee") :
      fs = "el"

    outdir = zjetsMethod+'_'+cut
    #outdir = outdir + '_bin'+str(bin_index)
    if(perBinStatUnc) :
      outdir = outdir + '_perBinStatUnc'
    outdir = outdir + '_' + sigType
    if not os.path.exists("Datacards/"+outdir):
       os.system('mkdir -p Datacards/'+outdir)
 
    #GJets_GMCEtaWt_GMCPhPtWt_
    prefix = "GJets_GMCEtaWt_GMCPhPtWt_"+cat+"_puWeightsummer16_muoneg_"+zjetsMethod+"_metfilter_unblind"
    inputfileName = prefix + "_" + fs + "_log_1pb"
    inputfile = TFile('Templates/'+inputfileName+".root","READ")

    ## Templates and Systematic Uncertainties
    Shape = {}
    Shape_ups = {}
    Shape_dns = {}
    ## Yields
    theRates = {}

    _temp = __import__('signals', globals(), locals(), ['sigNames'], -1)
    sigNames = _temp.sigNames
    sigName = sigNames[sigType]+str(MX)

    processes = ['NonReso','VVZReso','ZJets', sigName]
    #process xzz zjets vvreso nonreso
    systs = ['fidxsec','id','trg','ewk','qcd','JetEn','JetRes','MuonEn','ElectronEn','PhotonEn','TauEn','Uncluster','Recoil']
    systsMT = ['JetEn','JetRes','MuonEn','ElectronEn','PhotonEn','TauEn','Uncluster','Recoil']

    processNameinDC = {'NonReso':'nonreso','VVZReso':'vvreso','ZJets':'zjets', sigName:'xzz'+str(MX)}

    #bins = [0.0, 100.0, 150.0, 200.0, 250.0, 300.0, 350.0, 400.0, 450.0, 500.0, 550.0, 600.0, 650.0, 700.0, 750.0, 800.0, 850, 900, 1000, 1100, 1250, 1450, 1650, 1850.0, 3000.0]
    bins = [0.0, 100.0, 150.0, 200.0, 250.0, 300.0, 350.0, 400.0, 450.0, 500.0, 550.0, 600.0, 650.0, 700.0, 750.0, 800.0, 850, 900, 1000, 1100, 1250, 1450, 1600, 3000.0]

    '''
    bins = []
    if(bin_index ==0 ) :
      bins = list(bins_0)      
    elif(bin_index ==1 ) :
      bins = list(bins_0)
    '''
    nbins = len(bins)-1

    for process in processes :

       Shape_orig = (inputfile.Get("mT_"+process)).Clone()
       Shape[process] = TH1D(process, process, nbins, array('d',bins))
       rebinMerge(Shape[process],Shape_orig)  

       for i in range(0,Shape[process].GetXaxis().GetNbins()) :

          if(Shape[process].GetBinContent(i+1)<pedestal) :
            Shape[process].SetBinContent(i+1,pedestal)

       Shape[process].SetName(processNameinDC[process])
       Shape[process].SetTitle(processNameinDC[process])

       theRates[process] = Shape[process].Integral()

       for syst in systs :

         if(process=='NonReso') :
           continue
         if(process!='ZJets' and syst == 'Recoil' ) :
           continue

         Shape_dns[syst+"_"+process] = TH1D(syst+"Up_"+process, process, len(bins)-1,array('d',bins))
         rebinMerge(Shape_dns[syst+"_"+process],inputfile.Get("mT_"+syst+"Dn_"+process))
         Shape_ups[syst+"_"+process] = TH1D(syst+"Dn_"+process, process, len(bins)-1,array('d',bins))
         rebinMerge(Shape_ups[syst+"_"+process],inputfile.Get("mT_"+syst+"Up_"+process))

         if(syst=="trg" or syst=="id" or syst=="zpt" or syst=="fidxsec") :

           Shape_dns[syst+"_"+process].SetName(processNameinDC[process]+"_"+syst+fs+"Down")
           Shape_ups[syst+"_"+process].SetName(processNameinDC[process]+"_"+syst+fs+"Up")
           Shape_dns[syst+"_"+process].SetTitle(processNameinDC[process])
           Shape_ups[syst+"_"+process].SetTitle(processNameinDC[process])

         elif(syst in systsMT and processNameinDC[process]=='zjets'):        

           if(syst!='Recoil'):
             Shape_dns[syst+"_"+process].SetName(processNameinDC[process]+"_phyMET"+syst+"Down")
             Shape_ups[syst+"_"+process].SetName(processNameinDC[process]+"_phyMET"+syst+"Up")
           else :
             Shape_dns[syst+"_"+process].SetName(processNameinDC[process]+"_zjets"+syst+"Down")
             Shape_ups[syst+"_"+process].SetName(processNameinDC[process]+"_zjets"+syst+"Up")

           Shape_dns[syst+"_"+process].SetTitle(processNameinDC[process])
           Shape_ups[syst+"_"+process].SetTitle(processNameinDC[process])

         else :        

           Shape_dns[syst+"_"+process].SetName(processNameinDC[process]+"_"+syst+"Down")
           Shape_ups[syst+"_"+process].SetName(processNameinDC[process]+"_"+syst+"Up")
           Shape_dns[syst+"_"+process].SetTitle(processNameinDC[process])
           Shape_ups[syst+"_"+process].SetTitle(processNameinDC[process])

         for i in range(0,Shape[process].GetXaxis().GetNbins()) :

            if(Shape[process].GetBinContent(i+1)<=pedestal) : 
               Shape_dns[syst+"_"+process].SetBinContent(i+1,pedestal)
            if(Shape[process].GetBinContent(i+1)<=pedestal) : 
               Shape_ups[syst+"_"+process].SetBinContent(i+1,pedestal)

       # MC statistical uncertainty
       if(process=='VVZReso' or process=='NonReso' or process=='ZJets'):

         Shape_dns["statUnc_"+process] = Shape[process].Clone()
         Shape_ups["statUnc_"+process] = Shape[process].Clone()

         Shape_dns["statUnc_"+process].SetName(processNameinDC[process]+"_"+processNameinDC[process]+"StatUnc"+cat+fs+"Down")
         Shape_ups["statUnc_"+process].SetName(processNameinDC[process]+"_"+processNameinDC[process]+"StatUnc"+cat+fs+"Up")

         if(process=='ZJets'):

           Shape_dns["statUnc_"+process].SetName(processNameinDC[process]+"_"+processNameinDC[process]+"StatUnc"+cat+"Down")
           Shape_ups["statUnc_"+process].SetName(processNameinDC[process]+"_"+processNameinDC[process]+"StatUnc"+cat+"Up")

         Shape_dns["statUnc_"+process].SetTitle(processNameinDC[process])
         Shape_ups["statUnc_"+process].SetTitle(processNameinDC[process])

         for i in range(1,Shape[process].GetXaxis().GetNbins()+1) :

           dn = max(pedestal,Shape[process].GetBinContent(i)-fabs(0.5*Shape[process].GetBinError(i)))
           up = Shape[process].GetBinContent(i)+0.5*fabs(Shape[process].GetBinError(i))

           if(Shape[process].GetBinContent(i)<=pedestal) :
             dn = 0.5*pedestal
             up = 2*pedestal

           Shape_dns["statUnc_"+process].SetBinContent(i,dn)
           Shape_ups["statUnc_"+process].SetBinContent(i,up)

           Shape_dns["statUncBin"+str(i)+"_"+process] = Shape[process].Clone()
           Shape_ups["statUncBin"+str(i)+"_"+process] = Shape[process].Clone()
           Shape_dns["statUncBin"+str(i)+"_"+process].SetName(processNameinDC[process]+"_"+processNameinDC[process]+"StatUncBin"+str(i)+cat+fs+"Down")
           Shape_ups["statUncBin"+str(i)+"_"+process].SetName(processNameinDC[process]+"_"+processNameinDC[process]+"StatUncBin"+str(i)+cat+fs+"Up")

           if(process=='ZJets'):
             Shape_dns["statUncBin"+str(i)+"_"+process].SetName(processNameinDC[process]+"_"+processNameinDC[process]+"StatUncBin"+str(i)+cat+"Down")
             Shape_ups["statUncBin"+str(i)+"_"+process].SetName(processNameinDC[process]+"_"+processNameinDC[process]+"StatUncBin"+str(i)+cat+"Up")

           Shape_dns["statUncBin"+str(i)+"_"+process].SetTitle(processNameinDC[process])
           Shape_ups["statUncBin"+str(i)+"_"+process].SetTitle(processNameinDC[process])

           for j in range(1,Shape[process].GetXaxis().GetNbins()+1) :

             if(j==i) :

               dn = max(pedestal,Shape[process].GetBinContent(j)-0.5*fabs(Shape[process].GetBinError(j)))
               up = Shape[process].GetBinContent(j)+0.5*fabs(Shape[process].GetBinError(j))

               if(Shape[process].GetBinContent(j)<=pedestal) :
                 dn = 0.5*pedestal
                 up = 2*pedestal

               Shape_dns["statUncBin"+str(i)+"_"+process].SetBinContent(j,dn)
               Shape_ups["statUncBin"+str(i)+"_"+process].SetBinContent(j,up)
               
               break

    #########
    Shape[sigName].Scale(0.001)
    for syst in systs :
      if(syst=='Recoil') :
        continue;
      Shape_dns[syst+"_"+sigName].Scale(0.001)
      Shape_ups[syst+"_"+sigName].Scale(0.001)

    theRates[sigName] = Shape[sigName].Integral()
    
    ## obs data
    data_obs_orig = (inputfile.Get("mT_data")).Clone()
    data_obs = TH1D(process, process, len(bins)-1,array('d',bins))
    rebinMerge(data_obs,data_obs_orig)

    data_obs.SetName("data_obs")
    data_obs.SetTitle("data_obs")
    Nobs = data_obs.Integral()

    #### out #### 
    outfile = TFile("Datacards/"+outdir+"/mX"+str(MX)+"ZZ2l2nu_"+channel+"_"+cat+".root","RECREATE")
    outfile.cd()

    for process in processes :

      Shape[process].Write()
      for syst in systs :

        if(process=='NonReso') :
           continue
        if(syst=="Recoil" and process!='ZJets' ):
           continue

        Shape_dns[syst+"_"+process].Write()
        Shape_ups[syst+"_"+process].Write()

      if(process=='VVZReso' or process=='NonReso' or process=='ZJets'):

        Shape_dns["statUnc_"+process].Write()
        Shape_ups["statUnc_"+process].Write()

        for i in range(1,Shape[process].GetXaxis().GetNbins()+1) :

            Shape_dns["statUncBin"+str(i)+"_"+process].Write()
            Shape_ups["statUncBin"+str(i)+"_"+process].Write()             

    data_obs.Write()
    outfile.Close()

    ## Write Datacards 
    fo = open("Datacards/"+outdir+"/mX"+str(MX)+"ZZ2l2nu_"+channel+"_"+cat+".txt", "wb")
    WriteDatacard(fo, sigName,MX,channel,cat, theRates,Nobs, perBinStatUnc)
    fo.close()

def WriteDatacard(theFile, sigName,MX,channel,cat, theRates,obsEvents, perBinStatUnc):
        numberSig = 1
        numberBg  = len(theRates)-1

        fs="el"
        if(channel=="mm") :
          fs = "mu"
        elif(channel=="ee") :
          fs = "el"

        theFile.write("imax 1\n")
        theFile.write("jmax {0}\n".format(numberSig+numberBg-1))
        theFile.write("kmax *\n")

        theFile.write("------------\n")
        theFile.write("shapes * * mX{0}ZZ2l2nu_{1}_{2}.root $PROCESS $PROCESS_$SYSTEMATIC \n".format(str(MX),channel,cat))
        theFile.write("------------\n")
        theFile.write("bin {0}_{1} \n".format(channel,cat))
        theFile.write("observation {0} \n".format(obsEvents))
        theFile.write("------------\n")
        theFile.write("bin ")

        processList = [sigName,'ZJets','VVZReso','NonReso']
        processName1D=['xzz'+str(MX),'zjets','vvreso','nonreso']

        for process in processList:
            theFile.write("{0}_{1} ".format(channel,cat))
        theFile.write("\n")

        theFile.write("process ")

        i=0
        for process in processList:
            theFile.write("{0} ".format(processName1D[i]))
            i+=1

        theFile.write("\n")

        processLine = "process "

        for x in range(-numberSig+1,1):
            processLine += "{0} ".format(x)

        for y in range(1,numberBg+1):
            processLine += "{0} ".format(y)

        theFile.write(processLine)
        theFile.write("\n")

        theFile.write("rate ")
        for process in processList:
            theFile.write("{0:.4f} ".format(theRates[process]))
        theFile.write("\n")
        theFile.write("------------\n")

        theFile.write("# norm uncertainty \n")
        theFile.write("lumi_13TeV lnN 1.06 1.06 1.06 - \n")
        theFile.write("pdf_qqbar lnN - 1.017 1.015 - \n")
        theFile.write("xzz2l2nu_accept lnN 1.01 - - - \n")
        theFile.write("trg{0} shapeN2 1.0 - 1.0 - \n".format(fs))
        theFile.write("id{0} shapeN2 1.0 - 1.0 - \n".format(fs))
        theFile.write("qcd shapeN2 - - 1.0 - \n")
        theFile.write("ewk shapeN2 - - 1.0 - \n")
        ## non-res and z+jets are data-driven now, no direct trig/id uncertainty
        theFile.write("#non-res syst unc \n")
        unc_tmp = 1.02
        theFile.write("idnonres{0} lnN  - - - {1}\n".format(fs,str(unc_tmp)))
        unc_tmp = 1.01
        if(fs=="el") :
          unc_tmp = 1.06
        else :
          unc_tmp = 1.013
        theFile.write("trgnonres{0} lnN  - - - {1}\n".format(fs,str(unc_tmp)))
        unc_tmp = 1.01
        if(fs=="el") :
          unc_tmp = 1.04
        else :
          unc_tmp = 1.024
        theFile.write("pdfnonres{0} lnN  - - - {1}\n".format(fs,str(unc_tmp)))
        unc_tmp = 1.01
        if(fs=="el") :
          unc_tmp = 1.067
        else :
          unc_tmp = 1.01
        theFile.write("closurenonres{0} lnN  - - - {1}\n".format(fs,str(unc_tmp)))

        theFile.write("#MET-induced MT unc \n")
        systsMT = ['JetEn','JetRes','MuonEn','ElectronEn','PhotonEn','Uncluster']
        for syst in systsMT :
           #signal and reso MT uncertainty
           theFile.write("{0} shapeN2 1.0 - 1.0 - \n".format(syst))
           # zjets MT uncertainty from phyMET (subtraction) in photon+jet
           theFile.write("phyMET{0} shapeN2 - 1.0 - - \n".format(syst))

        theFile.write("# photon+jets zpt reweight uncertainty \n")
        theFile.write("fidxsec{0} shapeN2 - 0.5 - - \n".format(fs))
        theFile.write("zjetsRecoil shapeN2 - 1.0 - - \n".format(syst))

        theFile.write("#Stat unc \n")
        if(not perBinStatUnc) :
           theFile.write("zjetsStatUnc{0} shapeN2 - 1.0 - - \n".format(cat))
           theFile.write("vvresoStatUnc{0}{1} shapeN2 - - 1.0 - \n".format(cat,fs))
           theFile.write("nonresoStatUnc{0}{1} shapeN2 - - - 1.0 \n".format(cat,fs))
        else :
           for i in range(1, len(bins)) :
               theFile.write("zjetsStatUncBin{0}{1} shapeN2 - 1.0 - - \n".format(str(i),cat))
               theFile.write("vvresoStatUncBin{0}{1}{2} shapeN2 - - - 1.0 \n".format(str(i),cat,fs))
               theFile.write("nonresoStatUncBin{0}{1}{2} shapeN2 - - - 1.0 \n".format(str(i),cat,fs))


def rebinMerge(hist,hist_orig) :

    Nbins = hist.GetXaxis().GetNbins()
    Nbins_orig = hist_orig.GetXaxis().GetNbins()

    for i in range(1,Nbins+1):

      lower_edge = hist.GetXaxis().GetBinLowEdge(i)
      higher_edege = hist.GetXaxis().GetBinUpEdge(i)

      bincontent = 0.0
      binerror = 0.0

      for j in range(1,Nbins_orig+1):  
     
        bincenter = hist_orig.GetXaxis().GetBinCenter(j)
        if(bincenter>=lower_edge and bincenter<higher_edege) :
          bincontent = bincontent + hist_orig.GetBinContent(j)
          binerror = binerror + hist_orig.GetBinError(j)*hist_orig.GetBinError(j)
        else :
          continue

      binerror = pow(binerror,0.5)

      hist.SetBinContent(i, bincontent)
      hist.SetBinError(i, binerror)

### Define function for processing of os command
def processCmd(cmd, quiet = 0):
    #print cmd
    #status, output = commands.getstatusoutput(cmd)
    #output = subprocess.check_output(cmd, shell=True)
    output = '\n'
    p = Popen(cmd, shell=True, stdout=PIPE, stderr=STDOUT,bufsize=-1)
    for line in iter(p.stdout.readline, ''):
        output=output+str(line)
        print line,
    p.stdout.close()
    if p.wait() != 0:
        raise RuntimeError("%r failed, exit status: %d" % (cmd, p.returncode))
    #if (status !=0 and not quiet):
    #    print 'Error in processing command:\n   ['+cmd+']'
    #    print 'Output:\n   ['+output+'] \n'
    if (not quiet):
        print 'Output:\n   ['+output+'] \n'
    return output

def Run():
    parser=parseOptions()
    (options,args) = parser.parse_args()

    runLimits=options.RunLimits

    cut=options.CutChain
    perBinStatUnc=options.PerBinStatUnc
    isDyMC=options.DyMC
    isGJets= (not isDyMC)

    zjetsMethod = 'mczjet'
    if( not isDyMC) :
      zjetsMethod = 'gjet'

    # import siganl samples
    _temp = __import__('signals', globals(), locals(), ['sigMasses'], -1)
    sigMasses = _temp.sigMasses

    for key, value in sigMasses.iteritems():
        #if(key.startswith('VBF')):
        #  continue 
        sigType = key
        print 'signal ',sigType
        mass = sigMasses[sigType]
        print 'mass ',mass

        outdir = zjetsMethod+'_'+cut
        if(perBinStatUnc) :
          outdir = outdir + '_perBinStatUnc'
        outdir = outdir + '_' + sigType

        for m in mass :
            createXSworkspace(sigType,m, "mm",'SR', parser)
            createXSworkspace(sigType,m, "ee",'SR', parser)
            createXSworkspace(sigType,m, "mm",'CR1', parser)
            createXSworkspace(sigType,m, "ee",'CR1', parser)
            if(runLimits) :
              cmd = 'sh Limits/runAsymptotic.sh Datacards/'+outdir+' '+str(m)
              print cmd
              output = processCmd(cmd)

if __name__ == "__main__":
    Run()
