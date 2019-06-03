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

  if( argc<4 ) {
     std::cout << argv[0] << ":  " << std::endl ;
     std::cout << " Functionality: skimming... "  << std::endl;
     std::cout << "                 "  << std::endl;
     std::cout << " usage: " << argv[0] << " inputfile.root trigtreefile.root outputfile.root" << std::endl ;
     exit(1) ;
  }

  time_t now = time(0);
  char* dt = ctime(&now);
  std::cout << "Start time is: " << dt << std::endl;

  // input file name
  std::string inputfile((const char*)argv[1]); 
  // trg tree file name
  std::string trigfile((const char*)argv[2]); 
  // output file name
  std::string outputfile((const char*)argv[3]);
  // initialize
  // root files
  TFile* finput = TFile::Open(inputfile.c_str());
  TFile* ftrig = TFile::Open(trigfile.c_str());
  TFile* foutput = TFile::Open(outputfile.c_str(), "recreate");
  // tree
  TTree* tree = (TTree*)finput->Get("tree");
  TTree* tree_trig = (TTree*)ftrig->Get("tree");

  // vars
  UInt_t run;
  ULong64_t evt;  
  tree->SetBranchAddress("run", &run);
  tree->SetBranchAddress("evt", &evt);
  tree_trig->SetBranchAddress("run", &run);
  tree_trig->SetBranchAddress("evt", &evt);

  // clone output tree
  TTree* tree_out = tree->CloneTree(0);

  // add new branches
  Int_t HLT_PHOTONIDISO, HLT_PHOTONIDISO22, HLT_PHOTONIDISO30, HLT_PHOTONIDISO50, HLT_PHOTONIDISO75, HLT_PHOTONIDISO90, HLT_PHOTONIDISO120, HLT_PHOTONIDISO165;
  Int_t HLT_PHOTONIDISOMETEB, HLT_PHOTONIDISOMETEB22, HLT_PHOTONIDISOMETEB30, HLT_PHOTONIDISOMETEB50, HLT_PHOTONIDISOMETEB75, HLT_PHOTONIDISOMETEB90, HLT_PHOTONIDISOMETEB120;
  Int_t HLT_PHOTONIDISOVBFEB, HLT_PHOTONIDISOVBFEB22, HLT_PHOTONIDISOVBFEB30, HLT_PHOTONIDISOVBFEB50, HLT_PHOTONIDISOVBFEB75, HLT_PHOTONIDISOVBFEB90, HLT_PHOTONIDISOVBFEB120;

  // set address for trigger tree
  tree_trig->SetBranchAddress("HLT_PHOTONIDISO", &HLT_PHOTONIDISO);
  tree_trig->SetBranchAddress("HLT_PHOTONIDISO22", &HLT_PHOTONIDISO22);
  tree_trig->SetBranchAddress("HLT_PHOTONIDISO30", &HLT_PHOTONIDISO30);
  tree_trig->SetBranchAddress("HLT_PHOTONIDISO50", &HLT_PHOTONIDISO50);
  tree_trig->SetBranchAddress("HLT_PHOTONIDISO75", &HLT_PHOTONIDISO75);
  tree_trig->SetBranchAddress("HLT_PHOTONIDISO90", &HLT_PHOTONIDISO90);
  tree_trig->SetBranchAddress("HLT_PHOTONIDISO120", &HLT_PHOTONIDISO120);
  tree_trig->SetBranchAddress("HLT_PHOTONIDISO165", &HLT_PHOTONIDISO165);
  tree_trig->SetBranchAddress("HLT_PHOTONIDISOMETEB", &HLT_PHOTONIDISOMETEB);
  tree_trig->SetBranchAddress("HLT_PHOTONIDISOMETEB22", &HLT_PHOTONIDISOMETEB22);
  tree_trig->SetBranchAddress("HLT_PHOTONIDISOMETEB30", &HLT_PHOTONIDISOMETEB30);
  tree_trig->SetBranchAddress("HLT_PHOTONIDISOMETEB50", &HLT_PHOTONIDISOMETEB50);
  tree_trig->SetBranchAddress("HLT_PHOTONIDISOMETEB75", &HLT_PHOTONIDISOMETEB75);
  tree_trig->SetBranchAddress("HLT_PHOTONIDISOMETEB90", &HLT_PHOTONIDISOMETEB90);
  tree_trig->SetBranchAddress("HLT_PHOTONIDISOMETEB120", &HLT_PHOTONIDISOMETEB120);
  tree_trig->SetBranchAddress("HLT_PHOTONIDISOVBFEB", &HLT_PHOTONIDISOVBFEB);
  tree_trig->SetBranchAddress("HLT_PHOTONIDISOVBFEB22", &HLT_PHOTONIDISOVBFEB22);
  tree_trig->SetBranchAddress("HLT_PHOTONIDISOVBFEB30", &HLT_PHOTONIDISOVBFEB30);
  tree_trig->SetBranchAddress("HLT_PHOTONIDISOVBFEB50", &HLT_PHOTONIDISOVBFEB50);
  tree_trig->SetBranchAddress("HLT_PHOTONIDISOVBFEB75", &HLT_PHOTONIDISOVBFEB75);
  tree_trig->SetBranchAddress("HLT_PHOTONIDISOVBFEB90", &HLT_PHOTONIDISOVBFEB90);
  tree_trig->SetBranchAddress("HLT_PHOTONIDISOVBFEB120", &HLT_PHOTONIDISOVBFEB120);

  // add new branch for output tree
  tree_out->Branch("HLT_PHOTONIDISO", &HLT_PHOTONIDISO, "HLT_PHOTONIDISO/I");
  tree_out->Branch("HLT_PHOTONIDISO22", &HLT_PHOTONIDISO22, "HLT_PHOTONIDISO22/I");
  tree_out->Branch("HLT_PHOTONIDISO30", &HLT_PHOTONIDISO30, "HLT_PHOTONIDISO30/I");
  tree_out->Branch("HLT_PHOTONIDISO50", &HLT_PHOTONIDISO50, "HLT_PHOTONIDISO50/I");
  tree_out->Branch("HLT_PHOTONIDISO75", &HLT_PHOTONIDISO75, "HLT_PHOTONIDISO75/I");
  tree_out->Branch("HLT_PHOTONIDISO90", &HLT_PHOTONIDISO90, "HLT_PHOTONIDISO90/I");
  tree_out->Branch("HLT_PHOTONIDISO120", &HLT_PHOTONIDISO120, "HLT_PHOTONIDISO120/I");
  tree_out->Branch("HLT_PHOTONIDISO165", &HLT_PHOTONIDISO165, "HLT_PHOTONIDISO165/I");
  tree_out->Branch("HLT_PHOTONIDISOMETEB", &HLT_PHOTONIDISOMETEB, "HLT_PHOTONIDISOMETEB/I");
  tree_out->Branch("HLT_PHOTONIDISOMETEB22", &HLT_PHOTONIDISOMETEB22, "HLT_PHOTONIDISOMETEB22/I");
  tree_out->Branch("HLT_PHOTONIDISOMETEB30", &HLT_PHOTONIDISOMETEB30, "HLT_PHOTONIDISOMETEB30/I");
  tree_out->Branch("HLT_PHOTONIDISOMETEB50", &HLT_PHOTONIDISOMETEB50, "HLT_PHOTONIDISOMETEB50/I");
  tree_out->Branch("HLT_PHOTONIDISOMETEB75", &HLT_PHOTONIDISOMETEB75, "HLT_PHOTONIDISOMETEB75/I");
  tree_out->Branch("HLT_PHOTONIDISOMETEB90", &HLT_PHOTONIDISOMETEB90, "HLT_PHOTONIDISOMETEB90/I");
  tree_out->Branch("HLT_PHOTONIDISOMETEB120", &HLT_PHOTONIDISOMETEB120, "HLT_PHOTONIDISOMETEB120/I");
  tree_out->Branch("HLT_PHOTONIDISOVBFEB", &HLT_PHOTONIDISOVBFEB, "HLT_PHOTONIDISOVBFEB/I");
  tree_out->Branch("HLT_PHOTONIDISOVBFEB22", &HLT_PHOTONIDISOVBFEB22, "HLT_PHOTONIDISOVBFEB22/I");
  tree_out->Branch("HLT_PHOTONIDISOVBFEB30", &HLT_PHOTONIDISOVBFEB30, "HLT_PHOTONIDISOVBFEB30/I");
  tree_out->Branch("HLT_PHOTONIDISOVBFEB50", &HLT_PHOTONIDISOVBFEB50, "HLT_PHOTONIDISOVBFEB50/I");
  tree_out->Branch("HLT_PHOTONIDISOVBFEB75", &HLT_PHOTONIDISOVBFEB75, "HLT_PHOTONIDISOVBFEB75/I");
  tree_out->Branch("HLT_PHOTONIDISOVBFEB90", &HLT_PHOTONIDISOVBFEB90, "HLT_PHOTONIDISOVBFEB90/I");
  tree_out->Branch("HLT_PHOTONIDISOVBFEB120", &HLT_PHOTONIDISOVBFEB120, "HLT_PHOTONIDISOVBFEB120/I");

  // make index for trigger tree
  tree_trig->BuildIndex("run", "evt");

  // loop
  for (int i=0; i<(int)tree->GetEntries(); i++){
    if (i%1000000==0) std::cout << " Events " << i << std::endl;
    tree->GetEntry(i);
    tree_trig->GetEntryWithIndex(run, evt);
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



