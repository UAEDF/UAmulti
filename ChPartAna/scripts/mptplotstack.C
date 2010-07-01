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

void mptplotstack(double energy = 0.9, int iBin = 5 , int iSaveFig = 1, float npx = 900 , float npy = 900){

  globalEnergy = energy;
  gROOT->ProcessLine(".x cmsStyleRoot.C");

  plotReset();

  globalCanvasSizeX = npx;
  globalCanvasSizeY = npy;

  XaxisTitle = "n";
  YaxisTitle = "a + <p_{T}> [GeV/c]";
  
  #include "../macro/acceptanceMap.C"
  ostringstream legheader("");
  legheader<< "  |#eta| < "<<accMap.at(iBin).at(1);
  LegendTitle = legheader.str();
  
  xGlobalLabel = 0.6 ; 
  globalLabel =  "CMS Preliminary";

  globalSaveFig = iSaveFig;
  if (iBin == 5 ) globalFigBaseName = "mptstack_eta24" ; 
  if (iBin == 7 ) globalFigBaseName = "mptstack_eta15" ; 
  if (iBin == 9 ) globalFigBaseName = "mptstack_eta05" ; 
  if (iBin == 15 ) globalFigBaseName = "mptstack_pt500_eta24" ; 
  if (iBin == 17 ) globalFigBaseName = "mptstack_pt500_eta15" ; 
  if (iBin == 19 ) globalFigBaseName = "mptstack_pt500_eta05" ; 




  globalNorm = 0;

  histoXMin  = 0.0;
  if (iBin == 5 ) histoXMax  = 190.0;
  if (iBin == 7 ) histoXMax  = 150. ;
  if (iBin == 9 ) histoXMax  = 60.;
  if (iBin == 15 ) histoXMax  = 182.5;
  if (iBin == 17 ) histoXMax  = 120. ;
  if (iBin == 19 ) histoXMax  = 60.;


  histoYMin  = 0.0;
  histoYMax  = 2.5;

  ostringstream mchisto("");
  mchisto << "mptVSnchgen_gen_cut" << iBin;  
  ostringstream datahisto("");
  datahisto << "mptVSnch_cut" << iBin; 
  ostringstream systhisto("");
  systhisto << "gmptVSnch_cut" << iBin << "_syst"; 

  double mptcor900 = 0.9365/0.9596 ; 
  double mptcor236 = 0.9365/0.9609 ; 
  double mptcor7   = 0.9376/0.9560 ; 
  if (iBin > 10 ) { mptcor900 =1 ; mptcor236=1 ; mptcor7=1; }


  cout << "mptcor900 = " << mptcor900 << endl;
  cout << "mptcor236 = " << mptcor236 << endl;
  cout << "mptcor7   = " << mptcor7   << endl;


  // LineStyle

  int kD6TLineStyle = 2 ;
  int kD6TLineColor = kRed   ;

  int kPY8LineStyle = 1 ;
  int kPY8LineColor = kMagenta ;

  int kPHOLineStyle = 7 ;
  int kPHOLineColor = kBlue ;

  // dummy

    dataSetId.push_back(-1);
    dataSetFile.push_back("../plots/mptcorrv9/mptCorr_MC_ATLAS_7.0TeV_mbTr__dataType0.root");
    dataSetIsMc.push_back(0);
    dataSetHType.push_back(3);
    dataSetStyle.push_back(kOpenCircle);
    dataSetColor.push_back(kWhite);
    dataSetLegend.push_back("NONE");
    dataSetHisto.push_back(datahisto.str());
    dataSetFactor.push_back(mptcor7);
    dataSetOffset.push_back(1.);



  // ------ 900 GeV --------------------------------------------

    dataSetId.push_back(-1);
    dataSetFile.push_back("../plots/mptcorrv9/mptCorr_MC_D6T_0.9TeV_mbTr__dataType0.root");
    dataSetIsMc.push_back(0);
    dataSetHType.push_back(3);
    dataSetStyle.push_back(kOpenCircle);
    dataSetColor.push_back(kBlack);
    dataSetLegend.push_back("CMS");
    dataSetHisto.push_back(datahisto.str());
    dataSetFactor.push_back(mptcor900);
    dataSetOffset.push_back(0.);

    BinKillMax.push_back( dataSetId.size()-1 );
    if ( iBin == 5 ) BinKillXMax.push_back( 90. );
    if ( iBin == 7 ) BinKillXMax.push_back( 70. );
    if ( iBin == 9 ) BinKillXMax.push_back( 30. );

    // MC
  
    dataSetId.push_back(-1);
    dataSetFile.push_back("../plots/smallcodev6/smallCode_MCtype10_0.9TeV.root");
    dataSetIsMc.push_back(1);
    dataSetHType.push_back(3);
    dataSetStyle.push_back(kD6TLineStyle);
    dataSetColor.push_back(kD6TLineColor);
    dataSetLegend.push_back("PYTHIA D6T");
    dataSetHisto.push_back(mchisto.str());
    dataSetFactor.push_back(1);
    dataSetOffset.push_back(0.);

    BinKillMax.push_back( dataSetId.size()-1 );
    if ( iBin == 5 ) BinKillXMax.push_back( 90. );
    if ( iBin == 7 ) BinKillXMax.push_back( 70. );
    if ( iBin == 9 ) BinKillXMax.push_back( 30. );
    

    dataSetId.push_back(-1);
    dataSetFile.push_back("../expdata/meanPt-PYTHIA8-0.9TeV.root");
    dataSetIsMc.push_back(1);
    dataSetHType.push_back(3);
    dataSetStyle.push_back(kPY8LineStyle);
    dataSetColor.push_back(kPY8LineColor); 
    dataSetLegend.push_back("PYTHIA 8");
    if ( iBin == 5 ) dataSetHisto.push_back("hPt24");
    if ( iBin == 7 ) dataSetHisto.push_back("hPt15");
    if ( iBin == 9 ) dataSetHisto.push_back("hPt05");
    dataSetFactor.push_back(1);
    dataSetOffset.push_back(0.);

    BinKillMax.push_back( dataSetId.size()-1 );
    if ( iBin == 5 ) BinKillXMax.push_back( 90. );
    if ( iBin == 7 ) BinKillXMax.push_back( 70. );
    if ( iBin == 9 ) BinKillXMax.push_back( 30. );



/*  
    dataSetId.push_back(-1);
    dataSetFile.push_back("../plots/smallcodev6/smallCode_MCtype11_0.9TeV.root");
    dataSetIsMc.push_back(1);
    dataSetHType.push_back(1);
    dataSetStyle.push_back(20);
    dataSetColor.push_back(kBlue);
    dataSetLegend.push_back("PYTHIA DW");
    dataSetHisto.push_back(mchisto.str());
    dataSetFactor.push_back(1);
    dataSetOffset.push_back(0.);
  
    dataSetId.push_back(-1);
    dataSetFile.push_back("../plots/smallcodev6/smallCode_MCtype13_0.9TeV.root");
    dataSetIsMc.push_back(1);
    dataSetHType.push_back(1);
    dataSetStyle.push_back(20);
    dataSetColor.push_back(kOrange);
    dataSetLegend.push_back("PYTHIA ProQ20");
    dataSetHisto.push_back(mchisto.str());
    dataSetFactor.push_back(1);
    dataSetOffset.push_back(0.);
*/
  
    dataSetId.push_back(-1);
    dataSetFile.push_back("../plots/smallcodev6/smallCode_MCtype20_0.9TeV.root");
    dataSetIsMc.push_back(1);
    dataSetHType.push_back(3);
    dataSetStyle.push_back(kPHOLineStyle);
    dataSetColor.push_back(kPHOLineColor);
    dataSetLegend.push_back("PHOJET");
    dataSetHisto.push_back(mchisto.str());
    dataSetFactor.push_back(1);
    dataSetOffset.push_back(0.);
  
    BinKillMax.push_back( dataSetId.size()-1 );
    if ( iBin == 5 ) BinKillXMax.push_back( 90. );
    if ( iBin == 7 ) BinKillXMax.push_back( 70. );
    if ( iBin == 9 ) BinKillXMax.push_back( 30. );
  
    // Data Syst

    dataSetId.push_back(-1);
    dataSetFile.push_back("../plots/mptcorrv9/mptCorr_MC_D6T_0.9TeV_mbTr__dataType0.root");
    dataSetIsMc.push_back(0);
    dataSetHType.push_back(105);
    dataSetStyle.push_back(kOpenCircle);
    dataSetColor.push_back(kBlack);
    dataSetLegend.push_back("NONE");
    dataSetHisto.push_back(systhisto.str());
    dataSetFactor.push_back(mptcor900);
    dataSetOffset.push_back(0.);

    BinKillMax.push_back( dataSetId.size()-1 );
    if ( iBin == 5 ) BinKillXMax.push_back( 90. );
    if ( iBin == 7 ) BinKillXMax.push_back( 70. );
    if ( iBin == 9 ) BinKillXMax.push_back( 30. );

  // ------------ 2.36 TeV -------------------

    dataSetId.push_back(-1);
    dataSetFile.push_back("../plots/mptcorrv9/mptCorr_MC_D6T_2.36TeV_mbTr__dataType0.root");
    dataSetIsMc.push_back(0);
    dataSetHType.push_back(3);
    dataSetStyle.push_back(kOpenCircle);
    dataSetColor.push_back(kBlack);
    dataSetLegend.push_back("NONE");
    dataSetHisto.push_back(datahisto.str());
    dataSetFactor.push_back(mptcor236);
    dataSetOffset.push_back(.5);

    BinKillMax.push_back( dataSetId.size()-1 );
    if ( iBin == 5 ) BinKillXMax.push_back( 120 );
    if ( iBin == 7 ) BinKillXMax.push_back(  95 );
    if ( iBin == 9 ) BinKillXMax.push_back(  40 );


    // MC
 
    dataSetId.push_back(-1);
    dataSetFile.push_back("../plots/smallcodev6/smallCode_MCtype10_2.36TeV.root");
    dataSetIsMc.push_back(1);
    dataSetHType.push_back(3);
    dataSetStyle.push_back(kD6TLineStyle);
    dataSetColor.push_back(kD6TLineColor);
    dataSetLegend.push_back("NONE");
    dataSetHisto.push_back(mchisto.str());
    dataSetFactor.push_back(1);
    dataSetOffset.push_back(.5);

    BinKillMax.push_back( dataSetId.size()-1 );
    if ( iBin == 5 ) BinKillXMax.push_back( 120 );
    if ( iBin == 7 ) BinKillXMax.push_back(  95 );
    if ( iBin == 9 ) BinKillXMax.push_back(  40 );

    dataSetId.push_back(-1);
    dataSetFile.push_back("../expdata/meanPt-PYTHIA8-2.36TeV.root");
    dataSetIsMc.push_back(1);
    dataSetHType.push_back(3);
    dataSetStyle.push_back(kPY8LineStyle);
    dataSetColor.push_back(kPY8LineColor);
    dataSetLegend.push_back("NONE");
    if ( iBin == 5 ) dataSetHisto.push_back("hPt24");
    if ( iBin == 7 ) dataSetHisto.push_back("hPt15");
    if ( iBin == 9 ) dataSetHisto.push_back("hPt05");
    dataSetFactor.push_back(1);
    dataSetOffset.push_back(.5);

    BinKillMax.push_back( dataSetId.size()-1 );
    if ( iBin == 5 ) BinKillXMax.push_back( 120 );
    if ( iBin == 7 ) BinKillXMax.push_back(  95 );
    if ( iBin == 9 ) BinKillXMax.push_back(  40 );

    dataSetId.push_back(-1);
    dataSetFile.push_back("../plots/smallcodev6/smallCode_MCtype20_2.36TeV.root");
    dataSetIsMc.push_back(1);
    dataSetHType.push_back(3);
    dataSetStyle.push_back(kPHOLineStyle);
    dataSetColor.push_back(kPHOLineColor);
    dataSetLegend.push_back("NONE");
    dataSetHisto.push_back(mchisto.str());
    dataSetFactor.push_back(1);
    dataSetOffset.push_back(.5);

    BinKillMax.push_back( dataSetId.size()-1 );
    if ( iBin == 5 ) BinKillXMax.push_back( 120 );
    if ( iBin == 7 ) BinKillXMax.push_back(  95 );
    if ( iBin == 9 ) BinKillXMax.push_back(  40 );

    // Data Syst
 
    dataSetId.push_back(-1);
    dataSetFile.push_back("../plots/mptcorrv9/mptCorr_MC_D6T_2.36TeV_mbTr__dataType0.root");
    dataSetIsMc.push_back(0);
    dataSetHType.push_back(105);
    dataSetStyle.push_back(kOpenCircle);
    dataSetColor.push_back(kBlack);
    dataSetLegend.push_back("NONE");
    dataSetHisto.push_back(systhisto.str());
    dataSetFactor.push_back(mptcor236);
    dataSetOffset.push_back(.5);

    BinKillMax.push_back( dataSetId.size()-1 );
    if ( iBin == 5 ) BinKillXMax.push_back( 120 );
    if ( iBin == 7 ) BinKillXMax.push_back(  95 );
    if ( iBin == 9 ) BinKillXMax.push_back(  40 );


  // ------------ 7.0 TeV -------------------

    dataSetId.push_back(-1);
    dataSetFile.push_back("../plots/mptcorrv9/mptCorr_MC_ATLAS_7.0TeV_mbTr__dataType0.root");
    dataSetIsMc.push_back(0);
    dataSetHType.push_back(3);
    dataSetStyle.push_back(kOpenCircle);
    dataSetColor.push_back(kBlack);
    dataSetLegend.push_back("NONE");
    dataSetHisto.push_back(datahisto.str());
    dataSetFactor.push_back(mptcor7);
    dataSetOffset.push_back(1.);

   BinKillMax.push_back( dataSetId.size()-1 );
    if ( iBin == 5 ) BinKillXMax.push_back( 180 );
    if ( iBin == 7 ) BinKillXMax.push_back( 140 );
    if ( iBin == 9 ) BinKillXMax.push_back(  55 );

    // MC

    dataSetId.push_back(-1);
    dataSetFile.push_back("../plots/smallcodev6/smallCode_MCtype30_7TeV.root");
    dataSetIsMc.push_back(1);
    dataSetHType.push_back(3);
    dataSetStyle.push_back(kD6TLineStyle);
    dataSetColor.push_back(kD6TLineColor);
    dataSetLegend.push_back("NONE");
    dataSetHisto.push_back(mchisto.str());
    dataSetFactor.push_back(1);
    dataSetOffset.push_back(1.);

    BinKillMax.push_back( dataSetId.size()-1 );
    if ( iBin == 5 ) BinKillXMax.push_back( 150 );
    if ( iBin == 7 ) BinKillXMax.push_back( 100 );
    if ( iBin == 9 ) BinKillXMax.push_back(  35 );



    dataSetId.push_back(-1);
    dataSetFile.push_back("../expdata/meanPt-PYTHIA8-7TeV.root");
    dataSetIsMc.push_back(1);
    dataSetHType.push_back(3);
    dataSetStyle.push_back(kPY8LineStyle);
    dataSetColor.push_back(kPY8LineColor);
    dataSetLegend.push_back("NONE");
    if ( iBin == 5 ) dataSetHisto.push_back("hPt24");
    if ( iBin == 7 ) dataSetHisto.push_back("hPt15");
    if ( iBin == 9 ) dataSetHisto.push_back("hPt05");
    dataSetFactor.push_back(1);
    dataSetOffset.push_back(1.);

    BinKillMax.push_back( dataSetId.size()-1 );
    if ( iBin == 5 ) BinKillXMax.push_back( 180 );
    if ( iBin == 7 ) BinKillXMax.push_back( 140 );
    if ( iBin == 9 ) BinKillXMax.push_back(  55 );


 /* 
    dataSetId.push_back(-1);
    dataSetFile.push_back("../plots/smallcodev6/smallCode_MCtype31_7TeV.root");
    dataSetIsMc.push_back(1);
    dataSetHType.push_back(1);
    dataSetStyle.push_back(20);
    dataSetColor.push_back(kBlue);
    dataSetLegend.push_back("PYTHIA atlas");
    dataSetHisto.push_back(mchisto.str());
    dataSetFactor.push_back(1);
    dataSetOffset.push_back(2.);
*/  
    dataSetId.push_back(-1);
    dataSetFile.push_back("../plots/smallcodev6/smallCode_MCtype20_7TeV.root");
    dataSetIsMc.push_back(1);
    dataSetHType.push_back(3);
    dataSetStyle.push_back(kPHOLineStyle);
    dataSetColor.push_back(kPHOLineColor);
    dataSetLegend.push_back("NONE");
    dataSetHisto.push_back(mchisto.str());
    dataSetFactor.push_back(1);
    dataSetOffset.push_back(1.);

    BinKillMax.push_back( dataSetId.size()-1 );
    if ( iBin == 5 ) BinKillXMax.push_back( 150 );
    if ( iBin == 7 ) BinKillXMax.push_back( 100 );
    if ( iBin == 9 ) BinKillXMax.push_back(  35 );

  /*
    dataSetId.push_back(-1);
    dataSetFile.push_back("../plots/smallcodev6/smallCode_MCtype51_7TeV.root");
    dataSetIsMc.push_back(1);
    dataSetHType.push_back(1);
    dataSetStyle.push_back(20);
    dataSetColor.push_back(kOrange);
    dataSetLegend.push_back("PYTHIA X1");
    dataSetHisto.push_back(mchisto.str());
    dataSetFactor.push_back(1);
    dataSetOffset.push_back(2.);
  
    dataSetId.push_back(-1);
    dataSetFile.push_back("../plots/smallcodev6/smallCode_MCtype52_7TeV.root");
    dataSetIsMc.push_back(1);
    dataSetHType.push_back(1);
    dataSetStyle.push_back(20);
    dataSetColor.push_back(kGreen);
    dataSetLegend.push_back("PYTHIA X2");
    dataSetHisto.push_back(mchisto.str());
    dataSetFactor.push_back(1);
    dataSetOffset.push_back(2.);
 */

    // data Syst  
    dataSetId.push_back(-1);
    dataSetFile.push_back("../plots/mptcorrv9/mptCorr_MC_ATLAS_7.0TeV_mbTr__dataType0.root");
    dataSetIsMc.push_back(0);
    dataSetHType.push_back(105);
    dataSetStyle.push_back(kOpenCircle);
    dataSetColor.push_back(kBlack);
    dataSetLegend.push_back("NONE");
    dataSetHisto.push_back(systhisto.str());
    dataSetFactor.push_back(mptcor7);
    dataSetOffset.push_back(1.);

   BinKillMax.push_back( dataSetId.size()-1 );
    if ( iBin == 5 ) BinKillXMax.push_back( 180 );
    if ( iBin == 7 ) BinKillXMax.push_back( 140 );
    if ( iBin == 9 ) BinKillXMax.push_back(  55 );



  // Text

  Float_t xt1 , yt1  ;
  Float_t xt2 , yt2  ;
  Float_t xt3 , yt3  ;

  
     xt1 = .68; yt1 = .72;
     xt2 = .68; yt2 = .50;
     xt3 = .68; yt3 = .30;

  if (iBin == 7 ) {
     xt1 = .68; yt1 = .78;
     xt2 = .68; yt2 = .52;
     xt3 = .68; yt3 = .32;
  }

  if (iBin == 9 ) {
     xt1 = .68; yt1 = .78;
     xt2 = .68; yt2 = .52;
     xt3 = .68; yt3 = .32;
  }



  globalTextSize = 0.03;

  textXPos.push_back(xt1)  ; textYPos.push_back(yt1) ;
  textXPos.push_back(xt2)  ; textYPos.push_back(yt2) ;
  textXPos.push_back(xt3)  ; textYPos.push_back(yt3) ;

  textText.push_back("   7 TeV (a=1)");
  textText.push_back("2.36 TeV (a=0.5)" );
  textText.push_back(" 0.9 TeV (a=0)" );

  // PLOT

  yLegendWidth = 0.035;
  xLegendWidth = .13 ;
  globalLegendTextSize = 22;


  plot("none","AUTO",0,8);

}
