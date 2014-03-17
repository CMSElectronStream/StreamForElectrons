import FWCore.ParameterSet.Config as cms

#### Ecal Iso
hltL1SeededPhotonEcalIso = cms.EDProducer( "EgammaHLTEcalRecIsolationProducer",
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
     rhoProducer = cms.InputTag( 'hltKT6CaloJets','rho' ))

hltL1SeededPhotonEcalIsoRhoCorr = cms.EDProducer( "EgammaHLTEcalRecIsolationProducer",
    etMinEndcap = cms.double( 0.11 ),
    tryBoth = cms.bool( True ),
    ecalBarrelRecHitProducer = cms.InputTag( "hltEcalRegionalEgammaRecHit" ),
    rhoMax = cms.double( 9.9999999E7 ),
    useNumCrystals = cms.bool( True ),
    etMinBarrel = cms.double( -9999.0 ),
    doRhoCorrection = cms.bool( True ),
    eMinEndcap = cms.double( -9999.0 ),
    intRadiusEndcap = cms.double( 3.0 ),
    jurassicWidth = cms.double( 3.0 ),
    useIsolEt = cms.bool( True ),
    ecalBarrelRecHitCollection = cms.InputTag( "EcalRecHitsEB" ),
    recoEcalCandidateProducer = cms.InputTag( "hltL1SeededRecoEcalCandidate" ),
    eMinBarrel = cms.double( 0.095 ),
    effectiveAreaEndcap = cms.double( 0.115 ),
    ecalEndcapRecHitProducer = cms.InputTag( "hltEcalRegionalEgammaRecHit" ),
    extRadius = cms.double( 0.3 ),
    intRadiusBarrel = cms.double( 3.0 ),
    subtract = cms.bool( False ),
    rhoScale = cms.double( 1.0 ),
    effectiveAreaBarrel = cms.double( 0.205 ),
    ecalEndcapRecHitCollection = cms.InputTag( "EcalRecHitsEE" ),
    rhoProducer = cms.InputTag( 'hltKT6CaloJets','rho' )
)

####### HCAL Iso

hltL1SeededPhotonHcalForHE = cms.EDProducer( "EgammaHLTHcalIsolationProducersRegional",
    eMinHE = cms.double( 0.8 ),
    hbheRecHitProducer = cms.InputTag( "hltHbhereco" ),
    effectiveAreaBarrel = cms.double( 0.105 ),
    outerCone = cms.double( 0.14 ),
    eMinHB = cms.double( 0.7 ),
    innerCone = cms.double( 0.0 ),
    etMinHE = cms.double( -1.0 ),
    etMinHB = cms.double( -1.0 ),
    rhoProducer = cms.InputTag( 'hltKT6CaloJets','rho' ),
    depth = cms.int32( -1 ),
    doRhoCorrection = cms.bool( False ),
    effectiveAreaEndcap = cms.double( 0.17 ),
    recoEcalCandidateProducer = cms.InputTag( "hltL1SeededRecoEcalCandidate" ),
    rhoMax = cms.double( 9.9999999E7 ),
    rhoScale = cms.double( 1.0 ),
    doEtSum = cms.bool( False ))

hltL1SeededPhotonHcalIso = cms.EDProducer( "EgammaHLTHcalIsolationProducersRegional",
    eMinHE = cms.double( 0.8 ),
    hbheRecHitProducer = cms.InputTag( "hltHbhereco" ),
    effectiveAreaBarrel = cms.double( 0.105 ),
    outerCone = cms.double( 0.29 ),
    eMinHB = cms.double( 0.7 ),
    innerCone = cms.double( 0.16 ),
    etMinHE = cms.double( -1.0 ),
    etMinHB = cms.double( -1.0 ),
    rhoProducer = cms.InputTag( 'hltKT6CaloJets','rho' ),
    depth = cms.int32( -1 ),
    doRhoCorrection = cms.bool( False ),
    effectiveAreaEndcap = cms.double( 0.17 ),
    recoEcalCandidateProducer = cms.InputTag( "hltL1SeededRecoEcalCandidate" ),
    rhoMax = cms.double( 9.9999999E7 ),
    rhoScale = cms.double( 1.0 ),
    doEtSum = cms.bool( True )
)

hltL1SeededPhotonHcalIsoRhoCorr = cms.EDProducer( "EgammaHLTHcalIsolationProducersRegional",
    eMinHE = cms.double( 0.8 ),
    hbheRecHitProducer = cms.InputTag( "hltHbhereco" ),
    effectiveAreaBarrel = cms.double( 0.105 ),
    outerCone = cms.double( 0.29 ),
    eMinHB = cms.double( 0.7 ),
    innerCone = cms.double( 0.16 ),
    etMinHE = cms.double( -1.0 ),
    etMinHB = cms.double( -1.0 ),
    rhoProducer = cms.InputTag( 'hltKT6CaloJets','rho' ),
    depth = cms.int32( -1 ),
    doRhoCorrection = cms.bool( True ),
    effectiveAreaEndcap = cms.double( 0.17 ),
    recoEcalCandidateProducer = cms.InputTag( "hltL1SeededRecoEcalCandidate" ),
    rhoMax = cms.double( 9.9999999E7 ),
    rhoScale = cms.double( 1.0 ),
    doEtSum = cms.bool( True )
)


########## Tracker iso
hltL1SeededEgammaRegionalPixelSeedGenerator = cms.EDProducer( "EgammaHLTRegionalPixelSeedGeneratorProducers",
    deltaPhiRegion = cms.double( 0.3 ),
    vertexZ = cms.double( 0.0 ),
    originHalfLength = cms.double( 15.0 ),
    BSProducer = cms.InputTag( "hltOnlineBeamSpot" ),
    UseZInVertex = cms.bool( False ),
    OrderedHitsFactoryPSet = cms.PSet( 
      maxElement = cms.uint32( 0 ),
      ComponentName = cms.string( "StandardHitPairGenerator" ),
      SeedingLayers = cms.string( "hltESPPixelLayerPairs" )
    ),
    deltaEtaRegion = cms.double( 0.3 ),
    ptMin = cms.double( 1.5 ),
    TTRHBuilder = cms.string( "WithTrackAngle" ),
    candTag = cms.InputTag( "hltL1SeededRecoEcalCandidate" ),
    candTagEle = cms.InputTag( "pixelMatchElectrons" ),
    originRadius = cms.double( 0.02 )
)

hltL1SeededEgammaRegionalCkfTrackCandidates = cms.EDProducer( "CkfTrackCandidateMaker",
    src = cms.InputTag( "hltL1SeededEgammaRegionalPixelSeedGenerator" ),
    maxSeedsBeforeCleaning = cms.uint32( 1000 ),
    TransientInitialStateEstimatorParameters = cms.PSet( 
      propagatorAlongTISE = cms.string( "PropagatorWithMaterial" ),
      numberMeasurementsForFit = cms.int32( 4 ),
      propagatorOppositeTISE = cms.string( "PropagatorWithMaterialOpposite" )
    ),
    TrajectoryCleaner = cms.string( "hltESPTrajectoryCleanerBySharedHits" ),
    cleanTrajectoryAfterInOut = cms.bool( False ),
    useHitsSplitting = cms.bool( False ),
    RedundantSeedCleaner = cms.string( "CachingSeedCleanerBySharedInput" ),
    doSeedingRegionRebuilding = cms.bool( False ),
    maxNSeeds = cms.uint32( 100000 ),
    NavigationSchool = cms.string( "SimpleNavigationSchool" ),
    TrajectoryBuilder = cms.string( "hltESPCkfTrajectoryBuilder" )
)

hltL1SeededEgammaRegionalCTFFinalFitWithMaterial = cms.EDProducer( "TrackProducer",
    src = cms.InputTag( "hltL1SeededEgammaRegionalCkfTrackCandidates" ),
    clusterRemovalInfo = cms.InputTag( "" ),
    beamSpot = cms.InputTag( "hltOnlineBeamSpot" ),
    Fitter = cms.string( "hltESPKFFittingSmoother" ),
    useHitsSplitting = cms.bool( False ),
    MeasurementTracker = cms.string( "" ),
    AlgorithmName = cms.string( "undefAlgorithm" ),
    alias = cms.untracked.string( "hltEgammaRegionalCTFFinalFitWithMaterial" ),
    NavigationSchool = cms.string( "" ),
    TrajectoryInEvent = cms.bool( False ),
    TTRHBuilder = cms.string( "hltESPTTRHBWithTrackAngle" ),
    GeometricInnerState = cms.bool( True ),
    Propagator = cms.string( "PropagatorWithMaterial" )
)


HLTL1SeededEgammaRegionalRecoTrackerSequence = cms.Sequence( hltL1SeededEgammaRegionalPixelSeedGenerator + 
                                                             hltL1SeededEgammaRegionalCkfTrackCandidates + 
						             hltL1SeededEgammaRegionalCTFFinalFitWithMaterial )



hltEleAnyL1SeededElectronTrackIso = cms.EDProducer( "EgammaHLTElectronTrackIsolationProducers",
    egTrkIsoStripEndcap = cms.double( 0.03 ),
    electronProducer = cms.InputTag( "hltEleAnyPixelMatchElectronsL1Seeded" ),
    egTrkIsoZSpan = cms.double( 0.15 ),
    useGsfTrack = cms.bool( False ),
    useSCRefs = cms.bool( False ),
    egTrkIsoConeSize = cms.double( 0.3 ),
    trackProducer = cms.InputTag( "hltL1SeededEgammaRegionalCTFFinalFitWithMaterial" ),
    egTrkIsoStripBarrel = cms.double( 0.03 ),
    egTrkIsoVetoConeSizeBarrel = cms.double( 0.03 ),
    egTrkIsoVetoConeSize = cms.double( 0.03 ),
    egTrkIsoRSpan = cms.double( 999999.0 ),
    egTrkIsoVetoConeSizeEndcap = cms.double( 0.03 ),
    recoEcalCandidateProducer = cms.InputTag( "" ),
    beamSpotProducer = cms.InputTag( "hltOnlineBeamSpot" ),
    egTrkIsoPtMin = cms.double( 1.0 ),
    egCheckForOtherEleInCone = cms.untracked.bool( False )
)

hltGsfEleAnyL1SeededElectronTrackIso = cms.EDProducer( "EgammaHLTElectronTrackIsolationProducers",
    egTrkIsoStripEndcap = cms.double( 0.03 ),
    electronProducer = cms.InputTag( "hltL1SeededGsfElectrons" ),
    egTrkIsoZSpan = cms.double( 0.15 ),
    useGsfTrack = cms.bool( True ),
    useSCRefs = cms.bool( False ),
    egTrkIsoConeSize = cms.double( 0.3 ),
    trackProducer = cms.InputTag( "hltL1SeededEgammaRegionalCTFFinalFitWithMaterial" ),
    egTrkIsoStripBarrel = cms.double( 0.03 ),
    egTrkIsoVetoConeSizeBarrel = cms.double( 0.03 ),
    egTrkIsoVetoConeSize = cms.double( 0.03 ),
    egTrkIsoRSpan = cms.double( 999999.0 ),
    egTrkIsoVetoConeSizeEndcap = cms.double( 0.03 ),
    recoEcalCandidateProducer = cms.InputTag( "hltL1SeededRecoEcalCandidate" ),
    beamSpotProducer = cms.InputTag( "hltOnlineBeamSpot" ),
    egTrkIsoPtMin = cms.double( 1.0 ),
    egCheckForOtherEleInCone = cms.untracked.bool( False )
)

### tracker iso with for gsf electrons


### for the whole ECAL

hltActivityPhotonEcalIso = cms.EDProducer( "EgammaHLTEcalRecIsolationProducer",
    etMinEndcap = cms.double( 0.11 ),
    tryBoth = cms.bool( True ),
    ecalBarrelRecHitProducer = cms.InputTag( "hltEcalRecHitAll" ),
    rhoMax = cms.double( 9.9999999E7 ),
    useNumCrystals = cms.bool( True ),
    etMinBarrel = cms.double( -9999.0 ),
    doRhoCorrection = cms.bool( False ),
    eMinEndcap = cms.double( -9999.0 ),
    intRadiusEndcap = cms.double( 3.0 ),
    jurassicWidth = cms.double( 3.0 ),
    useIsolEt = cms.bool( True ),
    ecalBarrelRecHitCollection = cms.InputTag( "EcalRecHitsEB" ),
    recoEcalCandidateProducer = cms.InputTag( "hltRecoEcalSuperClusterActivityCandidate" ),
    eMinBarrel = cms.double( 0.095 ),
    effectiveAreaEndcap = cms.double( 0.046 ),
    ecalEndcapRecHitProducer = cms.InputTag( "hltEcalRecHitAll" ),
    extRadius = cms.double( 0.3 ),
    intRadiusBarrel = cms.double( 3.0 ),
    subtract = cms.bool( False ),
    rhoScale = cms.double( 1.0 ),
    effectiveAreaBarrel = cms.double( 0.101 ),
    ecalEndcapRecHitCollection = cms.InputTag( "EcalRecHitsEE" ),
    rhoProducer = cms.InputTag( 'hltKT6CaloJets','rho' ))

hltActivityPhotonEcalIsoRhoCorr = cms.EDProducer( "EgammaHLTEcalRecIsolationProducer",
    etMinEndcap = cms.double( 0.11 ),
    tryBoth = cms.bool( True ),
    ecalBarrelRecHitProducer = cms.InputTag( "hltEcalRecHitAll" ),
    rhoMax = cms.double( 9.9999999E7 ),
    useNumCrystals = cms.bool( True ),
    etMinBarrel = cms.double( -9999.0 ),
    doRhoCorrection = cms.bool( True ),
    eMinEndcap = cms.double( -9999.0 ),
    intRadiusEndcap = cms.double( 3.0 ),
    jurassicWidth = cms.double( 3.0 ),
    useIsolEt = cms.bool( True ),
    ecalBarrelRecHitCollection = cms.InputTag( "EcalRecHitsEB" ),
    recoEcalCandidateProducer = cms.InputTag( "hltRecoEcalSuperClusterActivityCandidate" ),
    eMinBarrel = cms.double( 0.095 ),
    effectiveAreaEndcap = cms.double( 0.046 ),
    ecalEndcapRecHitProducer = cms.InputTag( "hltEcalRecHitAll" ),
    extRadius = cms.double( 0.3 ),
    intRadiusBarrel = cms.double( 3.0 ),
    subtract = cms.bool( False ),
    rhoScale = cms.double( 1.0 ),
    effectiveAreaBarrel = cms.double( 0.101 ),
    ecalEndcapRecHitCollection = cms.InputTag( "EcalRecHitsEE" ),
    rhoProducer = cms.InputTag( 'hltKT6CaloJets','rho' ))

hltActivityPhotonHcalForHE = cms.EDProducer( "EgammaHLTHcalIsolationProducersRegional",
    eMinHE = cms.double( 0.8 ),
    hbheRecHitProducer = cms.InputTag( "hltHbhereco" ),
    effectiveAreaBarrel = cms.double( 0.105 ),
    outerCone = cms.double( 0.14 ),
    eMinHB = cms.double( 0.7 ),
    innerCone = cms.double( 0.0 ),
    etMinHE = cms.double( -1.0 ),
    etMinHB = cms.double( -1.0 ),
    rhoProducer = cms.InputTag( 'hltKT6CaloJets','rho' ),
    depth = cms.int32( -1 ),
    doRhoCorrection = cms.bool( False ),
    effectiveAreaEndcap = cms.double( 0.17 ),
    recoEcalCandidateProducer = cms.InputTag( "hltRecoEcalSuperClusterActivityCandidate" ),
    rhoMax = cms.double( 9.9999999E7 ),
    rhoScale = cms.double( 1.0 ),
    doEtSum = cms.bool( False )
)

hltActivityPhotonHcalIso = cms.EDProducer( "EgammaHLTHcalIsolationProducersRegional",
    eMinHE = cms.double( 0.8 ),
    hbheRecHitProducer = cms.InputTag( "hltHbhereco" ),
    effectiveAreaBarrel = cms.double( 0.105 ),
    outerCone = cms.double( 0.29 ),
    eMinHB = cms.double( 0.7 ),
    innerCone = cms.double( 0.16 ),
    etMinHE = cms.double( -1.0 ),
    etMinHB = cms.double( -1.0 ),
    rhoProducer = cms.InputTag( 'hltKT6CaloJets','rho' ),
    depth = cms.int32( -1 ),
    doRhoCorrection = cms.bool( False ),
    effectiveAreaEndcap = cms.double( 0.17 ),
    recoEcalCandidateProducer = cms.InputTag( "hltRecoEcalSuperClusterActivityCandidate" ),
    rhoMax = cms.double( 9.9999999E7 ),
    rhoScale = cms.double( 1.0 ),
    doEtSum = cms.bool( True ))

hltActivityPhotonHcalIsoRhoCorr = cms.EDProducer( "EgammaHLTHcalIsolationProducersRegional",
    eMinHE = cms.double( 0.8 ),
    hbheRecHitProducer = cms.InputTag( "hltHbhereco" ),
    effectiveAreaBarrel = cms.double( 0.105 ),
    outerCone = cms.double( 0.29 ),
    eMinHB = cms.double( 0.7 ),
    innerCone = cms.double( 0.16 ),
    etMinHE = cms.double( -1.0 ),
    etMinHB = cms.double( -1.0 ),
    rhoProducer = cms.InputTag( 'hltKT6CaloJets','rho' ),
    depth = cms.int32( -1 ),
    doRhoCorrection = cms.bool( True ),
    effectiveAreaEndcap = cms.double( 0.17 ),
    recoEcalCandidateProducer = cms.InputTag( "hltRecoEcalSuperClusterActivityCandidate" ),
    rhoMax = cms.double( 9.9999999E7 ),
    rhoScale = cms.double( 1.0 ),
    doEtSum = cms.bool( True )
)


########## Tracker iso
hltL1ActivitySeededEgammaRegionalPixelSeedGenerator = cms.EDProducer( "EgammaHLTRegionalPixelSeedGeneratorProducers",
    deltaPhiRegion = cms.double( 0.3 ),
    vertexZ = cms.double( 0.0 ),
    originHalfLength = cms.double( 15.0 ),
    BSProducer = cms.InputTag( "hltOnlineBeamSpot" ),
    UseZInVertex = cms.bool( False ),
    OrderedHitsFactoryPSet = cms.PSet( 
      maxElement = cms.uint32( 0 ),
      ComponentName = cms.string( "StandardHitPairGenerator" ),
      SeedingLayers = cms.string( "hltESPPixelLayerPairs" )
    ),
    deltaEtaRegion = cms.double( 0.3 ),
    ptMin = cms.double( 1.5 ),
    TTRHBuilder = cms.string( "WithTrackAngle" ),
    candTag = cms.InputTag( "hltRecoEcalSuperClusterActivityCandidate" ),
    candTagEle = cms.InputTag( "pixelMatchElectrons" ),
    originRadius = cms.double( 0.02 )
)

hltL1ActivitySeededEgammaRegionalCkfTrackCandidates = cms.EDProducer( "CkfTrackCandidateMaker",
    src = cms.InputTag( "hltL1ActivitySeededEgammaRegionalPixelSeedGenerator" ),
    maxSeedsBeforeCleaning = cms.uint32( 1000 ),
    TransientInitialStateEstimatorParameters = cms.PSet( 
      propagatorAlongTISE = cms.string( "PropagatorWithMaterial" ),
      numberMeasurementsForFit = cms.int32( 4 ),
      propagatorOppositeTISE = cms.string( "PropagatorWithMaterialOpposite" )
    ),
    TrajectoryCleaner = cms.string( "hltESPTrajectoryCleanerBySharedHits" ),
    cleanTrajectoryAfterInOut = cms.bool( False ),
    useHitsSplitting = cms.bool( False ),
    RedundantSeedCleaner = cms.string( "CachingSeedCleanerBySharedInput" ),
    doSeedingRegionRebuilding = cms.bool( False ),
    maxNSeeds = cms.uint32( 100000 ),
    NavigationSchool = cms.string( "SimpleNavigationSchool" ),
    TrajectoryBuilder = cms.string( "hltESPCkfTrajectoryBuilder" )
)

hltL1ActivitySeededEgammaRegionalCTFFinalFitWithMaterial = cms.EDProducer( "TrackProducer",
    src = cms.InputTag( "hltL1ActivitySeededEgammaRegionalCkfTrackCandidates" ),
    clusterRemovalInfo = cms.InputTag( "" ),
    beamSpot = cms.InputTag( "hltOnlineBeamSpot" ),
    Fitter = cms.string( "hltESPKFFittingSmoother" ),
    useHitsSplitting = cms.bool( False ),
    MeasurementTracker = cms.string( "" ),
    AlgorithmName = cms.string( "undefAlgorithm" ),
    alias = cms.untracked.string( "hltEgammaRegionalCTFFinalFitWithMaterial" ),
    NavigationSchool = cms.string( "" ),
    TrajectoryInEvent = cms.bool( False ),
    TTRHBuilder = cms.string( "hltESPTTRHBWithTrackAngle" ),
    GeometricInnerState = cms.bool( True ),
    Propagator = cms.string( "PropagatorWithMaterial" )
)


HLTL1ActivitySeededEgammaRegionalRecoTrackerSequence = cms.Sequence( hltL1ActivitySeededEgammaRegionalPixelSeedGenerator + 
hltL1ActivitySeededEgammaRegionalCkfTrackCandidates + 
hltL1ActivitySeededEgammaRegionalCTFFinalFitWithMaterial )

hltActivityEleAnyL1SeededElectronTrackIso = cms.EDProducer( "EgammaHLTElectronTrackIsolationProducers",
    egTrkIsoStripEndcap = cms.double( 0.03 ),
    electronProducer = cms.InputTag("hltActivityEleAnyPixelMatchElectronsL1Seeded"),
    egTrkIsoZSpan = cms.double( 0.15 ),
    useGsfTrack = cms.bool( False ),
    useSCRefs = cms.bool( False ),
    egTrkIsoConeSize = cms.double( 0.3 ),
    trackProducer = cms.InputTag( "hltL1SeededEgammaRegionalCTFFinalFitWithMaterial" ),
    egTrkIsoStripBarrel = cms.double( 0.03 ),
    egTrkIsoVetoConeSizeBarrel = cms.double( 0.03 ),
    egTrkIsoVetoConeSize = cms.double( 0.03 ),
    egTrkIsoRSpan = cms.double( 999999.0 ),
    egTrkIsoVetoConeSizeEndcap = cms.double( 0.03 ),
    recoEcalCandidateProducer = cms.InputTag( "" ),
    beamSpotProducer = cms.InputTag( "hltOnlineBeamSpot" ),
    egTrkIsoPtMin = cms.double( 1.0 ),
    egCheckForOtherEleInCone = cms.untracked.bool( False )
)
