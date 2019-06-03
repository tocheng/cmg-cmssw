{


  TFile* fin = new TFile("DataB2H36p1fbinv_NoRecoilZJets_tight_puWeightmoriondMC_metfilter_unblind_all_log_1pb.root");
  TFile* fout = new TFile("rhoratio_b2h36p1fbinv.root", "recreate");

  gROOT->ProcessLine(".x tdrstyle.h");

  TH1D* hdt = (TH1D*)fin->Get("DataB2H36p1fbinv_NoRecoilZJets_tight_puWeightmoriondMC_metfilter_unblind_all_log_1pb_rho_data_SingleEMU_Run2016B2H_ReReco_36p1fbinv");
  TH1D* hmc = (TH1D*)((THStack*)fin->Get("DataB2H36p1fbinv_NoRecoilZJets_tight_puWeightmoriondMC_metfilter_unblind_all_log_1pb_rho_stack"))->GetHistogram();
  
  TH1D* hdt_norm = (TH1D*)hdt->Clone("h_rho_dt_norm");
  TH1D* hmc_norm = (TH1D*)hmc->Clone("h_rho_mc_norm");

  hdt_norm->Scale(1./hdt_norm->Integral());
  hmc_norm->Scale(1./hmc_norm->Integral());

  hratio_dt_mc = (TH1D*)hdt_norm->Clone("h_rho_ratio_dt_mc");
  hratio_dt_mc->Divide(hmc_norm);

  hratio_dt_mc->SetTitle("Rho reweighting: Normalized data/MC, 2016 data 36.1 fb^{-1}.");
  hratio_dt_mc->GetYaxis()->SetTitle("Data/MC");

  char name[10000];
  //sprintf(name, "[0]+[1]*x+[2]*x*x+[3]*x*x*x+[4]*x*x*x*x");
  //sprintf(name, "[0]+[1]*x+[2]*x*x+[3]*x*x*x");
  //sprintf(name, "[0]+[1]*x+[2]*x*x");
  sprintf(name, "([0]-[1]*TMath::Erf((x-[2])/[3])+[4]*TMath::Erf((x-[5])/[6]))");

  TCanvas* plots = new TCanvas("plots", "plots");
//(0.121+0.160*rho+-8.276e-03*rho*rho+1.244e-04*rho*rho*rho)
  TF1* fratio_dt_mc = new TF1("f_rho_ratio_dt_mc", name, 0,50);
  //fratio_dt_mc->SetParameter(0,0.121);
  //fratio_dt_mc->SetParameter(1,8.276e-3);
  //fratio_dt_mc->SetParameter(2,1.244e-4);
  //fratio_dt_mc->FixParameter(3,8e-5);
  //fratio_dt_mc->SetParameter(4,13);
  //fratio_dt_mc->SetParameter(5,20);
  //fratio_dt_mc->SetParLimits(0,0,2.0);
  //fratio_dt_mc->SetParLimits(1,4,10);
  //fratio_dt_mc->SetParLimits(2,5,20);
  //fratio_dt_mc->SetParLimits(3,0,2.0);
  //fratio_dt_mc->SetParLimits(4,6,40);
  //fratio_dt_mc->SetParLimits(5,10,50);

  hratio_dt_mc->Fit(fratio_dt_mc);
  hratio_dt_mc->Fit(fratio_dt_mc);
  hratio_dt_mc->Fit(fratio_dt_mc);
  hratio_dt_mc->Fit(fratio_dt_mc);
  fratio_dt_mc->Print("v");

  hratio_dt_mc->GetYaxis()->SetRangeUser(0,5);

  TPaveText* lumipt;
  lumipt = new TPaveText(0.2,0.66,0.8,0.8,"brNDC");
  lumipt->SetBorderSize(0);
  lumipt->SetTextAlign(12);
  lumipt->SetFillStyle(0);
  lumipt->SetTextFont(42);
  lumipt->SetTextSize(0.03);
  sprintf(name, 
        //"f(#rho) = %.2f + %.4f #rho + %.4f #rho^{2} + + %.5f #rho^{3}",
        "f(x)=%.2f-%.2f*Erf((x-%.2f)/%.2f)+%.2f*Erf((x-%.2f)/%.2f)",
        fratio_dt_mc->GetParameter(0), 
        fratio_dt_mc->GetParameter(1), 
        fratio_dt_mc->GetParameter(2), 
        fratio_dt_mc->GetParameter(3),
        fratio_dt_mc->GetParameter(4),
        fratio_dt_mc->GetParameter(5),
        fratio_dt_mc->GetParameter(6)
        );
  lumipt->AddText(0.0,0.6, name);
  lumipt->Draw();

  plots->Print("rho_rewegiht_b2h36p1fbinv.pdf");
  plots->SaveAs("rho_rewegiht_b2g36p1fbinv.C");


  sprintf(name, 
          //"(%.3f+%.3f*rho+%.3e*rho*rho+%.3e*rho*rho*rho)",
          "%.2f-%.2f*TMath::Erf((x-%.2f)/%.2f)+%.2f*TMath::Erf((x-%.2f)/%.2f)",
          fratio_dt_mc->GetParameter(0),
          fratio_dt_mc->GetParameter(1),
          fratio_dt_mc->GetParameter(2), 
          fratio_dt_mc->GetParameter(3),
          fratio_dt_mc->GetParameter(4),
          fratio_dt_mc->GetParameter(5),
          fratio_dt_mc->GetParameter(6)
         );

  std::cout << "#############################################" << std::endl;          
  std::cout << "func = " << name << std::endl;
  std::cout << "#############################################" << std::endl;          
 
  fout->cd();
  hdt->Write();
  hmc->Write();
  hdt_norm->Write();
  hmc_norm->Write();
  hratio_dt_mc->Write();
  fratio_dt_mc->Write();


//  fout->Close(); 


}
