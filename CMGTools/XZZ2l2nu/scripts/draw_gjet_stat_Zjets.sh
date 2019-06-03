#!/bin/sh

tag="GJets_VS_ZJets_"

channels="el mu"
cutChains="tightzpt100"

logdir="log_gjets_35p9"

mkdir -p $logdir

for cutChain in $cutChains;
do
   for channel in $channels;
   do
      python stack_dtmc_skim_Zprime_zjetsVSgjets.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --channel="$channel" --muoneg --LogY &> ${logdir}/${tag}${cutChain}_log_${channel}_unblind_sys_plot.log &

   done
done
