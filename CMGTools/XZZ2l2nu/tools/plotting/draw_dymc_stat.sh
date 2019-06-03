#!/bin/sh


tag="Lumi36p46_DYRes_Rc36p46wHLT_"
#channels="mu"
channels="mu el"
cutChains="tight tightzpt100 tightzpt100met50"
logdir="log_36p46"

mkdir -p $logdir

for cutChain in $cutChains;
do
   for channel in $channels;
   do
      ./stack_dtmc_skim_gjets_dt.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --channel="$channel" --LogY  --test &> ${logdir}/${tag}${cutChain}_log_${channel}.log &
      ./stack_dtmc_skim_gjets_dt.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --channel="$channel" --LogY --Blind  --test &> ${logdir}/${tag}${cutChain}_bld_log_${channel}.log &

   done
done
