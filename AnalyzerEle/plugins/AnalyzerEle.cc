#include "StreamForElectrons/AnalyzerEle/plugins/AnalyzerEle.h"

AnalyzerEle::AnalyzerEle(const edm::ParameterSet& iConfig){

 std::cout << ">>> AnalyzerEle::AnalyzerEle begin <<<" << std::endl;

 edm::Service<TFileService> fs ;
 myTree_ = fs -> make <TTree>("AnalyzerEle","AnalyzerEle"); // create the output file with the output tree

 eventType_           = iConfig.getUntrackedParameter<int>("eventType",1);
 dataRun_             = iConfig.getParameter<std::string>("dataRun");
 digiCollection_EB_   = iConfig.getParameter<edm::InputTag>("digiCollection_EB");
 digiCollection_EE_   = iConfig.getParameter<edm::InputTag>("digiCollection_EE");
 recHitCollection_EB_ = iConfig.getParameter<edm::InputTag>("recHitCollection_EB");
 recHitCollection_EE_ = iConfig.getParameter<edm::InputTag>("recHitCollection_EE");
 BSTag_               = iConfig.getParameter<edm::InputTag>("theBeamSpotTag");
 SRFlagCollection_EB_ = iConfig.getParameter<edm::InputTag>("SRFlagCollection_EB");
 SRFlagCollection_EE_ = iConfig.getParameter<edm::InputTag>("SRFlagCollection_EE");
 PVTag_               = iConfig.getParameter<edm::InputTag>("PVTag");
 rhoTag_              = iConfig.getParameter<edm::InputTag>("rhoTag");
 EleTag_              = iConfig.getParameter<edm::InputTag>("EleTag");
 PFMetTag_            = iConfig.getParameter<edm::InputTag>("PFMetTag");
 conversionsInputTag_ = iConfig.getParameter<edm::InputTag>("conversionsInputTag");
 MCtruthTag_          = iConfig.getParameter<edm::InputTag>("MCtruthTag");
  
 triggerResultsCollection_ = iConfig.getParameter<edm::InputTag>("triggerResultsCollection");
 hltPaths_                 = iConfig.getParameter<std::vector<std::string> >("hltPaths") ;

 verbosity_          = iConfig.getUntrackedParameter<bool>("verbosity", false);
 doWZSelection_      = iConfig.getUntrackedParameter<bool>("doWZSelection", false);
 applyCorrections_   = iConfig.getUntrackedParameter<bool>("applyCorrections",false);
 dataFlag_           = iConfig.getUntrackedParameter<bool>("dataFlag", true);
 saveRecHitMatrix_   = iConfig.getUntrackedParameter<bool>("saveRecHitMatrix",false);
 saveFbrem_          = iConfig.getUntrackedParameter<bool>("saveFbrem", false);
 saveMCInfo_         = iConfig.getUntrackedParameter<bool>("saveMCInfo", false);

 eventNaiveId_ = 0;

 std::cout << ">>>>>> AnalyzerEle::AnalyzerEle::set event branches <<<" << std::endl;

 myTree_ -> Branch("bxId", &bxId_, "bxId/L");
 myTree_ -> Branch("eventId", &eventId_, "eventId/L");
 myTree_ -> Branch("lumiId", &lumiId_, "lumiId/I");
 myTree_ -> Branch("runId", &runId_, "runId/I");
 myTree_ -> Branch("timeStampHigh",&timeStampHigh_,"timeStampHigh/I");
 myTree_ -> Branch("isW", &isW_, "isW/I");
 myTree_ -> Branch("isZ", &isZ_, "isZ/I");

 //---- event variables
 myTree_ -> Branch("PV_n", &PV_n_, "PV_n/I");
 myTree_ -> Branch("PV_z", &PV_z_, "PV_z/F");
 myTree_ -> Branch("PV_d0", &PV_d0_, "PV_d0/F");
 myTree_ -> Branch("rho", &rho_, "rho/F");
 myTree_ -> Branch("nele", &nele_, "nele/I");

 if(saveMCInfo_){

  std::cout<< ">>>>>> SimpleNtupleEoverP::SimpleNtupleEoverP::set MC branches <<<" << std::endl;
  myTree_ -> Branch("nPU",&nPU_,"nPU/F");

  myTree_ -> Branch("mcV_E", &mcV_E, "mcV_E/F");
  myTree_ -> Branch("mcV_Px", &mcV_Px, "mcV_Px/F");
  myTree_ -> Branch("mcV_Py", &mcV_Py, "mcV_Py/F");
  myTree_ -> Branch("mcV_Pz", &mcV_Pz, "mcV_Pz/F");
  myTree_ -> Branch("mcV_Eta", &mcV_Eta, "mcV_Eta/F");
  myTree_ -> Branch("mcV_Phi", &mcV_Phi, "mcV_Phi/F");
  myTree_ -> Branch("mcV_M", &mcV_M, "mcV_M/F");
  myTree_ -> Branch("mcV_Charge",&mcV_Charge, "mcV_Charge/I");
  myTree_ -> Branch("mcV_PdgId", &mcV_PdgId, "mcV_PdgId/I");

  myTree_ -> Branch("mcF1_fromV_E", &mcF1_fromV_E, "mcF1_fromV_E/F");
  myTree_ -> Branch("mcF1_fromV_Px", &mcF1_fromV_Px, "mcF1_fromV_Px/F");
  myTree_ -> Branch("mcF1_fromV_Py", &mcF1_fromV_Py, "mcF1_fromV_Py/F");
  myTree_ -> Branch("mcF1_fromV_Pz", &mcF1_fromV_Pz, "mcF1_fromV_Pz/F");
  myTree_ -> Branch("mcF1_fromV_Eta", &mcF1_fromV_Eta, "mcF1_fromV_Eta/F");
  myTree_ -> Branch("mcF1_fromV_Phi", &mcF1_fromV_Phi, "mcF1_fromV_Phi/F");
  myTree_ -> Branch("mcF1_fromV_M", &mcF1_fromV_M, "mcF1_fromV_M/F");
  myTree_ -> Branch("mcF1_fromV_Charge",&mcF1_fromV_Charge,"mcF1_fromV_Charge/I");
  myTree_ -> Branch("mcF1_fromV_PdgId", &mcF1_fromV_PdgId, "mcF1_fromV_PdgId/I");
    
  myTree_ -> Branch("mcF2_fromV_E", &mcF2_fromV_E, "mcF2_fromV_E/F");
  myTree_ -> Branch("mcF2_fromV_Px", &mcF2_fromV_Px, "mcF2_fromV_Px/F");
  myTree_ -> Branch("mcF2_fromV_Py", &mcF2_fromV_Py, "mcF2_fromV_Py/F");
  myTree_ -> Branch("mcF2_fromV_Pz", &mcF2_fromV_Pz, "mcF2_fromV_Pz/F");
  myTree_ -> Branch("mcF2_fromV_Eta", &mcF2_fromV_Eta, "mcF2_fromV_Eta/F");
  myTree_ -> Branch("mcF2_fromV_Phi", &mcF2_fromV_Phi, "mcF2_fromV_Phi/F");
  myTree_ -> Branch("mcF2_fromV_M", &mcF2_fromV_M, "mcF2_fromV_M/F");
  myTree_ -> Branch("mcF2_fromV_Charge",&mcF2_fromV_Charge,"mcF2_fromV_Charge/I");
  myTree_ -> Branch("mcF2_fromV_PdgId", &mcF2_fromV_PdgId, "mcF2_fromV_PdgId/I");
 }
 
 // ele1 variables
 std::cout << ">>>>>> AnalyzerEle::AnalyzerEle::set ele1 branches <<<" << std::endl;

 myTree_ -> Branch("ele1_charge",&ele1_charge,"ele1_charge/F");
 myTree_ -> Branch("ele1_p", &ele1_p, "ele1_p/F");
 myTree_ -> Branch("ele1_pt", &ele1_pt, "ele1_pt/F");
 myTree_ -> Branch("ele1_eta", &ele1_eta, "ele1_eta/F");
 myTree_ -> Branch("ele1_phi", &ele1_phi, "ele1_phi/F");
  
 myTree_ -> Branch("ele1_isEB", &ele1_isEB, "ele1_isEB/I");
 myTree_ -> Branch("ele1_isEBEEGap", &ele1_isEBEEGap, "ele1_isEBEEGap/I");
 myTree_ -> Branch("ele1_isEBEtaGap", &ele1_isEBEtaGap, "ele1_isEBEtaGap/I");
 myTree_ -> Branch("ele1_isEBPhiGap", &ele1_isEBPhiGap, "ele1_isEBPhiGap/I");
 myTree_ -> Branch("ele1_isEEDeeGap", &ele1_isEEDeeGap, "ele1_isEEDeeGap/I");
 myTree_ -> Branch("ele1_isEERingGap",&ele1_isEERingGap,"ele1_isEERingGap/I");

 myTree_ -> Branch("ele1_idtype", &ele1_idtype, "ele1_idtype/I");
  
 myTree_ -> Branch("ele1_isTrackerDriven",&ele1_isTrackerDriven,"ele1_isTrackerDriven/I");
 myTree_ -> Branch("ele1_classification",&ele1_classification,"ele1_classification/I");
  
 myTree_ -> Branch("ele1_sigmaIetaIeta", &ele1_sigmaIetaIeta, "ele1_sigmaIetaIeta/F");
 myTree_ -> Branch("ele1_DphiIn", &ele1_DphiIn, "ele1_DphiIn/F");
 myTree_ -> Branch("ele1_DetaIn", &ele1_DetaIn, "ele1_DetaIn/F");

 myTree_ -> Branch("ele1_deltaEtaEleClusterTrackAtCalo", &ele1_deltaEtaEleClusterTrackAtCalo, "ele1_deltaEtaEleClusterTrackAtCalo/F");
 myTree_ -> Branch("ele1_deltaEtaPhiClusterTrackAtCalo", &ele1_deltaEtaPhiClusterTrackAtCalo, "ele1_deltaEtaPhiClusterTrackAtCalo/F");
 myTree_ -> Branch("ele1_deltaEtaSeedClusterTrackAtCalo", &ele1_deltaEtaSeedClusterTrackAtCalo, "ele1_deltaEtaSeedClusterTrackAtCalo/F");
 myTree_ -> Branch("ele1_deltaPhiSeedClusterTrackAtCalo", &ele1_deltaPhiSeedClusterTrackAtCalo, "ele1_deltaPhiSeedClusterTrackAtCalo/F");

 myTree_ -> Branch("ele1_HOverE", &ele1_HOverE, "ele1_HOverE/F");
 myTree_ -> Branch("ele1_tkIso", &ele1_tkIso, "ele1_tkIso/F");
 myTree_ -> Branch("ele1_emIso", &ele1_emIso, "ele1_emIso/F");
 myTree_ -> Branch("ele1_hadIso", &ele1_hadIso, "ele1_hadIso/F");
  
 myTree_ -> Branch("ele1_scERaw", &ele1_scERaw, "ele1_scERaw/F");
 myTree_ -> Branch("ele1_scEtRaw", &ele1_scEtRaw, "ele1_scEtRaw/F");
 myTree_ -> Branch("ele1_scE", &ele1_scE, "ele1_scE/F");
 myTree_ -> Branch("ele1_scEt", &ele1_scEt, "ele1_scEt/F");
 myTree_ -> Branch("ele1_scERaw_PUcleaned", &ele1_scERaw_PUcleaned, "ele1_scERaw_PUcleaned/F");
 myTree_ -> Branch("ele1_es", &ele1_es, "ele1_es/F");
 myTree_ -> Branch("ele1_scLaserCorr", &ele1_scLaserCorr, "ele1_scLaserCorr/F");
 myTree_ -> Branch("ele1_scCrackCorr", &ele1_scCrackCorr, "ele1_scCrackCorr/F");
 myTree_ -> Branch("ele1_scLocalContCorr", &ele1_scLocalContCorr, "ele1_scLocalContCorr/F");
  
 myTree_ -> Branch("ele1_scEta", &ele1_scEta, "ele1_scEta/F");
 myTree_ -> Branch("ele1_scPhi", &ele1_scPhi, "ele1_scPhi/F");
 myTree_ -> Branch("ele1_scLocalEta", &ele1_scLocalEta, "ele1_scLocalEta/F");
 myTree_ -> Branch("ele1_scLocalPhi", &ele1_scLocalPhi, "ele1_scLocalPhi/F");
 myTree_ -> Branch("ele1_scEtaWidth", &ele1_scEtaWidth, "ele1_scEtaWidth/F");
 myTree_ -> Branch("ele1_scPhiWidth", &ele1_scPhiWidth, "ele1_scPhiWidth/F");
 myTree_ -> Branch("ele1_scEtaWidth_PUcleaned", &ele1_scEtaWidth_PUcleaned, "ele1_scEtaWidth_PUcleaned/F");
 myTree_ -> Branch("ele1_scPhiWidth_PUcleaned", &ele1_scPhiWidth_PUcleaned, "ele1_scPhiWidth_PUcleaned/F");
 myTree_ -> Branch("ele1_fCorrection_PUcleaned", &ele1_fCorrection_PUcleaned, "ele1_fCorrection_PUcleaned/F");
  
 myTree_ -> Branch("ele1_fEta", &ele1_fEta, "ele1_fEta/F");
 myTree_ -> Branch("ele1_fEtaCorr", &ele1_fEtaCorr, "ele1_fEtaCorr/F");
 myTree_ -> Branch("ele1_tkP", &ele1_tkP, "ele1_tkP/F");
 myTree_ -> Branch("ele1_tkPt", &ele1_tkPt, "ele1_tkPt/F");
 myTree_ -> Branch("ele1_fbrem", &ele1_fbrem, "ele1_fbrem/F");
  
 myTree_ -> Branch("ele1_dxy_PV", &ele1_dxy_PV, "ele1_dxy_PV/F");
 myTree_ -> Branch("ele1_dz_PV", &ele1_dz_PV, "ele1_dz_PV/F");
 myTree_ -> Branch("ele1_sigmaP", &ele1_sigmaP, "ele1_sigmaP/F");
  
 myTree_ -> Branch("ele1_eSeedBC", &ele1_eSeedBC, "ele1_eSeedBC/F");
 myTree_ -> Branch("ele1_e5x5", &ele1_e5x5, "ele1_e5x5/F");
 myTree_ -> Branch("ele1_e3x3", &ele1_e3x3, "ele1_e3x3/F");
 myTree_ -> Branch("ele1_scNxtal", &ele1_scNxtal, "ele1_scNxtal/F");
 myTree_ -> Branch("ele1_bcN", &ele1_bcN, "ele1_bcN/I");
 myTree_ -> Branch("ele1_5x5LaserCorr", &ele1_5x5LaserCorr, "ele1_5x5LaserCorr/F");
 myTree_ -> Branch("ele1_3x3LaserCorr", &ele1_3x3LaserCorr, "ele1_3x3LaserCorr/F");
  
 myTree_ -> Branch("ele1_seedE", &ele1_seedE, "ele1_seedE/F");
 myTree_ -> Branch("ele1_seedLaserAlpha", &ele1_seedLaserAlpha, "ele1_seedLaserAlpha/F");
 myTree_ -> Branch("ele1_seedLaserCorr", &ele1_seedLaserCorr, "ele1_seedLaserCorr/F");
 myTree_ -> Branch("ele1_seedICConstant", &ele1_seedICConstant, "ele1_seedICConstant/F");
 myTree_ -> Branch("ele1_seedIeta", &ele1_seedIeta, "ele1_seedIeta/I");
 myTree_ -> Branch("ele1_seedIphi", &ele1_seedIphi, "ele1_seedIphi/I");
 myTree_ -> Branch("ele1_seedIx", &ele1_seedIx, "ele1_seedIx/I");
 myTree_ -> Branch("ele1_seedIy", &ele1_seedIy, "ele1_seedIy/I");
 myTree_ -> Branch("ele1_seedZside", &ele1_seedZside, "ele1_seedZside/I");
 myTree_ -> Branch("ele1_EOverP", &ele1_EOverP, "ele1_EOverP/F");
  
 myTree_ -> Branch("ele1_nRecHits",&ele1_nRecHits,"ele1_nRecHits/I");
 myTree_ -> Branch("ele1_recHit_E", "std::vector<float>",&ele1_recHit_E);
 myTree_ -> Branch("ele1_recHit_flag", "std::vector<int>", &ele1_recHit_flag);
 myTree_ -> Branch("ele1_recHit_hashedIndex", "std::vector<int>", &ele1_recHit_hashedIndex);
 myTree_ -> Branch("ele1_recHit_ietaORix", "std::vector<int>", &ele1_recHit_ietaORix);
 myTree_ -> Branch("ele1_recHit_iphiORiy", "std::vector<int>", &ele1_recHit_iphiORiy);
 myTree_ -> Branch("ele1_recHit_zside", "std::vector<int>", &ele1_recHit_zside);
 myTree_ -> Branch("ele1_recHit_laserCorrection","std::vector<float>",&ele1_recHit_laserCorrection);
 myTree_ -> Branch("ele1_recHit_Alpha", "std::vector<float>",&ele1_recHit_Alpha);
 myTree_ -> Branch("ele1_recHit_ICConstant", "std::vector<float>",&ele1_recHit_ICConstant);

 if(saveRecHitMatrix_) {
  myTree_ -> Branch("ele1_recHitMatrix_E", "std::vector<float>",&ele1_recHitMatrix_E);
  myTree_ -> Branch("ele1_recHitMatrix_flag", "std::vector<int>", &ele1_recHitMatrix_flag);
  myTree_ -> Branch("ele1_recHitMatrix_hashedIndex", "std::vector<int>", &ele1_recHitMatrix_hashedIndex);
  myTree_ -> Branch("ele1_recHitMatrix_ietaORix", "std::vector<int>", &ele1_recHitMatrix_ietaORix);
  myTree_ -> Branch("ele1_recHitMatrix_iphiORiy", "std::vector<int>", &ele1_recHitMatrix_iphiORiy);
  myTree_ -> Branch("ele1_recHitMatrix_zside", "std::vector<int>", &ele1_recHitMatrix_zside);
  myTree_ -> Branch("ele1_recHitMatrix_laserCorrection","std::vector<float>",&ele1_recHitMatrix_laserCorrection);
  myTree_ -> Branch("ele1_recHitMatrix_ICConstant", "std::vector<float>",&ele1_recHitMatrix_ICConstant);
  myTree_ -> Branch("ele1_recHitMatrix_samples", "std::vector<float>",&ele1_recHitMatrix_samples);
 }


 // ele2 variables
 std::cout << ">>>>>> AnalyzerEle::AnalyzerEle::set ele2 branches <<<" << std::endl;

 myTree_ -> Branch("ele2_charge",&ele2_charge,"ele2_charge/F");
 myTree_ -> Branch("ele2_p", &ele2_p, "ele2_p/F");
 myTree_ -> Branch("ele2_pt", &ele2_pt, "ele2_pt/F");
 myTree_ -> Branch("ele2_eta", &ele2_eta, "ele2_eta/F");
 myTree_ -> Branch("ele2_phi", &ele2_phi, "ele2_phi/F");

 myTree_ -> Branch("ele2_idtype", &ele2_idtype, "ele2_idtype/I");
  
 myTree_ -> Branch("ele2_isEB", &ele2_isEB, "ele2_isEB/I");
 myTree_ -> Branch("ele2_isEBEEGap", &ele2_isEBEEGap, "ele2_isEBEEGap/I");
 myTree_ -> Branch("ele2_isEBEtaGap", &ele2_isEBEtaGap, "ele2_isEBEtaGap/I");
 myTree_ -> Branch("ele2_isEBPhiGap", &ele2_isEBPhiGap, "ele2_isEBPhiGap/I");
 myTree_ -> Branch("ele2_isEEDeeGap", &ele2_isEEDeeGap, "ele2_isEEDeeGap/I");
 myTree_ -> Branch("ele2_isEERingGap", &ele2_isEERingGap, "ele2_isEERingGap/I");
  
 myTree_ -> Branch("ele2_isTrackerDriven",&ele2_isTrackerDriven,"ele2_isTrackerDriven/I");
 myTree_ -> Branch("ele2_classification",&ele2_classification,"ele2_classification/I");

 myTree_ -> Branch("ele2_sigmaIetaIeta", &ele2_sigmaIetaIeta, "ele2_sigmaIetaIeta/F");
 myTree_ -> Branch("ele2_DphiIn", &ele2_DphiIn, "ele2_DphiIn/F");
 myTree_ -> Branch("ele2_DetaIn", &ele2_DetaIn, "ele2_DetaIn/F");

 myTree_ -> Branch("ele2_deltaEtaEleClusterTrackAtCalo", &ele2_deltaEtaEleClusterTrackAtCalo, "ele2_deltaEtaEleClusterTrackAtCalo/F");
 myTree_ -> Branch("ele2_deltaEtaPhiClusterTrackAtCalo", &ele2_deltaEtaPhiClusterTrackAtCalo, "ele2_deltaEtaPhiClusterTrackAtCalo/F");
 myTree_ -> Branch("ele2_deltaEtaSeedClusterTrackAtCalo", &ele2_deltaEtaSeedClusterTrackAtCalo, "ele2_deltaEtaSeedClusterTrackAtCalo/F");
 myTree_ -> Branch("ele2_deltaPhiSeedClusterTrackAtCalo", &ele2_deltaPhiSeedClusterTrackAtCalo, "ele2_deltaPhiSeedClusterTrackAtCalo/F");

 myTree_ -> Branch("ele2_HOverE", &ele2_HOverE, "ele2_HOverE/F");
 myTree_ -> Branch("ele2_tkIso", &ele2_tkIso, "ele2_tkIso/F");
 myTree_ -> Branch("ele2_emIso", &ele2_emIso, "ele2_emIso/F");
 myTree_ -> Branch("ele2_hadIso", &ele2_hadIso, "ele2_hadIso/F");
  
 myTree_ -> Branch("ele2_dxy_PV", &ele2_dxy_PV, "ele2_dxy_PV/F");
 myTree_ -> Branch("ele2_dz_PV", &ele2_dz_PV, "ele2_dz_PV/F");
 myTree_ -> Branch("ele2_sigmaP", &ele2_sigmaP, "ele2_sigmaP/F");
  
 myTree_ -> Branch("ele2_scERaw", &ele2_scERaw, "ele2_scERaw/F");
 myTree_ -> Branch("ele2_scEtRaw", &ele2_scEtRaw, "ele2_scEtRaw/F");
 myTree_ -> Branch("ele2_scE", &ele2_scE, "ele2_scE/F");
 myTree_ -> Branch("ele2_scEt", &ele2_scEt, "ele2_scEt/F");
 myTree_ -> Branch("ele2_scERaw_PUcleaned", &ele2_scERaw_PUcleaned, "ele2_scERaw_PUcleaned/F");
 myTree_ -> Branch("ele2_es", &ele2_es, "ele2_es/F");
 myTree_ -> Branch("ele2_scLaserCorr", &ele2_scLaserCorr, "ele2_scLaserCorr/F");
 myTree_ -> Branch("ele2_scCrackCorr", &ele2_scCrackCorr, "ele2_scCrackCorr/F");
 myTree_ -> Branch("ele2_scLocalContCorr", &ele2_scLocalContCorr, "ele2_scLocalContCorr/F");
  
 myTree_ -> Branch("ele2_scEta", &ele2_scEta, "ele2_scEta/F");
 myTree_ -> Branch("ele2_scPhi", &ele2_scPhi, "ele2_scPhi/F");
 myTree_ -> Branch("ele2_scLocalEta", &ele2_scLocalEta, "ele2_scLocalEta/F");
 myTree_ -> Branch("ele2_scLocalPhi", &ele2_scLocalPhi, "ele2_scLocalPhi/F");
 myTree_ -> Branch("ele2_scEtaWidth", &ele2_scEtaWidth, "ele2_scEtaWidth/F");
 myTree_ -> Branch("ele2_scPhiWidth", &ele2_scPhiWidth, "ele2_scPhiWidth/F");
 myTree_ -> Branch("ele2_scEtaWidth_PUcleaned", &ele2_scEtaWidth_PUcleaned, "ele2_scEtaWidth_PUcleaned/F");
 myTree_ -> Branch("ele2_scPhiWidth_PUcleaned", &ele2_scPhiWidth_PUcleaned, "ele2_scPhiWidth_PUcleaned/F");
 myTree_ -> Branch("ele2_fCorrection_PUcleaned", &ele2_fCorrection_PUcleaned, "ele2_fCorrection_PUcleaned/F");
  
 myTree_ -> Branch("ele2_fEta", &ele2_fEta, "ele2_fEta/F");
 myTree_ -> Branch("ele2_fEtaCorr", &ele2_fEtaCorr, "ele2_fEtaCorr/F");
 myTree_ -> Branch("ele2_tkP", &ele2_tkP, "ele2_tkP/F");
 myTree_ -> Branch("ele2_tkPt", &ele2_tkPt, "ele2_tkPt/F");
 myTree_ -> Branch("ele2_fbrem", &ele2_fbrem, "ele2_fbrem/F");

 myTree_ -> Branch("ele2_eSeedBC", &ele2_eSeedBC, "ele2_eSeedBC/F");
 myTree_ -> Branch("ele2_e5x5", &ele2_e5x5, "ele2_e5x5/F");
 myTree_ -> Branch("ele2_e3x3", &ele2_e3x3, "ele2_e3x3/F");
 myTree_ -> Branch("ele2_scNxtal", &ele2_scNxtal, "ele2_scNxtal/F");
 myTree_ -> Branch("ele2_bcN", &ele2_bcN, "ele2_bcN/I");
 myTree_ -> Branch("ele2_5x5LaserCorr", &ele2_5x5LaserCorr, "ele2_5x5LaserCorr/F");
 myTree_ -> Branch("ele2_3x3LaserCorr", &ele2_3x3LaserCorr, "ele2_3x3LaserCorr/F");
   
 myTree_ -> Branch("ele2_seedE", &ele2_seedE, "ele2_seedE/F");
 myTree_ -> Branch("ele2_seedLaserAlpha",&ele2_seedLaserAlpha,"ele2_seedLaserAlpha/F");
 myTree_ -> Branch("ele2_seedLaserCorr", &ele2_seedLaserCorr, "ele2_seedLaserCorr/F");
 myTree_ -> Branch("ele2_seedICConstant",&ele2_seedICConstant,"ele2_seedICConstant/F");
 myTree_ -> Branch("ele2_seedIeta", &ele2_seedIeta, "ele2_seedIeta/I");
 myTree_ -> Branch("ele2_seedIphi", &ele2_seedIphi, "ele2_seedIphi/I");
 myTree_ -> Branch("ele2_seedIx", &ele2_seedIx, "ele2_seedIx/I");
 myTree_ -> Branch("ele2_seedIy", &ele2_seedIy, "ele2_seedIy/I");
 myTree_ -> Branch("ele2_seedZside", &ele2_seedZside, "ele2_seedZside/I");
 myTree_ -> Branch("ele2_EOverP", &ele2_EOverP, "ele2_EOverP/F");
  
 myTree_ -> Branch("ele2_nRecHits",&ele2_nRecHits,"ele2_nRecHits/I");
 myTree_ -> Branch("ele2_recHit_E", "std::vector<float>",&ele2_recHit_E);
 myTree_ -> Branch("ele2_recHit_flag", "std::vector<int>", &ele2_recHit_flag);
 myTree_ -> Branch("ele2_recHit_hashedIndex", "std::vector<int>", &ele2_recHit_hashedIndex);
 myTree_ -> Branch("ele2_recHit_ietaORix", "std::vector<int>", &ele2_recHit_ietaORix);
 myTree_ -> Branch("ele2_recHit_iphiORiy", "std::vector<int>", &ele2_recHit_iphiORiy);
 myTree_ -> Branch("ele2_recHit_zside", "std::vector<int>", &ele2_recHit_zside);
 myTree_ -> Branch("ele2_recHit_laserCorrection","std::vector<float>",&ele2_recHit_laserCorrection);
 myTree_ -> Branch("ele2_recHit_Alpha", "std::vector<float>",&ele2_recHit_Alpha);
 myTree_ -> Branch("ele2_recHit_ICConstant", "std::vector<float>",&ele2_recHit_ICConstant);

 if(saveRecHitMatrix_) {
  myTree_ -> Branch("ele2_recHitMatrix_E", "std::vector<float>",&ele2_recHitMatrix_E);
  myTree_ -> Branch("ele2_recHitMatrix_flag", "std::vector<int>", &ele2_recHitMatrix_flag);
  myTree_ -> Branch("ele2_recHitMatrix_hashedIndex", "std::vector<int>", &ele2_recHitMatrix_hashedIndex);
  myTree_ -> Branch("ele2_recHitMatrix_ietaORix", "std::vector<int>", &ele2_recHitMatrix_ietaORix);
  myTree_ -> Branch("ele2_recHitMatrix_iphiORiy", "std::vector<int>", &ele2_recHitMatrix_iphiORiy);
  myTree_ -> Branch("ele2_recHitMatrix_zside", "std::vector<int>", &ele2_recHitMatrix_zside);
  myTree_ -> Branch("ele2_recHitMatrix_laserCorrection","std::vector<float>",&ele2_recHitMatrix_laserCorrection);
  myTree_ -> Branch("ele2_recHitMatrix_ICConstant", "std::vector<float>",&ele2_recHitMatrix_ICConstant);
  myTree_ -> Branch("ele2_recHitMatrix_samples", "std::vector<float>",&ele2_recHitMatrix_samples);
 }

 // met variables
 std::cout << ">>>>>> AnalyzerEle::AnalyzerEle::set met branches <<<" << std::endl;
 myTree_ -> Branch("met_et", &met_et, "met_et/F");
 myTree_ -> Branch("met_phi", &met_phi, "met_phi/F");
 myTree_ -> Branch("ele1Met_mt", &ele1Met_mt, "ele1Met_mt/F");
 myTree_ -> Branch("ele1Met_Dphi",&ele1Met_Dphi,"ele1Met_Dphi/F");

 // di-electron variables
 std::cout << ">>>>>> AnalyzerEle::AnalyzerEle::set dielectron branches <<<" << std::endl;
 myTree_ -> Branch("ele1ele2_m", &ele1ele2_m, "ele1ele2_m/F");
 myTree_ -> Branch("ele1ele2_scM", &ele1ele2_scM, "ele1ele2_scM/F");

 // fbrem variables
 if(saveFbrem_) {

  std::cout << ">>>>>> AnalyzerEle::AnalyzerEle::set fBrem branches <<<" << std::endl;

  myTree_ -> Branch("ele1_inner_p", &ele1_inner_p, "ele1_inner_p/F");
  myTree_ -> Branch("ele1_inner_x", &ele1_inner_x, "ele1_inner_x/F");
  myTree_ -> Branch("ele1_inner_y", &ele1_inner_y, "ele1_inner_y/F");
  myTree_ -> Branch("ele1_inner_z", &ele1_inner_z, "ele1_inner_z/F");
  myTree_ -> Branch("ele1_outer_p", &ele1_outer_p, "ele1_outer_p/F");
  myTree_ -> Branch("ele1_outer_x", &ele1_outer_x, "ele1_outer_x/F");
  myTree_ -> Branch("ele1_outer_y", &ele1_outer_y, "ele1_outer_y/F");
  myTree_ -> Branch("ele1_outer_z", &ele1_outer_z, "ele1_outer_z/F");
  myTree_ -> Branch("ele1_tangent_n",&ele1_tangent_n, "ele1_tangent_n/I");
  myTree_ -> Branch("ele1_tangent_p", "std::vector<float>",&ele1_tangent_p);
  myTree_ -> Branch("ele1_tangent_x", "std::vector<float>",&ele1_tangent_x);
  myTree_ -> Branch("ele1_tangent_y", "std::vector<float>",&ele1_tangent_y);
  myTree_ -> Branch("ele1_tangent_z", "std::vector<float>",&ele1_tangent_z);
  myTree_ -> Branch("ele1_tangent_dP", "std::vector<float>",&ele1_tangent_dP);
  myTree_ -> Branch("ele1_tangent_dPerr","std::vector<float>",&ele1_tangent_dPerr);

  myTree_ -> Branch("ele2_inner_p", &ele2_inner_p, "ele2_inner_p/F");
  myTree_ -> Branch("ele2_inner_x", &ele2_inner_x, "ele2_inner_x/F");
  myTree_ -> Branch("ele2_inner_y", &ele2_inner_y, "ele2_inner_y/F");
  myTree_ -> Branch("ele2_inner_z", &ele2_inner_z, "ele2_inner_z/F");
  myTree_ -> Branch("ele2_outer_p", &ele2_outer_p, "ele2_outer_p/F");
  myTree_ -> Branch("ele2_outer_x", &ele2_outer_x, "ele2_outer_x/F");
  myTree_ -> Branch("ele2_outer_y", &ele2_outer_y, "ele2_outer_y/F");
  myTree_ -> Branch("ele2_outer_z", &ele2_outer_z, "ele2_outer_z/F");
  myTree_ -> Branch("ele2_tangent_n",&ele2_tangent_n,"ele2_tangent_n/I");
  myTree_ -> Branch("ele2_tangent_p", "std::vector<float>",&ele2_tangent_p);
  myTree_ -> Branch("ele2_tangent_x", "std::vector<float>",&ele2_tangent_x);
  myTree_ -> Branch("ele2_tangent_y", "std::vector<float>",&ele2_tangent_y);
  myTree_ -> Branch("ele2_tangent_z", "std::vector<float>",&ele2_tangent_z);
  myTree_ -> Branch("ele2_tangent_dP", "std::vector<float>",&ele2_tangent_dP);
  myTree_ -> Branch("ele2_tangent_dPerr","std::vector<float>",&ele2_tangent_dPerr);
 }

 std::cout << ">>>>>> AnalyzerEle::AnalyzerEle::get cluster corrections <<<" << std::endl;
 EcalClusterCrackCorrection = EcalClusterFunctionFactory::get()->create("EcalClusterCrackCorrection", iConfig);
 EcalClusterLocalContCorrection = EcalClusterFunctionFactory::get()->create("EcalClusterLocalContCorrection", iConfig);

 std::cout << ">>> AnalyzerEle::AnalyzerEle end <<<" << std::endl;
}

// --------------------------------------------------------------------
AnalyzerEle::~AnalyzerEle () { std::cout << ">>> AnalyzerEle::~AnalyzerEle <<< analyzed " << eventNaiveId_ << " events" << std::endl;}

// ------------ method called once each job just before starting event loop ------------
void AnalyzerEle::beginJob(){}

// ------------ method called once each job just after ending the event loop ------------
void AnalyzerEle::endJob(){

  if(myTree_->GetEntries()>0){
    myTree_->BuildIndex("runId","eventId");
  }

}

// -----------------------------------------------------------------------------------------
void AnalyzerEle::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {

 if( verbosity_ )
  std::cout << ">>> AnalyzerEle::analyze begin <<<" << std::endl;

 ++eventNaiveId_;

 bool isGoodEvent = false;
 initialize();

 // event variables
 bxId_          = iEvent.bunchCrossing();
 runId_         = iEvent.id().run();
 lumiId_        = iEvent.luminosityBlock();
 eventId_       = iEvent.id().event();
 timeStampHigh_ = (int)(iEvent.time().value() >> 32);

 //************* VERTEXES
 fillPVInfo (iEvent,iSetup) ;

 //************* RHO for ISO
 fillRhoInfo(iEvent,iSetup) ;

 //************* Fill HLT Info
 fillHLTInfo(iEvent,iSetup);

 //************* Pile Up Info
 if(saveMCInfo_) fillPileUpInfo(iEvent,iSetup); 

 //************* Generator info
 if(saveMCInfo_) fillGeneratorInfo(iEvent,iSetup);


 //************* ELECTRONS
 edm::Handle<edm::View<pat::Electron> > electronHandle;
 iEvent.getByLabel(EleTag_,electronHandle);
 edm::View<pat::Electron> electrons = *electronHandle;

 ///---- get the number of the electron in the event to know if it's a W or a Z ----
 nele_ = electrons.size();
 if(doWZSelection_){

   std::map<float,int> eleIts_ ; 
   eleIts_.clear();

   int nWP70 = 0; //only WP70
   int nWP90 = 0; //only WP90
   for( unsigned int iEle = 0 ; iEle < electrons.size(); iEle++){
     if(electrons.at(iEle).electronID("tight")){
	  ++nWP70;
          eleIts_[1./electrons.at(iEle).pt()] = iEle;
	  continue;
     }
     if(electrons.at(iEle).electronID("loose")){
	  ++nWP90;
          eleIts_[1./electrons.at(iEle).pt()] = iEle;
	  continue;
      }
   }

   if( nWP70 < 1 or nWP90 < 2 ) return;
   if( nWP90 > 2 ) return;

  ///---- check if the event is good----
  if( (nWP70 == 1) && (nWP90 < 2) ){
   isW_=1; isZ_=0;
   std::map<float,int>::const_iterator mapIt = eleIts_.begin();
   fillEleInfo ( iEvent, iSetup, mapIt->second, "ele1" );
   fillMetInfo (iEvent, iSetup);
   isGoodEvent = myWselection ( iEvent, iSetup);
   if ( isGoodEvent ) myTree_ -> Fill();
  }

  if( nWP70 == 2 or nWP90 == 2 or (nWP70 == 1 and nWP90 == 2 ) ){
   isW_=0; isZ_=1;
   std::map<float,int>::const_iterator mapIt = eleIts_.begin();
   fillEleInfo ( iEvent, iSetup, mapIt->second, "ele1" );
   mapIt++;
   fillEleInfo ( iEvent, iSetup, mapIt->second, "ele2" );
   fillDoubleEleInfo (iEvent, iSetup);
   fillMetInfo (iEvent, iSetup);
   isGoodEvent = myZselection ( iEvent, iSetup);
   if ( isGoodEvent ) myTree_ -> Fill();
  }
 }
 else {
   
  int nEle = electrons.size();      
  if ( nEle == 1 ) { isW_ = 1; isZ_ = 0; }
  if ( nEle >= 2 ) { isW_ = 0; isZ_ = 1; }

  if ( isW_ == 1 ) fillEleInfo ( iEvent, iSetup, 0, "ele1" );

  if ( isZ_ == 1 ) {
   fillEleInfo ( iEvent, iSetup, 0, "ele1" );
   fillEleInfo ( iEvent, iSetup, 1, "ele2" );
   fillDoubleEleInfo (iEvent, iSetup);
  }

  fillMetInfo (iEvent, iSetup);
  if (isW_ == 1 || isZ_ == 1) myTree_ -> Fill();

 }

 if( verbosity_ ) std::cout << ">>> AnalyzerEle::analyze end <<<" << std::endl;
}


// -----------------------------------------------------------------------------------------
bool AnalyzerEle::myWselection (const edm::Event & iEvent, const edm::EventSetup & iSetup){
  
 if( ele1_pt < 30. )       return false;         
 if( met_et < 25.00 )      return false;
 if( ele1Met_mt < 50.00 )  return false;
 if( ele1Met_Dphi < 1.57 ) return false;
  
 return true;
  
}

// -----------------------------------------------------------------------------------------
bool AnalyzerEle::myZselection (const edm::Event & iEvent, const edm::EventSetup & iSetup){

 if( met_et > 40. )      return false;
 if( ele1ele2_m < 60. )  return false;
 if( ele1ele2_m > 120. ) return false;
 if( (ele1_charge * ele2_charge) != -1. ) return false;
      
 return true;
  
}

// -----------------------------------------------------------------------------------------
void AnalyzerEle::fillPVInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup){

 edm::Handle<reco::VertexCollection> vertexes;
 iEvent.getByLabel(PVTag_, vertexes);
 PV_n_ = vertexes -> size();

 reco::Vertex PV;
 bool PVfound = (vertexes -> size() != 0);
 if(PVfound){    
  PV = vertexes->at(0);
  PV_z_ = PV.z();
  PV_d0_ = PV.position().Rho();
 }
 else {
  PV_z_ = -999.;
  PV_d0_ = -999.;
 }

 math::XYZPoint PVPoint(PV.position().x(), PV.position().y(), PV.position().z());
 PVPoint_ = PVPoint;

}

//------------------------------------------------------------------------------------------------------------
void AnalyzerEle::fillRhoInfo(const edm::Event & iEvent, const edm::EventSetup & iSetup){
  
 edm::Handle< double > rhoHandle;
 iEvent.getByLabel(rhoTag_,rhoHandle);
 rho_ = *rhoHandle;
}

//------------------------------------------------------------------------------------------------------------
void AnalyzerEle::fillHLTInfo(const edm::Event & iEvent, const edm::EventSetup & iSetup){

 //------------------------------ HLT
 edm::Handle<edm::TriggerResults> triggerResultsHandle;
 iEvent.getByLabel(triggerResultsCollection_, triggerResultsHandle);

 edm::TriggerNames HLTNames = iEvent.triggerNames(*triggerResultsHandle);
 int hltCount = triggerResultsHandle->size();
 HLTNames_.clear();
 HLTBits_.clear();
 HLTResults_.clear();
 for (int i = 0 ; i != hltCount; ++i) {
   std::string hltName_str(HLTNames.triggerName(i));
   HLTNames_.push_back(hltName_str);
   HLTBits_.push_back(triggerResultsHandle->accept(i));
   HLTResults_.insert(std::pair<std::string, bool>( hltName_str, triggerResultsHandle->accept(i)));
 } 

}

//--------------------------------------------------------------------------------------------------------------
void AnalyzerEle::fillPileUpInfo (const edm::Event & iEvent, const edm::EventSetup & iSetup){

 edm::Handle<std::vector< PileupSummaryInfo > > PupInfo;
 iEvent.getByLabel(edm::InputTag("addPileupInfo"), PupInfo);
 std::vector<PileupSummaryInfo>::const_iterator PVI;
 
 for(PVI = PupInfo->begin(); PVI != PupInfo->end(); ++PVI) {
   nPU_ = PVI->getTrueNumInteractions();
 }
 
}

//------------------------------------------------------------------------------------------------------------
void AnalyzerEle::fillGeneratorInfo(const edm::Event & iEvent, const edm::EventSetup & iSetup){

 edm::Handle<reco::GenParticleCollection> genParticles; // take the genParticle collection from the event
 iEvent.getByLabel(MCtruthTag_, genParticles);
 eventType_ = 0;

 std::vector<const reco::Candidate*> mcCharged_p ;
 std::vector<const reco::Candidate*> mcE_p ;
 std::vector<const reco::Candidate*> mcMu_p ;
 std::vector<const reco::Candidate*> mcTau_p ;
 std::vector<const reco::Candidate*> mcV_p;

 for(reco::GenParticleCollection::const_iterator p = genParticles -> begin(); p != genParticles -> end(); ++p){
    // take the current element in the list and his mother
    const reco::Candidate* pCurrent = &(*p);
    const reco::Candidate* pMother = 0;
    if(pCurrent -> mother()) pMother = pCurrent -> mother();
    
    int pdgId  = p -> pdgId();
    int status = p -> status();
    int charge = p -> charge();
    int motherPdgId = 0;
    if(pCurrent -> mother()) motherPdgId = pMother -> pdgId();            
    // charged
    if( (abs(charge) != 0) && (status == 1) ) mcCharged_p.push_back(pCurrent);
    // electrons
    if( (abs(pdgId) == 11) && (status == 1) ) mcE_p.push_back(pCurrent);    
    // muons
    if( (abs(pdgId) == 13) && (status == 1) ) mcMu_p.push_back(pCurrent);    
    // taus and tauJ
    if((abs(pdgId) == 16) && (status == 1) && (abs(motherPdgId) == 15)) mcTau_p.push_back(pMother);          
    // W or Z
    if(((pdgId == 23) && (status == 3)) || ( (abs(pdgId) == 24) && (status == 3)) ) mcV_p.push_back(pCurrent);
          
 } // loop over gen particles
      
 // check if it is signal
 if(mcV_p.empty()){
    if(verbosity_)
      std::cerr << ">>> MCDumperZW::Analyze::Warning: no W/Z in the event" << std::endl;    
      return;
 }

 if(mcV_p.size() > 1 ){
    if(verbosity_)
      std::cerr << ">>> MCDumperZW::Analyze::Warning: more than one W/Z in the event" << std::endl;    
      return;
 }

 // find fermions from vector boson decay
 std::vector<const reco::Candidate*> fFromVBuffer;

 for(unsigned int i = 0; i < mcV_p.size() ; ++i){
   for( unsigned int j = 0; j < mcV_p.at(i)->numberOfDaughters() ; ++j){
     if(mcV_p.at(i)->daughter(j)-> pdgId() != mcV_p.at(i)->pdgId())
       fFromVBuffer.push_back(mcV_p.at(i)->daughter(j)); 
   }
 }

 if(fFromVBuffer.size() != 2){
     if(verbosity_)
       std::cerr << "MCDumperZW::Analyze::Warning: not a V -> ff decay --> more than on W or Z at matrx element" << std::endl;
     return;
 }
  
 const reco::Candidate* mcF1_fromV_p = fFromVBuffer.at(0);
 const reco::Candidate* mcF2_fromV_p = fFromVBuffer.at(1);
    
 if(mcV_p.at(0) -> pdgId() == 23){
    if(mcF2_fromV_p -> pt() > mcF1_fromV_p -> pt()){
	const reco::Candidate* buffer = mcF1_fromV_p;
	mcF1_fromV_p = mcF2_fromV_p;
	mcF2_fromV_p = buffer;
    }
 }
 else if(abs(mcV_p.at(0) -> pdgId()) == 24){
     if( (abs(mcF1_fromV_p -> pdgId()))%2 == 0){
	 const reco::Candidate* buffer = mcF1_fromV_p;
	 mcF1_fromV_p = mcF2_fromV_p;
	 mcF2_fromV_p = buffer;
       }
 }
  
 //fill info
 mcV_E   = mcV_p.at(0)->p4().E();
 mcV_Px  = mcV_p.at(0)->p4().Px();
 mcV_Py  = mcV_p.at(0)->p4().Py();
 mcV_Pz  = mcV_p.at(0)->p4().Pz();
 mcV_Eta = mcV_p.at(0)->p4().Eta();
 mcV_Phi = mcV_p.at(0)->p4().Phi();
 mcV_M   = mcV_p.at(0)->p4().M();
 mcV_Charge = mcV_p.at(0)->charge();
 mcV_PdgId  = mcV_p.at(0)->pdgId();

 // lepton1 pT > lepton2 pT
 mcF1_fromV_E      = mcF1_fromV_p->p4().E();
 mcF1_fromV_Px     = mcF1_fromV_p->p4().Px();
 mcF1_fromV_Py     = mcF1_fromV_p->p4().Py();
 mcF1_fromV_Pz     = mcF1_fromV_p->p4().Pz();
 mcF1_fromV_Eta    = mcF1_fromV_p->p4().Eta();
 mcF1_fromV_Phi    = mcF1_fromV_p->p4().Phi();
 mcF1_fromV_M      = mcF1_fromV_p->p4().M();
 mcF1_fromV_Charge = mcF1_fromV_p->charge();
 mcF1_fromV_PdgId  = mcF1_fromV_p->pdgId();

 mcF2_fromV_E      = mcF2_fromV_p->p4().E();
 mcF2_fromV_Px     = mcF2_fromV_p->p4().Px();
 mcF2_fromV_Py     = mcF2_fromV_p->p4().Py();
 mcF2_fromV_Pz     = mcF2_fromV_p->p4().Pz();
 mcF2_fromV_Eta    = mcF2_fromV_p->p4().Eta();
 mcF2_fromV_Phi    = mcF2_fromV_p->p4().Phi();
 mcF2_fromV_M      = mcF2_fromV_p->p4().M();
 mcF2_fromV_Charge = mcF2_fromV_p->charge();
 mcF2_fromV_PdgId  = mcF2_fromV_p->pdgId();
            
}
  
  
  
  

// ------------------------------------------------------------------------------------------------------------
void AnalyzerEle::fillEleInfo(const edm::Event & iEvent, const edm::EventSetup & iSetup, const int iEle, const std::string eleName) {

 if( verbosity_ )std::cout << ">>> AnalyzerEle::fillEleInfo start <<<" << std::endl;
  
 //*********** TRACKER GEOMETRY
 edm::ESHandle<TrackerGeometry> pDD ;
 iSetup.get<TrackerDigiGeometryRecord> ().get(pDD);

 //*********** MAGNETIC FIELD
 edm::ESHandle<MagneticField> theMagField ;
 iSetup.get<IdealMagneticFieldRecord>().get(theMagField);
  
 //*********** CALO TOPOLOGY
 edm::ESHandle<CaloTopology> pTopology;
 iSetup.get<CaloTopologyRecord>().get(pTopology);
 const CaloTopology *topology = pTopology.product();
  
 //*********** IC CONSTANTS
 edm::ESHandle<EcalIntercalibConstants> theICConstants;
 iSetup.get<EcalIntercalibConstantsRcd>().get(theICConstants);
 const EcalIntercalibConstantMap& ICMap = theICConstants->getMap();
  
 //*********** ADCToGeV
 edm::ESHandle<EcalADCToGeVConstant> theADCToGeV;
 iSetup.get<EcalADCToGeVConstantRcd>().get(theADCToGeV);
  
 //*********** LASER ALPHAS
 edm::ESHandle<EcalLaserAlphas> theEcalLaserAlphas;
 iSetup.get<EcalLaserAlphasRcd>().get(theEcalLaserAlphas);
 const EcalLaserAlphaMap* theEcalLaserAlphaMap = theEcalLaserAlphas.product();
  
 //*********** LASER CORRECTION
 edm::ESHandle<EcalLaserDbService> theLaser;
 iSetup.get<EcalLaserDbRecord>().get(theLaser);
  
 //*********** EB DIGI HITS
 edm::Handle<EBDigiCollection> ebDigis;
 if(saveRecHitMatrix_){
  iEvent.getByLabel (digiCollection_EB_, ebDigis) ;
  if (! (ebDigis.isValid ()) ) {
   std::cerr << "EcalValidation::analyze --> ebDigis not found" << std::endl;
  }
 }

 //*********** EE DIGI HITS
 edm::Handle<EEDigiCollection> eeDigis;
 if(saveRecHitMatrix_){
  iEvent.getByLabel (digiCollection_EE_, eeDigis) ;
  if (! (eeDigis.isValid ()) ) {
   std::cerr << "EcalValidation::analyze --> eeDigis not found" << std::endl;
  }
 }

 //*********** EB REC HITS
 edm::Handle<EcalRecHitCollection> recHitsEB;
 iEvent.getByLabel( recHitCollection_EB_, recHitsEB );
 const EcalRecHitCollection* theBarrelEcalRecHits = recHitsEB.product () ;
 if ( ! recHitsEB.isValid() ) {
  std::cerr << "AnalyzerEle::analyze --> recHitsEB not found" << std::endl;
 }
  
 //*********** EE REC HITS
 edm::Handle<EcalRecHitCollection> recHitsEE;
 iEvent.getByLabel( recHitCollection_EE_, recHitsEE );
 const EcalRecHitCollection* theEndcapEcalRecHits = recHitsEE.product () ;
 if ( ! recHitsEE.isValid() ) {
  std::cerr << "AnalyzerEle::analyze --> recHitsEE not found" << std::endl;
 }
  
 //*********** EB SR FLAGS
 edm::Handle<EBSrFlagCollection> SRFlagsEB;
 std::vector<EcalTrigTowerDetId> TTIdList;
  
 if( saveRecHitMatrix_ ){
  iEvent.getByLabel(SRFlagCollection_EB_, SRFlagsEB );
  for(EBSrFlagCollection::const_iterator it = SRFlagsEB->begin(); it != SRFlagsEB->end(); ++it){
   const int flag = it->value();
   if( flag != EcalSrFlag::SRF_FULL ) continue;
   const EcalTrigTowerDetId TTId = it->id();
   TTIdList.push_back(TTId);
  }
 }
  
 //*********** EE SR FLAGS
 edm::Handle<EESrFlagCollection> SRFlagsEE;
 std::vector<EcalScDetId> SCIdList;
 if( saveRecHitMatrix_ ){

  iEvent.getByLabel(SRFlagCollection_EE_, SRFlagsEE );
  for(EESrFlagCollection::const_iterator it = SRFlagsEE->begin(); it != SRFlagsEE->end(); ++it){
   const int flag = it->value();
   if( flag != EcalSrFlag::SRF_FULL ) continue;
   const EcalScDetId SCId = it->id();
   SCIdList.push_back(SCId);
  }
 }
  
 //************* ELECTRONS
 edm::Handle<edm::View<pat::Electron> > electronHandle;
 iEvent.getByLabel(EleTag_,electronHandle);
 edm::View<pat::Electron> electrons = *electronHandle;
  
 //************* VERTEX COLLECTION
 edm::Handle<reco::VertexCollection> hVertexProduct;
 iEvent.getByLabel(PVTag_,hVertexProduct);
  
 //************* CLUSTER LAZY TOOLS
 EcalClusterLazyTools lazyTools(iEvent,iSetup,recHitCollection_EB_,recHitCollection_EE_);
 EcalClusterPUCleaningTools cleaningTools(iEvent, iSetup, recHitCollection_EB_, recHitCollection_EE_);


 // Take the correct ele
 bool printOut = false;
 pat::Electron electron = electrons.at(iEle);

 if ( eleName == "ele1") {

  ele1        = electron.p4();
  ele1_charge = electron.charge();
  ele1_p      = ele1.P();
  ele1_pt     = ele1.Pt();
  ele1_eta    = ele1.eta();
  ele1_phi    = ele1.phi();

  ele1_isEB       = electron.isEB();
  ele1_isEBEEGap  = electron.isEBEEGap();
  ele1_isEBEtaGap = electron.isEBEtaGap();
  ele1_isEBPhiGap = electron.isEBPhiGap();
  ele1_isEEDeeGap = electron.isEEDeeGap();
  ele1_isEERingGap = electron.isEERingGap();
 
  ele1_idtype = -1 ;
  if(electron.electronID("fiducial")) ele1_idtype = 0 ;
  if(electron.electronID("loose"))    ele1_idtype = 1;
  if(electron.electronID("medium"))   ele1_idtype = 2;
  if(electron.electronID("tight"))    ele1_idtype = 3;
  if(electron.electronID("WP90PU"))   ele1_idtype = 4;
  if(electron.electronID("WP80PU"))   ele1_idtype = 5;
  if(electron.electronID("WP70PU"))   ele1_idtype = 6;

  ele1_isTrackerDriven = !(electron.ecalDriven());
  ele1_classification  = electron.classification();
  ele1_sigmaIetaIeta   = electron.sigmaIetaIeta();
  ele1_DphiIn          = electron.deltaPhiSuperClusterTrackAtVtx();
  ele1_DetaIn          = electron.deltaEtaSuperClusterTrackAtVtx();

  ele1_deltaEtaEleClusterTrackAtCalo = electron.deltaEtaEleClusterTrackAtCalo();
  ele1_deltaEtaPhiClusterTrackAtCalo = electron.deltaPhiEleClusterTrackAtCalo();
  ele1_deltaEtaSeedClusterTrackAtCalo = electron.deltaEtaSeedClusterTrackAtCalo();
  ele1_deltaPhiSeedClusterTrackAtCalo = electron.deltaPhiSeedClusterTrackAtCalo();

  ele1_HOverE  = electron.hadronicOverEm();
  ele1_ooemoop = (1.0/electron.ecalEnergy() - electron.eSuperClusterOverP()/electron.ecalEnergy());
  ele1_tkIso   = electron.dr03TkSumPt();
  ele1_emIso   = electron.dr03EcalRecHitSumEt();
  ele1_hadIso  = electron.dr03HcalDepth1TowerSumEt()+electron.dr03HcalDepth2TowerSumEt();

  reco::GsfTrackRef eleTrack = electron.gsfTrack();
  ele1_dxy_PV  = eleTrack->dxy (PVPoint_);
  ele1_dz_PV   = eleTrack->dz (PVPoint_);
  ele1_sigmaP  = electron.corrections().trackMomentumError;

  reco::SuperClusterRef scRef = electron.superCluster();
  const edm::Ptr<reco::CaloCluster>& seedCluster = scRef->seed();

  double R  = TMath::Sqrt(scRef->x()*scRef->x() + scRef->y()*scRef->y() +scRef->z()*scRef->z());
  double Rt = TMath::Sqrt(scRef->x()*scRef->x() + scRef->y()*scRef->y());

  edm::Handle< double > rhoHandle;
  iEvent.getByLabel(rhoTag_,rhoHandle);

  ele1_scERaw  = scRef->rawEnergy();
  ele1_scEtRaw = scRef->rawEnergy()*(Rt/R);
  ele1_scEt    = scRef->energy()*(Rt/R);
  ele1_scEtaWidth = scRef->etaWidth();
  ele1_scPhiWidth = scRef->phiWidth();
  ele1_scE        = scRef->energy();
  ele1_scEta = scRef->eta();
  ele1_scPhi = scRef->phi();


  EcalClusterLocal ecalLocalCoord;
  float bcLocalEta, bcLocalPhi, bcThetatilt, bcPhitilt;
  int bcIeta, bcIphi;
  bcLocalEta = 0;

  if ( electron.isEB() )
   ecalLocalCoord.localCoordsEB(*seedCluster,iSetup,bcLocalEta,bcLocalPhi,bcIeta,bcIphi,bcThetatilt,bcPhitilt);
  if ( electron.isEE() )
   ecalLocalCoord.localCoordsEE(*seedCluster,iSetup,bcLocalEta,bcLocalPhi,bcIeta,bcIphi,bcThetatilt,bcPhitilt);

  ele1_scLocalEta = bcLocalEta;
  ele1_scLocalPhi = bcLocalPhi;

  // crack correction variables and local containment corrections
  EcalClusterCrackCorrection -> init(iSetup);
  EcalClusterLocalContCorrection -> init(iSetup);
  double crackcor = 1.;
  double localContCorr = 1.;

  for(reco::CaloCluster_iterator cIt = electron.superCluster()->clustersBegin(); cIt != electron.superCluster()->clustersEnd(); ++cIt) {
   const reco::CaloClusterPtr cc = *cIt;
   crackcor *= ( (electron.superCluster()->rawEnergy() + (*cIt)->energy()*(EcalClusterCrackCorrection->getValue(*cc)-1.)) / electron.superCluster()->rawEnergy() );
  }
  localContCorr = EcalClusterLocalContCorrection->getValue(*electron.superCluster(), 1) ;
    
  ele1_scCrackCorr     = crackcor;
  ele1_scLocalContCorr = localContCorr;
    
    
  reco::SuperCluster cleanedSC = cleaningTools.CleanedSuperCluster(0.02, *scRef, iEvent );
  reco::CaloClusterPtr myseed = (*scRef).seed();

  if ( !((myseed->seed()).rawId()) || (myseed->seed()).rawId()==0 ) {
   ele1_scERaw_PUcleaned      = -9999.;
   ele1_scEtaWidth_PUcleaned  = -9999.;
   ele1_scPhiWidth_PUcleaned  = -9999.;
   ele1_fCorrection_PUcleaned = -9999.;
  }
  else  {
   ele1_scERaw_PUcleaned     = cleanedSC.energy();
   ele1_scEtaWidth_PUcleaned = cleanedSC.etaWidth();
   ele1_scPhiWidth_PUcleaned = cleanedSC.phiWidth();
  }


  ele1_fEta    = scRef->energy()/scRef->rawEnergy();
  ele1_tkP     = electron.trackMomentumAtVtx().R();
  ele1_tkPt    = electron.trackMomentumAtVtx().Rho();
  ele1_fbrem   = electron.fbrem();
  ele1_e5x5    = electron.e5x5();
  ele1_eSeedBC = (scRef->seed())->energy();
  ele1_es      = scRef->preshowerEnergy();
    
  float E3x3 = 0;
    
  if(electron.isEB()) {
   E3x3 = EcalClusterTools::e3x3( *scRef, theBarrelEcalRecHits, topology);
  }
  if(electron.isEE()) {
   E3x3 = EcalClusterTools::e3x3( *scRef, theEndcapEcalRecHits, topology);
  }

  ele1_e3x3    = E3x3;
  ele1_scNxtal = scRef->hitsAndFractions().size();
  ele1_bcN     = electron.basicClustersSize();

  float energy = 0.;
  int ieta     = 0;
  int iphi     = 0;
  int ix       = 0;
  int iy       = 0;
  int zside    = 0;
  float seedICConstant = -1.;
  float seedLaserAlpha = -1.;
  float seedLaserCorrection = -1.;
    
  if(electron.isEB()) {
   if( printOut )
    std::cout << "*** EB ***" << std::endl;

   std::pair<DetId, float> id = EcalClusterTools::getMaximum(seedCluster->hitsAndFractions(), theBarrelEcalRecHits);
   EcalRecHitCollection::const_iterator it = theBarrelEcalRecHits->find(id.first);

   if( it != theBarrelEcalRecHits->end() ) {
    const EcalRecHit& rh = (*it);
    energy = rh.energy();
    ieta = (EBDetId(id.first)).ieta();
    iphi = (EBDetId(id.first)).iphi();
    ix = -999;
    iy = -999;
    zside = 0;
   }

   // intercalib constant
   EcalIntercalibConstantMap::const_iterator ICMapIt = ICMap.find(EBDetId(id.first));
   if( ICMapIt != ICMap.end() ) {
    seedICConstant = *ICMapIt;
   }
      // laser alphas
   EcalLaserAlphaMap::const_iterator italpha = theEcalLaserAlphaMap->find(id.first);
   if( italpha != theEcalLaserAlphaMap->end() )
    seedLaserAlpha = (*italpha);
      
      // laser correction
   seedLaserCorrection = theLaser->getLaserCorrection(EBDetId(id.first), iEvent.time());
  }
    
  else {
   if( printOut )
    std::cout << "*** EE ***" << std::endl;      
   std::pair<DetId, float> id = EcalClusterTools::getMaximum(seedCluster->hitsAndFractions(), theEndcapEcalRecHits);
      
   EcalRecHitCollection::const_iterator it = theEndcapEcalRecHits->find(id.first);
      
   if( it != theEndcapEcalRecHits->end() ) {
    const EcalRecHit& rh = (*it);
    energy = rh.energy();
    ix = (EEDetId(id.first)).ix();
    iy = (EEDetId(id.first)).iy();
    ieta = -999;
    iphi = -999;
    zside = (EEDetId(id.first)).zside();
   }
      
   // intercalib constant
   EcalIntercalibConstantMap::const_iterator ICMapIt = ICMap.find(EEDetId(id.first));
   if( ICMapIt != ICMap.end() )
    seedICConstant = *ICMapIt;
      
   // laser alphas
   EcalLaserAlphaMap::const_iterator italpha = theEcalLaserAlphaMap->find(id.first);
   if( italpha != theEcalLaserAlphaMap->end() )
    seedLaserAlpha = (*italpha);
      
   // laser correction
   seedLaserCorrection = theLaser->getLaserCorrection(EEDetId(id.first), iEvent.time());
  }
    
  ele1_seedE = energy;
  ele1_seedLaserAlpha  = seedLaserAlpha;
  ele1_seedLaserCorr   = seedLaserCorrection;
  ele1_seedICConstant  = seedICConstant;
  ele1_seedIeta = ieta;
  ele1_seedIphi = iphi;
  ele1_seedIx = ix;
  ele1_seedIy = iy;
  ele1_seedZside = zside;
  ele1_EOverP = electron.eSuperClusterOverP();
           
  // rechit variables
  int numRecHit = 0;
  float sumRecHitE = 0.;
  float sumLaserCorrectionRecHitE = 0.;
  float sumRecHitE5x5 = 0.;
  float sumLaserCorrectionRecHitE5x5 = 0.;
  float sumRecHitE3x3 = 0.;
  float sumLaserCorrectionRecHitE3x3 = 0.;
    
  const std::vector<std::pair<DetId,float> >& hits = scRef->hitsAndFractions();
    
  if( printOut ){
   std::cout << "runId: " << iEvent.id().run()
     << std::fixed
     << " electron eta: " << std::setprecision(2) << std::setw(5) << electron.eta()
     << " electron phi: " << std::setprecision(2) << std::setw(5) << electron.phi()
     << " SC energy: " << std::setprecision(2) << std::setw(6) << scRef -> energy()
     << std::endl;
  }


  if(saveRecHitMatrix_) {
   float theLaserCorrection = -1.;
   float theICCorrection = -1.;
      
   if(electron.isEB()){
    DetId seedId = EcalClusterTools::getMaximum( scRef->hitsAndFractions(), theBarrelEcalRecHits ).first;
    //save the matrix in case of eleSeed
    std::vector<DetId> rectangle = EcalClusterTools::matrixDetId(topology, seedId, -9, 9, -9, 9);

    int it = 0;
    for(std::vector<DetId>::const_iterator itr = rectangle.begin(); itr != rectangle.end(); ++itr){
     ++it;
     EcalRecHitCollection::const_iterator itrRecHit = theBarrelEcalRecHits->find(*itr) ;
     if(itrRecHit == theBarrelEcalRecHits->end()) continue;
          
          // fill recHit variables
     EBDetId barrelId(*itr);
     EcalTrigTowerDetId towerId = barrelId.tower();
          
          // laser correction
     theLaserCorrection = theLaser->getLaserCorrection(barrelId, iEvent.time());
          // IC correction
     EcalIntercalibConstantMap::const_iterator ICMapIt = ICMap.find(barrelId);
     theICCorrection = *ICMapIt;
          
     int SRFlag = 3;
     std::vector<EcalTrigTowerDetId>::iterator TTIdListIt = std::find(TTIdList.begin(),TTIdList.end(),towerId);
     if( TTIdListIt == TTIdList.end() ) SRFlag = 1;
          
     bool digiFound = false;
     for(EBDigiCollection::const_iterator digiItr = ebDigis->begin(); digiItr != ebDigis->end(); ++digiItr)
     {
      if(digiItr->id() != barrelId )continue;
      digiFound = true;
      EcalDataFrame df = *digiItr;
      for(int iSample = 0; iSample < 10; ++iSample)
       ele1_recHitMatrix_samples.push_back(df.sample(iSample).adc());
     }
     if( digiFound == false ) continue;
                    
     ele1_recHitMatrix_E.push_back(itrRecHit->energy());
     ele1_recHitMatrix_flag.push_back(SRFlag*1000+itrRecHit->recoFlag());
     ele1_recHitMatrix_hashedIndex.push_back(barrelId.hashedIndex());
     ele1_recHitMatrix_ietaORix.push_back(barrelId.ieta());
     ele1_recHitMatrix_iphiORiy.push_back(barrelId.iphi());
     ele1_recHitMatrix_zside.push_back(0);
     ele1_recHitMatrix_laserCorrection.push_back(theLaserCorrection);
     ele1_recHitMatrix_ICConstant.push_back(theICCorrection);
    }
   }
      
   else {
    DetId seedId = EcalClusterTools::getMaximum( scRef->hitsAndFractions(), theEndcapEcalRecHits ).first;
//save the matrix in case of eleSeed
    std::vector<DetId> rectangle = EcalClusterTools::matrixDetId(topology, seedId, -9, 9, -9, 9);

    for(std::vector<DetId>::const_iterator itr = rectangle.begin(); itr != rectangle.end(); ++itr) {
     EcalRecHitCollection::const_iterator itrRecHit = theEndcapEcalRecHits->find(*itr) ;
     if(itrRecHit == theEndcapEcalRecHits->end()) continue;
          
          // fill recHit variables
     EEDetId endcapId(*itr);
     EcalScDetId scId = endcapId.sc();

          // laser correction
     theLaserCorrection = theLaser->getLaserCorrection(endcapId, iEvent.time());
          // IC correction
     EcalIntercalibConstantMap::const_iterator ICMapIt = ICMap.find(endcapId);
     theICCorrection = *ICMapIt;
          
     int SRFlag = 3;
     std::vector<EcalScDetId>::iterator SCIdListIt = std::find(SCIdList.begin(),SCIdList.end(),scId);
     if( SCIdListIt == SCIdList.end() ) SRFlag = 1;
          
     bool digiFound = false;
     for(EEDigiCollection::const_iterator digiItr = eeDigis->begin(); digiItr != eeDigis->end(); ++digiItr) {
      if(digiItr->id() != endcapId) continue;
      digiFound = true;
      EcalDataFrame df = *digiItr;
      for(int iSample = 0; iSample < 10; ++iSample)
       ele1_recHitMatrix_samples.push_back(df.sample(iSample).adc());
     }
     if( digiFound == false ) continue;
          
     ele1_recHitMatrix_E.push_back(itrRecHit->energy());
     ele1_recHitMatrix_flag.push_back(SRFlag*1000+itrRecHit->recoFlag());
     ele1_recHitMatrix_hashedIndex.push_back(endcapId.hashedIndex());
     ele1_recHitMatrix_ietaORix.push_back(endcapId.ix());
     ele1_recHitMatrix_iphiORiy.push_back(endcapId.iy());
     ele1_recHitMatrix_zside.push_back(0);
     ele1_recHitMatrix_laserCorrection.push_back(theLaserCorrection);
     ele1_recHitMatrix_ICConstant.push_back(theICCorrection);
    }
   }
  }
    
  for(std::vector<std::pair<DetId,float> >::const_iterator rh = hits.begin(); rh!=hits.end(); ++rh) {
   float rhLaserCorrection = 1.;
   float rhICCorrection = 1.;
   float theLaserCorrection = -1.;
   float theICCorrection = -1.;
   float theAlpha = -1.;
            
   if ((*rh).first.subdetId()== EcalBarrel) {
    EBRecHitCollection::const_iterator itrechit = theBarrelEcalRecHits->find((*rh).first);
    if (itrechit==theBarrelEcalRecHits->end()) continue;
    EBDetId barrelId (itrechit->id ());
    ++numRecHit;
                      
        // laser correction
    theLaserCorrection = theLaser->getLaserCorrection(barrelId, iEvent.time());
    if ( applyCorrections_ ) rhLaserCorrection = theLaserCorrection;
        // IC correction
    EcalIntercalibConstantMap::const_iterator ICMapIt = ICMap.find(barrelId);
    theICCorrection = *ICMapIt;
    if ( applyCorrections_ ) rhICCorrection = theICCorrection;
        // Alpha
    EcalLaserAlphaMap::const_iterator italpha = theEcalLaserAlphaMap->find(itrechit->id());
    if( italpha != theEcalLaserAlphaMap->end() )
    theAlpha = (*italpha);
    sumRecHitE += itrechit->energy() * rhLaserCorrection * rhICCorrection ;
    sumLaserCorrectionRecHitE += theLaserCorrection * itrechit->energy() * rhLaserCorrection * rhICCorrection;
        // check if rh is inside the 5x5 matrix
    if ( fabs(barrelId.ieta() - ele1_seedIeta) < 3 && fabs(barrelId.iphi() - ele1_seedIphi) < 3 ) {
     sumRecHitE5x5 += itrechit->energy() * rhLaserCorrection * rhICCorrection ;
     sumLaserCorrectionRecHitE5x5 += theLaserCorrection * itrechit->energy() * rhLaserCorrection * rhICCorrection;
    }
        // check if rh is inside the 3x3 matrix
    if ( fabs(barrelId.ieta() - ele1_seedIeta) < 1 && fabs(barrelId.iphi() - ele1_seedIphi) < 1 ) {
     sumRecHitE3x3 += itrechit->energy() * rhLaserCorrection * rhICCorrection ;
     sumLaserCorrectionRecHitE3x3 += theLaserCorrection * itrechit->energy() * rhLaserCorrection * rhICCorrection ;
    }
        // fill recHit variables
    ele1_recHit_E.push_back(itrechit->energy() * rhLaserCorrection);
    ele1_recHit_flag.push_back(itrechit->recoFlag());
    ele1_recHit_hashedIndex.push_back(barrelId.hashedIndex());
    ele1_recHit_ietaORix.push_back(barrelId.ieta());
    ele1_recHit_iphiORiy.push_back(barrelId.iphi());
    ele1_recHit_zside.push_back(0);
    ele1_recHit_laserCorrection.push_back(theLaserCorrection);
    ele1_recHit_ICConstant.push_back(theICCorrection);
    ele1_recHit_Alpha.push_back(theAlpha);
        
    if( printOut && itrechit->energy() > 1. ) {
     std::cout << std::fixed
       << " recHitLC: " << std::setprecision(6) << std::setw(8) << theLaserCorrection
       << " recHitIC: " << std::setprecision(6) << std::setw(8) << theICCorrection
       << std::endl;
    }
   }
        
   if( (*rh).first.subdetId()== EcalEndcap ) {
    EERecHitCollection::const_iterator itrechit = theEndcapEcalRecHits->find((*rh).first);
    if (itrechit==theEndcapEcalRecHits->end()) continue;
    EEDetId endcapId (itrechit->id ());
    ++numRecHit;
        
        // laser correction
    theLaserCorrection = theLaser->getLaserCorrection(endcapId, iEvent.time());
    if ( applyCorrections_ ) rhLaserCorrection = theLaserCorrection;
        // IC correction
    EcalIntercalibConstantMap::const_iterator ICMapIt = ICMap.find(endcapId);
    theICCorrection = *ICMapIt;
    if ( applyCorrections_ ) rhICCorrection = theICCorrection;
        
    sumRecHitE += itrechit->energy() * rhLaserCorrection * rhICCorrection ;
    sumLaserCorrectionRecHitE += theLaserCorrection * itrechit->energy() * rhLaserCorrection * rhICCorrection;
    // check if rh is inside the 5x5 matrix
    if ( fabs(endcapId.ix() - ele1_seedIx) < 3 && fabs(endcapId.iy() - ele1_seedIy) < 3 ) {
     sumRecHitE5x5 += itrechit->energy() * rhLaserCorrection * rhICCorrection ;
     sumLaserCorrectionRecHitE5x5 += theLaserCorrection * itrechit->energy() * rhLaserCorrection * rhICCorrection;
    }
    // check if rh is inside the 3x3 matrix
    if ( fabs(endcapId.ix() - ele1_seedIx) < 1 && fabs(endcapId.iy() - ele1_seedIy) < 1 ) {
     sumRecHitE3x3 += itrechit->energy() * rhLaserCorrection * rhICCorrection ;
     sumLaserCorrectionRecHitE3x3 += theLaserCorrection * itrechit->energy() * rhLaserCorrection * rhICCorrection ;
    }
    // fill recHit variables
    ele1_recHit_E.push_back(itrechit->energy() * rhLaserCorrection);
    ele1_recHit_flag.push_back(itrechit->recoFlag());
    ele1_recHit_hashedIndex.push_back(endcapId.hashedIndex());
    ele1_recHit_ietaORix.push_back(endcapId.ix());
    ele1_recHit_iphiORiy.push_back(endcapId.iy());
    ele1_recHit_zside.push_back(endcapId.zside());
    ele1_recHit_laserCorrection.push_back(theLaserCorrection);
    ele1_recHit_ICConstant.push_back(theICCorrection);
        
    if( printOut && itrechit->energy() > 1. ){
     std::cout << std::fixed
       << " recHitLC: " << std::setprecision(6) << std::setw(8) << theLaserCorrection
       << " recHitIC: " << std::setprecision(6) << std::setw(8) << theICCorrection
       << std::endl;
    }
   }
      
  }
    
  ele1_nRecHits = numRecHit;
    
  ele1_scLaserCorr = sumLaserCorrectionRecHitE/sumRecHitE;
  if ( applyCorrections_ ) ele1_scE = scRef->energy()*ele1_fEta*sumRecHitE;
  else ele1_scE = scRef->energy();
    
  ele1_5x5LaserCorr = sumLaserCorrectionRecHitE5x5/sumRecHitE5x5;    
  ele1_3x3LaserCorr = sumLaserCorrectionRecHitE3x3/sumRecHitE3x3;
    
  /// add regression input variables
  reco::SuperClusterRef s = electron.superCluster();
  reco::CaloClusterPtr b = s->seed(); //seed basic cluster
  reco::CaloClusterPtr b2;
  reco::CaloClusterPtr bclast;
  reco::CaloClusterPtr bclast2;
  bool isbarrel = b->hitsAndFractions().at(0).first.subdetId()==EcalBarrel;
    
    
  ele1_eRegrInput_rawE = s->rawEnergy();
  ele1_eRegrInput_r9 = lazyTools.e3x3(*b)/s->rawEnergy();
  ele1_eRegrInput_eta = s->eta();
  ele1_eRegrInput_phi = s->phi();
  ele1_eRegrInput_r25 = lazyTools.e5x5(*b)/s->rawEnergy();
  ele1_eRegrInput_hoe = electron.hcalOverEcal();
  ele1_eRegrInput_etaW = s->etaWidth() ;
  ele1_eRegrInput_phiW = s->phiWidth() ;
  ele1_eRegrInput_SCsize = s->clustersSize() ;
  ele1_eRegrInput_rho = *rhoHandle;
  ele1_eRegrInput_nPV = hVertexProduct->size();
    
  //seed basic cluster variables
  double bemax = lazyTools.eMax(*b);
  double be2nd = lazyTools.e2nd(*b);
  double betop = lazyTools.eTop(*b);
  double bebottom = lazyTools.eBottom(*b);
  double beleft = lazyTools.eLeft(*b);
  double beright = lazyTools.eRight(*b);
    
  double be2x5max = lazyTools.e2x5Max(*b);
  double be2x5top = lazyTools.e2x5Top(*b);
  double be2x5bottom = lazyTools.e2x5Bottom(*b);
  double be2x5left = lazyTools.e2x5Left(*b);
  double be2x5right = lazyTools.e2x5Right(*b);
     
  ele1_eRegrInput_Deta_bC_sC     = b->eta()-s->eta();
  ele1_eRegrInput_Dphi_bC_sC     = reco::deltaPhi(b->phi(),s->phi());
  ele1_eRegrInput_bCE_Over_sCE   = b->energy()/b->energy();
  ele1_eRegrInput_e3x3_Over_bCE  = lazyTools.e3x3(*b)/b->energy();
  ele1_eRegrInput_e5x5_Over_bCE  = lazyTools.e5x5(*b)/b->energy();
  ele1_eRegrInput_sigietaieta_bC1   = sqrt(lazyTools.localCovariances(*b)[0]);
  ele1_eRegrInput_sigiphiiphi_bC1   = sqrt(lazyTools.localCovariances(*b)[2]);
  ele1_eRegrInput_sigietaiphi_bC1   = lazyTools.localCovariances(*b)[1];
  ele1_eRegrInput_bEMax_Over_bCE    = bemax/b->energy();
  ele1_eRegrInput_bE2nd_Over_bCE    = be2nd/b->energy();
  ele1_eRegrInput_bEtop_Over_bCE    = betop/b->energy();
  ele1_eRegrInput_bEbot_Over_bCE    = bebottom/b->energy();
  ele1_eRegrInput_bEleft_Over_bCE   = beleft/b->energy();
  ele1_eRegrInput_bEright_Over_bCE  = beright/b->energy();
  ele1_eRegrInput_be2x5max_Over_bCE = be2x5max/b->energy();
  ele1_eRegrInput_be2x5top_Over_bCE = be2x5top/b->energy();
  ele1_eRegrInput_be2x5bottom_Over_bCE = be2x5bottom/b->energy();
  ele1_eRegrInput_be2x5left_Over_bCE   = be2x5left/b->energy();
  ele1_eRegrInput_be2x5right_Over_bCE  = be2x5right/b->energy();
    
    
    
  if( isbarrel ) {
      // seed cluster
   float betacry, bphicry, bthetatilt, bphitilt;
   int bieta, biphi;
   EcalClusterLocal _ecalLocal;
   _ecalLocal.localCoordsEB(*b,iSetup,betacry,bphicry,bieta,biphi,bthetatilt,bphitilt);
      
   ele1_eRegrInput_seedbC_eta = bieta;
   ele1_eRegrInput_seedbC_phi = biphi;
   ele1_eRegrInput_seedbC_eta_p5 = bieta%5;
   ele1_eRegrInput_seedbC_phi_p2 = biphi%2;
   ele1_eRegrInput_seedbC_bieta = (TMath::Abs(bieta)<=25)*(bieta%25) + (TMath::Abs(bieta)>25)*((bieta-25*TMath::Abs(bieta)/bieta)%20);
   ele1_eRegrInput_seedbC_phi_p20 = biphi%20;
   ele1_eRegrInput_seedbC_etacry = betacry;
   ele1_eRegrInput_seedbC_phicry = bphicry;
   ele1_eRegrInput_ESoSC = -99. ;
  }
  else {
   ele1_eRegrInput_ESoSC = s->preshowerEnergy()/s->rawEnergy();
   ele1_eRegrInput_seedbC_eta = -99.;
   ele1_eRegrInput_seedbC_phi = -99.;
   ele1_eRegrInput_seedbC_eta_p5 = -99.;
   ele1_eRegrInput_seedbC_phi_p2 = -99.;
   ele1_eRegrInput_seedbC_bieta = -99.;
   ele1_eRegrInput_seedbC_phi_p20 = -99.;
   ele1_eRegrInput_seedbC_etacry = -99.;
   ele1_eRegrInput_seedbC_phicry =-99.;
  }

  if(saveFbrem_) {

   reco::GsfTrackRef eleTrack = electron.gsfTrack();
   reco::TrackRef eleNTrack = electron.closestTrack();
   GlobalPoint outPos(eleTrack->extra()->outerPosition().x(), eleTrack->extra()->outerPosition().y(), eleTrack->extra()->outerPosition().z());
   GlobalPoint innPos(eleTrack->extra()->innerPosition().x(), eleTrack->extra()->innerPosition().y(), eleTrack->extra()->innerPosition().z());
   std::vector<reco::GsfTangent> eleTangent = eleTrack->gsfExtra()->tangents();
   int numberOfValidHits_Trk = 0;
   if(!eleNTrack.isNull())
    for(unsigned int trkNH = 0; trkNH < eleNTrack->extra()->recHitsSize(); ++trkNH){
    if(!eleNTrack->extra()->recHit(trkNH)->isValid()) continue;
    DetId id = eleNTrack->extra()->recHit(trkNH)->geographicalId();
    const GeomDet* det = pDD->idToDet(id);

    if(eleTrack->extra()->seedRef().isNull()) continue;
    edm::RefToBase<TrajectorySeed> seed = eleTrack->extra()->seedRef();
    reco::ElectronSeedRef elseed = seed.castTo<reco::ElectronSeedRef>();
    TrajectoryStateOnSurface t = trajectoryStateTransform::transientState(elseed->startingState(), &(det->surface()), &(*theMagField));
    if(!t.isValid()) continue;

    GlobalPoint hitPos = t.globalPosition();
    if(hitPos.x() == ele1_inner_x && hitPos.y() == ele1_inner_y && hitPos.z() == ele1_inner_z){
     ele1_inner_p = (sqrt(eleTrack->extra()->innerMomentum().Mag2()) );
     ele1_inner_x = innPos.x();
     ele1_inner_y = innPos.y();
     ele1_inner_z = innPos.z();
    }
    if(hitPos.x() == ele1_outer_x && hitPos.y() == ele1_outer_y && hitPos.z() == ele1_outer_z){
     ele1_outer_p = (sqrt(eleTrack->extra()->outerMomentum().Mag2()) );
     ele1_outer_x = outPos.x();
     ele1_outer_y = outPos.y();
     ele1_outer_z = outPos.z();
    }
    for(unsigned int pp=0; pp<eleTangent.size(); ++pp ){
     GlobalPoint tangPos( eleTangent.at(pp).position().x(),
                          eleTangent.at(pp).position().y(),
                          eleTangent.at(pp).position().z() );
     if(hitPos.x() != tangPos.x() && hitPos.y() != tangPos.y() && hitPos.z() != tangPos.z()) continue;
     float tangMom = sqrt(eleTangent.at(pp).momentum().Mag2());
     ele1_tangent_p.push_back(tangMom);
     ele1_tangent_x.push_back(tangPos.x());
     ele1_tangent_y.push_back(tangPos.y());
     ele1_tangent_z.push_back(tangPos.z());
     ele1_tangent_dP.push_back(eleTangent.at(pp).deltaP().value());
     ele1_tangent_dPerr.push_back(eleTangent.at(pp).deltaP().error());
     ++numberOfValidHits_Trk;
    }
    }
    ele1_tangent_n = numberOfValidHits_Trk;
  }
 }

 if ( eleName == "ele2" ){
    
  ele2        = electron.p4();
  ele2_charge = electron.charge();
  ele2_p   = ele2.P();
  ele2_pt  = ele2.Pt();
  ele2_eta = ele2.eta();
  ele2_phi = ele2.phi();
    
  ele2_isEB        = electron.isEB();
  ele2_isEBEEGap   = electron.isEBEEGap();
  ele2_isEBEtaGap  = electron.isEBEtaGap();
  ele2_isEBPhiGap  = electron.isEBPhiGap();
  ele2_isEEDeeGap  = electron.isEEDeeGap();
  ele2_isEERingGap = electron.isEERingGap();

  ele2_idtype = -1 ;
  if(electron.electronID("fiducial")) ele2_idtype = 0 ;
  if(electron.electronID("loose"))    ele2_idtype = 1;
  if(electron.electronID("medium"))   ele2_idtype = 2;
  if(electron.electronID("tight"))    ele2_idtype = 3;
  if(electron.electronID("WP90PU"))   ele2_idtype = 4;
  if(electron.electronID("WP80PU"))   ele2_idtype = 5;
  if(electron.electronID("WP70PU"))   ele2_idtype = 6;
    
  ele2_isTrackerDriven = !(electron.ecalDriven());
  ele2_classification  = electron.classification();
  ele2_sigmaIetaIeta   = electron.sigmaIetaIeta();
  ele2_DphiIn          = electron.deltaPhiSuperClusterTrackAtVtx();
  ele2_DetaIn          = electron.deltaEtaSuperClusterTrackAtVtx();

  ele2_deltaEtaEleClusterTrackAtCalo  = electron.deltaEtaEleClusterTrackAtCalo();
  ele2_deltaEtaPhiClusterTrackAtCalo  = electron.deltaPhiEleClusterTrackAtCalo();
  ele2_deltaEtaSeedClusterTrackAtCalo = electron.deltaEtaSeedClusterTrackAtCalo();
  ele2_deltaPhiSeedClusterTrackAtCalo = electron.deltaPhiSeedClusterTrackAtCalo();
   
  ele2_HOverE  = electron.hadronicOverEm();
  ele2_ooemoop = (1.0/electron.ecalEnergy()-electron.eSuperClusterOverP()/electron.ecalEnergy());
    
  ele2_tkIso  = electron.dr03TkSumPt();
  ele2_emIso  = electron.dr03EcalRecHitSumEt();
  ele2_hadIso = electron.dr03HcalDepth1TowerSumEt()+electron.dr03HcalDepth2TowerSumEt();
    
  ele2_effAreaForIso = 0.;
    
  reco::GsfTrackRef eleTrack = electron.gsfTrack() ;
  ele2_dxy_PV = eleTrack->dxy (PVPoint_);
  ele2_dz_PV  = eleTrack->dz (PVPoint_);
  ele2_sigmaP = electron.corrections().trackMomentumError;
    
    
  reco::SuperClusterRef scRef = electron.superCluster();
  const edm::Ptr<reco::CaloCluster>& seedCluster = scRef->seed();
    
  double R = TMath::Sqrt(scRef->x()*scRef->x() + scRef->y()*scRef->y() +scRef->z()*scRef->z());
  double Rt = TMath::Sqrt(scRef->x()*scRef->x() + scRef->y()*scRef->y());
    
  edm::Handle< double > rhoHandle;
  iEvent.getByLabel(rhoTag_,rhoHandle);
    
  ele2_scERaw     = scRef->rawEnergy();
  ele2_scEtRaw    = scRef->rawEnergy()*(Rt/R);
  ele2_scEt       = scRef->energy()*(Rt/R);
  ele2_scEtaWidth = scRef->etaWidth();
  ele2_scPhiWidth = scRef->phiWidth();
  ele2_scE        = scRef->energy();
  ele2_scEta      = scRef->eta();
  ele2_scPhi      = scRef->phi();
    
  EcalClusterLocal ecalLocalCoord;
  float bcLocalEta, bcLocalPhi, bcThetatilt, bcPhitilt;
  int bcIeta, bcIphi;
  bcLocalEta = 0;
   
  if ( electron.isEB() )
   ecalLocalCoord.localCoordsEB(*seedCluster,iSetup,bcLocalEta,bcLocalPhi,bcIeta,bcIphi,bcThetatilt,bcPhitilt);
  if ( electron.isEE() )
   ecalLocalCoord.localCoordsEE(*seedCluster,iSetup,bcLocalEta,bcLocalPhi,bcIeta,bcIphi,bcThetatilt,bcPhitilt);
         
  ele2_scLocalEta=bcLocalEta;
  ele2_scLocalPhi=bcLocalPhi;
        
  // crack correction variables and local containment corrections
  EcalClusterCrackCorrection -> init(iSetup);
  EcalClusterLocalContCorrection -> init(iSetup);
  double crackcor = 1.;
  double localContCorr = 1.;
    
  for(reco::CaloCluster_iterator cIt = electron.superCluster()->clustersBegin();
      cIt != electron.superCluster()->clustersEnd(); ++cIt){
   const reco::CaloClusterPtr cc = *cIt;
   crackcor *= ( (electron.superCluster()->rawEnergy() + (*cIt)->energy()*(EcalClusterCrackCorrection->getValue(*cc)-1.)) / electron.superCluster()->rawEnergy() );
  }

  localContCorr = EcalClusterLocalContCorrection->getValue(*electron.superCluster(), 1) ;
    
  ele2_scCrackCorr     = crackcor;
  ele2_scLocalContCorr = localContCorr;
        
  reco::SuperCluster cleanedSC = cleaningTools.CleanedSuperCluster(0.02, *scRef, iEvent );
  reco::CaloClusterPtr myseed = (*scRef).seed();
  if ( !((myseed->seed()).rawId()) || (myseed->seed()).rawId()==0 ){
   ele2_scERaw_PUcleaned      = -9999.;
   ele2_scEtaWidth_PUcleaned  = -9999.;
   ele2_scPhiWidth_PUcleaned  = -9999.;
   ele2_fCorrection_PUcleaned = -9999.;
  }
  else
  {
   ele2_scERaw_PUcleaned     = cleanedSC.energy();
   ele2_scEtaWidth_PUcleaned = cleanedSC.etaWidth();
   ele2_scPhiWidth_PUcleaned = cleanedSC.phiWidth();
  }
    
    
  ele2_fEta  = scRef->energy()/scRef->rawEnergy();
  ele2_tkP   = electron.trackMomentumAtVtx().R();
  ele2_tkPt  = electron.trackMomentumAtVtx().Rho();
  ele2_fbrem = electron.fbrem();
  ele2_e5x5  = electron.e5x5();
  ele2_eSeedBC = (scRef->seed())->energy();
  ele2_es    = scRef->preshowerEnergy();
    
  float E3x3 = 0;    
  if( electron.isEB() ){
   E3x3 = EcalClusterTools::e3x3( *scRef, theBarrelEcalRecHits, topology);
  }
    
  if ( electron.isEE() ){
   E3x3 = EcalClusterTools::e3x3( *scRef, theEndcapEcalRecHits, topology);
  }
    
  ele2_e3x3    = E3x3;
  ele2_scNxtal = scRef->hitsAndFractions().size();
  ele2_bcN = electron.basicClustersSize();
    
  float energy=0.;
  int ieta=0;
  int iphi=0;
  int ix=0;
  int iy=0;
  int zside=0;
  float seedICConstant = -1.;
  float seedLaserAlpha = -1.;
  float seedLaserCorrection = -1.;
    
  if(electron.isEB()){

   std::pair<DetId, float> id = EcalClusterTools::getMaximum(seedCluster->hitsAndFractions(), theBarrelEcalRecHits);
   EcalRecHitCollection::const_iterator it = theBarrelEcalRecHits->find(id.first);
      
   if(it != theBarrelEcalRecHits->end()){
    const EcalRecHit& rh = (*it);
    energy = rh.energy();
    ieta = (EBDetId(id.first)).ieta();
    iphi = (EBDetId(id.first)).iphi();
    ix = -999;
    iy = -999;
    zside = 0;
   }
      
   // intercalib constant
   EcalIntercalibConstantMap::const_iterator ICMapIt = ICMap.find(EBDetId(id.first));
   if( ICMapIt != ICMap.end() )seedICConstant = *ICMapIt;
      
   // laser alphas
   EcalLaserAlphaMap::const_iterator italpha = theEcalLaserAlphaMap->find(id.first);
   if( italpha != theEcalLaserAlphaMap->end() )seedLaserAlpha = (*italpha);
      
   // laser correction
   seedLaserCorrection = theLaser->getLaserCorrection(EBDetId(id.first), iEvent.time());
  }
    
  else{

   std::pair<DetId, float> id = EcalClusterTools::getMaximum(seedCluster->hitsAndFractions(), theEndcapEcalRecHits);
   EcalRecHitCollection::const_iterator it = theEndcapEcalRecHits->find(id.first);
      
   if(it != theEndcapEcalRecHits->end()){
    const EcalRecHit& rh = (*it);
    energy = rh.energy();
    ix = (EEDetId(id.first)).ix();
    iy = (EEDetId(id.first)).iy();
    ieta = -999;
    iphi = -999;
    zside = (EEDetId(id.first)).zside();
   }
      
   // intercalib constant
   EcalIntercalibConstantMap::const_iterator ICMapIt = ICMap.find(EEDetId(id.first));
   if( ICMapIt != ICMap.end() )
    seedICConstant = *ICMapIt;
      
   // laser alphas
   EcalLaserAlphaMap::const_iterator italpha = theEcalLaserAlphaMap->find(id.first);
   if( italpha != theEcalLaserAlphaMap->end() )
    seedLaserAlpha = (*italpha);
      
      // laser correction
   seedLaserCorrection = theLaser->getLaserCorrection(EEDetId(id.first), iEvent.time());
  }
    
  ele2_seedE          = energy;
  ele2_seedLaserAlpha = seedLaserAlpha;
  ele2_seedLaserCorr  = seedLaserCorrection;
  ele2_seedICConstant = seedICConstant;
  ele2_seedIeta = ieta;
  ele2_seedIphi = iphi;
  ele2_seedIx = ix;
  ele2_seedIy = iy;
  ele2_seedZside = zside;
  ele2_EOverP = electron.eSuperClusterOverP();
           
  // rechit variables
  int numRecHit = 0;
  float sumRecHitE = 0.;
  float sumLaserCorrectionRecHitE = 0.;
  float sumRecHitE5x5 = 0.;
  float sumLaserCorrectionRecHitE5x5 = 0.;
  float sumRecHitE3x3 = 0.;
  float sumLaserCorrectionRecHitE3x3 = 0.;
    
  bool printOut = false;
  const std::vector<std::pair<DetId,float> >& hits = scRef->hitsAndFractions();
    
  if( printOut ){
     std::cout << "runId: " << iEvent.id().run()
     << std::fixed
     << " electron eta: " << std::setprecision(2) << std::setw(5) << electron.eta()
     << " electron phi: " << std::setprecision(2) << std::setw(5) << electron.phi()
     << " SC energy: " << std::setprecision(2) << std::setw(6) << scRef -> energy()
     << std::endl;
  }

  if(saveRecHitMatrix_){
   float theLaserCorrection = -1.;
   float theICCorrection = -1.;
      
   if(electron.isEB()){
    DetId seedId = EcalClusterTools::getMaximum( scRef->hitsAndFractions(), theBarrelEcalRecHits ).first;
    std::vector<DetId> rectangle = EcalClusterTools::matrixDetId(topology, seedId, -9, 9, -9, 9);

    for(std::vector<DetId>::const_iterator itr = rectangle.begin(); itr != rectangle.end(); ++itr){
     EcalRecHitCollection::const_iterator itrRecHit = theBarrelEcalRecHits->find(*itr) ;
     if(itrRecHit == theBarrelEcalRecHits->end()) continue;
          
     // fill recHit variables
     EBDetId barrelId(*itr);
     EcalTrigTowerDetId towerId = barrelId.tower();
          
     // laser correction
     theLaserCorrection = theLaser->getLaserCorrection(barrelId, iEvent.time());
     // IC correction
     EcalIntercalibConstantMap::const_iterator ICMapIt = ICMap.find(barrelId);
     theICCorrection = *ICMapIt;
          
     int SRFlag = 3;
     std::vector<EcalTrigTowerDetId>::iterator TTIdListIt = std::find(TTIdList.begin(),TTIdList.end(),towerId);
     if( TTIdListIt == TTIdList.end() ) SRFlag = 1;
          
     bool digiFound = false;
     for(EBDigiCollection::const_iterator digiItr = ebDigis->begin(); digiItr != ebDigis->end(); ++digiItr){
      if(digiItr->id() != barrelId )continue;
      digiFound = true;
      EcalDataFrame df = *digiItr;
      for(int iSample = 0; iSample < 10; ++iSample)
       ele2_recHitMatrix_samples.push_back(df.sample(iSample).adc());
     }
     if( digiFound == false ) continue;
          
     ele2_recHitMatrix_E.push_back(itrRecHit->energy());
     ele2_recHitMatrix_flag.push_back(SRFlag*1000+itrRecHit->recoFlag());
     ele2_recHitMatrix_hashedIndex.push_back(barrelId.hashedIndex());
     ele2_recHitMatrix_ietaORix.push_back(barrelId.ieta());
     ele2_recHitMatrix_iphiORiy.push_back(barrelId.iphi());
     ele2_recHitMatrix_zside.push_back(0);
     ele2_recHitMatrix_laserCorrection.push_back(theLaserCorrection);
     ele2_recHitMatrix_ICConstant.push_back(theICCorrection);
          
    }
   }
      
   else{
    DetId seedId = EcalClusterTools::getMaximum( scRef->hitsAndFractions(), theEndcapEcalRecHits ).first;
    std::vector<DetId> rectangle = EcalClusterTools::matrixDetId(topology, seedId, -9, 9, -9, 9);

    for(std::vector<DetId>::const_iterator itr = rectangle.begin(); itr != rectangle.end(); ++itr){
     EcalRecHitCollection::const_iterator itrRecHit = theEndcapEcalRecHits->find(*itr) ;
     if(itrRecHit == theEndcapEcalRecHits->end()) continue;
          
     // fill recHit variables
     EEDetId endcapId(*itr);
     EcalScDetId scId = endcapId.sc();
          
     // laser correction
     theLaserCorrection = theLaser->getLaserCorrection(endcapId, iEvent.time());
     // IC correction
     EcalIntercalibConstantMap::const_iterator ICMapIt = ICMap.find(endcapId);
     theICCorrection = *ICMapIt;
          
     int SRFlag = 3;
     std::vector<EcalScDetId>::iterator SCIdListIt = std::find(SCIdList.begin(),SCIdList.end(),scId);
     if( SCIdListIt == SCIdList.end() ) SRFlag = 1;
          
     bool digiFound = false;
     for(EEDigiCollection::const_iterator digiItr = eeDigis->begin(); digiItr != eeDigis->end(); ++digiItr)
     {
      if(digiItr->id() != endcapId) continue;
      digiFound = true;
      EcalDataFrame df = *digiItr;
      for(int iSample = 0; iSample < 10; ++iSample)
       ele2_recHitMatrix_samples.push_back(df.sample(iSample).adc());
     }
     if( digiFound == false ) continue;
          
     ele2_recHitMatrix_E.push_back(itrRecHit->energy());
     ele2_recHitMatrix_flag.push_back(SRFlag*1000+itrRecHit->recoFlag());
     ele2_recHitMatrix_hashedIndex.push_back(endcapId.hashedIndex());
     ele2_recHitMatrix_ietaORix.push_back(endcapId.ix());
     ele2_recHitMatrix_iphiORiy.push_back(endcapId.iy());
     ele2_recHitMatrix_zside.push_back(0);
     ele2_recHitMatrix_laserCorrection.push_back(theLaserCorrection);
     ele2_recHitMatrix_ICConstant.push_back(theICCorrection);
          
    }
   }
  }   
    
  for(std::vector<std::pair<DetId,float> >::const_iterator rh = hits.begin(); rh!=hits.end(); ++rh){
   float rhLaserCorrection = 1.;
   float rhICCorrection = 1.;
   float theLaserCorrection = -1.;
   float theICCorrection = -1.;
   float theAlpha = -1.;
            
   if ((*rh).first.subdetId()== EcalBarrel){
    EBRecHitCollection::const_iterator itrechit = theBarrelEcalRecHits->find((*rh).first);
    if (itrechit==theBarrelEcalRecHits->end()) continue;
    EBDetId barrelId (itrechit->id ());
    ++numRecHit;
                      
    // laser correction
    theLaserCorrection = theLaser->getLaserCorrection(barrelId, iEvent.time());
    if ( applyCorrections_ ) rhLaserCorrection = theLaserCorrection;
    // IC correction
    EcalIntercalibConstantMap::const_iterator ICMapIt = ICMap.find(barrelId);
    theICCorrection = *ICMapIt;
    if ( applyCorrections_ ) rhICCorrection = theICCorrection;
    // Alpha
    EcalLaserAlphaMap::const_iterator italpha = theEcalLaserAlphaMap->find(itrechit->id());
    if( italpha != theEcalLaserAlphaMap->end() )
     theAlpha = (*italpha);
    sumRecHitE += itrechit->energy() * rhLaserCorrection * rhICCorrection ;
    sumLaserCorrectionRecHitE += theLaserCorrection * itrechit->energy() * rhLaserCorrection * rhICCorrection;
    // check if rh is inside the 5x5 matrix
    if ( fabs(barrelId.ieta() - ele2_seedIeta) < 3 && fabs(barrelId.iphi() - ele2_seedIphi) < 3 ) {
     sumRecHitE5x5 += itrechit->energy() * rhLaserCorrection * rhICCorrection ;
     sumLaserCorrectionRecHitE5x5 += theLaserCorrection * itrechit->energy() * rhLaserCorrection * rhICCorrection;
    }
    // check if rh is inside the 3x3 matrix
    if ( fabs(barrelId.ieta() - ele2_seedIeta) < 1 && fabs(barrelId.iphi() - ele2_seedIphi) < 1 ) {
     sumRecHitE3x3 += itrechit->energy() * rhLaserCorrection * rhICCorrection ;
     sumLaserCorrectionRecHitE3x3 += theLaserCorrection * itrechit->energy() * rhLaserCorrection * rhICCorrection ;
    }
    // fill recHit variables
    ele2_recHit_E.push_back(itrechit->energy() * rhLaserCorrection);
    ele2_recHit_flag.push_back(itrechit->recoFlag());
    ele2_recHit_hashedIndex.push_back(barrelId.hashedIndex());
    ele2_recHit_ietaORix.push_back(barrelId.ieta());
    ele2_recHit_iphiORiy.push_back(barrelId.iphi());
    ele2_recHit_zside.push_back(0);
    ele2_recHit_laserCorrection.push_back(theLaserCorrection);
    ele2_recHit_ICConstant.push_back(theICCorrection);
    ele2_recHit_Alpha.push_back(theAlpha);
        
    if( printOut && itrechit->energy() > 1. ){
     std::cout << std::fixed
       << " recHitLC: " << std::setprecision(6) << std::setw(8) << theLaserCorrection
       << " recHitIC: " << std::setprecision(6) << std::setw(8) << theICCorrection
       << std::endl;
    }
   }
        
   if ((*rh).first.subdetId()== EcalEndcap){
    EERecHitCollection::const_iterator itrechit = theEndcapEcalRecHits->find((*rh).first);
    if (itrechit==theEndcapEcalRecHits->end()) continue;
    EEDetId endcapId (itrechit->id ());
    ++numRecHit;
          
    // laser correction
    theLaserCorrection = theLaser->getLaserCorrection(endcapId, iEvent.time());
    if ( applyCorrections_ ) rhLaserCorrection = theLaserCorrection;
    // IC correction
    EcalIntercalibConstantMap::const_iterator ICMapIt = ICMap.find(endcapId);
    theICCorrection = *ICMapIt;
    if ( applyCorrections_ ) rhICCorrection = theICCorrection;
        
    sumRecHitE += itrechit->energy() * rhLaserCorrection * rhICCorrection ;
    sumLaserCorrectionRecHitE += theLaserCorrection * itrechit->energy() * rhLaserCorrection * rhICCorrection;
    // check if rh is inside the 5x5 matrix
    if ( fabs(endcapId.ix() - ele2_seedIx) < 3 && fabs(endcapId.iy() - ele2_seedIy) < 3 ) {
     sumRecHitE5x5 += itrechit->energy() * rhLaserCorrection * rhICCorrection ;
     sumLaserCorrectionRecHitE5x5 += theLaserCorrection * itrechit->energy() * rhLaserCorrection * rhICCorrection;
    }
    // check if rh is inside the 3x3 matrix
    if ( fabs(endcapId.ix() - ele2_seedIx) < 1 && fabs(endcapId.iy() - ele2_seedIy) < 1 ) {
     sumRecHitE3x3 += itrechit->energy() * rhLaserCorrection * rhICCorrection ;
     sumLaserCorrectionRecHitE3x3 += theLaserCorrection * itrechit->energy() * rhLaserCorrection * rhICCorrection ;
    }
    // fill recHit variables
    ele2_recHit_E.push_back(itrechit->energy() * rhLaserCorrection);
    ele2_recHit_flag.push_back(itrechit->recoFlag());
    ele2_recHit_hashedIndex.push_back(endcapId.hashedIndex());
    ele2_recHit_ietaORix.push_back(endcapId.ix());
    ele2_recHit_iphiORiy.push_back(endcapId.iy());
    ele2_recHit_zside.push_back(endcapId.zside());
    ele2_recHit_laserCorrection.push_back(theLaserCorrection);
    ele2_recHit_ICConstant.push_back(theICCorrection);

    if( printOut && itrechit->energy() > 1. )
    {
     std::cout << std::fixed
       << " recHitLC: " << std::setprecision(6) << std::setw(8) << theLaserCorrection
       << " recHitIC: " << std::setprecision(6) << std::setw(8) << theICCorrection
       << std::endl;
    }
   }
  }
    
  ele2_nRecHits = numRecHit;
  ele2_scLaserCorr = sumLaserCorrectionRecHitE/sumRecHitE;
  if ( applyCorrections_ ) ele2_scE = scRef->energy()*ele2_fEta*sumRecHitE;
  else ele2_scE = scRef->energy();
    
  ele2_5x5LaserCorr = sumLaserCorrectionRecHitE5x5/sumRecHitE5x5;
  
  ele2_3x3LaserCorr = sumLaserCorrectionRecHitE3x3/sumRecHitE3x3;

  /// add regression input variables
  reco::SuperClusterRef s = electron.superCluster();
  reco::CaloClusterPtr b = s->seed(); //seed basic cluster
  reco::CaloClusterPtr b2;
  reco::CaloClusterPtr bclast;
  reco::CaloClusterPtr bclast2;
  bool isbarrel = b->hitsAndFractions().at(0).first.subdetId()==EcalBarrel;


  ele2_eRegrInput_rawE = s->rawEnergy();
  ele2_eRegrInput_r9   = lazyTools.e3x3(*b)/s->rawEnergy();
  ele2_eRegrInput_eta  = s->eta();
  ele2_eRegrInput_phi  = s->phi();
  ele2_eRegrInput_r25  = lazyTools.e5x5(*b)/s->rawEnergy();
  ele2_eRegrInput_hoe  = electron.hcalOverEcal();
  ele2_eRegrInput_etaW = s->etaWidth() ;
  ele2_eRegrInput_phiW = s->phiWidth() ;
  ele2_eRegrInput_SCsize = s->clustersSize() ;
  ele2_eRegrInput_rho  = *rhoHandle;
  ele2_eRegrInput_nPV  = hVertexProduct->size();
   
  //seed basic cluster variables
  double bemax    = lazyTools.eMax(*b);
  double be2nd    = lazyTools.e2nd(*b);
  double betop    = lazyTools.eTop(*b);
  double bebottom = lazyTools.eBottom(*b);
  double beleft   = lazyTools.eLeft(*b);
  double beright  = lazyTools.eRight(*b);

  double be2x5max    = lazyTools.e2x5Max(*b);
  double be2x5top    = lazyTools.e2x5Top(*b);
  double be2x5bottom = lazyTools.e2x5Bottom(*b);
  double be2x5left  = lazyTools.e2x5Left(*b);
  double be2x5right = lazyTools.e2x5Right(*b);
 
  ele2_eRegrInput_Deta_bC_sC = b->eta()-s->eta();
  ele2_eRegrInput_Dphi_bC_sC = reco::deltaPhi(b->phi(),s->phi());
  ele2_eRegrInput_bCE_Over_sCE = b->energy()/b->energy();
  ele2_eRegrInput_e3x3_Over_bCE = lazyTools.e3x3(*b)/b->energy();
  ele2_eRegrInput_e5x5_Over_bCE = lazyTools.e5x5(*b)/b->energy();
  ele2_eRegrInput_sigietaieta_bC1 = sqrt(lazyTools.localCovariances(*b)[0]);
  ele2_eRegrInput_sigiphiiphi_bC1 = sqrt(lazyTools.localCovariances(*b)[2]);
  ele2_eRegrInput_sigietaiphi_bC1 = lazyTools.localCovariances(*b)[1];
  ele2_eRegrInput_bEMax_Over_bCE = bemax/b->energy();
  ele2_eRegrInput_bE2nd_Over_bCE = be2nd/b->energy();
  ele2_eRegrInput_bEtop_Over_bCE = betop/b->energy();
  ele2_eRegrInput_bEbot_Over_bCE = bebottom/b->energy();
  ele2_eRegrInput_bEleft_Over_bCE = beleft/b->energy();
  ele2_eRegrInput_bEright_Over_bCE = beright/b->energy();
  ele2_eRegrInput_be2x5max_Over_bCE = be2x5max/b->energy();
  ele2_eRegrInput_be2x5top_Over_bCE = be2x5top/b->energy();
  ele2_eRegrInput_be2x5bottom_Over_bCE = be2x5bottom/b->energy();
  ele2_eRegrInput_be2x5left_Over_bCE = be2x5left/b->energy();
  ele2_eRegrInput_be2x5right_Over_bCE = be2x5right/b->energy();
   
      
  if( isbarrel){

   // seed cluster
   float betacry, bphicry, bthetatilt, bphitilt;
   int bieta, biphi;
   EcalClusterLocal _ecalLocal;
   _ecalLocal.localCoordsEB(*b,iSetup,betacry,bphicry,bieta,biphi,bthetatilt,bphitilt);
     
   ele2_eRegrInput_seedbC_eta = bieta;
   ele2_eRegrInput_seedbC_phi = biphi;
   ele2_eRegrInput_seedbC_eta_p5 = bieta%5;
   ele2_eRegrInput_seedbC_phi_p2 = biphi%2;
   ele2_eRegrInput_seedbC_bieta = (TMath::Abs(bieta)<=25)*(bieta%25) + (TMath::Abs(bieta)>25)*((bieta-25*TMath::Abs(bieta)/bieta)%20);
   ele2_eRegrInput_seedbC_phi_p20 = biphi%20;
   ele2_eRegrInput_seedbC_etacry = betacry;
   ele2_eRegrInput_seedbC_phicry = bphicry;
   ele2_eRegrInput_ESoSC = -99. ;
  }

  else{
   ele2_eRegrInput_ESoSC = s->preshowerEnergy()/s->rawEnergy();
   ele2_eRegrInput_seedbC_eta = -99.;
   ele2_eRegrInput_seedbC_phi = -99.;
   ele2_eRegrInput_seedbC_eta_p5 = -99.;
   ele2_eRegrInput_seedbC_phi_p2 = -99.;
   ele2_eRegrInput_seedbC_bieta = -99.;
   ele2_eRegrInput_seedbC_phi_p20 = -99.;
   ele2_eRegrInput_seedbC_etacry = -99.;
   ele2_eRegrInput_seedbC_phicry =-99.;
  }

  if(saveFbrem_){
   reco::GsfTrackRef eleTrack = electron.gsfTrack();
   reco::TrackRef eleNTrack = electron.closestTrack();
   GlobalPoint outPos(eleTrack->extra()->outerPosition().x(), eleTrack->extra()->outerPosition().y(), eleTrack->extra()->outerPosition().z());
   GlobalPoint innPos(eleTrack->extra()->innerPosition().x(), eleTrack->extra()->innerPosition().y(), eleTrack->extra()->innerPosition().z());
   std::vector<reco::GsfTangent> eleTangent = eleTrack->gsfExtra()->tangents();
   int numberOfValidHits_Trk = 0;
   if(!eleNTrack.isNull())
    for(unsigned int trkNH = 0; trkNH < eleNTrack->extra()->recHitsSize(); ++trkNH){
    if(!eleNTrack->extra()->recHit(trkNH)->isValid()) continue;
    DetId id = eleNTrack->extra()->recHit(trkNH)->geographicalId();
    const GeomDet* det = pDD->idToDet(id);
    if(eleTrack->extra()->seedRef().isNull()) continue;
    edm::RefToBase<TrajectorySeed> seed = eleTrack->extra()->seedRef();
    reco::ElectronSeedRef elseed = seed.castTo<reco::ElectronSeedRef>();
    TrajectoryStateOnSurface t = trajectoryStateTransform::transientState(elseed->startingState(), &(det->surface()), &(*theMagField));
    if(!t.isValid()) continue;

    GlobalPoint hitPos = t.globalPosition();

    if(hitPos.x() == ele2_inner_x && hitPos.y() == ele2_inner_y && hitPos.z() == ele2_inner_z){
     ele2_inner_p = (sqrt(eleTrack->extra()->innerMomentum().Mag2()) );
     ele2_inner_x = innPos.x();
     ele2_inner_y = innPos.y();
     ele2_inner_z = innPos.z();
    }
    if(hitPos.x() == ele2_outer_x && hitPos.y() == ele2_outer_y && hitPos.z() == ele2_outer_z){
     ele2_outer_p = (sqrt(eleTrack->extra()->outerMomentum().Mag2()) );
     ele2_outer_x = outPos.x();
     ele2_outer_y = outPos.y();
     ele2_outer_z = outPos.z();
    }

    for(unsigned int pp=0; pp<eleTangent.size(); ++pp ){
     GlobalPoint tangPos( eleTangent.at(pp).position().x(),
                          eleTangent.at(pp).position().y(),
                          eleTangent.at(pp).position().z() );

     if(hitPos.x() != tangPos.x() && hitPos.y() != tangPos.y() && hitPos.z() != tangPos.z()) continue;
     float tangMom = sqrt(eleTangent.at(pp).momentum().Mag2());
     ele2_tangent_p.push_back(tangMom);
     ele2_tangent_x.push_back(tangPos.x());
     ele2_tangent_y.push_back(tangPos.y());
     ele2_tangent_z.push_back(tangPos.z());
     ele2_tangent_dP.push_back(eleTangent.at(pp).deltaP().value());
     ele2_tangent_dPerr.push_back(eleTangent.at(pp).deltaP().error());
     ++numberOfValidHits_Trk;
    }
    }
    ele2_tangent_n = numberOfValidHits_Trk;
  }
 }
  
  
 if( verbosity_ ) std::cout << ">>> AnalyzerEle::fillEleInfo end <<<" << std::endl;
}

// -----------------------------------------------------------------------------------------

 void AnalyzerEle::fillMetInfo(const edm::Event & iEvent, const edm::EventSetup & iSetup){

 //*********** MET
  edm::Handle<edm::View<reco::MET> > PFmetHandle;
  iEvent.getByLabel(PFMetTag_,PFmetHandle);
  edm::View<reco::MET> mets = *PFmetHandle;
  reco::MET MET = mets.at(0);

  met = MET.p4();

  float cx0,cx1;
  float cy0,cy1;

  if( dataFlag_ == true && (dataRun_=="2012A" || dataRun_=="2012B")){
    
   if( dataRun_ == "2012A" ){
    cx1 = +2.65299e-01; cx0 = +3.54233e-01;
    cy1 = -1.66425e-01; cy0 = +1.88923e-01;
   }
   if( dataRun_ == "2012B" ){
    cx1 = +2.65299e-01; cx0 = +3.54233e-01;
    cy1 = -1.66425e-01; cy0 = +1.88923e-01;
   }
    
    
   float metx = met.px();
   float mety = met.py();
    
   metx -= (cx0 + cx1*PV_n_);
   mety -= (cy0 + cy1*PV_n_);
    
   met.SetPxPyPzE(metx,mety,0,sqrt(metx*metx+mety*mety));
  }
  
  p_met   = &met;
  met_et  = p_met->Et();
  met_phi = p_met->phi();  
  ele1Met_mt   = sqrt( 2. * ele1_pt * met_et * ( 1. - cos( deltaPhi(ele1_phi,met_phi) ) ) );
  ele1Met_Dphi = deltaPhi(ele1_phi,met_phi);


 }

//----------------------------------------------------------------------------------------------

void AnalyzerEle::fillDoubleEleInfo(const edm::Event & iEvent, const edm::EventSetup & iSetup){

  ele1ele2_m = (ele1 + ele2).mass();
  
  ROOT::Math::PtEtaPhiEVector ele1_sc(ele1_scE*sin(2*atan(exp(-1.*ele1_eta))),ele1_eta,ele1_phi,ele1_scE);
  ROOT::Math::PtEtaPhiEVector ele2_sc(ele2_scE*sin(2*atan(exp(-1.*ele2_eta))),ele2_eta,ele2_phi,ele2_scE);
  ele1ele2_scM = (ele1_sc + ele2_sc).mass();

 }


double AnalyzerEle::deltaPhi(const double& phi1, const double& phi2){
  double deltaphi = fabs(phi1 - phi2);
  if (deltaphi > 3.141592654) deltaphi = 6.283185308 - deltaphi;
  return deltaphi;
}


// -----------------------------------------
void AnalyzerEle::initialize(){

 isW_ = -1;
 isZ_ = -1;
 nele_ = 0;

 // electron variables
 ele1_charge =-99.;
 ele1_p =-99.;
 ele1_pt =-99.;
 ele1_eta =-99.;
 ele1_phi=-99.;
 ele1_isTrackerDriven=-99;
 ele1_classification=-99;

 ele1_idtype = -1 ;     
 ele1_sigmaIetaIeta =-99.;
 ele1_DphiIn =-99.;
 ele1_DetaIn =-99.;
 ele1_deltaEtaEleClusterTrackAtCalo = -99.;
 ele1_deltaEtaPhiClusterTrackAtCalo = -99.;
 ele1_deltaEtaSeedClusterTrackAtCalo = -99.;
 ele1_deltaPhiSeedClusterTrackAtCalo = -99.;
 ele1_HOverE =-99.;
 ele1_tkIso =-99.;
 ele1_emIso =-99.;
 ele1_hadIso =-99.;

 ele1_dxy_PV=-99.;
 ele1_dz_PV=-99.;
 ele1_sigmaP = -99.;
 ele1_effAreaForIso=-99.;

 ele1_scERaw =-99.;
 ele1_scEtRaw = -99.;
 ele1_scEt = -99.;
 ele1_scLocalEta =-99.;
 ele1_scLocalPhi = -99.;
 ele1_scEtaWidth = -99.;
 ele1_scPhiWidth = -99.;
 ele1_scCrackCorr =-99.;
 ele1_scLocalContCorr =-99.;
 ele1_scE =-99.;
 ele1_scEta =-99.;
 ele1_scPhi =-99.;
 ele1_scLaserCorr =-99.;
 ele1_scERaw_PUcleaned=-99.;
 ele1_scEtaWidth_PUcleaned=-99.;
 ele1_scPhiWidth_PUcleaned=-99.;
 ele1_fCorrection_PUcleaned=-99.;

 ele1_fEta=-99.;
 ele1_fEtaCorr=-99.;
 ele1_tkP=-99.;
 ele1_tkPt=-99.;
 ele1_fbrem=-99.;
 
 ele1_eSeedBC=-99.;
 ele1_e5x5=-99.;
 ele1_e3x3=-99.;
 ele1_scNxtal=-99.;
 ele1_bcN =-99;
 ele1_5x5LaserCorr=-99.;
 ele1_3x3LaserCorr=-99.;
 ele1_es=-99.;

 ele1_seedE=-99.;
 ele1_seedLaserAlpha=-99.;
 ele1_seedLaserCorr=-99.;
 ele1_seedICConstant=-99.;
 ele1_seedIeta=-9999;
 ele1_seedIphi=-9999;
 ele1_seedIx=-9999;
 ele1_seedIy=-9999;
 ele1_seedZside=-9999;
 ele1_EOverP=-99.;

 ele1_recHit_E.clear();
 ele1_recHit_flag.clear();
 ele1_recHit_hashedIndex.clear();
 ele1_recHit_ietaORix.clear();
 ele1_recHit_iphiORiy.clear();
 ele1_recHit_zside.clear();
 ele1_recHit_laserCorrection.clear();
 ele1_recHit_Alpha.clear();
 ele1_recHit_ICConstant.clear();
 ele1_nRecHits = -9999;

 ele1_recHitMatrix_E.clear();
 ele1_recHitMatrix_flag.clear();
 ele1_recHitMatrix_hashedIndex.clear();
 ele1_recHitMatrix_ietaORix.clear();
 ele1_recHitMatrix_iphiORiy.clear();
 ele1_recHitMatrix_zside.clear();
 ele1_recHitMatrix_laserCorrection.clear();
 ele1_recHitMatrix_ICConstant.clear();
 ele1_recHitMatrix_samples.clear();
  
 ele1_isEB= -9999;
 ele1_isEBEEGap= -9999;
 ele1_isEBEtaGap= -9999;
 ele1_isEBPhiGap= -9999;
 ele1_isEEDeeGap= -9999;
 ele1_isEERingGap= -9999;

 ele1_eRegrInput_rawE = -99.;
 ele1_eRegrInput_r9 = -99.;
 ele1_eRegrInput_eta = -99.;
 ele1_eRegrInput_phi= -99.;
 ele1_eRegrInput_r25= -99.;
 ele1_eRegrInput_etaW= -99.;
 ele1_eRegrInput_phiW= -99.;
 ele1_eRegrInput_rho= -99.;
 ele1_eRegrInput_Deta_bC_sC= -99.;
 ele1_eRegrInput_Dphi_bC_sC= -99.;
 ele1_eRegrInput_bCE_Over_sCE= -99.;
 ele1_eRegrInput_e3x3_Over_bCE= -99.;
 ele1_eRegrInput_e5x5_Over_bCE= -99.;
 ele1_eRegrInput_sigietaieta_bC1= -99.;
 ele1_eRegrInput_sigiphiiphi_bC1= -99.;
 ele1_eRegrInput_sigietaiphi_bC1= -99.;
 ele1_eRegrInput_bEMax_Over_bCE= -99.;
 ele1_eRegrInput_bE2nd_Over_bCE= -99.;
 ele1_eRegrInput_bEtop_Over_bCE= -99.;
 ele1_eRegrInput_bEbot_Over_bCE= -99.;

 ele1_eRegrInput_bEleft_Over_bCE= -99.;
 ele1_eRegrInput_bEright_Over_bCE= -99.;
 ele1_eRegrInput_be2x5max_Over_bCE= -99.;
 ele1_eRegrInput_be2x5top_Over_bCE= -99.;
 ele1_eRegrInput_be2x5bottom_Over_bCE= -99.;
 ele1_eRegrInput_be2x5left_Over_bCE= -99.;
 ele1_eRegrInput_be2x5right_Over_bCE= -99.;

 ele1_eRegrInput_seedbC_eta= -99.;
 ele1_eRegrInput_seedbC_phi= -99.;
 ele1_eRegrInput_seedbC_eta_p5= -99.;
 ele1_eRegrInput_seedbC_phi_p2= -99.;
 ele1_eRegrInput_seedbC_bieta= -99.;
 ele1_eRegrInput_seedbC_phi_p20= -99.;
 ele1_eRegrInput_seedbC_etacry= -99.;
 ele1_eRegrInput_seedbC_phicry= -99.;

 ele1_eRegrInput_ESoSC= -99.;
 ele1_eRegrInput_nPV= -99.;
 ele1_eRegrInput_SCsize= -99.;


 ele2_charge =-99.;
 ele2_p =-99.;
 ele2_pt =-99.;
 ele2_eta =-99.;
 ele2_phi=-99.;
 ele2_isTrackerDriven=-99;
 ele2_classification = -99;
  
 ele2_idtype = -1 ;     
 ele2_sigmaIetaIeta =-99.;
 ele2_DphiIn =-99.;
 ele2_DetaIn =-99.;
 ele2_deltaEtaEleClusterTrackAtCalo = -99.;
 ele2_deltaEtaPhiClusterTrackAtCalo = -99.;
 ele2_deltaEtaSeedClusterTrackAtCalo = -99.;
 ele2_deltaPhiSeedClusterTrackAtCalo = -99.;
 ele2_HOverE =-99.;
 ele2_tkIso =-99.;
 ele2_emIso =-99.;
 ele2_hadIso =-99.;

 ele2_dxy_PV=-99.;
 ele2_dz_PV=-99.;
 ele2_sigmaP = -99.;
 ele2_effAreaForIso=-99.;

 ele2_scERaw =-99.;
 ele2_scEtRaw = -99.;
 ele2_scEt = -99.;
 ele2_scLocalEta =-99.;
 ele2_scLocalPhi = -99.;
 ele2_scEtaWidth = -99.;
 ele2_scPhiWidth = -99.;
 ele2_scCrackCorr =-99.;
 ele2_scLocalContCorr =-99.;
 ele2_scE =-99.;
 ele2_scEta =-99.;
 ele2_scPhi =-99.;
 ele2_scLaserCorr =-99.;
 ele2_scERaw_PUcleaned=-99.;
 ele2_scEtaWidth_PUcleaned=-99.;
 ele2_scPhiWidth_PUcleaned=-99.;
 ele2_fCorrection_PUcleaned=-99.;

 ele2_fEta=-99.;
 ele2_fEtaCorr=-99.;
 ele2_tkP=-99.;
 ele2_tkPt=-99.;
 ele2_fbrem=-99.;
 
 ele2_eSeedBC=-99.;
 ele2_e5x5=-99.;
 ele2_e3x3=-99.;
 ele2_scNxtal=-99.;
 ele2_bcN =-99;
 ele2_5x5LaserCorr=-99.;
 ele2_3x3LaserCorr=-99.;
 ele2_es=-99.;

 ele2_seedE=-99.;
 ele2_seedLaserAlpha=-99.;
 ele2_seedLaserCorr=-99.;
 ele2_seedICConstant=-99.;
 ele2_seedIeta=-9999;
 ele2_seedIphi=-9999;
 ele2_seedIx=-9999;
 ele2_seedIy=-9999;
 ele2_seedZside=-9999;
 ele2_EOverP=-99.;

 ele2_recHit_E.clear();
 ele2_recHit_flag.clear();
 ele2_recHit_hashedIndex.clear();
 ele2_recHit_ietaORix.clear();
 ele2_recHit_iphiORiy.clear();
 ele2_recHit_zside.clear();
 ele2_recHit_laserCorrection.clear();
 ele2_recHit_Alpha.clear();
 ele2_recHit_ICConstant.clear();
 ele2_nRecHits = -9999;

 ele2_recHitMatrix_E.clear();
 ele2_recHitMatrix_flag.clear();
 ele2_recHitMatrix_hashedIndex.clear();
 ele2_recHitMatrix_ietaORix.clear();
 ele2_recHitMatrix_iphiORiy.clear();
 ele2_recHitMatrix_zside.clear();
 ele2_recHitMatrix_laserCorrection.clear();
 ele2_recHitMatrix_ICConstant.clear();
 ele2_recHitMatrix_samples.clear();
  
 ele2_isEB= -9999;
 ele2_isEBEEGap= -9999;
 ele2_isEBEtaGap= -9999;
 ele2_isEBPhiGap= -9999;
 ele2_isEEDeeGap= -9999;
 ele2_isEERingGap= -9999;

 ele2_eRegrInput_rawE = -99.;
 ele2_eRegrInput_r9 = -99.;
 ele2_eRegrInput_eta = -99.;
 ele2_eRegrInput_phi= -99.;
 ele2_eRegrInput_r25= -99.;
 ele2_eRegrInput_etaW= -99.;
 ele2_eRegrInput_phiW= -99.;
 ele2_eRegrInput_rho= -99.;
 ele2_eRegrInput_Deta_bC_sC= -99.;
 ele2_eRegrInput_Dphi_bC_sC= -99.;
 ele2_eRegrInput_bCE_Over_sCE= -99.;
 ele2_eRegrInput_e3x3_Over_bCE= -99.;
 ele2_eRegrInput_e5x5_Over_bCE= -99.;
 ele2_eRegrInput_sigietaieta_bC1= -99.;
 ele2_eRegrInput_sigiphiiphi_bC1= -99.;
 ele2_eRegrInput_sigietaiphi_bC1= -99.;
 ele2_eRegrInput_bEMax_Over_bCE= -99.;
 ele2_eRegrInput_bE2nd_Over_bCE= -99.;
 ele2_eRegrInput_bEtop_Over_bCE= -99.;
 ele2_eRegrInput_bEbot_Over_bCE= -99.;

 ele2_eRegrInput_bEleft_Over_bCE= -99.;
 ele2_eRegrInput_bEright_Over_bCE= -99.;
 ele2_eRegrInput_be2x5max_Over_bCE= -99.;
 ele2_eRegrInput_be2x5top_Over_bCE= -99.;
 ele2_eRegrInput_be2x5bottom_Over_bCE= -99.;
 ele2_eRegrInput_be2x5left_Over_bCE= -99.;
 ele2_eRegrInput_be2x5right_Over_bCE= -99.;

 ele2_eRegrInput_seedbC_eta= -99.;
 ele2_eRegrInput_seedbC_phi= -99.;
 ele2_eRegrInput_seedbC_eta_p5= -99.;
 ele2_eRegrInput_seedbC_phi_p2= -99.;
 ele2_eRegrInput_seedbC_bieta= -99.;
 ele2_eRegrInput_seedbC_phi_p20= -99.;
 ele2_eRegrInput_seedbC_etacry= -99.;
 ele2_eRegrInput_seedbC_phicry= -99.;

 ele2_eRegrInput_ESoSC= -99.;
 ele2_eRegrInput_nPV= -99.;
 ele2_eRegrInput_SCsize= -99.;

 if(saveMCInfo_){
   nPU_ = 0 ;
   mcV_E = -9999.;
   mcV_Px = -9999.;
   mcV_Py = -9999.;
   mcV_Pz = -9999.;
   mcV_Eta = -9999.;
   mcV_Phi = -9999.;
   mcV_M   = -9999.;
   mcV_Charge = -99;
   mcV_PdgId = -99;

   mcF1_fromV_E   = -9999.;
   mcF1_fromV_Px  = -9999.;
   mcF1_fromV_Py  = -9999.;
   mcF1_fromV_Pz  = -9999.;
   mcF1_fromV_Eta = -9999.;
   mcF1_fromV_Phi = -9999.;
   mcF1_fromV_M   = -9999.;
   mcF1_fromV_Charge = -99;
   mcF1_fromV_PdgId  = -99;

   mcF2_fromV_E = -9999.;
   mcF2_fromV_Px = -9999.;
   mcF2_fromV_Py = -9999.;
   mcF2_fromV_Pz = -9999.;
   mcF2_fromV_Eta = -9999.;
   mcF2_fromV_Phi = -9999.;
   mcF2_fromV_M   = -9999.;
   mcF2_fromV_Charge = -99;
   mcF2_fromV_PdgId = -99;
 }

 if(saveFbrem_){
  ele1_inner_p = -9999.;
  ele1_inner_x = -9999.;
  ele1_inner_y = -9999.;
  ele1_inner_z = -9999.;
  ele1_outer_p = -9999.;
  ele1_outer_x = -9999.;
  ele1_outer_y = -9999.;
  ele1_outer_z = -9999.;
  ele1_tangent_n = -1;
  ele1_tangent_p.clear();
  ele1_tangent_x.clear();
  ele1_tangent_y.clear();
  ele1_tangent_z.clear();
  ele1_tangent_dP.clear();
  ele1_tangent_dPerr.clear();

  ele2_inner_p = -9999.;
  ele2_inner_x = -9999.;
  ele2_inner_y = -9999.;
  ele2_inner_z = -9999.;
  ele2_outer_p = -9999.;
  ele2_outer_x = -9999.;
  ele2_outer_y = -9999.;
  ele2_outer_z = -9999.;
  ele2_tangent_n = -1;
  ele2_tangent_p.clear();
  ele2_tangent_x.clear();
  ele2_tangent_y.clear();
  ele2_tangent_z.clear();
  ele2_tangent_dP.clear();
  ele2_tangent_dPerr.clear();
 }

 met_et=-99.;
 met_phi=-99.;

 ele1Met_mt = -99.;
 ele1Met_Dphi = -99.;
  
  
  // di-electron variables
 ele1ele2_m=-99.;
 ele1ele2_scM=-99.;


}

//define this as a plug-in
DEFINE_FWK_MODULE(AnalyzerEle);
