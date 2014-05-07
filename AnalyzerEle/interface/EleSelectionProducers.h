#ifndef EleSelectionProducers_h
#define EleSelectionProducers_h

// -*- C++ -*-
//
// Package:    EleSelectionProducers
// Class:      EleSelectionProducers
// 
/**\class EleSelectionProducers EleSelectionProducers.cc Calibration/EleSelectionProducers/src/EleSelectionProducers.cc
*/
//
// Original Author:  Shervin Nourbakhsh,40 1-B24,+41227671643,
//         Created:  Tue Jul 17 20:57:01 CEST 2012
// $Id$
//
//

#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "PhysicsTools/SelectorUtils/interface/strbitset.h"

#include "DataFormats/Common/interface/ValueMap.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"

#include "StreamForElectrons/AnalyzerEle/interface/SimpleCutBasedElectronIDSelectionFunctor.h"

#include "DataFormats/RecoCandidate/interface/IsoDeposit.h"

//
// class declaration
//

class EleSelectionProducers : public edm::EDProducer {

  //  typedef pat::strbitset SelectionValue_t;
  typedef float SelectionValue_t;
  typedef edm::ValueMap<SelectionValue_t> SelectionMap;
  typedef std::vector< edm::Handle< edm::ValueMap<double> > > IsoDepositValsHandles_t;

public:
  explicit EleSelectionProducers(const edm::ParameterSet&);
  ~EleSelectionProducers();

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

private:
  virtual void beginJob() ;
  virtual void produce(edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
      
  virtual void beginRun(edm::Run&, edm::EventSetup const&);
  virtual void endRun(edm::Run&, edm::EventSetup const&);
  virtual void beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);
  virtual void endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);

  // ----------member data ---------------------------
private:
  edm::Handle<std::vector<reco::GsfElectron> > electronsHandle;
  edm::Handle<reco::ConversionCollection> conversionsHandle;
  edm::Handle<reco::BeamSpot> bsHandle;
  edm::Handle<reco::VertexCollection> vertexHandle;
  edm::Handle< edm::ValueMap<double> > chIsoValsHandle, emIsoValsHandle, nhIsoValsHandle;
  edm::Handle<double> rhoHandle;


  /// input tag for electrons
  edm::InputTag electronsTAG;
  // conversions
  edm::InputTag conversionsProducerTAG;
  edm::InputTag BeamSpotTAG;
  edm::InputTag VertexTAG;
  // isolation
  edm::InputTag chIsoValsTAG, emIsoValsTAG, nhIsoValsTAG;
  /// input rho
  edm::InputTag rhoTAG;

  
  SimpleCutBasedElectronIDSelectionFunctor fiducial_selector;
  SimpleCutBasedElectronIDSelectionFunctor relIso70_selector;
  SimpleCutBasedElectronIDSelectionFunctor relIso80_selector;
  SimpleCutBasedElectronIDSelectionFunctor relIso90_selector;
  SimpleCutBasedElectronIDSelectionFunctor cIso70_selector;
  SimpleCutBasedElectronIDSelectionFunctor cIso80_selector;
  SimpleCutBasedElectronIDSelectionFunctor cIso90_selector;
  
};

#endif
