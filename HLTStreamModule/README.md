========================
== StreamForElectrons ==
========================

Tutorial:

    https://indico.cern.ch/conferenceDisplay.py?confId=268771

Test sample:

    eos cp /eos/cms/store/group/comm_trigger/TriggerStudiesGroup/Skims/Top/SingleMuDS/pickevents_SemiMuon10k_1_1_Bbo.root ./

========================

To install in CMSSW_8_0_1 :

    cmsrel CMSSW_8_0_1
    cd CMSSW_8_0_1/src/
    cmsenv

    

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

    cd /afs/cern.ch/user/a/amassiro/work/ECALHLT/CMSSW_7_4_2/src/HLTrigger/Configuration/test/

    hltGetConfiguration /users/amassiro/amassiro/V6 --full --offline --mc --unprescale --process TEST --globaltag auto:run2_mc_GRun --l1-emulator 'stage1,gt' --l1Xml L1Menu_Collisions2015_25ns_v2_L1T_Scales_20141121_Imp0_0x1030.xml   > hlt_EleStream_2.py
    hltGetConfiguration /users/amassiro/amassiro/V7 --full --offline --mc --unprescale --process TEST --globaltag auto:run2_mc_GRun --l1-emulator 'stage1,gt' --l1Xml L1Menu_Collisions2015_25ns_v2_L1T_Scales_20141121_Imp0_0x1030.xml   > hlt_EleStream_3.py
    hltGetConfiguration /users/amassiro/amassiro/V8 --full --offline --mc --unprescale --process TEST --globaltag auto:run2_mc_GRun --l1-emulator 'stage1,gt' --l1Xml L1Menu_Collisions2015_25ns_v2_L1T_Scales_20141121_Imp0_0x1030.xml   > hlt_EleStream_3.py
    hltGetConfiguration /users/amassiro/amassiro/V10 --full --offline --mc --unprescale --process TEST --globaltag auto:run2_mc_GRun --l1-emulator 'stage1,gt' --l1Xml L1Menu_Collisions2015_25ns_v2_L1T_Scales_20141121_Imp0_0x1030.xml   > hlt_EleStream_MC.py
    hltGetConfiguration /users/amassiro/amassiro/V12 --full --offline --mc --unprescale --process TEST --globaltag auto:run2_mc_GRun --l1-emulator 'stage1,gt' --l1Xml L1Menu_Collisions2015_25ns_v2_L1T_Scales_20141121_Imp0_0x1030.xml   > hlt_EleStream_MC.py
    hltGetConfiguration /users/amassiro/amassiro/V16 --full --offline --mc --unprescale --process TEST --globaltag auto:run2_mc_GRun --l1-emulator 'stage1,gt' --l1Xml L1Menu_Collisions2015_25ns_v2_L1T_Scales_20141121_Imp0_0x1030.xml   > hlt_EleStream_MC.py
    hltGetConfiguration /users/amassiro/amassiro/V18 --full --offline --mc --unprescale --process TEST --globaltag auto:run2_mc_GRun --l1-emulator 'stage1,gt' --l1Xml L1Menu_Collisions2015_25ns_v2_L1T_Scales_20141121_Imp0_0x1030.xml   > hlt_EleStream_MC.py
    hltGetConfiguration /users/amassiro/amassiro/V19 --full --offline --mc --unprescale --process TEST --globaltag auto:run2_mc_GRun --l1-emulator 'stage1,gt' --l1Xml L1Menu_Collisions2015_25ns_v2_L1T_Scales_20141121_Imp0_0x1030.xml   > hlt_EleStream_MC.py
    hltGetConfiguration /users/amassiro/amassiro/V20 --full --offline --mc --unprescale --process TEST --globaltag auto:run2_mc_GRun --l1-emulator 'stage1,gt' --l1Xml L1Menu_Collisions2015_25ns_v2_L1T_Scales_20141121_Imp0_0x1030.xml   > hlt_EleStream_MC.py

    hltGetConfiguration /users/amassiro/amassiro/V22 --full --offline --mc --unprescale --process TEST --globaltag auto:run2_mc_GRun --l1-emulator 'stage1,gt' --l1Xml L1Menu_Collisions2015_25ns_v2_L1T_Scales_20141121_Imp0_0x1030.xml   > hlt_EleStream_MC.py
    hltGetConfiguration /users/amassiro/amassiro/V23 --full --offline --mc --unprescale --process TEST --globaltag auto:run2_mc_GRun --l1-emulator 'stage1,gt' --l1Xml L1Menu_Collisions2015_25ns_v2_L1T_Scales_20141121_Imp0_0x1030.xml   > hlt_EleStream_MC.py

    hltGetConfiguration /users/amassiro/myHLTAlCaEle/V3 --full --offline --mc --unprescale --process TEST --globaltag auto:run2_mc_GRun \
         --input  /store/relval/CMSSW_8_0_1/RelValWE_13/GEN-SIM-DIGI-RAW-HLTDEBUG/80X_mcRun2_asymptotic_v6-v1/10000/BA9CD940-4FE4-E511-83FD-0CC47A4D761A.root   > hlt_EleStream_MC.py

    cmsRun hlt_EleStream_MC.py
     
    
    cmsRun hlt_EleStream_3.py
    cmsRun hlt_EleStream_MC.py

        
    hltGetConfiguration /users/amassiro/amassiro/V8  --full --offline --data --unprescale --process TEST --globaltag auto:run2_mc_GRun --l1-emulator 'stage1,gt' --l1Xml L1Menu_Collisions2015_25ns_v2_L1T_Scales_20141121_Imp0_0x1030.xml   > hlt_EleStream_4.py
    hltGetConfiguration /users/amassiro/amassiro/V16 --full --offline --data --unprescale --process TEST --globaltag auto:run2_mc_GRun --l1-emulator 'stage1,gt' --l1Xml L1Menu_Collisions2015_25ns_v2_L1T_Scales_20141121_Imp0_0x1030.xml   > hlt_EleStream_4.py
    file:root://xrootd.unl.edu//store/data/Run2012C/SingleElectron/RAW/v1/000/198/022/3859DED3-DDC3-E111-AAF6-001D09F24D67.root
    
    cmsRun hlt_EleStream_4.py
    
    
    
    cmsrel CMSSW_7_5_X_2015-06-11-2300
    cd CMSSW_7_5_X_2015-06-11-2300/src
    cmsenv
    git cms-addpkg HLTrigger/Configuration
    scram build
    cd HLTrigger/Configuration/test
    
    hltGetConfiguration /users/amassiro/amassiro/V24 --full --offline --mc --unprescale --process TEST --globaltag auto:run2_mc_GRun --l1-emulator 'stage1,gt' --l1Xml L1Menu_Collisions2015_25ns_v2_L1T_Scales_20141121_Imp0_0x1030.xml  --input /store/relval/CMSSW_7_5_0_pre5/RelValZEE_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PU50ns_MCRUN2_75_V4-v1/00000/D0D29E17-E60B-E511-BEC5-0025905A6092.root > hlt_EleStream_MC.py
    hltGetConfiguration /users/amassiro/amassiro/V24 --full --offline --mc --unprescale --process TEST --globaltag auto:run2_mc_GRun  --input /store/relval/CMSSW_7_5_0_pre5/RelValZEE_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PU50ns_MCRUN2_75_V4-v1/00000/D0D29E17-E60B-E511-BEC5-0025905A6092.root > hlt_EleStream_MC.py
    cmsRun hlt_EleStream_MC.py &> tmp.txt
    
    in CMSSW_7_5_X_2015-06-11-2300
    hltGetConfiguration /users/amassiro/amassiro/V24 --full --offline --mc --unprescale --process TEST --globaltag auto:run2_mc_GRun  --input /store/relval/CMSSW_7_5_0_pre5/RelValZEE_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PU50ns_MCRUN2_75_V4-v1/00000/D0D29E17-E60B-E511-BEC5-0025905A6092.root > hlt_EleStream_MC_v24.py
    hltGetConfiguration /users/amassiro/amassiro/V26 --full --offline --mc --unprescale --process TEST --globaltag auto:run2_mc_GRun  --input /store/relval/CMSSW_7_5_0_pre5/RelValZEE_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PU50ns_MCRUN2_75_V4-v1/00000/D0D29E17-E60B-E511-BEC5-0025905A6092.root > hlt_EleStream_MC.py
    hltGetConfiguration /users/amassiro/amassiro/V26 --full --offline --mc --unprescale --process TEST --globaltag MCRUN2_75_V4       --input /store/relval/CMSSW_7_5_0_pre5/RelValZEE_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PU50ns_MCRUN2_75_V4-v1/00000/D0D29E17-E60B-E511-BEC5-0025905A6092.root > hlt_EleStream_MC.py
    hltGetConfiguration /users/amassiro/amassiro/V30 --full --offline --mc --unprescale --process TEST --globaltag MCRUN2_75_V4       --input /store/relval/CMSSW_7_5_0_pre5/RelValZEE_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PU50ns_MCRUN2_75_V4-v1/00000/D0D29E17-E60B-E511-BEC5-0025905A6092.root > hlt_EleStream_MC.py
    cmsRun hlt_EleStream_MC.py &> tmp.txt
    
    
    cmsrel CMSSW_7_5_X_2015-06-12-2300
    hltGetConfiguration /users/amassiro/amassiro/V26 --full --offline --mc --unprescale --process TEST --globaltag auto:run2_mc_GRun  --input /store/relval/CMSSW_7_5_0_pre5/RelValZEE_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PU50ns_MCRUN2_75_V4-v1/00000/D0D29E17-E60B-E511-BEC5-0025905A6092.root > hlt_EleStream_MC.py
    cmsRun hlt_EleStream_MC.py &> tmp.txt
    
    hltGetConfiguration /users/amassiro/amassiro/V27 --full --offline --mc --unprescale --process TEST --globaltag auto:run2_mc_GRun  --input /store/relval/CMSSW_7_5_0_pre5/RelValZEE_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PU50ns_MCRUN2_75_V4-v1/00000/D0D29E17-E60B-E511-BEC5-0025905A6092.root > hlt_EleStream_MC.py
    hltGetConfiguration /users/amassiro/amassiro/V28 --full --offline --mc --unprescale --process TEST --globaltag auto:run2_mc_GRun  --input /store/relval/CMSSW_7_5_0_pre5/RelValZEE_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PU50ns_MCRUN2_75_V4-v1/00000/D0D29E17-E60B-E511-BEC5-0025905A6092.root > hlt_EleStream_MC.py
    hltGetConfiguration /users/amassiro/amassiro/V29 --full --offline --mc --unprescale --process TEST --globaltag auto:run2_mc_GRun  --input /store/relval/CMSSW_7_5_0_pre5/RelValZEE_13/GEN-SIM-DIGI-RAW-HLTDEBUG/PU50ns_MCRUN2_75_V4-v1/00000/D0D29E17-E60B-E511-BEC5-0025905A6092.root > hlt_EleStream_MC.py
    cmsRun hlt_EleStream_MC.py &> tmp.txt
    
    
    fix from tracker:
    
    
        /afs/cern.ch/user/r/rovere/public/forMassironi/fix_crash.patch 
        diff --git a/CalibTracker/Records/interface/SiStripDependentRecords.h b/CalibTracker/Records/interface/SiStripDependentRecords.h
        index 6151db7..335a3dc 100644
        --- a/CalibTracker/Records/interface/SiStripDependentRecords.h
        +++ b/CalibTracker/Records/interface/SiStripDependentRecords.h
        @@ -17,7 +17,7 @@ class SiStripDetCablingRcd : public edm::eventsetup::DependentRecordImplementati
           boost::mpl::vector<SiStripFedCablingRcd,TrackerTopologyRcd,IdealGeometryRecord> > {};
         
         class SiStripRegionCablingRcd : public edm::eventsetup::DependentRecordImplementation<SiStripRegionCablingRcd,
        -  boost::mpl::vector<SiStripDetCablingRcd,TrackerDigiGeometryRecord,IdealGeometryRecord> > {};
        +  boost::mpl::vector<SiStripDetCablingRcd,TrackerDigiGeometryRecord,TrackerTopologyRcd> > {};
         
         // class SiStripGainRcd : public edm::eventsetup::DependentRecordImplementation<SiStripGainRcd, boost::mpl::vector<SiStripApvGainRcd> > {};
         class SiStripGainRcd : public edm::eventsetup::DependentRecordImplementation<SiStripGainRcd, boost::mpl::vector<SiStripApvGainRcd, SiStripApvGain2Rcd, SiStripApvGain3Rcd> > {};
        diff --git a/CalibTracker/SiStripESProducers/plugins/geom/SiStripRegionConnectivity.cc b/CalibTracker/SiStripESProducers/plugins/geom/SiStripRegionConnectivity.cc
        index a70bbc7..a9f5d25 100644
        --- a/CalibTracker/SiStripESProducers/plugins/geom/SiStripRegionConnectivity.cc
        +++ b/CalibTracker/SiStripESProducers/plugins/geom/SiStripRegionConnectivity.cc
        @@ -6,6 +6,7 @@
         #include "CalibTracker/Records/interface/SiStripDetCablingRcd.h"
         #include "CalibFormats/SiStripObjects/interface/SiStripDetCabling.h"
         #include "DataFormats/SiStripCommon/interface/SiStripConstants.h"
        +#include "Geometry/Records/interface/TrackerTopologyRcd.h"
                 
         using namespace sistrip;
         
        @@ -30,7 +31,7 @@ std::auto_ptr<SiStripRegionCabling> SiStripRegionConnectivity::produceRegionCabl
           iRecord.getRecord<TrackerDigiGeometryRecord>().get( tkgeom );
           
           edm::ESHandle<TrackerTopology> tTopoHandle;
        -  iRecord.getRecord<IdealGeometryRecord>().get(tTopoHandle);
                +  iRecord.getRecord<TrackerTopologyRcd>().get(tTopoHandle);
           const TrackerTopology* const tTopo = tTopoHandle.product();
         
 
    
    
    http://cmslxr.fnal.gov/lxr/source/Calibration/EcalAlCaRecoProducers/
    
    cmsrel CMSSW_7_4_3
    git cms-merge-topic 9256  ---> cp -r from a 75Xnigtbuilt release
    hltGetConfiguration /users/amassiro/amassiro/V26 --full --offline --mc --unprescale --process TEST --globaltag auto:run2_mc_GRun  --input /store/relval/CMSSW_7_4_1/RelValZEE_13/GEN-SIM-DIGI-RAW-HLTDEBUG/MCRUN2_74_V9_gensim71X-v1/00000/12C843BC-B6EC-E411-9C9E-0025905A60BC.root > hlt_EleStream_MC.py
    cmsRun hlt_EleStream_MC.py &> tmp.txt
    
    git cms-addpkg Calibration/EcalAlCaRecoProducers
    
    
    in CMSSW_7_4_2 it works
    hltGetConfiguration /users/amassiro/amassiro/V26 --full --offline --mc --unprescale --process TEST --globaltag MCRUN2_74_V9A      --input /store/relval/CMSSW_7_4_1/RelValZEE_13/GEN-SIM-DIGI-RAW-HLTDEBUG/MCRUN2_74_V9_gensim71X-v1/00000/12C843BC-B6EC-E411-9C9E-0025905A60BC.root > hlt_EleStream_MC.py
    hltGetConfiguration /users/amassiro/amassiro/V26 --full --offline --mc --unprescale --process TEST --globaltag auto:run2_mc_GRun  --input /store/relval/CMSSW_7_4_1/RelValZEE_13/GEN-SIM-DIGI-RAW-HLTDEBUG/MCRUN2_74_V9_gensim71X-v1/00000/12C843BC-B6EC-E411-9C9E-0025905A60BC.root > hlt_EleStream_MC.py
    hltGetConfiguration /users/amassiro/amassiro/V30 --full --offline --mc --unprescale --process TEST --globaltag auto:run2_mc_GRun  --input /store/relval/CMSSW_7_4_1/RelValZEE_13/GEN-SIM-DIGI-RAW-HLTDEBUG/MCRUN2_74_V9_gensim71X-v1/00000/12C843BC-B6EC-E411-9C9E-0025905A60BC.root > hlt_EleStream_MC.py
    hltGetConfiguration /users/amassiro/amassiro/V31 --full --offline --mc --unprescale --process TEST --globaltag auto:run2_mc_GRun  --input /store/relval/CMSSW_7_4_1/RelValZEE_13/GEN-SIM-DIGI-RAW-HLTDEBUG/MCRUN2_74_V9_gensim71X-v1/00000/12C843BC-B6EC-E411-9C9E-0025905A60BC.root > hlt_EleStream_MC.py
    cmsRun hlt_EleStream_MC.py &> tmp.txt
    
    hltGetConfiguration /users/amassiro/amassiro/V26 --full --offline --mc --unprescale --process TEST --globaltag auto:run2_mc_GRun  --input /store/relval/CMSSW_7_4_1/RelValZEE_13/GEN-SIM-DIGI-RAW-HLTDEBUG/MCRUN2_74_V9_gensim71X-v1/00000/12C843BC-B6EC-E411-9C9E-0025905A60BC.root  --l1-emulator 'stage1,gt' --l1Xml L1Menu_Collisions2015_25ns_v2_L1T_Scales_20141121_Imp0_0x1030.xml   > hlt_EleStream_MC_v26.py
    cmsRun hlt_EleStream_MC_v26.py &> tmp_v26.txt
    it does not work!
    
    
    CMSSW_7_4_X_2015-06-30-2300
    hltGetConfiguration /users/amassiro/amassiro/V31 --full --offline --mc --unprescale --process TEST --globaltag auto:run2_mc_GRun  --input /store/relval/CMSSW_7_4_1/RelValZEE_13/GEN-SIM-DIGI-RAW-HLTDEBUG/MCRUN2_74_V9_gensim71X-v1/00000/12C843BC-B6EC-E411-9C9E-0025905A60BC.root  --l1-emulator 'stage1,gt' --l1Xml L1Menu_Collisions2015_25ns_v2_L1T_Scales_20141121_Imp0_0x1030.xml   > hlt_EleStream_MC_v31.py
    cmsRun hlt_EleStream_MC_v31.py &> tmp_v31.txt
    

    CMSSW_7_4_6_patch3
    hltGetConfiguration /users/amassiro/amassiro/V31 --full --offline --mc --unprescale --process TEST --globaltag auto:run2_mc_GRun  --input /store/relval/CMSSW_7_4_1/RelValZEE_13/GEN-SIM-DIGI-RAW-HLTDEBUG/MCRUN2_74_V9_gensim71X-v1/00000/12C843BC-B6EC-E411-9C9E-0025905A60BC.root  --l1-emulator 'stage1,gt' --l1Xml L1Menu_Collisions2015_25ns_v2_L1T_Scales_20141121_Imp0_0x1030.xml   > hlt_EleStream_MC_v31.py
    cmsRun hlt_EleStream_MC_v31.py &> tmp_v31.txt
    
    CMSSW_7_4_X_2015-07-02-2300
    hltGetConfiguration /users/amassiro/amassiro/V31 --full --offline --mc --unprescale --process TEST --globaltag auto:run2_mc_GRun  --input /store/relval/CMSSW_7_4_1/RelValZEE_13/GEN-SIM-DIGI-RAW-HLTDEBUG/MCRUN2_74_V9_gensim71X-v1/00000/12C843BC-B6EC-E411-9C9E-0025905A60BC.root  --l1-emulator 'stage1,gt' --l1Xml L1Menu_Collisions2015_25ns_v2_L1T_Scales_20141121_Imp0_0x1030.xml   > hlt_EleStream_MC_v31.py
    cmsRun hlt_EleStream_MC_v31.py &> tmp_v31.txt
    hltGetConfiguration /users/amassiro/amassiro/V36 --full --offline --mc --unprescale --process TEST --globaltag auto:run2_mc_GRun  --input /store/relval/CMSSW_7_4_1/RelValZEE_13/GEN-SIM-DIGI-RAW-HLTDEBUG/MCRUN2_74_V9_gensim71X-v1/00000/12C843BC-B6EC-E411-9C9E-0025905A60BC.root  --l1-emulator 'stage1,gt' --l1Xml L1Menu_Collisions2015_25ns_v2_L1T_Scales_20141121_Imp0_0x1030.xml   > hlt_EleStream_MC_v36.py
    cmsRun hlt_EleStream_MC_v36.py &> tmp_v36.txt
    
    
    
    
test ReReco:
          
    cd /afs/cern.ch/user/a/amassiro/work/ECALHLT/CMSSW_7_1_0/src/StreamForElectrons/ReReco
    
    cmsRun step2_RAW2DIGI_L1Reco_RECO_onStream.py  \
        inputFiles=file:/afs/cern.ch/user/a/amassiro/work/ECALHLT/CMSSW_7_4_2/src/HLTrigger/Configuration/test/outputElectronStream_mc.root  \
        outputFile=reco_stream_test_HLT.root \
        isMC=True
        
    cmsRun step2_RAW2DIGI_L1Reco_RECO_onStream.py  \
        inputFiles=file:/afs/cern.ch/user/a/amassiro/work/ECALHLT/CMSSW_7_4_2/src/HLTrigger/Configuration/test/outputElectronStream_data.root  \
        outputFile=reco_stream_test_HLT.root \
        isMC=False
        
    cmsShow reco_stream_test_HLT.root  --no-version-check
        
    cmsRun step2_RAW2DIGI_L1Reco_RECO_onRaw.py  \
        inputFiles=/store/relval/CMSSW_7_4_1/RelValZEE_13/GEN-SIM-DIGI-RAW-HLTDEBUG/MCRUN2_74_V9_gensim71X-v1/00000/12C843BC-B6EC-E411-9C9E-0025905A60BC.root  \
        outputFile=reco_raw_test_HLT.root \
        isMC=True

        
     cd /afs/cern.ch/user/a/amassiro/work/ECALHLT/CMSSW_7_4_2/src/HLTrigger/Configuration
     cmsenv
     cd -
     cmsShow reco_raw_test_HLT.root  --no-version-check

                
        
Integration test:

    hltIntegrationTests /users/amassiro/amassiro/V8  -s /dev/CMSSW_7_4_0/HLT -i   root://xrootd.unl.edu//store/mc/Phys14DR/DYToEE_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/PU40bx25_tsg_castor_PHYS14_25_V1-v2/00000/06911F9F-9899-E411-B576-001E67396ACC.root  --mc -x "--globaltag auto:run2_mc_GRun"     -x "--l1-emulator stage1,gt" -x "--l1Xml L1Menu_Collisions2015_25ns_v2_L1T_Scales_20141121_Imp0_0x1030.xml"

    hltIntegrationTests /users/amassiro/amassiro/V12 -s /dev/CMSSW_7_4_0/HLT -i   root://xrootd.unl.edu//store/mc/Phys14DR/DYToEE_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/PU40bx25_tsg_castor_PHYS14_25_V1-v2/00000/06911F9F-9899-E411-B576-001E67396ACC.root  --mc -x "--globaltag auto:run2_mc_GRun"     -x "--l1-emulator stage1,gt" -x "--l1Xml L1Menu_Collisions2015_25ns_v2_L1T_Scales_20141121_Imp0_0x1030.xml"

    hltIntegrationTests /users/amassiro/amassiro/V13 -s /dev/CMSSW_7_4_0/HLT -i   root://xrootd.unl.edu//store/mc/Phys14DR/DYToEE_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/PU40bx25_tsg_castor_PHYS14_25_V1-v2/00000/06911F9F-9899-E411-B576-001E67396ACC.root  --mc -x "--globaltag auto:run2_mc_GRun"     -x "--l1-emulator stage1,gt" -x "--l1Xml L1Menu_Collisions2015_25ns_v2_L1T_Scales_20141121_Imp0_0x1030.xml"
    
    hltIntegrationTests /users/amassiro/amassiro/V16 -s /dev/CMSSW_7_4_0/HLT -i   root://xrootd.unl.edu//store/mc/Phys14DR/DYToEE_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/PU40bx25_tsg_castor_PHYS14_25_V1-v2/00000/06911F9F-9899-E411-B576-001E67396ACC.root  --mc -x "--globaltag auto:run2_mc_GRun"     -x "--l1-emulator stage1,gt" -x "--l1Xml L1Menu_Collisions2015_25ns_v2_L1T_Scales_20141121_Imp0_0x1030.xml"
    
    hltIntegrationTests /users/amassiro/amassiro/V19 -s /dev/CMSSW_7_4_0/HLT -i   root://xrootd.unl.edu//store/mc/Phys14DR/DYToEE_M-50_Tune4C_13TeV-pythia8/GEN-SIM-RAW/PU40bx25_tsg_castor_PHYS14_25_V1-v2/00000/06911F9F-9899-E411-B576-001E67396ACC.root  --mc -x "--globaltag auto:run2_mc_GRun"     -x "--l1-emulator stage1,gt" -x "--l1Xml L1Menu_Collisions2015_25ns_v2_L1T_Scales_20141121_Imp0_0x1030.xml"
    hltIntegrationTests /users/amassiro/amassiro/V19 -s /dev/CMSSW_7_4_0/HLT -i   /store/relval/CMSSW_7_4_1/RelValZEE_13/GEN-SIM-DIGI-RAW-HLTDEBUG/MCRUN2_74_V9_gensim71X-v1/00000/12C843BC-B6EC-E411-9C9E-0025905A60BC.root  --mc -x "--globaltag auto:run2_mc_GRun"     
 
    hltIntegrationTests /users/amassiro/amassiro/V20 -s /dev/CMSSW_7_4_0/HLT -i   /store/relval/CMSSW_7_4_1/RelValZEE_13/GEN-SIM-DIGI-RAW-HLTDEBUG/MCRUN2_74_V9_gensim71X-v1/00000/12C843BC-B6EC-E411-9C9E-0025905A60BC.root  --mc -x "--globaltag auto:run2_mc_GRun"     
    
    hltIntegrationTests /users/amassiro/amassiro/V21 -s /dev/CMSSW_7_4_0/HLT -i   /store/relval/CMSSW_7_4_1/RelValZEE_13/GEN-SIM-DIGI-RAW-HLTDEBUG/MCRUN2_74_V9_gensim71X-v1/00000/12C843BC-B6EC-E411-9C9E-0025905A60BC.root  --mc -x "--globaltag auto:run2_mc_GRun"     
 
    
Rate estimate:

    https://twiki.cern.ch/twiki/bin/viewauth/CMS/TmdRecipes
    
    hltGetConfiguration /users/amassiro/amassiro/V24 --full --offline --mc --unprescale --process TEST --globaltag auto:run2_mc_GRun --l1-emulator 'stage1,gt' --l1Xml L1Menu_Collisions2015_25ns_v2_L1T_Scales_20141121_Imp0_0x1030.xml   > hlt_EleStream_MC.py

    /QCD_Pt-20to30_EMEnriched_TuneCUETP8M1_13TeV_pythia8/RunIISpring15Digi74-AVE_30_BX_50ns_tsg_MCRUN2_74_V6-v1/GEN-SIM-RAW
    hltGetConfiguration /users/amassiro/amassiro/V24 --full --offline --mc --unprescale --process TEST --no-output --globaltag PHY1474_STV4 --l1-emulator 'gct,gt' --l1Xml L1Menu_Collisions2015_50nsGct_v1_L1T_Scales_20141121_Imp0_0x1030.xml \
        --input file:/tmp/amassiro/02150BCE-A4F2-E411-91CB-00259073E474.root \
        > EleStream_qcd_2030.py
    xrdcp root://xrootd.unl.edu//store/mc/RunIISpring15Digi74/QCD_Pt-30to50_EMEnriched_TuneCUETP8M1_13TeV_pythia8/GEN-SIM-RAW/AVE_30_BX_50ns_tsg_MCRUN2_74_V6-v1/00000/02150BCE-A4F2-E411-91CB-00259073E474.root /tmp/amassiro/
    cmsRun EleStream_qcd_2030.py  &> log.qcd2030.txt
    -> 0 rate
        
    /QCD_Pt-30to50_EMEnriched_TuneCUETP8M1_13TeV_pythia8/RunIISpring15Digi74-AVE_30_BX_50ns_tsg_MCRUN2_74_V6-v1/GEN-SIM-RAW
    hltGetConfiguration /users/amassiro/amassiro/V24 --full --offline --mc --unprescale --process TEST --no-output --globaltag PHY1474_STV4 --l1-emulator 'gct,gt' --l1Xml L1Menu_Collisions2015_50nsGct_v1_L1T_Scales_20141121_Imp0_0x1030.xml \
        --input file:/tmp/amassiro/04C31EAF-A0F2-E411-B61E-20CF305B0509.root \
        > EleStream_qcd_3050.py
    xrdcp root://xrootd.unl.edu//store/mc/RunIISpring15Digi74/QCD_Pt-30to50_EMEnriched_TuneCUETP8M1_13TeV_pythia8/GEN-SIM-RAW/AVE_30_BX_50ns_tsg_MCRUN2_74_V6-v1/00000/04C31EAF-A0F2-E411-B61E-20CF305B0509.root  /tmp/amassiro/

    xrdcp root://xrootd.unl.edu//store/mc/RunIISpring15Digi74/QCD_Pt-30to50_EMEnriched_TuneCUETP8M1_13TeV_pythia8/GEN-SIM-RAW/AVE_30_BX_50ns_tsg_MCRUN2_74_V6-v1/00000/02150BCE-A4F2-E411-91CB-00259073E474.root  /tmp/amassiro/
    
    xrdcp root://xrootd.unl.edu//store/mc/RunIISpring15Digi74/QCD_Pt-30to50_EMEnriched_TuneCUETP8M1_13TeV_pythia8/GEN-SIM-RAW/AVE_30_BX_50ns_tsg_MCRUN2_74_V6-v1/00000/04F1DC38-52F3-E411-8E61-842B2B185470.root  /tmp/amassiro/
    xrdcp root://xrootd.unl.edu//store/mc/RunIISpring15Digi74/QCD_Pt-30to50_EMEnriched_TuneCUETP8M1_13TeV_pythia8/GEN-SIM-RAW/AVE_30_BX_50ns_tsg_MCRUN2_74_V6-v1/00000/063007AF-43F3-E411-92D6-001E67397035.root  /tmp/amassiro/ 
    xrdcp root://xrootd.unl.edu//store/mc/RunIISpring15Digi74/QCD_Pt-30to50_EMEnriched_TuneCUETP8M1_13TeV_pythia8/GEN-SIM-RAW/AVE_30_BX_50ns_tsg_MCRUN2_74_V6-v1/00000/0AA9908F-52F3-E411-BF43-D067E5F91CAA.root  /tmp/amassiro/
    
    
    scp amassiro@lxplus0062.cern.ch:/tmp/amassiro/04C31EAF-A0F2-E411-B61E-20CF305B0509.root /data/user/amassiro/
    
    
    cmsRun EleStream_qcd_3050.py  &> log.qcd3050.txt    
    efficiency ~ 2. / 2474 = 8.1e-04
    
    efficiency ~ 4. / 7962 = 5.0e-04
    
    xsec < 185900000. * 0.06071 pb ~ 1.128599e+07 pb
    (the numbers are 30-80 GeV)
    
    events ~  2. / 2474 * 185900000. * 0.06071 pb ~ 9.123677e+03 pb ~ 9124 pb
    events ~  4. / 7962 * 185900000. * 0.06071 pb ~ 5.669927e+03 pb ~ 5670 pb
    30BX 50ns
    
    5e33 -> 5 Hz/nb
    -> 45 Hz    
    -> 28 +/- 10 Hz

    1.4e34 -> 14 Hz/nb
    -> 128 Hz
    
    
    /WJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring15Digi74-AVE_30_BX_50ns_tsg_MCRUN2_74_V6-v1/GEN-SIM-RAW
    hltGetConfiguration /users/amassiro/amassiro/V24 --full --offline --mc --unprescale --process TEST --no-output --globaltag PHY1474_STV4 --l1-emulator 'gct,gt' --l1Xml L1Menu_Collisions2015_50nsGct_v1_L1T_Scales_20141121_Imp0_0x1030.xml \
        --input file:/tmp/amassiro/04D2B5B1-3AF1-E411-BC4D-842B2B2B0EC5.root  \
        > EleStream_Wev.py
    xrdcp root://xrootd.unl.edu//store/mc/RunIISpring15Digi74/WJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/GEN-SIM-RAW/AVE_30_BX_50ns_tsg_MCRUN2_74_V6-v1/00000/04D2B5B1-3AF1-E411-BC4D-842B2B2B0EC5.root  /tmp/amassiro/
    cmsRun EleStream_Wev.py  &> log.Wev.txt    
    efficiency ~ 43. / 3085  (W>leptons) [AVE_30_BX_50ns]
    efficiency ~ 34. / 2395  (W>leptons) [AVE20BX25ns]

    events ~  34. / 2395  * 16000 * 3 pb ~ 681 pb
    30BX 50ns
    
    5e33 -> 5 Hz/nb    
    -> 3.4 +/- 0.6 Hz
    
    1.4e34 -> 14 Hz/nb
    -> 9.5 Hz

    
Time estimate:

    https://twiki.cern.ch/twiki/bin/viewauth/CMS/TriggerStudiesTiming
    
    ssh lxplus
    ssh vocms003
    
    ls /data/samples/NeutrinoGun_PU40bx25_L1_2015v2_CollisionsMenu_v2_Skim/ 
    
    
    cmsrel CMSSW_7_4_0  ---> no, because stream not yet there
    cd CMSSW_7_4_0/src
    cmsenv
    git cms-addpkg HLTrigger/Configuration
    scram build -j 4
    scramv1 b -j 8    
    cd HLTrigger/Configuration/test
    
    hltGetConfiguration /users/amassiro/amassiro/V24 --full --offline --mc --unprescale --process TEST --no-output --globaltag PHY1474_STV4 --l1-emulator 'gct,gt' --l1Xml L1Menu_Collisions2015_50nsGct_v1_L1T_Scales_20141121_Imp0_0x1030.xml \
        > EleStream_qcd_2030.py
 
    ls  /data/samples/NeutrinoGun_PU40bx25_L1_2015v2_CollisionsMenu_v2_Skim/  | awk '{print "@file:/data/samples/NeutrinoGun_PU40bx25_L1_2015v2_CollisionsMenu_v2_Skim/"$1"@,"}' | tr "@" "'"

    cmsRun cmsRun EleStream_qcd_2030.py
    
    
    cd /afs/cern.ch/user/a/amassiro/work/ECALHLT/CMSSW_7_4_2/src/HLTrigger/Configuration/test
    cmsenv
    cmsRun EleStream_timing.py
    
    
    timing results:
    -> 38 ms for the new path
    see 
        r99t DQM_V0001_R000000001__HLT__FastTimerService__All.root
    
    

    
Commit to cmssw:

    instructions: http://cms-sw.github.io/faq.html

    cmsrel CMSSW_X_X_X
    cd  CMSSW_X_X_X/src
    cmsenv
    git cms-init
    git cms-addpkg Calibration/EcalAlCaRecoProducers
    
    git remote show  ::: show which are the remote of your repository. origin (offcial-cmssw) and my fork (my-cmssw)
    
    modify code
    
    git fetch --all   
    git push my-cmssw remotes/official-cmssw/CMSSW_7_5_X:refs/heads/CMSSW_7_5_X   
       ---> push on my-cmssw what is in the official
    
    ----> create my new branch
    git checkout -b elestream_75X

        git status
        # On branch elestream_75X


    git fetch official-cmssw
    git merge official-cmssw/CMSSW_7_5_X
    
    ---> commit on my branch
    git commit -m "adding edm::stream::EDProducer friend for multithread" Calibration/EcalAlCaRecoProducers/plugins/SelectedElectronFEDListProducer.h
    
    ----> push on my branch
         push   origin      branch
    git push my-cmssw elestream_75X
    
    ---> pull request
    on
    https://github.com/amassiro/cmssw/tree/elestream_75X
    click on "pull requests"
    "new pull request"
    
 
    
    
    
    
    
    
    
    git cms-addpkg Calibration/EcalAlCaRecoProducers
    git checkout elestream_75X
    
    
    
    
    ---- porting in 74X
    
    git cherry-pick 7c1834f17707b8b54742c346d4d57944b005aff5  ----> no
    
    
    git fetch --all   
    
    git push my-cmssw remotes/official-cmssw/CMSSW_7_4_X:refs/heads/CMSSW_7_4_X   
    
    git checkout -b elestream_74X
    
    git fetch official-cmssw
    git merge official-cmssw/CMSSW_7_4_X

    git commit -m "adding edm::stream::EDProducer friend for multithread" plugins/SelectedElectronFEDListProducer.cc plugins/SelectedElectronFEDListProducer.h 

    git push my-cmssw elestream_74X
    
    https://github.com/amassiro/cmssw/tree/elestream_74X
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    75X
    
    /afs/cern.ch/work/a/amassiro/ECALHLT/CMSSW_7_5_X_2015-06-24-2300/src
    
    git cms-merge-topic  9717
    
    git checkout CMSSW_7_5_X
    
    git branch -vv

    git rebase -i official-cmssw/CMSSW_7_5_X
     
    git log 
    
    git log -p
    
    
    
    
    74X
    git cms-merge-topic  9720
     
     
     
     