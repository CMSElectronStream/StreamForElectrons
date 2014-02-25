#ifndef HLTTigger_Egamma_selectedElectronFEDListProducer_h
#define HLTTigger_Egamma_selectedFEDListProducer_h

#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <fstream>
#include "TLorentzVector.h"
#include "TVector3.h"
#include <boost/scoped_ptr.hpp>

#include "DataFormats/FEDRawData/interface/FEDRawData.h"
#include "DataFormats/FEDRawData/interface/FEDHeader.h"
#include "DataFormats/FEDRawData/interface/FEDTrailer.h"
#include "DataFormats/FEDRawData/interface/FEDRawDataCollection.h"
#include "DataFormats/FEDRawData/interface/FEDNumbering.h"

#include "DataFormats/HLTReco/interface/TriggerFilterObjectWithRefs.h"
#include "DataFormats/HLTReco/interface/TriggerFilterObjectWithRefs.h"

#include "DataFormats/Common/interface/Handle.h"

#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/EgammaReco/interface/SuperClusterFwd.h"
#include "DataFormats/EgammaReco/interface/PreshowerCluster.h"
#include "DataFormats/EgammaReco/interface/PreshowerClusterFwd.h"

#include "DataFormats/RecoCandidate/interface/RecoEcalCandidate.h"
#include "DataFormats/RecoCandidate/interface/RecoEcalCandidateFwd.h"

#include "DataFormats/EgammaCandidates/interface/Electron.h"
#include "DataFormats/EgammaCandidates/interface/ElectronFwd.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"

#include "DataFormats/TrackingRecHit/interface/TrackingRecHitFwd.h"

#include "DataFormats/SiStripDetId/interface/SiStripDetId.h"

#include "DataFormats/DetId/interface/DetIdCollection.h"

#include "Geometry/EcalMapping/interface/EcalElectronicsMapping.h"
#include "Geometry/EcalMapping/interface/EcalMappingRcd.h"

#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"

#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "Geometry/Records/interface/CaloGeometryRecord.h"

#include "Geometry/CaloEventSetup/interface/CaloTopologyRecord.h"

#include "Geometry/EcalAlgo/interface/EcalPreshowerGeometry.h"

#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "Geometry/TrackerGeometryBuilder/interface/PixelGeomDetUnit.h"

#include "CalibFormats/SiStripObjects/interface/SiStripRegionCabling.h"
#include "CalibFormats/SiStripObjects/interface/SiStripDetCabling.h"

#include "CalibTracker/Records/interface/SiStripDetCablingRcd.h"
#include "CalibTracker/Records/interface/SiStripRegionCablingRcd.h"

#include "FWCore/ParameterSet/interface/ConfigurationDescriptions.h"
#include "FWCore/ParameterSet/interface/ParameterSetDescription.h"

#include "FWCore/Utilities/interface/InputTag.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "FWCore/ServiceRegistry/interface/Service.h"


#include "CondFormats/SiStripObjects/interface/SiStripFedCabling.h"
#include "CondFormats/SiStripObjects/interface/FedChannelConnection.h"

#include "CondFormats/SiPixelObjects/interface/SiPixelFedCablingMap.h"
#include "CondFormats/SiPixelObjects/interface/SiPixelFedCabling.h"
#include "CondFormats/SiPixelObjects/interface/CablingPathToDetUnit.h"
#include "CondFormats/SiPixelObjects/interface/SiPixelFrameConverter.h"
#include "CondFormats/SiPixelObjects/interface/GlobalPixel.h"
#include "CondFormats/SiPixelObjects/interface/LocalPixel.h"
#include "CondFormats/SiPixelObjects/interface/ElectronicIndex.h"
#include "CondFormats/SiPixelObjects/interface/DetectorIndex.h"

#include "CondFormats/DataRecord/interface/SiPixelFedCablingMapRcd.h"

#include "DataFormats/Math/interface/normalizedPhi.h"

using namespace std;
class selectedElectronFEDListProducerv2 : public edm::EDProducer {

 public:

   explicit selectedElectronFEDListProducerv2( const edm::ParameterSet &);
   virtual ~selectedElectronFEDListProducerv2();

   class PixelRegion {
  
     public:
     
      PixelRegion(math::XYZVector & momentum, float dphi = 0.5, float deta = 0.5, float maxz = 24.0){
       vector = momentum;
       dPhi = dphi ;
       dEta = deta ;
       maxZ = maxz ; 
       cosphi = vector.x()/vector.rho();
       sinphi = vector.y()/vector.rho(); 
       atantheta = vector.z()/vector.rho();
      }

     math::XYZVector vector;
     float dPhi,dEta,maxZ;
     float cosphi, sinphi, atantheta;
   };   

 class PixelModule{

   public:

      PixelModule() {}
      PixelModule(float phi, float eta) : Phi(phi), Eta(eta), x(0.), y(0.), z(0.), DetId(0), Fed(0) {}

      bool operator < (const PixelModule& m) const {
      if(Phi < m.Phi) return true;
        if(Phi == m.Phi && Eta < m.Eta) return true;
        if(Phi == m.Phi && Eta == m.Eta && DetId < m.DetId) return true;
        return false;
      }

      float Phi,Eta;
      float x, y, z;
      unsigned int DetId;
      unsigned int Fed;

  };

 protected:

  virtual void beginJob() ;
  virtual void endJob() ;
  virtual void produce(edm::Event&, const edm::EventSetup&);

  virtual void pixelFedDump( std::vector<PixelModule>::const_iterator & itDn,  
                             std::vector<PixelModule>::const_iterator & itUp,
                             const PixelRegion & region);
 private:

  // input from edm::ParameterSet of the producer  
  std::vector<edm::InputTag> electronCollections_ ;
  std::vector<int> isGsfElectronCollection_ ;

  
  math::XYZVector beamSpotPosition_;

  edm::InputTag   beamSpotTag_ ;
  edm::FileInPath ESLookupTable_ ; 
  edm::InputTag rawDataLabel_ ;

  bool dumpEcalFedList_ ;
  bool dumpTrackSiStripFedList_ ;
  bool dumpTrackSiPixelFedList_ ;
  bool dumpAllEcalFed_ ;
  bool dumpAllTrackerFed_;
  bool dumpHCALFed_;

  bool debug_ ;

  double dRStripRegion_ ;
  double dPhiPixelRegion_;
  double dEtaPixelRegion_;
  double maxZPixelRegion_;

  uint32_t eventCounter_ ;

  std::string outputLabelModule_ ;

  // internal info of geometry of each sub-detector
  int ES_fedId_[2][2][40][40];

  const EcalElectronicsMapping* TheMapping_ ;
  const CaloGeometry* geometry_ ;
  const CaloSubdetectorGeometry *geometry__ES_ ;

  const SiStripRegionCabling* StripRegionCabling_;
  SiStripRegionCabling::Cabling cabling_ ;
  std::pair<double,double> regionDimension_ ;

  boost::scoped_ptr<SiPixelFedCabling> PixelCabling_;
  std::vector<PixelModule> pixelModuleVector_ ;
  
  // fed list and output raw data
  std::vector<uint32_t> fedList_ ;
  FEDRawDataCollection   *RawDataCollection_;
};

#endif
