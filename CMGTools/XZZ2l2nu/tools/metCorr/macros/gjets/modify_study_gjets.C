{
  gROOT->ProcessLine(".! cp study_gjets_data_fullv2.root study_gjets_data_fullv2_modify.root");

  TFile* file = TFile::Open("study_gjets_data_fullv2_modify.root", "update");

  TH1D* h_zpt_ratio = (TH1D*)file->Get("h_zpt_ratio");
  TH1D* h_zpt_ratio_el = (TH1D*)file->Get("h_zpt_ratio_el");
  TH1D* h_zpt_ratio_mu = (TH1D*)file->Get("h_zpt_ratio_mu");
  TH1D* h_zpt_ratio_up = (TH1D*)file->Get("h_zpt_ratio_up");
  TH1D* h_zpt_ratio_el_up = (TH1D*)file->Get("h_zpt_ratio_el_up");
  TH1D* h_zpt_ratio_mu_up = (TH1D*)file->Get("h_zpt_ratio_mu_up");
  TH1D* h_zpt_ratio_dn = (TH1D*)file->Get("h_zpt_ratio_dn");
  TH1D* h_zpt_ratio_el_dn = (TH1D*)file->Get("h_zpt_ratio_el_dn");
  TH1D* h_zpt_ratio_mu_dn = (TH1D*)file->Get("h_zpt_ratio_mu_dn");

  TH1D* h_zpt_ratio_el_old = (TH1D*)h_zpt_ratio_el->Clone("h_zpt_ratio_el_old");
  TH1D* h_zpt_ratio_mu_old = (TH1D*)h_zpt_ratio_mu->Clone("h_zpt_ratio_mu_old");

  TProfile* pr_zpt_1 = (TProfile*)file->Get("pr_zpt_1");
  TProfile* pr_zpt_1_mu = (TProfile*)file->Get("pr_zpt_1_mu");
  TProfile* pr_zpt_1_el = (TProfile*)file->Get("pr_zpt_1_el");
  TProfile* pr_zpt_1_up = (TProfile*)file->Get("pr_zpt_1");
  TProfile* pr_zpt_1_mu_up = (TProfile*)file->Get("pr_zpt_1_mu_up");
  TProfile* pr_zpt_1_el_up = (TProfile*)file->Get("pr_zpt_1_el_up");
  TProfile* pr_zpt_1_dn = (TProfile*)file->Get("pr_zpt_1");
  TProfile* pr_zpt_1_mu_dn = (TProfile*)file->Get("pr_zpt_1_mu_dn");
  TProfile* pr_zpt_1_el_dn = (TProfile*)file->Get("pr_zpt_1_el_dn");

  TGraphErrors* gr_zpt_ratio = (TGraphErrors*)file->Get("gr_zpt_ratio");
  TGraphErrors* gr_zpt_ratio_mu = (TGraphErrors*)file->Get("gr_zpt_ratio_mu");
  TGraphErrors* gr_zpt_ratio_el = (TGraphErrors*)file->Get("gr_zpt_ratio_el");
  TGraphErrors* gr_zpt_ratio_up = (TGraphErrors*)file->Get("gr_zpt_ratio_up");
  TGraphErrors* gr_zpt_ratio_mu_up = (TGraphErrors*)file->Get("gr_zpt_ratio_mu_up");
  TGraphErrors* gr_zpt_ratio_el_up = (TGraphErrors*)file->Get("gr_zpt_ratio_el_up");
  TGraphErrors* gr_zpt_ratio_dn = (TGraphErrors*)file->Get("gr_zpt_ratio_dn");
  TGraphErrors* gr_zpt_ratio_mu_dn = (TGraphErrors*)file->Get("gr_zpt_ratio_mu_dn");
  TGraphErrors* gr_zpt_ratio_el_dn = (TGraphErrors*)file->Get("gr_zpt_ratio_el_dn");
 
  h_zpt_ratio_el_old->SetLineColor(4);
  h_zpt_ratio_el_old->SetMarkerColor(4);
  h_zpt_ratio_mu_old->SetLineColor(4);
  h_zpt_ratio_mu_old->SetMarkerColor(4);

  // bin, scale
  int b;
  double s;

  // h_zpt_ratio_el

  b=49; s=0.0;
  h_zpt_ratio_el->SetBinContent(b,h_zpt_ratio_el->GetBinContent(b)*s);
  h_zpt_ratio_el_up->SetBinContent(b,h_zpt_ratio_el_up->GetBinContent(b)*s);
  h_zpt_ratio_el_dn->SetBinContent(b,h_zpt_ratio_el_dn->GetBinContent(b)*s);
  b=50; s=0.0;
  h_zpt_ratio_el->SetBinContent(b,h_zpt_ratio_el->GetBinContent(b)*s);
  h_zpt_ratio_el_up->SetBinContent(b,h_zpt_ratio_el_up->GetBinContent(b)*s);
  h_zpt_ratio_el_dn->SetBinContent(b,h_zpt_ratio_el_dn->GetBinContent(b)*s);
  b=51; s=0.0;
  h_zpt_ratio_el->SetBinContent(b,h_zpt_ratio_el->GetBinContent(b)*s);
  h_zpt_ratio_el_up->SetBinContent(b,h_zpt_ratio_el_up->GetBinContent(b)*s);
  h_zpt_ratio_el_dn->SetBinContent(b,h_zpt_ratio_el_dn->GetBinContent(b)*s);
  b=52; s=1.01;
  h_zpt_ratio_el->SetBinContent(b,h_zpt_ratio_el->GetBinContent(b)*s);
  h_zpt_ratio_el_up->SetBinContent(b,h_zpt_ratio_el_up->GetBinContent(b)*s);
  h_zpt_ratio_el_dn->SetBinContent(b,h_zpt_ratio_el_dn->GetBinContent(b)*s);
  b=53; s=1.01;
  h_zpt_ratio_el->SetBinContent(b,h_zpt_ratio_el->GetBinContent(b)*s);
  h_zpt_ratio_el_up->SetBinContent(b,h_zpt_ratio_el_up->GetBinContent(b)*s);
  h_zpt_ratio_el_dn->SetBinContent(b,h_zpt_ratio_el_dn->GetBinContent(b)*s);
  b=54; s=0.93;
  h_zpt_ratio_el->SetBinContent(b,h_zpt_ratio_el->GetBinContent(b)*s);
  h_zpt_ratio_el_up->SetBinContent(b,h_zpt_ratio_el_up->GetBinContent(b)*s);
  h_zpt_ratio_el_dn->SetBinContent(b,h_zpt_ratio_el_dn->GetBinContent(b)*s);
  b=55; s=1.06;
  h_zpt_ratio_el->SetBinContent(b,h_zpt_ratio_el->GetBinContent(b)*s);
  h_zpt_ratio_el_up->SetBinContent(b,h_zpt_ratio_el_up->GetBinContent(b)*s);
  h_zpt_ratio_el_dn->SetBinContent(b,h_zpt_ratio_el_dn->GetBinContent(b)*s);
  b=56; s=1.03;
  h_zpt_ratio_el->SetBinContent(b,h_zpt_ratio_el->GetBinContent(b)*s);
  h_zpt_ratio_el_up->SetBinContent(b,h_zpt_ratio_el_up->GetBinContent(b)*s);
  h_zpt_ratio_el_dn->SetBinContent(b,h_zpt_ratio_el_dn->GetBinContent(b)*s);
  b=57; s=1.00;
  h_zpt_ratio_el->SetBinContent(b,h_zpt_ratio_el->GetBinContent(b)*s);
  h_zpt_ratio_el_up->SetBinContent(b,h_zpt_ratio_el_up->GetBinContent(b)*s);
  h_zpt_ratio_el_dn->SetBinContent(b,h_zpt_ratio_el_dn->GetBinContent(b)*s);
  b=58; s=1.07;
  h_zpt_ratio_el->SetBinContent(b,h_zpt_ratio_el->GetBinContent(b)*s);
  h_zpt_ratio_el_up->SetBinContent(b,h_zpt_ratio_el_up->GetBinContent(b)*s);
  h_zpt_ratio_el_dn->SetBinContent(b,h_zpt_ratio_el_dn->GetBinContent(b)*s);
  b=59; s=0.96;
  h_zpt_ratio_el->SetBinContent(b,h_zpt_ratio_el->GetBinContent(b)*s);
  h_zpt_ratio_el_up->SetBinContent(b,h_zpt_ratio_el_up->GetBinContent(b)*s);
  h_zpt_ratio_el_dn->SetBinContent(b,h_zpt_ratio_el_dn->GetBinContent(b)*s);
  b=60; s=1.00;
  h_zpt_ratio_el->SetBinContent(b,h_zpt_ratio_el->GetBinContent(b)*s);
  h_zpt_ratio_el_up->SetBinContent(b,h_zpt_ratio_el_up->GetBinContent(b)*s);
  h_zpt_ratio_el_dn->SetBinContent(b,h_zpt_ratio_el_dn->GetBinContent(b)*s);
  b=61; s=1.1;
  h_zpt_ratio_el->SetBinContent(b,h_zpt_ratio_el->GetBinContent(b)*s);
  h_zpt_ratio_el_up->SetBinContent(b,h_zpt_ratio_el_up->GetBinContent(b)*s);
  h_zpt_ratio_el_dn->SetBinContent(b,h_zpt_ratio_el_dn->GetBinContent(b)*s);

  h_zpt_ratio_el->Draw();
  

  

  // h_zpt_ratio_mu
  b=52; s=0.98; 
  h_zpt_ratio_mu->SetBinContent(b,h_zpt_ratio_mu->GetBinContent(b)*s);
  h_zpt_ratio_mu_up->SetBinContent(b,h_zpt_ratio_mu_up->GetBinContent(b)*s);
  h_zpt_ratio_mu_dn->SetBinContent(b,h_zpt_ratio_mu_dn->GetBinContent(b)*s);
  b=54; s=1.03; 
  h_zpt_ratio_mu->SetBinContent(b,h_zpt_ratio_mu->GetBinContent(b)*s);
  h_zpt_ratio_mu_up->SetBinContent(b,h_zpt_ratio_mu_up->GetBinContent(b)*s);
  h_zpt_ratio_mu_dn->SetBinContent(b,h_zpt_ratio_mu_dn->GetBinContent(b)*s);
  b=57; s=1.0;
  h_zpt_ratio_mu->SetBinContent(b,h_zpt_ratio_mu->GetBinContent(b)*s);
  h_zpt_ratio_mu_up->SetBinContent(b,h_zpt_ratio_mu_up->GetBinContent(b)*s);
  h_zpt_ratio_mu_dn->SetBinContent(b,h_zpt_ratio_mu_dn->GetBinContent(b)*s);
  b=58; s=1.08;
  h_zpt_ratio_mu->SetBinContent(b,h_zpt_ratio_mu->GetBinContent(b)*s);
  h_zpt_ratio_mu_up->SetBinContent(b,h_zpt_ratio_mu_up->GetBinContent(b)*s);
  h_zpt_ratio_mu_dn->SetBinContent(b,h_zpt_ratio_mu_dn->GetBinContent(b)*s);
  b=59; s=1.03;
  h_zpt_ratio_mu->SetBinContent(b,h_zpt_ratio_mu->GetBinContent(b)*s);
  h_zpt_ratio_mu_up->SetBinContent(b,h_zpt_ratio_mu_up->GetBinContent(b)*s);
  h_zpt_ratio_mu_dn->SetBinContent(b,h_zpt_ratio_mu_dn->GetBinContent(b)*s);
  b=60; s=1.05;
  h_zpt_ratio_mu->SetBinContent(b,h_zpt_ratio_mu->GetBinContent(b)*s);
  h_zpt_ratio_mu_up->SetBinContent(b,h_zpt_ratio_mu_up->GetBinContent(b)*s);
  h_zpt_ratio_mu_dn->SetBinContent(b,h_zpt_ratio_mu_dn->GetBinContent(b)*s);
  b=61; s=1.25;
  h_zpt_ratio_mu->SetBinContent(b,h_zpt_ratio_mu->GetBinContent(b)*s);
  h_zpt_ratio_mu_up->SetBinContent(b,h_zpt_ratio_mu_up->GetBinContent(b)*s);
  h_zpt_ratio_mu_dn->SetBinContent(b,h_zpt_ratio_mu_dn->GetBinContent(b)*s);


  h_zpt_ratio_mu->Draw();
  h_zpt_ratio_mu_old->Draw("same");


  for (int i=0; i<pr_zpt_1->GetNbinsX(); i++){
    gr_zpt_ratio->SetPoint(i, pr_zpt_1->GetBinContent(i+1), h_zpt_ratio->GetBinContent(i+1));
    gr_zpt_ratio_el->SetPoint(i, pr_zpt_1_el->GetBinContent(i+1), h_zpt_ratio_el->GetBinContent(i+1));
    gr_zpt_ratio_mu->SetPoint(i, pr_zpt_1_mu->GetBinContent(i+1), h_zpt_ratio_mu->GetBinContent(i+1));
    gr_zpt_ratio_up->SetPoint(i, pr_zpt_1_up->GetBinContent(i+1), h_zpt_ratio_up->GetBinContent(i+1));
    gr_zpt_ratio_dn->SetPoint(i, pr_zpt_1_dn->GetBinContent(i+1), h_zpt_ratio_dn->GetBinContent(i+1));
    gr_zpt_ratio_el_up->SetPoint(i, pr_zpt_1_el_up->GetBinContent(i+1), h_zpt_ratio_el->GetBinContent(i+1)+sqrt(pow(h_zpt_ratio_el_up->GetBinContent(i+1)-h_zpt_ratio_el->GetBinContent(i+1),2)+pow(h_zpt_ratio_el->GetBinError(i+1),2)));
    gr_zpt_ratio_el_dn->SetPoint(i, pr_zpt_1_el_dn->GetBinContent(i+1), h_zpt_ratio_el->GetBinContent(i+1)-sqrt(pow(h_zpt_ratio_el_dn->GetBinContent(i+1)-h_zpt_ratio_el->GetBinContent(i+1),2)+pow(h_zpt_ratio_el->GetBinError(i+1),2)));
    gr_zpt_ratio_mu_up->SetPoint(i, pr_zpt_1_mu_up->GetBinContent(i+1), h_zpt_ratio_mu->GetBinContent(i+1)+sqrt(pow(h_zpt_ratio_mu_up->GetBinContent(i+1)-h_zpt_ratio_mu->GetBinContent(i+1),2)+pow(h_zpt_ratio_mu->GetBinError(i+1),2)));
    gr_zpt_ratio_mu_dn->SetPoint(i, pr_zpt_1_mu_dn->GetBinContent(i+1), h_zpt_ratio_mu->GetBinContent(i+1)-sqrt(pow(h_zpt_ratio_mu_dn->GetBinContent(i+1)-h_zpt_ratio_mu->GetBinContent(i+1),2)+pow(h_zpt_ratio_mu->GetBinError(i+1),2)));

    gr_zpt_ratio->SetPointError(i, pr_zpt_1->GetBinError(i+1), h_zpt_ratio->GetBinError(i+1));
    gr_zpt_ratio_el->SetPointError(i, pr_zpt_1_el->GetBinError(i+1), h_zpt_ratio_el->GetBinError(i+1));
    gr_zpt_ratio_mu->SetPointError(i, pr_zpt_1_mu->GetBinError(i+1), h_zpt_ratio_mu->GetBinError(i+1));
    gr_zpt_ratio_up->SetPointError(i, pr_zpt_1_up->GetBinError(i+1), sqrt(pow(h_zpt_ratio_up->GetBinContent(i+1)-h_zpt_ratio->GetBinContent(i+1),2)+pow(h_zpt_ratio->GetBinError(i+1),2)));
    gr_zpt_ratio_dn->SetPointError(i, pr_zpt_1_dn->GetBinError(i+1), sqrt(pow(h_zpt_ratio_dn->GetBinContent(i+1)-h_zpt_ratio->GetBinContent(i+1),2)+pow(h_zpt_ratio->GetBinError(i+1),2)));
    gr_zpt_ratio_el_up->SetPointError(i, pr_zpt_1_el_up->GetBinError(i+1), sqrt(pow(h_zpt_ratio_el_up->GetBinContent(i+1)-h_zpt_ratio_el->GetBinContent(i+1),2)+pow(h_zpt_ratio_el->GetBinError(i+1),2)));
    gr_zpt_ratio_el_dn->SetPointError(i, pr_zpt_1_el_dn->GetBinError(i+1), sqrt(pow(h_zpt_ratio_el_dn->GetBinContent(i+1)-h_zpt_ratio_el->GetBinContent(i+1),2)+pow(h_zpt_ratio_el->GetBinError(i+1),2)));
    gr_zpt_ratio_mu_up->SetPointError(i, pr_zpt_1_mu_up->GetBinError(i+1), sqrt(pow(h_zpt_ratio_mu_up->GetBinContent(i+1)-h_zpt_ratio_mu->GetBinContent(i+1),2)+pow(h_zpt_ratio_mu->GetBinError(i+1),2)));
    gr_zpt_ratio_mu_dn->SetPointError(i, pr_zpt_1_mu_dn->GetBinError(i+1), sqrt(pow(h_zpt_ratio_mu_dn->GetBinContent(i+1)-h_zpt_ratio_mu->GetBinContent(i+1),2)+pow(h_zpt_ratio_mu->GetBinError(i+1),2)));
  }

  gr_zpt_ratio_el->Draw("apl");
  gr_zpt_ratio_el_up->Draw("lx same");
  gr_zpt_ratio_el_dn->Draw("lx same");
  h_zpt_ratio_el_old->Draw("same");

  gr_zpt_ratio_mu->Draw("apl");
  gr_zpt_ratio_mu_up->Draw("lx same");
  gr_zpt_ratio_mu_dn->Draw("lx same");
  h_zpt_ratio_mu_old->Draw("same");


  file->cd();
  h_zpt_ratio_el->Write("h_zpt_ratio_el");
  h_zpt_ratio_el_up->Write("h_zpt_ratio_el_up");
  h_zpt_ratio_el_dn->Write("h_zpt_ratio_el_dn");
  h_zpt_ratio_mu->Write("h_zpt_ratio_mu");
  h_zpt_ratio_mu_up->Write("h_zpt_ratio_mu_up");
  h_zpt_ratio_mu_dn->Write("h_zpt_ratio_mu_dn");

  gr_zpt_ratio->Write("gr_zpt_ratio");
  gr_zpt_ratio_mu->Write("gr_zpt_ratio_mu");
  gr_zpt_ratio_el->Write("gr_zpt_ratio_el");
  gr_zpt_ratio_up->Write("gr_zpt_ratio_up");
  gr_zpt_ratio_dn->Write("gr_zpt_ratio_dn");
  gr_zpt_ratio_mu_up->Write("gr_zpt_ratio_mu_up");
  gr_zpt_ratio_mu_dn->Write("gr_zpt_ratio_mu_dn");
  gr_zpt_ratio_el_up->Write("gr_zpt_ratio_el_up");
  gr_zpt_ratio_el_dn->Write("gr_zpt_ratio_el_dn");
  

  file->Close();  

}
