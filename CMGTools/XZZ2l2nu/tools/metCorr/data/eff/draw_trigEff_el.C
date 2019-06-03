{

  gROOT->ProcessLine(".x tdrstyle.C");
  TFile* file1 = TFile::Open("hlt115electron_2016fulleff_absetapt.root");
  TH2D* h2d1 = (TH2D*)file1->Get("efficiency_dt");
  TH2D* h2d2 = (TH2D*)file1->Get("efficiency_mc");
  TH2D* h2d3 = (TH2D*)file1->Get("scalefactor");
  h2d1->GetYaxis()->SetTitle("p_{T} (GeV)");
  h2d1->GetXaxis()->SetTitle("|#eta|");
  h2d1->SetTitleSize(0.06,"XYZ");
  h2d2->GetYaxis()->SetTitle("p_{T} (GeV)");
  h2d2->GetXaxis()->SetTitle("|#eta|");
  h2d2->SetTitleSize(0.06,"XYZ");
  h2d3->GetYaxis()->SetTitle("p_{T} (GeV)");
  h2d3->GetXaxis()->SetTitle("|#eta|");
  h2d3->SetTitleSize(0.06,"XYZ");


  
  std::string  lumiTag_dt = "CMS 13 TeV 2016 L=36.814 fb^{-1}";
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
  plots->Print("hlt115electron_2016fulleff_absetapt.pdf[");



  plots->Clear();
  h2d1->GetYaxis()->SetRangeUser(90,150);
  h2d1->Draw("colz text45");
  lumipt_dt->Draw();
  plots->Print("hlt115electron_2016fulleff_absetapt.pdf");

  plots->Clear();
  h2d1->GetYaxis()->SetRangeUser(150,1000);
  h2d1->Draw("colz text45");
  lumipt_dt->Draw();
  plots->Print("hlt115electron_2016fulleff_absetapt.pdf");


  plots->Clear();
  h2d2->GetYaxis()->SetRangeUser(90,150);
  h2d2->Draw("colz text45");
  lumipt_mc->Draw();
  plots->Print("hlt115electron_2016fulleff_absetapt.pdf");

  plots->Clear();
  h2d2->GetYaxis()->SetRangeUser(150,1000);
  h2d2->Draw("colz text45");
  lumipt_mc->Draw();
  plots->Print("hlt115electron_2016fulleff_absetapt.pdf");  

  

  plots->Clear();
  h2d3->GetYaxis()->SetRangeUser(90,150);
  h2d3->Draw("colz text45");
  lumipt_dt->Draw();
  plots->Print("hlt115electron_2016fulleff_absetapt.pdf");

  plots->Clear();
  h2d3->GetYaxis()->SetRangeUser(150,1000);
  h2d3->Draw("colz text45");
  lumipt_dt->Draw();
  plots->Print("hlt115electron_2016fulleff_absetapt.pdf");


  plots->Print("hlt115electron_2016fulleff_absetapt.pdf]");


}
