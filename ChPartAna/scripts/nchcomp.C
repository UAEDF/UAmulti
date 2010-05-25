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
#include "plotNew.C" 


// iStab = 1 : + and -
// iStab = 2 : hypothese
// iStab = 3 : Data        with 7 TeV ATLAS unfolding (syst 401)
// iStab = 4 : Data        with 7 TeV D6T   unfolding (syst 402)
// iStab = 5 : Data        with 7 TeV ATLAS unfolding (syst 401)
// iStab = 6 : Data        with 7 TeV D6T   unfolding (syst 402)


// iStab = 7 : PHOJET      with       D6T   unfolding (syst    )  7 TeV only !!!
// iStab = 8 : PHOJET      with       ATLAS unfolding (syst    )  7 TeV only !!!

  
void nchcomp(int iBin = 0 , double energy = 0.9, int iSaveFig = 0, float npx = 1000 , float npy = 600){



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
  histoYMin  = 0.000001 ;
  histoYMax  = 1;
  histoXMin  = -0.5 ;

  if ( globalEnergy == 0.9 ) histoXMax  = 100.5;
  if ( globalEnergy == 2.36) histoXMax  = 140.5;
  if ( globalEnergy == 7.0 ) histoXMax  = 180.5;

  TString plotdir("");

  int iData1,iMc1,iSyst1,iSign1;
  int iData2,iMc2,iSyst2,iSign2;
  ostringstream outstr1("");
  ostringstream outstr2("");
  ostringstream legend1("");
  ostringstream legend2("");


     iData1 = 0 ;
     iData2 = 0 ;
     iMc1   = 10;
     iMc2   = 10;
     if ( globalEnergy == 7.0 ) { iMc1 = 31 ; iMc2 = 31 ; }
     iSyst1 =  0;
     iSyst2 =  0;  
     iSign1 =  0;
     iSign2 =  0;
     outstr1 << "hyp" << 1 << "_niter" << 0 << "_cut" << iBin << "_DataType" << iData1;
     outstr2 << "hyp" << 1 << "_niter" << 0 << "_cut" << iBin << "_DataType" << iData2;
     legend1 << "Data: v9";
     legend2 << "Data: v10";

     if ( globalEnergy == 0.9 ) histoXMax  = 80.5;
     if ( globalEnergy == 2.36) histoXMax  =120.5;
     if ( globalEnergy == 7.0 ) histoXMax  =180.5;

/*
     if ( globalEnergy == 0.9 ) plotdir = "../plots/finalv9/";
     if ( globalEnergy == 2.36) plotdir = "../plots/finalv9/";
     if ( globalEnergy == 7.0 ) plotdir = "../plots/finalv9/";

*/
     if ( globalEnergy == 0.9 ) globalFigBaseName = "comp_2version_09TeV";
     if ( globalEnergy == 2.36) globalFigBaseName = "comp_2version_236TeV";
     if ( globalEnergy == 7.0 ) globalFigBaseName = "comp_2version_7TeV";



  // define plots 

  plotdir = "../plots/finalv9/";
  if ( globalEnergy == 7.0 ) plotdir = "../plots/systv10/";


/*
  dataSetId.push_back(-1);
  dataSetFile.push_back(fileManager(3,iMc1,globalEnergy,1,iSyst1,iSign1,outstr1.str(),plotdir));
  dataSetIsMc.push_back(0);
  dataSetHType.push_back(1);
  dataSetStyle.push_back(kOpenCircle);
  dataSetColor.push_back(kWhite);
  dataSetLegend.push_back("NONE");
  dataSetHisto.push_back("unfolding/nch_data_corrected");
*/

  dataSetId.push_back(-1);
  dataSetFile.push_back(fileManager(3,iMc1,globalEnergy,1,iSyst1,iSign1,outstr1.str(),plotdir));
  dataSetIsMc.push_back(0);
  dataSetHType.push_back(5);
  dataSetStyle.push_back(kOpenCircle);
  dataSetColor.push_back(kBlue);
  dataSetLegend.push_back(legend1.str());
  dataSetHisto.push_back("unfolding/gnch_data_corrected");

  dataSetId.push_back(-1);
  dataSetFile.push_back(fileManager(3,iMc1,globalEnergy,1,iSyst1,iSign1,outstr1.str(),plotdir));
  dataSetIsMc.push_back(0);
  dataSetHType.push_back(105);
  dataSetStyle.push_back(kOpenCircle);
  dataSetColor.push_back(kBlue);
  dataSetLegend.push_back("NONE");
  dataSetHisto.push_back("unfolding/gnch_data_corrected_syst");


  plotdir = "../plots/systv10/"; 
  if ( globalEnergy == 7.0 ) plotdir = "../plots/systv11/";

  dataSetId.push_back(-1);
  dataSetFile.push_back(fileManager(3,iMc2,globalEnergy,1,iSyst2,iSign2,outstr2.str(),plotdir));
  dataSetIsMc.push_back(0);
  dataSetHType.push_back(5);
  dataSetStyle.push_back(kOpenCircle);
  dataSetColor.push_back(kRed);
  dataSetLegend.push_back(legend2.str());
  dataSetHisto.push_back("unfolding/gnch_data_corrected");

  dataSetId.push_back(-1);
  dataSetFile.push_back(fileManager(3,iMc2,globalEnergy,1,iSyst2,iSign2,outstr2.str(),plotdir));
  dataSetIsMc.push_back(0);
  dataSetHType.push_back(105);
  dataSetStyle.push_back(kOpenCircle);
  dataSetColor.push_back(kRed);
  dataSetLegend.push_back("NONE");
  dataSetHisto.push_back("unfolding/gnch_data_corrected_syst");


  // Plot 

  plot("none","AUTO",1,0);

}


