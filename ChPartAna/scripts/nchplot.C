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

void nchplot(double energy = 0.9, int iBin = 5 , int iSaveFig = 1, float npx = 1000 , float npy = 600){

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
  if ( iBin == 5 ) {
    if ( globalEnergy == 0.9 ) globalFigBaseName = "0.9TeV_Comp";  
    if ( globalEnergy == 2.36) globalFigBaseName = "2.36TeV_Comp";  
    if ( globalEnergy == 7.0 ) globalFigBaseName = "7.0TeV_Comp";  
  }
  if ( iBin == 15 ) {
    if ( globalEnergy == 0.9 ) globalFigBaseName = "0.9TeV_Comp_pt500";
    if ( globalEnergy == 2.36) globalFigBaseName = "2.36TeV_Comp_pt500";
    if ( globalEnergy == 7.0 ) globalFigBaseName = "7.0TeV_Comp_pt500";
  }

  globalNorm = 2; 
  histoYMin  = 0.000001 ;
  histoYMax  = 0.1;
  histoXMin  = -0.5 ;
  
  if ( iBin == 5 ) {
    if ( globalEnergy == 0.9 ) histoXMax  = 100.5;
    if ( globalEnergy == 2.36) histoXMax  = 140.5;
    if ( globalEnergy == 7.0 ) histoXMax  = 180.5;
  }
  if ( iBin == 15 ) {
    histoYMax  = .3;
    histoYMin  = 0.000001 ;
    LegendTitle = "        p_{t} > 0.5 GeV/c";
    if ( globalEnergy == 0.9 ) histoXMax  = 50.5;
    if ( globalEnergy == 2.36) histoXMax  = 70.5;
    if ( globalEnergy == 7.0 ) histoXMax  = 110.5;
  }

  int iMc;
  if ( globalEnergy == 0.9 ) iMc = 10 ;
  if ( globalEnergy == 2.36) iMc = 10 ;
  if ( globalEnergy == 7.0 ) iMc = 31 ;

  TString plotdir("");
  if ( globalEnergy == 0.9 ) plotdir = "../plots/systv10_undecies2/";
  if ( globalEnergy == 2.36) plotdir = "../plots/systv10_undecies2/";
  if ( globalEnergy == 7.0 ) plotdir = "../plots/systv10_undecies2/";

  ostringstream outstr("");
  outstr << "hyp" << 1 << "_niter" << 0 << "_cut" << iBin << "_DataType" << 0;  

  ostringstream mchisto("");
  mchisto << "nch_gen_NSD_noSel_cut" << iBin; 

  // ------ 900 GeV --------------------------------------------
  if ( globalEnergy == 0.9 ) {


    dataSetId.push_back(-1);
    // dataSetFile.push_back("../plots/finalv9/unfolding_MC_D6T_0.9TeV_mbTr__hyp1_niter0_cut5_DataType0.root");
    dataSetFile.push_back(fileManager(3,iMc,globalEnergy,1,0,0,outstr.str(),plotdir));
    dataSetIsMc.push_back(0);
    dataSetHType.push_back(1);
    dataSetStyle.push_back(kOpenCircle);
    dataSetColor.push_back(kWhite);
    dataSetLegend.push_back("NONE");
    dataSetHisto.push_back("unfolding/nch_data_corrected");
  
    dataSetId.push_back(-1);
    //dataSetFile.push_back("../plots/finalv9/unfolding_MC_D6T_0.9TeV_mbTr__hyp1_niter0_cut5_DataType0.root");
    dataSetFile.push_back(fileManager(3,iMc,globalEnergy,1,0,0,outstr.str(),plotdir));
    dataSetIsMc.push_back(0);
    dataSetHType.push_back(5);
    dataSetStyle.push_back(kOpenCircle);
    dataSetColor.push_back(kBlack);
    dataSetLegend.push_back("Data");
    dataSetHisto.push_back("unfolding/gnch_data_corrected");
  
    // MC
  
    dataSetId.push_back(-1);
    dataSetFile.push_back("../plots/smallcodev6/smallCode_MCtype10_0.9TeV.root");
    dataSetIsMc.push_back(1);
    dataSetHType.push_back(1);
    dataSetStyle.push_back(20);
    dataSetColor.push_back(kRed);
    dataSetLegend.push_back("pythia D6T");
    dataSetHisto.push_back(mchisto.str()); 
    
    dataSetId.push_back(-1);
    dataSetFile.push_back("../plots/smallcodev6/smallCode_MCtype11_0.9TeV.root");
    dataSetIsMc.push_back(1);
    dataSetHType.push_back(1);
    dataSetStyle.push_back(20);
    dataSetColor.push_back(kBlue);
    dataSetLegend.push_back("pythia DW");
    dataSetHisto.push_back(mchisto.str()); 
    
    dataSetId.push_back(-1);
    dataSetFile.push_back("../plots/smallcodev6/smallCode_MCtype13_0.9TeV.root");
    dataSetIsMc.push_back(1);
    dataSetHType.push_back(1);
    dataSetStyle.push_back(20);
    dataSetColor.push_back(kOrange);
    dataSetLegend.push_back("pythia ProQ20");
    dataSetHisto.push_back(mchisto.str()); 
  
    dataSetId.push_back(-1);
    dataSetFile.push_back("../plots/smallcodev6/smallCode_MCtype20_0.9TeV.root");
    dataSetIsMc.push_back(1);
    dataSetHType.push_back(1);
    dataSetStyle.push_back(20);
    dataSetColor.push_back(kMagenta);
    dataSetLegend.push_back("phojet");
    dataSetHisto.push_back(mchisto.str());
  
    // And data syst again
  
    dataSetId.push_back(-1);
    //dataSetFile.push_back("../plots/finalv9/unfolding_MC_D6T_0.9TeV_mbTr__hyp1_niter0_cut5_DataType0.root");
    dataSetFile.push_back(fileManager(3,iMc,globalEnergy,1,0,0,outstr.str(),plotdir));
    dataSetIsMc.push_back(0);
    dataSetHType.push_back(105);
    dataSetStyle.push_back(kOpenCircle);
    dataSetColor.push_back(kBlack);
    dataSetLegend.push_back("NONE");
    dataSetHisto.push_back("unfolding/gnch_data_corrected_syst");
 
  } 

  // ------------ 2.36 TeV -------------------
  if ( globalEnergy == 2.36) {


    dataSetId.push_back(-1);
    //dataSetFile.push_back("../plots/finalv9/unfolding_MC_D6T_2.36TeV_mbTr__hyp1_niter0_cut5_DataType0.root");
    dataSetFile.push_back(fileManager(3,iMc,globalEnergy,1,0,0,outstr.str(),plotdir));
    dataSetIsMc.push_back(0);
    dataSetHType.push_back(1);
    dataSetStyle.push_back(kOpenCircle);
    dataSetColor.push_back(kWhite);
    dataSetLegend.push_back("NONE");
    dataSetHisto.push_back("unfolding/nch_data_corrected");


    dataSetId.push_back(-1);
    //dataSetFile.push_back("../plots/finalv9/unfolding_MC_D6T_2.36TeV_mbTr__hyp1_niter0_cut5_DataType0.root");
    dataSetFile.push_back(fileManager(3,iMc,globalEnergy,1,0,0,outstr.str(),plotdir));
    dataSetIsMc.push_back(0);
    dataSetHType.push_back(5);
    dataSetStyle.push_back(kOpenCircle);
    dataSetColor.push_back(kBlack);
    dataSetLegend.push_back("Data");
    dataSetHisto.push_back("unfolding/gnch_data_corrected");

    // MC

    dataSetId.push_back(-1);
    dataSetFile.push_back("../plots/smallcodev6/smallCode_MCtype20_2.36TeV.root");
    dataSetIsMc.push_back(1);
    dataSetStyle.push_back(20);
    dataSetHType.push_back(1);
    dataSetColor.push_back(kBlue);
    dataSetLegend.push_back("phojet");
    dataSetHisto.push_back(mchisto.str()); 

    dataSetId.push_back(-1);
    dataSetFile.push_back("../plots/smallcodev6/smallCode_MCtype10_2.36TeV.root");
    dataSetIsMc.push_back(1);
    dataSetHType.push_back(1);
    dataSetStyle.push_back(20);
    dataSetColor.push_back(kRed);
    dataSetLegend.push_back("pythia D6T");
    dataSetHisto.push_back(mchisto.str());

    // Data systematics

    dataSetId.push_back(-1);
    //dataSetFile.push_back("../plots/finalv9/unfolding_MC_D6T_2.36TeV_mbTr__hyp1_niter0_cut5_DataType0.root");
    dataSetFile.push_back(fileManager(3,iMc,globalEnergy,1,0,0,outstr.str(),plotdir));
    dataSetIsMc.push_back(0);
    dataSetHType.push_back(105);
    dataSetStyle.push_back(kOpenCircle);
    dataSetColor.push_back(kBlack);
    dataSetLegend.push_back("NONE");
    dataSetHisto.push_back("unfolding/gnch_data_corrected_syst");

  }

   
  // ------------ 7.0 TeV -------------------
  if ( globalEnergy == 7.0) {


    dataSetId.push_back(-1);
    //dataSetFile.push_back("../plots/systv10/unfolding_MC_ATLAS_7.0TeV_mbTr__hyp1_niter0_cut5_DataType0.root");
    dataSetFile.push_back(fileManager(3,iMc,globalEnergy,1,0,0,outstr.str(),plotdir));
    dataSetIsMc.push_back(0);
    dataSetHType.push_back(1);
    dataSetStyle.push_back(kOpenCircle);
    dataSetColor.push_back(kWhite);
    dataSetLegend.push_back("NONE");
    dataSetHisto.push_back("unfolding/nch_data_corrected");


    dataSetId.push_back(-1);
    //dataSetFile.push_back("../plots/systv10/unfolding_MC_ATLAS_7.0TeV_mbTr__hyp1_niter0_cut5_DataType0.root");
    dataSetFile.push_back(fileManager(3,iMc,globalEnergy,1,0,0,outstr.str(),plotdir));
    dataSetIsMc.push_back(0);
    dataSetHType.push_back(5);
    dataSetStyle.push_back(kOpenCircle);
    dataSetColor.push_back(kBlack);
    dataSetLegend.push_back("Data");
    dataSetHisto.push_back("unfolding/gnch_data_corrected");

    // MC

    dataSetId.push_back(-1);
    dataSetFile.push_back("../plots/smallcodev6/smallCode_MCtype30_7TeV.root");
    dataSetIsMc.push_back(1);
    dataSetHType.push_back(1);
    dataSetStyle.push_back(20);
    dataSetColor.push_back(kRed);
    dataSetLegend.push_back("pythia D6T");
    dataSetHisto.push_back(mchisto.str());

/*
    dataSetId.push_back(-1);
    dataSetFile.push_back("../plots/smallcodev6/smallCode_MCtype31_7TeV.root");
    dataSetIsMc.push_back(1);
    dataSetHType.push_back(1);
    dataSetStyle.push_back(20);
    dataSetColor.push_back(kBlue);
    dataSetLegend.push_back("pythia atlas");
    dataSetHisto.push_back(mchisto.str());
*/
  
    dataSetId.push_back(-1);
    dataSetFile.push_back("../plots/smallcodev6/smallCode_MCtype20_7TeV.root");
    dataSetIsMc.push_back(1);
    dataSetHType.push_back(1);
    dataSetStyle.push_back(20);
    dataSetColor.push_back(kMagenta);
    dataSetLegend.push_back("phojet");
    dataSetHisto.push_back(mchisto.str());
 
/*
    dataSetId.push_back(-1);
    dataSetFile.push_back("../plots/smallcodev6/smallCode_MCtype51_7TeV.root");
    dataSetIsMc.push_back(1);
    dataSetHType.push_back(1);
    dataSetStyle.push_back(20);
    dataSetColor.push_back(kOrange);
    dataSetLegend.push_back("pythia X1");
    dataSetHisto.push_back(mchisto.str());
  
    dataSetId.push_back(-1);
    dataSetFile.push_back("../plots/smallcodev6/smallCode_MCtype52_7TeV.root");
    dataSetIsMc.push_back(1);
    dataSetHType.push_back(1);
    dataSetStyle.push_back(20);
    dataSetColor.push_back(kGreen);
    dataSetLegend.push_back("pythia X2");
    dataSetHisto.push_back(mchisto.str());
*/  
    // Data systematics

    dataSetId.push_back(-1);
    //dataSetFile.push_back("../plots/systv10/unfolding_MC_ATLAS_7.0TeV_mbTr__hyp1_niter0_cut5_DataType0.root");
    dataSetFile.push_back(fileManager(3,iMc,globalEnergy,1,0,0,outstr.str(),plotdir));
    dataSetIsMc.push_back(0);
    dataSetHType.push_back(105);
    dataSetStyle.push_back(kOpenCircle);
    dataSetColor.push_back(kBlack);
    dataSetLegend.push_back("NONE");
    dataSetHisto.push_back("unfolding/gnch_data_corrected_syst");

  }


   
  // ----- PLOT ----

  plot("none","AUTO",1,2);

}
