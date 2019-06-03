{
  TFile* fout = TFile::Open("muon_idiso_summer16.root", "recreate");

  TFile* f_mu_id_pog_1 = TFile::Open("MuonID_EfficienciesAndSF_BCDEF.root");
  TFile* f_mu_id_pog_2 = TFile::Open("MuonID_EfficienciesAndSF_GH.root");

  TFile* f_mu_iso_pog_1 = TFile::Open("MuonISO_EfficienciesAndSF_BCDEF.root");
  TFile* f_mu_iso_pog_2 = TFile::Open("MuonISO_EfficienciesAndSF_GH.root");

  TFile* f_mu_id_hpt_1 = TFile::Open("highpt_2016B2F_absetapt.root");
  TFile* f_mu_id_hpt_2 = TFile::Open("highpt_2016GH_absetapt.root");

  TFile* f_mu_id_tkhpt_1 = TFile::Open("tkhighpt_2016B2F_absetapt.root");  
  TFile* f_mu_id_tkhpt_2 = TFile::Open("tkhighpt_2016GH_absetapt.root");  

  TFile* f_mu_iso_1 = TFile::Open("tkiso_2016B2F_absetapt.root");  
  TFile* f_mu_iso_2 = TFile::Open("tkiso_2016GH_absetapt.root");  

  fout->cd();

//  TH1F* h_mu_hpt_data_1 = (TH1F*)f_mu_id_pog_1->Get("MC_NUM_HighPtID_DEN_genTracks_PAR_newpt_eta/efficienciesDATA/abseta_pair_ne_DATA");
//  TH1F* h_mu_hpt_data_2 = (TH1F*)f_mu_id_pog_2->Get("MC_NUM_HighPtID_DEN_genTracks_PAR_newpt_eta/efficienciesDATA/abseta_pair_ne_DATA");

//  TH1F* h_mu_hpt_mc_1 = (TH1F*)f_mu_id_pog_1->Get("MC_NUM_HighPtID_DEN_genTracks_PAR_newpt_eta/efficienciesMC/abseta_pair_ne_MC");
//  TH1F* h_mu_hpt_mc_2 = (TH1F*)f_mu_id_pog_2->Get("MC_NUM_HighPtID_DEN_genTracks_PAR_newpt_eta/efficienciesMC/abseta_pair_ne_MC");

//  TH1F* h_mu_iso_sf_1 = (TH1F*)f_mu_iso_pog_1->Get("tkLooseISO_highptID_newpt_eta/abseta_pair_ne_ratio");
//  TH1F* h_mu_iso_sf_2 = (TH1F*)f_mu_iso_pog_2->Get("tkLooseISO_highptID_newpt_eta/abseta_pair_ne_ratio");

  TH1F* h_mu_hpt_data_1 = (TH1F*)f_mu_id_hpt_1->Get("efficiency_dt");
  TH1F* h_mu_hpt_data_2 = (TH1F*)f_mu_id_hpt_2->Get("efficiency_dt");

  TH1F* h_mu_hpt_mc_1 = (TH1F*)f_mu_id_hpt_1->Get("efficiency_mc");
  TH1F* h_mu_hpt_mc_2 = (TH1F*)f_mu_id_hpt_2->Get("efficiency_mc");

  TH1F* h_mu_tkhpt_data_1 = (TH1F*)f_mu_id_tkhpt_1->Get("efficiency_dt");
  TH1F* h_mu_tkhpt_data_2 = (TH1F*)f_mu_id_tkhpt_2->Get("efficiency_dt");

  TH1F* h_mu_tkhpt_mc_1 = (TH1F*)f_mu_id_tkhpt_1->Get("efficiency_mc");
  TH1F* h_mu_tkhpt_mc_2 = (TH1F*)f_mu_id_tkhpt_2->Get("efficiency_mc");

  TH1F* h_mu_iso_sf_1 = (TH1F*)f_mu_iso_1->Get("scalefactor");
  TH1F* h_mu_iso_sf_2 = (TH1F*)f_mu_iso_2->Get("scalefactor");


  h_mu_hpt_data_1->SetName("h_mu_hpt_data_1");
  h_mu_hpt_data_2->SetName("h_mu_hpt_data_2");

  h_mu_hpt_mc_1->SetName("h_mu_hpt_mc_1");
  h_mu_hpt_mc_2->SetName("h_mu_hpt_mc_2");

  h_mu_tkhpt_data_1->SetName("h_mu_tkhpt_data_1");
  h_mu_tkhpt_data_2->SetName("h_mu_tkhpt_data_2");

  h_mu_tkhpt_mc_1->SetName("h_mu_tkhpt_mc_1");
  h_mu_tkhpt_mc_2->SetName("h_mu_tkhpt_mc_2");

  h_mu_iso_sf_1->SetName("h_mu_iso_sf_1");
  h_mu_iso_sf_2->SetName("h_mu_iso_sf_2");

  h_mu_hpt_data_1->Write();
  h_mu_hpt_data_2->Write();
  h_mu_hpt_mc_1->Write();
  h_mu_hpt_mc_2->Write();

  h_mu_tkhpt_data_1->Write();
  h_mu_tkhpt_data_2->Write();
  h_mu_tkhpt_mc_1->Write();
  h_mu_tkhpt_mc_2->Write();  

  h_mu_iso_sf_1->Write();
  h_mu_iso_sf_2->Write();

  fout->Close();


}
