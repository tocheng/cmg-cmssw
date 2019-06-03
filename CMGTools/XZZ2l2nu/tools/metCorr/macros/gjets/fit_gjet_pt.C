{

  TFile* fin=TFile::Open("study_gjets_data_b2h36p22fbinv_v2.root");
  //TFile* fin=TFile::Open("study_gjets_data_b2h36p22fbinv.root");

  char name[1000];

  TCanvas* plots = new TCanvas("plots");
  plots->SetLogx();

  //TH1D* hr = (TH1D*)fin->Get("h_zpt_lowlpt_ratio");
  //TH1D* hrel = (TH1D*)fin->Get("h_zpt_lowlpt_ratio_el");
  //TH1D* hrmu = (TH1D*)fin->Get("h_zpt_lowlpt_ratio_mu");
  TH1D* hr = (TH1D*)fin->Get("h_zpt_ratio");
  TH1D* hrel = (TH1D*)fin->Get("h_zpt_ratio_el");
  TH1D* hrmu = (TH1D*)fin->Get("h_zpt_ratio_mu");

  hrmu->GetXaxis()->SetRangeUser(21,3000);

  TF1* fcmu = new TF1("fcmu", "([0]+[1]*TMath::Erf((x-[2])/[3])+[4]*TMath::Erf((x-[5])/[6])+[7]*TMath::Erf((x-[8])/[9]))",0,3000);
  //TF1* fcmu = new TF1("fcmu", "([0]+[1]*TMath::Erf((x-[2])/[3])+[4]*TMath::Erf((x-[5])/[6]))",0,3000);
//  TF1* fcmu = new TF1("fcmu", "[0]-[1]*TMath::Erf((x-[2])/[3])",0,3000);

  fcmu->SetParameters(1e-09,1.5e-09,150,350,9e-10,71,83);
  //fcmu->SetParameters(1e-09,-1.5e-09,150,350,9e-10,171,83, -1e-9,200,100);
  //fcmu->SetParameters(1e-09,1.5e-09,150,350,9e-10,71,83, -1e-9,200,100);

  int nfits=100;
  for (int i=0; i<nfits; i++){
    hrmu->Fit(fcmu, "R", "", 50, 3000);
    //hrel->Fit(fcmu, "R", "", 120, 3000);
    //hr->Fit(fcmu, "R", "", 50, 3000);
  }

  sprintf(name,
          "%.3e+%.3e*TMath::Erf((x-%.3e)/%.3e)+%.3e*TMath::Erf((x-%.3e)/%.3e)+%.3e*TMath::Erf((x-%.3e)/%.3e)",
          //"%.3e+%.3e*TMath::Erf((x-%.3e)/%.3e)+%.3e*TMath::Erf((x-%.3e)/%.3e)",
          fcmu->GetParameter(0), fcmu->GetParameter(1), fcmu->GetParameter(2), fcmu->GetParameter(3), fcmu->GetParameter(4), fcmu->GetParameter(5),
          //fcmu->GetParameter(6)
          fcmu->GetParameter(6), fcmu->GetParameter(7), fcmu->GetParameter(8),fcmu->GetParameter(9)
         );

  std::cout << "#############################################" << std::endl;
  std::cout << "func_mu = " << name << std::endl;
  std::cout << "#############################################" << std::endl;
}
