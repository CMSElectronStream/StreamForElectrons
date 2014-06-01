// g++ -Wall -o EventComparison.exe `root-config --glibs --libs --cflags` EventComparison.cpp -lTreePlayer
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <memory>

#include "TTree.h"
#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TTreeIndex.h"
#include "TROOT.h"
#include "TDirectory.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TPaveStats.h"
#include "TLatex.h"
#include "TStyle.h"

// stream integer variables
Int_t runId_s,lumiId_s,isW_s,isZ_s,PV_n_s,nele_s,ele1_nRecHits_s,ele1_nGgsfTrackHits_s,ele1_numberOfLostHits_s,ele1_nAmbiguousGsfTrack_s,ele1_isEB_s;
Int_t ele1_seedIeta_s,ele1_seedIphi_s,ele1_seedIx_s,ele1_seedIy_s,ele1_seedZside_s;

// recco integer variables
Int_t runId_r,lumiId_r,isW_r,isZ_r,PV_n_r,nele_r,ele1_nRecHits_r,ele1_nGgsfTrackHits_r,ele1_numberOfLostHits_r,ele1_nAmbiguousGsfTrack_r,ele1_isEB_r;
Int_t ele1_seedIeta_r,ele1_seedIphi_r,ele1_seedIx_r,ele1_seedIy_r,ele1_seedZside_r;

Long64_t eventId_s, eventId_r ;

// stream float
Float_t rho_s,ele1_pt_s,ele1_eta_s,ele1_phi_s,ele1_sigmaIetaIeta_s,ele1_DphiIn_s,ele1_DetaIn_s,ele1_HOverE_s,ele1_tkIso_s,ele1_emIso_s,ele1_hadIso_s;
Float_t ele1_scERaw_s,ele1_scE_s,ele1_es_s,ele1_scEta_s,ele1_scPhi_s,ele1_scEtaWidth_s,ele1_scPhiWidth_s,ele1_tkP_s,ele1_fbrem_s,ele1_dxy_PV_s,ele1_dz_PV_s,ele1_e5x5_s,ele1_e3x3_s,ele1_seedE_s,ele1_scNxtal_s;
Float_t met_s,met_phi_s,ele1Met_mt_s;

// reco float
Float_t rho_r,ele1_pt_r,ele1_eta_r,ele1_phi_r,ele1_sigmaIetaIeta_r,ele1_DphiIn_r,ele1_DetaIn_r,ele1_HOverE_r,ele1_tkIso_r,ele1_emIso_r,ele1_hadIso_r;
Float_t ele1_scERaw_r,ele1_scE_r,ele1_es_r,ele1_scEta_r,ele1_scPhi_r,ele1_scEtaWidth_r,ele1_scPhiWidth_r,ele1_tkP_r,ele1_fbrem_r,ele1_dxy_PV_r,ele1_dz_PV_r,ele1_e5x5_r,ele1_e3x3_r,ele1_seedE_r,ele1_scNxtal_r;
Float_t met_r,met_phi_r,ele1Met_mt_r;

// stream regression inputs 
Float_t ele1_eRegrInput_rawE_s,ele1_eRegrInput_r9_s,ele1_eRegrInput_eta_s,ele1_eRegrInput_phi_s,ele1_eRegrInput_etaW_s,ele1_eRegrInput_phiW_s,ele1_eRegrInput_SCsize_s,ele1_eRegrInput_rho_s;
Float_t ele1_eRegrInput_hoe_s,ele1_eRegrInput_nPV_s,ele1_eRegrInput_seed_eta_s,ele1_eRegrInput_seed_phi_s,ele1_eRegrInput_seed_E_s,ele1_eRegrInput_seed_e3x3_s,ele1_eRegrInput_seed_e5x5_s;
Float_t ele1_eRegrInput_sigietaieta_s,ele1_eRegrInput_sigiphiiphi_s,ele1_eRegrInput_sigietaiphi_s,ele1_eRegrInput_eMax_s,ele1_eRegrInput_e2nd_s,ele1_eRegrInput_eTop_s,ele1_eRegrInput_eBottom_s;
Float_t ele1_eRegrInput_eLeft_s,ele1_eRegrInput_eRight_s,ele1_eRegrInput_e2x5Max_s,ele1_eRegrInput_e2x5Top_s,ele1_eRegrInput_e2x5Bottom_s,ele1_eRegrInput_e2x5Left_s,ele1_eRegrInput_e2x5Right_s;
Float_t ele1_eRegrInput_seed_ieta_s,ele1_eRegrInput_seed_iphi_s,ele1_eRegrInput_seed_etaCrySeed_s,ele1_eRegrInput_seed_phiCrySeed_s,ele1_eRegrInput_preshowerEnergyOverRaw_s,ele1_eRegrInput_ecalDrivenSeed_s;
Float_t ele1_eRegrInput_isEBEtaGap_s,ele1_eRegrInput_isEBPhiGap_s,ele1_eRegrInput_eSubClusters_s,ele1_eRegrInput_subClusterEnergy_1_s,ele1_eRegrInput_subClusterEnergy_2_s;
Float_t ele1_eRegrInput_subClusterEnergy_3_s,ele1_eRegrInput_subClusterEta_1_s,ele1_eRegrInput_subClusterEta_2_s,ele1_eRegrInput_subClusterEta_3_s,ele1_eRegrInput_subClusterPhi_1_s;
Float_t ele1_eRegrInput_subClusterPhi_2_s,ele1_eRegrInput_subClusterPhi_3_s,ele1_eRegrInput_subClusterEmax_1_s,ele1_eRegrInput_subClusterEmax_2_s,ele1_eRegrInput_subClusterEmax_3_s;
Float_t ele1_eRegrInput_subClusterE3x3_1_s,ele1_eRegrInput_subClusterE3x3_2_s,ele1_eRegrInput_subClusterE3x3_3_s,ele1_eRegrInput_eESClusters_s,ele1_eRegrInput_eESClusterEnergy_1_s;
Float_t ele1_eRegrInput_eESClusterEnergy_2_s,ele1_eRegrInput_eESClusterEnergy_3_s,ele1_eRegrInput_eESClusterEta_1_s,ele1_eRegrInput_eESClusterEta_2_s,ele1_eRegrInput_eESClusterEta_3_s;
Float_t ele1_eRegrInput_eESClusterPhi_1_s,ele1_eRegrInput_eESClusterPhi_2_s,ele1_eRegrInput_eESClusterPhi_3_s,ele1_eRegrInput_pt_s,ele1_eRegrInput_trackMomentumAtVtxR_s,ele1_eRegrInput_fbrem_s;
Float_t ele1_eRegrInput_charge_s,ele1_eRegrInput_eSuperClusterOverP_s;


// reco regression inputs 
Float_t ele1_eRegrInput_rawE_r,ele1_eRegrInput_r9_r,ele1_eRegrInput_eta_r,ele1_eRegrInput_phi_r,ele1_eRegrInput_etaW_r,ele1_eRegrInput_phiW_r,ele1_eRegrInput_SCsize_r,ele1_eRegrInput_rho_r;
Float_t ele1_eRegrInput_hoe_r,ele1_eRegrInput_nPV_r,ele1_eRegrInput_seed_eta_r,ele1_eRegrInput_seed_phi_r,ele1_eRegrInput_seed_E_r,ele1_eRegrInput_seed_e3x3_r,ele1_eRegrInput_seed_e5x5_r;
Float_t ele1_eRegrInput_sigietaieta_r,ele1_eRegrInput_sigiphiiphi_r,ele1_eRegrInput_sigietaiphi_r,ele1_eRegrInput_eMax_r,ele1_eRegrInput_e2nd_r,ele1_eRegrInput_eTop_r,ele1_eRegrInput_eBottom_r;
Float_t ele1_eRegrInput_eLeft_r,ele1_eRegrInput_eRight_r,ele1_eRegrInput_e2x5Max_r,ele1_eRegrInput_e2x5Top_r,ele1_eRegrInput_e2x5Bottom_r,ele1_eRegrInput_e2x5Left_r,ele1_eRegrInput_e2x5Right_r;
Float_t ele1_eRegrInput_seed_ieta_r,ele1_eRegrInput_seed_iphi_r,ele1_eRegrInput_seed_etaCrySeed_r,ele1_eRegrInput_seed_phiCrySeed_r,ele1_eRegrInput_preshowerEnergyOverRaw_r,ele1_eRegrInput_ecalDrivenSeed_r;
Float_t ele1_eRegrInput_isEBEtaGap_r,ele1_eRegrInput_isEBPhiGap_r,ele1_eRegrInput_eSubClusters_r,ele1_eRegrInput_subClusterEnergy_1_r,ele1_eRegrInput_subClusterEnergy_2_r;
Float_t ele1_eRegrInput_subClusterEnergy_3_r,ele1_eRegrInput_subClusterEta_1_r,ele1_eRegrInput_subClusterEta_2_r,ele1_eRegrInput_subClusterEta_3_r,ele1_eRegrInput_subClusterPhi_1_r;
Float_t ele1_eRegrInput_subClusterPhi_2_r,ele1_eRegrInput_subClusterPhi_3_r,ele1_eRegrInput_subClusterEmax_1_r,ele1_eRegrInput_subClusterEmax_2_r,ele1_eRegrInput_subClusterEmax_3_r;
Float_t ele1_eRegrInput_subClusterE3x3_1_r,ele1_eRegrInput_subClusterE3x3_2_r,ele1_eRegrInput_subClusterE3x3_3_r,ele1_eRegrInput_eESClusters_r,ele1_eRegrInput_eESClusterEnergy_1_r;
Float_t ele1_eRegrInput_eESClusterEnergy_2_r,ele1_eRegrInput_eESClusterEnergy_3_r,ele1_eRegrInput_eESClusterEta_1_r,ele1_eRegrInput_eESClusterEta_2_r,ele1_eRegrInput_eESClusterEta_3_r;
Float_t ele1_eRegrInput_eESClusterPhi_1_r,ele1_eRegrInput_eESClusterPhi_2_r,ele1_eRegrInput_eESClusterPhi_3_r,ele1_eRegrInput_pt_r,ele1_eRegrInput_trackMomentumAtVtxR_r,ele1_eRegrInput_fbrem_r;
Float_t ele1_eRegrInput_charge_r,ele1_eRegrInput_eSuperClusterOverP_r;

// vector branches
std::vector<float>* ele1_recHit_E_s = new std::vector<float>();
std::vector<float>* ele1_recHit_E_r = new std::vector<float>();
TBranch* b_ele1_recHit_E_s, *b_ele1_recHit_E_r;
std::vector<int>*  ele1_idtype_s = new std::vector<int>(); 
std::vector<int>*  ele1_idtype_r = new std::vector<int>(); 
TBranch* b_ele1_idtype_s, *b_ele1_idtype_r;

typedef std::pair<std::string,TH1F*> mapElement ;
typedef std::pair<std::string,TH2F*> mapElementCorr ;

void SetStartingBranchTrees(TTree*, TTree*);

void SetVariablesMapEntry(std::map<std::string, TH1F*> &, std::map<std::string, TH1F*> &, std::map<std::string, TH1F*> &);

void SetVariablesCorrelationMapEntry(std::map<std::string, TH2F*> &);

int main (int argc, char** argv){

 if(argc < 3){ std::cerr<<" two few input informations --> exit from the code"<<std::endl; return -1 ; }
 
 std::string ROOTStyle;
 if(getenv ("ROOTStyle")!=NULL){
  ROOTStyle = getenv ("ROOTStyle");
  gROOT->ProcessLine((".x "+ROOTStyle+"/setTDRStyle.C").c_str());
 }

 gROOT->ProcessLine("#include <vector>");
 gStyle->SetOptStat(11111111);

 std::map<std::string, TH1F*> DifferenceMap ;
 std::map<std::string, TH1F*> DistributionStreamMap ;
 std::map<std::string, TH1F*> DistributionRecoMap ;

 std::map<std::string, TH2F*> CorrelationMap ;

 TFile* inputFileStream  = new TFile(argv[1],"READ");
 TFile* inputFileStdReco = new TFile(argv[2],"READ");
 
 TTree* treeStream  = (TTree*) inputFileStream->Get("Analyzer/AnalyzerEle");
 TTree* treeStdReco = (TTree*) inputFileStdReco->Get("Analyzer/AnalyzerEle");

 treeStream->BuildIndex("eventId");
 treeStdReco->BuildIndex("eventId");
 
 TTreeIndex *TreeIndexStream  = (TTreeIndex*)treeStream->GetTreeIndex();
 TTreeIndex *TreeIndexStdReco = (TTreeIndex*)treeStdReco->GetTreeIndex();

 SetStartingBranchTrees(treeStream,treeStdReco);
 
 Long64_t* indexValuesStream = TreeIndexStream->GetIndexValues();
 Long64_t* indexStream       = TreeIndexStream->GetIndex();

 Long64_t* indexValuesReco   = TreeIndexStdReco->GetIndexValues();
 Long64_t* indexReco         = TreeIndexStdReco->GetIndex();

 std::vector<Long64_t> indexValuesRecoVector; indexValuesRecoVector.insert(indexValuesRecoVector.begin(),indexValuesReco,indexValuesReco+TreeIndexStream->GetN());
 std::vector<Long64_t> indexRecoVector; indexRecoVector.insert(indexRecoVector.begin(),indexReco,indexReco+TreeIndexStdReco->GetN());
 
 std::cout<<"Events in the stream file "<<TreeIndexStream->GetN()<<std::endl;
 std::cout<<"Events in the reco   file "<<TreeIndexStdReco->GetN()<<std::endl;
 
 SetVariablesMapEntry(DistributionStreamMap,DistributionRecoMap,DifferenceMap);

 SetVariablesCorrelationMapEntry(CorrelationMap);

 float rescaleEvents = 1 ;
 if(argc >= 4 and atof(argv[3]) >= 1) rescaleEvents = atof(argv[3]);
 std::cout<<"Rescale the events "<<rescaleEvents<<" effective loop "<<int(TreeIndexStream->GetN()/rescaleEvents)<<std::endl;
 for( int iEvent = 0; iEvent < int(TreeIndexStream->GetN()/rescaleEvents) ; iEvent++ ) {

  if( iEvent%1000 == 0 ) {
    std::cout << "reading saved entry " << iEvent << "::" << int(TreeIndexStream->GetN()/rescaleEvents) << "\r" << std::flush;
  }
  
  Long64_t localStream = treeStream->LoadTree(indexStream[iEvent]);
  treeStream->GetEntry(localStream);
    
  if(argc >= 5 ) std::cout<<"localStream "<<localStream<<" indexValues Stream "<<indexValuesStream[iEvent]<<" runId_s "<<runId_s<<" lumiId_s "<<lumiId_s<<" eventId_s "<<eventId_s<<std::endl;
  
  std::vector<Long64_t>::iterator itVal = std::find(indexValuesRecoVector.begin(),indexValuesRecoVector.end(),indexValuesStream[iEvent]);        
  
  if((*itVal) and int(itVal-indexValuesRecoVector.begin()) < int(indexRecoVector.size())){
   Long64_t localReco = indexRecoVector.at(int(itVal-indexValuesRecoVector.begin()));
   treeStdReco->GetEntry(localReco);
   if(argc >= 5 ) std::cout<<"localStdReco "<<localReco<<" indexValues Reco "<<(*itVal)<<" runId_r "<<runId_r<<" lumiId_r "<<lumiId_r<<" eventId_r "<<eventId_r<<std::endl;

   if( isW_s != 1 or isW_r != 1 ) continue;
         
   DistributionStreamMap["PV_n"]->Fill(PV_n_s);
   DistributionRecoMap["PV_n"]->Fill(PV_n_r);
   DifferenceMap["PV_n"]->Fill(PV_n_s-PV_n_r);
      
   DistributionStreamMap["rho"]->Fill(rho_s);       
   DistributionRecoMap["rho"]->Fill(rho_r);
   DifferenceMap["rho"]->Fill(rho_s-rho_r);

   DistributionStreamMap["met"]->Fill(met_s);       
   DistributionRecoMap["met"]->Fill(met_r);
   DifferenceMap["met"]->Fill(met_s-met_r);

   DistributionStreamMap["met_phi"]->Fill(met_phi_s);       
   DistributionRecoMap["met_phi"]->Fill(met_phi_r);
   DifferenceMap["met_phi"]->Fill(met_phi_s-met_phi_r);

   DistributionStreamMap["ele1Met_mt"]->Fill(ele1Met_mt_s);       
   DistributionRecoMap["ele1Met_mt"]->Fill(ele1Met_mt_r);
   DifferenceMap["ele1Met_mt"]->Fill(ele1Met_mt_s-ele1Met_mt_r);

   DistributionStreamMap["ele1_scNxtal"]->Fill(ele1_scNxtal_s);       
   DistributionRecoMap["ele1_scNxtal"]->Fill(ele1_scNxtal_r);
   DifferenceMap["ele1_scNxtal"]->Fill(ele1_scNxtal_s-ele1_scNxtal_r);

   DistributionStreamMap["ele1_nRecHits"]->Fill(ele1_nRecHits_s);       
   DistributionRecoMap["ele1_nRecHits"]->Fill(ele1_nRecHits_r);
   DifferenceMap["ele1_nRecHits"]->Fill(ele1_nRecHits_s-ele1_nRecHits_r);

   DistributionStreamMap["ele1_nGgsfTrackHits"]->Fill(ele1_nGgsfTrackHits_s);       
   DistributionRecoMap["ele1_nGgsfTrackHits"]->Fill(ele1_nGgsfTrackHits_r);
   DifferenceMap["ele1_nGgsfTrackHits"]->Fill(ele1_nGgsfTrackHits_s-ele1_nGgsfTrackHits_r);

   DistributionStreamMap["ele1_numberOfLostHits"]->Fill(ele1_numberOfLostHits_s);       
   DistributionRecoMap["ele1_numberOfLostHits"]->Fill(ele1_numberOfLostHits_r);
   DifferenceMap["ele1_numberOfLostHits"]->Fill(ele1_numberOfLostHits_s-ele1_numberOfLostHits_r);

   DistributionStreamMap["ele1_nAmbiguousGsfTrack"]->Fill(ele1_nAmbiguousGsfTrack_s);       
   DistributionRecoMap["ele1_nAmbiguousGsfTrack"]->Fill(ele1_nAmbiguousGsfTrack_r);
   DifferenceMap["ele1_nAmbiguousGsfTrack"]->Fill(ele1_nAmbiguousGsfTrack_s-ele1_nAmbiguousGsfTrack_r);
   
   // electron variables
   DistributionStreamMap["ele1_pt"]->Fill(ele1_pt_s);       
   DistributionRecoMap["ele1_pt"]->Fill(ele1_pt_r);
   DifferenceMap["ele1_pt"]->Fill(ele1_pt_s-ele1_pt_r);

   DistributionStreamMap["ele1_eta"]->Fill(ele1_eta_s);       
   DistributionRecoMap["ele1_eta"]->Fill(ele1_eta_r);
   DifferenceMap["ele1_eta"]->Fill(ele1_eta_s-ele1_eta_r);

   DistributionStreamMap["ele1_phi"]->Fill(ele1_phi_s);       
   DistributionRecoMap["ele1_phi"]->Fill(ele1_phi_r);
   DifferenceMap["ele1_phi"]->Fill(ele1_phi_s-ele1_phi_r);

   DistributionStreamMap["ele1_sigmaIetaIeta"]->Fill(ele1_sigmaIetaIeta_s);       
   DistributionRecoMap["ele1_sigmaIetaIeta"]->Fill(ele1_sigmaIetaIeta_r);
   DifferenceMap["ele1_sigmaIetaIeta"]->Fill(ele1_sigmaIetaIeta_s-ele1_sigmaIetaIeta_r);

   DistributionStreamMap["ele1_DphiIn"]->Fill(ele1_DphiIn_s);       
   DistributionRecoMap["ele1_DphiIn"]->Fill(ele1_DphiIn_r);
   DifferenceMap["ele1_DphiIn"]->Fill(ele1_DphiIn_s-ele1_DphiIn_r);

   DistributionStreamMap["ele1_DetaIn"]->Fill(ele1_DetaIn_s);       
   DistributionRecoMap["ele1_DetaIn"]->Fill(ele1_DetaIn_r);
   DifferenceMap["ele1_DetaIn"]->Fill(ele1_DetaIn_s-ele1_DetaIn_r);

   DistributionStreamMap["ele1_HOverE"]->Fill(ele1_HOverE_s);       
   DistributionRecoMap["ele1_HOverE"]->Fill(ele1_HOverE_r);
   DifferenceMap["ele1_HOverE"]->Fill(ele1_HOverE_s-ele1_HOverE_r);

   DistributionStreamMap["ele1_tkIso"]->Fill(ele1_tkIso_s);       
   DistributionRecoMap["ele1_tkIso"]->Fill(ele1_tkIso_r);
   DifferenceMap["ele1_tkIso"]->Fill(ele1_tkIso_s-ele1_tkIso_r);

   DistributionStreamMap["ele1_emIso"]->Fill(ele1_emIso_s);       
   DistributionRecoMap["ele1_emIso"]->Fill(ele1_emIso_r);
   DifferenceMap["ele1_emIso"]->Fill(ele1_emIso_s-ele1_emIso_r);

   DistributionStreamMap["ele1_hadIso"]->Fill(ele1_hadIso_s);       
   DistributionRecoMap["ele1_hadIso"]->Fill(ele1_hadIso_r);
   DifferenceMap["ele1_hadIso"]->Fill(ele1_hadIso_s-ele1_hadIso_r);
   
   DistributionStreamMap["ele1_scE"]->Fill(ele1_scE_s);       
   DistributionRecoMap["ele1_scE"]->Fill(ele1_scE_r);
   DifferenceMap["ele1_scE"]->Fill(ele1_scE_s-ele1_scE_r);

   DistributionStreamMap["ele1_scEta"]->Fill(ele1_scEta_s);       
   DistributionRecoMap["ele1_scEta"]->Fill(ele1_scEta_r);
   DifferenceMap["ele1_scEta"]->Fill(ele1_scEta_s-ele1_scEta_r);

   DistributionStreamMap["ele1_scPhi"]->Fill(ele1_scPhi_s);       
   DistributionRecoMap["ele1_scPhi"]->Fill(ele1_scPhi_r);
   DifferenceMap["ele1_scPhi"]->Fill(ele1_scPhi_s-ele1_scPhi_r);
     
   DistributionStreamMap["ele1_scEtaWidth"]->Fill(ele1_scEtaWidth_s);       
   DistributionRecoMap["ele1_scEtaWidth"]->Fill(ele1_scEtaWidth_r);
   DifferenceMap["ele1_scEtaWidth"]->Fill(ele1_scEtaWidth_s-ele1_scEtaWidth_r);
     
   DistributionStreamMap["ele1_scPhiWidth"]->Fill(ele1_scPhiWidth_s);       
   DistributionRecoMap["ele1_scPhiWidth"]->Fill(ele1_scPhiWidth_r);
   DifferenceMap["ele1_scPhiWidth"]->Fill(ele1_scPhiWidth_s-ele1_scPhiWidth_r);
     
   DistributionStreamMap["ele1_tkP"]->Fill(ele1_tkP_s);       
   DistributionRecoMap["ele1_tkP"]->Fill(ele1_tkP_r);
   DifferenceMap["ele1_tkP"]->Fill(ele1_tkP_s-ele1_tkP_r);
   
   DistributionStreamMap["ele1_fbrem"]->Fill(ele1_fbrem_s);       
   DistributionRecoMap["ele1_fbrem"]->Fill(ele1_fbrem_r);
   DifferenceMap["ele1_fbrem"]->Fill(ele1_fbrem_s-ele1_fbrem_r);
   
   DistributionStreamMap["ele1_dxy_PV"]->Fill(ele1_dxy_PV_s);       
   DistributionRecoMap["ele1_dxy_PV"]->Fill(ele1_dxy_PV_r);
   DifferenceMap["ele1_dxy_PV"]->Fill(ele1_dxy_PV_s-ele1_dxy_PV_r);
   
   DistributionStreamMap["ele1_dz_PV"]->Fill(ele1_dz_PV_s);       
   DistributionRecoMap["ele1_dz_PV"]->Fill(ele1_dz_PV_r);
   DifferenceMap["ele1_dz_PV"]->Fill(ele1_dz_PV_s-ele1_dz_PV_r);

   DistributionStreamMap["ele1_e5x5"]->Fill(ele1_e5x5_s);       
   DistributionRecoMap["ele1_e5x5"]->Fill(ele1_e5x5_r);
   DifferenceMap["ele1_e5x5"]->Fill(ele1_e5x5_s-ele1_e5x5_r);

   DistributionStreamMap["ele1_e3x3"]->Fill(ele1_e3x3_s);       
   DistributionRecoMap["ele1_e3x3"]->Fill(ele1_e3x3_r);
   DifferenceMap["ele1_e3x3"]->Fill(ele1_e3x3_s-ele1_e3x3_r);
   
   DistributionStreamMap["ele1_seedEnergy"]->Fill(ele1_seedE_s);       
   DistributionRecoMap["ele1_seedEnergy"]->Fill(ele1_seedE_r);
   DifferenceMap["ele1_seedEnergy"]->Fill(ele1_seedE_s-ele1_seedE_r);
   
   if(ele1_isEB_s == 1 and ele1_isEB_r == 1){

    DistributionStreamMap["ele1_seedIeta"]->Fill(ele1_seedIeta_s);
    DistributionRecoMap["ele1_seedIeta"]->Fill(ele1_seedIeta_r);
    DifferenceMap["ele1_seedIeta"]->Fill(ele1_seedIeta_s-ele1_seedIeta_r);

    DistributionStreamMap["ele1_seedIphi"]->Fill(ele1_seedIphi_s);
    DistributionRecoMap["ele1_seedIphi"]->Fill(ele1_seedIphi_r);
    DifferenceMap["ele1_seedIphi"]->Fill(ele1_seedIphi_s-ele1_seedIphi_r);

   }
   else if(ele1_isEB_s == 0 and ele1_isEB_r == 0){

    DistributionStreamMap["ele1_seedIx"]->Fill(ele1_seedIx_s);
    DistributionRecoMap["ele1_seedIx"]->Fill(ele1_seedIx_r);
    DifferenceMap["ele1_seedIx"]->Fill(ele1_seedIx_s-ele1_seedIx_r);

    DistributionStreamMap["ele1_seedIy"]->Fill(ele1_seedIy_s);
    DistributionRecoMap["ele1_seedIy"]->Fill(ele1_seedIy_r);
    DifferenceMap["ele1_seedIy"]->Fill(ele1_seedIy_s-ele1_seedIy_r);

    DistributionStreamMap["ele1_es"]->Fill(ele1_es_s);
    DistributionRecoMap["ele1_es"]->Fill(ele1_es_r);
    DifferenceMap["ele1_es"]->Fill(ele1_es_s-ele1_es_r);

   }

   // regression variables
   DistributionStreamMap["ele1_eRegrInput_r9"]->Fill(ele1_eRegrInput_r9_s);
   DistributionRecoMap["ele1_eRegrInput_r9"]->Fill(ele1_eRegrInput_r9_r);
   DifferenceMap["ele1_eRegrInput_r9"]->Fill(ele1_eRegrInput_r9_s-ele1_eRegrInput_r9_r);

   DistributionStreamMap["ele1_eRegrInput_SCsize"]->Fill(ele1_eRegrInput_SCsize_s);
   DistributionRecoMap["ele1_eRegrInput_SCsize"]->Fill(ele1_eRegrInput_SCsize_r);
   DifferenceMap["ele1_eRegrInput_SCsize"]->Fill(ele1_eRegrInput_SCsize_s-ele1_eRegrInput_SCsize_r);

   DistributionStreamMap["ele1_eRegrInput_seed_eta"]->Fill(ele1_eRegrInput_seed_eta_s);
   DistributionRecoMap["ele1_eRegrInput_seed_eta"]->Fill(ele1_eRegrInput_seed_eta_r);
   DifferenceMap["ele1_eRegrInput_seed_eta"]->Fill(ele1_eRegrInput_seed_eta_s-ele1_eRegrInput_seed_eta_r);

   DistributionStreamMap["ele1_eRegrInput_seed_phi"]->Fill(ele1_eRegrInput_seed_phi_s);
   DistributionRecoMap["ele1_eRegrInput_seed_phi"]->Fill(ele1_eRegrInput_seed_phi_r);
   DifferenceMap["ele1_eRegrInput_seed_phi"]->Fill(ele1_eRegrInput_seed_phi_s-ele1_eRegrInput_seed_phi_r);

   DistributionStreamMap["ele1_eRegrInput_sigiphiiphi"]->Fill(ele1_eRegrInput_sigiphiiphi_s);
   DistributionRecoMap["ele1_eRegrInput_sigiphiiphi"]->Fill(ele1_eRegrInput_sigiphiiphi_r);
   DifferenceMap["ele1_eRegrInput_sigiphiiphi"]->Fill(ele1_eRegrInput_sigiphiiphi_s-ele1_eRegrInput_sigiphiiphi_r);

   DistributionStreamMap["ele1_eRegrInput_sigietaiphi"]->Fill(ele1_eRegrInput_sigietaiphi_s);
   DistributionRecoMap["ele1_eRegrInput_sigietaiphi"]->Fill(ele1_eRegrInput_sigietaiphi_r);
   DifferenceMap["ele1_eRegrInput_sigietaiphi"]->Fill(ele1_eRegrInput_sigietaiphi_s-ele1_eRegrInput_sigietaiphi_r);

   DistributionStreamMap["ele1_eRegrInput_eMax"]->Fill(ele1_eRegrInput_eMax_s);
   DistributionRecoMap["ele1_eRegrInput_eMax"]->Fill(ele1_eRegrInput_eMax_r);
   DifferenceMap["ele1_eRegrInput_eMax"]->Fill(ele1_eRegrInput_eMax_s-ele1_eRegrInput_eMax_r);

   DistributionStreamMap["ele1_eRegrInput_e2nd"]->Fill(ele1_eRegrInput_e2nd_s);
   DistributionRecoMap["ele1_eRegrInput_e2nd"]->Fill(ele1_eRegrInput_e2nd_r);
   DifferenceMap["ele1_eRegrInput_e2nd"]->Fill(ele1_eRegrInput_e2nd_s-ele1_eRegrInput_e2nd_r);

   DistributionStreamMap["ele1_eRegrInput_eTop"]->Fill(ele1_eRegrInput_eTop_s);
   DistributionRecoMap["ele1_eRegrInput_eTop"]->Fill(ele1_eRegrInput_eTop_r);
   DifferenceMap["ele1_eRegrInput_eTop"]->Fill(ele1_eRegrInput_eTop_s-ele1_eRegrInput_eTop_r);

   DistributionStreamMap["ele1_eRegrInput_eBottom"]->Fill(ele1_eRegrInput_eBottom_s);
   DistributionRecoMap["ele1_eRegrInput_eBottom"]->Fill(ele1_eRegrInput_eBottom_r);
   DifferenceMap["ele1_eRegrInput_eBottom"]->Fill(ele1_eRegrInput_eBottom_s-ele1_eRegrInput_eBottom_r);

   DistributionStreamMap["ele1_eRegrInput_eLeft"]->Fill(ele1_eRegrInput_eLeft_s);
   DistributionRecoMap["ele1_eRegrInput_eLeft"]->Fill(ele1_eRegrInput_eLeft_r);
   DifferenceMap["ele1_eRegrInput_eLeft"]->Fill(ele1_eRegrInput_eLeft_s-ele1_eRegrInput_eLeft_r);

   DistributionStreamMap["ele1_eRegrInput_eRight"]->Fill(ele1_eRegrInput_eRight_s);
   DistributionRecoMap["ele1_eRegrInput_eRight"]->Fill(ele1_eRegrInput_eRight_r);
   DifferenceMap["ele1_eRegrInput_eRight"]->Fill(ele1_eRegrInput_eRight_s-ele1_eRegrInput_eRight_r);

   DistributionStreamMap["ele1_eRegrInput_e2x5Max"]->Fill(ele1_eRegrInput_e2x5Max_s);
   DistributionRecoMap["ele1_eRegrInput_e2x5Max"]->Fill(ele1_eRegrInput_e2x5Max_r);
   DifferenceMap["ele1_eRegrInput_e2x5Max"]->Fill(ele1_eRegrInput_e2x5Max_s-ele1_eRegrInput_e2x5Max_r);

   DistributionStreamMap["ele1_eRegrInput_e2x5Top"]->Fill(ele1_eRegrInput_e2x5Top_s);
   DistributionRecoMap["ele1_eRegrInput_e2x5Top"]->Fill(ele1_eRegrInput_e2x5Top_r);
   DifferenceMap["ele1_eRegrInput_e2x5Top"]->Fill(ele1_eRegrInput_e2x5Top_s-ele1_eRegrInput_e2x5Top_r);

   DistributionStreamMap["ele1_eRegrInput_e2x5Left"]->Fill(ele1_eRegrInput_e2x5Left_s);
   DistributionRecoMap["ele1_eRegrInput_e2x5Left"]->Fill(ele1_eRegrInput_e2x5Left_r);
   DifferenceMap["ele1_eRegrInput_e2x5Left"]->Fill(ele1_eRegrInput_e2x5Left_s-ele1_eRegrInput_e2x5Left_r);

   DistributionStreamMap["ele1_eRegrInput_e2x5Right"]->Fill(ele1_eRegrInput_e2x5Right_s);
   DistributionRecoMap["ele1_eRegrInput_e2x5Right"]->Fill(ele1_eRegrInput_e2x5Right_r);
   DifferenceMap["ele1_eRegrInput_e2x5Right"]->Fill(ele1_eRegrInput_e2x5Right_s-ele1_eRegrInput_e2x5Right_r);

   DistributionStreamMap["ele1_eRegrInput_seed_etaCrySeed"]->Fill(ele1_eRegrInput_seed_etaCrySeed_s);
   DistributionRecoMap["ele1_eRegrInput_seed_etaCrySeed"]->Fill(ele1_eRegrInput_seed_etaCrySeed_r);
   DifferenceMap["ele1_eRegrInput_seed_etaCrySeed"]->Fill(ele1_eRegrInput_seed_etaCrySeed_s-ele1_eRegrInput_seed_etaCrySeed_r);

   DistributionStreamMap["ele1_eRegrInput_seed_phiCrySeed"]->Fill(ele1_eRegrInput_seed_phiCrySeed_s);
   DistributionRecoMap["ele1_eRegrInput_seed_phiCrySeed"]->Fill(ele1_eRegrInput_seed_phiCrySeed_r);
   DifferenceMap["ele1_eRegrInput_seed_phiCrySeed"]->Fill(ele1_eRegrInput_seed_phiCrySeed_s-ele1_eRegrInput_seed_phiCrySeed_r);

   DistributionStreamMap["ele1_eRegrInput_preshowerEnergyOverRaw"]->Fill(ele1_eRegrInput_preshowerEnergyOverRaw_s);
   DistributionRecoMap["ele1_eRegrInput_preshowerEnergyOverRaw"]->Fill(ele1_eRegrInput_preshowerEnergyOverRaw_r);
   DifferenceMap["ele1_eRegrInput_preshowerEnergyOverRaw"]->Fill(ele1_eRegrInput_preshowerEnergyOverRaw_s-ele1_eRegrInput_preshowerEnergyOverRaw_r);

   DistributionStreamMap["ele1_eRegrInput_eSubClusters"]->Fill(ele1_eRegrInput_eSubClusters_s);
   DistributionRecoMap["ele1_eRegrInput_eSubClusters"]->Fill(ele1_eRegrInput_eSubClusters_r);
   DifferenceMap["ele1_eRegrInput_eSubClusters"]->Fill(ele1_eRegrInput_eSubClusters_s-ele1_eRegrInput_eSubClusters_r);

   DistributionStreamMap["ele1_eRegrInput_subClusterEnergy_1"]->Fill(ele1_eRegrInput_subClusterEnergy_1_s);
   DistributionRecoMap["ele1_eRegrInput_subClusterEnergy_1"]->Fill(ele1_eRegrInput_subClusterEnergy_1_r);
   DifferenceMap["ele1_eRegrInput_subClusterEnergy_1"]->Fill(ele1_eRegrInput_subClusterEnergy_1_s-ele1_eRegrInput_subClusterEnergy_1_r);

   DistributionStreamMap["ele1_eRegrInput_subClusterEnergy_2"]->Fill(ele1_eRegrInput_subClusterEnergy_2_s);
   DistributionRecoMap["ele1_eRegrInput_subClusterEnergy_2"]->Fill(ele1_eRegrInput_subClusterEnergy_2_r);
   DifferenceMap["ele1_eRegrInput_subClusterEnergy_2"]->Fill(ele1_eRegrInput_subClusterEnergy_2_s-ele1_eRegrInput_subClusterEnergy_2_r);

   DistributionStreamMap["ele1_eRegrInput_subClusterEnergy_3"]->Fill(ele1_eRegrInput_subClusterEnergy_3_s);
   DistributionRecoMap["ele1_eRegrInput_subClusterEnergy_3"]->Fill(ele1_eRegrInput_subClusterEnergy_3_r);
   DifferenceMap["ele1_eRegrInput_subClusterEnergy_3"]->Fill(ele1_eRegrInput_subClusterEnergy_3_s-ele1_eRegrInput_subClusterEnergy_3_r);

   DistributionStreamMap["ele1_eRegrInput_subClusterEta_1"]->Fill(ele1_eRegrInput_subClusterEta_1_s);
   DistributionRecoMap["ele1_eRegrInput_subClusterEta_1"]->Fill(ele1_eRegrInput_subClusterEta_1_r);
   DifferenceMap["ele1_eRegrInput_subClusterEta_1"]->Fill(ele1_eRegrInput_subClusterEta_1_s-ele1_eRegrInput_subClusterEta_1_r);

   DistributionStreamMap["ele1_eRegrInput_subClusterEta_2"]->Fill(ele1_eRegrInput_subClusterEta_2_s);
   DistributionRecoMap["ele1_eRegrInput_subClusterEta_2"]->Fill(ele1_eRegrInput_subClusterEta_2_r);
   DifferenceMap["ele1_eRegrInput_subClusterEta_2"]->Fill(ele1_eRegrInput_subClusterEta_2_s-ele1_eRegrInput_subClusterEta_2_r);

   DistributionStreamMap["ele1_eRegrInput_subClusterEta_3"]->Fill(ele1_eRegrInput_subClusterEta_3_s);
   DistributionRecoMap["ele1_eRegrInput_subClusterEta_3"]->Fill(ele1_eRegrInput_subClusterEta_3_r);
   DifferenceMap["ele1_eRegrInput_subClusterEta_3"]->Fill(ele1_eRegrInput_subClusterEta_3_s-ele1_eRegrInput_subClusterEta_3_r);

   DistributionStreamMap["ele1_eRegrInput_subClusterPhi_1"]->Fill(ele1_eRegrInput_subClusterPhi_1_s);
   DistributionRecoMap["ele1_eRegrInput_subClusterPhi_1"]->Fill(ele1_eRegrInput_subClusterPhi_1_r);
   DifferenceMap["ele1_eRegrInput_subClusterPhi_1"]->Fill(ele1_eRegrInput_subClusterPhi_1_s-ele1_eRegrInput_subClusterPhi_1_r);

   DistributionStreamMap["ele1_eRegrInput_subClusterPhi_2"]->Fill(ele1_eRegrInput_subClusterPhi_2_s);
   DistributionRecoMap["ele1_eRegrInput_subClusterPhi_2"]->Fill(ele1_eRegrInput_subClusterPhi_2_r);
   DifferenceMap["ele1_eRegrInput_subClusterPhi_2"]->Fill(ele1_eRegrInput_subClusterPhi_2_s-ele1_eRegrInput_subClusterPhi_2_r);

   DistributionStreamMap["ele1_eRegrInput_subClusterPhi_3"]->Fill(ele1_eRegrInput_subClusterPhi_3_s);
   DistributionRecoMap["ele1_eRegrInput_subClusterPhi_3"]->Fill(ele1_eRegrInput_subClusterPhi_3_r);
   DifferenceMap["ele1_eRegrInput_subClusterPhi_3"]->Fill(ele1_eRegrInput_subClusterPhi_3_s-ele1_eRegrInput_subClusterPhi_3_r);

   DistributionStreamMap["ele1_eRegrInput_subClusterEmax_1"]->Fill(ele1_eRegrInput_subClusterEmax_1_s);
   DistributionRecoMap["ele1_eRegrInput_subClusterEmax_1"]->Fill(ele1_eRegrInput_subClusterEmax_1_r);
   DifferenceMap["ele1_eRegrInput_subClusterEmax_1"]->Fill(ele1_eRegrInput_subClusterEmax_1_s-ele1_eRegrInput_subClusterEmax_1_r);

   DistributionStreamMap["ele1_eRegrInput_subClusterEmax_2"]->Fill(ele1_eRegrInput_subClusterEmax_2_s);
   DistributionRecoMap["ele1_eRegrInput_subClusterEmax_2"]->Fill(ele1_eRegrInput_subClusterEmax_2_r);
   DifferenceMap["ele1_eRegrInput_subClusterEmax_2"]->Fill(ele1_eRegrInput_subClusterEmax_2_s-ele1_eRegrInput_subClusterEmax_2_r);

   DistributionStreamMap["ele1_eRegrInput_subClusterEmax_3"]->Fill(ele1_eRegrInput_subClusterEmax_3_s);
   DistributionRecoMap["ele1_eRegrInput_subClusterEmax_3"]->Fill(ele1_eRegrInput_subClusterEmax_3_r);
   DifferenceMap["ele1_eRegrInput_subClusterEmax_3"]->Fill(ele1_eRegrInput_subClusterEmax_3_s-ele1_eRegrInput_subClusterEmax_3_r);

   DistributionStreamMap["ele1_eRegrInput_subClusterE3x3_1"]->Fill(ele1_eRegrInput_subClusterE3x3_1_s);
   DistributionRecoMap["ele1_eRegrInput_subClusterE3x3_1"]->Fill(ele1_eRegrInput_subClusterE3x3_1_r);
   DifferenceMap["ele1_eRegrInput_subClusterE3x3_1"]->Fill(ele1_eRegrInput_subClusterE3x3_1_s-ele1_eRegrInput_subClusterE3x3_1_r);

   DistributionStreamMap["ele1_eRegrInput_subClusterE3x3_2"]->Fill(ele1_eRegrInput_subClusterE3x3_2_s);
   DistributionRecoMap["ele1_eRegrInput_subClusterE3x3_2"]->Fill(ele1_eRegrInput_subClusterE3x3_2_r);
   DifferenceMap["ele1_eRegrInput_subClusterE3x3_2"]->Fill(ele1_eRegrInput_subClusterE3x3_2_s-ele1_eRegrInput_subClusterE3x3_2_r);

   DistributionStreamMap["ele1_eRegrInput_subClusterE3x3_3"]->Fill(ele1_eRegrInput_subClusterE3x3_3_s);
   DistributionRecoMap["ele1_eRegrInput_subClusterE3x3_3"]->Fill(ele1_eRegrInput_subClusterE3x3_3_r);
   DifferenceMap["ele1_eRegrInput_subClusterE3x3_3"]->Fill(ele1_eRegrInput_subClusterE3x3_3_s-ele1_eRegrInput_subClusterE3x3_3_r);

   DistributionStreamMap["ele1_eRegrInput_eESClusters"]->Fill(ele1_eRegrInput_eESClusters_s);
   DistributionRecoMap["ele1_eRegrInput_eESClusters"]->Fill(ele1_eRegrInput_eESClusters_r);
   DifferenceMap["ele1_eRegrInput_eESClusters"]->Fill(ele1_eRegrInput_eESClusters_s-ele1_eRegrInput_eESClusters_r);
   /*
   DistributionStreamMap["ele1_eRegrInput_eESClusterEnergy_1"]->Fill(ele1_eRegrInput_eESClusterEnergy_1_s);
   DistributionRecoMap["ele1_eRegrInput_eESClusterEnergy_1"]->Fill(ele1_eRegrInput_eESClusterEnergy_1_r);
   DifferenceMap["ele1_eRegrInput_eESClusterEnergy_1"]->Fill(ele1_eRegrInput_eESClusterEnergy_1_s-ele1_eRegrInput_eESClusterEnergy_1_r);

   DistributionStreamMap["ele1_eRegrInput_eESClusterEnergy_2"]->Fill(ele1_eRegrInput_eESClusterEnergy_2_s);
   DistributionRecoMap["ele1_eRegrInput_eESClusterEnergy_2"]->Fill(ele1_eRegrInput_eESClusterEnergy_2_r);
   DifferenceMap["ele1_eRegrInput_eESClusterEnergy_2"]->Fill(ele1_eRegrInput_eESClusterEnergy_2_s-ele1_eRegrInput_eESClusterEnergy_2_r);

   DistributionStreamMap["ele1_eRegrInput_eESClusterEnergy_3"]->Fill(ele1_eRegrInput_eESClusterEnergy_3_s);
   DistributionRecoMap["ele1_eRegrInput_eESClusterEnergy_3"]->Fill(ele1_eRegrInput_eESClusterEnergy_3_r);
   DifferenceMap["ele1_eRegrInput_eESClusterEnergy_3"]->Fill(ele1_eRegrInput_eESClusterEnergy_3_s-ele1_eRegrInput_eESClusterEnergy_3_r);

   DistributionStreamMap["ele1_eRegrInput_eESClusterEta_1"]->Fill(ele1_eRegrInput_eESClusterEta_1_s);
   DistributionRecoMap["ele1_eRegrInput_eESClusterEta_1"]->Fill(ele1_eRegrInput_eESClusterEta_1_r);
   DifferenceMap["ele1_eRegrInput_eESClusterEta_1"]->Fill(ele1_eRegrInput_eESClusterEta_1_s-ele1_eRegrInput_eESClusterEta_1_r);

   DistributionStreamMap["ele1_eRegrInput_eESClusterEta_2"]->Fill(ele1_eRegrInput_eESClusterEta_2_s);
   DistributionRecoMap["ele1_eRegrInput_eESClusterEta_2"]->Fill(ele1_eRegrInput_eESClusterEta_2_r);
   DifferenceMap["ele1_eRegrInput_eESClusterEta_2"]->Fill(ele1_eRegrInput_eESClusterEta_2_s-ele1_eRegrInput_eESClusterEta_2_r);

   DistributionStreamMap["ele1_eRegrInput_eESClusterEta_3"]->Fill(ele1_eRegrInput_eESClusterEta_3_s);
   DistributionRecoMap["ele1_eRegrInput_eESClusterEta_3"]->Fill(ele1_eRegrInput_eESClusterEta_3_r);
   DifferenceMap["ele1_eRegrInput_eESClusterEta_3"]->Fill(ele1_eRegrInput_eESClusterEta_3_s-ele1_eRegrInput_eESClusterEta_3_r);

   DistributionStreamMap["ele1_eRegrInput_eESClusterPhi_1"]->Fill(ele1_eRegrInput_eESClusterPhi_1_s);
   DistributionRecoMap["ele1_eRegrInput_eESClusterPhi_1"]->Fill(ele1_eRegrInput_eESClusterPhi_1_r);
   DifferenceMap["ele1_eRegrInput_eESClusterPhi_1"]->Fill(ele1_eRegrInput_eESClusterPhi_1_s-ele1_eRegrInput_eESClusterPhi_1_r);

   DistributionStreamMap["ele1_eRegrInput_eESClusterPhi_2"]->Fill(ele1_eRegrInput_eESClusterPhi_2_s);
   DistributionRecoMap["ele1_eRegrInput_eESClusterPhi_2"]->Fill(ele1_eRegrInput_eESClusterPhi_2_r);
   DifferenceMap["ele1_eRegrInput_eESClusterPhi_2"]->Fill(ele1_eRegrInput_eESClusterPhi_2_s-ele1_eRegrInput_eESClusterPhi_2_r);

   DistributionStreamMap["ele1_eRegrInput_eESClusterPhi_3"]->Fill(ele1_eRegrInput_eESClusterPhi_3_s);
   DistributionRecoMap["ele1_eRegrInput_eESClusterPhi_3"]->Fill(ele1_eRegrInput_eESClusterPhi_3_r);
   DifferenceMap["ele1_eRegrInput_eESClusterPhi_3"]->Fill(ele1_eRegrInput_eESClusterPhi_3_s-ele1_eRegrInput_eESClusterPhi_3_r);
   */


   CorrelationMap["rho"]->Fill(rho_s,rho_r);
   CorrelationMap["met"]->Fill(met_s,met_r);
   CorrelationMap["ele1Met_mt"]->Fill(ele1Met_mt_s,ele1Met_mt_r);
   CorrelationMap["PV_n"]->Fill(PV_n_s,PV_n_r);
   CorrelationMap["ele1_hadIso"]->Fill(ele1_hadIso_s,ele1_hadIso_r);
   CorrelationMap["ele1_tkIso"]->Fill(ele1_tkIso_s,ele1_tkIso_r);
   CorrelationMap["ele1_emIso"]->Fill(ele1_emIso_s,ele1_emIso_r);
   CorrelationMap["ele1_dxy_PV"]->Fill(ele1_dxy_PV_s,ele1_dxy_PV_r);
   CorrelationMap["ele1_dz_PV"]->Fill(ele1_dz_PV_s,ele1_dz_PV_r);

  }

 }
 
 
 // save in output root files
 TFile* outputFile = new TFile("outputPlots.root","RECREATE");
 outputFile->cd();
 
 TDirectory *streamPlots = outputFile->mkdir("streamVariables");   
 streamPlots->cd();

 std::map<std::string,TH1F*>::iterator itMap = DistributionStreamMap.begin(); 
 for( ; itMap != DistributionStreamMap.end() ; ++itMap ) {
  itMap->second->Write();
 }

 TDirectory *recoPlots = outputFile->mkdir("recoVariables");   
 recoPlots->cd();

 itMap = DistributionRecoMap.begin();
 for( ; itMap != DistributionRecoMap.end() ; ++itMap ) {
  itMap->second->Write();
 }

 TDirectory *comparePlots = outputFile->mkdir("VaribleComparison");   
 comparePlots->cd();

 itMap = DifferenceMap.begin();
 for( ; itMap != DifferenceMap.end() ; ++itMap ) {
  itMap->second->Write();
 }

 outputFile->Close();
 
 // save canvas
 system("mkdir plots");
 system("mkdir plots_log");
 system("mkdir plots_correlation");
 system("rm plots/*"); 
 system("rm plots_log/*"); 
 system("rm plots_correlation/*"); 
  
 TLegend* leg = new TLegend(0.5,0.75,0.8,0.95);

 std::map<std::string,TH1F*>::iterator itMap1 = DistributionStreamMap.begin();
 std::map<std::string,TH1F*>::iterator itMap2 = DistributionRecoMap.begin();

 for( ; itMap1 != DistributionStreamMap.end() and itMap2 != DistributionRecoMap.end() ; ++itMap1, ++itMap2){

  TString tempName = Form ("Comparison_%s",(itMap1)->first.c_str());
  TCanvas* canvasTemp = new TCanvas(tempName.Data(),"",600,650);
  canvasTemp->cd();
  (itMap1)->second->GetXaxis()->SetTitle(((itMap1)->first).c_str());
  (itMap1)->second->GetYaxis()->SetTitle("Events");
  (itMap1)->second->SetMaximum(std::max((itMap1)->second->GetMaximum(),(itMap2)->second->GetMaximum())*1.25);
  (itMap1)->second->SetMarkerColor(kBlack);
  (itMap1)->second->SetLineColor(kBlack);
  (itMap1)->second->SetLineWidth(2);
  (itMap1)->second->SetMarkerStyle(20);
  (itMap1)->second->SetMarkerSize(1);
  (itMap1)->second->Draw();
  gPad->Update();
  TPaveStats *tps1 = (TPaveStats*) (itMap1)->second->FindObject("stats");
  tps1->SetTextColor(kBlack);
  tps1->SetLineColor(kBlack);
  double X1 = tps1->GetX1NDC();
  double Y1 = tps1->GetY1NDC();
  double X2 = tps1->GetX2NDC();
  double Y2 = tps1->GetY2NDC();
 
  (itMap2)->second->GetXaxis()->SetTitle(((itMap2)->first).c_str());
  (itMap2)->second->GetYaxis()->SetTitle("Events");
  (itMap2)->second->SetMarkerColor(kRed);
  (itMap2)->second->SetLineColor(kRed);
  (itMap2)->second->SetMarkerStyle(20);
  (itMap2)->second->SetMarkerSize(1);
  (itMap2)->second->Draw("E");
  gPad->Update();
  (itMap1)->second->Draw();
  gPad->Update();
  (itMap2)->second->Draw("Esame");
  gPad->Update();
  TPaveStats *tps2 = (TPaveStats*) (itMap2)->second->FindObject("stats");
  tps2->SetTextColor(kRed);
  tps2->SetLineColor(kRed);
  tps2->SetX1NDC(X1);
  tps2->SetX2NDC(X2);
  tps2->SetY1NDC(Y1-(Y2-Y1));
  tps2->SetY2NDC(Y1);
  
  tps1->Draw("same");
  tps2->Draw("same");
  
  leg->SetFillColor(0);
  leg->SetFillStyle(0);
  leg->SetBorderSize(0);
  leg->SetLineColor(0);
  leg->SetLineWidth(0);
  leg->SetLineStyle(0);
  leg->AddEntry((itMap1)->second->GetName(),"Electron Stream","pl");
  leg->AddEntry((itMap2)->second->GetName(),"Standard Reco","pl");
  leg->Draw("same");

  // chi2 and KS test for compatibility
  double chi2    = itMap1->second->Chi2Test((itMap2)->second,"UU CHI2/NDF");
  double KS_test = itMap1->second->KolmogorovTest((itMap2)->second,"UO");

  TString probatext  = Form("#chi^{2}/ndf = %0.2f  K_{s} = %0.2f",float(chi2),float(KS_test));
  TLatex* tt         = new TLatex(0.2,0.85,probatext);
  tt->SetNDC();
  tt->SetTextSize(0.025);
  tt->AppendPad("same");

  canvasTemp->SaveAs(("plots/Comparison_"+(itMap1)->first+".pdf").c_str(),"pdf");
  canvasTemp->SaveAs(("plots/Comparison_"+(itMap1)->first+".png").c_str(),"png");
 
  (itMap1)->second->SetMaximum(std::max((itMap1)->second->GetMaximum(),(itMap2)->second->GetMaximum())*100);
  canvasTemp->SetLogy();
  gPad->Update();

  canvasTemp->SaveAs(("plots_log/Comparison_Log_"+(itMap1)->first+".pdf").c_str(),"pdf");
  canvasTemp->SaveAs(("plots_log/Comparison_Log_"+(itMap1)->first+".png").c_str(),"png");
 
  leg->Clear();

  delete tps1;
  delete tps2;
  delete canvasTemp;
  delete tt ;  
 }
 
 std::map<std::string,TH1F*>::iterator itMap3 = DifferenceMap.begin();

 for( ; itMap3 != DifferenceMap.end() ; ++itMap3){

  TString tempName = Form ("Difference_%s",(itMap3)->first.c_str());
  TCanvas* canvasTempDiff = new TCanvas(tempName.Data(),"",600,650);
  canvasTempDiff->cd();
  (itMap3)->second->GetXaxis()->SetTitle(("Difference "+(itMap3)->first).c_str());
  (itMap3)->second->GetYaxis()->SetTitle("Events");
  (itMap3)->second->SetMarkerColor(kBlack);
  (itMap3)->second->SetMarkerStyle(20);
  (itMap3)->second->SetMarkerSize(1);
  (itMap3)->second->Draw("E");
  canvasTempDiff->SaveAs(("plots/Difference_"+(itMap3)->first+".pdf").c_str(),"pdf"); 
  canvasTempDiff->SaveAs(("plots/Difference_"+(itMap3)->first+".png").c_str(),"png"); 

  canvasTempDiff->SetLogy();
  gPad->Update();

  canvasTempDiff->SaveAs(("plots_log/Difference_Log_"+(itMap3)->first+".pdf").c_str(),"pdf");
  canvasTempDiff->SaveAs(("plots_log/Difference_Log_"+(itMap3)->first+".png").c_str(),"png");
 
  delete canvasTempDiff;
 }

 std::map<std::string,TH2F*>::iterator itMap4 = CorrelationMap.begin();

 for( ; itMap4 != CorrelationMap.end() ; ++itMap4){

  TString tempName = Form ("Correlation_%s",(itMap4)->first.c_str());
  TCanvas* canvasTempCorr = new TCanvas(tempName.Data(),"",600,650);
  canvasTempCorr->cd();
  (itMap4)->second->GetXaxis()->SetTitle(((itMap4)->first+" stream").c_str());
  (itMap4)->second->GetYaxis()->SetTitle(((itMap4)->first+" reco").c_str());
  (itMap4)->second->SetMarkerColor(kBlack);
  (itMap4)->second->SetMarkerStyle(20);
  (itMap4)->second->SetMarkerSize(1);
  (itMap4)->second->Draw("colz");
  canvasTempCorr->SaveAs(("plots_correlation/Correlation_"+(itMap4)->first+".pdf").c_str(),"pdf"); 
  canvasTempCorr->SaveAs(("plots_correlation/Correlation_"+(itMap4)->first+".png").c_str(),"png"); 

  delete canvasTempCorr;
 }
 

 // deallocate objects 
 DistributionStreamMap.clear();
 DistributionRecoMap.clear();
 DifferenceMap.clear();
 CorrelationMap.clear();

 return 0 ;


}

void SetVariablesMapEntry(std::map<std::string,TH1F*> & DistributionStreamMap, std::map<std::string,TH1F*> & DistributionRecoMap, std::map<std::string,TH1F*> & DifferenceMap){

 DistributionStreamMap.insert(mapElement("PV_n",new TH1F("PV_nStream","",40,0,40)));       
 DistributionRecoMap.insert(mapElement("PV_n",new TH1F("PV_nReco","",40,0,40)));       
 DifferenceMap.insert(mapElement("PV_n",new TH1F("PV_nRatio","",80,-40,40)));       
 
 DistributionStreamMap.insert(mapElement("rho", new TH1F("rhoStream","",50,0,50)));       
 DistributionRecoMap.insert(mapElement("rho", new TH1F("rhoReco","",50,0,50)));       
 DifferenceMap.insert(mapElement("rho", new TH1F("rhoRatio","",40,-15,15)));       

 DistributionStreamMap.insert(mapElement("met", new TH1F("met_Stream","",50,0,200)));       
 DistributionRecoMap.insert(mapElement("met", new TH1F("met_Reco","",50,0,200)));       
 DifferenceMap.insert(mapElement("met", new TH1F("met_Ratio","",200,-50,150)));       

 DistributionStreamMap.insert(mapElement("met_phi", new TH1F("met_phiStream","",50,-3.14,3.14)));       
 DistributionRecoMap.insert(mapElement("met_phi", new TH1F("met_phiReco","",50,-3.14,3.14)));       
 DifferenceMap.insert(mapElement("met_phi", new TH1F("met_phiRatio","",100,-3.14,3.14)));       

 DistributionStreamMap.insert(mapElement("ele1Met_mt", new TH1F("ele1Met_mtStream","",100,0,200)));       
 DistributionRecoMap.insert(mapElement("ele1Met_mt", new TH1F("ele1Met_mtReco","",100,0,200)));       
 DifferenceMap.insert(mapElement("ele1Met_mt", new TH1F("ele1Met_mtRatio","",100,-25,25)));       
 
 // integer variables
 DistributionStreamMap.insert(mapElement("ele1_seedIeta", new TH1F("ele1_seedIetaStream","",50,-85,85)));       
 DistributionRecoMap.insert(mapElement("ele1_seedIeta", new TH1F("ele1_seedIetaReco","",50,-85,85)));       
 DifferenceMap.insert(mapElement("ele1_seedIeta", new TH1F("ele1_seedIetaRatio","",25,-25,25)));       

 DistributionStreamMap.insert(mapElement("ele1_seedIphi", new TH1F("ele1_seedIphiStream","",50,0,360)));       
 DistributionRecoMap.insert(mapElement("ele1_seedIphi", new TH1F("ele1_seedIphiReco","",50,0,360)));       
 DifferenceMap.insert(mapElement("ele1_seedIphi", new TH1F("ele1_seedIphiRatio","",20,-20,20)));       

 DistributionStreamMap.insert(mapElement("ele1_seedIx", new TH1F("ele1_seedIxStream","",101,0,101)));       
 DistributionRecoMap.insert(mapElement("ele1_seedIx", new TH1F("ele1_seedIxReco","",101,0,101)));       
 DifferenceMap.insert(mapElement("ele1_seedIx", new TH1F("ele1_seedIxRatio","",15,-25,25)));       

 DistributionStreamMap.insert(mapElement("ele1_seedIy", new TH1F("ele1_seedIyStream","",101,0,101)));       
 DistributionRecoMap.insert(mapElement("ele1_seedIy", new TH1F("ele1_seedIyReco","",101,0,101)));       
 DifferenceMap.insert(mapElement("ele1_seedIy", new TH1F("ele1_seedIyRatio","",25,-25,25)));       
 
 DistributionStreamMap.insert(mapElement("ele1_scNxtal", new TH1F("ele1_scNxtalStream","",90,0,90)));       
 DistributionRecoMap.insert(mapElement("ele1_scNxtal", new TH1F("ele1_scNxtalReco","",90,0,90)));       
 DifferenceMap.insert(mapElement("ele1_scNxtal", new TH1F("ele1_scNxtalRatio","",50,-25,25)));       

 DistributionStreamMap.insert(mapElement("ele1_nRecHits", new TH1F("ele1_nRecHitsStream","",90,0,90)));       
 DistributionRecoMap.insert(mapElement("ele1_nRecHits", new TH1F("ele1_nRecHitsReco","",90,0,90)));       
 DifferenceMap.insert(mapElement("ele1_nRecHits", new TH1F("ele1_nRecHitsRatio","",30,-15,15)));       

 DistributionStreamMap.insert(mapElement("ele1_nGgsfTrackHits", new TH1F("ele1_nGgsfTrackHitsStream","",50,0,50)));       
 DistributionRecoMap.insert(mapElement("ele1_nGgsfTrackHits", new TH1F("ele1_nGgsfTrackReco","",50,0,50)));       
 DifferenceMap.insert(mapElement("ele1_nGgsfTrackHits", new TH1F("ele1_nGgsfTrackHitsRatio","",20,-10,10)));       

 DistributionStreamMap.insert(mapElement("ele1_numberOfLostHits", new TH1F("ele1_numberOfLostHitsStream","",8,0,8)));       
 DistributionRecoMap.insert(mapElement("ele1_numberOfLostHits", new TH1F("ele1_numberOfLostHitsReco","",8,0,8)));       
 DifferenceMap.insert(mapElement("ele1_numberOfLostHits", new TH1F("ele1_numberOfLostHitsRatio","",10,-5,5)));       

 DistributionStreamMap.insert(mapElement("ele1_nAmbiguousGsfTrack", new TH1F("ele1_nAmbiguousGsfTrackStream","",20,0,20)));       
 DistributionRecoMap.insert(mapElement("ele1_nAmbiguousGsfTrack", new TH1F("ele1_nAmbiguousGsfTrackReco","",20,0,20)));       
 DifferenceMap.insert(mapElement("ele1_nAmbiguousGsfTrack", new TH1F("ele1_nAmbiguousGsfTrackRatio","",20,-10,10)));       
 
 // electron variables

 DistributionStreamMap.insert(mapElement("ele1_pt", new TH1F("ele1_ptStream","",40,0,200)));       
 DistributionRecoMap.insert(mapElement("ele1_pt", new TH1F("ele1_ptReco","",40,0,200)));       
 DifferenceMap.insert(mapElement("ele1_pt", new TH1F("ele1_ptRatio","",200,-1.5,1.5)));       

 DistributionStreamMap.insert(mapElement("ele1_eta", new TH1F("ele1_etaStream","",50,-3,3)));       
 DistributionRecoMap.insert(mapElement("ele1_eta", new TH1F("ele1_etaReco","",50,-3,3)));       
 DifferenceMap.insert(mapElement("ele1_eta", new TH1F("ele1_etaRatio","",100,-0.5,0.5)));       

 DistributionStreamMap.insert(mapElement("ele1_phi", new TH1F("ele1_phiStream","",50,-3.14,3.14)));       
 DistributionRecoMap.insert(mapElement("ele1_phi", new TH1F("ele1_phiReco","",50,-3.14,3.14)));       
 DifferenceMap.insert(mapElement("ele1_phi", new TH1F("ele1_phiRatio","",100,-0.5,.5)));       

 DistributionStreamMap.insert(mapElement("ele1_sigmaIetaIeta", new TH1F("ele1_sigmaIetaIetaStream","",50,0,0.1)));       
 DistributionRecoMap.insert(mapElement("ele1_sigmaIetaIeta", new TH1F("ele1_sigmaIetaIetaReco","",50,0,0.1)));       
 DifferenceMap.insert(mapElement("ele1_sigmaIetaIeta", new TH1F("ele1_sigmaIetaIetaRatio","",50,-0.1,0.1)));       

 DistributionStreamMap.insert(mapElement("ele1_DphiIn", new TH1F("ele1_DphiInStream","",50,-0.012,0.012)));       
 DistributionRecoMap.insert(mapElement("ele1_DphiIn", new TH1F("ele1_DphiInReco","",50,-0.012,0.012)));       
 DifferenceMap.insert(mapElement("ele1_DphiIn", new TH1F("ele1_DphiInRatio","",50,-0.0013,0.0013)));       

 DistributionStreamMap.insert(mapElement("ele1_DetaIn", new TH1F("ele1_DetaInStream","",50,-0.007,0.007)));       
 DistributionRecoMap.insert(mapElement("ele1_DetaIn", new TH1F("ele1_DetaInReco","",50,-0.007,0.007)));       
 DifferenceMap.insert(mapElement("ele1_DetaIn", new TH1F("ele1_DetaInRatio","",50,-0.0008,0.0008)));       

 DistributionStreamMap.insert(mapElement("ele1_HOverE", new TH1F("ele1_HOverEStream","",50,0,0.05)));       
 DistributionRecoMap.insert(mapElement("ele1_HOverE", new TH1F("ele1_HOverEReco","",50,0,0.05)));       
 DifferenceMap.insert(mapElement("ele1_HOverE", new TH1F("ele1_HOverERatio","",50,-0.08,0.08)));       

 DistributionStreamMap.insert(mapElement("ele1_tkIso", new TH1F("ele1_tkIsoStream","",50,0,10)));       
 DistributionRecoMap.insert(mapElement("ele1_tkIso", new TH1F("ele1_tkIsoReco","",50,0,10)));       
 DifferenceMap.insert(mapElement("ele1_tkIso", new TH1F("ele1_tkIsoRatio","",150,-7,7)));       

 DistributionStreamMap.insert(mapElement("ele1_emIso", new TH1F("ele1_emIsoStream","",50,0,10)));       
 DistributionRecoMap.insert(mapElement("ele1_emIso", new TH1F("ele1_emIsoReco","",50,0,10)));       
 DifferenceMap.insert(mapElement("ele1_emIso", new TH1F("ele1_emIsoRatio","",100,-8,5)));       

 DistributionStreamMap.insert(mapElement("ele1_hadIso", new TH1F("ele1_hadIsoStream","",50,0,10)));       
 DistributionRecoMap.insert(mapElement("ele1_hadIso", new TH1F("ele1_hadIsoReco","",50,0,10)));       
 DifferenceMap.insert(mapElement("ele1_hadIso", new TH1F("ele1_hadIsoRatio","",100,-5,5)));       
 
 DistributionStreamMap.insert(mapElement("ele1_scE", new TH1F("ele1_scEStream","",50,30,300)));       
 DistributionRecoMap.insert(mapElement("ele1_scE", new TH1F("ele1_scEReco","",50,30,300)));       
 DifferenceMap.insert(mapElement("ele1_scE", new TH1F("ele1_scERatio","",150,-5,5)));       

 DistributionStreamMap.insert(mapElement("ele1_es", new TH1F("ele1_esStream","",30,0,60)));       
 DistributionRecoMap.insert(mapElement("ele1_es", new TH1F("ele1_esReco","",30,0,60)));       
 DifferenceMap.insert(mapElement("ele1_es", new TH1F("ele1_esRatio","",50,-10,10)));       
 
 DistributionStreamMap.insert(mapElement("ele1_scEta", new TH1F("ele1_scEtaStream","",50,-2.5,2.5)));       
 DistributionRecoMap.insert(mapElement("ele1_scEta", new TH1F("ele1_scEtaReco","",50,-2.5,2.5)));       
 DifferenceMap.insert(mapElement("ele1_scEta", new TH1F("ele1_scEtaRatio","",100,-0.3,0.3)));       

 DistributionStreamMap.insert(mapElement("ele1_scPhi", new TH1F("ele1_scPhiStream","",50,-3.14,3.14)));       
 DistributionRecoMap.insert(mapElement("ele1_scPhi", new TH1F("ele1_scPhiReco","",50,-3.14,3.14)));       
 DifferenceMap.insert(mapElement("ele1_scPhi", new TH1F("ele1_scPhiRatio","",100,-0.3,0.3)));       

 DistributionStreamMap.insert(mapElement("ele1_scEtaWidth", new TH1F("ele1_scEtaWidthStream","",50,0.,0.2)));       
 DistributionRecoMap.insert(mapElement("ele1_scEtaWidth", new TH1F("ele1_scEtaWidthReco","",50,0.,0.2)));       
 DifferenceMap.insert(mapElement("ele1_scEtaWidth", new TH1F("ele1_scEtaWidthRatio","",50,-0.2,0.2)));       

 DistributionStreamMap.insert(mapElement("ele1_scPhiWidth", new TH1F("ele1_scPhiWidthStream","",50,0,0.5)));       
 DistributionRecoMap.insert(mapElement("ele1_scPhiWidth", new TH1F("ele1_scPhiWidthReco","",50,0,0.5)));       
 DifferenceMap.insert(mapElement("ele1_scPhiWidth", new TH1F("ele1_scPhiWidthRatio","",100,-0.7,0.7)));       

 DistributionStreamMap.insert(mapElement("ele1_tkP", new TH1F("ele1_tkPStream","",50,0,300)));       
 DistributionRecoMap.insert(mapElement("ele1_tkP", new TH1F("ele1_tkPReco","",50,0,300)));       
 DifferenceMap.insert(mapElement("ele1_tkP", new TH1F("ele1_tkPRatio","",150,-0.4,0.4)));       
  
 DistributionStreamMap.insert(mapElement("ele1_fbrem", new TH1F("ele1_fbremStream","",40,-0.4,1)));       
 DistributionRecoMap.insert(mapElement("ele1_fbrem", new TH1F("ele1_fbremReco","",40,-0.4,1)));       
 DifferenceMap.insert(mapElement("ele1_fbrem", new TH1F("ele1_fbremRatio","",150,-0.2,0.2)));       
 
 DistributionStreamMap.insert(mapElement("ele1_dxy_PV", new TH1F("ele1_dxy_PVStream","",50,0,0.5)));       
 DistributionRecoMap.insert(mapElement("ele1_dxy_PV", new TH1F("ele1_dxy_PVReco","",50,0,0.5)));       
 DifferenceMap.insert(mapElement("ele1_dxy_PV", new TH1F("ele1_dxy_PVRatio","",100,-0.5,0.5)));       
 
 DistributionStreamMap.insert(mapElement("ele1_dz_PV", new TH1F("ele1_dz_PVStream","",50,0,20)));       
 DistributionRecoMap.insert(mapElement("ele1_dz_PV", new TH1F("ele1_dz_PVReco","",50,0,20)));       
 DifferenceMap.insert(mapElement("ele1_dz_PV", new TH1F("ele1_dz_PVRatio","",50,-20,20)));       

 DistributionStreamMap.insert(mapElement("ele1_e5x5", new TH1F("ele1_e5x5Stream","",100,0,250)));       
 DistributionRecoMap.insert(mapElement("ele1_e5x5", new TH1F("ele1_e5x5Reco","",100,0,250)));       
 DifferenceMap.insert(mapElement("ele1_e5x5", new TH1F("ele1_e5x5Ratio","",100,-30,30)));       
 
 DistributionStreamMap.insert(mapElement("ele1_e3x3", new TH1F("ele1_e3x3Stream","",100,0,250)));       
 DistributionRecoMap.insert(mapElement("ele1_e3x3", new TH1F("ele1_e3x3Reco","",100,0,250)));       
 DifferenceMap.insert(mapElement("ele1_e3x3", new TH1F("ele1_e3x3Ratio","",100,-30,30)));       
  
 DistributionStreamMap.insert(mapElement("ele1_seedEnergy", new TH1F("ele1_seedEStream","",100,0,200)));       
 DistributionRecoMap.insert(mapElement("ele1_seedEnergy", new TH1F("ele1_seedEReco","",100,0,200)));       
 DifferenceMap.insert(mapElement("ele1_seedEnergy", new TH1F("ele1_seedERatio","",100,-20,20)));       

 // regression variables
 DistributionStreamMap.insert(mapElement("ele1_eRegrInput_r9", new TH1F("ele1_eRegrInput_r9Stream","",30,0.1,1)));       
 DistributionRecoMap.insert(mapElement("ele1_eRegrInput_r9", new TH1F("ele1_eRegrInput_r9Reco","",30,0.1,1)));       
 DifferenceMap.insert(mapElement("ele1_eRegrInput_r9", new TH1F("ele1_eRegrInput_r9Ratio","",50,-0.6,0.6)));       

 DistributionStreamMap.insert(mapElement("ele1_eRegrInput_SCsize", new TH1F("ele1_eRegrInput_SCsizeStream","",25,0,25)));       
 DistributionRecoMap.insert(mapElement("ele1_eRegrInput_SCsize", new TH1F("ele1_eRegrInput_SCsizeReco","",25,0,25)));       
 DifferenceMap.insert(mapElement("ele1_eRegrInput_SCsize", new TH1F("ele1_eRegrInput_SCsizeRatio","",30,-5,5)));       
 
 DistributionStreamMap.insert(mapElement("ele1_eRegrInput_seed_eta", new TH1F("ele1_eRegrInput_seed_etaStream","",50,-2.5,2.5)));       
 DistributionRecoMap.insert(mapElement("ele1_eRegrInput_seed_eta", new TH1F("ele1_eRegrInput_seed_etaReco","",50,-2.5,2.5)));       
 DifferenceMap.insert(mapElement("ele1_eRegrInput_seed_eta", new TH1F("ele1_eRegrInput_seed_etaRatio","",30,-0.5,0.5)));       

 DistributionStreamMap.insert(mapElement("ele1_eRegrInput_seed_phi", new TH1F("ele1_eRegrInput_seed_phiStream","",50,-3.14,3.14)));       
 DistributionRecoMap.insert(mapElement("ele1_eRegrInput_seed_phi", new TH1F("ele1_eRegrInput_seed_phiReco","",50,-3.14,3.14)));       
 DifferenceMap.insert(mapElement("ele1_eRegrInput_seed_phi", new TH1F("ele1_eRegrInput_seed_phiRatio","",30,-0.5,0.5)));       

 DistributionStreamMap.insert(mapElement("ele1_eRegrInput_sigiphiiphi", new TH1F("ele1_eRegrInput_sigiphiiphiStream","",40,0,0.2)));       
 DistributionRecoMap.insert(mapElement("ele1_eRegrInput_sigiphiiphi", new TH1F("ele1_eRegrInput_sigiphiiphiReco","",40,0,0.2)));       
 DifferenceMap.insert(mapElement("ele1_eRegrInput_sigiphiiphi", new TH1F("ele1_eRegrInput_sigiphiiphiRatio","",50,-0.1,0.1)));       

 DistributionStreamMap.insert(mapElement("ele1_eRegrInput_sigietaiphi", new TH1F("ele1_eRegrInput_sigietaiphiStream","",50,0.,1.)));       
 DistributionRecoMap.insert(mapElement("ele1_eRegrInput_sigietaiphi", new TH1F("ele1_eRegrInput_sigietaiphiReco","",50,0.,1.)));       
 DifferenceMap.insert(mapElement("ele1_eRegrInput_sigietaiphi", new TH1F("ele1_eRegrInput_sigietaiphiRatio","",50,-0.4,0.4)));       

 DistributionStreamMap.insert(mapElement("ele1_eRegrInput_eMax", new TH1F("ele1_eRegrInput_eMaxStream","",50,0,150.)));       
 DistributionRecoMap.insert(mapElement("ele1_eRegrInput_eMax", new TH1F("ele1_eRegrInput_eMaxReco","",50,0,150.)));       
 DifferenceMap.insert(mapElement("ele1_eRegrInput_eMax", new TH1F("ele1_eRegrInput_eMaxRatio","",50,-15,15)));       
 
 DistributionStreamMap.insert(mapElement("ele1_eRegrInput_e2nd", new TH1F("ele1_eRegrInput_e2ndStream","",50,0,100.)));       
 DistributionRecoMap.insert(mapElement("ele1_eRegrInput_e2nd", new TH1F("ele1_eRegrInput_e2ndReco","",50,0,100.)));       
 DifferenceMap.insert(mapElement("ele1_eRegrInput_e2nd", new TH1F("ele1_eRegrInput_e2ndRatio","",50,-15,15)));       

 DistributionStreamMap.insert(mapElement("ele1_eRegrInput_eTop", new TH1F("ele1_eRegrInput_eTopStream","",50,0,100.)));       
 DistributionRecoMap.insert(mapElement("ele1_eRegrInput_eTop", new TH1F("ele1_eRegrInput_eTopReco","",50,0,100.)));       
 DifferenceMap.insert(mapElement("ele1_eRegrInput_eTop", new TH1F("ele1_eRegrInput_eTopRatio","",50,-15,15)));       

 DistributionStreamMap.insert(mapElement("ele1_eRegrInput_eBottom", new TH1F("ele1_eRegrInput_eBottomStream","",50,0,100.)));       
 DistributionRecoMap.insert(mapElement("ele1_eRegrInput_eBottom", new TH1F("ele1_eRegrInput_eBottomReco","",50,0,100.)));       
 DifferenceMap.insert(mapElement("ele1_eRegrInput_eBottom", new TH1F("ele1_eRegrInput_eBottomRatio","",50,-15,15)));       

 DistributionStreamMap.insert(mapElement("ele1_eRegrInput_eLeft", new TH1F("ele1_eRegrInput_eLeftStream","",50,0,100.)));       
 DistributionRecoMap.insert(mapElement("ele1_eRegrInput_eLeft", new TH1F("ele1_eRegrInput_eLeftReco","",50,0,100.)));       
 DifferenceMap.insert(mapElement("ele1_eRegrInput_eLeft", new TH1F("ele1_eRegrInput_eLeftRatio","",50,-15,15)));       

 DistributionStreamMap.insert(mapElement("ele1_eRegrInput_eRight", new TH1F("ele1_eRegrInput_eRightStream","",50,0,100.)));       
 DistributionRecoMap.insert(mapElement("ele1_eRegrInput_eRight", new TH1F("ele1_eRegrInput_eRightReco","",50,0,100.)));       
 DifferenceMap.insert(mapElement("ele1_eRegrInput_eRight", new TH1F("ele1_eRegrInput_eRightRatio","",50,-15,15)));       
 
 DistributionStreamMap.insert(mapElement("ele1_eRegrInput_e2x5Max", new TH1F("ele1_eRegrInput_e2x5MaxStream","",50,0,200.)));       
 DistributionRecoMap.insert(mapElement("ele1_eRegrInput_e2x5Max", new TH1F("ele1_eRegrInput_e2x5MaxReco","",50,0,200.)));       
 DifferenceMap.insert(mapElement("ele1_eRegrInput_e2x5Max", new TH1F("ele1_eRegrInput_e2x5MaxRatio","",50,-15,15)));       

 DistributionStreamMap.insert(mapElement("ele1_eRegrInput_e2x5Top", new TH1F("ele1_eRegrInput_e2x5TopStream","",50,0,100.)));       
 DistributionRecoMap.insert(mapElement("ele1_eRegrInput_e2x5Top", new TH1F("ele1_eRegrInput_e2x5TopReco","",50,0,100.)));       
 DifferenceMap.insert(mapElement("ele1_eRegrInput_e2x5Top", new TH1F("ele1_eRegrInput_e2x5TopRatio","",50,-15,15)));       

 DistributionStreamMap.insert(mapElement("ele1_eRegrInput_e2x5Left", new TH1F("ele1_eRegrInput_e2x5LeftStream","",50,0,100.)));       
 DistributionRecoMap.insert(mapElement("ele1_eRegrInput_e2x5Left", new TH1F("ele1_eRegrInput_e2x5LeftReco","",50,0,100.)));       
 DifferenceMap.insert(mapElement("ele1_eRegrInput_e2x5Left", new TH1F("ele1_eRegrInput_e2x5LeftRatio","",50,-15,15)));       

 DistributionStreamMap.insert(mapElement("ele1_eRegrInput_e2x5Right", new TH1F("ele1_eRegrInput_e2x5RightStream","",50,0,100.)));       
 DistributionRecoMap.insert(mapElement("ele1_eRegrInput_e2x5Right", new TH1F("ele1_eRegrInput_e2x5RightReco","",50,0,100.)));       
 DifferenceMap.insert(mapElement("ele1_eRegrInput_e2x5Right", new TH1F("ele1_eRegrInput_e2x5RightRatio","",50,-15,15)));       

 DistributionStreamMap.insert(mapElement("ele1_eRegrInput_seed_etaCrySeed", new TH1F("ele1_eRegrInput_seed_etaCrySeedStream","",50,0,2.)));       
 DistributionRecoMap.insert(mapElement("ele1_eRegrInput_seed_etaCrySeed", new TH1F("ele1_eRegrInput_seed_etaCrySeedReco","",50,0,2.)));       
 DifferenceMap.insert(mapElement("ele1_eRegrInput_seed_etaCrySeed", new TH1F("ele1_eRegrInput_seed_etaCrySeedRatio","",50,-2,2)));       

 DistributionStreamMap.insert(mapElement("ele1_eRegrInput_seed_phiCrySeed", new TH1F("ele1_eRegrInput_seed_phiCrySeedStream","",50,0,3.)));       
 DistributionRecoMap.insert(mapElement("ele1_eRegrInput_seed_phiCrySeed", new TH1F("ele1_eRegrInput_seed_phiCrySeedReco","",50,0,3.)));       
 DifferenceMap.insert(mapElement("ele1_eRegrInput_seed_phiCrySeed", new TH1F("ele1_eRegrInput_seed_phiCrySeedRatio","",50,-2,2)));       

 DistributionStreamMap.insert(mapElement("ele1_eRegrInput_preshowerEnergyOverRaw", new TH1F("ele1_eRegrInput_preshowerEnergyOverRawStream","",40,0,0.4)));       
 DistributionRecoMap.insert(mapElement("ele1_eRegrInput_preshowerEnergyOverRaw", new TH1F("ele1_eRegrInput_preshowerEnergyOverRawReco","",40,0,0.4)));       
 DifferenceMap.insert(mapElement("ele1_eRegrInput_preshowerEnergyOverRaw", new TH1F("ele1_eRegrInput_preshowerEnergyOverRawRatio","",50,-0.3,0.3)));       

 DistributionStreamMap.insert(mapElement("ele1_eRegrInput_eSubClusters", new TH1F("ele1_eRegrInput_eSubClustersStream","",50,0,50.)));       
 DistributionRecoMap.insert(mapElement("ele1_eRegrInput_eSubClusters", new TH1F("ele1_eRegrInput_eSubClustersReco","",50,0,50.)));       
 DifferenceMap.insert(mapElement("ele1_eRegrInput_eSubClusters", new TH1F("ele1_eRegrInput_eSubClustersRatio","",40,-20.,20.)));       

 DistributionStreamMap.insert(mapElement("ele1_eRegrInput_subClusterEnergy_1", new TH1F("ele1_eRegrInput_subClusterEnergy_1Stream","",50,0,50.)));       
 DistributionRecoMap.insert(mapElement("ele1_eRegrInput_subClusterEnergy_1", new TH1F("ele1_eRegrInput_subClusterEnergy_1Reco","",50,0,50.)));       
 DifferenceMap.insert(mapElement("ele1_eRegrInput_subClusterEnergy_1", new TH1F("ele1_eRegrInput_subClusterEnergy_1Ratio","",50,-25.,25.)));       

 DistributionStreamMap.insert(mapElement("ele1_eRegrInput_subClusterEnergy_2", new TH1F("ele1_eRegrInput_subClusterEnergy_2Stream","",30,0,30.)));       
 DistributionRecoMap.insert(mapElement("ele1_eRegrInput_subClusterEnergy_2", new TH1F("ele1_eRegrInput_subClusterEnergy_2Reco","",30,0,30.)));       
 DifferenceMap.insert(mapElement("ele1_eRegrInput_subClusterEnergy_2", new TH1F("ele1_eRegrInput_subClusterEnergy_2Ratio","",40,-20.,20.)));       

 DistributionStreamMap.insert(mapElement("ele1_eRegrInput_subClusterEnergy_3", new TH1F("ele1_eRegrInput_subClusterEnergy_3Stream","",20,0,20.)));       
 DistributionRecoMap.insert(mapElement("ele1_eRegrInput_subClusterEnergy_3", new TH1F("ele1_eRegrInput_subClusterEnergy_3Reco","",20,0,20.)));       
 DifferenceMap.insert(mapElement("ele1_eRegrInput_subClusterEnergy_3", new TH1F("ele1_eRegrInput_subClusterEnergy_3Ratio","",40,-15.,15.)));       

 DistributionStreamMap.insert(mapElement("ele1_eRegrInput_subClusterEta_1", new TH1F("ele1_eRegrInput_subClusterEta_1Stream","",50,-2.5,2.5)));       
 DistributionRecoMap.insert(mapElement("ele1_eRegrInput_subClusterEta_1", new TH1F("ele1_eRegrInput_subClusterEta_1Reco","",50,-2.5,2.5)));       
 DifferenceMap.insert(mapElement("ele1_eRegrInput_subClusterEta_1", new TH1F("ele1_eRegrInput_subClusterEta_1Ratio","",50,-0.5,0.5)));       

 DistributionStreamMap.insert(mapElement("ele1_eRegrInput_subClusterEta_2", new TH1F("ele1_eRegrInput_subClusterEta_2Stream","",50,-2.5,2.5)));       
 DistributionRecoMap.insert(mapElement("ele1_eRegrInput_subClusterEta_2", new TH1F("ele1_eRegrInput_subClusterEta_2Reco","",50,-2.5,2.5)));       
 DifferenceMap.insert(mapElement("ele1_eRegrInput_subClusterEta_2", new TH1F("ele1_eRegrInput_subClusterEta_2Ratio","",50,-0.5,0.5)));       

 DistributionStreamMap.insert(mapElement("ele1_eRegrInput_subClusterEta_3", new TH1F("ele1_eRegrInput_subClusterEta_3Stream","",50,-2.5,2.5)));       
 DistributionRecoMap.insert(mapElement("ele1_eRegrInput_subClusterEta_3", new TH1F("ele1_eRegrInput_subClusterEta_3Reco","",50,-2.5,2.5)));       
 DifferenceMap.insert(mapElement("ele1_eRegrInput_subClusterEta_3", new TH1F("ele1_eRegrInput_subClusterEta_3Ratio","",50,-0.1,0.1)));       

 DistributionStreamMap.insert(mapElement("ele1_eRegrInput_subClusterPhi_1", new TH1F("ele1_eRegrInput_subClusterPhi_1Stream","",50,0,3.14)));       
 DistributionRecoMap.insert(mapElement("ele1_eRegrInput_subClusterPhi_1", new TH1F("ele1_eRegrInput_subClusterPhi_1Reco","",50,0,3.14)));       
 DifferenceMap.insert(mapElement("ele1_eRegrInput_subClusterPhi_1", new TH1F("ele1_eRegrInput_subClusterPhi_1Ratio","",50,-0.1,0.1)));       

 DistributionStreamMap.insert(mapElement("ele1_eRegrInput_subClusterPhi_2", new TH1F("ele1_eRegrInput_subClusterPhi_2Stream","",50,0,3.14)));       
 DistributionRecoMap.insert(mapElement("ele1_eRegrInput_subClusterPhi_2", new TH1F("ele1_eRegrInput_subClusterPhi_2Reco","",50,0,3.14)));       
 DifferenceMap.insert(mapElement("ele1_eRegrInput_subClusterPhi_2", new TH1F("ele1_eRegrInput_subClusterPhi_2Ratio","",50,-0.1,0.1)));       

 DistributionStreamMap.insert(mapElement("ele1_eRegrInput_subClusterPhi_3", new TH1F("ele1_eRegrInput_subClusterPhi_3Stream","",50,0,3.14)));       
 DistributionRecoMap.insert(mapElement("ele1_eRegrInput_subClusterPhi_3", new TH1F("ele1_eRegrInput_subClusterPhi_3Reco","",50,0,3.14)));       
 DifferenceMap.insert(mapElement("ele1_eRegrInput_subClusterPhi_3", new TH1F("ele1_eRegrInput_subClusterPhi_3Ratio","",50,-0.1,0.1)));       

 DistributionStreamMap.insert(mapElement("ele1_eRegrInput_subClusterEmax_1", new TH1F("ele1_eRegrInput_subClusterEmax_1Stream","",50,0,50.)));       
 DistributionRecoMap.insert(mapElement("ele1_eRegrInput_subClusterEmax_1", new TH1F("ele1_eRegrInput_subClusterEmax_1Reco","",50,0,50.)));       
 DifferenceMap.insert(mapElement("ele1_eRegrInput_subClusterEmax_1", new TH1F("ele1_eRegrInput_subClusterEmax_1Ratio","",50,-25.,25.)));       

 DistributionStreamMap.insert(mapElement("ele1_eRegrInput_subClusterEmax_2", new TH1F("ele1_eRegrInput_subClusterEmax_2Stream","",30,0,30.)));       
 DistributionRecoMap.insert(mapElement("ele1_eRegrInput_subClusterEmax_2", new TH1F("ele1_eRegrInput_subClusterEmax_2Reco","",30,0,30.)));       
 DifferenceMap.insert(mapElement("ele1_eRegrInput_subClusterEmax_2", new TH1F("ele1_eRegrInput_subClusterEmax_2Ratio","",40,-20.,20.)));       

 DistributionStreamMap.insert(mapElement("ele1_eRegrInput_subClusterEmax_3", new TH1F("ele1_eRegrInput_subClusterEmax_3Stream","",20,0,20.)));       
 DistributionRecoMap.insert(mapElement("ele1_eRegrInput_subClusterEmax_3", new TH1F("ele1_eRegrInput_subClusterEmax_3Reco","",20,0,20.)));       
 DifferenceMap.insert(mapElement("ele1_eRegrInput_subClusterEmax_3", new TH1F("ele1_eRegrInput_subClusterEmax_3Ratio","",40,-15.,15.)));       

 DistributionStreamMap.insert(mapElement("ele1_eRegrInput_subClusterE3x3_1", new TH1F("ele1_eRegrInput_subClusterE3x3_1Stream","",50,0,50.)));       
 DistributionRecoMap.insert(mapElement("ele1_eRegrInput_subClusterE3x3_1", new TH1F("ele1_eRegrInput_subClusterE3x3_1Reco","",50,0,50.)));       
 DifferenceMap.insert(mapElement("ele1_eRegrInput_subClusterE3x3_1", new TH1F("ele1_eRegrInput_subClusterE3x3_1Ratio","",50,-25.,25.)));       

 DistributionStreamMap.insert(mapElement("ele1_eRegrInput_subClusterE3x3_2", new TH1F("ele1_eRegrInput_subClusterE3x3_2Stream","",40,0,40.)));       
 DistributionRecoMap.insert(mapElement("ele1_eRegrInput_subClusterE3x3_2", new TH1F("ele1_eRegrInput_subClusterE3x3_2Reco","",40,0,40.)));       
 DifferenceMap.insert(mapElement("ele1_eRegrInput_subClusterE3x3_2", new TH1F("ele1_eRegrInput_subClusterE3x3_2Ratio","",40,-20.,20.)));       

 DistributionStreamMap.insert(mapElement("ele1_eRegrInput_subClusterE3x3_3", new TH1F("ele1_eRegrInput_subClusterE3x3_3Stream","",30,0,30.)));       
 DistributionRecoMap.insert(mapElement("ele1_eRegrInput_subClusterE3x3_3", new TH1F("ele1_eRegrInput_subClusterE3x3_3Reco","",30,0,30.)));       
 DifferenceMap.insert(mapElement("ele1_eRegrInput_subClusterE3x3_3", new TH1F("ele1_eRegrInput_subClusterE3x3_3Ratio","",40,-15.,15.)));       

 DistributionStreamMap.insert(mapElement("ele1_eRegrInput_eESClusters", new TH1F("ele1_eRegrInput_eESClustersStream","",50,0,50.)));       
 DistributionRecoMap.insert(mapElement("ele1_eRegrInput_eESClusters", new TH1F("ele1_eRegrInput_eESClustersReco","",50,0,50.)));       
 DifferenceMap.insert(mapElement("ele1_eRegrInput_eESClusters", new TH1F("ele1_eRegrInput_eESClustersRatio","",40,-20.,20.)));       
 /* 
 DistributionStreamMap.insert(mapElement("ele1_eRegrInput_eESClusterEnergy_1", new TH1F("ele1_eRegrInput_eESClusterEnergy_1Stream","",50,0,50.)));       
 DistributionRecoMap.insert(mapElement("ele1_eRegrInput_eESClusterEnergy_1", new TH1F("ele1_eRegrInput_eESClusterEnergy_1Reco","",50,0,50.)));       
 DifferenceMap.insert(mapElement("ele1_eRegrInput_eESClusterEnergy_1", new TH1F("ele1_eRegrInput_eESClusterEnergy_1Ratio","",40,-20.,20.)));       

 DistributionStreamMap.insert(mapElement("ele1_eRegrInput_eESClusterEnergy_2", new TH1F("ele1_eRegrInput_eESClusterEnergy_2Stream","",50,0,50.)));       
 DistributionRecoMap.insert(mapElement("ele1_eRegrInput_eESClusterEnergy_2", new TH1F("ele1_eRegrInput_eESClusterEnergy_2Reco","",50,0,50.)));       
 DifferenceMap.insert(mapElement("ele1_eRegrInput_eESClusterEnergy_2", new TH1F("ele1_eRegrInput_eESClusterEnergy_2Ratio","",40,-20.,20.)));       

 DistributionStreamMap.insert(mapElement("ele1_eRegrInput_eESClusterEnergy_3", new TH1F("ele1_eRegrInput_eESClusterEnergy_3Stream","",50,0,50.)));       
 DistributionRecoMap.insert(mapElement("ele1_eRegrInput_eESClusterEnergy_3", new TH1F("ele1_eRegrInput_eESClusterEnergy_3Reco","",50,0,50.)));       
 DifferenceMap.insert(mapElement("ele1_eRegrInput_eESClusterEnergy_3", new TH1F("ele1_eRegrInput_eESClusterEnergy_3Ratio","",40,-20.,20.)));       

 DistributionStreamMap.insert(mapElement("ele1_eRegrInput_eESClusterEta_1", new TH1F("ele1_eRegrInput_eESClusterEta_1Stream","",50,-2.5,2.5)));       
 DistributionRecoMap.insert(mapElement("ele1_eRegrInput_eESClusterEta_1", new TH1F("ele1_eRegrInput_eESClusterEta_1Reco","",50,-2.5,2.5)));       
 DifferenceMap.insert(mapElement("ele1_eRegrInput_eESClusterEta_1", new TH1F("ele1_eRegrInput_eESClusterEta_1Ratio","",40,-0.5,0.5)));       

 DistributionStreamMap.insert(mapElement("ele1_eRegrInput_eESClusterEta_2", new TH1F("ele1_eRegrInput_eESClusterEta_2Stream","",50,-2.5,2.5)));       
 DistributionRecoMap.insert(mapElement("ele1_eRegrInput_eESClusterEta_2", new TH1F("ele1_eRegrInput_eESClusterEta_2Reco","",50,-2.5,2.5)));       
 DifferenceMap.insert(mapElement("ele1_eRegrInput_eESClusterEta_2", new TH1F("ele1_eRegrInput_eESClusterEta_2Ratio","",40,-0.5,0.5)));       

 DistributionStreamMap.insert(mapElement("ele1_eRegrInput_eESClusterEta_3", new TH1F("ele1_eRegrInput_eESClusterEta_3Stream","",50,-2.5,2.5)));       
 DistributionRecoMap.insert(mapElement("ele1_eRegrInput_eESClusterEta_3", new TH1F("ele1_eRegrInput_eESClusterEta_3Reco","",50,-2.5,2.5)));       
 DifferenceMap.insert(mapElement("ele1_eRegrInput_eESClusterEta_3", new TH1F("ele1_eRegrInput_eESClusterEta_3Ratio","",40,-0.5,0.5)));       
 
 DistributionStreamMap.insert(mapElement("ele1_eRegrInput_eESClusterPhi_1", new TH1F("ele1_eRegrInput_eESClusterPhi_1Stream","",50,0,3.14)));       
 DistributionRecoMap.insert(mapElement("ele1_eRegrInput_eESClusterPhi_1",new TH1F("ele1_eRegrInput_eESClusterPhi_1Reco","",50,0,3.14)));       
 DifferenceMap.insert(mapElement("ele1_eRegrInput_eESClusterPhi_1",new TH1F("ele1_eRegrInput_eESClusterPhi_1Ratio","",40,-0.5,0.5)));       

 DistributionStreamMap.insert(mapElement("ele1_eRegrInput_eESClusterPhi_2",new TH1F("ele1_eRegrInput_eESClusterPhi_2Stream","",50,0,3.14)));       
 DistributionRecoMap.insert(mapElement("ele1_eRegrInput_eESClusterPhi_2",new TH1F("ele1_eRegrInput_eESClusterPhi_2Reco","",50,0,3.14)));       
 DifferenceMap.insert(mapElement("ele1_eRegrInput_eESClusterPhi_2",new TH1F("ele1_eRegrInput_eESClusterPhi_2Ratio","",40,-0.5,0.5)));       

 DistributionStreamMap.insert(mapElement("ele1_eRegrInput_eESClusterPhi_3",new TH1F("ele1_eRegrInput_eESClusterPhi_3Stream","",50,0,3.14)));       
 DistributionRecoMap.insert(mapElement("ele1_eRegrInput_eESClusterPhi_3",new TH1F("ele1_eRegrInput_eESClusterPhi_3Reco","",50,0,3.14)));       
 DifferenceMap.insert(mapElement("ele1_eRegrInput_eESClusterPhi_3",new TH1F("ele1_eRegrInput_eESClusterPhi_3Ratio","",40,-0.5,0.5)));       
 */

 return ;
}

void SetVariablesCorrelationMapEntry(std::map<std::string,TH2F*> & CorrelationMap){

 
 CorrelationMap.insert(mapElementCorr("rho",  new TH2F("rho_corr","",50,0,50.,50.,0,50.)));
 CorrelationMap.insert(mapElementCorr("PV_n", new TH2F("PV_n_corr","",40,0,40,40,0,40)));
 CorrelationMap.insert(mapElementCorr("met",  new TH2F("met","",50,0,200,50,0,200)));
 CorrelationMap.insert(mapElementCorr("ele1Met_mt", new TH2F("ele1Met_mt","",50,0,100,50,0,100)));

 CorrelationMap.insert(mapElementCorr("ele1_hadIso", new TH2F("ele1_hadIso","",50,0,10,50,0,10)));
 CorrelationMap.insert(mapElementCorr("ele1_tkIso",  new TH2F("ele1_tkIso","",50,0,10,50,0,10)));
 CorrelationMap.insert(mapElementCorr("ele1_emIso",  new TH2F("ele1_emIso","",50,0,10,50,0,10)));

 CorrelationMap.insert(mapElementCorr("ele1_dxy_PV", new TH2F("ele1_dxy_PV","",50,0,0.5,50,0,0.5)));
 CorrelationMap.insert(mapElementCorr("ele1_dz_PV", new TH2F("ele1_dz_PV","",50,0,20,50,0,20)));

 return ;

}
  

void SetStartingBranchTrees(TTree* streamTree, TTree* recoTree){

 streamTree->SetBranchStatus("*",0);

 streamTree->SetBranchStatus("lumiId",1);
 streamTree->SetBranchStatus("eventId",1);
 streamTree->SetBranchStatus("runId",1);
 streamTree->SetBranchStatus("isW",1);
 streamTree->SetBranchStatus("rho",1);
 streamTree->SetBranchStatus("PV_n",1);
 streamTree->SetBranchStatus("ele1_nRecHits",1);
 streamTree->SetBranchStatus("ele1_nGgsfTrackHits",1);
 streamTree->SetBranchStatus("ele1_numberOfLostHits",1);
 streamTree->SetBranchStatus("ele1_nAmbiguousGsfTrack",1);
 streamTree->SetBranchStatus("ele1_isEB",1);
 streamTree->SetBranchStatus("ele1_seedIeta",1);
 streamTree->SetBranchStatus("ele1_seedIphi",1);
 streamTree->SetBranchStatus("ele1_seedIx",1);
 streamTree->SetBranchStatus("ele1_seedIy",1);
 streamTree->SetBranchStatus("ele1_seedZside",1);
 streamTree->SetBranchStatus("ele1_scNxtal",1);

 streamTree->SetBranchStatus("ele1_pt",1);
 streamTree->SetBranchStatus("ele1_eta",1);
 streamTree->SetBranchStatus("ele1_phi",1);
 streamTree->SetBranchStatus("ele1_sigmaIetaIeta",1);
 streamTree->SetBranchStatus("ele1_DphiIn",1);
 streamTree->SetBranchStatus("ele1_DetaIn",1);
 streamTree->SetBranchStatus("ele1_HOverE",1);
 streamTree->SetBranchStatus("ele1_tkIso",1);
 streamTree->SetBranchStatus("ele1_emIso",1);
 streamTree->SetBranchStatus("ele1_hadIso",1);
 streamTree->SetBranchStatus("ele1_scERaw",1);
 streamTree->SetBranchStatus("ele1_scE",1);
 streamTree->SetBranchStatus("ele1_es",1);
 streamTree->SetBranchStatus("ele1_scEta",1);
 streamTree->SetBranchStatus("ele1_scPhi",1);
 streamTree->SetBranchStatus("ele1_scEtaWidth",1);
 streamTree->SetBranchStatus("ele1_scPhiWidth",1);
 streamTree->SetBranchStatus("ele1_tkP",1);
 streamTree->SetBranchStatus("ele1_fbrem",1);
 streamTree->SetBranchStatus("ele1_dxy_PV",1);
 streamTree->SetBranchStatus("ele1_dz_PV",1);
 streamTree->SetBranchStatus("ele1_e5x5",1);
 streamTree->SetBranchStatus("ele1_e3x3",1);
 streamTree->SetBranchStatus("ele1_seedE",1);

 streamTree->SetBranchStatus("met_et",1);
 streamTree->SetBranchStatus("met_phi",1);
 streamTree->SetBranchStatus("ele1Met_mt",1);

 streamTree->SetBranchStatus("ele1_recHit_E",1);
 streamTree->SetBranchStatus("ele1_idtype",1);

 streamTree->SetBranchStatus("ele1_eRegrInput_rawE",1);
 streamTree->SetBranchStatus("ele1_eRegrInput_r9",1);
 streamTree->SetBranchStatus("ele1_eRegrInput_eta",1);
 streamTree->SetBranchStatus("ele1_eRegrInput_phi",1);
 streamTree->SetBranchStatus("ele1_eRegrInput_etaW",1);
 streamTree->SetBranchStatus("ele1_eRegrInput_phiW",1);
 streamTree->SetBranchStatus("ele1_eRegrInput_SCsize",1);
 streamTree->SetBranchStatus("ele1_eRegrInput_rho",1);
 streamTree->SetBranchStatus("ele1_eRegrInput_hoe",1);
 streamTree->SetBranchStatus("ele1_eRegrInput_nPV",1);
 streamTree->SetBranchStatus("ele1_eRegrInput_seed_eta",1);
 streamTree->SetBranchStatus("ele1_eRegrInput_seed_phi",1);
 streamTree->SetBranchStatus("ele1_eRegrInput_seed_E",1);
 streamTree->SetBranchStatus("ele1_eRegrInput_sigietaieta",1);
 streamTree->SetBranchStatus("ele1_eRegrInput_sigiphiiphi",1);
 streamTree->SetBranchStatus("ele1_eRegrInput_sigietaiphi",1);
 streamTree->SetBranchStatus("ele1_eRegrInput_eMax",1);
 streamTree->SetBranchStatus("ele1_eRegrInput_e2nd",1);
 streamTree->SetBranchStatus("ele1_eRegrInput_eTop",1);
 streamTree->SetBranchStatus("ele1_eRegrInput_eBottom",1);
 streamTree->SetBranchStatus("ele1_eRegrInput_eLeft",1);
 streamTree->SetBranchStatus("ele1_eRegrInput_eRight",1);
 streamTree->SetBranchStatus("ele1_eRegrInput_e2x5Max",1);
 streamTree->SetBranchStatus("ele1_eRegrInput_e2x5Top",1);
 streamTree->SetBranchStatus("ele1_eRegrInput_e2x5Bottom",1);
 streamTree->SetBranchStatus("ele1_eRegrInput_e2x5Left",1);
 streamTree->SetBranchStatus("ele1_eRegrInput_e2x5Right",1);
 streamTree->SetBranchStatus("ele1_eRegrInput_seed_ieta",1);
 streamTree->SetBranchStatus("ele1_eRegrInput_seed_iphi",1);
 streamTree->SetBranchStatus("ele1_eRegrInput_seed_etaCrySeed",1);
 streamTree->SetBranchStatus("ele1_eRegrInput_seed_phiCrySeed",1);
 streamTree->SetBranchStatus("ele1_eRegrInput_preshowerEnergyOverRaw",1);
 streamTree->SetBranchStatus("ele1_eRegrInput_ecalDrivenSeed",1);
 streamTree->SetBranchStatus("ele1_eRegrInput_isEBEtaGap",1);
 streamTree->SetBranchStatus("ele1_eRegrInput_isEBPhiGap",1);
 streamTree->SetBranchStatus("ele1_eRegrInput_eSubClusters",1);
 streamTree->SetBranchStatus("ele1_eRegrInput_subClusterEnergy_1",1);
 streamTree->SetBranchStatus("ele1_eRegrInput_subClusterEnergy_2",1);
 streamTree->SetBranchStatus("ele1_eRegrInput_subClusterEnergy_3",1);
 streamTree->SetBranchStatus("ele1_eRegrInput_subClusterEta_1",1);
 streamTree->SetBranchStatus("ele1_eRegrInput_subClusterEta_2",1);
 streamTree->SetBranchStatus("ele1_eRegrInput_subClusterEta_3",1);
 streamTree->SetBranchStatus("ele1_eRegrInput_subClusterPhi_1",1);
 streamTree->SetBranchStatus("ele1_eRegrInput_subClusterPhi_2",1);
 streamTree->SetBranchStatus("ele1_eRegrInput_subClusterPhi_3",1);
 streamTree->SetBranchStatus("ele1_eRegrInput_subClusterEmax_1",1);
 streamTree->SetBranchStatus("ele1_eRegrInput_subClusterEmax_2",1);
 streamTree->SetBranchStatus("ele1_eRegrInput_subClusterEmax_3",1);
 streamTree->SetBranchStatus("ele1_eRegrInput_subClusterE3x3_1",1);
 streamTree->SetBranchStatus("ele1_eRegrInput_subClusterE3x3_2",1);
 streamTree->SetBranchStatus("ele1_eRegrInput_subClusterE3x3_3",1);
 streamTree->SetBranchStatus("ele1_eRegrInput_eESClusters",1);
 streamTree->SetBranchStatus("ele1_eRegrInput_eESClusterEnergy_1",1);
 streamTree->SetBranchStatus("ele1_eRegrInput_eESClusterEnergy_2",1);
 streamTree->SetBranchStatus("ele1_eRegrInput_eESClusterEnergy_3",1);
 streamTree->SetBranchStatus("ele1_eRegrInput_eESClusterEta_1",1);
 streamTree->SetBranchStatus("ele1_eRegrInput_eESClusterEta_2",1);
 streamTree->SetBranchStatus("ele1_eRegrInput_eESClusterEta_3",1);
 streamTree->SetBranchStatus("ele1_eRegrInput_eESClusterPhi_1",1);
 streamTree->SetBranchStatus("ele1_eRegrInput_eESClusterPhi_2",1);
 streamTree->SetBranchStatus("ele1_eRegrInput_eESClusterPhi_3",1);
 streamTree->SetBranchStatus("ele1_eRegrInput_pt",1);
 streamTree->SetBranchStatus("ele1_eRegrInput_trackMomentumAtVtxR",1);
 streamTree->SetBranchStatus("ele1_eRegrInput_fbrem",1);
 streamTree->SetBranchStatus("ele1_eRegrInput_charge",1);
 streamTree->SetBranchStatus("ele1_eRegrInput_eSuperClusterOverP",1);

 // branch Address
 streamTree->SetBranchAddress("lumiId",&lumiId_s);
 streamTree->SetBranchAddress("eventId",&eventId_s);
 streamTree->SetBranchAddress("runId",&runId_s);
 streamTree->SetBranchAddress("isW",&isW_s);
 streamTree->SetBranchAddress("rho",&rho_s);
 streamTree->SetBranchAddress("PV_n",&PV_n_s);
 streamTree->SetBranchAddress("ele1_nRecHits",&ele1_nRecHits_s);
 streamTree->SetBranchAddress("ele1_nGgsfTrackHits",&ele1_nGgsfTrackHits_s);
 streamTree->SetBranchAddress("ele1_numberOfLostHits",&ele1_numberOfLostHits_s);
 streamTree->SetBranchAddress("ele1_nAmbiguousGsfTrack",&ele1_nAmbiguousGsfTrack_s);
 streamTree->SetBranchAddress("ele1_isEB",&ele1_isEB_s);
 streamTree->SetBranchAddress("ele1_seedIeta",&ele1_seedIeta_s);
 streamTree->SetBranchAddress("ele1_seedIphi",&ele1_seedIphi_s);
 streamTree->SetBranchAddress("ele1_seedIx",&ele1_seedIx_s);
 streamTree->SetBranchAddress("ele1_seedIy",&ele1_seedIy_s);
 streamTree->SetBranchAddress("ele1_seedZside",&ele1_seedZside_s);
 streamTree->SetBranchAddress("ele1_scNxtal",&ele1_scNxtal_s);

 streamTree->SetBranchAddress("ele1_pt",&ele1_pt_s);
 streamTree->SetBranchAddress("ele1_eta",&ele1_eta_s);
 streamTree->SetBranchAddress("ele1_phi",&ele1_phi_s);
 streamTree->SetBranchAddress("ele1_sigmaIetaIeta",&ele1_sigmaIetaIeta_s);
 streamTree->SetBranchAddress("ele1_DphiIn",&ele1_DphiIn_s);
 streamTree->SetBranchAddress("ele1_DetaIn",&ele1_DetaIn_s);
 streamTree->SetBranchAddress("ele1_HOverE",&ele1_HOverE_s);
 streamTree->SetBranchAddress("ele1_tkIso",&ele1_tkIso_s);
 streamTree->SetBranchAddress("ele1_emIso",&ele1_emIso_s);
 streamTree->SetBranchAddress("ele1_hadIso",&ele1_hadIso_s);
 streamTree->SetBranchAddress("ele1_scERaw",&ele1_scERaw_s);
 streamTree->SetBranchAddress("ele1_scE",&ele1_scE_s);
 streamTree->SetBranchAddress("ele1_es",&ele1_es_s);
 streamTree->SetBranchAddress("ele1_scEta",&ele1_scEta_s);
 streamTree->SetBranchAddress("ele1_scPhi",&ele1_scPhi_s);
 streamTree->SetBranchAddress("ele1_scEtaWidth",&ele1_scEtaWidth_s);
 streamTree->SetBranchAddress("ele1_scPhiWidth",&ele1_scPhiWidth_s);
 streamTree->SetBranchAddress("ele1_tkP",&ele1_tkP_s);
 streamTree->SetBranchAddress("ele1_fbrem",&ele1_fbrem_s);
 streamTree->SetBranchAddress("ele1_dxy_PV",&ele1_dxy_PV_s);
 streamTree->SetBranchAddress("ele1_dz_PV",&ele1_dz_PV_s);
 streamTree->SetBranchAddress("ele1_e5x5",&ele1_e5x5_s);
 streamTree->SetBranchAddress("ele1_e3x3",&ele1_e3x3_s);
 streamTree->SetBranchAddress("ele1_seedE",&ele1_seedE_s);

 streamTree->SetBranchAddress("met_et",&met_s);
 streamTree->SetBranchAddress("met_phi",&met_phi_s);
 streamTree->SetBranchAddress("ele1Met_mt",&ele1Met_mt_s);

 streamTree->SetBranchAddress("ele1_recHit_E",&ele1_recHit_E_s);
 streamTree->SetBranchAddress("ele1_idtype",&ele1_idtype_s);

 streamTree->SetBranchAddress("ele1_eRegrInput_rawE",&ele1_eRegrInput_rawE_s);
 streamTree->SetBranchAddress("ele1_eRegrInput_r9",&ele1_eRegrInput_r9_s);
 streamTree->SetBranchAddress("ele1_eRegrInput_eta",&ele1_eRegrInput_eta_s);
 streamTree->SetBranchAddress("ele1_eRegrInput_phi",&ele1_eRegrInput_phi_s);
 streamTree->SetBranchAddress("ele1_eRegrInput_etaW",&ele1_eRegrInput_etaW_s);
 streamTree->SetBranchAddress("ele1_eRegrInput_phiW",&ele1_eRegrInput_phiW_s);
 streamTree->SetBranchAddress("ele1_eRegrInput_SCsize",&ele1_eRegrInput_SCsize_s);
 streamTree->SetBranchAddress("ele1_eRegrInput_rho",&ele1_eRegrInput_rho_s);
 streamTree->SetBranchAddress("ele1_eRegrInput_hoe",&ele1_eRegrInput_hoe_s);
 streamTree->SetBranchAddress("ele1_eRegrInput_nPV",&ele1_eRegrInput_nPV_s);
 streamTree->SetBranchAddress("ele1_eRegrInput_seed_eta",&ele1_eRegrInput_seed_eta_s);
 streamTree->SetBranchAddress("ele1_eRegrInput_seed_phi",&ele1_eRegrInput_seed_phi_s);
 streamTree->SetBranchAddress("ele1_eRegrInput_seed_E",&ele1_eRegrInput_seed_E_s);
 streamTree->SetBranchAddress("ele1_eRegrInput_sigietaieta",&ele1_eRegrInput_sigietaieta_s);
 streamTree->SetBranchAddress("ele1_eRegrInput_sigiphiiphi",&ele1_eRegrInput_sigiphiiphi_s);
 streamTree->SetBranchAddress("ele1_eRegrInput_sigietaiphi",&ele1_eRegrInput_sigietaiphi_s);
 streamTree->SetBranchAddress("ele1_eRegrInput_eMax",&ele1_eRegrInput_eMax_s);
 streamTree->SetBranchAddress("ele1_eRegrInput_e2nd",&ele1_eRegrInput_e2nd_s);
 streamTree->SetBranchAddress("ele1_eRegrInput_eTop",&ele1_eRegrInput_eTop_s);
 streamTree->SetBranchAddress("ele1_eRegrInput_eBottom",&ele1_eRegrInput_eBottom_s);
 streamTree->SetBranchAddress("ele1_eRegrInput_eLeft",&ele1_eRegrInput_eLeft_s);
 streamTree->SetBranchAddress("ele1_eRegrInput_eRight",&ele1_eRegrInput_eRight_s);
 streamTree->SetBranchAddress("ele1_eRegrInput_e2x5Max",&ele1_eRegrInput_e2x5Max_s);
 streamTree->SetBranchAddress("ele1_eRegrInput_e2x5Top",&ele1_eRegrInput_e2x5Top_s);
 streamTree->SetBranchAddress("ele1_eRegrInput_e2x5Bottom",&ele1_eRegrInput_e2x5Bottom_s);
 streamTree->SetBranchAddress("ele1_eRegrInput_e2x5Left",&ele1_eRegrInput_e2x5Left_s);
 streamTree->SetBranchAddress("ele1_eRegrInput_e2x5Right",&ele1_eRegrInput_e2x5Right_s);
 streamTree->SetBranchAddress("ele1_eRegrInput_seed_ieta",&ele1_eRegrInput_seed_ieta_s);
 streamTree->SetBranchAddress("ele1_eRegrInput_seed_iphi",&ele1_eRegrInput_seed_iphi_s);
 streamTree->SetBranchAddress("ele1_eRegrInput_seed_etaCrySeed",&ele1_eRegrInput_seed_etaCrySeed_s);
 streamTree->SetBranchAddress("ele1_eRegrInput_seed_phiCrySeed",&ele1_eRegrInput_seed_phiCrySeed_s);
 streamTree->SetBranchAddress("ele1_eRegrInput_preshowerEnergyOverRaw",&ele1_eRegrInput_preshowerEnergyOverRaw_s);
 streamTree->SetBranchAddress("ele1_eRegrInput_ecalDrivenSeed",&ele1_eRegrInput_ecalDrivenSeed_s);
 streamTree->SetBranchAddress("ele1_eRegrInput_isEBEtaGap",&ele1_eRegrInput_isEBEtaGap_s);
 streamTree->SetBranchAddress("ele1_eRegrInput_isEBPhiGap",&ele1_eRegrInput_isEBPhiGap_s);
 streamTree->SetBranchAddress("ele1_eRegrInput_eSubClusters",&ele1_eRegrInput_eSubClusters_s);
 streamTree->SetBranchAddress("ele1_eRegrInput_subClusterEnergy_1",&ele1_eRegrInput_subClusterEnergy_1_s);
 streamTree->SetBranchAddress("ele1_eRegrInput_subClusterEnergy_2",&ele1_eRegrInput_subClusterEnergy_2_s);
 streamTree->SetBranchAddress("ele1_eRegrInput_subClusterEnergy_3",&ele1_eRegrInput_subClusterEnergy_3_s);
 streamTree->SetBranchAddress("ele1_eRegrInput_subClusterEta_1",&ele1_eRegrInput_subClusterEta_1_s);
 streamTree->SetBranchAddress("ele1_eRegrInput_subClusterEta_2",&ele1_eRegrInput_subClusterEta_2_s);
 streamTree->SetBranchAddress("ele1_eRegrInput_subClusterEta_3",&ele1_eRegrInput_subClusterEta_3_s);
 streamTree->SetBranchAddress("ele1_eRegrInput_subClusterPhi_1",&ele1_eRegrInput_subClusterPhi_1_s);
 streamTree->SetBranchAddress("ele1_eRegrInput_subClusterPhi_2",&ele1_eRegrInput_subClusterPhi_2_s);
 streamTree->SetBranchAddress("ele1_eRegrInput_subClusterPhi_3",&ele1_eRegrInput_subClusterPhi_3_s);
 streamTree->SetBranchAddress("ele1_eRegrInput_subClusterEmax_1",&ele1_eRegrInput_subClusterEmax_1_s);
 streamTree->SetBranchAddress("ele1_eRegrInput_subClusterEmax_2",&ele1_eRegrInput_subClusterEmax_2_s);
 streamTree->SetBranchAddress("ele1_eRegrInput_subClusterEmax_3",&ele1_eRegrInput_subClusterEmax_3_s);
 streamTree->SetBranchAddress("ele1_eRegrInput_subClusterE3x3_1",&ele1_eRegrInput_subClusterE3x3_1_s);
 streamTree->SetBranchAddress("ele1_eRegrInput_subClusterE3x3_2",&ele1_eRegrInput_subClusterE3x3_2_s);
 streamTree->SetBranchAddress("ele1_eRegrInput_subClusterE3x3_3",&ele1_eRegrInput_subClusterE3x3_3_s);
 streamTree->SetBranchAddress("ele1_eRegrInput_eESClusters",&ele1_eRegrInput_eESClusters_s);
 streamTree->SetBranchAddress("ele1_eRegrInput_eESClusterEnergy_1",&ele1_eRegrInput_eESClusterEnergy_1_s);
 streamTree->SetBranchAddress("ele1_eRegrInput_eESClusterEnergy_2",&ele1_eRegrInput_eESClusterEnergy_2_s);
 streamTree->SetBranchAddress("ele1_eRegrInput_eESClusterEnergy_3",&ele1_eRegrInput_eESClusterEnergy_3_s);
 streamTree->SetBranchAddress("ele1_eRegrInput_eESClusterEta_1",&ele1_eRegrInput_eESClusterEta_1_s);
 streamTree->SetBranchAddress("ele1_eRegrInput_eESClusterEta_2",&ele1_eRegrInput_eESClusterEta_2_s);
 streamTree->SetBranchAddress("ele1_eRegrInput_eESClusterEta_3",&ele1_eRegrInput_eESClusterEta_3_s);
 streamTree->SetBranchAddress("ele1_eRegrInput_eESClusterPhi_1",&ele1_eRegrInput_eESClusterPhi_1_s);
 streamTree->SetBranchAddress("ele1_eRegrInput_eESClusterPhi_2",&ele1_eRegrInput_eESClusterPhi_2_s);
 streamTree->SetBranchAddress("ele1_eRegrInput_eESClusterPhi_3",&ele1_eRegrInput_eESClusterPhi_3_s);
 streamTree->SetBranchAddress("ele1_eRegrInput_pt",&ele1_eRegrInput_pt_s);
 streamTree->SetBranchAddress("ele1_eRegrInput_trackMomentumAtVtxR",&ele1_eRegrInput_trackMomentumAtVtxR_s);
 streamTree->SetBranchAddress("ele1_eRegrInput_fbrem",&ele1_eRegrInput_fbrem_s);
 streamTree->SetBranchAddress("ele1_eRegrInput_charge",&ele1_eRegrInput_charge_s);
 streamTree->SetBranchAddress("ele1_eRegrInput_eSuperClusterOverP",&ele1_eRegrInput_eSuperClusterOverP_s);

 recoTree->SetBranchStatus("lumiId",1);
 recoTree->SetBranchStatus("eventId",1);
 recoTree->SetBranchStatus("runId",1);
 recoTree->SetBranchStatus("isW",1);
 recoTree->SetBranchStatus("rho",1);
 recoTree->SetBranchStatus("PV_n",1);
 recoTree->SetBranchStatus("ele1_nRecHits",1);
 recoTree->SetBranchStatus("ele1_nGgsfTrackHits",1);
 recoTree->SetBranchStatus("ele1_numberOfLostHits",1);
 recoTree->SetBranchStatus("ele1_nAmbiguousGsfTrack",1);
 recoTree->SetBranchStatus("ele1_isEB",1);
 recoTree->SetBranchStatus("ele1_seedIeta",1);
 recoTree->SetBranchStatus("ele1_seedIphi",1);
 recoTree->SetBranchStatus("ele1_seedIx",1);
 recoTree->SetBranchStatus("ele1_seedIy",1);
 recoTree->SetBranchStatus("ele1_seedZside",1);
 recoTree->SetBranchStatus("ele1_scNxtal",1);

 recoTree->SetBranchStatus("ele1_pt",1);
 recoTree->SetBranchStatus("ele1_eta",1);
 recoTree->SetBranchStatus("ele1_phi",1);
 recoTree->SetBranchStatus("ele1_sigmaIetaIeta",1);
 recoTree->SetBranchStatus("ele1_DphiIn",1);
 recoTree->SetBranchStatus("ele1_DetaIn",1);
 recoTree->SetBranchStatus("ele1_HOverE",1);
 recoTree->SetBranchStatus("ele1_tkIso",1);
 recoTree->SetBranchStatus("ele1_emIso",1);
 recoTree->SetBranchStatus("ele1_hadIso",1);
 recoTree->SetBranchStatus("ele1_scERaw",1);
 recoTree->SetBranchStatus("ele1_scE",1);
 recoTree->SetBranchStatus("ele1_es",1);
 recoTree->SetBranchStatus("ele1_scEta",1);
 recoTree->SetBranchStatus("ele1_scPhi",1);
 recoTree->SetBranchStatus("ele1_scEtaWidth",1);
 recoTree->SetBranchStatus("ele1_scPhiWidth",1);
 recoTree->SetBranchStatus("ele1_tkP",1);
 recoTree->SetBranchStatus("ele1_fbrem",1);
 recoTree->SetBranchStatus("ele1_dxy_PV",1);
 recoTree->SetBranchStatus("ele1_dz_PV",1);
 recoTree->SetBranchStatus("ele1_e5x5",1);
 recoTree->SetBranchStatus("ele1_e3x3",1);
 recoTree->SetBranchStatus("ele1_seedE",1);

 recoTree->SetBranchStatus("met_et",1);
 recoTree->SetBranchStatus("met_phi",1);
 recoTree->SetBranchStatus("ele1Met_mt",1);

 recoTree->SetBranchStatus("ele1_recHit_E",1);
 recoTree->SetBranchStatus("ele1_idtype",1);
 
 // set branch address
 recoTree->SetBranchAddress("lumiId",&lumiId_r);
 recoTree->SetBranchAddress("eventId",&eventId_r);
 recoTree->SetBranchAddress("runId",&runId_r);
 recoTree->SetBranchAddress("isW",&isW_r);
 recoTree->SetBranchAddress("rho",&rho_r);
 recoTree->SetBranchAddress("PV_n",&PV_n_r);
 recoTree->SetBranchAddress("ele1_nRecHits",&ele1_nRecHits_r);
 recoTree->SetBranchAddress("ele1_nGgsfTrackHits",&ele1_nGgsfTrackHits_r);
 recoTree->SetBranchAddress("ele1_numberOfLostHits",&ele1_numberOfLostHits_r);
 recoTree->SetBranchAddress("ele1_nAmbiguousGsfTrack",&ele1_nAmbiguousGsfTrack_r);
 recoTree->SetBranchAddress("ele1_isEB",&ele1_isEB_r);
 recoTree->SetBranchAddress("ele1_seedIeta",&ele1_seedIeta_r);
 recoTree->SetBranchAddress("ele1_seedIphi",&ele1_seedIphi_r);
 recoTree->SetBranchAddress("ele1_seedIx",&ele1_seedIx_r);
 recoTree->SetBranchAddress("ele1_seedIy",&ele1_seedIy_r);
 recoTree->SetBranchAddress("ele1_seedZside",&ele1_seedZside_r);
 recoTree->SetBranchAddress("ele1_scNxtal",&ele1_scNxtal_r);

 recoTree->SetBranchAddress("ele1_pt",&ele1_pt_r);
 recoTree->SetBranchAddress("ele1_eta",&ele1_eta_r);
 recoTree->SetBranchAddress("ele1_phi",&ele1_phi_r);
 recoTree->SetBranchAddress("ele1_sigmaIetaIeta",&ele1_sigmaIetaIeta_r);
 recoTree->SetBranchAddress("ele1_DphiIn",&ele1_DphiIn_r);
 recoTree->SetBranchAddress("ele1_DetaIn",&ele1_DetaIn_r);
 recoTree->SetBranchAddress("ele1_HOverE",&ele1_HOverE_r);
 recoTree->SetBranchAddress("ele1_tkIso",&ele1_tkIso_r);
 recoTree->SetBranchAddress("ele1_emIso",&ele1_emIso_r);
 recoTree->SetBranchAddress("ele1_hadIso",&ele1_hadIso_r);
 recoTree->SetBranchAddress("ele1_scERaw",&ele1_scERaw_r);
 recoTree->SetBranchAddress("ele1_scE",&ele1_scE_r);
 recoTree->SetBranchAddress("ele1_es",&ele1_es_r);
 recoTree->SetBranchAddress("ele1_scEta",&ele1_scEta_r);
 recoTree->SetBranchAddress("ele1_scPhi",&ele1_scPhi_r);
 recoTree->SetBranchAddress("ele1_scEtaWidth",&ele1_scEtaWidth_r);
 recoTree->SetBranchAddress("ele1_scPhiWidth",&ele1_scPhiWidth_r);
 recoTree->SetBranchAddress("ele1_tkP",&ele1_tkP_r);
 recoTree->SetBranchAddress("ele1_fbrem",&ele1_fbrem_r);
 recoTree->SetBranchAddress("ele1_dxy_PV",&ele1_dxy_PV_r);
 recoTree->SetBranchAddress("ele1_dz_PV",&ele1_dz_PV_r);
 recoTree->SetBranchAddress("ele1_e5x5",&ele1_e5x5_r);
 recoTree->SetBranchAddress("ele1_e3x3",&ele1_e3x3_r);
 recoTree->SetBranchAddress("ele1_seedE",&ele1_seedE_r);

 recoTree->SetBranchAddress("met_et",&met_r);
 recoTree->SetBranchAddress("met_phi",&met_phi_r);
 recoTree->SetBranchAddress("ele1Met_mt",&ele1Met_mt_r);

 recoTree->SetBranchAddress("ele1_recHit_E",&ele1_recHit_E_r);
 recoTree->SetBranchAddress("ele1_idtype",&ele1_idtype_r);

 recoTree->SetBranchAddress("ele1_eRegrInput_rawE",&ele1_eRegrInput_rawE_r);
 recoTree->SetBranchAddress("ele1_eRegrInput_r9",&ele1_eRegrInput_r9_r);
 recoTree->SetBranchAddress("ele1_eRegrInput_eta",&ele1_eRegrInput_eta_r);
 recoTree->SetBranchAddress("ele1_eRegrInput_phi",&ele1_eRegrInput_phi_r);
 recoTree->SetBranchAddress("ele1_eRegrInput_etaW",&ele1_eRegrInput_etaW_r);
 recoTree->SetBranchAddress("ele1_eRegrInput_phiW",&ele1_eRegrInput_phiW_r);
 recoTree->SetBranchAddress("ele1_eRegrInput_SCsize",&ele1_eRegrInput_SCsize_r);
 recoTree->SetBranchAddress("ele1_eRegrInput_rho",&ele1_eRegrInput_rho_r);
 recoTree->SetBranchAddress("ele1_eRegrInput_hoe",&ele1_eRegrInput_hoe_r);
 recoTree->SetBranchAddress("ele1_eRegrInput_nPV",&ele1_eRegrInput_nPV_r);
 recoTree->SetBranchAddress("ele1_eRegrInput_seed_eta",&ele1_eRegrInput_seed_eta_r);
 recoTree->SetBranchAddress("ele1_eRegrInput_seed_phi",&ele1_eRegrInput_seed_phi_r);
 recoTree->SetBranchAddress("ele1_eRegrInput_seed_E",&ele1_eRegrInput_seed_E_r);
 recoTree->SetBranchAddress("ele1_eRegrInput_sigietaieta",&ele1_eRegrInput_sigietaieta_r);
 recoTree->SetBranchAddress("ele1_eRegrInput_sigiphiiphi",&ele1_eRegrInput_sigiphiiphi_r);
 recoTree->SetBranchAddress("ele1_eRegrInput_sigietaiphi",&ele1_eRegrInput_sigietaiphi_r);
 recoTree->SetBranchAddress("ele1_eRegrInput_eMax",&ele1_eRegrInput_eMax_r);
 recoTree->SetBranchAddress("ele1_eRegrInput_e2nd",&ele1_eRegrInput_e2nd_r);
 recoTree->SetBranchAddress("ele1_eRegrInput_eTop",&ele1_eRegrInput_eTop_r);
 recoTree->SetBranchAddress("ele1_eRegrInput_eBottom",&ele1_eRegrInput_eBottom_r);
 recoTree->SetBranchAddress("ele1_eRegrInput_eLeft",&ele1_eRegrInput_eLeft_r);
 recoTree->SetBranchAddress("ele1_eRegrInput_eRight",&ele1_eRegrInput_eRight_r);
 recoTree->SetBranchAddress("ele1_eRegrInput_e2x5Max",&ele1_eRegrInput_e2x5Max_r);
 recoTree->SetBranchAddress("ele1_eRegrInput_e2x5Top",&ele1_eRegrInput_e2x5Top_r);
 recoTree->SetBranchAddress("ele1_eRegrInput_e2x5Bottom",&ele1_eRegrInput_e2x5Bottom_r);
 recoTree->SetBranchAddress("ele1_eRegrInput_e2x5Left",&ele1_eRegrInput_e2x5Left_r);
 recoTree->SetBranchAddress("ele1_eRegrInput_e2x5Right",&ele1_eRegrInput_e2x5Right_r);
 recoTree->SetBranchAddress("ele1_eRegrInput_seed_ieta",&ele1_eRegrInput_seed_ieta_r);
 recoTree->SetBranchAddress("ele1_eRegrInput_seed_iphi",&ele1_eRegrInput_seed_iphi_r);
 recoTree->SetBranchAddress("ele1_eRegrInput_seed_etaCrySeed",&ele1_eRegrInput_seed_etaCrySeed_r);
 recoTree->SetBranchAddress("ele1_eRegrInput_seed_phiCrySeed",&ele1_eRegrInput_seed_phiCrySeed_r);
 recoTree->SetBranchAddress("ele1_eRegrInput_preshowerEnergyOverRaw",&ele1_eRegrInput_preshowerEnergyOverRaw_r);
 recoTree->SetBranchAddress("ele1_eRegrInput_ecalDrivenSeed",&ele1_eRegrInput_ecalDrivenSeed_r);
 recoTree->SetBranchAddress("ele1_eRegrInput_isEBEtaGap",&ele1_eRegrInput_isEBEtaGap_r);
 recoTree->SetBranchAddress("ele1_eRegrInput_isEBPhiGap",&ele1_eRegrInput_isEBPhiGap_r);
 recoTree->SetBranchAddress("ele1_eRegrInput_eSubClusters",&ele1_eRegrInput_eSubClusters_r);
 recoTree->SetBranchAddress("ele1_eRegrInput_subClusterEnergy_1",&ele1_eRegrInput_subClusterEnergy_1_r);
 recoTree->SetBranchAddress("ele1_eRegrInput_subClusterEnergy_2",&ele1_eRegrInput_subClusterEnergy_2_r);
 recoTree->SetBranchAddress("ele1_eRegrInput_subClusterEnergy_3",&ele1_eRegrInput_subClusterEnergy_3_r);
 recoTree->SetBranchAddress("ele1_eRegrInput_subClusterEta_1",&ele1_eRegrInput_subClusterEta_1_r);
 recoTree->SetBranchAddress("ele1_eRegrInput_subClusterEta_2",&ele1_eRegrInput_subClusterEta_2_r);
 recoTree->SetBranchAddress("ele1_eRegrInput_subClusterEta_3",&ele1_eRegrInput_subClusterEta_3_r);
 recoTree->SetBranchAddress("ele1_eRegrInput_subClusterPhi_1",&ele1_eRegrInput_subClusterPhi_1_r);
 recoTree->SetBranchAddress("ele1_eRegrInput_subClusterPhi_2",&ele1_eRegrInput_subClusterPhi_2_r);
 recoTree->SetBranchAddress("ele1_eRegrInput_subClusterPhi_3",&ele1_eRegrInput_subClusterPhi_3_r);
 recoTree->SetBranchAddress("ele1_eRegrInput_subClusterEmax_1",&ele1_eRegrInput_subClusterEmax_1_r);
 recoTree->SetBranchAddress("ele1_eRegrInput_subClusterEmax_2",&ele1_eRegrInput_subClusterEmax_2_r);
 recoTree->SetBranchAddress("ele1_eRegrInput_subClusterEmax_3",&ele1_eRegrInput_subClusterEmax_3_r);
 recoTree->SetBranchAddress("ele1_eRegrInput_subClusterE3x3_1",&ele1_eRegrInput_subClusterE3x3_1_r);
 recoTree->SetBranchAddress("ele1_eRegrInput_subClusterE3x3_2",&ele1_eRegrInput_subClusterE3x3_2_r);
 recoTree->SetBranchAddress("ele1_eRegrInput_subClusterE3x3_3",&ele1_eRegrInput_subClusterE3x3_3_r);
 recoTree->SetBranchAddress("ele1_eRegrInput_eESClusters",&ele1_eRegrInput_eESClusters_r);
 recoTree->SetBranchAddress("ele1_eRegrInput_eESClusterEnergy_1",&ele1_eRegrInput_eESClusterEnergy_1_r);
 recoTree->SetBranchAddress("ele1_eRegrInput_eESClusterEnergy_2",&ele1_eRegrInput_eESClusterEnergy_2_r);
 recoTree->SetBranchAddress("ele1_eRegrInput_eESClusterEnergy_3",&ele1_eRegrInput_eESClusterEnergy_3_r);
 recoTree->SetBranchAddress("ele1_eRegrInput_eESClusterEta_1",&ele1_eRegrInput_eESClusterEta_1_r);
 recoTree->SetBranchAddress("ele1_eRegrInput_eESClusterEta_2",&ele1_eRegrInput_eESClusterEta_2_r);
 recoTree->SetBranchAddress("ele1_eRegrInput_eESClusterEta_3",&ele1_eRegrInput_eESClusterEta_3_r);
 recoTree->SetBranchAddress("ele1_eRegrInput_eESClusterPhi_1",&ele1_eRegrInput_eESClusterPhi_1_r);
 recoTree->SetBranchAddress("ele1_eRegrInput_eESClusterPhi_2",&ele1_eRegrInput_eESClusterPhi_2_r);
 recoTree->SetBranchAddress("ele1_eRegrInput_eESClusterPhi_3",&ele1_eRegrInput_eESClusterPhi_3_r);
 recoTree->SetBranchAddress("ele1_eRegrInput_pt",&ele1_eRegrInput_pt_r);
 recoTree->SetBranchAddress("ele1_eRegrInput_trackMomentumAtVtxR",&ele1_eRegrInput_trackMomentumAtVtxR_r);
 recoTree->SetBranchAddress("ele1_eRegrInput_fbrem",&ele1_eRegrInput_fbrem_r);
 recoTree->SetBranchAddress("ele1_eRegrInput_charge",&ele1_eRegrInput_charge_r);
 recoTree->SetBranchAddress("ele1_eRegrInput_eSuperClusterOverP",&ele1_eRegrInput_eSuperClusterOverP_r);

 return ;

}
