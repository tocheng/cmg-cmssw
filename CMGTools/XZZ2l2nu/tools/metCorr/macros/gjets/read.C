{
  TFile* file = TFile::Open("afile.root");
  //TFile* file = TFile::Open("afile1.root");

  TTree* Events = (TTree*)file->Get("Events");
  //Events->Print();
  Events->SetAlias("ph", "patPhotons_slimmedPhotons__RECO.obj");
  Events->SetAlias("met", "patMETs_slimmedMETs__RECO.obj");
  Events->SetAlias("el", "patElectrons_slimmedElectrons__RECO.obj");
  Events->SetAlias("jet", "patJets_slimmedJets__RECO.obj");
  Events->SetAlias("mu", "patMuons_slimmedMuons__RECO.obj");
  Events->SetAlias("tau", "patTaus_slimmedTaus__RECO.obj");
  Events->SetAlias("pf", "patPackedCandidates_packedPFCandidates__RECO.obj");
  Events->SetAlias("pfltk", "patPackedCandidates_lostTracks__RECO.obj");
  //Events->SetAlias("", "");
  //Events->SetAlias("", "");
  //Events->SetAlias("", "");

}
