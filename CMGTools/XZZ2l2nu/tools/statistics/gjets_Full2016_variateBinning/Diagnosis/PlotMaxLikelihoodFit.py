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
parser.add_option("-l",action="callback",callback=callback_rootargs)
parser.add_option("-q",action="callback",callback=callback_rootargs)
parser.add_option("-b",action="callback",callback=callback_rootargs)

def ReadTemplates(MX,channel):

    (options,args) = parser.parse_args()

    ## ee_CR mm_CR ee_SR mm_SR
    inputfile = TFile("mlfit.root","READ")

    #####
    Shape_prefit = {}
    Shape_fit_b = {}
    Shape_fit_s = {}
    processes = ['nonreso','vvreso','zjets','xzz'+str(MX),'total_background','total_signal','total']

    for process in processes :

       print process

       Shape_prefit[process] = (inputfile.Get('shapes_prefit/'+channel+'/'+process)).Clone()
       Shape_fit_b[process] = (inputfile.Get('shapes_fit_b/'+channel+'/'+process)).Clone()
       Shape_fit_s[process] = (inputfile.Get('shapes_fit_s/'+channel+'/'+process)).Clone()

    #### out #### 
    for process in processes :

        c1 = TCanvas("c1","c1", 800, 800)
        c1.SetLogy()
        c1.SetBottomMargin(0.3)
        c1.SetRightMargin(0.03);

        Shape_prefit[process].SetMaximum(100000)
        Shape_prefit[process].SetMinimum(0.01)

        Shape_prefit[process].SetFillColor(kBlue)
        Shape_prefit[process].SetFillStyle(3018)
        Shape_prefit[process].DrawCopy("e2")
        Shape_prefit[process].Draw("ep3same");

        Shape_fit_b[process].SetMarkerColor(kRed)
        Shape_fit_b[process].SetMarkerStyle(20)
        Shape_fit_b[process].Draw("ep3same")

        Shape_fit_s[process].SetMarkerColor(kBlue)
        Shape_fit_s[process].SetMarkerStyle(22)
        Shape_fit_s[process].Draw("ep3same")

        legend = TLegend(.75,.75,.90,.90)
        legend.AddEntry(Shape_prefit[process], 'prefit', "f")
        legend.AddEntry(Shape_fit_b[process], 'postfit b-only', "p")
        legend.AddEntry(Shape_fit_s[process], 'postfit s+b', "p")
        legend.SetShadowColor(0);
        legend.SetFillColor(0);
        legend.SetLineColor(0);
        legend.Draw("same")        

        c1.SaveAs(process+'_'+channel+'_m'+str(MX)+'.pdf')
        c1.SaveAs(process+'_'+channel+'_m'+str(MX)+'.png')

mass=[800]

for m in mass :

   ReadTemplates(m,"mm_SR")
   ReadTemplates(m,"ee_SR")

