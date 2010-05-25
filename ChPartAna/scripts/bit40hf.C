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

void bit40hf(double energy = 0.9, int iSaveFig = 1, float npx = 500 , float npy = 500){

  globalEnergy = energy;
  gROOT->ProcessLine(".x cmsStyleRoot.C");

  plotReset();

  globalCanvasSizeX = npx;
  globalCanvasSizeY = npy;

  XaxisTitle = "n";
  YaxisTitle = "#frac{dN}{dn}";

  // xGlobalLabel = 0.6 ;

  if ( globalEnergy == 0.9 ) globalLabel =  "CMS Preliminary 0.9 TeV";
  if ( globalEnergy == 2.36) globalLabel =  "CMS Preliminary 2.36 TeV";
  if ( globalEnergy == 7.0 ) globalLabel =  "CMS Preliminary 7 TeV";

  globalSaveFig = iSaveFig;
  if ( globalEnergy == 0.9 ) globalFigBaseName = "0.9TeV_bit40hf";
  if ( globalEnergy == 2.36) globalFigBaseName = "2.36TeV_bit40hf";
  if ( globalEnergy == 7.0 ) globalFigBaseName = "7.0TeV_bit40hf";

  globalNorm = 0;
//  histoYMin  = 0.0 ;
//  histoYMax  = 0.1;
//  histoXMin  = -0.5 ;

  if ( globalEnergy == 0.9 ) histoXMax  = 60.5;
  if ( globalEnergy == 2.36) histoXMax  = 60.5;
  if ( globalEnergy == 7.0 ) histoXMax  = 60.5;

  ostringstream outstr("");
  outstr << "hyp" << 1 << "_niter" << 0 << "_cut" << 5 << "_DataType" << 0;

  TString plotdir = "../plots/systv9_bis/";

  int iMc;
  if ( globalEnergy == 0.9 ) iMc = 10 ; 
  if ( globalEnergy == 2.36) iMc = 10 ; 
  if ( globalEnergy == 7.0 ) iMc = 31 ; 


    dataSetId.push_back(-1);
    dataSetFile.push_back(fileManager(3,iMc,globalEnergy,1,0,0,outstr.str(),plotdir));
    dataSetIsMc.push_back(1);
    dataSetHType.push_back(1);
    dataSetStyle.push_back(1);
    dataSetColor.push_back(kBlack);
    dataSetLegend.push_back("Data");
    dataSetHisto.push_back("unfolding/nch_data_corrected");

    dataSetId.push_back(-1);
    dataSetFile.push_back(fileManager(3,iMc,globalEnergy,1,200,-1,outstr.str(),plotdir)); 
    dataSetIsMc.push_back(1);
    dataSetHType.push_back(1);
    dataSetStyle.push_back(1);
    dataSetColor.push_back(kBlue);
    dataSetLegend.push_back("L1+HF Syst-");
    dataSetHisto.push_back("unfolding/nch_data_corrected");

    dataSetId.push_back(-1);
    dataSetFile.push_back(fileManager(3,iMc,globalEnergy,1,200,1,outstr.str(),plotdir)); 
    dataSetIsMc.push_back(1);
    dataSetHType.push_back(1);
    dataSetStyle.push_back(1);
    dataSetColor.push_back(kRed);
    dataSetLegend.push_back("L1+HF Syst+");
    dataSetHisto.push_back("unfolding/nch_data_corrected");

    dataSetId.push_back(-1);
    dataSetFile.push_back(fileManager(3,iMc,globalEnergy,1,201,-1,outstr.str(),plotdir));
    dataSetIsMc.push_back(1);
    dataSetHType.push_back(1);
    dataSetStyle.push_back(1);
    dataSetColor.push_back(kMagenta);
    dataSetLegend.push_back("Bit40");
    dataSetHisto.push_back("unfolding/nch_data_corrected");




  // ----- PLOT ----

  plot("none","AUTO",0,0);


}
