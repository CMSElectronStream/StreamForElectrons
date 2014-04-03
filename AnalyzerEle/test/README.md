Simple Electron analyzer
==================

Simple dumper of electron information:

On simple raw:

    cmsRun AnalyzerEle_cfg.py \
        inputFiles=file:../../ReReco/raw2reco.root \
        outputFile=tree_raw.root

On streamed electrons:

    cmsRun AnalyzerEle_cfg.py \
        inputFiles=file:../../ReReco/stream2reco.root  \
        outputFile=tree_stream.root





