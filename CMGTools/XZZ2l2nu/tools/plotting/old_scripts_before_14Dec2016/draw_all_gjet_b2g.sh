#!/bin/sh


tag="GJets_ICHEPcfg_"
channels="all mu el"
#cutChains="tightzpt50 tightzpt100 tightzpt100met50"
cutChains="tightzpt100 tightzpt100met50"
#cutChains="tightzpt50"
#blind="--Blind"
#logy="--LogY"
#Test=""


#./stack_dtmc_skim.py -l -b -q  --tag="Test_DataB2G_ICHEPcfg_" --cutChain="tight" --channel="mu" --Blind --LogY --test


for cutChain in $cutChains;
do
   for channel in $channels;
   do
      ./stack_dtmc_skim_gjets_dt.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --channel="$channel" --Blind --LogY &> log_b2g/${tag}${cutChain}_bld_log_${channel}.log &
      #./stack_dtmc_skim_gjets_dt.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --channel="$channel" --Blind &> log_b2g/${tag}${cutChain}_bld_${channel}.log &
      ./stack_dtmc_skim_gjets_dt.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --channel="$channel" --LogY &> log_b2g/${tag}${cutChain}_log_${channel}.log &
      #./stack_dtmc_skim_gjets_dt.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --channel="$channel" &> log_b2g/${tag}${cutChain}_${channel}.log &

   done
done
