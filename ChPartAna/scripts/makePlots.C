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

//makePlot Plots;

#include "../macro/fileManager.C"
#include "plot.C"
//#include "plot2D.C"
//#include "plotMC.C"
//#include "plotMC2D.C"

// --- What to Plot ----
bool trkplot = 1;
bool vqlplot = 0;
bool vtxplot = 0;
bool effplot = 0;
bool mtxplot = 0;
bool mltplot = 1;

// ------------------------ makePlots ---------------------------
void makePlots (int itracking = 1 , double energy = 0.9 , double ptcut = 0.4 , double etacut = 2.5 ){

  dataSetId.clear();
  dataSetIsMc.clear();
  dataSetStyle.clear();
  dataSetColor.clear();
  dataSetLegend.clear();

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
  globalEnergy = energy;
  if ( energy == 0.9 ) {
    globalLabel =  "CMS 0.9 TeV";
 
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
    globalLabel =  "CMS 2.36 TeV";

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


  TString sel,tr,all,dir,cut;
  
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
    plot(dir,"nch_"+all,1,1);
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
    
    
    plot(dir,"z_wide_"+all);
    plot(dir,"z_wide_"+all,1);

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

    plot(dir,"z_"+all);
    plot(dir,"x_"+all);
    plot(dir,"y_"+all);
    plot(dir,"chi2n_"+all);
    plot(dir,"chi2n_"+all,1);
    plot(dir,"ntracks_"+all);
    
    // gROOT->ProcessLine(".X plot2D.C(\""+dir+"\",\"xy_"+all+"\"");
    
  }
  
  //-------- EFFICIENCIES --------
  if(effplot){
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
    
    //TH1F* h[20];
    //vector <TH1F*> vh;

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
  
  
  //-------- MATRIX --------
  if(mtxplot){
    sel = "L1_hf_VtxSel";
    if ( itracking == 1 ) {
      tr  = "PV_gTr_oVtx";
    } else if  ( itracking == 2 ) {
      tr  = "PV_mbTr_fVtx";
    } else {
      cout << "itraking = " << itracking << " :option not available !!! exit !!! " << endl;
      return;
    }

    cut = "eta_ptGenCut";
    all = cut+"_"+sel+"_"+tr;
    dir = "MatrixPlots_"+all;
    cout<<dir<<endl;
    
    gROOT->ProcessLine(".X plotMC2D.C(\""+dir+"\",\"nch_matrix_"+all+"\")");
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
 
    //gROOT->ProcessLine(".X plot.C(\""+dir+"\",\"rapidity_"+all+"\"");
  }
   
}
