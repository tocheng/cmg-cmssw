{

  std::string outtag="gjets_filter";

  gROOT->ProcessLine(".x tdrstyle.C");

  TFile* fineb = TFile::Open("getFileterString_eb.root");
  TFile* fineep = TFile::Open("getFileterString_eep.root");
  TFile* fineem = TFile::Open("getFileterString_eem.root");

  TH2F* heb = (TH2F*)fineb->Get("hist_map_after"); 
  TH2F* heep = (TH2F*)fineep->Get("hist_map_after"); 
  TH2F* heem = (TH2F*)fineem->Get("hist_map_after"); 



  char name[1000];

  TCanvas* plots = new TCanvas("plots", "plots");

  sprintf(name, "%s.pdf[", outtag.c_str());
  plots->Print(name);


  heb->GetXaxis()->SetTitle("i #eta");
  heb->GetYaxis()->SetTitle("i #phi");

  heep->GetXaxis()->SetTitle("i X");
  heep->GetYaxis()->SetTitle("i Y");
  heem->GetXaxis()->SetTitle("i X");
  heem->GetYaxis()->SetTitle("i Y");

  heb->GetXaxis()->SetTitleSize(0.08);
  heb->GetYaxis()->SetTitleSize(0.08);
  heb->GetXaxis()->SetTitleOffset(1.05);
  heb->GetYaxis()->SetTitleOffset(1.05);

  heep->GetXaxis()->SetTitleSize(0.08);
  heep->GetYaxis()->SetTitleSize(0.08);
  heep->GetXaxis()->SetTitleOffset(1.05);
  heep->GetYaxis()->SetTitleOffset(1.05);
  heem->GetXaxis()->SetTitleSize(0.08);
  heem->GetYaxis()->SetTitleSize(0.08);
  heem->GetXaxis()->SetTitleOffset(1.05);
  heem->GetYaxis()->SetTitleOffset(1.05);
 
  plots->Clear();
  heb->Draw("col");
  sprintf(name, "%s.pdf", outtag.c_str());
  plots->Print(name);
  plots->Clear();


  plots->Clear();
  heep->Draw("col");
  sprintf(name, "%s.pdf", outtag.c_str());
  plots->Print(name);
  plots->Clear();

  plots->Clear();
  heem->Draw("col");
  sprintf(name, "%s.pdf", outtag.c_str());
  plots->Print(name);
  plots->Clear();


  sprintf(name, "%s.pdf]", outtag.c_str());
  plots->Print(name);


}


