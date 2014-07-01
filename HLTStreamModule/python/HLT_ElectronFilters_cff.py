import FWCore.ParameterSet.Config as cms

hltPreEle = cms.EDFilter( "HLTPrescaler",
    L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
    offset = cms.uint32( 0 ))


## filter on L1 Seed candidate after the clustering --> just filter the original collection in a new one
hltEGL1SingleEG20ORL1SingleEG22Filter = cms.EDFilter( "HLTEgammaL1MatchFilterRegional",
    saveTags = cms.bool( False ),
    endcap_end = cms.double( 2.65 ),
    region_eta_size_ecap = cms.double( 1.0 ),
    barrel_end = cms.double( 1.4791 ),
    l1IsolatedTag = cms.InputTag( 'hltL1extraParticles','Isolated' ),
    candIsolatedTag = cms.InputTag( "hltEgammaCandidates" ),
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
    L1IsoCand = cms.InputTag( "hltEgammaCandidates" ),
    inputTag = cms.InputTag( "hltEGL1SingleEG20ORL1SingleEG22Filter" ),
    etcutEB = cms.double(0.),
    ncandcut = cms.int32( 1 ),
    etcutEE = cms.double(0.))
    
## filter on the selected EGamma candidate with et > 27 GeV appling WP80 cluster shape cuts
hltEleClusterShapeFilter = cms.EDFilter( "HLTEgammaGenericFilter",
    doIsolated = cms.bool( True ),
    nonIsoTag = cms.InputTag( "" ),
    L1NonIsoCand = cms.InputTag( "" ),
    saveTags = cms.bool( False ),
    thrOverE2EB = cms.double( -1.0 ),
    thrRegularEE = cms.double(-1.0),
    thrOverEEE = cms.double( -1.0 ),
    L1IsoCand = cms.InputTag( "hltEgammaCandidates" ),
    thrOverEEB = cms.double( -1.0 ),
    thrRegularEB = cms.double(-1.0),
    lessThan = cms.bool( True ),
    useEt = cms.bool( False ),
    ncandcut = cms.int32( 1 ),
    isoTag = cms.InputTag( "hltEgammaClusterShape" ),
    candTag = cms.InputTag( "hltEGEtFilter" ),
    thrOverE2EE = cms.double( -1.0 ))

### filter for HoverE
hltEleHEFilter = cms.EDFilter( "HLTEgammaGenericFilter",
    doIsolated = cms.bool( True ),
    nonIsoTag = cms.InputTag( "" ),
    L1NonIsoCand = cms.InputTag( "" ),
    saveTags = cms.bool( False ),
    thrOverE2EB = cms.double( -1.0 ),
    thrRegularEE = cms.double( -1.0 ),
    thrOverEEE = cms.double(-1.0),
    L1IsoCand = cms.InputTag( "hltEgammaCandidates" ),
    thrOverEEB = cms.double(-1.0),
    thrRegularEB = cms.double( -1.0 ),
    lessThan = cms.bool( True ),
    useEt = cms.bool( False ),
    ncandcut = cms.int32( 1 ),
    isoTag = cms.InputTag( "hltEgammaHoverE" ),
    candTag = cms.InputTag( "hltEleClusterShapeFilter" ),
    thrOverE2EE = cms.double( -1.0 )
)

### Iso Filter electron collection from L1Seeded Candidate from clustering, SeededPhotonEcalIso, Cluster Shape     
hltEleEcalIsoFilter = cms.EDFilter( "HLTEgammaGenericFilter",
    doIsolated = cms.bool( True ),
    nonIsoTag = cms.InputTag( "" ),
    L1NonIsoCand = cms.InputTag( "" ),
    saveTags = cms.bool( False ),
    thrOverE2EB = cms.double( -1.0 ),
    thrRegularEE = cms.double( -1.0 ),
    thrOverEEE = cms.double(-1.0),
    L1IsoCand = cms.InputTag( "hltEgammaCandidates" ),
    thrOverEEB = cms.double(-1.0),
    thrRegularEB = cms.double( -1.0 ),
    lessThan = cms.bool( True ),
    useEt = cms.bool( True ),
    ncandcut = cms.int32( 1 ),
    isoTag = cms.InputTag( "hltEgammaEcalPFClusterIso" ),
    candTag = cms.InputTag( "hltEleHEFilter" ),
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
    L1IsoCand = cms.InputTag( "hltEgammaCandidates" ),
    thrOverEEB = cms.double( -1.0 ),
    thrRegularEB = cms.double( 999999.0 ),
    lessThan = cms.bool( True ),
    useEt = cms.bool( True ),
    ncandcut = cms.int32( 1 ),
    isoTag = cms.InputTag( "hltEgammaHcalPFClusterIso" ),
    candTag = cms.InputTag( "hltEleEcalIsoFilter" ),
    thrOverE2EE = cms.double( -1.0 )
)

### filter the tracks looking at the electron candidates
hltElePixelMatchFilter = cms.EDFilter( "HLTElectronPixelMatchFilter",
    s_a_rF = cms.double( 0.04 ),
    saveTags = cms.bool( False ),
    s_a_phi1B = cms.double( 0.0069 ),
    L1NonIsoPixelSeedsTag = cms.InputTag( "" ),
    s_a_phi1F = cms.double( 0.0076 ),
    s_a_phi1I = cms.double( 0.0088 ),
    L1IsoCand = cms.InputTag( "hltEgammaCandidates" ),
    s2_threshold = cms.double( 0.4 ),
    useS = cms.bool( False ),
    s_a_rI = cms.double( 0.027 ),
    npixelmatchcut = cms.double( 1.0 ),
    L1NonIsoCand = cms.InputTag( "" ),
    ncandcut = cms.int32( 1 ),
    doIsolated = cms.bool( True ),
    candTag = cms.InputTag( "hltEleHcalIsoFilter" ),
    s_a_phi2B = cms.double( 3.7E-4 ),
    s_a_zB = cms.double( 0.012 ),
    L1IsoPixelSeedsTag = cms.InputTag( "hltEgammaElectronPixelSeeds" ),
    s_a_phi2F = cms.double( 0.00906 ),
    s_a_phi2I = cms.double( 7.0E-4 )
)

### OEO minus OOP filter
hltEleOneOEMinusOneOPFilter = cms.EDFilter( "HLTEgammaGenericFilter",
    doIsolated = cms.bool( True ),
    thrOverE2EE = cms.double( -1.0 ),
    L1NonIsoCand = cms.InputTag( "" ),
    saveTags = cms.bool( False ),
    thrOverE2EB = cms.double( -1.0 ),
    thrRegularEE = cms.double( -1.0 ),
    thrOverEEE = cms.double( -1.0 ),
    L1IsoCand = cms.InputTag( "hltEgammaCandidates" ),
    thrOverEEB = cms.double( -1.0 ),
    thrRegularEB = cms.double( -1.0 ),
    lessThan = cms.bool( True ),
    useEt = cms.bool( True ),
    ncandcut = cms.int32( 1 ),
    isoTag = cms.InputTag( 'hltEgammaGsfTrackVars','OneOESuperMinusOneOP' ),
    candTag = cms.InputTag( "hltElePixelMatchFilter" ),
    nonIsoTag = cms.InputTag( "" )
)

### Deta Filter
hltEleDetaFilter = cms.EDFilter( "HLTEgammaGenericFilter",
    doIsolated = cms.bool( True ),
    thrOverE2EE = cms.double( -1.0 ),
    L1NonIsoCand = cms.InputTag( "" ),
    saveTags = cms.bool( False ),
    thrOverE2EB = cms.double( -1.0 ),
    thrRegularEE = cms.double( -1.0 ),
    thrOverEEE = cms.double( -1.0 ),
    L1IsoCand = cms.InputTag( "hltEgammaCandidates" ),
    thrOverEEB = cms.double( -1.0 ),
    thrRegularEB = cms.double( -1.0 ),
    lessThan = cms.bool( True ),
    useEt = cms.bool( True ),
    ncandcut = cms.int32( 1 ),
    isoTag = cms.InputTag( 'hltEgammaGsfTrackVars','Deta' ),
    candTag = cms.InputTag( "hltEleOneOEMinusOneOPFilter" ),
    nonIsoTag = cms.InputTag( "" )
)


### Dphi filter
hltEleDphiFilter = cms.EDFilter( "HLTEgammaGenericFilter",
    doIsolated = cms.bool( True ),
    thrOverE2EE = cms.double( -1.0 ),
    L1NonIsoCand = cms.InputTag( "" ),
    saveTags = cms.bool( False ),
    thrOverE2EB = cms.double( -1.0 ),
    thrRegularEE = cms.double( 0.03 ),
    thrOverEEE = cms.double( -1.0 ),
    L1IsoCand = cms.InputTag( "hltEgammaCandidates" ),
    thrOverEEB = cms.double( -1.0 ),
    thrRegularEB = cms.double( 0.06 ),
    lessThan = cms.bool( True ),
    useEt = cms.bool( True ),
    ncandcut = cms.int32( 1 ),
    isoTag = cms.InputTag( 'hltEgammaGsfTrackVars','Dphi' ),
    candTag = cms.InputTag( "hltEleDetaFilter" ),
    nonIsoTag = cms.InputTag( "" )
)


## tracker iso filter
hltEleTrackIsoFilter = cms.EDFilter( "HLTEgammaGenericFilter",
    doIsolated = cms.bool( True ),
    thrOverE2EE = cms.double( -1.0 ),
    L1NonIsoCand = cms.InputTag( "" ),
    saveTags = cms.bool( True ),
    thrOverE2EB = cms.double( -1.0 ),
    thrRegularEE = cms.double( -1.0 ),
    thrOverEEE = cms.double( -1.0 ),
    L1IsoCand = cms.InputTag( "hltEgammaCandidates" ),
    thrOverEEB = cms.double( -1.0 ),
    thrRegularEB = cms.double( -1.0 ),
    lessThan = cms.bool( True ),
    useEt = cms.bool( True ),
    ncandcut = cms.int32( 1 ),
    isoTag = cms.InputTag( "hltEgammaEleGsfTrackIso" ),
    candTag = cms.InputTag( "hltEle27DphiFilter" ),
    nonIsoTag = cms.InputTag( "" )
)
