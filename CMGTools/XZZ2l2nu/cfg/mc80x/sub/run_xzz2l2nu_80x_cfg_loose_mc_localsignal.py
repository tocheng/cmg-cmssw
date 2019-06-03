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
    "MET120":triggers_metNoMu120_mhtNoMu120
}

#-------- Analyzer
from CMGTools.XZZ2l2nu.analyzers.treeXZZ_cff import *

leptonicVAna.selectMuMuPair = (lambda x: ((x.leg1.pt()>20 or x.leg2.pt()>20)))
leptonicVAna.selectElElPair =(lambda x: x.leg1.pt()>20.0 or x.leg2.pt()>20.0 )
leptonicVAna.selectVBoson = (lambda x: x.mass()>50.0 and x.mass()<180.0)
multiStateAna.selectPairLLNuNu = (lambda x: x.leg1.mass()>50.0 and x.leg1.mass()<180.0 and x.leg1.pt()>100)

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
    jetAna,
    jetAnaAK8PFPuppi,
    boostObjAnaAK8Puppi,
    metAna,
    leptonicVAna,
    multiStateAna,
    eventFlagsAna,
    triggerFlagsAna,
]
    
sequence = cfg.Sequence(coreSequence+[vvSkimmer,multtrg,vvTreeProducer])
 

#-------- HOW TO RUN
test = 1
if test==1:
    # test a single component, using a single thread.
    #selectedComponents = mcSamples
    #selectedComponents = [BulkGravToZZToZlepZinv_narrow_1600] 
    inputSample = cfg.MCComponent(
      'test_component',
      files = ['/eos/user/t/tocheng/X2l+MET+jets/Run2016/MiniAOD/ZpAnomalonZZbbminiaod.root'],  # returns a list of file for this release
    # a list of local or xrootd files can be specified by hand.
    )
    inputSample.isMC = True
    inputSample.isData = False
    inputSample.puFileMC=dataDir+"/pileup_MC_80x_271036-276811_68075.root"
    inputSample.puFileData=dataDir+"/pileup_DATA_80x_271036-276811_68075.root"
    inputSample.eSFinput=dataDir+"/CutBasedID_LooseWP_76X_18Feb.txt_SF2D.root"

    selectedComponents = [inputSample]#BulkGravToZZToZlepZinv_narrow_800]

    for c in selectedComponents:
        #c.files = c.files[:1]
        #c.splitFactor = (len(c.files)/10 if len(c.files)>10 else 1)
        c.splitFactor = len(c.files)
        print c,' ',len(c.files)
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

# and the following runs the process directly if running as with python filename.py  
'''
if __name__ == '__main__':
    from PhysicsTools.HeppyCore.framework.looper import Looper
    looper = Looper( 'Loop', config, nPrint = 0,nEvents=300)
    looper.loop()
    looper.write()
'''


