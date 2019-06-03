##########################################################
##      configuration for XZZ2l2nu 
##########################################################

import CMGTools.XZZ2l2nu.fwlite.Config as cfg
from CMGTools.XZZ2l2nu.fwlite.Config import printComps
from CMGTools.XZZ2l2nu.RootTools import *
from PhysicsTools.HeppyCore.framework.heppy_loop import getHeppyOption

#Load all common analyzers
from CMGTools.XZZ2l2nu.analyzers.coreXZZ_cff import *

#-------- SAMPLES AND TRIGGERS -----------
from CMGTools.XZZ2l2nu.samples.loadSamples80x import *
selectedComponents = mcSamples+dataSamples

triggerFlagsAna.triggerBits ={
    "ISOMU":triggers_1mu_iso,
    "MU":triggers_1mu_noniso,
    "MUv2":triggers_1mu_noniso_v2,
    "MU50":triggers_1mu_noniso_M50,
    "ISOELE":triggers_1e,
    "ELE":triggers_1e_noniso,
    "ELEv2":triggers_1e_noniso_v2,
    "ELE115":triggers_1e_noniso_E115,
    "MUMU": triggers_mumu,
    "MUMUNOISO":triggers_mumu_noniso,
    "ELEL": triggers_ee,
    "HT800":triggers_HT800,
    "HT900":triggers_HT900,
    "JJ":triggers_dijet_fat,
    "MET90":triggers_met90_mht90+triggers_metNoMu90_mhtNoMu90,
    "MET120":triggers_metNoMu120_mhtNoMu120
}

#-------- Analyzer
from CMGTools.XZZ2l2nu.analyzers.treeXZZ_cff import *

leptonicVAna.doElMu = True
leptonicVAna.selectElMuPair =(lambda x: x.leg1.pt()>20.0 or x.leg2.pt()>20.0 )
leptonicVAna.selectVBoson = (lambda x: x.mass()>30.0 and x.mass()<200.0)
multiStateAna.selectPairLLNuNu = (lambda x: x.leg1.mass()>30.0 and x.leg1.mass()<200.0)

#-------- SEQUENCE
coreSequence = [
    skimAnalyzer,
    genAna,
    jsonAna,
    triggerAna,
    pileUpAna,
    vertexAna,
    lepAna,
    jetAna,
    metAna,
    #photonAna,
    leptonicVAna,
    multiStateAna,
    eventFlagsAna,
    triggerFlagsAna,
]
    
#sequence = cfg.Sequence(coreSequence)
sequence = cfg.Sequence(coreSequence+[vvSkimmer,vvTreeProducer])
#sequence = cfg.Sequence(coreSequence+[vvSkimmer,fullTreeProducer])
 

#-------- HOW TO RUN
test = 1
if test==1:
    # test a single component, using a single thread.
    #selectedComponents = dataSamples
    #selectedComponents = [SingleMuon_Run2016D_PromptReco_v2] 
    #selectedComponents = [SingleMuon_Run2016G_PromptReco_v1] 
    #selectedComponents = SingleMuon+SingleElectron
    selectedComponents = MuonEG23Sep2016+[MuonEG_Run2016H_PromptReco_v1,MuonEG_Run2016H_PromptReco_v2,MuonEG_Run2016H_PromptReco_v3]
    for c in selectedComponents:
        #c.files = c.files[3:6]
        #c.splitFactor = (len(c.files)/5 if len(c.files)>5 else 1)
        c.splitFactor = len(c.files)
        #c.splitFactor = 1
        #c.triggers=triggers_1mu_noniso
        #c.triggers=triggers_1e_noniso

## output histogram
outputService=[]
from PhysicsTools.HeppyCore.framework.services.tfile import TFileService
output_service = cfg.Service(
    TFileService,
    'outputfile',
    name="outputfile",
    fname='vvTreeProducer/tree.root',
    option='recreate'
    )
outputService.append(output_service)

from PhysicsTools.HeppyCore.framework.eventsfwlite import Events
from CMGTools.TTHAnalysis.tools.EOSEventsWithDownload import EOSEventsWithDownload
event_class = EOSEventsWithDownload
event_class = Events
if getHeppyOption("nofetch"):
    event_class = Events
config = cfg.Config( components = selectedComponents,
                     sequence = sequence,
                     services = [],
                     events_class = event_class)


