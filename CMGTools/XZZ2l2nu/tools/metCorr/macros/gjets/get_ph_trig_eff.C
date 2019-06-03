{

  gROOT->ProcessLine(".x tdrstyle.C");

  std::string outtag="get_ph_trig_eff_fullv2";
  TFile* file1 = TFile::Open("/data2/XZZ2/80X_20170202_GJets/SinglePhoton_Run2016Full_03Feb2017_v0/vvTreeProducer/tree.root");

  std::string lumiTag;
  TPaveText* lumipt;

  lumiTag = "CMS 13 TeV 2016 L=35.87 fb^{-1}";

  lumipt = new TPaveText(0.2,0.8,0.8,0.88,"brNDC");
  lumipt->SetBorderSize(0);
  lumipt->SetTextAlign(12);
  lumipt->SetFillStyle(0);
  lumipt->SetTextFont(42);
  lumipt->SetTextSize(0.03);
  lumipt->AddText(0.15,0.3, lumiTag.c_str());
 
  char name[1000];
  sprintf(name, "%s.root", outtag.c_str());
  TFile* fout = TFile::Open(name, "recreate");
  TTree* tree1 = (TTree*)file1->Get("tree");

  // alias
  tree1->SetAlias("metfilter", "(Flag_EcalDeadCellTriggerPrimitiveFilter&&Flag_HBHENoiseIsoFilter&&Flag_goodVertices&&Flag_HBHENoiseFilter&&Flag_globalTightHalo2016Filter&&Flag_eeBadScFilter&&Flag_BadPFMuonFilter&&Flag_BadChargedCandidateFilter&&Flag_noBadMuons)");
  tree1->SetAlias("ieta", "gjet_l1_ieta");
  tree1->SetAlias("iphi", "gjet_l1_iphi"); 
  tree1->SetAlias("flag3", "(((gjet_l1_iphi>75||gjet_l1_iphi<25)&&fabs(gjet_l1_eta)>1.5)||(fabs(gjet_l1_eta)<1.5))");
  tree1->SetAlias("filter1", "(gjet_l1_sigmaIetaIeta>0.001&&gjet_l1_sigmaIphiIphi>0.001&&gjet_l1_SwissCross<0.95&&gjet_l1_mipTotE<4.9&&gjet_l1_time>-2.58&&gjet_l1_time<1.42)");
  tree1->SetAlias("flg1eb", "(fabs(eta)<1.47&&!((ieta==56&&iphi==67)||(ieta==-51&&iphi==196)||(ieta==79&&iphi==67)||(ieta==72&&iphi==67)||(ieta==58&&iphi==74)||(ieta==-24&&iphi==119)||(ieta==49&&iphi==6)||(ieta==-21&&iphi==308)||(ieta==1&&iphi==81)||(ieta==-31&&iphi==163)||(ieta==5&&iphi==180)||(ieta==14&&iphi==321)||(ieta==5&&iphi==305)||(ieta==6&&iphi==140)||(ieta==-18&&iphi==189)||(ieta==-2&&iphi==244)||(ieta==-12&&iphi==196)||(ieta==-7&&iphi==39)||(ieta==-10&&iphi==163)||(ieta==-4&&iphi==229)||(ieta==-27&&iphi==41)||(ieta==-11&&iphi==163)||(ieta==-16&&iphi==170)||(ieta==-5&&iphi==183)||(ieta==13&&iphi==218)||(ieta==-4&&iphi==125)||(ieta==-4&&iphi==183)||(ieta==-84&&iphi==168)||(ieta==5&&iphi==307)||(ieta==-10&&iphi==245)||(ieta==26&&iphi==262)||(ieta==8&&iphi==329)||(ieta==4&&iphi==245)||(ieta==-25&&iphi==109)||(ieta==-11&&iphi==28)||(ieta==2&&iphi==237)||(ieta==-3&&iphi==187)||(ieta==-4&&iphi==144)||(ieta==3&&iphi==186)||(ieta==-3&&iphi==229)||(ieta==2&&iphi==246)||(ieta==-4&&iphi==57)||(ieta==-32&&iphi==146)||(ieta==-11&&iphi==182)||(ieta==5&&iphi==24)||(ieta==-19&&iphi==185)||(ieta==7&&iphi==214)||(ieta==-4&&iphi==53)||(ieta==2&&iphi==66)||(ieta==23&&iphi==147)))");
  tree1->SetAlias("flg1eep", "(eta>1.566)");  
  tree1->SetAlias("flg1eem", "(eta<-1.566)");

  tree1->SetAlias("hltfilter", "((gjet_l1_trigerob_HLTbit>>0&1&&gjet_l1_trigerob_pt<=30)||(gjet_l1_trigerob_HLTbit>>1&1&&gjet_l1_trigerob_pt<=36)||(gjet_l1_trigerob_HLTbit>>2&1&&gjet_l1_trigerob_pt<=50)||(gjet_l1_trigerob_HLTbit>>3&1&&gjet_l1_trigerob_pt<=75)||(gjet_l1_trigerob_HLTbit>>4&1&&gjet_l1_trigerob_pt<=90)||(gjet_l1_trigerob_HLTbit>>5&1&&gjet_l1_trigerob_pt<=120)||(gjet_l1_trigerob_HLTbit>>6&1&&gjet_l1_trigerob_pt<=165)||(gjet_l1_trigerob_HLTbit>>7&1&&gjet_l1_trigerob_pt<=10000000))");

  //tree1->SetAlias("selec1", "(ngjet==1&&Max$(jet_pt[]*jet_chargedEmEnergyFraction[])<10&&Max$(jet_pt[]*jet_muonEnergyFraction[])<10&&flag3&&nlep==0)");
  //tree1->SetAlias("selec2", "(metfilter&&ngjet==1&&Max$(jet_pt[]*jet_chargedEmEnergyFraction[])<10&&Max$(jet_pt[]*jet_muonEnergyFraction[])<10&&flag3&&filter1&&nlep==0)&&hltfilter");
  tree1->SetAlias("selec1", "(metfilter&&ngjet==1&&Max$(jet_pt[]*jet_chargedEmEnergyFraction[])<10&&Max$(jet_pt[]*jet_muonEnergyFraction[])<10&&flag3&&nlep==0)");
  tree1->SetAlias("selec2", "(metfilter&&ngjet==1&&Max$(jet_pt[]*jet_chargedEmEnergyFraction[])<10&&Max$(jet_pt[]*jet_muonEnergyFraction[])<10&&flag3&&filter1&&nlep==0)&&HLT_PHOTONIDISO");


  TCanvas* plots = new TCanvas("plots", "plots");

  sprintf(name, "%s.pdf[", outtag.c_str());
  plots->Print(name);

  
  Double_t ZPtBins[] = {0,30,36,50,75,90,120,165,3000};
  Int_t NZPtBins = sizeof(ZPtBins)/sizeof(ZPtBins[0]) - 1;
  Double_t EtaBins[] = {0,0.5,0.7,0.9,1.1,1.3,1.5,1.7,1.9,2.1,2.3,2.5};

  Int_t NEtaBins = sizeof(EtaBins)/sizeof(EtaBins[0]) - 1; 

  TH2D* h_eta_pt_1 = new TH2D("h_eta_pt_1", "h_eta_pt_1", NZPtBins, ZPtBins, NEtaBins, EtaBins);
  TH2D* h_eta_pt_2 = new TH2D("h_eta_pt_2", "h_eta_pt_2", NZPtBins, ZPtBins, NEtaBins, EtaBins);

  h_eta_pt_1->Sumw2();
  h_eta_pt_2->Sumw2();

  tree1->Draw("fabs(gjet_l1_eta):gjet_l1_pt>>h_eta_pt_1", "selec1");
  tree1->Draw("fabs(gjet_l1_eta):gjet_l1_pt>>h_eta_pt_2", "selec2");

  TH2D* h_eta_pt_r21 = (TH2D*)h_eta_pt_2->Clone("h_eta_pt_r21");
  h_eta_pt_r21->Divide(h_eta_pt_1);

  sprintf(name, "%s.pdf", outtag.c_str());
  plots->Clear();
  
  plots->SetLogx(1);
  h_eta_pt_1->Draw("colz text");
  lumipt->Draw();
  plots->Print(name);
  plots->SetLogx(0);
  plots->Clear();

  plots->SetLogx(1);
  h_eta_pt_2->Draw("colz text");
  lumipt->Draw();
  plots->Print(name);
  plots->SetLogx(0);
  plots->Clear();

  plots->SetLogx(1);
  h_eta_pt_r21->Draw("colz text");
  lumipt->Draw();
  plots->Print(name);
  plots->SetLogx(0);
  plots->Clear();

  //
  TH1D* h_pt_1_sb = new TH1D("h_pt_1_sb", "h_pt_1_sb", 60, 0, 3000);
  TH1D* h_pt_2_sb = new TH1D("h_pt_2_sb", "h_pt_2_sb", 60, 0, 3000);
  h_pt_1_sb->Sumw2();
  h_pt_2_sb->Sumw2();

  tree1->Draw("gjet_l1_pt>>h_pt_1_sb", "selec1");
  tree1->Draw("gjet_l1_pt>>h_pt_2_sb", "selec2");

  TH1D* h_pt_r21_sb = (TH1D*)h_pt_2_sb->Clone("h_pt_r21_sb");
  h_pt_r21_sb->Divide(h_pt_1_sb);

  h_pt_1_sb->SetLineColor(2);
  h_pt_2_sb->SetLineColor(4);
  h_pt_1_sb->SetMarkerColor(2);
  h_pt_2_sb->SetMarkerColor(4);
 
  h_pt_1_sb->SetMarkerStyle(20);
  h_pt_2_sb->SetMarkerStyle(20);
 
  h_pt_r21_sb->SetMarkerStyle(20);

  plots->Clear();
  h_pt_1_sb->Draw();
  h_pt_2_sb->Draw("same");
  lumipt->Draw();
  plots->Print(name);

  plots->Clear();
  h_pt_r21_sb->Draw();
  lumipt->Draw();
  plots->Print(name);



  TProfile* pr_pt_1 = (TProfile*)h_eta_pt_1->ProfileX("pr_pt_1");
  TProfile* pr_pt_2 = (TProfile*)h_eta_pt_2->ProfileX("pr_pt_2");

  TH1D* h_pt_1 = (TH1D*)h_eta_pt_1->ProjectionX("h_pt_1");
  TH1D* h_pt_2 = (TH1D*)h_eta_pt_2->ProjectionX("h_pt_2");

  h_pt_1->SetTitle("h_pt_1");
  h_pt_2->SetTitle("h_pt_2");

  TH1D* h_pt_r21 = (TH1D*)h_pt_2->Clone("h_pt_r21");
  h_pt_r21->Divide(h_pt_1);

  
  h_pt_1->SetLineColor(2);
  h_pt_2->SetLineColor(4);
  h_pt_1->SetMarkerColor(2);
  h_pt_2->SetMarkerColor(4);

  h_pt_1->SetMarkerStyle(20);
  h_pt_2->SetMarkerStyle(20);

  h_pt_r21->SetMarkerStyle(20);

  plots->Clear();
  h_pt_1->Draw();
  h_pt_2->Draw("same");
  lumipt->Draw();
  plots->Print(name);

  plots->Clear();
  h_pt_r21->Draw();
  lumipt->Draw();
  plots->Print(name);


  TH2D* h_eta_pt_1_ptnorm = (TH2D*)h_eta_pt_1->Clone("h_eta_pt_1_ptnorm");
  TH2D* h_eta_pt_2_ptnorm = (TH2D*)h_eta_pt_2->Clone("h_eta_pt_2_ptnorm");

  h_eta_pt_1_ptnorm->SetTitle("h_eta_pt_1_ptnorm");
  h_eta_pt_2_ptnorm->SetTitle("h_eta_pt_2_ptnorm");

  for (int ipt=0; ipt<NZPtBins; ipt++){
    double npt1 = h_pt_1->GetBinContent(ipt+1);
    double npt2 = h_pt_2->GetBinContent(ipt+1);
 
    for (int ieta=0; ieta<NEtaBins; ieta++){
      h_eta_pt_1_ptnorm->SetBinContent(ipt+1, ieta+1, h_eta_pt_1_ptnorm->GetBinContent(ipt+1, ieta+1)/npt1);
      h_eta_pt_1_ptnorm->SetBinError(ipt+1, ieta+1, h_eta_pt_1_ptnorm->GetBinError(ipt+1, ieta+1)/npt1);
      h_eta_pt_2_ptnorm->SetBinContent(ipt+1, ieta+1, h_eta_pt_2_ptnorm->GetBinContent(ipt+1, ieta+1)/npt2);
      h_eta_pt_2_ptnorm->SetBinError(ipt+1, ieta+1, h_eta_pt_2_ptnorm->GetBinError(ipt+1, ieta+1)/npt2);
    }

  }

  h_eta_pt_weight = (TH2D*)h_eta_pt_2_ptnorm->Clone("h_eta_pt_weight");
  h_eta_pt_weight->Divide(h_eta_pt_1_ptnorm);
  h_eta_pt_weight->SetTitle("h_eta_pt_weight");

  
  plots->Clear();
  plots->SetLogx(1);
  h_eta_pt_1_ptnorm->Draw("colz text");
  lumipt->Draw();
  plots->Print(name);
  plots->SetLogx(0);

  plots->Clear();
  plots->SetLogx(1);
  h_eta_pt_2_ptnorm->Draw("colz text");
  lumipt->Draw();
  plots->Print(name);
  plots->SetLogx(0);
 
  plots->Clear();
  plots->SetLogx(1);
  h_eta_pt_weight->Draw("colz text");
  lumipt->Draw();
  plots->Print(name);
  plots->SetLogx(0);

//
  std::vector<std::string> hlt_lab={"HLT 22", "HLT 30", "HLT 36", "HLT 50", "HLT 75", "HLT 90", "HLT 120", "HLT 165"};

  for (int i=0; i<h_eta_pt_1_ptnorm->GetNbinsX(); i++){
    sprintf(name, "tmp_h1_%i", i);
    TH1D* h1 = (TH1D*)h_eta_pt_1_ptnorm->ProjectionY(name, i+1,i+1);
    sprintf(name, "tmp_h2_%i", i);
    TH1D* h2 = (TH1D*)h_eta_pt_2_ptnorm->ProjectionY(name, i+1,i+1);
    h1->SetLineColor(2);
    h1->SetMarkerColor(2);
    h2->SetLineColor(4);
    h2->SetMarkerColor(4);
    h1->SetMarkerStyle(20);
    h2->SetMarkerStyle(20);
    h1->GetXaxis()->SetTitle("Photon P_{T} (GeV)");
    h2->GetXaxis()->SetTitle("Photon P_{T} (GeV)");
    TLegend* lg1 = new TLegend(0.6,0.7, 0.8,0.8);
    sprintf(name, "tmp_lg1_%i", i);
    lg1->AddEntry(h1, "pass trigger", "apl");
    lg1->AddEntry(h2, "all data" , "apl");
    lg1->AddEntry(h2, hlt_lab.at(i).c_str() , "");

    sprintf(name, "tmp_h3_%i", i);
    TH1D* h3 = (TH1D*)h2->Clone(name);
    h3->Divide(h1);
    h3->SetMarkerColor(1);
    h3->SetLineColor(1);
    h3->GetYaxis()->SetTitle("Trigger Eff.");

    plots->Clear();
    plots->SetLogy(1);
    h2->Draw();
    h1->Draw("same");
    lumipt->Draw();
    sprintf(name, "%s.pdf", outtag.c_str());
    plots->Print(name);
    plots->SetLogy(0);
    plots->Print(name);
    plots->Clear();
    
    h3->Draw();
    plots->Print(name);
    plots->Clear();    

  }

  sprintf(name, "%s.pdf]", outtag.c_str());
  plots->Print(name);
  // save

  fout->cd();

  h_eta_pt_1->Write();
  h_eta_pt_2->Write();
  h_eta_pt_r21->Write();

  pr_pt_1->Write();
  pr_pt_2->Write();

  h_pt_1->Write();
  h_pt_2->Write();
  
  h_eta_pt_1_ptnorm->Write();
  h_eta_pt_2_ptnorm->Write();

  h_eta_pt_weight->Write();

  h_pt_1_sb->Write();
  h_pt_2_sb->Write();
  h_pt_r21_sb->Write();

  fout->Close();

}
