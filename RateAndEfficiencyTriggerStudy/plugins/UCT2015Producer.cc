//
// Package:    UCT2015Producer
//
// Original Author:  Sridhara Rao Dasu
//         Created:  Thu Jun  7 13:29:52 CDT 2012
//


// system include files
#include <memory>
#include <math.h>
#include <vector>
#include <list>
#include <TTree.h>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/L1CaloTrigger/interface/L1CaloCollections.h"
#include "DataFormats/L1CaloTrigger/interface/L1CaloRegion.h"
#include "DataFormats/L1CaloTrigger/interface/L1CaloEmCand.h"
#include "DataFormats/L1CaloTrigger/interface/L1CaloRegionDetId.h"

#include "L1Trigger/UCT2015/interface/UCTCandidate.h"
#include "L1Trigger/UCT2015/interface/helpers.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "L1Trigger/RegionalCaloTrigger/interface/L1RCT.h"

using namespace std;
using namespace edm;


class UCT2015Producer : public edm::EDProducer {
        public:

                static const unsigned N_JET_PHI;
                static const unsigned N_JET_ETA;

                // Concrete collection of L1Gobjects (with extra tuning information)
                typedef vector<UCTCandidate> UCTCandidateCollection;
                typedef std::auto_ptr<UCTCandidateCollection> UCTCandidateCollectionPtr;

                explicit UCT2015Producer(const edm::ParameterSet&);

        private:
                virtual void produce(edm::Event&, const edm::EventSetup&);

                double egPhysicalEt(const L1CaloEmCand& cand) const {
                        return egLSB_*cand.rank();
                }

                double regionPhysicalEt(const L1CaloRegion& cand) const {
                        return std::max(0.,regionLSB_*cand.et());
                }

                // Find information about observables in the annulus.  We define the annulus
                // as all regions around the central region, with the exception of the second
                // highest in ET, as this could be sharing the 2x1.
                // MIPS in annulus refers to number of regions in the annulus which have
                // their MIP bit set.
                // egFlags is the number where (!tauVeto && !mip)
                void findAnnulusInfo(int ieta, int iphi,
                                const L1CaloRegionCollection& regions,
                                double* associatedSecondRegionEt,
                                double* associatedThirdRegionEt,
                                unsigned int* mipsInAnnulus,
                                unsigned int* egFlagsInAnnulus,
                                unsigned int* mipInSecondRegion) const;

                // Helper methods

                void puSubtraction();
                void puMultSubtraction();

                void makeSums();
                void makeJets();
                void makeEGTaus();
                void makeTaus();

                list<UCTCandidate> correctJets(const list<UCTCandidate>&, bool isJet);

                // ----------member data ---------------------------
                bool puCorrectHI;
                bool applyJetCalibration;
                bool puMultCorrect;
                //bool puCorrectHISums;
                bool useUICrho; // which PU denstity to use for energy correction determination
                bool useHI; // do HI-style background subtraction

                unsigned int puETMax;
                unsigned int puLevelHI;
                unsigned int puLevelPUM0;
                //double puLevelHIUIC; // puLevelHI divided by puCount*Area, not multiply by 9.0
                unsigned int  puLevelHIUIC; // puLevelHI divided by puCount*Area, not multiply by 9.0
                vector<int> puLevelHIHI;

                unsigned int sumET;
                int sumEx;
                int sumEy;
                unsigned int MET;

                unsigned int regionETCutForHT;
                unsigned int regionETCutForMET;
                unsigned int minGctEtaForSums;
                unsigned int maxGctEtaForSums;
                unsigned int sumHT;
                int sumHx;
                int sumHy;
                unsigned int MHT;

                unsigned int sumExtraET;
                unsigned int extraMET;
                unsigned int sumExtraHT;
                unsigned int extraMHT;

                UCTCandidate METObject;
                UCTCandidate MHTObject;
                UCTCandidate SETObject;
                UCTCandidate SHTObject;

                unsigned int jetSeed;
                list<UCTCandidate> jetList, corrJetList;

                unsigned int egtSeed;
                unsigned int tauSeed;

                double relativeTauIsolationCut;
                double relativeJetIsolationCut;
                double switchOffTauIso;
                list<UCTCandidate> rlxTauList, corrRlxTauList;
                list<UCTCandidate> rlxEGList;
                list<UCTCandidate> isoTauList, corrIsoTauList;
                list<UCTCandidate> isoEGList;
                list<UCTCandidate> rlxTauRegionOnlyList, isoTauRegionOnlyList;

                Handle<L1CaloRegionCollection> newRegions;
                Handle<L1CaloEmCollection> newEMCands;

                vector<double> sinPhi;
                vector<double> cosPhi;

                double egLSB_;
                double regionLSB_;

                vector<double> m_jetSF;

};

unsigned const UCT2015Producer::N_JET_PHI = L1CaloRegionDetId::N_PHI * 4;
unsigned const UCT2015Producer::N_JET_ETA = L1CaloRegionDetId::N_ETA * 4;

//
// constructors and destructor
//
UCT2015Producer::UCT2015Producer(const edm::ParameterSet& iConfig) :
        puCorrectHI(iConfig.getParameter<bool>("puCorrectHI")), 
        applyJetCalibration(iConfig.getParameter<bool>("applyJetCalibration")),
        puMultCorrect(iConfig.getParameter<bool>("puMultCorrect")),
        useUICrho(iConfig.getParameter<bool>("useUICrho")),
        useHI(iConfig.getParameter<bool>("useHI")),
        puETMax(iConfig.getParameter<unsigned int>("puETMax")),
        regionETCutForHT(iConfig.getParameter<unsigned int>("regionETCutForHT")),
        regionETCutForMET(iConfig.getParameter<unsigned int>("regionETCutForMET")),
        minGctEtaForSums(iConfig.getParameter<unsigned int>("minGctEtaForSums")),
        maxGctEtaForSums(iConfig.getParameter<unsigned int>("maxGctEtaForSums")),
        jetSeed(iConfig.getParameter<unsigned int>("jetSeed")),
        egtSeed(iConfig.getParameter<unsigned int>("egtSeed")),
        tauSeed(iConfig.getParameter<unsigned int>("tauSeed")),

        relativeTauIsolationCut(iConfig.getParameter<double>("relativeTauIsolationCut")),
        relativeJetIsolationCut(iConfig.getParameter<double>("relativeJetIsolationCut")),
        switchOffTauIso(iConfig.getParameter<double>("switchOffTauIso")),
        egLSB_(iConfig.getParameter<double>("egammaLSB")),
        regionLSB_(iConfig.getParameter<double>("regionLSB"))
{
        m_jetSF=iConfig.getParameter<vector<double> >("jetSF");

        puLevelHI = 0;
        puLevelHIUIC = 0.0;
        puLevelHIHI.resize(L1CaloRegionDetId::N_ETA);
        for(unsigned i = 0; i < L1CaloRegionDetId::N_ETA; ++i)
                puLevelHIHI[i] = 0;

        // Also declare we produce unpacked collections (which have more info)
        produces<UCTCandidateCollection>( "JetUnpacked" ) ;
        produces<UCTCandidateCollection>( "CorrJetUnpacked" ) ;
        produces<UCTCandidateCollection>( "RelaxedEGUnpacked" ) ;
        produces<UCTCandidateCollection>( "IsolatedEGUnpacked" ) ;
        produces<UCTCandidateCollection>( "RelaxedTauUnpacked" ) ;
        produces<UCTCandidateCollection>( "IsolatedTauUnpacked" ) ;
        produces<UCTCandidateCollection>( "CorrRelaxedTauUnpacked" ) ;
        produces<UCTCandidateCollection>( "CorrIsolatedTauUnpacked" ) ;
        produces<UCTCandidateCollection>( "RelaxedTauEcalSeedUnpacked" ) ;
        produces<UCTCandidateCollection>( "IsolatedTauEcalSeedUnpacked" ) ;
        produces<UCTCandidateCollection>( "PULevelPUM0Unpacked" ) ;
        produces<UCTCandidateCollection>( "PULevelUnpacked" ) ;
        produces<UCTCandidateCollection>( "PULevelUICUnpacked" ) ;
        produces<UCTCandidateCollection>( "METUnpacked" ) ;
        produces<UCTCandidateCollection>( "MHTUnpacked" ) ;
        produces<UCTCandidateCollection>( "SETUnpacked" ) ;
        produces<UCTCandidateCollection>( "SHTUnpacked" ) ;

        //now do what ever initialization is needed
        for(unsigned int i = 0; i < L1CaloRegionDetId::N_PHI; i++) {
                sinPhi.push_back(sin(2. * 3.1415927 * i * 1.0 / L1CaloRegionDetId::N_PHI));
                cosPhi.push_back(cos(2. * 3.1415927 * i * 1.0 / L1CaloRegionDetId::N_PHI));
        }
}


// For the single objects, like MET/MHT, etc, convert them into a
// std::auto_ptr<UCTCandidateCollection> suitable for putting into the edm::Event
// The "collection" contains only 1 object.
UCT2015Producer::UCTCandidateCollectionPtr collectionize(const UCTCandidate& obj) {
        return UCT2015Producer::UCTCandidateCollectionPtr(
                        new UCT2015Producer::UCTCandidateCollection(1, obj));
}

// ------------ method called for each event  ------------
        void
UCT2015Producer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{

        puLevelPUM0=-1;
        if(puMultCorrect) {
                edm::Handle<int> puweightHandle;
                iEvent.getByLabel("CorrectedDigis","CorrectedRegions", newRegions);
                iEvent.getByLabel("CorrectedDigis","PUM0Level",puweightHandle);
                puLevelPUM0=(*puweightHandle);
        }
        else {iEvent.getByLabel("uctDigis", newRegions);}
        iEvent.getByLabel("uctDigis", newEMCands);

        if(puCorrectHI) puSubtraction();

        makeSums();
        makeJets();
        //corrected Jet and Tau collections
        corrJetList = correctJets(jetList,true);
        // electrons and taus 
        makeEGTaus();
        makeTaus();
        // nobody uses these
        //corrRlxTauList = correctJets(rlxTauList,false);
        //corrIsoTauList = correctJets(isoTauList,false);


        UCTCandidateCollectionPtr unpackedJets(new UCTCandidateCollection);
        UCTCandidateCollectionPtr unpackedRlxTaus(new UCTCandidateCollection);
        UCTCandidateCollectionPtr unpackedIsoTaus(new UCTCandidateCollection);
        UCTCandidateCollectionPtr unpackedCorrJets(new UCTCandidateCollection);
        UCTCandidateCollectionPtr unpackedCorrRlxTaus(new UCTCandidateCollection);
        UCTCandidateCollectionPtr unpackedCorrIsoTaus(new UCTCandidateCollection);
        UCTCandidateCollectionPtr unpackedRlxEGs(new UCTCandidateCollection);
        UCTCandidateCollectionPtr unpackedIsoEGs(new UCTCandidateCollection);
        UCTCandidateCollectionPtr unpackedRlxTauRegionOnlys(new UCTCandidateCollection);
        UCTCandidateCollectionPtr unpackedIsoTauRegionOnlys(new UCTCandidateCollection);

        //uncorrected Jet and Tau collections
        for(list<UCTCandidate>::iterator jet = jetList.begin();
                        jet != jetList.end();
                        jet++) {
                unpackedJets->push_back(*jet);
        }
        for(list<UCTCandidate>::iterator rlxTau = rlxTauList.begin();
                        rlxTau != rlxTauList.end();
                        rlxTau++) {
                unpackedRlxTaus->push_back(*rlxTau);
        }
        for(list<UCTCandidate>::iterator isoTau = isoTauList.begin();
                        isoTau != isoTauList.end();
                        isoTau++) {
                unpackedIsoTaus->push_back(*isoTau);
        }
        for(list<UCTCandidate>::iterator jet = corrJetList.begin();
                        jet != corrJetList.end();
                        jet++) {
                unpackedCorrJets->push_back(*jet);
        }
        /*
        for(list<UCTCandidate>::iterator rlxTau = corrRlxTauList.begin();
                        rlxTau != corrRlxTauList.end();
                        rlxTau++) {
                unpackedCorrRlxTaus->push_back(*rlxTau);
        }
        for(list<UCTCandidate>::iterator isoTau = corrIsoTauList.begin();
                        isoTau != corrIsoTauList.end();
                        isoTau++) {
                unpackedCorrIsoTaus->push_back(*isoTau);
        }
        */
        for(list<UCTCandidate>::iterator rlxTauRegionOnly = rlxTauRegionOnlyList.begin();
                        rlxTauRegionOnly != rlxTauRegionOnlyList.end();
                        rlxTauRegionOnly++) {
                unpackedRlxTauRegionOnlys->push_back(*rlxTauRegionOnly);
        }
        for(list<UCTCandidate>::iterator isoTauRegionOnly = isoTauRegionOnlyList.begin();
                        isoTauRegionOnly != isoTauRegionOnlyList.end();
                        isoTauRegionOnly++) {
                unpackedIsoTauRegionOnlys->push_back(*isoTauRegionOnly);
        }

        // egamma collections
        for(list<UCTCandidate>::iterator rlxEG = rlxEGList.begin();
                        rlxEG != rlxEGList.end();
                        rlxEG++) {
                unpackedRlxEGs->push_back(*rlxEG);
        }
        for(list<UCTCandidate>::iterator isoEG = isoEGList.begin();
                        isoEG != isoEGList.end();
                        isoEG++) {
                unpackedIsoEGs->push_back(*isoEG);
        }

        // Just store these as cands to make life easier.
        UCTCandidate puLevelHIAsCand(puLevelHI, 0, 0);
        UCTCandidate puLevelHIUICAsCand(puLevelHI, 0, 0);
        UCTCandidate puLevelPUM0AsCand(puLevelPUM0, 0, 0);


        iEvent.put(collectionize(puLevelPUM0AsCand), "PULevelPUM0Unpacked");
        iEvent.put(collectionize(puLevelHIAsCand), "PULevelUnpacked");
        iEvent.put(collectionize(puLevelHIUICAsCand), "PULevelUICUnpacked");
        iEvent.put(collectionize(METObject), "METUnpacked");
        iEvent.put(collectionize(MHTObject), "MHTUnpacked");
        iEvent.put(collectionize(SETObject), "SETUnpacked");
        iEvent.put(collectionize(SHTObject), "SHTUnpacked");

        iEvent.put(unpackedJets, "JetUnpacked");
        iEvent.put(unpackedRlxTaus, "RelaxedTauEcalSeedUnpacked");
        iEvent.put(unpackedIsoTaus, "IsolatedTauEcalSeedUnpacked");
        iEvent.put(unpackedCorrJets, "CorrJetUnpacked");
        iEvent.put(unpackedCorrRlxTaus, "CorrRelaxedTauUnpacked");
        iEvent.put(unpackedCorrIsoTaus, "CorrIsolatedTauUnpacked");
        iEvent.put(unpackedRlxEGs, "RelaxedEGUnpacked");
        iEvent.put(unpackedIsoEGs, "IsolatedEGUnpacked");
        iEvent.put(unpackedRlxTauRegionOnlys, "RelaxedTauUnpacked");
        iEvent.put(unpackedIsoTauRegionOnlys, "IsolatedTauUnpacked");
}

// NB PU is not in the physical scale!!  Needs to be multiplied by regionLSB
/*void UCT2015Producer::puMultSubtraction()
  {
  puMult = 0;
  for(L1CaloRegionCollection::const_iterator newRegion =
  newRegions->begin();
  newRegion != newRegions->end(); newRegion++){
  double regionET =  regionPhysicalEt(*newRegion);
// cout << "regionET: " << regionET <<endl; 
if (regionET > 0) {puMult++;}
}

for(L1CaloRegionCollection::const_iterator newRegion =
newRegions->begin();
newRegion != newRegions->end(); newRegion++){
double regionET =  regionPhysicalEt(*newRegion);
// cout << "regionET: " << regionET <<endl; 
//the divide by regionLSB to get back to gct Digis
double regionEtCorr = (pumcorr(regionET, newRegion->gctEta(),puMult))/regionLSB_;
//		newRegion->et()=region;    
}


}
*/

void UCT2015Producer::puSubtraction()
{
        puLevelHI = 0;
        puLevelHIUIC = 0;
        double r_puLevelHIUIC=0.0;
        double r_puLevelHIHI[L1CaloRegionDetId::N_ETA];

        int etaCount[L1CaloRegionDetId::N_ETA];
        for(unsigned i = 0; i < L1CaloRegionDetId::N_ETA; ++i)
        {
                puLevelHIHI[i] = 0;
                r_puLevelHIHI[i] = 0.0;
                etaCount[i] = 0;
        }

        int puCount = 0;
        double Rarea=0.0;
        for(L1CaloRegionCollection::const_iterator newRegion =
                        newRegions->begin();
                        newRegion != newRegions->end(); newRegion++){
                if(regionPhysicalEt(*newRegion) <= puETMax) {
                        puLevelHI += newRegion->et(); puCount++;
                        r_puLevelHIUIC += newRegion->et();
                        Rarea += getRegionArea(newRegion->gctEta());
                }
                r_puLevelHIHI[newRegion->gctEta()] += newRegion->et();
                etaCount[newRegion->gctEta()]++;
                // cout << "regionET: " << regionET <<endl; 

        } //end regionforloop
        // Add a factor of 9, so it corresponds to a jet.  Reduces roundoff error.
        puLevelHI *= 9;
        if(puCount != 0) puLevelHI = puLevelHI / puCount;
        r_puLevelHIUIC = r_puLevelHIUIC / Rarea;
        puLevelHIUIC=0;
        if (r_puLevelHIUIC > 0.) puLevelHIUIC = floor (r_puLevelHIUIC + 0.5);

        for(unsigned i = 0; i < L1CaloRegionDetId::N_ETA; ++i)
        {
                puLevelHIHI[i] = floor(r_puLevelHIHI[i]/etaCount[i] + 0.5);
        }
}

void UCT2015Producer::makeSums()
{
        sumET = 0;
        sumEx = 0;
        sumEy = 0;
        sumHT = 0;
        sumHx = 0;
        sumHy = 0;

        for(L1CaloRegionCollection::const_iterator newRegion = newRegions->begin();
                        newRegion != newRegions->end(); newRegion++){
                // Remove forward stuff
                if (newRegion->gctEta() < minGctEtaForSums || newRegion->gctEta() > maxGctEtaForSums) {
                        continue;
                }

                double regionET =  regionPhysicalEt(*newRegion);     

                /*
                if(puCorrectHISums)    {
                        regionET = std::max(regionPhysicalEt(*newRegion) - puLevelHI*regionLSB_/9., 0.);
                }
                */
 
                if(regionET >= regionETCutForMET){
                        sumET += regionET;
                        sumEx += (int) (((double) regionET) * cosPhi[newRegion->gctPhi()]);
                        sumEy += (int) (((double) regionET) * sinPhi[newRegion->gctPhi()]);
                }
                if(regionET >= regionETCutForHT) {
                        sumHT += regionET;
                        sumHx += (int) (((double) regionET) * cosPhi[newRegion->gctPhi()]);
                        sumHy += (int) (((double) regionET) * sinPhi[newRegion->gctPhi()]);
                }
        }
        MET = ((unsigned int) sqrt(sumEx * sumEx + sumEy * sumEy));
        MHT = ((unsigned int) sqrt(sumHx * sumHx + sumHy * sumHy));

        double physicalPhi = atan2(sumEy, sumEx) + 3.1415927;
        unsigned int iPhi = L1CaloRegionDetId::N_PHI * physicalPhi / (2 * 3.1415927);
        METObject = UCTCandidate(MET, 0, physicalPhi);
        METObject.setInt("rgnPhi", iPhi);
        METObject.setInt("rank", MET);

        double physicalPhiHT = atan2(sumHy, sumHx) + 3.1415927;
        iPhi = L1CaloRegionDetId::N_PHI * (physicalPhiHT) / (2 * 3.1415927);
        MHTObject = UCTCandidate(MHT, 0, physicalPhiHT);
        MHTObject.setInt("rgnPhi", iPhi);
        MHTObject.setInt("rank", MHT);

        SETObject = UCTCandidate(sumET, 0, 0);
        SETObject.setInt("rank", sumET);

        SHTObject = UCTCandidate(sumHT, 0, 0);
        SHTObject.setInt("rank", sumHT);

}

void UCT2015Producer::makeJets() {
        jetList.clear();
        for(L1CaloRegionCollection::const_iterator newRegion = newRegions->begin();
                        newRegion != newRegions->end(); newRegion++) {
                double regionET = regionPhysicalEt(*newRegion);
                if(puCorrectHI && useHI)
                        regionET = std::max(0.,regionET -
                                        (puLevelHIHI[newRegion->gctEta()]*regionLSB_));
                if((regionET > jetSeed) || (puCorrectHI && useHI)) {
                        double neighborN_et = 0;
                        double neighborS_et = 0;
                        double neighborE_et = 0;
                        double neighborW_et = 0;
                        double neighborNE_et = 0;
                        double neighborSW_et = 0;
                        double neighborNW_et = 0;
                        double neighborSE_et = 0;
                        unsigned int nNeighbors = 0;
                        for(L1CaloRegionCollection::const_iterator neighbor = newRegions->begin();
                                        neighbor != newRegions->end(); neighbor++) {
                                double neighborET = regionPhysicalEt(*neighbor);
                                if(deltaGctPhi(*newRegion, *neighbor) == 1 &&
                                                (newRegion->gctEta()    ) == neighbor->gctEta()) {
                                        neighborN_et = neighborET;
                                        if(puCorrectHI && useHI)
                                                neighborN_et = std::max(0.,neighborET -
                                                                (puLevelHIHI[neighbor->gctEta()]*regionLSB_));
                                        nNeighbors++; 
                                        //	std::cout<<"here neighborsN"<<endl;
                                        continue;
                                }
                                else if(deltaGctPhi(*newRegion, *neighbor) == -1 &&
                                                (newRegion->gctEta()    ) == neighbor->gctEta()) {
                                        neighborS_et = neighborET;
                                        if(puCorrectHI && useHI)
                                                neighborS_et = std::max(0.,neighborET -
                                                                (puLevelHIHI[neighbor->gctEta()]*regionLSB_));
                                        nNeighbors++;
                                        continue;
                                }
                                else if(deltaGctPhi(*newRegion, *neighbor) == 0 &&
                                                (newRegion->gctEta() + 1) == neighbor->gctEta()) {
                                        neighborE_et = neighborET;
                                        if(puCorrectHI && useHI)
                                                neighborE_et = std::max(0.,neighborET -
                                                                (puLevelHIHI[neighbor->gctEta()]*regionLSB_));
                                        nNeighbors++;
                                        continue;
                                }
                                else if(deltaGctPhi(*newRegion, *neighbor) == 0 &&
                                                (newRegion->gctEta() - 1) == neighbor->gctEta()) {
                                        neighborW_et = neighborET;
                                        if(puCorrectHI && useHI)
                                                neighborW_et = std::max(0.,neighborET -
                                                                (puLevelHIHI[neighbor->gctEta()]*regionLSB_));
                                        nNeighbors++;
                                        continue;
                                }
                                else if(deltaGctPhi(*newRegion, *neighbor) == 1 &&
                                                (newRegion->gctEta() + 1) == neighbor->gctEta()) {
                                        neighborNE_et = neighborET;
                                        if(puCorrectHI && useHI)
                                                neighborNE_et = std::max(0.,neighborET -
                                                                (puLevelHIHI[neighbor->gctEta()]*regionLSB_));
                                        nNeighbors++;
                                        continue;
                                }
                                else if(deltaGctPhi(*newRegion, *neighbor) == -1 &&
                                                (newRegion->gctEta() - 1) == neighbor->gctEta()) {
                                        neighborSW_et = neighborET;
                                        if(puCorrectHI && useHI)
                                                neighborSW_et = std::max(0.,neighborET -
                                                                (puLevelHIHI[neighbor->gctEta()]*regionLSB_));
                                        nNeighbors++;
                                        continue;
                                }
                                else if(deltaGctPhi(*newRegion, *neighbor) == 1 &&
                                                (newRegion->gctEta() - 1) == neighbor->gctEta()) {
                                        neighborNW_et = neighborET;
                                        if(puCorrectHI && useHI)
                                                neighborNW_et = std::max(0.,neighborET -
                                                                (puLevelHIHI[neighbor->gctEta()]*regionLSB_));
                                        nNeighbors++;
                                        continue;
                                }
                                else if(deltaGctPhi(*newRegion, *neighbor) == -1 &&
                                                (newRegion->gctEta() + 1) == neighbor->gctEta()) {
                                        neighborSE_et = neighborET;
                                        if(puCorrectHI && useHI)
                                                neighborSE_et = std::max(0.,neighborET -
                                                                (puLevelHIHI[neighbor->gctEta()]*regionLSB_));
                                        nNeighbors++;
                                        continue;
                                }
                        }
                        if(regionET > neighborN_et &&
                                        regionET > neighborNW_et &&
                                        regionET > neighborW_et &&
                                        regionET > neighborSW_et &&
                                        regionET >= neighborNE_et &&
                                        regionET >= neighborE_et &&
                                        regionET >= neighborSE_et &&
                                        regionET >= neighborS_et) {
                                unsigned int jetET = regionET +
                                        neighborN_et + neighborS_et + neighborE_et + neighborW_et +
                                        neighborNE_et + neighborSW_et + neighborSE_et + neighborNW_et;

                                /*std::cout<<"FOUND JET!   " <<regionET<<std::endl;
                                  std::cout<<"\t  "<<neighborNW_et<<"  "<<neighborN_et<<"   "<<neighborNE_et<<std::endl;
                                  std::cout<<"\t  "<<neighborW_et<<"  "<<regionET<<"   "<<neighborE_et<<std::endl;
                                  std::cout<<"\t  "<<neighborSW_et<<"  "<<neighborS_et<<"   "<<neighborSE_et<<std::endl;
                                  */

                                /*
                                   int jetPhi = newRegion->gctPhi() * 4 +
                                   ( - 2 * (neighborS_et + neighborSE_et + neighborSW_et)
                                   + 2 * (neighborN_et + neighborNE_et + neighborNW_et) ) / jetET;
                                   if(jetPhi < 0) {

                                   }
                                   else if(jetPhi >= ((int) N_JET_PHI)) {
                                   jetPhi -= N_JET_PHI;
                                   }
                                   int jetEta = newRegion->gctEta() * 4 +
                                   ( - 2 * (neighborW_et + neighborNW_et + neighborSW_et)
                                   + 2 * (neighborE_et + neighborNE_et + neighborSE_et) ) / jetET;
                                   if(jetEta < 0) jetEta = 0;
                                   if(jetEta >= ((int) N_JET_ETA)) jetEta = N_JET_ETA - 1;
                                   */
                                // Temporarily use the region granularity -- we will try to improve as above when code is debugged
                                int jetPhi = newRegion->gctPhi();
                                int jetEta = newRegion->gctEta();

                                bool neighborCheck = (nNeighbors == 8);
                                // On the eta edge we only expect 5 neighbors
                                if (!neighborCheck && (jetEta == 0 || jetEta == 21) && nNeighbors == 5)
                                        neighborCheck = true;

                                if (!neighborCheck) {
                                        std::cout << "phi: " << jetPhi << " eta: " << jetEta << " n: " << nNeighbors << std::endl;
                                        std::cout << "JetPt: " << jetET << " regionET: " << regionET << std::endl;
                                        assert(false);
                                }
                                UCTCandidate theJet(jetET, convertRegionEta(jetEta), convertRegionPhi(jetPhi));
                                theJet.setInt("rgnEta", jetEta);
                                theJet.setInt("rgnPhi", jetPhi);
                                theJet.setInt("rctEta",  newRegion->rctEta());
                                theJet.setInt("rctPhi", newRegion->rctPhi());
                                theJet.setInt("rank", jetET);

                                theJet.setInt("neighborNW_et", neighborNW_et);
                                theJet.setInt("neighborW_et", neighborW_et);
                                theJet.setInt("neighborSW_et", neighborSW_et);
                                theJet.setInt("neighborNE_et", neighborNE_et);
                                theJet.setInt("neighborE_et", neighborE_et);
                                theJet.setInt("neighborSW_et", neighborSW_et); 
                                theJet.setInt("neighborSE_et", neighborSE_et);
                                theJet.setInt("neighborN_et", neighborN_et);
                                theJet.setInt("neighborS_et", neighborS_et);
                                theJet.setInt("jetseed_et", regionET);

                                // Embed the puLevelHI information in the jet object for later tuning
                                theJet.setFloat("puLevelPUM0",puLevelPUM0);
                                theJet.setFloat("puLevelHI", puLevelHI);
                                theJet.setFloat("puLevelHIUIC", puLevelHIUIC);
                                // Store information about the "core" PT of the jet (central region)
                                theJet.setFloat("associatedRegionEt", regionET);
                                jetList.push_back(theJet);
                        }
                }
        }
        jetList.sort();
        jetList.reverse();
}

list<UCTCandidate>
UCT2015Producer::correctJets(const list<UCTCandidate>& jets, bool isJet) {
        // jet corrections only valid if PU density has been calculated
        list<UCTCandidate> corrlist;
        if (!applyJetCalibration) {corrlist=jets; return corrlist;}

        corrlist.clear();

        for(list<UCTCandidate>::const_iterator jet = jets.begin(); jet != jets.end(); jet++) {

                const double jetET=jet->pt();
                double alpha = m_jetSF[2*jet->getInt("rgnEta") + 0]; //Scale factor (See jetSF_cfi.py)
                double gamma = ((m_jetSF[2*jet->getInt("rgnEta") + 1])); //Offset

                double jpt = jetET*alpha+gamma;
                unsigned int corjetET =(int) jpt;

//                cout<<"JET :"<<jetET<<"    "<<jet->getInt("rgnEta")<<"    "<<alpha<<"   "<<gamma<<"    -->"<<jpt<<endl;

                UCTCandidate newJet(corjetET, convertRegionEta(jet->getInt("rgnEta")), convertRegionPhi(jet->getInt("rgnPhi")));
                newJet.setFloat("uncorrectedPt", jetET);
                newJet.setInt("rgnEta", jet->getInt("rgnEta"));
                newJet.setInt("rgnPhi", jet->getInt("rgnPhi"));
                newJet.setInt("rctEta", jet->getInt("rctEta"));
                newJet.setInt("rctPhi", jet->getInt("rctPhi"));
                newJet.setInt("rank", corjetET);

                if(isJet){
                newJet.setInt("jetseed_et", jet->getInt("jetseed_et"));
                newJet.setInt("neighborNW_et", jet->getInt("neighborNW_et"));
                newJet.setInt("neighborN_et", jet->getInt("neighborN_et"));
                newJet.setInt("neighborNE_et", jet->getInt("neighborNE_et"));
                newJet.setInt("neighborW_et", jet->getInt("neighborW_et"));
                newJet.setInt("neighborE_et", jet->getInt("neighborE_et"));
                newJet.setInt("neighborSW_et", jet->getInt("neighborSW_et"));
                newJet.setInt("neighborS_et", jet->getInt("neighborS_et"));
                newJet.setInt("neighborSE_et", jet->getInt("neighborSE_et"));
                }
                                newJet.setFloat("puLevelPUM0",puLevelPUM0);
                                newJet.setFloat("puLevelHI", puLevelHI);
                                newJet.setFloat("puLevelHIUIC", puLevelHIUIC);

                corrlist.push_back(newJet);
        }

        corrlist.sort();
        corrlist.reverse();

        return corrlist;
}   // This is outdated now - check with MIT for HI

// Given a region at iphi/ieta, find the highest region in the surrounding
// regions.
void UCT2015Producer::findAnnulusInfo(int ieta, int iphi,
                const L1CaloRegionCollection& regions,
                double* associatedSecondRegionEt,
                double* associatedThirdRegionEt,
                unsigned int* mipsInAnnulus,
                unsigned int* egFlagsInAnnulus,
                unsigned int* mipInSecondRegion) const {

        unsigned int neighborsFound = 0;
        unsigned int mipsCount = 0;
        unsigned int egFlagCount = 0;
        double highestNeighborEt = 0;
        // We don't want to count the contribution of the highest neighbor, this allows
        // us to subtract off the highest neighbor at the end, so we only loop once.
        bool highestNeighborHasMip = false;
        bool highestNeighborHasEGFlag = false;
        double secondNeighborEt = 0;


        for(L1CaloRegionCollection::const_iterator region = regions.begin();
                        region != regions.end(); region++) {
                int regionPhi = region->gctPhi();
                int regionEta = region->gctEta();
                unsigned int deltaPhi = std::abs(deltaPhiWrapAtN(18, iphi, regionPhi));
                unsigned int deltaEta = std::abs(ieta - regionEta);
                if ((deltaPhi + deltaEta) > 0 && deltaPhi < 2 && deltaEta < 2) {
                        double regionET = regionPhysicalEt(*region);
                        if (regionET > highestNeighborEt) {
                                if(highestNeighborEt!=0) secondNeighborEt=highestNeighborEt;
                                highestNeighborEt = regionET;
                                // Keep track of what flags the highest neighbor has
                                highestNeighborHasMip = region->mip();
                                highestNeighborHasEGFlag = !region->mip() && !region->tauVeto();
                        }

                        // count how many neighbors pass the flags.
                        if (region->mip()) {
                                mipsCount++;
                        }
                        if (!region->mip() && !region->tauVeto()) {
                                egFlagCount++;
                        }

                        // If we already found all 8 neighbors, we don't need to keep looping
                        // over the regions.
                        neighborsFound++;
                        if (neighborsFound == 8) {
                                break;
                        }
                }
        }
        // check if we need to remove the highest neighbor from the flag count.
        if (highestNeighborHasMip)
                mipsCount--;
        if (highestNeighborHasEGFlag)
                egFlagCount--;

        // set output
        *associatedSecondRegionEt = highestNeighborEt;
        *associatedThirdRegionEt =secondNeighborEt;
        *mipsInAnnulus = mipsCount;
        *mipInSecondRegion = highestNeighborHasMip;
        *egFlagsInAnnulus = egFlagCount;
}

void UCT2015Producer::makeEGTaus() {
        rlxTauList.clear();
        isoTauList.clear();
        rlxEGList.clear();
        isoEGList.clear();
        for(L1CaloEmCollection::const_iterator egtCand =
                        newEMCands->begin();
                        egtCand != newEMCands->end(); egtCand++){
                double et = egPhysicalEt(*egtCand);
                if(et > egtSeed) {
                        for(L1CaloRegionCollection::const_iterator region = newRegions->begin();
                                        region != newRegions->end(); region++) {
                                if(egtCand->regionId().iphi() == region->gctPhi() &&
                                                egtCand->regionId().ieta() == region->gctEta())
                                {
                                        double regionEt = regionPhysicalEt(*region);

                                        bool isEle=false;
                                        if(et<40 && (!region->tauVeto() && !region->mip() )) isEle=true;      
                                        if(et>=40 && et<63 && (!region->mip() )) isEle=true;
                                        if(et>=63) isEle=true;
                                        isEle=true;  // Lets rescue the old LUT

                                        // Find the highest region in the 3x3 annulus around the center
                                        // region.
                                        double associatedSecondRegionEt = 0;
                                        double associatedThirdRegionEt = 0;
                                        unsigned int mipsInAnnulus = 0;
                                        unsigned int egFlagsInAnnulus = 0;
                                        unsigned int mipInSecondRegion = 0;
                                        findAnnulusInfo(
                                                        egtCand->regionId().ieta(), egtCand->regionId().iphi(),
                                                        *newRegions,
                                                        &associatedSecondRegionEt, &associatedThirdRegionEt, &mipsInAnnulus, &egFlagsInAnnulus,
                                                        &mipInSecondRegion);

                                        UCTCandidate egtauCand(
                                                        et,
                                                        convertRegionEta(egtCand->regionId().ieta()),
                                                        convertRegionPhi(egtCand->regionId().iphi()));

                                        /*            UCTCandidate tauCand(
                                                      regionEt,
                                                      convertRegionEta(egtCand->regionId().ieta()),
                                                      convertRegionPhi(egtCand->regionId().iphi()));
                                                      */


                                        // Add extra information to the candidate
                                        egtauCand.setInt("rgnEta", egtCand->regionId().ieta());
                                        egtauCand.setInt("rgnPhi", egtCand->regionId().iphi());
                                        egtauCand.setInt("rctEta", egtCand->regionId().rctEta());
                                        egtauCand.setInt("rctPhi", egtCand->regionId().rctPhi());
                                        egtauCand.setInt("rank", egtCand->rank());
                                        egtauCand.setFloat("associatedJetPt", -3);
                                        egtauCand.setFloat("associatedRegionEt", regionEt);
                                        egtauCand.setFloat("associatedSecondRegionEt", associatedSecondRegionEt);
                                        egtauCand.setInt("associatedSecondRegionMIP", mipInSecondRegion);
                                        egtauCand.setFloat("puLevelHI", puLevelHI);
                                        egtauCand.setFloat("puLevelHIUIC", puLevelHIUIC);
                                        egtauCand.setFloat("puLevelPUM0",puLevelPUM0);
                                        egtauCand.setInt("ellIsolation", egtCand->isolated());
                                        egtauCand.setInt("tauVeto", region->tauVeto());
                                        egtauCand.setInt("mipBit", region->mip());
                                        egtauCand.setInt("isEle", isEle);
                                        egtauCand.setFloat("relativeJetIsolation", -100);

                                        /*
                                           tauCand.setInt("rgnEta", egtCand->regionId().ieta());
                                           tauCand.setInt("rgnPhi", egtCand->regionId().iphi());
                                           tauCand.setInt("rctEta", egtCand->regionId().rctEta());
                                           tauCand.setInt("rctPhi", egtCand->regionId().rctPhi());
                                           tauCand.setFloat("associatedRegionEt", regionEt);
                                           tauCand.setFloat("associatedJetPt", -3);
                                           tauCand.setFloat("associatedSecondRegionEt", associatedSecondRegionEt);
                                           tauCand.setInt("associatedSecondRegionMIP", mipInSecondRegion);
                                           tauCand.setInt("tauVeto", region->tauVeto());
                                           tauCand.setInt("mipBit", region->mip());
                                           */


                                        // A 2x1 and 1x2 cluster above egtSeed is always in tau list
                                        rlxTauList.push_back(egtauCand);

                                        // Note tauVeto now refers to emActivity pattern veto;
                                        // Good patterns are from EG candidates

                                        // Look for overlapping jet and require that isolation be passed
//                                          for(list<UCTCandidate>::iterator jet = corrJetList.begin(); jet != corrJetList.end(); jet++) { 

                                        if (isEle){
                                                rlxEGList.push_back(egtauCand);
                                        }

                                        bool MATCHEDJETFOUND_=false;        
                                        for(list<UCTCandidate>::iterator jet = jetList.begin(); jet != jetList.end(); jet++) {

                                                if((int)egtCand->regionId().iphi() == jet->getInt("rgnPhi") &&
                                                                (int)egtCand->regionId().ieta() == jet->getInt("rgnEta")) {
                                                        // Embed tuning parameters into the relaxed objects
                                                        rlxTauList.back().setFloat("associatedJetPt", jet->pt());

                                                        MATCHEDJETFOUND_=true;
                                                        // EG ID enabled! MC
                                                        if (isEle){
                                                                rlxEGList.back().setFloat("associatedJetPt", jet->pt());
                                                                bool isHighPtEle=true;                      
                                                                if(jet->pt()>2*regionEt) isHighPtEle=false;
                                                                rlxEGList.back().setInt("isHighPtEle",isHighPtEle);
                                                        }


                                                        double jetIsolation = jet->pt() - regionEt;        // Jet isolation
                                                        double relativeJetIsolation = jetIsolation / regionEt;
                                                        // A 2x1 and 1x2 cluster above egtSeed passing relative isolation will be in tau list
                                                        if(relativeJetIsolation < relativeTauIsolationCut || regionEt > switchOffTauIso){
                                                                isoTauList.push_back(rlxTauList.back());
                                                        }
                                                        //double jetIsolationRegionEG = jet->pt()-regionEt;   // Core isolation (could go less than zero)
                                                        //double relativeJetIsolationRegionEG = jetIsolationRegionEG / regionEt;
                                                        double jetIsolationEG = jet->pt() - et;        // Jet isolation
                                                        double relativeJetIsolationEG = jetIsolationEG / et;

                                                        rlxEGList.back().setFloat("relativeIsolationEG",relativeJetIsolationEG);

                                                        bool isolatedEG=false;
                                                        if(et<63 && relativeJetIsolationEG < relativeJetIsolationCut)  isolatedEG=true;; 
                                                        if (et>=63) isolatedEG=true;;
                                                        
                                                        if(isEle){
                                                                rlxEGList.back().setInt("isIsolated",isolatedEG);
                                                                if(isolatedEG){
                                                                        isoEGList.push_back(rlxEGList.back());
                                                                }
                                                        }
                                                        break;
                                                }
                                        }
                                        if(!MATCHEDJETFOUND_ && isEle) {
                                                rlxEGList.back().setFloat("associatedJetPt",-777);
                                                rlxEGList.back().setInt("isHighPtEle",true);        
                                                rlxEGList.back().setInt("isIsolated",true);
                                                rlxEGList.back().setFloat("relativeIsolationEG",-100);
                                                isoEGList.push_back(rlxEGList.back());

                                        }
                                        break;
                                }
                        }
                }
        }
        rlxEGList.sort();
        rlxTauList.sort();
        isoEGList.sort();
        isoTauList.sort();
        rlxEGList.reverse();
        rlxTauList.reverse();
        isoEGList.reverse();
        isoTauList.reverse();

}

void UCT2015Producer::makeTaus() {
        rlxTauRegionOnlyList.clear();
        isoTauRegionOnlyList.clear();
        for(L1CaloRegionCollection::const_iterator region = newRegions->begin();
                        region != newRegions->end(); region++) {
                double regionEt = regionPhysicalEt(*region);
                if(regionEt<tauSeed) continue;

                double associatedSecondRegionEt = 0;
                double associatedThirdRegionEt = 0;
                unsigned int mipsInAnnulus = 0;
                unsigned int egFlagsInAnnulus = 0;
                unsigned int mipInSecondRegion = 0;
                findAnnulusInfo(
                                region->id().ieta(), region->id().iphi(),
                                *newRegions,
                                &associatedSecondRegionEt, &associatedThirdRegionEt,  &mipsInAnnulus, &egFlagsInAnnulus,
                                &mipInSecondRegion);

                double tauEt=regionEt;
                //            if(associatedSecondRegionEt>tauSeed)  tauEt +=associatedSecondRegionEt;

                UCTCandidate tauCand(
                                tauEt,
                                convertRegionEta(region->id().ieta()),    // this is wrong (should take into acount the neighbours and center the tau in the division
                             convertRegionPhi(region->id().iphi()));   // also, two taus will appear! we need to remove one


                                tauCand.setInt("gctEta", region->gctEta());
                                tauCand.setInt("gctPhi", region->gctPhi());
                                tauCand.setInt("rgnEta", region->id().ieta());
                                tauCand.setInt("rgnPhi", region->id().iphi());
                                tauCand.setInt("rctEta", region->id().rctEta());
                                tauCand.setInt("rctPhi", region->id().rctPhi());
                                tauCand.setFloat("associatedJetPt", -3);
                                tauCand.setFloat("associatedRegionEt", regionEt);
                                tauCand.setFloat("puLevelHI", puLevelHI);
                                tauCand.setFloat("puLevelHIUIC", puLevelHIUIC);
                                tauCand.setFloat("puLevelPUM0",puLevelPUM0);
                                tauCand.setInt("tauVeto", region->tauVeto());
                                tauCand.setInt("mipBit", region->mip());
                                tauCand.setFloat("associatedSecondRegionEt", associatedSecondRegionEt);
                                tauCand.setInt("associatedSecondRegionMIP", mipInSecondRegion);
                                tauCand.setFloat("associatedThirdRegionEt", associatedThirdRegionEt);

                                rlxTauRegionOnlyList.push_back(tauCand);

                        
                                bool MATCHEDJETFOUND_=false;
                                // Look for overlapping jet and require that isolation be passed
                                for(list<UCTCandidate>::iterator jet = jetList.begin(); jet != jetList.end(); jet++) {
//                                  for(list<UCTCandidate>::iterator jet = corrJetList.begin(); jet != corrJetList.end(); jet++) {      
                                        if((int)region->gctPhi() == jet->getInt("rgnPhi") &&
                                                        (int)region->gctEta() == jet->getInt("rgnEta")) {
                                                MATCHEDJETFOUND_=true;
                                                rlxTauRegionOnlyList.back().setFloat("associatedJetPt", jet->pt());

                                                double jetIsolation = jet->pt() - regionEt;        // Jet isolation
                                                double relativeJetIsolation = jetIsolation / regionEt;
                                                if(relativeJetIsolation < relativeTauIsolationCut || regionEt > switchOffTauIso){
                                                        isoTauRegionOnlyList.push_back(rlxTauRegionOnlyList.back());
                                                }

                                                break;
                                        }
                                }
                                if(!MATCHEDJETFOUND_){ 
                                        rlxTauRegionOnlyList.back().setFloat("associatedJetPt", -777);
                                        isoTauRegionOnlyList.push_back(rlxTauRegionOnlyList.back());
                                 }       
        }
        rlxTauRegionOnlyList.sort();
        isoTauRegionOnlyList.sort();
        rlxTauRegionOnlyList.reverse();
        isoTauRegionOnlyList.reverse();



}


//define this as a plug-in
DEFINE_FWK_MODULE(UCT2015Producer);
