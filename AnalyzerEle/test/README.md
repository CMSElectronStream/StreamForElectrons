Simple Electron analyzer
==================

Simple dumper of electron information:

On simple raw:

    cmsRun test.py \
        inputFiles=file:/home/amassiro/ECAL/HLT/CMSSW_6_2_2/src/StreamForElectrons/ReReco/step2_RAW2DIGI_L1Reco_RECO_onSimpleRaw.root


On streamed electrons:

    cmsRun test.py \
        inputFiles=file:/home/amassiro/ECAL/HLT/CMSSW_6_2_2/src/StreamForElectrons/ReReco/step2_RAW2DIGI_L1Reco_RECO.root




//---- error ----

 >> AM << EgammaHadTower mode = 0
%MSG-e FatalSystemSignal:  ElectronSeedProducer:ecalDrivenElectronSeeds  13-Mar-2014 14:51:46 CET Run: 194533 Event: 848181861
A fatal system signal has occurred: segmentation violation
%MSG


A fatal system signal has occurred: segmentation violation
The following is the call stack containing the origin of the signal.
NOTE:The first few functions on the stack are artifacts of processing the signal and can be ignored

#0  0x0000003776a9a075 in waitpid () from /lib64/libc.so.6
#1  0x0000003776a3c741 in do_system () from /lib64/libc.so.6
#2  0x00002b0bc6b1244c in TUnixSystem::StackTrace() () from /afs/cern.ch/cms/slc5_amd64_gcc472/cms/cmssw/CMSSW_6_2_2/external/slc5_amd64_gcc472/lib/libCore.so
#3  0x00002b0bc9d34854 in sig_dostack_then_abort () from /afs/cern.ch/cms/slc5_amd64_gcc472/cms/cmssw/CMSSW_6_2_2/lib/slc5_amd64_gcc472/libFWCoreServices.so
#4  <signal handler called>
#5  0x00002b0bee5e6a52 in EgammaTowerIsolation::EgammaTowerIsolation(float, float, float, int, edm::SortedCollection<CaloTower, edm::StrictWeakOrdering<CaloTower> > const*) () from /home/amassiro/ECAL/HLT/CMSSW_6_2_2/lib/slc5_amd64_gcc472/libRecoEgammaEgammaIsolationAlgos.so
#6  0x00002b0bee5793b5 in ElectronHcalHelper::readEvent(edm::Event const&) () from /home/amassiro/ECAL/HLT/CMSSW_6_2_2/lib/slc5_amd64_gcc472/libRecoEgammaEgammaElectronAlgos.so
#7  0x00002b0bee4faa54 in ElectronSeedProducer::produce(edm::Event&, edm::EventSetup const&) () from /home/amassiro/ECAL/HLT/CMSSW_6_2_2/lib/slc5_amd64_gcc472/pluginRecoEgammaEgammaElectronProducersPlugins.so
#8  0x00002b0bc54fbee0 in edm::EDProducer::doEvent(edm::EventPrincipal&, edm::EventSetup const&, edm::CurrentProcessingContext const*) () from /afs/cern.ch/cms/slc5_amd64_gcc472