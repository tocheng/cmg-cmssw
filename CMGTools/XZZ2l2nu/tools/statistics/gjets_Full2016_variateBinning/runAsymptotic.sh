

   dir=$1
   mH=$2
  
   cd $1

   combineCards.py ee_SR=mX${mH}ZZ2l2nu_ee_SR.txt mm_SR=mX${mH}ZZ2l2nu_mm_SR.txt > mX${mH}ZZ2l2nu_SR.txt

   combine -m ${mH} -M Asymptotic mX${mH}ZZ2l2nu_SR.txt --run blind --rMax 10 --noFitAsimov

   combine -n ee_SR -m ${mH} -M Asymptotic mX${mH}ZZ2l2nu_ee_SR.txt --run blind --rMax 10 --noFitAsimov
   combine -n mm_SR -m ${mH} -M Asymptotic mX${mH}ZZ2l2nu_mm_SR.txt --run blind --rMax 10 --noFitAsimov

   cd -
