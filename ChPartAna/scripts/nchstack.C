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

void nchstack( double Energy = 0.9 , int iBin = 5 , int iSaveFig = 1, 
               float npx = 1000 , float npy = 600 , bool tracklets = false  ){

  globalEnergy = Energy ;
  gROOT->ProcessLine(".x cmsStyleRoot.C");

  plotReset();
 
  globalCanvasSizeX = npx; 
  globalCanvasSizeY = npy;
 
  XaxisTitle = "n";
  YaxisTitle = "P_{n}";

  xGlobalLabel = 0.6 ;
  if ( globalEnergy == 0.9 ) globalLabel =  "CMS Preliminary 0.9 TeV";
  if ( globalEnergy == 2.36) globalLabel =  "CMS Preliminary 2.36 TeV";
  if ( globalEnergy == 7.0 ) globalLabel =  "CMS Preliminary 7 TeV";

  globalSaveFig = iSaveFig;
  if ( iBin == 5 ) { 
    if ( ! tracklets ) {
      if ( globalEnergy == 0.9 ) globalFigBaseName = "nch_900GeV_pt0";  
      if ( globalEnergy == 2.36) globalFigBaseName = "nch_2360GeV_pt0";  
      if ( globalEnergy == 7.0 ) globalFigBaseName = "nch_7000GeV_pt0";
    } else {
      if ( globalEnergy == 0.9 ) globalFigBaseName = "nch_900GeV_pt0_tracklets";  
      if ( globalEnergy == 2.36) globalFigBaseName = "nch_2360GeV_pt0_tracklets";  
      if ( globalEnergy == 7.0 ) globalFigBaseName = "nch_7000GeV_pt0_tracklets";
    }     
  }
  if ( iBin == 15 ) { 
    if ( globalEnergy == 0.9 ) globalFigBaseName = "nch_900GeV_pt500";  
    if ( globalEnergy == 2.36) globalFigBaseName = "nch_2360GeV_pt500";  
    if ( globalEnergy == 7.0 ) globalFigBaseName = "nch_7000GeV_pt500";  
  }

  globalNorm = 2;

  if (iBin == 5 ) {
    if ( globalEnergy == 0.9 ) histoYMin  = 0.00001 ;
    if ( globalEnergy == 2.36) histoYMin  = 0.00001 ;
    if ( globalEnergy == 7.0 ) histoYMin  = 0.0000001 ;
    histoYMax  = 1000.;
    histoXMin  = -0.5 ;
    if ( globalEnergy == 0.9 ) histoXMax  = 90.5;
    if ( globalEnergy == 2.36) histoXMax  = 110.5;
    if ( globalEnergy == 7.0 ) histoXMax  = 180.5;
  }
  if (iBin == 15 ) {
    if ( globalEnergy == 0.9 ) histoYMin  = 0.00001 ;
    if ( globalEnergy == 2.36) histoYMin  = 0.00001 ;
    if ( globalEnergy == 7.0 ) histoYMin  = 0.00000006;
    histoYMax  = 3000.;
    histoXMin  = -0.5 ;
    if ( globalEnergy == 0.9 ) histoXMax  = 50.5;
    if ( globalEnergy == 2.36) histoXMax  = 70.5;
    if ( globalEnergy == 7.0 ) histoXMax  = 110.5;
  }

  int iMc;
  if ( globalEnergy == 0.9 ) iMc = 10 ;
  if ( globalEnergy == 2.36) iMc = 10 ;
  if ( globalEnergy == 7.0 ) iMc = 31 ;

  TString plotdir("");
//  if ( globalEnergy == 0.9 ) plotdir = "../plots/finalv9/";
//  if ( globalEnergy == 2.36) plotdir = "../plots/finalv9/";
//  if ( globalEnergy == 7.0 ) plotdir = "../plots/systv10/";

  if ( globalEnergy == 0.9 ) plotdir = "../plots/systv10_niter100_2/";
  if ( globalEnergy == 2.36) plotdir = "../plots/systv10_niter100_2/";
  if ( globalEnergy == 7.0 ) plotdir = "../plots/systv10_niter100_2/";

  ostringstream outstr0("");
  ostringstream outstr1("");
  ostringstream outstr2("");
  ostringstream outstr3("");
  ostringstream outstr4("");
  outstr0 << "hyp" << 1 << "_niter" << 0 << "_cut" << iBin   << "_DataType" << 0;  
  outstr1 << "hyp" << 1 << "_niter" << 0 << "_cut" << iBin+1 << "_DataType" << 0;  
  outstr2 << "hyp" << 1 << "_niter" << 0 << "_cut" << iBin+2 << "_DataType" << 0;  
  outstr3 << "hyp" << 1 << "_niter" << 0 << "_cut" << iBin+3 << "_DataType" << 0;  
  outstr4 << "hyp" << 1 << "_niter" << 0 << "_cut" << iBin+4 << "_DataType" << 0;  

  // Points and Colors for Experiments

  int kCMSPoint = kOpenCircle ;
  int kCMSColor = kBlack; 

  int kTrackletsPoint = kOpenCircle ;
  int kTrackletsColor = kRed; 

  int kUA1Point = kOpenStar ;
  int kUA1Color = kBlue; 

  int kUA5Point = kOpenTriangleUp ;
  int kUA5Color = kRed; 

  int kALICEPoint = kOpenDiamond ;
  int kALICEColor = kMagenta ;

  int kATLASPoint = kOpenTriangleUp ;
  int kATLASColor = kBlue; 

  // Define plots DUMMY first histo (kWhite)

  dataSetId.push_back(-1);
  dataSetFile.push_back(fileManager(3,iMc,globalEnergy,1,0,0,outstr0.str(),plotdir));
  dataSetIsMc.push_back(0);
  dataSetHType.push_back(1);
  dataSetStyle.push_back(kCMSPoint);
  dataSetColor.push_back(kCMSColor);
//  if (globalEnergy != 7.  ||  ! ( globalEnergy>2. && iBin == 15  ) ) 
//                           dataSetLegend.push_back("CMS");
//  else                     dataSetLegend.push_back("NONE"); 
  if ( ! tracklets ) dataSetLegend.push_back("CMS");
  else               dataSetLegend.push_back("Tracks");

  dataSetHisto.push_back("unfolding/nch_data_corrected");
  dataSetFactor.push_back(10000);

  // |eta| < 2.4
 
  if ( globalEnergy == 0.9 && iBin == 5 ) {
    if (! tracklets ) {
      // UA 1 @ 900 GeV |n|<2.5 
      dataSetId.push_back(-1);
      dataSetFile.push_back("../expdata/ua1_dsigdn_eta25");
      dataSetIsMc.push_back(0);
      dataSetHType.push_back(5);
      dataSetStyle.push_back(kUA1Point);
      dataSetColor.push_back(kUA1Color);
      dataSetLegend.push_back("UA1");
      dataSetHisto.push_back("UA1"); 
      dataSetFactor.push_back(10000/50.3); // Divided by UA1 sigma_inel
    }
  }

  if ( globalEnergy == 0.9 && iBin == 15 ) {
    // ATLAS @ 900 GeV Pt>500 MeV
    dataSetId.push_back(-1);
    dataSetFile.push_back("../expdata/atlas_dsigdn_nsd_900GeV_eta25_pt500");
    dataSetIsMc.push_back(0);
    dataSetHType.push_back(5);
    dataSetStyle.push_back(kATLASPoint);
    dataSetColor.push_back(kATLASColor);
    dataSetLegend.push_back("ATLAS");
    dataSetHisto.push_back("ALICE");
    dataSetFactor.push_back(10000);

    dataSetId.push_back(-1);
    dataSetFile.push_back("../expdata/atlas_dsigdn_nsd_900GeV_eta25_pt500");
    dataSetIsMc.push_back(0);
    dataSetHType.push_back(105);
    dataSetStyle.push_back(kOpenTriangleUp);
    dataSetColor.push_back(kBlue);
    dataSetLegend.push_back("NONE");
    dataSetHisto.push_back("ALICE");
    dataSetFactor.push_back(10000);
  }

/* 
  dataSetId.push_back(-1);
  dataSetFile.push_back(fileManager(3,iMc,globalEnergy,1,0,0,outstr0.str(),plotdir));
  dataSetIsMc.push_back(0);
  dataSetHType.push_back(1);
  dataSetStyle.push_back(kCMSPoint);
  dataSetColor.push_back(kCMSColor);
  dataSetLegend.push_back("NONE");
  dataSetHisto.push_back("unfolding/nch_data_corrected");
  dataSetFactor.push_back(10000);
*/

  dataSetId.push_back(-1);
  dataSetFile.push_back(fileManager(3,iMc,globalEnergy,1,0,0,outstr0.str(),plotdir));
  dataSetIsMc.push_back(0);
  dataSetHType.push_back(205);
  dataSetStyle.push_back(kCMSPoint);
  dataSetColor.push_back(kCMSColor);
  dataSetLegend.push_back("NONE");
  dataSetHisto.push_back("unfolding/gnch_data_corrected_syst");
  dataSetFactor.push_back(10000);

 // BinKillStat.push_back( dataSetId.size()-2 );
  BinKillStat.push_back( 0 );
  BinKillSyst.push_back( dataSetId.size()-1 );
  
  // |eta| < 2.0

  dataSetId.push_back(-1);
  dataSetFile.push_back(fileManager(3,iMc,globalEnergy,1,0,0,outstr1.str(),plotdir));
  dataSetIsMc.push_back(0);
  dataSetHType.push_back(1);
  dataSetStyle.push_back(kCMSPoint);
  dataSetColor.push_back(kCMSColor);
  dataSetLegend.push_back("NONE");
  dataSetHisto.push_back("unfolding/nch_data_corrected");
  dataSetFactor.push_back(1000);


  if ( tracklets && ( globalEnergy == 0.9 || globalEnergy == 7 ) ) {
    // Tracklets
    dataSetId.push_back(-1);
    if ( globalEnergy == 0.9 ) dataSetFile.push_back("../expdata/Run124023-2.0.txt");
    if ( globalEnergy == 7.0 ) dataSetFile.push_back("../expdata/Run133242-2.0.txt");
    dataSetIsMc.push_back(0);
    dataSetHType.push_back(5);
    dataSetStyle.push_back(kTrackletsPoint);
    dataSetColor.push_back(kTrackletsColor);
    dataSetLegend.push_back("Tracklets");
    dataSetHisto.push_back("TRACKLETS");
    dataSetFactor.push_back(1000);
    // Syst on Tracklets
    dataSetId.push_back(-1);
    if ( globalEnergy == 0.9 ) dataSetFile.push_back("../expdata/Run124023-2.0.txt");
    if ( globalEnergy == 7.0 ) dataSetFile.push_back("../expdata/Run133242-2.0.txt");
    dataSetIsMc.push_back(0);
    dataSetHType.push_back(205);
    dataSetStyle.push_back(kTrackletsPoint);
    dataSetColor.push_back(kTrackletsColor);
    dataSetLegend.push_back("NONE");
    dataSetHisto.push_back("TRACKLETS");
    dataSetFactor.push_back(1000);
  }

  dataSetId.push_back(-1);
  dataSetFile.push_back(fileManager(3,iMc,globalEnergy,1,0,0,outstr1.str(),plotdir));
  dataSetIsMc.push_back(0);
  dataSetHType.push_back(205);
  dataSetStyle.push_back(kCMSPoint);
  dataSetColor.push_back(kCMSColor);
  dataSetLegend.push_back("NONE");
  dataSetHisto.push_back("unfolding/gnch_data_corrected_syst");
  dataSetFactor.push_back(1000);

  BinKillStat.push_back( dataSetId.size()-2 );
  BinKillSyst.push_back( dataSetId.size()-1 );

  // |eta| < 1.5

  if ( globalEnergy == 0.9 && iBin == 5 ) {
    if (! tracklets ) {
      // UA5
      dataSetId.push_back(-1);
      dataSetFile.push_back("../expdata/ua5_dsigdn_eta15");
      dataSetIsMc.push_back(0);
      dataSetHType.push_back(5);
      dataSetStyle.push_back(kUA5Point);
      dataSetColor.push_back(kUA5Color);
      dataSetLegend.push_back("UA5");
      dataSetHisto.push_back("UA5");
      dataSetFactor.push_back(100);
    } 
  } 

  if ( tracklets && ( globalEnergy == 0.9 || globalEnergy == 7 ) ) {
    // Tracklets
    dataSetId.push_back(-1);
    if ( globalEnergy == 0.9 ) dataSetFile.push_back("../expdata/Run124023-1.5.txt");
    if ( globalEnergy == 7.0 ) dataSetFile.push_back("../expdata/Run133242-1.5.txt");
    dataSetIsMc.push_back(0);
    dataSetHType.push_back(5);
    dataSetStyle.push_back(kTrackletsPoint);
    dataSetColor.push_back(kTrackletsColor);
    dataSetLegend.push_back("NONE");
    dataSetHisto.push_back("TRACKLETS");
    dataSetFactor.push_back(100);
    // Syst on Tracklets
    dataSetId.push_back(-1);
    if ( globalEnergy == 0.9 ) dataSetFile.push_back("../expdata/Run124023-1.5.txt");
    if ( globalEnergy == 7.0 ) dataSetFile.push_back("../expdata/Run133242-1.5.txt");
    dataSetIsMc.push_back(0);
    dataSetHType.push_back(205);
    dataSetStyle.push_back(kTrackletsPoint);
    dataSetColor.push_back(kTrackletsColor);
    dataSetLegend.push_back("NONE");
    dataSetHisto.push_back("TRACKLETS");
    dataSetFactor.push_back(100);
  }

  dataSetId.push_back(-1);
  dataSetFile.push_back(fileManager(3,iMc,globalEnergy,1,0,0,outstr2.str(),plotdir));
  dataSetIsMc.push_back(0);
  dataSetHType.push_back(1);
  dataSetStyle.push_back(kCMSPoint);
  dataSetColor.push_back(kCMSColor);
  dataSetLegend.push_back("NONE");
  dataSetHisto.push_back("unfolding/nch_data_corrected");
  dataSetFactor.push_back(100);

  dataSetId.push_back(-1);
  dataSetFile.push_back(fileManager(3,iMc,globalEnergy,1,0,0,outstr2.str(),plotdir));
  dataSetIsMc.push_back(0);
  dataSetHType.push_back(205);
  dataSetStyle.push_back(kCMSPoint);
  dataSetColor.push_back(kCMSColor);
  dataSetLegend.push_back("NONE");
  dataSetHisto.push_back("unfolding/gnch_data_corrected_syst");
  dataSetFactor.push_back(100);

  BinKillStat.push_back( dataSetId.size()-2 );
  BinKillSyst.push_back( dataSetId.size()-1 );

  // |eta| < 1.0

  if ( globalEnergy == 0.9 && iBin == 5 ) {
    if (! tracklets ) {
      // ALICE (stat)
      dataSetId.push_back(-1);
      dataSetFile.push_back("../expdata/alice_dsigdn_nsd_900GeV_eta10");
      dataSetIsMc.push_back(0);
      dataSetHType.push_back(5);
      dataSetStyle.push_back(kALICEPoint);
      dataSetColor.push_back(kALICEColor);
      dataSetLegend.push_back("ALICE");
      dataSetHisto.push_back("ALICE");
      dataSetFactor.push_back(10);
      // ALICE (stat+syst)
      dataSetId.push_back(-1);
      dataSetFile.push_back("../expdata/alice_dsigdn_nsd_900GeV_eta10");
      dataSetIsMc.push_back(0);
      dataSetHType.push_back(105);
      dataSetStyle.push_back(kALICEPoint);
      dataSetColor.push_back(kALICEColor);
      dataSetLegend.push_back("NONE");
      dataSetHisto.push_back("ALICE");
      dataSetFactor.push_back(10);
    }
  } 
  if ( globalEnergy == 2.36 && iBin == 5 ) {
    // ALICE (stat)
    dataSetId.push_back(-1);
    dataSetFile.push_back("../expdata/alice_dsigdn_nsd_2.36TeV_eta10");
    dataSetIsMc.push_back(0);
    dataSetHType.push_back(5);
    dataSetStyle.push_back(kALICEPoint);
    dataSetColor.push_back(kALICEColor);
    dataSetLegend.push_back("ALICE");
    dataSetHisto.push_back("ALICE");
    dataSetFactor.push_back(10);
    // ALICE (stat+syst)
    dataSetId.push_back(-1);
    dataSetFile.push_back("../expdata/alice_dsigdn_nsd_2.36TeV_eta10");
    dataSetIsMc.push_back(0);
    dataSetHType.push_back(105);
    dataSetStyle.push_back(kALICEPoint);
    dataSetColor.push_back(kALICEColor);
    dataSetLegend.push_back("NONE");
    dataSetHisto.push_back("ALICE");
    dataSetFactor.push_back(10);
  } 


  if ( tracklets && ( globalEnergy == 0.9 || globalEnergy == 7 ) ) {
    // Tracklets
    dataSetId.push_back(-1);
    if ( globalEnergy == 0.9 ) dataSetFile.push_back("../expdata/Run124023-1.0.txt");
    if ( globalEnergy == 7.0 ) dataSetFile.push_back("../expdata/Run133242-1.0.txt");
    dataSetIsMc.push_back(0);
    dataSetHType.push_back(5);
    dataSetStyle.push_back(kTrackletsPoint);
    dataSetColor.push_back(kTrackletsColor);
    dataSetLegend.push_back("NONE");
    dataSetHisto.push_back("TRACKLETS");
    dataSetFactor.push_back(10);
    // Syst on Tracklets
    dataSetId.push_back(-1);
    if ( globalEnergy == 0.9 ) dataSetFile.push_back("../expdata/Run124023-1.0.txt");
    if ( globalEnergy == 7.0 ) dataSetFile.push_back("../expdata/Run133242-1.0.txt");
    dataSetIsMc.push_back(0);
    dataSetHType.push_back(205);
    dataSetStyle.push_back(kTrackletsPoint);
    dataSetColor.push_back(kTrackletsColor);
    dataSetLegend.push_back("NONE");
    dataSetHisto.push_back("TRACKLETS");
    dataSetFactor.push_back(10);
  }

  dataSetId.push_back(-1);
  dataSetFile.push_back(fileManager(3,iMc,globalEnergy,1,0,0,outstr3.str(),plotdir));
  dataSetIsMc.push_back(0);
  dataSetHType.push_back(1);
  dataSetStyle.push_back(kCMSPoint);
  dataSetColor.push_back(kCMSColor);
  dataSetLegend.push_back("NONE");
  dataSetHisto.push_back("unfolding/nch_data_corrected");
  dataSetFactor.push_back(10);

  dataSetId.push_back(-1);
  dataSetFile.push_back(fileManager(3,iMc,globalEnergy,1,0,0,outstr3.str(),plotdir));
  dataSetIsMc.push_back(0);
  dataSetHType.push_back(205);
  dataSetStyle.push_back(kCMSPoint);
  dataSetColor.push_back(kCMSColor);
  dataSetLegend.push_back("NONE");
  dataSetHisto.push_back("unfolding/gnch_data_corrected_syst");
  dataSetFactor.push_back(10);

  BinKillStat.push_back( dataSetId.size()-2 );
  BinKillSyst.push_back( dataSetId.size()-1 );
  // |eta| < 0.5

  if ( globalEnergy == 0.9 && iBin == 5 ) {
    if (! tracklets ) {
      // UA5
      dataSetId.push_back(-1);
      dataSetFile.push_back("../expdata/ua5_dsigdn_eta05");
      dataSetIsMc.push_back(0);
      dataSetHType.push_back(5);
      dataSetStyle.push_back(kUA5Point);
      dataSetColor.push_back(kUA5Color);
      dataSetLegend.push_back("NONE");
      dataSetHisto.push_back("UA5");
      dataSetFactor.push_back(1);
    }
  } 

  if ( globalEnergy == 0.9 && iBin == 5 ) {
    if (! tracklets ) {
      // ALICE (stat)
      dataSetId.push_back(-1);
      dataSetFile.push_back("../expdata/alice_dsigdn_nsd_900GeV_eta05");
      dataSetIsMc.push_back(0);
      dataSetHType.push_back(5);
      dataSetStyle.push_back(kALICEPoint);
      dataSetColor.push_back(kALICEColor);
      dataSetLegend.push_back("NONE");
      dataSetHisto.push_back("ALICE");
      dataSetFactor.push_back(1);
      // ALICE (stat+syst)
      dataSetId.push_back(-1);
      dataSetFile.push_back("../expdata/alice_dsigdn_nsd_900GeV_eta05");
      dataSetIsMc.push_back(0);
      dataSetHType.push_back(105);
      dataSetStyle.push_back(kALICEPoint);
      dataSetColor.push_back(kALICEColor);
      dataSetLegend.push_back("NONE");
      dataSetHisto.push_back("ALICE");
      dataSetFactor.push_back(1);
    } 
  } 
  if ( globalEnergy == 2.36 && iBin == 5 ) {
    // ALICE (stat)
    dataSetId.push_back(-1);
    dataSetFile.push_back("../expdata/alice_dsigdn_nsd_2.36TeV_eta05");
    dataSetIsMc.push_back(0);
    dataSetHType.push_back(5);
    dataSetStyle.push_back(kALICEPoint);
    dataSetColor.push_back(kALICEColor);
    dataSetLegend.push_back("NONE");
    dataSetHisto.push_back("ALICE");
    dataSetFactor.push_back(1);
    // ALICE (stat+syst)
    dataSetId.push_back(-1);
    dataSetFile.push_back("../expdata/alice_dsigdn_nsd_2.36TeV_eta05");
    dataSetIsMc.push_back(0);
    dataSetHType.push_back(105);
    dataSetStyle.push_back(kALICEPoint);
    dataSetColor.push_back(kALICEColor);
    dataSetLegend.push_back("NONE");
    dataSetHisto.push_back("ALICE");
    dataSetFactor.push_back(1);
  } 


  if ( tracklets && ( globalEnergy == 0.9 || globalEnergy == 7 ) ) {
    // Tracklets
    dataSetId.push_back(-1);
    if ( globalEnergy == 0.9 ) dataSetFile.push_back("../expdata/Run124023-0.5.txt");
    if ( globalEnergy == 7.0 ) dataSetFile.push_back("../expdata/Run133242-0.5.txt");
    dataSetIsMc.push_back(0);
    dataSetHType.push_back(5);
    dataSetStyle.push_back(kTrackletsPoint);
    dataSetColor.push_back(kTrackletsColor);
    dataSetLegend.push_back("NONE");
    dataSetHisto.push_back("TRACKLETS");
    dataSetFactor.push_back(1);
    // Syst on Tracklets
    dataSetId.push_back(-1);
    if ( globalEnergy == 0.9 ) dataSetFile.push_back("../expdata/Run124023-0.5.txt");
    if ( globalEnergy == 7.0 ) dataSetFile.push_back("../expdata/Run133242-0.5.txt");
    dataSetIsMc.push_back(0);
    dataSetHType.push_back(205);
    dataSetStyle.push_back(kTrackletsPoint);
    dataSetColor.push_back(kTrackletsColor);
    dataSetLegend.push_back("NONE");
    dataSetHisto.push_back("TRACKLETS");
    dataSetFactor.push_back(1);
  }

  dataSetId.push_back(-1);
  dataSetFile.push_back(fileManager(3,iMc,globalEnergy,1,0,0,outstr4.str(),plotdir));
  dataSetIsMc.push_back(0);
  dataSetHType.push_back(1);
  dataSetStyle.push_back(kCMSPoint);
  dataSetColor.push_back(kCMSColor);
  dataSetLegend.push_back("NONE");
  dataSetHisto.push_back("unfolding/nch_data_corrected");
  dataSetFactor.push_back(1);

  dataSetId.push_back(-1);
  dataSetFile.push_back(fileManager(3,iMc,globalEnergy,1,0,0,outstr4.str(),plotdir));
  dataSetIsMc.push_back(0);
  dataSetHType.push_back(205);
  dataSetStyle.push_back(kCMSPoint);
  dataSetColor.push_back(kCMSColor);
  dataSetLegend.push_back("NONE");
  dataSetHisto.push_back("unfolding/gnch_data_corrected_syst");
  dataSetFactor.push_back(1);

  BinKillStat.push_back( dataSetId.size()-2 );
  BinKillSyst.push_back( dataSetId.size()-1 );
  // Curve legends

  Float_t xt1 , yt1  ;
  Float_t xt2 , yt2  ;
  Float_t xt3 , yt3  ;
  Float_t xt4 , yt4  ;
  Float_t xt5 , yt5  ;

  if ( iBin == 5 ) {
    if ( Energy == 0.9 ) {
     xt1 = .58; yt1 = .80;
     xt2 = .53; yt2 = .68;
     xt3 = .50; yt3 = .57;
     xt4 = .38; yt4 = .49;
     xt5 = .27; yt5 = .41;
    }

    if ( Energy == 2.36 ) {
     xt1 = .58; yt1 = .82;
     xt2 = .53; yt2 = .70;
     xt3 = .50; yt3 = .57;
     xt4 = .38; yt4 = .49;
     xt5 = .27; yt5 = .41;
    }

    if ( Energy == 7.0 ) {
     xt1 = .58; yt1 = .81;
     xt2 = .53; yt2 = .72;
     xt3 = .50; yt3 = .62;
     xt4 = .38; yt4 = .55;
     xt5 = .27; yt5 = .47;
    }

  } else {
/*
    if ( Energy == 0.9 ) {
     xt1 = .48; yt1 = .82;
     xt2 = .43; yt2 = .75;
     xt3 = .36; yt3 = .68;
     xt4 = .32; yt4 = .62;
     xt5 = .25; yt5 = .55;
    }

    if ( Energy == 2.36 ) {
     xt1 = .48; yt1 = .79;
     xt2 = .43; yt2 = .71;
     xt3 = .36; yt3 = .66;
     xt4 = .32; yt4 = .58;
     xt5 = .25; yt5 = .52;
    }

    if ( Energy == 7.0 ) {
     xt1 = .48; yt1 = .77;
     xt2 = .43; yt2 = .70;
     xt3 = .36; yt3 = .65;
     xt4 = .32; yt4 = .57;
     xt5 = .25; yt5 = .50;
    }
*/
    if ( Energy == 0.9 ) {
     xt1 = .58; yt1 = .75;
     xt2 = .53; yt2 = .63;
     xt3 = .50; yt3 = .51;
     xt4 = .38; yt4 = .45;
     xt5 = .27; yt5 = .41;
    }

    if ( Energy == 2.36 ) {
     xt1 = .58; yt1 = .75;
     xt2 = .53; yt2 = .63;
     xt3 = .51; yt3 = .51;
     xt4 = .38; yt4 = .45;
     xt5 = .25; yt5 = .41;


    }

    if ( Energy == 7.0 ) {

     xt1 = .58; yt1 = .75;
     xt2 = .53; yt2 = .66;
     xt3 = .50; yt3 = .56;
     xt4 = .38; yt4 = .51;
     xt5 = .27; yt5 = .46;

    }



  }

  globalTextSize = 0.03;

  textXPos.push_back(xt1)  ; textYPos.push_back(yt1) ;
  textXPos.push_back(xt2)  ; textYPos.push_back(yt2) ;
  textXPos.push_back(xt3)  ; textYPos.push_back(yt3) ;
  textXPos.push_back(xt4)  ; textYPos.push_back(yt4) ;
  textXPos.push_back(xt5)  ; textYPos.push_back(yt5) ;

  textText.push_back("|#eta| < 2.4 (x10000)");
  textText.push_back("|#eta| < 2.0 (x1000)");
  textText.push_back("|#eta| < 1,5 (x100)");
  textText.push_back("|#eta| < 1.0 (x10)");
  textText.push_back("|#eta| < 0.5 (x1)");

   


  // Point Legend

  if ( iBin == 15 ) {
    textXPos.push_back(0.16) ; textYPos.push_back(.2) ;
    textText.push_back("p_{T} > 0.5 GeV/c");
  }

  // if ( iBin == 15 ) LegendTitle = "p_{t} > 0.5 GeV/c";

   yLegendWidth = 0.04;
  xLegendWidth = .1 ;


  // PLOT NOW

  plot("none","AUTO",1,7);


}


    








