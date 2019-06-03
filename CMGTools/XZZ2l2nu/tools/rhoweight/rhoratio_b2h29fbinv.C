{


  TFile* fin = new TFile("Test_DataB2H29fbinv_ICHEPcfg_tight_puWeight68075_metfilter_unblind_all_1pb.root");
  TFile* fout = new TFile("rhoratio_b2h29fbinv.root", "recreate");

  gROOT->ProcessLine(".x tdrstyle.h");

  TH1D* hdt = (TH1D*)fin->Get("Test_DataB2H29fbinv_ICHEPcfg_tight_puWeight68075_metfilter_unblind_all_1pb_rho_data_SingleEMU_Run2016B2H29fbinv_PromptReco");
  TH1D* hmc = (TH1D*)((THStack*)fin->Get("Test_DataB2H29fbinv_ICHEPcfg_tight_puWeight68075_metfilter_unblind_all_1pb_rho_stack"))->GetHistogram();
  
  TH1D* hdt_norm = (TH1D*)hdt->Clone("h_rho_dt_norm");
  TH1D* hmc_norm = (TH1D*)hmc->Clone("h_rho_mc_norm");

  hdt_norm->Scale(1./hdt_norm->Integral());
  hmc_norm->Scale(1./hmc_norm->Integral());

  hratio_dt_mc = (TH1D*)hdt_norm->Clone("h_rho_ratio_dt_mc");
  hratio_dt_mc->Divide(hmc_norm);

  hratio_dt_mc->SetTitle("Rho reweighting: Normalized data/MC, 2016 data 29.53 fb^{-1}.");
  hratio_dt_mc->GetYaxis()->SetTitle("Data/MC");

  char name[10000];
  //sprintf(name, "[0]+[1]*x+[2]*x*x+[3]*x*x*x+[4]*x*x*x*x");
  sprintf(name, "[0]+[1]*x+[2]*x*x+[3]*x*x*x");
  //sprintf(name, "[0]+[1]*x+[2]*x*x");

  TCanvas* plots = new TCanvas("plots", "plots");

  TF1* fratio_dt_mc = new TF1("f_rho_ratio_dt_mc", name, 0,50);
  //fratio_dt_mc->SetParameter(0,0.5);
  //fratio_dt_mc->SetParameter(1,8);
  //fratio_dt_mc->SetParameter(2,10);
  //fratio_dt_mc->SetParameter(3,0.5);
  //fratio_dt_mc->SetParameter(4,13);
  //fratio_dt_mc->SetParameter(5,20);
  //fratio_dt_mc->SetParLimits(0,0,2.0);
  //fratio_dt_mc->SetParLimits(1,4,10);
  //fratio_dt_mc->SetParLimits(2,5,20);
  //fratio_dt_mc->SetParLimits(3,0,2.0);
  //fratio_dt_mc->SetParLimits(4,6,40);
  //fratio_dt_mc->SetParLimits(5,10,50);

  hratio_dt_mc->Fit(fratio_dt_mc);
  fratio_dt_mc->Print("v");

  hratio_dt_mc->GetYaxis()->SetRangeUser(0,5);

  TPaveText* lumipt;
  lumipt = new TPaveText(0.2,0.66,0.8,0.8,"brNDC");
  lumipt->SetBorderSize(0);
  lumipt->SetTextAlign(12);
  lumipt->SetFillStyle(0);
  lumipt->SetTextFont(42);
  lumipt->SetTextSize(0.035);
  sprintf(name, "f(#rho) = %.2f + %.4f #rho + %.4f #rho^{2} + + %.5f #rho^{3}",
        fratio_dt_mc->GetParameter(0), 
        fratio_dt_mc->GetParameter(1), 
        fratio_dt_mc->GetParameter(2), 
        fratio_dt_mc->GetParameter(3)
        );
  lumipt->AddText(0.0,0.6, name);
  lumipt->Draw();

  plots->Print("rho_rewegiht_b2h29fbinv.pdf");
  plots->SaveAs("rho_rewegiht_b2g29fbinv.C");


  sprintf(name, "(%.3f+%.3f*rho+%.3e*rho*rho+%.3e*rho*rho*rho)",
          fratio_dt_mc->GetParameter(0),
          fratio_dt_mc->GetParameter(1),
          fratio_dt_mc->GetParameter(2), 
          fratio_dt_mc->GetParameter(3)
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
