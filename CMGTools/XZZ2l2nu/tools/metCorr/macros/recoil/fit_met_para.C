

std::string channel = "all";
bool doMC = true;
bool doGJets = false;
bool doGJetsMC = false;
bool useZSelecLowLPt = true;
bool useEffSf = false;
bool mcTrgSf = false;
bool dtTrgSf = false;
bool dtHLT = false;
bool mcUseRhoWt = false;

// recipe:
// 1.) useZSelecLowLPt can well reproduce the results with full cuts (useZSelec) + HLT (dtHLT or dtTrgSf or mcTrgSf) 
// 2.) for Data:  doMC=false, dGJets=false, useZSelecLowLPt=true, useEffSf=false
// 3.) for MC : doMC=false, doGJets=false, useZSelecLowLPt=true, useEffSf=false
// 4.) for GJets: doGJets=true, doMC=false, useZSelecLowLPt=true, useEffSf=false

std::string inputdir = 
"/home/heli/XZZ/80X_20170202_light_Skim"
//"/datac/heli/XZZ2/80X_20161029_light_Skim"
;
std::string filename;

std::string outputdir = "./recoil_out8";
std::vector< std::string > channels = {"all", "mu", "el"};
std::vector< std::string > mcfiles = {
  //  "DYJetsToLL_M50_MGMLM_BIG_NoRecoil", 
    "DYJetsToLL_M50_Ext_NoRecoil"
 };

std::vector< std::string > dtfiles = {
"SingleEMU_Run2016Full_03Feb2017_v0"
 };

std::vector< std::string > gjfiles = {
"SinglePhoton_Run2016Full_03Feb2017_v0_NoRecoil"
 };


char name[1000];
TCanvas* plots;
std::string tag0 = "";
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
TProfile* pzpt;
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
  if (useZSelecLowLPt) tag += "_ZSelecLowLPt";
  if (doMC && mcUseRhoWt) tag += "_RhoWt";
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
  //std::string cuts_zmass="(llnunu_l1_mass>70&&llnunu_l1_mass<110)";
  std::string cuts_zmass="(llnunu_l1_mass>70&&llnunu_l1_mass<110&&nbadmuon==0)"; // add bad muon filter
  //std::string cuts_loose_z="("+metfilter+"&&"+cuts_lepaccept+"&&"+cuts_zmass+")";
  //std::string cuts_loose_z_lowlpt="("+metfilter+"&&"+cuts_lepaccept_lowlpt+"&&"+cuts_zmass+")";
  // metfilter pre-applied already for full dataset
  std::string cuts_loose_z="("+cuts_lepaccept+"&&"+cuts_zmass+")";
  std::string cuts_loose_z_lowlpt="("+cuts_lepaccept_lowlpt+"&&"+cuts_zmass+")";



  base_selec =  cuts_loose_z;
  if (useZSelecLowLPt) base_selec =  cuts_loose_z_lowlpt;


  if (channel=="el") base_selec = base_selec+"&&(abs(llnunu_l1_l1_pdgId)==11&&abs(llnunu_l1_l2_pdgId)==11)";
  else if (channel=="mu") base_selec = base_selec+"&&(abs(llnunu_l1_l1_pdgId)==13&&abs(llnunu_l1_l2_pdgId)==13)";

  if (!doMC && dtHLT) base_selec = "(HLT_MU50||HLT_TkMU50||HLT_ELE115)&&"+base_selec;

  base_selec = "("+base_selec+")";


  // add weight
  std::string weight_selec = std::string("*(genWeight*ZPtWeight*puWeightsummer16/SumWeights*xsec*35867)");
  // rho weight
  std::string rhoweight_selec = std::string("*(0.366*TMath::Gaus(rho,8.280,5.427)+0.939*TMath::Gaus(rho,18.641,10.001)+0.644*TMath::Gaus(rho,40.041,10.050))"); // rereco/summer16 38.81fb-1

  // reco vtx
  std::string vtxweight_selec = std::string("*(0.807+0.007*nVert+-3.689e-05*nVert*nVert+6.730e-04*exp(2.500e-01*nVert))"); // rereco 33.59 fb-1
  // scale factors
  std::string effsf_selec = std::string("*(isosf*idsf*trksf)");

  // selec, cuts + weights
  std::string selec = base_selec;
  //if (doMC) selec +=  weight_selec + vtxweight_selec;
  if (doMC) {
     if (mcUseRhoWt) selec +=  weight_selec + rhoweight_selec;
     else selec +=  weight_selec;
  }
  if (doMC && useEffSf) selec += effsf_selec;
  if ( (doMC && mcTrgSf) || (!doMC && dtTrgSf) ) selec += "*(trgsf)";
  
  if (doGJets) {
    base_selec = "(1)";
    if (doGJetsMC) base_selec = "(1)*(genWeight*puWeightsummer16/SumWeights*xsec*35867)";
    if (useZSelecLowLPt) {
      if (channel=="el")  selec = base_selec+"*(GJetsZPtWeightLowLPtEl)";
      else if (channel=="mu") selec = base_selec+"*(GJetsZPtWeightLowLPtMu)";
      else  selec = base_selec+"*(GJetsZPtWeightLowLPt)";
    }
    else {
      if (channel=="el")  selec = base_selec+"*(GJetsZPtWeightEl)";
      else if (channel=="mu") selec = base_selec+"*(GJetsZPtWeightMu)";
      else  selec = base_selec+"*(GJetsZPtWeight)";
    }

    if (!doGJetsMC) selec = selec + "*(GJetsRhoWeight*GJetsPreScaleWeight)";
    else selec = selec +"*(GJetsRhoWeight)";
  }
  // style
  gROOT->ProcessLine(".x tdrstyle.C");
  gStyle->SetOptTitle(0);

  sprintf(name, ".! mkdir -p %s", outputdir.c_str());
  gROOT->ProcessLine(name);


  // lumiTag for plotting
  lumiTag = "CMS 13 TeV 2016 L=35.87 fb^{-1}";
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

   plots = new TCanvas("plots", "plots");

  sprintf(name, "%s/%s%s.pdf[", outputdir.c_str(),filename.c_str(), tag.c_str());
  plots->Print(name);

  sprintf(name, "%s/%s%s.pdf", outputdir.c_str(), filename.c_str(), tag.c_str());
  std::string plotfilename(name);


  // other control plots
  Double_t ZPtBins[] = {0,2,4,6,8,10,12,14,16,18,20,22,24,26,28, 30, 35, 40, 50, 60, 70, 80, 90, 100, 120, 140, 160, 180, 220, 260, 300, 500, 5000 };
  //Double_t ZPtBins[] = {0,2,4,6,8,10,12,14,16,18,20,22,24,26,28, 30, 35, 40, 50, 60, 70, 80, 90, 100, 110, 120, 130, 140, 150, 160, 170, 180, 190, 200, 220, 240, 260, 300,  5000 };
  //Double_t ZPtBins[] = {0,2,4,6,8,10,12,14,16,18,20,22,24,26,28, 30, 35, 40, 50, 60, 70, 80, 90, 100, 110, 120, 130, 140, 150, 160, 170, 180, 190, 200, 220, 240, 260, 300, 500, 5000 };
  //Double_t ZPtBins[] = {0,2,4,6,8,10,12,14,16,18,20,22,24,26,28, 30, 35, 40, 50, 60, 80, 100, 150, 250, 5000 };
  //Double_t ZPtBins[] = {0,2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32,34,35,36,36.5,37,37.5,38,38.5,39,39.5,40,41,42,43,44,45,46,47,48,49,50,55,60,65,70,75,80,85,90,95,100,105, 110,120,125, 130,140,150,160,180,200,300,5000 };
  Int_t NZPtBins = sizeof(ZPtBins)/sizeof(ZPtBins[0]) - 1;
  const Int_t NMetParaBins=400;
  Double_t MetParaBins[NMetParaBins+1];
  for (int i=0; i<=NMetParaBins; i++) { MetParaBins[i] = -200.0+400.0/NMetParaBins*i; };
  const Int_t NMetPerpBins=400;
  Double_t MetPerpBins[NMetPerpBins+1];
  for (int i=0; i<=NMetPerpBins; i++) { MetPerpBins[i] = -200.0+400.0/NMetPerpBins*i; };

  // zpt profile
  pzpt = new TProfile("p_zpt", "p_zpt", NZPtBins, ZPtBins);
  tree->Draw("llnunu_l1_pt:llnunu_l1_pt>>p_zpt", selec.c_str(), "prof");
  pzpt->SetLineColor(2);
  pzpt->SetMarkerColor(2);
  pzpt->SetMarkerStyle(20);
  pzpt->GetXaxis()->SetTitle("P_{T}(Z) (GeV)");
  if (doGJets) pzpt->GetXaxis()->SetTitle("P_{T}(#gamma) (GeV)");
  pzpt->GetYaxis()->SetTitle("mean P_{T}(Z) (GeV)");
  if (doGJets) pzpt->GetYaxis()->SetTitle("mean P_{T}(#gamma) (GeV)");
  plots->cd();
  plots->Clear();
  plots->SetLogx(1);
  pzpt->Draw();
  lumipt->Draw();
  plots->Print(plotfilename.c_str());
  plots->SetLogx(0);
  plots->Clear();

  fout->cd();
  pzpt->Write(); 

  h2d1 = new TH2D("h_met_para_vs_zpt", "h_met_para_vs_zpt", NZPtBins, ZPtBins, NMetParaBins, MetParaBins);
  h2d2 = new TH2D("h_met_perp_vs_zpt", "h_met_perp_vs_zpt", NZPtBins, ZPtBins, NMetPerpBins, MetPerpBins);
  h2d1->Sumw2();
  h2d2->Sumw2();
  std::cout << "start draw1, selec="<< selec << std::endl;
  tree->Draw("llnunu_l2_pt*cos(llnunu_l2_phi-llnunu_l1_phi):llnunu_l1_pt>>h_met_para_vs_zpt", selec.c_str(), "colz");
  std::cout << "start draw2, selec="<< selec << std::endl;
  tree->Draw("llnunu_l2_pt*sin(llnunu_l2_phi-llnunu_l1_phi):llnunu_l1_pt>>h_met_perp_vs_zpt", selec.c_str(), "colz");
  
  h2d1->GetXaxis()->SetTitle("P_{T}(Z) (GeV)");
  if (doGJets) h2d1->GetXaxis()->SetTitle("P_{T}(#gamma) (GeV)");
  h2d1->GetYaxis()->SetTitle("MET para (GeV)");
  h2d2->GetXaxis()->SetTitle("P_{T}(Z) (GeV)");
  if (doGJets) h2d2->GetXaxis()->SetTitle("P_{T}(#gamma) (GeV)");
  h2d2->GetYaxis()->SetTitle("MET perp (GeV)");

  Nbins = NZPtBins;
  for (int ii=0; ii<Nbins; ii++) fit_rebin.push_back(2);
  for (int ii=0; ii<Nbins; ii++) fit_min.push_back(-50);
  for (int ii=0; ii<Nbins; ii++) fit_max.push_back(20);

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
  if (doGJets) h_mean->GetXaxis()->SetTitle("P_{T}(#gamma) (GeV)");
  else h_mean->GetXaxis()->SetTitle("P_{T}(Z) (GeV)");
  h_mean->GetYaxis()->SetTitle("mean (GeV)");
  h_mean->SetLineColor(2);
  h_mean->SetMarkerColor(2);
  h_mean->SetMarkerStyle(20);
  sprintf(name, "%s_sigma", hname.c_str());
  TH1D* h_sigma = new TH1D(name, name, Nbins, xbins);
  h_sigma->Sumw2();
  if (doGJets) h_sigma->GetXaxis()->SetTitle("P_{T}(#gamma) (GeV)");
  else h_sigma->GetXaxis()->SetTitle("P_{T}(Z) (GeV)");
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
    sprintf(name, "%.2f < P_{T}(Z) < %.2f GeV", xbins[i], xbins[i+1]);
    if (doGJets) sprintf(name, "%.2f < P_{T}(#gamma) < %.2f GeV", xbins[i], xbins[i+1]);
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

  h_mean->GetXaxis()->SetRangeUser(2,5000);
  h_mean->GetYaxis()->SetRangeUser(-6,15);
  h_sigma->GetXaxis()->SetRangeUser(2,5000);
  h_sigma->GetYaxis()->SetRangeUser(15,40);

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
