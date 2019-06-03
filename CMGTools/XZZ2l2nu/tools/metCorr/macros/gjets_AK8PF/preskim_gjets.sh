#!/bin/sh



# compile
g++ preskim_gjets.cc -o preskim_gjets.exe `root-config --cflags` `root-config --libs`


#samples="T*"
samples="SinglePhoton_Run2016Full_03Feb2017_v0"
#samples="DYJetsToLL_M50_Ext"
indir=/data2/XZZ2/80X_20170202_GJets
#outdir=/home/heli/XZZ/80X_20170202_GJets_light
outdir=/home/heli/XZZ/80X_20170202_GJets_light_hlt
#outdir=/home/heli/XZZ/80X_20170202_GJets_light_halo15

mkdir -p $outdir

njob="0"

#for dd in ${indir}/*/vvTreeProducer;
#for dd in ${indir}/SinglePhoton_Run2016BCD_PromptReco/vvTreeProducer;
#for dd in ${indir}/SinglePhoton_Run2016B2G_PromptReco/vvTreeProducer;
#for dd in ${indir}/SinglePhoton_Run2016B2H29fbinv_PromptReco/vvTreeProducer;
for dd in ${indir}/${samples}/vvTreeProducer;
do 
  infile="${dd}/tree.root";
  oo="${dd/$indir/$outdir}";
  outfile="${oo}/tree_light.root";
  echo mkdir -p $oo;
  mkdir -p $oo ;
  echo "./preskim_gjets.exe $infile $outfile &> ${outfile}.log & "
  ./preskim_gjets.exe $infile $outfile &> ${outfile}.log &

  njob=$(( njob + 1 ))
  if [ "$njob" -eq "100" ]; then
    wait
    njob="0"
  fi

done



# wait and replace old one

wait

for dd in  ${indir}/${samples} ;
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
  echo "mv $ddo/vvTreeProducer/tree_light.root $ddo/vvTreeProducer/tree.root"
  rm $ddo/vvTreeProducer/tree.root
  mv $ddo/vvTreeProducer/tree_light.root $ddo/vvTreeProducer/tree.root
done
