from PhysicsTools.Heppy.analyzers.core.Analyzer import Analyzer
from PhysicsTools.Heppy.analyzers.core.AutoHandle import AutoHandle
from CMGTools.VVResonances.tools.Pair import Pair
from CMGTools.VVResonances.tools.Singlet import Singlet
from PhysicsTools.HeppyCore.utils.deltar import *
#from CMGTools.VVResonances.tools.BTagEventWeights import *
import itertools
import ROOT
import os
import math

class Substructure(object):
    def __init__(self):
        pass

class XZZBoostJetAnalyzer(Analyzer):
    def __init__(self, cfg_ana, cfg_comp, looperName):
        super(XZZBoostJetAnalyzer,self).__init__(cfg_ana, cfg_comp, looperName)
        if hasattr(self.cfg_ana,"doPUPPI") and self.cfg_ana.doPUPPI:
            self.doPUPPI=True
            puppiJecCorrWeightFile = os.path.expandvars(self.cfg_ana.puppiJecCorrFile)
            self.puppiJecCorr = ROOT.TFile.Open(puppiJecCorrWeightFile)
            self.puppisd_corrGEN = self.puppiJecCorr.Get("puppiJECcorr_gen")
            self.puppisd_corrRECO_cen = self.puppiJecCorr.Get("puppiJECcorr_reco_0eta1v3")
            self.puppisd_corrRECO_for = self.puppiJecCorr.Get("puppiJECcorr_reco_1v3eta2v5")
        else:
            self.doPUPPI=False

        # No need to have a post fix corresponding to the postfix of DeepAKX producer
        # as the producer's postfix doesn't enter the discriminant name
        # postfix of DeepAKX module
        #self.postfixDeepAKX = "AK8WithDeepTags"
        #if hasattr(self.cfg_ana,"postfixDeepAKX") :
        #   self.postfixDeepAKX = self.cfg_ana.postfixDeepAKX

        #self.smearing=ROOT.TRandom(10101982)
        #btag reweighting
        #self.btagSF = BTagEventWeights('btagsf',os.path.expandvars(self.cfg_ana.btagCSVFile))

    #def declareHandles(self):
        #super(XZZBoostJetAnalyzer, self).declareHandles()
        #self.handles['jets']   = AutoHandle( self.cfg_ana.jetCol, 'std::vector<pat::Jet>' )
        #if self.cfg_comp.isMC:
           #self.handles['genJet'] = AutoHandle( self.cfg_ana.genJetCol, 'std::vector<reco::GenJet>' )

    def copyLV(self,LV):
        out=[]
        for i in LV:
            out.append(ROOT.math.XYZTLorentzVector(i.px(),i.py(),i.pz(),i.energy()))
        return out

    def substructure(self,jet,event,suffix = ""):
        #if we already filled it exit
        tag ='substructure'+suffix
        if hasattr(jet,tag):
           return
        
        substructure=Substructure()

        #without L1
        corrNoL1 = jet.corr
        #PUPPI JEC doesn't have L1
        if self.doPUPPI:
           corrNoL1 = jet.corr
        else :
           corrNoL1 = jet.corr/jet.CorrFactor_L1

        #SoftDrop subjets and softdrop puppi jet mass
        subjetAlgo="SoftDropPuppi"
        #getv the btag of the subjets
        nSubjets = 2
        #for o in jet.subjets(subjetAlgo):
        #   nSubjets = nSubjets + 1
        jet.subJetTags=[-99.0]*nSubjets
        jet.subJetCTagL=[-99.0]*nSubjets
        jet.subJetCTagB=[-99.0]*nSubjets
        jet.subJet_hadronFlavour=[-99.0]*nSubjets
        jet.subJet_partonFlavour=[-99.0]*nSubjets

        #Get soft Drop lorentzVector and subjets
        softDropJetUnCorr=ROOT.math.XYZTLorentzVector(0,0,0,0)
        iSubjets = 0
        for o in jet.subjets(subjetAlgo):
            puppi_softdrop_subjet=ROOT.math.XYZTLorentzVector(o.correctedP4(0).px(),o.correctedP4(0).py(),o.correctedP4(0).pz(),o.correctedP4(0).energy())
            softDropJetUnCorr=softDropJetUnCorr+puppi_softdrop_subjet

            if(iSubjets<2) :
              jet.subJetTags[iSubjets] = o.bDiscriminator(self.cfg_ana.bDiscriminator)
              #jet.subJetCTagL[iSubjets] = o.bDiscriminator(self.cfg_ana.cDiscriminatorL)
              #jet.subJetCTagB[iSubjets] = o.bDiscriminator(self.cfg_ana.cDiscriminatorB)
              jet.subJet_partonFlavour[iSubjets] = o.partonFlavour()
              jet.subJet_hadronFlavour[iSubjets] = o.hadronFlavour()
 
            iSubjets = iSubjets + 1

        jet.nSubjets = iSubjets #(jet.subjets(subjetAlgo)).size()

        #Get softdrop/pruned mass and corresponding corrector
        substructure.prunedJetMassCor = corrNoL1#self.getPrunedJetMassCor(jet,event)

        if self.doPUPPI:
            substructure.softDropJetMassCorr = self.getPUPPIMassWeight(softDropJetUnCorr)
            substructure.softDropJetMassBare = softDropJetUnCorr.mass()
            substructure.prunedJetMassCorr = corrNoL1
            substructure.prunedJetMassBare = jet.userFloat("ak8PFJetsCHSValueMap:ak8PFJetsCHSPrunedMass")
            print 'jet.phyObj',jet.physObj.userFloat("ak8PFJetsCHSValueMap:ak8PFJetsCHSPrunedMass")
            print 'jet',jet.userFloat("ak8PFJetsCHSValueMap:ak8PFJetsCHSPrunedMass")
        else :
            substructure.softDropJetMassBare = jet.userFloat("ak8PFJetsCHSValueMap:ak8PFJetsCHSSoftDropMass")
            substructure.softDropJetMassCorr = corrNoL1 
            substructure.prunedJetMassBare = jet.userFloat("ak8PFJetsCHSValueMap:ak8PFJetsCHSPrunedMass")
            substructure.prunedJetMassCorr = corrNoL1

        #Get NTau
        substructure.ntau = [-99.0]*4
        if self.doPUPPI:
           substructure.ntau[0] = jet.userFloat("NjettinessAK8Puppi:tau1")
           substructure.ntau[1] = jet.userFloat("NjettinessAK8Puppi:tau2")
           substructure.ntau[2] = jet.userFloat("NjettinessAK8Puppi:tau3")
           substructure.ntau[3] = jet.userFloat("NjettinessAK8Puppi:tau4")
        else : 
           substructure.ntau[0] = jet.userFloat("ak8PFJetsCHSValueMap:NjettinessAK8CHSTau1")
           substructure.ntau[1] = jet.userFloat("ak8PFJetsCHSValueMap:NjettinessAK8CHSTau2")
           substructure.ntau[2] = jet.userFloat("ak8PFJetsCHSValueMap:NjettinessAK8CHSTau3")

        # calculate DDT tau21 (currently without softDropJetMassCorr, but the L2L3 corrections)
        substructure.softDropJet = softDropJetUnCorr*substructure.softDropJetMassCorr

        substructure.tau21_DDT = 0
        if (substructure.softDropJet.mass() > 0):
            substructure.tau21_DDT = substructure.ntau[1]/substructure.ntau[0] + ( 0.063 * math.log( (substructure.softDropJet.mass()*substructure.softDropJet.mass())/substructure.softDropJet.pt()))
        setattr(jet,tag,substructure)    

        #DeepAK8
        '''
        print 'DeepAKX'
        pairDiscriVector = jet.physObj.getPairDiscri()
        for i in range(len(pairDiscriVector)) :
            print "pairDiscriVector ",pairDiscriVector[i].first
        '''
        pfMassDecoDeepAK8 = "pfMassDecorrelatedDeepBoostedDiscriminatorsJetTags"
        substructure.pfMassDecorrelated_ZHbbvsQCD = jet.physObj.bDiscriminator(pfMassDecoDeepAK8+":ZHbbvsQCD")
        substructure.pfMassDecorrelated_WvsQCD    = jet.physObj.bDiscriminator(pfMassDecoDeepAK8+":WvsQCD")
        substructure.pfMassDecorrelated_ZvsQCD    = jet.physObj.bDiscriminator(pfMassDecoDeepAK8+":ZvsQCD")
        #pfBoostedDoubleSecondaryVertexAK8BJetTags
        substructure.pfBoostedDoubleSecondaryVertexAK8BJetTags = jet.physObj.bDiscriminator("pfBoostedDoubleSecondaryVertexAK8BJetTags")

#    def substructureGEN(self,jet,event):
#    def topology(self,VV,jets,leptons):

    def selectJets(self,jets,func,otherObjects,DR,otherObjects2=None,DR2=0.0):
        output=[]
        for j in jets:
            if not func(j):
                continue
            overlap=False
            for o in otherObjects:
                dr=deltaR(j.eta(),j.phi(),o.eta(),o.phi())
                if dr<DR:
                    overlap=True
                    break;
            if otherObjects2 !=None:
                for o in otherObjects2:
                    dr=deltaR(j.eta(),j.phi(),o.eta(),o.phi())
                    if dr<DR2:
                        overlap=True
                        break;
            if not overlap:
                output.append(j)
        return output


    def getPUPPIMassWeight(self, puppijet):
        # mass correction for PUPPI following https://github.com/thaarres/PuppiSoftdropMassCorr
        genCorr = 1.
        recoCorr = 1.
        # corrections only valid up to |eta| < 2.5, use 1. beyond
        if (abs(puppijet.eta()) < 2.5):
            genCorr = self.puppisd_corrGEN.Eval(puppijet.pt())
            if (abs(puppijet.eta()) <= 1.3):
                recoCorr = self.puppisd_corrRECO_cen.Eval(puppijet.pt())
            else:
                recoCorr = self.puppisd_corrRECO_for.Eval(puppijet.pt())
        totalWeight = genCorr*recoCorr
        return totalWeight


    def process(self, event):
        self.readCollections( event.input )

        tightLeptonsForV = filter(lambda x: (abs(x.pdgId())==11 and x.pt()>35) or (abs(x.pdgId())==13 and x.pt()>20), event.selectedLeptons)
        #load pat jets
        fatJets=[]
        if self.doPUPPI:
           if hasattr(event,"jetsAK8PFpuppi") :      
              fatJets=self.selectJets(event.jetsAK8PFpuppi,lambda x: x.pt()>200.0 and x.jetID('POG_PFID_Loose2016'),tightLeptonsForV,0.8)
        else :
           if hasattr(event,"jetsAK8PFchs") :
              fatJets=self.selectJets(event.jetsAK8PFchs,lambda x: x.pt()>200.0 and x.jetID('POG_PFID_Loose2016'),tightLeptonsForV,0.8)         

        self.boostObjs = []
        #substructure
        for fatJet in fatJets:
            self.substructure(fatJet,event)
            if hasattr(fatJet,'substructure'):
               self.boostObjs.append(fatJet)

        setattr(event,'boostObjs'+self.cfg_ana.suffix,self.boostObjs)
