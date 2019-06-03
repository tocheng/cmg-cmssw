#!/bin/sh

# compile
set -e
g++ skimming.cc -o skimming.exe `root-config --cflags` `root-config --libs`

#inputs
infile=/afs/cern.ch/work/y/yanchu/graviton/CMSSW_8_0_22/src/CMGTools/XZZ2l2nu/cfg/NonResBG_dt/lumi36/all.root
outfile=/afs/cern.ch/work/y/yanchu/graviton/CMSSW_8_0_22/src/CMGTools/XZZ2l2nu/cfg/NonResBG_dt/lumi36/alltrg.root

echo -- Command: ./skimming.exe $infile $outfile 
  
./skimming.exe $infile $outfile

