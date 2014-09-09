#include "StreamForElectrons/AnalyzerEle/plugins/AnalyzerEle.h"

AnalyzerEle::AnalyzerEle(const edm::ParameterSet& iConfig){

 // parse input informations
 dataRun_             = iConfig.getParameter<std::string>("dataRun");
 recHitCollection_EB_ = iConfig.getParameter<edm::InputTag>("recHitCollection_EB");
 recHitCollection_EE_ = iConfig.getParameter<edm::InputTag>("recHitCollection_EE");
 BSTag_               = iConfig.getParameter<edm::InputTag>("theBeamSpotTag");
 PVTag_               = iConfig.getParameter<edm::InputTag>("PVTag");
 PVTag_alternative_   = iConfig.getParameter<edm::InputTag>("PVTag_alternative");
 rhoTag_              = iConfig.getParameter<edm::InputTag>("rhoTag");
 EleTag_              = iConfig.getParameter<edm::InputTag>("EleTag");
 PFMetTag_            = iConfig.getParameter<edm::InputTag>("PFMetTag");
 conversionsInputTag_ = iConfig.getParameter<edm::InputTag>("conversionsInputTag");
 MCtruthTag_          = iConfig.getParameter<edm::InputTag>("MCtruthTag");
  
 triggerResultsCollection_ = iConfig.getParameter<edm::InputTag>("triggerResultsCollection");
 hltPaths_                 = iConfig.getParameter<std::vector<std::string> >("hltPaths") ;

 verbosity_          = iConfig.getUntrackedParameter<bool>("verbosity", false);
 doWZSelection_      = iConfig.getUntrackedParameter<bool>("doWZSelection", false);
 applyElectronID_    = iConfig.getUntrackedParameter<bool>("applyElectronID", false);
 applyCorrections_   = iConfig.getUntrackedParameter<bool>("applyCorrections",false);
 dataFlag_           = iConfig.getUntrackedParameter<bool>("dataFlag", true);
 saveMCInfo_         = iConfig.getUntrackedParameter<bool>("saveMCInfo", false);

 if(verbosity_) std::cout << ">>> AnalyzerEle::AnalyzerEle begin <<<" << std::endl;


 eventNaiveId_ = 0;
 
 //////////////////////////
 EcalClusterCrackCorrection     = EcalClusterFunctionFactory::get()->create("EcalClusterCrackCorrection", iConfig);
 EcalClusterLocalContCorrection = EcalClusterFunctionFactory::get()->create("EcalClusterLocalContCorrection", iConfig);

}

// --------------------------------------------------------------------
AnalyzerEle::~AnalyzerEle () { if(verbosity_) std::cout << ">>> AnalyzerEle::~AnalyzerEle <<< analyzed " << eventNaiveId_ << " events" << std::endl;}

// ------------ method called once each job just before starting event loop ------------
void AnalyzerEle::beginJob(){

 myTree_ = fs_->make<TTree>("AnalyzerEle","AnalyzerEle"); 

 if(verbosity_) std::cout << ">>>>>> AnalyzerEle::AnalyzerEle::set event branches <<<" << std::endl;

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
 myTree_ -> Branch("PV_n_alternative", &PV_n_alternative_, "PV_n_alternative/I");
 myTree_ -> Branch("PV_z_alternative", &PV_z_alternative_, "PV_z_alternative/F");
 myTree_ -> Branch("PV_d0_alternative", &PV_d0_alternative_, "PV_d0_alternative/F");
 myTree_ -> Branch("rho", &rho_, "rho/F");
 myTree_ -> Branch("nele", &nele_, "nele/I");

 myTree_ -> Branch("HLTResult","std::map<std::string,bool>",&HLTResults_);

 if(saveMCInfo_){

  if(verbosity_) std::cout<< ">>>>>> SimpleNtupleEoverP::SimpleNtupleEoverP::set MC branches <<<" << std::endl;
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
 if(verbosity_) std::cout << ">>>>>> AnalyzerEle::AnalyzerEle::set ele1 branches <<<" << std::endl;

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

 myTree_ -> Branch("ele1_idtype","std::vector<int>",&ele1_idtype);
  
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

 myTree_ -> Branch("ele1_fEta", &ele1_fEta, "ele1_fEta/F");
 myTree_ -> Branch("ele1_fEtaCorr", &ele1_fEtaCorr, "ele1_fEtaCorr/F");
 myTree_ -> Branch("ele1_tkP", &ele1_tkP, "ele1_tkP/F");
 myTree_ -> Branch("ele1_tkPt", &ele1_tkPt, "ele1_tkPt/F");
 myTree_ -> Branch("ele1_fbrem", &ele1_fbrem, "ele1_fbrem/F");
  
 myTree_ -> Branch("ele1_dxy_PV", &ele1_dxy_PV, "ele1_dxy_PV/F");
 myTree_ -> Branch("ele1_dz_PV", &ele1_dz_PV, "ele1_dz_PV/F");
 myTree_ -> Branch("ele1_dxy_PV_alternative", &ele1_dxy_PV_alternative, "ele1_dxy_PV_alternative/F");
 myTree_ -> Branch("ele1_dz_PV_alternative", &ele1_dz_PV_alternative, "ele1_dz_PV_alternative/F");
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


 // Regression V3 variables                                                                                                                                                               
 myTree_ -> Branch("ele1_eRegrInput_rawE",&ele1_eRegrInput_rawE,"ele1_eRegrInput_rawE/F");
 myTree_ -> Branch("ele1_eRegrInput_r9",&ele1_eRegrInput_r9,"ele1_eRegrInput_r9/F");
 myTree_ -> Branch("ele1_eRegrInput_eta",&ele1_eRegrInput_eta,"ele1_eRegrInput_eta/F");
 myTree_ -> Branch("ele1_eRegrInput_phi",&ele1_eRegrInput_phi,"ele1_eRegrInput_phi/F");
 myTree_ -> Branch("ele1_eRegrInput_etaW",&ele1_eRegrInput_etaW,"ele1_eRegrInput_etaW/F");
 myTree_ -> Branch("ele1_eRegrInput_phiW",&ele1_eRegrInput_phiW,"ele1_eRegrInput_phiW/F");
 myTree_ -> Branch("ele1_eRegrInput_SCsize",&ele1_eRegrInput_SCsize,"ele1_eRegrInput_SCsize/F");
 myTree_ -> Branch("ele1_eRegrInput_rho",&ele1_eRegrInput_rho,"ele1_eRegrInput_rho/F");
 myTree_ -> Branch("ele1_eRegrInput_hoe",&ele1_eRegrInput_hoe,"ele1_eRegrInput_hoe/F");
 myTree_ -> Branch("ele1_eRegrInput_nPV",&ele1_eRegrInput_nPV,"ele1_eRegrInput_nPV/F");
 myTree_ -> Branch("ele1_eRegrInput_seed_eta",&ele1_eRegrInput_seed_eta,"ele1_eRegrInput_seed_eta/F");
 myTree_ -> Branch("ele1_eRegrInput_seed_phi",&ele1_eRegrInput_seed_phi,"ele1_eRegrInput_seed_phi/F");
 myTree_ -> Branch("ele1_eRegrInput_seed_E",&ele1_eRegrInput_seed_E,"ele1_eRegrInput_seed_E/F");
 myTree_ -> Branch("ele1_eRegrInput_seed_e3x3",&ele1_eRegrInput_seed_e3x3,"ele1_eRegrInput_seed_e3x3/F");
 myTree_ -> Branch("ele1_eRegrInput_seed_e5x5",&ele1_eRegrInput_seed_e5x5,"ele1_eRegrInput_seed_e5x5/F");
 myTree_ -> Branch("ele1_eRegrInput_sigietaieta",&ele1_eRegrInput_sigietaieta,"ele1_eRegrInput_sigietaieta/F");
 myTree_ -> Branch("ele1_eRegrInput_sigiphiiphi",&ele1_eRegrInput_sigiphiiphi,"ele1_eRegrInput_sigiphiiphi/F");
 myTree_ -> Branch("ele1_eRegrInput_sigietaiphi",&ele1_eRegrInput_sigietaiphi,"ele1_eRegrInput_sigietaiphi/F");
 myTree_ -> Branch("ele1_eRegrInput_eMax",&ele1_eRegrInput_eMax,"ele1_eRegrInput_eMax/F");
 myTree_ -> Branch("ele1_eRegrInput_e2nd",&ele1_eRegrInput_e2nd,"ele1_eRegrInput_e2nd/F");
 myTree_ -> Branch("ele1_eRegrInput_eTop",&ele1_eRegrInput_eTop,"ele1_eRegrInput_eTop/F");
 myTree_ -> Branch("ele1_eRegrInput_eBottom",&ele1_eRegrInput_eBottom,"ele1_eRegrInput_eBottom/F");
 myTree_ -> Branch("ele1_eRegrInput_eLeft",&ele1_eRegrInput_eLeft,"ele1_eRegrInput_eLeft/F");
 myTree_ -> Branch("ele1_eRegrInput_eRight",&ele1_eRegrInput_eRight,"ele1_eRegrInput_eRight/F");
 myTree_ -> Branch("ele1_eRegrInput_e2x5Max",&ele1_eRegrInput_e2x5Max,"ele1_eRegrInput_e2x5Max/F");
 myTree_ -> Branch("ele1_eRegrInput_e2x5Top",&ele1_eRegrInput_e2x5Top,"ele1_eRegrInput_e2x5Top/F");
 myTree_ -> Branch("ele1_eRegrInput_e2x5Bottom",&ele1_eRegrInput_e2x5Bottom,"ele1_eRegrInput_e2x5Bottom/F");
 myTree_ -> Branch("ele1_eRegrInput_e2x5Left",&ele1_eRegrInput_e2x5Left,"ele1_eRegrInput_e2x5Left/F");
 myTree_ -> Branch("ele1_eRegrInput_e2x5Right",&ele1_eRegrInput_e2x5Right,"ele1_eRegrInput_e2x5Right/F");
 myTree_ -> Branch("ele1_eRegrInput_seed_ieta",&ele1_eRegrInput_seed_ieta,"ele1_eRegrInput_seed_ieta/F");
 myTree_ -> Branch("ele1_eRegrInput_seed_iphi",&ele1_eRegrInput_seed_iphi,"ele1_eRegrInput_seed_iphi/F");
 myTree_ -> Branch("ele1_eRegrInput_seed_etaCrySeed",&ele1_eRegrInput_seed_etaCrySeed,"ele1_eRegrInput_seed_etaCrySeed/F");
 myTree_ -> Branch("ele1_eRegrInput_seed_phiCrySeed",&ele1_eRegrInput_seed_phiCrySeed,"ele1_eRegrInput_seed_phiCrySeed/F");
 myTree_ -> Branch("ele1_eRegrInput_preshowerEnergyOverRaw",&ele1_eRegrInput_preshowerEnergyOverRaw,"ele1_eRegrInput_preshowerEnergyOverRaw/F");
 myTree_ -> Branch("ele1_eRegrInput_ecalDrivenSeed",&ele1_eRegrInput_ecalDrivenSeed,"ele1_eRegrInput_ecalDrivenSeed/F");
 myTree_ -> Branch("ele1_eRegrInput_isEBEtaGap",&ele1_eRegrInput_isEBEtaGap,"ele1_eRegrInput_isEBEtaGap/F");
 myTree_ -> Branch("ele1_eRegrInput_isEBPhiGap",&ele1_eRegrInput_isEBPhiGap,"ele1_eRegrInput_isEBPhiGap/F");
 myTree_ -> Branch("ele1_eRegrInput_eSubClusters",&ele1_eRegrInput_eSubClusters,"ele1_eRegrInput_eSubClusters/F");
 myTree_ -> Branch("ele1_eRegrInput_subClusterEnergy_1",&ele1_eRegrInput_subClusterEnergy_1,"ele1_eRegrInput_subClusterEnergy_1/F");
 myTree_ -> Branch("ele1_eRegrInput_subClusterEnergy_2",&ele1_eRegrInput_subClusterEnergy_2,"ele1_eRegrInput_subClusterEnergy_2/F");
 myTree_ -> Branch("ele1_eRegrInput_subClusterEnergy_3",&ele1_eRegrInput_subClusterEnergy_3,"ele1_eRegrInput_subClusterEnergy_3/F");
 myTree_ -> Branch("ele1_eRegrInput_subClusterEta_1",&ele1_eRegrInput_subClusterEta_1,"ele1_eRegrInput_subClusterEta_1/F");
 myTree_ -> Branch("ele1_eRegrInput_subClusterEta_2",&ele1_eRegrInput_subClusterEta_2,"ele1_eRegrInput_subClusterEta_2/F");
 myTree_ -> Branch("ele1_eRegrInput_subClusterEta_3",&ele1_eRegrInput_subClusterEta_3,"ele1_eRegrInput_subClusterEta_3/F");
 myTree_ -> Branch("ele1_eRegrInput_subClusterPhi_1",&ele1_eRegrInput_subClusterPhi_1,"ele1_eRegrInput_subClusterPhi_1/F");
 myTree_ -> Branch("ele1_eRegrInput_subClusterPhi_2",&ele1_eRegrInput_subClusterPhi_2,"ele1_eRegrInput_subClusterPhi_2/F");
 myTree_ -> Branch("ele1_eRegrInput_subClusterPhi_3",&ele1_eRegrInput_subClusterPhi_3,"ele1_eRegrInput_subClusterPhi_3/F");
 myTree_ -> Branch("ele1_eRegrInput_subClusterEmax_1",&ele1_eRegrInput_subClusterEmax_1,"ele1_eRegrInput_subClusterEmax_1/F");
 myTree_ -> Branch("ele1_eRegrInput_subClusterEmax_2",&ele1_eRegrInput_subClusterEmax_2,"ele1_eRegrInput_subClusterEmax_2/F");
 myTree_ -> Branch("ele1_eRegrInput_subClusterEmax_3",&ele1_eRegrInput_subClusterEmax_3,"ele1_eRegrInput_subClusterEmax_3/F");
 myTree_ -> Branch("ele1_eRegrInput_subClusterE3x3_1",&ele1_eRegrInput_subClusterE3x3_1,"ele1_eRegrInput_subClusterE3x3_1/F");
 myTree_ -> Branch("ele1_eRegrInput_subClusterE3x3_2",&ele1_eRegrInput_subClusterE3x3_2,"ele1_eRegrInput_subClusterE3x3_2/F");
 myTree_ -> Branch("ele1_eRegrInput_subClusterE3x3_3",&ele1_eRegrInput_subClusterE3x3_3,"ele1_eRegrInput_subClusterE3x3_3/F");
 myTree_ -> Branch("ele1_eRegrInput_eESClusters",&ele1_eRegrInput_eESClusters,"ele1_eRegrInput_eESClusters/F");
 myTree_ -> Branch("ele1_eRegrInput_eESClusterEnergy_1",&ele1_eRegrInput_eESClusterEnergy_1,"ele1_eRegrInput_eESClusterEnergy_1/F");
 myTree_ -> Branch("ele1_eRegrInput_eESClusterEnergy_2",&ele1_eRegrInput_eESClusterEnergy_2,"ele1_eRegrInput_eESClusterEnergy_2/F");
 myTree_ -> Branch("ele1_eRegrInput_eESClusterEnergy_3",&ele1_eRegrInput_eESClusterEnergy_3,"ele1_eRegrInput_eESClusterEnergy_3/F");
 myTree_ -> Branch("ele1_eRegrInput_eESClusterEta_1",&ele1_eRegrInput_eESClusterEta_1,"ele1_eRegrInput_eESClusterEta_1/F");
 myTree_ -> Branch("ele1_eRegrInput_eESClusterEta_2",&ele1_eRegrInput_eESClusterEta_2,"ele1_eRegrInput_eESClusterEta_2/F");
 myTree_ -> Branch("ele1_eRegrInput_eESClusterEta_3",&ele1_eRegrInput_eESClusterEta_3,"ele1_eRegrInput_eESClusterEta_3/F");
 myTree_ -> Branch("ele1_eRegrInput_eESClusterPhi_1",&ele1_eRegrInput_eESClusterPhi_1,"ele1_eRegrInput_eESClusterPhi_1/F");
 myTree_ -> Branch("ele1_eRegrInput_eESClusterPhi_2",&ele1_eRegrInput_eESClusterPhi_2,"ele1_eRegrInput_eESClusterPhi_2/F");
 myTree_ -> Branch("ele1_eRegrInput_eESClusterPhi_3",&ele1_eRegrInput_eESClusterPhi_3,"ele1_eRegrInput_eESClusterPhi_3/F");
 myTree_ -> Branch("ele1_eRegrInput_pt",&ele1_eRegrInput_pt,"ele1_eRegrInput_pt/F");
 myTree_ -> Branch("ele1_eRegrInput_trackMomentumAtVtxR",&ele1_eRegrInput_trackMomentumAtVtxR,"ele1_eRegrInput_trackMomentumAtVtxR/F");
 myTree_ -> Branch("ele1_eRegrInput_fbrem",&ele1_eRegrInput_fbrem,"ele1_eRegrInput_fbrem/F");
 myTree_ -> Branch("ele1_eRegrInput_charge",&ele1_eRegrInput_charge,"ele1_eRegrInput_charge/F");
 myTree_ -> Branch("ele1_eRegrInput_eSuperClusterOverP",&ele1_eRegrInput_eSuperClusterOverP,"ele1_eRegrInput_eSuperClusterOverP/F");

 myTree_ -> Branch("ele1_nGgsfTrackHits",&ele1_nGgsfTrackHits,"ele1_nGgsfTrackHits/I");
 myTree_ -> Branch("ele1_numberOfLostHits",&ele1_numberOfLostHits,"ele1_numberOfLostHits/I");
 myTree_ -> Branch("ele1_nAmbiguousGsfTrack",&ele1_nAmbiguousGsfTrack,"ele1_nAmbiguousGsfTrack/I");


 // ele2 variables
 if(verbosity_) std::cout << ">>>>>> AnalyzerEle::AnalyzerEle::set ele2 branches <<<" << std::endl;

 myTree_ -> Branch("ele2_charge",&ele2_charge,"ele2_charge/F");
 myTree_ -> Branch("ele2_p", &ele2_p, "ele2_p/F");
 myTree_ -> Branch("ele2_pt", &ele2_pt, "ele2_pt/F");
 myTree_ -> Branch("ele2_eta", &ele2_eta, "ele2_eta/F");
 myTree_ -> Branch("ele2_phi", &ele2_phi, "ele2_phi/F");

 myTree_ -> Branch("ele2_idtype", "std::vector<int>",&ele2_idtype);
  
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
 myTree_ -> Branch("ele2_dxy_PV_alternative", &ele2_dxy_PV_alternative, "ele2_dxy_PV_alternative/F");
 myTree_ -> Branch("ele2_dz_PV_alternative", &ele2_dz_PV_alternative, "ele2_dz_PV_alternative/F");
 myTree_ -> Branch("ele2_sigmaP", &ele2_sigmaP, "ele2_sigmaP/F");
  
 myTree_ -> Branch("ele2_scERaw", &ele2_scERaw, "ele2_scERaw/F");
 myTree_ -> Branch("ele2_scEtRaw", &ele2_scEtRaw, "ele2_scEtRaw/F");
 myTree_ -> Branch("ele2_scE", &ele2_scE, "ele2_scE/F");
 myTree_ -> Branch("ele2_scEt", &ele2_scEt, "ele2_scEt/F");
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


 // Regression V3 variables                                                                                                                                                               
 myTree_ -> Branch("ele2_eRegrInput_rawE",&ele2_eRegrInput_rawE,"ele2_eRegrInput_rawE/F");
 myTree_ -> Branch("ele2_eRegrInput_r9",&ele2_eRegrInput_r9,"ele2_eRegrInput_r9/F");
 myTree_ -> Branch("ele2_eRegrInput_eta",&ele2_eRegrInput_eta,"ele2_eRegrInput_eta/F");
 myTree_ -> Branch("ele2_eRegrInput_phi",&ele2_eRegrInput_phi,"ele2_eRegrInput_phi/F");
 myTree_ -> Branch("ele2_eRegrInput_etaW",&ele2_eRegrInput_etaW,"ele2_eRegrInput_etaW/F");
 myTree_ -> Branch("ele2_eRegrInput_phiW",&ele2_eRegrInput_phiW,"ele2_eRegrInput_phiW/F");
 myTree_ -> Branch("ele2_eRegrInput_SCsize",&ele2_eRegrInput_SCsize,"ele2_eRegrInput_SCsize/F");
 myTree_ -> Branch("ele2_eRegrInput_rho",&ele2_eRegrInput_rho,"ele2_eRegrInput_rho/F");
 myTree_ -> Branch("ele2_eRegrInput_hoe",&ele2_eRegrInput_hoe,"ele2_eRegrInput_hoe/F");
 myTree_ -> Branch("ele2_eRegrInput_nPV",&ele2_eRegrInput_nPV,"ele2_eRegrInput_nPV/F");
 myTree_ -> Branch("ele2_eRegrInput_seed_eta",&ele2_eRegrInput_seed_eta,"ele2_eRegrInput_seed_eta/F");
 myTree_ -> Branch("ele2_eRegrInput_seed_phi",&ele2_eRegrInput_seed_phi,"ele2_eRegrInput_seed_phi/F");
 myTree_ -> Branch("ele2_eRegrInput_seed_E",&ele2_eRegrInput_seed_E,"ele2_eRegrInput_seed_E/F");
 myTree_ -> Branch("ele2_eRegrInput_seed_e3x3",&ele2_eRegrInput_seed_e3x3,"ele2_eRegrInput_seed_e3x3/F");
 myTree_ -> Branch("ele2_eRegrInput_seed_e5x5",&ele2_eRegrInput_seed_e5x5,"ele2_eRegrInput_seed_e5x5/F");
 myTree_ -> Branch("ele2_eRegrInput_sigietaieta",&ele2_eRegrInput_sigietaieta,"ele2_eRegrInput_sigietaieta/F");
 myTree_ -> Branch("ele2_eRegrInput_sigiphiiphi",&ele2_eRegrInput_sigiphiiphi,"ele2_eRegrInput_sigiphiiphi/F");
 myTree_ -> Branch("ele2_eRegrInput_sigietaiphi",&ele2_eRegrInput_sigietaiphi,"ele2_eRegrInput_sigietaiphi/F");
 myTree_ -> Branch("ele2_eRegrInput_eMax",&ele2_eRegrInput_eMax,"ele2_eRegrInput_eMax/F");
 myTree_ -> Branch("ele2_eRegrInput_e2nd",&ele2_eRegrInput_e2nd,"ele2_eRegrInput_e2nd/F");
 myTree_ -> Branch("ele2_eRegrInput_eTop",&ele2_eRegrInput_eTop,"ele2_eRegrInput_eTop/F");
 myTree_ -> Branch("ele2_eRegrInput_eBottom",&ele2_eRegrInput_eBottom,"ele2_eRegrInput_eBottom/F");
 myTree_ -> Branch("ele2_eRegrInput_eLeft",&ele2_eRegrInput_eLeft,"ele2_eRegrInput_eLeft/F");
 myTree_ -> Branch("ele2_eRegrInput_eRight",&ele2_eRegrInput_eRight,"ele2_eRegrInput_eRight/F");
 myTree_ -> Branch("ele2_eRegrInput_e2x5Max",&ele2_eRegrInput_e2x5Max,"ele2_eRegrInput_e2x5Max/F");
 myTree_ -> Branch("ele2_eRegrInput_e2x5Top",&ele2_eRegrInput_e2x5Top,"ele2_eRegrInput_e2x5Top/F");
 myTree_ -> Branch("ele2_eRegrInput_e2x5Bottom",&ele2_eRegrInput_e2x5Bottom,"ele2_eRegrInput_e2x5Bottom/F");
 myTree_ -> Branch("ele2_eRegrInput_e2x5Left",&ele2_eRegrInput_e2x5Left,"ele2_eRegrInput_e2x5Left/F");
 myTree_ -> Branch("ele2_eRegrInput_e2x5Right",&ele2_eRegrInput_e2x5Right,"ele2_eRegrInput_e2x5Right/F");
 myTree_ -> Branch("ele2_eRegrInput_seed_ieta",&ele2_eRegrInput_seed_ieta,"ele2_eRegrInput_seed_ieta/F");
 myTree_ -> Branch("ele2_eRegrInput_seed_iphi",&ele2_eRegrInput_seed_iphi,"ele2_eRegrInput_seed_iphi/F");
 myTree_ -> Branch("ele2_eRegrInput_seed_etaCrySeed",&ele2_eRegrInput_seed_etaCrySeed,"ele2_eRegrInput_seed_etaCrySeed/F");
 myTree_ -> Branch("ele2_eRegrInput_seed_phiCrySeed",&ele2_eRegrInput_seed_phiCrySeed,"ele2_eRegrInput_seed_phiCrySeed/F");
 myTree_ -> Branch("ele2_eRegrInput_preshowerEnergyOverRaw",&ele2_eRegrInput_preshowerEnergyOverRaw,"ele2_eRegrInput_preshowerEnergyOverRaw/F");
 myTree_ -> Branch("ele2_eRegrInput_ecalDrivenSeed",&ele2_eRegrInput_ecalDrivenSeed,"ele2_eRegrInput_ecalDrivenSeed/F");
 myTree_ -> Branch("ele2_eRegrInput_isEBEtaGap",&ele2_eRegrInput_isEBEtaGap,"ele2_eRegrInput_isEBEtaGap/F");
 myTree_ -> Branch("ele2_eRegrInput_isEBPhiGap",&ele2_eRegrInput_isEBPhiGap,"ele2_eRegrInput_isEBPhiGap/F");
 myTree_ -> Branch("ele2_eRegrInput_eSubClusters",&ele2_eRegrInput_eSubClusters,"ele2_eRegrInput_eSubClusters/F");
 myTree_ -> Branch("ele2_eRegrInput_subClusterEnergy_1",&ele2_eRegrInput_subClusterEnergy_1,"ele2_eRegrInput_subClusterEnergy_1/F");
 myTree_ -> Branch("ele2_eRegrInput_subClusterEnergy_2",&ele2_eRegrInput_subClusterEnergy_2,"ele2_eRegrInput_subClusterEnergy_2/F");
 myTree_ -> Branch("ele2_eRegrInput_subClusterEnergy_3",&ele2_eRegrInput_subClusterEnergy_3,"ele2_eRegrInput_subClusterEnergy_3/F");
 myTree_ -> Branch("ele2_eRegrInput_subClusterEta_1",&ele2_eRegrInput_subClusterEta_1,"ele2_eRegrInput_subClusterEta_1/F");
 myTree_ -> Branch("ele2_eRegrInput_subClusterEta_2",&ele2_eRegrInput_subClusterEta_2,"ele2_eRegrInput_subClusterEta_2/F");
 myTree_ -> Branch("ele2_eRegrInput_subClusterEta_3",&ele2_eRegrInput_subClusterEta_3,"ele2_eRegrInput_subClusterEta_3/F");
 myTree_ -> Branch("ele2_eRegrInput_subClusterPhi_1",&ele2_eRegrInput_subClusterPhi_1,"ele2_eRegrInput_subClusterPhi_1/F");
 myTree_ -> Branch("ele2_eRegrInput_subClusterPhi_2",&ele2_eRegrInput_subClusterPhi_2,"ele2_eRegrInput_subClusterPhi_2/F");
 myTree_ -> Branch("ele2_eRegrInput_subClusterPhi_3",&ele2_eRegrInput_subClusterPhi_3,"ele2_eRegrInput_subClusterPhi_3/F");
 myTree_ -> Branch("ele2_eRegrInput_subClusterEmax_1",&ele2_eRegrInput_subClusterEmax_1,"ele2_eRegrInput_subClusterEmax_1/F");
 myTree_ -> Branch("ele2_eRegrInput_subClusterEmax_2",&ele2_eRegrInput_subClusterEmax_2,"ele2_eRegrInput_subClusterEmax_2/F");
 myTree_ -> Branch("ele2_eRegrInput_subClusterEmax_3",&ele2_eRegrInput_subClusterEmax_3,"ele2_eRegrInput_subClusterEmax_3/F");
 myTree_ -> Branch("ele2_eRegrInput_subClusterE3x3_1",&ele2_eRegrInput_subClusterE3x3_1,"ele2_eRegrInput_subClusterE3x3_1/F");
 myTree_ -> Branch("ele2_eRegrInput_subClusterE3x3_2",&ele2_eRegrInput_subClusterE3x3_2,"ele2_eRegrInput_subClusterE3x3_2/F");
 myTree_ -> Branch("ele2_eRegrInput_subClusterE3x3_3",&ele2_eRegrInput_subClusterE3x3_3,"ele2_eRegrInput_subClusterE3x3_3/F");
 myTree_ -> Branch("ele2_eRegrInput_eESClusters",&ele2_eRegrInput_eESClusters,"ele2_eRegrInput_eESClusters/F");
 myTree_ -> Branch("ele2_eRegrInput_eESClusterEnergy_1",&ele2_eRegrInput_eESClusterEnergy_1,"ele2_eRegrInput_eESClusterEnergy_1/F");
 myTree_ -> Branch("ele2_eRegrInput_eESClusterEnergy_2",&ele2_eRegrInput_eESClusterEnergy_2,"ele2_eRegrInput_eESClusterEnergy_2/F");
 myTree_ -> Branch("ele2_eRegrInput_eESClusterEnergy_3",&ele2_eRegrInput_eESClusterEnergy_3,"ele2_eRegrInput_eESClusterEnergy_3/F");
 myTree_ -> Branch("ele2_eRegrInput_eESClusterEta_1",&ele2_eRegrInput_eESClusterEta_1,"ele2_eRegrInput_eESClusterEta_1/F");
 myTree_ -> Branch("ele2_eRegrInput_eESClusterEta_2",&ele2_eRegrInput_eESClusterEta_2,"ele2_eRegrInput_eESClusterEta_2/F");
 myTree_ -> Branch("ele2_eRegrInput_eESClusterEta_3",&ele2_eRegrInput_eESClusterEta_3,"ele2_eRegrInput_eESClusterEta_3/F");
 myTree_ -> Branch("ele2_eRegrInput_eESClusterPhi_1",&ele2_eRegrInput_eESClusterPhi_1,"ele2_eRegrInput_eESClusterPhi_1/F");
 myTree_ -> Branch("ele2_eRegrInput_eESClusterPhi_2",&ele2_eRegrInput_eESClusterPhi_2,"ele2_eRegrInput_eESClusterPhi_2/F");
 myTree_ -> Branch("ele2_eRegrInput_eESClusterPhi_3",&ele2_eRegrInput_eESClusterPhi_3,"ele2_eRegrInput_eESClusterPhi_3/F");
 myTree_ -> Branch("ele2_eRegrInput_pt",&ele2_eRegrInput_pt,"ele2_eRegrInput_pt/F");
 myTree_ -> Branch("ele2_eRegrInput_trackMomentumAtVtxR",&ele2_eRegrInput_trackMomentumAtVtxR,"ele2_eRegrInput_trackMomentumAtVtxR/F");
 myTree_ -> Branch("ele2_eRegrInput_fbrem",&ele2_eRegrInput_fbrem,"ele2_eRegrInput_fbrem/F");
 myTree_ -> Branch("ele2_eRegrInput_charge",&ele2_eRegrInput_charge,"ele2_eRegrInput_charge/F");
 myTree_ -> Branch("ele2_eRegrInput_eSuperClusterOverP",&ele2_eRegrInput_eSuperClusterOverP,"ele2_eRegrInput_eSuperClusterOverP/F");

 myTree_ -> Branch("ele2_nGgsfTrackHits",&ele2_nGgsfTrackHits,"ele2_nGgsfTrackHits/I");
 myTree_ -> Branch("ele2_numberOfLostHits",&ele2_numberOfLostHits,"ele2_numberOfLostHits/I");
 myTree_ -> Branch("ele2_nAmbiguousGsfTrack",&ele2_nAmbiguousGsfTrack,"ele2_nAmbiguousGsfTrack/I");


 // met variables
 if(verbosity_) std::cout << ">>>>>> AnalyzerEle::AnalyzerEle::set met branches <<<" << std::endl;
 myTree_ -> Branch("met_et", &met_et, "met_et/F");
 myTree_ -> Branch("met_phi", &met_phi, "met_phi/F");
 myTree_ -> Branch("ele1Met_mt", &ele1Met_mt, "ele1Met_mt/F");
 myTree_ -> Branch("ele1Met_Dphi",&ele1Met_Dphi,"ele1Met_Dphi/F");

 // di-electron variables
 if(verbosity_) std::cout << ">>>>>> AnalyzerEle::AnalyzerEle::set dielectron branches <<<" << std::endl;
 myTree_ -> Branch("ele1ele2_m", &ele1ele2_m, "ele1ele2_m/F");
 myTree_ -> Branch("ele1ele2_scM", &ele1ele2_scM, "ele1ele2_scM/F");

 if(verbosity_) std::cout << ">>> AnalyzerEle::AnalyzerEle end <<<" << std::endl;

}

// ------------ method called once each job just after ending the event loop ------------
void AnalyzerEle::endJob(){
  if(myTree_->GetEntries()>0) myTree_->BuildIndex("runId","eventId");  
}

// -----------------------------------------------------------------------------------------
void AnalyzerEle::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {

 if(verbosity_) std::cout << ">>> AnalyzerEle::analyze begin <<<" << std::endl;

 ++eventNaiveId_;

 bool isGoodEvent = false;
 
 initialize(); // set all the branch variables to the default

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
 // make selection for W and Z topology
 if(doWZSelection_){

   std::map<float,int> eleIts_ ; //map to take track of the electron inside the collection 1/pt and position 
   eleIts_.clear(); // clear the list in every event

   int nWP70 = 0; //only WP70
   int nWP90 = 0; //only WP90

   for( unsigned int iEle = 0 ; iEle < electrons.size(); iEle++){ // Loop on the electron 
     if(electrons.at(iEle).electronID("cIso70") or electrons.at(iEle).electronID("relIso70")){ //if the electron is WP70 combined or absolute isolation 
          ++nWP70; ++nWP90;
          eleIts_[1./electrons.at(iEle).pt()] = iEle; // fill the map and this is both a WP70 and WP90
	  continue;
     }

     if(electrons.at(iEle).electronID("cIso90") or electrons.at(iEle).electronID("relIso90")){ // if the electron is WP90 combined or absolute isolation
	  ++nWP90;
          eleIts_[1./electrons.at(iEle).pt()] = iEle; 
	  continue;
      }
   }

   if(nWP90 > 2) return; // less than one WP70 or more than two WP90 -> skip the event

   if((nWP70 == 1) && (nWP90 < 2)){ // less than two WP90 and one WP70 -> W candidate

    isW_ = 1; isZ_ = 0;
    std::map<float,int>::const_iterator mapIt = eleIts_.begin();
    fillEleInfo ( iEvent, iSetup, mapIt->second, "ele1" ); //fill the electron variables for ele1
    fillMetInfo (iEvent, iSetup); // fill the met
    isGoodEvent = myWselection ( iEvent, iSetup); // make the W selection
    if (isGoodEvent) myTree_->Fill();  // Fill the tree
  }

  if( nWP70 == 2 or nWP90 == 2 or (nWP70 == 1 and nWP90 == 2 ) ){ // if two  WP70 or two WP90 

   isW_ = 0; isZ_ = 1;

   std::map<float,int>::const_iterator mapIt = eleIts_.begin();
   fillEleInfo ( iEvent, iSetup, mapIt->second, "ele1" ); // fill electron 1 info
   mapIt++;
   fillEleInfo ( iEvent, iSetup, mapIt->second, "ele2" ); // fill electron 2 info
   fillDoubleEleInfo (iEvent, iSetup); // fill double ele info
   fillMetInfo (iEvent, iSetup); // fill met info
   isGoodEvent = myZselection ( iEvent, iSetup); // make the Z selections
   if (isGoodEvent) myTree_ -> Fill(); // fill the tree
  }
 }
 // without doing W/Z selections
 else {
   
  int nele = electrons.size();

  if(applyElectronID_){

   std::map<float,int> eleIts_ ; //map to take track of the electron inside the collection 1/pt and position 
   eleIts_.clear(); // clear the list in every event

   int nWP80 = 0; //only WP70

   for( unsigned int iEle = 0 ; iEle < electrons.size(); iEle++){ // Loop on the electron 
     if(electrons.at(iEle).electronID("cIso80") or electrons.at(iEle).electronID("relIso80")){ //if the electron is WP70 combined or absolute isolation 
          ++nWP80;
          eleIts_[1./electrons.at(iEle).pt()] = iEle; // fill the map and this is both a WP70 and WP90
	  continue;
     }
   }

   if(nWP80 == 0 or nWP80 > 2 ) return ;
   if(nWP80 == 1){ isW_ = 1; isZ_ = 0; };
   if(nWP80 == 2){ isW_ = 0; isZ_ = 1; };
   if(isW_ == 1) fillEleInfo ( iEvent, iSetup, 0, "ele1" );
   if(isZ_ == 1) {
    fillEleInfo ( iEvent, iSetup, 0, "ele1" );
    fillEleInfo ( iEvent, iSetup, 1, "ele2" );
    fillDoubleEleInfo (iEvent, iSetup);
   }
   fillMetInfo (iEvent, iSetup);
   if (isW_ == 1 || isZ_ == 1) myTree_ -> Fill();
  }
  else{

   if(nele == 1) { isW_ = 1; isZ_ = 0; }
   if(nele == 2) { isW_ = 0; isZ_ = 1; }
   if(isW_ == 1) fillEleInfo ( iEvent, iSetup, 0, "ele1" );
   if(isZ_ == 1) {
    fillEleInfo ( iEvent, iSetup, 0, "ele1" );
    fillEleInfo ( iEvent, iSetup, 1, "ele2" );
    fillDoubleEleInfo (iEvent, iSetup);
   }
   fillMetInfo (iEvent, iSetup);
   if (isW_ == 1 || isZ_ == 1) myTree_ -> Fill();
  }

 }
 if( verbosity_ ) std::cout << ">>> AnalyzerEle::analyze end <<<" << std::endl;
}


// ----------------------------- Selection for W events
bool AnalyzerEle::myWselection (const edm::Event & iEvent, const edm::EventSetup & iSetup){
  
 if( ele1_pt < 30. )       return false;         
 if( met_et < 25.00 )      return false;
 if( ele1Met_mt < 50.00 )  return false;
 if( ele1Met_Dphi < 1.57 ) return false;
  
 return true;
  
}

// -------------------------- Selection for Z events
bool AnalyzerEle::myZselection (const edm::Event & iEvent, const edm::EventSetup & iSetup){

 if( met_et > 40. )      return false;
 if( ele1ele2_m < 60. )  return false;
 if( ele1ele2_m > 120. ) return false;
 if( (ele1_charge * ele2_charge) != -1. ) return false;
      
 return true;
  
}

// --------------------- Add primary vertex collection information
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

 iEvent.getByLabel(PVTag_alternative_, vertexes);

 PV_n_alternative_ = vertexes -> size();

 PVfound = (vertexes -> size() != 0);

 if(PVfound){    
  PV = vertexes->at(0);
  PV_z_alternative_ = PV.z();
  PV_d0_alternative_ = PV.position().Rho();
 }
 else {
  PV_z_alternative_  = -999.;
  PV_d0_alternative_ = -999.;
 }

 math::XYZPoint PVPoint_alterantive(PV.position().x(), PV.position().y(), PV.position().z());
 PVPoint_alternative_ = PVPoint_alterantive ;

}

//----------------------------------- Rho info for PU subtraction 
void AnalyzerEle::fillRhoInfo(const edm::Event & iEvent, const edm::EventSetup & iSetup){
  
 edm::Handle< double > rhoHandle;
 iEvent.getByLabel(rhoTag_,rhoHandle);
 rho_ = *rhoHandle;
}

//----------------------------------- Dumpm HLT information
void AnalyzerEle::fillHLTInfo(const edm::Event & iEvent, const edm::EventSetup & iSetup){

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

//----------------------------------- Dump Pile Up info only for MC
void AnalyzerEle::fillPileUpInfo (const edm::Event & iEvent, const edm::EventSetup & iSetup){

 edm::Handle<std::vector< PileupSummaryInfo > > PupInfo;
 iEvent.getByLabel(edm::InputTag("addPileupInfo"), PupInfo);
 std::vector<PileupSummaryInfo>::const_iterator PVI;
 
 for(PVI = PupInfo->begin(); PVI != PupInfo->end(); ++PVI)
   nPU_ = PVI->getTrueNumInteractions();
 
 
}

//--------------------------------------- Dump Generator information 
void AnalyzerEle::fillGeneratorInfo(const edm::Event & iEvent, const edm::EventSetup & iSetup){

 edm::Handle<reco::GenParticleCollection> genParticles; // take the genParticle collection from the event
 iEvent.getByLabel(MCtruthTag_, genParticles);

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
  
  
 
// --------------------------------------------------------- Dump two leading electron variables from event 
void AnalyzerEle::fillEleInfo(const edm::Event & iEvent, const edm::EventSetup & iSetup, const int iEle, const std::string eleName) {

 if(verbosity_) std::cout << ">>> AnalyzerEle::fillEleInfo start <<<" << std::endl;
  
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

 //*********** CALO GEOMETRY
 edm::ESHandle<CaloGeometry> theCaloGeometry;
 iSetup.get<CaloGeometryRecord>().get(theCaloGeometry);
 const CaloGeometry* calogeometry = & (*theCaloGeometry);
  
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
 
 
 //************* ELECTRONS
 edm::Handle<edm::View<pat::Electron> > electronHandle;
 iEvent.getByLabel(EleTag_,electronHandle);
 edm::View<pat::Electron> electrons = *electronHandle;
  
 //************* VERTEX COLLECTION
 edm::Handle<reco::VertexCollection> hVertexProduct;
 iEvent.getByLabel(PVTag_,hVertexProduct);
  
 //************* CLUSTER LAZY TOOLS
<<<<<<< Updated upstream
 recHitCollection_EB_token_  = consumes<EcalRecHitCollection>(recHitCollection_EB_);
 recHitCollection_EE_token_  = consumes<EcalRecHitCollection>(recHitCollection_EE_); 
 
 EcalClusterLazyTools lazyTools(iEvent,iSetup,recHitCollection_EB_token_,recHitCollection_EE_token_);
 EcalClusterPUCleaningTools cleaningTools(iEvent, iSetup, recHitCollection_EB_, recHitCollection_EE_);
=======
 EcalClusterLazyTools lazyTools(iEvent,iSetup,  consumes <EcalRecHitCollection>(recHitCollection_EB_),  consumes <EcalRecHitCollection>(recHitCollection_EE_));
>>>>>>> Stashed changes

 // Take the correct ele
 bool printOut = false;
 const pat::Electron electron = electrons.at(iEle);
 
 // Take SC helper for regression input
 if (electron.isEB())
   mySCHelper_ = new SuperClusterHelper(&electron,theBarrelEcalRecHits,topology,calogeometry);
 else
   mySCHelper_ = new SuperClusterHelper(&electron,theEndcapEcalRecHits,topology,calogeometry);

 if(eleName == "ele1") {

  ele1        = electron.p4();
  ele1_charge = electron.charge();
  ele1_p      = ele1.P();
  ele1_pt     = ele1.Pt();
  ele1_eta    = ele1.eta();
  ele1_phi    = ele1.phi();

  ele1_isEB        = electron.isEB();
  ele1_isEBEEGap   = electron.isEBEEGap();
  ele1_isEBEtaGap  = electron.isEBEtaGap();
  ele1_isEBPhiGap  = electron.isEBPhiGap();
  ele1_isEEDeeGap  = electron.isEEDeeGap();
  ele1_isEERingGap = electron.isEERingGap();
 
  ele1_idtype.assign(7,0);
  if(electron.electronID("fiducial"))  ele1_idtype.at(0) = 1;
  if(electron.electronID("relIso90"))  ele1_idtype.at(1) = 1;
  if(electron.electronID("relIso80"))  ele1_idtype.at(2) = 1;
  if(electron.electronID("relIso70"))  ele1_idtype.at(3) = 1;
  if(electron.electronID("cIso90"))    ele1_idtype.at(4) = 1;
  if(electron.electronID("cIso80"))    ele1_idtype.at(5) = 1;
  if(electron.electronID("cIso70"))    ele1_idtype.at(6) = 1;

  ele1_isTrackerDriven = !(electron.ecalDriven());
  ele1_classification  = electron.classification();
  ele1_sigmaIetaIeta   = electron.sigmaIetaIeta();
  ele1_DphiIn          = electron.deltaPhiSuperClusterTrackAtVtx();
  ele1_DetaIn          = electron.deltaEtaSuperClusterTrackAtVtx();

  ele1_deltaEtaEleClusterTrackAtCalo  = electron.deltaEtaEleClusterTrackAtCalo();
  ele1_deltaEtaPhiClusterTrackAtCalo  = electron.deltaPhiEleClusterTrackAtCalo();
  ele1_deltaEtaSeedClusterTrackAtCalo = electron.deltaEtaSeedClusterTrackAtCalo();
  ele1_deltaPhiSeedClusterTrackAtCalo = electron.deltaPhiSeedClusterTrackAtCalo();

  ele1_HOverE  = electron.hadronicOverEm();
  ele1_ooemoop = (1.0/electron.ecalEnergy()-electron.eSuperClusterOverP()/electron.ecalEnergy());
  ele1_tkIso   = electron.dr03TkSumPt();
  ele1_emIso   = electron.dr03EcalRecHitSumEt();
  ele1_hadIso  = electron.dr03HcalDepth1TowerSumEt()+electron.dr03HcalDepth2TowerSumEt();

  reco::GsfTrackRef eleTrack = electron.gsfTrack();
  ele1_dxy_PV  = eleTrack->dxy (PVPoint_);
  ele1_dz_PV   = eleTrack->dz (PVPoint_);
  ele1_dxy_PV_alternative  = eleTrack->dxy (PVPoint_alternative_);
  ele1_dz_PV_alternative   = eleTrack->dz (PVPoint_alternative_);
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

   if(printOut) std::cout << "*** EB ***" << std::endl;

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

   if(printOut) std::cout << "*** EE ***" << std::endl;      

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

  // Regression V3 variables                                                                                                                                                         
  ele1_eRegrInput_rawE   = mySCHelper_->rawEnergy();
  ele1_eRegrInput_r9     = mySCHelper_->r9(); 
  ele1_eRegrInput_eta    = mySCHelper_->eta();
  ele1_eRegrInput_phi    = mySCHelper_->phi();
  ele1_eRegrInput_etaW   = mySCHelper_->etaWidth();
  ele1_eRegrInput_phiW   = mySCHelper_->phiWidth();
  ele1_eRegrInput_SCsize = mySCHelper_->clustersSize();
  ele1_eRegrInput_rho    = rho_; 
  ele1_eRegrInput_hoe    = mySCHelper_->hadronicOverEm(); 
  ele1_eRegrInput_nPV    = PV_n_;
  ele1_eRegrInput_seed_eta = mySCHelper_->seedEta();
  ele1_eRegrInput_seed_phi = mySCHelper_->seedPhi();
  ele1_eRegrInput_seed_E   = mySCHelper_->seedEnergy();
  ele1_eRegrInput_seed_e3x3 = mySCHelper_->e3x3();
  ele1_eRegrInput_seed_e5x5 = mySCHelper_->e5x5();
  ele1_eRegrInput_sigietaieta = mySCHelper_->sigmaIetaIeta();
  ele1_eRegrInput_sigiphiiphi = mySCHelper_->spp();
  ele1_eRegrInput_sigietaiphi = mySCHelper_->sep();
  ele1_eRegrInput_eMax        = mySCHelper_->eMax();
  ele1_eRegrInput_e2nd        = mySCHelper_->e2nd();
  ele1_eRegrInput_eTop        = mySCHelper_->eTop();
  ele1_eRegrInput_eBottom     = mySCHelper_->eBottom();
  ele1_eRegrInput_eLeft   = mySCHelper_->eLeft();
  ele1_eRegrInput_eRight  = mySCHelper_->eRight();
  ele1_eRegrInput_e2x5Max = mySCHelper_->e2x5Max();
  ele1_eRegrInput_e2x5Top = mySCHelper_->e2x5Top();
  ele1_eRegrInput_e2x5Bottom = mySCHelper_->e2x5Bottom();
  ele1_eRegrInput_e2x5Left   = mySCHelper_->e2x5Left();
  ele1_eRegrInput_e2x5Right  = mySCHelper_->e2x5Right();
  ele1_eRegrInput_seed_ieta  = mySCHelper_->ietaSeed();
  ele1_eRegrInput_seed_iphi  = mySCHelper_->iphiSeed();
  ele1_eRegrInput_seed_etaCrySeed = mySCHelper_->etaCrySeed();
  ele1_eRegrInput_seed_phiCrySeed = mySCHelper_->phiCrySeed();
  ele1_eRegrInput_preshowerEnergyOverRaw = mySCHelper_->preshowerEnergyOverRaw();
  ele1_eRegrInput_ecalDrivenSeed = electron.ecalDrivenSeed();
  ele1_eRegrInput_isEBEtaGap = electron.isEBEtaGap();
  ele1_eRegrInput_isEBPhiGap = electron.isEBPhiGap();
  ele1_eRegrInput_eSubClusters = mySCHelper_->eSubClusters();
  ele1_eRegrInput_subClusterEnergy_1 = mySCHelper_->subClusterEnergy(1);
  ele1_eRegrInput_subClusterEnergy_2 = mySCHelper_->subClusterEnergy(2);
  ele1_eRegrInput_subClusterEnergy_3 = mySCHelper_->subClusterEnergy(3);
  ele1_eRegrInput_subClusterEta_1 = mySCHelper_->subClusterEta(1);
  ele1_eRegrInput_subClusterEta_2 = mySCHelper_->subClusterEta(2);
  ele1_eRegrInput_subClusterEta_3 = mySCHelper_->subClusterEta(3);
  ele1_eRegrInput_subClusterPhi_1 = mySCHelper_->subClusterPhi(1);
  ele1_eRegrInput_subClusterPhi_2 = mySCHelper_->subClusterPhi(2);
  ele1_eRegrInput_subClusterPhi_3 = mySCHelper_->subClusterPhi(3);
  ele1_eRegrInput_subClusterEmax_1 = mySCHelper_->subClusterEmax(1);
  ele1_eRegrInput_subClusterEmax_2 = mySCHelper_->subClusterEmax(2);
  ele1_eRegrInput_subClusterEmax_3 = mySCHelper_->subClusterEmax(3);
  ele1_eRegrInput_subClusterE3x3_1 = mySCHelper_->subClusterE3x3(1);
  ele1_eRegrInput_subClusterE3x3_2 = mySCHelper_->subClusterE3x3(2);
  ele1_eRegrInput_subClusterE3x3_3 = mySCHelper_->subClusterE3x3(3);
  ele1_eRegrInput_eESClusters = mySCHelper_->eESClusters();
  ele1_eRegrInput_eESClusterEnergy_1 = mySCHelper_->esClusterEnergy(0);
  ele1_eRegrInput_eESClusterEnergy_2 = mySCHelper_->esClusterEnergy(1);
  ele1_eRegrInput_eESClusterEnergy_3 = mySCHelper_->esClusterEnergy(2);
  ele1_eRegrInput_eESClusterEta_1 = mySCHelper_->esClusterEta(0);
  ele1_eRegrInput_eESClusterEta_2 = mySCHelper_->esClusterEta(1);
  ele1_eRegrInput_subClusterEta_3 = mySCHelper_->esClusterEta(2);
  ele1_eRegrInput_subClusterPhi_1 = mySCHelper_->esClusterPhi(0);
  ele1_eRegrInput_subClusterPhi_2 = mySCHelper_->esClusterPhi(1);
  ele1_eRegrInput_subClusterPhi_3 = mySCHelper_->esClusterPhi(2);
  ele1_eRegrInput_pt = electron.pt();
  ele1_eRegrInput_trackMomentumAtVtxR = electron.trackMomentumAtVtx().R();
  ele1_eRegrInput_fbrem = electron.fbrem();
  ele1_eRegrInput_charge = electron.charge();
  ele1_eRegrInput_eSuperClusterOverP = electron.eSuperClusterOverP();

  ele1_nGgsfTrackHits = eleTrack->recHitsSize();
  ele1_numberOfLostHits = eleTrack->trackerExpectedHitsInner().numberOfLostHits();
  ele1_nAmbiguousGsfTrack = electron.ambiguousGsfTracksSize();

    
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

  ele2_idtype.assign(7,0);
  if(electron.electronID("fiducial"))  ele2_idtype.at(0) = 1;
  if(electron.electronID("relIso90"))  ele2_idtype.at(1) = 1;
  if(electron.electronID("relIso80"))  ele2_idtype.at(2) = 1;
  if(electron.electronID("relIso70"))  ele2_idtype.at(3) = 1;
  if(electron.electronID("cIso90"))    ele2_idtype.at(4) = 1;
  if(electron.electronID("cIso80"))    ele2_idtype.at(5) = 1;
  if(electron.electronID("cIso70"))    ele2_idtype.at(6) = 1;

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
  ele2_dxy_PV_alternative  = eleTrack->dxy (PVPoint_alternative_);
  ele2_dz_PV_alternative   = eleTrack->dz (PVPoint_alternative_);
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

  // Regression V3 variables                                                                                                                                                         
  ele2_eRegrInput_rawE   = mySCHelper_->rawEnergy();
  ele2_eRegrInput_r9     = mySCHelper_->r9(); 
  ele2_eRegrInput_eta    = mySCHelper_->eta();
  ele2_eRegrInput_phi    = mySCHelper_->phi();
  ele2_eRegrInput_etaW   = mySCHelper_->etaWidth();
  ele2_eRegrInput_phiW   = mySCHelper_->phiWidth();
  ele2_eRegrInput_SCsize = mySCHelper_->clustersSize();
  ele2_eRegrInput_rho    = rho_; 
  ele2_eRegrInput_hoe    = mySCHelper_->hadronicOverEm(); 
  ele2_eRegrInput_nPV    = PV_n_;
  ele2_eRegrInput_seed_eta = mySCHelper_->seedEta();
  ele2_eRegrInput_seed_phi = mySCHelper_->seedPhi();
  ele2_eRegrInput_seed_E   = mySCHelper_->seedEnergy();
  ele2_eRegrInput_seed_e3x3 = mySCHelper_->e3x3();
  ele2_eRegrInput_seed_e5x5 = mySCHelper_->e5x5();
  ele2_eRegrInput_sigietaieta = mySCHelper_->sigmaIetaIeta();
  ele2_eRegrInput_sigiphiiphi = mySCHelper_->spp();
  ele2_eRegrInput_sigietaiphi = mySCHelper_->sep();
  ele2_eRegrInput_eMax        = mySCHelper_->eMax();
  ele2_eRegrInput_e2nd        = mySCHelper_->e2nd();
  ele2_eRegrInput_eTop        = mySCHelper_->eTop();
  ele2_eRegrInput_eBottom     = mySCHelper_->eBottom();
  ele2_eRegrInput_eLeft   = mySCHelper_->eLeft();
  ele2_eRegrInput_eRight  = mySCHelper_->eRight();
  ele2_eRegrInput_e2x5Max = mySCHelper_->e2x5Max();
  ele2_eRegrInput_e2x5Top = mySCHelper_->e2x5Top();
  ele2_eRegrInput_e2x5Bottom = mySCHelper_->e2x5Bottom();
  ele2_eRegrInput_e2x5Left   = mySCHelper_->e2x5Left();
  ele2_eRegrInput_e2x5Right  = mySCHelper_->e2x5Right();
  ele2_eRegrInput_seed_ieta  = mySCHelper_->ietaSeed();
  ele2_eRegrInput_seed_iphi  = mySCHelper_->iphiSeed();
  ele2_eRegrInput_seed_etaCrySeed = mySCHelper_->etaCrySeed();
  ele2_eRegrInput_seed_phiCrySeed = mySCHelper_->phiCrySeed();
  ele2_eRegrInput_preshowerEnergyOverRaw = mySCHelper_->preshowerEnergyOverRaw();
  ele2_eRegrInput_ecalDrivenSeed = electron.ecalDrivenSeed();
  ele2_eRegrInput_isEBEtaGap = electron.isEBEtaGap();
  ele2_eRegrInput_isEBPhiGap = electron.isEBPhiGap();
  ele2_eRegrInput_eSubClusters = mySCHelper_->eSubClusters();
  ele2_eRegrInput_subClusterEnergy_1 = mySCHelper_->subClusterEnergy(1);
  ele2_eRegrInput_subClusterEnergy_2 = mySCHelper_->subClusterEnergy(2);
  ele2_eRegrInput_subClusterEnergy_3 = mySCHelper_->subClusterEnergy(3);
  ele2_eRegrInput_subClusterEta_1 = mySCHelper_->subClusterEta(1);
  ele2_eRegrInput_subClusterEta_2 = mySCHelper_->subClusterEta(2);
  ele2_eRegrInput_subClusterEta_3 = mySCHelper_->subClusterEta(3);
  ele2_eRegrInput_subClusterPhi_1 = mySCHelper_->subClusterPhi(1);
  ele2_eRegrInput_subClusterPhi_2 = mySCHelper_->subClusterPhi(2);
  ele2_eRegrInput_subClusterPhi_3 = mySCHelper_->subClusterPhi(3);
  ele2_eRegrInput_subClusterEmax_1 = mySCHelper_->subClusterEmax(1);
  ele2_eRegrInput_subClusterEmax_2 = mySCHelper_->subClusterEmax(2);
  ele2_eRegrInput_subClusterEmax_3 = mySCHelper_->subClusterEmax(3);
  ele2_eRegrInput_subClusterE3x3_1 = mySCHelper_->subClusterE3x3(1);
  ele2_eRegrInput_subClusterE3x3_2 = mySCHelper_->subClusterE3x3(2);
  ele2_eRegrInput_subClusterE3x3_3 = mySCHelper_->subClusterE3x3(3);
  ele2_eRegrInput_eESClusters = mySCHelper_->eESClusters();
  ele2_eRegrInput_eESClusterEnergy_1 = mySCHelper_->esClusterEnergy(0);
  ele2_eRegrInput_eESClusterEnergy_2 = mySCHelper_->esClusterEnergy(1);
  ele2_eRegrInput_eESClusterEnergy_3 = mySCHelper_->esClusterEnergy(2);
  ele2_eRegrInput_eESClusterEta_1 = mySCHelper_->esClusterEta(0);
  ele2_eRegrInput_eESClusterEta_2 = mySCHelper_->esClusterEta(1);
  ele2_eRegrInput_subClusterEta_3 = mySCHelper_->esClusterEta(2);
  ele2_eRegrInput_subClusterPhi_1 = mySCHelper_->esClusterPhi(0);
  ele2_eRegrInput_subClusterPhi_2 = mySCHelper_->esClusterPhi(1);
  ele2_eRegrInput_subClusterPhi_3 = mySCHelper_->esClusterPhi(2);
  ele2_eRegrInput_pt = electron.pt();
  ele2_eRegrInput_trackMomentumAtVtxR = electron.trackMomentumAtVtx().R();
  ele2_eRegrInput_fbrem = electron.fbrem();
  ele2_eRegrInput_charge = electron.charge();
  ele2_eRegrInput_eSuperClusterOverP = electron.eSuperClusterOverP();

  ele2_nGgsfTrackHits = eleTrack->recHitsSize();
  ele2_numberOfLostHits = eleTrack->trackerExpectedHitsInner().numberOfLostHits();
  ele2_nAmbiguousGsfTrack = electron.ambiguousGsfTracksSize();


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
  ele1Met_Dphi = deltaPhi(ele2_phi,met_phi);


 }

//----------------------------------------------------------------------------------------------

void AnalyzerEle::fillDoubleEleInfo(const edm::Event & iEvent, const edm::EventSetup & iSetup){

  ele1ele2_m = (ele1 + ele2).mass();
  
  ROOT::Math::PtEtaPhiEVector ele1_sc(ele1_scE*sin(2*atan(exp(-1.*ele1_eta))),ele1_eta,ele1_phi,ele1_scE);
  ROOT::Math::PtEtaPhiEVector ele2_sc(ele2_scE*sin(2*atan(exp(-1.*ele2_eta))),ele2_eta,ele2_phi,ele2_scE);
  ele1ele2_scM = (ele1_sc + ele2_sc).mass();

 }


//----------------------------------------------------------------------------------------------

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

 ele1_idtype.clear() ;     
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
 ele1_dxy_PV_alternative=-99.;
 ele1_dz_PV_alternative=-99.;
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

 ele1_isEB= -9999;
 ele1_isEBEEGap= -9999;
 ele1_isEBEtaGap= -9999;
 ele1_isEBPhiGap= -9999;
 ele1_isEEDeeGap= -9999;
 ele1_isEERingGap= -9999;

 // Regression V3 variables                                                                                                                                                               
 ele1_eRegrInput_rawE = -999.;
 ele1_eRegrInput_r9   = -999.;
 ele1_eRegrInput_eta  = -999.;
 ele1_eRegrInput_phi  = -999.;
 ele1_eRegrInput_etaW = -999.;
 ele1_eRegrInput_phiW = -999.;
 ele1_eRegrInput_SCsize  = -999;
 ele1_eRegrInput_rho  = -999.;
 ele1_eRegrInput_hoe  = -999.;
 ele1_eRegrInput_nPV  = -999;
 ele1_eRegrInput_seed_eta  = -999.;
 ele1_eRegrInput_seed_phi  = -999.;
 ele1_eRegrInput_seed_E    = -999.;
 ele1_eRegrInput_seed_e3x3  = -999.;
 ele1_eRegrInput_seed_e5x5  = -999.;
 ele1_eRegrInput_sigietaieta = -999.;
 ele1_eRegrInput_sigiphiiphi = -999.;
 ele1_eRegrInput_sigietaiphi = -999. ;
 ele1_eRegrInput_eMax = -999.;
 ele1_eRegrInput_e2nd = -999.;
 ele1_eRegrInput_eTop = -999.;
 ele1_eRegrInput_eBottom = -999.;
 ele1_eRegrInput_eLeft   = -999.;
 ele1_eRegrInput_eRight  = -999.;
 ele1_eRegrInput_e2x5Max = -999.;
 ele1_eRegrInput_e2x5Top = -999.;
 ele1_eRegrInput_e2x5Bottom = -999.;
 ele1_eRegrInput_e2x5Left   = -999.;
 ele1_eRegrInput_e2x5Right = -999.;
 ele1_eRegrInput_seed_ieta = -999;
 ele1_eRegrInput_seed_iphi = -999;
 ele1_eRegrInput_seed_etaCrySeed = -999.;
 ele1_eRegrInput_seed_phiCrySeed = -999.;
 ele1_eRegrInput_preshowerEnergyOverRaw = -999.;
 ele1_eRegrInput_ecalDrivenSeed = -999;
 ele1_eRegrInput_isEBEtaGap = -999;
 ele1_eRegrInput_isEBPhiGap = -999;
 ele1_eRegrInput_eSubClusters = -999.;
 ele1_eRegrInput_subClusterEnergy_1 = -999.;
 ele1_eRegrInput_subClusterEnergy_2 = -999.;
 ele1_eRegrInput_subClusterEnergy_3 = -999.;
 ele1_eRegrInput_subClusterEta_1 = -999.;
 ele1_eRegrInput_subClusterEta_2 = -999.;
 ele1_eRegrInput_subClusterEta_3 = -999.;
 ele1_eRegrInput_subClusterPhi_1 = -999.;
 ele1_eRegrInput_subClusterPhi_2 = -999.;
 ele1_eRegrInput_subClusterPhi_3 = -999.;
 ele1_eRegrInput_subClusterEmax_1 = -999.;
 ele1_eRegrInput_subClusterEmax_2 = -999.;
 ele1_eRegrInput_subClusterEmax_3 = -999.;
 ele1_eRegrInput_subClusterE3x3_1 = -999.;
 ele1_eRegrInput_subClusterE3x3_2 = -999.;
 ele1_eRegrInput_subClusterE3x3_3 = -999.;
 ele1_eRegrInput_eESClusters = -999.;
 ele1_eRegrInput_eESClusterEnergy_1 = -999.;
 ele1_eRegrInput_eESClusterEnergy_2 = -999.;
 ele1_eRegrInput_eESClusterEnergy_3 = -999.;
 ele1_eRegrInput_eESClusterEta_1 = -999.;
 ele1_eRegrInput_eESClusterEta_2 = -999.;
 ele1_eRegrInput_eESClusterEta_3 = -999.;
 ele1_eRegrInput_eESClusterPhi_1 = -999.;
 ele1_eRegrInput_eESClusterPhi_2 = -999.;
 ele1_eRegrInput_pt  = -999.;
 ele1_eRegrInput_trackMomentumAtVtxR = -999.;
 ele1_eRegrInput_fbrem = -999.;
 ele1_eRegrInput_charge = -999.;
 ele1_eRegrInput_eSuperClusterOverP = -999.;

 ele1_nGgsfTrackHits     = -999;
 ele1_numberOfLostHits   = -999;
 ele1_nAmbiguousGsfTrack = -999;


 ele2_charge =-99.;
 ele2_p =-99.;
 ele2_pt =-99.;
 ele2_eta =-99.;
 ele2_phi=-99.;
 ele2_isTrackerDriven=-99;
 ele2_classification = -99;
  
 ele2_idtype.clear() ;     
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
 ele2_dxy_PV_alternative=-99.;
 ele2_dz_PV_alternative=-99.;
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

 ele2_isEB= -9999;
 ele2_isEBEEGap= -9999;
 ele2_isEBEtaGap= -9999;
 ele2_isEBPhiGap= -9999;
 ele2_isEEDeeGap= -9999;
 ele2_isEERingGap= -9999;

 ele2_eRegrInput_rawE = -999.;
 ele2_eRegrInput_r9   = -999.;
 ele2_eRegrInput_eta  = -999.;
 ele2_eRegrInput_phi  = -999.;
 ele2_eRegrInput_etaW = -999.;
 ele2_eRegrInput_phiW = -999.;
 ele2_eRegrInput_SCsize  = -999;
 ele2_eRegrInput_rho  = -999.;
 ele2_eRegrInput_hoe  = -999.;
 ele2_eRegrInput_nPV  = -999;
 ele2_eRegrInput_seed_eta  = -999.;
 ele2_eRegrInput_seed_phi  = -999.;
 ele2_eRegrInput_seed_E    = -999.;
 ele2_eRegrInput_seed_e3x3  = -999.;
 ele2_eRegrInput_seed_e5x5  = -999.;
 ele2_eRegrInput_sigietaieta = -999.;
 ele2_eRegrInput_sigiphiiphi = -999.;
 ele2_eRegrInput_sigietaiphi = -999. ;
 ele2_eRegrInput_eMax = -999.;
 ele2_eRegrInput_e2nd = -999.;
 ele2_eRegrInput_eTop = -999.;
 ele2_eRegrInput_eBottom = -999.;
 ele2_eRegrInput_eLeft   = -999.;
 ele2_eRegrInput_eRight  = -999.;
 ele2_eRegrInput_e2x5Max = -999.;
 ele2_eRegrInput_e2x5Top = -999.;
 ele2_eRegrInput_e2x5Bottom = -999.;
 ele2_eRegrInput_e2x5Left   = -999.;
 ele2_eRegrInput_e2x5Right = -999.;
 ele2_eRegrInput_seed_ieta = -999;
 ele2_eRegrInput_seed_iphi = -999;
 ele2_eRegrInput_seed_etaCrySeed = -999.;
 ele2_eRegrInput_seed_phiCrySeed = -999.;
 ele2_eRegrInput_preshowerEnergyOverRaw = -999.;
 ele2_eRegrInput_ecalDrivenSeed = -999;
 ele2_eRegrInput_isEBEtaGap = -999;
 ele2_eRegrInput_isEBPhiGap = -999;
 ele2_eRegrInput_eSubClusters = -999.;
 ele2_eRegrInput_subClusterEnergy_1 = -999.;
 ele2_eRegrInput_subClusterEnergy_2 = -999.;
 ele2_eRegrInput_subClusterEnergy_3 = -999.;
 ele2_eRegrInput_subClusterEta_1 = -999.;
 ele2_eRegrInput_subClusterEta_2 = -999.;
 ele2_eRegrInput_subClusterEta_3 = -999.;
 ele2_eRegrInput_subClusterPhi_1 = -999.;
 ele2_eRegrInput_subClusterPhi_2 = -999.;
 ele2_eRegrInput_subClusterPhi_3 = -999.;
 ele2_eRegrInput_subClusterEmax_1 = -999.;
 ele2_eRegrInput_subClusterEmax_2 = -999.;
 ele2_eRegrInput_subClusterEmax_3 = -999.;
 ele2_eRegrInput_subClusterE3x3_1 = -999.;
 ele2_eRegrInput_subClusterE3x3_2 = -999.;
 ele2_eRegrInput_subClusterE3x3_3 = -999.;
 ele2_eRegrInput_eESClusters = -999.;
 ele2_eRegrInput_eESClusterEnergy_1 = -999.;
 ele2_eRegrInput_eESClusterEnergy_2 = -999.;
 ele2_eRegrInput_eESClusterEnergy_3 = -999.;
 ele2_eRegrInput_eESClusterEta_1 = -999.;
 ele2_eRegrInput_eESClusterEta_2 = -999.;
 ele2_eRegrInput_eESClusterEta_3 = -999.;
 ele2_eRegrInput_eESClusterPhi_1 = -999.;
 ele2_eRegrInput_eESClusterPhi_2 = -999.;
 ele2_eRegrInput_pt  = -999.;
 ele2_eRegrInput_trackMomentumAtVtxR = -999.;
 ele2_eRegrInput_fbrem = -999.;
 ele2_eRegrInput_charge = -999.;
 ele2_eRegrInput_eSuperClusterOverP = -999.;

 ele2_nGgsfTrackHits     = -999;
 ele2_numberOfLostHits   = -999;
 ele2_nAmbiguousGsfTrack = -999;

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
