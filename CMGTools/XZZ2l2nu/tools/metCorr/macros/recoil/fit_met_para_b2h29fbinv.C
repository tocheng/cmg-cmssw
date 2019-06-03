

std::string channel = "all";
bool doMC = false;
bool doGJets = false;
bool useMzCut = false;
bool useZSelec = false;
bool useZSelecLowLPt = false;
bool useEffSf = false;
bool mcTrgSf = false;
bool dtTrgSf = false;
bool dtHLT = false;

// recipe:
// 1.) useZSelecLowLPt can well reproduce the results with full cuts (useZSelec) + HLT (dtHLT or dtTrgSf or mcTrgSf) 
// 2.) for Data:  doMC=false, dGJets=false, useZSelecLowLPt=true, useEffSf=false
// 3.) for MC : doMC=false, doGJets=false, useZSelecLowLPt=true, useEffSf=false
// 4.) for GJets: doGJets=true, doMC=false, useZSelecLowLPt=true, useEffSf=false

std::string inputdir = "/home/heli/XZZ/80X_20161018_light_Skim";
//std::string inputdir = "/home/heli/XZZ/80X_20161006_light_Skim";
//std::string inputdir = "/home/heli/XZZ/80X_20160810_light_Skim";
//std::string inputdir = "/home/heli/XZZ/80X_20160825_light_Skim";
//std::string inputdir = "/home/heli/XZZ/80X_20160927_light_Skim";
std::string filename;

//std::string outputdir = "./recoil_out2";
std::string outputdir = "./recoil_out4";
std::vector< std::string > channels = {"all", "mu", "el"};
std::vector< std::string > mcfiles = {
    //"DYJetsToLL_M50", "DYJetsToLL_M50_MGMLM_Ext1"
    "DYJetsToLL_M50_BIG_NoRecoil", 
    //"DYJetsToLL_M50_NoRecoil", "DYJetsToLL_M50_MGMLM_Ext1_NoRecoil"
    //"DYJetsToLL_M50_RecoilSmooth", "DYJetsToLL_M50_MGMLM_Ext1_RecoilSmooth"
    //"DYJetsToLL_M50_RecoilNoSmooth", "DYJetsToLL_M50_MGMLM_Ext1_RecoilNoSmooth"
 };

std::vector< std::string > dtfiles = {
    //"SingleEMU_Run2016BCD_PromptReco"
    //"SingleEMU_Run2016B2G_PromptReco"
    "SingleEMU_Run2016B2H29fbinv_PromptReco"
 };

std::vector< std::string > gjfiles = {
    "SinglePhoton_Run2016B2H29fbinv_PromptReco_newFilterLepVetoNoRecoil"
    //"SinglePhoton_Run2016B2G_PromptReco_newFilterLepVetoNoRecoil"
    //"SinglePhoton_Run2016B2G_PromptReco_RcDataB2GNewFilterLepVetol"
    //"SinglePhoton_Run2016B2G_PromptReco_newFilterLepVetoNoRecoil"
    //"SinglePhoton_Run2016BCD_PromptReco_newFilterLepVetoNoRecoil"
    //"SinglePhoton_Run2016BCD_PromptReco_newFilterEtaPhiCutNoRecoil"
    //"SinglePhoton_Run2016BCD_PromptReco_newFilterNoRecoil"
    //"SinglePhoton_Run2016BCD_PromptReco_HLTFlag3F2SiEtaNoRecoil"
    //"SinglePhoton_Run2016BCD_PromptReco_HLT_DtScale_Flag2"
    //"SinglePhoton_Run2016BCD_PromptReco_HLTNoRecoil"
    //"SinglePhoton_Run2016BCD_PromptReco_HLT_DtScale_RcSmBin"
    //"SinglePhoton_Run2016BCD_PromptReco_HLT_DtScale"
    //"SinglePhoton_Run2016BCD_PromptReco_HLTNo90NoRecoil"
    //"SinglePhoton_Run2016BCD_PromptReco_HLTNo90_DtScale"
    //"SinglePhoton_Run2016BCD_PromptReco_NoRecoil"
    //"SinglePhoton_Run2016BCD_PromptReco_RcSmBin"
 };


char name[1000];
TCanvas* plots;
std::string tag0 = "_DtB2H29fbinv";
//std::string tag0 = "_NoRhoWt";
//std::string tag0 = "";
//std::string tag0 = "_newfilterlepveto";
//std::string tag0 = "_newfilteretaphicut";
//std::string tag0 = "_newfilter";
//std::string tag0 = "_smbin";
//std::string tag0 = "_smbin_id3";
//std::string tag0 = "_smbin_id2";
//std::string tag0 = "_hlt";
//std::string tag0 = "_hltno75";
//std::string tag0 = "_hlt_phvto";
//std::string tag0 = "_hlt_flag2";
//std::string tag0 = "_hlt_flag2_smbin";
//std::string tag0 = "_hlt_flag3_f2_sIetaCut";
//std::string tag0 = "_smbin_hlt_phvto_id3";
//std::string tag0 = "_hltno90";
//std::string tag0 = "_hlt_id2";
//std::string tag0 = "_hlt_id3";
//std::string tag0 = "_smbin_hlt";
std::string base_selec;
std::string lumiTag;

std::vector<Double_t> fit_min; 
std::vector<Double_t> fit_max; 
std::vector<Int_t> fit_rebin; 


int fit_slice_gaus(TH2D* h2d, TH1D** h1d, std::string& plotfile);
void do_fit_met_para(std::string& infilename, std::string& chan);

std::string base_sele;

TFile* fin;
TFile* fout;

std::string histname;
TPaveText* lumipt;
TPaveText* pvtxt;
TH2D* h2d1;
TH2D* h2d2;
TH2D* h2d3;
TH2D* h2d4;
TH1D* h1d1[1000];
TH1D* h1d2[1000];
TH1D* h1d3[1000];
TH1D* h1d4[1000];
TF1* func1[1000];
TF1* func2[1000];
TF1* func3[1000];
TF1* func4[1000];

Int_t Nbins;

void fit_met_para(){

 if (doMC) 
 {
   for (int i=0; i<(int)mcfiles.size(); i++)
   {   
     for (int j=0; j<(int)channels.size(); j++){
       do_fit_met_para(mcfiles.at(i), channels.at(j));
     }
   }
 }
 else if (!doMC && !doGJets)  
 {
   for (int i=0; i<(int)dtfiles.size(); i++)
   {
     for (int j=0; j<(int)channels.size(); j++){
       do_fit_met_para(dtfiles.at(i), channels.at(j));
     }
   }
 }
 else if (!doMC && doGJets)
 {
   for (int i=0; i<(int)gjfiles.size(); i++)
   {
     for (int j=0; j<(int)channels.size(); j++){
       do_fit_met_para(gjfiles.at(i), channels.at(j));
     }
   }  
 }
}

void do_fit_met_para(std::string& infilename, std::string& chan) {

  filename = infilename;
  channel = chan;

  // tags
  std::string tag = tag0+"_met_para_study";
  if (useMzCut) tag += "_MzCut";
  if (useZSelec) tag += "_ZSelec"; 
  if (useZSelecLowLPt) tag += "_ZSelecLowLPt";
  if (doMC && useEffSf) tag += "_effSf";
  if ( (doMC && mcTrgSf) || (!doMC && dtTrgSf))  tag += "_trgSf";
  if (!doMC && dtHLT) tag += "_dtHLT";
  if (channel=="el") tag += "_el";
  else if (channel=="mu") tag += "_mu";


  // define cuts
  std::string metfilter="(Flag_EcalDeadCellTriggerPrimitiveFilter&&Flag_HBHENoiseIsoFilter&&Flag_goodVertices&&Flag_HBHENoiseFilter&&Flag_globalTightHalo2016Filter&&Flag_eeBadScFilter)";
  std::string cuts_lepaccept="((abs(llnunu_l1_l1_pdgId)==13&&abs(llnunu_l1_l2_pdgId)==13&&llnunu_l1_l1_pt>50&&abs(llnunu_l1_l1_eta)<2.4&&llnunu_l1_l2_pt>20&&abs(llnunu_l1_l2_eta)<2.4&&(llnunu_l1_l1_highPtID==1||llnunu_l1_l2_highPtID==1))";
  cuts_lepaccept+="||(abs(llnunu_l1_l1_pdgId)==11&&abs(llnunu_l1_l2_pdgId)==11&&llnunu_l1_l1_pt>115&&abs(llnunu_l1_l1_eta)<2.5&&llnunu_l1_l2_pt>35&&abs(llnunu_l1_l2_eta)<2.5))";
  std::string cuts_lepaccept_lowlpt="((abs(llnunu_l1_l1_pdgId)==13&&abs(llnunu_l1_l2_pdgId)==13&&llnunu_l1_l1_pt>20&&abs(llnunu_l1_l1_eta)<2.4&&llnunu_l1_l2_pt>20&&abs(llnunu_l1_l2_eta)<2.4&&(llnunu_l1_l1_highPtID==1||llnunu_l1_l2_highPtID==1))";
  cuts_lepaccept_lowlpt+="||(abs(llnunu_l1_l1_pdgId)==11&&abs(llnunu_l1_l2_pdgId)==11&&llnunu_l1_l1_pt>20&&abs(llnunu_l1_l1_eta)<2.5&&llnunu_l1_l2_pt>20&&abs(llnunu_l1_l2_eta)<2.5))";
  std::string cuts_zmass="(llnunu_l1_mass>70&&llnunu_l1_mass<110)";
  std::string cuts_loose_z="("+metfilter+"&&"+cuts_lepaccept+"&&"+cuts_zmass+")";
  std::string cuts_loose_z_lowlpt="("+metfilter+"&&"+cuts_lepaccept_lowlpt+"&&"+cuts_zmass+")";

  base_selec = "(llnunu_l1_mass>50&&llnunu_l1_mass<180)";
  if (useMzCut)  base_selec = "(llnunu_l1_mass>70&&llnunu_l1_mass<110)";

  if (useZSelec) base_selec =  cuts_loose_z;
  if (useZSelecLowLPt) base_selec =  cuts_loose_z_lowlpt;

  if (channel=="el") base_selec = "("+base_selec+"&&(abs(llnunu_l1_l1_pdgId)==11&&abs(llnunu_l1_l2_pdgId)==11))";
  else if (channel=="mu") base_selec = "("+base_selec+"&&(abs(llnunu_l1_l1_pdgId)==13&&abs(llnunu_l1_l2_pdgId)==13))";

  if (!doMC && dtHLT) base_selec = "((HLT_MUv2||HLT_ELEv2)&&"+base_selec+")";


  // add weight
  std::string weight_selec = std::string("*(genWeight*ZPtWeight*puWeight68075/SumWeights*1921.8*3*12900.0)");
  // rho weight
  //std::string rhoweight_selec = std::string("*(0.602*exp(-0.5*pow((rho-8.890)/6.187,2))+0.829*exp(-0.5*pow((rho-21.404)/10.866,2)))");
  //std::string rhoweight_selec = std::string("*(0.232+0.064*rho)");
  std::string rhoweight_selec = std::string("*(0.038+0.118*rho-4.329e-03*rho*rho+1.011e-04*rho*rho*rho)");
  // scale factors
  std::string effsf_selec = std::string("*(isosf*idsf*trksf)");

  // selec, cuts + weights
  std::string selec = base_selec;
  if (doMC) selec +=  weight_selec + rhoweight_selec;
  //if (doMC) selec +=  weight_selec;
  if (doMC && useEffSf) selec += effsf_selec;
  if ( (doMC && mcTrgSf) || (!doMC && dtTrgSf) ) selec += "*(trgsf)";
  
  if (doGJets) {
    //base_selec = "("+metfilter+"&&HLT_PHOTONHZZ)";
    //base_selec = "("+metfilter+"&&gjet_l1_idCutBased>=2)";
    //base_selec = "("+metfilter+"&&gjet_l1_idCutBased>=3)";
    //base_selec = "("+metfilter+"&&HLT_PHOTONHZZ&&gjet_l1_idCutBased>=2)";
    //base_selec = "("+metfilter+"&&HLT_PHOTONHZZ&&gjet_l1_idCutBased>=3)";
    //base_selec = "("+metfilter+"&&(HLT_PHOTONIDISO&&!HLT_PHOTONIDISO75))";
    //base_selec = "("+metfilter+"&&(HLT_PHOTONIDISO&&!HLT_PHOTONIDISO90))";
    //base_selec = "("+metfilter+"&&HLT_PHOTONIDISO)";
    //base_selec = "("+metfilter+"&&HLT_PHOTONIDISO&&!(absDeltaPhi>3.0&&metPara/llnunu_l1_pt>-1.5&&metPara/llnunu_l1_pt<-0.5))";
    //base_selec = "("+metfilter+"&&HLT_PHOTONIDISO&&!(metPara/llnunu_l1_pt<-0.8&&metPara/llnunu_l1_pt>-1.8&&fabs(metPerp/llnunu_l1_pt)<0.3))";
    //base_selec = "("+metfilter+"&&HLT_PHOTONIDISO&&fabs(llnunu_l1_eta)<1.47)";
    //base_selec = "("+metfilter+"&&HLT_PHOTONIDISO&&fabs(llnunu_l1_eta)<1.47&&gjet_l1_idCutBased>=3)";
    //base_selec = "("+metfilter+"&&HLT_PHOTONIDISO&&flag2)";
    //base_selec = "(phi>-1&&phi<2&&fabs(eta)<1.0)";
    base_selec = "(1)";
    //if (channel=="el")  selec = base_selec+"*(GJetsZPtWeightLowLPtEl)";
    //else if (channel=="mu") selec = base_selec+"*(GJetsZPtWeightLowLPtMu)";
    //else  selec = base_selec+"*(GJetsZPtWeightLowLPt)";
    if (channel=="el")  selec = base_selec+"*(GJetsWeightLowLPtEl)";
    else if (channel=="mu") selec = base_selec+"*(GJetsWeightLowLPtMu)";
    else  selec = base_selec+"*(GJetsWeightLowLPt)";

    selec = selec + "*(GJetsRhoWeight)";
  }
  // style
  gROOT->ProcessLine(".x tdrstyle.C");
  gStyle->SetOptTitle(0);

  sprintf(name, ".! mkdir -p %s", outputdir.c_str());
  gROOT->ProcessLine(name);

  // lumiTag for plotting
  lumiTag = "CMS 13 TeV 2016 L=29.53 fb^{-1}";
  //lumiTag = "CMS 13 TeV 2016 L=27.22 fb^{-1}";
  //lumiTag = "CMS 13 TeV 2016 L=12.9 fb^{-1}";
  if (doMC) lumiTag = "CMS 13 TeV Simulation for 2016 Data";

  lumipt = new TPaveText(0.2,0.9,0.8,0.98,"brNDC");
  lumipt->SetBorderSize(0);
  lumipt->SetTextAlign(12);
  lumipt->SetFillStyle(0);
  lumipt->SetTextFont(42);
  lumipt->SetTextSize(0.03);
  lumipt->AddText(0.15,0.3, lumiTag.c_str());

  pvtxt = new TPaveText(0.6,0.8,0.9,0.9,"brNDC");
  pvtxt->SetBorderSize(0);
  pvtxt->SetTextAlign(12);
  pvtxt->SetFillStyle(0);
  pvtxt->SetTextFont(42);
  pvtxt->SetTextSize(0.03);

  sprintf(name, "%s/%s.root", inputdir.c_str(), filename.c_str());
  fin = new TFile(name);


  sprintf(name, "%s/%s%s.root", outputdir.c_str(), filename.c_str(), tag.c_str());
  fout = new TFile(name, "recreate");

  TTree* tree = (TTree*)fin->Get("tree");

  tree->SetAlias("absDeltaPhi", "fabs(TVector2::Phi_mpi_pi(llnunu_l2_phi-llnunu_l1_phi))");
  tree->SetAlias("metPara", "llnunu_l2_pt*cos(llnunu_l2_phi-llnunu_l1_phi)");
  tree->SetAlias("metPerp", "llnunu_l2_pt*sin(llnunu_l2_phi-llnunu_l1_phi)");
  tree->SetAlias("eta", "llnunu_l1_eta");
  tree->SetAlias("phi", "llnunu_l1_phi");
  tree->SetAlias("flag2", "(!(eta>0&&eta<0.15&&phi>0.52&&phi<0.56)&&!(eta>-2.5&&eta<-1.4&&phi>-0.5&&phi<0.5)&&!(eta>1.5&&eta<2.5&&phi>-0.5&&phi<0.5)&&!(eta>1.4&&eta<1.6&&phi>-0.8&&phi<-0.5)&&!(eta>1.4&&eta<2.5&&phi>2.5&&phi<4)&&!(eta>1.4&&eta<2.5&&phi>-4&&phi<-2.5)&&!(eta>-2.5&&eta<-1.4&&phi>2.5&&phi<4)&&!(eta>-2.5&&eta<-1.4&&phi>-4&&phi<-2.5)&&!(eta>2.3&&eta<2.6&&phi>-2.5&&phi<-2.2)&&!(eta>0.2&&eta<0.3&&phi>-2.6&&phi<-2.5)&&!(eta>0.5&&eta<0.7&&phi>-1.5&&phi<-1.2)&&!(eta>-0.85&&eta<-0.7&&phi>-1.8&&phi<-1.4)&&!(eta<-2.4&&eta>-2.5&&phi<-1.75&&phi>-1.9)&&!(eta>-2.5&&eta<-2.4&&phi>-1.2&&phi<-1.1)&&!(eta>-2.4&&eta<-2.3&&phi>-2.4&&phi<-2.3))");

  plots = new TCanvas("plots", "plots");

  sprintf(name, "%s/%s%s.pdf[", outputdir.c_str(),filename.c_str(), tag.c_str());
  plots->Print(name);


  // other control plots
  Double_t ZPtBins[] = {0,2,4,6,8,10,12,14,16,18,20,22,24,26,28, 30, 35, 40, 50, 60, 70, 80, 90, 100, 110, 120, 130, 140, 150, 160, 170, 180, 190, 200, 220, 240, 260, 300, 500, 5000 };
  //Double_t ZPtBins[] = {0,2,4,6,8,10,12,14,16,18,20,22,24,26,28, 30, 35, 40, 50, 60, 80, 100, 150, 250, 5000 };
  //Double_t ZPtBins[] = {0,2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32,34,35,36,36.5,37,37.5,38,38.5,39,39.5,40,41,42,43,44,45,46,47,48,49,50,55,60,65,70,75,80,85,90,95,100,105, 110,120,125, 130,140,150,160,180,200,300,5000 };
  Int_t NZPtBins = sizeof(ZPtBins)/sizeof(ZPtBins[0]) - 1;
  const Int_t NMetParaBins=400;
  Double_t MetParaBins[NMetParaBins+1];
  for (int i=0; i<=NMetParaBins; i++) { MetParaBins[i] = -200.0+400.0/NMetParaBins*i; };
  const Int_t NMetPerpBins=400;
  Double_t MetPerpBins[NMetPerpBins+1];
  for (int i=0; i<=NMetPerpBins; i++) { MetPerpBins[i] = -200.0+400.0/NMetPerpBins*i; };

  h2d1 = new TH2D("h_met_para_vs_zpt", "h_met_para_vs_zpt", NZPtBins, ZPtBins, NMetParaBins, MetParaBins);
  h2d2 = new TH2D("h_met_perp_vs_zpt", "h_met_perp_vs_zpt", NZPtBins, ZPtBins, NMetPerpBins, MetPerpBins);
  h2d1->Sumw2();
  h2d2->Sumw2();
  std::cout << "start draw" << std::endl;
  tree->Draw("llnunu_l2_pt*cos(llnunu_l2_phi-llnunu_l1_phi):llnunu_l1_pt>>h_met_para_vs_zpt", selec.c_str(), "colz");
  std::cout << "end draw1" << std::endl;
  tree->Draw("llnunu_l2_pt*sin(llnunu_l2_phi-llnunu_l1_phi):llnunu_l1_pt>>h_met_perp_vs_zpt", selec.c_str(), "colz");
  std::cout << "end draw2" << std::endl;
  
  h2d1->GetXaxis()->SetTitle("P_{T}(Z) (GeV)");
  h2d1->GetYaxis()->SetTitle("MET para (GeV)");
  h2d2->GetXaxis()->SetTitle("P_{T}(Z) (GeV)");
  h2d2->GetYaxis()->SetTitle("MET para (GeV)");

  Nbins = NZPtBins;
  for (int ii=0; ii<Nbins; ii++) fit_rebin.push_back(2);
  for (int ii=0; ii<Nbins; ii++) fit_min.push_back(-50);
  for (int ii=0; ii<Nbins; ii++) fit_max.push_back(20);

  sprintf(name, "%s/%s%s.pdf", outputdir.c_str(), filename.c_str(), tag.c_str());
  std::string plotfilename(name);
  fit_slice_gaus(h2d1, h1d1, plotfilename);
  fit_slice_gaus(h2d2, h1d2, plotfilename);

  sprintf(name, "%s/%s%s.pdf]", outputdir.c_str(), filename.c_str(), tag.c_str());
  plots->Print(name);


  fout->Close();
}

int fit_slice_gaus(TH2D* h2d, TH1D** h1d, std::string& plotfile){ 

  std::string hname = h2d->GetName();
  Double_t* xbins = (Double_t*)h2d->GetXaxis()->GetXbins()->GetArray();
  plots->cd();
  plots->Clear();
  plots->SetLogx(1);
  h2d->Draw("colz");
  lumipt->Draw();
  plots->Print(plotfile.c_str());
  plots->SetLogx(0);
  plots->Clear();
  
  fout->cd();
  h2d->Write();

  sprintf(name, "%s_mean", hname.c_str());
  TH1D* h_mean = new TH1D(name, name, Nbins, xbins);
  h_mean->Sumw2();
  h_mean->GetXaxis()->SetTitle("P_{T}(Z) (GeV)");
  h_mean->GetYaxis()->SetTitle("mean (GeV)");
  h_mean->SetLineColor(2);
  h_mean->SetMarkerColor(2);
  h_mean->SetMarkerStyle(20);
  sprintf(name, "%s_sigma", hname.c_str());
  TH1D* h_sigma = new TH1D(name, name, Nbins, xbins);
  h_sigma->Sumw2();
  h_sigma->GetXaxis()->SetTitle("P_{T}(Z) (GeV)");
  h_sigma->GetYaxis()->SetTitle("sigma (GeV)");
  h_sigma->SetLineColor(4);
  h_sigma->SetMarkerColor(4);
  h_sigma->SetMarkerStyle(20);

  for (int i=0; i<Nbins; i++){
    sprintf(name, "%s_bin%d_func", hname.c_str(), i+1);
    TF1* afunc = new TF1(name, "gaus", -100,+100);
    sprintf(name, "%s_bin%d", hname.c_str(), i+1);
    TH1D* ahist = (TH1D*)h2d->ProjectionY(name, i+1, i+1, "e");
    ahist->SetTitle(name);
    ahist->Rebin(fit_rebin[i]);
    ahist->Fit(afunc, "R", "", fit_min[i], fit_max[i]);
    double mean = afunc->GetParameter(1);
    double sigma = afunc->GetParameter(2);
    ahist->Fit(afunc, "R", "", mean-2*sigma, mean+2*sigma); 
    for (int ifit=0; ifit<2; ifit++){
      mean = afunc->GetParameter(1);
      sigma = afunc->GetParameter(2);
      ahist->Fit(afunc, "R", "", mean-1.5*sigma, mean+1.5*sigma);
    }
    func1[i] = afunc;
    h1d1[i] = ahist;

    h_mean->SetBinContent(i+1, afunc->GetParameter(1));
    h_mean->SetBinError(i+1, afunc->GetParError(1));
    h_sigma->SetBinContent(i+1, afunc->GetParameter(2));
    h_sigma->SetBinError(i+1, afunc->GetParError(2));
    

    plots->cd();
    plots->Clear();
    ahist->Draw();
    lumipt->Draw();
    pvtxt->Clear();
    sprintf(name, "%.2f < P_{T}(Z) < %.2f", xbins[i], xbins[i+1]);
    pvtxt->AddText(0.15,0.6, name);
    sprintf(name, "#mu = %.2f #pm %.2f", afunc->GetParameter(1), afunc->GetParError(1));
    pvtxt->AddText(0.15,0.3, name);
    sprintf(name, "#sigma = %.2f #pm %.2f", afunc->GetParameter(2), afunc->GetParError(2));
    pvtxt->AddText(0.15,0.0, name);
    pvtxt->Draw();
    plots->Print(plotfile.c_str());    
    plots->Clear();    
   
    fout->cd();
    ahist->Write();
    afunc->Write();
  }


  plots->cd();
  plots->Clear();
  plots->SetLogx(1);
  h_mean->Draw();
  lumipt->Draw();
  plots->Print(plotfile.c_str());
  plots->SetLogx(0);
  plots->Clear();

  plots->cd();
  plots->Clear();
  plots->SetLogx(1);
  h_sigma->Draw();
  lumipt->Draw();
  plots->Print(plotfile.c_str());
  plots->SetLogx(0);
  plots->Clear();

  fout->cd();
  h_mean->Write();
  h_sigma->Write();


  return 0;

}
