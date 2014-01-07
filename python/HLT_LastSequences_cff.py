import FWCore.ParameterSet.Config as cms

hltLogMonitorFilter = cms.EDFilter( "HLTLogMonitorFilter",
    saveTags = cms.bool( False ),
    default_threshold = cms.uint32( 10 ),
    categories = cms.VPSet( 
      cms.PSet(  name = cms.string( "TooManyTriplets" ),
        threshold = cms.uint32( 0 )
      ),
      cms.PSet(  name = cms.string( "Muon" ),
        threshold = cms.uint32( 0 )
      ),
      cms.PSet(  name = cms.string( "RecoMuon" ),
        threshold = cms.uint32( 0 )
      ),
      cms.PSet(  name = cms.string( "L3MuonCandidateProducer" ),
        threshold = cms.uint32( 0 )
      ),
      cms.PSet(  name = cms.string( "MatrixInversionFailure" ),
        threshold = cms.uint32( 0 )
      ),
      cms.PSet(  name = cms.string( "BasicTrajectoryState" ),
        threshold = cms.uint32( 0 )
      )
    )
)

hltPreLogMonitor = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    offset = cms.uint32( 0 )
)

hltFEDSelector = cms.EDProducer( "EvFFEDSelector",
    inputTag = cms.InputTag( "rawDataCollector" ),
    fedList = cms.vuint32( 1023 )
)

hltTriggerSummaryAOD = cms.EDProducer( "TriggerSummaryProducerAOD",
    processName = cms.string( "@" )
)
hltTriggerSummaryRAW = cms.EDProducer( "TriggerSummaryProducerRAW",
    processName = cms.string( "@" )
)
