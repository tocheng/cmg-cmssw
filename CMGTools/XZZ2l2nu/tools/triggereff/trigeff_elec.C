{

  TFile* fin= new TFile("/home/heli/XZZ/80X_20160825_light_Skim/SingleEMU_Run2016BCD_PromptReco.root");
  std::string sel_tight = "(abs(llnunu_l1_l1_pdgId)==11&&abs(llnunu_l1_l2_pdgId)==11&&llnunu_l1_l1_pt>115&&abs(llnunu_l1_l1_eta)<2.5&&llnunu_l1_l2_pt>35&&abs(llnunu_l1_l2_eta)<2.5&&llnunu_l1_mass>70&&llnunu_l1_mass<110)";
  std::string sel_loose = "(abs(llnunu_l1_l1_pdgId)==11&&abs(llnunu_l1_l2_pdgId)==11&&llnunu_l1_l1_pt>35&&abs(llnunu_l1_l1_eta)<2.5&&llnunu_l1_l2_pt>35&&abs(llnunu_l1_l2_eta)<2.5&&llnunu_l1_mass>70&&llnunu_l1_mass<110)";
  TTree* tree = (TTree*)fin->Get("tree");

  std::string sel_passHLT = "(HLT_ELEv2)";

  std::string sel_Ele1Pass = "((llnunu_l1_l1_trigerob_HLTbit&3)!=0)";
  std::string sel_Ele1NotPassEle2Pass = "((llnunu_l1_l1_trigerob_HLTbit&3)==0&&(llnunu_l1_l2_trigerob_HLTbit&3)!=0)";

  std::string sel_pass_tight = "("+sel_tight+"&&"+sel_passHLT+")";
  std::string sel_base_tight = sel_tight;

  std::string sel_pass_tight_ele1pass = "("+sel_tight+"&&"+sel_Ele1Pass+")";
  std::string sel_pass_tight_ele1notpassele2pass = "("+sel_tight+"&&"+sel_Ele1NotPassEle2Pass+")";


  double N_pass_tight = (double)tree->GetEntries(sel_pass_tight.c_str());
  double N_base_tight = (double)tree->GetEntries(sel_base_tight.c_str());



  double N_pass_tight_ele1pass = (double)tree->GetEntries(sel_pass_tight_ele1pass.c_str());
  double N_pass_tight_ele1notpassele2pass = (double)tree->GetEntries(sel_pass_tight_ele1notpassele2pass.c_str());

  std::cout << " N_pass_tight = " << N_pass_tight << "; N_base_tight = " << N_base_tight << ";  eff = " << N_pass_tight/N_base_tight << std::endl;
  std::cout << " N_pass_tight_ele1pass = " << N_pass_tight_ele1pass << "; N_base_tight = " << N_base_tight << ";  eff = " << N_pass_tight_ele1pass/N_base_tight << std::endl;
  std::cout << " N_pass_tight_ele1notpassele2pass = " << N_pass_tight_ele1notpassele2pass << "; N_base_tight = " << N_base_tight << ";  eff = " << N_pass_tight_ele1notpassele2pass/N_base_tight << std::endl;



/*
  std::string sel_pass_loose = "("+sel_loose+"&&"+sel_passHLT+")";
  std::string sel_base_loose = sel_loose;


  double N_pass_loose = (double)tree->GetEntries(sel_pass_loose.c_str());
  double N_base_loose = (double)tree->GetEntries(sel_base_loose.c_str());

  std::cout << " N_pass_loose = " << N_pass_loose << "; N_base_loose = " << N_base_loose << ";  eff = " << N_pass_loose/N_base_loose << std::endl
*/
}
