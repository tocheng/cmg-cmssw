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
doGMCPhPtScale=True
dyGJets=options.dyGJets
muoneg=options.muoneg
doSys=options.doSys

if test: DrawLeptons = False

lepsf="trgsf*isosf*idsf*trksf"

g_scale='(1)'
mc_scale='(1)'
zjets_scale='(1)'

# compensate the normalization due to phyMet in photon+jets
if channel=='mu':
    zjets_scale='(1.03627471042)'
elif channel=='el':
    zjets_scale='(1.04227778889)'
else:
    zjets_scale='(1.03627471042)'

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

if doGMCPhPtScale:
    tag+="GMCPhPtWt_"
    g_scale+="*((0.295668+0.0127154*llnunu_l1_pt-7.71163e-05*pow(llnunu_l1_pt,2)+2.2603e-07*pow(llnunu_l1_pt,3)-3.50496e-10*pow(llnunu_l1_pt,4)+2.7572e-13*pow(llnunu_l1_pt,5)-8.66455e-17*pow(llnunu_l1_pt,6))*(llnunu_l1_pt<=800)+(0.912086)*(llnunu_l1_pt>800))"  # for reminiaod allcorV2 mc hlt


outdir='plots'
indirZjets='/eos/user/t/tocheng/X2l+MET+jets/Run2016/Analysis/ZJetsCleanPreskim/'
indirGjets='/eos/user/t/tocheng/X2l+MET+jets/Run2016/Analysis/GJetsCleanPreskim/'

lumi=35.9
sepSig=True
doRatio=True
Blind=options.Blind
FakeData=False
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

if not Blind: tag = tag+'unblind_'

tag = tag+channel+'_'
if LogY: tag = tag+'log_'
if SignalAll1pb:
    tag += '1pb'
else:
    tag += 'scale'+str(k)

paveText="#sqrt{s} = 13 TeV 2016 L = "+"{:.4}".format(float(lumi))+" fb^{-1}"

cuts_lepaccept="("+elChannel+"||"+muChannel+"||"+photonFakeID+")"
if channel=="el" : cuts_lepaccept="("+elChannel+"||"+photonFakeID+")"
elif channel=="mu" : cuts_lepaccept="("+muChannel+"||"+photonFakeID+")"
cuts_zmass="(llnunu_l1_mass_to_plot>70&&llnunu_l1_mass_to_plot<110)"
cuts_zpt100="(llnunu_l1_pt>100)"
cuts_met50="(llnunu_MET>50)"
cuts_loose_z="("+cuts_lepaccept+"&&"+cuts_zmass+")"
cuts_loose_zll="("+cuts_lepaccept+"&&"+cuts_zmass+"&&"+cuts_zpt100+")"
cuts_loose_zll_met50="("+cuts_lepaccept+"&&"+cuts_zmass+"&&"+cuts_zpt100+"&&"+cuts_met50+")"

cuts=''

if cutChain=='loosecut': cuts=cuts_loose
elif cutChain=='tight': cuts=cuts_loose_z
elif cutChain=='tightzpt100': cuts=cuts_loose_zll
elif cutChain=='tightzpt100met50': cuts=cuts_loose_zll_met50
elif cutChain=='SR': cuts=cuts_loose_zll_met50
elif cutChain=='SRdPhiGT0p5': cuts=cuts_loose_zll_met50+"&&fabs(TVector2::Phi_mpi_pi(llnunu_l2_phi_to_plot-llnunu_l1_phi))>0.5"

if cutChain=='SR_massSB':
   cuts=cuts_loose_zll
   cuts += '&&(boostJetAK8_softDropMass<70 || boostJetAK8_softDropMass>140)'
if cutChain=='SR_massSB_btag' :
   cuts=cuts_loose_zll
   cuts+='&&(boostJetAK8_softDropMass<70 || boostJetAK8_softDropMass>140)&&boostJetAK8_btagBOOSTED>0.8'
if cutChain=='SR_massSB_antibtag' :
   cuts=cuts_loose_zll
   cuts+='&&(boostJetAK8_softDropMass<70 || boostJetAK8_softDropMass>140)&&boostJetAK8_btagBOOSTED<=0.8'

cuts += '&&(nboostJetAK8Puppi>=1)'
cuts += '&&boostJetAK8_tau21<0.6'
cuts += '&&boostJetAK8_softDropMass>40'

#metfilter='(Flag_EcalDeadCellTriggerPrimitiveFilter&&Flag_HBHENoiseIsoFilter&&Flag_goodVertices&&Flag_HBHENoiseFilter&&Flag_globalTightHalo2016Filter&&Flag_eeBadScFilter)'
#if UseMETFilter:
#    cuts = '('+cuts+')&&'+metfilter
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
'ZZTo2L2Q',
'ggZZTo2e2nu',
'ggZZTo2mu2nu',
'ZZTo2L2Nu',
'WZTo3LNu',
'TTZToLLNuNu'
]

vvPlotters=[]
for sample in vvSamples:
    vvPlotters.append(TreePlotter(sample, indirZjets+'/'+sample+'.root','tree'))
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

# if use mc
else:

    mcnonresoSamples = ['WWTo2L2Nu','TTTo2L2Nu_forTTH']
    mcnonresoPlotters=[]
    for sample in mcnonresoSamples:
        mcnonresoPlotters.append(TreePlotter(sample, indirZjets+'/'+sample+'.root','tree'))
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
        MCNONRESO.setAlias('llnunu_mT_'+syst+'Up', 'llnunu_mt')
        MCNONRESO.setAlias('llnunu_mT_'+syst+'Dn', 'llnunu_mt')
        MCNONRESO.setAlias('llnunu_MET_'+syst+'Up', 'llnunu_l2_t1Pt_'+syst+'Up')
        MCNONRESO.setAlias('llnunu_MET_'+syst+'Dn', 'llnunu_l2_t1Pt_'+syst+'Dn')

    for syst in ['Recoil'] :
        MCNONRESO.setAlias('llnunu_MET_'+syst+'Up', 'llnunu_l2_pt')
        MCNONRESO.setAlias('llnunu_MET_'+syst+'Dn', 'llnunu_l2_pt')


################################
# ZJets backgrounds
######################

# if use GJets to describe ZJets
if dyGJets : 

    # parameters for GJets
    el_gjet_scale=1.00
    mu_gjet_scale=1.00 

    gdataYield = 922367.4625431895256
    zjetsFidXsecAll = 2.6483305973545592238
    zjetsFidXsecEl =  0.81292596359194457811
    zjetsFidXsecMu =  1.8137004444472601961
    zjetsFidXsecAll_up = 2.7434125669387401381
    zjetsFidXsecAll_dn = 2.5550767800007219144
    zjetsFidXsecEl_up = 0.84390843773360679769
    zjetsFidXsecEl_dn = 0.78247063628511537292
    zjetsFidXsecMu_up = 1.8770140756989130981
    zjetsFidXsecMu_dn = 1.7516748796936181254
    zjetsFidXsecLowLptAll = 3.3717905593324744018
    zjetsFidXsecLowLptEl = 1.4447351473453435844
    zjetsFidXsecLowLptMu = 1.927055411987123934

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
    'ZNuNuGJetsGt130', 
    'ZNuNuGJetsGt40Lt130',
    'WJetsToLNu_HT100to200',
    'WJetsToLNu_HT1200to2500',
    'WJetsToLNu_HT200to400',
    'WJetsToLNu_HT2500toInf',
    'WJetsToLNu_HT400to600',
    'WJetsToLNu_HT600to800',
    'WJetsToLNu_HT800to1200',
    'ZJetsToNuNu_HT100to200',
    'ZJetsToNuNu_HT1200to2500',
    'ZJetsToNuNu_HT200to400',
    'ZJetsToNuNu_HT2500toInf',
    'ZJetsToNuNu_HT400to600',
    'ZJetsToNuNu_HT600to800',
    'ZJetsToNuNu_HT800t1200',
    ]

    # all the factors below together normalized each process to the fraction of the process in the gjets data
    #   fidxsec_i / fidxsec_total
    # together with the gdata, we have:
    #  [ fidxsec_total-Sum(fidxsec_i) ]/fidxsec_total * fidxsec_zjets * lumi = zjets_yields
    # an additional scale factor GJetsNorm to absorbe the small difference.

    phymetPlotters=[]
    for sample in phymetSamples:
        phymetPlotters.append(TreePlotter(sample, indirGjets+'/'+sample+'.root','tree'))
        phymetPlotters[-1].addCorrectionFactor('-1/SumWeights','norm') # negative weight for subtraction
        if sample=='G_ZNuNuGJetsGt130': phymetPlotters[-1].addCorrectionFactor('0.1832*1.43','xsec')  # NNLO/LO k-factor from JHEP02 (2016) 057, Table 2
        elif sample=='G_ZNuNuGJetsGt40Lt130': phymetPlotters[-1].addCorrectionFactor('xsec*1.43','xsec')
        elif sample=='G_WGJetsPt130':  phymetPlotters[-1].addCorrectionFactor('0.834*2.53','xsec')  # NNLO/LO k-factor from JHEP04 (2015) 018, Table 1
        elif sample=='G_WGToLNuG':  phymetPlotters[-1].addCorrectionFactor('xsec*2.53','xsec')  # NNLO/LO k-factor from JHEP04 (2015) 018, Table 1
        else: phymetPlotters[-1].addCorrectionFactor('xsec','xsec')
        phymetPlotters[-1].addCorrectionFactor('genWeight','genWeight')
        phymetPlotters[-1].addCorrectionFactor('GJetsRhoWeight','GJetsRhoWeight')
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
    gdataSamples = [
      'SinglePhoton_Run2016B_03Feb2017_ver2',
      'SinglePhoton_Run2016C_03Feb2017',
      'SinglePhoton_Run2016G_03Feb2017',
      'SinglePhoton_Run2016D_03Feb2017',
      'SinglePhoton_Run2016E_03Feb2017',
      'SinglePhoton_Run2016F_03Feb2017',
      'SinglePhoton_Run2016H_03Feb2017_ver2',
      'SinglePhoton_Run2016H_03Feb2017_ver3'
    ]

    gdataPlotters=[]
    for sample in gdataSamples:
        gdataPlotters.append(TreePlotter(sample, indirGjets+'/'+sample+'.root','tree'))
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
    'DYJetsToLL_Pt_100To250',
    'DYJetsToLL_Pt_250To400',
    'DYJetsToLL_Pt_400To650',  
    'DYJetsToLL_Pt_650ToInf',
    ]

    mczjetsPlotters=[]
    for sample in mczjetsSamples:
        mczjetsPlotters.append(TreePlotter(sample, indirZjets+'/'+sample+'.root','tree'))
        mczjetsPlotters[-1].addCorrectionFactor('1./SumWeights','norm')
        mczjetsPlotters[-1].addCorrectionFactor(ZPtWeight,'ZPtWeight')
        mczjetsPlotters[-1].addCorrectionFactor('xsec','xsec') 
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
'ZpAnomalonZHbb',
'ZpAnomalonZZbb',
'ZpAnomalonZZjj',
]

sigPlotters=[]
sigSampleNames = {
'ZpAnomalonZHbb':str(k)+' x Zprime cascade Hbb',
'ZpAnomalonZZbb':str(k)+' x Zprime cascade Zbb',
'ZpAnomalonZZjj':str(k)+' x Zprime cascade Zjj',
}

sigXsec=0.001

if SignalAll1pb:
    for sig in sigSamples:
        sigSampleNames[sig] = string.replace(sigSampleNames[sig], str(k)+' x', '1 fb')

for sample in sigSamples:
    sigPlotters.append(TreePlotter(sample, indirZjets+'/'+sample+'.root','tree'))
    sigPlotters[-1].addCorrectionFactor('1./SumWeights','norm')
    sigPlotters[-1].addCorrectionFactor(str(sigXsec),'xsec')
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

### Graviton2PBqqbar

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
'SingleMuon_Run2016H_03Feb2017_ver2',
'SingleElectron_Run2016B_03Feb2017_ver2',
'SingleElectron_Run2016C_03Feb2017',
'SingleElectron_Run2016D_03Feb2017',
'SingleElectron_Run2016E_03Feb2017',
'SingleElectron_Run2016F_03Feb2017',
'SingleElectron_Run2016G_03Feb2017',
'SingleElectron_Run2016H_03Feb2017_ver2',
'SingleElectron_Run2016H_03Feb2017_ver3'
]

dataPlotters=[]
for sample in dataSamples:
    dataPlotters.append(TreePlotter(sample, indirZjets+'/'+sample+'.root','tree'))
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
    Stack.addPlotter(MCNONRESO, "NonReso","TT/WW ", "background")

Stack.addPlotter(VV, "VVZReso","ZZ WZ reson.", "background")
if dyGJets: 
    Stack.addPlotter(ZJets, "ZJets","ZJets(#gamma+Jets data)", "background")
else: 
    Stack.addPlotter(ZJets, "ZJets","ZJets(MC)", "background")

for i in range(len(sigSamples)):
  sigPlotters[i].setLineProperties(2,ROOT.kRed+i,2)
  Stack.addPlotter(sigPlotters[i],sigSamples[i],sigSampleNames[sigSamples[i]],'signal')  
 
Stack.setLog(LogY)
Stack.doRatio(doRatio)

tag+='_'

Stack.drawStack('llnunu_MET', cuts, str(lumi*1000), 40, 0, 1000, titlex = "E_{T}^{miss}", units = "GeV",output='MET',outDir=outdir,separateSignal=sepSig, blinding=Blind,blindingCut=300)
Stack.drawStack('llnunu_l1_pt', cuts, str(lumi*1000), 40, 0, 2000, titlex = "PT_{Z/#gamma}", units = "GeV",output='zpt',outDir=outdir,separateSignal=sepSig,blinding=Blind,blindingCut=200)
Stack.drawStack('llnunu_l1_mass', cuts, str(lumi*1000), 26, 50, 180, titlex = "m_{Z}", units = "GeV",output='zmass',outDir=outdir,separateSignal=sepSig,blinding=Blind,blindingCut=200)

Stack.drawStack('boostJetAK8_pt', cuts, str(lumi*1000), 40, 0, 2000, titlex = "P_{T}^{AK8Jet}", units = "GeV",output='P_{T}^{AK8Jet}',outDir=outdir,separateSignal=sepSig, blinding=Blind,blindingCut=300)
Stack.drawStack('boostJetAK8_btagBOOSTED', cuts, str(lumi*1000), 40, -1, 1, titlex = "BoostBtag", units = "",output='BoostBtag',outDir=outdir,separateSignal=sepSig, blinding=Blind,blindingCut=300)
Stack.drawStack('boostJetAK8_softDropMass', cuts, str(lumi*1000), 32, 40, 200, titlex = "SoftDrop Mass", units = "GeV",output='SoftDrop Mass',outDir=outdir,separateSignal=sepSig, blinding=Blind,blindingCut=300)
Stack.drawStack('boostJetAK8_tau21', cuts, str(lumi*1000), 40, 0, 1, titlex = "#tau_{21}", units = "", output='#tau_{21}',outDir=outdir,separateSignal=sepSig, blinding=Blind,blindingCut=300)

Stack.drawStack('mR', cuts, str(lumi*1000), 40, 0, 4000, titlex = "M_{R}", units = "GeV",output='mR',outDir=outdir,separateSignal=sepSig, blinding=Blind,blindingCut=300)
Stack.drawStack('mRT', cuts, str(lumi*1000), 40, 0, 4000, titlex = "M_{RT}", units = "GeV",output='mRT',outDir=outdir,separateSignal=sepSig, blinding=Blind,blindingCut=300)
Stack.drawStack('R', cuts, str(lumi*1000), 40, 0, 1, titlex = "R", units = "",output='R',outDir=outdir,separateSignal=sepSig, blinding=Blind,blindingCut=300)

Stack.drawStack('mTfull', cuts, str(lumi*1000), 40, 0, 4000, titlex = "M_{T}", units = "GeV",output='mT',outDir=outdir,separateSignal=sepSig, blinding=Blind,blindingCut=300)

Stack.closePSFile()
Stack.closeROOTFile()

