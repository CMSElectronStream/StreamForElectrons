import FWCore.ParameterSet.Config as cms

from HLTrigger.Configuration.HLT_ElectronTrackSequence_cff import *

hltL1SeededCkfTrackCandidatesForGSF = cms.EDProducer( "CkfTrackCandidateMaker",
   src = cms.InputTag( "hltL1SeededStartUpElectronPixelSeeds" ),
   maxSeedsBeforeCleaning = cms.uint32( 1000 ),
   TransientInitialStateEstimatorParameters = cms.PSet( 
      propagatorAlongTISE = cms.string( "PropagatorWithMaterial" ),
      numberMeasurementsForFit = cms.int32( 4 ),
      propagatorOppositeTISE = cms.string( "PropagatorWithMaterialOpposite" )
   ),
   TrajectoryCleaner = cms.string( "hltESPTrajectoryCleanerBySharedHits" ),
   cleanTrajectoryAfterInOut = cms.bool( True ),
   useHitsSplitting = cms.bool( True ),
   RedundantSeedCleaner = cms.string( "CachingSeedCleanerBySharedInput" ),
   doSeedingRegionRebuilding = cms.bool( True ),
   maxNSeeds = cms.uint32( 1000000 ),
   NavigationSchool = cms.string( "SimpleNavigationSchool" ),
   TrajectoryBuilder = cms.string( "hltESPTrajectoryBuilderForElectrons" )
)

hltL1SeededElectronGsfTracks = cms.EDProducer( "GsfTrackProducer",
    src = cms.InputTag( "hltL1SeededCkfTrackCandidatesForGSF" ),
    beamSpot = cms.InputTag( "hltOnlineBeamSpot" ),
    producer = cms.string( "" ),
    Fitter = cms.string( "hltESPGsfElectronFittingSmoother" ),
    useHitsSplitting = cms.bool( False ),
    MeasurementTracker = cms.string( "hltESPMeasurementTracker" ),
    GeometricInnerState = cms.bool( False ),
    NavigationSchool = cms.string( "SimpleNavigationSchool" ),
    TrajectoryInEvent = cms.bool( True ),
    TTRHBuilder = cms.string( "hltESPTTRHBWithTrackAngle" ),
    AlgorithmName = cms.string( "gsf" ),
    Propagator = cms.string( "hltESPFwdElectronPropagator" )
)

hltL1SeededGsfElectrons = cms.EDProducer( "EgammaHLTPixelMatchElectronProducers",
    BSProducer = cms.InputTag( "hltOnlineBeamSpot" ),
    UseGsfTracks = cms.bool(True),
    TrackProducer = cms.InputTag(""),
    GsfTrackProducer = cms.InputTag( "hltL1SeededElectronGsfTracks" )
)

hltL1SeededGsfTrackVars = cms.EDProducer( "EgammaHLTGsfTrackVarProducer",
    recoEcalCandidateProducer = cms.InputTag( "hltL1SeededRecoEcalCandidate" ),
    beamSpotProducer = cms.InputTag( "hltOnlineBeamSpot" ),
    upperTrackNrToRemoveCut = cms.int32( 9999 ),
    lowerTrackNrToRemoveCut = cms.int32( -1 ),
    inputCollection = cms.InputTag( "hltL1SeededGsfElectrons" )
)


HLTL1SeededGsfElectronSequence = cms.Sequence( hltL1SeededCkfTrackCandidatesForGSF + 
		                               hltL1SeededElectronGsfTracks + 
					       hltL1SeededGsfElectrons + 
			                       hltL1SeededGsfTrackVars )


hltGsfEleAnyL1SeededDetaDphi = cms.EDProducer( "EgammaHLTElectronDetaDphiProducer",
    variablesAtVtx = cms.bool( False ),
    useSCRefs = cms.bool( False ),
    BSProducer = cms.InputTag( "hltOnlineBeamSpot" ),
    electronProducer = cms.InputTag( "hltL1SeededGsfElectrons" ),
    recoEcalCandidateProducer = cms.InputTag( "" ),
    useTrackProjectionToEcal = cms.bool( False )
)
