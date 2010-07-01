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

void nchdiff(int iBin = 0 , double energy = 0.9 , bool norwght =false , int iSaveFig = 0, float npx = 1000 , float npy = 600){


  globalEnergy = energy;
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
  globalFigBaseName = "test";

  globalNorm = 2;

  int iMc;
  if ( globalEnergy == 0.9 ) iMc = 10 ;
  if ( globalEnergy == 2.36) iMc = 10 ;
  if ( globalEnergy == 7.0 ) iMc = 31 ;

  TString plotdir(""); 
  if ( globalEnergy == 0.9 ) plotdir = "../plots/systv10_niter100_2/";
  if ( globalEnergy == 2.36) plotdir = "../plots/systv10_niter100_2/";
  if ( globalEnergy == 7.0 ) plotdir = "../plots/systv10_niter100_2/";
  
  ostringstream outstr("");
  outstr << "hyp" << 1 << "_niter" << 0 << "_cut" << iBin << "_DataType" << 0;

  ostringstream mchisto("");
  mchisto << "nch_gen_NSD_noSel_cut" << iBin;


 // Points and Colors for Experiments

  int kCMSPoint = kOpenCircle ;
  int kCMSColor = kBlack;

  int kTrackletsPoint = kOpenCircle ;
  int kTrackletsColor = kGreen;

  int kUA1Point = kOpenStar ;
  int kUA1Color = kBlue;

  int kUA5Point = kOpenTriangleUp ;
  int kUA5Color = kRed;

  int kALICEPoint = kOpenDiamond ;
  int kALICEColor = kMagenta ;

  int kATLASPoint = kOpenTriangleUp ;
  int kATLASColor = kBlue;


  // ------ Start Plotting ------------------------------------------------------


  if (norwght) {
    dataSetId.push_back(-1);
    dataSetFile.push_back(fileManager(3,iMc,globalEnergy,1,0,0,outstr.str(),plotdir));
    dataSetIsMc.push_back(0);
    dataSetHType.push_back(1);
    dataSetStyle.push_back(kCMSPoint);
    dataSetColor.push_back(kCMSColor);
    dataSetLegend.push_back("Tracks");
    dataSetHisto.push_back("unfolding/nch_data_corrected");

    dataSetId.push_back(-1);
    dataSetFile.push_back(fileManager(3,iMc,globalEnergy,1,0,0,outstr.str(),plotdir));
    dataSetIsMc.push_back(0);
    dataSetHType.push_back(205);
    dataSetStyle.push_back(kCMSPoint);
    dataSetColor.push_back(kCMSColor);
    dataSetLegend.push_back("NONE");
    dataSetHisto.push_back("unfolding/gnch_data_corrected_syst");


  } else {

    dataSetId.push_back(-1);
    // dataSetFile.push_back(fileManager(3,iMc,globalEnergy,1,0,0,outstr.str(),"/user/rougny/Ferenc_Tracking_bis/CMSSW_3_3_6_patch3/src/UAmulti/ChPartAna/plots/systv10_13/"));
    dataSetFile.push_back(fileManager(3,iMc,globalEnergy,1,0,0,outstr.str(),"/user/rougny/Ferenc_Tracking_bis/CMSSW_3_3_6_patch3/src/UAmulti/ChPartAna/plots/"));
    dataSetIsMc.push_back(0);
    dataSetHType.push_back(1);
    dataSetStyle.push_back(kCMSPoint);
    dataSetColor.push_back(kBlue);
    dataSetLegend.push_back("Tracks-Rwght");
    dataSetHisto.push_back("unfolding/nch_data_corrected");


/*
  dataSetId.push_back(-1);
  dataSetFile.push_back(fileManager(3,iMc,globalEnergy,1,0,0,outstr.str(),"/user/rougny/Ferenc_Tracking_bis/CMSSW_3_3_6_patch3/src/UAmulti/ChPartAna/plots/"));
  dataSetIsMc.push_back(0);
  dataSetHType.push_back(205);
  dataSetStyle.push_back(kCMSPoint);
  dataSetColor.push_back(kCMSColor);
  dataSetLegend.push_back("NONE");
  dataSetHisto.push_back("unfolding/gnch_data_corrected_syst");
*/

  }
  
  globalRatioType = 2;
  if ( globalEnergy == 7.0 ) globalRatioType = 1;

  globalRatioBase = 0;


  if (    ( globalEnergy == 0.9 || globalEnergy == 7.0 )
       && ( iBin == 6 || iBin == 7 || iBin == 8 || iBin == 9 ) ) {
  
    dataSetId.push_back(-1);
    if        ( iBin == 6 ) {
      if ( globalEnergy == 0.9 ) dataSetFile.push_back("../expdata/Run124023-2.0.txt");
      if ( globalEnergy == 7.0 ) dataSetFile.push_back("../expdata/Run133242-2.0.txt");
    } else if ( iBin == 7 ) {
      if ( globalEnergy == 0.9 ) dataSetFile.push_back("../expdata/Run124023-1.5.txt");
      if ( globalEnergy == 7.0 ) dataSetFile.push_back("../expdata/Run133242-1.5.txt");
    } else if ( iBin == 8 ) {
      if ( globalEnergy == 0.9 ) dataSetFile.push_back("../expdata/Run124023-1.0.txt");
      if ( globalEnergy == 7.0 ) dataSetFile.push_back("../expdata/Run133242-1.0.txt");
    } else if ( iBin == 9 ) {
      if ( globalEnergy == 0.9 ) dataSetFile.push_back("../expdata/Run124023-0.5.txt");
      if ( globalEnergy == 7.0 ) dataSetFile.push_back("../expdata/Run133242-0.5.txt");
    } 
    dataSetIsMc.push_back(0);
    dataSetHType.push_back(5);
    dataSetStyle.push_back(kTrackletsPoint);
    dataSetColor.push_back(kTrackletsColor);
    dataSetLegend.push_back("Tracklets");
    dataSetHisto.push_back("TRACKLETS");

    dataSetId.push_back(-1);
    if        ( iBin == 6 ) {
      if ( globalEnergy == 0.9 ) dataSetFile.push_back("../expdata/Run124023-2.0.txt");
      if ( globalEnergy == 7.0 ) dataSetFile.push_back("../expdata/Run133242-2.0.txt");
    } else if ( iBin == 7 ) {
      if ( globalEnergy == 0.9 ) dataSetFile.push_back("../expdata/Run124023-1.5.txt");
      if ( globalEnergy == 7.0 ) dataSetFile.push_back("../expdata/Run133242-1.5.txt");
    } else if ( iBin == 8 ) {
      if ( globalEnergy == 0.9 ) dataSetFile.push_back("../expdata/Run124023-1.0.txt");
      if ( globalEnergy == 7.0 ) dataSetFile.push_back("../expdata/Run133242-1.0.txt");
    } else if ( iBin == 9 ) {
      if ( globalEnergy == 0.9 ) dataSetFile.push_back("../expdata/Run124023-0.5.txt");
      if ( globalEnergy == 7.0 ) dataSetFile.push_back("../expdata/Run133242-0.5.txt");
    }     
    dataSetIsMc.push_back(0);
    dataSetHType.push_back(205);
    dataSetStyle.push_back(kTrackletsPoint);
    dataSetColor.push_back(kTrackletsColor);
    dataSetLegend.push_back("NONE");
    dataSetHisto.push_back("TRACKLETS");
  }

  // OTHER DATA
 
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

    dataSetId.push_back(-1);
    dataSetFile.push_back("../expdata/atlas_dsigdn_nsd_900GeV_eta25_pt500");
    dataSetIsMc.push_back(0);
    dataSetHType.push_back(105);
    dataSetStyle.push_back(kOpenTriangleUp);
    dataSetColor.push_back(kBlue);
    dataSetLegend.push_back("NONE");
    dataSetHisto.push_back("ALICE");
  }

  // |eta| < 1.5

  if ( globalEnergy == 0.9 && iBin == 7 ) {
      // UA5
      dataSetId.push_back(-1);
      dataSetFile.push_back("../expdata/ua5_dsigdn_eta15");
      dataSetIsMc.push_back(0);
      dataSetHType.push_back(5);
      dataSetStyle.push_back(kUA5Point);
      dataSetColor.push_back(kUA5Color);
      dataSetLegend.push_back("UA5");
      dataSetHisto.push_back("UA5");
  }

  // |eta| < 1.0

  if ( globalEnergy == 0.9 && iBin == 8 ) {
      // ALICE (stat)
      dataSetId.push_back(-1);
      dataSetFile.push_back("../expdata/alice_dsigdn_nsd_900GeV_eta10");
      dataSetIsMc.push_back(0);
      dataSetHType.push_back(5);
      dataSetStyle.push_back(kALICEPoint);
      dataSetColor.push_back(kALICEColor);
      dataSetLegend.push_back("ALICE");
      dataSetHisto.push_back("ALICE");
      // ALICE (stat+syst)
      dataSetId.push_back(-1);
      dataSetFile.push_back("../expdata/alice_dsigdn_nsd_900GeV_eta10");
      dataSetIsMc.push_back(0);
      dataSetHType.push_back(105);
      dataSetStyle.push_back(kALICEPoint);
      dataSetColor.push_back(kALICEColor);
      dataSetLegend.push_back("NONE");
      dataSetHisto.push_back("ALICE");
  }

  if ( globalEnergy == 2.36 && iBin == 8 ) {
    // ALICE (stat)
    dataSetId.push_back(-1);
    dataSetFile.push_back("../expdata/alice_dsigdn_nsd_2.36TeV_eta10");
    dataSetIsMc.push_back(0);
    dataSetHType.push_back(5);
    dataSetStyle.push_back(kALICEPoint);
    dataSetColor.push_back(kALICEColor);
    dataSetLegend.push_back("ALICE");
    dataSetHisto.push_back("ALICE");
    // ALICE (stat+syst)
    dataSetId.push_back(-1);
    dataSetFile.push_back("../expdata/alice_dsigdn_nsd_2.36TeV_eta10");
    dataSetIsMc.push_back(0);
    dataSetHType.push_back(105);
    dataSetStyle.push_back(kALICEPoint);
    dataSetColor.push_back(kALICEColor);
    dataSetLegend.push_back("NONE");
    dataSetHisto.push_back("ALICE");
  }

  // |eta| < 0.5

  if ( globalEnergy == 0.9 && iBin == 9 ) {
      // UA5
      dataSetId.push_back(-1);
      dataSetFile.push_back("../expdata/ua5_dsigdn_eta05");
      dataSetIsMc.push_back(0);
      dataSetHType.push_back(5);
      dataSetStyle.push_back(kUA5Point);
      dataSetColor.push_back(kUA5Color);
      dataSetLegend.push_back("UA5");
      dataSetHisto.push_back("UA5");
  }

  if ( globalEnergy == 0.9 && iBin == 9 ) {
      // ALICE (stat)
      dataSetId.push_back(-1);
      dataSetFile.push_back("../expdata/alice_dsigdn_nsd_900GeV_eta05");
      dataSetIsMc.push_back(0);
      dataSetHType.push_back(5);
      dataSetStyle.push_back(kALICEPoint);
      dataSetColor.push_back(kALICEColor);
      dataSetLegend.push_back("ALICE");
      dataSetHisto.push_back("ALICE");
      // ALICE (stat+syst)
      dataSetId.push_back(-1);
      dataSetFile.push_back("../expdata/alice_dsigdn_nsd_900GeV_eta05");
      dataSetIsMc.push_back(0);
      dataSetHType.push_back(105);
      dataSetStyle.push_back(kALICEPoint);
      dataSetColor.push_back(kALICEColor);
      dataSetLegend.push_back("NONE");
      dataSetHisto.push_back("ALICE");
  }
  if ( globalEnergy == 2.36 && iBin == 9 ) {
    // ALICE (stat)
    dataSetId.push_back(-1);
    dataSetFile.push_back("../expdata/alice_dsigdn_nsd_2.36TeV_eta05");
    dataSetIsMc.push_back(0);
    dataSetHType.push_back(5);
    dataSetStyle.push_back(kALICEPoint);
    dataSetColor.push_back(kALICEColor);
    dataSetLegend.push_back("ALICE");
    dataSetHisto.push_back("ALICE");
    // ALICE (stat+syst)
    dataSetId.push_back(-1);
    dataSetFile.push_back("../expdata/alice_dsigdn_nsd_2.36TeV_eta05");
    dataSetIsMc.push_back(0);
    dataSetHType.push_back(105);
    dataSetStyle.push_back(kALICEPoint);
    dataSetColor.push_back(kALICEColor);
    dataSetLegend.push_back("NONE");
    dataSetHisto.push_back("ALICE");
  }




  // PLOT NOW

  histoYMax = 1;
  histoYMin = 0.0000001;

  plot("none","AUTO",1,0);


}






