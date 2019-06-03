#!/bin/sh


tag="Test1_ReRecoB2G_ICHEPcfg_"
#tag="Test1_DataB2G_ICHEPcfg_"
#tag="DataB2G_ICHEPcfg_"
#channels="all mu el"
channels="mu el"
#cutChains="tight tightzpt100 tightzpt100met50"
#cutChains="tightzpt100 tightzpt100met50"
cutChains="tight"
#blind="--Blind"
#logy="--LogY"
#Test=""


#./stack_dtmc_skim.py -l -b -q  --tag="Test_DataB2G_ICHEPcfg_" --cutChain="tight" --channel="mu" --Blind --LogY --test


for cutChain in $cutChains;
do
   for channel in $channels;
   do
      #./stack_dtmc_skim_b2g.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --channel="$channel" --Blind --LogY &> log_b2g/${tag}${cutChain}_bld_log_${channel}.log &
      #./stack_dtmc_skim_b2g.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --channel="$channel" --Blind &> log_b2g/${tag}${cutChain}_bld_${channel}.log &
      #./stack_dtmc_skim_b2g.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --channel="$channel" --LogY &> log_b2g/${tag}${cutChain}_log_${channel}.log &
      #./stack_dtmc_skim_b2g.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --channel="$channel" &> log_b2g/${tag}${cutChain}_${channel}.log &
      ./stack_dtmc_skim_b2g.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --channel="$channel" --test --LogY &> log_b2g/${tag}${cutChain}_log_${channel}.log &

   done
done
