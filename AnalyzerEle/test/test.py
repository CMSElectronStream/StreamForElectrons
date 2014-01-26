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
     electronCollection = cms.InputTag("gsfElectrons")
)



process.p = cms.Path(process.Analyzer)

