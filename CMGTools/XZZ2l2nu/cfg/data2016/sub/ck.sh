#!/bin/sh

dds="mc_phdynlo "

for nn in `seq 1 1 100`;
do 
  for dd in $dds;
  do
    ./check.sh $dd &> ck_${dd}.log &
  done
  #sleep 1800
  wait
done

