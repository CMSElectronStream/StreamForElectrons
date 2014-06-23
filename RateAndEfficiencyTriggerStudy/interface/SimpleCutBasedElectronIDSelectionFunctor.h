#ifndef SimpleCutBasedElectronIDSelectionFunctor_h
#define SimpleCutBasedElectronIDSelectionFunctor_h

#include <memory>
#include <iostream>

#include <TTree.h>
#include <TLorentzVector.h>
#include <TString.h>
#include <TFile.h>


#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"


#include "PhysicsTools/SelectorUtils/interface/Selector.h"

#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrackFwd.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHit.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/EcalDetId/interface/EcalSubdetector.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"

#include "EgammaAnalysis/ElectronTools/interface/ElectronEffectiveArea.h"

#include "RecoEcal/EgammaCoreTools/interface/EcalClusterTools.h"
#include "RecoEcal/EgammaCoreTools/interface/EcalClusterLazyTools.h"


#include "Geometry/CaloTopology/interface/CaloTopology.h"
#include "Geometry/CaloEventSetup/interface/CaloTopologyRecord.h"

#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

#include "DataFormats/EgammaCandidates/interface/ConversionFwd.h"
#include "RecoEgamma/EgammaTools/interface/ConversionFinder.h"
#include "RecoEgamma/EgammaTools/interface/ConversionInfo.h"
#include "RecoEgamma/EgammaTools/interface/ConversionTools.h"

#include "EgammaAnalysis/ElectronTools/interface/EGammaCutBasedEleId.h"


class SimpleCutBasedElectronIDSelectionFunctor : public Selector<reco::GsfElectronRef>  {

 public: // interface  
  
  enum Version_t { NONE=0, fiducial,  relIso90, relIso80, relIso70, cIso90,  cIso80, cIso70};
  
  SimpleCutBasedElectronIDSelectionFunctor(edm::ParameterSet const & parameters);

  ~SimpleCutBasedElectronIDSelectionFunctor();

  // initialize it by using only the version name
  
  SimpleCutBasedElectronIDSelectionFunctor(Version_t  version, 
					   const edm::Handle<reco::ConversionCollection>& ConversionsHandle, 
					   const edm::Handle<reco::BeamSpot>& BeamSpotHandle, 
					   const edm::Handle<double>& rhoHandle,
					   const edm::Handle< edm::ValueMap<double> > &isoVals
					   );
					   
  SimpleCutBasedElectronIDSelectionFunctor(TString versionStr, 
					   const edm::Handle<reco::GsfElectronCollection>& electronsHandle,
					   const edm::Handle<reco::ConversionCollection>& ConversionsHandle, 
					   const edm::Handle<reco::BeamSpot>& BeamSpotHandle, 
					   const edm::Handle<reco::VertexCollection>& VertexHandle,
					   const edm::Handle< edm::ValueMap<double> >& chIsoValsHandle,
					   const edm::Handle< edm::ValueMap<double> >& emIsoValsHandle,
					   const edm::Handle< edm::ValueMap<double> >& nhIsoValsHandle,
					   const edm::Handle<double>& rhoHandle);
					   
  void initialize( Version_t version );

  void initialize(Double_t trackIso_EB, Double_t ecalIso_EB, Double_t hcalIso_EB,
		  Double_t sihih_EB, Double_t  dphi_EB, Double_t deta_EB, Double_t hoe_EB,
		  Double_t cIso_EB,
		  Double_t trackIso_EE, Double_t ecalIso_EE, Double_t hcalIso_EE,
		  Double_t sihih_EE, Double_t  dphi_EE, Double_t deta_EE, Double_t hoe_EE,
		  Double_t cIso_EE, Int_t conversionRejection, 
		  Int_t maxNumberOfExpectedMissingHits);
  
  bool operator()( const reco::GsfElectronRef& electron, pat::strbitset& ret);
  
  float result();

  using Selector<reco::GsfElectronRef>::operator();

  bool WPxx_PU( const reco::GsfElectronRef electronRef, pat::strbitset& ret);
 
  int bitMask();

 private: // member variables
  // version of the cuts  
  Version_t version_;
  const edm::Handle<reco::GsfElectronCollection>& electronsHandle_;
  const edm::Handle<reco::ConversionCollection>& ConversionsHandle_;
  const edm::Handle<reco::BeamSpot>& BeamSpotHandle_;
  const edm::Handle<reco::VertexCollection>& VertexHandle_;
  const edm::Handle< edm::ValueMap<double> >& chIsoValsHandle_;
  const edm::Handle< edm::ValueMap<double> >& emIsoValsHandle_;
  const edm::Handle< edm::ValueMap<double> >& nhIsoValsHandle_;
  const edm::Handle< edm::ValueMap<double> >& IsoDeposit_ ; 
  const edm::Handle<double>& rhoHandle_;

};


#endif
