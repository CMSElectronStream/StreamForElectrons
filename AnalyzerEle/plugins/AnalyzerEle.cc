// -*- C++ -*-
//
// Package:    AnalyzerEle
// Class:      AnalyzerEle
// 
/**\class AnalyzerEle AnalyzerEle.cc StreamForElectrons/AnalyzerEle/plugins/AnalyzerEle.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Andrea Massironi
//         Created:  Sat, 25 Jan 2014 09:42:42 GMT
// $Id$
//
//

//
// ---- very very similar to
//      https://github.com/Bicocca/UserCode/blob/master/Calibration/EcalCalibNtuple/plugins/SimpleNtupleEoverP.cc
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"


//---- for auto-tree
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "TTree.h"


//---- for electrons
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"


#include "RecoEgamma/EgammaTools/interface/EcalClusterLocal.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "Geometry/CommonDetUnit/interface/GeomDet.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"
#include "DataFormats/TrackReco/interface/TrackExtraBase.h"
#include "DataFormats/TrackReco/interface/TrackExtra.h"
#include "DataFormats/TrajectoryState/interface/PTrajectoryStateOnDet.h"
#include "TrackingTools/TrajectoryState/interface/TrajectoryStateTransform.h"
#include "Math/Vector4D.h"
#include "Math/Vector3D.h"


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

#include "DataFormats/Math/interface/Vector3D.h"
#include "DataFormats/Math/interface/Point3D.h"
#include "DataFormats/Scalers/interface/DcsStatus.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "RecoVertex/PrimaryVertexProducer/interface/PrimaryVertexSorter.h"
#include "RecoVertex/PrimaryVertexProducer/interface/VertexHigherPtSquared.h"
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
#include "RecoEcal/EgammaCoreTools/interface/EcalClusterTools.h"
#include "RecoEcal/EgammaCoreTools/interface/EcalClusterFunctionBaseClass.h"
#include "RecoEcal/EgammaCoreTools/interface/EcalClusterFunctionFactory.h"
#include "RecoEcal/EgammaCoreTools/interface/EcalTools.h"
#include "RecoEcal/EgammaCoreTools/interface/PositionCalc.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
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
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/TrackReco/interface/TrackExtra.h"
#include "DataFormats/Candidate/interface/Particle.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "DataFormats/Common/interface/TriggerResults.h"
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



//
// class declaration
//

class AnalyzerEle : public edm::EDAnalyzer {
   public:
      explicit AnalyzerEle(const edm::ParameterSet&);
      ~AnalyzerEle();

   private:

      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;


      //virtual void beginRun(edm::Run const&, edm::EventSetup const&) override;
      //virtual void endRun(edm::Run const&, edm::EventSetup const&) override;
      //virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;
      //virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;

      bool myWselection (const edm::Event & iEvent, const edm::EventSetup & iSetup);
      bool myZselection (const edm::Event & iEvent, const edm::EventSetup & iSetup);
      void fillEleInfo (const edm::Event & iEvent, const edm::EventSetup & iSetup, const int iEle, const std::string eleName) ;
      void fillPVInfo (const edm::Event & iEvent, const edm::EventSetup & iSetup) ;
      void fillRhoInfo(const edm::Event & iEvent, const edm::EventSetup & iSetup) ;
      void fillMetInfo(const edm::Event & iEvent, const edm::EventSetup & iSetup) ;
      void fillDoubleEleInfo(const edm::Event & iEvent, const edm::EventSetup & iSetup) ;
      void fillMatrixRecHit(reco::SuperClusterRef scRef, int type, const EcalRecHitCollection* theRecHitCollection, const CaloTopology* topology);

      bool TightEle (const edm::Event & iEvent, const edm::EventSetup & iSetup, const int &iEle) ;
      bool MediumEle (const edm::Event & iEvent, const edm::EventSetup & iSetup, const int &iEle) ;
      bool LooseEle (const edm::Event & iEvent, const edm::EventSetup & iSetup,const int &iEle) ;
      double deltaPhi(const double& phi1, const double& phi2);


      EGEnergyCorrector ecorr_;
      EGEnergyCorrector ecorrPho_;
      EcalClusterFunctionBaseClass* EcalClusterCrackCorrection;
      EcalClusterFunctionBaseClass* EcalClusterLocalContCorrection;

      ///---- input tag ----
      edm::InputTag PVTag_;
      edm::InputTag rhoTag_;
      edm::InputTag recHitCollection_EB_;
      edm::InputTag recHitCollection_EE_;
      edm::InputTag BSTag_;
      edm::InputTag SRFlagCollection_EB_;
      edm::InputTag SRFlagCollection_EE_;
      edm::InputTag digiCollection_EB_;
      edm::InputTag digiCollection_EE_;
      edm::InputTag conversionsInputTag_;
      edm::InputTag EleTag_;
      edm::InputTag PFMetTag_;

      int eventType_;
      std::string dataRun_;

      // event variables
      long int bxId_;
      long int eventId_;
      int lumiId_;
      int runId_;
      int timeStampHigh_;
      int isW_;
      int isZ_;

      int eventNaiveId_;
      std::map<float,int> eleIts_;
      math::XYZPoint PVPoint_;

      bool verbosity_; //---- true = loquacious false = silence
      bool applyCorrections_; //---- true = correct the recHit and SC energy IN the analyzer
      bool doWZSelection_;
      bool dataFlag_;
      bool saveRecHitMatrix_;
      bool saveFbrem_;

//       float params_[42];

      // output
      TTree* myTree_;
      int nele_;

      // PV variables
      int PV_n_;
      float PV_z_;
      float PV_d0_;

      // PU variables
      float rho_;

      // ele1 variables
      ROOT::Math::XYZTVector ele1;
      float ele1_charge;
      float ele1_p;
      float ele1_pt;
      float ele1_eta;
      float ele1_phi;
      int ele1_isTrackerDriven;

      float ele1_sigmaIetaIeta;
      float ele1_DphiIn;
      float ele1_DetaIn;
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
      float ele1_scE_regression;
      float ele1_scEerr_regression;
      float ele1_scE_regression_PhotonTuned ;
      float ele1_scEerr_regression_PhotonTuned ;

      float ele1_scERaw_PUcleaned;
      float ele1_scEtaWidth_PUcleaned;
      float ele1_scPhiWidth_PUcleaned;
      float ele1_fCorrection_PUcleaned;

      float ele1_fEta;
      float ele1_fEtaCorr;
      float ele1_tkP;
      float ele1_tkPt;
      float ele1_fbrem;
  
      float ele1_dxy_PV ;
      float ele1_dz_PV ;
      float ele1_EcalEnergy ;
      float ele1_sigmaP ;

      float ele1_ooemoop ;
      float ele1_effAreaForIso;

      float ele1_eSeedBC;
      float ele1_e5x5;
      float ele1_e3x3;
      float ele1_scNxtal;
      int ele1_bcN;
      float ele1_5x5LaserCorr;
      float ele1_3x3LaserCorr;
      float ele1_es;

      float ele1_seedE;
      float ele1_seedLaserAlpha;
      float ele1_seedLaserCorr;
      float ele1_seedICConstant;
      int ele1_seedIeta;
      int ele1_seedIphi;
      int ele1_seedIx;
      int ele1_seedIy;
      int ele1_seedZside;
      float ele1_EOverP;

      std::vector<float> ele1_recHit_E;
      std::vector<int> ele1_recHit_flag;
      std::vector<int> ele1_recHit_hashedIndex;
      std::vector<int> ele1_recHit_ietaORix;
      std::vector<int> ele1_recHit_iphiORiy;
      std::vector<int> ele1_recHit_zside;
      std::vector<float> ele1_recHit_laserCorrection;
      std::vector<float> ele1_recHit_Alpha;
      std::vector<float> ele1_recHit_ICConstant;
      int ele1_nRecHits;

      std::vector<float> ele1_recHitMatrix_E;
      std::vector<int> ele1_recHitMatrix_flag;
      std::vector<int> ele1_recHitMatrix_hashedIndex;
      std::vector<int> ele1_recHitMatrix_ietaORix;
      std::vector<int> ele1_recHitMatrix_iphiORiy;
      std::vector<int> ele1_recHitMatrix_zside;
      std::vector<float> ele1_recHitMatrix_laserCorrection;
      std::vector<float> ele1_recHitMatrix_ICConstant;
      std::vector<float> ele1_recHitMatrix_samples;

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
      float ele1_eRegrInput_r25;
      float ele1_eRegrInput_hoe;
      float ele1_eRegrInput_etaW;
      float ele1_eRegrInput_phiW;
      float ele1_eRegrInput_rho;
      float ele1_eRegrInput_Deta_bC_sC;
      float ele1_eRegrInput_Dphi_bC_sC;
      float ele1_eRegrInput_bCE_Over_sCE;
      float ele1_eRegrInput_e3x3_Over_bCE;
      float ele1_eRegrInput_e5x5_Over_bCE;
      float ele1_eRegrInput_sigietaieta_bC1;
      float ele1_eRegrInput_sigiphiiphi_bC1;
      float ele1_eRegrInput_sigietaiphi_bC1;
      float ele1_eRegrInput_bEMax_Over_bCE;
      float ele1_eRegrInput_bE2nd_Over_bCE;
      float ele1_eRegrInput_bEtop_Over_bCE;
      float ele1_eRegrInput_bEbot_Over_bCE;

      float ele1_eRegrInput_bEleft_Over_bCE;
      float ele1_eRegrInput_bEright_Over_bCE;
      float ele1_eRegrInput_be2x5max_Over_bCE;
      float ele1_eRegrInput_be2x5top_Over_bCE;
      float ele1_eRegrInput_be2x5bottom_Over_bCE;
      float ele1_eRegrInput_be2x5left_Over_bCE;
      float ele1_eRegrInput_be2x5right_Over_bCE;

      float ele1_eRegrInput_seedbC_eta;
      float ele1_eRegrInput_seedbC_phi;
      float ele1_eRegrInput_seedbC_eta_p5;
      float ele1_eRegrInput_seedbC_phi_p2;
      float ele1_eRegrInput_seedbC_bieta;
      float ele1_eRegrInput_seedbC_phi_p20;
      float ele1_eRegrInput_seedbC_etacry;
      float ele1_eRegrInput_seedbC_phicry;


      float ele1_eRegrInput_ESoSC;
      float ele1_eRegrInput_nPV;
      float ele1_eRegrInput_SCsize;

      float ele1_inner_p;
      float ele1_inner_x;
      float ele1_inner_y;
      float ele1_inner_z;
      float ele1_outer_p;
      float ele1_outer_x;
      float ele1_outer_y;
      float ele1_outer_z;
      int ele1_tangent_n;
      std::vector<float> ele1_tangent_p;
      std::vector<float> ele1_tangent_x;
      std::vector<float> ele1_tangent_y;
      std::vector<float> ele1_tangent_z;
      std::vector<float> ele1_tangent_dP;
      std::vector<float> ele1_tangent_dPerr;


      // ele2 variables
      ROOT::Math::XYZTVector ele2;
      float ele2_charge;
      float ele2_p;
      float ele2_pt;
      float ele2_eta;
      float ele2_phi;
      int ele2_isTrackerDriven;


      float ele2_sigmaIetaIeta;
      float ele2_DphiIn;
      float ele2_DetaIn;
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
      float ele2_scE_regression;
      float ele2_scEerr_regression;
      float ele2_scE_regression_PhotonTuned ;
      float ele2_scEerr_regression_PhotonTuned ;
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
      float ele2_sigmaP ;
      float ele2_EcalEnergy ;
      float ele2_effAreaForIso ;
      float ele2_ooemoop ;

      float ele2_eSeedBC;
      float ele2_e5x5;
      float ele2_e3x3;
      float ele2_scNxtal;
      int ele2_bcN;
      float ele2_5x5LaserCorr;
      float ele2_3x3LaserCorr;
      float ele2_es;

      float ele2_seedE;
      float ele2_seedLaserAlpha;
      float ele2_seedLaserCorr;
      float ele2_seedICConstant;
      int ele2_seedIeta;
      int ele2_seedIphi;
      int ele2_seedIx;
      int ele2_seedIy;
      int ele2_seedZside;
      float ele2_EOverP;

      std::vector<float> ele2_recHit_E;
      std::vector<int> ele2_recHit_flag;
      std::vector<int> ele2_recHit_hashedIndex;
      std::vector<int> ele2_recHit_ietaORix;
      std::vector<int> ele2_recHit_iphiORiy;
      std::vector<int> ele2_recHit_zside;
      std::vector<float> ele2_recHit_laserCorrection;
      std::vector<float> ele2_recHit_Alpha;
      std::vector<float> ele2_recHit_ICConstant;
      int ele2_nRecHits;

      std::vector<float> ele2_recHitMatrix_E;
      std::vector<int> ele2_recHitMatrix_flag;
      std::vector<int> ele2_recHitMatrix_hashedIndex;
      std::vector<int> ele2_recHitMatrix_ietaORix;
      std::vector<int> ele2_recHitMatrix_iphiORiy;
      std::vector<int> ele2_recHitMatrix_zside;
      std::vector<float> ele2_recHitMatrix_laserCorrection;
      std::vector<float> ele2_recHitMatrix_ICConstant;
      std::vector<float> ele2_recHitMatrix_samples;


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
      float ele2_eRegrInput_r25;
      float ele2_eRegrInput_hoe;
      float ele2_eRegrInput_etaW;
      float ele2_eRegrInput_phiW;
      float ele2_eRegrInput_rho;
      float ele2_eRegrInput_Deta_bC_sC;
      float ele2_eRegrInput_Dphi_bC_sC;
      float ele2_eRegrInput_bCE_Over_sCE;
      float ele2_eRegrInput_e3x3_Over_bCE;
      float ele2_eRegrInput_e5x5_Over_bCE;
      float ele2_eRegrInput_sigietaieta_bC1;
      float ele2_eRegrInput_sigiphiiphi_bC1;
      float ele2_eRegrInput_sigietaiphi_bC1;
      float ele2_eRegrInput_bEMax_Over_bCE;
      float ele2_eRegrInput_bE2nd_Over_bCE;
      float ele2_eRegrInput_bEtop_Over_bCE;
      float ele2_eRegrInput_bEbot_Over_bCE;

      float ele2_eRegrInput_bEleft_Over_bCE;
      float ele2_eRegrInput_bEright_Over_bCE;
      float ele2_eRegrInput_be2x5max_Over_bCE;
      float ele2_eRegrInput_be2x5top_Over_bCE;
      float ele2_eRegrInput_be2x5bottom_Over_bCE;
      float ele2_eRegrInput_be2x5left_Over_bCE;
      float ele2_eRegrInput_be2x5right_Over_bCE;

      float ele2_eRegrInput_seedbC_eta;
      float ele2_eRegrInput_seedbC_phi;
      float ele2_eRegrInput_seedbC_eta_p5;
      float ele2_eRegrInput_seedbC_phi_p2;
      float ele2_eRegrInput_seedbC_bieta;
      float ele2_eRegrInput_seedbC_phi_p20;
      float ele2_eRegrInput_seedbC_etacry;
      float ele2_eRegrInput_seedbC_phicry;

 
      float ele2_eRegrInput_ESoSC;
      float ele2_eRegrInput_nPV;
      float ele2_eRegrInput_SCsize;
 
      float ele2_inner_p;
      float ele2_inner_x;
      float ele2_inner_y;
      float ele2_inner_z;
      float ele2_outer_p;
      float ele2_outer_x;
      float ele2_outer_y;
      float ele2_outer_z;
      int ele2_tangent_n;
      std::vector<float> ele2_tangent_p;
      std::vector<float> ele2_tangent_x;
      std::vector<float> ele2_tangent_y;
      std::vector<float> ele2_tangent_z;
      std::vector<float> ele2_tangent_dP;
      std::vector<float> ele2_tangent_dPerr;

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
      float ele1ele2_scM_regression;
      float ele1ele2_scMZS;

};

//
// constants, enums and typedefs
//

//
// static data member definitions
//


// -*- C++ -*-
//
// Package: AnalyzerEle
// Class: AnalyzerEle
//

AnalyzerEle::AnalyzerEle(const edm::ParameterSet& iConfig) {
  std::cout << ">>> AnalyzerEle::AnalyzerEle begin <<<" << std::endl;

   // Initialize parameters for cluster corrections
//   std::cout<< ">>>>>> AnalyzerEle::AnalyzerEle::read parameters <<<" << std::endl;
//   Initializeparams(params_);

  edm::Service<TFileService> fs ;
  myTree_ = fs -> make <TTree>("AnalyzerEle","AnalyzerEle");

  eventType_ = iConfig.getUntrackedParameter<int>("eventType",1);
  dataRun_ = iConfig.getParameter<std::string>("dataRun");
  digiCollection_EB_ = iConfig.getParameter<edm::InputTag>("digiCollection_EB");
  digiCollection_EE_ = iConfig.getParameter<edm::InputTag>("digiCollection_EE");
  recHitCollection_EB_ = iConfig.getParameter<edm::InputTag>("recHitCollection_EB");
  recHitCollection_EE_ = iConfig.getParameter<edm::InputTag>("recHitCollection_EE");
  BSTag_ = iConfig.getParameter<edm::InputTag>("theBeamSpotTag");
  SRFlagCollection_EB_ = iConfig.getParameter<edm::InputTag>("SRFlagCollection_EB");
  SRFlagCollection_EE_ = iConfig.getParameter<edm::InputTag>("SRFlagCollection_EE");
  PVTag_ = iConfig.getParameter<edm::InputTag>("PVTag");
  rhoTag_ = iConfig.getParameter<edm::InputTag>("rhoTag");
  EleTag_ = iConfig.getParameter<edm::InputTag>("EleTag");
  PFMetTag_ = iConfig.getParameter<edm::InputTag>("PFMetTag");
  conversionsInputTag_ = iConfig.getParameter<edm::InputTag>("conversionsInputTag");

  //---- flags ----
  verbosity_ = iConfig.getUntrackedParameter<bool>("verbosity", false);
  doWZSelection_ = iConfig.getUntrackedParameter<bool>("doWZSelection", false);
  applyCorrections_ = iConfig.getUntrackedParameter<bool>("applyCorrections",false);
  dataFlag_ = iConfig.getUntrackedParameter<bool>("dataFlag", true);
  saveRecHitMatrix_ = iConfig.getUntrackedParameter<bool>("saveRecHitMatrix",false);
  saveFbrem_ = iConfig.getUntrackedParameter<bool>("saveFbrem", false);
  eventNaiveId_ = 0;


  //---- Initialize tree branches ----

  // event variables
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
  
  myTree_ -> Branch("ele1_isTrackerDriven",&ele1_isTrackerDriven,"ele1_isTrackerDriven/I");
  myTree_ -> Branch("ele1_sigmaIetaIeta", &ele1_sigmaIetaIeta, "ele1_sigmaIetaIeta/F");
  myTree_ -> Branch("ele1_DphiIn", &ele1_DphiIn, "ele1_DphiIn/F");
  myTree_ -> Branch("ele1_DetaIn", &ele1_DetaIn, "ele1_DetaIn/F");
  myTree_ -> Branch("ele1_HOverE", &ele1_HOverE, "ele1_HOverE/F");
  myTree_ -> Branch("ele1_tkIso", &ele1_tkIso, "ele1_tkIso/F");
  myTree_ -> Branch("ele1_emIso", &ele1_emIso, "ele1_emIso/F");
  myTree_ -> Branch("ele1_hadIso", &ele1_hadIso, "ele1_hadIso/F");
  
  myTree_ -> Branch("ele1_scERaw", &ele1_scERaw, "ele1_scERaw/F");
  myTree_ -> Branch("ele1_scEtRaw", &ele1_scEtRaw, "ele1_scEtRaw/F");
  myTree_ -> Branch("ele1_scE", &ele1_scE, "ele1_scE/F");
  myTree_ -> Branch("ele1_scEt", &ele1_scEt, "ele1_scEt/F");
  myTree_ -> Branch("ele1_scE_regression", &ele1_scE_regression, "ele1_scE_regression/F");
  myTree_ -> Branch("ele1_scEerr_regression", &ele1_scEerr_regression, "ele1_scEerr_regression/F");
  myTree_ -> Branch("ele1_scE_regression_PhotonTuned", &ele1_scE_regression_PhotonTuned , "ele1_scE_regression_PhotonTuned/F");
  myTree_ -> Branch("ele1_scEerr_regression_PhotonTuned", &ele1_scEerr_regression_PhotonTuned , "ele1_scEerr_regression_PhotonTuned/F");
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
  
  myTree_ -> Branch("ele2_isEB", &ele2_isEB, "ele2_isEB/I");
  myTree_ -> Branch("ele2_isEBEEGap", &ele2_isEBEEGap, "ele2_isEBEEGap/I");
  myTree_ -> Branch("ele2_isEBEtaGap", &ele2_isEBEtaGap, "ele2_isEBEtaGap/I");
  myTree_ -> Branch("ele2_isEBPhiGap", &ele2_isEBPhiGap, "ele2_isEBPhiGap/I");
  myTree_ -> Branch("ele2_isEEDeeGap", &ele2_isEEDeeGap, "ele2_isEEDeeGap/I");
  myTree_ -> Branch("ele2_isEERingGap", &ele2_isEERingGap, "ele2_isEERingGap/I");
  
  myTree_ -> Branch("ele2_isTrackerDriven",&ele2_isTrackerDriven,"ele2_isTrackerDriven/I");
  myTree_ -> Branch("ele2_sigmaIetaIeta", &ele2_sigmaIetaIeta, "ele2_sigmaIetaIeta/F");
  myTree_ -> Branch("ele2_DphiIn", &ele2_DphiIn, "ele2_DphiIn/F");
  myTree_ -> Branch("ele2_DetaIn", &ele2_DetaIn, "ele2_DetaIn/F");
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
  myTree_ -> Branch("ele2_scE_regression", &ele2_scE_regression, "ele2_scE_regression/F");
  myTree_ -> Branch("ele2_scEerr_regression", &ele2_scEerr_regression, "ele2_scEerr_regression/F");
  myTree_ -> Branch("ele2_scE_regression_PhotonTuned", &ele2_scE_regression_PhotonTuned , "ele2_scE_regression_PhotonTuned/F");
  myTree_ -> Branch("ele2_scEerr_regression_PhotonTuned",&ele2_scEerr_regression_PhotonTuned ,"ele2_scEerr_regression_PhotonTuned/F");
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
  myTree_ -> Branch("ele1ele2_scM_regression",&ele1ele2_scM_regression,"ele1ele2_scM_regression/F");


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



AnalyzerEle::~AnalyzerEle () {
  std::cout << ">>> AnalyzerEle::~AnalyzerEle <<< analyzed " << eventNaiveId_ << " events" << std::endl;
}

// -----------------------------------------------------------------------------------------



void AnalyzerEle::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
  if( verbosity_ )
    std::cout << ">>> AnalyzerEle::analyze begin <<<" << std::endl;


  ++eventNaiveId_;

  bool isGoodEvent = false;

  // event variables
  bxId_ = iEvent.bunchCrossing();
  runId_ = iEvent.id().run();
  lumiId_ = iEvent.luminosityBlock();
  eventId_ = iEvent.id().event();
  timeStampHigh_ = (int)(iEvent.time().value() >> 32);

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

  ele1_sigmaIetaIeta =-99.;
  ele1_DphiIn =-99.;
  ele1_DetaIn =-99.;
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
  ele1_scE_regression =-99.;
  ele1_scEerr_regression=-99.;
  ele1_scE_regression_PhotonTuned=-99.;
  ele1_scEerr_regression_PhotonTuned=-99.;
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

  ele2_sigmaIetaIeta =-99.;
  ele2_DphiIn =-99.;
  ele2_DetaIn =-99.;
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
  ele2_scE_regression =-99.;
  ele2_scEerr_regression=-99.;
  ele2_scE_regression_PhotonTuned=-99.;
  ele2_scEerr_regression_PhotonTuned=-99.;
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

  ele1Met_mt=-99.;
  ele1Met_Dphi=-99.;
  
  
  // di-electron variables
  ele1ele2_m=-99.;
  ele1ele2_scM=-99.;
  ele1ele2_scM_regression=-99.;


  //************* VERTEXES
  fillPVInfo (iEvent,iSetup) ;

  //************* RHO for ISO
  fillRhoInfo(iEvent,iSetup) ;

  //************* ELECTRONS
  edm::Handle<edm::View<reco::GsfElectron> > electronHandle;
  iEvent.getByLabel(EleTag_,electronHandle);
  edm::View<reco::GsfElectron> electrons = *electronHandle;
 
  ///---- get the number of the electron in the event to know if it's a W or a Z ----
  if(doWZSelection_){

   int nEleTight=0,nEleMedium=0,nEleLoose=0;
   eleIts_.clear();

   for ( unsigned int iEle=0; iEle<electrons.size(); ++iEle ){
    bool isEleTight = TightEle(iEvent,iSetup,iEle);

    if(isEleTight){ nEleTight++;
                    eleIts_[1./electrons.at(iEle).pt()] = iEle;
                    continue;}

    bool isEleMedium = MediumEle(iEvent,iSetup,iEle);

    if(isEleMedium){nEleMedium++;
                    eleIts_[1./electrons.at(iEle).pt()] = iEle;
                    continue;}

    bool isEleLoose = LooseEle(iEvent,iSetup,iEle);

    if(isEleLoose){nEleLoose++;
                   eleIts_[1./electrons.at(iEle).pt()] = iEle;
                   continue;}
   }
   if( nEleTight < 1 ) return ;
   if( nEleTight > 2 ) return ;
   if( nEleMedium > 1) return ;
   if( nEleLoose > 0 ) return ;
  
   ///---- check if the event is good----
   
   if( (nEleTight == 1) && (nEleMedium == 0) ){
    isW_=1; isZ_=0;
    std::map<float,int>::const_iterator mapIt = eleIts_.begin();
    fillEleInfo ( iEvent, iSetup, mapIt->second, "ele1" );
    fillMetInfo (iEvent, iSetup);
 
   }

   if( (nEleTight == 2) || (nEleTight == 1 && nEleMedium == 1) ){
    isW_=0; isZ_=1;
    std::map<float,int>::const_iterator mapIt = eleIts_.begin();
    fillEleInfo ( iEvent, iSetup, mapIt->second, "ele1" );
    mapIt++;
    fillEleInfo ( iEvent, iSetup, mapIt->second, "ele2" );
    fillDoubleEleInfo (iEvent, iSetup);
    fillMetInfo (iEvent, iSetup);
 
   }

    
   if( (nEleTight == 1) && (nEleMedium == 0) ) isGoodEvent = myWselection ( iEvent, iSetup);
    if( (nEleTight == 2) || (nEleTight == 1 && nEleMedium == 1) ) isGoodEvent = myZselection ( iEvent, iSetup);
    ///---- save the entry of the tree only if W/Z event ----
    if ( isGoodEvent ) myTree_ -> Fill();
    
  }
  else{
         int nEle = electrons.size();
         if ( nEle == 1 ) { isW_ = 1; isZ_ = 0; }
         if ( nEle == 2 ) { isW_ = 0; isZ_ = 1; }
  
         if ( isW_ == 1 ) fillEleInfo ( iEvent, iSetup, 0, "ele1" );
                      
    
         if ( isZ_ == 1 ) {
           fillEleInfo ( iEvent, iSetup, 0, "ele1" );
           fillEleInfo ( iEvent, iSetup, 1, "ele2" );
           fillDoubleEleInfo (iEvent, iSetup);
          }

         fillMetInfo (iEvent, iSetup);
 
         if (isW_ == 1 || isZ_ == 1) myTree_ -> Fill();
         }
  
  
  if( verbosity_ )
    std::cout << ">>> AnalyzerEle::analyze end <<<" << std::endl;
}



//-----------------------------------------------------------------------------------------
bool AnalyzerEle::TightEle (const edm::Event & iEvent, const edm::EventSetup & iESetup, const int &iEle){

 //************* ELECTRONS
 edm::Handle<edm::View<reco::GsfElectron> > electronHandle;
 iEvent.getByLabel(EleTag_,electronHandle);
 edm::View<reco::GsfElectron> electrons = *electronHandle;

  //********* CONVERSION TOOLS
 edm::Handle<reco::ConversionCollection> conversions_h;
 iEvent.getByLabel(conversionsInputTag_, conversions_h);

 bool isTightEle = false;

 reco::GsfElectron electron = electrons.at(iEle);
 
 edm::InputTag EleBad = edm::InputTag("gsfElectrons");
 
 if(EleTag_== EleBad && !electron.ecalDriven()) return false;

 float pt = electron.pt();
 float eta = electron.eta();
      
 float tkIso = electron.dr03TkSumPt();
 float emIso = electron.dr03EcalRecHitSumEt();
 float hadIso = electron.dr03HcalDepth1TowerSumEt()+electron.dr03HcalDepth2TowerSumEt();
 float AEff = 0.; 
//    reco::ElectronEffectiveArea::GetElectronEffectiveArea(reco::ElectronEffectiveArea::kEleGammaAndNeutralHadronIso03, electron.superCluster()->eta(), reco::ElectronEffectiveArea::kEleEAData2011);
 
 // default
 float combIso = tkIso +std::max(emIso + hadIso - rho_*AEff,float(0.));
 
 // 2011-like test
 //float combIso = tkIso + emIso + hadIso - rho*3.14159*0.3*0.3;
 
 int isEB = electron.isEB();
 float sigmaIetaIeta = electron.sigmaIetaIeta();
 float DetaIn = electron.deltaEtaSuperClusterTrackAtVtx();
 float DphiIn = electron.deltaPhiSuperClusterTrackAtVtx();
 float HOverE = electron.hadronicOverEm();
 float ooemoop = (1.0/electron.ecalEnergy() - electron.eSuperClusterOverP()/electron.ecalEnergy());

 int mishits = electron.gsfTrack()->trackerExpectedHitsInner().numberOfHits();
 int nAmbiguousGsfTracks = electron.ambiguousGsfTracksSize();


 reco::GsfTrackRef eleTrack = electron.gsfTrack() ;
 float dxy = eleTrack->dxy(PVPoint_);
 float dz = eleTrack->dz (PVPoint_);

 edm::Handle<reco::BeamSpot> BSHandle;
 iEvent.getByLabel(BSTag_, BSHandle);
 const reco::BeamSpot BS = *BSHandle;

 
 bool isConverted = ConversionTools::hasMatchedConversion(electron, conversions_h, BS.position());
 
 // default
 if( (pt > 20.) && (fabs(eta) < 2.5) &&
      ( ( (isEB == 1) && (fabs(DetaIn) < 0.004) ) || ( (isEB == 0) && (fabs(DetaIn) < 0.007) ) ) &&
      ( ( (isEB == 1) && (fabs(DphiIn) < 0.060) ) || ( (isEB == 0) && (fabs(DphiIn) < 0.030) ) ) &&
      ( ( (isEB == 1) && (sigmaIetaIeta < 0.010) ) || ( (isEB == 0) && (sigmaIetaIeta < 0.030) ) ) &&
      ( ( (isEB == 1) && (HOverE < 0.120) ) || ( (isEB == 0) && (HOverE < 0.100) ) ) &&
      ( ( (isEB == 1) && (fabs(ooemoop) < 0.050) ) || ( (isEB == 0) && (fabs(ooemoop) < 0.050) ) ) &&
      ( ( (isEB == 1) && (fabs(dxy) < 0.020) ) || ( (isEB == 0) && (fabs(dxy) < 0.020) ) ) &&
      ( ( (isEB == 1) && (fabs(dz) < 0.100) ) || ( (isEB == 0) && (fabs(dz) < 0.100) ) ) &&
      ( ( (isEB == 1) && (!isConverted) ) || ( (isEB == 0) && (!isConverted) ) ) &&
      ( ( (isEB == 1) && (combIso/pt < 0.070) ) || ( (isEB == 0) && (combIso/pt < 0.060) ) ) &&
      ( mishits == 0 ) &&
      ( nAmbiguousGsfTracks == 0 ) )
   isTightEle=true;
 
 // 2011-like test
 //if( (pt > 20.) && (fabs(eta) < 2.5) &&
 // ( ( (isEB == 1) && (fabs(DetaIn) < 0.004) ) || ( (isEB == 0) && (fabs(DetaIn) < 0.007) ) ) &&
 // ( ( (isEB == 1) && (fabs(DphiIn) < 0.060) ) || ( (isEB == 0) && (fabs(DphiIn) < 0.030) ) ) &&
 // ( ( (isEB == 1) && (sigmaIetaIeta < 0.010) ) || ( (isEB == 0) && (sigmaIetaIeta < 0.030) ) ) &&
 // //( ( (isEB == 1) && (HOverE < 0.040) ) || ( (isEB == 0) && (HOverE < 0.025) ) ) &&
 // ( ( (isEB == 1) && (combIso/pt < 0.070) ) || ( (isEB == 0) && (combIso/pt < 0.060) ) ) &&
 // ( mishits == 0 ) &&
 // ( nAmbiguousGsfTracks == 0 ) )
 // isTightEle=true;

return isTightEle;

}

bool AnalyzerEle::MediumEle (const edm::Event & iEvent, const edm::EventSetup & iESetup, const int &iEle){

 //************* ELECTRONS
 edm::Handle<edm::View<reco::GsfElectron> > electronHandle;
 iEvent.getByLabel(EleTag_,electronHandle);
 edm::View<reco::GsfElectron> electrons = *electronHandle;

  //********* CONVERSION TOOLS
 edm::Handle<reco::ConversionCollection> conversions_h;
 iEvent.getByLabel(conversionsInputTag_, conversions_h);

 bool isMediumEle = false;

 reco::GsfElectron electron = electrons.at(iEle);

 edm::InputTag EleBad = edm::InputTag("gsfElectrons");
 
 if(EleTag_== EleBad && !electron.ecalDriven()) return false;

 float pt = electron.pt();
 float eta = electron.eta();
      
 float tkIso = electron.dr03TkSumPt();
 float emIso = electron.dr03EcalRecHitSumEt();
 float hadIso = electron.dr03HcalDepth1TowerSumEt()+electron.dr03HcalDepth2TowerSumEt();
 
 float AEff = 0.;
//  reco::ElectronEffectiveArea::GetElectronEffectiveArea(reco::ElectronEffectiveArea::kEleGammaAndNeutralHadronIso03, electron.superCluster()->eta(), reco::ElectronEffectiveArea::kEleEAData2011);
 
 // default
 float combIso = tkIso +std::max(emIso + hadIso - rho_*AEff,float(0.));
 
 // 2011-like test
 //float combIso = tkIso + emIso + hadIso - rho*3.14159*0.3*0.3;
 
 int isEB = electron.isEB();
 float sigmaIetaIeta = electron.sigmaIetaIeta();
 float DetaIn = electron.deltaEtaSuperClusterTrackAtVtx();
 float DphiIn = electron.deltaPhiSuperClusterTrackAtVtx();
 float HOverE = electron.hadronicOverEm();
 float ooemoop = (1.0/electron.ecalEnergy() - electron.eSuperClusterOverP()/electron.ecalEnergy());

 int mishits = electron.gsfTrack()->trackerExpectedHitsInner().numberOfHits();
 int nAmbiguousGsfTracks = electron.ambiguousGsfTracksSize();


 reco::GsfTrackRef eleTrack = electron.gsfTrack() ;
 float dxy = eleTrack->dxy(PVPoint_);
 float dz = eleTrack->dz (PVPoint_);

 edm::Handle<reco::BeamSpot> BSHandle;
 iEvent.getByLabel(BSTag_, BSHandle);
 const reco::BeamSpot BS = *BSHandle;

 
 bool isConverted = ConversionTools::hasMatchedConversion(electron, conversions_h, BS.position());
 
 // default
 if( (pt > 12.) && (fabs(eta) < 2.5) &&
      ( ( (isEB == 1) && (fabs(DetaIn) < 0.004) ) || ( (isEB == 0) && (fabs(DetaIn) < 0.007) ) ) &&
      ( ( (isEB == 1) && (fabs(DphiIn) < 0.060) ) || ( (isEB == 0) && (fabs(DphiIn) < 0.030) ) ) &&
      ( ( (isEB == 1) && (sigmaIetaIeta < 0.010) ) || ( (isEB == 0) && (sigmaIetaIeta < 0.030) ) ) &&
      ( ( (isEB == 1) && (HOverE < 0.120) ) || ( (isEB == 0) && (HOverE < 0.100) ) ) &&
      ( ( (isEB == 1) && (fabs(ooemoop) < 0.050) ) || ( (isEB == 0) && (fabs(ooemoop) < 0.050) ) ) &&
      ( ( (isEB == 1) && (fabs(dxy) < 0.020) ) || ( (isEB == 0) && (fabs(dxy) < 0.020) ) ) &&
      ( ( (isEB == 1) && (fabs(dz) < 0.100) ) || ( (isEB == 0) && (fabs(dz) < 0.100) ) ) &&
      ( ( (isEB == 1) && (!isConverted) ) || ( (isEB == 0) && (!isConverted) ) ) &&
      ( ( (isEB == 1) && (combIso/pt < 0.070) ) || ( (isEB == 0) && (combIso/pt < 0.060) ) ) &&
      ( mishits == 0 ) &&
      ( nAmbiguousGsfTracks == 0 )
        )
    isMediumEle=true;
 
 // 2011-like test
 //if( (pt > 12.) && (fabs(eta) < 2.5) &&
 // ( ( (isEB == 1) && (fabs(DetaIn) < 0.004) ) || ( (isEB == 0) && (fabs(DetaIn) < 0.007) ) ) &&
 // ( ( (isEB == 1) && (fabs(DphiIn) < 0.060) ) || ( (isEB == 0) && (fabs(DphiIn) < 0.030) ) ) &&
 // ( ( (isEB == 1) && (sigmaIetaIeta < 0.010) ) || ( (isEB == 0) && (sigmaIetaIeta < 0.030) ) ) &&
 // //( ( (isEB == 1) && (HOverE < 0.040) ) || ( (isEB == 0) && (HOverE < 0.025) ) ) &&
 // ( ( (isEB == 1) && (combIso/pt < 0.070) ) || ( (isEB == 0) && (combIso/pt < 0.060) ) ) &&
 // ( mishits == 0 ) &&
 // ( nAmbiguousGsfTracks == 0 )
 // )
 // isMediumEle=true;


return isMediumEle;
}
// ----------------------------------------------------------------------------------------
bool AnalyzerEle::LooseEle (const edm::Event & iEvent, const edm::EventSetup & iESetup,const int &iEle){

//************* ELECTRONS
 edm::Handle<edm::View<reco::GsfElectron> > electronHandle;
 iEvent.getByLabel(EleTag_,electronHandle);
 edm::View<reco::GsfElectron> electrons = *electronHandle;

  //********* CONVERSION TOOLS
 edm::Handle<reco::ConversionCollection> conversions_h;
 iEvent.getByLabel(conversionsInputTag_, conversions_h);

 bool isLooseEle = false;

 reco::GsfElectron electron = electrons.at(iEle);

 edm::InputTag EleBad = edm::InputTag("gsfElectrons");
 
 if(EleTag_== EleBad && !electron.ecalDriven()) return false;

 float pt = electron.pt();
 float eta = electron.eta();
      
 float tkIso = electron.dr03TkSumPt();
 float emIso = electron.dr03EcalRecHitSumEt();
 float hadIso = electron.dr03HcalDepth1TowerSumEt()+electron.dr03HcalDepth2TowerSumEt();
 
 float AEff = 0.;
//  reco::ElectronEffectiveArea::GetElectronEffectiveArea(reco::ElectronEffectiveArea::kEleGammaAndNeutralHadronIso03, electron.superCluster()->eta(), reco::ElectronEffectiveArea::kEleEAData2011);

 // default
 float combIso = tkIso +std::max(emIso + hadIso - rho_*AEff,float(0.));
 
 // 2011-like test
 //float combIso = tkIso + emIso + hadIso - rho*3.14159*0.3*0.3;
 
 int isEB = electron.isEB();
 float sigmaIetaIeta = electron.sigmaIetaIeta();
 float DetaIn = electron.deltaEtaSuperClusterTrackAtVtx();
 float DphiIn = electron.deltaPhiSuperClusterTrackAtVtx();
 float HOverE = electron.hadronicOverEm();
 float ooemoop = (1.0/electron.ecalEnergy() - electron.eSuperClusterOverP()/electron.ecalEnergy());

 int mishits = electron.gsfTrack()->trackerExpectedHitsInner().numberOfHits();
 int nAmbiguousGsfTracks = electron.ambiguousGsfTracksSize();


 reco::GsfTrackRef eleTrack = electron.gsfTrack() ;
 float dxy = eleTrack->dxy(PVPoint_);
 float dz = eleTrack->dz (PVPoint_);

 edm::Handle<reco::BeamSpot> BSHandle;
 iEvent.getByLabel(BSTag_, BSHandle);
 const reco::BeamSpot BS = *BSHandle;

 
 bool isConverted = ConversionTools::hasMatchedConversion(electron, conversions_h, BS.position());
 
 // default
 if( (pt > 10.) && (fabs(eta) < 2.5) &&
       ( ( (isEB == 1) && (fabs(DetaIn) < 0.007) ) || ( (isEB == 0) && (fabs(DetaIn) < 0.009) ) ) &&
       ( ( (isEB == 1) && (fabs(DphiIn) < 0.150) ) || ( (isEB == 0) && (fabs(DphiIn) < 0.100) ) ) &&
       ( ( (isEB == 1) && (sigmaIetaIeta < 0.010) ) || ( (isEB == 0) && (sigmaIetaIeta < 0.030) ) ) &&
       ( ( (isEB == 1) && (HOverE < 0.120) ) || ( (isEB == 0) && (HOverE < 0.100) ) ) &&
       ( ( (isEB == 1) && (fabs(ooemoop) < 0.050) ) || ( (isEB == 0) && (fabs(ooemoop) < 0.050) ) ) &&
       ( ( (isEB == 1) && (fabs(dxy) < 0.020) ) || ( (isEB == 0) && (fabs(dxy) < 0.020) ) ) &&
       ( ( (isEB == 1) && (fabs(dz) < 0.200) ) || ( (isEB == 0) && (fabs(dz) < 0.200) ) ) &&
       ( ( (isEB == 1) && (!isConverted) ) || ( (isEB == 0) && (!isConverted) ) ) &&
       ( mishits == 0 ) &&
       ( nAmbiguousGsfTracks == 0 ) &&
       ( ( (isEB == 1 ) && (combIso/pt < 0.150) ) || ( (isEB == 0 ) && (combIso/pt < 0.100) ) ) )
   isLooseEle=true;;
 
 // 2011-like test
 //if( (pt > 10.) && (fabs(eta) < 2.5) &&
 // ( ( (isEB == 1) && (fabs(DetaIn) < 0.007) ) || ( (isEB == 0) && (fabs(DetaIn) < 0.010) ) ) &&
 // ( ( (isEB == 1) && (fabs(DphiIn) < 0.800) ) || ( (isEB == 0) && (fabs(DphiIn) < 0.700) ) ) &&
 // ( ( (isEB == 1) && (sigmaIetaIeta < 0.010) ) || ( (isEB == 0) && (sigmaIetaIeta < 0.030) ) ) &&
 // ( ( (isEB == 1) && (HOverE < 0.150) ) || ( (isEB == 0) && (HOverE < 0.070) ) ) &&
 // ( mishits == 0 ) &&
 // ( nAmbiguousGsfTracks == 0 ) &&
 // ( ( (isEB == 1 ) && (combIso/pt < 0.150) ) || ( (isEB == 0 ) && (combIso/pt < 0.100) ) ) )
 // isLooseEle=true;;

 
return isLooseEle;
}

// -----------------------------------------------------------------------------------------

bool AnalyzerEle::myWselection (const edm::Event & iEvent, const edm::EventSetup & iSetup)
{
  // default
  float combIso = ele1_tkIso +std::max(ele1_emIso + ele1_hadIso - rho_*ele1_effAreaForIso,float(0.));
  
  // 2011-like test
  //float combIso = ele1_tkIso + ele1_emIso + ele1_hadIso - rho*3.14159*0.3*0.3;
  
  if( ele1_pt < 30. ) return false;
  
  // default
  if( ( ele1_isEB == 1 ) && ( combIso/ele1_pt > 0.05 ) ) return false;
  if( ( ele1_isEB == 1 ) && ( fabs(ele1_DetaIn) > 0.004 ) ) return false;
  if( ( ele1_isEB == 1 ) && ( fabs(ele1_DphiIn) > 0.030 ) ) return false;
  if( ( ele1_isEB == 1 ) && ( ele1_sigmaIetaIeta > 0.010 ) ) return false;
  if( ( ele1_isEB == 1 ) && ( ele1_HOverE > 0.120 ) ) return false;
  if( ( ele1_isEB == 1 ) && ( fabs(ele1_ooemoop) > 0.050 ) ) return false;
  
  if( ( ele1_isEB == 0 ) && ( combIso/ele1_pt > 0.035 ) ) return false;
  if( ( ele1_isEB == 0 ) && ( fabs(ele1_DetaIn) > 0.005 ) ) return false;
  if( ( ele1_isEB == 0 ) && ( fabs(ele1_DphiIn) > 0.020 ) ) return false;
  if( ( ele1_isEB == 0 ) && ( ele1_sigmaIetaIeta > 0.030 ) ) return false;
  if( ( ele1_isEB == 0 ) && ( ele1_HOverE > 0.100 ) ) return false;
  if( ( ele1_isEB == 0 ) && ( fabs(ele1_ooemoop) > 0.050 ) ) return false;
  
  // 2011-like test
  //if( ( ele1_isEB == 1 ) && ( combIso/ele1_pt > 0.04 ) ) return false;
  //if( ( ele1_isEB == 1 ) && ( fabs(ele1_DetaIn) > 0.004 ) ) return false;
  //if( ( ele1_isEB == 1 ) && ( fabs(ele1_DphiIn) > 0.030 ) ) return false;
  //if( ( ele1_isEB == 1 ) && ( ele1_HOverE > 0.25 ) ) return false;
  
  //if( ( ele1_isEB == 0 ) && ( combIso/ele1_pt > 0.03 ) ) return false;
  //if( ( ele1_isEB == 0 ) && ( fabs(ele1_DetaIn) > 0.005 ) ) return false;
  //if( ( ele1_isEB == 0 ) && ( fabs(ele1_DphiIn) > 0.020 ) ) return false;
  //if( ( ele1_isEB == 0 ) && ( ele1_HOverE > 0.025 ) ) return false;
       
  if( met_et < 25.00 ) return false;
  if( ele1Met_mt < 50.00 ) return false;
  if( ele1Met_Dphi < 1.57 ) return false;
  
  return true;
  
}

// -----------------------------------------------------------------------------------------

bool AnalyzerEle::myZselection (const edm::Event & iEvent, const edm::EventSetup & iSetup)
{
   if( met_et > 40. ) return false;
   if( ele1ele2_m < 60. ) return false;
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
    //creating a dummy PV
    PV_z_ = -999.;
    PV_d0_ = -999.;
   }

 math::XYZPoint PVPoint(PV.position().x(), PV.position().y(), PV.position().z());
 PVPoint_ = PVPoint;

}
//------------------------------------------------------------------------------------------------------------
void AnalyzerEle::fillRhoInfo(const edm::Event & iEvent, const edm::EventSetup & iSetup)
{
  
 edm::Handle< double > rhoHandle;
  iEvent.getByLabel(rhoTag_,rhoHandle);
  rho_ = *rhoHandle;
}

// ------------------------------------------------------------------------------------------------------------

void AnalyzerEle::fillEleInfo(const edm::Event & iEvent, const edm::EventSetup & iSetup, const int iEle, const std::string eleName)
{
  if( verbosity_ )
    std::cout << ">>> AnalyzerEle::fillEleInfo start <<<" << std::endl;
  
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
  
  if( saveRecHitMatrix_ )
  {
    // std::cout << ""
    iEvent.getByLabel(SRFlagCollection_EB_, SRFlagsEB );
    for(EBSrFlagCollection::const_iterator it = SRFlagsEB->begin(); it != SRFlagsEB->end(); ++it)
    {
      const int flag = it->value();
      if( flag != EcalSrFlag::SRF_FULL ) continue;
      const EcalTrigTowerDetId TTId = it->id();
      TTIdList.push_back(TTId);
      //std::cout << "flag: " << flag << " TTId: " << TTId << " iEta: " << TTId.ieta() << " iPhi: " << TTId.iphi() << std::endl;
    }
  }
  
  //*********** EE SR FLAGS
  edm::Handle<EESrFlagCollection> SRFlagsEE;
  std::vector<EcalScDetId> SCIdList;
  if( saveRecHitMatrix_ )
  {
    iEvent.getByLabel(SRFlagCollection_EE_, SRFlagsEE );
    for(EESrFlagCollection::const_iterator it = SRFlagsEE->begin(); it != SRFlagsEE->end(); ++it)
    {
      const int flag = it->value();
      if( flag != EcalSrFlag::SRF_FULL ) continue;
      const EcalScDetId SCId = it->id();
      SCIdList.push_back(SCId);
      //std::cout << "flag: " << flag << " SCId: " << SCId << " iEta: " << SCId.ieta() << " iPhi: " << SCId.iphi() << std::endl;
    }
  }
  
  //************* ELECTRONS
  edm::Handle<edm::View<reco::GsfElectron> > electronHandle;
  iEvent.getByLabel(EleTag_,electronHandle);
  edm::View<reco::GsfElectron> electrons = *electronHandle;
  
  //************* VERTEX COLLECTION
  edm::Handle<reco::VertexCollection> hVertexProduct;
  iEvent.getByLabel("offlinePrimaryVerticesWithBS",hVertexProduct);
  
  //************* CLUSTER LAZY TOOLS
  EcalClusterLazyTools lazyTools(iEvent,iSetup,recHitCollection_EB_,recHitCollection_EE_);
  
  //************* REGRESSION
//   if( !ecorr_.IsInitialized() ){
//     //ecorr_.Initialize(iSetup,"gbrv3ele_52x.root");
//     ecorr_.Initialize(iSetup,"/afs/cern.ch/user/b/bendavid/cmspublic/regweights52xV3/gbrv3ele_52x.root");
//   }
//   if( !ecorrPho_.IsInitialized()){
//     //ecorrPho_.Initialize(iSetup,"gbrv3ph_52x.root");
//     ecorrPho_.Initialize(iSetup,"/afs/cern.ch/user/b/bendavid/cmspublic/regweights52xV3/gbrv3ph_52x.root");
//   }
  
  //************* CLUSTER PU CLEANING TOOLS
  EcalClusterPUCleaningTools cleaningTools(iEvent, iSetup, recHitCollection_EB_, recHitCollection_EE_);
  
  
  
  // Take the correct ele
  bool printOut = false;
  reco::GsfElectron electron = electrons.at(iEle);
  
  if ( eleName == "ele1")
  {
    edm::InputTag EleBad = edm::InputTag("gsfElectrons");
    
    if(EleTag_== EleBad && !electron.ecalDriven()) return ;
    
    ele1=electron.p4();
    ele1_charge=electron.charge();
    ele1_p=ele1.P();
    ele1_pt=ele1.Pt();
    ele1_eta=ele1.eta();
    ele1_phi=ele1.phi();
    
    ele1_isEB=electron.isEB();
    ele1_isEBEEGap=electron.isEBEEGap();
    ele1_isEBEtaGap=electron.isEBEtaGap();
    ele1_isEBPhiGap=electron.isEBPhiGap();
    ele1_isEEDeeGap=electron.isEEDeeGap();
    ele1_isEERingGap=electron.isEERingGap();
    
    ele1_sigmaIetaIeta=electron.sigmaIetaIeta();
    ele1_DphiIn=electron.deltaPhiSuperClusterTrackAtVtx();
    ele1_DetaIn=electron.deltaEtaSuperClusterTrackAtVtx();
    ele1_HOverE=electron.hadronicOverEm();
    ele1_ooemoop = (1.0/electron.ecalEnergy() - electron.eSuperClusterOverP()/electron.ecalEnergy());
    ele1_tkIso=electron.dr03TkSumPt();
    ele1_emIso=electron.dr03EcalRecHitSumEt();
    ele1_hadIso=electron.dr03HcalDepth1TowerSumEt()+electron.dr03HcalDepth2TowerSumEt();
    
    ele1_effAreaForIso = 0.;
//     reco::ElectronEffectiveArea::GetElectronEffectiveArea(reco::ElectronEffectiveArea::kEleGammaAndNeutralHadronIso03, electron.superCluster()->eta(), reco::ElectronEffectiveArea::kEleEAData2011);
    
    reco::GsfTrackRef eleTrack = electron.gsfTrack();
    ele1_dxy_PV = eleTrack->dxy (PVPoint_);
    ele1_dz_PV = eleTrack->dz (PVPoint_);
    ele1_sigmaP =electron.corrections().trackMomentumError;
    
    reco::SuperClusterRef scRef = electron.superCluster();
    const edm::Ptr<reco::CaloCluster>& seedCluster = scRef->seed();
    
    double R = TMath::Sqrt(scRef->x()*scRef->x() + scRef->y()*scRef->y() +scRef->z()*scRef->z());
    double Rt = TMath::Sqrt(scRef->x()*scRef->x() + scRef->y()*scRef->y());
    
    edm::Handle< double > rhoHandle;
    iEvent.getByLabel(rhoTag_,rhoHandle);
    std::pair<double,double> cor = std::pair<double,double> (0,0);
//       ecorr_.CorrectedEnergyWithError(electron,*hVertexProduct,*rhoHandle,lazyTools,iSetup);
    
    ele1_scERaw=scRef->rawEnergy();
    ele1_scEtRaw=scRef->rawEnergy()*(Rt/R);
    ele1_scEt=scRef->energy()*(Rt/R);
    ele1_scEtaWidth=scRef->etaWidth();
    ele1_scPhiWidth=scRef->phiWidth();
    ele1_scE=scRef->energy();
    ele1_scEta=scRef->eta();
    ele1_scPhi=scRef->phi();
    ele1_scE_regression=cor.first;
    ele1_scEerr_regression = cor.second;
    
    std::pair<double,double> corPho = std::pair<double,double> (0,0);
//     ecorrPho_.CorrectedEnergyWithError(electron,*hVertexProduct,*rhoHandle,lazyTools,iSetup);
    ele1_scE_regression_PhotonTuned=corPho.first;
    ele1_scEerr_regression_PhotonTuned = corPho.second;
    
    
    
    EcalClusterLocal ecalLocalCoord;
    float bcLocalEta, bcLocalPhi, bcThetatilt, bcPhitilt;
    int bcIeta, bcIphi;
    bcLocalEta = 0;
    
    if ( electron.isEB() )
     ecalLocalCoord.localCoordsEB(*seedCluster,iSetup,bcLocalEta,bcLocalPhi,bcIeta,bcIphi,bcThetatilt,bcPhitilt);
    if ( electron.isEE() )
    ecalLocalCoord.localCoordsEE(*seedCluster,iSetup,bcLocalEta,bcLocalPhi,bcIeta,bcIphi,bcThetatilt,bcPhitilt);
    
    
    ele1_scLocalEta=bcLocalEta;
    ele1_scLocalPhi=bcLocalPhi;
    
    
    // crack correction variables and local containment corrections
    EcalClusterCrackCorrection -> init(iSetup);
    EcalClusterLocalContCorrection -> init(iSetup);
    double crackcor = 1.;
    double localContCorr = 1.;
    
    for(reco::CaloCluster_iterator cIt = electron.superCluster()->clustersBegin();
        cIt != electron.superCluster()->clustersEnd(); ++cIt)
    {
      const reco::CaloClusterPtr cc = *cIt;
      crackcor *= ( (electron.superCluster()->rawEnergy() + (*cIt)->energy()*(EcalClusterCrackCorrection->getValue(*cc)-1.)) / electron.superCluster()->rawEnergy() );
    }
    localContCorr = EcalClusterLocalContCorrection->getValue(*electron.superCluster(), 1) ;
    
    ele1_scCrackCorr=crackcor;
    ele1_scLocalContCorr=localContCorr;
    
    
    reco::SuperCluster cleanedSC = cleaningTools.CleanedSuperCluster(0.02, *scRef, iEvent );
    reco::CaloClusterPtr myseed = (*scRef).seed();
    if ( !((myseed->seed()).rawId()) || (myseed->seed()).rawId()==0 )
    {
      ele1_scERaw_PUcleaned=-9999.;
      ele1_scEtaWidth_PUcleaned=-9999.;
      ele1_scPhiWidth_PUcleaned=-9999.;
      ele1_fCorrection_PUcleaned=-9999.;
    }
    else
    {
      ele1_scERaw_PUcleaned=cleanedSC.energy();
      ele1_scEtaWidth_PUcleaned=cleanedSC.etaWidth();
      ele1_scPhiWidth_PUcleaned=cleanedSC.phiWidth();
//       float fCorrCleaned = fClusterCorrections(cleanedSC.energy() + scRef->preshowerEnergy(), cleanedSC.eta(),cleanedSC.phiWidth()/cleanedSC.etaWidth(),params_)/(cleanedSC.energy()+ scRef->preshowerEnergy());
//       ele1_fCorrection_PUcleaned=fCorrCleaned;
    }
    
    
    ele1_fEta = scRef->energy()/scRef->rawEnergy();
    ele1_tkP = electron.trackMomentumAtVtx().R();
    ele1_tkPt=electron.trackMomentumAtVtx().Rho();
    ele1_fbrem=electron.fbrem();
    ele1_e5x5=electron.e5x5();
    ele1_eSeedBC=(scRef->seed())->energy();
    ele1_es=scRef->preshowerEnergy();
    
    float E3x3 = 0;
    
    if( electron.isEB() ){
      E3x3 = EcalClusterTools::e3x3( *scRef, theBarrelEcalRecHits, topology);
    }
    if( electron.isEE() ){
      E3x3 = EcalClusterTools::e3x3( *scRef, theEndcapEcalRecHits, topology);
    }
    
    ele1_e3x3=E3x3;
    ele1_scNxtal = scRef->hitsAndFractions().size();
    ele1_bcN = electron.basicClustersSize();
    
    float energy=0.;
    int ieta=0;
    int iphi=0;
    int ix=0;
    int iy=0;
    int zside=0;
    float seedICConstant = -1.;
    float seedLaserAlpha = -1.;
    float seedLaserCorrection = -1.;
    
    if(electron.isEB())
    {
      if( printOut )
        std::cout << "*** EB ***" << std::endl;
      
      std::pair<DetId, float> id = EcalClusterTools::getMaximum(seedCluster->hitsAndFractions(), theBarrelEcalRecHits);
      
      // flag
      EcalRecHitCollection::const_iterator it = theBarrelEcalRecHits->find(id.first);
      
      if( it != theBarrelEcalRecHits->end() )
      {
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
      if( ICMapIt != ICMap.end() )
        seedICConstant = *ICMapIt;
      
      // laser alphas
      EcalLaserAlphaMap::const_iterator italpha = theEcalLaserAlphaMap->find(id.first);
      if( italpha != theEcalLaserAlphaMap->end() )
        seedLaserAlpha = (*italpha);
      
      // laser correction
      seedLaserCorrection = theLaser->getLaserCorrection(EBDetId(id.first), iEvent.time());
    }
    
    else
    {
      if( printOut )
        std::cout << "*** EE ***" << std::endl;
      
      std::pair<DetId, float> id = EcalClusterTools::getMaximum(seedCluster->hitsAndFractions(), theEndcapEcalRecHits);
      
      // flag - OutOfTime
      EcalRecHitCollection::const_iterator it = theEndcapEcalRecHits->find(id.first);
      
      if( it != theEndcapEcalRecHits->end() )
      {
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
    
    ele1_seedE=energy;
    ele1_seedLaserAlpha=seedLaserAlpha;
    ele1_seedLaserCorr=seedLaserCorrection;
    ele1_seedICConstant=seedICConstant;
    ele1_seedIeta =ieta;
    ele1_seedIphi = iphi;
    ele1_seedIx=ix;
    ele1_seedIy=iy;
    ele1_seedZside=zside;
    ele1_EOverP=electron.eSuperClusterOverP();
           
    // rechit variables
    int numRecHit = 0;
    float sumRecHitE = 0.;
    float sumLaserCorrectionRecHitE = 0.;
    float sumRecHitE5x5 = 0.;
    float sumLaserCorrectionRecHitE5x5 = 0.;
    float sumRecHitE3x3 = 0.;
    float sumLaserCorrectionRecHitE3x3 = 0.;
    
    const std::vector<std::pair<DetId,float> >& hits = scRef->hitsAndFractions();
    
    if( printOut )
    {
      std::cout << "runId: " << iEvent.id().run()
          << std::fixed
          << " electron eta: " << std::setprecision(2) << std::setw(5) << electron.eta()
          << " electron phi: " << std::setprecision(2) << std::setw(5) << electron.phi()
          << " SC energy: " << std::setprecision(2) << std::setw(6) << scRef -> energy()
          << std::endl;
    }


    if(saveRecHitMatrix_)
    {
      float theLaserCorrection = -1.;
      float theICCorrection = -1.;
      
      if(electron.isEB())
      {
        DetId seedId = EcalClusterTools::getMaximum( scRef->hitsAndFractions(), theBarrelEcalRecHits ).first;
//save the matrix in case of eleSeed
std::vector<DetId> rectangle = EcalClusterTools::matrixDetId(topology, seedId, -9, 9, -9, 9);

int it = 0;
for(std::vector<DetId>::const_iterator itr = rectangle.begin(); itr != rectangle.end(); ++itr)
        {
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
      
      else
      {
        DetId seedId = EcalClusterTools::getMaximum( scRef->hitsAndFractions(), theEndcapEcalRecHits ).first;
//save the matrix in case of eleSeed
std::vector<DetId> rectangle = EcalClusterTools::matrixDetId(topology, seedId, -9, 9, -9, 9);

for(std::vector<DetId>::const_iterator itr = rectangle.begin(); itr != rectangle.end(); ++itr)
        {
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
    
    for(std::vector<std::pair<DetId,float> >::const_iterator rh = hits.begin(); rh!=hits.end(); ++rh)
    {
      float rhLaserCorrection = 1.;
      float rhICCorrection = 1.;
      float theLaserCorrection = -1.;
      float theICCorrection = -1.;
      float theAlpha = -1.;
            
      if ((*rh).first.subdetId()== EcalBarrel)
      {
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
        
        if( printOut && itrechit->energy() > 1. )
        {
          std::cout << std::fixed
              << " recHitLC: " << std::setprecision(6) << std::setw(8) << theLaserCorrection
              << " recHitIC: " << std::setprecision(6) << std::setw(8) << theICCorrection
              << std::endl;
        }
      }
        
      if( (*rh).first.subdetId()== EcalEndcap )
      {
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
        
        if( printOut && itrechit->energy() > 1. )
        {
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
    
//     ele1_fEtaCorr = fClusterCorrections(sumRecHitE+ele1_es,ele1_scEta,scRef->phiWidth()/scRef->etaWidth(),params_)/fClusterCorrections(scRef->rawEnergy()+ele1_es,ele1_scEta,scRef->phiWidth()/scRef->etaWidth(),params_);
    
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
     
    ele1_eRegrInput_Deta_bC_sC = b->eta()-s->eta();
    ele1_eRegrInput_Dphi_bC_sC = reco::deltaPhi(b->phi(),s->phi());
    ele1_eRegrInput_bCE_Over_sCE = b->energy()/b->energy();
    ele1_eRegrInput_e3x3_Over_bCE = lazyTools.e3x3(*b)/b->energy();
    ele1_eRegrInput_e5x5_Over_bCE = lazyTools.e5x5(*b)/b->energy();
    ele1_eRegrInput_sigietaieta_bC1 = sqrt(lazyTools.localCovariances(*b)[0]);
    ele1_eRegrInput_sigiphiiphi_bC1 = sqrt(lazyTools.localCovariances(*b)[2]);
    ele1_eRegrInput_sigietaiphi_bC1 = lazyTools.localCovariances(*b)[1];
    ele1_eRegrInput_bEMax_Over_bCE = bemax/b->energy();
    ele1_eRegrInput_bE2nd_Over_bCE = be2nd/b->energy();
    ele1_eRegrInput_bEtop_Over_bCE = betop/b->energy();
    ele1_eRegrInput_bEbot_Over_bCE = bebottom/b->energy();
    ele1_eRegrInput_bEleft_Over_bCE = beleft/b->energy();
    ele1_eRegrInput_bEright_Over_bCE = beright/b->energy();
    ele1_eRegrInput_be2x5max_Over_bCE = be2x5max/b->energy();
    ele1_eRegrInput_be2x5top_Over_bCE = be2x5top/b->energy();
    ele1_eRegrInput_be2x5bottom_Over_bCE = be2x5bottom/b->energy();
    ele1_eRegrInput_be2x5left_Over_bCE = be2x5left/b->energy();
    ele1_eRegrInput_be2x5right_Over_bCE = be2x5right/b->energy();
    
    
    
    if( isbarrel )
    {
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
    else
    {
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
// ele1_tangent_n = eleTangent.size();
}
      ele1_tangent_n = numberOfValidHits_Trk;
    }
  }

  if ( eleName == "ele2" )
  {
    edm::InputTag EleBad = edm::InputTag("gsfElectrons");
    
    if(EleTag_== EleBad && !electron.ecalDriven()) return ;
    
    ele2=electron.p4();
    ele2_charge=electron.charge();
    ele2_p=ele2.P();
    ele2_pt=ele2.Pt();
    ele2_eta=ele2.eta();
    ele2_phi=ele2.phi();
    
    ele2_isEB=electron.isEB();
    ele2_isEBEEGap=electron.isEBEEGap();
    ele2_isEBEtaGap=electron.isEBEtaGap();
    ele2_isEBPhiGap=electron.isEBPhiGap();
    ele2_isEEDeeGap=electron.isEEDeeGap();
    ele2_isEERingGap=electron.isEERingGap();
    
    ele2_sigmaIetaIeta=electron.sigmaIetaIeta();
    ele2_DphiIn=electron.deltaPhiSuperClusterTrackAtVtx();
    ele2_DetaIn=electron.deltaEtaSuperClusterTrackAtVtx();
    ele2_HOverE=electron.hadronicOverEm();
    ele2_ooemoop= (1.0/electron.ecalEnergy() - electron.eSuperClusterOverP()/electron.ecalEnergy());
    
    ele2_tkIso=electron.dr03TkSumPt();
    ele2_emIso=electron.dr03EcalRecHitSumEt();
    ele2_hadIso=electron.dr03HcalDepth1TowerSumEt()+electron.dr03HcalDepth2TowerSumEt();
    
    ele2_effAreaForIso = 0.;
//     reco::ElectronEffectiveArea::GetElectronEffectiveArea(reco::ElectronEffectiveArea::kEleGammaAndNeutralHadronIso03, electron.superCluster()->eta(), reco::ElectronEffectiveArea::kEleEAData2011);
    
    reco::GsfTrackRef eleTrack = electron.gsfTrack() ;
    ele2_dxy_PV = eleTrack->dxy (PVPoint_);
    ele2_dz_PV = eleTrack->dz (PVPoint_);
    ele2_sigmaP =electron.corrections().trackMomentumError;
    
    
    reco::SuperClusterRef scRef = electron.superCluster();
    const edm::Ptr<reco::CaloCluster>& seedCluster = scRef->seed();
    
    double R = TMath::Sqrt(scRef->x()*scRef->x() + scRef->y()*scRef->y() +scRef->z()*scRef->z());
    double Rt = TMath::Sqrt(scRef->x()*scRef->x() + scRef->y()*scRef->y());
    
    edm::Handle< double > rhoHandle;
    iEvent.getByLabel(rhoTag_,rhoHandle);
    std::pair<double,double> cor = std::pair<double,double> (0,0);
//     ecorr_.CorrectedEnergyWithError(electron,*hVertexProduct,*rhoHandle,lazyTools,iSetup);
    
    
    ele2_scERaw=scRef->rawEnergy();
    ele2_scEtRaw=scRef->rawEnergy()*(Rt/R);
    ele2_scEt=scRef->energy()*(Rt/R);
    ele2_scEtaWidth=scRef->etaWidth();
    ele2_scPhiWidth=scRef->phiWidth();
    ele2_scE=scRef->energy();
    ele2_scEta=scRef->eta();
    ele2_scPhi=scRef->phi();
    ele2_scE_regression=cor.first;
    ele2_scEerr_regression = cor.second;
    
    std::pair<double,double> corPho = std::pair<double,double> (0,0);
//     ecorrPho_.CorrectedEnergyWithError(electron,*hVertexProduct,*rhoHandle,lazyTools,iSetup);
    ele2_scE_regression_PhotonTuned=corPho.first;
    ele2_scEerr_regression_PhotonTuned = corPho.second;
    
    
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
        cIt != electron.superCluster()->clustersEnd(); ++cIt)
    {
      const reco::CaloClusterPtr cc = *cIt;
      crackcor *= ( (electron.superCluster()->rawEnergy() + (*cIt)->energy()*(EcalClusterCrackCorrection->getValue(*cc)-1.)) / electron.superCluster()->rawEnergy() );
    }
    localContCorr = EcalClusterLocalContCorrection->getValue(*electron.superCluster(), 1) ;
    
    ele2_scCrackCorr=crackcor;
    ele2_scLocalContCorr=localContCorr;
    
    
    reco::SuperCluster cleanedSC = cleaningTools.CleanedSuperCluster(0.02, *scRef, iEvent );
    reco::CaloClusterPtr myseed = (*scRef).seed();
    if ( !((myseed->seed()).rawId()) || (myseed->seed()).rawId()==0 )
    {
      ele2_scERaw_PUcleaned=-9999.;
      ele2_scEtaWidth_PUcleaned=-9999.;
      ele2_scPhiWidth_PUcleaned=-9999.;
      ele2_fCorrection_PUcleaned=-9999.;
    }
    else
    {
      ele2_scERaw_PUcleaned=cleanedSC.energy();
      ele2_scEtaWidth_PUcleaned=cleanedSC.etaWidth();
      ele2_scPhiWidth_PUcleaned=cleanedSC.phiWidth();
//       float fCorrCleaned = fClusterCorrections(cleanedSC.energy() + scRef->preshowerEnergy(), cleanedSC.eta(),cleanedSC.phiWidth()/cleanedSC.etaWidth(),params_)/(cleanedSC.energy()+ scRef->preshowerEnergy());
//       ele2_fCorrection_PUcleaned=fCorrCleaned;
    }
    
    
    ele2_fEta = scRef->energy()/scRef->rawEnergy();
    ele2_tkP = electron.trackMomentumAtVtx().R();
    ele2_tkPt=electron.trackMomentumAtVtx().Rho();
    ele2_fbrem=electron.fbrem();
    ele2_e5x5=electron.e5x5();
    ele2_eSeedBC=(scRef->seed())->energy();
    ele2_es=scRef->preshowerEnergy();
    
    float E3x3 = 0;
    
    if( electron.isEB() ){
      E3x3 = EcalClusterTools::e3x3( *scRef, theBarrelEcalRecHits, topology);
    }
    
    if ( electron.isEE() ){
      E3x3 = EcalClusterTools::e3x3( *scRef, theEndcapEcalRecHits, topology);
    }
    
    ele2_e3x3=E3x3;
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
    
    if(electron.isEB())
    {
      std::pair<DetId, float> id = EcalClusterTools::getMaximum(seedCluster->hitsAndFractions(), theBarrelEcalRecHits);
      
      // flag
      EcalRecHitCollection::const_iterator it = theBarrelEcalRecHits->find(id.first);
      
      if( it != theBarrelEcalRecHits->end() )
      {
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
      if( ICMapIt != ICMap.end() )
        seedICConstant = *ICMapIt;
      
      // laser alphas
      EcalLaserAlphaMap::const_iterator italpha = theEcalLaserAlphaMap->find(id.first);
      if( italpha != theEcalLaserAlphaMap->end() )
        seedLaserAlpha = (*italpha);
      
      // laser correction
      seedLaserCorrection = theLaser->getLaserCorrection(EBDetId(id.first), iEvent.time());
    }
    
    else
    {
      std::pair<DetId, float> id = EcalClusterTools::getMaximum(seedCluster->hitsAndFractions(), theEndcapEcalRecHits);
      
      // flag - OutOfTime
      EcalRecHitCollection::const_iterator it = theEndcapEcalRecHits->find(id.first);
      
      if( it != theEndcapEcalRecHits->end() )
      {
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
    
    ele2_seedE=energy;
    ele2_seedLaserAlpha=seedLaserAlpha;
    ele2_seedLaserCorr=seedLaserCorrection;
    ele2_seedICConstant=seedICConstant;
    ele2_seedIeta =ieta;
    ele2_seedIphi = iphi;
    ele2_seedIx=ix;
    ele2_seedIy=iy;
    ele2_seedZside=zside;
    ele2_EOverP=electron.eSuperClusterOverP();
           
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
    
    if( printOut )
    {
      std::cout << "runId: " << iEvent.id().run()
          << std::fixed
          << " electron eta: " << std::setprecision(2) << std::setw(5) << electron.eta()
          << " electron phi: " << std::setprecision(2) << std::setw(5) << electron.phi()
          << " SC energy: " << std::setprecision(2) << std::setw(6) << scRef -> energy()
          << std::endl;
    }

    if(saveRecHitMatrix_)
    {
      float theLaserCorrection = -1.;
      float theICCorrection = -1.;
      
      if(electron.isEB())
      {
        DetId seedId = EcalClusterTools::getMaximum( scRef->hitsAndFractions(), theBarrelEcalRecHits ).first;
//save the matrix in case of eleSeed
std::vector<DetId> rectangle = EcalClusterTools::matrixDetId(topology, seedId, -9, 9, -9, 9);

for(std::vector<DetId>::const_iterator itr = rectangle.begin(); itr != rectangle.end(); ++itr)
        {
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
      
      else
      {
        DetId seedId = EcalClusterTools::getMaximum( scRef->hitsAndFractions(), theEndcapEcalRecHits ).first;
//save the matrix in case of eleSeed
std::vector<DetId> rectangle = EcalClusterTools::matrixDetId(topology, seedId, -9, 9, -9, 9);

for(std::vector<DetId>::const_iterator itr = rectangle.begin(); itr != rectangle.end(); ++itr)
        {
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
    
    for(std::vector<std::pair<DetId,float> >::const_iterator rh = hits.begin(); rh!=hits.end(); ++rh)
    {
      float rhLaserCorrection = 1.;
      float rhICCorrection = 1.;
      float theLaserCorrection = -1.;
      float theICCorrection = -1.;
      float theAlpha = -1.;
            
      if ((*rh).first.subdetId()== EcalBarrel)
      {
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
        
        if( printOut && itrechit->energy() > 1. )
        {
          std::cout << std::fixed
              << " recHitLC: " << std::setprecision(6) << std::setw(8) << theLaserCorrection
              << " recHitIC: " << std::setprecision(6) << std::setw(8) << theICCorrection
              << std::endl;
        }
      }
        
      if ((*rh).first.subdetId()== EcalEndcap)
      {
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
    
//     ele2_fEtaCorr = fClusterCorrections(sumRecHitE+ele2_es,ele2_scEta,scRef->phiWidth()/scRef->etaWidth(),params_)/fClusterCorrections(scRef->rawEnergy()+ele2_es,ele2_scEta,scRef->phiWidth()/scRef->etaWidth(),params_);
        
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
   ele2_eRegrInput_r9 = lazyTools.e3x3(*b)/s->rawEnergy();
   ele2_eRegrInput_eta = s->eta();
   ele2_eRegrInput_phi = s->phi();
   ele2_eRegrInput_r25 = lazyTools.e5x5(*b)/s->rawEnergy();
   ele2_eRegrInput_hoe = electron.hcalOverEcal();
   ele2_eRegrInput_etaW = s->etaWidth() ;
   ele2_eRegrInput_phiW = s->phiWidth() ;
   ele2_eRegrInput_SCsize = s->clustersSize() ;
   ele2_eRegrInput_rho = *rhoHandle;
   ele2_eRegrInput_nPV = hVertexProduct->size();
   
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
   
   
   
   if( isbarrel)
   {
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
   else
   {
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
  
  
 if( verbosity_ )
   std::cout << ">>> AnalyzerEle::fillEleInfo end <<<" << std::endl;
}

// -----------------------------------------------------------------------------------------

 void AnalyzerEle::fillMetInfo(const edm::Event & iEvent, const edm::EventSetup & iSetup){
 //std::cout << "SimpleNtupleCalib::fillPFMetInfo" << std::endl;
  
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
  
  p_met = &met;
  met_et = p_met->Et();
  met_phi = p_met->phi();
  
  ele1Met_mt = sqrt( 2. * ele1_pt * met_et * ( 1. - cos( deltaPhi(ele1_phi,met_phi) ) ) );
  ele1Met_Dphi = deltaPhi(ele1_phi,met_phi);


}

//----------------------------------------------------------------------------------------------

void AnalyzerEle::fillDoubleEleInfo(const edm::Event & iEvent, const edm::EventSetup & iSetup){

  ele1ele2_m = (ele1 + ele2).mass();
  
  ROOT::Math::PtEtaPhiEVector ele1_sc(ele1_scE*sin(2*atan(exp(-1.*ele1_eta))),ele1_eta,ele1_phi,ele1_scE);
  ROOT::Math::PtEtaPhiEVector ele2_sc(ele2_scE*sin(2*atan(exp(-1.*ele2_eta))),ele2_eta,ele2_phi,ele2_scE);
  ele1ele2_scM = (ele1_sc + ele2_sc).mass();

  ROOT::Math::PtEtaPhiEVector ele1_sc_regression(ele1_scE_regression*sin(2*atan(exp(-1.*ele1_eta))),ele1_eta,ele1_phi,ele1_scE_regression);
  ROOT::Math::PtEtaPhiEVector ele2_sc_regression(ele2_scE_regression*sin(2*atan(exp(-1.*ele2_eta))),ele2_eta,ele2_phi,ele2_scE_regression);
  ele1ele2_scM_regression = (ele1_sc_regression + ele2_sc_regression).mass();

}


double AnalyzerEle::deltaPhi(const double& phi1, const double& phi2){
  double deltaphi = fabs(phi1 - phi2);
  if (deltaphi > 3.141592654) deltaphi = 6.283185308 - deltaphi;
  return deltaphi;
}



// ------------ method called once each job just before starting event loop ------------
void
  AnalyzerEle::beginJob()
{
}

// ------------ method called once each job just after ending the event loop ------------
void
  AnalyzerEle::endJob()
{
}

// ------------ method called when starting to processes a run ------------
/*
void
AnalyzerEle::beginRun(edm::Run const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when ending the processing of a run ------------
/*
void
AnalyzerEle::endRun(edm::Run const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when starting to processes a luminosity block ------------
/*
void
AnalyzerEle::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when ending the processing of a luminosity block ------------
/*
void
AnalyzerEle::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

//define this as a plug-in
DEFINE_FWK_MODULE(AnalyzerEle);
