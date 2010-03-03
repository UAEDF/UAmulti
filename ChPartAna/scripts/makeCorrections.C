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
TH1F resample(double matrix[][matrixsize], TH1F* toUnfold, TH1F* hyp, int niter = 5);

//iFileType,iDataType,Energy,iTracking,iSystType,iSystSign,STest
#include "../macro/fileManager.C"
#include "unfolding.cc"

void makeCorrections(bool isData = false, int hyp=0 , int niter=5 , int acc = 0 , float mu = 14 , float sigma = 15 ){

   useData = isData;
   #include "../macro/acceptanceMap.C"
  //Get the MC file
  //TFile* mc = new TFile("/user/rougny/Ferenc_Tracking_bis/CMSSW_3_3_6_patch3/src/UAmulti/ChPartAna/macro/collectionPlotter_MC_finalv2_900GeV.root","READ");
  //TFile* mc = new TFile("../macro/GOODFILES/MC_v005b_2.36TeV_eta2.5_pt0.4_gTr.root","READ");
  //TFile* mc = new TFile("../macro/GOODFILES/MC_v005b_0.9TeV_eta2.5_pt0.4_gTr.root","READ");
  TFile* mc = new TFile(fileManager(2,10,0.9,0),"READ");
  
  //Get the data file
  //TFile* data = new TFile("../macro/GOODFILES/data_v005b_2.36TeV_eta2.5_pt0.4_gTr.root","READ");
  //TFile* data = new TFile("../macro/GOODFILES/data_v005b_0.9TeV_eta2.5_pt0.4_gTr.root","READ");
  TFile* data = new TFile(fileManager(2,0,0.9,0),"READ");
  
  ostringstream dirstr("");
  dirstr << "/ptGen" << accMap.at(acc).at(0) << "_etaGen" << accMap.at(acc).at(1) 
      << "_ptReco" << accMap.at(acc).at(2) << "_etaReco" << accMap.at(acc).at(3) ;
  /*if(accMap.at(acc).at(4)==-1)
    dirstr << "_chargemin";
  else if(accMap.at(acc).at(4)==1)
    dirstr << "_chargeplus";*/
  dirstr << "/";
  TString dir = dirstr.str();
  //cout<<dir<<endl;
  //return;
  
  //------------------------------------------------------------------------------
  //---------------------------- UNFOLDING ---------------------------------------
  //------------------------------------------------------------------------------
  
  //Get the Unfolding matrix
  //TFile* mc_matrix = new TFile("../macro/GOODFILES/MC_v005b_2.36TeV_eta2.5_pt0.4_gTr.root");
  //TH2F* matrixhist = (TH2F*) mc_matrix->Get("MatrixPlots_etaGenCut_L1_hf_VtxSel_PV_gTr_oVtx/nch_matrix_etaGenCut_L1_hf_VtxSel_PV_gTr_oVtx");
  TH2F* matrixhist = (TH2F*) mc->Get(dir+st("MatrixPlots_evtSel",acc)+st("/nch_matrix_evtSel",acc));
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
  TH1F* nch_trueGen_afterUnfolding = (TH1F*) mc->Get(dir+st("MatrixPlots_evtSel",acc)+st("/nch_gen_evtSel",acc));
  //TH1F* nch_trueGen = (TH1F*) mc->Get("MatrixPlots_etaGenCut_L1_hf_VtxSel_PV_gTr_oVtx/nch_gen_etaGenCut_L1_hf_VtxSel_PV_gTr_oVtx");
  TH1F* nch_trueGen = (TH1F*) mc->Get(dir+st("MultiPlots_etaCut_noSel_NSD_gen",acc)+st("/nch_etaCut_noSel_NSD_gen",acc));
  
  //nch_trueGen->Scale(1./nch_trueGen->GetEntries());
  //nch_trueGen->Draw();
  //gPad->WaitPrimitive();
  
  //get the nch to Unfold distribution
  TH1F* nch_INC;
  ////  BAD ?//if(!useData) nch_INC = (TH1F*) mc->Get("MultiPlots_L1_hf_VtxSel_PV_gTr_oVtx/nch_L1_hf_VtxSel_PV_gTr_oVtx");
  if(!useData)
    nch_INC = (TH1F*) mc->Get(dir+st("GenMultiPlots_evtSel_reco",acc)+st("/MultiPlots_INC_evtSel_reco",acc)+st("/nch_INC_evtSel_reco",acc));
  //if(!useData) nch_INC = (TH1F*) mc->Get("MatrixPlots_etaGenCut_L1_hf_VtxSel_PV_gTr_oVtx/nch_reco_etaGenCut_L1_hf_VtxSel_PV_gTr_oVtx");
  else
    nch_INC = (TH1F*) data->Get(dir+st("MultiPlots_evtSel_PV",acc)+st("/nch_evtSel_PV",acc));
  //else         nch_INC = (TH1F*) data->Get("MultiPlots_L1_hf_VtxSel_PV_gTr_oVtx/nch_L1_hf_VtxSel_PV_gTr_oVtx");
  
  //nch_INC->SetBinContent(1,0);
  
  
  //Getting the class
  MultiPlots* mp_INC_evtSel_reco_MC;
  MultiPlots* mp_INC_evtSel_reco_data;
  mp_INC_evtSel_reco_MC = (MultiPlots*) mc->Get(dir+st("GenMultiPlots_evtSel_reco",acc)+st("/MultiPlots_SD_evtSel_reco",acc)+st("/multi_class_SD_evtSel_reco",acc));
  if(useData) mp_INC_evtSel_reco_data = (MultiPlots*) data->Get(dir+st("MultiPlots_evtSel_INC_reco",acc)+st("/multi_class_evtSel_INC_reco",acc));
  else mp_INC_evtSel_reco_data = (MultiPlots*) mc->Get(dir+st("MultiPlots_evtSel_INC_reco",acc)+st("/multi_class_evtSel_INC_reco",acc));
  
  double MC_factor = double(mp_INC_evtSel_reco_data->nbEvts) / double(mp_INC_evtSel_reco_MC->nbEvts) ;
  cout<<"The ratio data_INC/MC_INC is "<<MC_factor<<endl;
 
   //if( useData ) {
    //TH1F* nch_trueRec = (TH1F*) mc->Get("MatrixPlots_etaGenCut_L1_hf_VtxSel_PV_gTr_oVtx/nch_reco_etaGenCut_L1_hf_VtxSel_PV_gTr_oVtx");
     
     //nch_trueRec->Draw("same");
   //} 
   
   
   
  //------------------------------------------------------------------------------
  //---------------------------- SD Substraction ---------------------------------
  //------------------------------------------------------------------------------
  
  TCanvas* c_SDsub = new TCanvas("c_SDsub","c_SDsub",2350,510,500,500);
  nch_INC->Draw();
  
  TH1F* nch_evtSel_SD =  (TH1F*) mc->Get(dir+st("GenMultiPlots_evtSel_reco",acc)+st("/MultiPlots_SD_evtSel_reco",acc)+st("/nch_SD_evtSel_reco",acc));
  TH1F* nch_evtSel_NSD = (TH1F*) mc->Get(dir+st("GenMultiPlots_evtSel_reco",acc)+st("/MultiPlots_NSD_evtSel_reco",acc)+st("/nch_NSD_evtSel_reco",acc));
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
  
  TH1F* eff_evtSel = (TH1F*) mc->Get(dir+st("eff_evtSel",acc));
  TH1F* nch_toUnfold = (TH1F*) nch_NSD->Clone("nch_toUnfold");
  //nch_toUnfold->Divide(eff_evtSel);
  
  //TEMP ----- TO CHANGE !!!
  //Opening the output file
  cout<<fileManager(3,10,0.9,0)<<endl;
  TFile* out = new TFile(fileManager(3,10,0.9,0),"RECREATE");
  out->cd();
  
  
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
 
  nch_unfolded = (runalgo(matrix,nch_toUnfold,hypothesis,niter));
  TH1F* nch_unfoldedPtr = (TH1F*) nch_unfolded.Clone("nch_unfoldedPtr");
  
  
  //------------------------------------------------------------------------------
  //---------------------------- Resampling -------------------------------------
  //------------------------------------------------------------------------------
  
  TH1F nch_resampled = resample(matrix,nch_toUnfold,hypothesis,niter);
  TH1F* nch_resampledPtr = &nch_resampled;
  
  TCanvas* c_resample = new TCanvas("resampling","resampling",810,510,500,500);
  nch_unfoldedPtr->SetLineColor(kBlack);
  //nch_unfoldedPtr->Draw("hist");
  nch_resampledPtr->Draw("e");
  //gPad->WaitPrimitive();
  gPad->Update();
  
  for(int nbin = 1 ; nbin<=nch_unfoldedPtr->GetNbinsX() ; ++nbin)
    nch_unfoldedPtr->SetBinError(nbin , nch_resampledPtr->GetBinError(nbin));
  
  //------------------------------------------------------------------------------
  //---------------------------- EvtSel Correction -------------------------------------
  //------------------------------------------------------------------------------
  
  TH1F* nch_corrected = (TH1F*) nch_unfolded.Clone("nch_corrected");
  //eff_evtSel->Sumw2();
  nch_corrected->Divide(eff_evtSel);
  
  nch_trueGen->Scale(MC_factor);
  nch_trueGen_afterUnfolding->Scale(MC_factor);
  
  TCanvas* c_effcorr = new TCanvas("c_effcorr","c_effcorr",1800,510,500,500);
  nch_trueGen->Draw("hist");
  nch_corrected->Draw("same e");
  nch_unfoldedPtr->SetLineColor(kGreen);
  nch_unfoldedPtr->Draw("same e");
  nch_trueGen_afterUnfolding->Draw("same hist");
  
  leg = new TLegend (.65,.70,.90,.99);
  leg->AddEntry(nch_trueGen,"MC after Correction","l" );
  leg->AddEntry(nch_corrected,"Data after Correction","l" );
  leg->AddEntry(nch_trueGen_afterUnfolding,"MC after Unfolding","l" );
  leg->AddEntry(nch_unfoldedPtr,"Data after Unfolding","l" );
  leg->Draw("same");
  
  cout<<"Mean of multiplicity --------> "<<nch_corrected->GetMean()<<endl;
  cout<<"RMS of multiplicity  --------> "<<nch_corrected->GetRMS()<<endl;
  
  
  
  
  
  
  
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
 

  leg = new TLegend (.65,.90,.90,.99);
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

  //eff_nch_L1_hf_vtxSel->Write();
  kno->Write();

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

void makeKNO(TH1F* multi,TH1F* kno){
  for( int k = 1 ; k <= multi->GetNbinsX() ; ++k){
    kno->SetBinContent(k , multi->GetMean() * multi->GetBinContent(k));
    kno->SetBinError(k , multi->GetMean() * multi->GetBinError(k));
  }
}

TH1F resample(double matrix[][matrixsize], TH1F* toUnfold, TH1F* hyp, int niter){

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
    bins[k-1] = TH1F(name2,name2,10000,-0.5,9999.5);
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
    bins[k-1].Fit("Gauss","Q");
    bins[k-1].Draw();
    //gPad->Update();
    //gPad->WaitPrimitive();
    out->SetBinContent(k,Gauss->GetParameter(1));
    out->SetBinError(k,Gauss->GetParameter(2));    
  }
  
  
  //out->Draw();
  //gPad->WaitPrimitive();
  return *out;
}

