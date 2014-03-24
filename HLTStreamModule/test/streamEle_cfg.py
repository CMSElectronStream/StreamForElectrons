# /online/collisions/2012/8e33/v2.2/HLT/V7 (CMSSW_6_2_0_pre6_HLT2)
import FWCore.ParameterSet.Config as cms
import pprint
import sys

process = cms.Process( "HLT8E33v2" )

process.HLTConfigVersion = cms.PSet(tableName = cms.string('/online/collisions/2012/8e33/v2.2/HLT/V7'))

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


process.hltL1sL1DoubleEG137 = cms.EDFilter( "HLTLevel1GTSeed",
    saveTags = cms.bool( True ),
    L1SeedsLogicalExpression = cms.string( "L1_DoubleEG_13_7" ),
    L1MuonCollectionTag = cms.InputTag( "hltL1extraParticles" ),
    L1UseL1TriggerObjectMaps = cms.bool( True ),
    L1UseAliasesForSeeding = cms.bool( True ),
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    L1CollectionsTag = cms.InputTag( "hltL1extraParticles" ),
    L1NrBxInEvent = cms.int32( 3 ),
    L1GtObjectMapTag = cms.InputTag( "hltL1GtObjectMap" ),
    L1TechTriggerSeeding = cms.bool( False )
)


### PATH executed:
process.HLTriggerFirstPath = cms.Path( process.hltGetConditions + process.hltGetRaw + process.hltBoolFalse )

### Complete Trigger path sequence for Ele25 WP70
process.load('HLTrigger.Configuration.HLT_ElectronStreamOutput_cff')
process.load("HLTrigger.Configuration.HLT_Ele25_WP70_cff")

#process.HLT_Ele25_WP70_v13 = cms.Path( process.HLTBeginSequence +
#                                       process.hltL1sL1SingleEG20ORL1SingleEG22 +
#                                       process.HLTEle25WP70Sequence+
#                                       process.HLTEndSequence)

#process.HLT_GsfEle25_WP70_v13 = cms.Path( process.HLTBeginSequence +
#                                          process.hltL1sL1SingleEG20ORL1SingleEG22 +
#                                          process.HLTEle25WP70Sequence+
#                                          process.HLTEndSequence)


### Complete Trigger path sequence for Ele25 WP80
process.load("HLTrigger.Configuration.HLT_Ele25_WP80_cff")

#process.HLT_Ele25_WP80_v13 = cms.Path( process.HLTBeginSequence +
#                                       process.hltL1sL1SingleEG20ORL1SingleEG22 +
#                                       process.HLTEle25WP80Sequence+
#                                       process.HLTEndSequence)

#process.HLT_GsfEle25_WP80_v13 = cms.Path( process.HLTBeginSequence +
#                                          process.hltL1sL1SingleEG20ORL1SingleEG22 +
#                                          process.HLTGsfEle25WP80Sequence+
#                                          process.HLTEndSequence)

### Complete Trigger path sequence for Ele25 WP90
process.load("HLTrigger.Configuration.HLT_Ele25_WP90_cff")

#process.HLT_Ele25_WP90_v13 = cms.Path( process.HLTBeginSequence +
#                                       process.hltL1sL1SingleEG20ORL1SingleEG22 +
#                                       process.HLTEle25WP90Sequence+
#                                       process.HLTEndSequence)

#process.HLT_GsfEle25_WP90_v13 = cms.Path( process.HLTBeginSequence +
#                                          process.hltL1sL1SingleEG20ORL1SingleEG22 +
#                                          process.HLTGsfEle25WP90Sequence+
#                                          process.HLTEndSequence)


### complete trigger path for HLT_Ele25_WP70_PFMET_MT50
process.load("HLTrigger.Configuration.HLT_Ele25_WP70_PFMET_MT50_cff")

#process.HLT_Ele25_WP70_PFMET_MT50_v9 = cms.Path( process.HLTBeginSequence +
#                                                 process.hltL1sL1SingleEG20ORL1SingleEG22 +
#	                                         process.HLTEle25WP70PFMETMT50Sequence +
#						 process.HLTEndSequence)
	 
#process.HLT_GsfEle25_WP70_PFMET_MT50_v9 = cms.Path(process.HLTBeginSequence +
#                                                   process.hltL1sL1SingleEG20ORL1SingleEG22 +
#	                                           process.HLTGsfEle25WP70PFMETMT50Sequence +
#						   process.HLTEndSequence)

	       
	       
### complete trigger path for HLT_Ele25_WP80_PFMET_MT50
process.load("HLTrigger.Configuration.HLT_Ele25_WP80_PFMET_MT50_cff")

#process.HLT_Ele25_WP80_PFMET_MT50_v9 = cms.Path(process.HLTBeginSequence +
#                                                process.hltL1sL1SingleEG20ORL1SingleEG22 +
#	                                         process.HLTEle25WP80PFMETMT50Sequence +
#						 process.HLTEndSequence)

process.HLT_GsfEle25_WP80_PFMET_MT50_v9 = cms.Path( process.HLTBeginSequence +
                                                    process.hltL1sL1SingleEG20ORL1SingleEG22 +
	                                            process.HLTGsfEle25WP80PFMETMT50Sequence +
 	                                            process.HLTselectedElectronFEDList + 
						    process.HLTEndSequence)


process.load("HLTrigger.Configuration.HLT_Ele25_WP90_PFMET_MT50_cff")
#process.HLT_Ele25_WP90_PFMET_MT50_v9 = cms.Path( process.HLTBeginSequence +
#                                                 process.hltL1sL1SingleEG20ORL1SingleEG22 +
#						 process.HLTEle25WP90Sequence +
#	                                         process.HLTEle25WP90PFMETMT50Sequence +
#						 process.HLTEndSequence)

#process.HLT_GsfEle25_WP90_PFMET_MT50_v9 = cms.Path( process.HLTBeginSequence +
#                                                    process.hltL1sL1SingleEG20ORL1SingleEG22 +
#	                                            process.HLTGsfEle25WP90PFMETMT50Sequence +
#						    process.HLTEndSequence)

#process.load("HLTrigger.Configuration.HLT_Ele17_Ele8_WP90_v1_cff")
#process.HLT_Ele17_Ele8_WP90_v1 = cms.Path( process.HLTBeginSequence +
#	                                   process.hltL1sL1DoubleEG137 +
#                                           process.HLT_Ele17_Ele8_WP90_v1 +
#					   process.HLTEndSequence)  

#process.load("HLTrigger.Configuration.HLT_Ele17_Ele12_WP90_v1_cff")
#process.HLT_Ele17_Ele_12_WP90_v1 = cms.Path(process.HLTBeginSequence +
# 	                                    process.hltL1sL1DoubleEG137 +
#                                            process.HLT_Ele17_Ele12_WP90_v1 +
#	 				    process.HLTEndSequence)  


#process.load("HLTrigger.Configuration.HLT_DoubleEle17_WP90_v1_cff")
#process.HLT_DoubleEle17_WP90_v1 = cms.Path(process.HLTBeginSequence +
# 	                                   process.hltL1sL1DoubleEG137 +
#                                           process.HLT_DoubleEle17_WP90_v1 +
#	 				   process.HLTEndSequence)  


### latest path in common to all trigger path
process.load("HLTrigger.Configuration.HLT_LastSequences_cff")
 
process.HLT_LogMonitor_v4 = cms.Path( process.hltGtDigis + 
	                              process.hltLogMonitorFilter + 
				      process.hltPreLogMonitor + 
		                      process.HLTEndSequence )


### final HLT trigger path
process.HLTriggerFinalPath = cms.Path( process.hltGtDigis + 
	                               process.hltScalersRawToDigi + 
				       process.hltFEDSelector + 
		                       process.hltTriggerSummaryAOD + 
				       process.hltTriggerSummaryRAW )

### endpath

process.hltOutputStreamElectron.fileName = cms.untracked.string("streamElectronRAW_HCAL.root")

process.ElectronStreamOutputPath = cms.EndPath( process.hltPreOutput + 
						process.hltOutputStreamElectron)


## inpute file and collection kept
process.source = cms.Source( "PoolSource",
    fileNames = cms.untracked.vstring('file:/media/DATA/CMSSWRoot/DATA2012/SingleElectron_Run2012B_RAW/B865DABE-BDA2-E111-854F-BCAEC53296F7.root'),
    #'file:/media/DATA/CMSSWRoot/DATA2012/DoubleElectron_Run2012B-ZElectron-13Jul2012-v1_RAW-RECO/FEE043A5-93D4-E111-84DC-0030486790C0.root'),		    
    #fileNames = cms.untracked.vstring('file:/data/amassiro/CMSSWRoot/DATA2012/SingleElectron_Run2012B_RAW/B865DABE-BDA2-E111-854F-BCAEC53296F7.root'),
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
    process.hltTrigReport.HLTriggerResults                    = cms.InputTag( 'TriggerResults', '', 'HLT8E33v2' )

if 'hltPreExpressCosmicsOutputSmart' in process.__dict__:
    process.hltPreExpressCosmicsOutputSmart.TriggerResultsTag = cms.InputTag( 'TriggerResults', '', 'HLT8E33v2' )

if 'hltPreExpressOutputSmart' in process.__dict__:
    process.hltPreExpressOutputSmart.TriggerResultsTag        = cms.InputTag( 'TriggerResults', '', 'HLT8E33v2' )

if 'hltPreDQMForHIOutputSmart' in process.__dict__:
    process.hltPreDQMForHIOutputSmart.TriggerResultsTag       = cms.InputTag( 'TriggerResults', '', 'HLT8E33v2' )

if 'hltPreDQMForPPOutputSmart' in process.__dict__:
    process.hltPreDQMForPPOutputSmart.TriggerResultsTag       = cms.InputTag( 'TriggerResults', '', 'HLT8E33v2' )

if 'hltPreHLTDQMResultsOutputSmart' in process.__dict__:
    process.hltPreHLTDQMResultsOutputSmart.TriggerResultsTag  = cms.InputTag( 'TriggerResults', '', 'HLT8E33v2' )

if 'hltPreHLTDQMOutputSmart' in process.__dict__:
    process.hltPreHLTDQMOutputSmart.TriggerResultsTag         = cms.InputTag( 'TriggerResults', '', 'HLT8E33v2' )

if 'hltPreHLTMONOutputSmart' in process.__dict__:
    process.hltPreHLTMONOutputSmart.TriggerResultsTag         = cms.InputTag( 'TriggerResults', '', 'HLT8E33v2' )

if 'hltDQMHLTScalers' in process.__dict__:
    process.hltDQMHLTScalers.triggerResults                   = cms.InputTag( 'TriggerResults', '', 'HLT8E33v2' )
    process.hltDQMHLTScalers.processname                      = 'HLT8E33v2'

if 'hltDQML1SeedLogicScalers' in process.__dict__:
    process.hltDQML1SeedLogicScalers.processname              = 'HLT8E33v2'

# limit the number of events to be processed
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(3000))

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.destinations = ['cout', 'cerr']
process.MessageLogger.cerr.FwkReport.reportEvery = 1 

# enable the TrigReport and TimeReport
process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))

# override the GlobalTag, connection string and pfnPrefix
if 'GlobalTag' in process.__dict__:
    from Configuration.AlCa.GlobalTag import GlobalTag as customiseGlobalTag
    process.GlobalTag = customiseGlobalTag(process.GlobalTag, globaltag = 'auto:hltonline_8E33v2')
    process.GlobalTag.connect   = 'frontier://FrontierProd/CMS_COND_31X_GLOBALTAG'
    process.GlobalTag.pfnPrefix = cms.untracked.string('frontier://FrontierProd/')
    for pset in process.GlobalTag.toGet.value():
        pset.connect = pset.connect.value().replace('frontier://FrontierProd/', 'frontier://FrontierProd/')

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
