{
  TFile* fin = TFile::Open("zz.root");
  TFile* fout = TFile::Open("zzqcd.root", "recreate");
  TH1D* hnnlo = (TH1D*)fin->Get("plot_m_ZZ_5_0__NNLO_QCD_dat");
  TH1D* hnlo = (TH1D*)fin->Get("plot_m_ZZ_5_0__NLO_QCD_dat");

  gROOT->ProcessLine(".x tdrstyle.C");

  TCanvas* plots = new TCanvas("plots", "plots");
  
  TH1D* hnnlo_ct = (TH1D*)hnnlo->Clone("hnnlo_ct");
  TH1D* hnnlo_up = (TH1D*)hnnlo->Clone("hnnlo_up");
  TH1D* hnnlo_dn = (TH1D*)hnnlo->Clone("hnnlo_dn");


  TH1D* hnlo_ct = (TH1D*)hnlo->Clone("hnlo_ct");
  TH1D* hnlo_up = (TH1D*)hnlo->Clone("hnlo_up");
  TH1D* hnlo_dn = (TH1D*)hnlo->Clone("hnlo_dn");

  for (int i=1; i<=100; i++) {
    hnnlo_up->SetBinContent(i, hnnlo_up->GetBinContent(i)+hnnlo_up->GetBinError(i)/2);
    hnnlo_dn->SetBinContent(i, hnnlo_dn->GetBinContent(i)-hnnlo_dn->GetBinError(i)/2);
    hnlo_up->SetBinContent(i, hnlo_up->GetBinContent(i)+hnlo_up->GetBinError(i)/2);
    hnlo_dn->SetBinContent(i, hnlo_dn->GetBinContent(i)-hnlo_dn->GetBinError(i)/2);
  }

  double rebin=5;
  hnnlo_ct->Rebin(int(rebin));
  hnnlo_up->Rebin(int(rebin));
  hnnlo_dn->Rebin(int(rebin));
  hnlo_ct->Rebin(int(rebin));
  hnlo_up->Rebin(int(rebin));
  hnlo_dn->Rebin(int(rebin));

  hnnlo_ct->Scale(1/rebin);
  hnnlo_up->Scale(1/rebin);
  hnnlo_dn->Scale(1/rebin);

  hnlo_ct->Scale(1/rebin);
  hnlo_up->Scale(1/rebin);
  hnlo_dn->Scale(1/rebin);


  TH1D* hr_ct = (TH1D*)hnnlo_ct->Clone("hr_ct");
  TH1D* hr_up = (TH1D*)hnnlo_up->Clone("hr_up");
  TH1D* hr_dn = (TH1D*)hnnlo_dn->Clone("hr_dn");

  hr_ct->Divide(hnlo_ct);
  hr_up->Divide(hnlo_up);
  hr_dn->Divide(hnlo_dn);

  hr_ct->SetLineColor(2);
  hr_up->SetLineColor(4);
  hr_dn->SetLineColor(4);

  //hr_ct->SetFillColor(2);

  hr_ct->GetXaxis()->SetTitle("Gen. M(ZZ) (GeV)");
  hr_ct->GetYaxis()->SetTitle("NNLO QCD / NLO QCD ");
  hr_ct->GetYaxis()->SetTitleOffset(1.5);

  hr_ct->GetYaxis()->SetRangeUser(0.7, 1.5);
 
  hr_ct->Draw("c hist");
  hr_up->Draw("same c hist");
  hr_dn->Draw("same c hist");


  plots->SaveAs("h_nnlo_to_nlo_vs_mzz.pdf");

  hr_ct->Write("h_nnlo_to_nlo_vs_mzz_ct");
  hr_up->Write("h_nnlo_to_nlo_vs_mzz_up");
  hr_dn->Write("h_nnlo_to_nlo_vs_mzz_dn");

    

}
