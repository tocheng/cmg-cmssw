

#for mH in 600 800 1000 1200 1400 1600 1800 2000
  
   cd $1

   for mH in 600

   do  

   echo ${mH}
   echo 'ee+mumu vv '
 
   combineCards.py ee_SR=mX${mH}ZZ2l2nu_ee_SR.txt mm_SR=mX${mH}ZZ2l2nu_mm_SR.txt > mX${mH}ZZ2l2nu_SR.txt

   combine -m ${mH} -M Asymptotic mX${mH}ZZ2l2nu_SR.txt --run blind --rMax 4 --noFitAsimov

   #combine -n ee_SR -m ${mH} -M Asymptotic mX${mH}ZZ2l2nu_ee_SR.txt --run blind --rMax 4 --noFitAsimov
   #combine -n mm_SR -m ${mH} -M Asymptotic mX${mH}ZZ2l2nu_mm_SR.txt --run blind --rMax 4 --noFitAsimov

   done

   cd -
