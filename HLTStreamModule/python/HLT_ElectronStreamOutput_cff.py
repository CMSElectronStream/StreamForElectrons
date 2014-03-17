# fill the output RAW data collection
import FWCore.ParameterSet.Config as cms

dumpAllRAW = False

## pre output prescaling
hltPreOutput = cms.EDFilter( "HLTPrescaler",
  L1GtReadoutRecordTag = cms.InputTag( "hltGtDigis" ),
  offset = cms.uint32( 0 ))

## select the trigger path to use to fire the output collection
StremSelectedEvents =  cms.vstring('HLT_Ele25_WP70_v13',
				   'HLT_Ele25_WP80_v13',
  			           'HLT_Ele25_WP90_v13',
				   'HLT_Ele25_WP70_PFMET_MT50_v9',
				   'HLT_Ele25_WP80_PFMET_MT50_v9',
				   'HLT_Ele25_WP90_PFMET_MT50_v9',
				   'HLT_Ele17_Ele8_WP90_v1',
				   'HLT_Ele17_Ele_12_WP90_v1',
				   'HLT_DoubleEle17_WP90_v1'
			            ) 
				    
StremSelectedEventsGsf = cms.vstring(#'HLT_GsfEle25_WP70_v13',
 	 			     'HLT_GsfEle25_WP80_v13',
  			             #'HLT_GsfEle25_WP90_v13',
				     #'HLT_GsfEle25_WP70_PFMET_MT50_v9',
				     'HLT_GsfEle25_WP80_PFMET_MT50_v9',
				     #'HLT_GsfEle25_WP90_PFMET_MT50_v9',
)

hltOutputStreamElectron = cms.OutputModule( "PoolOutputModule",
    fileName = cms.untracked.string( "outputHLT.root" ),
    fastCloning = cms.untracked.bool( False ),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string( "" ),
        dataTier = cms.untracked.string( "RAW" )
    ),
    SelectEvents = cms.untracked.PSet(  SelectEvents = ( #StremSelectedEvents 
			                                 StremSelectedEventsGsf)),
    outputCommands = cms.untracked.vstring('keep *')
)

if dumpAllRAW : 

 hltOutputStreamElectron.outputCommands = [
      'drop *',
      'keep *_hltL1GtObjectMap_*_*',
      'keep FEDRawDataCollection_rawDataCollector_*_*',
      'keep FEDRawDataCollection_source_*_*',
      'keep edmTriggerResults_*_*_*',
      'keep triggerTriggerEvent_*_*_*'
      'keep *_*hltKT6PFJets_rho_*' ,
      'keep *_*hltGsfEleAnyL1SeededElectronTrackIso*_*_*',
      'keep *_*hltL1SeededGsfTrackVars*_*_*',
      'keep *_*hltL1SeededPhotonEcalIso*_*_*',
      'keep *_*hltL1SeededPhotonHcalIso*_*_*',
      'keep *_*hltL1SeededPhotonHcalForHE*_*_*',
      'keep *_*hltPFMETProducer*_*_*', 
      'keep *_*hltFastPVPixelVertices*_*_*'
 ]


else:

 ## define what to store in the output
 hltOutputStreamElectron.outputCommands = ['drop * ',
                                           'keep *_edmTriggerResults_*_*',
                                           'keep *_hltL1GtObjectMap_*_*',
                                           'keep triggerTriggerEvent_*_*_*',
	  				   'keep *_HLTselectedElectronFEDList_*StremElectronRawFedData*_*',
#                                           'keep *_*hltKT6PFJets_rho_*' ,
#                                           'keep *_*hltGsfEleAnyL1SeededElectronTrackIso*_*_*',
#                                           'keep *_*hltL1SeededGsfTrackVars*_*_*',
#                                           'keep *_*hltL1SeededPhotonEcalIso*_*_*',
#                                           'keep *_*hltL1SeededPhotonHcalIso*_*_*',
#                                           'keep *_*hltL1SeededPhotonHcalForHE*_*_*',
#                                           'keep *_*hltPFMETProducer*_*_*',
#                                           'keep *_*hltFastPVPixelVertices*_*_*'
					  ]
 

HLTselectedElectronFEDList = cms.EDProducer("selectedElectronFEDListProducerv2",
	electronCollections = cms.VInputTag('hltEle25WP80PFMT50PFMTFilter'),
	isGsfElectronCollection = cms.vint32(True), 
	beamSpot            = cms.InputTag("hltOnlineBeamSpot"),
        hcalTowersTag       = cms.InputTag("hltTowerMakerForAll"),                
	ESLookupTable       = cms.untracked.FileInPath('EventFilter/ESDigiToRaw/data/ES_lookup_table.dat'),
	dRStripRegion       = cms.double(0.3),
	dPhiPixelRegion     = cms.double(0.3),
	dEtaPixelRegion     = cms.double(0.3),
	maxZPixelRegion     = cms.double(24.),
	outputLabelModule   = cms.string("StremElectronRawFedData"),
	rawDataLabel        = cms.InputTag("rawDataCollector"),
	dumpSelectedEcalFed     = cms.bool(True),
	dumpSelectedSiStripFed  = cms.bool(True),
	dumpSelectedSiPixelFed  = cms.bool(True),
	dumpSelectedHCALFed     = cms.bool(True),
	dumpAllEcalFed    = cms.bool(False),
	dumpAllTrackerFed = cms.bool(False),
	dumpAllHcalFed    = cms.bool(False),
        addThisSelectedFEDs = cms.vint32(812,813),                                     
	debug = cms.bool(False),
)
