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


// iStab = 1 : + and -
// iStab = 2 : hypothese
// iStab = 3 : Data        with 7 TeV ATLAS unfolding (syst 401)
// iStab = 4 : Data        with 7 TeV D6T   unfolding (syst 402)
// iStab = 5 : Data        with 7 TeV ATLAS unfolding (syst 401)


// iStab = 7 : MC PHOJET   with       D6T   unfolding (syst 402)  7 TeV only !!!
// iStab = 8 : MC PHOJET   with       ATLAS unfolding (syst 401)  7 TeV only !!!

  
void nchstab(int iStab = 0 , double energy = 0.9, int iSaveFig = 1, float npx = 1000 , float npy = 600){



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
  histoYMin  = 0.00000001 ;
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
  
  TString plot1 = "unfolding/nch_data_corrected";
  TString plot2 = "unfolding/nch_data_corrected";

  if ( iStab == 1 ) {

     LegendTitle = "        |#eta| < 2.4  p_{t} > 0.5 GeV/c";

     iData1 = 0 ;
     iData2 = 0 ;
     iMc1   = 10;
     iMc2   = 10;
     if ( globalEnergy == 7.0 ) { iMc1 = 31 ; iMc2 = 31 ; }
     iSyst1 =  0;
     iSyst2 =  0;  
     iSign1 =  0;
     iSign2 =  0;
     outstr1 << "hyp" << 1 << "_niter" << 0 << "_cut" << 26 << "_DataType" << iData1;
     outstr2 << "hyp" << 1 << "_niter" << 0 << "_cut" << 27 << "_DataType" << iData2;
     legend1 << "Data: Charge-";
     legend2 << "Data: Charge+";

     if ( globalEnergy == 0.9 ) histoXMax  = 30.5;
     if ( globalEnergy == 2.36) histoXMax  = 40.5;
     if ( globalEnergy == 7.0 ) histoXMax  = 70.5;

     if ( globalEnergy == 0.9 ) plotdir = "../plots/finalv9/";
     if ( globalEnergy == 2.36) plotdir = "../plots/finalv9/";
     if ( globalEnergy == 7.0 ) plotdir = "../plots/finalv9/";

     if ( globalEnergy == 0.9 ) globalFigBaseName = "nchstab_charge_09TeV";
     if ( globalEnergy == 2.36) globalFigBaseName = "nchstab_charge_236TeV";
     if ( globalEnergy == 7.0 ) globalFigBaseName = "nchstab_charge_7TeV";

  }

  if ( iStab == 2 ) {

     LegendTitle = "        |#eta| < 2.4";

     iData1 = 0 ;
     iData2 = 0 ;
     iMc1   = 10;
     iMc2   = 10;
     if ( globalEnergy == 7.0 ) { iMc1 = 31 ; iMc2 = 31 ; }
     iSyst1 =  0;
     iSyst2 =  0;
     iSign1 =  0;
     iSign2 =  0;
     outstr1 << "hyp" << 0 << "_niter" << 0 << "_cut" << 5 << "_DataType" << iData1;
     outstr2 << "hyp" << 1 << "_niter" << 0 << "_cut" << 5 << "_DataType" << iData2;
     legend1 << "Data: hyp=0";
     legend2 << "Data: hyp=1";

     histoYMax  = 1. ; 
     histoYMin  = 0.000001 ;
     if ( globalEnergy == 0.9 ) histoXMax  = 100.5;
     if ( globalEnergy == 2.36) histoXMax  = 120.5;
     if ( globalEnergy == 7.0 ) histoXMax  = 180.5;

     if ( globalEnergy == 0.9 ) plotdir = "../plots/systv10/";
     if ( globalEnergy == 2.36) plotdir = "../plots/systv10/";
     if ( globalEnergy == 7.0 ) plotdir = "../plots/systv10/";

     if ( globalEnergy == 0.9 ) globalFigBaseName = "nchstab_hyp_09TeV";
     if ( globalEnergy == 2.36) globalFigBaseName = "nchstab_hyp_236TeV";
     if ( globalEnergy == 7.0 ) globalFigBaseName = "nchstab_hyp_7TeV";

  }


 if ( iStab == 3 ) {

     LegendTitle = "        |#eta| < 2.4";

     iData1 = 0 ;
     iData2 = 0 ;
     iMc1   = 10;
     iMc2   = 10;
     if ( globalEnergy == 7.0 ) { iMc1 = 31 ; iMc2 = 31 ; }
     iSyst1 =  0;
     iSyst2 =  401;
     iSign1 =  0;
     iSign2 =  -1;
     outstr1 << "hyp" << 1 << "_niter" << 0 << "_cut" << 5 << "_DataType" << iData1;
     outstr2 << "hyp" << 1 << "_niter" << 0 << "_cut" << 5 << "_DataType" << iData2;
     legend1 << "Data ";
     legend2 << "Data Unf=ATLAS@7 TeV";

     histoYMax  = 1. ;
     histoYMin  = 0.000001 ;
     if ( globalEnergy == 0.9 ) histoXMax  = 100.5;
     if ( globalEnergy == 2.36) histoXMax  = 120.5;
     if ( globalEnergy == 7.0 ) histoXMax  = 180.5;

     if ( globalEnergy == 0.9 ) plotdir = "../plots/systv10/";
     if ( globalEnergy == 2.36) plotdir = "../plots/systv10/";
     if ( globalEnergy == 7.0 ) plotdir = "../plots/systv10/";

     if ( globalEnergy == 0.9 ) globalFigBaseName = "nchstab_syst401_09TeV";
     if ( globalEnergy == 2.36) globalFigBaseName = "nchstab_syst401_236TeV";
     if ( globalEnergy == 7.0 ) globalFigBaseName = "nchstab_syst401_7TeV";

  }

  if ( iStab == 4 ) {

     LegendTitle = "        |#eta| < 2.4";

     iData1 = 0 ;
     iData2 = 0 ;
     iMc1   = 10;
     iMc2   = 10;
     if ( globalEnergy == 7.0 ) { iMc1 = 31 ; iMc2 = 31 ; }
     iSyst1 =  0;
     iSyst2 =  402;
     iSign1 =  0;
     iSign2 =  -1;
     outstr1 << "hyp" << 1 << "_niter" << 0 << "_cut" << 5 << "_DataType" << iData1;
     outstr2 << "hyp" << 1 << "_niter" << 0 << "_cut" << 5 << "_DataType" << iData2;
     legend1 << "Data ";
     legend2 << "Data Unf=D6T@7 TeV";

     histoYMax  = 1. ;
     histoYMin  = 0.000001 ;
     if ( globalEnergy == 0.9 ) histoXMax  = 100.5;
     if ( globalEnergy == 2.36) histoXMax  = 120.5;
     if ( globalEnergy == 7.0 ) histoXMax  = 180.5;

     if ( globalEnergy == 0.9 ) plotdir = "../plots/systv10/";
     if ( globalEnergy == 2.36) plotdir = "../plots/systv10/";
     if ( globalEnergy == 7.0 ) plotdir = "../plots/systv10/";

     if ( globalEnergy == 0.9 ) globalFigBaseName = "nchstab_syst402_09TeV";
     if ( globalEnergy == 2.36) globalFigBaseName = "nchstab_syst402_236TeV";
     if ( globalEnergy == 7.0 ) globalFigBaseName = "nchstab_syst402_7TeV";

  }


  if ( iStab == 5 ) {

     LegendTitle = "        |#eta| < 2.4";

     iData1 = 10 ;
     iData2 = 10 ;
     iMc1   = 10;
     iMc2   = 10;
     if ( globalEnergy == 7.0 ) { iMc1 = 31 ; iMc2 = 31 ; }
     iSyst1 =  0;
     iSyst2 =  0;
     iSign1 =  -1;
     iSign2 =  -1;
     outstr1 << "hyp" << 1 << "_niter" << 0 << "_cut" << 5 << "_DataType" << iData1;
     outstr2 << "hyp" << 1 << "_niter" << 0 << "_cut" << 5 << "_DataType" << iData2;
     legend1 << "D6T ";
     legend2 << "D6T Unf=D6T@7 TeV";

     histoYMax  = 1. ;
     histoYMin  = 0.000001 ;
     if ( globalEnergy == 0.9 ) histoXMax  = 100.5;
     if ( globalEnergy == 2.36) histoXMax  = 120.5;
     if ( globalEnergy == 7.0 ) histoXMax  = 180.5;

     if ( globalEnergy == 0.9 ) plotdir = "../plots/systv10/";
     if ( globalEnergy == 2.36) plotdir = "../plots/systv10/";
     if ( globalEnergy == 7.0 ) plotdir = "../plots/systv10/";
     
     if ( globalEnergy == 0.9 ) globalFigBaseName = "nchstab_d6t_syst402_09TeV";
     if ( globalEnergy == 2.36) globalFigBaseName = "nchstab_d6t_syst402_236TeV";
     if ( globalEnergy == 7.0 ) globalFigBaseName = "nchstab_d6t_syst402_7TeV";

 plotdir = "/user/rougny/Ferenc_Tracking_bis/CMSSW_3_3_6_patch3/src/UAmulti/ChPartAna/plots/";

  }

  if ( iStab == 7 ) {

     LegendTitle = "        |#eta| < 2.4";

     iData1 = 10 ;
     iData2 = 10 ;
     iMc1   = 10;
     iMc2   = 10;
     if ( globalEnergy == 7.0 ) { return; }
     iSyst1 =  401;
     iSyst2 =  401;
     iSign1 =  -1;
     iSign2 =  -1;
     outstr1 << "hyp" << 1 << "_niter" << 0 << "_cut" << 5 << "_DataType" << iData1;
     outstr2 << "hyp" << 1 << "_niter" << 0 << "_cut" << 5 << "_DataType" << iData2;
     legend1 << "MC D6T ";
     legend2 << "MC D6T Unf=ATLAS@7 TeV";
     plot1 = "unfolding/nch_MC_gen_afterEvtSelCorrection";

     histoYMax  = 0.1 ;
     histoYMin  = 0.000003 ;
     if ( globalEnergy == 0.9 ) histoYMin  = 0.000003;
     if ( globalEnergy == 2.36) histoYMin  = 0.000000003;

     if ( globalEnergy == 0.9 ) histoXMax  = 75.5;
     if ( globalEnergy == 2.36) histoXMax  = 120.5;
     
     if ( globalEnergy == 0.9 ) plotdir = "../plots/systv10_bis/";
     if ( globalEnergy == 2.36) plotdir = "../plots/systv10_bis/";
     
     if ( globalEnergy == 0.9 ) globalFigBaseName = "nchstab_mcd6t_syst401_09TeV";
     if ( globalEnergy == 2.36) globalFigBaseName = "nchstab_mcd6t_syst401_236TeV";

  }

  if ( iStab == 8 ) {

     LegendTitle = "        |#eta| < 2.4";

     iData1 = 10 ;
     iData2 = 10 ;
     iMc1   = 10;
     iMc2   = 10;
     if ( globalEnergy == 7.0 ) { return; }
     iSyst1 =  402;
     iSyst2 =  402;
     iSign1 =  -1;
     iSign2 =  -1;
     outstr1 << "hyp" << 1 << "_niter" << 0 << "_cut" << 5 << "_DataType" << iData1;
     outstr2 << "hyp" << 1 << "_niter" << 0 << "_cut" << 5 << "_DataType" << iData2;
     legend1 << "MC D6T ";
     legend2 << "MC D6T Unf=D6T@7 TeV";
     plot1 = "unfolding/nch_MC_gen_afterEvtSelCorrection";

     histoYMax  = 0.1 ;
     histoYMin  = 0.000003 ;
     if ( globalEnergy == 0.9 ) histoYMin  = 0.000003;
     if ( globalEnergy == 2.36) histoYMin  = 0.000000003;

     if ( globalEnergy == 0.9 ) histoXMax  = 75.5;
     if ( globalEnergy == 2.36) histoXMax  = 120.5;
     
     if ( globalEnergy == 0.9 ) plotdir = "../plots/systv10_bis/";
     if ( globalEnergy == 2.36) plotdir = "../plots/systv10_bis/";
     
     if ( globalEnergy == 0.9 ) globalFigBaseName = "nchstab_mcd6t_syst402_09TeV";
     if ( globalEnergy == 2.36) globalFigBaseName = "nchstab_mcd6t_syst402_236TeV";

  }


  if ( iStab == 9 ) {

     LegendTitle = "        |#eta| < 2.4";

     iData1 = 10 ;
     iData2 = 10 ;
     iMc1   = 10;
     iMc2   = 10;
     if ( globalEnergy == 2.36 ) { return; }
     if ( globalEnergy == 7.0  ) { return; }
     iSyst1 =  411;
     iSyst2 =  411;
     iSign1 =  -1;
     iSign2 =  -1;
     outstr1 << "hyp" << 1 << "_niter" << 0 << "_cut" << 5 << "_DataType" << iData1;
     outstr2 << "hyp" << 1 << "_niter" << 0 << "_cut" << 5 << "_DataType" << iData2;
     legend1 << "MC D6T ";
     legend2 << "MC D6T Unf=DW@0.9 TeV";
     plot1 = "unfolding/nch_MC_gen_afterEvtSelCorrection";

     histoYMax  = 0.1 ;
     histoYMin  = 0.000003 ;
     if ( globalEnergy == 0.9 ) histoYMin  = 0.000003;

     if ( globalEnergy == 0.9 ) histoXMax  = 75.5;
     
     if ( globalEnergy == 0.9 ) plotdir = "../plots/systv10_bis/";
     
     if ( globalEnergy == 0.9 ) globalFigBaseName = "nchstab_mcd6t_syst411_09TeV";

  }
  
  
  if ( iStab == 10 ) {

     LegendTitle = "        |#eta| < 2.4";

     iData1 = 20 ;
     iData2 = 20 ;
     iMc1   = 20;
     iMc2   = 20;
     if ( globalEnergy == 2.36 ) { return; }
     if ( globalEnergy == 0.9  ) { return; }
     iSyst1 =  431;
     iSyst2 =  431;
     iSign1 =  -1;
     iSign2 =  -1;
     outstr1 << "hyp" << 1 << "_niter" << 0 << "_cut" << 5 << "_DataType" << iData1;
     outstr2 << "hyp" << 1 << "_niter" << 0 << "_cut" << 5 << "_DataType" << iData2;
     legend1 << "MC PHOJET ";
     legend2 << "MC PHOJET Unf=ATLAS";
     plot1 = "unfolding/nch_MC_gen_afterEvtSelCorrection";

     histoYMax  = 0.1 ;
     histoYMin  = 0.0000000003 ;

     histoXMax  = 195.5;
     plotdir = "../plots/systv10_bis/";
     globalFigBaseName = "nchstab_mcphojet_syst431_7TeV";

  }
  
  if ( iStab == 11 ) {

     LegendTitle = "        |#eta| < 2.4";

     iData1 = 20 ;
     iData2 = 20 ;
     iMc1   = 20;
     iMc2   = 20;
     if ( globalEnergy == 2.36 ) { return; }
     if ( globalEnergy == 0.9  ) { return; }
     iSyst1 =  430;
     iSyst2 =  430;
     iSign1 =  -1;
     iSign2 =  -1;
     outstr1 << "hyp" << 1 << "_niter" << 0 << "_cut" << 5 << "_DataType" << iData1;
     outstr2 << "hyp" << 1 << "_niter" << 0 << "_cut" << 5 << "_DataType" << iData2;
     legend1 << "MC PHOJET ";
     legend2 << "MC PHOJET Unf=D6T";
     plot1 = "unfolding/nch_MC_gen_afterEvtSelCorrection";

     histoYMax  = 0.1 ;
     histoYMin  = 0.0000000003 ;

     histoXMax  = 195.5;
     plotdir = "../plots/systv10_bis/";
     globalFigBaseName = "nchstab_mcphojet_syst430_7TeV";

  }
  
  // define plots 

  dataSetId.push_back(-1);
  dataSetFile.push_back(fileManager(3,iMc1,globalEnergy,1,iSyst1,iSign1,outstr1.str(),plotdir));
  dataSetIsMc.push_back(0);
  dataSetHType.push_back(1);
  dataSetStyle.push_back(kOpenCircle);
  dataSetColor.push_back(kBlue);
  dataSetLegend.push_back(legend1.str());
  dataSetHisto.push_back(plot1);

  dataSetId.push_back(-1);
  dataSetFile.push_back(fileManager(3,iMc2,globalEnergy,1,iSyst2,iSign2,outstr2.str(),plotdir));
  dataSetIsMc.push_back(0);
  dataSetHType.push_back(1);
  dataSetStyle.push_back(kOpenCircle);
  dataSetColor.push_back(kRed);
  dataSetLegend.push_back(legend2.str());
  dataSetHisto.push_back(plot2);

  // Plot 

    plot("none","AUTO",1,2);

}


