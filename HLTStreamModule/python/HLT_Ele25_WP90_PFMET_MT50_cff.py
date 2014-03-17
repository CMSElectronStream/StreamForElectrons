import FWCore.ParameterSet.Config as cms

from HLTrigger.Configuration.HLT_ElectronFilters_cff import *
from HLTrigger.Configuration.HLT_PFFastL1L3_Sequence_cff import *
from HLTrigger.Configuration.HLT_FastVertexing_cff import *
from HLTrigger.Configuration.HLT_Ele25_WP90_cff import *

hltEle25WP90PFMT50PFMTFilter = cms.EDFilter( "HLTElectronPFMTFilter",
    saveTags = cms.bool( True ),
    L1NonIsoCand = cms.InputTag( "" ),
    minN = cms.int32( 1 ),
    relaxed = cms.bool( False ),
    L1IsoCand = cms.InputTag( "hltL1SeededRecoEcalCandidate" ),
    inputMetTag = cms.InputTag( "hltPFMETProducer" ),
    lowerMTCut = cms.double( 50.0 ),
    minMht = cms.double( 0.0 ),
    inputEleTag = cms.InputTag( "hltEle25WP90TrackRhoIsoFilter" ),
    upperMTCut = cms.double( 9999.0 ))
    
hltGsfEle25WP90PFMT50PFMTFilter = hltEle25WP90PFMT50PFMTFilter.clone()
hltGsfEle25WP90PFMT50PFMTFilter.inputEleTag = cms.InputTag( "hltGsfEle25WP90TrackRhoIsoFilter" )

HLTEle25WP90Sequence.remove("HLTFastPrimaryVertexSequence")
HLTGsfEle25WP90Sequence.remove("HLTFastPrimaryVertexSequence")

	
HLTEle25WP90PFMETMT50Sequence = cms.Sequence(HLTPFL1FastL2L3ReconstructionSequence + 
	                                     hltPFMETProducerSequence +
	                                     hltEle25WP90PFMT50PFMTFilter+
                                             HLTFastPrimaryVertexSequence)

HLTGsfEle25WP90PFMETMT50Sequence = cms.Sequence(HLTPFL1FastL2L3ReconstructionSequence + 
	                                        hltPFMETProducerSequence +
	                                        hltGsfEle25WP90PFMT50PFMTFilter+
                                                HLTFastPrimaryVertexSequence)

