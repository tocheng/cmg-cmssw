

#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <ctime>

// Hengne Li 2017 @ CERN 

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
  TFile* fin = TFile::Open(inputfile.c_str());
  TFile* fout = TFile::Open(outputfile.c_str(), "recreate");

  
  TTree* tree = (TTree*)fin->Get("tree");
  fout->cd();
 
  TTree* otree = (TTree*)tree->CopyTree("lheNj==0");

  otree->Write();
  fout->Close(); 
  fin->Close();

  now = time(0);
  dt = ctime(&now);
  std::cout << "End time is: " << dt << std::endl;

  return 0;
}
