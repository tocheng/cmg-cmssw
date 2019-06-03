import PhysicsTools.HeppyCore.framework.config as cfg
import os

#####COMPONENT CREATOR

from CMGTools.RootTools.samples.ComponentCreator import ComponentCreator
kreator = ComponentCreator()


# Hengne:
json='/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/ReReco/Final/Cert_271036-284044_13TeV_23Sep2016ReReco_Collisions16_JSON.txt'
run_range = (271036,284044)

### ----------------------------- Zero Tesla run  ----------------------------------------
#dataDir = "$CMSSW_BASE/src/CMGTools/TTHAnalysis/data"  # use environmental variable, useful for instance to run on CRAB
#json=dataDir+'/json/Cert_246908-256869_13TeV_PromptReco_Collisions15_25ns_JSON.txt'
#https://hypernews.cern.ch/HyperNews/CMS/get/physics-validation/2496.html
#golden JSON 166.37/pb 


#jetHT_0T = cfg.DataComponent(
#    name = 'jetHT_0T',
#    files = kreator.getFilesFromEOS('jetHT_0T',
#                                    'firstData_JetHT_v2',
#                                    '/store/user/pandolf/MINIAOD/%s'),
#    intLumi = 4.0,
#    triggers = [],
#    json = None #json
#    )


# PromptReco-v1 for run > 251561
#run_range = (251643, 251883)
#label = "_runs%s_%s"%(run_range[0], run_range[1])

### ----------------------------- Run2016B PromptReco v1 ----------------------------------------

### ----------------------------- Run2016B PromptReco v2 ----------------------------------------

### ----------------------------- Run2016C PromptReco v2 ----------------------------------------

### ----------------------------- Run2016D PromptReco v2 ----------------------------------------

### ----------------------------- Run2016E PromptReco v2 ----------------------------------------

### ----------------------------- Run2016F PromptReco v1 ----------------------------------------

### ----------------------------- Run2016G PromptReco v1 ----------------------------------------

### ----------------------------- Run2016H PromptReco v1 ----------------------------------------

JetHT_Run2016H_PromptReco_v1          = kreator.makeDataComponent("JetHT_Run2016H_PromptReco_v1"         , "/JetHT/Run2016H-PromptReco-v1/MINIAOD"         , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
HTMHT_Run2016H_PromptReco_v1          = kreator.makeDataComponent("HTMHT_Run2016H_PromptReco_v1"         , "/HTMHT/Run2016H-PromptReco-v1/MINIAOD"         , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
MET_Run2016H_PromptReco_v1            = kreator.makeDataComponent("MET_Run2016H_PromptReco_v1"           , "/MET/Run2016H-PromptReco-v1/MINIAOD"           , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
SingleElectron_Run2016H_PromptReco_v1 = kreator.makeDataComponent("SingleElectron_Run2016H_PromptReco_v1", "/SingleElectron/Run2016H-PromptReco-v1/MINIAOD", "CMS", ".*root", json, useAAA=False,jsonFilter=False)
SingleMuon_Run2016H_PromptReco_v1     = kreator.makeDataComponent("SingleMuon_Run2016H_PromptReco_v1"    , "/SingleMuon/Run2016H-PromptReco-v1/MINIAOD"    , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
SinglePhoton_Run2016H_PromptReco_v1   = kreator.makeDataComponent("SinglePhoton_Run2016H_PromptReco_v1"  , "/SinglePhoton/Run2016H-PromptReco-v1/MINIAOD"  , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
DoubleEG_Run2016H_PromptReco_v1       = kreator.makeDataComponent("DoubleEG_Run2016H_PromptReco_v1"      , "/DoubleEG/Run2016H-PromptReco-v1/MINIAOD"      , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
MuonEG_Run2016H_PromptReco_v1         = kreator.makeDataComponent("MuonEG_Run2016H_PromptReco_v1"        , "/MuonEG/Run2016H-PromptReco-v1/MINIAOD"        , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
DoubleMuon_Run2016H_PromptReco_v1     = kreator.makeDataComponent("DoubleMuon_Run2016H_PromptReco_v1"    , "/DoubleMuon/Run2016H-PromptReco-v1/MINIAOD"    , "CMS", ".*root", json, useAAA=False,jsonFilter=False)


### ----------------------------- Run2016H PromptReco v2 ----------------------------------------

JetHT_Run2016H_PromptReco_v2          = kreator.makeDataComponent("JetHT_Run2016H_PromptReco_v2"         , "/JetHT/Run2016H-PromptReco-v2/MINIAOD"         , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
HTMHT_Run2016H_PromptReco_v2          = kreator.makeDataComponent("HTMHT_Run2016H_PromptReco_v2"         , "/HTMHT/Run2016H-PromptReco-v2/MINIAOD"         , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
MET_Run2016H_PromptReco_v2            = kreator.makeDataComponent("MET_Run2016H_PromptReco_v2"           , "/MET/Run2016H-PromptReco-v2/MINIAOD"           , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
SingleElectron_Run2016H_PromptReco_v2 = kreator.makeDataComponent("SingleElectron_Run2016H_PromptReco_v2", "/SingleElectron/Run2016H-PromptReco-v2/MINIAOD", "CMS", ".*root", json, useAAA=False,jsonFilter=False)
SingleMuon_Run2016H_PromptReco_v2     = kreator.makeDataComponent("SingleMuon_Run2016H_PromptReco_v2"    , "/SingleMuon/Run2016H-PromptReco-v2/MINIAOD"    , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
SinglePhoton_Run2016H_PromptReco_v2   = kreator.makeDataComponent("SinglePhoton_Run2016H_PromptReco_v2"  , "/SinglePhoton/Run2016H-PromptReco-v2/MINIAOD"  , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
DoubleEG_Run2016H_PromptReco_v2       = kreator.makeDataComponent("DoubleEG_Run2016H_PromptReco_v2"      , "/DoubleEG/Run2016H-PromptReco-v2/MINIAOD"      , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
MuonEG_Run2016H_PromptReco_v2         = kreator.makeDataComponent("MuonEG_Run2016H_PromptReco_v2"        , "/MuonEG/Run2016H-PromptReco-v2/MINIAOD"        , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
DoubleMuon_Run2016H_PromptReco_v2     = kreator.makeDataComponent("DoubleMuon_Run2016H_PromptReco_v2"    , "/DoubleMuon/Run2016H-PromptReco-v2/MINIAOD"    , "CMS", ".*root", json, useAAA=False,jsonFilter=False)

### ----------------------------- Run2016H PromptReco v3 ----------------------------------------

JetHT_Run2016H_PromptReco_v3          = kreator.makeDataComponent("JetHT_Run2016H_PromptReco_v3"         , "/JetHT/Run2016H-PromptReco-v3/MINIAOD"         , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
HTMHT_Run2016H_PromptReco_v3          = kreator.makeDataComponent("HTMHT_Run2016H_PromptReco_v3"         , "/HTMHT/Run2016H-PromptReco-v3/MINIAOD"         , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
MET_Run2016H_PromptReco_v3            = kreator.makeDataComponent("MET_Run2016H_PromptReco_v3"           , "/MET/Run2016H-PromptReco-v3/MINIAOD"           , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
SingleElectron_Run2016H_PromptReco_v3 = kreator.makeDataComponent("SingleElectron_Run2016H_PromptReco_v3", "/SingleElectron/Run2016H-PromptReco-v3/MINIAOD", "CMS", ".*root", json, useAAA=True,jsonFilter=True)
SingleMuon_Run2016H_PromptReco_v3     = kreator.makeDataComponent("SingleMuon_Run2016H_PromptReco_v3"    , "/SingleMuon/Run2016H-PromptReco-v3/MINIAOD"    , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
SinglePhoton_Run2016H_PromptReco_v3   = kreator.makeDataComponent("SinglePhoton_Run2016H_PromptReco_v3"  , "/SinglePhoton/Run2016H-PromptReco-v3/MINIAOD"  , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
DoubleEG_Run2016H_PromptReco_v3       = kreator.makeDataComponent("DoubleEG_Run2016H_PromptReco_v3"      , "/DoubleEG/Run2016H-PromptReco-v3/MINIAOD"      , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
MuonEG_Run2016H_PromptReco_v3         = kreator.makeDataComponent("MuonEG_Run2016H_PromptReco_v3"        , "/MuonEG/Run2016H-PromptReco-v3/MINIAOD"        , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
DoubleMuon_Run2016H_PromptReco_v3     = kreator.makeDataComponent("DoubleMuon_Run2016H_PromptReco_v3"    , "/DoubleMuon/Run2016H-PromptReco-v3/MINIAOD"    , "CMS", ".*root", json, useAAA=False,jsonFilter=False)


### ----------------------------- Run2016B 23Sep2016 v1 ----------------------------------------

JetHT_Run2016B_23Sep2016          = kreator.makeDataComponent("JetHT_Run2016B_23Sep2016"         , "/JetHT/Run2016B-23Sep2016-v1/MINIAOD"         , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
HTMHT_Run2016B_23Sep2016          = kreator.makeDataComponent("HTMHT_Run2016B_23Sep2016"         , "/HTMHT/Run2016B-23Sep2016-v1/MINIAOD"         , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
MET_Run2016B_23Sep2016            = kreator.makeDataComponent("MET_Run2016B_23Sep2016"           , "/MET/Run2016B-23Sep2016-v2/MINIAOD"           , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
SingleElectron_Run2016B_23Sep2016 = kreator.makeDataComponent("SingleElectron_Run2016B_23Sep2016", "/SingleElectron/Run2016B-23Sep2016-v2/MINIAOD", "CMS", ".*root", json, useAAA=False,jsonFilter=False)
SingleMuon_Run2016B_23Sep2016     = kreator.makeDataComponent("SingleMuon_Run2016B_23Sep2016"    , "/SingleMuon/Run2016B-23Sep2016-v1/MINIAOD"    , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
SinglePhoton_Run2016B_23Sep2016   = kreator.makeDataComponent("SinglePhoton_Run2016B_23Sep2016"  , "/SinglePhoton/Run2016B-23Sep2016-v1/MINIAOD"  , "CMS", ".*root", json, useAAA=True,jsonFilter=True)
DoubleEG_Run2016B_23Sep2016       = kreator.makeDataComponent("DoubleEG_Run2016B_23Sep2016"      , "/DoubleEG/Run2016B-23Sep2016-v2/MINIAOD"      , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
MuonEG_Run2016B_23Sep2016         = kreator.makeDataComponent("MuonEG_Run2016B_23Sep2016"        , "/MuonEG/Run2016B-23Sep2016-v2/MINIAOD"        , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
DoubleMuon_Run2016B_23Sep2016     = kreator.makeDataComponent("DoubleMuon_Run2016B_23Sep2016"    , "/DoubleMuon/Run2016B-23Sep2016-v1/MINIAOD"    , "CMS", ".*root", json, useAAA=False,jsonFilter=False)


### ----------------------------- Run2016B 23Sep2016 v2 ----------------------------------------

JetHT_Run2016B_23Sep2016_v2          = kreator.makeDataComponent("JetHT_Run2016B_23Sep2016_v2"         , "/JetHT/Run2016B-23Sep2016-v3/MINIAOD"         , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
HTMHT_Run2016B_23Sep2016_v2          = kreator.makeDataComponent("HTMHT_Run2016B_23Sep2016_v2"         , "/HTMHT/Run2016B-23Sep2016-v3/MINIAOD"         , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
MET_Run2016B_23Sep2016_v2            = kreator.makeDataComponent("MET_Run2016B_23Sep2016_v2"           , "/MET/Run2016B-23Sep2016-v3/MINIAOD"           , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
SingleElectron_Run2016B_23Sep2016_v2 = kreator.makeDataComponent("SingleElectron_Run2016B_23Sep2016_v2", "/SingleElectron/Run2016B-23Sep2016-v3/MINIAOD", "CMS", ".*root", json, useAAA=False,jsonFilter=False)
SingleMuon_Run2016B_23Sep2016_v2     = kreator.makeDataComponent("SingleMuon_Run2016B_23Sep2016_v2"    , "/SingleMuon/Run2016B-23Sep2016-v3/MINIAOD"    , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
SinglePhoton_Run2016B_23Sep2016_v2   = kreator.makeDataComponent("SinglePhoton_Run2016B_23Sep2016_v2"  , "/SinglePhoton/Run2016B-23Sep2016-v3/MINIAOD"  , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
DoubleEG_Run2016B_23Sep2016_v2       = kreator.makeDataComponent("DoubleEG_Run2016B_23Sep2016_v2"      , "/DoubleEG/Run2016B-23Sep2016-v3/MINIAOD"      , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
MuonEG_Run2016B_23Sep2016_v2         = kreator.makeDataComponent("MuonEG_Run2016B_23Sep2016_v2"        , "/MuonEG/Run2016B-23Sep2016-v3/MINIAOD"        , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
DoubleMuon_Run2016B_23Sep2016_v2     = kreator.makeDataComponent("DoubleMuon_Run2016B_23Sep2016_v2"    , "/DoubleMuon/Run2016B-23Sep2016-v3/MINIAOD"    , "CMS", ".*root", json, useAAA=False,jsonFilter=False)


### ----------------------------- Run2016C 23Sep2016 v2 ----------------------------------------

JetHT_Run2016C_23Sep2016          = kreator.makeDataComponent("JetHT_Run2016C_23Sep2016"         , "/JetHT/Run2016C-23Sep2016-v1/MINIAOD"         , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
HTMHT_Run2016C_23Sep2016          = kreator.makeDataComponent("HTMHT_Run2016C_23Sep2016"         , "/HTMHT/Run2016C-23Sep2016-v1/MINIAOD"         , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
MET_Run2016C_23Sep2016            = kreator.makeDataComponent("MET_Run2016C_23Sep2016"           , "/MET/Run2016C-23Sep2016-v1/MINIAOD"           , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
SingleElectron_Run2016C_23Sep2016 = kreator.makeDataComponent("SingleElectron_Run2016C_23Sep2016", "/SingleElectron/Run2016C-23Sep2016-v1/MINIAOD", "CMS", ".*root", json, useAAA=False,jsonFilter=False)
SingleMuon_Run2016C_23Sep2016     = kreator.makeDataComponent("SingleMuon_Run2016C_23Sep2016"    , "/SingleMuon/Run2016C-23Sep2016-v1/MINIAOD"    , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
SinglePhoton_Run2016C_23Sep2016   = kreator.makeDataComponent("SinglePhoton_Run2016C_23Sep2016"  , "/SinglePhoton/Run2016C-23Sep2016-v1/MINIAOD"  , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
DoubleEG_Run2016C_23Sep2016       = kreator.makeDataComponent("DoubleEG_Run2016C_23Sep2016"      , "/DoubleEG/Run2016C-23Sep2016-v1/MINIAOD"      , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
MuonEG_Run2016C_23Sep2016         = kreator.makeDataComponent("MuonEG_Run2016C_23Sep2016"        , "/MuonEG/Run2016C-23Sep2016-v1/MINIAOD"        , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
DoubleMuon_Run2016C_23Sep2016     = kreator.makeDataComponent("DoubleMuon_Run2016C_23Sep2016"    , "/DoubleMuon/Run2016C-23Sep2016-v1/MINIAOD"    , "CMS", ".*root", json, useAAA=False,jsonFilter=False)

### ----------------------------- Run2016D 23Sep2016 v2 ----------------------------------------

JetHT_Run2016D_23Sep2016          = kreator.makeDataComponent("JetHT_Run2016D_23Sep2016"         , "/JetHT/Run2016D-23Sep2016-v1/MINIAOD"         , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
HTMHT_Run2016D_23Sep2016          = kreator.makeDataComponent("HTMHT_Run2016D_23Sep2016"         , "/HTMHT/Run2016D-23Sep2016-v1/MINIAOD"         , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
MET_Run2016D_23Sep2016            = kreator.makeDataComponent("MET_Run2016D_23Sep2016"           , "/MET/Run2016D-23Sep2016-v1/MINIAOD"           , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
SingleElectron_Run2016D_23Sep2016 = kreator.makeDataComponent("SingleElectron_Run2016D_23Sep2016", "/SingleElectron/Run2016D-23Sep2016-v1/MINIAOD", "CMS", ".*root", json, useAAA=False,jsonFilter=False)
SingleMuon_Run2016D_23Sep2016     = kreator.makeDataComponent("SingleMuon_Run2016D_23Sep2016"    , "/SingleMuon/Run2016D-23Sep2016-v1/MINIAOD"    , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
SinglePhoton_Run2016D_23Sep2016   = kreator.makeDataComponent("SinglePhoton_Run2016D_23Sep2016"  , "/SinglePhoton/Run2016D-23Sep2016-v1/MINIAOD"  , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
DoubleEG_Run2016D_23Sep2016       = kreator.makeDataComponent("DoubleEG_Run2016D_23Sep2016"      , "/DoubleEG/Run2016D-23Sep2016-v1/MINIAOD"      , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
MuonEG_Run2016D_23Sep2016         = kreator.makeDataComponent("MuonEG_Run2016D_23Sep2016"        , "/MuonEG/Run2016D-23Sep2016-v1/MINIAOD"        , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
DoubleMuon_Run2016D_23Sep2016     = kreator.makeDataComponent("DoubleMuon_Run2016D_23Sep2016"    , "/DoubleMuon/Run2016D-23Sep2016-v1/MINIAOD"    , "CMS", ".*root", json, useAAA=False,jsonFilter=False)


### ----------------------------- Run2016E 23Sep2016 v2 ----------------------------------------

JetHT_Run2016E_23Sep2016          = kreator.makeDataComponent("JetHT_Run2016E_23Sep2016"         , "/JetHT/Run2016E-23Sep2016-v1/MINIAOD"         , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
HTMHT_Run2016E_23Sep2016          = kreator.makeDataComponent("HTMHT_Run2016E_23Sep2016"         , "/HTMHT/Run2016E-23Sep2016-v1/MINIAOD"         , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
MET_Run2016E_23Sep2016            = kreator.makeDataComponent("MET_Run2016E_23Sep2016"           , "/MET/Run2016E-23Sep2016-v1/MINIAOD"           , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
SingleElectron_Run2016E_23Sep2016 = kreator.makeDataComponent("SingleElectron_Run2016E_23Sep2016", "/SingleElectron/Run2016E-23Sep2016-v1/MINIAOD", "CMS", ".*root", json, useAAA=False,jsonFilter=False)
SingleMuon_Run2016E_23Sep2016     = kreator.makeDataComponent("SingleMuon_Run2016E_23Sep2016"    , "/SingleMuon/Run2016E-23Sep2016-v1/MINIAOD"    , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
SinglePhoton_Run2016E_23Sep2016   = kreator.makeDataComponent("SinglePhoton_Run2016E_23Sep2016"  , "/SinglePhoton/Run2016E-23Sep2016-v1/MINIAOD"  , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
DoubleEG_Run2016E_23Sep2016       = kreator.makeDataComponent("DoubleEG_Run2016E_23Sep2016"      , "/DoubleEG/Run2016E-23Sep2016-v1/MINIAOD"      , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
MuonEG_Run2016E_23Sep2016         = kreator.makeDataComponent("MuonEG_Run2016E_23Sep2016"        , "/MuonEG/Run2016E-23Sep2016-v1/MINIAOD"        , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
DoubleMuon_Run2016E_23Sep2016     = kreator.makeDataComponent("DoubleMuon_Run2016E_23Sep2016"    , "/DoubleMuon/Run2016E-23Sep2016-v1/MINIAOD"    , "CMS", ".*root", json, useAAA=False,jsonFilter=False)


### ----------------------------- Run2016F 23Sep2016 v1 ----------------------------------------

JetHT_Run2016F_23Sep2016           = kreator.makeDataComponent("JetHT_Run2016F_23Sep2016"         , "/JetHT/Run2016F-23Sep2016-v1/MINIAOD"         , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
HTMHT_Run2016F_23Sep2016           = kreator.makeDataComponent("HTMHT_Run2016F_23Sep2016"         , "/HTMHT/Run2016F-23Sep2016-v1/MINIAOD"         , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
MET_Run2016F_23Sep2016             = kreator.makeDataComponent("MET_Run2016F_23Sep2016"           , "/MET/Run2016F-23Sep2016-v1/MINIAOD"           , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
SingleElectron_Run2016F_23Sep2016  = kreator.makeDataComponent("SingleElectron_Run2016F_23Sep2016", "/SingleElectron/Run2016F-23Sep2016-v1/MINIAOD", "CMS", ".*root", json, useAAA=False,jsonFilter=False)
SingleMuon_Run2016F_23Sep2016      = kreator.makeDataComponent("SingleMuon_Run2016F_23Sep2016"    , "/SingleMuon/Run2016F-23Sep2016-v1/MINIAOD"    , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
SinglePhoton_Run2016F_23Sep2016    = kreator.makeDataComponent("SinglePhoton_Run2016F_23Sep2016"  , "/SinglePhoton/Run2016F-23Sep2016-v1/MINIAOD"  , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
DoubleEG_Run2016F_23Sep2016        = kreator.makeDataComponent("DoubleEG_Run2016F_23Sep2016"      , "/DoubleEG/Run2016F-23Sep2016-v1/MINIAOD"      , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
MuonEG_Run2016F_23Sep2016          = kreator.makeDataComponent("MuonEG_Run2016F_23Sep2016"        , "/MuonEG/Run2016F-23Sep2016-v1/MINIAOD"        , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
DoubleMuon_Run2016F_23Sep2016      = kreator.makeDataComponent("DoubleMuon_Run2016F_23Sep2016"    , "/DoubleMuon/Run2016F-23Sep2016-v1/MINIAOD"    , "CMS", ".*root", json, useAAA=False,jsonFilter=False)



### ----------------------------- Run2016G 23Sep2016 v1 ----------------------------------------

JetHT_Run2016G_23Sep2016           = kreator.makeDataComponent("JetHT_Run2016G_23Sep2016"         , "/JetHT/Run2016G-23Sep2016-v1/MINIAOD"         , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
HTMHT_Run2016G_23Sep2016           = kreator.makeDataComponent("HTMHT_Run2016G_23Sep2016"         , "/HTMHT/Run2016G-23Sep2016-v2/MINIAOD"         , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
MET_Run2016G_23Sep2016             = kreator.makeDataComponent("MET_Run2016G_23Sep2016"           , "/MET/Run2016G-23Sep2016-v1/MINIAOD"           , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
SingleElectron_Run2016G_23Sep2016  = kreator.makeDataComponent("SingleElectron_Run2016G_23Sep2016", "/SingleElectron/Run2016G-23Sep2016-v1/MINIAOD", "CMS", ".*root", json, useAAA=False,jsonFilter=False)
SingleMuon_Run2016G_23Sep2016      = kreator.makeDataComponent("SingleMuon_Run2016G_23Sep2016"    , "/SingleMuon/Run2016G-23Sep2016-v1/MINIAOD"    , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
SinglePhoton_Run2016G_23Sep2016    = kreator.makeDataComponent("SinglePhoton_Run2016G_23Sep2016"  , "/SinglePhoton/Run2016G-23Sep2016-v1/MINIAOD"  , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
DoubleEG_Run2016G_23Sep2016        = kreator.makeDataComponent("DoubleEG_Run2016G_23Sep2016"      , "/DoubleEG/Run2016G-23Sep2016-v1/MINIAOD"      , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
MuonEG_Run2016G_23Sep2016          = kreator.makeDataComponent("MuonEG_Run2016G_23Sep2016"        , "/MuonEG/Run2016G-23Sep2016-v1/MINIAOD"        , "CMS", ".*root", json, useAAA=False,jsonFilter=False)
DoubleMuon_Run2016G_23Sep2016      = kreator.makeDataComponent("DoubleMuon_Run2016G_23Sep2016"    , "/DoubleMuon/Run2016G-23Sep2016-v1/MINIAOD"    , "CMS", ".*root", json, useAAA=False,jsonFilter=False)

### ----------------------------- summary 23Sep2016  ----------------------------------------
SingleMuon23Sep2016=[
            SingleMuon_Run2016B_23Sep2016,
            SingleMuon_Run2016B_23Sep2016_v2,
            SingleMuon_Run2016C_23Sep2016,
            SingleMuon_Run2016D_23Sep2016,
            SingleMuon_Run2016E_23Sep2016,
            SingleMuon_Run2016F_23Sep2016,
            SingleMuon_Run2016G_23Sep2016,
            SingleMuon_Run2016H_PromptReco_v1,
            SingleMuon_Run2016H_PromptReco_v2,
            SingleMuon_Run2016H_PromptReco_v3,
            ]
SingleElectron23Sep2016=[
                SingleElectron_Run2016B_23Sep2016,
                SingleElectron_Run2016B_23Sep2016_v2,
                SingleElectron_Run2016C_23Sep2016,
                SingleElectron_Run2016D_23Sep2016,
                SingleElectron_Run2016E_23Sep2016,
                SingleElectron_Run2016F_23Sep2016,
                SingleElectron_Run2016G_23Sep2016,
                SingleElectron_Run2016H_PromptReco_v1,
                SingleElectron_Run2016H_PromptReco_v2,
                SingleElectron_Run2016H_PromptReco_v3,
               ]
SinglePhoton23Sep2016=[
              SinglePhoton_Run2016B_23Sep2016,
              SinglePhoton_Run2016B_23Sep2016_v2,
              SinglePhoton_Run2016C_23Sep2016,
              SinglePhoton_Run2016D_23Sep2016,
              SinglePhoton_Run2016E_23Sep2016,
              SinglePhoton_Run2016F_23Sep2016,
              SinglePhoton_Run2016G_23Sep2016,
              SinglePhoton_Run2016H_PromptReco_v1,
              SinglePhoton_Run2016H_PromptReco_v2,
              SinglePhoton_Run2016H_PromptReco_v3,
             ]
MuonEG23Sep2016=[
      MuonEG_Run2016B_23Sep2016,
      MuonEG_Run2016B_23Sep2016_v2,
      MuonEG_Run2016C_23Sep2016,
      MuonEG_Run2016D_23Sep2016,
      MuonEG_Run2016E_23Sep2016,
      MuonEG_Run2016F_23Sep2016,
      MuonEG_Run2016G_23Sep2016,
      MuonEG_Run2016H_PromptReco_v1,
      MuonEG_Run2016H_PromptReco_v2,
      MuonEG_Run2016H_PromptReco_v3,
      ]

MET23Sep2016=[
      MET_Run2016B_23Sep2016,
      MET_Run2016B_23Sep2016_v2,
      MET_Run2016C_23Sep2016,
      MET_Run2016D_23Sep2016,
      MET_Run2016E_23Sep2016,
      MET_Run2016F_23Sep2016,
      MET_Run2016G_23Sep2016,
      MET_Run2016H_PromptReco_v1,
      MET_Run2016H_PromptReco_v2,
      MET_Run2016H_PromptReco_v3,
      ]


### ----------------------------- ReMiniAOD 2016 03Feb2017 ----------------------------------------

### ----------------------------- SingleElectron 03Feb2017 ----------------------------------------
SingleElectron_Run2016B_03Feb2017_ver1 = kreator.makeDataComponent("SingleElectron_Run2016B_03Feb2017_ver1", "/SingleElectron/Run2016B-03Feb2017_ver1-v1/MINIAOD", "CMS", ".*root", json, useAAA=True,jsonFilter=True)
SingleElectron_Run2016B_03Feb2017_ver2 = kreator.makeDataComponent("SingleElectron_Run2016B_03Feb2017_ver2", "/SingleElectron/Run2016B-03Feb2017_ver2-v2/MINIAOD", "CMS", ".*root", json, useAAA=True,jsonFilter=True)
SingleElectron_Run2016C_03Feb2017 = kreator.makeDataComponent("SingleElectron_Run2016C_03Feb2017", "/SingleElectron/Run2016C-03Feb2017-v1/MINIAOD", "CMS", ".*root", json, useAAA=True,jsonFilter=True)
SingleElectron_Run2016D_03Feb2017 = kreator.makeDataComponent("SingleElectron_Run2016D_03Feb2017", "/SingleElectron/Run2016D-03Feb2017-v1/MINIAOD", "CMS", ".*root", json, useAAA=True,jsonFilter=True)
SingleElectron_Run2016E_03Feb2017 = kreator.makeDataComponent("SingleElectron_Run2016E_03Feb2017", "/SingleElectron/Run2016E-03Feb2017-v1/MINIAOD", "CMS", ".*root", json, useAAA=True,jsonFilter=True)
SingleElectron_Run2016F_03Feb2017 = kreator.makeDataComponent("SingleElectron_Run2016F_03Feb2017", "/SingleElectron/Run2016F-03Feb2017-v1/MINIAOD", "CMS", ".*root", json, useAAA=True,jsonFilter=True)
SingleElectron_Run2016G_03Feb2017 = kreator.makeDataComponent("SingleElectron_Run2016G_03Feb2017", "/SingleElectron/Run2016G-03Feb2017-v1/MINIAOD", "CMS", ".*root", json, useAAA=True,jsonFilter=True)
SingleElectron_Run2016H_03Feb2017_ver2 = kreator.makeDataComponent("SingleElectron_Run2016H_03Feb2017_ver2", "/SingleElectron/Run2016H-03Feb2017_ver2-v1/MINIAOD", "CMS", ".*root", json, useAAA=True,jsonFilter=True)
SingleElectron_Run2016H_03Feb2017_ver3 = kreator.makeDataComponent("SingleElectron_Run2016H_03Feb2017_ver3", "/SingleElectron/Run2016H-03Feb2017_ver3-v1/MINIAOD", "CMS", ".*root", json, useAAA=True,jsonFilter=True)


### ----------------------------- SingleMuon 03Feb2017 ----------------------------------------
SingleMuon_Run2016B_03Feb2017_ver1 = kreator.makeDataComponent("SingleMuon_Run2016B_03Feb2017_ver1", "/SingleMuon/Run2016B-03Feb2017_ver1-v1/MINIAOD", "CMS", ".*root", json, useAAA=True,jsonFilter=True)
SingleMuon_Run2016B_03Feb2017_ver2 = kreator.makeDataComponent("SingleMuon_Run2016B_03Feb2017_ver2", "/SingleMuon/Run2016B-03Feb2017_ver2-v2/MINIAOD", "CMS", ".*root", json, useAAA=True,jsonFilter=True)
SingleMuon_Run2016C_03Feb2017 = kreator.makeDataComponent("SingleMuon_Run2016C_03Feb2017", "/SingleMuon/Run2016C-03Feb2017-v1/MINIAOD", "CMS", ".*root", json, useAAA=True,jsonFilter=True)
SingleMuon_Run2016D_03Feb2017 = kreator.makeDataComponent("SingleMuon_Run2016D_03Feb2017", "/SingleMuon/Run2016D-03Feb2017-v1/MINIAOD", "CMS", ".*root", json, useAAA=True,jsonFilter=True)
SingleMuon_Run2016E_03Feb2017 = kreator.makeDataComponent("SingleMuon_Run2016E_03Feb2017", "/SingleMuon/Run2016E-03Feb2017-v1/MINIAOD", "CMS", ".*root", json, useAAA=True,jsonFilter=True)
SingleMuon_Run2016F_03Feb2017 = kreator.makeDataComponent("SingleMuon_Run2016F_03Feb2017", "/SingleMuon/Run2016F-03Feb2017-v1/MINIAOD", "CMS", ".*root", json, useAAA=True,jsonFilter=True)
SingleMuon_Run2016G_03Feb2017 = kreator.makeDataComponent("SingleMuon_Run2016G_03Feb2017", "/SingleMuon/Run2016G-03Feb2017-v1/MINIAOD", "CMS", ".*root", json, useAAA=True,jsonFilter=True)
SingleMuon_Run2016H_03Feb2017_ver2 = kreator.makeDataComponent("SingleMuon_Run2016H_03Feb2017_ver2", "/SingleMuon/Run2016H-03Feb2017_ver2-v1/MINIAOD", "CMS", ".*root", json, useAAA=True,jsonFilter=True)
SingleMuon_Run2016H_03Feb2017_ver3 = kreator.makeDataComponent("SingleMuon_Run2016H_03Feb2017_ver3", "/SingleMuon/Run2016H-03Feb2017_ver3-v1/MINIAOD", "CMS", ".*root", json, useAAA=True,jsonFilter=True)


### ----------------------------- SinglePhoton 03Feb2017 ----------------------------------------
SinglePhoton_Run2016B_03Feb2017_ver1 = kreator.makeDataComponent("SinglePhoton_Run2016B_03Feb2017_ver1", "/SinglePhoton/Run2016B-03Feb2017_ver1-v1/MINIAOD", "CMS", ".*root", json, useAAA=True,jsonFilter=True)
SinglePhoton_Run2016B_03Feb2017_ver2 = kreator.makeDataComponent("SinglePhoton_Run2016B_03Feb2017_ver2", "/SinglePhoton/Run2016B-03Feb2017_ver2-v2/MINIAOD", "CMS", ".*root", json, useAAA=True,jsonFilter=True)
SinglePhoton_Run2016C_03Feb2017 = kreator.makeDataComponent("SinglePhoton_Run2016C_03Feb2017", "/SinglePhoton/Run2016C-03Feb2017-v1/MINIAOD", "CMS", ".*root", json, useAAA=True,jsonFilter=True)
SinglePhoton_Run2016D_03Feb2017 = kreator.makeDataComponent("SinglePhoton_Run2016D_03Feb2017", "/SinglePhoton/Run2016D-03Feb2017-v1/MINIAOD", "CMS", ".*root", json, useAAA=True,jsonFilter=True)
SinglePhoton_Run2016E_03Feb2017 = kreator.makeDataComponent("SinglePhoton_Run2016E_03Feb2017", "/SinglePhoton/Run2016E-03Feb2017-v1/MINIAOD", "CMS", ".*root", json, useAAA=True,jsonFilter=True)
SinglePhoton_Run2016F_03Feb2017 = kreator.makeDataComponent("SinglePhoton_Run2016F_03Feb2017", "/SinglePhoton/Run2016F-03Feb2017-v1/MINIAOD", "CMS", ".*root", json, useAAA=True,jsonFilter=True)
SinglePhoton_Run2016G_03Feb2017 = kreator.makeDataComponent("SinglePhoton_Run2016G_03Feb2017", "/SinglePhoton/Run2016G-03Feb2017-v1/MINIAOD", "CMS", ".*root", json, useAAA=True,jsonFilter=True)
SinglePhoton_Run2016H_03Feb2017_ver2 = kreator.makeDataComponent("SinglePhoton_Run2016H_03Feb2017_ver2", "/SinglePhoton/Run2016H-03Feb2017_ver2-v1/MINIAOD", "CMS", ".*root", json, useAAA=True,jsonFilter=True)
SinglePhoton_Run2016H_03Feb2017_ver3 = kreator.makeDataComponent("SinglePhoton_Run2016H_03Feb2017_ver3", "/SinglePhoton/Run2016H-03Feb2017_ver3-v1/MINIAOD", "CMS", ".*root", json, useAAA=True,jsonFilter=True)


### ----------------------------- MuonEG 03Feb2017 ----------------------------------------
MuonEG_Run2016B_03Feb2017_ver1 = kreator.makeDataComponent("MuonEG_Run2016B_03Feb2017_ver1", "/MuonEG/Run2016B-03Feb2017_ver1-v1/MINIAOD", "CMS", ".*root", json, useAAA=True,jsonFilter=True)
MuonEG_Run2016B_03Feb2017_ver2 = kreator.makeDataComponent("MuonEG_Run2016B_03Feb2017_ver2", "/MuonEG/Run2016B-03Feb2017_ver2-v2/MINIAOD", "CMS", ".*root", json, useAAA=True,jsonFilter=True)
MuonEG_Run2016C_03Feb2017 = kreator.makeDataComponent("MuonEG_Run2016C_03Feb2017", "/MuonEG/Run2016C-03Feb2017-v1/MINIAOD", "CMS", ".*root", json, useAAA=True,jsonFilter=True)
MuonEG_Run2016D_03Feb2017 = kreator.makeDataComponent("MuonEG_Run2016D_03Feb2017", "/MuonEG/Run2016D-03Feb2017-v1/MINIAOD", "CMS", ".*root", json, useAAA=True,jsonFilter=True)
MuonEG_Run2016E_03Feb2017 = kreator.makeDataComponent("MuonEG_Run2016E_03Feb2017", "/MuonEG/Run2016E-03Feb2017-v1/MINIAOD", "CMS", ".*root", json, useAAA=True,jsonFilter=True)
MuonEG_Run2016F_03Feb2017 = kreator.makeDataComponent("MuonEG_Run2016F_03Feb2017", "/MuonEG/Run2016F-03Feb2017-v1/MINIAOD", "CMS", ".*root", json, useAAA=True,jsonFilter=True)
MuonEG_Run2016G_03Feb2017 = kreator.makeDataComponent("MuonEG_Run2016G_03Feb2017", "/MuonEG/Run2016G-03Feb2017-v1/MINIAOD", "CMS", ".*root", json, useAAA=True,jsonFilter=True)
MuonEG_Run2016H_03Feb2017_ver2 = kreator.makeDataComponent("MuonEG_Run2016H_03Feb2017_ver2", "/MuonEG/Run2016H-03Feb2017_ver2-v1/MINIAOD", "CMS", ".*root", json, useAAA=False,jsonFilter=True)
MuonEG_Run2016H_03Feb2017_ver3 = kreator.makeDataComponent("MuonEG_Run2016H_03Feb2017_ver3", "/MuonEG/Run2016H-03Feb2017_ver3-v1/MINIAOD", "CMS", ".*root", json, useAAA=False,jsonFilter=True)


### ----------------------------- summary 03Feb2017 ----------------------------------------
SingleElectron_03Feb2017= [ SingleElectron_Run2016B_03Feb2017_ver2,
SingleElectron_Run2016C_03Feb2017,
SingleElectron_Run2016D_03Feb2017,
SingleElectron_Run2016E_03Feb2017,
SingleElectron_Run2016F_03Feb2017,
SingleElectron_Run2016G_03Feb2017,
SingleElectron_Run2016H_03Feb2017_ver2,
SingleElectron_Run2016H_03Feb2017_ver3,
]

SingleMuon_03Feb2017= [ SingleMuon_Run2016B_03Feb2017_ver2,
SingleMuon_Run2016C_03Feb2017,
SingleMuon_Run2016D_03Feb2017,
SingleMuon_Run2016E_03Feb2017,
SingleMuon_Run2016F_03Feb2017,
SingleMuon_Run2016G_03Feb2017,
SingleMuon_Run2016H_03Feb2017_ver2,
SingleMuon_Run2016H_03Feb2017_ver3,
]

SinglePhoton_03Feb2017= [SinglePhoton_Run2016B_03Feb2017_ver2,
SinglePhoton_Run2016C_03Feb2017,
SinglePhoton_Run2016D_03Feb2017,
SinglePhoton_Run2016E_03Feb2017,
SinglePhoton_Run2016F_03Feb2017,
SinglePhoton_Run2016G_03Feb2017,
SinglePhoton_Run2016H_03Feb2017_ver2,
SinglePhoton_Run2016H_03Feb2017_ver3,
]

MuonEG_03Feb2017= [MuonEG_Run2016B_03Feb2017_ver2,
MuonEG_Run2016C_03Feb2017,
MuonEG_Run2016D_03Feb2017,
MuonEG_Run2016E_03Feb2017,
MuonEG_Run2016F_03Feb2017,
MuonEG_Run2016G_03Feb2017,
MuonEG_Run2016H_03Feb2017_ver2,
MuonEG_Run2016H_03Feb2017_ver3,
]

### ----------------------------- summary ----------------------------------------



### ---------------------------------------------------------------------
