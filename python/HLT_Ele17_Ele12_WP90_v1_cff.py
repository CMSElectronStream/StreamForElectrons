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
hltEG1712FilterDoubleEG1712 = hltEGFilterDoubleEG.clone()
hltEG1712FilterDoubleEG1712.inputTag = cms.InputTag("hltEGRegionalL1DoubleEG178")
hltEG1712FilterDoubleEG1712.etcutEB = cms.double( 17.0 )
hltEG1712FilterDoubleEG1712.etcutEE = cms.double( 17.0 )

## cluster shape filter
hltEG1712WP90ClusterShapeFilter = hltEleClusterShapeFilter.clone()
hltEG1712WP90ClusterShapeFilter.thrRegularEE = cms.double( 0.03 )
hltEG1712WP90ClusterShapeFilter.thrRegularEB = cms.double( 0.01 )
hltEG1712WP90ClusterShapeFilter.ncandcut = cms.int32(1)
hltEG1712WP90ClusterShapeFilter.candTag = cms.InputTag("hltEG1712FilterDoubleEG1712")

## ecal iso filter
hltEG1712WP90EcalRhoIsoFilter = hltEleEcalIsoFilter.clone()
hltEG1712WP90EcalRhoIsoFilter.thrOverEEE = cms.double( 0.035 )
hltEG1712WP90EcalRhoIsoFilter.thrOverEEB = cms.double( 0.07 )
hltEG1712WP90EcalRhoIsoFilter.ncandcut = cms.int32(1)
hltEG1712WP90EcalRhoIsoFilter.candTag = cms.InputTag("hltEG1712WP90ClusterShapeFilter")
hltEG1712WP90EcalRhoIsoFilter.saveTags = cms.bool(True)
hltEG1712WP90EcalRhoIsoFilter.isoTag = cms.InputTag("hltL1SeededPhotonEcalIsoRhoCorr")

### filter for HoverE
hltEG1712WP90RhoHEFilter = hltEleHEFilter.clone()
hltEG1712WP90RhoHEFilter.thrOverEEE = cms.double( 0.05 )
hltEG1712WP90RhoHEFilter.thrOverEEB = cms.double( 0.05 )
hltEG1712WP90RhoHEFilter.ncandcut = cms.int32(1)
hltEG1712WP90RhoHEFilter.candTag = cms.InputTag("hltEG1712WP90EcalRhoIsoFilter")
hltEG1712WP90RhoHEFilter.saveTags = cms.bool(True)

### filter on HCal isolation for electron ID
hltEG1712WP90HcalRhoIsoFilter = hltEleHcalIsoFilter.clone()
hltEG1712WP90HcalRhoIsoFilter.thrOverEEE = cms.double( 0.05 )
hltEG1712WP90HcalRhoIsoFilter.thrOverEEB = cms.double( 0.05 )
hltEG1712WP90HcalRhoIsoFilter.ncandcut = cms.int32(1)
hltEG1712WP90HcalRhoIsoFilter.candTag = cms.InputTag("hltEG1712WP90RhoHEFilter")
hltEG1712WP90HcalRhoIsoFilter.saveTags = cms.bool(True)
hltEG1712WP90HcalRhoIsoFilter.isoTag = cms.InputTag("hltL1SeededPhotonHcalIsoRhoCorr")

### filter the tracks looking at the electron candidates
hltEG1712WP90RhoPixelMatchFilter = hltElePixelMatchFilter.clone()
hltEG1712WP90RhoPixelMatchFilter.npixelmatchcut = cms.double(1.0)
hltEG1712WP90RhoPixelMatchFilter.ncandcut = cms.int32(1)
hltEG1712WP90RhoPixelMatchFilter.candTag = cms.InputTag("hltEG1712WP90HcalRhoIsoFilter")

###
hltEG1712WP90RhoOneOEMinusOneOPFilter = hltEleOneOEMinusOneOPFilter.clone()
hltEG1712WP90RhoOneOEMinusOneOPFilter.barrelcut = cms.double( 999.9 )
hltEG1712WP90RhoOneOEMinusOneOPFilter.endcapcut = cms.double( 999.9 )
hltEG1712WP90RhoOneOEMinusOneOPFilter.ncandcut = cms.int32(1)
hltEG1712WP90RhoOneOEMinusOneOPFilter.candTag = cms.InputTag("hltEG1712WP90RhoPixelMatchFilter")

## Deta filter
hltEG1712WP90RhoDetaFilter = hltEleDetaFilter.clone()
hltEG1712WP90RhoDetaFilter.thrRegularEE = cms.double( 0.006 )
hltEG1712WP90RhoDetaFilter.thrRegularEB = cms.double( 0.006 )
hltEG1712WP90RhoDetaFilter.ncandcut = cms.int32(1)
hltEG1712WP90RhoDetaFilter.candTag = cms.InputTag("hltEG1712WP90RhoOneOEMinusOneOPFilter")

## Dphi filter
hltEG1712WP90RhoDphiFilter = hltEleDphiFilter.clone()
hltEG1712WP90RhoDphiFilter.thrRegularEE = cms.double( 0.05 )
hltEG1712WP90RhoDphiFilter.thrRegularEB = cms.double( 0.04 )
hltEG1712WP90RhoDphiFilter.ncandcut = cms.int32(1)
hltEG1712WP90RhoDphiFilter.candTag = cms.InputTag("hltEG1712WP90RhoDetaFilter")

## track iso filter
hltEG1712WP90TrackRhoIsoFilter = hltEleTrackIsoFilter.clone()
hltEG1712WP90TrackRhoIsoFilter.thrOverPtEE = cms.double( 0.05 )
hltEG1712WP90TrackRhoIsoFilter.thrOverPtEB = cms.double( 0.05 )
hltEG1712WP90TrackRhoIsoFilter.ncandcut = cms.int32(1)
hltEG1712WP90TrackRhoIsoFilter.candTag = cms.InputTag("hltEG1712WP90RhoDphiFilter")

## et filter on the second electron
hltDoubleEG12EtFilterUnseeded = hltEGFilterDoubleEG.clone()
hltDoubleEG12EtFilterUnseeded.L1IsoCand = cms.InputTag("hltRecoEcalSuperClusterActivityCandidate")
hltDoubleEG12EtFilterUnseeded.inputTag = cms.InputTag("hltEcalActivitySuperClusterWrapper")
hltDoubleEG12EtFilterUnseeded.ncandcut = cms.int32( 2 )
hltDoubleEG12EtFilterUnseeded.etcutEB = cms.double( 12.0 )
hltDoubleEG12EtFilterUnseeded.etcutEE = cms.double( 12.0 )

## cluster shape filter
hltEG1712WP90ClusterShapeFilterv2 = hltEleClusterShapeFilter.clone()
hltEG1712WP90ClusterShapeFilterv2.thrRegularEE = cms.double( 0.03 )
hltEG1712WP90ClusterShapeFilterv2.thrRegularEB = cms.double( 0.01 )
hltEG1712WP90ClusterShapeFilterv2.ncandcut  = cms.int32(2)
hltEG1712WP90ClusterShapeFilterv2.candTag   = cms.InputTag("hltDoubleEG12EtFilterUnseeded")
hltEG1712WP90ClusterShapeFilterv2.L1IsoCand = cms.InputTag("hltRecoEcalSuperClusterActivityCandidate")
hltEG1712WP90ClusterShapeFilterv2.isoTag    = cms.InputTag("hltActivityPhotonClusterShape")

## ecal iso filter
hltEG1712WP90EcalRhoIsoFilterv2 = hltEleEcalIsoFilter.clone()
hltEG1712WP90EcalRhoIsoFilterv2.thrOverEEE = cms.double( 0.035 )
hltEG1712WP90EcalRhoIsoFilterv2.thrOverEEB = cms.double( 0.07 )
hltEG1712WP90EcalRhoIsoFilterv2.ncandcut   = cms.int32(2)
hltEG1712WP90EcalRhoIsoFilterv2.candTag    = cms.InputTag("hltEG1712WP90ClusterShapeFilterv2")
hltEG1712WP90EcalRhoIsoFilterv2.saveTags   = cms.bool(True)
hltEG1712WP90EcalRhoIsoFilterv2.isoTag     = cms.InputTag("hltActivityPhotonEcalIsoRhoCorr")
hltEG1712WP90EcalRhoIsoFilterv2.L1IsoCand  = cms.InputTag("hltRecoEcalSuperClusterActivityCandidate")

### filter for HoverE
hltEG1712WP90RhoHEFilterv2 = hltEleHEFilter.clone()
hltEG1712WP90RhoHEFilterv2.thrOverEEE = cms.double( 0.05 )
hltEG1712WP90RhoHEFilterv2.thrOverEEB = cms.double( 0.05 )
hltEG1712WP90RhoHEFilterv2.ncandcut   = cms.int32(2)
hltEG1712WP90RhoHEFilterv2.candTag    = cms.InputTag("hltEG1712WP90EcalRhoIsoFilterv2")
hltEG1712WP90RhoHEFilterv2.isoTag     = cms.InputTag("hltActivityPhotonHcalForHE")
hltEG1712WP90RhoHEFilterv2.L1IsoCand  = cms.InputTag("hltRecoEcalSuperClusterActivityCandidate")
hltEG1712WP90RhoHEFilterv2.saveTags   = cms.bool(True)

### filter on HCal isolation for electron ID
hltEG1712WP90HcalRhoIsoFilterv2 = hltEleHcalIsoFilter.clone()
hltEG1712WP90HcalRhoIsoFilterv2.thrOverEEE = cms.double( 0.05 )
hltEG1712WP90HcalRhoIsoFilterv2.thrOverEEB = cms.double( 0.05 )
hltEG1712WP90HcalRhoIsoFilterv2.ncandcut = cms.int32(2)
hltEG1712WP90HcalRhoIsoFilterv2.candTag = cms.InputTag("hltEG1712WP90RhoHEFilterv2")
hltEG1712WP90HcalRhoIsoFilterv2.saveTags = cms.bool(True)
hltEG1712WP90HcalRhoIsoFilterv2.isoTag = cms.InputTag("hltActivityPhotonHcalIsoRhoCorr")
hltEG1712WP90HcalRhoIsoFilterv2.L1IsoCand  = cms.InputTag("hltRecoEcalSuperClusterActivityCandidate")

### filter the tracks looking at the electron candidates
hltEG1712WP90RhoPixelMatchFilterv2 = hltElePixelMatchFilter.clone()
hltEG1712WP90RhoPixelMatchFilterv2.npixelmatchcut = cms.double(1.0)
hltEG1712WP90RhoPixelMatchFilterv2.ncandcut = cms.int32(2)
hltEG1712WP90RhoPixelMatchFilterv2.candTag = cms.InputTag("hltEG1712WP90HcalRhoIsoFilterv2")
hltEG1712WP90RhoPixelMatchFilterv2.L1IsoPixelSeedsTag = cms.InputTag("hltActivityStartUpElectronPixelSeeds")
hltEG1712WP90RhoPixelMatchFilterv2.L1IsoCand = cms.InputTag("hltRecoEcalSuperClusterActivityCandidate")

###
hltEG1712WP90RhoOneOEMinusOneOPFilterv2 = hltEleOneOEMinusOneOPFilter.clone()
hltEG1712WP90RhoOneOEMinusOneOPFilterv2.barrelcut = cms.double( 999.9 )
hltEG1712WP90RhoOneOEMinusOneOPFilterv2.endcapcut = cms.double( 999.9 )
hltEG1712WP90RhoOneOEMinusOneOPFilterv2.ncandcut = cms.int32(2)
hltEG1712WP90RhoOneOEMinusOneOPFilterv2.candTag = cms.InputTag("hltEG1712WP90RhoPixelMatchFilterv2")
hltEG1712WP90RhoOneOEMinusOneOPFilterv2.electronIsolatedProducer = cms.InputTag("hltActivityEleAnyPixelMatchElectronsL1Seeded")

## Deta filter
hltEG1712WP90RhoDetaFilterv2 = hltEleDetaFilter.clone()
hltEG1712WP90RhoDetaFilterv2.thrRegularEE = cms.double( 0.006 )
hltEG1712WP90RhoDetaFilterv2.thrRegularEB = cms.double( 0.006 )
hltEG1712WP90RhoDetaFilterv2.ncandcut = cms.int32(2)
hltEG1712WP90RhoDetaFilterv2.candTag = cms.InputTag("hltEG1712WP90RhoOneOEMinusOneOPFilterv2")
hltEG1712WP90RhoDetaFilterv2.L1IsoCand = cms.InputTag("hltEG1712WP90RhoPixelMatchFilterv2")
hltEG1712WP90RhoDetaFilterv2.isoTag = cms.InputTag('hltActivityEleAnyL1SeededDetaDphi','Deta')

## Dphi filter
hltEG1712WP90RhoDphiFilterv2 = hltEleDphiFilter.clone()
hltEG1712WP90RhoDphiFilterv2.thrRegularEE = cms.double( 0.05 )
hltEG1712WP90RhoDphiFilterv2.thrRegularEB = cms.double( 0.04 )
hltEG1712WP90RhoDphiFilterv2.ncandcut = cms.int32(2)
hltEG1712WP90RhoDphiFilterv2.candTag = cms.InputTag("hltEG1712WP90RhoDetaFilterv2")
hltEG1712WP90RhoDphiFilterv2.L1IsoCand = cms.InputTag("hltEG1712WP90RhoPixelMatchFilterv2")
hltEG1712WP90RhoDphiFilterv2.isoTag = cms.InputTag('hltActivityEleAnyL1SeededDetaDphi','Dphi')

## track iso filter
hltEG1712WP90TrackRhoIsoFilterv2 = hltEleTrackIsoFilter.clone()
hltEG1712WP90TrackRhoIsoFilterv2.thrOverPtEE = cms.double( 0.05 )
hltEG1712WP90TrackRhoIsoFilterv2.thrOverPtEB = cms.double( 0.05 )
hltEG1712WP90TrackRhoIsoFilterv2.ncandcut = cms.int32(2)
hltEG1712WP90TrackRhoIsoFilterv2.candTag = cms.InputTag("hltEG1712WP90RhoDphiFilterv2")
hltEG1712WP90TrackRhoIsoFilterv2.L1IsoCand = cms.InputTag("hltEG1712WP90RhoPixelMatchFilterv2")
hltEG1712WP90TrackRhoIsoFilterv2.isoTag = cms.InputTag("hltActivityEleAnyL1SeededElectronTrackIso")

HLT_Ele17_Ele12_WP90_v1 = cms.Sequence( HLTDoRegionalEgammaEcalSequence + 
	                               HLTL1SeededEcalClustersSequence + 
				       hltL1SeededRecoEcalCandidate +  
		                       hltEGRegionalL1DoubleEG178+ 
				       hltEG1712FilterDoubleEG1712+ 
		                       hltL1SeededHLTClusterShape +
				       hltEG1712WP90ClusterShapeFilter +
				       hltL1SeededPhotonEcalIsoRhoCorr + 
		                       hltEG1712WP90EcalRhoIsoFilter +
				       HLTDoLocalHcalWithHOSequence + 
		                       hltL1SeededPhotonHcalForHE + 
				       hltEG1712WP90RhoHEFilter + 
		                       hltL1SeededPhotonHcalIsoRhoCorr + 
				       hltEG1712WP90HcalRhoIsoFilter + 
		                       HLTDoLocalPixelSequence + 
				       HLTDoLocalStripSequence + 
		                       hltL1SeededStartUpElectronPixelSeeds+
				       hltEG1712WP90RhoPixelMatchFilter +
                                       hltCkfL1SeededTrackCandidates+
                                       hltCtfL1SeededWithMaterialTracks+
                                       hltEleAnyWPBarrelTracks+
                                       hltCtfL1SeededWithMaterialCleanTracks+
                                       hltEleAnyCleanMergedTracks+
                                       hltEleAnyPixelMatchElectronsL1Seeded +
                                       hltEG1712WP90RhoOneOEMinusOneOPFilter +
                                       hltEleAnyL1SeededDetaDphi+
				       hltEG1712WP90RhoDetaFilter+
		                       hltEG1712WP90RhoDphiFilter+
                                       HLTL1SeededEgammaRegionalRecoTrackerSequence + 
				       hltEleAnyL1SeededElectronTrackIso +
		                       hltEG1712WP90TrackRhoIsoFilter+
				       HLTEcalActivitySequence+
				       hltDoubleEG12EtFilterUnseeded+
		                       hltActivityPhotonClusterShape +
				       hltEG1712WP90ClusterShapeFilterv2 +
		                       hltActivityPhotonEcalIsoRhoCorr+
				       hltEG1712WP90EcalRhoIsoFilterv2+ 
				       hltActivityPhotonHcalForHE + 
				       hltEG1712WP90RhoHEFilterv2+
				       hltActivityPhotonHcalIsoRhoCorr +
		                       hltEG1712WP90HcalRhoIsoFilterv2 +
				       hltActivityStartUpElectronPixelSeeds + 
		                       hltEG1712WP90RhoPixelMatchFilterv2 +
				       hltActivityCkfL1SeededTrackCandidates+
                                       hltActivityCtfL1SeededWithMaterialTracks+
                                       hltActivityEleAnyBarrelTracks+
                                       hltActivityCtfL1SeededWithMaterialCleanTracks+
                                       hltActivityEleAnyCleanMergedTracks+
                                       hltActivityEleAnyPixelMatchElectronsL1Seeded +
				       hltEG1712WP90RhoOneOEMinusOneOPFilterv2+
		                       hltActivityEleAnyL1SeededDetaDphi +
				       hltEG1712WP90RhoDetaFilterv2 +
		                       hltEG1712WP90RhoDphiFilterv2 +
				       HLTL1ActivitySeededEgammaRegionalRecoTrackerSequence + 
		                       hltActivityEleAnyL1SeededElectronTrackIso +
		                       hltEG1712WP90TrackRhoIsoFilterv2 +
				       hltEle17Ele8TrackIsoDZ)

