import FWCore.ParameterSet.Config as cms

hltPreEle = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    offset = cms.uint32( 0 ))


## filter on L1 Seed candidate after the clustering --> just filter the original collection in a new one
hltEGRegionalL1SingleEG20ORL1SingleEG22 = cms.EDFilter( "HLTEgammaL1MatchFilterRegional",
    saveTags = cms.bool( False ),
    endcap_end = cms.double( 2.65 ),
    region_eta_size_ecap = cms.double( 1.0 ),
    barrel_end = cms.double( 1.4791 ),
    l1IsolatedTag = cms.InputTag( 'hltL1extraParticles','Isolated' ),
    candIsolatedTag = cms.InputTag( "hltL1SeededRecoEcalCandidate" ),
    region_phi_size = cms.double( 1.044 ),
    region_eta_size = cms.double( 0.522 ),
    L1SeedFilterTag = cms.InputTag( "hltL1sL1SingleEG20ORL1SingleEG22" ),
    ncandcut = cms.int32( 1 ),
    doIsolated = cms.bool( False ),
    candNonIsolatedTag = cms.InputTag( "" ),
    l1NonIsolatedTag = cms.InputTag( 'hltL1extraParticles','NonIsolated' ))

##tEGRegionalL1SingleEG20ORL1SingleEG22, et cut at 25 GeV --> new fitered collection in the output
hltEGEtFilter = cms.EDFilter( "HLTEgammaEtFilter",
    saveTags = cms.bool( False ),
    L1NonIsoCand = cms.InputTag( "" ),
    relaxed = cms.untracked.bool( False ),
    L1IsoCand = cms.InputTag( "hltL1SeededRecoEcalCandidate" ),
    inputTag = cms.InputTag( "hltEGRegionalL1SingleEG20ORL1SingleEG22" ),
    etcutEB = cms.double(0.),
    ncandcut = cms.int32( 1 ),
    etcutEE = cms.double(0.))
    
## filter on the selected EGamma candidate with et > 25 GeV appling WP80 cluster shape cuts
hltEleClusterShapeFilter = cms.EDFilter( "HLTEgammaGenericFilter",
    doIsolated = cms.bool( True ),
    nonIsoTag = cms.InputTag( "" ),
    L1NonIsoCand = cms.InputTag( "" ),
    saveTags = cms.bool( False ),
    thrOverE2EB = cms.double( -1.0 ),
    thrRegularEE = cms.double(-1.0),
    thrOverEEE = cms.double( -1.0 ),
    L1IsoCand = cms.InputTag( "hltL1SeededRecoEcalCandidate" ),
    thrOverEEB = cms.double( -1.0 ),
    thrRegularEB = cms.double(-1.0),
    lessThan = cms.bool( True ),
    useEt = cms.bool( False ),
    ncandcut = cms.int32( 1 ),
    isoTag = cms.InputTag( "hltL1SeededHLTClusterShape" ),
    candTag = cms.InputTag( "hltEGEtFilter" ),
    thrOverE2EE = cms.double( -1.0 ))

### Iso Filter electron collection from L1Seeded Candidate from clustering, SeededPhotonEcalIso, Cluster Shape     
hltEleEcalIsoFilter = cms.EDFilter( "HLTEgammaGenericFilter",
    doIsolated = cms.bool( True ),
    nonIsoTag = cms.InputTag( "" ),
    L1NonIsoCand = cms.InputTag( "" ),
    saveTags = cms.bool( False ),
    thrOverE2EB = cms.double( -1.0 ),
    thrRegularEE = cms.double( -1.0 ),
    thrOverEEE = cms.double(-1.0),
    L1IsoCand = cms.InputTag( "hltL1SeededRecoEcalCandidate" ),
    thrOverEEB = cms.double(-1.0),
    thrRegularEB = cms.double( -1.0 ),
    lessThan = cms.bool( True ),
    useEt = cms.bool( True ),
    ncandcut = cms.int32( 1 ),
    isoTag = cms.InputTag( "hltL1SeededPhotonEcalIso" ),
    candTag = cms.InputTag( "hltEleClusterShapeFilter" ),
    thrOverE2EE = cms.double( -1.0 )
)

### filter for HoverE
hltEleHEFilter = cms.EDFilter( "HLTEgammaGenericFilter",
    doIsolated = cms.bool( True ),
    nonIsoTag = cms.InputTag( "" ),
    L1NonIsoCand = cms.InputTag( "" ),
    saveTags = cms.bool( False ),
    thrOverE2EB = cms.double( -1.0 ),
    thrRegularEE = cms.double( -1.0 ),
    thrOverEEE = cms.double(-1.0),
    L1IsoCand = cms.InputTag( "hltL1SeededRecoEcalCandidate" ),
    thrOverEEB = cms.double(-1.0),
    thrRegularEB = cms.double( -1.0 ),
    lessThan = cms.bool( True ),
    useEt = cms.bool( False ),
    ncandcut = cms.int32( 1 ),
    isoTag = cms.InputTag( "hltL1SeededPhotonHcalForHE" ),
    candTag = cms.InputTag( "hltEleEcalIsoFilter" ),
    thrOverE2EE = cms.double( -1.0 )
)

### filter on HCal isolation for electron ID
hltEleHcalIsoFilter = cms.EDFilter( "HLTEgammaGenericFilter",
    doIsolated = cms.bool( True ),
    nonIsoTag = cms.InputTag( "" ),
    L1NonIsoCand = cms.InputTag( "" ),
    saveTags = cms.bool( False ),
    thrOverE2EB = cms.double( -1.0 ),
    thrRegularEE = cms.double( -1.0 ),
    thrOverEEE = cms.double( -1.0 ),
    L1IsoCand = cms.InputTag( "hltL1SeededRecoEcalCandidate" ),
    thrOverEEB = cms.double( -1.0 ),
    thrRegularEB = cms.double( 999999.0 ),
    lessThan = cms.bool( True ),
    useEt = cms.bool( True ),
    ncandcut = cms.int32( 1 ),
    isoTag = cms.InputTag( "hltL1SeededPhotonHcalIso" ),
    candTag = cms.InputTag( "hltEleHEFilter" ),
    thrOverE2EE = cms.double( -1.0 )
)

### filter the tracks looking at the electron candidates
hltElePixelMatchFilter = cms.EDFilter( "HLTElectronPixelMatchFilter",
    saveTags = cms.bool( False ),
    doIsolated = cms.bool( True ),
    L1NonIsoCand = cms.InputTag( "" ),
    L1NonIsoPixelSeedsTag = cms.InputTag( "" ),
    L1IsoCand = cms.InputTag( "hltL1SeededRecoEcalCandidate" ),
    npixelmatchcut = cms.double(0.),
    ncandcut = cms.int32( 1 ),
    candTag = cms.InputTag( "hltEleHcalIsoFilter" ),
    L1IsoPixelSeedsTag = cms.InputTag( "hltL1SeededStartUpElectronPixelSeeds" )
)

### OEO minus OOP filter
hltEleOneOEMinusOneOPFilter = cms.EDFilter( "HLTElectronOneOEMinusOneOPFilterRegional",
    saveTags = cms.bool( False ),
    doIsolated = cms.bool( True ),
    electronNonIsolatedProducer = cms.InputTag( "" ),
    barrelcut = cms.double(-1.0),
    electronIsolatedProducer = cms.InputTag( "hltEleAnyPixelMatchElectronsL1Seeded" ),
    ncandcut = cms.int32( 1 ),
    candTag = cms.InputTag( "hltElePixelMatchFilter" ),
    endcapcut = cms.double(-1.0)
)

### Deta Filter
hltEleDetaFilter = cms.EDFilter( "HLTElectronGenericFilter",
    doIsolated = cms.bool( True ),
    nonIsoTag = cms.InputTag( "" ),
    L1NonIsoCand = cms.InputTag( "" ),
    thrTimesPtEB = cms.double( -1.0 ),
    saveTags = cms.bool( False ),
    thrRegularEE = cms.double( -1.0 ),
    L1IsoCand = cms.InputTag( "hltEleAnyPixelMatchElectronsL1Seeded" ),
    thrRegularEB = cms.double( -1.0 ),
    lessThan = cms.bool( True ),
    ncandcut = cms.int32( 1 ),
    isoTag = cms.InputTag( 'hltEleAnyL1SeededDetaDphi','Deta' ),
    candTag = cms.InputTag( "hltEleOneOEMinusOneOPFilter" ),
    thrTimesPtEE = cms.double( -1.0 ),
    thrOverPtEE = cms.double( -1.0 ),
    thrOverPtEB = cms.double( -1.0 )
)

### Dphi filter
hltEleDphiFilter = cms.EDFilter( "HLTElectronGenericFilter",
    doIsolated = cms.bool( True ),
    nonIsoTag = cms.InputTag( "" ),
    L1NonIsoCand = cms.InputTag( "" ),
    thrTimesPtEB = cms.double( -1.0 ),
    saveTags = cms.bool( False ),
    thrRegularEE = cms.double( -1.0 ),
    L1IsoCand = cms.InputTag( "hltEleAnyPixelMatchElectronsL1Seeded" ),
    thrRegularEB = cms.double( -1.0 ),
    lessThan = cms.bool( True ),
    ncandcut = cms.int32( 1 ),
    isoTag = cms.InputTag( 'hltEleAnyL1SeededDetaDphi','Dphi' ),
    candTag = cms.InputTag( "hltEleDetaFilter" ),
    thrTimesPtEE = cms.double( -1.0 ),
    thrOverPtEE = cms.double( -1.0 ),
    thrOverPtEB = cms.double( -1.0 )
)

## tracker iso filter
hltEleTrackIsoFilter = cms.EDFilter( "HLTElectronGenericFilter",
    doIsolated = cms.bool( True ),
    nonIsoTag = cms.InputTag( "" ),
    L1NonIsoCand = cms.InputTag( "" ),
    thrTimesPtEB = cms.double( -1.0 ),
    saveTags = cms.bool( True ),
    thrRegularEE = cms.double( -1.0 ),
    L1IsoCand = cms.InputTag( "hltEleAnyPixelMatchElectronsL1Seeded" ),
    thrRegularEB = cms.double( -1.0 ),
    lessThan = cms.bool( True ),
    ncandcut = cms.int32( 1 ),
    isoTag = cms.InputTag( "hltEleAnyL1SeededElectronTrackIso" ),
    candTag = cms.InputTag( "hltEleDphiFilter" ),
    thrTimesPtEE = cms.double( -1.0 ),
    thrOverPtEE = cms.double(-1.0),
    thrOverPtEB = cms.double(-1.0)
)

### start with double electron filter
hltEGRegionalL1DoubleEG178 = cms.EDFilter( "HLTEgammaL1MatchFilterRegional",
    saveTags = cms.bool( False ),
    endcap_end = cms.double( 2.65 ),
    region_eta_size_ecap = cms.double( 1.0 ),
    barrel_end = cms.double( 1.4791 ),
    l1IsolatedTag = cms.InputTag( 'hltL1extraParticles','Isolated' ),
    candIsolatedTag = cms.InputTag( "hltL1SeededRecoEcalCandidate" ),
    region_phi_size = cms.double( 1.044 ),
    region_eta_size = cms.double( 0.522 ),
    L1SeedFilterTag = cms.InputTag( "hltL1sL1DoubleEG137" ),
    ncandcut = cms.int32( 1 ),
    doIsolated = cms.bool( False ),
    candNonIsolatedTag = cms.InputTag( "" ),
    l1NonIsolatedTag = cms.InputTag( 'hltL1extraParticles','NonIsolated' )
)

hltEGFilterDoubleEG = cms.EDFilter( "HLTEgammaEtFilter",
    saveTags = cms.bool( False ),
    L1NonIsoCand = cms.InputTag(""),
    relaxed = cms.untracked.bool( False ),
    L1IsoCand = cms.InputTag( "hltL1SeededRecoEcalCandidate" ),
    inputTag = cms.InputTag( "" ),
    etcutEB = cms.double( 0. ),
    ncandcut = cms.int32( 1 ),
    etcutEE = cms.double( 0. )
)

hltEle17Ele8TrackIsoDZ = cms.EDFilter( "HLT2ElectronElectronDZ",
    saveTags = cms.bool( True ),
    originTag1 = cms.InputTag( "hltEG178WP90RhoPixelMatchFilterv2" ),
    originTag2 = cms.InputTag( "hltEG178WP90RhoPixelMatchFilterv2" ),
    MinN = cms.int32( 1 ),
    triggerType1 = cms.int32( 82 ),
    triggerType2 = cms.int32( 82 ),
    MinDR = cms.double( -1.0 ),
    MaxDZ = cms.double( 0.2 ),
    inputTag1 = cms.InputTag( "hltEG178WP90TrackRhoIsoFilterv2" ),
    checkSC = cms.bool( True ),
    inputTag2 = cms.InputTag( "hltEG178WP90TrackRhoIsoFilterv2" )
)
