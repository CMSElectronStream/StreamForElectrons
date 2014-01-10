import FWCore.ParameterSet.Config as cms

## Load regional Ecal sequence to build RecHit EB,EE and ES
from HLTrigger.Configuration.HLTDoRegionalEgammaEcal_cff import *

## Load regional Ecal clutering sequence + correction in EB and EE
from HLTrigger.Configuration.HLT_ECALCLusteringSequence_cff import *

## Load CaloJets Sequences for Rho correction for isolation
from HLTrigger.Configuration.HLT_CaloJets_cff import *

### candidate producer for Electron Isolation in Ecal, HCAL and tracker
from HLTrigger.Configuration.HLT_ElectronIsolation_cff import *

### strip and pixel sequences 
from HLTrigger.Configuration.HLT_TrackerLocalSequence_cff import *

### strip and pixel sequences 
from HLTrigger.Configuration.HLT_ElectronTrackSequence_cff import *
from HLTrigger.Configuration.HLT_ElectronFilters_cff import *

### filter on double electron candidate L1 seeds --> taking the probe electron
hltEG178FilterDoubleEG178 = hltEGFilterDoubleEG.clone()
hltEG178FilterDoubleEG178.inputTag = cms.InputTag("hltEGRegionalL1DoubleEG178")
hltEG178FilterDoubleEG178.etcutEB = cms.double( 17.0 )
hltEG178FilterDoubleEG178.etcutEE = cms.double( 17.0 )

## cluster shape filter
hltEG178WP90ClusterShapeFilter = hltEleClusterShapeFilter.clone()
hltEG178WP90ClusterShapeFilter.thrRegularEE = cms.double( 0.03 )
hltEG178WP90ClusterShapeFilter.thrRegularEB = cms.double( 0.01 )
hltEG178WP90ClusterShapeFilter.ncandcut = cms.int32(1)
hltEG178WP90ClusterShapeFilter.candTag = cms.InputTag("hltEG178FilterDoubleEG178")

## ecal iso filter
hltEG178WP90EcalRhoIsoFilter = hltEleEcalIsoFilter.clone()
hltEG178WP90EcalRhoIsoFilter.thrOverEEE = cms.double( 0.035 )
hltEG178WP90EcalRhoIsoFilter.thrOverEEB = cms.double( 0.07 )
hltEG178WP90EcalRhoIsoFilter.ncandcut = cms.int32(1)
hltEG178WP90EcalRhoIsoFilter.candTag = cms.InputTag("hltEG178WP90ClusterShapeFilter")
hltEG178WP90EcalRhoIsoFilter.saveTags = cms.bool(True)
hltEG178WP90EcalRhoIsoFilter.isoTag = cms.InputTag("hltL1SeededPhotonEcalIsoRhoCorr")

### filter for HoverE
hltEG178WP90RhoHEFilter = hltEleHEFilter.clone()
hltEG178WP90RhoHEFilter.thrOverEEE = cms.double( 0.05 )
hltEG178WP90RhoHEFilter.thrOverEEB = cms.double( 0.05 )
hltEG178WP90RhoHEFilter.ncandcut = cms.int32(1)
hltEG178WP90RhoHEFilter.candTag = cms.InputTag("hltEG178WP90EcalRhoIsoFilter")
hltEG178WP90RhoHEFilter.saveTags = cms.bool(True)

### filter on HCal isolation for electron ID
hltEG178WP90HcalRhoIsoFilter = hltEleHcalIsoFilter.clone()
hltEG178WP90HcalRhoIsoFilter.thrOverEEE = cms.double( 0.05 )
hltEG178WP90HcalRhoIsoFilter.thrOverEEB = cms.double( 0.05 )
hltEG178WP90HcalRhoIsoFilter.ncandcut = cms.int32(1)
hltEG178WP90HcalRhoIsoFilter.candTag = cms.InputTag("hltEG178WP90RhoHEFilter")
hltEG178WP90HcalRhoIsoFilter.saveTags = cms.bool(True)
hltEG178WP90HcalRhoIsoFilter.isoTag = cms.InputTag("hltL1SeededPhotonHcalIsoRhoCorr")

### filter the tracks looking at the electron candidates
hltEG178WP90RhoPixelMatchFilter = hltElePixelMatchFilter.clone()
hltEG178WP90RhoPixelMatchFilter.npixelmatchcut = cms.double(1.0)
hltEG178WP90RhoPixelMatchFilter.ncandcut = cms.int32(1)
hltEG178WP90RhoPixelMatchFilter.candTag = cms.InputTag("hltEG178WP90HcalRhoIsoFilter")

###
hltEG178WP90RhoOneOEMinusOneOPFilter = hltEleOneOEMinusOneOPFilter.clone()
hltEG178WP90RhoOneOEMinusOneOPFilter.barrelcut = cms.double( 999.9 )
hltEG178WP90RhoOneOEMinusOneOPFilter.endcapcut = cms.double( 999.9 )
hltEG178WP90RhoOneOEMinusOneOPFilter.ncandcut = cms.int32(1)
hltEG178WP90RhoOneOEMinusOneOPFilter.candTag = cms.InputTag("hltEG178WP90RhoPixelMatchFilter")

## Deta filter
hltEG178WP90RhoDetaFilter = hltEleDetaFilter.clone()
hltEG178WP90RhoDetaFilter.thrRegularEE = cms.double( 0.006 )
hltEG178WP90RhoDetaFilter.thrRegularEB = cms.double( 0.006 )
hltEG178WP90RhoDetaFilter.ncandcut = cms.int32(1)
hltEG178WP90RhoDetaFilter.candTag = cms.InputTag("hltEG178WP90RhoOneOEMinusOneOPFilter")

## Dphi filter
hltEG178WP90RhoDphiFilter = hltEleDphiFilter.clone()
hltEG178WP90RhoDphiFilter.thrRegularEE = cms.double( 0.05 )
hltEG178WP90RhoDphiFilter.thrRegularEB = cms.double( 0.04 )
hltEG178WP90RhoDphiFilter.ncandcut = cms.int32(1)
hltEG178WP90RhoDphiFilter.candTag = cms.InputTag("hltEG178WP90RhoDetaFilter")

## track iso filter
hltEG178WP90TrackRhoIsoFilter = hltEleTrackIsoFilter.clone()
hltEG178WP90TrackRhoIsoFilter.thrOverPtEE = cms.double( 0.05 )
hltEG178WP90TrackRhoIsoFilter.thrOverPtEB = cms.double( 0.05 )
hltEG178WP90TrackRhoIsoFilter.ncandcut = cms.int32(1)
hltEG178WP90TrackRhoIsoFilter.candTag = cms.InputTag("hltEG178WP90RhoDphiFilter")

## et filter on the second electron
hltDoubleEG8EtFilterUnseeded = hltEGFilterDoubleEG.clone()
hltDoubleEG8EtFilterUnseeded.L1IsoCand = cms.InputTag("hltRecoEcalSuperClusterActivityCandidate")
hltDoubleEG8EtFilterUnseeded.inputTag = cms.InputTag("hltEcalActivitySuperClusterWrapper")
hltDoubleEG8EtFilterUnseeded.ncandcut = cms.int32( 2 )
hltDoubleEG8EtFilterUnseeded.etcutEB = cms.double( 8.0 )
hltDoubleEG8EtFilterUnseeded.etcutEE = cms.double( 8.0 )

## cluster shape filter
hltEG178WP90ClusterShapeFilterv2 = hltEleClusterShapeFilter.clone()
hltEG178WP90ClusterShapeFilterv2.thrRegularEE = cms.double( 0.03 )
hltEG178WP90ClusterShapeFilterv2.thrRegularEB = cms.double( 0.01 )
hltEG178WP90ClusterShapeFilterv2.ncandcut  = cms.int32(2)
hltEG178WP90ClusterShapeFilterv2.candTag   = cms.InputTag("hltDoubleEG8EtFilterUnseeded")
hltEG178WP90ClusterShapeFilterv2.L1IsoCand = cms.InputTag("hltRecoEcalSuperClusterActivityCandidate")
hltEG178WP90ClusterShapeFilterv2.isoTag    = cms.InputTag("hltActivityPhotonClusterShape")

## ecal iso filter
hltEG178WP90EcalRhoIsoFilterv2 = hltEleEcalIsoFilter.clone()
hltEG178WP90EcalRhoIsoFilterv2.thrOverEEE = cms.double( 0.035 )
hltEG178WP90EcalRhoIsoFilterv2.thrOverEEB = cms.double( 0.07 )
hltEG178WP90EcalRhoIsoFilterv2.ncandcut   = cms.int32(2)
hltEG178WP90EcalRhoIsoFilterv2.candTag    = cms.InputTag("hltEG178WP90ClusterShapeFilterv2")
hltEG178WP90EcalRhoIsoFilterv2.saveTags   = cms.bool(True)
hltEG178WP90EcalRhoIsoFilterv2.isoTag     = cms.InputTag("hltActivityPhotonEcalIsoRhoCorr")
hltEG178WP90EcalRhoIsoFilterv2.L1IsoCand  = cms.InputTag("hltRecoEcalSuperClusterActivityCandidate")

### filter for HoverE
hltEG178WP90RhoHEFilterv2 = hltEleHEFilter.clone()
hltEG178WP90RhoHEFilterv2.thrOverEEE = cms.double( 0.05 )
hltEG178WP90RhoHEFilterv2.thrOverEEB = cms.double( 0.05 )
hltEG178WP90RhoHEFilterv2.ncandcut   = cms.int32(2)
hltEG178WP90RhoHEFilterv2.candTag    = cms.InputTag("hltEG178WP90EcalRhoIsoFilterv2")
hltEG178WP90RhoHEFilterv2.isoTag     = cms.InputTag("hltActivityPhotonHcalForHE")
hltEG178WP90RhoHEFilterv2.L1IsoCand  = cms.InputTag("hltRecoEcalSuperClusterActivityCandidate")
hltEG178WP90RhoHEFilterv2.saveTags   = cms.bool(True)

### filter on HCal isolation for electron ID
hltEG178WP90HcalRhoIsoFilterv2 = hltEleHcalIsoFilter.clone()
hltEG178WP90HcalRhoIsoFilterv2.thrOverEEE = cms.double( 0.05 )
hltEG178WP90HcalRhoIsoFilterv2.thrOverEEB = cms.double( 0.05 )
hltEG178WP90HcalRhoIsoFilterv2.ncandcut = cms.int32(2)
hltEG178WP90HcalRhoIsoFilterv2.candTag = cms.InputTag("hltEG178WP90RhoHEFilterv2")
hltEG178WP90HcalRhoIsoFilterv2.saveTags = cms.bool(True)
hltEG178WP90HcalRhoIsoFilterv2.isoTag = cms.InputTag("hltActivityPhotonHcalIsoRhoCorr")
hltEG178WP90HcalRhoIsoFilterv2.L1IsoCand  = cms.InputTag("hltRecoEcalSuperClusterActivityCandidate")

### filter the tracks looking at the electron candidates
hltEG178WP90RhoPixelMatchFilterv2 = hltElePixelMatchFilter.clone()
hltEG178WP90RhoPixelMatchFilterv2.npixelmatchcut = cms.double(1.0)
hltEG178WP90RhoPixelMatchFilterv2.ncandcut = cms.int32(2)
hltEG178WP90RhoPixelMatchFilterv2.candTag = cms.InputTag("hltEG178WP90HcalRhoIsoFilterv2")
hltEG178WP90RhoPixelMatchFilterv2.L1IsoPixelSeedsTag = cms.InputTag("hltActivityStartUpElectronPixelSeeds")
hltEG178WP90RhoPixelMatchFilterv2.L1IsoCand = cms.InputTag("hltRecoEcalSuperClusterActivityCandidate")

###
hltEG178WP90RhoOneOEMinusOneOPFilterv2 = hltEleOneOEMinusOneOPFilter.clone()
hltEG178WP90RhoOneOEMinusOneOPFilterv2.barrelcut = cms.double( 999.9 )
hltEG178WP90RhoOneOEMinusOneOPFilterv2.endcapcut = cms.double( 999.9 )
hltEG178WP90RhoOneOEMinusOneOPFilterv2.ncandcut = cms.int32(2)
hltEG178WP90RhoOneOEMinusOneOPFilterv2.candTag = cms.InputTag("hltEG178WP90RhoPixelMatchFilterv2")
hltEG178WP90RhoOneOEMinusOneOPFilterv2.electronIsolatedProducer = cms.InputTag("hltActivityEleAnyPixelMatchElectronsL1Seeded")

## Deta filter
hltEG178WP90RhoDetaFilterv2 = hltEleDetaFilter.clone()
hltEG178WP90RhoDetaFilterv2.thrRegularEE = cms.double( 0.006 )
hltEG178WP90RhoDetaFilterv2.thrRegularEB = cms.double( 0.006 )
hltEG178WP90RhoDetaFilterv2.ncandcut = cms.int32(2)
hltEG178WP90RhoDetaFilterv2.candTag = cms.InputTag("hltEG178WP90RhoOneOEMinusOneOPFilterv2")
hltEG178WP90RhoDetaFilterv2.L1IsoCand = cms.InputTag("hltEG178WP90RhoPixelMatchFilterv2")
hltEG178WP90RhoDetaFilterv2.isoTag = cms.InputTag('hltActivityEleAnyL1SeededDetaDphi','Deta')

## Dphi filter
hltEG178WP90RhoDphiFilterv2 = hltEleDphiFilter.clone()
hltEG178WP90RhoDphiFilterv2.thrRegularEE = cms.double( 0.05 )
hltEG178WP90RhoDphiFilterv2.thrRegularEB = cms.double( 0.04 )
hltEG178WP90RhoDphiFilterv2.ncandcut = cms.int32(2)
hltEG178WP90RhoDphiFilterv2.candTag = cms.InputTag("hltEG178WP90RhoDetaFilterv2")
hltEG178WP90RhoDphiFilterv2.L1IsoCand = cms.InputTag("hltEG178WP90RhoPixelMatchFilterv2")
hltEG178WP90RhoDphiFilterv2.isoTag = cms.InputTag('hltActivityEleAnyL1SeededDetaDphi','Dphi')

## track iso filter
hltEG178WP90TrackRhoIsoFilterv2 = hltEleTrackIsoFilter.clone()
hltEG178WP90TrackRhoIsoFilterv2.thrOverPtEE = cms.double( 0.05 )
hltEG178WP90TrackRhoIsoFilterv2.thrOverPtEB = cms.double( 0.05 )
hltEG178WP90TrackRhoIsoFilterv2.ncandcut = cms.int32(2)
hltEG178WP90TrackRhoIsoFilterv2.candTag = cms.InputTag("hltEG178WP90RhoDphiFilterv2")
hltEG178WP90TrackRhoIsoFilterv2.L1IsoCand = cms.InputTag("hltEG178WP90RhoPixelMatchFilterv2")
hltEG178WP90TrackRhoIsoFilterv2.isoTag = cms.InputTag("hltActivityEleAnyL1SeededElectronTrackIso")

HLT_Ele17_Ele8_WP90_v1 = cms.Sequence( HLTDoRegionalEgammaEcalSequence + 
	                               HLTL1SeededEcalClustersSequence + 
				       hltL1SeededRecoEcalCandidate +  
		                       hltEGRegionalL1DoubleEG178+ 
				       hltEG178FilterDoubleEG178+ 
		                       hltL1SeededHLTClusterShape +
				       hltEG178WP90ClusterShapeFilter +
				       hltL1SeededPhotonEcalIsoRhoCorr + 
		                       hltEG178WP90EcalRhoIsoFilter +
				       HLTDoLocalHcalWithHOSequence + 
		                       hltL1SeededPhotonHcalForHE + 
				       hltEG178WP90RhoHEFilter + 
		                       hltL1SeededPhotonHcalIsoRhoCorr + 
				       hltEG178WP90HcalRhoIsoFilter + 
		                       HLTDoLocalPixelSequence + 
				       HLTDoLocalStripSequence + 
		                       hltL1SeededStartUpElectronPixelSeeds+
				       hltEG178WP90RhoPixelMatchFilter +
                                       hltCkfL1SeededTrackCandidates+
                                       hltCtfL1SeededWithMaterialTracks+
                                       hltEleAnyBarrelTracks+
                                       hltCtfL1SeededWithMaterialCleanTracks+
                                       hltEleAnyCleanMergedTracks+
                                       hltEleAnyPixelMatchElectronsL1Seeded +
                                       hltEG178WP90RhoOneOEMinusOneOPFilter +
                                       hltEleAnyL1SeededDetaDphi+
				       hltEG178WP90RhoDetaFilter+
		                       hltEG178WP90RhoDphiFilter+
                                       HLTL1SeededEgammaRegionalRecoTrackerSequence + 
				       hltEleAnyL1SeededElectronTrackIso +
		                       hltEG178WP90TrackRhoIsoFilter+
				       HLTEcalActivitySequence+
				       hltDoubleEG8EtFilterUnseeded+
		                       hltActivityPhotonClusterShape +
				       hltEG178WP90ClusterShapeFilterv2 +
		                       hltActivityPhotonEcalIsoRhoCorr+
				       hltEG178WP90EcalRhoIsoFilterv2+ 
				       hltActivityPhotonHcalForHE + 
				       hltEG178WP90RhoHEFilterv2+
				       hltActivityPhotonHcalIsoRhoCorr +
		                       hltEG178WP90HcalRhoIsoFilterv2 +
				       hltActivityStartUpElectronPixelSeeds + 
		                       hltEG178WP90RhoPixelMatchFilterv2 +
				       hltActivityCkfL1SeededTrackCandidates+
                                       hltActivityCtfL1SeededWithMaterialTracks+
                                       hltActivityEleAnyBarrelTracks+
                                       hltActivityCtfL1SeededWithMaterialCleanTracks+
                                       hltActivityEleAnyCleanMergedTracks+
                                       hltActivityEleAnyPixelMatchElectronsL1Seeded +
				       hltEG178WP90RhoOneOEMinusOneOPFilterv2+
		                       hltActivityEleAnyL1SeededDetaDphi +
				       hltEG178WP90RhoDetaFilterv2 +
		                       hltEG178WP90RhoDphiFilterv2 +
				       HLTL1ActivitySeededEgammaRegionalRecoTrackerSequence + 
		                       hltActivityEleAnyL1SeededElectronTrackIso +
		                       hltEG178WP90TrackRhoIsoFilterv2 +
				       hltEle17Ele8TrackIsoDZ)

