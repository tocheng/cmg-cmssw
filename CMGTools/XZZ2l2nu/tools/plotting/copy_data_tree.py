#!/bin/env python

import ROOT



cutChain="tightzpt100met50"

infile=ROOT.TFile.Open("/home/heli/XZZ/80X_20160825_light_Skim/SingleEMU_Run2016BCD_PromptReco.root");
intree=infile.Get("tree");

outfile=ROOT.TFile.Open("data_tree_zpt100met50.root", "recreate");

elChannel='(abs(llnunu_l1_l1_pdgId)==11&&abs(llnunu_l1_l2_pdgId)==11)'
muChannel='(abs(llnunu_l1_l1_pdgId)==13&&abs(llnunu_l1_l2_pdgId)==13)'


metfilter='(Flag_EcalDeadCellTriggerPrimitiveFilter&&Flag_HBHENoiseIsoFilter&&Flag_goodVertices&&Flag_HBHENoiseFilter&&Flag_globalTightHalo2016Filter&&Flag_eeBadScFilter)'

cuts_hlt="(HLT_MUv2||HLT_ELEv2)"
cuts_loose='(nllnun)'
cuts_lepaccept="((abs(llnunu_l1_l1_pdgId)==13&&abs(llnunu_l1_l2_pdgId)==13&&llnunu_l1_l1_pt>50&&abs(llnunu_l1_l1_eta)<2.4&&llnunu_l1_l2_pt>20&&abs(llnunu_l1_l2_eta)<2.4&&(llnunu_l1_l1_highPtID>0.99||llnunu_l1_l2_highPtID>0.99))"
cuts_lepaccept+="||(abs(llnunu_l1_l1_pdgId)==11&&abs(llnunu_l1_l2_pdgId)==11&&llnunu_l1_l1_pt>115&&abs(llnunu_l1_l1_eta)<2.5&&llnunu_l1_l2_pt>35&&abs(llnunu_l1_l2_eta)<2.5))"
cuts_zmass="(llnunu_l1_mass>70&&llnunu_l1_mass<110)"
cuts_zpt100="(llnunu_l1_pt>100)"
cuts_met50="(llnunu_l2_pt>50)"
cuts_met100="(llnunu_l2_pt>100)"
cuts_met200="(llnunu_l2_pt>200)"
cuts_loose_z="("+cuts_hlt+"&&"+cuts_lepaccept+"&&"+cuts_zmass+")"
cuts_loose_zll="("+cuts_hlt+"&&"+cuts_lepaccept+"&&"+cuts_zmass+"&&"+cuts_zpt100+")"
cuts_loose_zll_met50="("+cuts_hlt+"&&"+cuts_lepaccept+"&&"+cuts_zmass+"&&"+cuts_zpt100+"&&"+cuts_met50+")"
cuts_loose_zll_met100="("+cuts_hlt+"&&"+cuts_lepaccept+"&&"+cuts_zmass+"&&"+cuts_zpt100+"&&"+cuts_met100+")"
cuts_loose_zll_met200="("+cuts_hlt+"&&"+cuts_lepaccept+"&&"+cuts_zmass+"&&"+cuts_zpt100+"&&"+cuts_met200+")"


if cutChain=='loosecut': cuts=cuts_loose
elif cutChain=='tight': cuts=cuts_loose_z
elif cutChain=='tightzpt100': cuts=cuts_loose_zll
elif cutChain=='tightzpt100met50': cuts=cuts_loose_zll_met50
elif cutChain=='tightzpt100met100': cuts=cuts_loose_zll_met100
elif cutChain=='tightzpt100met200': cuts=cuts_loose_zll_met200
else : cuts=cuts_loose


tmpfile=ROOT.TFile.Open("/tmp/tmp_data_tree_zpt100met50.root", "recreate");
tmptree_el = intree.CopyTree(cuts+'&&'+elChannel)
tmptree_el.SetName("tree_el");
tmptree_mu = intree.CopyTree(cuts+'&&'+muChannel)
tmptree_mu.SetName("tree_mu");

tmptree_el.SetBranchStatus("*",0);
tmptree_el.SetBranchStatus("llnunu_mt",1);
tmptree_el.SetBranchStatus("llnunu_l1_pt",1);
tmptree_el.SetBranchStatus("llnunu_l2_pt",1);
tmptree_mu.SetBranchStatus("*",0);
tmptree_mu.SetBranchStatus("llnunu_mt",1);
tmptree_mu.SetBranchStatus("llnunu_l1_pt",1);
tmptree_mu.SetBranchStatus("llnunu_l2_pt",1);

outfile.cd()
outtree_el = tmptree_el.CloneTree()
outtree_mu = tmptree_mu.CloneTree()
outtree_el.Write();
outtree_mu.Write();

