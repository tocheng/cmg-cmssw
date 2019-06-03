// Hengne Li, 2016 @ CERN, initial version.
 
// root
#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TBranch.h"
#include "TLegend.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TH2F.h"
#include "TH2.h"
#include "TProfile2D.h"
#include "TF1.h"
#include "TPaveText.h"
#include "TObjArray.h"
#include "TMath.h"
#include "TCanvas.h"
#include "TVector2.h"
#include "TProfile.h"
#include "TGraphErrors.h"
#include "TEntryList.h"
#include "TLorentzVector.h"
#include "TRandom3.h"

// std
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <time.h>

// other
#include "tdrstyle.h"
#include "PParameterReader.h"
#include "KalmanMuonCalibrator.h"
#include "ZZCorrections.h"
#include "JetCorrectorParameters.h"
#include "JetCorrectionUncertainty.h"
#include "FactorizedJetCorrector.h"
#include "JetResolution.h"



//======================================================
// ╔═╗╦  ╔═╗╔╗ ╔═╗╦    ╦  ╦╔═╗╦═╗╦╔═╗╔╗ ╦  ╔═╗╔═╗ 
// ║ ╦║  ║ ║╠╩╗╠═╣║    ╚╗╔╝╠═╣╠╦╝║╠═╣╠╩╗║  ║╣ ╚═╗ 
// ╚═╝╩═╝╚═╝╚═╝╩ ╩╩═╝   ╚╝ ╩ ╩╩╚═╩╩ ╩╚═╝╩═╝╚═╝╚═╝ 
//======================================================


// Intput output root files

std::string _file_in_name;
std::string _file_out_name;
std::string _file_config_name;

TFile* _file_in;
TFile* _file_out;

// input output root trees
TTree* _tree_in;
TTree* _tree_out;

// selected entry list
TEntryList* _selected_entries;

// output plots file
std::string _plots_out_name;

// if DYJets samples
bool _isDyJets, _isDyJetsLO;

int _isDyJetsLOnjets;
double _DyJetsNLOxsec;

// if SM ZZ samples
bool _isZZ;

// strings for temporary usage
char name[3000], name1[3000], name2[3000];

// random number
TRandom3* _rand3;

//======================================================
// ╔═╗╔═╗╔╗╔╔═╗╦╔═╗  ╔═╗╔═╗╦═╗╔═╗╔╦╗╔═╗╔╦╗╔═╗╦═╗╔═╗
// ║  ║ ║║║║╠╣ ║║ ╦  ╠═╝╠═╣╠╦╝╠═╣║║║║╣  ║ ║╣ ╠╦╝╚═╗
// ╚═╝╚═╝╝╚╝╚  ╩╚═╝  ╩  ╩ ╩╩╚═╩ ╩╩ ╩╚═╝ ╩ ╚═╝╩╚═╚═╝
//======================================================


//====================
// general paramters
//====================

// debug
bool _debug=false;

// starting entry number 
int _n_start = 0;

// number of entries to be run, -1 means all entries
int _n_test = -1;

// n entries interval between which print once entry number
int _n_interval = 1000;

// use slimmed tree or not
bool _useLightTree = true;

// store errs
bool _storeErr = true;

// store HLT flags
bool _removeHLTFlag = false;

// store MET flags
bool _removeMETFlag = false;

// tree selection string
std::string _selection = "(1)";

// store old branches 
bool _storeOldBranches = false;


/// MT Unc from MET Unc
bool _doMTUnc = true;
bool _doMTUncDummy = false;


//=========================
// add PU weights
//=========================
bool _addPUWeights = true;
// protect pu weight not greater than this value
double _PUWeightProtectionCut = 1000;
// PU input files directory
std::string _PUInputDir;
// PU input tags, "puWeight<tag>" branches will be added
std::vector<std::string> _PUTags;
// PU input root files, one-to-one correspnding to the tags above
std::vector<std::string> _PUInputFileNames;
// PU weight hist name
std::string _PUWeightHistName;

// not from config file:
std::vector<TFile*> _PUFiles;
std::vector<TH1D*> _PUHists;
std::vector<Float_t*> _PUWeights;


//==========================
// recalibrate muon pt
//==========================
bool _doMuonPtRecalib = false;
// recalibrator input files
std::string _MuonPtRecalibInputForData, _MuonPtRecalibInputForMC;

// not from config file:
KalmanMuonCalibrator* _muCalib;

// electron simple recalib
bool _doElecPtRecalibSimpleData = false;
bool _doElecPtRecalibSimpleDataPogRecipe = false;
// elec escale
double _ElecPtRecalibSimpleDataScale = 1.0;

// muon simple recalib
bool _doMuonPtRecalibSimpleData = false;
// muon escale
double _MuonPtRecalibSimpleDataScale = 1.0;

//========================
// Add DYJet gen reweight 
//========================
// Add DYJet gen reweight according to 
// unfolded 2015 data precision measurement results 
// will also reweight LO to NLO MC samples to gain statistics.

// default will use histograms for both NLO and LO reweight
bool _addDyZPtWeight = true;

// for NLO MC use function parametrization instead of hist.
bool _addDyZPtWeightUseFunction = true;

// for NLO MC use function from resbos nnlo gluon resummation
bool _addDyZPtWeightUseResummationFunction = true;

// for NLO MC use function from resbos nnlo gluon resummation but refitted to data
bool _addDyZPtWeightUseResummationRefitFunction = true;

// for LO MC to NLO, also use function  
bool _addDyZPtWeightLOUseFunction = true;

// Input root file
std::string _DyZPtWeightInputFileName = "data/zptweight/dyjets_zpt_weight_lo_nlo_sel.root";

// add new DY jets gen-weights for mergeing NLO and LO
// to gain statistics
bool _addDyNewGenWeight = true;

// not from config file
TFile* _fdyzpt;
TH1D*  _hdyzpt_dtmc_ratio;
TF1*   _fcdyzpt_dtmc_ratio;
TF1*   _fcdyzpt_dtmc_ratio_resbos;
TF1*   _fcdyzpt_dtmc_ratio_resbos_refit;
TH1D*  _hdyzpt_mc_nlo_lo_ratio;
TF1*   _fcdyzpt_mc_nlo_lo_ratio;

//========================
// Add SM ZZ Corrections 
//========================
// Add SM ZZ NNLO/NLO QCD corrections and NLO EW corrections

// 
bool _addZZCorrections = true;

// ew correction input file name
std::string _ZZCorrectionEwkInputFileName = "data/zzcorr/ZZ_EwkCorrections.dat";
std::string _ZZCorrectionQcdInputFileName = "data/zzcorr/zzqcd.root";

// not from config file
ZZCorrections* _zzCorr;


//==============================================
// Do JEC JER
//==============================================

bool _doJEC = false;
bool _doJER = false;

// JEC parameters
// data jec
std::string _JECParTxt_DATA_L2L3Residual;
std::string _JECParTxt_DATA_L3Absolute;
std::string _JECParTxt_DATA_L2Relative; 
std::string _JECParTxt_DATA_L1FastJet;
std::string _JECParTxt_DATA_Uncertainty;
// mc jec
std::string _JECParTxt_MC_L2L3Residual;
std::string _JECParTxt_MC_L3Absolute;
std::string _JECParTxt_MC_L2Relative; 
std::string _JECParTxt_MC_L1FastJet;
std::string _JECParTxt_MC_Uncertainty;

// JER parameters
std::string _JERParTxt_Reso_DATA;
std::string _JERParTxt_Reso_MC;
std::string _JERParTxt_SF_MC;


// not from config file
// JEC parameters
// data
JetCorrectorParameters *_JEC_ResJetPar_DATA;
JetCorrectorParameters *_JEC_L3JetPar_DATA;
JetCorrectorParameters *_JEC_L2JetPar_DATA;
JetCorrectorParameters *_JEC_L1JetPar_DATA;
// mc
JetCorrectorParameters *_JEC_ResJetPar_MC;
JetCorrectorParameters *_JEC_L3JetPar_MC;
JetCorrectorParameters *_JEC_L2JetPar_MC;
JetCorrectorParameters *_JEC_L1JetPar_MC;

// jec parametesrs vec
std::vector<JetCorrectorParameters> _JEC_vPar_DATA;
std::vector<JetCorrectorParameters> _JEC_vPar_MC;

// jec corrector
FactorizedJetCorrector *_JEC_JetCorrector_DATA;
FactorizedJetCorrector *_JEC_JetCorrector_MC;

// jec uncertainty
JetCorrectionUncertainty *_JEC_Uncertainty_DATA; 
JetCorrectionUncertainty *_JEC_Uncertainty_MC; 

// JER parameters
JME::JetResolution* _JER_Reso_DATA;
JME::JetResolution* _JER_Reso_MC;
JME::JetResolutionScaleFactor* _JER_SF_MC;

JME::JetParameters* _JERPar_Reso_DATA;
JME::JetParameters* _JERPar_Reso_MC;


//==============================================
// Do simple version of the MET recoil tuning
//==============================================

// default use histogram
bool _doRecoil = false;

// use smoothed hist
bool _doRecoilUseSmooth = true;

// use graph from smoothed hist
bool _doRecoilUseSmoothGraph = true;

// input files
std::string _RecoilInputFileNameData_all, _RecoilInputFileNameData_mu, _RecoilInputFileNameData_el;
std::string _RecoilInputFileNameMC_all, _RecoilInputFileNameMC_mu, _RecoilInputFileNameMC_el;
std::string _RecoilInputFileNameMCLO_all, _RecoilInputFileNameMCLO_mu, _RecoilInputFileNameMCLO_el;
std::string _RecoilInputFileNameGJets_all, _RecoilInputFileNameGJets_mu, _RecoilInputFileNameGJets_el;

// not from config file
TFile* _file_dt_sigma[10];
TFile* _file_mc_sigma[10];
TH1D* _h_dt_met_para_shift[10];
TH1D* _h_mc_met_para_shift[10];
TH1D* _h_met_para_shift_dtmc[10];
TH1D* _h_dt_met_para_sigma[10];
TH1D* _h_dt_met_perp_sigma[10];
TH1D* _h_mc_met_para_sigma[10];
TH1D* _h_mc_met_perp_sigma[10];
TH1D* _h_ratio_met_para_sigma_dtmc[10];
TH1D* _h_ratio_met_perp_sigma_dtmc[10];
TProfile* _p_dt_zpt[10];
TGraphErrors* _gr_dt_met_para_shift[10];
TGraphErrors* _gr_mc_met_para_shift[10];
TGraphErrors* _gr_met_para_shift_dtmc[10];
TGraphErrors* _gr_ratio_met_para_sigma_dtmc[10];
TGraphErrors* _gr_ratio_met_perp_sigma_dtmc[10];


//==============================================
// eff scale
//==============================================
bool _addEffScale = true;
bool _addEMuTrgScale = false;
bool _addEffScaleOnData = false;
// eff scale options
std::string _EffScaleMCVersion = "80xSummer16";
// Input files for:
// - el id iso eff
std::string _EffScaleInputFileName_IdIso_El = "data/eff/egammaEffi.txt_SF2D.root";
// - el tracking eff
std::string _EffScaleInputFileName_Trk_El = "data/eff/egammatracking.root";
// - mu id iso eff
std::string _EffScaleInputFileName_IdIso_Mu = "data/eff/muon80x12p9.root";
// - mu tracking eff
std::string _EffScaleInputFileName_Trk_Mu = "data/eff/muontrackingsf.root";
// - el trigger eff
std::string _EffScaleInputFileName_Trg_El = "data/eff/trigereff12p9.root";
// - mu trigger eff
std::string _EffScaleInputFileName_Trg_Mu = "data/eff/trigeff_mu.root";


// not from config file
// electron sf
TFile* _file_idiso_el;
TH2F* _h_sf_idiso_el;
// electron tracking sf
TFile* _file_trksf_el;
TH2F* _h_sf_trk_el;

// muon tracking sf
TFile* _file_trksf_mu;
TH1F* _h_sf_trk_mu;

// muon id iso sf
TFile* _file_idiso_mu;
// for Spring16
TH2F* _h_eff_trkhpt_mu_dt;
TH2F* _h_eff_trkhpt_mu_mc;
TH2F* _h_eff_hpt_mu_dt;
TH2F* _h_eff_hpt_mu_mc;
TH2F* _h_sf_iso_mu;
// for Summer16
TH2* _h_eff_trkhpt_mu_dt_1;
TH2* _h_eff_trkhpt_mu_mc_1;
TH2* _h_eff_hpt_mu_dt_1;
TH2* _h_eff_hpt_mu_mc_1;
TH2* _h_sf_iso_mu_1;
TH2* _h_eff_trkhpt_mu_dt_2;
TH2* _h_eff_trkhpt_mu_mc_2;
TH2* _h_eff_hpt_mu_dt_2;
TH2* _h_eff_hpt_mu_mc_2;
TH2* _h_sf_iso_mu_2;

// electron trigger sf
TFile* _file_trg_el;
TH2* _h_sf_trg_el_l1;


// muon trigger sf
TFile* _file_trg_mu;
// for Spring16
TH2D* _h_eff_trg_mu_l1_tot;
TH2D* _h_eff_trg_mu_l2_tot;
TH2D* _h_eff_trg_mu_l1_l1p;
TH2D* _h_eff_trg_mu_l2_l1p;
TH2D* _h_eff_trg_mu_l1_l1f;
TH2D* _h_eff_trg_mu_l2_l1f;
TH2D* _h_eff_trg_mu_l1_l1pl2f;
TH2D* _h_eff_trg_mu_l1_l1pl2p;
TH2D* _h_eff_trg_mu_l1_l1fl2p;
TH2D* _h_eff_trg_mu_l2_l1pl2f;
TH2D* _h_eff_trg_mu_l2_l1pl2p;
TH2D* _h_eff_trg_mu_l2_l1fl2p;
Double_t _N_eff_trg_mu_tot;
Double_t _N_eff_trg_mu_tot_err;
Double_t _N_eff_trg_mu_l1pl2f;
Double_t _N_eff_trg_mu_l1pl2f_err;
Double_t _N_eff_trg_mu_l1pl2p;
Double_t _N_eff_trg_mu_l1pl2p_err;
Double_t _N_eff_trg_mu_l1fl2p;
Double_t _N_eff_trg_mu_l1fl2p_err;
Double_t _N_eff_trg_mu_l1p;
Double_t _N_eff_trg_mu_l1p_err;
Double_t _N_eff_trg_mu_l1f;
Double_t _N_eff_trg_mu_l1f_err;
Int_t _NPtBins_eff_trg_mu;
Int_t _NEtaBins_eff_trg_mu;
TH2D* _h_eff_trg_mu_l1_tot_norm;
TH2D* _h_eff_trg_mu_l2_tot_norm;
TH2D* _h_eff_trg_mu_l1_l1p_norm;
TH2D* _h_eff_trg_mu_l1_l1f_norm;
TH2D* _h_eff_trg_mu_l2_l1p_norm;
TH2D* _h_eff_trg_mu_l2_l1f_norm;
TH2D* _h_eff_trg_mu_l1_l1pl2f_norm;
TH2D* _h_eff_trg_mu_l1_l1pl2p_norm;
TH2D* _h_eff_trg_mu_l1_l1fl2p_norm;
TH2D* _h_eff_trg_mu_l2_l1pl2f_norm;
TH2D* _h_eff_trg_mu_l2_l1pl2p_norm;
TH2D* _h_eff_trg_mu_l2_l1fl2p_norm;
TH2D* _h_eff_trg_mu_l1_l1p_norm_vs_tot;
TH2D* _h_eff_trg_mu_l1_l1f_norm_vs_tot;
TH2D* _h_eff_trg_mu_l2_l1p_norm_vs_tot;
TH2D* _h_eff_trg_mu_l2_l1f_norm_vs_tot;
TH2D* _h_eff_trg_mu_l1_l1pl2f_norm_vs_tot;
TH2D* _h_eff_trg_mu_l1_l1pl2p_norm_vs_tot;
TH2D* _h_eff_trg_mu_l1_l1fl2p_norm_vs_tot;
TH2D* _h_eff_trg_mu_l2_l1pl2f_norm_vs_tot;
TH2D* _h_eff_trg_mu_l2_l1pl2p_norm_vs_tot;
TH2D* _h_eff_trg_mu_l2_l1fl2p_norm_vs_tot;
TH2D* _h_eff_trg_mu_l1_l1pl2f_norm_vs_l1p;
TH2D* _h_eff_trg_mu_l1_l1pl2p_norm_vs_l1p;
TH2D* _h_eff_trg_mu_l1_l1fl2p_norm_vs_l1f;
TH2D* _h_eff_trg_mu_l2_l1pl2f_norm_vs_l1p;
TH2D* _h_eff_trg_mu_l2_l1pl2p_norm_vs_l1p;
TH2D* _h_eff_trg_mu_l2_l1fl2p_norm_vs_l1f;
// for Summer16
TH2* _h_eff_trg_mu50_dt_1;
TH2* _h_eff_trg_mu50_dt_2;
TH2* _h_eff_trg_mu50_dt_3;
TH2* _h_eff_trg_mu50_dt_4;
TH2* _h_eff_trg_mu50_mc_1;
TH2* _h_eff_trg_mu50_mc_2;
TH2* _h_eff_trg_mu50_mc_3;
TH2* _h_eff_trg_mu50_mc_4;
TH2* _h_eff_trg_mu50tkmu50_mc_1;
TH2* _h_eff_trg_mu50tkmu50_mc_2;
TH2* _h_eff_trg_mu50tkmu50_mc_3;
TH2* _h_eff_trg_mu50tkmu50_mc_4;
TH2* _h_eff_trg_mu50tkmu50_dt_1;
TH2* _h_eff_trg_mu50tkmu50_dt_2;
TH2* _h_eff_trg_mu50tkmu50_dt_3;
TH2* _h_eff_trg_mu50tkmu50_dt_4;


//==================================================
// GJets Skimming
//==================================================
bool _doGJetsSkim = false;
bool _doGJetsSkimAddPhiWeight = false;
bool _doGJetsSkimAddTrigEff = false;
std::string _GJetsSkimInputFileName;
std::string _GJetsSkimPhiWeightInputFileName;
std::string _GJetsSkimRhoWeightInputFileName;
std::string _GJetsSkimTrigEffInputFileName;


TFile* _gjets_input_file;
TH2D* _gjets_h_zmass_zpt;
TH2D* _gjets_h_zmass_zpt_el;
TH2D* _gjets_h_zmass_zpt_mu;
TH1D* _gjets_h_zpt_ratio;
TH1D* _gjets_h_zpt_ratio_el;
TH1D* _gjets_h_zpt_ratio_mu;
TH1D* _gjets_h_zpt_lowlpt_ratio;
TH1D* _gjets_h_zpt_lowlpt_ratio_el;
TH1D* _gjets_h_zpt_lowlpt_ratio_mu;
TH1D* _gjets_h_zpt_ratio_up;
TH1D* _gjets_h_zpt_ratio_dn;
TH1D* _gjets_h_zpt_ratio_el_up;
TH1D* _gjets_h_zpt_ratio_el_dn;
TH1D* _gjets_h_zpt_ratio_mu_up;
TH1D* _gjets_h_zpt_ratio_mu_dn;
TGraphErrors* _gjets_gr_zpt_ratio;
TGraphErrors* _gjets_gr_zpt_ratio_el;
TGraphErrors* _gjets_gr_zpt_ratio_mu;
TGraphErrors* _gjets_gr_zpt_lowlpt_ratio;
TGraphErrors* _gjets_gr_zpt_lowlpt_ratio_el;
TGraphErrors* _gjets_gr_zpt_lowlpt_ratio_mu;
TGraphErrors* _gjets_gr_zpt_ratio_up;
TGraphErrors* _gjets_gr_zpt_ratio_dn;
TGraphErrors* _gjets_gr_zpt_ratio_el_up;
TGraphErrors* _gjets_gr_zpt_ratio_el_dn;
TGraphErrors* _gjets_gr_zpt_ratio_mu_up;
TGraphErrors* _gjets_gr_zpt_ratio_mu_dn;
std::vector< TH1D* > _gjets_h_zmass_zpt_1d_vec;
std::vector< TH1D* > _gjets_h_zmass_zpt_el_1d_vec;
std::vector< TH1D* > _gjets_h_zmass_zpt_mu_1d_vec;

TFile* _gjets_phi_weight_input_file;
TH1D* _gjets_h_photon_phi_weight;

TFile* _gjet_rho_weight_input_file;
TH2D* _gjet_h_rho_weight;

TFile* _gjets_trig_eff_input_file;
TH2D* _gjets_h_trig_eff_weight;

bool _doBoostJet = true;

//======================================================
// ╔╦╗╦═╗╔═╗╔═╗  ╦  ╦╔═╗╦═╗╦╔═╗╔╗ ╦  ╔═╗╔═╗
//  ║ ╠╦╝║╣ ║╣   ╚╗╔╝╠═╣╠╦╝║╠═╣╠╩╗║  ║╣ ╚═╗
//  ╩ ╩╚═╚═╝╚═╝   ╚╝ ╩ ╩╩╚═╩╩ ╩╚═╝╩═╝╚═╝╚═╝
//======================================================


// sum events, sum weights
Double_t _SumEvents, _SumWeights;

// isData
Int_t _isData;

// run, lumi, evt
UInt_t _run, _lumi;
ULong64_t _evt;

// other var
Float_t _rho;
Float_t _xsec;

// leptons, Z, MET
Float_t _llnunu_mt;
// alternative mts
Float_t _llnunu_mt_JetEnUp,_llnunu_mt_JetEnDn, _llnunu_mt_JetResUp,_llnunu_mt_JetResDn;
Float_t _llnunu_mt_MuonEnUp,_llnunu_mt_MuonEnDn; 
Float_t _llnunu_mt_ElectronEnUp,_llnunu_mt_ElectronEnDn;
Float_t _llnunu_mt_PhotonEnUp,_llnunu_mt_PhotonEnDn;
Float_t _llnunu_mt_TauEnUp,_llnunu_mt_TauEnDn;
Float_t _llnunu_mt_UnclusterUp,_llnunu_mt_UnclusterDn;
////////////////////
Float_t _llnunu_mt_el_JetEnUp,_llnunu_mt_el_JetEnDn, _llnunu_mt_el_JetResUp,_llnunu_mt_el_JetResDn;
Float_t _llnunu_mt_el_MuonEnUp,_llnunu_mt_el_MuonEnDn;
Float_t _llnunu_mt_el_ElectronEnUp,_llnunu_mt_el_ElectronEnDn;
Float_t _llnunu_mt_el_PhotonEnUp,_llnunu_mt_el_PhotonEnDn;
Float_t _llnunu_mt_el_TauEnUp,_llnunu_mt_el_TauEnDn;
Float_t _llnunu_mt_el_UnclusterUp,_llnunu_mt_el_UnclusterDn;
////
Float_t _llnunu_mt_mu_JetEnUp,_llnunu_mt_mu_JetEnDn, _llnunu_mt_mu_JetResUp,_llnunu_mt_mu_JetResDn;
Float_t _llnunu_mt_mu_MuonEnUp,_llnunu_mt_mu_MuonEnDn;
Float_t _llnunu_mt_mu_ElectronEnUp,_llnunu_mt_mu_ElectronEnDn;
Float_t _llnunu_mt_mu_PhotonEnUp,_llnunu_mt_mu_PhotonEnDn;
Float_t _llnunu_mt_mu_TauEnUp,_llnunu_mt_mu_TauEnDn;
Float_t _llnunu_mt_mu_UnclusterUp,_llnunu_mt_mu_UnclusterDn;

//////////////////// 
Float_t _llnunu_l1_mass, _llnunu_l1_mt;
Float_t _llnunu_l1_pt, _llnunu_l1_phi, _llnunu_l1_eta;
Float_t _llnunu_l1_deltaPhi, _llnunu_l1_deltaR, _llnunu_l1_rapidity;
Float_t _llnunu_l2_pt, _llnunu_l2_phi;
// alternative METs
// _llnunu_l2_t1Phi_JetEnDn
Float_t _llnunu_l2_pt_JetEnUp, _llnunu_l2_phi_JetEnUp;
Float_t _llnunu_l2_pt_JetEnDn, _llnunu_l2_phi_JetEnDn;
Float_t _llnunu_l2_pt_JetResUp, _llnunu_l2_phi_JetResUp;
Float_t _llnunu_l2_pt_JetResDn, _llnunu_l2_phi_JetResDn;
Float_t _llnunu_l2_pt_MuonEnUp, _llnunu_l2_phi_MuonEnUp;
Float_t _llnunu_l2_pt_MuonEnDn, _llnunu_l2_phi_MuonEnDn;
Float_t _llnunu_l2_pt_ElectronEnUp, _llnunu_l2_phi_ElectronEnUp;
Float_t _llnunu_l2_pt_ElectronEnDn, _llnunu_l2_phi_ElectronEnDn;
Float_t _llnunu_l2_pt_PhotonEnUp, _llnunu_l2_phi_PhotonEnUp;
Float_t _llnunu_l2_pt_PhotonEnDn, _llnunu_l2_phi_PhotonEnDn;
Float_t _llnunu_l2_pt_TauEnUp, _llnunu_l2_phi_TauEnUp;
Float_t _llnunu_l2_pt_TauEnDn, _llnunu_l2_phi_TauEnDn;
Float_t _llnunu_l2_pt_UnclusterUp, _llnunu_l2_phi_UnclusterUp;
Float_t _llnunu_l2_pt_UnclusterDn, _llnunu_l2_phi_UnclusterDn;
///////////////////
//  Recoil uncertainties
Float_t _llnunu_l2_pt_RecoilUp, _llnunu_l2_pt_RecoilDn;
Float_t _llnunu_l2_phi_RecoilUp, _llnunu_l2_phi_RecoilDn;
Float_t _llnunu_mt_RecoilUp, _llnunu_mt_RecoilDn;
Float_t _llnunu_l2_pt_el_RecoilUp, _llnunu_l2_pt_el_RecoilDn;
Float_t _llnunu_l2_phi_el_RecoilUp, _llnunu_l2_phi_el_RecoilDn;
Float_t _llnunu_mt_el_RecoilUp, _llnunu_mt_el_RecoilDn;
Float_t _llnunu_l2_pt_mu_RecoilUp, _llnunu_l2_pt_mu_RecoilDn;
Float_t _llnunu_l2_phi_mu_RecoilUp, _llnunu_l2_phi_mu_RecoilDn;
Float_t _llnunu_mt_mu_RecoilUp, _llnunu_mt_mu_RecoilDn;

/////////////////////////
Float_t _llnunu_l2_sumEt, _llnunu_l2_rawPt, _llnunu_l2_rawPhi, _llnunu_l2_rawSumEt;
Float_t _llnunu_l2_genPhi, _llnunu_l2_genEta;
Float_t _llnunu_l1_l1_pt, _llnunu_l1_l1_eta, _llnunu_l1_l1_phi;
Float_t _llnunu_l1_l1_rapidity, _llnunu_l1_l1_mass, _llnunu_l1_l1_ptErr;
Int_t   _llnunu_l1_l1_pdgId, _llnunu_l1_l1_charge;
Float_t _llnunu_l1_l2_pt, _llnunu_l1_l2_eta, _llnunu_l1_l2_phi;
Float_t _llnunu_l1_l2_rapidity, _llnunu_l1_l2_mass, _llnunu_l1_l2_ptErr;
Int_t   _llnunu_l1_l2_pdgId, _llnunu_l1_l2_charge;
Float_t _llnunu_l1_l1_eSCeta, _llnunu_l1_l2_eSCeta;
Float_t _llnunu_l1_l1_eSeedXtal, _llnunu_l1_l2_eSeedXtal;
Float_t _llnunu_l1_l1_highPtID, _llnunu_l1_l2_highPtID;
Int_t   _llnunu_l1_l1_trigerob_HLTbit, _llnunu_l1_l2_trigerob_HLTbit;

// boostJet
Int_t _nboostJetAK8Puppi;
Int_t _boostJetAK8Puppi_puId[10];
Float_t _boostJetAK8Puppi_rawPt[10];

Float_t _boostJetAK8Puppi_pt[10];
Float_t _boostJetAK8Puppi_eta[10];
Float_t _boostJetAK8Puppi_rapidity[10];
Float_t _boostJetAK8Puppi_phi[10];
Float_t _boostJetAK8Puppi_mass[10];
Float_t _boostJetAK8_pt,_boostJetAK8_eta,_boostJetAK8_phi,_boostJetAK8_mass;

Float_t _boostJetAK8Puppi_softDrop_massCorr[10];
Float_t _boostJetAK8Puppi_softDrop_massBare[10];
Float_t _boostJetAK8_softDropMass = -9999;

Float_t _boostJetAK8Puppi_tau1[10];
Float_t _boostJetAK8Puppi_tau2[10];
Float_t _boostJetAK8Puppi_tau3[10];
Float_t _boostJetAK8Puppi_tau4[10];
Float_t _boostJetAK8Puppi_tau21_DDT[10];
Float_t _boostJetAK8_tau21 = -9999;

Float_t _boostJetAK8Puppi_s1BTag[10];
Float_t _boostJetAK8Puppi_s2BTag[10];

Float_t _boostJetAK8Puppi_btagBOOSTED[10];
Float_t _boostJetAK8_btagBOOSTED = -9999;

Float_t _mTfull = -9999;

// MC Only
Int_t   _nTrueInt;
Float_t _genWeight;
Int_t   _ngenZ;
Float_t _genZ_pt[10];
Float_t _pdf_x1;
Float_t _pdf_x2;
Int_t   _ngenQ;
Int_t   _genQ_pdgId[10];
Float_t _genZ_eta[10];
Float_t _genZ_phi[10];
Float_t _genZ_mass[10];
Int_t   _ngenLep;
Float_t _genLep_pt[10];
Float_t _genLep_eta[10];
Float_t _genLep_phi[10];
Int_t   _ngenNeu;
Float_t _genNeu_pt[10];
Float_t _genNeu_eta[10];
Float_t _genNeu_phi[10];

// tree_out only

// store old branches
Float_t _llnunu_mt_old;
Float_t _llnunu_l1_mass_old, _llnunu_l1_pt_old, _llnunu_l1_phi_old, _llnunu_l1_eta_old;
Float_t _llnunu_l2_pt_old, _llnunu_l2_phi_old;
Float_t _llnunu_l1_l1_pt_old, _llnunu_l1_l1_eta_old, _llnunu_l1_l1_phi_old, _llnunu_l1_l1_ptErr_old;
Float_t _llnunu_l1_l2_pt_old, _llnunu_l1_l2_eta_old, _llnunu_l1_l2_phi_old, _llnunu_l1_l2_ptErr_old;

// zpt gen weights
Float_t _ZPtWeight, _ZPtWeight_up, _ZPtWeight_dn;
Float_t _ZJetsGenWeight;

// sm qqZZ QCD and EW corrections
Float_t _ZZEwkCorrWeight, _ZZEwkCorrWeight_up, _ZZEwkCorrWeight_dn;
Float_t _ZZQcdCorrWeight, _ZZQcdCorrWeight_up, _ZZQcdCorrWeight_dn;

// efficiency scale factors
Float_t _trgsf, _isosf, _idsf, _trksf, _idisotrksf,_etrgsf,_mtrgsf;
Float_t _trgsf_err, _isosf_err, _idsf_err, _trksf_err,_etrgsf_err,_mtrgsf_err;
Float_t _trgsf_up, _trgsf_dn, _idisotrksf_up, _idisotrksf_dn,_etrgsf_up, _etrgsf_dn,_mtrgsf_up, _mtrgsf_dn;


// for GJets samples
Float_t _GJetsPhiWeight;
Float_t _GJetsTrigEff;
Float_t _GJetsRhoWeight;
Float_t _GJetsZPtWeight, _GJetsZPtWeightEl, _GJetsZPtWeightMu;
Float_t _GJetsZPtWeightLowLPt, _GJetsZPtWeightLowLPtEl, _GJetsZPtWeightLowLPtMu;
Float_t _GJetsZPtWeight_up, _GJetsZPtWeightEl_up, _GJetsZPtWeightMu_up;
Float_t _GJetsZPtWeight_dn, _GJetsZPtWeightEl_dn, _GJetsZPtWeightMu_dn;
Int_t   _PreScale22, _PreScale30, _PreScale36, _PreScale50, _PreScale75, _PreScale90, _PreScale120, _PreScale165;
Float_t _GJetsPreScaleWeight;
Float_t _gjet_mt, _gjet_l1_pt, _gjet_l1_eta, _gjet_l1_rapidity, _gjet_l1_phi;
Int_t   _gjet_l1_idCutBased;
Int_t   _gjet_l1_trigerob_HLTbit;
Float_t _gjet_l1_trigerob_pt, _gjet_l1_trigerob_eta, _gjet_l1_trigerob_phi;
Int_t   _llnunu_l1_trigerob_HLTbit;
Float_t _llnunu_l1_trigerob_pt, _llnunu_l1_trigerob_eta, _llnunu_l1_trigerob_phi;
Float_t _gjet_l2_pt, _gjet_l2_phi, _gjet_l2_sumEt, _gjet_l2_rawPt, _gjet_l2_rawPhi, _gjet_l2_rawSumEt;

Float_t _gjet_l2_t1Pt_JetEnUp,_gjet_l2_t1Phi_JetEnUp,_gjet_l2_t1Pt_JetEnDn,_gjet_l2_t1Phi_JetEnDn;
Float_t _gjet_l2_t1Pt_JetResUp,_gjet_l2_t1Phi_JetResUp,_gjet_l2_t1Pt_JetResDn,_gjet_l2_t1Phi_JetResDn;
Float_t _gjet_l2_t1Pt_MuonEnUp,_gjet_l2_t1Phi_MuonEnUp,_gjet_l2_t1Pt_MuonEnDn,_gjet_l2_t1Phi_MuonEnDn;
Float_t _gjet_l2_t1Pt_ElectronEnUp,_gjet_l2_t1Phi_ElectronEnUp,_gjet_l2_t1Pt_ElectronEnDn,_gjet_l2_t1Phi_ElectronEnDn;
Float_t _gjet_l2_t1Pt_TauEnUp,_gjet_l2_t1Phi_TauEnUp,_gjet_l2_t1Pt_TauEnDn,_gjet_l2_t1Phi_TauEnDn;
Float_t _gjet_l2_t1Pt_PhotonEnUp,_gjet_l2_t1Phi_PhotonEnUp,_gjet_l2_t1Pt_PhotonEnDn,_gjet_l2_t1Phi_PhotonEnDn;
Float_t _gjet_l2_t1Pt_UnclusterUp,_gjet_l2_t1Phi_UnclusterUp,_gjet_l2_t1Pt_UnclusterDn,_gjet_l2_t1Phi_UnclusterDn;

Float_t _gjet_l2_genPhi, _gjet_l2_genEta;
Float_t _llnunu_mt_el, _llnunu_l1_mass_el;
Float_t _llnunu_mt_mu, _llnunu_l1_mass_mu;
Float_t _llnunu_l2_pt_el, _llnunu_l2_pt_mu;
Float_t _llnunu_l2_phi_el, _llnunu_l2_phi_mu;

//======================================================
//  ╔╦╗╔═╗╔═╗╦╔╗╔╔═╗  ╔═╗╦ ╦╔╗╔╔═╗╔╦╗╦╔═╗╔╗╔╔═╗
//   ║║║╣ ╠╣ ║║║║║╣   ╠╣ ║ ║║║║║   ║ ║║ ║║║║╚═╗
//  ═╩╝╚═╝╚  ╩╝╚╝╚═╝  ╚  ╚═╝╝╚╝╚═╝ ╩ ╩╚═╝╝╚╝╚═╝
//======================================================

// read configration file
void readConfigFile();

// prepare the trees, if no entries in _tree_in, stop the program.
bool prepareTrees();

// store Old Branches
void storeOldBranches();

// prepare inputs for pu weights
void preparePUWeights();

// add more pileup weights
void addPUWeights();

// prepare inputs for muon re-calib
void prepareMuonPtRecalib();

// do muon re-calib
void doMuonPtRecalib();

// do elec re-calib, simple version
void doElecPtRecalibSimpleData();

// do muon re-calib, simple version
void doMuonPtRecalibSimpleData();

// prepare inputs for addDyZPtWeight
void prepareDyZPtWeight();

// addDyZPtWeight
void addDyZPtWeight();

// prepare for addZZCorrections
void prepareZZCorrections();

// addZCorrections
void addZZCorrections();

// prepare JEC/JER
void prepareJECJER();

// do JEC/JER
void doJECJER();

// prepare inputs for simple met recoil tune.
void prepareRecoil();

// do simple met recoil fine tuning
void doRecoil();

// prepare eff scale factors
void prepareEffScale();
    
void prepareEmuTrgsf();
// add eff scale factors
void addEffScale();

void addEmuTrgsf();

// prepare gjets skimming
void prepareGJetsSkim();

// do gjets skim
void doGJetsSkim();

// do boostJet selection
void doBoostJetSelection();

// do MT alternatives
float MTCalc(float pt, float phi);
float MTCalcEl(float pt, float phi);
float MTCalcMu(float pt, float phi);

void doMTUnc();
void doMTUncMu();
void doMTUncEl();

// 


//======================================================
//======================================================
//======================================================
//======================================================
//======================================================
//======================================================
//======================================================
//======================================================
//======================================================
//======================================================
//======================================================
//======================================================
//======================================================
//======================================================
//======================================================
//======================================================
//======================================================
//======================================================
//======================================================
//======================================================
//======================================================
//======================================================


