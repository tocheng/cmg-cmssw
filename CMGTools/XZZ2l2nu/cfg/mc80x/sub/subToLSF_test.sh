#!/bin/sh

#export X509_USER_PROXY=/afs/cern.ch/work/t/tocheng/Heppy2017/x509up_u5185

outdir=data_batch_onLSF_test
config=run_xzz2l2nu_92x_cfg_loose_dt_test.py
otherfiles=" pogRecipesData92X.py "

heppy_batch.py -o ${outdir} ${config}  -b 'bsub -q sssss < ./batchScript.sh' -n
for dd in ${outdir}/* ; 
do cp $otherfiles ${dd}/;
done

cd $outdir
heppy_check.py * -b "bsub -q 8nh"

cd ../

