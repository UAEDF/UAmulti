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

using namespace std;

const int matrixsize = 50;
bool useData = true;

void transform2Matrix(TH2F* matrixhist,double matrix[][matrixsize]);

#include "unfolding.cc"

void makeCorrections(){
  //Get the MC file
  TFile* mc = new TFile("/user/rougny/Ferenc_Tracking_bis/CMSSW_3_3_6_patch3/src/UAmulti/ChPartAna/macro/collectionPlotter_MC_small_test.root","READ");
  
  //Get the data file
  TFile* data = new TFile("/user/rougny/Ferenc_Tracking_bis/CMSSW_3_3_6_patch3/src/UAmulti/ChPartAna/macro/collectionPlotter_data_final_test.root","READ");
  
  //------------------------------------------------------------------------------
  //---------------------------- UNFOLDING ---------------------------------------
  //------------------------------------------------------------------------------
  
  //Get the Unfolding matrix
  TH2F* matrixhist = (TH2F*) mc->Get("MatrixPlots_etaGenCut_L1_hf_VtxSel_PV_gTr_oVtx/nch_matrix_etaGenCut_L1_hf_VtxSel_PV_gTr_oVtx");
  
  //Transform the TH2F into an array
  matrix4dObj matrix;
  transform2Matrix(matrixhist,matrix);
  
  //get the true nch distribution (from MC)
  TH1F* nch_trueGen = (TH1F*) mc->Get("GenMultiPlots_etaCut_noSel/MultiPlots_NSD_etaCut_noSel/nch_NSD_etaCut_noSel");
  
  //get the nch to Unfold distribution
  TH1F* nch_toUnfold;
  if(!useData) nch_toUnfold = (TH1F*) mc->Get("MultiPlots_L1_hf_VtxSel_PV_gTr_oVtx/nch_L1_hf_VtxSel_PV_gTr_oVtx");
  else         nch_toUnfold = (TH1F*) data->Get("MultiPlots_L1_hf_VtxSel_PV_gTr_oVtx/nch_L1_hf_VtxSel_PV_gTr_oVtx");
  
  
  TH1F nch_unfolded = (runalgo(matrix,nch_toUnfold,0));
  
  
  
  //------------------------------------------------------------------------------
  //---------------------------- Corrections -------------------------------------
  //------------------------------------------------------------------------------
  
  //Eta correction
  TH1F* eta_toCorrect;
  if(!useData) eta_toCorrect = (TH1F*) mc->Get("MultiPlots_L1_hf_VtxSel_PV_gTr_oVtx/eta_L1_hf_VtxSel_PV_gTr_oVtx");
  else         eta_toCorrect = (TH1F*) data->Get("MultiPlots_L1_hf_VtxSel_PV_gTr_oVtx/eta_L1_hf_VtxSel_PV_gTr_oVtx");
  
  TH1F* eta_trueGen = (TH1F*) mc->Get("GenMultiPlots_etaCut_noSel/MultiPlots_NSD_etaCut_noSel/eta_NSD_etaCut_noSel");
  
  TH1F* eff_etaCut    = (TH1F*) mc->Get("EvtSelPlots_PV_gTr_oVtx/Eff_GEN_NSD_etaCut_noSel_PV_gTr_oVtx/eff_eta_L1_hf_vtxSel_PV_gTr_oVtx");
  TH1F* eff_pt_etaCut = (TH1F*) mc->Get("EvtSelPlots_PV_gTr_oVtx/Eff_GEN_NSD_pt_eta_noSel_PV_gTr_oVtx/eff_eta_L1_hf_vtxSel_PV_gTr_oVtx");
  
  TH1F* eta_corrected  = (TH1F*) eta_toCorrect->Clone("eta_corrected");
  eta_corrected->Divide(eff_etaCut);
  
  
  
  
  //------------------------------------------------------------------------------
  //---------------------------- Plots -------------------------------------
  //------------------------------------------------------------------------------
  
  TCanvas* c1 = new TCanvas("c1","c",200,10,500,500);
  c1->SetLeftMargin(0.17);
  c1->SetBottomMargin(0.10);
  c1->SetFillColor(0);
  c1->GetFrame()->SetFillColor(21);
  c1->GetFrame()->SetBorderSize(12);
  c1->SetGrid();
  
  
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(kFALSE);
  
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
  c1->SaveAs("eta.gif","");
  c1->SaveAs("eta.eps","");
  c1->SaveAs("eta.root","");
  
  gPad->WaitPrimitive();
  
  //Unfolding
  nch_trueGen->SetLineWidth(2);
  nch_toUnfold->SetLineWidth(2);
  nch_unfolded.SetLineWidth(2);
  nch_trueGen->SetLineColor(kRed);
  nch_toUnfold->SetLineColor(kBlue);
  nch_unfolded.SetLineColor(kBlack);
  
  nch_toUnfold->Draw("e");
  nch_trueGen->Draw("same e");
  nch_unfolded.Draw("same hist");
  leg->Draw();
  c1->SaveAs("eta.gif","");
  c1->SaveAs("eta.eps","");
  c1->SaveAs("eta.root","");
  
  gPad->WaitPrimitive();
  
  //Opening the output file
  TFile* out = new TFile("output2.root","RECREATE");
  out->cd();
  
  //Unfolding
  nch_trueGen->Write();
  nch_toUnfold->Write();
  nch_unfolded.Write();
  
  
  //eta correction
  eta_trueGen->Write();
  eta_toCorrect->Write();
  eff_etaCut->Write();
  eta_corrected->Write();
  
  //out->Close();
}

void transform2Matrix(TH2F* matrixhist,double matrix[][matrixsize]){
  
  //j-->RECO || i-->GEN
  for(int i=0;i<matrixsize;++i){
    for(int j=0;j<matrixsize;++j)
      matrix[j][i] = matrixhist->GetBinContent(i+1,j+1);
  }
}
