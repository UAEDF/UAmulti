#include <TROOT.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TGraph.h>
#include <TString.h>
#include <TProfile.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TFrame.h>
#include <TFile.h>
#include <TDirectory.h>
#include <TLegend.h>
#include <TText.h>


#include <TSystem.h>

#include <iostream>
#include <sstream>
using namespace std;

#include "makePlots.h"
int     globalTraking = 0;
double  globalEnergy = 0.9;

//makePlot Plots;

#include "../macro/fileManager.C"
#include "plot.C"

void plotptextrapolation(double energy = 7, int iBin = 5 , int iSaveFig = 0, float npx = 1000 , float npy = 600){

  globalEnergy = energy;
  gROOT->ProcessLine(".x cmsStyleRoot.C");

  plotReset();
 
  globalCanvasSizeX = npx; 
  globalCanvasSizeY = npy;
 
  XaxisTitle = "n";
  YaxisTitle = "P_{n}";
  
  LegendTitle = "pt extra. from :";

  xGlobalLabel = 0.6 ;
  if ( globalEnergy == 0.9 ) globalLabel =  "CMS Preliminary 0.9 TeV";
  if ( globalEnergy == 2.36) globalLabel =  "CMS Preliminary 2.36 TeV";
  if ( globalEnergy == 7.0 ) globalLabel =  "CMS Preliminary 7 TeV";

  globalSaveFig = iSaveFig;
  if ( globalEnergy == 0.9 ) globalFigBaseName = "0.9TeV_ptextra";  
  if ( globalEnergy == 2.36) globalFigBaseName = "2.36TeV_ptextra";  
  if ( globalEnergy == 7.0 ) globalFigBaseName = "7.0TeV_ptextra"; 

  globalLegendTextSize = 20;
  yLegendWidth = 0.04;
  xLegendWidth = .1 ;
  
  globalNorm = 2; 
  histoYMin  = 0.000001 ;
  histoYMax  = 0.1;
  histoXMin  = -0.5 ;
  
  int iMc;
  if ( globalEnergy == 0.9 ) iMc = 10 ;
  if ( globalEnergy == 2.36) iMc = 10 ;
  if ( globalEnergy == 7.0 ) iMc = 31 ;

  TString plotdir = "/user/rougny/Ferenc_Tracking_bis/CMSSW_3_3_6_patch3/src/UAmulti/ChPartAna/plots/systv13/";
  
  globalRatioType = 1;
  globalRatioBase = 0;

  ostringstream outstr("");

  outstr << "hyp" << 1 << "_niter" << 0 << "_cut" << 30 << "_DataType" << 0;  
  
  dataSetId.push_back(-1);
  dataSetFile.push_back(fileManager(3,iMc,globalEnergy,1,0,0,outstr.str(),plotdir));
  dataSetIsMc.push_back(0);
  dataSetHType.push_back(1);
  dataSetStyle.push_back(kOpenCircle);
  dataSetColor.push_back(kBlack);
  dataSetLegend.push_back("100 MeV");
  dataSetHisto.push_back("unfolding/nch_data_corrected");
  
  outstr.str("");
  outstr << "hyp" << 1 << "_niter" << 0 << "_cut" << 31 << "_DataType" << 0;  
  
  dataSetId.push_back(-1);
  dataSetFile.push_back(fileManager(3,iMc,globalEnergy,1,0,0,outstr.str(),plotdir));
  dataSetIsMc.push_back(0);
  dataSetHType.push_back(1);
  dataSetStyle.push_back(kOpenCircle);
  dataSetColor.push_back(kRed);
  dataSetLegend.push_back("150 MeV");
  dataSetHisto.push_back("unfolding/nch_data_corrected");
  
  outstr.str("");
  outstr << "hyp" << 1 << "_niter" << 0 << "_cut" << 32 << "_DataType" << 0;  
  
  dataSetId.push_back(-1);
  dataSetFile.push_back(fileManager(3,iMc,globalEnergy,1,0,0,outstr.str(),plotdir));
  dataSetIsMc.push_back(0);
  dataSetHType.push_back(1);
  dataSetStyle.push_back(kOpenCircle);
  dataSetColor.push_back(kBlue);
  dataSetLegend.push_back("200 MeV");
  dataSetHisto.push_back("unfolding/nch_data_corrected");
  
  outstr.str("");
  outstr << "hyp" << 1 << "_niter" << 0 << "_cut" << 33 << "_DataType" << 0;  
  
  dataSetId.push_back(-1);
  dataSetFile.push_back(fileManager(3,iMc,globalEnergy,1,0,0,outstr.str(),plotdir));
  dataSetIsMc.push_back(0);
  dataSetHType.push_back(1);
  dataSetStyle.push_back(kOpenCircle);
  dataSetColor.push_back(kMagenta);
  dataSetLegend.push_back("250 MeV");
  dataSetHisto.push_back("unfolding/nch_data_corrected");
  
  outstr.str("");
  outstr << "hyp" << 1 << "_niter" << 0 << "_cut" << 34 << "_DataType" << 0;  
  
  dataSetId.push_back(-1);
  dataSetFile.push_back(fileManager(3,iMc,globalEnergy,1,0,0,outstr.str(),plotdir));
  dataSetIsMc.push_back(0);
  dataSetHType.push_back(1);
  dataSetStyle.push_back(kOpenCircle);
  dataSetColor.push_back(kGreen);
  dataSetLegend.push_back("300 MeV");
  dataSetHisto.push_back("unfolding/nch_data_corrected");
  
  
  // ----- PLOT ----

  plot("none","AUTO",1,2);

}
