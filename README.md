StreamForElectrons
==================

Tutorial:

    https://indico.cern.ch/conferenceDisplay.py?confId=268771

Test sample:

    eos cp /eos/cms/store/group/comm_trigger/TriggerStudiesGroup/Skims/Top/SingleMuDS/pickevents_SemiMuon10k_1_1_Bbo.root ./


To install:

    cmsrel CMSSW_6_2_2
    cd CMSSW_6_2_2/src/
    cmsenv
    git cms-addpkg HLTrigger
    git cms-addpkg DataFormats/HLTReco
    git cms-addpkg RecoEgamma/EgammaHLTProducers    
    git clone git@github.com:CMSElectronStream/StreamForElectrons.git
    cp StreamForElectrons/python/* HLTrigger/Configuration/python 
    cp StreamForElectrons/test/streamEle_cfg.py HLTrigger/Configuration/test
    cp StreamForElectrons/plugins/HLTElectronOneOEMinusOneOPFilterRegional.h HLTrigger/Egamma/interface/
    cp StreamForElectrons/plugins/HLTElectronOneOEMinusOneOPFilterRegional.cc HLTrigger/Egamma/src/
    cp StreamForElectrons/plugins/EgammaHLTGsfTrackVarProducer.h RecoEgamma/EgammaHLTProducers/interface/
    cp StreamForElectrons/plugins/EgammaHLTGsfTrackVarProducer.cc RecoEgamma/EgammaHLTProducers/src/
    cp StreamForElectrons/plugins/selectedElectronFEDListProducer.h  HLTrigger/Egamma/interface/
    cp StreamForElectrons/plugins/selectedElectronFEDListProducer.cc HLTrigger/Egamma/src/
    scramv1 b -j 8

test:

    cd HLTrigger/Configuration/test ;

1. python to be executed via cmsRun is streamEle_cfg.py:

        # The following part are loaded:

        process.source = cms.Source( "PoolSource",
           fileNames = cms.untracked.vstring('file:/media/DATA/CMSSWRoot/DATA2012/SingleElectron_Run2012B_RAW/B865DABE-BDA2-E111-854F-BCAEC53296F7.root'),
           secondaryFileNames = cms.untracked.vstring(),
           inputCommands = cms.untracked.vstring('keep *'))

        process.MessageLogger.cerr.FwkReport.reportEvery = 1 

        # enable the TrigReport and TimeReport
        #process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool( True ))

        process.load("HLTrigger.Configuration.SourceHLT_cff") -> add to the process the HLT ESSource

        process.load("HLTrigger.Configuration.ESProducerHLT_cff") -> add to the process the HLT ESProdcer (geometry, track reco etc ..)

        process.load("HLTrigger.Configuration.ServiceHLT_cff") -> add some service tool, like PrescaleService -> define the prescale, for the moment
                                                              I have put there just the path that I have developed

        process.load("HLTrigger.Configuration.HLTBeginSequence_cff") -> first unpacker stage in common for all the trigger paths

        process.hltL1sL1SingleEG20ORL1SingleEG22 ; process.hltL1sL1DoubleEG137 -> L1 seeds for single Electron and double ele paths

        process.HLTriggerFirstPath = cms.Path( process.hltGetConditions + process.hltGetRaw + process.hltBoolFalse ) -> basic path run in HLT FULL

        ## final path in common with all the HLT paths in the HLT Full code

        process.HLT_LogMonitor_v4 = cms.Path( process.hltGtDigis +
                                  process.hltLogMonitorFilter +
              				      process.hltPreLogMonitor +
		                          process.HLTEndSequence )

        ## final HLT trigger path
        process.HLTriggerFinalPath = cms.Path( process.hltGtDigis +
	                                  process.hltScalersRawToDigi +
	              	   		          process.hltFEDSelector +
		                              process.hltTriggerSummaryAOD +
				                      process.hltTriggerSummaryRAW )



2. Single Electron Paths --> use the CFTrack --> you can load the sequences in the process by:

        process.load("HLTrigger.Configuration.HLT_Ele25_WP80_cff")

   then define the path:

        process.HLT_Ele25_WP80_v13 = cms.Path( process.HLTBeginSequence +  # unpacker
                                           process.hltL1sL1SingleEG20ORL1SingleEG22 + #L1 seed
                                           process.HLTEle25WP80Sequence+ #Sequence definedHLT_Ele25_WP80_cff.py
                                           process.HLTEndSequence) #end sequence 


        process.load("HLTrigger.Configuration.HLT_Ele25_WP80_PFMET_MT50_cff") -> in case you want a cross-trigger with PFMET+MT

3. Single Electron Paths --> use the GsfTrack --> you can load the sequences in the process by:


        process.load("HLTrigger.Configuration.HLT_Ele25_WP80_cff")

   then define the path:

        process.HLT_GsfEle25_WP80_v13 = cms.Path( process.HLTBeginSequence +
                                              process.hltL1sL1SingleEG20ORL1SingleEG22 +
                                              process.HLTGsfEle25WP80Sequence+
                                              process.HLTEndSequence)

        process.HLTGsfEle25WP80Sequence

    is equivalent to the process.HLTEle25WP80Sequence for the ECAL part.
    Then the Gsf tracking is run instead of the CF tracking.

    The problem is that to apply correctly the filters to do cut on : 1/E-1/p, DetaIn, DphiIn and TrackIso,
    the following codes have been changed:

                  HLTElectronOneOEMinusOneOPFilterRegional.h
                  HLTElectronOneOEMinusOneOPFilterRegional.cc 
                  EgammaHLTGsfTrackVarProducer.h 
                  EgammaHLTGsfTrackVarProducer.cc 


4. Output Producer:

        process.load('HLTrigger.Configuration.HLT_ElectronStreamOutput_cff')

        process.hltOutputStreamElectron.fileName = cms.untracked.string("streamElectronRAW_tmp.root")

        process.ElectronStreamOutputPath = cms.EndPath( process.hltPreOutput + 
	                                            process.HLTselectedElectronFEDList + ## this calls the FED subset producer 
		  				                        process.hltOutputStreamElectron) ## this defines the output content and the trigger path

        StremSelectedEventsGsf = cms.vstring('HLT_GsfEle25_WP70_v13', ## pay attention to comment the path that are not running, otherwise error
 	 			     'HLT_GsfEle25_WP80_v13',
                     'HLT_GsfEle25_WP90_v13',
				     'HLT_GsfEle25_WP70_PFMET_MT50_v9',
				     'HLT_GsfEle25_WP80_PFMET_MT50_v9',
				     'HLT_GsfEle25_WP90_PFMET_MT50_v9',)



        hltOutputStreamElectron = cms.OutputModule( "PoolOutputModule",
          fileName = cms.untracked.string( "outputHLT.root" ),
          fastCloning = cms.untracked.bool( False ),
          dataset = cms.untracked.PSet(
            filterName = cms.untracked.string( "" ),
            dataTier = cms.untracked.string( "RAW" )
           ),
          SelectEvents = cms.untracked.PSet(  SelectEvents = ( #StremSelectedEvents 
	 		                                 StremSelectedEventsGsf)),
          outputCommands = cms.untracked.vstring('keep *'))

        hltOutputStreamElectron.outputCommands = ['drop * ', ## output content
                                          'keep *_edmTriggerResults_*_*',
                                          'keep *_hltL1GtObjectMap_*_*',
                                          'keep triggerTriggerEvent_*_*_*',
					                      'keep *_HLTselectedElectronFEDList_*StremElectronRawFedData*_*']

        ## output producer
        HLTselectedElectronFEDList = cms.EDProducer("selectedElectronFEDListProducer",
    	electronCollections = cms.VInputTag('hltGsfEle25WP80TrackIsoFilter'),
    	isGsfElectronCollection = cms.vint32(True), 
    	ESLookupTable       = cms.untracked.FileInPath('EventFilter/ESDigiToRaw/data/ES_lookup_table.dat'),
    	outputLabelModule   = cms.string("StremElectronRawFedData"),
    	rawDataLabel        = cms.InputTag("rawDataCollector"),
    	dumpEcalFedList         = cms.bool(True),
    	dumpTrackSiStripFedList = cms.bool(True),
    	dumpTrackSiPixelFedList = cms.bool(True),
    	dumpAllEcalFed  = cms.bool(False),
    	dumpAllTrackerFed = cms.bool(False),
    	debug = cms.bool(True))





        ## New output producer for regional FED dumper in the tracker
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
    	debug = cms.bool(False))

     * electronCollections -> list of all the electron collection produced by the trigger paths you have chosen to be stored
     * isGsfElectronCollection -> list of bool to indicate if each electron collection is Gsf or standard reco electron
     * beamSpot -> beam spot tag, fundamental for pixel fed dump
     * ESLookupTable -> look up table for fed mapping in the pre-shower
     * dRStripRegion -> dump al the FED in the strips related to all the channel within a dR wrt the electron track
     * dPhiPixelRegion -> dump al the FED in the pixel related to all the channel within a dPhi window wrt the electron track position
     * dEtaPixelRegion -> dump al the FED in the pixel related to all the channel within a dEta window wrt the electron track position    
     * maxZPixelRegion -> dump al the FED in the pixel related to all the channel within a maxZ wrt to the beam spot
     * outputLabelModule -> Label of the output module collection
     * rawDataLabel -> name for the raw in the output

     * dumpEcalFedList -> bool to dump the ecal fed related to the SC + pre-shower in case of electron in EE
     * dumpTrackSiStripFedList -> bool to dump the fed on the strip inside the cone region of the electron track
     * dumpTrackSiPixelFedList -> bool to dump the fed on the pixel inside the eta-phi window wrt to the electron track position

     * dumpAllEcalFed -> if true dump all the ecal fed
     * dumpAllTrackerFed -> if true dump all the tracker fed in both strip and pixel detector
     * debug -> if true debug mode -> print a lot of info




Test instructions
==================

where:

    cmsneu
    /home/amassiro/ECAL/HLT/CMSSW_6_2_2/src/StreamForElectrons


