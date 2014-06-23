/*
 * =====================================================================================
 *
 *       Filename:  EfficiencyExample.cc
 *
 *    Description:  Produce a tree for making efficiencies
 *
 * =====================================================================================
 */

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "DataFormats/Math/interface/deltaR.h"

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "DataFormats/PatCandidates/interface/Electron.h"

#include <TTree.h>

typedef std::vector<edm::InputTag> VInputTag;
typedef std::vector<unsigned int> PackedUIntCollection;

class EfficiencyExample : public edm::EDAnalyzer {
        public:
                EfficiencyExample(const edm::ParameterSet& pset);
                virtual ~EfficiencyExample(){}
                void analyze(const edm::Event& evt, const edm::EventSetup& es);
        private:
                VInputTag recoSrc_;
                VInputTag l1Src_;
                VInputTag genSrc_;
                edm::InputTag pvSrc_;
                double maxDR_;

                TTree* tree_;
                std::vector<Float_t>* ptsL1_;
                std::vector<Float_t>* etasL1_;
                std::vector<Float_t>* phisL1_;
                std::vector<Float_t>* ptsReco_;
                std::vector<Float_t>* etasReco_;
                std::vector<Float_t>* phisReco_;
                std::vector<Float_t>* ptsGen_;
                std::vector<Float_t>* etasGen_;
                std::vector<Float_t>* phisGen_;
                std::vector<Int_t>* isRecoL1matched_;
                std::vector<Int_t>* relIso80_;
                std::vector<Int_t>* relIso90_;
                std::vector<Int_t>* cIso80_;
                std::vector<Int_t>* cIso90_;

                UInt_t run_;
                UInt_t lumi_;
                ULong64_t event_;
                int nvtx_;

};

EfficiencyExample::EfficiencyExample(const edm::ParameterSet& pset){

        // input collections
        recoSrc_ = pset.getParameter<VInputTag>("recoSrc");
        l1Src_   = pset.getParameter<VInputTag>("l1Src");
        genSrc_  = pset.getParameter<VInputTag>("genSrc");
        pvSrc_ = pset.getParameter<edm::InputTag>("pvSrc");
        maxDR_ = pset.getParameter<double>("maxDR");

        // output tree   
        edm::Service<TFileService> fs;
        tree_ = fs->make<TTree>("Ntuple", "Ntuple");

        ptsReco_  = new std::vector<Float_t>();
        etasReco_ = new std::vector<Float_t>();
        phisReco_ = new std::vector<Float_t>();
        ptsL1_    = new std::vector<Float_t>();
        etasL1_   = new std::vector<Float_t>();
        phisL1_   = new std::vector<Float_t>();
        ptsGen_   = new std::vector<Float_t>();
        etasGen_  = new std::vector<Float_t>();
        phisGen_  = new std::vector<Float_t>();
        relIso80_ = new std::vector<Int_t>();
        relIso90_ = new std::vector<Int_t>();
        cIso80_   = new std::vector<Int_t>();
        cIso90_   = new std::vector<Int_t>();
        isRecoL1matched_ = new std::vector<Int_t>(); 

        tree_->Branch("ptReco", "std::vector<float>", &ptsReco_);
        tree_->Branch("etaReco", "std::vector<float>", &etasReco_);
        tree_->Branch("phiReco", "std::vector<float>", &phisReco_);
        tree_->Branch("ptL1", "std::vector<float>", &ptsL1_);
        tree_->Branch("etaL1", "std::vector<float>", &etasL1_);
        tree_->Branch("phiL1", "std::vector<float>", &phisL1_);
        tree_->Branch("ptsGen", "std::vector<float>", &ptsGen_);
        tree_->Branch("etasGen", "std::vector<float>", &etasGen_);
        tree_->Branch("phisGen", "std::vector<float>", &phisGen_);
        tree_->Branch("relIso80", "std::vector<int>", &relIso80_);
        tree_->Branch("relIso90", "std::vector<int>", &relIso90_);
        tree_->Branch("cIso80", "std::vector<int>", &cIso80_);
        tree_->Branch("cIso90", "std::vector<int>", &cIso90_);
        tree_->Branch("isRecoL1matched", "std::vector<int>", &isRecoL1matched_);
        tree_->Branch("run", &run_, "run/i");
        tree_->Branch("lumi", &lumi_, "lumi/i");
        tree_->Branch("evt", &event_, "evt/l");
        tree_->Branch("nvtx", &nvtx_, "nvtx/i");
}



namespace {

// Turn a set of InputTags into a colleciton of candidate pointers.
std::vector<const reco::Candidate*> getCollections(const edm::Event& evt, const VInputTag& collections) {
      
   std::vector<const reco::Candidate*> output;
   // Loop over collections
   for (size_t i = 0; i < collections.size(); ++i) {                  
        edm::Handle<edm::View<reco::Candidate> > handle;
        evt.getByLabel(collections[i], handle);
        // Loop over objects in current collection
        for (size_t j = 0; j < handle->size(); ++j) {
                 const reco::Candidate& object = handle->at(j);
                 output.push_back(&object);
        }
   }
   return output;
}

// Find the reco::Candidate in the [l1Collection] closes in DeltaR to
// [recoObject].  Only objects within [maxDR] are considered.  If no match
// is found, NULL is returned.
const reco::Candidate* findBestMatch(const reco::Candidate* recoObject,std::vector<const reco::Candidate*>& l1Collection, double maxDR) {

 const reco::Candidate* output = NULL;
 double bestDeltaR = -1;
 for (size_t i = 0; i < l1Collection.size(); ++i) {
      double deltaR = reco::deltaR(*recoObject, *l1Collection[i]);
      if (deltaR < maxDR) {
         if (!output || deltaR < bestDeltaR) {
            output = l1Collection[i];
            bestDeltaR = deltaR;
         }
       }
 }
 
 return output;
}

}

void EfficiencyExample::analyze(const edm::Event& evt, const edm::EventSetup& es) {

        // Get the RECO and regular L1 corrections
        std::vector<const reco::Candidate*> recoObjects = getCollections(evt,recoSrc_);
        std::vector<const reco::Candidate*> l1Objects   = getCollections(evt,l1Src_);
        std::vector<const reco::Candidate*> genObjects  = getCollections(evt,genSrc_);

        // Get PV collection
        // edm::Handle<reco::VertexCollection> vertices;
        // evt.getByLabel(pvSrc_, vertices);

        // general collections
        // nvtx_ = vertices->size();

        run_   = evt.id().run();
        lumi_  = evt.id().luminosityBlock();
        event_ = evt.id().event();

        // L1-Reco match
        ptsL1_->clear(); etasL1_->clear(); phisL1_->clear();
        ptsReco_->clear(); etasReco_->clear(); phisReco_->clear();
        ptsGen_->clear(); etasGen_->clear(); phisGen_->clear();
        relIso80_->clear(); relIso80_->clear(); cIso80_->clear(); cIso90_->clear();
        isRecoL1matched_->clear();
   
        for (size_t i = 0; i < genObjects.size(); ++i) {
	  const reco::Candidate* genObject = genObjects[i]; // loop on the generated electrons
          const reco::Candidate* bestL1    = findBestMatch(genObject,l1Objects,maxDR_);
          const reco::Candidate* bestReco  = findBestMatch(genObject,recoObjects,maxDR_);
	  const pat::Electron*  bestPat = dynamic_cast<const pat::Electron*>(bestReco);
         
          ptsGen_->push_back(genObject->pt()); 
          etasGen_->push_back(genObject->eta()); 
          phisGen_->push_back(genObject->phi());

          if(bestL1!=NULL and bestReco != NULL ){

           ptsL1_->push_back(bestL1->pt());
           etasL1_->push_back(bestL1->eta());
           phisL1_->push_back(bestL1->phi());

           ptsReco_ ->push_back(bestReco->pt());
           etasReco_->push_back(bestReco->eta());
           phisReco_->push_back(bestReco->phi());

           if(reco::deltaR(*bestL1, *bestReco) <= maxDR_) isRecoL1matched_->push_back(1);
           else isRecoL1matched_->push_back(0);

           relIso80_->push_back(bestPat->electronID("relIso80"));
           relIso90_->push_back(bestPat->electronID("relIso90"));
           cIso80_->push_back(bestPat->electronID("cIso80"));
           cIso90_->push_back(bestPat->electronID("cIso90"));

          }
	  else if(bestL1!=NULL and bestReco == NULL){

           ptsL1_->push_back(bestL1->pt());
           etasL1_->push_back(bestL1->eta());
           phisL1_->push_back(bestL1->phi());

           ptsReco_ ->push_back(-10);
           etasReco_->push_back(-10);
           phisReco_->push_back(-10);

	   relIso80_->push_back(-10);
           relIso90_->push_back(-10);
           cIso80_->push_back(-10);
           cIso90_->push_back(-10);

	   isRecoL1matched_->push_back(0);
       
          }
	  
          else if(bestL1 ==NULL and bestReco != NULL){

           ptsL1_->push_back(-10);
           etasL1_->push_back(-10);
           phisL1_->push_back(-10);

           ptsReco_ ->push_back(bestReco->pt());
           etasReco_->push_back(bestReco->eta());
           phisReco_->push_back(bestReco->phi());

           relIso80_->push_back(bestPat->electronID("relIso80"));
           relIso90_->push_back(bestPat->electronID("relIso90"));
           cIso80_->push_back(bestPat->electronID("cIso80"));
           cIso90_->push_back(bestPat->electronID("cIso90"));

	   isRecoL1matched_->push_back(0);
       
          }
          else {

           ptsL1_->push_back(-10);
           etasL1_->push_back(-10);
           phisL1_->push_back(-10);

           ptsReco_ ->push_back(-10);
           etasReco_->push_back(-10);
           phisReco_->push_back(-10);

	   relIso80_->push_back(-10);
           relIso90_->push_back(-10);
           cIso80_->push_back(-10);
           cIso90_->push_back(-10);

	   isRecoL1matched_->push_back(0);
	  }  

	}
        // fill!
        tree_->Fill();

}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(EfficiencyExample);
