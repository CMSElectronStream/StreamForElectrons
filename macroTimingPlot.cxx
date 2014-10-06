#include <iostream>
#include <vector>
#include <string>
 
#include "TFile.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TSystem.h"
#include "TROOT.h"

void plotSingleHistogram(TH1F* histo, const std::string & plotName, const std::string & sampleName, const int & normalize, const int & dimX = 0, const int & dimY = 0);
void plotDoubleHistogram(TH1F* histo1, TH1F* histo2, const std::string & plotName, const std::string & sampleName, const int & normalize, const int & dimX = 0, const int & dimY = 0);


int main (int argc, char** argv){

  // Set Root style from global enviroment path                                                                                                                                           
  std::string ROOTStyle;
  if(getenv ("ROOTStyle")!=NULL){
    ROOTStyle = getenv ("ROOTStyle");
    gROOT->ProcessLine((".x "+ROOTStyle+"/rootLogon.C").c_str());
    gROOT->ProcessLine((".x "+ROOTStyle+"/rootPalette.C").c_str());
    gROOT->ProcessLine((".x "+ROOTStyle+"/rootColors.C").c_str());
    gROOT->ProcessLine((".x "+ROOTStyle+"/setTDRStyle.C").c_str());
  }

  gStyle->SetOptStat(0);
  gStyle->SetPadLeftMargin(0.13);
  gStyle->SetPadTopMargin(0.09);
  gStyle->SetErrorX(0.5);

  system("mkdir -p PlotTiming_Minibias/");
  system("rm PlotTiming_Minibias/*");

  system("mkdir -p PlotTiming_Signal/");
  system("rm PlotTiming_Signal/*");

  TFile* originalMenu_minibias  = TFile::Open("/data2/rgerosa/ECALSTREAMSTUDY/Timing/MiniBias/HLT_ORIGINAL/DQM_V0001_R000000001__HLT__FastTimerService__All.root","READ");
  TFile* modifiedMenu_minibias  = TFile::Open("/data2/rgerosa/ECALSTREAMSTUDY/Timing/MiniBias/HLT_MODIFIED/DQM_V0001_R000000001__HLT__FastTimerService__All.root","READ");
  TFile* streamMenu_minibias    = TFile::Open("/data2/rgerosa/ECALSTREAMSTUDY/Timing/MiniBias/HLT_STREAM/DQM_V0001_R000000001__HLT__FastTimerService__All.root","READ");

  TFile* originalMenu_signal    = TFile::Open("/data2/rgerosa/ECALSTREAMSTUDY/Timing/WENU_SIGNAL/HLT_ORIGINAL/DQM_V0001_R000000001__HLT__FastTimerService__All.root","READ");
  TFile* modifiedMenu_signal    = TFile::Open("/data2/rgerosa/ECALSTREAMSTUDY/Timing/WENU_SIGNAL/HLT_MODIFIED/DQM_V0001_R000000001__HLT__FastTimerService__All.root","READ");
  TFile* streamMenu_signal      = TFile::Open("/data2/rgerosa/ECALSTREAMSTUDY/Timing/WENU_SIGNAL/HLT_STREAM/DQM_V0001_R000000001__HLT__FastTimerService__All.root","READ");

  // Minibias analysis taking histograms  
  TH1F* originalMenu_minibias_event     = (TH1F*) originalMenu_minibias->FindObjectAny("all_paths");
  originalMenu_minibias_event->Rebin(2);
  TH1F* originalMenu_minibias_endpath   = (TH1F*) originalMenu_minibias->FindObjectAny("all_endpaths");
  originalMenu_minibias_endpath->Rebin(2);
  TH1F* originalMenu_minibias_Aoutput_1 = (TH1F*) originalMenu_minibias->FindObjectAny("AOutput_total");
  TH1F* originalMenu_minibias_Aoutput_2 = (TH1F*) originalMenu_minibias->FindObjectAny("AOutput_module_average");
  TH1F* originalMenu_minibias_Aoutput_3 = (TH1F*) originalMenu_minibias->FindObjectAny("AOutput_module_active");
  TH1F* originalMenu_minibias_Aoutput_4 = (TH1F*) originalMenu_minibias->FindObjectAny("AOutput_module_efficiency");
  TH1F* originalMenu_minibias_Ele27_1   = (TH1F*) originalMenu_minibias->FindObjectAny("HLT_Ele27_eta2p1_WP85_Gsf_v1_total");
  TH1F* originalMenu_minibias_Ele27_2   = (TH1F*) originalMenu_minibias->FindObjectAny("HLT_Ele27_eta2p1_WP85_Gsf_v1_module_average");
  TH1F* originalMenu_minibias_Ele27_3   = (TH1F*) originalMenu_minibias->FindObjectAny("HLT_Ele27_eta2p1_WP85_Gsf_v1_module_efficiency");

  plotSingleHistogram(originalMenu_minibias_event,    "PlotTiming_Minibias/minibias_original_all_paths","Neutrino Gun p_{T} [0,20] GeV",1); 
  plotSingleHistogram(originalMenu_minibias_endpath,  "PlotTiming_Minibias/minibias_original_endpaths","Neutrino Gun p_{T} [0,20] GeV",1); 
  plotSingleHistogram(originalMenu_minibias_Aoutput_1,"PlotTiming_Minibias/minibias_original_AOutput_total","Neutrino Gun p_{T} [0,20] GeV",1); 
  plotSingleHistogram(originalMenu_minibias_Aoutput_2,"PlotTiming_Minibias/minibias_original_AOutput_module_average","Neutrino Gun p_{T} [0,20] GeV",0); 
  plotSingleHistogram(originalMenu_minibias_Aoutput_3,"PlotTiming_Minibias/minibias_original_AOutput_module_active","Neutrino Gun p_{T} [0,20] GeV",0); 
  plotSingleHistogram(originalMenu_minibias_Aoutput_4,"PlotTiming_Minibias/minibias_original_AOutput_module_efficiency","Neutrino Gun p_{T} [0,20] GeV",0); 
  plotSingleHistogram(originalMenu_minibias_Ele27_1,  "PlotTiming_Minibias/minibias_original_HLT_Ele27_eta2p1_WP85_Gsf_v1_total","Neutrino Gun p_{T} [0,20] GeV",1); 
  plotSingleHistogram(originalMenu_minibias_Ele27_2,  "PlotTiming_Minibias/minibias_original_HLT_Ele27_eta2p1_WP85_Gsf_v1_module_average","Neutrino Gun p_{T} [0,20] GeV",0,1200,650); 
  plotSingleHistogram(originalMenu_minibias_Ele27_3,  "PlotTiming_Minibias/minibias_original_HLT_Ele27_eta2p1_WP85_Gsf_v1_module_efficiency","Neutrino Gun p_{T} [0,20] GeV",0,1200,650); 

  TH1F* modifiedMenu_minibias_event     = (TH1F*) modifiedMenu_minibias->FindObjectAny("all_paths");
  modifiedMenu_minibias_event->Rebin(2);
  TH1F* modifiedMenu_minibias_endpath   = (TH1F*) modifiedMenu_minibias->FindObjectAny("all_endpaths");
  modifiedMenu_minibias_endpath->Rebin(2);
  TH1F* modifiedMenu_minibias_Aoutput_1 = (TH1F*) modifiedMenu_minibias->FindObjectAny("AOutput_total");
  TH1F* modifiedMenu_minibias_Aoutput_2 = (TH1F*) modifiedMenu_minibias->FindObjectAny("AOutput_module_average");
  TH1F* modifiedMenu_minibias_Aoutput_3 = (TH1F*) modifiedMenu_minibias->FindObjectAny("AOutput_module_active");
  TH1F* modifiedMenu_minibias_Aoutput_4 = (TH1F*) modifiedMenu_minibias->FindObjectAny("AOutput_module_efficiency");
  TH1F* modifiedMenu_minibias_Ele27_PFMET_1 = (TH1F*) modifiedMenu_minibias->FindObjectAny("HLT_Ele27_eta2p1_WP85_PFMET_MT50_Gsf_v1_total");
  TH1F* modifiedMenu_minibias_Ele27_PFMET_2 = (TH1F*) modifiedMenu_minibias->FindObjectAny("HLT_Ele27_eta2p1_WP85_PFMET_MT50_Gsf_v1_module_average");
  TH1F* modifiedMenu_minibias_Ele27_PFMET_3 = (TH1F*) modifiedMenu_minibias->FindObjectAny("HLT_Ele27_eta2p1_WP85_PFMET_MT50_Gsf_v1_module_efficiency");

  plotDoubleHistogram(originalMenu_minibias_event,modifiedMenu_minibias_event,"PlotTiming_Minibias/minibias_comparison_modified_all_paths","Neutrino Gun p_{T} [0,20] GeV",1); 
  plotDoubleHistogram(originalMenu_minibias_endpath,modifiedMenu_minibias_endpath,"PlotTiming_Minibias/minibias_comparison_modified_endpaths","Neutrino Gun p_{T} [0,20] GeV",1); 
  plotDoubleHistogram(originalMenu_minibias_Aoutput_1,modifiedMenu_minibias_Aoutput_1,"PlotTiming_Minibias/minibias_comparison_modified_AOutput_total","Neutrino Gun p_{T} [0,20] GeV",0); 
  plotDoubleHistogram(originalMenu_minibias_Aoutput_2,modifiedMenu_minibias_Aoutput_2,"PlotTiming_Minibias/minibias_comparison_modified_AOutput_module_average","Neutrino Gun p_{T} [0,20] GeV",0); 
  plotDoubleHistogram(originalMenu_minibias_Aoutput_3,modifiedMenu_minibias_Aoutput_3,"PlotTiming_Minibias/minibias_comparison_modified_AOutput_module_active","Neutrino Gun p_{T} [0,20] GeV",0); 
  plotDoubleHistogram(originalMenu_minibias_Aoutput_4,modifiedMenu_minibias_Aoutput_4,"PlotTiming_Minibias/minibias_comparison_modified_AOutput_module_efficiency","Neutrino Gun p_{T} [0,20] GeV",0); 
  plotSingleHistogram(modifiedMenu_minibias_Ele27_PFMET_1,"PlotTiming_Minibias/minibias_modified_HLT_Ele27_eta2p1_WP85_PFMET_Gsf_v1_total","Neutrino Gun p_{T} [0,20] GeV",0); 
  plotSingleHistogram(modifiedMenu_minibias_Ele27_PFMET_2,"PlotTiming_Minibias/minibias_modified_HLT_Ele27_eta2p1_WP85_PFMET_Gsf_v1_module_average","Neutrino Gun p_{T} [0,20] GeV",0,1200,650); 
  plotSingleHistogram(modifiedMenu_minibias_Ele27_PFMET_3,"PlotTiming_Minibias/minibias_modified_HLT_Ele27_eta2p1_WP85_PFMET_Gsf_v1_module_efficiency","Neutrino Gun p_{T} [0,20] GeV",0,1200,650); 
  TH1F* streamMenu_minibias_event     = (TH1F*) streamMenu_minibias->FindObjectAny("all_paths");
  streamMenu_minibias_event->Rebin(2);
  TH1F* streamMenu_minibias_endpath   = (TH1F*) streamMenu_minibias->FindObjectAny("all_endpaths");
  streamMenu_minibias_endpath->Rebin(2);
  TH1F* streamMenu_minibias_Aoutput_1 = (TH1F*) streamMenu_minibias->FindObjectAny("AOutput_total");
  TH1F* streamMenu_minibias_Aoutput_2 = (TH1F*) streamMenu_minibias->FindObjectAny("AOutput_module_average");
  TH1F* streamMenu_minibias_Aoutput_3 = (TH1F*) streamMenu_minibias->FindObjectAny("AOutput_module_active");
  TH1F* streamMenu_minibias_Aoutput_4 = (TH1F*) streamMenu_minibias->FindObjectAny("AOutput_module_efficiency");
  TH1F* streamMenu_minibias_Soutput_1 = (TH1F*) streamMenu_minibias->FindObjectAny("ElectronStreamOutput_total");
  TH1F* streamMenu_minibias_Soutput_2 = (TH1F*) streamMenu_minibias->FindObjectAny("ElectronStreamOutput_module_average");
  TH1F* streamMenu_minibias_Soutput_3 = (TH1F*) streamMenu_minibias->FindObjectAny("ElectronStreamOutput_module_active");
  TH1F* streamMenu_minibias_Soutput_4 = (TH1F*) streamMenu_minibias->FindObjectAny("ElectronStreamOutput_module_efficiency");

  plotDoubleHistogram(originalMenu_minibias_event,streamMenu_minibias_event,"PlotTiming_Minibias/minibias_comparison_stream_all_paths","Neutrino Gun p_{T} [0,20] GeV",1); 
  plotDoubleHistogram(originalMenu_minibias_endpath,streamMenu_minibias_endpath,"PlotTiming_Minibias/minibias_comparison_stream_endpaths","Neutrino Gun p_{T} [0,20] GeV",1); 
  plotDoubleHistogram(originalMenu_minibias_Aoutput_1,streamMenu_minibias_Aoutput_1,"PlotTiming_Minibias/minibias_comparison_stream_AOutput_total","Neutrino Gun p_{T} [0,20] GeV",0); 
  plotDoubleHistogram(originalMenu_minibias_Aoutput_2,streamMenu_minibias_Aoutput_2,"PlotTiming_Minibias/minibias_comparison_stream_AOutput_module_average","Neutrino Gun p_{T} [0,20] GeV",0); 
  plotDoubleHistogram(originalMenu_minibias_Aoutput_3,streamMenu_minibias_Aoutput_3,"PlotTiming_Minibias/minibias_comparison_stream_AOutput_module_active","Neutrino Gun p_{T} [0,20] GeV",0); 
  plotDoubleHistogram(originalMenu_minibias_Aoutput_4,streamMenu_minibias_Aoutput_4,"PlotTiming_Minibias/minibias_comparison_stream_AOutput_module_efficiency","Neutrino Gun p_{T} [0,20] GeV",0); 
  plotSingleHistogram(streamMenu_minibias_Soutput_1,"PlotTiming_Minibias/stream_SOutput_total","Neutrino Gun p_{T} [0,20] GeV",0); 
  plotSingleHistogram(streamMenu_minibias_Soutput_2,"PlotTiming_Minibias/stream_AOutput_module_average","Neutrino Gun p_{T} [0,20] GeV",0); 
  plotSingleHistogram(streamMenu_minibias_Soutput_3,"PlotTiming_Minibias/stream_SOutput_module_active","Neutrino Gun p_{T} [0,20] GeV",0); 
  plotSingleHistogram(streamMenu_minibias_Soutput_4,"PlotTiming_Minibias/stream_SOutput_module_efficiency","Neutrino Gun p_{T} [0,20] GeV",0); 

  // do the same for signal
  TH1F* originalMenu_signal_event     = (TH1F*) originalMenu_signal->FindObjectAny("all_paths");
  TH1F* originalMenu_signal_endpath   = (TH1F*) originalMenu_signal->FindObjectAny("all_endpaths");
  TH1F* originalMenu_signal_Aoutput_1 = (TH1F*) originalMenu_signal->FindObjectAny("AOutput_total");
  TH1F* originalMenu_signal_Aoutput_2 = (TH1F*) originalMenu_signal->FindObjectAny("AOutput_module_average");
  TH1F* originalMenu_signal_Aoutput_3 = (TH1F*) originalMenu_signal->FindObjectAny("AOutput_module_active");
  TH1F* originalMenu_signal_Aoutput_4 = (TH1F*) originalMenu_signal->FindObjectAny("AOutput_module_efficiency");
  TH1F* originalMenu_signal_Ele27_1 = (TH1F*) originalMenu_signal->FindObjectAny("HLT_Ele27_eta2p1_WP85_Gsf_v1_total");
  TH1F* originalMenu_signal_Ele27_2 = (TH1F*) originalMenu_signal->FindObjectAny("HLT_Ele27_eta2p1_WP85_Gsf_v1_module_average");
  TH1F* originalMenu_signal_Ele27_3 = (TH1F*) originalMenu_signal->FindObjectAny("HLT_Ele27_eta2p1_WP85_Gsf_v1_module_efficiency");

  plotSingleHistogram(originalMenu_signal_event,"PlotTiming_Signal/signal_original_all_paths","W #rightarrow e#nu Pythia 8",0); 
  plotSingleHistogram(originalMenu_signal_endpath,"PlotTiming_Signal/signal_original_endpaths","W #rightarrow e#nu Pythia 8",0); 
  plotSingleHistogram(originalMenu_signal_Aoutput_1,"PlotTiming_Signal/signal_original_AOutput_total","W #rightarrow e#nu Pythia 8",0); 
  plotSingleHistogram(originalMenu_signal_Aoutput_2,"PlotTiming_Signal/signal_original_AOutput_module_average","W #rightarrow e#nu Pythia 8",0); 
  plotSingleHistogram(originalMenu_signal_Aoutput_3,"PlotTiming_Signal/signal_original_AOutput_module_active","W #rightarrow e#nu Pythia 8",0); 
  plotSingleHistogram(originalMenu_signal_Aoutput_4,"PlotTiming_Signal/signal_original_AOutput_module_efficiency","W #rightarrow e#nu Pythia 8",0); 
  plotSingleHistogram(originalMenu_signal_Ele27_1,"PlotTiming_Signal/signal_original_HLT_Ele27_eta2p1_WP85_Gsf_v1_total","W #rightarrow e#nu Pythia 8",0); 
  plotSingleHistogram(originalMenu_signal_Ele27_2,"PlotTiming_Signal/signal_original_HLT_Ele27_eta2p1_WP85_Gsf_v1_module_average","W #rightarrow e#nu Pythia 8",0); 
  plotSingleHistogram(originalMenu_signal_Ele27_3,"PlotTiming_Signal/signal_original_HLT_Ele27_eta2p1_WP85_Gsf_v1_module_efficiency","W #rightarrow e#nu Pythia 8",0); 

  TH1F* modifiedMenu_signal_event   = (TH1F*) modifiedMenu_signal->FindObjectAny("all_paths");
  TH1F* modifiedMenu_signal_endpath = (TH1F*) modifiedMenu_signal->FindObjectAny("all_endpaths");
  TH1F* modifiedMenu_signal_Aoutput_1 = (TH1F*) modifiedMenu_signal->FindObjectAny("AOutput_total");
  TH1F* modifiedMenu_signal_Aoutput_2 = (TH1F*) modifiedMenu_signal->FindObjectAny("AOutput_module_average");
  TH1F* modifiedMenu_signal_Aoutput_3 = (TH1F*) modifiedMenu_signal->FindObjectAny("AOutput_module_active");
  TH1F* modifiedMenu_signal_Aoutput_4 = (TH1F*) modifiedMenu_signal->FindObjectAny("AOutput_module_efficiency");
  TH1F* modifiedMenu_signal_Ele27_PFMET_1 = (TH1F*) modifiedMenu_signal->FindObjectAny("HLT_Ele27_eta2p1_WP85_PFMET_MT50_Gsf_v1_total");
  TH1F* modifiedMenu_signal_Ele27_PFMET_2 = (TH1F*) modifiedMenu_signal->FindObjectAny("HLT_Ele27_eta2p1_WP85_PFMET_MT50_Gsf_v1_module_average");
  TH1F* modifiedMenu_signal_Ele27_PFMET_3 = (TH1F*) modifiedMenu_signal->FindObjectAny("HLT_Ele27_eta2p1_WP85_PFMET_MT50_Gsf_v1_module_efficiency");

  plotDoubleHistogram(originalMenu_signal_event,modifiedMenu_signal_event,"PlotTiming_Signal/signal_comparison_modified_all_paths","W #rightarrow e#nu Pythia 8",1); 
  plotDoubleHistogram(originalMenu_signal_endpath,modifiedMenu_signal_endpath,"PlotTiming_Signal/signal_comparison_modified_endpaths","W #rightarrow e#nu Pythia 8",1); 
  plotDoubleHistogram(originalMenu_signal_Aoutput_1,modifiedMenu_signal_Aoutput_1,"PlotTiming_Signal/signal_comparison_modified_AOutput_total","W #rightarrow e#nu Pythia 8",0); 
  plotDoubleHistogram(originalMenu_signal_Aoutput_2,modifiedMenu_signal_Aoutput_2,"PlotTiming_Signal/signal_comparison_modified_AOutput_module_average","W #rightarrow e#nu Pythia 8",0); 
  plotDoubleHistogram(originalMenu_signal_Aoutput_3,modifiedMenu_signal_Aoutput_3,"PlotTiming_Signal/signal_comparison_modified_AOutput_module_active","W #rightarrow e#nu Pythia 8",0); 
  plotDoubleHistogram(originalMenu_signal_Aoutput_4,modifiedMenu_signal_Aoutput_4,"PlotTiming_Signal/signal_comparison_modified_AOutput_module_efficiency","W #rightarrow e#nu Pythia 8",0); 
  plotSingleHistogram(modifiedMenu_signal_Ele27_PFMET_1,"PlotTiming_Signal/signal_modified_HLT_Ele27_eta2p1_WP85_PFMET_Gsf_v1_total","W #rightarrow e#nu Pythia 8",0); 
  plotSingleHistogram(modifiedMenu_signal_Ele27_PFMET_2,"PlotTiming_Signal/signal_modified_HLT_Ele27_eta2p1_WP85_PFMET_Gsf_v1_module_average","W #rightarrow e#nu Pythia 8",0); 
  plotSingleHistogram(modifiedMenu_signal_Ele27_PFMET_3,"PlotTiming_Signal/signal_modified_HLT_Ele27_eta2p1_WP85_PFMET_Gsf_v1_module_efficiency","W #rightarrow e#nu Pythia 8",0); 

  TH1F* streamMenu_signal_event   = (TH1F*) streamMenu_signal->FindObjectAny("all_paths");
  TH1F* streamMenu_signal_endpath = (TH1F*) streamMenu_signal->FindObjectAny("all_endpaths");
  TH1F* streamMenu_signal_Aoutput_1 = (TH1F*) streamMenu_signal->FindObjectAny("AOutput_total");
  TH1F* streamMenu_signal_Aoutput_2 = (TH1F*) streamMenu_signal->FindObjectAny("AOutput_module_average");
  TH1F* streamMenu_signal_Aoutput_3 = (TH1F*) streamMenu_signal->FindObjectAny("AOutput_module_active");
  TH1F* streamMenu_signal_Aoutput_4 = (TH1F*) streamMenu_signal->FindObjectAny("AOutput_module_efficiency");
  TH1F* streamMenu_signal_Soutput_1 = (TH1F*) streamMenu_signal->FindObjectAny("ElectronStreamOutput_total");
  TH1F* streamMenu_signal_Soutput_2 = (TH1F*) streamMenu_signal->FindObjectAny("ElectronStreamOutput_module_average");
  TH1F* streamMenu_signal_Soutput_3 = (TH1F*) streamMenu_signal->FindObjectAny("ElectronStreamOutput_module_active");
  TH1F* streamMenu_signal_Soutput_4 = (TH1F*) streamMenu_signal->FindObjectAny("ElectronStreamOutput_module_efficiency");

  plotDoubleHistogram(originalMenu_signal_event,streamMenu_signal_event,"PlotTiming_Signal/signal_comparison_stream_all_paths","W #rightarrow e#nu Pythia 8",1); 
  plotDoubleHistogram(originalMenu_signal_endpath,streamMenu_signal_endpath,"PlotTiming_Signal/signal_comparison_stream_endpaths","W #rightarrow e#nu Pythia 8",1); 
  plotDoubleHistogram(originalMenu_signal_Aoutput_1,streamMenu_signal_Aoutput_1,"PlotTiming_Signal/signal_comparison_stream_AOutput_total","W #rightarrow e#nu Pythia 8",0); 
  plotDoubleHistogram(originalMenu_signal_Aoutput_2,streamMenu_signal_Aoutput_2,"PlotTiming_Signal/signal_comparison_stream_AOutput_module_average","W #rightarrow e#nu Pythia 8",0); 
  plotDoubleHistogram(originalMenu_signal_Aoutput_3,streamMenu_signal_Aoutput_3,"PlotTiming_Signal/signal_comparison_stream_AOutput_module_active","W #rightarrow e#nu Pythia 8",0); 
  plotDoubleHistogram(originalMenu_signal_Aoutput_4,streamMenu_signal_Aoutput_4,"PlotTiming_Signal/signal_comparison_stream_AOutput_module_efficiency","W #rightarrow e#nu Pythia 8",0); 
  plotSingleHistogram(streamMenu_signal_Soutput_1,"PlotTiming_Signal/stream_SOutput_total","W #rightarrow e#nu Pythia 8",0); 
  plotSingleHistogram(streamMenu_signal_Soutput_2,"PlotTiming_Signal/stream_AOutput_module_average","W #rightarrow e#nu Pythia 8",0); 
  plotSingleHistogram(streamMenu_signal_Soutput_3,"PlotTiming_Signal/stream_SOutput_module_active","W #rightarrow e#nu Pythia 8",0); 
  plotSingleHistogram(streamMenu_signal_Soutput_4,"PlotTiming_Signal/stream_SOutput_module_efficiency","W #rightarrow e#nu Pythia 8",0); 

  return 0 ;

}

void plotSingleHistogram(TH1F* histo, const std::string & plotName, const std::string & sampleName, const int & normalize, const int & dimX, const int & dimY){

  TCanvas *chisto = 0;
  if(dimX == 0 and dimY == 0)        chisto = new TCanvas("chisto","",180,52,550,550);
  else if(dimX != 0 and dimY == 0)   chisto = new TCanvas("chisto","",180,52,dimX,550);
  else if(dimX == 0 and dimY != 0)   chisto = new TCanvas("chisto","",180,52,550,dimY);
  else if(dimX != 0 and dimY != 0)   chisto = new TCanvas("chisto","",180,52,dimX,dimY);

  chisto->SetTicks();
  chisto->SetFillColor(0);
  chisto->SetBorderMode(0);
  chisto->SetBorderSize(2);
  chisto->SetTickx(1);
  chisto->SetTicky(1);
  chisto->SetRightMargin(0.05);
  chisto->SetBottomMargin(0.12);
  chisto->SetFrameBorderMode(0);

  histo->SetLineWidth(2);
  histo->SetMarkerStyle(21);
  histo->SetMarkerSize(1.0);
  histo->GetXaxis()->SetNdivisions(505);
  histo->GetYaxis()->SetNdivisions(505);
  histo->GetXaxis()->SetLabelOffset(0.012);
  histo->GetXaxis()->SetLabelSize(0.038);
  histo->GetXaxis()->SetTitleSize(0.05);
  histo->GetXaxis()->SetTitleOffset(1.10);
  histo->GetYaxis()->SetLabelOffset(0.012);
  histo->GetYaxis()->SetLabelSize(0.038);
  histo->GetYaxis()->SetTitleSize(0.05);
  histo->GetYaxis()->SetTitleOffset(1.18);

  TLatex *   tex = new TLatex(0.94,0.92," 13 TeV");
  tex->SetNDC();
  tex->SetTextAlign(31);
  tex->SetTextFont(42);
  tex->SetTextSize(0.04);
  tex->SetLineWidth(2);
  TLatex * tex2 = new TLatex(0.14,0.92,"CMS");
  tex2->SetNDC();
  tex2->SetTextFont(61);
  tex2->SetTextSize(0.04);
  tex2->SetLineWidth(2);
  TLatex * tex3 = new TLatex(0.236,0.92,"Simulation Preliminary");
  tex3->SetNDC();
  tex3->SetTextFont(52);
  tex3->SetTextSize(0.035);
  tex3->SetLineWidth(2);

  TLatex *   Banner = new TLatex(0.52,0.82,Form("%s",sampleName.c_str()));
  Banner->SetNDC();
  Banner->SetTextAlign(11);
  Banner->SetTextFont(42);
  Banner->SetTextSize(0.036);
  Banner->SetLineWidth(2);

  if(normalize) { 
    histo->Sumw2();
    histo->Scale(1./histo->Integral(0,histo->GetNbinsX()));
  }

  histo->Draw("ple");
  tex->Draw("same");
  tex2->Draw("same");
  tex3->Draw("same");
  Banner->Draw("same");

  chisto->SaveAs(Form("%s.pdf",plotName.c_str()),"pdf");
  chisto->SaveAs(Form("%s.png",plotName.c_str()),"png");

  chisto->SetLogy();
  chisto->SaveAs(Form("%s_Log.pdf",plotName.c_str()),"pdf");
  chisto->SaveAs(Form("%s_Log.png",plotName.c_str()),"png");

  delete chisto;
  delete tex;
  delete tex2;
  delete tex3;
  delete Banner;

  return ;

}


void plotDoubleHistogram(TH1F* histo1, TH1F* histo2, const std::string & plotName, const std::string & sampleName, const int & normalize,const int & dimX, const int & dimY){

  TCanvas *chisto = 0;
  if(dimX == 0 and dimY == 0)        chisto = new TCanvas("chisto","",180,52,550,550);
  else if(dimX != 0 and dimY == 0)   chisto = new TCanvas("chisto","",180,52,dimX,550);
  else if(dimX == 0 and dimY != 0)   chisto = new TCanvas("chisto","",180,52,550,dimY);
  else if(dimX != 0 and dimY != 0)   chisto = new TCanvas("chisto","",180,52,dimX,dimY);

  chisto->SetTicks();
  chisto->SetFillColor(0);
  chisto->SetBorderMode(0);
  chisto->SetBorderSize(2);
  chisto->SetTickx(1);
  chisto->SetTicky(1);
  chisto->SetRightMargin(0.05);
  chisto->SetBottomMargin(0.12);
  chisto->SetFrameBorderMode(0);

  histo1->SetLineWidth(2);
  histo1->SetMarkerStyle(21);
  histo1->SetMarkerSize(1);
  histo1->GetXaxis()->SetNdivisions(505);
  histo1->GetYaxis()->SetNdivisions(505);
  histo1->GetXaxis()->SetLabelOffset(0.012);
  histo1->GetXaxis()->SetLabelSize(0.038);
  histo1->GetXaxis()->SetTitleSize(0.05);
  histo1->GetXaxis()->SetTitleOffset(1.10);
  histo1->GetYaxis()->SetLabelOffset(0.012);
  histo1->GetYaxis()->SetLabelSize(0.038);
  histo1->GetYaxis()->SetTitleSize(0.05);
  histo1->GetYaxis()->SetTitleOffset(1.18);

  histo2->SetLineWidth(2);
  histo2->SetLineColor(2);
  histo2->SetMarkerColor(2);
  histo2->SetMarkerStyle(21);
  histo2->SetMarkerSize(1);
  histo2->GetXaxis()->SetNdivisions(505);
  histo2->GetYaxis()->SetNdivisions(505);
  histo2->GetXaxis()->SetLabelOffset(0.012);
  histo2->GetXaxis()->SetLabelSize(0.038);
  histo2->GetXaxis()->SetTitleSize(0.05);
  histo2->GetXaxis()->SetTitleOffset(1.10);
  histo2->GetYaxis()->SetLabelOffset(0.012);
  histo2->GetYaxis()->SetLabelSize(0.038);
  histo2->GetYaxis()->SetTitleSize(0.05);
  histo2->GetYaxis()->SetTitleOffset(1.18);


  TLatex *   tex = new TLatex(0.94,0.92," 13 TeV");
  tex->SetNDC();
  tex->SetTextAlign(31);
  tex->SetTextFont(42);
  tex->SetTextSize(0.04);
  tex->SetLineWidth(2);
  TLatex * tex2 = new TLatex(0.14,0.92,"CMS");
  tex2->SetNDC();
  tex2->SetTextFont(61);
  tex2->SetTextSize(0.04);
  tex2->SetLineWidth(2);
  TLatex * tex3 = new TLatex(0.236,0.92,"Simulation Preliminary");
  tex3->SetNDC();
  tex3->SetTextFont(52);
  tex3->SetTextSize(0.035);
  tex3->SetLineWidth(2);

  TLatex *   Banner = new TLatex(0.52,0.82,Form("%s",sampleName.c_str()));
  Banner->SetNDC();
  Banner->SetTextAlign(11);
  Banner->SetTextFont(42);
  Banner->SetTextSize(0.036);
  Banner->SetLineWidth(2);

  TLegend* Legend = new TLegend(0.175,0.2,0.53,0.4);
  Legend->SetBorderSize(0);
  Legend->SetFillColor(0);
  Legend->SetFillStyle(0);
  Legend->SetTextSize(0.031);
  Legend->SetTextFont(42);

  if(normalize) { 
    histo1->Sumw2();
    histo1->Scale(1./histo1->Integral(0,histo1->GetNbinsX()));
    histo2->Sumw2();
    histo2->Scale(1./histo2->Integral(0,histo2->GetNbinsX()));
  }

  histo1->SetMaximum(std::max(histo1->GetMaximum(),histo2->GetMaximum())*1.1);

  histo1->Draw("ple");
  histo2->Draw("pelsame");
  tex->Draw("same");
  tex2->Draw("same");
  tex3->Draw("same");
  Banner->Draw("same");

  chisto->SaveAs(Form("%s.pdf",plotName.c_str()),"pdf");
  chisto->SaveAs(Form("%s.png",plotName.c_str()),"png");

  chisto->SetLogy();
  chisto->SaveAs(Form("%s_Log.pdf",plotName.c_str()),"pdf");
  chisto->SaveAs(Form("%s_Log.png",plotName.c_str()),"png");

  delete chisto;
  delete tex;
  delete tex2;
  delete tex3;
  delete Banner;

  return ;


}
