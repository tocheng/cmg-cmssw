import optparse
import ROOT, sys, os, string, re
import math
from math import *

from ROOT import *
from array import array

from tdrStyle import *
setTDRStyle()

grootargs = []
def callback_rootargs(option, opt, value, parser):
    grootargs.append(opt)

parser = optparse.OptionParser()
parser.add_option("--cutChain",dest="cutChain",default='zpt100met50',help="kinematic cuts on zpt and met")
parser.add_option("--dyMC",action="store_true", dest="dyMC", default=False,help="whether use DY MC to predict Z+jets MT spectrum")
parser.add_option("-l",action="callback",callback=callback_rootargs)
parser.add_option("-q",action="callback",callback=callback_rootargs)
parser.add_option("-b",action="callback",callback=callback_rootargs)

def ReadTemplates(MX,channel,cat, parser):

    (options,args) = parser.parse_args()

    cut=options.cutChain
    isDyMC=options.dyMC
    isGJets= (not isDyMC)

    zjetsMethod = 'mczjet'
    if( not isDyMC) :
      zjetsMethod = 'gjet'

    fs=""
    if(channel=="mm") :
      fs = "mu"
    elif(channel=="ee") :
      fs = "el"

    #gjet_zpt100met50_BulkSpin2
    indir = zjetsMethod
    indir = indir + '_'
    indir = indir + cut + '_BulkGrav_narrow'

    #mX600ZZ2l2nu_ee_zpt100met50_SR.root
    inputfileName = 'mX'
    inputfileName = inputfileName +str(MX)
    inputfileName = inputfileName+'ZZ2l2nu_'
    inputfileName = inputfileName+channel
    inputfileName = inputfileName+'_'+cat
    inputfile = TFile("Datacards/"+indir+"/"+inputfileName+".root","READ")

    #####
    Shape = {}
    Shape_ups = {}
    Shape_dns = {}

    processes = ['nonreso','vvreso','zjets']#'xzz'+str(MX)]
    #process xzz zjets vvreso nonreso
    systs = ['fidxsec','id','trg','ewk','qcd','JetEn','JetRes','MuonEn','ElectronEn','PhotonEn','TauEn','Uncluster','Recoil']
    systsMT = ['JetEn','JetRes','MuonEn','ElectronEn','PhotonEn','TauEn','Uncluster','Recoil']
    systsFull = ['statUnc','fidxsec','id','trg','ewk','qcd','JetEn','JetRes','MuonEn','ElectronEn','PhotonEn','TauEn','Uncluster','Recoil']

    print 'import histograms from ',indir+"/"+inputfileName   

    for process in processes :

       Shape[process] = (inputfile.Get(process)).Clone()
       Shape[process].SetName(process)
       Shape[process].SetTitle(process)

       for syst in systs :

         if(process=='nonreso') :
           continue
         if(process!='zjets' and syst == 'Recoil' ) :
           continue

         if(syst=="trg" or syst=="id" or syst=="zpt" or syst=="fidxsec") :
           Shape_dns[syst+"_"+process] = (inputfile.Get(process+"_"+syst+fs+"Down")).Clone()
           Shape_ups[syst+"_"+process] = (inputfile.Get(process+"_"+syst+fs+"Up")).Clone()
         elif(syst in systsMT and process=='zjets'):        
           if(syst!='Recoil') :
             Shape_dns[syst+"_"+process] = (inputfile.Get(process+"_phyMET"+syst+"Down")).Clone()
             Shape_ups[syst+"_"+process] = (inputfile.Get(process+"_phyMET"+syst+"Up")).Clone()
           else :
             Shape_dns[syst+"_"+process] = (inputfile.Get(process+"_zjets"+syst+"Down")).Clone()
             Shape_ups[syst+"_"+process] = (inputfile.Get(process+"_zjets"+syst+"Up")).Clone()
         else :        
           Shape_dns[syst+"_"+process] = (inputfile.Get(process+"_"+syst+"Down")).Clone()
           Shape_ups[syst+"_"+process] = (inputfile.Get(process+"_"+syst+"Up")).Clone()

       # MC statistical uncertainty
       if(process=='vvreso' or process=='nonreso'):
         Shape_dns["statUnc_"+process] = (inputfile.Get(process+"_"+process+"StatUnc"+cat+fs+"Down")).Clone()
         Shape_ups["statUnc_"+process] = (inputfile.Get(process+"_"+process+"StatUnc"+cat+fs+"Up")).Clone()

       if(process=='zjets'):  
         Shape_dns["statUnc_"+process] = (inputfile.Get(process+"_"+process+"StatUnc"+cat+"Down")).Clone()
         Shape_ups["statUnc_"+process] = (inputfile.Get(process+"_"+process+"StatUnc"+cat+"Up")).Clone()

    #### out #### 
    os.system('mkdir -p Plots')
    for process in processes :

      r = Shape[process].Clone()
      r.SetName("r")
      r.Divide(Shape[process])
      r.SetLineColor(kBlack)
      Shape[process].SetLineColor(kBlack)
      Shape[process].SetFillColor(0)

      for syst in systsFull :

        if(syst!="statUnc" and process=='nonreso') :
           continue
        if(syst=="statUnc" and process=='xzz'+str(MX) ):
           continue       
        if(syst=="Recoil" and process!='zjets' ):
           continue 

        Shape_dns[syst+"_"+process].SetLineColor(0)
        Shape_ups[syst+"_"+process].SetLineColor(0)
        Shape_dns[syst+"_"+process].SetFillColor(0)
        Shape_ups[syst+"_"+process].SetFillColor(0)

        r_dn = Shape_dns[syst+"_"+process].Clone()
        r_up = Shape_ups[syst+"_"+process].Clone() 

        r_dn.SetName("rdn_"+syst)
        r_up.SetName("rup_"+syst)
        r_dn.Divide(Shape[process])
        r_up.Divide(Shape[process])

        r_dn.SetLineColor(kBlue)
        r_up.SetLineColor(kRed)

        c1 = TCanvas("c1","c1", 800, 800)
        c1.SetLogy()
        c1.SetBottomMargin(0.3)
        c1.SetRightMargin(0.03);

        Shape_ups[syst+"_"+process].SetMinimum(0.01)
        Shape_ups[syst+"_"+process].Draw("")
        Shape_ups[syst+"_"+process].GetXaxis().SetTitleSize(0)
        Shape_ups[syst+"_"+process].GetXaxis().SetLabelSize(0)

        Shape_dns[syst+"_"+process].Draw("same")
        Shape[process].Draw("same")

        legend = TLegend(.75,.75,.90,.90)
        legend.AddEntry(Shape[process], 'central', "l")
        legend.AddEntry(Shape_ups[syst+"_"+process], 'upALT: '+syst, "l")
        legend.AddEntry(Shape_dns[syst+"_"+process], 'dnALT: '+syst, "l")
        legend.SetShadowColor(0);
        legend.SetFillColor(0);
        legend.SetLineColor(0);
        legend.Draw("same")        

        gPad.RedrawAxis()

        pad = TPad("pad", "pad", 0.0, 0.0, 1.0, 1.0);
        pad.SetTopMargin(0.7);
        pad.SetRightMargin(0.03);
        pad.SetFillColor(0);
        pad.SetGridy(1);
        pad.SetFillStyle(0);
        pad.Draw();
        pad.cd(0);

        r_up.GetYaxis().SetTitleSize(0.04);
        r_up.GetYaxis().SetTitleOffset(1.8);
        r_up.GetYaxis().SetTitle("ALT-to-Centre")
        r_up.GetYaxis().CenterTitle();
        r_up.GetYaxis().SetLabelSize(0.03);
        r_up.SetMinimum(0.2);
        r_up.SetMaximum(2.0);

        r_up.Draw("")
        r_dn.Draw("same")
        r.Draw("same")

        if(syst in systsMT and process=='zjets'and syst!='Recoil') :
          c1.SaveAs('Plots/'+process+'_'+fs+'_phyMET'+syst+'_'+cat+'.pdf')
          c1.SaveAs('Plots/'+process+'_'+fs+'_phyMET'+syst+'_'+cat+'.png')
        else :
          c1.SaveAs('Plots/'+process+'_'+fs+'_'+syst+'_'+cat+'.pdf')
          c1.SaveAs('Plots/'+process+'_'+fs+'_'+syst+'_'+cat+'.png')

mass=[800]

for m in mass :

   ReadTemplates(m,"mm",'SR', parser)
   ReadTemplates(m,"ee",'SR', parser)

