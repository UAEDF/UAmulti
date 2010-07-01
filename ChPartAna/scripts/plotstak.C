#include <TROOT.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TGraphErrors.h>
#include <TGraphAsymmErrors.h>
#include <TString.h>
#include <TProfile.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TFrame.h>
#include <TFile.h>
#include <TDirectory.h>
#include <TLegend.h>
#include <TText.h>
#include <iostream>
#include <fstream>
#include <TMath.h>
#include <TSystem.h>
using namespace std;

void plotstak ( int Energy , float npx = 1000 , float npy = 600 , int iBin = 5 , bool MCUnf = false) {


  TCanvas* c1 = new TCanvas("c1","c",200,10,npx,npy);
  c1->SetLeftMargin(0.10);
  c1->SetBottomMargin(0.10);
  c1->SetFillColor(0);
  c1->GetFrame()->SetFillColor(21);
  c1->GetFrame()->SetBorderSize(12);
  c1->SetGrid(0,0);



  c1->SetLogy(true);


  TFile* fData1 = NULL ;
  TFile* fData2 = NULL ;
  TFile* fData3 = NULL ;
  TFile* fData4 = NULL ;
  TFile* fData5 = NULL ;

  
  if ( iBin == 5 ) {
    if        ( Energy == 1  ) {
      if (MCUnf) {
        fData1 = new TFile("../plots/finalv9/unfolding_MC_D6T_0.9TeV_mbTr__hyp1_niter0_cut5_DataType10.root","READ");
        fData2 = new TFile("../plots/finalv9/unfolding_MC_D6T_0.9TeV_mbTr__hyp1_niter0_cut6_DataType10.root","READ");
        fData3 = new TFile("../plots/finalv9/unfolding_MC_D6T_0.9TeV_mbTr__hyp1_niter0_cut7_DataType10.root","READ");
        fData4 = new TFile("../plots/finalv9/unfolding_MC_D6T_0.9TeV_mbTr__hyp1_niter0_cut8_DataType10.root","READ");
        fData5 = new TFile("../plots/finalv9/unfolding_MC_D6T_0.9TeV_mbTr__hyp1_niter0_cut9_DataType10.root","READ");
      } else {
        fData1 = new TFile("../plots/finalv9/unfolding_MC_D6T_0.9TeV_mbTr__hyp1_niter0_cut5_DataType0.root","READ");
        fData2 = new TFile("../plots/finalv9/unfolding_MC_D6T_0.9TeV_mbTr__hyp1_niter0_cut6_DataType0.root","READ");
        fData3 = new TFile("../plots/finalv9/unfolding_MC_D6T_0.9TeV_mbTr__hyp1_niter0_cut7_DataType0.root","READ");
        fData4 = new TFile("../plots/finalv9/unfolding_MC_D6T_0.9TeV_mbTr__hyp1_niter0_cut8_DataType0.root","READ");
        fData5 = new TFile("../plots/finalv9/unfolding_MC_D6T_0.9TeV_mbTr__hyp1_niter0_cut9_DataType0.root","READ");
      }
    } else if ( Energy == 2 ) {
      if (MCUnf) {
        fData1 = new TFile("../plots/finalv9/unfolding_MC_D6T_2.36TeV_mbTr__hyp1_niter0_cut5_DataType10.root","READ");
        fData2 = new TFile("../plots/finalv9/unfolding_MC_D6T_2.36TeV_mbTr__hyp1_niter0_cut6_DataType10.root","READ");
        fData3 = new TFile("../plots/finalv9/unfolding_MC_D6T_2.36TeV_mbTr__hyp1_niter0_cut7_DataType10.root","READ");
        fData4 = new TFile("../plots/finalv9/unfolding_MC_D6T_2.36TeV_mbTr__hyp1_niter0_cut8_DataType10.root","READ");
        fData5 = new TFile("../plots/finalv9/unfolding_MC_D6T_2.36TeV_mbTr__hyp1_niter0_cut9_DataType10.root","READ");
      } else {
        fData1 = new TFile("../plots/finalv9/unfolding_MC_D6T_2.36TeV_mbTr__hyp1_niter0_cut5_DataType0.root","READ");
        fData2 = new TFile("../plots/finalv9/unfolding_MC_D6T_2.36TeV_mbTr__hyp1_niter0_cut6_DataType0.root","READ");
        fData3 = new TFile("../plots/finalv9/unfolding_MC_D6T_2.36TeV_mbTr__hyp1_niter0_cut7_DataType0.root","READ");
        fData4 = new TFile("../plots/finalv9/unfolding_MC_D6T_2.36TeV_mbTr__hyp1_niter0_cut8_DataType0.root","READ");
        fData5 = new TFile("../plots/finalv9/unfolding_MC_D6T_2.36TeV_mbTr__hyp1_niter0_cut9_DataType0.root","READ");
      }
    } else if ( Energy == 3 ) {
      if (MCUnf) {
        fData1 = new TFile("../plots/systv10/unfolding_MC_ATLAS_7.0TeV_mbTr__hyp1_niter0_cut5_DataType31.root","READ");
        fData2 = new TFile("../plots/systv10/unfolding_MC_ATLAS_7.0TeV_mbTr__hyp1_niter0_cut6_DataType31.root","READ");
        fData3 = new TFile("../plots/systv10/unfolding_MC_ATLAS_7.0TeV_mbTr__hyp1_niter0_cut7_DataType31.root","READ");
        fData4 = new TFile("../plots/systv10/unfolding_MC_ATLAS_7.0TeV_mbTr__hyp1_niter0_cut8_DataType31.root","READ");
        fData5 = new TFile("../plots/systv10/unfolding_MC_ATLAS_7.0TeV_mbTr__hyp1_niter0_cut9_DataType31.root","READ");
      } else {
        fData1 = new TFile("../plots/systv10/unfolding_MC_ATLAS_7.0TeV_mbTr__hyp1_niter0_cut5_DataType0.root","READ");
        fData2 = new TFile("../plots/systv10/unfolding_MC_ATLAS_7.0TeV_mbTr__hyp1_niter0_cut6_DataType0.root","READ");
        fData3 = new TFile("../plots/systv10/unfolding_MC_ATLAS_7.0TeV_mbTr__hyp1_niter0_cut7_DataType0.root","READ");
        fData4 = new TFile("../plots/systv10/unfolding_MC_ATLAS_7.0TeV_mbTr__hyp1_niter0_cut8_DataType0.root","READ");
        fData5 = new TFile("../plots/systv10/unfolding_MC_ATLAS_7.0TeV_mbTr__hyp1_niter0_cut9_DataType0.root","READ");
      }
    }

  } else if ( iBin == 15 ) {

   if        ( Energy == 1  ) {
      if (MCUnf) {
        fData1 = new TFile("../plots/finalv9/unfolding_MC_D6T_0.9TeV_mbTr__hyp1_niter0_cut15_DataType10.root","READ");
        fData2 = new TFile("../plots/finalv9/unfolding_MC_D6T_0.9TeV_mbTr__hyp1_niter0_cut16_DataType10.root","READ");
        fData3 = new TFile("../plots/finalv9/unfolding_MC_D6T_0.9TeV_mbTr__hyp1_niter0_cut17_DataType10.root","READ");
        fData4 = new TFile("../plots/finalv9/unfolding_MC_D6T_0.9TeV_mbTr__hyp1_niter0_cut18_DataType10.root","READ");
        fData5 = new TFile("../plots/finalv9/unfolding_MC_D6T_0.9TeV_mbTr__hyp1_niter0_cut19_DataType10.root","READ");
      } else {
        fData1 = new TFile("../plots/finalv9/unfolding_MC_D6T_0.9TeV_mbTr__hyp1_niter0_cut15_DataType0.root","READ");
        fData2 = new TFile("../plots/finalv9/unfolding_MC_D6T_0.9TeV_mbTr__hyp1_niter0_cut16_DataType0.root","READ");
        fData3 = new TFile("../plots/finalv9/unfolding_MC_D6T_0.9TeV_mbTr__hyp1_niter0_cut17_DataType0.root","READ");
        fData4 = new TFile("../plots/finalv9/unfolding_MC_D6T_0.9TeV_mbTr__hyp1_niter0_cut18_DataType0.root","READ");
        fData5 = new TFile("../plots/finalv9/unfolding_MC_D6T_0.9TeV_mbTr__hyp1_niter0_cut19_DataType0.root","READ");
      }
    } else if ( Energy == 2 ) {
      if (MCUnf) {
        fData1 = new TFile("../plots/finalv9/unfolding_MC_D6T_2.36TeV_mbTr__hyp1_niter0_cut15_DataType10.root","READ");
        fData2 = new TFile("../plots/finalv9/unfolding_MC_D6T_2.36TeV_mbTr__hyp1_niter0_cut16_DataType10.root","READ");
        fData3 = new TFile("../plots/finalv9/unfolding_MC_D6T_2.36TeV_mbTr__hyp1_niter0_cut17_DataType10.root","READ");
        fData4 = new TFile("../plots/finalv9/unfolding_MC_D6T_2.36TeV_mbTr__hyp1_niter0_cut18_DataType10.root","READ");
        fData5 = new TFile("../plots/finalv9/unfolding_MC_D6T_2.36TeV_mbTr__hyp1_niter0_cut19_DataType10.root","READ");
      } else {
        fData1 = new TFile("../plots/finalv9/unfolding_MC_D6T_2.36TeV_mbTr__hyp1_niter0_cut15_DataType0.root","READ");
        fData2 = new TFile("../plots/finalv9/unfolding_MC_D6T_2.36TeV_mbTr__hyp1_niter0_cut16_DataType0.root","READ");
        fData3 = new TFile("../plots/finalv9/unfolding_MC_D6T_2.36TeV_mbTr__hyp1_niter0_cut17_DataType0.root","READ");
        fData4 = new TFile("../plots/finalv9/unfolding_MC_D6T_2.36TeV_mbTr__hyp1_niter0_cut18_DataType0.root","READ");
        fData5 = new TFile("../plots/finalv9/unfolding_MC_D6T_2.36TeV_mbTr__hyp1_niter0_cut19_DataType0.root","READ");
      }
    } else if ( Energy == 3 ) {
      if (MCUnf) {
        fData1 = new TFile("../plots/finalv9/unfolding_MC_ATLAS_7.0TeV_mbTr__hyp1_niter0_cut15_DataType31.root","READ");
        fData2 = new TFile("../plots/finalv9/unfolding_MC_ATLAS_7.0TeV_mbTr__hyp1_niter0_cut16_DataType31.root","READ");
        fData3 = new TFile("../plots/finalv9/unfolding_MC_ATLAS_7.0TeV_mbTr__hyp1_niter0_cut17_DataType31.root","READ");
        fData4 = new TFile("../plots/finalv9/unfolding_MC_ATLAS_7.0TeV_mbTr__hyp1_niter0_cut18_DataType31.root","READ");
        fData5 = new TFile("../plots/finalv9/unfolding_MC_ATLAS_7.0TeV_mbTr__hyp1_niter0_cut19_DataType31.root","READ");
      } else {
        fData1 = new TFile("../plots/finalv9/unfolding_MC_ATLAS_7.0TeV_mbTr__hyp1_niter0_cut15_DataType0.root","READ");
        fData2 = new TFile("../plots/finalv9/unfolding_MC_ATLAS_7.0TeV_mbTr__hyp1_niter0_cut16_DataType0.root","READ");
        fData3 = new TFile("../plots/finalv9/unfolding_MC_ATLAS_7.0TeV_mbTr__hyp1_niter0_cut17_DataType0.root","READ");
        fData4 = new TFile("../plots/finalv9/unfolding_MC_ATLAS_7.0TeV_mbTr__hyp1_niter0_cut18_DataType0.root","READ");
        fData5 = new TFile("../plots/finalv9/unfolding_MC_ATLAS_7.0TeV_mbTr__hyp1_niter0_cut19_DataType0.root","READ");
      }
    }

  } else {
    cout << "Bin not available: " << iBin << endl;
    return;
  }
  
  fData1->cd();
  TH1F* hData1 = (TH1F*) fData1->Get("unfolding/nch_data_corrected");
  TH1F* hmoca1 = (TH1F*) fData1->Get("unfolding/nch_MC_gen_afterEvtSelCorrection");
  fData2->cd();
  TH1F* hData2 = (TH1F*) fData2->Get("unfolding/nch_data_corrected");
  TH1F* hmoca2 = (TH1F*) fData2->Get("unfolding/nch_MC_gen_afterEvtSelCorrection");
  fData3->cd();
  TH1F* hData3 = (TH1F*) fData3->Get("unfolding/nch_data_corrected");
  TH1F* hmoca3 = (TH1F*) fData3->Get("unfolding/nch_MC_gen_afterEvtSelCorrection");
  fData4->cd();
  TH1F* hData4 = (TH1F*) fData4->Get("unfolding/nch_data_corrected");
  TH1F* hmoca4 = (TH1F*) fData4->Get("unfolding/nch_MC_gen_afterEvtSelCorrection");
  fData5->cd();
  TH1F* hData5 = (TH1F*) fData5->Get("unfolding/nch_data_corrected");
  TH1F* hmoca5 = (TH1F*) fData5->Get("unfolding/nch_MC_gen_afterEvtSelCorrection");

  TString gnchsyst ("unfolding/gnch_data_corrected_syst");
  if ( MCUnf ) gnchsyst = "unfolding/gnch_data_corrected";
 
  fData1->cd();
  TGraphAsymmErrors* gr1 = (TGraphAsymmErrors*) fData1->Get("unfolding/gnch_data_corrected");
  TGraphAsymmErrors* gs1 = (TGraphAsymmErrors*) fData1->Get(gnchsyst);
  fData2->cd();
  TGraphAsymmErrors* gr2 = (TGraphAsymmErrors*) fData2->Get("unfolding/gnch_data_corrected");
  TGraphAsymmErrors* gs2 = (TGraphAsymmErrors*) fData2->Get(gnchsyst);
  fData3->cd();
  TGraphAsymmErrors* gr3 = (TGraphAsymmErrors*) fData3->Get("unfolding/gnch_data_corrected");
  TGraphAsymmErrors* gs3 = (TGraphAsymmErrors*) fData3->Get(gnchsyst);
  fData4->cd();
  TGraphAsymmErrors* gr4 = (TGraphAsymmErrors*) fData4->Get("unfolding/gnch_data_corrected");
  TGraphAsymmErrors* gs4 = (TGraphAsymmErrors*) fData4->Get(gnchsyst);
  fData5->cd();
  TGraphAsymmErrors* gr5 = (TGraphAsymmErrors*) fData5->Get("unfolding/gnch_data_corrected");
  TGraphAsymmErrors* gs5 = (TGraphAsymmErrors*) fData5->Get(gnchsyst);


  Float_t ndata1 = hData1->Integral(1, hData1->GetNbinsX() ); 
  Float_t ndata2 = hData2->Integral(1, hData2->GetNbinsX() ); 
  Float_t ndata3 = hData3->Integral(1, hData3->GetNbinsX() ); 
  Float_t ndata4 = hData4->Integral(1, hData4->GetNbinsX() ); 
  Float_t ndata5 = hData5->Integral(1, hData5->GetNbinsX() ); 

  TH1F* hDataSc1 = (TH1F*) hData1->Clone("hDataSc1");
  TH1F* hDataSc2 = (TH1F*) hData2->Clone("hDataSc2");
  TH1F* hDataSc3 = (TH1F*) hData3->Clone("hDataSc3");
  TH1F* hDataSc4 = (TH1F*) hData4->Clone("hDataSc4");
  TH1F* hDataSc5 = (TH1F*) hData5->Clone("hDataSc5");

  hDataSc1->Scale(1/ndata1,"width");
  hDataSc2->Scale(1/ndata2,"width");
  hDataSc3->Scale(1/ndata3,"width");
  hDataSc4->Scale(1/ndata4,"width");
  hDataSc5->Scale(1/ndata5,"width");

  hDataSc1->Scale(10000,"width");
  hDataSc2->Scale(1000,"width");
  hDataSc3->Scale(100,"width");
  hDataSc4->Scale(10,"width");
  hDataSc5->Scale(1,"width");

  Float_t nmoca1 = hmoca1->Integral(1, hmoca1->GetNbinsX() );
  Float_t nmoca2 = hmoca2->Integral(1, hmoca2->GetNbinsX() );
  Float_t nmoca3 = hmoca3->Integral(1, hmoca3->GetNbinsX() );
  Float_t nmoca4 = hmoca4->Integral(1, hmoca4->GetNbinsX() );
  Float_t nmoca5 = hmoca5->Integral(1, hmoca5->GetNbinsX() );

  TH1F* hmocaSc1 = (TH1F*) hmoca1->Clone("hmocaSc1");
  TH1F* hmocaSc2 = (TH1F*) hmoca2->Clone("hmocaSc2");
  TH1F* hmocaSc3 = (TH1F*) hmoca3->Clone("hmocaSc3");
  TH1F* hmocaSc4 = (TH1F*) hmoca4->Clone("hmocaSc4");
  TH1F* hmocaSc5 = (TH1F*) hmoca5->Clone("hmocaSc5");

  hmocaSc1->Scale(1/nmoca1,"width");
  hmocaSc2->Scale(1/nmoca2,"width");
  hmocaSc3->Scale(1/nmoca3,"width");
  hmocaSc4->Scale(1/nmoca4,"width");
  hmocaSc5->Scale(1/nmoca5,"width");

  hmocaSc1->Scale(10000,"width");
  hmocaSc2->Scale(1000,"width");
  hmocaSc3->Scale(100,"width");
  hmocaSc4->Scale(10,"width");
  hmocaSc5->Scale(1,"width");



  Double_t* ScF1 = new Double_t[hData1->GetNbinsX()];
  Double_t* ScF2 = new Double_t[hData2->GetNbinsX()];
  Double_t* ScF3 = new Double_t[hData3->GetNbinsX()];
  Double_t* ScF4 = new Double_t[hData4->GetNbinsX()];
  Double_t* ScF5 = new Double_t[hData5->GetNbinsX()];

  for(int i = 1 ; i <= hData1->GetNbinsX() ; ++i) ScF1[i-1] = hDataSc1->GetBinContent(i) / hData1->GetBinContent(i) ;
  for(int i = 1 ; i <= hData2->GetNbinsX() ; ++i) ScF2[i-1] = hDataSc2->GetBinContent(i) / hData2->GetBinContent(i) ;
  for(int i = 1 ; i <= hData3->GetNbinsX() ; ++i) ScF3[i-1] = hDataSc3->GetBinContent(i) / hData3->GetBinContent(i) ;
  for(int i = 1 ; i <= hData4->GetNbinsX() ; ++i) ScF4[i-1] = hDataSc4->GetBinContent(i) / hData4->GetBinContent(i) ;
  for(int i = 1 ; i <= hData5->GetNbinsX() ; ++i) ScF5[i-1] = hDataSc5->GetBinContent(i) / hData5->GetBinContent(i) ;

  Double_t* x1   = new Double_t[hData1->GetNbinsX()];
  Double_t* y1   = new Double_t[hData1->GetNbinsX()];
  Double_t* exl1 = new Double_t[hData1->GetNbinsX()];
  Double_t* exh1 = new Double_t[hData1->GetNbinsX()];
  Double_t* eyl1 = new Double_t[hData1->GetNbinsX()];
  Double_t* eyh1 = new Double_t[hData1->GetNbinsX()];
  for(int i = 0 ; i < hData1->GetNbinsX() ; ++i) {
    gr1->GetPoint(i,x1[i],y1[i]);
    eyh1[i] = gr1->GetErrorYhigh(i); 
    eyl1[i] = gr1->GetErrorYlow(i); 
    exh1[i] = gr1->GetErrorXhigh(i);
    exl1[i] = gr1->GetErrorXlow(i);
    y1[i]   *= ScF1[i];
    eyl1[i] *= ScF1[i];
    eyh1[i] *= ScF1[i];
  }
  TGraphAsymmErrors* grSc1 = new TGraphAsymmErrors(hData1->GetNbinsX(),x1,y1,exl1,exh1,eyl1,eyh1);
  for(int i = 0 ; i < hData1->GetNbinsX() ; ++i) {
    gs1->GetPoint(i,x1[i],y1[i]);
    eyh1[i] = gs1->GetErrorYhigh(i); 
    eyl1[i] = gs1->GetErrorYlow(i); 
    exh1[i] = gs1->GetErrorXhigh(i);
    exl1[i] = gs1->GetErrorXlow(i);
    y1[i]   *= ScF1[i];
    eyl1[i] *= ScF1[i];
    eyh1[i] *= ScF1[i];
  }
  TGraphAsymmErrors* gsSc1 = new TGraphAsymmErrors(hData1->GetNbinsX(),x1,y1,exl1,exh1,eyl1,eyh1);

  Double_t* x2   = new Double_t[hData2->GetNbinsX()];
  Double_t* y2   = new Double_t[hData2->GetNbinsX()];
  Double_t* exl2 = new Double_t[hData2->GetNbinsX()];
  Double_t* exh2 = new Double_t[hData2->GetNbinsX()];
  Double_t* eyl2 = new Double_t[hData2->GetNbinsX()];
  Double_t* eyh2 = new Double_t[hData2->GetNbinsX()];
  for(int i = 0 ; i < hData2->GetNbinsX() ; ++i) {
    gr2->GetPoint(i,x2[i],y2[i]);
    eyh2[i] = gr2->GetErrorYhigh(i);    
    eyl2[i] = gr2->GetErrorYlow(i);    
    exh2[i] = gr2->GetErrorXhigh(i);
    exl2[i] = gr2->GetErrorXlow(i);
    y2[i]   *= ScF2[i];
    eyl2[i] *= ScF2[i];
    eyh2[i] *= ScF2[i];
  }
  TGraphAsymmErrors* grSc2 = new TGraphAsymmErrors(hData2->GetNbinsX(),x2,y2,exl2,exh2,eyl2,eyh2);
  for(int i = 0 ; i < hData2->GetNbinsX() ; ++i) {
    gs2->GetPoint(i,x2[i],y2[i]);
    eyh2[i] = gs2->GetErrorYhigh(i);    
    eyl2[i] = gs2->GetErrorYlow(i);    
    exh2[i] = gs2->GetErrorXhigh(i);
    exl2[i] = gs2->GetErrorXlow(i);
    y2[i]   *= ScF2[i];
    eyl2[i] *= ScF2[i];
    eyh2[i] *= ScF2[i];
  }
  TGraphAsymmErrors* gsSc2 = new TGraphAsymmErrors(hData2->GetNbinsX(),x2,y2,exl2,exh2,eyl2,eyh2);


  Double_t* x3   = new Double_t[hData3->GetNbinsX()];
  Double_t* y3   = new Double_t[hData3->GetNbinsX()];
  Double_t* exl3 = new Double_t[hData3->GetNbinsX()];
  Double_t* exh3 = new Double_t[hData3->GetNbinsX()];
  Double_t* eyl3 = new Double_t[hData3->GetNbinsX()];
  Double_t* eyh3 = new Double_t[hData3->GetNbinsX()];
  for(int i = 0 ; i < hData3->GetNbinsX() ; ++i) {
    gr3->GetPoint(i,x3[i],y3[i]);
    eyh3[i] = gr3->GetErrorYhigh(i);    
    eyl3[i] = gr3->GetErrorYlow(i);    
    exh3[i] = gr3->GetErrorXhigh(i);
    exl3[i] = gr3->GetErrorXlow(i);
    y3[i]   *= ScF3[i];
    eyl3[i] *= ScF3[i];
    eyh3[i] *= ScF3[i];
  }
  TGraphAsymmErrors* grSc3 = new TGraphAsymmErrors(hData3->GetNbinsX(),x3,y3,exl3,exh3,eyl3,eyh3);
  for(int i = 0 ; i < hData3->GetNbinsX() ; ++i) {
    gs3->GetPoint(i,x3[i],y3[i]);
    eyh3[i] = gs3->GetErrorYhigh(i);    
    eyl3[i] = gs3->GetErrorYlow(i);    
    exh3[i] = gs3->GetErrorXhigh(i);
    exl3[i] = gs3->GetErrorXlow(i);
    y3[i]   *= ScF3[i];
    eyl3[i] *= ScF3[i];
    eyh3[i] *= ScF3[i];
  }
  TGraphAsymmErrors* gsSc3 = new TGraphAsymmErrors(hData3->GetNbinsX(),x3,y3,exl3,exh3,eyl3,eyh3);

  Double_t* x4   = new Double_t[hData4->GetNbinsX()];
  Double_t* y4   = new Double_t[hData4->GetNbinsX()];
  Double_t* exl4 = new Double_t[hData4->GetNbinsX()];
  Double_t* exh4 = new Double_t[hData4->GetNbinsX()];
  Double_t* eyl4 = new Double_t[hData4->GetNbinsX()];
  Double_t* eyh4 = new Double_t[hData4->GetNbinsX()];
  for(int i = 0 ; i < hData4->GetNbinsX() ; ++i) {
    gr4->GetPoint(i,x4[i],y4[i]);
    eyh4[i] = gr4->GetErrorYhigh(i);    
    eyl4[i] = gr4->GetErrorYlow(i);    
    exh4[i] = gr4->GetErrorXhigh(i);
    exl4[i] = gr4->GetErrorXlow(i);
    y4[i]   *= ScF4[i];
    eyl4[i] *= ScF4[i];
    eyh4[i] *= ScF4[i];
  }
  TGraphAsymmErrors* grSc4 = new TGraphAsymmErrors(hData4->GetNbinsX(),x4,y4,exl4,exh4,eyl4,eyh4);
  for(int i = 0 ; i < hData4->GetNbinsX() ; ++i) {
    gs4->GetPoint(i,x4[i],y4[i]);
    eyh4[i] = gs4->GetErrorYhigh(i);    
    eyl4[i] = gs4->GetErrorYlow(i);    
    exh4[i] = gs4->GetErrorXhigh(i);
    exl4[i] = gs4->GetErrorXlow(i);
    y4[i]   *= ScF4[i];
    eyl4[i] *= ScF4[i];
    eyh4[i] *= ScF4[i];
  }
  TGraphAsymmErrors* gsSc4 = new TGraphAsymmErrors(hData4->GetNbinsX(),x4,y4,exl4,exh4,eyl4,eyh4);

  Double_t* x5   = new Double_t[hData5->GetNbinsX()];
  Double_t* y5   = new Double_t[hData5->GetNbinsX()];
  Double_t* exl5 = new Double_t[hData5->GetNbinsX()];
  Double_t* exh5 = new Double_t[hData5->GetNbinsX()];
  Double_t* eyl5 = new Double_t[hData5->GetNbinsX()];
  Double_t* eyh5 = new Double_t[hData5->GetNbinsX()];
  for(int i = 0 ; i < hData5->GetNbinsX() ; ++i) {
    gr5->GetPoint(i,x5[i],y5[i]);
    eyh5[i] = gr5->GetErrorYhigh(i);    
    eyl5[i] = gr5->GetErrorYlow(i);    
    exh5[i] = gr5->GetErrorXhigh(i);
    exl5[i] = gr5->GetErrorXlow(i);
    y5[i]   *= ScF5[i];
    eyl5[i] *= ScF5[i];
    eyh5[i] *= ScF5[i];
  }
  TGraphAsymmErrors* grSc5 = new TGraphAsymmErrors(hData5->GetNbinsX(),x5,y5,exl5,exh5,eyl5,eyh5);
  for(int i = 0 ; i < hData5->GetNbinsX() ; ++i) {
    gs5->GetPoint(i,x5[i],y5[i]);
    eyh5[i] = gs5->GetErrorYhigh(i);    
    eyl5[i] = gs5->GetErrorYlow(i);    
    exh5[i] = gs5->GetErrorXhigh(i);
    exl5[i] = gs5->GetErrorXlow(i);
    y5[i]   *= ScF5[i];
    eyl5[i] *= ScF5[i];
    eyh5[i] *= ScF5[i];
  }
  TGraphAsymmErrors* gsSc5 = new TGraphAsymmErrors(hData5->GetNbinsX(),x5,y5,exl5,exh5,eyl5,eyh5);

// Load Yen-Jie data points

   TGraphErrors* yjst2 = NULL ;
   TGraphErrors* yjst3 = NULL ;
   TGraphErrors* yjst4 = NULL ;
   TGraphErrors* yjst5 = NULL ;
  
   TGraphErrors* yjsy2 = NULL ;
   TGraphErrors* yjsy3 = NULL ;
   TGraphErrors* yjsy4 = NULL ;
   TGraphErrors* yjsy5 = NULL ;


   if ( Energy == 3 && iBin == 5 ) {

     int   n = 0;
     const int  nmax = 300 ;
     double bin[nmax] , x[nmax] , y[nmax] , ex[nmax] , ey[nmax] , sy[nmax], st[nmax] , Factor;
     ifstream mydata ; 


     n = 0 ;     
     Factor = 1000 ;   
     if (MCUnf) mydata.open ("../expdata/ATLAS-B0-2.0.txt");          
     else       mydata.open ("../expdata/Run133242-2.0.txt");          
     while (mydata >> bin[n] >> x[n] >> y[n] >> ey[n] >> sy[n] ) { ex[n] = 0. ; n++; }
     mydata.close();
     for ( int i=0 ; i<n ; ++i ) {
       y [i]  *= Factor ;
       ey [i] *= Factor ;
       sy [i] *= Factor ;
       st [i] = TMath::Sqrt ( ey[i]*ey[i] + sy[i]*sy[i]) ; 
       //cout << ey[i] << " " << sy[i] << " " << st [i] << endl;
     }


     yjst2 = new TGraphErrors(n,x,y,ex,ey); 
     yjsy2 = new TGraphErrors(n,x,y,ex,st); 


     n = 0 ;     
     Factor = 100 ;   
     if (MCUnf) mydata.open ("../expdata/ATLAS-B0-1.5.txt");    
     else       mydata.open ("../expdata/Run133242-1.5.txt");
     while (mydata >> bin[n] >> x[n] >> y[n] >> ey[n] >> sy[n] ) { ex[n] = 0. ; n++; }
     mydata.close();
     for ( int i=0 ; i<n ; ++i ) {
       y [i]  *= Factor ;
       ey [i] *= Factor ;
       sy [i] *= Factor ;
       st [i] = TMath::Sqrt ( ey[i]*ey[i] + sy[i]*sy[i]) ; 
     }
     yjst3 = new TGraphErrors(n,x,y,ex,ey); 
     yjsy3 = new TGraphErrors(n,x,y,ex,st); 

     n = 0 ;     
     Factor = 10 ;   
     if (MCUnf) mydata.open ("../expdata/ATLAS-B0-1.0.txt");
     else       mydata.open ("../expdata/Run133242-1.0.txt");
     while (mydata >> bin[n] >> x[n] >> y[n] >> ey[n] >> sy[n] ) { ex[n] = 0. ; n++; }
     mydata.close();
     for ( int i=0 ; i<n ; ++i ) {
       y [i]  *= Factor ;
       ey [i] *= Factor ;
       sy [i] *= Factor ;
       st [i] = TMath::Sqrt ( ey[i]*ey[i] + sy[i]*sy[i]) ; 
     }
     yjst4 = new TGraphErrors(n,x,y,ex,ey); 
     yjsy4 = new TGraphErrors(n,x,y,ex,st); 

     n = 0 ;     
     Factor = 1 ;   
     if (MCUnf) mydata.open ("../expdata/ATLAS-B0-0.5.txt");
     else       mydata.open ("../expdata/Run133242-0.5.txt");
     while (mydata >> bin[n] >> x[n] >> y[n] >> ey[n] >> sy[n] ) { ex[n] = 0. ; n++; }
     mydata.close();
     for ( int i=0 ; i<n ; ++i ) {
       y [i]  *= Factor ;
       ey [i] *= Factor ;
       sy [i] *= Factor ;
       st [i] = TMath::Sqrt ( ey[i]*ey[i] + sy[i]*sy[i]) ; 
     }
     yjst5 = new TGraphErrors(n,x,y,ex,ey); 
     yjsy5 = new TGraphErrors(n,x,y,ex,st); 


   } 



// Plots

  hDataSc1->SetMarkerColor(kWhite); 

/*
  hDataSc1->SetMarkerStyle(20);
  hDataSc1->SetMarkerSize(.8);
  hDataSc1->SetMarkerColor(kRed);

  hDataSc2->SetMarkerStyle(20);
  hDataSc2->SetMarkerSize(.8);
  hDataSc2->SetMarkerColor(kRed);

  hDataSc3->SetMarkerStyle(20);
  hDataSc3->SetMarkerSize(.8);
  hDataSc3->SetMarkerColor(kRed);

  hDataSc4->SetMarkerStyle(20);
  hDataSc4->SetMarkerSize(.8);
  hDataSc4->SetMarkerColor(kRed);

  hDataSc5->SetMarkerStyle(20);
  hDataSc5->SetMarkerSize(.8);
  hDataSc5->SetMarkerColor(kRed);
*/
 
//  grSc1->SetFillColor(16);
//  grSc2->SetFillColor(16);
//  grSc3->SetFillColor(16);
//  grSc4->SetFillColor(16);
//  grSc5->SetFillColor(16);


  //hDataSc1->GetYaxis()->SetTitleOffset(1.);
  hDataSc1->GetYaxis()->SetTitleSize( 1.2*(hDataSc1->GetYaxis()->GetTitleSize()) );
  hDataSc1->GetXaxis()->SetTitleSize( 1.2*(hDataSc1->GetXaxis()->GetTitleSize()) );
  hDataSc1->GetYaxis()->SetTitle("P_{n}");
  hDataSc1->GetXaxis()->SetTitle("n");
  

  if (iBin == 5 ) {
    hDataSc1->GetYaxis()->SetRangeUser(0.0000001,1000);
    if ( Energy == 1 )  hDataSc1->GetXaxis()->SetRangeUser(-0.5, 90.5);
    if ( Energy == 2 )  hDataSc1->GetXaxis()->SetRangeUser(-0.5,110.5);
    if ( Energy == 3 )  hDataSc1->GetXaxis()->SetRangeUser(-0.5,180.5);
  }
  if (iBin == 15 ) {
    hDataSc1->GetYaxis()->SetRangeUser(0.0000001,10000);
    if ( Energy == 1 )  hDataSc1->GetXaxis()->SetRangeUser(-0.5, 50.5);
    if ( Energy == 2 )  hDataSc1->GetXaxis()->SetRangeUser(-0.5, 80.5);
    if ( Energy == 3 )  hDataSc1->GetXaxis()->SetRangeUser(-0.5,120.5);
  }

/*
  if ( Energy == 1 ) {
    hDataSc1->SetMinimum(0.000001);
    hDataSc1->SetMaximum(1000);
  }
*/  

  hDataSc1->SetLineColor(kWhite);
  hDataSc1->Draw(); 

  grSc1->SetMarkerStyle(kOpenCircle);
  grSc2->SetMarkerStyle(kOpenCircle);
  grSc3->SetMarkerStyle(kOpenCircle);
  grSc4->SetMarkerStyle(kOpenCircle);
  grSc5->SetMarkerStyle(kOpenCircle);

  gsSc1->SetMarkerStyle(kOpenCircle);
  gsSc2->SetMarkerStyle(kOpenCircle);
  gsSc3->SetMarkerStyle(kOpenCircle);
  gsSc4->SetMarkerStyle(kOpenCircle);
  gsSc5->SetMarkerStyle(kOpenCircle);

  grSc1->SetMarkerColor(kRed);
  grSc2->SetMarkerColor(kRed);
  grSc3->SetMarkerColor(kRed);
  grSc4->SetMarkerColor(kRed);
  grSc5->SetMarkerColor(kRed);

  gsSc1->SetMarkerColor(kRed);
  gsSc2->SetMarkerColor(kRed);
  gsSc3->SetMarkerColor(kRed);
  gsSc4->SetMarkerColor(kRed);
  gsSc5->SetMarkerColor(kRed);

  grSc1->SetLineColor(kRed);
  grSc2->SetLineColor(kRed);
  grSc3->SetLineColor(kRed);
  grSc4->SetLineColor(kRed);
  grSc5->SetLineColor(kRed);

  gsSc1->SetLineColor(kRed);
  gsSc2->SetLineColor(kRed);
  gsSc3->SetLineColor(kRed);
  gsSc4->SetLineColor(kRed);
  gsSc5->SetLineColor(kRed);

  grSc1->Draw("p");
  grSc2->Draw("p");
  grSc3->Draw("p");
  grSc4->Draw("p");
  grSc5->Draw("p");
  
  gsSc1->Draw("pZ");
  gsSc2->Draw("pZ");
  gsSc3->Draw("pZ");
  gsSc4->Draw("pZ");
  gsSc5->Draw("pZ");

  hmocaSc1->SetLineColor(kBlack);
  hmocaSc2->SetLineColor(kBlack);
  hmocaSc3->SetLineColor(kBlack);
  hmocaSc4->SetLineColor(kBlack);
  hmocaSc5->SetLineColor(kBlack);
 
  hmocaSc1->Draw("samehist"); 
  hmocaSc2->Draw("samehist"); 
  hmocaSc3->Draw("samehist"); 
  hmocaSc4->Draw("samehist");
  hmocaSc5->Draw("samehist"); 

  

  // Yen-Jie 

  if ( Energy == 3 && iBin == 5 ) {

   yjst2->SetMarkerStyle(kOpenCircle);
   yjst3->SetMarkerStyle(kOpenCircle);
   yjst4->SetMarkerStyle(kOpenCircle);
   yjst5->SetMarkerStyle(kOpenCircle);

   yjsy2->SetMarkerStyle(kOpenCircle);
   yjsy3->SetMarkerStyle(kOpenCircle);
   yjsy4->SetMarkerStyle(kOpenCircle);
   yjsy5->SetMarkerStyle(kOpenCircle);



    yjst2->Draw("p");
    yjst3->Draw("p");
    yjst4->Draw("p");
    yjst5->Draw("p");

    yjsy2->Draw("pZ");
    yjsy3->Draw("pZ");
    yjsy4->Draw("pZ");
    yjsy5->Draw("pZ");

  }

  gsSc1->Draw("pZ");
  gsSc2->Draw("pZ");
  gsSc3->Draw("pZ");
  gsSc4->Draw("pZ");
  gsSc5->Draw("pZ");

/*
  hDataSc1->Draw("same"); 
  hDataSc2->Draw("same"); 
  hDataSc3->Draw("same"); 
  hDataSc4->Draw("same");
  hDataSc5->Draw("same"); 

*/
 
  TText* txt;
  if ( Energy == 1 ) txt=new TText(.6 , 0.96, "CMS Preliminary 0.9 TeV");
  if ( Energy == 2 ) txt=new TText(.6 , 0.96, "CMS Preliminary 2.36 TeV");
  if ( Energy == 3 ) txt=new TText(.6 , 0.96, "CMS Preliminary 7.0 TeV");

  txt->SetNDC(kTRUE);
  txt->SetTextSize(0.04);
  txt->Draw();

  Float_t xt1 , yt1  ;
  Float_t xt2 , yt2  ;
  Float_t xt3 , yt3  ;
  Float_t xt4 , yt4  ;
  Float_t xt5 , yt5  ;

  if ( iBin == 5 ) {
    if ( Energy == 1 ) {  
     xt1 = .55; yt1 = .82;
     xt2 = .40; yt2 = .77;
     xt3 = .33; yt3 = .70;
     xt4 = .27; yt4 = .62;
     xt5 = .19; yt5 = .55;
    }
  
    if ( Energy == 2 ) {  
     xt1 = .57; yt1 = .82;
     xt2 = .42; yt2 = .77;
     xt3 = .33; yt3 = .70;
     xt4 = .27; yt4 = .62;
     xt5 = .19; yt5 = .57;
    }
  
    if ( Energy == 3 ) {  
     xt1 = .55; yt1 = .82;
     xt2 = .40; yt2 = .75;
     xt3 = .33; yt3 = .68;
     xt4 = .27; yt4 = .60;
     xt5 = .17; yt5 = .55;
    }
  } else {
    if ( Energy == 1 ) { 
     xt1 = .49; yt1 = .77;
     xt2 = .42; yt2 = .70;
     xt3 = .37; yt3 = .62;
     xt4 = .30; yt4 = .55;
     xt5 = .25; yt5 = .45;
    }
 
    if ( Energy == 2 ) {
     xt1 = .44; yt1 = .77;
     xt2 = .36; yt2 = .70;
     xt3 = .34; yt3 = .62;
     xt4 = .27; yt4 = .55;
     xt5 = .22; yt5 = .45; 
    }
 
    if ( Energy == 3 ) {
     xt1 = .48; yt1 = .75;
     xt2 = .40; yt2 = .68;
     xt3 = .34; yt3 = .60;
     xt4 = .25; yt4 = .55;
     xt5 = .22; yt5 = .45;
    }
  }


  TText* txt1 = new TText( xt1 , yt1 , "|eta| < 2.4 (x10000)" );
  txt1->SetNDC(kTRUE);
  txt1->SetTextSize(0.02);
  txt1->Draw();

  TText* txt2 = new TText( xt2 , yt2 , "|eta| < 2.  (x1000)" );
  txt2->SetNDC(kTRUE);
  txt2->SetTextSize(0.02);
  txt2->Draw();

  TText* txt3 = new TText( xt3 , yt3 , "|eta| < 1.5 (x100)" );
  txt3->SetNDC(kTRUE);
  txt3->SetTextSize(0.02);
  txt3->Draw();

  TText* txt4 = new TText( xt4 , yt4 , "|eta| < 1.  (x10)" );
  txt4->SetNDC(kTRUE);
  txt4->SetTextSize(0.02);
  txt4->Draw(); 

  TText* txt5 = new TText( xt5 , yt5 , "|eta| < .5 (x1)" );
  txt5->SetNDC(kTRUE);
  txt5->SetTextSize(0.02);
  txt5->Draw();


  TLegend *leg = new TLegend ( .7 , .85 , .9 , .93 );
  if ( iBin == 5  ) leg->SetHeader("P_{T} > 0 GeV"); 
  if ( iBin == 15 ) leg->SetHeader("P_{T} > 0.5 GeV");
  if ( MCUnf ) {
    leg->AddEntry(grSc1,"MC Data (tracks)","p");
    if ( Energy == 3 && iBin == 5) leg->AddEntry(yjst2,"MC Data (tracklets)","p");
  } else {
    leg->AddEntry(grSc1,"Data (tracks)","p");
    if ( Energy == 3 && iBin == 5) leg->AddEntry(yjst2,"Data (tracklets)","p");
  }
  //leg->AddEntry(hDataSc1,"Data","p");
  if ( Energy == 1 ) leg->AddEntry(hmocaSc1,"PYHTHIA D6T  ");
  if ( Energy == 2 ) leg->AddEntry(hmocaSc1,"PYHTHIA D6T  ");
  if ( Energy == 3 ) leg->AddEntry(hmocaSc1,"PYHTHIA ATLAS");
  leg->SetBorderSize(0);
  leg->SetFillColor(0);
  leg->Draw();

 // c1->SaveAs( );


/*
  hData2->Draw("same"); 
  hData3->Draw("same"); 
  hData4->Draw("same"); 
  hData5->Draw("same"); 

  gr1->Draw("psame");
  gr2->Draw("psame");
  gr3->Draw("psame");
  gr4->Draw("psame");
  gr5->Draw("psame");
*/


  if ( iBin == 5 ) {
    if ( MCUnf ) {
      if ( Energy == 1 ) c1->SaveAs("../figs/nchstak_mcunf_0.9TeV.gif","");
      if ( Energy == 2 ) c1->SaveAs("../figs/nchstak_mcunf_2.36TeV.gif","");
      if ( Energy == 3 ) c1->SaveAs("../figs/nchstak_mcunf_7.0TeV.gif","");
    
      if ( Energy == 1 ) c1->SaveAs("../figs/nchstak_mcunf_0.9TeV.eps","");
      if ( Energy == 2 ) c1->SaveAs("../figs/nchstak_mcunf_2.36TeV.eps","");
      if ( Energy == 3 ) c1->SaveAs("../figs/nchstak_mcunf_7.0TeV.eps","");
  
      if ( Energy == 1 ) gSystem->Exec("convert ../figs/nchstak_mcunf_0.9TeV.eps ../figs/nchstak_mcunf_0.9TeV.pdf");
      if ( Energy == 2 ) gSystem->Exec("convert ../figs/nchstak_mcunf_2.36TeV.eps ../figs/nchstak_mcunf_2.36TeV.pdf");
      if ( Energy == 3 ) gSystem->Exec("convert ../figs/nchstak_mcunf_7.0TeV.eps ../figs/nchstak_mcunf_7.0TeV.pdf");
  
    } else {
      if ( Energy == 1 ) c1->SaveAs("../figs/nchstak_0.9TeV.gif","");
      if ( Energy == 2 ) c1->SaveAs("../figs/nchstak_2.36TeV.gif","");
      if ( Energy == 3 ) c1->SaveAs("../figs/nchstak_7.0TeV.gif","");
  
      if ( Energy == 1 ) c1->SaveAs("../figs/nchstak_0.9TeV.eps","");
      if ( Energy == 2 ) c1->SaveAs("../figs/nchstak_2.36TeV.eps","");
      if ( Energy == 3 ) c1->SaveAs("../figs/nchstak_7.0TeV.eps","");
  
      if ( Energy == 1 ) gSystem->Exec("convert ../figs/nchstak_0.9TeV.eps ../figs/nchstak_0.9TeV.pdf");
      if ( Energy == 2 ) gSystem->Exec("convert ../figs/nchstak_2.36TeV.eps ../figs/nchstak_2.36TeV.pdf");
      if ( Energy == 3 ) gSystem->Exec("convert ../figs/nchstak_7.0TeV.eps ../figs/nchstak_7.0TeV.pdf");
    }
  }

  if ( iBin == 15 ) {
    if ( MCUnf ) {
      if ( Energy == 1 ) c1->SaveAs("../figs/nchstak_pt500_mcunf_0.9TeV.gif","");
      if ( Energy == 2 ) c1->SaveAs("../figs/nchstak_pt500_mcunf_2.36TeV.gif","");
      if ( Energy == 3 ) c1->SaveAs("../figs/nchstak_pt500_mcunf_7.0TeV.gif","");
 
      if ( Energy == 1 ) c1->SaveAs("../figs/nchstak_pt500_mcunf_0.9TeV.eps","");
      if ( Energy == 2 ) c1->SaveAs("../figs/nchstak_pt500_mcunf_2.36TeV.eps","");
      if ( Energy == 3 ) c1->SaveAs("../figs/nchstak_pt500_mcunf_7.0TeV.eps","");
 
      if ( Energy == 1 ) gSystem->Exec("convert ../figs/nchstak_pt500_mcunf_0.9TeV.eps ../figs/nchstak_pt500_mcunf_0.9TeV.pdf");
      if ( Energy == 2 ) gSystem->Exec("convert ../figs/nchstak_pt500_mcunf_2.36TeV.eps ../figs/nchstak_pt500_mcunf_2.36TeV.pdf");
      if ( Energy == 3 ) gSystem->Exec("convert ../figs/nchstak_pt500_mcunf_7.0TeV.eps ../figs/nchstak_pt500_mcunf_7.0TeV.pdf");
 
    } else {
      if ( Energy == 1 ) c1->SaveAs("../figs/nchstak_pt500_0.9TeV.gif","");
      if ( Energy == 2 ) c1->SaveAs("../figs/nchstak_pt500_2.36TeV.gif","");
      if ( Energy == 3 ) c1->SaveAs("../figs/nchstak_pt500_7.0TeV.gif","");
 
      if ( Energy == 1 ) c1->SaveAs("../figs/nchstak_pt500_0.9TeV.eps","");
      if ( Energy == 2 ) c1->SaveAs("../figs/nchstak_pt500_2.36TeV.eps","");
      if ( Energy == 3 ) c1->SaveAs("../figs/nchstak_pt500_7.0TeV.eps","");
 
      if ( Energy == 1 ) gSystem->Exec("convert ../figs/nchstak_pt500_0.9TeV.eps ../figs/nchstak_pt500_0.9TeV.pdf");
      if ( Energy == 2 ) gSystem->Exec("convert ../figs/nchstak_pt500_2.36TeV.eps ../figs/nchstak_pt500_2.36TeV.pdf");
      if ( Energy == 3 ) gSystem->Exec("convert ../figs/nchstak_pt500_7.0TeV.eps ../figs/nchstak_pt500_7.0TeV.pdf");
    }
  }



}
