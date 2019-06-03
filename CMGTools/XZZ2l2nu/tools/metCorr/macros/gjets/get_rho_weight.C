{

  gROOT->ProcessLine(".x tdrstyle.C");

  std::string outtag="get_rho_weight_fullv2";
  TFile* file1 = TFile::Open("/home/heli/XZZ/80X_20170202_light_Skim/SingleEMU_Run2016Full_ReReco_v2_DtReCalib.root");
  TFile* file2 = TFile::Open("/home/heli/XZZ/80X_20170202_GJets_light_Skim/SinglePhoton_Run2016Full_ReReco_v2_NoRecoil.root");

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
 
  char name[1000];
  sprintf(name, "%s.root", outtag.c_str());
  TFile* fout = TFile::Open(name, "recreate");
  TTree* tree1 = (TTree*)file1->Get("tree");
  TTree* tree2 = (TTree*)file2->Get("tree");



  TCanvas* plots = new TCanvas("plots", "plots");

  sprintf(name, "%s.pdf[", outtag.c_str());
  plots->Print(name);

  
  Double_t ZPtBins[] = {0,30,36,50,75,90,120,165,3000};
  Int_t NZPtBins = sizeof(ZPtBins)/sizeof(ZPtBins[0]) - 1;
  Double_t RhoBins[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100};

  Int_t NRhoBins = sizeof(RhoBins)/sizeof(RhoBins[0]) - 1; 

  TH2D* h_rho_zpt_1 = new TH2D("h_rho_zpt_1", "h_rho_zpt_1", NZPtBins, ZPtBins, NRhoBins, RhoBins);
  TH2D* h_rho_zpt_2 = new TH2D("h_rho_zpt_2", "h_rho_zpt_2", NZPtBins, ZPtBins, NRhoBins, RhoBins);

  h_rho_zpt_1->Sumw2();
  h_rho_zpt_2->Sumw2();

  tree1->Draw("rho:llnunu_l1_pt>>h_rho_zpt_1");
  //tree1->Draw("rho:llnunu_l1_pt>>h_rho_zpt_1", "abs(llnunu_l1_l1_pdgId)==11");
  //tree1->Draw("rho:llnunu_l1_pt>>h_rho_zpt_1", "abs(llnunu_l1_l1_pdgId)==13");
  //tree2->Draw("rho:llnunu_l1_pt>>h_rho_zpt_2");
  //tree2->Draw("rho:gjet_l1_pt>>h_rho_zpt_2");
  tree2->Draw("rho:llnunu_l1_pt>>h_rho_zpt_2", "(1)*(GJetsPreScaleWeight)");

  TProfile* pr_zpt_1 = (TProfile*)h_rho_zpt_1->ProfileX("pr_zpt_1");
  TProfile* pr_zpt_2 = (TProfile*)h_rho_zpt_2->ProfileX("pr_zpt_2");

  TH1D* h_zpt_1 = (TH1D*)h_rho_zpt_1->ProjectionX("h_zpt_1");
  TH1D* h_zpt_2 = (TH1D*)h_rho_zpt_2->ProjectionX("h_zpt_2");

  h_zpt_1->SetTitle("h_zpt_1");
  h_zpt_2->SetTitle("h_zpt_2");

  TH2D* h_rho_zpt_1_zptnorm = (TH2D*)h_rho_zpt_1->Clone("h_rho_zpt_1_zptnorm");
  TH2D* h_rho_zpt_2_zptnorm = (TH2D*)h_rho_zpt_2->Clone("h_rho_zpt_2_zptnorm");

  h_rho_zpt_1_zptnorm->SetTitle("h_rho_zpt_1_zptnorm");
  h_rho_zpt_2_zptnorm->SetTitle("h_rho_zpt_2_zptnorm");

  for (int izpt=0; izpt<NZPtBins; izpt++){
    double nzpt1 = h_zpt_1->GetBinContent(izpt+1);
    double nzpt2 = h_zpt_2->GetBinContent(izpt+1);
 
    for (int irho=0; irho<NRhoBins; irho++){
      h_rho_zpt_1_zptnorm->SetBinContent(izpt+1, irho+1, h_rho_zpt_1_zptnorm->GetBinContent(izpt+1, irho+1)/nzpt1);
      h_rho_zpt_1_zptnorm->SetBinError(izpt+1, irho+1, h_rho_zpt_1_zptnorm->GetBinError(izpt+1, irho+1)/nzpt1);
      h_rho_zpt_2_zptnorm->SetBinContent(izpt+1, irho+1, h_rho_zpt_2_zptnorm->GetBinContent(izpt+1, irho+1)/nzpt2);
      h_rho_zpt_2_zptnorm->SetBinError(izpt+1, irho+1, h_rho_zpt_2_zptnorm->GetBinError(izpt+1, irho+1)/nzpt2);
    }

  }

  h_rho_zpt_weight = (TH2D*)h_rho_zpt_1_zptnorm->Clone("h_rho_zpt_weight");
  h_rho_zpt_weight->Divide(h_rho_zpt_2_zptnorm);
  h_rho_zpt_weight->SetTitle("h_rho_zpt_weight");


  // plotting

  TH1D* h_zpt_sb_nops = new TH1D("h_zpt_sb_nops", "h_zpt_sb_nops", 500, 0, 1000);
  TH1D* h_zpt_sb_wps  = new TH1D("h_zpt_sb_wps",  "h_zpt_sb_wps",  500, 0, 1000);

  h_zpt_sb_nops->Sumw2();
  h_zpt_sb_wps->Sumw2();

  tree2->Draw("llnunu_l1_pt>>h_zpt_sb_nops");
  tree2->Draw("llnunu_l1_pt>>h_zpt_sb_wps", "(1)*(GJetsPreScaleWeight)");

  h_zpt_sb_nops->SetLineColor(2);
  h_zpt_sb_nops->SetMarkerColor(2);
  h_zpt_sb_wps->SetLineColor(4);
  h_zpt_sb_wps->SetMarkerColor(4);
 
  h_zpt_sb_nops->GetXaxis()->SetTitle("photon p_{T} (GeV)"); 
  h_zpt_sb_wps->GetXaxis()->SetTitle("photon p_{T} (GeV)"); 
  
  TLegend* lg_zpt_ps = new TLegend(0.6,0.7, 0.8,0.8);
  lg_zpt_ps->AddEntry(h_zpt_sb_nops, "no HLT prescale");
  lg_zpt_ps->AddEntry(h_zpt_sb_wps, "with HLT prescale");

  plots->Clear();
  h_zpt_sb_wps->Draw();
  h_zpt_sb_nops->Draw("same");
  lg_zpt_ps->Draw();
  lumipt->Draw();
  plots->SetLogy(1);
  sprintf(name, "%s.pdf", outtag.c_str());
  plots->Print(name);

  plots->SetLogy(0);
  plots->Print(name);

  plots->SetLogy(0);
  plots->Clear();


  std::vector<std::string> hlt_lab={"HLT 22", "HLT 30", "HLT 36", "HLT 50", "HLT 75", "HLT 90", "HLT 120", "HLT 165"};

  for (int i=0; i<h_rho_zpt_1_zptnorm->GetNbinsX(); i++){
    sprintf(name, "tmp_h1_%i", i);
    TH1D* h1 = (TH1D*)h_rho_zpt_1_zptnorm->ProjectionY(name, i+1,i+1);
    sprintf(name, "tmp_h2_%i", i);
    TH1D* h2 = (TH1D*)h_rho_zpt_2_zptnorm->ProjectionY(name, i+1,i+1);
    h1->SetLineColor(2);
    h1->SetMarkerColor(2);
    h2->SetLineColor(4);
    h2->SetMarkerColor(4);
    h1->GetXaxis()->SetTitle("#rho");
    h2->GetXaxis()->SetTitle("#rho");
    TLegend* lg1 = new TLegend(0.6,0.7, 0.8,0.8);
    sprintf(name, "tmp_lg1_%i", i);
    lg1->AddEntry(h1, "di-lepton data", "apl");
    lg1->AddEntry(h2, "photon data" , "apl");
    lg1->AddEntry(h2, hlt_lab.at(i).c_str() , "");

    plots->Clear();
    plots->SetLogy(1);
    h2->Draw();
    h1->Draw("same");
    lumipt->Draw();
    sprintf(name, "%s.pdf", outtag.c_str());
    plots->Print(name);
    plots->SetLogy(0);
    plots->Print(name);
    plots->Clear();

  }

  sprintf(name, "%s.pdf]", outtag.c_str());
  plots->Print(name);
  // save

  fout->cd();

  h_rho_zpt_1->Write();
  h_rho_zpt_2->Write();

  pr_zpt_1->Write();
  pr_zpt_2->Write();

  h_zpt_1->Write();
  h_zpt_2->Write();
  
  h_rho_zpt_1_zptnorm->Write();
  h_rho_zpt_2_zptnorm->Write();

  h_rho_zpt_weight->Write();


}
