import FWCore.ParameterSet.Config as cms
from FWCore.ParameterSet.VarParsing import VarParsing

options = VarParsing ('analysis')

# add a list of strings for events to process
options.parseArguments()

process = cms.Process("SimpleAnalyzer")
process.source = cms.Source ("PoolSource",
      fileNames = cms.untracked.vstring (options.inputFiles),
)

process.TFileService = cms.Service("TFileService", 
      fileName = cms.string("tree.root"),
      closeFileFast = cms.untracked.bool(True)
)

process.Analyzer = cms.EDAnalyzer('AnalyzerEle',
    EleTag = cms.InputTag("gsfElectrons"),

    PVTag = cms.InputTag("offlinePrimaryVerticesWithBS"),
    recHitCollection_EB = cms.InputTag("reducedEcalRecHitsEB"),
    recHitCollection_EE = cms.InputTag("reducedEcalRecHitsEE"),
    SRFlagCollection_EB = cms.InputTag("ecalDigis"),
    SRFlagCollection_EE = cms.InputTag("ecalDigis"),
    digiCollection_EB = cms.InputTag("ecalDigis","ebDigis"),
    digiCollection_EE = cms.InputTag("ecalDigis","eeDigis"),
    theBeamSpotTag = cms.InputTag("offlineBeamSpot"),
    PFMetTag = cms.InputTag("pfMet"),
    rhoTag = cms.InputTag("kt6PFJets","rho"),
    conversionsInputTag = cms.InputTag("allConversions"),
    dataRun = cms.string("noCorrection"),
    verbosity = cms.untracked.bool(False),
    doWZSelection = cms.untracked.bool(True),
    applyCorrections = cms.untracked.bool(False),
    dataFlag = cms.untracked.bool(True),
    saveRecHitMatrix = cms.untracked.bool(False),
    saveFbrem = cms.untracked.bool(False) # set False if running on AOD
    
)



process.p = cms.Path(process.Analyzer)

