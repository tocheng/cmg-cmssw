{

  gROOT->ProcessLine(".x tdrstyle.C");
  TFile* file1 = TFile::Open("muon_trg_summer16.root");
  TH2D* h_dt_1 = (TH2D*)file1->Get("h_eff_trg_mu50tkmu50_dt_1");
  TH2D* h_dt_2 = (TH2D*)file1->Get("h_eff_trg_mu50tkmu50_dt_2");
  TH2D* h_dt_3 = (TH2D*)file1->Get("h_eff_trg_mu50tkmu50_dt_3");
  TH2D* h_dt_4 = (TH2D*)file1->Get("h_eff_trg_mu50tkmu50_dt_4");

  TH2D* h_mc_1 = (TH2D*)file1->Get("h_eff_trg_mu50tkmu50_mc_1");
  TH2D* h_mc_2 = (TH2D*)file1->Get("h_eff_trg_mu50tkmu50_mc_2");
  TH2D* h_mc_3 = (TH2D*)file1->Get("h_eff_trg_mu50tkmu50_mc_3");
  TH2D* h_mc_4 = (TH2D*)file1->Get("h_eff_trg_mu50tkmu50_mc_4");

  TH2D* h_sf_1 = (TH2D*)file1->Get("h_eff_trg_mu50tkmu50_sf_1");
  TH2D* h_sf_2 = (TH2D*)file1->Get("h_eff_trg_mu50tkmu50_sf_2");
  TH2D* h_sf_3 = (TH2D*)file1->Get("h_eff_trg_mu50tkmu50_sf_3");
  TH2D* h_sf_4 = (TH2D*)file1->Get("h_eff_trg_mu50tkmu50_sf_4");

  h_dt_1->GetYaxis()->SetTitle("p_{T} (GeV)");
  h_dt_2->GetYaxis()->SetTitle("p_{T} (GeV)");
  h_dt_3->GetYaxis()->SetTitle("p_{T} (GeV)");
  h_dt_4->GetYaxis()->SetTitle("p_{T} (GeV)");
  h_mc_1->GetYaxis()->SetTitle("p_{T} (GeV)");
  h_mc_2->GetYaxis()->SetTitle("p_{T} (GeV)");
  h_mc_3->GetYaxis()->SetTitle("p_{T} (GeV)");
  h_mc_4->GetYaxis()->SetTitle("p_{T} (GeV)");
  h_sf_1->GetYaxis()->SetTitle("p_{T} (GeV)");
  h_sf_2->GetYaxis()->SetTitle("p_{T} (GeV)");
  h_sf_3->GetYaxis()->SetTitle("p_{T} (GeV)");
  h_sf_4->GetYaxis()->SetTitle("p_{T} (GeV)");

  h_dt_1->GetXaxis()->SetTitle("|#eta|");
  h_dt_2->GetXaxis()->SetTitle("|#eta|");
  h_dt_3->GetXaxis()->SetTitle("|#eta|");
  h_dt_4->GetXaxis()->SetTitle("|#eta|");
  h_mc_1->GetXaxis()->SetTitle("|#eta|");
  h_mc_2->GetXaxis()->SetTitle("|#eta|");
  h_mc_3->GetXaxis()->SetTitle("|#eta|");
  h_mc_4->GetXaxis()->SetTitle("|#eta|");
  h_sf_1->GetXaxis()->SetTitle("|#eta|");
  h_sf_2->GetXaxis()->SetTitle("|#eta|");
  h_sf_3->GetXaxis()->SetTitle("|#eta|");
  h_sf_4->GetXaxis()->SetTitle("|#eta|");

  h_dt_1->SetTitleSize(0.06,"XYZ");
  h_dt_2->SetTitleSize(0.06,"XYZ");
  h_dt_3->SetTitleSize(0.06,"XYZ");
  h_dt_4->SetTitleSize(0.06,"XYZ");
  h_mc_1->SetTitleSize(0.06,"XYZ");
  h_mc_2->SetTitleSize(0.06,"XYZ");
  h_mc_3->SetTitleSize(0.06,"XYZ");
  h_mc_4->SetTitleSize(0.06,"XYZ");
  h_sf_1->SetTitleSize(0.06,"XYZ");
  h_sf_2->SetTitleSize(0.06,"XYZ");
  h_sf_3->SetTitleSize(0.06,"XYZ");
  h_sf_4->SetTitleSize(0.06,"XYZ");


  
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
  plots->Print("muon_trg_summer16.pdf[");



  plots->SetLogy();

  plots->Clear();
  h_dt_1->Draw("colz text45");
  lumipt_dt->Draw();
  plots->Print("muon_trg_summer16.pdf");

  plots->Clear();
  h_dt_2->Draw("colz text45");
  lumipt_dt->Draw();
  plots->Print("muon_trg_summer16.pdf");

  plots->Clear();
  h_dt_3->Draw("colz text45");
  lumipt_dt->Draw();
  plots->Print("muon_trg_summer16.pdf");

  plots->Clear();
  h_dt_4->Draw("colz text45");
  lumipt_dt->Draw();
  plots->Print("muon_trg_summer16.pdf");

  plots->Clear();
  h_mc_1->Draw("colz text45");
  lumipt_mc->Draw();
  plots->Print("muon_trg_summer16.pdf");
  plots->Clear();
  h_mc_2->Draw("colz text45");
  lumipt_mc->Draw();
  plots->Print("muon_trg_summer16.pdf");
  plots->Clear();
  h_mc_3->Draw("colz text45");
  lumipt_mc->Draw();
  plots->Print("muon_trg_summer16.pdf");
  plots->Clear();
  h_mc_4->Draw("colz text45");
  lumipt_mc->Draw();
  plots->Print("muon_trg_summer16.pdf");

  plots->Clear();
  h_sf_1->Draw("colz text45");
  lumipt_dt->Draw();
  plots->Print("muon_trg_summer16.pdf");
  plots->Clear();
  h_sf_2->Draw("colz text45");
  lumipt_dt->Draw();
  plots->Print("muon_trg_summer16.pdf");
  plots->Clear();
  h_sf_3->Draw("colz text45");
  lumipt_dt->Draw();
  plots->Print("muon_trg_summer16.pdf");
  plots->Clear();
  h_sf_4->Draw("colz text45");
  lumipt_dt->Draw();
  plots->Print("muon_trg_summer16.pdf");

  plots->Print("muon_trg_summer16.pdf]");

}
