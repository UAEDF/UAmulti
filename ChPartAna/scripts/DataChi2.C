

#include <TROOT.h>
#include <TF1.h>
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

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>

using namespace std;
using namespace TMath;

double  globalEnergy = 0.9;
#include "../macro/fileManager.C"

// This give the integral of a TGraph assuming x error bar as bin width
// Integral done between iBinMin and iBinMax
Double_t TGIntegral ( TGraphAsymmErrors *&graph , int iBinMin , int iBinMax )
{
   //cout << graph->GetN() << endl;
   if ( graph->GetN() > iBinMax ) iBinMax = graph->GetN() ;
   if ( iBinMin       > iBinMax ) return 0. ;
   Double_t Integral = 0.;
   for(int i = iBinMin ; i < iBinMax ; ++i) {
     Double_t x,y,deltax ;
     graph->GetPoint(i,x,y);
     deltax = graph->GetErrorXhigh(i) + graph->GetErrorXlow(i) ;
     //cout << i << " " << x << " " << y << " " << graph->GetErrorXhigh(i) << " " << graph->GetErrorXlow(i)  << " " << deltax << endl;
     Integral += y*deltax ;
   }
   return Integral;
}


// Scale a TGraphAsymErrors
void TGAsymScale ( TGraphAsymmErrors *&graph , Float_t Scale )
{  
   for(int i = 0 ; i < graph->GetN() ; ++i) {
     Double_t x,y,eyup,eydown ;
     graph->GetPoint(i,x,y);
     eyup   = graph->GetErrorYhigh(i);
     eydown = graph->GetErrorYlow(i);
     y      *= Scale; 
     eyup   *= Scale; 
     eydown *= Scale;
     graph->SetPoint(i,x,y);
     graph->SetPointEYhigh(i,eyup);
     graph->SetPointEYlow(i,eydown);
   }   
}



void DataChi2(double energy = 0.9, int iBin = 6 ) {

  globalEnergy = energy ;

  //gROOT->ProcessLine(".x cmsStyleRoot.C");

  // Fetch Track Data 
  
  TString fileName ;

  TFile* fData      = NULL; 
  TGraphAsymmErrors* gTrackingStat = NULL ;
  TGraphAsymmErrors* gTrackingSyst = NULL ;
  TH2F* hTrackingCov1 = NULL ;    
  TH2F* hTrackingCov2 = NULL ;    

  TString plotdir("");
  plotdir = "/user/rougny/Ferenc_Tracking_bis/CMSSW_3_3_6_patch3/src/UAmulti/ChPartAna/plots/systv10_17_3/";

  ostringstream outstr("");
  outstr << "hyp" << 1 << "_niter" << 0 << "_cut" << iBin << "_DataType" << 0;

  int iMc;
  if ( globalEnergy == 0.9 ) iMc = 10 ;
  if ( globalEnergy == 2.36) iMc = 10 ;
  if ( globalEnergy == 7.0 ) iMc = 31 ;

  fileName = fileManager(3,iMc,globalEnergy,1,0,0,outstr.str(),plotdir);
  fData = new TFile(fileName,"READ");
  //cout << "opening: " << fileName << endl; 
  if ( fData == 0 ) {
    cout << "[plot] File does not exist " << endl;
    return ;
  } 
 

  gTrackingStat = (TGraphAsymmErrors*) fData->Get("unfolding/gnch_data_corrected");
  gTrackingSyst = (TGraphAsymmErrors*) fData->Get("unfolding/gnch_data_corrected_syst");
  hTrackingCov1 = (TH2F*) fData->Get("unfolding/hist_resampling/covmtx");
  hTrackingCov2 = (TH2F*) fData->Get("unfolding/mtx_resampling/covmtx");

  Float_t ndata = TGIntegral( gTrackingStat , 0, gTrackingStat->GetN() );     

  TGAsymScale(gTrackingStat,1/ndata);
  TGAsymScale(gTrackingSyst,1/ndata);
  

  gTrackingStat->Draw("ap");
  gTrackingSyst->Draw("psame");
  
  // Fetch Tracklet Data 

  if ( globalEnergy == 0.9 ) {
    if ( iBin == 6 ) fileName = "../expdata/Run124023-2.0.txt";
    if ( iBin == 7 ) fileName = "../expdata/Run124023-1.5.txt";
    if ( iBin == 8 ) fileName = "../expdata/Run124023-1.0.txt";
    if ( iBin == 9 ) fileName = "../expdata/Run124023-0.5.txt";
  }
  if ( globalEnergy == 7.0 ) {
    if ( iBin == 6 ) fileName = "../expdata/Run133242-2.0.txt";
    if ( iBin == 7 ) fileName = "../expdata/Run133242-1.5.txt";
    if ( iBin == 8 ) fileName = "../expdata/Run133242-1.0.txt";
    if ( iBin == 9 ) fileName = "../expdata/Run133242-0.5.txt";
  }

  int   n = 0;
  const int  nmax = 300 ;
  double xl, x[nmax], y[nmax] , ex[nmax] , ey[nmax] , sy[nmax] , eyt[nmax]  ;

  ifstream mydata ;
  mydata.open (fileName);
  while (mydata >>  xl >> x[n] >> y[n] >> ey[n] >> sy[n] ) {
    ex[n] = 0.; 
    eyt[n] = sqrt( ey[n]*ey[n] + sy[n]*sy[n] ) ;
    n++;
  }
  mydata.close();

  TGraphAsymmErrors* gTrackletStat = new TGraphAsymmErrors(n,x,y,ex,ex,ey,ey); 
  TGraphAsymmErrors* gTrackletSyst = new TGraphAsymmErrors(n,x,y,ex,ex,eyt,eyt);

  gTrackletStat->SetMarkerColor(kRed);
  gTrackletSyst->SetMarkerColor(kRed);
  gTrackletStat->SetLineColor(kRed);
  gTrackletSyst->SetLineColor(kRed);
  gTrackletStat->Draw("psame");
  gTrackletSyst->Draw("psame");

  // Compute Chi2

  int ndof = 0 ;
  int ntot = min (gTrackingStat->GetN(),gTrackletStat->GetN())  ;
  TH1F* hTracking = new TH1F("hTracking","hTracking",ntot-1,0.,1.*(ntot-1));
  TH1F* hTracklet = new TH1F("hTracklet","hTracklet",ntot-1,0.,1.*(ntot-1));
  TH1F* hDivided  = new TH1F("hDivided ","hDivided ",ntot-1,0.,1.*(ntot-1));

  float chi2 = 0.0;
  for ( int i = 0 ; i < ntot ; ++i ) {  
    double x1,y1,eyup1,eydown1 ;
    double x2,y2,eyup2,eydown2 ;
    gTrackingStat->GetPoint(i,x1,y1);
    eyup1 = gTrackingSyst->GetErrorYhigh(i);
    gTrackletStat->GetPoint(i,x2,y2);
    eyup2 = gTrackletSyst->GetErrorYhigh(i);
    if ( y1>0 && y2>0 ) {
      ++ndof;
      chi2 += pow(y1-y2,2) / (eyup1*eyup1) ;
    }
    if (i > 0) {
      hTracking->SetBinContent(i+1,y1);
      hTracking->SetBinError(i+1,eyup1);
      hTracklet->SetBinContent(i+1,y2);
      //hTracklet->SetBinError(i+1,eyup2);
    }
  }
//  cout << "chi2= " << chi2 << endl;
//  cout << "ntot= " << ntot << endl;
//  cout << "ndof= " << ndof << endl;  

  cout << "E= " << energy << " Bin: " << iBin << endl;
  cout << " --> chi2/ndof= " << chi2/ndof << endl;


  hTracking->Draw();
  hTracklet->Draw("same");

  hDivided->Divide(hTracklet,hTracking,1.,1.);

//  Double_t res[200];
//  hTracking->Chi2Test(hTracklet,"WW P CHI2/NDF",res);

  cout << "KolmogorovTest: " << hTracking-> KolmogorovTest(hTracklet) << endl;

  hDivided->Draw();

}
