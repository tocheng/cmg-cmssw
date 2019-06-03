#! /usr/bin/env python
from ROOT import *
from DataFormats.FWLite import Events, Handle
import sys
from array import array
events=Events(['root://eoscms.cern.ch/'+i for i in open("List.txt").read().split("\n") if i.strip()])
handle_pu=Handle('std::vector<PileupSummaryInfo>')
handle_vtx=Handle('std::vector<reco::Vertex>')
fo=TFile("output.root","recreate")
t1=TTree('tree','tree')
runnum=array("L",[0])
lumnum=array("L",[0])
evtnum=array("L",[0])
ntruevtx=array("f",[0])
nvtx=array("f",[0])

t1.Branch("run",runnum,"run/l")
t1.Branch("lumi",lumnum,"run/l")
t1.Branch("evt",evtnum,"evt/l")
t1.Branch("nTrueInt",ntruevtx,"nTrueInt/F")
t1.Branch("nVert",nvtx,"nVert/F")

for event in events:
    #event.getByLabel('slimmedAddPileupInfo','','PAT',handle_pu)
    #event.getByLabel('offlineSlimmedPrimaryVertices','','PAT',handle_vtx)
    event.getByLabel('slimmedAddPileupInfo',handle_pu)
    event.getByLabel('offlineSlimmedPrimaryVertices', handle_vtx)
    runnum[0]=event.eventAuxiliary().id().run()
    lumnum[0]=event.eventAuxiliary().id().luminosityBlock()
    evtnum[0]=event.eventAuxiliary().id().event()
    ntruevtx[0]=handle_pu.product()[0].getTrueNumInteractions()
    nvtx[0]=len(handle_vtx.product())
    t1.Fill()
fo.cd()
t1.Write()
fo.Close()

