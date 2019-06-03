{

  TFile* file1 = TFile::Open("/home/heli/XZZ/80X_20161029_GJets_light_Skim/QCD_Pt20to30_EMEnriched.root");
  TFile* file2 = TFile::Open("/home/heli/XZZ/80X_20161029_GJets_light_Skim/QCD_Pt30to50_EMEnriched.root");
  TFile* file3 = TFile::Open("/home/heli/XZZ/80X_20161029_GJets_light_Skim/QCD_Pt50to80_EMEnriched.root");
  TFile* file4 = TFile::Open("/home/heli/XZZ/80X_20161029_GJets_light_Skim/QCD_Pt80to120_EMEnriched.root");
  TFile* file5 = TFile::Open("/home/heli/XZZ/80X_20161029_GJets_light_Skim/QCD_Pt120to170_EMEnriched.root");
  TFile* file6 = TFile::Open("/home/heli/XZZ/80X_20161029_GJets_light_Skim/QCD_Pt170to300_EMEnriched.root");
  TFile* file7 = TFile::Open("/home/heli/XZZ/80X_20161029_GJets_light_Skim/QCD_Pt300toInf_EMEnriched.root");

  TTree* tree1 = (TTree*)file1->Get("tree");
  TTree* tree2 = (TTree*)file2->Get("tree");
  TTree* tree3 = (TTree*)file3->Get("tree");
  TTree* tree4 = (TTree*)file4->Get("tree");
  TTree* tree5 = (TTree*)file5->Get("tree");
  TTree* tree6 = (TTree*)file6->Get("tree");
  TTree* tree7 = (TTree*)file7->Get("tree");


  TH1D* h000 = new TH1D("h000", "h000", 1000,0,1000);
  TH1D* h001 = new TH1D("h001", "h001", 1000,0,1000);
  TH1D* h002 = new TH1D("h002", "h002", 1000,0,1000);
  TH1D* h003 = new TH1D("h003", "h003", 1000,0,1000);
  TH1D* h004 = new TH1D("h004", "h004", 1000,0,1000);
  TH1D* h005 = new TH1D("h005", "h005", 1000,0,1000);
  TH1D* h006 = new TH1D("h006", "h006", 1000,0,1000);
  TH1D* h007 = new TH1D("h007", "h007", 1000,0,1000);

  tree1->Draw("llnunu_l1_pt>>h001", "(1)*(xsec/SumWeights)", "e");
  tree2->Draw("llnunu_l1_pt>>h002", "(1)*(xsec/SumWeights)", "e");
  tree3->Draw("llnunu_l1_pt>>h003", "(1)*(xsec/SumWeights)", "e");
  tree4->Draw("llnunu_l1_pt>>h004", "(1)*(xsec/SumWeights)", "e");
  tree5->Draw("llnunu_l1_pt>>h005", "(1)*(xsec/SumWeights)", "e");
  tree6->Draw("llnunu_l1_pt>>h006", "(1)*(xsec/SumWeights)", "e");
  tree7->Draw("llnunu_l1_pt>>h007", "(1)*(xsec/SumWeights)", "e");

  tree1->Draw("llnunu_l1_pt>>h000", "(1)*(xsec/SumWeights)", "e");
  tree2->Draw("llnunu_l1_pt>>+h000", "(1)*(xsec/SumWeights)", "e");
  tree3->Draw("llnunu_l1_pt>>+h000", "(1)*(xsec/SumWeights)", "e");
  tree4->Draw("llnunu_l1_pt>>+h000", "(1)*(xsec/SumWeights)", "e");
  tree5->Draw("llnunu_l1_pt>>+h000", "(1)*(xsec/SumWeights)", "e");
  tree6->Draw("llnunu_l1_pt>>+h000", "(1)*(xsec/SumWeights)", "e");
  tree7->Draw("llnunu_l1_pt>>+h000", "(1)*(xsec/SumWeights)", "e");

  h000->SetLineColor(1);
  h001->SetLineColor(2);
  h002->SetLineColor(3);
  h003->SetLineColor(4);
  h004->SetLineColor(5);
  h005->SetLineColor(6);
  h006->SetLineColor(7);
  h007->SetLineColor(8);
  h001->SetLineColor(9);


  h000->Draw();
  h001->Draw("same");
  h002->Draw("same");
  h003->Draw("same");
  h004->Draw("same");
  h005->Draw("same");
  h006->Draw("same");
  h007->Draw("same");

}
