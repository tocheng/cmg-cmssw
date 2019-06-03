#!/bin/sh


# compile
g++ addtrigbit.cc -o addtrigbit.exe `root-config --cflags` `root-config --libs`


infile=/home/heli/XZZ/80X_20160810_light/SinglePhoton_Run2016BCD_PromptReco/vvTreeProducer/tree.root
outfile=/home/heli/XZZ/80X_20160810_light/SinglePhoton_Run2016BCD_PromptReco/vvTreeProducer/tree_moretrigbit.root
trigfile=/datac/heli/XZZ/80X_20160916/SinglePhoton_Run2016BCDEFG_PromptReco/vvTreeProducer/tree.root

./addtrigbit.exe ${infile} ${trigfile} ${outfile}


#alias eos='/afs/cern.ch/project/eos/installation/0.3.84-aquamarine/bin/eos.select'

#inputs
#inputdir=/home/heli/XZZ/80X_20160810
#outputdir=/home/heli/XZZ/80X_20160818_light_Skim_EffSkim

#mkdir -p ${outputdir}
#eos mkdir -p ${outputdir}

#njob="0"

#for infile in $(ls $inputdir/*.root|grep -v 2016B|grep -v DYJetsToLL|grep -v BulkGrav);
#for infile in $(eos ls $inputdir/*.root | grep root |grep  Run2016 | grep killdup );
#for infile in $(ls $inputdir/*.root|grep -v Run2016);
#for infile in $(ls $inputdir/*.root | grep root |grep  Run2016 | grep killdup );
#for infile in $(ls $inputdir/*.root | grep root |grep  Run2016BCD );
#for infile in $inputdir/DYJetsToLL_M50_MGMLM_Ext1.root;
#for infile in $inputdir/DYJetsToLL_M50.root;
#for infile in $inputdir/DYJetsToLL_M50_RecoilSmooth.root;
#for infile in $inputdir/DYJetsToLL_M50_Recoil*.root;
#for infile in  $inputdir/*.root ;
#for infile in $inputdir/DYJetsToLL_M50_MGMLM_Ext1_RecoilSmooth.root;
#for infile in $inputdir/DYJetsToLL_M50_MGMLM_Ext1_RecoilGraph.root;
#for infile in $inputdir/DYJetsToLL_M50_MGMLM_Ext1_RecoilNoSmooth.root;
#for infile in $inputdir/DYJetsToLL_M50_RecoilGraph.root;
#for infile in $inputdir/SingleEMU_Run2016BCD_PromptReco.root;
#for infile in $(ls $inputdir/*.root|grep -v Single | grep -v DYJets);
#for infile in $inputdir/DYJetsToLL_*_RecoilGraph.root;
#do
#  echo "+++ skimming $infile +++"
#  outfile="${outputdir}/${infile/$inputdir\//}"
#  outfile="${outfile/\/vvTreeProducer\/tree/}"

#  #infile="root://eoscms/"${inputdir}/${infile} 
#  #outfile="root://eoscms/"${outfile}

#  echo -- Command: ./skimming.exe $infile $outfile 

  
#  ./skimming.exe $infile $outfile > ${outfile}.log &
#
#  njob=$(( njob + 1 ))
#  if [ "$njob" -eq "100" ]; then
#    wait
#    njob="0"
#  fi
#done

