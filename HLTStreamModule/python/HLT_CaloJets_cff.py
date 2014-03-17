import FWCore.ParameterSet.Config as cms

from HLTrigger.Configuration.HLTDoRegionalEgammaEcal_cff import *
 
hltHcalDigis = cms.EDProducer( "HcalRawToDigi",
    UnpackZDC = cms.untracked.bool( True ),
    FilterDataQuality = cms.bool( True ),
    InputLabel = cms.InputTag( "rawDataCollector" ),
    ComplainEmptyData = cms.untracked.bool( False ),
    UnpackCalib = cms.untracked.bool( True ),
    UnpackTTP = cms.untracked.bool( False ),
    lastSample = cms.int32( 9 ),
    firstSample = cms.int32( 0 )
)

hltHbhereco = cms.EDProducer( "HcalHitReconstructor",
    digiTimeFromDB = cms.bool( True ),
    S9S1stat = cms.PSet(  ),
    saturationParameters = cms.PSet(  maxADCvalue = cms.int32( 127 ) ),
    tsFromDB = cms.bool( True ),
    samplesToAdd = cms.int32( 4 ),
    correctionPhaseNS = cms.double( 13.0 ),
    HFInWindowStat = cms.PSet(  ),
    digiLabel = cms.InputTag( "hltHcalDigis" ),
    setHSCPFlags = cms.bool( False ),
    firstAuxTS = cms.int32( 4 ),
    setSaturationFlags = cms.bool( False ),
    hfTimingTrustParameters = cms.PSet(  ),
    PETstat = cms.PSet(  ),
    digistat = cms.PSet(  ),
    useLeakCorrection = cms.bool( False ),
    setTimingTrustFlags = cms.bool( False ),
    S8S1stat = cms.PSet(  ),
    correctForPhaseContainment = cms.bool( True ),
    correctForTimeslew = cms.bool( True ),
    setNoiseFlags = cms.bool( False ),
    correctTiming = cms.bool( False ),
    setPulseShapeFlags = cms.bool( False ),
    Subdetector = cms.string( "HBHE" ),
    dropZSmarkedPassed = cms.bool( True ),
    recoParamsFromDB = cms.bool( True ),
    firstSample = cms.int32( 4 ),
    setTimingShapedCutsFlags = cms.bool( False ),
    timingshapedcutsParameters = cms.PSet( 
      ignorelowest = cms.bool( True ),
      win_offset = cms.double( 0.0 ),
      ignorehighest = cms.bool( False ),
      win_gain = cms.double( 1.0 ),
      tfilterEnvelope = cms.vdouble( 4.0, 12.04, 13.0, 10.56, 23.5, 8.82, 37.0, 7.38, 56.0, 6.3, 81.0, 5.64, 114.5, 5.44, 175.5, 5.38, 350.5, 5.14 )
    ),
    pulseShapeParameters = cms.PSet(  ),
    flagParameters = cms.PSet( 
      nominalPedestal = cms.double( 3.0 ),
      hitMultiplicityThreshold = cms.int32( 17 ),
      hitEnergyMinimum = cms.double( 1.0 ),
      pulseShapeParameterSets = cms.VPSet( 
        cms.PSet(  pulseShapeParameters = cms.vdouble( 0.0, 100.0, -50.0, 0.0, -15.0, 0.15 )        ),
        cms.PSet(  pulseShapeParameters = cms.vdouble( 100.0, 2000.0, -50.0, 0.0, -5.0, 0.05 )        ),
        cms.PSet(  pulseShapeParameters = cms.vdouble( 2000.0, 1000000.0, -50.0, 0.0, 95.0, 0.0 )        ),
        cms.PSet(  pulseShapeParameters = cms.vdouble( -1000000.0, 1000000.0, 45.0, 0.1, 1000000.0, 0.0 )        )
      )
    ),
    hscpParameters = cms.PSet( 
      slopeMax = cms.double( -0.6 ),
      r1Max = cms.double( 1.0 ),
      r1Min = cms.double( 0.15 ),
      TimingEnergyThreshold = cms.double( 30.0 ),
      slopeMin = cms.double( -1.5 ),
      outerMin = cms.double( 0.0 ),
      outerMax = cms.double( 0.1 ),
      fracLeaderMin = cms.double( 0.4 ),
      r2Min = cms.double( 0.1 ),
      r2Max = cms.double( 0.5 ),
      fracLeaderMax = cms.double( 0.7 )
    )
)

hltHoreco = cms.EDProducer( "HcalHitReconstructor",
    digiTimeFromDB = cms.bool( True ),
    S9S1stat = cms.PSet(  ),
    saturationParameters = cms.PSet(  maxADCvalue = cms.int32( 127 ) ),
    tsFromDB = cms.bool( True ),
    samplesToAdd = cms.int32( 4 ),
    correctionPhaseNS = cms.double( 13.0 ),
    HFInWindowStat = cms.PSet(  ),
    digiLabel = cms.InputTag( "hltHcalDigis" ),
    setHSCPFlags = cms.bool( False ),
    firstAuxTS = cms.int32( 4 ),
    setSaturationFlags = cms.bool( False ),
    hfTimingTrustParameters = cms.PSet(  ),
    PETstat = cms.PSet(  ),
    digistat = cms.PSet(  ),
    useLeakCorrection = cms.bool( False ),
    setTimingTrustFlags = cms.bool( False ),
    S8S1stat = cms.PSet(  ),
    correctForPhaseContainment = cms.bool( True ),
    correctForTimeslew = cms.bool( True ),
    setNoiseFlags = cms.bool( False ),
    correctTiming = cms.bool( False ),
    setPulseShapeFlags = cms.bool( False ),
    Subdetector = cms.string( "HO" ),
    dropZSmarkedPassed = cms.bool( True ),
    recoParamsFromDB = cms.bool( True ),
    firstSample = cms.int32( 4 ),
    setTimingShapedCutsFlags = cms.bool( False ),
    timingshapedcutsParameters = cms.PSet(  ),
    pulseShapeParameters = cms.PSet(  ),
    flagParameters = cms.PSet(  ),
    hscpParameters = cms.PSet(  )
)


hltHfreco = cms.EDProducer( "HcalHitReconstructor",
  digiTimeFromDB = cms.bool( True ),
  S9S1stat = cms.PSet( 
   longETParams = cms.vdouble( 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 ),
   shortEnergyParams = cms.vdouble( 35.1773, 35.37, 35.7933, 36.4472, 37.3317, 38.4468, 39.7925, 41.3688, 43.1757, 45.2132, 47.4813, 49.98, 52.7093 ),
   flagsToSkip = cms.int32( 24 ),
   shortETParams = cms.vdouble( 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 ),
   short_optimumSlope = cms.vdouble( -99999.0, 0.0164905, 0.0238698, 0.0321383, 0.041296, 0.0513428, 0.0622789, 0.0741041, 0.0868186, 0.100422, 0.135313, 0.136289, 0.0589927 ),
   longEnergyParams = cms.vdouble( 43.5, 45.7, 48.32, 51.36, 54.82, 58.7, 63.0, 67.72, 72.86, 78.42, 84.4, 90.8, 97.62 ),
   long_optimumSlope = cms.vdouble( -99999.0, 0.0164905, 0.0238698, 0.0321383, 0.041296, 0.0513428, 0.0622789, 0.0741041, 0.0868186, 0.100422, 0.135313, 0.136289, 0.0589927 ),
   isS8S1 = cms.bool( False ),
   HcalAcceptSeverityLevel = cms.int32( 9 )
    ),
  saturationParameters = cms.PSet(  maxADCvalue = cms.int32( 127 ) ),
  tsFromDB = cms.bool( True ),
  samplesToAdd = cms.int32( 2 ),
  correctionPhaseNS = cms.double( 13.0 ),
  HFInWindowStat = cms.PSet( 
      hflongEthresh = cms.double( 40.0 ),
      hflongMinWindowTime = cms.vdouble( -10.0 ),
      hfshortEthresh = cms.double( 40.0 ),
      hflongMaxWindowTime = cms.vdouble( 10.0 ),
      hfshortMaxWindowTime = cms.vdouble( 10.0 ),
      hfshortMinWindowTime = cms.vdouble( -12.0 )
  ),
  digiLabel = cms.InputTag( "hltHcalDigis" ),
  setHSCPFlags = cms.bool( False ),
  firstAuxTS = cms.int32( 1 ),
  setSaturationFlags = cms.bool( False ),
  hfTimingTrustParameters = cms.PSet( 
  hfTimingTrustLevel2 = cms.int32( 4 ),
  hfTimingTrustLevel1 = cms.int32( 1 )),
  PETstat = cms.PSet( 
      longETParams = cms.vdouble( 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 ),
      short_R_29 = cms.vdouble( 0.8 ),
      shortEnergyParams = cms.vdouble( 35.1773, 35.37, 35.7933, 36.4472, 37.3317, 38.4468, 39.7925, 41.3688, 43.1757, 45.2132, 47.4813, 49.98, 52.7093 ),
      flagsToSkip = cms.int32( 0 ),
      short_R = cms.vdouble( 0.8 ),
      shortETParams = cms.vdouble( 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 ),
      long_R_29 = cms.vdouble( 0.8 ),
      longEnergyParams = cms.vdouble( 43.5, 45.7, 48.32, 51.36, 54.82, 58.7, 63.0, 67.72, 72.86, 78.42, 84.4, 90.8, 97.62 ),
      long_R = cms.vdouble( 0.98 ),
      HcalAcceptSeverityLevel = cms.int32( 9 )
    ),
  digistat = cms.PSet( 
      HFdigiflagFirstSample = cms.int32( 1 ),
      HFdigiflagMinEthreshold = cms.double( 40.0 ),
      HFdigiflagSamplesToAdd = cms.int32( 3 ),
      HFdigiflagExpectedPeak = cms.int32( 2 ),
      HFdigiflagCoef = cms.vdouble( 0.93, -0.012667, -0.38275 )
    ),
  useLeakCorrection = cms.bool( False ),
  setTimingTrustFlags = cms.bool( False ),
  S8S1stat = cms.PSet( 
      longETParams = cms.vdouble( 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 ),
      shortEnergyParams = cms.vdouble( 40.0, 100.0, 100.0, 100.0, 100.0, 100.0, 100.0, 100.0, 100.0, 100.0, 100.0, 100.0, 100.0 ),
      flagsToSkip = cms.int32( 16 ),
      shortETParams = cms.vdouble( 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 ),
      short_optimumSlope = cms.vdouble( 0.3, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1 ),
      longEnergyParams = cms.vdouble( 40.0, 100.0, 100.0, 100.0, 100.0, 100.0, 100.0, 100.0, 100.0, 100.0, 100.0, 100.0, 100.0 ),
      long_optimumSlope = cms.vdouble( 0.3, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1 ),
      isS8S1 = cms.bool( True ),
      HcalAcceptSeverityLevel = cms.int32( 9 )
    ),
  correctForPhaseContainment = cms.bool( False ),
  correctForTimeslew = cms.bool( False ),
  setNoiseFlags = cms.bool( True ),
  correctTiming = cms.bool( False ),
  setPulseShapeFlags = cms.bool( False ),
  Subdetector = cms.string( "HF" ),
  dropZSmarkedPassed = cms.bool( True ),
  recoParamsFromDB = cms.bool( True ),
  firstSample = cms.int32( 2 ),
  setTimingShapedCutsFlags = cms.bool( False ),
  timingshapedcutsParameters = cms.PSet(  ),
  pulseShapeParameters = cms.PSet(  ),
  flagParameters = cms.PSet(  ),
  hscpParameters = cms.PSet(  )
)

hltTowerMakerForAll = cms.EDProducer( "CaloTowersCreator",
 EBSumThreshold = cms.double( 0.2 ),
 MomHBDepth = cms.double( 0.2 ),
 UseEtEBTreshold = cms.bool( False ),
 hfInput = cms.InputTag( "hltHfreco" ),
 AllowMissingInputs = cms.bool( False ),
 MomEEDepth = cms.double( 0.0 ),
 EESumThreshold = cms.double( 0.45 ),
 HBGrid = cms.vdouble(  ),
 HcalAcceptSeverityLevelForRejectedHit = cms.uint32( 9999 ),
 HBThreshold = cms.double( 0.7 ),
 EcalSeveritiesToBeUsedInBadTowers = cms.vstring(  ),
 UseEcalRecoveredHits = cms.bool( False ),
 MomConstrMethod = cms.int32( 1 ),
 MomHEDepth = cms.double( 0.4 ),
 HcalThreshold = cms.double( -1000.0 ),
 HF2Weights = cms.vdouble(  ),
 HOWeights = cms.vdouble(  ),
 EEGrid = cms.vdouble(  ),
 UseSymEBTreshold = cms.bool( False ),
 EEWeights = cms.vdouble(  ),
 EEWeight = cms.double( 1.0 ),
 UseHO = cms.bool( False ),
 HBWeights = cms.vdouble(  ),
 HF1Weight = cms.double( 1.0 ),
 HF2Grid = cms.vdouble(  ),
 HEDWeights = cms.vdouble(  ),
 HEDGrid = cms.vdouble(  ),
 EBWeight = cms.double( 1.0 ),
 HF1Grid = cms.vdouble(  ),
 EBWeights = cms.vdouble(  ),
 HOWeight = cms.double( 1.0E-99 ),
 HESWeight = cms.double( 1.0 ),
 HESThreshold = cms.double( 0.8 ),
 hbheInput = cms.InputTag( "hltHbhereco" ),
 HF2Weight = cms.double( 1.0 ),
 HF2Threshold = cms.double( 0.85 ),
 HcalAcceptSeverityLevel = cms.uint32( 9 ),
 EEThreshold = cms.double( 0.3 ),
 HOThresholdPlus1 = cms.double( 3.5 ),
 HOThresholdPlus2 = cms.double( 3.5 ),
 HF1Weights = cms.vdouble(  ),
 hoInput = cms.InputTag( "hltHoreco" ),
 HF1Threshold = cms.double( 0.5 ),
 HOThresholdMinus1 = cms.double( 3.5 ),
 HESGrid = cms.vdouble(  ),
 EcutTower = cms.double( -1000.0 ),
 UseRejectedRecoveredEcalHits = cms.bool( False ),
 UseEtEETreshold = cms.bool( False ),
 HESWeights = cms.vdouble(  ),
 EcalRecHitSeveritiesToBeExcluded = cms.vstring('kTime','kWeird','kBad' ),
 HEDWeight = cms.double( 1.0 ),
 UseSymEETreshold = cms.bool( False ),
 HEDThreshold = cms.double( 0.8 ),
 EBThreshold = cms.double( 0.07 ),
 UseRejectedHitsOnly = cms.bool( False ),
 UseHcalRecoveredHits = cms.bool( False ),
 HOThresholdMinus2 = cms.double( 3.5 ),
 HOThreshold0 = cms.double( 3.5 ),
 ecalInputs = cms.VInputTag( 'hltEcalRegionalEgammaRecHit:EcalRecHitsEB','hltEcalRegionalEgammaRecHit:EcalRecHitsEE' ),
 UseRejectedRecoveredHcalHits = cms.bool( False ),
 MomEBDepth = cms.double( 0.3 ),
 HBWeight = cms.double( 1.0 ),
 HOGrid = cms.vdouble(  ),                                             
 EBGrid = cms.vdouble(  ))

		
hltKT6CaloJets = cms.EDProducer( "FastjetJetProducer",
    Active_Area_Repeats = cms.int32( 1 ),
    doAreaFastjet = cms.bool( False ),
    voronoiRfact = cms.double( 0.9 ),
    maxBadHcalCells = cms.uint32( 9999999 ),
    doAreaDiskApprox = cms.bool( True ),
    maxRecoveredEcalCells = cms.uint32( 9999999 ),
    jetType = cms.string( "CaloJet" ),
    minSeed = cms.uint32( 14327 ),
    Ghost_EtaMax = cms.double( 5.0 ),
    doRhoFastjet = cms.bool( True ),
    jetAlgorithm = cms.string( "Kt" ),
    nSigmaPU = cms.double( 1.0 ),
    GhostArea = cms.double( 0.01 ),
    Rho_EtaMax = cms.double( 4.4 ),
    maxBadEcalCells = cms.uint32( 9999999 ),
    useDeterministicSeed = cms.bool( True ),
    doPVCorrection = cms.bool( False ),
    maxRecoveredHcalCells = cms.uint32( 9999999 ),
    rParam = cms.double( 0.6 ),
    maxProblematicHcalCells = cms.uint32( 9999999 ),
    doOutputJets = cms.bool( True ),
    src = cms.InputTag( "hltTowerMakerForAll" ),
    inputEtMin = cms.double( 0.3 ),
    srcPVs = cms.InputTag( "NotUsed" ),
    jetPtMin = cms.double( 1.0 ),
    radiusPU = cms.double( 0.5 ),
    maxProblematicEcalCells = cms.uint32( 9999999 ),
    doPUOffsetCorr = cms.bool( False ),
    inputEMin = cms.double( 0.0 ),
    puPtMin = cms.double( 10.0 ),
    subtractorName = cms.string( "" ),
    MinVtxNdof = cms.int32( 0 ),
    MaxVtxZ = cms.double( 15.0 ),
    UseOnlyVertexTracks = cms.bool( False ),
    UseOnlyOnePV = cms.bool( False ),
    DzTrVtxMax = cms.double( 0.0 ),
    sumRecHits = cms.bool( False ),
    DxyTrVtxMax = cms.double( 0.0 )
)


hltAntiKT5CaloJets = cms.EDProducer( "FastjetJetProducer",
    Active_Area_Repeats = cms.int32( 5 ),
    doAreaFastjet = cms.bool( False ),
    maxBadHcalCells = cms.uint32( 9999999 ),
    doAreaDiskApprox = cms.bool( True ),
    maxRecoveredEcalCells = cms.uint32( 9999999 ),
    jetType = cms.string( "CaloJet" ),
    minSeed = cms.uint32( 14327 ),
    Ghost_EtaMax = cms.double( 6.0 ),
    doRhoFastjet = cms.bool( False ),
    jetAlgorithm = cms.string( "AntiKt" ),
    nSigmaPU = cms.double( 1.0 ),
    GhostArea = cms.double( 0.01 ),
    Rho_EtaMax = cms.double( 4.4 ),
    maxBadEcalCells = cms.uint32( 9999999 ),
    useDeterministicSeed = cms.bool( True ),
    doPVCorrection = cms.bool( False ),
    maxRecoveredHcalCells = cms.uint32( 9999999 ),
    rParam = cms.double( 0.5 ),
    maxProblematicHcalCells = cms.uint32( 9999999 ),
    doOutputJets = cms.bool( True ),
    src = cms.InputTag( "hltTowerMakerForAll" ),
    inputEtMin = cms.double( 0.3 ),
    srcPVs = cms.InputTag( "NotUsed" ),
    jetPtMin = cms.double( 1.0 ),
    radiusPU = cms.double( 0.5 ),
    maxProblematicEcalCells = cms.uint32( 9999999 ),
    doPUOffsetCorr = cms.bool( False ),
    inputEMin = cms.double( 0.0 ),
    puPtMin = cms.double( 10.0 ),
    subtractorName = cms.string( "" ),
    MinVtxNdof = cms.int32( 5 ),
    MaxVtxZ = cms.double( 15.0 ),
    UseOnlyVertexTracks = cms.bool( False ),
    UseOnlyOnePV = cms.bool( False ),
    DzTrVtxMax = cms.double( 0.0 ),
    sumRecHits = cms.bool( False ),
    DxyTrVtxMax = cms.double( 0.0 ))

    
## sequence without HO		
HLTDoLocalHcalWithHOSequence = cms.Sequence( hltHcalDigis + hltHbhereco + hltHfreco + hltHoreco + hltTowerMakerForAll+ hltKT6CaloJets + hltAntiKT5CaloJets)

from HLTrigger.Configuration.HLTDoRegionalEgammaEcal_cff import *		
		
### anti-kt calo jets for PF

hltTowerMakerForPF = hltTowerMakerForAll.clone()
hltTowerMakerForPF.HBThreshold  = cms.double( 0.4 )
hltTowerMakerForPF.HOWeight     = cms.double( 1.0 )
hltTowerMakerForPF.HESThreshold = cms.double( 0.4 )
hltTowerMakerForPF.HF2Threshold = cms.double( 1.8 )
hltTowerMakerForPF.HcalAcceptSeverityLevel = cms.uint32( 11 )
hltTowerMakerForPF.HOThresholdPlus1 = cms.double( 1.1 )
hltTowerMakerForPF.HOThresholdPlus2 = cms.double( 1.1 )
hltTowerMakerForPF.HF1Threshold = cms.double( 1.2 )
hltTowerMakerForPF.HOThresholdMinus1 = cms.double( 1.1 )
hltTowerMakerForPF.HEDThreshold = cms.double( 0.4 )
hltTowerMakerForPF.UseHcalRecoveredHits = cms.bool( True )
hltTowerMakerForPF.HOThresholdMinus2 = cms.double( 1.1 )
hltTowerMakerForPF.HOThreshold0 = cms.double( 1.1 )
hltTowerMakerForPF.ecalInputs   = cms.VInputTag( 'hltEcalRecHitAll:EcalRecHitsEB','hltEcalRecHitAll:EcalRecHitsEE' )


HLTDoCaloSequencePF = cms.Sequence( hltEcalRawToRecHitFacility + 
	                            hltEcalRegionalRestFEDs+
	                            hltEcalRecHitAll +
	                            HLTDoLocalHcalWithHOSequence + 
				    hltTowerMakerForPF )


hltAntiKT5CaloJetsPF = cms.EDProducer( "FastjetJetProducer",
    Active_Area_Repeats = cms.int32( 5 ),
    doAreaFastjet = cms.bool( False ),
    voronoiRfact = cms.double( -9.0 ),
    maxBadHcalCells = cms.uint32( 9999999 ),
    doAreaDiskApprox = cms.bool( False ),
    maxRecoveredEcalCells = cms.uint32( 9999999 ),
    jetType = cms.string( "CaloJet" ),
    minSeed = cms.uint32( 0 ),
    Ghost_EtaMax = cms.double( 6.0 ),
    doRhoFastjet = cms.bool( False ),
    jetAlgorithm = cms.string( "AntiKt" ),
    nSigmaPU = cms.double( 1.0 ),
    GhostArea = cms.double( 0.01 ),
    Rho_EtaMax = cms.double( 4.4 ),
    maxBadEcalCells = cms.uint32( 9999999 ),
    useDeterministicSeed = cms.bool( True ),
    doPVCorrection = cms.bool( False ),
    maxRecoveredHcalCells = cms.uint32( 9999999 ),
    rParam = cms.double( 0.5 ),
    maxProblematicHcalCells = cms.uint32( 9999999 ),
    doOutputJets = cms.bool( True ),
    src = cms.InputTag( "hltTowerMakerForPF" ),
    inputEtMin = cms.double( 0.3 ),
    srcPVs = cms.InputTag( "NotUsed" ),
    jetPtMin = cms.double( 1.0 ),
    radiusPU = cms.double( 0.5 ),
    maxProblematicEcalCells = cms.uint32( 9999999 ),
    doPUOffsetCorr = cms.bool( False ),
    inputEMin = cms.double( 0.0 ),
    puPtMin = cms.double( 10.0 ),
    subtractorName = cms.string( "" ),
    MinVtxNdof = cms.int32( 5 ),
    MaxVtxZ = cms.double( 15.0 ),
    UseOnlyVertexTracks = cms.bool( False ),
    UseOnlyOnePV = cms.bool( False ),
    DzTrVtxMax = cms.double( 0.0 ),
    sumRecHits = cms.bool( False ),
    DxyTrVtxMax = cms.double( 0.0 )
)


HLTRecoJetSequenceAK5UncorrectedPF = cms.Sequence( HLTDoCaloSequencePF + hltAntiKT5CaloJetsPF )

hltAntiKT5CaloJetsPFEt5 = cms.EDFilter( "EtMinCaloJetSelector",
    filter = cms.bool( False ),
    src = cms.InputTag( "hltAntiKT5CaloJetsPF" ),
    etMin = cms.double( 5.0 ))

HLTRecoJetSequencePrePF = cms.Sequence( HLTRecoJetSequenceAK5UncorrectedPF + hltAntiKT5CaloJetsPFEt5 )
