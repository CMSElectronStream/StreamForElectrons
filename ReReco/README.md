Test Reco on top of raw
=========================

Original reco configuration file:

    step2_RAW2DIGI_L1Reco_RECO.py

Important changes to run on stream:

 * "rawDataCollector" to be moved to "HLTselectedElectronFEDList:StremElectronRawFedData" everywhere

Dump config file into the complete one:

    edmConfigDump step2_RAW2DIGI_L1Reco_RECO.py  >  dump_step2_RAW2DIGI_L1Reco_RECO.py

Prepare to run on simple raw:

    sed 's:MYINPUTFILE:/eos/cms/store/user/rgerosa/ElectronStreamStudy/streamElectronRAW_ALL.root:' \
       < dump_step2_RAW2DIGI_L1Reco_RECO.py |
       sed 's|MYOUTPUTFILE|raw2reco.root|' \
        > dump_step2_RAW2DIGI_L1Reco_RECO_onRaw.py


Prepare to run on stream:

    sed 's:MYINPUTFILE:/eos/cms/store/user/rgerosa/ElectronStreamStudy/streamElectronRAW_STREAM.root:' \
       < dump_step2_RAW2DIGI_L1Reco_RECO.py |
       sed 's|rawDataCollector|HLTselectedElectronFEDList:StremElectronRawFedData|' | \
       sed 's|MYOUTPUTFILE|stream2reco.root|' \
        > dump_step2_RAW2DIGI_L1Reco_RECO_onStream.py


# Patch the Reco
=======

src/RecoEgamma/EgammaIsolationAlgos/src/

    cmsenv
    git cms-addpkg RecoEgamma/EgammaIsolationAlgos
    cp StreamForElectrons/ReReco/patch/EgammaTowerIsolation.cc  src/RecoEgamma/EgammaIsolationAlgos/src/ 
    scramv1 b -j 8


# Run
=======

to run on raw:

    cmsRun dump_step2_RAW2DIGI_L1Reco_RECO_onRaw.py

to run on stream:

    cmsRun dump_step2_RAW2DIGI_L1Reco_RECO_onStream.py





# Backup
=======

Run simple raw2reco

    cmsRun test_rereco_cfg.py
    (to be fixed)


It works:

    cmsRun step2_RAW2DIGI_L1Reco_RECO.py

After dumping the config file:

    cmsRun dump_step2_RAW2DIGI_L1Reco_RECO.py



Filter a file (keep only the first 100 events):

    cmsRun filter.py outputFile=pickevents.root \
      numberOfEventsToProcess=100 \
      inputFiles=file:/data/amassiro/CMSSWRoot/DATA2012/SingleElectron_Run2012B_RAW/B865DABE-BDA2-E111-854F-BCAEC53296F7.root


Run the stream:

    cd ../../HLTrigger/Configuration/test
    cmsRun streamEle_cfg.py

Reco the stream:

    cd -
    cd StreamForElectrons/ReReco
    cmsRun dump_step2_RAW2DIGI_L1Reco_RECO.py
    cmsRun dump_step2_RAW2DIGI_L1Reco_RECO_onSkim.py     (from scratch!)


Reco the simple RAW file:

    cmsRun dump_step2_RAW2DIGI_L1Reco_RECO_onSimpleRaw.py



Where:

    cmsneu
    /home/amassiro/ECAL/HLT
    /home/amassiro/ECAL/HLT/CMSSW_6_2_2/src/StreamForElectrons/ReReco/





# raw2digi

    cmsDriver.py step2 -s RAW2DIGI --data --datatier RECO --eventcontent RECO --conditions GR10_P_V11::All --scenario pp --no_exec --magField AutoFromDBCurrent --process reRECO

replace:

    rawDataCollector

into:

    HLTselectedElectronFEDList:StremElectronRawFedData



