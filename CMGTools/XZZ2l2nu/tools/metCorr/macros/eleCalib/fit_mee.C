{

  bool doMu=false;

  TFile* file1 = TFile::Open("/home/heli/XZZ/80X_20170124_light_Skim/SingleEMU_Run2016Full_ReReco_v1.root");
  TFile* file2 = TFile::Open("/home/heli/XZZ/80X_20170124_light_Skim/DYJetsToLL_M50_MGMLM_BIG.root");

  TTree* tree1 = (TTree*)file1->Get("tree");
  TTree* tree2 = (TTree*)file2->Get("tree");

  std::string selec0 ;
  if (doMu) 
    selec0 += "abs(llnunu_l1_l1_pdgId)==13&&abs(llnunu_l1_l2_pdgId)==13&&llnunu_l1_l1_pt>50&&abs(llnunu_l1_l1_eta)<2.4&&llnunu_l1_l2_pt>20&&abs(llnunu_l1_l2_eta)<2.4&&(llnunu_l1_l1_highPtID>0.99||llnunu_l1_l2_highPtID>0.99)";
  else
    selec0 += "abs(llnunu_l1_l1_pdgId)==11&&abs(llnunu_l1_l2_pdgId)==11&&llnunu_l1_l1_pt>115&&abs(llnunu_l1_l1_eta)<2.5&&llnunu_l1_l2_pt>35&&abs(llnunu_l1_l2_eta)<2.5";
  selec0 += "&&llnunu_l1_mass>50&&llnunu_l1_mass<150";
  
  std::string selec_trg;
  if (doMu) 
    selec_trg = "((llnunu_l1_l1_trigerob_HLTbit>>3&1)||(llnunu_l1_l1_trigerob_HLTbit>>4&1)||(llnunu_l1_l2_trigerob_HLTbit>>3&1)||(llnunu_l1_l2_trigerob_HLTbit>>4&1))";
  else
    selec_trg = "((llnunu_l1_l1_trigerob_HLTbit>>1&1)||(llnunu_l1_l2_trigerob_HLTbit>>1&1))";

  std::string selec_dt = selec0;
  selec_dt = "("+selec_dt+"&&"+selec_trg+")";

  std::string selec_mc = selec0;
  selec_mc = "("+selec_mc+"&&"+selec_trg+")";

  std::string mc_weight;
  mc_weight += "trgsf*idsf*isosf*trksf";
  mc_weight += "*ZPtWeight";
  //mc_weight += "*(0.32+0.42*TMath::Erf((rho-4.16)/4.58)+0.31*TMath::Erf((rho+115.00)/29.58))";
  mc_weight += "*puWeightsummer16";
  mc_weight += "*36814*xsec/SumWeights*genWeight";
  if (doMu)
    mc_weight += "*1.0";
  else
    mc_weight += "*1.0";
  mc_weight = "("+mc_weight+")";

  selec_mc = "("+selec_mc+"*"+mc_weight+")";

  TH1D* h1 = new TH1D("h1", "h1", 200, 50, 150);
  TH1D* h2 = new TH1D("h2", "h2", 200, 50, 150);

  h1->Sumw2();
  h2->Sumw2();

  h1->SetLineColor(2);
  h1->SetMarkerColor(2);
  h2->SetLineColor(4);
  h2->SetMarkerColor(4);
 

  tree1->Draw("llnunu_l1_mass>>h1", selec_dt.c_str());
  tree2->Draw("llnunu_l1_mass>>h2", selec_mc.c_str());


  h1->Draw();
  h2->Draw("same");
 

using namespace RooFit;


  // Declare observable x
  RooRealVar x("x","x",50,150) ;
  RooDataHist dh_dt("dh_dt","dh_dt",x,Import(*h1)) ;
  RooDataHist dh_mc("dh_mc","dh_mc",x,Import(*h2)) ;

  RooPlot* frame = x.frame(Title("Z mass")) ;
  dh_dt.plotOn(frame,MarkerColor(2),MarkerSize(0.9),MarkerStyle(21));  //this will show histogram data points on canvas 
  dh_mc.plotOn(frame,MarkerColor(4),MarkerSize(0.9),MarkerStyle(21));  //this will show histogram data points on canvas 
  dh_dt.statOn(frame);  //this will display hist stat on canvas
  dh_mc.statOn(frame);  //this will display hist stat on canvas

  RooRealVar mean_dt("mean_dt","mean_dt",95.0, 70.0, 120.0);
  RooRealVar width_dt("width_dt","width_dt",5.0, 0.0, 120.0);
  RooRealVar sigma_dt("sigma_dt","sigma_dt",5.0, 0.0, 120.0);
  RooVoigtian gauss_dt("gauss_dt","gauss_dt",x,mean_dt,width_dt,sigma_dt);

  RooRealVar mean_mc("mean_mc","mean_mc",95.0, 70.0, 120.0);
  RooRealVar width_mc("width_mc","width_mc",5.0, 0.0, 120.0);
  RooRealVar sigma_mc("sigma_mc","sigma_mc",5.0, 0.0, 120.0);
  RooVoigtian gauss_mc("gauss_mc","gauss_mc",x,mean_mc,width_mc,sigma_mc);

  RooFitResult* filters_dt = gauss_dt.fitTo(dh_dt,"qr");
  RooFitResult* filters_mc = gauss_mc.fitTo(dh_mc,"qr");
  gauss_dt.plotOn(frame,LineColor(2));//this will show fit overlay on canvas 
  gauss_mc.plotOn(frame,LineColor(4));//this will show fit overlay on canvas 
  gauss_dt.paramOn(frame); //this will display the fit parameters on canvas
  gauss_mc.paramOn(frame); //this will display the fit parameters on canvas
  //filters_dt->Print("v");
  //filters_mc->Print("v");

  // Draw all frames on a canvas
  TCanvas* c = new TCanvas("ZmassHisto","ZmassHisto",800,400) ;
  c->cd() ; 
  gPad->SetLeftMargin(0.15);
           
  frame->GetXaxis()->SetTitle("Z mass (GeV)");  
  frame->GetXaxis()->SetTitleOffset(1.2);
  //float binsize = hzmass->GetBinWidth(1); 
  //char Bsize[50]; 
  //sprintf(Bsize,"Events per %2.2f",binsize);
  // frame->GetYaxis()->SetTitle(Bsize);  
  //frame->GetYaxis()->SetTitleOffset(1.2);
  frame->Draw() ;
  c->Print("myZmaa.eps");  





}
