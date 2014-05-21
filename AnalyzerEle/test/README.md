Simple Electron analyzer
==================

Simple dumper of electron information:

On simple raw:

    cmsRun AnalyzerEle_cfg.py \
        inputFiles=file:../../ReReco/raw2reco.root \
        outputFile=tree_raw.root

    cmsRun AnalyzerEle_cfg.py \
        inputFiles=file:../../ReReco/reco_raw.root \
        outputFile=tree_raw.root

On streamed electrons:

    cmsRun AnalyzerEle_cfg.py \
        inputFiles=file:../../ReReco/stream2reco.root  \
        outputFile=tree_stream.root

    cmsRun AnalyzerEle_cfg.py \
        inputFiles=file:../../ReReco/reco_stream_highpu.root  \
        outputFile=tree_stream_highpu.root




Variables comparison
====

Compile:

    g++ -Wall -o EventComparison.exe `root-config --glibs --libs --cflags` EventComparison.cpp -lTreePlayer

Run:

    EventComparison.exe    /data/amassiro/ECAL/EleStream/AnalyzerEle_AlcaElectronStream_noSelection.root    /data/amassiro/ECAL/EleStream/AnalyzerEle_streamA_noSelection.root







