{


  TFile* fin = new TFile("Test1_NoRecoilZJets_NoRhoWt_DataB2H33fbinv_tight_puWeight68075_metfilter_all_log_1pb.root");
  TFile* fout = new TFile("nVertratio_b2h33fbinv.root", "recreate");

  gROOT->ProcessLine(".x tdrstyle.h");

  TH1D* hdt = (TH1D*)fin->Get("Test1_NoRecoilZJets_NoRhoWt_DataB2H33fbinv_tight_puWeight68075_metfilter_all_log_1pb_nVert_data_SingleEMU_Run2016B2H_ReReco_33fbinv");
  TH1D* hmc = (TH1D*)((THStack*)fin->Get("Test1_NoRecoilZJets_NoRhoWt_DataB2H33fbinv_tight_puWeight68075_metfilter_all_log_1pb_nVert_stack"))->GetHistogram();
  
  TH1D* hdt_norm = (TH1D*)hdt->Clone("h_nVert_dt_norm");
  TH1D* hmc_norm = (TH1D*)hmc->Clone("h_nVert_mc_norm");

  hdt_norm->Scale(1./hdt_norm->Integral());
  hmc_norm->Scale(1./hmc_norm->Integral());

  hratio_dt_mc = (TH1D*)hdt_norm->Clone("h_nVert_ratio_dt_mc");
  hratio_dt_mc->Divide(hmc_norm);

  hratio_dt_mc->SetTitle("NVtx reweighting: Normalized data/MC, 2016 data 33.59 fb^{-1}.");
  hratio_dt_mc->GetYaxis()->SetTitle("Data/MC");

  char name[10000];
  sprintf(name, "[0]+[1]*x+[2]*x*x+[3]*exp([4]*x)");
  //sprintf(name, "[0]+[1]*x+[2]*x*x+[3]*x*x*x+[4]*x*x*x*x+[5]*exp(x)");
  //sprintf(name, "[0]+[1]*x+[2]*x*x+[3]*x*x*x+[4]*x*x*x*x+[5]*x*x*x*x*x");
  //sprintf(name, "[0]+[1]*x+[2]*x*x+[3]*x*x*x+[4]*x*x*x*x");
  //sprintf(name, "[0]+[1]*x+[2]*x*x+[3]*x*x*x");
  //sprintf(name, "[0]+[1]*x+[2]*x*x");

  TCanvas* plots = new TCanvas("plots", "plots");

  TF1* fratio_dt_mc = new TF1("f_nVert_ratio_dt_mc", name, 0,50);
  fratio_dt_mc->FixParameter(4,0.25);
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

  hratio_dt_mc->GetYaxis()->SetRangeUser(0,100);

  TPaveText* lumipt;
  lumipt = new TPaveText(0.2,0.66,0.8,0.8,"brNDC");
  lumipt->SetBorderSize(0);
  lumipt->SetTextAlign(12);
  lumipt->SetFillStyle(0);
  lumipt->SetTextFont(42);
  lumipt->SetTextSize(0.03);
  sprintf(name, 
        //"f(#nVert) = %.2f + %.4f #nVert + %.4f #nVert^{2} + %.5f #nVert^{3}",
        //"f(#nVert) = %.2e + %.3e #nVert + %.3e #nVert^{2} + %.5e #nVert^{3} + %.3e #nVert^{4}",
        //"f(#nVert) = %.2e + %.3e #nVert + %.3e #nVert^{2} + %.5e #nVert^{3} + %.3e #nVert^{4} + %.3e #nVert^{5}",
        //"f(nVert) = %.2e + %.3e nVert + %.3e nVert^{2} + %.5e nVert^{3} + %.3e nVert^{4} + %.3e exp(nVert)",
        //"f(nVert) = %.2e + %.3e nVert + %.3e nVert^{2} + %.5e nVert^{3} + %.3e nVert^{4} + %.3e exp(nVert)",
        "f(x) = %.1e + %.1e x + %.1e x^{2} + %.1e exp(%.1e x)",
        fratio_dt_mc->GetParameter(0), 
        fratio_dt_mc->GetParameter(1), 
        fratio_dt_mc->GetParameter(2), 
        fratio_dt_mc->GetParameter(3),
        fratio_dt_mc->GetParameter(4)
       // fratio_dt_mc->GetParameter(5)
        );
  lumipt->AddText(0.0,0.6, name);
  lumipt->Draw();

  plots->Print("nVert_rewegiht_b2h33fbinv.pdf");
  plots->SaveAs("nVert_rewegiht_b2g33fbinv.C");


  sprintf(name, 
          //"(%.3f+%.3f*nVert+%.3e*nVert*nVert+%.3e*nVert*nVert*nVert)",
          //"(%.3f+%.3f*nVert+%.3e*nVert*nVert+%.3e*nVert*nVert*nVert+%.3e*nVert*nVert*nVert*nVert)",
          //"(%.3f+%.3f*nVert+%.3e*nVert*nVert+%.3e*nVert*nVert*nVert+%.3e*nVert*nVert*nVert*nVert+%.3e*nVert*nVert*nVert*nVert*nVert)",
          //"(%.3f+%.3f*nVert+%.3e*nVert*nVert+%.3e*nVert*nVert*nVert+%.3e*nVert*nVert*nVert*nVert+%.3e*exp(nVert))",
          "(%.3f+%.3f*nVert+%.3e*nVert*nVert+%.3e*exp(%.3e*nVert))",
          fratio_dt_mc->GetParameter(0),
          fratio_dt_mc->GetParameter(1),
          fratio_dt_mc->GetParameter(2), 
          fratio_dt_mc->GetParameter(3),
          fratio_dt_mc->GetParameter(4)
//          fratio_dt_mc->GetParameter(5)
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
