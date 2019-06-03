{

  TFile* file1 = TFile::Open("/home/heli/XZZ/80X_20160825_light_Skim/DYJetsToLL_M50_BIG.root");
  //TFile* file2 = TFile::Open("/home/heli/XZZ/80X_20160810_light/GJet_Pt_20toInf_DoubleEMEnriched/vvTreeProducer/tree.root");
  TFile* file2 = TFile::Open("/home/heli/XZZ/80X_20160810_light/SinglePhoton_Run2016BCD_PromptReco/vvTreeProducer/tree.root");

  //std::string outtag="study_gjets_tight";
  //std::string outtag="study_gjets";
  std::string outtag="study_gjets_data";

  char name[1000];

  TCanvas* plots = new TCanvas("plots", "plots");

  sprintf(name, "%s.pdf[", outtag.c_str());
  plots->Print(name);

  TTree* tree1 = (TTree*)file1->Get("tree");
  TTree* tree2 = (TTree*)file2->Get("tree");

  bool useMzCut = false;

  // define cuts
  std::string metfilter="(Flag_EcalDeadCellTriggerPrimitiveFilter&&Flag_HBHENoiseIsoFilter&&Flag_goodVertices&&Flag_HBHENoiseFilter&&Flag_CSCTightHalo2015Filter&&Flag_eeBadScFilter)";
  std::string cuts_lepaccept="((abs(llnunu_l1_l1_pdgId)==13&&abs(llnunu_l1_l2_pdgId)==13&&llnunu_l1_l1_pt>50&&abs(llnunu_l1_l1_eta)<2.4&&llnunu_l1_l2_pt>20&&abs(llnunu_l1_l2_eta)<2.4&&(llnunu_l1_l1_highPtID==1||llnunu_l1_l2_highPtID==1))";
  cuts_lepaccept+="||(abs(llnunu_l1_l1_pdgId)==11&&abs(llnunu_l1_l2_pdgId)==11&&llnunu_l1_l1_pt>115&&abs(llnunu_l1_l1_eta)<2.5&&llnunu_l1_l2_pt>35&&abs(llnunu_l1_l2_eta)<2.5))";
  std::string cuts_lepaccept_lowlpt="((abs(llnunu_l1_l1_pdgId)==13&&abs(llnunu_l1_l2_pdgId)==13&&llnunu_l1_l1_pt>20&&abs(llnunu_l1_l1_eta)<2.4&&llnunu_l1_l2_pt>20&&abs(llnunu_l1_l2_eta)<2.4&&(llnunu_l1_l1_highPtID==1||llnunu_l1_l2_highPtID==1))";
  cuts_lepaccept_lowlpt+="||(abs(llnunu_l1_l1_pdgId)==11&&abs(llnunu_l1_l2_pdgId)==11&&llnunu_l1_l1_pt>20&&abs(llnunu_l1_l1_eta)<2.5&&llnunu_l1_l2_pt>20&&abs(llnunu_l1_l2_eta)<2.5))";
  std::string cuts_zmass="(llnunu_l1_mass>70&&llnunu_l1_mass<110)";
  std::string cuts_loose_z="("+metfilter+"&&"+cuts_lepaccept+"&&"+cuts_zmass+")";
  std::string cuts_loose_z_lowlpt="("+metfilter+"&&"+cuts_lepaccept_lowlpt+"&&"+cuts_zmass+")";


  std::string base_selec =  cuts_loose_z;
  //std::string base_selec =  cuts_loose_z_lowlpt;

  // add weight
  std::string weight_selec = std::string("*(ZJetsGenWeight*ZPtWeight*puWeight68075*1921.8*3)");
  //std::string weight_selec = std::string("*(ZJetsGenWeight*ZPtWeight*1921.8*3*12900.0)");
  // rho weight
  std::string rhoweight_selec = std::string("*(0.602*exp(-0.5*pow((rho-8.890)/6.187,2))+0.829*exp(-0.5*pow((rho-21.404)/10.866,2)))");
  // scale factors
  //std::string effsf_selec = std::string("*(trgsf*isosf*idsf*trksf)");
  std::string effsf_selec = std::string("*(trgsf*isosf*idsf*trksf)");

  // selec, cuts + weights
  std::string zjet_selec = base_selec + weight_selec + rhoweight_selec + effsf_selec;

  //std::string gjet_selec = metfilter + rhoweight_selec;
  std::string gjet_selec = metfilter;

  //Double_t ZPtBins[] = {0,1.25,2.5,3.75,5,6.25,7.5,8.75,10,11.25,12.5,15,17.5,20,25,30,35,40,45,50,60,70,80,90,100,110,130,150,170,190,220,250,400,1000};
  Double_t ZPtBins[] = {20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,65,70,75,80,85,90,95,100,105,110,120,130,140,150,160,170,180,190,200,220,240,260,300,400,500,1000};
  Int_t NZPtBins = sizeof(ZPtBins)/sizeof(ZPtBins[0]) - 1;
  Double_t ZMassBins[] = {50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,176,177,178,179,180};
  Int_t NZMassBins = sizeof(ZMassBins)/sizeof(ZMassBins[0]) - 1;
  //Double_t ZRapBins[] = {-3.0,-2.9,-2.8,-2.7,-2.6,-2.5,-2.4,-2.3,-2.2,-2.1,-2.0,-1.9,-1.8,-1.7,-1.6,-1.5,-1.4,-1.3,-1.2,-1.1,-1.0,-0.9,-0.8,-0.7,-0.6,-0.5,-0.4,-0.3,-0.2,-0.1,0.0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0,1.1,1.2,1.3,1.4,1.5,1.6,1.7,1.8,1.9,2.0,2.1,2.2,2.3,2.4,2.5,2.6,2.7,2.8,2.9,3.0};
  Double_t ZRapBins[] = {-2.5,-2.4,-2.3,-2.2,-2.1,-2.0,-1.9,-1.8,-1.7,-1.6,-1.5,-1.4,-1.3,-1.2,-1.1,-1.0,-0.9,-0.8,-0.7,-0.6,-0.5,-0.4,-0.3,-0.2,-0.1,0.0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0,1.1,1.2,1.3,1.4,1.5,1.6,1.7,1.8,1.9,2.0,2.1,2.2,2.3,2.4,2.5};
  //Double_t ZRapBins[] = {-2.5,-2.3,-2.1,-1.9,-1.7,-1.5,-1.3,-1.1,-0.9,-0.7,-0.5,-0.3,-0.1,0.1,0.3,0.5,0.7,0.9,1.1,1.3,1.5,1.7,1.9,2.1,2.3,2.5};
  Int_t NZRapBins = sizeof(ZRapBins)/sizeof(ZRapBins[0]) - 1;


  TH1D* hzptbb1 = new TH1D("hzptbb1", "hzptbb1", NZPtBins, ZPtBins);
  TH1D* hzptbb2 = new TH1D("hzptbb2", "hzptbb2", NZPtBins, ZPtBins);

  hzptbb1->Sumw2();
  hzptbb2->Sumw2();


  tree1->Draw("llnunu_l1_pt>>hzptbb1", zjet_selec.c_str());
  tree2->Draw("gjet_l1_pt>>hzptbb2",gjet_selec.c_str());

  hzptbb1->SetMarkerColor(2);
  hzptbb2->SetMarkerColor(4);
  hzptbb1->SetLineColor(2);
  hzptbb2->SetLineColor(4);

  //hzptbb1->Scale(1./hzptbb1->Integral(),"width");
  //hzptbb2->Scale(1./hzptbb2->Integral(),"width");

  TLegend* lgzptbb = new TLegend(0.5,0.6,0.8,0.8);
  lgzptbb->SetName("lgzptbb");
  lgzptbb->AddEntry(hzptbb1, "ZJets", "pl");
  lgzptbb->AddEntry(hzptbb2, "GJets", "pl");

  plots->Clear();

  hzptbb1->Draw();
  hzptbb2->Draw("same");
  lgzptbb->Draw();
  sprintf(name, "%s.pdf", outtag.c_str());
  plots->Print(name);
  plots->Clear();

  TH1D* hzptbbr12 = (TH1D*)hzptbb1->Clone("hzptbbr12");
  hzptbbr12->Divide(hzptbb2);


  // 3D ZMass

  TH3D* hzmass_zpt_zrap = new TH3D("hzmass_zpt_zrap", "hzmass_zpt_zrap", NZMassBins, ZMassBins, NZPtBins,ZPtBins,NZRapBins,ZRapBins);
  hzmass_zpt_zrap->Sumw2();
  tree1->Draw("llnunu_l1_rapidity:llnunu_l1_pt:llnunu_l1_mass>>hzmass_zpt_zrap", zjet_selec.c_str());


  // 2D zpt zrap
  TH2D* hzpt_zrap1 = new TH2D("hzpt_zrap1", "hzpt_zrap1", NZPtBins,ZPtBins,NZRapBins,ZRapBins);
  TH2D* hzpt_zrap2 = new TH2D("hzpt_zrap2", "hzpt_zrap2", NZPtBins,ZPtBins,NZRapBins,ZRapBins);

  hzpt_zrap1->Sumw2();
  hzpt_zrap2->Sumw2();


  tree1->Draw("llnunu_l1_rapidity:llnunu_l1_pt>>hzpt_zrap1", zjet_selec.c_str());
  tree2->Draw("gjet_l1_rapidity:gjet_l1_pt>>hzpt_zrap2", gjet_selec.c_str());

  //hzpt_zrap1->Scale(1.0/hzpt_zrap1->Integral(),"width");
  //hzpt_zrap2->Scale(1.0/hzpt_zrap2->Integral(),"width");


  TH2D* hzpt_zrap_r12 = (TH2D*)hzpt_zrap1->Clone("hzpt_zrap_r12");
  hzpt_zrap_r12->Divide(hzpt_zrap2);




  sprintf(name, "%s.root", outtag.c_str());
  TFile* fout = new TFile(name, "recreate");

  hzptbb1->Write("h_zpt_1");
  hzptbb2->Write("h_zpt_2");
  hzptbbr12->Write("h_zpt_ratio");

  //hzrap1->Write();
  //hzrap2->Write();
  //hmet1->Write();
  //hmet2->Write();


  hzmass_zpt_zrap->Write("h_zmass_zpt_zrap");

  hzpt_zrap1->Write("h_zpt_zrap_1");
  hzpt_zrap2->Write("h_zpt_zrap_2");

  hzpt_zrap_r12->Write("h_zpt_zrap_ratio");

  fout->Close();

  sprintf(name, "%s.pdf]", outtag.c_str());
  plots->Print(name);


}
