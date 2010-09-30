#ifndef __makePlot_H__
#define __makePlot_H__

#include <vector>
using namespace std;
#include <TString.h>
#include "../plugins/TPlotter.h"
#include "../plugins/TPlotterBase.h"

 // TCanvas size 
 float         globalCanvasSizeX = 500 ;
 float         globalCanvasSizeY = 500 ;

 // File level
 int           globalFileType = 1; 
 TString       ptcutstr = "NONE" ;
 TString       globalDirPlot = "../plots.romain/";
// TString       globalDirPlot = "../plots.xavier/";
  
 // Energy + CMS Label
 TString       globalLabel      ("CMS");
 double        globalLabelSize = 0.04;
 float         xGlobalLabel    = 0.45 ;
 float         yGlobalLabel    = 0.90 ;

 // hiso type
 // 1 = TH1F
 // 2 = TH2F ( Only 1 histo or 2 side by side ! ) 

 int           globalHistoType = 1 ; 
 TString       global2DplotOpt = "colz" ;

 // Normalisation 
 // 0: No
 // 1: to #events in first histo
 // 2: to Integral of each histo
 int           globalNorm    = 0 ;

 // MC Smoothing option
 bool          globalSmoothMC  =  0 ;

 // Plot Ratio 
 int           globalRatioType =  0;
 int           globalRatioBase = -1;
 int           globalRatioSyst = -1;
 bool          globalRatioErrorBand = 1;
 
 // Pad Margins
 double        leftMargin   = -1;
 double        rightMargin  = -1;
 double        topMargin    = -1;
 double        bottomMargin = -1;

 // Bin Killer

 vector<int>    BinKillStat (1,-1);
 vector<int>    BinKillSyst (1,-1);
 vector<int>    BinKillMax  (1,-1);
 vector<double> BinKillXMax (1,-99999.);


 // histo limit 
 float           histoXMin = -99999.;   
 float           histoXMax = -99999.;   
 float           histoYMin = -99999.;   
 float           histoYMax = -99999.;   
 
 // ratio Y limit 
 float           ratioYMin = 0.;   
 float           ratioYMax = 4.;   

 // Axis Title
 TString         XaxisTitle = "NONE";
 TString         YaxisTitle = "NONE";
 double          XaxisTitleOffset = 0.70;
 double          YaxisTitleOffset = 1.0 ;
 double          XaxisTitleSize   = 0.06;
 double          YaxisTitleSize   = 0.06;

 double          globalAxisTitleSize = 0.06;

 // TLegend position +  Title 

 int       globalLegendTextSize = 20;
 TString  LegendTitle ("NONE"); 
 float    xLegendMin[] = { 0.57 , 0.35 , 0.20 , 0.70 , 0.70 , 0.75 , 0.45 , 0.80 , 0.20 , 0.55 , 0.60};
 float    xLegendWidth = 0.35;
 float    yLegendMax[] = { 0.80 , 0.40 , 0.40 , 0.85 , 0.90 , 0.90 , 0.87 , 0.87 , 0.90 , 0.40 , 0.93};

 float    standardColor[]  = {kBlack , kRed , kBlue , kMagenta , kOrange , kGreen};
 float    standardMarker[] = {kOpenCircle , kOpenSquare , kOpenTriangleUp , kOpenStar};
 float    standardStyle[]  = {1 , 2 , 3 , 4 , 5, 6 , 7};


 float    yLegendWidth = 0.03;
 TString  ExtLegTitle ("NONE");

 // Save Figs 
 bool          globalWait    = 1 ;
 bool          globalSaveFig = 0 ; 
 TString       globalFigDir  ("../figs/");
 TString       globalFigBaseName  ("NONE");
 
 
 TPlotterBase plotterbase;
 TPlotter     plotter;

 // Dataset Settings
 vector<int>      dataSetId       (1,0);
 vector<bool>     dataSetIsMc     (1,0);
 vector<int>      dataSetStyle    (1,20);
 vector<int>      dataSetHType    (1,1);
 vector<int>      dataSetColor    (1,2);
 vector<float>    dataSetFactor   (1,1);
 vector<float>    dataSetOffset   (1,0);
 vector<TString>  dataSetLegend   (1,"Undef");
 vector<TString>  dataSetFile     (1,"Undef"); 
 vector<TString>  dataSetDir      (1,"Undef"); 
 vector<TString>  dataSetHisto    (1,"Undef"); 
 vector<int>      dataSetNSmooth  (20,50); 
 vector<TPlotter> dataSetTPlotter (20,TPlotter()); 

 // text  

 float globalTextSize = 0.02;
 vector<float>   textXPos (1,0); 
 vector<float>   textYPos (1,0); 
 vector<TString> textText (1,"Undef"); 

void plotReset()
{

  globalFileType = 1;
  ptcutstr = "NONE" ;

 // Energy + CMS Label
 // globalLabel   = "CMS" ;
 // xGlobalLabel  = 0.60 ;
 // yGlobalLabel  = 0.90 ;

 // hiso type
 // 1 = TH1F
 // 2 = TH2F ( Only 1 histo or 2 side by side ! ) 

  globalHistoType = 1 ;
  global2DplotOpt = "colz" ;

 // Normalisation 
 // 0: No
 // 1: to #events in first histo
 // 2: to Integral of each histo
  globalNorm    = 0 ;

 // Ratio 
// globalRatioBase = -1;

 BinKillStat.clear() ;
 BinKillSyst.clear() ;
 BinKillMax.clear() ;
 BinKillXMax.clear() ;


 // TLegend position +  Title 
 
  LegendTitle = "NONE";
  ExtLegTitle = "NONE";

 // Save Figs 
 // globalWait    = 1 ;
 // globalSaveFig = 0 ;
 // globalFigDir  = "../figs/" ;

 // Dataset Settings

  histoXMin = -99999.;
  histoXMax = -99999.;
  histoYMin = -99999.;
  histoYMax = -99999.;

  XaxisTitle       = "NONE";
  YaxisTitle       = "NONE";
  XaxisTitleOffset = 0.7 ; 
  YaxisTitleOffset = 1.0 ;
  XaxisTitleSize   = 0.06;
  YaxisTitleSize   = 0.06;
  
  dataSetId.clear();     
  dataSetIsMc.clear();   
  dataSetStyle.clear();  
  dataSetHType.clear();  
  dataSetColor.clear();  
  dataSetFactor.clear();  
  dataSetOffset.clear();  
  dataSetLegend.clear(); 
  dataSetFile.clear();
  dataSetDir.clear();    
  dataSetHisto.clear();  
  //dataSetNSmooth.clear();   
  //dataSetTPlotter.clear();
  dataSetTPlotter.assign(20,TPlotter()); 
  // 

 globalTextSize = 0.02;
 textXPos.clear();  
 textYPos.clear(); 
 textText.clear();


}

#endif

