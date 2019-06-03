import PhysicsTools.HeppyCore.framework.config as cfg
import os

#####COMPONENT CREATOR

from CMGTools.RootTools.samples.ComponentCreator import ComponentCreator
kreator = ComponentCreator()

# DY PT bins:
#https://twiki.cern.ch/twiki/bin/view/CMS/SummaryTable1G25ns#DY_Z

DYJetsToLL_Pt_0To50 = kreator.makeMCComponent("DYJetsToLL_Pt_0To50", "/DYJetsToLL_Zpt-0To50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM", "CMS", ".*root",5352.58, useAAA=True)

DYJetsToLL_Pt_50To100 = kreator.makeMCComponent("DYJetsToLL_Pt_50To100", "/DYJetsToLL_Pt-50To100_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v3/MINIAODSIM", "CMS", ".*root",363.81428, useAAA=True)

############
DYJetsToLL_Pt_100To250 = kreator.makeMCComponent("DYJetsToLL_Pt_100To250_Chunk0", "/DYJetsToLL_Pt-100To250_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v2/MINIAODSIM", "CMS", ".*root",84.014804, useAAA=True)
DYJetsToLL_Pt_100To250_ext1_v1 = kreator.makeMCComponent("DYJetsToLL_Pt_100To250_Chunk1", "/DYJetsToLL_Pt-100To250_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM", "CMS", ".*root",84.014804, useAAA=True)
DYJetsToLL_Pt_100To250_ext2_v1 = kreator.makeMCComponent("DYJetsToLL_Pt_100To250_Chunk2", "/DYJetsToLL_Pt-100To250_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext2-v1/MINIAODSIM", "CMS", ".*root",84.014804, useAAA=True)
DYJetsToLL_Pt_100To250_ext5_v1 = kreator.makeMCComponent("DYJetsToLL_Pt_100To250_Chunk5", "/DYJetsToLL_Pt-100To250_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext5-v1/MINIAODSIM", "CMS", ".*root",84.014804, useAAA=True)

##########
DYJetsToLL_Pt_250To400 = kreator.makeMCComponent("DYJetsToLL_Pt_250To400_Chunk0", "/DYJetsToLL_Pt-250To400_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM", "CMS", ".*root",3.228256512, useAAA=True)
DYJetsToLL_Pt_250To400_ext1_v1 = kreator.makeMCComponent("DYJetsToLL_Pt_250To400_Chunk1", "/DYJetsToLL_Pt-250To400_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM", "CMS", ".*root",3.228256512, useAAA=True)
DYJetsToLL_Pt_250To400_ext2_v1 = kreator.makeMCComponent("DYJetsToLL_Pt_250To400_Chunk2", "/DYJetsToLL_Pt-250To400_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext2-v1/MINIAODSIM", "CMS", ".*root",3.228256512, useAAA=True)
DYJetsToLL_Pt_250To400_ext5_v1 = kreator.makeMCComponent("DYJetsToLL_Pt_250To400_Chunk5", "/DYJetsToLL_Pt-250To400_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext5-v1/MINIAODSIM", "CMS", ".*root",3.228256512, useAAA=True)

#########
DYJetsToLL_Pt_400To650 = kreator.makeMCComponent("DYJetsToLL_Pt_400To650_Chunk0", "/DYJetsToLL_Pt-400To650_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM", "CMS", ".*root",0.436041144, useAAA=True)
DYJetsToLL_Pt_400To650_ext1_v1 = kreator.makeMCComponent("DYJetsToLL_Pt_400To650_Chunk1", "/DYJetsToLL_Pt-400To650_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM", "CMS", ".*root",0.436041144, useAAA=True)
DYJetsToLL_Pt_400To650_ext2_v1 = kreator.makeMCComponent("DYJetsToLL_Pt_400To650_Chunk2", "/DYJetsToLL_Pt-400To650_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext2-v1/MINIAODSIM", "CMS", ".*root",0.436041144, useAAA=True)

#########
DYJetsToLL_Pt_650ToInf = kreator.makeMCComponent("DYJetsToLL_Pt_650ToInf_Chunk0", "/DYJetsToLL_Pt-650ToInf_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM", "CMS", ".*root",0.040981055, useAAA=True)
DYJetsToLL_Pt_650ToInf_ext1_v1 = kreator.makeMCComponent("DYJetsToLL_Pt_650ToInf_Chunk1", "/DYJetsToLL_Pt-650ToInf_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM", "CMS", ".*root",0.040981055, useAAA=True)
DYJetsToLL_Pt_650ToInf_ext2_v1 = kreator.makeMCComponent("DYJetsToLL_Pt_650ToInf_Chunk2", "/DYJetsToLL_Pt-650ToInf_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext2-v1/MINIAODSIM", "CMS", ".*root",0.040981055, useAAA=True)

# cross-section:
# https://twiki.cern.ch/twiki/bin/viewauth/CMS/StandardModelCrossSectionsat13TeV
#https://twiki.cern.ch/twiki/bin/viewauth/CMS/HowToGenXSecAnalyzer
# DY inclusive, NLO RunIISpring16MiniAODv2 
# 28M, x-sec recalculated using FEWZ using z_m50_nnlo_inclusive_NNPDF30_nlo_as_0118 QCD NNLO, QED NLO, including ISR, no FSR (because xsec reduction due to FSR is coming from the M50 mass cut)
DYJetsToLL_M50_Ext = kreator.makeMCComponent("DYJetsToLL_M50_Chunk0", 
"/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext2-v1/MINIAODSIM", "CMS", ".*root", 1921.8*3, useAAA=True)  #the "ext4" set with 129M evts 

#LO inclusive
DYJetsToLL_M50_MGMLM_Ext1 = kreator.makeMCComponent("DYJetsToLL_M50_MGMLM_Chunk1",
"/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v2/MINIAODSIM", "CMS", ".*root", 1921.8*3, useAAA=True) # 50M

# LO* NLO kfactor 1.16261343013
# LO xsec calculated from miniAOD
# NLO/LO = njets NLO calculation / LO from miniAOD
#        = 1921.8*3/4959.0 = 1.16261343013
DY1JetsToLL_M50_MGMLM = kreator.makeMCComponent("DY1JetsToLL_M50_MGMLM",
"/DY1JetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM", "CMS", ".*root", 1013.0*1.16261343013, useAAA=True) 
DY2JetsToLL_M50_MGMLM = kreator.makeMCComponent("DY2JetsToLL_M50_MGMLM",
"/DY2JetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM", "CMS", ".*root", 334.7*1.16261343013, useAAA=True)
DY3JetsToLL_M50_MGMLM = kreator.makeMCComponent("DY3JetsToLL_M50_MGMLM",
"/DY3JetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM", "CMS", ".*root", 102.4*1.16261343013, useAAA=True) 
DY4JetsToLL_M50_MGMLM = kreator.makeMCComponent("DY4JetsToLL_M50_MGMLM",
"/DY4JetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM", "CMS", ".*root", 54.45*1.16261343013, useAAA=True) 
DYBJetsToLL_M50_MGMLM = kreator.makeMCComponent("DYBJetsToLL_M50_MGMLM",
"/DYBJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM", "CMS", ".*root", 88.2771*1.16261343013, useAAA=True) 

# W+Jets
WJetsToLNu = kreator.makeMCComponent("WJetsToLNu",
"/WJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM", "CMS", ".*root", 3* 20508.9,useAAA=True)

### DiBosons

# cross section from https://twiki.cern.ch/twiki/bin/view/CMS/SummaryTable1G25ns#Diboson

ZZTo2L2Nu = kreator.makeMCComponent("ZZTo2L2Nu", 
"/ZZTo2L2Nu_13TeV_powheg_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM", "CMS", ".*root", 0.564,useAAA=True) # 226 files, 8.8M evts, 208GB
ZZTo4L = kreator.makeMCComponent("ZZTo4L", 
"/ZZTo4L_13TeV_powheg_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM", "CMS", ".*root", 1.212,useAAA=True) #  6.7M evts
ZZTo2L2Q = kreator.makeMCComponent("ZZTo2L2Q", 
"/ZZTo2L2Q_13TeV_amcatnloFXFX_madspin_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM", "CMS", ".*root", 3.22,useAAA=True) # 8.6M.
WWTo2L2Nu = kreator.makeMCComponent("WWTo2L2Nu", 
"/WWTo2L2Nu_13TeV-powheg/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM", "CMS", ".*root", 12.178,useAAA=True) #1.9M evts
WWToLNuQQ = kreator.makeMCComponent("WWToLNuQQ", 
"/WWToLNuQQ_13TeV-powheg/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM", "CMS", ".*root", 49.997,useAAA=True) # 1.9M evts
WWToLNuQQ_Ext1 = kreator.makeMCComponent("WWToLNuQQ_Ext1", 
"/WWToLNuQQ_13TeV-powheg/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM", "CMS", ".*root", 49.997,useAAA=True) # 1.9M evts
WZTo1L1Nu2Q = kreator.makeMCComponent("WZTo1L1Nu2Q", 
"/WZTo1L1Nu2Q_13TeV_amcatnloFXFX_madspin_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v3/MINIAODSIM", "CMS", ".*root", 10.71,useAAA=True) # 1.7M
WZTo2L2Q = kreator.makeMCComponent("WZTo2L2Q", 
"/WZTo2L2Q_13TeV_amcatnloFXFX_madspin_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM", "CMS", ".*root", 5.595,useAAA=True) # 25M
WZTo3LNu = kreator.makeMCComponent("WZTo3LNu", 
"/WZTo3LNu_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM", "CMS",  ".*root", 5.26,useAAA=True) # 12.5M NLO up to 1 jet in ME #4.4297 from HZZ2l2nu note

### top
# noSC: 9.8M gen seg: https://cms-pdmv.cern.ch/mcm/public/restapi/requests/get_fragment/TOP-RunIISummer15wmLHEGS-00049/0 
TTTo2L2Nu_noSC = kreator.makeMCComponent("TTTo2L2Nu_noSC", 
"/TTTo2L2Nu_noSC_TuneCUETP8M2T4_13TeV-powheg-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM", "CMS", ".*root", 831.76*((3*0.108)**2),useAAA=True) # 9.8M
# forTTH: 79M Gen frag https://cms-pdmv.cern.ch/mcm/public/restapi/requests/get_fragment/HIG-RunIISummer15wmLHEGS-00481/0
TTTo2L2Nu_forTTH = kreator.makeMCComponent("TTTo2L2Nu_forTTH", 
"/TTTo2L2Nu_TuneCUETP8M2_ttHtranche3_13TeV-powheg-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM", "CMS", ".*root", 831.76*((3*0.108)**2),useAAA=False) 

###
TTZToLLNuNu = kreator.makeMCComponent("TTZToLLNuNu", 
"/TTZToLLNuNu_M-10_TuneCUETP8M1_13TeV-amcatnlo-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM", "CMS", ".*root", 0.2529,useAAA=True) # 2M

TTWJetsToLNu_ext1 = kreator.makeMCComponent("TTWJetsToLNu_Chunk0", 
"/TTWJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v3/MINIAODSIM", "CMS", ".*root", 0.2043,useAAA=True) # 2.2M evt
TTWJetsToLNu_ext2 = kreator.makeMCComponent("TTWJetsToLNu_Chunk1", 
"/TTWJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext2-v1/MINIAODSIM", "CMS", ".*root", 0.2043,useAAA=True) # 3.1M evt

### gamma+jets
### GJets Xsec: https://twiki.cern.ch/twiki/bin/viewauth/CMS/SummaryTable1G25ns#Gamma_jets
GJets_HT40to100 = kreator.makeMCComponent("GJets_HT40to100", 
"/GJets_HT-40To100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM","CMS", ".*root",20790,useAAA=True) #4M
GJets_HT100to200 = kreator.makeMCComponent("GJets_HT100to200", 
"/GJets_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM","CMS", ".*root",9238,useAAA=True) # 5 M
GJets_HT200to400 = kreator.makeMCComponent("GJets_HT200to400", 
"/GJets_HT-200To400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM","CMS", ".*root",2305,useAAA=True) # 10M 
GJets_HT400to600 = kreator.makeMCComponent("GJets_HT400to600", 
"/GJets_HT-400To600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM",
"CMS", ".*root",274.4,useAAA=True) # 2.4M
GJets_HT600toInf = kreator.makeMCComponent("GJets_HT600toInf", 
"/GJets_HT-600ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM",
"CMS", ".*root",93.46,useAAA=True) # 2.45M

GJets_HT40to100_ext = kreator.makeMCComponent("GJets_HT40to100_ext", 
"/GJets_HT-40To100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM",
"CMS", ".*root",20790,useAAA=True) #4M
GJets_HT100to200_ext = kreator.makeMCComponent("GJets_HT100to200_ext", 
"/GJets_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM",
"CMS", ".*root",9238,useAAA=True) # 5 M
GJets_HT200to400_ext = kreator.makeMCComponent("GJets_HT200to400_ext", 
"/GJets_HT-200To400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM",
"CMS", ".*root",2305,useAAA=True) # 10M 
GJets_HT400to600_ext = kreator.makeMCComponent("GJets_HT400to600_ext", 
"/GJets_HT-400To600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v2/MINIAODSIM",
"CMS", ".*root",274.4,useAAA=True) # 2.4M
GJets_HT600toInf_ext = kreator.makeMCComponent("GJets_HT600toInf_ext", 
"/GJets_HT-600ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM",
"CMS", ".*root",93.46,useAAA=True) # 2.45M

GJetsHT = [
GJets_HT40to100,
GJets_HT100to200,
GJets_HT200to400,
GJets_HT400to600,
GJets_HT600toInf
]

### ggZZ
ggZZTo2e2nu = kreator.makeMCComponent("ggZZTo2e2nu", 
"/GluGluToContinToZZTo2e2nu_13TeV_MCFM701_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM", "CMS", ".*root", 0.01898,useAAA=True) #0.01898 
# xsec from McM : https://cms-pdmv.cern.ch/mcm/requests?prepid=HIG-RunIIWinter15pLHE-00083&page=0&shown=4063359
ggZZTo2mu2nu = kreator.makeMCComponent("ggZZTo2mu2nu", 
"/GluGluToContinToZZTo2mu2nu_13TeV_MCFM701_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM", "CMS", ".*root", 0.01898,useAAA=True) #0.01898

# TTGJets
TTGJets = kreator.makeMCComponent("TTGJets",
"/TTGJets_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM",
"CMS", ".*root", 3.697,useAAA=True)
TTGJets_ext = kreator.makeMCComponent("TTGJets_ext",
"/TTGJets_TuneCUETP8M1_13TeV-amcatnloFXFX-madspin-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM",
"CMS", ".*root", 3.697,useAAA=True)
ttgjets = [TTGJets, TTGJets_ext]

# ZnunuGJets
ZNuNuGJetsGt130 = kreator.makeMCComponent("ZNuNuGJetsGt130", 
"/ZNuNuGJets_MonoPhoton_PtG-130_TuneCUETP8M1_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM", 
"CMS", ".*root", 0.1903,useAAA=True)
ZNuNuGJetsGt40Lt130 = kreator.makeMCComponent("ZNuNuGJetsGt40Lt130", 
"/ZNuNuGJets_MonoPhoton_PtG-40to130_TuneCUETP8M1_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM",
"CMS", ".*root",2.816,useAAA=True)
ZNuNuGJets = [ZNuNuGJetsGt130,ZNuNuGJetsGt40Lt130]

#WGJets
WGToLNuG = kreator.makeMCComponent("WGToLNuG", 
"/WGToLNuG_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM",
"CMS", ".*root", 585.8,useAAA=True)
WGJetsPt130 = kreator.makeMCComponent("WGJetsPt130", 
"/WGJets_MonoPhoton_PtG-130_TuneCUETP8M1_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM",
"CMS", ".*root", 585.8,useAAA=True)
WGJets = [WGToLNuG, WGJetsPt130]

# QCD HT bins (cross sections from https://twiki.cern.ch/twiki/bin/viewauth/CMS/SummaryTable1G25ns#QCD

# QCD PT bins

# qcd emenr
#QCD_Pt15to20_EMEnriched   = kreator.makeMCComponent("QCD_Pt15to20_EMEnriched"  ,"/QCD_Pt-15to20_EMEnriched_TuneCUETP8M1_13TeV_pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM"  , "CMS", ".*root", 1273000000*0.0002)
QCD_Pt20to30_EMEnriched   = kreator.makeMCComponent("QCD_Pt20to30_EMEnriched"  ,
"/QCD_Pt-20to30_EMEnriched_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM",
"CMS", ".*root", 557600000*0.0096, useAAA=True)
QCD_Pt30to50_EMEnriched   = kreator.makeMCComponent("QCD_Pt30to50_EMEnriched"  ,
"/QCD_Pt-30to50_EMEnriched_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM",
"CMS", ".*root", 136000000*0.073, useAAA=True)
QCD_Pt30to50_EMEnriched_ext   = kreator.makeMCComponent("QCD_Pt30to50_EMEnriched_ext"  ,
"/QCD_Pt-30to50_EMEnriched_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM",
"CMS", ".*root", 136000000*0.073, useAAA=True)
QCD_Pt50to80_EMEnriched   = kreator.makeMCComponent("QCD_Pt50to80_EMEnriched", 
"/QCD_Pt-50to80_EMEnriched_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM", # in production
"CMS", ".*root", 19800000*0.146, useAAA=True)
QCD_Pt50to80_EMEnriched_ext   = kreator.makeMCComponent("QCD_Pt50to80_EMEnriched_ext", 
"/QCD_Pt-50to80_EMEnriched_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM",
"CMS", ".*root", 19800000*0.146, useAAA=True)
QCD_Pt80to120_EMEnriched  = kreator.makeMCComponent("QCD_Pt80to120_EMEnriched" ,
"/QCD_Pt-80to120_EMEnriched_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM",
"CMS", ".*root", 2800000*0.125, useAAA=True)
QCD_Pt80to120_EMEnriched_ext  = kreator.makeMCComponent("QCD_Pt80to120_EMEnriched_ext" ,
"/QCD_Pt-80to120_EMEnriched_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM",
"CMS", ".*root", 2800000*0.125, useAAA=True)
QCD_Pt120to170_EMEnriched = kreator.makeMCComponent("QCD_Pt120to170_EMEnriched",
"/QCD_Pt-120to170_EMEnriched_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM",
"CMS", ".*root", 477000*0.132, useAAA=True)
QCD_Pt170to300_EMEnriched = kreator.makeMCComponent("QCD_Pt170to300_EMEnriched",
"/QCD_Pt-170to300_EMEnriched_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM",
"CMS", ".*root", 114000*0.165, useAAA=True)
QCD_Pt300toInf_EMEnriched = kreator.makeMCComponent("QCD_Pt300toInf_EMEnriched",
"/QCD_Pt-300toInf_EMEnriched_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM",
"CMS", ".*root", 9000*0.15, useAAA=True)

QCDPtEMEnriched = [
QCD_Pt20to30_EMEnriched,
QCD_Pt30to50_EMEnriched,
QCD_Pt50to80_EMEnriched,
QCD_Pt80to120_EMEnriched,
QCD_Pt120to170_EMEnriched,
QCD_Pt170to300_EMEnriched,
QCD_Pt300toInf_EMEnriched
]


QCD_Pt15to20_MuEnrichedPt5 = kreator.makeMCComponent("QCD_Pt15to20_MuEnrichedPt5",
"/QCD_Pt-15to20_MuEnrichedPt5_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM",
"CMS", ".*root",1273190000*0.003, useAAA=True)
QCD_Pt20to30_MuEnrichedPt5 = kreator.makeMCComponent("QCD_Pt20to30_MuEnrichedPt5",
"/QCD_Pt-20to30_MuEnrichedPt5_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM",
"CMS", ".*root",558528000*0.0053, useAAA=True)
QCD_Pt30to50_MuEnrichedPt5 = kreator.makeMCComponent("QCD_Pt30to50_MuEnrichedPt5",
"/QCD_Pt-30to50_MuEnrichedPt5_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM",
"CMS", ".*root", 139803000* 0.01182 , useAAA=True)
QCD_Pt50to80_MuEnrichedPt5 = kreator.makeMCComponent("QCD_Pt50to80_MuEnrichedPt5",
"/QCD_Pt-50to80_MuEnrichedPt5_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM",
"CMS", ".*root", 19222500* 0.02276 , useAAA=True)
QCD_Pt80to120_MuEnrichedPt5 = kreator.makeMCComponent("QCD_Pt80to120_MuEnrichedPt5",
"/QCD_Pt-80to120_MuEnrichedPt5_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM",
"CMS", ".*root",2758420*0.03844 , useAAA=True)
QCD_Pt80to120_MuEnrichedPt5_ext1 = kreator.makeMCComponent("QCD_Pt80to120_MuEnrichedPt5_ext1",
"/QCD_Pt-80to120_MuEnrichedPt5_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v3/MINIAODSIM",
"CMS", ".*root",2758420*0.03844, useAAA=True)
QCD_Pt120to170_MuEnrichedPt5 = kreator.makeMCComponent("QCD_Pt120to170_MuEnrichedPt5",
"/QCD_Pt-120to170_MuEnrichedPt5_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM",
"CMS", ".*root",469797*0.05362 , useAAA=True)
QCD_Pt120to170_MuEnrichedPt5_backup = kreator.makeMCComponent("QCD_Pt120to170_MuEnrichedPt5_backup",
"/QCD_Pt-120to170_MuEnrichedPt5_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_backup_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM",
"CMS", ".*root",469797*0.05362, useAAA=True)
QCD_Pt170to300_MuEnrichedPt5 = kreator.makeMCComponent("QCD_Pt170to300_MuEnrichedPt5",
"/QCD_Pt-170to300_MuEnrichedPt5_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM",
"CMS", ".*root",117989*0.07335, useAAA=True)
QCD_Pt170to300_MuEnrichedPt5_ext1 = kreator.makeMCComponent("QCD_Pt170to300_MuEnrichedPt5_ext1",
"/QCD_Pt-170to300_MuEnrichedPt5_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM",
"CMS", ".*root",117989*0.07335, useAAA=True)
QCD_Pt170to300_MuEnrichedPt5_backup = kreator.makeMCComponent("QCD_Pt170to300_MuEnrichedPt5_backup",
"/QCD_Pt-170to300_MuEnrichedPt5_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_backup_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM",
"CMS", ".*root",117989*0.07335, useAAA=True)
QCD_Pt300to470_MuEnrichedPt5 = kreator.makeMCComponent("QCD_Pt300to470_MuEnrichedPt5",
"/QCD_Pt-300to470_MuEnrichedPt5_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM",
"CMS", ".*root", 7820.25*0.10196, useAAA=True)
QCD_Pt300to470_MuEnrichedPt5_ext1 = kreator.makeMCComponent("QCD_Pt300to470_MuEnrichedPt5_ext1",
"/QCD_Pt-300to470_MuEnrichedPt5_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM",
"CMS", ".*root",7820.25*0.10196, useAAA=True)
QCD_Pt300to470_MuEnrichedPt5_ext2 = kreator.makeMCComponent("QCD_Pt300to470_MuEnrichedPt5_ext2",
"/QCD_Pt-300to470_MuEnrichedPt5_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext2-v1/MINIAODSIM",
"CMS", ".*root",7820.25*0.10196, useAAA=True)
QCD_Pt470to600_MuEnrichedPt5 = kreator.makeMCComponent("QCD_Pt470to600_MuEnrichedPt5",
"/QCD_Pt-470to600_MuEnrichedPt5_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM",
"CMS", ".*root", 645.528*0.12242, useAAA=True)
QCD_Pt470to600_MuEnrichedPt5_ext1 = kreator.makeMCComponent("QCD_Pt470to600_MuEnrichedPt5_ext1",
"/QCD_Pt-470to600_MuEnrichedPt5_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM",
"CMS", ".*root",645.528*0.12242, useAAA=True)
QCD_Pt470to600_MuEnrichedPt5_ext2 = kreator.makeMCComponent("QCD_Pt470to600_MuEnrichedPt5_ext2",
"/QCD_Pt-470to600_MuEnrichedPt5_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext2-v1/MINIAODSIM",
"CMS", ".*root",645.528*0.12242, useAAA=True)
QCD_Pt600to800_MuEnrichedPt5 = kreator.makeMCComponent("QCD_Pt600to800_MuEnrichedPt5",
"/QCD_Pt-600to800_MuEnrichedPt5_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM",
"CMS", ".*root",187.109*0.13412, useAAA=True)
QCD_Pt600to800_MuEnrichedPt5_ext1 = kreator.makeMCComponent("QCD_Pt600to800_MuEnrichedPt5_ext1",
"/QCD_Pt-600to800_MuEnrichedPt5_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM",
"CMS", ".*root",187.109*0.13412, useAAA=True)
QCD_Pt600to800_MuEnrichedPt5_backup = kreator.makeMCComponent("QCD_Pt600to800_MuEnrichedPt5_backup",
"/QCD_Pt-600to800_MuEnrichedPt5_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_backup_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM",
"CMS", ".*root",187.109*0.13412, useAAA=True)
QCD_Pt800to1000_MuEnrichedPt5 = kreator.makeMCComponent("QCD_Pt800to1000_MuEnrichedPt5",
"/QCD_Pt-800to1000_MuEnrichedPt5_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM",
"CMS", ".*root",32.3486*0.14552, useAAA=True)
QCD_Pt800to1000_MuEnrichedPt5_ext1 = kreator.makeMCComponent("QCD_Pt800to1000_MuEnrichedPt5_ext1",
"/QCD_Pt-800to1000_MuEnrichedPt5_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM",
"CMS", ".*root", 32.3486*0.14552, useAAA=True)
QCD_Pt800to1000_MuEnrichedPt5_ext2 = kreator.makeMCComponent("QCD_Pt800to1000_MuEnrichedPt5_ext2",
"/QCD_Pt-800to1000_MuEnrichedPt5_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext2-v1/MINIAODSIM",
"CMS", ".*root", 32.3486*0.14552, useAAA=True)
QCD_Pt1000toInf_MuEnrichedPt5 = kreator.makeMCComponent("QCD_Pt1000toInf_MuEnrichedPt5",
"/QCD_Pt-1000toInf_MuEnrichedPt5_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM",
"CMS", ".*root",10.4305*0.15544, useAAA=True)
QCD_Pt1000toInf_MuEnrichedPt5_ext1 = kreator.makeMCComponent("QCD_Pt1000toInf_MuEnrichedPt5_ext1",
"/QCD_Pt-1000toInf_MuEnrichedPt5_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v3/MINIAODSIM",
"CMS", ".*root",10.4305*0.15544, useAAA=True)
QCD_Pt20toInf_MuEnrichedPt15 = kreator.makeMCComponent("QCD_Pt20toInf_MuEnrichedPt15",
"/QCD_Pt-20toInf_MuEnrichedPt15_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM",
"CMS", ".*root",720648000*0.00042, useAAA=True)

QCDPtMuEnriched=[
QCD_Pt15to20_MuEnrichedPt5,
QCD_Pt20to30_MuEnrichedPt5,
QCD_Pt30to50_MuEnrichedPt5,
QCD_Pt50to80_MuEnrichedPt5,
QCD_Pt80to120_MuEnrichedPt5,
QCD_Pt80to120_MuEnrichedPt5_ext1,
QCD_Pt120to170_MuEnrichedPt5,
QCD_Pt120to170_MuEnrichedPt5_backup,
QCD_Pt170to300_MuEnrichedPt5,
QCD_Pt170to300_MuEnrichedPt5_ext1,
QCD_Pt170to300_MuEnrichedPt5_backup,
QCD_Pt300to470_MuEnrichedPt5,
QCD_Pt300to470_MuEnrichedPt5_ext1,
QCD_Pt300to470_MuEnrichedPt5_ext2,
QCD_Pt470to600_MuEnrichedPt5,
QCD_Pt470to600_MuEnrichedPt5_ext1,
QCD_Pt470to600_MuEnrichedPt5_ext2,
QCD_Pt600to800_MuEnrichedPt5,
QCD_Pt600to800_MuEnrichedPt5_ext1,
QCD_Pt600to800_MuEnrichedPt5_backup,
QCD_Pt800to1000_MuEnrichedPt5,
QCD_Pt800to1000_MuEnrichedPt5_ext1,
QCD_Pt800to1000_MuEnrichedPt5_ext2,
QCD_Pt1000toInf_MuEnrichedPt5,
QCD_Pt1000toInf_MuEnrichedPt5_ext1,
QCD_Pt20toInf_MuEnrichedPt15,
]

T_tch_powheg = kreator.makeMCComponent("T_tch_powheg", 
"/ST_t-channel_top_4f_inclusiveDecays_13TeV-powhegV2-madspin-pythia8_TuneCUETP8M1/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM",
"CMS", ".*root", 136.02, useAAA=True)
TBar_tch_powheg = kreator.makeMCComponent("TBar_tch_powheg",
"/ST_t-channel_antitop_4f_inclusiveDecays_13TeV-powhegV2-madspin-pythia8_TuneCUETP8M1/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM",
"CMS", ".*root", 80.95, useAAA=True)

TBar_tWch = kreator.makeMCComponent("TBar_tWch", 
"/ST_tW_antitop_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM",
"CMS", ".*root",35.6, useAAA=True)
T_tWch= kreator.makeMCComponent("T_tWch", 
"/ST_tW_top_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM",
"CMS", ".*root",35.6, useAAA=True)

TGJets = kreator.makeMCComponent("TGJets",
"/TGJets_TuneCUETP8M1_13TeV_amcatnlo_madspin_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM", 
"CMS", ".*root", 2.967, useAAA=True)
TGJets_ext = kreator.makeMCComponent("TGJets_ext", 
"/TGJets_TuneCUETP8M1_13TeV_amcatnlo_madspin_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM",
"CMS", ".*root", 2.967, useAAA=True)

tgjets = [TGJets, TGJets_ext]

SingleTop = [
T_tch_powheg,
TBar_tch_powheg,
T_tWch,
TBar_tWch,
TGJets,
]

### Zinv
ZJetsToNuNu_HT100to200 = kreator.makeMCComponent("ZJetsToNuNu_HT100to200", 
"/ZJetsToNuNu_HT-100To200_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM",
"CMS", ".*root",280.47*1.23, useAAA=True)
ZJetsToNuNu_HT100to200_ext = kreator.makeMCComponent("ZJetsToNuNu_HT100to200_ext", 
"/ZJetsToNuNu_HT-100To200_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM",
"CMS", ".*root",280.47*1.23, useAAA=True)
ZJetsToNuNu_HT200to400 = kreator.makeMCComponent("ZJetsToNuNu_HT200to400", 
"/ZJetsToNuNu_HT-200To400_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM",
"CMS", ".*root",78.36*1.23, useAAA=True)
ZJetsToNuNu_HT200to400_ext = kreator.makeMCComponent("ZJetsToNuNu_HT200to400_ext", 
"/ZJetsToNuNu_HT-200To400_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM",
"CMS", ".*root",78.36*1.23, useAAA=True)
ZJetsToNuNu_HT400to600 = kreator.makeMCComponent("ZJetsToNuNu_HT400to600", 
"/ZJetsToNuNu_HT-400To600_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM",
"CMS", ".*root",10.94*1.23, useAAA=True)
ZJetsToNuNu_HT600to800 = kreator.makeMCComponent("ZJetsToNuNu_HT600to800", 
"/ZJetsToNuNu_HT-400To600_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM",
"CMS", ".*root",3.221*1.23, useAAA=True)
ZJetsToNuNu_HT800to1200 = kreator.makeMCComponent("ZJetsToNuNu_HT800t1200", 
"/ZJetsToNuNu_HT-800To1200_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM",
"CMS", ".*root",1.474*1.23, useAAA=True)
ZJetsToNuNu_HT1200to2500 = kreator.makeMCComponent("ZJetsToNuNu_HT1200to2500", 
"/ZJetsToNuNu_HT-1200To2500_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM",
"CMS", ".*root",0.3586*1.23, useAAA=True)
ZJetsToNuNu_HT1200to2500_ext = kreator.makeMCComponent("ZJetsToNuNu_HT1200to2500_ext", 
"/ZJetsToNuNu_HT-1200To2500_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM",
"CMS", ".*root",0.3586*1.23, useAAA=True)
ZJetsToNuNu_HT2500toInf = kreator.makeMCComponent("ZJetsToNuNu_HT2500toInf", 
"/ZJetsToNuNu_HT-2500ToInf_13TeV-madgraph/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM",
"CMS", ".*root",0.008203*1.23, useAAA=True)

#280.47*1.23+78.36*1.23+10.94*1.23+1.474*1.23+0.3586*1.23+0.008203*1.23

ZJetsToNuNuHT = [
ZJetsToNuNu_HT100to200,
ZJetsToNuNu_HT200to400,
ZJetsToNuNu_HT400to600,
ZJetsToNuNu_HT600to800,
ZJetsToNuNu_HT800to1200,
ZJetsToNuNu_HT1200to2500,
ZJetsToNuNu_HT2500toInf,
]


### W+jets
# xsec from https://twiki.cern.ch/twiki/bin/viewauth/CMS/SummaryTable1G25ns#W_jets
WJetsToLNu_HT100to200 = kreator.makeMCComponent("WJetsToLNu_HT100to200", 
"/WJetsToLNu_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM", 
"CMS", ".*root",1345*1.21,useAAA=True)
WJetsToLNu_HT100to200_ext1 = kreator.makeMCComponent("WJetsToLNu_HT100to200_ext1", 
"/WJetsToLNu_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM", 
"CMS", ".*root",1345*1.21,useAAA=True)
WJetsToLNu_HT100to200_ext2 = kreator.makeMCComponent("WJetsToLNu_HT100to200_ext2", 
"/WJetsToLNu_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext2-v1/MINIAODSIM", 
"CMS", ".*root",1345*1.21,useAAA=True)
WJetsToLNu_HT200to400 = kreator.makeMCComponent("WJetsToLNu_HT200to400", 
"/WJetsToLNu_HT-200To400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM", 
"CMS", ".*root",359.7*1.21,useAAA=True)
WJetsToLNu_HT200to400_ext1 = kreator.makeMCComponent("WJetsToLNu_HT200to400_ext1", 
"/WJetsToLNu_HT-200To400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM", 
"CMS", ".*root",359.7*1.21,useAAA=True)
WJetsToLNu_HT200to400_ext2 = kreator.makeMCComponent("WJetsToLNu_HT200to400_ext2", 
"/WJetsToLNu_HT-200To400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext2-v1/MINIAODSIM", 
"CMS", ".*root",359.7*1.21,useAAA=True)
WJetsToLNu_HT400to600 = kreator.makeMCComponent("WJetsToLNu_HT400to600", 
"/WJetsToLNu_HT-400To600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM", 
"CMS", ".*root",48.91*1.21,useAAA=True)
WJetsToLNu_HT400to600_ext1 = kreator.makeMCComponent("WJetsToLNu_HT400to600_ext1", 
"/WJetsToLNu_HT-400To600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM", 
"CMS", ".*root",48.91*1.21,useAAA=True)
WJetsToLNu_HT600to800 = kreator.makeMCComponent("WJetsToLNu_HT600to800", 
"/WJetsToLNu_HT-600To800_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM", 
"CMS", ".*root",12.05*1.21,useAAA=True)
WJetsToLNu_HT600to800_ext1 = kreator.makeMCComponent("WJetsToLNu_HT600to800_ext1", 
"/WJetsToLNu_HT-600To800_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM", 
"CMS", ".*root",12.05*1.21,useAAA=True)
WJetsToLNu_HT800to1200 = kreator.makeMCComponent("WJetsToLNu_HT800to1200", 
"/WJetsToLNu_HT-800To1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM", 
"CMS", ".*root",5.501*1.21,useAAA=True)
WJetsToLNu_HT800to1200_ext1 = kreator.makeMCComponent("WJetsToLNu_HT800to1200_ext1", 
"/WJetsToLNu_HT-800To1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM", 
"CMS", ".*root",5.501*1.21,useAAA=True)
WJetsToLNu_HT1200to2500 = kreator.makeMCComponent("WJetsToLNu_HT1200to2500", 
"/WJetsToLNu_HT-1200To2500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM", 
"CMS", ".*root",1.329*1.21,useAAA=True)
WJetsToLNu_HT1200to2500_ext1 = kreator.makeMCComponent("WJetsToLNu_HT1200to2500_ext1", 
"/WJetsToLNu_HT-1200To2500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM", 
"CMS", ".*root",1.329*1.21,useAAA=True)
WJetsToLNu_HT2500toInf = kreator.makeMCComponent("WJetsToLNu_HT2500toInf", 
"/WJetsToLNu_HT-2500ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM", 
"CMS", ".*root",0.03216*1.21,useAAA=True)
WJetsToLNu_HT2500toInf_ext1 = kreator.makeMCComponent("WJetsToLNu_HT2500toInf_ext1", 
"/WJetsToLNu_HT-2500ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM", 
"CMS", ".*root",0.03216*1.21,useAAA=True)

# 1345*1.21+359.7*1.21+48.91*1.21+12.05*1.21+5.501*1.21+1.329*1.21+0.03216*1.21

WJetsToLNuHT = [
WJetsToLNu_HT100to200,
WJetsToLNu_HT200to400,
WJetsToLNu_HT400to600,
WJetsToLNu_HT600to800,
WJetsToLNu_HT800to1200,
WJetsToLNu_HT1200to2500,
WJetsToLNu_HT2500toInf,
]

