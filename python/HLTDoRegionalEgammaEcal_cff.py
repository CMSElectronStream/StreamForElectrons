import FWCore.ParameterSet.Config as cms


hltESPESUnpackerWorker = cms.ESProducer( "ESUnpackerWorkerESProducer",
      RHAlgo = cms.PSet(
       ESRecoAlgo = cms.int32( 0 ),
       Type = cms.string( "ESRecHitWorker" )),
      DCCDataUnpacker = cms.PSet(  LookupTable = cms.FileInPath( "EventFilter/ESDigiToRaw/data/ES_lookup_table.dat" ) ),
      ComponentName = cms.string( "hltESPESUnpackerWorker" ))

hltESRawToRecHitFacility = cms.EDProducer( "EcalRawToRecHitFacility",
      sourceTag = cms.InputTag( "rawDataCollector" ),
      workerName = cms.string( "hltESPESUnpackerWorker" ))

hltEcalRawToRecHitFacility = cms.EDProducer( "EcalRawToRecHitFacility",
      sourceTag = cms.InputTag( "rawDataCollector" ),
      workerName = cms.string( "" ))

hltEcalRegionalEgammaFEDs = cms.EDProducer("EcalRawToRecHitRoI",
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

hltEcalRegionalEgammaRecHit = cms.EDProducer( "EcalRawToRecHitProducer",
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


hltESRegionalEgammaRecHit = cms.EDProducer( "EcalRawToRecHitProducer",
        splitOutput = cms.bool( False ),
        rechitCollection = cms.string( "EcalRecHitsES" ),
        EErechitCollection = cms.string( "" ),
        EBrechitCollection = cms.string( "" ),
        sourceTag = cms.InputTag( 'hltEcalRegionalEgammaFEDs','es' ),
        cleaningConfig = cms.PSet(  ),
        lazyGetterTag = cms.InputTag( "hltESRawToRecHitFacility" ))

### EGamma Ecal Rec Hit Sequence
HLTDoRegionalEgammaEcalSequence = cms.Sequence( hltESRawToRecHitFacility +
                                                hltEcalRawToRecHitFacility +
                                                hltEcalRegionalEgammaFEDs +
                                                hltEcalRegionalEgammaRecHit +
                                                hltESRegionalEgammaRecHit )

hltEcalRegionalRestFEDs = cms.EDProducer( "EcalRawToRecHitRoI",
  JetJobPSet = cms.VPSet(),
  sourceTag_es = cms.InputTag( "NotNeededoESfalse" ),
  doES = cms.bool( False ),
  type = cms.string( "all" ),
  sourceTag = cms.InputTag( "hltEcalRawToRecHitFacility" ),
  EmJobPSet = cms.VPSet(),
  CandJobPSet = cms.VPSet(),
  MuonJobPSet = cms.PSet(  ),
  esInstance = cms.untracked.string( "es" ),
  MuJobPSet = cms.PSet(  ))

hltEcalRecHitAll = hltEcalRegionalEgammaRecHit.clone()
hltEcalRecHitAll.souceTag = cms.InputTag("hltEcalRegionalRestFEDs")

hltEcalRegionalESRestFEDs = cms.EDProducer( "EcalRawToRecHitRoI",
    JetJobPSet = cms.VPSet(),
    sourceTag_es = cms.InputTag( "hltESRawToRecHitFacility" ),
    doES = cms.bool( True ),
    type = cms.string( "all" ),
    sourceTag = cms.InputTag( "hltEcalRawToRecHitFacility" ),
    EmJobPSet = cms.VPSet(),
    CandJobPSet = cms.VPSet(),
    MuonJobPSet = cms.PSet(),
    esInstance = cms.untracked.string( "es" ),
    MuJobPSet = cms.PSet())

hltESRecHitAll = cms.EDProducer( "EcalRawToRecHitProducer",
    splitOutput = cms.bool( False ),
    rechitCollection = cms.string( "EcalRecHitsES" ),
    EErechitCollection = cms.string( "" ),
    EBrechitCollection = cms.string( "" ),
    sourceTag = cms.InputTag( 'hltEcalRegionalESRestFEDs','es' ),
    cleaningConfig = cms.PSet(  ),
    lazyGetterTag = cms.InputTag( "hltESRawToRecHitFacility" ))
