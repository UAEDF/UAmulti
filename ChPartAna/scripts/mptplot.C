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

void mptplot(double energy = 0.9, int iBin = 5 , int iSaveFig = 1, float npx = 1000 , float npy = 600){

  globalEnergy = energy;
  gROOT->ProcessLine(".x cmsStyleRoot.C");

  plotReset();

  globalCanvasSizeX = npx;
  globalCanvasSizeY = npy;

  XaxisTitle = "n";
  YaxisTitle = "<p_{T}>";
  
  globalLegendTextSize = 20;
  yLegendWidth = 0.04;
  xLegendWidth = .1 ;

 
  #include "../macro/acceptanceMap.C"
  ostringstream legheader("");
  legheader<< "  |#eta| < "<<accMap.at(iBin).at(1);
  LegendTitle = legheader.str();
  
  xGlobalLabel = 0.6 ; 
  if ( globalEnergy == 0.9 ) globalLabel =  "CMS Preliminary 0.9 TeV";
  if ( globalEnergy == 2.36) globalLabel =  "CMS Preliminary 2.36 TeV";
  if ( globalEnergy == 7.0 ) globalLabel =  "CMS Preliminary 7 TeV";

  globalSaveFig = iSaveFig;
  if ( globalEnergy == 0.9 ) globalFigBaseName = "0.9TeV_mpt";  
  if ( globalEnergy == 2.36) globalFigBaseName = "2.36TeV_mpt";
  if ( globalEnergy == 7.0 ) globalFigBaseName = "7.0TeV_mpt";

  globalNorm = 0;

  histoXMin  = 0.5;
  if ( globalEnergy == 0.9 ) histoXMax  = 94.5;
  if ( globalEnergy == 2.36) histoXMax  = 115.5;
  if ( globalEnergy == 7.0 ) histoXMax  = 180.5;

  if ( globalEnergy == 0.9 ) histoYMin  = 0.2;
  if ( globalEnergy == 2.36) histoYMin  = 0.3;
  if ( globalEnergy == 7.0 ) histoYMin  = 0.3;
  
  if ( globalEnergy == 0.9 ) histoYMax  = 1.;
  if ( globalEnergy == 2.36) histoYMax  = 0.9;
  if ( globalEnergy == 7.0 ) histoYMax  = 1.25;


  ostringstream mchisto("");
  mchisto << "mptVSnchgen_gen_cut" << iBin; 
  
  
  ostringstream datahisto("");
  datahisto << "mptVSnch_cut" << iBin; 
  ostringstream systhisto("");
  systhisto << "gmptVSnch_cut" << iBin << "_syst"; 

  double mptcor900 = 0.9365/0.9596 ; 
  double mptcor236 = 0.9365/0.9609 ; 
  double mptcor7   = 0.9376/0.9560 ; 

  cout << "mptcor900 = " << mptcor900 << endl;
  cout << "mptcor236 = " << mptcor236 << endl;
  cout << "mptcor7   = " << mptcor7   << endl;

  // ------ 900 GeV --------------------------------------------
  if ( globalEnergy == 0.9 ) {

    dataSetId.push_back(-1);
    dataSetFile.push_back("../plots/mptcorrv8/mptCorr_MC_D6T_0.9TeV_mbTr__dataType0.root");
    dataSetIsMc.push_back(0);
    dataSetHType.push_back(1);
    dataSetStyle.push_back(kOpenCircle);
    dataSetColor.push_back(kBlack);
    dataSetLegend.push_back("CMS");
    dataSetHisto.push_back(datahisto.str());
    dataSetFactor.push_back(mptcor900);

    // MC
  
    dataSetId.push_back(-1);
    dataSetFile.push_back("../plots/smallcodev6/smallCode_MCtype10_0.9TeV.root");
    dataSetIsMc.push_back(1);
    dataSetHType.push_back(1);
    dataSetStyle.push_back(20);
    dataSetColor.push_back(kRed);
    dataSetLegend.push_back("PYTHIA D6T");
    dataSetHisto.push_back(mchisto.str());
    dataSetFactor.push_back(1);
  
    dataSetId.push_back(-1);
    dataSetFile.push_back("../plots/smallcodev6/smallCode_MCtype11_0.9TeV.root");
    dataSetIsMc.push_back(1);
    dataSetHType.push_back(1);
    dataSetStyle.push_back(20);
    dataSetColor.push_back(kBlue);
    dataSetLegend.push_back("PYTHIA DW");
    dataSetHisto.push_back(mchisto.str());
    dataSetFactor.push_back(1);
  
    dataSetId.push_back(-1);
    dataSetFile.push_back("../plots/smallcodev6/smallCode_MCtype13_0.9TeV.root");
    dataSetIsMc.push_back(1);
    dataSetHType.push_back(1);
    dataSetStyle.push_back(20);
    dataSetColor.push_back(kOrange);
    dataSetLegend.push_back("PYTHIA ProQ20");
    dataSetHisto.push_back(mchisto.str());
    dataSetFactor.push_back(1);
  
    dataSetId.push_back(-1);
    dataSetFile.push_back("../plots/smallcodev6/smallCode_MCtype20_0.9TeV.root");
    dataSetIsMc.push_back(1);
    dataSetHType.push_back(1);
    dataSetStyle.push_back(20);
    dataSetColor.push_back(kMagenta);
    dataSetLegend.push_back("PHOJET");
    dataSetHisto.push_back(mchisto.str());
    dataSetFactor.push_back(1);
  
  
    // Data Syst

    dataSetId.push_back(-1);
    dataSetFile.push_back("../plots/mptcorrv8/mptCorr_MC_D6T_0.9TeV_mbTr__dataType0.root");
    dataSetIsMc.push_back(0);
    dataSetHType.push_back(105);
    dataSetStyle.push_back(kOpenCircle);
    dataSetColor.push_back(kBlack);
    dataSetLegend.push_back("NONE");
    dataSetHisto.push_back(systhisto.str());
    dataSetFactor.push_back(mptcor900);

  }

  // ------------ 2.36 TeV -------------------
  if ( globalEnergy == 2.36) {

    dataSetId.push_back(-1);
    dataSetFile.push_back("../plots/mptcorrv8/mptCorr_MC_D6T_2.36TeV_mbTr__dataType0.root");
    dataSetIsMc.push_back(0);
    dataSetHType.push_back(1);
    dataSetStyle.push_back(kOpenCircle);
    dataSetColor.push_back(kBlack);
    dataSetLegend.push_back("CMS");
    dataSetHisto.push_back(datahisto.str());
    dataSetFactor.push_back(mptcor236);

    // MC
 
    dataSetId.push_back(-1);
    dataSetFile.push_back("../plots/smallcodev6/smallCode_MCtype10_2.36TeV.root");
    dataSetIsMc.push_back(1);
    dataSetHType.push_back(1);
    dataSetStyle.push_back(20);
    dataSetColor.push_back(kRed);
    dataSetLegend.push_back("PYTHIA D6T");
    dataSetHisto.push_back(mchisto.str());
    dataSetFactor.push_back(1);
  
    dataSetId.push_back(-1);
    dataSetFile.push_back("../plots/smallcodev6/smallCode_MCtype20_2.36TeV.root");
    dataSetIsMc.push_back(1);
    dataSetHType.push_back(1);
    dataSetStyle.push_back(20);
<<<<<<< mptplot.C
    dataSetColor.push_back(kBlue);
    dataSetLegend.push_back("PYTHIA phojet");
=======
    dataSetColor.push_back(kMagenta);
    dataSetLegend.push_back("PHOJET");
>>>>>>> 1.4
    dataSetHisto.push_back(mchisto.str());
    dataSetFactor.push_back(1);

    // Data Syst
 
    dataSetId.push_back(-1);
    dataSetFile.push_back("../plots/mptcorrv8/mptCorr_MC_D6T_2.36TeV_mbTr__dataType0.root");
    dataSetIsMc.push_back(0);
    dataSetHType.push_back(105);
    dataSetStyle.push_back(kOpenCircle);
    dataSetColor.push_back(kBlack);
    dataSetLegend.push_back("NONE");
    dataSetHisto.push_back(systhisto.str());
    dataSetFactor.push_back(mptcor236);

  }


  // ------------ 7.0 TeV -------------------
  if ( globalEnergy == 7.0) {

    dataSetId.push_back(-1);
    dataSetFile.push_back("../plots/mptcorrv8/mptCorr_MC_ATLAS_7.0TeV_mbTr__dataType0.root");
    dataSetIsMc.push_back(0);
    dataSetHType.push_back(1);
    dataSetStyle.push_back(kOpenCircle);
    dataSetColor.push_back(kBlack);
    dataSetLegend.push_back("CMS");
    dataSetHisto.push_back(datahisto.str());
    dataSetFactor.push_back(mptcor7);

    // MC

    dataSetId.push_back(-1);
    dataSetFile.push_back("../plots/smallcodev6/smallCode_MCtype30_7TeV.root");
    dataSetIsMc.push_back(1);
    dataSetHType.push_back(1);
    dataSetStyle.push_back(20);
    dataSetColor.push_back(kRed);
    dataSetLegend.push_back("PYTHIA D6T");
    dataSetHisto.push_back(mchisto.str());
    dataSetFactor.push_back(1);
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
*/  
    dataSetId.push_back(-1);
    dataSetFile.push_back("../plots/smallcodev6/smallCode_MCtype20_7TeV.root");
    dataSetIsMc.push_back(1);
    dataSetHType.push_back(1);
    dataSetStyle.push_back(20);
    dataSetColor.push_back(kMagenta);
    dataSetLegend.push_back("PHOJET");
    dataSetHisto.push_back(mchisto.str());
    dataSetFactor.push_back(1);
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
  
    dataSetId.push_back(-1);
    dataSetFile.push_back("../plots/smallcodev6/smallCode_MCtype52_7TeV.root");
    dataSetIsMc.push_back(1);
    dataSetHType.push_back(1);
    dataSetStyle.push_back(20);
    dataSetColor.push_back(kGreen);
    dataSetLegend.push_back("PYTHIA X2");
    dataSetHisto.push_back(mchisto.str());
    dataSetFactor.push_back(1);
 */

    // data Syst  
    dataSetId.push_back(-1);
    dataSetFile.push_back("../plots/mptcorrv8/mptCorr_MC_ATLAS_7.0TeV_mbTr__dataType0.root");
    dataSetIsMc.push_back(0);
    dataSetHType.push_back(105);
    dataSetStyle.push_back(kOpenCircle);
    dataSetColor.push_back(kBlack);
    dataSetLegend.push_back("NONE");
    dataSetHisto.push_back(systhisto.str());
    dataSetFactor.push_back(mptcor7);

  }

  yLegendWidth = 0.040;
  xLegendWidth = .13 ;
  globalLegendTextSize = 22;


  plot("none","AUTO",0,8);

}
