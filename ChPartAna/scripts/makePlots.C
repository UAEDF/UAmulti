//
// makePlots: Do all control plots
// ---------
//
// itracking = 1 : general Tracks and offline Vtx
//             2 : minbias Tracks and ferenc  Vtx

#include <TROOT.h>
#include <TH1F.h>
#include <TH2F.h>
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
bool mltplot = 0;

// eta / pt / pt2
bool corplot = 1;

// Nch Unfolding
bool effplot = 0;
bool mtxplot = 0;



bool nchplot = 0;

// ------------------------ trackPlot

void trackPlots()
{
  trkplot = 1;
  vqlplot = 0;
  vtxplot = 1;
  mltplot = 0;

  effplot = 0;
  mtxplot = 0;

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
void makePlots (int itracking = 1 , double energy = 0.9 , double ptcut = 0.4 , double etacut = 2.5 ){

  gROOT->ProcessLine(".x cmsStyleRoot.C");

  TString sel,tr,all,dir,cut;

  plotReset();
  globalTraking = itracking -1; 
  globalEnergy  = energy;

  if ( globalEnergy == 0.9 ) globalLabel =  "CMS 0.9 TeV";
  if ( globalEnergy == 2.36) globalLabel =  "CMS 2.36 TeV";  

  //------ TRACK & VTX PLOTS CONFIG ---------
  if ( trkplot || vqlplot || vtxplot || mltplot ) { 

    globalNorm = 2; 
  
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

    } else if  ( itracking == 2 ) {
      tr  = "allTr_mbTr_fVtx";
      all = sel+"_"+tr;
      dir = "EvtSelPlots_"+tr   +   "/TrackPlots_"+sel+"_"+tr;
      cout<<dir<<endl;

      plot(dir,"dzOsz_"+all);
      plot(dir,"dzOsz_"+all,1);
      plot(dir,"dxybsOsxy_"+all);
      plot(dir,"dxybsOsxy_"+all,1);

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

    
    // Plot settings
    globalNorm     = 0;
    globalFileType = 2;

    dataSetId.clear();
    dataSetIsMc.clear();
    dataSetStyle.clear();
    dataSetColor.clear();
    dataSetLegend.clear();
    dataSetDir.clear();
    dataSetHisto.clear();

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

    plotReset();

    // Plot settings
    globalNorm     = 0;
    globalFileType = 2;
    globalHistoType= 2;

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

  }


  //-------- ETA/PT/PT2 -------- Correction history
  if( corplot ){

    plotReset();

    // Plot settings
    globalNorm     = 0;
    globalFileType = 3;
    globalHistoType= 1;

    TString bin ("5");

    dataSetId.push_back(-1);
    ptcutstr = "hyp1_niter10_cut" + bin + "_isData1";
    dataSetFile.push_back(fileManager(globalFileType,10,globalEnergy,globalTraking,0,0,ptcutstr));
    dataSetIsMc.push_back(0);
    dataSetStyle.push_back(20);
    dataSetColor.push_back(2);
    dataSetLegend.push_back("Data Uncor.");
    dataSetHisto.push_back("eta_data_toCorrect"); 

    dataSetId.push_back(-1);
    ptcutstr = "hyp1_niter10_cut" + bin + "_isData1";
    dataSetFile.push_back(fileManager(globalFileType,10,globalEnergy,globalTraking,0,0,ptcutstr));
    dataSetIsMc.push_back(0);
    dataSetStyle.push_back(20);
    dataSetColor.push_back(4);
    dataSetLegend.push_back("Data");
    dataSetHisto.push_back("eta_corrected"); 

    dataSetId.push_back(-1);
    ptcutstr = "hyp1_niter10_cut" + bin + "_isData0";
    dataSetFile.push_back(fileManager(globalFileType,10,globalEnergy,globalTraking,0,0,ptcutstr));
    dataSetIsMc.push_back(1);
    dataSetStyle.push_back(2);
    dataSetColor.push_back(1);
    dataSetLegend.push_back("Effic.");
    dataSetHisto.push_back("eff_eta");

    dataSetId.push_back(-1);
    ptcutstr = "hyp1_niter10_cut" + bin + "_isData0";
    dataSetFile.push_back(fileManager(globalFileType,10,globalEnergy,globalTraking,0,0,ptcutstr));
    dataSetIsMc.push_back(1);
    dataSetStyle.push_back(1);
    dataSetColor.push_back(2);
    dataSetLegend.push_back("MC Uncor.");
    dataSetHisto.push_back("eta_data_toCorrect");

    dataSetId.push_back(-1);
    ptcutstr = "hyp1_niter10_cut" + bin + "_isData0";
    dataSetFile.push_back(fileManager(globalFileType,10,globalEnergy,globalTraking,0,0,ptcutstr));
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
    ptcutstr = "hyp1_niter10_cut5_isData1";
    dataSetFile.push_back(fileManager(globalFileType,10,globalEnergy,globalTraking,0,0,ptcutstr));
    dataSetIsMc.push_back(0);
    dataSetStyle.push_back(20);
    dataSetColor.push_back(2);
    dataSetLegend.push_back("Data");

    dataSetId.push_back(-1);
    ptcutstr = "hyp1_niter10_cut5_isData0";
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

  //-------- Mch Unfold --------
  if( nchplot ){

    plotReset();

    // Plot settings
    globalNorm     = 2;
    globalFileType = 3;
    globalHistoType= 1;


    dataSetId.push_back(-1);
    ptcutstr = "hyp1_niter10_cut0_isData1";
    dataSetFile.push_back(fileManager(globalFileType,10,globalEnergy,globalTraking,0,0,ptcutstr));
    dataSetIsMc.push_back(0);
    dataSetStyle.push_back(20);
    dataSetColor.push_back(2);
    dataSetLegend.push_back("Data");

    dataSetId.push_back(-1);
    ptcutstr = "hyp1_niter10_cut0_isData0";
    dataSetFile.push_back(fileManager(globalFileType,10,globalEnergy,globalTraking,0,0,ptcutstr));
    dataSetIsMc.push_back(1);
    dataSetStyle.push_back(1);
    dataSetColor.push_back(1);
    dataSetLegend.push_back("PYTHIA D6T");

    dir   = "unfolding";
    
    plot(dir,"nch_data_INC_beforeSDsub",0,1); 
 

  }


}
