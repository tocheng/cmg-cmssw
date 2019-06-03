#!/bin/sh

#inputs
inputdir=/home/heli/XZZ/80X_20170202_light
outputdir=/home/heli/XZZ/80X_20170202_light_Skim
config=config/parameters_light

mkdir -p ${outputdir}

gmake all

njob="0"

#for infile in $inputdir/*/vvTreeProducer/tree.root ; 
#for infile in $(ls $inputdir/Single*/vvTreeProducer/tree.root ); 
#for infile in $inputdir/DYJetsToLL_M50_MGMLM/vvTreeProducer/tree.root ; 
#for infile in $inputdir/DYJetsToLL_M50/vvTreeProducer/tree.root ; 
#for infile in $inputdir/SingleEMU_*/vvTreeProducer/tree.root ; 
#for infile in $inputdir/DYJetsToLL*/vvTreeProducer/tree.root ; 
#for infile in $inputdir/SingleEMU_Run2016B2H29fbinv_*/vvTreeProducer/tree.root ; 
#for infile in $inputdir/DYJetsToLL_M50_reHLT/vvTreeProducer/tree.root ; 
#for infile in $inputdir/Bulk*/vvTreeProducer/tree.root ; 
#for infile in $(ls $inputdir/*/vvTreeProducer/tree.root | grep -v Single | grep -v DYJets  ); 
#for infile in $(ls $inputdir/*/vvTreeProducer/tree.root | grep -v Single | grep -v DYJets | grep -v Bulk); 
#for infile in $inputdir/BulkGravToZZToZlepZinv_narrow_1000/vvTreeProducer/tree.root ;
#for infile in $inputdir/*/vvTreeProducer/tree.root ;
#for infile in $(ls $inputdir/*/vvTreeProducer/tree.root | grep Single ); 
#for infile in $(ls $inputdir/*/vvTreeProducer/tree.root | grep -v Single | grep DY | grep -v MGMLM ); 
#for infile in $(ls $inputdir/*/vvTreeProducer/tree.root | grep -v Single | grep -v DY); 
#for infile in $(ls $inputdir/*/vvTreeProducer/tree.root | grep  SingleEMU ); 
for infile in $(ls $inputdir/*/vvTreeProducer/tree.root | grep  DYJetsToLL_M50_Ext ); 
do
  echo "+++ skimming $infile +++"
  outfile="${outputdir}/${infile/$inputdir\//}"

  # options for outputs
  #outfile="${outfile/\/vvTreeProducer\/tree/_NoRecoil}"
  #outfile="${outfile/\/vvTreeProducer\/tree/_DtReCalib}"
  outfile="${outfile/\/vvTreeProducer\/tree/}"
  #outfile="${outfile/\/vvTreeProducer\/tree/_RcRhoWt}"
  #outfile="${outfile/\/vvTreeProducer\/tree/_test}"

  inSkimFile=${infile/vvTreeProducer\/tree.root/skimAnalyzerCount\/SkimReport.txt}

  #echo $inSkimFile
  AllEvents=`grep "All Events" ${inSkimFile} | awk {'print $3'}`
  SumWeights=`grep "Sum Weights" ${inSkimFile} | awk {'print $3'}`

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
  if [ "$njob" -eq "200" ]; then
   # wait
    njob="0"
  fi

done

