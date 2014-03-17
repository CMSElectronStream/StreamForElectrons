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
hltEG25WP90EtFilter = hltEGEtFilter.clone()
hltEG25WP90EtFilter.etcutEB = cms.double(25)
hltEG25WP90EtFilter.etcutEE = cms.double(25)
hltEG25WP90EtFilter.ncandcut = cms.int32(1)

## cluster shape filter
hltEle25WP90ClusterShapeFilter = hltEleClusterShapeFilter.clone()
hltEle25WP90ClusterShapeFilter.thrRegularEE = cms.double( 0.03 )
hltEle25WP90ClusterShapeFilter.thrRegularEB = cms.double( 0.01 )
hltEle25WP90ClusterShapeFilter.ncandcut = cms.int32(1)
hltEle25WP90ClusterShapeFilter.candTag = cms.InputTag("hltEG25WP90EtFilter")

## ecal iso filter
hltEle25WP90EcalRhoIsoFilter = hltEleEcalIsoFilter.clone()
hltEle25WP90EcalRhoIsoFilter.thrOverEEE = cms.double( 0.035 )
hltEle25WP90EcalRhoIsoFilter.thrOverEEB = cms.double( 0.07 )
hltEle25WP90EcalRhoIsoFilter.ncandcut = cms.int32(1)
hltEle25WP90EcalRhoIsoFilter.candTag = cms.InputTag("hltEle25WP90ClusterShapeFilter")
hltEle25WP90EcalRhoIsoFilter.saveTags = cms.bool(True)
hltEle25WP90EcalRhoIsoFilter.isoTag = cms.InputTag("hltL1SeededPhotonEcalIsoRhoCorr")

### filter for HoverE
hltEle25WP90RhoHEFilter = hltEleHEFilter.clone()
hltEle25WP90RhoHEFilter.thrOverEEE = cms.double( 0.05 )
hltEle25WP90RhoHEFilter.thrOverEEB = cms.double( 0.05 )
hltEle25WP90RhoHEFilter.ncandcut = cms.int32(1)
hltEle25WP90RhoHEFilter.candTag = cms.InputTag("hltEle25WP90EcalRhoIsoFilter")
hltEle25WP90RhoHEFilter.saveTags = cms.bool(True)

### filter on HCal isolation for electron ID
hltEle25WP90HcalRhoIsoFilter = hltEleHcalIsoFilter.clone()
hltEle25WP90HcalRhoIsoFilter.thrOverEEE = cms.double( 0.05 )
hltEle25WP90HcalRhoIsoFilter.thrOverEEB = cms.double( 0.05 )
hltEle25WP90HcalRhoIsoFilter.ncandcut = cms.int32(1)
hltEle25WP90HcalRhoIsoFilter.candTag = cms.InputTag("hltEle25WP90RhoHEFilter")
hltEle25WP90HcalRhoIsoFilter.saveTags = cms.bool(True)
hltEle25WP90HcalRhoIsoFilter.isoTag = cms.InputTag("hltL1SeededPhotonHcalIsoRhoCorr")

### filter the tracks looking at the electron candidates
hltEle25WP90RhoPixelMatchFilter = hltElePixelMatchFilter.clone()
hltEle25WP90RhoPixelMatchFilter.npixelmatchcut = cms.double(1.0)
hltEle25WP90RhoPixelMatchFilter.ncandcut = cms.int32(1)
hltEle25WP90RhoPixelMatchFilter.candTag = cms.InputTag("hltEle25WP90HcalRhoIsoFilter")

###
hltEle25WP90RhoOneOEMinusOneOPFilter = hltEleOneOEMinusOneOPFilter.clone()
hltEle25WP90RhoOneOEMinusOneOPFilter.barrelcut = cms.double( 999.9 )
hltEle25WP90RhoOneOEMinusOneOPFilter.endcapcut = cms.double( 999.9 )
hltEle25WP90RhoOneOEMinusOneOPFilter.ncandcut = cms.int32(1)
hltEle25WP90RhoOneOEMinusOneOPFilter.candTag = cms.InputTag("hltEle25WP90RhoPixelMatchFilter")

## Deta filter
hltEle25WP90RhoDetaFilter = hltEleDetaFilter.clone()
hltEle25WP90RhoDetaFilter.thrRegularEE = cms.double( 0.006 )
hltEle25WP90RhoDetaFilter.thrRegularEB = cms.double( 0.006 )
hltEle25WP90RhoDetaFilter.ncandcut = cms.int32(1)
hltEle25WP90RhoDetaFilter.candTag = cms.InputTag("hltEle25WP90RhoOneOEMinusOneOPFilter")

## Dphi filter
hltEle25WP90RhoDphiFilter = hltEleDphiFilter.clone()
hltEle25WP90RhoDphiFilter.thrRegularEE = cms.double( 0.05 )
hltEle25WP90RhoDphiFilter.thrRegularEB = cms.double( 0.04 )
hltEle25WP90RhoDphiFilter.ncandcut = cms.int32(1)
hltEle25WP90RhoDphiFilter.candTag = cms.InputTag("hltEle25WP90RhoDetaFilter")

## track iso filter
hltEle25WP90TrackRhoIsoFilter = hltEleTrackIsoFilter.clone()
hltEle25WP90TrackRhoIsoFilter.thrOverPtEE = cms.double( 0.05 )
hltEle25WP90TrackRhoIsoFilter.thrOverPtEB = cms.double( 0.05 )
hltEle25WP90TrackRhoIsoFilter.ncandcut = cms.int32(1)
hltEle25WP90TrackRhoIsoFilter.candTag = cms.InputTag("hltEle25WP90RhoDphiFilter")

### Final Sequence
HLTEle25WP90Sequence = cms.Sequence( hltPreEle+
	                             HLTDoRegionalEgammaEcalSequence+
                                     HLTL1SeededEcalClustersSequence+
                                     hltL1SeededRecoEcalCandidate+ 
                                     hltEGRegionalL1SingleEG20ORL1SingleEG22+ 
                                     hltEG25WP90EtFilter +
                                     hltL1SeededHLTClusterShape +
                                     hltEle25WP90ClusterShapeFilter+
                                     HLTDoLocalHcalWithHOSequence+					     
                                     hltL1SeededPhotonEcalIsoRhoCorr+
                                     hltEle25WP90EcalRhoIsoFilter+
                                     hltL1SeededPhotonHcalForHE +
                                     hltEle25WP90RhoHEFilter+ 
                                     hltL1SeededPhotonHcalIsoRhoCorr +
                                     hltEle25WP90HcalRhoIsoFilter + 
                                     HLTDoLocalPixelSequence +
                                     HLTDoLocalStripSequence +
                                     hltL1SeededStartUpElectronPixelSeeds+ 
                                     hltEle25WP90RhoPixelMatchFilter+
                                     hltCkfL1SeededTrackCandidates+
                                     hltCtfL1SeededWithMaterialTracks+
                                     hltEleAnyWPBarrelTracks+
                                     hltCtfL1SeededWithMaterialCleanTracks+
                                     hltEleAnyCleanMergedTracks+
                                     hltEleAnyPixelMatchElectronsL1Seeded +
                                     hltEle25WP90RhoOneOEMinusOneOPFilter+
                                     hltEleAnyL1SeededDetaDphi+
                                     hltEle25WP90RhoDetaFilter+ 
                                     hltEle25WP90RhoDphiFilter+
                                     HLTL1SeededEgammaRegionalRecoTrackerSequence +
                                     hltEleAnyL1SeededElectronTrackIso +
                                     hltEle25WP90TrackRhoIsoFilter+
                                     HLTFastPrimaryVertexSequence)


#############################
### using gsf electrons  ####
#############################

from HLTrigger.Configuration.HLT_GsfElectronTrackSequence_cff import *

hltGsfEle25WP90RhoOneOEMinusOneOPFilter = hltEleOneOEMinusOneOPFilter.clone()
hltGsfEle25WP90RhoOneOEMinusOneOPFilter.barrelcut = cms.double( 999.9 )
hltGsfEle25WP90RhoOneOEMinusOneOPFilter.endcapcut = cms.double( 999.9 )
hltGsfEle25WP90RhoOneOEMinusOneOPFilter.ncandcut = cms.int32(1)
hltGsfEle25WP90RhoOneOEMinusOneOPFilter.candTag = cms.InputTag("hltEle25WP90RhoPixelMatchFilter")
hltGsfEle25WP90RhoOneOEMinusOneOPFilter.electronIsolatedProducer = cms.InputTag("hltL1SeededGsfElectrons")
hltGsfEle25WP90RhoOneOEMinusOneOPFilter.useGsfElectron = cms.bool(True)

## Deta filter
hltGsfEle25WP90RhoDetaFilter = hltEleDetaFilter.clone()
hltGsfEle25WP90RhoDetaFilter.thrRegularEE = cms.double( 0.007 )
hltGsfEle25WP90RhoDetaFilter.thrRegularEB = cms.double( 0.007 )
hltGsfEle25WP90RhoDetaFilter.ncandcut = cms.int32(1)
hltGsfEle25WP90RhoDetaFilter.candTag = cms.InputTag("hltGsfEle25WP90RhoOneOEMinusOneOPFilter")
hltGsfEle25WP90RhoDetaFilter.L1IsoCand = cms.InputTag("hltEle25WP90RhoPixelMatchFilter")
hltGsfEle25WP90RhoDetaFilter.isoTag = cms.InputTag( 'hltL1SeededGsfTrackVars','Deta' )

## Dphi filter
hltGsfEle25WP90RhoDphiFilter = hltEleDphiFilter.clone()
hltGsfEle25WP90RhoDphiFilter.thrRegularEE = cms.double( 0.03 )
hltGsfEle25WP90RhoDphiFilter.thrRegularEB = cms.double( 0.06 )
hltGsfEle25WP90RhoDphiFilter.ncandcut = cms.int32(1)
hltGsfEle25WP90RhoDphiFilter.candTag = cms.InputTag("hltGsfEle25WP90RhoDetaFilter")
hltGsfEle25WP90RhoDphiFilter.L1IsoCand = cms.InputTag("hltEle25WP90RhoPixelMatchFilter")
hltGsfEle25WP90RhoDphiFilter.isoTag = cms.InputTag( 'hltL1SeededGsfTrackVars','Dphi' )

## track iso filter
hltGsfEle25WP90TrackRhoIsoFilter = hltEleTrackIsoFilter.clone()
hltGsfEle25WP90TrackRhoIsoFilter.thrOverPtEE = cms.double( 0.05 )
hltGsfEle25WP90TrackRhoIsoFilter.thrOverPtEB = cms.double( 0.05 )
hltGsfEle25WP90TrackRhoIsoFilter.ncandcut = cms.int32(1)
hltGsfEle25WP90TrackRhoIsoFilter.candTag = cms.InputTag("hltGsfEle25WP90RhoDphiFilter")
hltGsfEle25WP90TrackRhoIsoFilter.L1IsoCand = cms.InputTag("hltEle25WP90RhoPixelMatchFilter")
hltGsfEle25WP90TrackRhoIsoFilter.isoTag = cms.InputTag("hltGsfEleAnyL1SeededElectronTrackIso")

HLTGsfEle25WP90Sequence = cms.Sequence(	hltPreEle+
	                                HLTDoRegionalEgammaEcalSequence+
                                        HLTL1SeededEcalClustersSequence+
                                        hltL1SeededRecoEcalCandidate+ 
                                        hltEGRegionalL1SingleEG20ORL1SingleEG22+ 
                                        hltEG25WP90EtFilter +
                                        hltL1SeededHLTClusterShape +
                                        hltEle25WP90ClusterShapeFilter+
                                        HLTDoLocalHcalWithHOSequence+					     
                                        hltL1SeededPhotonEcalIsoRhoCorr+
                                        hltEle25WP90EcalRhoIsoFilter+
                                        hltL1SeededPhotonHcalForHE +
                                        hltEle25WP90RhoHEFilter+ 
                                        hltL1SeededPhotonHcalIsoRhoCorr +
                                        hltEle25WP90HcalRhoIsoFilter + 
                                        HLTDoLocalPixelSequence +
                                        HLTDoLocalStripSequence +
                                        hltL1SeededStartUpElectronPixelSeeds+ 
				        hltEle25WP90RhoPixelMatchFilter+
                                        HLTL1SeededGsfElectronSequence+
                                        hltGsfEle25WP90RhoOneOEMinusOneOPFilter+
                                        hltGsfEle25WP90RhoDetaFilter+
                                        hltGsfEle25WP90RhoDphiFilter+
                                        HLTL1SeededEgammaRegionalRecoTrackerSequence +
                                        hltGsfEleAnyL1SeededElectronTrackIso+
                                        hltGsfEle25WP90TrackRhoIsoFilter+
                                        HLTFastPrimaryVertexSequence)
