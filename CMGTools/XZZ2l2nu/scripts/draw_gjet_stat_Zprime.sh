#!/bin/sh

tag="GJets_"

channels="mu el"
cutChains="SR_massSB"

logdir="log_gjets_35p9"

mkdir -p $logdir

for cutChain in $cutChains;
do
   for channel in $channels;
   do
      python stack_dtmc_skim_Zprime.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --channel="$channel" --dyGJets --LogY &> ${logdir}/${tag}${cutChain}_log_${channel}_unblind_sys_plot.log &

   done
done

tag="DYJets_"

for cutChain in $cutChains;
do
   for channel in $channels;
   do
      python stack_dtmc_skim_Zprime.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --channel="$channel" --LogY &> ${logdir}/${tag}${cutChain}_log_${channel}_unblind_sys_plot.log &

   done
done

