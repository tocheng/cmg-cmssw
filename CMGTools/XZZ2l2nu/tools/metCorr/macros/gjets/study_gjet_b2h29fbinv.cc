#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TH1D.h"
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
#include "TROOT.h"

// Hengne Li @ CERN, 2016

int main(int argc, char** argv) {


  //TFile* file1 = TFile::Open("/home/heli/XZZ/80X_20160825_light_Skim/DYJetsToLL_M50_BIG.root");
  //TFile* file1 = TFile::Open("/home/heli/XZZ/80X_20161006_light_Skim/DYJetsToLL_M50_BIG_NoRecoil.root");
  TFile* file1 = TFile::Open("/home/heli/XZZ/80X_20161018_light_Skim/DYJetsToLL_M50_BIG_NoRecoil.root");
  TFile* file2 = TFile::Open("/home/heli/XZZ/80X_20161018_light_Skim/SinglePhoton_Run2016B2H29fbinv_PromptReco_newFilterLepVetoNoRecoil.root");
  //TFile* file2 = TFile::Open("/home/heli/XZZ/80X_20161006_light_Skim/SinglePhoton_Run2016B2G_PromptReco_newFilterLepVetoNoRecoil.root");
  //TFile* file2 = TFile::Open("/home/heli/XZZ/80X_20161006_light/SinglePhoton_Run2016B2G_PromptReco/vvTreeProducer/tree.root");
  //TFile* file2 = TFile::Open("/home/heli/XZZ/80X_20160810_light/GJet_Pt_20toInf_DoubleEMEnriched/vvTreeProducer/tree.root");
  //TFile* file2 = TFile::Open("/home/heli/XZZ/80X_20160810_light/SinglePhoton_Run2016BCD_PromptReco/vvTreeProducer/tree.root");
  //TFile* file2 = TFile::Open("/home/heli/XZZ/80X_20160927_light/SinglePhoton_Run2016BCD_PromptReco/vvTreeProducer/tree.root");

  std::string outtag="study_gjets_data_b2h29fbinv_newFilterLepVeto";
  //std::string outtag="study_gjets_data_b2g_newFilterLepVeto";
  //std::string outtag="study_gjets_tight";
  //std::string outtag="study_gjets";
  //std::string outtag="study_gjets_data";
  //std::string outtag="study_gjets_data_hlt";
  //std::string outtag="study_gjets_data_hlt_dtscale_nohlt90";
  //std::string outtag="study_gjets_data_hlt_dtscale_flag3_f2_sIetaCut";
  //std::string outtag="study_gjets_data_newFilterLepVeto";


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

  //
  //tree2->SetAlias("metPara", "gjet_l2_pt*cos(gjet_l2_phi-gjet_l1_phi)");
  //tree2->SetAlias("metPerp", "gjet_l2_pt*sin(gjet_l2_phi-gjet_l1_phi)");
  //tree2->SetAlias("absDeltaPhi", "fabs(TVector2::Phi_mpi_pi(gjet_l2_phi-gjet_l1_phi))");
  //tree2->SetAlias("eta", "gjet_l1_eta");
  //tree2->SetAlias("phi", "gjet_l1_phi");
  //tree2->SetAlias("flag2", "(!(eta>0&&eta<0.15&&phi>0.52&&phi<0.56)&&!(eta>-2.5&&eta<-1.4&&phi>-0.5&&phi<0.5)&&!(eta>1.5&&eta<2.5&&phi>-0.5&&phi<0.5)&&!(eta>1.4&&eta<1.6&&phi>-0.8&&phi<-0.5)&&!(eta>1.4&&eta<2.5&&phi>2.5&&phi<4)&&!(eta>1.4&&eta<2.5&&phi>-4&&phi<-2.5)&&!(eta>-2.5&&eta<-1.4&&phi>2.5&&phi<4)&&!(eta>-2.5&&eta<-1.4&&phi>-4&&phi<-2.5)&&!(eta>2.3&&eta<2.6&&phi>-2.5&&phi<-2.2)&&!(eta>0.2&&eta<0.3&&phi>-2.6&&phi<-2.5)&&!(eta>0.5&&eta<0.7&&phi>-1.5&&phi<-1.2)&&!(eta>-0.85&&eta<-0.7&&phi>-1.8&&phi<-1.4)&&!(eta<-2.4&&eta>-2.5&&phi<-1.75&&phi>-1.9)&&!(eta>-2.5&&eta<-2.4&&phi>-1.2&&phi<-1.1)&&!(eta>-2.4&&eta<-2.3&&phi>-2.4&&phi<-2.3))");

  // define cuts
  std::string metfilter="(Flag_EcalDeadCellTriggerPrimitiveFilter&&Flag_HBHENoiseIsoFilter&&Flag_goodVertices&&Flag_HBHENoiseFilter&&Flag_globalTightHalo2016Filter&&Flag_eeBadScFilter)";
  std::string cuts_lepaccept="((abs(llnunu_l1_l1_pdgId)==13&&abs(llnunu_l1_l2_pdgId)==13&&llnunu_l1_l1_pt>50&&abs(llnunu_l1_l1_eta)<2.4&&llnunu_l1_l2_pt>20&&abs(llnunu_l1_l2_eta)<2.4&&(llnunu_l1_l1_highPtID==1||llnunu_l1_l2_highPtID==1))";
  cuts_lepaccept+="||(abs(llnunu_l1_l1_pdgId)==11&&abs(llnunu_l1_l2_pdgId)==11&&llnunu_l1_l1_pt>115&&abs(llnunu_l1_l1_eta)<2.5&&llnunu_l1_l2_pt>35&&abs(llnunu_l1_l2_eta)<2.5))";
  std::string cuts_lepaccept_lowlpt="((abs(llnunu_l1_l1_pdgId)==13&&abs(llnunu_l1_l2_pdgId)==13&&llnunu_l1_l1_pt>20&&abs(llnunu_l1_l1_eta)<2.4&&llnunu_l1_l2_pt>20&&abs(llnunu_l1_l2_eta)<2.4&&(llnunu_l1_l1_highPtID==1||llnunu_l1_l2_highPtID==1))";
  cuts_lepaccept_lowlpt+="||(abs(llnunu_l1_l1_pdgId)==11&&abs(llnunu_l1_l2_pdgId)==11&&llnunu_l1_l1_pt>20&&abs(llnunu_l1_l1_eta)<2.5&&llnunu_l1_l2_pt>20&&abs(llnunu_l1_l2_eta)<2.5))";
  std::string cuts_zmass="(llnunu_l1_mass>70&&llnunu_l1_mass<110)";
  std::string cuts_loose_z="("+metfilter+"&&"+cuts_lepaccept+"&&"+cuts_zmass+")";
  std::string cuts_loose_z_lowlpt="("+metfilter+"&&"+cuts_lepaccept_lowlpt+"&&"+cuts_zmass+")";


  std::string base_selec =  cuts_loose_z;
  std::string base_selec_lowlpt =  cuts_loose_z_lowlpt;

  std::string base_selec_el = "(" + cuts_loose_z + "&&(abs(llnunu_l1_l1_pdgId)==11&&abs(llnunu_l1_l2_pdgId)==11))";
  std::string base_selec_mu = "(" + cuts_loose_z + "&&(abs(llnunu_l1_l1_pdgId)==13&&abs(llnunu_l1_l2_pdgId)==13))";
  std::string base_selec_lowlpt_el = "(" + cuts_loose_z_lowlpt + "&&(abs(llnunu_l1_l1_pdgId)==11&&abs(llnunu_l1_l2_pdgId)==11))";
  std::string base_selec_lowlpt_mu = "(" + cuts_loose_z_lowlpt + "&&(abs(llnunu_l1_l1_pdgId)==13&&abs(llnunu_l1_l2_pdgId)==13))";

  // add weight
  std::string weight_selec = std::string("*(genWeight/SumWeights*ZPtWeight*puWeight68075*1921.8*3)");
  std::string weight_selec_up = std::string("*(genWeight/SumWeights*ZPtWeight_up*puWeight68075*1921.8*3)");
  std::string weight_selec_dn = std::string("*(genWeight/SumWeights*ZPtWeight_dn*puWeight68075*1921.8*3)");
  //std::string weight_selec = std::string("*(ZJetsGenWeight*ZPtWeight*puWeight68075*1921.8*3)");
  //std::string weight_selec_up = std::string("*(ZJetsGenWeight*ZPtWeight_up*puWeight68075*1921.8*3)");
  //std::string weight_selec_dn = std::string("*(ZJetsGenWeight*ZPtWeight_dn*puWeight68075*1921.8*3)");
  //std::string weight_selec = std::string("*(ZJetsGenWeight*ZPtWeight*1921.8*3*12900.0)");
  // rho weight
  //std::string rhoweight_selec = std::string("*(0.602*exp(-0.5*pow((rho-8.890)/6.187,2))+0.829*exp(-0.5*pow((rho-21.404)/10.866,2)))");
  //std::string rhoweight_selec = "*(0.232+0.064*rho)";  // for b-g 27.22fb-l
  std::string rhoweight_selec = "*(0.038+0.118*rho-4.329e-03*rho*rho+1.011e-04*rho*rho*rho)"; // for b-h 29.53 fb-1
  //std::string rhoweight_selec = "*(1)";
  // scale factors
  std::string effsf_selec = std::string("*(trgsf*isosf*idsf*trksf)");
  //std::string effsf_selec = std::string("*(trgsf*isosf*idsf*trksf*(1*(abs(llnunu_l1_l1_pdgId)==13&&abs(llnunu_l1_l2_pdgId)==13)+1.06*(abs(llnunu_l1_l1_pdgId)==11&&abs(llnunu_l1_l2_pdgId)==11)))");

  // selec, cuts + weights
  std::string zjet_selec = base_selec + weight_selec + rhoweight_selec + effsf_selec;
  std::string zjet_selec_el = base_selec_el + weight_selec + rhoweight_selec + effsf_selec;
  std::string zjet_selec_mu = base_selec_mu + weight_selec + rhoweight_selec + effsf_selec;
  std::string zjet_selec_lowlpt = base_selec_lowlpt + weight_selec + rhoweight_selec + effsf_selec;
  std::string zjet_selec_lowlpt_el = base_selec_lowlpt_el + weight_selec + rhoweight_selec + effsf_selec;
  std::string zjet_selec_lowlpt_mu = base_selec_lowlpt_mu + weight_selec + rhoweight_selec + effsf_selec;

  std::string zjet_selec_up = base_selec + weight_selec_up + rhoweight_selec + effsf_selec;
  std::string zjet_selec_el_up = base_selec_el + weight_selec_up + rhoweight_selec + effsf_selec;
  std::string zjet_selec_mu_up = base_selec_mu + weight_selec_up + rhoweight_selec + effsf_selec;
  std::string zjet_selec_lowlpt_up = base_selec_lowlpt + weight_selec_up + rhoweight_selec + effsf_selec;
  std::string zjet_selec_lowlpt_el_up = base_selec_lowlpt_el + weight_selec_up + rhoweight_selec + effsf_selec;
  std::string zjet_selec_lowlpt_mu_up = base_selec_lowlpt_mu + weight_selec_up + rhoweight_selec + effsf_selec;

  std::string zjet_selec_dn = base_selec + weight_selec_dn + rhoweight_selec + effsf_selec;
  std::string zjet_selec_el_dn = base_selec_el + weight_selec_dn + rhoweight_selec + effsf_selec;
  std::string zjet_selec_mu_dn = base_selec_mu + weight_selec_dn + rhoweight_selec + effsf_selec;
  std::string zjet_selec_lowlpt_dn = base_selec_lowlpt + weight_selec_dn + rhoweight_selec + effsf_selec;
  std::string zjet_selec_lowlpt_el_dn = base_selec_lowlpt_el + weight_selec_dn + rhoweight_selec + effsf_selec;
  std::string zjet_selec_lowlpt_mu_dn = base_selec_lowlpt_mu + weight_selec_dn + rhoweight_selec + effsf_selec;



  //std::string gjet_selec = metfilter + rhoweight_selec;
  //std::string gjet_selec = "("+metfilter+"&&HLT_PHOTONHZZ)";
  //std::string gjet_selec = "("+metfilter+"&&(HLT_PHOTONIDISO&&!HLT_PHOTONIDISO90))";
  //std::string gjet_selec = "("+metfilter+"&&HLT_PHOTONIDISO)";
  //std::string gjet_selec = "("+metfilter+"&&HLT_PHOTONIDISO&&(!(absDeltaPhi>3.0&&metPara/gjet_l1_pt>-1.5&&metPara/gjet_l1_pt<-0.5)))";
  //std::string gjet_selec = "("+metfilter+"&&HLT_PHOTONIDISO&&(!(metPara/gjet_l1_pt<-0.8&&metPara/gjet_l1_pt>-1.8&&fabs(metPerp/gjet_l1_pt)<0.3)))";

  //std::string gjet_selec = "("+metfilter+"&&HLT_PHOTONIDISO&&fabs(gjet_l1_eta)<1.47)";
  //std::string gjet_selec = "("+metfilter+"&&HLT_PHOTONIDISO&&flag2)";
  //std::string gjet_selec = "(phi>-1&&phi<2&&fabs(eta)<1.0)"; // input ntuple preselected. 
  //std::string gjet_selec = "(1)"; // input ntuple preselected. 
  std::string gjet_selec = "(1)*(GJetsRhoWeight)"; // input ntuple preselected. 

  //Double_t ZPtBins[] = {0,1.25,2.5,3.75,5,6.25,7.5,8.75,10,11.25,12.5,15,17.5,20,25,30,35,40,45,50,60,70,80,90,100,110,130,150,170,190,220,250,400,1000};
  Double_t ZPtBins[] = {20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,65,70,75,80,85,90,95,100,105,110,115,120,125,130,135,140,145,150,155,160,165,170,175,180,185,190,195,200,215,220,225,230,235,240,245,250,255,260,265,270,275,280,285,290,295,300,350,400,500,700,3000};
  Int_t NZPtBins = sizeof(ZPtBins)/sizeof(ZPtBins[0]) - 1;
  Double_t ZMassBins[] = {50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,176,177,178,179,180};
  Int_t NZMassBins = sizeof(ZMassBins)/sizeof(ZMassBins[0]) - 1;
  //Double_t ZRapBins[] = {-3.0,-2.9,-2.8,-2.7,-2.6,-2.5,-2.4,-2.3,-2.2,-2.1,-2.0,-1.9,-1.8,-1.7,-1.6,-1.5,-1.4,-1.3,-1.2,-1.1,-1.0,-0.9,-0.8,-0.7,-0.6,-0.5,-0.4,-0.3,-0.2,-0.1,0.0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0,1.1,1.2,1.3,1.4,1.5,1.6,1.7,1.8,1.9,2.0,2.1,2.2,2.3,2.4,2.5,2.6,2.7,2.8,2.9,3.0};
  Double_t ZRapBins[] = {-2.5,-2.4,-2.3,-2.2,-2.1,-2.0,-1.9,-1.8,-1.7,-1.6,-1.5,-1.4,-1.3,-1.2,-1.1,-1.0,-0.9,-0.8,-0.7,-0.6,-0.5,-0.4,-0.3,-0.2,-0.1,0.0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0,1.1,1.2,1.3,1.4,1.5,1.6,1.7,1.8,1.9,2.0,2.1,2.2,2.3,2.4,2.5};
  //Double_t ZRapBins[] = {-2.5,-2.3,-2.1,-1.9,-1.7,-1.5,-1.3,-1.1,-0.9,-0.7,-0.5,-0.3,-0.1,0.1,0.3,0.5,0.7,0.9,1.1,1.3,1.5,1.7,1.9,2.1,2.3,2.5};
  Int_t NZRapBins = sizeof(ZRapBins)/sizeof(ZRapBins[0]) - 1;

  // all
  TH1D* hzptbb1 = new TH1D("hzptbb1", "hzptbb1", NZPtBins, ZPtBins);
  TH1D* hzptbb2 = new TH1D("hzptbb2", "hzptbb2", NZPtBins, ZPtBins);

  hzptbb1->Sumw2();
  hzptbb2->Sumw2();


  tree1->Draw("llnunu_l1_pt>>hzptbb1", zjet_selec.c_str());
  tree2->Draw("l1_pt>>hzptbb2", gjet_selec.c_str());

  hzptbb1->Scale(1./hzptbb1->Integral(), "width");
  //hzptbb2->Scale(1./hzptbb2->Integral(), "width");
  hzptbb2->Scale(1., "width");

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
  sprintf(name, "%s.pdf", outtag.c_str());
  plots->Print(name);
  plots->Clear();

  TH1D* hzptbbr12 = (TH1D*)hzptbb1->Clone("hzptbbr12");
  hzptbbr12->Divide(hzptbb2);
  hzptbbr12->Scale(Norm_All);

  plots->Clear();
  hzptbbr12->Draw();
  sprintf(name, "%s.pdf", outtag.c_str());
  plots->Print(name);
  plots->Clear();

  // el
  TH1D* hzptbb1_el = new TH1D("hzptbb1_el", "hzptbb1_el", NZPtBins, ZPtBins);
  hzptbb1_el->Sumw2();
  tree1->Draw("llnunu_l1_pt>>hzptbb1_el", zjet_selec_el.c_str());
  hzptbb1_el->Scale(1./hzptbb1_el->Integral(), "width");

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
  sprintf(name, "%s.pdf", outtag.c_str());
  plots->Print(name);
  plots->Clear();

  TH1D* hzptbbr12_el = (TH1D*)hzptbb1_el->Clone("hzptbbr12_el");
  hzptbbr12_el->Divide(hzptbb2);
  hzptbbr12_el->Scale(Norm_El);

  plots->Clear();
  hzptbbr12_el->Draw();
  sprintf(name, "%s.pdf", outtag.c_str());
  plots->Print(name);
  plots->Clear();


  // mu
  TH1D* hzptbb1_mu = new TH1D("hzptbb1_mu", "hzptbb1_mu", NZPtBins, ZPtBins);
  hzptbb1_mu->Sumw2();
  tree1->Draw("llnunu_l1_pt>>hzptbb1_mu", zjet_selec_mu.c_str());
  hzptbb1_mu->Scale(1./hzptbb1_mu->Integral(), "width");

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
  sprintf(name, "%s.pdf", outtag.c_str());
  plots->Print(name);
  plots->Clear();

  TH1D* hzptbbr12_mu = (TH1D*)hzptbb1_mu->Clone("hzptbbr12_mu");
  hzptbbr12_mu->Divide(hzptbb2);
  hzptbbr12_mu->Scale(Norm_Mu);

  plots->Clear();
  hzptbbr12_mu->Draw();
  sprintf(name, "%s.pdf", outtag.c_str());
  plots->Print(name);
  plots->Clear();


  // all
  TH1D* hzptbb1_lowlpt = new TH1D("hzptbb1_lowlpt", "hzptbb1_lowlpt", NZPtBins, ZPtBins);
  hzptbb1_lowlpt->Sumw2();
  tree1->Draw("llnunu_l1_pt>>hzptbb1_lowlpt", zjet_selec_lowlpt.c_str());
  hzptbb1_lowlpt->Scale(1./hzptbb1_lowlpt->Integral(), "width");

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
  sprintf(name, "%s.pdf", outtag.c_str());
  plots->Print(name);
  plots->Clear();

  TH1D* hzptbbr12_lowlpt = (TH1D*)hzptbb1_lowlpt->Clone("hzptbbr12_lowlpt");
  hzptbbr12_lowlpt->Divide(hzptbb2);
  hzptbbr12_lowlpt->Scale(Norm_All);

  plots->Clear();
  hzptbbr12_lowlpt->Draw();
  sprintf(name, "%s.pdf", outtag.c_str());
  plots->Print(name);
  plots->Clear();

  // lowlpt el
  TH1D* hzptbb1_lowlpt_el = new TH1D("hzptbb1_lowlpt_el", "hzptbb1_lowlpt_el", NZPtBins, ZPtBins);
  hzptbb1_lowlpt_el->Sumw2();
  tree1->Draw("llnunu_l1_pt>>hzptbb1_lowlpt_el", zjet_selec_lowlpt_el.c_str());
  hzptbb1_lowlpt_el->Scale(1./hzptbb1_lowlpt_el->Integral(), "width");

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
  sprintf(name, "%s.pdf", outtag.c_str());
  plots->Print(name);
  plots->Clear();

  TH1D* hzptbbr12_lowlpt_el = (TH1D*)hzptbb1_lowlpt_el->Clone("hzptbbr12_lowlpt_el");
  hzptbbr12_lowlpt_el->Divide(hzptbb2);
  hzptbbr12_lowlpt_el->Scale(Norm_El);

  plots->Clear();
  hzptbbr12_lowlpt_el->Draw();
  sprintf(name, "%s.pdf", outtag.c_str());
  plots->Print(name);
  plots->Clear();


  // lowlpt  mu
  TH1D* hzptbb1_lowlpt_mu = new TH1D("hzptbb1_lowlpt_mu", "hzptbb1_lowlpt_mu", NZPtBins, ZPtBins);
  hzptbb1_lowlpt_mu->Sumw2();
  tree1->Draw("llnunu_l1_pt>>hzptbb1_lowlpt_mu", zjet_selec_lowlpt_mu.c_str());
  hzptbb1_lowlpt_mu->Scale(1./hzptbb1_lowlpt_mu->Integral(), "width");

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
  sprintf(name, "%s.pdf", outtag.c_str());
  plots->Print(name);
  plots->Clear();

  TH1D* hzptbbr12_lowlpt_mu = (TH1D*)hzptbb1_lowlpt_mu->Clone("hzptbbr12_lowlpt_mu");
  hzptbbr12_lowlpt_mu->Divide(hzptbb2);
  hzptbbr12_lowlpt_mu->Scale(Norm_Mu);

  plots->Clear();
  hzptbbr12_lowlpt_mu->Draw();
  sprintf(name, "%s.pdf", outtag.c_str());
  plots->Print(name);
  plots->Clear();


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


  // 2D zpt zrap
  TH2D* hzpt_zrap1 = new TH2D("hzpt_zrap1", "hzpt_zrap1", NZPtBins,ZPtBins,NZRapBins,ZRapBins);
  TH2D* hzpt_zrap2 = new TH2D("hzpt_zrap2", "hzpt_zrap2", NZPtBins,ZPtBins,NZRapBins,ZRapBins);

  hzpt_zrap1->Sumw2();
  hzpt_zrap2->Sumw2();


  tree1->Draw("llnunu_l1_rapidity:llnunu_l1_pt>>hzpt_zrap1", zjet_selec.c_str());
  tree2->Draw("l1_rapidity:l1_pt>>hzpt_zrap2", gjet_selec.c_str());

  hzpt_zrap1->Scale(1.0/hzpt_zrap1->Integral(),"width");
  hzpt_zrap2->Scale(1.0,"width");


  TH2D* hzpt_zrap_r12 = (TH2D*)hzpt_zrap1->Clone("hzpt_zrap_r12");
  hzpt_zrap_r12->Divide(hzpt_zrap2);
  hzpt_zrap_r12->Scale(Norm_All);


  plots->Clear();
  hzpt_zrap1->Draw("colz");
  sprintf(name, "%s.pdf", outtag.c_str());
  plots->Print(name);
  plots->Clear();

  plots->Clear();
  hzpt_zrap2->Draw("colz");
  sprintf(name, "%s.pdf", outtag.c_str());
  plots->Print(name);
  plots->Clear();

  plots->Clear();
  hzpt_zrap_r12->Draw("colz");
  sprintf(name, "%s.pdf", outtag.c_str());
  plots->Print(name);
  plots->Clear();



  // 2D zpt zrap el
  TH2D* hzpt_zrap1_el = new TH2D("hzpt_zrap1_el", "hzpt_zrap1_el", NZPtBins,ZPtBins,NZRapBins,ZRapBins);
  hzpt_zrap1_el->Sumw2();
  tree1->Draw("llnunu_l1_rapidity:llnunu_l1_pt>>hzpt_zrap1_el", zjet_selec_el.c_str());
  hzpt_zrap1_el->Scale(1.0/hzpt_zrap1_el->Integral(),"width");
  TH2D* hzpt_zrap_r12_el = (TH2D*)hzpt_zrap1_el->Clone("hzpt_zrap_r12_el");
  hzpt_zrap_r12_el->Divide(hzpt_zrap2);
  hzpt_zrap_r12_el->Scale(Norm_El);

  plots->Clear();
  hzpt_zrap1_el->Draw("colz");
  sprintf(name, "%s.pdf", outtag.c_str());
  plots->Print(name);
  plots->Clear();

  plots->Clear();
  hzpt_zrap_r12_el->Draw("colz");
  sprintf(name, "%s.pdf", outtag.c_str());
  plots->Print(name);
  plots->Clear();


  // 2D zpt zrap mu
  TH2D* hzpt_zrap1_mu = new TH2D("hzpt_zrap1_mu", "hzpt_zrap1_mu", NZPtBins,ZPtBins,NZRapBins,ZRapBins);
  hzpt_zrap1_mu->Sumw2();
  tree1->Draw("llnunu_l1_rapidity:llnunu_l1_pt>>hzpt_zrap1_mu", zjet_selec_mu.c_str());
  hzpt_zrap1_mu->Scale(1.0/hzpt_zrap1_mu->Integral(),"width");
  TH2D* hzpt_zrap_r12_mu = (TH2D*)hzpt_zrap1_mu->Clone("hzpt_zrap_r12_mu");
  hzpt_zrap_r12_mu->Divide(hzpt_zrap2);
  hzpt_zrap_r12_mu->Scale(Norm_Mu);

  plots->Clear();
  hzpt_zrap1_mu->Draw("colz");
  sprintf(name, "%s.pdf", outtag.c_str());
  plots->Print(name);
  plots->Clear();

  plots->Clear();
  hzpt_zrap_r12_mu->Draw("colz");
  sprintf(name, "%s.pdf", outtag.c_str());
  plots->Print(name);
  plots->Clear();


  // 2D zpt zrap _lowlpt
  TH2D* hzpt_zrap1_lowlpt = new TH2D("hzpt_zrap1_lowlpt", "hzpt_zrap1_lowlpt", NZPtBins,ZPtBins,NZRapBins,ZRapBins);
  hzpt_zrap1_lowlpt->Sumw2();
  tree1->Draw("llnunu_l1_rapidity:llnunu_l1_pt>>hzpt_zrap1_lowlpt", zjet_selec_lowlpt.c_str());
  hzpt_zrap1_lowlpt->Scale(1.0/hzpt_zrap1->Integral(),"width");
  TH2D* hzpt_zrap_lowlpt_r12 = (TH2D*)hzpt_zrap1_lowlpt->Clone("hzpt_zrap_lowlpt_r12");
  hzpt_zrap_lowlpt_r12->Divide(hzpt_zrap2);
  hzpt_zrap_lowlpt_r12->Scale(Norm_All);


  plots->Clear();
  hzpt_zrap1_lowlpt->Draw("colz");
  sprintf(name, "%s.pdf", outtag.c_str());
  plots->Print(name);
  plots->Clear();

  plots->Clear();
  hzpt_zrap_lowlpt_r12->Draw("colz");
  sprintf(name, "%s.pdf", outtag.c_str());
  plots->Print(name);
  plots->Clear();



  // 2D zpt zrap lowlpt el
  TH2D* hzpt_zrap1_lowlpt_el = new TH2D("hzpt_zrap1_lowlpt_el", "hzpt_zrap1_lowlpt_el", NZPtBins,ZPtBins,NZRapBins,ZRapBins);
  hzpt_zrap1_lowlpt_el->Sumw2();
  tree1->Draw("llnunu_l1_rapidity:llnunu_l1_pt>>hzpt_zrap1_lowlpt_el", zjet_selec_lowlpt_el.c_str());
  hzpt_zrap1_lowlpt_el->Scale(1.0/hzpt_zrap1->Integral(),"width");
  TH2D* hzpt_zrap_lowlpt_r12_el = (TH2D*)hzpt_zrap1_lowlpt_el->Clone("hzpt_zrap_lowlpt_r12_el");
  hzpt_zrap_lowlpt_r12_el->Divide(hzpt_zrap2);
  hzpt_zrap_lowlpt_r12_el->Scale(Norm_El);

  plots->Clear();
  hzpt_zrap1_lowlpt_el->Draw("colz");
  sprintf(name, "%s.pdf", outtag.c_str());
  plots->Print(name);
  plots->Clear();

  plots->Clear();
  hzpt_zrap_lowlpt_r12_el->Draw("colz");
  sprintf(name, "%s.pdf", outtag.c_str());
  plots->Print(name);
  plots->Clear();


  // 2D zpt zrap lowlpt mu
  TH2D* hzpt_zrap1_lowlpt_mu = new TH2D("hzpt_zrap1_lowlpt_mu", "hzpt_zrap1_lowlpt_mu", NZPtBins,ZPtBins,NZRapBins,ZRapBins);
  hzpt_zrap1_lowlpt_mu->Sumw2();
  tree1->Draw("llnunu_l1_rapidity:llnunu_l1_pt>>hzpt_zrap1_lowlpt_mu", zjet_selec_lowlpt_mu.c_str());
  hzpt_zrap1_lowlpt_mu->Scale(1.0/hzpt_zrap1->Integral(),"width");
  TH2D* hzpt_zrap_lowlpt_r12_mu = (TH2D*)hzpt_zrap1_lowlpt_mu->Clone("hzpt_zrap_lowlpt_r12_mu");
  hzpt_zrap_lowlpt_r12_mu->Divide(hzpt_zrap2);
  hzpt_zrap_lowlpt_r12_mu->Scale(Norm_Mu);

  plots->Clear();
  hzpt_zrap1_lowlpt_mu->Draw("colz");
  sprintf(name, "%s.pdf", outtag.c_str());
  plots->Print(name);
  plots->Clear();

  plots->Clear();
  hzpt_zrap_lowlpt_r12_mu->Draw("colz");
  sprintf(name, "%s.pdf", outtag.c_str());
  plots->Print(name);
  plots->Clear();



  // 2D zpt zrap up
  TH2D* hzpt_zrap1_up = new TH2D("hzpt_zrap1_up", "hzpt_zrap1_up", NZPtBins,ZPtBins,NZRapBins,ZRapBins);

  hzpt_zrap1_up->Sumw2();
  tree1->Draw("llnunu_l1_rapidity:llnunu_l1_pt>>hzpt_zrap1_up", zjet_selec_up.c_str());
  hzpt_zrap1_up->Scale(1.0/hzpt_zrap1_up->Integral(),"width");

  TH2D* hzpt_zrap_r12_up = (TH2D*)hzpt_zrap1_up->Clone("hzpt_zrap_r12_up");
  hzpt_zrap_r12_up->Divide(hzpt_zrap2);
  hzpt_zrap_r12_up->Scale(Norm_All);


  plots->Clear();
  hzpt_zrap1_up->Draw("colz");
  sprintf(name, "%s.pdf", outtag.c_str());
  plots->Print(name);
  plots->Clear();

  plots->Clear();
  hzpt_zrap_r12_up->Draw("colz");
  sprintf(name, "%s.pdf", outtag.c_str());
  plots->Print(name);
  plots->Clear();



  // 2D zpt zrap el up
  TH2D* hzpt_zrap1_el_up = new TH2D("hzpt_zrap1_el_up", "hzpt_zrap1_el_up", NZPtBins,ZPtBins,NZRapBins,ZRapBins);
  hzpt_zrap1_el_up->Sumw2();
  tree1->Draw("llnunu_l1_rapidity:llnunu_l1_pt>>hzpt_zrap1_el_up", zjet_selec_el_up.c_str());
  hzpt_zrap1_el_up->Scale(1.0/hzpt_zrap1_el_up->Integral(),"width");
  TH2D* hzpt_zrap_r12_el_up = (TH2D*)hzpt_zrap1_el_up->Clone("hzpt_zrap_r12_el_up");
  hzpt_zrap_r12_el_up->Divide(hzpt_zrap2);
  hzpt_zrap_r12_el_up->Scale(Norm_El);

  plots->Clear();
  hzpt_zrap1_el_up->Draw("colz");
  sprintf(name, "%s.pdf", outtag.c_str());
  plots->Print(name);
  plots->Clear();

  plots->Clear();
  hzpt_zrap_r12_el_up->Draw("colz");
  sprintf(name, "%s.pdf", outtag.c_str());
  plots->Print(name);
  plots->Clear();


  // 2D zpt zrap mu
  TH2D* hzpt_zrap1_mu_up = new TH2D("hzpt_zrap1_mu_up", "hzpt_zrap1_mu_up", NZPtBins,ZPtBins,NZRapBins,ZRapBins);
  hzpt_zrap1_mu_up->Sumw2();
  tree1->Draw("llnunu_l1_rapidity:llnunu_l1_pt>>hzpt_zrap1_mu_up", zjet_selec_mu_up.c_str());
  hzpt_zrap1_mu_up->Scale(1.0/hzpt_zrap1_mu_up->Integral(),"width");
  TH2D* hzpt_zrap_r12_mu_up = (TH2D*)hzpt_zrap1_mu_up->Clone("hzpt_zrap_r12_mu_up");
  hzpt_zrap_r12_mu_up->Divide(hzpt_zrap2);
  hzpt_zrap_r12_mu_up->Scale(Norm_Mu);

  plots->Clear();
  hzpt_zrap1_mu_up->Draw("colz");
  sprintf(name, "%s.pdf", outtag.c_str());
  plots->Print(name);
  plots->Clear();

  plots->Clear();
  hzpt_zrap_r12_mu_up->Draw("colz");
  sprintf(name, "%s.pdf", outtag.c_str());
  plots->Print(name);
  plots->Clear();



  // 2D zpt zrap dn
  TH2D* hzpt_zrap1_dn = new TH2D("hzpt_zrap1_dn", "hzpt_zrap1_dn", NZPtBins,ZPtBins,NZRapBins,ZRapBins);

  hzpt_zrap1_dn->Sumw2();
  tree1->Draw("llnunu_l1_rapidity:llnunu_l1_pt>>hzpt_zrap1_dn", zjet_selec_dn.c_str());
  hzpt_zrap1_dn->Scale(1.0/hzpt_zrap1_dn->Integral(),"width");

  TH2D* hzpt_zrap_r12_dn = (TH2D*)hzpt_zrap1_dn->Clone("hzpt_zrap_r12_dn");
  hzpt_zrap_r12_dn->Divide(hzpt_zrap2);
  hzpt_zrap_r12_dn->Scale(Norm_All);


  plots->Clear();
  hzpt_zrap1_dn->Draw("colz");
  sprintf(name, "%s.pdf", outtag.c_str());
  plots->Print(name);
  plots->Clear();

  plots->Clear();
  hzpt_zrap_r12_dn->Draw("colz");
  sprintf(name, "%s.pdf", outtag.c_str());
  plots->Print(name);
  plots->Clear();



  // 2D zpt zrap el dn
  TH2D* hzpt_zrap1_el_dn = new TH2D("hzpt_zrap1_el_dn", "hzpt_zrap1_el_dn", NZPtBins,ZPtBins,NZRapBins,ZRapBins);
  hzpt_zrap1_el_dn->Sumw2();
  tree1->Draw("llnunu_l1_rapidity:llnunu_l1_pt>>hzpt_zrap1_el_dn", zjet_selec_el_dn.c_str());
  hzpt_zrap1_el_dn->Scale(1.0/hzpt_zrap1_el_dn->Integral(),"width");
  TH2D* hzpt_zrap_r12_el_dn = (TH2D*)hzpt_zrap1_el_dn->Clone("hzpt_zrap_r12_el_dn");
  hzpt_zrap_r12_el_dn->Divide(hzpt_zrap2);
  hzpt_zrap_r12_el_dn->Scale(Norm_El);

  plots->Clear();
  hzpt_zrap1_el_dn->Draw("colz");
  sprintf(name, "%s.pdf", outtag.c_str());
  plots->Print(name);
  plots->Clear();

  plots->Clear();
  hzpt_zrap_r12_el_dn->Draw("colz");
  sprintf(name, "%s.pdf", outtag.c_str());
  plots->Print(name);
  plots->Clear();


  // 2D zpt zrap mu dn
  TH2D* hzpt_zrap1_mu_dn = new TH2D("hzpt_zrap1_mu_dn", "hzpt_zrap1_mu_dn", NZPtBins,ZPtBins,NZRapBins,ZRapBins);
  hzpt_zrap1_mu_dn->Sumw2();
  tree1->Draw("llnunu_l1_rapidity:llnunu_l1_pt>>hzpt_zrap1_mu_dn", zjet_selec_mu_dn.c_str());
  hzpt_zrap1_mu_dn->Scale(1.0/hzpt_zrap1_mu_dn->Integral(),"width");
  TH2D* hzpt_zrap_r12_mu_dn = (TH2D*)hzpt_zrap1_mu_dn->Clone("hzpt_zrap_r12_mu_dn");
  hzpt_zrap_r12_mu_dn->Divide(hzpt_zrap2);
  hzpt_zrap_r12_mu_dn->Scale(Norm_Mu);

  plots->Clear();
  hzpt_zrap1_mu_dn->Draw("colz");
  sprintf(name, "%s.pdf", outtag.c_str());
  plots->Print(name);
  plots->Clear();

  plots->Clear();
  hzpt_zrap_r12_mu_dn->Draw("colz");
  sprintf(name, "%s.pdf", outtag.c_str());
  plots->Print(name);
  plots->Clear();



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

  hzptbb1_lowlpt->Write("h_zpt_lowlpt_1");
  hzptbbr12_lowlpt->Write("h_zpt_lowlpt_ratio");

  hzptbb1_lowlpt_el->Write("h_zpt_lowlpt_1_el");
  hzptbbr12_lowlpt_el->Write("h_zpt_lowlpt_ratio_el");

  hzptbb1_lowlpt_mu->Write("h_zpt_lowlpt_1_mu");
  hzptbbr12_lowlpt_mu->Write("h_zpt_lowlpt_ratio_mu");

  hzmass_zpt_zrap->Write("h_zmass_zpt_zrap");
  hzmass_zpt_zrap_el->Write("h_zmass_zpt_zrap_el");
  hzmass_zpt_zrap_mu->Write("h_zmass_zpt_zrap_mu");
  hzmass_zpt_zrap_lowlpt->Write("h_zmass_zpt_zrap_lowlpt");
  hzmass_zpt_zrap_lowlpt_el->Write("h_zmass_zpt_zrap_lowlpt_el");
  hzmass_zpt_zrap_lowlpt_mu->Write("h_zmass_zpt_zrap_lowlpt_mu");


  hzmass_zpt->Write("h_zmass_zpt");
  hzmass_zpt_el->Write("h_zmass_zpt_el");
  hzmass_zpt_mu->Write("h_zmass_zpt_mu");
  hzmass_zpt_lowlpt->Write("h_zmass_zpt_lowlpt");
  hzmass_zpt_lowlpt_el->Write("h_zmass_zpt_lowlpt_el");
  hzmass_zpt_lowlpt_mu->Write("h_zmass_zpt_lowlpt_mu");

  hzpt_zrap1->Write("h_zpt_zrap_1");
  hzpt_zrap2->Write("h_zpt_zrap_2");
  hzpt_zrap_r12->Write("h_zpt_zrap_ratio");

  hzpt_zrap1_el->Write("h_zpt_zrap_1_el");
  hzpt_zrap_r12_el->Write("h_zpt_zrap_ratio_el");

  hzpt_zrap1_mu->Write("h_zpt_zrap_1_mu");
  hzpt_zrap_r12_mu->Write("h_zpt_zrap_ratio_mu");

  hzpt_zrap1_up->Write("h_zpt_zrap_1_up");
  hzpt_zrap_r12_up->Write("h_zpt_zrap_ratio_up");
  hzpt_zrap1_el_up->Write("h_zpt_zrap_1_el_up");
  hzpt_zrap_r12_el_up->Write("h_zpt_zrap_ratio_el_up");
  hzpt_zrap1_mu_up->Write("h_zpt_zrap_1_mu_up");
  hzpt_zrap_r12_mu_up->Write("h_zpt_zrap_ratio_mu_up");

  hzpt_zrap1_dn->Write("h_zpt_zrap_1_dn");
  hzpt_zrap_r12_dn->Write("h_zpt_zrap_ratio_dn");
  hzpt_zrap1_el_dn->Write("h_zpt_zrap_1_el_dn");
  hzpt_zrap_r12_el_dn->Write("h_zpt_zrap_ratio_el_dn");
  hzpt_zrap1_mu_dn->Write("h_zpt_zrap_1_mu_dn");
  hzpt_zrap_r12_mu_dn->Write("h_zpt_zrap_ratio_mu_dn");


  hzpt_zrap1_lowlpt->Write("h_zpt_zrap_lowlpt_1");
  hzpt_zrap_lowlpt_r12->Write("h_zpt_zrap_lowlpt_ratio");

  hzpt_zrap1_lowlpt_el->Write("h_zpt_zrap_lowlpt_1_el");
  hzpt_zrap_lowlpt_r12_el->Write("h_zpt_zrap_lowlpt_ratio_el");

  hzpt_zrap1_lowlpt_mu->Write("h_zpt_zrap_lowlpt_1_mu");
  hzpt_zrap_lowlpt_r12_mu->Write("h_zpt_zrap_lowlpt_ratio_mu");

  fout->Close();

  sprintf(name, "%s.pdf]", outtag.c_str());
  plots->Print(name);


  return 0; 
}
