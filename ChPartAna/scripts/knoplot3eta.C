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

void knoplot3eta(int iBin = 5 , int iSaveFig = 0, float npx = 1000 , float npy = 1328){

  gROOT->ProcessLine(".x cmsStyleRoot.C");

  plotReset();
 
  globalCanvasSizeX = npx; 
  globalCanvasSizeY = npy;
  
  globalLegendTextSize = 40;
 
  XaxisTitle = "z=n/<n>";
  YaxisTitle = "#Psi(z)";

  xGlobalLabel = 0.6 ;
  globalLabel =  "CMS Preliminary";

  globalSaveFig = iSaveFig;
  if ( iBin ==  5 ) globalFigBaseName = "kno_3eta";  
  if ( iBin == 15 ) globalFigBaseName = "kno_3eta_pt500";


  globalNorm = 0; 
/*
  if ( iBin ==  5 )  histoYMin  = 0.0003 ;
  if ( iBin ==  7 )  histoYMin  = 0.000004 ;
  if ( iBin ==  9 )  histoYMin  = 0.00001 ;
  if ( iBin == 15 )  histoYMin  = 0.0001 ;
  if ( iBin == 17 )  histoYMin  = 0.00001 ;
  if ( iBin == 19 )  histoYMin  = 0.00001 ;

  histoYMax  = 1.3 ;
  histoXMin  = -0.5 ;
*/

  if ( iBin ==  5 ) {
    histoXMin  = -0.5 ;
    histoXMax  = 9.5  ;
    histoYMin  = 0.000005 ;
    histoYMax  = 50000. ; 
  }

/*
  if (iBin ==  5 ) histoXMax  = 6 ;
  if (iBin ==  7 ) histoXMax  = 9.;
  if (iBin ==  9 ) histoXMax  = 9. ;
  if (iBin > 10 ) histoYMax = 1.0; 
  if (iBin == 15 ) histoXMax  = 10.;
  if (iBin == 17 ) histoXMax  = 12 ;
  if (iBin == 19 ) histoXMax  = 12. ;
*/

/*
  if (iBin ==  5 ) LegendTitle = "        |#eta| < 2.4"; 
  if (iBin ==  7 ) LegendTitle = "        |#eta| < 1.5"; 
  if (iBin ==  9 ) LegendTitle = "        |#eta| < 0.5"; 
  if (iBin == 15 ) LegendTitle = "        |#eta| < 2.4  p_{t} > 0.5 GeV/c";
  if (iBin == 17 ) LegendTitle = "        |#eta| < 1.5  p_{t} > 0.5 GeV/c";
  if (iBin == 19 ) LegendTitle = "        |#eta| < 0.5  p_{t} > 0.5 GeV/c";
*/

  int iMc;
//  if ( globalEnergy == 0.9 ) iMc = 10 ;
//  if ( globalEnergy == 2.36) iMc = 10 ;
//  if ( globalEnergy == 7.0 ) iMc = 31 ;

    TString plotdir("");
    //plotdir = "../plots/systv10_undecies2/";
    //plotdir = "../plots/systv10_ter2/";
    //plotdir = "../plots/systv10_binning1v2/";

//  if ( globalEnergy == 0.9 ) plotdir = "../plots/finalv9/";
//  if ( globalEnergy == 2.36) plotdir = "../plots/finalv9/";
//  if ( globalEnergy == 7.0 ) plotdir = "../plots/systv10/";

  plotdir = "/user/rougny/Ferenc_Tracking_bis/CMSSW_3_3_6_patch3/src/UAmulti/ChPartAna/plots/systv10_17_3/";



  ostringstream outstr1("");
  ostringstream outstr2("");
  ostringstream outstr3("");
  outstr1 << "hyp" << 1 << "_niter" << 0 << "_cut" << iBin << "_DataType" << 0;  
  outstr2 << "hyp" << 1 << "_niter" << 0 << "_cut" << iBin+2 << "_DataType" << 0;  
  outstr3 << "hyp" << 1 << "_niter" << 0 << "_cut" << iBin+4 << "_DataType" << 0;  

  // Define plot: |eta|<2.4

    dataSetId.push_back(-1);
    dataSetFile.push_back(fileManager(3,10,0.9,1,0,0,outstr1.str(),plotdir));
    dataSetIsMc.push_back(0);
    dataSetHType.push_back(1);
    dataSetStyle.push_back(kOpenCircle);
    dataSetColor.push_back(kBlack);
    dataSetLegend.push_back("0.9 TeV");
    dataSetHisto.push_back("unfolding/kno_corrected");  
    dataSetFactor.push_back(10000);

    dataSetId.push_back(-1);
    dataSetFile.push_back(fileManager(3,10,0.9,1,0,0,outstr1.str(),plotdir));
    dataSetIsMc.push_back(0);
    dataSetHType.push_back(205);
    dataSetStyle.push_back(kOpenCircle);
    dataSetColor.push_back(kBlack);
    dataSetLegend.push_back("NONE");
    dataSetHisto.push_back("unfolding/gkno_corrected_syst");
    dataSetFactor.push_back(10000);

    BinKillStat.push_back( dataSetId.size()-2 );
    BinKillSyst.push_back( dataSetId.size()-1 );

    BinKillMax.push_back( dataSetId.size()-1 );
    BinKillMax.push_back( dataSetId.size()-2 );
    BinKillXMax.push_back( 6.5 );
    BinKillXMax.push_back( 6.5 );

    dataSetId.push_back(-1);
    dataSetFile.push_back(fileManager(3,10,2.36,1,0,0,outstr1.str(),plotdir));
    dataSetIsMc.push_back(0);
    dataSetHType.push_back(1);
    dataSetStyle.push_back(kOpenSquare);
    dataSetColor.push_back(kBlue);
    dataSetLegend.push_back("2.36 TeV");
    dataSetHisto.push_back("unfolding/kno_corrected");
    dataSetFactor.push_back(10000);

    dataSetId.push_back(-1);
    dataSetFile.push_back(fileManager(3,10,2.36,1,0,0,outstr1.str(),plotdir));
    dataSetIsMc.push_back(0);
    dataSetHType.push_back(205);
    dataSetStyle.push_back(kOpenSquare);
    dataSetColor.push_back(kBlue);
    dataSetLegend.push_back("NONE");
    dataSetHisto.push_back("unfolding/gkno_corrected_syst");
    dataSetFactor.push_back(10000);

    BinKillStat.push_back( dataSetId.size()-2 );
    BinKillSyst.push_back( dataSetId.size()-1 );

    BinKillMax.push_back( dataSetId.size()-1 );
    BinKillMax.push_back( dataSetId.size()-2 );
    BinKillXMax.push_back( 5.3 );
    BinKillXMax.push_back( 5.3 );


    dataSetId.push_back(-1);
    dataSetFile.push_back(fileManager(3,31,7.0,1,0,0,outstr1.str(),plotdir));
    dataSetIsMc.push_back(0);
    dataSetHType.push_back(1);
    dataSetStyle.push_back(kOpenTriangleUp);
    dataSetColor.push_back(kRed);
    dataSetLegend.push_back("7.0 TeV");
    dataSetHisto.push_back("unfolding/kno_corrected");
    dataSetFactor.push_back(10000);

    dataSetId.push_back(-1);
    dataSetFile.push_back(fileManager(3,31,7.0,1,0,0,outstr1.str(),plotdir));
    dataSetIsMc.push_back(0);
    dataSetHType.push_back(205);
    dataSetStyle.push_back(kOpenTriangleUp);
    dataSetColor.push_back(kRed);
    dataSetLegend.push_back("NONE");
    dataSetHisto.push_back("unfolding/gkno_corrected_syst");
    dataSetFactor.push_back(10000);

    BinKillStat.push_back( dataSetId.size()-2 );
    BinKillSyst.push_back( dataSetId.size()-1 );

/*
    BinKillMax.push_back( dataSetId.size()-1 );
    BinKillMax.push_back( dataSetId.size()-2 );
    BinKillXMax.push_back( 5.3 );
    BinKillXMax.push_back( 5.3 );
*/

  // Define plot: |eta|<1.5

    dataSetId.push_back(-1);
    dataSetFile.push_back(fileManager(3,10,0.9,1,0,0,outstr2.str(),plotdir));
    dataSetIsMc.push_back(0);
    dataSetHType.push_back(1);
    dataSetStyle.push_back(kOpenCircle);
    dataSetColor.push_back(kBlack);
    dataSetLegend.push_back("NONE");
    dataSetHisto.push_back("unfolding/kno_corrected");  
    dataSetFactor.push_back(100);

    dataSetId.push_back(-1);
    dataSetFile.push_back(fileManager(3,10,0.9,1,0,0,outstr2.str(),plotdir));
    dataSetIsMc.push_back(0);
    dataSetHType.push_back(205);
    dataSetStyle.push_back(kOpenCircle);
    dataSetColor.push_back(kBlack);
    dataSetLegend.push_back("NONE");
    dataSetHisto.push_back("unfolding/gkno_corrected_syst");
    dataSetFactor.push_back(100);

  BinKillStat.push_back( dataSetId.size()-2 );
  BinKillSyst.push_back( dataSetId.size()-1 );

    dataSetId.push_back(-1);
    dataSetFile.push_back(fileManager(3,10,2.36,1,0,0,outstr2.str(),plotdir));
    dataSetIsMc.push_back(0);
    dataSetHType.push_back(1);
    dataSetStyle.push_back(kOpenSquare);
    dataSetColor.push_back(kBlue);
    dataSetLegend.push_back("NONE");
    dataSetHisto.push_back("unfolding/kno_corrected");
    dataSetFactor.push_back(100);

    dataSetId.push_back(-1);
    dataSetFile.push_back(fileManager(3,10,2.36,1,0,0,outstr2.str(),plotdir));
    dataSetIsMc.push_back(0);
    dataSetHType.push_back(205);
    dataSetStyle.push_back(kOpenSquare);
    dataSetColor.push_back(kBlue);
    dataSetLegend.push_back("NONE");
    dataSetHisto.push_back("unfolding/gkno_corrected_syst");
    dataSetFactor.push_back(100);

  BinKillStat.push_back( dataSetId.size()-2 );
  BinKillSyst.push_back( dataSetId.size()-1 );

    dataSetId.push_back(-1);
    dataSetFile.push_back(fileManager(3,31,7.0,1,0,0,outstr2.str(),plotdir));
    dataSetIsMc.push_back(0);
    dataSetHType.push_back(1);
    dataSetStyle.push_back(kOpenTriangleUp);
    dataSetColor.push_back(kRed);
    dataSetLegend.push_back("NONE");
    dataSetHisto.push_back("unfolding/kno_corrected");
    dataSetFactor.push_back(100);

    dataSetId.push_back(-1);
    dataSetFile.push_back(fileManager(3,31,7.0,1,0,0,outstr2.str(),plotdir));
    dataSetIsMc.push_back(0);
    dataSetHType.push_back(205);
    dataSetStyle.push_back(kOpenTriangleUp);
    dataSetColor.push_back(kRed);
    dataSetLegend.push_back("NONE");
    dataSetHisto.push_back("unfolding/gkno_corrected_syst");
    dataSetFactor.push_back(100);

  BinKillStat.push_back( dataSetId.size()-2 );
  BinKillSyst.push_back( dataSetId.size()-1 );


    BinKillMax.push_back( dataSetId.size()-1 );
    BinKillMax.push_back( dataSetId.size()-2 );
    BinKillXMax.push_back( 8.3 );
    BinKillXMax.push_back( 8.3 );




  // Define plot: |eta|<2.4

    dataSetId.push_back(-1);
    dataSetFile.push_back(fileManager(3,10,0.9,1,0,0,outstr3.str(),plotdir));
    dataSetIsMc.push_back(0);
    dataSetHType.push_back(1);
    dataSetStyle.push_back(kOpenCircle);
    dataSetColor.push_back(kBlack);
    dataSetLegend.push_back("NONE");
    dataSetHisto.push_back("unfolding/kno_corrected");  
    dataSetFactor.push_back(1);

    dataSetId.push_back(-1);
    dataSetFile.push_back(fileManager(3,10,0.9,1,0,0,outstr3.str(),plotdir));
    dataSetIsMc.push_back(0);
    dataSetHType.push_back(205);
    dataSetStyle.push_back(kOpenCircle);
    dataSetColor.push_back(kBlack);
    dataSetLegend.push_back("NONE");
    dataSetHisto.push_back("unfolding/gkno_corrected_syst");
    dataSetFactor.push_back(1);

  BinKillStat.push_back( dataSetId.size()-2 );
  BinKillSyst.push_back( dataSetId.size()-1 );

    dataSetId.push_back(-1);
    dataSetFile.push_back(fileManager(3,10,2.36,1,0,0,outstr3.str(),plotdir));
    dataSetIsMc.push_back(0);
    dataSetHType.push_back(1);
    dataSetStyle.push_back(kOpenSquare);
    dataSetColor.push_back(kBlue);
    dataSetLegend.push_back("NONE");
    dataSetHisto.push_back("unfolding/kno_corrected");
    dataSetFactor.push_back(1);

    dataSetId.push_back(-1);
    dataSetFile.push_back(fileManager(3,10,2.36,1,0,0,outstr3.str(),plotdir));
    dataSetIsMc.push_back(0);
    dataSetHType.push_back(205);
    dataSetStyle.push_back(kOpenSquare);
    dataSetColor.push_back(kBlue);
    dataSetLegend.push_back("NONE");
    dataSetHisto.push_back("unfolding/gkno_corrected_syst");
    dataSetFactor.push_back(1);

  BinKillStat.push_back( dataSetId.size()-2 );
  BinKillSyst.push_back( dataSetId.size()-1 );

    dataSetId.push_back(-1);
    dataSetFile.push_back(fileManager(3,31,7.0,1,0,0,outstr3.str(),plotdir));
    dataSetIsMc.push_back(0);
    dataSetHType.push_back(1);
    dataSetStyle.push_back(kOpenTriangleUp);
    dataSetColor.push_back(kRed);
    dataSetLegend.push_back("NONE");
    dataSetHisto.push_back("unfolding/kno_corrected");
    dataSetFactor.push_back(1);

    dataSetId.push_back(-1);
    dataSetFile.push_back(fileManager(3,31,7.0,1,0,0,outstr3.str(),plotdir));
    dataSetIsMc.push_back(0);
    dataSetHType.push_back(205);
    dataSetStyle.push_back(kOpenTriangleUp);
    dataSetColor.push_back(kRed);
    dataSetLegend.push_back("NONE");
    dataSetHisto.push_back("unfolding/gkno_corrected_syst");
    dataSetFactor.push_back(1);

  BinKillStat.push_back( dataSetId.size()-2 );
  BinKillSyst.push_back( dataSetId.size()-1 );


    BinKillMax.push_back( dataSetId.size()-1 );
    BinKillMax.push_back( dataSetId.size()-2 );
    BinKillXMax.push_back( 9.5 );
    BinKillXMax.push_back( 9.5 );




  // Curve legends

  Float_t xt1 , yt1  ;
  Float_t xt3 , yt3  ;
  Float_t xt5 , yt5  ;

  if ( iBin == 5 ) {
     xt1 = .35; yt1 = .82;
     xt3 = .25; yt3 = .70;
     xt5 = .20; yt5 = .55;
  }

  globalTextSize = 0.03;

  textXPos.push_back(xt1)  ; textYPos.push_back(yt1) ;
  textXPos.push_back(xt3)  ; textYPos.push_back(yt3) ;
  textXPos.push_back(xt5)  ; textYPos.push_back(yt5) ;

  textText.push_back("|#eta| < 2.4 (x10000)");
  textText.push_back("|#eta| < 1.5 (x100)");
  textText.push_back("|#eta| < 0.5 (x1)");

   
  // ----- PLOT ----

/*
   yLegendWidth = 0.04;
*/
  xLegendWidth = .1 ;


  plot("none","AUTO",1,2);

}
