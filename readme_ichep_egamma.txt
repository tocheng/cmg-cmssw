ECAL scale and resolution corrections for ICHEP 2016 (CMSSW 8_0_11)
Release is based on CMSSW_8_0_11. Scale and resolution corrections are based on the Golden JSON file with 12.9 fb-1 of 2016 data (final corrections for ICHEP 2016).

# setup CMSSW and the base git
cmsrel CMSSW_8_0_11 
cd CMSSW_8_0_11/src 
cmsenv
git cms-init

# add the repository where the updated Egamma package is
git remote add -f -t ecal_smear_fix_80X emanueledimarco https://github.com/emanueledimarco/cmssw.git
git cms-addpkg EgammaAnalysis/ElectronTools
git checkout -b from-52f192a 52f192a

# download the txt files with the corrections
cd EgammaAnalysis/ElectronTools/data
# corrections calculated with 12.9 fb-1 of 2016 data (ICHEP 16 dataset).
git clone -b ICHEP2016_v2 https://github.com/ECALELFS/ScalesSmearings.git

#compile
cd $CMSSW_BASE/src && scram b -j 8

The package contains a class which is able to produce a new collection of electrons (or photons) with the proper correction applied. In order to run it just add to your configuration the following include:

process.load('Configuration.StandardSequences.Services_cff')
process.RandomNumberGeneratorService = cms.Service("RandomNumberGeneratorService",
                                                       calibratedPatElectrons  = cms.PSet( initialSeed = cms.untracked.uint32(81),
                                                                                                                 engineName = cms.untracked.string('TRandom3'),
                                                                                           ),
                                                       calibratedPatPhotons  = cms.PSet( initialSeed = cms.untracked.uint32(81),
                                                                                                                 engineName = cms.untracked.string('TRandom3'),
                                                                                           ),
                                                       )
process.load('EgammaAnalysis.ElectronTools.calibratedElectronsRun2_cfi')
and/or
process.load('EgammaAnalysis.ElectronTools.calibratedPhotonsRun2_cfi')

The two first commands are required for MC smearing only and should be adapted if you are not using Pat (if you are using reco objects, just remove Pat in the RandomNumberServiceGenerator)

and then add to you sequence to process miniAOD:

process.calibratedPatElectrons 
or
process.calibratedPatPhotons 

(similar modules are available for RECO/AOD). In order to avoid annoying crashes please add the following selector to the electrons before the calibrated collection is produced (in the meantime we are providing a protection in the calibrator itself): process.selectedElectrons = cms.EDFilter("PATElectronSelector", src = cms.InputTag("slimmedElectrons"), cut = cms.string("pt > 5 && abs(eta)<2.5") ) Then of course you have to change the input of the "smearer" from slimmedElectrons to selectedElectrons.

The configuration of the module is rather simple (see for example python/calibratedElectronsRun2_cfi.py in 76X version):

correctionType = "80Xapproval"
.......
calibratedPatElectrons = cms.EDProducer("CalibratedPatElectronProducerRun2",
                                        
                                        # input collections
                                        electrons = cms.InputTag('slimmedElectrons'),
                                        gbrForestName = cms.string("gedelectron_p4combination_25ns"),
                                        
                                        # data or MC corrections
                                        # if isMC is false, data corrections are applied
                                        isMC = cms.bool(False),
                                        
                                        # set to True to get special "fake" smearing for synchronization. Use JUST in case of synchronization
                                        isSynchronization = cms.bool(False),

                                        correctionFile = cms.string(files[correctionType])
                                        )

From the above snapshot it is clear that you just need to:

    choose the proper correctionType (see table below for available set of corrections)
    update (if needed) the electron source and the isMC flag 

"correctionType" to apply according to dataset/MC sample:
2015 	MC miniAODv1 (74X) 	MC miniAODv2 (76X)
Data Prompt 	Prompt2015 	-
Data ReReco 76X 	- 	76XReReco
Data prompt for approval 	- 	80Xapproval

* * Obsolete: to use the newest corrections evaluated on 7.65 fb-1* one should simply change the input files of the corrector modules (in python/calibratedElectronsRun2_cfi.py), from 80X_Golden22June_approval to 80X_DCS05July_plus_Golden22 and use the tag ICHEP2016_approval_7p65fb of the ScaleSmearings git package. 


