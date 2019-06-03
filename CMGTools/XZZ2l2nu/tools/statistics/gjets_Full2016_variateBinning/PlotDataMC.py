
import ROOT, string
import math
from math import *

from ROOT import *
from array import array

import sys, os, pwd, commands
from subprocess import *
import optparse, shlex, re

from tdrStyle import *
setTDRStyle()

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

    parser.add_option("-l",action="callback",callback=callback_rootargs)
    parser.add_option("-q",action="callback",callback=callback_rootargs)
    parser.add_option("-b",action="callback",callback=callback_rootargs)

    #(option, args) = parser.parse_args()
    return parser 

def plotDataMC(sigType, channel,cat, parser):
    
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
    if not os.path.exists("Plots/"+outdir):
       os.system('mkdir -p Plots/'+outdir)

    prefix = "GJets_GMCEtaWt_GMCPhPtWt_"+cat+"_puWeightsummer16_muoneg_"+zjetsMethod+"_metfilter_unblind"
    inputfileName = prefix + "_" + fs + "_log_1pb"
    inputfile = TFile('Templates/'+inputfileName+".root","READ")

    ## Templates and Systematic Uncertainties
    Shape = {}
    Shape_orig = {}
    Shape_ups = {}
    Shape_dns = {}

    Shape_prefit = {}

    processes = ['NonReso','VVZReso','ZJets', 'BulkGravToZZToZlepZinv_narrow_600', 'BulkGravToZZToZlepZinv_narrow_1000', 'BulkGravToZZToZlepZinv_narrow_1600']
    processNameinDC = {'NonReso':'nonreso','VVZReso':'vvreso','ZJets':'zjets', 'BulkGravToZZToZlepZinv_narrow_600':'mX600','BulkGravToZZToZlepZinv_narrow_1000':'mX1000','BulkGravToZZToZlepZinv_narrow_1600':'mX1600'}

    systs = ['fidxsec','id','trg','ewk','qcd','JetEn','JetRes','MuonEn','ElectronEn','PhotonEn','Uncluster','Recoil']
    systsMT = ['JetEn','JetRes','MuonEn','ElectronEn','PhotonEn','Uncluster','Recoil']

    bins = [0.0, 100.0, 150.0, 200.0, 250.0, 300.0, 350.0, 400.0, 450.0, 500.0, 550.0, 600.0, 650.0, 700.0, 750.0, 800.0, 850, 900, 1000, 1100, 1250, 1450, 1650, 1850.0, 3000.0]
    nbins = len(bins)-1
    print 'nbins ',nbins

    errorUps = [0.0] *nbins
    errorDns = [0.0] *nbins

    stack = THStack("stack","stack")    
    added_orig = TH1D('added_orig', 'total bkg',60, 0,3000)
    added = TH1D('added', 'total bkg',nbins,array('d',bins))

    for process in processes :

       Shape_orig[process] = (inputfile.Get("mT_"+process)).Clone()
       Shape[process] = TH1D(process, process, nbins, array('d',bins))
       rebinMerge(Shape[process],Shape_orig[process])  

       Shape[process].SetName(processNameinDC[process])
       Shape[process].SetTitle(processNameinDC[process])

       for i in range(0,Shape[process].GetXaxis().GetNbins()) :

          if(Shape[process].GetBinContent(i+1)<pedestal) :
            Shape[process].SetBinContent(i+1,pedestal)

       for syst in systs :

         if(process=='NonReso') :
           continue
         if(process!='Zjets' and process!='VVZReso') : 
           continue
         if(process!='ZJets' and syst == 'Recoil' ) :
           continue

         Shape_dns[syst+"_"+process] = TH1D(syst+"Up_"+process, process, len(bins)-1,array('d',bins))
         rebinMerge(Shape_dns[syst+"_"+process],inputfile.Get("mT_"+syst+"Dn_"+process))
         Shape_ups[syst+"_"+process] = TH1D(syst+"Dn_"+process, process, len(bins)-1,array('d',bins))
         rebinMerge(Shape_ups[syst+"_"+process],inputfile.Get("mT_"+syst+"Up_"+process))

         if(syst=="trg" or syst=="id" or syst=="fidxsec") :

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

         if(process=='VVZReso' or process=='ZJets'):
             for i in range(1,Shape[process].GetXaxis().GetNbins()+1) :
                 max_val = max(Shape_ups[syst+"_"+process].GetBinContent(i),Shape_dns[syst+"_"+process].GetBinContent(i))  
                 min_val = min(Shape_ups[syst+"_"+process].GetBinContent(i),Shape_dns[syst+"_"+process].GetBinContent(i))
                 error_up = fabs(max_val-Shape[process].GetBinContent(i))
                 error_dn = fabs(min_val-Shape[process].GetBinContent(i))
                 #errorUps[i-1] = pow(errorUps[i-1]*errorUps[i-1]+error_up*error_up,0.5)
                 #errorDns[i-1] = pow(errorDns[i-1]*errorDns[i-1]+error_dn*error_dn,0.5)

       # MC statistical uncertainty
       if(process=='VVZReso' or process=='NonReso' or process=='ZJets'):

         Shape_dns["statUnc_"+process] = Shape[process].Clone()
         Shape_ups["statUnc_"+process] = Shape[process].Clone()

         Shape_dns["statUnc_"+process].SetName(processNameinDC[process]+"_"+processNameinDC[process]+"StatUnc"+cat+fs+"Down")
         Shape_ups["statUnc_"+process].SetName(processNameinDC[process]+"_"+processNameinDC[process]+"StatUnc"+cat+fs+"Up")

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

           error_up = fabs(up-Shape[process].GetBinContent(i))
           error_dn = fabs(dn-Shape[process].GetBinContent(i))
           errorUps[i-1] = pow(errorUps[i-1]*errorUps[i-1]+error_up*error_up,0.5)
           errorDns[i-1] = pow(errorDns[i-1]*errorDns[i-1]+error_dn*error_dn,0.5)

    ## obs data
    data_obs_orig = (inputfile.Get("mT_data")).Clone()
    data_obs = TH1D(process, process, len(bins)-1,array('d',bins))
    rebinMerge(data_obs,data_obs_orig)
    blindData(data_obs_orig,500)

    # total uncertainty from prefit templates in MaxLikelihood
    processes = ['total_background']
    inputfileML = TFile("Diagnosis/mlfit.root","READ")
  
    for process in processes :
       print process
       Shape_prefit[process] = (inputfileML.Get('shapes_prefit/'+channel+'_'+cat+'/'+process)).Clone()

    errorUpsFull = [0.0] *nbins
    errorDnsFull = [0.0] *nbins
    for i in range(1,Shape_prefit['total_background'].GetXaxis().GetNbins()+1) :

        errorUpsFull[i-1] = Shape_prefit['total_background'].GetBinError(i)/Shape_prefit['total_background'].GetBinContent(i)
        errorDnsFull[i-1] = Shape_prefit['total_background'].GetBinError(i)/Shape_prefit['total_background'].GetBinContent(i)

    #plotting data/MC
    c1 = TCanvas("c1","c1", 800, 800)
    c1.SetLogy()
    c1.SetBottomMargin(0.3)
    c1.SetRightMargin(0.03);

    dummy = TH1D("dummy","dummy", 1, 0,2000)
    dummy.SetBinContent(1,0.0)
    dummy.GetXaxis().SetTitle('mT [GeV]')
    #dummy.GetYaxis().SetTitle('#sigma(pp#rightarrowH)#timesBR(H#rightarrowZZ) [pb]')
    dummy.SetLineColor(0)
    dummy.SetLineWidth(0)
    dummy.SetFillColor(0)
    dummy.SetMinimum(0.001)
    dummy.SetMaximum(1000000.0)
    dummy.GetXaxis().SetMoreLogLabels(kTRUE)
    dummy.GetXaxis().SetNoExponent(kTRUE)
    dummy.GetXaxis().SetTitleSize(0)
    dummy.GetXaxis().SetLabelSize(0)
    dummy.Draw()

    stack.Add(Shape_orig["NonReso"])
    stack.Add(Shape_orig["VVZReso"])
    stack.Add(Shape_orig["ZJets"])
    stack.Draw("histsame")

    stack.GetXaxis().SetMoreLogLabels(kTRUE)
    stack.GetXaxis().SetNoExponent(kTRUE)
    stack.GetXaxis().SetTitleSize(0)
    stack.GetXaxis().SetLabelSize(0)

    data_obs_orig.SetMarkerStyle(20)
    data_obs_orig.SetMarkerSize(1)
    data_obs_orig.SetBinErrorOption(TH1.kPoisson)
    data_obs_orig.Draw("ex0psame")

    Shape_orig["BulkGravToZZToZlepZinv_narrow_600"].Draw("histsame")
    Shape_orig["BulkGravToZZToZlepZinv_narrow_1000"].Draw("histsame")
    Shape_orig["BulkGravToZZToZlepZinv_narrow_1600"].Draw("histsame")

    legend = TLegend(.75,.75,.90,.90)
    legend.AddEntry(Shape_orig["ZJets"], 'ZJets(#gamma+jets data)', "f")
    legend.AddEntry(Shape_orig["VVZReso"], 'Z reson. (MC ZZ/WZ/TTZ)', "f")
    legend.AddEntry(Shape_orig["NonReso"], 'Non-reson. (e#mu data)', "f")
    legend.AddEntry(data_obs_orig, 'Data', "p")
    legend.SetShadowColor(0);
    legend.SetFillColor(0);
    legend.SetLineColor(0);
    legend.Draw("same")

    latex2 = TLatex()
    latex2.SetNDC()
    latex2.SetTextSize(0.6*c1.GetTopMargin())
    latex2.SetTextFont(42)
    latex2.SetTextAlign(31) # align right
    latex2.DrawLatex(0.92, 0.94," 35.6 fb^{-1} (13 TeV)")
    latex2.SetTextSize(0.4*c1.GetTopMargin())
    latex2.SetTextFont(52)
    latex2.SetTextAlign(11) # align right
    latex2.DrawLatex(0.2, 0.94, "CMS")
    latex2.SetTextSize(0.4*c1.GetTopMargin())
    latex2.SetTextFont(52)
    latex2.SetTextAlign(11)
    latex2.DrawLatex(0.3, 0.94, "Work in progress")

    gPad.RedrawAxis()

    pad = TPad("pad", "pad", 0.0, 0.0, 1.0, 1.0);
    pad.SetTopMargin(0.70);
    pad.SetRightMargin(0.03);
    pad.SetFillColor(0);
    #pad.SetGridy(1);
    pad.SetFillStyle(0);
    pad.Draw();
    pad.cd(0);

    dummyR = TH1D("dummyR","dummyR", 1, 0,2000)
    dummyR.SetBinContent(1,0.0)
    dummyR.GetXaxis().SetTitle('mT [GeV]')
    dummyR.SetMinimum(0.5)
    dummyR.SetMaximum(1.5)
    dummyR.GetXaxis().SetMoreLogLabels(kTRUE)
    dummyR.GetXaxis().SetNoExponent(kTRUE)
    dummyR.GetYaxis().SetTitleSize(0.04);
    dummyR.GetYaxis().SetTitleOffset(1.8);
    dummyR.GetYaxis().SetTitle("Data/MC");
    dummyR.GetYaxis().CenterTitle();
    dummyR.GetYaxis().SetLabelSize(0.03);
    dummyR.GetXaxis().SetMoreLogLabels(kTRUE)
    dummyR.GetXaxis().SetNoExponent(kTRUE)
    dummyR.GetXaxis().SetTitle("MT(GeV)")
    dummyR.Draw()

    ## Bkg uncertainty band
    errorp = array('d',[])
    r = array('d',[])
    errorm = array('d',[])
    mass = array('d',[])
    zeros = array('d',[])

    errorp_full = array('d',[])
    errorm_full = array('d',[])

    added.Add(Shape["NonReso"])
    added.Add(Shape["VVZReso"])
    added.Add(Shape["ZJets"])

    for i in range(1,added.GetXaxis().GetNbins()+1) :
           zeros.append(0.0)
           mass.append(added.GetXaxis().GetBinCenter(i))
           r.append(1)
           errorp.append(errorUps[i-1]/added.GetBinContent(i))
           errorm.append(errorDns[i-1]/added.GetBinContent(i))

           errorp_full.append(errorUpsFull[i-1])
           errorm_full.append(errorDnsFull[i-1])

    v_mass = TVectorD(len(mass),mass)
    v_zeros = TVectorD(len(zeros),zeros)
    v_errorm = TVectorD(len(errorm),errorm)
    v_r = TVectorD(len(r),r)
    v_errorp = TVectorD(len(errorp),errorp)
 
    v_errorm_full = TVectorD(len(errorm_full),errorm_full)
    v_errorp_full = TVectorD(len(errorp_full),errorp_full)

    gr_full = TGraphAsymmErrors(v_mass,v_r,v_zeros,v_zeros,v_errorm_full,v_errorp_full)
    gr_full.SetLineColor(kGreen)
    gr_full.SetFillColor(kGreen)
    gr_full.SetFillStyle(3003)
    gr_full.Draw("e3same")

    gr = TGraphAsymmErrors(v_mass,v_r,v_zeros,v_zeros,v_errorm,v_errorp)
    gr.SetLineColor(kCyan)
    gr.SetFillColor(kCyan)
    gr.SetFillStyle(3003)
    gr.Draw("e3same")


    added_orig.Add(Shape_orig["NonReso"])
    added_orig.Add(Shape_orig["VVZReso"])
    added_orig.Add(Shape_orig["ZJets"])

    ratio = data_obs_orig.Clone('ratio')
    ratio.Divide(added_orig)
    ratio.SetLineColor(1)
    ratio.SetMarkerColor(1)
    ratio.SetLineStyle(1)
    ratio.Draw("epsame");

    c1.SaveAs('Plots/'+outdir+'/'+inputfileName+'.pdf')
    c1.SaveAs('Plots/'+outdir+'/'+inputfileName+'.png')


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

def blindData(hist,threshold) :
    Nbins = hist.GetXaxis().GetNbins()
    for i in range(1,Nbins+1):
      lower_edge = hist.GetXaxis().GetBinLowEdge(i)
      if(lower_edge>=threshold):
        hist.SetBinContent(i, 0)
        hist.SetBinError(i, 0)

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

    sigType="BulkGrav"

    plotDataMC(sigType, "ee","SR", parser)
    plotDataMC(sigType, "mm","SR", parser)

if __name__ == "__main__":
    Run()
