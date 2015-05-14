import FWCore.ParameterSet.Config as cms

eleSelectionProducers = cms.EDProducer('EleSelectionProducers',
                                       electronCollection = cms.InputTag('gedGsfElectrons'),
                                       rhoFastJet = cms.InputTag('fixedGridRhoAll'),
                                       vertexCollection = cms.InputTag('offlinePrimaryVerticesWithBS'),
                                       conversionCollection = cms.InputTag('allConversions'),
                                       BeamSpotCollection = cms.InputTag('offlineBeamSpot'),
                                       chIsoVals = cms.InputTag('elPFIsoValueCharged03PFId'),
                                       emIsoVals = cms.InputTag('elPFIsoValueGamma03PFId'),
                                       nhIsoVals = cms.InputTag('elPFIsoValueNeutral03PFId'))

