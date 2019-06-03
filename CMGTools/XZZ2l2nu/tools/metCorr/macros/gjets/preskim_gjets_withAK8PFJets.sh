#!/bin/sh
# compile
g++ preskim_gjets.cc -o preskim_gjets.exe `root-config --cflags` `root-config --libs`

#samples="DYJetsToLL_M50_Ext"
inputdir=/eos/cms/store/user/tocheng/X2l+MET+jets/Run2016/SinglePhoton
outputdir=/eos/cms/store/user/tocheng/X2l+MET+jets/Run2016/SinglePhotonWithAK8_Preskim

mkdir -p $outputdir

njob="0"

for infile in $(ls $inputdir/*.root);
do
  echo "+++ skimming $infile +++"
  outfile="${outputdir}/${infile/$inputdir\//}"

  echo "./preskim_gjets.exe $infile $outfile &> ${outfile}.log & "
  ./preskim_gjets.exe $infile $outfile &> ${outfile}.log &

  njob=$(( njob + 1 ))
  if [ "$njob" -eq "100" ]; then
    wait
    njob="0"
  fi

done
