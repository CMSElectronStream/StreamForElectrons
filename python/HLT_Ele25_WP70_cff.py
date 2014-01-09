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

## fix threshold for et cut
hltEG25WP70EtFilter = hltEGEtFilter.clone()
hltEG25WP70EtFilter.etcutEB = cms.double(25)
hltEG25WP70EtFilter.etcutEE = cms.double(25)
hltEG25WP70EtFilter.ncandcut = cms.int32(1)

## cluster shape filter
hltEle25WP70ClusterShapeFilter = hltEleClusterShapeFilter.clone()
hltEle25WP70ClusterShapeFilter.thrRegularEE = cms.double( 0.03 )
hltEle25WP70ClusterShapeFilter.thrRegularEB = cms.double( 0.01 )
hltEle25WP70ClusterShapeFilter.ncandcut = cms.int32(1)
hltEle25WP70ClusterShapeFilter.candTag = cms.InputTag("hltEG25WP70EtFilter")

## ecal iso filter
hltEle25WP70EcalIsoFilter = hltEleEcalIsoFilter.clone()
hltEle25WP70EcalIsoFilter.thrOverEEE = cms.double( 0.1 )
hltEle25WP70EcalIsoFilter.thrOverEEB = cms.double( 0.15 )
hltEle25WP70EcalIsoFilter.ncandcut = cms.int32(1)
hltEle25WP70EcalIsoFilter.candTag = cms.InputTag("hltEle25WP70ClusterShapeFilter")

### filter for HoverE
hltEle25WP70HEFilter = hltEleHEFilter.clone()
hltEle25WP70HEFilter.thrOverEEE = cms.double( 0.05 )
hltEle25WP70HEFilter.thrOverEEB = cms.double( 0.05 )
hltEle25WP70HEFilter.ncandcut = cms.int32(1)
hltEle25WP70HEFilter.candTag = cms.InputTag("hltEle25WP70EcalIsoFilter")

### filter on HCal isolation for electron ID
hltEle25WP70HcalIsoFilter = hltEleHcalIsoFilter.clone()
hltEle25WP70HcalIsoFilter.thrOverEEE = cms.double( 0.1 )
hltEle25WP70HcalIsoFilter.thrOverEEB = cms.double( 0.1 )
hltEle25WP70HcalIsoFilter.ncandcut = cms.int32(1)
hltEle25WP70HcalIsoFilter.candTag = cms.InputTag("hltEle25WP70HEFilter")

### filter the tracks looking at the electron candidates
hltEle25WP70PixelMatchFilter = hltElePixelMatchFilter.clone()
hltEle25WP70PixelMatchFilter.npixelmatchcut = cms.double(1.0)
hltEle25WP70PixelMatchFilter.ncandcut = cms.int32(1)
hltEle25WP70PixelMatchFilter.candTag = cms.InputTag("hltEle25WP70HcalIsoFilter")

###
hltEle25WP70OneOEMinusOneOPFilter = hltEleOneOEMinusOneOPFilter.clone()
hltEle25WP70OneOEMinusOneOPFilter.barrelcut = cms.double( 0.05 )
hltEle25WP70OneOEMinusOneOPFilter.endcapcut = cms.double( 0.05 )
hltEle25WP70OneOEMinusOneOPFilter.ncandcut = cms.int32(1)
hltEle25WP70OneOEMinusOneOPFilter.candTag = cms.InputTag("hltEle25WP70PixelMatchFilter")

## Deta filter
hltEle25WP70DetaFilter = hltEleDetaFilter.clone()
hltEle25WP70DetaFilter.thrRegularEE = cms.double( 0.006 )
hltEle25WP70DetaFilter.thrRegularEB = cms.double( 0.004 )
hltEle25WP70DetaFilter.ncandcut = cms.int32(1)
hltEle25WP70DetaFilter.candTag = cms.InputTag("hltEle25WP70OneOEMinusOneOPFilter")

## Dphi filter
hltEle25WP70DphiFilter = hltEleDphiFilter.clone()
hltEle25WP70DphiFilter.thrRegularEE = cms.double( 0.03 )
hltEle25WP70DphiFilter.thrRegularEB = cms.double( 0.04 )
hltEle25WP70DphiFilter.ncandcut = cms.int32(1)
hltEle25WP70DphiFilter.candTag = cms.InputTag("hltEle25WP70DetaFilter")

## track iso filter
hltEle25WP70TrackIsoFilter = hltEleTrackIsoFilter.clone()
hltEle25WP70TrackIsoFilter.thrOverPtEE = cms.double( 0.05 )
hltEle25WP70TrackIsoFilter.thrOverPtEB = cms.double( 0.05 )
hltEle25WP70TrackIsoFilter.ncandcut = cms.int32(1)
hltEle25WP70TrackIsoFilter.candTag = cms.InputTag("hltEle25WP70DphiFilter")

### Final Sequence
HLTEle25WP70Sequence = cms.Sequence( hltPreEle+
	                             HLTDoRegionalEgammaEcalSequence+
                                     HLTL1SeededEcalClustersSequence+
                                     hltL1SeededRecoEcalCandidate+ 
                                     hltEGRegionalL1SingleEG20ORL1SingleEG22+ 
                                     hltEG25WP70EtFilter +
                                     hltL1SeededHLTClusterShape +
                                     hltEle25WP70ClusterShapeFilter+
                                     HLTDoLocalHcalWithHOSequence+					     
                                     hltL1SeededPhotonEcalIso+
                                     hltEle25WP70EcalIsoFilter+
                                     hltL1SeededPhotonHcalForHE +
                                     hltEle25WP70HEFilter+ 
                                     hltL1SeededPhotonHcalIso +
                                     hltEle25WP70HcalIsoFilter + 
                                     HLTDoLocalPixelSequence +
                                     HLTDoLocalStripSequence +
                                     hltL1SeededStartUpElectronPixelSeeds+ 
                                     hltEle25WP70PixelMatchFilter+
                                     hltCkfL1SeededTrackCandidates+
                                     hltCtfL1SeededWithMaterialTracks+
                                     hltEleAnyBarrelTracks+
                                     hltCtfL1SeededWithMaterialCleanTracks+
                                     hltEleAnyCleanMergedTracks+
                                     hltEleAnyPixelMatchElectronsL1Seeded +
                                     hltEle25WP70OneOEMinusOneOPFilter+
                                     hltEleAnyL1SeededDetaDphi+
                                     hltEle25WP70DetaFilter+ 
                                     hltEle25WP70DphiFilter+
                                     HLTL1SeededEgammaRegionalRecoTrackerSequence +
                                     hltEleAnyL1SeededElectronTrackIso +
                                     hltEle25WP70TrackIsoFilter)

