#!/bin/sh

channels="mu"
#cutChains="tightzpt100 SR_massSB SR_massSR" #SRdPhiGT0p5"
cutChains="tightzpt100 zllSR_massSB zllSR_massSR zllSR_massSR_btag zllSR_massSR_antibtag"

logdir="log_gjets_35p9_debug"

mkdir -p $logdir

tag="GJets_"

for cutChain in $cutChains;
do
   for channel in $channels;
   do
      python stack_dtmc_skim_Zprime_Debug.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --dyGJets --doBoost --doTau21 --channel="$channel" --LogY &> ${logdir}/${tag}${cutChain}BoostAK8_log_${channel}_debug.log &

   done
done


tag="DYJets_"

for cutChain in $cutChains;
do
   for channel in $channels;
   do
      python stack_dtmc_skim_Zprime_Debug.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --doBoost --doTau21 --channel="$channel" --LogY &> ${logdir}/${tag}${cutChain}BoostAK8_log_${channel}_debug.log &

   done
done
