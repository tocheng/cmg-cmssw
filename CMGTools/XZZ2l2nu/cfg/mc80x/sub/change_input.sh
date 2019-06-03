#!/bin/sh

#dir=gjetsmc3
dir=othermc2
#dir=dt3

#tag="ZJetsToNuNu_HT200to400_Chunk*"
#tag="*_Run2016D*"
tag="*_*"
#tag="TTZToLLNuNu*"
#tag="TTWJetsToLNu*"
#tag="WZTo3LNu*"
#tag="MuonEG_"
#tag="SingleMuon_Run2016H_PromptReco_v2"
#tag="SinglePhoton_Run2016C_23Sep2016"
#tag="Single"
#tag="SingleElectron_"
#tag="SingleMuon_"
#tag="MET_"
from="eoscms.cern.ch\/\/eos\/cms"
#to="eoscms.cern.ch\/\/eos\/cms"
#to="cms-xrd-global.cern.ch\/"
#from="cms-xrd-global.cern.ch"
to="cmsxrootd.fnal.gov"
#to="xrootd-cms.infn.it"
#from="xrootd-cms.infn.it"
#from="cmsxrootd.fnal.gov"
#to="cms-xrd-global.cern.ch"

cd $dir

for dd in ${tag}*; 
do
  cd $dd
  sed "s/${from}/${to}/g" config.pck > config.pck.new 
  mv config.pck.new config.pck
  cd ..
done

cd ..
