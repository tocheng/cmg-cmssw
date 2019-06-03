{

  gROOT->ProcessLine(".x tdrstyle.C");
  TFile* file1 = TFile::Open("muon_idiso_summer16.root");
  TH2D* h_hpt_dt_1 = (TH2D*)file1->Get("h_mu_hpt_data_1");
  TH2D* h_hpt_dt_2 = (TH2D*)file1->Get("h_mu_hpt_data_2");
  TH2D* h_tkhpt_dt_1 = (TH2D*)file1->Get("h_mu_tkhpt_data_1");
  TH2D* h_tkhpt_dt_2 = (TH2D*)file1->Get("h_mu_tkhpt_data_2");

  TH2D* h_hpt_mc_1 = (TH2D*)file1->Get("h_mu_hpt_mc_1");
  TH2D* h_hpt_mc_2 = (TH2D*)file1->Get("h_mu_hpt_mc_2");
  TH2D* h_tkhpt_mc_1 = (TH2D*)file1->Get("h_mu_tkhpt_mc_1");
  TH2D* h_tkhpt_mc_2 = (TH2D*)file1->Get("h_mu_tkhpt_mc_2");
  
  TH2D* h_iso_sf_1 = (TH2D*)file1->Get("h_mu_iso_sf_1");
  TH2D* h_iso_sf_2 = (TH2D*)file1->Get("h_mu_iso_sf_2");

  h_hpt_dt_1->GetYaxis()->SetTitle("p_{T} (GeV)");
  h_hpt_dt_2->GetYaxis()->SetTitle("p_{T} (GeV)");
  h_tkhpt_dt_1->GetYaxis()->SetTitle("p_{T} (GeV)");
  h_tkhpt_dt_2->GetYaxis()->SetTitle("p_{T} (GeV)");
  h_hpt_mc_1->GetYaxis()->SetTitle("p_{T} (GeV)");
  h_hpt_mc_2->GetYaxis()->SetTitle("p_{T} (GeV)");
  h_tkhpt_mc_1->GetYaxis()->SetTitle("p_{T} (GeV)");
  h_tkhpt_mc_2->GetYaxis()->SetTitle("p_{T} (GeV)");
  h_iso_sf_1->GetYaxis()->SetTitle("p_{T} (GeV)");
  h_iso_sf_2->GetYaxis()->SetTitle("p_{T} (GeV)");

  h_hpt_dt_1->GetXaxis()->SetTitle("|#eta|");
  h_hpt_dt_2->GetXaxis()->SetTitle("|#eta|");
  h_tkhpt_dt_1->GetXaxis()->SetTitle("|#eta|");
  h_tkhpt_dt_2->GetXaxis()->SetTitle("|#eta|");
  h_hpt_mc_1->GetXaxis()->SetTitle("|#eta|");
  h_hpt_mc_2->GetXaxis()->SetTitle("|#eta|");
  h_tkhpt_mc_1->GetXaxis()->SetTitle("|#eta|");
  h_tkhpt_mc_2->GetXaxis()->SetTitle("|#eta|");
  h_iso_sf_1->GetXaxis()->SetTitle("|#eta|");
  h_iso_sf_2->GetXaxis()->SetTitle("|#eta|");

  h_hpt_dt_1->SetTitleSize(0.06,"XYZ");
  h_hpt_dt_2->SetTitleSize(0.06,"XYZ");
  h_tkhpt_dt_1->SetTitleSize(0.06,"XYZ");
  h_tkhpt_dt_2->SetTitleSize(0.06,"XYZ");
  h_hpt_mc_1->SetTitleSize(0.06,"XYZ");
  h_hpt_mc_2->SetTitleSize(0.06,"XYZ");
  h_tkhpt_mc_1->SetTitleSize(0.06,"XYZ");
  h_tkhpt_mc_2->SetTitleSize(0.06,"XYZ");
  h_iso_sf_1->SetTitleSize(0.06,"XYZ");
  h_iso_sf_1->SetTitleSize(0.06,"XYZ");


  
  std::string  lumiTag_dt = "CMS 13 TeV 2016 L=36.81 fb^{-1}";
  std::string  lumiTag_mc = "CMS 13 TeV Simulation for 2016 Data";

  TPaveText* lumipt_dt = new TPaveText(0.2,0.9,0.8,0.98,"brNDC");
  lumipt_dt->SetBorderSize(0);
  lumipt_dt->SetTextAlign(12);
  lumipt_dt->SetFillStyle(0);
  lumipt_dt->SetTextFont(42);
  lumipt_dt->SetTextSize(0.03);
  lumipt_dt->AddText(0.15,0.3, lumiTag_dt.c_str()); 

  TPaveText* lumipt_mc = new TPaveText(0.2,0.9,0.8,0.98,"brNDC");
  lumipt_mc->SetBorderSize(0);
  lumipt_mc->SetTextAlign(12);
  lumipt_mc->SetFillStyle(0);
  lumipt_mc->SetTextFont(42);
  lumipt_mc->SetTextSize(0.03);
  lumipt_mc->AddText(0.15,0.3, lumiTag_mc.c_str());

 
  TCanvas* plots = new TCanvas("plots", "plots");
  plots->Print("muon_idiso_summer16.pdf[");



  plots->SetLogy();

  plots->Clear();
  h_hpt_dt_1->Draw("colz text45");
  lumipt_dt->Draw();
  plots->Print("muon_idiso_summer16.pdf");


  plots->Clear();
  h_hpt_dt_2->Draw("colz text45");
  lumipt_dt->Draw();
  plots->Print("muon_idiso_summer16.pdf");
  plots->Clear();
  h_tkhpt_dt_1->Draw("colz text45");
  lumipt_dt->Draw();
  plots->Print("muon_idiso_summer16.pdf");
  plots->Clear();
  h_tkhpt_dt_2->Draw("colz text45");
  lumipt_dt->Draw();
  plots->Print("muon_idiso_summer16.pdf");
  plots->Clear();
  h_hpt_mc_1->Draw("colz text45");
  lumipt_dt->Draw();
  plots->Print("muon_idiso_summer16.pdf");
  plots->Clear();
  h_hpt_mc_2->Draw("colz text45");
  lumipt_dt->Draw();
  plots->Print("muon_idiso_summer16.pdf");
  plots->Clear();
  h_tkhpt_mc_1->Draw("colz text45");
  lumipt_dt->Draw();
  plots->Print("muon_idiso_summer16.pdf");
  plots->Clear();
  h_tkhpt_mc_2->Draw("colz text45");
  lumipt_dt->Draw();
  plots->Print("muon_idiso_summer16.pdf");
  plots->Clear();
  h_iso_sf_1->Draw("colz text45");
  lumipt_dt->Draw();
  plots->Print("muon_idiso_summer16.pdf");
  plots->Clear();
  h_iso_sf_2->Draw("colz text45");
  lumipt_dt->Draw();
  plots->Print("muon_idiso_summer16.pdf");



  plots->Print("muon_idiso_summer16.pdf]");

}
