#!/bin/sh


tag="test_wj2_PhSummer16_"
#tag="PhSummer16_"
#cutChains="loosecut"
#cutChains="tight"
#cutChains="SR CR CR1 CR2 CR3"
#cutChains="tightzptgt50lt100 tightzptgt100"
cutChains="tight tightzptgt50lt100 tightzptgt100"
logdir="log_ph"

mkdir -p $logdir


for cutChain in $cutChains;
do
#   ./stack_dtmc_skim_photon.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --SeparateProcess --WtQCDToGJets --LogY  &> ${logdir}/${tag}${cutChain}_SepProc_WtQCDToGJets_log.log &

#   ./stack_dtmc_skim_photon.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --LogY  &> ${logdir}/${tag}${cutChain}_log.log &
#   ./stack_dtmc_skim_photon.py -l -b -q  --tag="$tag" --cutChain="$cutChain" &> ${logdir}/${tag}${cutChain}.log &
#   ./stack_dtmc_skim_photon.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --ZWeight --channel="mu" --LogY   &> ${logdir}/${tag}${cutChain}_ZWt_mu_log.log &
#   ./stack_dtmc_skim_photon.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --ZWeight --channel="mu"  &> ${logdir}/${tag}${cutChain}_ZWt_mu.log &
#   ./stack_dtmc_skim_photon.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --ZWeight --channel="el" --LogY   &> ${logdir}/${tag}${cutChain}_ZWt_el_log.log &
#   ./stack_dtmc_skim_photon.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --ZWeight --channel="el"  &> ${logdir}/${tag}${cutChain}_ZWt_el.log &


#   ./stack_dtmc_skim_photon.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --SeparateProcess --LogY  &> ${logdir}/${tag}${cutChain}_SepProc_log.log &
#   ./stack_dtmc_skim_photon.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --SeparateProcess --ZWeight --channel="mu" --LogY   &> ${logdir}/${tag}${cutChain}_SepProc_ZWt_mu_log.log &
#   ./stack_dtmc_skim_photon.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --SeparateProcess --ZWeight --channel="el" --LogY   &> ${logdir}/${tag}${cutChain}_SepProc_ZWt_el_log.log &

#   ./stack_dtmc_skim_photon.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --SeparateProcess  &> ${logdir}/${tag}${cutChain}_SepProc.log &
#   ./stack_dtmc_skim_photon.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --SeparateProcess --ZWeight --channel="mu"  &> ${logdir}/${tag}${cutChain}_SepProc_ZWt_mu.log &
#   ./stack_dtmc_skim_photon.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --SeparateProcess --ZWeight --channel="el"  &> ${logdir}/${tag}${cutChain}_SepProc_ZWt_el.log &

# for test
   ./stack_dtmc_skim_photon.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --SeparateProcess --LogY  --test &> ${logdir}/${tag}${cutChain}_SepProc_log.log &
#   ./stack_dtmc_skim_photon.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --SeparateProcess --test &> ${logdir}/${tag}${cutChain}_SepProc.log &
#   ./stack_dtmc_skim_photon.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --SeparateProcess --ZWeight --channel="mu" --LogY  --test &> ${logdir}/${tag}${cutChain}_SepProc_ZWt_mu_log.log &
#   ./stack_dtmc_skim_photon.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --SeparateProcess --ZWeight --channel="mu" --test &> ${logdir}/${tag}${cutChain}_SepProc_ZWt_mu.log &
#   ./stack_dtmc_skim_photon.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --SeparateProcess --ZWeight --channel="el" --LogY  --test &> ${logdir}/${tag}${cutChain}_SepProc_ZWt_el_log.log &
#   ./stack_dtmc_skim_photon.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --SeparateProcess --ZWeight --channel="el" --test &> ${logdir}/${tag}${cutChain}_SepProc_ZWt_el.log &

done
