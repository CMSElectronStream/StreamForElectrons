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

from HLTrigger.Configuration.HLT_FastVertexing_cff import *

## fix threshold for et cut
hltEG25WP80EtFilter = hltEGEtFilter.clone()
hltEG25WP80EtFilter.etcutEB = cms.double(25)
hltEG25WP80EtFilter.etcutEE = cms.double(25)
hltEG25WP80EtFilter.ncandcut = cms.int32(1)

## cluster shape filter
hltEle25WP80ClusterShapeFilter = hltEleClusterShapeFilter.clone()
hltEle25WP80ClusterShapeFilter.thrRegularEE = cms.double( 0.03 )
hltEle25WP80ClusterShapeFilter.thrRegularEB = cms.double( 0.01 )
hltEle25WP80ClusterShapeFilter.ncandcut = cms.int32(1)
hltEle25WP80ClusterShapeFilter.candTag = cms.InputTag("hltEG25WP80EtFilter")

## ecal iso filter
hltEle25WP80EcalIsoFilter = hltEleEcalIsoFilter.clone()
hltEle25WP80EcalIsoFilter.thrOverEEE = cms.double( 0.1 )
hltEle25WP80EcalIsoFilter.thrOverEEB = cms.double( 0.15 )
hltEle25WP80EcalIsoFilter.ncandcut = cms.int32(1)
hltEle25WP80EcalIsoFilter.candTag = cms.InputTag("hltEle25WP80ClusterShapeFilter")

### filter for HoverE
hltEle25WP80HEFilter = hltEleHEFilter.clone()
hltEle25WP80HEFilter.thrOverEEE = cms.double( 0.05 )
hltEle25WP80HEFilter.thrOverEEB = cms.double( 0.1 )
hltEle25WP80HEFilter.ncandcut = cms.int32(1)
hltEle25WP80HEFilter.candTag = cms.InputTag("hltEle25WP80EcalIsoFilter")

### filter on HCal isolation for electron ID
hltEle25WP80HcalIsoFilter = hltEleHcalIsoFilter.clone()
hltEle25WP80HcalIsoFilter.thrOverEEE = cms.double( 0.1 )
hltEle25WP80HcalIsoFilter.thrOverEEB = cms.double( 0.1 )
hltEle25WP80HcalIsoFilter.ncandcut = cms.int32(1)
hltEle25WP80HcalIsoFilter.candTag = cms.InputTag("hltEle25WP80HEFilter")

### filter the tracks looking at the electron candidates
hltEle25WP80PixelMatchFilter = hltElePixelMatchFilter.clone()
hltEle25WP80PixelMatchFilter.npixelmatchcut = cms.double(1.0)
hltEle25WP80PixelMatchFilter.ncandcut = cms.int32(1)
hltEle25WP80PixelMatchFilter.candTag = cms.InputTag("hltEle25WP80HcalIsoFilter")

###
hltEle25WP80OneOEMinusOneOPFilter = hltEleOneOEMinusOneOPFilter.clone()
hltEle25WP80OneOEMinusOneOPFilter.barrelcut = cms.double( 0.05 )
hltEle25WP80OneOEMinusOneOPFilter.endcapcut = cms.double( 0.05 )
hltEle25WP80OneOEMinusOneOPFilter.ncandcut = cms.int32(1)
hltEle25WP80OneOEMinusOneOPFilter.candTag = cms.InputTag("hltEle25WP80PixelMatchFilter")

## Deta filter
hltEle25WP80DetaFilter = hltEleDetaFilter.clone()
hltEle25WP80DetaFilter.thrRegularEE = cms.double( 0.007 )
hltEle25WP80DetaFilter.thrRegularEB = cms.double( 0.007 )
hltEle25WP80DetaFilter.ncandcut = cms.int32(1)
hltEle25WP80DetaFilter.candTag = cms.InputTag("hltEle25WP80OneOEMinusOneOPFilter")

## Dphi filter
hltEle25WP80DphiFilter = hltEleDphiFilter.clone()
hltEle25WP80DphiFilter.thrRegularEE = cms.double( 0.03 )
hltEle25WP80DphiFilter.thrRegularEB = cms.double( 0.06 )
hltEle25WP80DphiFilter.ncandcut = cms.int32(1)
hltEle25WP80DphiFilter.candTag = cms.InputTag("hltEle25WP80DetaFilter")

## track iso filter
hltEle25WP80TrackIsoFilter = hltEleTrackIsoFilter.clone()
hltEle25WP80TrackIsoFilter.thrOverPtEE = cms.double( 0.05 )
hltEle25WP80TrackIsoFilter.thrOverPtEB = cms.double( 0.05 )
hltEle25WP80TrackIsoFilter.ncandcut = cms.int32(1)
hltEle25WP80TrackIsoFilter.candTag = cms.InputTag("hltEle25WP80DphiFilter")

### Final Sequence
HLTEle25WP80Sequence = cms.Sequence( hltPreEle+
	                             HLTDoRegionalEgammaEcalSequence+
                                     HLTL1SeededEcalClustersSequence+
                                     hltL1SeededRecoEcalCandidate+ 
                                     hltEGRegionalL1SingleEG20ORL1SingleEG22+ 
                                     hltEG25WP80EtFilter +
                                     hltL1SeededHLTClusterShape +
                                     hltEle25WP80ClusterShapeFilter+
                                     HLTDoLocalHcalWithHOSequence+					     
                                     hltL1SeededPhotonEcalIso+
                                     hltEle25WP80EcalIsoFilter+
                                     hltL1SeededPhotonHcalForHE +
                                     hltEle25WP80HEFilter+ 
                                     hltL1SeededPhotonHcalIso +
                                     hltEle25WP80HcalIsoFilter+ 
                                     HLTDoLocalPixelSequence +
                                     HLTDoLocalStripSequence +
                                     hltL1SeededStartUpElectronPixelSeeds+ 
                                     hltEle25WP80PixelMatchFilter+
                                     hltCkfL1SeededTrackCandidates+
                                     hltCtfL1SeededWithMaterialTracks+
                                     hltEleAnyWPBarrelTracks+
                                     hltCtfL1SeededWithMaterialCleanTracks+
                                     hltEleAnyCleanMergedTracks+
                                     hltEleAnyPixelMatchElectronsL1Seeded +
                                     hltEle25WP80OneOEMinusOneOPFilter+
                                     hltEleAnyL1SeededDetaDphi+
                                     hltEle25WP80DetaFilter+ 
                                     hltEle25WP80DphiFilter+
                                     HLTL1SeededEgammaRegionalRecoTrackerSequence +
                                     hltEleAnyL1SeededElectronTrackIso +
                                     hltEle25WP80TrackIsoFilter+
                                     HLTFastPrimaryVertexSequence)

#############################
### using gsf electrons  ####
#############################

from HLTrigger.Configuration.HLT_GsfElectronTrackSequence_cff import *

hltGsfEle25WP80OneOEMinusOneOPFilter = hltEleOneOEMinusOneOPFilter.clone()
hltGsfEle25WP80OneOEMinusOneOPFilter.barrelcut = cms.double( 0.05 )
hltGsfEle25WP80OneOEMinusOneOPFilter.endcapcut = cms.double( 0.05 )
hltGsfEle25WP80OneOEMinusOneOPFilter.ncandcut = cms.int32(1)
hltGsfEle25WP80OneOEMinusOneOPFilter.candTag = cms.InputTag("hltEle25WP80PixelMatchFilter")
hltGsfEle25WP80OneOEMinusOneOPFilter.electronIsolatedProducer = cms.InputTag("hltL1SeededGsfElectrons")
hltGsfEle25WP80OneOEMinusOneOPFilter.useGsfElectron = cms.bool(True)

## Deta filter
hltGsfEle25WP80DetaFilter = hltEleDetaFilter.clone()
hltGsfEle25WP80DetaFilter.thrRegularEE = cms.double( 0.007 )
hltGsfEle25WP80DetaFilter.thrRegularEB = cms.double( 0.007 )
hltGsfEle25WP80DetaFilter.ncandcut = cms.int32(1)
hltGsfEle25WP80DetaFilter.candTag = cms.InputTag("hltGsfEle25WP80OneOEMinusOneOPFilter")
hltGsfEle25WP80DetaFilter.L1IsoCand = cms.InputTag("hltEle25WP80PixelMatchFilter")
hltGsfEle25WP80DetaFilter.isoTag = cms.InputTag( 'hltL1SeededGsfTrackVars','Deta' )

## Dphi filter
hltGsfEle25WP80DphiFilter = hltEleDphiFilter.clone()
hltGsfEle25WP80DphiFilter.thrRegularEE = cms.double( 0.03 )
hltGsfEle25WP80DphiFilter.thrRegularEB = cms.double( 0.06 )
hltGsfEle25WP80DphiFilter.ncandcut = cms.int32(1)
hltGsfEle25WP80DphiFilter.candTag = cms.InputTag("hltGsfEle25WP80DetaFilter")
hltGsfEle25WP80DphiFilter.L1IsoCand = cms.InputTag("hltEle25WP80PixelMatchFilter")
hltGsfEle25WP80DphiFilter.isoTag = cms.InputTag( 'hltL1SeededGsfTrackVars','Dphi' )

## track iso filter
hltGsfEle25WP80TrackIsoFilter = hltEleTrackIsoFilter.clone()
hltGsfEle25WP80TrackIsoFilter.thrOverPtEE = cms.double( 0.05 )
hltGsfEle25WP80TrackIsoFilter.thrOverPtEB = cms.double( 0.05 )
hltGsfEle25WP80TrackIsoFilter.ncandcut = cms.int32(1)
hltGsfEle25WP80TrackIsoFilter.candTag = cms.InputTag("hltGsfEle25WP80DphiFilter")
hltGsfEle25WP80TrackIsoFilter.L1IsoCand = cms.InputTag("hltEle25WP80PixelMatchFilter")
hltGsfEle25WP80TrackIsoFilter.isoTag = cms.InputTag("hltGsfEleAnyL1SeededElectronTrackIso")

HLTGsfEle25WP80Sequence = cms.Sequence(	hltPreEle+
	                                HLTDoRegionalEgammaEcalSequence+
                                        HLTL1SeededEcalClustersSequence+
                                        hltL1SeededRecoEcalCandidate+ 
                                        hltEGRegionalL1SingleEG20ORL1SingleEG22+ 
                                        hltEG25WP80EtFilter +
                                        hltL1SeededHLTClusterShape +
                                        hltEle25WP80ClusterShapeFilter+
                                        HLTDoLocalHcalWithHOSequence+					     
                                        hltL1SeededPhotonEcalIso+
                                        hltEle25WP80EcalIsoFilter+
                                        hltL1SeededPhotonHcalForHE +
                                        hltEle25WP80HEFilter+ 
                                        hltL1SeededPhotonHcalIso +
                                        hltEle25WP80HcalIsoFilter+ 
                                        HLTDoLocalPixelSequence +
                                        HLTDoLocalStripSequence +
                                        hltL1SeededStartUpElectronPixelSeeds+ 
				        hltEle25WP80PixelMatchFilter+
                                        HLTL1SeededGsfElectronSequence+
                                        hltGsfEle25WP80OneOEMinusOneOPFilter+
                                        hltGsfEle25WP80DetaFilter+
                                        hltGsfEle25WP80DphiFilter+
                                        HLTL1SeededEgammaRegionalRecoTrackerSequence +
                                        hltGsfEleAnyL1SeededElectronTrackIso+
                                        hltGsfEle25WP80TrackIsoFilter+
                                        HLTFastPrimaryVertexSequence)
