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

void knoplot(int iBin = 5 , int iSaveFig = 1, bool plot2360 = false , float npx = 700 , float npy = 700){

  gROOT->ProcessLine(".x cmsStyleRoot.C");

  plotReset();
 
  globalCanvasSizeX = npx; 
  globalCanvasSizeY = npy;
  
  globalLegendTextSize = 25;
 
  XaxisTitle = "z=n/<n>";
  YaxisTitle = "#Psi(z)";
  
  yLegendWidth = 0.06;
  xLegendWidth = .1 ;

  xGlobalLabel = 0.50 ;
  yGlobalLabel = 0.87 ;
  globalLabelSize = 0.06;
  globalLabel =  "CMS";

  globalSaveFig = iSaveFig;
  if ( iBin ==  5 ) globalFigBaseName = "kno_eta25";  
  if ( iBin ==  7 ) globalFigBaseName = "kno_eta15";  
  if ( iBin ==  9 ) globalFigBaseName = "kno_eta05";  
  if ( iBin == 15 ) globalFigBaseName = "kno_eta25_pt500";
  if ( iBin == 17 ) globalFigBaseName = "kno_eta15_pt500";
  if ( iBin == 19 ) globalFigBaseName = "kno_eta05_pt500";

  if(!plot2360)
    globalFigBaseName+="_no2360";

  globalNorm = 0; 
  if ( iBin ==  5 )  histoYMin  = 0.0003 ;
  if ( iBin ==  7 )  histoYMin  = 0.000004 ;
  if ( iBin ==  9 )  histoYMin  = 0.00001 ;
  if ( iBin == 15 )  histoYMin  = 0.0001 ;
  if ( iBin == 17 )  histoYMin  = 0.00001 ;
  if ( iBin == 19 )  histoYMin  = 0.00001 ;

  histoYMax  = 1.3 ;
  if ( iBin ==  5 )  histoYMax  = 3 ;
  histoXMin  = -0.5 ;

  if (iBin ==  5 ) histoXMax  = 6 ;
  if (iBin ==  7 ) histoXMax  = 9.;
  if (iBin ==  9 ) histoXMax  = 9. ;
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

  globalTextSize = 0.04;
  textXPos.push_back(0.89);
  textYPos.push_back(0.90) ;
  if(iBin == 5)
    textText.push_back("a)");
  if(iBin == 9)
    textText.push_back("b)");
  
  int iMc;
//  if ( globalEnergy == 0.9 ) iMc = 10 ;
//  if ( globalEnergy == 2.36) iMc = 10 ;
//  if ( globalEnergy == 7.0 ) iMc = 31 ;

    TString plotdir("");
    plotdir = "../plots/systv10_niter100_2/";
    //plotdir = "../plots/systv10_ter2/";
    //plotdir = "../plots/systv10_binning1v2/";

//  if ( globalEnergy == 0.9 ) plotdir = "../plots/finalv9/";
//  if ( globalEnergy == 2.36) plotdir = "../plots/finalv9/";
//  if ( globalEnergy == 7.0 ) plotdir = "../plots/systv10/";

  plotdir = "/user/rougny/Ferenc_Tracking_bis/CMSSW_3_3_6_patch3/src/UAmulti/ChPartAna/plots/systv10_17_3/";



  ostringstream outstr("");
  outstr << "hyp" << 1 << "_niter" << 0 << "_cut" << iBin << "_DataType" << 0;  

  // Define plot

    dataSetId.push_back(-1);
    dataSetFile.push_back(fileManager(3,10,0.9,1,0,0,outstr.str(),plotdir));
    dataSetIsMc.push_back(0);
    dataSetHType.push_back(1);
    dataSetStyle.push_back(kOpenCircle);
    dataSetColor.push_back(kBlack);
    dataSetLegend.push_back("CMS 0.9 TeV");
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
    
    BinKillStat.push_back( 0 );
    BinKillSyst.push_back( 1 );


    if(plot2360==1){

      dataSetId.push_back(-1);
      dataSetFile.push_back(fileManager(3,10,2.36,1,0,0,outstr.str(),plotdir));
      dataSetIsMc.push_back(0);
      dataSetHType.push_back(1);
      dataSetStyle.push_back(kOpenSquare);
      dataSetColor.push_back(kBlue);
      dataSetLegend.push_back("CMS 2.36 TeV");
      dataSetHisto.push_back("unfolding/kno_corrected");

      dataSetId.push_back(-1);
      dataSetFile.push_back(fileManager(3,10,2.36,1,0,0,outstr.str(),plotdir));
      dataSetIsMc.push_back(0);
      dataSetHType.push_back(205);
      dataSetStyle.push_back(kOpenSquare);
      dataSetColor.push_back(kBlue);
      dataSetLegend.push_back("NONE");
      dataSetHisto.push_back("unfolding/gkno_corrected_syst");
    }    


    dataSetId.push_back(-1);
    dataSetFile.push_back(fileManager(3,31,7.0,1,0,0,outstr.str(),plotdir));
    dataSetIsMc.push_back(0);
    dataSetHType.push_back(1);
    dataSetStyle.push_back(kOpenTriangleUp);
    dataSetColor.push_back(kRed);
    dataSetLegend.push_back("CMS 7.0 TeV");
    dataSetHisto.push_back("unfolding/kno_corrected");


    dataSetId.push_back(-1);
    dataSetFile.push_back(fileManager(3,31,7.0,1,0,0,outstr.str(),plotdir));
    dataSetIsMc.push_back(0);
    dataSetHType.push_back(205);
    dataSetStyle.push_back(kOpenTriangleUp);
    dataSetColor.push_back(kRed);
    dataSetLegend.push_back("NONE");
    dataSetHisto.push_back("unfolding/gkno_corrected_syst");
    
    BinKillStat.push_back( 2 );
    BinKillSyst.push_back( 3 );
    
    if(plot2360){
      BinKillStat.push_back( 4 );
      BinKillSyst.push_back( 5 );
    }
   
  // ----- PLOT ----

  plot("none","AUTO",1,2);

}
