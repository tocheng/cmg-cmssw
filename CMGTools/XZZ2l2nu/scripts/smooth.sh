#!/bin/sh



# compile
g++ smooth.cc -o smooth.exe `root-config --cflags` `root-config --libs`

#
#for file in *.root; 
#for file in *_tightzpt100met50*.root; 
for file in *_antitightzpt100met50_CR1_*.root; 
do
   #./smooth.exe $file Smooth/$file SR  &> Smooth/$file.log &
   ./smooth.exe $file Smooth/$file CR1  &> Smooth/$file.log &
done
