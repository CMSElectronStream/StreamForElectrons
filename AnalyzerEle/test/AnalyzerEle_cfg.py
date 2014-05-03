import FWCore.ParameterSet.Config as cms
import pprint
import sys

####### option parsing
from FWCore.ParameterSet.VarParsing import VarParsing
options = VarParsing ('python');

options.register ('hltPath','HLT_GsfEle25_WP80_PFMET_MT50_v9',VarParsing.multiplicity.singleton,VarParsing.varType.string,'in order to match trigger electrons with pat ones');
options.register ('isAlcaStreamOutput',0,VarParsing.multiplicity.singleton,VarParsing.varType.int,'to set properly the input collection for the analyzer');

options.parseArguments();
print options;


####### define process
process = cms.Process("SimpleAnalyzer");

####### define modules
process.load("FWCore.MessageService.MessageLogger_cfi");

process.source = cms.Source ("PoolSource",fileNames = cms.untracked.vstring (options.inputFiles));

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(options.maxEvents));

####### define generic configuration
process.load('Configuration.StandardSequences.GeometryDB_cff')  # fix missing ESSource e.g. "TrackerDigiGeometryRecord"
process.load("Configuration.StandardSequences.MagneticField_cff")  # missing "IdealMagneticFieldRecord"
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")

process.GlobalTag.globaltag = 'GR_R_62_V1::All';

####### define output file
process.TFileService = cms.Service("TFileService", 
      fileName = cms.string(options.outputFile),
      closeFileFast = cms.untracked.bool(True));

####### call the electron PAT sequence + embending of electron ID informations:
process.load('StreamForElectrons.AnalyzerEle.patElectronSequence_cff')

if options.isAlcaStreamOutput != 0 :
    process.patElectrons.pvSrc = cms.InputTag("hltFastPVPixelVertices");
else:
    process.eleSelectionProducers.rhoFastJet =  cms.InputTag("kt6PFJets","rho");    
    process.eleSelectionProducers.vertexCollection = cms.InputTag("offlinePrimaryVerticesWithBS");
    
process.PatElectronTriggerMatchHLTEle.matchedCuts = cms.string('path('+options.hltPath+')');
####### call the final analyzer
process.load('StreamForElectrons.AnalyzerEle.ntupleAnalyzer_cfi')
if options.isAlcaStreamOutput != 0 :
 process.Analyzer.PVTag    = cms.InputTag("hltFastPVPixelVertices");
 process.Analyzer.PFMetTag = cms.InputTag("hltPFMETProducer");
 process.Analyzer.rhoTag   = cms.InputTag("hltKT6PFJets","rho");
 process.Analyzer.triggerResultsCollection = cms.InputTag('TriggerResults::HLT');
 process.Analyzer.doWZSelection = cms.bool(False);
 process.Analyzer.saveMCInfo    = cms.bool(False);
else:
 process.Analyzer.PVTag    = cms.InputTag("offlinePrimaryVerticesWithBS");
 process.Analyzer.PFMetTag = cms.InputTag("pfMet");
 process.Analyzer.rhoTag   = cms.InputTag("kt6PFJets","rho");   
 process.Analyzer.triggerResultsCollection = cms.InputTag('TriggerResults::HLT')
 process.Analyzer.doWZSelection = cms.bool(False);
 process.Analyzer.saveMCInfo    = cms.bool(False);

### final path 
process.path = cms.Path(process.patElectronSequence*
                        process.Analyzer)

process.schedule = cms.Schedule(process.path)

############################
## Dump the output Python ##
############################

processDumpFile = open('processDump.py', 'w')
print >> processDumpFile, process.dumpPython()
