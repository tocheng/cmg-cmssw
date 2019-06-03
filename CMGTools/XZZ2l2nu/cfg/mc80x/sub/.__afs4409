#!/bin/sh

outdir=$1
config=$2
otherfiles=" pogRecipesMC.py "

heppy_batch.py -o ${outdir} ${config} -r /store/user/tocheng/X2l+MET+jets/Run2016/${outdir} -b 'bsub -q sssss < ./batchScript.sh' -n
for dd in ${outdir}/* ; 
do cp $otherfiles ${dd}/;
done

cd $outdir
heppy_check.py * -b "bsub -q 1nw"
cd ../
