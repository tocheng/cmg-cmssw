#include "CommonTools/PileupAlgos/interface/PuppiContainer.h"
#include "fastjet/internal/base.hh"
#include "fastjet/FunctionOfPseudoJet.hh"
#include "Math/ProbFunc.h"
#include "TMath.h"
#include <iostream>
#include <math.h>
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/isFinite.h"

using namespace std;
using namespace fastjet;

PuppiContainer::PuppiContainer(const edm::ParameterSet &iConfig) {
  fApplyCHS        = iConfig.getParameter<bool>("applyCHS"); 
  fUseExp          = iConfig.getParameter<bool>("useExp");
  fPuppiWeightCut  = iConfig.getParameter<double>("MinPuppiWeight");
  std::vector<edm::ParameterSet> lAlgos = iConfig.getParameter<std::vector<edm::ParameterSet> >("algos"); 
  fNAlgos = lAlgos.size();
  for(unsigned int i0 = 0; i0 < lAlgos.size(); i0++) { 
    PuppiAlgo pPuppiConfig(lAlgos[i0]);
    fPuppiAlgo.push_back(pPuppiConfig);
  }
}

void PuppiContainer::initialize(const std::vector<RecoObj> &iRecoObjects) { 
    //Clear everything
  fRecoParticles.resize(0);
  fPFParticles  .resize(0);
  fChargedPV    .resize(0);
  fPupParticles .resize(0);
  fWeights      .resize(0);
  fVals.resize(0);
    //fChargedNoPV.resize(0);
    //Link to the RecoObjects
  fPVFrac = 0.; 
  fNPV    = 1.;
  fRecoParticles = iRecoObjects;
  for (unsigned int i = 0; i < fRecoParticles.size(); i++){
    fastjet::PseudoJet curPseudoJet;
    auto fRecoParticle = fRecoParticles[i];
    curPseudoJet.reset_PtYPhiM(fRecoParticle.pt,fRecoParticle.eta,fRecoParticle.phi,fRecoParticle.m);
    int puppi_register = 0;
    if(fRecoParticle.id == 0 or fRecoParticle.charge == 0)  puppi_register = 0; // zero is neutral hadron                                                         
    if(fRecoParticle.id == 1 and fRecoParticle.charge != 0) puppi_register = fRecoParticle.charge; // from PV use the                             
    if(fRecoParticle.id == 2 and fRecoParticle.charge != 0) puppi_register = fRecoParticle.charge+5; // from NPV use the charge as key +5 as key
    curPseudoJet.set_user_info( new PuppiUserInfo( puppi_register ) );
    // fill vector of pseudojets for internal references
    fPFParticles.push_back(curPseudoJet);
    //Take Charged particles associated to PV
    if(std::abs(fRecoParticle.id) == 1) fChargedPV.push_back(curPseudoJet);
    if(std::abs(fRecoParticle.id) >= 1 ) fPVFrac+=1.;
    //if((fRecoParticle.id == 0) && (inParticles[i].id == 2))  _genParticles.push_back( curPseudoJet);
    //if(fRecoParticle.id <= 2 && !(inParticles[i].pt < fNeutralMinE && fRecoParticle.id < 2)) _pfchsParticles.push_back(curPseudoJet); 
    //if(fRecoParticle.id == 3) _chargedNoPV.push_back(curPseudoJet);
    if(fNPV < fRecoParticle.vtxId) fNPV = fRecoParticle.vtxId;
  }
  fPVFrac = double(fChargedPV.size())/fPVFrac;
}
PuppiContainer::~PuppiContainer(){}

double PuppiContainer::goodVar(PseudoJet const &iPart,std::vector<PseudoJet> const &iParts, int iOpt,double iRCone) {
  double lPup = 0;
  lPup = var_within_R(iOpt,iParts,iPart,iRCone);
  return lPup;
}
double PuppiContainer::var_within_R(int iId, const vector<PseudoJet> & particles, const PseudoJet& centre, double R){
  if(iId == -1) return 1;
  fastjet::Selector sel = fastjet::SelectorCircle(R);
  sel.set_reference(centre);
  vector<PseudoJet> near_particles = sel(particles);
  double var = 0;
  //double lSumPt = 0;
  //if(iId == 1) for(unsigned int i=0; i<near_particles.size(); i++) lSumPt += near_particles[i].pt();
  for(unsigned int i=0; i<near_particles.size(); i++){
    double pDEta = near_particles[i].eta()-centre.eta();
    double pDPhi = std::abs(near_particles[i].phi()-centre.phi());
    if(pDPhi > 2.*M_PI-pDPhi) pDPhi =  2.*M_PI-pDPhi;
    double pDR2 = pDEta*pDEta+pDPhi*pDPhi;
    if(std::abs(pDR2)  <  0.0001) continue;
    if(iId == 0) var += (near_particles[i].pt()/pDR2);
    if(iId == 1) var += near_particles[i].pt();
    if(iId == 2) var += (1./pDR2);
    if(iId == 3) var += (1./pDR2);
    if(iId == 4) var += near_particles[i].pt();  
    if(iId == 5) var += (near_particles[i].pt() * near_particles[i].pt()/pDR2);
  }
  if(iId == 1) var += centre.pt(); //Sum in a cone
  if(iId == 0 && var != 0) var = log(var);
  if(iId == 3 && var != 0) var = log(var);
  if(iId == 5 && var != 0) var = log(var);
  return var;
}
//In fact takes the median not the average
void PuppiContainer::getRMSAvg(int iOpt,std::vector<fastjet::PseudoJet> const &iConstits,std::vector<fastjet::PseudoJet> const &iParticles,std::vector<fastjet::PseudoJet> const &iChargedParticles) { 
  for(unsigned int i0 = 0; i0 < iConstits.size(); i0++ ) { 
    double pVal = -1;
    //Calculate the Puppi Algo to use
    int  pPupId   = getPuppiId(iConstits[i0].pt(),iConstits[i0].eta());
    if(fPuppiAlgo[pPupId].numAlgos() <= iOpt) pPupId = -1;
    if(pPupId == -1) {fVals.push_back(-1); continue;}
    //Get the Puppi Sub Algo (given iteration)
    int  pAlgo    = fPuppiAlgo[pPupId].algoId   (iOpt); 
    bool pCharged = fPuppiAlgo[pPupId].isCharged(iOpt);
    double pCone  = fPuppiAlgo[pPupId].coneSize (iOpt);
    //Compute the Puppi Metric 
    if(!pCharged) pVal = goodVar(iConstits[i0],iParticles       ,pAlgo,pCone);
    if( pCharged) pVal = goodVar(iConstits[i0],iChargedParticles,pAlgo,pCone);
    fVals.push_back(pVal);
    //if(std::isnan(pVal) || std::isinf(pVal)) cerr << "====> Value is Nan " << pVal << " == " << iConstits[i0].pt() << " -- " << iConstits[i0].eta() << endl;
    if( ! edm::isFinite(pVal)) {
      LogDebug( "NotFound" )  << "====> Value is Nan " << pVal << " == " << iConstits[i0].pt() << " -- " << iConstits[i0].eta() << endl;
      continue;
    }
    fPuppiAlgo[pPupId].add(iConstits[i0],pVal,iOpt);
  }
  for(int i0 = 0; i0 < fNAlgos; i0++) fPuppiAlgo[i0].computeMedRMS(iOpt,fPVFrac);
}
int    PuppiContainer::getPuppiId( float iPt, float iEta) { 
  int lId = -1; 
  for(int i0 = 0; i0 < fNAlgos; i0++) { 
    if(std::abs(iEta) < fPuppiAlgo[i0].etaMin()) continue;
    if(std::abs(iEta) > fPuppiAlgo[i0].etaMax()) continue;
    if(iPt        < fPuppiAlgo[i0].ptMin())  continue;
    lId = i0; 
    break;
  }
  //if(lId == -1) std::cerr << "Error : Full fiducial range is not defined " << std::endl;
  return lId;
}
double PuppiContainer::getChi2FromdZ(double iDZ) { 
  //We need to obtain prob of PU + (1-Prob of LV)
  // Prob(LV) = Gaus(dZ,sigma) where sigma = 1.5mm  (its really more like 1mm)
  //double lProbLV = ROOT::Math::normal_cdf_c(std::abs(iDZ),0.2)*2.; //*2 is to do it double sided
  //Take iDZ to be corrected by sigma already
  double lProbLV = ROOT::Math::normal_cdf_c(std::abs(iDZ),1.)*2.; //*2 is to do it double sided
  double lProbPU = 1-lProbLV;
  if(lProbPU <= 0) lProbPU = 1e-16;   //Quick Trick to through out infs
  if(lProbPU >= 0) lProbPU = 1-1e-16; //Ditto
  double lChi2PU = TMath::ChisquareQuantile(lProbPU,1);
  lChi2PU*=lChi2PU;
  return lChi2PU;
}
std::vector<double> const & PuppiContainer::puppiWeights() {
  fPupParticles .resize(0);
  fWeights      .resize(0);
  fVals         .resize(0);
  for(int i0 = 0; i0 < fNAlgos; i0++) fPuppiAlgo[i0].reset();

    int lNMaxAlgo = 1;
    for(int i0 = 0; i0 < fNAlgos; i0++) lNMaxAlgo = std::max(fPuppiAlgo[i0].numAlgos(),lNMaxAlgo);
    //Run through all compute mean and RMS
    int lNParticles    = fRecoParticles.size();
  for(int i0 = 0; i0 < lNMaxAlgo; i0++) { 
    getRMSAvg(i0,fPFParticles,fPFParticles,fChargedPV);
  }
  std::vector<double> pVals;
  for(int i0 = 0; i0 < lNParticles; i0++) {
    //Refresh
    pVals.clear();
    double pWeight = 1;
    //Get the Puppi Id and if ill defined move on
    int  pPupId   = getPuppiId(fRecoParticles[i0].pt,fRecoParticles[i0].eta);
    if(pPupId == -1) {
     fWeights .push_back(pWeight);
     continue;
   }
      // fill the p-values
   double pChi2   = 0;
   if(fUseExp){ 
	//Compute an Experimental Puppi Weight with delta Z info (very simple example)
     pChi2 = getChi2FromdZ(fRecoParticles[i0].dZ);
	//Now make sure Neutrals are not set
     if(fRecoParticles[i0].pfType > 3) pChi2 = 0;
   }
      //Fill and compute the PuppiWeight
   int lNAlgos = fPuppiAlgo[pPupId].numAlgos();
   for(int i1 = 0; i1 < lNAlgos; i1++) pVals.push_back(fVals[lNParticles*i1+i0]);
    pWeight = fPuppiAlgo[pPupId].compute(pVals,pChi2);
      //Apply the CHS weights
    if(fRecoParticles[i0].id == 1 && fApplyCHS ) pWeight = 1;
    if(fRecoParticles[i0].id == 2 && fApplyCHS ) pWeight = 0;
      //Basic Weight Checks
    if( ! edm::isFinite(pWeight)) {
      pWeight = 0.0;
      LogDebug("PuppiWeightError") << "====> Weight is nan : " << pWeight << " : pt " << fRecoParticles[i0].pt << " -- eta : " << fRecoParticles[i0].eta << " -- Value" << fVals[i0] << " -- id :  " << fRecoParticles[i0].id << " --  NAlgos: " << lNAlgos << std::endl;
    }
    //Basic Cuts      
    if(pWeight                         < fPuppiWeightCut) pWeight = 0;  //==> Elminate the low Weight stuff
    if(pWeight*fPFParticles[i0].pt()   < fPuppiAlgo[pPupId].neutralPt(fNPV) && fRecoParticles[i0].id == 0 ) pWeight = 0;  //threshold cut on the neutral Pt
    fWeights .push_back(pWeight);
    //Now get rid of the thrown out weights for the particle collection
    if(std::abs(pWeight) < std::numeric_limits<double>::denorm_min() ) continue;
      //Produce
    PseudoJet curjet( pWeight*fPFParticles[i0].px(), pWeight*fPFParticles[i0].py(), pWeight*fPFParticles[i0].pz(), pWeight*fPFParticles[i0].e());
    curjet.set_user_index(i0);
    fPupParticles.push_back(curjet);
  }
  return fWeights;
}

