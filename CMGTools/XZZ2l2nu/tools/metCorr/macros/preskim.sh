#!/bin/sh



# compile
g++ preskim.cc -o preskim.exe `root-config --cflags` `root-config --libs`

#inputs
indir=/data2/XZZ2/80X_20170202
outdir=/home/heli/XZZ/80X_20170202_light
#samples="QCD_Pt*_MuEnriched*"
#samples="QCD_Pt*_EMEnriched"
#samples="WJetsToLNu_HT*"
#samples="DYJetsToLL*"
#samples="DY?JetsToLL*"
#samples="Graviton*"
#samples="QCD_Pt*"
#samples="VBF_HToZ*"
#samples="DYJetsToLL_M50_Ext"
#samples="GluGluHTo*"
#samples="*"
samples="SingleMuon*_03Feb2017*"
#samples="SingleElectron*_03Feb2017*"
#samples="SingleEMU_Run2016B2H_ReReco_36p1fbinv"
#samples="SingleEMU_Run2016H_PromptReco_new"
#samples="Single*_Run2016B_23Sep2016_v2_resub"
#samples="SingleEMU_Run2016B2G_PromptReco"
#samples="SingleEMU_Run2016B2G_ReReco_27fbinv"
#samples="ggZZ*"
#samples="ZZ*"
#samples="WW*"
#samples="WZ*"
#samples="ZZTo2L2Nu"
#samples="TT*"
#samples="WJets*"
#samples="Bulk*"
#samples="DYJetsToLL_M50_BIG"
#samples="DYJetsToLL_M50_BIG_JEC"
#samples="WWToLNuQQ_BIG"
#samples="SingleEMU_Run2016B2H_ReReco_33fbinv"
#samples="SingleEMU_Run2016B2H_ReReco_36p1fbinv"
#samples="SingleEMU_Run2016H_PromptReco_new"
#indir=/data2/XZZ2/80X_20161018
#outdir=/home/heli/XZZ/80X_20161018_light
#samples="SingleEMU_Run2016B2H29fbinv_PromptReco"
#indir=/data2/XZZ2/80X_20161006
#outdir=/home/heli/XZZ/80X_20161006_light
#samples="SingleEMU_Run2016B2G_PromptReco"
#indir=/home/heli/XZZ/80X_20160825
#outdir=/home/heli/XZZ/80X_20160825_light
#indir=/data2/XZZ2/80X_20160905
#outdir=/data2/XZZ2/80X_20160905_light
#samples="DYJetsToLL_M50"
#samples="DYJetsToLL_M50_reHLT"
#samples="DYJetsToLL_M50_BIG"
mkdir -p $outdir

#for dd in ${indir}/*/vvTreeProducer;
#for dd in ${indir}/Single*/vvTreeProducer;
#for dd in ${indir}/SingleEMU_Run2016B2G_PromptReco/vvTreeProducer;
for dd in ${indir}/${samples}/vvTreeProducer;
do 
  infile="${dd}/tree.root";
  oo="${dd/$indir/$outdir}";
  outfile="${oo}/tree_light.root";
  echo mkdir -p $oo;
  mkdir -p $oo ;
  echo "./preskim.exe $infile $outfile &> ${outfile}.log & "
  ./preskim.exe $infile $outfile &> ${outfile}.log &
done


# wait and replace old one

wait

#for dd in  ${indir}/* ;
#for dd in ${indir}/SingleEMU_Run2016B2G_PromptReco ;
for dd in ${indir}/${samples} ;
do
  echo $dd;
  ddo=${dd/$indir/$outdir}
  echo "mkdir -p ${ddo} " 
  mkdir -p ${ddo}

  for dd2 in ${dd}/* ;
  do
    if [[ ${dd2} != *"vvTree"* ]]; then
      #echo $dd2;
      #ddo2="${dd2/$indir/$outdir} "
      #echo "cp -rp $dd2 $ddo2 " 
      #cp -rp $dd2 $ddo2
      echo "cp -rp $dd2 $ddo/ " 
      cp -rp $dd2 $ddo/
    fi
  done;

  ttin="${ddo/_light/}/vvTreeProducer"

#  echo "cp -rp $ttin $ddo/"
#  cp -rp $ttin $ddo/
  mkdir -p $ddo/vvTreeProducer
  rm -f $ddo/vvTreeProducer/tree.root
  echo "mv $ddo/vvTreeProducer/tree_light.root $ddo/vvTreeProducer/tree.root"
  mv $ddo/vvTreeProducer/tree_light.root $ddo/vvTreeProducer/tree.root
done

