# /online/collisions/2012/8e33/v2.2/HLT/V7 (CMSSW_6_2_0_pre6_HLT2)
import FWCore.ParameterSet.Config as cms
import pprint
import sys

process = cms.Process( "HLT8E33v2" )

process.HLTConfigVersion = cms.PSet(
  tableName = cms.string('/online/collisions/2012/8e33/v2.2/HLT/V7')
)

## streams
process.streams = cms.PSet( 
  ALCAECALELE = cms.vstring( 'AlCaECALEle' ),
)

## dataset
process.datasets = cms.PSet( 
  AlCaECALEle = cms.vstring( 'AlCa_EcalEle_v1' )
)

## ES-Sources
process.load("HLTrigger.Configuration.SourceHLT_cff")

## ES-Producers
process.load("HLTrigger.Configuration.ESProducerHLT_cff")

## Service

process.FastTimerService = cms.Service( "FastTimerService",
    dqmPath = cms.untracked.string( "HLT/TimerService" ),
    dqmModuleTimeRange = cms.untracked.double( 40.0 ),
    luminosityProduct = cms.untracked.InputTag( "hltScalersRawToDigi" ),
    enableTimingExclusive = cms.untracked.bool( False ),
    enableTimingModules = cms.untracked.bool( True ),
    enableDQMbyPathOverhead = cms.untracked.bool( False ),
    dqmTimeResolution = cms.untracked.double( 5.0 ),
    enableDQMbyModule = cms.untracked.bool( False ),
    dqmLuminosityResolution = cms.untracked.double( 1.0E31 ),
    skipFirstPath = cms.untracked.bool( False ),
    enableTimingPaths = cms.untracked.bool( True ),
    enableDQMbyLumiSection = cms.untracked.bool( True ),
    dqmPathTimeResolution = cms.untracked.double( 0.5 ),
    dqmPathTimeRange = cms.untracked.double( 100.0 ),
    dqmTimeRange = cms.untracked.double( 1000.0 ),
    dqmLumiSectionsRange = cms.untracked.uint32( 2500 ),
    enableDQMSummary = cms.untracked.bool( True ),
    enableTimingSummary = cms.untracked.bool( False ),
    enableDQMbyPathTotal = cms.untracked.bool( False ),
    useRealTimeClock = cms.untracked.bool( True ),
    enableDQMbyPathExclusive = cms.untracked.bool( False ),
    enableDQMbyLuminosity = cms.untracked.bool( True ),
    enableDQM = cms.untracked.bool( True ),
    supportedProcesses = cms.untracked.vuint32( 8, 12, 16, 24, 32 ),
    dqmModuleTimeResolution = cms.untracked.double( 0.2 ),
    dqmLuminosityRange = cms.untracked.double( 1.0E34 ),
    enableDQMbyPathActive = cms.untracked.bool( False ),
    enableDQMbyPathDetails = cms.untracked.bool( False ),
    enableDQMbyProcesses = cms.untracked.bool( True ),
    enableDQMbyPathCounters = cms.untracked.bool( False ),
    enableDQMbyModuleType = cms.untracked.bool( False )
)
process.DQM = cms.Service( "DQM",
    publishFrequency = cms.untracked.double( 5.0 ),
    debug = cms.untracked.bool( False ),
    collectorPort = cms.untracked.int32( 0 ),
    collectorHost = cms.untracked.string( "" )
)
process.DQMStore = cms.Service( "DQMStore",
    verboseQT = cms.untracked.int32( 0 ),
    verbose = cms.untracked.int32( 0 ),
    collateHistograms = cms.untracked.bool( False )
)
process.DTDataIntegrityTask = cms.Service( "DTDataIntegrityTask",
    processingMode = cms.untracked.string( "HLT" ),
    fedIntegrityFolder = cms.untracked.string( "DT/FEDIntegrity_EvF" ),
    getSCInfo = cms.untracked.bool( True )
)
process.MessageLogger = cms.Service( "MessageLogger",
    suppressInfo = cms.untracked.vstring(  ),
    debugs = cms.untracked.PSet( 
      threshold = cms.untracked.string( "INFO" ),
      placeholder = cms.untracked.bool( True ),
      suppressInfo = cms.untracked.vstring(  ),
      suppressWarning = cms.untracked.vstring(  ),
      suppressDebug = cms.untracked.vstring(  ),
      suppressError = cms.untracked.vstring(  )
    ),
    suppressDebug = cms.untracked.vstring(  ),
    cout = cms.untracked.PSet( 
      threshold = cms.untracked.string( "ERROR" ),
      suppressInfo = cms.untracked.vstring(  ),
      suppressWarning = cms.untracked.vstring(  ),
      suppressDebug = cms.untracked.vstring(  ),
      suppressError = cms.untracked.vstring(  )
    ),
    cerr_stats = cms.untracked.PSet( 
      threshold = cms.untracked.string( "WARNING" ),
      output = cms.untracked.string( "cerr" ),
      optionalPSet = cms.untracked.bool( True )
    ),
    warnings = cms.untracked.PSet( 
      threshold = cms.untracked.string( "INFO" ),
      placeholder = cms.untracked.bool( True ),
      suppressInfo = cms.untracked.vstring(  ),
      suppressWarning = cms.untracked.vstring(  ),
      suppressDebug = cms.untracked.vstring(  ),
      suppressError = cms.untracked.vstring(  )
    ),
    statistics = cms.untracked.vstring( 'cerr' ),
    cerr = cms.untracked.PSet( 
      INFO = cms.untracked.PSet(  limit = cms.untracked.int32( 0 ) ),
      noTimeStamps = cms.untracked.bool( False ),
      FwkReport = cms.untracked.PSet( 
        reportEvery = cms.untracked.int32( 1 ),
        limit = cms.untracked.int32( 0 )
      ),
      default = cms.untracked.PSet(  limit = cms.untracked.int32( 10000000 ) ),
      Root_NoDictionary = cms.untracked.PSet(  limit = cms.untracked.int32( 0 ) ),
      FwkJob = cms.untracked.PSet(  limit = cms.untracked.int32( 0 ) ),
      FwkSummary = cms.untracked.PSet( 
        reportEvery = cms.untracked.int32( 1 ),
        limit = cms.untracked.int32( 10000000 )
      ),
      threshold = cms.untracked.string( "INFO" ),
      suppressInfo = cms.untracked.vstring(  ),
      suppressWarning = cms.untracked.vstring(  ),
      suppressDebug = cms.untracked.vstring(  ),
      suppressError = cms.untracked.vstring(  )
    ),
    FrameworkJobReport = cms.untracked.PSet( 
      default = cms.untracked.PSet(  limit = cms.untracked.int32( 0 ) ),
      FwkJob = cms.untracked.PSet(  limit = cms.untracked.int32( 10000000 ) )
    ),
    suppressWarning = cms.untracked.vstring( 'hltOnlineBeamSpot',
      'hltCtf3HitL1SeededWithMaterialTracks',
      'hltL3MuonsOIState',
      'hltPixelTracksForHighMult',
      'hltHITPixelTracksHE',
      'hltHITPixelTracksHB',
      'hltCtfL1SeededWithMaterialTracks',
      'hltRegionalTracksForL3MuonIsolation',
      'hltSiPixelClusters',
      'hltActivityStartUpElectronPixelSeeds',
      'hltLightPFTracks',
      'hltPixelVertices3DbbPhi',
      'hltL3MuonsIOHit',
      'hltPixelTracks',
      'hltSiPixelDigis',
      'hltL3MuonsOIHit',
      'hltL1SeededElectronGsfTracks',
      'hltL1SeededStartUpElectronPixelSeeds',
      'hltBLifetimeRegionalCtfWithMaterialTracksbbPhiL1FastJetFastPV',
      'hltCtfActivityWithMaterialTracks' ),
    errors = cms.untracked.PSet( 
      threshold = cms.untracked.string( "INFO" ),
      placeholder = cms.untracked.bool( True ),
      suppressInfo = cms.untracked.vstring(  ),
      suppressWarning = cms.untracked.vstring(  ),
      suppressDebug = cms.untracked.vstring(  ),
      suppressError = cms.untracked.vstring(  )
    ),
    fwkJobReports = cms.untracked.vstring( 'FrameworkJobReport' ),
    debugModules = cms.untracked.vstring(  ),
    infos = cms.untracked.PSet( 
      threshold = cms.untracked.string( "INFO" ),
      Root_NoDictionary = cms.untracked.PSet(  limit = cms.untracked.int32( 0 ) ),
      placeholder = cms.untracked.bool( True ),
      suppressInfo = cms.untracked.vstring(  ),
      suppressWarning = cms.untracked.vstring(  ),
      suppressDebug = cms.untracked.vstring(  ),
      suppressError = cms.untracked.vstring(  )
    ),
    categories = cms.untracked.vstring( 'FwkJob',
      'FwkReport',
      'FwkSummary',
      'Root_NoDictionary' ),
    destinations = cms.untracked.vstring( 'warnings',
      'errors',
      'infos',
      'debugs',
      'cout',
      'cerr' ),
    threshold = cms.untracked.string( "INFO" ),
    suppressError = cms.untracked.vstring( 'hltOnlineBeamSpot',
      'hltL3MuonCandidates',
      'hltL3TkTracksFromL2OIState',
      'hltPFJetCtfWithMaterialTracks',
      'hltL3TkTracksFromL2IOHit',
      'hltL3TkTracksFromL2OIHit' )
)
process.MicroStateService = cms.Service( "MicroStateService",
)
process.ModuleWebRegistry = cms.Service( "ModuleWebRegistry",
)

## Prescale

process.PrescaleService = cms.Service( "PrescaleService",
    forceDefault = cms.bool( False ),
    prescaleTable = cms.VPSet( *(
      cms.PSet(  pathName = cms.string( "HLT_Ele25_WP70_v13" ),
        prescales = cms.vuint32( 1, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_Ele25_WP80_v13" ),
        prescales = cms.vuint32( 1, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_Ele25_WP90_v13" ),
        prescales = cms.vuint32( 1, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_Ele25_WP70_PFMET_MT50_v9" ),
        prescales = cms.vuint32( 1, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_Ele25_WP80_PFMET_MT50_v9" ),
        prescales = cms.vuint32( 1, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_Ele25_WP90_PFMET_MT50_v9" ),
        prescales = cms.vuint32( 1, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_DoubleEle10_WP95_v1" ),
        prescales = cms.vuint32( 1, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_Ele15_Ele_10_WP95_v1" ),
        prescales = cms.vuint32( 1, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_DoubleEle15_WP95_v1" ),
        prescales = cms.vuint32( 2, 0, 2, 2, 2, 2, 32, 80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_DoubleEle10_WP90_v1" ),
        prescales = cms.vuint32( 1, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_Ele15_Ele_10_WP90_v1" ),
        prescales = cms.vuint32( 1, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
      ),
      cms.PSet(  pathName = cms.string( "HLT_DoubleEle15_WP90_v1" ),
        prescales = cms.vuint32( 2, 0, 2, 2, 2, 2, 32, 80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 )
      ),
) ),
    lvl1DefaultLabel = cms.string( "3e33" ),
    lvl1Labels = cms.vstring( '9e33nopark',
      'Half Rate',
      '8e33nopark',
      '8e33',
      '7e33',
      '6e33',
      '4e33',
      '2e33',
      '5e32',
      '6000Hz',
      '5000Hz',
      '4000Hz',
      '3000Hz',
      '2000Hz',
      '1500Hz',
      '1000Hz',
      '500Hz',
      'EM1',
      'EM2',
      'PAPilot8Bunches',
      'PAEM',
      'PA2MHz',
      'PA1600kHz',
      'PA1100kHz',
      'PA750kHz',
      'PA500kHz',
      'PA300kHz',
      'PA200kHz',
      'PA140kHz',
      'PA90kHz',
      'PA60kHz',
      'PA40kHz',
      'PP4MHz',
      'PAPilot8BunchesEM',
      'PAPilot4Bunches',
      'PAPilot4BunchesEM',
      'CirculatingBeam',
      'CirculatingBeam+HighRandom' )
)
process.UpdaterService = cms.Service( "UpdaterService",
)

## EDProducers --> remove unuseful producer

process.hltGetConditions = cms.EDAnalyzer( "EventSetupRecordDataGetter",
                                               toGet = cms.VPSet(),
                                               verbose = cms.untracked.bool( False ))
process.hltGetRaw = cms.EDAnalyzer( "HLTGetRaw",
                                     RawDataCollection = cms.InputTag( "rawDataCollector" ))

process.hltBoolFalse = cms.EDFilter( "HLTBool",
                                      result = cms.bool( False ))

process.hltTriggerType = cms.EDFilter("HLTTriggerTypeFilter",
                                       SelectedTriggerType = cms.int32( 1 ))

process.hltGtDigis = cms.EDProducer( "L1GlobalTriggerRawToDigi",
                                      DaqGtFedId = cms.untracked.int32( 813 ),
                                      DaqGtInputTag = cms.InputTag( "rawDataCollector" ),
                                      UnpackBxInEvent = cms.int32( 5 ),
                                      ActiveBoardsMask = cms.uint32( 0xffff ))

process.hltGctDigis = cms.EDProducer( "GctRawToDigi",
                                       unpackSharedRegions = cms.bool( False ),
                                       numberOfGctSamplesToUnpack = cms.uint32( 1 ),
                                       verbose = cms.untracked.bool( False ),
                                       numberOfRctSamplesToUnpack = cms.uint32( 1 ),
                                       inputLabel = cms.InputTag( "rawDataCollector" ),
                                       unpackerVersion = cms.uint32( 0 ),
                                       gctFedId = cms.untracked.int32( 745 ),
                                       hltMode = cms.bool( True ))

process.hltL1GtObjectMap = cms.EDProducer( "L1GlobalTrigger",
                                            TechnicalTriggersUnprescaled = cms.bool( True ),
                                            ProduceL1GtObjectMapRecord = cms.bool( True ),
                                            AlgorithmTriggersUnmasked = cms.bool( False ),
                                            EmulateBxInEvent = cms.int32( 1 ),
                                            AlgorithmTriggersUnprescaled = cms.bool( True ),
                                            ProduceL1GtDaqRecord = cms.bool( False ),
                                            ReadTechnicalTriggerRecords = cms.bool( True ),
                                            RecordLength = cms.vint32( 3, 0 ),
                                            TechnicalTriggersUnmasked = cms.bool( False ),
                                            ProduceL1GtEvmRecord = cms.bool( False ),
                                            GmtInputTag = cms.InputTag( "hltGtDigis" ),
                                            TechnicalTriggersVetoUnmasked = cms.bool( True ),
                                            AlternativeNrBxBoardEvm = cms.uint32( 0 ),
                                            TechnicalTriggersInputTags = cms.VInputTag( 'simBscDigis' ),
                                            CastorInputTag = cms.InputTag( "castorL1Digis" ),
                                            GctInputTag = cms.InputTag( "hltGctDigis" ),
                                            AlternativeNrBxBoardDaq = cms.uint32( 0 ),
                                            WritePsbL1GtDaqRecord = cms.bool( False ),
                                            BstLengthBytes = cms.int32( -1 )
                                           )

process.hltL1extraParticles = cms.EDProducer( "L1ExtraParticlesProd",
                                               tauJetSource = cms.InputTag( 'hltGctDigis','tauJets' ),
                                               etHadSource = cms.InputTag( "hltGctDigis" ),
                                               etTotalSource = cms.InputTag( "hltGctDigis" ),
                                               centralBxOnly = cms.bool( True ),
                                               centralJetSource = cms.InputTag( 'hltGctDigis','cenJets' ),
                                               etMissSource = cms.InputTag( "hltGctDigis" ),
                                               hfRingEtSumsSource = cms.InputTag( "hltGctDigis" ),
                                               produceMuonParticles = cms.bool( True ),
                                               forwardJetSource = cms.InputTag( 'hltGctDigis','forJets' ),
                                               ignoreHtMiss = cms.bool( False ),
                                               htMissSource = cms.InputTag( "hltGctDigis" ),
                                               produceCaloParticles = cms.bool( True ),
                                               muonSource = cms.InputTag( "hltGtDigis" ),
                                               isolatedEmSource = cms.InputTag( 'hltGctDigis','isoEm' ),
                                               nonIsolatedEmSource = cms.InputTag( 'hltGctDigis','nonIsoEm' ),
                                               hfRingBitCountsSource = cms.InputTag( "hltGctDigis" )
                                              )



process.hltScalersRawToDigi = cms.EDProducer( "ScalersRawToDigi",
                                               scalersInputTag = cms.InputTag( "rawDataCollector" ))

process.hltOnlineBeamSpot = cms.EDProducer( "BeamSpotOnlineProducer",
                                             maxZ = cms.double( 40.0 ),
                                             src = cms.InputTag( "hltScalersRawToDigi" ),
                                             gtEvmLabel = cms.InputTag( "" ),
                                             changeToCMSCoordinates = cms.bool( False ),
                                             setSigmaZ = cms.double( 0.0 ),
                                             maxRadius = cms.double( 2.0 ))


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
                                                          L1TechTriggerSeeding = cms.bool( False )
                                                       )

process.hltPreEle25WP80 = cms.EDFilter( "HLTPrescaler",
                                         L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
                                         offset = cms.uint32( 0 ))

process.hltESPESUnpackerWorker = cms.ESProducer( "ESUnpackerWorkerESProducer",
                                                  RHAlgo = cms.PSet(
                                                    ESRecoAlgo = cms.int32( 0 ),
                                                    Type = cms.string( "ESRecHitWorker" )),
                                                  DCCDataUnpacker = cms.PSet(  LookupTable = cms.FileInPath( "EventFilter/ESDigiToRaw/data/ES_lookup_table.dat" ) ),
                                                  ComponentName = cms.string( "hltESPESUnpackerWorker" ))

process.hltESRawToRecHitFacility = cms.EDProducer( "EcalRawToRecHitFacility",
                                                    sourceTag = cms.InputTag( "rawDataCollector" ),
                                                    workerName = cms.string( "hltESPESUnpackerWorker" ))

process.hltEcalRawToRecHitFacility = cms.EDProducer( "EcalRawToRecHitFacility",
                                                      sourceTag = cms.InputTag( "rawDataCollector" ),
                                                      workerName = cms.string( "" ))

process.hltEcalRegionalEgammaFEDs = cms.EDProducer("EcalRawToRecHitRoI",
                                                    JetJobPSet = cms.VPSet(),
                                                    sourceTag_es = cms.InputTag( "hltESRawToRecHitFacility" ),
                                                    doES = cms.bool( True ),
                                                    type = cms.string( "egamma" ),
                                                    sourceTag = cms.InputTag( "hltEcalRawToRecHitFacility" ),
                                                    EmJobPSet = cms.VPSet(cms.PSet(regionEtaMargin = cms.double( 0.25 ),
                                                                                   regionPhiMargin = cms.double( 0.4 ),
                                                                                   Ptmin = cms.double( 5.0 ),
                                                                                   Source = cms.InputTag( 'hltL1extraParticles','Isolated' )),
                                                                            cms.PSet(regionEtaMargin = cms.double( 0.25 ),
                                                                                     regionPhiMargin = cms.double( 0.4 ),
                                                                                     Ptmin = cms.double( 5.0 ),
                                                                                     Source = cms.InputTag( 'hltL1extraParticles','NonIsolated' ))),
                                                    CandJobPSet = cms.VPSet(),
                                                    esInstance = cms.untracked.string( "es" ),
                                                    MuJobPSet = cms.PSet())

process.hltEcalRegionalEgammaRecHit = cms.EDProducer( "EcalRawToRecHitProducer",
                                                       splitOutput = cms.bool( True ),
                                                       rechitCollection = cms.string( "NotNeededsplitOutputTrue" ),
                                                       EErechitCollection = cms.string( "EcalRecHitsEE" ),
                                                       EBrechitCollection = cms.string( "EcalRecHitsEB" ),
                                                       sourceTag = cms.InputTag( "hltEcalRegionalEgammaFEDs" ),
                                                       cleaningConfig = cms.PSet(
                                                         e6e2thresh = cms.double( 0.04 ),
                                                         tightenCrack_e6e2_double = cms.double( 3.0 ),
                                                         e4e1Threshold_endcap = cms.double( 0.3 ),
                                                         tightenCrack_e4e1_single = cms.double( 3.0 ),
                                                         tightenCrack_e1_double = cms.double( 2.0 ),
                                                         cThreshold_barrel = cms.double( 4.0 ),
                                                         e4e1Threshold_barrel = cms.double( 0.08 ),
                                                         tightenCrack_e1_single = cms.double( 2.0 ),
                                                         e4e1_b_barrel = cms.double( -0.024 ),
                                                         e4e1_a_barrel = cms.double( 0.04 ),
                                                         ignoreOutOfTimeThresh = cms.double( 1.0E9 ),
                                                         cThreshold_endcap = cms.double( 15.0 ),
                                                         e4e1_b_endcap = cms.double( -0.0125 ),
                                                         e4e1_a_endcap = cms.double( 0.02 ),
                                                         cThreshold_double = cms.double( 10.0 )),
                                                      lazyGetterTag = cms.InputTag( "hltEcalRawToRecHitFacility" ))


process.hltESRegionalEgammaRecHit = cms.EDProducer( "EcalRawToRecHitProducer",
                                                     splitOutput = cms.bool( False ),
                                                     rechitCollection = cms.string( "EcalRecHitsES" ),
                                                     EErechitCollection = cms.string( "" ),
                                                     EBrechitCollection = cms.string( "" ),
                                                     sourceTag = cms.InputTag( 'hltEcalRegionalEgammaFEDs','es' ),
                                                     cleaningConfig = cms.PSet(  ),
                                                     lazyGetterTag = cms.InputTag( "hltESRawToRecHitFacility" )
                                                    )



process.hltHybridSuperClustersL1Seeded = cms.EDProducer( "EgammaHLTHybridClusterProducer",
                                                          xi = cms.double( 0.0 ),
                                                          regionEtaMargin = cms.double( 0.14 ),
                                                          regionPhiMargin = cms.double( 0.4 ),
                                                          severityRecHitThreshold = cms.double( 4.0 ),
                                                          RecHitFlagToBeExcluded = cms.vstring(  ),
                                                          ecalhitcollection = cms.string( "EcalRecHitsEB" ),
                                                          eThreshA = cms.double( 0.003 ),
                                                          basicclusterCollection = cms.string( "" ),
                                                          eThreshB = cms.double( 0.1 ),
                                                          dynamicPhiRoad = cms.bool( False ),
                                                          RecHitSeverityToBeExcluded = cms.vstring( 'kWeird' ),
                                                          l1UpperThr = cms.double( 999.0 ),
                                                          excludeFlagged = cms.bool( True ),
                                                          posCalcParameters = cms.PSet(
                                                               T0_barl = cms.double( 7.4 ),
                                                               LogWeighted = cms.bool( True ),
                                                               T0_endc = cms.double( 3.1 ),
                                                               T0_endcPresh = cms.double( 1.2 ),
                                                               W0 = cms.double( 4.2 ),
                                                               X0 = cms.double( 0.89 )),
                                                          l1LowerThr = cms.double( 5.0 ),                                                         
                                                          doIsolated = cms.bool( True ),
                                                          eseed = cms.double( 0.35 ),
                                                          ethresh = cms.double( 0.1 ),
                                                          ewing = cms.double( 0.0 ),
                                                          useEtForXi = cms.bool( True ),
                                                          step = cms.int32( 17 ),
                                                          debugLevel = cms.string( "INFO" ),
                                                          dynamicEThresh = cms.bool( False ),
                                                          l1TagIsolated = cms.InputTag( 'hltL1extraParticles','Isolated' ),
                                                          superclusterCollection = cms.string( "" ),
                                                          HybridBarrelSeedThr = cms.double( 1.5 ),
                                                          l1TagNonIsolated = cms.InputTag( 'hltL1extraParticles','NonIsolated' ),
                                                          l1LowerThrIgnoreIsolation = cms.double( 0.0 ),
                                                          ecalhitproducer = cms.InputTag( "hltEcalRegionalEgammaRecHit" )
                                                         )


process.hltCorrectedHybridSuperClustersL1Seeded = cms.EDProducer( "EgammaSCCorrectionMaker",
                                                                   corectedSuperClusterCollection = cms.string( "" ),
                                                                   sigmaElectronicNoise = cms.double( 0.03 ),
                                                                   superClusterAlgo = cms.string( "Hybrid" ),
                                                                   etThresh = cms.double( 1.0 ),
                                                                   rawSuperClusterProducer = cms.InputTag( "hltHybridSuperClustersL1Seeded" ),
                                                                   applyEnergyCorrection = cms.bool( True ),
                                                                   isl_fCorrPset = cms.PSet(  ),
                                                                   VerbosityLevel = cms.string( "ERROR" ),
                                                                   recHitProducer = cms.InputTag( 'hltEcalRegionalEgammaRecHit','EcalRecHitsEB' ),
                                                                   fix_fCorrPset = cms.PSet(  ),
                                                                   modeEE = cms.int32( 0 ),
                                                                   modeEB = cms.int32( 0 ),
                                                                   dyn_fCorrPset = cms.PSet(  ),
                                                                   energyCorrectorName = cms.string( "EcalClusterEnergyCorrectionObjectSpecific" ),
                                                                   applyLocalContCorrection = cms.bool( False ),
                                                                   localContCorrectorName = cms.string( "EcalBasicClusterLocalContCorrection" ),
                                                                   crackCorrectorName = cms.string( "EcalClusterCrackCorrection" ),
                                                                   applyCrackCorrection = cms.bool( False ),
                                                                   hyb_fCorrPset = cms.PSet(
                                                                   brLinearLowThr = cms.double( 1.1 ),
                                                                   BremVec = cms.vdouble( -0.05208, 0.1331, 0.9196, -5.735E-4, 1.343 ),
                                                                   brLinearHighThr = cms.double( 8.0 ),
                                              fEtEtaVec = cms.vdouble( 1.0012, -0.5714, 0.0, 0.0, 0.0, 0.5549, 12.74, 1.0448, 0.0, 0.0, 0.0, 0.0, 8.0, 1.023, -0.00181, 0.0, 0.0 )))



process.hltMulti5x5BasicClustersL1Seeded = cms.EDProducer( "EgammaHLTMulti5x5ClusterProducer",
                                                            l1LowerThr = cms.double( 5.0 ),
                                                            Multi5x5BarrelSeedThr = cms.double( 0.5 ),
                                                            Multi5x5EndcapSeedThr = cms.double( 0.18 ),
                                                            endcapHitCollection = cms.string( "EcalRecHitsEE" ),
                                                            barrelClusterCollection = cms.string( "notused" ),
                                                            doEndcaps = cms.bool( True ),
                                                            regionEtaMargin = cms.double( 0.3 ),
                                                            regionPhiMargin = cms.double( 0.4 ),
                                                            RecHitFlagToBeExcluded = cms.vstring(  ),
                                                            l1TagNonIsolated = cms.InputTag( 'hltL1extraParticles','NonIsolated' ),
                                                            endcapHitProducer = cms.InputTag( "hltEcalRegionalEgammaRecHit" ),
                                                            posCalcParameters = cms.PSet(
                                                                    T0_barl = cms.double( 7.4 ),
                                                                    LogWeighted = cms.bool( True ),
                                                                    T0_endc = cms.double( 3.1 ),
                                                                    T0_endcPresh = cms.double( 1.2 ),
                                                                    W0 = cms.double( 4.2 ),
                                                                    X0 = cms.double( 0.89 )),
                                                             VerbosityLevel = cms.string( "ERROR" ),
                                                             doIsolated = cms.bool( True ),
                                                             barrelHitProducer = cms.InputTag( "hltEcalRegionalEgammaRecHit" ),
                                                             l1LowerThrIgnoreIsolation = cms.double( 0.0 ),
                                                             l1TagIsolated = cms.InputTag( 'hltL1extraParticles','Isolated' ),
                                                             barrelHitCollection = cms.string( "EcalRecHitsEB" ),
                                                             doBarrel = cms.bool( False ),
                                                             endcapClusterCollection = cms.string( "multi5x5EndcapBasicClusters" ),
                                                             l1UpperThr = cms.double( 999.0 )
                                                           )

process.hltMulti5x5SuperClustersL1Seeded = cms.EDProducer( "Multi5x5SuperClusterProducer",
                                                            barrelSuperclusterCollection = cms.string( "multi5x5BarrelSuperClusters" ),
                                                            endcapEtaSearchRoad = cms.double( 0.14 ),
                                                            barrelClusterCollection = cms.string( "multi5x5BarrelBasicClusters" ),
                                                            dynamicPhiRoad = cms.bool( False ),
                                                            endcapClusterProducer = cms.string( "hltMulti5x5BasicClustersL1Seeded" ),
                                                            barrelPhiSearchRoad = cms.double( 0.8 ),
                                                            endcapPhiSearchRoad = cms.double( 0.6 ),
                                                            barrelClusterProducer = cms.string( "notused" ),
                                                            seedTransverseEnergyThreshold = cms.double( 1.0 ),
                                                            endcapSuperclusterCollection = cms.string( "multi5x5EndcapSuperClusters" ),
                                                            barrelEtaSearchRoad = cms.double( 0.06 ),
                                                            bremRecoveryPset = cms.PSet(
                                                              barrel = cms.PSet(  ),
                                                              endcap = cms.PSet(
                                                                       a = cms.double( 47.85 ),
                                                                       c = cms.double( 0.1201 ),
                                                                       b = cms.double( 108.8 )),
                                                              doEndcaps = cms.bool( True ),
                                                              doBarrel = cms.bool( False )),
                                                              doEndcaps = cms.bool( True ),
                                                              endcapClusterCollection = cms.string( "multi5x5EndcapBasicClusters" ),
                                                              doBarrel = cms.bool( False )
                                                           )

process.hltMulti5x5EndcapSuperClustersWithPreshowerL1Seeded = cms.EDProducer( "PreshowerClusterProducer",
                                                                              assocSClusterCollection = cms.string( "" ),
                                                                              preshStripEnergyCut = cms.double( 0.0 ),
                                                                              preshClusterCollectionY = cms.string( "preshowerYClusters" ),
                                                                              preshClusterCollectionX = cms.string( "preshowerXClusters" ),
                                                                              etThresh = cms.double( 5.0 ),
                                                                              preshRecHitProducer = cms.InputTag( 'hltESRegionalEgammaRecHit','EcalRecHitsES' ),
                                                                              endcapSClusterProducer = cms.InputTag( 'hltMulti5x5SuperClustersL1Seeded','multi5x5EndcapSuperClusters' ),
                                                                              preshNclust = cms.int32( 4 ),
                                                                              debugLevel = cms.string( "" ),
                                                                              preshClusterEnergyCut = cms.double( 0.0 ),
                                                                              preshSeededNstrip = cms.int32( 15 )
                                                                             )


process.hltCorrectedMulti5x5EndcapSuperClustersWithPreshowerL1Seeded = cms.EDProducer( "EgammaSCCorrectionMaker",
                                             corectedSuperClusterCollection = cms.string( "" ),
                                             sigmaElectronicNoise = cms.double( 0.15 ),
                                             superClusterAlgo = cms.string( "Multi5x5" ),
                                             etThresh = cms.double( 1.0 ),
                                             rawSuperClusterProducer = cms.InputTag( "hltMulti5x5EndcapSuperClustersWithPreshowerL1Seeded" ),
                                             applyEnergyCorrection = cms.bool( True ),
                                             isl_fCorrPset = cms.PSet(  ),
                                             VerbosityLevel = cms.string( "ERROR" ),
                                             recHitProducer = cms.InputTag( 'hltEcalRegionalEgammaRecHit','EcalRecHitsEE' ),
                                             fix_fCorrPset = cms.PSet(
                                                brLinearLowThr = cms.double( 0.6 ),
                                                fBremVec = cms.vdouble( -0.04163, 0.08552, 0.95048, -0.002308, 1.077 ),
                                                brLinearHighThr = cms.double( 6.0 ),
                                             fEtEtaVec = cms.vdouble( 0.9746, -6.512, 0.0, 0.0, 0.02771, 4.983, 0.0, 0.0, -0.007288, -0.9446, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0 )),
                                             modeEE = cms.int32( 0 ),
                                             modeEB = cms.int32( 0 ),
                                             dyn_fCorrPset = cms.PSet(  ),
                                             energyCorrectorName = cms.string( "EcalClusterEnergyCorrectionObjectSpecific" ),
                                             applyLocalContCorrection = cms.bool( False ),
                                             localContCorrectorName = cms.string( "EcalBasicClusterLocalContCorrection" ),
                                             crackCorrectorName = cms.string( "EcalClusterCrackCorrection" ),
                                             applyCrackCorrection = cms.bool( False ),
                                             hyb_fCorrPset = cms.PSet())

process.hltL1SeededRecoEcalCandidate = cms.EDProducer( "EgammaHLTRecoEcalCandidateProducers",
                                                       scIslandEndcapProducer = cms.InputTag( "hltCorrectedMulti5x5EndcapSuperClustersWithPreshowerL1Seeded" ),
                                                       scHybridBarrelProducer = cms.InputTag( "hltCorrectedHybridSuperClustersL1Seeded" ),
                                                       recoEcalCandidateCollection = cms.string( "" )
                                                       )


process.hltL1SeededHLTClusterShape = cms.EDProducer( "EgammaHLTClusterShapeProducer",
                                                      recoEcalCandidateProducer = cms.InputTag( "hltL1SeededRecoEcalCandidate" ),
                                                      ecalRechitEB = cms.InputTag( 'hltEcalRegionalEgammaRecHit','EcalRecHitsEB' ),
                                                      ecalRechitEE = cms.InputTag( 'hltEcalRegionalEgammaRecHit','EcalRecHitsEE' ),
                                                      isIeta = cms.bool( True )
                                                     )


process.hltL1SeededPhotonEcalIso = cms.EDProducer( "EgammaHLTEcalRecIsolationProducer",
                                                    etMinEndcap = cms.double( 0.11 ),
                                                    tryBoth = cms.bool( True ),
                                                    ecalBarrelRecHitProducer = cms.InputTag( "hltEcalRegionalEgammaRecHit" ),
                                                    rhoMax = cms.double( 9.9999999E7 ),
                                                    useNumCrystals = cms.bool( True ),
                                                    etMinBarrel = cms.double( -9999.0 ),
                                                    doRhoCorrection = cms.bool( False ),
                                                    eMinEndcap = cms.double( -9999.0 ),
                                                    intRadiusEndcap = cms.double( 3.0 ),
                                                    jurassicWidth = cms.double( 3.0 ),
                                                    useIsolEt = cms.bool( True ),
                                                    ecalBarrelRecHitCollection = cms.InputTag( "EcalRecHitsEB" ),
                                                    recoEcalCandidateProducer = cms.InputTag( "hltL1SeededRecoEcalCandidate" ),
                                                    eMinBarrel = cms.double( 0.095 ),
                                                    effectiveAreaEndcap = cms.double( 0.046 ),
                                                    ecalEndcapRecHitProducer = cms.InputTag( "hltEcalRegionalEgammaRecHit" ),
                                                    extRadius = cms.double( 0.3 ),
                                                    intRadiusBarrel = cms.double( 3.0 ),
                                                    subtract = cms.bool( False ),
                                                    rhoScale = cms.double( 1.0 ),
                                                    effectiveAreaBarrel = cms.double( 0.101 ),
                                                    ecalEndcapRecHitCollection = cms.InputTag( "EcalRecHitsEE" ),
                                                    rhoProducer = cms.InputTag( 'hltKT6CaloJets','rho' )
                                                   )


process.hltKT6CaloJets = cms.EDProducer( "FastjetJetProducer",
                                          Active_Area_Repeats = cms.int32( 1 ),
                                          doAreaFastjet = cms.bool( False ),
                                          voronoiRfact = cms.double( 0.9 ),
                                          maxBadHcalCells = cms.uint32( 9999999 ),
                                          doAreaDiskApprox = cms.bool( True ),
                                          maxRecoveredEcalCells = cms.uint32( 9999999 ),
                                          jetType = cms.string( "CaloJet" ),
                                          minSeed = cms.uint32( 14327 ),
                                          Ghost_EtaMax = cms.double( 5.0 ),
                                          doRhoFastjet = cms.bool( True ),
                                          jetAlgorithm = cms.string( "Kt" ),
                                          nSigmaPU = cms.double( 1.0 ),
                                          GhostArea = cms.double( 0.01 ),
                                          Rho_EtaMax = cms.double( 4.4 ),
                                          maxBadEcalCells = cms.uint32( 9999999 ),
                                          useDeterministicSeed = cms.bool( True ),
                                          doPVCorrection = cms.bool( False ),
                                          maxRecoveredHcalCells = cms.uint32( 9999999 ),
                                          rParam = cms.double( 0.6 ),
                                          maxProblematicHcalCells = cms.uint32( 9999999 ),
                                          doOutputJets = cms.bool( True ),
                                          src = cms.InputTag( "hltTowerMakerForAll" ),
                                          inputEtMin = cms.double( 0.3 ),
                                          srcPVs = cms.InputTag( "NotUsed" ),
                                          jetPtMin = cms.double( 1.0 ),
                                          radiusPU = cms.double( 0.5 ),
                                          maxProblematicEcalCells = cms.uint32( 9999999 ),
                                          doPUOffsetCorr = cms.bool( False ),
                                          inputEMin = cms.double( 0.0 ),
                                          puPtMin = cms.double( 10.0 ),
                                          subtractorName = cms.string( "" ),
                                          MinVtxNdof = cms.int32( 0 ),
                                          MaxVtxZ = cms.double( 15.0 ),
                                          UseOnlyVertexTracks = cms.bool( False ),
                                          UseOnlyOnePV = cms.bool( False ),
                                          DzTrVtxMax = cms.double( 0.0 ),
                                          sumRecHits = cms.bool( False ),
                                          DxyTrVtxMax = cms.double( 0.0 )
                                         )


process.hltTowerMakerForAll = cms.EDProducer( "CaloTowersCreator",
                                               EBSumThreshold = cms.double( 0.2 ),
                                               MomHBDepth = cms.double( 0.2 ),
                                               UseEtEBTreshold = cms.bool( False ),
                                               hfInput = cms.InputTag( "hltHfreco" ),
                                               AllowMissingInputs = cms.bool( False ),
                                               MomEEDepth = cms.double( 0.0 ),
                                               EESumThreshold = cms.double( 0.45 ),
                                               HBGrid = cms.vdouble(  ),
                                               HcalAcceptSeverityLevelForRejectedHit = cms.uint32( 9999 ),
                                               HBThreshold = cms.double( 0.7 ),
                                               EcalSeveritiesToBeUsedInBadTowers = cms.vstring(  ),
                                               UseEcalRecoveredHits = cms.bool( False ),
                                               MomConstrMethod = cms.int32( 1 ),
                                               MomHEDepth = cms.double( 0.4 ),
                                               HcalThreshold = cms.double( -1000.0 ),
                                               HF2Weights = cms.vdouble(  ),
                                               HOWeights = cms.vdouble(  ),
                                               EEGrid = cms.vdouble(  ),
                                               UseSymEBTreshold = cms.bool( False ),
                                               EEWeights = cms.vdouble(  ),
                                               EEWeight = cms.double( 1.0 ),
                                               UseHO = cms.bool( False ),
                                               HBWeights = cms.vdouble(  ),
                                               HF1Weight = cms.double( 1.0 ),
                                               HF2Grid = cms.vdouble(  ),
                                               HEDWeights = cms.vdouble(  ),
                                               HEDGrid = cms.vdouble(  ),
                                               EBWeight = cms.double( 1.0 ),
                                               HF1Grid = cms.vdouble(  ),
                                               EBWeights = cms.vdouble(  ),
                                               HOWeight = cms.double( 1.0E-99 ),
                                               HESWeight = cms.double( 1.0 ),
                                               HESThreshold = cms.double( 0.8 ),
                                               hbheInput = cms.InputTag( "hltHbhereco" ),
                                               HF2Weight = cms.double( 1.0 ),
                                               HF2Threshold = cms.double( 0.85 ),
                                               HcalAcceptSeverityLevel = cms.uint32( 9 ),
                                               EEThreshold = cms.double( 0.3 ),
                                               HOThresholdPlus1 = cms.double( 3.5 ),
                                               HOThresholdPlus2 = cms.double( 3.5 ),
                                               HF1Weights = cms.vdouble(  ),
                                               hoInput = cms.InputTag( "hltHoreco" ),
                                               HF1Threshold = cms.double( 0.5 ),
                                               HOThresholdMinus1 = cms.double( 3.5 ),
                                               HESGrid = cms.vdouble(  ),
                                               EcutTower = cms.double( -1000.0 ),
                                               UseRejectedRecoveredEcalHits = cms.bool( False ),
                                               UseEtEETreshold = cms.bool( False ),
                                               HESWeights = cms.vdouble(  ),
                                               EcalRecHitSeveritiesToBeExcluded = cms.vstring( 'kTime',
                                                                                               'kWeird',
                                                                                               'kBad' ),
                                               HEDWeight = cms.double( 1.0 ),
                                               UseSymEETreshold = cms.bool( False ),
                                               HEDThreshold = cms.double( 0.8 ),
                                               EBThreshold = cms.double( 0.07 ),
                                               UseRejectedHitsOnly = cms.bool( False ),
                                               UseHcalRecoveredHits = cms.bool( False ),
                                               HOThresholdMinus2 = cms.double( 3.5 ),
                                               HOThreshold0 = cms.double( 3.5 ),
                                               ecalInputs = cms.VInputTag( 'hltEcalRecHitAll:EcalRecHitsEB','hltEcalRecHitAll:EcalRecHitsEE' ),
                                               UseRejectedRecoveredHcalHits = cms.bool( False ),
                                               MomEBDepth = cms.double( 0.3 ),
                                               HBWeight = cms.double( 1.0 ),
                                               HOGrid = cms.vdouble(  ),                                             
                                               EBGrid = cms.vdouble(  )
                                              )



#### EDFilter --> remove unuseful part
process.hltEGRegionalL1SingleEG20ORL1SingleEG22 = cms.EDFilter( "HLTEgammaL1MatchFilterRegional",
                                                                 saveTags = cms.bool( False ),
                                                                 endcap_end = cms.double( 2.65 ),
                                                                 region_eta_size_ecap = cms.double( 1.0 ),
                                                                 barrel_end = cms.double( 1.4791 ),
                                                                 l1IsolatedTag = cms.InputTag( 'hltL1extraParticles','Isolated' ),
                                                                 candIsolatedTag = cms.InputTag( "hltL1SeededRecoEcalCandidate" ),
                                                                 region_phi_size = cms.double( 1.044 ),
                                                                 region_eta_size = cms.double( 0.522 ),
                                                                 L1SeedFilterTag = cms.InputTag( "hltL1sL1SingleEG20ORL1SingleEG22" ),
                                                                 ncandcut = cms.int32( 1 ),
                                                                 doIsolated = cms.bool( False ),
                                                                 candNonIsolatedTag = cms.InputTag( "" ),
                                                                 l1NonIsolatedTag = cms.InputTag( 'hltL1extraParticles','NonIsolated' )
                                                                )

process.hltEG25EtFilter = cms.EDFilter( "HLTEgammaEtFilter",
                                         saveTags = cms.bool( False ),
                                         L1NonIsoCand = cms.InputTag( "" ),
                                         relaxed = cms.untracked.bool( False ),
                                         L1IsoCand = cms.InputTag( "hltL1SeededRecoEcalCandidate" ),
                                         inputTag = cms.InputTag( "hltEGRegionalL1SingleEG20ORL1SingleEG22" ),
                                         etcutEB = cms.double( 25.0 ),
                                         ncandcut = cms.int32( 1 ),
                                         etcutEE = cms.double( 25.0 )
                                        )



process.hltEle25WP80ClusterShapeFilter = cms.EDFilter( "HLTEgammaGenericFilter",
                                                       doIsolated = cms.bool( True ),
                                                       nonIsoTag = cms.InputTag( "" ),
                                                       L1NonIsoCand = cms.InputTag( "" ),
                                                       saveTags = cms.bool( False ),
                                                       thrOverE2EB = cms.double( -1.0 ),
                                                       thrRegularEE = cms.double( 0.03 ),
                                                       thrOverEEE = cms.double( -1.0 ),
                                                       L1IsoCand = cms.InputTag( "hltL1SeededRecoEcalCandidate" ),
                                                       thrOverEEB = cms.double( -1.0 ),
                                                       thrRegularEB = cms.double( 0.01 ),
                                                       lessThan = cms.bool( True ),
                                                       useEt = cms.bool( False ),
                                                       ncandcut = cms.int32( 1 ),
                                                       isoTag = cms.InputTag( "hltL1SeededHLTClusterShape" ),
                                                       candTag = cms.InputTag( "hltEG25EtFilter" ),
                                                       thrOverE2EE = cms.double( -1.0 )
                                                       )


## Sequences --> only those one that are interesting in an electron strem

### Unpacker Sequence: RawToDigis Producer --> in a RAW file FEDRawDataCollection is the only collection produced by the rawDataCollector producer; the Digis @ HLT starts from that collection

## Link to the FEDRawData Object: http://cmslxr.fnal.gov/lxr/source/DataFormats/FEDRawData/interface/FEDRawData.h?v=CMSSW_6_2_2 --> vector of string
## Link to the unpacker class : http://cmslxr.fnal.gov/lxr/source/EventFilter/L1GlobalTriggerRawToDigi/interface/L1GlobalTriggerRawToDigi.h?v=CMSSW_6_2_2
process.HLTL1UnpackerSequence = cms.Sequence( process.hltGtDigis + process.hltGctDigis + process.hltL1GtObjectMap + process.hltL1extraParticles )
process.HLTBeamSpot = cms.Sequence( process.hltScalersRawToDigi + process.hltOnlineBeamSpot )

### first sequence of beginning: Unpacker L1 Sequence + HLT Beam spot reconstruction
process.HLTBeginSequence = cms.Sequence( process.hltTriggerType + process.HLTL1UnpackerSequence + process.HLTBeamSpot )

### EGamma Ecal Rec Hit Sequence
process.HLTDoRegionalEgammaEcalSequence = cms.Sequence( process.hltESRawToRecHitFacility +
                                                        process.hltEcalRawToRecHitFacility +
                                                        process.hltEcalRegionalEgammaFEDs +
                                                        process.hltEcalRegionalEgammaRecHit +
                                                        process.hltESRegionalEgammaRecHit )

process.HLTMulti5x5SuperClusterL1Seeded = cms.Sequence( process.hltMulti5x5BasicClustersL1Seeded +
                                                        process.hltMulti5x5SuperClustersL1Seeded +
                                                        process.hltMulti5x5EndcapSuperClustersWithPreshowerL1Seeded +
                                                        process.hltCorrectedMulti5x5EndcapSuperClustersWithPreshowerL1Seeded )

process.HLTL1SeededEcalClustersSequence = cms.Sequence( process.hltHybridSuperClustersL1Seeded +
                                                        process.hltCorrectedHybridSuperClustersL1Seeded +
                                                        process.HLTMulti5x5SuperClusterL1Seeded)


### Trigger Path for HLT Ele25 WP80
process.HLTEle25WP80Sequence = cms.Sequence( process.HLTDoRegionalEgammaEcalSequence +
                                             process.HLTL1SeededEcalClustersSequence+
                                             process.hltL1SeededRecoEcalCandidate+ 
                                             process.hltEGRegionalL1SingleEG20ORL1SingleEG22 +
                                             process.hltEG25EtFilter +
                                             process.hltL1SeededHLTClusterShape +
                                             process.hltEle25WP80ClusterShapeFilter
#                                             process.hltL1SeededPhotonEcalIso +
                                             #process.hltEle27WP80EcalIsoFilter +
                                             #process.HLTDoLocalHcalWithoutHOSequence +
                                             #process.hltL1SeededPhotonHcalForHE +
                                             #process.hltEle27WP80HEFilter +
                                             #process.hltL1SeededPhotonHcalIso +
                                             #process.hltEle27WP80HcalIsoFilter +
                                             #process.HLTDoLocalPixelSequence +
                                             #process.HLTDoLocalStripSequence +
                                             #process.hltL1SeededStartUpElectronPixelSeeds +
                                             #process.htEle27WP80PixelMatchFilter +
                                             #process.hltCkfL1SeededTrackCandidates +
                                             #process.hltCtfL1SeededWithMaterialTracks +
                                             #process.hltEleAnyWP80BarrelTracks +
                                             #process.hltCtfL1SeededWithMaterialCleanTracks +
                                             #process.hltEleAnyWP80CleanMergedTracks +
                                             #process.hltEleAnyWP80PixelMatchElectronsL1Seeded +
                                             #process.hltEle27WP80OneOEMinusOneOPFilter +
                                             #process.hltEleAnyWP80L1SeededDetaDphi +
                                             #process.hltEle27WP80DetaFilter +
                                             #process.hltEle27WP80DphiFilter +
                                             #process.HLTL1SeededEgammaRegionalRecoTrackerSequence +
                                             #process.hltEleAnyWP80L1SeededElectronTrackIso +
                                             #process.hltEle27WP80TrackIsoFilter
                                             )



### Complete Trigger path sequence for Ele25 WP80
process.HLT_Ele25_WP80_v13 = cms.Path( process.HLTBeginSequence +
                                       process.hltL1sL1SingleEG20ORL1SingleEG22 +
                                       process.hltPreEle25WP80 +
                                       process.HLTEle25WP80Sequence
                                       #process.HLTEndSequence
                                      )


## inpute file and collection kept
process.source = cms.Source( "PoolSource",
    fileNames = cms.untracked.vstring('file:/media/DATA/CMSSWRoot/DATA2012/SingleElectron_Run2012B_RAW/B865DABE-BDA2-E111-854F-BCAEC53296F7.root'),
    secondaryFileNames = cms.untracked.vstring(),
    inputCommands = cms.untracked.vstring('keep *')
)

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
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32( 100 ))

# enable the TrigReport and TimeReport
process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool( True ))

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



process.hltGetConditions = cms.EDAnalyzer( "EventSetupRecordDataGetter",
                                            toGet = cms.VPSet(),
                                            verbose = cms.untracked.bool( False ))
