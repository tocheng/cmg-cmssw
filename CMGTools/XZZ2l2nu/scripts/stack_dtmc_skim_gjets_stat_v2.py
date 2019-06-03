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

def cutsALT(cuts,syst,variation) :
    cutsNew=cuts.replace("MET", "MET_"+syst+variation, 1) 
    return cutsNew    
    #return cuts

parser = optparse.OptionParser()
parser.add_option("-t","--tag",dest="tag",default='DataB2G_ICHEPcfg_',help="")
parser.add_option("--channel",dest="channel",default='mu',help="")
parser.add_option("--cutChain",dest="cutChain",default='tight',help="")
parser.add_option("--LogY",action="store_true", dest="LogY", default=False, help="")
parser.add_option("--Blind",action="store_true", dest="Blind", default=False,help="")
parser.add_option("--test",action="store_true", dest="test", default=False,help="")
parser.add_option("--dyGJets",action="store_true", dest="dyGJets", default=False,help="")
parser.add_option("--muoneg",action="store_true", dest="muoneg", default=False,help="")
parser.add_option("--doSys",action="store_true", dest="doSys", default=False,help="")
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
DrawLeptons=False
doRhoScale=False
doGMCEtaScale=False
doGMCPhPtScale=True
dyGJets=options.dyGJets
muoneg=options.muoneg
doSys=options.doSys

if test: DrawLeptons = False

lepsf="trgsf*isosf*idsf*trksf"

g_scale='(1)'
mc_scale='(1)'
zjets_scale='(1)'

# temp turn off mc_scale
#mc_scale="(1)"
#zjets_scale="(1)"

if channel=='mu':
#    mc_scale='(1.02942)'
    zjets_scale='(1.03627471042)'
#    if cutChain=='SR': zjets_scale='(0.960865)' # mc SR
#    elif cutChain=='SRdPhiGT0p5': zjets_scale='(0.999113)' # mc SR
#    else: zjets_scale='(1.03410)' # mc zpt50
elif channel=='el':
#    mc_scale='(1.02139)'
    zjets_scale='(1.04227778889)'
#    if cutChain=='SR': zjets_scale='(1.01822)' # mc SR
#    elif cutChain=='SRdPhiGT0p5': zjets_scale='(1.01265)' # mc SR
#    else: zjets_scale='(1.03737)' # mc zpt50
else:
#    mc_scale='(1.02942)'
    zjets_scale='(1.03627471042)'

# temp turn off mc_scale
#mc_scale="(1)"
#zjets_scale="(1)"

# non reso alpha
nonreso_alpha_el=1.0
nonreso_alpha_mu=1.0

nonreso_alpha_el=0.397075177316
nonreso_alpha_mu=0.704939528419

if doRhoScale:
    tag+="RhoWt_"
    rho_scale = "*(0.366*TMath::Gaus(rho,8.280,5.427)+0.939*TMath::Gaus(rho,18.641,10.001)+0.644*TMath::Gaus(rho,40.041,10.050))" # 2016 rereco/summer16 81.81 fb-1
    lepsf += rho_scale
    g_scale += rho_scale

# trigger eff
if doGMCPhPtScale:
    tag+="GMCPhPtWt_"
    g_scale+="*((0.295668+0.0127154*llnunu_l1_pt-7.71163e-05*pow(llnunu_l1_pt,2)+2.2603e-07*pow(llnunu_l1_pt,3)-3.50496e-10*pow(llnunu_l1_pt,4)+2.7572e-13*pow(llnunu_l1_pt,5)-8.66455e-17*pow(llnunu_l1_pt,6))*(llnunu_l1_pt<=800)+(0.912086)*(llnunu_l1_pt>800))"  # for reminiaod allcorV2 mc hlt

outdir='plots'

#indir='/home/heli/XZZ/80X_20170202_light_hlt_Skim/'
#indir='/home/heli/XZZ/80X_20170202_light_hlt_RcSkim/'
indir='/home/heli/XZZ/80X_20170202_light_hlt_allcorV2RcSkim/'
#indir='/home/heli/XZZ/80X_20170202_light_hlt_allcorV2Skim/'
#indir='/home/heli/XZZ/80X_20170202_light_Skim/'
lumi=35.9
sepSig=True
doRatio=True
Blind=options.Blind
FakeData=False
UseMETFilter=True
SignalAll1pb=True
puWeight='puWeightsummer16'
k=1 # signal scale
ZPtWeight="ZPtWeight"

elChannel='((abs(llnunu_l1_l1_pdgId)==11||abs(llnunu_l1_l2_pdgId)==11)&&llnunu_l1_l1_pt>120&&abs(llnunu_l1_l1_eta)<2.5&&llnunu_l1_l2_pt>35&&abs(llnunu_l1_l2_eta)<2.5)'
muChannel='((abs(llnunu_l1_l1_pdgId)==13||abs(llnunu_l1_l2_pdgId)==13)&&llnunu_l1_l1_pt>60&&abs(llnunu_l1_l1_eta)<2.4&&llnunu_l1_l2_pt>20&&abs(llnunu_l1_l2_eta)<2.4&&(llnunu_l1_l1_highPtID>0.99||llnunu_l1_l2_highPtID>0.99))'
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
cuts_met50="(llnunu_MET>50)"
cuts_met100="(llnunu_l2_pt_to_plot>100)"
cuts_met200="(llnunu_l2_pt_to_plot>200)"
cuts_loose_z="("+cuts_lepaccept+"&&"+cuts_zmass+")"
cuts_loose_zpt20="("+cuts_lepaccept+"&&"+cuts_zmass+"&&llnunu_l1_pt>20)"
cuts_loose_zpt50="("+cuts_lepaccept+"&&"+cuts_zmass+"&&llnunu_l1_pt>50)"
cuts_loose_zptgt50lt200="("+cuts_lepaccept+"&&"+cuts_zmass+"&&llnunu_l1_pt>50&&llnunu_l1_pt<200)"
cuts_loose_zptgt100lt400="("+cuts_lepaccept+"&&"+cuts_zmass+"&&llnunu_l1_pt>100&&llnunu_l1_pt<400)"
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
elif cutChain=='tightzptgt100lt400': cuts=cuts_loose_zptgt100lt400
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
elif cutChain=='SR': cuts=cuts_loose_zll_met50
elif cutChain=='CR': cuts=cuts_CR
elif cutChain=='CR1': cuts=cuts_CR1
elif cutChain=='CR2': cuts=cuts_CR2
elif cutChain=='CR3': cuts=cuts_CR3
elif cutChain=='SRdPhiGT0p5': cuts=cuts_loose_zll_met50+"&&fabs(TVector2::Phi_mpi_pi(llnunu_l2_phi_to_plot-llnunu_l1_phi))>0.5"
elif cutChain=='SRzptGT200': cuts=cuts_loose_zll_met50+"&&llnunu_l1_pt>200"
else : cuts=cuts_loose


if UseMETFilter:
    cuts = '('+cuts+')' # metfilter pre-applied in preskim

# badmuon filter
cuts += '&&(nbadmuon==0)' 

cuts = '('+cuts+')'
print cuts

ROOT.gROOT.ProcessLine('.x tdrstyle.C') 

#######################
#  VV Reso backgrounds
#######################
vvPlotters=[]
vvSamples = [
'WZTo2L2Q',
'WZTo3LNu',
'ZZTo2L2Nu',
'ZZTo2L2Q',
'ZZTo4L',
'ggZZTo2e2nu','ggZZTo2mu2nu',
'TTZToLLNuNu'
]

vvPlotters=[]
for sample in vvSamples:
    vvPlotters.append(TreePlotter(sample, indir+'/'+sample+'.root','tree'))
    vvPlotters[-1].addCorrectionFactor('1/SumWeights','norm')
    vvPlotters[-1].addCorrectionFactor('genWeight','genWeight')
    vvPlotters[-1].addCorrectionFactor(puWeight,'puWeight')
    vvPlotters[-1].addCorrectionFactor(lepsf, 'lepsf')
    vvPlotters[-1].addCorrectionFactor(mc_scale,'mc_scale')
    if sample == 'WZTo3LNu': vvPlotters[-1].addCorrectionFactor('4.4297','xsec') 
    else: vvPlotters[-1].addCorrectionFactor('xsec','xsec')
    if sample == 'ZZTo2L2Nu' : vvPlotters[-1].addCorrectionFactor("(ZZEwkCorrWeight*ZZQcdCorrWeight)", 'nnlo')
    vvPlotters[-1].setAlias('passMuHLT', '((llnunu_l1_l1_trigerob_HLTbit>>3&1)||(llnunu_l1_l1_trigerob_HLTbit>>4&1)||(llnunu_l1_l2_trigerob_HLTbit>>3&1)||(llnunu_l1_l2_trigerob_HLTbit>>4&1))');
    vvPlotters[-1].setAlias('passElHLT', '((llnunu_l1_l1_trigerob_HLTbit>>1&1)||(llnunu_l1_l2_trigerob_HLTbit>>1&1))');
    vvPlotters[-1].addCorrectionFactor('(passMuHLT||passElHLT)','HLT')

VV = MergedPlotter(vvPlotters)
VV.setFillProperties(1001,ROOT.kMagenta)

# some plotting definition
VV.setAlias('llnunu_l1_mass_to_plot', 'llnunu_l1_mass')
VV.setAlias('llnunu_l2_pt_to_plot', 'llnunu_l2_pt')
VV.setAlias('llnunu_MET', 'llnunu_l2_pt')
VV.setAlias('llnunu_l2_phi_to_plot', 'llnunu_l2_phi')
VV.setAlias('llnunu_mt_to_plot', 'llnunu_mt')
VV.setAlias('llnunu_mT', 'llnunu_mt')
for syst in ['JetEn','JetRes','MuonEn','ElectronEn','TauEn','PhotonEn','Uncluster'] :
    VV.setAlias('llnunu_mT_'+syst+'Up', 'llnunu_mt_'+syst+'Up')
    VV.setAlias('llnunu_mT_'+syst+'Dn', 'llnunu_mt_'+syst+'Dn')
    VV.setAlias('llnunu_MET_'+syst+'Dn', 'llnunu_l2_t1Pt_'+syst+'Dn')
    VV.setAlias('llnunu_MET_'+syst+'Up', 'llnunu_l2_t1Pt_'+syst+'Up')

VV.setAlias('llnunu_mT_RecoilUp', 'llnunu_mt')
VV.setAlias('llnunu_mT_RecoilDn', 'llnunu_mt')
VV.setAlias('llnunu_MET_RecoilUp', 'llnunu_l2_pt')
VV.setAlias('llnunu_MET_RecoilDn', 'llnunu_l2_pt')

#######################
#  NonReso backgrounds
#######################

# if use emu data driven method
if muoneg: 

    if channel=="el": 
        emuscale="(etrgsf*"+str(nonreso_alpha_el)+")"
    elif channel=="mu": 
        emuscale="(mtrgsf*"+str(nonreso_alpha_mu)+")"
    else: 
        emuscale="(etrgsf*"+str(nonreso_alpha_el)+"+mtrgsf*"+str(nonreso_alpha_mu)+")"

    nonresPlotters=[]
    nonresSamples = ['muoneg_light_skim','DYJetsToLL_emupair']
    for sample in nonresSamples:
        nonresPlotters.append(TreePlotter(sample, indir+'/'+sample+'.root','tree'))
        nonresPlotters[-1].addCorrectionFactor(emuscale, 'emuscale')
        nonresPlotters[-1].addCorrectionFactor(mc_scale,'mc_scale')
        if 'muoneg_' in sample:
            nonresPlotters[-1].addCorrectionFactor(str(0.001/lumi), 'norm')
        else:
            nonresPlotters[-1].addCorrectionFactor('(-1./SumWeights)','norm')
            nonresPlotters[-1].addCorrectionFactor('xsec','xsec')
            nonresPlotters[-1].addCorrectionFactor('genWeight','genWeight')
            nonresPlotters[-1].addCorrectionFactor(puWeight,'puWeight')
            if 'DYJetsToLL' in sample: nonresPlotters[-1].addCorrectionFactor('ZPtWeight','ZPtWeight')

    NONRES = MergedPlotter(nonresPlotters)
    NONRES.setFillProperties(1001,ROOT.kOrange)

    # some plotting definition
    NONRES.setAlias('llnunu_l1_mass_to_plot', 'llnunu_l1_mass')
    NONRES.setAlias('llnunu_l2_pt_to_plot', 'llnunu_l2_pt')
    NONRES.setAlias('llnunu_MET', 'llnunu_l2_pt')
    NONRES.setAlias('llnunu_l2_phi_to_plot', 'llnunu_l2_phi')
    NONRES.setAlias('llnunu_mt_to_plot', 'llnunu_mt')
    NONRES.setAlias('llnunu_mT', 'llnunu_mt')
    #NONRES.setAlias('nbadmuon', '(!nllnunu)')
    for syst in ['JetEn','JetRes','MuonEn','ElectronEn','TauEn','PhotonEn','Uncluster','Recoil'] :
        NONRES.setAlias('llnunu_mT_'+syst+'Up', 'llnunu_mt')
        NONRES.setAlias('llnunu_mT_'+syst+'Dn', 'llnunu_mt')
        NONRES.setAlias('llnunu_MET_'+syst+'Up', 'llnunu_l2_pt')
        NONRES.setAlias('llnunu_MET_'+syst+'Dn', 'llnunu_l2_pt')

# if use mc
else:

    mcnonresoSamples = ['WWTo2L2Nu','WWToLNuQQ_BIG','WZTo1L1Nu2Q','WJetsToLNuHTBinBIG',
                       'TTTo2L2Nu_forTTH','TTWJetsToLNu_BIG', 'TGJets_BIG',
                       # 'QCDPtBinMuEMEnriched', 
                       'T_tWch', 'T_tch_powheg', 'TBar_tWch', 'TBar_tch_powheg']

    mcnonresoPlotters=[]
    for sample in mcnonresoSamples:
        mcnonresoPlotters.append(TreePlotter(sample, indir+'/'+sample+'.root','tree'))
        mcnonresoPlotters[-1].addCorrectionFactor('1./SumWeights','norm')
        mcnonresoPlotters[-1].addCorrectionFactor('xsec','xsec')
        mcnonresoPlotters[-1].addCorrectionFactor('genWeight','genWeight')
        mcnonresoPlotters[-1].addCorrectionFactor(puWeight,'puWeight')
        mcnonresoPlotters[-1].addCorrectionFactor(lepsf,'lepsf')
        mcnonresoPlotters[-1].addCorrectionFactor(mc_scale,'mc_scale')
        mcnonresoPlotters[-1].setAlias('passMuHLT', '((llnunu_l1_l1_trigerob_HLTbit>>3&1)||(llnunu_l1_l1_trigerob_HLTbit>>4&1)||(llnunu_l1_l2_trigerob_HLTbit>>3&1)||(llnunu_l1_l2_trigerob_HLTbit>>4&1))');
        mcnonresoPlotters[-1].setAlias('passElHLT', '((llnunu_l1_l1_trigerob_HLTbit>>1&1)||(llnunu_l1_l2_trigerob_HLTbit>>1&1))');
        mcnonresoPlotters[-1].addCorrectionFactor('(passMuHLT||passElHLT)','HLT')
    
    MCNONRESO = MergedPlotter(mcnonresoPlotters)
    MCNONRESO.setFillProperties(1001,ROOT.kAzure-9)

    # some plotting definition
    MCNONRESO.setAlias('llnunu_l1_mass_to_plot', 'llnunu_l1_mass')
    MCNONRESO.setAlias('llnunu_l2_pt_to_plot', 'llnunu_l2_pt')
    MCNONRESO.setAlias('llnunu_MET', 'llnunu_l2_pt')
    MCNONRESO.setAlias('llnunu_l2_phi_to_plot', 'llnunu_l2_phi')
    MCNONRESO.setAlias('llnunu_mt_to_plot', 'llnunu_mt')

    for syst in ['JetEn','JetRes','MuonEn','ElectronEn','TauEn','PhotonEn','Uncluster','Recoil'] :
        NONRES.setAlias('llnunu_mT_'+syst+'Up', 'llnunu_mt')
        NONRES.setAlias('llnunu_mT_'+syst+'Dn', 'llnunu_mt')
        NONRES.setAlias('llnunu_MET_'+syst+'Up', 'llnunu_l2_t1Pt_'+syst+'Up')
        NONRES.setAlias('llnunu_MET_'+syst+'Dn', 'llnunu_l2_t1Pt_'+syst+'Dn')

    for syst in ['Recoil'] :
        NONRES.setAlias('llnunu_MET_'+syst+'Up', 'llnunu_l2_pt')
        NONRES.setAlias('llnunu_MET_'+syst+'Dn', 'llnunu_l2_pt')

################################
# ZJets backgrounds
######################

# if use GJets to describe ZJets
if dyGJets : 

    # parameters for GJets
    el_gjet_scale=1.00
    mu_gjet_scale=1.00 

    if cutChain=='SR': 
        el_gjet_scale=1.01720174117
        mu_gjet_scale=1.0452050458
    else:
        el_gjet_scale=1.02810944619 
        mu_gjet_scale=1.00061373085

    gdataYield = 3402037584.2277574539
    zjetsFidXsecAll = 72.39368615170057808
    zjetsFidXsecEl =  1.8368830484768923217
    zjetsFidXsecMu =  70.494245975345435795
    zjetsFidXsecAll_up = 73.340989238570472253
    zjetsFidXsecAll_dn = 71.45465868226966677
    zjetsFidXsecEl_up = 1.9004022884222013801
    zjetsFidXsecEl_dn = 1.7743842806529528389
    zjetsFidXsecMu_up = 71.375949126216639229
    zjetsFidXsecMu_dn = 69.619765151461066921
    zjetsFidXsecLowLptAll = 1119.9216265291902346
    zjetsFidXsecLowLptEl = 459.14012486577632899
    zjetsFidXsecLowLptMu = 660.78150166340503802

    gdataLumi=35.867*1000
    gdataFidXsec=gdataYield/gdataLumi
    zjetsFidXsecEl*=el_gjet_scale
    zjetsFidXsecMu*=mu_gjet_scale
    zjetsFidXsecEl_up*=el_gjet_scale
    zjetsFidXsecEl_dn*=el_gjet_scale
    zjetsFidXsecMu_up*=mu_gjet_scale
    zjetsFidXsecMu_dn*=mu_gjet_scale

    # for GJets photon bkg subtraction

    phymetSamples = [
    'G_DYJetsToLL_M50_Ext',
    'G_TBar_tWch',
    'G_TBar_tch_powheg',
    'G_TGJets_BIG', 
    'G_TTGJets_BIG', 
    'G_T_tWch',  
    'G_T_tch_powheg', 
    'G_WGToLNuG', 
    'G_ZNuNuGJetsGt130', 
    'G_ZNuNuGJetsGt40Lt130',
    'G_WJetsToLNu_HT100to200_BIG',
    'G_WJetsToLNu_HT1200to2500_BIG',
    'G_WJetsToLNu_HT200to400_BIG',
    'G_WJetsToLNu_HT2500toInf_BIG',
    'G_WJetsToLNu_HT400to600_BIG',
    'G_WJetsToLNu_HT600to800_BIG',
    'G_WJetsToLNu_HT800to1200_BIG',
    'G_ZJetsToNuNu_HT100to200_BIG',
    'G_ZJetsToNuNu_HT1200to2500_BIG',
    'G_ZJetsToNuNu_HT200to400_BIG',
    'G_ZJetsToNuNu_HT2500toInf_BIG',
    'G_ZJetsToNuNu_HT400to600_BIG',
    'G_ZJetsToNuNu_HT600to800_BIG',
    'G_ZJetsToNuNu_HT800t1200_BIG',
    ]

    # all the factors below together normalized each process to the fraction of the process in the gjets data
    #   fidxsec_i / fidxsec_total
    # together with the gdata, we have:
    #  [ fidxsec_total-Sum(fidxsec_i) ]/fidxsec_total * fidxsec_zjets * lumi = zjets_yields
    # an additional scale factor GJetsNorm to absorbe the small difference.

    phymetPlotters=[]
    for sample in phymetSamples:
        phymetPlotters.append(TreePlotter(sample, indir+'/'+sample+'.root','tree'))
        phymetPlotters[-1].addCorrectionFactor('-1/SumWeights','norm') # negative weight for subtraction
        if sample=='G_ZNuNuGJetsGt130': phymetPlotters[-1].addCorrectionFactor('0.1832*1.43','xsec')  # NNLO/LO k-factor from JHEP02 (2016) 057, Table 2
        elif sample=='G_ZNuNuGJetsGt40Lt130': phymetPlotters[-1].addCorrectionFactor('xsec*1.43','xsec')
        elif sample=='G_WGJetsPt130':  phymetPlotters[-1].addCorrectionFactor('0.834*2.53','xsec')  # NNLO/LO k-factor from JHEP04 (2015) 018, Table 1
        elif sample=='G_WGToLNuG':  phymetPlotters[-1].addCorrectionFactor('xsec*2.53','xsec')  # NNLO/LO k-factor from JHEP04 (2015) 018, Table 1
        else: phymetPlotters[-1].addCorrectionFactor('xsec','xsec')
        phymetPlotters[-1].addCorrectionFactor('genWeight','genWeight')
        phymetPlotters[-1].addCorrectionFactor('GJetsRhoWeight','GJetsRhoWeight')
        phymetPlotters[-1].addCorrectionFactor(puWeight,'puWeight')
        # parametrized single photon trigger eff
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
    gdataSamples = [
    'SinglePhoton_Run2016Full_03Feb2017_allcorV2', 
    ]

    gdataPlotters=[]
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
    gjetsPlotters = gdataPlotters+phymetPlotters

    GJets = MergedPlotter(gjetsPlotters)
    GJets.setFillProperties(1001,ROOT.kGreen+2)

    # some plotting definition
    GJets.setAlias('llnunu_mt_to_plot', 'llnunu_mt')

    if channel=='el':
        GJets.setAlias('llnunu_l1_mass_to_plot', 'llnunu_l1_mass_el')
        GJets.setAlias('llnunu_l2_pt_to_plot', 'llnunu_l2_pt_el')
        GJets.setAlias('llnunu_MET', 'llnunu_l2_pt_el')
        GJets.setAlias('llnunu_l2_phi_to_plot', 'llnunu_l2_phi_el')
        GJets.setAlias('llnunu_mt_to_plot', 'llnunu_mt_el')
    elif channel=='mu':
        GJets.setAlias('llnunu_l1_mass_to_plot', 'llnunu_l1_mass_mu')
        GJets.setAlias('llnunu_l2_pt_to_plot', 'llnunu_l2_pt_mu')
        GJets.setAlias('llnunu_MET', 'llnunu_l2_pt_mu')
        GJets.setAlias('llnunu_l2_phi_to_plot', 'llnunu_l2_phi_mu')
        GJets.setAlias('llnunu_mt_to_plot', 'llnunu_mt_mu')
    else:
        GJets.setAlias('llnunu_l1_mass_to_plot', 'llnunu_l1_mass')
        GJets.setAlias('llnunu_l2_pt_to_plot', 'llnunu_l2_pt')
        GJets.setAlias('llnunu_MET', 'llnunu_l2_pt')
        GJets.setAlias('llnunu_l2_phi_to_plot', 'llnunu_l2_phi')
        GJets.setAlias('llnunu_mt_to_plot', 'llnunu_mt')

#if not dyGJets
else: 
    ### MC ZJets
    mczjetsSamples = [
    'DYJetsToLL_M50_Ext',
#    'DYJetsToLL_M50_MGMLM_BIG_NoRecoil',
    ]

    mczjetsPlotters=[]
    for sample in mczjetsSamples:
        mczjetsPlotters.append(TreePlotter(sample, indir+'/'+sample+'.root','tree'))
        mczjetsPlotters[-1].addCorrectionFactor('1./SumWeights','norm')
        mczjetsPlotters[-1].addCorrectionFactor(ZPtWeight,'ZPtWeight')
        if 'DY1JetsToLL' in sample: mczjetsPlotters[-1].addCorrectionFactor('(1177.7274)', 'xsec')
        elif 'DY2JetsToLL' in sample: mczjetsPlotters[-1].addCorrectionFactor('(389.1267)', 'xsec')
        elif 'DY3JetsToLL' in sample: mczjetsPlotters[-1].addCorrectionFactor('(119.0516)', 'xsec')
        elif 'DY4JetsToLL' in sample: mczjetsPlotters[-1].addCorrectionFactor('(63.3043)', 'xsec')
        else: mczjetsPlotters[-1].addCorrectionFactor('xsec','xsec') 
        mczjetsPlotters[-1].addCorrectionFactor('genWeight','genWeight')
        mczjetsPlotters[-1].addCorrectionFactor(puWeight,'puWeight')
        mczjetsPlotters[-1].addCorrectionFactor(lepsf,'lepsf')
        mczjetsPlotters[-1].addCorrectionFactor(mc_scale,'mc_scale')
        mczjetsPlotters[-1].addCorrectionFactor(zjets_scale,'zjets_scale')
        mczjetsPlotters[-1].setAlias('passMuHLT', '((llnunu_l1_l1_trigerob_HLTbit>>3&1)||(llnunu_l1_l1_trigerob_HLTbit>>4&1)||(llnunu_l1_l2_trigerob_HLTbit>>3&1)||(llnunu_l1_l2_trigerob_HLTbit>>4&1))');
        mczjetsPlotters[-1].setAlias('passElHLT', '((llnunu_l1_l1_trigerob_HLTbit>>1&1)||(llnunu_l1_l2_trigerob_HLTbit>>1&1))');
        mczjetsPlotters[-1].addCorrectionFactor('(passMuHLT||passElHLT)','HLT') 

    MCZJets = MergedPlotter(mczjetsPlotters)
    MCZJets.setFillProperties(1001,ROOT.kGreen+2)

    # some plotting definition
    MCZJets.setAlias('llnunu_l1_mass_to_plot', 'llnunu_l1_mass')
    MCZJets.setAlias('llnunu_l2_pt_to_plot', 'llnunu_l2_pt')
    MCZJets.setAlias('llnunu_MET', 'llnunu_l2_pt')
    MCZJets.setAlias('llnunu_l2_phi_to_plot', 'llnunu_l2_phi')
    MCZJets.setAlias('llnunu_mt_to_plot', 'llnunu_mt')

# choose GJets or ZJets MC
if dyGJets:
  ZJets = GJets
else: 
  ZJets = MCZJets

if dyGJets and channel=='el':
    ZJets.setAlias('llnunu_l1_mass_to_plot', 'llnunu_l1_mass_el')
    ZJets.setAlias('llnunu_l2_pt_to_plot', 'llnunu_l2_pt_el')
    ZJets.setAlias('llnunu_MET', 'llnunu_l2_pt_el')
    ZJets.setAlias('llnunu_l2_phi_to_plot', 'llnunu_l2_phi_el')
    ZJets.setAlias('llnunu_mt_to_plot', 'llnunu_mt_el')
    ZJets.setAlias('llnunu_mT', 'llnunu_mt_el')
    for syst in ['JetEn','JetRes','MuonEn','ElectronEn','TauEn','PhotonEn','Uncluster'] :
        ZJets.setAlias('llnunu_mT_'+syst+'Up', 'llnunu_mt_el_'+syst+'Up')
        ZJets.setAlias('llnunu_mT_'+syst+'Dn', 'llnunu_mt_el_'+syst+'Dn')
        ZJets.setAlias('llnunu_MET_'+syst+'Up', 'llnunu_l2_pt_el')
        ZJets.setAlias('llnunu_MET_'+syst+'Dn', 'llnunu_l2_pt_el')

    ZJets.setAlias('llnunu_mT_RecoilUp', 'llnunu_mt_el_RecoilUp')
    ZJets.setAlias('llnunu_mT_RecoilDn', 'llnunu_mt_el_RecoilDn')
    ZJets.setAlias('llnunu_MET_RecoilUp', 'llnunu_l2_pt_el_RecoilUp')
    ZJets.setAlias('llnunu_MET_RecoilDn', 'llnunu_l2_pt_el_RecoilDn')

elif dyGJets and channel=='mu':
    ZJets.setAlias('llnunu_l1_mass_to_plot', 'llnunu_l1_mass_mu')
    ZJets.setAlias('llnunu_l2_pt_to_plot', 'llnunu_l2_pt_mu')
    ZJets.setAlias('llnunu_MET', 'llnunu_l2_pt_mu')
    ZJets.setAlias('llnunu_l2_phi_to_plot', 'llnunu_l2_phi_mu')
    ZJets.setAlias('llnunu_mt_to_plot', 'llnunu_mt_mu')
    ZJets.setAlias('llnunu_mT', 'llnunu_mt_mu')
    for syst in ['JetEn','JetRes','MuonEn','ElectronEn','TauEn','PhotonEn','Uncluster'] :
        ZJets.setAlias('llnunu_mT_'+syst+'Up', 'llnunu_mt_mu_'+syst+'Up')
        ZJets.setAlias('llnunu_mT_'+syst+'Dn', 'llnunu_mt_mu_'+syst+'Dn')
        ZJets.setAlias('llnunu_MET_'+syst+'Up', 'llnunu_l2_pt_mu')
        ZJets.setAlias('llnunu_MET_'+syst+'Dn', 'llnunu_l2_pt_mu')

    ZJets.setAlias('llnunu_mT_RecoilUp', 'llnunu_mt_mu_RecoilUp')
    ZJets.setAlias('llnunu_mT_RecoilDn', 'llnunu_mt_mu_RecoilDn')
    ZJets.setAlias('llnunu_MET_RecoilUp', 'llnunu_l2_pt_mu_RecoilUp')
    ZJets.setAlias('llnunu_MET_RecoilDn', 'llnunu_l2_pt_mu_RecoilDn')

else:

    ZJets.setAlias('llnunu_l1_mass_to_plot', 'llnunu_l1_mass')
    ZJets.setAlias('llnunu_l2_pt_to_plot', 'llnunu_l2_pt')
    ZJets.setAlias('llnunu_MET', 'llnunu_l2_pt')
    ZJets.setAlias('llnunu_l2_phi_to_plot', 'llnunu_l2_phi')
    ZJets.setAlias('llnunu_mt_to_plot', 'llnunu_mt')
    ZJets.setAlias('llnunu_mT', 'llnunu_mt')
    for syst in ['JetEn','JetRes','MuonEn','ElectronEn','TauEn','PhotonEn','Uncluster'] :
        ZJets.setAlias('llnunu_mT_'+syst+'Up', 'llnunu_mt')
        ZJets.setAlias('llnunu_mT_'+syst+'Dn', 'llnunu_mt')
        ZJets.setAlias('llnunu_MET_'+syst+'Up', 'llnunu_l2_pt')
        ZJets.setAlias('llnunu_MET_'+syst+'Dn', 'llnunu_l2_pt')

    ZJets.setAlias('llnunu_mT_RecoilUp', 'llnunu_mt_RecoilUp')
    ZJets.setAlias('llnunu_mT_RecoilDn', 'llnunu_mt_RecoilDn')
    ZJets.setAlias('llnunu_MET_RecoilUp', 'llnunu_l2_pt_RecoilUp')
    ZJets.setAlias('llnunu_MET_RecoilDn', 'llnunu_l2_pt_RecoilDn')

######################
# Signal samples
#####################

sigSamples = [
'BulkGravToZZToZlepZinv_narrow_200',
'BulkGravToZZToZlepZinv_narrow_300',
'BulkGravToZZToZlepZinv_narrow_400',
'BulkGravToZZToZlepZinv_narrow_500',
'BulkGravToZZToZlepZinv_narrow_600',
'BulkGravToZZToZlepZinv_narrow_700',
'BulkGravToZZToZlepZinv_narrow_800',
'BulkGravToZZToZlepZinv_narrow_900',
'BulkGravToZZToZlepZinv_narrow_1000',
'BulkGravToZZToZlepZinv_narrow_1100',
'BulkGravToZZToZlepZinv_narrow_1200',
'BulkGravToZZToZlepZinv_narrow_1300',
'BulkGravToZZToZlepZinv_narrow_1400',
'BulkGravToZZToZlepZinv_narrow_1500',
'BulkGravToZZToZlepZinv_narrow_1600',
'BulkGravToZZToZlepZinv_narrow_1800',
'BulkGravToZZToZlepZinv_narrow_2000',
'BulkGravToZZToZlepZinv_narrow_2500',
'BulkGravToZZToZlepZinv_narrow_3000',
'BulkGravToZZToZlepZinv_narrow_3500',
'BulkGravToZZToZlepZinv_narrow_4000',
'BulkGravToZZToZlepZinv_narrow_4500',
]

sigPlotters=[]
sigSampleNames = {
'BulkGravToZZToZlepZinv_narrow_200':str(k)+' x BulkG-200',
'BulkGravToZZToZlepZinv_narrow_300':str(k)+' x BulkG-300',
'BulkGravToZZToZlepZinv_narrow_400':str(k)+' x BulkG-400',
'BulkGravToZZToZlepZinv_narrow_500':str(k)+' x BulkG-500',
'BulkGravToZZToZlepZinv_narrow_600':str(k)+' x BulkG-600',
'BulkGravToZZToZlepZinv_narrow_700':str(k)+' x BulkG-700',
'BulkGravToZZToZlepZinv_narrow_800':str(k)+' x BulkG-800',
'BulkGravToZZToZlepZinv_narrow_900':str(k)+' x BulkG-900',
'BulkGravToZZToZlepZinv_narrow_1000':str(k)+' x BulkG-1000',
'BulkGravToZZToZlepZinv_narrow_1100':str(k)+' x BulkG-1100',
'BulkGravToZZToZlepZinv_narrow_1200':str(k)+' x BulkG-1200',
'BulkGravToZZToZlepZinv_narrow_1300':str(k)+' x BulkG-1300',
'BulkGravToZZToZlepZinv_narrow_1400':str(k)+' x BulkG-1400',
'BulkGravToZZToZlepZinv_narrow_1500':str(k)+' x BulkG-1500',
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
200:1.80565e+00,
300:3.83982e-01,
400:9.53589e-02,
500:2.56377e-02,
600:8.61578e-03,
700:3.45608e-03,
800:1.57965e-03,
900:7.89276e-04,
1000:4.21651e-04,
1100:2.38382e-04,
1200:1.39919e-04,
1300:8.50521e-05,
1400:5.32921e-05,
1500:3.43731e-05,
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
'BulkGravToZZToZlepZinv_narrow_200'  : BulkGZZ2l2nuXsec[200]*k,
'BulkGravToZZToZlepZinv_narrow_300'  : BulkGZZ2l2nuXsec[300]*k,
'BulkGravToZZToZlepZinv_narrow_400'  : BulkGZZ2l2nuXsec[400]*k,
'BulkGravToZZToZlepZinv_narrow_500'  : BulkGZZ2l2nuXsec[500]*k,
'BulkGravToZZToZlepZinv_narrow_600'  : BulkGZZ2l2nuXsec[600]*k,
'BulkGravToZZToZlepZinv_narrow_700'  : BulkGZZ2l2nuXsec[700]*k,
'BulkGravToZZToZlepZinv_narrow_800'  : BulkGZZ2l2nuXsec[800]*k,
'BulkGravToZZToZlepZinv_narrow_900'  : BulkGZZ2l2nuXsec[900]*k,
'BulkGravToZZToZlepZinv_narrow_1000' : BulkGZZ2l2nuXsec[1000]*k,
'BulkGravToZZToZlepZinv_narrow_1100' : BulkGZZ2l2nuXsec[1100]*k,
'BulkGravToZZToZlepZinv_narrow_1200' : BulkGZZ2l2nuXsec[1200]*k,
'BulkGravToZZToZlepZinv_narrow_1300' : BulkGZZ2l2nuXsec[1300]*k,
'BulkGravToZZToZlepZinv_narrow_1400' : BulkGZZ2l2nuXsec[1400]*k,
'BulkGravToZZToZlepZinv_narrow_1500' : BulkGZZ2l2nuXsec[1500]*k,
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
    sigPlotters[-1].setAlias('passMuHLT', '((llnunu_l1_l1_trigerob_HLTbit>>3&1)||(llnunu_l1_l1_trigerob_HLTbit>>4&1)||(llnunu_l1_l2_trigerob_HLTbit>>3&1)||(llnunu_l1_l2_trigerob_HLTbit>>4&1))');
    sigPlotters[-1].setAlias('passElHLT', '((llnunu_l1_l1_trigerob_HLTbit>>1&1)||(llnunu_l1_l2_trigerob_HLTbit>>1&1))');
    sigPlotters[-1].addCorrectionFactor('(passMuHLT||passElHLT)','HLT')
    sigPlotters[-1].setAlias('llnunu_l1_mass_to_plot', 'llnunu_l1_mass')
    sigPlotters[-1].setAlias('llnunu_l2_pt_to_plot', 'llnunu_l2_pt')
    sigPlotters[-1].setAlias('llnunu_MET', 'llnunu_l2_pt')
    sigPlotters[-1].setAlias('llnunu_l2_phi_to_plot', 'llnunu_l2_phi')
    sigPlotters[-1].setAlias('llnunu_mt_to_plot', 'llnunu_mt')
    sigPlotters[-1].setAlias('llnunu_mT', 'llnunu_mt')
    for syst in ['JetEn','JetRes','MuonEn','ElectronEn','TauEn','PhotonEn','Uncluster'] :
        sigPlotters[-1].setAlias('llnunu_mT_'+syst+'Up', 'llnunu_mt_'+syst+'Up')
        sigPlotters[-1].setAlias('llnunu_mT_'+syst+'Dn', 'llnunu_mt_'+syst+'Dn')
        sigPlotters[-1].setAlias('llnunu_MET_'+syst+'Up', 'llnunu_l2_t1Pt_'+syst+'Up')
        sigPlotters[-1].setAlias('llnunu_MET_'+syst+'Dn', 'llnunu_l2_t1Pt_'+syst+'Dn')

    sigPlotters[-1].setAlias('llnunu_mT_RecoilUp', 'llnunu_mt')
    sigPlotters[-1].setAlias('llnunu_mT_RecoilDn', 'llnunu_mt')
    sigPlotters[-1].setAlias('llnunu_MET_RecoilUp', 'llnunu_l2_pt')
    sigPlotters[-1].setAlias('llnunu_MET_RecoilDn', 'llnunu_l2_pt')

### ggH

ggHPlotters=[]

ggHSamples = [
'GluGluHToZZTo2L2Nu_M200',
'GluGluHToZZTo2L2Nu_M300',
'GluGluHToZZTo2L2Nu_M400',
'GluGluHToZZTo2L2Nu_M500',
'GluGluHToZZTo2L2Nu_M600',
'GluGluHToZZTo2L2Nu_M700',
'GluGluHToZZTo2L2Nu_M800',
'GluGluHToZZTo2L2Nu_M900',
'GluGluHToZZTo2L2Nu_M1000',
'GluGluHToZZTo2L2Nu_M1500',
'GluGluHToZZTo2L2Nu_M2000',
'GluGluHToZZTo2L2Nu_M2500',
'GluGluHToZZTo2L2Nu_M3000',

'VBF_HToZZTo2L2Nu_200',
'VBF_HToZZTo2L2Nu_300',
'VBF_HToZZTo2L2Nu_400',
'VBF_HToZZTo2L2Nu_500',
'VBF_HToZZTo2L2Nu_600',
'VBF_HToZZTo2L2Nu_700',
'VBF_HToZZTo2L2Nu_800',
'VBF_HToZZTo2L2Nu_900',
'VBF_HToZZTo2L2Nu_1000',
'VBF_HToZZTo2L2Nu_1500',
'VBF_HToZZTo2L2Nu_2000',
'VBF_HToZZTo2L2Nu_2500',
'VBF_HToZZTo2L2Nu_3000'
]

for sample in ggHSamples:
    ggHPlotters.append(TreePlotter(sample, indir+'/'+sample+'.root','tree'))
    ggHPlotters[-1].addCorrectionFactor('1./SumWeights','norm')
    ggHPlotters[-1].addCorrectionFactor(str(1),'xsec')
    ggHPlotters[-1].addCorrectionFactor('genWeight','genWeight')
    ggHPlotters[-1].addCorrectionFactor(puWeight,'puWeight')
    ggHPlotters[-1].addCorrectionFactor(lepsf,'lepsf')
    ggHPlotters[-1].addCorrectionFactor(mc_scale,'mc_scale')
    ggHPlotters[-1].setFillProperties(0,ROOT.kWhite)
    ggHPlotters[-1].setAlias('passMuHLT', '((llnunu_l1_l1_trigerob_HLTbit>>3&1)||(llnunu_l1_l1_trigerob_HLTbit>>4&1)||(llnunu_l1_l2_trigerob_HLTbit>>3&1)||(llnunu_l1_l2_trigerob_HLTbit>>4&1))');
    ggHPlotters[-1].setAlias('passElHLT', '((llnunu_l1_l1_trigerob_HLTbit>>1&1)||(llnunu_l1_l2_trigerob_HLTbit>>1&1))');
    ggHPlotters[-1].addCorrectionFactor('(passMuHLT||passElHLT)','HLT')
    ggHPlotters[-1].setAlias('llnunu_l1_mass_to_plot', 'llnunu_l1_mass')
    ggHPlotters[-1].setAlias('llnunu_l2_pt_to_plot', 'llnunu_l2_pt')
    ggHPlotters[-1].setAlias('llnunu_MET', 'llnunu_l2_pt')
    ggHPlotters[-1].setAlias('llnunu_l2_phi_to_plot', 'llnunu_l2_phi')
    ggHPlotters[-1].setAlias('llnunu_mt_to_plot', 'llnunu_mt')
    ggHPlotters[-1].setAlias('llnunu_mT', 'llnunu_mt')
    for syst in ['JetEn','JetRes','MuonEn','ElectronEn','TauEn','PhotonEn','Uncluster'] :
        ggHPlotters[-1].setAlias('llnunu_mT_'+syst+'Up', 'llnunu_mt_'+syst+'Up')
        ggHPlotters[-1].setAlias('llnunu_mT_'+syst+'Dn', 'llnunu_mt_'+syst+'Dn')
        ggHPlotters[-1].setAlias('llnunu_MET_'+syst+'Up', 'llnunu_l2_t1Pt_'+syst+'Up')
        ggHPlotters[-1].setAlias('llnunu_MET_'+syst+'Dn', 'llnunu_l2_t1Pt_'+syst+'Dn')

    ggHPlotters[-1].setAlias('llnunu_mT_RecoilUp', 'llnunu_mt')
    ggHPlotters[-1].setAlias('llnunu_mT_RecoilDn', 'llnunu_mt')
    ggHPlotters[-1].setAlias('llnunu_MET_RecoilUp', 'llnunu_l2_pt')
    ggHPlotters[-1].setAlias('llnunu_MET_RecoilDn', 'llnunu_l2_pt')


### Graviton2PBqqbar
Graviton2PBPlotters=[]

Graviton2PBSamples = [
'Graviton2PBqqbarToZZTo2L2Nu_width0_200',
'Graviton2PBqqbarToZZTo2L2Nu_width0_300',
'Graviton2PBqqbarToZZTo2L2Nu_width0_400',
'Graviton2PBqqbarToZZTo2L2Nu_width0_500',
'Graviton2PBqqbarToZZTo2L2Nu_width0_600',
'Graviton2PBqqbarToZZTo2L2Nu_width0_700',
'Graviton2PBqqbarToZZTo2L2Nu_width0_750',
'Graviton2PBqqbarToZZTo2L2Nu_width0_800',
'Graviton2PBqqbarToZZTo2L2Nu_width0_900',
'Graviton2PBqqbarToZZTo2L2Nu_width0_1000',
'Graviton2PBqqbarToZZTo2L2Nu_width0_1100',
'Graviton2PBqqbarToZZTo2L2Nu_width0_1200',
'Graviton2PBqqbarToZZTo2L2Nu_width0_1300',
'Graviton2PBqqbarToZZTo2L2Nu_width0_1400',
'Graviton2PBqqbarToZZTo2L2Nu_width0_1500',
'Graviton2PBqqbarToZZTo2L2Nu_width0_1600',
'Graviton2PBqqbarToZZTo2L2Nu_width0_1800',
'Graviton2PBqqbarToZZTo2L2Nu_width0_2000',
'Graviton2PBqqbarToZZTo2L2Nu_width0_2500',
'Graviton2PBqqbarToZZTo2L2Nu_width0_3000',
'Graviton2PBqqbarToZZTo2L2Nu_width0_3500',
'Graviton2PBqqbarToZZTo2L2Nu_width0_4000',


'Graviton2PBqqbarToZZTo2L2Nu_width0p1_200',
'Graviton2PBqqbarToZZTo2L2Nu_width0p1_300',
'Graviton2PBqqbarToZZTo2L2Nu_width0p1_400',
'Graviton2PBqqbarToZZTo2L2Nu_width0p1_500',
'Graviton2PBqqbarToZZTo2L2Nu_width0p1_600',
'Graviton2PBqqbarToZZTo2L2Nu_width0p1_700',
'Graviton2PBqqbarToZZTo2L2Nu_width0p1_750',
'Graviton2PBqqbarToZZTo2L2Nu_width0p1_800',
'Graviton2PBqqbarToZZTo2L2Nu_width0p1_900',
'Graviton2PBqqbarToZZTo2L2Nu_width0p1_1000',
'Graviton2PBqqbarToZZTo2L2Nu_width0p1_1100',
'Graviton2PBqqbarToZZTo2L2Nu_width0p1_1200',
'Graviton2PBqqbarToZZTo2L2Nu_width0p1_1300',
'Graviton2PBqqbarToZZTo2L2Nu_width0p1_1400',
'Graviton2PBqqbarToZZTo2L2Nu_width0p1_1500',
'Graviton2PBqqbarToZZTo2L2Nu_width0p1_1600',
'Graviton2PBqqbarToZZTo2L2Nu_width0p1_1800',
'Graviton2PBqqbarToZZTo2L2Nu_width0p1_2000',
'Graviton2PBqqbarToZZTo2L2Nu_width0p1_2500',
'Graviton2PBqqbarToZZTo2L2Nu_width0p1_3000',
'Graviton2PBqqbarToZZTo2L2Nu_width0p1_3500',
'Graviton2PBqqbarToZZTo2L2Nu_width0p1_4000',

'Graviton2PBqqbarToZZTo2L2Nu_width0p2_200',
'Graviton2PBqqbarToZZTo2L2Nu_width0p2_300',
'Graviton2PBqqbarToZZTo2L2Nu_width0p2_400',
'Graviton2PBqqbarToZZTo2L2Nu_width0p2_500',
'Graviton2PBqqbarToZZTo2L2Nu_width0p2_600',
'Graviton2PBqqbarToZZTo2L2Nu_width0p2_700',
'Graviton2PBqqbarToZZTo2L2Nu_width0p2_750',
'Graviton2PBqqbarToZZTo2L2Nu_width0p2_800',
'Graviton2PBqqbarToZZTo2L2Nu_width0p2_900',
'Graviton2PBqqbarToZZTo2L2Nu_width0p2_1000',
'Graviton2PBqqbarToZZTo2L2Nu_width0p2_1100',
'Graviton2PBqqbarToZZTo2L2Nu_width0p2_1200',
'Graviton2PBqqbarToZZTo2L2Nu_width0p2_1300',
'Graviton2PBqqbarToZZTo2L2Nu_width0p2_1400',
'Graviton2PBqqbarToZZTo2L2Nu_width0p2_1500',
'Graviton2PBqqbarToZZTo2L2Nu_width0p2_1600',
'Graviton2PBqqbarToZZTo2L2Nu_width0p2_1800',
'Graviton2PBqqbarToZZTo2L2Nu_width0p2_2000',
'Graviton2PBqqbarToZZTo2L2Nu_width0p2_2500',
'Graviton2PBqqbarToZZTo2L2Nu_width0p2_3000',
'Graviton2PBqqbarToZZTo2L2Nu_width0p2_3500',
'Graviton2PBqqbarToZZTo2L2Nu_width0p2_4000',

'Graviton2PBqqbarToZZTo2L2Nu_width0p3_200',
'Graviton2PBqqbarToZZTo2L2Nu_width0p3_300',
'Graviton2PBqqbarToZZTo2L2Nu_width0p3_400',
'Graviton2PBqqbarToZZTo2L2Nu_width0p3_500',
'Graviton2PBqqbarToZZTo2L2Nu_width0p3_600',
'Graviton2PBqqbarToZZTo2L2Nu_width0p3_700',
'Graviton2PBqqbarToZZTo2L2Nu_width0p3_750',
'Graviton2PBqqbarToZZTo2L2Nu_width0p3_800',
'Graviton2PBqqbarToZZTo2L2Nu_width0p3_900',
'Graviton2PBqqbarToZZTo2L2Nu_width0p3_1000',
'Graviton2PBqqbarToZZTo2L2Nu_width0p3_1100',
'Graviton2PBqqbarToZZTo2L2Nu_width0p3_1200',
'Graviton2PBqqbarToZZTo2L2Nu_width0p3_1300',
'Graviton2PBqqbarToZZTo2L2Nu_width0p3_1400',
'Graviton2PBqqbarToZZTo2L2Nu_width0p3_1500',
'Graviton2PBqqbarToZZTo2L2Nu_width0p3_1600',
'Graviton2PBqqbarToZZTo2L2Nu_width0p3_1800',
'Graviton2PBqqbarToZZTo2L2Nu_width0p3_2000',
'Graviton2PBqqbarToZZTo2L2Nu_width0p3_2500',
'Graviton2PBqqbarToZZTo2L2Nu_width0p3_3000',
'Graviton2PBqqbarToZZTo2L2Nu_width0p3_3500',
'Graviton2PBqqbarToZZTo2L2Nu_width0p3_4000',

'Graviton2PBToZZTo2L2Nu_width0_200',
'Graviton2PBToZZTo2L2Nu_width0_300',
'Graviton2PBToZZTo2L2Nu_width0_400',
'Graviton2PBToZZTo2L2Nu_width0_500',
'Graviton2PBToZZTo2L2Nu_width0_600',
'Graviton2PBToZZTo2L2Nu_width0_700',
'Graviton2PBToZZTo2L2Nu_width0_750',
'Graviton2PBToZZTo2L2Nu_width0_800',
'Graviton2PBToZZTo2L2Nu_width0_900',
'Graviton2PBToZZTo2L2Nu_width0_1000',
'Graviton2PBToZZTo2L2Nu_width0_1100',
'Graviton2PBToZZTo2L2Nu_width0_1200',
'Graviton2PBToZZTo2L2Nu_width0_1300',
'Graviton2PBToZZTo2L2Nu_width0_1400',
'Graviton2PBToZZTo2L2Nu_width0_1500',
'Graviton2PBToZZTo2L2Nu_width0_1600',
'Graviton2PBToZZTo2L2Nu_width0_1800',
'Graviton2PBToZZTo2L2Nu_width0_2000',
'Graviton2PBToZZTo2L2Nu_width0_2500',
'Graviton2PBToZZTo2L2Nu_width0_3000',
'Graviton2PBToZZTo2L2Nu_width0_3500',
'Graviton2PBToZZTo2L2Nu_width0_4000',

'Graviton2PBToZZTo2L2Nu_width0p1_200',
'Graviton2PBToZZTo2L2Nu_width0p1_300',
'Graviton2PBToZZTo2L2Nu_width0p1_400',
'Graviton2PBToZZTo2L2Nu_width0p1_500',
'Graviton2PBToZZTo2L2Nu_width0p1_600',
'Graviton2PBToZZTo2L2Nu_width0p1_700',
'Graviton2PBToZZTo2L2Nu_width0p1_750',
'Graviton2PBToZZTo2L2Nu_width0p1_800',
'Graviton2PBToZZTo2L2Nu_width0p1_900',
'Graviton2PBToZZTo2L2Nu_width0p1_1000',
'Graviton2PBToZZTo2L2Nu_width0p1_1100',
'Graviton2PBToZZTo2L2Nu_width0p1_1200',
'Graviton2PBToZZTo2L2Nu_width0p1_1300',
'Graviton2PBToZZTo2L2Nu_width0p1_1400',
'Graviton2PBToZZTo2L2Nu_width0p1_1500',
'Graviton2PBToZZTo2L2Nu_width0p1_1600',
'Graviton2PBToZZTo2L2Nu_width0p1_1800',
'Graviton2PBToZZTo2L2Nu_width0p1_2000',
'Graviton2PBToZZTo2L2Nu_width0p1_2500',
'Graviton2PBToZZTo2L2Nu_width0p1_3000',
'Graviton2PBToZZTo2L2Nu_width0p1_3500',
'Graviton2PBToZZTo2L2Nu_width0p1_4000',

'Graviton2PBToZZTo2L2Nu_width0p2_200',
'Graviton2PBToZZTo2L2Nu_width0p2_300',
'Graviton2PBToZZTo2L2Nu_width0p2_400',
'Graviton2PBToZZTo2L2Nu_width0p2_500',
'Graviton2PBToZZTo2L2Nu_width0p2_600',
'Graviton2PBToZZTo2L2Nu_width0p2_700',
'Graviton2PBToZZTo2L2Nu_width0p2_750',
'Graviton2PBToZZTo2L2Nu_width0p2_800',
'Graviton2PBToZZTo2L2Nu_width0p2_900',
'Graviton2PBToZZTo2L2Nu_width0p2_1000',
'Graviton2PBToZZTo2L2Nu_width0p2_1100',
'Graviton2PBToZZTo2L2Nu_width0p2_1200',
'Graviton2PBToZZTo2L2Nu_width0p2_1300',
'Graviton2PBToZZTo2L2Nu_width0p2_1400',
'Graviton2PBToZZTo2L2Nu_width0p2_1500',
'Graviton2PBToZZTo2L2Nu_width0p2_1600',
'Graviton2PBToZZTo2L2Nu_width0p2_1800',
'Graviton2PBToZZTo2L2Nu_width0p2_2000',
'Graviton2PBToZZTo2L2Nu_width0p2_2500',
'Graviton2PBToZZTo2L2Nu_width0p2_3000',
'Graviton2PBToZZTo2L2Nu_width0p2_3500',
'Graviton2PBToZZTo2L2Nu_width0p2_4000',

'Graviton2PBToZZTo2L2Nu_width0p3_200',
'Graviton2PBToZZTo2L2Nu_width0p3_300',
'Graviton2PBToZZTo2L2Nu_width0p3_400',
'Graviton2PBToZZTo2L2Nu_width0p3_500',
'Graviton2PBToZZTo2L2Nu_width0p3_600',
'Graviton2PBToZZTo2L2Nu_width0p3_700',
'Graviton2PBToZZTo2L2Nu_width0p3_750',
'Graviton2PBToZZTo2L2Nu_width0p3_800',
'Graviton2PBToZZTo2L2Nu_width0p3_900',
'Graviton2PBToZZTo2L2Nu_width0p3_1000',
'Graviton2PBToZZTo2L2Nu_width0p3_1100',
'Graviton2PBToZZTo2L2Nu_width0p3_1200',
'Graviton2PBToZZTo2L2Nu_width0p3_1300',
'Graviton2PBToZZTo2L2Nu_width0p3_1400',
'Graviton2PBToZZTo2L2Nu_width0p3_1500',
'Graviton2PBToZZTo2L2Nu_width0p3_1600',
'Graviton2PBToZZTo2L2Nu_width0p3_1800',
'Graviton2PBToZZTo2L2Nu_width0p3_2000',
'Graviton2PBToZZTo2L2Nu_width0p3_2500',
'Graviton2PBToZZTo2L2Nu_width0p3_3000',
'Graviton2PBToZZTo2L2Nu_width0p3_3500',
'Graviton2PBToZZTo2L2Nu_width0p3_4000',

]

for sample in Graviton2PBSamples:

    Graviton2PBPlotters.append(TreePlotter(sample, indir+'/'+sample+'.root','tree'))
    Graviton2PBPlotters[-1].addCorrectionFactor('1./SumWeights','norm')
    Graviton2PBPlotters[-1].addCorrectionFactor(str(1),'xsec')
    Graviton2PBPlotters[-1].addCorrectionFactor('genWeight','genWeight')
    Graviton2PBPlotters[-1].addCorrectionFactor(puWeight,'puWeight')
    Graviton2PBPlotters[-1].addCorrectionFactor(lepsf,'lepsf')
    Graviton2PBPlotters[-1].addCorrectionFactor(mc_scale,'mc_scale')
    Graviton2PBPlotters[-1].setFillProperties(0,ROOT.kWhite)
    Graviton2PBPlotters[-1].setAlias('passMuHLT', '((llnunu_l1_l1_trigerob_HLTbit>>3&1)||(llnunu_l1_l1_trigerob_HLTbit>>4&1)||(llnunu_l1_l2_trigerob_HLTbit>>3&1)||(llnunu_l1_l2_trigerob_HLTbit>>4&1))');
    Graviton2PBPlotters[-1].setAlias('passElHLT', '((llnunu_l1_l1_trigerob_HLTbit>>1&1)||(llnunu_l1_l2_trigerob_HLTbit>>1&1))');
    Graviton2PBPlotters[-1].addCorrectionFactor('(passMuHLT||passElHLT)','HLT')
    Graviton2PBPlotters[-1].setAlias('llnunu_l1_mass_to_plot', 'llnunu_l1_mass')
    Graviton2PBPlotters[-1].setAlias('llnunu_l2_pt_to_plot', 'llnunu_l2_pt')
    Graviton2PBPlotters[-1].setAlias('llnunu_MET', 'llnunu_l2_pt')
    Graviton2PBPlotters[-1].setAlias('llnunu_l2_phi_to_plot', 'llnunu_l2_phi')
    Graviton2PBPlotters[-1].setAlias('llnunu_mt_to_plot', 'llnunu_mt')
    Graviton2PBPlotters[-1].setAlias('llnunu_mT', 'llnunu_mt')
    for syst in ['JetEn','JetRes','MuonEn','ElectronEn','TauEn','PhotonEn','Uncluster'] :
        Graviton2PBPlotters[-1].setAlias('llnunu_mT_'+syst+'Up', 'llnunu_mt_'+syst+'Up')
        Graviton2PBPlotters[-1].setAlias('llnunu_mT_'+syst+'Dn', 'llnunu_mt_'+syst+'Dn')
        Graviton2PBPlotters[-1].setAlias('llnunu_MET_'+syst+'Up', 'llnunu_l2_t1Pt_'+syst+'Up')
        Graviton2PBPlotters[-1].setAlias('llnunu_MET_'+syst+'Dn', 'llnunu_l2_t1Pt_'+syst+'Dn')

    Graviton2PBPlotters[-1].setAlias('llnunu_mT_RecoilUp', 'llnunu_mt')
    Graviton2PBPlotters[-1].setAlias('llnunu_mT_RecoilDn', 'llnunu_mt')

##########################
# Data Observed
##########################

dataSamples = [

'SingleMuon_Run2016B_03Feb2017_ver2',
'SingleMuon_Run2016C_03Feb2017',
'SingleMuon_Run2016D_03Feb2017',
'SingleMuon_Run2016E_03Feb2017',
'SingleMuon_Run2016F_03Feb2017',
'SingleMuon_Run2016G_03Feb2017',
'SingleMuon_Run2016H_03Feb2017_ver2'

]

dataPlotters=[]
for sample in dataSamples:
    dataPlotters.append(TreePlotter(sample, indir+'/'+sample+'.root','tree'))
    dataPlotters[-1].setAlias('passMuHLT', '((llnunu_l1_l1_trigerob_HLTbit>>3&1)||(llnunu_l1_l1_trigerob_HLTbit>>4&1)||(llnunu_l1_l2_trigerob_HLTbit>>3&1)||(llnunu_l1_l2_trigerob_HLTbit>>4&1))');
    dataPlotters[-1].setAlias('passElHLT', '((llnunu_l1_l1_trigerob_HLTbit>>1&1)||(llnunu_l1_l2_trigerob_HLTbit>>1&1))');
    dataPlotters[-1].addCorrectionFactor('(passMuHLT||passElHLT)','HLT')


Data = MergedPlotter(dataPlotters)

# some plotting definition
Data.setAlias('llnunu_l1_mass_to_plot', 'llnunu_l1_mass')
Data.setAlias('llnunu_l2_pt_to_plot', 'llnunu_l2_pt')
Data.setAlias('llnunu_MET', 'llnunu_l2_pt')
Data.setAlias('llnunu_l2_phi_to_plot', 'llnunu_l2_phi')
Data.setAlias('llnunu_mt_to_plot', 'llnunu_mt')
Data.setAlias('llnunu_mT', 'llnunu_mt')

for syst in ['JetEn','JetRes','MuonEn','ElectronEn','TauEn','PhotonEn','Uncluster','Recoil'] :
    Data.setAlias('llnunu_mT_'+syst+'Up', 'llnunu_mt')
    Data.setAlias('llnunu_mT_'+syst+'Dn', 'llnunu_mt')
    Data.setAlias('llnunu_MET_'+syst+'Up', 'llnunu_l2_pt')
    Data.setAlias('llnunu_MET_'+syst+'Dn', 'llnunu_l2_pt')


############################
# Stack Plotter to draw all
############################

Stack = StackPlotter(outTag=tag, outDir=outdir)
Stack.setPaveText(paveText)
Stack.addPlotter(Data, "data_obs", "Data", "data")
if muoneg: 
    Stack.addPlotter(NONRES, "NonReso","Non-reson. (e#mu data)", "background")
else:
    Stack.addPlotter(WW, "NonReso","WW/WZ/WJets non-reson.", "background")
    Stack.addPlotter(TT, "TT","TT", "background")
Stack.addPlotter(VV, "VVZReso","ZZ WZ reson.", "background")
if dyGJets: 
    Stack.addPlotter(ZJets, "ZJets","ZJets(#gamma+Jets data)", "background")
else: 
    Stack.addPlotter(ZJets, "ZJets","ZJets(MC)", "background")

for i in range(len(sigSamples)):
  sigPlotters[i].setLineProperties(2,ROOT.kRed+i,2)
  Stack.addPlotter(sigPlotters[i],sigSamples[i],sigSampleNames[sigSamples[i]],'signal')  
 
for i in range(len(ggHSamples)):
  ggHPlotters[i].setLineProperties(2,ROOT.kBlue+i,2)
  Stack.addPlotter(ggHPlotters[i],ggHSamples[i],ggHSamples[i],'signal')

for i in range(len(Graviton2PBSamples)):
  Graviton2PBPlotters[i].setLineProperties(2,ROOT.kGreen+i,2)
  Stack.addPlotter(Graviton2PBPlotters[i],Graviton2PBSamples[i],Graviton2PBSamples[i],'signal')

Stack.setLog(LogY)
Stack.doRatio(doRatio)

tag+='_'

nBins=60
mtMin=0
mtMax=3000

metnBins=30
metMin=0
metMax=1500

Stack.drawStack('llnunu_mT', cuts, str(lumi*1000), nBins, mtMin, mtMax, titlex = "M_{T}", units = "GeV",output='mT',outDir=outdir,separateSignal=sepSig, blinding=Blind,blindingCut=300)

Stack.drawStack('llnunu_MET', cuts, str(lumi*1000), metnBins, metMin, metMax, titlex = "E_{T}^{miss}", units = "GeV",output='MET',outDir=outdir,separateSignal=sepSig, blinding=Blind,blindingCut=300)

Stack.drawStack('llnunu_l2_pt_to_plot*cos(llnunu_l2_phi_to_plot-llnunu_l1_phi)', cuts, str(lumi*1000), 50, -500, 500.0, titlex = "E_{T#parallel}^{miss}", units = "GeV",output='met_para',outDir=outdir,separateSignal=sepSig)

if(not doSys) :
  Stack.drawStack('llnunu_l2_pt_to_plot', cuts, str(lumi*1000), metnBins, metMin, metMax, titlex = "E_{T}^{miss}", units = "GeV",output='met',outDir=outdir,separateSignal=sepSig,blinding=Blind,blindingCut=200)
  Stack.drawStack('llnunu_l1_pt', cuts, str(lumi*1000), metnBins, metMin, metMax, titlex = "PT_{Z}", units = "GeV",output='zpt',outDir=outdir,separateSignal=sepSig,blinding=Blind,blindingCut=200)

else :

   if dyGJets :
      ## zjetsFidXsec
      postfix = 'El'
      fidXsec = zjetsFidXsecEl
      fidXsecUp = zjetsFidXsecEl_up
      fidXsecDn = zjetsFidXsecEl_dn
      if ( channel=='mu' ) :
         postfix = "Mu"
         fidXsec = zjetsFidXsecMu
         fidXsecUp = zjetsFidXsecMu_up
         fidXsecDn = zjetsFidXsecMu_dn

   ## trg
   for (plotter,typeP,label,name) in zip(Stack.plotters,Stack.types,Stack.labels,Stack.names):
          if ( typeP != "data" and  name != "NonReso" and name!="ZJets"):
             plotter.changeCorrectionFactor("trgsf_up*idisotrksf","lepsf")

   Stack.drawStack('llnunu_mT', cuts, str(lumi*1000), nBins, mtMin, mtMax, titlex = "M_{T}", units = "GeV",output='mT_trgUp',outDir=outdir,separateSignal=sepSig,blinding=Blind,blindingCut=300)
   Stack.drawStack('llnunu_MET', cuts, str(lumi*1000), metnBins, metMin, metMax, titlex = "E_{T}^{miss}", units = "GeV",output='MET_trgUp',outDir=outdir,separateSignal=sepSig,blinding=Blind,blindingCut=300)

   for (plotter,typeP,label,name) in zip(Stack.plotters,Stack.types,Stack.labels,Stack.names):
       if ( typeP != "data" and  name != "NonReso" and name!="ZJets"):
          plotter.changeCorrectionFactor("trgsf_dn*idisotrksf","lepsf")

   Stack.drawStack('llnunu_mT', cuts, str(lumi*1000), nBins, mtMin, mtMax, titlex = "M_{T}", units = "GeV",output='mT_trgDn',outDir=outdir,separateSignal=sepSig,blinding=Blind,blindingCut=300)
   Stack.drawStack('llnunu_MET', cuts, str(lumi*1000), metnBins, metMin, metMax, titlex = "E_{T}^{miss}", units = "GeV",output='MET_trgDn',outDir=outdir,separateSignal=sepSig,blinding=Blind,blindingCut=300)

   ## id
   for (plotter,typeP,label,name) in zip(Stack.plotters,Stack.types,Stack.labels,Stack.names):
       if ( typeP != "data" and  name != "NonReso" and name!="ZJets"):
          plotter.changeCorrectionFactor("trgsf*idisotrksf_up","lepsf")

   Stack.drawStack('llnunu_mT', cuts, str(lumi*1000), nBins, mtMin, mtMax, titlex = "M_{T}", units = "GeV",output='mT_idUp',outDir=outdir,separateSignal=sepSig,blinding=Blind,blindingCut=300)
   Stack.drawStack('llnunu_MET', cuts, str(lumi*1000), metnBins, metMin, metMax, titlex = "E_{T}^{miss}", units = "GeV",output='MET_idUp',outDir=outdir,separateSignal=sepSig,blinding=Blind,blindingCut=300)

   for (plotter,typeP,label,name) in zip(Stack.plotters,Stack.types,Stack.labels,Stack.names):
       if ( typeP != "data" and name != "NonReso" and name!="ZJets"):
          plotter.changeCorrectionFactor("trgsf*idisotrksf_dn","lepsf")

   Stack.drawStack('llnunu_mT', cuts, str(lumi*1000), nBins, mtMin, mtMax, titlex = "M_{T}", units = "GeV",output='mT_idDn',outDir=outdir,separateSignal=sepSig,blinding=Blind,blindingCut=300)
   Stack.drawStack('llnunu_MET', cuts, str(lumi*1000), metnBins, metMin, metMax, titlex = "E_{T}^{miss}", units = "GeV",output='MET_idDn',outDir=outdir,separateSignal=sepSig,blinding=Blind,blindingCut=300)

   ## EWK and QCD
   for (plotter,typeP,label,name) in zip(Stack.plotters,Stack.types,Stack.labels,Stack.names):
       if ( typeP != "data" and name != "NonReso" and name!="ZJets"):
          plotter.changeCorrectionFactor("trgsf*idisotrksf","lepsf")
   for (plotter,typeP,label,name) in zip(Stack.plotters,Stack.types,Stack.labels,Stack.names):
       if ( typeP != "data" and  name == "VVZReso"):
          plotter.changeCorrectionFactor("(ZZEwkCorrWeight_up*ZZQcdCorrWeight)*xsec","nnlo")

   Stack.drawStack('llnunu_mT', cuts, str(lumi*1000), nBins, mtMin, mtMax, titlex = "M_{T}", units = "GeV",output='mT_ewkUp',outDir=outdir,separateSignal=sepSig,blinding=Blind,blindingCut=300)
   Stack.drawStack('llnunu_MET', cuts, str(lumi*1000), metnBins, metMin, metMax, titlex = "E_{T}^{miss}", units = "GeV",output='MET_ewkUp',outDir=outdir,separateSignal=sepSig,blinding=Blind,blindingCut=300)

   for (plotter,typeP,label,name) in zip(Stack.plotters,Stack.types,Stack.labels,Stack.names):
       if ( typeP != "data" and  name == "VVZReso"):
          plotter.changeCorrectionFactor("(ZZEwkCorrWeight_dn*ZZQcdCorrWeight)*xsec","nnlo")

   Stack.drawStack('llnunu_mT', cuts, str(lumi*1000), nBins, mtMin, mtMax, titlex = "M_{T}", units = "GeV",output='mT_ewkDn',outDir=outdir,separateSignal=sepSig,blinding=Blind,blindingCut=300)
   Stack.drawStack('llnunu_MET', cuts, str(lumi*1000), metnBins, metMin, metMax, titlex = "E_{T}^{miss}", units = "GeV",output='MET_ewkDn',outDir=outdir,separateSignal=sepSig,blinding=Blind,blindingCut=300)

   for (plotter,typeP,label,name) in zip(Stack.plotters,Stack.types,Stack.labels,Stack.names):
       if ( typeP != "data" and  name == "VVZReso"):
          plotter.changeCorrectionFactor("(ZZEwkCorrWeight*ZZQcdCorrWeight_up)*xsec","nnlo")

   Stack.drawStack('llnunu_mT', cuts, str(lumi*1000), nBins, mtMin, mtMax, titlex = "M_{T}", units = "GeV",output='mT_qcdUp',outDir=outdir,separateSignal=sepSig,blinding=Blind,blindingCut=300)
   Stack.drawStack('llnunu_MET', cuts, str(lumi*1000), metnBins, metMin, metMax, titlex = "E_{T}^{miss}", units = "GeV",output='MET_qcdUp',outDir=outdir,separateSignal=sepSig,blinding=Blind,blindingCut=300)

   for (plotter,typeP,label,name) in zip(Stack.plotters,Stack.types,Stack.labels,Stack.names):
       if ( typeP != "data" and  name == "VVZReso"):

          plotter.changeCorrectionFactor("(ZZEwkCorrWeight*ZZQcdCorrWeight_dn)*xsec","nnlo")
   Stack.drawStack('llnunu_mT', cuts, str(lumi*1000), nBins, mtMin, mtMax, titlex = "M_{T}", units = "GeV",output='mT_qcdDn',outDir=outdir,separateSignal=sepSig,blinding=Blind,blindingCut=300)
   Stack.drawStack('llnunu_MET', cuts, str(lumi*1000), metnBins, metMin, metMax, titlex = "E_{T}^{miss}", units = "GeV",output='MET_qcdDn',outDir=outdir,separateSignal=sepSig,blinding=Blind,blindingCut=300)

   ## GJetsZPtWeight
   if ( channel=='el' and dyGJets ) :
       for (plotter,typeP,label,name) in zip(Stack.plotters,Stack.types,Stack.labels,Stack.names):
           if ( typeP != "data" and name == "ZJets" ):
              plotter.changeCorrectionFactor(str(zjetsFidXsecEl_up),"zjetsFidXsecEl")
              plotter.changeCorrectionFactor("GJetsZPtWeightEl_up","GJetsZPtWeight")

       Stack.drawStack('llnunu_mT', cuts, str(lumi*1000), nBins, mtMin, mtMax, titlex = "M_{T}", units = "GeV",output='mT_fidxsecUp',outDir=outdir,separateSignal=sepSig,blinding=Blind,blindingCut=300)
       Stack.drawStack('llnunu_MET', cuts, str(lumi*1000), metnBins, metMin, metMax, titlex = "E_{T}^{miss}", units = "GeV",output='MET_fidxsecUp',outDir=outdir,separateSignal=sepSig,blinding=Blind,blindingCut=300)

       for (plotter,typeP,label,name) in zip(Stack.plotters,Stack.types,Stack.labels,Stack.names):
           if ( typeP != "data" and name == "ZJets"  ):
              plotter.changeCorrectionFactor(str(zjetsFidXsecEl_dn),"zjetsFidXsecEl")
              plotter.changeCorrectionFactor("GJetsZPtWeightEl_dn","GJetsZPtWeight")

       Stack.drawStack('llnunu_mT', cuts, str(lumi*1000), nBins, mtMin, mtMax, titlex = "M_{T}", units = "GeV",output='mT_fidxsecDn',outDir=outdir,separateSignal=sepSig,blinding=Blind,blindingCut=300)
       Stack.drawStack('llnunu_MET', cuts, str(lumi*1000), metnBins, metMin, metMax, titlex = "E_{T}^{miss}", units = "GeV",output='MET_fidxsecDn',outDir=outdir,separateSignal=sepSig,blinding=Blind,blindingCut=300)

   if ( channel=='mu' and dyGJets ) :
       for (plotter,typeP,label,name) in zip(Stack.plotters,Stack.types,Stack.labels,Stack.names):
           if ( typeP != "data" and name == "ZJets" ):
              plotter.changeCorrectionFactor(str(zjetsFidXsecMu_up),"zjetsFidXsecMu")
              plotter.changeCorrectionFactor("GJetsZPtWeightMu_up","GJetsZPtWeight")
       Stack.drawStack('llnunu_mT', cuts, str(lumi*1000), nBins, mtMin, mtMax, titlex = "M_{T}", units = "GeV",output='mT_fidxsecUp',outDir=outdir,separateSignal=sepSig,blinding=Blind,blindingCut=300)
       Stack.drawStack('llnunu_MET', cuts, str(lumi*1000), metnBins, metMin, metMax, titlex = "E_{T}^{miss}", units = "GeV",output='MET_fidxsecUp',outDir=outdir,separateSignal=sepSig,blinding=Blind,blindingCut=300)

       for (plotter,typeP,label,name) in zip(Stack.plotters,Stack.types,Stack.labels,Stack.names):
           if ( typeP != "data" and name == "ZJets"  ):
              plotter.changeCorrectionFactor(str(zjetsFidXsecMu_dn),"zjetsFidXsecMu")
              plotter.changeCorrectionFactor("GJetsZPtWeightMu_dn","GJetsZPtWeight")
       Stack.drawStack('llnunu_mT', cuts, str(lumi*1000), nBins, mtMin, mtMax, titlex = "M_{T}", units = "GeV",output='mT_fidxsecDn',outDir=outdir,separateSignal=sepSig,blinding=Blind,blindingCut=300)
       Stack.drawStack('llnunu_MET', cuts, str(lumi*1000), metnBins, metMin, metMax, titlex = "E_{T}^{miss}", units = "GeV",output='MET_fidxsecDn',outDir=outdir,separateSignal=sepSig,blinding=Blind,blindingCut=300)

       ######## MT Unc
       for (plotter,typeP,label,name) in zip(Stack.plotters,Stack.types,Stack.labels,Stack.names):
           if ( typeP != "data" and name == 'ZJets'):
              if ( channel=='mu' ):
                 plotter.changeCorrectionFactor("GJetsZPtWeightMu","GJetsZPtWeight")
                 plotter.changeCorrectionFactor(str(zjetsFidXsecMu),"zjetsFidXsecMu")
              if ( channel=='el' ):
                 plotter.changeCorrectionFactor("GJetsZPtWeightEl","GJetsZPtWeight")
                 plotter.changeCorrectionFactor(str(zjetsFidXsecEl),"zjetsFidXsecEl")

   for systs in ['Recoil','JetEn','JetRes','MuonEn','ElectronEn','TauEn','PhotonEn','Uncluster'] :

      print systs
      Stack.drawStack('llnunu_mT_'+systs+'Up', cutsALT(cuts,systs,'Up'), str(lumi*1000), nBins, mtMin, mtMax, titlex = "M_{T}", units = "GeV",output='mT_'+systs+'Up',outDir=outdir,separateSignal=sepSig,blinding=Blind,blindingCut=300)
      Stack.drawStack('llnunu_mT_'+systs+'Dn', cutsALT(cuts,systs,'Dn'), str(lumi*1000), nBins, mtMin, mtMax, titlex = "M_{T}", units = "GeV",output='mT_'+systs+'Dn',outDir=outdir,separateSignal=sepSig,blinding=Blind,blindingCut=300)

      Stack.drawStack('llnunu_MET_'+systs+'Up', cutsALT(cuts,systs,'Up'), str(lumi*1000), metnBins, metMin, metMax, titlex = "E_{T}^{miss}", units = "GeV",output='MET_'+systs+'Up',outDir=outdir,separateSignal=sepSig,blinding=Blind,blindingCut=300)
      Stack.drawStack('llnunu_MET_'+systs+'Dn', cutsALT(cuts,systs,'Dn'), str(lumi*1000), metnBins, metMin, metMax, titlex = "E_{T}^{miss}", units = "GeV",output='MET_'+systs+'Dn',outDir=outdir,separateSignal=sepSig,blinding=Blind,blindingCut=300)

Stack.closePSFile()
Stack.closeROOTFile()



