#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TH1D.h"
#include "TProfile.h"
#include "TH2D.h"
#include "TH3D.h"
#include "TCanvas.h"
#include "TMath.h"
#include "TGraphErrors.h"
#include "TVector2.h"
#include "TEntryList.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <algorithm>
#include <set>
#include <utility>
#include "TLegend.h"
#include "TPaveText.h"
#include "TROOT.h"

// Hengne Li @ CERN, 2016

int main(int argc, char** argv) {


  TFile* file1 = TFile::Open("/home/heli/XZZ/80X_20170202_light_Skim/DYJetsToLL_M50_Ext_NoRecoil.root");
  TFile* file2 = TFile::Open("/home/heli/XZZ/80X_20170202_GJets_light_Skim/SinglePhoton_Run2016Full_ReReco_v2_NoRecoil.root");

  std::string outtag="study_gjets_data_fullv2";


  gROOT->ProcessLine(".x tdrstyle.C");

  // yields:

  // all:
  //Data : 648231.000000 // 660583.000000
  //WW/WZ/WJets non-reson. : 617.417435 // 619.270746
  //TT : 10075.407445  // 10113.088935
  //ZZ WZ reson. : 6728.956797 // 6753.921537
  //ZJets Data-xxx : 630809 = 648231.0-617.417435-10075.407445-6728.956797
  //    // 643097  = 660583.000000 - 619.270746 - 10113.088935 - 6753.921537
  // GJets weight integral: 4805.009858 // 4820.096067
  // scale = [ZJets Data-xxx]/[GJets weight integral]
  Double_t Norm_All = 1.0;

  // mu: zpt [50,200]
  //Data : 633430.000000 // 645610.000000 
  //WW/WZ/WJets non-reson. : 604.878487 // 606.650081
  //TT : 9929.086077 // 9966.319579
  //ZZ WZ reson. : 6438.827293 // 6462.592193
  //ZJets Data-xxx : 616457. = 633430.000000-604.878487-9929.086077-6438.827293 
  //    //  645610.000000 - 606.650081 - 9966.319579 - 6462.592193 = 628574.0
  // GJets weight integral: 4771.470601 // 4786.843272
  // scale = [ZJets Data-xxx]/[GJets weight integral]
  Double_t Norm_Mu = 1.0;

  // el:
  //Data : 14801.000000 // 14973.000000
  //WW/WZ/WJets non-reson. : 12.538947 // 12.620665 
  //TT : 146.321369  // 146.769356
  //ZZ WZ reson. : 290.129504  // 291.329345
  //ZJets Data-xxx : 14352.0 = 14801.000000-12.538947-146.321369-290.129504
  //    //  14522.3  = 14973.000000 - 12.620665 - 146.769356 - 291.329345
  // GJets weight integral: 6865.677331 // 6864.595736
  // scale = [ZJets Data-xxx]/[GJets weight integral]
  Double_t Norm_El = 1.0;

  char name[1000];


  std::string lumiTag;
  TPaveText* lumipt;

  lumiTag = "CMS 13 TeV 2016 L=35.87 fb^{-1}";
  lumipt = new TPaveText(0.2,0.8,0.8,0.88,"brNDC");
  lumipt->SetBorderSize(0);
  lumipt->SetTextAlign(12);
  lumipt->SetFillStyle(0);
  lumipt->SetTextFont(42);
  lumipt->SetTextSize(0.03);
  lumipt->AddText(0.15,0.3, lumiTag.c_str());

  TCanvas* plots = new TCanvas("plots", "plots");

  sprintf(name, "%s.pdf[", outtag.c_str());
  plots->Print(name);

  TTree* tree1 = (TTree*)file1->Get("tree");
  TTree* tree2 = (TTree*)file2->Get("tree");

  // some gjets alias
  // alias to use skimmed GJets ntuple
  tree2->SetAlias("l1_pt", "llnunu_l1_pt");
  tree2->SetAlias("l1_phi", "llnunu_l1_phi");
  tree2->SetAlias("l1_eta", "llnunu_l1_eta");
  tree2->SetAlias("l1_rapidity", "llnunu_l1_rapidity");
  tree2->SetAlias("l2_pt", "llnunu_l2_pt");
  tree2->SetAlias("l2_phi", "llnunu_l2_phi");

  // hlt alias
  tree1->SetAlias("passMuHLT", "((llnunu_l1_l1_trigerob_HLTbit>>3&1)||(llnunu_l1_l1_trigerob_HLTbit>>4&1)||(llnunu_l1_l2_trigerob_HLTbit>>3&1)||(llnunu_l1_l2_trigerob_HLTbit>>4&1))");
  tree1->SetAlias("passElHLT", "((llnunu_l1_l1_trigerob_HLTbit>>1&1)||(llnunu_l1_l2_trigerob_HLTbit>>1&1))");
  //
  std::string cuts_lepaccept="((abs(llnunu_l1_l1_pdgId)==13&&abs(llnunu_l1_l2_pdgId)==13&&llnunu_l1_l1_pt>60&&abs(llnunu_l1_l1_eta)<2.4&&llnunu_l1_l2_pt>20&&abs(llnunu_l1_l2_eta)<2.4&&(llnunu_l1_l1_highPtID==1||llnunu_l1_l2_highPtID==1))";
  cuts_lepaccept+="||(abs(llnunu_l1_l1_pdgId)==11&&abs(llnunu_l1_l2_pdgId)==11&&llnunu_l1_l1_pt>120&&abs(llnunu_l1_l1_eta)<2.5&&llnunu_l1_l2_pt>35&&abs(llnunu_l1_l2_eta)<2.5))";
  std::string cuts_lepaccept_lowlpt="((abs(llnunu_l1_l1_pdgId)==13&&abs(llnunu_l1_l2_pdgId)==13&&llnunu_l1_l1_pt>20&&abs(llnunu_l1_l1_eta)<2.4&&llnunu_l1_l2_pt>20&&abs(llnunu_l1_l2_eta)<2.4&&(llnunu_l1_l1_highPtID==1||llnunu_l1_l2_highPtID==1))";
  cuts_lepaccept_lowlpt+="||(abs(llnunu_l1_l1_pdgId)==11&&abs(llnunu_l1_l2_pdgId)==11&&llnunu_l1_l1_pt>20&&abs(llnunu_l1_l1_eta)<2.5&&llnunu_l1_l2_pt>20&&abs(llnunu_l1_l2_eta)<2.5))";
  std::string cuts_zmass="(llnunu_l1_mass>70&&llnunu_l1_mass<110)";
  std::string cuts_loose_z="("+cuts_lepaccept+"&&"+cuts_zmass+")";
  std::string cuts_loose_z_lowlpt="("+cuts_lepaccept_lowlpt+"&&"+cuts_zmass+")";


  std::string base_selec =  cuts_loose_z;
  std::string base_selec_lowlpt =  cuts_loose_z_lowlpt;

  std::string base_selec_el = "(" + cuts_loose_z + "&&(abs(llnunu_l1_l1_pdgId)==11&&abs(llnunu_l1_l2_pdgId)==11)&&passElHLT)";
  std::string base_selec_mu = "(" + cuts_loose_z + "&&(abs(llnunu_l1_l1_pdgId)==13&&abs(llnunu_l1_l2_pdgId)==13)&&passMuHLT)";
  std::string base_selec_lowlpt_el = "(" + cuts_loose_z_lowlpt + "&&(abs(llnunu_l1_l1_pdgId)==11&&abs(llnunu_l1_l2_pdgId)==11))";
  std::string base_selec_lowlpt_mu = "(" + cuts_loose_z_lowlpt + "&&(abs(llnunu_l1_l1_pdgId)==13&&abs(llnunu_l1_l2_pdgId)==13))";

  // add weight
  std::string weight_selec = std::string("*(genWeight/SumWeights*ZPtWeight*puWeightsummer16*xsec)");
  std::string weight_selec_up = std::string("*(genWeight/SumWeights*ZPtWeight_up*puWeightsummer16*xsec)");
  std::string weight_selec_dn = std::string("*(genWeight/SumWeights*ZPtWeight_dn*puWeightsummer16*xsec)");
  // rho weight
  //std::string rhoweight_selec = "*(0.32+0.42*TMath::Erf((rho-4.16)/4.58)+0.31*TMath::Erf((rho+115.00)/29.58))"; // for b-h 36.22 fb-1
  std::string rhoweight_selec = "*(1)";
  // scale factors
  // temporary remove tracking eff scale factors
  std::string effsf_selec = std::string("*(trgsf*isosf*idsf*trksf)");
  std::string effsf_selec_up = std::string("*(trgsf_up*idisotrksf_up)");
  std::string effsf_selec_dn = std::string("*(trgsf_dn*idisotrksf_dn)");
  std::string effsf_selec_lowlpt = std::string("*(isosf*idsf*trksf)");
  std::string effsf_selec_lowlpt_up = std::string("*(idisotrksf_up)");
  std::string effsf_selec_lowlpt_dn = std::string("*(idisotrksf_dn)");


  // selec, cuts + weights
  std::string zjet_selec = base_selec + weight_selec + rhoweight_selec + effsf_selec;
  std::string zjet_selec_el = base_selec_el + weight_selec + rhoweight_selec + effsf_selec;
  std::string zjet_selec_mu = base_selec_mu + weight_selec + rhoweight_selec + effsf_selec;
  std::string zjet_selec_lowlpt = base_selec_lowlpt + weight_selec + rhoweight_selec + effsf_selec_lowlpt;
  std::string zjet_selec_lowlpt_el = base_selec_lowlpt_el + weight_selec + rhoweight_selec + effsf_selec_lowlpt;
  std::string zjet_selec_lowlpt_mu = base_selec_lowlpt_mu + weight_selec + rhoweight_selec + effsf_selec_lowlpt;

  std::string zjet_selec_up = base_selec + weight_selec_up + rhoweight_selec + effsf_selec_up;
  std::string zjet_selec_el_up = base_selec_el + weight_selec_up + rhoweight_selec + effsf_selec_up;
  std::string zjet_selec_mu_up = base_selec_mu + weight_selec_up + rhoweight_selec + effsf_selec_up;
  std::string zjet_selec_lowlpt_up = base_selec_lowlpt + weight_selec_up + rhoweight_selec + effsf_selec_lowlpt_up;
  std::string zjet_selec_lowlpt_el_up = base_selec_lowlpt_el + weight_selec_up + rhoweight_selec + effsf_selec_lowlpt_up;
  std::string zjet_selec_lowlpt_mu_up = base_selec_lowlpt_mu + weight_selec_up + rhoweight_selec + effsf_selec_lowlpt_up;

  std::string zjet_selec_dn = base_selec + weight_selec_dn + rhoweight_selec + effsf_selec_dn;
  std::string zjet_selec_el_dn = base_selec_el + weight_selec_dn + rhoweight_selec + effsf_selec_dn;
  std::string zjet_selec_mu_dn = base_selec_mu + weight_selec_dn + rhoweight_selec + effsf_selec_dn;
  std::string zjet_selec_lowlpt_dn = base_selec_lowlpt + weight_selec_dn + rhoweight_selec + effsf_selec_lowlpt_dn;
  std::string zjet_selec_lowlpt_el_dn = base_selec_lowlpt_el + weight_selec_dn + rhoweight_selec + effsf_selec_lowlpt_dn;
  std::string zjet_selec_lowlpt_mu_dn = base_selec_lowlpt_mu + weight_selec_dn + rhoweight_selec + effsf_selec_lowlpt_dn;



  //std::string gjet_selec = "(1)"; // input ntuple preselected. 
  std::string gjet_selec = "(1)*(GJetsRhoWeight*GJetsPreScaleWeight)"; // input ntuple preselected. 

  //Double_t ZPtBins[] = {0,1.25,2.5,3.75,5,6.25,7.5,8.75,10,11.25,12.5,15,17.5,20,25,30,35,40,45,50,60,70,80,90,100,110,130,150,170,190,220,250,400,1000};
  //Double_t ZPtBins[] = {0,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,65,70,75,80,85,90,95,100,105,110,115,120,125,130,135,140,145,150,155,160,165,170,175,180,185,190,195,200,215,220,225,230,235,240,245,250,255,260,265,270,275,280,285,290,295,300,350,400,500,700,3000};
  //Double_t ZPtBins[] = {0,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,65,70,75,80,85,90,95,100,110,120,130,140,150,160,170,180,190,200,220,240,245,260,280,300,350,500,1000,3000};
  Double_t ZPtBins[] = {0,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,65,70,75,80,85,90,95,100,110,120,130,140,160,180,200,240,280,320,400,600,1000};
  Int_t NZPtBins = sizeof(ZPtBins)/sizeof(ZPtBins[0]) - 1;
  Double_t ZMassBins[] = {50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,176,177,178,179,180};
  Int_t NZMassBins = sizeof(ZMassBins)/sizeof(ZMassBins[0]) - 1;
  //Double_t ZRapBins[] = {-3.0,-2.9,-2.8,-2.7,-2.6,-2.5,-2.4,-2.3,-2.2,-2.1,-2.0,-1.9,-1.8,-1.7,-1.6,-1.5,-1.4,-1.3,-1.2,-1.1,-1.0,-0.9,-0.8,-0.7,-0.6,-0.5,-0.4,-0.3,-0.2,-0.1,0.0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0,1.1,1.2,1.3,1.4,1.5,1.6,1.7,1.8,1.9,2.0,2.1,2.2,2.3,2.4,2.5,2.6,2.7,2.8,2.9,3.0};
  Double_t ZRapBins[] = {-2.5,-2.4,-2.3,-2.2,-2.1,-2.0,-1.9,-1.8,-1.7,-1.6,-1.5,-1.4,-1.3,-1.2,-1.1,-1.0,-0.9,-0.8,-0.7,-0.6,-0.5,-0.4,-0.3,-0.2,-0.1,0.0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0,1.1,1.2,1.3,1.4,1.5,1.6,1.7,1.8,1.9,2.0,2.1,2.2,2.3,2.4,2.5};
  //Double_t ZRapBins[] = {-2.5,-2.3,-2.1,-1.9,-1.7,-1.5,-1.3,-1.1,-0.9,-0.7,-0.5,-0.3,-0.1,0.1,0.3,0.5,0.7,0.9,1.1,1.3,1.5,1.7,1.9,2.1,2.3,2.5};
  Int_t NZRapBins = sizeof(ZRapBins)/sizeof(ZRapBins[0]) - 1;



  // photon
  TH1D* hzptbb2 = new TH1D("hzptbb2", "Z or Photon PT Distribution", NZPtBins, ZPtBins);
  hzptbb2->Sumw2();
  tree2->Draw("l1_pt>>hzptbb2", gjet_selec.c_str());
  double gnorm = hzptbb2->Integral();
  hzptbb2->Scale(1./gnorm, "width");

  std::cout << "gdataYield = " << std::setprecision(20) << gnorm << std::endl;

  // photon pt profile
  TProfile* pr_zpt_2 = new TProfile("pr_zpt_2", "Z or Photon PT Profile", NZPtBins, ZPtBins);
  tree2->Draw("l1_pt:l1_pt>>pr_zpt_2", gjet_selec.c_str());


  // all
  TH1D* hzptbb1 = new TH1D("hzptbb1", "Z or Photon PT Distribution", NZPtBins, ZPtBins);
  hzptbb1->Sumw2();
  tree1->Draw("llnunu_l1_pt>>hzptbb1", zjet_selec.c_str());

  // profile
  TProfile* pr_zpt_1 = new TProfile("pr_zpt_1", "Z or Photon PT Profile", NZPtBins, ZPtBins);
  tree1->Draw("llnunu_l1_pt:llnunu_l1_pt>>pr_zpt_1",zjet_selec.c_str());

  double znorm = hzptbb1->Integral();
  hzptbb1->Scale(1./znorm, "width");
  std::cout << "zjetsFidXsecAll = " << std::setprecision(20) << znorm << std::endl;



  hzptbb1->SetMarkerColor(2);
  hzptbb2->SetMarkerColor(4);
  hzptbb1->SetLineColor(2);
  hzptbb2->SetLineColor(4);

  TLegend* lgzptbb = new TLegend(0.5,0.6,0.8,0.8);
  lgzptbb->SetName("lgzptbb");
  lgzptbb->AddEntry(hzptbb1, "ZJets", "pl");
  lgzptbb->AddEntry(hzptbb2, "GJets", "pl");

  plots->Clear();

  hzptbb1->Draw();
  hzptbb2->Draw("same");
  lgzptbb->Draw();
  lumipt->Draw();
  sprintf(name, "%s.pdf", outtag.c_str());
  plots->Print(name);
  plots->Clear();

  pr_zpt_1->SetMarkerColor(2);
  pr_zpt_2->SetMarkerColor(4);
  pr_zpt_1->SetLineColor(2);
  pr_zpt_2->SetLineColor(4);
  
  TLegend* lg_pr_zpt = new TLegend(0.5,0.6,0.8,0.8);
  lg_pr_zpt->SetName("lg_pr_zpt");
  lg_pr_zpt->AddEntry(pr_zpt_1, "ZJets", "pl");
  lg_pr_zpt->AddEntry(pr_zpt_2, "GJets", "pl");

  plots->Clear();

  pr_zpt_1->Draw();
  pr_zpt_2->Draw("same");
  lg_pr_zpt->Draw();
  lumipt->Draw();
  sprintf(name, "%s.pdf", outtag.c_str());
  plots->Print(name);
  plots->Clear();


  TH1D* hzptbbr12 = (TH1D*)hzptbb1->Clone("hzptbbr12");
  hzptbbr12->SetTitle("Z PT / Photon PT Ratio");
  hzptbbr12->Divide(hzptbb2);
  hzptbbr12->Scale(Norm_All);

  plots->Clear();
  hzptbbr12->Draw();
  lumipt->Draw();
  sprintf(name, "%s.pdf", outtag.c_str());
  plots->Print(name);
  plots->Clear();

  // el
  // profile
  TProfile* pr_zpt_1_el = new TProfile("pr_zpt_1_el", "Z PT Profile El channel", NZPtBins, ZPtBins);
  tree1->Draw("llnunu_l1_pt:llnunu_l1_pt>>pr_zpt_1_el",zjet_selec_el.c_str());

  TH1D* hzptbb1_el = new TH1D("hzptbb1_el", "Z PT Distribution El channel", NZPtBins, ZPtBins);
  hzptbb1_el->Sumw2();
  tree1->Draw("llnunu_l1_pt>>hzptbb1_el", zjet_selec_el.c_str());
  double znorm_el = hzptbb1_el->Integral();
  hzptbb1_el->Scale(1./znorm_el, "width");
  std::cout << "zjetsFidXsecEl =  " << std::setprecision(20) << znorm_el << std::endl;

  hzptbb1_el->SetMarkerColor(2);
  hzptbb1_el->SetLineColor(2);

  TLegend* lgzptbb_el = new TLegend(0.5,0.6,0.8,0.8);
  lgzptbb_el->SetName("lgzptbb_el");
  lgzptbb_el->AddEntry(hzptbb1_el, "ZJets", "pl");
  lgzptbb_el->AddEntry(hzptbb2, "GJets", "pl");

  plots->Clear();

  hzptbb1_el->Draw();
  hzptbb2->Draw("same");
  lgzptbb_el->Draw();
  lumipt->Draw();
  sprintf(name, "%s.pdf", outtag.c_str());
  plots->Print(name);
  plots->Clear();

  TH1D* hzptbbr12_el = (TH1D*)hzptbb1_el->Clone("hzptbbr12_el");
  hzptbbr12_el->SetTitle("Z PT / Photon PT Ratio El channel");
  hzptbbr12_el->Divide(hzptbb2);
  hzptbbr12_el->Scale(Norm_El);

  plots->Clear();
  hzptbbr12_el->Draw();
  lumipt->Draw();
  sprintf(name, "%s.pdf", outtag.c_str());
  plots->Print(name);
  plots->Clear();


  // mu
  // profile
  TProfile* pr_zpt_1_mu = new TProfile("pr_zpt_1_mu", "Z PT Profile Mu channel", NZPtBins, ZPtBins);
  tree1->Draw("llnunu_l1_pt:llnunu_l1_pt>>pr_zpt_1_mu",zjet_selec_mu.c_str());

  TH1D* hzptbb1_mu = new TH1D("hzptbb1_mu", "Z PT Distribution Mu channel", NZPtBins, ZPtBins);
  hzptbb1_mu->Sumw2();
  tree1->Draw("llnunu_l1_pt>>hzptbb1_mu", zjet_selec_mu.c_str());
  double znorm_mu = hzptbb1_mu->Integral();
  hzptbb1_mu->Scale(1./znorm_mu, "width");
  std::cout << "zjetsFidXsecMu =  " << std::setprecision(20) << znorm_mu << std::endl;


  hzptbb1_mu->SetMarkerColor(2);
  hzptbb1_mu->SetLineColor(2);

  TLegend* lgzptbb_mu = new TLegend(0.5,0.6,0.8,0.8);
  lgzptbb_mu->SetName("lgzptbb_mu");
  lgzptbb_mu->AddEntry(hzptbb1_mu, "ZJets", "pl");
  lgzptbb_mu->AddEntry(hzptbb2, "GJets", "pl");

  plots->Clear();

  hzptbb1_mu->Draw();
  hzptbb2->Draw("same");
  lgzptbb_mu->Draw();
  lumipt->Draw();
  sprintf(name, "%s.pdf", outtag.c_str());
  plots->Print(name);
  plots->Clear();

  TH1D* hzptbbr12_mu = (TH1D*)hzptbb1_mu->Clone("hzptbbr12_mu");
  hzptbbr12_mu->SetTitle("Z PT / Photon PT Ratio Mu channel");
  hzptbbr12_mu->Divide(hzptbb2);
  hzptbbr12_mu->Scale(Norm_Mu);

  plots->Clear();
  hzptbbr12_mu->Draw();
  lumipt->Draw();
  sprintf(name, "%s.pdf", outtag.c_str());
  plots->Print(name);
  plots->Clear();


  // all up
  // profile
  TProfile* pr_zpt_1_up = new TProfile("pr_zpt_1_up", "Z PT Profile Up", NZPtBins, ZPtBins);
  tree1->Draw("llnunu_l1_pt:llnunu_l1_pt>>pr_zpt_1_up",zjet_selec_up.c_str());

  TH1D* hzptbb1_up = new TH1D("hzptbb1_up", "Z PT Distribution Up", NZPtBins, ZPtBins);
  tree1->Draw("llnunu_l1_pt>>hzptbb1_up", zjet_selec_up.c_str());
  double znorm_up = hzptbb1_up->Integral();
  hzptbb1_up->Scale(1./znorm_up, "width");
  std::cout << "zjetsFidXsecAll_up = " << std::setprecision(20) << znorm_up << std::endl;

  TH1D* hzptbbr12_up = (TH1D*)hzptbb1_up->Clone("hzptbbr12_up");
  hzptbbr12_up->SetTitle("Z PT / Photon PT Ratio Up");
  hzptbbr12_up->Divide(hzptbb2);
  hzptbbr12_up->Scale(Norm_All);

  // all dn
  // profile
  TProfile* pr_zpt_1_dn = new TProfile("pr_zpt_1_dn", "Z PT Profile Down", NZPtBins, ZPtBins);
  tree1->Draw("llnunu_l1_pt:llnunu_l1_pt>>pr_zpt_1_dn",zjet_selec_dn.c_str());
  TH1D* hzptbb1_dn = new TH1D("hzptbb1_dn", "Z PT Distribution Down", NZPtBins, ZPtBins);
  tree1->Draw("llnunu_l1_pt>>hzptbb1_dn", zjet_selec_dn.c_str());
  double znorm_dn = hzptbb1_dn->Integral();
  hzptbb1_dn->Scale(1./znorm_dn, "width");
  std::cout << "zjetsFidXsecAll_dn = " << std::setprecision(20) << znorm_dn << std::endl;
  TH1D* hzptbbr12_dn = (TH1D*)hzptbb1_dn->Clone("hzptbbr12_dn");
  hzptbbr12_dn->SetTitle("Z PT / Photon PT Ratio Down");
  hzptbbr12_dn->Divide(hzptbb2);
  hzptbbr12_dn->Scale(Norm_All);

  // el up
  // profile
  TProfile* pr_zpt_1_el_up = new TProfile("pr_zpt_1_el_up", "Z PT Profile El channel Up", NZPtBins, ZPtBins);
  tree1->Draw("llnunu_l1_pt:llnunu_l1_pt>>pr_zpt_1_el_up",zjet_selec_el_up.c_str());
  TH1D* hzptbb1_el_up = new TH1D("hzptbb1_el_up", "Z PT Distribution El channel Up", NZPtBins, ZPtBins);
  hzptbb1_el_up->Sumw2();
  tree1->Draw("llnunu_l1_pt>>hzptbb1_el_up", zjet_selec_el_up.c_str());
  double znorm_el_up = hzptbb1_el_up->Integral();
  hzptbb1_el_up->Scale(1./znorm_el_up, "width");
  std::cout << "zjetsFidXsecEl_up = " << std::setprecision(20) << znorm_el_up << std::endl;
  TH1D* hzptbbr12_el_up = (TH1D*)hzptbb1_el_up->Clone("hzptbbr12_el_up");
  hzptbbr12_el_up->SetTitle("Z PT / Photon PT Ratio El channel Up");
  hzptbbr12_el_up->Divide(hzptbb2);
  hzptbbr12_el_up->Scale(Norm_El);

  // el dn
  // profile
  TProfile* pr_zpt_1_el_dn = new TProfile("pr_zpt_1_el_dn", "Z PT Profile El channel Down", NZPtBins, ZPtBins);
  tree1->Draw("llnunu_l1_pt:llnunu_l1_pt>>pr_zpt_1_el_dn",zjet_selec_el_dn.c_str());
  TH1D* hzptbb1_el_dn = new TH1D("hzptbb1_el_dn", "Z PT Distribution El channel Down", NZPtBins, ZPtBins);
  hzptbb1_el_dn->Sumw2();
  tree1->Draw("llnunu_l1_pt>>hzptbb1_el_dn", zjet_selec_el_dn.c_str());
  double znorm_el_dn = hzptbb1_el_dn->Integral();
  hzptbb1_el_dn->Scale(1./znorm_el_dn, "width");
  std::cout << "zjetsFidXsecEl_dn = " << std::setprecision(20) << znorm_el_dn << std::endl;
  TH1D* hzptbbr12_el_dn = (TH1D*)hzptbb1_el_dn->Clone("hzptbbr12_el_dn");
  hzptbbr12_el_dn->SetTitle("Z PT / Photon PT Ratio El channel Down");
  hzptbbr12_el_dn->Divide(hzptbb2);
  hzptbbr12_el_dn->Scale(Norm_El);

  // mu up
  // profile
  TProfile* pr_zpt_1_mu_up = new TProfile("pr_zpt_1_mu_up", "Z PT Profile Mu channel Up", NZPtBins, ZPtBins);
  tree1->Draw("llnunu_l1_pt:llnunu_l1_pt>>pr_zpt_1_mu_up",zjet_selec_mu_up.c_str());
  TH1D* hzptbb1_mu_up = new TH1D("hzptbb1_mu_up", "Z PT Distribution Mu channel Up", NZPtBins, ZPtBins);
  hzptbb1_mu_up->Sumw2();
  tree1->Draw("llnunu_l1_pt>>hzptbb1_mu_up", zjet_selec_mu_up.c_str());
  double znorm_mu_up = hzptbb1_mu_up->Integral();
  hzptbb1_mu_up->Scale(1./znorm_mu_up, "width");
  std::cout << "zjetsFidXsecMu_up = " << std::setprecision(20) << znorm_mu_up << std::endl;
  TH1D* hzptbbr12_mu_up = (TH1D*)hzptbb1_mu_up->Clone("hzptbbr12_mu_up");
  hzptbbr12_mu_up->SetTitle("Z PT / Photon PT Ratio Mu channel Up");
  hzptbbr12_mu_up->Divide(hzptbb2);
  hzptbbr12_mu_up->Scale(Norm_El);

  // mu dn
  // profile
  TProfile* pr_zpt_1_mu_dn = new TProfile("pr_zpt_1_mu_dn", "Z PT Profile Mu channel Down", NZPtBins, ZPtBins);
  tree1->Draw("llnunu_l1_pt:llnunu_l1_pt>>pr_zpt_1_mu_dn",zjet_selec_mu_dn.c_str());
  TH1D* hzptbb1_mu_dn = new TH1D("hzptbb1_mu_dn", "Z PT Distribution Mu channel Down", NZPtBins, ZPtBins);
  hzptbb1_mu_dn->Sumw2();
  tree1->Draw("llnunu_l1_pt>>hzptbb1_mu_dn", zjet_selec_mu_dn.c_str());
  double znorm_mu_dn = hzptbb1_mu_dn->Integral();
  hzptbb1_mu_dn->Scale(1./znorm_mu_dn, "width");
  std::cout << "zjetsFidXsecMu_dn = " << std::setprecision(20) << znorm_mu_dn << std::endl;
  TH1D* hzptbbr12_mu_dn = (TH1D*)hzptbb1_mu_dn->Clone("hzptbbr12_mu_dn");
  hzptbbr12_mu_dn->SetTitle("Z PT / Photon PT Ratio Mu channel Down");
  hzptbbr12_mu_dn->Divide(hzptbb2);
  hzptbbr12_mu_dn->Scale(Norm_Mu);


  // zpt lowlpt all
  // profile
  TProfile* pr_zpt_1_lowlpt = new TProfile("pr_zpt_1_lowlpt", "Z PT Profile Low Lep. pT", NZPtBins, ZPtBins);
  tree1->Draw("llnunu_l1_pt:llnunu_l1_pt>>pr_zpt_1_lowlpt",zjet_selec_lowlpt.c_str());

  TH1D* hzptbb1_lowlpt = new TH1D("hzptbb1_lowlpt", "Z PT Distribution Low Lep. pT", NZPtBins, ZPtBins);
  hzptbb1_lowlpt->Sumw2();
  tree1->Draw("llnunu_l1_pt>>hzptbb1_lowlpt", zjet_selec_lowlpt.c_str());
  double znorm_lowlpt = hzptbb1_lowlpt->Integral();
  hzptbb1_lowlpt->Scale(1./znorm_lowlpt, "width");
  std::cout << "zjetsFidXsecLowLptAll = " << std::setprecision(20) << znorm_lowlpt << std::endl;

  hzptbb1_lowlpt->SetMarkerColor(2);
  hzptbb1_lowlpt->SetLineColor(2);

  TLegend* lgzptbb_lowlpt = new TLegend(0.5,0.6,0.8,0.8);
  lgzptbb_lowlpt->SetName("lgzptbb_lowlpt");
  lgzptbb_lowlpt->AddEntry(hzptbb1_lowlpt, "ZJets", "pl");
  lgzptbb_lowlpt->AddEntry(hzptbb2, "GJets", "pl");

  plots->Clear();

  hzptbb1_lowlpt->Draw();
  hzptbb2->Draw("same");
  lgzptbb_lowlpt->Draw();
  lumipt->Draw();
  sprintf(name, "%s.pdf", outtag.c_str());
  plots->Print(name);
  plots->Clear();

  TH1D* hzptbbr12_lowlpt = (TH1D*)hzptbb1_lowlpt->Clone("hzptbbr12_lowlpt");
  hzptbbr12_lowlpt->SetTitle("Z PT / Photon PT Ratio Low Lep. pT");
  hzptbbr12_lowlpt->Divide(hzptbb2);
  hzptbbr12_lowlpt->Scale(Norm_All);

  plots->Clear();
  hzptbbr12_lowlpt->Draw();
  lumipt->Draw();
  sprintf(name, "%s.pdf", outtag.c_str());
  plots->Print(name);
  plots->Clear();

  // lowlpt el
  // profile
  TProfile* pr_zpt_1_lowlpt_el = new TProfile("pr_zpt_1_lowlpt_el", "Z PT Profile Low Lep. pT El channel", NZPtBins, ZPtBins);
  tree1->Draw("llnunu_l1_pt:llnunu_l1_pt>>pr_zpt_1_lowlpt_el",zjet_selec_lowlpt_el.c_str());
  TH1D* hzptbb1_lowlpt_el = new TH1D("hzptbb1_lowlpt_el", "Z PT Distribution Low Lep. pT El channel", NZPtBins, ZPtBins);
  hzptbb1_lowlpt_el->Sumw2();
  tree1->Draw("llnunu_l1_pt>>hzptbb1_lowlpt_el", zjet_selec_lowlpt_el.c_str());
  double znorm_lowlpt_el = hzptbb1_lowlpt_el->Integral();
  hzptbb1_lowlpt_el->Scale(1./znorm_lowlpt_el, "width");
  std::cout << "zjetsFidXsecLowLptEl = " << std::setprecision(20) << znorm_lowlpt_el << std::endl;

  hzptbb1_lowlpt_el->SetMarkerColor(2);
  hzptbb1_lowlpt_el->SetLineColor(2);

  TLegend* lgzptbb_lowlpt_el = new TLegend(0.5,0.6,0.8,0.8);
  lgzptbb_lowlpt_el->SetName("lgzptbb_lowlpt_el");
  lgzptbb_lowlpt_el->AddEntry(hzptbb1_lowlpt_el, "ZJets", "pl");
  lgzptbb_lowlpt_el->AddEntry(hzptbb2, "GJets", "pl");

  plots->Clear();

  hzptbb1_lowlpt_el->Draw();
  hzptbb2->Draw("same");
  lgzptbb_lowlpt_el->Draw();
  lumipt->Draw();
  sprintf(name, "%s.pdf", outtag.c_str());
  plots->Print(name);
  plots->Clear();

  TH1D* hzptbbr12_lowlpt_el = (TH1D*)hzptbb1_lowlpt_el->Clone("hzptbbr12_lowlpt_el");
  hzptbbr12_lowlpt_el->SetTitle("Z PT / Photon PT Ratio El channel Low Lep. pT");
  hzptbbr12_lowlpt_el->Divide(hzptbb2);
  hzptbbr12_lowlpt_el->Scale(Norm_El);

  plots->Clear();
  hzptbbr12_lowlpt_el->Draw();
  lumipt->Draw();
  sprintf(name, "%s.pdf", outtag.c_str());
  plots->Print(name);
  plots->Clear();


  // lowlpt  mu
  // profile
  TProfile* pr_zpt_1_lowlpt_mu = new TProfile("pr_zpt_1_lowlpt_mu", "Z PT Profile Low Lep. pT Mu Channel", NZPtBins, ZPtBins);
  tree1->Draw("llnunu_l1_pt:llnunu_l1_pt>>pr_zpt_1_lowlpt_mu",zjet_selec_lowlpt_mu.c_str());
  TH1D* hzptbb1_lowlpt_mu = new TH1D("hzptbb1_lowlpt_mu", "Z PT Distribution Low Lep. pT Mu channel", NZPtBins, ZPtBins);
  hzptbb1_lowlpt_mu->Sumw2();
  tree1->Draw("llnunu_l1_pt>>hzptbb1_lowlpt_mu", zjet_selec_lowlpt_mu.c_str());
  double znorm_lowlpt_mu = hzptbb1_lowlpt_mu->Integral();
  hzptbb1_lowlpt_mu->Scale(1./znorm_lowlpt_mu, "width");
  std::cout << "zjetsFidXsecLowLptMu = " << std::setprecision(20) << znorm_lowlpt_mu << std::endl;

  hzptbb1_lowlpt_mu->SetMarkerColor(2);
  hzptbb1_lowlpt_mu->SetLineColor(2);

  TLegend* lgzptbb_lowlpt_mu = new TLegend(0.5,0.6,0.8,0.8);
  lgzptbb_lowlpt_mu->SetName("lgzptbb_lowlpt_mu");
  lgzptbb_lowlpt_mu->AddEntry(hzptbb1_lowlpt_mu, "ZJets", "pl");
  lgzptbb_lowlpt_mu->AddEntry(hzptbb2, "GJets", "pl");

  plots->Clear();

  hzptbb1_lowlpt_mu->Draw();
  hzptbb2->Draw("same");
  lgzptbb_lowlpt_mu->Draw();
  lumipt->Draw();
  sprintf(name, "%s.pdf", outtag.c_str());
  plots->Print(name);
  plots->Clear();

  TH1D* hzptbbr12_lowlpt_mu = (TH1D*)hzptbb1_lowlpt_mu->Clone("hzptbbr12_lowlpt_mu");
  hzptbbr12_lowlpt_mu->SetTitle("Z PT / Photon PT Ratio Mu channel Low Lep. pT");
  hzptbbr12_lowlpt_mu->Divide(hzptbb2);
  hzptbbr12_lowlpt_mu->Scale(Norm_Mu);

  plots->Clear();
  hzptbbr12_lowlpt_mu->Draw();
  lumipt->Draw();
  sprintf(name, "%s.pdf", outtag.c_str());
  plots->Print(name);
  plots->Clear();




  // fine bin zpt hists
//  TH1D* hzptsb2 = new TH1D("hzptsb2", "hzptsb2", 500, 0, 1000);
//  TH1D* hzptsb1 = new TH1D("hzptsb1", "hzptsb1", 500, 0, 1000);
//  TH1D* hzptsb1_el = new TH1D("hzptsb1_el", "hzptsb1_el", 500, 0, 1000);
//  TH1D* hzptsb1_mu = new TH1D("hzptsb1_mu", "hzptsb1_mu", 500, 0, 1000);

//  hzptsb2->Sumw2();
//  hzptsb1->Sumw2();
//  hzptsb1_el->Sumw2();
//  hzptsb1_mu->Sumw2();

//  tree2->Draw("l1_pt>>hzptsb2", gjet_selec.c_str());
//  tree1->Draw("llnunu_l1_pt>>hzptsb1", zjet_selec.c_str());
//  tree1->Draw("llnunu_l1_pt>>hzptsb1_el", zjet_selec_el.c_str());
//  tree1->Draw("llnunu_l1_pt>>hzptsb1_mu", zjet_selec_mu.c_str());

//  hzptsb2->Scale(1./hzptsb2->Integral(), "width");
//  hzptsb1->Scale(1./hzptsb1->Integral(), "width");
//  hzptsb1_el->Scale(1./hzptsb1_el->Integral(), "width");
//  hzptsb1_mu->Scale(1./hzptsb1_mu->Integral(), "width");

//  TH1D* hzptsbr12 = (TH1D*)hzptsb1->Clone("hzptsbr12");
//  TH1D* hzptsbr12_el = (TH1D*)hzptsb1_el->Clone("hzptsbr12_el");
//  TH1D* hzptsbr12_mu = (TH1D*)hzptsb1_mu->Clone("hzptsbr12_mu");

//  hzptsbr12->Divide(hzptsb2);
//  hzptsbr12_el->Divide(hzptsb2);
//  hzptsbr12_mu->Divide(hzptsb2);



  // use TGraph with mean pt value to model
  TGraphErrors* gr_zpt_ratio = new TGraphErrors(pr_zpt_2->GetNbinsX());
  TGraphErrors* gr_zpt_ratio_el = new TGraphErrors(pr_zpt_2->GetNbinsX());
  TGraphErrors* gr_zpt_ratio_mu = new TGraphErrors(pr_zpt_2->GetNbinsX());
  TGraphErrors* gr_zpt_ratio_up = new TGraphErrors(pr_zpt_2->GetNbinsX());
  TGraphErrors* gr_zpt_ratio_dn = new TGraphErrors(pr_zpt_2->GetNbinsX());
  TGraphErrors* gr_zpt_ratio_el_up = new TGraphErrors(pr_zpt_2->GetNbinsX());
  TGraphErrors* gr_zpt_ratio_el_dn = new TGraphErrors(pr_zpt_2->GetNbinsX());
  TGraphErrors* gr_zpt_ratio_mu_up = new TGraphErrors(pr_zpt_2->GetNbinsX());
  TGraphErrors* gr_zpt_ratio_mu_dn = new TGraphErrors(pr_zpt_2->GetNbinsX());
  TGraphErrors* gr_zpt_lowlpt_ratio = new TGraphErrors(pr_zpt_2->GetNbinsX());
  TGraphErrors* gr_zpt_lowlpt_ratio_el = new TGraphErrors(pr_zpt_2->GetNbinsX());
  TGraphErrors* gr_zpt_lowlpt_ratio_mu = new TGraphErrors(pr_zpt_2->GetNbinsX());



  for (int i=0; i<pr_zpt_2->GetNbinsX(); i++){
    gr_zpt_ratio->SetPoint(i, pr_zpt_1->GetBinContent(i+1), hzptbbr12->GetBinContent(i+1));
    gr_zpt_ratio_el->SetPoint(i, pr_zpt_1_el->GetBinContent(i+1), hzptbbr12_el->GetBinContent(i+1));
    gr_zpt_ratio_mu->SetPoint(i, pr_zpt_1_mu->GetBinContent(i+1), hzptbbr12_mu->GetBinContent(i+1));
    gr_zpt_ratio_up->SetPoint(i, pr_zpt_1_up->GetBinContent(i+1), hzptbbr12_up->GetBinContent(i+1));
    gr_zpt_ratio_dn->SetPoint(i, pr_zpt_1_dn->GetBinContent(i+1), hzptbbr12_dn->GetBinContent(i+1));
    gr_zpt_ratio_el_up->SetPoint(i, pr_zpt_1_el_up->GetBinContent(i+1), hzptbbr12_el_up->GetBinContent(i+1));
    gr_zpt_ratio_el_dn->SetPoint(i, pr_zpt_1_el_dn->GetBinContent(i+1), hzptbbr12_el_dn->GetBinContent(i+1));
    gr_zpt_ratio_mu_up->SetPoint(i, pr_zpt_1_mu_up->GetBinContent(i+1), hzptbbr12_mu_up->GetBinContent(i+1));
    gr_zpt_ratio_mu_dn->SetPoint(i, pr_zpt_1_mu_dn->GetBinContent(i+1), hzptbbr12_mu_dn->GetBinContent(i+1));
    gr_zpt_lowlpt_ratio->SetPoint(i, pr_zpt_1_lowlpt->GetBinContent(i+1), hzptbbr12_lowlpt->GetBinContent(i+1));
    gr_zpt_lowlpt_ratio_el->SetPoint(i, pr_zpt_1_lowlpt_el->GetBinContent(i+1), hzptbbr12_lowlpt_el->GetBinContent(i+1));
    gr_zpt_lowlpt_ratio_mu->SetPoint(i, pr_zpt_1_lowlpt_mu->GetBinContent(i+1), hzptbbr12_lowlpt_mu->GetBinContent(i+1));

    gr_zpt_ratio->SetPointError(i, pr_zpt_1->GetBinError(i+1), hzptbbr12->GetBinError(i+1));
    gr_zpt_ratio_el->SetPointError(i, pr_zpt_1_el->GetBinError(i+1), hzptbbr12_el->GetBinError(i+1));
    gr_zpt_ratio_mu->SetPointError(i, pr_zpt_1_mu->GetBinError(i+1), hzptbbr12_mu->GetBinError(i+1));
    gr_zpt_ratio_up->SetPointError(i, pr_zpt_1_up->GetBinError(i+1), sqrt(pow(hzptbbr12_up->GetBinContent(i+1)-hzptbbr12->GetBinContent(i+1),2)+pow(hzptbbr12->GetBinError(i+1),2)));
    gr_zpt_ratio_dn->SetPointError(i, pr_zpt_1_dn->GetBinError(i+1), sqrt(pow(hzptbbr12_dn->GetBinContent(i+1)-hzptbbr12->GetBinContent(i+1),2)+pow(hzptbbr12->GetBinError(i+1),2)));
    gr_zpt_ratio_el_up->SetPointError(i, pr_zpt_1_el_up->GetBinError(i+1), sqrt(pow(hzptbbr12_el_up->GetBinContent(i+1)-hzptbbr12_el->GetBinContent(i+1),2)+pow(hzptbbr12_el->GetBinError(i+1),2)));
    gr_zpt_ratio_el_dn->SetPointError(i, pr_zpt_1_el_dn->GetBinError(i+1), sqrt(pow(hzptbbr12_el_dn->GetBinContent(i+1)-hzptbbr12_el->GetBinContent(i+1),2)+pow(hzptbbr12_el->GetBinError(i+1),2)));
    gr_zpt_ratio_mu_up->SetPointError(i, pr_zpt_1_mu_up->GetBinError(i+1), sqrt(pow(hzptbbr12_mu_up->GetBinContent(i+1)-hzptbbr12_mu->GetBinContent(i+1),2)+pow(hzptbbr12_mu->GetBinError(i+1),2)));
    gr_zpt_ratio_mu_dn->SetPointError(i, pr_zpt_1_mu_dn->GetBinError(i+1), sqrt(pow(hzptbbr12_mu_dn->GetBinContent(i+1)-hzptbbr12_mu->GetBinContent(i+1),2)+pow(hzptbbr12_mu->GetBinError(i+1),2)));
    gr_zpt_lowlpt_ratio->SetPointError(i, pr_zpt_1_lowlpt->GetBinError(i+1), hzptbbr12_lowlpt->GetBinError(i+1));
    gr_zpt_lowlpt_ratio_el->SetPointError(i, pr_zpt_1_lowlpt_el->GetBinError(i+1), hzptbbr12_lowlpt_el->GetBinError(i+1));
    gr_zpt_lowlpt_ratio_mu->SetPointError(i, pr_zpt_1_lowlpt_mu->GetBinError(i+1), hzptbbr12_lowlpt_mu->GetBinError(i+1));
  }



/*
  // 3D ZMass

  TH3D* hzmass_zpt_zrap = new TH3D("hzmass_zpt_zrap", "hzmass_zpt_zrap", NZMassBins, ZMassBins, NZPtBins,ZPtBins,NZRapBins,ZRapBins);
  TH3D* hzmass_zpt_zrap_el = new TH3D("hzmass_zpt_zrap_el", "hzmass_zpt_zrap_el", NZMassBins, ZMassBins, NZPtBins,ZPtBins,NZRapBins,ZRapBins);
  TH3D* hzmass_zpt_zrap_mu = new TH3D("hzmass_zpt_zrap_mu", "hzmass_zpt_zrap_mu", NZMassBins, ZMassBins, NZPtBins,ZPtBins,NZRapBins,ZRapBins);
  TH3D* hzmass_zpt_zrap_lowlpt = new TH3D("hzmass_zpt_zrap_lowlpt", "hzmass_zpt_zrap_lowlpt", NZMassBins, ZMassBins, NZPtBins,ZPtBins,NZRapBins,ZRapBins);
  TH3D* hzmass_zpt_zrap_lowlpt_el = new TH3D("hzmass_zpt_zrap_lowlpt_el", "hzmass_zpt_zrap_lowlpt_el", NZMassBins, ZMassBins, NZPtBins,ZPtBins,NZRapBins,ZRapBins);
  TH3D* hzmass_zpt_zrap_lowlpt_mu = new TH3D("hzmass_zpt_zrap_lowlpt_mu", "hzmass_zpt_zrap_lowlpt_mu", NZMassBins, ZMassBins, NZPtBins,ZPtBins,NZRapBins,ZRapBins);
  hzmass_zpt_zrap->Sumw2();
  hzmass_zpt_zrap_el->Sumw2();
  hzmass_zpt_zrap_mu->Sumw2();
  hzmass_zpt_zrap_lowlpt->Sumw2();
  hzmass_zpt_zrap_lowlpt_el->Sumw2();
  hzmass_zpt_zrap_lowlpt_mu->Sumw2();
  tree1->Draw("llnunu_l1_rapidity:llnunu_l1_pt:llnunu_l1_mass>>hzmass_zpt_zrap", zjet_selec.c_str());
  tree1->Draw("llnunu_l1_rapidity:llnunu_l1_pt:llnunu_l1_mass>>hzmass_zpt_zrap_el", zjet_selec_el.c_str());
  tree1->Draw("llnunu_l1_rapidity:llnunu_l1_pt:llnunu_l1_mass>>hzmass_zpt_zrap_mu", zjet_selec_mu.c_str());
  tree1->Draw("llnunu_l1_rapidity:llnunu_l1_pt:llnunu_l1_mass>>hzmass_zpt_zrap_lowlpt", zjet_selec_lowlpt.c_str());
  tree1->Draw("llnunu_l1_rapidity:llnunu_l1_pt:llnunu_l1_mass>>hzmass_zpt_zrap_lowlpt_el", zjet_selec_lowlpt_el.c_str());
  tree1->Draw("llnunu_l1_rapidity:llnunu_l1_pt:llnunu_l1_mass>>hzmass_zpt_zrap_lowlpt_mu", zjet_selec_lowlpt_mu.c_str());
*/
  // 2D ZMass
  TH2D* hzmass_zpt = new TH2D("hzmass_zpt", "hzmass_zpt", NZMassBins, ZMassBins, NZPtBins,ZPtBins);
  TH2D* hzmass_zpt_el = new TH2D("hzmass_zpt_el", "hzmass_zpt_el", NZMassBins, ZMassBins, NZPtBins,ZPtBins);
  TH2D* hzmass_zpt_mu = new TH2D("hzmass_zpt_mu", "hzmass_zpt_mu", NZMassBins, ZMassBins, NZPtBins,ZPtBins);
  TH2D* hzmass_zpt_lowlpt = new TH2D("hzmass_zpt_lowlpt", "hzmass_zpt_lowlpt", NZMassBins, ZMassBins, NZPtBins,ZPtBins);
  TH2D* hzmass_zpt_lowlpt_el = new TH2D("hzmass_zpt_lowlpt_el", "hzmass_zpt_lowlpt_el", NZMassBins, ZMassBins, NZPtBins,ZPtBins);
  TH2D* hzmass_zpt_lowlpt_mu = new TH2D("hzmass_zpt_lowlpt_mu", "hzmass_zpt_lowlpt_mu", NZMassBins, ZMassBins, NZPtBins,ZPtBins);
  hzmass_zpt->Sumw2();
  hzmass_zpt_el->Sumw2();
  hzmass_zpt_mu->Sumw2();
  hzmass_zpt_lowlpt->Sumw2();
  hzmass_zpt_lowlpt_el->Sumw2();
  hzmass_zpt_lowlpt_mu->Sumw2();
  tree1->Draw("llnunu_l1_pt:llnunu_l1_mass>>hzmass_zpt", zjet_selec.c_str());
  tree1->Draw("llnunu_l1_pt:llnunu_l1_mass>>hzmass_zpt_el", zjet_selec_el.c_str());
  tree1->Draw("llnunu_l1_pt:llnunu_l1_mass>>hzmass_zpt_mu", zjet_selec_mu.c_str());
  tree1->Draw("llnunu_l1_pt:llnunu_l1_mass>>hzmass_zpt_lowlpt", zjet_selec_lowlpt.c_str());
  tree1->Draw("llnunu_l1_pt:llnunu_l1_mass>>hzmass_zpt_lowlpt_el", zjet_selec_lowlpt_el.c_str());
  tree1->Draw("llnunu_l1_pt:llnunu_l1_mass>>hzmass_zpt_lowlpt_mu", zjet_selec_lowlpt_mu.c_str());


  //
  sprintf(name, "%s.root", outtag.c_str());
  TFile* fout = new TFile(name, "recreate");

  hzptbb1->Write("h_zpt_1");
  hzptbb2->Write("h_zpt_2");
  hzptbbr12->Write("h_zpt_ratio");

  hzptbb1_el->Write("h_zpt_1_el");
  hzptbbr12_el->Write("h_zpt_ratio_el");

  hzptbb1_mu->Write("h_zpt_1_mu");
  hzptbbr12_mu->Write("h_zpt_ratio_mu");

  hzptbb1_up->Write("h_zpt_1_up");
  hzptbbr12_up->Write("h_zpt_ratio_up");

  hzptbb1_el_up->Write("h_zpt_1_el_up");
  hzptbbr12_el_up->Write("h_zpt_ratio_el_up");

  hzptbb1_mu_up->Write("h_zpt_1_mu_up");
  hzptbbr12_mu_up->Write("h_zpt_ratio_mu_up");

  hzptbb1_dn->Write("h_zpt_1_dn");
  hzptbbr12_dn->Write("h_zpt_ratio_dn");

  hzptbb1_el_dn->Write("h_zpt_1_el_dn");
  hzptbbr12_el_dn->Write("h_zpt_ratio_el_dn");

  hzptbb1_mu_dn->Write("h_zpt_1_mu_dn");
  hzptbbr12_mu_dn->Write("h_zpt_ratio_mu_dn");


  hzptbb1_lowlpt->Write("h_zpt_lowlpt_1");
  hzptbbr12_lowlpt->Write("h_zpt_lowlpt_ratio");

  hzptbb1_lowlpt_el->Write("h_zpt_lowlpt_1_el");
  hzptbbr12_lowlpt_el->Write("h_zpt_lowlpt_ratio_el");

  hzptbb1_lowlpt_mu->Write("h_zpt_lowlpt_1_mu");
  hzptbbr12_lowlpt_mu->Write("h_zpt_lowlpt_ratio_mu");

  pr_zpt_2->Write("pr_zpt_2");
  pr_zpt_1->Write("pr_zpt_1");
  pr_zpt_1_el->Write("pr_zpt_1_el");
  pr_zpt_1_mu->Write("pr_zpt_1_mu");
  pr_zpt_1_up->Write("pr_zpt_1_up");
  pr_zpt_1_el_up->Write("pr_zpt_1_el_up");
  pr_zpt_1_mu_up->Write("pr_zpt_1_mu_up");
  pr_zpt_1_dn->Write("pr_zpt_1_dn");
  pr_zpt_1_el_dn->Write("pr_zpt_1_el_dn");
  pr_zpt_1_mu_dn->Write("pr_zpt_1_mu_dn");

  pr_zpt_1_lowlpt->Write("pr_zpt_1_lowlpt");
  pr_zpt_1_lowlpt_el->Write("pr_zpt_1_lowlpt_el");
  pr_zpt_1_lowlpt_mu->Write("pr_zpt_1_lowlpt_mu");


//  hzptsb2->Write("h_zpt_2_sb");
//  hzptsb1->Write("h_zpt_1_sb");
//  hzptsb1_el->Write("h_zpt_1_el_sb");
//  hzptsb1_mu->Write("h_zpt_1_mu_sb");

//  hzptsbr12->Write("h_zpt_ratio_sb");
//  hzptsbr12_el->Write("h_zpt_ratio_el_sb");
//  hzptsbr12_mu->Write("h_zpt_ratio_mu_sb");

  gr_zpt_ratio->Write("gr_zpt_ratio");
  gr_zpt_ratio_el->Write("gr_zpt_ratio_el");
  gr_zpt_ratio_mu->Write("gr_zpt_ratio_mu");
  gr_zpt_ratio_up->Write("gr_zpt_ratio_up");
  gr_zpt_ratio_dn->Write("gr_zpt_ratio_dn");
  gr_zpt_ratio_el_up->Write("gr_zpt_ratio_el_up");
  gr_zpt_ratio_el_dn->Write("gr_zpt_ratio_el_dn");
  gr_zpt_ratio_mu_up->Write("gr_zpt_ratio_mu_up");
  gr_zpt_ratio_mu_dn->Write("gr_zpt_ratio_mu_dn");
  gr_zpt_lowlpt_ratio->Write("gr_zpt_lowlpt_ratio");
  gr_zpt_lowlpt_ratio_el->Write("gr_zpt_lowlpt_ratio_el");
  gr_zpt_lowlpt_ratio_mu->Write("gr_zpt_lowlpt_ratio_mu");


/*
  hzmass_zpt_zrap->Write("h_zmass_zpt_zrap");
  hzmass_zpt_zrap_el->Write("h_zmass_zpt_zrap_el");
  hzmass_zpt_zrap_mu->Write("h_zmass_zpt_zrap_mu");
  hzmass_zpt_zrap_lowlpt->Write("h_zmass_zpt_zrap_lowlpt");
  hzmass_zpt_zrap_lowlpt_el->Write("h_zmass_zpt_zrap_lowlpt_el");
  hzmass_zpt_zrap_lowlpt_mu->Write("h_zmass_zpt_zrap_lowlpt_mu");
*/

  hzmass_zpt->Write("h_zmass_zpt");
  hzmass_zpt_el->Write("h_zmass_zpt_el");
  hzmass_zpt_mu->Write("h_zmass_zpt_mu");
  hzmass_zpt_lowlpt->Write("h_zmass_zpt_lowlpt");
  hzmass_zpt_lowlpt_el->Write("h_zmass_zpt_lowlpt_el");
  hzmass_zpt_lowlpt_mu->Write("h_zmass_zpt_lowlpt_mu");


  // 
  fout->Close();

  sprintf(name, "%s.pdf]", outtag.c_str());
  plots->Print(name);


  return 0; 
}
