{

  std::string option="eb";
  std::string tag="_more1";
  std::string foutname="getFileterString_"+option+tag;

  Int_t n_loop=70;


  TFile *_file0 = TFile::Open("/home/heli/XZZ/80X_20170202_GJets_light_big/SinglePhoton_Run2016Full_ReReco_v2_big/vvTreeProducer/tree.root");
  gROOT->ProcessLine(".x tdrstyle.C");
  TTree* tree = (TTree*)_file0->Get("tree");
  tree->SetAlias("absDeltaPhi", "fabs(TVector2::Phi_mpi_pi(gjet_l2_phi-gjet_l1_phi))");
  tree->SetAlias("metPara", "gjet_l2_pt*cos(gjet_l2_phi-gjet_l1_phi)");
  tree->SetAlias("metPerp", "gjet_l2_pt*sin(gjet_l2_phi-gjet_l1_phi)");
  tree->SetAlias("uPara", "-gjet_l2_pt*cos(gjet_l2_phi-gjet_l1_phi)-gjet_l1_pt");
  tree->SetAlias("uPerp", "-gjet_l2_pt*sin(gjet_l2_phi-gjet_l1_phi)");
  tree->SetAlias("ut", "sqrt(uPara*uPara+uPerp*uPerp)");
  tree->SetAlias("eta", "gjet_l1_eta");
  tree->SetAlias("phi", "gjet_l1_phi");
  tree->SetAlias("pt", "gjet_l1_pt");
  tree->SetAlias("metfilter", "(Flag_EcalDeadCellTriggerPrimitiveFilter&&Flag_HBHENoiseIsoFilter&&Flag_goodVertices&&Flag_HBHENoiseFilter&&Flag_globalTightHalo2016Filter&&Flag_eeBadScFilter&&Flag_CSCTightHalo2015Filter)");

  tree->SetAlias("ieta", "gjet_l1_ieta");
  tree->SetAlias("iphi", "gjet_l1_iphi");



  sprintf(name, "%s.root", foutname.c_str());
  TFile* fout = TFile::Open(name, "recreate");

  TCanvas* plots = new TCanvas("plots", "plots");

  sprintf(name, "%s.pdf[", foutname.c_str());
  plots->Print(name);



  // ee+
  if (option=="eep") tree->Draw("gjet_l1_iphi:gjet_l1_ieta>>hist(100,0.5,100.5,100,0.5,100.5)", "eta>1.566&&fabs(uPara)<100", "colz");
  // ee-
  else if (option=="eem") tree->Draw("gjet_l1_iphi:gjet_l1_ieta>>hist(100,0.5,100.5,100,0.5,100.5)", "eta<-1.566&&fabs(uPara)<100", "colz");
  // eb
  else if (option=="eb") tree->Draw("gjet_l1_iphi:gjet_l1_ieta>>hist(181,-90.5,90.5,360,0.5,360.5)", "fabs(eta)<1.47&&fabs(uPara)<100", "colz");
  else std::cout << "wrong option do nothing" << std::endl;

  TH2D* hist = (TH2D*)gDirectory->Get("hist");

  plots->Clear();
  hist->Draw("colz");
  sprintf(name, "%s.pdf[", foutname.c_str());
  plots->Print(name);
  plots->Clear();

  hist->Write("hist_map_before");

  char name[1000];
  std::string selec ;
  if (option=="eep") selec = "eta>1.566";
  else if (option=="eem") selec = "eta<-1.566";
  else if (option=="eb") selec = "fabs(eta)<1.47";
  else std::cout << "wrong option do nothing" << std::endl;

  float utcut=0.5;

  Int_t cx;   Int_t cy;   Int_t bc;
  Int_t bx;   Int_t by; Int_t bz;
  Double_t n1; Double_t n2;

  std::string flag1 = "(";
  std::cout << "(";
  for (int i=0;i<n_loop; i++) {
    hist->GetMaximumBin(bx,by,bz);
    cx = (Int_t)hist->GetXaxis()->GetBinCenter(bx);
    cy = (Int_t)hist->GetYaxis()->GetBinCenter(by);
    bc = (Int_t)hist->GetBinContent(bx,by);
    std::cout << "(ieta==" << cx << "&&iphi==" << cy << ")||";
    std::cout  << "    # bc="<<bc ;
    std::cout << std::endl;

    sprintf(name, "(ieta==%d&&iphi==%d)", cx, cy);

    if (i==n_loop-1) flag1 += std::string(name);
    else flag1 += std::string(name)+"||";

    // draw the xtal
    sprintf(name, "(ieta==%d&&iphi==%d)", cx, cy);
    std::string xtal_selec = selec+"&&"+std::string(name);

    plots->Clear();
    plots->SetLogy(1);
    tree->Draw("metPara/pt>>hist1(60,-3,3)", xtal_selec.c_str(), "e");
    TH1D* hist1 = (TH1D*)gDirectory->Get("hist1");
    sprintf(name, "hmetparavpt_%i", i);
    hist1->SetName(name);
    sprintf(name, "(ieta==%d&&iphi==%d)", cx, cy);
    hist1->SetTitle(name);
    sprintf(name, "%s.pdf", foutname.c_str());
    plots->Print(name);
    hist1->Write();
    plots->SetLogy(0);
    plots->Clear();




    hist->SetBinContent(bx,by,0);
  }; 
  
  hist->Write("hist_map_after");

  flag1 += ")";

  std::cout << flag1 << std::endl;

  std::string selec_flag1 = selec+"&&"+flag1;
  std::string selec_notflag1 = selec+"&&!"+flag1;

  tree->Draw("metPara/pt>>h_metParaVpT_flag1(1000,-10,10)", selec_flag1.c_str(), "e");
  tree->Draw("metPara/pt>>h_metParaVpT_Nflag1(1000,-10,10)", selec_notflag1.c_str(), "e");

  TH1D* h_flg1 = (TH1D*)gDirectory->Get("h_metParaVpT_flag1");
  TH1D* h_nflg1 = (TH1D*)gDirectory->Get("h_metParaVpT_Nflag1");

  h_flg1->Scale(1./h_flg1->Integral());
  h_nflg1->Scale(1./h_nflg1->Integral());

  h_flg1->SetLineColor(2);
  h_nflg1->SetLineColor(4);

  plots->Clear();
  plots->SetLogy(1);
  h_flg1->Draw();
  h_nflg1->Draw("same");
  sprintf(name, "%s.pdf", foutname.c_str());
  plots->Print(name);
  plots->SetLogy(0);
  plots->Clear();

  h_flg1->Write();
  h_nflg1->Write();


  sprintf(name, "%s.pdf]", foutname.c_str());
  plots->Print(name);

}
