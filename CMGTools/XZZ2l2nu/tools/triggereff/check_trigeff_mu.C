{

  TFile* file1 = TFile::Open("/home/heli/XZZ/80X_20161018_light_Skim/DYJetsToLL_M50_reHLT_NoRecoil.root");
  //TFile* file1 = TFile::Open("/home/heli/XZZ/80X_20160825_light_Skim/SingleEMU_Run2016BCD_PromptReco.root");
  TTree* tree = (TTree*)file1->Get("tree");


  //std::string  lumiTag = "CMS 13 TeV 2016 L=29.53 fb^{-1}";
  //std::string  lumiTag = "CMS 13 TeV 2016 L=27.22 fb^{-1}";
  //std::string  lumiTag = "CMS 13 TeV 2016 L=12.9 fb^{-1}";
  std::string lumiTag = "CMS 13 TeV Simulation for 2016 Data";

  TPaveText* lumipt = new TPaveText(0.2,0.9,0.8,0.98,"brNDC");
  lumipt->SetBorderSize(0);
  lumipt->SetTextAlign(12);
  lumipt->SetFillStyle(0);
  lumipt->SetTextFont(42);
  lumipt->SetTextSize(0.03);
  lumipt->AddText(0.15,0.3, lumiTag.c_str());

  char outputplot[1000];
  sprintf(outputplot,"%s", "check_trigeff_mu_29p53.pdf");
  TCanvas* plots = new TCanvas("plots", "plots");
  char name[10000];
  sprintf(name, "%s[", outputplot);
  plots->Print(name);

  std::string sel_base = "((llnunu_l1_mass>70&&llnunu_l1_mass<110)&&abs(llnunu_l1_l1_pdgId)==13&&abs(llnunu_l1_l2_pdgId)==13&&llnunu_l1_l1_pt>50&&fabs(llnunu_l1_l1_eta)<2.4&&llnunu_l1_l2_pt>20&&fabs(llnunu_l1_l2_eta)<2.4&&(llnunu_l1_l1_highPtID>=0.991||llnunu_l1_l2_highPtID>=0.99))";

  std::string sel_pass = sel_base+"&&(HLT_MUv2)";
  std::string sel_tgsf = sel_base+"*(trgsf)";


  Double_t PtBins[] = {20, 25, 30, 32, 34, 36, 38, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 58, 60, 66, 72, 78, 84, 90, 96, 112, 118, 124, 130, 140, 160, 180, 200, 3000};
  Int_t NPtBins = sizeof(PtBins)/sizeof(PtBins[0]) - 1;

  Double_t EtaBins[] = {-0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2.0, 2.02,2.04,2.06,2.08, 2.1, 2.12,2.14, 2.16, 2.18, 2.2, 2.25, 2.5};
  Int_t NEtaBins = sizeof(EtaBins)/sizeof(EtaBins[0]) - 1;



  // validation
  TH2D* htrg_l1_base = new TH2D("htrg_l1_base", "htrg_l1_base", NPtBins, PtBins, NEtaBins, EtaBins);
  TH2D* htrg_l2_base = new TH2D("htrg_l2_base", "htrg_l1_base", NPtBins, PtBins, NEtaBins, EtaBins);
  TH2D* htrg_l1_pass = new TH2D("htrg_l1_pass", "htrg_l1_pass", NPtBins, PtBins, NEtaBins, EtaBins);
  TH2D* htrg_l2_pass = new TH2D("htrg_l2_pass", "htrg_l1_pass", NPtBins, PtBins, NEtaBins, EtaBins);
  htrg_l1_base->Sumw2();
  htrg_l2_base->Sumw2();
  htrg_l1_pass->Sumw2();
  htrg_l2_pass->Sumw2();

  TH1D* htrg_l1_pt_base  = new TH1D("htrg_l1_pt_base", "htrg_l1_pt_base", 100, 0, 1000);
  TH1D* htrg_l2_pt_base  = new TH1D("htrg_l2_pt_base", "htrg_l2_pt_base", 100, 0, 500);
  TH1D* htrg_l1_pt_pass  = new TH1D("htrg_l1_pt_pass", "htrg_l1_pt_pass", 100, 0, 1000);
  TH1D* htrg_l2_pt_pass  = new TH1D("htrg_l2_pt_pass", "htrg_l2_pt_pass", 100, 0, 500);

  htrg_l1_pt_base->Sumw2();
  htrg_l2_pt_base->Sumw2();
  htrg_l1_pt_pass->Sumw2();
  htrg_l2_pt_pass->Sumw2();

  TH1D* htrg_l1_eta_base  = new TH1D("htrg_l1_eta_base", "htrg_l1_eta_base", 30, -3, 3);
  TH1D* htrg_l2_eta_base  = new TH1D("htrg_l2_eta_base", "htrg_l2_eta_base", 30, -3, 3);
  TH1D* htrg_l1_eta_pass  = new TH1D("htrg_l1_eta_pass", "htrg_l1_eta_pass", 30, -3, 3);
  TH1D* htrg_l2_eta_pass  = new TH1D("htrg_l2_eta_pass", "htrg_l2_eta_pass", 30, -3, 3);

  htrg_l1_eta_base->Sumw2();
  htrg_l2_eta_base->Sumw2();
  htrg_l1_eta_pass->Sumw2();
  htrg_l2_eta_pass->Sumw2();

  // get trig eff from data sample
  tree->Draw("fabs(llnunu_l1_l1_eta):llnunu_l1_l1_pt>>htrg_l1_base",  sel_base.c_str());
  tree->Draw("fabs(llnunu_l1_l2_eta):llnunu_l1_l2_pt>>htrg_l2_base",  sel_base.c_str());
  tree->Draw("fabs(llnunu_l1_l1_eta):llnunu_l1_l1_pt>>htrg_l1_pass",  sel_pass.c_str());
  tree->Draw("fabs(llnunu_l1_l2_eta):llnunu_l1_l2_pt>>htrg_l2_pass",  sel_pass.c_str());

  tree->Draw("llnunu_l1_l1_pt>>htrg_l1_pt_base",  sel_base.c_str());
  tree->Draw("llnunu_l1_l2_pt>>htrg_l2_pt_base",  sel_base.c_str());
  tree->Draw("llnunu_l1_l1_pt>>htrg_l1_pt_pass",  sel_pass.c_str());
  tree->Draw("llnunu_l1_l2_pt>>htrg_l2_pt_pass",  sel_pass.c_str());

  tree->Draw("llnunu_l1_l1_eta>>htrg_l1_eta_base",  sel_base.c_str());
  tree->Draw("llnunu_l1_l2_eta>>htrg_l2_eta_base",  sel_base.c_str());
  tree->Draw("llnunu_l1_l1_eta>>htrg_l1_eta_pass",  sel_pass.c_str());
  tree->Draw("llnunu_l1_l2_eta>>htrg_l2_eta_pass",  sel_pass.c_str());

  // trig eff from data
  TH2D* htrg_eff_l1_trig = (TH2D*)htrg_l1_pass->Clone("htrg_eff_l1_trig");
  TH2D* htrg_eff_l2_trig = (TH2D*)htrg_l2_pass->Clone("htrg_eff_l2_trig");

  htrg_eff_l1_trig->Divide(htrg_l1_base);
  htrg_eff_l2_trig->Divide(htrg_l2_base);

  TH1D* htrg_eff_l1_pt_trig = (TH1D*)htrg_l1_pt_pass->Clone("htrg_eff_l1_pt_trig");
  TH1D* htrg_eff_l2_pt_trig = (TH1D*)htrg_l2_pt_pass->Clone("htrg_eff_l2_pt_trig");
  TH1D* htrg_eff_l1_eta_trig = (TH1D*)htrg_l1_eta_pass->Clone("htrg_eff_l1_eta_trig");
  TH1D* htrg_eff_l2_eta_trig = (TH1D*)htrg_l2_eta_pass->Clone("htrg_eff_l2_eta_trig");

  htrg_eff_l1_pt_trig->Divide(htrg_l1_pt_base);
  htrg_eff_l2_pt_trig->Divide(htrg_l2_pt_base);
  htrg_eff_l1_eta_trig->Divide(htrg_l1_eta_base);
  htrg_eff_l2_eta_trig->Divide(htrg_l2_eta_base);


  // check real sf
  TH2D* htrg_l1_tgsf = new TH2D("htrg_l1_tgsf", "htrg_l1_tgsf", NPtBins, PtBins, NEtaBins, EtaBins);
  TH2D* htrg_l2_tgsf = new TH2D("htrg_l2_tgsf", "htrg_l1_tgsf", NPtBins, PtBins, NEtaBins, EtaBins);
  htrg_l1_tgsf->Sumw2();
  htrg_l2_tgsf->Sumw2();

  TH1D* htrg_l1_pt_tgsf  = new TH1D("htrg_l1_pt_tgsf", "htrg_l1_pt_tgsf", 100, 0, 1000);
  TH1D* htrg_l2_pt_tgsf  = new TH1D("htrg_l2_pt_tgsf", "htrg_l2_pt_tgsf", 100, 0, 500);
  htrg_l1_pt_tgsf->Sumw2();
  htrg_l2_pt_tgsf->Sumw2();

  TH1D* htrg_l1_eta_tgsf  = new TH1D("htrg_l1_eta_tgsf", "htrg_l1_eta_tgsf", 30, -3, 3);
  TH1D* htrg_l2_eta_tgsf  = new TH1D("htrg_l2_eta_tgsf", "htrg_l2_eta_tgsf", 30, -3, 3);
  htrg_l1_eta_tgsf->Sumw2();
  htrg_l2_eta_tgsf->Sumw2();


  tree->Draw("fabs(llnunu_l1_l1_eta):llnunu_l1_l1_pt>>htrg_l1_tgsf",  sel_tgsf.c_str());
  tree->Draw("fabs(llnunu_l1_l2_eta):llnunu_l1_l2_pt>>htrg_l2_tgsf",  sel_tgsf.c_str());

  tree->Draw("llnunu_l1_l1_pt>>htrg_l1_pt_tgsf",  sel_tgsf.c_str());
  tree->Draw("llnunu_l1_l2_pt>>htrg_l2_pt_tgsf",  sel_tgsf.c_str());

  tree->Draw("llnunu_l1_l1_eta>>htrg_l1_eta_tgsf",  sel_tgsf.c_str());
  tree->Draw("llnunu_l1_l2_eta>>htrg_l2_eta_tgsf",  sel_tgsf.c_str());


  // eff by sf
  TH2D* htrg_eff_l1_tgsf = (TH2D*)htrg_l1_tgsf->Clone("htrg_eff_l1_tgsf");
  TH2D* htrg_eff_l2_tgsf = (TH2D*)htrg_l2_tgsf->Clone("htrg_eff_l2_tgsf");
  htrg_eff_l1_tgsf->Divide(htrg_l1_base);
  htrg_eff_l2_tgsf->Divide(htrg_l2_base);

  TH1D* htrg_eff_l1_pt_tgsf = (TH1D*)htrg_l1_pt_tgsf->Clone("htrg_eff_l1_pt_tgsf");
  TH1D* htrg_eff_l2_pt_tgsf = (TH1D*)htrg_l2_pt_tgsf->Clone("htrg_eff_l2_pt_tgsf");
  htrg_eff_l1_pt_tgsf->Divide(htrg_l1_pt_base);
  htrg_eff_l2_pt_tgsf->Divide(htrg_l2_pt_base);

  TH1D* htrg_eff_l1_eta_tgsf = (TH1D*)htrg_l1_eta_tgsf->Clone("htrg_eff_l1_eta_tgsf");
  TH1D* htrg_eff_l2_eta_tgsf = (TH1D*)htrg_l2_eta_tgsf->Clone("htrg_eff_l2_eta_tgsf");
  htrg_eff_l1_eta_tgsf->Divide(htrg_l1_eta_base);
  htrg_eff_l2_eta_tgsf->Divide(htrg_l2_eta_base);

  Double_t N_base = htrg_l1_base->Integral();
  Double_t N_pass = htrg_l1_pass->Integral();
  Double_t N_tgsf = htrg_l1_tgsf->Integral();
  std::cout << "N_pass = " << N_pass << "; N_tgsf = " << N_tgsf << "; N_base = " << N_base << std::endl;
  std::cout << "N_pass/N_tgsf = " << N_pass/N_tgsf << "; N_pass/N_base = " << N_pass/N_base << "; N_tgsf/N_base = " << N_tgsf/N_base << std::endl;


  TH1D* htrg_l1_pt_pass_vs_tgsf = (TH1D*)htrg_l1_pt_pass->Clone("htrg_l1_pt_pass_vs_tgsf");
  htrg_l1_pt_pass_vs_tgsf->Divide(htrg_l1_pt_tgsf);
  TH1D* htrg_l2_pt_pass_vs_tgsf = (TH1D*)htrg_l2_pt_pass->Clone("htrg_l2_pt_pass_vs_tgsf");
  htrg_l2_pt_pass_vs_tgsf->Divide(htrg_l2_pt_tgsf);
  TH1D* htrg_l1_eta_pass_vs_tgsf = (TH1D*)htrg_l1_eta_pass->Clone("htrg_l1_eta_pass_vs_tgsf");
  htrg_l1_eta_pass_vs_tgsf->Divide(htrg_l1_eta_tgsf);
  TH1D* htrg_l2_eta_pass_vs_tgsf = (TH1D*)htrg_l2_eta_pass->Clone("htrg_l2_eta_pass_vs_tgsf");
  htrg_l2_eta_pass_vs_tgsf->Divide(htrg_l2_eta_tgsf);
  TH1D* htrg_eff_l1_pt_trig_vs_tgsf = (TH1D*)htrg_eff_l1_pt_trig->Clone("htrg_eff_l1_pt_trig_vs_tgsf");
  htrg_eff_l1_pt_trig_vs_tgsf->Divide(htrg_eff_l1_pt_tgsf);
  TH1D* htrg_eff_l2_pt_trig_vs_tgsf = (TH1D*)htrg_eff_l2_pt_trig->Clone("htrg_eff_l2_pt_trig_vs_tgsf");
  htrg_eff_l2_pt_trig_vs_tgsf->Divide(htrg_eff_l2_pt_tgsf);
  TH1D* htrg_eff_l1_eta_trig_vs_tgsf = (TH1D*)htrg_eff_l1_eta_trig->Clone("htrg_eff_l1_eta_trig_vs_tgsf");
  htrg_eff_l1_eta_trig_vs_tgsf->Divide(htrg_eff_l1_eta_tgsf);
  TH1D* htrg_eff_l2_eta_trig_vs_tgsf = (TH1D*)htrg_eff_l2_eta_trig->Clone("htrg_eff_l2_eta_trig_vs_tgsf");
  htrg_eff_l2_eta_trig_vs_tgsf->Divide(htrg_eff_l2_eta_tgsf);


  htrg_l1_pt_base->SetMarkerStyle(20);
  htrg_l2_pt_base->SetMarkerStyle(20);
  htrg_l1_pt_pass->SetMarkerStyle(20);
  htrg_l2_pt_pass->SetMarkerStyle(20);
  htrg_l1_eta_base->SetMarkerStyle(20);
  htrg_l2_eta_base->SetMarkerStyle(20);
  htrg_l1_eta_pass->SetMarkerStyle(20);
  htrg_l2_eta_pass->SetMarkerStyle(20);
  htrg_eff_l1_pt_trig->SetMarkerStyle(20);
  htrg_eff_l2_pt_trig->SetMarkerStyle(20);
  htrg_eff_l1_eta_trig->SetMarkerStyle(20);
  htrg_eff_l2_eta_trig->SetMarkerStyle(20);
  htrg_l1_pt_tgsf->SetMarkerStyle(20);
  htrg_l2_pt_tgsf->SetMarkerStyle(20);
  htrg_l1_eta_tgsf->SetMarkerStyle(20);
  htrg_l2_eta_tgsf->SetMarkerStyle(20);
  htrg_eff_l1_pt_tgsf->SetMarkerStyle(20);
  htrg_eff_l2_pt_tgsf->SetMarkerStyle(20);
  htrg_eff_l1_eta_tgsf->SetMarkerStyle(20);
  htrg_eff_l2_eta_tgsf->SetMarkerStyle(20);

  htrg_l1_pt_base->SetMarkerSize(0.5);
  htrg_l2_pt_base->SetMarkerSize(0.5);
  htrg_l1_pt_pass->SetMarkerSize(0.5);
  htrg_l2_pt_pass->SetMarkerSize(0.5);
  htrg_l1_eta_base->SetMarkerSize(0.5);
  htrg_l2_eta_base->SetMarkerSize(0.5);
  htrg_l1_eta_pass->SetMarkerSize(0.5);
  htrg_l2_eta_pass->SetMarkerSize(0.5);
  htrg_eff_l1_pt_trig->SetMarkerSize(0.5);
  htrg_eff_l2_pt_trig->SetMarkerSize(0.5);
  htrg_eff_l1_eta_trig->SetMarkerSize(0.5);
  htrg_eff_l2_eta_trig->SetMarkerSize(0.5);
  htrg_l1_pt_tgsf->SetMarkerSize(0.5);
  htrg_l2_pt_tgsf->SetMarkerSize(0.5);
  htrg_l1_eta_tgsf->SetMarkerSize(0.5);
  htrg_l2_eta_tgsf->SetMarkerSize(0.5);
  htrg_eff_l1_pt_tgsf->SetMarkerSize(0.5);
  htrg_eff_l2_pt_tgsf->SetMarkerSize(0.5);
  htrg_eff_l1_eta_tgsf->SetMarkerSize(0.5);
  htrg_eff_l2_eta_tgsf->SetMarkerSize(0.5);

  htrg_l1_pt_base->GetXaxis()->SetTitle("p_{T} (GeV)");
  htrg_l2_pt_base->GetXaxis()->SetTitle("p_{T} (GeV)");
  htrg_l1_pt_pass->GetXaxis()->SetTitle("p_{T} (GeV)");
  htrg_l2_pt_pass->GetXaxis()->SetTitle("p_{T} (GeV)");
  htrg_eff_l1_pt_trig->GetXaxis()->SetTitle("p_{T} (GeV)");
  htrg_eff_l2_pt_trig->GetXaxis()->SetTitle("p_{T} (GeV)");
  htrg_l1_pt_tgsf->GetXaxis()->SetTitle("p_{T} (GeV)");
  htrg_l2_pt_tgsf->GetXaxis()->SetTitle("p_{T} (GeV)");
  htrg_eff_l1_pt_tgsf->GetXaxis()->SetTitle("p_{T} (GeV)");
  htrg_eff_l2_pt_tgsf->GetXaxis()->SetTitle("p_{T} (GeV)");
  htrg_l1_eta_base->GetXaxis()->SetTitle("#eta");
  htrg_l2_eta_base->GetXaxis()->SetTitle("#eta");
  htrg_l1_eta_pass->GetXaxis()->SetTitle("#eta");
  htrg_l2_eta_pass->GetXaxis()->SetTitle("#eta");
  htrg_eff_l1_eta_trig->GetXaxis()->SetTitle("#eta");
  htrg_eff_l2_eta_trig->GetXaxis()->SetTitle("#eta");
  htrg_l1_eta_tgsf->GetXaxis()->SetTitle("#eta");
  htrg_l2_eta_tgsf->GetXaxis()->SetTitle("#eta");
  htrg_eff_l1_eta_tgsf->GetXaxis()->SetTitle("#eta");
  htrg_eff_l2_eta_tgsf->GetXaxis()->SetTitle("#eta");

  htrg_l1_pt_base->GetYaxis()->SetTitle("Entries");
  htrg_l2_pt_base->GetYaxis()->SetTitle("Entries");
  htrg_l1_pt_pass->GetYaxis()->SetTitle("Entries");
  htrg_l2_pt_pass->GetYaxis()->SetTitle("Entries");
  htrg_l1_eta_base->GetYaxis()->SetTitle("Entries");
  htrg_l2_eta_base->GetYaxis()->SetTitle("Entries");
  htrg_l1_eta_pass->GetYaxis()->SetTitle("Entries");
  htrg_l2_eta_pass->GetYaxis()->SetTitle("Entries");
  htrg_l1_pt_tgsf->GetYaxis()->SetTitle("Entries");
  htrg_l2_pt_tgsf->GetYaxis()->SetTitle("Entries");
  htrg_l1_eta_tgsf->GetYaxis()->SetTitle("Entries");
  htrg_l2_eta_tgsf->GetYaxis()->SetTitle("Entries");
  htrg_eff_l1_pt_trig->GetYaxis()->SetTitle("eff.");
  htrg_eff_l2_pt_trig->GetYaxis()->SetTitle("eff.");
  htrg_eff_l1_eta_trig->GetYaxis()->SetTitle("eff.");
  htrg_eff_l2_eta_trig->GetYaxis()->SetTitle("eff.");
  htrg_eff_l1_pt_tgsf->GetYaxis()->SetTitle("eff.");
  htrg_eff_l2_pt_tgsf->GetYaxis()->SetTitle("eff.");
  htrg_eff_l1_eta_tgsf->GetYaxis()->SetTitle("eff.");
  htrg_eff_l2_eta_tgsf->GetYaxis()->SetTitle("eff.");

  htrg_l1_pt_pass_vs_tgsf->SetMarkerStyle(20);
  htrg_l2_pt_pass_vs_tgsf->SetMarkerStyle(20);
  htrg_l1_eta_pass_vs_tgsf->SetMarkerStyle(20);
  htrg_l2_eta_pass_vs_tgsf->SetMarkerStyle(20);
  htrg_eff_l1_pt_trig_vs_tgsf->SetMarkerStyle(20);
  htrg_eff_l2_pt_trig_vs_tgsf->SetMarkerStyle(20);
  htrg_eff_l1_eta_trig_vs_tgsf->SetMarkerStyle(20);
  htrg_eff_l2_eta_trig_vs_tgsf->SetMarkerStyle(20);
  htrg_l1_pt_pass_vs_tgsf->GetXaxis()->SetTitle("p_{T} (GeV)");
  htrg_l2_pt_pass_vs_tgsf->GetXaxis()->SetTitle("p_{T} (GeV)");
  htrg_eff_l1_pt_trig_vs_tgsf->GetXaxis()->SetTitle("p_{T} (GeV)");
  htrg_eff_l2_pt_trig_vs_tgsf->GetXaxis()->SetTitle("p_{T} (GeV)");
  htrg_l1_eta_pass_vs_tgsf->GetXaxis()->SetTitle("#eta");
  htrg_l2_eta_pass_vs_tgsf->GetXaxis()->SetTitle("#eta");
  htrg_eff_l1_eta_trig_vs_tgsf->GetXaxis()->SetTitle("#eta");
  htrg_eff_l2_eta_trig_vs_tgsf->GetXaxis()->SetTitle("#eta");



  htrg_eff_l1_trig->SetTitle("htrg_eff_l1_trig");
  htrg_eff_l2_trig->SetTitle("htrg_eff_l2_trig");
  htrg_eff_l1_pt_trig->SetTitle("htrg_eff_l1_pt_trig");
  htrg_eff_l2_pt_trig->SetTitle("htrg_eff_l2_pt_trig");
  htrg_eff_l1_eta_trig->SetTitle("htrg_eff_l1_eta_trig");
  htrg_eff_l2_eta_trig->SetTitle("htrg_eff_l2_eta_trig");
  htrg_eff_l1_tgsf->SetTitle("htrg_eff_l1_tgsf");
  htrg_eff_l2_tgsf->SetTitle("htrg_eff_l2_tgsf");
  htrg_eff_l1_pt_tgsf->SetTitle("htrg_eff_l1_pt_tgsf");
  htrg_eff_l2_pt_tgsf->SetTitle("htrg_eff_l2_pt_tgsf");
  htrg_eff_l1_eta_tgsf->SetTitle("htrg_eff_l1_eta_tgsf");
  htrg_eff_l2_eta_tgsf->SetTitle("htrg_eff_l2_eta_tgsf");
  htrg_l1_pt_pass_vs_tgsf->SetTitle("htrg_l1_pt_pass_vs_tgsf");
  htrg_l2_pt_pass_vs_tgsf->SetTitle("htrg_l2_pt_pass_vs_tgsf");
  htrg_l1_eta_pass_vs_tgsf->SetTitle("htrg_l1_eta_pass_vs_tgsf");
  htrg_l2_eta_pass_vs_tgsf->SetTitle("htrg_l2_eta_pass_vs_tgsf");
  htrg_eff_l1_pt_trig_vs_tgsf->SetTitle("htrg_eff_l1_pt_trig_vs_tgsf");
  htrg_eff_l2_pt_trig_vs_tgsf->SetTitle("htrg_eff_l2_pt_trig_vs_tgsf");
  htrg_eff_l1_eta_trig_vs_tgsf->SetTitle("htrg_eff_l1_eta_trig_vs_tgsf");
  htrg_eff_l2_eta_trig_vs_tgsf->SetTitle("htrg_eff_l2_eta_trig_vs_tgsf");



  TLegend* lg[100];

  lg[0] = new TLegend(0.3,0.3,0.7,0.5);  
  lg[0]->SetName("lg_htrg_pt_base_l1_l2");
  htrg_l1_pt_base->SetLineColor(2);
  htrg_l2_pt_base->SetLineColor(4);
  htrg_l1_pt_base->SetMarkerColor(2);
  htrg_l2_pt_base->SetMarkerColor(4);
  lg[0]->AddEntry(htrg_l1_pt_base, "l_{1} no trig.", "pl");
  lg[0]->AddEntry(htrg_l2_pt_base, "l_{2} no trig.", "pl");

  plots->cd();
  plots->Clear();
  htrg_l1_pt_base->Draw();
  htrg_l2_pt_base->Draw("same");
  lg[0]->Draw("same");
  lumipt->Draw();
  sprintf(name,"%s",  outputplot);
  plots->Print(name);
  plots->Clear();
 

  lg[1] = new TLegend(0.3,0.3,0.7,0.5);
  lg[1]->SetName("lg_htrg_l1_pt_pass_tgsf");
  htrg_l1_pt_pass->SetLineColor(2);
  htrg_l1_pt_tgsf->SetLineColor(4);
  htrg_l1_pt_pass->SetMarkerColor(2);
  htrg_l1_pt_tgsf->SetMarkerColor(4);
  lg[1]->AddEntry(htrg_l1_pt_pass, "l_{1} pass trig.", "pl");
  lg[1]->AddEntry(htrg_l1_pt_tgsf, "l_{1} trig. s.f.", "pl");

  plots->cd();
  plots->Clear();
  htrg_l1_pt_pass->Draw();
  htrg_l1_pt_tgsf->Draw("same");
  lg[1]->Draw("same");
  lumipt->Draw();
  sprintf(name,"%s", outputplot);
  plots->Print(name);
  plots->Clear();

  lg[2] = new TLegend(0.3,0.3,0.7,0.5);
  lg[2]->SetName("lg_htrg_l2_pt_pass_tgsf");
  htrg_l2_pt_pass->SetLineColor(2);
  htrg_l2_pt_tgsf->SetLineColor(4);
  htrg_l2_pt_pass->SetMarkerColor(2);
  htrg_l2_pt_tgsf->SetMarkerColor(4);
  lg[2]->AddEntry(htrg_l2_pt_pass, "l_{2} pass trig.", "pl");
  lg[2]->AddEntry(htrg_l2_pt_tgsf, "l_{2} trig. s.f.", "pl");

  plots->cd();
  plots->Clear();
  htrg_l2_pt_pass->Draw();
  htrg_l2_pt_tgsf->Draw("same");
  lg[2]->Draw("same");
  lumipt->Draw();
  sprintf(name,"%s", outputplot);
  plots->Print(name);
  plots->Clear();

  lg[3] = new TLegend(0.3,0.3,0.7,0.5);
  lg[3]->SetName("lg_htrg_eff_l1_pt_trig_tgsf");
  htrg_eff_l1_pt_trig->SetLineColor(2);
  htrg_eff_l1_pt_tgsf->SetLineColor(4);
  htrg_eff_l1_pt_trig->SetMarkerColor(2);
  htrg_eff_l1_pt_tgsf->SetMarkerColor(4);
  lg[3]->AddEntry(htrg_eff_l1_pt_trig, "l_{1} pass trig.", "pl");
  lg[3]->AddEntry(htrg_eff_l1_pt_tgsf, "l_{1} trig. s.f.", "pl");

  plots->cd();
  plots->Clear();
  htrg_eff_l1_pt_trig->Draw();
  htrg_eff_l1_pt_tgsf->Draw("same");
  lg[3]->Draw("same");
  lumipt->Draw();
  sprintf(name,"%s", outputplot);
  plots->Print(name);
  plots->Clear();

  lg[4] = new TLegend(0.3,0.3,0.7,0.5);
  lg[4]->SetName("lg_htrg_eff_l2_pt_trig_tgsf");
  htrg_eff_l2_pt_trig->SetLineColor(2);
  htrg_eff_l2_pt_tgsf->SetLineColor(4);
  htrg_eff_l2_pt_trig->SetMarkerColor(2);
  htrg_eff_l2_pt_tgsf->SetMarkerColor(4);
  lg[4]->AddEntry(htrg_eff_l2_pt_trig, "l_{2} pass trig.", "pl");
  lg[4]->AddEntry(htrg_eff_l2_pt_tgsf, "l_{2} trig. s.f.", "pl");

  plots->cd();
  plots->Clear();
  htrg_eff_l2_pt_trig->Draw();
  htrg_eff_l2_pt_tgsf->Draw("same");
  lg[4]->Draw("same");
  lumipt->Draw();
  sprintf(name,"%s", outputplot);
  plots->Print(name);
  plots->Clear();


  // eta
  lg[5] = new TLegend(0.3,0.3,0.7,0.5);
  lg[5]->SetName("lg_htrg_eta_base_l1_l2");
  htrg_l1_eta_base->SetLineColor(2);
  htrg_l2_eta_base->SetLineColor(4);
  htrg_l1_eta_base->SetMarkerColor(2);
  htrg_l2_eta_base->SetMarkerColor(4);
  lg[5]->AddEntry(htrg_l1_eta_base, "l_{1} no trig.", "pl");
  lg[5]->AddEntry(htrg_l2_eta_base, "l_{2} no trig.", "pl");

  plots->cd();
  plots->Clear();
  htrg_l1_eta_base->Draw();
  htrg_l2_eta_base->Draw("same");
  lg[5]->Draw("same");
  lumipt->Draw();
  sprintf(name,"%s", outputplot);
  plots->Print(name);
  plots->Clear();


  lg[6] = new TLegend(0.3,0.3,0.7,0.5);
  lg[6]->SetName("lg_htrg_l1_eta_pass_tgsf");
  htrg_l1_eta_pass->SetLineColor(2);
  htrg_l1_eta_tgsf->SetLineColor(4);
  htrg_l1_eta_pass->SetMarkerColor(2);
  htrg_l1_eta_tgsf->SetMarkerColor(4);
  lg[6]->AddEntry(htrg_l1_eta_pass, "l_{1} pass trig.", "pl");
  lg[6]->AddEntry(htrg_l1_eta_tgsf, "l_{1} trig. s.f.", "pl");

  plots->cd();
  plots->Clear();
  htrg_l1_eta_pass->Draw();
  htrg_l1_eta_tgsf->Draw("same");
  lg[6]->Draw("same");
  lumipt->Draw();
  sprintf(name,"%s", outputplot);
  plots->Print(name);
  plots->Clear();

  lg[7] = new TLegend(0.3,0.3,0.7,0.5);
  lg[7]->SetName("lg_htrg_l2_eta_pass_tgsf");
  htrg_l2_eta_pass->SetLineColor(2);
  htrg_l2_eta_tgsf->SetLineColor(4);
  htrg_l2_eta_pass->SetMarkerColor(2);
  htrg_l2_eta_tgsf->SetMarkerColor(4);
  lg[7]->AddEntry(htrg_l2_eta_pass, "l_{2} pass trig.", "pl");
  lg[7]->AddEntry(htrg_l2_eta_tgsf, "l_{2} trig. s.f.", "pl");

  plots->cd();
  plots->Clear();
  htrg_l2_eta_pass->Draw();
  htrg_l2_eta_tgsf->Draw("same");
  lg[7]->Draw("same");
  lumipt->Draw();
  sprintf(name,"%s", outputplot);
  plots->Print(name);
  plots->Clear();
 
  lg[8] = new TLegend(0.3,0.3,0.7,0.5);
  lg[8]->SetName("lg_htrg_eff_l1_eta_trig_tgsf");
  htrg_eff_l1_eta_trig->SetLineColor(2);
  htrg_eff_l1_eta_tgsf->SetLineColor(4);
  htrg_eff_l1_eta_trig->SetMarkerColor(2);
  htrg_eff_l1_eta_tgsf->SetMarkerColor(4);
  lg[8]->AddEntry(htrg_eff_l1_eta_trig, "l_{1} pass trig.", "pl");
  lg[8]->AddEntry(htrg_eff_l1_eta_tgsf, "l_{1} trig. s.f.", "pl");

  plots->cd();
  plots->Clear();
  htrg_eff_l1_eta_trig->Draw();
  htrg_eff_l1_eta_tgsf->Draw("same");
  lg[8]->Draw("same");
  lumipt->Draw();
  sprintf(name,"%s", outputplot);
  plots->Print(name);
  plots->Clear();

  lg[9] = new TLegend(0.3,0.3,0.7,0.5);
  lg[9]->SetName("lg_htrg_eff_l2_eta_trig_tgsf");
  htrg_eff_l2_eta_trig->SetLineColor(2);
  htrg_eff_l2_eta_tgsf->SetLineColor(4);
  htrg_eff_l2_eta_trig->SetMarkerColor(2);
  htrg_eff_l2_eta_tgsf->SetMarkerColor(4);
  lg[9]->AddEntry(htrg_eff_l2_eta_trig, "l_{2} pass trig.", "pl");
  lg[9]->AddEntry(htrg_eff_l2_eta_tgsf, "l_{2} trig. s.f.", "pl");

  plots->cd();
  plots->Clear();
  htrg_eff_l2_eta_trig->Draw();
  htrg_eff_l2_eta_tgsf->Draw("same");
  lg[9]->Draw("same");
  lumipt->Draw();
  sprintf(name,"%s", outputplot);
  plots->Print(name);
  plots->Clear();

  plots->cd();
  plots->Clear();
  htrg_l1_pt_pass_vs_tgsf->Draw();
  lumipt->Draw();
  sprintf(name,"%s", outputplot);
  plots->Print(name);
  plots->Clear();
 
  plots->cd();
  plots->Clear();
  htrg_l2_pt_pass_vs_tgsf->Draw();
  lumipt->Draw();
  sprintf(name,"%s", outputplot);
  plots->Print(name);
  plots->Clear();
 
  plots->cd();
  plots->Clear();
  htrg_l1_eta_pass_vs_tgsf->Draw();
  lumipt->Draw();
  sprintf(name,"%s", outputplot);
  plots->Print(name);
  plots->Clear();
 
  plots->cd();
  plots->Clear();
  htrg_l2_eta_pass_vs_tgsf->Draw();
  lumipt->Draw();
  sprintf(name,"%s", outputplot);
  plots->Print(name);
  plots->Clear();
 
  plots->cd();
  plots->Clear();
  htrg_eff_l1_pt_trig_vs_tgsf->Draw();
  lumipt->Draw();
  sprintf(name,"%s", outputplot);
  plots->Print(name);
  plots->Clear();
 
  plots->cd();
  plots->Clear();
  htrg_eff_l2_pt_trig_vs_tgsf->Draw();
  lumipt->Draw();
  sprintf(name,"%s", outputplot);
  plots->Print(name);
  plots->Clear();
 
  plots->cd();
  plots->Clear();
  htrg_eff_l1_eta_trig_vs_tgsf->Draw();
  lumipt->Draw();
  sprintf(name,"%s", outputplot);
  plots->Print(name);
  plots->Clear();
 
  plots->cd();
  plots->Clear();
  htrg_eff_l2_eta_trig_vs_tgsf->Draw();
  lumipt->Draw();
  sprintf(name,"%s", outputplot);
  plots->Print(name);
  plots->Clear();



  // end
  sprintf(name, "%s]", outputplot);
  plots->Print(name);


}
