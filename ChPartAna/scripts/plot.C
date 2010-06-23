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
         cout << "Killing : " << x << " " << y << " " << eydown/y << endl; 
         hstat->SetBinContent(i+1,0.);
         hstat->SetBinError(i+1,0.);
         gsyst->RemovePoint(i);
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
         cout << "Killing : " << x << " " << y << " " << eydown/y << endl;
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

void plot (TString dir , TString histo , int logY = false , int iLegendPos = 0 )
{

  if ( dataSetId.size()==0 ) return;

  if ( globalHistoType == 2 ) {
    if ( dataSetId.size() > 1 ) {
      cout << "[plot] Can not plot more than 1 TH2F histo !!! " << endl;
      return;
    } 
  } 
   
  cout<<histo<<endl;
  //TCanvas* c1 = new TCanvas("c1","c",500,500); 
  TCanvas* c1 = new TCanvas("c1","c",globalCanvasSizeX,globalCanvasSizeY);
  //c1->SetLeftMargin(0.17);
  //c1->SetBottomMargin(0.10);
  c1->SetFillColor(0);
  c1->GetFrame()->SetFillColor(21);
  c1->GetFrame()->SetBorderSize(12);
  c1->SetGrid(0,0);
  c1->cd();

   TPad *p1 = NULL; 
   TPad *p2 = NULL; 

  if (globalRatioType>0) { 
    //c1->Divide(1,2);
    //c1->cd(1);
    p1=new TPad("p1","p1",0,0.5, 1,1);
    p2=new TPad("p2","p2",0,0, 1,0.5);
    p1->Draw();
    p2->Draw();
    p1->cd();   
  }

  // FIXME
  if(logY == 1 ) gPad->SetLogy(true);
  if(logY == 2 ) gPad->SetLogx(true);

  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(kFALSE);

  Float_t hMax = 0 ;
  Float_t IntKeep = 0 ;
 
  vector<TLatex*>          Text  ( textText.size() , NULL );

  vector<int>             kData ( dataSetId.size() , 0 );
  vector<int>             tData ( dataSetId.size() , globalHistoType );
  vector<int>             sData ( dataSetId.size() , 0 );
  vector<bool>            rData ( dataSetId.size() , 1    ); 
  vector<TFile*>          fData ( dataSetId.size() , NULL );
  vector<TH1*>            hData ( dataSetId.size() , NULL );
//  vector<TGraph*>         gData ( dataSetId.size() , NULL );
  vector<TGraphAsymmErrors*> gData ( dataSetId.size() , NULL );

  vector<TH1*>                hDivData ( dataSetId.size() , NULL );
  vector<TGraphAsymmErrors*>  gDivData ( dataSetId.size() , NULL ); 

  TF1* f1 = NULL;

  // Dividor histogram or TGraph
  TH1* hDividor = NULL;
  TGraphAsymmErrors* gDividor = NULL;

  for(int iData = 0 ; iData < (signed) dataSetId.size() ; ++iData) { 
    // Get histo Type
    if ( (signed) dataSetHType.size() > iData ) tData.at(iData) = dataSetHType.at(iData);
    if ( tData.at(iData) == 104 ) { tData.at(iData) = 4 ; sData.at(iData) = 1 ; }
    if ( tData.at(iData) == 105 ) { tData.at(iData) = 5 ; sData.at(iData) = 1 ; }
    if ( tData.at(iData) == 205 ) { tData.at(iData) = 5 ; sData.at(iData) = 1 ;  kData.at(iData) = 1 ; }
    // Get histo
    TString fileName ;
    if      ( dataSetId.at(iData) >=  0 )
      fileName = fileManager(globalFileType,dataSetId.at(iData),globalEnergy,globalTraking,0,0,ptcutstr,globalDirPlot) ; 
    else 
      fileName = dataSetFile.at(iData);
    cout << "File: " << fileName << endl;

    // Data from root file ?
    // bool isRootData = true; 
    if ( histo == "AUTO" ) {
      if (    dataSetHisto.at(iData) == "EXTDATA" || dataSetHisto.at(iData) == "UA5"
           || dataSetHisto.at(iData) == "TRACKLETS" 
           || dataSetHisto.at(iData) == "UA1"     || dataSetHisto.at(iData) == "ALICE" )  
         rData.at(iData) = 0 ; //isRootData = false;
    }

    // Data from root files: TH1, TH2, TProfile
    if ( rData.at(iData) ) 
    {
      fData.at(iData) = new TFile(fileName,"READ");
      if ( fData.at(iData) == 0 ) { 
        cout << "[plot] File does not exist " << endl;
        return;
      }
  
      fData.at(iData)->cd();
  
      TString histoName ("");
      if ( dir   != "none" ) histoName += dir + "/"; 
      if ( histo == "AUTO" ) histoName += dataSetHisto.at(iData);
      else                   histoName += histo;
    
      cout << "Histo: " << histoName << endl;
  
      if ( tData.at(iData) == 1 ) hData.at(iData) = (TH1F*) fData.at(iData)->Get(histoName);
      if ( tData.at(iData) == 2 ) hData.at(iData) = (TH2F*) fData.at(iData)->Get(histoName);
      if ( tData.at(iData) == 3 ) hData.at(iData) = (TProfile*) fData.at(iData)->Get(histoName);
//      if ( tData.at(iData) == 4 ) gData.at(iData) = (TGraphErrors*) fData.at(iData)->Get(histoName);
      if ( tData.at(iData) == 4 ) { cout << "TGraphErrors not available !" << endl; return;}
      if ( tData.at(iData) == 5 ) gData.at(iData) = (TGraphAsymmErrors*) fData.at(iData)->Get(histoName);
  
      if ( ! ( tData.at(iData) == 4 || tData.at(iData) == 5 ) ) { 
        if ( hData.at(iData) == 0 ) {
          cout << "[plot] Histo does not exist " << endl;
          return;
        }
      } else {
        if ( gData.at(iData) == 0 ) {
          cout << "[plot] Graph does not exist " << endl;
          return;
        }   
      }

      // Plot Style
      if ( ! dataSetIsMc.at(iData) ) {
        if ( ! ( tData.at(iData) == 4 || tData.at(iData) == 5 ) ) {
          hData.at(iData)->SetLineWidth(1);
          hData.at(iData)->SetMarkerColor(dataSetColor.at(iData));
          hData.at(iData)->SetMarkerStyle(dataSetStyle.at(iData)); 
          hData.at(iData)->SetLineColor(dataSetColor.at(iData));
        } else {
          gData.at(iData)->SetLineWidth(1);
          gData.at(iData)->SetMarkerColor(dataSetColor.at(iData));
          gData.at(iData)->SetMarkerStyle(dataSetStyle.at(iData));
          gData.at(iData)->SetLineColor(dataSetColor.at(iData));
        } 

      } else {
        if ( ! ( tData.at(iData) == 4 || tData.at(iData) == 5 ) ) {
          hData.at(iData)->SetLineWidth(3);
          hData.at(iData)->SetLineColor(dataSetColor.at(iData));
          hData.at(iData)->SetLineStyle(dataSetStyle.at(iData));
        } else {
          gData.at(iData)->SetLineWidth(3);
          gData.at(iData)->SetLineColor(dataSetColor.at(iData));
          gData.at(iData)->SetLineStyle(dataSetStyle.at(iData));
        }

      }
      // Normalisation
      if ( iData>0 && globalNorm == 1 ) {
        Float_t ndata = 1. ; 
        Float_t nmoca = 1. ;  
        if ( ! ( tData.at(0) == 4 || tData.at(0) == 5 ) ) {
          ndata = hData.at(0)->Integral(1, hData.at(0)->GetNbinsX() );
          cout << "TH1F Int " << 0 << " = " << ndata << endl; 
        } 
        else
        {
          ndata = TGIntegral(gData.at(0),0, gData.at(0)->GetN() );
          cout << "TGraph Int " << 0 << " = " << ndata << endl; 
        }
        
        if ( ! ( tData.at(iData) == 4 || tData.at(iData) == 5 ) ) {
          nmoca = hData.at(iData)->Integral(1, hData.at(iData)->GetNbinsX() );
          cout << "TH1F Int " << iData << " = " << nmoca << endl; 
          hData.at(iData)->Scale(ndata/nmoca);
          // hData.at(iData)->Scale(ndata/nmoca,"width"); // FIXME
        } else {
          nmoca = TGIntegral(gData.at(iData) , 0, gData.at(iData)->GetN() );
          cout << "TGraph Int " << iData << " = " << nmoca << endl; 
          if (tData.at(iData) == 4) cout << "fixme" << endl;
          if (tData.at(iData) == 5) TGAsymScale(gData.at(iData),ndata/nmoca); 
        }   
      }
      if ( globalNorm == 2 ) {
        if ( ! ( tData.at(iData) == 4 || tData.at(iData) == 5 ) ) {
          Float_t ndata = hData.at(iData)->Integral(1, hData.at(iData)->GetNbinsX() );
          IntKeep = ndata ; 
          hData.at(iData)->Scale(1/ndata);
          //cout << "TH1F Int " << iData << " = " << ndata << endl; 
          //cout << "-->Scale " <<  hData.at(iData)->Integral(1, hData.at(iData)->GetNbinsX() ) << endl;
 
          // hData.at(iData)->Scale(1/ndata,"width"); // FIXME
        } else {
          Float_t ndata = TGIntegral(gData.at(iData) , 0, gData.at(iData)->GetN() );
           cout << "TGraph Int " << iData << " " << ndata << endl;
          if( kData.at(iData) ) ndata = IntKeep ;

          //cout << "TGraph Int " << iData << " = " << ndata << endl; 
//           gData.at(iData)->GetHistogram()->Draw();
//            gPad->WaitPrimitive();

          if (tData.at(iData) == 4) cout << "fixme" << endl;
          if (tData.at(iData) == 5) TGAsymScale(gData.at(iData),1/ndata);
        }
      }
      if ( globalNorm == 3 ) { // Excluding bin 0 for normalization
        if ( ! ( tData.at(iData) == 4 || tData.at(iData) == 5 ) ) {
          Float_t ndata = hData.at(iData)->Integral(2, hData.at(iData)->GetNbinsX() );
          IntKeep = ndata ; 
          hData.at(iData)->Scale(1/ndata);
          // hData.at(iData)->Scale(1/ndata,"width"); // FIXME
        } else {
          Float_t ndata = TGIntegral(gData.at(iData) , 1, gData.at(iData)->GetN() );
          if( kData.at(iData) ) ndata = IntKeep ;
          if (tData.at(iData) == 4) cout << "fixme" << endl;
          if (tData.at(iData) == 5) TGAsymScale(gData.at(iData),1/ndata);
        }  

      }
      // Factor
      cout << dataSetFactor.size() << " " << iData << endl;
      if ( (signed) dataSetFactor.size() > iData ) {
        cout << "ScaleFactor = " << dataSetFactor.at(iData) << endl ; 
        if ( ! ( tData.at(iData) == 4 || tData.at(iData) == 5 ) ) {
          hData.at(iData)->Scale(dataSetFactor.at(iData));
        } else {
          if (tData.at(iData) == 4) cout << "fixme" << endl;
          if (tData.at(iData) == 5) TGAsymScale(gData.at(iData),dataSetFactor.at(iData));
        } 
      }


    // Data from external text file 
    } else {
     
 
      cout << "[plot] make TGraphError !!!!" << endl; 

      if ( dataSetHisto.at(iData) == "EXTDATA" ) 
      {
        int   n = 0; 
        const int  nmax = 300 ;
        double x[nmax] , y[nmax] , ex[nmax] , ey[nmax] ;
  
        ifstream mydata ;
        mydata.open (fileName);          
        while (mydata >> x[n] >> y[n] >> ey[n] ) { ex[n] = 0. ; n++; }
        mydata.close();
 
        // Factor
        cout << dataSetFactor.size() << " " << iData << endl;
        if ( (signed) dataSetFactor.size() > iData ) {
          cout << "ScaleFactor = " << dataSetFactor.at(iData) << endl ;
          for ( int i=0 ; i<n ; ++i ) {
              y [i] *= dataSetFactor.at(iData);
             ey [i] *= dataSetFactor.at(iData);
          }
        } 
 
        gData.at(iData) = new TGraphAsymmErrors(n,x,y,ex,ex,ey,ey);
      }  
  
      if (   dataSetHisto.at(iData) == "UA5"   || dataSetHisto.at(iData) == "UA1" 
          || dataSetHisto.at(iData) == "ALICE" || dataSetHisto.at(iData) == "TRACKLETS" )
      { 

        int   n = 0;
        const int  nmax = 300 ;
        double x[nmax], xl[nmax] , xh[nmax] , y[nmax] , ex[nmax] , eyl[nmax] , eyh[nmax] , wh[nmax] ;
        double syl[nmax] , syh[nmax] ;     
 
        ifstream mydata ;
        mydata.open (fileName);

        if ( dataSetHisto.at(iData) == "UA5" ) {
         while (mydata >>  xl[n] >> xh[n] >> y[n] >> eyh[n] >> eyl[n] ) {
          ex[n] = 0.;
          eyl[n] = -eyl[n] ;
          x[n]  = xl[n]+(xh[n]-xl[n])/2;

          // div by binwidth
          if ( true ) {
            float ww = 1+ xh[n]-xl[n] ; 
             y [n] /= ww ;
            eyl[n] /= ww ;
            eyh[n] /= ww ;
          }
          n++;
         }
        }

        if ( dataSetHisto.at(iData) == "UA1" ) {
         while (mydata >>  xl[n] >> xh[n] >> y[n] >> eyh[n] >> eyl[n] >> wh[n] ) {
          ex[n] = 0.;
          eyl[n] = -eyl[n] ;
          x[n]  = xl[n]+(xh[n]-xl[n])/2;

          // div by binwidth
          if ( true ) {
             y [n] /= wh[n] ;
            eyl[n] /= wh[n] ;
            eyh[n] /= wh[n] ;
          }
          n++;
         }
        }

        if ( dataSetHisto.at(iData) == "ALICE" ) {
          while (mydata >>  xl[n] >> xh[n] >> y[n] >> eyh[n] >> eyl[n] >> syh[n] >> syl[n] ) { 
            //cout << " " <<  xl[n] << " " << xh[n] << " " << y[n] << " " << eyh[n] << " " << eyl[n] << " " << syl[n] << " " << syh[n] << endl;
            ex[n] = 0.;
            x[n]  = xl[n]+(xh[n]-xl[n])/2;
            eyh[n] = fabs ( eyh[n] );
            eyl[n] = fabs ( eyl[n] );
            syh[n] = fabs ( syh[n] );
            syl[n] = fabs ( syl[n] );
            if ( sData.at(iData) ) {
              eyh[n] =    sqrt( eyh[n]*eyh[n] + syh[n]*syh[n] ) ;
              eyl[n] =    sqrt( eyl[n]*eyl[n] + syl[n]*syl[n] ) ;              
            }
            cout << " " << eyh[n] << " " << eyl[n] << endl;
            n++;
          }
        }

        if ( dataSetHisto.at(iData) == "TRACKLETS" ) {
           while (mydata >>  xl[n] >> x[n] >> y[n] >> eyh[n] >> syh[n] ) {
             ex[n] = 0.;
             eyl[n] = eyh[n] ;
             syl[n] = syh[n] ;
             if ( sData.at(iData) ) {
              eyh[n] =    sqrt( eyh[n]*eyh[n] + syh[n]*syh[n] ) ;
              eyl[n] =    sqrt( eyl[n]*eyl[n] + syl[n]*syl[n] ) ; 
             }             
             n++;
           }
        }

        mydata.close();

        // Factor
        cout << dataSetFactor.size() << " " << iData << endl;
        if ( (signed) dataSetFactor.size() > iData ) {
          cout << "ScaleFactor = " << dataSetFactor.at(iData) << endl ;
          for ( int i=0 ; i<n ; ++i ) {
              y [i] *= dataSetFactor.at(iData);
             eyl[i] *= dataSetFactor.at(iData);
             eyh[i] *= dataSetFactor.at(iData);
          }
        } 

        gData.at(iData) = new TGraphAsymmErrors(n,x,y,ex,ex,eyl,eyh);

      }

      // Plot Style
      gData.at(iData)->SetMarkerColor(dataSetColor.at(iData));
      gData.at(iData)->SetMarkerStyle(dataSetStyle.at(iData)); 
      gData.at(iData)->SetLineColor(dataSetColor.at(iData));

    }
  }

  // SerialBinKiller

  if ( (signed) BinKillStat.size() > 0 ) {
    for(int iData2Kill = 0 ; iData2Kill < (signed) BinKillStat.size() ; ++iData2Kill ) {
        cout << "SerialBinKiller: " << BinKillStat.at(iData2Kill) << " " << BinKillSyst.at(iData2Kill) << endl; 
        cout << "SerialBinKiller: " << tData.at(BinKillStat.at(iData2Kill)) << " " << tData.at(BinKillSyst.at(iData2Kill)) << endl; 

        if (   tData.at(BinKillStat.at(iData2Kill)) == 1 &&    tData.at(BinKillSyst.at(iData2Kill)) == 5 )
           SerialBinKiller(hData.at(BinKillStat.at(iData2Kill)),gData.at(BinKillSyst.at(iData2Kill)));
        if (   tData.at(BinKillStat.at(iData2Kill)) == 5 &&    tData.at(BinKillSyst.at(iData2Kill)) == 5 )
           SerialBinKiller(gData.at(BinKillStat.at(iData2Kill)),gData.at(BinKillSyst.at(iData2Kill)));
    }
  }

  // Divide if requested and find hMax

  if ( globalRatioBase != -1 ) {
    if ( tData.at(globalRatioBase) == 1 ) hDividor = (TH1F*) hData.at(globalRatioBase)->Clone("hDividor");
    if ( tData.at(globalRatioBase) == 4 || tData.at(globalRatioBase) == 5 )
                              gDividor = (TGraphAsymmErrors*) gData.at(globalRatioBase)->Clone("gDividor");  
  } 

  for(int iData = 0 ; iData < (signed) dataSetId.size() ; ++iData) {
//    if ( rData.at(iData) ) 
    {

      // Divide by 1 of the histogram (same binning !)
      if ( globalRatioType > 0 ) {
        cout << "  Divide by 1 of the histogram " << endl;

        // TH1F and TGraph only FIXME: same binning !!!
        if ( (tData.at(iData) == 1 || tData.at(iData) == 4 || tData.at(iData) == 5 ) ) {
          if ( tData.at(globalRatioBase) == 1 ) {
            if ( tData.at(iData) == 1 ) {
               hDivData.at(iData) = (TH1F*) hData.at(iData)->Clone();
               for ( int ibin =1 ; ibin <= hDividor->GetNbinsX() ; ++ibin ) {
                 Double_t dividor = hDividor->GetBinContent(ibin);
                 if ( dividor != 0. ) {
                   hDivData.at(iData)->SetBinContent(ibin, hData.at(iData)->GetBinContent(ibin) / dividor );
                   hDivData.at(iData)->SetBinError  (ibin, hData.at(iData)->GetBinError(ibin)   / dividor );
                 } else {
                   hDivData.at(iData)->SetBinContent(ibin,0.);  
                   hDivData.at(iData)->SetBinError(ibin,0.);
                 }
               }
            } else {
               gDivData.at(iData) = (TGraphAsymmErrors*) gData.at(iData)->Clone();
               for ( int ibin =1 ; ibin <= hDividor->GetNbinsX() ; ++ibin ) {
                 Double_t dividor = hDividor->GetBinContent(ibin);
                 if ( dividor != 0. ) {
                   Double_t x,y;
                   gDivData.at(iData)->GetPoint(ibin-1,x,y);
                   gDivData.at(iData)->SetPoint(ibin-1,x,y / dividor);
                   gDivData.at(iData)->SetPointEYhigh(ibin-1, gDivData.at(iData)->GetErrorYhigh(ibin-1) / dividor );
                   gDivData.at(iData)->SetPointEYlow (ibin-1, gDivData.at(iData)->GetErrorYlow(ibin-1)  / dividor );
                 } else {
                   gDivData.at(iData)->SetPoint(ibin-1,0,0);
                   gDivData.at(iData)->SetPointEYhigh(ibin-1,0);
                   gDivData.at(iData)->SetPointEYlow(ibin-1,0);
                 }
               }    
            }
          } else {
            if ( tData.at(iData) == 1 ) {
               hDivData.at(iData) = (TH1F*) hData.at(iData)->Clone();
               for ( int ibin =1 ; ibin <= gDividor->GetN() ; ++ibin ) {
                 Double_t xx,dividor;
                 gDividor->GetPoint(ibin-1,xx,dividor);
                 if ( dividor != 0. ) {
                   hDivData.at(iData)->SetBinContent(ibin, hData.at(iData)->GetBinContent(ibin) / dividor );
                   hDivData.at(iData)->SetBinError  (ibin, hData.at(iData)->GetBinError(ibin)   / dividor );
                 } else {
                   hDivData.at(iData)->SetBinContent(ibin,0.);
                   hDivData.at(iData)->SetBinError(ibin,0.);
                 }
               }
            } else { 
               gDivData.at(iData) = (TGraphAsymmErrors*) gData.at(iData)->Clone();
               for ( int ibin =1 ; ibin <= gDividor->GetN() ; ++ibin ) {
                 Double_t xx,dividor;
                 gDividor->GetPoint(ibin-1,xx,dividor);
                 if ( dividor != 0. ) {
                   Double_t x,y;
                   gDivData.at(iData)->GetPoint(ibin-1,x,y);
                   gDivData.at(iData)->SetPoint(ibin-1,x,y / dividor);
                   gDivData.at(iData)->SetPointEYhigh(ibin-1, gDivData.at(iData)->GetErrorYhigh(ibin-1) / dividor );
                   gDivData.at(iData)->SetPointEYlow (ibin-1, gDivData.at(iData)->GetErrorYlow(ibin-1)  / dividor );

                 } else {
                   gDivData.at(iData)->SetPoint(ibin-1,0,0);
                   gDivData.at(iData)->SetPointEYhigh(ibin-1,0);
                   gDivData.at(iData)->SetPointEYlow(ibin-1,0);
                 }
               }
            } 
          }
        } else {
           cout << "[plot] ERROR: Divide possible only for TH1F and TGraph" << endl;
           return; 
        }  

      }

      // Divide by fit to 1 of the histogram (allow different binning !)
      if ( globalRatioType == 2  ) {
        cout << "  Divide by 1 of the histogram " << endl;
        //f1 = new TF1("f1","[0] + [1] * x + [2] * x*x + [3] * x*x*x + [4] * x*x*x*x + [5] * pow(x,5) + [6] * pow(x,6) + [7] * pow(x,7) + [8] * pow(x,8) + [9] * pow(x,9) ",1.,180.);
        //f1 = new TF1("f1","[0] + [1] * x + [2] * x*x + [3] * x*x*x  ",.5,150.);
        f1 = new TF1("f1",funcDoubleNBD , 2.5 , 180.5 , 6 );
        f1->SetParameters(1,.5,12.,3.,30.,7.);

         
        if ( tData.at(globalRatioBase) == 1 ) {
          hDividor->Fit("f1","R");
        } else {
          gDividor->Fit("f1","R");
        }   
 
        if ( tData.at(iData) == 1 ) {
          hDivData.at(iData) = (TH1F*) hData.at(iData)->Clone();
          for ( int ibin =1 ; ibin <= hDividor->GetNbinsX() ; ++ibin ) { 
            Double_t dividor = 1.;
            if (ibin < 20 ) {
              if ( tData.at(globalRatioBase) == 1 ) {
                dividor = hDividor->GetBinContent(ibin);
              } else {
                Double_t xx;
                gDividor->GetPoint(ibin-1,xx,dividor);
              }
            } else { 
              dividor = f1->Eval(hData.at(iData)->GetBinCenter(ibin));
            }
            if ( dividor != 0. ) {
              hDivData.at(iData)->SetBinContent(ibin, hData.at(iData)->GetBinContent(ibin) / dividor );
              hDivData.at(iData)->SetBinError  (ibin, hData.at(iData)->GetBinError(ibin)   / dividor );
            } else {
              hDivData.at(iData)->SetBinContent(ibin,0.);
              hDivData.at(iData)->SetBinError(ibin,0.);
            }
          }
        } else {  
               gDivData.at(iData) = (TGraphAsymmErrors*) gData.at(iData)->Clone();
               for ( int ibin =1 ; ibin <= hDividor->GetNbinsX() ; ++ibin ) {
                 Double_t xx,yy,dividor;
                 if (ibin < 20 ) {
                   if ( tData.at(globalRatioBase) == 1 ) {
                     dividor = hDividor->GetBinContent(ibin);
                   } else {
                     gDividor->GetPoint(ibin-1,xx,dividor);
                   }
                 } else { 
                   gDivData.at(iData)->GetPoint(ibin-1,xx,yy); 
                   dividor = f1->Eval(xx);
                 }
                 if ( dividor != 0. ) {
                   Double_t x,y;
                   gDivData.at(iData)->GetPoint(ibin-1,x,y);
                   gDivData.at(iData)->SetPoint(ibin-1,x,y / dividor);
                   gDivData.at(iData)->SetPointEYhigh(ibin-1, gDivData.at(iData)->GetErrorYhigh(ibin-1) / dividor );
                   gDivData.at(iData)->SetPointEYlow (ibin-1, gDivData.at(iData)->GetErrorYlow(ibin-1)  / dividor );
                 } else {
                   gDivData.at(iData)->SetPoint(ibin-1,0,0);
                   gDivData.at(iData)->SetPointEYhigh(ibin-1,0);
                   gDivData.at(iData)->SetPointEYlow(ibin-1,0);
                 }
               }
        }


      }

      // hMax
      if ( ! ( tData.at(iData) == 4 || tData.at(iData) == 5 ) ) {
        if ( hData.at(iData)->GetMaximum() > hMax ) hMax = hData.at(iData)->GetMaximum() ;
      } else {
        if ( gData.at(iData)->GetHistogram()->GetMaximum() > hMax ) hMax = gData.at(iData)->GetHistogram()->GetMaximum() ;
      }
    
    }
  } 



  // Global Style (histo)
  cout << " Global Style (histo) " << endl;
  // FIXME 
  if ( ! ( tData.at(0) == 4 || tData.at(0) == 5 ) ) {
  //if ( rData.at(0) ) {

    hData.at(0)->GetYaxis()->SetTitleOffset(1.1);
    hData.at(0)->GetXaxis()->SetTitleSize(globalAxisTitleSize);
    hData.at(0)->GetYaxis()->SetTitleSize(globalAxisTitleSize);
    if (globalRatioType>0) {
      if (XaxisTitle != "NONE" ) hDivData.at(0)->GetXaxis()->SetTitle(XaxisTitle);
      hData.at(0)->GetXaxis()->SetTitle("");
    } else { 
      if (XaxisTitle != "NONE" ) hData.at(0)->GetXaxis()->SetTitle(XaxisTitle);
    }
    if (YaxisTitle != "NONE" ) hData.at(0)->GetYaxis()->SetTitle(YaxisTitle);

    if(!( logY == 1 )) {
      hData.at(0)->SetMinimum(0);
      hData.at(0)->SetMaximum(hMax*1.1 );
    }
    if (histoYMin != histoYMax) hData.at(0)->GetYaxis()->SetRangeUser(histoYMin,histoYMax);
    if (histoXMin != histoXMax) hData.at(0)->GetXaxis()->SetRangeUser(histoXMin,histoXMax);
    if(histoXMax==999.)         hData.at(0)->GetXaxis()->SetRangeUser(histoXMin,getLastFilledBin(hData.at(0)));

    if (globalRatioType>0) {
      if (histoXMin != histoXMax) hDivData.at(0)->GetXaxis()->SetRangeUser(histoXMin,histoXMax);
      if(histoXMax==999.)         hDivData.at(0)->GetXaxis()->SetRangeUser(histoXMin,getLastFilledBin(hData.at(0)));
      
      hDivData.at(0)->GetYaxis()->SetRangeUser(0,4);
      hDivData.at(0)->GetYaxis()->SetTitle("Ratio");
    }

  // Global Style (TGraph) 
  } else {

    gData.at(0)->GetYaxis()->SetTitleOffset(1.1);
    gData.at(0)->GetXaxis()->SetTitleSize(globalAxisTitleSize);
    gData.at(0)->GetYaxis()->SetTitleSize(globalAxisTitleSize);
    if (globalRatioType>0) {
      if (XaxisTitle != "NONE" ) gDivData.at(0)->GetXaxis()->SetTitle(XaxisTitle);
      gData.at(0)->GetXaxis()->SetTitle("");
    } else {  
      if (XaxisTitle != "NONE" ) gData.at(0)->GetXaxis()->SetTitle(XaxisTitle);
    }

    if (YaxisTitle != "NONE" ) gData.at(0)->GetYaxis()->SetTitle(YaxisTitle);

    if(!( logY  == 1)) {
      gData.at(0)->SetMinimum(0);
      gData.at(0)->SetMaximum(hMax*1.1 );
    }
    if (histoYMin != histoYMax) gData.at(0)->GetYaxis()->SetRangeUser(histoYMin,histoYMax);
    if (histoXMin != histoXMax) gData.at(0)->GetXaxis()->SetRangeUser(histoXMin,histoXMax);
    if(histoXMax==999.)         gData.at(0)->GetXaxis()->SetRangeUser(histoXMin,getLastFilledBin(gData.at(0)));
    if (globalRatioType>0) {
      if (histoXMin != histoXMax) gDivData.at(0)->GetXaxis()->SetRangeUser(histoXMin,histoXMax);
      if(histoXMax==999.)         gDivData.at(0)->GetXaxis()->SetRangeUser(histoXMin,getLastFilledBin(gData.at(0)));
      gDivData.at(0)->GetYaxis()->SetRangeUser(0,2);
      gDivData.at(0)->GetYaxis()->SetTitle("Ratio");
    }



  }


  TString opt;
  for(int iData = 0 ; iData < (signed) dataSetId.size() ; ++iData) {

    // TH1F Plot
    if ( tData.at(iData) == 1 )
    {
      if ( dataSetIsMc.at(iData) )  opt  = "hist"; 
      else                          opt  = "e1";
      if ( iData > 0 )              opt += "same";
      if ( rData.at(iData) ) hData.at(iData)->Draw(opt);
    }

    // TH2F Plot // Only 1 TH2F  possible ! 
    else if ( tData.at(iData) == 2 && dataSetId.size() == 1 )
    {
      if ( rData.at(iData) ) hData.at(iData)->Draw(global2DplotOpt);
    }
    
    // TGraph
    else if ( tData.at(iData) == 4 || tData.at(iData) == 5 )
    {
      if ( sData.at(iData) == 0 ) opt  = "p" ;  // stat error style
      if ( sData.at(iData) == 1 ) opt  = "pZ" ; // systematics error style
      if ( iData == 0 ) opt += "a";
      gData.at(iData)->Draw(opt);
    }
  } 

   if ( globalRatioType == 2  ) f1->Draw("same"); 

  // Bottom: Divided panel
  if (globalRatioType>0) { 
    cout << " Divided panel Plotting " << endl;

    p2->cd();
    for(int iData = 0 ; iData < (signed) dataSetId.size() ; ++iData) {
  
      // TH1F Plot
      if ( tData.at(iData) == 1 )
      {
        if ( dataSetIsMc.at(iData) )  opt  = "hist"; 
        else                          opt  = "e1";
        if ( iData > 0 )              opt += "same";
        if ( rData.at(iData) ) hDivData.at(iData)->Draw(opt);
      }
  
      // TH2F Plot // Only 1 TH2F  possible ! 
      else if ( tData.at(iData) == 2 && dataSetId.size() == 1 )
      {
        if ( rData.at(iData) ) hDivData.at(iData)->Draw(global2DplotOpt);
      }
      
      // TGraph
      else if ( tData.at(iData) == 4 || tData.at(iData) == 5 )
      {
        if ( sData.at(iData) == 0 ) opt  = "p" ;  // stat error style
        if ( sData.at(iData) == 1 ) opt  = "pZ" ; // systematics error style
        if ( iData == 0 ) opt += "a";
          gDivData.at(iData)->Draw(opt);
      }
  
    } 

    p1->cd();
  } 


/*  if ( tData.at(iData) == 1 ) 
  {
    for(int iData = 0 ; iData < (signed) dataSetId.size() ; ++iData) {
      if ( dataSetIsMc.at(iData) )  opt  = "hist"; 
      else                          opt  = "e";
      if ( iData > 0 )              opt += "same";
      if ( rData.at(iData) ) hData.at(iData)->Draw(opt);
    }
    for(int iData = 0 ; iData < (signed) dataSetId.size() ; ++iData) {
      if ( ! dataSetIsMc.at(iData) ) 
      {
        if ( rData.at(iData) ) { hData.at(iData)->Draw("esame"); }
        else                   { gData.at(iData)->Draw("psame"); }  
      }
    }
  }

  // TH2F Plot
  if ( tData.at(iData) == 2 )
  {
    if ( dataSetId.size() == 1 )
      if ( rData.at(0) ) hData.at(0)->Draw(global2DplotOpt);
  }
*/

  // Legend


  int LegendSize = 1;
  for(int iData = 0 ; iData < (signed) dataSetId.size() ; ++iData) 
    if ( dataSetLegend.at(iData) != "NONE" ) ++LegendSize;

  TLegend *leg = new TLegend (xLegendMin[iLegendPos] ,
                              yLegendMax[iLegendPos] - yLegendWidth * LegendSize  ,
                              xLegendMin[iLegendPos] + xLegendWidth ,
                              yLegendMax[iLegendPos] );
  if ( LegendTitle != "NONE")  leg->SetHeader(LegendTitle);
  for(int iData = 0 ; iData < (signed) dataSetId.size() ; ++iData) {
    if ( tData.at(iData) != 2 ) {
      if ( dataSetIsMc.at(iData) )  opt  = "l"; 
      else                          opt  = "p";
    } else
      opt  = "box";
    
    if ( dataSetLegend.at(iData) != "NONE" ) 
    { 
      //if ( rData.at(iData) ) leg->AddEntry(hData.at(iData),dataSetLegend.at(iData),opt );
      //else                   leg->AddEntry(gData.at(iData),dataSetLegend.at(iData),opt );
      if ( rData.at(iData) ) {
        if ( ! ( tData.at(iData) == 4 || tData.at(iData) == 5 ) ) 
          leg->AddEntry(hData.at(iData),dataSetLegend.at(iData),opt );
        else 
          leg->AddEntry(gData.at(iData),dataSetLegend.at(iData),opt );
      } else {
          leg->AddEntry(gData.at(iData),dataSetLegend.at(iData),opt );
      } 
    }
  }
  leg->SetBorderSize(0);
  leg->SetFillColor(0);
  leg->SetTextSizePixels(globalLegendTextSize); 
  leg->Draw();

  TLatex* ExtLT = new TLatex( xLegendMin[iLegendPos] , yLegendMax[iLegendPos] + yLegendWidth , ExtLegTitle  );
  if (ExtLegTitle != "NONE") {
    ExtLT->SetNDC(kTRUE);
    ExtLT->SetTextSize(0.023);
    ExtLT->Draw();
  }


  cout << textText.size() << " " << textXPos.size() << " " << textYPos.size() << endl;
  for ( int iText = 0 ; iText < (signed) textText.size() ; ++iText ){

    Text.at(iText) = new TLatex( textXPos.at(iText) , textYPos.at(iText) ,textText.at(iText) );
    Text.at(iText)->SetNDC(kTRUE);
    Text.at(iText)->SetTextSize(globalTextSize);
    Text.at(iText)->Draw();  

  }

  // 
  //TLatex* txt=new TLatex(.65, 0.96, "CMS 0.9 TeV");
  TLatex* txt=new TLatex(xGlobalLabel,yGlobalLabel,globalLabel);
  txt->SetNDC(kTRUE);
  txt->SetTextSize(0.04);
  txt->Draw();

  // Save Plot
  string basedir ( globalFigDir );
  stringstream out("");
  out<< globalEnergy << "Tev/";
  basedir += out.str();
  string sdir    (dir);
  string shisto  (histo);
  if ( histo == "AUTO") shisto = globalFigBaseName;
  
  if (globalWait) gPad->WaitPrimitive();
  
  bool save = globalSaveFig;
  if(save){

     string fneps ("");
     fneps += basedir;
     fneps += "eps/";  
     fneps += sdir;
     fneps += "/";
  
     if (!gSystem->OpenDirectory(fneps.c_str())) gSystem->mkdir(fneps.c_str(),true);
     fneps += shisto;
     fneps += "_" + ptcutstr;
     if(logY) fneps +="_logY";
     fneps += ".eps";
     c1->SaveAs(fneps.c_str(),"");
  
    
     string fngif ("");
     fngif += basedir;
     fngif += "gif/"; 
     fngif += sdir;
     fngif += "/";
  
     if (!gSystem->OpenDirectory(fngif.c_str())) gSystem->mkdir(fngif.c_str(),true);
     fngif += shisto;
     fngif += "_" + ptcutstr;
     if(logY) fngif +="_logY";
     fngif += ".gif";
     c1->SaveAs(fngif.c_str(),"");
  
     string fnpdf ("");
     fnpdf += basedir;
     fnpdf += "pdf/"; 
     fnpdf += sdir;
     fnpdf += "/";
  
     if (!gSystem->OpenDirectory(fnpdf.c_str())) gSystem->mkdir(fnpdf.c_str(),true);
     fnpdf += shisto;
     fnpdf += "_" + ptcutstr;
     if(logY) fnpdf +="_logY";
     fnpdf += ".pdf";
     string command("convert ") ;
     command += fneps + " " + fnpdf ;
     gSystem->Exec(command.c_str());
     //c1->SaveAs(fnpdf.c_str(),"");
  
  /*  
    {
     string fngif ("");
     fngif += basedir;
     fngif += "root/";
     fngif += sdir;
  
     fngif += "/";
     if (!gSystem->OpenDirectory(fngif.c_str())) gSystem->mkdir(fngif.c_str(),true);
     fngif += shisto;
     fngif += "_" + ptcutstr;
     if(logY) fngif +="_logY";
     fngif += ".root";
     c1->SaveAs(fngif.c_str(),"");
    }
  */
  
  }

  for(int iData = 0 ; iData < (signed) dataSetId.size() ; ++iData) 
    if ( rData.at(iData) ) fData.at(iData)->Close(); 

  // delete c1;
  // return 1;
}  
