#!/bin/sh


tag="Test_DataB2G27p1_ReRecoVsPrompt_"
#tag="Test_Data36p46_VsReCalib_"
channels="mu el"
#cutChains="tight tightzpt100 tightzpt100met50"
#cutChains="tightzpt100 tightzpt100met50"
cutChains="tight"
#cutChains="tightzptlt200"
#logdir="log_cmp_data_36p46"
logdir="log_cmp_data_27p1"

mkdir -p $logdir

for cutChain in $cutChains;
do
   for channel in $channels;
   do
#      ./compare_data_skim.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --channel="$channel" --Blind --LogY &> ${logdir}/${tag}${cutChain}_bld_log_${channel}.log &
#      ./compare_data_skim.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --channel="$channel" --Blind &> ${logdir}/${tag}${cutChain}_bld_${channel}.log &
#      ./compare_data_skim.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --channel="$channel" --LogY &> ${logdir}/${tag}${cutChain}_log_${channel}.log &
#      ./compare_data_skim.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --channel="$channel" &> ${logdir}/${tag}${cutChain}_${channel}.log &
#      ./compare_data_skim.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --channel="$channel" --test &> ${logdir}/${tag}${cutChain}_${channel}.log &
#      ./compare_data_skim.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --channel="$channel" --LogY  --test &> ${logdir}/${tag}${cutChain}_log_${channel}.log &
      #./compare_data_skim.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --channel="$channel" --LogY --Blind  --test &> ${logdir}/${tag}${cutChain}_bld_log_${channel}.log &
      ./compare_data_skim.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --channel="$channel" --ThreeLepVeto --LogY  --test &> ${logdir}/${tag}${cutChain}_lVeto_log_${channel}.log &

   done
done
