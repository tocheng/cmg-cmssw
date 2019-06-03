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
parser.add_option("--channel",dest="channel",default='mu',help="")
parser.add_option("--cutChain",dest="cutChain",default='tight',help="")
parser.add_option("--LogY",action="store_true", dest="LogY", default=False, help="")
parser.add_option("--Blind",action="store_true", dest="Blind", default=False,help="")
parser.add_option("--test",action="store_true", dest="test", default=False,help="")
parser.add_option("--dyGJets",action="store_true", dest="dyGJets", default=False,help="")
parser.add_option("--muoneg",action="store_true", dest="muoneg", default=False,help="")
parser.add_option("-l",action="callback",callback=callback_rootargs)
parser.add_option("-q",action="callback",callback=callback_rootargs)
parser.add_option("-b",action="callback",callback=callback_rootargs)


####################################

(options,args) = parser.parse_args()

tag=options.tag
cutChain=options.cutChain

# can be el or mu or both
channel=options.channel
LogY=options.LogY
test=options.test
DrawLeptons=False
doRhoScale=True
doGMCEtaScale=True
dyGJets=options.dyGJets
muoneg=options.muoneg

lepsf="trgsf*isosf*idsf"

g_scale='(1)'

if doRhoScale:
    tag+="RhoWt_"
    rho_scale = "*(0.32+0.42*TMath::Erf((rho-4.16)/4.58)+0.31*TMath::Erf((rho+115.00)/29.58))" # b2h rereco 36.1 fb-1
    lepsf += rho_scale
    g_scale += rho_scale

if doGMCEtaScale:
    tag+="GMCEtaWt_"
    g_scale=g_scale+"*(0.87*TMath::Gaus(llnunu_l1_eta,0.65,0.56)+0.87*TMath::Gaus(llnunu_l1_eta,-0.65,0.56)+0.65*TMath::Gaus(llnunu_l1_eta,1.90,0.25)+0.65*TMath::Gaus(llnunu_l1_eta,-1.90,0.25))"

outdir='plots_gjets_36p46'

indir='/datab/tocheng/XZZ/80X_20161029_light_Skim/'
indirGJets='/datab/tocheng/XZZ/80X_20161029_GJets_light_Skim/'
lumi=36.814
sepSig=True
doRatio=True
Blind=options.Blind
FakeData=False
UseMETFilter=True
SignalAll1pb=True
puWeight='puWeightmoriondMC'
DataHLT=True
k=1 # signal scale
ZPtWeight="ZPtWeight"

elChannel='((abs(llnunu_l1_l1_pdgId)==11||abs(llnunu_l1_l2_pdgId)==11)||(llnunu_l1_l1_pdgId==19801117))'
muChannel='((abs(llnunu_l1_l1_pdgId)==13||abs(llnunu_l1_l2_pdgId)==13)||(llnunu_l1_l1_pdgId==19801117))'

if not os.path.exists(outdir): os.system('mkdir -p '+outdir)

tag = tag+cutChain+'_'
tag = tag+puWeight+'_'

if doSys: tag = tag+"sys_"
if UseMETFilter: tag = tag+'metfilter_'
if not Blind: tag = tag+'unblind_'

tag = tag+channel+'_'
if LogY: tag = tag+'log_'
if SignalAll1pb:
    tag += '1pb'
else:
    tag += 'scale'+str(k)

paveText="#sqrt{s} = 13 TeV 2016 L = "+"{:.4}".format(float(lumi))+" fb^{-1}"

metfilter='(Flag_EcalDeadCellTriggerPrimitiveFilter&&Flag_HBHENoiseIsoFilter&&Flag_goodVertices&&Flag_HBHENoiseFilter&&Flag_globalTightHalo2016Filter&&Flag_eeBadScFilter)'

cuts_loose='(nllnunu)'
cuts_lepaccept="((abs(llnunu_l1_l1_pdgId)==13||abs(llnunu_l1_l2_pdgId)==13&&llnunu_l1_l1_pt>50&&abs(llnunu_l1_l1_eta)<2.4&&llnunu_l1_l2_pt>20&&abs(llnunu_l1_l2_eta)<2.4&&(llnunu_l1_l1_highPtID>0.99||llnunu_l1_l2_highPtID>0.99))"
cuts_lepaccept+="||(abs(llnunu_l1_l1_pdgId)==11||abs(llnunu_l1_l2_pdgId)==11&&llnunu_l1_l1_pt>115&&abs(llnunu_l1_l1_eta)<2.5&&llnunu_l1_l2_pt>35&&abs(llnunu_l1_l2_eta)<2.5)"
cuts_lepaccept+="||(llnunu_l1_l1_pdgId==19801117))"
cuts_zmass="(llnunu_l1_mass_to_plot>70&&llnunu_l1_mass_to_plot<110)"
cuts_zpt100="(llnunu_l1_pt>100)"
cuts_zpt150="(llnunu_l1_pt>150)"
cuts_met50="(llnunu_l2_pt_to_plot>50)"
cuts_met100="(llnunu_l2_pt_to_plot>100)"
cuts_met200="(llnunu_l2_pt_to_plot>200)"
cuts_loose_z="("+cuts_lepaccept+"&&"+cuts_zmass+")"
cuts_loose_zpt20="("+cuts_lepaccept+"&&"+cuts_zmass+"&&llnunu_l1_pt>20)"
cuts_loose_zpt50="("+cuts_lepaccept+"&&"+cuts_zmass+"&&llnunu_l1_pt>50)"
cuts_loose_zptgt50lt200="("+cuts_lepaccept+"&&"+cuts_zmass+"&&llnunu_l1_pt>50&&llnunu_l1_pt<200)"
cuts_loose_zll="("+cuts_lepaccept+"&&"+cuts_zmass+"&&"+cuts_zpt100+")"
cuts_loose_zpt150="("+cuts_lepaccept+"&&"+cuts_zmass+"&&"+cuts_zpt150+")"
cuts_loose_zll_met50="("+cuts_lepaccept+"&&"+cuts_zmass+"&&"+cuts_zpt100+"&&"+cuts_met50+")"
cuts_loose_zll_met100="("+cuts_lepaccept+"&&"+cuts_zmass+"&&"+cuts_zpt100+"&&"+cuts_met100+")"
cuts_loose_zll_met200="("+cuts_lepaccept+"&&"+cuts_zmass+"&&"+cuts_zpt100+"&&"+cuts_met200+")"


if cutChain=='loosecut': cuts=cuts_loose
elif cutChain=='tight': cuts=cuts_loose_z
elif cutChain=='tightzpt20': cuts=cuts_loose_zpt20
elif cutChain=='tightzpt50': cuts=cuts_loose_zpt50
elif cutChain=='tightzptgt50lt200': cuts=cuts_loose_zptgt50lt200
elif cutChain=='tightzpt100': cuts=cuts_loose_zll
elif cutChain=='tightzpt150': cuts=cuts_loose_zpt150
elif cutChain=='tightzpt100met50': cuts=cuts_loose_zll_met50
elif cutChain=='tightzpt100met100': cuts=cuts_loose_zll_met100
elif cutChain=='tightzpt100met200': cuts=cuts_loose_zll_met200
else : cuts=cuts_loose


if channel=='el': cuts = cuts+'&&'+elChannel
elif channel=='mu': cuts = cuts+'&&'+muChannel

if UseMETFilter:
    #cuts = '('+cuts+'&&'+metfilter+')'
    cuts = '('+cuts+')' # metfilter pre-applied in preskim

cuts = '('+cuts+')'

ROOT.gROOT.ProcessLine('.x tdrstyle.C') 

#########################################################################
## NON-RESONANCE
#########################################################################

### data-driven

nonresPlotters=[]
nonresCRPlotters=[]
nonresSamples = ['muonegtrgsf']
for sample in nonresSamples:
    nonresPlotters.append(TreePlotter(sample, indir+'/'+sample+'.root','tree'))
    nonresCRPlotters[-1].addCorrectionFactor('1','norm')
    if channel=='el' :
        nonresPlotters[-1].addCorrectionFactor('etrgsf', 'etrgsf')
        nonresPlotters[-1].addCorrectionFactor('0.377075586068', 'norm')
    elif channel=='mu' :
        nonresPlotters[-1].addCorrectionFactor('mtrgsf', 'mtrgsf')
        nonresPlotters[-1].addCorrectionFactor('0.602260745361', 'norm')
    else:
        nonresPlotters[-1].addCorrectionFactor('(etrgsf*0.377075586068+mtrgsf*0.602260745361)','norm')
    nonresPlotters[-1].addCorrectionFactor('1./({0}*1000)'.format(lumi), 'lumi')


NONRES = MergedPlotter(nonresPlotters)
NONRES.setFillProperties(1001,ROOT.kOrange)

NONRES_CR = MergedPlotter(nonresCRPlotters)

###############

wwPlotters=[]
wwSamples = ['WWTo2L2Nu','WWToLNuQQ_BIG','WZTo1L1Nu2Q']

for sample in wwSamples:
    wwPlotters.append(TreePlotter(sample, indir+'/'+sample+'.root','tree'))
    wwPlotters[-1].addCorrectionFactor('1./SumWeights','norm')
    wwPlotters[-1].addCorrectionFactor('xsec','xsec')
    wwPlotters[-1].addCorrectionFactor('genWeight','genWeight')
    wwPlotters[-1].addCorrectionFactor(puWeight,'puWeight')
    wwPlotters[-1].addCorrectionFactor(lepsf,'lepsf')

WW = MergedPlotter(wwPlotters)
WW.setFillProperties(1001,ROOT.kOrange)


ttPlotters=[]
ttSamples = ['TTTo2L2Nu','TTZToLLNuNu','TTWJetsToLNu']

for sample in ttSamples:
    ttPlotters.append(TreePlotter(sample, indir+'/'+sample+'.root','tree'))
    ttPlotters[-1].addCorrectionFactor('1./SumWeights','norm')
    ttPlotters[-1].addCorrectionFactor('xsec','xsec')
    ttPlotters[-1].addCorrectionFactor('genWeight','genWeight')
    ttPlotters[-1].addCorrectionFactor(puWeight,'puWeight')
    ttPlotters[-1].addCorrectionFactor(lepsf,'lepsf')

TT = MergedPlotter(ttPlotters)
TT.setFillProperties(1001,ROOT.kAzure-9)

########################################################################################

### di-boson

vvPlotters=[]
vvSamples = ['WZTo2L2Q','WZTo3LNu_AMCNLO',
'ZZTo2L2Nu',
'ZZTo2L2Q','ZZTo4L',
'ggZZTo2e2nu','ggZZTo2mu2nu']

for sample in vvSamples:
    vvPlotters.append(TreePlotter(sample, indir+'/'+sample+'.root','tree'))
    vvPlotters[-1].addCorrectionFactor('1/SumWeights','norm')
    vvPlotters[-1].addCorrectionFactor('genWeight','genWeight')
    vvPlotters[-1].addCorrectionFactor(puWeight,'puWeight')
    vvPlotters[-1].addCorrectionFactor(lepsf, 'lepsf')
    if sample == 'ZZTo2L2Nu' : vvPlotters[-1].addCorrectionFactor("(ZZEwkCorrWeight*ZZQcdCorrWeight)", 'nnlo')
    if 'ggZZTo2' in sample: vvPlotters[-1].addCorrectionFactor('0.01898','xsec')
    else: vvPlotters[-1].addCorrectionFactor('xsec','xsec')

VV = MergedPlotter(vvPlotters)
VV.setFillProperties(1001,ROOT.kMagenta)

## w+jets
wjetsPlotters=[]
wjetsSamples = ['WJetsToLNu']

for sample in wjetsSamples:
    wjetsPlotters.append(TreePlotter(sample, indir+'/'+sample+'.root','tree'))
    wjetsPlotters[-1].addCorrectionFactor('1./SumWeights','norm')
    wjetsPlotters[-1].addCorrectionFactor('xsec','xsec')
    wjetsPlotters[-1].addCorrectionFactor('genWeight','genWeight')
    wjetsPlotters[-1].addCorrectionFactor(puWeight,'puWeight')
    wjetsPlotters[-1].addCorrectionFactor(lepsf,'lepsf')

WJets = MergedPlotter(wjetsPlotters)
WJets.setFillProperties(1001,ROOT.kBlue-6)


# ZJets ###############################

# ZJets from GJets#

# parameters for GJets
gdataLumi=36.46*1000
gdataYield=3447971384.222
gdataFidXsec=gdataYield/gdataLumi

zjetsFidXsecAll=158.50536907646062446
zjetsFidXsecEl =2.0703970677230105757
zjetsFidXsecMu =156.43497200873761699

zjetsFidXsecAll_up=159.3345561421179184
zjetsFidXsecEl_up =2.1132602740550670006
zjetsFidXsecMu_up =157.22129586806292423
zjetsFidXsecAll_dn=157.67618228330914576
zjetsFidXsecEl_dn =2.0275338821661335054
zjetsFidXsecMu_dn =155.64864840114299227

### the GJets data
gdataPlotters=[]
gdataSamples = [
'SinglePhoton_Run2016B2H_ReReco_36p46',
]

for sample in gdataSamples:
    gdataPlotters.append(TreePlotter(sample, indirGJets+'/'+sample+'.root','tree'))
    gdataPlotters[-1].addCorrectionFactor('GJetsPreScaleWeight','GJetsPreScaleWeight')
    gdataPlotters[-1].addCorrectionFactor('GJetsRhoWeight','GJetsRhoWeight')
    gdataPlotters[-1].addCorrectionFactor(str(1/gdataYield),'GJetsNorm0')
    if channel=='el' :
        gdataPlotters[-1].addCorrectionFactor('GJetsZPtWeightEl','GJetsZPtWeight')
        gdataPlotters[-1].addCorrectionFactor(str(zjetsFidXsecEl),'zjetsFidXsecEl')
        gdataPlotters[-1].addCorrectionFactor('(0.999)','GJetsNorm')
    elif channel=='mu' :
        gdataPlotters[-1].addCorrectionFactor('GJetsZPtWeightMu','GJetsZPtWeight')
        gdataPlotters[-1].addCorrectionFactor(str(zjetsFidXsecMu),'zjetsFidXsecMu')
        gdataPlotters[-1].addCorrectionFactor('(1.12206)','GJetsNorm')
    else :
        gdataPlotters[-1].addCorrectionFactor('GJetsZPtWeight','GJetsZPtWeight')
        gdataPlotters[-1].addCorrectionFactor(str(zjetsFidXsecAll),'zjetsFidXsecAll')
        gdataPlotters[-1].addCorrectionFactor('(1.10104)','GJetsNorm')

# for GJets photon bkg subtraction
phymetPlotters=[]
phymetSamples = [
'G_DYJetsToLL_M50_reHLT',
'G_ZJetsToNuNu_HT100to200_BIG',
'G_ZJetsToNuNu_HT200to400_BIG',
'G_ZJetsToNuNu_HT400to600_BIG',
'G_ZJetsToNuNu_HT600to800_BIG',
'G_ZJetsToNuNu_HT800t1200_BIG',
'G_ZJetsToNuNu_HT1200to2500_BIG',
'G_ZJetsToNuNu_HT2500toInf_BIG',
'G_ZNuNuGJetsGt40Lt130',
'G_ZNuNuGJetsGt130',
'G_WGToLNuG',
'G_WJetsToLNu_HT100to200_BIG',
'G_WJetsToLNu_HT1200to2500_BIG',
'G_WJetsToLNu_HT200to400_BIG',
'G_WJetsToLNu_HT2500toInf_BIG',
'G_WJetsToLNu_HT400to600_BIG',
'G_WJetsToLNu_HT600to800_BIG',
'G_WJetsToLNu_HT800to1200_BIG',
'G_TToLeptons_tch_powheg',
'G_TBarToLeptons_tch_powheg',
'G_T_tWch',
'G_TBar_tWch',
'G_TGJets_BIG',
'G_TTGJets',
]

# all the factors below together normalized each process to the fraction of the process in the gjets data
#   fidxsec_i / fidxsec_total
# together with the gdata, we have:
#  [ fidxsec_total-Sum(fidxsec_i) ]/fidxsec_total * fidxsec_zjets * lumi = zjets_yields
# an additional scale factor GJetsNorm to absorbe the small difference.

for sample in phymetSamples:
    phymetPlotters.append(TreePlotter(sample, indirGJets+'/'+sample+'.root','tree'))
    phymetPlotters[-1].addCorrectionFactor('-1/SumWeights','norm') # negative weight for subtraction
    phymetPlotters[-1].addCorrectionFactor('xsec','xsec')
    phymetPlotters[-1].addCorrectionFactor('genWeight','genWeight')
    phymetPlotters[-1].addCorrectionFactor(puWeight,'puWeight')
    phymetPlotters[-1].addCorrectionFactor(g_scale,'scale')
    phymetPlotters[-1].addCorrectionFactor(str(1/gdataFidXsec),'frac') # divided by g data fid-xsec
    if channel=='el' :
        phymetPlotters[-1].addCorrectionFactor('GJetsZPtWeightEl','GJetsZPtWeight')
        phymetPlotters[-1].addCorrectionFactor(str(zjetsFidXsecEl),'zjetsFidXsecEl')
    elif channel=='mu' :
        phymetPlotters[-1].addCorrectionFactor('GJetsZPtWeightMu','GJetsZPtWeight')
        phymetPlotters[-1].addCorrectionFactor(str(zjetsFidXsecMu),'zjetsFidXsecMu')
    else :
        phymetPlotters[-1].addCorrectionFactor('GJetsZPtWeight','GJetsZPtWeight')
        phymetPlotters[-1].addCorrectionFactor(str(zjetsFidXsecAll),'zjetsFidXsecAll')

# the GJets plotter ie, ZJets from GJets is
# instrumental MET + (-1)true MET
gjetsPlotters = gdataPlotters+phymetPlotters

GJets = MergedPlotter(gjetsPlotters)
GJets.setFillProperties(1001,ROOT.kGreen+2)

### ZJets from MC ##############################

mczjetsPlotters=[]
mczjetsSamples = [
'DYJetsToLL_M50_BIG_ResBos_Rc36p22HLTPUcutEffSf',
]

for sample in mczjetsSamples:
    mczjetsPlotters.append(TreePlotter(sample, indir+'/'+sample+'.root','tree'))
    mczjetsPlotters[-1].addCorrectionFactor('1./SumWeights','norm')
    mczjetsPlotters[-1].addCorrectionFactor(ZPtWeight,'ZPtWeight')
    mczjetsPlotters[-1].addCorrectionFactor('(1921.8*3)','xsec') # FEWZ NNLO.results_z_m50_nnlo_inclusive_NNPDF30_nlo_as_0118
    mczjetsPlotters[-1].addCorrectionFactor('genWeight','genWeight')
    mczjetsPlotters[-1].addCorrectionFactor(puWeight,'puWeight')
    mczjetsPlotters[-1].addCorrectionFactor(lepsf,'lepsf')
    if channel=='el' :
        mczjetsPlotters[-1].addCorrectionFactor('(0.985054)','scale') #el ResBos
    elif channel=='mu' :
        mczjetsPlotters[-1].addCorrectionFactor('(1.11546)','scale') #mu ResBos
    else :
        mczjetsPlotters[-1].addCorrectionFactor('(1.11376)','scale') #all ResBos


MCZJets = MergedPlotter(mczjetsPlotters)
MCZJets.setFillProperties(1001,ROOT.kGreen+2)

##
# choose GJets or ZJets MC
if dyGJets:
  ZJets = GJets
else: 
  ZJets = MCZJets

####################################################################################

sigPlotters=[]
sigSamples = [
'BulkGravToZZToZlepZinv_narrow_600',
'BulkGravToZZToZlepZinv_narrow_800',
'BulkGravToZZToZlepZinv_narrow_1000',
'BulkGravToZZToZlepZinv_narrow_1200',
'BulkGravToZZToZlepZinv_narrow_1400',
'BulkGravToZZToZlepZinv_narrow_1600', 
'BulkGravToZZToZlepZinv_narrow_1800', 
'BulkGravToZZToZlepZinv_narrow_2000',
'BulkGravToZZToZlepZinv_narrow_2500',
'BulkGravToZZToZlepZinv_narrow_3000',
'BulkGravToZZToZlepZinv_narrow_3500', 
'BulkGravToZZToZlepZinv_narrow_4000', 
'BulkGravToZZToZlepZinv_narrow_4500', 
]


sigSampleNames = {
'BulkGravToZZToZlepZinv_narrow_600':str(k)+' x BulkG-600',
'BulkGravToZZToZlepZinv_narrow_800':str(k)+' x BulkG-800',
'BulkGravToZZToZlepZinv_narrow_1000':str(k)+' x BulkG-1000',
'BulkGravToZZToZlepZinv_narrow_1200':str(k)+' x BulkG-1200',
'BulkGravToZZToZlepZinv_narrow_1400':str(k)+' x BulkG-1400',
'BulkGravToZZToZlepZinv_narrow_1600':str(k)+' x BulkG-1600',
'BulkGravToZZToZlepZinv_narrow_1800':str(k)+' x BulkG-1800',
'BulkGravToZZToZlepZinv_narrow_2000':str(k)+' x BulkG-2000',
'BulkGravToZZToZlepZinv_narrow_2500':str(k)+' x BulkG-2500',
'BulkGravToZZToZlepZinv_narrow_3000':str(k)+' x BulkG-3000',
'BulkGravToZZToZlepZinv_narrow_3500':str(k)+' x BulkG-3500',
'BulkGravToZZToZlepZinv_narrow_4000':str(k)+' x BulkG-4000',
'BulkGravToZZToZlepZinv_narrow_4500':str(k)+' x BulkG-4500',
}

BulkGZZ2l2nuXsec = {
600:8.61578e-03,
800:1.57965e-03,
1000:4.21651e-04,
1200:1.39919e-04,
1400:5.32921e-05,
1600:2.24428e-05,
1800:1.01523e-05,
2000:4.86037e-06,
2500:9.08739e-07,
3000:1.98856e-07,
3500:4.87505e-08,
4000:1.25937e-08,
4500:1.0,
}

sigXsec = {
'BulkGravToZZToZlepZinv_narrow_600'  : BulkGZZ2l2nuXsec[600]*k,
'BulkGravToZZToZlepZinv_narrow_800'  : BulkGZZ2l2nuXsec[800]*k,
'BulkGravToZZToZlepZinv_narrow_1000' : BulkGZZ2l2nuXsec[1000]*k,
'BulkGravToZZToZlepZinv_narrow_1200' : BulkGZZ2l2nuXsec[1200]*k,
'BulkGravToZZToZlepZinv_narrow_1400' : BulkGZZ2l2nuXsec[1400]*k,
'BulkGravToZZToZlepZinv_narrow_1600' : BulkGZZ2l2nuXsec[1600]*k,
'BulkGravToZZToZlepZinv_narrow_1800' : BulkGZZ2l2nuXsec[1800]*k,
'BulkGravToZZToZlepZinv_narrow_2000' : BulkGZZ2l2nuXsec[2000]*k,
'BulkGravToZZToZlepZinv_narrow_2500' : BulkGZZ2l2nuXsec[2500]*k,
'BulkGravToZZToZlepZinv_narrow_3000' : BulkGZZ2l2nuXsec[3000]*k,
'BulkGravToZZToZlepZinv_narrow_3500' : BulkGZZ2l2nuXsec[3500]*k,
'BulkGravToZZToZlepZinv_narrow_4000' : BulkGZZ2l2nuXsec[4000]*k,
'BulkGravToZZToZlepZinv_narrow_4500' : BulkGZZ2l2nuXsec[4500]*k,
}

if SignalAll1pb:
    for sig in sigSamples:
        sigXsec[sig] = 1.0
        sigSampleNames[sig] = string.replace(sigSampleNames[sig], str(k)+' x', '1 pb')

for sample in sigSamples:
    sigPlotters.append(TreePlotter(sample, indir+'/'+sample+'.root','tree'))
    sigPlotters[-1].addCorrectionFactor('1./SumWeights','norm')
    sigPlotters[-1].addCorrectionFactor(str(sigXsec[sample]),'xsec')
    sigPlotters[-1].addCorrectionFactor('genWeight','genWeight')
    sigPlotters[-1].addCorrectionFactor(puWeight,'puWeight')
    sigPlotters[-1].addCorrectionFactor(lepsf,'lepsf')
    sigPlotters[-1].setFillProperties(0,ROOT.kWhite)
    # some plotting definition
    sigPlotters[-1].setAlias('llnunu_l1_mass_to_plot', 'llnunu_l1_mass')
    sigPlotters[-1].setAlias('llnunu_l2_pt_to_plot', 'llnunu_l2_pt')
    sigPlotters[-1].setAlias('llnunu_l2_phi_to_plot', 'llnunu_l2_phi')
    sigPlotters[-1].setAlias('llnunu_mt_to_plot', 'llnunu_mt')


##### E/Mu data #####
dataPlotters=[]
dataSamples = [
'SingleEMU_Run2016B2H_ReReco_36p22fbinv',
]
for sample in dataSamples:
    dataPlotters.append(TreePlotter(sample, indir+'/'+sample+'.root','tree'))
if DataHLT:
    dataPlotters[0].addCorrectionFactor('(HLT_MUv2||HLT_ELEv2)','HLT')

Data = MergedPlotter(dataPlotters)

# some plotting definition
Data.setAlias('llnunu_l1_mass_to_plot', 'llnunu_l1_mass')
Data.setAlias('llnunu_l2_pt_to_plot', 'llnunu_l2_pt')
Data.setAlias('llnunu_l2_phi_to_plot', 'llnunu_l2_phi')
Data.setAlias('llnunu_mt_to_plot', 'llnunu_mt')

# some plotting definition
WW.setAlias('llnunu_l1_mass_to_plot', 'llnunu_l1_mass')
WW.setAlias('llnunu_l2_pt_to_plot', 'llnunu_l2_pt')
WW.setAlias('llnunu_l2_phi_to_plot', 'llnunu_l2_phi')
WW.setAlias('llnunu_mt_to_plot', 'llnunu_mt')

TT.setAlias('llnunu_l1_mass_to_plot', 'llnunu_l1_mass')
TT.setAlias('llnunu_l2_pt_to_plot', 'llnunu_l2_pt')
TT.setAlias('llnunu_l2_phi_to_plot', 'llnunu_l2_phi')
TT.setAlias('llnunu_mt_to_plot', 'llnunu_mt')

VV.setAlias('llnunu_l1_mass_to_plot', 'llnunu_l1_mass')
VV.setAlias('llnunu_l2_pt_to_plot', 'llnunu_l2_pt')
VV.setAlias('llnunu_l2_phi_to_plot', 'llnunu_l2_phi')
VV.setAlias('llnunu_mt_to_plot', 'llnunu_mt')

NONRES.setAlias('llnunu_l1_mass_to_plot', 'llnunu_l1_mass')
NONRES.setAlias('llnunu_l2_pt_to_plot', 'llnunu_l2_pt')
NONRES.setAlias('llnunu_l2_phi_to_plot', 'llnunu_l2_phi')
NONRES.setAlias('llnunu_mt_to_plot', 'llnunu_mt')

if dyGJets and channel=='el':
    ZJets.setAlias('llnunu_l1_mass_to_plot', 'llnunu_l1_mass_el')
    ZJets.setAlias('llnunu_l2_pt_to_plot', 'llnunu_l2_pt_el')
    ZJets.setAlias('llnunu_l2_phi_to_plot', 'llnunu_l2_phi_el')
    ZJets.setAlias('llnunu_mt_to_plot', 'llnunu_mt_el')
elif dyGJets and channel=='mu':
    ZJets.setAlias('llnunu_l1_mass_to_plot', 'llnunu_l1_mass_mu')
    ZJets.setAlias('llnunu_l2_pt_to_plot', 'llnunu_l2_pt_mu')
    ZJets.setAlias('llnunu_l2_phi_to_plot', 'llnunu_l2_phi_mu')
    ZJets.setAlias('llnunu_mt_to_plot', 'llnunu_mt_mu')
else:
    ZJets.setAlias('llnunu_l1_mass_to_plot', 'llnunu_l1_mass')
    ZJets.setAlias('llnunu_l2_pt_to_plot', 'llnunu_l2_pt')
    ZJets.setAlias('llnunu_l2_phi_to_plot', 'llnunu_l2_phi')
    ZJets.setAlias('llnunu_mt_to_plot', 'llnunu_mt')

############

Stack = StackPlotter(outTag=tag, outDir=outdir)
Stack.setPaveText(paveText)
Stack.addPlotter(Data, "data_obs", "Data", "data")
if muoneg: 
    Stack.addPlotter(NONRES, "NONReso","non reson.", "background")
    Stack.addPlotter(NONRES_CR, "NONResoCR","DataCR", "dataCR")
else:
    Stack.addPlotter(WW, "NonReso","WW/WZ/WJets non-reson.", "background")
    Stack.addPlotter(TT, "TT","TT", "background")
Stack.addPlotter(VV, "VVZReso","ZZ WZ reson.", "background")
if dyGJets: Stack.addPlotter(ZJets, "ZJets","ZJets(#gamma+Jets)", "background")
else: Stack.addPlotter(ZJets, "ZJets","ZJets(MC)", "background")

for i in range(len(sigSamples)):
  sigPlotters[i].setLineProperties(2,ROOT.kRed+i,2)
  Stack.addPlotter(sigPlotters[i],sigSamples[i],sigSampleNames[sigSamples[i]],'signal')  

Stack.setLog(LogY)
Stack.doRatio(doRatio)

tag+='_'

Stack.drawStack('llnunu_mt', cuts, str(lumi*1000), 100, 0.0, 3000.0, titlex = "M_{T}", units = "GeV",output=tag+'mt',outDir=outdir,separateSignal=sepSig)

if doSys:
   ## trg
   for (plotter,typeP,label,name) in zip(Stack.plotters,Stack.types,Stack.labels,Stack.names):
       if ( typeP != "data" and  typeP != "dataCR"): 
          plotter.changeCorrectionFactor("trgsf_up*idisotrksf","lepsf")
   Stack.drawStack('llnunu_mt', cuts, str(lumi*1000), 300, 0.0, 3000.0, titlex = "M_{T}", units = "GeV",output=tag+'mt_trgUp',outDir=outdir,separateSignal=sepSig)

   for (plotter,typeP,label,name) in zip(Stack.plotters,Stack.types,Stack.labels,Stack.names):
       if ( typeP != "data" and  typeP != "dataCR"): 
          plotter.changeCorrectionFactor("trgsf_dn*idisotrksf","lepsf")
   Stack.drawStack('llnunu_mt', cuts, str(lumi*1000), 300, 0.0, 3000.0, titlex = "M_{T}", units = "GeV",output=tag
+'mt_trgDn',outDir=outdir,separateSignal=sepSig)

   ## id
   for (plotter,typeP,label,name) in zip(Stack.plotters,Stack.types,Stack.labels,Stack.names):
       if ( typeP != "data" and  typeP != "dataCR"):
          plotter.changeCorrectionFactor("trgsf*idisotrksf_up","lepsf")
   Stack.drawStack('llnunu_mt', cuts, str(lumi*1000), 300, 0.0, 3000.0, titlex = "M_{T}", units = "GeV",output=tag+'mt_idUp',outDir=outdir,separateSignal=sepSig)

   for (plotter,typeP,label,name) in zip(Stack.plotters,Stack.types,Stack.labels,Stack.names):
       if ( typeP != "data" and  typeP != "dataCR"):
          plotter.changeCorrectionFactor("trgsf*idisotrksf_up","lepsf")
   Stack.drawStack('llnunu_mt', cuts, str(lumi*1000), 300, 0.0, 3000.0, titlex = "M_{T}", units = "GeV",output=tag+'mt_idDn',outDir=outdir,separateSignal=sepSig)

    ######## MT Unc
    for (plotter,typeP,label,name) in zip(Stack.plotters,Stack.types,Stack.labels,Stack.names):
        if ( typeP != "data" and typeP != "dataCR"):
           plotter.changeCorrectionFactor("trgsf*idisotrksf","lepsf")

    for systs in {'JetEn','JetRes','MuonEn','ElectronEn','TauEn','PhotonEn','Uncluster'}
      Stack.drawStack('llnunu_mt_'+systs+'Up', cuts, str(lumi*1000), 300, 0.0, 3000.0, titlex = "M_{T}", units = "GeV",output=tag+'mt_'+systs+'Up',outDir=outdir,separateSignal=sepSig)
      Stack.drawStack('llnunu_mt_'+systs+'Dn', cuts, str(lumi*1000), 300, 0.0, 3000.0, titlex = "M_{T}", units = "GeV",output=tag+'mt_'+systs+'Dn',outDir=outdir,separateSignal=sepSig)

Stack.closePSFile()
Stack.closeROOTFile()

