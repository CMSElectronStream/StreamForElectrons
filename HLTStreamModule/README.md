========================
== StreamForElectrons ==
========================

Tutorial:

    https://indico.cern.ch/conferenceDisplay.py?confId=268771

Test sample:

    eos cp /eos/cms/store/group/comm_trigger/TriggerStudiesGroup/Skims/Top/SingleMuDS/pickevents_SemiMuon10k_1_1_Bbo.root ./

========================

To install in CMSSW_6_2_X :

    cmsrel CMSSW_6_2_2
    cd CMSSW_6_2_2/src/
    cmsenv
    git cms-addpkg HLTrigger
    git cms-addpkg DataFormats/HLTReco
    git cms-addpkg RecoEgamma/EgammaHLTProducers 
    git clone git@github.com:CMSElectronStream/StreamForElectrons.git 
    cd StreamForElectrons/ 
    git checkout -b STREAM_CMSSW_6_2_X 
    cd - ;
    cp StreamForElectrons/HLTStreamModule/python/* HLTrigger/Configuration/python 
    cp StreamForElectrons/HLTStreamModule/test/streamEle_cfg.py HLTrigger/Configuration/test
    cp StreamForElectrons/HLTStreamModule/test/ES_lookup_table.dat HLTrigger/Configuration/test
    cp StreamForElectrons/HLTStreamModule/plugins/HLTElectronOneOEMinusOneOPFilterRegional.h HLTrigger/Egamma/interface/
    cp StreamForElectrons/HLTStreamModule/plugins/HLTElectronOneOEMinusOneOPFilterRegional.cc HLTrigger/Egamma/src/
    cp StreamForElectrons/HLTStreamModule/plugins/EgammaHLTGsfTrackVarProducer.h RecoEgamma/EgammaHLTProducers/interface/
    cp StreamForElectrons/HLTStreamModule/plugins/EgammaHLTGsfTrackVarProducer.cc RecoEgamma/EgammaHLTProducers/src/
    cp StreamForElectrons/HLTStreamModule/plugins/selectedElectronFEDListProducer.h  HLTrigger/Egamma/interface/
    cp StreamForElectrons/HLTStreamModule/plugins/selectedElectronFEDListProducer.cc HLTrigger/Egamma/src/
    cp StreamForElectrons/HLTStreamModule/plugins/selectedElectronFEDListProducerv2.h  HLTrigger/Egamma/interface/
    cp StreamForElectrons/HLTStreamModule/plugins/selectedElectronFEDListProducerv2.cc HLTrigger/Egamma/src/
    cp StreamForElectrons/HLTStreamModule/plugins/BuildFile.xml.decomment HLTrigger/Egamma/BuildFile.xml
    cp StreamForElectrons/HLTStreamModule/test/ES_lookup_table.dat             HLTrigger/Configuration/test  
    cp StreamForElectrons/HLTStreamModule/test/HcalElectronicsMap_v7.00_offline HLTrigger/Configuration/test
    scramv1 b -j 8

test:

    cd HLTrigger/Configuration/test ;
    cmsRun streamEle_cfg.py

========================

To install in CMSSW_7_1_X (on SLC6 machine only):

    cmsrel CMSSW_7_1_0
    cd CMSSW_7_1_0/src/
    cmsenv
    git cms-addpkg HLTrigger
    git cms-addpkg DataFormats/HLTReco
    git cms-addpkg RecoEgamma/EgammaHLTProducers 
    git clone git@github.com:CMSElectronStream/StreamForElectrons.git 
    cd StreamForElectrons/ 
    git checkout -b STREAM_CMSSW_7_1_X 
    cd - ;
    cp StreamForElectrons/HLTStreamModule/python/* HLTrigger/Configuration/python 
    cp StreamForElectrons/HLTStreamModule/test/streamEle_cfg.py HLTrigger/Configuration/test
    cp StreamForElectrons/HLTStreamModule/plugins/selectedElectronFEDListProducerv2.h  HLTrigger/Egamma/interface/
    cp StreamForElectrons/HLTStreamModule/plugins/selectedElectronFEDListProducerv2.cc HLTrigger/Egamma/src/
    cp StreamForElectrons/HLTStreamModule/plugins/BuildFile.xml.decomment              HLTrigger/Egamma/BuildFile.xml
    cp StreamForElectrons/HLTStreamModule/test/HcalElectronicsMap_v7.00_offline HLTrigger/Configuration/test
    cp StreamForElectrons/HLTStreamModule/test/ES_lookup_table.dat HLTrigger/Configuration/test
    rm -rf StreamForElectrons/RateAndEfficiencyTriggerStudy 
    rm StreamForElectrons/AnalyzerEle/src/EleSelectionProducers.cc
    rm StreamForElectrons/AnalyzerEle/src/SimpleCutBasedElectronIDSelectionFunctor.cc
    rm StreamForElectrons/AnalyzerEle/src/BuildFile.xml
    rm -rf StreamForElectrons/AnalyzerEle/src/
    scramv1 b -j 8 -Werror=unused-variable

test:

Data:

    cd HLTrigger/Configuration/test ;
    cmsRun streamEle_cfg.py  inputFiles=file:root://xrootd.unl.edu//store/data/Run2012C/SingleElectron/RAW/v1/000/198/022/3859DED3-DDC3-E111-AAF6-001D09F24D67.root

MC:

    cd StreamForElectrons/HLTStreamModule/test/
    scp amassiro@cmsneu.cern.ch:/data/amassiro/CMSSWRoot/Spring14/DYToEE_Tune4C_13TeV-pythia8_GEN-SIM-RAW_tsg_PU40bx50_POSTLS162_V2-v1/009A2A53-A16B-E311-8345-003048F0E3B2.root /tmp/amassiro/
    cmsRun StreamForElectrons/HLTStreamModule/test/streamEle_cfg.py  inputFiles=file:/tmp/amassiro/009A2A53-A16B-E311-8345-003048F0E3B2.root   isMC=True
    

    
============================================================
============================================================
============================================================

1. python to be executed via cmsRun is streamEle_cfg.py (cmsRun streamEle_cfg.py saveAlcaElectronStreamOutput=1 outputFile=streamElectronRAW.root maxEvents=1000):

   #### The following part are loaded:
   process.source = cms.Source( "PoolSource",
      fileNames = cms.untracked.vstring('file:/media/DATA/CMSSWRoot/DATA2012/SingleElectron_Run2012B_RAW/B865DABE-BDA2-E111-854F-BCAEC53296F7.root'),
      secondaryFileNames = cms.untracked.vstring(),
      inputCommands = cms.untracked.vstring('keep *'))

   process.MessageLogger.cerr.FwkReport.reportEvery = 1 

   #enable the TrigReport and TimeReport
   process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool( True ))

   process.load("HLTrigger.Configuration.SourceHLT_cff") -> add to the process the HLT ESSource
   process.load("HLTrigger.Configuration.ESProducerHLT_cff") -> add to the process the HLT ESProdcer (geometry, track reco etc ..)
   process.load("HLTrigger.Configuration.ServiceHLT_cff") -> add some service tool, like PrescaleService -> define the prescale, for the moment I have put there just the path that I have developed

   process.load("HLTrigger.Configuration.HLTBeginSequence_cff") -> first unpacker stage in common for all the trigger paths
   process.hltL1sL1SingleEG20ORL1SingleEG22 ; process.hltL1sL1DoubleEG137 -> L1 seeds for single Electron and double ele paths

   process.HLTriggerFirstPath = cms.Path( process.hltGetConditions + process.hltGetRaw + process.hltBoolFalse ) -> basic path run in HLT FULL

   ##### final path in common with all the HLT paths in the HLT Full code
   process.HLT_LogMonitor_v4 = cms.Path(process.hltGtDigis+process.hltLogMonitorFilter+process.hltPreLogMonitor+process.HLTEndSequence )

   ##### final HLT trigger path
   process.HLTriggerFinalPath = cms.Path( process.hltGtDigis +
	                                  process.hltScalersRawToDigi +
	              	   		  process.hltFEDSelector +
		                          process.hltTriggerSummaryAOD +
				          process.hltTriggerSummaryRAW )

2. Single Electron Paths --> use the GsfTrack by default in 7X --> you can load the sequences in the process by:

        process.load("HLTrigger.Configuration.HLT_Ele27_WP80_cff")

   then define the path:

        process.HLT_Ele27_WP80_Gsf_v1 = cms.Path(process.HLTBeginSequence +  # unpacker
                                                 process.hltL1sL1SingleEG20ORL1SingleEG22 + #L1 seed
                                                 process.HLTEle27WP80Sequence+ #Sequence defined HLT_Ele27_WP80_cff.py
                                                 process.HLTEndSequence) #end sequence 


4. Output Producer:

        process.load('HLTrigger.Configuration.HLT_ElectronStreamOutput_cff')

        process.hltOutputStreamElectron.fileName = cms.untracked.string("streamElectronRAW_tmp.root")

        process.ElectronStreamOutputPath = cms.EndPath( process.hltPreOutput + 
	                                                process.HLTselectedElectronFEDList + ## this calls the FED subset producer 
		  				        process.hltOutputStreamElectron) ## this defines the output content and the trigger path

        ## select the trigger path to use to fire the output collection                                                                                          
        StremSelectedEvents =  cms.vstring("HLT_Ele27_WP80_Gsf_v1",#"HLT_Ele27_WP80_Gsf_PFMET_MT50_v1")

        hltOutputStreamElectron = cms.OutputModule( "PoolOutputModule",
          fileName = cms.untracked.string( "outputHLT.root" ),
          fastCloning = cms.untracked.bool( False ),
          dataset = cms.untracked.PSet(
            filterName = cms.untracked.string( "" ),
            dataTier = cms.untracked.string( "RAW" )),
          SelectEvents = cms.untracked.PSet(SelectEvents =(StremSelectedEvents)),
          outputCommands = cms.untracked.vstring('keep *'))


        if not saveAlcaElectronStreamOutput :

         process.hltOutputStreamElectron.outputCommands = [ 'drop *',
                              'keep *_hltL1GtObjectMap_*_*',
                              'keep FEDRawDataCollection_rawDataCollector_*_*',
                              'keep FEDRawDataCollection_source_*_*',
                              'keep edmTriggerResults_*_*_*',
                              'keep triggerTriggerEvent_*_*_*',
                              'drop *_*_*_*HLT*']

        else:

         ## define what to store in the output                                                                                                                           
         process.hltOutputStreamElectron.outputCommands = ['drop * ',
                                            'keep edmTriggerResults_*_*_*',
                                            'keep *_hltL1GtObjectMap_*_*',
                                            'keep triggerTriggerEvent_*_*_*',
                                            'keep *_HLTselectedElectronFEDList_*StremElectronRawFedData*_*',
                                            'keep *_*hltFixedGridRhoFastjetAllCaloForMuons*_*_*' ,
                                            'keep *_*hltEgammaEcalPFClusterIso*_*_*',
                                            'keep *_*hltEgammaEleGsfTrackIso*_*_*',
                                            'keep *_*hltEgammaHcalPFClusterIso*_*_*',
                                            'keep *_*hltPixelVertices*_*_*',
                                            'drop *_*_*_*HLT*'
                                           ]


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
         debug = cms.bool(False))

     * recoEcalCandidateCollections -> this is the collection produced by the HLT filters
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
     * addThisSelectedFEDs -> add selected feds, e.g. L1GT feds
     * debug -> if true debug mode -> print a lot of info



==================
Test instructions
==================

where:

    cmsneu
    /home/amassiro/ECAL/HLT/CMSSW_6_2_2/src/StreamForElectrons

    cd /home/amassiro/ECAL/HLT/CMSSW_6_2_2/src/
    cd HLTrigger/Configuration/test
    cmsRun streamEle_cfg.py


25ns run:

    root://cms-xrd-global.cern.ch//store/data/Run2012D/SingleElectron25ns/RAW/v1/000/209/089/3E148D63-E246-E211-A824-003048D2C0F2.root
    in
    streamEle_cfg.py


To run only the trigger and dump all raw information:

    cmsRun streamEle_cfg.py saveAlcaElectronStreamOutput=0   outputFile=simpleRaw.root

To run the stream:

    cmsRun streamEle_cfg.py saveAlcaElectronStreamOutput=1   outputFile=Stream.root



==================
Test HLT 
==================

Instructions:

    https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideGlobalHLT

to do:

    hltGetConfiguration /users/amassiro/amassiro/V6 --full --offline --mc --unprescale --process TEST --globaltag auto:run2_mc_GRun --l1-emulator 'stage1,gt' --l1Xml L1Menu_Collisions2015_25ns_v2_L1T_Scales_20141121_Imp0_0x1030.xml   > hlt_EleStream_2.py
    hltGetConfiguration /users/amassiro/amassiro/V7 --full --offline --mc --unprescale --process TEST --globaltag auto:run2_mc_GRun --l1-emulator 'stage1,gt' --l1Xml L1Menu_Collisions2015_25ns_v2_L1T_Scales_20141121_Imp0_0x1030.xml   > hlt_EleStream_3.py

    cmsRun hlt_EleStream_3.py
    

test ReReco:
          
    cd /afs/cern.ch/user/a/amassiro/work/ECALHLT/CMSSW_7_1_0/src/StreamForElectrons/ReReco
    
    cmsRun step2_RAW2DIGI_L1Reco_RECO_onStream.py  \
        inputFiles=file:/afs/cern.ch/user/a/amassiro/work/ECALHLT/CMSSW_7_4_2/src/HLTrigger/Configuration/test/outputElectronStream.root  \
        outputFile=reco_stream_test_HLT.root \
        isMC=True
        

    cmsShow reco_stream_test_HLT  --no-version-check
        
        
        
Integration test:

    hltIntegrationTests /users/amassiro/amassiro/V7 -s /dev/CMSSW_7_4_0/HLT -i   root://xrootd.unl.edu//store/mc/Phys14DR/DYToEE_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/PU40bx25_tsg_castor_PHYS14_25_V1-v2/00000/06911F9F-9899-E411-B576-001E67396ACC.root  --mc -x "--globaltag auto:run2_mc_GRun"     -x "--l1-emulator stage1,gt" -x "--l1Xml L1Menu_Collisions2015_25ns_v2_L1T_Scales_20141121_Imp0_0x1030.xml"

    
    
    

    
    
    
    
