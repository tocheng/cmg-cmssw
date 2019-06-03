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





(options,args) = parser.parse_args()

tag=options.tag
cutChain=options.cutChain

# can be el or mu or both
channel=options.channel
LogY=options.LogY
test=options.test
DrawLeptons=True
doRhoScale=True
doGMCEtaScale=True
doGMCPhPtScale=False
dyGJets=options.dyGJets
muoneg=options.muoneg

if test: DrawLeptons = False

#lepsf="(1)"
#lepsf="idsf"
#lepsf="trgsf"
#lepsf="isosf"
#lepsf="isosf*idsf"
lepsf="trgsf*isosf*idsf"
#lepsf="trgsf*isosf*idsf*trksf"


g_scale='(1)'
mc_scale='(1)'
zjets_scale='(1)'

if channel=='mu': 
    mc_scale='(1.14905419293)'
    zjets_scale='(1.0)'
    #zjets_scale='(0.98418652884)'
elif channel=='el': 
    mc_scale='(1.13338415211)'
    zjets_scale='(1.0)'
else: 
    mc_scale='(1.1484406619)'
    zjets_scale='(1)'

# non reso alpha
nonreso_alpha_el=1.0
nonreso_alpha_mu=1.0
#zpt>10
#nonreso_alpha_el=0.35360488202
#nonreso_alpha_mu=0.682444330736
# zpt>70
#nonreso_alpha_el=0.368708
#nonreso_alpha_mu=0.607408
# zpt>50
#nonreso_alpha_el=0.353651462281
#nonreso_alpha_mu=0.614219922129
# zpt>50 met<100
nonreso_alpha_el=0.332060243754
nonreso_alpha_mu=0.629043099213 

#

#
if doRhoScale:
    tag+="RhoWt_"
    rho_scale = "*(0.32+0.42*TMath::Erf((rho-4.16)/4.58)+0.31*TMath::Erf((rho+115.00)/29.58))" # b2h rereco 36.1 fb-1
    lepsf += rho_scale
    g_scale += rho_scale

if doGMCEtaScale:
    tag+="GMCEtaWt_"
    g_scale=g_scale+"*(0.87*TMath::Gaus(llnunu_l1_eta,0.65,0.56)+0.87*TMath::Gaus(llnunu_l1_eta,-0.65,0.56)+0.65*TMath::Gaus(llnunu_l1_eta,1.90,0.25)+0.65*TMath::Gaus(llnunu_l1_eta,-1.90,0.25))"

if doGMCPhPtScale:
    tag+="GMCPhPtWt_"
    g_scale=g_scale+"*((-1.06624+0.0580113*pow(llnunu_l1_pt,1)-5.09328e-4*pow(llnunu_l1_pt,2)+2.28513e-6*pow(llnunu_l1_pt,3)-6.03131e-9*pow(llnunu_l1_pt,4)+9.84946e-12*pow(llnunu_l1_pt,5)-1.00558e-14*pow(llnunu_l1_pt,6)+6.244e-18*pow(llnunu_l1_pt,7)-2.15543e-21*pow(llnunu_l1_pt,8)+3.17021e-25*pow(llnunu_l1_pt,9))*(llnunu_l1_pt<=1000)+(0.688060)*(llnunu_l1_pt>1000))"

outdir='plots'

indir='/home/heli/XZZ/80X_20161029_light_Skim/'
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

elChannel='((abs(llnunu_l1_l1_pdgId)==11||abs(llnunu_l1_l2_pdgId)==11)&&llnunu_l1_l1_pt>115&&abs(llnunu_l1_l1_eta)<2.5&&llnunu_l1_l2_pt>35&&abs(llnunu_l1_l2_eta)<2.5)'
muChannel='((abs(llnunu_l1_l1_pdgId)==13||abs(llnunu_l1_l2_pdgId)==13)&&llnunu_l1_l1_pt>50&&abs(llnunu_l1_l1_eta)<2.4&&llnunu_l1_l2_pt>20&&abs(llnunu_l1_l2_eta)<2.4&&(llnunu_l1_l1_highPtID>0.99||llnunu_l1_l2_highPtID>0.99))'
photonFakeID='(llnunu_l1_l1_pdgId==19801117)'

if not os.path.exists(outdir): os.system('mkdir -p '+outdir)


tag = tag+cutChain+'_'
tag = tag+puWeight+'_'

if muoneg: tag = tag+"muoneg_"
if dyGJets: tag = tag+"gjet_"

if UseMETFilter: tag = tag+'metfilter_'
if not Blind: tag = tag+'unblind_'

tag = tag+channel+'_'
if LogY: tag = tag+'log_'
if SignalAll1pb:
    tag += '1pb'
else:
    tag += 'scale'+str(k)

#tag += '_'


paveText="#sqrt{s} = 13 TeV 2016 L = "+"{:.4}".format(float(lumi))+" fb^{-1}"

metfilter='(Flag_EcalDeadCellTriggerPrimitiveFilter&&Flag_HBHENoiseIsoFilter&&Flag_goodVertices&&Flag_HBHENoiseFilter&&Flag_globalTightHalo2016Filter&&Flag_eeBadScFilter)'

cuts_loose='(nllnunu)'
cuts_lepaccept="("+elChannel+"||"+muChannel+"||"+photonFakeID+")"
if channel=="el" : cuts_lepaccept="("+elChannel+"||"+photonFakeID+")"
elif channel=="mu" : cuts_lepaccept="("+muChannel+"||"+photonFakeID+")"
cuts_zmass="(llnunu_l1_mass_to_plot>70&&llnunu_l1_mass_to_plot<110)"
cuts_zmass_50_180="(llnunu_l1_mass_to_plot>50&&llnunu_l1_mass_to_plot<180)"
cuts_zpt100="(llnunu_l1_pt>100)"
cuts_zpt150="(llnunu_l1_pt>150)"
cuts_zpt200="(llnunu_l1_pt>200)"
cuts_met50="(llnunu_l2_pt_to_plot>50)"
cuts_met100="(llnunu_l2_pt_to_plot>100)"
cuts_met200="(llnunu_l2_pt_to_plot>200)"
cuts_loose_z="("+cuts_lepaccept+"&&"+cuts_zmass+")"
cuts_loose_zpt20="("+cuts_lepaccept+"&&"+cuts_zmass+"&&llnunu_l1_pt>20)"
cuts_loose_zpt50="("+cuts_lepaccept+"&&"+cuts_zmass+"&&llnunu_l1_pt>50)"
cuts_loose_zptgt50lt200="("+cuts_lepaccept+"&&"+cuts_zmass+"&&llnunu_l1_pt>50&&llnunu_l1_pt<200)"
cuts_loose_zll="("+cuts_lepaccept+"&&"+cuts_zmass+"&&"+cuts_zpt100+")"
cuts_loose_zpt150="("+cuts_lepaccept+"&&"+cuts_zmass+"&&"+cuts_zpt150+")"
cuts_loose_zpt200="("+cuts_lepaccept+"&&"+cuts_zmass+"&&"+cuts_zpt200+")"
cuts_loose_zll_met50="("+cuts_lepaccept+"&&"+cuts_zmass+"&&"+cuts_zpt100+"&&"+cuts_met50+")"
cuts_loose_zll_met100="("+cuts_lepaccept+"&&"+cuts_zmass+"&&"+cuts_zpt100+"&&"+cuts_met100+")"
cuts_loose_zll_met200="("+cuts_lepaccept+"&&"+cuts_zmass+"&&"+cuts_zpt100+"&&"+cuts_met200+")"
cuts_nonreso_zptgt0="("+cuts_lepaccept+"&&!"+cuts_zmass+"&&"+cuts_zmass_50_180+")"
cuts_nonreso_zptgt10="("+cuts_lepaccept+"&&!"+cuts_zmass+"&&"+cuts_zmass_50_180+"&&llnunu_l1_pt>10)"
cuts_nonreso_zptgt20="("+cuts_lepaccept+"&&!"+cuts_zmass+"&&"+cuts_zmass_50_180+"&&llnunu_l1_pt>20)"
cuts_nonreso_zptgt30="("+cuts_lepaccept+"&&!"+cuts_zmass+"&&"+cuts_zmass_50_180+"&&llnunu_l1_pt>30)"
cuts_nonreso_zptgt40="("+cuts_lepaccept+"&&!"+cuts_zmass+"&&"+cuts_zmass_50_180+"&&llnunu_l1_pt>40)"
cuts_nonreso_zptgt50="("+cuts_lepaccept+"&&!"+cuts_zmass+"&&"+cuts_zmass_50_180+"&&llnunu_l1_pt>50)"
cuts_nonreso_zptgt50_metlt20="("+cuts_lepaccept+"&&!"+cuts_zmass+"&&"+cuts_zmass_50_180+"&&llnunu_l1_pt>50&&llnunu_l2_pt_to_plot<20)"
cuts_nonreso_zptgt50_metlt30="("+cuts_lepaccept+"&&!"+cuts_zmass+"&&"+cuts_zmass_50_180+"&&llnunu_l1_pt>50&&llnunu_l2_pt_to_plot<30)"
cuts_nonreso_zptgt50_metlt50="("+cuts_lepaccept+"&&!"+cuts_zmass+"&&"+cuts_zmass_50_180+"&&llnunu_l1_pt>50&&llnunu_l2_pt_to_plot<50)"
cuts_nonreso_zptgt50_metlt100="("+cuts_lepaccept+"&&!"+cuts_zmass+"&&"+cuts_zmass_50_180+"&&llnunu_l1_pt>50&&llnunu_l2_pt_to_plot<100)"
cuts_CR="("+cuts_lepaccept+"&&"+cuts_zmass+"&&llnunu_l1_pt>50&&!(llnunu_l1_pt>100&&llnunu_l2_pt_to_plot>50))"
cuts_CR1="("+cuts_lepaccept+"&&"+cuts_zmass+"&&llnunu_l1_pt>100&&llnunu_l2_pt_to_plot<50)"
cuts_CR2="("+cuts_lepaccept+"&&"+cuts_zmass+"&&llnunu_l1_pt>50&&llnunu_l1_pt<100&&llnunu_l2_pt_to_plot>50)"
cuts_CR3="("+cuts_lepaccept+"&&"+cuts_zmass+"&&llnunu_l1_pt>50&&llnunu_l1_pt<100&&llnunu_l2_pt_to_plot<50)"

if cutChain=='loosecut': cuts=cuts_loose
elif cutChain=='tight': cuts=cuts_loose_z
elif cutChain=='tightzpt20': cuts=cuts_loose_zpt20
elif cutChain=='tightzpt50': cuts=cuts_loose_zpt50
elif cutChain=='tightzptgt50lt200': cuts=cuts_loose_zptgt50lt200
elif cutChain=='tightzpt100': cuts=cuts_loose_zll
elif cutChain=='tightzpt150': cuts=cuts_loose_zpt150
elif cutChain=='tightzpt200': cuts=cuts_loose_zpt200
elif cutChain=='tightzpt100met50': cuts=cuts_loose_zll_met50
elif cutChain=='tightzpt100met100': cuts=cuts_loose_zll_met100
elif cutChain=='tightzpt100met200': cuts=cuts_loose_zll_met200
elif cutChain=='nonreso_zptgt0': cuts=cuts_nonreso_zptgt0
elif cutChain=='nonreso_zptgt10': cuts=cuts_nonreso_zptgt10
elif cutChain=='nonreso_zptgt20': cuts=cuts_nonreso_zptgt20
elif cutChain=='nonreso_zptgt30': cuts=cuts_nonreso_zptgt30
elif cutChain=='nonreso_zptgt40': cuts=cuts_nonreso_zptgt40
elif cutChain=='nonreso_zptgt50': cuts=cuts_nonreso_zptgt50
elif cutChain=='nonreso_zptgt50_metlt20': cuts=cuts_nonreso_zptgt50_metlt20
elif cutChain=='nonreso_zptgt50_metlt30': cuts=cuts_nonreso_zptgt50_metlt30
elif cutChain=='nonreso_zptgt50_metlt50': cuts=cuts_nonreso_zptgt50_metlt50
elif cutChain=='nonreso_zptgt50_metlt100': cuts=cuts_nonreso_zptgt50_metlt100
elif cutChain=='CR': cuts=cuts_CR
elif cutChain=='CR1': cuts=cuts_CR1
elif cutChain=='CR2': cuts=cuts_CR2
elif cutChain=='CR3': cuts=cuts_CR3
else : cuts=cuts_loose


if UseMETFilter:
    #cuts = '('+cuts+'&&'+metfilter+')'
    cuts = '('+cuts+')' # metfilter pre-applied in preskim

cuts = '('+cuts+')'

ROOT.gROOT.ProcessLine('.x tdrstyle.C') 




#################

if channel=="el": 
    emuscale="(etrgsf*"+str(nonreso_alpha_el)+")"
elif channel=="mu": 
    emuscale="(mtrgsf*"+str(nonreso_alpha_mu)+")"
else: 
    emuscale="(etrgsf*"+str(nonreso_alpha_el)+"+mtrgsf*"+str(nonreso_alpha_mu)+")"

nonresSamples = [
#'muonegtrgsf'
'muonegtree_light_skim'
]
nonresPlotters=[]
for sample in nonresSamples:
    nonresPlotters.append(TreePlotter(sample, indir+'/'+sample+'.root','tree'))
    nonresPlotters[-1].addCorrectionFactor(str(0.001/lumi), 'norm')
    nonresPlotters[-1].addCorrectionFactor(emuscale, 'emuscale')
    nonresPlotters[-1].addCorrectionFactor(mc_scale,'mc_scale')

NONRES = MergedPlotter(nonresPlotters)
NONRES.setFillProperties(1001,ROOT.kOrange)


wwSamples = ['WWTo2L2Nu','WWToLNuQQ_BIG','WZTo1L1Nu2Q']
wwPlotters=[]
for sample in wwSamples:
    wwPlotters.append(TreePlotter(sample, indir+'/'+sample+'.root','tree'))
    wwPlotters[-1].addCorrectionFactor('1./SumWeights','norm')
    wwPlotters[-1].addCorrectionFactor('xsec','xsec')
    wwPlotters[-1].addCorrectionFactor('genWeight','genWeight')
    wwPlotters[-1].addCorrectionFactor(puWeight,'puWeight')
    wwPlotters[-1].addCorrectionFactor(lepsf,'lepsf')
    wwPlotters[-1].addCorrectionFactor(mc_scale,'mc_scale')

WW = MergedPlotter(wwPlotters)
WW.setFillProperties(1001,ROOT.kOrange)


vvSamples = ['WZTo2L2Q','WZTo3LNu_AMCNLO',
'ZZTo2L2Nu',
'ZZTo2L2Q','ZZTo4L',
'ggZZTo2e2nu','ggZZTo2mu2nu']

vvPlotters=[]
for sample in vvSamples:
    vvPlotters.append(TreePlotter(sample, indir+'/'+sample+'.root','tree'))
    vvPlotters[-1].addCorrectionFactor('1/SumWeights','norm')
    vvPlotters[-1].addCorrectionFactor('genWeight','genWeight')
    vvPlotters[-1].addCorrectionFactor(puWeight,'puWeight')
    vvPlotters[-1].addCorrectionFactor(lepsf, 'lepsf')
    vvPlotters[-1].addCorrectionFactor(mc_scale,'mc_scale')
    if sample == 'ZZTo2L2Nu' : vvPlotters[-1].addCorrectionFactor("(ZZEwkCorrWeight*ZZQcdCorrWeight*xsec)", 'nnlo')
    if 'ggZZTo2' in sample: vvPlotters[-1].addCorrectionFactor('0.01898','xsec')
    else: vvPlotters[-1].addCorrectionFactor('xsec','xsec')

VV = MergedPlotter(vvPlotters)
VV.setFillProperties(1001,ROOT.kMagenta)


wjetsPlotters=[]
wjetsSamples = ['WJetsToLNu']

for sample in wjetsSamples:
    wjetsPlotters.append(TreePlotter(sample, indir+'/'+sample+'.root','tree'))
    wjetsPlotters[-1].addCorrectionFactor('1./SumWeights','norm')
    wjetsPlotters[-1].addCorrectionFactor('xsec','xsec')
    wjetsPlotters[-1].addCorrectionFactor('genWeight','genWeight')
    wjetsPlotters[-1].addCorrectionFactor(puWeight,'puWeight')
    wjetsPlotters[-1].addCorrectionFactor(lepsf,'lepsf')
    wjetsPlotters[-1].addCorrectionFactor(mc_scale,'mc_scale')

WJets = MergedPlotter(wjetsPlotters)
WJets.setFillProperties(1001,ROOT.kBlue-6)



################################
# ZJets


# parameters for GJets


gdataLumi=36.46*1000
gdataYield=3451449849.011390686 
gdataFidXsec=gdataYield/gdataLumi

zjetsFidXsecAll = 151.06068438939382759
zjetsFidXsecEl =  1.8318217140038339785
zjetsFidXsecMu =  149.22886267539001892
zjetsFidXsecAll_up = 151.85715853322426483
zjetsFidXsecAll_dn = 150.26421019455997907
zjetsFidXsecEl_up = 1.8728979304188486665
zjetsFidXsecEl_dn = 1.7907454975888201787
zjetsFidXsecMu_up = 149.98426060280544903
zjetsFidXsecMu_dn = 148.47346469697114912
zjetsFidXsecLowLptAll = 807.42655018368884612
zjetsFidXsecLowLptEl = 229.77648821257676559
zjetsFidXsecLowLptMu = 577.65006197098625762


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
    phymetPlotters.append(TreePlotter(sample, indir+'/'+sample+'.root','tree'))
    phymetPlotters[-1].addCorrectionFactor('-1/SumWeights','norm') # negative weight for subtraction
    phymetPlotters[-1].addCorrectionFactor('xsec','xsec')
    phymetPlotters[-1].addCorrectionFactor('genWeight','genWeight')
    phymetPlotters[-1].addCorrectionFactor(puWeight,'puWeight')
    phymetPlotters[-1].addCorrectionFactor(g_scale,'scale')
    phymetPlotters[-1].addCorrectionFactor(str(1/gdataFidXsec),'frac') # divided by g data fid-xsec
    phymetPlotters[-1].addCorrectionFactor(mc_scale,'mc_scale')
    phymetPlotters[-1].addCorrectionFactor(zjets_scale,'zjets_scale')
    if channel=='el' :
        phymetPlotters[-1].addCorrectionFactor('GJetsZPtWeightEl','GJetsZPtWeight')
        phymetPlotters[-1].addCorrectionFactor(str(zjetsFidXsecEl),'zjetsFidXsecEl')
    elif channel=='mu' :
        phymetPlotters[-1].addCorrectionFactor('GJetsZPtWeightMu','GJetsZPtWeight')
        phymetPlotters[-1].addCorrectionFactor(str(zjetsFidXsecMu),'zjetsFidXsecMu')
    else :
        phymetPlotters[-1].addCorrectionFactor('GJetsZPtWeight','GJetsZPtWeight')
        phymetPlotters[-1].addCorrectionFactor(str(zjetsFidXsecAll),'zjetsFidXsecAll')

### the GJets data
gdataPlotters=[]
gdataSamples = [
#'SinglePhoton_Run2016B2H_ReReco_36p46_ResBos_Rc36p46ReCalib',
#'SinglePhoton_Run2016B2H_ReReco_36p46_ResBosRefit_Rc36p46ReCalib',
'SinglePhoton_Run2016B2H_ReReco_36p46_Rc36p46ReCalib',
]



for sample in gdataSamples:
    gdataPlotters.append(TreePlotter(sample, indir+'/'+sample+'.root','tree'))
    gdataPlotters[-1].addCorrectionFactor('GJetsPreScaleWeight','GJetsPreScaleWeight')
    gdataPlotters[-1].addCorrectionFactor('GJetsRhoWeight','GJetsRhoWeight')
    gdataPlotters[-1].addCorrectionFactor(str(1/gdataYield),'GJetsNorm0')
    gdataPlotters[-1].addCorrectionFactor(mc_scale,'mc_scale')
    gdataPlotters[-1].addCorrectionFactor(zjets_scale,'zjets_scale')
    if channel=='el' :
        gdataPlotters[-1].addCorrectionFactor('GJetsZPtWeightEl','GJetsZPtWeight')
        gdataPlotters[-1].addCorrectionFactor(str(zjetsFidXsecEl),'zjetsFidXsecEl')
    elif channel=='mu' :
        gdataPlotters[-1].addCorrectionFactor('GJetsZPtWeightMu','GJetsZPtWeight')
        gdataPlotters[-1].addCorrectionFactor(str(zjetsFidXsecMu),'zjetsFidXsecMu')
    else :
        gdataPlotters[-1].addCorrectionFactor('GJetsZPtWeight','GJetsZPtWeight')
        gdataPlotters[-1].addCorrectionFactor(str(zjetsFidXsecAll),'zjetsFidXsecAll')

# the GJets plotter
#gjetsPlotters = gdataPlotters
gjetsPlotters = gdataPlotters+phymetPlotters


GJets = MergedPlotter(gjetsPlotters)
GJets.setFillProperties(1001,ROOT.kGreen+2)


### MC ZJets
mczjetsPlotters=[]
mczjetsSamples = [
#'DYJetsToLL_M50_BIG_ResBos_NoRecoil',
#'DYJetsToLL_M50_BIG_ResBos_Rc36p46',
#'DYJetsToLL_M50_BIG_ResBosRefit_Rc36p46',
'DYJetsToLL_M50_BIG_Rc36p46DtReCalib',
]



for sample in mczjetsSamples:
    mczjetsPlotters.append(TreePlotter(sample, indir+'/'+sample+'.root','tree'))
    mczjetsPlotters[-1].addCorrectionFactor('1./SumWeights','norm')
    #mczjetsPlotters[-1].addCorrectionFactor('(1)','norm')
    mczjetsPlotters[-1].addCorrectionFactor(ZPtWeight,'ZPtWeight')
    #mczjetsPlotters[-1].addCorrectionFactor('xsec','xsec')
    mczjetsPlotters[-1].addCorrectionFactor('(1921.8*3)','xsec') # FEWZ NNLO.results_z_m50_nnlo_inclusive_NNPDF30_nlo_as_0118
    #mczjetsPlotters[-1].addCorrectionFactor('(1907.0*3)','xsec') # FEWZ NNLO.results_z_m50_nnlo_fsrOn_lowstat_inclusive_NNPDF30_nlo_as_0118
    mczjetsPlotters[-1].addCorrectionFactor('genWeight','genWeight')
    #mczjetsPlotters[-1].addCorrectionFactor("ZJetsGenWeight",'genWeight')
    mczjetsPlotters[-1].addCorrectionFactor(puWeight,'puWeight')
    mczjetsPlotters[-1].addCorrectionFactor(lepsf,'lepsf')
    mczjetsPlotters[-1].addCorrectionFactor(mc_scale,'mc_scale')
    mczjetsPlotters[-1].addCorrectionFactor(zjets_scale,'zjets_scale')


MCZJets = MergedPlotter(mczjetsPlotters)
MCZJets.setFillProperties(1001,ROOT.kGreen+2)


##
# choose GJets or ZJets MC
if dyGJets:
  ZJets = GJets
else: 
  ZJets = MCZJets

####

ttPlotters=[]
ttSamples = ['TTTo2L2Nu','TTZToLLNuNu','TTWJetsToLNu']

for sample in ttSamples:
    ttPlotters.append(TreePlotter(sample, indir+'/'+sample+'.root','tree'))
    ttPlotters[-1].addCorrectionFactor('1./SumWeights','norm')
    ttPlotters[-1].addCorrectionFactor('xsec','xsec')
    ttPlotters[-1].addCorrectionFactor('genWeight','genWeight')
    ttPlotters[-1].addCorrectionFactor(puWeight,'puWeight')
    ttPlotters[-1].addCorrectionFactor(lepsf,'lepsf')
    ttPlotters[-1].addCorrectionFactor(mc_scale,'mc_scale')

TT = MergedPlotter(ttPlotters)
TT.setFillProperties(1001,ROOT.kAzure-9)

sigPlotters=[]
sigSamples = [
'BulkGravToZZToZlepZinv_narrow_600',
#'BulkGravToZZToZlepZinv_narrow_800',
'BulkGravToZZToZlepZinv_narrow_1000',
#'BulkGravToZZToZlepZinv_narrow_1200',
#'BulkGravToZZToZlepZinv_narrow_1400',
#'BulkGravToZZToZlepZinv_narrow_1600', 
#'BulkGravToZZToZlepZinv_narrow_1800', 
'BulkGravToZZToZlepZinv_narrow_2000',
#'BulkGravToZZToZlepZinv_narrow_2500',
#'BulkGravToZZToZlepZinv_narrow_3000',
#'BulkGravToZZToZlepZinv_narrow_3500', 
#'BulkGravToZZToZlepZinv_narrow_4000', 
#'BulkGravToZZToZlepZinv_narrow_4500', 
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
    sigPlotters[-1].addCorrectionFactor(mc_scale,'mc_scale')
    sigPlotters[-1].setFillProperties(0,ROOT.kWhite)
    # some plotting definition
    sigPlotters[-1].setAlias('llnunu_l1_mass_to_plot', 'llnunu_l1_mass')
    sigPlotters[-1].setAlias('llnunu_l2_pt_to_plot', 'llnunu_l2_pt')
    sigPlotters[-1].setAlias('llnunu_l2_phi_to_plot', 'llnunu_l2_phi')
    sigPlotters[-1].setAlias('llnunu_mt_to_plot', 'llnunu_mt')




dataPlotters=[]
dataSamples = [
#'SingleEMU_Run2016B2H_ReReco_36p46',
'SingleEMU_Run2016B2H_ReReco_36p46_DtReCalib',
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



Stack = StackPlotter(outTag=tag, outDir=outdir)
Stack.setPaveText(paveText)
Stack.addPlotter(Data, "data_obs", "Data", "data")
if muoneg: 
    Stack.addPlotter(NONRES, "NonReso","Non-reson. (e#mu data)", "background")
else:
    Stack.addPlotter(WW, "NonReso","WW/WZ/WJets non-reson.", "background")
    Stack.addPlotter(TT, "TT","TT", "background")
Stack.addPlotter(VV, "VVZReso","ZZ WZ reson.", "background")
if dyGJets: Stack.addPlotter(ZJets, "ZJets","ZJets(#gamma+Jets data)", "background")
else: Stack.addPlotter(ZJets, "ZJets","ZJets(MC)", "background")

for i in range(len(sigSamples)):
  sigPlotters[i].setLineProperties(2,ROOT.kRed+i,2)
  Stack.addPlotter(sigPlotters[i],sigSamples[i],sigSampleNames[sigSamples[i]],'signal')  

 
Stack.setLog(LogY)
Stack.doRatio(doRatio)



tag+='_'


if test: 
#    Stack.drawStack('nVert', cuts, str(lumi*1000), 80, 0.0, 80.0, titlex = "N vertices", units = "",output=tag+'nVert',outDir=outdir,separateSignal=sepSig)
#    Stack.drawStack('rho', cuts, str(lumi*1000), 55, 0.0, 55.0, titlex = "#rho", units = "",output=tag+'rho',outDir=outdir,separateSignal=sepSig)
#    Stack.drawStack('llnunu_l1_pt', cuts, str(lumi*1000), 50, 0.0, 500.0, titlex = "P_{T}(Z)", units = "GeV",output=tag+'zpt_low',outDir=outdir,separateSignal=sepSig)
#    Stack.drawStack('llnunu_l1_pt', cuts, str(lumi*1000), 30, 0.0, 1500.0, titlex = "P_{T}(Z)", units = "GeV",output=tag+'zpt',outDir=outdir,separateSignal=sepSig)
#    Stack.drawStack('llnunu_l1_pt', cuts, str(lumi*1000), 75, 0.0, 1500.0, titlex = "P_{T}(Z)", units = "GeV",output=tag+'zpt',outDir=outdir,separateSignal=sepSig)
#    Stack.drawStack('llnunu_l1_mass_to_plot', cuts, str(lumi*1000), 60, 60, 120, titlex = "M(Z)", units = "GeV",output=tag+'zmass',outDir=outdir,separateSignal=sepSig)
#    Stack.drawStack('llnunu_l1_mass_to_plot', cuts, str(lumi*1000), 100, 0, 200, titlex = "M(Z)", units = "GeV",output=tag+'zmass',outDir=outdir,separateSignal=sepSig)
#    Stack.drawStack('llnunu_mt_to_plot', cuts, str(lumi*1000), 50, 100.0, 600.0, titlex = "M_{T}", units = "GeV",output=tag+'mt_low',outDir=outdir,separateSignal=sepSig,blinding=Blind,blindingCut=300)
#    Stack.drawStack('llnunu_mt_to_plot', cuts, str(lumi*1000), 50, 100.0, 1600.0, titlex = "M_{T}", units = "GeV",output=tag+'mt',outDir=outdir,separateSignal=sepSig,blinding=Blind,blindingCut=300)
#    Stack.drawStack('llnunu_mt_to_plot', cuts, str(lumi*1000), 55, 100.0, 1200.0, titlex = "M_{T}", units = "GeV",output=tag+'mt',outDir=outdir,separateSignal=sepSig,blinding=Blind,blindingCut=300)
#    Stack.drawStack('llnunu_mt_to_plot', cuts, str(lumi*1000), 100, 0.0, 3000.0, titlex = "M_{T}", units = "GeV",output=tag+'mt',outDir=outdir,separateSignal=sepSig,blinding=Blind,blindingCut=300)
    Stack.drawStack('llnunu_mt_to_plot', cuts, str(lumi*1000), 300, 0.0, 3000.0, titlex = "M_{T}", units = "GeV",output=tag+'mt',outDir=outdir,separateSignal=sepSig,blinding=Blind,blindingCut=300)

#    Stack.drawStack('llnunu_l2_pt_to_plot', cuts, str(lumi*1000), 50, 0, 500, titlex = "MET", units = "GeV",output=tag+'met_low',outDir=outdir,separateSignal=sepSig,blinding=Blind,blindingCut=200)
#    Stack.drawStack('llnunu_l2_pt_to_plot', cuts, str(lumi*1000), 30, 0, 1500, titlex = "MET", units = "GeV",output=tag+'met',outDir=outdir,separateSignal=sepSig,blinding=Blind,blindingCut=200)
#    Stack.drawStack('llnunu_l2_pt_to_plot', cuts, str(lumi*1000), 50, 0, 1000, titlex = "MET", units = "GeV",output=tag+'met',outDir=outdir,separateSignal=sepSig,blinding=Blind,blindingCut=200)
#    Stack.drawStack('llnunu_l2_pt_to_plot*cos(llnunu_l2_phi_to_plot-llnunu_l1_phi)', cuts, str(lumi*1000), 50, -500, 500.0, titlex = "MET_{#parallel}", units = "GeV",output=tag+'met_para',outDir=outdir,separateSignal=sepSig)
#    Stack.drawStack('llnunu_l2_pt_to_plot*sin(llnunu_l2_phi_to_plot-llnunu_l1_phi)', cuts, str(lumi*1000), 50, -500, 500.0, titlex = "MET_{#perp}", units = "GeV",output=tag+'met_perp',outDir=outdir,separateSignal=sepSig)
#    Stack.drawStack('fabs(TVector2::Phi_mpi_pi(llnunu_l2_phi_to_plot-llnunu_l1_phi))', cuts, str(lumi*1000), 50, 0, 5, titlex = "#Delta#phi(Z,MET)", units = "",output=tag+'dphiZMet',outDir=outdir,separateSignal=sepSig)

else: 
    Stack.drawStack('nVert', cuts, str(lumi*1000), 80, 0.0, 80.0, titlex = "N vertices", units = "",output=tag+'nVert',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('rho', cuts, str(lumi*1000), 55, 0.0, 55.0, titlex = "#rho", units = "",output=tag+'rho',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('llnunu_mt_to_plot', cuts, str(lumi*1000), 100, 0.0, 3000.0, titlex = "M_{T}", units = "GeV",output=tag+'mt_high3',outDir=outdir,separateSignal=sepSig,blinding=Blind,blindingCut=300)
    Stack.drawStack('llnunu_mt_to_plot', cuts, str(lumi*1000), 50, 100.0, 1600.0, titlex = "M_{T}", units = "GeV",output=tag+'mt_high',outDir=outdir,separateSignal=sepSig,blinding=Blind,blindingCut=300)
    Stack.drawStack('llnunu_mt_to_plot', cuts, str(lumi*1000), 55, 100.0, 1200.0, titlex = "M_{T}", units = "GeV",output=tag+'mt',outDir=outdir,separateSignal=sepSig,blinding=Blind,blindingCut=300)
    Stack.drawStack('llnunu_mt_to_plot', cuts, str(lumi*1000), 50, 100.0, 600.0, titlex = "M_{T}", units = "GeV",output=tag+'mt_low',outDir=outdir,separateSignal=sepSig,blinding=Blind,blindingCut=300)
    Stack.drawStack('llnunu_mt_to_plot', cuts, str(lumi*1000), 80, 100.0, 300.0, titlex = "M_{T}", units = "GeV",output=tag+'mt_low2',outDir=outdir,separateSignal=sepSig,blinding=Blind,blindingCut=300)
    Stack.drawStack('llnunu_l1_mass_to_plot', cuts, str(lumi*1000), 60, 60, 120, titlex = "M(Z)", units = "GeV",output=tag+'zmass',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('llnunu_l1_pt', cuts, str(lumi*1000), 30, 0.0, 1500.0, titlex = "P_{T}(Z)", units = "GeV",output=tag+'zpt_high',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('llnunu_l1_pt', cuts, str(lumi*1000), 75, 0.0, 1500.0, titlex = "P_{T}(Z)", units = "GeV",output=tag+'zpt',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('llnunu_l1_pt', cuts, str(lumi*1000), 50, 0.0, 500.0, titlex = "P_{T}(Z)", units = "GeV",output=tag+'zpt_low',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('llnunu_l1_pt', cuts, str(lumi*1000), 100, 0.0, 300.0, titlex = "P_{T}(Z)", units = "GeV",output=tag+'zpt_low2',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('llnunu_l1_eta', cuts, str(lumi*1000), 200, -10.0, 10.0, titlex = "#eta(Z) ", units = "",output=tag+'zeta',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('llnunu_l1_rapidity', cuts, str(lumi*1000), 60, -3.0, 3.0, titlex = "Rapidity(Z) ", units = "",output=tag+'zrapidity',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('llnunu_l1_phi', cuts, str(lumi*1000), 64, -3.2, 3.2, titlex = "#phi(Z)", units = "",output=tag+'zphi',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('llnunu_l2_pt_to_plot', cuts, str(lumi*1000), 30, 0, 1500, titlex = "MET", units = "GeV",output=tag+'met_high',outDir=outdir,separateSignal=sepSig,blinding=Blind,blindingCut=200)
    Stack.drawStack('llnunu_l2_pt_to_plot', cuts, str(lumi*1000), 50, 0, 1000, titlex = "MET", units = "GeV",output=tag+'met',outDir=outdir,separateSignal=sepSig,blinding=Blind,blindingCut=200)
    Stack.drawStack('llnunu_l2_pt_to_plot', cuts, str(lumi*1000), 50, 0, 500, titlex = "MET", units = "GeV",output=tag+'met_low',outDir=outdir,separateSignal=sepSig,blinding=Blind,blindingCut=200)
    Stack.drawStack('llnunu_l2_pt_to_plot', cuts, str(lumi*1000), 100, 0, 300, titlex = "MET", units = "GeV",output=tag+'met_low2',outDir=outdir,separateSignal=sepSig,blinding=Blind,blindingCut=200)
    Stack.drawStack('llnunu_l2_phi_to_plot', cuts, str(lumi*1000), 100, -3.2, 3.2, titlex = "#phi(MET)", units = "",output=tag+'metPhi',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('llnunu_l2_sumEt', cuts, str(lumi*1000), 80, 0.0, 3000.0, titlex = "sumE_{T}", units = "GeV",output=tag+'metSumEt',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('llnunu_l2_pt_to_plot*cos(llnunu_l2_phi_to_plot-llnunu_l1_phi)', cuts, str(lumi*1000), 100, -300, 300.0, titlex = "MET_{#parallel}", units = "GeV",output=tag+'met_para',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('llnunu_l2_pt_to_plot*sin(llnunu_l2_phi_to_plot-llnunu_l1_phi)', cuts, str(lumi*1000), 100, -300, 300.0, titlex = "MET_{#perp}", units = "GeV",output=tag+'met_perp',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('llnunu_l2_pt_to_plot*cos(llnunu_l2_phi_to_plot-llnunu_l1_phi)', cuts, str(lumi*1000), 50, -500, 500.0, titlex = "MET_{#parallel}", units = "GeV",output=tag+'met_para_high',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('llnunu_l2_pt_to_plot*sin(llnunu_l2_phi_to_plot-llnunu_l1_phi)', cuts, str(lumi*1000), 50, -500, 500.0, titlex = "MET_{#perp}", units = "GeV",output=tag+'met_perp_high',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('llnunu_l2_pt_to_plot/sqrt(llnunu_l2_sumEt)', cuts, str(lumi*1000), 100, 0.0, 20.0, titlex = "MET/#sqrt{sumE_{T}}", units = "",output=tag+'metOvSqSET',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('fabs(TVector2::Phi_mpi_pi(llnunu_l2_phi_to_plot-llnunu_l1_phi))', cuts, str(lumi*1000), 50, 0, 5, titlex = "#Delta#phi(Z,MET)", units = "",output=tag+'dphiZMet',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('fabs(llnunu_l1_pt+llnunu_l2_pt_to_plot*cos(llnunu_l2_phi_to_plot-llnunu_l1_phi))', cuts, str(lumi*1000), 100, 0, 1000, titlex = "#Delta P_{T}^{#parallel}(Z,MET)", units = "GeV",output=tag+'dPTPara',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('fabs(llnunu_l2_pt_to_plot*sin(llnunu_l2_phi_to_plot-llnunu_l1_phi))', cuts, str(lumi*1000), 100, 0, 100, titlex = "#Delta P_{T}^{#perp}(Z,MET)", units = "GeV",output=tag+'dPTPerp',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('fabs(llnunu_l1_pt+llnunu_l2_pt_to_plot*cos(llnunu_l2_phi_to_plot-llnunu_l1_phi))/llnunu_l1_pt', cuts, str(lumi*1000), 100, 0, 5, titlex = "#Delta P_{T}^{#parallel}(Z,MET)/P_{T}(Z)", units = "",output=tag+'dPTParaRel',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('fabs(llnunu_l2_pt_to_plot*sin(llnunu_l2_phi_to_plot-llnunu_l1_phi))/llnunu_l1_pt', cuts, str(lumi*1000), 100, 0, 5, titlex = "#Delta P_{T}^{#perp}(Z,MET)/P_{T}", units = "",output=tag+'dPTPerpRel',outDir=outdir,separateSignal=sepSig)

    if not dyGJets: 
        Stack.drawStack('llnunu_l1_deltaPhi', cuts, str(lumi*1000), 50, 0.0, 5.0, titlex = "#Delta#phi", units = "",output=tag+'ZdeltaPhi',outDir=outdir,separateSignal=sepSig)
        Stack.drawStack('llnunu_l1_deltaR', cuts, str(lumi*1000), 50, 0.0, 5.0, titlex = "#Delta R", units = "",output=tag+'ZdeltaR',outDir=outdir,separateSignal=sepSig)
        Stack.drawStack('TMath::Tan((TMath::Pi()-TMath::Abs(llnunu_l1_deltaPhi))/2.0)*TMath::Sin(TMath::ACos(TMath::TanH((llnunu_l1_l1_eta-llnunu_l1_l2_eta)/2.0)))', cuts, str(lumi*1000), 100, 0.0, 10, titlex = "#phi_{#eta}*", units = "",output=tag+'PhiStar',outDir=outdir,separateSignal=sepSig)
  


    if DrawLeptons and not dyGJets and  channel=='mu' :
        Stack.drawStack('llnunu_l1_l1_pt', cuts+"&&(abs(llnunu_l1_l1_pdgId)==13)", str(lumi*1000), 100, 0.0, 1000.0, titlex = "P_{T}(#mu_{1})", units = "GeV",output=tag+'pTlep1_mu',outDir=outdir,separateSignal=sepSig)
        Stack.drawStack('llnunu_l1_l1_eta', cuts+"&&(abs(llnunu_l1_l1_pdgId)==13)", str(lumi*1000), 30, -3.0, 3.0, titlex = "#eta(#mu_{1})", units = "",output=tag+'etalep1_mu',outDir=outdir,separateSignal=sepSig)
        Stack.drawStack('llnunu_l1_l1_phi', cuts+"&&(abs(llnunu_l1_l1_pdgId)==13)", str(lumi*1000), 32, -3.2, 3.2, titlex = "#phi(#mu_{1})", units = "",output=tag+'philep1_mu',outDir=outdir,separateSignal=sepSig)
        Stack.drawStack('llnunu_l1_l1_trackerIso', cuts+"&&(abs(llnunu_l1_l1_pdgId)==13)", str(lumi*1000), 100, 0.0, 0.2, titlex = "trackerISO_{rel}(#mu_{1})", units = "",output=tag+'ISOlep1_mu',outDir=outdir,separateSignal=sepSig)
        Stack.drawStack('llnunu_l1_l2_pt', cuts+"&&(abs(llnunu_l1_l2_pdgId)==13)", str(lumi*1000), 100, 0.0, 500.0, titlex = "P_{T}(#mu_{2})", units = "GeV",output=tag+'pTlep2_mu',outDir=outdir,separateSignal=sepSig)
        Stack.drawStack('llnunu_l1_l2_eta', cuts+"&&(abs(llnunu_l1_l2_pdgId)==13)", str(lumi*1000), 30, -3.0, 3.0, titlex = "#eta(#mu_{2})", units = "",output=tag+'etalep2_mu',outDir=outdir,separateSignal=sepSig)
        Stack.drawStack('llnunu_l1_l2_phi', cuts+"&&(abs(llnunu_l1_l2_pdgId)==13)", str(lumi*1000), 32, -3.2, 3.2, titlex = "#phi(#mu_{2})", units = "",output=tag+'philep2_mu',outDir=outdir,separateSignal=sepSig)
        Stack.drawStack('llnunu_l1_l2_trackerIso', cuts+"&&(abs(llnunu_l1_l2_pdgId)==13)", str(lumi*1000), 100, 0.0, 0.2, titlex = "trackerISO_{rel}(#mu_{2})", units = "",output=tag+'ISOlep2_mu',outDir=outdir,separateSignal=sepSig)

    if DrawLeptons and not dyGJets and  channel=='el' :
        Stack.drawStack('llnunu_l1_l1_pt', cuts+"&&(abs(llnunu_l1_l1_pdgId)==11)", str(lumi*1000), 100, 0.0, 1000.0, titlex = "P_{T}(e_{1})", units = "GeV",output=tag+'pTlep1_el',outDir=outdir,separateSignal=sepSig)
        Stack.drawStack('llnunu_l1_l1_eta', cuts+"&&(abs(llnunu_l1_l1_pdgId)==11)", str(lumi*1000), 30, -3.0, 3.0, titlex = "#eta(e_{1})", units = "",output=tag+'etalep1_el',outDir=outdir,separateSignal=sepSig)
        Stack.drawStack('llnunu_l1_l1_phi', cuts+"&&(abs(llnunu_l1_l1_pdgId)==11)", str(lumi*1000), 32, -3.2, 3.2, titlex = "#phi(e_{1})", units = "",output=tag+'philep1_el',outDir=outdir,separateSignal=sepSig)
        Stack.drawStack('llnunu_l1_l1_electronrelIsoea03', cuts+"&&(abs(llnunu_l1_l1_pdgId)==11)", str(lumi*1000), 100, 0.0, 0.2, titlex = "looseISO_{rel}(e_{1})", units = "",output=tag+'ISOlep1_el',outDir=outdir,separateSignal=sepSig)
        Stack.drawStack('llnunu_l1_l2_pt', cuts+"&&(abs(llnunu_l1_l2_pdgId)==11)", str(lumi*1000), 100, 0.0, 500.0, titlex = "P_{T}(e_{2})", units = "GeV",output=tag+'pTlep2_el',outDir=outdir,separateSignal=sepSig)
        Stack.drawStack('llnunu_l1_l2_eta', cuts+"&&(abs(llnunu_l1_l2_pdgId)==11)", str(lumi*1000), 30, -3.0, 3.0, titlex = "#eta(e_{2})", units = "",output=tag+'etalep2_el',outDir=outdir,separateSignal=sepSig)
        Stack.drawStack('llnunu_l1_l2_phi', cuts+"&&(abs(llnunu_l1_l2_pdgId)==11)", str(lumi*1000), 32, -3.2, 3.2, titlex = "#phi(e_{2})", units = "",output=tag+'philep2_el',outDir=outdir,separateSignal=sepSig)
        Stack.drawStack('llnunu_l1_l2_electronrelIsoea03', cuts+"&&(abs(llnunu_l1_l2_pdgId)==11)", str(lumi*1000), 100, 0.0, 0.2, titlex = "looseISO_{rel}(e_{2})", units = "",output=tag+'ISOlep2_el',outDir=outdir,separateSignal=sepSig)


Stack.closePSFile()
Stack.closeROOTFile()

