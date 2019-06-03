#!/bin/sh



# compile
g++ preskimnonreso.cc -o preskimnonreso.exe `root-config --cflags` `root-config --libs`


./preskimnonreso.exe /home/heli/XZZ/80X_20170202_light_Skim/muonegtree_light_skim_38.root /home/heli/XZZ/80X_20170202_light_Skim/muonegtree_light_skim_38_skim.root 
