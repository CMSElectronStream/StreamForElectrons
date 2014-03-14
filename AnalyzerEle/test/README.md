Simple Electron analyzer
==================

Simple dumper of electron information:

On simple raw:

    cmsRun test.py \
        inputFiles=file:../../ReReco/raw2reco.root \
        options.outputFile=tree_raw.root

On streamed electrons:

    cmsRun test.py \
        inputFiles=file:../../ReReco/stream2reco.root  \
        options.outputFile=tree_stream.root





