#ifndef _MAKECORRECTIONS_
#define _MAKECORRECTIONS_

#include "TH1F.h"
#include "TF1.h"
#include "TH1D.h"
#include "TH2F.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TLegend.h"
#include "TPad.h"
#include "TRandom.h"
#include "TGraphAsymmErrors.h"

#include "TFrame.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include "../plugins/MultiPlots.h"

#endif
using namespace std;

const int matrixsize = 71;
bool useData = false;
//TString name = "data_uniform_0.9_eta2.5_v004";
TString name = "test";

TString st(string input , int cut){
  stringstream out("");
  out<<cut;
  return input+"_cut"+out.str();
}

void transform2Matrix(TH2F* matrixhist,double matrix[][matrixsize]);
TH1F resample(double matrix[][matrixsize], TH1F*, TH1F*, int = 5 , bool = false);

//iFileType,iDataType,Energy,iTracking,iSystType,iSystSign,STest
#include "../macro/fileManager.C"
#include "unfolding.cc"

void makeCorrections(bool isData = false, int hyp=0 , int niter=5 , int acc = 0 , 
                     double E = 0.9 , double type = 10 , int iTr = 0 , bool drawcanv = true ,
		     float mu = 14 , float sigma = 15 ){

   useData = isData;
   #include "../macro/acceptanceMap.C"
  //Get the MC file
  //TFile* mc = new TFile("/user/rougny/Ferenc_Tracking_bis/CMSSW_3_3_6_patch3/src/UAmulti/ChPartAna/macro/collectionPlotter_MC_finalv2_900GeV.root","READ");
  //TFile* mc = new TFile("../macro/GOODFILES/MC_v005b_2.36TeV_eta2.5_pt0.4_gTr.root","READ");
  //TFile* mc = new TFile("../macro/GOODFILES/MC_v005b_0.9TeV_eta2.5_pt0.4_gTr.root","READ");
  TFile* mc = new TFile(fileManager(2,type,E,iTr),"READ");
  
  //Get the data file
  //TFile* data = new TFile("../macro/GOODFILES/data_v005b_2.36TeV_eta2.5_pt0.4_gTr.root","READ");
  //TFile* data = new TFile("../macro/GOODFILES/data_v005b_0.9TeV_eta2.5_pt0.4_gTr.root","READ");
  TFile* data = new TFile(fileManager(2,0,E,iTr),"READ");
  
  ostringstream dirstr("");
  dirstr << "/ptGen" << accMap.at(acc).at(0) << "_etaGen" << accMap.at(acc).at(1) 
      << "_ptReco" << accMap.at(acc).at(2) << "_etaReco" << accMap.at(acc).at(3) ;
  if(accMap.at(acc).at(4)==-1)
    dirstr << "_chargemin";
  else if(accMap.at(acc).at(4)==1)
    dirstr << "_chargeplus";
  dirstr << "/";
  TString dir = dirstr.str();
  //cout<<dir<<endl;
  //return;
  
  //TEMP ----- TO CHANGE !!!
  //Opening the output file
  ostringstream outstr("");
  outstr << "hyp" << hyp << "_niter" << niter << "_cut" << acc << "_isData" << isData;
  cout<<"Output file : "<<fileManager(3,type,E,iTr,0,0,outstr.str())<<endl;
  TFile* out = new TFile(fileManager(3,type,E,iTr,0,0,outstr.str()),"RECREATE");
  out->cd();
  
  
  //------------------------------------------------------------------------------
  //---------------------------- UNFOLDING ---------------------------------------
  //------------------------------------------------------------------------------
  
  gDirectory->mkdir("unfolding");
  gDirectory->cd("unfolding");
  
  //Get the Unfolding matrix
  //TFile* mc_matrix = new TFile("../macro/GOODFILES/MC_v005b_2.36TeV_eta2.5_pt0.4_gTr.root");
  //TH2F* matrixhist = (TH2F*) mc_matrix->Get("MatrixPlots_etaGenCut_L1_hf_VtxSel_PV_gTr_oVtx/nch_matrix_etaGenCut_L1_hf_VtxSel_PV_gTr_oVtx");
  TH2F* matrixhist = (TH2F*) mc->Get(dir+st("MatrixPlots_evtSel",acc)+st("/nch_matrix_evtSel",acc));
  matrixhist->SetName("nch_matrix");
  matrixhist->Write();
/*
  for(int i=1;i<=matrixhist->GetNbinsX();++i){
    matrixhist->SetBinContent(i,1,0);
    matrixhist->SetBinContent(1,i,0);
  }
*/
  if(drawcanv){
    TCanvas* cm = new TCanvas("cm","c",2500,10,500,500);
    matrixhist->Draw("colz");
    //gPad->WaitPrimitive();
  }
  
  //Transform the TH2F into an array
  matrix4dObj matrix;
  transform2Matrix(matrixhist,matrix);
  
  //get the true nch distribution (from MC)
  //TH1F* nch_trueGen = (TH1F*) mc->Get("GenMultiPlots_etaCut/MultiPlots_NSD_etaCut/nch_NSD_etaCut");
  TH1F* nch_trueGen_afterUnfolding = (TH1F*) mc->Get(dir+st("MatrixPlots_evtSel",acc)+st("/nch_gen_evtSel",acc));
  nch_trueGen_afterUnfolding->SetName("nch_MC_gen_afterUnfolding");
  //TH1F* nch_trueGen = (TH1F*) mc->Get("MatrixPlots_etaGenCut_L1_hf_VtxSel_PV_gTr_oVtx/nch_gen_etaGenCut_L1_hf_VtxSel_PV_gTr_oVtx");
  TH1F* nch_trueGen = (TH1F*) mc->Get(dir+st("MultiPlots_etaCut_noSel_NSD_gen",acc)+st("/nch_etaCut_noSel_NSD_gen",acc));
  nch_trueGen->SetName("nch_MC_gen_afterEvtSelCorrection");
  
  //nch_trueGen->Scale(1./nch_trueGen->GetEntries());
  //nch_trueGen->Draw();
  //gPad->WaitPrimitive();
  
  //get the nch to Unfold distribution
  TH1F* nch_INC;
  if(!useData)
    //nch_INC = (TH1F*) mc->Get(dir+st("GenMultiPlots_evtSel_reco",acc)+st("/MultiPlots_INC_evtSel_reco",acc)+st("/nch_INC_evtSel_reco",acc));
    nch_INC = (TH1F*) mc->Get(dir+st("MultiPlots_evtSel_INC_reco",acc)+st("/nch_evtSel_INC_reco",acc));
  else
    nch_INC = (TH1F*) data->Get(dir+st("MultiPlots_evtSel_INC_reco",acc)+st("/nch_evtSel_INC_reco",acc));
    //nch_INC = (TH1F*) data->Get(dir+st("MultiPlots_evtSel_PV",acc)+st("/nch_evtSel_PV",acc));
  nch_INC->SetName("nch_data_INC_beforeSDsub");
  
  //nch_INC->SetBinContent(1,0);
  
  
  //Getting the class
  MultiPlots* mp_INC_evtSel_reco_MC;
  MultiPlots* mp_INC_evtSel_reco_data;
  mp_INC_evtSel_reco_MC = (MultiPlots*) mc->Get(dir+st("GenMultiPlots_evtSel_reco",acc)+st("/MultiPlots_SD_evtSel_reco",acc)+st("/multi_class_SD_evtSel_reco",acc));
  if(useData) mp_INC_evtSel_reco_data = (MultiPlots*) data->Get(dir+st("MultiPlots_evtSel_INC_reco",acc)+st("/multi_class_evtSel_INC_reco",acc));
  else mp_INC_evtSel_reco_data = (MultiPlots*) mc->Get(dir+st("MultiPlots_evtSel_INC_reco",acc)+st("/multi_class_evtSel_INC_reco",acc));
  
  //nch_INC->Scale(1./double(mp_INC_evtSel_reco_data->nbEvts));
  double MC_factor;
  if(nch_INC->Integral()<2)
    MC_factor = 1. / double(mp_INC_evtSel_reco_MC->nbEvts) ;
  else
    MC_factor = double(mp_INC_evtSel_reco_data->nbEvts) / double(mp_INC_evtSel_reco_MC->nbEvts) ;
  cout<<"The ratio data_INC/MC_INC is "<<MC_factor<<endl;
 
   //if( useData ) {
    //TH1F* nch_trueRec = (TH1F*) mc->Get("MatrixPlots_etaGenCut_L1_hf_VtxSel_PV_gTr_oVtx/nch_reco_etaGenCut_L1_hf_VtxSel_PV_gTr_oVtx");
     
     //nch_trueRec->Draw("same");
   //} 
   
   
   
  //------------------------------------------------------------------------------
  //---------------------------- SD Substraction ---------------------------------
  //------------------------------------------------------------------------------
  
  
  TH1F* nch_evtSel_SD =  (TH1F*) mc->Get(dir+st("GenMultiPlots_evtSel_reco",acc)+st("/MultiPlots_SD_evtSel_reco",acc)+st("/nch_SD_evtSel_reco",acc));
  TH1F* nch_evtSel_NSD = (TH1F*) mc->Get(dir+st("GenMultiPlots_evtSel_reco",acc)+st("/MultiPlots_NSD_evtSel_reco",acc)+st("/nch_NSD_evtSel_reco",acc));
  nch_evtSel_SD->SetName("nch_MC_gen_SD");
  nch_evtSel_NSD->SetName("nch_MC_gen_NSD");
  nch_evtSel_SD->Scale(MC_factor);
  nch_evtSel_NSD->Scale(MC_factor);
  TH1F* nch_NSD = (TH1F*) nch_INC->Clone("nch_data_NSD_afterSDsub");
  nch_NSD->Add(nch_evtSel_SD,-1);
  
  if(drawcanv){
    TCanvas* c_SDsub = new TCanvas("c_SDsub","c_SDsub",2500,510,500,500);
    nch_INC->Draw();
    nch_evtSel_SD->SetLineColor(kRed);
    nch_evtSel_SD->Draw("hist same");
    nch_evtSel_NSD->Draw("hist same");
    nch_NSD->SetLineColor(kBlue);
    nch_NSD->Draw("e same");
  
    TLegend *leg = new TLegend (.65,.70,.90,.99);
    leg->AddEntry(nch_INC,"data - Raw","l" );
    leg->AddEntry(nch_NSD,"Data after SD corr","l" );
    leg->AddEntry(nch_evtSel_SD,"MC SD","l" );
    leg->AddEntry(nch_evtSel_NSD,"MC NSD","l" );
    leg->Draw("same");
  }
  
  TH1F* eff_evtSel = (TH1F*) mc->Get(dir+st("eff_evtSel",acc));
  eff_evtSel->SetName("eff_evtSel");
  TH1F* nch_toUnfold = (TH1F*) nch_NSD->Clone("nch_toUnfold");
  //nch_toUnfold->Divide(eff_evtSel);
  
  nch_evtSel_SD->Write();
  nch_evtSel_NSD->Write();
  
  
  //------------------------------------------------------------------------------
  //---------------------------- Unfolding ---------------------------------------
  //------------------------------------------------------------------------------
  
  TH1F nch_unfolded("","",1,0,1);
  TH1F* hypothesis = (TH1F*) nch_trueGen->Clone("hypothesis");
  if   (hyp == 0){
    for (int i=1;i<=hypothesis->GetNbinsX();i++)
      hypothesis->SetBinContent(i,1.);
  }
  else if (hyp == 1)
    hypothesis = nch_trueGen_afterUnfolding;
  else if (hyp == 2)
    hypothesis = nch_toUnfold;
  else if (hyp == 3)
  {
    hypothesis->Reset();
    for (int i=0 ; i<10000 ; ++i) hypothesis->Fill(gRandom->Gaus(mu,sigma) ) ;
  }
  
  hypothesis->Write();
  
  nch_unfolded = (runalgo(matrix,nch_toUnfold,hypothesis,niter));
  TH1F* nch_unfoldedPtr = (TH1F*) nch_unfolded.Clone("nch_unfoldedPtr");
  
  
  //------------------------------------------------------------------------------
  //---------------------------- Resampling -------------------------------------
  //------------------------------------------------------------------------------
  
  TH1F nch_resampled = resample(matrix,nch_toUnfold,hypothesis,niter);
  TH1F* nch_resampledPtr = &nch_resampled;
  
  if(drawcanv){
    TCanvas* c_resample = new TCanvas("resampling","resampling",1460,510,500,500);
    c_resample->cd();
    nch_unfoldedPtr->SetLineColor(kBlack);
    nch_unfoldedPtr->Draw("hist");
    nch_resampledPtr->Draw("e");
    //gPad->WaitPrimitive();
  }
  
  for(int nbin = 1 ; nbin<=nch_unfoldedPtr->GetNbinsX() ; ++nbin)
    nch_unfoldedPtr->SetBinError(nbin , nch_resampledPtr->GetBinError(nbin));
  
  
  //------------------------------------------------------------------------------
  //---------------------------- ReScaling -------------------------------------
  //------------------------------------------------------------------------------
  
  
  /*nch_unfoldedPtr->Scale(1./double(mp_INC_evtSel_reco_data->nbEvts));
  nch_INC->Scale(1./double(mp_INC_evtSel_reco_data->nbEvts));
  if(nch_unfoldedPtr->Integral()<2)
    MC_factor = 1. / double(mp_INC_evtSel_reco_MC->nbEvts) ;
  else
    MC_factor = double(mp_INC_evtSel_reco_data->nbEvts) / double(mp_INC_evtSel_reco_MC->nbEvts) ;
  */  
    
  //------------------------------------------------------------------------------
  //---------------------------- EvtSel Correction -------------------------------------
  //------------------------------------------------------------------------------
  
  TH1F* nch_corrected = (TH1F*) nch_unfoldedPtr->Clone("nch_data_corrected");
  //eff_evtSel->Sumw2();
  nch_corrected->Divide(eff_evtSel);
  
  nch_trueGen->Scale(MC_factor);
  nch_trueGen_afterUnfolding->Scale(MC_factor);
  
  if(drawcanv){
    TCanvas* c_effcorr = new TCanvas("c_effcorr","c_effcorr",1980,510,500,500);
    nch_trueGen->Draw("hist");
    nch_corrected->Draw("same e");
    nch_trueGen_afterUnfolding->Draw("same hist");
    nch_unfoldedPtr->SetLineWidth(2);
    nch_unfoldedPtr->SetLineColor(kGreen);
    nch_unfoldedPtr->Draw("same e");
  
    TLegend* leg = new TLegend (.65,.70,.90,.99);
    leg->AddEntry(nch_trueGen,"MC after Correction","l" );
    leg->AddEntry(nch_corrected,"Data after Correction","l" );
    leg->AddEntry(nch_trueGen_afterUnfolding,"MC after Unfolding","l" );
    leg->AddEntry(nch_unfoldedPtr,"Data after Unfolding","l" );
    leg->Draw("same");
  }
  cout<<"Mean of multiplicity --------> "<<nch_corrected->GetMean()<<endl;
  cout<<"RMS of multiplicity  --------> "<<nch_corrected->GetRMS()<<endl;
  
  
  
    //------------------------------------------------------------------------------
  //------------------------------------ KNO -------------------------------------
  //------------------------------------------------------------------------------
  
  TH1F* kno = new TH1F("kno_corrected","kno_corrected;z = n_{ch} / < n_{ch} >;#psi(z)",nch_corrected->GetNbinsX(),0.,double(nch_corrected->GetXaxis()->GetXmax()/nch_corrected->GetMean()));
  kno->Sumw2();
  /*for( int k = 60 ; k <= nch_corrected->GetNbinsX() ; ++k)
    nch_corrected->SetBinContent(k,0);*/
  for( int k = 1 ; k <= nch_corrected->GetNbinsX() ; ++k){
    kno->SetBinContent(k , nch_corrected->GetMean() * nch_corrected->GetBinContent(k));
    kno->SetBinError(k , nch_corrected->GetMean() * nch_corrected->GetBinError(k));
  }
  if(drawcanv){
    TCanvas* c_kno = new TCanvas("c_kno","c_kno",200,510,500,500);
    kno->Draw();
  }
  
  TH1F* kno_gen = new TH1F("kno_gen","kno_gen;z = n_{ch} / < n_{ch} >;#psi(z)",nch_trueGen->GetNbinsX(),0.,double(nch_trueGen->GetXaxis()->GetXmax()/nch_trueGen->GetMean()));
  kno_gen->Sumw2();
  for( int k = 1 ; k <= nch_trueGen->GetNbinsX() ; ++k){
    kno_gen->SetBinContent(k , nch_trueGen->GetMean() * nch_trueGen->GetBinContent(k));
    kno_gen->SetBinError(k , nch_trueGen->GetMean() * nch_trueGen->GetBinError(k));
  }
  kno_gen->Write();
  
  
  //------------------------------------------------------------------------------
  //---------------------------------- Plots -------------------------------------
  //------------------------------------------------------------------------------
  
if(drawcanv){

  TCanvas* c_final = new TCanvas("c_final","c_final",1980,10,500,500);
  c_final->SetLeftMargin(0.17);
  c_final->SetBottomMargin(0.10);
  c_final->SetFillColor(0);
  c_final->GetFrame()->SetFillColor(21);
  c_final->GetFrame()->SetBorderSize(12);
  c_final->SetGrid();
  
  //Unfolding
  nch_trueGen->SetLineWidth(2);
  nch_INC->SetLineWidth(2);
  nch_corrected->SetLineWidth(2);
  nch_trueGen->SetLineColor(kBlue);
  nch_INC->SetLineColor(kGreen);
  nch_corrected->SetLineColor(kRed);
 
  
 
  nch_INC->Draw("hist");
  nch_trueGen->Draw("same hist");
  nch_corrected->Draw("same e");

  // UA5
  bool lua5 = true;
  TGraphAsymmErrors* ua5_multi;
  if ( lua5 ) {
    ifstream ua5("/user/xjanssen/ua5_dsigdn_eta15");
    const Int_t   n = 300 ;
    Int_t   i = 0;
    Double_t xl[n], xh[n], x[n] , y[n], eyl[n] , eyh[n] , ex[n];
    //ua5.open ("/user/xjanssen/ua5_dsigdn_eta15");
    while ( ua5 >>  xl[i] >> xh[i] >> y[i] >> eyh[i] >> eyl[i] ) {
      eyl[i] = -eyl[i] ;
      x[i]  = xl[i]+(xh[i]-xl[i])/2;
      y[i] = 3*50000 * y[i];
      eyl[i]*=3*50000;
      eyh[i]*=3*50000;
      //cout<<y[i]<<endl;
      ex[i] = (xh[i]-xl[i])/2;
      i++;
    }
    
    ua5.close();
    ua5_multi = new TGraphAsymmErrors(i,x,y,ex,ex,eyl,eyh);
    //gr->SetMarkerColor(0);
    //gr->SetMarkerStyle(24);
    ua5_multi->Draw("L");
    ua5_multi->Write();
  }
 

  TLegend* leg = new TLegend (.65,.90,.90,.99);
  leg->AddEntry(nch_trueGen,"PYTHIA D6T","l" );
  if (useData) {
    leg->AddEntry(nch_INC,"Data - Raw","l" );
    leg->AddEntry(nch_corrected,"Data - Corrected","l" );
  } else {
    leg->AddEntry(nch_INC,"MC - Raw","l" );
    leg->AddEntry(nch_corrected,"MC - Corrected","l" );
  }
  if (lua5) leg->AddEntry(ua5_multi,"UA5 #eta < 1.5","l"); 
  leg->SetBorderSize(0);
  leg->SetFillColor(0);

  leg->Draw();
}
  
  //Unfolding
  nch_trueGen->Write();
  nch_trueGen_afterUnfolding->Write();
  nch_INC->Write();
  nch_NSD->Write();
  nch_unfoldedPtr->Write();
  nch_corrected->Write();
  nch_resampledPtr->Write();

  //eff_nch_L1_hf_vtxSel->Write();
  kno->Write();

  gDirectory->cd("../");

  
  //------------------------------------------------------------------------------
  //---------------------------- ETA Corrections ---------------------------------
  //------------------------------------------------------------------------------

  //Eta correction
  TH1F* eta_toCorrect;
  if(!useData) eta_toCorrect = (TH1F*) mc->Get(dir+st("MultiPlots_evtSel_INC_reco",acc)+st("/eta_evtSel_INC_reco",acc));
  else         eta_toCorrect = (TH1F*) data->Get(dir+st("MultiPlots_evtSel_INC_reco",acc)+st("/eta_evtSel_INC_reco",acc));
  eta_toCorrect->SetName("eta_data_toCorrect");
  
  TH1F* eta_noSel_NSD_gen = (TH1F*) mc->Get(dir+st("MultiPlots_etaCut_noSel_NSD_gen",acc)+st("/eta_etaCut_noSel_NSD_gen",acc));
  TH1F* eta_evtSel_INC_reco = (TH1F*) mc->Get(dir+st("MultiPlots_evtSel_INC_reco",acc)+st("/eta_evtSel_INC_reco",acc));
  eta_noSel_NSD_gen->SetName("eta_MC_gen_afterCorrection");
  eta_evtSel_INC_reco->SetName("eta_MC_reco_beforeCorrection");
  
  TH1F* eff_eta = (TH1F*) eta_evtSel_INC_reco->Clone("eff_eta");
  eff_eta->Divide(eta_noSel_NSD_gen);
  
  eta_toCorrect->Scale(1./double(mp_INC_evtSel_reco_data->nbEvts));
  TH1F* eta_corrected  = (TH1F*) eta_toCorrect->Clone("eta_corrected");
  eta_corrected->Divide(eff_eta);
  
  eta_noSel_NSD_gen->Scale(MC_factor/double(mp_INC_evtSel_reco_data->nbEvts));
  
if(drawcanv){
  TCanvas* c_eta = new TCanvas("c_eta","c_eta",1460,10,500,500);
  c_eta->SetLeftMargin(0.17);
  c_eta->SetBottomMargin(0.10);
  c_eta->SetFillColor(0);
  c_eta->GetFrame()->SetFillColor(21);
  c_eta->GetFrame()->SetBorderSize(12);
  c_eta->SetGrid();
  
  
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(kFALSE);


  //eta plot
  eta_noSel_NSD_gen->SetLineWidth(2);
  eta_toCorrect->SetLineWidth(2);
  eta_corrected->SetLineWidth(2);
  eta_noSel_NSD_gen->SetLineColor(kRed);
  eta_toCorrect->SetLineColor(kBlue);
  eta_corrected->SetLineColor(kBlack);

  
  TLegend *leg = new TLegend (.65,.90,.90,.99);
  leg->AddEntry(eta_noSel_NSD_gen,"PYTHIA D6T","l" );
  leg->AddEntry(eta_toCorrect,"Data - Raw","l" );
  leg->AddEntry(eta_corrected,"Data - Corrected","l" );
  leg->SetBorderSize(0);
  leg->SetFillColor(0);
  
  eta_corrected->Draw("hist");
  eta_toCorrect->Draw("samee");
  eta_noSel_NSD_gen->Draw("samee");
  leg->Draw();
  /*c1->SaveAs("eta_"+name+".gif","");
  c1->SaveAs("eta_"+name+".eps","");
  c1->SaveAs("eta_"+name+".root","");*/
  
  gPad->WaitPrimitive();
}
  
  gDirectory->mkdir("eta");
  gDirectory->cd("eta");

  eta_noSel_NSD_gen->Write();
  eta_evtSel_INC_reco->Write();
  eta_toCorrect->Write();
  eff_eta->Write();
  eta_corrected->Write();
  
  gDirectory->cd("../");
  
  //------------------------------------------------------------------------------
  //---------------------------- pt Corrections ---------------------------------
  //------------------------------------------------------------------------------

  //pt correction
  TH1F* pt_toCorrect;
  if(!useData) pt_toCorrect = (TH1F*) mc->Get(dir+st("MultiPlots_evtSel_INC_reco",acc)+st("/pt_evtSel_INC_reco",acc));
  else         pt_toCorrect = (TH1F*) data->Get(dir+st("MultiPlots_evtSel_INC_reco",acc)+st("/pt_evtSel_INC_reco",acc));
  pt_toCorrect->SetName("pt_data_toCorrect");

  TH1F* pt_noSel_NSD_gen = (TH1F*) mc->Get(dir+st("MultiPlots_etaCut_noSel_NSD_gen",acc)+st("/pt_etaCut_noSel_NSD_gen",acc));
  TH1F* pt_evtSel_INC_reco = (TH1F*) mc->Get(dir+st("MultiPlots_evtSel_INC_reco",acc)+st("/pt_evtSel_INC_reco",acc));
  pt_noSel_NSD_gen->SetName("pt_MC_gen_afterCorrection");
  pt_evtSel_INC_reco->SetName("pt_MC_reco_beforeCorrection");

  TH1F* eff_pt = (TH1F*) pt_evtSel_INC_reco->Clone("eff_pt");
  eff_pt->Divide(pt_noSel_NSD_gen);
  
  pt_toCorrect->Scale(1./double(mp_INC_evtSel_reco_data->nbEvts));
  TH1F* pt_corrected  = (TH1F*) pt_toCorrect->Clone("pt_corrected");
  pt_corrected->Divide(eff_pt);
  
  pt_noSel_NSD_gen->Scale(MC_factor/double(mp_INC_evtSel_reco_data->nbEvts));
  
if(drawcanv){
  TCanvas* c_pt = new TCanvas("c_pt","c_pt",1460,10,500,500);
  c_pt->SetLeftMargin(0.17);
  c_pt->SetBottomMargin(0.10);
  c_pt->SetFillColor(0);
  c_pt->GetFrame()->SetFillColor(21);
  c_pt->GetFrame()->SetBorderSize(12);
  c_pt->SetGrid();
  
  
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(kFALSE);


  //pt plot
  pt_noSel_NSD_gen->SetLineWidth(2);
  pt_toCorrect->SetLineWidth(2);
  pt_corrected->SetLineWidth(2);
  pt_noSel_NSD_gen->SetLineColor(kRed);
  pt_toCorrect->SetLineColor(kBlue);
  pt_corrected->SetLineColor(kBlack);

  
  TLegend *leg = new TLegend (.65,.90,.90,.99);
  leg->AddEntry(pt_noSel_NSD_gen,"PYTHIA D6T","l" );
  leg->AddEntry(pt_toCorrect,"Data - Raw","l" );
  leg->AddEntry(pt_corrected,"Data - Corrected","l" );
  leg->SetBorderSize(0);
  leg->SetFillColor(0);
  
  pt_corrected->Draw("hist");
  pt_toCorrect->Draw("samee");
  pt_noSel_NSD_gen->Draw("samee");
  leg->Draw();
  /*c1->SaveAs("pt_"+name+".gif","");
  c1->SaveAs("pt_"+name+".eps","");
  c1->SaveAs("pt_"+name+".root","");*/
  
  gPad->WaitPrimitive();
}
  
  gDirectory->mkdir("pt");
  gDirectory->cd("pt");


  pt_noSel_NSD_gen->Write();
  pt_evtSel_INC_reco->Write();
  pt_toCorrect->Write();
  eff_pt->Write();
  pt_corrected->Write();
  
  gDirectory->cd("../");
  
  //------------------------------------------------------------------------------
  //---------------------------- pt2 Corrections ---------------------------------
  //------------------------------------------------------------------------------

  //pt2 correction
  TH1F* pt2_toCorrect;
  if(!useData) pt2_toCorrect = (TH1F*) mc->Get(dir+st("MultiPlots_evtSel_INC_reco",acc)+st("/pt2_evtSel_INC_reco",acc));
  else         pt2_toCorrect = (TH1F*) data->Get(dir+st("MultiPlots_evtSel_INC_reco",acc)+st("/pt2_evtSel_INC_reco",acc));
  pt2_toCorrect->SetName("pt2_data_toCorrect");

  
  TH1F* pt2_noSel_NSD_gen = (TH1F*) mc->Get(dir+st("MultiPlots_etaCut_noSel_NSD_gen",acc)+st("/pt2_etaCut_noSel_NSD_gen",acc));
  TH1F* pt2_evtSel_INC_reco = (TH1F*) mc->Get(dir+st("MultiPlots_evtSel_INC_reco",acc)+st("/pt2_evtSel_INC_reco",acc));
  pt2_noSel_NSD_gen->SetName("pt2_MC_gen_afterCorrection");
  pt2_evtSel_INC_reco->SetName("pt2_MC_reco_beforeCorrection");

  
  TH1F* eff_pt2 = (TH1F*) pt2_evtSel_INC_reco->Clone("eff_pt2");
  eff_pt2->Divide(pt2_noSel_NSD_gen);
  
  pt2_toCorrect->Scale(1./double(mp_INC_evtSel_reco_data->nbEvts));
  TH1F* pt2_corrected  = (TH1F*) pt2_toCorrect->Clone("pt2_corrected");
  pt2_corrected->Divide(eff_pt2);
  
  pt2_noSel_NSD_gen->Scale(MC_factor/double(mp_INC_evtSel_reco_data->nbEvts));
  
if(drawcanv){
  TCanvas* c_pt2 = new TCanvas("c_pt2","c_pt2",1460,10,500,500);
  c_pt2->SetLeftMargin(0.17);
  c_pt2->SetBottomMargin(0.10);
  c_pt2->SetFillColor(0);
  c_pt2->GetFrame()->SetFillColor(21);
  c_pt2->GetFrame()->SetBorderSize(12);
  c_pt2->SetGrid();
  
  
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(kFALSE);


  //pt2 plot
  pt2_noSel_NSD_gen->SetLineWidth(2);
  pt2_toCorrect->SetLineWidth(2);
  pt2_corrected->SetLineWidth(2);
  pt2_noSel_NSD_gen->SetLineColor(kRed);
  pt2_toCorrect->SetLineColor(kBlue);
  pt2_corrected->SetLineColor(kBlack);

  
  TLegend *leg = new TLegend (.65,.90,.90,.99);
  leg->AddEntry(pt2_noSel_NSD_gen,"PYTHIA D6T","l" );
  leg->AddEntry(pt2_toCorrect,"Data - Raw","l" );
  leg->AddEntry(pt2_corrected,"Data - Corrected","l" );
  leg->SetBorderSize(0);
  leg->SetFillColor(0);
  
  pt2_corrected->Draw("hist");
  pt2_toCorrect->Draw("samee");
  pt2_noSel_NSD_gen->Draw("samee");
  leg->Draw();
  /*c1->SaveAs("pt2_"+name+".gif","");
  c1->SaveAs("pt2_"+name+".eps","");
  c1->SaveAs("pt2_"+name+".root","");*/
  
  gPad->WaitPrimitive();
}
  
  gDirectory->mkdir("pt2");
  gDirectory->cd("pt2");

  pt2_noSel_NSD_gen->Write();
  pt2_evtSel_INC_reco->Write();
  pt2_toCorrect->Write();
  eff_pt2->Write();
  pt2_corrected->Write();
  
  gDirectory->cd("../");
  
  
  //------------------------------------------------------------------------------
  //---------------------------- nch2 Corrections ---------------------------------
  //------------------------------------------------------------------------------

  //nch2 correction
  TH1F* nch2_toCorrect;
  if(!useData) nch2_toCorrect = (TH1F*) mc->Get(dir+st("MultiPlots_evtSel_INC_reco",acc)+st("/nch_evtSel_INC_reco",acc));
  else         nch2_toCorrect = (TH1F*) data->Get(dir+st("MultiPlots_evtSel_INC_reco",acc)+st("/nch_evtSel_INC_reco",acc));
  nch2_toCorrect->SetName("nch2_data_toCorrect");

  TH1F* nch2_noSel_NSD_gen = (TH1F*) mc->Get(dir+st("MultiPlots_etaCut_noSel_NSD_gen",acc)+st("/nch_etaCut_noSel_NSD_gen",acc));
  TH1F* nch2_evtSel_INC_reco = (TH1F*) mc->Get(dir+st("MultiPlots_evtSel_INC_reco",acc)+st("/nch_evtSel_INC_reco",acc));
  nch2_noSel_NSD_gen->SetName("nch2_MC_gen_afterCorrection");
  nch2_evtSel_INC_reco->SetName("nch2_MC_reco_beforeCorrection");

  TH1F* eff_nch2 = (TH1F*) nch2_evtSel_INC_reco->Clone("eff_nch2");
  eff_nch2->Divide(nch2_noSel_NSD_gen);
  
  nch2_toCorrect->Scale(1./double(mp_INC_evtSel_reco_data->nbEvts));
  TH1F* nch2_corrected  = (TH1F*) nch2_toCorrect->Clone("nch2_corrected");
  nch2_corrected->Divide(eff_nch2);
  
  nch2_noSel_NSD_gen->Scale(MC_factor/double(mp_INC_evtSel_reco_data->nbEvts));
  
if(drawcanv){
  TCanvas* c_nch2 = new TCanvas("c_nch2","c_nch2",1460,10,500,500);
  c_nch2->SetLeftMargin(0.17);
  c_nch2->SetBottomMargin(0.10);
  c_nch2->SetFillColor(0);
  c_nch2->GetFrame()->SetFillColor(21);
  c_nch2->GetFrame()->SetBorderSize(12);
  c_nch2->SetGrid();
  
  
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(kFALSE);


  //nch2 plot
  nch2_noSel_NSD_gen->SetLineWidth(2);
  nch2_toCorrect->SetLineWidth(2);
  nch2_corrected->SetLineWidth(2);
  nch2_noSel_NSD_gen->SetLineColor(kRed);
  nch2_toCorrect->SetLineColor(kBlue);
  nch2_corrected->SetLineColor(kBlack);

  
  TLegend *leg = new TLegend (.65,.90,.90,.99);
  leg->AddEntry(nch2_noSel_NSD_gen,"PYTHIA D6T","l" );
  leg->AddEntry(nch2_toCorrect,"Data - Raw","l" );
  leg->AddEntry(nch2_corrected,"Data - Corrected","l" );
  leg->SetBorderSize(0);
  leg->SetFillColor(0);
  
  nch2_corrected->Draw("hist");
  nch2_toCorrect->Draw("samee");
  nch2_noSel_NSD_gen->Draw("samee");
  leg->Draw();
  /*c1->SaveAs("nch2_"+name+".gif","");
  c1->SaveAs("nch2_"+name+".eps","");
  c1->SaveAs("nch2_"+name+".root","");*/
  
  gPad->WaitPrimitive();
}
  
  gDirectory->mkdir("nch");
  gDirectory->cd("nch");

  nch2_noSel_NSD_gen->Write();
  nch2_evtSel_INC_reco->Write();
  nch2_toCorrect->Write();
  eff_nch2->Write();
  nch2_corrected->Write();
  
  gDirectory->cd("../");
  

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

void makeKNO(TH1F* multi,TH1F* kno){
  for( int k = 1 ; k <= multi->GetNbinsX() ; ++k){
    kno->SetBinContent(k , multi->GetMean() * multi->GetBinContent(k));
    kno->SetBinError(k , multi->GetMean() * multi->GetBinError(k));
  }
}

TH1F resample(double matrix[][matrixsize], TH1F* toUnfold, TH1F* hyp, int niter, bool doFit ){

  const int nresampling = 1000;
  
  TH1F sample[nresampling];

  TH1F bins[matrixsize];// = new vector<TH1F>;
  char* name = "bin_%d";
  char name2[60];
  for(int k=1;k<=toUnfold->GetNbinsX();++k){
    sprintf(name2,name,k);
    //cout<<name2<<endl;
    //cout<<bins.size()<<endl;
    //TH1F temp = (TH1F) toUnfold->Clone(name2);
    //(name2,name2,71,-0.5,70.5);
    //temp.SetLineColor(kRed);
    //temp.Draw();
    //gPad->WaitPrimitive();
    //if(temp==0) cout<<"pb"<<endl;
    if(toUnfold->Integral()<2)
      bins[k-1] = TH1F(name2,name2,1000,0.,1.);
    else
      bins[k-1] = TH1F(name2,name2,(int)toUnfold->GetMaximum()+501,-0.5,(int)toUnfold->GetMaximum()+500.5);
    //cout<<bins.size()<<"  "<<toUnfold->GetNbinsX()<<endl;
    //delete temp;
  }
  
  
  TH1F* out = (TH1F*) toUnfold->Clone("nch_resampled");

  TF1*Gauss = new TF1("Gauss","gaus");

  for(int i=0;i<nresampling;++i){
    char tmp[160] = "sample_nsample%d";
    char samplechar[160] = "";
    sprintf(samplechar,tmp,i+1);
    //cout<<samplechar<<endl;
    TH1F* nch_temp = new TH1F(samplechar,samplechar,matrixsize,-0.5,matrixsize-0.5);
    for(int n=1;n<=matrixsize;++n){
      nch_temp->SetBinContent(n,gRandom->Poisson(toUnfold->GetBinContent(n)));
    }
    //TCanvas* c_kno = new TCanvas("c_kno","c_kno",200,510,500,500);
    //nch_temp->Draw();

    if(hyp==0) cout<<"PROBLEM -----> the hypothesis is void !!"<<endl;
    sample[i] = (runalgo(matrix,nch_temp,hyp,niter,i+1));
    //sample[i].SetLineColor(kRed);
    //sample[i].Draw("same");
  
    for(int k=1;k<=matrixsize;++k){
      bins[k-1].Fill(sample[i].GetBinContent(k));
      //bins[k-1].SetLineColor(kBlue);
      //bins[k-1].Draw("same");
    }  
      
    //gPad->Update();
    //gPad->WaitPrimitive();
  }
  //TCanvas* c_kno = new TCanvas("c_kno","c_kno",200,510,500,500);
  for(int k=1;k<=matrixsize;++k){
    if(doFit){
      bins[k-1].Fit("Gauss","Q");
      out->SetBinContent(k,Gauss->GetParameter(1));
      out->SetBinError(k,Gauss->GetParameter(2));
    }
    else {
      out->SetBinContent(k,bins[k-1].GetMean());
      out->SetBinError(k,bins[k-1].GetRMS());
    }
    //bins[k-1].Draw();
    //gPad->Update();
    //gPad->WaitPrimitive();   
  }
  
  
  //out->Draw();
  //gPad->WaitPrimitive();
  return *out;
}


