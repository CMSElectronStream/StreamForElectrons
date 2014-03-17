import FWCore.ParameterSet.Config as cms

from HLTrigger.Configuration.HLT_ElectronFilters_cff import *
from HLTrigger.Configuration.HLT_PFFastL1L3_Sequence_cff import *

hltEle25WP70PFMT50PFMTFilter = cms.EDFilter( "HLTElectronPFMTFilter",
    saveTags = cms.bool( True ),
    L1NonIsoCand = cms.InputTag( "" ),
    minN = cms.int32( 1 ),
    relaxed = cms.bool( False ),
    L1IsoCand = cms.InputTag( "hltL1SeededRecoEcalCandidate" ),
    inputMetTag = cms.InputTag( "hltPFMETProducer" ),
    lowerMTCut = cms.double( 50.0 ),
    minMht = cms.double( 0.0 ),
    inputEleTag = cms.InputTag( "hltEle25WP70TrackIsoFilter" ),
    upperMTCut = cms.double( 9999.0 ))

hltGsfEle25WP70PFMT50PFMTFilter = hltEle25WP70PFMT50PFMTFilter.clone()
hltGsfEle25WP70PFMT50PFMTFilter.inputEleTag = cms.InputTag("hltGsfEle25WP70TrackIsoFilter")

HLTEle25WP70PFMETMT50Sequence = cms.Sequence(HLTPFL1FastL2L3ReconstructionSequence + 
	                                     hltPFMETProducerSequence +
	                                     hltEle25WP70PFMT50PFMTFilter)

HLTGsfEle25WP70PFMETMT50Sequence = cms.Sequence(HLTPFL1FastL2L3ReconstructionSequence + 
	                                        hltPFMETProducerSequence +
	                                        hltGsfEle25WP70PFMT50PFMTFilter)

