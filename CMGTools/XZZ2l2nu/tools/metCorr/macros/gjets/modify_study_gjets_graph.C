{
  gROOT->ProcessLine(".! cp study_gjets_data_36p46_norm_npucut.root study_gjets_data_36p46_norm_npucut_modify.root");

  TFile* file = TFile::Open("study_gjets_data_36p46_norm_npucut_modify.root", "update");

  TGraphErrors* hzpt = (TGraphErrors*)file->Get("gr_zpt_ratio");
  TGraphErrors* hzpt_el = (TGraphErrors*)file->Get("gr_zpt_ratio_el");
  TGraphErrors* hzpt_mu = (TGraphErrors*)file->Get("gr_zpt_ratio_mu");
  TGraphErrors* hzpt_up = (TGraphErrors*)file->Get("gr_zpt_ratio_up");
  TGraphErrors* hzpt_el_up = (TGraphErrors*)file->Get("gr_zpt_ratio_el_up");
  TGraphErrors* hzpt_mu_up = (TGraphErrors*)file->Get("gr_zpt_ratio_mu_up");
  TGraphErrors* hzpt_dn = (TGraphErrors*)file->Get("gr_zpt_ratio_dn");
  TGraphErrors* hzpt_el_dn = (TGraphErrors*)file->Get("gr_zpt_ratio_el_dn");
  TGraphErrors* hzpt_mu_dn = (TGraphErrors*)file->Get("gr_zpt_ratio_mu_dn");


  TGraphErrors* hzpt_lowlpt = (TGraphErrors*)file->Get("gr_zpt_lowlpt_ratio");
  TGraphErrors* hzpt_lowlpt_el = (TGraphErrors*)file->Get("gr_zpt_lowlpt_ratio_el");
  TGraphErrors* hzpt_lowlpt_mu = (TGraphErrors*)file->Get("gr_zpt_lowlpt_ratio_mu");


  int i;
  double x;
  double y;
  double s;

  // gr_zpt_ratio_el
  i=47; x=100; y=0;
  hzpt_el->SetPoint(i, x, y);
  hzpt_el_up->SetPoint(i, x, y);
  hzpt_el_dn->SetPoint(i, x, y);
  
  i=60; s=1.1;
  hzpt_el->GetPoint(i,x,y);
  hzpt_el->SetPoint(i,x,s*y);
  hzpt_el_up->GetPoint(i,x,y);
  hzpt_el_up->SetPoint(i,x,s*y);
  hzpt_el_dn->GetPoint(i,x,y);
  hzpt_el_dn->SetPoint(i,x,s*y);

  i=59; s=0.92;
  hzpt_el->GetPoint(i,x,y);
  hzpt_el->SetPoint(i,x,s*y);
  hzpt_el_up->GetPoint(i,x,y);
  hzpt_el_up->SetPoint(i,x,s*y);
  hzpt_el_dn->GetPoint(i,x,y);
  hzpt_el_dn->SetPoint(i,x,s*y);

  i=58; s=1.03;
  hzpt_el->GetPoint(i,x,y);
  hzpt_el->SetPoint(i,x,s*y);
  hzpt_el_up->GetPoint(i,x,y);
  hzpt_el_up->SetPoint(i,x,s*y);
  hzpt_el_dn->GetPoint(i,x,y);
  hzpt_el_dn->SetPoint(i,x,s*y);

  i=57; s=0.98;
  hzpt_el->GetPoint(i,x,y);
  hzpt_el->SetPoint(i,x,s*y);
  hzpt_el_up->GetPoint(i,x,y);
  hzpt_el_up->SetPoint(i,x,s*y);
  hzpt_el_dn->GetPoint(i,x,y);
  hzpt_el_dn->SetPoint(i,x,s*y);

  hzpt_el->Draw("apl");
  hzpt_el->Print();


  // gr_zpt_ratio_mu
  i=0; x=20; y=0;
  hzpt_mu->SetPoint(i, x, y);
  hzpt_mu_up->SetPoint(i, x, y);
  hzpt_mu_dn->SetPoint(i, x, y);

  i=52; s=0.98;
  hzpt_mu->GetPoint(i,x,y);
  hzpt_mu->SetPoint(i,x,s*y);
  hzpt_mu_up->GetPoint(i,x,y);
  hzpt_mu_up->SetPoint(i,x,s*y);
  hzpt_mu_dn->GetPoint(i,x,y);
  hzpt_mu_dn->SetPoint(i,x,s*y);

  i=54; s=1.03;
  hzpt_mu->GetPoint(i,x,y);
  hzpt_mu->SetPoint(i,x,s*y);
  hzpt_mu_up->GetPoint(i,x,y);
  hzpt_mu_up->SetPoint(i,x,s*y);
  hzpt_mu_dn->GetPoint(i,x,y);
  hzpt_mu_dn->SetPoint(i,x,s*y);

  i=56; s=1.04;
  hzpt_mu->GetPoint(i,x,y);
  hzpt_mu->SetPoint(i,x,s*y);
  hzpt_mu_up->GetPoint(i,x,y);
  hzpt_mu_up->SetPoint(i,x,s*y);
  hzpt_mu_dn->GetPoint(i,x,y);
  hzpt_mu_dn->SetPoint(i,x,s*y);

  i=57; s=1.02;
  hzpt_mu->GetPoint(i,x,y);
  hzpt_mu->SetPoint(i,x,s*y);
  hzpt_mu_up->GetPoint(i,x,y);
  hzpt_mu_up->SetPoint(i,x,s*y);
  hzpt_mu_dn->GetPoint(i,x,y);
  hzpt_mu_dn->SetPoint(i,x,s*y);

  i=58; s=1.036;
  hzpt_mu->GetPoint(i,x,y);
  hzpt_mu->SetPoint(i,x,s*y);
  hzpt_mu_up->GetPoint(i,x,y);
  hzpt_mu_up->SetPoint(i,x,s*y);
  hzpt_mu_dn->GetPoint(i,x,y);
  hzpt_mu_dn->SetPoint(i,x,s*y);

  i=59; s=1.115;
  hzpt_mu->GetPoint(i,x,y);
  hzpt_mu->SetPoint(i,x,s*y);
  hzpt_mu_up->GetPoint(i,x,y);
  hzpt_mu_up->SetPoint(i,x,s*y);
  hzpt_mu_dn->GetPoint(i,x,y);
  hzpt_mu_dn->SetPoint(i,x,s*y);

  i=60; s=1.35;
  hzpt_mu->GetPoint(i,x,y);
  hzpt_mu->SetPoint(i,x,s*y);
  hzpt_mu_up->GetPoint(i,x,y);
  hzpt_mu_up->SetPoint(i,x,s*y);
  hzpt_mu_dn->GetPoint(i,x,y);
  hzpt_mu_dn->SetPoint(i,x,s*y);



  hzpt_mu->Draw("apl");
  //hzpt_mu->Print();


  //gr_zpt_lowlpt_ratio, _el, _mu
  i=0; x=20; y=0;
  hzpt_lowlpt->SetPoint(i,x,y);
  hzpt_lowlpt_el->SetPoint(i,x,y);
  hzpt_lowlpt_mu->SetPoint(i,x,y);

  // gr_zpt_lowlpt_ratio_el
  i=59; s=0.9;
  hzpt_lowlpt_el->GetPoint(i,x,y);
  hzpt_lowlpt_el->SetPoint(i,x,s*y);

  i=58; s=1.02;
  hzpt_lowlpt_el->GetPoint(i,x,y);
  hzpt_lowlpt_el->SetPoint(i,x,s*y);

  i=57; s=0.98;
  hzpt_lowlpt_el->GetPoint(i,x,y);
  hzpt_lowlpt_el->SetPoint(i,x,s*y);



//  hzpt_lowlpt_el->Draw("apl");


  // gr_zpt_lowlpt_ratio_mu
  i=52; s=0.98;
  hzpt_lowlpt_mu->GetPoint(i,x,y);
  hzpt_lowlpt_mu->SetPoint(i,x,s*y);

  i=54; s=1.03;
  hzpt_lowlpt_mu->GetPoint(i,x,y);
  hzpt_lowlpt_mu->SetPoint(i,x,s*y);

  i=56; s=1.04;
  hzpt_lowlpt_mu->GetPoint(i,x,y);
  hzpt_lowlpt_mu->SetPoint(i,x,s*y);

  i=59; s=1.09;
  hzpt_lowlpt_mu->GetPoint(i,x,y);
  hzpt_lowlpt_mu->SetPoint(i,x,s*y);

  i=60; s=1.36;
  hzpt_lowlpt_mu->GetPoint(i,x,y);
  hzpt_lowlpt_mu->SetPoint(i,x,s*y);


//  hzpt_lowlpt_mu->Draw("apl");


  file->cd();
  hzpt_el->Write("gr_zpt_ratio_el");
  hzpt_el_up->Write("gr_zpt_ratio_el_up");
  hzpt_el_dn->Write("gr_zpt_ratio_el_dn");
  hzpt_mu->Write("gr_zpt_ratio_mu");
  hzpt_mu_up->Write("gr_zpt_ratio_mu_up");
  hzpt_mu_dn->Write("gr_zpt_ratio_mu_dn");

  
  hzpt_lowlpt->Write("gr_zpt_lowlpt_ratio_el");
  hzpt_lowlpt_el->Write("gr_zpt_lowlpt_ratio_el");
  hzpt_lowlpt_mu->Write("gr_zpt_lowlpt_ratio_mu");

  
//  file->Close();  

}
