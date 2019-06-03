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

bool lightWeight=true;

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

  if(tree->FindBranch("etrgsf"))  tree->SetBranchStatus("etrgsf",0);
  if(tree->FindBranch("mtrgsf"))  tree->SetBranchStatus("mtrgsf",0);
  if(tree->FindBranch("etrgsf_err"))  tree->SetBranchStatus("etrgsf_err",0);
  if(tree->FindBranch("mtrgsf_err"))  tree->SetBranchStatus("mtrgsf_err",0);
  // out_tree
  TTree* tree_out = tree->CloneTree(0);

  TFile* ftrg = TFile::Open("trigeff_el.root");
  TH2D* mul1pteta=(TH2D*)ftrg->Get("mul1pteta");
  TH2D* mul2pteta=(TH2D*)ftrg->Get("mul2pteta");
  TH2D* ell1pteta=(TH2D*)ftrg->Get("ell1pteta");

  TFile* ftrg_mu = TFile::Open("trigeff_mu.root");
  TH2D* htrg_l1_tot = (TH2D*)ftrg_mu->Get("htrg_l1_tot");
  TH2D* htrg_l2_tot = (TH2D*)ftrg_mu->Get("htrg_l2_tot");
  TH2D* htrg_l1_l1p = (TH2D*)ftrg_mu->Get("htrg_l1_l1p");
  TH2D* htrg_l2_l1p = (TH2D*)ftrg_mu->Get("htrg_l2_l1p");
  TH2D* htrg_l1_l1f = (TH2D*)ftrg_mu->Get("htrg_l1_l1f");
  TH2D* htrg_l2_l1f = (TH2D*)ftrg_mu->Get("htrg_l2_l1f");
  TH2D* htrg_l1_l1pl2f = (TH2D*)ftrg_mu->Get("htrg_l1_l1pl2f");
  TH2D* htrg_l1_l1pl2p = (TH2D*)ftrg_mu->Get("htrg_l1_l1pl2p");
  TH2D* htrg_l1_l1fl2p = (TH2D*)ftrg_mu->Get("htrg_l1_l1fl2p");
  TH2D* htrg_l2_l1pl2f = (TH2D*)ftrg_mu->Get("htrg_l2_l1pl2f");
  TH2D* htrg_l2_l1pl2p = (TH2D*)ftrg_mu->Get("htrg_l2_l1pl2p");
  TH2D* htrg_l2_l1fl2p = (TH2D*)ftrg_mu->Get("htrg_l2_l1fl2p");

  Double_t Ntrg_tot;
  Double_t Ntrg_tot_err;
  Double_t Ntrg_l1pl2f;
  Double_t Ntrg_l1pl2f_err;
  Double_t Ntrg_l1pl2p;
  Double_t Ntrg_l1pl2p_err;
  Double_t Ntrg_l1fl2p;
  Double_t Ntrg_l1fl2p_err;
  Double_t Ntrg_l1p;
  Double_t Ntrg_l1p_err;
  Double_t Ntrg_l1f;
  Double_t Ntrg_l1f_err;

  Int_t trg_NPtBins = htrg_l2_tot->GetNbinsX();
  Int_t trg_NEtaBins = htrg_l2_tot->GetNbinsY();
  Ntrg_tot = htrg_l2_tot->IntegralAndError(Int_t(1), trg_NPtBins, Int_t(1), trg_NEtaBins, Ntrg_tot_err);
  Ntrg_l1p = htrg_l2_l1p->IntegralAndError(Int_t(1), trg_NPtBins, Int_t(1), trg_NEtaBins, Ntrg_l1p_err);
  Ntrg_l1f = htrg_l2_l1f->IntegralAndError(Int_t(1), trg_NPtBins, Int_t(1), trg_NEtaBins, Ntrg_l1f_err);
  Ntrg_l1pl2f = htrg_l2_l1pl2f->IntegralAndError(Int_t(1), trg_NPtBins, Int_t(1), trg_NEtaBins, Ntrg_l1pl2f_err);
  Ntrg_l1pl2p = htrg_l2_l1pl2p->IntegralAndError(Int_t(1), trg_NPtBins, Int_t(1), trg_NEtaBins, Ntrg_l1pl2p_err);
  Ntrg_l1fl2p = htrg_l2_l1fl2p->IntegralAndError(Int_t(1), trg_NPtBins, Int_t(1), trg_NEtaBins, Ntrg_l1fl2p_err);

  TH2D* htrg_l1_tot_norm = (TH2D*)htrg_l1_tot->Clone("htrg_l1_tot_norm");
  TH2D* htrg_l2_tot_norm = (TH2D*)htrg_l2_tot->Clone("htrg_l2_tot_norm");
  TH2D* htrg_l1_l1p_norm = (TH2D*)htrg_l1_l1p->Clone("htrg_l1_l1p_norm");
  TH2D* htrg_l1_l1f_norm = (TH2D*)htrg_l1_l1f->Clone("htrg_l1_l1f_norm");
  TH2D* htrg_l2_l1p_norm = (TH2D*)htrg_l2_l1p->Clone("htrg_l2_l1p_norm");
  TH2D* htrg_l2_l1f_norm = (TH2D*)htrg_l2_l1f->Clone("htrg_l2_l1f_norm");
  TH2D* htrg_l1_l1pl2f_norm = (TH2D*)htrg_l1_l1pl2f->Clone("htrg_l1_l1pl2f_norm");
  TH2D* htrg_l1_l1pl2p_norm = (TH2D*)htrg_l1_l1pl2p->Clone("htrg_l1_l1pl2p_norm");
  TH2D* htrg_l1_l1fl2p_norm = (TH2D*)htrg_l1_l1fl2p->Clone("htrg_l1_l1fl2p_norm");
  TH2D* htrg_l2_l1pl2f_norm = (TH2D*)htrg_l2_l1pl2f->Clone("htrg_l2_l1pl2f_norm");
  TH2D* htrg_l2_l1pl2p_norm = (TH2D*)htrg_l2_l1pl2p->Clone("htrg_l2_l1pl2p_norm");
  TH2D* htrg_l2_l1fl2p_norm = (TH2D*)htrg_l2_l1fl2p->Clone("htrg_l2_l1fl2p_norm");

  htrg_l1_tot_norm->Scale(1./Ntrg_tot);
  htrg_l2_tot_norm->Scale(1./Ntrg_tot);
  htrg_l1_l1p_norm->Scale(1./Ntrg_l1p);
  htrg_l1_l1f_norm->Scale(1./Ntrg_l1f);
  htrg_l2_l1p_norm->Scale(1./Ntrg_l1p);
  htrg_l2_l1f_norm->Scale(1./Ntrg_l1f);
  htrg_l1_l1pl2f_norm->Scale(1./Ntrg_l1pl2f);
  htrg_l1_l1pl2p_norm->Scale(1./Ntrg_l1pl2p);
  htrg_l1_l1fl2p_norm->Scale(1./Ntrg_l1fl2p);
  htrg_l2_l1pl2f_norm->Scale(1./Ntrg_l1pl2f);
  htrg_l2_l1pl2p_norm->Scale(1./Ntrg_l1pl2p);
  htrg_l2_l1fl2p_norm->Scale(1./Ntrg_l1fl2p);

  TH2D* htrg_l1_l1p_norm_vs_tot    = (TH2D*)htrg_l1_l1p_norm->Clone("htrg_l1_l1p_norm_vs_tot");
  TH2D* htrg_l1_l1f_norm_vs_tot    = (TH2D*)htrg_l1_l1f_norm->Clone("htrg_l1_l1f_norm_vs_tot");
  TH2D* htrg_l2_l1p_norm_vs_tot    = (TH2D*)htrg_l2_l1p_norm->Clone("htrg_l2_l1p_norm_vs_tot");
  TH2D* htrg_l2_l1f_norm_vs_tot    = (TH2D*)htrg_l2_l1f_norm->Clone("htrg_l2_l1f_norm_vs_tot");
  TH2D* htrg_l1_l1pl2f_norm_vs_tot = (TH2D*)htrg_l1_l1pl2f_norm->Clone("htrg_l1_l1pl2f_norm_vs_tot");
  TH2D* htrg_l1_l1pl2p_norm_vs_tot = (TH2D*)htrg_l1_l1pl2p_norm->Clone("htrg_l1_l1pl2p_norm_vs_tot");
  TH2D* htrg_l1_l1fl2p_norm_vs_tot = (TH2D*)htrg_l1_l1fl2p_norm->Clone("htrg_l1_l1fl2p_norm_vs_tot");
  TH2D* htrg_l2_l1pl2f_norm_vs_tot = (TH2D*)htrg_l2_l1pl2f_norm->Clone("htrg_l2_l1pl2f_norm_vs_tot");
  TH2D* htrg_l2_l1pl2p_norm_vs_tot = (TH2D*)htrg_l2_l1pl2p_norm->Clone("htrg_l2_l1pl2p_norm_vs_tot");
  TH2D* htrg_l2_l1fl2p_norm_vs_tot = (TH2D*)htrg_l2_l1fl2p_norm->Clone("htrg_l2_l1fl2p_norm_vs_tot");
  TH2D* htrg_l1_l1pl2f_norm_vs_l1p = (TH2D*)htrg_l1_l1pl2f_norm->Clone("htrg_l1_l1pl2f_norm_vs_l1p");
  TH2D* htrg_l1_l1pl2p_norm_vs_l1p = (TH2D*)htrg_l1_l1pl2p_norm->Clone("htrg_l1_l1pl2p_norm_vs_l1p");
  TH2D* htrg_l1_l1fl2p_norm_vs_l1f = (TH2D*)htrg_l1_l1fl2p_norm->Clone("htrg_l1_l1fl2p_norm_vs_l1f");
  TH2D* htrg_l2_l1pl2f_norm_vs_l1p = (TH2D*)htrg_l2_l1pl2f_norm->Clone("htrg_l2_l1pl2f_norm_vs_l1p");
  TH2D* htrg_l2_l1pl2p_norm_vs_l1p = (TH2D*)htrg_l2_l1pl2p_norm->Clone("htrg_l2_l1pl2p_norm_vs_l1p");
  TH2D* htrg_l2_l1fl2p_norm_vs_l1f = (TH2D*)htrg_l2_l1fl2p_norm->Clone("htrg_l2_l1fl2p_norm_vs_l1f");

  htrg_l1_l1p_norm_vs_tot->Divide(htrg_l1_tot_norm);
  htrg_l1_l1f_norm_vs_tot->Divide(htrg_l1_tot_norm);
  htrg_l2_l1p_norm_vs_tot->Divide(htrg_l2_tot_norm);
  htrg_l2_l1f_norm_vs_tot->Divide(htrg_l2_tot_norm);
  htrg_l1_l1pl2f_norm_vs_tot->Divide(htrg_l1_tot_norm);
  htrg_l1_l1pl2p_norm_vs_tot->Divide(htrg_l1_tot_norm);
  htrg_l1_l1fl2p_norm_vs_tot->Divide(htrg_l1_tot_norm);
  htrg_l2_l1pl2f_norm_vs_tot->Divide(htrg_l2_tot_norm);
  htrg_l2_l1pl2p_norm_vs_tot->Divide(htrg_l2_tot_norm);
  htrg_l2_l1fl2p_norm_vs_tot->Divide(htrg_l2_tot_norm);
  htrg_l1_l1pl2f_norm_vs_l1p->Divide(htrg_l1_l1p_norm);
  htrg_l1_l1pl2p_norm_vs_l1p->Divide(htrg_l1_l1p_norm);
  htrg_l1_l1fl2p_norm_vs_l1f->Divide(htrg_l1_l1f_norm);
  htrg_l2_l1pl2f_norm_vs_l1p->Divide(htrg_l2_l1p_norm);
  htrg_l2_l1pl2p_norm_vs_l1p->Divide(htrg_l2_l1p_norm);
  htrg_l2_l1fl2p_norm_vs_l1f->Divide(htrg_l2_l1f_norm);

  Double_t effdt1a,effdt2a,effmc1a,effmc2a,errdt1a,errdt2a,errmc1a,errmc2a,effdt1,effdt2,effmc1,effmc2,errdt1,errdt2,errmc1,errmc2,etrgsfall,etrgsfallerr,mtrgsfall,mtrgsfallerr,temp1,temp2;
  Double_t etrgsfall_up,etrgsfall_dn,mtrgsfall_up,mtrgsfall_dn;
  TBranch *b_etrgsfall=tree_out->Branch("etrgsf",&etrgsfall,"etrgsf/D");
  TBranch *b_mtrgsfall=tree_out->Branch("mtrgsf",&mtrgsfall,"mtrgsf/D");
  TBranch *b_etrgsfallerr=tree_out->Branch("etrgsf_err",&etrgsfallerr,"etrgsf_err/D");
  TBranch *b_mtrgsfallerr=tree_out->Branch("mtrgsf_err",&mtrgsfallerr,"mtrgsf_err/D");
  TBranch *b_etrgsfall_up=tree_out->Branch("etrgsf_up",&etrgsfall_up,"etrgsf_up/D");
  TBranch *b_etrgsfall_dn=tree_out->Branch("etrgsf_dn",&etrgsfall_dn,"etrgsf_dn/D");
  TBranch *b_mtrgsfall_up=tree_out->Branch("mtrgsf_up",&mtrgsfall_up,"mtrgsf_up/D");
  TBranch *b_mtrgsfall_dn=tree_out->Branch("mtrgsf_dn",&mtrgsfall_dn,"mtrgsf_dn/D");

  Float_t llnunu_l1_l1_phi, llnunu_l1_l1_eta, llnunu_l1_l2_phi, llnunu_l1_l2_eta,llnunu_l1_l1_pt,llnunu_l1_l2_pt,llnunu_l1_l1_eSCeta,llnunu_l1_l2_eSCeta;
  Int_t llnunu_l1_l1_pdgId, llnunu_l1_l2_pdgId;
  tree->SetBranchAddress("llnunu_l1_l1_pdgId",&llnunu_l1_l1_pdgId);
  tree->SetBranchAddress("llnunu_l1_l2_pdgId",&llnunu_l1_l2_pdgId);
  tree->SetBranchAddress("llnunu_l1_l1_phi",&llnunu_l1_l1_phi);
  tree->SetBranchAddress("llnunu_l1_l2_phi",&llnunu_l1_l2_phi);
  tree->SetBranchAddress("llnunu_l1_l1_pt",&llnunu_l1_l1_pt);
  tree->SetBranchAddress("llnunu_l1_l2_pt",&llnunu_l1_l2_pt);
  tree->SetBranchAddress("llnunu_l1_l1_eta",&llnunu_l1_l1_eta);
  tree->SetBranchAddress("llnunu_l1_l2_eta",&llnunu_l1_l2_eta);
  tree->SetBranchAddress("llnunu_l1_l1_eSCeta",&llnunu_l1_l1_eSCeta);
  tree->SetBranchAddress("llnunu_l1_l2_eSCeta",&llnunu_l1_l2_eSCeta);

  for (int i=0; i<(int)tree->GetEntries(); i++){
    if (i%10000==0) std::cout << " Events " << i << std::endl;
    tree->GetEntry(i);
    int trg_bin_l1 = htrg_l1_l1p_norm_vs_tot->FindBin(llnunu_l1_l1_pt,fabs(llnunu_l1_l1_eta));
    int trg_bin_l2 = htrg_l2_l1pl2f_norm_vs_l1p->FindBin(llnunu_l1_l2_pt,fabs(llnunu_l1_l2_eta));
    double trg_sc_l1_l1p_vs_tot = htrg_l1_l1p_norm_vs_tot->GetBinContent(trg_bin_l1);
    double trg_sc_l2_l1pl2f_vs_l1p = htrg_l2_l1pl2f_norm_vs_l1p->GetBinContent(trg_bin_l2);
    double trg_sc_l2_l1pl2p_vs_l1p = htrg_l2_l1pl2p_norm_vs_l1p->GetBinContent(trg_bin_l2);
    double trg_sc_l2_l1fl2p_vs_tot = htrg_l2_l1fl2p_norm_vs_tot->GetBinContent(trg_bin_l2);
    double trg_sc_l1_l1p_vs_tot_err = htrg_l1_l1p_norm_vs_tot->GetBinError(trg_bin_l1);
    double trg_sc_l2_l1pl2f_vs_l1p_err = htrg_l2_l1pl2f_norm_vs_l1p->GetBinError(trg_bin_l2);
    double trg_sc_l2_l1pl2p_vs_l1p_err = htrg_l2_l1pl2p_norm_vs_l1p->GetBinError(trg_bin_l2);
    double trg_sc_l2_l1fl2p_vs_tot_err = htrg_l2_l1fl2p_norm_vs_tot->GetBinError(trg_bin_l2);

    double trg_npass = Ntrg_l1pl2f*trg_sc_l1_l1p_vs_tot*trg_sc_l2_l1pl2f_vs_l1p
                       + Ntrg_l1pl2p*trg_sc_l1_l1p_vs_tot*trg_sc_l2_l1pl2p_vs_l1p
                       + Ntrg_l1fl2p*trg_sc_l2_l1fl2p_vs_tot
                       ;
    double trg_npass_err = pow(Ntrg_l1pl2f_err*trg_sc_l1_l1p_vs_tot*trg_sc_l2_l1pl2f_vs_l1p,2)
                           + pow(Ntrg_l1pl2f*trg_sc_l1_l1p_vs_tot_err*trg_sc_l2_l1pl2f_vs_l1p,2)
                           + pow(Ntrg_l1pl2f*trg_sc_l1_l1p_vs_tot*trg_sc_l2_l1pl2f_vs_l1p_err,2)
                           + pow(Ntrg_l1pl2p_err*trg_sc_l1_l1p_vs_tot*trg_sc_l2_l1pl2p_vs_l1p,2)
                           + pow(Ntrg_l1pl2p*trg_sc_l1_l1p_vs_tot_err*trg_sc_l2_l1pl2p_vs_l1p,2)
                           + pow(Ntrg_l1pl2p*trg_sc_l1_l1p_vs_tot*trg_sc_l2_l1pl2p_vs_l1p_err,2)
                           + pow(Ntrg_l1fl2p_err*trg_sc_l2_l1fl2p_vs_tot,2)
                           + pow(Ntrg_l1fl2p*trg_sc_l2_l1fl2p_vs_tot_err,2)
                           ;
    trg_npass_err = sqrt(trg_npass_err);

    double trg_nfail = Ntrg_tot-trg_npass;
    double trg_nfail_err = sqrt(Ntrg_tot_err*Ntrg_tot_err-Ntrg_l1pl2f_err*Ntrg_l1pl2f_err-Ntrg_l1pl2p_err*Ntrg_l1pl2p_err-Ntrg_l1fl2p_err*Ntrg_l1fl2p_err);

    double trg_eff = trg_npass/(trg_npass+trg_nfail);
    double trg_eff_err = (pow(trg_nfail*trg_npass_err,2)+pow(trg_npass*trg_nfail_err,2))/pow(trg_npass+trg_nfail,4);
    trg_eff_err = sqrt(trg_eff_err);

    double trg_eff_up = trg_eff+0.5*trg_eff_err;
    double trg_eff_dn = trg_eff-0.5*trg_eff_err;

    if (trg_eff>=1) trg_eff=1; 
    if (trg_eff<=0) trg_eff=0; 
    if (trg_eff_up>=1) trg_eff_up=1;
    if (trg_eff_dn>=1) trg_eff_dn=1;
    if (trg_eff_up<=0) trg_eff_up=0;
    if (trg_eff_dn<=0) trg_eff_dn=0;
    trg_eff_err = fabs(trg_eff_up-trg_eff_dn);

    mtrgsfall = trg_eff;
    mtrgsfallerr = trg_eff_err;
    mtrgsfall_up = trg_eff_up;
    mtrgsfall_dn = trg_eff_dn;

    etrgsfall=ell1pteta->GetBinContent(ell1pteta->FindBin(llnunu_l1_l1_pt,abs(llnunu_l1_l1_eta)))/100;
    etrgsfallerr=ell1pteta->GetBinError(ell1pteta->FindBin(llnunu_l1_l1_pt,abs(llnunu_l1_l1_eta)))/100;
    etrgsfall_up = etrgsfall+0.5*etrgsfallerr;
    etrgsfall_dn = etrgsfall-0.5*etrgsfallerr;
    if (etrgsfall>=1) etrgsfall=1;
    if (etrgsfall<=0) etrgsfall=0;
    if (etrgsfall_up>=1) etrgsfall_up=1;
    if (etrgsfall_up<=0) etrgsfall_up=0;
    if (etrgsfall_dn>=1) etrgsfall_dn=1;
    if (etrgsfall_dn<=0) etrgsfall_dn=0;
    etrgsfallerr = fabs(etrgsfall_up-etrgsfall_dn);     

    tree_out->Fill();
  }

  foutput->cd();
  tree_out->Write();
  foutput->Close();
  finput->Close();


  now = time(0);
  dt = ctime(&now);
  std::cout << "End time is: " << dt << std::endl;

  return 0;

}

