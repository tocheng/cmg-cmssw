#!/bin/sh


tag="GJets_"

channels="mu"
cutChains="SR CR1"

logdir="log_gjets_36p81"

mkdir -p $logdir

for cutChain in $cutChains;
do
   for channel in $channels;
   do
      python stack_dtmc_skim_gjets_stat.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --channel="$channel" --dyGJets --LogY --muoneg --doSys &> ${logdir}/${tag}${cutChain}_log_${channel}_unblind_sys_plot.log &

   done
done
