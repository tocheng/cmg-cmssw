{
  TFile* fout = new TFile("dyjets_zpt_weight_lo_nlo_genAccV2.root");

  gROOT->ProcessLine(".x tdrstyle.C");

  TGraphErrors* gr = (TGraphErrors*)fout->Get("gdyzpt_dtmc_ratio");
  TF1* fc = (TF1*)fout->Get("fcdyzpt_dtmc_ratio");

  gr->GetXaxis()->SetTitle("unfolded Z p_{T} (GeV)");
  gr->GetYaxis()->SetTitle("Data/aMC@NLO");


  TGraphErrors* grer = (TGraphErrors*)gr->Clone("gr_er");
  
  for (int i=0; i<grer->GetN(); i++){
    double x,y;
    grer->GetPoint(i, x, y);
    y=fc->Eval(x);
    grer->SetPoint(i, x, y);
  }

  grer->GetXaxis()->SetTitle("unfolded Z p_{T} (GeV)");
  grer->GetYaxis()->SetTitle("Data/aMC@NLO");
  grer->SetFillColor(6);
  grer->SetFillStyle(3005);

  TCanvas* plots = new TCanvas("plots", "plots");
  grer->Draw("a a3");
  gr->Draw("p same");
  fc->Draw("same");
  plots->SaveAs("dyjets_zpt_data_vs_nlo_fit.pdf");

  

}
