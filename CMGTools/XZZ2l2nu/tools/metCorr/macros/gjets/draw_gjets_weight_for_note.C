{
  TFile* file = new TFile("study_gjets_data_36p46_norm_npucut_modify.root");

  gROOT->ProcessLine(".x tdrstyle.C");

  TGraphErrors* hzpt_el = (TGraphErrors*)file->Get("gr_zpt_ratio_el");
  TGraphErrors* hzpt_mu = (TGraphErrors*)file->Get("gr_zpt_ratio_mu");
  TGraphErrors* hzpt_el_up = (TGraphErrors*)file->Get("gr_zpt_ratio_el_up");
  TGraphErrors* hzpt_mu_up = (TGraphErrors*)file->Get("gr_zpt_ratio_mu_up");
  TGraphErrors* hzpt_el_dn = (TGraphErrors*)file->Get("gr_zpt_ratio_el_dn");
  TGraphErrors* hzpt_mu_dn = (TGraphErrors*)file->Get("gr_zpt_ratio_mu_dn");



  hzpt_el->GetXaxis()->SetTitle("Z or #gamma p_{T} (GeV)");
  hzpt_el->GetYaxis()->SetTitle("Z/#gamma");

  hzpt_mu->GetXaxis()->SetTitle("Z or #gamma p_{T} (GeV)");
  hzpt_mu->GetYaxis()->SetTitle("Z/#gamma");

  
  for (int i=0; i<hzpt_el->GetN(); i++){
    double x,y, ex, ey;
    double xu, yu, xd, yd;
    hzpt_el->GetPoint(i, x, y);
    ex = hzpt_el->GetErrorX(i);
    ey = hzpt_el->GetErrorY(i);
    hzpt_el_up->GetPoint(i, xu, yu);
    hzpt_el_dn->GetPoint(i, xd, yd);
    
    ey = sqrt(ey*ey+(yu-yd)*(yu-yd));
    hzpt_el->SetPointError(i, ex, ey);
  }

  for (int i=0; i<hzpt_mu->GetN(); i++){
    double x,y, ex, ey;
    double xu, yu, xd, yd;
    hzpt_mu->GetPoint(i, x, y);
    ex = hzpt_mu->GetErrorX(i);
    ey = hzpt_mu->GetErrorY(i); 
    hzpt_mu_up->GetPoint(i, xu, yu);
    hzpt_mu_dn->GetPoint(i, xd, yd);
    
    ey = sqrt(ey*ey+(yu-yd)*(yu-yd));
    hzpt_mu->SetPointError(i, ex, ey);
  }

  hzpt_el->GetXaxis()->SetTitle("Z or #gamma p_{T} (GeV)");
  hzpt_el->GetYaxis()->SetTitle("Z/#gamma");

  hzpt_mu->GetXaxis()->SetTitle("Z or #gamma p_{T} (GeV)");
  hzpt_mu->GetYaxis()->SetTitle("Z/#gamma");


  hzpt_el->SetMarkerStyle(20);
  hzpt_mu->SetMarkerStyle(20);

  hzpt_el->SetFillColor(6);
  hzpt_el->SetFillStyle(3005);

  hzpt_mu->SetFillColor(6);
  hzpt_mu->SetFillStyle(3005);


  TCanvas* plots = new TCanvas("plots", "plots");
  hzpt_el->Draw("ap a3");
  hzpt_el->Draw("pl same"); 
//  hzpt_el_up->Draw("l same");
//  hzpt_el_dn->Draw("l same");
  plots->SaveAs("study_gjets_data_36p46_norm_npucut_modify_el.pdf");

  hzpt_mu->Draw("ap a3"); 
  hzpt_mu->Draw("pl same"); 
//  hzpt_mu_up->Draw("l same");
//  hzpt_mu_dn->Draw("l same");
  plots->SaveAs("study_gjets_data_36p46_norm_npucut_modify_mu.pdf");

  

}
