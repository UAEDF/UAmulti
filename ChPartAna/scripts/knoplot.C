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

void knoplot(int iBin = 5 , int iSaveFig = 1, float npx = 1000 , float npy = 600){

  gROOT->ProcessLine(".x cmsStyleRoot.C");

  plotReset();
 
  globalCanvasSizeX = npx; 
  globalCanvasSizeY = npy;
 
  XaxisTitle = "z=n/<n>";
  YaxisTitle = "#Psi(z)";

  xGlobalLabel = 0.6 ;
  globalLabel =  "CMS Preliminary";

  globalSaveFig = iSaveFig;
  if ( iBin ==  5 ) globalFigBaseName = "kno_eta25";  
  if ( iBin ==  7 ) globalFigBaseName = "kno_eta15";  
  if ( iBin ==  9 ) globalFigBaseName = "kno_eta05";  
  if ( iBin == 15 ) globalFigBaseName = "kno_eta25_pt500";
  if ( iBin == 17 ) globalFigBaseName = "kno_eta15_pt500";
  if ( iBin == 19 ) globalFigBaseName = "kno_eta05_pt500";


  globalNorm = 2; 
  if ( iBin ==  5 )  histoYMin  = 0.000001 ;
  if ( iBin ==  7 )  histoYMin  = 0.000000001 ;
  if ( iBin ==  9 )  histoYMin  = 0.000000001 ;
  if ( iBin == 15 )  histoYMin  = 0.000001 ;
  if ( iBin == 17 )  histoYMin  = 0.000000001 ;
  if ( iBin == 19 )  histoYMin  = 0.000000001 ;

  histoYMax  = 0.3 ;
  histoXMin  = -0.5 ;

  if (iBin ==  5 ) histoXMax  = 6. ;
  if (iBin ==  7 ) histoXMax  = 10.;
  if (iBin ==  9 ) histoXMax  = 10. ;
  if (iBin > 10 ) histoYMax = 1.0; 
  if (iBin == 15 ) histoXMax  = 10.;
  if (iBin == 17 ) histoXMax  = 12 ;
  if (iBin == 19 ) histoXMax  = 12. ;


  if (iBin ==  5 ) LegendTitle = "        |#eta| < 2.4"; 
  if (iBin ==  7 ) LegendTitle = "        |#eta| < 1.5"; 
  if (iBin ==  9 ) LegendTitle = "        |#eta| < 0.5"; 
  if (iBin == 15 ) LegendTitle = "        |#eta| < 2.4  p_{t} > 0.5 GeV/c";
  if (iBin == 17 ) LegendTitle = "        |#eta| < 1.5  p_{t} > 0.5 GeV/c";
  if (iBin == 19 ) LegendTitle = "        |#eta| < 0.5  p_{t} > 0.5 GeV/c";


  int iMc;
//  if ( globalEnergy == 0.9 ) iMc = 10 ;
//  if ( globalEnergy == 2.36) iMc = 10 ;
//  if ( globalEnergy == 7.0 ) iMc = 31 ;

    TString plotdir("");
    plotdir = "../plots/systv9_bis/";

//  if ( globalEnergy == 0.9 ) plotdir = "../plots/finalv9/";
//  if ( globalEnergy == 2.36) plotdir = "../plots/finalv9/";
//  if ( globalEnergy == 7.0 ) plotdir = "../plots/systv10/";

  ostringstream outstr("");
  outstr << "hyp" << 1 << "_niter" << 0 << "_cut" << iBin << "_DataType" << 0;  

  // Define plot

    dataSetId.push_back(-1);
    dataSetFile.push_back(fileManager(3,10,0.9,1,0,0,outstr.str(),plotdir));
    dataSetIsMc.push_back(0);
    dataSetHType.push_back(1);
    dataSetStyle.push_back(kOpenCircle);
    dataSetColor.push_back(kBlack);
    dataSetLegend.push_back("Data 0.9 TeV");
    //dataSetHisto.push_back("unfolding/nch_data_corrected");  
    dataSetHisto.push_back("unfolding/kno_corrected");  


    dataSetId.push_back(-1);
    dataSetFile.push_back(fileManager(3,10,0.9,1,0,0,outstr.str(),plotdir));
    dataSetIsMc.push_back(0);
    dataSetHType.push_back(205);
    dataSetStyle.push_back(kOpenCircle);
    dataSetColor.push_back(kBlack);
    dataSetLegend.push_back("NONE");
    //dataSetHisto.push_back("unfolding/gnch_data_corrected_syst");
    dataSetHisto.push_back("unfolding/gkno_corrected_syst");


    dataSetId.push_back(-1);
    dataSetFile.push_back(fileManager(3,10,2.36,1,0,0,outstr.str(),plotdir));
    dataSetIsMc.push_back(0);
    dataSetHType.push_back(1);
    dataSetStyle.push_back(kOpenSquare);
    dataSetColor.push_back(kBlue);
    dataSetLegend.push_back("Data 2.36 TeV");
    dataSetHisto.push_back("unfolding/kno_corrected");


    dataSetId.push_back(-1);
    dataSetFile.push_back(fileManager(3,10,2.36,1,0,0,outstr.str(),plotdir));
    dataSetIsMc.push_back(0);
    dataSetHType.push_back(205);
    dataSetStyle.push_back(kOpenSquare);
    dataSetColor.push_back(kBlue);
    dataSetLegend.push_back("NONE");
    dataSetHisto.push_back("unfolding/gkno_corrected_syst");

    plotdir = "../plots/systv10/";

    dataSetId.push_back(-1);
    dataSetFile.push_back(fileManager(3,31,7.0,1,0,0,outstr.str(),plotdir));
    dataSetIsMc.push_back(0);
    dataSetHType.push_back(1);
    dataSetStyle.push_back(kOpenTriangleUp);
    dataSetColor.push_back(kRed);
    dataSetLegend.push_back("Data 7 TeV");
    dataSetHisto.push_back("unfolding/kno_corrected");


    dataSetId.push_back(-1);
    dataSetFile.push_back(fileManager(3,31,7.0,1,0,0,outstr.str(),plotdir));
    dataSetIsMc.push_back(0);
    dataSetHType.push_back(205);
    dataSetStyle.push_back(kOpenTriangleUp);
    dataSetColor.push_back(kRed);
    dataSetLegend.push_back("NONE");
    dataSetHisto.push_back("unfolding/gkno_corrected_syst");

   
  // ----- PLOT ----

  plot("none","AUTO",1,2);

}