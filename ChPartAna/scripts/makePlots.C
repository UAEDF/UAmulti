//
// makePlots: Do all control plots
// ---------
//
// itracking = 1 : general Tracks and offline Vtx
//             2 : minbias Tracks and ferenc  Vtx

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
//#include "plot2D.C"
//#include "plotMC.C"
//#include "plotMC2D.C"


// --- What to Plot ----
// Control Plots
bool trkplot = 0;
bool vqlplot = 0;
bool vtxplot = 0;
bool nvtplot = 0;
bool tvtplot = 0;
bool mltplot = 0;

// eta / pt / pt2
bool corplot = 0;
bool etaplot = 0;

// Nch Unfolding
bool effplot = 0;
bool mtxplot = 0;

bool nchplot = 0;
bool nchctrk = 0;
bool nchcbin = 0;
bool nchcunf = 0;
bool nchchyp = 0;
bool nchcitr = 0;
bool nchcene = 0;

bool nchstak = 0;
bool knostak = 0;

bool nch2ene = 0;
bool kno2ene = 0;

// Unfolding options
int iUfoldNIter;
int iUfoldDataType;
int iUfoldHyp;
int iUfoldMCType;

void setnchrangeX(int );

// ------------------------ trackPlot

void trackPlots()
{
  trkplot = 1;
  vqlplot = 0;
  vtxplot = 1;
  

/*
  mltplot = 0;
  effplot = 0;
  mtxplot = 0;
*/

  gROOT->ProcessLine(".x cmsStyleRoot.C");
  gROOT->ProcessLine("makePlots(1,0.9,0.15)");
  gROOT->ProcessLine("makePlots(1,0.9,0.4)");
  gROOT->ProcessLine("makePlots(2,0.9,0.15)");
  gROOT->ProcessLine("makePlots(2,0.9,0.4)");
  gROOT->ProcessLine("makePlots(1,2.36,0.15)");
  gROOT->ProcessLine("makePlots(1,2.36,0.4)");
  gROOT->ProcessLine("makePlots(2,2.36,0.15)");
  gROOT->ProcessLine("makePlots(2,2.36,0.4)");

}


// ------------------------ makePlots ---------------------------
void makePlots (int itracking = 1 , double energy = 0.9 , double ptcut = 0.4 , double etacut = 2.5 , int iUfoldBin = 0){

//  vector< vector<double> > accMap;
  #include "../macro/acceptanceMap.C"

  gROOT->ProcessLine(".x cmsStyleRoot.C");

  TString sel,tr,all,dir,cut;

  plotReset();
  globalTraking = itracking -1; 
  globalEnergy  = energy;

  if ( globalEnergy == 0.9 ) globalLabel =  "CMS 0.9 TeV";
  if ( globalEnergy == 2.36) globalLabel =  "CMS 2.36 TeV";  

  //------ TRACK & VTX PLOTS CONFIG ---------
  if ( trkplot || vqlplot || vtxplot || nvtplot || tvtplot || mltplot ) { 

    globalNorm = 1; 
  
    // PTCUT string
  
    std::stringstream PT("");
    PT << "ptcut" << ptcut;
    if (etacut != 2.5) {
      PT << "_eta" << etacut; 
    }
    ptcutstr = PT.str();
  
    // Legend Title
    std::stringstream LT("");
    if (itracking == 1 ) LT << "genTracks: " << endl;  
    if (itracking == 2 ) LT << "mbiasTracks: " << endl; 
    if (itracking == 3 ) LT << "mbiasTracks+pVtx: " << endl; 
    LT << "  |#eta|<" << etacut  ;
    LT << "  p_{T}>"  << ptcut   ;
    LegendTitle = LT.str();
  
    // 
    if ( energy == 0.9 ) {
 
      // DATA
      dataSetId.push_back(0);
      dataSetIsMc.push_back(0);
      dataSetStyle.push_back(20);
      dataSetColor.push_back(2);
      dataSetLegend.push_back("Data");
  
      // PYTHIA - D6T
      dataSetId.push_back(10);
      dataSetIsMc.push_back(1);
      dataSetStyle.push_back(1);
      dataSetColor.push_back(1);
      dataSetLegend.push_back("PYTHIA D6T");
/*  
  
      // PYTHIA - DW
      dataSetId.push_back(11);
      dataSetIsMc.push_back(1);
      dataSetStyle.push_back(2);
      dataSetColor.push_back(2);
      dataSetLegend.push_back("PYTHIA DW");
  
      // PYTHIA - P0
      dataSetId.push_back(12);
      dataSetIsMc.push_back(1);
      dataSetStyle.push_back(2);
      dataSetColor.push_back(3);
      dataSetLegend.push_back("PYTHIA P0");
  
      // PYTHIA - P0
      dataSetId.push_back(13);
      dataSetIsMc.push_back(1);
      dataSetStyle.push_back(2);
      dataSetColor.push_back(4);
      dataSetLegend.push_back("PYTHIA ProQ20");
*/  
  
    }
    if ( energy == 2.36) {
  
      // DATA
      dataSetId.push_back(0);
      dataSetIsMc.push_back(0);
      dataSetStyle.push_back(20);
      dataSetColor.push_back(2);
      dataSetLegend.push_back("Data");
  
      // PYTHIA - D6T
      dataSetId.push_back(10);
      dataSetIsMc.push_back(1);
      dataSetStyle.push_back(1);
      dataSetColor.push_back(1);
      dataSetLegend.push_back("PYTHIA D6T");
  
    }
  
  } // if trkplot || vtxplot
  
  //------ TRACK PLOTS ---------
  if(trkplot){

    // Plots.allPlotSetting->clear();

    sel = "L1_hf_vtxqual_vtxSel";
    if ( itracking == 1 ) {
      tr  = "PV_gTr_oVtx";
    } else if  ( itracking == 2 ) {
      tr  = "PV_mbTr_fVtx";
    } else if  ( itracking == 3 ) {
      tr  = "PV_mbTr_pVtx";
    } else {
      cout << "itraking = " << itracking << " :option not available !!! exit !!! " << endl;
      return;
    }
  
  
    all = sel+"_"+tr;
    dir = "EvtSelPlots_"+tr   +   "/TrackPlots_"+sel+"_"+tr;

    cout << dir << endl;
    cout << all << endl;


    plot(dir,"nch_"+all);
    plot(dir,"nch_"+all,1,2);
    plot(dir,"chi2n_"+all);
    plot(dir,"chi2n_"+all,1);
    plot(dir,"pt_"+all);
    plot(dir,"pt_"+all,1);
    plot(dir,"eta_"+all,0,1);
    plot(dir,"phi_"+all,0,1);
    plot(dir,"charge_"+all,0,1);
    plot(dir,"nhit_"+all);
    plot(dir,"quality_"+all,1,0);
    
    if ( itracking == 1 ) {
      tr  = "allTr_gTr_oVtx";
      all = sel+"_"+tr;
      dir = "EvtSelPlots_"+tr   +   "/TrackPlots_"+sel+"_"+tr;
      cout<<dir<<endl;
     
      plot(dir,"dzOedz_"+all);
      plot(dir,"dzOedz_"+all,1);
      plot(dir,"dxyOed0_"+all);
      plot(dir,"dxyOed0_"+all,1);
      plot(dir,"eptOpt_"+all);
      plot(dir,"eptOpt_"+all,1);

    } else if  ( itracking == 2 || itracking == 3 ) {
      tr  = "allTr_mbTr_fVtx";
      all = sel+"_"+tr;
      dir = "EvtSelPlots_"+tr   +   "/TrackPlots_"+sel+"_"+tr;
      cout<<dir<<endl;

      plot(dir,"dzOsz_"+all);
      plot(dir,"dzOsz_"+all,1);
      plot(dir,"dxybsOsxy_"+all);
      plot(dir,"dxybsOsxy_"+all,1);
      plot(dir,"eptOpt_"+all);
      plot(dir,"eptOpt_"+all,1);


//    gROOT->ProcessLine(".X plot.C(\""+dir+"\",\"sxy_"+all+"\",1");
//    gROOT->ProcessLine(".X plot.C(\""+dir+"\",\"ed0_"+all+"\",1");
//    gROOT->ProcessLine(".X plot.C(\""+dir+"\",\"d0_"+all+"\",1");
//    gROOT->ProcessLine(".X plot.C(\""+dir+"\",\"d0bs_"+all+"\",1");
//    tr  = "PV_mbTr_fVtx";
//    all = sel+"_"+tr;
//    dir = "EvtSelPlots_"+tr   +   "/TrackPlots_"+sel+"_"+tr;
//    gROOT->ProcessLine(".X plot.C(\""+dir+"\",\"dxybsOsxy_"+all+"\",1");
//    gROOT->ProcessLine(".X plot.C(\""+dir+"\",\"sxy_"+all+"\",1");
//    gROOT->ProcessLine(".X plot.C(\""+dir+"\",\"d0bs_"+all+"\",1");

    } else {
      cout << "itraking = " << itracking << " :option not available !!! exit !!! " << endl;
      return;
    }


 
  }


  //------ TRACK PLOTS --------- vs VTX position
  if(tvtplot){

    // Plots.allPlotSetting->clear();

    if ( itracking == 1 ) {
      tr  = "PV_gTr_oVtx";
    } else if  ( itracking == 2 ) {
      tr  = "PV_mbTr_fVtx";
    } else if  ( itracking == 3 ) {
      tr  = "PV_mbTr_pVtx";
    } else {
      cout << "itraking = " << itracking << " :option not available !!! exit !!! " << endl;
      return;
    }
  
  
    sel = "smallzvtx_evtSel";
    all = sel+"_"+tr;
    dir = "EvtSelPlots_"+tr   +   "/TrackPlots_"+sel+"_"+tr;

    // Legend Title
    TString LTKEEP(LegendTitle);
    LegendTitle = LTKEEP + "  |z_{vtx}|<1" ;  

    cout << dir << endl;
    cout << all << endl;

/*
    plot(dir,"nch_"+all);
    plot(dir,"nch_"+all,1,2);
    plot(dir,"chi2n_"+all);
    plot(dir,"chi2n_"+all,1);
    plot(dir,"pt_"+all);
    plot(dir,"pt_"+all,1);
*/
    plot(dir,"eta_"+all,0,1);
/*
    plot(dir,"phi_"+all,0,1);
    plot(dir,"charge_"+all,0,1);
    plot(dir,"nhit_"+all);
*/

    sel = "mediumzvtx_evtSel";
    all = sel+"_"+tr;
    dir = "EvtSelPlots_"+tr   +   "/TrackPlots_"+sel+"_"+tr;

    LegendTitle = LTKEEP + "  1<|z_{vtx}|<5" ;

    cout << dir << endl;
    cout << all << endl;
/*
    plot(dir,"nch_"+all);
    plot(dir,"nch_"+all,1,2);
    plot(dir,"chi2n_"+all);
    plot(dir,"chi2n_"+all,1);
    plot(dir,"pt_"+all);
    plot(dir,"pt_"+all,1);
*/
    plot(dir,"eta_"+all,0,1);
/*
    plot(dir,"phi_"+all,0,1);
    plot(dir,"charge_"+all,0,1);
    plot(dir,"nhit_"+all);
*/


    sel = "largezvtx_evtSel";
    all = sel+"_"+tr;
    dir = "EvtSelPlots_"+tr   +   "/TrackPlots_"+sel+"_"+tr;

    LegendTitle = LTKEEP + "  |z_{vtx}|>5" ;

    cout << dir << endl;
    cout << all << endl;
/*
    plot(dir,"nch_"+all);
    plot(dir,"nch_"+all,1,2);
    plot(dir,"chi2n_"+all);
    plot(dir,"chi2n_"+all,1);
    plot(dir,"pt_"+all);
    plot(dir,"pt_"+all,1);
*/
    plot(dir,"eta_"+all,0,1);
/*
    plot(dir,"phi_"+all,0,1);
    plot(dir,"charge_"+all,0,1);
    plot(dir,"nhit_"+all);
*/


    LegendTitle = LTKEEP ;

/*    
    if ( itracking == 1 ) {
      tr  = "allTr_gTr_oVtx";
      all = sel+"_"+tr;
      dir = "EvtSelPlots_"+tr   +   "/TrackPlots_"+sel+"_"+tr;
      cout<<dir<<endl;
     
      plot(dir,"dzOedz_"+all);
      plot(dir,"dzOedz_"+all,1);
      plot(dir,"dxyOed0_"+all);
      plot(dir,"dxyOed0_"+all,1);
      plot(dir,"eptOpt_"+all);
      plot(dir,"eptOpt_"+all,1);

    } else if  ( itracking == 2 || itracking == 3 ) {
      tr  = "allTr_mbTr_fVtx";
      all = sel+"_"+tr;
      dir = "EvtSelPlots_"+tr   +   "/TrackPlots_"+sel+"_"+tr;
      cout<<dir<<endl;

      plot(dir,"dzOsz_"+all);
      plot(dir,"dzOsz_"+all,1);
      plot(dir,"dxybsOsxy_"+all);
      plot(dir,"dxybsOsxy_"+all,1);

    } else {
      cout << "itraking = " << itracking << " :option not available !!! exit !!! " << endl;
      return;
    }

*/
 
  }
  
/*  
  //-------- VERTEX QUAL --------
  if(vqlplot){
    if ( itracking == 1 ) {
      tr  = "PV_gTr_oVtx";
    } else if  ( itracking == 2 ) {
      tr  = "PV_mbTr_fVtx";
    } else {
      cout << "itraking = " << itracking << " :option not available !!! exit !!! " << endl;
      return;
    }
    dir = "EvtSelPlots_"+tr;
  
    gROOT->ProcessLine(".X plot2D.C(\""+dir+"\",\"vtxqual_L1Sel_"+tr+"\"");
    gROOT->ProcessLine(".X plot2D.C(\""+dir+"\",\"vtxqual_L1_hfSel_"+tr+"\"");
    gROOT->ProcessLine(".X plot2D.C(\""+dir+"\",\"vtxqual_L1_hf_vtxSel_"+tr+"\"");
  
  
  }
*/
    
  //------ VERTEX PLOTS ---------
  if(vtxplot){
    sel = "L1_hf_vtxqualSel";
    if ( itracking == 1 ) {
       tr  = "allTr_gTr_oVtx";
    } else if  ( itracking == 2 ) {
       tr  = "allTr_mbTr_fVtx";
    } else if  ( itracking == 3 ) {
       tr  = "allTr_mbTr_pVtx";
    } else {
       cout << "itraking = " << itracking << " :option not available !!! exit !!! " << endl;
       return;
    }
  
  
    all = sel+"_"+tr;
    dir = "EvtSelPlots_"+tr   +   "/VertexPlots_"+sel+"_"+tr;
    cout<<dir<<endl;
    
    
    //plot(dir,"z_wide_"+all);
    //plot(dir,"z_wide_"+all,1);

    if ( itracking == 1 ) {    
       tr  = "PV_gTr_oVtx";
    } else if  ( itracking == 2 ) {
       tr  = "PV_mbTr_fVtx";
    } else if  ( itracking == 3 ) {
       tr  = "PV_mbTr_pVtx";
    } else {
       cout << "itraking = " << itracking << " :option not available !!! exit !!! " << endl;
       return;
    }
 

    sel = "L1_hf_vtxqual_vtxSel";
    all = sel+"_"+tr;
    dir = "EvtSelPlots_"+tr   +   "/VertexPlots_"+sel+"_"+tr;
    cout<<dir<<endl;

    plot(dir,"z_"+all,0,3);
    plot(dir,"x_"+all,0,2);
    plot(dir,"y_"+all,0,2);
    plot(dir,"chi2n_"+all);
    plot(dir,"chi2n_"+all,1);
    plot(dir,"ntracks_"+all);
    
    // gROOT->ProcessLine(".X plot2D.C(\""+dir+"\",\"xy_"+all+"\"");
    
  }

  //------ VERTEX PLOTS --------- How many vtx ?
  if (nvtplot) {

    //--- #vertex

    if ( itracking == 1 ) {
       tr  = "PV_gTr_oVtx";
    } else if  ( itracking == 2 ) {
       tr  = "PV_mbTr_fVtx";
    } else if  ( itracking == 3 ) {
       tr  = "PV_mbTr_pVtx";
    } else {
       cout << "itraking = " << itracking << " :option not available !!! exit !!! " << endl;
       return;
    }
  
    sel = "evtSel";
    all = sel+"_"+tr;
    dir = "EvtSelPlots_"+tr  ;

    //plot(dir,"nvtx_"+all,1,0);
    plot(dir,"nvtx_ntrneq0_"+all,1,0);

    //--- Tracks for events with 1 vtx : PV assoc

    if ( itracking == 1 ) {
       tr  = "allTr_gTr_oVtx";
    } else if  ( itracking == 2 ) {
       tr  = "allTr_mbTr_fVtx";
    } else if  ( itracking == 3 ) {
       tr  = "allTr_mbTr_pVtx";
    } else {
       cout << "itraking = " << itracking << " :option not available !!! exit !!! " << endl;
       return;
    }

    sel = "nvtxeq1_evtSel";
    all = sel+"_"+tr;
    dir = "EvtSelPlots_"+tr   +   "/TrackPlots_"+sel+"_"+tr;      

    if ( itracking == 1 ) {

      plot(dir,"dzOedz_"+all);
      plot(dir,"dzOedz_"+all,1);
      plot(dir,"dxyOed0_"+all);
      plot(dir,"dxyOed0_"+all,1);
      plot(dir,"eptOpt_"+all);
      plot(dir,"eptOpt_"+all,1);

    } else if  ( itracking == 2 || itracking == 3) {

      plot(dir,"dzOsz_"+all);
      plot(dir,"dzOsz_"+all,1);
      plot(dir,"dxybsOsxy_"+all);
      plot(dir,"dxybsOsxy_"+all,1);

    }

    //--- Tracks for events with >1 vtx : PV assoc

    if ( itracking == 1 ) {
       tr  = "allTr_gTr_oVtx";
    } else if  ( itracking == 2 ) {
       tr  = "allTr_mbTr_fVtx";
    } else if  ( itracking == 3 ) {
       tr  = "allTr_mbTr_pVtx";
    } else {
       cout << "itraking = " << itracking << " :option not available !!! exit !!! " << endl;
       return;
    }

    sel = "nvtxgt1_evtSel";
    all = sel+"_"+tr;
    dir = "EvtSelPlots_"+tr   +   "/TrackPlots_"+sel+"_"+tr;


    if ( itracking == 1 ) {

      plot(dir,"dzOedz_"+all);
      plot(dir,"dzOedz_"+all,1);
      plot(dir,"dxyOed0_"+all);
      plot(dir,"dxyOed0_"+all,1);
      plot(dir,"eptOpt_"+all);
      plot(dir,"eptOpt_"+all,1);

    } else if  ( itracking == 2 || itracking == 3 ) {

      plot(dir,"dzOsz_"+all);
      plot(dir,"dzOsz_"+all,1);
      plot(dir,"dxybsOsxy_"+all);
      plot(dir,"dxybsOsxy_"+all,1);
   
    }

  }

  //--------- MULTI PLOTS -----------
  if(mltplot){
    sel = "L1_hf_VtxSel";
    if ( itracking == 1 ) {
      tr  = "PV_gTr_oVtx";
    } else if  ( itracking == 2 ) {
      tr  = "PV_mbTr_fVtx";
    } else {
      cout << "itraking = " << itracking << " :option not available !!! exit !!! " << endl;
      return;
    }


    all = sel+"_"+tr;
    dir = "MultiPlots_"+all;
    cout<<dir<<endl;
   
    plot(dir,"eta_"+all,0,1);
    plot(dir,"nch_"+all,1);
    plot(dir,"kno_"+all,1);
    plot(dir,"pt_"+all,1);
    plot(dir,"pt2_"+all,1);
    plot(dir,"rapidity_"+all,1);
  
  }
 

  //-------- EFFICIENCIES --------
  if(effplot){

    globalDirPlot = "../plots.romain/unfoldingv2/";
    cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
    cout << "globalDirPlot= " << globalDirPlot << endl;
    cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;


    plotReset();
    ptcutstr = "newbinning";
    
    // Plot settings
    globalNorm     = 0;
    globalFileType = 2;

    dataSetId.push_back(10);
    dataSetIsMc.push_back(1);
    dataSetStyle.push_back(2);
    dataSetColor.push_back(2);
    dataSetLegend.push_back("L1");

    dataSetId.push_back(10);
    dataSetIsMc.push_back(1);
    dataSetStyle.push_back(2);
    dataSetColor.push_back(3);
    dataSetLegend.push_back("HF");

    dataSetId.push_back(10);
    dataSetIsMc.push_back(1);
    dataSetStyle.push_back(2);
    dataSetColor.push_back(4);
    dataSetLegend.push_back("Vertex");

    dataSetId.push_back(10);
    dataSetIsMc.push_back(1);
    dataSetStyle.push_back(1);
    dataSetColor.push_back(1);
    dataSetLegend.push_back("All");


    // Plots

    dir   = "ptGen0_etaGen2.4_ptReco0.4_etaReco2.4" ;  
    all = "_cut0";
    dataSetHisto.clear();
    dataSetHisto.push_back("eff_L1"+all); 
    dataSetHisto.push_back("eff_hf"+all); 
    dataSetHisto.push_back("eff_vtx"+all); 
    dataSetHisto.push_back("eff_evtSel"+all); 
    plot(dir,"AUTO");

    dir   = "ptGen0_etaGen2.4_ptReco0.15_etaReco2.4" ;  
    all = "_cut5";
    dataSetHisto.clear();
    dataSetHisto.push_back("eff_L1"+all); 
    dataSetHisto.push_back("eff_hf"+all); 
    dataSetHisto.push_back("eff_vtx"+all); 
    dataSetHisto.push_back("eff_evtSel"+all); 
    plot(dir,"AUTO");

    dir   = "ptGen0.2_etaGen2.4_ptReco0.2_etaReco2.4" ;  
    all = "_cut10";
    dataSetHisto.clear();
    dataSetHisto.push_back("eff_L1"+all); 
    dataSetHisto.push_back("eff_hf"+all); 
    dataSetHisto.push_back("eff_vtx"+all); 
    dataSetHisto.push_back("eff_evtSel"+all); 
    plot(dir,"AUTO");

    dir   = "ptGen0.5_etaGen2.4_ptReco0.5_etaReco2.4" ;
    all = "_cut15";
    dataSetHisto.clear();
    dataSetHisto.push_back("eff_L1"+all);
    dataSetHisto.push_back("eff_hf"+all);
    dataSetHisto.push_back("eff_vtx"+all);
    dataSetHisto.push_back("eff_evtSel"+all);
    plot(dir,"AUTO");

  }



/*
SetId.push_back(10);
      dataSetIsMc.push_back(1);
      dataSetStyle.push_back(1);
      dataSetColor.push_back(1);
      dataSetLegend.push_back("PYTHIA D6T");

    if ( itracking == 1 ) {
      tr  = "PV_gTr_oVtx";
    } else if  ( itracking == 2 ) {
      tr  = "PV_mbTr_fVtx";
    } else {
      cout << "itraking = " << itracking << " :option not available !!! exit !!! " << endl;
      return;
    }

    dir = "EvtSelPlots_"+tr   +   "/Eff_RECO_"+tr;
    cout<<dir<<endl;
*/    
    //TH1F* h[20];
    //vector <TH1F*> vh;
/*
    //TH1F* h;
    TCanvas* c1 = new TCanvas("c1","c",200,10,500,500);
    c1->cd();
    
    gROOT->ProcessLine(".X plotMC.C(c1,h,\""+dir+"\",\"eff_nch_L1Sel_"+tr+"\"");
    gROOT->ProcessLine(".X plotMC.C(c1,h,\""+dir+"\",\"eff_nch_hfSel_"+tr+"\",0");
    gROOT->ProcessLine(".X plotMC.C(c1,h,\""+dir+"\",\"eff_nch_vtxSel_"+tr+"\",0");
    gROOT->ProcessLine(".X plotMC.C(c1,h,\""+dir+"\",\"eff_nch_L1_hf_vtxSel_"+tr+"\",0");
    //gPad->Update();
    //gPad->WaitPrimitive();
   
    dir = "EvtSelPlots_"+tr   +   "/Eff_GEN_"+tr;
    gROOT->ProcessLine(".X plotMC.C(c1,h,\""+dir+"\",\"eff_pt_L1Sel_"+tr+"\"");
    gROOT->ProcessLine(".X plotMC.C(c1,h,\""+dir+"\",\"eff_pt_hfSel_"+tr+"\",0");
    gROOT->ProcessLine(".X plotMC.C(c1,h,\""+dir+"\",\"eff_pt_vtxSel_"+tr+"\",0");
    gROOT->ProcessLine(".X plotMC.C(c1,h,\""+dir+"\",\"eff_pt_L1_hf_vtxSel_"+tr+"\",0");
    
    gROOT->ProcessLine(".X plotMC.C(c1,h,\""+dir+"\",\"eff_eta_L1Sel_"+tr+"\"");
    gROOT->ProcessLine(".X plotMC.C(c1,h,\""+dir+"\",\"eff_eta_hfSel_"+tr+"\",0");
    gROOT->ProcessLine(".X plotMC.C(c1,h,\""+dir+"\",\"eff_eta_vtxSel_"+tr+"\",0");
    gROOT->ProcessLine(".X plotMC.C(c1,h,\""+dir+"\",\"eff_eta_L1_hf_vtxSel_"+tr+"\",0");
    //gROOT->ProcessLine(".X plotMC.C(\""+dir+"\",\"eff_nch_vtxSel_"+tr+"\"");
    //gROOT->ProcessLine(".X plotMC.C(\""+dir+"\",\"eff_nch_L1_hf_vtxSel_"+tr+"\"");

  }
*/  
  
  //-------- MATRIX --------
  if(mtxplot){

    globalDirPlot = "../plots.romain/unfoldingv2/";
    cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
    cout << "globalDirPlot= " << globalDirPlot << endl;
    cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;


    plotReset();
    ptcutstr = "newbinning";

    // Plot settings
    globalNorm     = 0;
    globalFileType = 2;
    globalHistoType= 2;
    global2DplotOpt = "col" ;

    dataSetId.push_back(10);
    dataSetIsMc.push_back(1);
    dataSetStyle.push_back(2);
    dataSetColor.push_back(2);
    dataSetLegend.push_back("PYTHYA D6T");


    // Legend Title
    {
      ExtLegTitle = "    genPart:  |#eta|<2.4  p_{T}>0"; 
      std::stringstream LT(""); 
      if (itracking == 1 ) LT << "   genTracks: " ;
      if (itracking == 2 ) LT << "   mbiasTracks: " ;
      LT << "  |#eta|<" << "2.4" ;
      LT << "  p_{T}>"  << "0.4" ;
      LegendTitle = LT.str();
    }

    dir   = "ptGen0_etaGen2.4_ptReco0.4_etaReco2.4/MatrixPlots_evtSel_cut0" ;
    all = "_cut0";
    plot(dir,"nch_matrix_evtSel"+all);


    // Legend Title
    {
      ExtLegTitle = "    genPart:  |#eta|<2.4  p_{T}>0"; 
      std::stringstream LT(""); 
      if (itracking == 1 ) LT << "   genTracks: " ;
      if (itracking == 2 ) LT << "   mbiasTracks: " ;
      LT << "  |#eta|<" << "2.4" ;
      LT << "  p_{T}>"  << "0.15" ;
      LegendTitle = LT.str();
    }

    dir   = "ptGen0_etaGen2.4_ptReco0.15_etaReco2.4/MatrixPlots_evtSel_cut5" ;
    all = "_cut5";
    plot(dir,"nch_matrix_evtSel"+all);


    // Legend Title
    {
      ExtLegTitle = "    genPart:  |#eta|<2.4  p_{T}>0.2"; 
      std::stringstream LT("");
      if (itracking == 1 ) LT << "   genTracks: " ;
      if (itracking == 2 ) LT << "   mbiasTracks: " ;
      LT << "  |#eta|<" << "2.4" ;
      LT << "  p_{T}>"  << "0.2"  ;
      LegendTitle = LT.str();
    }

    dir   = "ptGen0.2_etaGen2.4_ptReco0.2_etaReco2.4/MatrixPlots_evtSel_cut10" ;
    all = "_cut10";
    plot(dir,"nch_matrix_evtSel"+all);

    // Legend Title
    {
      ExtLegTitle = "    genPart:  |#eta|<2.4  p_{T}>0.5"; 
      std::stringstream LT("");
      if (itracking == 1 ) LT << "   genTracks: " ;
      if (itracking == 2 ) LT << "   mbiasTracks: " ;
      LT << "  |#eta|<" << "2.4" ;
      LT << "  p_{T}>"  << "0.5"  ;
      LegendTitle = LT.str();
    }

    dir   = "ptGen0.5_etaGen2.4_ptReco0.5_etaReco2.4/MatrixPlots_evtSel_cut15" ;
    all = "_cut15";
    plot(dir,"nch_matrix_evtSel"+all);

    // Legend Title
    {
      ExtLegTitle = "    genPart:  |#eta|<2.4  p_{T}>1.0"; 
      std::stringstream LT("");
      if (itracking == 1 ) LT << "   genTracks: " ;
      if (itracking == 2 ) LT << "   mbiasTracks: " ;
      LT << "  |#eta|<" << "2.4" ;
      LT << "  p_{T}>"  << "1.0"  ;
      LegendTitle = LT.str();
    }

    dir   = "ptGen1.0_etaGen2.4_ptReco1.0_etaReco2.4/MatrixPlots_evtSel_cut20" ;
    all = "_cut20";
    plot(dir,"nch_matrix_evtSel"+all);

  }


  //-------- ETA/PT/PT2 -------- Correction history
  if( corplot ){

    globalDirPlot = "../plots.romain/unfoldingv2/";
    cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
    cout << "globalDirPlot= " << globalDirPlot << endl;
    cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;

    plotReset();

    // Plot settings
    globalNorm     = 0;
    globalFileType = 3;
    globalHistoType= 1;

    TString bin("0");
    std::stringstream BIN ("");
    BIN << iUfoldBin;
    bin = BIN.str();

    dataSetId.push_back(-1);
    ptcutstr = "hyp1_niter5_cut" + bin + "_DataType0";
    dataSetFile.push_back(fileManager(globalFileType,10,globalEnergy,globalTraking,0,0,ptcutstr,globalDirPlot));
    dataSetIsMc.push_back(0);
    dataSetStyle.push_back(20);
    dataSetColor.push_back(2);
    dataSetLegend.push_back("Data Uncor.");
    dataSetHisto.push_back("eta_data_toCorrect"); 

    dataSetId.push_back(-1);
    ptcutstr = "hyp1_niter5_cut" + bin + "_DataType0";
    dataSetFile.push_back(fileManager(globalFileType,10,globalEnergy,globalTraking,0,0,ptcutstr,globalDirPlot));
    dataSetIsMc.push_back(0);
    dataSetStyle.push_back(20);
    dataSetColor.push_back(4);
    dataSetLegend.push_back("Data");
    dataSetHisto.push_back("eta_corrected"); 

    dataSetId.push_back(-1);
    ptcutstr = "hyp1_niter5_cut" + bin + "_DataType10";
    dataSetFile.push_back(fileManager(globalFileType,10,globalEnergy,globalTraking,0,0,ptcutstr,globalDirPlot));
    dataSetIsMc.push_back(1);
    dataSetStyle.push_back(2);
    dataSetColor.push_back(1);
    dataSetLegend.push_back("Effic.");
    dataSetHisto.push_back("eff_eta");

    dataSetId.push_back(-1);
    ptcutstr = "hyp1_niter5_cut" + bin + "_DataType10";
    dataSetFile.push_back(fileManager(globalFileType,10,globalEnergy,globalTraking,0,0,ptcutstr,globalDirPlot));
    dataSetIsMc.push_back(1);
    dataSetStyle.push_back(1);
    dataSetColor.push_back(2);
    dataSetLegend.push_back("MC Uncor.");
    dataSetHisto.push_back("eta_data_toCorrect");

    dataSetId.push_back(-1);
    ptcutstr = "hyp1_niter5_cut" + bin + "_DataType10";
    dataSetFile.push_back(fileManager(globalFileType,10,globalEnergy,globalTraking,0,0,ptcutstr,globalDirPlot));
    dataSetIsMc.push_back(1);
    dataSetStyle.push_back(1);
    dataSetColor.push_back(4);
    dataSetLegend.push_back("PYTHIA D6T");
    dataSetHisto.push_back("eta_corrected"); 

    // eta
    plot("eta","AUTO",0,1);

    // pt
    dataSetHisto.clear();
    dataSetHisto.push_back("pt_data_toCorrect"); 
    dataSetHisto.push_back("pt_corrected"); 
    dataSetHisto.push_back("eff_pt");
    dataSetHisto.push_back("pt_data_toCorrect");
    dataSetHisto.push_back("pt_corrected");
    plot("pt","AUTO",1,0);

    // pt2
    dataSetHisto.clear();
    dataSetHisto.push_back("pt2_data_toCorrect"); 
    dataSetHisto.push_back("pt2_corrected"); 
    dataSetHisto.push_back("eff_pt2");
    dataSetHisto.push_back("pt2_data_toCorrect");
    dataSetHisto.push_back("pt2_corrected");
    plot("pt2","AUTO",1,0);

/*
    dataSetId.push_back(-1);
    ptcutstr = "hyp1_niter10_cut5_DataType1";
    dataSetFile.push_back(fileManager(globalFileType,10,globalEnergy,globalTraking,0,0,ptcutstr));
    dataSetIsMc.push_back(0);
    dataSetStyle.push_back(20);
    dataSetColor.push_back(2);
    dataSetLegend.push_back("Data");

    dataSetId.push_back(-1);
    ptcutstr = "hyp1_niter10_cut5_DataType0";
    dataSetFile.push_back(fileManager(globalFileType,10,globalEnergy,globalTraking,0,0,ptcutstr));
    dataSetIsMc.push_back(1);
    dataSetStyle.push_back(1);
    dataSetColor.push_back(1);
    dataSetLegend.push_back("PYTHIA D6T");
*/

/*
    dir   = "eta";
    plot(dir,"AUTO",0,0);
    dir   = "pt";
    plot(dir,"pt_corrected",1,0);
*/

/*
    dir   = "pt2";
    plot(dir,"pt2_corrected",1,0);
*/
/*
    dir   = "nch";
    plot(dir,"nch2_corrected",1,0);
*/

  }


  //-------- ETA -------- Final Plot
  if( etaplot ){

    globalDirPlot = "../plots.romain/unfoldingv4/";
    cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
    cout << "globalDirPlot= " << globalDirPlot << endl;
    cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;

    // Fig name
    std::stringstream figname; 
    figname << "dndeta_" ;
    if (iUfoldMCType == 10 )  figname << "MC_D6T_" ;
    if (iUfoldMCType == 11 )  figname << "MC_DW_" ;
    if (iUfoldMCType == 12 )  figname << "MC_P0_" ;
    if (iUfoldMCType == 13 )  figname << "MC_ProQ20_" ;
    //if (globalEnergy == 0.9  ) figname << "0.9TeV_";
    //if (globalEnergy == 2.36 ) figname << "2.36TeV_";
    //if (itracking == 1 )  figname << "gTr_" ;
    //if (itracking == 2 )  figname << "mbTr_" ;
    globalFigBaseName = figname.str();


    plotReset();

    // Plot settings
    globalNorm     = 0;
    globalFileType = 3;
    globalHistoType= 1;
    globalLabel = "CMS internal";
    XaxisTitle = "#eta";
    YaxisTitle = "#frac{1}{N} #frac{dn_{ch}}{d#eta}";
 
    TString bin("5");
//    std::stringstream BIN ("");
//    BIN << iUfoldBin;
//    bin = BIN.str();

    // 0.9 TeV

    dataSetId.push_back(-1);
    ptcutstr = "hyp1_niter5_cut" + bin + "_DataType0";
    dataSetFile.push_back(fileManager(globalFileType,10,0.9,0,0,0,ptcutstr,globalDirPlot));
    dataSetIsMc.push_back(0);
    dataSetStyle.push_back(20);
    dataSetColor.push_back(kRed);
    dataSetLegend.push_back("Data 0.9 TeV - gTr");
    dataSetHisto.push_back("eta_corrected"); 


    dataSetId.push_back(-1);
    ptcutstr = "hyp1_niter5_cut" + bin + "_DataType0";
    dataSetFile.push_back(fileManager(globalFileType,10,0.9,1,0,0,ptcutstr,globalDirPlot));
    dataSetIsMc.push_back(0);
    dataSetStyle.push_back(20);
    dataSetColor.push_back(kBlue);
    dataSetLegend.push_back("Data 0.9 TeV - mbTr");
    dataSetHisto.push_back("eta_corrected"); 

    dataSetId.push_back(-1);
    dataSetFile.push_back("../expdata/cms09_dndeta_0.9TeV");
    dataSetIsMc.push_back(0);
    dataSetStyle.push_back(20);
    dataSetColor.push_back(kBlack);
    dataSetLegend.push_back("CMS 0.9 TeV");
    dataSetHisto.push_back("EXTDATA");

/*
    dataSetId.push_back(-1);
    ptcutstr = "hyp1_niter5_cut" + bin + "_DataType10";
    dataSetFile.push_back(fileManager(globalFileType,10,0.9,0,0,0,ptcutstr,globalDirPlot));
    dataSetIsMc.push_back(1);
    dataSetStyle.push_back(1);
    dataSetColor.push_back(1);
    dataSetLegend.push_back("PYTHIA D6T 0.9 TeV");
    dataSetHisto.push_back("eta_corrected"); 
*/

    // 2.36 TeV

    dataSetId.push_back(-1);
    ptcutstr = "hyp1_niter5_cut" + bin + "_DataType0";
    dataSetFile.push_back(fileManager(globalFileType,10,2.36,0,0,0,ptcutstr,globalDirPlot));
    dataSetIsMc.push_back(0);
    dataSetStyle.push_back(21);
    dataSetColor.push_back(kRed);
    dataSetLegend.push_back("Data 2.36 TeV - gTr");
    dataSetHisto.push_back("eta_corrected"); 


    dataSetId.push_back(-1);
    ptcutstr = "hyp1_niter5_cut" + bin + "_DataType0";
    dataSetFile.push_back(fileManager(globalFileType,10,2.36,1,0,0,ptcutstr,globalDirPlot));
    dataSetIsMc.push_back(0);
    dataSetStyle.push_back(21);
    dataSetColor.push_back(kBlue);
    dataSetLegend.push_back("Data 2.36 TeV - mbTr");
    dataSetHisto.push_back("eta_corrected"); 

    dataSetId.push_back(-1);
    dataSetFile.push_back("../expdata/cms09_dndeta_2.36TeV");
    dataSetIsMc.push_back(0);
    dataSetStyle.push_back(21);
    dataSetColor.push_back(kBlack);
    dataSetLegend.push_back("CMS 2.36 TeV");
    dataSetHisto.push_back("EXTDATA");

/*
    dataSetId.push_back(-1);
    ptcutstr = "hyp1_niter5_cut" + bin + "_DataType10";
    dataSetFile.push_back(fileManager(globalFileType,10,2.36,0,0,0,ptcutstr,globalDirPlot));
    dataSetIsMc.push_back(1);
    dataSetStyle.push_back(2);
    dataSetColor.push_back(1);
    dataSetLegend.push_back("PYTHIA D6T 2.36 TeV");
    dataSetHisto.push_back("eta_corrected"); 
*/

    // eta
    plot("eta","AUTO",0,1);


  }

  //-------- Mch Unfold --------
  if( nchplot ){

    globalDirPlot = "../plots.romain/unfoldingv2/";
    cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
    cout << "globalDirPlot= " << globalDirPlot << endl;
    cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;

    // Retrieve bin info
    if ( iUfoldBin > (signed) accMap.size() ) {
      cout << "[makePlots] iUfoldBin > accMap.size()" << endl;
      return;
    }

    double ptGen   = accMap[iUfoldBin].at(0);
    double etaGen  = accMap[iUfoldBin].at(1);
    double ptReco  = accMap[iUfoldBin].at(2);
    double etaReco = accMap[iUfoldBin].at(3);
    cout << ptGen << " " << etaGen << " " << ptReco << " " << etaReco << endl;


    std::stringstream BIN ("");
    BIN << "hyp" << iUfoldHyp << "_niter" << iUfoldNIter << "_cut" << iUfoldBin << "_DataType" << iUfoldDataType;

    std::stringstream LT("");
    if (itracking == 1 ) LT << "   genTracks: " ;
    if (itracking == 2 ) LT << "   mbiasTracks: " ;
    LT << "  |#eta|<" << etaGen ;
    LT << "  p_{T}>"  << ptGen ;

    std::stringstream ELT("");
    if ( iUfoldMCType == 10 )  ELT << "  Unf.=D6T" ;
    if ( iUfoldMCType == 11 )  ELT << "  Unf.=DW" ;
    if ( iUfoldMCType == 12 )  ELT << "  Unf.=P0" ;
    if ( iUfoldMCType == 13 )  ELT << "  Unf.=ProQ20" ;
    ELT << "  Hyp.="      << iUfoldHyp; 
    ELT << "  Niter=" << iUfoldNIter;  
    
    cout<<BIN.str()<<endl;
    
    plotReset();
    ptcutstr = BIN.str();
    LegendTitle = LT.str();
    ExtLegTitle = ELT.str();
    // Plot settings
    globalNorm     = 0;
    globalFileType = 3;
    globalHistoType= 1;
    XaxisTitle = "n_{ch}";
    YaxisTitle = "#frac{1}{N} #frac{dN}{dn_{ch}}";
    setnchrangeX(iUfoldBin);


    dir   = "unfolding";
    
    dataSetId.push_back(-1);
    dataSetFile.push_back(fileManager(globalFileType,iUfoldMCType,globalEnergy,globalTraking,0,0,ptcutstr,globalDirPlot));
    dataSetIsMc.push_back(0);
    dataSetStyle.push_back(20);
    dataSetColor.push_back(2);
    dataSetLegend.push_back("Data INC");
    dataSetHisto.push_back("nch_data_INC_beforeSDsub");
    
    dataSetId.push_back(-1);
    dataSetFile.push_back(fileManager(globalFileType,iUfoldMCType,globalEnergy,globalTraking,0,0,ptcutstr,globalDirPlot));
    dataSetIsMc.push_back(0);
    dataSetStyle.push_back(20);
    dataSetColor.push_back(4);
    dataSetLegend.push_back("Data NSD");
    dataSetHisto.push_back("nch_data_NSD_afterSDsub");

    dataSetId.push_back(-1);
    dataSetFile.push_back(fileManager(globalFileType,iUfoldMCType,globalEnergy,globalTraking,0,0,ptcutstr,globalDirPlot));
    dataSetIsMc.push_back(1);
    dataSetStyle.push_back(1);
    dataSetColor.push_back(1);
    dataSetLegend.push_back("NSD PYTHIA D6T");
    dataSetHisto.push_back("nch_MC_gen_NSD");

    dataSetId.push_back(-1);
    dataSetFile.push_back(fileManager(globalFileType,iUfoldMCType,globalEnergy,globalTraking,0,0,ptcutstr,globalDirPlot));
    dataSetIsMc.push_back(1);
    dataSetStyle.push_back(1);
    dataSetColor.push_back(2);
    dataSetLegend.push_back("SD PYTHIA D6T");
    dataSetHisto.push_back("nch_MC_gen_SD");


    // Fig name
    {
    std::stringstream figname; 
    figname << "unfolding_step1_" ;
    if (iUfoldMCType == 10 )  figname << "MC_D6T_" ;
    if (iUfoldMCType == 11 )  figname << "MC_DW_" ;
    if (iUfoldMCType == 12 )  figname << "MC_P0_" ;
    if (iUfoldMCType == 13 )  figname << "MC_ProQ20_" ;
    if (globalEnergy == 0.9  ) figname << "0.9TeV_";
    if (globalEnergy == 2.36 ) figname << "2.36TeV_";
    if (itracking == 1 )  figname << "gTr_" ;
    if (itracking == 2 )  figname << "mbTr_" ;
    globalFigBaseName = figname.str();
    }    

    plot(dir,"AUTO");
    
    
    plotReset();
    ptcutstr = BIN.str();
    LegendTitle = LT.str();
    ExtLegTitle = ELT.str();
    
    // Plot settings
    globalNorm     = 2;
    globalFileType = 3;
    globalHistoType= 1;
    XaxisTitle = "n_{ch}";
    YaxisTitle = "#frac{1}{N} #frac{dN}{dn_{ch}}";
    setnchrangeX(iUfoldBin);
    
    dataSetId.push_back(-1);
    dataSetFile.push_back(fileManager(globalFileType,iUfoldMCType,globalEnergy,globalTraking,0,0,ptcutstr,globalDirPlot));
    dataSetIsMc.push_back(0);
    dataSetStyle.push_back(20);
    dataSetColor.push_back(2);
    dataSetLegend.push_back("Data NSD");
    dataSetHisto.push_back("nch_data_NSD_afterSDsub");
    
    dataSetId.push_back(-1);
    dataSetFile.push_back(fileManager(globalFileType,iUfoldMCType,globalEnergy,globalTraking,0,0,ptcutstr,globalDirPlot));
    dataSetIsMc.push_back(0);
    dataSetStyle.push_back(20);
    dataSetColor.push_back(4);
    dataSetLegend.push_back("Data Unfolded");
    dataSetHisto.push_back("nch_unfoldedPtr");

    dataSetId.push_back(-1);
    dataSetFile.push_back(fileManager(globalFileType,iUfoldMCType,globalEnergy,globalTraking,0,0,ptcutstr,globalDirPlot));
    dataSetIsMc.push_back(1);
    dataSetStyle.push_back(1);
    dataSetColor.push_back(1);
    dataSetLegend.push_back("Reco NSD PYTHIA D6T");
    dataSetHisto.push_back("nch_MC_gen_NSD");

    dataSetId.push_back(-1);
    dataSetFile.push_back(fileManager(globalFileType,iUfoldMCType,globalEnergy,globalTraking,0,0,ptcutstr,globalDirPlot));
    dataSetIsMc.push_back(1);
    dataSetStyle.push_back(1);
    dataSetColor.push_back(2);
    dataSetLegend.push_back("Gen NSD PYTHIA D6T");
    dataSetHisto.push_back("nch_MC_gen_afterUnfolding");

    // Fig name
    {
    std::stringstream figname; 
    figname << "unfolding_step2_" ;
    if (iUfoldMCType == 10 )  figname << "MC_D6T_" ;
    if (iUfoldMCType == 11 )  figname << "MC_DW_" ;
    if (iUfoldMCType == 12 )  figname << "MC_P0_" ;
    if (iUfoldMCType == 13 )  figname << "MC_ProQ20_" ;
    if (globalEnergy == 0.9  ) figname << "0.9TeV_";
    if (globalEnergy == 2.36 ) figname << "2.36TeV_";
    if (itracking == 1 )  figname << "gTr_" ;
    if (itracking == 2 )  figname << "mbTr_" ;
    globalFigBaseName = figname.str();
    }    

   
    plot(dir,"AUTO");
    
    
    plotReset();
    ptcutstr = BIN.str();
    LegendTitle = LT.str();
    ExtLegTitle = ELT.str();
    // Plot settings
    globalNorm     = 2;
    globalFileType = 3;
    globalHistoType= 1;
    XaxisTitle = "n_{ch}";
    YaxisTitle = "#frac{1}{N} #frac{dN}{dn_{ch}}";
    setnchrangeX(iUfoldBin);

    dataSetId.push_back(-1);
    dataSetFile.push_back(fileManager(globalFileType,iUfoldMCType,globalEnergy,globalTraking,0,0,ptcutstr,globalDirPlot));
    dataSetIsMc.push_back(0);
    dataSetStyle.push_back(20);
    dataSetColor.push_back(2);
    dataSetLegend.push_back("Data Unfolded");
    dataSetHisto.push_back("nch_unfoldedPtr");

    dataSetId.push_back(-1);
    dataSetFile.push_back(fileManager(globalFileType,iUfoldMCType,globalEnergy,globalTraking,0,0,ptcutstr,globalDirPlot));
    dataSetIsMc.push_back(0);
    dataSetStyle.push_back(20);
    dataSetColor.push_back(4);
    dataSetLegend.push_back("Data Corrected");
    dataSetHisto.push_back("nch_data_corrected");

    dataSetId.push_back(-1);
    dataSetFile.push_back(fileManager(globalFileType,iUfoldMCType,globalEnergy,globalTraking,0,0,ptcutstr,globalDirPlot));
    dataSetIsMc.push_back(1);
    dataSetStyle.push_back(1);
    dataSetColor.push_back(2);
    dataSetLegend.push_back("Gen NSD PYTHIA D6T");
    dataSetHisto.push_back("nch_MC_gen_afterUnfolding");

    dataSetId.push_back(-1);
    dataSetFile.push_back(fileManager(globalFileType,iUfoldMCType,globalEnergy,globalTraking,0,0,ptcutstr,globalDirPlot));
    dataSetIsMc.push_back(1);
    dataSetStyle.push_back(1);
    dataSetColor.push_back(4 );
    dataSetLegend.push_back("Gen NSD w/o evtSel PYTHIA D6T");
    dataSetHisto.push_back("nch_MC_gen_afterEvtSelCorrection");

    // Fig name
    {
    std::stringstream figname; 
    figname << "unfolding_step3_" ;
    if (iUfoldMCType == 10 )  figname << "MC_D6T_" ;
    if (iUfoldMCType == 11 )  figname << "MC_DW_" ;
    if (iUfoldMCType == 12 )  figname << "MC_P0_" ;
    if (iUfoldMCType == 13 )  figname << "MC_ProQ20_" ;
    if (globalEnergy == 0.9  ) figname << "0.9TeV_";
    if (globalEnergy == 2.36 ) figname << "2.36TeV_";
    if (itracking == 1 )  figname << "gTr_" ;
    if (itracking == 2 )  figname << "mbTr_" ;
    globalFigBaseName = figname.str();
    }    

   
    plot(dir,"AUTO",1,2);
    
    
    /*dataSetHisto.clear();
    dataSetHisto.push_back("nch_data_corrected");
    dataSetHisto.push_back("nch_MC_gen_afterEvtSelCorrection");
    plot(dir,"AUTO");
    
    plot(dir,"nch_data_INC_beforeSDsub",0,1); 
    plot(dir,"nch_data_NSD_afterSDsub",0,1); 
    plot(dir,"nch_unfoldedPtr",0,1); 
    plot(dir,"nch_data_corrected",0,1); */
 

  }

  //-------- Mch Unfold -------- Compare Trackings
  if( nchctrk ){

    globalDirPlot = "../plots.romain/unfoldingv2/";
    cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
    cout << "globalDirPlot= " << globalDirPlot << endl;
    cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;

    // Retrieve bin info
    if ( iUfoldBin > (signed) accMap.size() ) {
      cout << "[makePlots] iUfoldBin > accMap.size()" << endl;
      return;
    }

    double ptGen   = accMap[iUfoldBin].at(0);
    double etaGen  = accMap[iUfoldBin].at(1);
    double ptReco  = accMap[iUfoldBin].at(2);
    double etaReco = accMap[iUfoldBin].at(3);
    cout << ptGen << " " << etaGen << " " << ptReco << " " << etaReco << endl;
  
    std::stringstream BIN ("");
    BIN << "hyp" << iUfoldHyp << "_niter" << iUfoldNIter << "_cut" << iUfoldBin << "_DataType" << iUfoldDataType;

    std::stringstream LT("");
    LT << "  |#eta|<" << etaGen ;
    LT << "  p_{T}>"  << ptGen ;

    std::stringstream ELT("");
    if ( iUfoldMCType == 10 )  ELT << "  Unf.=D6T" ;
    if ( iUfoldMCType == 11 )  ELT << "  Unf.=DW" ;
    if ( iUfoldMCType == 12 )  ELT << "  Unf.=P0" ;
    if ( iUfoldMCType == 13 )  ELT << "  Unf.=ProQ20" ;
    ELT << "  Hyp.="      << iUfoldHyp; 
    ELT << "  N_{iter}=" << iUfoldNIter;  

    std::string datatype;
    if ( iUfoldDataType ==  0 ) datatype = "Data";
    if ( iUfoldDataType == 10 ) datatype = "D6T";
    if ( iUfoldDataType == 11 ) datatype = "DW";
    if ( iUfoldDataType == 12 ) datatype = "P0";
    if ( iUfoldDataType == 13 ) datatype = "ProQ20";
  
    std::stringstream LD1("");
    std::stringstream LD2("");
    LD1 << datatype << " - genTracks p_{T}>" << ptReco ;
    LD2 << datatype << " - mbiasTracks p_{T}>" << ptReco ;

    // Fig name

    std::stringstream figname; 
    figname << "nch_ctracking_" ;
    if (iUfoldMCType == 10 )  figname << "MC_D6T_" ;
    if (iUfoldMCType == 11 )  figname << "MC_DW_" ;
    if (iUfoldMCType == 12 )  figname << "MC_P0_" ;
    if (iUfoldMCType == 13 )  figname << "MC_ProQ20_" ;
    if (globalEnergy == 0.9  ) figname << "0.9TeV_";
    if (globalEnergy == 2.36 ) figname << "2.36TeV_";
    globalFigBaseName = figname.str();


    plotReset();
    ptcutstr = BIN.str();
    LegendTitle = LT.str();
    ExtLegTitle = ELT.str();
    // Plot settings
    globalNorm     = 2;
    globalFileType = 3;
    globalHistoType= 1;
    XaxisTitle = "n_{ch}";
    YaxisTitle = "#frac{1}{N} #frac{dN}{dn_{ch}}";
    setnchrangeX(iUfoldBin);

    dir   = "unfolding";
    
    dataSetId.push_back(-1);
    dataSetFile.push_back(fileManager(globalFileType,iUfoldMCType,globalEnergy,0,0,0,ptcutstr,globalDirPlot));
    dataSetIsMc.push_back(0);
    dataSetStyle.push_back(20);
    dataSetColor.push_back(kRed);
    dataSetLegend.push_back(LD1.str());
    dataSetHisto.push_back("nch_data_corrected");

    dataSetId.push_back(-1);
    dataSetFile.push_back(fileManager(globalFileType,iUfoldMCType,globalEnergy,1,0,0,ptcutstr,globalDirPlot));
    dataSetIsMc.push_back(0);
    dataSetStyle.push_back(20);
    dataSetColor.push_back(kBlue);
    dataSetLegend.push_back(LD2.str());
    dataSetHisto.push_back("nch_data_corrected");

    dataSetId.push_back(-1);
    dataSetFile.push_back(fileManager(globalFileType,iUfoldMCType,globalEnergy,0,0,0,ptcutstr,globalDirPlot));
    dataSetIsMc.push_back(1);
    dataSetStyle.push_back(1);
    dataSetColor.push_back(1);
    dataSetLegend.push_back("NSD - PYTHIA D6T");
    dataSetHisto.push_back("nch_MC_gen_afterEvtSelCorrection");
   
    plot(dir,"AUTO",1,2);
    



  }



  //-------- Mch Unfold -------- Compare Binnings 
  if( nchcbin ){

//    iUfoldDataType = 0;

    globalDirPlot = "../plots.romain/unfoldingv2/";
    cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
    cout << "globalDirPlot= " << globalDirPlot << endl;
    cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;

    int iUfoldBin1 = iUfoldBin + 0 ;
    int iUfoldBin2 = iUfoldBin + 5 ;

    // Retrieve bin info --> Bin 1
    if ( iUfoldBin1 > (signed) accMap.size() ) {
      cout << "[makePlots] iUfoldBin1 > accMap.size()" << endl;
      return;
    }

    double ptGen1   = accMap[iUfoldBin1].at(0);
    double etaGen1  = accMap[iUfoldBin1].at(1);
    double ptReco1  = accMap[iUfoldBin1].at(2);
    double etaReco1 = accMap[iUfoldBin1].at(3);
    cout << ptGen1 << " " << etaGen1 << " " << ptReco1 << " " << etaReco1 << endl;

    std::stringstream BIN1 ("");
    BIN1 << "hyp" << iUfoldHyp << "_niter" << iUfoldNIter << "_cut" << iUfoldBin1 << "_DataType" << iUfoldDataType;

    // Retrieve bin info --> Bin 2
    if ( iUfoldBin2 > (signed) accMap.size() ) {
      cout << "[makePlots] iUfoldBin2 > accMap.size()" << endl;
      return;
    }

    double ptGen2   = accMap[iUfoldBin2].at(0);
    double etaGen2  = accMap[iUfoldBin2].at(1);
    double ptReco2  = accMap[iUfoldBin2].at(2);
    double etaReco2 = accMap[iUfoldBin2].at(3);
    cout << ptGen2 << " " << etaGen2 << " " << ptReco2 << " " << etaReco2 << endl;
  
    std::stringstream BIN2 ("");
    BIN2 << "hyp" << iUfoldHyp << "_niter" << iUfoldNIter << "_cut" << iUfoldBin2 << "_DataType" << iUfoldDataType;

    // Check Consistency of both bins
    if (   ( ptGen1 != ptGen2 ) || ( etaGen1 != etaGen2 ) || ( etaReco1 != etaReco2 )  ) {
      cout << "[makePlots] iUfoldBin1 <-> iUfoldBin2 are incompatible" << endl;
      return;
    }

    //
  
    std::stringstream LT("");
    LT << "  |#eta|<" << etaGen1 ;
    LT << "  p_{T}>"  << ptGen1  ;

    std::stringstream ELT("");
    if ( iUfoldMCType == 10 )  ELT << "  Unf.=D6T" ;
    if ( iUfoldMCType == 11 )  ELT << "  Unf.=DW" ;
    if ( iUfoldMCType == 12 )  ELT << "  Unf.=P0" ;
    if ( iUfoldMCType == 13 )  ELT << "  Unf.=ProQ20" ;
    ELT << "  Hyp.="      << iUfoldHyp; 
    ELT << "  N_{iter}=" << iUfoldNIter;  
   
    std::string datatype;
    if ( iUfoldDataType ==  0 ) datatype = "Data";
    if ( iUfoldDataType == 10 ) datatype = "D6T";
    if ( iUfoldDataType == 11 ) datatype = "DW";
    if ( iUfoldDataType == 12 ) datatype = "P0";
    if ( iUfoldDataType == 13 ) datatype = "ProQ20";
 
    std::stringstream LD1("");
    std::stringstream LD2("");
    std::stringstream LD3("");
    std::stringstream LD4("");
    LD1 << datatype << " - genTracks p_{T}>"   << ptReco1 ;
    LD2 << datatype << " - mbiasTracks p_{T}>" << ptReco1 ;
    LD3 << datatype << " - genTracks p_{T}>"   << ptReco2 ;
    LD4 << datatype << " - mbiasTracks p_{T}>" << ptReco2 ;

    // Fig name

    std::stringstream figname; 
    figname << "nch_cbin_" ;
    if (iUfoldMCType == 10 )  figname << "MC_D6T_" ;
    if (iUfoldMCType == 11 )  figname << "MC_DW_" ;
    if (iUfoldMCType == 12 )  figname << "MC_P0_" ;
    if (iUfoldMCType == 13 )  figname << "MC_ProQ20_" ;
    if (globalEnergy == 0.9  ) figname << "0.9TeV_";
    if (globalEnergy == 2.36 ) figname << "2.36TeV_";
    if (itracking == 1 )  figname << "gTr_" ;
    if (itracking == 2 )  figname << "mbTr_" ;
    globalFigBaseName = figname.str();


    plotReset();
    LegendTitle = LT.str();
    ExtLegTitle = ELT.str();
    // Plot settings
    globalNorm     = 3;
    globalFileType = 3;
    globalHistoType= 1;
    XaxisTitle = "n_{ch}";
    YaxisTitle = "#frac{1}{N} #frac{dN}{dn_{ch}}";
    setnchrangeX(iUfoldBin);

    dir   = "unfolding";
    
    ptcutstr = BIN1.str();
    dataSetId.push_back(-1);
    dataSetFile.push_back(fileManager(globalFileType,iUfoldMCType,globalEnergy,0,0,0,ptcutstr,globalDirPlot));
    dataSetIsMc.push_back(0);
    dataSetStyle.push_back(24);
    dataSetColor.push_back(kRed);
    dataSetLegend.push_back(LD1.str());
    dataSetHisto.push_back("nch_data_corrected");

    ptcutstr = BIN1.str();
    dataSetId.push_back(-1);
    dataSetFile.push_back(fileManager(globalFileType,iUfoldMCType,globalEnergy,1,0,0,ptcutstr,globalDirPlot));
    dataSetIsMc.push_back(0);
    dataSetStyle.push_back(24);
    dataSetColor.push_back(kGreen);
    dataSetLegend.push_back(LD2.str());
    dataSetHisto.push_back("nch_data_corrected");

    ptcutstr = BIN2.str();
    dataSetId.push_back(-1);
    dataSetFile.push_back(fileManager(globalFileType,iUfoldMCType,globalEnergy,0,0,0,ptcutstr,globalDirPlot));
    dataSetIsMc.push_back(0);
    dataSetStyle.push_back(24);
    dataSetColor.push_back(kBlack);
    dataSetLegend.push_back(LD3.str());
    dataSetHisto.push_back("nch_data_corrected");


    ptcutstr = BIN2.str();
    dataSetId.push_back(-1);
    dataSetFile.push_back(fileManager(globalFileType,iUfoldMCType,globalEnergy,1,0,0,ptcutstr,globalDirPlot));
    dataSetIsMc.push_back(0);
    dataSetStyle.push_back(24);
    dataSetColor.push_back(kBlue);
    dataSetLegend.push_back(LD4.str());
    dataSetHisto.push_back("nch_data_corrected");



    dataSetId.push_back(-1);
    dataSetFile.push_back(fileManager(globalFileType,iUfoldMCType,globalEnergy,0,0,0,ptcutstr,globalDirPlot));
    dataSetIsMc.push_back(1);
    dataSetStyle.push_back(1);
    dataSetColor.push_back(1);
    dataSetLegend.push_back("NSD - PYTHIA D6T");
    dataSetHisto.push_back("nch_MC_gen_afterEvtSelCorrection");

   
    plot(dir,"AUTO",1,2);
    
  }


  //-------- Mch Unfold -------- Compare Unf. MC
  if( nchcunf ){

    globalDirPlot = "../plots.romain/unfoldingv2/";
    cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
    cout << "globalDirPlot= " << globalDirPlot << endl;
    cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;

    // Retrieve bin info
    if ( iUfoldBin > (signed) accMap.size() ) {
      cout << "[makePlots] iUfoldBin > accMap.size()" << endl;
      return;
    }

    double ptGen   = accMap[iUfoldBin].at(0);
    double etaGen  = accMap[iUfoldBin].at(1);
    double ptReco  = accMap[iUfoldBin].at(2);
    double etaReco = accMap[iUfoldBin].at(3);
    cout << ptGen << " " << etaGen << " " << ptReco << " " << etaReco << endl;
  
    std::stringstream BIN ("");
    BIN << "hyp" << iUfoldHyp << "_niter" << iUfoldNIter << "_cut" << iUfoldBin << "_DataType" << iUfoldDataType;

    std::stringstream LT("");
    LT << "  |#eta|<" << etaGen ;
    LT << "  p_{T}>"  << ptGen ;

    std::stringstream ELT("");
    ELT << "  Hyp.="      << iUfoldHyp; 
    ELT << "  N_{iter}=" << iUfoldNIter;  
    if (globalTraking==0) ELT << "  gTr - PT>" << ptReco;
    if (globalTraking==1) ELT << " mbTr - PT>" << ptReco;

    std::string datatype;
    if ( iUfoldDataType ==  0 ) datatype = "Data";
    if ( iUfoldDataType == 10 ) datatype = "D6T";
    if ( iUfoldDataType == 11 ) datatype = "DW";
    if ( iUfoldDataType == 12 ) datatype = "P0";
    if ( iUfoldDataType == 13 ) datatype = "ProQ20";

    std::stringstream LD1("");
    std::stringstream LD2("");
    LD1 << datatype << " - Unf.= D6T" ;
    LD2 << datatype << " - Unf.= P0"  ;

    // Fig name

    std::stringstream figname; 
    figname << "nch_cunfold_D6T_P0_" ;
    if (globalEnergy == 0.9  ) figname << "0.9TeV_";
    if (globalEnergy == 2.36 ) figname << "2.36TeV_";
    if (itracking == 1 )  figname << "gTr_" ;
    if (itracking == 2 )  figname << "mbTr_" ;
    globalFigBaseName = figname.str();


    plotReset();
    ptcutstr = BIN.str();
    LegendTitle = LT.str();
    ExtLegTitle = ELT.str();
    // Plot settings
    globalNorm     = 2;
    globalFileType = 3;
    globalHistoType= 1;
    XaxisTitle = "n_{ch}";
    YaxisTitle = "#frac{1}{N} #frac{dN}{dn_{ch}}";
    setnchrangeX(iUfoldBin);

    dir   = "unfolding";
    
    dataSetId.push_back(-1);
    dataSetFile.push_back(fileManager(globalFileType,10,globalEnergy,globalTraking,0,0,ptcutstr,globalDirPlot));
    dataSetIsMc.push_back(0);
    dataSetStyle.push_back(20);
    dataSetColor.push_back(kRed);
    dataSetLegend.push_back(LD1.str());
    dataSetHisto.push_back("nch_data_corrected");

    dataSetId.push_back(-1);
    dataSetFile.push_back(fileManager(globalFileType,12,globalEnergy,globalTraking,0,0,ptcutstr,globalDirPlot));
    dataSetIsMc.push_back(0);
    dataSetStyle.push_back(20);
    dataSetColor.push_back(kBlue);
    dataSetLegend.push_back(LD2.str());
    dataSetHisto.push_back("nch_data_corrected");

    dataSetId.push_back(-1);
    dataSetFile.push_back(fileManager(globalFileType,10,globalEnergy,0,0,0,ptcutstr,globalDirPlot));
    dataSetIsMc.push_back(1);
    dataSetStyle.push_back(1);
    dataSetColor.push_back(1);
    dataSetLegend.push_back("NSD - PYTHIA D6T");
    dataSetHisto.push_back("nch_MC_gen_afterEvtSelCorrection");
   
    plot(dir,"AUTO",1,2);
    
  }

  //-------- Mch Unfold -------- Compare Unf. MC at different energies
  if( nchcene ){

    globalDirPlot = "../plots.romain/unfoldingv2/";
    cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
    cout << "globalDirPlot= " << globalDirPlot << endl;
    cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;

    // Retrieve bin info
    if ( iUfoldBin > (signed) accMap.size() ) {
      cout << "[makePlots] iUfoldBin > accMap.size()" << endl;
      return;
    }

    double ptGen   = accMap[iUfoldBin].at(0);
    double etaGen  = accMap[iUfoldBin].at(1);
    double ptReco  = accMap[iUfoldBin].at(2);
    double etaReco = accMap[iUfoldBin].at(3);
    cout << ptGen << " " << etaGen << " " << ptReco << " " << etaReco << endl;
  
    std::stringstream BIN1 ("");
    std::stringstream BIN2 ("");
    BIN1 << "hyp" << iUfoldHyp << "_niter" << iUfoldNIter << "_cut" << iUfoldBin << "_DataType" << iUfoldDataType;
    BIN2 << "hyp" << iUfoldHyp << "_niter" << iUfoldNIter << "_cut" << iUfoldBin << "_DataType" << iUfoldDataType;
    if (globalEnergy == 0.9 ) BIN2 << "_Emc2.36";
    if (globalEnergy == 2.36) BIN2 << "_Emc0.9";

    std::stringstream LT("");
    LT << "  |#eta|<" << etaGen ;
    LT << "  p_{T}>"  << ptGen ;

    std::stringstream ELT("");
    if ( iUfoldMCType == 10 )  ELT << "  Unf.=D6T" ;
    if ( iUfoldMCType == 11 )  ELT << "  Unf.=DW" ;
    if ( iUfoldMCType == 12 )  ELT << "  Unf.=P0" ;
    if ( iUfoldMCType == 13 )  ELT << "  Unf.=ProQ20" ;
    ELT << "  Hyp.="      << iUfoldHyp; 
    ELT << "  N_{iter}=" << iUfoldNIter;  
    if (globalTraking==0) ELT << "  gTr - PT>" << ptReco;
    if (globalTraking==1) ELT << " mbTr - PT>" << ptReco;

    std::string datatype;
    if ( iUfoldDataType ==  0 ) datatype = "Data";
    if ( iUfoldDataType == 10 ) datatype = "D6T";
    if ( iUfoldDataType == 11 ) datatype = "DW";
    if ( iUfoldDataType == 12 ) datatype = "P0";
    if ( iUfoldDataType == 13 ) datatype = "ProQ20";

    std::stringstream LD1("");
    std::stringstream LD2("");
    if (globalEnergy == 0.9 ) {
      LD1 << datatype << " - Unf@0.9" ;
      LD2 << datatype << " - Unf@2.36"  ;
    } else {
      LD2 << datatype << " - Unf@0.9" ;
      LD1 << datatype << " - Unf@2.36"  ;
    }

    // Fig name

    std::stringstream figname; 
    figname << "nch_cenergy_0.9-2.36_" ;
    if (iUfoldMCType == 10 )  figname << "MC_D6T_" ;
    if (iUfoldMCType == 11 )  figname << "MC_DW_" ;
    if (iUfoldMCType == 12 )  figname << "MC_P0_" ;
    if (iUfoldMCType == 13 )  figname << "MC_ProQ20_" ;
    if (globalEnergy == 0.9  ) figname << "0.9TeV_";
    if (globalEnergy == 2.36 ) figname << "2.36TeV_";
    if (itracking == 1 )  figname << "gTr_" ;
    if (itracking == 2 )  figname << "mbTr_" ;
    globalFigBaseName = figname.str();


    plotReset();
    LegendTitle = LT.str();
    ExtLegTitle = ELT.str();
    // Plot settings
    globalNorm     = 2;
    globalFileType = 3;
    globalHistoType= 1;
    XaxisTitle = "n_{ch}";
    YaxisTitle = "#frac{1}{N} #frac{dN}{dn_{ch}}";
    setnchrangeX(iUfoldBin);

    dir   = "unfolding";
    
    ptcutstr = BIN1.str();
    dataSetId.push_back(-1);
    dataSetFile.push_back(fileManager(globalFileType,iUfoldMCType,globalEnergy,globalTraking,0,0,ptcutstr,globalDirPlot));
    dataSetIsMc.push_back(0);
    dataSetStyle.push_back(20);
    dataSetColor.push_back(kRed);
    dataSetLegend.push_back(LD1.str());
    dataSetHisto.push_back("nch_data_corrected");

    ptcutstr = BIN2.str();
    dataSetId.push_back(-1);
    dataSetFile.push_back(fileManager(globalFileType,iUfoldMCType,globalEnergy,globalTraking,0,0,ptcutstr,globalDirPlot));
    dataSetIsMc.push_back(0);
    dataSetStyle.push_back(20);
    dataSetColor.push_back(kBlue);
    dataSetLegend.push_back(LD2.str());
    dataSetHisto.push_back("nch_data_corrected");

    ptcutstr = BIN1.str();
    dataSetId.push_back(-1);
    dataSetFile.push_back(fileManager(globalFileType,iUfoldMCType,globalEnergy,0,0,0,ptcutstr,globalDirPlot));
    dataSetIsMc.push_back(1);
    dataSetStyle.push_back(1);
    dataSetColor.push_back(1);
    dataSetLegend.push_back("NSD - PYTHIA D6T");
    dataSetHisto.push_back("nch_MC_gen_afterEvtSelCorrection");
   
    plot(dir,"AUTO",1,2);
    
  }


  //-------- Mch Unfold -------- Compare Unf. Hypothesis
  if( nchchyp ){

    globalDirPlot = "../plots.romain/unfoldingv2/";
    cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
    cout << "globalDirPlot= " << globalDirPlot << endl;
    cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;

    // Retrieve bin info
    if ( iUfoldBin > (signed) accMap.size() ) {
      cout << "[makePlots] iUfoldBin > accMap.size()" << endl;
      return;
    }

    double ptGen   = accMap[iUfoldBin].at(0);
    double etaGen  = accMap[iUfoldBin].at(1);
    double ptReco  = accMap[iUfoldBin].at(2);
    double etaReco = accMap[iUfoldBin].at(3);
    cout << ptGen << " " << etaGen << " " << ptReco << " " << etaReco << endl;
  
    std::stringstream BIN1 ("");
    std::stringstream BIN2 ("");
    BIN1 << "hyp0" << "_niter" << iUfoldNIter << "_cut" << iUfoldBin << "_DataType" << iUfoldDataType;
    BIN2 << "hyp1" << "_niter" << iUfoldNIter << "_cut" << iUfoldBin << "_DataType" << iUfoldDataType;

    std::stringstream LT("");
    LT << "  |#eta|<" << etaGen ;
    LT << "  p_{T}>"  << ptGen ;

    std::stringstream ELT("");
    if ( iUfoldMCType == 10 )  ELT << "  Unf.=D6T" ;
    if ( iUfoldMCType == 11 )  ELT << "  Unf.=DW" ;
    if ( iUfoldMCType == 12 )  ELT << "  Unf.=P0" ;
    if ( iUfoldMCType == 13 )  ELT << "  Unf.=ProQ20" ;
    ELT << "  N_{iter}=" << iUfoldNIter;  
    if (globalTraking==0) ELT << "  gTr - PT>" << ptReco;
    if (globalTraking==1) ELT << " mbTr - PT>" << ptReco;

    std::string datatype;
    if ( iUfoldDataType ==  0 ) datatype = "Data";
    if ( iUfoldDataType == 10 ) datatype = "D6T";
    if ( iUfoldDataType == 11 ) datatype = "DW";
    if ( iUfoldDataType == 12 ) datatype = "P0";
    if ( iUfoldDataType == 13 ) datatype = "ProQ20";

    std::stringstream LD1("");
    std::stringstream LD2("");
    LD1 << datatype << " - Hyp.= 0";
    LD2 << datatype << " - Hyp.= 1";

    // Fig name

    std::stringstream figname; 
    figname << "nch_chyp_0-1_" ;
    if (iUfoldMCType == 10 )  figname << "MC_D6T_" ;
    if (iUfoldMCType == 11 )  figname << "MC_DW_" ;
    if (iUfoldMCType == 12 )  figname << "MC_P0_" ;
    if (iUfoldMCType == 13 )  figname << "MC_ProQ20_" ;
    if (globalEnergy == 0.9  ) figname << "0.9TeV_";
    if (globalEnergy == 2.36 ) figname << "2.36TeV_";
    if (itracking == 1 )  figname << "gTr_" ;
    if (itracking == 2 )  figname << "mbTr_" ;
    globalFigBaseName = figname.str();


    plotReset();
    LegendTitle = LT.str();
    ExtLegTitle = ELT.str();
    // Plot settings
    globalNorm     = 2;
    globalFileType = 3;
    globalHistoType= 1;
    XaxisTitle = "n_{ch}";
    YaxisTitle = "#frac{1}{N} #frac{dN}{dn_{ch}}";

    setnchrangeX(iUfoldBin);
    dir   = "unfolding";
    
    ptcutstr = BIN1.str();
    dataSetId.push_back(-1);
    dataSetFile.push_back(fileManager(globalFileType,10,globalEnergy,globalTraking,0,0,ptcutstr,globalDirPlot));
    dataSetIsMc.push_back(0);
    dataSetStyle.push_back(20);
    dataSetColor.push_back(kRed);
    dataSetLegend.push_back(LD1.str());
    dataSetHisto.push_back("nch_data_corrected");

    ptcutstr = BIN2.str();
    dataSetId.push_back(-1);
    dataSetFile.push_back(fileManager(globalFileType,10,globalEnergy,globalTraking,0,0,ptcutstr,globalDirPlot));
    dataSetIsMc.push_back(0);
    dataSetStyle.push_back(20);
    dataSetColor.push_back(kBlue);
    dataSetLegend.push_back(LD2.str());
    dataSetHisto.push_back("nch_data_corrected");

    dataSetId.push_back(-1);
    dataSetFile.push_back(fileManager(globalFileType,10,globalEnergy,0,0,0,ptcutstr,globalDirPlot));
    dataSetIsMc.push_back(1);
    dataSetStyle.push_back(1);
    dataSetColor.push_back(1);
    dataSetLegend.push_back("NSD - PYTHIA D6T");
    dataSetHisto.push_back("nch_MC_gen_afterEvtSelCorrection");
   
    plot(dir,"AUTO",1,2);
    
  }

  //-------- Mch Unfold -------- Compare Unf. Iterations
  if( nchcitr ){

    globalDirPlot = "../plots.romain/unfoldingv2/";
    cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
    cout << "globalDirPlot= " << globalDirPlot << endl;
    cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;

    // Retrieve bin info
    if ( iUfoldBin > (signed) accMap.size() ) {
      cout << "[makePlots] iUfoldBin > accMap.size()" << endl;
      return;
    }

    double ptGen   = accMap[iUfoldBin].at(0);
    double etaGen  = accMap[iUfoldBin].at(1);
    double ptReco  = accMap[iUfoldBin].at(2);
    double etaReco = accMap[iUfoldBin].at(3);
    cout << ptGen << " " << etaGen << " " << ptReco << " " << etaReco << endl;
  
    std::stringstream BIN1 ("");
    std::stringstream BIN2 ("");
    std::stringstream BIN3 ("");
    std::stringstream BIN4 ("");
    BIN1 << "hyp" << iUfoldHyp << "_niter3" << "_cut" << iUfoldBin << "_DataType" << iUfoldDataType;
    BIN2 << "hyp" << iUfoldHyp << "_niter5" << "_cut" << iUfoldBin << "_DataType" << iUfoldDataType;
    BIN3 << "hyp" << iUfoldHyp << "_niter10" << "_cut" << iUfoldBin << "_DataType" << iUfoldDataType;
//    BIN4 << "hyp" << iUfoldHyp << "_niter3" << "_cut" << iUfoldBin << "_DataType" << iUfoldDataType;

    std::stringstream LT("");
    LT << "  |#eta|<" << etaGen ;
    LT << "  p_{T}>"  << ptGen ;

    std::stringstream ELT("");
    if ( iUfoldMCType == 10 )  ELT << "  Unf.=D6T" ;
    if ( iUfoldMCType == 11 )  ELT << "  Unf.=DW" ;
    if ( iUfoldMCType == 12 )  ELT << "  Unf.=P0" ;
    if ( iUfoldMCType == 13 )  ELT << "  Unf.=ProQ20" ;
    ELT << "  Hyp.="      << iUfoldHyp; 
    // ELT << "  N_{iter}=" << iUfoldNIter;  
    if (globalTraking==0) ELT << "  gTr - PT>" << ptReco;
    if (globalTraking==1) ELT << " mbTr - PT>" << ptReco;

    std::string datatype;
    if ( iUfoldDataType ==  0 ) datatype = "Data";
    if ( iUfoldDataType == 10 ) datatype = "D6T";
    if ( iUfoldDataType == 11 ) datatype = "DW";
    if ( iUfoldDataType == 12 ) datatype = "P0";
    if ( iUfoldDataType == 13 ) datatype = "ProQ20";

    std::stringstream LD1("");
    std::stringstream LD2("");
    std::stringstream LD3("");
    std::stringstream LD4("");
    LD1 << datatype << " - N_{iter}= 3";
    LD2 << datatype << " - N_{iter}= 5";
    LD3 << datatype << " - N_{iter}= 10";
//    LD4 << datatype << " - N_{iter}= 15";

    // Fig name

    std::stringstream figname; 
    figname << "nch_citr_3-5-10_" ;
    if (iUfoldMCType == 10 )  figname << "MC_D6T_" ;
    if (iUfoldMCType == 11 )  figname << "MC_DW_" ;
    if (iUfoldMCType == 12 )  figname << "MC_P0_" ;
    if (iUfoldMCType == 13 )  figname << "MC_ProQ20_" ;
    if (globalEnergy == 0.9  ) figname << "0.9TeV_";
    if (globalEnergy == 2.36 ) figname << "2.36TeV_";
    if (itracking == 1 )  figname << "gTr_" ;
    if (itracking == 2 )  figname << "mbTr_" ;
    globalFigBaseName = figname.str();



    plotReset();
    LegendTitle = LT.str();
    ExtLegTitle = ELT.str();
    // Plot settings
    globalNorm     = 2;
    globalFileType = 3;
    globalHistoType= 1;

    setnchrangeX(iUfoldBin);

    XaxisTitle = "n_{ch}";
    YaxisTitle = "#frac{1}{N} #frac{dN}{dn_{ch}}";

    dir   = "unfolding";
    
    ptcutstr = BIN1.str();
    dataSetId.push_back(-1);
    dataSetFile.push_back(fileManager(globalFileType,10,globalEnergy,globalTraking,0,0,ptcutstr,globalDirPlot));
    dataSetIsMc.push_back(0);
    dataSetStyle.push_back(20);
    dataSetColor.push_back(kRed);
    dataSetLegend.push_back(LD1.str());
    dataSetHisto.push_back("nch_data_corrected");

    ptcutstr = BIN2.str();
    dataSetId.push_back(-1);
    dataSetFile.push_back(fileManager(globalFileType,10,globalEnergy,globalTraking,0,0,ptcutstr,globalDirPlot));
    dataSetIsMc.push_back(0);
    dataSetStyle.push_back(20);
    dataSetColor.push_back(kBlue);
    dataSetLegend.push_back(LD2.str());
    dataSetHisto.push_back("nch_data_corrected");


    ptcutstr = BIN3.str();
    dataSetId.push_back(-1);
    dataSetFile.push_back(fileManager(globalFileType,10,globalEnergy,globalTraking,0,0,ptcutstr,globalDirPlot));
    dataSetIsMc.push_back(0);
    dataSetStyle.push_back(24);
    dataSetColor.push_back(kRed);
    dataSetLegend.push_back(LD3.str());
    dataSetHisto.push_back("nch_data_corrected");

/*
    ptcutstr = BIN4.str();
    dataSetId.push_back(-1);
    dataSetFile.push_back(fileManager(globalFileType,10,globalEnergy,globalTraking,0,0,ptcutstr,globalDirPlot));
    dataSetIsMc.push_back(0);
    dataSetStyle.push_back(24);
    dataSetColor.push_back(kBlue);
    dataSetLegend.push_back(LD4.str());
    dataSetHisto.push_back("nch_data_corrected");
*/

    ptcutstr = BIN2.str();
    dataSetId.push_back(-1);
    dataSetFile.push_back(fileManager(globalFileType,10,globalEnergy,0,0,0,ptcutstr,globalDirPlot));
    dataSetIsMc.push_back(1);
    dataSetStyle.push_back(1);
    dataSetColor.push_back(1);
    dataSetLegend.push_back("NSD - PYTHIA D6T");
    dataSetHisto.push_back("nch_MC_gen_afterEvtSelCorrection");
   
    plot(dir,"AUTO",1,2);
    
  }

//----------------------------------------- Nch Final plots --------------------------------------------



  //-------- Mch Unfold -------- Stack
  if( nchstak ){

    globalDirPlot = "../plots.romain/unfoldingv2/";
    cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
    cout << "globalDirPlot= " << globalDirPlot << endl;
    cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;

    // Retrieve bin info
    if ( iUfoldBin > (signed) accMap.size() ) {
      cout << "[makePlots] iUfoldBin > accMap.size()" << endl;
      return;
    }

    double ptGen1   = accMap[iUfoldBin+0].at(0);
    double etaGen1  = accMap[iUfoldBin+0].at(1);
    double ptReco1  = accMap[iUfoldBin+0].at(2);
    double etaReco1 = accMap[iUfoldBin+0].at(3);

    double ptGen2   = accMap[iUfoldBin+1].at(0);
    double etaGen2  = accMap[iUfoldBin+1].at(1);
    double ptReco2  = accMap[iUfoldBin+1].at(2);
    double etaReco2 = accMap[iUfoldBin+1].at(3);

    double ptGen3   = accMap[iUfoldBin+2].at(0);
    double etaGen3  = accMap[iUfoldBin+2].at(1);
    double ptReco3  = accMap[iUfoldBin+2].at(2);
    double etaReco3 = accMap[iUfoldBin+2].at(3);

    double ptGen4   = accMap[iUfoldBin+3].at(0);
    double etaGen4  = accMap[iUfoldBin+3].at(1);
    double ptReco4  = accMap[iUfoldBin+3].at(2);
    double etaReco4 = accMap[iUfoldBin+3].at(3);

    double ptGen5   = accMap[iUfoldBin+4].at(0);
    double etaGen5  = accMap[iUfoldBin+4].at(1);
    double ptReco5  = accMap[iUfoldBin+4].at(2);
    double etaReco5 = accMap[iUfoldBin+4].at(3);

    cout << ptGen1 << " " << etaGen1 << " " << ptReco1 << " " << etaReco1 << endl;
    cout << ptGen2 << " " << etaGen2 << " " << ptReco2 << " " << etaReco2 << endl;
    cout << ptGen3 << " " << etaGen3 << " " << ptReco3 << " " << etaReco3 << endl;
    cout << ptGen4 << " " << etaGen4 << " " << ptReco4 << " " << etaReco4 << endl;
    cout << ptGen5 << " " << etaGen5 << " " << ptReco5 << " " << etaReco5 << endl;
 
 
    std::stringstream BIN1 ("");
    std::stringstream BIN2 ("");
    std::stringstream BIN3 ("");
    std::stringstream BIN4 ("");
    std::stringstream BIN5 ("");

    BIN1 << "hyp" << iUfoldHyp << "_niter" << iUfoldNIter << "_cut" << (iUfoldBin+0) << "_DataType" << iUfoldDataType;
    BIN2 << "hyp" << iUfoldHyp << "_niter" << iUfoldNIter << "_cut" << (iUfoldBin+1) << "_DataType" << iUfoldDataType;
    BIN3 << "hyp" << iUfoldHyp << "_niter" << iUfoldNIter << "_cut" << (iUfoldBin+2) << "_DataType" << iUfoldDataType;
    BIN4 << "hyp" << iUfoldHyp << "_niter" << iUfoldNIter << "_cut" << (iUfoldBin+3) << "_DataType" << iUfoldDataType;
    BIN5 << "hyp" << iUfoldHyp << "_niter" << iUfoldNIter << "_cut" << (iUfoldBin+4) << "_DataType" << iUfoldDataType;

    std::stringstream LT("");
    if (itracking == 1 ) LT << "   genTracks: " ;
    if (itracking == 2 ) LT << "   mbiasTracks: " ;
    LT << "  p_{T}>"  << ptGen1 ;

    std::stringstream ELT("");
/*
    if ( iUfoldMCType == 10 )  ELT << "  Unf.=D6T" ;
    if ( iUfoldMCType == 11 )  ELT << "  Unf.=DW" ;
    if ( iUfoldMCType == 12 )  ELT << "  Unf.=P0" ;
    if ( iUfoldMCType == 13 )  ELT << "  Unf.=ProQ20" ;
    ELT << "  Hyp.="      << iUfoldHyp; 
    // ELT << "  N_{iter}=" << iUfoldNIter;  
    if (globalTraking==0) ELT << "  gTr - PT>" << ptReco;
    if (globalTraking==1) ELT << " mbTr - PT>" << ptReco;
*/

    std::string datatype;
    if ( iUfoldDataType ==  0 ) datatype = "Data";
    if ( iUfoldDataType == 10 ) datatype = "D6T";
    if ( iUfoldDataType == 11 ) datatype = "DW";
    if ( iUfoldDataType == 12 ) datatype = "P0";
    if ( iUfoldDataType == 13 ) datatype = "ProQ20";

    std::stringstream LD1("");
    std::stringstream LD2("");
    std::stringstream LD3("");
    std::stringstream LD4("");
    std::stringstream LD5("");
    LD1 << datatype << " - |#eta|<" << etaReco1 ;
    LD2 << datatype << " - |#eta|<" << etaReco2 ;
    LD3 << datatype << " - |#eta|<" << etaReco3 ;
    LD4 << datatype << " - |#eta|<" << etaReco4 ;
    LD5 << datatype << " - |#eta|<" << etaReco5 ;

    // Fig name

    std::stringstream figname; 
    figname << "nch_" ;
    if (iUfoldMCType == 10 )  figname << "MC_D6T_" ;
    if (iUfoldMCType == 11 )  figname << "MC_DW_" ;
    if (iUfoldMCType == 12 )  figname << "MC_P0_" ;
    if (iUfoldMCType == 13 )  figname << "MC_ProQ20_" ;
    if (globalEnergy == 0.9  ) figname << "0.9TeV_";
    if (globalEnergy == 2.36 ) figname << "2.36TeV_";
    if (itracking == 1 )  figname << "gTr_" ;
    if (itracking == 2 )  figname << "mbTr_" ;
    globalFigBaseName = figname.str();

    // Plot

    plotReset();
    LegendTitle = LT.str();
    ExtLegTitle = ELT.str();
    // Plot settings
    globalNorm     = 2;
    globalFileType = 3;
    globalHistoType= 1;

    XaxisTitle = "n_{ch}";
    YaxisTitle = "#frac{1}{N} #frac{dN}{dn_{ch}}";


    if ( globalEnergy == 0.9 ) 
    {
      if ( iUfoldBin == 0  || iUfoldBin == 5 )
        { histoYMin = 0.000001    ; histoYMax = 10000 ;
          histoXMin = -0.5        ; histoXMax = 90.5  ; }
      if ( iUfoldBin == 10 )
        { histoYMin = 0.0000001   ; histoYMax = 10000 ; 
          histoXMin = -0.5        ; histoXMax = 80.5  ; }
      if ( iUfoldBin == 15 )
        { histoYMin = 0.00000001  ; histoYMax = 10000 ; 
          histoXMin = -0.5        ; histoXMax = 50.5  ; }
      if ( iUfoldBin == 20 )
        { histoYMin = 0.000000001 ; histoYMax = 10000 ; 
          histoXMin = -0.5        ; histoXMax = 30.5  ; }
    }

    if ( globalEnergy == 2.36 )
    {
      if ( iUfoldBin == 0  || iUfoldBin == 5 )
        { histoYMin = 0.000001    ; histoYMax = 10000 ;
          histoXMin = -0.5        ; histoXMax = 110.5  ; }
      if ( iUfoldBin == 10 )
        { histoYMin = 0.0000001   ; histoYMax = 10000 ;
          histoXMin = -0.5        ; histoXMax = 100.5  ; }
      if ( iUfoldBin == 15 )
        { histoYMin = 0.00000001  ; histoYMax = 10000 ;
          histoXMin = -0.5        ; histoXMax =  80.5  ; }
      if ( iUfoldBin == 20 )
        { histoYMin = 0.000000001 ; histoYMax = 10000 ;
          histoXMin = -0.5        ; histoXMax =  50.5  ; }
    }


    dir   = "unfolding";
    
    ptcutstr = BIN1.str();
    dataSetId.push_back(-1);
    dataSetFile.push_back(fileManager(globalFileType,10,globalEnergy,globalTraking,0,0,ptcutstr,globalDirPlot));
    dataSetIsMc.push_back(0);
    dataSetStyle.push_back(20);
    dataSetColor.push_back(kBlack);
    dataSetLegend.push_back(LD1.str());
    dataSetHisto.push_back("nch_data_corrected");

    ptcutstr = BIN2.str();
    dataSetId.push_back(-1);
    dataSetFile.push_back(fileManager(globalFileType,10,globalEnergy,globalTraking,0,0,ptcutstr,globalDirPlot));
    dataSetIsMc.push_back(0);
    dataSetStyle.push_back(21);
    dataSetColor.push_back(kBlack);
    dataSetLegend.push_back(LD2.str());
    dataSetHisto.push_back("nch_data_corrected");


    ptcutstr = BIN3.str();
    dataSetId.push_back(-1);
    dataSetFile.push_back(fileManager(globalFileType,10,globalEnergy,globalTraking,0,0,ptcutstr,globalDirPlot));
    dataSetIsMc.push_back(0);
    dataSetStyle.push_back(22);
    dataSetColor.push_back(kBlack);
    dataSetLegend.push_back(LD3.str());
    dataSetHisto.push_back("nch_data_corrected");

    ptcutstr = BIN4.str();
    dataSetId.push_back(-1);
    dataSetFile.push_back(fileManager(globalFileType,10,globalEnergy,globalTraking,0,0,ptcutstr,globalDirPlot));
    dataSetIsMc.push_back(0);
    dataSetStyle.push_back(23);
    dataSetColor.push_back(kBlack);
    dataSetLegend.push_back(LD4.str());
    dataSetHisto.push_back("nch_data_corrected");

    ptcutstr = BIN5.str();
    dataSetId.push_back(-1);
    dataSetFile.push_back(fileManager(globalFileType,10,globalEnergy,globalTraking,0,0,ptcutstr,globalDirPlot));
    dataSetIsMc.push_back(0);
    dataSetStyle.push_back(24);
    dataSetColor.push_back(kBlack);
    dataSetLegend.push_back(LD5.str());
    dataSetHisto.push_back("nch_data_corrected");

    if (globalEnergy == 0.9 && ( iUfoldBin == 0 || iUfoldBin == 5 ) ) {
      dataSetId.push_back(-1);
      dataSetFile.push_back("../expdata/ua5_dsigdn_eta15");
      dataSetIsMc.push_back(0);
      dataSetStyle.push_back(24);
      dataSetColor.push_back(kRed);
      dataSetLegend.push_back("UA5 - |#eta|<1.5");
      dataSetHisto.push_back("UA5");
    }

    ptcutstr = BIN1.str();
    dataSetId.push_back(-1);
    dataSetFile.push_back(fileManager(globalFileType,10,globalEnergy,0,0,0,ptcutstr,globalDirPlot));
    dataSetIsMc.push_back(1);
    dataSetStyle.push_back(1);
    dataSetColor.push_back(1);
    dataSetLegend.push_back("PYTHIA D6T");
    dataSetHisto.push_back("nch_MC_gen_afterEvtSelCorrection");

    ptcutstr = BIN2.str();
    dataSetId.push_back(-1);
    dataSetFile.push_back(fileManager(globalFileType,10,globalEnergy,0,0,0,ptcutstr,globalDirPlot));
    dataSetIsMc.push_back(1);
    dataSetStyle.push_back(1);
    dataSetColor.push_back(1);
    dataSetLegend.push_back("NONE");
    dataSetHisto.push_back("nch_MC_gen_afterEvtSelCorrection");

    ptcutstr = BIN3.str();
    dataSetId.push_back(-1);
    dataSetFile.push_back(fileManager(globalFileType,10,globalEnergy,0,0,0,ptcutstr,globalDirPlot));
    dataSetIsMc.push_back(1);
    dataSetStyle.push_back(1);
    dataSetColor.push_back(1);
    dataSetLegend.push_back("NONE");
    dataSetHisto.push_back("nch_MC_gen_afterEvtSelCorrection");

    ptcutstr = BIN4.str();
    dataSetId.push_back(-1);
    dataSetFile.push_back(fileManager(globalFileType,10,globalEnergy,0,0,0,ptcutstr,globalDirPlot));
    dataSetIsMc.push_back(1);
    dataSetStyle.push_back(1);
    dataSetColor.push_back(1);
    dataSetLegend.push_back("NONE");
    dataSetHisto.push_back("nch_MC_gen_afterEvtSelCorrection");

    ptcutstr = BIN5.str();
    dataSetId.push_back(-1);
    dataSetFile.push_back(fileManager(globalFileType,10,globalEnergy,0,0,0,ptcutstr,globalDirPlot));
    dataSetIsMc.push_back(1);
    dataSetStyle.push_back(1);
    dataSetColor.push_back(1);
    dataSetLegend.push_back("NONE");
    dataSetHisto.push_back("nch_MC_gen_afterEvtSelCorrection");


    // Data scaling Factor
    dataSetFactor.push_back(10000);
    dataSetFactor.push_back(1000);
    dataSetFactor.push_back(100);
    dataSetFactor.push_back(10);
    dataSetFactor.push_back(1);
    if (globalEnergy == 0.9 && ( iUfoldBin == 0 || iUfoldBin == 5 ) ) { dataSetFactor.push_back(100); }   

    // MC  scaling Factor
    dataSetFactor.push_back(10000);
    dataSetFactor.push_back(1000);
    dataSetFactor.push_back(100);
    dataSetFactor.push_back(10);
    dataSetFactor.push_back(1);


    plot(dir,"AUTO",1,0);
    
  }


  //-------- Mch Unfold -------- Stack
  if( knostak ){

    globalDirPlot = "../plots.romain/unfoldingv2/";
    cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
    cout << "globalDirPlot= " << globalDirPlot << endl;
    cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;

    // Retrieve bin info
    if ( iUfoldBin > (signed) accMap.size() ) {
      cout << "[makePlots] iUfoldBin > accMap.size()" << endl;
      return;
    }

    double ptGen1   = accMap[iUfoldBin+0].at(0);
    double etaGen1  = accMap[iUfoldBin+0].at(1);
    double ptReco1  = accMap[iUfoldBin+0].at(2);
    double etaReco1 = accMap[iUfoldBin+0].at(3);

    double ptGen2   = accMap[iUfoldBin+1].at(0);
    double etaGen2  = accMap[iUfoldBin+1].at(1);
    double ptReco2  = accMap[iUfoldBin+1].at(2);
    double etaReco2 = accMap[iUfoldBin+1].at(3);

    double ptGen3   = accMap[iUfoldBin+2].at(0);
    double etaGen3  = accMap[iUfoldBin+2].at(1);
    double ptReco3  = accMap[iUfoldBin+2].at(2);
    double etaReco3 = accMap[iUfoldBin+2].at(3);

    double ptGen4   = accMap[iUfoldBin+3].at(0);
    double etaGen4  = accMap[iUfoldBin+3].at(1);
    double ptReco4  = accMap[iUfoldBin+3].at(2);
    double etaReco4 = accMap[iUfoldBin+3].at(3);

    double ptGen5   = accMap[iUfoldBin+4].at(0);
    double etaGen5  = accMap[iUfoldBin+4].at(1);
    double ptReco5  = accMap[iUfoldBin+4].at(2);
    double etaReco5 = accMap[iUfoldBin+4].at(3);

    cout << ptGen1 << " " << etaGen1 << " " << ptReco1 << " " << etaReco1 << endl;
    cout << ptGen2 << " " << etaGen2 << " " << ptReco2 << " " << etaReco2 << endl;
    cout << ptGen3 << " " << etaGen3 << " " << ptReco3 << " " << etaReco3 << endl;
    cout << ptGen4 << " " << etaGen4 << " " << ptReco4 << " " << etaReco4 << endl;
    cout << ptGen5 << " " << etaGen5 << " " << ptReco5 << " " << etaReco5 << endl;
  
    std::stringstream BIN1 ("");
    std::stringstream BIN2 ("");
    std::stringstream BIN3 ("");
    std::stringstream BIN4 ("");
    std::stringstream BIN5 ("");

    BIN1 << "hyp" << iUfoldHyp << "_niter" << iUfoldNIter << "_cut" << (iUfoldBin+0) << "_DataType" << iUfoldDataType;
    BIN2 << "hyp" << iUfoldHyp << "_niter" << iUfoldNIter << "_cut" << (iUfoldBin+1) << "_DataType" << iUfoldDataType;
    BIN3 << "hyp" << iUfoldHyp << "_niter" << iUfoldNIter << "_cut" << (iUfoldBin+2) << "_DataType" << iUfoldDataType;
    BIN4 << "hyp" << iUfoldHyp << "_niter" << iUfoldNIter << "_cut" << (iUfoldBin+3) << "_DataType" << iUfoldDataType;
    BIN5 << "hyp" << iUfoldHyp << "_niter" << iUfoldNIter << "_cut" << (iUfoldBin+4) << "_DataType" << iUfoldDataType;

    std::stringstream LT("");
    if (itracking == 1 ) LT << "   genTracks: " ;
    if (itracking == 2 ) LT << "   mbiasTracks: " ;
    LT << "  p_{T}>"  << ptGen1 ;

    std::stringstream ELT("");
/*
    if ( iUfoldMCType == 10 )  ELT << "  Unf.=D6T" ;
    if ( iUfoldMCType == 11 )  ELT << "  Unf.=DW" ;
    if ( iUfoldMCType == 12 )  ELT << "  Unf.=P0" ;
    if ( iUfoldMCType == 13 )  ELT << "  Unf.=ProQ20" ;
    ELT << "  Hyp.="      << iUfoldHyp; 
    // ELT << "  N_{iter}=" << iUfoldNIter;  
    if (globalTraking==0) ELT << "  gTr - PT>" << ptReco;
    if (globalTraking==1) ELT << " mbTr - PT>" << ptReco;
*/

    std::string datatype;
    if ( iUfoldDataType ==  0 ) datatype = "Data";
    if ( iUfoldDataType == 10 ) datatype = "D6T";
    if ( iUfoldDataType == 11 ) datatype = "DW";
    if ( iUfoldDataType == 12 ) datatype = "P0";
    if ( iUfoldDataType == 13 ) datatype = "ProQ20";

    std::stringstream LD1("");
    std::stringstream LD2("");
    std::stringstream LD3("");
    std::stringstream LD4("");
    std::stringstream LD5("");
    LD1 << datatype << " - |#eta|<" << etaReco1 ;
    LD2 << datatype << " - |#eta|<" << etaReco2 ;
    LD3 << datatype << " - |#eta|<" << etaReco3 ;
    LD4 << datatype << " - |#eta|<" << etaReco4 ;
    LD5 << datatype << " - |#eta|<" << etaReco5 ;

    // Fig name

    std::stringstream figname; 
    figname << "kno_" ;
    if (iUfoldMCType == 10 )  figname << "MC_D6T_" ;
    if (iUfoldMCType == 11 )  figname << "MC_DW_" ;
    if (iUfoldMCType == 12 )  figname << "MC_P0_" ;
    if (iUfoldMCType == 13 )  figname << "MC_ProQ20_" ;
    if (globalEnergy == 0.9  ) figname << "0.9TeV_";
    if (globalEnergy == 2.36 ) figname << "2.36TeV_";
    if (itracking == 1 )  figname << "gTr_" ;
    if (itracking == 2 )  figname << "mbTr_" ;
    globalFigBaseName = figname.str();

    // Plot

    plotReset();
    LegendTitle = LT.str();
    ExtLegTitle = ELT.str();
    // Plot settings
    globalNorm     = 2;
    globalFileType = 3;
    globalHistoType= 1;

      if ( iUfoldBin == 0  || iUfoldBin == 5 )
        { histoYMin = 0.000001    ; histoYMax = 10000 ; }
      if ( iUfoldBin == 10 )
        { histoYMin = 0.0000001   ; histoYMax = 10000 ; }
      if ( iUfoldBin == 15 )
        { histoYMin = 0.00000001  ; histoYMax = 10000 ; }
      if ( iUfoldBin == 20 )
        { histoYMin = 0.000000001 ; histoYMax = 10000 ; }

    dir   = "unfolding";
    
    ptcutstr = BIN1.str();
    dataSetId.push_back(-1);
    dataSetFile.push_back(fileManager(globalFileType,10,globalEnergy,globalTraking,0,0,ptcutstr,globalDirPlot));
    dataSetIsMc.push_back(0);
    dataSetStyle.push_back(20);
    dataSetColor.push_back(kBlack);
    dataSetLegend.push_back(LD1.str());
    dataSetHisto.push_back("kno_corrected");

    ptcutstr = BIN2.str();
    dataSetId.push_back(-1);
    dataSetFile.push_back(fileManager(globalFileType,10,globalEnergy,globalTraking,0,0,ptcutstr,globalDirPlot));
    dataSetIsMc.push_back(0);
    dataSetStyle.push_back(21);
    dataSetColor.push_back(kBlack);
    dataSetLegend.push_back(LD2.str());
    dataSetHisto.push_back("kno_corrected");


    ptcutstr = BIN3.str();
    dataSetId.push_back(-1);
    dataSetFile.push_back(fileManager(globalFileType,10,globalEnergy,globalTraking,0,0,ptcutstr,globalDirPlot));
    dataSetIsMc.push_back(0);
    dataSetStyle.push_back(22);
    dataSetColor.push_back(kBlack);
    dataSetLegend.push_back(LD3.str());
    dataSetHisto.push_back("kno_corrected");

    ptcutstr = BIN4.str();
    dataSetId.push_back(-1);
    dataSetFile.push_back(fileManager(globalFileType,10,globalEnergy,globalTraking,0,0,ptcutstr,globalDirPlot));
    dataSetIsMc.push_back(0);
    dataSetStyle.push_back(23);
    dataSetColor.push_back(kBlack);
    dataSetLegend.push_back(LD4.str());
    dataSetHisto.push_back("kno_corrected");

    ptcutstr = BIN5.str();
    dataSetId.push_back(-1);
    dataSetFile.push_back(fileManager(globalFileType,10,globalEnergy,globalTraking,0,0,ptcutstr,globalDirPlot));
    dataSetIsMc.push_back(0);
    dataSetStyle.push_back(24);
    dataSetColor.push_back(kBlack);
    dataSetLegend.push_back(LD5.str());
    dataSetHisto.push_back("kno_corrected");


    ptcutstr = BIN1.str();
    dataSetId.push_back(-1);
    dataSetFile.push_back(fileManager(globalFileType,10,globalEnergy,0,0,0,ptcutstr,globalDirPlot));
    dataSetIsMc.push_back(1);
    dataSetStyle.push_back(1);
    dataSetColor.push_back(1);
    dataSetLegend.push_back("PYTHIA D6T");
    dataSetHisto.push_back("kno_gen");

    ptcutstr = BIN2.str();
    dataSetId.push_back(-1);
    dataSetFile.push_back(fileManager(globalFileType,10,globalEnergy,0,0,0,ptcutstr,globalDirPlot));
    dataSetIsMc.push_back(1);
    dataSetStyle.push_back(1);
    dataSetColor.push_back(1);
    dataSetLegend.push_back("NONE");
    dataSetHisto.push_back("kno_gen");

    ptcutstr = BIN3.str();
    dataSetId.push_back(-1);
    dataSetFile.push_back(fileManager(globalFileType,10,globalEnergy,0,0,0,ptcutstr,globalDirPlot));
    dataSetIsMc.push_back(1);
    dataSetStyle.push_back(1);
    dataSetColor.push_back(1);
    dataSetLegend.push_back("NONE");
    dataSetHisto.push_back("kno_gen");

    ptcutstr = BIN4.str();
    dataSetId.push_back(-1);
    dataSetFile.push_back(fileManager(globalFileType,10,globalEnergy,0,0,0,ptcutstr,globalDirPlot));
    dataSetIsMc.push_back(1);
    dataSetStyle.push_back(1);
    dataSetColor.push_back(1);
    dataSetLegend.push_back("NONE");
    dataSetHisto.push_back("kno_gen");

    ptcutstr = BIN5.str();
    dataSetId.push_back(-1);
    dataSetFile.push_back(fileManager(globalFileType,10,globalEnergy,0,0,0,ptcutstr,globalDirPlot));
    dataSetIsMc.push_back(1);
    dataSetStyle.push_back(1);
    dataSetColor.push_back(1);
    dataSetLegend.push_back("NONE");
    dataSetHisto.push_back("kno_gen");


    // Data scaling Factor
    dataSetFactor.push_back(10000);
    dataSetFactor.push_back(1000);
    dataSetFactor.push_back(100);
    dataSetFactor.push_back(10);
    dataSetFactor.push_back(1);
   
    // MC  scaling Factor
    dataSetFactor.push_back(10000);
    dataSetFactor.push_back(1000);
    dataSetFactor.push_back(100);
    dataSetFactor.push_back(10);
    dataSetFactor.push_back(1);


    plot(dir,"AUTO",1,2);
    
  }


  bool compenergy = false;
  if( nch2ene || kno2ene ) compenergy = true;

  //-------- Mch Unfold -------- Stack - 2 energies
  if( compenergy ){

    TString hdata , hmoca; 
    
    if (nch2ene ) {
      hdata = "nch_data_corrected";
      hmoca = "nch_MC_gen_afterEvtSelCorrection";
    }

    if (kno2ene ) {
      hdata = "kno_corrected";
      hmoca = "kno_gen";
    }

    globalDirPlot = "../plots.romain/unfoldingv2/";
    cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
    cout << "globalDirPlot= " << globalDirPlot << endl;
    cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;

    // Retrieve bin info
    if ( iUfoldBin > (signed) accMap.size() ) {
      cout << "[makePlots] iUfoldBin > accMap.size()" << endl;
      return;
    }

    double ptGen1   = accMap[iUfoldBin+0].at(0);
    double etaGen1  = accMap[iUfoldBin+0].at(1);
    double ptReco1  = accMap[iUfoldBin+0].at(2);
    double etaReco1 = accMap[iUfoldBin+0].at(3);

    double ptGen2   = accMap[iUfoldBin+1].at(0);
    double etaGen2  = accMap[iUfoldBin+1].at(1);
    double ptReco2  = accMap[iUfoldBin+1].at(2);
    double etaReco2 = accMap[iUfoldBin+1].at(3);

    double ptGen3   = accMap[iUfoldBin+2].at(0);
    double etaGen3  = accMap[iUfoldBin+2].at(1);
    double ptReco3  = accMap[iUfoldBin+2].at(2);
    double etaReco3 = accMap[iUfoldBin+2].at(3);

    double ptGen4   = accMap[iUfoldBin+3].at(0);
    double etaGen4  = accMap[iUfoldBin+3].at(1);
    double ptReco4  = accMap[iUfoldBin+3].at(2);
    double etaReco4 = accMap[iUfoldBin+3].at(3);

    double ptGen5   = accMap[iUfoldBin+4].at(0);
    double etaGen5  = accMap[iUfoldBin+4].at(1);
    double ptReco5  = accMap[iUfoldBin+4].at(2);
    double etaReco5 = accMap[iUfoldBin+4].at(3);

    cout << ptGen1 << " " << etaGen1 << " " << ptReco1 << " " << etaReco1 << endl;
    cout << ptGen2 << " " << etaGen2 << " " << ptReco2 << " " << etaReco2 << endl;
    cout << ptGen3 << " " << etaGen3 << " " << ptReco3 << " " << etaReco3 << endl;
    cout << ptGen4 << " " << etaGen4 << " " << ptReco4 << " " << etaReco4 << endl;
    cout << ptGen5 << " " << etaGen5 << " " << ptReco5 << " " << etaReco5 << endl;
  
    std::stringstream BIN1 ("");
    std::stringstream BIN2 ("");
    std::stringstream BIN3 ("");
    std::stringstream BIN4 ("");
    std::stringstream BIN5 ("");

    BIN1 << "hyp" << iUfoldHyp << "_niter" << iUfoldNIter << "_cut" << (iUfoldBin+0) << "_DataType" << iUfoldDataType;
    BIN2 << "hyp" << iUfoldHyp << "_niter" << iUfoldNIter << "_cut" << (iUfoldBin+1) << "_DataType" << iUfoldDataType;
    BIN3 << "hyp" << iUfoldHyp << "_niter" << iUfoldNIter << "_cut" << (iUfoldBin+2) << "_DataType" << iUfoldDataType;
    BIN4 << "hyp" << iUfoldHyp << "_niter" << iUfoldNIter << "_cut" << (iUfoldBin+3) << "_DataType" << iUfoldDataType;
    BIN5 << "hyp" << iUfoldHyp << "_niter" << iUfoldNIter << "_cut" << (iUfoldBin+4) << "_DataType" << iUfoldDataType;

    std::stringstream LT("");
    if (itracking == 1 ) LT << "   genTracks: " ;
    if (itracking == 2 ) LT << "   mbiasTracks: " ;
    LT << "  p_{T}>"  << ptGen1 ;

    std::stringstream ELT("");
/*
    if ( iUfoldMCType == 10 )  ELT << "  Unf.=D6T" ;
    if ( iUfoldMCType == 11 )  ELT << "  Unf.=DW" ;
    if ( iUfoldMCType == 12 )  ELT << "  Unf.=P0" ;
    if ( iUfoldMCType == 13 )  ELT << "  Unf.=ProQ20" ;
    ELT << "  Hyp.="      << iUfoldHyp; 
    // ELT << "  N_{iter}=" << iUfoldNIter;  
    if (globalTraking==0) ELT << "  gTr - PT>" << ptReco;
    if (globalTraking==1) ELT << " mbTr - PT>" << ptReco;
*/

    std::string datatype;
    if ( iUfoldDataType ==  0 ) datatype = "Data";
    if ( iUfoldDataType == 10 ) datatype = "D6T";
    if ( iUfoldDataType == 11 ) datatype = "DW";
    if ( iUfoldDataType == 12 ) datatype = "P0";
    if ( iUfoldDataType == 13 ) datatype = "ProQ20";

    std::stringstream LD1("");
    std::stringstream LD2("");
    std::stringstream LD3("");
    std::stringstream LD4("");
    std::stringstream LD5("");
    LD1 << datatype << " - |#eta|<" << etaReco1 ;
    LD2 << datatype << " - |#eta|<" << etaReco2 ;
    LD3 << datatype << " - |#eta|<" << etaReco3 ;
    LD4 << datatype << " - |#eta|<" << etaReco4 ;
    LD5 << datatype << " - |#eta|<" << etaReco5 ;

    // Fig name

    std::stringstream figname; 
    if (nch2ene ) figname << "nch_" ; 
    if (kno2ene ) figname << "kno_" ;
    if (iUfoldMCType == 10 )  figname << "MC_D6T_" ;
    if (iUfoldMCType == 11 )  figname << "MC_DW_" ;
    if (iUfoldMCType == 12 )  figname << "MC_P0_" ;
    if (iUfoldMCType == 13 )  figname << "MC_ProQ20_" ;
    figname << "0.9+2.36TeV_";
    if (itracking == 1 )  figname << "gTr_" ;
    if (itracking == 2 )  figname << "mbTr_" ;
    globalFigBaseName = figname.str();

    // Define plot
    plotReset();
    globalLabel = "CMS";

    LegendTitle = LT.str();
    ExtLegTitle = ELT.str();
    // Plot settings
    globalNorm     = 3;
    globalFileType = 3;
    globalHistoType= 1;

    if (nch2ene) {
      XaxisTitle = "n_{ch}";
      YaxisTitle = "#frac{1}{N} #frac{dN}{dn_{ch}}";
    }

      if ( iUfoldBin == 0  || iUfoldBin == 5 )
        { histoYMin = 0.000001    ; histoYMax = 10000 ;
          histoXMin = -0.5        ; histoXMax = 110.5  ; }
      if ( iUfoldBin == 10 )
        { histoYMin = 0.0000001   ; histoYMax = 10000 ;
          histoXMin = -0.5        ; histoXMax = 100.5  ; }
      if ( iUfoldBin == 15 )
        { histoYMin = 0.00000001  ; histoYMax = 10000 ;
          histoXMin = -0.5        ; histoXMax =  80.5  ; }
      if ( iUfoldBin == 20 )
        { histoYMin = 0.000000001 ; histoYMax = 10000 ;
          histoXMin = -0.5        ; histoXMax =  50.5  ; }

    dir   = "unfolding";
      
    int kColor = kBlack ;

    for ( int iE = 1 ; iE < 3 ; ++iE ) {

      std::stringstream LD1("");
      std::stringstream LD2("");
      std::stringstream LD3("");
      std::stringstream LD4("");
      std::stringstream LD5("");
 
      if ( iE==1 ) {
        globalEnergy = 0.9; 
        kColor = kBlack ;
        LD1 << datatype << " 0.9 TeV - |#eta|<" << etaReco1 ;
        LD2 << datatype << " 0.9 TeV - |#eta|<" << etaReco2 ;
        LD3 << datatype << " 0.9 TeV - |#eta|<" << etaReco3 ;
        LD4 << datatype << " 0.9 TeV - |#eta|<" << etaReco4 ;
        LD5 << datatype << " 0.9 TeV - |#eta|<" << etaReco5 ;
      }

      if ( iE==2 ) {
        globalEnergy = 2.36; 
        kColor = kBlue ;
        LD1 << datatype << " 2.36 TeV - |#eta|<" << etaReco1 ;
        LD2 << datatype << " 2.36 TeV - |#eta|<" << etaReco2 ;
        LD3 << datatype << " 2.36 TeV - |#eta|<" << etaReco3 ;
        LD4 << datatype << " 2.36 TeV - |#eta|<" << etaReco4 ;
        LD5 << datatype << " 2.36 TeV - |#eta|<" << etaReco5 ;
      }

      ptcutstr = BIN1.str();
      dataSetId.push_back(-1);
      dataSetFile.push_back(fileManager(globalFileType,10,globalEnergy,globalTraking,0,0,ptcutstr,globalDirPlot));
      dataSetIsMc.push_back(0);
      dataSetStyle.push_back(20);
      dataSetColor.push_back(kColor);
      dataSetLegend.push_back(LD1.str());
      dataSetHisto.push_back(hdata);
  
      ptcutstr = BIN2.str();
      dataSetId.push_back(-1);
      dataSetFile.push_back(fileManager(globalFileType,10,globalEnergy,globalTraking,0,0,ptcutstr,globalDirPlot));
      dataSetIsMc.push_back(0);
      dataSetStyle.push_back(21);
      dataSetColor.push_back(kColor);
      dataSetLegend.push_back(LD2.str());
      dataSetHisto.push_back(hdata);
  
  
      ptcutstr = BIN3.str();
      dataSetId.push_back(-1);
      dataSetFile.push_back(fileManager(globalFileType,10,globalEnergy,globalTraking,0,0,ptcutstr,globalDirPlot));
      dataSetIsMc.push_back(0);
      dataSetStyle.push_back(22);
      dataSetColor.push_back(kColor);
      dataSetLegend.push_back(LD3.str());
      dataSetHisto.push_back(hdata);
  
      ptcutstr = BIN4.str();
      dataSetId.push_back(-1);
      dataSetFile.push_back(fileManager(globalFileType,10,globalEnergy,globalTraking,0,0,ptcutstr,globalDirPlot));
      dataSetIsMc.push_back(0);
      dataSetStyle.push_back(23);
      dataSetColor.push_back(kColor);
      dataSetLegend.push_back(LD4.str());
      dataSetHisto.push_back(hdata);
  
      ptcutstr = BIN5.str();
      dataSetId.push_back(-1);
      dataSetFile.push_back(fileManager(globalFileType,10,globalEnergy,globalTraking,0,0,ptcutstr,globalDirPlot));
      dataSetIsMc.push_back(0);
      dataSetStyle.push_back(24);
      dataSetColor.push_back(kColor);
      dataSetLegend.push_back(LD5.str());
      dataSetHisto.push_back(hdata);
  
  /*
      ptcutstr = BIN1.str();
      dataSetId.push_back(-1);
      dataSetFile.push_back(fileManager(globalFileType,10,globalEnergy,0,0,0,ptcutstr,globalDirPlot));
      dataSetIsMc.push_back(1);
      dataSetStyle.push_back(1);
      dataSetColor.push_back(kColor);
      if (iE == 1 ) dataSetLegend.push_back("PYTHIA D6T 0.9 TeV");
      if (iE == 2 ) dataSetLegend.push_back("PYTHIA D6T 2.36 TeV");
      dataSetHisto.push_back(hmoca);
  
      ptcutstr = BIN2.str();
      dataSetId.push_back(-1);
      dataSetFile.push_back(fileManager(globalFileType,10,globalEnergy,0,0,0,ptcutstr,globalDirPlot));
      dataSetIsMc.push_back(1);
      dataSetStyle.push_back(1);
      dataSetColor.push_back(kColor);
      dataSetLegend.push_back("NONE");
      dataSetHisto.push_back(hmoca);
  
      ptcutstr = BIN3.str();
      dataSetId.push_back(-1);
      dataSetFile.push_back(fileManager(globalFileType,10,globalEnergy,0,0,0,ptcutstr,globalDirPlot));
      dataSetIsMc.push_back(1);
      dataSetStyle.push_back(1);
      dataSetColor.push_back(kColor);
      dataSetLegend.push_back("NONE");
      dataSetHisto.push_back(hmoca);
  
      ptcutstr = BIN4.str();
      dataSetId.push_back(-1);
      dataSetFile.push_back(fileManager(globalFileType,10,globalEnergy,0,0,0,ptcutstr,globalDirPlot));
      dataSetIsMc.push_back(1);
      dataSetStyle.push_back(1);
      dataSetColor.push_back(kColor);
      dataSetLegend.push_back("NONE");
      dataSetHisto.push_back(hmoca);
  
      ptcutstr = BIN5.str();
      dataSetId.push_back(-1);
      dataSetFile.push_back(fileManager(globalFileType,10,globalEnergy,0,0,0,ptcutstr,globalDirPlot));
      dataSetIsMc.push_back(1);
      dataSetStyle.push_back(1);
      dataSetColor.push_back(kColor);
      dataSetLegend.push_back("NONE");
      dataSetHisto.push_back(hmoca);
  */
  
      // Data scaling Factor
      dataSetFactor.push_back(10000);
      dataSetFactor.push_back(1000);
      dataSetFactor.push_back(100);
      dataSetFactor.push_back(10);
      dataSetFactor.push_back(1);
    /* 
      // MC  scaling Factor
      dataSetFactor.push_back(10000);
      dataSetFactor.push_back(1000);
      dataSetFactor.push_back(100);
      dataSetFactor.push_back(10);
      dataSetFactor.push_back(1);
   */

    } // end for (iE)

    plot(dir,"AUTO",1,2);
    
  }




}


void makeUPlots ( int iplot            = 1   ,
                  int itracking        = 0   , 
                  double energy        = 0.9 , 
                  int iUfoldBin        = 0   , 
                  int iUfoldDataTypein = 0   , 
                  int iUfoldMCTypein   = 10  , 
                  int iUfoldHypin      = 1   , 
                  int iUfoldNIterin    = 5   )
{
  iUfoldDataType = iUfoldDataTypein ; iUfoldHyp = iUfoldHypin ; iUfoldNIter = iUfoldNIterin;  iUfoldMCType =  iUfoldMCTypein ;

 trkplot = 0;
 vqlplot = 0;
 vtxplot = 0;
 nvtplot = 0;
 tvtplot = 0;
 mltplot = 0;

// eta / pt / pt2
 corplot = 0;

// Nch Unfolding
 effplot = 0;
 mtxplot = 0;

//-------------------

 nchplot = 0;
 nchctrk = 0;
 nchcbin = 0;
 nchcunf = 0;
 nchchyp = 0;
 nchcitr = 0;
 nchcene = 0;

 nchstak = 0;
 knostak = 0;
 nch2ene = 0;
 kno2ene = 0;

 if      ( iplot == 1 ) nchplot = 1;
 else if ( iplot == 2 ) nchctrk = 1;
 else if ( iplot == 3 ) nchcbin = 1; // Only for 0,1,2,3,4 bins !!!!
 else if ( iplot == 4 ) nchcunf = 1;
 else if ( iplot == 5 ) nchchyp = 1;
 else if ( iplot == 6 ) nchcitr = 1;
 else if ( iplot == 7 ) nchcene = 1;

 else if ( iplot == 10) nchstak = 1;  
 else if ( iplot == 11) knostak = 1;  
 else if ( iplot == 12) nch2ene = 1; 
 else if ( iplot == 13) kno2ene = 1; 

 makePlots ( itracking  , energy , 0 , 0 , iUfoldBin ); 

}

void setnchrangeX(int iUfoldBin )
{
    if ( globalEnergy == 0.9 ) 
    {
      if ( iUfoldBin == 0  || iUfoldBin == 5 )
        { 
          histoXMin = -0.5        ; histoXMax = 90.5  ; }
      if ( iUfoldBin == 10 )
        { 
          histoXMin = -0.5        ; histoXMax = 80.5  ; }
      if ( iUfoldBin == 15 )
        { 
          histoXMin = -0.5        ; histoXMax = 50.5  ; }
      if ( iUfoldBin == 20 )
        { 
          histoXMin = -0.5        ; histoXMax = 30.5  ; }
    }

    if ( globalEnergy == 2.36 )
    {
      if ( iUfoldBin == 0  || iUfoldBin == 5 )
        { 
          histoXMin = -0.5        ; histoXMax = 110.5  ; }
      if ( iUfoldBin == 10 )
        { 
          histoXMin = -0.5        ; histoXMax = 100.5  ; }
      if ( iUfoldBin == 15 )
        { 
          histoXMin = -0.5        ; histoXMax =  80.5  ; }
      if ( iUfoldBin == 20 )
        { 
          histoXMin = -0.5        ; histoXMax =  50.5  ; }
    }
}
