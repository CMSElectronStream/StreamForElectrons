##########################################
### Package useful to run the Analyzer ###
##########################################

git clone https://github.com/cms-analysis/EgammaAnalysis-ElectronTools EgammaAnalysis/ElectronTools ;
cd EgammaAnalysis/ElectronTools ;
git checkout EgammaAnalysis-ElectronTools-MD-21Apr2013-test-2 ;
cd - ;
git clone https://github.com/cms-cvs-history/RecoEgamma-EgammaTools RecoEgamma/EgammaTools ;

scramv1 b -j 4