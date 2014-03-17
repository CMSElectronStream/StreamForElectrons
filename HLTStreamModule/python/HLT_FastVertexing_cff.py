import FWCore.ParameterSet.Config as cms

hltCaloJetIDPassed = cms.EDProducer( "HLTCaloJetIDProducer",
 min_N90 = cms.int32( -2 ),
 min_N90hits = cms.int32( 2 ),
 min_EMF = cms.double( 1.0E-6 ),
 jetsInput = cms.InputTag( "hltAntiKT5CaloJets" ),
 JetIDParams = cms.PSet(
 useRecHits = cms.bool( True ),
 hbheRecHitsColl = cms.InputTag( "hltHbhereco" ),
 hoRecHitsColl = cms.InputTag( "hltHoreco" ),
 hfRecHitsColl = cms.InputTag( "hltHfreco" ),
 ebRecHitsColl = cms.InputTag( 'hltEcalRegionalEgammaRecHit','EcalRecHitsEB'),
 eeRecHitsColl = cms.InputTag( 'hltEcalRegionalEgammaRecHit','EcalRecHitsEE')),
 max_EMF = cms.double( 999.0 )
)



hltCaloJetL1FastJetCorrected = cms.EDProducer( "CaloJetCorrectionProducer",
                                                src = cms.InputTag( "hltCaloJetIDPassed" ),
                                                correctors = cms.vstring( 'hltESPAK5CaloL1L2L3' ))



hltSelector4JetsL1FastJet = cms.EDFilter( "LargestEtCaloJetSelector",
                                          maxNumber = cms.uint32( 4 ),
                                          filter = cms.bool( False ),
                                          src = cms.InputTag( "hltCaloJetL1FastJetCorrected" ))


hltSelectorJets20L1FastJet = cms.EDFilter( "EtMinCaloJetSelector",
                                           filter = cms.bool( False ),
                                           src = cms.InputTag( "hltSelector4JetsL1FastJet" ),
                                           etMin = cms.double( 20.0 )
                                           )

from HLTrigger.Configuration.HLT_TrackerLocalSequence_cff import *

#### vertex sequence
hltFastPrimaryVertex = cms.EDProducer( "FastPrimaryVertexProducer",
                                        maxZ = cms.double( 18.0 ),
                                        maxDeltaPhi = cms.double( 0.2 ),
                                        beamSpot = cms.InputTag( "hltOnlineBeamSpot" ),
                                        maxSizeX = cms.double( 3.0 ),
                                        jets = cms.InputTag( "hltSelectorJets20L1FastJet" ),
                                        clusters = cms.InputTag( "hltSiPixelClusters" ),
                                        pixelCPE = cms.string( "hltESPPixelCPEGeneric" ),
                                        clusterLength = cms.double( 2.0 )
                                       )



hltFastPVPixelTracks = cms.EDProducer( "PixelTrackProducer",
  FilterPSet = cms.PSet(
     chi2 = cms.double( 1000.0 ),
     nSigmaTipMaxTolerance = cms.double( 0.0 ),
     ComponentName = cms.string( "PixelTrackFilterByKinematics" ),
     nSigmaInvPtTolerance = cms.double( 0.0 ),
     ptMin = cms.double( 0.1 ),
     tipMax = cms.double( 1.0 )),
  useFilterWithES = cms.bool( False ),
  passLabel = cms.string( "Pixel triplet primary tracks with vertex constraint" ),
  FitterPSet = cms.PSet(
      ComponentName = cms.string( "PixelFitterByHelixProjections" ),
      TTRHBuilder = cms.string( "hltESPTTRHBuilderPixelOnly" ),
      fixImpactParameter = cms.double( 0.0 )),
  RegionFactoryPSet = cms.PSet(
          ComponentName = cms.string( "GlobalTrackingRegionWithVerticesProducer" ),
          RegionPSet = cms.PSet(
            precise = cms.bool( True ),
            originRadius = cms.double( 0.2 ),
            ptMin = cms.double( 0.9 ),
            originHalfLength = cms.double( 24.0 ),
            beamSpot = cms.InputTag( "hltOnlineBeamSpot" ),
            VertexCollection = cms.InputTag( "hltFastPrimaryVertex" ),
            useFixedError = cms.bool( True ),
            sigmaZVertex = cms.double( 3.0 ),
            fixedError = cms.double( 1.5 ),
            useFoundVertices = cms.bool( True ),
            nSigmaZ = cms.double( 3.0 ))),
  CleanerPSet = cms.PSet(  ComponentName = cms.string( "PixelTrackCleanerBySharedHits" ) ),
   OrderedHitsFactoryPSet = cms.PSet(
     ComponentName = cms.string( "StandardHitTripletGenerator" ),
     GeneratorPSet = cms.PSet(
          useBending = cms.bool( True ),
          useFixedPreFiltering = cms.bool( False ),
          maxElement = cms.uint32( 10000 ),
          phiPreFiltering = cms.double( 0.3 ),
          extraHitRPhitolerance = cms.double( 0.06 ),
          useMultScattering = cms.bool( True ),
          ComponentName = cms.string( "PixelTripletHLTGenerator" ),
          extraHitRZtolerance = cms.double( 0.06 ),
          SeedComparitorPSet = cms.PSet(  ComponentName = cms.string( "LowPtClusterShapeSeedComparitor" ) )),
   SeedingLayers = cms.string( "hltESPPixelLayerTriplets" )))

hltFastPVJetTracksAssociator = cms.EDProducer( "JetTracksAssociatorAtVertex",
                                                jets = cms.InputTag( "hltSelectorJets20L1FastJet" ),
                                                tracks = cms.InputTag( "hltFastPVPixelTracks" ),
                                                useAssigned = cms.bool( False ),
                                                coneSize = cms.double( 0.5 ),
                                                pvSrc = cms.InputTag( "" ))

hltFastPVJetVertexChecker = cms.EDFilter("JetVertexChecker",
                                         beamSpot = cms.InputTag( "hltOnlineBeamSpot" ),
                                         maxNJetsToCheck = cms.int32( 2 ),
                                         minPtRatio = cms.double( 0.05 ),
                                         doFilter = cms.bool( False ),
                                         jetTracks = cms.InputTag( "hltFastPVJetTracksAssociator" ),
                                         minPt = cms.double( 0.0 ))

hltFastPVPixelTracksRecover = cms.EDProducer( "PixelTrackProducer",
      FilterPSet = cms.PSet(
          chi2 = cms.double( 1000.0 ),
                nSigmaTipMaxTolerance = cms.double( 0.0 ),
                ComponentName = cms.string( "PixelTrackFilterByKinematics" ),
                nSigmaInvPtTolerance = cms.double( 0.0 ),
                ptMin = cms.double( 0.1 ),
                tipMax = cms.double( 1.0 )
      ),
      useFilterWithES = cms.bool( False ),
      passLabel = cms.string( "Pixel triplet primary tracks with vertex constraint" ),
      FitterPSet = cms.PSet(
          ComponentName = cms.string( "PixelFitterByHelixProjections" ),
          TTRHBuilder = cms.string( "hltESPTTRHBuilderPixelOnly" ),
          fixImpactParameter = cms.double( 0.0 )),
      RegionFactoryPSet = cms.PSet(
          ComponentName = cms.string( "TauRegionalPixelSeedGenerator" ),
          RegionPSet = cms.PSet(
            precise = cms.bool( True ),
            originRadius = cms.double( 0.2 ),
            ptMin = cms.double( 0.9 ),
            originHalfLength = cms.double( 15.0 ),
            deltaPhiRegion = cms.double( 0.5 ),
            deltaEtaRegion = cms.double( 0.5 ),
            JetSrc = cms.InputTag( "hltFastPVJetVertexChecker" ),
            vertexSrc = cms.InputTag( "hltFastPVJetVertexChecker" ),
            originZPos = cms.double( 0.0 ),
            beamSpot = cms.InputTag( "hltOnlineBeamSpot" ),
            JetMaxN = cms.int32( 10 ),
            deltaPhi = cms.double( -1.0 ),
            deltaEta = cms.double( -1.0 ),
            JetMinPt = cms.double( 20.0 ),
            JetMaxEta = cms.double( 2.5 ))),
      CleanerPSet = cms.PSet(  ComponentName = cms.string( "PixelTrackCleanerBySharedHits" ) ),
      OrderedHitsFactoryPSet = cms.PSet(
          ComponentName = cms.string( "StandardHitTripletGenerator" ),
          GeneratorPSet = cms.PSet(
            useBending = cms.bool( True ),
            useFixedPreFiltering = cms.bool( False ),
            maxElement = cms.uint32( 100000 ),
            phiPreFiltering = cms.double( 0.3 ),
            extraHitRPhitolerance = cms.double( 0.06 ),
            useMultScattering = cms.bool( True ),
            ComponentName = cms.string( "PixelTripletHLTGenerator" ),
            extraHitRZtolerance = cms.double( 0.06 ),
            SeedComparitorPSet = cms.PSet(  ComponentName = cms.string( "LowPtClusterShapeSeedComparitor" ) )),
            SeedingLayers = cms.string( "hltESPPixelLayerTriplets" )))


hltFastPVPixelTracksMerger = cms.EDProducer( "SimpleTrackListMerger",
                                              ShareFrac = cms.double( 0.19 ),
                                              promoteTrackQuality = cms.bool( False ),
                                              MinPT = cms.double( 0.05 ),
                                              copyExtras = cms.untracked.bool( False ),
                                              Epsilon = cms.double( -0.001 ),
                                              allowFirstHitShare = cms.bool( True ),
                                              newQuality = cms.string( "confirmed" ),
                                              MaxNormalizedChisq = cms.double( 1000.0 ),
                                              TrackProducer1 = cms.string( "hltFastPVPixelTracks" ),
                                              MinFound = cms.int32( 3 ),
                                              TrackProducer2 = cms.string( "hltFastPVPixelTracksRecover" ),
                                              LostHitPenalty = cms.double( 20.0 ),
                                              FoundHitBonus = cms.double( 5.0 ))
                                             
hltFastPVPixelVertices = cms.EDProducer( "PixelVertexProducer",
                                          WtAverage = cms.bool( True ),
                                          Method2 = cms.bool( True ),
                                          beamSpot = cms.InputTag( "hltOnlineBeamSpot" ),
                                          Verbosity = cms.int32( 0 ),
                                          UseError = cms.bool( True ),
                                          TrackCollection = cms.InputTag( "hltFastPVPixelTracksMerger" ),
                                          PtMin = cms.double( 1.0 ),
                                          NTrkMin = cms.int32( 2 ),
                                          ZOffset = cms.double( 5.0 ),
                                          Finder = cms.string( "DivisiveVertexFinder" ),
                                          ZSeparation = cms.double( 0.05 )
                                         )

HLTFastRecopixelvertexingSequence = cms.Sequence( hltFastPrimaryVertex +
                                                  hltFastPVPixelTracks +
                                                  hltFastPVJetTracksAssociator +
                                                  hltFastPVJetVertexChecker +
                                                  hltFastPVPixelTracksRecover +
                                                  hltFastPVPixelTracksMerger +
                                                  hltFastPVPixelVertices )

##################################

HLTFastPrimaryVertexSequence = cms.Sequence( hltCaloJetIDPassed +
                                             hltCaloJetL1FastJetCorrected +
                                             hltSelector4JetsL1FastJet +
                                             hltSelectorJets20L1FastJet +
                                             HLTDoLocalPixelSequence +
                                             HLTFastRecopixelvertexingSequence )

