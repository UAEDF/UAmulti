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

void plotcorrectionsteps(double E = 0.9 , int cut = 5 , int typeCorr = 2 , int iSaveFig = 1, float npx = 500 , float npy = 500){

  gROOT->ProcessLine(".x cmsStyleRoot.C");

  plotReset();
  
  
  globalEnergy = E;
 
  globalCanvasSizeX = npx; 
  globalCanvasSizeY = npy;
  
  globalLegendTextSize = 20;
  yLegendWidth = 0.05;
  xLegendWidth = .1 ;
 
  xGlobalLabel = 0.45 ;
  if(E==0.9)  globalLabel =  "CMS Preliminary 0.9 TeV";
  if(E==2.36) globalLabel =  "CMS Preliminary 2.36 TeV";
  if(E==7)    globalLabel =  "CMS Preliminary 7.0 TeV";

  globalSaveFig = iSaveFig;
  ostringstream globalfigbasename("");
  if(typeCorr==1)
    globalfigbasename<<"mptSteps";
  if(typeCorr==2)
    globalfigbasename<<"nchSteps";
  globalfigbasename<<E<<"TeV_cut"<<cut;
  globalFigBaseName = globalfigbasename.str();
  
  
  globalNorm = 0; 
  /*if ( iBin ==  5 )  histoYMin  = 0.0003 ;
  if ( iBin ==  7 )  histoYMin  = 0.000004 ;
  if ( iBin ==  9 )  histoYMin  = 0.00001 ;
  if ( iBin == 15 )  histoYMin  = 0.0001 ;
  if ( iBin == 17 )  histoYMin  = 0.00001 ;
  if ( iBin == 19 )  histoYMin  = 0.00001 ;

  histoYMax  = 1.3 ;
  histoXMin  = -0.5 ;

  if (iBin ==  5 ) histoXMax  = 6 ;
  if (iBin ==  7 ) histoXMax  = 9.;
  if (iBin ==  9 ) histoXMax  = 9. ;
  if (iBin > 10 ) histoYMax = 1.0; 
  if (iBin == 15 ) histoXMax  = 10.;
  if (iBin == 17 ) histoXMax  = 12 ;
  if (iBin == 19 ) histoXMax  = 12. ;*/

  #include "../macro/acceptanceMap.C"
  ostringstream legheader("");
  legheader << "   |#eta| < " << accMap.at(cut).at(1);
  if(accMap.at(cut).at(0)!=0)
    legheader << "  p_{t} > " << accMap.at(cut).at(0) << " GeV/c";
  LegendTitle = legheader.str();
  

  int iMC = 10;
  if ( E == 7.0 ) iMC = 31 ;

  if(typeCorr==1){
    
    
//    globalLegendTextSize = 13;
  
    XaxisTitle = "n";
    YaxisTitle = "p_{T}";
    
    TString plotdir = "../plots/mptcorrv7_bis/";
    
    //histoYMin  = 0.1 ;
    //histoYMax  = 0.1;
    histoXMin  = 0.5 ;
    histoYMin  = 0.2 ;
    histoYMax  = 1.  ;
    if(E==7) histoYMax  = 0.8  ;

    if ( cut == 5 ) {
      if ( E == 0.9 ) histoXMax  = 70.5;
      if ( E == 2.36) histoXMax  = 95.5;
      if ( E == 7.0 ) histoXMax  = 140.5;
    }
    if ( cut == 15 ) {
      if ( E == 0.9 ) histoXMax  = 50.5;
      if ( E == 2.36) histoXMax  = 70.5;
      if ( E == 7.0 ) histoXMax  = 110.5;
    }
  
    ostringstream TCUT("");
    TCUT << "_cut" << cut;
    TString tcut = TCUT.str();

    ostringstream strout("");
    strout<<"dataType"<<iMC;
    
    dataSetId.push_back(-1);
    dataSetFile.push_back(fileManager(5,iMC,E,1,0,0,strout.str(),plotdir));
    dataSetIsMc.push_back(1);
    dataSetHType.push_back(1);
    dataSetStyle.push_back(1);
    dataSetColor.push_back(kBlue);
    dataSetLegend.push_back("MC Raw");
    dataSetHisto.push_back(TString("mptVSnch_noCorr")+tcut);
    
    
    dataSetId.push_back(-1);
    dataSetFile.push_back(fileManager(5,iMC,E,1,0,0,strout.str(),plotdir));
    dataSetIsMc.push_back(1);
    dataSetHType.push_back(1);
    dataSetStyle.push_back(1);
    dataSetColor.push_back(kRed);
    dataSetLegend.push_back("+ nch Corr");
    dataSetHisto.push_back(TString("mptVSnch_nchCorr")+tcut);
    
    
    
    dataSetId.push_back(-1);
    dataSetFile.push_back(fileManager(5,iMC,E,1,0,0,strout.str(),plotdir));
    dataSetIsMc.push_back(1);
    dataSetHType.push_back(1);
    dataSetStyle.push_back(2);
    dataSetColor.push_back(kMagenta);
    dataSetLegend.push_back("+ <p_{T}>^{gen} / <p_{T}>^{reco} Corr");
    dataSetHisto.push_back(TString("mptVSnch_nch_mptCorr")+tcut);
    
    
    
    dataSetId.push_back(-1);
    dataSetFile.push_back(fileManager(5,iMC,E,1,0,0,strout.str(),plotdir));
    dataSetIsMc.push_back(0);
    dataSetHType.push_back(1);
    dataSetStyle.push_back(kOpenCircle);
    dataSetColor.push_back(kBlack);
    dataSetLegend.push_back("+ evtSel Corr");
    dataSetHisto.push_back(TString("mptVSnch")+tcut);
    
    TString tune = "PYTHIA D6T";
    if(E==7.0) tune = "PYTHIA ATLAS";
    dataSetId.push_back(-1);
    dataSetFile.push_back(fileManager(5,iMC,E,1,0,0,strout.str(),plotdir));
    dataSetIsMc.push_back(1);
    dataSetHType.push_back(1);
    dataSetStyle.push_back(kOpenCircle);
    dataSetColor.push_back(kBlack);
    dataSetLegend.push_back(tune);
    dataSetHisto.push_back(TString("mptVSnch_gen")+tcut);
    
    
    plot("none","AUTO",0,1);
  }

  if(typeCorr==2){
  
    
    XaxisTitle = "n";
    YaxisTitle = "P_{n}";
  
    TString plotdir("");
    plotdir = "../plots/systv10_niter100_2/";
    
    //histoYMin  = 0.1 ;
    //histoYMax  = 0.1;
    histoXMin  = -0.5 ;

    if ( cut == 5 ) {
      if ( E == 0.9 ) histoXMax  = 70.5;
      if ( E == 2.36) histoXMax  = 95.5;
      if ( E == 7.0 ) histoXMax  = 140.5;
    }
    if ( cut == 15 ) {
      if ( E == 0.9 ) histoXMax  = 50.5;
      if ( E == 2.36) histoXMax  = 70.5;
      if ( E == 7.0 ) histoXMax  = 110.5;
    }
  
    ostringstream outstr("");
    outstr << "hyp" << 1 << "_niter" << 0 << "_cut" << cut << "_DataType" << 0;  

    dataSetId.push_back(-1);
    dataSetFile.push_back(fileManager(3,iMC,E,1,0,0,outstr.str(),plotdir));
    dataSetIsMc.push_back(1);
    dataSetHType.push_back(1);
    dataSetStyle.push_back(1);
    dataSetColor.push_back(kBlue);
    dataSetLegend.push_back("Data Raw");
    dataSetHisto.push_back("unfolding/nch_data_INC_beforeSDsub");
    
    
    dataSetId.push_back(-1);
    dataSetFile.push_back(fileManager(3,iMC,E,1,0,0,outstr.str(),plotdir));
    dataSetIsMc.push_back(1);
    dataSetHType.push_back(1);
    dataSetStyle.push_back(1);
    dataSetColor.push_back(kRed);
    dataSetLegend.push_back("Data - MC SD");
    dataSetHisto.push_back("unfolding/nch_data_NSD_afterSDsub");
    
    
    
    dataSetId.push_back(-1);
    dataSetFile.push_back(fileManager(3,iMC,E,1,0,0,outstr.str(),plotdir));
    dataSetIsMc.push_back(1);
    dataSetHType.push_back(1);
    dataSetStyle.push_back(1);
    dataSetColor.push_back(kMagenta);
    dataSetLegend.push_back("Data Unfolded");
    dataSetHisto.push_back("unfolding/nch_unfoldedPtr");
    
    
    
    dataSetId.push_back(-1);
    dataSetFile.push_back(fileManager(3,iMC,E,1,0,0,outstr.str(),plotdir));
    dataSetIsMc.push_back(0);
    dataSetHType.push_back(1);
    dataSetStyle.push_back(kOpenCircle);
    dataSetColor.push_back(kBlack);
    dataSetLegend.push_back("Data evtSel Corr");
    dataSetHisto.push_back("unfolding/nch_data_corrected");
    
    
   // plot("none","AUTO",1,2);
    plot("none","AUTO",0,6);
    
    
  }
  
  


   
  // ----- PLOT ----


}
