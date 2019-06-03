#!/usr/bin/env python

import optparse
import ROOT
import os,sys, string, math, pickle
from CMGTools.XZZ2l2nu.plotting.TreePlotter import TreePlotter
from CMGTools.XZZ2l2nu.plotting.MergedPlotter import MergedPlotter
from CMGTools.XZZ2l2nu.plotting.StackPlotter import StackPlotter

grootargs = []
def callback_rootargs(option, opt, value, parser):
    grootargs.append(opt)

parser = optparse.OptionParser()
parser.add_option("-t","--tag",dest="tag",default='DataB2G_ICHEPcfg_',help="")
parser.add_option("--cutChain",dest="cutChain",default='tight',help="")
parser.add_option("--channel",dest="channel",default='mu',help="only works with --ZWeight")
parser.add_option("--LogY",action="store_true", dest="LogY", default=False, help="")
parser.add_option("--Blind",action="store_true", dest="Blind", default=False,help="")
parser.add_option("--test",action="store_true", dest="test", default=False,help="")
parser.add_option("--ZWeight",action="store_true", dest="ZWeight", default=False,help="")
parser.add_option("--SeparateProcess",action="store_true", dest="SeparateProcess", default=False,help="")
parser.add_option("--WtQCDToGJets",action="store_true", dest="WtQCDToGJets", default=False,help="")

parser.add_option("-l",action="callback",callback=callback_rootargs)
parser.add_option("-q",action="callback",callback=callback_rootargs)
parser.add_option("-b",action="callback",callback=callback_rootargs)





(options,args) = parser.parse_args()


tag=options.tag
cutChain=options.cutChain

channel=options.channel
ZWeight=options.ZWeight
LogY=options.LogY
test=options.test
SeparateProcess=options.SeparateProcess
WtQCDToGJets=options.WtQCDToGJets
doRhoScale=False
doVtxScale=False
doEtaScale=True
doPhPtScale=True
doMETPhiScale=True


if SeparateProcess: tag +='SepProc_'

scale='(1)'

if doRhoScale: 
    tag+="RhoWt_"
    scale+="*(0.32+0.42*TMath::Erf((rho-4.16)/4.58)+0.31*TMath::Erf((rho+115.00)/29.58))" # b2h rereco 36.1 fb-1

if doVtxScale:
    tag+="VtxWt_"
    scale+="*(0.807+0.007*nVert+-3.689e-05*nVert*nVert+6.730e-04*exp(2.500e-01*nVert))" # b2h rereco 33.59fb-1

if doEtaScale:
    tag+="EtaWt_"
    scale+="*(1.05*TMath::Gaus(llnunu_l1_eta,0.7,0.63)+1.05*TMath::Gaus(llnunu_l1_eta,-0.7,0.63)+0.94*TMath::Gaus(llnunu_l1_eta,2.02,0.34)+0.94*TMath::Gaus(llnunu_l1_eta,-2.02,0.34))"
#    scale=scale+"*(GJetsTrigEff)"
#    scale=scale+"*(0.87*TMath::Gaus(llnunu_l1_eta,0.65,0.56)+0.87*TMath::Gaus(llnunu_l1_eta,-0.65,0.56)+0.65*TMath::Gaus(llnunu_l1_eta,1.90,0.25)+0.65*TMath::Gaus(llnunu_l1_eta,-1.90,0.25))"

if doMETPhiScale: 
    tag+="METPhiWt_"
    scale+="*(0.41349*sin(1.1228*llnunu_l2_phi+1.429)+1.0818)"



if doPhPtScale:
    tag+="PhPtWt_"
    scale=scale+"*((-0.371771+0.0193019*pow(llnunu_l1_pt,1)-0.000119102*pow(llnunu_l1_pt,2)+3.90785e-07*pow(llnunu_l1_pt,3)-7.29192e-10*pow(llnunu_l1_pt,4)+7.7063e-13*pow(llnunu_l1_pt,5)-4.27744e-16*pow(llnunu_l1_pt,6)+9.61926e-20*pow(llnunu_l1_pt,7))*(llnunu_l1_pt<=900)+(0.723945)*(llnunu_l1_pt>900))"
    #scale=scale+"*((-0.105507+0.00821057*pow(llnunu_l1_pt,1)-2.70772e-05*pow(llnunu_l1_pt,2)+4.66026e-08*pow(llnunu_l1_pt,3)-4.51688e-11*pow(llnunu_l1_pt,4)+2.44653e-14*pow(llnunu_l1_pt,5)-6.89362e-18*pow(llnunu_l1_pt,6)+7.87089e-22*pow(llnunu_l1_pt,7))*(llnunu_l1_pt<=1300)+(0.689695)*(llnunu_l1_pt>1300))" # data trig eff


wt_qcd_to_gjets = "((1/(1.525e-01*exp(-6.201e-02*llnunu_l1_pt+5.999e+00)+-5.024e-04*llnunu_l1_pt+1.646e-01))*(llnunu_l1_pt<200)+(1/(8.801e-02*exp(-4.075e-02*llnunu_l1_pt+5.228e+00)-8.291e-05*llnunu_l1_pt+7.234e-02))*(llnunu_l1_pt>=200&&llnunu_l1_pt<600)+(44.2595)*(llnunu_l1_pt>=600))"

if WtQCDToGJets:
    tag+="WtQCDToGJets_"

outdir='plots_ph'

indir='/home/heli/XZZ/80X_20170202_GJets_light_Skim'
lumi=35.87
sepSig=True
doRatio=True
Blind=options.Blind
puWeight='puWeightsummer16'

if not os.path.exists(outdir): os.system('mkdir -p '+outdir)

tag = tag+cutChain+'_'
tag = tag+puWeight+'_'


if not Blind: tag = tag+'unblind_'

if LogY: tag = tag+'log_'

if ZWeight:
    tag +='ZWeight_'
    if   channel=='el': tag+='el'
    elif channel=='mu': tag+='mu'
    else:  tag+='all'

paveText="#sqrt{s} = 13 TeV 2016 L = "+"{:.4}".format(float(lumi))+" fb^{-1}"


cuts_loose='(1)'
cuts_tight='llnunu_l1_pt>50'
cuts_tightzptgt50lt100='llnunu_l1_pt>50&&llnunu_l1_pt<100'
cuts_tightzptgt100='llnunu_l1_pt>100'
cuts_SR="llnunu_l1_pt>100&&llnunu_l2_pt>50"
cuts_CR="llnunu_l1_pt>50&&!(llnunu_l1_pt>100&&llnunu_l2_pt>50)"
cuts_CR1="llnunu_l1_pt>100&&llnunu_l2_pt<50"
cuts_CR2="llnunu_l1_pt>50&&llnunu_l1_pt<100&&llnunu_l2_pt>50"
cuts_CR3="llnunu_l1_pt>50&&llnunu_l1_pt<100&&llnunu_l2_pt<50"
if cutChain=='loosecut': cuts=cuts_loose
elif cutChain=='tight': cuts=cuts_tight
elif cutChain=='tightzptgt50lt100': cuts=cuts_tightzptgt50lt100
elif cutChain=='tightzptgt100': cuts=cuts_tightzptgt100
elif cutChain=='SR': cuts=cuts_SR
elif cutChain=='CR': cuts=cuts_CR
elif cutChain=='CR1': cuts=cuts_CR1
elif cutChain=='CR2': cuts=cuts_CR2
elif cutChain=='CR3': cuts=cuts_CR3
else : cuts=cuts_loose

cuts = '('+cuts+')'


ROOT.gROOT.ProcessLine('.x tdrstyle.C') 


# parameters for GJets
gdataLumi=35.867*1000
gdataYield = 3402037584.2277574539
gdataFidXsec=gdataYield/gdataLumi
zjetsFidXsecAll = 72.31330890818101409
zjetsFidXsecEl =  1.8368830484768923217
zjetsFidXsecMu =  70.413868731825942859
zjetsFidXsecAll_up = 73.317313237038433726
zjetsFidXsecAll_dn = 71.318806854104892068
zjetsFidXsecEl_up = 1.9004022884222013801
zjetsFidXsecEl_dn = 1.7743842806529528389
zjetsFidXsecMu_up = 71.35227312468455807
zjetsFidXsecMu_dn = 69.483913323296292219
zjetsFidXsecLowLptAll = 1119.9216265291902346
zjetsFidXsecLowLptEl = 459.14012486577632899
zjetsFidXsecLowLptMu = 660.78150166340503802



############################################
#
#  Physical MET
#
############################################

################
# DYJets->LL
################
zllSamples = ['DYJetsToLL_M50_Ext']

zllPlotters=[]
for sample in zllSamples:
    zllPlotters.append(TreePlotter(sample, indir+'/'+sample+'.root','tree'))
    zllPlotters[-1].addCorrectionFactor('1./SumWeights','norm')
    zllPlotters[-1].addCorrectionFactor('xsec','xsec')
    zllPlotters[-1].addCorrectionFactor('genWeight','genWeight')
    zllPlotters[-1].addCorrectionFactor(puWeight,'puWeight')
    zllPlotters[-1].addCorrectionFactor(scale,'scale')
    zllPlotters[-1].setAlias('nbadmuon', '(!nllnunu)')


##################
# DYJets -> NuNu
##################
znnSamples = [
'ZJetsToNuNu_HT100to200_BIG',
'ZJetsToNuNu_HT200to400_BIG',
'ZJetsToNuNu_HT400to600_BIG',
'ZJetsToNuNu_HT600to800_BIG',
'ZJetsToNuNu_HT800t1200_BIG',
'ZJetsToNuNu_HT1200to2500_BIG',
'ZJetsToNuNu_HT2500toInf_BIG',
]

znnPlotters=[]
for sample in znnSamples:
    znnPlotters.append(TreePlotter(sample, indir+'/'+sample+'.root','tree'))
    znnPlotters[-1].addCorrectionFactor('1./SumWeights','norm')
    znnPlotters[-1].addCorrectionFactor('xsec','xsec')
    znnPlotters[-1].addCorrectionFactor('genWeight','genWeight')
    znnPlotters[-1].addCorrectionFactor(puWeight,'puWeight')
    znnPlotters[-1].addCorrectionFactor(scale,'scale')



#######################
# Zgamma->nu nu gamma
#######################

znngSamples = [
'ZNuNuGJetsGt40Lt130',
'ZNuNuGJetsGt130',
]

znngPlotters=[]
for sample in znngSamples:
    znngPlotters.append(TreePlotter(sample, indir+'/'+sample+'.root','tree'))
    znngPlotters[-1].addCorrectionFactor('1./SumWeights','norm')
    znngPlotters[-1].addCorrectionFactor('xsec','xsec')
    znngPlotters[-1].addCorrectionFactor('genWeight','genWeight')
    znngPlotters[-1].addCorrectionFactor(puWeight,'puWeight')
    znngPlotters[-1].addCorrectionFactor(scale,'scale')



###########################
# Wgamma-> l nu gamma
###########################
wlngSamples = [
'WGToLNuG',
]

wlngPlotters=[]
for sample in wlngSamples:
    wlngPlotters.append(TreePlotter(sample, indir+'/'+sample+'.root','tree'))
    wlngPlotters[-1].addCorrectionFactor('1./SumWeights','norm')
    wlngPlotters[-1].addCorrectionFactor('xsec','xsec')
    wlngPlotters[-1].addCorrectionFactor('genWeight','genWeight')
    wlngPlotters[-1].addCorrectionFactor(puWeight,'puWeight')
    wlngPlotters[-1].addCorrectionFactor(scale,'scale')



###############################
# WJets->lnu
###############################

wlnSamples = [
'WJetsToLNu_HT100to200_BIG',
'WJetsToLNu_HT200to400_BIG',
'WJetsToLNu_HT400to600_BIG',
'WJetsToLNu_HT600to800_BIG',
'WJetsToLNu_HT800to1200_BIG',
'WJetsToLNu_HT1200to2500_BIG',
'WJetsToLNu_HT2500toInf_BIG',
]

wlnPlotters=[]
for sample in wlnSamples:
    wlnPlotters.append(TreePlotter(sample, indir+'/'+sample+'.root','tree'))
    wlnPlotters[-1].addCorrectionFactor('1./SumWeights','norm')
    wlnPlotters[-1].addCorrectionFactor('xsec*2','xsec')
    wlnPlotters[-1].addCorrectionFactor('genWeight','genWeight')
    wlnPlotters[-1].addCorrectionFactor(puWeight,'puWeight')
    wlnPlotters[-1].addCorrectionFactor(scale,'scale')


################################
# single top, ttbar
################################

tSamples = [
'T_tch_powheg',
'TBar_tch_powheg',
'T_tWch',
'TBar_tWch',
'TGJets_BIG',
'TTGJets_BIG',
]


tPlotters=[]
for sample in tSamples:
    tPlotters.append(TreePlotter(sample, indir+'/'+sample+'.root','tree'))
    tPlotters[-1].addCorrectionFactor('1./SumWeights','norm')
    tPlotters[-1].addCorrectionFactor('xsec','xsec')
    tPlotters[-1].addCorrectionFactor('genWeight','genWeight')
    tPlotters[-1].addCorrectionFactor(puWeight,'puWeight')
    tPlotters[-1].addCorrectionFactor(scale,'scale')


#########################################
# all above  physical met adding together
##########################################

phymetPlotters = zllPlotters+znnPlotters+znngPlotters+wlngPlotters+wlnPlotters+tPlotters
#phymetPlotters = znnPlotters+znngPlotters+wlngPlotters+wlnPlotters+tPlotters




############################################
#
#  Instrumental MET
#
############################################

##########################
# GJets
##########################

gjetsSamples = [
'GJets_HT40to100_BIG',
'GJets_HT100to200_BIG',
'GJets_HT200to400_BIG',
'GJets_HT400to600_BIG',
'GJets_HT600toInf_BIG',
]

gjetsPlotters=[]
for sample in gjetsSamples:
    gjetsPlotters.append(TreePlotter(sample, indir+'/'+sample+'.root','tree'))
    gjetsPlotters[-1].addCorrectionFactor('1./SumWeights','norm')
    gjetsPlotters[-1].addCorrectionFactor('xsec','xsec')
    gjetsPlotters[-1].addCorrectionFactor('genWeight','genWeight')
    gjetsPlotters[-1].addCorrectionFactor(puWeight,'puWeight')
    gjetsPlotters[-1].addCorrectionFactor(scale,'scale')



##########################
# QCD
##########################

QCD_EMEnriched_samples = [
'QCD_Pt20to30_EMEnriched_BIG',
'QCD_Pt30to50_EMEnriched_BIG',
'QCD_Pt50to80_EMEnriched_BIG',
'QCD_Pt80to120_EMEnriched_BIG',
'QCD_Pt120to170_EMEnriched_BIG',
'QCD_Pt170to300_EMEnriched_BIG',
'QCD_Pt300toInf_EMEnriched_BIG',
]

qcdSamples=QCD_EMEnriched_samples
 
qcdPlotters=[]
for sample in qcdSamples:
    qcdPlotters.append(TreePlotter(sample, indir+'/'+sample+'.root','tree'))
    qcdPlotters[-1].addCorrectionFactor('1./SumWeights','norm')
    qcdPlotters[-1].addCorrectionFactor('xsec','xsec')
    qcdPlotters[-1].addCorrectionFactor('genWeight','genWeight')
    qcdPlotters[-1].addCorrectionFactor(puWeight,'puWeight')
    qcdPlotters[-1].addCorrectionFactor(scale,'scale')
    if WtQCDToGJets: qcdPlotters[-1].addCorrectionFactor(wt_qcd_to_gjets,'wt_qcd_to_gjets')




##############################################
# all above instrumental met adding together
##############################################

insmetPlotters = gjetsPlotters+qcdPlotters



############################################
#
# All above Instrumental + Physical MET
#
############################################

allmcPlotters = phymetPlotters+insmetPlotters

for i in range(len(allmcPlotters)) :
    if ZWeight:
        allmcPlotters[i].addCorrectionFactor(str(1/gdataFidXsec),'frac') # divided by g data fid-xsec
        allmcPlotters[i].addCorrectionFactor('GJetsRhoWeight','GJetsRhoWeight')
        if channel=='el' :
            allmcPlotters[i].addCorrectionFactor('GJetsZPtWeightEl','GJetsZPtWeight')
            allmcPlotters[i].addCorrectionFactor(str(zjetsFidXsecEl),'zjetsFidXsecEl')
            allmcPlotters[i].addCorrectionFactor('(1.16725)','GJetsNorm')
        elif channel=='mu' :
            allmcPlotters[i].addCorrectionFactor('GJetsZPtWeightMu','GJetsZPtWeight')
            allmcPlotters[i].addCorrectionFactor(str(zjetsFidXsecMu),'zjetsFidXsecMu')
            allmcPlotters[i].addCorrectionFactor('(1.15559)','GJetsNorm')
        else :
            allmcPlotters[i].addCorrectionFactor('GJetsZPtWeight','GJetsZPtWeight')
            allmcPlotters[i].addCorrectionFactor(str(zjetsFidXsecAll),'zjetsFidXsecAll')
            allmcPlotters[i].addCorrectionFactor('(1.15559)','GJetsNorm')






############################################
#
# gamma  Data 
#
############################################

gdataSamples = [
#'SinglePhoton_Run2016Full_ReReco_v2',
'SinglePhoton_Run2016Full_03Feb2017_v0',
]

gdataPlotters=[]
for sample in gdataSamples:
    gdataPlotters.append(TreePlotter(sample, indir+'/'+sample+'.root','tree'))
    gdataPlotters[-1].addCorrectionFactor('GJetsPreScaleWeight','prescale')
    if ZWeight:
        gdataPlotters[-1].addCorrectionFactor(str(lumi*1000),'GJetsLumi')
        gdataPlotters[-1].addCorrectionFactor(str(1/gdataYield),'GJetsNorm0')
        gdataPlotters[-1].addCorrectionFactor('GJetsRhoWeight','GJetsRhoWeight')
        if channel=='el' :
            gdataPlotters[-1].addCorrectionFactor('GJetsZPtWeightEl','GJetsZPtWeight')
            gdataPlotters[-1].addCorrectionFactor(str(zjetsFidXsecEl),'zjetsFidXsecEl')
            gdataPlotters[-1].addCorrectionFactor('(1.16725)','GJetsNorm')
        elif channel=='mu' :
            gdataPlotters[-1].addCorrectionFactor('GJetsZPtWeightMu','GJetsZPtWeight')
            gdataPlotters[-1].addCorrectionFactor(str(zjetsFidXsecMu),'zjetsFidXsecMu')
            gdataPlotters[-1].addCorrectionFactor('(1.15559)','GJetsNorm')
        else :
            gdataPlotters[-1].addCorrectionFactor('GJetsZPtWeight','GJetsZPtWeight')
            gdataPlotters[-1].addCorrectionFactor(str(zjetsFidXsecAll),'zjetsFidXsecAll')
            gdataPlotters[-1].addCorrectionFactor('(1.15559)','GJetsNorm')








############################################
#
#  Create Plotters
#
############################################


############################################
# Create MergedPlotters
############################################

#########################
# Physical MET
#########################

ZLL = MergedPlotter(zllPlotters)
ZLL.setFillProperties(1001,ROOT.kOrange)
ZNN = MergedPlotter(znnPlotters)
ZNN.setFillProperties(1001,ROOT.kMagenta)
ZNNG = MergedPlotter(znngPlotters)
ZNNG.setFillProperties(1001,ROOT.kMagenta+2)
WLNG = MergedPlotter(wlngPlotters)
WLNG.setFillProperties(1001,ROOT.kRed)
WLN = MergedPlotter(wlnPlotters)
WLN.setFillProperties(1001,ROOT.kYellow)
T = MergedPlotter(tPlotters)
T.setFillProperties(1001,ROOT.kAzure-9)

#########################
# All Physical MET
#########################

PhyMET = MergedPlotter(phymetPlotters)
PhyMET.setFillProperties(1001, ROOT.kOrange)


#########################
# Instrumental MET
#########################

GJETS = MergedPlotter(gjetsPlotters)
GJETS.setFillProperties(1001,ROOT.kBlue-6)
QCD = MergedPlotter(qcdPlotters)
QCD.setFillProperties(1001,ROOT.kGreen+2)

#########################
# All Instrumental MET
#########################

InsMET = MergedPlotter(insmetPlotters)
InsMET.setFillProperties(1001, ROOT.kGreen)



########################
# gamma Data
########################
GData = MergedPlotter(gdataPlotters)



############################################
# Create StackPlotter
############################################

Stack = StackPlotter(outTag=tag, outDir=outdir)
Stack.setPaveText(paveText)
Stack.addPlotter(GData, "data_obs", "#gamma Data", "data")
if not SeparateProcess:
    Stack.addPlotter(PhyMET, "PhyMET","Physical MET", "background")
    Stack.addPlotter(InsMET, "InsMET","Instrumental MET", "background")
else:
    Stack.addPlotter(ZNN, "ZNN","Z->#nu#nu", "background")
    Stack.addPlotter(ZNNG, "ZNNG","Z#gamma->#nu#nu#gamma", "background")
    Stack.addPlotter(T, "T","Top", "background")
    Stack.addPlotter(ZLL, "ZLL","Z->ll", "background")
    Stack.addPlotter(WLNG, "WGToLNuG","W#gamma->l#nu#gamma", "background")
    Stack.addPlotter(WLN, "WLN","W->l#nu", "background")
    Stack.addPlotter(GJETS, "GJETS","#gamma+jets", "background")
    Stack.addPlotter(QCD, "QCD","QCD", "background")

 
Stack.setLog(LogY)
Stack.doRatio(doRatio)



tag+='_'

if test: 
#    Stack.drawStack('llnunu_l1_pt', cuts, str(lumi*1000), 75, 0.0, 1500.0, titlex = "P_{T}(#gamma)", units = "GeV",output='zpt_low',outDir=outdir,separateSignal=sepSig)
#    Stack.drawStack('llnunu_l2_pt', cuts, str(lumi*1000), 25, 0, 1000, titlex = "MET", units = "GeV",output='met_low',outDir=outdir,separateSignal=sepSig,blinding=Blind,blindingCut=200)
#    Stack.drawStack('llnunu_l1_eta', cuts, str(lumi*1000), 100, -2.5, 2.5, titlex = "#eta(#gamma) ", units = "",output='zeta',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('llnunu_l2_pt*cos(llnunu_l2_phi-llnunu_l1_phi)', cuts, str(lumi*1000), 100, -500, 500.0, titlex = "MET_{#parallel}", units = "GeV",output='met_para',outDir=outdir,separateSignal=sepSig)
else: 
    Stack.drawStack('nVert', cuts, str(lumi*1000), 100, 0.0, 100.0, titlex = "N vertices", units = "",output='nVert',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('rho', cuts, str(lumi*1000), 55, 0.0, 55.0, titlex = "#rho", units = "",output='rho',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('llnunu_l1_pt', cuts, str(lumi*1000), 75, 0.0, 1500.0, titlex = "P_{T}(#gamma)", units = "GeV",output='zpt',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('llnunu_l1_pt', cuts, str(lumi*1000), 250, 50, 300.0, titlex = "P_{T}(#gamma)", units = "GeV",output='zpt_low',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('llnunu_l1_eta', cuts, str(lumi*1000), 100, -2.5, 2.5, titlex = "#eta(#gamma) ", units = "",output='zeta',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('llnunu_l1_phi', cuts, str(lumi*1000), 64, -3.2, 3.2, titlex = "#phi(#gamma)", units = "",output='zphi',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('llnunu_l2_pt', cuts, str(lumi*1000), 25, 0, 1000, titlex = "MET", units = "GeV",output='met_low',outDir=outdir,separateSignal=sepSig,blinding=Blind,blindingCut=200)
    Stack.drawStack('fabs(TVector2::Phi_mpi_pi(llnunu_l2_phi-llnunu_l1_phi))', cuts, str(lumi*1000), 50, 0, 5, titlex = "#Delta#phi(#gamma,MET)", units = "",output='dphiGMet',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('llnunu_l2_pt*cos(llnunu_l2_phi-llnunu_l1_phi)', cuts, str(lumi*1000), 100, -500, 500.0, titlex = "MET_{#parallel}", units = "GeV",output='met_para',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('llnunu_l2_pt*sin(llnunu_l2_phi-llnunu_l1_phi)', cuts, str(lumi*1000), 100, -500, 500.0, titlex = "MET_{#perp}", units = "GeV",output='met_perp',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('llnunu_l2_phi', cuts, str(lumi*1000), 100, -3.2, 3.2, titlex = "#phi(MET)", units = "",output='metPhi',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('llnunu_l2_sumEt', cuts, str(lumi*1000), 80, 0.0, 3000.0, titlex = "sumE_{T}", units = "GeV",output='metSumEt',outDir=outdir,separateSignal=sepSig)


Stack.closePSFile()
Stack.closeROOTFile()

