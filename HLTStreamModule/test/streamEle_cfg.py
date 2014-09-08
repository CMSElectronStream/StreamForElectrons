# /online/collisions/2012/8e33/v2.2/HLT/V7 (CMSSW_6_2_0_pre6_HLT2)
import FWCore.ParameterSet.Config as cms
import pprint
import sys

from FWCore.ParameterSet.VarParsing import VarParsing
options = VarParsing ('python')

options.register ('saveAlcaElectronStreamOutput',True,VarParsing.multiplicity.singleton,VarParsing.varType.int,'dump the full raw collection or only the AlcaElectron Stream')

options.parseArguments()
print options

process = cms.Process( "TEST" )

process.HLTConfigVersion = cms.PSet(tableName = cms.string('/dev/CMSSW_7_1_1/HLT/V104'))

## streams
process.streams = cms.PSet(ALCAECALELE = cms.vstring( 'AlCaECALEle' ))

## dataset
process.datasets = cms.PSet(AlCaECALEle = cms.vstring( 'AlCa_EcalEle_v1' ))

## ES-Sources
process.load("HLTrigger.Configuration.SourceHLT_cff")

## ES-Producers
process.load("HLTrigger.Configuration.ESProducerHLT_cff")

## Service
process.load("HLTrigger.Configuration.ServiceHLT_cff")

### Load begin unpacker and beam spot sequences
process.load("HLTrigger.Configuration.HLTBeginSequence_cff")

process.hltL1sL1SingleEG20ORL1SingleEG22 = cms.EDFilter( "HLTLevel1GTSeed",
        saveTags = cms.bool( True ),
        L1SeedsLogicalExpression = cms.string( "L1_SingleEG20 OR L1_SingleEG22" ),
        L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" ),
        L1UseL1TriggerObjectMaps = cms.bool( True ),
        L1UseAliasesForSeeding = cms.bool( True ),
        L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
        L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
        L1NrBxInEvent = cms.int32( 3 ),
        L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
        L1TechTriggerSeeding = cms.bool( False ))

## EDProducers --> remove unuseful producer
process.hltGetConditions = cms.EDAnalyzer( "EventSetupRecordDataGetter",
        toGet = cms.VPSet(),
        verbose = cms.untracked.bool( False ))

process.hltGetRaw = cms.EDAnalyzer( "HLTGetRaw",
        RawDataCollection = cms.InputTag( "rawDataCollector" ))

process.hltBoolFalse = cms.EDFilter( "HLTBool",
        result = cms.bool( False ))

process.hltBoolEnd = cms.EDFilter( "HLTBool",
    result = cms.bool( True )
)

process.HLTEndSequence = cms.Sequence( process.hltBoolEnd )


### PATH executed:
process.HLTriggerFirstPath = cms.Path( process.hltGetConditions + process.hltGetRaw + process.hltBoolFalse )

### call the module for the electron stream output
from HLTrigger.Configuration.HLT_ElectronStreamOutput_cff import ElectronStreamOutput
ElectronStreamOutput(process,options.saveAlcaElectronStreamOutput)


### Complete Trigger path sequence for Ele27 WP80
process.load("HLTrigger.Configuration.HLT_Ele27_WP80_cff")

process.HLT_Ele27_WP80_Gsf_v1 = cms.Path( process.HLTBeginSequence +
                                          process.hltL1sL1SingleEG20ORL1SingleEG22 +
                                          process.HLTEle27WP80Sequence+
                                          process.HLTselectedElectronFEDList+
                                          process.HLTEndSequence)

if not options.saveAlcaElectronStreamOutput :
 process.HLT_Ele27_WP80_Gsf_v1.remove(process.HLTselectedElectronFEDList)

### complete trigger path for HLT_Ele27_WP80_PFMET_MT50
#process.load("HLTrigger.Configuration.HLT_Ele27_WP80_PFMET_MT50_cff")
#process.HLT_GsfEle27_WP80_PFMET_MT50_v9 = cms.Path( process.HLTBeginSequence +
#                                                    process.hltL1sL1SingleEG20ORL1SingleEG22 +
#                                                    process.HLTGsfEle27WP80PFMETMT50Sequence +
#                                                    process.HLTselectedElectronFEDList + 
#                                                    process.HLTEndSequence)



### latest path in common to all trigger path
process.load("HLTrigger.Configuration.HLT_LastSequences_cff")
 
### final HLT trigger path
process.HLTriggerFinalPath = cms.Path( process.hltGtDigis + 
                                       process.hltScalersRawToDigi + 
                                       process.hltFEDSelector + 
                                       process.hltTriggerSummaryAOD + 
                                       process.hltTriggerSummaryRAW )

### endpath
process.hltOutputStreamElectron.fileName = cms.untracked.string(options.outputFile)

process.ElectronStreamOutputPath = cms.EndPath(process.hltPreOutput + 
                                               process.hltOutputStreamElectron)


## inpute file and collection kept
process.source = cms.Source( "PoolSource",
    # high pu : https://twiki.cern.ch/twiki/bin/viewauth/CMS/TriggerStudies
    fileNames = cms.untracked.vstring(
     options.inputFiles
     
      #'file:root://xrootd.unl.edu//store/data/Run2012C/SingleElectron/RAW/v1/000/198/022/3859DED3-DDC3-E111-AAF6-001D09F24D67.root',
      
#      '/store/group/comm_trigger/TriggerStudiesGroup/Timing/run198609/run198609_lumi0050_part2.root',
#      '/store/group/comm_trigger/TriggerStudiesGroup/Timing/run198609/run198609_lumi0050_part3.root',
#      '/store/group/comm_trigger/TriggerStudiesGroup/Timing/run198609/run198609_lumi0050_part4.root',
      ),

    secondaryFileNames = cms.untracked.vstring(),
    inputCommands = cms.untracked.vstring('keep *'))

        
# Enable HF Noise filters in GRun menu
if 'hltHfreco' in process.__dict__:
    process.hltHfreco.setNoiseFlags = cms.bool( True )

# remove the HLT prescales
if 'PrescaleService' in process.__dict__:
    process.PrescaleService.lvl1DefaultLabel = cms.string( '0' )
    process.PrescaleService.lvl1Labels       = cms.vstring( '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' )
    process.PrescaleService.prescaleTable    = cms.VPSet( )

# CMSSW version specific customizations
import os
cmsswVersion = os.environ['CMSSW_VERSION']

# adapt HLT modules to the correct process name
if 'hltTrigReport' in process.__dict__:
    process.hltTrigReport.HLTriggerResults                    = cms.InputTag( 'TriggerResults', '', 'TEST' )

if 'hltPreExpressCosmicsOutputSmart' in process.__dict__:
    process.hltPreExpressCosmicsOutputSmart.TriggerResultsTag = cms.InputTag( 'TriggerResults', '', 'TEST' )

if 'hltPreExpressOutputSmart' in process.__dict__:
    process.hltPreExpressOutputSmart.TriggerResultsTag        = cms.InputTag( 'TriggerResults', '', 'TEST' )

if 'hltPreDQMForHIOutputSmart' in process.__dict__:
    process.hltPreDQMForHIOutputSmart.TriggerResultsTag       = cms.InputTag( 'TriggerResults', '', 'TEST' )

if 'hltPreDQMForPPOutputSmart' in process.__dict__:
    process.hltPreDQMForPPOutputSmart.TriggerResultsTag       = cms.InputTag( 'TriggerResults', '', 'TEST' )

if 'hltPreHLTDQMResultsOutputSmart' in process.__dict__:
    process.hltPreHLTDQMResultsOutputSmart.TriggerResultsTag  = cms.InputTag( 'TriggerResults', '', 'TEST' )

if 'hltPreHLTDQMOutputSmart' in process.__dict__:
    process.hltPreHLTDQMOutputSmart.TriggerResultsTag         = cms.InputTag( 'TriggerResults', '', 'TEST' )

if 'hltPreHLTMONOutputSmart' in process.__dict__:
    process.hltPreHLTMONOutputSmart.TriggerResultsTag         = cms.InputTag( 'TriggerResults', '', 'TEST' )

if 'hltDQMHLTScalers' in process.__dict__:
    process.hltDQMHLTScalers.triggerResults                   = cms.InputTag( 'TriggerResults', '', 'TEST' )
    process.hltDQMHLTScalers.processname                      = 'TEST'

if 'hltDQML1SeedLogicScalers' in process.__dict__:
    process.hltDQML1SeedLogicScalers.processname              = 'TEST'

# limit the number of events to be processed
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(options.maxEvents))

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.destinations = ['cout', 'cerr']
process.MessageLogger.cerr.FwkReport.reportEvery = 1 

# enable the TrigReport and TimeReport
process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))

# override the GlobalTag, connection string and pfnPrefix
if 'GlobalTag' in process.__dict__:
    from Configuration.AlCa.GlobalTag import GlobalTag as customiseGlobalTag
    process.GlobalTag = customiseGlobalTag(process.GlobalTag, globaltag = 'auto:hltonline_GRun')
    process.GlobalTag.connect   = 'frontier://FrontierProd/CMS_COND_31X_GLOBALTAG'
    process.GlobalTag.pfnPrefix = cms.untracked.string('frontier://FrontierProd/')
    for pset in process.GlobalTag.toGet.value():
        pset.connect = pset.connect.value().replace('frontier://FrontierProd/', 'frontier://FrontierProd/')
    # fix for multi-run processing                                                                                                                                                        
    process.GlobalTag.RefreshEachRun = cms.untracked.bool( True )
    process.GlobalTag.ReconnectEachRun = cms.untracked.bool( True )
    process.GlobalTag.globaltag = cms.string('GR_R_71_V5::All')
 

if 'MessageLogger' in process.__dict__:
    process.MessageLogger.categories.append('TriggerSummaryProducerAOD')
    process.MessageLogger.categories.append('L1GtTrigReport')
    process.MessageLogger.categories.append('HLTrigReport')
    process.MessageLogger.categories.append('FastReport')

############################
## Dump the output Python ##
############################

processDumpFile = open('processDump.py', 'w')
print >> processDumpFile, process.dumpPython()
