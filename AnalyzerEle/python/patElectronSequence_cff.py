import FWCore.ParameterSet.Config as cms

##------------------------------ electron selection producer
from StreamForElectrons.AnalyzerEle.electronSelection_cfi import *

from PhysicsTools.PatAlgos.producersLayer1.electronProducer_cfi import *

patElectrons.electronSource  = cms.InputTag("gsfElectrons")
patElectrons.embedHighLevelSelection = cms.bool(False)
patElectrons.useParticleFlow = cms.bool(False)
patElectrons.addElectronID   = cms.bool(True)
patElectrons.addGenMatch     = cms.bool(True)
patElectrons.pvSrc           = cms.InputTag("offlinePrimaryVertices")
patElectrons.reducedBarrelRecHitCollection = cms.InputTag("reducedEcalRecHitsEB")
patElectrons.reducedEndcapRecHitCollection = cms.InputTag("reducedEcalRecHitsEE")
patElectrons.embedPFCandidate = cms.bool(True) ## embed in AOD externally stored particle flow candidate
patElectrons.embedTrack = cms.bool(True) ## embed in AOD externally stored track (note: gsf electrons don't have a track), make it point to reducedtrack collection
patElectrons.isolationValues = cms.PSet(
        pfChargedHadrons   = cms.InputTag("elPFIsoValueCharged03PFId"),
        pfChargedAll       = cms.InputTag("elPFIsoValueChargedAll03PFId"),
        pfPUChargedHadrons = cms.InputTag("elPFIsoValuePU03PFId"),
        pfNeutralHadrons   = cms.InputTag("elPFIsoValueNeutral03PFId"),
        pfPhotons          = cms.InputTag("elPFIsoValueGamma03PFId")
)  

patElectrons.electronIDSources = cms.PSet(
# configure many IDs as InputTag <someName> = <someTag> you
# can comment out those you don't want to save some disk space
  fiducial = cms.InputTag("eleSelectionProducers", "fiducial"),
  WP70PU   = cms.InputTag("eleSelectionProducers", "WP70PU"),
  WP80PU   = cms.InputTag("eleSelectionProducers", "WP80PU"),
  WP90PU   = cms.InputTag("eleSelectionProducers", "WP90PU"),
  loose    = cms.InputTag("eleSelectionProducers", "loose"),
  medium   = cms.InputTag("eleSelectionProducers", "medium"),
  tight    = cms.InputTag("eleSelectionProducers", "tight")
)



##------------------------------ trigger information producer
from PhysicsTools.PatAlgos.triggerLayer1.triggerProducer_cfi import *

patTrigger.processName  = cms.string("HLT8E33v2")

PatElectronTriggerMatchHLTEle = cms.EDProducer("PATTriggerMatcherDRDPtLessByR", # match by DeltaR only, best match by DeltaR
            src = cms.InputTag("patElectrons"),
            matched = cms.InputTag("patTrigger"), # default producer label as defined in PhysicsTools/PatAlgos/python/triggerLayer1/triggerProducer_cfi.py
            matchedCuts = cms.string('path("HLT_GsfEle25_WP80_PFMET_MT50_v9")'),
            maxDPtRel = cms.double(0.5),
            maxDeltaR = cms.double(0.5),
            resolveAmbiguities = cms.bool(True), # only one match per trigger object
            resolveByMatchQuality = cms.bool(True))


##------------------------------ trigger matching embedder
PatElectronsTriggerMatch = cms.EDProducer("PATTriggerMatchElectronEmbedder",
                           src = cms.InputTag( "patElectrons" ),
                           matches = cms.VInputTag('PatElectronTriggerMatchHLTEle'))

##------------------------------ trigger event producerD
from PhysicsTools.PatAlgos.triggerLayer1.triggerEventProducer_cfi import *
patTriggerEvent.patTriggerMatches = cms.VInputTag( 'PatElectronsTriggerMatch' )
patTriggerEvent.processName  = cms.string("HLT8E33v2")


############### Final Sequence
patElectronSequence = cms.Sequence(eleSelectionProducers* ## make the electron id info on gsf electron
                                   patElectrons* ## build pat electron 
                                   patTrigger*   ## patTrigger
                                   PatElectronTriggerMatchHLTEle* ## matching electron with pat trigger
                                   PatElectronsTriggerMatch*                                         
                                   patTriggerEvent)
