{


//TFile* f1 = TFile::Open("study_gjets_data_b2h36p22fbinv_v5resbos_norm.root");
TFile* f1 = TFile::Open("study_gjets_data_b2h36p22fbinv_v5resbos_norm_modify.root");
TFile* f2 = TFile::Open("study_gjets_data_36p46_resbos_norm_npucut.root");
//TFile* f2 = TFile::Open("study_gjets_data_36p46_resbos_norm.root");
char name[1000];
//sprintf(name, "h_zpt_1_mu");
sprintf(name, "h_zpt_ratio_mu");
TH1D* h1 = (TH1D*)f1->Get(name);
TH1D* h2 = (TH1D*)f2->Get(name);
//TProfile* h1 = (TProfile*)f2->Get(name);
//sprintf(name, "pr_zpt_2");
//TProfile* h2 = (TProfile*)f2->Get(name);

h1->SetLineColor(2);
h2->SetLineColor(4);
h1->Draw();
h2->Draw("same");


}
