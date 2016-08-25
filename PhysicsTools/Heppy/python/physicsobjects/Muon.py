from PhysicsTools.Heppy.physicsobjects.Lepton import Lepton
from PhysicsTools.HeppyCore.utils.deltar import deltaR
from ctypes import c_double
import ROOT

class Muon( Lepton ):

    def __init__(self, *args, **kwargs):
        super(Muon, self).__init__(*args, **kwargs)
        self._trackForDxyDz = "muonBestTrack"
        self._muonUseTuneP = False
        # store pf muon and tunep muon 4vectors for inter-change
        self._pf_muon_p4 = ROOT.math.PtEtaPhiMLorentzVector(self.physObj.pt(),
                        self.physObj.eta(),
                        self.physObj.phi(),
                        self.physObj.mass())
        self._tunep_muon_p4 = ROOT.math.PtEtaPhiMLorentzVector(self.physObj.tunePMuonBestTrack().pt(),
                        self.physObj.tunePMuonBestTrack().eta(),
                        self.physObj.tunePMuonBestTrack().phi(),
                        self.physObj.mass())
        # pt err
        self._pf_ptErr = self.physObj.bestTrack().ptError()
        self._tunep_ptErr = self.physObj.tunePMuonBestTrack().ptError()       

    def setMuonUseTuneP(self,muonUseTuneP=True):
        # Comments by Hengne Li:
        # restore the corresponding p4 when pfmuon and tunep muon are inter-changed
        # only do the exchange when necessary.. 
        if muonUseTuneP and not self._muonUseTuneP:
            self.physObj.setP4(self._tunep_muon_p4)
        elif not muonUseTuneP and self._muonUseTuneP:
            self.physObj.setP4(self._pf_muon_p4)
        self._muonUseTuneP = muonUseTuneP

    def setP4(self,newP4):
        # Comments by Hengne Li:
        #  when you re-calib the p4, will apply to the corrent physObj,
        # and will also change the pfmuon/tunepmuon internal p4 accordingly
        # e.g. if you re-calib muon when muonUseTuneP is True, it will also
        # apply the re-calib to the internal tunepmuon, but will NOT change
        # the internal pfmuon p4. 
        # So, if you want to recalib both, you need to setMuonUseTuneP(True/False),
        #  and calibrate once more.
        # and due to tricky problems, one has to redefine a PtEtaPhiMLorentzVector 
        self.physObj.setP4(newP4)
        newP4 = ROOT.math.PtEtaPhiMLorentzVector(self.physObj.pt(),
                        self.physObj.eta(),
                        self.physObj.phi(),
                        self.physObj.mass())
        if self._muonUseTuneP: self._tunep_muon_p4 = newP4
        else: self._pf_muon_p4 = newP4

    def setPz(self,newPz):
        # Comments by Hengne Li:
        # same comments in setP4 applies here. 
        # no setPx/y needed, since reco::Candidate doesn't have those functions.
        # and due to tricky problems, one has to redefine a PtEtaPhiMLorentzVector
        #  instead of the ROOT::Math::LorentzVector<xxx>::SetPz(xx), which doesn't 
        #  work for unknown reasons so far...  
        self.physObj.setPz(newPz)
        newP4 = ROOT.math.PtEtaPhiMLorentzVector(self.physObj.pt(),
                        self.physObj.eta(),
                        self.physObj.phi(),
                        self.physObj.mass())
        if self._muonUseTuneP: self._tunep_muon_p4 = newP4
        else: self._pf_muon_p4 = newP4

    def setTrackForDxyDz(self,what):
        if not hasattr(self,what):
            raise RuntimeError("I don't have a track called "+what)
        self._trackForDxyDz = what

    def looseId( self ):
        '''Loose ID as recommended by mu POG.'''
        return self.physObj.isLooseMuon()

    def tightId( self ):
        '''Tight ID as recommended by mu POG 
        (unless redefined in the lepton analyzer).

        If not using the LeptonAnalyzer, make sure to set self.associatedVertex, 
        that is necessary for tight muon identification. 
        '''
        return getattr(self,"tightIdResult",self.muonID("POG_ID_Tight"))

    def muonID(self, name, vertex=None):
        if name == "" or name is None: 
            return True
        if name.startswith("POG_"):
            if name == "POG_ID_Loose": return self.physObj.isLooseMuon()
            if vertex is None:
                vertex = getattr(self, 'associatedVertex', None)
            if name == "POG_ID_Tight":  return self.physObj.isTightMuon(vertex)
            if name == "POG_ID_HighPt": return self.physObj.isHighPtMuon(vertex)
            if name == "POG_ID_Soft":   return self.physObj.isSoftMuon(vertex)
            if name == "POG_ID_TightNoVtx":  return self.looseId() and \
                                                 self.isGlobalMuon() and \
                                                 self.globalTrack().normalizedChi2() < 10 and \
                                                 self.globalTrack().hitPattern().numberOfValidMuonHits() > 0 and \
                                                 self.numberOfMatchedStations()>1 and \
                                                 self.innerTrack().hitPattern().numberOfValidPixelHits()>0 and \
                                                 self.innerTrack().hitPattern().trackerLayersWithMeasurement() > 5
            if name == "POG_ID_Medium":
                if not self.looseId(): return False
                goodGlb = self.physObj.isGlobalMuon() and self.physObj.globalTrack().normalizedChi2() < 3 and self.physObj.combinedQuality().chi2LocalPosition < 12 and self.physObj.combinedQuality().trkKink < 20;
                return self.physObj.innerTrack().validFraction() > 0.8 and self.physObj.segmentCompatibility() >= (0.303 if goodGlb else 0.451)
            if name == "POG_ID_Medium_ICHEP":
                #validFraction() > 0.49 changed from 0.8
                if not self.looseId(): return False
                goodGlb = self.physObj.isGlobalMuon() and self.physObj.globalTrack().normalizedChi2() < 3 and self.physObj.combinedQuality().chi2LocalPosition < 12 and self.physObj.combinedQuality().trkKink < 20;
                return self.physObj.innerTrack().validFraction() > 0.49 and self.physObj.segmentCompatibility() >= (0.303 if goodGlb else 0.451)

            if name == "POG_Global_OR_TMArbitrated":
                return self.physObj.isGlobalMuon() or (self.physObj.isTrackerMuon() and self.physObj.numberOfMatchedStations() > 0)
        elif name.startswith("HZZ_"):
            if name == "HZZ_ID_TkHighPt":
                primaryVertex = vertex if vertex != None else getattr(self, 'associatedVertex', None) 
                return ( self.physObj.numberOfMatchedStations() > 1 
                         and (self.physObj.muonBestTrack().ptError()/self.physObj.muonBestTrack().pt()) < 0.3 
                         and abs(self.physObj.muonBestTrack().dxy(primaryVertex.position())) < 0.2 
                         and abs(self.physObj.muonBestTrack().dz(primaryVertex.position())) < 0.5 
                         and self.physObj.innerTrack().hitPattern().numberOfValidPixelHits() > 0 
                         and self.physObj.innerTrack().hitPattern().trackerLayersWithMeasurement() > 5 )
            if name == "HZZ_ID_LooseOrTkHighPt":
                if self.physObj.isLooseMuon(): return True
                return self.physObj.pt() > 200 and self.muonID("HZZ_ID_TkHighPt")
        return self.physObj.muonID(name)
            
    def mvaId(self):
        '''For a transparent treatment of electrons and muons. Returns -99'''
        return -99
    

    def dxy(self, vertex=None):
        '''either pass the vertex, or set associatedVertex before calling the function.
        note: the function does not work with standalone muons as innerTrack
        is not available.
        '''
        if vertex is None:
            vertex = self.associatedVertex
        return getattr(self,self._trackForDxyDz)().dxy( vertex.position() )

    def edxy(self):
        '''returns the uncertainty on dxy (from gsf track)'''
        return getattr(self,self._trackForDxyDz)().dxyError()
 

    def dz(self, vertex=None):
        '''either pass the vertex, or set associatedVertex before calling the function.
        note: the function does not work with standalone muons as innerTrack
        is not available.
        '''
        if vertex is None:
            vertex = self.associatedVertex
        return getattr(self,self._trackForDxyDz)().dz( vertex.position() )

    def edz(self):
        '''returns the uncertainty on dxz (from gsf track)'''
        return getattr(self,self._trackForDxyDz)().dzError()

    def chargedHadronIsoR(self,R=0.4):
        if   R == 0.3: return self.physObj.pfIsolationR03().sumChargedHadronPt 
        elif R == 0.4: return self.physObj.pfIsolationR04().sumChargedHadronPt 
        raise RuntimeError("Muon chargedHadronIso missing for R=%s" % R)

    def neutralHadronIsoR(self,R=0.4):
        if   R == 0.3: return self.physObj.pfIsolationR03().sumNeutralHadronEt 
        elif R == 0.4: return self.physObj.pfIsolationR04().sumNeutralHadronEt 
        raise RuntimeError("Muon neutralHadronIso missing for R=%s" % R)

    def photonIsoR(self,R=0.4):
        if   R == 0.3: return self.physObj.pfIsolationR03().sumPhotonEt 
        elif R == 0.4: return self.physObj.pfIsolationR04().sumPhotonEt 
        raise RuntimeError("Muon photonIso missing for R=%s" % R)

    def chargedAllIsoR(self,R=0.4):
        if   R == 0.3: return self.physObj.pfIsolationR03().sumChargedParticlePt 
        elif R == 0.4: return self.physObj.pfIsolationR04().sumChargedParticlePt 
        raise RuntimeError("Muon chargedAllIso missing for R=%s" % R)

    def chargedAllIso(self):
        return self.chargedAllIsoR()

    def puChargedHadronIsoR(self,R=0.4):
        if   R == 0.3: return self.physObj.pfIsolationR03().sumPUPt 
        elif R == 0.4: return self.physObj.pfIsolationR04().sumPUPt 
        raise RuntimeError("Muon chargedHadronIso missing for R=%s" % R)


    def absIsoWithFSR(self, R=0.4, puCorr="deltaBeta", dBetaFactor=0.5):
        '''
        Calculate Isolation, subtract FSR, apply specific PU corrections" 
        '''
        photonIso = self.photonIsoR(R)
        if hasattr(self,'fsrPhotons'):
            for gamma in self.fsrPhotons:
                dr = deltaR(gamma.eta(), gamma.phi(), self.physObj.eta(), self.physObj.phi())
                if dr > 0.01 and dr < R:
                    photonIso = max(photonIso-gamma.pt(),0.0)                
        if puCorr == "deltaBeta":
            offset = dBetaFactor * self.puChargedHadronIsoR(R)
        elif puCorr == "rhoArea":
            offset = self.rho*getattr(self,"EffectiveArea"+(str(R).replace(".","")))
        elif puCorr in ["none","None",None]:
            offset = 0
        else:
             raise RuntimeError("Unsupported PU correction scheme %s" % puCorr)
        return self.chargedHadronIsoR(R)+max(0.,photonIso+self.neutralHadronIsoR(R)-offset)            

    def ptErr(self):
        # Comments by Hengne Li:
        # KalmanMuonCorrector changed accordingly, use setPtErr(xx) to set this value
        #if "_ptErr" in self.__dict__: return self.__dict__['_ptErr']
        if self._muonUseTuneP: return self._tunep_ptErr
        else: return self._pf_ptErr

    def setPtErr(self,newPtErr):
        if self._muonUseTuneP: self._tunep_ptErr = newPtErr
        else: self._pf_ptErr = newPtErr

    def TuneP_pt(self):
        return self.physObj.tunePMuonBestTrack().pt()

    def TuneP_eta(self):
        return self.physObj.tunePMuonBestTrack().eta()

    def TuneP_phi(self):
        return self.physObj.tunePMuonBestTrack().phi()

    def TuneP_m(self):
        return .105

    def TuneP_ptErr(self):
        return self.physObj.tunePMuonBestTrack().ptError()

    def TuneP_type(self):
        return self.physObj.tunePMuonBestTrackType()





 
