#!/bin/sh

./storehist_fullmc.exe  config/TRG80/ell1pteta
./macros/geteff.py  ell1pteta_36p46recalib.root  ell1pteta 

#./storehist_fullmc.exe  config/TRG80/ell1pt
#./macros/geteff.py  ell1pt_36p46.root  ell1pt

#./storehist_fullmc.exe  config/TRG80/ell1pt_recalib
#./macros/geteff.py  ell1pt_36p46recalib.root  ell1pt


