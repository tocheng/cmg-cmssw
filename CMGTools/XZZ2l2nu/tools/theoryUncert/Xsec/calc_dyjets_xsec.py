#!/bin/env python



#dy1jets_lo.txt:After filter: final cross section = 1.013e+03 +- 7.822e-01 pb
#dy2jets_lo.txt:After filter: final cross section = 3.347e+02 +- 2.643e-01 pb
#dy3jets_lo.txt:After filter: final cross section = 1.024e+02 +- 1.362e-01 pb
#dy4jets_lo.txt:After filter: final cross section = 5.445e+01 +- 4.216e-02 pb
#dyjets_lo.txt:After filter: final cross section = 4.959e+03 +- 2.627e+00 pb

lo_njets = 4.959e+03
lo_1jets = 1.013e+03
lo_2jets = 3.347e+02
lo_3jets = 1.024e+02
lo_4jets = 5.445e+01

lo_0jets = lo_njets-lo_1jets-lo_2jets-lo_3jets-lo_4jets


print "lo_njets=",lo_njets
print "lo_0jets=",lo_0jets
print "lo_1jets=",lo_1jets
print "lo_2jets=",lo_2jets
print "lo_3jets=",lo_3jets
print "lo_4jets=",lo_4jets


# print results:
#njets= 4959.0
#0jets= 3454.45
#1jets= 1013.0
#2jets= 334.7
#3jets= 102.4
#4jets= 54.45 

nlo_njets=1921.8*3

r_nlo_lo_njets = nlo_njets/lo_njets

print "r_nlo_lo_njets=",r_nlo_lo_njets

nlo_0jets=lo_0jets*r_nlo_lo_njets
nlo_1jets=lo_1jets*r_nlo_lo_njets
nlo_2jets=lo_2jets*r_nlo_lo_njets
nlo_3jets=lo_3jets*r_nlo_lo_njets
nlo_4jets=lo_4jets*r_nlo_lo_njets

print "nlo_njets=",nlo_njets
print "nlo_0jets=",nlo_0jets
print "nlo_1jets=",nlo_1jets
print "nlo_2jets=",nlo_2jets
print "nlo_3jets=",nlo_3jets
print "nlo_4jets=",nlo_4jets

#nlo_njets= 5765.4
#nlo_0jets= 4016.1899637
#nlo_1jets= 1177.72740472
#nlo_2jets= 389.126715064
#nlo_3jets= 119.051615245
#nlo_4jets= 63.3043012704
