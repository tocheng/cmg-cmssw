{

  TFile * file = TFile::Open("gjets_dick.root");
  TTree* tree = (TTree*)file->Get("tree");

  UInt_t run;
  UInt_t lum;
  ULong64_t evt; 

  tree->SetBranchAddress("run", &run);
  tree->SetBranchAddress("lumi", &lum);
  tree->SetBranchAddress("evt", &evt);


  for (int i=0; i<tree->GetEntries(); i++){

    tree->GetEntry(i);

    std::cout << run << ":" << lum << ":" << evt << std::endl;

  }


}
