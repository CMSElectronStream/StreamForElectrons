import FWCore.ParameterSet.Config as cms
from FWCore.ParameterSet.VarParsing import VarParsing

####### option parsing
options = VarParsing ('AnalyzerEle')

options.register ('hltPath','HLT_GsfEle25_WP80_PFMET_MT50_v9',VarParsing.multiplicity.singleton,VarParsing.varType.string,'in order to match trigger electrons with pat ones')

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
process.PatElectronTriggerMatchHLTEle.matchedCuts = cms.string('path('+options.hltPath+')')

####### call the final analyzer
process.load('StreamForElectrons.AnalyzerEle.ntupleAnalyzer_cfi')

### final path 
process.path = cms.Path(process.patElectronSequence*
                        process.Analyzer)

process.schedule = cms.Schedule(process.path)

