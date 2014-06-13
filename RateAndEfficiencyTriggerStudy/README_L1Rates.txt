#########################################################
############## DPG code to create L1 ntuples ############# 
##########################################################

Installation:

cmsrel CMSSW_7_1_0_pre8
cd CMSSW_7_1_0_pre8/src
cmsenv

git clone https://github.com/uwcms/UCT2015.git L1Trigger/UCT2015
cd L1Trigger/UCT2015
git checkout 2014-Menus-V47
cd ../..

git cms-merge-topic --unsafe cms-l1t-offline:l1t-devel-$CMSSW_VERSION

git clone https://github.com/cms-l1-dpg/L1Ntuples.git L1TriggerDPG/L1Ntuples
cd  L1TriggerDPG/L1Ntuples
git checkout -b mydevel_71X origin/devel_71X 
cd ../..

git clone https://github.com/cms-l1-dpg/L1Menu.git L1TriggerDPG/L1Menu
cd L1TriggerDPG/L1Menu 
git checkout -b mydevel_71X origin/devel_71X
cd ../..

git clone https://github.com/mcepeda/ExamplesTSG.git TutorialL1/Examples 

scramv1 b -j 9

######################

Code for L1 Ntuple:

L1TriggerDPG/L1Menu/test/customL1NtupleFromRaw.py

Input Parameters: 1) globalTag     -> to be specified 
                  2) reEmulation   -> to run the emulator or not
                  3) reEmulMuons   -> emulator for L1 muons
                  4) reEmulCalos   -> emulator for L1 calos
                  5) patchNtuple   -> use or not emulator ntuples
                  6) runOnMC       -> if you run on the MC
                  7) runOnPostLS1  -> run on post LS1 MC
                  8) whichPU       -> average pile-up
                  9) keepEDMOutput -> keep part of the edm in the output
                  10) dttfLutsFile -> DTTF LUTs sqlite input file
                  11) customPACT   -> Enables usage of new RPC PACT patterns
                  12) customGMT    -> Switches to minPT for the GMT
                  13) useUct2015   -> Enables UCT2015 emulation for calos
                  14) puReweightingFile -> InputFile to be used for PU reweighting           


-> Given the suggested set of inputs -> reEmulation=True, reEmulMuons=True, reEmulCalos=True, patchNtuple=True, force2012Config=True, customDTTF=True, 
                                        dttfLutsFile=sqlite:../data/dttf_config.db globalTag=POSTLS162_V2::All, runOnMC=True runOnPostLS1=True useUct2015=True.


-> To Run (example):  cd $CMSSW_BASE/src/L1TriggerDPG/L1Menu/test
                      cmsRun customL1NtupleFromRaw.py reEmulation=True reEmulMuons=True reEmulCalos=True patchNtuple=True force2012Config=True customDTTF=True dttfLutsFile=sqlite:../data/dttf_config.db globalTag=POSTLS162_V2::All runOnMC=True runOnPostLS1=True useUct2015=True
   

   Input is a GEN-SIM-RAW AOD sample


-> cms.Path that is executed:

   Unpacker stage: process.csctfDigis, process.dttfDigis, process.gctDigis, process.gtDigis, process.gtEvmDigis, process.siPixelDigis, process.siStripDigis, 
                   process.ecalPreshowerDigis+process.hcalDigis+process.muonCSCDigis+process.muonRPCDigis+process.castorDigis+process.scalersRawToDigi

   Emulator chain: process.reEmul -> process.reEmulUctChain+process.reEmulMuonChain+process.gtReEmulDigis+process.l1ExtraReEmul
                   

                   process.reEmulUctChain = cms.Sequence(process.emulationSequence+process.uctGctDigis)
                   
                   emulationSequence =  process.gctDigis+process.gtDigis+process.ecalDigis+process.hcalDigis ->  already done in the unpacker stage
                                        process.valHcalTriggerPrimitiveDigis -> digi of the trigger primitive in Hcal
                                        process.hackHCALMIPs   -> HcalTpgMipEmbedder 
                                        process.uctDigis       -> L1RCTProducer

                                        process.CorrectedDigis -> RegionCorrection 

                                                                  regionSubtraction -> for PU subtraction -> select a set of info
                                                                  regionLSB -> dimension
                                                                  puMultCorrect ->if true PU subtraction is calculated 

                                        process.UCT2015Producer -> some tresholds are interesting here

                                                                  jetSeed ->sed energy for jets 
                                                                  tauSeed 
                                                                  regionLSB
                                                                  relativeJetIsolationCut -> EG Isolation
                                                                  puMultCorrect
                                                                  puETMax
                                                                  jetSF
                                                                  useUICrho
                                                                  applyJetCalibration
                                                                  regionETCutForHT
                                                                  maxGctEtaForSums = cms.uint32(17) -> exclude HF

                                        process.uctGctDigis = = cms.EDProducer("UCT2015GctCandsProducer", -> objected produced
                                                 metSource = cms.InputTag("UCT2015Producer","METUnpacked"),
                                                 jetSource = cms.InputTag("UCT2015Producer","CorrJetUnpacked"),
                                                 egRelaxed = cms.InputTag("UCT2015Producer","RelaxedEGUnpacked"),
                                                 shtSource = cms.InputTag("UCT2015Producer","SHTUnpacked"),
                                                 mhtSource = cms.InputTag("UCT2015Producer","MHTUnpacked"),
                                                 egIsolated = cms.InputTag("UCT2015Producer","IsolatedEGUnpacked"),
                                                 tauIsolated = cms.InputTag("UCT2015Producer","IsolatedTauUnpacked"),
                                                 setSource = cms.InputTag("UCT2015Producer","SETUnpacked"))


   Emulator muon chain: process.reEmulMuonChain = process.rpcTriggerReEmulDigis + process.simCscTriggerPrimitiveDigis + process.csctfReEmulTrackDigis + process.csctfReEmulDigis + 
                                                  process.dttfReEmulDigis + process.gmtReEmulDigis

 
   L1Global Trigger emulator: process.gtReEmulDigis + process.l1ExtraReEmul -> L1 trigger menu result

   process.l1extraParticles -> extra particle collection producer 

   process.l1GtTriggerMenuLite

   process.l1NtupleProducer: EDAnalyzer a lot of input elements: maxGMT = cms.uint32(12)
                                                                 maxDTTH = cms.uint32(50)
                                                                 maxCSCTFLCTS = cms.uint32(360)
                                                                 gctNonIsoEmSource = cms.InputTag("uctGctDigis","nonIsoEm")
                                                                 gctCentralJetsSource = cms.InputTag("uctGctDigis","cenJets")
                                                                 gtEvmSource = cms.InputTag("gtEvmDigis")
                                                                 simulationSource = cms.InputTag("addPileupInfo")
                                                                 hltSource = cms.InputTag("TriggerResults","","HLT")
                                                                 csctfStatusSource = cms.InputTag("csctfReEmulTrackDigis")
                                                                 gctForwardJetsSource = cms.InputTag("uctGctDigis","forJets")
                                                                 dttfSource = cms.InputTag("dttfReEmulDigis")
                                                                 gctEnergySumsSource = cms.InputTag("uctGctDigis")
                                                                 hcalSource = cms.InputTag("hcalDigis"),
                                                                 ecalSource = cms.InputTag("ecalDigis","EcalTriggerPrimitives"),
                                                                 csctfLCTSource = cms.InputTag("csctfDigis"),
                                                                 csctfDTStubsSource = cms.InputTag("csctfDigis","DT"),
                                                                 gtSource = cms.InputTag("gtReEmulDigis"),
                                                                 csctfTrkSource = cms.InputTag("csctfReEmulTrackDigis"),
                                                                 gmtSource = cms.InputTag("gmtReEmulDigis"),
                                                                 gctIsoEmSource = cms.InputTag("uctGctDigis","isoEm"),


   process.l1ExtraTreeProducer = cms.EDAnalyzer("L1ExtraTreeProducer",
    nonIsoEmLabel = cms.untracked.InputTag("l1ExtraReEmul","NonIsolated"),
    tauJetLabel = cms.untracked.InputTag("l1ExtraReEmul","Tau"),
    maxL1Extra = cms.uint32(20),
    mhtLabel = cms.untracked.InputTag("l1ExtraReEmul","MHT"),
    cenJetLabel = cms.untracked.InputTag("l1ExtraReEmul","Central"),
    isoEmLabel = cms.untracked.InputTag("l1ExtraReEmul","Isolated"),
    hfRingsLabel = cms.untracked.InputTag("l1extraParticles"),
    fwdJetLabel = cms.untracked.InputTag("l1ExtraReEmul","Forward"),
    muonLabel = cms.untracked.InputTag("l1ExtraReEmul"),
    metLabel = cms.untracked.InputTag("l1ExtraReEmul","MET"))



#######################################
### Class to calcualte rates ##########
#######################################

macros/BasicRatePlots.C

void BasicRatePlots::run(bool runOnData, std::string resultTag, int minLs, int maxLs, float crossSec, float avPU, int nBunches, int isCrossSec, int nEvents, bool noTauInJet)

1) runOnData -> discriminate among data and mc
2) resultTag -> name for the resulting plot output directory
3) minLs -> minimum lumi section to be analyzed
4) maxLs -> maximum lumi section to be analyzed
5) crossSec
6) avPU -> average nPU used in the emulation
7) nBunches -> input 
8) isCrossSec -> to have the final result in expected cross section or rate
9) nEvents -> number of events to be used for the study -> zero means all the events
10) noTauInJet -> skip tau in jet 

../L1Ntuples/macros/L1Ntuple.h -> since the object heredits from this class
../L1Ntuples/macros/L1AlgoFactory.h -> called by this object

Allocate a lot of TH1F  for rate distribution vs the quantity we are interesting to study -> rate shape for each L1 that you can emulate on top of the ntuples which give you
a sort of the output of the L1 as it was done by the hardware. Map[string,TH1F*]

Loop on the event ->  1) FillBits -> read which are the interesting bit for each event
                      2) ZeroBias is the denominator taking into account the weight of each event
                      3) take the jet with highest pt in the event both central and forward; take the HT val, ET val, ETM (met), SingleEgPt .. etc (the isolation cut is applied at
                         ntuple production level and not here)

                      4) Fill the istogram with those value --> single float -> number of jet as a function of pt -> same for EG and so on
                      5) Scale for zero bias cross section -> ScaleFactor(nZeroBias,nBunches) -> ZeroBias total number of weighted events , float scal = 11246.; // ZB per bunch in Hz

                         Scale 1/ZeroBias the nPU, the other for the ScaleFactor -> rate error sqrt(N)

goRatePlots -> method to run the code.

##################################
## Proposal  for L1 rate study ###
##################################

1) On Data 8TeV : Zero Bias and High PU runs in 2012 D period

                  -> /ZeroBias1/Run2012D-v1/RAW
                  -> /ZeroBias25ns1/Run2012D-v1/RAW
                  -> Not in T2: /MinimumBias25ns/Run2012D-v1/RAW
                  -> Not in T2: /MinimumBias/Run2012D-HighLumi-22Jan2013-v1/RAW-RECO



2) On MC 13 TeV -> TSG samples:

   /QCD_Pt-15to30_Tune4C_13TeV_pythia8/Fall13dr-castor_tsg_PU40bx50_POSTLS162_V2-v1/GEN-SIM-RAW
   /QCD_Pt-15to30_Tune4C_13TeV_pythia8/Fall13dr-castor_tsg_PU40bx25_POSTLS162_V2-v1/GEN-SIM-RAW

   Same for :  QCD Pt30-50, QCD Pt50-80 

   Neutrino Gun samples: Neutrino_Pt-2to20_gun PU40BX50
                         Neutrino_Pt-2to20_gun PU40BX25


###########################
## Efficiency Study Code ##
###########################

process.uctDigiStep     = cms.Sequence(process.gctDigis+process.gtDigis+process.ecalDigis+process.hcalDigis+process.valHcalTriggerPrimitiveDigis) -> already seen in Emulator
process.uctEmulatorStep = cms.Sequence(process.hackHCALMIPs+process.uctDigis+process.CorrectedDigis+process.UCT2015Producer) -> already seen in the Emulator
process.uct2015L1Extra  = cms.Sequence(process.gtEvmDigis+process.dttfDigis+process.csctfDigis+process.uctGctDigis+process.gtUCTDigis+process.scalersRawToDigi+
                                       process.l1extraParticles+process.l1extraParticlesUCT)

process.createGenParticlesEle = cms.EDProducer("FilterGenParticles",
    MaxEtaThreshold = cms.untracked.double(2.5),
    GenLevelSelect = cms.untracked.int32(11),
    MinPtThreshold = cms.untracked.double(5))


process.rlxEGEfficiency = cms.EDAnalyzer("EfficiencyExample",
    maxDR = cms.double(0.5), --> matching treshold
    recoSrc = cms.VInputTag("createGenParticlesEle"), -> match with GenParticles
    l1Src = cms.VInputTag(cms.InputTag("l1extraParticlesUCT","NonIsolated"), cms.InputTag("l1extraParticlesUCT","Isolated")), -> l1extraParticles are used
    pvSrc = cms.InputTag("offlinePrimaryVertices")) -> offlinePrimaryVertex ->GEN-SIM-RAW-RECO

process.isoEGEfficiency = cms.EDAnalyzer("EfficiencyExample",
    maxDR = cms.double(0.5),
    recoSrc = cms.VInputTag("createGenParticlesEle"),
    l1Src = cms.VInputTag(cms.InputTag("l1extraParticlesUCT","Isolated")),
    pvSrc = cms.InputTag("offlinePrimaryVertices")
)


EfficiencyExample -> EDAnalyzer in which L1 electron/gamma informations and reco (GenParticles in case of MC) ones are stored.

##################
To make the plots:
###################

plotEfficienciesBasic.C

take both Iso+NonIso and Iso output trees which are in the same root file.

Numertator: ntuple->Draw("ptReco[0]>>EGEffi(20,0,100)","ptL1[0]>25&&abs(etaReco[0])<2.5");
Denominator: ntuple->Draw("ptReco[0]>>Den(20,0,100)","abs(etaReco[0])<2.5");
 
This is the end.

Samples:

WtoEnu_Tune4C_13TeV-pythia8	PU40BX50
WToENu_Tune4C_13TeV-pythia8	PU40BX25

