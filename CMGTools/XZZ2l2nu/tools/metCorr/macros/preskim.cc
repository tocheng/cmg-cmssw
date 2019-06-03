#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TH2D.h"
#include "TH2F.h"
#include "TH1F.h"
#include "TMath.h"
#include "TVector2.h"
#include "TGraphErrors.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <ctime>
#include "TROOT.h"
// Hengne Li 2017 @ cern

int main(int argc, char** argv) {

  if( argc<3 ) {
     std::cout << argv[0] << ":  " << std::endl ;
     std::cout << " Functionality: skimming... "  << std::endl;
     std::cout << "                 "  << std::endl;
     std::cout << " usage: " << argv[0] << " inputfile.root outputfile.root" << std::endl ;
     exit(1) ;
  }

  time_t now = time(0);
  char* dt = ctime(&now);
  std::cout << "Start time is: " << dt << std::endl;

  // input file name
  std::string inputfile((const char*)argv[1]); 
  // output file name
  std::string outputfile((const char*)argv[2]);
  // initialize
  // root files
  TFile* finput = TFile::Open(inputfile.c_str());
  TFile* foutput = TFile::Open(outputfile.c_str(), "recreate");

  // check if it is sm ZZ sample, based on file names
  bool isZZ = (inputfile.find("ZZTo2L2Nu")!=std::string::npos);
  // check if it is ZJets sample, based on file names
  bool isZJets = (inputfile.find("DYJets")!=std::string::npos)
               ||(inputfile.find("DY0Jets")!=std::string::npos)
               ||(inputfile.find("DY1Jets")!=std::string::npos)
               ||(inputfile.find("DY2Jets")!=std::string::npos)
               ||(inputfile.find("DY3Jets")!=std::string::npos)
               ||(inputfile.find("DY4Jets")!=std::string::npos)
               ;

  // tree
  TTree* tree = (TTree*)finput->Get("tree");

  Int_t isData;
  tree->SetBranchAddress("isData",&isData);
  // check if tree has events
  if (tree->GetEntries()<=0) return 0;
  // get isData info
  tree->GetEntry(0);

  // select branches
  tree->SetBranchStatus("lep_*",0);
  tree->SetBranchStatus("jet_*",0);
  tree->SetBranchStatus("vtx_*",0);
  //tree->SetBranchStatus("triggers*",0);
  tree->SetBranchStatus("nLL",0); 
  tree->SetBranchStatus("nLLNuNu",0);
  tree->SetBranchStatus("llnunu_deltaPhi",0);
  //tree->SetBranchStatus("llnunu_TuneP_deltaPhi",0);
  tree->SetBranchStatus("llnunu_deltaR",0);
  //tree->SetBranchStatus("llnunu_TuneP_deltaR",0);
  //tree->SetBranchStatus("llnunu_TuneP_LV_*",0);
  //tree->SetBranchStatus("llnunu_LV_*",0);
  //tree->SetBranchStatus("llnunu_CosdphiZMet",0);
  //tree->SetBranchStatus("llnunu_CosZdeltaPhi",0);
  //tree->SetBranchStatus("llnunu_dPTPara",0);
  //tree->SetBranchStatus("llnunu_dPTParaRel",0);
  //tree->SetBranchStatus("llnunu_dPTPerp",0);
  //tree->SetBranchStatus("llnunu_dPTPerpRel",0);
  //tree->SetBranchStatus("llnunu_metOvSqSET",0);
  //tree->SetBranchStatus("llnunu_l1_px",0);
  //tree->SetBranchStatus("llnunu_l1_py",0);
  //tree->SetBranchStatus("llnunu_l1_pz",0);
  //tree->SetBranchStatus("llnunu_l1_TuneP_usage",0);
  //tree->SetBranchStatus("llnunu_l1_l*_TuneP_type",0);
  tree->SetBranchStatus("llnunu_l1_l*_dxy",0);
  tree->SetBranchStatus("llnunu_l1_l*_dz",0);
  tree->SetBranchStatus("llnunu_l1_l*_edxy",0);
  tree->SetBranchStatus("llnunu_l1_l*_edz",0);
  tree->SetBranchStatus("llnunu_l1_l*_ip3d",0);
  //tree->SetBranchStatus("llnunu_l1_l*_miniRelIso",0);
  tree->SetBranchStatus("llnunu_l1_l*_muonincore03",0);
  //tree->SetBranchStatus("llnunu_l1_l*_heepV60_noISO",0);
  tree->SetBranchStatus("llnunu_l1_l*_looseelectron",0);
  tree->SetBranchStatus("llnunu_l1_l*_looseelectronnoiso",0);
  tree->SetBranchStatus("llnunu_l1_l*_looseisoelectron",0);
  //tree->SetBranchStatus("llnunu_l1_l*_lepsf",0);
  //tree->SetBranchStatus("llnunu_l1_l*_lepsfUp",0);
  //tree->SetBranchStatus("llnunu_l1_l*_lepsfLo",0);
  //tree->SetBranchStatus("llnunu_l1_l*_px",0);
  //tree->SetBranchStatus("llnunu_l1_l*_py",0);
  //tree->SetBranchStatus("llnunu_l1_l*_pz",0);
  tree->SetBranchStatus("llnunu_l1_l*_chargedHadRelIso03",0);
  tree->SetBranchStatus("llnunu_l1_l*_chargedHadRelIso04",0);
  tree->SetBranchStatus("llnunu_l1_l*_softMuonId",0);
  tree->SetBranchStatus("llnunu_l1_l*_pfMuonId",0);
  tree->SetBranchStatus("llnunu_l1_l*_eleCutId2012_full5x5",0);
  tree->SetBranchStatus("llnunu_l1_l*_trackerLayers",0);
  tree->SetBranchStatus("llnunu_l1_l*_pixelLayers",0);
  tree->SetBranchStatus("llnunu_l1_l*_trackerHits",0);
  tree->SetBranchStatus("llnunu_l1_l*_lostOuterHits",0);
  tree->SetBranchStatus("llnunu_l1_l*_innerTrackValidHitFraction",0);
  tree->SetBranchStatus("llnunu_l1_l*_innerTrackChi2",0);
  tree->SetBranchStatus("llnunu_l1_l*_nStations",0);
  tree->SetBranchStatus("llnunu_l1_l*_caloCompatibility",0);
  tree->SetBranchStatus("llnunu_l1_l*_globalTrackChi2",0);
  tree->SetBranchStatus("llnunu_l1_l*_trkKink",0);
  tree->SetBranchStatus("llnunu_l1_l*_segmentCompatibility",0);
  tree->SetBranchStatus("llnunu_l1_l*_chi2LocalPosition",0);
  tree->SetBranchStatus("llnunu_l1_l*_chi2LocalMomentum",0);
  tree->SetBranchStatus("llnunu_l1_l*_glbTrackProbability",0);
  tree->SetBranchStatus("llnunu_l1_l*_sigmaIEtaIEta",0);
  tree->SetBranchStatus("llnunu_l1_l*_dEtaScTrkIn",0);
  tree->SetBranchStatus("llnunu_l1_l*_dPhiScTrkIn",0);
  tree->SetBranchStatus("llnunu_l1_l*_hadronicOverEm",0);
  tree->SetBranchStatus("llnunu_l1_l*_eInvMinusPInv",0);
  tree->SetBranchStatus("llnunu_l1_l*_eInvMinusPInv_tkMom",0);
  tree->SetBranchStatus("llnunu_l1_l*_etaSc",0);
  //tree->SetBranchStatus("llnunu_l2_px",0);
  //tree->SetBranchStatus("llnunu_l2_py",0);
  //tree->SetBranchStatus("llnunu_l2_pz",0);
  tree->SetBranchStatus("llnunu_l2_eta",0);
  tree->SetBranchStatus("llnunu_l2_rapidity",0);
  tree->SetBranchStatus("llnunu_l2_mass",0);
  //tree->SetBranchStatus("llnunu_l2_metSig",0);
  //tree->SetBranchStatus("llnunu_l2_*smear*", 0);
  //tree->SetBranchStatus("llnunu_l2_*JER*",0);
  //tree->SetBranchStatus("llnunu_l2_*Smear*",0); 

  tree->SetBranchStatus("llnunu_l2_t1*",0); 
  if (!isData&&!isZJets){
    tree->SetBranchStatus("llnunu_l2_t1XYPt", 1); 
    tree->SetBranchStatus("llnunu_l2_t1XYPhi", 1); 
    tree->SetBranchStatus("llnunu_l2_t1XYSumEt", 1); 
    tree->SetBranchStatus("llnunu_l2_t1SmearPt", 1); 
    tree->SetBranchStatus("llnunu_l2_t1SmearPhi", 1); 
    tree->SetBranchStatus("llnunu_l2_t1SmearSumEt", 1); 
    tree->SetBranchStatus("llnunu_l2_t1Pt_*", 1); 
    tree->SetBranchStatus("llnunu_l2_t1Phi_*", 1); 
  }

  if (!isData) {
    //tree->SetBranchStatus("HLT_*",0); 
    tree->SetBranchStatus("genX_*",0); 
    tree->SetBranchStatus("genLep_*",0);
    tree->SetBranchStatus("genLepFsr_*",0);
    tree->SetBranchStatus("genjet_*",0);
    //tree->SetBranchStatus("genZ_*",0);
    //tree->SetBranchStatus("genZ_px",0); 
    //tree->SetBranchStatus("genZ_py",0); 
    //tree->SetBranchStatus("genZ_pz",0); 
    tree->SetBranchStatus("llnunu_l2_genEta",0);
    tree->SetBranchStatus("genQ_*",0);
    tree->SetBranchStatus("genNeu_*",0);
  }


  // add back jets info
  tree->SetBranchStatus("nlep", 1);
  tree->SetBranchStatus("njet", 1);
  tree->SetBranchStatus("nbadmuon", 1);
/*
  tree->SetBranchStatus("jet_rawPt",1);
  tree->SetBranchStatus("jet_eta",1);
  tree->SetBranchStatus("jet_phi",1);
  tree->SetBranchStatus("jet_area",1);
  tree->SetBranchStatus("jet_neutralEmEnergyFraction",1);
  tree->SetBranchStatus("jet_chargedEmEnergyFraction",1);
  tree->SetBranchStatus("jet_muonEnergyFraction",1);
  tree->SetBranchStatus("llnunu_l2_*smear*", 1);
  tree->SetBranchStatus("llnunu_l2_*JER*",1);
  tree->SetBranchStatus("llnunu_l2_*Smear*",1);
*/
  // add back special for ggZZ2l2nu
  if (isZZ) {
    tree->SetBranchStatus("genQ_pdgId",1);
    tree->SetBranchStatus("genX*",1);
    tree->SetBranchStatus("genLep*",1);
    tree->SetBranchStatus("genNeu*",1);
  }

  // remove other HLT only keep useful ones
  tree->SetBranchStatus("HLT_*", 0);
  tree->SetBranchStatus("HLT_MU", 1);
  tree->SetBranchStatus("HLT_MU50", 1);
  tree->SetBranchStatus("HLT_TkMU50", 1);
  tree->SetBranchStatus("HLT_ELE", 1);
  tree->SetBranchStatus("HLT_ELE115", 1);

  // alias
  tree->SetAlias("muselec", "(abs(llnunu_l1_l1_pdgId)==13&&abs(llnunu_l1_l2_pdgId)==13&&abs(llnunu_l1_l1_eta)<2.4&&abs(llnunu_l1_l2_eta)<2.4&&llnunu_l1_l1_pt>20&&llnunu_l1_l2_pt>20&&(llnunu_l1_l1_highPtID>0.99||llnunu_l1_l2_highPtID>0.99))");
  tree->SetAlias("elselec", "(abs(llnunu_l1_l1_pdgId)==11&&abs(llnunu_l1_l2_pdgId)==11&&abs(llnunu_l1_l1_eta)<2.5&&abs(llnunu_l1_l2_eta)<2.5&&llnunu_l1_l1_pt>20&&llnunu_l1_l2_pt>20)");

  if (isData){
    tree->SetAlias("metfilter", "(Flag_EcalDeadCellTriggerPrimitiveFilter&&Flag_HBHENoiseIsoFilter&&Flag_goodVertices&&Flag_HBHENoiseFilter&&Flag_globalTightHalo2016Filter&&Flag_eeBadScFilter&&Flag_BadPFMuonFilter&&Flag_BadChargedCandidateFilter&&Flag_noBadMuons)");
  }
  else{
    tree->SetAlias("metfilter", "(Flag_EcalDeadCellTriggerPrimitiveFilter&&Flag_HBHENoiseIsoFilter&&Flag_goodVertices&&Flag_HBHENoiseFilter&&Flag_globalTightHalo2016Filter&&Flag_eeBadScFilter&&Flag_BadPFMuonFilter&&Flag_BadChargedCandidateFilter)");
  }
  tree->SetAlias("selecv0", "(metfilter&&(muselec||elselec))");

  char ftmp1_name[1000];
  sprintf(ftmp1_name, "%s_tmp1.root", outputfile.c_str() );
  TFile* ftmp1 = TFile::Open(ftmp1_name, "recreate");
  TTree* tree_tmp1 = tree->CopyTree("selecv0");

  // remove further useless branches
  tree_tmp1->SetBranchStatus("Flag_*", 0);
  tree_tmp1->SetBranchStatus("Flag_hasBadMuon", 1);

  if (isData) {
    tree_tmp1->SetBranchStatus("llnunu_l1_l1_hasgen", 0);
    tree_tmp1->SetBranchStatus("llnunu_l1_l1_isfromX", 0);
    tree_tmp1->SetBranchStatus("llnunu_l1_l2_hasgen", 0);
    tree_tmp1->SetBranchStatus("llnunu_l1_l2_isfromX", 0);
    //tree_tmp1->SetBranchStatus("ngenjet", 0);
    //tree_tmp1->SetBranchStatus("ngenZ", 0);
    //tree_tmp1->SetBranchStatus("ngenX", 0);

  }



  foutput->cd();
  TTree* tree_out = tree_tmp1->CloneTree(-1);


  foutput->cd();
  tree_out->Write();
  foutput->Close();
  finput->Close();

  char line[1000];
  sprintf(line, ".! rm %s", ftmp1_name);
  gROOT->ProcessLine(line);


  now = time(0);
  dt = ctime(&now);
  std::cout << "End time is: " << dt << std::endl;

  return 0;

}



