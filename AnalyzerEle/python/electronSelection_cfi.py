import FWCore.ParameterSet.Config as cms

eleSelectionProducers = cms.EDProducer('EleSelectionProducers',
                                       electronCollection = cms.InputTag('gsfElectrons'),
                                       rhoFastJet = cms.InputTag('hltKT6PFJets',"rho"),
                                       vertexCollection = cms.InputTag('hltFastPVPixelVertices'),
                                       conversionCollection = cms.InputTag('allConversions'),
                                       BeamSpotCollection = cms.InputTag('offlineBeamSpot'),
                                       chIsoVals = cms.InputTag('elPFIsoValueCharged03PFId'),
                                       emIsoVals = cms.InputTag('elPFIsoValueGamma03PFId'),
                                       nhIsoVals = cms.InputTag('elPFIsoValueNeutral03PFId'))

