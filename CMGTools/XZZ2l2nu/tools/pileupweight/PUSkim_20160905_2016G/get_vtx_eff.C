{
  TFile* file1 = TFile::Open("relval_vtx.root");
  TFile* file2 = TFile::Open("/home/heli/XZZ/80X_20160825_light_Skim/DYJetsToLL_M50_PUScan.root");

  std::string tag = "get_vtx_eff";

  gROOT->ProcessLine(".x tdrstyle.C");
  gStyle->SetPadTopMargin(0.10);
  gStyle->SetPadBottomMargin(0.2);
  gStyle->SetPadLeftMargin(0.2);
  gStyle->SetPadRightMargin(0.15);
  char name[1000];

  sprintf(name, "%s.root", tag.c_str());
  TFile* fout = TFile::Open(name, "recreate");

  TCanvas* plots = new TCanvas("plots", "plots", 400, 400);
  sprintf(name, "%s.pdf[", tag.c_str());  
  plots->Print(name);


  TTree* tree1 = (TTree*)file1->Get("tree");
  TTree* tree2 = (TTree*)file2->Get("tree");

  TH2D* h2d1 = new TH2D("h2d1", "h2d1", 100, 0,100, 100, 0, 100);
  TH2D* h2d2 = new TH2D("h2d2", "h2d2", 100, 0,100, 100, 0, 100);

  h2d1->Sumw2();
  h2d2->Sumw2();

  h2d1->GetXaxis()->SetTitle("n True PU");
  h2d2->GetXaxis()->SetTitle("n True PU");

  h2d1->GetYaxis()->SetTitle("n Reco. Vtx");
  h2d2->GetYaxis()->SetTitle("n Reco. Vtx");

  tree1->Draw("nVert:nTrueInt>>h2d1", "", "colz");
  tree2->Draw("nVert:nTrueInt>>h2d2", "", "colz");

 
  h2d1->FitSlicesY();
  h2d2->FitSlicesY();

  TH1D* h2d1_1 = (TH1D*)gDirectory->Get("h2d1_1");  
  TH1D* h2d2_1 = (TH1D*)gDirectory->Get("h2d2_1");  

  h2d1_1->GetXaxis()->SetTitle("n True PU");
  h2d2_1->GetXaxis()->SetTitle("n True PU");
  h2d1_1->GetYaxis()->SetTitle("mean n reco. vtx");
  h2d2_1->GetYaxis()->SetTitle("mean n reco. vtx");

  TF1* fc1 = new TF1("fc1", "[0]+[1]*x", 0, 100);
  TF1* fc2 = new TF1("fc2", "[0]+[1]*x", 0, 100);

  fc1->GetXaxis()->SetTitle("n True PU");
  fc2->GetXaxis()->SetTitle("n True PU");
  fc1->GetYaxis()->SetTitle("mean n reco. vtx");
  fc2->GetYaxis()->SetTitle("mean n reco. vtx");

  h2d1_1->Fit(fc1);
  h2d2_1->Fit(fc2);

  fc1->Print("v");
  fc2->Print("v");

  TF1* fcr = new TF1("fcr", "([0]+[1]*x)/([2]+[3]*x)", 0, 100);

  fcr->SetTitle("Eff. Scale Factor; n True PU; Eff. Ratio Relval / 80X MC");
  fcr->GetXaxis()->SetRangeUser(0,50);
  fcr->GetYaxis()->SetRangeUser(0,50);

  fcr->SetParameter(0, fc1->GetParameter(0));
  fcr->SetParameter(1, fc1->GetParameter(1));
  fcr->SetParameter(2, fc2->GetParameter(0));
  fcr->SetParameter(3, fc2->GetParameter(1));

  fcr->Print("v");

  h2d1->GetXaxis()->SetRangeUser(0,50);
  h2d2->GetXaxis()->SetRangeUser(0,50);
  h2d1->GetYaxis()->SetRangeUser(0,50);
  h2d2->GetYaxis()->SetRangeUser(0,50);
  h2d1_1->GetXaxis()->SetRangeUser(0,50);
  h2d2_1->GetXaxis()->SetRangeUser(0,50);
  h2d1_1->GetYaxis()->SetRangeUser(0,50);
  h2d2_1->GetYaxis()->SetRangeUser(0,50);
 
  plots->Clear();
  h2d1->Draw("colz");
  h2d1_1->Draw("same");
  sprintf(name, "%s.pdf", tag.c_str());
  plots->Print(name);
  plots->Clear();

  plots->Clear();
  h2d2->Draw("colz");
  h2d2_1->Draw("same");
  sprintf(name, "%s.pdf", tag.c_str());
  plots->Print(name);
  plots->Clear();


  plots->Clear();
  fcr->Draw("L");
  sprintf(name, "%s.pdf", tag.c_str());
  plots->Print(name);
  plots->Clear();

  
  sprintf(name, "%s.pdf]", tag.c_str());
  plots->Print(name);


  fout->Write();
 
}
