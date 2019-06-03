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
#tag="GJets_ICHEPcfg_"
#tag="Test_GJets_ICHEPcfg_"
#tag="test_"
#tag=""
cutChain=options.cutChain
#cutChain='loosecut'
#cutChain='tight'
#cutChain='tightzpt20'
#cutChain='tightzpt50'
#cutChain='tightzptgt50lt200'
#cutChain='tightzpt100'
#cutChain='tightzpt150'
#cutChain='tightzpt100met50'
#cutChain='tightzpt100met100'
#cutChain='tightzpt100met200'

# can be el or mu or both
channel=options.channel
#channel='el' 
LogY=options.LogY
test=options.test
#LogY=True
#test=True
DrawLeptons=False
doRhoScale=True

dyGJets=True

if test: DrawLeptons = False

#lepsf="(1)"
#lepsf="idsf"
#lepsf="trgsf"
#lepsf="isosf"
#lepsf="isosf*idsf"
lepsf="trgsf*isosf*idsf*trksf"
#lepsf="(trgsf*isosf*idsf*((1.03128*(abs(llnunu_l1_l1_pdgId)==11&&abs(llnunu_l1_l2_pdgId)==11))+(0.908956*(abs(llnunu_l1_l1_pdgId)==13&&abs(llnunu_l1_l2_pdgId)==13))))"
#lepsf="(trgsf*((isosf*idsf)*(abs(llnunu_l1_l1_pdgId)==11&&abs(llnunu_l1_l2_pdgId)==11)+(highpt_dt_eff_m1/highpt_mc_eff_m1*highpt_dt_eff_m2/highpt_mc_eff_m2)*(abs(llnunu_l1_l1_pdgId)==13&&abs(llnunu_l1_l2_pdgId)==13)))"
#lepsf="(trgsf*trksf*((isosf*idsf)*(abs(llnunu_l1_l1_pdgId)==11&&abs(llnunu_l1_l2_pdgId)==11)+(tkhighpt_dt_eff_m1/tkhighpt_mc_eff_m1*tkhighpt_dt_eff_m2/tkhighpt_mc_eff_m2)*(abs(llnunu_l1_l1_pdgId)==13&&abs(llnunu_l1_l2_pdgId)==13)))"

#if channel=='el' : lepsf="isosf*idsf*trksf"

if doRhoScale: 
    tag+="RhoWt_"
    lepsf=lepsf+"*(0.232+0.064*rho)"
    #lepsf=lepsf+"*(0.602*exp(-0.5*pow((rho-8.890)/6.187,2))+0.829*exp(-0.5*pow((rho-21.404)/10.866,2)))"
    #lepsf=lepsf+"*(0.122360+0.180976*rho+-0.010879*pow(rho,2)+0.000226*pow(rho,3)-0.000002*pow(rho,4))"

outdir='plots_b2g'

indir='/home/heli/XZZ/80X_20161006_light_Skim'
lumi=27.22
sepSig=True
doRatio=True
#Blind=False
Blind=options.Blind
FakeData=False
UseMETFilter=True
SignalAll1pb=True
puWeight='puWeight68075'
#puWeight='puWeight61651'
#puWeight='puWeight62118'
#puWeight='puWeight62154'
#puWeight='puWeight61665'
#puWeight='puWeight68715'
#puWeight='puWeight62525'
#puWeight='puWeight62127'
#puWeight='puWeight'
ZJetsZPtWeight=True
DataHLT=True
k=1 # signal scale

elChannel='((abs(llnunu_l1_l1_pdgId)==11&&abs(llnunu_l1_l2_pdgId)==11)||(llnunu_l1_l1_pdgId==19801117))'
muChannel='((abs(llnunu_l1_l1_pdgId)==13&&abs(llnunu_l1_l2_pdgId)==13)||(llnunu_l1_l1_pdgId==19801117))'

if not os.path.exists(outdir): os.system('mkdir '+outdir)

tag = tag+cutChain+'_'
tag = tag+puWeight+'_'


if UseMETFilter: tag = tag+'metfilter_'

if not Blind: tag = tag+'unblind_'

tag = tag+channel+'_'
if LogY: tag = tag+'log_'
if SignalAll1pb:
    tag += '1pb'
else:
    tag += 'scale'+str(k)

#tag += '_'


paveText="#sqrt{s} = 13 TeV 2016 L = "+"{:.3}".format(float(lumi))+" fb^{-1}"

metfilter='(Flag_EcalDeadCellTriggerPrimitiveFilter&&Flag_HBHENoiseIsoFilter&&Flag_goodVertices&&Flag_HBHENoiseFilter&&Flag_globalTightHalo2016Filter&&Flag_eeBadScFilter)'

cuts_loose='(nllnunu)'
cuts_lepaccept="((abs(llnunu_l1_l1_pdgId)==13&&abs(llnunu_l1_l2_pdgId)==13&&llnunu_l1_l1_pt>50&&abs(llnunu_l1_l1_eta)<2.4&&llnunu_l1_l2_pt>20&&abs(llnunu_l1_l2_eta)<2.4&&(llnunu_l1_l1_highPtID>0.99||llnunu_l1_l2_highPtID>0.99))"
cuts_lepaccept+="||(abs(llnunu_l1_l1_pdgId)==11&&abs(llnunu_l1_l2_pdgId)==11&&llnunu_l1_l1_pt>115&&abs(llnunu_l1_l1_eta)<2.5&&llnunu_l1_l2_pt>35&&abs(llnunu_l1_l2_eta)<2.5)"
cuts_lepaccept+="||(llnunu_l1_l1_pdgId==19801117))"
cuts_zmass="(llnunu_l1_mass_to_plot>70&&llnunu_l1_mass_to_plot<110)"
cuts_zpt100="(llnunu_l1_pt>100)"
cuts_zpt150="(llnunu_l1_pt>150)"
cuts_met50="(llnunu_l2_pt_to_plot>50)"
cuts_met100="(llnunu_l2_pt_to_plot>100)"
cuts_met200="(llnunu_l2_pt_to_plot>200)"
cuts_loose_z="("+cuts_lepaccept+"&&"+cuts_zmass+")"
cuts_loose_zpt20="("+cuts_lepaccept+"&&"+cuts_zmass+"&&llnunu_l1_pt>20)"
cuts_loose_zpt50="("+cuts_lepaccept+"&&"+cuts_zmass+"&&llnunu_l1_pt>50)"
cuts_loose_zptgt50lt200="("+cuts_lepaccept+"&&"+cuts_zmass+"&&llnunu_l1_pt>50&&llnunu_l1_pt<200)"
cuts_loose_zll="("+cuts_lepaccept+"&&"+cuts_zmass+"&&"+cuts_zpt100+")"
cuts_loose_zpt150="("+cuts_lepaccept+"&&"+cuts_zmass+"&&"+cuts_zpt150+")"
cuts_loose_zll_met50="("+cuts_lepaccept+"&&"+cuts_zmass+"&&"+cuts_zpt100+"&&"+cuts_met50+")"
cuts_loose_zll_met100="("+cuts_lepaccept+"&&"+cuts_zmass+"&&"+cuts_zpt100+"&&"+cuts_met100+")"
cuts_loose_zll_met200="("+cuts_lepaccept+"&&"+cuts_zmass+"&&"+cuts_zpt100+"&&"+cuts_met200+")"


if cutChain=='loosecut': cuts=cuts_loose
elif cutChain=='tight': cuts=cuts_loose_z
elif cutChain=='tightzpt20': cuts=cuts_loose_zpt20
elif cutChain=='tightzpt50': cuts=cuts_loose_zpt50
elif cutChain=='tightzptgt50lt200': cuts=cuts_loose_zptgt50lt200
elif cutChain=='tightzpt100': cuts=cuts_loose_zll
elif cutChain=='tightzpt150': cuts=cuts_loose_zpt150
elif cutChain=='tightzpt100met50': cuts=cuts_loose_zll_met50
elif cutChain=='tightzpt100met100': cuts=cuts_loose_zll_met100
elif cutChain=='tightzpt100met200': cuts=cuts_loose_zll_met200
else : cuts=cuts_loose


if channel=='el': cuts = cuts+'&&'+elChannel
elif channel=='mu': cuts = cuts+'&&'+muChannel

if UseMETFilter:
    cuts = '('+cuts+'&&'+metfilter+')'

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
'ZZTo2L2Q','ZZTo4L']

for sample in vvSamples:
    vvPlotters.append(TreePlotter(sample, indir+'/'+sample+'.root','tree'))
    vvPlotters[-1].addCorrectionFactor('1/SumWeights','norm')
    vvPlotters[-1].addCorrectionFactor('xsec','xsec')
    vvPlotters[-1].addCorrectionFactor('genWeight','genWeight')
    vvPlotters[-1].addCorrectionFactor(puWeight,'puWeight')
    vvPlotters[-1].addCorrectionFactor(lepsf, 'lepsf')
    allPlotters[sample] = vvPlotters[-1]

VV = MergedPlotter(vvPlotters)
VV.setFillProperties(1001,ROOT.kMagenta)

#ggZZPlotters=[]
#ggZZSamples = ['ggZZTo2e2nu','ggZZTo2mu2nu']

#for sample in ggZZSamples:
#    ggZZPlotters.append(TreePlotter(indir+'/'+sample+'.root','tree'))
#    ggZZPlotters[-1].addCorrectionFactor('1./SumWeights','tree')
#    ggZZPlotters[-1].addCorrectionFactor('xsec','tree')
#    ggZZPlotters[-1].addCorrectionFactor('genWeight','genWeight')
#    ggZZPlotters[-1].addCorrectionFactor(puWeight,'tree')
#    ggZZPlotters[-1].addCorrectionFactor('(llnunu_l1_l1_lepsf*llnunu_l1_l2_lepsf)','tree')
#    ggZZPlotters[-1].addCorrectionFactor('triggersf','tree')
#    allPlotters[sample] = ggZZPlotters[-1]

#ggZZ = MergedPlotter(ggZZPlotters)
#ggZZ.setFillProperties(1001,ROOT.kRed)



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
zjetsSamples = ['SinglePhoton_Run2016B2G_PromptReco_RcDataB2GNewFilterLepVeto']


for sample in zjetsSamples:
    zjetsPlotters.append(TreePlotter(sample, indir+'/'+sample+'.root','tree'))
    if dyGJets: 
        zjetsPlotters[-1].setAlias("metPara", "llnunu_l2_pt*cos(llnunu_l2_phi-llnunu_l1_phi)");
        zjetsPlotters[-1].setAlias("metPerp", "llnunu_l2_pt*sin(llnunu_l2_phi-llnunu_l1_phi)");
        zjetsPlotters[-1].setAlias("absDeltaPhi", "fabs(TVector2::Phi_mpi_pi(llnunu_l2_phi-llnunu_l1_phi))");
        zjetsPlotters[-1].setAlias("eta", "llnunu_l1_eta");
        zjetsPlotters[-1].setAlias("phi", "llnunu_l1_phi");
        zjetsPlotters[-1].setAlias("flag2", "(!(eta>0&&eta<0.15&&phi>0.52&&phi<0.56)&&!(eta>-2.5&&eta<-1.4&&phi>-0.5&&phi<0.5)&&!(eta>1.5&&eta<2.5&&phi>-0.5&&phi<0.5)&&!(eta>1.4&&eta<1.6&&phi>-0.8&&phi<-0.5)&&!(eta>1.4&&eta<2.5&&phi>2.5&&phi<4)&&!(eta>1.4&&eta<2.5&&phi>-4&&phi<-2.5)&&!(eta>-2.5&&eta<-1.4&&phi>2.5&&phi<4)&&!(eta>-2.5&&eta<-1.4&&phi>-4&&phi<-2.5)&&!(eta>2.3&&eta<2.6&&phi>-2.5&&phi<-2.2)&&!(eta>0.2&&eta<0.3&&phi>-2.6&&phi<-2.5)&&!(eta>0.5&&eta<0.7&&phi>-1.5&&phi<-1.2)&&!(eta>-0.85&&eta<-0.7&&phi>-1.8&&phi<-1.4)&&!(eta<-2.4&&eta>-2.5&&phi<-1.75&&phi>-1.9)&&!(eta>-2.5&&eta<-2.4&&phi>-1.2&&phi<-1.1)&&!(eta>-2.4&&eta<-2.3&&phi>-2.4&&phi<-2.3))");
        #zjetsPlotters[-1].addCorrectionFactor('(!(metPara/llnunu_l1_pt>-1.8&&metPara/llnunu_l1_pt<-0.8&&fabs(metPerp/llnunu_l1_pt)<0.3))', 'photonVeto')
        #zjetsPlotters[-1].addCorrectionFactor('(fabs(llnunu_l1_eta)<1.47)', 'photonVeto')
        #zjetsPlotters[-1].addCorrectionFactor('(fabs(llnunu_l1_eta)<1.1)', 'photonVeto')
        #zjetsPlotters[-1].addCorrectionFactor('(gjet_l1_idCutBased>=3)', 'photonVeto')
        #zjetsPlotters[-1].addCorrectionFactor('(1)','norm')
        #zjetsPlotters[-1].addCorrectionFactor('(HLT_PHOTONHZZ)','hlt')
        #zjetsPlotters[-1].addCorrectionFactor('(HLT_PHOTONIDISO)','hlt')
        #zjetsPlotters[-1].addCorrectionFactor('(flag2)','flag2')
        #zjetsPlotters[-1].addCorrectionFactor('(phi>-1&&phi<2&&fabs(eta)<1.0)','photonID')
        #zjetsPlotters[-1].addCorrectionFactor('GJetsPhiWeight','PhiWeight')
        zjetsPlotters[-1].addCorrectionFactor('GJetsRhoWeight','RhoWeight')
        if channel=='el' : 
            #zjetsPlotters[-1].addCorrectionFactor('GJetsZPtWeightEl','genWeight')
            zjetsPlotters[-1].addCorrectionFactor('GJetsWeightEl','genWeight')
            zjetsPlotters[-1].addCorrectionFactor('(2.0023405738368289519)','norm') #all
            #zjetsPlotters[-1].addCorrectionFactor('(2.0065962265992)','norm') #all
        elif channel=='mu' : 
            #zjetsPlotters[-1].addCorrectionFactor('GJetsZPtWeightMu','genWeight')
            zjetsPlotters[-1].addCorrectionFactor('GJetsWeightMu','genWeight')
            zjetsPlotters[-1].addCorrectionFactor('(131.444602454)','norm') #mu
            #zjetsPlotters[-1].addCorrectionFactor('(132.382497631379266)','norm') #mu
        else : 
            #zjetsPlotters[-1].addCorrectionFactor('GJetsZPtWeight','genWeight')
            zjetsPlotters[-1].addCorrectionFactor('GJetsWeight','genWeight')
            zjetsPlotters[-1].addCorrectionFactor('(133.42692934197972932)','norm') #el
            #zjetsPlotters[-1].addCorrectionFactor('(134.3822520849576847)','norm') #el
    else:
        zjetsPlotters[-1].addCorrectionFactor('1./SumWeights','norm')
        #zjetsPlotters[-1].addCorrectionFactor('(1)','norm')
        if ZJetsZPtWeight: zjetsPlotters[-1].addCorrectionFactor('ZPtWeight','ZPtWeight')
        #zjetsPlotters[-1].addCorrectionFactor('xsec','xsec')
        zjetsPlotters[-1].addCorrectionFactor('(1921.8*3)','xsec') # FEWZ NNLO.results_z_m50_nnlo_inclusive_NNPDF30_nlo_as_0118
        zjetsPlotters[-1].addCorrectionFactor('genWeight','genWeight')
        #zjetsPlotters[-1].addCorrectionFactor('ZJetsGenWeight','genWeight')
        zjetsPlotters[-1].addCorrectionFactor(puWeight,'puWeight')
        zjetsPlotters[-1].addCorrectionFactor(lepsf,'lepsf')

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
    # some plotting definition
    sigPlotters[-1].setAlias('llnunu_l1_mass_to_plot', 'llnunu_l1_mass')
    sigPlotters[-1].setAlias('llnunu_l2_pt_to_plot', 'llnunu_l2_pt')
    sigPlotters[-1].setAlias('llnunu_l2_phi_to_plot', 'llnunu_l2_phi')
    sigPlotters[-1].setAlias('llnunu_mt_to_plot', 'llnunu_mt')
    allPlotters[sample] = sigPlotters[-1]




dataPlotters=[]
dataSamples = [
'SingleEMU_Run2016B2G_PromptReco', 
]
for sample in dataSamples:
    dataPlotters.append(TreePlotter(sample, indir+'/'+sample+'.root','tree'))

if DataHLT:
    dataPlotters[0].addCorrectionFactor('(HLT_MUv2||HLT_ELEv2)','HLT')


Data = MergedPlotter(dataPlotters)

# some plotting definition
Data.setAlias('llnunu_l1_mass_to_plot', 'llnunu_l1_mass')
Data.setAlias('llnunu_l2_pt_to_plot', 'llnunu_l2_pt')
Data.setAlias('llnunu_l2_phi_to_plot', 'llnunu_l2_phi')
Data.setAlias('llnunu_mt_to_plot', 'llnunu_mt')

# some plotting definition
WW.setAlias('llnunu_l1_mass_to_plot', 'llnunu_l1_mass')
WW.setAlias('llnunu_l2_pt_to_plot', 'llnunu_l2_pt')
WW.setAlias('llnunu_l2_phi_to_plot', 'llnunu_l2_phi')
WW.setAlias('llnunu_mt_to_plot', 'llnunu_mt')

TT.setAlias('llnunu_l1_mass_to_plot', 'llnunu_l1_mass')
TT.setAlias('llnunu_l2_pt_to_plot', 'llnunu_l2_pt')
TT.setAlias('llnunu_l2_phi_to_plot', 'llnunu_l2_phi')
TT.setAlias('llnunu_mt_to_plot', 'llnunu_mt')

VV.setAlias('llnunu_l1_mass_to_plot', 'llnunu_l1_mass')
VV.setAlias('llnunu_l2_pt_to_plot', 'llnunu_l2_pt')
VV.setAlias('llnunu_l2_phi_to_plot', 'llnunu_l2_phi')
VV.setAlias('llnunu_mt_to_plot', 'llnunu_mt')

if dyGJets and channel=='el':
    ZJets.setAlias('llnunu_l1_mass_to_plot', 'llnunu_l1_mass_el')
    ZJets.setAlias('llnunu_l2_pt_to_plot', 'llnunu_l2_pt_el')
    ZJets.setAlias('llnunu_l2_phi_to_plot', 'llnunu_l2_phi_el')
    ZJets.setAlias('llnunu_mt_to_plot', 'llnunu_mt_el')
elif dyGJets and channel=='mu':
    ZJets.setAlias('llnunu_l1_mass_to_plot', 'llnunu_l1_mass_mu')
    ZJets.setAlias('llnunu_l2_pt_to_plot', 'llnunu_l2_pt_mu')
    ZJets.setAlias('llnunu_l2_phi_to_plot', 'llnunu_l2_phi_mu')
    ZJets.setAlias('llnunu_mt_to_plot', 'llnunu_mt_mu')
else:
    ZJets.setAlias('llnunu_l1_mass_to_plot', 'llnunu_l1_mass')
    ZJets.setAlias('llnunu_l2_pt_to_plot', 'llnunu_l2_pt')
    ZJets.setAlias('llnunu_l2_phi_to_plot', 'llnunu_l2_phi')
    ZJets.setAlias('llnunu_mt_to_plot', 'llnunu_mt')



Stack = StackPlotter(outTag=tag, outDir=outdir)
Stack.setPaveText(paveText)
Stack.addPlotter(Data, "data_obs", "Data", "data")
#Stack.addPlotter(WJets, "WJets","W+Jets", "background")
Stack.addPlotter(WW, "NonReso","WW/WZ/WJets non-reson.", "background")
Stack.addPlotter(TT, "TT","TT", "background")
Stack.addPlotter(VV, "VVZReso","ZZ WZ reson.", "background")
#Stack.addPlotter(ggZZ, "ggZZ","ggZZ", "background")
Stack.addPlotter(ZJets, "ZJets","ZJets(#gamma+Jets)", "background")


for i in range(len(sigSamples)):
  sigPlotters[i].setLineProperties(2,ROOT.kRed+i,2)
  Stack.addPlotter(sigPlotters[i],sigSamples[i],sigSampleNames[sigSamples[i]],'signal')  

 
Stack.setLog(LogY)
Stack.doRatio(doRatio)



tag+='_'


if test: 
#    Stack.drawStack('llnunu_l1_mass_to_plot', cuts, str(lumi*1000), 50, 50, 150, titlex = "M(Z)", units = "GeV",output=tag+'zmass',outDir=outdir,separateSignal=sepSig)
    #Stack.drawStack('llnunu_l1_mass_to_plot', cuts, str(lumi*1000), 50, 50, 150, titlex = "M(Z)", units = "GeV",output=tag+'zmass',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('llnunu_l1_pt', cuts, str(lumi*1000), 100, 0.0, 500.0, titlex = "P_{T}(Z)", units = "GeV",output=tag+'zpt_low',outDir=outdir,separateSignal=sepSig)
#    Stack.drawStack('llnunu_l1_rapidity', cuts, str(lumi*1000), 60, -3.0, 3.0, titlex = "Rapidity(Z) ", units = "",output=tag+'zrapidity',outDir=outdir,separateSignal=sepSig)
    #Stack.drawStack('llnunu_l1_l1_pt', cuts, str(lumi*1000), 200, 0.0, 1000.0, titlex = "P_{T}(l_{1})", units = "GeV",output=tag+'pTlep1',outDir=outdir,separateSignal=sepSig)
    #Stack.drawStack('llnunu_l1_l2_pt_to_plot', cuts, str(lumi*1000), 200, 0.0, 500.0, titlex = "P_{T}(l_{2})", units = "GeV",output=tag+'pTlep2',outDir=outdir,separateSignal=sepSig)
    #Stack.drawStack('llnunu_l1_l1_trackerIso', cuts+"&&(abs(llnunu_l1_l1_pdgId)==13)", str(lumi*1000), 100, 0.0, 0.2, titlex = "trackerISO_{rel}(#mu_{1})", units = "",output=tag+'ISOlep1_mu',outDir=outdir,separateSignal=sepSig)
    #Stack.drawStack('llnunu_l1_l2_trackerIso', cuts+"&&(abs(llnunu_l1_l2_pdgId)==13)", str(lumi*1000), 100, 0.0, 0.2, titlex = "trackerISO_{rel}(#mu_{2})", units = "",output=tag+'ISOlep2_mu',outDir=outdir,separateSignal=sepSig)
    #Stack.drawStack('abs(llnunu_l1_pt+llnunu_l2_pt_to_plot*cos(llnunu_l2_phi_to_plot-llnunu_l1_phi))/llnunu_l1_pt', cuts, str(lumi*1000), 100, 0, 5, titlex = "#Delta P_{T}^{#parallel}(Z,MET)/P_{T}(Z)", units = "",output=tag+'dPTParaRel',outDir=outdir,separateSignal=sepSig)
#    Stack.drawStack('llnunu_mt_to_plot', cuts, str(lumi*1000), 100, 0.0, 500.0, titlex = "M_{T}", units = "GeV",output=tag+'mt',outDir=outdir,separateSignal=sepSig,blinding=Blind,blindingCut=500)
    #Stack.drawStack('llnunu_mt_to_plot', cuts, str(lumi*1000), 300, 0.0, 3000.0, titlex = "M_{T}", units = "GeV",output=tag+'mt_high3',outDir=outdir,separateSignal=sepSig,blinding=Blind,blindingCut=500)
    #Stack.drawStack('llnunu_l2_pt_to_plot', cuts, str(lumi*1000), 100, 0, 300, titlex = "MET", units = "GeV",output=tag+'met_low2',outDir=outdir,separateSignal=sepSig,blinding=Blind,blindingCut=200)
#    Stack.drawStack('llnunu_l2_pt_to_plot', cuts, str(lumi*1000), 100, 0, 300, titlex = "MET", units = "GeV",output=tag+'met_low2',outDir=outdir,separateSignal=sepSig,blinding=Blind,blindingCut=500)
#    Stack.drawStack('llnunu_l2_pt_to_plot', cuts, str(lumi*1000), 50, 0, 500, titlex = "MET", units = "GeV",output=tag+'met_low',outDir=outdir,separateSignal=sepSig,blinding=Blind,blindingCut=500)
    #Stack.drawStack('llnunu_l2_pt_to_plot*cos(llnunu_l2_phi_to_plot-llnunu_l1_phi)', cuts, str(lumi*1000), 100, -200, 200.0, titlex = "MET_{#parallel}", units = "GeV",output=tag+'met_para',outDir=outdir,separateSignal=sepSig)
#    Stack.drawStack('llnunu_l2_pt_to_plot*cos(llnunu_l2_phi_to_plot-llnunu_l1_phi)', cuts, str(lumi*1000), 100, -200, 200.0, titlex = "MET_{#parallel}", units = "GeV",output=tag+'met_para',outDir=outdir,separateSignal=sepSig)
#    Stack.drawStack('llnunu_l2_pt_to_plot*sin(llnunu_l2_phi_to_plot-llnunu_l1_phi)', cuts, str(lumi*1000), 100, -200, 200.0, titlex = "MET_{#perp}", units = "GeV",output=tag+'met_perp',outDir=outdir,separateSignal=sepSig)
#    Stack.drawStack('nVert', cuts, str(lumi*1000), 50, 0.0, 50.0, titlex = "N vertices", units = "",output=tag+'nVert',outDir=outdir,separateSignal=sepSig)
#    Stack.drawStack('rho', cuts, str(lumi*1000), 50, 0.0, 50.0, titlex = "#rho", units = "",output=tag+'rho',outDir=outdir,separateSignal=sepSig)
else: 
    Stack.drawStack('llnunu_mt_to_plot', cuts, str(lumi*1000), 300, 0.0, 3000.0, titlex = "M_{T}", units = "GeV",output=tag+'mt_high3',outDir=outdir,separateSignal=sepSig,blinding=Blind,blindingCut=300)
    Stack.drawStack('llnunu_mt_to_plot', cuts, str(lumi*1000), 240, 0.0, 1200.0, titlex = "M_{T}", units = "GeV",output=tag+'mt',outDir=outdir,separateSignal=sepSig,blinding=Blind,blindingCut=300)
    Stack.drawStack('llnunu_mt_to_plot', cuts, str(lumi*1000), 120, 0.0, 600.0, titlex = "M_{T}", units = "GeV",output=tag+'mt_low',outDir=outdir,separateSignal=sepSig,blinding=Blind,blindingCut=300)
    Stack.drawStack('llnunu_mt_to_plot', cuts, str(lumi*1000), 60, 0.0, 300.0, titlex = "M_{T}", units = "GeV",output=tag+'mt_low2',outDir=outdir,separateSignal=sepSig,blinding=Blind,blindingCut=300)
    Stack.drawStack('llnunu_l2_pt_to_plot', cuts, str(lumi*1000), 100, 0, 1000, titlex = "MET", units = "GeV",output=tag+'met',outDir=outdir,separateSignal=sepSig,blinding=Blind,blindingCut=200)
    Stack.drawStack('llnunu_l2_pt_to_plot', cuts, str(lumi*1000), 50, 0, 500, titlex = "MET", units = "GeV",output=tag+'met_low',outDir=outdir,separateSignal=sepSig,blinding=Blind,blindingCut=200)
    Stack.drawStack('llnunu_l2_pt_to_plot', cuts, str(lumi*1000), 100, 0, 300, titlex = "MET", units = "GeV",output=tag+'met_low2',outDir=outdir,separateSignal=sepSig,blinding=Blind,blindingCut=200)
    Stack.drawStack('llnunu_l1_mass_to_plot', cuts, str(lumi*1000), 50, 50, 150, titlex = "M(Z)", units = "GeV",output=tag+'zmass',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('llnunu_l1_pt', cuts, str(lumi*1000), 75, 0.0, 1500.0, titlex = "P_{T}(Z)", units = "GeV",output=tag+'zpt',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('llnunu_l1_pt', cuts, str(lumi*1000), 100, 0.0, 500.0, titlex = "P_{T}(Z)", units = "GeV",output=tag+'zpt_low',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('llnunu_l1_pt', cuts, str(lumi*1000), 100, 0.0, 300.0, titlex = "P_{T}(Z)", units = "GeV",output=tag+'zpt_low2',outDir=outdir,separateSignal=sepSig)
    if not dyGJets:   Stack.drawStack('TMath::Tan((TMath::Pi()-TMath::Abs(llnunu_l1_deltaPhi))/2.0)*TMath::Sin(TMath::ACos(TMath::TanH((llnunu_l1_l1_eta-llnunu_l1_l2_eta)/2.0)))', cuts, str(lumi*1000), 100, 0.0, 10, titlex = "#phi_{#eta}*", units = "",output=tag+'PhiStar',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('nVert', cuts, str(lumi*1000), 50, 0.0, 50.0, titlex = "N vertices", units = "",output=tag+'nVert',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('rho', cuts, str(lumi*1000), 50, 0.0, 50.0, titlex = "#rho", units = "",output=tag+'rho',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('llnunu_l2_pt_to_plot*cos(llnunu_l2_phi_to_plot-llnunu_l1_phi)', cuts, str(lumi*1000), 100, -200, 200.0, titlex = "MET_{#parallel}", units = "GeV",output=tag+'met_para',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('llnunu_l2_pt_to_plot*sin(llnunu_l2_phi_to_plot-llnunu_l1_phi)', cuts, str(lumi*1000), 100, -200, 200.0, titlex = "MET_{#perp}", units = "GeV",output=tag+'met_perp',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('llnunu_l2_pt_to_plot/sqrt(llnunu_l2_sumEt)', cuts, str(lumi*1000), 100, 0.0, 20.0, titlex = "MET/#sqrt{sumE_{T}}", units = "",output=tag+'metOvSqSET',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('fabs(TVector2::Phi_mpi_pi(llnunu_l2_phi_to_plot-llnunu_l1_phi))', cuts, str(lumi*1000), 50, 0, 5, titlex = "#Delta#phi(Z,MET)", units = "",output=tag+'dphiZMet',outDir=outdir,separateSignal=sepSig)
    #Stack.drawStack('cos(llnunu_l2_phi_to_plot-llnunu_l1_phi)', cuts, str(lumi*1000), 50, -1, 1, titlex = "cos(#Delta#phi(Z,MET))", units = "",output=tag+'CosdphiZMet',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('fabs(llnunu_l1_pt+llnunu_l2_pt_to_plot*cos(llnunu_l2_phi_to_plot-llnunu_l1_phi))', cuts, str(lumi*1000), 100, 0, 1000, titlex = "#Delta P_{T}^{#parallel}(Z,MET)", units = "GeV",output=tag+'dPTPara',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('fabs(llnunu_l2_pt_to_plot*sin(llnunu_l2_phi_to_plot-llnunu_l1_phi))', cuts, str(lumi*1000), 100, 0, 100, titlex = "#Delta P_{T}^{#perp}(Z,MET)", units = "GeV",output=tag+'dPTPerp',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('fabs(llnunu_l1_pt+llnunu_l2_pt_to_plot*cos(llnunu_l2_phi_to_plot-llnunu_l1_phi))/llnunu_l1_pt', cuts, str(lumi*1000), 100, 0, 5, titlex = "#Delta P_{T}^{#parallel}(Z,MET)/P_{T}(Z)", units = "",output=tag+'dPTParaRel',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('fabs(llnunu_l2_pt_to_plot*sin(llnunu_l2_phi_to_plot-llnunu_l1_phi))/llnunu_l1_pt', cuts, str(lumi*1000), 100, 0, 5, titlex = "#Delta P_{T}^{#perp}(Z,MET)/P_{T}", units = "",output=tag+'dPTPerpRel',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('llnunu_l1_eta', cuts, str(lumi*1000), 200, -10.0, 10.0, titlex = "#eta(Z) ", units = "",output=tag+'zeta',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('llnunu_l1_rapidity', cuts, str(lumi*1000), 60, -3.0, 3.0, titlex = "Rapidity(Z) ", units = "",output=tag+'zrapidity',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('llnunu_l1_phi', cuts, str(lumi*1000), 64, -3.2, 3.2, titlex = "#phi(Z)", units = "",output=tag+'zphi',outDir=outdir,separateSignal=sepSig)
    if not dyGJets: Stack.drawStack('llnunu_l1_mt', cuts, str(lumi*1000), 50, 0.0, 150.0, titlex = "M_{T}(Z)", units = "GeV",output=tag+'zmt',outDir=outdir,separateSignal=sepSig)
    if not dyGJets: Stack.drawStack('llnunu_l1_deltaPhi', cuts, str(lumi*1000), 50, 0.0, 5.0, titlex = "#Delta#phi", units = "",output=tag+'ZdeltaPhi',outDir=outdir,separateSignal=sepSig)
    #if not dyGJets: Stack.drawStack('cos(llnunu_l1_deltaPhi)', cuts, str(lumi*1000), 50, -1, 1, titlex = "Cos(#Delta#phi)", units = "",output=tag+'CosZdeltaPhi',outDir=outdir,separateSignal=sepSig)
    if not dyGJets:  Stack.drawStack('llnunu_l1_deltaR', cuts, str(lumi*1000), 50, 0.0, 5.0, titlex = "#Delta R", units = "",output=tag+'ZdeltaR',outDir=outdir,separateSignal=sepSig)
  
    Stack.drawStack('llnunu_l2_phi_to_plot', cuts, str(lumi*1000), 100, -3.2, 3.2, titlex = "#phi(MET)", units = "",output=tag+'metPhi',outDir=outdir,separateSignal=sepSig)
    Stack.drawStack('llnunu_l2_sumEt', cuts, str(lumi*1000), 80, 0.0, 3000.0, titlex = "sumE_{T}", units = "GeV",output=tag+'metSumEt',outDir=outdir,separateSignal=sepSig)

    if channel!='el' and channel!='mu' and not dyGJets:
        Stack.drawStack('llnunu_l1_l1_pt', cuts, str(lumi*1000), 200, 0.0, 1000.0, titlex = "P_{T}(l_{1})", units = "GeV",output=tag+'pTlep1',outDir=outdir,separateSignal=sepSig)
        Stack.drawStack('llnunu_l1_l1_eta', cuts, str(lumi*1000), 60, -3.0, 3.0, titlex = "#eta(l_{1})", units = "",output=tag+'etalep1',outDir=outdir,separateSignal=sepSig)
        Stack.drawStack('llnunu_l1_l1_phi', cuts, str(lumi*1000), 64, -3.2, 3.2, titlex = "#phi(l_{1})", units = "",output=tag+'philep1',outDir=outdir,separateSignal=sepSig)
        Stack.drawStack('llnunu_l1_l2_pt', cuts, str(lumi*1000), 200, 0.0, 500.0, titlex = "P_{T}(l_{2})", units = "GeV",output=tag+'pTlep2',outDir=outdir,separateSignal=sepSig)
        Stack.drawStack('llnunu_l1_l2_eta', cuts, str(lumi*1000), 60, -3.0, 3.0, titlex = "#eta(l_{2})", units = "",output=tag+'etalep2',outDir=outdir,separateSignal=sepSig)
        Stack.drawStack('llnunu_l1_l2_phi', cuts, str(lumi*1000), 64, -3.2, 3.2, titlex = "#phi(l_{2})", units = "",output=tag+'philep2',outDir=outdir,separateSignal=sepSig)

if DrawLeptons and not dyGJets:

    if channel!='el' :
        Stack.drawStack('llnunu_l1_l1_pt', cuts+"&&(abs(llnunu_l1_l1_pdgId)==13)", str(lumi*1000), 100, 0.0, 1000.0, titlex = "P_{T}(#mu_{1})", units = "GeV",output=tag+'pTlep1_mu',outDir=outdir,separateSignal=sepSig)
        Stack.drawStack('llnunu_l1_l1_eta', cuts+"&&(abs(llnunu_l1_l1_pdgId)==13)", str(lumi*1000), 30, -3.0, 3.0, titlex = "#eta(#mu_{1})", units = "",output=tag+'etalep1_mu',outDir=outdir,separateSignal=sepSig)
        Stack.drawStack('llnunu_l1_l1_phi', cuts+"&&(abs(llnunu_l1_l1_pdgId)==13)", str(lumi*1000), 32, -3.2, 3.2, titlex = "#phi(#mu_{1})", units = "",output=tag+'philep1_mu',outDir=outdir,separateSignal=sepSig)
        Stack.drawStack('llnunu_l1_l1_trackerIso', cuts+"&&(abs(llnunu_l1_l1_pdgId)==13)", str(lumi*1000), 100, 0.0, 0.2, titlex = "trackerISO_{rel}(#mu_{1})", units = "",output=tag+'ISOlep1_mu',outDir=outdir,separateSignal=sepSig)

        Stack.drawStack('llnunu_l1_l2_pt', cuts+"&&(abs(llnunu_l1_l2_pdgId)==13)", str(lumi*1000), 100, 0.0, 500.0, titlex = "P_{T}(#mu_{2})", units = "GeV",output=tag+'pTlep2_mu',outDir=outdir,separateSignal=sepSig)
        Stack.drawStack('llnunu_l1_l2_eta', cuts+"&&(abs(llnunu_l1_l2_pdgId)==13)", str(lumi*1000), 30, -3.0, 3.0, titlex = "#eta(#mu_{2})", units = "",output=tag+'etalep2_mu',outDir=outdir,separateSignal=sepSig)
        Stack.drawStack('llnunu_l1_l2_phi', cuts+"&&(abs(llnunu_l1_l2_pdgId)==13)", str(lumi*1000), 32, -3.2, 3.2, titlex = "#phi(#mu_{2})", units = "",output=tag+'philep2_mu',outDir=outdir,separateSignal=sepSig)
        Stack.drawStack('llnunu_l1_l2_trackerIso', cuts+"&&(abs(llnunu_l1_l2_pdgId)==13)", str(lumi*1000), 100, 0.0, 0.2, titlex = "trackerISO_{rel}(#mu_{2})", units = "",output=tag+'ISOlep2_mu',outDir=outdir,separateSignal=sepSig)

    if channel!='mu' : 
        Stack.drawStack('llnunu_l1_l1_pt', cuts+"&&(abs(llnunu_l1_l1_pdgId)==11)", str(lumi*1000), 100, 0.0, 1000.0, titlex = "P_{T}(e_{1})", units = "GeV",output=tag+'pTlep1_el',outDir=outdir,separateSignal=sepSig)
        Stack.drawStack('llnunu_l1_l1_eta', cuts+"&&(abs(llnunu_l1_l1_pdgId)==11)", str(lumi*1000), 30, -3.0, 3.0, titlex = "#eta(e_{1})", units = "",output=tag+'etalep1_el',outDir=outdir,separateSignal=sepSig)
        Stack.drawStack('llnunu_l1_l1_phi', cuts+"&&(abs(llnunu_l1_l1_pdgId)==11)", str(lumi*1000), 32, -3.2, 3.2, titlex = "#phi(e_{1})", units = "",output=tag+'philep1_el',outDir=outdir,separateSignal=sepSig)
        Stack.drawStack('llnunu_l1_l1_electronrelIsoea03', cuts+"&&(abs(llnunu_l1_l1_pdgId)==11)", str(lumi*1000), 100, 0.0, 0.2, titlex = "looseISO_{rel}(e_{1})", units = "",output=tag+'ISOlep1_el',outDir=outdir,separateSignal=sepSig)

        Stack.drawStack('llnunu_l1_l2_pt', cuts+"&&(abs(llnunu_l1_l2_pdgId)==11)", str(lumi*1000), 100, 0.0, 500.0, titlex = "P_{T}(e_{2})", units = "GeV",output=tag+'pTlep2_el',outDir=outdir,separateSignal=sepSig)
        Stack.drawStack('llnunu_l1_l2_eta', cuts+"&&(abs(llnunu_l1_l2_pdgId)==11)", str(lumi*1000), 30, -3.0, 3.0, titlex = "#eta(e_{2})", units = "",output=tag+'etalep2_el',outDir=outdir,separateSignal=sepSig)
        Stack.drawStack('llnunu_l1_l2_phi', cuts+"&&(abs(llnunu_l1_l2_pdgId)==11)", str(lumi*1000), 32, -3.2, 3.2, titlex = "#phi(e_{2})", units = "",output=tag+'philep2_el',outDir=outdir,separateSignal=sepSig)
        Stack.drawStack('llnunu_l1_l2_electronrelIsoea03', cuts+"&&(abs(llnunu_l1_l2_pdgId)==11)", str(lumi*1000), 100, 0.0, 0.2, titlex = "looseISO_{rel}(e_{2})", units = "",output=tag+'ISOlep2_el',outDir=outdir,separateSignal=sepSig)


Stack.closePSFile()
Stack.closeROOTFile()

