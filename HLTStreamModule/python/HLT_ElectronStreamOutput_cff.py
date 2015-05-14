# fill the output RAW data collection
import FWCore.ParameterSet.Config as cms

def ElectronStreamOutput(process,
                         saveAlcaElectronStreamOutput):

 ## pre output prescaling
 process.hltPreOutput = cms.EDFilter( "HLTPrescaler",
                                       L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
                                       offset = cms.uint32( 0 ))

 ## select the trigger path to use to fire the output collection
 StreamSelectedEvents =  cms.vstring("HLT_Ele27_WP80_Gsf_v1",
				    #"HLT_Ele27_WP80_Gsf_PFMET_MT50_v1"
 ) 
				    

 process.HLTselectedElectronFEDList = cms.EDProducer("selectedElectronFEDListProducerv2",
	 electronCollections = cms.VInputTag('hltEgammaGsfElectrons'),
         recoEcalCandidateCollections = cms.VInputTag('hltEle27WP80TrackIsoFilter'),
	 isGsfElectronCollection = cms.vint32(True), 
	 beamSpot            = cms.InputTag("hltOnlineBeamSpot"),
         HBHERecHitCollection = cms.InputTag("hltHbhereco"),                
	 ESLookupTable       = cms.string("ES_lookup_table.dat"),
         HCALLookUpTable     = cms.string("HcalElectronicsMap_v7.00_offline"),                              
	 dRStripRegion       = cms.double(0.3),
	 dRHcalRegion        = cms.double(0.5),
	 dPhiPixelRegion     = cms.double(0.3),
	 dEtaPixelRegion     = cms.double(0.3),
	 maxZPixelRegion     = cms.double(24.),
	 outputLabelModule   = cms.string("StreamElectronRawFedData"),
	 rawDataLabel        = cms.InputTag("rawDataCollector"),
	 dumpSelectedEcalFed     = cms.bool(True),
	 dumpSelectedSiStripFed  = cms.bool(True),
	 dumpSelectedSiPixelFed  = cms.bool(True),
	 dumpSelectedHCALFed     = cms.bool(True),
	 dumpAllEcalFed    = cms.bool(False),
	 dumpAllTrackerFed = cms.bool(False),
	 dumpAllHcalFed    = cms.bool(False),
         addThisSelectedFEDs = cms.vint32(812,813),                                     
	 debug = cms.bool(False))

 process.hltOutputStreamElectron = cms.OutputModule( "PoolOutputModule",
    fileName = cms.untracked.string( "outputHLT.root" ),
    fastCloning = cms.untracked.bool( False ),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string( "" ),
        dataTier = cms.untracked.string( "RAW" )
    ),
    SelectEvents = cms.untracked.PSet(  SelectEvents = (StreamSelectedEvents)),
    outputCommands = cms.untracked.vstring('keep *'))
 
 if not saveAlcaElectronStreamOutput : 

  process.hltOutputStreamElectron.outputCommands = [
      'drop *',
      'keep *_hltL1GtObjectMap_*_*',
      'keep triggerTriggerEvent_*_*_*',
      'keep edmTriggerResults_*_*_*',
      'drop *_*_*_*HLT*',
      'keep FEDRawDataCollection*_*_*_*',
      'keep *_simMuonCSCDigis_*_*',
      'keep *_simMuonRPCDigis_*_*'
  ]


 else:

  ## define what to store in the output
  process.hltOutputStreamElectron.outputCommands = ['drop * ',
                                            'keep edmTriggerResults_*_*_*',
                                            'keep *_hltL1GtObjectMap_*_*',
                                            'keep triggerTriggerEvent_*_*_*',
                                            'drop *_*_*_*HLT*',
   	   				    'keep *_HLTselectedElectronFEDList_*StreamElectronRawFedData*_*',
                                            'keep *_*hltFixedGridRho*_*_*' ,
                                            'keep *_*hltEgammaEcalPFClusterIso*_*_*',
                                            'keep *_*hltEgammaEleGsfTrackIso*_*_*',
                                            'keep *_*hltEgammaHcalPFClusterIso*_*_*',
                                            'keep *_*hltPixelVertices*_*_*',
                                            'keep *_simMuonCSCDigis_*_*',
                                            'keep *_simMuonRPCDigis_*_*'
					   ]
 

