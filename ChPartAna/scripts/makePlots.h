#ifndef __makePlot_H__
#define __makePlot_H__

#include <vector>
using namespace std;
#include <TString.h>

 // File level
 int           globalFileType = 1; 
 TString       ptcutstr = "NONE" ;

 // Energy + CMS Label
 double        globalEnergy  = 0.9 ; 
 TString       globalLabel   ("CMS 0.9 TeV");
 float         xGlobalLabel  = 0.60 ;
 float         yGlobalLabel  = 0.90 ;

 // Normalisation 
 // 0: No
 // 1: to #events in first histo
 // 2: to Integral of each histo
 int           globalNorm    = 2 ;

 // TLegend position +  Title 

 TString  LegendTitle ("NONE"); 
 float    xLegendMin[] = { 0.57 , 0.35 };
 float    xLegendWidth = 0.35;
 float    yLegendMax[] = { 0.80 , 0.50 };
 float    yLegendWidth = 0.03;

 // Save Figs 
 bool          globalSaveFig = 0 ; 
 TString       globalFigDir  ("../figs/");

 // Dataset Settings
  
 vector<int>     dataSetId     (1,0);
 vector<bool>    dataSetIsMc   (1,0);
 vector<int>     dataSetStyle  (1,20);
 vector<int>     dataSetColor  (1,2);
 vector<TString> dataSetLegend (1,"Undef");

#endif

