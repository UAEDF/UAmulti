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
using namespace std;

void plot3ene ( float npx , float npy ) {

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

    fData1 = new TFile("../plots/final2/unfolding_MC_D6T_0.9TeV_mbTr__hyp1_niter0_cut5_DataType0.root","READ");
    fData2 = new TFile("../plots/final2/unfolding_MC_D6T_2.36TeV_mbTr__hyp1_niter0_cut5_DataType0.root","READ");
    fData3 = new TFile("../plots/final2/unfolding_MC_ATLAS_7.0TeV_mbTr__hyp1_niter0_cut5_DataType0.root","READ");

  fData1->cd();
  TH1F* hData1 = (TH1F*) fData1->Get("unfolding/nch_data_corrected");
  TH1F* hmoca1 = (TH1F*) fData1->Get("unfolding/nch_MC_gen_afterEvtSelCorrection");
  fData2->cd();
  TH1F* hData2 = (TH1F*) fData2->Get("unfolding/nch_data_corrected");
  TH1F* hmoca2 = (TH1F*) fData2->Get("unfolding/nch_MC_gen_afterEvtSelCorrection");
  fData3->cd();
  TH1F* hData3 = (TH1F*) fData3->Get("unfolding/nch_data_corrected");
  TH1F* hmoca3 = (TH1F*) fData3->Get("unfolding/nch_MC_gen_afterEvtSelCorrection");
 
  fData1->cd();
  TGraphAsymmErrors* gr1 = (TGraphAsymmErrors*) fData1->Get("unfolding/gnch_data_corrected");
  TGraphAsymmErrors* gs1 = (TGraphAsymmErrors*) fData1->Get("unfolding/gnch_data_corrected_syst");
  fData2->cd();
  TGraphAsymmErrors* gr2 = (TGraphAsymmErrors*) fData2->Get("unfolding/gnch_data_corrected");
  TGraphAsymmErrors* gs2 = (TGraphAsymmErrors*) fData2->Get("unfolding/gnch_data_corrected_syst");
  fData3->cd();
  TGraphAsymmErrors* gr3 = (TGraphAsymmErrors*) fData3->Get("unfolding/gnch_data_corrected");
  TGraphAsymmErrors* gs3 = (TGraphAsymmErrors*) fData3->Get("unfolding/gnch_data_corrected_syst");

  Float_t ndata1 = hData1->Integral(1, hData1->GetNbinsX() ); 
  Float_t ndata2 = hData2->Integral(1, hData2->GetNbinsX() ); 
  Float_t ndata3 = hData3->Integral(1, hData3->GetNbinsX() ); 

  TH1F* hDataSc1 = (TH1F*) hData1->Clone("hDataSc1");
  TH1F* hDataSc2 = (TH1F*) hData2->Clone("hDataSc2");
  TH1F* hDataSc3 = (TH1F*) hData3->Clone("hDataSc3");

  hDataSc1->Scale(1/ndata1,"width");
  hDataSc2->Scale(1/ndata2,"width");
  hDataSc3->Scale(1/ndata3,"width");

  hDataSc1->Scale(1,"width");
  hDataSc2->Scale(10,"width");
  hDataSc3->Scale(100,"width");

  Double_t* ScF1 = new Double_t[hData1->GetNbinsX()];
  Double_t* ScF2 = new Double_t[hData2->GetNbinsX()];
  Double_t* ScF3 = new Double_t[hData3->GetNbinsX()];

  for(int i = 1 ; i <= hData1->GetNbinsX() ; ++i) ScF1[i-1] = hDataSc1->GetBinContent(i) / hData1->GetBinContent(i) ;
  for(int i = 1 ; i <= hData2->GetNbinsX() ; ++i) ScF2[i-1] = hDataSc2->GetBinContent(i) / hData2->GetBinContent(i) ;
  for(int i = 1 ; i <= hData3->GetNbinsX() ; ++i) ScF3[i-1] = hDataSc3->GetBinContent(i) / hData3->GetBinContent(i) ;

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

  hDataSc1->SetMarkerColor(kWhite); 

  //hDataSc1->GetYaxis()->SetTitleOffset(1.);
  hDataSc1->GetYaxis()->SetTitleSize( 1.2*(hDataSc1->GetYaxis()->GetTitleSize()) );
  hDataSc1->GetXaxis()->SetTitleSize( 1.2*(hDataSc1->GetXaxis()->GetTitleSize()) );
  hDataSc1->GetYaxis()->SetTitle("P_{n}");
  hDataSc1->GetXaxis()->SetTitle("n");
  

  hDataSc1->GetYaxis()->SetRangeUser(0.00001,10);
  hDataSc1->GetXaxis()->SetRangeUser(-0.5,180.5);
  hDataSc1->SetLineColor(kWhite);
  hDataSc1->Draw(); 



  grSc1->SetMarkerStyle(kOpenCircle);
  grSc2->SetMarkerStyle(kOpenCircle);
  grSc3->SetMarkerStyle(kOpenCircle);

  gsSc1->SetMarkerStyle(kOpenCircle);
  gsSc2->SetMarkerStyle(kOpenCircle);
  gsSc3->SetMarkerStyle(kOpenCircle);

  grSc3->SetMarkerColor(kRed);
  grSc2->SetMarkerColor(kBlue);
  grSc1->SetMarkerColor(kBlack);

  gsSc3->SetMarkerColor(kRed);
  gsSc2->SetMarkerColor(kBlue);
  gsSc1->SetMarkerColor(kBlack);

  grSc3->SetLineColor(kRed);
  grSc2->SetLineColor(kBlue);
  grSc1->SetLineColor(kBlack);

  gsSc3->SetLineColor(kRed);
  gsSc2->SetLineColor(kBlue);
  gsSc1->SetLineColor(kBlack);

  grSc1->Draw("p");
  grSc2->Draw("p");
  grSc3->Draw("p");
  
  gsSc1->Draw("pZ");
  gsSc2->Draw("pZ");
  gsSc3->Draw("pZ");


/*
  hDataSc1->Draw("same"); 
  hDataSc2->Draw("same"); 
  hDataSc3->Draw("same"); 
  hDataSc4->Draw("same");
  hDataSc5->Draw("same"); 
*/

 
  TText* txt;
  txt=new TText(.7 , 0.96, "CMS Preliminary");

  txt->SetNDC(kTRUE);
  txt->SetTextSize(0.04);
  txt->Draw();

  Float_t xt1 , yt1  ;
  Float_t xt2 , yt2  ;
  Float_t xt3 , yt3  ;

  xt1 = .27; yt1 = .55;
  xt2 = .35; yt2 = .65;
  xt3 = .47; yt3 = .75;


  TText* txt1 = new TText( xt1 , yt1 , "(x1)" );
  txt1->SetNDC(kTRUE);
  txt1->SetTextSize(0.02);
  txt1->Draw();

  TText* txt2 = new TText( xt2 , yt2 , "(x10)" );
  txt2->SetNDC(kTRUE);
  txt2->SetTextSize(0.02);
  txt2->Draw();

  TText* txt3 = new TText( xt3 , yt3 , "(x100)" );
  txt3->SetNDC(kTRUE);
  txt3->SetTextSize(0.02);
  txt3->Draw();


  TLegend *leg = new TLegend ( .7 , .75 , .9 , .93 );
   leg->SetHeader("|#eta|<2.5");
  leg->AddEntry(grSc1,"Data 0.9 TeV ",  "p");
  leg->AddEntry(grSc2,"Data 2.36 TeV ",  "p");
  leg->AddEntry(grSc3,"Data 7 TeV ",  "p");
  //leg->AddEntry(hDataSc1,"Data","p");
  leg->SetBorderSize(0);
  leg->SetFillColor(0);
  leg->Draw();


  c1->SaveAs("../figs/nch3ene.gif","");
  c1->SaveAs("../figs/nch3ene.eps","");

}
