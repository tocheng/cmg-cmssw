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
parser.add_option("--cutChain",dest="CutChain",default='zpt100met50',help="kinematic cuts on zpt and met")
parser.add_option("--perBinStatUnc",action="store_true", dest="PerBinStatUnc", default=False,help="whether do per-bin statistical uncertainty")
parser.add_option('--sigType',dest='SigType',type='string',default='BulkSpin2', help='Type of signal to run. If skip,will run Bulk spin-2')
parser.add_option("--dyMC",action="store_true", dest="DyMC", default=False,help="whether use DY MC to predict Z+jets MT spectrum")
parser.add_option("-l",action="callback",callback=callback_rootargs)
parser.add_option("-q",action="callback",callback=callback_rootargs)
parser.add_option("-b",action="callback",callback=callback_rootargs)

def ReadTemplates(channel,cat, parser):

    (options,args) = parser.parse_args()

    cut=options.CutChain
    perBinStatUnc=options.PerBinStatUnc
    isDyMC=options.DyMC
    isGJets= (not isDyMC)
    sigType=options.SigType

    fs=""
    if(channel=="mm") :
      fs = "mu"
    elif(channel=="ee") :
      fs = "el"

    zjetsMethod = 'mczjet'
    if( not isDyMC) :
      zjetsMethod = 'gjet'

    prefix = "GJets_GMCEtaWt_GMCPhPtWt_"+cat+"_puWeightsummer16_muoneg_"+zjetsMethod+"_metfilter_unblind"
    inputfileName = prefix + "_" + fs + "_log_1pb"
    inputfile = TFile('Templates/'+inputfileName+".root","READ")

    #####
    Shape = {}

    print 'import histograms from ',inputfileName   

    masses=[600,800,1000,1200,1400,1600,1800,2000]

    for mass in masses :

       process = 'BulkGravToZZToZlepZinv_narrow_'+str(mass)
       #process = 'GluGluHToZZTo2L2Nu_M'+str(mass)
       template = "mT_"+process
       print process
       Shape[process] = (inputfile.Get(template)).Clone()
       Shape[process].Scale(0.001)
       Shape[process].SetName(process)
       Shape[process].SetTitle(process)

    #### out #### 
    os.system('mkdir -p Plots')

    c1 = TCanvas("c1","c1", 800, 800)
    c1.SetLogy()
    c1.SetBottomMargin(0.3)
    c1.SetRightMargin(0.03);

    legend = TLegend(.75,.75,.90,.90)

    for mass in masses :

      process = 'BulkGravToZZToZlepZinv_narrow_'+str(mass)
      #process = 'GluGluHToZZTo2L2Nu_M'+str(mass)


      Shape[process].SetMarkerColor(0) 
      Shape[process].SetLineColor(1+int(mass-600)/200)
      if(mass==600) :
        Shape[process].SetMinimum(0.1)
        Shape[process].Draw("hist")
      else :
        Shape[process].Draw("histsame")      

      legend.AddEntry(Shape[process], 'mX'+str(mass), "l")

    legend.SetShadowColor(0);
    legend.SetFillColor(0);
    legend.SetLineColor(0);
    legend.Draw("same")        

    c1.SaveAs('Plots/BulkGrav_'+cut+'_'+fs+'_'+cat+'.pdf')
    c1.SaveAs('Plots/BulkGrav_'+cut+'_'+fs+'_'+cat+'.png')

ReadTemplates("mm",'SR', parser)
ReadTemplates("ee",'SR', parser)

