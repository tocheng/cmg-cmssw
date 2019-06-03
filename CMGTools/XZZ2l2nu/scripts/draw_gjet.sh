#!/bin/sh


tag="GJets_"

channels="mu el"
cutChains="SR CR1"
logdir="log_gjets_36p46"

mkdir -p $logdir

for cutChain in $cutChains;
do
   for channel in $channels;
   do
      python stack_dtmc_skim_gjets_dt.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --channel="$channel" --dyGJets --LogY --muoneg --test &> ${logdir}/${tag}${cutChain}_log_${channel}_unblind_test_plot.log &

  done
done
