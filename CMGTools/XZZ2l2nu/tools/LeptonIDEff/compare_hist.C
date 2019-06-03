{


TFile* f1 = TFile::Open("eff_out_ell1pt_36p46recalib.root");
TFile* f2 = TFile::Open("eff_out_ell1pt_36p46.root");
TH1D* h1 = (TH1D*)f1->Get("el1pt");
h1= (TH1D*)f1->Get("ell1pt");
TH1D* h2 = (TH1D*)f2->Get("ell1pt");
h1->SetLineColor(2);
h2->SetLineColor(4);
h1->Draw();
h2->Draw("same");


}
