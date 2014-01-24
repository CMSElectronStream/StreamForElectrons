import FWCore.ParameterSet.Config as cms
from FWCore.ParameterSet.VarParsing import VarParsing

options = VarParsing ('analysis')

# add a list of strings for events to process
options.register ('numberOfEventsToProcess',
                  '',
                  VarParsing.multiplicity.singleton,
                  VarParsing.varType.int,
                  "Number of events to process")
options.parseArguments()

process = cms.Process("SkimEvent")
process.source = cms.Source ("PoolSource",
      fileNames = cms.untracked.vstring (options.inputFiles),
)


process.maxEvents = cms.untracked.PSet(
        input = cms.untracked.int32 (options.numberOfEventsToProcess)
)

process.Out = cms.OutputModule("PoolOutputModule",
        fileName = cms.untracked.string (options.outputFile)
)

process.end = cms.EndPath(process.Out)
