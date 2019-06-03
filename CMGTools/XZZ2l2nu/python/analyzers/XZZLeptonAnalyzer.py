from PhysicsTools.Heppy.analyzers.core.Analyzer import Analyzer
from PhysicsTools.Heppy.analyzers.core.AutoHandle import AutoHandle
from PhysicsTools.Heppy.physicsobjects.Electron import Electron
from PhysicsTools.Heppy.physicsobjects.Muon import Muon
from PhysicsTools.HeppyCore.utils.deltar import deltaR
import PhysicsTools.HeppyCore.framework.config as cfg
from PhysicsTools.Heppy.physicsutils.RochesterCorrections import rochcor
from PhysicsTools.Heppy.physicsutils.MuScleFitCorrector   import MuScleFitCorr
from PhysicsTools.Heppy.physicsutils.KalmanMuonCorrector  import KalmanMuonCorrector
from PhysicsTools.Heppy.physicsutils.ElectronCalibrator import Run2ElectronCalibrator
from PhysicsTools.HeppyCore.utils.deltar import * 
from PhysicsTools.Heppy.physicsutils.genutils import *
import ROOT
from ROOT import heppy

# bad muon tagger
from RecoMET.METFilters.badGlobalMuonTaggerFWLite import *

cmgMuonCleanerBySegments = heppy.CMGMuonCleanerBySegmentsAlgo()

class XZZLeptonAnalyzer( Analyzer ):
    
    def __init__(self, cfg_ana, cfg_comp, looperName ):
        super(XZZLeptonAnalyzer,self).__init__(cfg_ana,cfg_comp,looperName)

        self.muonUseTuneP = self.cfg_ana.muonUseTuneP if hasattr(self.cfg_ana,'muonUseTuneP') else False

        if self.cfg_ana.doMuonScaleCorrections:
            algo, options = self.cfg_ana.doMuonScaleCorrections
            if algo == "Kalman":
                corr = options['MC' if self.cfg_comp.isMC else 'Data']
                self.muonScaleCorrector = KalmanMuonCorrector(corr,
                                                    self.cfg_comp.isMC,
                                                    options['isSync'] if 'isSync' in options else False,
                                                    options['smearMode'] if 'smearMode' in options else "ebe")
            elif algo == "Rochester":
                print "WARNING: the Rochester correction in heppy is still from Run 1"
                self.muonScaleCorrector = RochesterCorrections()
            elif algo == "MuScleFit":
                print "WARNING: the MuScleFit correction in heppy is still from Run 1 (and probably no longer functional)"
                if options not in [ "prompt", "prompt-sync", "rereco", "rereco-sync" ]:
                    raise RuntimeError, 'MuScleFit correction name must be one of [ "prompt", "prompt-sync", "rereco", "rereco-sync" ] '
                    rereco = ("prompt" not in self.cfg_ana.doMuScleFitCorrections)
                    sync   = ("sync"       in self.cfg_ana.doMuScleFitCorrections)
                    self.muonScaleCorrector = MuScleFitCorr(cfg_comp.isMC, rereco, sync)
            else: raise RuntimeError, "Unknown muon scale correction algorithm"
        else:
            self.muonScaleCorrector = None

        if self.cfg_ana.doElectronScaleCorrections:
            conf = cfg_ana.doElectronScaleCorrections
            self.electronEnergyCalibrator = Run2ElectronCalibrator(
                conf['data'],
                conf['GBRForest'],
                cfg_comp.isMC,
                conf['isSync'] if 'isSync' in conf else False,
            )

        self.isEleCutBasedId = True
        self.isEleMvaId = False
        if self.cfg_ana.electronIDVersion.find("mva") or self.cfg_ana.electronIDVersion.find("MVA") :
           self.isEleCutBasedId = False
           self.isEleMvaId = True
    #----------------------------------------
    # DECLARATION OF HANDLES OF LEPTONS STUFF   
    #----------------------------------------
    def declareHandles(self):
        super(XZZLeptonAnalyzer, self).declareHandles()
        
        #leptons
        self.handles['muons'] = AutoHandle(self.cfg_ana.muons,"std::vector<pat::Muon>")            
        self.handles['electrons'] = AutoHandle(self.cfg_ana.electrons,"std::vector<pat::Electron>")            
        # packedCandidates
        self.handles['packedCandidates'] = AutoHandle( self.cfg_ana.packedCandidates, 'std::vector<pat::PackedCandidate>') 
        #rho for muons
        self.handles['rhoMu'] = AutoHandle( self.cfg_ana.rhoMuon, 'double')
        #rho for electrons
        self.handles['rhoEleMiniIso'] = AutoHandle( self.cfg_ana.rhoElectronMiniIso, 'double')
        #rho for electron pfIso
        self.handles['rhoElePfIso'] = AutoHandle( self.cfg_ana.rhoElectronPfIso, 'double')
        self.handles['rhoEleHLT'] = AutoHandle( 'fixedGridRhoFastjetCentralCalo', 'double')
        # ecal reco hits collections
        self.handles['ebhits'] = AutoHandle( ("reducedEgamma","reducedEBRecHits"),'edm::SortedCollection<EcalRecHit,edm::StrictWeakOrdering<EcalRecHit> >')
        self.handles['eehits'] = AutoHandle( ("reducedEgamma","reducedEERecHits"),'edm::SortedCollection<EcalRecHit,edm::StrictWeakOrdering<EcalRecHit> >')
        self.handles['eshits'] = AutoHandle( ("reducedEgamma","reducedESRecHits"),'edm::SortedCollection<EcalRecHit,edm::StrictWeakOrdering<EcalRecHit> >')

        # decide to filter events not passing lepton requirements
        self.do_filter = getattr(self.cfg_ana, 'do_filter', True)

        # use ISO
        self.applyIso = getattr(self.cfg_ana, 'applyIso', True)

        # electronIDVersion
        self.electronIDVersion = getattr(self.cfg_ana, 'electronIDVersion', 'cutBase') # can be cutBase or mva/MVA
        # electronIsoVersion
        self.electronIsoVersion = getattr(self.cfg_ana, 'electronIsoVersion', 'pfISO') # can be pfISO or miniISO

        # effective area
        self.ele_effectiveAreas = getattr(self.cfg_ana, 'ele_effectiveAreas', "Spring15_25ns_v1")
        self.mu_effectiveAreas  = getattr(self.cfg_ana, 'mu_effectiveAreas',  "Spring15_25ns_v1")

        self.miniIsolationPUCorr = self.cfg_ana.miniIsolationPUCorr
        if self.miniIsolationPUCorr == "weights":
            self.IsolationComputer = heppy.IsolationComputer(0.4)
        else:
            self.IsolationComputer = heppy.IsolationComputer()
        #if self.cfg_comp.isMC:
            #self.esfinput=ROOT.TFile(self.cfg_comp.eSFinput)
            #self.esfh2=self.esfinput.Get("EGamma_SF2D")

    def beginLoop(self, setup):
        super(XZZLeptonAnalyzer,self).beginLoop(setup)
        self.counters.addCounter('events')
        count = self.counters.counter('events')
        count.register('all events')
        count.register('pass events')
        count.register('pass 2mu events')
        count.register('pass 2el events')
        count.register('pass 2mu kin events')
        count.register('pass 2el kin events')
        count.register('pass 2mu kin+id events')
        count.register('pass 2el kin+id events')
        count.register('pass 2mu kin+id+iso events')
        count.register('pass 2el kin+id+iso events')
        count.register('pass 2mu kin+id+iso+acc events')
        count.register('pass 2el kin+id+iso+acc events')
        count.register('pass 1mu kin events')
        count.register('pass 1el kin events')
        count.register('pass 1mu kin+id events')
        count.register('pass 1el kin+id events')
        count.register('pass 1mu kin+id+iso events')
        count.register('pass 1el kin+id+iso events')


    #------------------
    # MAKE LEPTON LISTS
    #------------------
    def makeLeptons(self, event):
        event.selectedLeptons = []
        event.selectedMuons = []
        event.selectedElectrons = []
        event.otherLeptons = []
        
        #muons
        allmuons = self.makeAllMuons(event)

        #electrons        
        allelectrons = self.makeAllElectrons(event)
       
        #pass kin selection
        self.n_mu_passKin = len(allmuons)
        self.n_el_passKin = len(allelectrons)

        # select muons
        for mu in allmuons:
            if (mu.highPtID or mu.trackerHighPtID ) :
                self.n_mu_passId += 1
                minc=[i for i in allmuons if i != mu and (i.highPtID or i.trackerHighPtID ) and deltaR(mu.eta(),mu.phi(),i.eta(),i.phi())<0.3]
                mu.nminc=len(minc)
                for i in minc: 
                    if i.physObj.innerTrack().isNonnull():mu.trackerIso-=i.physObj.innerTrack().pt()
                mu.trackerIso/=mu.pt()
                if mu.trackerIso<0.1 or not self.applyIso:
                    event.selectedLeptons.append(mu)
                    event.selectedMuons.append(mu)
                    self.n_mu_passIso += 1
            else:
                event.otherLeptons.append(mu)
        #select electrons
        for ele in allelectrons:
            if  self.isEleCutBasedId and ele.cutBasedId_loose :
                self.n_el_passId += 1
                event.selectedLeptons.append(ele)
                event.selectedElectrons.append(ele)
                # iso is part of cutBased Id
                self.n_el_passIso += 1
            if  self.isEleMvaId and ele.mvaId_wpHZZ :
                self.n_el_passId += 1
                if self.applyIso :
                   event.selectedLeptons.append(ele)
                   event.selectedElectrons.append(ele)
                   self.n_el_passIso += 1
            else:
                event.otherLeptons.append(ele)        

        # set reverse to True to rank from higher to lower pt
        event.otherLeptons.sort(key = lambda l : l.pt(), reverse = True)
        event.selectedLeptons.sort(key = lambda l : l.pt(), reverse = True)
        event.selectedMuons.sort(key = lambda l : l.pt(), reverse = True)
        event.selectedElectrons.sort(key = lambda l : l.pt(), reverse = True)
        if self.cfg_comp.isMC:
            for i in event.selectedLeptons: i.hasgen=int(i.physObj.genParticle().__nonzero__())+int(i.physObj.genParticle().__nonzero__() and i.physObj.genParticle().isPromptFinalState())

    def makeAllMuons(self, event):
        """
               make a list of all muons, and apply basic corrections to them
        """

        allmuons = map( Muon, self.handles['muons'].product() )
        allmuons0 = self.handles['muons'].product() 
 
        # tag bad muons
        badGlobalMuonTagger = BadGlobalMuonTagger(True,20.0)
        
        # bad muons
        badMuons0 = badGlobalMuonTagger.badMuons(allmuons0,event.goodVertices)
        # store badMuons in event
        event.badMuons = map( Muon, badMuons0 )

        # flag out
        for mu in allmuons: 
            if mu.physObj in badMuons0: 
                mu.isBadMuon = 1
                #debug
                print "bad muon pt=",mu.pt(),",eta=",mu.eta(),",phi=",mu.phi()
            else:
                mu.isBadMuon = 0

        # debug
        event.hasBadMuon=0
        if len(badMuons0)>0: 
            event.hasBadMuon=1
            print "Have",len(badMuons0),"bad muons"

        # set options for muons to use default pt or TuneP pt
        for mu in allmuons: mu.setMuonUseTuneP(self.muonUseTuneP)

        # Muon scale and resolution corrections (if enabled)
        if self.muonScaleCorrector:
            #for idx,mu in enumerate(allmuons): print 'LeptonAnalyzer:: mu(',idx,'): before Corr, pT(mu) =',mu.pt() 
            self.muonScaleCorrector.correct_all(allmuons, event.run)
            #for idx,mu in enumerate(allmuons): print 'LeptonAnalyzer:: mu(',idx,'):  after Corr, pT(mu) =',mu.pt() 
       
        # Attach EAs for isolation:
        for mu in allmuons:
          mu.rho = float(self.handles['rhoMu'].product()[0])
          if self.mu_effectiveAreas == "Spring15_25ns_v1":
              aeta = abs(mu.eta())
              if   aeta < 0.800: mu.EffectiveArea03 = 0.0735
              elif aeta < 1.300: mu.EffectiveArea03 = 0.0619
              elif aeta < 2.000: mu.EffectiveArea03 = 0.0465
              elif aeta < 2.200: mu.EffectiveArea03 = 0.0433
              elif aeta < 2.500: mu.EffectiveArea03 = 0.0577
              else:              mu.EffectiveArea03 = 0.0000
          else: 
              raise RuntimeError,  "Unsupported value for mu_effectiveAreas, can use Spring15_25ns_v1"
 
        # calculate miniIso
        for mu in allmuons:
            mu.trackerIso=mu.physObj.isolationR03().sumPt
            
        # Attach the vertex to them, for dxy/dz calculation
        for mu in allmuons:
            mu.associatedVertex = event.goodVertices[0] if hasattr(event,"goodVertices") and len(event.goodVertices)>0 else event.vertices[0]

        # also do this for bad muons
        for mu in event.badMuons:
            mu.associatedVertex = event.goodVertices[0] if hasattr(event,"goodVertices") and len(event.goodVertices)>0 else event.vertices[0]

        # define muon id
        for mu in allmuons:
            mu.highPtID = mu.physObj.isHighPtMuon(mu.associatedVertex)
            mu.trackerHighPtID = mu.physObj.isTrackerMuon() \
                         and mu.physObj.track().isNonnull() \
                         and mu.physObj.innerTrack().isNonnull() \
                         and mu.physObj.numberOfMatchedStations()>1 \
                         and (mu.physObj.muonBestTrack().ptError()/mu.physObj.muonBestTrack().pt())<0.3 \
                         and abs(mu.physObj.muonBestTrack().dxy(mu.associatedVertex.position()))<0.2 \
                         and abs(mu.physObj.muonBestTrack().dz(mu.associatedVertex.position()))<0.5 \
                         and mu.physObj.innerTrack().hitPattern().numberOfValidPixelHits()>0 \
                         and mu.physObj.track().hitPattern().trackerLayersWithMeasurement()>5 


        return allmuons


    def makeAllElectrons(self, event):
        """
               make a list of all electrons, and apply basic corrections to them
        """
        allelectrons = map( Electron, self.handles['electrons'].product() )

        # pre-selection with kinematic cut
        allelectrons = [el for el in allelectrons if el.pt()>20.0 and abs(el.eta())<2.5]

        # fill EA for rho-corrected isolation
        for ele in allelectrons:
          if self.ele_effectiveAreas == "Spring15_25ns_v1":
              SCEta = abs(ele.superCluster().eta())
              ## ----- https://github.com/ikrav/cmssw/blob/egm_id_747_v2/RecoEgamma/ElectronIdentification/data/Spring15/effAreaElectrons_cone03_pfNeuHadronsAndPhotons_25ns.txt
              if   SCEta < 1.000: ele.EffectiveArea03 = 0.1752
              elif SCEta < 1.479: ele.EffectiveArea03 = 0.1862
              elif SCEta < 2.000: ele.EffectiveArea03 = 0.1411
              elif SCEta < 2.200: ele.EffectiveArea03 = 0.1534
              elif SCEta < 2.300: ele.EffectiveArea03 = 0.1903
              elif SCEta < 2.400: ele.EffectiveArea03 = 0.2243
              elif SCEta < 5.000: ele.EffectiveArea03 = 0.2687
              else:               ele.EffectiveArea03 = 0.0000
          elif self.ele_effectiveAreas == "Spring16_25ns_v1":
              SCEta = abs(ele.superCluster().eta())
              ## ----- https://github.com/ikrav/cmssw/blob/egm_id_747_v2/RecoEgamma/ElectronIdentification/data/Spring15/effAreaElectrons_cone03_pfNeuHadronsAndPhotons_25ns.txt
              if   SCEta < 1.000: ele.EffectiveArea03 = 0.1703
              elif SCEta < 1.479: ele.EffectiveArea03 = 0.1715
              elif SCEta < 2.000: ele.EffectiveArea03 = 0.1213
              elif SCEta < 2.200: ele.EffectiveArea03 = 0.1230
              elif SCEta < 2.300: ele.EffectiveArea03 = 0.1635
              elif SCEta < 2.400: ele.EffectiveArea03 = 0.1937
              else:              ele.EffectiveArea03 = 0.2393
              # warning: EAs not computed for cone DR=0.4 yet. Do not correct
              ele.EffectiveArea04 = 0.0

          else: 
              raise RuntimeError,  "Unsupported value for ele_effectiveAreas: can only use Data2012 (rho: ?), Phys14_v1 and Spring15_v1 (rho: fixedGridRhoFastjetAll)"

        # calculate miniIso and pfIso
        for ele in allelectrons:
            ele.rho = float(self.handles['rhoElePfIso'].product()[0])
            ele.rhoHLT = float(self.handles['rhoEleHLT'].product()[0])
            ele.relIsoea03=ele.absIsoWithFSR(0.3)/ele.pt()
            if abs(ele.physObj.superCluster().eta())<1.479:
                #ele.looseiso=True if ele.relIsoea03<0.0893 else False
                ele.looseiso=True if ele.relIsoea03<0.0994 else False
            else:
                #ele.looseiso=True if ele.relIsoea03<0.121 else False
                ele.looseiso=True if ele.relIsoea03<0.107 else False

        # Electron scale calibrations
        if self.cfg_ana.doElectronScaleCorrections:
            for idx,ele in enumerate(allelectrons):
                self.electronEnergyCalibrator.correct(ele, event.run)
        else:
            #void SimpleElectron::writeTo(reco::GsfElectron & out) const
            #DataFormats/EgammaCandidates/interface/GsfElectron.h
            for idx,ele in enumerate(allelectrons):
                print 'LeptonAnalyzer:: ele(',idx,'): before Corr, pT(e) =',ele.pt()
                ele.setCorrectedEcalEnergy(ele.userFloat("ecalEnergyPostCorr"))
                ele.setCorrectedEcalEnergyError(ele.userFloat("ecalEnergyErrPostCorr"))
                #auto corrP4  = pat::Electron::p4() * pat::Electron::userFloat("ecalTrkEnergyPostCorr") / pat::Electron::energy()
                newMomentum = ele.p4() * ele.userFloat("ecalTrkEnergyPostCorr") / ele.energy()
                newMomentumErr = ele.userFloat("ecalTrkEnergyErrPostCorr")
                ele.correctMomentum(newMomentum, ele.trackMomentumError(), newMomentumErr)
                print 'LeptonAnalyzer:: ele(',idx,'):  after Corr, pT(e) =',ele.pt()

        # Electron ecal cluster/crystals
        for ele in allelectrons:
            seedId = ele.superCluster().seed().seed()
            if seedId.subdetId()==1:
                for did in self.handles['ebhits'].product():
                    if did.id()==seedId: ele.seedXtal = did
            elif seedId.subdetId()==2: 
                for did in self.handles['eehits'].product():
                    if did.id()==seedId: ele.seedXtal = did
            else: print "Couldn't find seed Xtal!"

        # Attach the vertex
        for ele in allelectrons:
            ele.associatedVertex = event.goodVertices[0] if hasattr(event,"goodVertices") and len(event.goodVertices)>0 else event.vertices[0]

        # define electron ID
        for ele in allelectrons:
            ele.loose_nonISO = ele.id_passes("cutBasedElectronID-Fall17-94X-V2","loose")
            #same as ele.physObj.electronID("cutBasedElectronID_Fall17_94X_V2_loose")
            #but will print error message if the ID is not available
            ele.looseiso = ele.id_passes("cutBasedElectronID-Fall17-94X-V2","loose")
            print 'ele cut based Id ',ele.loose_nonISO,' ',ele.looseiso
            ele.cutBasedId_loose = ele.id_passes("cutBasedElectronID-Fall17-94X-V2","loose")

            # mva Id
            ele.mva_Iso = ele.mva_score("mvaEleID-Fall17-iso-V2",norm=True)
            ele.mva_nonIso = ele.mva_score("mvaEleID-Fall17-noIso-V2",norm=True)
            print 'ele mva iso ',ele.mva_Iso,', noniso ',ele.mva_nonIso
            ele.mvaId_wpHZZ = ele.id_passes("mvaEleID-Fall17-iso-V2","wpHZZ")
            ele.hltSafeId = ele.electronID("POG_Cuts_ID_SPRING16_25ns_v1_HLT")

        return allelectrons 


    def process(self, event):
        self.readCollections( event.input )
        self.counters.counter('events').inc('all events')

        # counters
        self.n_el_passKin=0
        self.n_mu_passKin=0
        self.n_el_passId=0
        self.n_mu_passId=0
        self.n_el_passIso=0
        self.n_mu_passIso=0

        #call the leptons functions
        self.makeLeptons(event)

        if self.n_mu_passId>=1:
            self.counters.counter('events').inc('pass 1mu kin+id events')
        if self.n_el_passId>=1:
            self.counters.counter('events').inc('pass 1el kin+id events')

        if self.n_mu_passIso>=1:
            self.counters.counter('events').inc('pass 1mu kin+id+iso events')
        if self.n_el_passIso>=1:
            self.counters.counter('events').inc('pass 1el kin+id+iso events')

        if self.n_mu_passKin>=1:
            self.counters.counter('events').inc('pass 1mu kin events')
        if self.n_el_passKin>=1:
            self.counters.counter('events').inc('pass 1el kin events')

        if self.n_mu_passKin>=2:
            self.counters.counter('events').inc('pass 2mu kin events')
        if self.n_el_passKin>=2:
            self.counters.counter('events').inc('pass 2el kin events')

        if self.n_mu_passId>=2:
            self.counters.counter('events').inc('pass 2mu kin+id events')
        if self.n_el_passId>=2:
            self.counters.counter('events').inc('pass 2el kin+id events')

        if self.n_mu_passIso>=2:
            self.counters.counter('events').inc('pass 2mu kin+id+iso events')
        if self.n_el_passIso>=2:
            self.counters.counter('events').inc('pass 2el kin+id+iso events')

        if len(event.selectedMuons)>=2:
            self.counters.counter('events').inc('pass 2mu events')
            if (event.selectedMuons[0].pt()>50.0 and abs(event.selectedMuons[0].eta())<2.4 and
                event.selectedMuons[1].pt()>20.0 and abs(event.selectedMuons[1].eta())<2.4):
                self.counters.counter('events').inc('pass 2mu kin+id+iso+acc events')
        if len(event.selectedElectrons)>=2 :
            self.counters.counter('events').inc('pass 2el events')
            if (event.selectedElectrons[0].pt()>115.0 and abs(event.selectedElectrons[0].eta())<2.5 and
                event.selectedElectrons[1].pt()>35.0 and abs(event.selectedElectrons[1].eta())<2.5):
                self.counters.counter('events').inc('pass 2el kin+id+iso+acc events')

        if self.do_filter:
            if len(event.selectedMuons)>=2 or len(event.selectedElectrons)>=2 :
                self.counters.counter('events').inc('pass events')
                return True
            else: 
                return False
        else:
            self.counters.counter('events').inc('pass events')
            return True

#A default config
setattr(XZZLeptonAnalyzer,"defaultConfig",cfg.Analyzer(
    verbose=False,
    class_object=XZZLeptonAnalyzer,
    # input collections
    muons='slimmedMuons',
    electrons='slimmedElectrons',
    packedCandidates = 'packedPFCandidates',
    muonUseTuneP= False,
    rhoMuon= 'fixedGridRhoFastjetCentralNeutral',
    rhoElectronMiniIso = 'fixedGridRhoFastjetCentralNeutral',
    rhoElectronPfIso = 'fixedGridRhoFastjetAll',
    electronIDVersion = 'cutBase', # can be cutBase or mva
    applyIso = True,
    electronIsoVersion = 'pfISO', # can be pfISO or miniISO
    mu_isoCorr = "rhoArea" ,
    ele_isoCorr = "rhoArea" ,
    mu_effectiveAreas = "Spring15_25ns_v1", 
    ele_effectiveAreas = "Spring15_25ns_v1",
    miniIsolationPUCorr = None, # Allowed options: 'rhoArea' (EAs for 03 cone scaled by R^2), 'deltaBeta', 
                                     # 'raw' (uncorrected), 'weights' (delta beta weights; not validated)
                                     # Choose None to just use the individual object's PU correction
    doMuonScaleCorrections = ( 'Kalman', {
        'MC': 'MC_80X_13TeV',
        'Data': 'DATA_80X_13TeV',
        'isSync': False,
        'smearMode': 'ebe'
    }), 
    doElectronScaleCorrections = {
        'data' : 'EgammaAnalysis/ElectronTools/data/ScalesSmearings/80X_ichepV1_2016_ele',
        'GBRForest': ('$CMSSW_BASE/src/CMGTools/XZZ2l2nu/data/GBRForest_data_ICHEP16_combined.root',
                      'gedelectron_p4combination_25ns'),
        'isSync': False
    },
    do_filter=True,
    )
)
