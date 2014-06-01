import FWCore.ParameterSet.Config as cms

Analyzer = cms.EDAnalyzer('AnalyzerEle',
               EleTag              = cms.InputTag("patElectrons"),
               PVTag               = cms.InputTag("hltFastPVPixelVertices"),
               PVTag_alternative   = cms.InputTag("offlinePrimaryVerticesWithBS"),           
               recHitCollection_EB = cms.InputTag("reducedEcalRecHitsEB"),
               recHitCollection_EE = cms.InputTag("reducedEcalRecHitsEE"),
               theBeamSpotTag      = cms.InputTag("offlineBeamSpot"),
               PFMetTag            = cms.InputTag("pfMet"),
               rhoTag              = cms.InputTag("kt6PFJets","rho"),
               conversionsInputTag = cms.InputTag("allConversions"),
               dataRun             = cms.string("noCorrection"),
               MCtruthTag          = cms.InputTag("genParticles"),                          
               triggerResultsCollection = cms.InputTag('TriggerResults::HLT'),
               hltPaths = cms.vstring('HLT_GsfEle25_WP70_v13',
                                      'HLT_GsfEle25_WP80_v13',
                                      'HLT_GsfEle25_WP90_v13',
                                      'HLT_GsfEle25_WP70_PFMET_MT50_v9',
                                      'HLT_GsfEle25_WP80_PFMET_MT50_v9',
                                      'HLT_GsfEle25_WP90_PFMET_MT50_v9 '),
               verbosity           = cms.untracked.bool(False),
               doWZSelection       = cms.untracked.bool(False),  # save *all* electrons combinations
               applyCorrections    = cms.untracked.bool(False),
               dataFlag            = cms.untracked.bool(True),
               saveMCInfo          = cms.untracked.bool(False)           
)
