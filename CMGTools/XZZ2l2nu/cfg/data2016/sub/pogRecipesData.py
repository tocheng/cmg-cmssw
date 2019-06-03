import FWCore.ParameterSet.Config as cms

runOnData=True
usePrivateSQlite=False

process = cms.Process("NEW")

# Load the standard set of configuration modules
process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.GeometryDB_cff')
#process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

# Load for e/gamma regression
#process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')

# import of standard configurations
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.StandardSequences.MagneticField_cff')

# Message Logger settings
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.destinations = ['cout', 'cerr']
process.MessageLogger.cerr.FwkReport.reportEvery = 10000

# Set the process options -- Display summary at the end, enable unscheduled execution
process.options = cms.untracked.PSet( 
    allowUnscheduled = cms.untracked.bool(True),
    wantSummary = cms.untracked.bool(False) 
)

# How many events to process
process.maxEvents = cms.untracked.PSet( 
   input = cms.untracked.int32(-1)
)


process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
    )
)

from Configuration.AlCa.autoCond import autoCond
if runOnData:
  # Spring16_25nsV6_DATA_AK4PFchs
  process.GlobalTag.globaltag = '80X_dataRun2_2016SeptRepro_v7'
else:
  # Summer16_25nsV5_MC_AK4PFchs
  process.GlobalTag.globaltag = '80X_mcRun2_asymptotic_2016_TrancheIV_v6'

if usePrivateSQlite:
    from CondCore.DBCommon.CondDBSetup_cfi import *
    import os
    if runOnData:
      era="Summer16_23Sep2016AllV4_DATA"
    else:
      era="Summer16_23Sep2016V4_MC"

    process.jec = cms.ESSource("PoolDBESSource",CondDBSetup,
                               connect = cms.string( "frontier://FrontierProd/CMS_CONDITIONS"),
                               #connect = cms.string( "frontier://FrontierPrep/CMS_COND_PHYSICSTOOLS"),
                               #connect = cms.string('sqlite:'+era+'.db'),
                               toGet =  cms.VPSet(
            cms.PSet(
                record = cms.string("JetCorrectionsRecord"),
                tag = cms.string("JetCorrectorParametersCollection_"+era+"_AK4PF"),
                label= cms.untracked.string("AK4PF")
                ),
            cms.PSet(
                record = cms.string("JetCorrectionsRecord"),
                tag = cms.string("JetCorrectorParametersCollection_"+era+"_AK4PFchs"),
                label= cms.untracked.string("AK4PFchs")
                ),
            )
                               )
    process.es_prefer_jec = cms.ESPrefer("PoolDBESSource",'jec')


# e/gamma regression
#process.load('EgammaAnalysis.ElectronTools.regressionApplication_cff')
#process.EGMenergyCorrection = cms.Path(process.regressionApplication)

## jet recluster
from JMEAnalysis.JetToolbox.jetToolbox_cff import jetToolbox

# AK R=0.8 from PUPPI inputs with basic grooming, W tagging, and top tagging
jetToolbox( process, 'ak8', 'ak8JetSubs', 'OUT', miniAOD=True, runOnMC=False, PUMethod='Puppi', addPruning=True, addSoftDrop=True ,
# add Nsubjettiness tau1, tau2, tau3, tau4
addTrimming=True, addFiltering=True, addSoftDropSubjets=True, addPrunedSubjets=True, addNsub=True, maxTau=4,
# add btagging for 'fat' jet
bTagDiscriminators = ['pfBoostedDoubleSecondaryVertexAK8BJetTags','pfCombinedSecondaryVertexV2BJetTags','pfCombinedInclusiveSecondaryVertexV2BJetTags'],
# add JEC
JETCorrPayload = 'AK8PFPuppi', JETCorrLevels = ['L2Relative', 'L3Absolute'] )

process.OUT = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string('Loop/cmsswPreProcessing.root'),
    outputCommands = cms.untracked.vstring(
        'keep *_slimmedMETs_*_NEW',
        'keep *_TriggerResults_*_NEW',
        'keep *_BadChargedCandidateFilter_*_*',
        'keep *_BadPFMuonFilter_*_*',
        'keep *_packedPFCandidates*_*_*', 
        'keep *_selectedPatJetsAK8PFPuppi*_*_NEW',
        'keep *_packedPatJetsAK8PFPuppi*_*_NEW',
        'keep floatedmValueMap_ak8PFJetsPuppi*__NEW',
        'keep floatedmValueMap_NjettinessAK8Puppi_*_NEW',
        'keep floatedmValueMap_*_*_NEW',
        )
)


# met 
from PhysicsTools.PatUtils.tools.runMETCorrectionsAndUncertainties import runMetCorAndUncFromMiniAOD

# If you only want to re-correct and get the proper uncertainties
runMetCorAndUncFromMiniAOD(process,
                           isData=runOnData,
                           )

# If you would like to re-cluster and get the proper uncertainties
#runMetCorAndUncFromMiniAOD(process,
#                           isData=False,
#                           pfCandColl=cms.InputTag("packedPFCandidates"),
#                           recoMetFromPFCs=True,
#                           )
#process.p = cms.Path(process.fullPatMetSequence)


# met filters
process.load('RecoMET.METFilters.BadPFMuonFilter_cfi')
process.BadPFMuonFilter.muons = cms.InputTag("slimmedMuons")
process.BadPFMuonFilter.PFCandidates = cms.InputTag("packedPFCandidates")

process.load('RecoMET.METFilters.BadChargedCandidateFilter_cfi')
process.BadChargedCandidateFilter.muons = cms.InputTag("slimmedMuons")
process.BadChargedCandidateFilter.PFCandidates = cms.InputTag("packedPFCandidates")

process.endpath= cms.EndPath(process.BadPFMuonFilter * process.BadChargedCandidateFilter * process.OUT)
