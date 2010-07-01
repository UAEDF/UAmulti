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

void nchPlotRecoFinal(double energy = 0.9, int iBin = 5 , int iSaveFig = 1, float npx = 500 , float npy = 500){

  globalEnergy = energy;
  gROOT->ProcessLine(".x cmsStyleRoot.C");

  plotReset();

  globalCanvasSizeX = npx;
  globalCanvasSizeY = npy;

  XaxisTitle = "n";
  YaxisTitle = "N";

  xGlobalLabel = 0.5 ;
  if ( globalEnergy == 0.9 ) globalLabel =  "CMS Preliminary 0.9 TeV";
  if ( globalEnergy == 2.36) globalLabel =  "CMS Preliminary 2.36 TeV";
  if ( globalEnergy == 7.0 ) globalLabel =  "CMS Preliminary 7 TeV";

  globalSaveFig = iSaveFig;
  if ( iBin == 5 ) {
    if ( globalEnergy == 0.9 ) globalFigBaseName = "nch_unfolding_900GeV";
    if ( globalEnergy == 2.36) globalFigBaseName = "nch_unfolding_2360GeV";
    if ( globalEnergy == 7.0 ) globalFigBaseName = "nch_unfolding_7000GeV";
  }

  histoXMin  = -1.5 ;
  histoYMin  = 0.   ;
  if ( globalEnergy == 0.9 ) {
    histoXMax  = 80. ;
    histoYMax  = 11500.;
  }
  if ( globalEnergy == 0.9 ) {
    histoXMax  = 80. ;
    histoYMax  = 900.;
  }
  if ( globalEnergy == 7.0 ) {
    histoXMax  = 160. ;
    if ( iBin == 15 ) histoXMax  =  70;
    histoYMax  = 29000.;
    if ( iBin == 15 ) histoYMax  = 45000;
  }


  TString plotdir("");
  plotdir = "/user/rougny/Ferenc_Tracking_bis/CMSSW_3_3_6_patch3/src/UAmulti/ChPartAna/plots/systv10_17_3/";

  ostringstream outstr("");
  outstr << "hyp" << 1 << "_niter" << 0 << "_cut" << iBin << "_DataType" << 0;

  int iMc;
  if ( globalEnergy == 0.9 ) iMc = 10 ;
  if ( globalEnergy == 2.36) iMc = 10 ;
  if ( globalEnergy == 7.0 ) iMc = 31 ;


  // PLOT


    dataSetId.push_back(-1);
    dataSetFile.push_back(fileManager(3,iMc,globalEnergy,1,0,0,outstr.str(),plotdir));
    dataSetIsMc.push_back(0);
    dataSetHType.push_back(5);
    dataSetStyle.push_back(kOpenCircle);
    dataSetColor.push_back(kBlack);
    dataSetLegend.push_back("Hadrons");
    dataSetHisto.push_back("unfolding/gnch_data_corrected");


    dataSetId.push_back(-1);
    dataSetFile.push_back(fileManager(3,iMc,globalEnergy,1,0,0,outstr.str(),plotdir));
    dataSetIsMc.push_back(0);
    dataSetHType.push_back(1);
    dataSetStyle.push_back(kOpenSquare);
    dataSetColor.push_back(kRed);
    dataSetLegend.push_back("Tracks");
    dataSetHisto.push_back("unfolding/nch_data_INC_beforeSDsub");
    //dataSetHisto.push_back("unfolding/gnch_data_corrected");

    dataSetId.push_back(-1);
    dataSetFile.push_back(fileManager(3,iMc,globalEnergy,1,0,0,outstr.str(),plotdir));
    dataSetIsMc.push_back(0);
    dataSetHType.push_back(105);
    dataSetStyle.push_back(kOpenCircle);
    dataSetColor.push_back(kBlack);
    dataSetLegend.push_back("NONE");
    dataSetHisto.push_back("unfolding/gnch_data_corrected_syst");



  // ----- PLOT ----

  plot("none","AUTO",0,0);

}





