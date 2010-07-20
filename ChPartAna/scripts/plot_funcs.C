#include <TROOT.h>
#include <TH1D.h>
#include <TH2D.h>
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
#include <TF1.h>
//#include <TText.h>
#include <TLatex.h>
#include <iostream>
#include <fstream>
using namespace std;

#include "TMath.h"
using namespace TMath;

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


TGraphAsymmErrors h2g(TH1* h , bool withErrors = true){
  int nmax = h->GetNbinsX()-1;
  int n = 0;  
  Double_t x[nmax],ex[nmax],y[nmax],ey[nmax];
  for (int i = 1 ; i <=  h->GetNbinsX() ; ++i ) { 
    x[n]   = h->GetBinCenter(i);  
    y[n]   = h->GetBinContent(i);    
    if ( ! withErrors ) {
      ex[n]  = 0.;
      ey[n]  = 0.;
    }
    else{
      ex[n]  = x[n] - h->GetBinLowEdge(i);
      ey[n]  = h->GetBinError(i);
    }
    n++;       
  } 
  TGraphAsymmErrors g(n,x,y,ex,ex,ey,ey);
  g.SetLineWidth(h->GetLineWidth());
  g.SetLineColor(h->GetLineColor());
  g.SetLineStyle(h->GetLineStyle());
  g.SetMarkerColor(h->GetMarkerColor());
  g.SetMarkerStyle(h->GetMarkerStyle());
  
  for(int i = (g.GetN()-1) ; i>-1 ; --i) {
    Double_t x,y ;  
    g.GetPoint(i,x,y);
    if ( y <= histoYMin )
      g.RemovePoint(i);
  }
  return g;
} 


//NBD: From websight
Double_t NBD(Double_t x, Double_t nmean, Double_t k){
  Double_t p = 1. / ( (nmean / k) + 1. );

/*  
  cout << "p: " << p << endl;
  cout << "1: " << Gamma(x+k) << endl;
  cout << "2: " << ( Gamma(x+1) * Gamma(k) ) << endl;
  cout << "3: " << pow(p,k) << endl;
  cout << "4: " << pow ( 1 - p , x) << endl;
*/

  //Double_t val = Gamma(x+k)/( Gamma(x+1) * Gamma(k) ) * pow(p,k) * pow ( 1 - p , x);
  Double_t val = Exp ( LnGamma(x+k) - LnGamma(x+1) - LnGamma(k) ) * pow(p,k) * pow ( 1 - p , x);
  if ( isnan(val) || isinf(val) ) val=0. ;
  return val;
}

Double_t funcSingleNBD(Double_t *x, Double_t *par)
{
   Float_t xx =x[0];
   Double_t f = par[0] * NBD( xx , par[1] , par[2] ) ;
   return f;
}

Double_t funcDoubleNBD(Double_t *x, Double_t *par)
{
   Float_t xx =x[0];
   Double_t f = par[0] * (   (1.-par[1]) * NBD( xx , par[2] , par[3] )
                           +   par[1]    * NBD( xx , par[4] , par[5] ) )  ;
   return f;
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

void XMaxBinKiller ( TH1 *&hstat  , double XMax ) {

    for(int i = 1 ; i <=  hstat->GetNbinsX() ; ++i ) {
//    for(int i = (hstat->GetNbinsX()) ; i>0 ; --i) {  
      if ( hstat->GetBinCenter(i) > XMax ) {
         cout << "-------------------------------- Killing bin: "  << i << " " << hstat->GetBinCenter(i) << endl;
         hstat->SetBinContent(i,0.0);
         hstat->SetBinError(i,0.0);
	 cout << "-------------------------------- "  << hstat->GetBinContent(i) << endl;
      }
    } 
}

void YMinBinKiller ( TH1 *&hstat  , double YMin ) {

    //for(int i = 1 ; i <=  hstat->GetNbinsX() ; ++i ) {
    for(int i = (hstat->GetNbinsX()) ; i>0 ; --i) {  
      if ( hstat->GetBinContent(i) <= YMin ) {
         cout << "Killing bin: "  << i << " " << hstat->GetBinCenter(i) << endl;
         hstat->SetBinContent(i,0.);
         hstat->SetBinError(i,0.);
      }
    } 
}



// Remove Points with RelError > 100 % 
void SerialBinKiller ( TH1 *&hstat  , TGraphAsymmErrors *&gsyst , 
                       Float_t downLimit     = -9999. , Float_t upLimit     = 9999. , 
                       Float_t downTolerance =   1.   , Float_t upTolerance = 9999.   ) {

  
  cout << hstat->GetNbinsX() << endl; 
  cout << gsyst->GetN() << endl;
  if ( hstat->GetNbinsX() !=   gsyst->GetN() ) return;
  
  for(int i = (gsyst->GetN()-1) ; i>-1 ; --i) {  
     Double_t x,y,eyup,eydown ;
     gsyst->GetPoint(i,x,y);
     eyup   = gsyst->GetErrorYhigh(i);
     eydown = gsyst->GetErrorYlow(i);
     float iSLargeError = false;
     //cout << "Bin : " << i << " " << x << endl; 
     if ( fabs(y) > 0 ) {
        if ( fabs(eydown/y) >= downTolerance ) iSLargeError = true ;
        if ( fabs(eyup/y)   >= upTolerance   ) iSLargeError = true ;
     } 
     if (iSLargeError) {
         cout << "Killing : " << x << " " << y << " " << endl; 
         hstat->SetBinContent(i+1,0.);
         hstat->SetBinError(i+1,0.);
         gsyst->RemovePoint(i);
     }

   }   
}

void XMaxBinKiller ( TGraphAsymmErrors *&gstat  , double XMax ) {
    //for(int i = 0 ; i < gstat->GetN() ; ++i ) {
    for(int i = (gstat->GetN()-1) ; i>-1 ; --i) {
      Double_t x,y ;  
      gstat->GetPoint(i,x,y);
      if ( x > XMax ) { 
        gstat->RemovePoint(i); 
        cout << "Killing bin: "  << i << " " << x << endl;
      }  
    }
}

void YMinBinKiller ( TGraphAsymmErrors *&gstat  , double YMin ) {
    //for(int i = 0 ; i < gstat->GetN() ; ++i ) {
    for(int i = (gstat->GetN()-1) ; i>-1 ; --i) {
      Double_t x,y ;  
      gstat->GetPoint(i,x,y);
      if ( y <= YMin ) { 
        gstat->RemovePoint(i); 
        cout << "Killing bin: "  << i << " " << x << endl;
      }  
    }
}


// Remove Points with RelError > 100 % 
void SerialBinKiller ( TGraphAsymmErrors *&gstat  , TGraphAsymmErrors *&gsyst , 
                       Float_t downLimit     = -9999. , Float_t upLimit     = 9999. , 
                       Float_t downTolerance = 1. , Float_t upTolerance = 999.  ) {

    cout << gstat->GetN() << endl;
    cout << gsyst->GetN() << endl;
    if (  gsyst->GetN() != gstat->GetN() ) return;

    for(int i = (gsyst->GetN()-1) ; i>-1 ; --i) {
       Double_t x,y,eyup,eydown ;
       gsyst->GetPoint(i,x,y);
       eyup   = gsyst->GetErrorYhigh(i);
       eydown = gsyst->GetErrorYlow(i);
       float iSLargeError = false;
       if ( fabs(y) > 0 ) {
         if ( fabs(eydown/y) >= downTolerance ) iSLargeError = true ;
         if ( fabs(eyup/y)   >= upTolerance   ) iSLargeError = true ;
       } else iSLargeError = true ;

       if (iSLargeError) {
         cout << "Killing : " << x << " " << y << " " << endl;
         gstat->RemovePoint(i);        
         gsyst->RemovePoint(i);
       }
    }
}

Double_t getLastFilledBin(TH1* h){
  int ibin = 2;
  for(;ibin <= h->GetNbinsX() ; ++ibin){
    if(h->GetBinContent(ibin) == 0.) break;
  }
  return h->GetBinCenter(ibin-1) + h->GetBinWidth(ibin-1)/2.;
}

Double_t getLastFilledBin(TGraphAsymmErrors* g){
  int ibin = 1;
  Double_t x , y;
  for(;ibin < g->GetN() ; ++ibin){
    g->GetPoint(ibin , x , y);
    if(y == 0.) break;
  }
  g->GetPoint(ibin-1 , x , y);
  return x + g->GetErrorXhigh(ibin-1);
}
