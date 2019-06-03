
indir=$1
mX=$2

cp $1/mX${mX}* .

combineCards.py ee_SR=mX${mX}ZZ2l2nu_ee_SR.txt mm_SR=mX${mX}ZZ2l2nu_mm_SR.txt > mX${mX}ZZ2l2nu_SR.txt

combine -M MaxLikelihoodFit --saveNormalizations --saveShapes --saveWithUncertainties -m ${mX} mX${mX}ZZ2l2nu_SR.txt -t -1 --expectSignal 0
