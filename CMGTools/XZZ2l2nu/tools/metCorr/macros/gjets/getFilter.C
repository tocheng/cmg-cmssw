{
  //TFile *_file0 = TFile::Open("/home/heli/XZZ/80X_20160810_light/SinglePhoton_Run2016BCD_PromptReco/vvTreeProducer/tree_hlt_flag3.root");
  TFile *_file0 = TFile::Open("/home/heli/XZZ/80X_20160810_light/SinglePhoton_Run2016BCD_PromptReco/vvTreeProducer/tree.root");
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
  tree->Draw("phi:eta>>h001(30,-3,3,40,-4,4)", "ut<80&&pt>70&&metPara<-100", "colz");
  TH2D* h001 = (TH2D*)gDirectory->Get("h001");
  h001->GetZaxis()->SetRangeUser(21,30);
  h001->Draw("colz");
  printFilter(h001,20);

  //tree->SetAlias("metfilter", "(Flag_EcalDeadCellTriggerPrimitiveFilter&&Flag_HBHENoiseIsoFilter&&Flag_goodVertices&&Flag_HBHENoiseFilter&&Flag_globalTightHalo2016Filter&&Flag_eeBadScFilter)");
  tree->SetAlias("metfilter", "(Flag_EcalDeadCellTriggerPrimitiveFilter&&Flag_HBHENoiseIsoFilter&&Flag_goodVertices&&Flag_HBHENoiseFilter&&Flag_globalTightHalo2016Filter&&Flag_eeBadScFilter&&CSCTightHalo2015Filter)");

  tree->SetAlias("flag3", "((pt>=60&&!(eta>-2.5&&eta<-1.4&&phi>2.4&&phi<3.2)&&!(eta>-2.5&&eta<-1.4&&phi>-0.9&&phi<0.9)&&!(eta>-2.5&&eta<-1.4&&phi>-3.2&&phi<-2.4)&&!(eta>1.4&&eta<2.5&&phi>2.4&&phi<3.2)&&!(eta>1.4&&eta<2.5&&phi>-0.9&&phi<0.9)&&!(eta>1.4&&eta<2.5&&phi>-3.2&&phi<-2.0)&&!(eta>-2.1&&eta<-1.8&&phi>1.2&&phi<1.6)&&!(eta>-2.0&&eta<-1.6&&phi>-2.1&&phi<-1.8)&&!(eta>-0.3&&eta<0.0&&phi>2.5&&phi<3.0)&&!(eta>0.0&&eta<0.3&&phi>2.8&&phi<3.2)&&!(eta>-0.2&&eta<0.3&&phi>0.2&&phi<0.8)&&!(eta>-0.5&&eta<-0.2&&phi>-2.4&&phi<2.0)&&!(eta>2.3&&eta<2.5&&phi>-1.5&&phi<-1.0))||(pt<60&&!(eta>-2.5&&eta<-2.2&&phi>-3.0&&phi<-2.6)&&!(eta>0&&eta<0.3&&phi>-2.4&&phi<-1.4)&&!(eta>-0.2&&eta<0.2&&phi>-3.2&&phi<-2.6)&&!(eta>2.3&&eta<2.5&&phi>-2.6&&phi<-2.0)))");

  tree->SetAlias("f2", "(!(eta>=-2.4&&eta<=-2.2&&phi>=1.6&&phi<=1.8)&&!(eta>=-2.2&&eta<=-2&&phi>=-2.2&&phi<=-2)&&!(eta>=-2.2&&eta<=-2&&phi>=-1.6&&phi<=-1.4)&&!(eta>=-2.2&&eta<=-2&&phi>=-1.2&&phi<=-1)&&!(eta>=-2&&eta<=-1.8&&phi>=-2.4&&phi<=-2.2)&&!(eta>=-2&&eta<=-1.8&&phi>=-1.6&&phi<=-1.4)&&!(eta>=-0.4&&eta<=-0.2&&phi>=-2.6&&phi<=-2.4)&&!(eta>=-0.4&&eta<=-0.2&&phi>=2&&phi<=2.2)&&!(eta>=0.2&&eta<=0.4&&phi>=-1&&phi<=-0.8)&&!(eta>=0.2&&eta<=0.4&&phi>=1.4&&phi<=1.6)&&!(eta>=0.8&&eta<=1&&phi>=-2.4&&phi<=-2.2)&&!(eta>=0.8&&eta<=1&&phi>=-0.4&&phi<=-0.2)&&!(eta>=1.8&&eta<=2&&phi>=-2&&phi<=-1.8)&&!(eta>=1.8&&eta<=2&&phi>=-1.4&&phi<=-1.2)&&!(eta>=1.8&&eta<=2&&phi>=-1.2&&phi<=-1)&&!(eta>=1.8&&eta<=2&&phi>=1&&phi<=1.2)&&!(eta>=1.8&&eta<=2&&phi>=1.8&&phi<=2)&&!(eta>=1.8&&eta<=2&&phi>=2&&phi<=2.2)&&!(eta>=2&&eta<=2.2&&phi>=-1.4&&phi<=-1.2)&&!(eta>=2&&eta<=2.2&&phi>=-1.2&&phi<=-1))");

  TFile *fflag3 = new TFile("/home/heli/XZZ/80X_20160810_light/SinglePhoton_Run2016BCD_PromptReco/vvTreeProducer/tree_hlt_flag3.root", "recreate");
  TTree* tflag3 = (TTree*)tree->CopyTree("HLT_PHOTONIDISO&&ngjet==1&&flag3&&metfilter");
  tflag3->Write();
  


  TFile *ff2 = new TFile("/home/heli/XZZ/80X_20160810_light/SinglePhoton_Run2016BCD_PromptReco/vvTreeProducer/tree_hlt_flag3_f2.root", "recreate");
  TTree* tf2 = (TTree*)tflag3->CopyTree("f2");

  tf2->Write();
  ff2->Close();
  fflag3->Close();
  _file0->Close();

}
