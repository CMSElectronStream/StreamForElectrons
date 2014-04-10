import FWCore.ParameterSet.Config as cms
from FWCore.ParameterSet.VarParsing import VarParsing

####### option parsing

options = VarParsing ('python')
# add a list of strings for events to process
options.parseArguments()
print options


####### define process
process = cms.Process("SimpleAnalyzer")

####### define modules
process.load("FWCore.MessageService.MessageLogger_cfi")

process.source = cms.Source ("PoolSource",
                 fileNames = cms.untracked.vstring (options.inputFiles)
)

process.maxEvents = cms.untracked.PSet(
        input = cms.untracked.int32(options.maxEvents))


####### define generic configuration
process.load('Configuration.StandardSequences.GeometryDB_cff')  # fix missing ESSource e.g. "TrackerDigiGeometryRecord"
process.load("Configuration.StandardSequences.MagneticField_cff")  # missing "IdealMagneticFieldRecord"
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")

process.GlobalTag.globaltag = 'GR_R_62_V1::All'


####### define output file
process.TFileService = cms.Service("TFileService", 
      fileName = cms.string(options.outputFile),
      closeFileFast = cms.untracked.bool(True))

####### call the electron PAT sequence + embending of electron ID informations:
process.load('StreamForElectrons.AnalyzerEle.patElectronSequence_cff')


####### call the final analyzer
#process.load('StreamForElectrons.AnalyzerEle.ntupleAnalyzer_cfi')

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
    doWZSelection = cms.untracked.bool(False),  # save *all* electrons combinations
    applyCorrections = cms.untracked.bool(False),
    dataFlag = cms.untracked.bool(True),
    saveRecHitMatrix = cms.untracked.bool(False),
    saveFbrem = cms.untracked.bool(False) # set False if running on AOD
)

### final path 
process.p = cms.Path(process.Analyzer)
process.schedule = cms.Schedule(process.p)

