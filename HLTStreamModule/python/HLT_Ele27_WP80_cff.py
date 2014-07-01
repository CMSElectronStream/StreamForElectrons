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
from HLTrigger.Configuration.HLT_GsfElectronTrackSequence_cff import *

from HLTrigger.Configuration.HLT_ElectronFilters_cff import *

from HLTrigger.Configuration.HLT_FastVertexing_cff import *

## fix threshold for et cut
hltEG27WP80EtFilter = hltEGEtFilter.clone()
hltEG27WP80EtFilter.etcutEB = cms.double(27)
hltEG27WP80EtFilter.etcutEE = cms.double(27)
hltEG27WP80EtFilter.ncandcut = cms.int32(1)

## cluster shape filter
hltEle27WP80ClusterShapeFilter = hltEleClusterShapeFilter.clone()
hltEle27WP80ClusterShapeFilter.thrRegularEE = cms.double( 0.03 )
hltEle27WP80ClusterShapeFilter.thrRegularEB = cms.double( 0.01 )
hltEle27WP80ClusterShapeFilter.ncandcut = cms.int32(1)
hltEle27WP80ClusterShapeFilter.candTag = cms.InputTag("hltEG27WP80EtFilter")

### filter for HoverE
hltEle27WP80HEFilter = hltEleHEFilter.clone()
hltEle27WP80HEFilter.thrOverEEE = cms.double( 0.05 )
hltEle27WP80HEFilter.thrOverEEB = cms.double( 0.1 )
hltEle27WP80HEFilter.ncandcut = cms.int32(1)
hltEle27WP80HEFilter.candTag = cms.InputTag("hltEle27WP80ClusterShapeFilter")

## ecal iso filter
hltEle27WP80EcalIsoFilter = hltEleEcalIsoFilter.clone()
hltEle27WP80EcalIsoFilter.thrOverEEE = cms.double( 0.15 )
hltEle27WP80EcalIsoFilter.thrOverEEB = cms.double( 0.2 )
hltEle27WP80EcalIsoFilter.ncandcut = cms.int32(1)
hltEle27WP80EcalIsoFilter.candTag = cms.InputTag("hltEle27WP80HEFilter")


### filter on HCal isolation for electron ID
hltEle27WP80HcalIsoFilter = hltEleHcalIsoFilter.clone()
hltEle27WP80HcalIsoFilter.thrOverEEE = cms.double( 0.11 )
hltEle27WP80HcalIsoFilter.thrOverEEB = cms.double( 0.11 )
hltEle27WP80HcalIsoFilter.ncandcut = cms.int32(1)
hltEle27WP80HcalIsoFilter.candTag = cms.InputTag("hltEle27WP80EcalIsoFilter")

### filter the tracks looking at the electron candidates
hltEle27WP80PixelMatchFilter = hltElePixelMatchFilter.clone()
hltEle27WP80PixelMatchFilter.npixelmatchcut = cms.double(1.0)
hltEle27WP80PixelMatchFilter.ncandcut = cms.int32(1)
hltEle27WP80PixelMatchFilter.candTag = cms.InputTag("hltEle27WP80HcalIsoFilter")

###
hltEle27WP80OneOEMinusOneOPFilter = hltEleOneOEMinusOneOPFilter.clone()
hltEle27WP80OneOEMinusOneOPFilter.thrRegularEB = cms.double( 0.05 )
hltEle27WP80OneOEMinusOneOPFilter.thrRegularEE = cms.double( 0.05 )
hltEle27WP80OneOEMinusOneOPFilter.ncandcut = cms.int32(1)
hltEle27WP80OneOEMinusOneOPFilter.candTag = cms.InputTag("hltEle27WP80PixelMatchFilter")

## Deta filter
hltEle27WP80DetaFilter = hltEleDetaFilter.clone()
hltEle27WP80DetaFilter.thrRegularEE = cms.double( 0.007 )
hltEle27WP80DetaFilter.thrRegularEB = cms.double( 0.007 )
hltEle27WP80DetaFilter.ncandcut = cms.int32(1)
hltEle27WP80DetaFilter.candTag = cms.InputTag("hltEle27WP80OneOEMinusOneOPFilter")

## Dphi filter
hltEle27WP80DphiFilter = hltEleDphiFilter.clone()
hltEle27WP80DphiFilter.thrRegularEE = cms.double( 0.03 )
hltEle27WP80DphiFilter.thrRegularEB = cms.double( 0.06 )
hltEle27WP80DphiFilter.ncandcut = cms.int32(1)
hltEle27WP80DphiFilter.candTag = cms.InputTag("hltEle27WP80DetaFilter")

## track iso filter
hltEle27WP80TrackIsoFilter = hltEleTrackIsoFilter.clone()
hltEle27WP80TrackIsoFilter.thrOverEEB = cms.double( 0.05 )
hltEle27WP80TrackIsoFilter.thrOverEEE = cms.double( 0.05 )
hltEle27WP80TrackIsoFilter.ncandcut = cms.int32(1)
hltEle27WP80TrackIsoFilter.candTag = cms.InputTag("hltEle27WP80DphiFilter")

### Final Sequence
HLTEle27WP80Sequence = cms.Sequence( hltPreEle+
	                             HLTDoRegionalEgammaEcalSequence+
                                     HLTPFClusteringForEgamma+
                                     hltEgammaCandidates+
                                     hltEGL1SingleEG20ORL1SingleEG22Filter+ 
                                     hltEG27WP80EtFilter +
                                     hltEgammaClusterShape+
                                     hltEle27WP80ClusterShapeFilter+
                                     HLTDoLocalHcalWithTowerSequence+
                                     HLTFastJetForEgamma+
                                     hltEgammaHoverE+
                                     hltEle27WP80HEFilter+
                                     hltEgammaEcalPFClusterIso+
                                     hltEle27WP80EcalIsoFilter+
                                     HLTPFHcalClusteringForEgamma+
                                     hltEgammaHcalPFClusterIso+
                                     hltEle27WP80HcalIsoFilter+
                                     HLTDoLocalPixelSequence +
                                     HLTDoLocalStripSequence +
                                     hltMixedLayerPairs+
                                     hltEgammaElectronPixelSeeds+
                                     hltEle27WP80PixelMatchFilter+
                                     HLTGsfElectronSequence+
                                     hltEle27WP80OneOEMinusOneOPFilter+
                                     hltEle27WP80DetaFilter+
                                     hltEle27WP80DphiFilter+
                                     HLTPixelTrackingForElectron+
                                     HLTIterativeTrackingForElectronIter02+
                                     hltEgammaEleGsfTrackIso+
                                     hltEle27WP80TrackIsoFilter)
