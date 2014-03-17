# fill the output RAW data collection
import FWCore.ParameterSet.Config as cms

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
				    
StremSelectedEventsGsf = cms.vstring('HLT_GsfEle25_WP70_v13',
 	 			     'HLT_GsfEle25_WP80_v13',
  			             'HLT_GsfEle25_WP90_v13',
				     'HLT_GsfEle25_WP70_PFMET_MT50_v9',
				     'HLT_GsfEle25_WP80_PFMET_MT50_v9',
				     'HLT_GsfEle25_WP90_PFMET_MT50_v9',
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

#hltOutputStreamElectron.outputCommands = [
#      'drop *',
#      'keep *_hltL1GtObjectMap_*_*',
#      'keep FEDRawDataCollection_rawDataCollector_*_*',
#      'keep FEDRawDataCollection_source_*_*',
#      'keep edmTriggerResults_*_*_*',
#      'keep triggerTriggerEvent_*_*_*'
#]


## define what to store in the output
hltOutputStreamElectron.outputCommands = ['drop * ',
                                          'keep *_edmTriggerResults_*_*',
                                          'keep *_hltL1GtObjectMap_*_*',
                                          'keep triggerTriggerEvent_*_*_*',
					  'keep *_HLTselectedElectronFEDList_*StremElectronRawFedData*_*'
					  ]
 

HLTselectedElectronFEDList = cms.EDProducer("selectedElectronFEDListProducerv2",
	electronCollections = cms.VInputTag('hltGsfEle25WP80TrackIsoFilter'),
	isGsfElectronCollection = cms.vint32(True), 
	beamSpot            = cms.InputTag("hltOnlineBeamSpot"),
	ESLookupTable       = cms.untracked.FileInPath('EventFilter/ESDigiToRaw/data/ES_lookup_table.dat'),
	dRStripRegion       = cms.double(0.5),
	dPhiPixelRegion     = cms.double(0.5),
	dEtaPixelRegion     = cms.double(0.5),
	maxZPixelRegion     = cms.double(24.),
	outputLabelModule   = cms.string("StremElectronRawFedData"),
	rawDataLabel        = cms.InputTag("rawDataCollector"),
	dumpEcalFedList         = cms.bool(True),
	dumpTrackSiStripFedList = cms.bool(True),
	dumpTrackSiPixelFedList = cms.bool(True),
	dumpAllEcalFed  = cms.bool(False),
	dumpAllTrackerFed = cms.bool(False),
	dumpHCALFed       = cms.bool(True),
        dumpThisSelectedFEDs = cms.vint32(812,813),                                     
	debug = cms.bool(True),
)
