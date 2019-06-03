#!/bin/sh



# compile
g++ select0jet.cc -o select0jet.exe `root-config --cflags` `root-config --libs`

input=/home/heli/XZZ/80X_20170202_light/DYJetsToLL_M50_MGMLM_Ext1/vvTreeProducer/tree.root
output=/home/heli/XZZ/80X_20170202_light/DY0JetsToLL_M50_MGMLM/vvTreeProducer/tree.root

./select0jet.exe $input $output
