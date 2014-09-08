#ifndef  AnalyzerEle_h
#define AnalyzerEle_h

// -*- C++ -*-
//
// Package:    AnalyzerEle
// Class:      AnalyzerEle
// 
/**\class AnalyzerEle AnalyzerEle.cc StreamForElectrons/AnalyzerEle/plugins/AnalyzerEle.cc
// Original Author:  Andrea Massironi
//         Created:  Sat, 25 Jan 2014 09:42:42 GMT
// $Id$
//
//
*/
//
// ---- very very similar to
//      https://github.com/Bicocca/UserCode/blob/master/Calibration/EcalCalibNtuple/plugins/SimpleNtupleEoverP.cc
//

// system include files
#include <memory>
#include <map>
#include "TTree.h"

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Framework/interface/TriggerNamesService.h"
#include "FWCore/Common/interface/TriggerNames.h"


//---- for electrons
#include "Math/Vector4D.h"
#include "Math/Vector3D.h"

#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "DataFormats/TrackReco/interface/TrackExtra.h"
#include "DataFormats/TrajectoryState/interface/PTrajectoryStateOnDet.h"
#include "DataFormats/Math/interface/Vector3D.h"
#include "DataFormats/Math/interface/Point3D.h"
#include "DataFormats/Scalers/interface/DcsStatus.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/EcalDigi/interface/EBSrFlag.h"
#include "DataFormats/EcalDigi/interface/EESrFlag.h"
#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"
#include "DataFormats/EcalDetId/interface/EcalDetIdCollections.h"
#include "DataFormats/EcalDetId/interface/EcalSubdetector.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"
#include "DataFormats/EcalDetId/interface/EcalTrigTowerDetId.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrackFwd.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrackExtra.h"
#include "DataFormats/GsfTrackReco/interface/GsfTangent.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "DataFormats/EgammaCandidates/interface/Conversion.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/Flags.h"
#include "DataFormats/PatCandidates/interface/Lepton.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/CompositeCandidate.h"

#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/TrackReco/interface/TrackExtra.h"
#include "DataFormats/Candidate/interface/Particle.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "DataFormats/Common/interface/TriggerResults.h"


#include "RecoEgamma/EgammaTools/interface/EcalClusterLocal.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "Geometry/CommonDetUnit/interface/GeomDet.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"
#include "DataFormats/TrackReco/interface/TrackExtraBase.h"
#include "TrackingTools/TrajectoryState/interface/TrajectoryStateTransform.h"

#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "Geometry/CaloEventSetup/interface/CaloTopologyRecord.h"
#include "Geometry/CaloGeometry/interface/TruncatedPyramid.h"
#include "CondFormats/DataRecord/interface/EcalChannelStatusRcd.h"
#include "CondFormats/EcalObjects/interface/EcalIntercalibConstants.h"
#include "CondFormats/DataRecord/interface/EcalIntercalibConstantsRcd.h"
#include "CondFormats/EcalObjects/interface/EcalLaserAlphas.h"
#include "CondFormats/DataRecord/interface/EcalLaserAlphasRcd.h"
#include "CondFormats/EcalObjects/interface/EcalADCToGeVConstant.h"
#include "CondFormats/DataRecord/interface/EcalADCToGeVConstantRcd.h"
#include "CalibCalorimetry/EcalLaserCorrection/interface/EcalLaserDbService.h"
#include "CalibCalorimetry/EcalLaserCorrection/interface/EcalLaserDbRecord.h"

#include "RecoVertex/PrimaryVertexProducer/interface/PrimaryVertexSorter.h"
#include "RecoVertex/PrimaryVertexProducer/interface/VertexHigherPtSquared.h"
#include "RecoEcal/EgammaCoreTools/interface/EcalClusterTools.h"
#include "RecoEcal/EgammaCoreTools/interface/EcalClusterFunctionBaseClass.h"
#include "RecoEcal/EgammaCoreTools/interface/EcalClusterFunctionFactory.h"
#include "RecoEcal/EgammaCoreTools/interface/EcalTools.h"
#include "RecoEcal/EgammaCoreTools/interface/PositionCalc.h"
#include "CondFormats/L1TObjects/interface/L1GtTriggerMenu.h"
#include "CondFormats/DataRecord/interface/L1GtTriggerMenuRcd.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutSetupFwd.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutSetup.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "L1Trigger/GlobalTriggerAnalyzer/interface/L1GtUtils.h"
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"
#include "DataFormats/EgammaCandidates/interface/ConversionFwd.h"
#include "DataFormats/EgammaCandidates/interface/Conversion.h"
#include "RecoEgamma/EgammaTools/interface/ConversionTools.h"

// Cluster PU cleaning
#include "RecoEcal/EgammaCoreTools/interface/EcalClusterPUCleaningTools.h"
// Cluster correction functions - regression
#include "RecoEcal/EgammaCoreTools/interface/EcalClusterLazyTools.h"
#include "RecoEgamma/EgammaTools/interface/EGEnergyCorrector.h"

#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"

#include "EgammaAnalysis/ElectronTools/interface/SuperClusterHelper.h"


//----------------------------------
// class declaration
//-----------------------------------

class AnalyzerEle : public edm::EDAnalyzer {

 public:

 
  // --- constructor and deconstructor
  AnalyzerEle(const edm::ParameterSet&);
  ~AnalyzerEle();
  
  // analyze method
  virtual void beginJob();
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob();

  // class method
  void initialize() ;
  void fillPVInfo     (const edm::Event & iEvent, const edm::EventSetup & iSetup);
  void fillRhoInfo    (const edm::Event & iEvent, const edm::EventSetup & iSetup); 
  void fillHLTInfo    (const edm::Event & iEvent, const edm::EventSetup & iSetup);
  void fillPileUpInfo (const edm::Event & iEvent, const edm::EventSetup & iSetup);
  bool myWselection   (const edm::Event & iEvent, const edm::EventSetup & iSetup);
  bool myZselection   (const edm::Event & iEvent, const edm::EventSetup & iSetup);
  void fillEleInfo    (const edm::Event & iEvent, const edm::EventSetup & iSetup, const int iEle, const std::string eleName) ;
  void fillMetInfo    (const edm::Event & iEvent, const edm::EventSetup & iSetup) ;
  void fillGeneratorInfo ( const edm::Event & iEvent, const edm::EventSetup & iSetup);
  void fillDoubleEleInfo(const edm::Event & iEvent, const edm::EventSetup & iSetup) ;
  void fillMatrixRecHit(reco::SuperClusterRef scRef, int type, const EcalRecHitCollection* theRecHitCollection, const CaloTopology* topology);
  double deltaPhi(const double& phi1, const double& phi2);


 private:

  EcalClusterFunctionBaseClass* EcalClusterCrackCorrection;
  EcalClusterFunctionBaseClass* EcalClusterLocalContCorrection;
 
  ///---- input tags ----
  edm::InputTag PVTag_;
  edm::InputTag PVTag_alternative_;
  edm::InputTag rhoTag_;
  edm::InputTag recHitCollection_EB_;
  edm::InputTag recHitCollection_EE_;
  edm::EDGetTokenT<EcalRecHitCollection> recHitCollection_EB_token_;
  edm::EDGetTokenT<EcalRecHitCollection> recHitCollection_EE_token_;
  
  edm::InputTag BSTag_;
  edm::InputTag conversionsInputTag_;
  edm::InputTag EleTag_;
  edm::InputTag PFMetTag_;
  edm::InputTag MCtruthTag_ ;

  edm::InputTag triggerResultsCollection_;
  std::vector<std::string> hltPaths_ ;
         
  // some other useful info
  std::string dataRun_;
  bool verbosity_; 
  bool applyCorrections_;
  bool applyElectronID_;
  bool doWZSelection_;
  bool dataFlag_;
  bool saveMCInfo_ ;

  std::map<float,int> eleIts_;
  math::XYZPoint PVPoint_;
  math::XYZPoint PVPoint_alternative_;

  SuperClusterHelper * mySCHelper_ ;

  ////////////////////////////////////////////////////////////
  // event variables --> branch in the output tree
  ////////////////////////////////////////////////////////////

  TTree* myTree_;
  edm::Service<TFileService> fs_ ;

  long int bxId_;
  long int eventId_;
  int      lumiId_;
  int      runId_;
  int      timeStampHigh_;
  int      isW_;
  int      isZ_;
  int      eventNaiveId_;
  int      nele_;

  // PV variables
  int   PV_n_;
  float PV_z_;
  float PV_d0_;

  int   PV_n_alternative_;
  float PV_z_alternative_;
  float PV_d0_alternative_;

  // PU variables
  float rho_;
  float nPU_ ; 

  // HLT info -> not stored in the output but applied on the fly
  std::vector< std::string > HLTNames_; 
  std::vector<Bool_t>        HLTBits_; 
  std::map<std::string,bool> HLTResults_ ;

  // ele1 variables

  ROOT::Math::XYZTVector ele1;
  float ele1_charge;
  float ele1_p;
  float ele1_pt;
  float ele1_eta;
  float ele1_phi;
  int   ele1_isTrackerDriven;
  int   ele1_classification;

  ///< bit mask for eleID: 1=fiducial, 2=loose, 6=medium, 14=tight, 16=WP90PU, 48=WP80PU. Selection from https://twiki.cern.ch/twiki/bin/viewauth/CMS/EgammaCutBasedIdentification#Electron_ID_Working_Points
  std::vector<int>   ele1_idtype ; 

  float ele1_sigmaIetaIeta;
  float ele1_DphiIn;
  float ele1_DetaIn;
  float ele1_deltaEtaEleClusterTrackAtCalo;
  float ele1_deltaEtaPhiClusterTrackAtCalo;
  float ele1_deltaEtaSeedClusterTrackAtCalo;
  float ele1_deltaPhiSeedClusterTrackAtCalo;
  float ele1_HOverE;
  float ele1_tkIso;
  float ele1_emIso;
  float ele1_hadIso;

  float ele1_scERaw;
  float ele1_scEtRaw;
  float ele1_scEt;
  float ele1_scLocalEta;
  float ele1_scLocalPhi;
  float ele1_scEtaWidth;
  float ele1_scPhiWidth;
  float ele1_scCrackCorr;
  float ele1_scLocalContCorr;
  float ele1_scE;
  float ele1_scEta;
  float ele1_scPhi;
  float ele1_scLaserCorr;

  float ele1_fEta;
  float ele1_fEtaCorr;
  float ele1_tkP;
  float ele1_tkPt;
  float ele1_fbrem;
  
  float ele1_dxy_PV ;
  float ele1_dz_PV ;
  float ele1_dxy_PV_alternative ;
  float ele1_dz_PV_alternative ;
  float ele1_EcalEnergy ;
  float ele1_sigmaP ;

  float ele1_ooemoop ;
  float ele1_effAreaForIso;

  float ele1_eSeedBC;
  float ele1_e5x5;
  float ele1_e3x3;
  float ele1_scNxtal;
  int   ele1_bcN;
  float ele1_5x5LaserCorr;
  float ele1_3x3LaserCorr;
  float ele1_es;

  float ele1_seedE;
  float ele1_seedLaserAlpha;
  float ele1_seedLaserCorr;
  float ele1_seedICConstant;
  int   ele1_seedIeta;
  int   ele1_seedIphi;
  int   ele1_seedIx;
  int   ele1_seedIy;
  int   ele1_seedZside;
  float ele1_EOverP;

  std::vector<float> ele1_recHit_E;
  std::vector<int>   ele1_recHit_flag;
  std::vector<int>   ele1_recHit_hashedIndex;
  std::vector<int>   ele1_recHit_ietaORix;
  std::vector<int>   ele1_recHit_iphiORiy;
  std::vector<int>   ele1_recHit_zside;
  std::vector<float> ele1_recHit_laserCorrection;
  std::vector<float> ele1_recHit_Alpha;
  std::vector<float> ele1_recHit_ICConstant;
  int ele1_nRecHits;

  int ele1_isEB;
  int ele1_isEBEEGap;
  int ele1_isEBEtaGap;
  int ele1_isEBPhiGap;
  int ele1_isEEDeeGap;
  int ele1_isEERingGap;

  // Regression V3 variables
  float ele1_eRegrInput_rawE;
  float ele1_eRegrInput_r9;
  float ele1_eRegrInput_eta;
  float ele1_eRegrInput_phi;
  float ele1_eRegrInput_etaW;
  float ele1_eRegrInput_phiW;
  float ele1_eRegrInput_SCsize;
  float ele1_eRegrInput_rho;
  float ele1_eRegrInput_hoe;
  float ele1_eRegrInput_nPV;
  float ele1_eRegrInput_seed_eta;
  float ele1_eRegrInput_seed_phi;
  float ele1_eRegrInput_seed_E;
  float ele1_eRegrInput_seed_e3x3;
  float ele1_eRegrInput_seed_e5x5;
  float ele1_eRegrInput_sigietaieta;
  float ele1_eRegrInput_sigiphiiphi;
  float ele1_eRegrInput_sigietaiphi;
  float ele1_eRegrInput_eMax;
  float ele1_eRegrInput_e2nd;
  float ele1_eRegrInput_eTop;
  float ele1_eRegrInput_eBottom;
  float ele1_eRegrInput_eLeft;
  float ele1_eRegrInput_eRight;
  float ele1_eRegrInput_e2x5Max;
  float ele1_eRegrInput_e2x5Top;
  float ele1_eRegrInput_e2x5Bottom;
  float ele1_eRegrInput_e2x5Left;
  float ele1_eRegrInput_e2x5Right;
  float ele1_eRegrInput_seed_ieta;
  float ele1_eRegrInput_seed_iphi;
  float ele1_eRegrInput_seed_etaCrySeed;
  float ele1_eRegrInput_seed_phiCrySeed;
  float ele1_eRegrInput_preshowerEnergyOverRaw;
  float ele1_eRegrInput_ecalDrivenSeed;
  float ele1_eRegrInput_isEBEtaGap;
  float ele1_eRegrInput_isEBPhiGap;
  float ele1_eRegrInput_eSubClusters;
  float ele1_eRegrInput_subClusterEnergy_1;
  float ele1_eRegrInput_subClusterEnergy_2;
  float ele1_eRegrInput_subClusterEnergy_3;
  float ele1_eRegrInput_subClusterEta_1;
  float ele1_eRegrInput_subClusterEta_2;
  float ele1_eRegrInput_subClusterEta_3;
  float ele1_eRegrInput_subClusterPhi_1;
  float ele1_eRegrInput_subClusterPhi_2;
  float ele1_eRegrInput_subClusterPhi_3;
  float ele1_eRegrInput_subClusterEmax_1;
  float ele1_eRegrInput_subClusterEmax_2;
  float ele1_eRegrInput_subClusterEmax_3;
  float ele1_eRegrInput_subClusterE3x3_1;
  float ele1_eRegrInput_subClusterE3x3_2;
  float ele1_eRegrInput_subClusterE3x3_3;
  float ele1_eRegrInput_eESClusters;
  float ele1_eRegrInput_eESClusterEnergy_1;
  float ele1_eRegrInput_eESClusterEnergy_2;
  float ele1_eRegrInput_eESClusterEnergy_3;
  float ele1_eRegrInput_eESClusterEta_1;
  float ele1_eRegrInput_eESClusterEta_2;
  float ele1_eRegrInput_eESClusterEta_3;
  float ele1_eRegrInput_eESClusterPhi_1;
  float ele1_eRegrInput_eESClusterPhi_2;
  float ele1_eRegrInput_eESClusterPhi_3;
  float ele1_eRegrInput_pt;
  float ele1_eRegrInput_trackMomentumAtVtxR;
  float ele1_eRegrInput_fbrem;
  float ele1_eRegrInput_charge;
  float ele1_eRegrInput_eSuperClusterOverP;

  int ele1_nGgsfTrackHits;
  int ele1_numberOfLostHits;
  int ele1_nAmbiguousGsfTrack;    

  // ele2 variables
  ROOT::Math::XYZTVector ele2;
  float ele2_charge;
  float ele2_p;
  float ele2_pt;
  float ele2_eta;
  float ele2_phi;
  int   ele2_isTrackerDriven;
  int   ele2_classification;

  ///< bit mask for eleID: 1=fiducial, 2=loose, 6=medium, 14=tight, 16=WP90PU, 48=WP80PU. Selection from https://twiki.cern.ch/twiki/bin/viewauth/CMS/EgammaCutBasedIdentification#Electron_ID_Working_Points
  std::vector<int>   ele2_idtype ;
  float ele2_sigmaIetaIeta;
  float ele2_DphiIn;
  float ele2_DetaIn;
  float ele2_deltaEtaEleClusterTrackAtCalo;
  float ele2_deltaEtaPhiClusterTrackAtCalo;
  float ele2_deltaEtaSeedClusterTrackAtCalo;
  float ele2_deltaPhiSeedClusterTrackAtCalo;
  float ele2_HOverE;
  float ele2_tkIso;
  float ele2_emIso;
  float ele2_hadIso;

  float ele2_scERaw;
  float ele2_scEtRaw;
  float ele2_scEt;
  float ele2_scLocalEta;
  float ele2_scLocalPhi;
  float ele2_scEtaWidth;
  float ele2_scPhiWidth;
  float ele2_scCrackCorr;
  float ele2_scLocalContCorr;
  float ele2_scE;
  float ele2_scEta;
  float ele2_scPhi;
  float ele2_scLaserCorr;
  float ele2_scERaw_PUcleaned;
  float ele2_scEtaWidth_PUcleaned;
  float ele2_scPhiWidth_PUcleaned;
  float ele2_fCorrection_PUcleaned;

  float ele2_fEta;
  float ele2_fEtaCorr;
  float ele2_tkP;
  float ele2_tkPt;
  float ele2_fbrem;

  float ele2_dxy_PV ;
  float ele2_dz_PV ;
  float ele2_dxy_PV_alternative ;
  float ele2_dz_PV_alternative ;
  float ele2_sigmaP ;
  float ele2_EcalEnergy ;
  float ele2_effAreaForIso ;
  float ele2_ooemoop ;

  float ele2_eSeedBC;
  float ele2_e5x5;
  float ele2_e3x3;
  float ele2_scNxtal;
  int   ele2_bcN;
  float ele2_5x5LaserCorr;
  float ele2_3x3LaserCorr;
  float ele2_es;

  float ele2_seedE;
  float ele2_seedLaserAlpha;
  float ele2_seedLaserCorr;
  float ele2_seedICConstant;
  int   ele2_seedIeta;
  int   ele2_seedIphi;
  int   ele2_seedIx;
  int   ele2_seedIy;
  int   ele2_seedZside;
  float ele2_EOverP;

  std::vector<float> ele2_recHit_E;
  std::vector<int>   ele2_recHit_flag;
  std::vector<int>   ele2_recHit_hashedIndex;
  std::vector<int>   ele2_recHit_ietaORix;
  std::vector<int>   ele2_recHit_iphiORiy;
  std::vector<int>   ele2_recHit_zside;
  std::vector<float> ele2_recHit_laserCorrection;
  std::vector<float> ele2_recHit_Alpha;
  std::vector<float> ele2_recHit_ICConstant;
  int ele2_nRecHits;

  int ele2_isEB;
  int ele2_isEBEEGap;
  int ele2_isEBEtaGap;
  int ele2_isEBPhiGap;
  int ele2_isEEDeeGap;
  int ele2_isEERingGap;

  // Regression V3 variables
  float ele2_eRegrInput_rawE;
  float ele2_eRegrInput_r9;
  float ele2_eRegrInput_eta;
  float ele2_eRegrInput_phi;
  float ele2_eRegrInput_etaW;
  float ele2_eRegrInput_phiW;
  float ele2_eRegrInput_SCsize;
  float ele2_eRegrInput_rho;
  float ele2_eRegrInput_hoe;
  float ele2_eRegrInput_nPV;
  float ele2_eRegrInput_seed_eta;
  float ele2_eRegrInput_seed_phi;
  float ele2_eRegrInput_seed_E;
  float ele2_eRegrInput_seed_e3x3;
  float ele2_eRegrInput_seed_e5x5;
  float ele2_eRegrInput_sigietaieta;
  float ele2_eRegrInput_sigiphiiphi;
  float ele2_eRegrInput_sigietaiphi;
  float ele2_eRegrInput_eMax;
  float ele2_eRegrInput_e2nd;
  float ele2_eRegrInput_eTop;
  float ele2_eRegrInput_eBottom;
  float ele2_eRegrInput_eLeft;
  float ele2_eRegrInput_eRight;
  float ele2_eRegrInput_e2x5Max;
  float ele2_eRegrInput_e2x5Top;
  float ele2_eRegrInput_e2x5Bottom;
  float ele2_eRegrInput_e2x5Left;
  float ele2_eRegrInput_e2x5Right;
  float ele2_eRegrInput_seed_ieta;
  float ele2_eRegrInput_seed_iphi;
  float ele2_eRegrInput_seed_etaCrySeed;
  float ele2_eRegrInput_seed_phiCrySeed;
  float ele2_eRegrInput_preshowerEnergyOverRaw;
  float ele2_eRegrInput_ecalDrivenSeed;
  float ele2_eRegrInput_isEBEtaGap;
  float ele2_eRegrInput_isEBPhiGap;
  float ele2_eRegrInput_eSubClusters;
  float ele2_eRegrInput_subClusterEnergy_1;
  float ele2_eRegrInput_subClusterEnergy_2;
  float ele2_eRegrInput_subClusterEnergy_3;
  float ele2_eRegrInput_subClusterEta_1;
  float ele2_eRegrInput_subClusterEta_2;
  float ele2_eRegrInput_subClusterEta_3;
  float ele2_eRegrInput_subClusterPhi_1;
  float ele2_eRegrInput_subClusterPhi_2;
  float ele2_eRegrInput_subClusterPhi_3;
  float ele2_eRegrInput_subClusterEmax_1;
  float ele2_eRegrInput_subClusterEmax_2;
  float ele2_eRegrInput_subClusterEmax_3;
  float ele2_eRegrInput_subClusterE3x3_1;
  float ele2_eRegrInput_subClusterE3x3_2;
  float ele2_eRegrInput_subClusterE3x3_3;
  float ele2_eRegrInput_eESClusters;
  float ele2_eRegrInput_eESClusterEnergy_1;
  float ele2_eRegrInput_eESClusterEnergy_2;
  float ele2_eRegrInput_eESClusterEnergy_3;
  float ele2_eRegrInput_eESClusterEta_1;
  float ele2_eRegrInput_eESClusterEta_2;
  float ele2_eRegrInput_eESClusterEta_3;
  float ele2_eRegrInput_eESClusterPhi_1;
  float ele2_eRegrInput_eESClusterPhi_2;
  float ele2_eRegrInput_eESClusterPhi_3;
  float ele2_eRegrInput_pt;
  float ele2_eRegrInput_trackMomentumAtVtxR;
  float ele2_eRegrInput_fbrem;
  float ele2_eRegrInput_charge;
  float ele2_eRegrInput_eSuperClusterOverP;

  int ele2_nGgsfTrackHits;
  int ele2_numberOfLostHits;
  int ele2_nAmbiguousGsfTrack;    


  // met variables
  ROOT::Math::XYZTVector met;
  ROOT::Math::XYZTVector* p_met;
  float met_et;
  float met_phi;

  float ele1Met_mt;
  float ele1Met_Dphi;

  // di-electron variables
  float ele1ele2_m;
  float ele1ele2_scM;
  float ele1ele2_scMZS;

  // MC Info
  float mcV_E;
  float mcV_Px;
  float mcV_Py;
  float mcV_Pz;
  float mcV_Eta;
  float mcV_Phi;
  float mcV_M;
  int   mcV_Charge;
  int   mcV_PdgId;

  float mcF1_fromV_E;
  float mcF1_fromV_Px;
  float mcF1_fromV_Py;
  float mcF1_fromV_Pz;
  float mcF1_fromV_Eta;
  float mcF1_fromV_Phi;
  float mcF1_fromV_M;
  int   mcF1_fromV_Charge;
  int   mcF1_fromV_PdgId;

  float mcF2_fromV_E;
  float mcF2_fromV_Px;
  float mcF2_fromV_Py;
  float mcF2_fromV_Pz;
  float mcF2_fromV_Eta;
  float mcF2_fromV_Phi;
  float mcF2_fromV_M;
  int   mcF2_fromV_Charge;
  int   mcF2_fromV_PdgId;

  
};

#endif
