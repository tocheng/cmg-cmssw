{

  gROOT->ProcessLine(".x tdrstyle.C");
  TFile* file1 = TFile::Open("muon80x12p9.root");
  TH2D* h2d1 = (TH2D*)file1->Get("sf_HighPt_80X_pteta");
  h2d1->GetYaxis()->SetTitle("p_{T} (GeV)");
  h2d1->GetXaxis()->SetTitle("#eta");
  //h2d1->GetXaxis()->SetRangeUser(100,1000);
  h2d1->SetTitleSize(0.06,"XYZ");
  


  std::string  lumiTag = "CMS 13 TeV 2016 L=12.9 fb^{-1}";
  //if (doMC) lumiTag = "CMS 13 TeV Simulation for 2016 Data";

  TPaveText* lumipt = new TPaveText(0.2,0.9,0.8,0.98,"brNDC");
  lumipt->SetBorderSize(0);
  lumipt->SetTextAlign(12);
  lumipt->SetFillStyle(0);
  lumipt->SetTextFont(42);
  lumipt->SetTextSize(0.03);
  lumipt->AddText(0.15,0.3, lumiTag.c_str()); 

 
  TCanvas* plots = new TCanvas("plots", "plots");
  plots->SetLogy();
  h2d1->Draw("colz text45");
  lumipt->Draw();
  TPaletteAxis *palette = (TPaletteAxis*)h2d1->GetListOfFunctions()->FindObject("palette");
  palette->SetX1NDC(0.87);
  palette->SetX2NDC(0.9);
  palette->SetY1NDC(0.15);
  palette->SetY2NDC(0.9);
  plots->Modified();
  plots->Update();
  plots->SaveAs("sf_HighPt_80X_pteta.pdf");



  TH2D* h2d2 = (TH2D*)file1->Get("sf_trackHighPt_80X_pteta");
  h2d2->GetYaxis()->SetTitle("p_{T} (GeV)");
  h2d2->GetXaxis()->SetTitle("#eta");
  h2d2->SetTitleSize(0.06,"XYZ");


  plots->Clear();
  plots->SetLogy();
  //h2d2->GetXaxis()->SetRangeUser(90,120);
  h2d2->Draw("colz text45");
  lumipt->Draw();

  palette = (TPaletteAxis*)h2d2->GetListOfFunctions()->FindObject("palette");
  palette->SetX1NDC(0.87);
  palette->SetX2NDC(0.9);
  palette->SetY1NDC(0.15);
  palette->SetY2NDC(0.9);
  plots->Modified();
  plots->Update();

  plots->SaveAs("sf_trackHighPt_80X_pteta.pdf");



 
  TH2D* h2d3 = (TH2D*)file1->Get("sf_trackerIso_80X_pteta");
  h2d3->GetYaxis()->SetTitle("p_{T} (GeV)");
  h2d3->GetXaxis()->SetTitle("#eta");
  h2d3->SetTitleSize(0.06,"XYZ");


  plots->Clear();
  plots->SetLogy();
  //h2d3->GetXaxis()->SetRangeUser(90,120);
  h2d3->Draw("colz text45");
  lumipt->Draw();

  palette = (TPaletteAxis*)h2d3->GetListOfFunctions()->FindObject("palette");
  palette->SetX1NDC(0.87);
  palette->SetX2NDC(0.9);
  palette->SetY1NDC(0.15);
  palette->SetY2NDC(0.9);
  plots->Modified();
  plots->Update();

  plots->SaveAs("sf_trackerIso_80X_pteta.pdf"); 

  
  // 
  TFile* file2 = TFile::Open("muontrackingsf.root");

  TH1F* h1d1 = (TH1F*)file2->Get("hist_ratio_eta");

  h1d1->GetXaxis()->SetTitle("#eta");
  h1d1->GetYaxis()->SetTitle("tracking scale factor");
  h1d1->GetYaxis()->SetTitleOffset(1.5);
  h1d1->SetMarkerStyle(20);

  plots->Clear();
  plots->SetLogy(0);
  h1d1->Draw();
  lumipt->Draw();
  plots->SaveAs("sf_muon_trackingeff_80x_eta.pdf");
  
  
}
