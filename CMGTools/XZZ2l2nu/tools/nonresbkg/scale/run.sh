#! /bin/bash

#cuts='0 5 10 15 20 25 30 35 40 45 50 55 60'
cuts='0 10 20 30 40 50 60 70 80 90'
echo -e "MET&Zpt cut\t\t   electron\t   \t       muon">log
for acut in $cuts
do
    python electronploting.py $acut &
    python muonploting.py $acut &
    wait
    esc=$(python getscale elconfig)
    msc=$(python getscale muconfig)
    echo -e "$acut GeV   \t$esc\t   $msc" >>log
done