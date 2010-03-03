// makePlots: Do all control plots
// ---------
//
// itracking = 1 : general Tracks and offline Vtx
//             2 : minbias Tracks and ferenc  Vtx

void makePlots (int itracking = 1){
  //gROOT->ProcessLine(".X plot.C(\"VertexPlots_allVtx_oVtx\",\"chi2n_allVtx_oVtx\")");
  bool trkplot = 1;
  bool vqlplot = 0;
  bool vtxplot = 0;
  bool effplot = 0;
  bool mtxplot = 0;
  bool mltplot = 0 ;
  TString sel,tr,all,dir,cut;
  
  //------ TRACK PLOTS ---------
  if(trkplot){
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

    cout<<dir<<endl;
    gROOT->ProcessLine(".X plot.C(\""+dir+"\",\"nch_"+all+"\"");
    gROOT->ProcessLine(".X plot.C(\""+dir+"\",\"chi2n_"+all+"\"");
    gROOT->ProcessLine(".X plot.C(\""+dir+"\",\"chi2n_"+all+"\",1");
    gROOT->ProcessLine(".X plot.C(\""+dir+"\",\"pt_"+all+"\"");
    gROOT->ProcessLine(".X plot.C(\""+dir+"\",\"pt_"+all+"\",1");
    gROOT->ProcessLine(".X plot.C(\""+dir+"\",\"phi_"+all+"\"");
    gROOT->ProcessLine(".X plot.C(\""+dir+"\",\"eta_"+all+"\"");
    gROOT->ProcessLine(".X plot.C(\""+dir+"\",\"nhit_"+all+"\"");
    gROOT->ProcessLine(".X plot.C(\""+dir+"\",\"charge_"+all+"\"");
    //gROOT->ProcessLine(".X plot.C(\""+dir+"\",\"chi2n_"+all+"\"");

    if ( itracking == 1 ) {
      tr  = "allTr_gTr_oVtx";
      all = sel+"_"+tr;
      dir = "EvtSelPlots_"+tr   +   "/TrackPlots_"+sel+"_"+tr;
      cout<<dir<<endl;
      gROOT->ProcessLine(".X plot.C(\""+dir+"\",\"dzOedz_"+all+"\"");
      gROOT->ProcessLine(".X plot.C(\""+dir+"\",\"dzOedz_"+all+"\",1");
      gROOT->ProcessLine(".X plot.C(\""+dir+"\",\"dxyOed0_"+all+"\"");
      gROOT->ProcessLine(".X plot.C(\""+dir+"\",\"dxyOed0_"+all+"\",1");
      gROOT->ProcessLine(".X plot.C(\""+dir+"\",\"eptOpt_"+all+"\"");
      gROOT->ProcessLine(".X plot.C(\""+dir+"\",\"eptOpt_"+all+"\",1");
    } else if  ( itracking == 2 ) {
      tr  = "allTr_mbTr_fVtx";
      all = sel+"_"+tr;
      dir = "EvtSelPlots_"+tr   +   "/TrackPlots_"+sel+"_"+tr;
      cout<<dir<<endl;
      gROOT->ProcessLine(".X plot.C(\""+dir+"\",\"dzOsz_"+all+"\"");
      gROOT->ProcessLine(".X plot.C(\""+dir+"\",\"dzOsz_"+all+"\",1");
      gROOT->ProcessLine(".X plot.C(\""+dir+"\",\"dxybsOsxy_"+all+"\"");
      gROOT->ProcessLine(".X plot.C(\""+dir+"\",\"dxybsOsxy_"+all+"\",1");
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
    
    
    gROOT->ProcessLine(".X plot.C(\""+dir+"\",\"z_wide_"+all+"\"");
    gROOT->ProcessLine(".X plot.C(\""+dir+"\",\"z_wide_"+all+"\",1");

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
    
    gROOT->ProcessLine(".X plot.C(\""+dir+"\",\"z_"+all+"\"");
    //gROOT->ProcessLine(".X plot.C(\""+dir+"\",\"z_wide_"+all+"\"");
    //gROOT->ProcessLine(".X plot.C(\""+dir+"\",\"z_wide_"+all+"\",1");
    gROOT->ProcessLine(".X plot.C(\""+dir+"\",\"x_"+all+"\"");
    gROOT->ProcessLine(".X plot.C(\""+dir+"\",\"y_"+all+"\"");
    gROOT->ProcessLine(".X plot.C(\""+dir+"\",\"chi2n_"+all+"\"");
    gROOT->ProcessLine(".X plot.C(\""+dir+"\",\"chi2n_"+all+"\",1");
    gROOT->ProcessLine(".X plot.C(\""+dir+"\",\"ntracks_"+all+"\"");
    
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

    TH1F* h;
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
    
    gROOT->ProcessLine(".X plot.C(\""+dir+"\",\"eta_"+all+"\"");
    gROOT->ProcessLine(".X plot.C(\""+dir+"\",\"kno_"+all+"\"");
    gROOT->ProcessLine(".X plot.C(\""+dir+"\",\"kno_"+all+"\",1");
    gROOT->ProcessLine(".X plot.C(\""+dir+"\",\"nch_"+all+"\"");
    gROOT->ProcessLine(".X plot.C(\""+dir+"\",\"nch_"+all+"\",1");
    gROOT->ProcessLine(".X plot.C(\""+dir+"\",\"pt_"+all+"\"");
    gROOT->ProcessLine(".X plot.C(\""+dir+"\",\"pt_"+all+"\",1");
    gROOT->ProcessLine(".X plot.C(\""+dir+"\",\"pt2_"+all+"\"");
    gROOT->ProcessLine(".X plot.C(\""+dir+"\",\"pt2_"+all+"\",1");
    gROOT->ProcessLine(".X plot.C(\""+dir+"\",\"rapidity_"+all+"\"");
  }
   
}
