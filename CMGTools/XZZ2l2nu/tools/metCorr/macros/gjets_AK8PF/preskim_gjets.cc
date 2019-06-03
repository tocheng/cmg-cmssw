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

bool biggerTree=false;
bool haloTree=true;

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

  // tree
  TTree* tree = (TTree*)finput->Get("tree");

  Int_t isData;
  tree->SetBranchAddress("isData",&isData);
  // check if tree has events
  if (tree->GetEntries()<=0) return 0;
  // get isData info
  tree->GetEntry(0);

  // select branches
//  tree->SetBranchStatus("photon_*",0);
// tree->SetBranchStatus("jet_*",0);
  tree->SetBranchStatus("lep_*",0);

//  tree->SetBranchStatus("gjet_l*_px",0);
//  tree->SetBranchStatus("gjet_l*_py",0);
//  tree->SetBranchStatus("gjet_l*_pz",0);
  tree->SetBranchStatus("gjet_l2_eta",0);
  tree->SetBranchStatus("gjet_l2_rapidity",0);
  tree->SetBranchStatus("gjet_l2_mass",0);
  tree->SetBranchStatus("gjet_dPT*",0); 
  tree->SetBranchStatus("gjet_deltaPhi",0); 
  tree->SetBranchStatus("gjet_CosDeltaPhi",0);
  tree->SetBranchStatus("gjet_metOvSqSET",0);
  tree->SetBranchStatus("gjet_l1_mass",0);
///////
//  tree->SetBranchStatus("gjet_l1_chHadIso",0);
//  tree->SetBranchStatus("gjet_l1_phIso",0);
//  tree->SetBranchStatus("gjet_l1_neuHadIso",0);
//  tree->SetBranchStatus("gjet_l1_e5x5",0);
//  tree->SetBranchStatus("gjet_l1_e3x3",0);
//  tree->SetBranchStatus("gjet_l1_e2x5",0);
//  tree->SetBranchStatus("gjet_l1_e1x5",0);
//  tree->SetBranchStatus("gjet_l1_eTop",0);
//  tree->SetBranchStatus("gjet_l1_eBottom",0);
//  tree->SetBranchStatus("gjet_l1_eLeft",0);
//  tree->SetBranchStatus("gjet_l1_eRight",0);
//  tree->SetBranchStatus("gjet_l1_eMax",0);
////
  //tree->SetBranchStatus("gjet_l2_metSig",0);

  tree->SetBranchStatus("gjet_l2_t1*",0);
  tree->SetBranchStatus("gjet_l2_t1P*",0);
//  tree->SetBranchStatus("gjet_l2_*smear*", 0);
//  tree->SetBranchStatus("gjet_l2_*JER*",0);
//  tree->SetBranchStatus("gjet_l2_*Smear*",0);
  

  if (!isData) {
    //tree->SetBranchStatus("HLT_*",0); 
    tree->SetBranchStatus("gjet_l1_mcMatchId",0);
    tree->SetBranchStatus("gjet_l1_mcPt",0);
    tree->SetBranchStatus("gjet_l1_mcEta",0);
    tree->SetBranchStatus("gjet_l1_mcPhi",0);
    tree->SetBranchStatus("gjet_l2_genPt",0);
    tree->SetBranchStatus("gjet_l2_t1Pt_*",1);
    tree->SetBranchStatus("gjet_l2_t1Phi_*",1);
  }

  tree->SetBranchStatus("nlep", 1);
  tree->SetBranchStatus("njet", 1);
  tree->SetBranchStatus("nbadmuon", 1);

  tree->SetAlias("absDeltaPhi", "fabs(TVector2::Phi_mpi_pi(gjet_l2_phi-gjet_l1_phi))");
  tree->SetAlias("metPara", "gjet_l2_pt*cos(gjet_l2_phi-gjet_l1_phi)");
  tree->SetAlias("metPerp", "gjet_l2_pt*sin(gjet_l2_phi-gjet_l1_phi)");
  tree->SetAlias("uPara", "-gjet_l2_pt*cos(gjet_l2_phi-gjet_l1_phi)-gjet_l1_pt");
  tree->SetAlias("uPerp", "-gjet_l2_pt*sin(gjet_l2_phi-gjet_l1_phi)");
  tree->SetAlias("ut", "sqrt(uPara*uPara+uPerp*uPerp)");
  tree->SetAlias("eta", "gjet_l1_eta");
  tree->SetAlias("phi", "gjet_l1_phi");
  tree->SetAlias("pt", "gjet_l1_pt");
  if (isData) { 
    //tree->SetAlias("metfilter", "(Flag_EcalDeadCellTriggerPrimitiveFilter&&Flag_HBHENoiseIsoFilter&&Flag_goodVertices&&Flag_HBHENoiseFilter&&Flag_globalTightHalo2016Filter&&Flag_eeBadScFilter&&Flag_BadPFMuonFilter&&Flag_BadChargedCandidateFilter&&Flag_noBadMuons)"); // for reminiaod
    tree->SetAlias("metfilter", "(Flag_EcalDeadCellTriggerPrimitiveFilter&&Flag_HBHENoiseIsoFilter&&Flag_goodVertices&&Flag_HBHENoiseFilter&&Flag_eeBadScFilter&&Flag_BadPFMuonFilter&&Flag_BadChargedCandidateFilter&&Flag_noBadMuons)"); // for select only beam halo
    //tree->SetAlias("metfilter", "(Flag_EcalDeadCellTriggerPrimitiveFilter&&Flag_HBHENoiseIsoFilter&&Flag_goodVertices&&Flag_HBHENoiseFilter&&Flag_globalTightHalo2016Filter&&Flag_eeBadScFilter&&Flag_BadPFMuonFilter&&Flag_BadChargedCandidateFilter)"); // for rereco
  }
  else {
    tree->SetAlias("metfilter", "(Flag_EcalDeadCellTriggerPrimitiveFilter&&Flag_HBHENoiseIsoFilter&&Flag_goodVertices&&Flag_HBHENoiseFilter&&Flag_globalTightHalo2016Filter&&Flag_eeBadScFilter&&Flag_BadPFMuonFilter&&Flag_BadChargedCandidateFilter)");
  }
  //tree->SetAlias("metfilter", "(Flag_EcalDeadCellTriggerPrimitiveFilter&&Flag_HBHENoiseIsoFilter&&Flag_goodVertices&&Flag_HBHENoiseFilter&&Flag_globalTightHalo2016Filter&&Flag_eeBadScFilter&&Flag_BadPFMuonFilter&&Flag_BadChargedCandidateFilter&&Flag_CSCTightHalo2015Filter)");
  //else tree->SetAlias("metfilter", "(Flag_EcalDeadCellTriggerPrimitiveFilter&&Flag_HBHENoiseIsoFilter&&Flag_goodVertices&&Flag_HBHENoiseFilter&&Flag_CSCTightHalo2015Filter)");
  


  tree->SetAlias("ieta", "gjet_l1_ieta");
  tree->SetAlias("iphi", "gjet_l1_iphi");

//  tree->SetAlias("flag3", "((pt>=60&&!(eta>-2.5&&eta<-1.4&&phi>2.4&&phi<3.2)&&!(eta>-2.5&&eta<-1.4&&phi>-0.9&&phi<0.9)&&!(eta>-2.5&&eta<-1.4&&phi>-3.2&&phi<-2.4)&&!(eta>1.4&&eta<2.5&&phi>2.4&&phi<3.2)&&!(eta>1.4&&eta<2.5&&phi>-0.9&&phi<0.9)&&!(eta>1.4&&eta<2.5&&phi>-3.2&&phi<-2.0)&&!(eta>-2.1&&eta<-1.8&&phi>1.2&&phi<1.6)&&!(eta>-2.0&&eta<-1.6&&phi>-2.1&&phi<-1.8)&&!(eta>-0.3&&eta<0.0&&phi>2.5&&phi<3.0)&&!(eta>0.0&&eta<0.3&&phi>2.8&&phi<3.2)&&!(eta>-0.2&&eta<0.3&&phi>0.2&&phi<0.8)&&!(eta>-0.5&&eta<-0.2&&phi>-2.4&&phi<2.0)&&!(eta>2.3&&eta<2.5&&phi>-1.5&&phi<-1.0))||(pt<60&&!(eta>-2.5&&eta<-2.2&&phi>-3.0&&phi<-2.6)&&!(eta>0&&eta<0.3&&phi>-2.4&&phi<-1.4)&&!(eta>-0.2&&eta<0.2&&phi>-3.2&&phi<-2.6)&&!(eta>2.3&&eta<2.5&&phi>-2.6&&phi<-2.0)))");
  tree->SetAlias("flag3", "(((gjet_l1_iphi>75||gjet_l1_iphi<25)&&fabs(gjet_l1_eta)>1.5)||(fabs(gjet_l1_eta)<1.5))");

  // EB flag
  //tree->SetAlias("flg1eb", "(fabs(eta)<1.47&&!((ieta==5&&iphi==41)||(ieta==-51&&iphi==196)||(ieta==56&&iphi==67)||(ieta==-45&&iphi==340)||(ieta==58&&iphi==74)||(ieta==79&&iphi==67)||(ieta==72&&iphi==67)||(ieta==4&&iphi==70)||(ieta==17&&iphi==290)||(ieta==-44&&iphi==133)||(ieta==13&&iphi==67)||(ieta==-24&&iphi==119)||(ieta==-84&&iphi==168)||(ieta==73&&iphi==299)||(ieta==49&&iphi==6)||(ieta==-21&&iphi==308)||(ieta==59&&iphi==180)||(ieta==2&&iphi==81)||(ieta==22&&iphi==138)))");
  //tree->SetAlias("flg1eb", "(fabs(eta)<1.47&&!((ieta==56&&iphi==67)||(ieta==-24&&iphi==119)||(ieta==72&&iphi==67)||(ieta==79&&iphi==67)||(ieta==-51&&iphi==196)||(ieta==-21&&iphi==308)||(ieta==1&&iphi==81)||(ieta==49&&iphi==6)||(ieta==58&&iphi==74)||(ieta==-31&&iphi==163)))";
  //tree->SetAlias("flg1eb", "(fabs(eta)<1.47&&!((ieta==56&&iphi==67)||(ieta==79&&iphi==67)||(ieta==-51&&iphi==196)||(ieta==72&&iphi==67)||(ieta==58&&iphi==74)||(ieta==-24&&iphi==119)||(ieta==-21&&iphi==308)||(ieta==49&&iphi==6)||(ieta==1&&iphi==81)||(ieta==5&&iphi==180)||(ieta==-31&&iphi==163)||(ieta==5&&iphi==305)||(ieta==14&&iphi==321)||(ieta==-2&&iphi==244)||(ieta==-7&&iphi==39)||(ieta==-18&&iphi==189)||(ieta==-12&&iphi==196)||(ieta==6&&iphi==140)||(ieta==-27&&iphi==41)||(ieta==-5&&iphi==183)||(ieta==13&&iphi==218)||(ieta==-4&&iphi==125)||(ieta==-10&&iphi==163)||(ieta==-4&&iphi==229)||(ieta==-11&&iphi==163)||(ieta==-16&&iphi==170)||(ieta==-84&&iphi==168)||(ieta==5&&iphi==307)||(ieta==-10&&iphi==245)||(ieta==8&&iphi==329)||(ieta==-25&&iphi==109)||(ieta==-4&&iphi==183)||(ieta==-11&&iphi==28)||(ieta==26&&iphi==262)||(ieta==4&&iphi==245)||(ieta==3&&iphi==69)||(ieta==3&&iphi==186)||(ieta==-3&&iphi==187)||(ieta==6&&iphi==228)||(ieta==-24&&iphi==176)||(ieta==-3&&iphi==229)||(ieta==2&&iphi==246)||(ieta==-32&&iphi==146)||(ieta==-11&&iphi==182)||(ieta==2&&iphi==237)||(ieta==-4&&iphi==144)||(ieta==-11&&iphi==183)||(ieta==7&&iphi==214)||(ieta==1&&iphi==93)||(ieta==23&&iphi==147)))");
  tree->SetAlias("flg1eb", "(fabs(eta)<1.47&&!((ieta==56&&iphi==67)||(ieta==-51&&iphi==196)||(ieta==79&&iphi==67)||(ieta==72&&iphi==67)||(ieta==58&&iphi==74)||(ieta==-24&&iphi==119)||(ieta==49&&iphi==6)||(ieta==-21&&iphi==308)||(ieta==1&&iphi==81)||(ieta==-31&&iphi==163)||(ieta==5&&iphi==180)||(ieta==14&&iphi==321)||(ieta==5&&iphi==305)||(ieta==6&&iphi==140)||(ieta==-18&&iphi==189)||(ieta==-2&&iphi==244)||(ieta==-12&&iphi==196)||(ieta==-7&&iphi==39)||(ieta==-10&&iphi==163)||(ieta==-4&&iphi==229)||(ieta==-27&&iphi==41)||(ieta==-11&&iphi==163)||(ieta==-16&&iphi==170)||(ieta==-5&&iphi==183)||(ieta==13&&iphi==218)||(ieta==-4&&iphi==125)||(ieta==-4&&iphi==183)||(ieta==-84&&iphi==168)||(ieta==5&&iphi==307)||(ieta==-10&&iphi==245)||(ieta==26&&iphi==262)||(ieta==8&&iphi==329)||(ieta==4&&iphi==245)||(ieta==-25&&iphi==109)||(ieta==-11&&iphi==28)||(ieta==2&&iphi==237)||(ieta==-3&&iphi==187)||(ieta==-4&&iphi==144)||(ieta==3&&iphi==186)||(ieta==-3&&iphi==229)||(ieta==2&&iphi==246)||(ieta==-4&&iphi==57)||(ieta==-32&&iphi==146)||(ieta==-11&&iphi==182)||(ieta==5&&iphi==24)||(ieta==-19&&iphi==185)||(ieta==7&&iphi==214)||(ieta==-4&&iphi==53)||(ieta==2&&iphi==66)||(ieta==23&&iphi==147)))");



  // EE+ flag
  //tree->SetAlias("flg1eep", "(eta>1.566&&!((ieta==55&&iphi==27)||(ieta==62&&iphi==30)||(ieta==36&&iphi==64)||(ieta==43&&iphi==31)||(ieta==46&&iphi==31)||(ieta==42&&iphi==68)||(ieta==63&&iphi==31)||(ieta==48&&iphi==33)||(ieta==61&&iphi==35)||(ieta==61&&iphi==31)||(ieta==43&&iphi==32)||(ieta==46&&iphi==33)||(ieta==62&&iphi==35)||(ieta==38&&iphi==65)||(ieta==41&&iphi==68)||(ieta==40&&iphi==69)||(ieta==43&&iphi==70)||(ieta==44&&iphi==70)||(ieta==55&&iphi==70)||(ieta==52&&iphi==17)||(ieta==48&&iphi==21)||(ieta==48&&iphi==27)||(ieta==56&&iphi==29)||(ieta==65&&iphi==29)||(ieta==61&&iphi==30)||(ieta==37&&iphi==68)||(ieta==43&&iphi==69)||(ieta==49&&iphi==71)||(ieta==48&&iphi==74)))");
  tree->SetAlias("flg1eep", "(eta>1.566)");
  //tree->SetAlias("flg1eep", "((eta>1.566)&&!((ieta==48&&iphi==76)||(ieta==52&&iphi==76)||(ieta==52&&iphi==79)||(ieta==52&&iphi==80)||(ieta==53&&iphi==76)||(ieta==59&&iphi==24)||(ieta==47&&iphi==76)||(ieta==49&&iphi==76)||(ieta==48&&iphi==77)||(ieta==52&&iphi==21)||(ieta==53&&iphi==22)||(ieta==45&&iphi==76)))");

  // EE- flag
  //tree->SetAlias("flg1eem", "(eta<-1.566&&!((ieta==44&&iphi==31)||(ieta==46&&iphi==31)||(ieta==47&&iphi==32)||(ieta==50&&iphi==31)||(ieta==61&&iphi==32)||(ieta==43&&iphi==33)||(ieta==37&&iphi==34)||(ieta==61&&iphi==34)||(ieta==37&&iphi==65)||(ieta==23&&iphi==21)||(ieta==59&&iphi==29)||(ieta==58&&iphi==30)||(ieta==46&&iphi==32)||(ieta==41&&iphi==33)||(ieta==46&&iphi==33)||(ieta==61&&iphi==35)||(ieta==38&&iphi==36)||(ieta==38&&iphi==64)||(ieta==68&&iphi==28)||(ieta==59&&iphi==30)||(ieta==36&&iphi==31)||(ieta==43&&iphi==32)||(ieta==42&&iphi==33)||(ieta==41&&iphi==34)||(ieta==59&&iphi==34)||(ieta==36&&iphi==35)||(ieta==37&&iphi==37)||(ieta==43&&iphi==69)||(ieta==58&&iphi==69)||(ieta==52&&iphi==74)||(ieta==36&&iphi==26)||(ieta==46&&iphi==27)||(ieta==49&&iphi==30)||(ieta==36&&iphi==32)||(ieta==37&&iphi==32)||(ieta==42&&iphi==32)||(ieta==38&&iphi==33)||(ieta==58&&iphi==33)||(ieta==63&&iphi==34)||(ieta==39&&iphi==65)||(ieta==37&&iphi==66)||(ieta==41&&iphi==68)||(ieta==59&&iphi==68)||(ieta==62&&iphi==70)||(ieta==49&&iphi==71)||(ieta==55&&iphi==71)||(ieta==51&&iphi==72)))");
  tree->SetAlias("flg1eem", "(eta<-1.566)");


  //tree->SetAlias("filter1", "(gjet_l1_sigmaIetaIeta>0.001&&gjet_l1_sigmaIphiIphi>0.001&&gjet_l1_SwissCross<0.95&&gjet_l1_mipTotE<4.9&&gjet_l1_time>-2.08&&gjet_l1_time<0.92)");
  tree->SetAlias("filter1", "(gjet_l1_sigmaIetaIeta>0.001&&gjet_l1_sigmaIphiIphi>0.001&&gjet_l1_SwissCross<0.95&&gjet_l1_mipTotE<4.9&&gjet_l1_time>-2.58&&gjet_l1_time<1.42)");
  if (haloTree) tree->SetAlias("filter1", "(gjet_l1_sigmaIetaIeta>0.001&&gjet_l1_sigmaIphiIphi>0.001&&gjet_l1_SwissCross<0.95&&gjet_l1_mipTotE>4.9)"); // select halo

  //TTree* tree_out = tree->CloneTree(-1);

  char ftmp1_name[1000];
  sprintf(ftmp1_name, "%s_tmp1.root", outputfile.c_str() );
  TFile* ftmp1 = TFile::Open(ftmp1_name, "recreate");
  TTree* tree_tmp1;
  TTree* tree_tmp2;
  TTree* tree_tmp3;


  std::string selec;

  if (biggerTree) {
    selec = "HLT_PHOTONIDISO&&ngjet==1&&Max$(jet_pt[]*jet_chargedEmEnergyFraction[])<10&&Max$(jet_pt[]*jet_muonEnergyFraction[])<10&&flag3&&nlep==0";
  }
  else {
    selec = "HLT_PHOTONIDISO&&metfilter&&ngjet==1&&Max$(jet_pt[]*jet_chargedEmEnergyFraction[])<10&&Max$(jet_pt[]*jet_muonEnergyFraction[])<10&&flag3&&filter1&&nlep==0";
    if (haloTree) selec = "HLT_PHOTONIDISO&&metfilter&&ngjet==1&&filter1&&flag3"; // select halo
  }
  

  std::cout << "tree:  " << tree->GetEntries() << " Entries" <<  std::endl;
 
  tree_tmp1 = tree->CopyTree(selec.c_str());

  std::cout << "tree_tmp1:  " << tree_tmp1->GetEntries() << " Entries" <<  std::endl;
 
  if (!biggerTree){
    if (!haloTree) tree_tmp1->SetBranchStatus("Flag_*",0);
//    tree_tmp1->SetBranchStatus("Flag_hasBadMuon", 1); // for reminiaod
    tree_tmp1->SetBranchStatus("Flag_CSCTightHalo2015Filter", 1); 
    tree_tmp1->SetBranchStatus("Flag_CSCTightHaloFilter", 1);
  }
  tree_tmp1->SetBranchStatus("HLT_*",0);
  tree_tmp1->SetBranchStatus("HLT_PHOTONIDISO", 1);
  tree_tmp1->SetBranchStatus("jet_*",0);
  tree_tmp1->SetBranchStatus("photon_*",0);
  tree_tmp1->SetBranchStatus("nphoton",1);
  tree_tmp1->SetBranchStatus("vtx_x",0);
  tree_tmp1->SetBranchStatus("vtx_y",0);
  //tree_tmp1->SetBranchStatus("jet_rawPt",1);
  //tree_tmp1->SetBranchStatus("jet_eta",1);
  //tree_tmp1->SetBranchStatus("jet_phi",1);
  //tree_tmp1->SetBranchStatus("jet_area",1);

  if (biggerTree){
    tree_tmp3 = tree_tmp1;
  }
  else {
    tree_tmp2 = tree_tmp1->CopyTree("((eta<-1.566)||flg1eb||flg1eep)");
    std::cout << "tree_tmp2:  " << tree_tmp2->GetEntries() << " Entries" <<  std::endl;
    tree_tmp3 = tree_tmp2->CopyTree("((eta>-1.566)||flg1eem)");
    std::cout << "tree_tmp3:  " << tree_tmp3->GetEntries() << " Entries" <<  std::endl;
  }

  foutput->cd();  

  TTree* tree_out;

  if (isData) tree_out = tree_tmp3->CopyTree("(gjet_l1_trigerob_HLTbit>>0&1&&gjet_l1_trigerob_pt<=30)||(gjet_l1_trigerob_HLTbit>>1&1&&gjet_l1_trigerob_pt<=36)||(gjet_l1_trigerob_HLTbit>>2&1&&gjet_l1_trigerob_pt<=50)||(gjet_l1_trigerob_HLTbit>>3&1&&gjet_l1_trigerob_pt<=75)||(gjet_l1_trigerob_HLTbit>>4&1&&gjet_l1_trigerob_pt<=90)||(gjet_l1_trigerob_HLTbit>>5&1&&gjet_l1_trigerob_pt<=120)||(gjet_l1_trigerob_HLTbit>>6&1&&gjet_l1_trigerob_pt<=165)||(gjet_l1_trigerob_HLTbit>>7&1&&gjet_l1_trigerob_pt<=10000000)");
  else tree_out = tree_tmp3->CloneTree(-1);

  std::cout << "tree_out:  " << tree_out->GetEntries() << " Entries" <<  std::endl;

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



