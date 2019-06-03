#!/usr/bin/env python
import math,sys
import ROOT
import os, string, math, pickle
from CMGTools.XZZ2l2nu.plotting.TreePlotter import TreePlotter
from CMGTools.XZZ2l2nu.plotting.MergedPlotter import MergedPlotter
from CMGTools.XZZ2l2nu.plotting.StackPlotter import StackPlotter
ROOT.gROOT.SetBatch()
if len(sys.argv)!=2:
    print "./muonploting.py tag\ntag: zveto/full/metzpt50/metzpt100"
    sys.exit()
tag="test_{0}_".format(sys.argv[1])
channel='mu'
LogY=("_log_" in tag)
Blind=("_blind_" in tag)
test=True
DrawLeptons=True
doRhoScale=True

if test: DrawLeptons = False

lepsf="trgsf*isosf*idsf"

if doRhoScale: 
    tag+="RhoWt_"
    lepsf=lepsf+"*(0.32+0.42*TMath::Erf((rho-4.16)/4.58)+0.31*TMath::Erf((rho+115.00)/29.58))" # b2h rereco 36.1 fb-1
#    lepsf=lepsf+"*(0.807+0.007*nVert+-3.689e-05*nVert*nVert+6.730e-04*exp(2.500e-01*nVert))" # b2h rereco 33.59fb-1

outdir='plots'

indir='/home/heli/XZZ/80X_20161029_light_Skim'

lumi=36.4592
sepSig=True
doRatio=True
FakeData=False
UseMETFilter=True

puWeight='puWeightmoriondMC'

ZJetsZPtWeight=True
DataHLT=True
k=1 # signal scale

elChannel='((abs(llnunu_l1_l1_pdgId)==11||abs(llnunu_l1_l2_pdgId)==11)&&llnunu_l1_l1_pt>115&&abs(llnunu_l1_l1_eta)<2.5&&llnunu_l1_l2_pt>35&&abs(llnunu_l1_l2_eta)<2.5)'
muChannel='((abs(llnunu_l1_l1_pdgId)==13||abs(llnunu_l1_l2_pdgId)==13)&&llnunu_l1_l1_pt>50&&abs(llnunu_l1_l1_eta)<2.4&&llnunu_l1_l2_pt>20&&abs(llnunu_l1_l2_eta)<2.4&&(llnunu_l1_l1_highPtID>0.99||llnunu_l1_l2_highPtID>0.99))'

if not os.path.exists(outdir): os.system('mkdir '+outdir)

tag = tag+puWeight+'_'


if UseMETFilter: tag = tag+'metfilter_'

if not Blind: tag = tag+'unblind_'

tag = tag+channel+'_'

paveText="#sqrt{s} = 13 TeV 2016 L = "+"{:.3}".format(float(lumi))+" fb^{-1}"

metfilter='(Flag_EcalDeadCellTriggerPrimitiveFilter&&Flag_HBHENoiseIsoFilter&&Flag_goodVertices&&Flag_HBHENoiseFilter&&Flag_globalTightHalo2016Filter&&Flag_eeBadScFilter)'

#cuts_loose='(nllnunu)'
#cuts_lepaccept="((abs(llnunu_l1_l1_pdgId)==13abs(llnunu_l1_l2_pdgId)==13&&llnunu_l1_l1_pt>50&&abs(llnunu_l1_l1_eta)<2.4&&llnunu_l1_l2_pt>20&&abs(llnunu_l1_l2_eta)<2.4&&(llnunu_l1_l1_highPtID>0.99||llnunu_l1_l2_highPtID>0.99))"
#cuts_lepaccept+="||(abs(llnunu_l1_l1_pdgId)==11&&abs(llnunu_l1_l2_pdgId)==11&&llnunu_l1_l1_pt>115&&abs(llnunu_l1_l1_eta)<2.5&&llnunu_l1_l2_pt>35&&abs(llnunu_l1_l2_eta)<2.5))"
cuts_zmass="(llnunu_l1_mass>50&&llnunu_l1_mass<180)&&!(llnunu_l1_mass>70&&llnunu_l1_mass<110)"
cuts_zmassin="(llnunu_l1_mass>50&&llnunu_l1_mass<180)"
cuts_zpt100="(llnunu_l1_pt>100)"
cuts_met50="(llnunu_l2_pt>50)"
cuts_met100="(llnunu_l2_pt>100)"
cuts_met200="(llnunu_l2_pt>200)"
#cuts_loose_z="("+cuts_lepaccept+"&&"+cuts_zmass+")"
#cuts_loose_zll="("+cuts_lepaccept+"&&"+cuts_zmass+"&&"+cuts_zpt100+")"
#cuts_loose_zll_met50="("+cuts_lepaccept+"&&"+cuts_zmass+"&&"+cuts_zpt100+"&&"+cuts_met50+")"
#cuts_loose_zll_met100="("+cuts_lepaccept+"&&"+cuts_zmass+"&&"+cuts_zpt100+"&&"+cuts_met100+")"
#cuts_loose_zll_met200="("+cuts_lepaccept+"&&"+cuts_zmass+"&&"+cuts_zpt100+"&&"+cuts_met200+")"
cuts=''
if "metzpt30" in tag:cuts=cuts_zmass+'&&(llnunu_l1_pt>30)&&(llnunu_l2_pt>30)'
elif "metzptCR" in tag:cuts=cuts_zmass+'&&(llnunu_l1_pt>70)&&(llnunu_l2_pt>70)'
elif "metzpt100" in tag:cuts=cuts_zmass+'&&(llnunu_l1_pt>100)&&(llnunu_l2_pt>100)'
elif "zveto" in tag:cuts=cuts_zmass
elif "full" in tag:cuts=cuts_zmassin
elif "sigall" in tag: cuts="llnunu_l1_mass>70&&llnunu_l1_mass<110"
elif "sigzpt100" in tag: cuts="llnunu_l1_mass>70&&llnunu_l1_mass<110&&(llnunu_l1_pt>100)"
elif "sigpt100" in tag: cuts="llnunu_l1_mass>70&&llnunu_l1_mass<110&&(llnunu_l1_pt>100)&&(llnunu_l2_pt>100)"
else:
    raise RuntimeError, "tag not defined"
if channel=='el': cuts = cuts+'&&'+elChannel
elif channel=='mu': cuts = cuts+'&&'+muChannel
else: cuts = cuts+'&&({0}||{1})'.format(elChannel,muChannel)
if UseMETFilter:
    cuts = '('+cuts+')'

ROOT.gROOT.ProcessLine('.x tdrstyle.C') 

allPlotters = {}

vvPlotters=[]
vvSamples = ['WZTo2L2Q','WZTo3LNu_AMCNLO',
'ZZTo2L2Nu',
'ZZTo2L2Q','ZZTo4L',
'ggZZTo2e2nu','ggZZTo2mu2nu']
for sample in vvSamples:
    vvPlotters.append(TreePlotter(sample, indir+'/'+sample+'.root','tree'))
    vvPlotters[-1].addCorrectionFactor('1/SumWeights','norm')
    #vvPlotters[-1].addCorrectionFactor('xsec','xsec')
    vvPlotters[-1].addCorrectionFactor('genWeight','genWeight')
    vvPlotters[-1].addCorrectionFactor(puWeight,'puWeight')
    vvPlotters[-1].addCorrectionFactor(lepsf, 'lepsf')
    if sample == 'ZZTo2L2Nu' : vvPlotters[-1].addCorrectionFactor("(ZZEwkCorrWeight*ZZQcdCorrWeight)", 'nnlo')
    if 'ggZZTo2' in sample: vvPlotters[-1].addCorrectionFactor('0.01898','xsec') 
    else: vvPlotters[-1].addCorrectionFactor('xsec','xsec')
    allPlotters[sample] = vvPlotters[-1]
VV = MergedPlotter(vvPlotters)
VV.setFillProperties(1001,ROOT.kMagenta)

nonresPlotters=[]
nonresSamples = ['muonegtrgsf']
for sample in nonresSamples:
    nonresPlotters.append(TreePlotter(sample, indir+'/'+sample+'.root','tree'))
    nonresPlotters[-1].addCorrectionFactor('mtrgsf', 'mtrgsf')
#    nonresPlotters[-1].addCorrectionFactor('0.625551077292', 'norm')
    nonresPlotters[-1].addCorrectionFactor('0.602260745361', 'norm')
    nonresPlotters[-1].addCorrectionFactor('1./({0}*1000)'.format(lumi), 'lumi')
    allPlotters[sample] = nonresPlotters[-1]
NONRES = MergedPlotter(nonresPlotters)
NONRES.setFillProperties(1001,ROOT.kOrange)


zjetsPlotters=[]
zjetsSamples = ['DYJetsToLL_M50_BIG_ResBos_Rc36p22']
for sample in zjetsSamples:
    zjetsPlotters.append(TreePlotter(sample, indir+'/'+sample+'.root','tree'))
    zjetsPlotters[-1].addCorrectionFactor('1./SumWeights','norm')
    zjetsPlotters[-1].addCorrectionFactor('(1.16158)','norm')
    if ZJetsZPtWeight: zjetsPlotters[-1].addCorrectionFactor('ZPtWeight','ZPtWeight')
    #zjetsPlotters[-1].addCorrectionFactor('PhiStarWeight','PhiStarWeight')
    #zjetsPlotters[-1].addCorrectionFactor('xsec','xsec')
    zjetsPlotters[-1].addCorrectionFactor('(1921.8*3)','xsec') # FEWZ NNLO.results_z_m50_nnlo_inclusive_NNPDF30_nlo_as_0118
    #zjetsPlotters[-1].addCorrectionFactor('(1907.0*3)','xsec') # FEWZ NNLO.results_z_m50_nnlo_fsrOn_lowstat_inclusive_NNPDF30_nlo_as_0118
    zjetsPlotters[-1].addCorrectionFactor('genWeight','genWeight')
    #zjetsPlotters[-1].addCorrectionFactor('ZJetsGenWeight','genWeight')
    zjetsPlotters[-1].addCorrectionFactor(puWeight,'puWeight')
    zjetsPlotters[-1].addCorrectionFactor(lepsf,'lepsf')
    allPlotters[sample] = zjetsPlotters[-1]
ZJets = MergedPlotter(zjetsPlotters)
ZJets.setFillProperties(1001,ROOT.kGreen+2)

dataPlotters=[]
dataSamples = [
'SingleEMU_Run2016B2H_ReReco_36p46',
]
for sample in dataSamples:
    dataPlotters.append(TreePlotter(sample, indir+'/'+sample+'.root','tree'))

if DataHLT:
    dataPlotters[0].addCorrectionFactor('(HLT_MUv2||HLT_ELEv2)','HLT')


Data = MergedPlotter(dataPlotters)




Stack = StackPlotter(outTag=tag, outDir=outdir)
Stack.setPaveText(paveText)
Stack.addPlotter(Data, "data_obs", "Data", "data")
#Stack.addPlotter(WJets, "WJets","W+Jets", "background")
Stack.addPlotter(NONRES, "NONReso","non reson.", "background")
Stack.addPlotter(VV, "VVZReso","ZZ WZ reson.", "background")
#Stack.addPlotter(ggZZ, "ggZZ","ggZZ", "background")
Stack.addPlotter(ZJets, "ZJets","ZJets", "background")

Stack.setLog(LogY)
Stack.doRatio(doRatio)



tag+='_'
print cuts
if test: 
    Stack.drawStack('llnunu_l1_mass', cuts, str(lumi*1000), 65, 50, 180, titlex = "M(Z)", units = "GeV",output=tag+'zmass',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('llnunu_l1_pt', cuts, str(lumi*1000), 100, 0.0, 500.0, titlex = "P_{T}(Z)", units = "GeV",output=tag+'zpt_low',outDir=outdir,separateSignal=sepSig)
    #Stack.drawStack('llnunu_l1_l1_pt', cuts, str(lumi*1000), 200, 0.0, 1000.0, titlex = "P_{T}(l_{1})", units = "GeV",output=tag+'pTlep1',outDir=outdir,separateSignal=sepSig)
    #Stack.drawStack('llnunu_l1_l2_pt', cuts, str(lumi*1000), 200, 0.0, 500.0, titlex = "P_{T}(l_{2})", units = "GeV",output=tag+'pTlep2',outDir=outdir,separateSignal=sepSig)
    #Stack.drawStack('llnunu_l1_l1_trackerIso', cuts+"&&(abs(llnunu_l1_l1_pdgId)==13)", str(lumi*1000), 100, 0.0, 0.2, titlex = "trackerISO_{rel}(#mu_{1})", units = "",output=tag+'ISOlep1_mu',outDir=outdir,separateSignal=sepSig)
    #Stack.drawStack('llnunu_l1_l2_trackerIso', cuts+"&&(abs(llnunu_l1_l2_pdgId)==13)", str(lumi*1000), 100, 0.0, 0.2, titlex = "trackerISO_{rel}(#mu_{2})", units = "",output=tag+'ISOlep2_mu',outDir=outdir,separateSignal=sepSig)
    #Stack.drawStack('abs(llnunu_l1_pt+llnunu_l2_pt*cos(llnunu_l2_phi-llnunu_l1_phi))/llnunu_l1_pt', cuts, str(lumi*1000), 100, 0, 5, titlex = "#Delta P_{T}^{#parallel}(Z,MET)/P_{T}(Z)", units = "",output=tag+'dPTParaRel',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('llnunu_mt', cuts, str(lumi*1000), 100, 0.0, 500.0, titlex = "M_{T}", units = "GeV",output=tag+'mt_high3',outDir=outdir,separateSignal=sepSig,blinding=Blind,blindingCut=300)
    Stack.drawStack('llnunu_l2_pt', cuts, str(lumi*1000), 100, 0, 300, titlex = "MET", units = "GeV",output=tag+'met_low2',outDir=outdir,separateSignal=sepSig,blinding=Blind,blindingCut=200)
    #Stack.drawStack('llnunu_l2_pt*cos(llnunu_l2_phi-llnunu_l1_phi)', cuts, str(lumi*1000), 100, -200, 200.0, titlex = "MET_{#parallel}", units = "GeV",output=tag+'met_para',outDir=outdir,separateSignal=sepSig)
    #Stack.drawStack('llnunu_l2_pt*sin(llnunu_l2_phi-llnunu_l1_phi)', cuts, str(lumi*1000), 100, -200, 200.0, titlex = "MET_{#perp}", units = "GeV",output=tag+'met_perp',outDir=outdir,separateSignal=sepSig)
else: 
    Stack.drawStack('llnunu_mt', cuts, str(lumi*1000), 300, 0.0, 3000.0, titlex = "M_{T}", units = "GeV",output=tag+'mt_high3',outDir=outdir,separateSignal=sepSig,blinding=Blind,blindingCut=300)
    Stack.drawStack('llnunu_mt', cuts, str(lumi*1000), 240, 0.0, 1200.0, titlex = "M_{T}", units = "GeV",output=tag+'mt',outDir=outdir,separateSignal=sepSig,blinding=Blind,blindingCut=300)
    Stack.drawStack('llnunu_mt', cuts, str(lumi*1000), 120, 0.0, 600.0, titlex = "M_{T}", units = "GeV",output=tag+'mt_low',outDir=outdir,separateSignal=sepSig,blinding=Blind,blindingCut=300)
    Stack.drawStack('llnunu_mt', cuts, str(lumi*1000), 60, 0.0, 300.0, titlex = "M_{T}", units = "GeV",output=tag+'mt_low2',outDir=outdir,separateSignal=sepSig,blinding=Blind,blindingCut=300)
    Stack.drawStack('llnunu_l2_pt', cuts, str(lumi*1000), 100, 0, 1000, titlex = "MET", units = "GeV",output=tag+'met',outDir=outdir,separateSignal=sepSig,blinding=Blind,blindingCut=200)
    Stack.drawStack('llnunu_l2_pt', cuts, str(lumi*1000), 50, 0, 500, titlex = "MET", units = "GeV",output=tag+'met_low',outDir=outdir,separateSignal=sepSig,blinding=Blind,blindingCut=200)
    Stack.drawStack('llnunu_l2_pt', cuts, str(lumi*1000), 100, 0, 300, titlex = "MET", units = "GeV",output=tag+'met_low2',outDir=outdir,separateSignal=sepSig,blinding=Blind,blindingCut=200)
    Stack.drawStack('llnunu_l1_mass', cuts, str(lumi*1000), 50, 50, 150, titlex = "M(Z)", units = "GeV",output=tag+'zmass',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('llnunu_l1_pt', cuts, str(lumi*1000), 75, 0.0, 1500.0, titlex = "P_{T}(Z)", units = "GeV",output=tag+'zpt',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('llnunu_l1_pt', cuts, str(lumi*1000), 100, 0.0, 500.0, titlex = "P_{T}(Z)", units = "GeV",output=tag+'zpt_low',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('llnunu_l1_pt', cuts, str(lumi*1000), 100, 0.0, 300.0, titlex = "P_{T}(Z)", units = "GeV",output=tag+'zpt_low2',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('TMath::Tan((TMath::Pi()-TMath::Abs(llnunu_l1_deltaPhi))/2.0)*TMath::Sin(TMath::ACos(TMath::TanH((llnunu_l1_l1_eta-llnunu_l1_l2_eta)/2.0)))', cuts, str(lumi*1000), 100, 0.0, 10, titlex = "#phi_{#eta}*", units = "",output=tag+'PhiStar',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('nVert', cuts, str(lumi*1000), 50, 0.0, 50.0, titlex = "N vertices", units = "",output=tag+'nVert',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('rho', cuts, str(lumi*1000), 50, 0.0, 50.0, titlex = "#rho", units = "",output=tag+'rho',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('llnunu_l2_pt*cos(llnunu_l2_phi-llnunu_l1_phi)', cuts, str(lumi*1000), 100, -200, 200.0, titlex = "MET_{#parallel}", units = "GeV",output=tag+'met_para',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('llnunu_l2_pt*sin(llnunu_l2_phi-llnunu_l1_phi)', cuts, str(lumi*1000), 100, -200, 200.0, titlex = "MET_{#perp}", units = "GeV",output=tag+'met_perp',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('llnunu_l2_pt/sqrt(llnunu_l2_sumEt)', cuts, str(lumi*1000), 100, 0.0, 20.0, titlex = "MET/#sqrt{sumE_{T}}", units = "",output=tag+'metOvSqSET',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('fabs(TVector2::Phi_mpi_pi(llnunu_l2_phi-llnunu_l1_phi))', cuts, str(lumi*1000), 50, 0, 5, titlex = "#Delta#phi(Z,MET)", units = "",output=tag+'dphiZMet',outDir=outdir,separateSignal=sepSig)
    #Stack.drawStack('cos(llnunu_l2_phi-llnunu_l1_phi)', cuts, str(lumi*1000), 50, -1, 1, titlex = "cos(#Delta#phi(Z,MET))", units = "",output=tag+'CosdphiZMet',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('fabs(llnunu_l1_pt+llnunu_l2_pt*cos(llnunu_l2_phi-llnunu_l1_phi))', cuts, str(lumi*1000), 100, 0, 1000, titlex = "#Delta P_{T}^{#parallel}(Z,MET)", units = "GeV",output=tag+'dPTPara',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('fabs(llnunu_l2_pt*sin(llnunu_l2_phi-llnunu_l1_phi))', cuts, str(lumi*1000), 100, 0, 100, titlex = "#Delta P_{T}^{#perp}(Z,MET)", units = "GeV",output=tag+'dPTPerp',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('fabs(llnunu_l1_pt+llnunu_l2_pt*cos(llnunu_l2_phi-llnunu_l1_phi))/llnunu_l1_pt', cuts, str(lumi*1000), 100, 0, 5, titlex = "#Delta P_{T}^{#parallel}(Z,MET)/P_{T}(Z)", units = "",output=tag+'dPTParaRel',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('fabs(llnunu_l2_pt*sin(llnunu_l2_phi-llnunu_l1_phi))/llnunu_l1_pt', cuts, str(lumi*1000), 100, 0, 5, titlex = "#Delta P_{T}^{#perp}(Z,MET)/P_{T}", units = "",output=tag+'dPTPerpRel',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('llnunu_l1_eta', cuts, str(lumi*1000), 200, -10.0, 10.0, titlex = "#eta(Z) ", units = "",output=tag+'zeta',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('llnunu_l1_phi', cuts, str(lumi*1000), 64, -3.2, 3.2, titlex = "#phi(Z)", units = "",output=tag+'zphi',outDir=outdir,separateSignal=sepSig)
    #Stack.drawStack('llnunu_l1_mt', cuts, str(lumi*1000), 50, 0.0, 150.0, titlex = "M_{T}(Z)", units = "GeV",output=tag+'zmt',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('llnunu_l1_deltaPhi', cuts, str(lumi*1000), 50, 0.0, 5.0, titlex = "#Delta#phi", units = "",output=tag+'ZdeltaPhi',outDir=outdir,separateSignal=sepSig)
    #Stack.drawStack('cos(llnunu_l1_deltaPhi)', cuts, str(lumi*1000), 50, -1, 1, titlex = "Cos(#Delta#phi)", units = "",output=tag+'CosZdeltaPhi',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('llnunu_l1_deltaR', cuts, str(lumi*1000), 50, 0.0, 5.0, titlex = "#Delta R", units = "",output=tag+'ZdeltaR',outDir=outdir,separateSignal=sepSig)
  
    Stack.drawStack('llnunu_l2_phi', cuts, str(lumi*1000), 100, -3.2, 3.2, titlex = "#phi(MET)", units = "",output=tag+'metPhi',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('llnunu_l2_sumEt', cuts, str(lumi*1000), 80, 0.0, 3000.0, titlex = "sumE_{T}", units = "GeV",output=tag+'metSumEt',outDir=outdir,separateSignal=sepSig)

    if channel!='el' and channel!='mu' :
        Stack.drawStack('llnunu_l1_l1_pt', cuts, str(lumi*1000), 200, 0.0, 1000.0, titlex = "P_{T}(l_{1})", units = "GeV",output=tag+'pTlep1',outDir=outdir,separateSignal=sepSig)
        Stack.drawStack('llnunu_l1_l1_eta', cuts, str(lumi*1000), 60, -3.0, 3.0, titlex = "#eta(l_{1})", units = "",output=tag+'etalep1',outDir=outdir,separateSignal=sepSig)
        Stack.drawStack('llnunu_l1_l1_phi', cuts, str(lumi*1000), 64, -3.2, 3.2, titlex = "#phi(l_{1})", units = "",output=tag+'philep1',outDir=outdir,separateSignal=sepSig)
        Stack.drawStack('llnunu_l1_l2_pt', cuts, str(lumi*1000), 200, 0.0, 500.0, titlex = "P_{T}(l_{2})", units = "GeV",output=tag+'pTlep2',outDir=outdir,separateSignal=sepSig)
        Stack.drawStack('llnunu_l1_l2_eta', cuts, str(lumi*1000), 60, -3.0, 3.0, titlex = "#eta(l_{2})", units = "",output=tag+'etalep2',outDir=outdir,separateSignal=sepSig)
        Stack.drawStack('llnunu_l1_l2_phi', cuts, str(lumi*1000), 64, -3.2, 3.2, titlex = "#phi(l_{2})", units = "",output=tag+'philep2',outDir=outdir,separateSignal=sepSig)

if DrawLeptons:

    if channel!='el' :
        Stack.drawStack('llnunu_l1_l1_pt', cuts+"&&(abs(llnunu_l1_l1_pdgId)==13)", str(lumi*1000), 100, 0.0, 1000.0, titlex = "P_{T}(#mu_{1})", units = "GeV",output=tag+'pTlep1_mu',outDir=outdir,separateSignal=sepSig)
        Stack.drawStack('llnunu_l1_l1_eta', cuts+"&&(abs(llnunu_l1_l1_pdgId)==13)", str(lumi*1000), 30, -3.0, 3.0, titlex = "#eta(#mu_{1})", units = "",output=tag+'etalep1_mu',outDir=outdir,separateSignal=sepSig)
        Stack.drawStack('llnunu_l1_l1_phi', cuts+"&&(abs(llnunu_l1_l1_pdgId)==13)", str(lumi*1000), 32, -3.2, 3.2, titlex = "#phi(#mu_{1})", units = "",output=tag+'philep1_mu',outDir=outdir,separateSignal=sepSig)
        Stack.drawStack('llnunu_l1_l1_trackerIso', cuts+"&&(abs(llnunu_l1_l1_pdgId)==13)", str(lumi*1000), 100, 0.0, 0.2, titlex = "trackerISO_{rel}(#mu_{1})", units = "",output=tag+'ISOlep1_mu',outDir=outdir,separateSignal=sepSig)

        Stack.drawStack('llnunu_l1_l2_pt', cuts+"&&(abs(llnunu_l1_l2_pdgId)==13)", str(lumi*1000), 100, 0.0, 500.0, titlex = "P_{T}(#mu_{2})", units = "GeV",output=tag+'pTlep2_mu',outDir=outdir,separateSignal=sepSig)
        Stack.drawStack('llnunu_l1_l2_eta', cuts+"&&(abs(llnunu_l1_l2_pdgId)==13)", str(lumi*1000), 30, -3.0, 3.0, titlex = "#eta(#mu_{2})", units = "",output=tag+'etalep2_mu',outDir=outdir,separateSignal=sepSig)
        Stack.drawStack('llnunu_l1_l2_phi', cuts+"&&(abs(llnunu_l1_l2_pdgId)==13)", str(lumi*1000), 32, -3.2, 3.2, titlex = "#phi(#mu_{2})", units = "",output=tag+'philep2_mu',outDir=outdir,separateSignal=sepSig)
        Stack.drawStack('llnunu_l1_l2_trackerIso', cuts+"&&(abs(llnunu_l1_l2_pdgId)==13)", str(lumi*1000), 100, 0.0, 0.2, titlex = "trackerISO_{rel}(#mu_{2})", units = "",output=tag+'ISOlep2_mu',outDir=outdir,separateSignal=sepSig)

    if channel!='mu' : 
        Stack.drawStack('llnunu_l1_l1_pt', cuts+"&&(abs(llnunu_l1_l1_pdgId)==11)", str(lumi*1000), 100, 0.0, 1000.0, titlex = "P_{T}(e_{1})", units = "GeV",output=tag+'pTlep1_el',outDir=outdir,separateSignal=sepSig)
        Stack.drawStack('llnunu_l1_l1_eta', cuts+"&&(abs(llnunu_l1_l1_pdgId)==11)", str(lumi*1000), 30, -3.0, 3.0, titlex = "#eta(e_{1})", units = "",output=tag+'etalep1_el',outDir=outdir,separateSignal=sepSig)
        Stack.drawStack('llnunu_l1_l1_phi', cuts+"&&(abs(llnunu_l1_l1_pdgId)==11)", str(lumi*1000), 32, -3.2, 3.2, titlex = "#phi(e_{1})", units = "",output=tag+'philep1_el',outDir=outdir,separateSignal=sepSig)
        Stack.drawStack('llnunu_l1_l1_electronrelIsoea03', cuts+"&&(abs(llnunu_l1_l1_pdgId)==11)", str(lumi*1000), 100, 0.0, 0.2, titlex = "looseISO_{rel}(e_{1})", units = "",output=tag+'ISOlep1_el',outDir=outdir,separateSignal=sepSig)

        Stack.drawStack('llnunu_l1_l2_pt', cuts+"&&(abs(llnunu_l1_l2_pdgId)==11)", str(lumi*1000), 100, 0.0, 500.0, titlex = "P_{T}(e_{2})", units = "GeV",output=tag+'pTlep2_el',outDir=outdir,separateSignal=sepSig)
        Stack.drawStack('llnunu_l1_l2_eta', cuts+"&&(abs(llnunu_l1_l2_pdgId)==11)", str(lumi*1000), 30, -3.0, 3.0, titlex = "#eta(e_{2})", units = "",output=tag+'etalep2_el',outDir=outdir,separateSignal=sepSig)
        Stack.drawStack('llnunu_l1_l2_phi', cuts+"&&(abs(llnunu_l1_l2_pdgId)==11)", str(lumi*1000), 32, -3.2, 3.2, titlex = "#phi(e_{2})", units = "",output=tag+'philep2_el',outDir=outdir,separateSignal=sepSig)
        Stack.drawStack('llnunu_l1_l2_electronrelIsoea03', cuts+"&&(abs(llnunu_l1_l2_pdgId)==11)", str(lumi*1000), 100, 0.0, 0.2, titlex = "looseISO_{rel}(e_{2})", units = "",output=tag+'ISOlep2_el',outDir=outdir,separateSignal=sepSig)


Stack.closePSFile()


