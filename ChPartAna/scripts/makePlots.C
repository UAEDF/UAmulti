{
  //gROOT->ProcessLine(".X plot.C(\"VertexPlots_allVtx_oVtx\",\"chi2n_allVtx_oVtx\")");
  bool trkplot = true;
  bool vqlplot = false;
  bool vtxplot = false;
  bool effplot = true;
  bool mtxplot = true;
  bool mltplot = false;
  TString sel,tr,all,dir,cut;
  
  //------ TRACK PLOTS ---------
  if(trkplot){
   sel = "L1_hf_vtxSel";
   tr  = "PV_gTr_oVtx";
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
  //gROOT->ProcessLine(".X plot.C(\""+dir+"\",\"chi2n_"+all+"\"");
  
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
  }
  
  
  //-------- VERTEX QUAL --------
  if(vqlplot){
    tr  = "PV_gTr_oVtx";
    dir = "EvtSelPlots_"+tr;
  
    gROOT->ProcessLine(".X plot2D.C(\""+dir+"\",\"vtxqual_L1Sel_"+tr+"\"");
    gROOT->ProcessLine(".X plot2D.C(\""+dir+"\",\"vtxqual_L1_hfSel_"+tr+"\"");
    gROOT->ProcessLine(".X plot2D.C(\""+dir+"\",\"vtxqual_L1_hf_vtxSel_"+tr+"\"");
  
  
  
  }
    
  //------ VERTEX PLOTS ---------
  if(vtxplot){
  sel = "L1_hf_vtxSel";
  tr  = "allTr_gTr_oVtx";
  all = sel+"_"+tr;
  dir = "EvtSelPlots_"+tr   +   "/VertexPlots_"+sel+"_"+tr;
  cout<<dir<<endl;
  
  
  gROOT->ProcessLine(".X plot.C(\""+dir+"\",\"z_wide_"+all+"\"");
  gROOT->ProcessLine(".X plot.C(\""+dir+"\",\"z_wide_"+all+"\",1");
  
  tr  = "PV_gTr_oVtx";
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
  
  gROOT->ProcessLine(".X plot2D.C(\""+dir+"\",\"xy_"+all+"\"");
  
  }
  
  //-------- EFFICIENCIES --------
  if(effplot){
    tr  = "PV_gTr_oVtx";
    dir = "EvtSelPlots_"+tr   +   "/Eff_RECO_"+tr;
    cout<<dir<<endl;
    
    TH1F* h[20];
    TCanvas* c1 = new TCanvas("c1","c",200,10,500,500);
    c1->cd();
    
    gROOT->ProcessLine(".X plotMC.C(c1,h,\""+dir+"\",\"eff_nch_L1Sel_"+tr+"\"");
    gROOT->ProcessLine(".X plotMC.C(c1,h,\""+dir+"\",\"eff_nch_hfSel_"+tr+"\",1");
    gROOT->ProcessLine(".X plotMC.C(c1,h,\""+dir+"\",\"eff_nch_vtxSel_"+tr+"\",2");
    gROOT->ProcessLine(".X plotMC.C(c1,h,\""+dir+"\",\"eff_nch_L1_hf_vtxSel_"+tr+"\",3");
    gPad->Update();
    gPad->WaitPrimitive();
    
    dir = "EvtSelPlots_"+tr   +   "/Eff_GEN_"+tr;
    gROOT->ProcessLine(".X plotMC.C(c1,h,\""+dir+"\",\"eff_pt_L1Sel_"+tr+"\"");
    gROOT->ProcessLine(".X plotMC.C(c1,h,\""+dir+"\",\"eff_pt_hfSel_"+tr+"\",1");
    gROOT->ProcessLine(".X plotMC.C(c1,h,\""+dir+"\",\"eff_pt_vtxSel_"+tr+"\",2");
    gROOT->ProcessLine(".X plotMC.C(c1,h,\""+dir+"\",\"eff_pt_L1_hf_vtxSel_"+tr+"\",3");
    
    gROOT->ProcessLine(".X plotMC.C(c1,h,\""+dir+"\",\"eff_eta_L1Sel_"+tr+"\"");
    gROOT->ProcessLine(".X plotMC.C(c1,h,\""+dir+"\",\"eff_eta_hfSel_"+tr+"\",1");
    gROOT->ProcessLine(".X plotMC.C(c1,h,\""+dir+"\",\"eff_eta_vtxSel_"+tr+"\",2");
    gROOT->ProcessLine(".X plotMC.C(c1,h,\""+dir+"\",\"eff_eta_L1_hf_vtxSel_"+tr+"\",3");
    //gROOT->ProcessLine(".X plotMC.C(\""+dir+"\",\"eff_nch_vtxSel_"+tr+"\"");
    //gROOT->ProcessLine(".X plotMC.C(\""+dir+"\",\"eff_nch_L1_hf_vtxSel_"+tr+"\"");
  }
  
  
  //-------- MATRIX --------
  if(mtxplot){
    sel = "L1_hf_VtxSel";
    tr  = "PV_gTr_oVtx";
    cut = "eta_ptGenCut";
    all = cut+"_"+sel+"_"+tr;
    dir = "MatrixPlots_"+all;
    cout<<dir<<endl;
    
    gROOT->ProcessLine(".X plotMC2D.C(\""+dir+"\",\"nch_matrix_"+all+"\")");
  }
  
  
  
  //--------- MULTI PLOTS -----------
  if(mltplot){
  sel = "L1_hf_VtxSel";
  tr  = "PV_gTr_oVtx";
  all = sel+"_"+tr;
  dir = "MultiPlots_"+all;
  cout<<dir<<endl;
  
  gROOT->ProcessLine(".X plot.C(\""+dir+"\",\"eta_"+all+"\"");
  gROOT->ProcessLine(".X plot.C(\""+dir+"\",\"kno_"+all+"\"");
  gROOT->ProcessLine(".X plot.C(\""+dir+"\",\"kno_"+all+"\",1");
  gROOT->ProcessLine(".X plot.C(\""+dir+"\",\"nch_"+all+"\"");
  gROOT->ProcessLine(".X plot.C(\""+dir+"\",\"nch_"+all+"\",1");
  gROOT->ProcessLine(".X plot.C(\""+dir+"\",\"pt_"+all+"\"");
  gROOT->ProcessLine(".X plot.C(\""+dir+"\",\"pt2_"+all+"\"");
  gROOT->ProcessLine(".X plot.C(\""+dir+"\",\"rapidity_"+all+"\"");
  }
   
}
