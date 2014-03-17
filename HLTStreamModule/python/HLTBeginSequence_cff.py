import FWCore.ParameterSet.Config as cms

hltTriggerType = cms.EDFilter("HLTTriggerTypeFilter",
  SelectedTriggerType = cms.int32( 1 ))

hltGtDigis = cms.EDProducer( "L1GlobalTriggerRawToDigi",
  DaqGtFedId = cms.untracked.int32( 813 ),
  DaqGtInputTag = cms.InputTag( "rawDataCollector" ),
  UnpackBxInEvent = cms.int32( 5 ),
  ActiveBoardsMask = cms.uint32( 0xffff ))

hltGctDigis = cms.EDProducer( "GctRawToDigi",
  unpackSharedRegions = cms.bool( False ),
  numberOfGctSamplesToUnpack = cms.uint32( 1 ),
  verbose = cms.untracked.bool( False ),
  numberOfRctSamplesToUnpack = cms.uint32( 1 ),
  inputLabel = cms.InputTag( "rawDataCollector" ),
  unpackerVersion = cms.uint32( 0 ),
  gctFedId = cms.untracked.int32( 745 ),
  hltMode = cms.bool( True ))

hltL1GtObjectMap = cms.EDProducer( "L1GlobalTrigger",
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
  BstLengthBytes = cms.int32( -1 ))

hltL1extraParticles = cms.EDProducer( "L1ExtraParticlesProd",
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
  hfRingBitCountsSource = cms.InputTag( "hltGctDigis" ))

hltScalersRawToDigi = cms.EDProducer( "ScalersRawToDigi",
  scalersInputTag = cms.InputTag( "rawDataCollector" ))

hltOnlineBeamSpot = cms.EDProducer( "BeamSpotOnlineProducer",
  maxZ = cms.double( 40.0 ),
  src = cms.InputTag( "hltScalersRawToDigi" ),
  gtEvmLabel = cms.InputTag( "" ),
  changeToCMSCoordinates = cms.bool( False ),
  setSigmaZ = cms.double( 0.0 ),
  maxRadius = cms.double( 2.0 ))

#### final Sequence

### Unpacker Sequence: RawToDigis Producer --> in a RAW file FEDRawDataCollection is the only collection produced by the rawDataCollector 
## Link to the FEDRawData Object: http://cmslxr.fnal.gov/lxr/source/DataFormats/FEDRawData/interface/FEDRawData.h?v=CMSSW_6_2_2 --> vector of string
## Link to the unpacker class : http://cmslxr.fnal.gov/lxr/source/EventFilter/L1GlobalTriggerRawToDigi/interface/L1GlobalTriggerRawToDigi.h?v=CMSSW_6_2_2

HLTL1UnpackerSequence = cms.Sequence( hltGtDigis + hltGctDigis + hltL1GtObjectMap + hltL1extraParticles )
HLTBeamSpot           = cms.Sequence( hltScalersRawToDigi + hltOnlineBeamSpot )
HLTBeginSequence      = cms.Sequence( hltTriggerType + HLTL1UnpackerSequence + HLTBeamSpot )


