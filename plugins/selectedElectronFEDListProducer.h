#ifndef HLTTigger_Egamma_selectedElectronFEDListProducer_h
#define HLTTigger_Egamma_selectedElectronFEDListProducer_h

#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <fstream>
#include "TLorentzVector.h"
#include "TVector3.h"

#include "DataFormats/FEDRawData/interface/FEDRawData.h"
#include "DataFormats/FEDRawData/interface/FEDRawDataCollection.h"
#include "DataFormats/FEDRawData/interface/FEDHeader.h"
#include "DataFormats/FEDRawData/interface/FEDTrailer.h"
#include "DataFormats/SiStripDigi/interface/SiStripDigi.h"
#include "DataFormats/HLTReco/interface/TriggerFilterObjectWithRefs.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include "DataFormats/SiStripCommon/interface/SiStripConstants.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/HLTReco/interface/TriggerFilterObjectWithRefs.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/EgammaReco/interface/SuperClusterFwd.h"
#include "DataFormats/RecoCandidate/interface/RecoEcalCandidate.h"
#include "DataFormats/RecoCandidate/interface/RecoEcalCandidateFwd.h"
#include "DataFormats/EgammaCandidates/interface/Electron.h"
#include "DataFormats/EgammaCandidates/interface/ElectronFwd.h"
#include "DataFormats/FEDRawData/interface/FEDNumbering.h"
#include "DataFormats/EgammaReco/interface/PreshowerCluster.h"
#include "DataFormats/EgammaReco/interface/PreshowerClusterFwd.h"
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

#include "CalibFormats/SiStripObjects/interface/SiStripRegionCabling.h"
#include "CalibTracker/Records/interface/SiStripDetCablingRcd.h"
#include "CalibFormats/SiStripObjects/interface/SiStripDetCabling.h"
#include "CalibTracker/Records/interface/SiStripRegionCablingRcd.h"
#include "CalibFormats/SiStripObjects/interface/SiStripFecCabling.h"
#include "CalibFormats/SiStripObjects/interface/SiStripFec.h"

#include "FWCore/ParameterSet/interface/ConfigurationDescriptions.h"
#include "FWCore/ParameterSet/interface/ParameterSetDescription.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "CondFormats/SiStripObjects/interface/SiStripFedCabling.h"
#include "CondFormats/SiPixelObjects/interface/SiPixelFedCabling.h"
#include "CondFormats/SiPixelObjects/interface/CablingPathToDetUnit.h"
#include "CondFormats/SiPixelObjects/interface/SiPixelFrameConverter.h"
#include "CondFormats/SiPixelObjects/interface/GlobalPixel.h"
#include "CondFormats/SiPixelObjects/interface/LocalPixel.h"
#include "CondFormats/SiPixelObjects/interface/ElectronicIndex.h"
#include "CondFormats/SiPixelObjects/interface/DetectorIndex.h"
#include "CondFormats/SiStripObjects/interface/FedChannelConnection.h"
#include "CondFormats/SiPixelObjects/interface/SiPixelFrameReverter.h"
#include "CondFormats/SiPixelObjects/interface/SiPixelFedCabling.h"
#include "CondFormats/DataRecord/interface/SiPixelFedCablingMapRcd.h"
#include "FWCore/Framework/interface/ESTransientHandle.h"

using namespace std;
class selectedElectronFEDListProducer : public edm::EDProducer {

 public:

   explicit selectedElectronFEDListProducer( const edm::ParameterSet &);
   virtual ~selectedElectronFEDListProducer();
   
 protected:

  virtual void beginJob(const edm::EventSetup &) ;
  virtual void endJob() ;
  virtual void produce(edm::Event&, const edm::EventSetup&);

 private:
  
  std::vector<edm::InputTag> electronCollections_ ;
  edm::FileInPath ESLookupTable_ ; 
  edm::InputTag rawDataLabel_ ;

  std::map<uint32_t,uint32_t> stripFedDetIdMap_ ;

  bool dumpEcalFedList_ ;
  bool dumpTrackSiStripFedList_ ;
  bool dumpTrackSiPixelFedList_ ;
  std::string outputLabelModule_ ;

  // fed id for pre-shower
  int ES_fedId_[2][2][40][40];

  std::vector<uint32_t> fedList_ ;
  FEDRawDataCollection   *RawDataCollection_;

  const EcalElectronicsMapping* TheMapping_ ;
  const CaloGeometry* geometry_ ;
  const CaloSubdetectorGeometry *geometry__ES_ ;
  const SiStripDetCabling* stripReverter_;
  const SiStripFedCabling* stripFedCabling_ ;
  SiPixelFrameReverter* frameReverter_ ;
  const SiStripFecCabling *stripFecCabling_;

  bool debug_ ;
  uint32_t eventCounter_ ;
};

#endif