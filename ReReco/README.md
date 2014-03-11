Test Reco on top of raw
=========================

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



