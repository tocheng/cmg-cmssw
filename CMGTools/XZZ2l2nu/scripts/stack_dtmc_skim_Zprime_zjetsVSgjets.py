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
doGMCPhPtScale=True
dyGJets=options.dyGJets
muoneg=options.muoneg
doSys=options.doSys

if test: DrawLeptons = False

lepsf="trgsf*isosf*idsf*trksf"

# for photon+jets MC
g_scale='(1)'
##################
zjets_scale='(1)'

if channel=='mu':
    zjets_scale='(1.03627471042)'
elif channel=='el':
    zjets_scale='(1.04227778889)'
else:
    zjets_scale='(1.03627471042)'

if doGMCPhPtScale:
    tag+="GMCPhPtWt_"
    g_scale+="*((0.295668+0.0127154*llnunu_l1_pt-7.71163e-05*pow(llnunu_l1_pt,2)+2.2603e-07*pow(llnunu_l1_pt,3)-3.50496e-10*pow(llnunu_l1_pt,4)+2.7572e-13*pow(llnunu_l1_pt,5)-8.66455e-17*pow(llnunu_l1_pt,6))*(llnunu_l1_pt<=800)+(0.912086)*(llnunu_l1_pt>800))"  # for reminiaod allcorV2 mc hlt


outdir='plots_gjetsVSzjets'
indirMCZjets='/eos/user/t/tocheng/X2l+MET+jets/Run2016/Analysis/ZJetsCleanPreskim/'
indirGjets='/eos/user/t/tocheng/X2l+MET+jets/Run2016/Analysis/GJetsCleanPreskim/'


lumi=35.9
sepSig=True
doRatio=True
Blind=options.Blind
FakeData=False
UseMETFilter=False
puWeight='puWeightsummer16'
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

paveText="#sqrt{s} = 13 TeV 2016 L = "+"{:.4}".format(float(lumi))+" fb^{-1}"

cuts_loose='(nllnunu)'
cuts_lepaccept="("+elChannel+"||"+muChannel+"||"+photonFakeID+")"
if channel=="el" : cuts_lepaccept="("+elChannel+"||"+photonFakeID+")"
elif channel=="mu" : cuts_lepaccept="("+muChannel+"||"+photonFakeID+")"
cuts_zmass="(llnunu_l1_mass_to_plot>70&&llnunu_l1_mass_to_plot<110)"
cuts_zpt100="(llnunu_l1_pt>100)"
cuts_met50="(llnunu_MET>50)"
cuts_loose_z="("+cuts_lepaccept+"&&"+cuts_zmass+")"
cuts_loose_zll="("+cuts_lepaccept+"&&"+cuts_zmass+"&&"+cuts_zpt100+")"
cuts_loose_zll_met50="("+cuts_lepaccept+"&&"+cuts_zmass+"&&"+cuts_zpt100+"&&"+cuts_met50+")"

if cutChain=='loosecut': cuts=cuts_loose
elif cutChain=='tight': cuts=cuts_loose_z
elif cutChain=='tightzpt100': cuts=cuts_loose_zll
elif cutChain=='tightzpt100met50': cuts=cuts_loose_zll_met50
elif cutChain=='SR': cuts=cuts_loose_zll_met50
elif cutChain=='SRdPhiGT0p5': cuts=cuts_loose_zll_met50+"&&fabs(TVector2::Phi_mpi_pi(llnunu_l2_phi_to_plot-llnunu_l1_phi))>0.5"
else : cuts=cuts_loose

if cutChain=='SR_massSB':
   cuts=cuts_loose_zll_met50
   cuts += '&&(boostJetAK8_softDropMass<70 || boostJetAK8_softDropMass>140)'
if cutChain=='SR_massSR':
   cuts=cuts_loose_zll_met50
   cuts += '&&(boostJetAK8_softDropMass>70 && boostJetAK8_softDropMass<140)'

if cutChain=='SR_massSB_btag' :
   cuts=cuts_loose_zll_met50
   cuts+='&&(boostJetAK8_softDropMass<70 || boostJetAK8_softDropMass>140)&&boostJetAK8_btagBOOSTED>0.8'
if cutChain=='SR_massSB_antibtag' :
   cuts=cuts_loose_zll_met50
   cuts+='&&(boostJetAK8_softDropMass<70 || boostJetAK8_softDropMass>140)&&boostJetAK8_btagBOOSTED<=0.8'
if cutChain=='SR_massSR_btag' :
   cuts=cuts_loose_zll_met50
   cuts+='&&(boostJetAK8_softDropMass>70 && boostJetAK8_softDropMass<140)&&boostJetAK8_btagBOOSTED>0.8'
if cutChain=='SR_massSR_antibtag' :
   cuts=cuts_loose_zll_met50
   cuts+='&&(boostJetAK8_softDropMass>70 && boostJetAK8_softDropMass<140)&&boostJetAK8_btagBOOSTED<=0.8'

# badmuon filter
cuts += '&&(nbadmuon==0)'
cuts += '&&(nboostJetAK8Puppi==1)'
#cuts += '&&boostJetAK8_tau21<0.6'
#cuts += '&&boostJetAK8_softDropMass>40'

'''
metfilter='(Flag_EcalDeadCellTriggerPrimitiveFilter&&Flag_HBHENoiseIsoFilter&&Flag_goodVertices&&Flag_HBHENoiseFilter&&Flag_globalTightHalo2016Filter&&Flag_eeBadScFilter)'
if UseMETFilter:
    cuts = '('+cuts+')&&'+metfilter
'''

cuts = '('+cuts+')'
print cuts

ROOT.gROOT.ProcessLine('.x tdrstyle.C') 

################################
# ZJets backgrounds
######################

# parameters for GJets
el_gjet_scale=1.00
mu_gjet_scale=1.00 

'''
if cutChain=='SR': 
        el_gjet_scale=1.01720174117
        mu_gjet_scale=1.0452050458
else:
        el_gjet_scale=1.02810944619 
        mu_gjet_scale=1.00061373085
'''

gdataYield = 892395.64372849464417
zjetsFidXsecAll = 2.7852682130237882419
zjetsFidXsecEl =  0.86284805348593140994
zjetsFidXsecMu =  1.899546018503648126
zjetsFidXsecAll_up = 2.886455724225923003
zjetsFidXsecAll_dn = 2.6860637110403673411
zjetsFidXsecEl_up = 0.89596620882464739211
zjetsFidXsecEl_dn = 0.83029756907083229756
zjetsFidXsecMu_up = 1.9667826839455833099
zjetsFidXsecMu_dn = 1.8337108912466133503
zjetsFidXsecLowLptAll = 3.1855070234185993705
zjetsFidXsecLowLptEl = 1.1701341293368183738
zjetsFidXsecLowLptMu = 2.0153728940818029791

gdataLumi=35.867*1000
gdataFidXsec=gdataYield/gdataLumi
zjetsFidXsecEl*=el_gjet_scale
zjetsFidXsecMu*=mu_gjet_scale
zjetsFidXsecEl_up*=el_gjet_scale
zjetsFidXsecEl_dn*=el_gjet_scale
zjetsFidXsecMu_up*=mu_gjet_scale
zjetsFidXsecMu_dn*=mu_gjet_scale

# for GJets photon bkg subtraction

    # all the factors below together normalized each process to the fraction of the process in the gjets data
    #   fidxsec_i / fidxsec_total
    # together with the gdata, we have:
    #  [ fidxsec_total-Sum(fidxsec_i) ]/fidxsec_total * fidxsec_zjets * lumi = zjets_yields
    # an additional scale factor GJetsNorm to absorbe the small difference.

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

phymetPlotters=[]
for sample in phymetSamples:

        phymetPlotters.append(TreePlotter(sample, indirGjets+'/'+sample+'.root','tree'))
        phymetPlotters[-1].addCorrectionFactor('1/SumWeights','norm') # negative weight for subtraction

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
        # because the weighted gjet data will be plotted as data, so need to as the lumi section in pb-1
        gdataPlotters[-1].addCorrectionFactor(str(lumi*1000),'luminosity')
        gdataPlotters[-1].addCorrectionFactor('GJetsPreScaleWeight','GJetsPreScaleWeight')
        gdataPlotters[-1].addCorrectionFactor('GJetsRhoWeight','GJetsRhoWeight')
        gdataPlotters[-1].addCorrectionFactor(str(1/gdataYield),'GJetsNorm0')
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
gjetsPlotters = gdataPlotters#+phymetPlotters
GJets = MergedPlotter(gjetsPlotters)

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

####################################################################

### MC ZJets
mczjetsSamples = [
    'DYJetsToLL_Pt_50To100',
    'DYJetsToLL_Pt_100To250',
    'DYJetsToLL_Pt_250To400',
    'DYJetsToLL_Pt_400To650',  
    'DYJetsToLL_Pt_650ToInf',
    ]

mczjetsPlotters=[]
for sample in mczjetsSamples:
        mczjetsPlotters.append(TreePlotter(sample, indirMCZjets+'/'+sample+'.root','tree'))
        mczjetsPlotters[-1].addCorrectionFactor('1./SumWeights','norm')
        mczjetsPlotters[-1].addCorrectionFactor(ZPtWeight,'ZPtWeight')
        mczjetsPlotters[-1].addCorrectionFactor('xsec','xsec') 
        mczjetsPlotters[-1].addCorrectionFactor('genWeight','genWeight')
        mczjetsPlotters[-1].addCorrectionFactor(puWeight,'puWeight')
        mczjetsPlotters[-1].addCorrectionFactor(lepsf,'lepsf')
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
if channel=='el':
    GJets.setAlias('llnunu_l1_mass_to_plot', 'llnunu_l1_mass_el')
    GJets.setAlias('llnunu_l2_pt_to_plot', 'llnunu_l2_pt_el')
    GJets.setAlias('llnunu_MET', 'llnunu_l2_pt_el')
    GJets.setAlias('llnunu_l2_phi_to_plot', 'llnunu_l2_phi_el')
    GJets.setAlias('llnunu_mt_to_plot', 'llnunu_mt_el')
    GJets.setAlias('llnunu_mT', 'llnunu_mt_el')
    for syst in ['JetEn','JetRes','MuonEn','ElectronEn','TauEn','PhotonEn','Uncluster'] :
        GJets.setAlias('llnunu_mT_'+syst+'Up', 'llnunu_mt_el_'+syst+'Up')
        GJets.setAlias('llnunu_mT_'+syst+'Dn', 'llnunu_mt_el_'+syst+'Dn')
        GJets.setAlias('llnunu_MET_'+syst+'Up', 'llnunu_l2_pt_el')
        GJets.setAlias('llnunu_MET_'+syst+'Dn', 'llnunu_l2_pt_el')

    GJets.setAlias('llnunu_mT_RecoilUp', 'llnunu_mt_el_RecoilUp')
    GJets.setAlias('llnunu_mT_RecoilDn', 'llnunu_mt_el_RecoilDn')
    GJets.setAlias('llnunu_MET_RecoilUp', 'llnunu_l2_pt_el_RecoilUp')
    GJets.setAlias('llnunu_MET_RecoilDn', 'llnunu_l2_pt_el_RecoilDn')

if channel=='mu':
    GJets.setAlias('llnunu_l1_mass_to_plot', 'llnunu_l1_mass_mu')
    GJets.setAlias('llnunu_l2_pt_to_plot', 'llnunu_l2_pt_mu')
    GJets.setAlias('llnunu_MET', 'llnunu_l2_pt_mu')
    GJets.setAlias('llnunu_l2_phi_to_plot', 'llnunu_l2_phi_mu')
    GJets.setAlias('llnunu_mt_to_plot', 'llnunu_mt_mu')
    GJets.setAlias('llnunu_mT', 'llnunu_mt_mu')
    for syst in ['JetEn','JetRes','MuonEn','ElectronEn','TauEn','PhotonEn','Uncluster'] :
        GJets.setAlias('llnunu_mT_'+syst+'Up', 'llnunu_mt_mu_'+syst+'Up')
        GJets.setAlias('llnunu_mT_'+syst+'Dn', 'llnunu_mt_mu_'+syst+'Dn')
        GJets.setAlias('llnunu_MET_'+syst+'Up', 'llnunu_l2_pt_mu')
        GJets.setAlias('llnunu_MET_'+syst+'Dn', 'llnunu_l2_pt_mu')

    GJets.setAlias('llnunu_mT_RecoilUp', 'llnunu_mt_mu_RecoilUp')
    GJets.setAlias('llnunu_mT_RecoilDn', 'llnunu_mt_mu_RecoilDn')
    GJets.setAlias('llnunu_MET_RecoilUp', 'llnunu_l2_pt_mu_RecoilUp')
    GJets.setAlias('llnunu_MET_RecoilDn', 'llnunu_l2_pt_mu_RecoilDn')


GJets.setAlias('llnunu_l1_mass_to_plot', 'llnunu_l1_mass')
GJets.setAlias('llnunu_l2_pt_to_plot', 'llnunu_l2_pt')
GJets.setAlias('llnunu_MET', 'llnunu_l2_pt')
GJets.setAlias('llnunu_l2_phi_to_plot', 'llnunu_l2_phi')
GJets.setAlias('llnunu_mt_to_plot', 'llnunu_mt')
GJets.setAlias('llnunu_mT', 'llnunu_mt')
for syst in ['JetEn','JetRes','MuonEn','ElectronEn','TauEn','PhotonEn','Uncluster'] :
        GJets.setAlias('llnunu_mT_'+syst+'Up', 'llnunu_mt')
        GJets.setAlias('llnunu_mT_'+syst+'Dn', 'llnunu_mt')
        GJets.setAlias('llnunu_MET_'+syst+'Up', 'llnunu_l2_pt')
        GJets.setAlias('llnunu_MET_'+syst+'Dn', 'llnunu_l2_pt')

GJets.setAlias('llnunu_mT_RecoilUp', 'llnunu_mt_RecoilUp')
GJets.setAlias('llnunu_mT_RecoilDn', 'llnunu_mt_RecoilDn')
GJets.setAlias('llnunu_MET_RecoilUp', 'llnunu_l2_pt_RecoilUp')
GJets.setAlias('llnunu_MET_RecoilDn', 'llnunu_l2_pt_RecoilDn')

############################
# Stack Plotter to draw all
############################

Stack = StackPlotter(outTag=tag, outDir=outdir)
Stack.setPaveText(paveText)

Stack.addPlotter(GJets, "GJets","ZJets(#gamma+Jets data)", "data")
Stack.addPlotter(MCZJets, "ZJets", "MC ZJets (DY)", "background")
 
PhyMET = MergedPlotter(phymetPlotters)

if channel=='el':
        PhyMET.setAlias('llnunu_l1_mass_to_plot', 'llnunu_l1_mass_el')
        PhyMET.setAlias('llnunu_l2_pt_to_plot', 'llnunu_l2_pt_el')
        PhyMET.setAlias('llnunu_MET', 'llnunu_l2_pt_el')
        PhyMET.setAlias('llnunu_l2_phi_to_plot', 'llnunu_l2_phi_el')
        PhyMET.setAlias('llnunu_mt_to_plot', 'llnunu_mt_el')
elif channel=='mu':
        PhyMET.setAlias('llnunu_l1_mass_to_plot', 'llnunu_l1_mass_mu')
        PhyMET.setAlias('llnunu_l2_pt_to_plot', 'llnunu_l2_pt_mu')
        PhyMET.setAlias('llnunu_MET', 'llnunu_l2_pt_mu')
        PhyMET.setAlias('llnunu_l2_phi_to_plot', 'llnunu_l2_phi_mu')
        PhyMET.setAlias('llnunu_mt_to_plot', 'llnunu_mt_mu')
else:
        PhyMET.setAlias('llnunu_l1_mass_to_plot', 'llnunu_l1_mass')
        PhyMET.setAlias('llnunu_l2_pt_to_plot', 'llnunu_l2_pt')
        PhyMET.setAlias('llnunu_MET', 'llnunu_l2_pt')
        PhyMET.setAlias('llnunu_l2_phi_to_plot', 'llnunu_l2_phi')
        PhyMET.setAlias('llnunu_mt_to_plot', 'llnunu_mt')

Stack.addPlotter(PhyMET, "NonReso","#gamma+jet phys. MET", "background")

Stack.setLog(LogY)
Stack.doRatio(doRatio)

tag+='_'

Stack.drawStack('abs(TVector2::Phi_mpi_pi(llnunu_l2_phi_to_plot-boostJetAK8_phi))', cuts, str(lumi*1000), 32, 0, 3.2,titlex = "#Delta#Phi_{MET, V_{had}}", units = "",output='dPhiVhadMET',outDir=outdir,separateSignal=sepSig,blinding=Blind,blindingCut=200)
Stack.drawStack('abs(TVector2::Phi_mpi_pi(llnunu_l1_phi-boostJetAK8_phi))', cuts, str(lumi*1000), 32, 0, 3.2,titlex = "#Delta#Phi_{V_{lep}, V_{had}}", units = "",output='dPhiVlepVhad',outDir=outdir,separateSignal=sepSig,blinding=Blind,blindingCut=200)
Stack.drawStack('abs(TVector2::Phi_mpi_pi(llnunu_l2_phi_to_plot-llnunu_l1_phi))', cuts, str(lumi*1000), 32, 0, 3.2,titlex = "#Delta#Phi_{MET, V_{lep}}", units = "",output='dPhiVlepMET',outDir=outdir,separateSignal=sepSig,blinding=Blind,blindingCut=200)

Stack.drawStack('llnunu_MET', cuts, str(lumi*1000), 40, 0, 1000, titlex = "E_{T}^{miss}", units = "GeV",output='MET',outDir=outdir,separateSignal=sepSig, blinding=Blind,blindingCut=300)

Stack.drawStack('llnunu_l2_pt_to_plot*cos(llnunu_l2_phi_to_plot-llnunu_l1_phi)', cuts, str(lumi*1000), 50, -500, 500.0, titlex = "E_{T#parallel}^{miss}", units = "GeV",output='met_para',outDir=outdir,separateSignal=sepSig, blinding=Blind,blindingCut=300)
Stack.drawStack('llnunu_l2_pt_to_plot*sin(llnunu_l2_phi_to_plot-llnunu_l1_phi)', cuts, str(lumi*1000), 50, -500, 500.0, titlex = "E_{T#perp}^{miss}", units = "GeV",output='met_perp',outDir=outdir,separateSignal=sepSig, blinding=Blind,blindingCut=300)

Stack.drawStack('llnunu_l2_pt_to_plot*cos(llnunu_l2_phi_to_plot-boostJetAK8_phi)', cuts, str(lumi*1000), 50, -500, 500.0, titlex = "E_{T#parallelAK8}^{miss}", units = "GeV",output='met_para',outDir=outdir,separateSignal=sepSig, blinding=Blind,blindingCut=300)
Stack.drawStack('llnunu_l2_pt_to_plot*sin(llnunu_l2_phi_to_plot-boostJetAK8_phi)', cuts, str(lumi*1000), 50, -500, 500.0, titlex = "E_{T#perpAK8}^{miss}", units = "GeV",output='met_perp',outDir=outdir,separateSignal=sepSig, blinding=Blind,blindingCut=300)

Stack.drawStack('nboostJetAK8Puppi', cuts, str(lumi*1000), 5, 0, 5, titlex = "N^{AK8Jet}", units = "",output='N^{AK8Jet}',outDir=outdir,separateSignal=sepSig, blinding=Blind,blindingCut=5)
Stack.drawStack('boostJetAK8_pt', cuts, str(lumi*1000), 80, 0, 2000, titlex = "P_{T}^{AK8Jet}", units = "GeV",output='P_{T}^{AK8Jet}',outDir=outdir,separateSignal=sepSig, blinding=Blind,blindingCut=300)
Stack.drawStack('llnunu_l1_pt', cuts, str(lumi*1000), 80, 0, 2000, titlex = "PT_{Z/#gamma}", units = "GeV",output='zpt',outDir=outdir,separateSignal=sepSig,blinding=Blind,blindingCut=200)

Stack.drawStack('boostJetAK8_btagBOOSTED', cuts, str(lumi*1000), 40, -1, 1, titlex = "BoostBtag", units = "",output='BoostBtag',outDir=outdir,separateSignal=sepSig, blinding=Blind,blindingCut=300)
Stack.drawStack('boostJetAK8_softDropMass', cuts, str(lumi*1000), 32, 40, 200, titlex = "SoftDrop Mass", units = "GeV",output='SoftDrop Mass',outDir=outdir,separateSignal=sepSig, blinding=Blind,blindingCut=300)
Stack.drawStack('boostJetAK8_tau21', cuts, str(lumi*1000), 40, 0, 1, titlex = "#tau_{21}", units = "", output='#tau_{21}',outDir=outdir,separateSignal=sepSig, blinding=Blind,blindingCut=300)

Stack.drawStack('mR', cuts, str(lumi*1000), 40, 0, 4000, titlex = "M_{R}", units = "GeV",output='mR',outDir=outdir,separateSignal=sepSig, blinding=Blind,blindingCut=300)
Stack.drawStack('mRT', cuts, str(lumi*1000), 40, 0, 4000, titlex = "M_{RT}", units = "GeV",output='mRT',outDir=outdir,separateSignal=sepSig, blinding=Blind,blindingCut=300)
Stack.drawStack('R', cuts, str(lumi*1000), 40, 0, 1, titlex = "R", units = "",output='R',outDir=outdir,separateSignal=sepSig, blinding=Blind,blindingCut=300)

Stack.closePSFile()
Stack.closeROOTFile()

