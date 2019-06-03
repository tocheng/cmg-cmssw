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
from CMGTools.XZZ2l2nu.samples.loadSamples80xSummer16 import *
selectedComponents = mcSamples+dataSamples

triggerFlagsAna.triggerBits ={
    "ISOMU":triggers_1mu_iso,
    "MU":triggers_1mu_noniso,
    "MUv2":triggers_1mu_noniso_v2,
    "MU50":triggers_1mu_noniso_M50,
    "TkMU50":triggers_1mu_noniso_tkM50,
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
    "MET120":triggers_metNoMu120_mhtNoMu120,
    "ALLPHOTON": triggers_all_photons,
    "PHOTONIDISO": triggers_photon_idiso,
    "PHOTONIDISOMETEB": triggers_photon_idisometeb,
    "PHOTONIDISOVBFEB": triggers_photon_idisovbfeb,
    "HALOCLEAN": triggers_halo_clean,
#    "PHOTONIDISO22": triggers_photon22_idiso,
#    "PHOTONIDISO30": triggers_photon30_idiso,
#    "PHOTONIDISO36": triggers_photon36_idiso,
#    "PHOTONIDISO50": triggers_photon50_idiso,
#    "PHOTONIDISO75": triggers_photon75_idiso,
#    "PHOTONIDISO90": triggers_photon90_idiso,
#    "PHOTONIDISO120": triggers_photon120_idiso,
#    "PHOTONIDISO165": triggers_photon165_idiso,
#    "PHOTONIDISOMETEB22": triggers_photon22_idisometeb,
#    "PHOTONIDISOMETEB30": triggers_photon30_idisometeb,
#    "PHOTONIDISOMETEB36": triggers_photon36_idisometeb,
#    "PHOTONIDISOMETEB50": triggers_photon50_idisometeb,
#    "PHOTONIDISOMETEB75": triggers_photon75_idisometeb,
#    "PHOTONIDISOMETEB90": triggers_photon90_idisometeb,
#    "PHOTONIDISOMETEB120": triggers_photon120_idisometeb,
#    "PHOTONIDISOVBFEB22": triggers_photon22_idisovbfeb,
#    "PHOTONIDISOVBFEB30": triggers_photon30_idisovbfeb,
#    "PHOTONIDISOVBFEB36": triggers_photon36_idisovbfeb,
#    "PHOTONIDISOVBFEB50": triggers_photon50_idisovbfeb,
#    "PHOTONIDISOVBFEB75": triggers_photon75_idisovbfeb,
#    "PHOTONIDISOVBFEB90": triggers_photon90_idisovbfeb,
#    "PHOTONIDISOVBFEB120": triggers_photon120_idisovbfeb,
}

#-------- Analyzer
from CMGTools.XZZ2l2nu.analyzers.treeXZZ_cff import *

multiStateAna.processTypes = ['PhotonJets']
multiStateAna.selectPhotonJets = (lambda x: x.leg1.pt()>20.0 and x.leg2.pt()>-0.0)
vvSkimmer.required = ['PhotonJets']

vvTreeProducer.globalVariables = [
         NTupleVariable("nVert",  lambda ev: len(ev.goodVertices), int, help="Number of good vertices"), 
         NTupleVariable("nVertAll",  lambda ev: len(ev.vertices), int, help="Number of good vertices"), 
         NTupleVariable("vtx_z",  lambda ev: ev.goodVertices[0].z(), float, help="primary vertex z"),
         NTupleVariable("rho", lambda ev: ev.rho , float),
     ]
vvTreeProducer.globalObjects =  {  }

vvTreeProducer.collections = {
	 "jets"       : NTupleCollection("jet",JetType,100, help="all jets in miniaod"),
#         "selectedPhotons"       : NTupleCollection("photon",photonType,100, help="selected photons in miniaod"),
         "selectedLeptons" : NTupleCollection("lep",leptonType,10, help="selected leptons"),
         "PhotonJets"     : NTupleCollection("gjet",PhotonJetType ,100, help="photon and MET"),
         "badMuons" : NTupleCollection("badmuon",leptonType,100, help="bad muons"),
         "boostObjsAK8Puppi" : NTupleCollection("boostJetAK8Puppi",FatJetType,10, help="merged jets with substructures in miniaod"),
         "genJetsAK8Puppi"   : NTupleCollection("genJetAK8",genJetType,10, mcOnly=True, help="genJets in miniaod"),
     }




#-------- SEQUENCE
coreSequence = [
    skimAnalyzer,
    genAna,
    genAnaGeneric,
    jsonAna,
    triggerAna,
    pileUpAna,
    vertexAna,
    lepAna,
    photonAna, 
    jetAna,
    jetAnaAK8PFPuppi,
    boostObjAnaAK8Puppi,
    metAna,
    multiStateAna,
    eventFlagsAna,
    triggerFlagsAna,
]
    
#sequence = cfg.Sequence(coreSequence)
sequence = cfg.Sequence(coreSequence+[vvSkimmer,vvTreeProducer])
#sequence = cfg.Sequence(coreSequence+[vvSkimmer,multtrg,vvTreeProducer])

#-------- HOW TO RUN
test = 1
if test==1:
    # test a single component, using a single thread.
    #selectedComponents = mcSamples
    #selectedComponents = [GJet_Pt_20toInf_DoubleEMEnriched, GJet_Pt_20to40_DoubleEMEnriched, GJet_Pt_40toInf_DoubleEMEnriched]
    selectedComponents = ttgjets
    for c in selectedComponents:
        #c.files = c.files[3:10]
        c.splitFactor = (len(c.files)/3+1 if ((len(c.files) % 3)>0) else len(c.files)/3)
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


from PhysicsTools.Heppy.utils.cmsswPreprocessor import CmsswPreprocessor
preprocessor = CmsswPreprocessor("pogRecipesMC.py")

from PhysicsTools.HeppyCore.framework.eventsfwlite import Events
event_class = Events
config = cfg.Config( components = selectedComponents,
                     sequence = sequence,
                     services = [],
                     preprocessor=preprocessor, #this would run cmsRun before running Heppy
                     events_class = event_class)

'''
# and the following runs the process directly if running as with python filename.py  
if __name__ == '__main__':
    from PhysicsTools.HeppyCore.framework.looper import Looper
    looper = Looper( 'Loop', config, nPrint = 1,nEvents=-1)
    looper.loop()
    looper.write()
'''





