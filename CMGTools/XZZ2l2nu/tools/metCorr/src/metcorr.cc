// Hengne Li, 2016 @ CERN, initial version.

#include "metcorr.h"


int main(int argc, char** argv) {

  if( argc<5 ) {
     std::cout << argv[0] << ":  " << std::endl ;
     std::cout << " Functionality: met kin-fit framework, actually a full analysis framework  ... "  << std::endl;
     std::cout << "                 "  << std::endl;
     std::cout << " usage: " << argv[0] << " config.file inputfile.root outputfile.root Nevts SumWeights " << std::endl ;
     exit(1) ;
  }

  // config file name
  _file_config_name = std::string((const char*)argv[1]);

  // input file name
  _file_in_name = std::string((const char*)argv[2]);

  // output file name
  _file_out_name = std::string((const char*)argv[3]);

  // sum events
  _SumEvents = atof(argv[4]);

  // sum weights
  _SumWeights = atof(argv[5]);

  // input file
  _file_in = TFile::Open(_file_in_name.c_str());

  // output file 
  _file_out = TFile::Open(_file_out_name.c_str(), "recreate");

  // initialize randome number
  _rand3 = new TRandom3(1117);

  // special for DYJets MC samples,
  //  decide if this is the DYJets samples, and LO or NLO, 
  //  based on the output file names. So...note below:
  // ATTENTION: 
  //   Makes sure output NLO DYJets MC output file name has string
  //   "DYJets" but does not have "MGMLM", and LO DYJets one has 
  //   both strings "DYJets" and "MGMLM". 
  _isDyJets = (_file_out_name.find("DYJets")!=std::string::npos)
            ||(_file_out_name.find("DY0Jets")!=std::string::npos)
            ||(_file_out_name.find("DY1Jets")!=std::string::npos)
            ||(_file_out_name.find("DY2Jets")!=std::string::npos)
            ||(_file_out_name.find("DY3Jets")!=std::string::npos)
            ||(_file_out_name.find("DY4Jets")!=std::string::npos)
            ;

  _isDyJetsLO = (_isDyJets && _file_out_name.find("MGMLM")!=std::string::npos);

  // njets samples
  _isDyJetsLOnjets = -1; // -1 for inclusive

  // nlo xsec for each lo ijets binned samples
  _DyJetsNLOxsec = 5765.4; // inclusive
   
  // find samples
  if (_isDyJets && _isDyJetsLOnjets) {
    if (_file_out_name.find("DY1Jets")!=std::string::npos) {
      _isDyJetsLOnjets = 1;
      _DyJetsNLOxsec = 1177.72740472;
    }
    else if  (_file_out_name.find("DY2Jets")!=std::string::npos) {
      _isDyJetsLOnjets = 2;
      _DyJetsNLOxsec = 389.126715064;
    }
    else if  (_file_out_name.find("DY3Jets")!=std::string::npos) {
      _isDyJetsLOnjets = 3;
      _DyJetsNLOxsec = 119.051615245;
    }
    else if  (_file_out_name.find("DY4Jets")!=std::string::npos) {
      _isDyJetsLOnjets = 4;
      _DyJetsNLOxsec = 63.3043012704;
    }
  }
 
  // check if it is sm ZZ sample, based on file names
  _isZZ = (_file_out_name.find("ZZTo2L2Nu")!=std::string::npos);
 
  if (_debug) std::cout << "DEBUG: isDyJets = " << _isDyJets << ", isDyJetsLO = " << _isDyJetsLO << std::endl;

  // read config file
  readConfigFile();


  // make sure to turn off not needed options if _doGJetsSkim is true
  if (_doGJetsSkim) {
    _isDyJets = false;
    _isDyJetsLO = false;
    _isZZ = false;
  }

  // prepare the trees
  prepareTrees();

  // make sure _doMTUncDummy is on if DyJets and doGJets/is data
  if (_isDyJets||(_doGJetsSkim&&_isData)) {
    _doMTUncDummy = true;
  }

  // prepare inputs for pu weights
  if (_addPUWeights && !_isData) preparePUWeights();

  // GJets skim
  if (_doGJetsSkim) prepareGJetsSkim();

  // prepare inputs for muon re-calib
  if (_doMuonPtRecalib && !_doGJetsSkim) prepareMuonPtRecalib();

  // prepare inputs for addDyZPtWeight
  if (_addDyZPtWeight && !_isData && _isDyJets && !_doGJetsSkim) prepareDyZPtWeight();

  // prepare inputs for addZZCorrections
  if (_addZZCorrections && !_isData && _isZZ ) prepareZZCorrections();

  // prepare inputs for JEC/JER
  if (_doJEC )  prepareJECJER();

  // prepare inputs for simple met recoil tune.
  if (_doRecoil && ((!_isData && _isDyJets && !_doGJetsSkim )||(_isData && _doGJetsSkim)) ) prepareRecoil();

  // prepare eff scale factors
  if (_addEffScale && (!_isData || _addEffScaleOnData) && !_doGJetsSkim ) prepareEffScale();

  // prepare eff scale factor for emu
  if (_addEMuTrgScale) prepareEmuTrgsf();

  // loop
  int n_pass = 0;

  // get n selected entries to loop 
  Long64_t n_selected_entries = _selected_entries->GetN();

  for (Int_t i_slist = _n_start; i_slist < n_selected_entries; i_slist++) {

    // get _tree_in's entry number from selected list
    Int_t i = _selected_entries->GetEntry(i_slist);

    // get the selected entry from tree_in
    _tree_in->GetEntry(i);

    // beak if runs more events than  you want to test
    n_pass++;
    if (_n_test>0 && n_pass>_n_test) break;

    //
    if (_debug || i%_n_interval == 0) {
      if (_debug) std::cout << "#############################################" << std::endl;
      std::cout << "##  Entry " << i << ", Run " << _run << ", Event " << _evt << std::endl;
      if (_debug) std::cout << "#############################################" << std::endl;
    }

    // _storeOldBranches
    if (_storeOldBranches && !_doGJetsSkim) storeOldBranches();

    // add pu weights
    if (_addPUWeights && !_isData) addPUWeights();

    // GJets skim
    if (_doGJetsSkim) doGJetsSkim();

    //  addDyZPtWeight
    if (_addDyZPtWeight && !_isData && _isDyJets && !_doGJetsSkim) addDyZPtWeight();

    //  addZZCorrections
    if (_addZZCorrections && !_isData && _isZZ ) addZZCorrections();

    // simple met recoil tune.
    if (_doRecoil && ((!_isData && _isDyJets && !_doGJetsSkim )||(_isData && _doGJetsSkim)) ) doRecoil();

    // add eff scale factors
    if (_addEffScale && (!_isData || _addEffScaleOnData) && !_doGJetsSkim ) addEffScale();
    // add eff scale factors for emu
    if (_addEMuTrgScale) addEmuTrgsf();

    // add alternative MT due to MET unc NOT for data
    if (_doMTUnc ) doMTUnc();
    if (_doMTUnc && _doGJetsSkim ) doMTUncEl();
    if (_doMTUnc && _doGJetsSkim ) doMTUncMu();

    if (_doBoostJet){
 
       //std::cout<<"nboostJetAK8Puppi "<<_nboostJetAK8Puppi<<std::endl;
       if(_nboostJetAK8Puppi>0){
         doBoostJetSelection(); 
       }
       else continue;

    }

    // fill output tree
    _tree_out->Fill(); 

  }  


  // store output tree
  _file_out->cd();
  _tree_out->Write();
  _file_out->Close();

  return 0;

}





//======================================================
// ╦═╗╔═╗╔═╗╔╦╗  ╔═╗╔═╗╔╗╔╔═╗╦╔═╗  ╔═╗╦╦  ╔═╗
// ╠╦╝║╣ ╠═╣ ║║  ║  ║ ║║║║╠╣ ║║ ╦  ╠╣ ║║  ║╣ 
// ╩╚═╚═╝╩ ╩═╩╝  ╚═╝╚═╝╝╚╝╚  ╩╚═╝  ╚  ╩╩═╝╚═╝
//======================================================

void readConfigFile() 
{

  // init paramter file
  PParameterReader parm(_file_config_name.c_str());

  // print it
  parm.Print();

  //===============================================
  // NOTE: 
  //  - See notes above in parameter definitions
  //     for the meanings and usage of them.
  //  - Please always use the same variable name
  //     in the c++ codes and in the config files,
  //     with the c++ one has "_" in front, 
  //      e.g.:   
  //       bool _AAA = parm.GetBool("AAA", kFALSE);
  //     This rule can minimize mistakes.
  //==============================================

  //======================
  // general parameters
  //======================
  _debug = parm.GetBool("debug", kFALSE);
  _n_start = parm.GetInt("n_start", 0);
  _n_test = parm.GetInt("n_test", -1);
  _n_interval = parm.GetInt("n_interval", 3000);
  _selection = parm.GetString("selection", "(1)");
  _storeOldBranches = parm.GetBool("storeOldBranches", kFALSE);


  //==========================
  // use slimmed tree or not
  //=========================
  _useLightTree = parm.GetBool("useLightTree", kTRUE);
  _storeErr = parm.GetBool("storeErr", kTRUE);
  _removeHLTFlag = parm.GetBool("removeHLTFlag", kFALSE);
  _removeMETFlag = parm.GetBool("removeMETFlag", kFALSE);
  _doMTUnc = parm.GetBool("doMTUnc", kTRUE);
  _doMTUncDummy = parm.GetBool("doMTUncDummy", kFALSE);

  //=========================
  // add PU weights
  //=========================
  _addPUWeights = parm.GetBool("addPUWeights", kTRUE);
  _PUWeightProtectionCut = parm.GetDouble("PUWeightProtectionCut", 1000);

  if (_addPUWeights) {
    _PUTags = parm.GetVString("PUTags");
    _PUInputDir = parm.GetString("PUInputDir", "data/pileup");
    _PUInputFileNames = parm.GetVString("PUInputFileNames");
    _PUWeightHistName = parm.GetString("PUWeightHistName", "puweight_dtmc");
  }

  //==========================
  // muon pT recalibration
  //==========================
  _doMuonPtRecalib = parm.GetBool("doMuonPtRecalib", kFALSE);

  if (_doMuonPtRecalib) {
    _MuonPtRecalibInputForData = parm.GetString("MuonPtRecalibInputForData", "data/kalman/DATA_80X_13TeV.root");
    _MuonPtRecalibInputForMC = parm.GetString("MuonPtRecalibInputForMC", "data/kalman/MC_80X_13TeV.root");
  }

  _doElecPtRecalibSimpleData = parm.GetBool("doElecPtRecalibSimpleData", kFALSE);
  _doElecPtRecalibSimpleDataPogRecipe = parm.GetBool("doElecPtRecalibSimpleDataPogRecipe", kFALSE);
  _ElecPtRecalibSimpleDataScale = parm.GetDouble("ElecPtRecalibSimpleDataScale", 1.0);
  _doMuonPtRecalibSimpleData = parm.GetBool("doMuonPtRecalibSimpleData", kFALSE);
  _MuonPtRecalibSimpleDataScale = parm.GetDouble("MuonPtRecalibSimpleDataScale", 1.0);


  //========================
  // Add DYJet gen reweight 
  //========================
  _addDyZPtWeight = parm.GetBool("addDyZPtWeight", kTRUE);
  
  if (_addDyZPtWeight) {
    _addDyZPtWeightUseFunction = parm.GetBool("addDyZPtWeightUseFunction", kTRUE);
    _addDyZPtWeightUseResummationFunction = parm.GetBool("addDyZPtWeightUseResummationFunction", kFALSE);
    _addDyZPtWeightUseResummationRefitFunction = parm.GetBool("addDyZPtWeightUseResummationRefitFunction", kFALSE);
    _addDyZPtWeightLOUseFunction = parm.GetBool("addDyZPtWeightLOUseFunction", kTRUE);
    _DyZPtWeightInputFileName = parm.GetString("DyZPtWeightInputFileName", "data/zptweight/dyjets_zpt_weight_lo_nlo_sel.root");
    _addDyNewGenWeight = parm.GetBool("addDyNewGenWeight", kTRUE);;
  }

  //========================
  // Add ZZ correction
  //========================
  _addZZCorrections = parm.GetBool("addZZCorrections", kTRUE);
  
  if (_addZZCorrections) {
    _ZZCorrectionEwkInputFileName = parm.GetString("ZZCorrectionEwkInputFileName", "data/zzcorr/ZZ_EwkCorrections.dat");
    _ZZCorrectionQcdInputFileName = parm.GetString("ZZCorrectionQcdInputFileName", "data/zzcorr/zzqcd.root");
    

  }

  //==========================
  // do JEC JER correction
  //==========================
  _doJEC = parm.GetBool("doJEC", kFALSE); 
  _doJER = parm.GetBool("doJER", kFALSE); 
  
  // data jec
  _JECParTxt_DATA_L2L3Residual = parm.GetString("JECParTxt_DATA_L2L3Residual", "data/jec2016/Spring16_25nsV6_DATA_L2L3Residual_AK4PFchs.txt");
  _JECParTxt_DATA_L3Absolute   = parm.GetString("JECParTxt_DATA_L3Absolute", "data/jec2016/Spring16_25nsV6_DATA_L3Absolute_AK4PFchs.txt");
  _JECParTxt_DATA_L2Relative   = parm.GetString("JECParTxt_DATA_L2Relative", "data/jec2016/Spring16_25nsV6_DATA_L2Relative_AK4PFchs.txt");
  _JECParTxt_DATA_L1FastJet    = parm.GetString("JECParTxt_DATA_L1FastJet", "data/jec2016/Spring16_25nsV6_DATA_L1FastJet_AK4PFchs.txt");
  _JECParTxt_DATA_Uncertainty  = parm.GetString("JECParTxt_DATA_Uncertainty", "data/jec2016/Spring16_25nsV6_DATA_Uncertainty_AK4PFchs.txt"); 
  // mc jec
  _JECParTxt_MC_L2L3Residual = parm.GetString("JECParTxt_MC_L2L3Residual", "data/jec2016/Spring16_25nsV6_MC_L2L3Residual_AK4PFchs.txt");
  _JECParTxt_MC_L3Absolute   = parm.GetString("JECParTxt_MC_L3Absolute", "data/jec2016/Spring16_25nsV6_MC_L3Absolute_AK4PFchs.txt");
  _JECParTxt_MC_L2Relative   = parm.GetString("JECParTxt_MC_L2Relative", "data/jec2016/Spring16_25nsV6_MC_L2Relative_AK4PFchs.txt");
  _JECParTxt_MC_L1FastJet    = parm.GetString("JECParTxt_MC_L1FastJet", "data/jec2016/Spring16_25nsV6_MC_L1FastJet_AK4PFchs.txt");
  _JECParTxt_MC_Uncertainty  = parm.GetString("JECParTxt_MC_Uncertainty", "data/jec2016/Spring16_25nsV6_MC_Uncertainty_AK4PFchs.txt"); 

  _JERParTxt_Reso_DATA = parm.GetString("JERParTxt_Reso_DATA", "data/jer2016/Spring16_25nsV6_DATA_PtResolution_AK4PFchs.txt");
  _JERParTxt_Reso_MC = parm.GetString("JERParTxt_Reso_MC", "data/jer2016/Spring16_25nsV6_MC_PtResolution_AK4PFchs.txt");
  _JERParTxt_SF_MC = parm.GetString("JERParTxt_SF_MC", "data/jer2016/Spring16_25nsV6_MC_SF_AK4PFchs.txt");

  //==============================================
  // Do simple version of the MET recoil tuning
  //==============================================
  _doRecoil = parm.GetBool("doRecoil", kFALSE);

  if (_doRecoil) {
    _doRecoilUseSmooth = parm.GetBool("doRecoilUseSmooth", kTRUE);
    _doRecoilUseSmoothGraph = parm.GetBool("doRecoilUseSmoothGraph", kTRUE);
    _RecoilInputFileNameData_all = parm.GetString("RecoilInputFileNameData_all", "data/recoil/SingleEMU_Run2016BCD_PromptReco_met_para_study.root"); 
    _RecoilInputFileNameData_mu = parm.GetString("RecoilInputFileNameData_mu", "data/recoil/SingleEMU_Run2016BCD_PromptReco_met_para_study_mu.root"); 
    _RecoilInputFileNameData_el = parm.GetString("RecoilInputFileNameData_el", "data/recoil/SingleEMU_Run2016BCD_PromptReco_met_para_study_el.root"); 
    _RecoilInputFileNameMC_all = parm.GetString("RecoilInputFileNameMC_all", "data/recoil/DYJetsToLL_M50_NoRecoil_met_para_study.root"); 
    _RecoilInputFileNameMC_mu = parm.GetString("RecoilInputFileNameMC_mu", "data/recoil/DYJetsToLL_M50_NoRecoil_met_para_study_mu.root"); 
    _RecoilInputFileNameMC_el = parm.GetString("RecoilInputFileNameMC_el", "data/recoil/DYJetsToLL_M50_NoRecoil_met_para_study_el.root"); 
    _RecoilInputFileNameMCLO_all = parm.GetString("RecoilInputFileNameMCLO_all", "data/recoil/DYJetsToLL_M50_MGMLM_Ext1_NoRecoil_met_para_study.root"); 
    _RecoilInputFileNameMCLO_mu = parm.GetString("RecoilInputFileNameMCLO_mu", "data/recoil/DYJetsToLL_M50_MGMLM_Ext1_NoRecoil_met_para_study_mu.root"); 
    _RecoilInputFileNameMCLO_el = parm.GetString("RecoilInputFileNameMCLO_el", "data/recoil/DYJetsToLL_M50_MGMLM_Ext1_NoRecoil_met_para_study_el.root"); 
    _RecoilInputFileNameGJets_all = parm.GetString("RecoilInputFileNameGJets_all", "data/recoil/SinglePhoton_Run2016BCD_PromptReco_met_para_study_ZSelecLowLPt.root"); 
    _RecoilInputFileNameGJets_mu = parm.GetString("RecoilInputFileNameGJets_mu", "data/recoil/SinglePhoton_Run2016BCD_PromptReco_met_para_study_ZSelecLowLPt_mu.root"); 
    _RecoilInputFileNameGJets_el = parm.GetString("RecoilInputFileNameGJets_el", "data/recoil/SinglePhoton_Run2016BCD_PromptReco_met_para_study_ZSelecLowLPt_el.root"); 
  }

  //==============================================
  // Add efficiency scale factors
  //==============================================  
  _addEffScale = parm.GetBool("addEffScale", kFALSE);
  
  if (_addEffScale){
    _addEffScaleOnData = parm.GetBool("addEffScaleOnData", kFALSE);
    _EffScaleMCVersion = parm.GetString("EffScaleMCVersion", "80xSummer16");
    _EffScaleInputFileName_IdIso_El = parm.GetString("EffScaleInputFileName_IdIso_El", "data/eff/egammaEffi.txt_SF2D.root");
    _EffScaleInputFileName_Trk_El = parm.GetString("EffScaleInputFileName_Trk_El", "data/eff/egammatracking.root");
    _EffScaleInputFileName_IdIso_Mu = parm.GetString("EffScaleInputFileName_IdIso_Mu", "data/eff/muon80x12p9.root");
    _EffScaleInputFileName_Trk_Mu = parm.GetString("EffScaleInputFileName_Trk_Mu", "data/eff/muontrackingsf.root");
  }

  //==============================================
  // Add trigger efficiency scale factors for EMu
  //==============================================  
  _addEMuTrgScale = parm.GetBool("addEMuTrgScale", kFALSE);
  
  if (_addEMuTrgScale || _addEffScale){
    _EffScaleInputFileName_Trg_El = parm.GetString("EffScaleInputFileName_Trg_El", "data/eff/trigereff12p9.root");
    _EffScaleInputFileName_Trg_Mu = parm.GetString("EffScaleInputFileName_Trg_Mu", "data/eff/trigeff_mu.root");
  }


  //==============================================
  // Do GJets skimming
  //==============================================  
  _doGJetsSkim = parm.GetBool("doGJetsSkim", kFALSE);
  _doGJetsSkimAddPhiWeight = parm.GetBool("doGJetsSkimAddPhiWeight", kFALSE);
  _doGJetsSkimAddTrigEff = parm.GetBool("doGJetsSkimAddTrigEff", kFALSE);

  if (_doGJetsSkim) {
    _GJetsSkimInputFileName = parm.GetString("GJetsSkimInputFileName", "data/gjets/study_gjets.root");
    _GJetsSkimRhoWeightInputFileName = parm.GetString("GJetsSkimRhoWeightInputFileName", "data/gjets/get_rho_weight.root");
    if (_doGJetsSkimAddPhiWeight) {
      _GJetsSkimPhiWeightInputFileName = parm.GetString("GJetsSkimPhiWeightInputFileName", "data/gjets/gjet_photon_phi_weight.root");
    }
    if (_doGJetsSkimAddTrigEff) {
      _GJetsSkimTrigEffInputFileName = parm.GetString("GJetsSkimTrigEffInputFileName", "data/gjets/get_ph_trig_eff_fullv2.root");
    }
  }

}




// prepare the trees
bool  prepareTrees() 
{

  // 1.) prepare in put tree:

  // input tree
  _tree_in = (TTree*)_file_in->Get("tree");

  // selection 
  _tree_in->Draw(">>_selected_entries", _selection.c_str(), "entrylist");
  _selected_entries = (TEntryList*)gDirectory->Get("_selected_entries");

  // isData  
  _tree_in->SetBranchAddress("isData",&_isData);

  // check if tree has events
  if (_tree_in->GetEntries()<=0) {
    std::cout << "prepareTrees():: input tree has no event pass selection. Quit." <<std::endl;
    return false;
  }

  // get isData info
  _tree_in->GetEntry(0);

  // set common branches
  _tree_in->SetBranchAddress("run", &_run);
  _tree_in->SetBranchAddress("lumi", &_lumi);
  _tree_in->SetBranchAddress("evt", &_evt);
  _tree_in->SetBranchAddress("rho", &_rho);
  _tree_in->SetBranchAddress("xsec", &_xsec);
  _tree_in->SetBranchAddress("nboostJetAK8Puppi", &_nboostJetAK8Puppi);

  //if(_tree_in->FindBranch("boostJetAK8*")){

  _tree_in->SetBranchAddress("boostJetAK8Puppi_pt", _boostJetAK8Puppi_pt);
  _tree_in->SetBranchAddress("boostJetAK8Puppi_eta", _boostJetAK8Puppi_eta);
  _tree_in->SetBranchAddress("boostJetAK8Puppi_phi", _boostJetAK8Puppi_phi);
  _tree_in->SetBranchAddress("boostJetAK8Puppi_mass", _boostJetAK8Puppi_mass);

  _tree_in->SetBranchAddress("boostJetAK8Puppi_tau1", _boostJetAK8Puppi_tau1);
  _tree_in->SetBranchAddress("boostJetAK8Puppi_tau2", _boostJetAK8Puppi_tau2);
  _tree_in->SetBranchAddress("boostJetAK8Puppi_tau3", _boostJetAK8Puppi_tau3);
  _tree_in->SetBranchAddress("boostJetAK8Puppi_tau4", _boostJetAK8Puppi_tau4);

  _tree_in->SetBranchAddress("boostJetAK8Puppi_btagBOOSTED", _boostJetAK8Puppi_btagBOOSTED);
  _tree_in->SetBranchAddress("boostJetAK8Puppi_softDrop_massCorr", _boostJetAK8Puppi_softDrop_massCorr);
  _tree_in->SetBranchAddress("boostJetAK8Puppi_softDrop_massBare", _boostJetAK8Puppi_softDrop_massBare);


  //}

  if (_doGJetsSkim) {
    _tree_in->SetBranchAddress("gjet_mt", &_gjet_mt);
    _tree_in->SetBranchAddress("gjet_l1_pt", &_gjet_l1_pt);
    _tree_in->SetBranchAddress("gjet_l1_eta", &_gjet_l1_eta);
    _tree_in->SetBranchAddress("gjet_l1_rapidity", &_gjet_l1_rapidity);
    _tree_in->SetBranchAddress("gjet_l1_phi", &_gjet_l1_phi);
    _tree_in->SetBranchAddress("gjet_l1_idCutBased", &_gjet_l1_idCutBased);
    _tree_in->SetBranchAddress("gjet_l1_trigerob_HLTbit", &_gjet_l1_trigerob_HLTbit);
    _tree_in->SetBranchAddress("gjet_l1_trigerob_pt", &_gjet_l1_trigerob_pt);
    _tree_in->SetBranchAddress("gjet_l1_trigerob_eta", &_gjet_l1_trigerob_eta);
    _tree_in->SetBranchAddress("gjet_l1_trigerob_phi", &_gjet_l1_trigerob_phi);
    _tree_in->SetBranchAddress("gjet_l2_pt", &_gjet_l2_pt);
    _tree_in->SetBranchAddress("gjet_l2_phi", &_gjet_l2_phi);
    _tree_in->SetBranchAddress("gjet_l2_sumEt", &_gjet_l2_sumEt);
    _tree_in->SetBranchAddress("gjet_l2_rawPt", &_gjet_l2_rawPt);
    _tree_in->SetBranchAddress("gjet_l2_rawPhi", &_gjet_l2_rawPhi);
    _tree_in->SetBranchAddress("gjet_l2_rawSumEt", &_gjet_l2_rawSumEt);

    if (_doMTUnc && !_doMTUncDummy ) {
      _tree_in->SetBranchAddress("gjet_l2_genPhi", &_gjet_l2_genPhi);
      _tree_in->SetBranchAddress("gjet_l2_genEta", &_gjet_l2_genEta);

      _tree_in->SetBranchAddress("gjet_l2_t1Pt_JetEnUp", &_gjet_l2_t1Pt_JetEnUp);
      _tree_in->SetBranchAddress("gjet_l2_t1Pt_JetEnDn", &_gjet_l2_t1Pt_JetEnDn);
      _tree_in->SetBranchAddress("gjet_l2_t1Phi_JetEnUp", &_gjet_l2_t1Phi_JetEnUp);
      _tree_in->SetBranchAddress("gjet_l2_t1Phi_JetEnDn", &_gjet_l2_t1Phi_JetEnDn);

      _tree_in->SetBranchAddress("gjet_l2_t1Pt_JetResUp", &_gjet_l2_t1Pt_JetResUp);
      _tree_in->SetBranchAddress("gjet_l2_t1Pt_JetResDn", &_gjet_l2_t1Pt_JetResDn);
      _tree_in->SetBranchAddress("gjet_l2_t1Phi_JetResUp", &_gjet_l2_t1Phi_JetResUp);
      _tree_in->SetBranchAddress("gjet_l2_t1Phi_JetResDn", &_gjet_l2_t1Phi_JetResDn);

      _tree_in->SetBranchAddress("gjet_l2_t1Pt_UnclusterUp", &_gjet_l2_t1Pt_UnclusterUp);
      _tree_in->SetBranchAddress("gjet_l2_t1Pt_UnclusterDn", &_gjet_l2_t1Pt_UnclusterDn);
      _tree_in->SetBranchAddress("gjet_l2_t1Phi_UnclusterUp", &_gjet_l2_t1Phi_UnclusterUp);
      _tree_in->SetBranchAddress("gjet_l2_t1Phi_UnclusterDn", &_gjet_l2_t1Phi_UnclusterDn);

      _tree_in->SetBranchAddress("gjet_l2_t1Pt_MuonEnUp", &_gjet_l2_t1Pt_MuonEnUp);
      _tree_in->SetBranchAddress("gjet_l2_t1Pt_MuonEnDn", &_gjet_l2_t1Pt_MuonEnDn);
      _tree_in->SetBranchAddress("gjet_l2_t1Phi_MuonEnUp", &_gjet_l2_t1Phi_MuonEnUp);
      _tree_in->SetBranchAddress("gjet_l2_t1Phi_MuonEnDn", &_gjet_l2_t1Phi_MuonEnDn);

      _tree_in->SetBranchAddress("gjet_l2_t1Pt_TauEnUp", &_gjet_l2_t1Pt_TauEnUp);
      _tree_in->SetBranchAddress("gjet_l2_t1Pt_TauEnDn", &_gjet_l2_t1Pt_TauEnDn);
      _tree_in->SetBranchAddress("gjet_l2_t1Phi_TauEnUp", &_gjet_l2_t1Phi_TauEnUp);
      _tree_in->SetBranchAddress("gjet_l2_t1Phi_TauEnDn", &_gjet_l2_t1Phi_TauEnDn);

      _tree_in->SetBranchAddress("gjet_l2_t1Pt_ElectronEnUp", &_gjet_l2_t1Pt_ElectronEnUp);
      _tree_in->SetBranchAddress("gjet_l2_t1Pt_ElectronEnDn", &_gjet_l2_t1Pt_ElectronEnDn);
      _tree_in->SetBranchAddress("gjet_l2_t1Phi_ElectronEnUp", &_gjet_l2_t1Phi_ElectronEnUp);
      _tree_in->SetBranchAddress("gjet_l2_t1Phi_ElectronEnDn", &_gjet_l2_t1Phi_ElectronEnDn);

      _tree_in->SetBranchAddress("gjet_l2_t1Pt_PhotonEnUp", &_gjet_l2_t1Pt_PhotonEnUp);
      _tree_in->SetBranchAddress("gjet_l2_t1Pt_PhotonEnDn", &_gjet_l2_t1Pt_PhotonEnDn);
      _tree_in->SetBranchAddress("gjet_l2_t1Phi_PhotonEnUp", &_gjet_l2_t1Phi_PhotonEnUp);
      _tree_in->SetBranchAddress("gjet_l2_t1Phi_PhotonEnDn", &_gjet_l2_t1Phi_PhotonEnDn);

    }
    if (_isData){
      _tree_in->SetBranchAddress("PreScale22", &_PreScale22);
      _tree_in->SetBranchAddress("PreScale30", &_PreScale30);
      _tree_in->SetBranchAddress("PreScale36", &_PreScale36);
      _tree_in->SetBranchAddress("PreScale50", &_PreScale50);
      _tree_in->SetBranchAddress("PreScale75", &_PreScale75);
      _tree_in->SetBranchAddress("PreScale90", &_PreScale90);
      _tree_in->SetBranchAddress("PreScale120", &_PreScale120);
      _tree_in->SetBranchAddress("PreScale165", &_PreScale165);
    }
  } 
  else {
    _tree_in->SetBranchAddress("llnunu_mt", &_llnunu_mt);
    _tree_in->SetBranchAddress("llnunu_l1_mass",&_llnunu_l1_mass);
    _tree_in->SetBranchAddress("llnunu_l1_mt", &_llnunu_l1_mt);
    _tree_in->SetBranchAddress("llnunu_l1_pt", &_llnunu_l1_pt);
    _tree_in->SetBranchAddress("llnunu_l1_phi", &_llnunu_l1_phi);
    _tree_in->SetBranchAddress("llnunu_l1_eta", &_llnunu_l1_eta);
    _tree_in->SetBranchAddress("llnunu_l1_deltaPhi", &_llnunu_l1_deltaPhi);
    _tree_in->SetBranchAddress("llnunu_l1_deltaR", &_llnunu_l1_deltaR);
    _tree_in->SetBranchAddress("llnunu_l1_rapidity", &_llnunu_l1_rapidity);

    _tree_in->SetBranchAddress("llnunu_l2_pt", &_llnunu_l2_pt);
    _tree_in->SetBranchAddress("llnunu_l2_phi", &_llnunu_l2_phi);

    _tree_in->SetBranchAddress("llnunu_l1_l1_pt", &_llnunu_l1_l1_pt);
    _tree_in->SetBranchAddress("llnunu_l1_l1_eta", &_llnunu_l1_l1_eta);
    _tree_in->SetBranchAddress("llnunu_l1_l1_phi", &_llnunu_l1_l1_phi);
    _tree_in->SetBranchAddress("llnunu_l1_l1_rapidity", &_llnunu_l1_l1_rapidity);
    _tree_in->SetBranchAddress("llnunu_l1_l1_mass", &_llnunu_l1_l1_mass);
    _tree_in->SetBranchAddress("llnunu_l1_l1_pdgId", &_llnunu_l1_l1_pdgId);
    _tree_in->SetBranchAddress("llnunu_l1_l1_charge", &_llnunu_l1_l1_charge);
    _tree_in->SetBranchAddress("llnunu_l1_l1_ptErr", &_llnunu_l1_l1_ptErr);
    _tree_in->SetBranchAddress("llnunu_l1_l1_eSCeta", &_llnunu_l1_l1_eSCeta);
    _tree_in->SetBranchAddress("llnunu_l1_l1_eSeedXtal", &_llnunu_l1_l1_eSeedXtal);
    _tree_in->SetBranchAddress("llnunu_l1_l1_trigerob_HLTbit", &_llnunu_l1_l1_trigerob_HLTbit);

    _tree_in->SetBranchAddress("llnunu_l1_l2_pt", &_llnunu_l1_l2_pt);
    _tree_in->SetBranchAddress("llnunu_l1_l2_eta", &_llnunu_l1_l2_eta);
    _tree_in->SetBranchAddress("llnunu_l1_l2_phi", &_llnunu_l1_l2_phi);
    _tree_in->SetBranchAddress("llnunu_l1_l2_rapidity", &_llnunu_l1_l2_rapidity);
    _tree_in->SetBranchAddress("llnunu_l1_l2_mass", &_llnunu_l1_l2_mass);
    _tree_in->SetBranchAddress("llnunu_l1_l2_pdgId", &_llnunu_l1_l2_pdgId);
    _tree_in->SetBranchAddress("llnunu_l1_l2_charge", &_llnunu_l1_l2_charge);
    _tree_in->SetBranchAddress("llnunu_l1_l2_ptErr", &_llnunu_l1_l2_ptErr);
    _tree_in->SetBranchAddress("llnunu_l1_l2_eSCeta", &_llnunu_l1_l2_eSCeta);
    _tree_in->SetBranchAddress("llnunu_l1_l2_eSeedXtal", &_llnunu_l1_l2_eSeedXtal);
    _tree_in->SetBranchAddress("llnunu_l1_l2_trigerob_HLTbit", &_llnunu_l1_l2_trigerob_HLTbit);

  }

  // other branches for not light weight tree   
  if (!_useLightTree) {
    std::cout << " Warning: for not _useLightTree, to be implemented." << std::endl;
  }

  // 
  // MC only
  if (!_isData) {
    _tree_in->SetBranchAddress("nTrueInt", &_nTrueInt );
    _tree_in->SetBranchAddress("genWeight",&_genWeight);
  }

  if (!_isData&&!_doGJetsSkim) {
    _tree_in->SetBranchAddress("ngenZ", &_ngenZ);
    _tree_in->SetBranchAddress("genZ_pt", _genZ_pt);
    if(_addZZCorrections&&_isZZ) {
      _tree_in->SetBranchAddress("pdf_x1", &_pdf_x1);
      _tree_in->SetBranchAddress("pdf_x2", &_pdf_x2);
      _tree_in->SetBranchAddress("genZ_eta", _genZ_eta);
      _tree_in->SetBranchAddress("genZ_phi", _genZ_phi);
      _tree_in->SetBranchAddress("genZ_mass", _genZ_mass);
      _tree_in->SetBranchAddress("ngenQ", &_ngenQ);
      _tree_in->SetBranchAddress("genQ_pdgId", _genQ_pdgId);
      _tree_in->SetBranchAddress("ngenLep", &_ngenLep);
      _tree_in->SetBranchAddress("genLep_pt", _genLep_pt);
      _tree_in->SetBranchAddress("genLep_eta", _genLep_eta);
      _tree_in->SetBranchAddress("genLep_phi", _genLep_phi);
      _tree_in->SetBranchAddress("ngenNeu", &_ngenNeu);
      _tree_in->SetBranchAddress("genNeu_pt", _genNeu_pt);
      _tree_in->SetBranchAddress("genNeu_eta", _genNeu_eta);
      _tree_in->SetBranchAddress("genNeu_phi", _genNeu_phi);
    }
  }

  if(_doMTUnc && !_doGJetsSkim && !_doMTUncDummy ){

    _tree_in->SetBranchAddress("llnunu_l2_t1Pt_JetEnUp", &_llnunu_l2_pt_JetEnUp);
    _tree_in->SetBranchAddress("llnunu_l2_t1Pt_JetEnDn", &_llnunu_l2_pt_JetEnDn);
    _tree_in->SetBranchAddress("llnunu_l2_t1Phi_JetEnUp", &_llnunu_l2_phi_JetEnUp);
    _tree_in->SetBranchAddress("llnunu_l2_t1Phi_JetEnDn", &_llnunu_l2_phi_JetEnDn);
    /////
    _tree_in->SetBranchAddress("llnunu_l2_t1Pt_JetResUp", &_llnunu_l2_pt_JetResUp);
    _tree_in->SetBranchAddress("llnunu_l2_t1Pt_JetResDn", &_llnunu_l2_pt_JetResDn);
    _tree_in->SetBranchAddress("llnunu_l2_t1Phi_JetResUp", &_llnunu_l2_phi_JetResUp);
    _tree_in->SetBranchAddress("llnunu_l2_t1Phi_JetResDn", &_llnunu_l2_phi_JetResDn);
    /////
    _tree_in->SetBranchAddress("llnunu_l2_t1Pt_MuonEnUp", &_llnunu_l2_pt_MuonEnUp);
    _tree_in->SetBranchAddress("llnunu_l2_t1Pt_MuonEnDn", &_llnunu_l2_pt_MuonEnDn);
    _tree_in->SetBranchAddress("llnunu_l2_t1Phi_MuonEnUp", &_llnunu_l2_phi_MuonEnUp);
    _tree_in->SetBranchAddress("llnunu_l2_t1Phi_MuonEnDn", &_llnunu_l2_phi_MuonEnDn);
    /////
    _tree_in->SetBranchAddress("llnunu_l2_t1Pt_TauEnUp", &_llnunu_l2_pt_TauEnUp);
    _tree_in->SetBranchAddress("llnunu_l2_t1Pt_TauEnDn", &_llnunu_l2_pt_TauEnDn);
    _tree_in->SetBranchAddress("llnunu_l2_t1Phi_TauEnUp", &_llnunu_l2_phi_TauEnUp);
    _tree_in->SetBranchAddress("llnunu_l2_t1Phi_TauEnDn", &_llnunu_l2_phi_TauEnDn);
    /////
    _tree_in->SetBranchAddress("llnunu_l2_t1Pt_ElectronEnUp", &_llnunu_l2_pt_ElectronEnUp);
    _tree_in->SetBranchAddress("llnunu_l2_t1Pt_ElectronEnDn", &_llnunu_l2_pt_ElectronEnDn);
    _tree_in->SetBranchAddress("llnunu_l2_t1Phi_ElectronEnUp", &_llnunu_l2_phi_ElectronEnUp);
    _tree_in->SetBranchAddress("llnunu_l2_t1Phi_ElectronEnDn", &_llnunu_l2_phi_ElectronEnDn);
    /////
    _tree_in->SetBranchAddress("llnunu_l2_t1Pt_PhotonEnUp", &_llnunu_l2_pt_PhotonEnUp);
    _tree_in->SetBranchAddress("llnunu_l2_t1Pt_PhotonEnDn", &_llnunu_l2_pt_PhotonEnDn);
    _tree_in->SetBranchAddress("llnunu_l2_t1Phi_PhotonEnUp", &_llnunu_l2_phi_PhotonEnUp);
    _tree_in->SetBranchAddress("llnunu_l2_t1Phi_PhotonEnDn", &_llnunu_l2_phi_PhotonEnDn);
    /////
    _tree_in->SetBranchAddress("llnunu_l2_t1Pt_UnclusterUp", &_llnunu_l2_pt_UnclusterUp);
    _tree_in->SetBranchAddress("llnunu_l2_t1Pt_UnclusterDn", &_llnunu_l2_pt_UnclusterDn);
    _tree_in->SetBranchAddress("llnunu_l2_t1Phi_UnclusterUp", &_llnunu_l2_phi_UnclusterUp);
    _tree_in->SetBranchAddress("llnunu_l2_t1Phi_UnclusterDn", &_llnunu_l2_phi_UnclusterDn);


  }

  // 2.) Output tree

  // output tree
  _file_out->cd();
  _tree_out = _tree_in->CloneTree(0);

  // sum of events and weights
  _tree_out->Branch("SumEvents", &_SumEvents, "SumEvents/D");
  _tree_out->Branch("SumWeights", &_SumWeights, "SumWeights/D");

  // keep a copy of old branches
  if (_storeOldBranches && !_doGJetsSkim) {
    _tree_out->Branch("llnunu_mt_old", &_llnunu_mt_old, "llnunu_mt_old/F");
    _tree_out->Branch("llnunu_l1_mass_old", &_llnunu_l1_mass_old, "llnunu_l1_mass_old/F");
    _tree_out->Branch("llnunu_l1_pt_old", &_llnunu_l1_pt_old, "llnunu_l1_pt_old/F");
    _tree_out->Branch("llnunu_l1_phi_old", &_llnunu_l1_phi_old, "llnunu_l1_phi_old/F");
    _tree_out->Branch("llnunu_l1_eta_old", &_llnunu_l1_eta_old, "llnunu_l1_eta_old/F");
    _tree_out->Branch("llnunu_l2_pt_old", &_llnunu_l2_pt_old, "llnunu_l2_pt_old/F");
    _tree_out->Branch("llnunu_l2_phi_old", &_llnunu_l2_phi_old, "llnunu_l2_phi_old/F");
    _tree_out->Branch("llnunu_l1_l1_pt_old", &_llnunu_l1_l1_pt_old, "llnunu_l1_l1_pt_old/F");
    _tree_out->Branch("llnunu_l1_l1_eta_old", &_llnunu_l1_l1_eta_old, "llnunu_l1_l1_eta_old/F");
    _tree_out->Branch("llnunu_l1_l1_phi_old", &_llnunu_l1_l1_phi_old, "llnunu_l1_l1_phi_old/F");
    _tree_out->Branch("llnunu_l1_l1_ptErr_old", &_llnunu_l1_l1_ptErr_old, "llnunu_l1_l1_ptErr_old/F");
    _tree_out->Branch("llnunu_l1_l2_pt_old", &_llnunu_l1_l2_pt_old, "llnunu_l1_l2_pt_old/F");
    _tree_out->Branch("llnunu_l1_l2_eta_old", &_llnunu_l1_l2_eta_old, "llnunu_l1_l2_eta_old/F");
    _tree_out->Branch("llnunu_l1_l2_phi_old", &_llnunu_l1_l2_phi_old, "llnunu_l1_l2_phi_old/F");
    _tree_out->Branch("llnunu_l1_l2_ptErr_old", &_llnunu_l1_l2_ptErr_old, "llnunu_l1_l2_ptErr_old/F");
  }

  // selected the "best" boost jet as hardonic Z/H candidate
  if(_doBoostJet){

    _tree_out->Branch("boostJetAK8_tau21", &_boostJetAK8_tau21, "boostJetAK8_tau21/F");
    _tree_out->Branch("boostJetAK8_softDropMass", &_boostJetAK8_softDropMass, "boostJetAK8_softDropMass/F");

    _tree_out->Branch("boostJetAK8_pt", &_boostJetAK8_pt, "boostJetAK8_pt/F");
    _tree_out->Branch("boostJetAK8_eta", &_boostJetAK8_eta, "boostJetAK8_eta/F");
    _tree_out->Branch("boostJetAK8_phi", &_boostJetAK8_phi, "boostJetAK8_phi/F");
    _tree_out->Branch("boostJetAK8_mass", &_boostJetAK8_mass, "boostJetAK8_mass/F");

    _tree_out->Branch("boostJetAK8_btagBOOSTED", &_boostJetAK8_btagBOOSTED, "boostJetAK8_btagBOOSTED/F");
    _tree_out->Branch("mTfull",&_mTfull,"mTfull/F");

  }

  // for add SM qqZZ QCD/EW corrections
  if (_addZZCorrections&&_isZZ){
    _tree_out->Branch("ZZEwkCorrWeight", &_ZZEwkCorrWeight, "ZZEwkCorrWeight/F");
    _tree_out->Branch("ZZEwkCorrWeight_up", &_ZZEwkCorrWeight_up, "ZZEwkCorrWeight_up/F");
    _tree_out->Branch("ZZEwkCorrWeight_dn", &_ZZEwkCorrWeight_dn, "ZZEwkCorrWeight_dn/F");
    _tree_out->Branch("ZZQcdCorrWeight", &_ZZQcdCorrWeight, "ZZQcdCorrWeight/F");
    _tree_out->Branch("ZZQcdCorrWeight_up", &_ZZQcdCorrWeight_up, "ZZQcdCorrWeight_up/F");
    _tree_out->Branch("ZZQcdCorrWeight_dn", &_ZZQcdCorrWeight_dn, "ZZQcdCorrWeight_dn/F");
  }

  // GJets Skim
  if (_doGJetsSkim){
    _tree_out->Branch("GJetsRhoWeight", &_GJetsRhoWeight, "GJetsRhoWeight/F");
    _tree_out->Branch("GJetsZPtWeight", &_GJetsZPtWeight, "GJetsZPtWeight/F");
    _tree_out->Branch("GJetsZPtWeightEl", &_GJetsZPtWeightEl, "GJetsZPtWeightEl/F");
    _tree_out->Branch("GJetsZPtWeightMu", &_GJetsZPtWeightMu, "GJetsZPtWeightMu/F");
    _tree_out->Branch("GJetsZPtWeightLowLPt", &_GJetsZPtWeightLowLPt, "GJetsZPtWeightLowLPt/F");
    _tree_out->Branch("GJetsZPtWeightLowLPtEl", &_GJetsZPtWeightLowLPtEl, "GJetsZPtWeightLowLPtEl/F");
    _tree_out->Branch("GJetsZPtWeightLowLPtMu", &_GJetsZPtWeightLowLPtMu, "GJetsZPtWeightLowLPtMu/F");
    _tree_out->Branch("GJetsZPtWeight_up", &_GJetsZPtWeight_up, "GJetsZPtWeight_up/F");
    _tree_out->Branch("GJetsZPtWeight_dn", &_GJetsZPtWeight_dn, "GJetsZPtWeight_dn/F");
    _tree_out->Branch("GJetsZPtWeightMu_up", &_GJetsZPtWeightMu_up, "GJetsZPtWeightMu_up/F");
    _tree_out->Branch("GJetsZPtWeightMu_dn", &_GJetsZPtWeightMu_dn, "GJetsZPtWeightMu_dn/F");
    _tree_out->Branch("GJetsZPtWeightEl_up", &_GJetsZPtWeightEl_up, "GJetsZPtWeightEl_up/F");
    _tree_out->Branch("GJetsZPtWeightEl_dn", &_GJetsZPtWeightEl_dn, "GJetsZPtWeightEl_dn/F");
    _tree_out->Branch("llnunu_mt", &_llnunu_mt, "llnunu_mt/F");
    _tree_out->Branch("llnunu_l1_mass", &_llnunu_l1_mass, "llnunu_l1_mass/F");
    _tree_out->Branch("llnunu_l1_pt", &_llnunu_l1_pt, "llnunu_l1_pt/F");
    _tree_out->Branch("llnunu_l1_phi", &_llnunu_l1_phi, "llnunu_l1_phi/F");
    _tree_out->Branch("llnunu_l1_eta", &_llnunu_l1_eta, "llnunu_l1_eta/F");
    _tree_out->Branch("llnunu_l1_rapidity", &_llnunu_l1_rapidity, "llnunu_l1_rapidity/F");
    _tree_out->Branch("llnunu_l1_trigerob_HLTbit", &_llnunu_l1_trigerob_HLTbit, "llnunu_l1_trigerob_HLTbit/I");
    _tree_out->Branch("llnunu_l1_trigerob_pt", &_llnunu_l1_trigerob_pt, "llnunu_l1_trigerob_pt/F");
    _tree_out->Branch("llnunu_l1_trigerob_eta", &_llnunu_l1_trigerob_eta, "llnunu_l1_trigerob_eta/F");
    _tree_out->Branch("llnunu_l1_trigerob_phi", &_llnunu_l1_trigerob_phi, "llnunu_l1_trigerob_phi/F");
    _tree_out->Branch("llnunu_l2_pt", &_llnunu_l2_pt, "llnunu_l2_pt/F");
    _tree_out->Branch("llnunu_l2_phi", &_llnunu_l2_phi, "llnunu_l2_phi/F");
    _tree_out->Branch("llnunu_l2_sumEt", &_llnunu_l2_sumEt, "llnunu_l2_sumEt/F");
    _tree_out->Branch("llnunu_l2_rawPt", &_llnunu_l2_rawPt, "llnunu_l2_rawPt/F");
    _tree_out->Branch("llnunu_l2_rawPhi", &_llnunu_l2_rawPhi, "llnunu_l2_rawPhi/F");
    _tree_out->Branch("llnunu_l2_rawSumEt", &_llnunu_l2_rawSumEt, "llnunu_l2_rawSumEt/F");
    _tree_out->Branch("llnunu_l1_l1_pt", &_llnunu_l1_l1_pt, "llnunu_l1_l1_pt/F");
    _tree_out->Branch("llnunu_l1_l1_eta", &_llnunu_l1_l1_eta, "llnunu_l1_l1_eta/F");
    _tree_out->Branch("llnunu_l1_l1_pdgId", &_llnunu_l1_l1_pdgId, "llnunu_l1_l1_pdgId/I");
    _tree_out->Branch("llnunu_l1_l2_pt", &_llnunu_l1_l2_pt, "llnunu_l1_l2_pt/F");
    _tree_out->Branch("llnunu_l1_l2_eta", &_llnunu_l1_l2_eta, "llnunu_l1_l2_eta/F");
    _tree_out->Branch("llnunu_l1_l2_pdgId", &_llnunu_l1_l2_pdgId, "llnunu_l1_l2_pdgId/I");
    _tree_out->Branch("llnunu_l1_l1_highPtID", &_llnunu_l1_l1_highPtID, "llnunu_l1_l1_highPtID/F");
    _tree_out->Branch("llnunu_l1_l2_highPtID", &_llnunu_l1_l2_highPtID, "llnunu_l1_l2_highPtID/F");
    _tree_out->Branch("llnunu_mt_el", &_llnunu_mt_el, "llnunu_mt_el/F");
    _tree_out->Branch("llnunu_mt_mu", &_llnunu_mt_mu, "llnunu_mt_mu/F");
    _tree_out->Branch("llnunu_l1_mass_el", &_llnunu_l1_mass_el, "llnunu_l1_mass_el/F");
    _tree_out->Branch("llnunu_l1_mass_mu", &_llnunu_l1_mass_mu, "llnunu_l1_mass_mu/F");
    _tree_out->Branch("llnunu_l2_pt_el", &_llnunu_l2_pt_el, "llnunu_l2_pt_el/F");
    _tree_out->Branch("llnunu_l2_phi_el", &_llnunu_l2_phi_el, "llnunu_l2_phi_el/F");
    _tree_out->Branch("llnunu_l2_pt_mu", &_llnunu_l2_pt_mu, "llnunu_l2_pt_mu/F");
    _tree_out->Branch("llnunu_l2_phi_mu", &_llnunu_l2_phi_mu, "llnunu_l2_phi_mu/F");
    if (!_isData) {
      _tree_out->Branch("llnunu_l2_genPhi", &_llnunu_l2_genPhi, "llnunu_l2_genPhi/F");
      _tree_out->Branch("llnunu_l2_genEta", &_llnunu_l2_genEta, "llnunu_l2_genEta/F");
    }
    if (_isData){
      _tree_out->Branch("GJetsPreScaleWeight", &_GJetsPreScaleWeight, "GJetsPreScaleWeight/F");
    }
    if (_doGJetsSkimAddPhiWeight) {
      _tree_out->Branch("GJetsPhiWeight", &_GJetsPhiWeight, "GJetsPhiWeight/F");
    }
    if (_doGJetsSkimAddTrigEff) {
      _tree_out->Branch("GJetsTrigEff", &_GJetsTrigEff, "GJetsTrigEff/F");
    }
    if (!_storeOldBranches) {
      _tree_out->SetBranchStatus("gjet_*", 0);
      _tree_out->SetBranchStatus("PreScale*", 0);
    }
  }

  // store alternative MT
  if(_doMTUnc){

    _tree_out->Branch("llnunu_mt_JetEnUp", &_llnunu_mt_JetEnUp, "llnunu_mt_JetEnUp/F");
    _tree_out->Branch("llnunu_mt_JetEnDn", &_llnunu_mt_JetEnDn, "llnunu_mt_JetEnDn/F");
    _tree_out->Branch("llnunu_mt_JetResUp", &_llnunu_mt_JetResUp, "llnunu_mt_JetResUp/F");
    _tree_out->Branch("llnunu_mt_JetResDn", &_llnunu_mt_JetResDn, "llnunu_mt_JetResDn/F");
    _tree_out->Branch("llnunu_mt_MuonEnUp", &_llnunu_mt_MuonEnUp, "llnunu_mt_MuonEnUp/F");
    _tree_out->Branch("llnunu_mt_MuonEnDn", &_llnunu_mt_MuonEnDn, "llnunu_mt_MuonEnDn/F");
    _tree_out->Branch("llnunu_mt_ElectronEnUp", &_llnunu_mt_ElectronEnUp, "llnunu_mt_ElectronEnUp/F");
    _tree_out->Branch("llnunu_mt_ElectronEnDn", &_llnunu_mt_ElectronEnDn, "llnunu_mt_ElectronEnDn/F");
    _tree_out->Branch("llnunu_mt_TauEnUp", &_llnunu_mt_TauEnUp, "llnunu_mt_TauEnUp/F");
    _tree_out->Branch("llnunu_mt_TauEnDn", &_llnunu_mt_TauEnDn, "llnunu_mt_TauEnDn/F");
    _tree_out->Branch("llnunu_mt_PhotonEnUp", &_llnunu_mt_PhotonEnUp, "llnunu_mt_PhotonEnUp/F");
    _tree_out->Branch("llnunu_mt_PhotonEnDn", &_llnunu_mt_PhotonEnDn, "llnunu_mt_PhotonEnDn/F");
    _tree_out->Branch("llnunu_mt_UnclusterUp", &_llnunu_mt_UnclusterUp, "llnunu_mt_UnclusterUp/F");
    _tree_out->Branch("llnunu_mt_UnclusterDn", &_llnunu_mt_UnclusterDn, "llnunu_mt_UnclusterDn/F");

    if(_doGJetsSkim){

	_tree_out->Branch("llnunu_mt_el_JetEnUp", &_llnunu_mt_el_JetEnUp, "llnunu_mt_el_JetEnUp/F");
    	_tree_out->Branch("llnunu_mt_el_JetEnDn", &_llnunu_mt_el_JetEnDn, "llnunu_mt_el_JetEnDn/F");
    	_tree_out->Branch("llnunu_mt_el_JetResUp", &_llnunu_mt_el_JetResUp, "llnunu_mt_el_JetResUp/F");
    	_tree_out->Branch("llnunu_mt_el_JetResDn", &_llnunu_mt_el_JetResDn, "llnunu_mt_el_JetResDn/F");
    	_tree_out->Branch("llnunu_mt_el_MuonEnUp", &_llnunu_mt_el_MuonEnUp, "llnunu_mt_el_MuonEnUp/F");
    	_tree_out->Branch("llnunu_mt_el_MuonEnDn", &_llnunu_mt_el_MuonEnDn, "llnunu_mt_el_MuonEnDn/F");
    	_tree_out->Branch("llnunu_mt_el_ElectronEnUp", &_llnunu_mt_el_ElectronEnUp, "llnunu_mt_el_ElectronEnUp/F");
    	_tree_out->Branch("llnunu_mt_el_ElectronEnDn", &_llnunu_mt_el_ElectronEnDn, "llnunu_mt_el_ElectronEnDn/F");
    	_tree_out->Branch("llnunu_mt_el_TauEnUp", &_llnunu_mt_el_TauEnUp, "llnunu_mt_el_TauEnUp/F");
    	_tree_out->Branch("llnunu_mt_el_TauEnDn", &_llnunu_mt_el_TauEnDn, "llnunu_mt_el_TauEnDn/F");
    	_tree_out->Branch("llnunu_mt_el_PhotonEnUp", &_llnunu_mt_el_PhotonEnUp, "llnunu_mt_el_PhotonEnUp/F");
    	_tree_out->Branch("llnunu_mt_el_PhotonEnDn", &_llnunu_mt_el_PhotonEnDn, "llnunu_mt_el_PhotonEnDn/F");
    	_tree_out->Branch("llnunu_mt_el_UnclusterUp", &_llnunu_mt_el_UnclusterUp, "llnunu_mt_el_UnclusterUp/F");
    	_tree_out->Branch("llnunu_mt_el_UnclusterDn", &_llnunu_mt_el_UnclusterDn, "llnunu_mt_el_UnclusterDn/F");

    	_tree_out->Branch("llnunu_mt_mu_JetEnUp", &_llnunu_mt_mu_JetEnUp, "llnunu_mt_mu_JetEnUp/F");
    	_tree_out->Branch("llnunu_mt_mu_JetEnDn", &_llnunu_mt_mu_JetEnDn, "llnunu_mt_mu_JetEnDn/F");
    	_tree_out->Branch("llnunu_mt_mu_JetResUp", &_llnunu_mt_mu_JetResUp, "llnunu_mt_mu_JetResUp/F");
    	_tree_out->Branch("llnunu_mt_mu_JetResDn", &_llnunu_mt_mu_JetResDn, "llnunu_mt_mu_JetResDn/F");
    	_tree_out->Branch("llnunu_mt_mu_MuonEnUp", &_llnunu_mt_mu_MuonEnUp, "llnunu_mt_mu_MuonEnUp/F");
    	_tree_out->Branch("llnunu_mt_mu_MuonEnDn", &_llnunu_mt_mu_MuonEnDn, "llnunu_mt_mu_MuonEnDn/F");
    	_tree_out->Branch("llnunu_mt_mu_ElectronEnUp", &_llnunu_mt_mu_ElectronEnUp, "llnunu_mt_mu_ElectronEnUp/F");
    	_tree_out->Branch("llnunu_mt_mu_ElectronEnDn", &_llnunu_mt_mu_ElectronEnDn, "llnunu_mt_mu_ElectronEnDn/F");
    	_tree_out->Branch("llnunu_mt_mu_TauEnUp", &_llnunu_mt_mu_TauEnUp, "llnunu_mt_mu_TauEnUp/F");
    	_tree_out->Branch("llnunu_mt_mu_TauEnDn", &_llnunu_mt_mu_TauEnDn, "llnunu_mt_mu_TauEnDn/F");
    	_tree_out->Branch("llnunu_mt_mu_PhotonEnUp", &_llnunu_mt_mu_PhotonEnUp, "llnunu_mt_mu_PhotonEnUp/F");
    	_tree_out->Branch("llnunu_mt_mu_PhotonEnDn", &_llnunu_mt_mu_PhotonEnDn, "llnunu_mt_mu_PhotonEnDn/F");
    	_tree_out->Branch("llnunu_mt_mu_UnclusterUp", &_llnunu_mt_mu_UnclusterUp, "llnunu_mt_mu_UnclusterUp/F");
    	_tree_out->Branch("llnunu_mt_mu_UnclusterDn", &_llnunu_mt_mu_UnclusterDn, "llnunu_mt_mu_UnclusterDn/F");

    }
  }

  // Recoil uncertainties
  if (_doRecoil) {
    _tree_out->Branch("llnunu_l2_pt_RecoilUp", &_llnunu_l2_pt_RecoilUp, "llnunu_l2_pt_RecoilUp/F");
    _tree_out->Branch("llnunu_l2_pt_RecoilDn", &_llnunu_l2_pt_RecoilDn, "llnunu_l2_pt_RecoilDn/F");
    _tree_out->Branch("llnunu_l2_phi_RecoilUp", &_llnunu_l2_phi_RecoilUp, "llnunu_l2_phi_RecoilUp/F");
    _tree_out->Branch("llnunu_l2_phi_RecoilDn", &_llnunu_l2_phi_RecoilDn, "llnunu_l2_phi_RecoilDn/F");
    _tree_out->Branch("llnunu_mt_RecoilUp", &_llnunu_mt_RecoilUp, "llnunu_mt_RecoilUp/F");
    _tree_out->Branch("llnunu_mt_RecoilDn", &_llnunu_mt_RecoilDn, "llnunu_mt_RecoilDn/F");
    
    if (_doGJetsSkim){
      _tree_out->Branch("llnunu_l2_pt_el_RecoilUp", &_llnunu_l2_pt_el_RecoilUp, "llnunu_l2_pt_el_RecoilUp/F");
      _tree_out->Branch("llnunu_l2_pt_el_RecoilDn", &_llnunu_l2_pt_el_RecoilDn, "llnunu_l2_pt_el_RecoilDn/F");
      _tree_out->Branch("llnunu_l2_phi_el_RecoilUp", &_llnunu_l2_phi_el_RecoilUp, "llnunu_l2_phi_el_RecoilUp/F");
      _tree_out->Branch("llnunu_l2_phi_el_RecoilDn", &_llnunu_l2_phi_el_RecoilDn, "llnunu_l2_phi_el_RecoilDn/F");
      _tree_out->Branch("llnunu_mt_el_RecoilUp", &_llnunu_mt_el_RecoilUp, "llnunu_mt_el_RecoilUp/F");
      _tree_out->Branch("llnunu_mt_el_RecoilDn", &_llnunu_mt_el_RecoilDn, "llnunu_mt_el_RecoilDn/F");
      _tree_out->Branch("llnunu_l2_pt_mu_RecoilUp", &_llnunu_l2_pt_mu_RecoilUp, "llnunu_l2_pt_mu_RecoilUp/F");
      _tree_out->Branch("llnunu_l2_pt_mu_RecoilDn", &_llnunu_l2_pt_mu_RecoilDn, "llnunu_l2_pt_mu_RecoilDn/F");
      _tree_out->Branch("llnunu_l2_phi_mu_RecoilUp", &_llnunu_l2_phi_mu_RecoilUp, "llnunu_l2_phi_mu_RecoilUp/F");
      _tree_out->Branch("llnunu_l2_phi_mu_RecoilDn", &_llnunu_l2_phi_mu_RecoilDn, "llnunu_l2_phi_mu_RecoilDn/F");
      _tree_out->Branch("llnunu_mt_mu_RecoilUp", &_llnunu_mt_mu_RecoilUp, "llnunu_mt_mu_RecoilUp/F");
      _tree_out->Branch("llnunu_mt_mu_RecoilDn", &_llnunu_mt_mu_RecoilDn, "llnunu_mt_mu_RecoilDn/F");

    }
  }

  // _storeErr 
  if(_tree_out->FindBranch("llnunu_l2_t1*_*")) _tree_out->SetBranchStatus("llnunu_l2_t1*_*", 0);
  if(_tree_out->FindBranch("*_err")) _tree_out->SetBranchStatus("*_err", 0);
  if(_tree_out->FindBranch("*_up")) _tree_out->SetBranchStatus("*_up", 0);
  if(_tree_out->FindBranch("*_dn")) _tree_out->SetBranchStatus("*_dn", 0);
  if (_storeErr) {
    if(_tree_out->FindBranch("llnunu_l2_t1P*_*")) _tree_out->SetBranchStatus("llnunu_l2_t1P*_*", 1);
    if(_tree_out->FindBranch("*_err")) _tree_out->SetBranchStatus("*_err", 1);
    if(_tree_out->FindBranch("*_up")) _tree_out->SetBranchStatus("*_up", 1);
    if(_tree_out->FindBranch("*_dn")) _tree_out->SetBranchStatus("*_dn", 1);

  }
  
  // store HLT flags, only if not data
  if (_removeHLTFlag && !_isData ){
    _tree_out->SetBranchStatus("HLT_*", 0);
  }
  // store MET flags
  if (_removeMETFlag){
    _tree_out->SetBranchStatus("Flag_*", 0);
  }
 

  return true;

}

// store Old Branches
void storeOldBranches()
{
  _llnunu_mt_old = _llnunu_mt;
  _llnunu_l1_mass_old = _llnunu_l1_mass;
  _llnunu_l1_pt_old = _llnunu_l1_pt;
  _llnunu_l1_phi_old = _llnunu_l1_phi;
  _llnunu_l1_eta_old = _llnunu_l1_eta;
  _llnunu_l2_pt_old = _llnunu_l2_pt;
  _llnunu_l2_phi_old = _llnunu_l2_phi;
  _llnunu_l1_l1_pt_old = _llnunu_l1_l1_pt;
  _llnunu_l1_l1_eta_old = _llnunu_l1_l1_eta;
  _llnunu_l1_l1_phi_old = _llnunu_l1_l1_phi;
  _llnunu_l1_l1_ptErr_old = _llnunu_l1_l1_ptErr;
  _llnunu_l1_l2_pt_old = _llnunu_l1_l2_pt;
  _llnunu_l1_l2_eta_old = _llnunu_l1_l2_eta;
  _llnunu_l1_l2_phi_old = _llnunu_l1_l2_phi;
  _llnunu_l1_l2_ptErr_old = _llnunu_l1_l2_ptErr;
}

// prepare inputs for pu weights
void preparePUWeights()
{
  // for each puWeight, read input weight hist and create branch
  for (int i=0; i<(int)_PUTags.size(); i++) {
    sprintf(name, "%s/%s", _PUInputDir.c_str(), _PUInputFileNames.at(i).c_str());
    _PUFiles.push_back(new TFile(name));
    _PUHists.push_back((TH1D*)_PUFiles.at(i)->Get(_PUWeightHistName.c_str()));
    _PUWeights.push_back(new Float_t(1.0));
    sprintf(name, "puWeight%s", _PUTags.at(i).c_str());
    sprintf(name1, "puWeight%s/F", _PUTags.at(i).c_str());
    _tree_out->Branch(name, _PUWeights.at(i),name1);
  }

}

// add more pileup weights
void addPUWeights() 
{
  // for each puWeight, get the weight
  for (int i=0; i<(int)_PUTags.size(); i++){
    *(_PUWeights.at(i)) = _PUHists.at(i)->GetBinContent(_PUHists.at(i)->FindBin(_nTrueInt));
    // protect over big PU weight
    if ( *(_PUWeights.at(i))>_PUWeightProtectionCut ) *(_PUWeights.at(i)) = 1.0; 
  }
}


// prepare inputs for muon re-calib
void prepareMuonPtRecalib()
{
  if (_isData) _muCalib = new KalmanMuonCalibrator(_MuonPtRecalibInputForData.c_str());
  else _muCalib = new KalmanMuonCalibrator(_MuonPtRecalibInputForMC.c_str());
}


// do muon re-calib
void doMuonPtRecalib()
{
  if (abs(_llnunu_l1_l1_pdgId)==13&&abs(_llnunu_l1_l2_pdgId)==13 && _llnunu_l1_l1_pt>2. && _llnunu_l1_l1_pt<200. ) {
    _llnunu_l1_l1_pt = (Float_t)_muCalib->getCorrectedPt(double(_llnunu_l1_l1_pt), double(_llnunu_l1_l1_eta), double(_llnunu_l1_l1_phi), double(_llnunu_l1_l1_charge));
    _llnunu_l1_l2_pt = (Float_t)_muCalib->getCorrectedPt(double(_llnunu_l1_l2_pt), double(_llnunu_l1_l2_eta), double(_llnunu_l1_l2_phi), double(_llnunu_l1_l2_charge));
    if (!_isData) {
      _llnunu_l1_l1_pt = (Float_t)_muCalib->smear(double(_llnunu_l1_l1_pt), double(_llnunu_l1_l1_eta));
      _llnunu_l1_l2_pt = (Float_t)_muCalib->smear(double(_llnunu_l1_l2_pt), double(_llnunu_l1_l2_eta));
    }
    _llnunu_l1_l1_ptErr = _llnunu_l1_l1_pt*(Float_t)_muCalib->getCorrectedError(double(_llnunu_l1_l1_pt), double(_llnunu_l1_l1_eta), double(_llnunu_l1_l1_ptErr/_llnunu_l1_l1_pt));
    _llnunu_l1_l2_ptErr = _llnunu_l1_l2_pt*(Float_t)_muCalib->getCorrectedError(double(_llnunu_l1_l2_pt), double(_llnunu_l1_l2_eta), double(_llnunu_l1_l2_ptErr/_llnunu_l1_l2_pt));
    TLorentzVector l1v, l2v;
    l1v.SetPtEtaPhiM(_llnunu_l1_l1_pt, _llnunu_l1_l1_eta, _llnunu_l1_l1_phi, _llnunu_l1_l1_mass);
    l2v.SetPtEtaPhiM(_llnunu_l1_l2_pt, _llnunu_l1_l2_eta, _llnunu_l1_l2_phi, _llnunu_l1_l2_mass);
    TLorentzVector zv = l1v+l2v;
    _llnunu_l1_l1_rapidity = (Float_t)l1v.Rapidity(); 
    _llnunu_l1_l2_rapidity = (Float_t)l2v.Rapidity();
    _llnunu_l1_pt = (Float_t)zv.Pt();
    _llnunu_l1_eta = (Float_t)zv.Eta();
    _llnunu_l1_phi = (Float_t)zv.Phi();
    _llnunu_l1_rapidity = (Float_t)zv.Rapidity();
    _llnunu_l1_deltaPhi = (Float_t)l1v.DeltaPhi(l2v);
    _llnunu_l1_deltaR = (Float_t)l1v.DeltaR(l2v);
    _llnunu_l1_mt = (Float_t)zv.Mt();
    _llnunu_l1_mass = (Float_t)zv.M();

    /*
    TVector2 vec_met(_llnunu_l2_pt*cos(_llnunu_l2_phi), _llnunu_l2_pt*sin(_llnunu_l2_phi));
    Float_t et1 = TMath::Sqrt(_llnunu_l1_mass*_llnunu_l1_mass + _llnunu_l1_pt*_llnunu_l1_pt);
    Float_t et2 = TMath::Sqrt(_llnunu_l1_mass*_llnunu_l1_mass + _llnunu_l2_pt*_llnunu_l2_pt);
    _llnunu_mt = TMath::Sqrt(2.0*_llnunu_l1_mass*_llnunu_l1_mass+2.0*(et1*et2
               -_llnunu_l1_pt*cos(_llnunu_l1_phi)*_llnunu_l2_pt*cos(_llnunu_l2_phi)
               -_llnunu_l1_pt*sin(_llnunu_l1_phi)*_llnunu_l2_pt*sin(_llnunu_l2_phi)));
    */    
    _llnunu_mt = MTCalc(_llnunu_l2_pt,_llnunu_l2_phi);

  }

}

void doMTUnc(){

    if(!_doMTUncDummy){

        _llnunu_mt = MTCalc(_llnunu_l2_pt,_llnunu_l2_phi);

        float mt_JetEn_col[5] = {
               _llnunu_mt,
               MTCalc(_llnunu_l2_pt_JetEnUp,_llnunu_l2_phi_JetEnUp),
               MTCalc(_llnunu_l2_pt_JetEnUp,_llnunu_l2_phi_JetEnDn),
               MTCalc(_llnunu_l2_pt_JetEnDn,_llnunu_l2_phi_JetEnUp),
               MTCalc(_llnunu_l2_pt_JetEnDn,_llnunu_l2_phi_JetEnDn)
              };

      	_llnunu_mt_JetEnUp = TMath::MaxElement(5, mt_JetEn_col);
      	_llnunu_mt_JetEnDn = TMath::MinElement(5, mt_JetEn_col);

        float mt_JetRes_col[5] = {
               _llnunu_mt,
               MTCalc(_llnunu_l2_pt_JetResUp,_llnunu_l2_phi_JetResUp),
               MTCalc(_llnunu_l2_pt_JetResUp,_llnunu_l2_phi_JetResDn),
               MTCalc(_llnunu_l2_pt_JetResDn,_llnunu_l2_phi_JetResUp),
               MTCalc(_llnunu_l2_pt_JetResDn,_llnunu_l2_phi_JetResDn)
              };

        _llnunu_mt_JetResUp = TMath::MaxElement(5, mt_JetRes_col);
        _llnunu_mt_JetResDn = TMath::MinElement(5, mt_JetRes_col);


        float mt_MuonEn_col[5] = {
               _llnunu_mt,
               MTCalc(_llnunu_l2_pt_MuonEnUp,_llnunu_l2_phi_MuonEnUp),
               MTCalc(_llnunu_l2_pt_MuonEnUp,_llnunu_l2_phi_MuonEnDn),
               MTCalc(_llnunu_l2_pt_MuonEnDn,_llnunu_l2_phi_MuonEnUp),
               MTCalc(_llnunu_l2_pt_MuonEnDn,_llnunu_l2_phi_MuonEnDn)
              };

        _llnunu_mt_MuonEnUp = TMath::MaxElement(5, mt_MuonEn_col);
        _llnunu_mt_MuonEnDn = TMath::MinElement(5, mt_MuonEn_col);


        float mt_ElectronEn_col[5] = {
               _llnunu_mt,
               MTCalc(_llnunu_l2_pt_ElectronEnUp,_llnunu_l2_phi_ElectronEnUp),
               MTCalc(_llnunu_l2_pt_ElectronEnUp,_llnunu_l2_phi_ElectronEnDn),
               MTCalc(_llnunu_l2_pt_ElectronEnDn,_llnunu_l2_phi_ElectronEnUp),
               MTCalc(_llnunu_l2_pt_ElectronEnDn,_llnunu_l2_phi_ElectronEnDn)
              };

        _llnunu_mt_ElectronEnUp = TMath::MaxElement(5, mt_ElectronEn_col);
        _llnunu_mt_ElectronEnDn = TMath::MinElement(5, mt_ElectronEn_col);

        float mt_PhotonEn_col[5] = {
               _llnunu_mt,
               MTCalc(_llnunu_l2_pt_PhotonEnUp,_llnunu_l2_phi_PhotonEnUp),
               MTCalc(_llnunu_l2_pt_PhotonEnUp,_llnunu_l2_phi_PhotonEnDn),
               MTCalc(_llnunu_l2_pt_PhotonEnDn,_llnunu_l2_phi_PhotonEnUp),
               MTCalc(_llnunu_l2_pt_PhotonEnDn,_llnunu_l2_phi_PhotonEnDn)
              };

        _llnunu_mt_PhotonEnUp = TMath::MaxElement(5, mt_PhotonEn_col);
        _llnunu_mt_PhotonEnDn = TMath::MinElement(5, mt_PhotonEn_col);


        float mt_TauEn_col[5] = {
               _llnunu_mt,
               MTCalc(_llnunu_l2_pt_TauEnUp,_llnunu_l2_phi_TauEnUp),
               MTCalc(_llnunu_l2_pt_TauEnUp,_llnunu_l2_phi_TauEnDn),
               MTCalc(_llnunu_l2_pt_TauEnDn,_llnunu_l2_phi_TauEnUp),
               MTCalc(_llnunu_l2_pt_TauEnDn,_llnunu_l2_phi_TauEnDn)
              };

        _llnunu_mt_TauEnUp = TMath::MaxElement(5, mt_TauEn_col);
        _llnunu_mt_TauEnDn = TMath::MinElement(5, mt_TauEn_col);

        float mt_Uncluster_col[5] = {
               _llnunu_mt,
               MTCalc(_llnunu_l2_pt_UnclusterUp,_llnunu_l2_phi_UnclusterUp),
               MTCalc(_llnunu_l2_pt_UnclusterUp,_llnunu_l2_phi_UnclusterDn),
               MTCalc(_llnunu_l2_pt_UnclusterDn,_llnunu_l2_phi_UnclusterUp),
               MTCalc(_llnunu_l2_pt_UnclusterDn,_llnunu_l2_phi_UnclusterDn)
              };

        _llnunu_mt_UnclusterUp = TMath::MaxElement(5, mt_Uncluster_col);
        _llnunu_mt_UnclusterDn = TMath::MinElement(5, mt_Uncluster_col);

    }
    else{
	_llnunu_mt_JetEnUp = _llnunu_mt;
        _llnunu_mt_JetEnDn = _llnunu_mt;
        _llnunu_mt_JetResUp = _llnunu_mt;
        _llnunu_mt_JetResDn = _llnunu_mt;
        _llnunu_mt_MuonEnUp = _llnunu_mt;
        _llnunu_mt_MuonEnDn = _llnunu_mt;
        _llnunu_mt_ElectronEnUp = _llnunu_mt;
        _llnunu_mt_ElectronEnDn = _llnunu_mt;
        _llnunu_mt_PhotonEnUp = _llnunu_mt;
        _llnunu_mt_PhotonEnDn = _llnunu_mt;
        _llnunu_mt_TauEnUp = _llnunu_mt;
        _llnunu_mt_TauEnDn = _llnunu_mt;
        _llnunu_mt_UnclusterUp = _llnunu_mt;
        _llnunu_mt_UnclusterDn = _llnunu_mt;

    }

}

void doMTUncEl(){

      if(!_doMTUncDummy){

        _llnunu_mt_el = MTCalcEl(_llnunu_l2_pt_el,_llnunu_l2_phi_el);

        float Up_pt = _gjet_l2_t1Pt_JetEnUp/_gjet_l2_pt;
        float Up_phi = _gjet_l2_t1Phi_JetEnUp/_gjet_l2_phi; 
        float Dn_pt = _gjet_l2_t1Pt_JetEnDn/_gjet_l2_pt;
        float Dn_phi = _gjet_l2_t1Phi_JetEnDn/_gjet_l2_phi;

        float mt_col[5];
        
        mt_col[0] = _llnunu_mt_el;
        mt_col[1] = MTCalcEl(Up_pt*_llnunu_l2_pt_el, Up_phi*_llnunu_l2_phi_el);
        mt_col[2] = MTCalcEl(Up_pt*_llnunu_l2_pt_el, Dn_phi*_llnunu_l2_phi_el);
        mt_col[3] = MTCalcEl(Dn_pt*_llnunu_l2_pt_el, Up_phi*_llnunu_l2_phi_el);
        mt_col[4] = MTCalcEl(Dn_pt*_llnunu_l2_pt_el, Dn_phi*_llnunu_l2_phi_el);
              
        _llnunu_mt_el_JetEnUp = TMath::MaxElement(5, mt_col);
        _llnunu_mt_el_JetEnDn = TMath::MinElement(5, mt_col);

        Up_pt = _gjet_l2_t1Pt_JetResUp/_gjet_l2_pt;
        Up_phi = _gjet_l2_t1Phi_JetResUp/_gjet_l2_phi;    
        Dn_pt = _gjet_l2_t1Pt_JetResDn/_gjet_l2_pt;
        Dn_phi = _gjet_l2_t1Phi_JetResDn/_gjet_l2_phi;

        mt_col[0] = _llnunu_mt_el;
        mt_col[1] = MTCalcEl(Up_pt*_llnunu_l2_pt_el, Up_phi*_llnunu_l2_phi_el);
        mt_col[2] = MTCalcEl(Up_pt*_llnunu_l2_pt_el, Dn_phi*_llnunu_l2_phi_el);
        mt_col[3] = MTCalcEl(Dn_pt*_llnunu_l2_pt_el, Up_phi*_llnunu_l2_phi_el);
        mt_col[4] = MTCalcEl(Dn_pt*_llnunu_l2_pt_el, Dn_phi*_llnunu_l2_phi_el);

        _llnunu_mt_el_JetResUp = TMath::MaxElement(5, mt_col);
        _llnunu_mt_el_JetResDn = TMath::MinElement(5, mt_col);

        Up_pt = _gjet_l2_t1Pt_MuonEnUp/_gjet_l2_pt;
        Up_phi = _gjet_l2_t1Phi_MuonEnUp/_gjet_l2_phi;
        Dn_pt = _gjet_l2_t1Pt_MuonEnDn/_gjet_l2_pt;
        Dn_phi = _gjet_l2_t1Phi_MuonEnDn/_gjet_l2_phi;

        mt_col[0] = _llnunu_mt_el;
        mt_col[1] = MTCalcEl(Up_pt*_llnunu_l2_pt_el, Up_phi*_llnunu_l2_phi_el);
        mt_col[2] = MTCalcEl(Up_pt*_llnunu_l2_pt_el, Dn_phi*_llnunu_l2_phi_el);
        mt_col[3] = MTCalcEl(Dn_pt*_llnunu_l2_pt_el, Up_phi*_llnunu_l2_phi_el);
        mt_col[4] = MTCalcEl(Dn_pt*_llnunu_l2_pt_el, Dn_phi*_llnunu_l2_phi_el);

        _llnunu_mt_el_MuonEnUp = TMath::MaxElement(5, mt_col);
        _llnunu_mt_el_MuonEnDn = TMath::MinElement(5, mt_col);

        Up_pt = _gjet_l2_t1Pt_ElectronEnUp/_gjet_l2_pt;
        Up_phi = _gjet_l2_t1Phi_ElectronEnUp/_gjet_l2_phi;
        Dn_pt = _gjet_l2_t1Pt_ElectronEnDn/_gjet_l2_pt;
        Dn_phi = _gjet_l2_t1Phi_ElectronEnDn/_gjet_l2_phi;

        mt_col[0] = _llnunu_mt_el;
        mt_col[1] = MTCalcEl(Up_pt*_llnunu_l2_pt_el, Up_phi*_llnunu_l2_phi_el);
        mt_col[2] = MTCalcEl(Up_pt*_llnunu_l2_pt_el, Dn_phi*_llnunu_l2_phi_el);
        mt_col[3] = MTCalcEl(Dn_pt*_llnunu_l2_pt_el, Up_phi*_llnunu_l2_phi_el);
        mt_col[4] = MTCalcEl(Dn_pt*_llnunu_l2_pt_el, Dn_phi*_llnunu_l2_phi_el);

        _llnunu_mt_el_ElectronEnUp = TMath::MaxElement(5, mt_col);
        _llnunu_mt_el_ElectronEnDn = TMath::MinElement(5, mt_col);

        Up_pt = _gjet_l2_t1Pt_PhotonEnUp/_gjet_l2_pt;
        Up_phi = _gjet_l2_t1Phi_PhotonEnUp/_gjet_l2_phi;
        Dn_pt = _gjet_l2_t1Pt_PhotonEnDn/_gjet_l2_pt;
        Dn_phi = _gjet_l2_t1Phi_PhotonEnDn/_gjet_l2_phi;

        mt_col[0] = _llnunu_mt_el;
        mt_col[1] = MTCalcEl(Up_pt*_llnunu_l2_pt_el, Up_phi*_llnunu_l2_phi_el);
        mt_col[2] = MTCalcEl(Up_pt*_llnunu_l2_pt_el, Dn_phi*_llnunu_l2_phi_el);
        mt_col[3] = MTCalcEl(Dn_pt*_llnunu_l2_pt_el, Up_phi*_llnunu_l2_phi_el);
        mt_col[4] = MTCalcEl(Dn_pt*_llnunu_l2_pt_el, Dn_phi*_llnunu_l2_phi_el);

        _llnunu_mt_el_PhotonEnUp = TMath::MaxElement(5, mt_col);
        _llnunu_mt_el_PhotonEnDn = TMath::MinElement(5, mt_col);

        Up_pt = _gjet_l2_t1Pt_TauEnUp/_gjet_l2_pt;
        Up_phi = _gjet_l2_t1Phi_TauEnUp/_gjet_l2_phi;
        Dn_pt = _gjet_l2_t1Pt_TauEnDn/_gjet_l2_pt;
        Dn_phi = _gjet_l2_t1Phi_TauEnDn/_gjet_l2_phi;

        mt_col[0] = _llnunu_mt_el;
        mt_col[1] = MTCalcEl(Up_pt*_llnunu_l2_pt_el, Up_phi*_llnunu_l2_phi_el);
        mt_col[2] = MTCalcEl(Up_pt*_llnunu_l2_pt_el, Dn_phi*_llnunu_l2_phi_el);
        mt_col[3] = MTCalcEl(Dn_pt*_llnunu_l2_pt_el, Up_phi*_llnunu_l2_phi_el);
        mt_col[4] = MTCalcEl(Dn_pt*_llnunu_l2_pt_el, Dn_phi*_llnunu_l2_phi_el);

        _llnunu_mt_el_TauEnUp = TMath::MaxElement(5, mt_col);
        _llnunu_mt_el_TauEnDn = TMath::MinElement(5, mt_col);

        Up_pt = _gjet_l2_t1Pt_UnclusterUp/_gjet_l2_pt;
        Up_phi = _gjet_l2_t1Phi_UnclusterUp/_gjet_l2_phi;
        Dn_pt = _gjet_l2_t1Pt_UnclusterDn/_gjet_l2_pt;
        Dn_phi = _gjet_l2_t1Phi_UnclusterDn/_gjet_l2_phi;

        mt_col[0] = _llnunu_mt_el;
        mt_col[1] = MTCalcEl(Up_pt*_llnunu_l2_pt_el, Up_phi*_llnunu_l2_phi_el);
        mt_col[2] = MTCalcEl(Up_pt*_llnunu_l2_pt_el, Dn_phi*_llnunu_l2_phi_el);
        mt_col[3] = MTCalcEl(Dn_pt*_llnunu_l2_pt_el, Up_phi*_llnunu_l2_phi_el);
        mt_col[4] = MTCalcEl(Dn_pt*_llnunu_l2_pt_el, Dn_phi*_llnunu_l2_phi_el);

        _llnunu_mt_el_UnclusterUp = TMath::MaxElement(5, mt_col);
        _llnunu_mt_el_UnclusterDn = TMath::MinElement(5, mt_col);


    }
    else{

        _llnunu_mt_el_JetEnUp = _llnunu_mt_el;
        _llnunu_mt_el_JetEnDn = _llnunu_mt_el;
        _llnunu_mt_el_JetResUp = _llnunu_mt_el;
        _llnunu_mt_el_JetResDn = _llnunu_mt_el;
        _llnunu_mt_el_MuonEnUp = _llnunu_mt_el;
        _llnunu_mt_el_MuonEnDn = _llnunu_mt_el;
        _llnunu_mt_el_ElectronEnUp = _llnunu_mt_el;
        _llnunu_mt_el_ElectronEnDn = _llnunu_mt_el;
        _llnunu_mt_el_PhotonEnUp = _llnunu_mt_el;
        _llnunu_mt_el_PhotonEnDn = _llnunu_mt_el;
        _llnunu_mt_el_TauEnUp = _llnunu_mt_el;
        _llnunu_mt_el_TauEnDn = _llnunu_mt_el;
        _llnunu_mt_el_UnclusterUp = _llnunu_mt_el;
        _llnunu_mt_el_UnclusterDn = _llnunu_mt_el;

    }

}

void doMTUncMu(){

     if(!_doMTUncDummy){

        _llnunu_mt_mu = MTCalcMu(_llnunu_l2_pt_mu,_llnunu_l2_phi_mu);

        float Up_pt = _gjet_l2_t1Pt_JetEnUp/_gjet_l2_pt;
        float Up_phi = _gjet_l2_t1Phi_JetEnUp/_gjet_l2_phi;
        float Dn_pt = _gjet_l2_t1Pt_JetEnDn/_gjet_l2_pt;
        float Dn_phi = _gjet_l2_t1Phi_JetEnDn/_gjet_l2_phi;

        float mt_col[5];

        mt_col[0] = _llnunu_mt_mu;
        mt_col[1] = MTCalcMu(Up_pt*_llnunu_l2_pt_mu, Up_phi*_llnunu_l2_phi_mu);
        mt_col[2] = MTCalcMu(Up_pt*_llnunu_l2_pt_mu, Dn_phi*_llnunu_l2_phi_mu);
        mt_col[3] = MTCalcMu(Dn_pt*_llnunu_l2_pt_mu, Up_phi*_llnunu_l2_phi_mu);
        mt_col[4] = MTCalcMu(Dn_pt*_llnunu_l2_pt_mu, Dn_phi*_llnunu_l2_phi_mu);

        _llnunu_mt_mu_JetEnUp = TMath::MaxElement(5, mt_col);
        _llnunu_mt_mu_JetEnDn = TMath::MinElement(5, mt_col);

        Up_pt = _gjet_l2_t1Pt_JetResUp/_gjet_l2_pt;
        Up_phi = _gjet_l2_t1Phi_JetResUp/_gjet_l2_phi;
        Dn_pt = _gjet_l2_t1Pt_JetResDn/_gjet_l2_pt;
        Dn_phi = _gjet_l2_t1Phi_JetResDn/_gjet_l2_phi;

        mt_col[0] = _llnunu_mt_mu;
        mt_col[1] = MTCalcMu(Up_pt*_llnunu_l2_pt_mu, Up_phi*_llnunu_l2_phi_mu);
        mt_col[2] = MTCalcMu(Up_pt*_llnunu_l2_pt_mu, Dn_phi*_llnunu_l2_phi_mu);
        mt_col[3] = MTCalcMu(Dn_pt*_llnunu_l2_pt_mu, Up_phi*_llnunu_l2_phi_mu);
        mt_col[4] = MTCalcMu(Dn_pt*_llnunu_l2_pt_mu, Dn_phi*_llnunu_l2_phi_mu);

        _llnunu_mt_mu_JetResUp = TMath::MaxElement(5, mt_col);
        _llnunu_mt_mu_JetResDn = TMath::MinElement(5, mt_col);

        Up_pt = _gjet_l2_t1Pt_MuonEnUp/_gjet_l2_pt;
        Up_phi = _gjet_l2_t1Phi_MuonEnUp/_gjet_l2_phi;
        Dn_pt = _gjet_l2_t1Pt_MuonEnDn/_gjet_l2_pt;
        Dn_phi = _gjet_l2_t1Phi_MuonEnDn/_gjet_l2_phi;

        mt_col[0] = _llnunu_mt_mu;
        mt_col[1] = MTCalcMu(Up_pt*_llnunu_l2_pt_mu, Up_phi*_llnunu_l2_phi_mu);
        mt_col[2] = MTCalcMu(Up_pt*_llnunu_l2_pt_mu, Dn_phi*_llnunu_l2_phi_mu);
        mt_col[3] = MTCalcMu(Dn_pt*_llnunu_l2_pt_mu, Up_phi*_llnunu_l2_phi_mu);
        mt_col[4] = MTCalcMu(Dn_pt*_llnunu_l2_pt_mu, Dn_phi*_llnunu_l2_phi_mu);

        _llnunu_mt_mu_MuonEnUp = TMath::MaxElement(5, mt_col);
        _llnunu_mt_mu_MuonEnDn = TMath::MinElement(5, mt_col);

        Up_pt = _gjet_l2_t1Pt_ElectronEnUp/_gjet_l2_pt;
        Up_phi = _gjet_l2_t1Phi_ElectronEnUp/_gjet_l2_phi;
        Dn_pt = _gjet_l2_t1Pt_ElectronEnDn/_gjet_l2_pt;
        Dn_phi = _gjet_l2_t1Phi_ElectronEnDn/_gjet_l2_phi;

        mt_col[0] = _llnunu_mt_mu;
        mt_col[1] = MTCalcMu(Up_pt*_llnunu_l2_pt_mu, Up_phi*_llnunu_l2_phi_mu);
        mt_col[2] = MTCalcMu(Up_pt*_llnunu_l2_pt_mu, Dn_phi*_llnunu_l2_phi_mu);
        mt_col[3] = MTCalcMu(Dn_pt*_llnunu_l2_pt_mu, Up_phi*_llnunu_l2_phi_mu);
        mt_col[4] = MTCalcMu(Dn_pt*_llnunu_l2_pt_mu, Dn_phi*_llnunu_l2_phi_mu);

        _llnunu_mt_mu_ElectronEnUp = TMath::MaxElement(5, mt_col);
        _llnunu_mt_mu_ElectronEnDn = TMath::MinElement(5, mt_col);

        Up_pt = _gjet_l2_t1Pt_PhotonEnUp/_gjet_l2_pt;
        Up_phi = _gjet_l2_t1Phi_PhotonEnUp/_gjet_l2_phi;
        Dn_pt = _gjet_l2_t1Pt_PhotonEnDn/_gjet_l2_pt;
        Dn_phi = _gjet_l2_t1Phi_PhotonEnDn/_gjet_l2_phi;

        mt_col[0] = _llnunu_mt_mu;
        mt_col[1] = MTCalcMu(Up_pt*_llnunu_l2_pt_mu, Up_phi*_llnunu_l2_phi_mu);
        mt_col[2] = MTCalcMu(Up_pt*_llnunu_l2_pt_mu, Dn_phi*_llnunu_l2_phi_mu);
        mt_col[3] = MTCalcMu(Dn_pt*_llnunu_l2_pt_mu, Up_phi*_llnunu_l2_phi_mu);
        mt_col[4] = MTCalcMu(Dn_pt*_llnunu_l2_pt_mu, Dn_phi*_llnunu_l2_phi_mu);

        _llnunu_mt_mu_PhotonEnUp = TMath::MaxElement(5, mt_col);
        _llnunu_mt_mu_PhotonEnDn = TMath::MinElement(5, mt_col);

        Up_pt = _gjet_l2_t1Pt_TauEnUp/_gjet_l2_pt;
        Up_phi = _gjet_l2_t1Phi_TauEnUp/_gjet_l2_phi;
        Dn_pt = _gjet_l2_t1Pt_TauEnDn/_gjet_l2_pt;
        Dn_phi = _gjet_l2_t1Phi_TauEnDn/_gjet_l2_phi;

        mt_col[0] = _llnunu_mt_mu;
        mt_col[1] = MTCalcMu(Up_pt*_llnunu_l2_pt_mu, Up_phi*_llnunu_l2_phi_mu);
        mt_col[2] = MTCalcMu(Up_pt*_llnunu_l2_pt_mu, Dn_phi*_llnunu_l2_phi_mu);
        mt_col[3] = MTCalcMu(Dn_pt*_llnunu_l2_pt_mu, Up_phi*_llnunu_l2_phi_mu);
        mt_col[4] = MTCalcMu(Dn_pt*_llnunu_l2_pt_mu, Dn_phi*_llnunu_l2_phi_mu);

        _llnunu_mt_mu_TauEnUp = TMath::MaxElement(5, mt_col);
        _llnunu_mt_mu_TauEnDn = TMath::MinElement(5, mt_col);

        Up_pt = _gjet_l2_t1Pt_UnclusterUp/_gjet_l2_pt;
        Up_phi = _gjet_l2_t1Phi_UnclusterUp/_gjet_l2_phi;
        Dn_pt = _gjet_l2_t1Pt_UnclusterDn/_gjet_l2_pt;
        Dn_phi = _gjet_l2_t1Phi_UnclusterDn/_gjet_l2_phi;

        mt_col[0] = _llnunu_mt_mu;
        mt_col[1] = MTCalcMu(Up_pt*_llnunu_l2_pt_mu, Up_phi*_llnunu_l2_phi_mu);
        mt_col[2] = MTCalcMu(Up_pt*_llnunu_l2_pt_mu, Dn_phi*_llnunu_l2_phi_mu);
        mt_col[3] = MTCalcMu(Dn_pt*_llnunu_l2_pt_mu, Up_phi*_llnunu_l2_phi_mu);
        mt_col[4] = MTCalcMu(Dn_pt*_llnunu_l2_pt_mu, Dn_phi*_llnunu_l2_phi_mu);

        _llnunu_mt_mu_UnclusterUp = TMath::MaxElement(5, mt_col);
        _llnunu_mt_mu_UnclusterDn = TMath::MinElement(5, mt_col);

    }
    else{

        _llnunu_mt_mu_JetEnUp = _llnunu_mt_mu;
        _llnunu_mt_mu_JetEnDn = _llnunu_mt_mu;
        _llnunu_mt_mu_JetResUp = _llnunu_mt_mu;
        _llnunu_mt_mu_JetResDn = _llnunu_mt_mu;
        _llnunu_mt_mu_MuonEnUp = _llnunu_mt_mu;
        _llnunu_mt_mu_MuonEnDn = _llnunu_mt_mu;
        _llnunu_mt_mu_ElectronEnUp = _llnunu_mt_mu;
        _llnunu_mt_mu_ElectronEnDn = _llnunu_mt_mu;
        _llnunu_mt_mu_PhotonEnUp = _llnunu_mt_mu;
        _llnunu_mt_mu_PhotonEnDn = _llnunu_mt_mu;
        _llnunu_mt_mu_TauEnUp = _llnunu_mt_mu;
        _llnunu_mt_mu_TauEnDn = _llnunu_mt_mu;
        _llnunu_mt_mu_UnclusterUp = _llnunu_mt_mu;
        _llnunu_mt_mu_UnclusterDn = _llnunu_mt_mu;

    }

}


// do elec pt recalib simple
void doElecPtRecalibSimpleData()
{
  if ((abs(_llnunu_l1_l1_pdgId)==11||abs(_llnunu_l1_l2_pdgId)==11) && _isData ) {
   
    
    if (_doElecPtRecalibSimpleDataPogRecipe) { 
      // pog recipe will correct energy according to seeding xtal energies.
      if (abs(_llnunu_l1_l1_pdgId)==11) {
        _ElecPtRecalibSimpleDataScale = 1.0; // reset to be 1.0
        if (_llnunu_l1_l1_eSeedXtal>200 && _llnunu_l1_l1_eSeedXtal<=300) _ElecPtRecalibSimpleDataScale = 1.0199;
        else if (_llnunu_l1_l1_eSeedXtal>300 && _llnunu_l1_l1_eSeedXtal<=400) _ElecPtRecalibSimpleDataScale = 1.052;
        else if (_llnunu_l1_l1_eSeedXtal>400 && _llnunu_l1_l1_eSeedXtal<=500) _ElecPtRecalibSimpleDataScale = 1.015;
        _llnunu_l1_l1_pt = Float_t(_llnunu_l1_l1_pt*_ElecPtRecalibSimpleDataScale);
      }
      if (abs(_llnunu_l1_l2_pdgId)==11) {
        _ElecPtRecalibSimpleDataScale = 1.0; // reset to be 1.0
        if (_llnunu_l1_l2_eSeedXtal>200 && _llnunu_l1_l2_eSeedXtal<=300) _ElecPtRecalibSimpleDataScale = 1.0199;
        else if (_llnunu_l1_l2_eSeedXtal>300 && _llnunu_l1_l2_eSeedXtal<=400) _ElecPtRecalibSimpleDataScale = 1.052;
        else if (_llnunu_l1_l2_eSeedXtal>400 && _llnunu_l1_l2_eSeedXtal<=500) _ElecPtRecalibSimpleDataScale = 1.015;
        _llnunu_l1_l2_pt = Float_t(_llnunu_l1_l2_pt*_ElecPtRecalibSimpleDataScale);
      }
    }
    else {
    }
    TLorentzVector l1v, l2v;
    l1v.SetPtEtaPhiM(_llnunu_l1_l1_pt, _llnunu_l1_l1_eta, _llnunu_l1_l1_phi, _llnunu_l1_l1_mass);
    l2v.SetPtEtaPhiM(_llnunu_l1_l2_pt, _llnunu_l1_l2_eta, _llnunu_l1_l2_phi, _llnunu_l1_l2_mass);
    TLorentzVector zv = l1v+l2v;
    _llnunu_l1_l1_rapidity = (Float_t)l1v.Rapidity();
    _llnunu_l1_l2_rapidity = (Float_t)l2v.Rapidity();
    _llnunu_l1_pt = (Float_t)zv.Pt();
    _llnunu_l1_eta = (Float_t)zv.Eta();
    _llnunu_l1_phi = (Float_t)zv.Phi();
    _llnunu_l1_rapidity = (Float_t)zv.Rapidity();
    _llnunu_l1_deltaPhi = (Float_t)l1v.DeltaPhi(l2v);
    _llnunu_l1_deltaR = (Float_t)l1v.DeltaR(l2v);
    _llnunu_l1_mt = (Float_t)zv.Mt();
    _llnunu_l1_mass = (Float_t)zv.M();

    TVector2 vec_met(_llnunu_l2_pt*cos(_llnunu_l2_phi), _llnunu_l2_pt*sin(_llnunu_l2_phi));

    Float_t et1 = TMath::Sqrt(_llnunu_l1_mass*_llnunu_l1_mass + _llnunu_l1_pt*_llnunu_l1_pt);
    Float_t et2 = TMath::Sqrt(_llnunu_l1_mass*_llnunu_l1_mass + _llnunu_l2_pt*_llnunu_l2_pt);
    _llnunu_mt = TMath::Sqrt(2.0*_llnunu_l1_mass*_llnunu_l1_mass+2.0*(et1*et2
               -_llnunu_l1_pt*cos(_llnunu_l1_phi)*_llnunu_l2_pt*cos(_llnunu_l2_phi)
               -_llnunu_l1_pt*sin(_llnunu_l1_phi)*_llnunu_l2_pt*sin(_llnunu_l2_phi)));
  } 
  

}

// do muon pt recalib simple
void doMuonPtRecalibSimpleData()
{
  if ((abs(_llnunu_l1_l1_pdgId)==13||abs(_llnunu_l1_l2_pdgId)==13) && _isData ) {
    if (abs(_llnunu_l1_l1_pdgId)==13) _llnunu_l1_l1_pt = Float_t(_llnunu_l1_l1_pt*_MuonPtRecalibSimpleDataScale);
    if (abs(_llnunu_l1_l2_pdgId)==13) _llnunu_l1_l2_pt = Float_t(_llnunu_l1_l2_pt*_MuonPtRecalibSimpleDataScale);

    TLorentzVector l1v, l2v;
    l1v.SetPtEtaPhiM(_llnunu_l1_l1_pt, _llnunu_l1_l1_eta, _llnunu_l1_l1_phi, _llnunu_l1_l1_mass);
    l2v.SetPtEtaPhiM(_llnunu_l1_l2_pt, _llnunu_l1_l2_eta, _llnunu_l1_l2_phi, _llnunu_l1_l2_mass);
    TLorentzVector zv = l1v+l2v;
    _llnunu_l1_l1_rapidity = (Float_t)l1v.Rapidity();
    _llnunu_l1_l2_rapidity = (Float_t)l2v.Rapidity();
    _llnunu_l1_pt = (Float_t)zv.Pt();
    _llnunu_l1_eta = (Float_t)zv.Eta();
    _llnunu_l1_phi = (Float_t)zv.Phi();
    _llnunu_l1_rapidity = (Float_t)zv.Rapidity();
    _llnunu_l1_deltaPhi = (Float_t)l1v.DeltaPhi(l2v);
    _llnunu_l1_deltaR = (Float_t)l1v.DeltaR(l2v);
    _llnunu_l1_mt = (Float_t)zv.Mt();
    _llnunu_l1_mass = (Float_t)zv.M();

    TVector2 vec_met(_llnunu_l2_pt*cos(_llnunu_l2_phi), _llnunu_l2_pt*sin(_llnunu_l2_phi));

    Float_t et1 = TMath::Sqrt(_llnunu_l1_mass*_llnunu_l1_mass + _llnunu_l1_pt*_llnunu_l1_pt);
    Float_t et2 = TMath::Sqrt(_llnunu_l1_mass*_llnunu_l1_mass + _llnunu_l2_pt*_llnunu_l2_pt);
    _llnunu_mt = TMath::Sqrt(2.0*_llnunu_l1_mass*_llnunu_l1_mass+2.0*(et1*et2
               -_llnunu_l1_pt*cos(_llnunu_l1_phi)*_llnunu_l2_pt*cos(_llnunu_l2_phi)
               -_llnunu_l1_pt*sin(_llnunu_l1_phi)*_llnunu_l2_pt*sin(_llnunu_l2_phi)));
  }


}

float MTCalc(float l2_pt, float l2_phi){

    Float_t et1 = TMath::Sqrt(_llnunu_l1_mass*_llnunu_l1_mass + _llnunu_l1_pt*_llnunu_l1_pt);
    Float_t et2 = TMath::Sqrt(_llnunu_l1_mass*_llnunu_l1_mass + l2_pt*l2_pt);
    Float_t mt = TMath::Sqrt(2.0*_llnunu_l1_mass*_llnunu_l1_mass+2.0*(et1*et2
               -_llnunu_l1_pt*cos(_llnunu_l1_phi)*l2_pt*cos(l2_phi)
               -_llnunu_l1_pt*sin(_llnunu_l1_phi)*l2_pt*sin(l2_phi)));

    return mt; 

}

float MTCalcEl(float l2_pt, float l2_phi){

    Float_t et1 = TMath::Sqrt(_llnunu_l1_mass_el*_llnunu_l1_mass_el + _llnunu_l1_pt*_llnunu_l1_pt);
    Float_t et2 = TMath::Sqrt(_llnunu_l1_mass_el*_llnunu_l1_mass_el + l2_pt*l2_pt);
    Float_t mt = TMath::Sqrt(2.0*_llnunu_l1_mass_el*_llnunu_l1_mass_el+2.0*(et1*et2
               -_llnunu_l1_pt*cos(_llnunu_l1_phi)*l2_pt*cos(l2_phi)
               -_llnunu_l1_pt*sin(_llnunu_l1_phi)*l2_pt*sin(l2_phi)));

    return mt;

}

float MTCalcMu(float l2_pt, float l2_phi){

    Float_t et1 = TMath::Sqrt(_llnunu_l1_mass_mu*_llnunu_l1_mass_mu + _llnunu_l1_pt*_llnunu_l1_pt); 
   Float_t et2 = TMath::Sqrt(_llnunu_l1_mass_mu*_llnunu_l1_mass_mu + l2_pt*l2_pt);
    Float_t mt = TMath::Sqrt(2.0*_llnunu_l1_mass_mu*_llnunu_l1_mass_mu+2.0*(et1*et2
               -_llnunu_l1_pt*cos(_llnunu_l1_phi)*l2_pt*cos(l2_phi)
               -_llnunu_l1_pt*sin(_llnunu_l1_phi)*l2_pt*sin(l2_phi)));

    return mt;

}

// prepare inputs addZZCorrections
void prepareZZCorrections()
{
  _zzCorr = new ZZCorrections();
  _zzCorr->loadEwkTable(_ZZCorrectionEwkInputFileName);
  _zzCorr->loadQcdFile(_ZZCorrectionQcdInputFileName);

}

// addZZCorrections
void addZZCorrections()
{
  // Ewk corr and error
  TLorentzVector V1, V2;
  V1.SetPtEtaPhiM(_genZ_pt[0],_genZ_eta[0],_genZ_phi[0],_genZ_mass[0]);
  V2.SetPtEtaPhiM(_genZ_pt[1],_genZ_eta[1],_genZ_phi[1],_genZ_mass[1]);
  float sum4Pt = _genLep_pt[0]+_genLep_pt[1]+_genNeu_pt[0]+_genNeu_pt[1];
  float ewkCorrections_error;
  _ZZEwkCorrWeight = _zzCorr->getEwkCorrections(ewkCorrections_error, _genQ_pdgId[0], _pdf_x1, _pdf_x2, V1, V2, sum4Pt );
  _ZZEwkCorrWeight_up = _ZZEwkCorrWeight+ewkCorrections_error;
  _ZZEwkCorrWeight_dn = _ZZEwkCorrWeight-ewkCorrections_error;

  // Qcd corr and error
  float mZZ = (V1+V2).M();
  _ZZQcdCorrWeight = _zzCorr->getKfactor_qqZZ_qcd_mZZ(mZZ);
  _ZZQcdCorrWeight_up = _zzCorr->getKfactor_qqZZ_qcd_mZZ_up(mZZ);
  _ZZQcdCorrWeight_dn = _zzCorr->getKfactor_qqZZ_qcd_mZZ_dn(mZZ);

  if (_debug) {
    std::cout << " qqZZEwkCorr,up,dn = " << _ZZEwkCorrWeight << "," << _ZZEwkCorrWeight_up << "," << _ZZEwkCorrWeight_dn << std::endl;
    std::cout << " qqZZQcdCorr,up,dn = " << _ZZQcdCorrWeight << "," << _ZZQcdCorrWeight_up << "," << _ZZQcdCorrWeight_dn << std::endl;
  }
}

// prepare inputs for addDyZPtWeight
void prepareDyZPtWeight()
{

  // needed output branches
  _tree_out->Branch("ZPtWeight", &_ZPtWeight, "ZPtWeight/F");
  _tree_out->Branch("ZPtWeight_up", &_ZPtWeight_up, "ZPtWeight_up/F");
  _tree_out->Branch("ZPtWeight_dn", &_ZPtWeight_dn, "ZPtWeight_dn/F");
  if (_addDyNewGenWeight) {
    _tree_out->Branch("ZJetsGenWeight", &_ZJetsGenWeight, "ZJetsGenWeight/F");
  }

  // get input file and building materials
  _fdyzpt = new TFile(_DyZPtWeightInputFileName.c_str());
  _hdyzpt_dtmc_ratio = (TH1D*)_fdyzpt->Get("hdyzpt_dtmc_ratio");
  _fcdyzpt_dtmc_ratio = (TF1*)_fdyzpt->Get("fcdyzpt_dtmc_ratio");
  _fcdyzpt_dtmc_ratio_resbos = (TF1*)_fdyzpt->Get("fcdyzpt_dtmc_ratio_resbos");
  _fcdyzpt_dtmc_ratio_resbos_refit = (TF1*)_fdyzpt->Get("fcdyzpt_dtmc_ratio_resbos_refit");
  _hdyzpt_mc_nlo_lo_ratio = (TH1D*)_fdyzpt->Get("hdyzpt_mc_nlo_lo_ratio");
  _fcdyzpt_mc_nlo_lo_ratio = (TF1*)_fdyzpt->Get("fcdyzpt_mc_nlo_lo_ratio");
  
}

// addDyZPtWeight
void addDyZPtWeight()
{
  if (_addDyZPtWeight && !_isData && _isDyJets) {
    Int_t zptBin=0;
    if (_ngenZ>0) {
      zptBin = _hdyzpt_dtmc_ratio->FindBin(_genZ_pt[0]);
      if (_genZ_pt[0]>1000) zptBin = _hdyzpt_dtmc_ratio->FindBin(999);
    }
    else {
      zptBin = _hdyzpt_dtmc_ratio->FindBin(_llnunu_l1_pt);
      if (_llnunu_l1_pt>1000) zptBin = _hdyzpt_dtmc_ratio->FindBin(999);
    }
    if (_addDyZPtWeightUseFunction && !_addDyZPtWeightUseResummationFunction ) {
      if (_ngenZ>0) _ZPtWeight = _fcdyzpt_dtmc_ratio->Eval(_genZ_pt[0]);
      else _ZPtWeight = _fcdyzpt_dtmc_ratio->Eval(_llnunu_l1_pt);
    }
    else if (_addDyZPtWeightUseFunction && _addDyZPtWeightUseResummationFunction && !_addDyZPtWeightUseResummationRefitFunction) {
      if (_ngenZ>0) _ZPtWeight = _fcdyzpt_dtmc_ratio_resbos->Eval(_genZ_pt[0]);
      else _ZPtWeight = _fcdyzpt_dtmc_ratio_resbos->Eval(_llnunu_l1_pt);
    }
    else if (_addDyZPtWeightUseFunction && _addDyZPtWeightUseResummationFunction && _addDyZPtWeightUseResummationRefitFunction) {
      if (_ngenZ>0) _ZPtWeight = _fcdyzpt_dtmc_ratio_resbos_refit->Eval(_genZ_pt[0]);
      else _ZPtWeight = _fcdyzpt_dtmc_ratio_resbos_refit->Eval(_llnunu_l1_pt);
    }
    else {
      _ZPtWeight = _hdyzpt_dtmc_ratio->GetBinContent(zptBin);
    }
    _ZPtWeight_up = _ZPtWeight+0.5*_hdyzpt_dtmc_ratio->GetBinError(zptBin);
    _ZPtWeight_dn = _ZPtWeight-0.5*_hdyzpt_dtmc_ratio->GetBinError(zptBin);

    if (_isDyJetsLO) {
      // for LO ZJets samples
      zptBin=0;
      if (_ngenZ>0) {
        zptBin = _hdyzpt_mc_nlo_lo_ratio->FindBin(_genZ_pt[0]);
        if (_genZ_pt[0]>1000) zptBin = _hdyzpt_mc_nlo_lo_ratio->FindBin(999);
      }
      else {
        zptBin = _hdyzpt_mc_nlo_lo_ratio->FindBin(_llnunu_l1_pt);
        if (_llnunu_l1_pt>1000) zptBin = _hdyzpt_mc_nlo_lo_ratio->FindBin(999);
      }
      if (_addDyZPtWeightLOUseFunction) {
        if (_ngenZ>0) _ZPtWeight *= _fcdyzpt_mc_nlo_lo_ratio->Eval(_genZ_pt[0]);
        else _ZPtWeight *= _fcdyzpt_mc_nlo_lo_ratio->Eval(_llnunu_l1_pt);
      }
      else {
        _ZPtWeight *= _hdyzpt_mc_nlo_lo_ratio->GetBinContent(zptBin);
      }

      _ZPtWeight_up = _ZPtWeight+0.5*_hdyzpt_dtmc_ratio->GetBinError(zptBin);
      _ZPtWeight_dn = _ZPtWeight-0.5*_hdyzpt_dtmc_ratio->GetBinError(zptBin);

      // fix njets binned lo sample xsec, modify to nlo version
      if (_isDyJetsLOnjets>=1) {
        _xsec = _DyJetsNLOxsec;
      }

    }
  }

  /*
  if (_addDyNewGenWeight && !_isData && _isDyJets) {
    Float_t ZJetsLOSumWeights(49877138);
    Float_t ZJetsNLOSumWeights(450670522117);
    Float_t ZJetsLOSumEvents(49877138);
    Float_t ZJetsNLOSumEvents(28696958);
    if (!_isDyJetsLO) {
      _ZJetsGenWeight = _genWeight/ZJetsNLOSumWeights*ZJetsNLOSumEvents/(ZJetsNLOSumEvents+ZJetsLOSumEvents);
    }
    else {
      _ZJetsGenWeight = _genWeight/ZJetsLOSumWeights*ZJetsLOSumEvents/(ZJetsNLOSumEvents+ZJetsLOSumEvents);
    }
  }
  */

}



// prepare JEC/JER
void prepareJECJER()
{

  // JEC parameters
  // data
  _JEC_ResJetPar_DATA = new JetCorrectorParameters(_JECParTxt_DATA_L2L3Residual);
  _JEC_L3JetPar_DATA  = new JetCorrectorParameters(_JECParTxt_DATA_L3Absolute);
  _JEC_L2JetPar_DATA  = new JetCorrectorParameters(_JECParTxt_DATA_L2Relative);
  _JEC_L1JetPar_DATA  = new JetCorrectorParameters(_JECParTxt_DATA_L1FastJet);
  // mc
  _JEC_ResJetPar_MC = new JetCorrectorParameters(_JECParTxt_MC_L2L3Residual);
  _JEC_L3JetPar_MC  = new JetCorrectorParameters(_JECParTxt_MC_L3Absolute);
  _JEC_L2JetPar_MC  = new JetCorrectorParameters(_JECParTxt_MC_L2Relative);
  _JEC_L1JetPar_MC  = new JetCorrectorParameters(_JECParTxt_MC_L1FastJet);

  // jec parametesrs vec
  _JEC_vPar_DATA.push_back(*_JEC_ResJetPar_DATA);
  _JEC_vPar_DATA.push_back(*_JEC_L3JetPar_DATA);
  _JEC_vPar_DATA.push_back(*_JEC_L2JetPar_DATA);
  _JEC_vPar_DATA.push_back(*_JEC_L1JetPar_DATA);
  _JEC_vPar_MC.push_back(*_JEC_ResJetPar_MC);
  _JEC_vPar_MC.push_back(*_JEC_L3JetPar_MC);
  _JEC_vPar_MC.push_back(*_JEC_L2JetPar_MC);
  _JEC_vPar_MC.push_back(*_JEC_L1JetPar_MC);    

  // jec corrector
  _JEC_JetCorrector_DATA = new FactorizedJetCorrector(_JEC_vPar_DATA);
  _JEC_JetCorrector_MC   = new FactorizedJetCorrector(_JEC_vPar_MC);

  // jec uncertaities
  _JEC_Uncertainty_DATA = new JetCorrectionUncertainty(_JECParTxt_DATA_Uncertainty);
  _JEC_Uncertainty_MC   = new JetCorrectionUncertainty(_JECParTxt_MC_Uncertainty);
    

  // JER parameters
  _JER_Reso_DATA = new JME::JetResolution(_JERParTxt_Reso_DATA);
  _JER_Reso_MC   = new JME::JetResolution(_JERParTxt_Reso_MC);
  _JER_SF_MC     = new JME::JetResolutionScaleFactor(_JERParTxt_SF_MC);

}


// do JEC/JER
void doJECJER() 
{



}

// prepare inputs for simple met recoil tune.
void prepareRecoil()
{
  if (_doRecoil && ((!_isData && _isDyJets && !_doGJetsSkim )||(_isData && _doGJetsSkim)) ) {
    // met shift  sigma
    _file_dt_sigma[0] = new TFile(_RecoilInputFileNameData_all.c_str());
    _file_dt_sigma[1] = new TFile(_RecoilInputFileNameData_mu.c_str());
    _file_dt_sigma[2] = new TFile(_RecoilInputFileNameData_el.c_str());
    if (_doGJetsSkim){
      _file_mc_sigma[0] = new TFile(_RecoilInputFileNameGJets_all.c_str());
      _file_mc_sigma[1] = new TFile(_RecoilInputFileNameGJets_mu.c_str());
      _file_mc_sigma[2] = new TFile(_RecoilInputFileNameGJets_el.c_str());
    }
    else if (!_isDyJetsLO) {
      _file_mc_sigma[0] = new TFile(_RecoilInputFileNameMC_all.c_str());
      _file_mc_sigma[1] = new TFile(_RecoilInputFileNameMC_mu.c_str());
      _file_mc_sigma[2] = new TFile(_RecoilInputFileNameMC_el.c_str());
    }
    else {
      _file_mc_sigma[0] = new TFile(_RecoilInputFileNameMCLO_all.c_str());
      _file_mc_sigma[1] = new TFile(_RecoilInputFileNameMCLO_mu.c_str());
      _file_mc_sigma[2] = new TFile(_RecoilInputFileNameMCLO_el.c_str());
    }

    // zpt profile, mean zpt in each zpt bin
    _p_dt_zpt[0] = (TProfile*)_file_dt_sigma[0]->Get("p_zpt");
    _p_dt_zpt[1] = (TProfile*)_file_dt_sigma[1]->Get("p_zpt");
    _p_dt_zpt[2] = (TProfile*)_file_dt_sigma[2]->Get("p_zpt");

    //
    _h_dt_met_para_shift[0] = (TH1D*)_file_dt_sigma[0]->Get("h_met_para_vs_zpt_mean");
    _h_mc_met_para_shift[0] = (TH1D*)_file_mc_sigma[0]->Get("h_met_para_vs_zpt_mean");
    _h_met_para_shift_dtmc[0] = (TH1D*)_h_dt_met_para_shift[0]->Clone("h_met_para_shift_dtmc_all");
    _h_met_para_shift_dtmc[0]->Add(_h_mc_met_para_shift[0], -1);

    _h_dt_met_para_sigma[0] = (TH1D*)_file_dt_sigma[0]->Get("h_met_para_vs_zpt_sigma");
    _h_dt_met_perp_sigma[0] = (TH1D*)_file_dt_sigma[0]->Get("h_met_perp_vs_zpt_sigma");
    _h_mc_met_para_sigma[0] = (TH1D*)_file_mc_sigma[0]->Get("h_met_para_vs_zpt_sigma");
    _h_mc_met_perp_sigma[0] = (TH1D*)_file_mc_sigma[0]->Get("h_met_perp_vs_zpt_sigma");

    _h_ratio_met_para_sigma_dtmc[0] = (TH1D*)_h_dt_met_para_sigma[0]->Clone("h_ratio_met_para_sigma_dtmc_all");
    _h_ratio_met_perp_sigma_dtmc[0] = (TH1D*)_h_dt_met_perp_sigma[0]->Clone("h_ratio_met_perp_sigma_dtmc_all");
    _h_ratio_met_para_sigma_dtmc[0]->Divide(_h_mc_met_para_sigma[0]);
    _h_ratio_met_perp_sigma_dtmc[0]->Divide(_h_mc_met_perp_sigma[0]);

    _h_dt_met_para_shift[1] = (TH1D*)_file_dt_sigma[1]->Get("h_met_para_vs_zpt_mean");
    _h_mc_met_para_shift[1] = (TH1D*)_file_mc_sigma[1]->Get("h_met_para_vs_zpt_mean");
    _h_met_para_shift_dtmc[1] = (TH1D*)_h_dt_met_para_shift[1]->Clone("h_met_para_shift_dtmc_mu");
    _h_met_para_shift_dtmc[1]->Add(_h_mc_met_para_shift[1], -1);

    _h_dt_met_para_sigma[1] = (TH1D*)_file_dt_sigma[1]->Get("h_met_para_vs_zpt_sigma");
    _h_dt_met_perp_sigma[1] = (TH1D*)_file_dt_sigma[1]->Get("h_met_perp_vs_zpt_sigma");
    _h_mc_met_para_sigma[1] = (TH1D*)_file_mc_sigma[1]->Get("h_met_para_vs_zpt_sigma");
    _h_mc_met_perp_sigma[1] = (TH1D*)_file_mc_sigma[1]->Get("h_met_perp_vs_zpt_sigma");

    _h_ratio_met_para_sigma_dtmc[1] = (TH1D*)_h_dt_met_para_sigma[1]->Clone("h_ratio_met_para_sigma_dtmc_mu");
    _h_ratio_met_perp_sigma_dtmc[1] = (TH1D*)_h_dt_met_perp_sigma[1]->Clone("h_ratio_met_perp_sigma_dtmc_mu");
    _h_ratio_met_para_sigma_dtmc[1]->Divide(_h_mc_met_para_sigma[1]);
    _h_ratio_met_perp_sigma_dtmc[1]->Divide(_h_mc_met_perp_sigma[1]);

    _h_dt_met_para_shift[2] = (TH1D*)_file_dt_sigma[2]->Get("h_met_para_vs_zpt_mean");
    _h_mc_met_para_shift[2] = (TH1D*)_file_mc_sigma[2]->Get("h_met_para_vs_zpt_mean");
    _h_met_para_shift_dtmc[2] = (TH1D*)_h_dt_met_para_shift[2]->Clone("h_met_para_shift_dtmc_el");
    _h_met_para_shift_dtmc[2]->Add(_h_mc_met_para_shift[2], -1);

    _h_dt_met_para_sigma[2] = (TH1D*)_file_dt_sigma[2]->Get("h_met_para_vs_zpt_sigma");
    _h_dt_met_perp_sigma[2] = (TH1D*)_file_dt_sigma[2]->Get("h_met_perp_vs_zpt_sigma");
    _h_mc_met_para_sigma[2] = (TH1D*)_file_mc_sigma[2]->Get("h_met_para_vs_zpt_sigma");
    _h_mc_met_perp_sigma[2] = (TH1D*)_file_mc_sigma[2]->Get("h_met_perp_vs_zpt_sigma");

    _h_ratio_met_para_sigma_dtmc[2] = (TH1D*)_h_dt_met_para_sigma[2]->Clone("h_ratio_met_para_sigma_dtmc_el");
    _h_ratio_met_perp_sigma_dtmc[2] = (TH1D*)_h_dt_met_perp_sigma[2]->Clone("h_ratio_met_perp_sigma_dtmc_el");
    _h_ratio_met_para_sigma_dtmc[2]->Divide(_h_mc_met_para_sigma[2]);
    _h_ratio_met_perp_sigma_dtmc[2]->Divide(_h_mc_met_perp_sigma[2]);

    // smooth functions
    _h_dt_met_para_shift[0]->SetName("h_dt_met_para_shift_all");
    _h_mc_met_para_shift[0]->SetName("h_mc_met_para_shift_all");
    _h_dt_met_para_shift[1]->SetName("h_dt_met_para_shift_mu");
    _h_mc_met_para_shift[1]->SetName("h_mc_met_para_shift_mu");
    _h_dt_met_para_shift[2]->SetName("h_dt_met_para_shift_el");
    _h_mc_met_para_shift[2]->SetName("h_mc_met_para_shift_el");
    _h_dt_met_para_shift[3] = (TH1D*)_h_dt_met_para_shift[0]->Clone("h_dt_met_para_shift_all_smooth");
    _h_mc_met_para_shift[3] = (TH1D*)_h_mc_met_para_shift[0]->Clone("h_mc_met_para_shift_all_smooth");
    _h_dt_met_para_shift[4] = (TH1D*)_h_dt_met_para_shift[1]->Clone("h_dt_met_para_shift_mu_smooth");
    _h_mc_met_para_shift[4] = (TH1D*)_h_mc_met_para_shift[1]->Clone("h_mc_met_para_shift_mu_smooth");
    _h_dt_met_para_shift[5] = (TH1D*)_h_dt_met_para_shift[2]->Clone("h_dt_met_para_shift_el_smooth");
    _h_mc_met_para_shift[5] = (TH1D*)_h_mc_met_para_shift[2]->Clone("h_mc_met_para_shift_el_smooth");
    _h_dt_met_para_shift[3]->Smooth();
    _h_mc_met_para_shift[3]->Smooth();
    _h_dt_met_para_shift[4]->Smooth();
    _h_mc_met_para_shift[4]->Smooth();
    _h_dt_met_para_shift[5]->Smooth();
    _h_mc_met_para_shift[5]->Smooth();

    _gr_dt_met_para_shift[3] = new TGraphErrors(_h_dt_met_para_shift[3]);
    _gr_mc_met_para_shift[3] = new TGraphErrors(_h_mc_met_para_shift[3]);
    _gr_dt_met_para_shift[4] = new TGraphErrors(_h_dt_met_para_shift[4]);
    _gr_mc_met_para_shift[4] = new TGraphErrors(_h_mc_met_para_shift[4]);
    _gr_dt_met_para_shift[5] = new TGraphErrors(_h_dt_met_para_shift[5]);
    _gr_mc_met_para_shift[5] = new TGraphErrors(_h_mc_met_para_shift[5]);

    _h_met_para_shift_dtmc[3] = (TH1D*)_h_met_para_shift_dtmc[0]->Clone("h_met_para_shift_dtmc_all_smooth");
    _h_met_para_shift_dtmc[4] = (TH1D*)_h_met_para_shift_dtmc[1]->Clone("h_met_para_shift_dtmc_mu_smooth");
    _h_met_para_shift_dtmc[5] = (TH1D*)_h_met_para_shift_dtmc[2]->Clone("h_met_para_shift_dtmc_el_smooth");
    _h_met_para_shift_dtmc[3]->Smooth();
    _h_met_para_shift_dtmc[4]->Smooth();
    _h_met_para_shift_dtmc[5]->Smooth();

    _gr_met_para_shift_dtmc[3] = new TGraphErrors(_h_met_para_shift_dtmc[3]);
    _gr_met_para_shift_dtmc[4] = new TGraphErrors(_h_met_para_shift_dtmc[4]);
    _gr_met_para_shift_dtmc[5] = new TGraphErrors(_h_met_para_shift_dtmc[5]);

    _h_ratio_met_para_sigma_dtmc[3] = (TH1D*)_h_ratio_met_para_sigma_dtmc[0]->Clone("h_ratio_met_para_sigma_dtmc_all_smooth");
    _h_ratio_met_para_sigma_dtmc[4] = (TH1D*)_h_ratio_met_para_sigma_dtmc[1]->Clone("h_ratio_met_para_sigma_dtmc_mu_smooth");
    _h_ratio_met_para_sigma_dtmc[5] = (TH1D*)_h_ratio_met_para_sigma_dtmc[2]->Clone("h_ratio_met_para_sigma_dtmc_el_smooth");
    _h_ratio_met_para_sigma_dtmc[3]->Smooth();
    _h_ratio_met_para_sigma_dtmc[4]->Smooth();
    _h_ratio_met_para_sigma_dtmc[5]->Smooth();

    _h_ratio_met_perp_sigma_dtmc[3] = (TH1D*)_h_ratio_met_perp_sigma_dtmc[0]->Clone("h_ratio_met_perp_sigma_dtmc_all_smooth");
    _h_ratio_met_perp_sigma_dtmc[4] = (TH1D*)_h_ratio_met_perp_sigma_dtmc[1]->Clone("h_ratio_met_perp_sigma_dtmc_mu_smooth");
    _h_ratio_met_perp_sigma_dtmc[5] = (TH1D*)_h_ratio_met_perp_sigma_dtmc[2]->Clone("h_ratio_met_perp_sigma_dtmc_el_smooth");
    _h_ratio_met_perp_sigma_dtmc[3]->Smooth();
    _h_ratio_met_perp_sigma_dtmc[4]->Smooth();
    _h_ratio_met_perp_sigma_dtmc[5]->Smooth();

    _gr_ratio_met_para_sigma_dtmc[3] = new TGraphErrors(_h_ratio_met_para_sigma_dtmc[3]);
    _gr_ratio_met_para_sigma_dtmc[4] = new TGraphErrors(_h_ratio_met_para_sigma_dtmc[4]);
    _gr_ratio_met_para_sigma_dtmc[5] = new TGraphErrors(_h_ratio_met_para_sigma_dtmc[5]);
    _gr_ratio_met_perp_sigma_dtmc[3] = new TGraphErrors(_h_ratio_met_perp_sigma_dtmc[3]);
    _gr_ratio_met_perp_sigma_dtmc[4] = new TGraphErrors(_h_ratio_met_perp_sigma_dtmc[4]);
    _gr_ratio_met_perp_sigma_dtmc[5] = new TGraphErrors(_h_ratio_met_perp_sigma_dtmc[5]);


    // reset the x-axis bin centers to the mean zpt in each zpt bin
    for (int ihist=0; ihist<3; ihist++){
      double xx,yy;
      for (int ibin=0; ibin<_p_dt_zpt[ihist]->GetNbinsX(); ibin++){

        // _gr_dt_met_para_shift
        _gr_dt_met_para_shift[3+ihist]->GetPoint(ibin, xx, yy);
        xx = _p_dt_zpt[ihist]->GetBinContent(ibin+1);
        _gr_dt_met_para_shift[3+ihist]->SetPoint(ibin, xx, yy);

        // _gr_mc_met_para_shift
        _gr_mc_met_para_shift[3+ihist]->GetPoint(ibin, xx, yy);
        xx = _p_dt_zpt[ihist]->GetBinContent(ibin+1);
        _gr_mc_met_para_shift[3+ihist]->SetPoint(ibin, xx, yy);

        // _gr_met_para_shift_dtmc
        _gr_met_para_shift_dtmc[3+ihist]->GetPoint(ibin, xx, yy);
        xx = _p_dt_zpt[ihist]->GetBinContent(ibin+1);
        _gr_met_para_shift_dtmc[3+ihist]->SetPoint(ibin, xx, yy);

        // _gr_ratio_met_para_sigma_dtmc
        _gr_ratio_met_para_sigma_dtmc[3+ihist]->GetPoint(ibin, xx, yy);
        xx = _p_dt_zpt[ihist]->GetBinContent(ibin+1);
        _gr_ratio_met_para_sigma_dtmc[3+ihist]->SetPoint(ibin, xx, yy);

        // _gr_ratio_met_perp_sigma_dtmc
        _gr_ratio_met_perp_sigma_dtmc[3+ihist]->GetPoint(ibin, xx, yy);
        xx = _p_dt_zpt[ihist]->GetBinContent(ibin+1);
        _gr_ratio_met_perp_sigma_dtmc[3+ihist]->SetPoint(ibin, xx, yy);
      }
    }

  }
  

}

// do simple met recoil fine tuning
void doRecoil()
{
  if ( _doRecoil && ((!_isData && _isDyJets && !_doGJetsSkim )||(_isData && _doGJetsSkim)) ) {
    
    // variable central values
    Float_t met_para = _llnunu_l2_pt*cos(_llnunu_l2_phi-_llnunu_l1_phi);
    Float_t met_perp = _llnunu_l2_pt*sin(_llnunu_l2_phi-_llnunu_l1_phi);
    Float_t met_para_mu = met_para;
    Float_t met_perp_mu = met_perp;
    Float_t met_para_el = met_para;
    Float_t met_perp_el = met_perp;

    // variable errors
    Float_t met_para_mean_ct_reso_up = met_para;
    Float_t met_para_mean_ct_reso_dn = met_para;
    Float_t met_para_mean_up_reso_up = met_para;
    Float_t met_para_mean_up_reso_ct = met_para;
    Float_t met_para_mean_up_reso_dn = met_para;
    Float_t met_para_mean_dn_reso_up = met_para;
    Float_t met_para_mean_dn_reso_ct = met_para;
    Float_t met_para_mean_dn_reso_dn = met_para;

    Float_t met_perp_mean_ct_reso_up = met_perp;     
    Float_t met_perp_mean_ct_reso_dn = met_perp;
     
    Float_t met_para_mu_mean_ct_reso_up = met_para;
    Float_t met_para_mu_mean_ct_reso_dn = met_para;
    Float_t met_para_mu_mean_up_reso_up = met_para;
    Float_t met_para_mu_mean_up_reso_ct = met_para;
    Float_t met_para_mu_mean_up_reso_dn = met_para;
    Float_t met_para_mu_mean_dn_reso_up = met_para;
    Float_t met_para_mu_mean_dn_reso_ct = met_para;
    Float_t met_para_mu_mean_dn_reso_dn = met_para;

    Float_t met_perp_mu_mean_ct_reso_up = met_perp;
    Float_t met_perp_mu_mean_ct_reso_dn = met_perp;

    Float_t met_para_el_mean_ct_reso_up = met_para;
    Float_t met_para_el_mean_ct_reso_dn = met_para;
    Float_t met_para_el_mean_up_reso_up = met_para;
    Float_t met_para_el_mean_up_reso_ct = met_para;
    Float_t met_para_el_mean_up_reso_dn = met_para;
    Float_t met_para_el_mean_dn_reso_up = met_para;
    Float_t met_para_el_mean_dn_reso_ct = met_para;
    Float_t met_para_el_mean_dn_reso_dn = met_para;

    Float_t met_perp_el_mean_ct_reso_up = met_perp;
    Float_t met_perp_el_mean_ct_reso_dn = met_perp;


    if (_doGJetsSkim) {//GJets
      Int_t idd0=0;
      if (_doRecoilUseSmooth) idd0=3;
      for (int idd=0; idd<3; idd++){
        _h_dt_met_para_shift[6+idd] = _h_dt_met_para_shift[idd+idd0];
        _h_mc_met_para_shift[6+idd] = _h_mc_met_para_shift[idd+idd0];
        _h_met_para_shift_dtmc[6+idd] = _h_met_para_shift_dtmc[idd+idd0];
        _h_ratio_met_para_sigma_dtmc[6+idd] = _h_ratio_met_para_sigma_dtmc[idd+idd0];
        _h_ratio_met_perp_sigma_dtmc[6+idd] = _h_ratio_met_perp_sigma_dtmc[idd+idd0];
        _gr_dt_met_para_shift[6+idd] = _gr_dt_met_para_shift[3+idd];
        _gr_mc_met_para_shift[6+idd] = _gr_mc_met_para_shift[3+idd];
        _gr_met_para_shift_dtmc[6+idd] = _gr_met_para_shift_dtmc[3+idd];
        _gr_ratio_met_para_sigma_dtmc[6+idd] = _gr_ratio_met_para_sigma_dtmc[3+idd];
        _gr_ratio_met_perp_sigma_dtmc[6+idd] = _gr_ratio_met_perp_sigma_dtmc[3+idd];
      }
    }
    else if (!_doGJetsSkim&&abs(_llnunu_l1_l1_pdgId)==13&&abs(_llnunu_l1_l2_pdgId)==13) {
      Int_t idd=1;
      if (_doRecoilUseSmooth) idd=4;
      _h_dt_met_para_shift[6] = _h_dt_met_para_shift[idd];
      _h_mc_met_para_shift[6] = _h_mc_met_para_shift[idd];
      _h_met_para_shift_dtmc[6] = _h_met_para_shift_dtmc[idd];
      _h_ratio_met_para_sigma_dtmc[6] = _h_ratio_met_para_sigma_dtmc[idd];
      _h_ratio_met_perp_sigma_dtmc[6] = _h_ratio_met_perp_sigma_dtmc[idd];
      _gr_dt_met_para_shift[6] = _gr_dt_met_para_shift[4];
      _gr_mc_met_para_shift[6] = _gr_mc_met_para_shift[4];
      _gr_met_para_shift_dtmc[6] = _gr_met_para_shift_dtmc[4];
      _gr_ratio_met_para_sigma_dtmc[6] = _gr_ratio_met_para_sigma_dtmc[4];
      _gr_ratio_met_perp_sigma_dtmc[6] = _gr_ratio_met_perp_sigma_dtmc[4];
    }
    else if (!_doGJetsSkim&&abs(_llnunu_l1_l1_pdgId)==11&&abs(_llnunu_l1_l2_pdgId)==11) {
      Int_t idd=2;
      if (_doRecoilUseSmooth) idd=5;
      _h_dt_met_para_shift[6] = _h_dt_met_para_shift[idd];
      _h_mc_met_para_shift[6] = _h_mc_met_para_shift[idd];
      _h_met_para_shift_dtmc[6] = _h_met_para_shift_dtmc[idd];
      _h_ratio_met_para_sigma_dtmc[6] = _h_ratio_met_para_sigma_dtmc[idd];
      _h_ratio_met_perp_sigma_dtmc[6] = _h_ratio_met_perp_sigma_dtmc[idd];
      _gr_dt_met_para_shift[6] = _gr_dt_met_para_shift[5];
      _gr_mc_met_para_shift[6] = _gr_mc_met_para_shift[5];
      _gr_met_para_shift_dtmc[6] = _gr_met_para_shift_dtmc[5];
      _gr_ratio_met_para_sigma_dtmc[6] = _gr_ratio_met_para_sigma_dtmc[5];
      _gr_ratio_met_perp_sigma_dtmc[6] = _gr_ratio_met_perp_sigma_dtmc[5];
    }
    else {
      Int_t idd=0;
      if (_doRecoilUseSmooth) idd=3;
      _h_dt_met_para_shift[6] = _h_dt_met_para_shift[idd];
      _h_mc_met_para_shift[6] = _h_mc_met_para_shift[idd];
      _h_met_para_shift_dtmc[6] = _h_met_para_shift_dtmc[idd];
      _h_ratio_met_para_sigma_dtmc[6] = _h_ratio_met_para_sigma_dtmc[idd];
      _h_ratio_met_perp_sigma_dtmc[6] = _h_ratio_met_perp_sigma_dtmc[idd];
      _gr_dt_met_para_shift[6] = _gr_dt_met_para_shift[3];
      _gr_mc_met_para_shift[6] = _gr_mc_met_para_shift[3];
      _gr_met_para_shift_dtmc[6] = _gr_met_para_shift_dtmc[3];
      _gr_ratio_met_para_sigma_dtmc[6] = _gr_ratio_met_para_sigma_dtmc[3];
      _gr_ratio_met_perp_sigma_dtmc[6] = _gr_ratio_met_perp_sigma_dtmc[3];
    }

    // peak shift
    if (_doRecoilUseSmoothGraph) {
      met_para += _gr_met_para_shift_dtmc[6]->Eval(_llnunu_l1_pt);
      if (_doGJetsSkim) {
        met_para_mu += _gr_met_para_shift_dtmc[7]->Eval(_llnunu_l1_pt);
        met_para_el += _gr_met_para_shift_dtmc[8]->Eval(_llnunu_l1_pt);
      }
    }
    else {
      met_para += _h_met_para_shift_dtmc[6]->GetBinContent(_h_met_para_shift_dtmc[6]->FindBin(_llnunu_l1_pt));
      if (_doGJetsSkim) {
        met_para_mu += _h_met_para_shift_dtmc[7]->GetBinContent(_h_met_para_shift_dtmc[7]->FindBin(_llnunu_l1_pt));
        met_para_el += _h_met_para_shift_dtmc[8]->GetBinContent(_h_met_para_shift_dtmc[8]->FindBin(_llnunu_l1_pt));
      }
    }

    // peak shift errors
    met_para_mean_up_reso_up = met_para + 0.5*(_h_met_para_shift_dtmc[6]->GetBinError(_h_met_para_shift_dtmc[6]->FindBin(_llnunu_l1_pt)));
    met_para_mean_up_reso_ct = met_para_mean_up_reso_up; 
    met_para_mean_up_reso_dn = met_para_mean_up_reso_up;
    met_para_mean_dn_reso_up = met_para - 0.5*(_h_met_para_shift_dtmc[6]->GetBinError(_h_met_para_shift_dtmc[6]->FindBin(_llnunu_l1_pt)));
    met_para_mean_dn_reso_ct = met_para_mean_dn_reso_up;
    met_para_mean_dn_reso_dn = met_para_mean_dn_reso_up;
    if (_doGJetsSkim) {
      met_para_mu_mean_up_reso_up = met_para_mu + 0.5*(_h_met_para_shift_dtmc[7]->GetBinError(_h_met_para_shift_dtmc[7]->FindBin(_llnunu_l1_pt)));
      met_para_mu_mean_up_reso_ct = met_para_mu_mean_up_reso_up;
      met_para_mu_mean_up_reso_dn = met_para_mu_mean_up_reso_up;
      met_para_mu_mean_dn_reso_up = met_para_mu - 0.5*(_h_met_para_shift_dtmc[7]->GetBinError(_h_met_para_shift_dtmc[7]->FindBin(_llnunu_l1_pt)));
      met_para_mu_mean_dn_reso_ct = met_para_mu_mean_dn_reso_up;
      met_para_mu_mean_dn_reso_dn = met_para_mu_mean_dn_reso_up;
      met_para_el_mean_up_reso_up = met_para_el + 0.5*(_h_met_para_shift_dtmc[8]->GetBinError(_h_met_para_shift_dtmc[8]->FindBin(_llnunu_l1_pt)));
      met_para_el_mean_up_reso_ct = met_para_el_mean_up_reso_up;
      met_para_el_mean_up_reso_dn = met_para_el_mean_up_reso_up;
      met_para_el_mean_dn_reso_up = met_para_el - 0.5*(_h_met_para_shift_dtmc[8]->GetBinError(_h_met_para_shift_dtmc[8]->FindBin(_llnunu_l1_pt)));
      met_para_el_mean_dn_reso_ct = met_para_el_mean_dn_reso_up;
      met_para_el_mean_dn_reso_dn = met_para_el_mean_dn_reso_up;
    }

    // smearing
    float m_para, r_para, r_para_er, r_para_up, r_para_dn;
    float r_perp, r_perp_er, r_perp_up, r_perp_dn;
    float m_para_mu, r_para_mu, r_para_mu_er, r_para_mu_up, r_para_mu_dn;
    float r_perp_mu, r_perp_mu_er, r_perp_mu_up, r_perp_mu_dn;
    float m_para_el, r_para_el, r_para_el_er, r_para_el_up, r_para_el_dn;
    float r_perp_el, r_perp_el_er, r_perp_el_up, r_perp_el_dn;

    if (_doRecoilUseSmoothGraph) {
      m_para    = _gr_dt_met_para_shift[6]->Eval(_llnunu_l1_pt);
      r_para    = _gr_ratio_met_para_sigma_dtmc[6]->Eval(_llnunu_l1_pt);
      r_perp    = _gr_ratio_met_perp_sigma_dtmc[6]->Eval(_llnunu_l1_pt);
      if (_doGJetsSkim) {
        m_para_mu = _gr_dt_met_para_shift[7]->Eval(_llnunu_l1_pt);
        r_para_mu = _gr_ratio_met_para_sigma_dtmc[7]->Eval(_llnunu_l1_pt);
        r_perp_mu = _gr_ratio_met_perp_sigma_dtmc[7]->Eval(_llnunu_l1_pt);
        m_para_el = _gr_dt_met_para_shift[8]->Eval(_llnunu_l1_pt);
        r_para_el = _gr_ratio_met_para_sigma_dtmc[8]->Eval(_llnunu_l1_pt);
        r_perp_el = _gr_ratio_met_perp_sigma_dtmc[8]->Eval(_llnunu_l1_pt);
      }
    }
    else {
      m_para    = _h_dt_met_para_shift[6]->GetBinContent(_h_dt_met_para_shift[6]->FindBin(_llnunu_l1_pt));
      r_para    = _h_ratio_met_para_sigma_dtmc[6]->GetBinContent(_h_ratio_met_para_sigma_dtmc[6]->FindBin(_llnunu_l1_pt));
      r_perp    = _h_ratio_met_perp_sigma_dtmc[6]->GetBinContent(_h_ratio_met_perp_sigma_dtmc[6]->FindBin(_llnunu_l1_pt));
      if (_doGJetsSkim) {
        m_para_mu = _h_dt_met_para_shift[7]->GetBinContent(_h_dt_met_para_shift[7]->FindBin(_llnunu_l1_pt));
        r_para_mu = _h_ratio_met_para_sigma_dtmc[7]->GetBinContent(_h_ratio_met_para_sigma_dtmc[7]->FindBin(_llnunu_l1_pt));
        r_perp_mu = _h_ratio_met_perp_sigma_dtmc[7]->GetBinContent(_h_ratio_met_perp_sigma_dtmc[7]->FindBin(_llnunu_l1_pt));
        m_para_el = _h_dt_met_para_shift[8]->GetBinContent(_h_dt_met_para_shift[8]->FindBin(_llnunu_l1_pt));
        r_para_el = _h_ratio_met_para_sigma_dtmc[8]->GetBinContent(_h_ratio_met_para_sigma_dtmc[8]->FindBin(_llnunu_l1_pt));
        r_perp_el = _h_ratio_met_perp_sigma_dtmc[8]->GetBinContent(_h_ratio_met_perp_sigma_dtmc[8]->FindBin(_llnunu_l1_pt));
      }
    }

    // error from the hists
    r_para_er = _h_ratio_met_para_sigma_dtmc[6]->GetBinError(_h_ratio_met_para_sigma_dtmc[6]->FindBin(_llnunu_l1_pt));
    r_para_up = r_para+0.5*r_para_er;
    r_para_dn = r_para-0.5*r_para_er;
    r_perp_er = _h_ratio_met_perp_sigma_dtmc[6]->GetBinError(_h_ratio_met_perp_sigma_dtmc[6]->FindBin(_llnunu_l1_pt));
    r_perp_up = r_perp+0.5*r_perp_er;
    r_perp_dn = r_perp-0.5*r_perp_er;

    if (_doGJetsSkim){
      r_para_mu_er = _h_ratio_met_para_sigma_dtmc[7]->GetBinError(_h_ratio_met_para_sigma_dtmc[7]->FindBin(_llnunu_l1_pt));
      r_para_mu_up = r_para_mu+0.5*r_para_mu_er;
      r_para_mu_dn = r_para_mu-0.5*r_para_mu_er;
      r_perp_mu_er = _h_ratio_met_perp_sigma_dtmc[7]->GetBinError(_h_ratio_met_perp_sigma_dtmc[7]->FindBin(_llnunu_l1_pt));
      r_perp_mu_up = r_perp_mu+0.5*r_perp_mu_er;
      r_perp_mu_dn = r_perp_mu-0.5*r_perp_mu_er;
      r_para_el_er = _h_ratio_met_para_sigma_dtmc[8]->GetBinError(_h_ratio_met_para_sigma_dtmc[8]->FindBin(_llnunu_l1_pt));
      r_para_el_up = r_para_el+0.5*r_para_el_er;
      r_para_el_dn = r_para_el-0.5*r_para_el_er;
      r_perp_el_er = _h_ratio_met_perp_sigma_dtmc[8]->GetBinError(_h_ratio_met_perp_sigma_dtmc[8]->FindBin(_llnunu_l1_pt));
      r_perp_el_up = r_perp_el+0.5*r_perp_el_er;
      r_perp_el_dn = r_perp_el-0.5*r_perp_el_er;

    }
    
    // mean
    met_para = (met_para-m_para)*r_para+m_para;
    met_perp *= r_perp;

    // error
    met_para_mean_up_reso_up = (met_para_mean_up_reso_up - m_para) * r_para_up + m_para;
    met_para_mean_up_reso_ct = (met_para_mean_up_reso_ct - m_para) * r_para    + m_para;
    met_para_mean_up_reso_dn = (met_para_mean_up_reso_dn - m_para) * r_para_dn + m_para;
    met_para_mean_ct_reso_up = (met_para_mean_ct_reso_up - m_para) * r_para_up + m_para;
    met_para_mean_ct_reso_dn = (met_para_mean_ct_reso_dn - m_para) * r_para_dn + m_para;
    met_para_mean_dn_reso_up = (met_para_mean_dn_reso_up - m_para) * r_para_up + m_para;
    met_para_mean_dn_reso_ct = (met_para_mean_dn_reso_ct - m_para) * r_para    + m_para;
    met_para_mean_dn_reso_dn = (met_para_mean_dn_reso_dn - m_para) * r_para_dn + m_para;
    met_perp_mean_ct_reso_up *= r_perp_up;
    met_perp_mean_ct_reso_dn *= r_perp_dn;

    if (_doGJetsSkim) {
      // mean
      met_para_mu  = (met_para_mu-m_para_mu)*r_para_mu+m_para_mu;
      met_perp_mu *= r_perp_mu;
      met_para_el  = (met_para_el-m_para_el)*r_para_el+m_para_el;
      met_perp_el *= r_perp_el;

      // error
      met_para_mu_mean_up_reso_up = (met_para_mu_mean_up_reso_up - m_para_mu) * r_para_mu_up + m_para_mu;
      met_para_mu_mean_up_reso_ct = (met_para_mu_mean_up_reso_ct - m_para_mu) * r_para_mu    + m_para_mu;
      met_para_mu_mean_up_reso_dn = (met_para_mu_mean_up_reso_dn - m_para_mu) * r_para_mu_dn + m_para_mu;
      met_para_mu_mean_ct_reso_up = (met_para_mu_mean_ct_reso_up - m_para_mu) * r_para_mu_up + m_para_mu;
      met_para_mu_mean_ct_reso_dn = (met_para_mu_mean_ct_reso_dn - m_para_mu) * r_para_mu_dn + m_para_mu;
      met_para_mu_mean_dn_reso_up = (met_para_mu_mean_dn_reso_up - m_para_mu) * r_para_mu_up + m_para_mu;
      met_para_mu_mean_dn_reso_ct = (met_para_mu_mean_dn_reso_ct - m_para_mu) * r_para_mu    + m_para_mu;
      met_para_mu_mean_dn_reso_dn = (met_para_mu_mean_dn_reso_dn - m_para_mu) * r_para_mu_dn + m_para_mu;
      met_perp_mu_mean_ct_reso_up *= r_perp_mu_up;
      met_perp_mu_mean_ct_reso_dn *= r_perp_mu_dn;
      met_para_el_mean_up_reso_up = (met_para_el_mean_up_reso_up - m_para_el) * r_para_el_up + m_para_el;
      met_para_el_mean_up_reso_ct = (met_para_el_mean_up_reso_ct - m_para_el) * r_para_el    + m_para_el;
      met_para_el_mean_up_reso_dn = (met_para_el_mean_up_reso_dn - m_para_el) * r_para_el_dn + m_para_el;
      met_para_el_mean_ct_reso_up = (met_para_el_mean_ct_reso_up - m_para_el) * r_para_el_up + m_para_el;
      met_para_el_mean_ct_reso_dn = (met_para_el_mean_ct_reso_dn - m_para_el) * r_para_el_dn + m_para_el;
      met_para_el_mean_dn_reso_up = (met_para_el_mean_dn_reso_up - m_para_el) * r_para_el_up + m_para_el;
      met_para_el_mean_dn_reso_ct = (met_para_el_mean_dn_reso_ct - m_para_el) * r_para_el    + m_para_el;
      met_para_el_mean_dn_reso_dn = (met_para_el_mean_dn_reso_dn - m_para_el) * r_para_el_dn + m_para_el;
      met_perp_el_mean_ct_reso_up *= r_perp_el_up;
      met_perp_el_mean_ct_reso_dn *= r_perp_el_dn;

    }

    // recalculate vars
    Float_t met_px = met_para*cos(_llnunu_l1_phi)-met_perp*sin(_llnunu_l1_phi);
    Float_t met_py = met_para*sin(_llnunu_l1_phi)+met_perp*cos(_llnunu_l1_phi);
    TVector2 vec_met(met_px,met_py);
    _llnunu_l2_pt = vec_met.Mod();
    _llnunu_l2_phi = TVector2::Phi_mpi_pi(vec_met.Phi());
    _llnunu_mt = MTCalc(_llnunu_l2_pt,_llnunu_l2_phi);  

    if (_doGJetsSkim){
      //mu
      met_px = met_para_mu*cos(_llnunu_l1_phi)-met_perp_mu*sin(_llnunu_l1_phi);
      met_py = met_para_mu*sin(_llnunu_l1_phi)+met_perp_mu*cos(_llnunu_l1_phi);
      vec_met.Set(met_px, met_py);
      _llnunu_l2_pt_mu = vec_met.Mod();
      _llnunu_l2_phi_mu = TVector2::Phi_mpi_pi(vec_met.Phi());
      _llnunu_mt_mu = MTCalcMu(_llnunu_l2_pt_mu,_llnunu_l2_phi_mu);
      //el
      met_px = met_para_el*cos(_llnunu_l1_phi)-met_perp_el*sin(_llnunu_l1_phi);
      met_py = met_para_el*sin(_llnunu_l1_phi)+met_perp_el*cos(_llnunu_l1_phi);
      vec_met.Set(met_px, met_py);
      _llnunu_l2_pt_el = vec_met.Mod();
      _llnunu_l2_phi_el = TVector2::Phi_mpi_pi(vec_met.Phi());
      _llnunu_mt_el = MTCalcEl(_llnunu_l2_pt_el,_llnunu_l2_phi_el);
    }


    // error propagation
    float met_para_variations[9] = {
          met_para,
          met_para_mean_up_reso_up,
          met_para_mean_up_reso_ct,
          met_para_mean_up_reso_dn,
          met_para_mean_ct_reso_up,
          met_para_mean_ct_reso_dn,
          met_para_mean_dn_reso_up,
          met_para_mean_dn_reso_ct,
          met_para_mean_dn_reso_dn
         };
    float met_perp_variations[3] = {
          met_perp,
          met_perp_mean_ct_reso_up,
          met_perp_mean_ct_reso_dn
         };

    _llnunu_l2_pt_RecoilUp = _llnunu_l2_pt;
    _llnunu_l2_pt_RecoilDn = _llnunu_l2_pt;
    _llnunu_l2_phi_RecoilUp = _llnunu_l2_phi;
    _llnunu_l2_phi_RecoilDn = _llnunu_l2_phi;
    _llnunu_mt_RecoilUp = _llnunu_mt;
    _llnunu_mt_RecoilDn = _llnunu_mt;
    for (int i_para=0; i_para<9; i_para++){
      for (int i_perp=0; i_perp<3; i_perp++){
        float xx = met_para_variations[i_para]*cos(_llnunu_l1_phi)-met_perp_variations[i_perp]*sin(_llnunu_l1_phi);
        float yy = met_para_variations[i_para]*sin(_llnunu_l1_phi)+met_perp_variations[i_perp]*cos(_llnunu_l1_phi);
        TVector2 tmp_vec(xx,yy);    
        float pt = tmp_vec.Mod();
        float phi = TVector2::Phi_mpi_pi(tmp_vec.Phi());
        float mt = MTCalc(pt, phi);
        if (pt>_llnunu_l2_pt_RecoilUp) _llnunu_l2_pt_RecoilUp = pt;
        if (pt<_llnunu_l2_pt_RecoilDn) _llnunu_l2_pt_RecoilDn = pt;
        if (TVector2::Phi_mpi_pi(phi-_llnunu_l2_phi_RecoilUp)>0.0) _llnunu_l2_phi_RecoilUp = phi;
        if (TVector2::Phi_mpi_pi(phi-_llnunu_l2_phi_RecoilDn)<0.0) _llnunu_l2_phi_RecoilDn = phi;
        if (mt>_llnunu_mt_RecoilUp) _llnunu_mt_RecoilUp = mt;
        if (mt<_llnunu_mt_RecoilDn) _llnunu_mt_RecoilDn = mt;
      }
    }

    if (_doGJetsSkim){
      //mu
      float met_para_mu_variations[9] = {
          met_para_mu,
          met_para_mu_mean_up_reso_up,
          met_para_mu_mean_up_reso_ct,
          met_para_mu_mean_up_reso_dn,
          met_para_mu_mean_ct_reso_up,
          met_para_mu_mean_ct_reso_dn,
          met_para_mu_mean_dn_reso_up,
          met_para_mu_mean_dn_reso_ct,
          met_para_mu_mean_dn_reso_dn
         };
      float met_perp_mu_variations[3] = {
          met_perp_mu,
          met_perp_mu_mean_ct_reso_up,
          met_perp_mu_mean_ct_reso_dn
         };

      _llnunu_l2_pt_mu_RecoilUp = _llnunu_l2_pt_mu;
      _llnunu_l2_pt_mu_RecoilDn = _llnunu_l2_pt_mu;
      _llnunu_l2_phi_mu_RecoilUp = _llnunu_l2_phi_mu;
      _llnunu_l2_phi_mu_RecoilDn = _llnunu_l2_phi_mu;
      _llnunu_mt_mu_RecoilUp = _llnunu_mt_mu;
      _llnunu_mt_mu_RecoilDn = _llnunu_mt_mu;
      for (int i_para=0; i_para<9; i_para++){
        for (int i_perp=0; i_perp<3; i_perp++){
          float xx = met_para_mu_variations[i_para]*cos(_llnunu_l1_phi)-met_perp_mu_variations[i_perp]*sin(_llnunu_l1_phi);
          float yy = met_para_mu_variations[i_para]*sin(_llnunu_l1_phi)+met_perp_mu_variations[i_perp]*cos(_llnunu_l1_phi);
          TVector2 tmp_vec(xx,yy);
          float pt = tmp_vec.Mod();
          float phi = TVector2::Phi_mpi_pi(tmp_vec.Phi());
          float mt = MTCalcMu(pt, phi);
          if (pt>_llnunu_l2_pt_mu_RecoilUp) _llnunu_l2_pt_RecoilUp = pt;
          if (pt<_llnunu_l2_pt_mu_RecoilDn) _llnunu_l2_pt_RecoilDn = pt;
          if (TVector2::Phi_mpi_pi(phi-_llnunu_l2_phi_mu_RecoilUp)>0.0) _llnunu_l2_phi_mu_RecoilUp = phi;
          if (TVector2::Phi_mpi_pi(phi-_llnunu_l2_phi_mu_RecoilDn)<0.0) _llnunu_l2_phi_mu_RecoilDn = phi;
          if (mt>_llnunu_mt_mu_RecoilUp) _llnunu_mt_mu_RecoilUp = mt;
          if (mt<_llnunu_mt_mu_RecoilDn) _llnunu_mt_mu_RecoilDn = mt;
        }
      }
      //el
      float met_para_el_variations[9] = {
          met_para_el,
          met_para_el_mean_up_reso_up,
          met_para_el_mean_up_reso_ct,
          met_para_el_mean_up_reso_dn,
          met_para_el_mean_ct_reso_up,
          met_para_el_mean_ct_reso_dn,
          met_para_el_mean_dn_reso_up,
          met_para_el_mean_dn_reso_ct,
          met_para_el_mean_dn_reso_dn
         };
      float met_perp_el_variations[3] = {
          met_perp_el,
          met_perp_el_mean_ct_reso_up,
          met_perp_el_mean_ct_reso_dn
         };

      _llnunu_l2_pt_el_RecoilUp = _llnunu_l2_pt_el;
      _llnunu_l2_pt_el_RecoilDn = _llnunu_l2_pt_el;
      _llnunu_l2_phi_el_RecoilUp = _llnunu_l2_phi_el;
      _llnunu_l2_phi_el_RecoilDn = _llnunu_l2_phi_el;
      _llnunu_mt_el_RecoilUp = _llnunu_mt_el;
      _llnunu_mt_el_RecoilDn = _llnunu_mt_el;
      for (int i_para=0; i_para<9; i_para++){
        for (int i_perp=0; i_perp<3; i_perp++){
          float xx = met_para_el_variations[i_para]*cos(_llnunu_l1_phi)-met_perp_el_variations[i_perp]*sin(_llnunu_l1_phi);
          float yy = met_para_el_variations[i_para]*sin(_llnunu_l1_phi)+met_perp_el_variations[i_perp]*cos(_llnunu_l1_phi);
          TVector2 tmp_vec(xx,yy);
          float pt = tmp_vec.Mod();
          float phi = TVector2::Phi_mpi_pi(tmp_vec.Phi());
          float mt = MTCalcMu(pt, phi);
          if (pt>_llnunu_l2_pt_el_RecoilUp) _llnunu_l2_pt_RecoilUp = pt;
          if (pt<_llnunu_l2_pt_el_RecoilDn) _llnunu_l2_pt_RecoilDn = pt;
          if (TVector2::Phi_mpi_pi(phi-_llnunu_l2_phi_el_RecoilUp)>0.0) _llnunu_l2_phi_el_RecoilUp = phi;
          if (TVector2::Phi_mpi_pi(phi-_llnunu_l2_phi_el_RecoilDn)<0.0) _llnunu_l2_phi_el_RecoilDn = phi;
          if (mt>_llnunu_mt_el_RecoilUp) _llnunu_mt_el_RecoilUp = mt;
          if (mt<_llnunu_mt_el_RecoilDn) _llnunu_mt_el_RecoilDn = mt;
        }
      }

    }



  }
  else if (_doGJetsSkim) {
    // if doGJets but not gjets data, create dummy branches.  
    _llnunu_l2_pt_RecoilUp = _llnunu_l2_pt;
    _llnunu_l2_pt_RecoilDn = _llnunu_l2_pt;
    _llnunu_l2_phi_RecoilUp = _llnunu_l2_phi;
    _llnunu_l2_phi_RecoilDn = _llnunu_l2_phi;
    _llnunu_mt_RecoilUp = _llnunu_mt;
    _llnunu_mt_RecoilDn = _llnunu_mt;

    _llnunu_l2_pt_mu_RecoilUp = _llnunu_l2_pt_mu;
    _llnunu_l2_pt_mu_RecoilDn = _llnunu_l2_pt_mu;
    _llnunu_l2_phi_mu_RecoilUp = _llnunu_l2_phi_mu;
    _llnunu_l2_phi_mu_RecoilDn = _llnunu_l2_phi_mu;
    _llnunu_mt_mu_RecoilUp = _llnunu_mt_mu;
    _llnunu_mt_mu_RecoilDn = _llnunu_mt_mu;

    _llnunu_l2_pt_el_RecoilUp = _llnunu_l2_pt_el;
    _llnunu_l2_pt_el_RecoilDn = _llnunu_l2_pt_el;
    _llnunu_l2_phi_el_RecoilUp = _llnunu_l2_phi_el;
    _llnunu_l2_phi_el_RecoilDn = _llnunu_l2_phi_el;
    _llnunu_mt_el_RecoilUp = _llnunu_mt_el;
    _llnunu_mt_el_RecoilDn = _llnunu_mt_el;


  }

}



// prepareEffScale
void prepareEffScale()
{

  // needed branches 
  _tree_out->Branch("trgsf", &_trgsf, "trgsf/F");
  //_tree_out->Branch("trgmu50tkmu50sf", &_trgmu50tkmu50sf, "trgmu50tkmu50sf/F");
  _tree_out->Branch("isosf", &_isosf, "isosf/F");
  _tree_out->Branch("idsf", &_idsf, "idsf/F");
  _tree_out->Branch("trksf", &_trksf, "trksf/F");
  _tree_out->Branch("trgsf_err", &_trgsf_err, "trgsf_err/F");
  _tree_out->Branch("isosf_err", &_isosf_err, "isosf_err/F");
  _tree_out->Branch("idsf_err", &_idsf_err, "idsf_err/F");
  _tree_out->Branch("trksf_err", &_trksf_err, "trksf_err/F");
  _tree_out->Branch("trgsf_up", &_trgsf_up, "trgsf_up/F");
  _tree_out->Branch("trgsf_dn", &_trgsf_dn, "trgsf_dn/F");
  _tree_out->Branch("idisotrksf", &_idisotrksf, "idisotrksf/F");
  _tree_out->Branch("idisotrksf_up", &_idisotrksf_up, "idisotrksf_up/F");
  _tree_out->Branch("idisotrksf_dn", &_idisotrksf_dn, "idisotrksf_dn/F");

  // Electron ID ISO scale factors 
  _file_idiso_el = TFile::Open(_EffScaleInputFileName_IdIso_El.c_str());
  _h_sf_idiso_el = (TH2F*)_file_idiso_el->Get("EGamma_SF2D");

  // Electron tracking scale factors
  _file_trksf_el = TFile::Open(_EffScaleInputFileName_Trk_El.c_str());
  _h_sf_trk_el = (TH2F*)_file_trksf_el->Get("EGamma_SF2D");

  // muon tracking scale factors
  _file_trksf_mu = TFile::Open(_EffScaleInputFileName_Trk_Mu.c_str());
  _h_sf_trk_mu = (TH1F*)_file_trksf_mu->Get("hist_ratio_eta");

  // muon id iso scale factors
  _file_idiso_mu = TFile::Open(_EffScaleInputFileName_IdIso_Mu.c_str());

  // options
  if (_EffScaleMCVersion=="80xSpring16") {
    _h_eff_trkhpt_mu_dt = (TH2F*)_file_idiso_mu->Get("eff_trackHighPt_80Xdata_pteta");
    _h_eff_trkhpt_mu_mc = (TH2F*)_file_idiso_mu->Get("eff_trackHighPt_80Xmc_pteta");
    _h_eff_hpt_mu_dt = (TH2F*)_file_idiso_mu->Get("eff_HighPt_80Xdata_pteta");
    _h_eff_hpt_mu_mc = (TH2F*)_file_idiso_mu->Get("eff_HighPt_80Xmc_pteta");
    _h_sf_iso_mu = (TH2F*)_file_idiso_mu->Get("sf_trackerIso_80X_pteta");
  }
  else {
    _h_eff_trkhpt_mu_dt_1 = (TH2*)_file_idiso_mu->Get("h_mu_tkhpt_data_1");
    _h_eff_trkhpt_mu_dt_2 = (TH2*)_file_idiso_mu->Get("h_mu_tkhpt_data_2");
    _h_eff_trkhpt_mu_mc_1 = (TH2*)_file_idiso_mu->Get("h_mu_tkhpt_mc_1");
    _h_eff_trkhpt_mu_mc_2 = (TH2*)_file_idiso_mu->Get("h_mu_tkhpt_mc_2");
    _h_eff_hpt_mu_dt_1 = (TH2*)_file_idiso_mu->Get("h_mu_hpt_data_1");
    _h_eff_hpt_mu_dt_2 = (TH2*)_file_idiso_mu->Get("h_mu_hpt_data_2");
    _h_eff_hpt_mu_mc_1 = (TH2*)_file_idiso_mu->Get("h_mu_hpt_mc_1");
    _h_eff_hpt_mu_mc_2 = (TH2*)_file_idiso_mu->Get("h_mu_hpt_mc_2");
    _h_sf_iso_mu_1 = (TH2*)_file_idiso_mu->Get("h_mu_iso_sf_1");
    _h_sf_iso_mu_2 = (TH2*)_file_idiso_mu->Get("h_mu_iso_sf_2");
  }

  // electron trigger scale factors
  _file_trg_el = TFile::Open(_EffScaleInputFileName_Trg_El.c_str());
  if (_EffScaleMCVersion=="80xSpring16") {
    _h_sf_trg_el_l1=(TH2D*)_file_trg_el->Get("ell1pteta"); 
  }
  else {
    _h_sf_trg_el_l1=(TH2*)_file_trg_el->Get("scalefactor");
  }

  // muon trigger scale factors
  _file_trg_mu = TFile::Open(_EffScaleInputFileName_Trg_Mu.c_str());

  // options
  if (_EffScaleMCVersion=="80xSpring16") {
    _h_eff_trg_mu_l1_tot = (TH2D*)_file_trg_mu->Get("htrg_l1_tot");
    _h_eff_trg_mu_l2_tot = (TH2D*)_file_trg_mu->Get("htrg_l2_tot");
    _h_eff_trg_mu_l1_l1p = (TH2D*)_file_trg_mu->Get("htrg_l1_l1p");
    _h_eff_trg_mu_l2_l1p = (TH2D*)_file_trg_mu->Get("htrg_l2_l1p");
    _h_eff_trg_mu_l1_l1f = (TH2D*)_file_trg_mu->Get("htrg_l1_l1f");
    _h_eff_trg_mu_l2_l1f = (TH2D*)_file_trg_mu->Get("htrg_l2_l1f");
    _h_eff_trg_mu_l1_l1pl2f = (TH2D*)_file_trg_mu->Get("htrg_l1_l1pl2f");
    _h_eff_trg_mu_l1_l1pl2p = (TH2D*)_file_trg_mu->Get("htrg_l1_l1pl2p");
    _h_eff_trg_mu_l1_l1fl2p = (TH2D*)_file_trg_mu->Get("htrg_l1_l1fl2p");
    _h_eff_trg_mu_l2_l1pl2f = (TH2D*)_file_trg_mu->Get("htrg_l2_l1pl2f");
    _h_eff_trg_mu_l2_l1pl2p = (TH2D*)_file_trg_mu->Get("htrg_l2_l1pl2p");
    _h_eff_trg_mu_l2_l1fl2p = (TH2D*)_file_trg_mu->Get("htrg_l2_l1fl2p");

    _NPtBins_eff_trg_mu = _h_eff_trg_mu_l2_tot->GetNbinsX();
    _NEtaBins_eff_trg_mu = _h_eff_trg_mu_l2_tot->GetNbinsY();
    _N_eff_trg_mu_tot = _h_eff_trg_mu_l2_tot->IntegralAndError(Int_t(1), _NPtBins_eff_trg_mu, Int_t(1), _NEtaBins_eff_trg_mu, _N_eff_trg_mu_tot_err);
    _N_eff_trg_mu_l1p = _h_eff_trg_mu_l2_l1p->IntegralAndError(Int_t(1), _NPtBins_eff_trg_mu, Int_t(1), _NEtaBins_eff_trg_mu, _N_eff_trg_mu_l1p_err);
    _N_eff_trg_mu_l1f = _h_eff_trg_mu_l2_l1f->IntegralAndError(Int_t(1), _NPtBins_eff_trg_mu, Int_t(1), _NEtaBins_eff_trg_mu, _N_eff_trg_mu_l1f_err);
    _N_eff_trg_mu_l1pl2f = _h_eff_trg_mu_l2_l1pl2f->IntegralAndError(Int_t(1), _NPtBins_eff_trg_mu, Int_t(1), _NEtaBins_eff_trg_mu, _N_eff_trg_mu_l1pl2f_err);
    _N_eff_trg_mu_l1pl2p = _h_eff_trg_mu_l2_l1pl2p->IntegralAndError(Int_t(1), _NPtBins_eff_trg_mu, Int_t(1), _NEtaBins_eff_trg_mu, _N_eff_trg_mu_l1pl2p_err);
    _N_eff_trg_mu_l1fl2p = _h_eff_trg_mu_l2_l1fl2p->IntegralAndError(Int_t(1), _NPtBins_eff_trg_mu, Int_t(1), _NEtaBins_eff_trg_mu, _N_eff_trg_mu_l1fl2p_err);

    _h_eff_trg_mu_l1_tot_norm = (TH2D*)_h_eff_trg_mu_l1_tot->Clone("htrg_l1_tot_norm");
    _h_eff_trg_mu_l2_tot_norm = (TH2D*)_h_eff_trg_mu_l2_tot->Clone("htrg_l2_tot_norm");
    _h_eff_trg_mu_l1_l1p_norm = (TH2D*)_h_eff_trg_mu_l1_l1p->Clone("htrg_l1_l1p_norm");
    _h_eff_trg_mu_l1_l1f_norm = (TH2D*)_h_eff_trg_mu_l1_l1f->Clone("htrg_l1_l1f_norm");
    _h_eff_trg_mu_l2_l1p_norm = (TH2D*)_h_eff_trg_mu_l2_l1p->Clone("htrg_l2_l1p_norm");
    _h_eff_trg_mu_l2_l1f_norm = (TH2D*)_h_eff_trg_mu_l2_l1f->Clone("htrg_l2_l1f_norm");
    _h_eff_trg_mu_l1_l1pl2f_norm = (TH2D*)_h_eff_trg_mu_l1_l1pl2f->Clone("htrg_l1_l1pl2f_norm");
    _h_eff_trg_mu_l1_l1pl2p_norm = (TH2D*)_h_eff_trg_mu_l1_l1pl2p->Clone("htrg_l1_l1pl2p_norm");
    _h_eff_trg_mu_l1_l1fl2p_norm = (TH2D*)_h_eff_trg_mu_l1_l1fl2p->Clone("htrg_l1_l1fl2p_norm");
    _h_eff_trg_mu_l2_l1pl2f_norm = (TH2D*)_h_eff_trg_mu_l2_l1pl2f->Clone("htrg_l2_l1pl2f_norm");
    _h_eff_trg_mu_l2_l1pl2p_norm = (TH2D*)_h_eff_trg_mu_l2_l1pl2p->Clone("htrg_l2_l1pl2p_norm");
    _h_eff_trg_mu_l2_l1fl2p_norm = (TH2D*)_h_eff_trg_mu_l2_l1fl2p->Clone("htrg_l2_l1fl2p_norm");


    _h_eff_trg_mu_l1_tot_norm->Scale(1./_N_eff_trg_mu_tot);
    _h_eff_trg_mu_l2_tot_norm->Scale(1./_N_eff_trg_mu_tot);
    _h_eff_trg_mu_l1_l1p_norm->Scale(1./_N_eff_trg_mu_l1p);
    _h_eff_trg_mu_l1_l1f_norm->Scale(1./_N_eff_trg_mu_l1f);
    _h_eff_trg_mu_l2_l1p_norm->Scale(1./_N_eff_trg_mu_l1p);
    _h_eff_trg_mu_l2_l1f_norm->Scale(1./_N_eff_trg_mu_l1f);
    _h_eff_trg_mu_l1_l1pl2f_norm->Scale(1./_N_eff_trg_mu_l1pl2f);
    _h_eff_trg_mu_l1_l1pl2p_norm->Scale(1./_N_eff_trg_mu_l1pl2p);
    _h_eff_trg_mu_l1_l1fl2p_norm->Scale(1./_N_eff_trg_mu_l1fl2p);
    _h_eff_trg_mu_l2_l1pl2f_norm->Scale(1./_N_eff_trg_mu_l1pl2f);
    _h_eff_trg_mu_l2_l1pl2p_norm->Scale(1./_N_eff_trg_mu_l1pl2p);
    _h_eff_trg_mu_l2_l1fl2p_norm->Scale(1./_N_eff_trg_mu_l1fl2p);

    _h_eff_trg_mu_l1_l1p_norm_vs_tot    = (TH2D*)_h_eff_trg_mu_l1_l1p_norm->Clone("htrg_l1_l1p_norm_vs_tot");
    _h_eff_trg_mu_l1_l1f_norm_vs_tot    = (TH2D*)_h_eff_trg_mu_l1_l1f_norm->Clone("htrg_l1_l1f_norm_vs_tot");
    _h_eff_trg_mu_l2_l1p_norm_vs_tot    = (TH2D*)_h_eff_trg_mu_l2_l1p_norm->Clone("htrg_l2_l1p_norm_vs_tot");
    _h_eff_trg_mu_l2_l1f_norm_vs_tot    = (TH2D*)_h_eff_trg_mu_l2_l1f_norm->Clone("htrg_l2_l1f_norm_vs_tot");
    _h_eff_trg_mu_l1_l1pl2f_norm_vs_tot = (TH2D*)_h_eff_trg_mu_l1_l1pl2f_norm->Clone("htrg_l1_l1pl2f_norm_vs_tot");
    _h_eff_trg_mu_l1_l1pl2p_norm_vs_tot = (TH2D*)_h_eff_trg_mu_l1_l1pl2p_norm->Clone("htrg_l1_l1pl2p_norm_vs_tot");
    _h_eff_trg_mu_l1_l1fl2p_norm_vs_tot = (TH2D*)_h_eff_trg_mu_l1_l1fl2p_norm->Clone("htrg_l1_l1fl2p_norm_vs_tot");
    _h_eff_trg_mu_l2_l1pl2f_norm_vs_tot = (TH2D*)_h_eff_trg_mu_l2_l1pl2f_norm->Clone("htrg_l2_l1pl2f_norm_vs_tot");
    _h_eff_trg_mu_l2_l1pl2p_norm_vs_tot = (TH2D*)_h_eff_trg_mu_l2_l1pl2p_norm->Clone("htrg_l2_l1pl2p_norm_vs_tot");
    _h_eff_trg_mu_l2_l1fl2p_norm_vs_tot = (TH2D*)_h_eff_trg_mu_l2_l1fl2p_norm->Clone("htrg_l2_l1fl2p_norm_vs_tot");
    _h_eff_trg_mu_l1_l1pl2f_norm_vs_l1p = (TH2D*)_h_eff_trg_mu_l1_l1pl2f_norm->Clone("htrg_l1_l1pl2f_norm_vs_l1p");
    _h_eff_trg_mu_l1_l1pl2p_norm_vs_l1p = (TH2D*)_h_eff_trg_mu_l1_l1pl2p_norm->Clone("htrg_l1_l1pl2p_norm_vs_l1p");
    _h_eff_trg_mu_l1_l1fl2p_norm_vs_l1f = (TH2D*)_h_eff_trg_mu_l1_l1fl2p_norm->Clone("htrg_l1_l1fl2p_norm_vs_l1f");
    _h_eff_trg_mu_l2_l1pl2f_norm_vs_l1p = (TH2D*)_h_eff_trg_mu_l2_l1pl2f_norm->Clone("htrg_l2_l1pl2f_norm_vs_l1p");
    _h_eff_trg_mu_l2_l1pl2p_norm_vs_l1p = (TH2D*)_h_eff_trg_mu_l2_l1pl2p_norm->Clone("htrg_l2_l1pl2p_norm_vs_l1p");
    _h_eff_trg_mu_l2_l1fl2p_norm_vs_l1f = (TH2D*)_h_eff_trg_mu_l2_l1fl2p_norm->Clone("htrg_l2_l1fl2p_norm_vs_l1f");

    _h_eff_trg_mu_l1_l1p_norm_vs_tot->Divide(_h_eff_trg_mu_l1_tot_norm);
    _h_eff_trg_mu_l1_l1f_norm_vs_tot->Divide(_h_eff_trg_mu_l1_tot_norm);
    _h_eff_trg_mu_l2_l1p_norm_vs_tot->Divide(_h_eff_trg_mu_l2_tot_norm);
    _h_eff_trg_mu_l2_l1f_norm_vs_tot->Divide(_h_eff_trg_mu_l2_tot_norm);
    _h_eff_trg_mu_l1_l1pl2f_norm_vs_tot->Divide(_h_eff_trg_mu_l1_tot_norm);
    _h_eff_trg_mu_l1_l1pl2p_norm_vs_tot->Divide(_h_eff_trg_mu_l1_tot_norm);
    _h_eff_trg_mu_l1_l1fl2p_norm_vs_tot->Divide(_h_eff_trg_mu_l1_tot_norm);
    _h_eff_trg_mu_l2_l1pl2f_norm_vs_tot->Divide(_h_eff_trg_mu_l2_tot_norm);
    _h_eff_trg_mu_l2_l1pl2p_norm_vs_tot->Divide(_h_eff_trg_mu_l2_tot_norm);
    _h_eff_trg_mu_l2_l1fl2p_norm_vs_tot->Divide(_h_eff_trg_mu_l2_tot_norm);
    _h_eff_trg_mu_l1_l1pl2f_norm_vs_l1p->Divide(_h_eff_trg_mu_l1_l1p_norm);
    _h_eff_trg_mu_l1_l1pl2p_norm_vs_l1p->Divide(_h_eff_trg_mu_l1_l1p_norm);
    _h_eff_trg_mu_l1_l1fl2p_norm_vs_l1f->Divide(_h_eff_trg_mu_l1_l1f_norm);
    _h_eff_trg_mu_l2_l1pl2f_norm_vs_l1p->Divide(_h_eff_trg_mu_l2_l1p_norm);
    _h_eff_trg_mu_l2_l1pl2p_norm_vs_l1p->Divide(_h_eff_trg_mu_l2_l1p_norm);
    _h_eff_trg_mu_l2_l1fl2p_norm_vs_l1f->Divide(_h_eff_trg_mu_l2_l1f_norm);

  }
  else {
    _h_eff_trg_mu50_dt_1 = (TH2*)_file_trg_mu->Get("h_eff_trg_mu50_dt_1");
    _h_eff_trg_mu50_dt_2 = (TH2*)_file_trg_mu->Get("h_eff_trg_mu50_dt_2");
    _h_eff_trg_mu50_dt_3 = (TH2*)_file_trg_mu->Get("h_eff_trg_mu50_dt_3");
    _h_eff_trg_mu50_dt_4 = (TH2*)_file_trg_mu->Get("h_eff_trg_mu50_dt_4");
    _h_eff_trg_mu50_mc_1 = (TH2*)_file_trg_mu->Get("h_eff_trg_mu50_mc_1");
    _h_eff_trg_mu50_mc_2 = (TH2*)_file_trg_mu->Get("h_eff_trg_mu50_mc_2");
    _h_eff_trg_mu50_mc_3 = (TH2*)_file_trg_mu->Get("h_eff_trg_mu50_mc_3");
    _h_eff_trg_mu50_mc_4 = (TH2*)_file_trg_mu->Get("h_eff_trg_mu50_mc_4");
    _h_eff_trg_mu50tkmu50_dt_1 = (TH2*)_file_trg_mu->Get("h_eff_trg_mu50tkmu50_dt_1");
    _h_eff_trg_mu50tkmu50_dt_2 = (TH2*)_file_trg_mu->Get("h_eff_trg_mu50tkmu50_dt_2");
    _h_eff_trg_mu50tkmu50_dt_3 = (TH2*)_file_trg_mu->Get("h_eff_trg_mu50tkmu50_dt_3");
    _h_eff_trg_mu50tkmu50_dt_4 = (TH2*)_file_trg_mu->Get("h_eff_trg_mu50tkmu50_dt_4");
    _h_eff_trg_mu50tkmu50_mc_1 = (TH2*)_file_trg_mu->Get("h_eff_trg_mu50tkmu50_mc_1");
    _h_eff_trg_mu50tkmu50_mc_2 = (TH2*)_file_trg_mu->Get("h_eff_trg_mu50tkmu50_mc_2");
    _h_eff_trg_mu50tkmu50_mc_3 = (TH2*)_file_trg_mu->Get("h_eff_trg_mu50tkmu50_mc_3");
    _h_eff_trg_mu50tkmu50_mc_4 = (TH2*)_file_trg_mu->Get("h_eff_trg_mu50tkmu50_mc_4");

  }

}


// add efficiency scale factors
void addEffScale()
{
  // muon 
  if (abs(_llnunu_l1_l1_pdgId)==13 && abs(_llnunu_l1_l2_pdgId)==13) {

    // id iso
    double effdt1,effmc1,errdt1,errmc1,effdt1a,effmc1a,errdt1a,errmc1a;
    double effdt2,effmc2,errdt2,errmc2,effdt2a,effmc2a,errdt2a,errmc2a;
    double isosf1,isosf2,isosferr1,isosferr2;
    
    if (_EffScaleMCVersion=="80xSpring16") {

      //id
      effdt1  = _h_eff_trkhpt_mu_dt->GetBinContent(_h_eff_trkhpt_mu_dt->FindBin(_llnunu_l1_l1_eta, _llnunu_l1_l1_pt));
      effmc1  = _h_eff_trkhpt_mu_mc->GetBinContent(_h_eff_trkhpt_mu_mc->FindBin(_llnunu_l1_l1_eta, _llnunu_l1_l1_pt));
      errdt1  = _h_eff_trkhpt_mu_dt->GetBinError(_h_eff_trkhpt_mu_dt->FindBin(_llnunu_l1_l1_eta, _llnunu_l1_l1_pt));
      errmc1  = _h_eff_trkhpt_mu_mc->GetBinError(_h_eff_trkhpt_mu_mc->FindBin(_llnunu_l1_l1_eta, _llnunu_l1_l1_pt));
      effdt1a = _h_eff_hpt_mu_dt->GetBinContent(_h_eff_hpt_mu_dt->FindBin(_llnunu_l1_l1_eta, _llnunu_l1_l1_pt));
      effmc1a = _h_eff_hpt_mu_mc->GetBinContent(_h_eff_hpt_mu_mc->FindBin(_llnunu_l1_l1_eta, _llnunu_l1_l1_pt));
      errdt1a = _h_eff_hpt_mu_dt->GetBinError(_h_eff_hpt_mu_dt->FindBin(_llnunu_l1_l1_eta, _llnunu_l1_l1_pt));
      errmc1a = _h_eff_hpt_mu_mc->GetBinError(_h_eff_hpt_mu_mc->FindBin(_llnunu_l1_l1_eta, _llnunu_l1_l1_pt));
      effdt2  = _h_eff_trkhpt_mu_dt->GetBinContent(_h_eff_trkhpt_mu_dt->FindBin(_llnunu_l1_l2_eta, _llnunu_l1_l2_pt));
      effmc2  = _h_eff_trkhpt_mu_mc->GetBinContent(_h_eff_trkhpt_mu_mc->FindBin(_llnunu_l1_l2_eta, _llnunu_l1_l2_pt));
      errdt2  = _h_eff_trkhpt_mu_dt->GetBinError(_h_eff_trkhpt_mu_dt->FindBin(_llnunu_l1_l2_eta, _llnunu_l1_l2_pt));
      errmc2  = _h_eff_trkhpt_mu_mc->GetBinError(_h_eff_trkhpt_mu_mc->FindBin(_llnunu_l1_l2_eta, _llnunu_l1_l2_pt));
      effdt2a = _h_eff_hpt_mu_dt->GetBinContent(_h_eff_hpt_mu_dt->FindBin(_llnunu_l1_l2_eta, _llnunu_l1_l2_pt));
      effmc2a = _h_eff_hpt_mu_mc->GetBinContent(_h_eff_hpt_mu_mc->FindBin(_llnunu_l1_l2_eta, _llnunu_l1_l2_pt));
      errdt2a = _h_eff_hpt_mu_dt->GetBinError(_h_eff_hpt_mu_dt->FindBin(_llnunu_l1_l2_eta, _llnunu_l1_l2_pt));
      errmc2a = _h_eff_hpt_mu_mc->GetBinError(_h_eff_hpt_mu_mc->FindBin(_llnunu_l1_l2_eta, _llnunu_l1_l2_pt));

      //iso
      isosf1 = _h_sf_iso_mu->GetBinContent(_h_sf_iso_mu->FindBin(_llnunu_l1_l1_eta,_llnunu_l1_l1_pt));
      isosf2 = _h_sf_iso_mu->GetBinContent(_h_sf_iso_mu->FindBin(_llnunu_l1_l2_eta,_llnunu_l1_l2_pt));
      isosferr1 = _h_sf_iso_mu->GetBinError(_h_sf_iso_mu->FindBin(_llnunu_l1_l1_eta,_llnunu_l1_l1_pt));
      isosferr2 = _h_sf_iso_mu->GetBinError(_h_sf_iso_mu->FindBin(_llnunu_l1_l2_eta,_llnunu_l1_l2_pt));

    }
    else {

      double eta1 = fabs(_llnunu_l1_l1_eta); 
      double pt1 = _llnunu_l1_l1_pt;
      double eta2 = fabs(_llnunu_l1_l2_eta);
      double pt2 = _llnunu_l1_l2_pt;

      double eta1a = fabs(_llnunu_l1_l1_eta);
      double pt1a = _llnunu_l1_l1_pt;
      double eta2a = fabs(_llnunu_l1_l2_eta);
      double pt2a = _llnunu_l1_l2_pt;

      double isoeta1 = fabs(_llnunu_l1_l1_eta);
      double isopt1 = _llnunu_l1_l1_pt;
      double isoeta2 = fabs(_llnunu_l1_l2_eta);
      double isopt2 = _llnunu_l1_l2_pt;

      double rnd = _rand3->Rndm();

      // for 2016 b+c+d+e+f 20.1/fb
      if (rnd<20.1/(20.1+16.3)) {
         
        // id
        if (eta1<_h_eff_trkhpt_mu_dt_1->GetXaxis()->GetXmin()) eta1 =  0.1+_h_eff_trkhpt_mu_dt_1->GetXaxis()->GetXmin();
        if (eta1>_h_eff_trkhpt_mu_dt_1->GetXaxis()->GetXmax()) eta1 = -0.1+_h_eff_trkhpt_mu_dt_1->GetXaxis()->GetXmax();
        if (pt1<_h_eff_trkhpt_mu_dt_1->GetYaxis()->GetXmin()) pt1 =  0.1+_h_eff_trkhpt_mu_dt_1->GetYaxis()->GetXmin();
        if (pt1>_h_eff_trkhpt_mu_dt_1->GetYaxis()->GetXmax()) pt1 = -0.1+_h_eff_trkhpt_mu_dt_1->GetYaxis()->GetXmax();

        if (eta1a<_h_eff_hpt_mu_dt_1->GetXaxis()->GetXmin()) eta1a =  0.1+_h_eff_hpt_mu_dt_1->GetXaxis()->GetXmin();
        if (eta1a>_h_eff_hpt_mu_dt_1->GetXaxis()->GetXmax()) eta1a = -0.1+_h_eff_hpt_mu_dt_1->GetXaxis()->GetXmax();
        if (pt1a<_h_eff_hpt_mu_dt_1->GetYaxis()->GetXmin()) pt1a =  0.1+_h_eff_hpt_mu_dt_1->GetYaxis()->GetXmin();
        if (pt1a>_h_eff_hpt_mu_dt_1->GetYaxis()->GetXmax()) pt1a = -0.1+_h_eff_hpt_mu_dt_1->GetYaxis()->GetXmax();

        if (eta2<_h_eff_trkhpt_mu_dt_1->GetXaxis()->GetXmin()) eta2 =  0.1+_h_eff_trkhpt_mu_dt_1->GetXaxis()->GetXmin();
        if (eta2>_h_eff_trkhpt_mu_dt_1->GetXaxis()->GetXmax()) eta2 = -0.1+_h_eff_trkhpt_mu_dt_1->GetXaxis()->GetXmax();
        if (pt2<_h_eff_trkhpt_mu_dt_1->GetYaxis()->GetXmin()) pt2 =  0.1+_h_eff_trkhpt_mu_dt_1->GetYaxis()->GetXmin();
        if (pt2>_h_eff_trkhpt_mu_dt_1->GetYaxis()->GetXmax()) pt2 = -0.1+_h_eff_trkhpt_mu_dt_1->GetYaxis()->GetXmax();

        if (eta2a<_h_eff_hpt_mu_dt_1->GetXaxis()->GetXmin()) eta2a =  0.1+_h_eff_hpt_mu_dt_1->GetXaxis()->GetXmin();
        if (eta2a>_h_eff_hpt_mu_dt_1->GetXaxis()->GetXmax()) eta2a = -0.1+_h_eff_hpt_mu_dt_1->GetXaxis()->GetXmax();
        if (pt2a<_h_eff_hpt_mu_dt_1->GetYaxis()->GetXmin()) pt2a =  0.1+_h_eff_hpt_mu_dt_1->GetYaxis()->GetXmin();
        if (pt2a>_h_eff_hpt_mu_dt_1->GetYaxis()->GetXmax()) pt2a = -0.1+_h_eff_hpt_mu_dt_1->GetYaxis()->GetXmax();

        effdt1  = _h_eff_trkhpt_mu_dt_1->GetBinContent(_h_eff_trkhpt_mu_dt_1->FindBin(eta1, pt1));
        effmc1  = _h_eff_trkhpt_mu_mc_1->GetBinContent(_h_eff_trkhpt_mu_mc_1->FindBin(eta1, pt1));
        errdt1  = _h_eff_trkhpt_mu_dt_1->GetBinError(_h_eff_trkhpt_mu_dt_1->FindBin(eta1, pt1));
        errmc1  = _h_eff_trkhpt_mu_mc_1->GetBinError(_h_eff_trkhpt_mu_mc_1->FindBin(eta1, pt1));
        effdt1a = _h_eff_hpt_mu_dt_1->GetBinContent(_h_eff_hpt_mu_dt_1->FindBin(eta1a, pt1a));
        effmc1a = _h_eff_hpt_mu_mc_1->GetBinContent(_h_eff_hpt_mu_mc_1->FindBin(eta1a, pt1a));
        errdt1a = _h_eff_hpt_mu_dt_1->GetBinError(_h_eff_hpt_mu_dt_1->FindBin(eta1a, pt1a));
        errmc1a = _h_eff_hpt_mu_mc_1->GetBinError(_h_eff_hpt_mu_mc_1->FindBin(eta1a, pt1a));
        effdt2  = _h_eff_trkhpt_mu_dt_1->GetBinContent(_h_eff_trkhpt_mu_dt_1->FindBin(eta2, pt2));
        effmc2  = _h_eff_trkhpt_mu_mc_1->GetBinContent(_h_eff_trkhpt_mu_mc_1->FindBin(eta2, pt2));
        errdt2  = _h_eff_trkhpt_mu_dt_1->GetBinError(_h_eff_trkhpt_mu_dt_1->FindBin(eta2, pt2));
        errmc2  = _h_eff_trkhpt_mu_mc_1->GetBinError(_h_eff_trkhpt_mu_mc_1->FindBin(eta2, pt2));
        effdt2a = _h_eff_hpt_mu_dt_1->GetBinContent(_h_eff_hpt_mu_dt_1->FindBin(eta2a, pt2a));
        effmc2a = _h_eff_hpt_mu_mc_1->GetBinContent(_h_eff_hpt_mu_mc_1->FindBin(eta2a, pt2a));
        errdt2a = _h_eff_hpt_mu_dt_1->GetBinError(_h_eff_hpt_mu_dt_1->FindBin(eta2a, pt2a));
        errmc2a = _h_eff_hpt_mu_mc_1->GetBinError(_h_eff_hpt_mu_mc_1->FindBin(eta2a, pt2a));

        // iso
        if (isoeta1<_h_sf_iso_mu_1->GetXaxis()->GetXmin()) isoeta1 =  0.1+_h_sf_iso_mu_1->GetXaxis()->GetXmin();
        if (isoeta1>_h_sf_iso_mu_1->GetXaxis()->GetXmax()) isoeta1 = -0.1+_h_sf_iso_mu_1->GetXaxis()->GetXmax();
        if (isopt1<_h_sf_iso_mu_1->GetYaxis()->GetXmin()) isopt1 =  0.1+_h_sf_iso_mu_1->GetYaxis()->GetXmin();
        if (isopt1>_h_sf_iso_mu_1->GetYaxis()->GetXmax()) isopt1 = -0.1+_h_sf_iso_mu_1->GetYaxis()->GetXmax();
        if (isoeta2<_h_sf_iso_mu_1->GetXaxis()->GetXmin()) isoeta2 =  0.1+_h_sf_iso_mu_1->GetXaxis()->GetXmin();
        if (isoeta2>_h_sf_iso_mu_1->GetXaxis()->GetXmax()) isoeta2 = -0.1+_h_sf_iso_mu_1->GetXaxis()->GetXmax();
        if (isopt2<_h_sf_iso_mu_1->GetYaxis()->GetXmin()) isopt2 =  0.1+_h_sf_iso_mu_1->GetYaxis()->GetXmin();
        if (isopt2>_h_sf_iso_mu_1->GetYaxis()->GetXmax()) isopt2 = -0.1+_h_sf_iso_mu_1->GetYaxis()->GetXmax();

        isosf1 = _h_sf_iso_mu_1->GetBinContent(_h_sf_iso_mu_1->FindBin(isoeta1,isopt1));
        isosf2 = _h_sf_iso_mu_1->GetBinContent(_h_sf_iso_mu_1->FindBin(isoeta2,isopt2));
        isosferr1 = _h_sf_iso_mu_1->GetBinError(_h_sf_iso_mu_1->FindBin(isoeta1,isopt1));
        isosferr2 = _h_sf_iso_mu_1->GetBinError(_h_sf_iso_mu_1->FindBin(isoeta2,isopt2)); 
  
      }
      // for 2016 g+h 16.3/fb
      else {

        // id
        if (eta1<_h_eff_trkhpt_mu_dt_2->GetXaxis()->GetXmin()) eta1 =  0.1+_h_eff_trkhpt_mu_dt_2->GetXaxis()->GetXmin();
        if (eta1>_h_eff_trkhpt_mu_dt_2->GetXaxis()->GetXmax()) eta1 = -0.1+_h_eff_trkhpt_mu_dt_2->GetXaxis()->GetXmax();
        if (pt1<_h_eff_trkhpt_mu_dt_2->GetYaxis()->GetXmin()) pt1 =  0.1+_h_eff_trkhpt_mu_dt_2->GetYaxis()->GetXmin();
        if (pt1>_h_eff_trkhpt_mu_dt_2->GetYaxis()->GetXmax()) pt1 = -0.1+_h_eff_trkhpt_mu_dt_2->GetYaxis()->GetXmax();

        if (eta1a<_h_eff_hpt_mu_dt_2->GetXaxis()->GetXmin()) eta1a =  0.1+_h_eff_hpt_mu_dt_2->GetXaxis()->GetXmin();
        if (eta1a>_h_eff_hpt_mu_dt_2->GetXaxis()->GetXmax()) eta1a = -0.1+_h_eff_hpt_mu_dt_2->GetXaxis()->GetXmax();
        if (pt1a<_h_eff_hpt_mu_dt_2->GetYaxis()->GetXmin()) pt1a =  0.1+_h_eff_hpt_mu_dt_2->GetYaxis()->GetXmin();
        if (pt1a>_h_eff_hpt_mu_dt_2->GetYaxis()->GetXmax()) pt1a = -0.1+_h_eff_hpt_mu_dt_2->GetYaxis()->GetXmax();

        if (eta2<_h_eff_trkhpt_mu_dt_2->GetXaxis()->GetXmin()) eta2 =  0.1+_h_eff_trkhpt_mu_dt_2->GetXaxis()->GetXmin();
        if (eta2>_h_eff_trkhpt_mu_dt_2->GetXaxis()->GetXmax()) eta2 = -0.1+_h_eff_trkhpt_mu_dt_2->GetXaxis()->GetXmax();
        if (pt2<_h_eff_trkhpt_mu_dt_2->GetYaxis()->GetXmin()) pt2 =  0.1+_h_eff_trkhpt_mu_dt_2->GetYaxis()->GetXmin();
        if (pt2>_h_eff_trkhpt_mu_dt_2->GetYaxis()->GetXmax()) pt2 = -0.1+_h_eff_trkhpt_mu_dt_2->GetYaxis()->GetXmax();

        if (eta2a<_h_eff_hpt_mu_dt_2->GetXaxis()->GetXmin()) eta2a =  0.1+_h_eff_hpt_mu_dt_2->GetXaxis()->GetXmin();
        if (eta2a>_h_eff_hpt_mu_dt_2->GetXaxis()->GetXmax()) eta2a = -0.1+_h_eff_hpt_mu_dt_2->GetXaxis()->GetXmax();
        if (pt2a<_h_eff_hpt_mu_dt_2->GetYaxis()->GetXmin()) pt2a =  0.1+_h_eff_hpt_mu_dt_2->GetYaxis()->GetXmin();
        if (pt2a>_h_eff_hpt_mu_dt_2->GetYaxis()->GetXmax()) pt2a = -0.1+_h_eff_hpt_mu_dt_2->GetYaxis()->GetXmax();

        effdt1  = _h_eff_trkhpt_mu_dt_2->GetBinContent(_h_eff_trkhpt_mu_dt_2->FindBin(eta1, pt1));
        effmc1  = _h_eff_trkhpt_mu_mc_2->GetBinContent(_h_eff_trkhpt_mu_mc_2->FindBin(eta1, pt1));
        errdt1  = _h_eff_trkhpt_mu_dt_2->GetBinError(_h_eff_trkhpt_mu_dt_2->FindBin(eta1, pt1));
        errmc1  = _h_eff_trkhpt_mu_mc_2->GetBinError(_h_eff_trkhpt_mu_mc_2->FindBin(eta1, pt1));
        effdt1a = _h_eff_hpt_mu_dt_2->GetBinContent(_h_eff_hpt_mu_dt_2->FindBin(eta1a, pt1a));
        effmc1a = _h_eff_hpt_mu_mc_2->GetBinContent(_h_eff_hpt_mu_mc_2->FindBin(eta1a, pt1a));
        errdt1a = _h_eff_hpt_mu_dt_2->GetBinError(_h_eff_hpt_mu_dt_2->FindBin(eta1a, pt1a));
        errmc1a = _h_eff_hpt_mu_mc_2->GetBinError(_h_eff_hpt_mu_mc_2->FindBin(eta1a, pt1a));
        effdt2  = _h_eff_trkhpt_mu_dt_2->GetBinContent(_h_eff_trkhpt_mu_dt_2->FindBin(eta2, pt2));
        effmc2  = _h_eff_trkhpt_mu_mc_2->GetBinContent(_h_eff_trkhpt_mu_mc_2->FindBin(eta2, pt2));
        errdt2  = _h_eff_trkhpt_mu_dt_2->GetBinError(_h_eff_trkhpt_mu_dt_2->FindBin(eta2, pt2));
        errmc2  = _h_eff_trkhpt_mu_mc_2->GetBinError(_h_eff_trkhpt_mu_mc_2->FindBin(eta2, pt2));
        effdt2a = _h_eff_hpt_mu_dt_2->GetBinContent(_h_eff_hpt_mu_dt_2->FindBin(eta2a, pt2a));
        effmc2a = _h_eff_hpt_mu_mc_2->GetBinContent(_h_eff_hpt_mu_mc_2->FindBin(eta2a, pt2a));
        errdt2a = _h_eff_hpt_mu_dt_2->GetBinError(_h_eff_hpt_mu_dt_2->FindBin(eta2a, pt2a));
        errmc2a = _h_eff_hpt_mu_mc_2->GetBinError(_h_eff_hpt_mu_mc_2->FindBin(eta2a, pt2a));

        // iso
        if (isoeta1<_h_sf_iso_mu_2->GetXaxis()->GetXmin()) isoeta1 =  0.1+_h_sf_iso_mu_2->GetXaxis()->GetXmin();
        if (isoeta1>_h_sf_iso_mu_2->GetXaxis()->GetXmax()) isoeta1 = -0.1+_h_sf_iso_mu_2->GetXaxis()->GetXmax();
        if (isopt1<_h_sf_iso_mu_2->GetYaxis()->GetXmin()) isopt1 =  0.1+_h_sf_iso_mu_2->GetYaxis()->GetXmin();
        if (isopt1>_h_sf_iso_mu_2->GetYaxis()->GetXmax()) isopt1 = -0.1+_h_sf_iso_mu_2->GetYaxis()->GetXmax();
        if (isoeta2<_h_sf_iso_mu_2->GetXaxis()->GetXmin()) isoeta2 =  0.1+_h_sf_iso_mu_2->GetXaxis()->GetXmin();
        if (isoeta2>_h_sf_iso_mu_2->GetXaxis()->GetXmax()) isoeta2 = -0.1+_h_sf_iso_mu_2->GetXaxis()->GetXmax();
        if (isopt2<_h_sf_iso_mu_2->GetYaxis()->GetXmin()) isopt2 =  0.1+_h_sf_iso_mu_2->GetYaxis()->GetXmin();
        if (isopt2>_h_sf_iso_mu_2->GetYaxis()->GetXmax()) isopt2 = -0.1+_h_sf_iso_mu_2->GetYaxis()->GetXmax();

        isosf1 = _h_sf_iso_mu_2->GetBinContent(_h_sf_iso_mu_2->FindBin(isoeta1,isopt1));
        isosf2 = _h_sf_iso_mu_2->GetBinContent(_h_sf_iso_mu_2->FindBin(isoeta2,isopt2));
        isosferr1 = _h_sf_iso_mu_2->GetBinError(_h_sf_iso_mu_2->FindBin(isoeta1,isopt1));
        isosferr2 = _h_sf_iso_mu_2->GetBinError(_h_sf_iso_mu_2->FindBin(isoeta2,isopt2));

      }
    }
    
    // id
    double effdt = effdt1*effdt2a+effdt1a*effdt2-effdt1a*effdt2a;
    double effmc = effmc1*effmc2a+effmc1a*effmc2-effmc1a*effmc2a;
    if(effmc>0){
      _idsf = effdt/effmc;
      _idsf_err = (  TMath::Power((effdt2-effdt2a)*errdt1a,2)
                  + TMath::Power((effdt1-effdt1a)*errdt2a,2)
                  + TMath::Power(effdt1a*errdt2,2)
                  + TMath::Power(effdt2a*errdt1,2) 
                 ) / TMath::Power(effdt,2) 
               + (  TMath::Power((effmc2-effmc2a)*errmc1a,2)
                  + TMath::Power((effmc1-effmc1a)*errmc2a,2)
                  + TMath::Power(effmc1a*errmc2,2)
                  + TMath::Power(effmc2a*errmc1,2)
                 ) / TMath::Power(effmc,2);
  
      _idsf_err = TMath::Power(_idsf_err,.5)*_idsf;
    }
    else {
      _idsf = 1;
      _idsf_err = 1;
    }
    
    // iso
    _isosf = isosf1*isosf2;
    _isosf_err = TMath::Power((TMath::Power(isosf1*isosferr2,2) + TMath::Power(isosferr1*isosf2,2)), .5);


    // tracking
    if (_EffScaleMCVersion=="80xSpring16") {
      effdt1 = _h_sf_trk_mu->GetBinContent(_h_sf_trk_mu->FindBin(_llnunu_l1_l1_eta));
      effdt2 = _h_sf_trk_mu->GetBinContent(_h_sf_trk_mu->FindBin(_llnunu_l1_l2_eta));
      errdt1 = _h_sf_trk_mu->GetBinError(_h_sf_trk_mu->FindBin(_llnunu_l1_l1_eta));
      errdt2 = _h_sf_trk_mu->GetBinError(_h_sf_trk_mu->FindBin(_llnunu_l1_l2_eta));
      _trksf = effdt1*effdt2;
      _trksf_err = TMath::Power((TMath::Power(effdt1*errdt2,2) + TMath::Power(errdt1*effdt2,2)), .5);
    }
    else {
      // since Summer16, tracking eff is included into id eff.
      _trksf = 1.0;
      _trksf_err = 0.0;
    }
    
   // id, iso, tracking combined uncertainty up/down
    double idisotrksf_err = sqrt(_idsf_err*_idsf_err+_isosf_err*_isosf_err+_trksf_err*_trksf_err);
    _idisotrksf = _idsf*_isosf*_trksf;
    _idisotrksf_up = _idisotrksf+0.5*idisotrksf_err;
    _idisotrksf_dn = _idisotrksf-0.5*idisotrksf_err;

    // trigger
    if (_EffScaleMCVersion=="80xSpring16") {
      int trg_bin_l1 = _h_eff_trg_mu_l1_l1p_norm_vs_tot->FindBin(_llnunu_l1_l1_pt,fabs(_llnunu_l1_l1_eta));
      int trg_bin_l2 = _h_eff_trg_mu_l2_l1pl2f_norm_vs_l1p->FindBin(_llnunu_l1_l2_pt,fabs(_llnunu_l1_l2_eta));
      double trg_sc_l1_l1p_vs_tot = _h_eff_trg_mu_l1_l1p_norm_vs_tot->GetBinContent(trg_bin_l1);
      double trg_sc_l2_l1pl2f_vs_l1p = _h_eff_trg_mu_l2_l1pl2f_norm_vs_l1p->GetBinContent(trg_bin_l2);
      double trg_sc_l2_l1pl2p_vs_l1p = _h_eff_trg_mu_l2_l1pl2p_norm_vs_l1p->GetBinContent(trg_bin_l2);
      double trg_sc_l2_l1fl2p_vs_tot = _h_eff_trg_mu_l2_l1fl2p_norm_vs_tot->GetBinContent(trg_bin_l2);
      double trg_sc_l1_l1p_vs_tot_err = _h_eff_trg_mu_l1_l1p_norm_vs_tot->GetBinError(trg_bin_l1);
      double trg_sc_l2_l1pl2f_vs_l1p_err = _h_eff_trg_mu_l2_l1pl2f_norm_vs_l1p->GetBinError(trg_bin_l2);
      double trg_sc_l2_l1pl2p_vs_l1p_err = _h_eff_trg_mu_l2_l1pl2p_norm_vs_l1p->GetBinError(trg_bin_l2);
      double trg_sc_l2_l1fl2p_vs_tot_err = _h_eff_trg_mu_l2_l1fl2p_norm_vs_tot->GetBinError(trg_bin_l2);

      double trg_npass = _N_eff_trg_mu_l1pl2f*trg_sc_l1_l1p_vs_tot*trg_sc_l2_l1pl2f_vs_l1p
                       + _N_eff_trg_mu_l1pl2p*trg_sc_l1_l1p_vs_tot*trg_sc_l2_l1pl2p_vs_l1p
                       + _N_eff_trg_mu_l1fl2p*trg_sc_l2_l1fl2p_vs_tot
                       ;
      double trg_npass_err = pow(_N_eff_trg_mu_l1pl2f_err*trg_sc_l1_l1p_vs_tot*trg_sc_l2_l1pl2f_vs_l1p,2)
                         + pow(_N_eff_trg_mu_l1pl2f*trg_sc_l1_l1p_vs_tot_err*trg_sc_l2_l1pl2f_vs_l1p,2)
                         + pow(_N_eff_trg_mu_l1pl2f*trg_sc_l1_l1p_vs_tot*trg_sc_l2_l1pl2f_vs_l1p_err,2)
                         + pow(_N_eff_trg_mu_l1pl2p_err*trg_sc_l1_l1p_vs_tot*trg_sc_l2_l1pl2p_vs_l1p,2)
                         + pow(_N_eff_trg_mu_l1pl2p*trg_sc_l1_l1p_vs_tot_err*trg_sc_l2_l1pl2p_vs_l1p,2)
                         + pow(_N_eff_trg_mu_l1pl2p*trg_sc_l1_l1p_vs_tot*trg_sc_l2_l1pl2p_vs_l1p_err,2)
                         + pow(_N_eff_trg_mu_l1fl2p_err*trg_sc_l2_l1fl2p_vs_tot,2)
                         + pow(_N_eff_trg_mu_l1fl2p*trg_sc_l2_l1fl2p_vs_tot_err,2)
                         ;
      trg_npass_err = sqrt(trg_npass_err);

      double trg_nfail = _N_eff_trg_mu_tot-trg_npass;
      double trg_nfail_err = sqrt(_N_eff_trg_mu_tot_err*_N_eff_trg_mu_tot_err
                                 - _N_eff_trg_mu_l1pl2f_err*_N_eff_trg_mu_l1pl2f_err
                                 - _N_eff_trg_mu_l1pl2p_err*_N_eff_trg_mu_l1pl2p_err
                                 - _N_eff_trg_mu_l1fl2p_err*_N_eff_trg_mu_l1fl2p_err);

      double trg_eff = trg_npass/(trg_npass+trg_nfail);
      double trg_eff_err = (pow(trg_nfail*trg_npass_err,2)+pow(trg_npass*trg_nfail_err,2))/pow(trg_npass+trg_nfail,4);
      trg_eff_err = sqrt(trg_eff_err);

      double trg_eff_up = trg_eff+0.5*trg_eff_err;
      double trg_eff_dn = trg_eff-0.5*trg_eff_err;

      if (trg_eff>=1) trg_eff=1;
      if (trg_eff<=0) trg_eff=0;
      if (trg_eff_up>=1) trg_eff_up=1;
      if (trg_eff_dn>=1) trg_eff_dn=1;
      if (trg_eff_up<=0) trg_eff_up=0;
      if (trg_eff_dn<=0) trg_eff_dn=0;
      trg_eff_err = fabs(trg_eff_up-trg_eff_dn);

      _trgsf = trg_eff;
      _trgsf_err = trg_eff_err;
      _trgsf_up = trg_eff_up;
      _trgsf_dn = trg_eff_dn;
    }
    else {

      double eta1a = fabs(_llnunu_l1_l1_eta);
      double pt1a = _llnunu_l1_l1_pt;
      double eta2a = fabs(_llnunu_l1_l2_eta);
      double pt2a = _llnunu_l1_l2_pt;

      double eta1b = fabs(_llnunu_l1_l1_eta);
      double pt1b = _llnunu_l1_l1_pt;
      double eta2b = fabs(_llnunu_l1_l2_eta);
      double pt2b = _llnunu_l1_l2_pt;

      double eff1a_dt, err1a_dt, eff1a_mc, err1a_mc;
      double eff2a_dt, err2a_dt, eff2a_mc, err2a_mc;
      double eff1b_dt, err1b_dt, eff1b_mc, err1b_mc;
      double eff2b_dt, err2b_dt, eff2b_mc, err2b_mc;


      double rnd = _rand3->Rndm();

      TH2* h_effa_dt;
      TH2* h_effa_mc;
      TH2* h_effb_dt;
      TH2* h_effb_mc;

      if (rnd<=0.5/(0.5+17.0+3.0+16.0)) {
        // for 2016 period1 0.5/fb, run B, including starkup problems, up to run 274094
        h_effa_dt = (TH2*)_h_eff_trg_mu50_dt_1;
        h_effa_mc = (TH2*)_h_eff_trg_mu50_mc_1;
        h_effb_dt = (TH2*)_h_eff_trg_mu50tkmu50_dt_1;
        h_effb_mc = (TH2*)_h_eff_trg_mu50tkmu50_mc_1;
      }
      else if (rnd>0.5/(0.5+17.0+3.0+16.0)&&rnd<=(0.5+17.0)/(0.5+17.0+3.0+16.0)) {
        // for 2016 period2 17.0/fb, run BCDEF, until L1 EMTF fixed
        h_effa_dt = (TH2*)_h_eff_trg_mu50_dt_2;
        h_effa_mc = (TH2*)_h_eff_trg_mu50_mc_2;
        h_effb_dt = (TH2*)_h_eff_trg_mu50tkmu50_dt_2;
        h_effb_mc = (TH2*)_h_eff_trg_mu50tkmu50_mc_2;
      }
      else if (rnd>(0.5+17.0)/(0.5+17.0+3.0+16.0)&&rnd<=(0.5+17.0+3.0)/(0.5+17.0+3.0+16.0)) {
        // for 2016 period3 3.0/fb, run F post L1 EMFT fix from Run 278167
        h_effa_dt = (TH2*)_h_eff_trg_mu50_dt_3;
        h_effa_mc = (TH2*)_h_eff_trg_mu50_mc_3;
        h_effb_dt = (TH2*)_h_eff_trg_mu50tkmu50_dt_3;
        h_effb_mc = (TH2*)_h_eff_trg_mu50tkmu50_mc_3;
      } 
      else {
        // for 2016 period4 16.0/fb, run GH, post HIP fix
        h_effa_dt = (TH2*)_h_eff_trg_mu50_dt_4;
        h_effa_mc = (TH2*)_h_eff_trg_mu50_mc_4;
        h_effb_dt = (TH2*)_h_eff_trg_mu50tkmu50_dt_4;
        h_effb_mc = (TH2*)_h_eff_trg_mu50tkmu50_mc_4;
      }    

      if (_debug){ 
        std::cout << "Mu Trig Eff: rnd="<< rnd << std::endl;
      }      
  
      // protection
      if (eta1a<h_effa_dt->GetXaxis()->GetXmin()) eta1a =  0.1+h_effa_dt->GetXaxis()->GetXmin();
      if (eta1a>h_effa_dt->GetXaxis()->GetXmax()) eta1a = -0.1+h_effa_dt->GetXaxis()->GetXmax();
      if (pt1a<h_effa_dt->GetYaxis()->GetXmin()) pt1a =  0.1+h_effa_dt->GetYaxis()->GetXmin();
      if (pt1a>h_effa_dt->GetYaxis()->GetXmax()) pt1a = -0.1+h_effa_dt->GetYaxis()->GetXmax();
      if (eta1b<h_effb_dt->GetXaxis()->GetXmin()) eta1b =  0.1+h_effb_dt->GetXaxis()->GetXmin();
      if (eta1b>h_effb_dt->GetXaxis()->GetXmax()) eta1b = -0.1+h_effb_dt->GetXaxis()->GetXmax();
      if (pt1b<h_effb_dt->GetYaxis()->GetXmin()) pt1b =  0.1+h_effb_dt->GetYaxis()->GetXmin();
      if (pt1b>h_effb_dt->GetYaxis()->GetXmax()) pt1b = -0.1+h_effb_dt->GetYaxis()->GetXmax();

      // get eff
      eff1a_dt = h_effa_dt->GetBinContent(h_effa_dt->FindBin(eta1a,pt1a));
      eff1a_mc = h_effa_mc->GetBinContent(h_effa_mc->FindBin(eta1a,pt1a));
      err1a_dt = h_effa_dt->GetBinError(h_effa_dt->FindBin(eta1a,pt1a));
      err1a_mc = h_effa_mc->GetBinError(h_effa_mc->FindBin(eta1a,pt1a));
      eff2a_dt = h_effa_dt->GetBinContent(h_effa_dt->FindBin(eta2a,pt2a));
      eff2a_mc = h_effa_mc->GetBinContent(h_effa_mc->FindBin(eta2a,pt2a));
      err2a_dt = h_effa_dt->GetBinError(h_effa_dt->FindBin(eta2a,pt2a));
      err2a_mc = h_effa_mc->GetBinError(h_effa_mc->FindBin(eta2a,pt2a));      

      eff1b_dt = h_effb_dt->GetBinContent(h_effb_dt->FindBin(eta1b,pt1b));
      eff1b_mc = h_effb_mc->GetBinContent(h_effb_mc->FindBin(eta1b,pt1b));
      err1b_dt = h_effb_dt->GetBinError(h_effb_dt->FindBin(eta1b,pt1b));
      err1b_mc = h_effb_mc->GetBinError(h_effb_mc->FindBin(eta1b,pt1b));
      eff2b_dt = h_effb_dt->GetBinContent(h_effb_dt->FindBin(eta2b,pt2b));
      eff2b_mc = h_effb_mc->GetBinContent(h_effb_mc->FindBin(eta2b,pt2b));
      err2b_dt = h_effb_dt->GetBinError(h_effb_dt->FindBin(eta2b,pt2b));
      err2b_mc = h_effb_mc->GetBinError(h_effb_mc->FindBin(eta2b,pt2b));

      // get eff/err
      double effdt, effmc, errdt, errmc;
/*
      // mu50 only
      if (_llnunu_l1_l1_trigerob_HLTbit>>3&1) {
        // leading pass
        effdt = eff1a_dt;
        effmc = eff1a_mc;
        errdt = err1a_dt;
        errmc = err1a_mc;
      }
      else if (_llnunu_l1_l2_trigerob_HLTbit>>3&1) {
        // subleading pass
        effdt = eff2a_dt;
        effmc = eff2a_mc;
        errdt = err2a_dt;
        errmc = err2a_mc;
      }
      else {
        // no pass
        effdt = 0;
        effmc = 1;
        errdt = 0;
        effmc = 1;
      }
*/      
      // if use mu50||tkmu50
      if ((_llnunu_l1_l1_trigerob_HLTbit>>3&1)||(_llnunu_l1_l1_trigerob_HLTbit>>4&1)) {
        // leading pass
        effdt = eff1b_dt;
        effmc = eff1b_mc;
        errdt = err1b_dt;
        errmc = err1b_mc;
      }
      else if ((_llnunu_l1_l2_trigerob_HLTbit>>3&1)||(_llnunu_l1_l2_trigerob_HLTbit>>4&1)) {
        // subleading pass
        effdt = eff2b_dt;
        effmc = eff2b_mc;
        errdt = err2b_dt;
        errmc = err2b_mc;
      }
      else {
        // no pass
        effdt = 0;
        effmc = 1;
        errdt = 0;
        effmc = 1;
      }

      if (_debug) {
        std::cout << "Mu Trig Eff: rnd="<< rnd << ", effdt=" << effdt << ", effmc=" << effmc << ", errdt=" << errdt << ", errmc=" << errmc << std::endl;
        
      }
      if (effmc>0.0) {
        _trgsf = effdt/effmc;
        _trgsf_err = sqrt(effmc*effmc*errdt*errdt+effdt*effdt*errmc*errmc)/(effmc*effmc);
      }
      else {
        _trgsf = 1.0;
        _trgsf_err = 1.0;
      } 

      _trgsf_up = _trgsf+0.5*_trgsf_err;
      _trgsf_dn = _trgsf-0.5*_trgsf_err;


    }

  }
  // electron
  else if (abs(_llnunu_l1_l1_pdgId)==11 && abs(_llnunu_l1_l2_pdgId)==11) {

    // id
    double el_sf_ptmax=_h_sf_idiso_el->GetYaxis()->GetXmax();
    double effdt1 = 1.0;
    if(_llnunu_l1_l1_pt<=el_sf_ptmax) {
      effdt1 = _h_sf_idiso_el->GetBinContent(_h_sf_idiso_el->FindBin(_llnunu_l1_l1_eSCeta,_llnunu_l1_l1_pt));
    }
    else {
      effdt1 = _h_sf_idiso_el->GetBinContent(_h_sf_idiso_el->FindBin(_llnunu_l1_l1_eSCeta,el_sf_ptmax-0.1));
    }
    double effdt2 = 1.0;
    if(_llnunu_l1_l2_pt<=200) {
      effdt2 = _h_sf_idiso_el->GetBinContent(_h_sf_idiso_el->FindBin(_llnunu_l1_l2_eSCeta,_llnunu_l1_l2_pt));
    }
    else {
      effdt2 = _h_sf_idiso_el->GetBinContent(_h_sf_idiso_el->FindBin(_llnunu_l1_l2_eSCeta,el_sf_ptmax-0.1));
    }
    double errdt1 = _h_sf_idiso_el->GetBinError(_h_sf_idiso_el->FindBin(_llnunu_l1_l1_eSCeta,_llnunu_l1_l1_pt));
    double errdt2 = _h_sf_idiso_el->GetBinError(_h_sf_idiso_el->FindBin(_llnunu_l1_l2_eSCeta,_llnunu_l1_l2_pt));
    _idsf = effdt1*effdt2;
    _idsf_err = TMath::Power((TMath::Power(effdt1*errdt2,2)+TMath::Power(errdt1*effdt2,2)),.5);

    // iso (iso included in id sf)
    _isosf=1;
    _isosf_err=0;

    // track
    effdt1 = _h_sf_trk_el->GetBinContent(_h_sf_trk_el->FindBin(_llnunu_l1_l1_eSCeta,100));
    effdt2 = _h_sf_trk_el->GetBinContent(_h_sf_trk_el->FindBin(_llnunu_l1_l2_eSCeta,100));
    errdt1 = _h_sf_trk_el->GetBinError(_h_sf_trk_el->FindBin(_llnunu_l1_l1_eSCeta,100));
    errdt2 = _h_sf_trk_el->GetBinError(_h_sf_trk_el->FindBin(_llnunu_l1_l2_eSCeta,100));
    _trksf = effdt1*effdt2;
    _trksf_err = TMath::Power((TMath::Power(effdt1*errdt2,2)+TMath::Power(errdt1*effdt2,2)),.5);


    // id, iso, tracking combined uncertainty up/down
    double idisotrksf_err = sqrt(_idsf_err*_idsf_err+_isosf_err*_isosf_err+_trksf_err*_trksf_err);
    _idisotrksf = _idsf*_isosf*_trksf;
    _idisotrksf_up = _idisotrksf+0.5*idisotrksf_err;
    _idisotrksf_dn = _idisotrksf-0.5*idisotrksf_err;

    // trigger
    if (_EffScaleMCVersion=="80xSpring16") {
      _trgsf = _h_sf_trg_el_l1->GetBinContent(_h_sf_trg_el_l1->FindBin(_llnunu_l1_l1_pt,fabs(_llnunu_l1_l1_eta)))/100;
      _trgsf_err = _h_sf_trg_el_l1->GetBinError(_h_sf_trg_el_l1->FindBin(_llnunu_l1_l1_pt,fabs(_llnunu_l1_l1_eta)))/100;
    }
    else {
      double el_eta=fabs(_llnunu_l1_l1_eta);
      double el_pt = _llnunu_l1_l1_pt;
      if (el_eta>_h_sf_trg_el_l1->GetXaxis()->GetXmax()) el_eta = -0.1 + _h_sf_trg_el_l1->GetXaxis()->GetXmax();
      if (el_pt>_h_sf_trg_el_l1->GetYaxis()->GetXmax()) el_pt = -0.1 + _h_sf_trg_el_l1->GetYaxis()->GetXmax();
      if (el_pt<_h_sf_trg_el_l1->GetYaxis()->GetXmin()) el_pt = +0.1 + _h_sf_trg_el_l1->GetYaxis()->GetXmin();
      _trgsf = _h_sf_trg_el_l1->GetBinContent(_h_sf_trg_el_l1->FindBin(el_eta, el_pt));
      _trgsf_err = _h_sf_trg_el_l1->GetBinError(_h_sf_trg_el_l1->FindBin(el_eta, el_pt));
    }

    _trgsf_up = _trgsf+0.5*_trgsf_err;
    _trgsf_dn = _trgsf-0.5*_trgsf_err;
    if (_trgsf>=1) _trgsf = 1;
    if (_trgsf<=0) _trgsf = 0;
    if (_trgsf_up>=1) _trgsf_up=1;
    if (_trgsf_up<=0) _trgsf_up=0;
    if (_trgsf_dn>=1) _trgsf_dn=1;
    if (_trgsf_dn<=0) _trgsf_dn=0;

    _trgsf_err = fabs(_trgsf_up-_trgsf_dn);



  }

}


// prepare eff sf for emu
void prepareEmuTrgsf()
{
  _tree_out->Branch("etrgsf", &_etrgsf, "etrgsf/F");
  _tree_out->Branch("etrgsf_err", &_etrgsf_err, "etrgsf_err/F");
  _tree_out->Branch("etrgsf_up", &_etrgsf_up, "etrgsf_up/F");
  _tree_out->Branch("etrgsf_dn", &_etrgsf_dn, "etrgsf_dn/F");
  _tree_out->Branch("mtrgsf", &_mtrgsf, "mtrgsf/F");
  _tree_out->Branch("mtrgsf_err", &_mtrgsf_err, "mtrgsf_err/F");
  _tree_out->Branch("mtrgsf_up", &_mtrgsf_up, "mtrgsf_up/F");
  _tree_out->Branch("mtrgsf_dn", &_mtrgsf_dn, "mtrgsf_dn/F");
  _file_trg_el = TFile::Open(_EffScaleInputFileName_Trg_El.c_str());
  _h_sf_trg_el_l1=(TH2D*)_file_trg_el->Get("ell1pteta");
  _file_trg_mu = TFile::Open(_EffScaleInputFileName_Trg_Mu.c_str());
  _h_eff_trg_mu_l1_tot = (TH2D*)_file_trg_mu->Get("htrg_l1_tot");
  _h_eff_trg_mu_l2_tot = (TH2D*)_file_trg_mu->Get("htrg_l2_tot");
  _h_eff_trg_mu_l1_l1p = (TH2D*)_file_trg_mu->Get("htrg_l1_l1p");
  _h_eff_trg_mu_l2_l1p = (TH2D*)_file_trg_mu->Get("htrg_l2_l1p");
  _h_eff_trg_mu_l1_l1f = (TH2D*)_file_trg_mu->Get("htrg_l1_l1f");
  _h_eff_trg_mu_l2_l1f = (TH2D*)_file_trg_mu->Get("htrg_l2_l1f");
  _h_eff_trg_mu_l1_l1pl2f = (TH2D*)_file_trg_mu->Get("htrg_l1_l1pl2f");
  _h_eff_trg_mu_l1_l1pl2p = (TH2D*)_file_trg_mu->Get("htrg_l1_l1pl2p");
  _h_eff_trg_mu_l1_l1fl2p = (TH2D*)_file_trg_mu->Get("htrg_l1_l1fl2p");
  _h_eff_trg_mu_l2_l1pl2f = (TH2D*)_file_trg_mu->Get("htrg_l2_l1pl2f");
  _h_eff_trg_mu_l2_l1pl2p = (TH2D*)_file_trg_mu->Get("htrg_l2_l1pl2p");
  _h_eff_trg_mu_l2_l1fl2p = (TH2D*)_file_trg_mu->Get("htrg_l2_l1fl2p");

  _NPtBins_eff_trg_mu = _h_eff_trg_mu_l2_tot->GetNbinsX();
  _NEtaBins_eff_trg_mu = _h_eff_trg_mu_l2_tot->GetNbinsY();
  _N_eff_trg_mu_tot = _h_eff_trg_mu_l2_tot->IntegralAndError(Int_t(1), _NPtBins_eff_trg_mu, Int_t(1), _NEtaBins_eff_trg_mu, _N_eff_trg_mu_tot_err);
  _N_eff_trg_mu_l1p = _h_eff_trg_mu_l2_l1p->IntegralAndError(Int_t(1), _NPtBins_eff_trg_mu, Int_t(1), _NEtaBins_eff_trg_mu, _N_eff_trg_mu_l1p_err);
  _N_eff_trg_mu_l1f = _h_eff_trg_mu_l2_l1f->IntegralAndError(Int_t(1), _NPtBins_eff_trg_mu, Int_t(1), _NEtaBins_eff_trg_mu, _N_eff_trg_mu_l1f_err);
  _N_eff_trg_mu_l1pl2f = _h_eff_trg_mu_l2_l1pl2f->IntegralAndError(Int_t(1), _NPtBins_eff_trg_mu, Int_t(1), _NEtaBins_eff_trg_mu, _N_eff_trg_mu_l1pl2f_err);
  _N_eff_trg_mu_l1pl2p = _h_eff_trg_mu_l2_l1pl2p->IntegralAndError(Int_t(1), _NPtBins_eff_trg_mu, Int_t(1), _NEtaBins_eff_trg_mu, _N_eff_trg_mu_l1pl2p_err);
  _N_eff_trg_mu_l1fl2p = _h_eff_trg_mu_l2_l1fl2p->IntegralAndError(Int_t(1), _NPtBins_eff_trg_mu, Int_t(1), _NEtaBins_eff_trg_mu, _N_eff_trg_mu_l1fl2p_err);

  _h_eff_trg_mu_l1_tot_norm = (TH2D*)_h_eff_trg_mu_l1_tot->Clone("htrg_l1_tot_norm");
  _h_eff_trg_mu_l2_tot_norm = (TH2D*)_h_eff_trg_mu_l2_tot->Clone("htrg_l2_tot_norm");
  _h_eff_trg_mu_l1_l1p_norm = (TH2D*)_h_eff_trg_mu_l1_l1p->Clone("htrg_l1_l1p_norm");
  _h_eff_trg_mu_l1_l1f_norm = (TH2D*)_h_eff_trg_mu_l1_l1f->Clone("htrg_l1_l1f_norm");
  _h_eff_trg_mu_l2_l1p_norm = (TH2D*)_h_eff_trg_mu_l2_l1p->Clone("htrg_l2_l1p_norm");
  _h_eff_trg_mu_l2_l1f_norm = (TH2D*)_h_eff_trg_mu_l2_l1f->Clone("htrg_l2_l1f_norm");
  _h_eff_trg_mu_l1_l1pl2f_norm = (TH2D*)_h_eff_trg_mu_l1_l1pl2f->Clone("htrg_l1_l1pl2f_norm");
  _h_eff_trg_mu_l1_l1pl2p_norm = (TH2D*)_h_eff_trg_mu_l1_l1pl2p->Clone("htrg_l1_l1pl2p_norm");
  _h_eff_trg_mu_l1_l1fl2p_norm = (TH2D*)_h_eff_trg_mu_l1_l1fl2p->Clone("htrg_l1_l1fl2p_norm");
  _h_eff_trg_mu_l2_l1pl2f_norm = (TH2D*)_h_eff_trg_mu_l2_l1pl2f->Clone("htrg_l2_l1pl2f_norm");
  _h_eff_trg_mu_l2_l1pl2p_norm = (TH2D*)_h_eff_trg_mu_l2_l1pl2p->Clone("htrg_l2_l1pl2p_norm");
  _h_eff_trg_mu_l2_l1fl2p_norm = (TH2D*)_h_eff_trg_mu_l2_l1fl2p->Clone("htrg_l2_l1fl2p_norm");

 
  _h_eff_trg_mu_l1_tot_norm->Scale(1./_N_eff_trg_mu_tot);
  _h_eff_trg_mu_l2_tot_norm->Scale(1./_N_eff_trg_mu_tot);
  _h_eff_trg_mu_l1_l1p_norm->Scale(1./_N_eff_trg_mu_l1p);
  _h_eff_trg_mu_l1_l1f_norm->Scale(1./_N_eff_trg_mu_l1f);
  _h_eff_trg_mu_l2_l1p_norm->Scale(1./_N_eff_trg_mu_l1p);
  _h_eff_trg_mu_l2_l1f_norm->Scale(1./_N_eff_trg_mu_l1f);
  _h_eff_trg_mu_l1_l1pl2f_norm->Scale(1./_N_eff_trg_mu_l1pl2f);
  _h_eff_trg_mu_l1_l1pl2p_norm->Scale(1./_N_eff_trg_mu_l1pl2p);
  _h_eff_trg_mu_l1_l1fl2p_norm->Scale(1./_N_eff_trg_mu_l1fl2p);
  _h_eff_trg_mu_l2_l1pl2f_norm->Scale(1./_N_eff_trg_mu_l1pl2f);
  _h_eff_trg_mu_l2_l1pl2p_norm->Scale(1./_N_eff_trg_mu_l1pl2p);
  _h_eff_trg_mu_l2_l1fl2p_norm->Scale(1./_N_eff_trg_mu_l1fl2p);

  _h_eff_trg_mu_l1_l1p_norm_vs_tot    = (TH2D*)_h_eff_trg_mu_l1_l1p_norm->Clone("htrg_l1_l1p_norm_vs_tot");
  _h_eff_trg_mu_l1_l1f_norm_vs_tot    = (TH2D*)_h_eff_trg_mu_l1_l1f_norm->Clone("htrg_l1_l1f_norm_vs_tot");
  _h_eff_trg_mu_l2_l1p_norm_vs_tot    = (TH2D*)_h_eff_trg_mu_l2_l1p_norm->Clone("htrg_l2_l1p_norm_vs_tot");
  _h_eff_trg_mu_l2_l1f_norm_vs_tot    = (TH2D*)_h_eff_trg_mu_l2_l1f_norm->Clone("htrg_l2_l1f_norm_vs_tot");
  _h_eff_trg_mu_l1_l1pl2f_norm_vs_tot = (TH2D*)_h_eff_trg_mu_l1_l1pl2f_norm->Clone("htrg_l1_l1pl2f_norm_vs_tot");
  _h_eff_trg_mu_l1_l1pl2p_norm_vs_tot = (TH2D*)_h_eff_trg_mu_l1_l1pl2p_norm->Clone("htrg_l1_l1pl2p_norm_vs_tot");
  _h_eff_trg_mu_l1_l1fl2p_norm_vs_tot = (TH2D*)_h_eff_trg_mu_l1_l1fl2p_norm->Clone("htrg_l1_l1fl2p_norm_vs_tot");
  _h_eff_trg_mu_l2_l1pl2f_norm_vs_tot = (TH2D*)_h_eff_trg_mu_l2_l1pl2f_norm->Clone("htrg_l2_l1pl2f_norm_vs_tot");
  _h_eff_trg_mu_l2_l1pl2p_norm_vs_tot = (TH2D*)_h_eff_trg_mu_l2_l1pl2p_norm->Clone("htrg_l2_l1pl2p_norm_vs_tot");
  _h_eff_trg_mu_l2_l1fl2p_norm_vs_tot = (TH2D*)_h_eff_trg_mu_l2_l1fl2p_norm->Clone("htrg_l2_l1fl2p_norm_vs_tot");
  _h_eff_trg_mu_l1_l1pl2f_norm_vs_l1p = (TH2D*)_h_eff_trg_mu_l1_l1pl2f_norm->Clone("htrg_l1_l1pl2f_norm_vs_l1p");
  _h_eff_trg_mu_l1_l1pl2p_norm_vs_l1p = (TH2D*)_h_eff_trg_mu_l1_l1pl2p_norm->Clone("htrg_l1_l1pl2p_norm_vs_l1p");
  _h_eff_trg_mu_l1_l1fl2p_norm_vs_l1f = (TH2D*)_h_eff_trg_mu_l1_l1fl2p_norm->Clone("htrg_l1_l1fl2p_norm_vs_l1f");
  _h_eff_trg_mu_l2_l1pl2f_norm_vs_l1p = (TH2D*)_h_eff_trg_mu_l2_l1pl2f_norm->Clone("htrg_l2_l1pl2f_norm_vs_l1p");
  _h_eff_trg_mu_l2_l1pl2p_norm_vs_l1p = (TH2D*)_h_eff_trg_mu_l2_l1pl2p_norm->Clone("htrg_l2_l1pl2p_norm_vs_l1p");
  _h_eff_trg_mu_l2_l1fl2p_norm_vs_l1f = (TH2D*)_h_eff_trg_mu_l2_l1fl2p_norm->Clone("htrg_l2_l1fl2p_norm_vs_l1f");

  _h_eff_trg_mu_l1_l1p_norm_vs_tot->Divide(_h_eff_trg_mu_l1_tot_norm);
  _h_eff_trg_mu_l1_l1f_norm_vs_tot->Divide(_h_eff_trg_mu_l1_tot_norm);
  _h_eff_trg_mu_l2_l1p_norm_vs_tot->Divide(_h_eff_trg_mu_l2_tot_norm);
  _h_eff_trg_mu_l2_l1f_norm_vs_tot->Divide(_h_eff_trg_mu_l2_tot_norm);
  _h_eff_trg_mu_l1_l1pl2f_norm_vs_tot->Divide(_h_eff_trg_mu_l1_tot_norm);
  _h_eff_trg_mu_l1_l1pl2p_norm_vs_tot->Divide(_h_eff_trg_mu_l1_tot_norm);
  _h_eff_trg_mu_l1_l1fl2p_norm_vs_tot->Divide(_h_eff_trg_mu_l1_tot_norm);
  _h_eff_trg_mu_l2_l1pl2f_norm_vs_tot->Divide(_h_eff_trg_mu_l2_tot_norm);
  _h_eff_trg_mu_l2_l1pl2p_norm_vs_tot->Divide(_h_eff_trg_mu_l2_tot_norm);
  _h_eff_trg_mu_l2_l1fl2p_norm_vs_tot->Divide(_h_eff_trg_mu_l2_tot_norm);
  _h_eff_trg_mu_l1_l1pl2f_norm_vs_l1p->Divide(_h_eff_trg_mu_l1_l1p_norm);
  _h_eff_trg_mu_l1_l1pl2p_norm_vs_l1p->Divide(_h_eff_trg_mu_l1_l1p_norm);
  _h_eff_trg_mu_l1_l1fl2p_norm_vs_l1f->Divide(_h_eff_trg_mu_l1_l1f_norm);
  _h_eff_trg_mu_l2_l1pl2f_norm_vs_l1p->Divide(_h_eff_trg_mu_l2_l1p_norm);
  _h_eff_trg_mu_l2_l1pl2p_norm_vs_l1p->Divide(_h_eff_trg_mu_l2_l1p_norm);
  _h_eff_trg_mu_l2_l1fl2p_norm_vs_l1f->Divide(_h_eff_trg_mu_l2_l1f_norm);
}

// add efficiency scale factors for emu
void addEmuTrgsf()
{
      int trg_bin_l1 = _h_eff_trg_mu_l1_l1p_norm_vs_tot->FindBin(_llnunu_l1_l1_pt,fabs(_llnunu_l1_l1_eta));
    int trg_bin_l2 = _h_eff_trg_mu_l2_l1pl2f_norm_vs_l1p->FindBin(_llnunu_l1_l2_pt,fabs(_llnunu_l1_l2_eta));
    double trg_sc_l1_l1p_vs_tot = _h_eff_trg_mu_l1_l1p_norm_vs_tot->GetBinContent(trg_bin_l1);
    double trg_sc_l2_l1pl2f_vs_l1p = _h_eff_trg_mu_l2_l1pl2f_norm_vs_l1p->GetBinContent(trg_bin_l2);
    double trg_sc_l2_l1pl2p_vs_l1p = _h_eff_trg_mu_l2_l1pl2p_norm_vs_l1p->GetBinContent(trg_bin_l2);
    double trg_sc_l2_l1fl2p_vs_tot = _h_eff_trg_mu_l2_l1fl2p_norm_vs_tot->GetBinContent(trg_bin_l2);
    double trg_sc_l1_l1p_vs_tot_err = _h_eff_trg_mu_l1_l1p_norm_vs_tot->GetBinError(trg_bin_l1);
    double trg_sc_l2_l1pl2f_vs_l1p_err = _h_eff_trg_mu_l2_l1pl2f_norm_vs_l1p->GetBinError(trg_bin_l2);
    double trg_sc_l2_l1pl2p_vs_l1p_err = _h_eff_trg_mu_l2_l1pl2p_norm_vs_l1p->GetBinError(trg_bin_l2);
    double trg_sc_l2_l1fl2p_vs_tot_err = _h_eff_trg_mu_l2_l1fl2p_norm_vs_tot->GetBinError(trg_bin_l2);

    double trg_npass = _N_eff_trg_mu_l1pl2f*trg_sc_l1_l1p_vs_tot*trg_sc_l2_l1pl2f_vs_l1p
                     + _N_eff_trg_mu_l1pl2p*trg_sc_l1_l1p_vs_tot*trg_sc_l2_l1pl2p_vs_l1p
                     + _N_eff_trg_mu_l1fl2p*trg_sc_l2_l1fl2p_vs_tot
                     ;
    double trg_npass_err = pow(_N_eff_trg_mu_l1pl2f_err*trg_sc_l1_l1p_vs_tot*trg_sc_l2_l1pl2f_vs_l1p,2)
                         + pow(_N_eff_trg_mu_l1pl2f*trg_sc_l1_l1p_vs_tot_err*trg_sc_l2_l1pl2f_vs_l1p,2)
                         + pow(_N_eff_trg_mu_l1pl2f*trg_sc_l1_l1p_vs_tot*trg_sc_l2_l1pl2f_vs_l1p_err,2)
                         + pow(_N_eff_trg_mu_l1pl2p_err*trg_sc_l1_l1p_vs_tot*trg_sc_l2_l1pl2p_vs_l1p,2)
                         + pow(_N_eff_trg_mu_l1pl2p*trg_sc_l1_l1p_vs_tot_err*trg_sc_l2_l1pl2p_vs_l1p,2)
                         + pow(_N_eff_trg_mu_l1pl2p*trg_sc_l1_l1p_vs_tot*trg_sc_l2_l1pl2p_vs_l1p_err,2)
                         + pow(_N_eff_trg_mu_l1fl2p_err*trg_sc_l2_l1fl2p_vs_tot,2)
                         + pow(_N_eff_trg_mu_l1fl2p*trg_sc_l2_l1fl2p_vs_tot_err,2)
                         ;
    trg_npass_err = sqrt(trg_npass_err);

    double trg_nfail = _N_eff_trg_mu_tot-trg_npass;
    double trg_nfail_err = sqrt(_N_eff_trg_mu_tot_err*_N_eff_trg_mu_tot_err
                                 - _N_eff_trg_mu_l1pl2f_err*_N_eff_trg_mu_l1pl2f_err
                                 - _N_eff_trg_mu_l1pl2p_err*_N_eff_trg_mu_l1pl2p_err
                                 - _N_eff_trg_mu_l1fl2p_err*_N_eff_trg_mu_l1fl2p_err);

    double trg_eff = trg_npass/(trg_npass+trg_nfail);
    double trg_eff_err = (pow(trg_nfail*trg_npass_err,2)+pow(trg_npass*trg_nfail_err,2))/pow(trg_npass+trg_nfail,4);
    trg_eff_err = sqrt(trg_eff_err);

    double trg_eff_up = trg_eff+0.5*trg_eff_err;
    double trg_eff_dn = trg_eff-0.5*trg_eff_err;

    if (trg_eff>=1) trg_eff=1;
    if (trg_eff<=0) trg_eff=0;
    if (trg_eff_up>=1) trg_eff_up=1;
    if (trg_eff_dn>=1) trg_eff_dn=1;
    if (trg_eff_up<=0) trg_eff_up=0;
    if (trg_eff_dn<=0) trg_eff_dn=0;
    trg_eff_err = fabs(trg_eff_up-trg_eff_dn);

    _mtrgsf = trg_eff;
    _mtrgsf_err = trg_eff_err;
    _mtrgsf_up = trg_eff_up;
    _mtrgsf_dn = trg_eff_dn;

    _etrgsf = _h_sf_trg_el_l1->GetBinContent(_h_sf_trg_el_l1->FindBin(_llnunu_l1_l1_pt,fabs(_llnunu_l1_l1_eta)))/100;
    _etrgsf_err = _h_sf_trg_el_l1->GetBinError(_h_sf_trg_el_l1->FindBin(_llnunu_l1_l1_pt,fabs(_llnunu_l1_l1_eta)))/100;

    _etrgsf_up = _etrgsf+0.5*_etrgsf_err;
    _etrgsf_dn = _etrgsf-0.5*_etrgsf_err;
    if (_etrgsf>=1) _etrgsf = 1;
    if (_etrgsf<=0) _etrgsf = 0;
    if (_etrgsf_up>=1) _etrgsf_up=1;
    if (_etrgsf_up<=0) _etrgsf_up=0;
    if (_etrgsf_dn>=1) _etrgsf_dn=1;
    if (_etrgsf_dn<=0) _etrgsf_dn=0;

    _etrgsf_err = fabs(_etrgsf_up-_etrgsf_dn);


}



// prepare gjets skimming
void prepareGJetsSkim() 
{
  if (_doGJetsSkim) {
    
    // input file
    _gjets_input_file = TFile::Open(_GJetsSkimInputFileName.c_str());

    // for mass generation
    _gjets_h_zmass_zpt = (TH2D*)_gjets_input_file->Get("h_zmass_zpt_lowlpt");
    _gjets_h_zmass_zpt_el = (TH2D*)_gjets_input_file->Get("h_zmass_zpt_lowlpt_el");
    _gjets_h_zmass_zpt_mu = (TH2D*)_gjets_input_file->Get("h_zmass_zpt_lowlpt_mu");

    // for zpt reweighting
    // zpt 1d
    _gjets_h_zpt_ratio = (TH1D*)_gjets_input_file->Get("h_zpt_ratio");
    _gjets_h_zpt_ratio_el = (TH1D*)_gjets_input_file->Get("h_zpt_ratio_el");
    _gjets_h_zpt_ratio_mu = (TH1D*)_gjets_input_file->Get("h_zpt_ratio_mu");
    _gjets_h_zpt_lowlpt_ratio = (TH1D*)_gjets_input_file->Get("h_zpt_lowlpt_ratio");
    _gjets_h_zpt_lowlpt_ratio_el = (TH1D*)_gjets_input_file->Get("h_zpt_lowlpt_ratio_el");
    _gjets_h_zpt_lowlpt_ratio_mu = (TH1D*)_gjets_input_file->Get("h_zpt_lowlpt_ratio_mu");

    _gjets_h_zpt_ratio_up = (TH1D*)_gjets_input_file->Get("h_zpt_ratio_up");
    _gjets_h_zpt_ratio_dn = (TH1D*)_gjets_input_file->Get("h_zpt_ratio_dn");
    _gjets_h_zpt_ratio_el_up = (TH1D*)_gjets_input_file->Get("h_zpt_ratio_el_up");
    _gjets_h_zpt_ratio_el_dn = (TH1D*)_gjets_input_file->Get("h_zpt_ratio_el_dn");
    _gjets_h_zpt_ratio_mu_up = (TH1D*)_gjets_input_file->Get("h_zpt_ratio_mu_up");
    _gjets_h_zpt_ratio_mu_dn = (TH1D*)_gjets_input_file->Get("h_zpt_ratio_mu_dn");

    // tgraph
    _gjets_gr_zpt_ratio = (TGraphErrors*)_gjets_input_file->Get("gr_zpt_ratio");
    _gjets_gr_zpt_ratio_el = (TGraphErrors*)_gjets_input_file->Get("gr_zpt_ratio_el");
    _gjets_gr_zpt_ratio_mu = (TGraphErrors*)_gjets_input_file->Get("gr_zpt_ratio_mu");
    _gjets_gr_zpt_ratio_up = (TGraphErrors*)_gjets_input_file->Get("gr_zpt_ratio_up");
    _gjets_gr_zpt_ratio_dn = (TGraphErrors*)_gjets_input_file->Get("gr_zpt_ratio_dn");
    _gjets_gr_zpt_ratio_el_up = (TGraphErrors*)_gjets_input_file->Get("gr_zpt_ratio_el_up");
    _gjets_gr_zpt_ratio_el_dn = (TGraphErrors*)_gjets_input_file->Get("gr_zpt_ratio_el_dn");
    _gjets_gr_zpt_ratio_mu_up = (TGraphErrors*)_gjets_input_file->Get("gr_zpt_ratio_mu_up");
    _gjets_gr_zpt_ratio_mu_dn = (TGraphErrors*)_gjets_input_file->Get("gr_zpt_ratio_mu_dn");
    _gjets_gr_zpt_lowlpt_ratio = (TGraphErrors*)_gjets_input_file->Get("gr_zpt_lowlpt_ratio");
    _gjets_gr_zpt_lowlpt_ratio_el = (TGraphErrors*)_gjets_input_file->Get("gr_zpt_lowlpt_ratio_el");
    _gjets_gr_zpt_lowlpt_ratio_mu = (TGraphErrors*)_gjets_input_file->Get("gr_zpt_lowlpt_ratio_mu");
 
    // project mass to 1d
    for (int ix=0; ix<(int)_gjets_h_zmass_zpt->GetNbinsX(); ix++) {
      for (int iy=0; iy<(int)_gjets_h_zmass_zpt->GetNbinsY(); iy++) {
        if (_gjets_h_zmass_zpt->GetBinContent(ix+1, iy+1)<0) {
          _gjets_h_zmass_zpt->SetBinContent(ix+1, iy+1, 0.0);
        }
      }
    }

    for (int ix=0; ix<(int)_gjets_h_zmass_zpt_el->GetNbinsX(); ix++) {
      for (int iy=0; iy<(int)_gjets_h_zmass_zpt_el->GetNbinsY(); iy++) {
        if (_gjets_h_zmass_zpt_el->GetBinContent(ix+1, iy+1)<0) {
          _gjets_h_zmass_zpt_el->SetBinContent(ix+1, iy+1, 0.0);
        }
      }
    }

    for (int ix=0; ix<(int)_gjets_h_zmass_zpt_mu->GetNbinsX(); ix++) {
      for (int iy=0; iy<(int)_gjets_h_zmass_zpt_mu->GetNbinsY(); iy++) {
        if (_gjets_h_zmass_zpt_mu->GetBinContent(ix+1, iy+1)<0) {
          _gjets_h_zmass_zpt_mu->SetBinContent(ix+1, iy+1, 0.0);
        }
      }
    }

    for (int iy=0; iy<(int)_gjets_h_zmass_zpt->GetNbinsY(); iy++){
      sprintf(name, "h_zmass_zpt_%i", iy+1);
      TH1D* htmp = (TH1D*)_gjets_h_zmass_zpt->ProjectionX(name, iy+1, iy+1, "e");
      _gjets_h_zmass_zpt_1d_vec.push_back(htmp);
    }

    for (int iy=0; iy<(int)_gjets_h_zmass_zpt_el->GetNbinsY(); iy++){
      sprintf(name, "h_zmass_zpt_el_%i", iy+1);
      TH1D* htmp = (TH1D*)_gjets_h_zmass_zpt_el->ProjectionX(name, iy+1, iy+1, "e");
      _gjets_h_zmass_zpt_el_1d_vec.push_back(htmp);
    }

    for (int iy=0; iy<(int)_gjets_h_zmass_zpt_mu->GetNbinsY(); iy++){
      sprintf(name, "h_zmass_zpt_mu_%i", iy+1);
      TH1D* htmp = (TH1D*)_gjets_h_zmass_zpt_mu->ProjectionX(name, iy+1, iy+1, "e");
      _gjets_h_zmass_zpt_mu_1d_vec.push_back(htmp);
    }


    // photon phi weight
    if (_doGJetsSkimAddPhiWeight) {
      _gjets_phi_weight_input_file = TFile::Open(_GJetsSkimPhiWeightInputFileName.c_str());
      _gjets_h_photon_phi_weight = (TH1D*)_gjets_phi_weight_input_file->Get("h_gjet_phi_weight");
    }   

    // photon trig eff
    if (_doGJetsSkimAddTrigEff) {
      _gjets_trig_eff_input_file = TFile::Open(_GJetsSkimTrigEffInputFileName.c_str());
      _gjets_h_trig_eff_weight = (TH2D*)_gjets_trig_eff_input_file->Get("h_eta_pt_weight");
    }

    // rho weight
    _gjet_rho_weight_input_file = TFile::Open(_GJetsSkimRhoWeightInputFileName.c_str());
    _gjet_h_rho_weight = (TH2D*)_gjet_rho_weight_input_file->Get("h_rho_zpt_weight");

  }

}


// do gjets skim
void doGJetsSkim()
{

  // copy branches
  _llnunu_mt = _gjet_mt;
  _llnunu_l1_pt = _gjet_l1_pt;
  _llnunu_l1_eta = _gjet_l1_eta;
  _llnunu_l1_rapidity = _gjet_l1_rapidity;
  _llnunu_l1_phi = _gjet_l1_phi;
  _llnunu_l1_trigerob_HLTbit = _gjet_l1_trigerob_HLTbit;
  _llnunu_l1_trigerob_pt = _gjet_l1_trigerob_pt;
  _llnunu_l1_trigerob_eta = _gjet_l1_trigerob_eta;
  _llnunu_l1_trigerob_phi = _gjet_l1_trigerob_phi;
  _llnunu_l2_pt = _gjet_l2_pt;
  _llnunu_l2_phi = _gjet_l2_phi;
  _llnunu_l2_sumEt = _gjet_l2_sumEt;
  _llnunu_l2_rawPt = _gjet_l2_rawPt;
  _llnunu_l2_rawPhi = _gjet_l2_rawPhi;
  _llnunu_l2_rawSumEt = _gjet_l2_rawSumEt;
  _llnunu_l1_l1_pt = 19801117;
  _llnunu_l1_l1_eta = 0;
  _llnunu_l1_l1_pdgId = 19801117;
  _llnunu_l1_l2_pt = 19801117;
  _llnunu_l1_l2_eta = 0;
  _llnunu_l1_l2_pdgId = 19801117;
  _llnunu_l1_l1_highPtID = 1.0;
  _llnunu_l1_l2_highPtID = 1.0;

  _llnunu_l2_pt_el = _llnunu_l2_pt;
  _llnunu_l2_phi_el = _llnunu_l2_phi;
  _llnunu_l2_pt_mu = _llnunu_l2_pt;
  _llnunu_l2_phi_mu = _llnunu_l2_phi;

  if (!_isData){
    _llnunu_l2_genPhi = _gjet_l2_genPhi;
    _llnunu_l2_genEta = _gjet_l2_genEta;  
  }


  // get prescale/ only for data
  if (_isData){
    if (_llnunu_l1_trigerob_HLTbit>>0&1&&_llnunu_l1_trigerob_pt<30) {
      _GJetsPreScaleWeight = _PreScale22*1000;
    }
    else 
    if (_llnunu_l1_trigerob_HLTbit>>1&1&&_llnunu_l1_trigerob_pt<36) {
      _GJetsPreScaleWeight = (float)_PreScale30*140;
    }
    else 
    if (_llnunu_l1_trigerob_HLTbit>>2&1&&_llnunu_l1_trigerob_pt<50) {
      _GJetsPreScaleWeight = (float)_PreScale36*140;
    }
    else 
    if (_llnunu_l1_trigerob_HLTbit>>3&1&&_llnunu_l1_trigerob_pt<75) {
      _GJetsPreScaleWeight = (float)_PreScale50;
    }
    else 
    if (_llnunu_l1_trigerob_HLTbit>>4&1&&_llnunu_l1_trigerob_pt<90) {
      _GJetsPreScaleWeight = (float)_PreScale75;
    }
    else 
    if (_llnunu_l1_trigerob_HLTbit>>5&1&&_llnunu_l1_trigerob_pt<120) {
      _GJetsPreScaleWeight = (float)_PreScale90;
    }
    else 
    if (_llnunu_l1_trigerob_HLTbit>>6&1&&_llnunu_l1_trigerob_pt<165) {
      _GJetsPreScaleWeight = (float)_PreScale120;
    }
    else 
    if (_llnunu_l1_trigerob_HLTbit>>7&1) {
      _GJetsPreScaleWeight = (float)_PreScale165;
    }
    else {
      _GJetsPreScaleWeight = 1.0;
    }
  }

  // generate z mass
  // default all, known to be wrong
  int ipt;

  // use 2d
  ipt = _gjets_h_zmass_zpt->GetYaxis()->FindBin(_llnunu_l1_pt) - 1;
  if (ipt<=0) ipt=0;
  if (ipt>=_gjets_h_zmass_zpt->GetNbinsY()) ipt=_gjets_h_zmass_zpt->GetNbinsY()-1;
  if (_debug) std::cout << "doGJetsSkim:: begin all zmass random " << std::endl;
  _llnunu_l1_mass = _gjets_h_zmass_zpt_1d_vec.at(ipt)->GetRandom();
  if (_debug) std::cout << "doGJetsSkim:: end all zmass random " << std::endl;


  // calculate mt
  Float_t et1 = TMath::Sqrt(_llnunu_l1_mass*_llnunu_l1_mass + _llnunu_l1_pt*_llnunu_l1_pt);
  Float_t et2 = TMath::Sqrt(_llnunu_l1_mass*_llnunu_l1_mass + _llnunu_l2_pt*_llnunu_l2_pt);
  _llnunu_mt = TMath::Sqrt(2.0*_llnunu_l1_mass*_llnunu_l1_mass+2.0*(et1*et2
             -_llnunu_l1_pt*cos(_llnunu_l1_phi)*_llnunu_l2_pt*cos(_llnunu_l2_phi)
             -_llnunu_l1_pt*sin(_llnunu_l1_phi)*_llnunu_l2_pt*sin(_llnunu_l2_phi)));

  //  el
  // use 2d
  ipt = _gjets_h_zmass_zpt_el->GetYaxis()->FindBin(_llnunu_l1_pt) - 1;
  if (ipt<=0) ipt=0;
  if (ipt>=_gjets_h_zmass_zpt_el->GetNbinsY()) ipt=_gjets_h_zmass_zpt_el->GetNbinsY()-1;
  if (_debug) std::cout << "doGJetsSkim:: begin el zmass random " << std::endl;
  _llnunu_l1_mass_el = _gjets_h_zmass_zpt_el_1d_vec.at(ipt)->GetRandom();
  if (_debug) std::cout << "doGJetsSkim:: end el zmass random " << std::endl;

  // calculate mt
  et1 = TMath::Sqrt(_llnunu_l1_mass_el*_llnunu_l1_mass_el + _llnunu_l1_pt*_llnunu_l1_pt);
  et2 = TMath::Sqrt(_llnunu_l1_mass_el*_llnunu_l1_mass_el + _llnunu_l2_pt_el*_llnunu_l2_pt_el);
  _llnunu_mt_el = TMath::Sqrt(2.0*_llnunu_l1_mass_el*_llnunu_l1_mass_el+2.0*(et1*et2
                 -_llnunu_l1_pt*cos(_llnunu_l1_phi)*_llnunu_l2_pt_el*cos(_llnunu_l2_phi_el)
                 -_llnunu_l1_pt*sin(_llnunu_l1_phi)*_llnunu_l2_pt_el*sin(_llnunu_l2_phi_el)));

  //  mu
  // use 2d
  ipt = _gjets_h_zmass_zpt_mu->GetYaxis()->FindBin(_llnunu_l1_pt) - 1;
  if (ipt<=0) ipt=0;
  if (ipt>=_gjets_h_zmass_zpt_mu->GetNbinsY()) ipt=_gjets_h_zmass_zpt_mu->GetNbinsY()-1;
  if (_debug) std::cout << "doGJetsSkim:: begin mu zmass random " << std::endl;
  _llnunu_l1_mass_mu = _gjets_h_zmass_zpt_mu_1d_vec.at(ipt)->GetRandom();
  if (_debug) std::cout << "doGJetsSkim:: end mu zmass random " << std::endl;

  // calculate mt
  et1 = TMath::Sqrt(_llnunu_l1_mass_mu*_llnunu_l1_mass_mu + _llnunu_l1_pt*_llnunu_l1_pt);
  et2 = TMath::Sqrt(_llnunu_l1_mass_mu*_llnunu_l1_mass_mu + _llnunu_l2_pt_mu*_llnunu_l2_pt_mu);
  _llnunu_mt_mu = TMath::Sqrt(2.0*_llnunu_l1_mass_mu*_llnunu_l1_mass_mu+2.0*(et1*et2
                 -_llnunu_l1_pt*cos(_llnunu_l1_phi)*_llnunu_l2_pt_mu*cos(_llnunu_l2_phi_mu)
                 -_llnunu_l1_pt*sin(_llnunu_l1_phi)*_llnunu_l2_pt_mu*sin(_llnunu_l2_phi_mu)));


  if (_debug) std::cout << "doGJetsSkim:: start getting photon pt weight " << std::endl;
  // get zpt weight
  ipt = _gjets_h_zpt_ratio->GetXaxis()->FindBin(_llnunu_l1_pt); 

  _GJetsZPtWeight = _gjets_gr_zpt_ratio->Eval(_llnunu_l1_pt);
  _GJetsZPtWeightEl = _gjets_gr_zpt_ratio_el->Eval(_llnunu_l1_pt);
  _GJetsZPtWeightMu = _gjets_gr_zpt_ratio_mu->Eval(_llnunu_l1_pt);
  _GJetsZPtWeightLowLPt = _gjets_gr_zpt_lowlpt_ratio->Eval(_llnunu_l1_pt);
  _GJetsZPtWeightLowLPtEl = _gjets_gr_zpt_lowlpt_ratio_el->Eval(_llnunu_l1_pt);
  _GJetsZPtWeightLowLPtMu = _gjets_gr_zpt_lowlpt_ratio_mu->Eval(_llnunu_l1_pt);

  _GJetsZPtWeight_up = _gjets_gr_zpt_ratio_up->Eval(_llnunu_l1_pt);
  _GJetsZPtWeight_dn = _gjets_gr_zpt_ratio_dn->Eval(_llnunu_l1_pt);
  _GJetsZPtWeightEl_up = _gjets_gr_zpt_ratio_el_up->Eval(_llnunu_l1_pt);
  _GJetsZPtWeightEl_dn = _gjets_gr_zpt_ratio_el_dn->Eval(_llnunu_l1_pt);
  _GJetsZPtWeightMu_up = _gjets_gr_zpt_ratio_mu_up->Eval(_llnunu_l1_pt);
  _GJetsZPtWeightMu_dn = _gjets_gr_zpt_ratio_mu_dn->Eval(_llnunu_l1_pt);

  if (_debug) std::cout << "doGJetsSkim:: end getting photon pt weight " << std::endl;
  // get photon phi weight
  if (_doGJetsSkimAddPhiWeight) {
    _GJetsPhiWeight = _gjets_h_photon_phi_weight->GetBinContent(_gjets_h_photon_phi_weight->FindBin(_llnunu_l1_phi));
  }

  // get photon trig eff
  if (_doGJetsSkimAddTrigEff) {
    _GJetsTrigEff = _gjets_h_trig_eff_weight->GetBinContent(_gjets_h_trig_eff_weight->FindBin(_llnunu_l1_pt,fabs(_llnunu_l1_eta)));
  }

  // gjet rho weight
  _GJetsRhoWeight = _gjet_h_rho_weight->GetBinContent(_gjet_h_rho_weight->FindBin(_llnunu_l1_pt, _rho));


}


void doBoostJetSelection(){

    //std::cout<<"_genZ_pt "<<_genZ_pt[0]<<std::endl;
    //std::cout<<"boostJetAK8Puppi_pt "<<_boostJetAK8Puppi_pt[0]<<std::endl;

    _boostJetAK8_pt = _boostJetAK8Puppi_pt[0];
    _boostJetAK8_eta = _boostJetAK8Puppi_eta[0];
    _boostJetAK8_phi = _boostJetAK8Puppi_phi[0];
    _boostJetAK8_mass = _boostJetAK8Puppi_mass[0];

    _boostJetAK8_tau21 = _boostJetAK8Puppi_tau2[0]/_boostJetAK8Puppi_tau1[0];
    _boostJetAK8_softDropMass = _boostJetAK8Puppi_softDrop_massCorr[0]*_boostJetAK8Puppi_softDrop_massBare[0];
    _boostJetAK8_btagBOOSTED = _boostJetAK8Puppi_btagBOOSTED[0];

    TLorentzVector Vhad;
    Vhad.SetPtEtaPhiM(_boostJetAK8_pt,_boostJetAK8_eta,_boostJetAK8_phi,_boostJetAK8_mass);
    TLorentzVector Vlep;    
    Vlep.SetPtEtaPhiM(_llnunu_l1_pt,_llnunu_l1_eta,_llnunu_l1_phi,_llnunu_l1_mass);

    TLorentzVector Vis = Vhad+Vlep;

    Float_t et1 = TMath::Sqrt(Vis.M()*Vis.M() + Vis.Pt()*Vis.Pt());
    Float_t et2 = TMath::Sqrt(_llnunu_l2_pt*_llnunu_l2_pt);
    _mTfull = TMath::Sqrt(2.0*Vis.M()*Vis.M()+2.0*(et1*et2
                 -Vis.Pt()*cos(Vis.Phi())*_llnunu_l2_pt*cos(_llnunu_l2_phi)
                 -Vis.Pt()*sin(Vis.Phi())*_llnunu_l2_pt*sin(_llnunu_l2_phi)));

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  ,.  .:;::;@;,;;:,::...@;,@,                                                                                                                 
//  ..,,.,,.,,:.,':,:::.`.;::':                                                                                                                 
//   ,``. .`,;'.,;::::'`     +@                                                                                                                 
//  :.```.``..:;:'::.::`,@@@+;,:                                                                                                                
//  ,`. :````.,.:..,`';,,    .@++                                                                                                               
//  ```.,``.`..`.```    .:@@@@#,``                                                                                                              
//    `,```` :..,.;@@@             +                                                                                                            
//   ,,,`...,:,,:    .+@@'                                                                                                                      
//  `,,,`,``.:::;@        `:;',,,,:',                                                                                                           
//  ,,.```.`:::.':@@@@@@@@@;,  `@'::.,                                                                                                          
//  ,`,````.::,,;:,.`@+#@@@@';`` ;@.,,                                                                                                          
//  ,`...`..,,.,;,:, @ .,,@@@@@@++. ::,:                                                  `                                                     
//  ``..``.`;:`,;::. @ `..':@@@@++''+@',`;                                  `     .        `                                                    
//  .``,....;,:,:,;,`@ `.`'.;+@   @@+';':;.,                            `         `.,  ``.,.     :,                                             
//  `...```.;:,`:,:. @ ```+`;.@     @@@@@@',`,.                    ,.,,.,.    `   `,;.':':.```      ``                                          
//  :,,`.` ,,:..:`. `@ ` .'`:.@      @@#+'''+#```.               `.::::;;'.,,;` ..',,::,.```                                                    
//  :,,.,`.`,;, ;````@ ``:'.:,'       :@++';::,..``,             ``: ``;;''';.;++#,.`  ,.`  `.:'..,`` ``                                        
//  ,.,,,``  ,: '` ` ' ,,:;:,:'         @+++';:,,.``:@                  `,;;;';;::` `      .  `,  `.`:,`..                                      
//  .```  `` ```..`,:,,::;::,,;          @+#+''::,.. ``.        `          .:.    ,    `     ` `. ``..`,.``                                     
//  `   `...``` ..` .,,,,,::.,,           @+##+';:,,..```,       ``     ``..,.`     `       ` `  .` `..`,.`                                     
//  ` .``````   `,`.` ;`,.::..,`           ;@#@@@'#@:,,.```.   .@@.`    `..,,:::#@'`.`,    `  ````..``,`.,.                                     
//  ` ``` ,  :@@ .; ,.;`,`,,`,,`             @'@@@+@@@@,,.```. ,; .;@:``....,:;:;'#@;`  .  .. `.`.```..,`..                                     
//  ,    `,, :,:# ',`,;., ,`,:,`              `@###@@'@@':';,,`  :'#@@@@@,`,,:;'+@':;,` +.`.,.`.....``..` ``                                    
//  ,    `` ;`@'.:' ;`:.,  :,,,.                ,@##@@@#@@+@` `,...`,@#`:;:+,::'+'+#:.` :,,.,,.``,.``.`.` `.                                    
//  .  `,  `.``,.,;.`.`,::@+,,,,                   @@@@@#+;;#@,.:':`` '@@@@#';;''@@,`. ,;;:,`:,.```` ``````.`                                   
//   @.`,  :.``` :,,::``@` :.;,;                      @@##':.,@@@@;.`.....`,:;;++@:.;``,:::,;;.        `. `.                                    
//  `..`, ,,```` , .`:.`@``+:`,@                          @@`@. `..,,,:::::;;+++@@:::`.,,`, . ,,.,;,``      `                                   
//    , .`;`.``. .`, ,,,@. ,,`:'                            @`,@:.,::;;'''++++#@@@,;,`. .  .  ``   .:.;.``                                      
//  ```,:,. ```` .```,,,@.,:,.:@                            :@@@:'';;'''+'+++#@@#@,,,`;        `, `  ` ;``..`                                   
//  .``.,..     ```, .`:@,,,:.:@                            @::;;;'''''''''+++#@@@@,  @;`   `.` ..     `,.`````                                 
//  .` `,.` ` ```.```` `@,,':,;+                              @@@@@@@@@+''++@@@@@@@,  @@ .` .:.`.,`.` ``````` ` `                               
//  .```,`` : ,.``.`.`` @`.:.','                                       @@@@@@@@@@@#'  @'` , `.,`...```   ```                                    
//  .```.`` . ,``.`....`@.`:...;                                       @@@@@@@@@@;@  .:,  ,.`,:`....``` `.                                      
//  ``` .`` ` ,``.````` @:`;.,.@                                      ,,  ````..,:,  `, ,``,.,.,.`...``````                                     
//:#,`` . . ` ,     .`.`@:`.`.,@                                   ..`        `.,,:` , `:. ,.,...`.,,. ```                                      
//':, ``, ,```, `` , `` ';`.,.,#                           .``         ``````..,,:::   ;., ,,;:.``..,; ````                                     
//,,, ` ``.   .` ` ,`,`.,+,,,,:,@@@,;;@:::`             ..`    `` ````````.,.:;';'': ` :, . ';,   :.,:.,`````                                   
//,.,`` `.,.```  ``. :`,,@.:'::,::,..:,:.,,,` `  .,,#@..``       .`    ``...,..,;;``',  ,   @,    :.,.....``                                    
//@#:`.``.,```.``````,`,,@,;,,:,::. :: `.: .:+,`....::,..`        ``````...,,::;;; @@,,`    ;      ',..``,. `                                   
//@@@````,, .``.`.`.`,`,,@::.,,,::..``'@, ',,@,``,:;;::,.`         `..`..,,::::;: @'+`, ,   .     .  .,,`,                                      
//+++``.`.,``,`..`..`;`:,@:..:,::@+'''';::@@,#@+:::;+;;:,.`         ``..,,,:;::,';.# @ # :   .   : `. ..:                                       
// ,@`.``.,` .```` ` ,`',#:.,;:::@@@@@@@@@@@@@@@@#+';,:;,..`        ``..;'::;;:;;''  @@.:.:``     .. .```                                       
//   ,.,`.:. ,``..```:.;:':,`:;:;@@@@@@@@@@@@@@@@@@@@. ;::..`        ``.,@;;;;;;;;++ @@ ,`,.`. .`   ,`.      ,;;@@`                             
//   ,.,.`,,`. `..`  , :,;::,:::':@@@@@@@@@@@@@@@@   `.:;;:,.``      ```,:@;;;;;;;'@. @:@ `:,... ;;` `` ,`  .  ::  :,,..,:::,.                  
//   :.`.,;,,. ``.```:`:.+;,:,::@                `   .,:;;;:,.```     ``.,''';;:;;'#@@ @.:``;,.`;.,;,.   `.` ,   ``:,``  `.:+.`.,,:;'++@,,`     
//   ,...,:,:. `.`` `:,,.@.:..::@            :`` ` ``.;;#:;:,..``      ``.,@'';;;''+##@@ .,,.:.',.;..`.. , .::;@,+,,;'':;, ``  ```...  `',``...,
//   .``..,:,, `,.   :,:.@.,:::;@        `,,.````` `..;#@@::,,.```      ``,;@'''''+++@@:,.:,,`,,:::,. ; , `,@@,';;+#@@@@@+:,,:,,:'';;:;';`.:.   
//   .``.,.,,, .,`. ```..@`,,:::@        ;:,....`````..,:#;:,,.```      ``.,@+''+'+'@, ``:#@ :,.;:;,:@ ,`'@,@@@@@@@@@@@@@@@@@':;';::'@@@@: ;; ``
//   .`` :,:,.``. `` `, `@`:,;:;@         ;:,,...````...,:',,,..``       ``.,@++++#: @@:.'@ ;.,,,#,.@    :'@@@@@@@@@@@@+@@@@@@@@@@@@@@@@@@@@@;+@
//   , .`,.,`` `.`` ``.` @`::::;@         @;::,,.......,,:;',,..`         ``.;@@@+;+@@@,,,;;,,,:@';;.                 `;@@@@@@@@@@@@@@@@@@@@@#@@
//`,,` . .,.````,``  `.``@,,,::;@          @;;:::,,,,,,,,::';,..``         `..@@+#@`@@;:.,'.`,.;+''@                                .@@@@@@@@@@@
//+'@`````::`  `.`.```..`@:.,:::+           @++';;;;;::,,,,:',,.``         ``.,@@@##@@@,.,,`;`@`:+;:                                  `         
//@@@````.:,,```..`. ..``+;`.:::',,;'  `:    @@@@@@@'::,,,,,:'..``          ``.'@''+@@'.;+:;`'';#'#                                             
//     `.::,,   `` .``,``+; .`,:;,,.``..,. `,` @@@@@':,,,,,,:;:,.``          ``.;''@@#`;;'''';;';#                                              
//     `,:::, `  .`...,``:;`.`.:,+@@@@@@@@@';.   ,@@;:,,,,,:::',..````       ``.;+@@';;;;;;;;;;;'`                                              
//    `,..,::`.````...,``.@,,.,.,, `:@@@@@@@@@@@@@@@,:::::,:,:;+:,.``        ``.:@@;::::::::;;;'                                                
//    `,, ,,, ` .`.`...``:',,::,,:                `@@;:,,,,,:::;+:,``         ``:@'::::::::;;';,,,,...:,                                        
//    `....,,  `.```..`.;,+````:.:                   @:::,,,,::;+@;,.`        `..;;::::::::;;;:@@@@@';;:,:;,````  `,,',.`                       
//    `..,:,, ``.```.`.,:,:..,,`:'                    +;::,,,,::;'@'.`       ``..:;::::::::::;@@@@@+;..  `...::.  .`.```  ..  ,.`.',..`         
//    `` ,.,:```````..:;...,,.,``@                    :;;,,,,,::;@;         ```.,,::::::;:;+` `:@@@@@@@@@@@@@@@@@#''@@@@'.:.::,,`,,,..  ``.`` `,
//     ` ``,,.,.    `+,: ..;,:` @;                     '::,,,:'@.          ``...,,:,,:;;:;@                    `,'@@@@@@@@@@@@@@@@@@+@@+'';::,,,
//     ```.:,.,`....`..,`..',.,.@;                    .::,,,:;         `....,,,,,,:,,,,::@                                       `'@@@@@@@@@@@@@
//     ,`.;:,..     .``.``.+, :;,:                    ;::::.       ``...,,,::::;;+:,,,,,,                                                       
//    `. :,;   `,++`.;..``.@.:'.:,                    ;,;`      ``....,,::;;;'+@;::,.,,,,`                                                      
//    @`:` ``.` .` ,;`:,...#`,::`:                   @:       ``....,::;'''+@;;;;::,,...,;                                                      
//    `;,`  , `  , `'`,,+,.;..,:.,                  `       ``....::;;''+@':;;;;;::,,...,,'                                                     
//@@@@`..``,.;;+:.         ;.,;;.;.                      ``....,:;''++@';;;;;;:::::,,,,,...;                                     `,,:'@@@@@;;:;'
//    `.` `'`.,``@@,,,,.#'..   ;,'.  `.,``;@,         ``...,,:;'+++@+:;;;;;;;::::,,,,.........                       .`,;';:;:,,:::,;::;,::,,,::
//    ..`  :` . .@.@@@@@@        `@@@@@@;        `````..,:;''++#@+::;;;;;;;;::::,,,....```....,`             .:++''::@;,:,,,,,,,:.,:::;,:;';;;''
//+:;:```                         `            ````.,,:;'++#@@:,:;;;;;;';;;;:::,,...````````````, `.,;@+;,:,:;,,   ::@+'''::::::::::;;::;;'++@@@
//;@@'`` .,,,,,,,,,,:;',.                  `  ``..:;'+#@@;..::::;;'''''';;;:::.``````` ` ````````` @#@';;'';;:.:  ``:@@'@@@@@@@@@@@@@@@@@@.     
//@@@@.`.@@@@;:.                    `````````.,:'+@@@,`.::::;;;''+'''';::,..````````````     `````` ,@'';###@@@@@@`.:.:+@@@@                    
//    . `,,.,:::,..,,:';.:,````````..`.``.,:;++;::::,::::;;'+++++';:,..``````````````` `       `````` @@@@@@;.  ,.``..;+;@@@@                   
//    :,.                  ....,,.....:@@@@@'@@@@::::;;;'+++#':,.....```````````````.```        ``````.         ,..`@;. `;#@                    
//    ,`;@@@'':;':;;'@@@@:` `...,,.'@     :'+@@@;,:;;'+++':,,.....```       ` ``````.,````      ```````.:        @.@@@#'@;;@                    
//    .`,: :,,          .;+'#+#@@@@      @;:;@:.,:;'+#':,,,,....````          ` ```..,.```       ` ```...,       ;,:,,;@@@ @                    
//    .`., ,:@@@@@@@@@@@##+##@#@++@       @;@@@@,:+#'::,,,....`````           ````...::..``      ` ```....       .,:` `.;'@@                    
//    .`., `,:::``;..`,.`,.,.::';'@      `@;@;@;;'+'::,,,....``````         ` ```...,:#,..`   `` ````....,,       :.`,'`:';@                    
//    .`.:,`::,:``:.``,, :...;,':;@       @';;@;;';;::,,....````````   ``````````..,,:@:..``````````....,,,`      :,..`.,;:+                    
//    ```;` ;:,,`,::....`.,,.;,';;@        ;::';'';::,,,.....`````````````````....,,:;@:...```````.....,,,,:      ,,.,`.,;:'                    
//    ```,,.;::,,:',..,,.`,.,,.':;@.       ;::#:';;::,,,.......``````````````...,,,::@@:,............,,,,,,:      ,...`..:;;                    
//    ```,` ':;,:::,;.;.,,:,:..;:;',       ,::+:'';:,,,..........`````````....,,,:::;@':,,,.........,,,,,,,:.     .,...`,::;                    
//     ` ;` '::,,'::;,,.:::,:..::;:;       +,:';';::,,,,,...........``.......,,:::;;@@';::,,,,.,,.,,,,,,,,,::     ..,,`.,:,;`                   
//     . .` :,`,,':,'::.,::,;.`::,:#       +,:';';::,,,,,,.................,,,::;;'@@@';;:::,,,,,,,,,,,,::::;     ....`..,;:`                   
//     .``. ;  :,;::'::.,;::;.`::,.@       ,,:''';:::,,,,,.,...........,,,,:::;''+@@@@@+'';;:::,,,::::::::::;     ..`@ ..,:::                   
//     .`.. :. ,,;:,'::`:;:::;.:,,,@       ::::;';;:::,,,,,,,,,,,,,,,,,::::;''+@@@@@@@@@@@+'';;;;;:::;:;;;;:;     .,....,.:,;                   
//     ,`.` ,``.,,,:+::,;::,;:.,,``+       ,:,:;';;;;:::,,,,,,,,,,,,:::;'+#@@@@@:@@@@@@@@@@@@@@@++''';'';;;:;     `,..`,,,:,;                   
//     ,``  ; ` .,::'::::,,:'  `;';#       ,:,,;';;;;:::,,,,,,,,,,,,::;;;;;;;;@ @@@@@@':,:::''+#@@+'';;;;;;;'      ,,,.,,.:,+                   
//     .``` '`` ::.:':,:;,::.`,::;;@       ::,:.@'';;;::,,,,,,,,,,,,,:::::::;;'+@@@@@;:,,,,,....,,:::::;;;;;;      :..,,.,,,+                   
//     ,.`  ;.` `:,:'::;;::;.`.:;;:@       ::,,`@'';;::::,,,,,,,,,,,,,::::;:;;;#@@@@;::,,,......,,,,::::;;;;:      ,.` `..,:;                   
//     ,...`+.`  `.:;,,:::::'.`:;;:@       ::,,,:+';;;::,,,,,,,,,,,,,,,:::::::;+@@@+;:,,,.........,,::::;;;;+.,,,,,,, @ ,,.::,:,.` :            
//     ,,..`',...` .:,:;:,::':`,;:,+       .,..:`@+'';:::,,,,,,,,,,,,,,,::::;;;#@@@';::,..........,,:::;;;;+       ,,:`,,..,:`     :            
//``   :,.,`+..` ``,,,:;:.;,;;`.::,'`      ,.+;:.#''';;::,,,,,,..,.,,,,,:::;;;;@@@@';::,.........,,,::::;;;;@;;@@@@,.,.,,,.:,;@+;.`+`:,:,,,``.``
//`,   ,...`'`,``. ..,,;..;,;:,..;;+`      .,;....#++';::,,,,,.......,,,::::;;#.@@@';::,,........,,,,:::::;;:,,,,,...,,,,,,,,,  ` `      ` `   `
// :  .,... '.. `` `.``:`.',;;,..+;';;';;,;:,,,..`@+'';;:,,,,.........,,,:::;;+`@@@+;::,,.........,,,,::::;,,..````.,.,,.,,,:,.,...``.:.,.,,,`,`
//  ,` :``. @``  ` ````;..:,;:,..';;;,... ..,,,.``:#+'';;:,,,.........,,:::;;;@.@@@';::,,.........,,,,::::;````.`.`:.,.,,,,,,:.`... `,..,`.,,...
//...`.,.., @`` ` ``. `:`.,,;;:`,+;;::...``.:,:,..:'++'';::,,,........,,::::;;@ ;@@';::,,.........,,:::::::,,,,:,.,.,.,.,` ,:,.:,,,:,.,::,...``.
// `,`.,`.. @``   ` .` , `..;:;;`,',:.`..`,.:;....:.@+'';::,,,,....,,,,,:::;;':,#@@+;;:,,........,,,:::::;,  `.,,,,,,.,...@`,:';@;;,;;:,::;::,:'
//.`..`,:`` @ `     .``, `.`,`:;:...:`.,.,..,:,@...,:#+'';::,,,,...,,,,,:::;;@..:@@+;::,,.......,,,::::::;.,.`..`.`,,,.,,;..,:.`,,,..`,,..`.,,..
//..,`.,:.. @ `:;.:;;. .,;,.,`.:;,..:``..,...::,.`.: @+''';::,,,..,.,,,,::;;'@` :@@+'::,,,,.....,,,,:::::'. ..,....,.:.:,,`,,;:,;,::,:;,.,,,...,
//..`.,,,;;:@,:`,;.:;:;,;:::+,.,:;.:,..,`.`.,:::.`,;.,@+'';::,,,,,,,,,,:::;;'@``:@@+'::,,,......,,,::::;:'.,,,,`..,,,..:,,.:,;'';:;:'';,;';,:,::
//.,,,,`````@````.` ,.;::,:,''`..;'+'`.,`,.,,;:::.,'.`'#+';;:,,,,,,,,,,,:::;'@`,,@@';::,,.......,,,::::;;:,,,....,,,,,:.:,.,,;,.,,::.:`,,,..,:..
//,.,,...```;,.``.``..;::```;,'.`.';',`,.,..,:::,..+`. @+'';;:,,,,,,,,,,,::;;''.`@@';::,........,,,:::;:;,.,.,`,`,::,,,,,:.,,:..`..@,@.,.`.';,;,
//;,:.::,.:,:;`` `  .`;:...:;..';`'.,::.,::,,:,,,..:.;,,@'';;:,,,,,,,,,,,:::;;@':@#':,,,....`....,,:::::'`...,.`..`,,,,:,,;,:;:`,``,,`,;;'::,:',
//`,.'`,.:,::',,,,,;  :..:,,,,;.',,,,..``. ..:,...,'.,..,@';;;:::,,,,,,,,,:::;#;:@@';,,......`...,,,::::,           :,,::,,,:;,   .. .`` `. ...`
//...``, ....+,.,,::,`.`,,`:;;.,:,':#::@,   .,:,.:.;     `@';;:,,,,,,.,,,,,:::;, @@+::,...````...,,,:::'            :,,,:,,::,,                 
//.,,...: ``:#` .``.;,,.,;;.,`.;,..:,' `,;',:.:.;:,:      ,+;;::,,,,....,,,::::@ ,@#;:,....```...,,::,,+            ;.,,,,.,:::                 
//...`;,; `:.+;:;::..:,:.,.,+;+..`:.'@:.    ..,,``.:       @;;::,,......,,,::::@  @@';,,..`````..,,,,,:@            ;,,,,,,,:,:                 
//::.,.::`.; @.`  .'.,..:`;:+,::' ,#,#         ...:'       @;;:::,.......,,,,,:+  ;@+;:,...```...,,,,,,+            :.,.,,.:,,: `    ``         
//      .,,``#,`..,:' ``, '.,,;,`:;,;':;.::,               ;;;::,...```...,,,,:'`  @@';:,..```....,,,..:            ;,,....:,,:           ````  
//,,.   ,`: `@,``.;,`` '`. `.;';`,:'.; ;.`.`. .`;  `. ..   `';;:,..``  `..,,,,:'    @#;:,.....`..........          ,,.....,,;:,         ````    
//````  ;, ` @:;`: ;`   . ;..;'`'...#,    `  ``    ``       @;;:,.`    `.,,,,::+. `.@@+;:,,........,..``,              ``.,:':'  `              
//      ;:,.`@.:..`; @@@@.`;`@,.'.';@:     `          . @   ';;:,..` ``.,,,,,:;@, ` @@#';:,,,,.....,.```.:     ,::,`. `                         
//      ;`.;.@:@, : :+@+:.@`.@,::,':',  `  .             ` ,::;:,..`    `...,,;;   @ .@#';::,,,..,..``   `.   :.`:`;@@`..,.,,,,.,;.,..,. .'`,,;.
//  ```.::.+:+.@ `;'':.:.@;@,, @.,#,:,.,,,:..` .``.    .    ;:::,,..`   ``...,'`   `.`.@++;::,.....       .. `, `` ` `, ': .  .,`;,  `,`;   ```,
//      :,:,:;.@ ..`.` `...`.##:;,+:;,`           ,:    `` .+,:,,..``   ``...,;, .``...`@+';:,...```      `.`@`   .   .      ::.@`.           ` 
//      ,.: ;'.@:.` ,: ,,.++`..,'.@,,.,.```.`,.`.`:.        ;,,....````  `...,:.`. . .`,.@';:,,.```       `.,   , ,`              `   ` :      `
//@@@@# :`;.`;;@:::::`,,,;;,:,:;,,+:,,,`..``....`:..```.   ::,,..`````  ```...,# ``.,.:;.;@';:,.``        `..,` @ `              .   `; ;`      
//````  :..:..:@,:;,``:,`'::;:;@`''.,,+`` ````,`` .`..@@@@@::,,..`````` ```..,,:@@@,      :+;:,.``        ``..,., , `` `,        `     ` `:     
//`   ` :,`:;`:@ ````,::., :,;;:.'@.,.@````   `.,;,,:,;'''';:,,..`````` ````..,:;`,...  ```@;:,.``        ```...,`#@`.:;@ ```@  .@ .    ``      
//      .,`,;,:@:,:,,:,:.:.,`...:':,:.@`..``. ```.`.#@@@@@@;:,,...``````````...,' `  ````  ';:,..```   ``````..:                 ``.`` .```';   
//,,,',`.:....:@,:,,::,:.: .`,,,+::.:'@            ``,     .;:,,...`....``.`...,:,...`...``:;:,..`` ` `  ```.,,,,.`           `                 
//     `.:``.`,@,,,,:,::`.`,.:,:;.,`';@    .```.```.:.....``@;:,,.........`....,,'`......``,;:,..```` ` ````..,,:.::::::.,.....``.,`..`.``````  
//,`.;`..;`..`,;:...:.,:`.,:,:;;,.:.@;@..   `   `      .``.,@';:,,,,,,,,......,,,;:,.......,':,,.````` ````..,::: `  ```  ` ` .`` `````` . ``.` 
//..    `'``.`,':```+ ,`  '::+;::,,,+;+`      `  .``       .`@';::::,,,,.,....,,::,`,`..``..+:,,...````````.,::::`` `  `            ` .`..`.:`  
// .`  ``:;`.`.;:``.,`, .`+;:;:::.:,';' `..` ` ``..,`.` ```...';::,,,,,,,,,.,,,,:;;...``.   @;,,.....``..`.,::::,`.``. ` `.,:,`..`,,;'@@;:,@':,`
//       .;;: ,';`.`. . .,;::;;:.`.,':'`  ``;@@@@:,,.,.``.....@'::,,....,,,,,,,,:;,    `  ` :':,,.........,::::;`` `.,;;:.,`                    
//`     ``;';:`;:`.`,`: ::::;;:;.,.,';;:;@@@,.` ,:,,`,...`` `` @;:,,.....,,,:::::':, `,@@@@@@';:,,,.....,::::::#                                
//`      ,:,,:.::` `. ;,,;,,:;:;..,:';;.        ``:.`.:.`.,`.. :+::,......,,,,:;;+`          @'';::,,,,,:::,,::@                         `   `  
//     `.,:;,,',;`..`.',:,``.::,...;'';' @@@@@@':+'.  `         @';,,......,,,:::'`           +;;;:;:::::,:,,::' :`#.,, ` ` `:.                 
//@@@'@::..,``;,'.::;:':,,`@@;::,,,;:;:;.:``                     @;:,,......,,,,:+  .   ` `:;,@;::::::,,,,,,,:'                                 
//,,,.. `.`..`;.::::::',.,.`.,:,,.:;::;'     ,@+@          `     :+;:,,.....,,,,:@  ` ` `     :;:,,,....,,,,,:#  `                              
//.:::,:;.`.``,.;,:;,:':..,...,...:',:;' ,    ;: .,@@@@@          @';:,,...,.,,:'.       ``    @::,,....,,,,:;:            .``'@@':@`       .::#
//       , .` ,.::,:,:;:.`..`.,..,:;,.+:   ` @`  .',         `.    @';:,,.,.,,,:@ .;  , .``  . ,;:,,.....,,,:;@@@@';,+@@@;:':@@'.:;;++;@@@:,@   
//:   .@ ` .` ,+:.::,:;,,`,.`,,.`,:;``;:        .@         .       :+'::,,,.,,,;@     ,+. ;:@#+:+;:,,....,,:;;':+:@@:@@,.,@'@@@;                
//` @   `.``` `'.:,.,:':` ,` ,,`.::'``:,    :    ',`:@       ``     @;;:,,,,.,,;#   `@.#@+:#:'+@;;:,,....,::;@@+                `.` `     .``@+`
//:@@,:  `..  `# :,:::;`` ,``,.`,:::``:.  ,  `@ +,@::#@@,       @:@@,';:,,,,.,,;:+;@;@@'@;@@@@@@'':,,,,..,::'      ``   ,     `.  ` `` .  .   ` 
// @:@ ,````  `+ .`,,:'`` :``,``,:::..',@  @@@@@#@@.@'@.@;@@,@@+,@,,'@;:,,,,,.,::`  `           +;::,....,:;: ``          `` . `    `` ;, `     
//@'@@@@@.``  .' .....,```,``,..:;;;.:+,;@@` @@@@@@@@@,.              @;,,....,::'      `` ..,..';;:,,...,:'.` ,.`          `.  `. :  .,  `     
//`@@#@@@,``` .+ .```., ` ,``:`..;::,:;,:            . ,       ` ````..':,,..`.,:+`.`,`` .,`  ``,';::,,..,,' `. ,`      `` ,   #```  `   ``  `  
//;`@@.@@:    .# `````.`.`,``;`.`;:;,,',:    ``.`;.,`,``.,`.:,,``   .  @;:,.```,:'...,.;```.:`  .';;:,,.`,,# ``,` ,..   ,`         ``   `@@@@@@@
//    `  . ```.' .`.``.:.`:`.:`..;;;.::,'    `    .` .     . .: .`,::..,'::,.`..,'` ,. `';.. `;@:;;::,..`,:;``: ``    ,@@@@':@.````.`..;;,`     
// ,@  @ . ````' :....,..`,.,,`.:::;:';:;    `` ` ``  ':@@'@+::;@@;;    +;:,,,` .:;  `  `   .`;+;;;;::,.``:.   ` , ` `    . `,. . ` `    ``  `  
//      @:`,```',,,,...,,.,`,,`.;;:;;;+,'    :.    :    .;#  .;'.,;'@# ,@:;:.,,  :'@@:'`@@@@@+#@;;;;::,.``;' .`.`:@'., `,:,@@@'` ```  `.   . :``
//@++@@@@@`.` .:.``...`,,`,.,.,;,';::::,,@ . `,', :.;`  .`    , ,,  ';@@+,:'..    ,   `:@@'@:;+@:;::;;,.``':..'+@@,:;;,.                 `  `  `
//';@;@@''`.  `:. ....`..`:`,,`,,;;;;;::: . .````  .'..,.@@';@'@@@@@@@@+@:,,:;      @@@+@#@@@@'+,::,:;:.  ,, ``. `  .`  `` ` `         `` ` ` , 
//+#'''#;;`.`  ;` .``.....,.,...,';;':;',,`   ``.`, `: ``:.`:@@@+@;@@+`:,;,,,         ,`.  `,  ,:,`.,::.`  '      `   .` ` .``.,..   ```  `    .
//':,@@@@@ ,`  ;` .`,````.` ,...,;:::;''`.`     ` .@;.` .  `;            @:,``           `  `.,.',`.,;@     `.:``..,`  ``  ,   ` ``` . ``     ` 
//    '@;'`.`  '.`.......:`..`..,,:;;;+#.:       `.`,`:::@@@. `      ``` `',``          `````` :@:,.;        `   ..` `., ,`, `.`   `. ., ` , :,.
//        `.`  ', `.`...`;...`..,.;:':+@`,.  `. ` ` . `,`.,    ,'@@@@.,   `..``   `   `,  ```  @;:,, `        `.`` .   `  .            ```.```:`
//@@    :@ ``  ;,``..:.``;``..`...,:::;;';@.,.` `.,`,``, . ,``` `    ```:@@,.```        ,`,`.`.:;;`` `         .`` ` .     ` .     ,  ` .`.  .  
//.`.. `. .`` `;:`.:,:.``;``.`...`,.:,.,:.  `..,,,,``.`..,..```. ``,.`.,:@@:.....      `,.`   ':;```.          ..        ` `        `  `.  ``,  
//. ````. :  `.:;,.,.,```;``.`,.,`,`,;;.`,`....;.:;`,....;...,...,``.. ` ` `,,.`.`     `,..,;;+#``..`     ``, ,.  `` `,.``` . .`` . .`,`` ` ` .,
//  ` `.:.; ``,:;,,..;```; ```.@''+',..':; :., ;`: ..,. .,.,`` .,`.:`,.:``,,,,`@@`.    @ ,`` :;,`....`'. ` ```:... `` ,  `.` ```..`.. ,`....`   
//   ,.`.`;`.`.,',:,`:`. . ``;:`` ,.;;..`:.,`.,.. `.:.,`.,`...., , ;,,````, :, @@@:`   @..,``+.,,.,,+.,`.`  `,` ` ``..,. ,`.` .  .````  ` `     
//        :`,.:,',,.,',';#,:';.`.:;. +..`:` ,.:  .,,...`. `:;,.,  ,`..:`.`, :. @@@@`.  @ `,. ',;`.,.@@`...` . `.```.,...``,,. . ```  `. ,`,  .  
//   ``   ;. `` '.`````.,',:;: ::..;: ,;`@. .` `: :+ ;,,,:,.,`,,, .`. ,: ,  .,.;@@@@   @ `.,;::,,:;@@@@..`  ` `,`, .`.````; `.`` `. .    ``     
//```,` ..;.`,.`::`.``.``,,;:, ;``@. ,.`;; .`..'::` ;.,`.:````  ```. `.` .`,`,,,@@@@:` @ ,`.,:;,,,`@@@@@ `  `.,`.`..``.` ``  ... `   ` ``.``   `
//`  . . `.' ```,;,,::,.` ..,`., ::``., ;' ,`': ,. `  `  ` ` `  `` `    `  ` `, @#@@@   .  . ;:::,,@@@@@ . .```.``  ` ```` ````` ``` `..`   ``  
//. `  ` `.;`.  ,@.',.,,,.  ` ,, ;:``.,,,;.` ..``.  `````. `...``,``..`.  `. .. @@+@@@  @ ` .,,.,`@@@@@@   ..`.. ``,. ``,`;`   `` .` , `` ` `  `
// ``  ..` :`:` `@.':',,,:,: `., ;;,`,`.``,......``. ```,``  `  `,``...`  ` `,. @##@@@` @@  `:',`,@@@@@@.  ,````.`.,,..` `..   ` ``  ``  ` `  ,`
// . ```, ` ,,` ,@,,`.,   .:,::`,:,;. ,..:::..`.`..` ``  . ```    .``` ,,: ``,, @''+#@@  @`  :;,`@@@@@@@,  ,...,.,`    ` ` ` ..  . ` `` .` `  ``
//  ;: `. `::.,.+ ;,.. ` `` :  ,;.`:+,,,.,.,.,::,:,.`  ```   ;` . `` .., ,...`, @'#+#+@  @@  :+.@@@@@@@@;  :..`.`` `  .``  .::   , ``  ,.```,`.`
//`.`...,.,#,,, ,@,,   ',`.`@`@,`.  .``.,`,,,....,,.``.,,,`  .    .``...`.`:`., @#@@@#@@@@@@ :;@@@@@@@@@@  ,,,.. ;. ``    `,, :.`. .  :  `  ` . 
//....` ...`,`.`,. ;#: ,  ,.```,`   `.``.`:@;,...``..`.:  `.,   `     .```, `:,:@'@@@@@@@@@@@@:;.  .. .,,  .. ,.``  .`. `,  `` ``  ` .  . .  ,.,
//   ::,::'`` ..':. ,,    ,,,   .`` ,,,@#;       ` ` `..,,;#,.:.;    .', ..`,`:,+@@@@@@@@@@@#:,':..,`,. @@@  .      `` ``    ` .     .`` `` `   
//  ` `   ,+::.,;@+'#,..` :,...,,                 ``.........  `        `,+,. .  .   ` ,,.,`,   .  ,;.  ` .  .   ,   .   .@.  @ .:  .`. .   ,`. 
//
