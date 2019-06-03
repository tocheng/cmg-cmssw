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
doRhoScale=False
doGMCEtaScale=False
doGMCPhPtScale=True
dyGJets=options.dyGJets
muoneg=options.muoneg

if test: DrawLeptons = False

#lepsf="(1)"
#lepsf="idsf"
#lepsf="trgsf"
#lepsf="isosf"
#lepsf="isosf*idsf"
#lepsf="trgsf*isosf*idsf"
lepsf="trgsf*isosf*idsf*trksf"


g_scale='(1)'
mc_scale='(1)'
zjets_scale='(1)'

if channel=='mu': 
#    mc_scale='(1.02942)'
    if cutChain=='SR': zjets_scale='(0.960865)' # mc SR
    else: zjets_scale='(1.03410)' # mc zpt50
elif channel=='el': 
#    mc_scale='(1.02139)'
    if cutChain=='SR': zjets_scale='(1.01822)' # mc SR
    else: zjets_scale='(1.03737)' # mc zpt50
else: 
#    mc_scale='(1.02942)'
    zjets_scale='(1.03741)'

# temp turn off mc_scale
#mc_scale="(1)"
#zjets_scale="(1)"

# non reso alpha
nonreso_alpha_el=1.0
nonreso_alpha_mu=1.0

nonreso_alpha_el=0.397075177316
nonreso_alpha_mu=0.704939528419

#

#
if doRhoScale:
    tag+="RhoWt_"
    rho_scale = "*(0.366*TMath::Gaus(rho,8.280,5.427)+0.939*TMath::Gaus(rho,18.641,10.001)+0.644*TMath::Gaus(rho,40.041,10.050))" # 2016 rereco/summer16 81.81 fb-1
    lepsf += rho_scale
    g_scale += rho_scale

if doGMCEtaScale:
    tag+="GMCEtaWt_"
    g_scale=g_scale+"*(1.05*TMath::Gaus(llnunu_l1_eta,0.7,0.63)+1.05*TMath::Gaus(llnunu_l1_eta,-0.7,0.63)+0.94*TMath::Gaus(llnunu_l1_eta,2.02,0.34)+0.94*TMath::Gaus(llnunu_l1_eta,-2.02,0.34))"

if doGMCPhPtScale:
    tag+="GMCPhPtWt_"
#    g_scale+="*((-0.371771+0.0193019*pow(llnunu_l1_pt,1)-0.000119102*pow(llnunu_l1_pt,2)+3.90785e-07*pow(llnunu_l1_pt,3)-7.29192e-10*pow(llnunu_l1_pt,4)+7.7063e-13*pow(llnunu_l1_pt,5)-4.27744e-16*pow(llnunu_l1_pt,6)+9.61926e-20*pow(llnunu_l1_pt,7))*(llnunu_l1_pt<=900)+(0.723945)*(llnunu_l1_pt>900))"  # for allcorV2
#    g_scale+="*((-0.0359107+0.0106695*llnunu_l1_pt-4.35056e-05*pow(llnunu_l1_pt,2)+7.6524e-08*pow(llnunu_l1_pt,3)-6.28775e-11*pow(llnunu_l1_pt,4)+1.9693e-14*pow(llnunu_l1_pt,5))*(llnunu_l1_pt<=900)+(0.487691)*(llnunu_l1_pt>900))"  # for ReReco
    g_scale+="*((0.295668+0.0127154*llnunu_l1_pt-7.71163e-05*pow(llnunu_l1_pt,2)+2.2603e-07*pow(llnunu_l1_pt,3)-3.50496e-10*pow(llnunu_l1_pt,4)+2.7572e-13*pow(llnunu_l1_pt,5)-8.66455e-17*pow(llnunu_l1_pt,6))*(llnunu_l1_pt<=800)+(0.912086)*(llnunu_l1_pt>800))"  # for reminiaod allcorV2 mc hlt
#    g_scale+="*((0.322959+0.0107055*llnunu_l1_pt-5.56587e-05*pow(llnunu_l1_pt,2)+1.26764e-07*pow(llnunu_l1_pt,3)-1.49478e-10*pow(llnunu_l1_pt,4)+8.91559e-14*pow(llnunu_l1_pt,5)-2.13034e-17*pow(llnunu_l1_pt,6))*(llnunu_l1_pt<=900)+(0.536969)*(llnunu_l1_pt>900))"  # for ReReco mc hlt

outdir='plots'

#indir='/home/heli/XZZ/80X_20170202_light_hlt_Skim/'
#indir='/home/heli/XZZ/80X_20170202_light_hlt_RcSkim/'
indir='/home/heli/XZZ/80X_20170202_light_hlt_allcorV2RcSkim/'
#indir='/home/heli/XZZ/80X_20170202_light_hlt_allcorV2Skim/'
#indir='/home/heli/XZZ/80X_20170202_light_Skim/'
lumi=35.87
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
cuts_zpt300="(llnunu_l1_pt>300)"
cuts_met50="(llnunu_l2_pt_to_plot>50)"
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
cuts_loose_zpt300="("+cuts_lepaccept+"&&"+cuts_zmass+"&&"+cuts_zpt300+")"
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
cuts_zptgt55_metgt125="("+cuts_lepaccept+"&&"+cuts_zmass+"&&llnunu_l1_pt>55&&llnunu_l2_pt_to_plot>125)"
cuts_CR="("+cuts_lepaccept+"&&"+cuts_zmass+"&&llnunu_l1_pt>50&&!(llnunu_l1_pt>100&&llnunu_l2_pt_to_plot>50))"
cuts_CR1="("+cuts_lepaccept+"&&"+cuts_zmass+"&&llnunu_l1_pt>100&&llnunu_l2_pt_to_plot<50)"
cuts_CR2="("+cuts_lepaccept+"&&"+cuts_zmass+"&&llnunu_l1_pt>50&&llnunu_l1_pt<100&&llnunu_l2_pt_to_plot>50)"
cuts_CR3="("+cuts_lepaccept+"&&"+cuts_zmass+"&&llnunu_l1_pt>50&&llnunu_l1_pt<100&&llnunu_l2_pt_to_plot<50)"

if cutChain=='loosecut': cuts=cuts_loose
elif cutChain=='lepaccept': cuts=cuts_lepaccept
elif cutChain=='tight': cuts=cuts_loose_z
elif cutChain=='tightzpt20': cuts=cuts_loose_zpt20
elif cutChain=='tightzpt50': cuts=cuts_loose_zpt50
elif cutChain=='tightzptgt50lt200': cuts=cuts_loose_zptgt50lt200
elif cutChain=='tightzptgt100lt400': cuts=cuts_loose_zptgt100lt400
elif cutChain=='tightzpt100': cuts=cuts_loose_zll
elif cutChain=='tightzpt150': cuts=cuts_loose_zpt150
elif cutChain=='tightzpt200': cuts=cuts_loose_zpt200
elif cutChain=='tightzpt300': cuts=cuts_loose_zpt300
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
elif cutChain=='tightzptgt55metgt125': cuts=cuts_zptgt55_metgt125
elif cutChain=='SR': cuts=cuts_loose_zll_met50
elif cutChain=='CR': cuts=cuts_CR
elif cutChain=='CR1': cuts=cuts_CR1
elif cutChain=='CR2': cuts=cuts_CR2
elif cutChain=='CR3': cuts=cuts_CR3
elif cutChain=='SRmetParaLTm80': cuts=cuts_loose_zll_met50+"&&llnunu_l2_pt_to_plot*cos(llnunu_l2_phi_to_plot-llnunu_l1_phi)<-80"
else : cuts=cuts_loose


if UseMETFilter:
    cuts = '('+cuts+')' # metfilter pre-applied in preskim

# badmuon filter
#cuts += '&&(nbadmuon==0&&nlep<=2)'  # veto 3+ leptons
cuts += '&&(nbadmuon==0)' 

cuts = '('+cuts+')'

ROOT.gROOT.ProcessLine('.x tdrstyle.C') 


#######################
#  VV Reso backgrounds
#######################
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
VV.setAlias('llnunu_l2_phi_to_plot', 'llnunu_l2_phi')
VV.setAlias('llnunu_mt_to_plot', 'llnunu_mt')



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

    nonresSamples = ['muoneg_light_skim','DYJetsToLL_emupair']

    nonresPlotters=[]
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
    NONRES.setAlias('llnunu_l2_phi_to_plot', 'llnunu_l2_phi')
    NONRES.setAlias('llnunu_mt_to_plot', 'llnunu_mt')

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
    MCNONRESO.setAlias('llnunu_l2_phi_to_plot', 'llnunu_l2_phi')
    MCNONRESO.setAlias('llnunu_mt_to_plot', 'llnunu_mt')



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
    #'G_DYJetsToLL_M50_MGMLM_Ext1',
    'G_DYJetsToLL_M50_Ext',
    'G_TBar_tWch',
    'G_TBar_tch_powheg',
    'G_TGJets_BIG', 
    'G_TTGJets_BIG', 
    'G_T_tWch',  
    'G_T_tch_powheg', 
    'G_WGToLNuG',
    #'G_WGJetsPt130', 
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
    #'SinglePhoton_Run2016Full_03Feb2017_uncorr', 
    #'SinglePhoton_Run2016Full_03Feb2017_allcor', 
    'SinglePhoton_Run2016Full_03Feb2017_allcorV2', 
    #'SinglePhoton_Run2016Full_03Feb2017_allcorV2_NoRecoil', 
    #'SinglePhoton_Run2016Full_03Feb2017_v0', 
    #'SinglePhoton_Run2016Full_ReReco_v2', 
    #'SinglePhoton_Run2016Full_ReReco_v2_oldSkim', 
    #'SinglePhoton_Run2016Full_ReReco_v2_ReSkim', 
    #'SinglePhoton_Run2016Full_ReReco_v2_RePreSkim', 
    #'SinglePhoton_Run2016Full_ReReco_v2_RePreSkim_RcNoSmooth', 
    #'SinglePhoton_Run2016Full_ReReco_v2_NoRecoil', 
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
    #gjetsPlotters = gdataPlotters
    gjetsPlotters = gdataPlotters+phymetPlotters


    GJets = MergedPlotter(gjetsPlotters)
    GJets.setFillProperties(1001,ROOT.kGreen+2)

    # some plotting definition
    if channel=='el':
        GJets.setAlias('llnunu_l1_mass_to_plot', 'llnunu_l1_mass_el')
        GJets.setAlias('llnunu_l2_pt_to_plot', 'llnunu_l2_pt_el')
        GJets.setAlias('llnunu_l2_phi_to_plot', 'llnunu_l2_phi_el')
        GJets.setAlias('llnunu_mt_to_plot', 'llnunu_mt_el')
    elif channel=='mu':
        GJets.setAlias('llnunu_l1_mass_to_plot', 'llnunu_l1_mass_mu')
        GJets.setAlias('llnunu_l2_pt_to_plot', 'llnunu_l2_pt_mu')
        GJets.setAlias('llnunu_l2_phi_to_plot', 'llnunu_l2_phi_mu')
        GJets.setAlias('llnunu_mt_to_plot', 'llnunu_mt_mu')
    else:
        GJets.setAlias('llnunu_l1_mass_to_plot', 'llnunu_l1_mass')
        GJets.setAlias('llnunu_l2_pt_to_plot', 'llnunu_l2_pt')
        GJets.setAlias('llnunu_l2_phi_to_plot', 'llnunu_l2_phi')
        GJets.setAlias('llnunu_mt_to_plot', 'llnunu_mt')


#if not dyGJets
else: 

    ### MC ZJets
    mczjetsSamples = [
    'DYJetsToLL_M50_Ext',
    ]

    mczjetsPlotters=[]
    for sample in mczjetsSamples:
        mczjetsPlotters.append(TreePlotter(sample, indir+'/'+sample+'.root','tree'))
        mczjetsPlotters[-1].addCorrectionFactor('1./SumWeights','norm')
        #mczjetsPlotters[-1].addCorrectionFactor('(1)','norm')
        mczjetsPlotters[-1].addCorrectionFactor(ZPtWeight,'ZPtWeight')
        if 'DY1JetsToLL' in sample: mczjetsPlotters[-1].addCorrectionFactor('(1177.7274)', 'xsec')
        elif 'DY2JetsToLL' in sample: mczjetsPlotters[-1].addCorrectionFactor('(389.1267)', 'xsec')
        elif 'DY3JetsToLL' in sample: mczjetsPlotters[-1].addCorrectionFactor('(119.0516)', 'xsec')
        elif 'DY4JetsToLL' in sample: mczjetsPlotters[-1].addCorrectionFactor('(63.3043)', 'xsec')
        else: mczjetsPlotters[-1].addCorrectionFactor('xsec','xsec') 
        mczjetsPlotters[-1].addCorrectionFactor('genWeight','genWeight')
        #mczjetsPlotters[-1].addCorrectionFactor("ZJetsGenWeight",'genWeight')
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
    MCZJets.setAlias('llnunu_l2_phi_to_plot', 'llnunu_l2_phi')
    MCZJets.setAlias('llnunu_mt_to_plot', 'llnunu_mt')


# end if dyGJets:..., else: ...

##
# choose GJets or ZJets MC
if dyGJets:
  ZJets = GJets
else: 
  ZJets = MCZJets



######################
# Signal samples
#####################

sigSamples = [
'BulkGravToZZToZlepZinv_narrow_600',
#'BulkGravToZZToZlepZinv_narrow_800',
'BulkGravToZZToZlepZinv_narrow_1000',
#'BulkGravToZZToZlepZinv_narrow_1200',
#'BulkGravToZZToZlepZinv_narrow_1400',
'BulkGravToZZToZlepZinv_narrow_1600', 
#'BulkGravToZZToZlepZinv_narrow_1800', 
#'BulkGravToZZToZlepZinv_narrow_2000',
#'BulkGravToZZToZlepZinv_narrow_2500',
#'BulkGravToZZToZlepZinv_narrow_3000',
#'BulkGravToZZToZlepZinv_narrow_3500', 
#'BulkGravToZZToZlepZinv_narrow_4000', 
#'BulkGravToZZToZlepZinv_narrow_4500', 
]


sigPlotters=[]
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
    sigPlotters[-1].setAlias('passMuHLT', '((llnunu_l1_l1_trigerob_HLTbit>>3&1)||(llnunu_l1_l1_trigerob_HLTbit>>4&1)||(llnunu_l1_l2_trigerob_HLTbit>>3&1)||(llnunu_l1_l2_trigerob_HLTbit>>4&1))');
    sigPlotters[-1].setAlias('passElHLT', '((llnunu_l1_l1_trigerob_HLTbit>>1&1)||(llnunu_l1_l2_trigerob_HLTbit>>1&1))');
    sigPlotters[-1].addCorrectionFactor('(passMuHLT||passElHLT)','HLT')
    sigPlotters[-1].setAlias('llnunu_l1_mass_to_plot', 'llnunu_l1_mass')
    sigPlotters[-1].setAlias('llnunu_l2_pt_to_plot', 'llnunu_l2_pt')
    sigPlotters[-1].setAlias('llnunu_l2_phi_to_plot', 'llnunu_l2_phi')
    sigPlotters[-1].setAlias('llnunu_mt_to_plot', 'llnunu_mt')



##########################
# Data Observed
##########################

dataSamples = [
'SingleEMU_Run2016Full_03Feb2017_allcorV2',
#'SingleEMU_Run2016Full_03Feb2017_v0',
#'SingleEMU_Run2016Full_ReReco_v2_DtReCalib',
#'SingleEMU_Run2016Full_ReReco_v2',
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
Data.setAlias('llnunu_l2_phi_to_plot', 'llnunu_l2_phi')
Data.setAlias('llnunu_mt_to_plot', 'llnunu_mt')




############################
# Stack Plotter to draw all
############################

Stack = StackPlotter(outTag=tag, outDir=outdir)
Stack.setPaveText(paveText)
Stack.addPlotter(Data, "data_obs", "Data", "data")
if muoneg: 
    Stack.addPlotter(NONRES, "NonReso","Non-reson. (e#mu data)", "background")
else:
    Stack.addPlotter(MCNONRESO, "NonReso","Non-reson. (MC WW/Top/W/QCD)", "background")
Stack.addPlotter(VV, "VVZReso","Z reson. (MC ZZ/WZ/TTZ)", "background")
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


if test: 
    Stack.drawStack('nVert', cuts, str(lumi*1000), 80, 0.0, 80.0, titlex = "N vertices", units = "",output='nVert',outDir=outdir,separateSignal=sepSig)
#    Stack.drawStack('rho', cuts, str(lumi*1000), 55, 0.0, 55.0, titlex = "#rho", units = "",output='rho',outDir=outdir,separateSignal=sepSig)
#    Stack.drawStack('llnunu_l1_pt', cuts, str(lumi*1000), 30, 0.0, 1500.0, titlex = "P_{T}(Z)", units = "GeV",output='zpt',outDir=outdir,separateSignal=sepSig)
#    Stack.drawStack('llnunu_l1_mass_to_plot', cuts, str(lumi*1000), 60, 60, 120, titlex = "M(Z)", units = "GeV",output='zmass',outDir=outdir,separateSignal=sepSig)
#    Stack.drawStack('llnunu_mt_to_plot', cuts, str(lumi*1000), 50, 100.0, 1600.0, titlex = "M_{T}", units = "GeV",output='mt',outDir=outdir,separateSignal=sepSig,blinding=Blind,blindingCut=300)
#    Stack.drawStack('llnunu_l2_pt_to_plot', cuts, str(lumi*1000), 30, 0, 1500, titlex = "MET", units = "GeV",output='met',outDir=outdir,separateSignal=sepSig,blinding=Blind,blindingCut=200)
#    Stack.drawStack('llnunu_l2_pt_to_plot*cos(llnunu_l2_phi_to_plot-llnunu_l1_phi)', cuts, str(lumi*1000), 25, -500, 500.0, titlex = "MET_{#parallel}", units = "GeV",output='met_para',outDir=outdir,separateSignal=sepSig)

#    Stack.drawStack('llnunu_l1_pt', cuts, str(lumi*1000), 200, 0.0, 2000.0, titlex = "P_{T}(Z)", units = "GeV",output='zpt_high2k',outDir=outdir,separateSignal=sepSig)
#    Stack.drawStack('llnunu_mt_to_plot', cuts, str(lumi*1000), 300, 0.0, 3000.0, titlex = "M_{T}", units = "GeV",output='mt_high3k',outDir=outdir,separateSignal=sepSig,blinding=Blind,blindingCut=300)

#    if not dyGJets and  channel=='mu' :
#        Stack.drawStack('llnunu_l1_l1_pt', cuts+"&&(abs(llnunu_l1_l1_pdgId)==13)", str(lumi*1000), 300, 0.0, 3000.0, titlex = "P_{T}(#mu_{1})", units = "GeV",output='pTlep1_mu_high3k',outDir=outdir,separateSignal=sepSig)
#    if not dyGJets and  channel=='el' :
#        Stack.drawStack('llnunu_l1_l1_pt', cuts+"&&(abs(llnunu_l1_l1_pdgId)==11)", str(lumi*1000), 300, 0.0, 3000.0, titlex = "P_{T}(e_{1})", units = "GeV",output='pTlep1_el_high3k',outDir=outdir,separateSignal=sepSig)

#    Stack.drawStack('llnunu_l1_pt', cuts, str(lumi*1000), 50, 0.0, 500.0, titlex = "P_{T}(Z)", units = "GeV",output='zpt_low',outDir=outdir,separateSignal=sepSig)
#    Stack.drawStack('llnunu_l1_pt', cuts, str(lumi*1000), 75, 0.0, 1500.0, titlex = "P_{T}(Z)", units = "GeV",output='zpt',outDir=outdir,separateSignal=sepSig)
#    Stack.drawStack('llnunu_l1_mass_to_plot', cuts, str(lumi*1000), 100, 0, 200, titlex = "M(Z)", units = "GeV",output='zmass',outDir=outdir,separateSignal=sepSig)
#    Stack.drawStack('llnunu_mt_to_plot', cuts, str(lumi*1000), 50, 100.0, 600.0, titlex = "M_{T}", units = "GeV",output='mt_low',outDir=outdir,separateSignal=sepSig,blinding=Blind,blindingCut=300)
#    Stack.drawStack('llnunu_mt_to_plot', cuts, str(lumi*1000), 55, 100.0, 1200.0, titlex = "M_{T}", units = "GeV",output='mt',outDir=outdir,separateSignal=sepSig,blinding=Blind,blindingCut=300)
#    Stack.drawStack('llnunu_mt_to_plot', cuts, str(lumi*1000), 100, 0.0, 3000.0, titlex = "M_{T}", units = "GeV",output='mt',outDir=outdir,separateSignal=sepSig,blinding=Blind,blindingCut=300)
#    Stack.drawStack('llnunu_mt_to_plot', cuts, str(lumi*1000), 300, 0.0, 3000.0, titlex = "M_{T}", units = "GeV",output='mt',outDir=outdir,separateSignal=sepSig,blinding=Blind,blindingCut=300)

#    Stack.drawStack('llnunu_l2_pt_to_plot', cuts, str(lumi*1000), 50, 0, 500, titlex = "MET", units = "GeV",output='met_low',outDir=outdir,separateSignal=sepSig,blinding=Blind,blindingCut=200)
#    Stack.drawStack('llnunu_l2_pt_to_plot', cuts, str(lumi*1000), 50, 0, 1000, titlex = "MET", units = "GeV",output='met',outDir=outdir,separateSignal=sepSig,blinding=Blind,blindingCut=200)
#    Stack.drawStack('llnunu_l2_pt_to_plot*sin(llnunu_l2_phi_to_plot-llnunu_l1_phi)', cuts, str(lumi*1000), 50, -500, 500.0, titlex = "MET_{#perp}", units = "GeV",output='met_perp',outDir=outdir,separateSignal=sepSig)
#    Stack.drawStack('fabs(TVector2::Phi_mpi_pi(llnunu_l2_phi_to_plot-llnunu_l1_phi))', cuts, str(lumi*1000), 50, 0, 5, titlex = "#Delta#phi(Z,MET)", units = "",output='dphiZMet',outDir=outdir,separateSignal=sepSig)


else: 
    Stack.drawStack('nVert', cuts, str(lumi*1000), 80, 0.0, 80.0, titlex = "N vertices", units = "",output='nVert',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('rho', cuts, str(lumi*1000), 55, 0.0, 55.0, titlex = "#rho", units = "",output='rho',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('llnunu_mt_to_plot', cuts, str(lumi*1000), 100, 0.0, 3000.0, titlex = "M_{T}", units = "GeV",output='mt_high3',outDir=outdir,separateSignal=sepSig,blinding=Blind,blindingCut=300)
    Stack.drawStack('llnunu_mt_to_plot', cuts, str(lumi*1000), 50, 100.0, 1600.0, titlex = "M_{T}", units = "GeV",output='mt_high',outDir=outdir,separateSignal=sepSig,blinding=Blind,blindingCut=300)
    Stack.drawStack('llnunu_mt_to_plot', cuts, str(lumi*1000), 55, 100.0, 1200.0, titlex = "M_{T}", units = "GeV",output='mt',outDir=outdir,separateSignal=sepSig,blinding=Blind,blindingCut=300)
    Stack.drawStack('llnunu_mt_to_plot', cuts, str(lumi*1000), 50, 100.0, 600.0, titlex = "M_{T}", units = "GeV",output='mt_low',outDir=outdir,separateSignal=sepSig,blinding=Blind,blindingCut=300)
    Stack.drawStack('llnunu_mt_to_plot', cuts, str(lumi*1000), 80, 100.0, 300.0, titlex = "M_{T}", units = "GeV",output='mt_low2',outDir=outdir,separateSignal=sepSig,blinding=Blind,blindingCut=300)
    Stack.drawStack('llnunu_l1_mass_to_plot', cuts, str(lumi*1000), 60, 60, 120, titlex = "M(Z)", units = "GeV",output='zmass',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('llnunu_l1_pt', cuts, str(lumi*1000), 30, 0.0, 1500.0, titlex = "P_{T}(Z)", units = "GeV",output='zpt_high',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('llnunu_l1_pt', cuts, str(lumi*1000), 75, 0.0, 1500.0, titlex = "P_{T}(Z)", units = "GeV",output='zpt',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('llnunu_l1_pt', cuts, str(lumi*1000), 50, 0.0, 500.0, titlex = "P_{T}(Z)", units = "GeV",output='zpt_low',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('llnunu_l1_pt', cuts, str(lumi*1000), 100, 0.0, 300.0, titlex = "P_{T}(Z)", units = "GeV",output='zpt_low2',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('llnunu_l1_eta', cuts, str(lumi*1000), 200, -10.0, 10.0, titlex = "#eta(Z) ", units = "",output='zeta',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('llnunu_l1_rapidity', cuts, str(lumi*1000), 60, -3.0, 3.0, titlex = "Rapidity(Z) ", units = "",output='zrapidity',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('llnunu_l1_phi', cuts, str(lumi*1000), 64, -3.2, 3.2, titlex = "#phi(Z)", units = "",output='zphi',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('llnunu_l2_pt_to_plot', cuts, str(lumi*1000), 30, 0, 1500, titlex = "MET", units = "GeV",output='met_high',outDir=outdir,separateSignal=sepSig,blinding=Blind,blindingCut=200)
    Stack.drawStack('llnunu_l2_pt_to_plot', cuts, str(lumi*1000), 50, 0, 1000, titlex = "MET", units = "GeV",output='met',outDir=outdir,separateSignal=sepSig,blinding=Blind,blindingCut=200)
    Stack.drawStack('llnunu_l2_pt_to_plot', cuts, str(lumi*1000), 50, 0, 500, titlex = "MET", units = "GeV",output='met_low',outDir=outdir,separateSignal=sepSig,blinding=Blind,blindingCut=200)
    Stack.drawStack('llnunu_l2_pt_to_plot', cuts, str(lumi*1000), 100, 0, 300, titlex = "MET", units = "GeV",output='met_low2',outDir=outdir,separateSignal=sepSig,blinding=Blind,blindingCut=200)
    Stack.drawStack('llnunu_l2_phi_to_plot', cuts, str(lumi*1000), 100, -3.2, 3.2, titlex = "#phi(MET)", units = "",output='metPhi',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('llnunu_l2_sumEt', cuts, str(lumi*1000), 80, 0.0, 3000.0, titlex = "sumE_{T}", units = "GeV",output='metSumEt',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('llnunu_l2_pt_to_plot*cos(llnunu_l2_phi_to_plot-llnunu_l1_phi)', cuts, str(lumi*1000), 100, -300, 300.0, titlex = "MET_{#parallel}", units = "GeV",output='met_para',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('llnunu_l2_pt_to_plot*sin(llnunu_l2_phi_to_plot-llnunu_l1_phi)', cuts, str(lumi*1000), 100, -300, 300.0, titlex = "MET_{#perp}", units = "GeV",output='met_perp',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('llnunu_l2_pt_to_plot*cos(llnunu_l2_phi_to_plot-llnunu_l1_phi)', cuts, str(lumi*1000), 50, -500, 500.0, titlex = "MET_{#parallel}", units = "GeV",output='met_para_high',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('llnunu_l2_pt_to_plot*sin(llnunu_l2_phi_to_plot-llnunu_l1_phi)', cuts, str(lumi*1000), 50, -500, 500.0, titlex = "MET_{#perp}", units = "GeV",output='met_perp_high',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('llnunu_l2_pt_to_plot/sqrt(llnunu_l2_sumEt)', cuts, str(lumi*1000), 100, 0.0, 20.0, titlex = "MET/#sqrt{sumE_{T}}", units = "",output='metOvSqSET',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('fabs(TVector2::Phi_mpi_pi(llnunu_l2_phi_to_plot-llnunu_l1_phi))', cuts, str(lumi*1000), 50, 0, 5, titlex = "#Delta#phi(Z,MET)", units = "",output='dphiZMet',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('fabs(llnunu_l1_pt+llnunu_l2_pt_to_plot*cos(llnunu_l2_phi_to_plot-llnunu_l1_phi))', cuts, str(lumi*1000), 100, 0, 1000, titlex = "#Delta P_{T}^{#parallel}(Z,MET)", units = "GeV",output='dPTPara',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('fabs(llnunu_l2_pt_to_plot*sin(llnunu_l2_phi_to_plot-llnunu_l1_phi))', cuts, str(lumi*1000), 100, 0, 100, titlex = "#Delta P_{T}^{#perp}(Z,MET)", units = "GeV",output='dPTPerp',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('fabs(llnunu_l1_pt+llnunu_l2_pt_to_plot*cos(llnunu_l2_phi_to_plot-llnunu_l1_phi))/llnunu_l1_pt', cuts, str(lumi*1000), 100, 0, 5, titlex = "#Delta P_{T}^{#parallel}(Z,MET)/P_{T}(Z)", units = "",output='dPTParaRel',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('fabs(llnunu_l2_pt_to_plot*sin(llnunu_l2_phi_to_plot-llnunu_l1_phi))/llnunu_l1_pt', cuts, str(lumi*1000), 100, 0, 5, titlex = "#Delta P_{T}^{#perp}(Z,MET)/P_{T}", units = "",output='dPTPerpRel',outDir=outdir,separateSignal=sepSig)

    if not dyGJets: 
        Stack.drawStack('llnunu_l1_deltaPhi', cuts, str(lumi*1000), 50, 0.0, 5.0, titlex = "#Delta#phi", units = "",output='ZdeltaPhi',outDir=outdir,separateSignal=sepSig)
        Stack.drawStack('llnunu_l1_deltaR', cuts, str(lumi*1000), 50, 0.0, 5.0, titlex = "#Delta R", units = "",output='ZdeltaR',outDir=outdir,separateSignal=sepSig)
        Stack.drawStack('TMath::Tan((TMath::Pi()-TMath::Abs(llnunu_l1_deltaPhi))/2.0)*TMath::Sin(TMath::ACos(TMath::TanH((llnunu_l1_l1_eta-llnunu_l1_l2_eta)/2.0)))', cuts, str(lumi*1000), 100, 0.0, 10, titlex = "#phi_{#eta}*", units = "",output='PhiStar',outDir=outdir,separateSignal=sepSig)
  


    if DrawLeptons and not dyGJets and  channel=='mu' :
        Stack.drawStack('llnunu_l1_l1_pt', cuts+"&&(abs(llnunu_l1_l1_pdgId)==13)", str(lumi*1000), 100, 0.0, 1000.0, titlex = "P_{T}(#mu_{1})", units = "GeV",output='pTlep1_mu',outDir=outdir,separateSignal=sepSig)
        Stack.drawStack('llnunu_l1_l1_eta', cuts+"&&(abs(llnunu_l1_l1_pdgId)==13)", str(lumi*1000), 30, -3.0, 3.0, titlex = "#eta(#mu_{1})", units = "",output='etalep1_mu',outDir=outdir,separateSignal=sepSig)
        Stack.drawStack('llnunu_l1_l1_phi', cuts+"&&(abs(llnunu_l1_l1_pdgId)==13)", str(lumi*1000), 32, -3.2, 3.2, titlex = "#phi(#mu_{1})", units = "",output='philep1_mu',outDir=outdir,separateSignal=sepSig)
        Stack.drawStack('llnunu_l1_l1_trackerIso', cuts+"&&(abs(llnunu_l1_l1_pdgId)==13)", str(lumi*1000), 100, 0.0, 0.2, titlex = "trackerISO_{rel}(#mu_{1})", units = "",output='ISOlep1_mu',outDir=outdir,separateSignal=sepSig)
        Stack.drawStack('llnunu_l1_l2_pt', cuts+"&&(abs(llnunu_l1_l2_pdgId)==13)", str(lumi*1000), 100, 0.0, 500.0, titlex = "P_{T}(#mu_{2})", units = "GeV",output='pTlep2_mu',outDir=outdir,separateSignal=sepSig)
        Stack.drawStack('llnunu_l1_l2_eta', cuts+"&&(abs(llnunu_l1_l2_pdgId)==13)", str(lumi*1000), 30, -3.0, 3.0, titlex = "#eta(#mu_{2})", units = "",output='etalep2_mu',outDir=outdir,separateSignal=sepSig)
        Stack.drawStack('llnunu_l1_l2_phi', cuts+"&&(abs(llnunu_l1_l2_pdgId)==13)", str(lumi*1000), 32, -3.2, 3.2, titlex = "#phi(#mu_{2})", units = "",output='philep2_mu',outDir=outdir,separateSignal=sepSig)
        Stack.drawStack('llnunu_l1_l2_trackerIso', cuts+"&&(abs(llnunu_l1_l2_pdgId)==13)", str(lumi*1000), 100, 0.0, 0.2, titlex = "trackerISO_{rel}(#mu_{2})", units = "",output='ISOlep2_mu',outDir=outdir,separateSignal=sepSig)

    if DrawLeptons and not dyGJets and  channel=='el' :
        Stack.drawStack('llnunu_l1_l1_pt', cuts+"&&(abs(llnunu_l1_l1_pdgId)==11)", str(lumi*1000), 100, 0.0, 1000.0, titlex = "P_{T}(e_{1})", units = "GeV",output='pTlep1_el',outDir=outdir,separateSignal=sepSig)
        Stack.drawStack('llnunu_l1_l1_eta', cuts+"&&(abs(llnunu_l1_l1_pdgId)==11)", str(lumi*1000), 30, -3.0, 3.0, titlex = "#eta(e_{1})", units = "",output='etalep1_el',outDir=outdir,separateSignal=sepSig)
        Stack.drawStack('llnunu_l1_l1_phi', cuts+"&&(abs(llnunu_l1_l1_pdgId)==11)", str(lumi*1000), 32, -3.2, 3.2, titlex = "#phi(e_{1})", units = "",output='philep1_el',outDir=outdir,separateSignal=sepSig)
        Stack.drawStack('llnunu_l1_l1_electronrelIsoea03', cuts+"&&(abs(llnunu_l1_l1_pdgId)==11)", str(lumi*1000), 100, 0.0, 0.2, titlex = "looseISO_{rel}(e_{1})", units = "",output='ISOlep1_el',outDir=outdir,separateSignal=sepSig)
        Stack.drawStack('llnunu_l1_l2_pt', cuts+"&&(abs(llnunu_l1_l2_pdgId)==11)", str(lumi*1000), 100, 0.0, 500.0, titlex = "P_{T}(e_{2})", units = "GeV",output='pTlep2_el',outDir=outdir,separateSignal=sepSig)
        Stack.drawStack('llnunu_l1_l2_eta', cuts+"&&(abs(llnunu_l1_l2_pdgId)==11)", str(lumi*1000), 30, -3.0, 3.0, titlex = "#eta(e_{2})", units = "",output='etalep2_el',outDir=outdir,separateSignal=sepSig)
        Stack.drawStack('llnunu_l1_l2_phi', cuts+"&&(abs(llnunu_l1_l2_pdgId)==11)", str(lumi*1000), 32, -3.2, 3.2, titlex = "#phi(e_{2})", units = "",output='philep2_el',outDir=outdir,separateSignal=sepSig)
        Stack.drawStack('llnunu_l1_l2_electronrelIsoea03', cuts+"&&(abs(llnunu_l1_l2_pdgId)==11)", str(lumi*1000), 100, 0.0, 0.2, titlex = "looseISO_{rel}(e_{2})", units = "",output='ISOlep2_el',outDir=outdir,separateSignal=sepSig)


