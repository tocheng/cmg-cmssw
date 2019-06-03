#!/bin/env python

# import ROOT in batch mode
import ROOT

# load FWLite C++ libraries
ROOT.gSystem.Load("libFWCoreFWLite.so");
ROOT.gSystem.Load("libDataFormatsFWLite.so");
ROOT.FWLiteEnabler.enable()

# load FWlite python libraries
from DataFormats.FWLite import Handle, Events

triggers_photon_idiso = ["HLT_Photon%d_R9Id90_HE10_IsoM_v*" % pt for pt in (22,30,36,50,75,90,120,165) ]
trigVec = ROOT.vector(ROOT.string)()
for t in triggers_photon_idiso: trigVec.push_back(t)
mainFilter = ROOT.heppy.TriggerBitChecker(trigVec)

halo_tight = ["Flag_globalSuperTightHalo2016Filter"]
halo_2015 = ["Flag_CSCTightHalo2015Filter"]
halo_clean = ["HLT_PFMET170_HBHECleaned_v*", "HLT_PFMET170_BeamHaloCleaned_v*", "HLT_PFMET170_HBHE_BeamHaloCleaned_v*", "HLT_PFMETTypeOne190_HBHE_BeamHaloCleaned_v*"]
halo_tight_vec = ROOT.vector(ROOT.string)()
halo_2015_vec = ROOT.vector(ROOT.string)()
halo_clean_vec = ROOT.vector(ROOT.string)()
for t in halo_tight: halo_tight_vec.push_back(t)
for t in halo_2015: halo_2015_vec.push_back(t)
for t in halo_clean: halo_clean_vec.push_back(t)
halo_tight_filter = ROOT.heppy.TriggerBitChecker(halo_tight_vec)
halo_2015_filter = ROOT.heppy.TriggerBitChecker(halo_2015_vec)
halo_clean_filter = ROOT.heppy.TriggerBitChecker(halo_clean_vec)


photons, photonLabel = Handle("std::vector<pat::Photon>"), "slimmedPhotons"
#edm::SortedCollection<EcalRecHit,edm::StrictWeakOrdering<EcalRecHit> >    "reducedEgamma"             "reducedEBRecHits"   "RECO"
ebhits, ebhitsLabel = Handle("edm::SortedCollection<EcalRecHit,edm::StrictWeakOrdering<EcalRecHit> >"), "reducedEBRecHits"
eehits, eehitsLabel = Handle("edm::SortedCollection<EcalRecHit,edm::StrictWeakOrdering<EcalRecHit> >"), "reducedEERecHits"
eshits, eshitsLabel = Handle("edm::SortedCollection<EcalRecHit,edm::StrictWeakOrdering<EcalRecHit> >"), "reducedESRecHits"

triggerBits, triggerBitLabel = Handle("edm::TriggerResults"), ("TriggerResults","","HLT")
triggerObjects, triggerObjectLabel  = Handle("std::vector<pat::TriggerObjectStandAlone>"), "selectedPatTrigger"
triggerPrescales, triggerPrescaleLabel  = Handle("pat::PackedTriggerPrescales"), "patTrigger"

metFilters, metFiltersLabel = Handle("edm::TriggerResults"), ("TriggerResults","","RECO")

#events = Events("afile.root")
events = Events("pickevents.root")
#events = Events("root://eoscms//store/data/Run2016D/SinglePhoton/MINIAOD/PromptReco-v2/000/276/317/00000/585BECAF-1945-E611-8D75-02163E0119BF.root")

fout = ROOT.TFile.Open("dick_evt_out.root", "recreate")
#fout = ROOT.TFile.Open("nondick_evt_out.root", "recreate")
hSeedTime = ROOT.TH1D("hSeedTime", "hSeedTime", 5000,-25,25)
hSeedTime.Sumw2()
hSwissCross = ROOT.TH1D("hSwissCross", "hSwissCross", 1000,-2,1)
hSwissCross.Sumw2()
hMipTotE = ROOT.TH1D("hMipTotE", "hMipTotE", 1000,0,100)
hMipTotE.Sumw2()
hSigmaIetaIeta = ROOT.TH1D("hSigmaIetaIeta", "hSigmaIetaIeta", 10000,0,0.1)
hSigmaIphiIphi = ROOT.TH1D("hSigmaIphiIphi", "hSigmaIphiIphi", 10000,0,0.1)
hSigmaIetaIeta.Sumw2()
hSigmaIphiIphi.Sumw2()

N_tot = 0
N_tot_pt170 = 0
N_halo_tight_filter = 0
N_halo_2015_filter = 0
N_halo_clean_filter = 0

for iev,event in enumerate(events):
    event.getByLabel(photonLabel, photons)
    event.getByLabel("reducedEgamma","reducedEBRecHits", ebhits)
    event.getByLabel("reducedEgamma","reducedEERecHits", eehits)
    event.getByLabel("reducedEgamma","reducedESRecHits", eshits)

    event.getByLabel(triggerBitLabel, triggerBits)
    event.getByLabel(triggerObjectLabel, triggerObjects)
    event.getByLabel(triggerPrescaleLabel, triggerPrescales)
    event.getByLabel(metFiltersLabel, metFilters)

    #names = event.object().triggerNames(triggerBits.product())
    #for i in xrange(triggerBits.product().size()):
    #    print "Trigger ", names.triggerName(i), ", prescale ", triggerPrescales.product().getPrescaleForIndex(i), ": ", ("PASS" if triggerBits.product().accept(i) else "fail (or not run)") 

    filterNames = event.object().triggerNames(metFilters.product())
    for i in xrange(metFilters.product().size()):
         print "metFilter:",filterNames.triggerName(i)

    if not mainFilter.check(event.object(), triggerBits.product()):
        #print 'skip event'
        continue

    N_tot += 1

    #print " ievt = ", iev
    #print " halo_tight_filter = ", halo_tight_filter.check(event.object(), metFilters.product())
    #print " halo_2015_filter = ", halo_2015_filter.check(event.object(), metFilters.product())
    #print " halo_clean_filter = ", halo_clean_filter.check(event.object(), triggerBits.product())
    if halo_tight_filter.check(event.object(), metFilters.product()):
        N_halo_tight_filter += 1
    if halo_2015_filter.check(event.object(), metFilters.product()):
        N_halo_2015_filter += 1
    if halo_clean_filter.check(event.object(), triggerBits.product()):
        N_halo_clean_filter += 1
    
    # Photon
    for i,pho in enumerate(photons.product()):
        #print "pho.superCluster().energy(): ",pho.superCluster().energy()
        #print "pho.seed().seed(): ",pho.seed().seed()
        #print "pho.seed().energy(): ",pho.seed().energy()
        #print "pho.superCluster().seed().energy(): ",pho.superCluster().seed().energy()
        #print "pho.superCluster().seed().hitsAndFractions().size(): ",pho.superCluster().seed().hitsAndFractions().size()
        #print "pho.superCluster().seed().printHitAndFraction(): ",pho.superCluster().seed().printHitAndFraction(0)
        #print "pho.pt(): ",pho.pt()
        #print "pho.recHits().size(): ",pho.recHits().size()
        #print "pho.recHits()[0].time(): ",pho.recHits()[0].time()

        if i>0: break
        if pho.pt()>170: N_tot_pt170 += 1

        seedId = pho.seed().seed()
        seedhit = ROOT.EcalRecHit()
        for did in ebhits.product():
            if did.id()==seedId: 
                #print "eb: seed.time():", did.time()
                seedhit = did 
        for did in eehits.product():
            if did.id()==seedId: 
                #print "ee: seed.time():", did.time()
                seedhit = did 
        for did in eshits.product():
            if did.id()==seedId: 
                #print "es: seed.time():", did.time()
                seedhit = did 

        hSeedTime.Fill(seedhit.time())
        print "seed.time():",seedhit.time()
        print "seed.energy(),pho.eMax():",seedhit.energy(),",",pho.eMax()

        #print "seed1.time():",pho.recHits().find(seedId).time()

        E4 = pho.eLeft()+pho.eRight()+pho.eTop()+pho.eBottom()
        E1 = pho.eMax()
        S = 1.0-E4/E1
        #print 'S=(1-E4/E1) : ', S
        hSwissCross.Fill(S)

        # mip energy
        mipTotE = pho.mipTotEnergy()
        #print "pho.mipTotEnergy():",pho.mipTotEnergy()
        hMipTotE.Fill(mipTotE)

        # shower shape
        sigmaIetaIeta = pho.showerShapeVariables().sigmaIetaIeta
        sigmaIphiIphi = pho.showerShapeVariables().sigmaIphiIphi
        #print "sigmaIetaIeta, sigmaIphiIphi = ",sigmaIetaIeta,",",sigmaIphiIphi
        hSigmaIetaIeta.Fill(sigmaIetaIeta)
        hSigmaIphiIphi.Fill(sigmaIphiIphi)

    
fout.cd()
hSeedTime.Write()
hSwissCross.Write()
hMipTotE.Write()
hSigmaIetaIeta.Write()
hSigmaIphiIphi.Write()

print "N_tot = ",N_tot
print "N_tot_pt170 = ",N_tot_pt170
print "N_halo_tight_filter = ",N_halo_tight_filter
print "N_halo_2015_filter = ",N_halo_2015_filter
print "N_halo_cleanfilter = ",N_halo_clean_filter
