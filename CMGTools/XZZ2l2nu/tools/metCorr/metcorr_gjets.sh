#!/bin/sh

#inputs
inputdir=/eos/cms/store/user/tocheng/X2l+MET+jets/Run2016/SinglePhoton/
outputdir=/eos/user/t/tocheng/X2l+MET+jets/Run2016/Analysis/GJetsPreskim
config=config/parameters_light_gjets

mkdir -p ${outputdir}

gmake all

njob="0"

for infile in $(ls $inputdir/*.root | grep SinglePhoton ); 
do
  echo "+++ skimming $infile +++"
  outfile="${outputdir}/${infile/$inputdir\//}"
  #outfile="${outfile/\/vvTreeProducer\/tree/}"

  #inSkimFile=${infile%.root}
  #inSkimFile=${inSkimFile}/skimAnalyzerCount/SkimReport.txt
  #echo "inSkimFile $inSkimFile"

  #AllEvents=`grep "All Events" ${inSkimFile} | awk {'print $3'}`
  #SumWeights=`grep "Sum Weights" ${inSkimFile} | awk {'print $3'}`

  #if [ -z $AllEvents ]; then
    #echo Fail to get All Events from file ${inSkimFile}
    #continue
  #fi
  #if [ -z $SumWeights ]; then
    #SumWeights=$AllEvents
  #fi

  AllEvents=1
  SumWeights=1

  echo -- Input file: $infile
  echo -- Output file: $outfile
  echo -- AllEvents: $AllEvents , SumWeights: $SumWeights
  echo -- Selection: $selection
  echo -- Command: ./bin/metcorr.exe $config $infile $outfile $AllEvents $SumWeights

  ./bin/metcorr.exe $config $infile $outfile $AllEvents $SumWeights &> ${outfile}.skim.log &

  #njob=$(( njob + 1 ))
  #if [ "$njob" -eq "200" ]; then
   ## wait
   # njob="0"
  #fi

done

