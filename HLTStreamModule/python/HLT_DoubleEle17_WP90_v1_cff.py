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

### filter on double electron candidate L1 seeds --> taking the probe and trailing electron
hltEG17FilterDoubleEG17          = hltEGFilterDoubleEG.clone()
hltEG17FilterDoubleEG17.inputTag = cms.InputTag("hltEGRegionalL1DoubleEG178")
hltEG17FilterDoubleEG17.etcutEB  = cms.double( 17.0 )
hltEG17FilterDoubleEG17.etcutEE  = cms.double( 17.0 )

## cluster shape filter
hltEG17WP90ClusterShapeFilter = hltEleClusterShapeFilter.clone()
hltEG17WP90ClusterShapeFilter.thrRegularEE = cms.double( 0.03 )
hltEG17WP90ClusterShapeFilter.thrRegularEB = cms.double( 0.01 )
hltEG17WP90ClusterShapeFilter.ncandcut = cms.int32(1)
hltEG17WP90ClusterShapeFilter.candTag = cms.InputTag("hltEG17FilterDoubleEG17")

## ecal iso filter
hltEG17WP90EcalRhoIsoFilter = hltEleEcalIsoFilter.clone()
hltEG17WP90EcalRhoIsoFilter.thrOverEEE = cms.double( 0.035 )
hltEG17WP90EcalRhoIsoFilter.thrOverEEB = cms.double( 0.07 )
hltEG17WP90EcalRhoIsoFilter.ncandcut = cms.int32(1)
hltEG17WP90EcalRhoIsoFilter.candTag = cms.InputTag("hltEG17WP90ClusterShapeFilter")
hltEG17WP90EcalRhoIsoFilter.saveTags = cms.bool(True)
hltEG17WP90EcalRhoIsoFilter.isoTag = cms.InputTag("hltL1SeededPhotonEcalIsoRhoCorr")

### filter for HoverE
hltEG17WP90RhoHEFilter = hltEleHEFilter.clone()
hltEG17WP90RhoHEFilter.thrOverEEE = cms.double( 0.05 )
hltEG17WP90RhoHEFilter.thrOverEEB = cms.double( 0.05 )
hltEG17WP90RhoHEFilter.ncandcut = cms.int32(1)
hltEG17WP90RhoHEFilter.candTag = cms.InputTag("hltEG17WP90EcalRhoIsoFilter")
hltEG17WP90RhoHEFilter.saveTags = cms.bool(True)

### filter on HCal isolation for electron ID
hltEG17WP90HcalRhoIsoFilter = hltEleHcalIsoFilter.clone()
hltEG17WP90HcalRhoIsoFilter.thrOverEEE = cms.double( 0.05 )
hltEG17WP90HcalRhoIsoFilter.thrOverEEB = cms.double( 0.05 )
hltEG17WP90HcalRhoIsoFilter.ncandcut = cms.int32(1)
hltEG17WP90HcalRhoIsoFilter.candTag = cms.InputTag("hltEG17WP90RhoHEFilter")
hltEG17WP90HcalRhoIsoFilter.saveTags = cms.bool(True)
hltEG17WP90HcalRhoIsoFilter.isoTag = cms.InputTag("hltL1SeededPhotonHcalIsoRhoCorr")

### filter the tracks looking at the electron candidates
hltEG17WP90RhoPixelMatchFilter = hltElePixelMatchFilter.clone()
hltEG17WP90RhoPixelMatchFilter.npixelmatchcut = cms.double(1.0)
hltEG17WP90RhoPixelMatchFilter.ncandcut = cms.int32(1)
hltEG17WP90RhoPixelMatchFilter.candTag = cms.InputTag("hltEG17WP90HcalRhoIsoFilter")

###
hltEG17WP90RhoOneOEMinusOneOPFilter = hltEleOneOEMinusOneOPFilter.clone()
hltEG17WP90RhoOneOEMinusOneOPFilter.barrelcut = cms.double( 999.9 )
hltEG17WP90RhoOneOEMinusOneOPFilter.endcapcut = cms.double( 999.9 )
hltEG17WP90RhoOneOEMinusOneOPFilter.ncandcut = cms.int32(1)
hltEG17WP90RhoOneOEMinusOneOPFilter.candTag = cms.InputTag("hltEG17WP90RhoPixelMatchFilter")

## Deta filter
hltEG17WP90RhoDetaFilter = hltEleDetaFilter.clone()
hltEG17WP90RhoDetaFilter.thrRegularEE = cms.double( 0.006 )
hltEG17WP90RhoDetaFilter.thrRegularEB = cms.double( 0.006 )
hltEG17WP90RhoDetaFilter.ncandcut = cms.int32(1)
hltEG17WP90RhoDetaFilter.candTag = cms.InputTag("hltEG17WP90RhoOneOEMinusOneOPFilter")

## Dphi filter
hltEG17WP90RhoDphiFilter = hltEleDphiFilter.clone()
hltEG17WP90RhoDphiFilter.thrRegularEE = cms.double( 0.05 )
hltEG17WP90RhoDphiFilter.thrRegularEB = cms.double( 0.04 )
hltEG17WP90RhoDphiFilter.ncandcut = cms.int32(1)
hltEG17WP90RhoDphiFilter.candTag = cms.InputTag("hltEG17WP90RhoDetaFilter")

## track iso filter
hltEG17WP90TrackRhoIsoFilter = hltEleTrackIsoFilter.clone()
hltEG17WP90TrackRhoIsoFilter.thrOverPtEE = cms.double( 0.05 )
hltEG17WP90TrackRhoIsoFilter.thrOverPtEB = cms.double( 0.05 )
hltEG17WP90TrackRhoIsoFilter.ncandcut = cms.int32(1)
hltEG17WP90TrackRhoIsoFilter.candTag = cms.InputTag("hltEG17WP90RhoDphiFilter")

## et filter on the second electron
hltDoubleEG17EtFilterUnseeded = hltEGFilterDoubleEG.clone()
hltDoubleEG17EtFilterUnseeded.L1IsoCand = cms.InputTag("hltRecoEcalSuperClusterActivityCandidate")
hltDoubleEG17EtFilterUnseeded.inputTag = cms.InputTag("hltEcalActivitySuperClusterWrapper")
hltDoubleEG17EtFilterUnseeded.ncandcut = cms.int32( 2 )
hltDoubleEG17EtFilterUnseeded.etcutEB = cms.double( 17.0 )
hltDoubleEG17EtFilterUnseeded.etcutEE = cms.double( 17.0 )

## cluster shape filter
hltEG17WP90ClusterShapeFilterv2 = hltEleClusterShapeFilter.clone()
hltEG17WP90ClusterShapeFilterv2.thrRegularEE = cms.double( 0.03 )
hltEG17WP90ClusterShapeFilterv2.thrRegularEB = cms.double( 0.01 )
hltEG17WP90ClusterShapeFilterv2.ncandcut  = cms.int32(2)
hltEG17WP90ClusterShapeFilterv2.candTag   = cms.InputTag("hltDoubleEG17EtFilterUnseeded")
hltEG17WP90ClusterShapeFilterv2.L1IsoCand = cms.InputTag("hltRecoEcalSuperClusterActivityCandidate")
hltEG17WP90ClusterShapeFilterv2.isoTag    = cms.InputTag("hltActivityPhotonClusterShape")

## ecal iso filter
hltEG17WP90EcalRhoIsoFilterv2 = hltEleEcalIsoFilter.clone()
hltEG17WP90EcalRhoIsoFilterv2.thrOverEEE = cms.double( 0.035 )
hltEG17WP90EcalRhoIsoFilterv2.thrOverEEB = cms.double( 0.07 )
hltEG17WP90EcalRhoIsoFilterv2.ncandcut   = cms.int32(2)
hltEG17WP90EcalRhoIsoFilterv2.candTag    = cms.InputTag("hltEG17WP90ClusterShapeFilterv2")
hltEG17WP90EcalRhoIsoFilterv2.saveTags   = cms.bool(True)
hltEG17WP90EcalRhoIsoFilterv2.isoTag     = cms.InputTag("hltActivityPhotonEcalIsoRhoCorr")
hltEG17WP90EcalRhoIsoFilterv2.L1IsoCand  = cms.InputTag("hltRecoEcalSuperClusterActivityCandidate")

### filter for HoverE
hltEG17WP90RhoHEFilterv2 = hltEleHEFilter.clone()
hltEG17WP90RhoHEFilterv2.thrOverEEE = cms.double( 0.05 )
hltEG17WP90RhoHEFilterv2.thrOverEEB = cms.double( 0.05 )
hltEG17WP90RhoHEFilterv2.ncandcut   = cms.int32(2)
hltEG17WP90RhoHEFilterv2.candTag    = cms.InputTag("hltEG17WP90EcalRhoIsoFilterv2")
hltEG17WP90RhoHEFilterv2.isoTag     = cms.InputTag("hltActivityPhotonHcalForHE")
hltEG17WP90RhoHEFilterv2.L1IsoCand  = cms.InputTag("hltRecoEcalSuperClusterActivityCandidate")
hltEG17WP90RhoHEFilterv2.saveTags   = cms.bool(True)

### filter on HCal isolation for electron ID
hltEG17WP90HcalRhoIsoFilterv2 = hltEleHcalIsoFilter.clone()
hltEG17WP90HcalRhoIsoFilterv2.thrOverEEE = cms.double( 0.05 )
hltEG17WP90HcalRhoIsoFilterv2.thrOverEEB = cms.double( 0.05 )
hltEG17WP90HcalRhoIsoFilterv2.ncandcut = cms.int32(2)
hltEG17WP90HcalRhoIsoFilterv2.candTag = cms.InputTag("hltEG17WP90RhoHEFilterv2")
hltEG17WP90HcalRhoIsoFilterv2.saveTags = cms.bool(True)
hltEG17WP90HcalRhoIsoFilterv2.isoTag = cms.InputTag("hltActivityPhotonHcalIsoRhoCorr")
hltEG17WP90HcalRhoIsoFilterv2.L1IsoCand  = cms.InputTag("hltRecoEcalSuperClusterActivityCandidate")

### filter the tracks looking at the electron candidates
hltEG17WP90RhoPixelMatchFilterv2 = hltElePixelMatchFilter.clone()
hltEG17WP90RhoPixelMatchFilterv2.npixelmatchcut = cms.double(1.0)
hltEG17WP90RhoPixelMatchFilterv2.ncandcut = cms.int32(2)
hltEG17WP90RhoPixelMatchFilterv2.candTag = cms.InputTag("hltEG17WP90HcalRhoIsoFilterv2")
hltEG17WP90RhoPixelMatchFilterv2.L1IsoPixelSeedsTag = cms.InputTag("hltActivityStartUpElectronPixelSeeds")
hltEG17WP90RhoPixelMatchFilterv2.L1IsoCand = cms.InputTag("hltRecoEcalSuperClusterActivityCandidate")

###
hltEG17WP90RhoOneOEMinusOneOPFilterv2 = hltEleOneOEMinusOneOPFilter.clone()
hltEG17WP90RhoOneOEMinusOneOPFilterv2.barrelcut = cms.double( 999.9 )
hltEG17WP90RhoOneOEMinusOneOPFilterv2.endcapcut = cms.double( 999.9 )
hltEG17WP90RhoOneOEMinusOneOPFilterv2.ncandcut = cms.int32(2)
hltEG17WP90RhoOneOEMinusOneOPFilterv2.candTag = cms.InputTag("hltEG17WP90RhoPixelMatchFilterv2")
hltEG17WP90RhoOneOEMinusOneOPFilterv2.electronIsolatedProducer = cms.InputTag("hltActivityEleAnyPixelMatchElectronsL1Seeded")

## Deta filter
hltEG17WP90RhoDetaFilterv2 = hltEleDetaFilter.clone()
hltEG17WP90RhoDetaFilterv2.thrRegularEE = cms.double( 0.006 )
hltEG17WP90RhoDetaFilterv2.thrRegularEB = cms.double( 0.006 )
hltEG17WP90RhoDetaFilterv2.ncandcut = cms.int32(2)
hltEG17WP90RhoDetaFilterv2.candTag = cms.InputTag("hltEG17WP90RhoOneOEMinusOneOPFilterv2")
hltEG17WP90RhoDetaFilterv2.L1IsoCand = cms.InputTag("hltEG17WP90RhoPixelMatchFilterv2")
hltEG17WP90RhoDetaFilterv2.isoTag = cms.InputTag('hltActivityEleAnyL1SeededDetaDphi','Deta')

## Dphi filter
hltEG17WP90RhoDphiFilterv2 = hltEleDphiFilter.clone()
hltEG17WP90RhoDphiFilterv2.thrRegularEE = cms.double( 0.05 )
hltEG17WP90RhoDphiFilterv2.thrRegularEB = cms.double( 0.04 )
hltEG17WP90RhoDphiFilterv2.ncandcut = cms.int32(2)
hltEG17WP90RhoDphiFilterv2.candTag = cms.InputTag("hltEG17WP90RhoDetaFilterv2")
hltEG17WP90RhoDphiFilterv2.L1IsoCand = cms.InputTag("hltEG17WP90RhoPixelMatchFilterv2")
hltEG17WP90RhoDphiFilterv2.isoTag = cms.InputTag('hltActivityEleAnyL1SeededDetaDphi','Dphi')

## track iso filter
hltEG17WP90TrackRhoIsoFilterv2 = hltEleTrackIsoFilter.clone()
hltEG17WP90TrackRhoIsoFilterv2.thrOverPtEE = cms.double( 0.05 )
hltEG17WP90TrackRhoIsoFilterv2.thrOverPtEB = cms.double( 0.05 )
hltEG17WP90TrackRhoIsoFilterv2.ncandcut = cms.int32(2)
hltEG17WP90TrackRhoIsoFilterv2.candTag = cms.InputTag("hltEG17WP90RhoDphiFilterv2")
hltEG17WP90TrackRhoIsoFilterv2.L1IsoCand = cms.InputTag("hltEG17WP90RhoPixelMatchFilterv2")
hltEG17WP90TrackRhoIsoFilterv2.isoTag = cms.InputTag("hltActivityEleAnyL1SeededElectronTrackIso")

HLT_DoubleEle17_WP90_v1 = cms.Sequence( HLTDoRegionalEgammaEcalSequence + 
	                                HLTL1SeededEcalClustersSequence + 
		  	 	        hltL1SeededRecoEcalCandidate +  
		                        hltEGRegionalL1DoubleEG178+ 
				        hltEG17FilterDoubleEG17+ 
		                        hltL1SeededHLTClusterShape +
				        hltEG17WP90ClusterShapeFilter +
				        hltL1SeededPhotonEcalIsoRhoCorr + 
		                        hltEG17WP90EcalRhoIsoFilter +
				        HLTDoLocalHcalWithHOSequence + 
		                        hltL1SeededPhotonHcalForHE + 
				        hltEG17WP90RhoHEFilter + 
		                        hltL1SeededPhotonHcalIsoRhoCorr + 
				        hltEG17WP90HcalRhoIsoFilter + 
		                        HLTDoLocalPixelSequence + 
				        HLTDoLocalStripSequence + 
		                        hltL1SeededStartUpElectronPixelSeeds+
				        hltEG17WP90RhoPixelMatchFilter +
                                        hltCkfL1SeededTrackCandidates+
                                        hltCtfL1SeededWithMaterialTracks+
                                        hltEleAnyBarrelTracks+
                                        hltCtfL1SeededWithMaterialCleanTracks+
                                        hltEleAnyCleanMergedTracks+
                                        hltEleAnyPixelMatchElectronsL1Seeded +
                                        hltEG17WP90RhoOneOEMinusOneOPFilter +
                                        hltEleAnyL1SeededDetaDphi+
				        hltEG17WP90RhoDetaFilter+
		                        hltEG17WP90RhoDphiFilter+
                                        HLTL1SeededEgammaRegionalRecoTrackerSequence + 
				        hltEleAnyL1SeededElectronTrackIso +
		                        hltEG17WP90TrackRhoIsoFilter+
				        HLTEcalActivitySequence+
				        hltDoubleEG17EtFilterUnseeded+
		                        hltActivityPhotonClusterShape +
				        hltEG17WP90ClusterShapeFilterv2 +
		                        hltActivityPhotonEcalIsoRhoCorr+
				        hltEG17WP90EcalRhoIsoFilterv2+ 
				        hltActivityPhotonHcalForHE + 
				        hltEG17WP90RhoHEFilterv2+
				        hltActivityPhotonHcalIsoRhoCorr +
		                        hltEG17WP90HcalRhoIsoFilterv2 +
				        hltActivityStartUpElectronPixelSeeds + 
		                        hltEG17WP90RhoPixelMatchFilterv2 +
				        hltActivityCkfL1SeededTrackCandidates+
                                        hltActivityCtfL1SeededWithMaterialTracks+
                                        hltActivityEleAnyBarrelTracks+
                                        hltActivityCtfL1SeededWithMaterialCleanTracks+
                                        hltActivityEleAnyCleanMergedTracks+
                                        hltActivityEleAnyPixelMatchElectronsL1Seeded +
				        hltEG17WP90RhoOneOEMinusOneOPFilterv2+
		                        hltActivityEleAnyL1SeededDetaDphi +
				        hltEG17WP90RhoDetaFilterv2 +
		                        hltEG17WP90RhoDphiFilterv2 +
				        HLTL1ActivitySeededEgammaRegionalRecoTrackerSequence + 
		                        hltActivityEleAnyL1SeededElectronTrackIso +
		                        hltEG17WP90TrackRhoIsoFilterv2 +
				        hltEle17Ele8TrackIsoDZ)

