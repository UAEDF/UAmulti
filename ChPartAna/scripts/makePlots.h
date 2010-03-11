#ifndef __makePlot_H__
#define __makePlot_H__

#include <vector>
using namespace std;
#include <TString.h>

 // File level
 int           globalFileType = 1; 
 TString       ptcutstr = "NONE" ;
 TString       globalDirPlot = "../plots.romain/";
// TString       globalDirPlot = "../plots.xavier/";
  
 // Energy + CMS Label
 TString       globalLabel   ("CMS");
 float         xGlobalLabel  = 0.60 ;
 float         yGlobalLabel  = 0.90 ;

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

 // TLegend position +  Title 

 TString  LegendTitle ("NONE"); 
 float    xLegendMin[] = { 0.57 , 0.35 , 0.20 , 0.70 };
 float    xLegendWidth = 0.35;
 float    yLegendMax[] = { 0.80 , 0.40 , 0.40 , 0.80 };
 float    yLegendWidth = 0.03;
 TString  ExtLegTitle ("NONE");

 // Save Figs 
 bool          globalWait    = 1 ;
 bool          globalSaveFig = 0 ; 
 TString       globalFigDir  ("../figs/");
 TString       globalFigBaseName  ("NONE");

 // Dataset Settings

 float           histoXMin = -99999.;   
 float           histoXMax = -99999.;   

 vector<int>     dataSetId     (1,0);
 vector<bool>    dataSetIsMc   (1,0);
 vector<int>     dataSetStyle  (1,20);
 vector<int>     dataSetColor  (1,2);
 vector<float>   dataSetFactor (1,1);
 vector<TString> dataSetLegend (1,"Undef");
 vector<TString> dataSetFile   (1,"Undef"); 
 vector<TString> dataSetDir    (1,"Undef"); 
 vector<TString> dataSetHisto  (1,"Undef"); 

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

  dataSetId.clear();     
  dataSetIsMc.clear();   
  dataSetStyle.clear();  
  dataSetColor.clear();  
  dataSetFactor.clear();  
  dataSetLegend.clear(); 
  dataSetFile.clear();
  dataSetDir.clear();    
  dataSetHisto.clear();    

}

#endif

