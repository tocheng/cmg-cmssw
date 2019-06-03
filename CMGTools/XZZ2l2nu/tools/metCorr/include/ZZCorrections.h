
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <cmath>
#include "TLorentzVector.h"
#include "TFile.h"
#include "TH1D.h"

class ZZCorrections {
 public:
  ZZCorrections();
  ~ZZCorrections();
 
  void loadEwkTable(std::string& filename); 
  std::vector<float> findEwkCorrection(float sqrt_s_hat, float t_hat);
  float getEwkCorrections(float& ewkCorrections_error, int incomePdgId, float pdf_x1, float pdf_x2, TLorentzVector V1, TLorentzVector V2, float sum4Pt );

  void loadQcdFile(std::string& filename);
  float getKfactor_qqZZ_qcd_mZZ(float mzz); 
  float getKfactor_qqZZ_qcd_mZZ_up(float mzz);
  float getKfactor_qqZZ_qcd_mZZ_dn(float mzz);
  
  float kfactor_qqZZ_qcd_dPhi(float GENdPhiZZ, int finalState);
  float kfactor_qqZZ_qcd_M(float GENmassZZ, int finalState);
  float kfactor_qqZZ_qcd_Pt(float GENpTZZ, int finalState);

 private:
  std::vector<std::vector<float>> Table_EWK;
  TFile* QCDCorrInputFile;
  TH1D* h_nnlo_to_nlo_vs_mzz_ct;
  TH1D* h_nnlo_to_nlo_vs_mzz_up;
  TH1D* h_nnlo_to_nlo_vs_mzz_dn;

};

