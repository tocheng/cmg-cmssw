{
  //TFile *_file0 = TFile::Open("/home/heli/XZZ/80X_20160810_light/SinglePhoton_Run2016BCD_PromptReco/vvTreeProducer/tree_hlt_flag3_f2.root");
  TFile *_file0 = TFile::Open("/home/heli/XZZ/80X_20160810_light/SinglePhoton_Run2016BCD_PromptReco/vvTreeProducer/tree_hlt_flag3_f2.root");
  gROOT->ProcessLine(".L printFilter.C");
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
  //tree->Draw("phi:eta>>h001(60,-3,3,80,-4,4)", "fabs(eta)<1.4&&gjet_l1_sigmaIetaIeta<0.00005", "colz");
  //TH2D* h001 = (TH2D*)gDirectory->Get("h001");
  //h001->GetZaxis()->SetRangeUser(15,30);
  //h001->Draw("colz");
  //printFilter(h001,15);

  //tree->Draw("phi:eta>>h068(60,-3,3,80,-4,4)", "(fabs(eta)<1.4&&pt>100&&gjet_l1_sigmaIetaIeta<0.00005)*pt", "colz");
  //TH2D* h = (TH2D*)gDirectory->Get("h068");
  //printFilter(h,300);

//  tree->SetAlias("f2", "(!(eta>=-2.4&&eta<=-2.2&&phi>=1.6&&phi<=1.8)&&!(eta>=-2.2&&eta<=-2&&phi>=-2.2&&phi<=-2)&&!(eta>=-2.2&&eta<=-2&&phi>=-1.6&&phi<=-1.4)&&!(eta>=-2.2&&eta<=-2&&phi>=-1.2&&phi<=-1)&&!(eta>=-2&&eta<=-1.8&&phi>=-2.4&&phi<=-2.2)&&!(eta>=-2&&eta<=-1.8&&phi>=-1.6&&phi<=-1.4)&&!(eta>=-0.4&&eta<=-0.2&&phi>=-2.6&&phi<=-2.4)&&!(eta>=-0.4&&eta<=-0.2&&phi>=2&&phi<=2.2)&&!(eta>=0.2&&eta<=0.4&&phi>=-1&&phi<=-0.8)&&!(eta>=0.2&&eta<=0.4&&phi>=1.4&&phi<=1.6)&&!(eta>=0.8&&eta<=1&&phi>=-2.4&&phi<=-2.2)&&!(eta>=0.8&&eta<=1&&phi>=-0.4&&phi<=-0.2)&&!(eta>=1.8&&eta<=2&&phi>=-2&&phi<=-1.8)&&!(eta>=1.8&&eta<=2&&phi>=-1.4&&phi<=-1.2)&&!(eta>=1.8&&eta<=2&&phi>=-1.2&&phi<=-1)&&!(eta>=1.8&&eta<=2&&phi>=1&&phi<=1.2)&&!(eta>=1.8&&eta<=2&&phi>=1.8&&phi<=2)&&!(eta>=1.8&&eta<=2&&phi>=2&&phi<=2.2)&&!(eta>=2&&eta<=2.2&&phi>=-1.4&&phi<=-1.2)&&!(eta>=2&&eta<=2.2&&phi>=-1.2&&phi<=-1)&&(1))");

  TFile *ff2 = new TFile("/home/heli/XZZ/80X_20160810_light/SinglePhoton_Run2016BCD_PromptReco/vvTreeProducer/tree_hlt_flag3_f2_sIetaCut.root", "recreate");
  TTree* tf2 = (TTree*)tree->CopyTree("gjet_l1_sigmaIetaIeta>0.005");

  tf2->Write();
  ff2->Close();


}
