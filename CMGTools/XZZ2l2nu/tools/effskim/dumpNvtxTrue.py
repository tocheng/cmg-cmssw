#! /usr/bin/env python
from ROOT import *
from DataFormats.FWLite import Events, Handle
import sys
from array import array
events=Events(['root://eoscms.cern.ch/'+i for i in open("List.txt").read().split("\n") if i.strip()])
psi=Handle('vector<PileupSummaryInfo>')
fo=TFile("output.root","recreate")
t1=TTree('tree','tree')
runnum=array("L",[0])
lumnum=array("L",[0])
evtnum=array("L",[0])
vtcnum=array("d",[0])

t1.Branch("run",runnum,"run/l")
t1.Branch("lumi",lumnum,"run/l")
t1.Branch("evt",evtnum,"evt/l")
t1.Branch("ntrueVertices",vtcnum,"ntrueVertices/D")

for event in events:
    event.getByLabel('slimmedAddPileupInfo','','PAT',psi)
    runnum[0]=event.eventAuxiliary().id().run()
    lumnum[0]=event.eventAuxiliary().id().luminosityBlock()
    evtnum[0]=event.eventAuxiliary().id().event()
    vtcnum[0]=psi.product()[0].getTrueNumInteractions()
    t1.Fill()
fo.cd()
t1.Write()
fo.Close()

