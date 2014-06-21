#include "L1Ntuple.h"
#include "L1AlgoFactory.h"
#include <algorithm>
#include<map>

#include "TCanvas.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TF1.h"
#include "TGraph2D.h"
#include "TGraph.h"
#include "TObjArray.h"

Double_t convertRegionEta(int iEta) {

  static const double rgnEtaValues[11] = {
     0.174, // HB and inner HE bins are 0.348 wide
     0.522,
     0.870,
     1.218,
     1.566,
     1.956, // Last two HE bins are 0.432 and 0.828 wide
     2.586,
     3.250, // HF bins are 0.5 wide
     3.750,
     4.250,
     4.750
  };

  if(iEta < 11)        return -rgnEtaValues[-(iEta - 10)]; // 0-10 are negative eta values  
  else if (iEta < 22)  return rgnEtaValues[iEta - 11];     // 11-21 are positive eta values  
  return -9;
}


class BasicRatePlots : public L1Ntuple{

 public :
  
  //constructor    
  BasicRatePlots(std::string filename) : L1Ntuple(filename) {}
  BasicRatePlots()  {}
  ~BasicRatePlots() {}
  
  void run(bool runOnData, std::string resultTag, int minLs, int maxLs, 
	   float crossSec, float avPU, int nBunches, int isCrossSec, int nEvents = 0);
  
 private :
  
  void FillBits();

  float ScaleFactor(float nZeroBias, float nBunches);
  float SingleEGEta(float ptCut, bool doIso);

  //GMT stuff
  float DttfPt();
  float RpcbPt();
  float RpcfPt();
  float CsctfPt();

  void setRateError(TH1F* histo);

  float computeAvgLumi(float xSec, float avPU, int nBunches) { return 11246. * avPU * nBunches / (1E7 * xSec); };

  bool PhysicsBits[128];

  L1AlgoFactory *algoFactory;

  std::map<std::string,TH1F*> hTH1F;
  std::map<std::string,TH2F*> hTH2F;

};

void BasicRatePlots::FillBits() {
  //Fill the physics bits:
  for (int ibit=0; ibit < 128; ibit++) {
      PhysicsBits[ibit] = 0;
      if (ibit<64) PhysicsBits[ibit] = (gt_->tw1[2]>>ibit)&1;
      else PhysicsBits[ibit] = (gt_->tw2[2]>>(ibit-64))&1;
    }
}       

// ------------------------------------------------------------------

// scale factor computed w.r.t. ZeroBias rate fratcion and # bunches 
float BasicRatePlots::ScaleFactor(float nZeroBias, float nBunches) {

  float scal = 11246.*0.001; // ZB per bunch in kHz --> LHC frequency 
  scal /= nZeroBias;
  scal *= nBunches;

  return scal;
}

float BasicRatePlots::DttfPt()  {

  float maxPt = -10;
  
  int Nmu = gmt_ -> Ndt;
  for (int imu=0; imu < Nmu; imu++) 
    {
      int bx = gmt_ -> Bxdt[imu];           // BX = 0, +/- 1 or +/- 2
      if (bx != 0) continue;
      float pt = gmt_ -> Ptdt[imu];         // get pt to get highest pt one
      if ( pt > maxPt) maxPt = pt;
    }
  
  return maxPt;
}

float BasicRatePlots::RpcbPt(){

  float maxPt = -10;
  
  int Nmu = gmt_ -> Nrpcb;
  for (int imu=0; imu < Nmu; imu++) 
    {
      int bx = gmt_ -> Bxrpcb[imu];           // BX = 0, +/- 1 or +/- 2
      if (bx != 0) continue;
      float pt = gmt_ -> Ptrpcb[imu];         // get pt to get highest pt one
      if ( pt > maxPt)  maxPt = pt;
    }
  
  return maxPt;
}

float BasicRatePlots::CsctfPt(){

  float maxPt = -10;
  
  int Nmu = gmt_ -> Ncsc;
  for (int imu=0; imu < Nmu; imu++) 
    {
      int bx = gmt_ -> Bxcsc[imu];           // BX = 0, +/- 1 or +/- 2
      if (bx != 0) continue;
      float pt = gmt_ -> Ptcsc[imu];         // get pt to get highest pt one
      if ( pt > maxPt) maxPt = pt;
    }
  
  return maxPt;
}

float BasicRatePlots::RpcfPt()  {

  float maxPt = -10;
  
  int Nmu = gmt_ -> Nrpcf;
  for (int imu=0; imu < Nmu; imu++) 
    {
      int bx = gmt_ -> Bxrpcf[imu];           // BX = 0, +/- 1 or +/- 2
      if (bx != 0) continue;
      float pt = gmt_ -> Ptrpcf[imu];         // get pt to get highest pt one
      if ( pt > maxPt) maxPt = pt;
    }
  
  return maxPt;
}

float BasicRatePlots::SingleEGEta(float ptCut, bool doIso) {

  float maxPt = -10;
  float iEGMaxPt = -10;

  Int_t Nele = gt_ -> Nele;
  for (Int_t ue=0; ue < Nele; ue++) {
    Int_t bx = gt_ -> Bxel[ue];        		
    if (bx != 0) continue;
    Bool_t iso = gt_ -> Isoel[ue];
    if (!iso && doIso) continue;
    Float_t pt = gt_ -> Rankel[ue];    // the rank of the electron
    if ( pt >= maxPt) {
	maxPt = pt;
	iEGMaxPt = ue;
    }
  }

  float eta = -10.;
  int ieta = iEGMaxPt>=0 && maxPt>ptCut ? gt_ -> Etael[iEGMaxPt] : -10; 
  if(ieta > 0) eta = convertRegionEta(ieta);
  return eta;
}

void BasicRatePlots::setRateError(TH1F* histo) {

  int nBins = histo->GetNbinsX();

  for (int iBin=1; iBin<=nBins; ++iBin) {
    float value = histo->GetBinContent(iBin);
    float error = sqrt(value);

    histo->SetBinError(iBin,error);
  }

}

// --------------------------------------------------------------------
//                             run function 
// --------------------------------------------------------------------

void BasicRatePlots::run(bool runOnData, std::string resultTag, int minLs, int maxLs, float crossSec, float avPU, int nBunches, int isCrossSec, int nEvents) {

  system("mkdir -p results");
  std::string resultName = "results_" + resultTag + (isCrossSec ? "_XSEC" : "_RATE") + ".root";
  TFile *outFile = new TFile(("results/" + resultName).c_str(),"recreate");
  outFile->cd();

  algoFactory = new L1AlgoFactory(gt_,gmt_,gct_);

  float metcut = 30.;
  float mtcut  = 35.; 
  float ptcut  = 25.;

  hTH1F["nEGVsPt"]     = new TH1F("nEGVsPt","SingleEG; E_{T} cut; rate [kHz]",41,9.5,50.5);
  hTH1F["nEGVsPt"]->GetXaxis()->SetTitle("E_{T} (GeV)");
  hTH1F["nEGVsEta"]    = new TH1F("nEGVsEta","SingleEG; #eta cut; rate [kHz]",16,-3.,3.);
  hTH1F["nEGVsEta"]->GetXaxis()->SetTitle("#eta");

  hTH2F["nEGVsPtIso"]  = new TH2F("nEGVsPtIso","",41,9.5,50.5,20,-1.,1.);
  hTH2F["nEGVsPtIso"]->GetXaxis()->SetTitle("E_{T} (GeV)");
  hTH2F["nEGVsPtIso"]->GetYaxis()->SetTitle("Isolation");
  
  hTH2F["nEGVsEtaIso"] = new TH2F("nEGVsEtaIso","",16,-3.,3.,20,-1.,1.);
  hTH2F["nEGVsEtaIso"]->GetXaxis()->SetTitle("#eta");
  hTH2F["nEGVsEtaIso"]->GetYaxis()->SetTitle("Isolation");

  hTH2F["nEGVsPtEta"]  = new TH2F("nEGVsPtEta","",41,9.5,50.5,16,-3.,3.);
  hTH2F["nEGVsPtEta"]->GetXaxis()->SetTitle("E_{T} (GeV)");
  hTH2F["nEGVsPtEta"]->GetYaxis()->SetTitle("#eta");

  hTH1F["nEGVsPtEtaR"] = new TH1F("nEGVsPtEtaR","SingleEGer; E_{T} cut; rate [kHz]",41,9.5,50.5);
  hTH1F["nEGVsPtEtaR"]->GetXaxis()->SetTitle("E_{T} (GeV)");

  hTH2F["nEGVsPtIsoEtaR"]  = new TH2F("nEGVsPtIsoEtaR","",41,9.5,50.5,20,-1.,1.);
  hTH2F["nEGVsPtIsoEtaR"]->GetXaxis()->SetTitle("E_{T} (GeV)");
  hTH2F["nEGVsPtIsoEtaR"]->GetYaxis()->SetTitle("Isolation");

  hTH1F["nEGVsPtEB"]     = new TH1F("nEGVsPtEB","SingleEGEB; E_{T} cut; rate [kHz]",41,9.5,50.5);
  hTH1F["nEGVsPtEB"]->GetXaxis()->SetTitle("E_{T} (GeV)");

  hTH2F["nEGVsPtIsoEB"]  = new TH2F("nEGVsPtIsoEB","",41,9.5,50.5,20,-1.,1.);
  hTH2F["nEGVsPtIsoEB"]->GetXaxis()->SetTitle("E_{T} (GeV)");
  hTH2F["nEGVsPtIsoEB"]->GetYaxis()->SetTitle("Isolation");

  hTH1F["nEGVsPtEE"]     = new TH1F("nEGVsPtEE","SingleEGEE; E_{T} cut; rate [kHz]",41,9.5,50.5);
  hTH1F["nEGVsPtEE"]->GetXaxis()->SetTitle("E_{T} (GeV)");

  hTH2F["nEGVsPtIsoEE"]  = new TH2F("nEGVsPtIsoEE","",41,9.5,50.5,20,-1.,1.);
  hTH2F["nEGVsPtIsoEE"]->GetXaxis()->SetTitle("E_{T} (GeV)");
  hTH2F["nEGVsPtIsoEE"]->GetYaxis()->SetTitle("Isolation");

  // ---------------------------------------
  hTH1F["nEGVsPtMet"]     = new TH1F("nEGVsPtMet","SingleEGMet; E_{T} cut; rate [kHz]",41,9.5,50.5);
  hTH1F["nEGVsPtMet"]->GetXaxis()->SetTitle("E_{T} (GeV)");

  hTH1F["nEGVsEtaMet"]    = new TH1F("nEGVsEtaMet","",16,-3.,3.);
  hTH1F["nEGVsPtMet"]->GetXaxis()->SetTitle("#eta");

  hTH2F["nEGVsPtIsoMet"]  = new TH2F("nEGVsPtIsoMet","",41,9.5,50.5,20,-1.,1.);
  hTH2F["nEGVsPtIsoMet"]->GetXaxis()->SetTitle("E_{T} (GeV)");
  hTH2F["nEGVsPtIsoMet"]->GetYaxis()->SetTitle("Isolation");

  hTH2F["nEGVsEtaIsoMet"] = new TH2F("nEGVsEtaIsoMet","",16,-3.,3.,20,-1.,1.);
  hTH2F["nEGVsEtaIsoMet"]->GetXaxis()->SetTitle("#eta");
  hTH2F["nEGVsEtaIsoMet"]->GetYaxis()->SetTitle("Isolation");

  hTH2F["nEGVsPtEtaMet"]  = new TH2F("nEGVsPtEtaMet","",41,9.5,50.5,16,-3.,3.);
  hTH2F["nEGVsPtEtaMet"]->GetXaxis()->SetTitle("E_{T} (GeV)");
  hTH2F["nEGVsPtEtaMet"]->GetYaxis()->SetTitle("#eta");

  hTH1F["nEGVsPtEtaRMet"]     = new TH1F("nEGVsPtEtaRMet","SingleEGerMet; E_{T} cut; rate [kHz]",41,9.5,50.5);
  hTH1F["nEGVsPtEtaRMet"]->GetXaxis()->SetTitle("E_{T} (GeV)");

  hTH2F["nEGVsPtIsoEtaRMet"]  = new TH2F("nEGVsPtIsoEtaRMet","",41,9.5,50.5,20,-1.,1.);
  hTH2F["nEGVsPtIsoEtaRMet"]->GetXaxis()->SetTitle("E_{T} (GeV)");
  hTH2F["nEGVsPtIsoEtaRMet"]->GetYaxis()->SetTitle("Isolation");

  hTH1F["nEGVsPtEBMet"]     = new TH1F("nEGVsPtEBMet","SingleEGEBMet; E_{T} cut; rate [kHz]",41,9.5,50.5);
  hTH1F["nEGVsPtEBMet"]->GetXaxis()->SetTitle("E_{T} (GeV)");

  hTH2F["nEGVsPtIsoEBMet"]  = new TH2F("nEGVsPtIsoEBMet","",41,9.5,50.5,20,-1.,1.);
  hTH2F["nEGVsPtIsoEBMet"]->GetXaxis()->SetTitle("E_{T} (GeV)");
  hTH2F["nEGVsPtIsoEBMet"]->GetYaxis()->SetTitle("Isolation");

  hTH1F["nEGVsPtEEMet"]     = new TH1F("nEGVsPtEEMet","SingleEGEEMet; E_{T} cut; rate [kHz]",41,9.5,50.5);
  hTH1F["nEGVsPtEEMet"]->GetXaxis()->SetTitle("E_{T} (GeV)");

  hTH2F["nEGVsPtIsoEEMet"]  = new TH2F("nEGVsPtIsoEEMet","",41,9.5,50.5,20,-1.,1.);
  hTH2F["nEGVsPtIsoEEMet"]->GetXaxis()->SetTitle("E_{T} (GeV)");
  hTH2F["nEGVsPtIsoEEMet"]->GetYaxis()->SetTitle("Isolation");

  // ---------------------------------------
  hTH1F["nEGVsPtMT"]     = new TH1F("nEGVsPtMT","SingleEGMT; E_{T} cut; rate [kHz]",41,9.5,50.5);
  hTH1F["nEGVsPtMT"]->GetXaxis()->SetTitle("E_{T} (GeV)");

  hTH1F["nEGVsEtaMT"]    = new TH1F("nEGVsEtaMT","",16,-3.,3.);
  hTH1F["nEGVsEtaMT"]->GetXaxis()->SetTitle("#eta");

  hTH2F["nEGVsPtIsoMT"]  = new TH2F("nEGVsPtIsoMT","",41,9.5,50.5,20,-1.,1.);
  hTH2F["nEGVsPtIsoMT"]->GetXaxis()->SetTitle("E_{T} (GeV)");
  hTH2F["nEGVsPtIsoMT"]->GetYaxis()->SetTitle("Isolation");

  hTH2F["nEGVsEtaIsoMT"] = new TH2F("nEGVsEtaIsoMT","",16,-3.,3.,20,-1.,1.);
  hTH2F["nEGVsEtaIsoMT"]->GetXaxis()->SetTitle("#eta");
  hTH2F["nEGVsEtaIsoMT"]->GetYaxis()->SetTitle("Isolation");

  hTH2F["nEGVsPtEtaMT"]  = new TH2F("nEGVsPtEtaMT","",41,9.5,50.5,16,-3.,3.);
  hTH2F["nEGVsPtEtaMT"]->GetXaxis()->SetTitle("E_{T} (GeV)");
  hTH2F["nEGVsPtEtaMT"]->GetYaxis()->SetTitle("#eta");

  hTH1F["nEGVsPtEtaRMT"]     = new TH1F("nEGVsPtEtaRMT","SingleEGerMT; E_{T} cut; rate [kHz]",41,9.5,50.5);
  hTH1F["nEGVsPtEtaRMT"]->GetXaxis()->SetTitle("E_{T} (GeV)");

  hTH2F["nEGVsPtIsoEtaRMT"]  = new TH2F("nEGVsPtIsoEtaRMT","",41,9.5,50.5,20,-1.,1.);
  hTH2F["nEGVsPtIsoEtaRMT"]->GetXaxis()->SetTitle("E_{T} (GeV)");
  hTH2F["nEGVsPtIsoEtaRMT"]->GetYaxis()->SetTitle("Isolation");

  hTH1F["nEGVsPtEBMT"]     = new TH1F("nEGVsPtEBMT","SingleEGEBMT; E_{T} cut; rate [kHz]",41,9.5,50.5);
  hTH1F["nEGVsPtEBMT"]->GetXaxis()->SetTitle("E_{T} (GeV)");

  hTH2F["nEGVsPtIsoEBMT"]  = new TH2F("nEGVsPtIsoEBMT","",41,9.5,50.5,20,-1.,1.);
  hTH2F["nEGVsPtIsoEBMT"]->GetXaxis()->SetTitle("E_{T} (GeV)");
  hTH2F["nEGVsPtIsoEBMT"]->GetYaxis()->SetTitle("Isolation");

  hTH1F["nEGVsPtEEMT"]     = new TH1F("nEGVsPtEEMT","SingleEGEEMT; E_{T} cut; rate [kHz]",41,9.5,50.5);
  hTH1F["nEGVsPtEEMT"]->GetXaxis()->SetTitle("E_{T} (GeV)");

  hTH2F["nEGVsPtIsoEEMT"]  = new TH2F("nEGVsPtIsoEEMT","",41,9.5,50.5,20,-1.,1.);
  hTH2F["nEGVsPtIsoEEMT"]->GetXaxis()->SetTitle("E_{T} (GeV)");
  hTH2F["nEGVsPtIsoEEMT"]->GetYaxis()->SetTitle("Isolation");

  hTH1F["nPUvsPU"]  = new TH1F("nPUvsPU","Num. of PU iteractions; Num of iteractions; Reweighted couns [arb units]",101,9.5,100.5);

  // Fixed pt threshold as a function of met and mT cut

  hTH1F["nEGVsMet"]     = new TH1F("nEGVsMet","SingleEG; E_{T}^{miss} cut; rate [kHz]",41,9.5,50.5);
  hTH1F["nEGVsMet"]->GetXaxis()->SetTitle("E_{T}^{miss} (GeV)");

  hTH1F["nEGVsIso"]     = new TH1F("nEGVsIso","SingleEG; Iso cut; rate [kHz]",20,-1.,1.);
  hTH1F["nEGVsIso"]->GetXaxis()->SetTitle("Isolation");

  hTH1F["nEGVsMT"]     = new TH1F("nEGVsMT","SingleEG; m_{T} cut; rate [kHz]",41,9.5,50.5);
  hTH1F["nEGVsMT"]->GetXaxis()->SetTitle("m_{T} (GeV)");

  hTH2F["nEGVsMetIso"]  = new TH2F("nEGVsMetIso","",41,9.5,50.5,20,-1.,1.);
  hTH2F["nEGVsMetIso"]->GetXaxis()->SetTitle("MET (GeV)");
  hTH2F["nEGVsMetIso"]->GetYaxis()->SetTitle("Isolation");

  hTH2F["nEGVsMTIso"]  = new TH2F("nEGVsMTIso","",41,9.5,50.5,20,-1.,1.);
  hTH2F["nEGVsMTIso"]->GetXaxis()->SetTitle("m_{T} (GeV)");
  hTH2F["nEGVsMTIso"]->GetYaxis()->SetTitle("Isolation");

  hTH1F["nEGVsMetEtaR"]     = new TH1F("nEGVsMetEtaR","SingleEGer; E_{T}^{miss} cut; rate [kHz]",41,9.5,50.5);
  hTH1F["nEGVsMetEtaR"]->GetXaxis()->SetTitle("MET (GeV)");

  hTH1F["nEGVsIsoEtaR"]     = new TH1F("nEGVsIsoEtaR","SingleEGer; Iso cut; rate [kHz]",20,-1.,1.);
  hTH1F["nEGVsIsoEtaR"]->GetXaxis()->SetTitle("Isolation");

  hTH1F["nEGVsMTEtaR"]     = new TH1F("nEGVsMTEtaR","SingleEGer; m_{T} cut; rate [kHz]",41,9.5,50.5);
  hTH1F["nEGVsMTEtaR"]->GetXaxis()->SetTitle("m_{T} (GeV)");

  hTH2F["nEGVsMetIsoEtaR"]  = new TH2F("nEGVsMetIsoEtaR","",41,9.5,50.5,20,-1.,1.);
  hTH2F["nEGVsMetIsoEtaR"]->GetXaxis()->SetTitle("MET (GeV)");
  hTH2F["nEGVsMetIsoEtaR"]->GetYaxis()->SetTitle("Isolation");

  hTH2F["nEGVsMTIsoEtaR"]  = new TH2F("nEGVsMTIsoEtaR","",41,9.5,50.5,20,-1.,1.);
  hTH2F["nEGVsMTIsoEtaR"]->GetXaxis()->SetTitle("m_{T} (GeV)");
  hTH2F["nEGVsMTIsoEtaR"]->GetYaxis()->SetTitle("Isolation");

  if (isCrossSec) {
    std::map<std::string,TH1F*>::iterator hTH1FIt  = hTH1F.begin();
    std::map<std::string,TH1F*>::iterator hTH1FEnd = hTH1F.end();
    for(; hTH1FIt!=hTH1FEnd; ++hTH1FIt) hTH1FIt->second->GetYaxis()->SetTitle("cross section [#mubarn]");

    std::map<std::string,TH2F*>::iterator hTH2FIt  = hTH2F.begin();
    std::map<std::string,TH2F*>::iterator hTH2FEnd = hTH2F.end();
    for(; hTH2FIt!=hTH2FEnd; ++hTH2FIt){ hTH2FIt->second->GetZaxis()->SetTitle("cross section [#mubarn]");
                                              hTH2FIt->second->GetZaxis()->SetTitleOffset(0.85);
    }
  }
  else{

    std::map<std::string,TH1F*>::iterator hTH1FIt  = hTH1F.begin();
    std::map<std::string,TH1F*>::iterator hTH1FEnd = hTH1F.end();
    for(; hTH1FIt!=hTH1FEnd; ++hTH1FIt) hTH1FIt->second->GetYaxis()->SetTitle("Rate [kHz]");

    std::map<std::string,TH2F*>::iterator hTH2FIt  = hTH2F.begin();
    std::map<std::string,TH2F*>::iterator hTH2FEnd = hTH2F.end();
    for(; hTH2FIt!=hTH2FEnd; ++hTH2FIt){ hTH2FIt->second->GetZaxis()->SetTitle("Rate [kHz]");
                                         hTH2FIt->second->GetZaxis()->SetTitleOffset(0.85);
    }
  }

  float nZeroBias = 0;

  int nevents = nEvents == 0 ? GetEntries() : nEvents;
    
  std::cout << "Running on " << nevents << " events." << std::endl;

  for (Long64_t event=0; event<nevents; ++event) { 

    Long64_t eventEntry = LoadTree(event); 
    
    if (eventEntry < 0) break;
    GetEntry(event);
      
    if (event%200000 == 0) {
      std::cout << "Processed " << event << " events." << std::endl;
    }
      
    if (event_->lumi < minLs || event_->lumi > maxLs ) continue;  // minimum and maximum lumi section to be used ... useful for data

    double weight = event_->puWeight > -0.001 ? event_->puWeight : 1; // reweight for the pileUp
      
    FillBits();

    if(runOnData && !PhysicsBits[0]) continue; // used only on real data
      
    nZeroBias += weight; // cout the weight of each event
    
    float egPt      = -1.; algoFactory->SingleEGPt(egPt);
    float egPtEtaR  = -1.; algoFactory->SingleEGEta2p1Pt(egPtEtaR);
    float egPtEtaEB = -1.; algoFactory->SingleEGEtaEBPt(egPtEtaEB);
    float egPtEtaEE = -1.; algoFactory->SingleEGEtaEEPt(egPtEtaEE);
    float egEta     = SingleEGEta(16.,false);

    float egPtMet      = -1.; algoFactory->SingleEGPtMet(egPtMet,metcut);
    float egPtEtaRMet  = -1.; algoFactory->SingleEGEta2p1PtMet(egPtEtaRMet,metcut);
    float egPtEtaEBMet = -1.; algoFactory->SingleEGEtaEBPtMet(egPtEtaEBMet,metcut);
    float egPtEtaEEMet = -1.; algoFactory->SingleEGEtaEEPtMet(egPtEtaEEMet,metcut);

    float egPtMT      = -1.; algoFactory->SingleEGPtMT(egPtMT,mtcut);
    float egPtEtaRMT  = -1.; algoFactory->SingleEGEta2p1PtMT(egPtEtaRMT,mtcut);
    float egPtEtaEBMT = -1.; algoFactory->SingleEGEtaEBPtMT(egPtEtaEBMT,mtcut);
    float egPtEtaEEMT = -1.; algoFactory->SingleEGEtaEEPtMT(egPtEtaEEMT,mtcut);

    float egPt2D      = -1.; float egIso2D      = -100.; algoFactory->SingleEGPt2D(egPt2D,egIso2D);
    float egPtEtaR2D  = -1.; float egIsoEtaR2D  = -100.; algoFactory->SingleEGEta2p1Pt2D(egPtEtaR2D,egIsoEtaR2D);
    float egPtEtaEB2D = -1.; float egIsoEtaEB2D = -100.; algoFactory->SingleEGEtaEBPt2D(egPtEtaEB2D,egIsoEtaEB2D);
    float egPtEtaEE2D = -1.; float egIsoEtaEE2D = -100.; algoFactory->SingleEGEtaEEPt2D(egPtEtaEE2D,egIsoEtaEE2D);

    float egPtMet2D      = -1.; float egIsoMet2D      = -100.; algoFactory->SingleEGPtMet2D(egPtMet2D,egIsoMet2D,metcut);
    float egPtEtaRMet2D  = -1.; float egIsoEtaRMet2D  = -100.; algoFactory->SingleEGEta2p1PtMet2D(egPtEtaRMet2D,egIsoEtaRMet2D,metcut);
    float egPtEtaEBMet2D = -1.; float egIsoEtaEBMet2D = -100.; algoFactory->SingleEGEtaEBPtMet2D(egPtEtaEBMet2D,egIsoEtaEBMet2D,metcut);
    float egPtEtaEEMet2D = -1.; float egIsoEtaEEMet2D = -100.; algoFactory->SingleEGEtaEEPtMet2D(egPtEtaEEMet2D,egIsoEtaEEMet2D,metcut);

    float egPtMT2D      = -1.; float egIsoMT2D      = -100.; algoFactory->SingleEGPtMT2D(egPtMT2D,egIsoMT2D,mtcut);
    float egPtEtaRMT2D  = -1.; float egIsoEtaRMT2D  = -100.; algoFactory->SingleEGEta2p1PtMT2D(egPtEtaRMT2D,egIsoEtaRMT2D,mtcut);
    float egPtEtaEBMT2D = -1.; float egIsoEtaEBMT2D = -100.; algoFactory->SingleEGEtaEBPtMT2D(egPtEtaEBMT2D,egIsoEtaEBMT2D,mtcut);
    float egPtEtaEEMT2D = -1.; float egIsoEtaEEMT2D = -100.; algoFactory->SingleEGEtaEEPtMT2D(egPtEtaEEMT2D,egIsoEtaEEMT2D,mtcut);
    
    hTH1F["nPUvsPU"]->Fill(simulation_->actualInt,weight);
    hTH1F["nEGVsEta"]->Fill(egEta,weight);
    if(gt_->RankETM > metcut) hTH1F["nEGVsEtaMet"]->Fill(egEta,weight);    
    if(sqrt(2*gct_->EmRnk[0]*gt_->RankETM*(1-TMath::Cos(gt_->PhiETM))) < mtcut)  hTH1F["nEGVsEtaMT"]->Fill(egEta,weight);

    for(int ptCut=10; ptCut<51; ++ptCut) {

      if(egPt >= ptCut)      hTH1F["nEGVsPt"]->Fill(ptCut,weight);
      if(egPtEtaR  >= ptCut) hTH1F["nEGVsPtEtaR"]->Fill(ptCut,weight);
      if(egPtEtaEB >= ptCut) hTH1F["nEGVsPtEB"]->Fill(ptCut,weight);
      if(egPtEtaEE >= ptCut) hTH1F["nEGVsPtEE"]->Fill(ptCut,weight);

      if(egPtMet >= ptCut)      hTH1F["nEGVsPtMet"]->Fill(ptCut,weight);
      if(egPtEtaRMet  >= ptCut) hTH1F["nEGVsPtEtaRMet"]->Fill(ptCut,weight);
      if(egPtEtaEBMet >= ptCut) hTH1F["nEGVsPtEBMet"]->Fill(ptCut,weight);
      if(egPtEtaEEMet >= ptCut) hTH1F["nEGVsPtEEMet"]->Fill(ptCut,weight);

      if(egPtMT >= ptCut)      hTH1F["nEGVsPtMT"]->Fill(ptCut,weight);
      if(egPtEtaRMT  >= ptCut) hTH1F["nEGVsPtEtaRMT"]->Fill(ptCut,weight);
      if(egPtEtaEBMT >= ptCut) hTH1F["nEGVsPtEBMT"]->Fill(ptCut,weight);
      if(egPtEtaEEMT >= ptCut) hTH1F["nEGVsPtEEMT"]->Fill(ptCut,weight);

      for(float isoCut=-1.; isoCut<=1 ; isoCut = isoCut+0.1){

	if(egPt2D >= ptCut   && egIso2D <= isoCut) hTH2F["nEGVsPtIso"]->Fill(ptCut,isoCut,weight);
	if(egIso2D >= isoCut && egPt2D >= 0.)      hTH2F["nEGVsEtaIso"]->Fill(egEta,isoCut,weight);
	if(egPt2D >= ptCut)                        hTH2F["nEGVsPtEta"]->Fill(ptCut,egEta,weight);

        if(egPtEtaR2D  >= ptCut && egIsoEtaR2D  <= isoCut) hTH2F["nEGVsPtIsoEtaR"]->Fill(ptCut,isoCut,weight);
        if(egPtEtaEB2D >= ptCut && egIsoEtaEB2D <= isoCut) hTH2F["nEGVsPtIsoEB"]->Fill(ptCut,isoCut,weight);
        if(egPtEtaEE2D >= ptCut && egIsoEtaEE2D <= isoCut) hTH2F["nEGVsPtIsoEE"]->Fill(ptCut,isoCut,weight);

	if(egPtMet2D >= ptCut   && egIsoMet2D <= isoCut) hTH2F["nEGVsPtIsoMet"]->Fill(ptCut,isoCut,weight);
        if(egPtEtaRMet2D  >= ptCut && egIsoEtaRMet2D  <= isoCut) hTH2F["nEGVsPtIsoEtaRMet"]->Fill(ptCut,isoCut,weight);
        if(egPtEtaEBMet2D >= ptCut && egIsoEtaEBMet2D <= isoCut) hTH2F["nEGVsPtIsoEBMet"]->Fill(ptCut,isoCut,weight);
        if(egPtEtaEEMet2D >= ptCut && egIsoEtaEEMet2D <= isoCut) hTH2F["nEGVsPtIsoEEMet"]->Fill(ptCut,isoCut,weight);

	if(egIsoMet2D >= isoCut && egPtMet2D >= 0.) hTH2F["nEGVsEtaIsoMet"]->Fill(egEta,isoCut,weight);
	if(egPtMet2D >= ptCut)                      hTH2F["nEGVsPtEtaMet"]->Fill(egPt2D,egEta,weight);

	if(egPtMT2D >= ptCut   && egIsoMT2D <= isoCut) hTH2F["nEGVsPtIsoMT"]->Fill(ptCut,isoCut,weight);
        if(egPtEtaRMT2D  >= ptCut && egIsoEtaRMT2D  <= isoCut) hTH2F["nEGVsPtIsoEtaRMT"]->Fill(ptCut,isoCut,weight);
        if(egPtEtaEBMT2D >= ptCut && egIsoEtaEBMT2D <= isoCut) hTH2F["nEGVsPtIsoEBMT"]->Fill(ptCut,isoCut,weight);
        if(egPtEtaEEMT2D >= ptCut && egIsoEtaEEMT2D <= isoCut) hTH2F["nEGVsPtIsoEEMT"]->Fill(ptCut,isoCut,weight);

	if(egIsoMT2D >= isoCut && egPtMT2D >= 0.) hTH2F["nEGVsEtaIsoMT"]->Fill(egEta,isoCut,weight);
	if(egPtMT2D >= ptCut)                     hTH2F["nEGVsPtEtaMT"]->Fill(egPt2D,egEta,weight);

      }
    }

    // asaf met fixed pt 
    if (gct_->EmRnk[0] >= ptcut){

     for(int metCut=10; metCut<51; ++metCut) {
      if (gt_->RankETM < metCut) continue ;
      hTH1F["nEGVsMet"]->Fill(metCut,weight); 
      if(gct_->EmEta[0] >= 4.5 && gct_->EmEta[0] <= 16.5 ) hTH1F["nEGVsMetEtaR"]->Fill(metCut,weight);      
     
      for(float isoCut=-1.; isoCut<=1 ; isoCut = isoCut+0.1){
	if(egIso2D > isoCut) continue ;
        hTH2F["nEGVsMetIso"]->Fill(metCut,isoCut,weight);
        if(gct_->EmEta[0] >= 4.5 && gct_->EmEta[0] <= 16.5 ) hTH2F["nEGVsMetIsoEtaR"]->Fill(metCut,isoCut,weight);
      }       
     }
    
     // asaf isolation 
     for(float isoCut=-1.; isoCut<=1 ; isoCut = isoCut+0.1){
      if(egIso2D > isoCut) continue ;
      hTH1F["nEGVsIso"]->Fill(isoCut,weight);
      if(gct_->EmEta[0] >= 4.5 && gct_->EmEta[0] <= 16.5 ) hTH1F["nEGVsIsoEtaR"]->Fill(isoCut,weight);
     }   

     for(int mtCut=10; mtCut<51; ++mtCut) {
      float mt_tmp = sqrt(2*gct_->EmRnk[0]*gt_->RankETM*(1-TMath::Cos(gt_->PhiETM)));
      if (mt_tmp < mtCut) continue ;      
      hTH1F["nEGVsMT"]->Fill(mtCut,weight);
      if(gct_->EmEta[0] >= 4.5 && gct_->EmEta[0] <= 16.5 ) hTH1F["nEGVsMTEtaR"]->Fill(mtCut,weight);

      for(float isoCut=-1.; isoCut<=1 ; isoCut = isoCut+0.1){
	if(egIso2D > isoCut) continue ;
        hTH2F["nEGVsMTIso"]->Fill(mtCut,isoCut,weight);
        if(gct_->EmEta[0] >= 4.5 && gct_->EmEta[0] <= 16.5 ) hTH2F["nEGVsMTIsoEtaR"]->Fill(mtCut,isoCut,weight);
      }
     }         
    } // end pt cut fixed
  }

  cout << "# of zero bias events (weighted) used for rate computation : " << nZeroBias << std::endl;

  float scaleFactor = ScaleFactor(nZeroBias,nBunches);

  if (isCrossSec) scaleFactor /= (computeAvgLumi(crossSec,avPU,nBunches)*10000) ; // CB lumi is in 1E34 units
  
  map<string,TH1F*>::iterator hTH1FIt  = hTH1F.begin();
  map<string,TH1F*>::iterator hTH1FEnd = hTH1F.end();

  for(; hTH1FIt!=hTH1FEnd; ++hTH1FIt) {
    TH1F* histo = hTH1FIt->second;
    histo->Sumw2();
    if (hTH1FIt->first == "nPUvsPU") histo->Scale(1./nZeroBias);      
    setRateError(histo);
    histo->Scale(scaleFactor);
  }

  map<string,TH2F*>::iterator hTH2FIt  = hTH2F.begin();
  map<string,TH2F*>::iterator hTH2FEnd = hTH2F.end();

  for(; hTH2FIt!=hTH2FEnd; ++hTH2FIt) {
    TH2F* histo = hTH2FIt->second;
    histo->Sumw2();
    histo->Scale(scaleFactor);
  }
  
  // --------------------- Plot canvas
  system("rm results/*png");
  system("rm results/*pdf");

  TLegend* leg = new TLegend(0.5,0.65,0.8,0.9);
  leg->SetFillColor(0);
  leg->SetFillStyle(0);
  leg->SetBorderSize(0);
  leg->SetLineColor(0);
  leg->SetLineWidth(0);
  leg->SetLineStyle(0);

  TF1* f1 = new TF1("f1","15",9.5,51.5);
  f1->SetLineColor(kGreen+1);
  f1->SetLineStyle(7);
  f1->SetLineWidth(2);

  TF1* f5 = new TF1("f1","15",-1.,1.);
  f5->SetLineColor(kGreen+1);
  f5->SetLineStyle(7);
  f5->SetLineWidth(2);

  TCanvas* canvasTemp = new TCanvas("canvasTemp","",600,650);
  canvasTemp->cd();

  hTH1F["nEGVsPt"]->SetMarkerColor(kBlack);
  hTH1F["nEGVsPt"]->SetLineColor(kBlack);
  hTH1F["nEGVsPt"]->SetLineWidth(2);
  hTH1F["nEGVsPt"]->SetMarkerStyle(20);
  hTH1F["nEGVsPt"]->SetMarkerSize(1);
  hTH1F["nEGVsPt"]->Draw("E");

  hTH1F["nEGVsPtMet"]->SetMarkerColor(kRed);
  hTH1F["nEGVsPtMet"]->SetLineColor(kRed);
  hTH1F["nEGVsPtMet"]->SetLineWidth(2);
  hTH1F["nEGVsPtMet"]->SetMarkerStyle(22);
  hTH1F["nEGVsPtMet"]->SetMarkerSize(1);
  hTH1F["nEGVsPtMet"]->Draw("Esame");

  hTH1F["nEGVsPtMT"]->SetMarkerColor(kBlue);
  hTH1F["nEGVsPtMT"]->SetLineColor(kBlue);
  hTH1F["nEGVsPtMT"]->SetLineWidth(2);
  hTH1F["nEGVsPtMT"]->SetMarkerStyle(34);
  hTH1F["nEGVsPtMT"]->SetMarkerSize(1);
  hTH1F["nEGVsPtMT"]->Draw("Esame");  

  f1->Draw("same");

  leg->AddEntry(hTH1F["nEGVsPt"],"SingleEG","p");
  leg->AddEntry(hTH1F["nEGVsPtMet"],"SingleEG E_{T}^{miss} > 30 GeV","p");
  leg->AddEntry(hTH1F["nEGVsPtMT"],"SingleEG m_{T} > 35 GeV","p");
  leg->Draw("same");

  canvasTemp->SetLogy();
  gPad->Update();
  canvasTemp->SaveAs("results/SingleEG_Comparison_vs_ET.pdf","pdf");
  canvasTemp->SaveAs("results/SingleEG_Comparison_vs_ET.png","png");
  
  // ------------------------------------------
  leg->Clear();

  hTH1F["nEGVsPtEtaR"]->SetMarkerColor(kBlack);
  hTH1F["nEGVsPtEtaR"]->SetLineColor(kBlack);
  hTH1F["nEGVsPtEtaR"]->SetLineWidth(2);
  hTH1F["nEGVsPtEtaR"]->SetMarkerStyle(20);
  hTH1F["nEGVsPtEtaR"]->SetMarkerSize(1);
  hTH1F["nEGVsPtEtaR"]->Draw("E");

  hTH1F["nEGVsPtEB"]->SetMarkerColor(kBlue);
  hTH1F["nEGVsPtEB"]->SetLineColor(kBlue);
  hTH1F["nEGVsPtEB"]->SetLineWidth(2);
  hTH1F["nEGVsPtEB"]->SetMarkerStyle(21);
  hTH1F["nEGVsPtEB"]->SetMarkerSize(1);
  hTH1F["nEGVsPtEB"]->Draw("Esame");

  hTH1F["nEGVsPtEE"]->SetMarkerColor(kRed);
  hTH1F["nEGVsPtEE"]->SetLineColor(kRed);
  hTH1F["nEGVsPtEE"]->SetLineWidth(2);
  hTH1F["nEGVsPtEE"]->SetMarkerStyle(22);
  hTH1F["nEGVsPtEE"]->SetMarkerSize(1);
  hTH1F["nEGVsPtEE"]->Draw("Esame");

  f1->Draw("same");

  leg->AddEntry(hTH1F["nEGVsPtEtaR"],"SingleEG  ER","p");
  leg->AddEntry(hTH1F["nEGVsPtEB"],"SingleEG EB","p");
  leg->AddEntry(hTH1F["nEGVsPtEE"],"SingleEG EE","p");
  leg->Draw("same");

  canvasTemp->SetLogy();
  gPad->Update();
  canvasTemp->SaveAs("results/SingleEG_EtaDependence_vs_ET.pdf","pdf");
  canvasTemp->SaveAs("results/SingleEG_EtaDependence_vs_ET.png","png");

  // ------------------------------------------
  leg->Clear();

  hTH1F["nEGVsPtEtaRMet"]->SetMarkerColor(kBlack);
  hTH1F["nEGVsPtEtaRMet"]->SetLineColor(kBlack);
  hTH1F["nEGVsPtEtaRMet"]->SetLineWidth(2);
  hTH1F["nEGVsPtEtaRMet"]->SetMarkerStyle(20);
  hTH1F["nEGVsPtEtaRMet"]->SetMarkerSize(1);
  hTH1F["nEGVsPtEtaRMet"]->Draw("E");

  hTH1F["nEGVsPtEBMet"]->SetMarkerColor(kBlue);
  hTH1F["nEGVsPtEBMet"]->SetLineColor(kBlue);
  hTH1F["nEGVsPtEBMet"]->SetLineWidth(2);
  hTH1F["nEGVsPtEBMet"]->SetMarkerStyle(21);
  hTH1F["nEGVsPtEBMet"]->SetMarkerSize(1);
  hTH1F["nEGVsPtEBMet"]->Draw("Esame");

  hTH1F["nEGVsPtEEMet"]->SetMarkerColor(kRed);
  hTH1F["nEGVsPtEEMet"]->SetLineColor(kRed);
  hTH1F["nEGVsPtEEMet"]->SetLineWidth(2);
  hTH1F["nEGVsPtEEMet"]->SetMarkerStyle(22);
  hTH1F["nEGVsPtEEMet"]->SetMarkerSize(1);
  hTH1F["nEGVsPtEEMet"]->Draw("Esame");

  f1->Draw("same");

  leg->AddEntry(hTH1F["nEGVsPtEtaRMet"],"SingleEG ER E_{T}^{miss} > 30 GeV","p");
  leg->AddEntry(hTH1F["nEGVsPtEBMet"],"SingleEG EB E_{T}^{miss} > 30 GeV","p");
  leg->AddEntry(hTH1F["nEGVsPtEEMet"],"SingleEG EE E_{T}^{miss} > 30 GeV","p");
  leg->Draw("same");
  canvasTemp->SetLogy();
  gPad->Update();
  canvasTemp->SaveAs("results/SingleEG_EtaDependence_Met_vs_ET.pdf","pdf");
  canvasTemp->SaveAs("results/SingleEG_EtaDependence_Met_vs_ET.png","png");

  // ------------------------------------------
  leg->Clear();

  hTH1F["nEGVsPtEtaRMT"]->SetMarkerColor(kBlack);
  hTH1F["nEGVsPtEtaRMT"]->SetLineColor(kBlack);
  hTH1F["nEGVsPtEtaRMT"]->SetLineWidth(2);
  hTH1F["nEGVsPtEtaRMT"]->SetMarkerStyle(20);
  hTH1F["nEGVsPtEtaRMT"]->SetMarkerSize(1);
  hTH1F["nEGVsPtEtaRMT"]->Draw("E");

  hTH1F["nEGVsPtEBMT"]->SetMarkerColor(kBlue);
  hTH1F["nEGVsPtEBMT"]->SetLineColor(kBlue);
  hTH1F["nEGVsPtEBMT"]->SetLineWidth(2);
  hTH1F["nEGVsPtEBMT"]->SetMarkerStyle(21);
  hTH1F["nEGVsPtEBMT"]->SetMarkerSize(1);
  hTH1F["nEGVsPtEBMT"]->Draw("Esame");

  hTH1F["nEGVsPtEEMT"]->SetMarkerColor(kRed);
  hTH1F["nEGVsPtEEMT"]->SetLineColor(kRed);
  hTH1F["nEGVsPtEEMT"]->SetLineWidth(2);
  hTH1F["nEGVsPtEEMT"]->SetMarkerStyle(22);
  hTH1F["nEGVsPtEEMT"]->SetMarkerSize(1);
  hTH1F["nEGVsPtEEMT"]->Draw("Esame");

  leg->AddEntry(hTH1F["nEGVsPtEtaRMT"],"SingleEG ER mT > 35 GeV","p");
  leg->AddEntry(hTH1F["nEGVsPtEBMT"],"SingleEG EB mT > 35 GeV","p");
  leg->AddEntry(hTH1F["nEGVsPtEEMT"],"SingleEG EE mT > 35 GeV","p");
  leg->Draw("same");

  f1->Draw("same");

  canvasTemp->SetLogy();
  gPad->Update();
  
  canvasTemp->SaveAs("results/SingleEG_EtaDependence_MT_vs_ET.pdf","pdf");
  canvasTemp->SaveAs("results/SingleEG_EtaDependence_MT_vs_ET.png","png");
  
  // --------- 2D plot
  leg->Clear();

  TCanvas* canvasTemp2 = new TCanvas("canvas2","",600,650);
  Double_t counturLevel[1];
  counturLevel[0] = 15.; 
  
  canvasTemp2->cd();
  canvasTemp2->SetLogz();
  hTH2F["nEGVsPtIso"]->GetZaxis()->SetRangeUser(0.05,1000);   
  hTH2F["nEGVsPtIso"]->SetContour(hTH2F["nEGVsPtIso"]->GetNbinsX()*hTH2F["nEGVsPtIso"]->GetNbinsY());
  hTH2F["nEGVsPtIso"]->SetContour(1,counturLevel);
  hTH2F["nEGVsPtIso"]->Draw("cont,list");
  canvasTemp2->Update();
  TObjArray* arrayList = (TObjArray*) gROOT->GetListOfSpecials()->FindObject("contours");
  TList *counturList = new TList(); 
  for(int i = 0; i < arrayList->GetSize(); i++){
    TList* List_tmp = (TList*) arrayList->At(i);
    for (int j = 0, n = List_tmp->GetSize(); j < n; ++j) {
      TGraph *gr1 = (TGraph*) List_tmp->At(j);
      gr1->SetLineColor(kBlack);
      gr1->SetLineWidth(2);
      counturList->Add(gr1->Clone());
    }
  }

  hTH2F["nEGVsPtIso"]->SetContour(hTH2F["nEGVsPtIso"]->GetNbinsX()*hTH2F["nEGVsPtIso"]->GetNbinsY());
  hTH2F["nEGVsPtIso"]->Draw("colz");   
  counturList->Draw("lsame");

  canvasTemp2->SaveAs("results/nEGVsPtIso_noET_Cut.pdf","pdf");
  canvasTemp2->SaveAs("results/nEGVsPtIso_noET_Cut.png","png");

  hTH2F["nEGVsEtaIso"]->GetZaxis()->SetRangeUser(10,1000);   
  hTH2F["nEGVsEtaIso"]->SetContour(hTH2F["nEGVsEtaIso"]->GetNbinsX()*hTH2F["nEGVsEtaIso"]->GetNbinsY());
  hTH2F["nEGVsEtaIso"]->SetContour(1,counturLevel);
  hTH2F["nEGVsEtaIso"]->Draw("cont,list");
  canvasTemp2->Update();
  arrayList = (TObjArray*) gROOT->GetListOfSpecials()->FindObject("contours");
  counturList->Clear(); 
  for(int i = 0; i < arrayList->GetSize(); i++){
    TList* List_tmp = (TList*) arrayList->At(i);
    for (int j = 0, n = List_tmp->GetSize(); j < n; ++j) {
      TGraph *gr1 = (TGraph*) List_tmp->At(j);      
      gr1->SetLineColor(kBlack);
      gr1->SetLineWidth(2);
      counturList->Add(gr1->Clone());
    }
  }

  hTH2F["nEGVsEtaIso"]->SetContour(hTH2F["nEGVsEtaIso"]->GetNbinsX()*hTH2F["nEGVsEtaIso"]->GetNbinsY());
  hTH2F["nEGVsEtaIso"]->GetXaxis()->SetRangeUser(-2.5,2.5);
  hTH2F["nEGVsEtaIso"]->Draw("colz");   
  counturList->Draw("lsame");

  canvasTemp2->SaveAs("results/nEGVsEtaIso_noET_Cut.pdf","pdf");
  canvasTemp2->SaveAs("results/nEGVsEtaIso_noET_Cut.png","png");
 
  //hTH2F["nEGVsPtEta"]->GetZaxis()->SetRangeUser(0.01,1000);   
  hTH2F["nEGVsPtEta"]->SetContour(hTH2F["nEGVsPtEta"]->GetNbinsX()*hTH2F["nEGVsPtEta"]->GetNbinsY());
  hTH2F["nEGVsPtEta"]->SetContour(1,counturLevel);
  hTH2F["nEGVsPtEta"]->Draw("cont,list");
  canvasTemp2->Update();
  arrayList = (TObjArray*) gROOT->GetListOfSpecials()->FindObject("contours");
  counturList->Clear(); 
  for(int i = 0; i < arrayList->GetSize(); i++){
    TList* List_tmp = (TList*) arrayList->At(i);
    for (int j = 0, n = List_tmp->GetSize(); j < n; ++j) {
      TGraph *gr1 = (TGraph*) List_tmp->At(j);      
      gr1->SetLineColor(kBlack);
      gr1->SetLineWidth(2);
      counturList->Add(gr1->Clone());
    }
  }

  hTH2F["nEGVsPtEta"]->SetContour(hTH2F["nEGVsPtEta"]->GetNbinsX()*hTH2F["nEGVsPtEta"]->GetNbinsY());
  hTH2F["nEGVsPtEta"]->GetYaxis()->SetRangeUser(-2.5,2.5);
  hTH2F["nEGVsPtEta"]->Draw("colz");   
  counturList->Draw("lsame");

  canvasTemp2->SaveAs("results/nEGVsPtEta_noET_Cut.pdf","pdf");
  canvasTemp2->SaveAs("results/nEGVsPtEta_noET_Cut.png","png");

  hTH2F["nEGVsPtIsoEtaR"]->GetZaxis()->SetRangeUser(0.01,1000);   
  hTH2F["nEGVsPtIsoEtaR"]->SetContour(hTH2F["nEGVsPtIsoEtaR"]->GetNbinsX()*hTH2F["nEGVsPtIsoEtaR"]->GetNbinsY());
  hTH2F["nEGVsPtIsoEtaR"]->SetContour(1,counturLevel);
  hTH2F["nEGVsPtIsoEtaR"]->Draw("cont,list");
  canvasTemp2->Update();
  arrayList = (TObjArray*) gROOT->GetListOfSpecials()->FindObject("contours");
  counturList->Clear(); 
  for(int i = 0; i < arrayList->GetSize(); i++){
    TList* List_tmp = (TList*) arrayList->At(i);
    for (int j = 0, n = List_tmp->GetSize(); j < n; ++j) {
      TGraph *gr1 = (TGraph*) List_tmp->At(j);      
      gr1->SetLineColor(kBlack);
      gr1->SetLineWidth(2);
      counturList->Add(gr1->Clone());
    }
  }

  hTH2F["nEGVsPtIsoEtaR"]->SetContour(hTH2F["nEGVsPtIsoEtaR"]->GetNbinsX()*hTH2F["nEGVsPtIsoEtaR"]->GetNbinsY());
  hTH2F["nEGVsPtIsoEtaR"]->Draw("colz");   
  counturList->Draw("lsame");

  canvasTemp2->SaveAs("results/nEGVsPtIsoEtaR_noET_Cut.pdf","pdf");
  canvasTemp2->SaveAs("results/nEGVsPtIsoEtaR_noET_Cut.png","png");

  hTH2F["nEGVsPtIsoEB"]->GetZaxis()->SetRangeUser(0.1,1000);   
  hTH2F["nEGVsPtIsoEB"]->SetContour(hTH2F["nEGVsPtIsoEB"]->GetNbinsX()*hTH2F["nEGVsPtIsoEB"]->GetNbinsY());
  hTH2F["nEGVsPtIsoEB"]->SetContour(1,counturLevel);
  hTH2F["nEGVsPtIsoEB"]->Draw("cont,list");
  canvasTemp2->Update();
  arrayList = (TObjArray*) gROOT->GetListOfSpecials()->FindObject("contours");
  counturList->Clear(); 
  for(int i = 0; i < arrayList->GetSize(); i++){
    TList* List_tmp = (TList*) arrayList->At(i);
    for (int j = 0, n = List_tmp->GetSize(); j < n; ++j) {
      TGraph *gr1 = (TGraph*) List_tmp->At(j);      
      gr1->SetLineColor(kBlack);
      gr1->SetLineWidth(2);
      counturList->Add(gr1->Clone());
    }
  }

  hTH2F["nEGVsPtIsoEE"]->SetContour(hTH2F["nEGVsPtIsoEE"]->GetNbinsX()*hTH2F["nEGVsPtIsoEE"]->GetNbinsY());
  hTH2F["nEGVsPtIsoEE"]->Draw("colz");   
  counturList->Draw("lsame");

  canvasTemp2->SaveAs("results/nEGVsPtIsoEE_noET_Cut.pdf","pdf");
  canvasTemp2->SaveAs("results/nEGVsPtIsoEE_noET_Cut.png","png");

  hTH2F["nEGVsPtIsoMet"]->GetZaxis()->SetRangeUser(0.05,1000);   
  hTH2F["nEGVsPtIsoMet"]->SetContour(hTH2F["nEGVsPtIsoMet"]->GetNbinsX()*hTH2F["nEGVsPtIsoMet"]->GetNbinsY());
  hTH2F["nEGVsPtIsoMet"]->SetContour(1,counturLevel);
  hTH2F["nEGVsPtIsoMet"]->Draw("cont,list");
  canvasTemp2->Update();
  arrayList = (TObjArray*) gROOT->GetListOfSpecials()->FindObject("contours");
  counturList->Clear(); 
  for(int i = 0; i < arrayList->GetSize(); i++){
    TList* List_tmp = (TList*) arrayList->At(i);
    for (int j = 0, n = List_tmp->GetSize(); j < n; ++j) {
      TGraph *gr1 = (TGraph*) List_tmp->At(j);      
      gr1->SetLineColor(kBlack);
      gr1->SetLineWidth(2);
      counturList->Add(gr1->Clone());
    }
  }

  hTH2F["nEGVsPtIsoMet"]->SetContour(hTH2F["nEGVsPtIsoMet"]->GetNbinsX()*hTH2F["nEGVsPtIsoMet"]->GetNbinsY());
  hTH2F["nEGVsPtIsoMet"]->Draw("colz");   
  counturList->Draw("lsame");

  canvasTemp2->SaveAs("results/nEGVsPtIso_Met_30.pdf","pdf");
  canvasTemp2->SaveAs("results/nEGVsPtIso_Met_30.png","png");

  hTH2F["nEGVsPtEtaMet"]->GetZaxis()->SetRangeUser(0.1,1000);   
  hTH2F["nEGVsPtEtaMet"]->SetContour(hTH2F["nEGVsPtEtaMet"]->GetNbinsX()*hTH2F["nEGVsPtEtaMet"]->GetNbinsY());
  hTH2F["nEGVsPtEtaMet"]->SetContour(1,counturLevel);
  hTH2F["nEGVsPtEtaMet"]->Draw("cont,list");
  canvasTemp2->Update();
  arrayList = (TObjArray*) gROOT->GetListOfSpecials()->FindObject("contours");
  counturList->Clear(); 
  for(int i = 0; i < arrayList->GetSize(); i++){
    TList* List_tmp = (TList*) arrayList->At(i);
    for (int j = 0, n = List_tmp->GetSize(); j < n; ++j) {
      TGraph *gr1 = (TGraph*) List_tmp->At(j);      
      gr1->SetLineColor(kBlack);
      gr1->SetLineWidth(2);
      counturList->Add(gr1->Clone());
    }
  }

  hTH2F["nEGVsPtEtaMet"]->SetContour(hTH2F["nEGVsPtEtaMet"]->GetNbinsX()*hTH2F["nEGVsPtEtaMet"]->GetNbinsY());
  hTH2F["nEGVsPtEtaMet"]->GetYaxis()->SetRangeUser(-2.5,2.5);
  hTH2F["nEGVsPtEtaMet"]->Draw("colz");   
  counturList->Draw("lsame");

  canvasTemp2->SaveAs("results/nEGVsPtEta_Met_30.pdf","pdf");
  canvasTemp2->SaveAs("results/nEGVsPtEta_Met_30.png","png");

  hTH2F["nEGVsPtIsoEtaRMet"]->GetZaxis()->SetRangeUser(0.05,1000);   
  hTH2F["nEGVsPtIsoEtaRMet"]->SetContour(hTH2F["nEGVsPtIsoEtaRMet"]->GetNbinsX()*hTH2F["nEGVsPtIsoEtaRMet"]->GetNbinsY());
  hTH2F["nEGVsPtIsoEtaRMet"]->SetContour(1,counturLevel);
  hTH2F["nEGVsPtIsoEtaRMet"]->Draw("cont,list");
  canvasTemp2->Update();
  arrayList = (TObjArray*) gROOT->GetListOfSpecials()->FindObject("contours");
  counturList->Clear(); 
  for(int i = 0; i < arrayList->GetSize(); i++){
    TList* List_tmp = (TList*) arrayList->At(i);
    for (int j = 0, n = List_tmp->GetSize(); j < n; ++j) {
      TGraph *gr1 = (TGraph*) List_tmp->At(j);      
      gr1->SetLineColor(kBlack);
      gr1->SetLineWidth(2);
      counturList->Add(gr1->Clone());
    }
  }

  hTH2F["nEGVsPtIsoEtaRMet"]->SetContour(hTH2F["nEGVsPtIsoEtaRMet"]->GetNbinsX()*hTH2F["nEGVsPtIsoEtaRMet"]->GetNbinsY());
  hTH2F["nEGVsPtIsoEtaRMet"]->Draw("colz");   
  counturList->Draw("lsame");

  canvasTemp2->SaveAs("results/nEGVsPtIsoEtaR_Met_30.pdf","pdf");
  canvasTemp2->SaveAs("results/nEGVsPtIsoEtaR_Met_30.png","png");

  hTH2F["nEGVsPtIsoEBMet"]->GetZaxis()->SetRangeUser(0.001,1000);   
  hTH2F["nEGVsPtIsoEBMet"]->SetContour(hTH2F["nEGVsPtIsoEBMet"]->GetNbinsX()*hTH2F["nEGVsPtIsoEBMet"]->GetNbinsY());
  hTH2F["nEGVsPtIsoEBMet"]->SetContour(1,counturLevel);
  hTH2F["nEGVsPtIsoEBMet"]->Draw("cont,list");
  canvasTemp2->Update();
  arrayList = (TObjArray*) gROOT->GetListOfSpecials()->FindObject("contours");
  counturList->Clear(); 
  for(int i = 0; i < arrayList->GetSize(); i++){
    TList* List_tmp = (TList*) arrayList->At(i);
    for (int j = 0, n = List_tmp->GetSize(); j < n; ++j) {
      TGraph *gr1 = (TGraph*) List_tmp->At(j);      
      gr1->SetLineColor(kBlack);
      gr1->SetLineWidth(2);
      counturList->Add(gr1->Clone());
    }
  }

  hTH2F["nEGVsPtIsoEBMet"]->SetContour(hTH2F["nEGVsPtIsoEBMet"]->GetNbinsX()*hTH2F["nEGVsPtIsoEBMet"]->GetNbinsY());
  hTH2F["nEGVsPtIsoEBMet"]->Draw("colz");   
  counturList->Draw("lsame");

  canvasTemp2->SaveAs("results/nEGVsPtIsoEB_Met_30.pdf","pdf");
  canvasTemp2->SaveAs("results/nEGVsPtIsoEB_Met_30.png","png");

  hTH2F["nEGVsPtIsoEEMet"]->GetZaxis()->SetRangeUser(0.05,1000);   
  hTH2F["nEGVsPtIsoEEMet"]->SetContour(hTH2F["nEGVsPtIsoEEMet"]->GetNbinsX()*hTH2F["nEGVsPtIsoEEMet"]->GetNbinsY());
  hTH2F["nEGVsPtIsoEEMet"]->SetContour(1,counturLevel);
  hTH2F["nEGVsPtIsoEEMet"]->Draw("cont,list");
  canvasTemp2->Update();
  arrayList = (TObjArray*) gROOT->GetListOfSpecials()->FindObject("contours");
  counturList->Clear(); 
  for(int i = 0; i < arrayList->GetSize(); i++){
    TList* List_tmp = (TList*) arrayList->At(i);
    for (int j = 0, n = List_tmp->GetSize(); j < n; ++j) {
      TGraph *gr1 = (TGraph*) List_tmp->At(j);      
      gr1->SetLineColor(kBlack);
      gr1->SetLineWidth(2);
      counturList->Add(gr1->Clone());
    }
  }

  hTH2F["nEGVsPtIsoEEMet"]->SetContour(hTH2F["nEGVsPtIsoEEMet"]->GetNbinsX()*hTH2F["nEGVsPtIsoEEMet"]->GetNbinsY());
  hTH2F["nEGVsPtIsoEEMet"]->Draw("colz");   
  counturList->Draw("lsame");

  canvasTemp2->SaveAs("results/nEGVsPtIsoEE_Met_30.pdf","pdf");
  canvasTemp2->SaveAs("results/nEGVsPtIsoEE_Met_30.png","png");

  hTH2F["nEGVsPtIsoMT"]->GetZaxis()->SetRangeUser(0.05,1000);   
  hTH2F["nEGVsPtIsoMT"]->SetContour(hTH2F["nEGVsPtIsoMT"]->GetNbinsX()*hTH2F["nEGVsPtIsoMT"]->GetNbinsY());
  hTH2F["nEGVsPtIsoMT"]->SetContour(1,counturLevel);
  hTH2F["nEGVsPtIsoMT"]->Draw("cont,list");
  canvasTemp2->Update();
  arrayList = (TObjArray*) gROOT->GetListOfSpecials()->FindObject("contours");
  counturList->Clear(); 
  for(int i = 0; i < arrayList->GetSize(); i++){
    TList* List_tmp = (TList*) arrayList->At(i);
    for (int j = 0, n = List_tmp->GetSize(); j < n; ++j) {
      TGraph *gr1 = (TGraph*) List_tmp->At(j);      
      gr1->SetLineColor(kBlack);
      gr1->SetLineWidth(2);
      counturList->Add(gr1->Clone());
    }
  }

  hTH2F["nEGVsPtIsoMT"]->SetContour(hTH2F["nEGVsPtIsoMT"]->GetNbinsX()*hTH2F["nEGVsPtIsoMT"]->GetNbinsY());
  hTH2F["nEGVsPtIsoMT"]->Draw("colz");   
  counturList->Draw("lsame");

  canvasTemp2->SaveAs("results/nEGVsPtIso_MT_35.pdf","pdf");
  canvasTemp2->SaveAs("results/nEGVsPtIso_MT35.png","png");

  hTH2F["nEGVsPtIsoEtaRMT"]->GetZaxis()->SetRangeUser(0.005,1000);   
  hTH2F["nEGVsPtIsoEtaRMT"]->SetContour(hTH2F["nEGVsPtIsoEtaRMT"]->GetNbinsX()*hTH2F["nEGVsPtIsoEtaRMT"]->GetNbinsY());
  hTH2F["nEGVsPtIsoEtaRMT"]->SetContour(1,counturLevel);
  hTH2F["nEGVsPtIsoEtaRMT"]->Draw("cont,list");
  canvasTemp2->Update();
  arrayList = (TObjArray*) gROOT->GetListOfSpecials()->FindObject("contours");
  counturList->Clear(); 
  for(int i = 0; i < arrayList->GetSize(); i++){
    TList* List_tmp = (TList*) arrayList->At(i);
    for (int j = 0, n = List_tmp->GetSize(); j < n; ++j) {
      TGraph *gr1 = (TGraph*) List_tmp->At(j);      
      gr1->SetLineColor(kBlack);
      gr1->SetLineWidth(2);
      counturList->Add(gr1->Clone());
    }
  }

  hTH2F["nEGVsPtIsoEtaRMT"]->SetContour(hTH2F["nEGVsPtIsoEtaRMT"]->GetNbinsX()*hTH2F["nEGVsPtIsoEtaRMT"]->GetNbinsY());
  hTH2F["nEGVsPtIsoEtaRMT"]->Draw("colz");   
  counturList->Draw("lsame");

  canvasTemp2->SaveAs("results/nEGVsPtIsoEtaR_MT_35.pdf","pdf");
  canvasTemp2->SaveAs("results/nEGVsPtIsoEtaR_MT_35.png","png");


  hTH2F["nEGVsPtIsoEBMT"]->GetZaxis()->SetRangeUser(0.001,1000);   
  hTH2F["nEGVsPtIsoEBMT"]->SetContour(hTH2F["nEGVsPtIsoEBMT"]->GetNbinsX()*hTH2F["nEGVsPtIsoEBMT"]->GetNbinsY());
  hTH2F["nEGVsPtIsoEBMT"]->SetContour(1,counturLevel);
  hTH2F["nEGVsPtIsoEBMT"]->Draw("cont,list");
  canvasTemp2->Update();
  arrayList = (TObjArray*) gROOT->GetListOfSpecials()->FindObject("contours");
  counturList->Clear(); 
  for(int i = 0; i < arrayList->GetSize(); i++){
    TList* List_tmp = (TList*) arrayList->At(i);
    for (int j = 0, n = List_tmp->GetSize(); j < n; ++j) {
      TGraph *gr1 = (TGraph*) List_tmp->At(j);      
      gr1->SetLineColor(kBlack);
      gr1->SetLineWidth(2);
      counturList->Add(gr1->Clone());
    }
  }

  hTH2F["nEGVsPtIsoEBMT"]->SetContour(hTH2F["nEGVsPtIsoEBMT"]->GetNbinsX()*hTH2F["nEGVsPtIsoEBMT"]->GetNbinsY());
  hTH2F["nEGVsPtIsoEBMT"]->Draw("colz");   
  counturList->Draw("lsame");

  canvasTemp2->SaveAs("results/nEGVsPtIsoEB_MT_35.pdf","pdf");
  canvasTemp2->SaveAs("results/nEGVsPtIsoEB_MT_35.png","png");


  hTH2F["nEGVsPtIsoEEMT"]->GetZaxis()->SetRangeUser(0.01,1000);   
  hTH2F["nEGVsPtIsoEEMT"]->SetContour(hTH2F["nEGVsPtIsoEEMT"]->GetNbinsX()*hTH2F["nEGVsPtIsoEEMT"]->GetNbinsY());
  hTH2F["nEGVsPtIsoEEMT"]->SetContour(1,counturLevel);
  hTH2F["nEGVsPtIsoEEMT"]->Draw("cont,list");
  canvasTemp2->Update();
  arrayList = (TObjArray*) gROOT->GetListOfSpecials()->FindObject("contours");
  counturList->Clear(); 
  for(int i = 0; i < arrayList->GetSize(); i++){
    TList* List_tmp = (TList*) arrayList->At(i);
    for (int j = 0, n = List_tmp->GetSize(); j < n; ++j) {
      TGraph *gr1 = (TGraph*) List_tmp->At(j);      
      gr1->SetLineColor(kBlack);
      gr1->SetLineWidth(2);
      counturList->Add(gr1->Clone());
    }
  }

  hTH2F["nEGVsPtIsoEEMT"]->SetContour(hTH2F["nEGVsPtIsoEEMT"]->GetNbinsX()*hTH2F["nEGVsPtIsoEEMT"]->GetNbinsY());
  hTH2F["nEGVsPtIsoEEMT"]->Draw("colz");   
  counturList->Draw("lsame");

  canvasTemp2->SaveAs("results/nEGVsPtIsoEE_MT_35.pdf","pdf");
  canvasTemp2->SaveAs("results/nEGVsPtIsoEE_MT_35.png","png");


  // fixed pt treshold for the seed
  TLegend* leg2 = new TLegend(0.5,0.8,0.8,0.9);
  leg2->SetFillColor(0);
  leg2->SetFillStyle(0);
  leg2->SetBorderSize(0);
  leg2->SetLineColor(0);
  leg2->SetLineWidth(0);
  leg2->SetLineStyle(0);

  canvasTemp->cd();

  hTH1F["nEGVsMet"]->SetMarkerColor(kBlack);
  hTH1F["nEGVsMet"]->SetLineColor(kBlack);
  hTH1F["nEGVsMet"]->SetLineWidth(2);
  hTH1F["nEGVsMet"]->SetMarkerStyle(20);
  hTH1F["nEGVsMet"]->SetMarkerSize(1);
  hTH1F["nEGVsMet"]->Draw("E");

  f5->Draw("same");

  canvasTemp->SetLogy();
  leg2->AddEntry(hTH1F["nEGVsMet"],"SingleEG E_{T} > 25 GeV","p");
  leg2->Draw("same");
  gPad->Update();


  canvasTemp->SaveAs("results/nEGVsMet_ET_25.pdf","pdf");
  canvasTemp->SaveAs("results/nEGVsMet_ET_25.png","png");

  canvasTemp->cd();
  leg2->Clear();

  hTH1F["nEGVsIso"]->SetMarkerColor(kBlack);
  hTH1F["nEGVsIso"]->SetLineColor(kBlack);
  hTH1F["nEGVsIso"]->SetLineWidth(2);
  hTH1F["nEGVsIso"]->SetMarkerStyle(20);
  hTH1F["nEGVsIso"]->SetMarkerSize(1);
  hTH1F["nEGVsIso"]->Draw("E");
  canvasTemp->SetLogy();
  leg2->AddEntry(hTH1F["nEGVsIso"],"SingleEG E_{T} > 25 GeV","p");
  leg2->Draw("same");

  f5->Draw("same");

  gPad->Update();
  canvasTemp->SaveAs("results/nEGVsIso_ET_25.pdf","pdf");
  canvasTemp->SaveAs("results/nEGVsIso_ET_25.png","png");

  canvasTemp->cd();
  leg2->Clear();

  hTH1F["nEGVsMT"]->SetMarkerColor(kBlack);
  hTH1F["nEGVsMT"]->SetLineColor(kBlack);
  hTH1F["nEGVsMT"]->SetLineWidth(2);
  hTH1F["nEGVsMT"]->SetMarkerStyle(20);
  hTH1F["nEGVsMT"]->SetMarkerSize(1);
  hTH1F["nEGVsMT"]->Draw("E");
  canvasTemp->SetLogy();
  leg2->AddEntry(hTH1F["nEGVsMT"],"SingleEG E_{T} > 25 GeV","p");
  leg2->Draw("same");
  f1->Draw("same");
  gPad->Update();
  canvasTemp->SaveAs("results/nEGVsMT_ET_25.pdf","pdf");
  canvasTemp->SaveAs("results/nEGVsMT_ET_25.png","png");

  // ---------------------
  canvasTemp->cd();
  leg2->Clear();

  hTH1F["nEGVsMetEtaR"]->SetMarkerColor(kBlack);
  hTH1F["nEGVsMetEtaR"]->SetLineColor(kBlack);
  hTH1F["nEGVsMetEtaR"]->SetLineWidth(2);
  hTH1F["nEGVsMetEtaR"]->SetMarkerStyle(20);
  hTH1F["nEGVsMetEtaR"]->SetMarkerSize(1);
  hTH1F["nEGVsMetEtaR"]->Draw("E");
  canvasTemp->SetLogy();
  leg2->AddEntry(hTH1F["nEGVsMetEtaR"],"SingleEG ER E_{T} > 25 GeV","p");
  leg2->Draw("same");
  f1->Draw("same");
  gPad->Update();
  canvasTemp->SaveAs("results/nEGVsMetEtaR_ET_25.pdf","pdf");
  canvasTemp->SaveAs("results/nEGVsMetEtaR_ET_25.png","png");

  canvasTemp->cd();
  leg2->Clear();

  hTH1F["nEGVsIsoEtaR"]->SetMarkerColor(kBlack);
  hTH1F["nEGVsIsoEtaR"]->SetLineColor(kBlack);
  hTH1F["nEGVsIsoEtaR"]->SetLineWidth(2);
  hTH1F["nEGVsIsoEtaR"]->SetMarkerStyle(20);
  hTH1F["nEGVsIsoEtaR"]->SetMarkerSize(1);
  hTH1F["nEGVsIsoEtaR"]->Draw("E");
  canvasTemp->SetLogy();
  leg2->AddEntry(hTH1F["nEGVsIsoEtaR"],"SingleEG ER E_{T} > 25 GeV","p");
  leg2->Draw("same");
  f5->Draw("same");
  gPad->Update();

  canvasTemp->SaveAs("results/nEGVsIsoEtaR_ET_25.pdf","pdf");
  canvasTemp->SaveAs("results/nEGVsIsoEtaR_ET_25.png","png");

  canvasTemp->cd();
  leg2->Clear();

  hTH1F["nEGVsMTEtaR"]->SetMarkerColor(kBlack);
  hTH1F["nEGVsMTEtaR"]->SetLineColor(kBlack);
  hTH1F["nEGVsMTEtaR"]->SetLineWidth(2);
  hTH1F["nEGVsMTEtaR"]->SetMarkerStyle(20);
  hTH1F["nEGVsMTEtaR"]->SetMarkerSize(1);
  hTH1F["nEGVsMTEtaR"]->Draw("E");
  canvasTemp->SetLogy();
  leg2->AddEntry(hTH1F["nEGVsMT"],"SingleEG ER E_{T} > 25 GeV","p");
  leg2->Draw("same");
  f1->Draw("same");
  gPad->Update();
  canvasTemp->SaveAs("results/nEGVsMT_ET_25.pdf","pdf");
  canvasTemp->SaveAs("results/nEGVsMT_ET_25.png","png");
  
  // ------------------

  canvasTemp2->cd();
  hTH2F["nEGVsMetIso"]->GetZaxis()->SetRangeUser(0.3,100);   
  hTH2F["nEGVsMetIso"]->SetContour(hTH2F["nEGVsMetIso"]->GetNbinsX()*hTH2F["nEGVsMetIso"]->GetNbinsY());
  hTH2F["nEGVsMetIso"]->SetContour(1,counturLevel);
  hTH2F["nEGVsMetIso"]->Draw("cont,list");
  canvasTemp2->Update();
  arrayList = (TObjArray*) gROOT->GetListOfSpecials()->FindObject("contours");
  counturList->Clear(); 
  for(int i = 0; i < arrayList->GetSize(); i++){
    TList* List_tmp = (TList*) arrayList->At(i);
    for (int j = 0, n = List_tmp->GetSize(); j < n; ++j) {
      TGraph *gr1 = (TGraph*) List_tmp->At(j);      
      gr1->SetLineColor(kBlack);
      gr1->SetLineWidth(2);
      counturList->Add(gr1->Clone());
    }
  }

  hTH2F["nEGVsMetIso"]->SetContour(hTH2F["nEGVsMetIso"]->GetNbinsX()*hTH2F["nEGVsMetIso"]->GetNbinsY());
  hTH2F["nEGVsMetIso"]->Draw("colz");   
  counturList->Draw("lsame");

  canvasTemp2->SaveAs("results/nEGVsMetIso_ET_25.pdf","pdf");
  canvasTemp2->SaveAs("results/nEGVsMetIso_ET_25.png","png");

  hTH2F["nEGVsMetIsoEtaR"]->GetZaxis()->SetRangeUser(0.2,100);   
  hTH2F["nEGVsMetIsoEtaR"]->SetContour(hTH2F["nEGVsMetIsoEtaR"]->GetNbinsX()*hTH2F["nEGVsMetIsoEtaR"]->GetNbinsY());
  hTH2F["nEGVsMetIsoEtaR"]->SetContour(1,counturLevel);
  hTH2F["nEGVsMetIsoEtaR"]->Draw("cont,list");
  canvasTemp2->Update();
  arrayList = (TObjArray*) gROOT->GetListOfSpecials()->FindObject("contours");
  counturList->Clear(); 
  for(int i = 0; i < arrayList->GetSize(); i++){
    TList* List_tmp = (TList*) arrayList->At(i);
    for (int j = 0, n = List_tmp->GetSize(); j < n; ++j) {
      TGraph *gr1 = (TGraph*) List_tmp->At(j);      
      gr1->SetLineColor(kBlack);
      gr1->SetLineWidth(2);
      counturList->Add(gr1->Clone());
    }
  }

  hTH2F["nEGVsMetIsoEtaR"]->SetContour(hTH2F["nEGVsMetIsoEtaR"]->GetNbinsX()*hTH2F["nEGVsMetIsoEtaR"]->GetNbinsY());
  hTH2F["nEGVsMetIsoEtaR"]->Draw("colz");   
  counturList->Draw("lsame");

  canvasTemp2->SaveAs("results/nEGVsMetIsoEtaR_ET_25.pdf","pdf");
  canvasTemp2->SaveAs("results/nEGVsMetIsoEtaR_ET_25.png","png");

  hTH2F["nEGVsMTIso"]->GetZaxis()->SetRangeUser(0.3,100);   
  hTH2F["nEGVsMTIso"]->SetContour(hTH2F["nEGVsMTIso"]->GetNbinsX()*hTH2F["nEGVsMTIso"]->GetNbinsY());
  hTH2F["nEGVsMTIso"]->SetContour(1,counturLevel);
  hTH2F["nEGVsMTIso"]->Draw("cont,list");
  canvasTemp2->Update();
  arrayList = (TObjArray*) gROOT->GetListOfSpecials()->FindObject("contours");
  counturList->Clear(); 
  for(int i = 0; i < arrayList->GetSize(); i++){
    TList* List_tmp = (TList*) arrayList->At(i);
    for (int j = 0, n = List_tmp->GetSize(); j < n; ++j) {
      TGraph *gr1 = (TGraph*) List_tmp->At(j);      
      gr1->SetLineColor(kBlack);
      gr1->SetLineWidth(2);
      counturList->Add(gr1->Clone());
    }
  }

  hTH2F["nEGVsMTIso"]->SetContour(hTH2F["nEGVsMTIso"]->GetNbinsX()*hTH2F["nEGVsMTIso"]->GetNbinsY());
  hTH2F["nEGVsMTIso"]->Draw("colz");   
  counturList->Draw("lsame");

  canvasTemp2->SaveAs("results/nEGVsMTIso_ET_25.pdf","pdf");
  canvasTemp2->SaveAs("results/nEGVsMTIso_ET_25.png","png");

  hTH2F["nEGVsMTIsoEtaR"]->GetZaxis()->SetRangeUser(0.3,100);   
  hTH2F["nEGVsMTIsoEtaR"]->SetContour(hTH2F["nEGVsMTIsoEtaR"]->GetNbinsX()*hTH2F["nEGVsMTIsoEtaR"]->GetNbinsY());
  hTH2F["nEGVsMTIsoEtaR"]->SetContour(1,counturLevel);
  hTH2F["nEGVsMTIsoEtaR"]->Draw("cont,list");
  canvasTemp2->Update();
  arrayList = (TObjArray*) gROOT->GetListOfSpecials()->FindObject("contours");
  counturList->Clear(); 
  for(int i = 0; i < arrayList->GetSize(); i++){
    TList* List_tmp = (TList*) arrayList->At(i);
    for (int j = 0, n = List_tmp->GetSize(); j < n; ++j) {
      TGraph *gr1 = (TGraph*) List_tmp->At(j);      
      gr1->SetLineColor(kBlack);
      gr1->SetLineWidth(2);
      counturList->Add(gr1->Clone());
    }
  }

  hTH2F["nEGVsMTIsoEtaR"]->SetContour(hTH2F["nEGVsMTIsoEtaR"]->GetNbinsX()*hTH2F["nEGVsMTIsoEtaR"]->GetNbinsY());
  hTH2F["nEGVsMTIsoEtaR"]->Draw("colz");   
  counturList->Draw("lsame");

  canvasTemp2->SaveAs("results/nEGVsMTIsoEtaR_ET_25.pdf","pdf");
  canvasTemp2->SaveAs("results/nEGVsMTIsoEtaR_ET_25.png","png");

  outFile->Write();
  outFile->Close();
  delete outFile;

}

 
 


// --------------------------------------------------------------------

void goRatePlots(std::string fileType, int isCrossSec = false, int nEvents = 0) {

  std::string ROOTStyle;
  if(getenv ("ROOTStyle")!=NULL){
    ROOTStyle = getenv ("ROOTStyle");
    gROOT->ProcessLine((".x "+ROOTStyle+"/setTDRStyle.C").c_str());
  }

  int nBunches50ns = 1368;
  int nBunches25ns = 2508; //2508 is what agreed with TSG for # bunches

  float xSec13TeV = isCrossSec ? 78.26 : 80.; // Using McM for cross section comparison and 80 (agreed with TSG) for rates
  float xSec8TeV  = 72.7; 

  if (fileType == "DATA") {
      BasicRatePlots basicRatePlots("/afs/cern.ch/user/h/heistera/scratch1/L1Ntuples/L1TreeL1Accept_207477_LS_57_133.root");
      basicRatePlots.run(true,"DATA_207477",57,133,xSec8TeV,999.,nBunches50ns,isCrossSec,nEvents); // 999 is dummy do not use for cross-section
    }

  else if( fileType == "Neutrino_Pt-2to20_gu_PU40bx25") {
      BasicRatePlots basicRatePlots("Neutrino_Pt-2to20_gu_PU40bx25.txt");
      basicRatePlots.run(false,"Neutrino_PU40bx25",0,500000000,xSec13TeV,40,nBunches25ns,isCrossSec,nEvents); // 999 is dummy do not use for cross-section
  }

  else if( fileType == "Neutrino_Pt-2to20_gu_PU40bx50") {
      BasicRatePlots basicRatePlots("Neutrino_Pt-2to20_gu_PU40bx50.txt");
      basicRatePlots.run(false,"Neutrino_PU40bx50",0,500000000,xSec13TeV,40,nBunches50ns,isCrossSec,nEvents); // 999 is dummy do not use for cross-section
  }

  else if( fileType == "Neutrino_Pt-2to20_gu_PU20bx25") {
      BasicRatePlots basicRatePlots("Neutrino_Pt-2to20_gu_PU20bx25.txt");
      basicRatePlots.run(false,"Neutrino_PU20bx25",0,500000000,xSec13TeV,20,nBunches25ns,isCrossSec,nEvents); // 999 is dummy do not use for cross-section
  }

  else{
      std::cout << "Config param " << fileType << " invalid! \n"
		<< "Valid fileType values are : DATA, Neutrino_Pt-2to20_gu_PU40bx25, Neutrino_Pt-2to20_gu_PU40bx50, Neutrino_Pt-2to20_gu_PU20bx25 "<<std::endl;
  }
    
}



