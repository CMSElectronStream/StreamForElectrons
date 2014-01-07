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
hltEGEtFilter.etcutEB = cms.double(25)
hltEGEtFilter.etcutEE = cms.double(25)
hltEGEtFilter.ncandcut = cms.int32(1)

## cluster shape filter
hltEleClusterShapeFilter.thrRegularEE = cms.double( 0.03 )
hltEleClusterShapeFilter.thrRegularEB = cms.double( 0.01 )
hltEleClusterShapeFilter.ncandcut = cms.int32(1)

## ecal iso filter
hltEleEcalIsoFilter.thrOverEEE = cms.double( 0.1 )
hltEleEcalIsoFilter.thrOverEEB = cms.double( 0.15 )
hltEleEcalIsoFilter.ncandcut = cms.int32(1)

### filter for HoverE
hltEleHEFilter.thrOverEEE = cms.double( 0.05 )
hltEleHEFilter.thrOverEEB = cms.double( 0.1 )
hltEleHEFilter.ncandcut = cms.int32(1)

### filter on HCal isolation for electron ID
hltEleHcalIsoFilter.thrOverEEE = cms.double( 0.1 )
hltEleHcalIsoFilter.thrOverEEB = cms.double( 0.1 )
hltEleHcalIsoFilter.ncandcut = cms.int32(1)

### filter the tracks looking at the electron candidates
hltElePixelMatchFilter.npixelmatchcut = cms.double(1.0)

## matching with the right collection of electron
hltElePixelMatchFilter.ncandcut = cms.int32(1)

hltEleOneOEMinusOneOPFilter.barrelcut = cms.double( 0.05 )
hltEleOneOEMinusOneOPFilter.endcapcut = cms.double( 0.05 )
hltEleOneOEMinusOneOPFilter.ncandcut = cms.int32(1)

## Deta filter
hltEleDetaFilter.thrRegularEE = cms.double( 0.007 )
hltEleDetaFilter.thrRegularEB = cms.double( 0.007 )
hltEleDetaFilter.ncandcut = cms.int32(1)

## Dphi filter
hltEleDphiFilter.thrRegularEE = cms.double( 0.03 )
hltEleDphiFilter.thrRegularEE = cms.double( 0.06 )
hltEleDphiFilter.ncandcut = cms.int32(1)

## track iso filter
hltEleTrackIsoFilter.thrOverPtEE = cms.double( 0.05 )
hltEleTrackIsoFilter.thrOverPtEB = cms.double( 0.05 )
hltEleTrackIsoFilter.ncandcut = cms.int32(1)

### Final Sequence
HLTEle25WP80Sequence = cms.Sequence( hltPreEle+
	                             HLTDoRegionalEgammaEcalSequence+
                                     HLTL1SeededEcalClustersSequence+
                                     hltL1SeededRecoEcalCandidate+ 
                                     hltEGRegionalL1SingleEG20ORL1SingleEG22+ 
                                     hltEGEtFilter +
                                     hltL1SeededHLTClusterShape +
                                     hltEleClusterShapeFilter+
                                     HLTDoLocalHcalWithHOSequence+					     
                                     hltL1SeededPhotonEcalIso+
                                     hltEleEcalIsoFilter+
                                     hltL1SeededPhotonHcalForHE +
                                     hltEleHEFilter+ 
                                     hltL1SeededPhotonHcalIso +
                                     hltEleHcalIsoFilter + 
                                     HLTDoLocalPixelSequence +
                                     HLTDoLocalStripSequence +
                                     hltL1SeededStartUpElectronPixelSeeds+ 
                                     hltElePixelMatchFilter+
                                     hltCkfL1SeededTrackCandidates+
                                     hltCtfL1SeededWithMaterialTracks+
                                     hltEleAnyBarrelTracks+
                                     hltCtfL1SeededWithMaterialCleanTracks+
                                     hltEleAnyCleanMergedTracks+
                                     hltEleAnyPixelMatchElectronsL1Seeded +
				     hltElePixelMatchFilter+
                                     hltEleOneOEMinusOneOPFilter+
                                     hltEleAnyL1SeededDetaDphi+
                                     hltEleDetaFilter+ 
                                     hltEleDphiFilter+
                                     HLTL1SeededEgammaRegionalRecoTrackerSequence +
                                     hltEleAnyL1SeededElectronTrackIso +
                                     hltEleTrackIsoFilter
	)
