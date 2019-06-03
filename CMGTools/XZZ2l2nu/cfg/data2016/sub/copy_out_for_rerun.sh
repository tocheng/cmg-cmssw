#!/bin/sh


file=dynlo_list.txt
indir=/data2/XZZ2/80X_20170202_Chunks/mc_zjnlo
indir2=mc_dynlo2
outdir=mc_dynlo3

mkdir -p $outdir

for ff in `cat $file`; 
do 

  dd=`grep  "$ff" ${indir}/*Chunk*/config.pck ` ;
  if [ "$dd" == "" ]; then
     echo $dd; 
     dd1=`grep "$ff" ${indir2}/*Chunk*/config.pck | sed 's/\/config.pck.*//g'`
     if [ "$dd1" != "" ]; then
       echo $dd1 
       mv  $dd1 $outdir/
     fi
  fi     

#  mv $dd $outdir/

done 
