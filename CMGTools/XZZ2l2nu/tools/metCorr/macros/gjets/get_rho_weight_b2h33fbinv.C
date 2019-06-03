{

  TFile* file1 = TFile::Open("/home/heli/XZZ/80X_20161029_light_Skim/SingleEMU_Run2016B2H_ReReco_33fbinv.root");
  TFile* file2 = TFile::Open("/home/heli/XZZ/80X_20161029_GJets_light/SinglePhoton_Run2016B2H_ReReco_33fbinv/vvTreeProducer/tree.root");
  TFile* fout = TFile::Open("get_rho_weight_33fbinv.root", "recreate");
  //TFile* fout = TFile::Open("get_rho_weight_33fbinv_el.root", "recreate");
  //TFile* fout = TFile::Open("get_rho_weight_33fbinv_mu.root", "recreate");
  TTree* tree1 = (TTree*)file1->Get("tree");
  TTree* tree2 = (TTree*)file2->Get("tree");

  
  Double_t ZPtBins[] = {0,30,36,50,75,90,120,165,3000};
  Int_t NZPtBins = sizeof(ZPtBins)/sizeof(ZPtBins[0]) - 1;
  Double_t RhoBins[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100};

  Int_t NRhoBins = sizeof(RhoBins)/sizeof(RhoBins[0]) - 1; 

  TH2D* h_rho_zpt_1 = new TH2D("h_rho_zpt_1", "h_rho_zpt_1", NZPtBins, ZPtBins, NRhoBins, RhoBins);
  TH2D* h_rho_zpt_2 = new TH2D("h_rho_zpt_2", "h_rho_zpt_2", NZPtBins, ZPtBins, NRhoBins, RhoBins);

  h_rho_zpt_1->Sumw2();
  h_rho_zpt_2->Sumw2();

  tree1->Draw("rho:llnunu_l1_pt>>h_rho_zpt_1");
  //tree1->Draw("rho:llnunu_l1_pt>>h_rho_zpt_1", "abs(llnunu_l1_l1_pdgId)==11");
  //tree1->Draw("rho:llnunu_l1_pt>>h_rho_zpt_1", "abs(llnunu_l1_l1_pdgId)==13");
  //tree2->Draw("rho:llnunu_l1_pt>>h_rho_zpt_2");
  tree2->Draw("rho:gjet_l1_pt>>h_rho_zpt_2");


  TH1D* h_zpt_1 = (TH1D*)h_rho_zpt_1->ProjectionX("h_zpt_1");
  TH1D* h_zpt_2 = (TH1D*)h_rho_zpt_2->ProjectionX("h_zpt_2");

  h_zpt_1->SetTitle("h_zpt_1");
  h_zpt_2->SetTitle("h_zpt_2");

  TH2D* h_rho_zpt_1_zptnorm = (TH2D*)h_rho_zpt_1->Clone("h_rho_zpt_1_zptnorm");
  TH2D* h_rho_zpt_2_zptnorm = (TH2D*)h_rho_zpt_2->Clone("h_rho_zpt_2_zptnorm");

  h_rho_zpt_1_zptnorm->SetTitle("h_rho_zpt_1_zptnorm");
  h_rho_zpt_2_zptnorm->SetTitle("h_rho_zpt_2_zptnorm");

  for (int izpt=0; izpt<NZPtBins; izpt++){
    double nzpt1 = h_zpt_1->GetBinContent(izpt+1);
    double nzpt2 = h_zpt_2->GetBinContent(izpt+1);
 
    for (int irho=0; irho<NRhoBins; irho++){
      h_rho_zpt_1_zptnorm->SetBinContent(izpt+1, irho+1, h_rho_zpt_1_zptnorm->GetBinContent(izpt+1, irho+1)/nzpt1);
      h_rho_zpt_1_zptnorm->SetBinError(izpt+1, irho+1, h_rho_zpt_1_zptnorm->GetBinError(izpt+1, irho+1)/nzpt1);
      h_rho_zpt_2_zptnorm->SetBinContent(izpt+1, irho+1, h_rho_zpt_2_zptnorm->GetBinContent(izpt+1, irho+1)/nzpt2);
      h_rho_zpt_2_zptnorm->SetBinError(izpt+1, irho+1, h_rho_zpt_2_zptnorm->GetBinError(izpt+1, irho+1)/nzpt2);
    }

  }

  h_rho_zpt_weight = (TH2D*)h_rho_zpt_1_zptnorm->Clone("h_rho_zpt_weight");
  h_rho_zpt_weight->Divide(h_rho_zpt_2_zptnorm);
  h_rho_zpt_weight->SetTitle("h_rho_zpt_weight");

  fout->cd();

  h_rho_zpt_1->Write();
  h_rho_zpt_2->Write();

  h_zpt_1->Write();
  h_zpt_2->Write();
  
  h_rho_zpt_1_zptnorm->Write();
  h_rho_zpt_2_zptnorm->Write();

  h_rho_zpt_weight->Write();


}
