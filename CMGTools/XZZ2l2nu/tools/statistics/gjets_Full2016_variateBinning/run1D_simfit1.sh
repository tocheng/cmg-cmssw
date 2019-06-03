

for mH in 600 800 1000 1200 1400 1600 1800 2000

   do  

   cd $1
  
   echo ${mH}
   echo 'ee+mumu vv '
 
   combineCards.py ee_CR1=mX${mH}ZZ2l2nu_ee_CR1.txt ee_SR=mX${mH}ZZ2l2nu_ee_SR.txt mm_CR1=mX${mH}ZZ2l2nu_mm_CR1.txt mm_SR=mX${mH}ZZ2l2nu_mm_SR.txt > mX${mH}ZZ2l2nu_simfit1.txt

   combine -n simfit1 -m ${mH} -M Asymptotic mX${mH}ZZ2l2nu_simfit1.txt --run blind --rMax 3 --noFitAsimov

   cd -

   echo '   '

   done


