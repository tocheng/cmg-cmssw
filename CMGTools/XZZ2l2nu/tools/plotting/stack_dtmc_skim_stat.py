#!/usr/bin/env python

import optparse
import ROOT
import os,sys, string, math, pickle
from CMGTools.XZZ2l2nu.plotting.TreePlotter import TreePlotter
from CMGTools.XZZ2l2nu.plotting.MergedPlotter import MergedPlotter
from CMGTools.XZZ2l2nu.plotting.StackPlotter import StackPlotter

grootargs = []
def callback_rootargs(option, opt, value, parser):
    grootargs.append(opt)

parser = optparse.OptionParser()
parser.add_option("-t","--tag",dest="tag",default='DataB2G_ICHEPcfg_',help="")
parser.add_option("--channel",dest="channel",default='mu',help="")
parser.add_option("--cutChain",dest="cutChain",default='tight',help="")
parser.add_option("--LogY",action="store_true", dest="LogY", default=False, help="")
parser.add_option("--Blind",action="store_true", dest="Blind", default=False,help="")
parser.add_option("--test",action="store_true", dest="test", default=False,help="")
parser.add_option("-l",action="callback",callback=callback_rootargs)
parser.add_option("-q",action="callback",callback=callback_rootargs)
parser.add_option("-b",action="callback",callback=callback_rootargs)


(options,args) = parser.parse_args()


tag=options.tag
cutChain=options.cutChain

# can be el or mu or both
channel=options.channel
LogY=options.LogY
test=options.test
DrawLeptons=True
doRhoScale=True
doVtxScale=False
doSys=False

lepsf="trgsf*idsf*isosf"
ZPtWeight="ZPtWeight"

if test: DrawLeptons = False

if doRhoScale: 
    tag+="RhoWt_"
    lepsf=lepsf+"*(0.32+0.42*TMath::Erf((rho-4.16)/4.58)+0.31*TMath::Erf((rho+115.00)/29.58))" # b2h rereco 36.1 fb-1

if doVtxScale:
    tag+="VtxWt_"
    lepsf=lepsf+"*(0.807+0.007*nVert+-3.689e-05*nVert*nVert+6.730e-04*exp(2.500e-01*nVert))" # b2h rereco 33.59fb-1

outdir='plots_36p46'

indir='/datab/tocheng/XZZ/80X_20161029_light_Skim'
lumi=36.46
sepSig=True
doRatio=True
#Blind=True
Blind=options.Blind
FakeData=False
UseMETFilter=True
SignalAll1pb=True
puWeight='puWeightmoriondMC'
#puWeight='puWeight68075'
#puWeight='puWeight'
ZJetsZPtWeight=True
DataHLT=True
k=1 # signal scale
UseRhoCut=False

elChannel='(abs(llnunu_l1_l1_pdgId)==11&&abs(llnunu_l1_l2_pdgId)==11)'
muChannel='(abs(llnunu_l1_l1_pdgId)==13&&abs(llnunu_l1_l2_pdgId)==13)'

rhoCut='(rho<22)'

if not os.path.exists(outdir): os.system('mkdir -p '+outdir)

tag = tag+cutChain+'_'
tag = tag+puWeight+'_'

if doSys: tag = tag+"sys_"

if UseRhoCut: tag=tag+'rhoCut_'

if UseMETFilter: tag = tag+'metfilter_'

if not Blind: tag = tag+'unblind_'

tag = tag+channel+'_'
if LogY: tag = tag+'log_'
if SignalAll1pb:
    tag += '1pb'
else:
    tag += 'scale'+str(k)


paveText="#sqrt{s} = 13 TeV 2016 L = "+"{:.3}".format(float(lumi))+" fb^{-1}"

metfilter='(Flag_EcalDeadCellTriggerPrimitiveFilter&&Flag_HBHENoiseIsoFilter&&Flag_goodVertices&&Flag_HBHENoiseFilter&&Flag_globalTightHalo2016Filter&&Flag_eeBadScFilter)'

cuts_loose='(nllnunu)'
cuts_lepaccept="((abs(llnunu_l1_l1_pdgId)==13&&abs(llnunu_l1_l2_pdgId)==13&&llnunu_l1_l1_pt>50&&abs(llnunu_l1_l1_eta)<2.4&&llnunu_l1_l2_pt>20&&abs(llnunu_l1_l2_eta)<2.4&&(llnunu_l1_l1_highPtID>0.99||llnunu_l1_l2_highPtID>0.99))"
cuts_lepaccept+="||(abs(llnunu_l1_l1_pdgId)==11&&abs(llnunu_l1_l2_pdgId)==11&&llnunu_l1_l1_pt>115&&abs(llnunu_l1_l1_eta)<2.5&&llnunu_l1_l2_pt>35&&abs(llnunu_l1_l2_eta)<2.5))"
cuts_zmass="(llnunu_l1_mass>70&&llnunu_l1_mass<110)"
cuts_zpt100="(llnunu_l1_pt>100)"
cuts_zptlt200="(llnunu_l1_pt<200)"
cuts_met50="(llnunu_l2_pt>50)"
cuts_met100="(llnunu_l2_pt>100)"
cuts_met200="(llnunu_l2_pt>200)"
cuts_loose_z="("+cuts_lepaccept+"&&"+cuts_zmass+")"
cuts_loose_zll="("+cuts_lepaccept+"&&"+cuts_zmass+"&&"+cuts_zpt100+")"
cuts_loose_zll_zptlt200="("+cuts_lepaccept+"&&"+cuts_zmass+"&&"+cuts_zptlt200+")"
cuts_loose_zll_met50="("+cuts_lepaccept+"&&"+cuts_zmass+"&&"+cuts_zpt100+"&&"+cuts_met50+")"
cuts_loose_zll_met100="("+cuts_lepaccept+"&&"+cuts_zmass+"&&"+cuts_zpt100+"&&"+cuts_met100+")"
cuts_loose_zll_met200="("+cuts_lepaccept+"&&"+cuts_zmass+"&&"+cuts_zpt100+"&&"+cuts_met200+")"


if cutChain=='loosecut': cuts=cuts_loose
elif cutChain=='tight': cuts=cuts_loose_z
elif cutChain=='tightzpt100': cuts=cuts_loose_zll
elif cutChain=='tightzpt100met50': cuts=cuts_loose_zll_met50
elif cutChain=='tightzpt100met100': cuts=cuts_loose_zll_met100
elif cutChain=='tightzpt100met200': cuts=cuts_loose_zll_met200
elif cutChain=='tightzptlt200': cuts=cuts_loose_zll_zptlt200
else : cuts=cuts_loose


if channel=='el': cuts = cuts+'&&'+elChannel
elif channel=='mu': cuts = cuts+'&&'+muChannel

if UseMETFilter:
    #cuts = '('+cuts+'&&'+metfilter+')'
    cuts = '('+cuts+')' # metfilter pre-applied in preskim


if UseRhoCut:
    cuts = cuts+'&&'+rhoCut

cuts = '('+cuts+')'


ROOT.gROOT.ProcessLine('.x tdrstyle.C') 

allPlotters = {}


wwPlotters=[]
wwSamples = ['WWTo2L2Nu','WWToLNuQQ_BIG','WZTo1L1Nu2Q']

for sample in wwSamples:
    wwPlotters.append(TreePlotter(sample, indir+'/'+sample+'.root','tree'))
    wwPlotters[-1].addCorrectionFactor('1./SumWeights','norm')
    wwPlotters[-1].addCorrectionFactor('xsec','xsec')
    wwPlotters[-1].addCorrectionFactor('genWeight','genWeight')
    wwPlotters[-1].addCorrectionFactor(puWeight,'puWeight')
    wwPlotters[-1].addCorrectionFactor(lepsf,'lepsf')
    allPlotters[sample] = wwPlotters[-1]

WW = MergedPlotter(wwPlotters)
WW.setFillProperties(1001,ROOT.kOrange)


vvPlotters=[]
vvSamples = ['WZTo2L2Q','WZTo3LNu_AMCNLO',
'ZZTo2L2Nu',
'ZZTo2L2Q','ZZTo4L',
'ggZZTo2e2nu','ggZZTo2mu2nu']
for sample in vvSamples:
    vvPlotters.append(TreePlotter(sample, indir+'/'+sample+'.root','tree'))
    vvPlotters[-1].addCorrectionFactor('1/SumWeights','norm')
    vvPlotters[-1].addCorrectionFactor('genWeight','genWeight')
    vvPlotters[-1].addCorrectionFactor(puWeight,'puWeight')
    vvPlotters[-1].addCorrectionFactor(lepsf, 'lepsf')
    if sample == 'ZZTo2L2Nu' : 
        vvPlotters[-1].addCorrectionFactor("(ZZEwkCorrWeight*ZZQcdCorrWeight*xsec)", 'xsec')
    if 'ggZZTo2' in sample: 
        vvPlotters[-1].addCorrectionFactor('0.01898','xsec') 
    else: 
        vvPlotters[-1].addCorrectionFactor('xsec','xsec')
    allPlotters[sample] = vvPlotters[-1]

VV = MergedPlotter(vvPlotters)
VV.setFillProperties(1001,ROOT.kMagenta)


wjetsPlotters=[]
wjetsSamples = ['WJetsToLNu']

for sample in wjetsSamples:
    wjetsPlotters.append(TreePlotter(sample, indir+'/'+sample+'.root','tree'))
    wjetsPlotters[-1].addCorrectionFactor('1./SumWeights','norm')
    wjetsPlotters[-1].addCorrectionFactor('xsec','xsec')
    wjetsPlotters[-1].addCorrectionFactor('genWeight','genWeight')
    wjetsPlotters[-1].addCorrectionFactor(puWeight,'puWeight')
    wjetsPlotters[-1].addCorrectionFactor(lepsf,'lepsf')

WJets = MergedPlotter(wjetsPlotters)
WJets.setFillProperties(1001,ROOT.kBlue-6)


zjetsPlotters=[]
#zjetsSamples = ['DYJetsToLL_M50_BIG_Rc36p22']
zjetsSamples = ['DYJetsToLL_M50_BIG_ResBos_Rc36p22']
#zjetsSamples = ['DYJetsToLL_M50_BIG_NoRecoil']



for sample in zjetsSamples:
    zjetsPlotters.append(TreePlotter(sample, indir+'/'+sample+'.root','tree'))
    zjetsPlotters[-1].addCorrectionFactor('1./SumWeights','norm')
    #zjetsPlotters[-1].addCorrectionFactor('(1)','norm')
    if ZJetsZPtWeight: zjetsPlotters[-1].addCorrectionFactor(ZPtWeight,'ZPtWeight')
    #zjetsPlotters[-1].addCorrectionFactor('xsec','xsec')
    zjetsPlotters[-1].addCorrectionFactor('(1921.8*3)','xsec') # FEWZ NNLO.results_z_m50_nnlo_inclusive_NNPDF30_nlo_as_0118
    #zjetsPlotters[-1].addCorrectionFactor('(1907.0*3)','xsec') # FEWZ NNLO.results_z_m50_nnlo_fsrOn_lowstat_inclusive_NNPDF30_nlo_as_0118
    zjetsPlotters[-1].addCorrectionFactor('genWeight','genWeight')
    #zjetsPlotters[-1].addCorrectionFactor("ZJetsGenWeight",'genWeight')
    zjetsPlotters[-1].addCorrectionFactor(puWeight,'puWeight')
    zjetsPlotters[-1].addCorrectionFactor(lepsf,'lepsf')
    if channel=='el' :
        zjetsPlotters[-1].addCorrectionFactor('(0.985054)','scale') #el ResBos
        #zjetsPlotters[-1].addCorrectionFactor('(1.06937)','scale') #el
        #zjetsPlotters[-1].addCorrectionFactor('(1)','scale') #el
    elif channel=='mu' :
        zjetsPlotters[-1].addCorrectionFactor('(1.11546)','scale') #mu ResBos
        #zjetsPlotters[-1].addCorrectionFactor('(1.12403)','scale') #mu
        #zjetsPlotters[-1].addCorrectionFactor('(1)','scale') #mu
    else :
        zjetsPlotters[-1].addCorrectionFactor('(1.11376)','scale') #all ResBos
        #zjetsPlotters[-1].addCorrectionFactor('(1.12337)','scale') #all
        #zjetsPlotters[-1].addCorrectionFactor('(1)','scale') #all
    allPlotters[sample] = zjetsPlotters[-1]



ZJets = MergedPlotter(zjetsPlotters)
ZJets.setFillProperties(1001,ROOT.kGreen+2)

ttPlotters=[]
ttSamples = ['TTTo2L2Nu','TTZToLLNuNu','TTWJetsToLNu']

for sample in ttSamples:
    ttPlotters.append(TreePlotter(sample, indir+'/'+sample+'.root','tree'))
    ttPlotters[-1].addCorrectionFactor('1./SumWeights','norm')
    ttPlotters[-1].addCorrectionFactor('xsec','xsec')
    ttPlotters[-1].addCorrectionFactor('genWeight','genWeight')
    ttPlotters[-1].addCorrectionFactor(puWeight,'puWeight')
    ttPlotters[-1].addCorrectionFactor(lepsf,'lepsf')
    allPlotters[sample] = ttPlotters[-1]

TT = MergedPlotter(ttPlotters)
TT.setFillProperties(1001,ROOT.kAzure-9)

sigPlotters=[]
sigSamples = [
'BulkGravToZZToZlepZinv_narrow_600',
#'BulkGravToZZToZlepZinv_narrow_800',
'BulkGravToZZToZlepZinv_narrow_1000',
#'BulkGravToZZToZlepZinv_narrow_1200',
#'BulkGravToZZToZlepZinv_narrow_1400',
#'BulkGravToZZToZlepZinv_narrow_1600', 
#'BulkGravToZZToZlepZinv_narrow_1800', 
'BulkGravToZZToZlepZinv_narrow_2000',
#'BulkGravToZZToZlepZinv_narrow_2500',
#'BulkGravToZZToZlepZinv_narrow_3000',
#'BulkGravToZZToZlepZinv_narrow_3500', 
#'BulkGravToZZToZlepZinv_narrow_4000', 
#'BulkGravToZZToZlepZinv_narrow_4500', 
]


sigSampleNames = {
'BulkGravToZZToZlepZinv_narrow_600':str(k)+' x BulkG-600',
'BulkGravToZZToZlepZinv_narrow_800':str(k)+' x BulkG-800',
'BulkGravToZZToZlepZinv_narrow_1000':str(k)+' x BulkG-1000',
'BulkGravToZZToZlepZinv_narrow_1200':str(k)+' x BulkG-1200',
'BulkGravToZZToZlepZinv_narrow_1400':str(k)+' x BulkG-1400',
'BulkGravToZZToZlepZinv_narrow_1600':str(k)+' x BulkG-1600',
'BulkGravToZZToZlepZinv_narrow_1800':str(k)+' x BulkG-1800',
'BulkGravToZZToZlepZinv_narrow_2000':str(k)+' x BulkG-2000',
'BulkGravToZZToZlepZinv_narrow_2500':str(k)+' x BulkG-2500',
'BulkGravToZZToZlepZinv_narrow_3000':str(k)+' x BulkG-3000',
'BulkGravToZZToZlepZinv_narrow_3500':str(k)+' x BulkG-3500',
'BulkGravToZZToZlepZinv_narrow_4000':str(k)+' x BulkG-4000',
'BulkGravToZZToZlepZinv_narrow_4500':str(k)+' x BulkG-4500',
}

BulkGZZ2l2nuXsec = {
600:8.61578e-03,
800:1.57965e-03,
1000:4.21651e-04,
1200:1.39919e-04,
1400:5.32921e-05,
1600:2.24428e-05,
1800:1.01523e-05,
2000:4.86037e-06,
2500:9.08739e-07,
3000:1.98856e-07,
3500:4.87505e-08,
4000:1.25937e-08,
4500:1.0,
}

sigXsec = {
'BulkGravToZZToZlepZinv_narrow_600'  : BulkGZZ2l2nuXsec[600]*k,
'BulkGravToZZToZlepZinv_narrow_800'  : BulkGZZ2l2nuXsec[800]*k,
'BulkGravToZZToZlepZinv_narrow_1000' : BulkGZZ2l2nuXsec[1000]*k,
'BulkGravToZZToZlepZinv_narrow_1200' : BulkGZZ2l2nuXsec[1200]*k,
'BulkGravToZZToZlepZinv_narrow_1400' : BulkGZZ2l2nuXsec[1400]*k,
'BulkGravToZZToZlepZinv_narrow_1600' : BulkGZZ2l2nuXsec[1600]*k,
'BulkGravToZZToZlepZinv_narrow_1800' : BulkGZZ2l2nuXsec[1800]*k,
'BulkGravToZZToZlepZinv_narrow_2000' : BulkGZZ2l2nuXsec[2000]*k,
'BulkGravToZZToZlepZinv_narrow_2500' : BulkGZZ2l2nuXsec[2500]*k,
'BulkGravToZZToZlepZinv_narrow_3000' : BulkGZZ2l2nuXsec[3000]*k,
'BulkGravToZZToZlepZinv_narrow_3500' : BulkGZZ2l2nuXsec[3500]*k,
'BulkGravToZZToZlepZinv_narrow_4000' : BulkGZZ2l2nuXsec[4000]*k,
'BulkGravToZZToZlepZinv_narrow_4500' : BulkGZZ2l2nuXsec[4500]*k,
}

if SignalAll1pb:
    for sig in sigSamples:
        sigXsec[sig] = 1.0
        sigSampleNames[sig] = string.replace(sigSampleNames[sig], str(k)+' x', '1 pb')

for sample in sigSamples:
    sigPlotters.append(TreePlotter(sample, indir+'/'+sample+'.root','tree'))
    sigPlotters[-1].addCorrectionFactor('1./SumWeights','norm')
    sigPlotters[-1].addCorrectionFactor(str(sigXsec[sample]),'xsec')
    sigPlotters[-1].addCorrectionFactor('genWeight','genWeight')
    sigPlotters[-1].addCorrectionFactor(puWeight,'puWeight')
    sigPlotters[-1].addCorrectionFactor(lepsf,'lepsf')
    sigPlotters[-1].setFillProperties(0,ROOT.kWhite)
    allPlotters[sample] = sigPlotters[-1]

dataPlotters=[]
dataSamples = [
'SingleEMU_Run2016B2H_ReReco_36p22fbinv',
]
for sample in dataSamples:
    dataPlotters.append(TreePlotter(sample, indir+'/'+sample+'.root','tree'))

if DataHLT:
    dataPlotters[0].addCorrectionFactor('(HLT_MUv2||HLT_ELEv2)','HLT')


Data = MergedPlotter(dataPlotters)

Stack = StackPlotter(outTag=tag, outDir=outdir)
Stack.setPaveText(paveText)
Stack.addPlotter(Data, "data_obs", "Data", "data")
#Stack.addPlotter(WJets, "WJets","W+Jets", "background")
Stack.addPlotter(WW, "NonReso","WW/WZ/WJets non-reson.", "background")
Stack.addPlotter(TT, "TT","TT", "background")
Stack.addPlotter(VV, "VVZReso","ZZ WZ reson.", "background")
Stack.addPlotter(ZJets, "ZJets","ZJets", "background")


for i in range(len(sigSamples)):
  sigPlotters[i].setLineProperties(2,ROOT.kRed+i,2)
  Stack.addPlotter(sigPlotters[i],sigSamples[i],sigSampleNames[sigSamples[i]],'signal')  

 
Stack.setLog(LogY)
Stack.doRatio(doRatio)



tag+='_'

if doSys:
    Stack.drawStack('llnunu_mt', cuts, str(lumi*1000), 300, 0.0, 3000.0, titlex = "M_{T}", units = "GeV",output=tag+'mt',outDir=outdir,separateSignal=sepSig)

    for (plotter,typeP,label,name) in zip(Stack.plotters,Stack.types,Stack.labels,Stack.names): 
        if typeP != "data": plotter.changeCorrectionFactor("trgsf_up*idisotrksf","lepsf")
    Stack.drawStack('llnunu_mt', cuts, str(lumi*1000), 300, 0.0, 3000.0, titlex = "M_{T}", units = "GeV",output=tag+'mt_trgup',outDir=outdir,separateSignal=sepSig) 

    for (plotter,typeP,label,name) in zip(Stack.plotters,Stack.types,Stack.labels,Stack.names):
        if typeP != "data": plotter.changeCorrectionFactor("trgsf_dn*idisotrksf","lepsf") 
    Stack.drawStack('llnunu_mt', cuts, str(lumi*1000), 300, 0.0, 3000.0, titlex = "M_{T}", units = "GeV",output=tag+'mt_trgdn',outDir=outdir,separateSignal=sepSig) 

    for (plotter,typeP,label,name) in zip(Stack.plotters,Stack.types,Stack.labels,Stack.names):
        if typeP != "data": plotter.changeCorrectionFactor("trgsf*idisotrksf_up","lepsf") 
    Stack.drawStack('llnunu_mt', cuts, str(lumi*1000), 300, 0.0, 3000.0, titlex = "M_{T}", units = "GeV",output=tag+'mt_idup',outDir=outdir,separateSignal=sepSig) 

    for (plotter,typeP,label,name) in zip(Stack.plotters,Stack.types,Stack.labels,Stack.names):
        if typeP != "data": plotter.changeCorrectionFactor("trgsf*idisotrksf_dn","lepsf") 
    Stack.drawStack('llnunu_mt', cuts, str(lumi*1000), 300, 0.0, 3000.0, titlex = "M_{T}", units = "GeV",output=tag+'mt_iddn',outDir=outdir,separateSignal=sepSig) 

    for (plotter,typeP,label,name) in zip(Stack.plotters,Stack.types,Stack.labels,Stack.names):
        if typeP != "data" and name=="ZJets" : 
            plotter.changeCorrectionFactor("trgsf*idisotrksf","lepsf") 
            plotter.changeCorrectionFactor("ZPtWeight_up","ZPtWeight") 
    Stack.drawStack('llnunu_mt', cuts, str(lumi*1000), 300, 0.0, 3000.0, titlex = "M_{T}", units = "GeV",output=tag+'mt_zptup',outDir=outdir,separateSignal=sepSig) 

    for (plotter,typeP,label,name) in zip(Stack.plotters,Stack.types,Stack.labels,Stack.names):
        if typeP != "data" and name=="ZJets" :
            plotter.changeCorrectionFactor("trgsf*idisotrksf","lepsf")
            plotter.changeCorrectionFactor("ZPtWeight_dn","ZPtWeight")
    Stack.drawStack('llnunu_mt', cuts, str(lumi*1000), 300, 0.0, 3000.0, titlex = "M_{T}", units = "GeV",output=tag+'mt_zptdn',outDir=outdir,separateSignal=sepSig)

    ######## MT Unc
    for (plotter,typeP,label,name) in zip(Stack.plotters,Stack.types,Stack.labels,Stack.names):
        if typeP != "data":
            plotter.changeCorrectionFactor("trgsf*idisotrksf","lepsf")

    for systs in {'JetEn','JetRes','MuonEn','ElectronEn','TauEn','PhotonEn','Uncluster'}
      Stack.drawStack('llnunu_mt_'+systs+'Up', cuts, str(lumi*1000), 300, 0.0, 3000.0, titlex = "M_{T}", units = "GeV",output=tag+'mt_'+systs+'Up',outDir=outdir,separateSignal=sepSig)
      Stack.drawStack('llnunu_mt_'+systs+'Dn', cuts, str(lumi*1000), 300, 0.0, 3000.0, titlex = "M_{T}", units = "GeV",output=tag+'mt_'+systs+'Dn',outDir=outdir,separateSignal=sepSig)

Stack.closePSFile()
Stack.closeROOTFile()

