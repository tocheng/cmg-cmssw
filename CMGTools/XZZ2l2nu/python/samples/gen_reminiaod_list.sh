#!/bin/sh

list="
/SingleElectron/Run2016B-03Feb2017_ver1-v1/MINIAOD
/SingleElectron/Run2016B-03Feb2017_ver2-v2/MINIAOD
/SingleElectron/Run2016C-03Feb2017-v1/MINIAOD
/SingleElectron/Run2016D-03Feb2017-v1/MINIAOD
/SingleElectron/Run2016E-03Feb2017-v1/MINIAOD
/SingleElectron/Run2016F-03Feb2017-v1/MINIAOD
/SingleElectron/Run2016G-03Feb2017-v1/MINIAOD
/SingleElectron/Run2016H-03Feb2017_ver2-v1/MINIAOD
/SingleElectron/Run2016H-03Feb2017_ver3-v1/MINIAOD

/SingleMuon/Run2016B-03Feb2017_ver2-v2/MINIAOD
/SingleMuon/Run2016B-03Feb2017_ver1-v1/MINIAOD
/SingleMuon/Run2016C-03Feb2017-v1/MINIAOD
/SingleMuon/Run2016D-03Feb2017-v1/MINIAOD
/SingleMuon/Run2016E-03Feb2017-v1/MINIAOD
/SingleMuon/Run2016F-03Feb2017-v1/MINIAOD
/SingleMuon/Run2016G-03Feb2017-v1/MINIAOD
/SingleMuon/Run2016H-03Feb2017_ver2-v1/MINIAOD
/SingleMuon/Run2016H-03Feb2017_ver3-v1/MINIAOD

/SinglePhoton/Run2016B-03Feb2017_ver1-v1/MINIAOD
/SinglePhoton/Run2016B-03Feb2017_ver2-v2/MINIAOD
/SinglePhoton/Run2016C-03Feb2017-v1/MINIAOD
/SinglePhoton/Run2016D-03Feb2017-v1/MINIAOD
/SinglePhoton/Run2016E-03Feb2017-v1/MINIAOD
/SinglePhoton/Run2016F-03Feb2017-v1/MINIAOD
/SinglePhoton/Run2016G-03Feb2017-v1/MINIAOD
/SinglePhoton/Run2016H-03Feb2017_ver2-v1/MINIAOD
/SinglePhoton/Run2016H-03Feb2017_ver3-v1/MINIAOD


/MuonEG/Run2016B-03Feb2017_ver1-v1/MINIAOD
/MuonEG/Run2016B-03Feb2017_ver2-v2/MINIAOD
/MuonEG/Run2016C-03Feb2017-v1/MINIAOD
/MuonEG/Run2016D-03Feb2017-v1/MINIAOD
/MuonEG/Run2016E-03Feb2017-v1/MINIAOD
/MuonEG/Run2016F-03Feb2017-v1/MINIAOD
/MuonEG/Run2016G-03Feb2017-v1/MINIAOD
/MuonEG/Run2016H-03Feb2017_ver2-v1/MINIAOD
/MuonEG/Run2016H-03Feb2017_ver3-v1/MINIAOD

"

for dd in $list;
do
  tt=`echo $dd | sed "s/\/Run2016/_Run2016/g"`
  tt=`echo $tt | sed "s/-v1\/MINIAOD//g"`
  tt=`echo $tt | sed "s/-v2\/MINIAOD//g"`
  tt=`echo $tt | sed "s/-v3\/MINIAOD//g"`
  tt=`echo $tt | sed "s/-/_/g"`
  tt=`echo $tt | sed "s/\///g"`

#  echo "$tt = kreator.makeDataComponent(\"$tt\", \"$dd\", \"CMS\", \".*root\", json, useAAA=False,jsonFilter=False)"
  echo "$tt,"
done

