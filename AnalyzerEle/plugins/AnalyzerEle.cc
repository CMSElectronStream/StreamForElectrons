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

//
// class declaration
//

//
// ---- similar to
//      https://github.com/Bicocca/UserCode/blob/master/Calibration/EcalCalibNtuple/plugins/SimpleNtupleEoverP.cc
//

class AnalyzerEle : public edm::EDAnalyzer {
   public:
      explicit AnalyzerEle(const edm::ParameterSet&);
      ~AnalyzerEle();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

      //virtual void beginRun(edm::Run const&, edm::EventSetup const&) override;
      //virtual void endRun(edm::Run const&, edm::EventSetup const&) override;
      //virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;
      //virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;

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
      edm::InputTag MCtruthTag_;
      edm::InputTag MCPileupTag_;

      int eventType_;
      std::string jsonFileName_;
      std::string dataRun_;



      std::map<int, std::vector<std::pair<int, int> > > jsonMap_;
      // event variables
      long int bxId;
      long int eventId;
      int lumiId;
      int runId;
      int timeStampHigh;
      int isW;
      int isZ;

      // output
      TTree* myTree_;
      int nele_;
      float pt_;
      float eta_;
      float phi_;

};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
AnalyzerEle::AnalyzerEle(const edm::ParameterSet& iConfig) {
 //now do what ever initialization is needed
 eventType_ = iConfig.getUntrackedParameter<int>("eventType",1);
 dataRun_ = iConfig.getParameter<std::string>("dataRun");
 jsonFileName_ = iConfig.getParameter<std::string>("jsonFileName");
 digiCollection_EB_ = iConfig.getParameter<edm::InputTag>("digiCollection_EB");
 digiCollection_EE_ = iConfig.getParameter<edm::InputTag>("digiCollection_EE");
 recHitCollection_EB_ = iConfig.getParameter<edm::InputTag>("recHitCollection_EB");
 recHitCollection_EE_ = iConfig.getParameter<edm::InputTag>("recHitCollection_EE");
 BSTag_ = iConfig.getParameter<edm::InputTag>("theBeamSpotTag");
 SRFlagCollection_EB_ = iConfig.getParameter<edm::InputTag>("SRFlagCollection_EB");
 SRFlagCollection_EE_ = iConfig.getParameter<edm::InputTag>("SRFlagCollection_EE");
 MCPileupTag_ = iConfig.getParameter<edm::InputTag>("MCPileupTag");
 MCtruthTag_ = iConfig.getParameter<edm::InputTag>("MCtruthTag");
 PVTag_ = iConfig.getParameter<edm::InputTag>("PVTag");
 rhoTag_ = iConfig.getParameter<edm::InputTag>("rhoTag");
 EleTag_ = iConfig.getParameter<edm::InputTag>("EleTag");
 PFMetTag_ = iConfig.getParameter<edm::InputTag>("PFMetTag");
 conversionsInputTag_ = iConfig.getParameter<edm::InputTag>("conversionsInputTag");


 
 

 edm::Service<TFileService> fs ;
 myTree_ = fs -> make <TTree>("myTree","myTree");

 myTree_ -> Branch("nele", &nele_, "nele/I");
 myTree_ -> Branch("pt", &pt_, "pt/F");
 myTree_ -> Branch("eta", &eta_, "eta/F");
 myTree_ -> Branch("phi", &phi_, "phi/F");

}


AnalyzerEle::~AnalyzerEle()
{

   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
AnalyzerEle::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

   edm::Handle<reco::GsfElectronCollection> gsfElectrons;
   iEvent.getByLabel(EleTag_,gsfElectrons);

   nele_ = (*gsfElectrons).size();

   for (reco::GsfElectronCollection::const_iterator gsfIter=gsfElectrons->begin(); gsfIter!=gsfElectrons->end(); gsfIter++){
    phi_ =  gsfIter->phi();
    eta_ =  gsfIter->eta();
    pt_ =  gsfIter->pt();
    myTree_->Fill();
   }

}


// ------------ method called once each job just before starting event loop  ------------
void 
AnalyzerEle::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
AnalyzerEle::endJob() 
{
}

// ------------ method called when starting to processes a run  ------------
/*
void 
AnalyzerEle::beginRun(edm::Run const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when ending the processing of a run  ------------
/*
void 
AnalyzerEle::endRun(edm::Run const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when starting to processes a luminosity block  ------------
/*
void 
AnalyzerEle::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when ending the processing of a luminosity block  ------------
/*
void 
AnalyzerEle::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
AnalyzerEle::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(AnalyzerEle);
