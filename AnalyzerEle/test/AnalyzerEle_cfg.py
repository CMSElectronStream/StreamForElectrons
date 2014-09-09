import FWCore.ParameterSet.Config as cms
import pprint
import sys

####### option parsing
from FWCore.ParameterSet.VarParsing import VarParsing
options = VarParsing ('python');

options.register ('hltPath','HLT_Ele27_WP80_Gsf_v1',VarParsing.multiplicity.singleton,VarParsing.varType.string,
                  'in order to match trigger electrons with pat ones');
options.register ('isAlcaStreamOutput',0,VarParsing.multiplicity.singleton,VarParsing.varType.int,
                  'to set properly the input collection for the analyzer');
options.register ('skipAnalyzerAndDumpOutput', False, VarParsing.multiplicity.singleton, VarParsing.varType.int,
                  "true if you don't want to run the analyzer but dump a output file with all the collections keep*")
options.register ('usePatElectronsTriggerMatch',False,VarParsing.multiplicity.singleton, VarParsing.varType.int,
                  "true if you want to match trigger electron with the pat one and use this collection as input in the analyzer") 
options.register ('applyWZSelections',False,VarParsing.multiplicity.singleton, VarParsing.varType.int,
                  "options in order to apply WZ selection topology")
options.register ('applyElectronID',False,VarParsing.multiplicity.singleton, VarParsing.varType.int,
                  "options in order to apply electron ID WP80 on the whole electron collection")
options.parseArguments();
print options;


####### define process
process = cms.Process("SimpleAnalyzer");

####### define modules
process.load("FWCore.MessageService.MessageLogger_cfi");
process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.GeometryDB_cff')
process.load('Configuration.EventContent.EventContent_cff')
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.source = cms.Source ("PoolSource",fileNames = cms.untracked.vstring (options.inputFiles));

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(options.maxEvents));


process.GlobalTag.globaltag = 'GR_R_71_V4::All';

process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))

####### define output file
if not options.skipAnalyzerAndDumpOutput :
 process.TFileService = cms.Service("TFileService", 
                       fileName = cms.string(options.outputFile),
                       closeFileFast = cms.untracked.bool(True));
else:
 process.output = cms.OutputModule("PoolOutputModule",
                                    fileName = cms.untracked.string(options.outputFile),
                                    outputCommands = cms.untracked.vstring('keep *'),
                                    dropMetaData   = cms.untracked.string('ALL'))

####### sequence for doing the pfIsolation --> fix for 6_2_2
process.load('CommonTools.ParticleFlow.pfParticleSelection_cff')
process.pfPileUpIso.PFCandidates = cms.InputTag("particleFlowPtrs")
process.pfNoPileUpIso.bottomCollection = cms.InputTag("particleFlowPtrs")
process.pfParticleSelectionSequence.remove(process.pfNoPileUpSequence)

process.load('CommonTools.ParticleFlow.Isolation.pfElectronIsolation_cff')
process.elPFIsoDepositCharged.src    = cms.InputTag("gedGsfElectrons")
process.elPFIsoDepositChargedAll.src = cms.InputTag("gedGsfElectrons")
process.elPFIsoDepositGamma.src      = cms.InputTag("gedGsfElectrons")
process.elPFIsoDepositNeutral.src    = cms.InputTag("gedGsfElectrons")
process.elPFIsoDepositPU.src         = cms.InputTag("gedGsfElectrons")   

process.IsolationSequence = cms.Sequence(process.pfParticleSelectionSequence*
                                         process.pfElectronIsolationSequence)


####### the electron PAT sequence + embending of electron ID informations:
process.load('StreamForElectrons.AnalyzerEle.patElectronSequence_cff')

if options.isAlcaStreamOutput != 0 :
    process.patElectrons.pvSrc = cms.InputTag("offlinePrimaryVerticesWithBS");
else:
    process.eleSelectionProducers.rhoFastJet =  cms.InputTag("fixedGridRhoAll");    
    process.eleSelectionProducers.vertexCollection = cms.InputTag("offlinePrimaryVerticesWithBS");
    
process.PatElectronTriggerMatchHLTEle.matchedCuts = cms.string('path("'+options.hltPath+'")');
####### call the final analyzer
process.load('StreamForElectrons.AnalyzerEle.ntupleAnalyzer_cfi')
process.Analyzer.hltPaths = cms.vstring(options.hltPath)

if options.isAlcaStreamOutput != 0 :
 if options.usePatElectronsTriggerMatch :
     process.Analyzer.EleTag = cms.InputTag("PatElectronsTriggerMatch")
 process.Analyzer.PVTag    = cms.InputTag("offlinePrimaryVerticesWithBS");
 process.Analyzer.PVTag_alternative   = cms.InputTag("offlinePrimaryVerticesWithBS");
 process.Analyzer.PFMetTag = cms.InputTag("pfMet");
 process.Analyzer.rhoTag   = cms.InputTag("fixedGridRhoAll");
 process.Analyzer.triggerResultsCollection = cms.InputTag('TriggerResults::TEST');
 if options.applyWZSelections:
    process.Analyzer.doWZSelection = cms.untracked.bool(True);
 else:
    process.Analyzer.doWZSelection = cms.untracked.bool(False);
 if options.applyElectronID:
    process.Analyzer.applyElectronID = cms.untracked.bool(True);
 else:
    process.Analyzer.applyElectronID = cms.untracked.bool(False);
     
 process.Analyzer.saveMCInfo    = cms.untracked.bool(False);
else:
 if options.usePatElectronsTriggerMatch :
     process.Analyzer.EleTag = cms.InputTag("PatElectronsTriggerMatch")
 process.Analyzer.PVTag    = cms.InputTag("offlinePrimaryVerticesWithBS");
 process.Analyzer.PVTag_alternative   = cms.InputTag("offlinePrimaryVerticesWithBS");
 process.Analyzer.PFMetTag = cms.InputTag("pfMet");
 process.Analyzer.rhoTag   = cms.InputTag("fixedGridRhoAll");   
 process.Analyzer.triggerResultsCollection = cms.InputTag('TriggerResults::TEST')
 if options.applyWZSelections:
    process.Analyzer.doWZSelection = cms.untracked.bool(True);
 else:
    process.Analyzer.doWZSelection = cms.untracked.bool(False);
 if options.applyElectronID:
    process.Analyzer.applyElectronID = cms.untracked.bool(True);
 else:
    process.Analyzer.applyElectronID = cms.untracked.bool(False);
 process.Analyzer.saveMCInfo    = cms.untracked.bool(False);

### final path
if not options.skipAnalyzerAndDumpOutput: 
 process.path = cms.Path(process.IsolationSequence*
                         process.patElectronSequence*
                         process.Analyzer)
 
else:
 process.path = cms.Path(process.IsolationSequence*
                         process.patElectronSequence)

 process.EndPath = cms.EndPath(process.output)

 process.schedule = cms.Schedule(process.path,process.EndPath)
     

############################
## Dump the output Python ##
############################

processDumpFile = open('processDump.py', 'w')
print >> processDumpFile, process.dumpPython()
