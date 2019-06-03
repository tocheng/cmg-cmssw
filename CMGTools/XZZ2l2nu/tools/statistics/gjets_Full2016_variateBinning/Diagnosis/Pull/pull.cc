#include <iostream>
#include <iomanip>
#include <string>
#include <cstdlib>
#include <stdio.h>

#include "TROOT.h"
#include "TFile.h"
#include "TString.h"
#include "TH1.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TF1.h"
#include "TLegend.h"
#include "TMultiGraph.h"
#include "THStack.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TMath.h"
#include "TTree.h"
#include "TTreeIndex.h"
#include "TH2F.h"
#include "TLatex.h"
#include "TLine.h"
#include "TGraphAsymmErrors.h"
#include "Math/QuantFuncMathCore.h"

#include "RooChebychev.h"

#include "TSystem.h"
#include "TStyle.h"

#include "TPaveLabel.h"
#include "TLegend.h"

#include "TLorentzRotation.h"
#include "TVector3.h"
#include "TLorentzVector.h"
//
#include <vector>
#include <fstream>
//
#include "TRandom3.h"
  
#include "RooRealVar.h"
#include "RooArgSet.h"
#include "RooGaussian.h"
#include "RooBreitWigner.h"
#include "RooProdPdf.h"
#include "RooDataSet.h"
#include "RooGlobalFunc.h"
#include "RooDataHist.h"
#include "RooHistPdf.h"
#include "RooCBShape.h"
#include "RooMinuit.h"
#include "RooFormulaVar.h"
#include "RooAddPdf.h"
#include "RooPlot.h"
#include "RooAbsPdf.h"
#include "RooLandau.h"

//make own PDF
#include "RooClassFactory.h"
#include "RooGenericPdf.h"
#include "RooFFTConvPdf.h"

// I/O
#include <iostream>

using namespace std;

/////////////////////

void ReadTree(TTree* tree, TString type);

bool debug_;

int main(int argc, char *argv[])
{

  gStyle->SetOptStat(0000);
  //gStyle->SetOptTitle(0);  

  gSystem->AddIncludePath("-I $ROOFITSYS");
  gSystem->AddIncludePath("-I$ROOFITSYS/include/");

  using namespace std;

  if(argc != 3)  {
      cout<<argv[0]<<" filename, "<<argv[1]<<" type "<<argv[2]<<endl;
      return -1;
    }

  debug_ = false;

  /////////////////////

  TString filename = argv[1];
  TString type = argv[2];

  /////////////////////

    TFile* infile = new TFile(filename+".root");

    TTree* tree; 

    if(infile) tree = (TTree*) infile->Get("limit");
    if(!tree) { cout<<"ERROR could not find the tree for "<<filename<<endl; return -1;}

    // void BookPDF(TString filename, vector<TH1F*> & QCD, TString obs, TString pdf, vector<double> binning)
    //BookHists();

    // read tree     
    cout<<"start reading tree"<<endl;
    ReadTree(tree, type);

    delete tree;

  return 0;

}


void ReadTree(TTree* tree, TString type){    

        TFile* fmZ1 = new TFile("Tree.root", "recreate");

        double Vpull;
        TTree* newtree = new TTree("passedEvents_new","passedEvents_new");
        newtree->Branch("pull", &Vpull,   "pull/D");

        float MH, quantileExpected;
        tree->SetBranchAddress(type,&MH);
        tree->SetBranchAddress("quantileExpected",&quantileExpected);

        double nominal = 125;
        if(type=="MH") nominal = 125;
        if(type=="r") nominal = 0.1;

        for(int mcfmEvt_HZZ=0; mcfmEvt_HZZ < tree->GetEntries()/3; mcfmEvt_HZZ++) {             

            tree->GetEntry(3*mcfmEvt_HZZ);             
            double c = double(MH);
            tree->GetEntry(3*mcfmEvt_HZZ+1);
            double d = double(MH);
            tree->GetEntry(3*mcfmEvt_HZZ+2);
            double u = double (MH);
 

            cout<<"c "<<c<<" u "<<u<<" d "<<d<<endl;
            double err;

            err = (u-c);
            Vpull = (c-nominal)/err;
            cout<<"pull "<<Vpull<<endl;
            newtree->Fill();

            err = (c-d);
            Vpull = (c-nominal)/err;
            cout<<"pull "<<Vpull<<endl;
            newtree->Fill();

       } // event loop

       cout<<"end loop"<<endl;

       cout<<"Number of tree entries is "<<newtree->GetEntries()<<endl;
     
       RooRealVar pull("pull","pull", -3, 3);
       RooDataSet* data =  new RooDataSet("data", "dataset with pull",newtree,RooArgSet(pull));

       RooRealVar mean("mean", "mean", 0.0, -0.2,0.2);
       RooRealVar sigma("sigma", "sigma", 1.0, 0.5, 1.5);

       RooGaussian gauss("gauss","gauss",pull,mean,sigma);

       gauss.fitTo(*data);

       RooPlot *Pmass = pull.frame(RooFit::Bins(30));
       data->plotOn(Pmass);
       
       gauss.plotOn(Pmass,  RooFit::LineColor(2));
       gauss.paramOn(Pmass);
   
       TCanvas* c = new TCanvas("c","c",1000,800);
       c->cd();

       Pmass->Draw("");

       c->SaveAs("Pull_Fitting_"+type+".pdf");
       c->SaveAs("Pull_Fitting_"+type+".png");

       delete newtree;
       delete c;
       delete fmZ1;

}


/*
 ummy->SetMarkerSize(0);
void Plot(TString fs, double mZ1RECO){

                    RooRealVar massZ1("mZ1RECO", "mZ1RECO", mZ1RECO);
                    RooRealVar s1("s1", "s1", 0.8);
                    s1.setVal(SIGMAmZ1); s1.setConstant(1);
                    RooRealVar mZ1fit("massZ1", "massZ1", mZ1RECO, 40.0, 120.0);

                    RooGaussian gauss("gauss","gaussian PDF", massZ1, mZ1fit, s1);
                    TString fmZ1_s;
                    fmZ1_s = TString("../hist/massZ1"+fs+".root");
                    TFile fmZ1 = new TFile(fmZ1_s, "READ");

                    TH1F* PDFmZ1 = TH1F* (fmZ1->Get("massZ1"));
                    PDFmZ1->Sumw2();
                   

                    RooDataHist* mZ1TempDataHist = new RooDataHist("DATAmZ1","DATAmZ1",RooArgList(massZ1fit),PDFmZ1);
                    RooHistPdf*  mZ1TemplatePdf  = new RooHistPdf("PDFmZ1","PDFmZ1",RooArgSet(massZ1fit),*mZ1TempDataHist);

                    RooProdPdf PDF("PDF","PDF", gauss, *mZ1TemplatePdf);
                    RooDataSet mass = = PDF.generate(x,1000)

*/

