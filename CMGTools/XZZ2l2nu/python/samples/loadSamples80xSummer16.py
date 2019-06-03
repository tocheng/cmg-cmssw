#####################
# load samples 
#####################

import PhysicsTools.HeppyCore.framework.config as cfg
import os

# Load backgrounds
from CMGTools.XZZ2l2nu.samples.samples_13TeV_RunIISummer16MiniAODv2 import *
# Load signals
from CMGTools.XZZ2l2nu.samples.samples_13TeV_signal80XSummer16 import *
# Load Data 
from CMGTools.XZZ2l2nu.samples.samples_13TeV_DATA2016 import *
# Load triggers
from CMGTools.XZZ2l2nu.samples.triggers_13TeV_Spring16 import *

# backgrounds
backgroundSamples = [
DYJetsToLL_M50_Ext,
DYJetsToLL_M50_MGMLM_Ext1,
WJetsToLNu,
TTTo2L2Nu_noSC,
TTTo2L2Nu_forTTH,
ZZTo2L2Nu,
ZZTo4L,
ZZTo2L2Q,
WWTo2L2Nu,
WWToLNuQQ,
WWToLNuQQ_Ext1,
WZTo1L1Nu2Q,
WZTo2L2Q,
WZTo3LNu,
TTZToLLNuNu,
TTWJetsToLNu_ext1,
TTWJetsToLNu_ext2,
ggZZTo2e2nu,
ggZZTo2mu2nu
]

resoSamples = [
ZZTo2L2Nu,
ZZTo4L,
ZZTo2L2Q,
WZTo2L2Q,
TTZToLLNuNu,
ggZZTo2e2nu,
ggZZTo2mu2nu,
WZTo3LNu
]

nonresoSamples = [
TTTo2L2Nu_forTTH,
WWTo2L2Nu,
WWToLNuQQ,
WZTo1L1Nu2Q,
WZTo3LNu,
TTWJetsToLNu_ext1,
TTWJetsToLNu_ext2
]

extraBackgroundMC = [
DY1JetsToLL_M50_MGMLM,
DY2JetsToLL_M50_MGMLM,
DY3JetsToLL_M50_MGMLM,
DY4JetsToLL_M50_MGMLM,
DYBJetsToLL_M50_MGMLM
]

DySamples = [
DYJetsToLL_Pt_0To50,
DYJetsToLL_Pt_50To100,
DYJetsToLL_Pt_100To250,
DYJetsToLL_Pt_250To400,
DYJetsToLL_Pt_400To650,
DYJetsToLL_Pt_650ToInf
]

DySamplesExt1 = [
DYJetsToLL_Pt_100To250_ext1_v1,
DYJetsToLL_Pt_250To400_ext1_v1,
DYJetsToLL_Pt_400To650_ext1_v1,
DYJetsToLL_Pt_650ToInf_ext1_v1
]

DySamplesExt2 = [
DYJetsToLL_Pt_100To250_ext2_v1,
DYJetsToLL_Pt_250To400_ext2_v1,
DYJetsToLL_Pt_400To650_ext2_v1,
DYJetsToLL_Pt_650ToInf_ext2_v1
]

DySamplesExt5 = [
DYJetsToLL_Pt_100To250_ext5_v1,
DYJetsToLL_Pt_250To400_ext5_v1
]

G_DYJetsDY=[DYJetsToLL_M50_Ext]
GJetsMC= WJetsToLNuHT + ZJetsToNuNuHT + SingleTop + ZNuNuGJets + tgjets+ttgjets + WGJets + G_DYJetsDY
#GJetsMC += GJetsHT
#GJetsMC += QCDPtEMEnriched

# signals
signalSamples = [
BulkGravToZZToZlepZinv_narrow_600,
BulkGravToZZToZlepZinv_narrow_800,
BulkGravToZZToZlepZinv_narrow_1000,
BulkGravToZZToZlepZinv_narrow_1200,
BulkGravToZZToZlepZinv_narrow_1400,
BulkGravToZZToZlepZinv_narrow_1600,
BulkGravToZZToZlepZinv_narrow_1800,
BulkGravToZZToZlepZinv_narrow_2000,
BulkGravToZZToZlepZinv_narrow_2500,
BulkGravToZZToZlepZinv_narrow_3000,
BulkGravToZZToZlepZinv_narrow_3500,
BulkGravToZZToZlepZinv_narrow_4000,
BulkGravToZZToZlepZinv_narrow_4500,
]

BulkGravToZZToZlepZhad = [
BulkGravToZZToZlepZhad_narrow_600,
BulkGravToZZToZlepZhad_narrow_800,
BulkGravToZZToZlepZhad_narrow_1000,
BulkGravToZZToZlepZhad_narrow_1200,
BulkGravToZZToZlepZhad_narrow_1400,
BulkGravToZZToZlepZhad_narrow_1600,
BulkGravToZZToZlepZhad_narrow_1800,
BulkGravToZZToZlepZhad_narrow_2000,
BulkGravToZZToZlepZhad_narrow_2500,
BulkGravToZZToZlepZhad_narrow_3000,
BulkGravToZZToZlepZhad_narrow_3500,
BulkGravToZZToZlepZhad_narrow_4000,
BulkGravToZZToZlepZhad_narrow_4500,
]

# MC samples
otherMcSamples = BulkGravToZZToZlepZhad + ggHZZ2L2Nu + Graviton2PBToZZTo2L2Nu+Graviton2PBqqbarToZZTo2L2Nu+VBF_HToZZTo2L2Nu 
otherMcSamples += QCDPtMuEnriched

mcSamples = signalSamples + backgroundSamples + extraBackgroundMC + GJetsMC + DySamples

# data
Data03Feb2017 = SingleElectron_03Feb2017+SingleMuon_03Feb2017
dataSamples=Data03Feb2017

otherDataSamples=SinglePhoton_03Feb2017+MuonEG_03Feb2017
allDataSamples=dataSamples+otherDataSamples

for s in allDataSamples:
    s.triggers = []
    s.vetoTriggers = []


# JSON
goldenJson = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/ReReco/Final/Cert_271036-284044_13TeV_23Sep2016ReReco_Collisions16_JSON.txt'
run_range = (271036,284044)
jsonFile = goldenJson

from CMGTools.XZZ2l2nu.setup.Efficiencies import *
dataDir = "$CMSSW_BASE/src/CMGTools/XZZ2l2nu/data"

for comp in mcSamples+otherMcSamples:
    comp.isMC = True
    comp.isData = False
    comp.splitFactor = 250
    comp.puFileMC=dataDir+"/pileup_MC_80x_271036-276811_68075.root"
    comp.puFileData=dataDir+"/pileup_DATA_80x_271036-276811_68075.root"
    #comp.eSFinput=dataDir+"/CutBasedID_LooseWP_76X_18Feb.txt_SF2D.root"
    comp.efficiency = eff2012
    #comp.triggers=triggers_1mu_noniso+triggers_1e_noniso
    comp.triggers= []
    comp.globalTag = "Summer15_25nsV6_MC"

for comp in allDataSamples:
    comp.splitFactor = 250
    comp.isMC = False
    comp.isData = True
    comp.json = jsonFile
    comp.run_range = run_range

