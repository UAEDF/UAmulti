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

void nchmodels(int iBin = 5 , int iSaveFig = 1,
               float npx = 900  , float npy = 900 , bool tracklets = false  ){

  //globalEnergy = 0.9  ;
  gROOT->ProcessLine(".x cmsStyleRoot.C");
  
  plotterbase.g_smoothDrawing = 1;
  plotterbase.g_doSmoothing = 1;
  if(iBin==5)  plotterbase.g_nSmooth = 50;
  if(iBin==15) plotterbase.g_nSmooth = 100;

  plotReset();

  globalCanvasSizeX = npx;
  globalCanvasSizeY = npy;

  XaxisTitle = "n";
  YaxisTitle = "P_{n}";

  xGlobalLabel = 0.6 ;
  globalLabel =  "CMS"; 

  globalSaveFig = iSaveFig;
  if ( iBin == 5  ) globalFigBaseName = "nch_models_pt0";
  if ( iBin == 15 ) globalFigBaseName = "nch_models_pt500"; 

  globalSmoothMC = 1 ; 

  globalNorm = 2;
  if (iBin == 5 ) {
    histoYMin  = 0.000001 ;
    histoYMax  = 5000.;
    histoXMin  = 0. ;
    histoXMax  = 185.0;
  }
  if (iBin == 15 ) {
    histoYMin  = 0.000001 ;
    histoYMax  = 5000.;
    histoXMin  = 0. ;
    histoXMax  = 110.5;
  }

  TString plotdir("");
  //plotdir = "../plots//";
  plotdir = "/user/rougny/Ferenc_Tracking_bis/CMSSW_3_3_6_patch3/src/UAmulti/ChPartAna/plots/systv10_17_3/";



  ostringstream outstr("");
  outstr << "hyp" << 1 << "_niter" << 0 << "_cut" << iBin   << "_DataType" << 0;

  ostringstream mchisto("");
  mchisto << "nch_gen_NSD_noSel_cut" << iBin;

  // Points and Colors for Experiments

  int kCMSPoint = kOpenCircle ;
  int kCMSColor = kBlack;

  int kTrackletsPoint = kOpenCircle ;
  int kTrackletsColor = kRed;

  int kD6TLineStyle = 2 ;
  int kD6TLineColor = kRed   ;

  int kPY8LineStyle = 1 ;
  int kPY8LineColor = kMagenta ;

  int kPHOLineStyle = 7 ;
  int kPHOLineColor = kBlue ;

  // Define plots 
  
  // ... Data

  dataSetId.push_back(-1);
  dataSetFile.push_back(fileManager(3,31,7.,1,0,0,outstr.str(),plotdir));
  dataSetIsMc.push_back(0);
  dataSetHType.push_back(5);
  dataSetStyle.push_back(kCMSPoint);
  dataSetColor.push_back(kCMSColor);
  dataSetLegend.push_back("CMS"); 
  dataSetHisto.push_back("unfolding/gnch_data_corrected");
  dataSetFactor.push_back(10000);
  BinKillStat.push_back( dataSetId.size()-1 );
  dataSetTPlotter.at( dataSetId.size()-1 ).doSmoothing = 0;

  dataSetId.push_back(-1);
  dataSetFile.push_back(fileManager(3,10,2.36,1,0,0,outstr.str(),plotdir));
  dataSetIsMc.push_back(0);
  dataSetHType.push_back(5);
  dataSetStyle.push_back(kCMSPoint);
  dataSetColor.push_back(kCMSColor);
  dataSetLegend.push_back("NONE"); 
  dataSetHisto.push_back("unfolding/gnch_data_corrected");
  dataSetFactor.push_back(100);
  BinKillStat.push_back( dataSetId.size()-1 );
  dataSetTPlotter.at( dataSetId.size()-1 ).doSmoothing = 0;

  dataSetId.push_back(-1);
  dataSetFile.push_back(fileManager(3,10,0.9,1,0,0,outstr.str(),plotdir));
  dataSetIsMc.push_back(0);
  dataSetHType.push_back(5);
  dataSetStyle.push_back(kCMSPoint);
  dataSetColor.push_back(kCMSColor);
  dataSetLegend.push_back("NONE"); 
  dataSetHisto.push_back("unfolding/gnch_data_corrected");
  dataSetFactor.push_back(1);
  BinKillStat.push_back( dataSetId.size()-1 );
  dataSetTPlotter.at( dataSetId.size()-1 ).doSmoothing = 0;

  // MC - PYTHIA D6T

  dataSetId.push_back(-1);
  dataSetFile.push_back("../plots/smallcodev8/smallCode_MCtype30_7TeV.root");
  dataSetIsMc.push_back(1);
  dataSetHType.push_back(1);
  dataSetStyle.push_back(kD6TLineStyle);
  dataSetColor.push_back(kD6TLineColor);
  dataSetLegend.push_back("PYTHIA D6T");
  dataSetHisto.push_back(mchisto.str());
  dataSetFactor.push_back(10000);
  if(iBin==5)  dataSetTPlotter.at( dataSetId.size()-1 ).binKillXMax = 160;
  if(iBin==5)  dataSetTPlotter.at( dataSetId.size()-1 ).nSmooth = 70;
  if(iBin==5)  dataSetTPlotter.at( dataSetId.size()-1 ).smoothingXMin = 60;
  if(iBin==5)  dataSetTPlotter.at( dataSetId.size()-1 ).smoothingXMax = 140;
  //if(iBin==5)  dataSetTPlotter.at( dataSetId.size()-1 ).doSmoothing = 0;
  if(iBin==15) dataSetTPlotter.at( dataSetId.size()-1 ).doSmoothing = 0;
  if(iBin==15) dataSetTPlotter.at( dataSetId.size()-1 ).binKillXMax = 100;

//  if ( iBin ==  5 ) { BinKillMax.push_back( dataSetId.size()-1 ) ; BinKillXMax.push_back( 150 ); }
//  if ( iBin == 15 ) { BinKillMax.push_back( dataSetId.size()-1 ) ; BinKillXMax.push_back( 110 ); }


  dataSetId.push_back(-1);
  dataSetFile.push_back("../plots/smallcodev8/smallCode_MCtype10_2.36TeV.root");
  dataSetIsMc.push_back(1);
  dataSetHType.push_back(1);
  dataSetStyle.push_back(kD6TLineStyle);
  dataSetColor.push_back(kD6TLineColor);
  dataSetLegend.push_back("NONE");
  dataSetHisto.push_back(mchisto.str());
  dataSetFactor.push_back(100);
  //if(iBin==5) dataSetTPlotter.at( dataSetId.size()-1 ).binKillXMax = 108;
  //if(iBin==5)  dataSetTPlotter.at( dataSetId.size()-1 ).doSmoothing = 0;
  if(iBin==5)  dataSetTPlotter.at( dataSetId.size()-1 ).nSmooth = 10;
  if(iBin==5)  dataSetTPlotter.at( dataSetId.size()-1 ).smoothingXMin = 100;
  if(iBin==5)  dataSetTPlotter.at( dataSetId.size()-1 ).smoothingXMax = 115;
  if(iBin==15) dataSetTPlotter.at( dataSetId.size()-1 ).doSmoothing = 0;

//  if ( iBin ==  5 ) { BinKillMax.push_back( dataSetId.size()-1 ) ; BinKillXMax.push_back( 120 ); }
//  if ( iBin == 15 ) { BinKillMax.push_back( dataSetId.size()-1 ) ; BinKillXMax.push_back(  90 ); }

  dataSetId.push_back(-1);
  dataSetFile.push_back("../plots/smallcodev8/smallCode_MCtype10_0.9TeV.root");
  dataSetIsMc.push_back(1);
  dataSetHType.push_back(1);
  dataSetStyle.push_back(kD6TLineStyle);
  dataSetColor.push_back(kD6TLineColor);
  dataSetLegend.push_back("NONE");
  dataSetHisto.push_back(mchisto.str());
  dataSetFactor.push_back(1);
  if(iBin==5)  dataSetTPlotter.at( dataSetId.size()-1 ).doSmoothing = 0;
  if(iBin==5)  dataSetTPlotter.at( dataSetId.size()-1 ).nSmooth = 10;
  if(iBin==5)  dataSetTPlotter.at( dataSetId.size()-1 ).smoothingXMin = 100;
  if(iBin==5)  dataSetTPlotter.at( dataSetId.size()-1 ).smoothingXMax = 115;
  if(iBin==15) dataSetTPlotter.at( dataSetId.size()-1 ).doSmoothing = 0;

//  if ( iBin ==  5 ) { BinKillMax.push_back( dataSetId.size()-1 ) ; BinKillXMax.push_back( 100 ); }
//  if ( iBin == 15 ) { BinKillMax.push_back( dataSetId.size()-1 ) ; BinKillXMax.push_back(  50 ); }

  // MC - PYTHIA CWA 
/*
  if ( iBin == 5) {

    dataSetId.push_back(-1);
    dataSetFile.push_back("../expdata/CW-0.9TeV-Multiplicity.root");
    dataSetIsMc.push_back(1);
    dataSetHType.push_back(1);
    dataSetStyle.push_back(2);
    dataSetColor.push_back(kBlack); 
    dataSetLegend.push_back("PYTHIA CWA");
    dataSetHisto.push_back("h24");
    dataSetFactor.push_back(1);

    dataSetId.push_back(-1);
    dataSetFile.push_back("../expdata/CW-2.36TeV-Multiplicity.root");
    dataSetIsMc.push_back(1);
    dataSetHType.push_back(1);
    dataSetStyle.push_back(2);
    dataSetColor.push_back(kBlack); 
    dataSetLegend.push_back("NONE");
    dataSetHisto.push_back("h24");
    dataSetFactor.push_back(100);
 
    dataSetId.push_back(-1);
    dataSetFile.push_back("../expdata/CW-7TeV-Multiplicity.root");
    dataSetIsMc.push_back(1);
    dataSetHType.push_back(1);
    dataSetStyle.push_back(2);
    dataSetColor.push_back(kBlack); 
    dataSetLegend.push_back("NONE");
    dataSetHisto.push_back("h24");
    dataSetFactor.push_back(10000);

  }


  if ( iBin == 15) {

    dataSetId.push_back(-1);
    dataSetFile.push_back("../expdata/CW-0.9TeV-ptCutMultiplicity.root");
    dataSetIsMc.push_back(1);
    dataSetHType.push_back(1);
    dataSetStyle.push_back(2);
    dataSetColor.push_back(kBlack); 
    dataSetLegend.push_back("PYTHIA CWA");
    dataSetHisto.push_back("h24");
    dataSetFactor.push_back(1);

    dataSetId.push_back(-1);
    dataSetFile.push_back("../expdata/CW-2.36TeV-ptCutMultiplicity.root");
    dataSetIsMc.push_back(1);
    dataSetHType.push_back(1);
    dataSetStyle.push_back(2);
    dataSetColor.push_back(kBlack); 
    dataSetLegend.push_back("NONE");
    dataSetHisto.push_back("h24");
    dataSetFactor.push_back(100);
 
    dataSetId.push_back(-1);
    dataSetFile.push_back("../expdata/CW-7TeV-ptCutMultiplicity.root");
    dataSetIsMc.push_back(1);
    dataSetHType.push_back(1);
    dataSetStyle.push_back(2);
    dataSetColor.push_back(kBlack); 
    dataSetLegend.push_back("NONE");
    dataSetHisto.push_back("h24");
    dataSetFactor.push_back(10000);

  }

*/
  // MC - PYTHIA8    

  if ( iBin == 5) {

    dataSetId.push_back(-1);
    dataSetFile.push_back("../expdata/PYTHIA8-0.9TeV-Multiplicity.root");
    dataSetIsMc.push_back(1);
    dataSetHType.push_back(1);
    dataSetStyle.push_back(kPY8LineStyle);
    dataSetColor.push_back(kPY8LineColor); 
    dataSetLegend.push_back("PYTHIA 8");
    dataSetHisto.push_back("h24");
    dataSetFactor.push_back(1);
    //if(iBin==5)  dataSetTPlotter.at( dataSetId.size()-1 ).doSmoothing = 0;
    if(iBin==5)  dataSetTPlotter.at( dataSetId.size()-1 ).nSmooth = 20;
    if(iBin==5)  dataSetTPlotter.at( dataSetId.size()-1 ).smoothingXMin = 20;
    if(iBin==5)  dataSetTPlotter.at( dataSetId.size()-1 ).smoothingXMax = 75;
    if(iBin==5)  dataSetTPlotter.at( dataSetId.size()-1 ).binKillXMax = 100;

//    BinKillMax.push_back( dataSetId.size()-1 );
//    BinKillXMax.push_back( 50 );

    dataSetId.push_back(-1);
    dataSetFile.push_back("../expdata/PYTHIA8-2.36TeV-Multiplicity.root");
    dataSetIsMc.push_back(1);
    dataSetHType.push_back(1);
    dataSetStyle.push_back(kPY8LineStyle);
    dataSetColor.push_back(kPY8LineColor); 
    dataSetLegend.push_back("NONE");
    dataSetHisto.push_back("h24");
    dataSetFactor.push_back(100);
    dataSetTPlotter.at( dataSetId.size()-1 ).binKillXMax = 118;

    BinKillMax.push_back( dataSetId.size()-1 );
    BinKillXMax.push_back( 118 );
 
    dataSetId.push_back(-1);
    dataSetFile.push_back("../expdata/PYTHIA8-7TeV-Multiplicity.root");
    dataSetIsMc.push_back(1);
    dataSetHType.push_back(1);
    dataSetStyle.push_back(kPY8LineStyle);
    dataSetColor.push_back(kPY8LineColor); 
    dataSetLegend.push_back("NONE");
    dataSetHisto.push_back("h24");
    dataSetFactor.push_back(10000);
    //if(iBin==5)  dataSetTPlotter.at( dataSetId.size()-1 ).doSmoothing = 0;
    if(iBin==5)  dataSetTPlotter.at( dataSetId.size()-1 ).nSmooth = 50;
    if(iBin==5)  dataSetTPlotter.at( dataSetId.size()-1 ).smoothingXMin = 40;
    if(iBin==5)  dataSetTPlotter.at( dataSetId.size()-1 ).smoothingXMax = 130;

//    BinKillMax.push_back( dataSetId.size()-1 );
//    BinKillXMax.push_back( 100 );


  }


  if ( iBin == 15) {

    dataSetId.push_back(-1);
    dataSetFile.push_back("../expdata/PYTHIA8-0.9TeV-ptCutMultiplicity.root");
    dataSetIsMc.push_back(1);
    dataSetHType.push_back(1);
    dataSetStyle.push_back(kPY8LineStyle);
    dataSetColor.push_back(kPY8LineColor); 
    dataSetLegend.push_back("PYTHIA 8");
    dataSetHisto.push_back("h24");
    dataSetFactor.push_back(1);
    dataSetTPlotter.at( dataSetId.size()-1 ).nSmooth = 50;
    //dataSetTPlotter.at( dataSetId.size()-1 ).doSmoothing = 0;
    //dataSetTPlotter.at( dataSetId.size()-1 ).smoothingXMin = 10;
    //dataSetTPlotter.at( dataSetId.size()-1 ).smoothingXMax = 35;

  //  BinKillMax.push_back( dataSetId.size()-1 );
  //  BinKillXMax.push_back( 110 );


    dataSetId.push_back(-1);
    dataSetFile.push_back("../expdata/PYTHIA8-2.36TeV-ptCutMultiplicity.root");
    dataSetIsMc.push_back(1);
    dataSetHType.push_back(1);
    dataSetStyle.push_back(kPY8LineStyle);
    dataSetColor.push_back(kPY8LineColor); 
    dataSetLegend.push_back("NONE");
    dataSetHisto.push_back("h24");
    dataSetFactor.push_back(100);
    dataSetTPlotter.at( dataSetId.size()-1 ).nSmooth = 50;
    dataSetTPlotter.at( dataSetId.size()-1 ).binKillXMax = 69;


  //  BinKillMax.push_back( dataSetId.size()-1 );
  //  BinKillXMax.push_back(  70 );
 
    dataSetId.push_back(-1);
    dataSetFile.push_back("../expdata/PYTHIA8-7TeV-ptCutMultiplicity.root");
    dataSetIsMc.push_back(1);
    dataSetHType.push_back(1);
    dataSetStyle.push_back(kPY8LineStyle);
    dataSetColor.push_back(kPY8LineColor); 
    dataSetLegend.push_back("NONE");
    dataSetHisto.push_back("h24");
    dataSetFactor.push_back(10000);
    dataSetTPlotter.at( dataSetId.size()-1 ).nSmooth = 150;
    //dataSetTPlotter.at( dataSetId.size()-1 ).doSmoothing = 0;
    //dataSetTPlotter.at( dataSetId.size()-1 ).smoothingXMin = 10;
    //dataSetTPlotter.at( dataSetId.size()-1 ).smoothingXMax = 35;


 //   BinKillMax.push_back( dataSetId.size()-1 );
 //   BinKillXMax.push_back(  50 );

  }


  // MC Phojet
  dataSetId.push_back(-1);
  dataSetFile.push_back("../plots/smallcodev8/smallCode_MCtype20_7TeV.root");
  dataSetIsMc.push_back(1);
  dataSetHType.push_back(1);
  dataSetStyle.push_back(kPHOLineStyle);
  dataSetColor.push_back(kPHOLineColor);
  dataSetLegend.push_back("PHOJET");
  dataSetHisto.push_back(mchisto.str());
  dataSetFactor.push_back(10000);
  //if(iBin==5) dataSetTPlotter.at( dataSetId.size()-1 ).doSmoothing = 0;
  if(iBin==5) dataSetTPlotter.at( dataSetId.size()-1 ).smoothingXMin = 70;
  if(iBin==5) dataSetTPlotter.at( dataSetId.size()-1 ).smoothingXMax = 140;
  if(iBin==5) dataSetTPlotter.at( dataSetId.size()-1 ).nSmooth = 300;
  if(iBin==5) dataSetTPlotter.at( dataSetId.size()-1 ).binKillXMax = 160;
  if(iBin==15) dataSetTPlotter.at( dataSetId.size()-1 ).binKillXMax = 70;
  //if(iBin==15) dataSetTPlotter.at( dataSetId.size()-1 ).doSmoothing = 0;
  //if(iBin==15) dataSetTPlotter.at( dataSetId.size()-1 ).nSmooth = 40;
  //if(iBin==15) dataSetTPlotter.at( dataSetId.size()-1 ).smoothingXMin = 10;
  //if(iBin==15) dataSetTPlotter.at( dataSetId.size()-1 ).smoothingXMax = 35;

  dataSetId.push_back(-1);
  dataSetFile.push_back("../plots/smallcodev8/smallCode_MCtype20_2.36TeV.root");
  dataSetIsMc.push_back(1);
  dataSetHType.push_back(1);
  dataSetStyle.push_back(kPHOLineStyle);
  dataSetColor.push_back(kPHOLineColor);
  dataSetLegend.push_back("NONE");
  dataSetHisto.push_back(mchisto.str());
  dataSetFactor.push_back(100);
  //dataSetTPlotter.at( dataSetId.size()-1 ).doSmoothing = 0;
  if(iBin==5) dataSetTPlotter.at( dataSetId.size()-1 ).nSmooth = 20;
  if(iBin==5) dataSetTPlotter.at( dataSetId.size()-1 ).binKillXMax = 118;
  if(iBin==15) dataSetTPlotter.at( dataSetId.size()-1 ).binKillXMax = 47;
  //if(iBin==15) dataSetTPlotter.at( dataSetId.size()-1 ).doSmoothing = 0;
  if(iBin==15) dataSetTPlotter.at( dataSetId.size()-1 ).nSmooth = 40;
  if(iBin==15) dataSetTPlotter.at( dataSetId.size()-1 ).smoothingXMin = 10;
  if(iBin==15) dataSetTPlotter.at( dataSetId.size()-1 ).smoothingXMax = 35;

  dataSetId.push_back(-1);
  dataSetFile.push_back("../plots/smallcodev8/smallCode_MCtype20_0.9TeV.root");
  dataSetIsMc.push_back(1);
  dataSetHType.push_back(1);
  dataSetStyle.push_back(kPHOLineStyle);
  dataSetColor.push_back(kPHOLineColor);
  dataSetLegend.push_back("NONE");
  dataSetHisto.push_back(mchisto.str());
  dataSetFactor.push_back(1);
  //dataSetTPlotter.at( dataSetId.size()-1 ).doSmoothing = 0;
  if(iBin==5) dataSetTPlotter.at( dataSetId.size()-1 ).nSmooth = 30;
  if(iBin==5) dataSetTPlotter.at( dataSetId.size()-1 ).smoothingXMin = 30;
  if(iBin==5) dataSetTPlotter.at( dataSetId.size()-1 ).smoothingXMax = 70;
  if(iBin==5) dataSetTPlotter.at( dataSetId.size()-1 ).binKillXMax = 95;
  //if(iBin==15) dataSetTPlotter.at( dataSetId.size()-1 ).doSmoothing = 0;
  if(iBin==15) dataSetTPlotter.at( dataSetId.size()-1 ).nSmooth = 100;
  if(iBin==15) dataSetTPlotter.at( dataSetId.size()-1 ).smoothingXMin = 20;
  if(iBin==15) dataSetTPlotter.at( dataSetId.size()-1 ).smoothingXMax = 35;


  // Data systematics
 
  dataSetId.push_back(-1);
  dataSetFile.push_back(fileManager(3,31,7.,1,0,0,outstr.str(),plotdir));
  dataSetIsMc.push_back(0);
  dataSetHType.push_back(105);
  dataSetStyle.push_back(kCMSPoint);
  dataSetColor.push_back(kCMSColor);
  dataSetLegend.push_back("NONE");
  dataSetHisto.push_back("unfolding/gnch_data_corrected_syst");
  dataSetFactor.push_back(10000);
  BinKillSyst.push_back( dataSetId.size()-1 );
  dataSetTPlotter.at( dataSetId.size()-1 ).doSmoothing = 0;

  dataSetId.push_back(-1);
  dataSetFile.push_back(fileManager(3,10,2.36,1,0,0,outstr.str(),plotdir));
  dataSetIsMc.push_back(0);
  dataSetHType.push_back(105);
  dataSetStyle.push_back(kCMSPoint);
  dataSetColor.push_back(kCMSColor);
  dataSetLegend.push_back("NONE");
  dataSetHisto.push_back("unfolding/gnch_data_corrected_syst");
  dataSetFactor.push_back(100);
  BinKillSyst.push_back( dataSetId.size()-1 );
  dataSetTPlotter.at( dataSetId.size()-1 ).doSmoothing = 0;

  dataSetId.push_back(-1);
  dataSetFile.push_back(fileManager(3,10,0.9,1,0,0,outstr.str(),plotdir));
  dataSetIsMc.push_back(0);
  dataSetHType.push_back(105);
  dataSetStyle.push_back(kCMSPoint);
  dataSetColor.push_back(kCMSColor);
  dataSetLegend.push_back("NONE");
  dataSetHisto.push_back("unfolding/gnch_data_corrected_syst");
  dataSetFactor.push_back(1);
  BinKillSyst.push_back( dataSetId.size()-1 );
  dataSetTPlotter.at( dataSetId.size()-1 ).doSmoothing = 0;

  // Curve legends

  Float_t xt1 , yt1  ;
  Float_t xt3 , yt3  ;
  Float_t xt5 , yt5  ;

  if ( iBin == 5 ) {
     xt1 = .35; yt1 = .82;
     xt3 = .25; yt3 = .68;
     xt5 = .20; yt5 = .54;
  }
  if ( iBin == 15 ) {
     xt1 = .35; yt1 = .82;
     xt3 = .25; yt3 = .68;
     xt5 = .20; yt5 = .54;
  }

  globalTextSize = 0.03;

  textXPos.push_back(xt1)  ; textYPos.push_back(yt1) ;
  textXPos.push_back(xt3)  ; textYPos.push_back(yt3) ;
  textXPos.push_back(xt5)  ; textYPos.push_back(yt5) ;

  textText.push_back("7 TeV (x10000)");
  textText.push_back("2.36 TeV (x100)");
  textText.push_back("0.9 TeV (x1)");

  textXPos.push_back(0.16) ; textYPos.push_back(.25) ;
  textText.push_back("|#eta| < 2.4");

  if ( iBin == 5 ) {
    textXPos.push_back(0.16) ; textYPos.push_back(.2) ;
    textText.push_back("p_{T} > 0 ");
  }


  if ( iBin == 15 ) {
    textXPos.push_back(0.16) ; textYPos.push_back(.2) ;
    textText.push_back("p_{T} > 0.5 GeV/c");
  }

  // PLOT NOW

  yLegendWidth = 0.030;
  xLegendWidth = .13 ;
  globalLegendTextSize = 22;
  
  plot("none","AUTO",1,4);



}
