#!/bin/sh

tag="test_ReMini_MC_"
#tag="test_PhNewDY_NewDJ_"
#tag="test_NoWZ2l2q_NewDJ_"
#tag="test_NoWZ3l1nu_NewDJ_"
#tag="test_ZZ2l2nu0p5_NewDJ_"
#tag="test_NoZZ2l2q_NewDJ_"
#tag="test_NoZZ4l_NewDJ_"
#tag="test_NoggZZ_NewDJ_"
#tag="test_NoTTZ_NewDJ_"
#tag="test_NewDJ_"
#tag="test_NewDJNoMETSub_"
#tag="test29_DtRecalib_"
#tag="trigStudyHighLepPt_"
#tag="GJetsSumm16JEC_badMuonFilter_"
#tag="MCSummer16JECNLOZJETHighLepPtCutAllSig_"
#tag="MCSummer16JECHighLepPt_"
#tag="test_"

channels="mu el"
#channels="el"
#channels="mu"
#cutChains="tight"
#cutChains="tightzpt50"
#cutChains="tightzptgt100lt400"
#cutChains="tightzptgt50lt200"
#cutChains="tightzptgt55metgt125"
#cutChains="SR"
cutChains="SR tightzpt50"
#cutChains="CR1 CR2 CR3"
#cutChains="CR1"
#cutChains="CR2 CR3"
#cutChains="tightzpt50 tightzpt100 tightzpt100met50"
#cutChains="tightzpt50 tightzpt100"
#cutChains="tightzpt100 tightzpt100met50"
#cutChains="tightzpt100met50"
#cutChains="tightzpt150 tightzpt100"
#cutChains="tightzpt150 tightzpt200 tightzpt100met50"
#cutChains="tightzpt200"
#cutChains="tightzpt100"
#cutChains="tightzpt50"
#cutChains="tight"
#cutChains="tight tightzpt50 tightzpt100"
#cutChains="nonreso_zptgt70"
#cutChains="nonreso_zptgt50"
#cutChains="nonreso_zptgt0 nonreso_zptgt10 nonreso_zptgt20 nonreso_zptgt30 nonreso_zptgt40"
#cutChains="nonreso_zptgt50_metlt20 nonreso_zptgt50_metlt30 nonreso_zptgt50_metlt50 "

#cutChains="nonreso_zptgt50 nonreso_zptgt50_metlt100 tightzpt50"
#cutChains="nonreso_zptgt70 tightzpt50"
#cutChains="nonreso_zptgt50_metlt100 tightzpt50"
#cutChains="tightzptgt50lt200"
logdir="log"

mkdir -p $logdir

for cutChain in $cutChains;
do
   for channel in $channels;
   do  
#      ./stack_dtmc_skim_gjets_dt.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --channel="$channel" --Blind --LogY &> ${logdir}/${tag}${cutChain}_mc_bld_log_${channel}.log &
#      ./stack_dtmc_skim_gjets_dt.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --channel="$channel"  --LogY &> ${logdir}/${tag}${cutChain}_mc_log_${channel}.log &
#      ./stack_dtmc_skim_gjets_dt.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --channel="$channel" --dyGJets --muoneg --Blind --LogY &> ${logdir}/${tag}${cutChain}_bld_log_${channel}.log &
#      ./stack_dtmc_skim_gjets_dt.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --channel="$channel" --dyGJets --muoneg --LogY &> ${logdir}/${tag}${cutChain}_log_${channel}.log &

#      ./stack_dtmc_skim_gjets_dt.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --channel="$channel" --dyGJets --Blind --LogY &> ${logdir}/${tag}${cutChain}_bld_log_${channel}.log &
#      ./stack_dtmc_skim_gjets_dt.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --channel="$channel" --dyGJets --LogY &> ${logdir}/${tag}${cutChain}_log_${channel}.log &
      #./stack_dtmc_skim_gjets_dt.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --channel="$channel" --dyGJets --Blind &> ${logdir}/${tag}${cutChain}_bld_${channel}.log &
      #./stack_dtmc_skim_gjets_dt.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --channel="$channel" --dyGJets &> ${logdir}/${tag}${cutChain}_${channel}.log &
#      ./stack_dtmc_skim_gjets_dt.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --channel="$channel" --dyGJets --test &> ${logdir}/${tag}${cutChain}_${channel}.log &
#      ./stack_dtmc_skim_gjets_dt.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --channel="$channel" --dyGJets --LogY  --test &> ${logdir}/${tag}${cutChain}_log_${channel}.log &
#      ./stack_dtmc_skim_gjets_dt.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --channel="$channel" --dyGJets --Blind --test &> ${logdir}/${tag}${cutChain}_bld_${channel}.log &
#      ./stack_dtmc_skim_gjets_dt.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --channel="$channel" --dyGJets --Blind --LogY  --test &> ${logdir}/${tag}${cutChain}_bld_log_${channel}.log &
#      ./stack_dtmc_skim_gjets_dt.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --channel="$channel" --dyGJets --muoneg --test &> ${logdir}/${tag}${cutChain}_${channel}.log &
#      ./stack_dtmc_skim_gjets_dt.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --channel="$channel" --dyGJets --muoneg --LogY  --test &> ${logdir}/${tag}${cutChain}_log_${channel}.log &

      ./stack_dtmc_skim_gjets_dt.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --channel="$channel"  --LogY --test &> ${logdir}/${tag}${cutChain}_log_${channel}.log &
#       ./stack_dtmc_skim_gjets_dt.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --channel="$channel" --muoneg  --LogY --test &> ${logdir}/${tag}${cutChain}_emu_log_${channel}.log &
#       ./stack_dtmc_skim_gjets_dt.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --channel="$channel" --dyGJets  --muoneg --LogY --test &> ${logdir}/${tag}${cutChain}_emu_gjet_log_${channel}.log &
#       ./stack_dtmc_skim_gjets_dt.py -l -b -q  --tag="$tag" --cutChain="$cutChain" --channel="$channel" --Blind --dyGJets  --muoneg --LogY --test &> ${logdir}/${tag}${cutChain}_emu_gjet_bld_log_${channel}.log &

   done
done
