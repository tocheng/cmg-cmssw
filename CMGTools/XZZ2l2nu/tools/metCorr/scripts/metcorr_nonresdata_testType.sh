#!/bin/sh

#inputs
inputdir=/datac/tocheng/XZZ2/
outputdir=/datab/tocheng/XZZ/80X_20161029_light_Skim
config=config/parameters_light_testType

mkdir -p ${outputdir}

gmake all

njob="0"

for infile in $inputdir/muonegtrgsf.root ; 
do
  echo "+++ skimming $infile +++"
  outfile="${outputdir}/${infile/$inputdir\/}"

  # options for outputs
  inSkimFile=${infile/skimAnalyzerCount\/SkimReport.txt}

  #echo $inSkimFile
  AllEvents=1  #`grep "All Events" ${inSkimFile} | awk {'print $3'}`
  SumWeights=1 #`grep "Sum Weights" ${inSkimFile} | awk {'print $3'}`

  if [ -z $AllEvents ]; then
    echo Fail to get All Events from file ${inSkimFile}
    continue
  fi
  if [ -z $SumWeights ]; then
    SumWeights=$AllEvents
  fi

  echo -- Input file: $infile
  echo -- Output file: $outfile
  echo -- AllEvents: $AllEvents , SumWeights: $SumWeights
  echo -- Selection: $selection
  echo -- Command: ./bin/metcorr.exe $config $infile $outfile $AllEvents $SumWeights

  ./bin/metcorr.exe $config $infile $outfile $AllEvents $SumWeights &> ${outfile}.skim.log &

  njob=$(( njob + 1 ))
  if [ "$njob" -eq "100" ]; then
   # wait
    njob="0"
  fi

done

