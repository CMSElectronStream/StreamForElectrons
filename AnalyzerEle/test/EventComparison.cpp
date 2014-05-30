// g++ -Wall -o EventComparison.exe `root-config --glibs --libs --cflags` EventComparison.cpp -lTreePlayer
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

#include "TTree.h"
#include "TFile.h"
#include "TH1F.h"
#include "TTreeIndex.h"
#include "TROOT.h"
#include "TDirectory.h"
#include "TCanvas.h"
#include "TLegend.h"

#include "TStyle.h"


Int_t runId_s,lumiId_s, runId_r,lumiId_r, isW_s, isW_r, isZ_s, isZ_r, PV_n_s, PV_n_r, nele_s, nele_r,ele1_isEB_s,ele1_isEB_r;
Int_t ele1_seedIeta_s,ele1_seedIeta_r,ele1_seedIphi_s,ele1_seedIphi_r,ele1_seedIx_s,ele1_seedIx_r,ele1_seedIy_s,ele1_seedIy_r,ele1_seedZside_r,ele1_seedZside_s,ele1_scNxtal_s,ele1_scNxtal_r;

Long64_t eventId_s, eventId_r ;

Float_t rho_s, rho_r,ele1_pt_s,ele1_pt_r,ele1_eta_s,ele1_eta_r,ele1_phi_s,ele1_phi_r,ele1_sigmaIetaIeta_s, ele1_sigmaIetaIeta_r;
Float_t ele1_DphiIn_s,ele1_DphiIn_r,ele1_DetaIn_s,ele1_DetaIn_r,ele1_HOverE_s,ele1_HOverE_r,ele1_tkIso_s,ele1_tkIso_r,ele1_emIso_s,ele1_emIso_r,ele1_hadIso_s,ele1_hadIso_r;
Float_t ele1_scERaw_s,ele1_scERaw_r,ele1_scE_s,ele1_scE_r,ele1_es_r,ele1_es_s,ele1_scLaserCorr_s,ele1_scLaserCorr_r,ele1_scEta_r,ele1_scEta_s,ele1_scPhi_s,ele1_scPhi_r;
Float_t ele1_scEtaWidth_s,ele1_scEtaWidth_r,ele1_scPhiWidth_s,ele1_scPhiWidth_r,ele1_fEta_s,ele1_fEta_r,ele1_tkP_s,ele1_tkP_r,ele1_fbrem_s,ele1_fbrem_r;
Float_t ele1_dxy_PV_s,ele1_dxy_PV_r,ele1_dz_PV_s,ele1_dz_PV_r,ele1_e5x5_s,ele1_e5x5_r,ele1_e3x3_s,ele1_e3x3_r,ele1_seedE_s,ele1_seedE_r;
Float_t met_s, met_r, met_phi_s, met_phi_r, ele1Met_mt_s, ele1Met_mt_r;

std::vector<float>* ele1_recHit_E_s = new std::vector<float>();
std::vector<float>* ele1_recHit_E_r = new std::vector<float>();
TBranch* b_ele1_recHit_E_s, *b_ele1_recHit_E_r;

std::vector<int>*  ele1_idtype_s = new std::vector<int>(); 
std::vector<int>*  ele1_idtype_r = new std::vector<int>(); 
TBranch* b_ele1_idtype_s, *b_ele1_idtype_r;

typedef std::pair<std::string,TH1F*> mapElement ;

void SetStartingBranchTrees(TTree*, TTree*);

void SetVariablesMapEntry(std::map<std::string,TH1F*> &, std::map<std::string,TH1F*> &, std::map<std::string,TH1F*> &);

int main (int argc, char** argv){

 gStyle->SetOptStat(0);

 if(argc < 3){ std::cerr<<" two few input informations --> exit from the code"<<std::endl; return -1 ; }

 std::string ROOTStyle;
 if(getenv ("ROOTStyle")!=NULL){
  ROOTStyle = getenv ("ROOTStyle");
  gROOT->ProcessLine((".x "+ROOTStyle+"/setTDRStyle.C").c_str());
 }

 gROOT->ProcessLine("#include <vector>");
 
 std::map<std::string,TH1F*> DifferenceMap ;
 std::map<std::string,TH1F*> DistributionStreamMap ;
 std::map<std::string,TH1F*> DistributionRecoMap ;


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
 std::vector<Long64_t> indexRecoVector; indexRecoVector.insert(indexRecoVector.begin(),indexReco,indexReco+TreeIndexStream->GetN());

 std::cout<<"Events in the stream file "<<TreeIndexStream->GetN()<<std::endl;
 std::cout<<"Events in the reco   file "<<TreeIndexStdReco->GetN()<<std::endl;

 SetVariablesMapEntry(DistributionStreamMap,DistributionRecoMap,DifferenceMap);

 for( int iEvent = 0; iEvent < TreeIndexStream->GetN()/100 ; iEvent++ ) {

  if( iEvent%1000 == 0 ) {
   std::cout << "reading saved entry " << iEvent << "::" << TreeIndexStream->GetN() << "\r" << std::flush;
  }
  
  Long64_t localStream = treeStream->LoadTree(indexStream[iEvent]);
  treeStream->GetEntry(localStream);
    
  if(argc >= 4 ) std::cout<<"localStream "<<localStream<<" indexValues Stream "<<indexValuesStream[iEvent]<<" runId_s "<<runId_s<<" lumiId_s "<<lumiId_s<<" eventId_s "<<eventId_s<<std::endl;
    
  std::vector<Long64_t>::iterator itVal = std::find(indexValuesRecoVector.begin(),indexValuesRecoVector.end(),indexValuesStream[iEvent]);        
  if((*itVal)){
   Long64_t localReco = indexRecoVector.at(int(itVal-indexValuesRecoVector.begin()));
   treeStdReco->GetEntry(localReco);
   if(argc >= 4 ) std::cout<<"localStdReco "<<localReco<<" indexValues Reco "<<(*itVal)<<" runId_r "<<runId_r<<" lumiId_r "<<lumiId_r<<" eventId_r "<<eventId_r<<std::endl;

   if( isW_s != 1 or isW_r != 1 ) continue;

     // event properties
   DistributionStreamMap["isW"]->Fill(isW_s);
   DistributionRecoMap["isW"]->Fill(isW_r);
   DifferenceMap["isW"]->Fill((isW_s-isW_r));

   DistributionStreamMap["PV_n"]->Fill(PV_n_s);
   DistributionRecoMap["PV_n"]->Fill(PV_n_r);
   DifferenceMap["PV_n"]->Fill(PV_n_s-PV_n_r);

   DistributionStreamMap["rho"]->Fill(rho_s);       
   DistributionRecoMap["rho"]->Fill(rho_r);
   DifferenceMap["rho"]->Fill(rho_s-rho_r);

   DistributionStreamMap["met_et"]->Fill(met_s);       
   DistributionRecoMap["met_et"]->Fill(met_r);
   DifferenceMap["met_et"]->Fill(met_s-met_r);

   DistributionStreamMap["met_phi"]->Fill(met_phi_s);       
   DistributionRecoMap["met_phi"]->Fill(met_phi_r);
   DifferenceMap["met_phi"]->Fill(met_phi_s-met_phi_r);

   DistributionStreamMap["ele1Met_mt"]->Fill(ele1Met_mt_s);       
   DistributionRecoMap["ele1Met_mt"]->Fill(ele1Met_mt_r);
   DifferenceMap["ele1Met_mt"]->Fill(ele1Met_mt_s-ele1Met_mt_r);

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

<<<<<<< Updated upstream
   DistributionStreamMap["ele1_tkP"]->Fill(ele1_tkP_s);       
   DistributionRecoMap["ele1_tkP"]->Fill(ele1_tkP_r);
   DifferenceMap["ele1_tkP"]->Fill(ele1_tkP_s-ele1_tkP_r);

   DistributionStreamMap["ele1_dz_PV"]->Fill(ele1_dz_PV_s);       
   DistributionRecoMap["ele1_dz_PV"]->Fill(ele1_dz_PV_r);
   DifferenceMap["ele1_dz_PV"]->Fill(ele1_dz_PV_s-ele1_dz_PV_r);
=======
     DistributionStreamMap["ele1_dz_PV"]->Fill(ele1_dz_PV_s);       
     DistributionRecoMap["ele1_dz_PV"]->Fill(ele1_dz_PV_r);
     RatioMap["ele1_dz_PV"]->Fill(ele1_dz_PV_s-ele1_dz_PV_r);
>>>>>>> Stashed changes

   DistributionStreamMap["ele1_e5x5"]->Fill(ele1_e5x5_s);       
   DistributionRecoMap["ele1_e5x5"]->Fill(ele1_e5x5_r);
   DifferenceMap["ele1_e5x5"]->Fill(ele1_e5x5_s-ele1_e5x5_r);

   DistributionStreamMap["ele1_e3x3"]->Fill(ele1_e3x3_s);       
   DistributionRecoMap["ele1_e3x3"]->Fill(ele1_e3x3_r);
   DifferenceMap["ele1_e3x3"]->Fill(ele1_e3x3_s-ele1_e3x3_r);

   DistributionStreamMap["ele1_scNxtal"]->Fill(ele1_scNxtal_s);       
   DistributionRecoMap["ele1_scNxtal"]->Fill(ele1_scNxtal_r);
   DifferenceMap["ele1_scNxtal"]->Fill(ele1_scNxtal_s-ele1_scNxtal_r);


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

  // save canvas
 system("mkdir plots");

 outputFile->cd();

 TLegend* leg = new TLegend(0.5,0.6,0.9,0.9);

 std::map<std::string,TH1F*>::iterator itMap1 = DistributionStreamMap.begin();
 std::map<std::string,TH1F*>::iterator itMap2 = DistributionRecoMap.begin();

 for( ; itMap1 != DistributionStreamMap.end() and itMap2 != DistributionRecoMap.end() ; ++itMap1, ++itMap2){

  TString tempName = Form ("Comparison_%s",(itMap1)->first.c_str());
//   std::cout << " tempName.Data() = " << tempName.Data() << std::endl;
  TCanvas* canvasTemp = new TCanvas(tempName.Data(),"",600,650);
  canvasTemp->cd();
  (itMap1)->second->GetXaxis()->SetTitle(((itMap1)->first).c_str());
  (itMap1)->second->GetYaxis()->SetTitle("Events");
  (itMap1)->second->SetMarkerColor(kRed);
  (itMap1)->second->SetMarkerStyle(20);
  (itMap1)->second->SetMarkerSize(1);
  (itMap1)->second->Draw("E");


  (itMap2)->second->GetXaxis()->SetTitle(((itMap2)->first).c_str());
  (itMap2)->second->GetYaxis()->SetTitle("Events");
  (itMap2)->second->SetMarkerColor(kBlue);
  (itMap2)->second->SetMarkerStyle(20);
  (itMap2)->second->SetMarkerSize(1);
  (itMap2)->second->Draw("Esame");

  leg->SetFillColor(0);
  leg->SetFillStyle(0);
  leg->SetBorderSize(0);
  leg->SetLineColor(0);
  leg->SetLineWidth(0);
  leg->SetLineStyle(0);
  leg->AddEntry((itMap1)->second->GetName(),"Electron Stream");
  leg->AddEntry((itMap2)->second->GetName(),"Standard Reco");
  leg->Draw("same");

  canvasTemp->SaveAs(("plots/Comparison_"+(itMap1)->first+".png").c_str(),"png");
  canvasTemp->Write();
  leg->Clear();
  delete canvasTemp;
 }

 std::map<std::string,TH1F*>::iterator itMap3 = DifferenceMap.begin();

 for( ; itMap3 != DifferenceMap.end() ; ++itMap3){

  TString tempName = Form ("Difference_%s",(itMap3)->first.c_str());
//   std::cout << " tempName.Data() = " << tempName.Data() << std::endl;
  TCanvas* canvasTempDiff = new TCanvas(tempName.Data(),"",600,650);
  canvasTempDiff->cd();
  canvasTempDiff->SetLogy();
  (itMap3)->second->GetXaxis()->SetTitle(("Diff "+(itMap3)->first).c_str());
  (itMap3)->second->GetYaxis()->SetTitle("Events");
  (itMap3)->second->SetMarkerColor(kBlack);
  (itMap3)->second->SetMarkerStyle(20);
  (itMap3)->second->SetMarkerSize(1);
  (itMap3)->second->Draw("E");
  canvasTempDiff->SaveAs(("plots/Difference_"+(itMap3)->first+".png").c_str(),"png"); 
  canvasTempDiff->Write();
  delete canvasTempDiff;
 }

 outputFile->Close();


 return 0 ;
}

void SetVariablesMapEntry(std::map<std::string,TH1F*> & DistributionStreamMap, std::map<std::string,TH1F*> & DistributionRecoMap, std::map<std::string,TH1F*> & DifferenceMap){

  //Event properties
 DistributionStreamMap.insert(mapElement("isW",new TH1F("isWStream","",2,0,1)));       
 DistributionRecoMap.insert(mapElement("isW",new TH1F("isWReco","",2,0,1)));       
 DifferenceMap.insert(mapElement("isW",new TH1F("isWRatio","",40,-5,5)));       

 DistributionStreamMap.insert(mapElement("PV_n",new TH1F("PV_nStream","",40,0,40)));       
 DistributionRecoMap.insert(mapElement("PV_n",new TH1F("PV_nReco","",40,0,40)));       
 DifferenceMap.insert(mapElement("PV_n",new TH1F("PV_nRatio","",40,-20,20)));       

 DistributionStreamMap.insert(mapElement("rho",new TH1F("rhoStream","",50,0,100)));       
 DistributionRecoMap.insert(mapElement("rho",new TH1F("rhoReco","",50,0,100)));       
 DifferenceMap.insert(mapElement("rho",new TH1F("rhoRatio","",50,-25,25)));       

 DistributionStreamMap.insert(mapElement("met_et",new TH1F("met_etStream","",50,0,200)));       
 DistributionRecoMap.insert(mapElement("met_et",new TH1F("met_etReco","",50,0,200)));       
 DifferenceMap.insert(mapElement("met_et",new TH1F("met_etRatio","",100,-50,50)));       

 DistributionStreamMap.insert(mapElement("met_phi",new TH1F("met_phiStream","",50,0,2*3.14)));       
 DistributionRecoMap.insert(mapElement("met_phi",new TH1F("met_phiReco","",50,0,2*3.14)));       
 DifferenceMap.insert(mapElement("met_phi",new TH1F("met_phiRatio","",100,-3.14,3.14)));       

 DistributionStreamMap.insert(mapElement("ele1Met_mt",new TH1F("ele1Met_mtStream","",100,0,200)));       
 DistributionRecoMap.insert(mapElement("ele1Met_mt",new TH1F("ele1Met_mtReco","",100,0,200)));       
 DifferenceMap.insert(mapElement("ele1Met_mt",new TH1F("ele1Met_mtRatio","",100,-25,25)));       

  // electron variables
 DistributionStreamMap.insert(mapElement("ele1_pt",new TH1F("ele1_ptStream","",50,30,300)));       
 DistributionRecoMap.insert(mapElement("ele1_pt",new TH1F("ele1_ptReco","",50,30,300)));       
 DifferenceMap.insert(mapElement("ele1_pt",new TH1F("ele1_ptRatio","",200,-1,1)));       

 DistributionStreamMap.insert(mapElement("ele1_eta",new TH1F("ele1_etaStream","",50,-3,3)));       
 DistributionRecoMap.insert(mapElement("ele1_eta",new TH1F("ele1_etaReco","",50,-3,3)));       
 DifferenceMap.insert(mapElement("ele1_eta",new TH1F("ele1_etaRatio","",50,-0.5,0.5)));       

 DistributionStreamMap.insert(mapElement("ele1_phi",new TH1F("ele1_phiStream","",50,-3.14,3.14)));       
 DistributionRecoMap.insert(mapElement("ele1_phi",new TH1F("ele1_phiReco","",50,-3.14,3.14)));       
 DifferenceMap.insert(mapElement("ele1_phi",new TH1F("ele1_phiRatio","",50,-0.5,.5)));       

 DistributionStreamMap.insert(mapElement("ele1_sigmaIetaIeta",new TH1F("ele1_sigmaIetaIetaStream","",50,0,5)));       
 DistributionRecoMap.insert(mapElement("ele1_sigmaIetaIeta",new TH1F("ele1_sigmaIetaIetaReco","",50,0,5)));       
 DifferenceMap.insert(mapElement("ele1_sigmaIetaIeta",new TH1F("ele1_sigmaIetaIetaRatio","",50,-5,5)));       

 DistributionStreamMap.insert(mapElement("ele1_DphiIn",new TH1F("ele1_DphiInStream","",50,-0.01,0.01)));       
 DistributionRecoMap.insert(mapElement("ele1_DphiIn",new TH1F("ele1_DphiInReco","",50,-0.01,0.01)));       
 DifferenceMap.insert(mapElement("ele1_DphiIn",new TH1F("ele1_DphiInRatio","",50,-0.001,0.001)));       

 DistributionStreamMap.insert(mapElement("ele1_DetaIn",new TH1F("ele1_DetaInStream","",50,-0.005,0.005)));       
 DistributionRecoMap.insert(mapElement("ele1_DetaIn",new TH1F("ele1_DetaInReco","",50,-0.005,0.005)));       
 DifferenceMap.insert(mapElement("ele1_DetaIn",new TH1F("ele1_DetaInRatio","",50,-0.0005,0.0005)));       

 DistributionStreamMap.insert(mapElement("ele1_HOverE",new TH1F("ele1_HOverEStream","",50,0,0.05)));       
 DistributionRecoMap.insert(mapElement("ele1_HOverE",new TH1F("ele1_HOverEReco","",50,0,0.05)));       
 DifferenceMap.insert(mapElement("ele1_HOverE",new TH1F("ele1_HOverERatio","",50,-0.05,0.05)));       

 DistributionStreamMap.insert(mapElement("ele1_tkIso",new TH1F("ele1_tkIsoStream","",50,0,10)));       
 DistributionRecoMap.insert(mapElement("ele1_tkIso",new TH1F("ele1_tkIsoReco","",50,0,10)));       
 DifferenceMap.insert(mapElement("ele1_tkIso",new TH1F("ele1_tkIsoRatio","",100,-5,5)));       

 DistributionStreamMap.insert(mapElement("ele1_emIso",new TH1F("ele1_emIsoStream","",50,0,10)));       
 DistributionRecoMap.insert(mapElement("ele1_emIso",new TH1F("ele1_emIsoReco","",50,0,10)));       
 DifferenceMap.insert(mapElement("ele1_emIso",new TH1F("ele1_emIsoRatio","",100,-5,5)));       

 DistributionStreamMap.insert(mapElement("ele1_hadIso",new TH1F("ele1_hadIsoStream","",50,0,10)));       
 DistributionRecoMap.insert(mapElement("ele1_hadIso",new TH1F("ele1_hadIsoReco","",50,0,10)));       
 DifferenceMap.insert(mapElement("ele1_hadIso",new TH1F("ele1_hadIsoRatio","",100,-5,5)));       

 DistributionStreamMap.insert(mapElement("ele1_scE",new TH1F("ele1_scEStream","",50,30,300)));       
 DistributionRecoMap.insert(mapElement("ele1_scE",new TH1F("ele1_scEReco","",50,30,300)));       
 DifferenceMap.insert(mapElement("ele1_scE",new TH1F("ele1_scERatio","",100,-3,3)));       

 DistributionStreamMap.insert(mapElement("ele1_es",new TH1F("ele1_esStream","",50,0,30)));       
 DistributionRecoMap.insert(mapElement("ele1_es",new TH1F("ele1_esReco","",50,0,30)));       
 DifferenceMap.insert(mapElement("ele1_es",new TH1F("ele1_esRatio","",40,-3,3)));       

 DistributionStreamMap.insert(mapElement("ele1_scEta",new TH1F("ele1_scEtaStream","",50,-2.5,2.5)));       
 DistributionRecoMap.insert(mapElement("ele1_scEta",new TH1F("ele1_scEtaReco","",50,-2.5,2.5)));       
 DifferenceMap.insert(mapElement("ele1_scEta",new TH1F("ele1_scEtaRatio","",50,-0.5,0.5)));       

 DistributionStreamMap.insert(mapElement("ele1_scPhi",new TH1F("ele1_scPhiStream","",50,-3.14,3.14)));       
 DistributionRecoMap.insert(mapElement("ele1_scPhi",new TH1F("ele1_scPhiReco","",50,-3.14,3.14)));       
 DifferenceMap.insert(mapElement("ele1_scPhi",new TH1F("ele1_scPhiRatio","",50,-0.5,0.5)));       

 DistributionStreamMap.insert(mapElement("ele1_scEtaWidth",new TH1F("ele1_scEtaWidthStream","",50,-1,1)));       
 DistributionRecoMap.insert(mapElement("ele1_scEtaWidth",new TH1F("ele1_scEtaWidthReco","",50,-1,1)));       
 DifferenceMap.insert(mapElement("ele1_scEtaWidth",new TH1F("ele1_scEtaWidthRatio","",50,-1,1)));       

 DistributionStreamMap.insert(mapElement("ele1_scPhiWidth",new TH1F("ele1_scPhiWidthStream","",50,0,10)));       
 DistributionRecoMap.insert(mapElement("ele1_scPhiWidth",new TH1F("ele1_scPhiWidthReco","",50,0,10)));       
 DifferenceMap.insert(mapElement("ele1_scPhiWidth",new TH1F("ele1_scPhiWidthRatio","",50,-5,5)));       

 DistributionStreamMap.insert(mapElement("ele1_tkP",new TH1F("ele1_tkPStream","",50,0,300)));       
 DistributionRecoMap.insert(mapElement("ele1_tkP",new TH1F("ele1_tkPReco","",50,0,300)));       
 DifferenceMap.insert(mapElement("ele1_tkP",new TH1F("ele1_tkPRatio","",100,-0.01,0.01)));       

 DistributionStreamMap.insert(mapElement("ele1_fbrem",new TH1F("ele1_fbremStream","",50,-1,1)));       
 DistributionRecoMap.insert(mapElement("ele1_fbrem",new TH1F("ele1_fbremReco","",50,-1,1)));       
 DifferenceMap.insert(mapElement("ele1_fbrem",new TH1F("ele1_fbremRatio","",100,-0.02,0.02)));       

 DistributionStreamMap.insert(mapElement("ele1_dxy_PV",new TH1F("ele1_dxy_PVStream","",50,0,0.5)));       
 DistributionRecoMap.insert(mapElement("ele1_dxy_PV",new TH1F("ele1_dxy_PVReco","",50,0,0.5)));       
 DifferenceMap.insert(mapElement("ele1_dxy_PV",new TH1F("ele1_dxy_PVRatio","",100,-0.5,0.5)));       

 DistributionStreamMap.insert(mapElement("ele1_dz_PV",new TH1F("ele1_dz_PVStream","",50,0,20)));       
 DistributionRecoMap.insert(mapElement("ele1_dz_PV",new TH1F("ele1_dz_PVReco","",50,0,20)));       
 DifferenceMap.insert(mapElement("ele1_dz_PV",new TH1F("ele1_dz_PVRatio","",50,-20,20)));       

 DistributionStreamMap.insert(mapElement("ele1_e5x5",new TH1F("ele1_e5x5Stream","",100,0,200)));       
 DistributionRecoMap.insert(mapElement("ele1_e5x5",new TH1F("ele1_e5x5Reco","",100,0,200)));       
 DifferenceMap.insert(mapElement("ele1_e5x5",new TH1F("ele1_e5x5Ratio","",100,-25,25)));       

 DistributionStreamMap.insert(mapElement("ele1_e3x3",new TH1F("ele1_e3x3Stream","",100,0,200)));       
 DistributionRecoMap.insert(mapElement("ele1_e3x3",new TH1F("ele1_e3x3Reco","",100,0,200)));       
 DifferenceMap.insert(mapElement("ele1_e3x3",new TH1F("ele1_e3x3Ratio","",100,-25,25)));       

 DistributionStreamMap.insert(mapElement("ele1_seedE",new TH1F("ele1_seedEStream","",100,0,200)));       
 DistributionRecoMap.insert(mapElement("ele1_seedE",new TH1F("ele1_seedEReco","",100,0,200)));       
 DifferenceMap.insert(mapElement("ele1_seedE",new TH1F("ele1_seedERatio","",100,-25,25)));       

 DistributionStreamMap.insert(mapElement("ele1_seedIeta",new TH1F("ele1_seedIetaStream","",50,0,171)));       
 DistributionRecoMap.insert(mapElement("ele1_seedIeta",new TH1F("ele1_seedIetaReco","",50,0,171)));       
 DifferenceMap.insert(mapElement("ele1_seedIeta",new TH1F("ele1_seedIetaRatio","",10,-5,5)));       

 DistributionStreamMap.insert(mapElement("ele1_seedIphi",new TH1F("ele1_seedIphiStream","",50,0,360)));       
 DistributionRecoMap.insert(mapElement("ele1_seedIphi",new TH1F("ele1_seedIphiReco","",50,0,360)));       
 DifferenceMap.insert(mapElement("ele1_seedIphi",new TH1F("ele1_seedIphiRatio","",10,-5,5)));       

 DistributionStreamMap.insert(mapElement("ele1_seedIx",new TH1F("ele1_seedIxStream","",50,0,50)));       
 DistributionRecoMap.insert(mapElement("ele1_seedIx",new TH1F("ele1_seedIxReco","",50,0,50)));       
 DifferenceMap.insert(mapElement("ele1_seedIx",new TH1F("ele1_seedIxRatio","",10,-5,5)));       

 DistributionStreamMap.insert(mapElement("ele1_seedIy",new TH1F("ele1_seedIyStream","",50,0,50)));       
 DistributionRecoMap.insert(mapElement("ele1_seedIy",new TH1F("ele1_seedIyReco","",50,0,50)));       
 DifferenceMap.insert(mapElement("ele1_seedIy",new TH1F("ele1_seedIyRatio","",10,-5,5)));       

 DistributionStreamMap.insert(mapElement("ele1_scNxtal",new TH1F("ele1_scNxtalStream","",200,0,200)));       
 DistributionRecoMap.insert(mapElement("ele1_scNxtal",new TH1F("ele1_scNxtalReco","",200,0,200)));       
 DifferenceMap.insert(mapElement("ele1_scNxtal",new TH1F("ele1_scNxtalRatio","",40,-20,20)));       

 return ;

}
  

void SetStartingBranchTrees(TTree* streamTree, TTree* recoTree){

 streamTree->SetBranchStatus("*",0);
 streamTree->SetBranchStatus("lumiId",1);
 streamTree->SetBranchStatus("isW",1);
 streamTree->SetBranchStatus("PV_n",1);
 streamTree->SetBranchStatus("eventId",1);
 streamTree->SetBranchStatus("runId",1);
 streamTree->SetBranchStatus("rho",1);
 streamTree->SetBranchStatus("ele1_pt",1);
 streamTree->SetBranchStatus("ele1_isEB",1);
 streamTree->SetBranchStatus("ele1_eta",1);
 streamTree->SetBranchStatus("ele1_phi",1);
 streamTree->SetBranchStatus("ele1_isEB",1);
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
 streamTree->SetBranchStatus("ele1_scLaserCorr",1);
 streamTree->SetBranchStatus("ele1_scEta",1);
 streamTree->SetBranchStatus("ele1_scPhi",1);
 streamTree->SetBranchStatus("ele1_scEtaWidth",1);
 streamTree->SetBranchStatus("ele1_scPhiWidth",1);
 streamTree->SetBranchStatus("ele1_fEta",1);
 streamTree->SetBranchStatus("ele1_tkP",1);
 streamTree->SetBranchStatus("ele1_fbrem",1);
 streamTree->SetBranchStatus("ele1_dxy_PV",1);
 streamTree->SetBranchStatus("ele1_dz_PV",1);
 streamTree->SetBranchStatus("ele1_e5x5",1);
 streamTree->SetBranchStatus("ele1_e3x3",1);
 streamTree->SetBranchStatus("ele1_seedE",1);
 streamTree->SetBranchStatus("ele1_seedIeta",1);
 streamTree->SetBranchStatus("ele1_seedIphi",1);
 streamTree->SetBranchStatus("ele1_seedIx",1);
 streamTree->SetBranchStatus("ele1_seedIy",1);
 streamTree->SetBranchStatus("ele1_seedZside",1);
 streamTree->SetBranchStatus("ele1_scNxtal",1);
 streamTree->SetBranchStatus("ele1_recHit_E",1);
 streamTree->SetBranchStatus("ele1_idtype",1);
 streamTree->SetBranchStatus("met_et",1);
 streamTree->SetBranchStatus("met_phi",1);
 streamTree->SetBranchStatus("ele1Met_mt",1);

 streamTree->SetBranchAddress("lumiId",&lumiId_s);
 streamTree->SetBranchAddress("eventId",&eventId_s);
 streamTree->SetBranchAddress("runId",&runId_s);
 streamTree->SetBranchAddress("isW",&isW_s);
 streamTree->SetBranchAddress("PV_n",&PV_n_s);
 streamTree->SetBranchAddress("rho",&rho_s);
 streamTree->SetBranchAddress("ele1_isEB",&ele1_isEB_s);
 streamTree->SetBranchAddress("ele1_pt",&ele1_pt_s);
 streamTree->SetBranchAddress("ele1_eta",&ele1_eta_s);
 streamTree->SetBranchAddress("ele1_phi",&ele1_phi_s);
 streamTree->SetBranchAddress("ele1_isEB",&ele1_isEB_s);
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
 streamTree->SetBranchAddress("ele1_scLaserCorr",&ele1_scLaserCorr_s);
 streamTree->SetBranchAddress("ele1_scEta",&ele1_scEta_s);
 streamTree->SetBranchAddress("ele1_scPhi",&ele1_scPhi_s);
 streamTree->SetBranchAddress("ele1_scEtaWidth",&ele1_scEtaWidth_s);
 streamTree->SetBranchAddress("ele1_scPhiWidth",&ele1_scPhiWidth_s);
 streamTree->SetBranchAddress("ele1_fEta",&ele1_fEta_s);
 streamTree->SetBranchAddress("ele1_tkP",&ele1_tkP_s);
 streamTree->SetBranchAddress("ele1_fbrem",&ele1_fbrem_s);
 streamTree->SetBranchAddress("ele1_dxy_PV",&ele1_dxy_PV_s);
 streamTree->SetBranchAddress("ele1_dz_PV",&ele1_dz_PV_s);
 streamTree->SetBranchAddress("ele1_e5x5",&ele1_e5x5_s);
 streamTree->SetBranchAddress("ele1_e3x3",&ele1_e3x3_s);
 streamTree->SetBranchAddress("ele1_seedE",&ele1_seedE_s);
 streamTree->SetBranchAddress("ele1_seedIeta",&ele1_seedIeta_s);
 streamTree->SetBranchAddress("ele1_seedIphi",&ele1_seedIphi_s);
 streamTree->SetBranchAddress("ele1_seedIx",&ele1_seedIx_s);
 streamTree->SetBranchAddress("ele1_seedIy",&ele1_seedIy_s);
 streamTree->SetBranchAddress("ele1_seedZside",&ele1_seedZside_s);
 streamTree->SetBranchAddress("ele1_scNxtal",&ele1_scNxtal_s);
 streamTree->SetBranchAddress("ele1_recHit_E",&ele1_recHit_E_s,&b_ele1_recHit_E_s);
 streamTree->SetBranchAddress("ele1_idtype",&ele1_idtype_s,&b_ele1_idtype_s);
 streamTree->SetBranchAddress("met_et",&met_s);
 streamTree->SetBranchAddress("met_phi",&met_phi_s);
 streamTree->SetBranchAddress("ele1Met_mt",&ele1Met_mt_s);
  

 recoTree->SetBranchStatus("lumiId",1);
 recoTree->SetBranchStatus("eventId",1);
 recoTree->SetBranchStatus("runId",1);
 recoTree->SetBranchStatus("rho",1);
 recoTree->SetBranchStatus("isW",1);
 recoTree->SetBranchStatus("PV_n",1);
 recoTree->SetBranchStatus("ele1_isEB",1);
 recoTree->SetBranchStatus("ele1_pt",1);
 recoTree->SetBranchStatus("ele1_eta",1);
 recoTree->SetBranchStatus("ele1_phi",1);
 recoTree->SetBranchStatus("ele1_isEB",1);
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
 recoTree->SetBranchStatus("ele1_scLaserCorr",1);
 recoTree->SetBranchStatus("ele1_scEta",1);
 recoTree->SetBranchStatus("ele1_scPhi",1);
 recoTree->SetBranchStatus("ele1_scEtaWidth",1);
 recoTree->SetBranchStatus("ele1_scPhiWidth",1);
 recoTree->SetBranchStatus("ele1_fEta",1);
 recoTree->SetBranchStatus("ele1_tkP",1);
 recoTree->SetBranchStatus("ele1_fbrem",1);
 recoTree->SetBranchStatus("ele1_dxy_PV",1);
 recoTree->SetBranchStatus("ele1_dz_PV",1);
 recoTree->SetBranchStatus("ele1_e5x5",1);
 recoTree->SetBranchStatus("ele1_e3x3",1);
 recoTree->SetBranchStatus("ele1_seedE",1);
 recoTree->SetBranchStatus("ele1_seedIeta",1);
 recoTree->SetBranchStatus("ele1_seedIphi",1);
 recoTree->SetBranchStatus("ele1_seedIx",1);
 recoTree->SetBranchStatus("ele1_seedIy",1);
 recoTree->SetBranchStatus("ele1_seedZside",1);
 recoTree->SetBranchStatus("ele1_recHit_E",1);
 recoTree->SetBranchStatus("ele1_idtype",1);
 recoTree->SetBranchStatus("met_et",1);
 recoTree->SetBranchStatus("met_phi",1);
 recoTree->SetBranchStatus("ele1Met_mt",1);

 recoTree->SetBranchAddress("lumiId",&lumiId_r);
 recoTree->SetBranchAddress("eventId",&eventId_r);
 recoTree->SetBranchAddress("runId",&runId_r);
 recoTree->SetBranchAddress("rho",&rho_r);
 recoTree->SetBranchAddress("isW",&isW_r);
 recoTree->SetBranchAddress("PV_n",&PV_n_r);
 recoTree->SetBranchAddress("ele1_isEB",&ele1_isEB_r);
 recoTree->SetBranchAddress("ele1_pt",&ele1_pt_r);
 recoTree->SetBranchAddress("ele1_eta",&ele1_eta_r);
 recoTree->SetBranchAddress("ele1_phi",&ele1_phi_r);
 recoTree->SetBranchAddress("ele1_isEB",&ele1_isEB_r);
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
 recoTree->SetBranchAddress("ele1_scLaserCorr",&ele1_scLaserCorr_r);
 recoTree->SetBranchAddress("ele1_scEta",&ele1_scEta_r);
 recoTree->SetBranchAddress("ele1_scPhi",&ele1_scPhi_r);
 recoTree->SetBranchAddress("ele1_scEtaWidth",&ele1_scEtaWidth_r);
 recoTree->SetBranchAddress("ele1_scPhiWidth",&ele1_scPhiWidth_r);
 recoTree->SetBranchAddress("ele1_fEta",&ele1_fEta_r);
 recoTree->SetBranchAddress("ele1_tkP",&ele1_tkP_r);
 recoTree->SetBranchAddress("ele1_fbrem",&ele1_fbrem_r);
 recoTree->SetBranchAddress("ele1_dxy_PV",&ele1_dxy_PV_r);
 recoTree->SetBranchAddress("ele1_dz_PV",&ele1_dz_PV_r);
 recoTree->SetBranchAddress("ele1_e5x5",&ele1_e5x5_r);
 recoTree->SetBranchAddress("ele1_e3x3",&ele1_e3x3_r);
 recoTree->SetBranchAddress("ele1_seedE",&ele1_seedE_r);
 recoTree->SetBranchAddress("ele1_seedIeta",&ele1_seedIeta_r);
 recoTree->SetBranchAddress("ele1_seedIphi",&ele1_seedIphi_r);
 recoTree->SetBranchAddress("ele1_seedIx",&ele1_seedIx_r);
 recoTree->SetBranchAddress("ele1_seedIy",&ele1_seedIy_r);
 recoTree->SetBranchAddress("ele1_seedZside",&ele1_seedZside_r);
 recoTree->SetBranchAddress("ele1_scNxtal",&ele1_scNxtal_r);
 recoTree->SetBranchAddress("ele1_recHit_E",&ele1_recHit_E_r,&b_ele1_recHit_E_r);
 recoTree->SetBranchAddress("ele1_idtype",&ele1_idtype_r,&b_ele1_idtype_r);
 recoTree->SetBranchAddress("met_et",&met_r);
 recoTree->SetBranchAddress("met_phi",&met_phi_r);
 recoTree->SetBranchAddress("ele1Met_mt",&ele1Met_mt_r);

 return ;
}
