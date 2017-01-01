import FWCore.ParameterSet.Config as cms

process = cms.Process('GETGBR')

process.load('Configuration.StandardSequences.Services_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1)
)

# Input source
process.source = cms.Source("PoolSource",
    #fileNames = cms.untracked.vstring('/store/data/Run2015D/DoubleEG/MINIAOD/PromptReco-v4/000/258/159/00000/027612B0-306C-E511-BD47-02163E014496.root'),
    fileNames = cms.untracked.vstring('/store/data/Run2016D/SingleMuon/MINIAOD/PromptReco-v2/000/276/315/00000/168C3DE5-F444-E611-A012-02163E014230.root'),
    #fileNames = cms.untracked.vstring('/store/data/Run2016C/SingleMuon/MINIAOD/PromptReco-v2/000/275/658/00000/04263529-8B3B-E611-99AE-02163E011918.root'),
    #fileNames = cms.untracked.vstring('/store/mc/RunIISpring16MiniAODv2/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/MINIAODSIM/PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1/50000/00CFA37F-9B2A-E611-8F6B-24BE05C62711.root'),
)

from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
#process.GlobalTag = GlobalTag(process.GlobalTag, '74X_dataRun2_Prompt_v4', '')
process.GlobalTag = GlobalTag(process.GlobalTag, '80X_dataRun2_Prompt_ICHEP16JEC_v0', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, '80X_dataRun2_Prompt_v10', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, '80X_mcRun2_asymptotic_2016_miniAODv2_v1', '')
# https://cms-conddb.cern.ch/cmsDbBrowser/diff/Prod/gts/80X_dataRun2_Prompt_ICHEP16JEC_v0/80X_dataRun2_Prompt_v10

process.getGBR25ns = cms.EDAnalyzer("GBRForestGetterFromDB",
    grbForestName = cms.string("gedelectron_p4combination_25ns"),
    outputFileName = cms.untracked.string("GBRForest_data_25ns.root"),
)
    
process.getGBR50ns = cms.EDAnalyzer("GBRForestGetterFromDB",
    grbForestName = cms.string("gedelectron_p4combination_50ns"),
    outputFileName = cms.untracked.string("GBRForest_data_50ns.root"),
)

process.path = cms.Path(
    process.getGBR25ns +
    process.getGBR50ns
)
