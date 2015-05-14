##########################################
### Package useful to run the Analyzer ###
##########################################

git-cms-addpkg EgammaAnalysis/ElectronTools
cd EgammaAnalysis/ElectronTools/data/ 
cat download.url | grep '.root' | xargs wget 
git-cms-addpkg RecoEcal/EgammaCoreTools
scramv1 b -j 4


##### In this commit if you run on stream output you are using the same reco collection for general event info and not the trigger ones... change them by hand