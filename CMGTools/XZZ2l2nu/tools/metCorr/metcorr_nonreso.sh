#!/bin/sh

#inputs
inputdir=/home/heli/XZZ/80X_20161029_light
outputdir=/home/heli/XZZ/80X_20161029_light_Skim
config=config/parameters_light_nonres

mkdir -p ${outputdir}

gmake all

njob="0"

infile="$inputdir/muonegtree_light.root"

outfile="${outputdir}/${infile/$inputdir\//}"

# options for outputs
outfile="${outfile/light.root/light_skim.root}"

echo $config $infile $outfile

./bin/metcorr.exe $config $infile $outfile 1000 1000 &> ${outfile}.skim.log &


