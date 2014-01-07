import FWCore.ParameterSet.Config as cms

hltHybridSuperClustersL1Seeded = cms.EDProducer( "EgammaHLTHybridClusterProducer",
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
 ecalhitproducer = cms.InputTag( "hltEcalRegionalEgammaRecHit" ))


hltCorrectedHybridSuperClustersL1Seeded = cms.EDProducer( "EgammaSCCorrectionMaker",
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


hltMulti5x5BasicClustersL1Seeded = cms.EDProducer( "EgammaHLTMulti5x5ClusterProducer",
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
  l1UpperThr = cms.double( 999.0 ))

hltMulti5x5SuperClustersL1Seeded = cms.EDProducer( "Multi5x5SuperClusterProducer",
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
  doBarrel = cms.bool( False ))

hltMulti5x5EndcapSuperClustersWithPreshowerL1Seeded = cms.EDProducer( "PreshowerClusterProducer",
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
  preshSeededNstrip = cms.int32( 15 ))

hltCorrectedMulti5x5EndcapSuperClustersWithPreshowerL1Seeded = cms.EDProducer( "EgammaSCCorrectionMaker",
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

### candidare producer HLTRecoEcalEgamma
hltL1SeededRecoEcalCandidate = cms.EDProducer( "EgammaHLTRecoEcalCandidateProducers",
        scIslandEndcapProducer = cms.InputTag( "hltCorrectedMulti5x5EndcapSuperClustersWithPreshowerL1Seeded" ),
        scHybridBarrelProducer = cms.InputTag( "hltCorrectedHybridSuperClustersL1Seeded" ),
        recoEcalCandidateCollection = cms.string( "" ))


hltL1SeededHLTClusterShape = cms.EDProducer( "EgammaHLTClusterShapeProducer",
    recoEcalCandidateProducer = cms.InputTag( "hltL1SeededRecoEcalCandidate" ),
    ecalRechitEB = cms.InputTag( 'hltEcalRegionalEgammaRecHit','EcalRecHitsEB' ),
    ecalRechitEE = cms.InputTag( 'hltEcalRegionalEgammaRecHit','EcalRecHitsEE' ),
    isIeta = cms.bool( True ))



HLTMulti5x5SuperClusterL1Seeded = cms.Sequence( hltMulti5x5BasicClustersL1Seeded +
                                                hltMulti5x5SuperClustersL1Seeded +
                                                hltMulti5x5EndcapSuperClustersWithPreshowerL1Seeded +
                                                hltCorrectedMulti5x5EndcapSuperClustersWithPreshowerL1Seeded )

HLTL1SeededEcalClustersSequence = cms.Sequence( hltHybridSuperClustersL1Seeded +
                                                hltCorrectedHybridSuperClustersL1Seeded +
                                                HLTMulti5x5SuperClusterL1Seeded)

