{

  gROOT->ProcessLine(".x tdrstyle.C");
  TFile* file1 = TFile::Open("trigereff12p9_20160902.root");
  TH2D* h2d1 = (TH2D*)file1->Get("ell1pteta");
  h2d1->GetXaxis()->SetTitle("p_{T} (GeV)");
  h2d1->GetYaxis()->SetTitle("|#eta|");
  h2d1->GetXaxis()->SetRangeUser(100,1000);
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
  //plots->SetLogx();
  h2d1->GetXaxis()->SetRangeUser(120,1000);
  h2d1->Draw("colz text45");
  lumipt->Draw();
  plots->SaveAs("trigeff12p9_el_pt120to1k.pdf");

  plots->Clear();
  h2d1->GetXaxis()->SetRangeUser(90,120);
  h2d1->Draw("colz text45");
  lumipt->Draw();
  plots->SaveAs("trigeff12p9_el_pt90to120.pdf");

  

  


}
