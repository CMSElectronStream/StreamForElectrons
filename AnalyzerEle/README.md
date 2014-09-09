##########################################
### Package useful to run the Analyzer ###
##########################################

git-cms-addpkg EgammaAnalysis/ElectronTools
cd EgammaAnalysis/ElectronTools/data/ 
cat download.url | grep '.root' | xargs wget 
git-cms-addpkg RecoEcal/EgammaCoreTools
scramv1 b -j 4
