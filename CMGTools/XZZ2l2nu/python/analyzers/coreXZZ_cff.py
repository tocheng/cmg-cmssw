import os
import PhysicsTools.HeppyCore.framework.config as cfg
from PhysicsTools.Heppy.analyzers.core.all import * # SkimAnalyzerCount
from PhysicsTools.Heppy.analyzers.objects.all import *  
from PhysicsTools.Heppy.analyzers.gen.all import *
from PhysicsTools.HeppyCore.utils.deltar import *
from CMGTools.XZZ2l2nu.analyzers.Skimmer import *
from CMGTools.XZZ2l2nu.analyzers.XZZLeptonicVMaker import *
from CMGTools.XZZ2l2nu.analyzers.PackedCandidateLoader import *
from CMGTools.XZZ2l2nu.analyzers.XZZMultiFinalState  import *
from CMGTools.XZZ2l2nu.analyzers.XZZMultTrgEff import *
from CMGTools.XZZ2l2nu.tools.leptonID  import *
from CMGTools.XZZ2l2nu.analyzers.XZZGenAnalyzer import *
from CMGTools.XZZ2l2nu.analyzers.XZZLeptonAnalyzer import *
from CMGTools.XZZ2l2nu.analyzers.XZZTriggerBitFilter import *
from CMGTools.XZZ2l2nu.analyzers.XZZVertexAnalyzer import *
from CMGTools.XZZ2l2nu.analyzers.XZZMETAnalyzer import *
from CMGTools.XZZ2l2nu.analyzers.XZZDumpEvtList import *

from PhysicsTools.Heppy.analyzers.objects.JetAnalyzer import *
#from CMGTools.XZZ2l2nu.analyzers.XZZJetAnalyzer import *
from CMGTools.XZZ2l2nu.analyzers.XZZLHEWeightAnalyzer import *
from CMGTools.XZZ2l2nu.analyzers.XZZPhotonAnalyzer import *
from CMGTools.XZZ2l2nu.analyzers.XZZBoostJetAnalyzer import *

###########################
# define analyzers
###########################

skimAnalyzer = cfg.Analyzer(
    SkimAnalyzerCount, name='skimAnalyzerCount',
    useLumiBlocks = False,
    )

# Apply json file (if the dataset has one)
jsonAna = cfg.Analyzer(
    JSONAnalyzer, name="JSONAnalyzer",
    )

# Filter using the 'triggers' and 'vetoTriggers' specified in the dataset
triggerAna = cfg.Analyzer(
    XZZTriggerBitFilter, name="TriggerBitFilter",
    )

# This analyzer actually does the pile-up reweighting (generic)
pileUpAna = cfg.Analyzer(
    PileUpAnalyzer, name="PileUpAnalyzer",
    true = True,  # use number of true interactions for reweighting
    makeHists=False
    )


## Gen Info Analyzer 
genAna = cfg.Analyzer(
    XZZGenAnalyzer, name="XZZGenAnalyzer",
    # Print out debug information
    verbose = True,
    filter = "None",
    )

# Gen Info Analyzer (generic, but should be revised)
genAnaGeneric = cfg.Analyzer(
    GeneratorAnalyzer, name="GeneratorAnalyzerGeneric",
    # BSM particles that can appear with status <= 2 and should be kept
    stableBSMParticleIds = [ 1000022 ],
    # Particles of which we want to save the pre-FSR momentum (a la status 3).
    # Note that for quarks and gluons the post-FSR doesn't make sense,
    # so those should always be in the list
    savePreFSRParticleIds = [ 1,2,3,4,5, 11,12,13,14,15,16, 21 ],
    # Make also the list of all genParticles, for other analyzers to handle
    makeAllGenParticles = True,
    # Make also the splitted lists
    makeSplittedGenLists = True,
    allGenTaus = False,
    # Print out debug information
    verbose = True,
    )

## LHEWeightsAnalyzer
lheWeightAna = cfg.Analyzer(
    XZZLHEWeightAnalyzer, name="LHEWeightAnalyzer",
)

# Select a list of good primary vertices (generic)
vertexAna = cfg.Analyzer(
    XZZVertexAnalyzer, name="VertexAnalyzer",
    allVertices = "offlineSlimmedPrimaryVertices",
    vertexWeight = None,
    fixedWeight = 1,
    verbose = False
    )

lepAna = cfg.Analyzer(
    XZZLeptonAnalyzer, name="leptonAnalyzer",
    muons='slimmedMuons',
    electrons='slimmedElectrons',
    packedCandidates = 'packedPFCandidates',
    muonUseTuneP = True,
    rhoMuon= 'fixedGridRhoFastjetCentralNeutral',
    rhoElectronMiniIso = 'fixedGridRhoFastjetCentralNeutral',
    rhoElectronPfIso = 'fixedGridRhoFastjetAll',
    applyIso = True,
    electronIDVersion = 'cutBase', # can be looseID or HEEPv6
    electronIsoVersion = 'pfISO', # can be pfISO or miniISO
    mu_isoCorr = "rhoArea" ,
    ele_isoCorr = "rhoArea" ,
    mu_effectiveAreas = "Spring15_25ns_v1",
    ele_effectiveAreas = "Spring16_25ns_v1",
    miniIsolationPUCorr = None, # Allowed options: 'rhoArea' (EAs for 03 cone scaled by R^2), 'deltaBeta', 
                                     # 'raw' (uncorrected), 'weights' (delta beta weights; not validated)
                                     # Choose None to just use the individual object's PU correction
    doMuonScaleCorrections = False,
    doElectronScaleCorrections = False,
    do_filter = False # whether filter on two ele/muo events
    )

multtrg = cfg.Analyzer(
    XZZMultTrgEff, name="multitrigger",
    HLTlist=[
        'HLT_Ele105_CaloIdVT_GsfTrkIdT',
        'HLT_Ele115_CaloIdVT_GsfTrkIdT',
        'HLT_Mu45_eta2p1',
        'HLT_Mu50',
        'HLT_TkMu50',
        'HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ',
        'HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ',
        'HLT_Ele23_WPLoose_Gsf',
        'HLT_Ele22_eta2p1_WP75_Gsf',
        'HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ',
        'HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ',
        'HLT_IsoMu20',
        'HLT_IsoTkMu20',
        'HLT_IsoMu27',
        ],
    photonjet=False
)

## Photon Analyzer (generic)
photonAna = cfg.Analyzer(
    XZZPhotonAnalyzer, name='photonAnalyzer',
    photons='slimmedPhotons',
    ebhits=("reducedEgamma","reducedEBRecHits"),
    eehits=("reducedEgamma","reducedEERecHits"),
    eshits=("reducedEgamma","reducedESRecHits"),
    ptMin = 50,
    etaMax = 2.5,
    doPhotonScaleCorrections= False,
    #doPhotonScaleCorrections= {
    #    'data' : 'EgammaAnalysis/ElectronTools/data/ScalesSmearings/Winter_2016_reReco_v1_ele',
    #    'isSync': False
    #},
    gammaID = "cutBasedPhotonID-Fall17-94X-V1-loose",
    rhoPhoton = 'fixedGridRhoFastjetAll',
    gamma_isoCorr = 'rhoArea',
    doFootprintRemovedIsolation = True,
    packedCandidates = 'packedPFCandidates',
    footprintRemovedIsolationPUCorr = 'rhoArea',
    conversionSafe_eleVeto = True,
    do_mc_match = True,
    do_randomCone = False,
)


## Jets Analyzer (generic)
jetAna = cfg.Analyzer(
    JetAnalyzer, name='jetAnalyzer',
    jetCol = 'slimmedJets',
    copyJetsByValue = True,      #Whether or not to copy the input jets or to work with references (should be 'True' if JetAnalyzer is run more than once)
    genJetCol = 'slimmedGenJets',
    rho = ('fixedGridRhoFastjetAll','',''),
    jetPt = 25.,
    jetEta = 4.7,
    jetEtaCentral = 2.4,
    cleanJetsFromLeptons = True,
    jetLepDR = 0.4,
    cleanSelectedLeptons = False, #Whether to clean 'selectedLeptons' after disambiguation. Treat with care (= 'False') if running Jetanalyzer more than once
    minLepPt = 10,
    relaxJetId = False,
    doPuId = False, # Not commissioned in 7.0.X
    recalibrateJets = True,
    applyL2L3Residual = 'Data', # Switch to 'Data' when they will become available for Data
    recalibrationType = "AK4PFchs",
    mcGT     = "Fall17_17Nov2017_V32_MC",
    dataGT   = [(1,"Fall17_17Nov2017B_V32_DATA"),(299337,"Fall17_17Nov2017C_V32_DATA"),(302030,"Fall17_17Nov2017DE_V32_DATA"),(304911,"Fall17_17Nov2017F_V32_DATA")],
    jecPath = "${CMSSW_BASE}/src/CMGTools/XZZ2l2nu/data/jec2017",
    shiftJEC = 0, # set to +1 or -1 to apply +/-1 sigma shift to the nominal jet energies
    addJECShifts = True, # if true, add  "corr", "corrJECUp", and "corrJECDown" for each jet (requires uncertainties to be available!)
    smearJets = False,
    shiftJER = 0, # set to +1 or -1 to get +/-1 sigma shifts
    alwaysCleanPhotons = False,
    cleanGenJetsFromPhoton = False,
    cleanJetsFromFirstPhoton = False,
    cleanJetsFromTaus = False,
    cleanJetsFromIsoTracks = False,
    doQG = False,
    do_mc_match = True,
    collectionPostFix = "",
    calculateSeparateCorrections = True, # should be True if recalibrateJets is True, otherwise L1s will be inconsistent
    calculateType1METCorrection  = False,
    type1METParams = { 'jetPtThreshold':15., 'skipEMfractionThreshold':0.9, 'skipMuons':True },
    storeLowPtJets = False,
    )

jetAnaAK8PFpuppi = cfg.Analyzer(
    JetAnalyzer, name='jetAnalyzerAK8Puppi',
    jetCol = 'selectedUpdatedPatJetsAK8WithDeepTags',#'slimmedJetsAK8',#'packedPatJetsAK8PFPuppiSoftDrop',
    copyJetsByValue = True,      #Whether or not to copy the input jets or to work with references (should be 'True' if JetAnalyzer is run more than once)
    genJetCol = 'selectedUpdatedPatJetsAK8WithDeepTags',#'slimmedGenJetsAK8',
    rho = ('fixedGridRhoFastjetAll','',''),
    jetPt = 150.,
    jetEta = 2.4,
    jetEtaCentral = 2.4,
    cleanJetsFromLeptons = False,
    jetLepDR = 0.8,
    cleanSelectedLeptons = False, #Whether to clean 'selectedLeptons' after disambiguation. Treat with care (= 'False') if running Jetanalyzer more than once
    minLepPt = 10,
    relaxJetId = False,
    doPuId = False, # Not commissioned in 7.0.X
    recalibrateJets = True, #'MC', # True, False, 'MC', 'Data'
    applyL2L3Residual = 'Data', # Switch to 'Data' when they will become available for Data
    recalibrationType = "AK8PFPuppi",
    mcGT     = "Fall17_17Nov2017_V32_MC",
    dataGT   = [(1,"Fall17_17Nov2017B_V32_DATA"),(299337,"Fall17_17Nov2017C_V32_DATA"),(302030,"Fall17_17Nov2017DE_V32_DATA"),(304911,"Fall17_17Nov2017F_V32_DATA")],
    jecPath = "${CMSSW_BASE}/src/CMGTools/XZZ2l2nu/data/jec2017",
    jerPath = "${CMSSW_BASE}/src/CMGTools/XZZ2l2nu/data/jer2016",
    shiftJEC = 0, # set to +1 or -1 to apply +/-1 sigma shift to the nominal jet energies
    addJECShifts = True, # if true, add  "corr", "corrJECUp", and "corrJECDown" for each jet (requires uncertainties to be available!)
    smearJets = False,
    shiftJER = 0, # set to +1 or -1 to get +/-1 sigma shifts
    alwaysCleanPhotons = False,
    cleanGenJetsFromPhoton = False,
    cleanJetsFromFirstPhoton = False,
    cleanJetsFromTaus = False,
    cleanJetsFromIsoTracks = False,
    doQG = False,
    do_mc_match = False,
    collectionPostFix = "AK8PFpuppi",
    calculateSeparateCorrections = True, # should be True if recalibrateJets is True, otherwise L1s will be inconsistent
    calculateType1METCorrection  = False,
    type1METParams = { 'jetPtThreshold':15., 'skipEMfractionThreshold':0.9, 'skipMuons':False },
    storeLowPtJets = False,
    )

boostObjAnaAK8PFpuppi = cfg.Analyzer(
    XZZBoostJetAnalyzer,name='boostObjAnaAK8PFpuppi',
    suffix = 'AK8PFpuppi',
    doPUPPI=True,
    bDiscriminator = "pfCombinedInclusiveSecondaryVertexV2BJetTags",
    boostedBdiscriminator = "pfBoostedDoubleSecondaryVertexAK8BJetTags",
    btagCSVFile = "${CMSSW_BASE}/src/CMGTools/XZZ2l2nu/data/btag.csv",
    puppiJecCorrFile = "${CMSSW_BASE}/src/CMGTools/XZZ2l2nu/data/puppiCorr.root"
)

metAna = cfg.Analyzer(
    XZZMETAnalyzer, name="metAnalyzer",
    metCollection     = "slimmedMETsModifiedMET",
    noPUMetCollection = "slimmedMETsModifiedMET",
    copyMETsByValue = False,
    doTkMet = False,
    doMetNoPU = False,
    doMetNoMu = False,
    doMetNoEle = False,
    doMetNoPhoton = False,
    recalibrate = False, # or "type1", or True, or False
    doMetShiftFromJEC = False, # only works with recalibrate on
    applyJetSmearing = False, # not change the final met used in multiStateAna, does nothing unless the jet smearing turned on in jetAna for MC, copy self.met for data
    old74XMiniAODs = False, # set to True to get the correct Raw MET when running on old 74X MiniAODs
    jetAnalyzerPostFix = "",
    candidates='packedPFCandidates',
    candidatesTypes='std::vector<pat::PackedCandidate>',
    dzMax = 0.1,
    collectionPostFix = "",
    )


leptonicVAna = cfg.Analyzer(
    XZZLeptonicVMaker,
    name='leptonicVMaker',
    selectMuMuPair = (lambda x: (x.leg1.highPtID or x.leg2.highPtID) and (x.leg1.pt()>50.0 or x.leg2.pt()>50.0)),
    selectElElPair = (lambda x: x.leg1.pt()>115.0 or x.leg2.pt()>115.0 ),
    selectVBoson = (lambda x: x.pt()>100.0 and x.mass()>60.0 and x.mass()<120.0),
    doElMu = False, # it would save events with ElMu final states + LL final stats
    selectElMuPair = (lambda x: (x.leg1.pt()>115.0) or (x.leg2.highPtID and x.leg2.pt()>50.0)), # be sure to have leg1=e, leg2=mu
    )

packedAna = cfg.Analyzer(
    PackedCandidateLoader,
    name = 'PackedCandidateLoader',
    select=lambda x: x.pt()<13000.0
    )

multiStateAna = cfg.Analyzer(
    XZZMultiFinalState,
    name='MultiFinalStateMaker',
    processTypes = ["LLNuNu"], # can include "LLNuNu", "ElMuNuNu", "PhotonJets" 
    selectPairLLNuNu = (lambda x: x.leg1.pt()>20.0 and x.leg1.mass()>60.0 and x.leg1.mass()<180.0 and x.leg2.pt()>0.0),
    selectPairElMuNuNu = (lambda x: x.leg1.pt()>20.0 and x.leg1.mass()>30.0 and x.leg1.mass()<300.0 and x.leg2.pt()>0.0),
    selectPhotonJets = (lambda x: x.leg1.pt()>20.0 and x.leg2.pt()>50.0),
    suffix = '',
    )

# Create flags for MET filter bits
"""followed by the MET filters recommendations from
https://twiki.cern.ch/twiki/bin/view/CMS/MissingETOptionalFiltersRun2#MiniAOD_76X_v2_produced_with_the"""
eventFlagsAna = cfg.Analyzer(
    TriggerBitAnalyzer, name="EventFlags",
    processName = 'PAT',
    fallbackProcessName = 'RECO',
    outprefix   = 'Flag',
    triggerBits = {
        "HBHENoiseFilter" : [ "Flag_HBHENoiseFilter" ],
        "HBHENoiseIsoFilter" : [ "Flag_HBHENoiseIsoFilter" ],
        "CSCTightHaloFilter" : [ "Flag_CSCTightHaloFilter" ],
        "CSCTightHalo2015Filter" : [ "Flag_CSCTightHalo2015Filter" ],
        #"hcalLaserEventFilter" : [ "Flag_hcalLaserEventFilter" ],
        "EcalDeadCellTriggerPrimitiveFilter" : [ "Flag_EcalDeadCellTriggerPrimitiveFilter" ],
        "goodVertices" : [ "Flag_goodVertices" ],
        #"trackingFailureFilter" : [ "Flag_trackingFailureFilter" ],
        "eeBadScFilter" : [ "Flag_eeBadScFilter" ],
        # "ecalLaserCorrFilter" : [ "Flag_ecalLaserCorrFilter" ],
        # "trkPOGFilters" : [ "Flag_trkPOGFilters" ],
        # "trkPOG_manystripclus53X" : [ "Flag_trkPOG_manystripclus53X" ],
        # "trkPOG_toomanystripclus53X" : [ "Flag_trkPOG_toomanystripclus53X" ],
        # "trkPOG_logErrorTooManyClusters" : [ "Flag_trkPOG_logErrorTooManyClusters" ],
        "globalSuperTightHalo2016Filter" : [ "Flag_globalSuperTightHalo2016Filter" ],
        "globalTightHalo2016Filter" : [ "Flag_globalTightHalo2016Filter" ],
        "METFilters" : [ "Flag_METFilters" ],
        "badMuons" : [ "Flag_badMuons" ],
        "duplicateMuons" : [ "Flag_duplicateMuons" ],
        "noBadMuons" : [ "Flag_noBadMuons" ],
    }
    )

# Create flags for trigger bits
triggerFlagsAna = cfg.Analyzer(
    TriggerBitAnalyzer, name="TriggerFlags",
    processName = 'HLT',
    triggerBits = {
    }
    )

dumpEvents = cfg.Analyzer(
    XZZDumpEvtList, name="XZZDumpEvtList",
    )


################
coreSequence = [
    skimAnalyzer,
    genAna,
    genAnaGeneric,
    jsonAna,
    triggerAna,
    pileUpAna,
    vertexAna,
    lepAna,
    jetAna,
    jetAnaAK8PFpuppi,
    boostObjAnaAK8PFpuppi,
    metAna,
    photonAna,
    leptonicVAna,
#    packedAna,
    multiStateAna,
    eventFlagsAna,
#    triggerFlagsAna
]

###################




