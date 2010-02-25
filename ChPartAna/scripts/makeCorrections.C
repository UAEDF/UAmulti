#ifndef
#define _MAKECORRECTIONS_

#include "TH1F.h"
#include "TH1D.h"
#include "TH2F.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TLegend.h"
#include "TPad.h"

#include "TFrame.h"

#include <iostream>

#include "../plugins/MultiPlots.h"

using namespace std;

const int matrixsize = 71;
bool useData = false;
TString name = "MC_test_uniform";

void transform2Matrix(TH2F* matrixhist,double matrix[][matrixsize]);

#include "unfolding.cc"

void makeCorrections(bool isData = false, int hyp=0 , int niter=5 , float mu = 14 , float sigma = 15 ){

   useData = isData;
  //Get the MC file
  //TFile* mc = new TFile("/user/rougny/Ferenc_Tracking_bis/CMSSW_3_3_6_patch3/src/UAmulti/ChPartAna/macro/collectionPlotter_MC_finalv2_900GeV.root","READ");
  TFile* mc = new
  TFile("../macro/collectionPlotter_simpleana_MC3_test24.root","READ");
  
  //Get the data file
  TFile* data = new TFile("../macro/collectionPlotter_simpleana_data3_test24.root","READ");
  
  
  
  
  //------------------------------------------------------------------------------
  //---------------------------- UNFOLDING ---------------------------------------
  //------------------------------------------------------------------------------
  
  //Get the Unfolding matrix
  TH2F* matrixhist = (TH2F*) mc->Get("MatrixPlots_etaGenCut_L1_hf_VtxSel_PV_gTr_oVtx/nch_matrix_etaGenCut_L1_hf_VtxSel_PV_gTr_oVtx");
/*
  for(int i=1;i<=matrixhist->GetNbinsX();++i){
    matrixhist->SetBinContent(i,1,0);
    matrixhist->SetBinContent(1,i,0);
  }
*/
  TCanvas* cm = new TCanvas("cm","c",2350,10,500,500);
  matrixhist->Draw("colz");
  //gPad->WaitPrimitive();
  
  //Transform the TH2F into an array
  matrix4dObj matrix;
  transform2Matrix(matrixhist,matrix);
  
  //get the true nch distribution (from MC)
  //TH1F* nch_trueGen = (TH1F*) mc->Get("GenMultiPlots_etaCut/MultiPlots_NSD_etaCut/nch_NSD_etaCut");
  TH1F* nch_trueGen_afterUnfolding = (TH1F*) mc->Get("MatrixPlots_etaGenCut_L1_hf_VtxSel_PV_gTr_oVtx/nch_gen_etaGenCut_L1_hf_VtxSel_PV_gTr_oVtx");
  //TH1F* nch_trueGen = (TH1F*) mc->Get("MatrixPlots_etaGenCut_L1_hf_VtxSel_PV_gTr_oVtx/nch_gen_etaGenCut_L1_hf_VtxSel_PV_gTr_oVtx");
  TH1F* nch_trueGen = (TH1F*) mc->Get("MultiPlots_etaCut_noSel_NSD_gen/nch_etaCut_noSel_NSD_gen");
  
  //nch_trueGen->Scale(1./nch_trueGen->GetEntries());
  //nch_trueGen->Draw();
  //gPad->WaitPrimitive();
  
  //get the nch to Unfold distribution
  TH1F* nch_INC;
  ////  BAD ?//if(!useData) nch_INC = (TH1F*) mc->Get("MultiPlots_L1_hf_VtxSel_PV_gTr_oVtx/nch_L1_hf_VtxSel_PV_gTr_oVtx");
  if(!useData) nch_INC = (TH1F*) mc->Get("GenMultiPlots_evtSel_reco/MultiPlots_INC_evtSel_reco/nch_INC_evtSel_reco");
  //if(!useData) nch_INC = (TH1F*) mc->Get("MatrixPlots_etaGenCut_L1_hf_VtxSel_PV_gTr_oVtx/nch_reco_etaGenCut_L1_hf_VtxSel_PV_gTr_oVtx");
  else         nch_INC = (TH1F*) data->Get("MultiPlots_evtSel_PV_gTr_oVtx/nch_evtSel_PV_gTr_oVtx");
  //else         nch_INC = (TH1F*) data->Get("MultiPlots_L1_hf_VtxSel_PV_gTr_oVtx/nch_L1_hf_VtxSel_PV_gTr_oVtx");
  
  //nch_INC->SetBinContent(1,0);
  
  
  //Getting the class
  MultiPlots* mp_INC_evtSel_reco_MC;
  MultiPlots* mp_INC_evtSel_reco_data;
  mp_INC_evtSel_reco_MC = (MultiPlots*) mc->Get("GenMultiPlots_evtSel_reco/MultiPlots_SD_evtSel_reco/multi_class_SD_evtSel_reco");
  if(useData) mp_INC_evtSel_reco_data = (MultiPlots*) data->Get("MultiPlots_evtSel_INC_reco/multi_class_evtSel_INC_reco");
  else mp_INC_evtSel_reco_data = (MultiPlots*) mc->Get("MultiPlots_evtSel_INC_reco/multi_class_evtSel_INC_reco");
  
  double MC_factor = double(mp_INC_evtSel_reco_data->nbEvts) / double(mp_INC_evtSel_reco_MC->nbEvts) ;
  cout<<"The ratio data_INC/MC_INC is "<<MC_factor<<endl;
 
   //if( useData ) {
    //TH1F* nch_trueRec = (TH1F*) mc->Get("MatrixPlots_etaGenCut_L1_hf_VtxSel_PV_gTr_oVtx/nch_reco_etaGenCut_L1_hf_VtxSel_PV_gTr_oVtx");
     
     //nch_trueRec->Draw("same");
   //} 
  
  TCanvas* cc = new TCanvas("cc","c",2350,510,500,500);
  nch_INC->Draw();
  
  TH1F* nch_evtSel_SD =  (TH1F*) mc->Get("GenMultiPlots_evtSel_reco/MultiPlots_SD_evtSel_reco/nch_SD_evtSel_reco");
  //TH1F* nch_evtSel_NSD = (TH1F*) mc->Get("MatrixPlots_etaGenCut_L1_hf_VtxSel_PV_gTr_oVtx/nch_reco_etaGenCut_L1_hf_VtxSel_PV_gTr_oVtx");
  TH1F* nch_evtSel_NSD =  (TH1F*) mc->Get("GenMultiPlots_evtSel_reco/MultiPlots_NSD_evtSel_reco/nch_NSD_evtSel_reco");
  nch_evtSel_SD->Scale(MC_factor);
  nch_evtSel_SD->SetLineColor(kRed);
  nch_evtSel_SD->Draw("hist same");
  nch_evtSel_NSD->Scale(MC_factor);
  nch_evtSel_NSD->Draw("hist same");
  TH1F* nch_NSD = (TH1F*) nch_INC->Clone("nch_NSD");
  nch_NSD->Add(nch_evtSel_SD,-1);
  nch_NSD->SetLineColor(kBlue);
  nch_NSD->Draw("e same");
   
  TLegend *leg = new TLegend (.65,.70,.90,.99);
  leg->AddEntry(nch_INC,"data - Raw","l" );
  leg->AddEntry(nch_NSD,"Data after SD corr","l" );
  leg->AddEntry(nch_evtSel_SD,"MC SD","l" );
  leg->AddEntry(nch_evtSel_NSD,"MC NSD","l" );
  leg->Draw("same");
  
  
  
  TH1F* eff_evtSel = (TH1F*) mc->Get("eff_evtSel");
  TH1F* nch_toUnfold = (TH1F*) nch_NSD->Clone("nch_toUnfold");
  //nch_toUnfold->Divide(eff_evtSel);
  
  //TEMP ----- TO CHANGE !!!
  //Opening the output file
  TFile* out = new TFile("../out/unfolding_"+name+".root","RECREATE");
  out->cd();
  
  if   (hyp == 0) 
    TH1F nch_unfolded = (runalgo(matrix,nch_toUnfold,0,niter));
  else if (hyp == 1)
    TH1F nch_unfolded = (runalgo(matrix,nch_toUnfold,nch_trueGen_afterUnfolding,niter));
  else if (hyp == 2)
    TH1F nch_unfolded = (runalgo(matrix,nch_toUnfold,nch_toUnfold,niter));
  else if (hyp == 3)
  {
    TH1F* nch_gauss =  (TH1F*) nch_trueGen->Clone("nch_gauss");
    nch_gauss->Reset();
    for (int i=0 ; i<10000 ; ++i) nch_gauss->Fill(gRandom->Gaus(mu,sigma) ) ;
    TH1F nch_unfolded = (runalgo(matrix,nch_toUnfold,nch_gauss,niter));
  }
 

  TH1F* nch_unfoldedPtr = (TH1F*) nch_unfolded.Clone("nch_unfoldedPtr");
  TH1F* nch_corrected = (TH1F*) nch_unfolded.Clone("nch_corrected");
  nch_corrected->Divide(eff_evtSel);
  
  nch_trueGen->Scale(MC_factor);
  nch_trueGen_afterUnfolding->Scale(MC_factor);
  
  TCanvas* c_effcorr = new TCanvas("c_effcorr","c_effcorr",1800,510,500,500);
  nch_trueGen->Draw("hist");
  nch_corrected->Draw("same e");
  nch_unfoldedPtr->SetLineColor(kGreen);
  nch_unfoldedPtr->Draw("same");
  nch_trueGen_afterUnfolding->Draw("same e");
  
  TLegend *leg = new TLegend (.65,.70,.90,.99);
  leg->AddEntry(nch_trueGen,"MC after Correction","l" );
  leg->AddEntry(nch_corrected,"Data after Correction","l" );
  leg->AddEntry(nch_trueGen_afterUnfolding,"MC after Unfolding","l" );
  leg->AddEntry(nch_unfoldedPtr,"Data after Unfolding","l" );
  leg->Draw("same");
  //------------------------------------------------------------------------------
  //---------------------------- Corrections -------------------------------------
  //------------------------------------------------------------------------------
/*  
  //Eta correction
  TH1F* eta_toCorrect;
  if(!useData) eta_toCorrect = (TH1F*) mc->Get("MultiPlots_L1_hf_VtxSel_PV_gTr_oVtx/eta_L1_hf_VtxSel_PV_gTr_oVtx");
  else         eta_toCorrect = (TH1F*) data->Get("MultiPlots_L1_hf_VtxSel_PV_gTr_oVtx/eta_L1_hf_VtxSel_PV_gTr_oVtx");
  
  TH1F* eta_trueGen = (TH1F*) mc->Get("GenMultiPlots_etaCut_noSel/MultiPlots_NSD_etaCut_noSel/eta_NSD_etaCut_noSel");
  
  TH1F* eff_etaCut    = (TH1F*) mc->Get("EvtSelPlots_PV_gTr_oVtx/Eff_GEN_NSD_etaCut_noSel_PV_gTr_oVtx/eff_eta_L1_hf_vtxSel_PV_gTr_oVtx");
  TH1F* eff_pt_etaCut = (TH1F*) mc->Get("EvtSelPlots_PV_gTr_oVtx/Eff_GEN_NSD_pt_eta_noSel_PV_gTr_oVtx/eff_eta_L1_hf_vtxSel_PV_gTr_oVtx");
  
  TH1F* eta_corrected  = (TH1F*) eta_toCorrect->Clone("eta_corrected");
  eta_corrected->Divide(eff_etaCut);
  
  */
  
  //------------------------------------------------------------------------------
  //------------------------------------ KNO -------------------------------------
  //------------------------------------------------------------------------------
  
  TH1F* kno = new TH1F("kno_corrected","kno_corrected;z = n_{ch} / < n_{ch} >;#psi(z)",nch_corrected->GetNbinsX(),0.,double(nch_corrected->GetXaxis()->GetXmax()/nch_corrected->GetMean()));
  kno->Sumw2();
  for( int k = 1 ; k <= nch_corrected->GetNbinsX() ; ++k){
    kno->SetBinContent(k , nch_corrected->GetMean() * nch_corrected->GetBinContent(k));
    kno->SetBinError(k , nch_corrected->GetMean() * nch_corrected->GetBinError(k));
  }
  
  TCanvas* c_kno = new TCanvas("c_kno","c_kno",200,510,500,500);
  kno->Draw();
 
  //------------------------------------------------------------------------------
  //---------------------------------- Plots -------------------------------------
  //------------------------------------------------------------------------------
  
  TCanvas* c1 = new TCanvas("c1","c",1800,10,500,500);
  c1->SetLeftMargin(0.17);
  c1->SetBottomMargin(0.10);
  c1->SetFillColor(0);
  c1->GetFrame()->SetFillColor(21);
  c1->GetFrame()->SetBorderSize(12);
  c1->SetGrid();
  
  
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(kFALSE);

/*  
  //eta plot
  eta_trueGen->SetLineWidth(2);
  eta_toCorrect->SetLineWidth(2);
  eta_corrected->SetLineWidth(2);
  eta_trueGen->SetLineColor(kRed);
  eta_toCorrect->SetLineColor(kBlue);
  eta_corrected->SetLineColor(kBlack);

  
  TLegend *leg = new TLegend (.65,.90,.90,.99);
  leg->AddEntry(eta_trueGen,"PYTHIA D6T","l" );
  leg->AddEntry(eta_toCorrect,"Data - Raw","l" );
  leg->AddEntry(eta_corrected,"Data - Corrected","l" );
  leg->SetBorderSize(0);
  leg->SetFillColor(0);
  
  eta_corrected->Draw("hist");
  eta_toCorrect->Draw("samee");
  eta_trueGen->Draw("same e");
  leg->Draw();
  c1->SaveAs("eta_"+name+".gif","");
  c1->SaveAs("eta_"+name+".eps","");
  c1->SaveAs("eta_"+name+".root","");
  
  gPad->WaitPrimitive();
*/
  
  //Unfolding
  nch_trueGen->SetLineWidth(2);
  nch_INC->SetLineWidth(2);
  nch_corrected->SetLineWidth(2);
  nch_trueGen->SetLineColor(kBlue);
  nch_INC->SetLineColor(kGreen);
  nch_corrected->SetLineColor(kRed);
 
  
 
  nch_INC->Draw("hist");
  nch_corrected->Draw("same e");
  nch_trueGen->Draw("same ");

  // UA5
  bool lua5 = true;
  if ( lua5 ) {
    ifstream ua5;
    const Int_t   n = 300 ;
    Int_t   i = 0;
    Double_t xl[n], xh[n], x[n] , y[n], eyl[n] , eyh[n] , ex[n];
    ua5.open ("/user/xjanssen/ua5_dsigdn_eta15");
    while ( ua5 >>  xl[i] >> xh[i] >> y[i] >> eyh[i] >> eyl[i] ) {
      eyl[i] = -eyl[i] ;
      x[i]  = xl[i]+(xh[i]-xl[i])/2;
      y[i] = 3*50000 * y[i];
      eyl[i]*=3*50000;
      eyh[i]*=3*50000;
      cout<<y[i]<<endl;
      ex[i] = (xh[i]-xl[i])/2;
      i++;
    }
    
    ua5.close();
    gr = new TGraphAsymmErrors(i,x,y,ex,ex,eyl,eyh);
    //gr->SetMarkerColor(0);
    //gr->SetMarkerStyle(24);
    gr->Draw("L");

  }
 

  TLegend *leg = new TLegend (.65,.90,.90,.99);
  leg->AddEntry(nch_trueGen,"PYTHIA D6T","l" );
  if (useData) {
    leg->AddEntry(nch_INC,"Data - Raw","l" );
    leg->AddEntry(nch_corrected,"Data - Corrected","l" );
  } else {
    leg->AddEntry(nch_INC,"MC - Raw","l" );
    leg->AddEntry(nch_corrected,"MC - Corrected","l" );
  }
  if (lua5) leg->AddEntry(gr,"UA5 #eta < 1.5","l"); 
  leg->SetBorderSize(0);
  leg->SetFillColor(0);

  leg->Draw();
/*
  c1->SaveAs("nch_"+name+".gif","");
  c1->SaveAs("nch_"+name+".eps","");
  c1->SaveAs("nch_"+name+".root","");
*/  

  // gPad->WaitPrimitive();

/*  
  kno->Draw("ERR");
  c1->SaveAs("kno_"+name+".gif","");
  c1->SaveAs("kno_"+name+".eps","");
  c1->SaveAs("kno_"+name+".root","");
*/
  
  //Unfolding
  nch_trueGen->Write();
  nch_toUnfold->Write();
  nch_unfolded.Write();
  matrixhist->Write();
  nch_corrected->Write();
/*
  eff_nch_L1_hf_vtxSel->Write();
  kno->Write();
*/
/*  
  //eta correction
  eta_trueGen->Write();
  eta_toCorrect->Write();
  eff_etaCut->Write();
  eta_corrected->Write();
*/ 
/*
  delete c1;
  delete cm; 
  out->Close();
  data->Close();
  mc->Close();
*/
}

void transform2Matrix(TH2F* matrixhist,double matrix[][matrixsize]){
  
  //j-->RECO || i-->GEN
  for(int i=0;i<matrixsize;++i){
    for(int j=0;j<matrixsize;++j)
      matrix[j][i] = matrixhist->GetBinContent(i+1,j+1);
  }
}


#endif
