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
  TCanvas* c1 = new TCanvas("c1","c",200,10,500,500);
  c1->SetLeftMargin(0.17);
  c1->SetBottomMargin(0.10);
  c1->SetFillColor(0);
  c1->GetFrame()->SetFillColor(21);
  c1->GetFrame()->SetBorderSize(12);
  c1->SetGrid(0,0);

  if(logY == 1 ) c1->SetLogy(true);
  if(logY == 2 ) c1->SetLogx(true);

  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(kFALSE);

  Float_t hMax = 0 ;

  vector<bool>            rData ( dataSetId.size() , 1    ); 
  vector<TFile*>          fData ( dataSetId.size() , NULL );
  vector<TH1*>            hData ( dataSetId.size() , NULL );
  vector<TGraph*>         gData ( dataSetId.size() , NULL );

  for(int iData = 0 ; iData < (signed) dataSetId.size() ; ++iData) { 
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
      if ( dataSetHisto.at(iData) == "EXTDATA" || dataSetHisto.at(iData) == "UA5" )  rData.at(iData) = 0 ; //isRootData = false;
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
  
      if ( globalHistoType == 1 ) hData.at(iData) = (TH1F*) fData.at(iData)->Get(histoName);
      if ( globalHistoType == 2 ) hData.at(iData) = (TH2F*) fData.at(iData)->Get(histoName);
  
      if ( hData.at(iData) == 0 ) {
        cout << "[plot] Histo does not exist " << endl;
        return;
      }
   
      // Plot Style
      if ( ! dataSetIsMc.at(iData) ) {
        hData.at(iData)->SetMarkerColor(dataSetColor.at(iData));
        hData.at(iData)->SetMarkerStyle(dataSetStyle.at(iData)); 
        hData.at(iData)->SetLineColor(dataSetColor.at(iData));
      } else {
        hData.at(iData)->SetLineWidth(2);
        hData.at(iData)->SetLineColor(dataSetColor.at(iData));
        hData.at(iData)->SetLineStyle(dataSetStyle.at(iData));
      }
      // Normalisation
      if ( iData>0 && globalNorm == 1 ) {
        Float_t ndata = hData.at(0)->Integral(1, hData.at(0)->GetNbinsX() );
        Float_t nmoca = hData.at(iData)->Integral(1, hData.at(iData)->GetNbinsX() );
        hData.at(iData)->Scale(ndata/nmoca);
      }
      if ( globalNorm == 2 ) {
        Float_t ndata = hData.at(iData)->Integral(1, hData.at(iData)->GetNbinsX() );
        hData.at(iData)->Scale(1/ndata,"width"); 
      }
      if ( globalNorm == 3 ) { // Excluding bin 0 for normalization
        Float_t ndata = hData.at(iData)->Integral(2, hData.at(iData)->GetNbinsX() );
        hData.at(iData)->Scale(1/ndata,"width"); 
      }
      // Factor
      cout << dataSetFactor.size() << " " << iData << endl;
      if ( (signed) dataSetFactor.size() > iData ) {
        cout << "ScaleFactor = " << dataSetFactor.at(iData) << endl ; 
        hData.at(iData)->Scale(dataSetFactor.at(iData));
      }
      // hMax
      if ( hData.at(iData)->GetMaximum() > hMax ) hMax = hData.at(iData)->GetMaximum() ;

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
 
        gData.at(iData) = new TGraphErrors(n,x,y,ex,ey);
      }  
  
      if ( dataSetHisto.at(iData) == "UA5" )
      { 

        int   n = 0;
        const int  nmax = 300 ;
        double x[nmax], xl[nmax] , xh[nmax] , y[nmax] , ex[nmax] , eyl[nmax] , eyh[nmax] ;
      
        ifstream mydata ;
        mydata.open (fileName);
        while (mydata >>  xl[n] >> xh[n] >> y[n] >> eyh[n] >> eyl[n] ) {
          ex[n] = 0.;
          eyl[n] = -eyl[n] ;
          x[n]  = xl[n]+(xh[n]-xl[n])/2;
          n++;
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

  // Global Style (histo)
  if ( rData.at(0) ) {

    hData.at(0)->GetYaxis()->SetTitleOffset(2);
    if (XaxisTitle != "NONE" ) hData.at(0)->GetXaxis()->SetTitle(XaxisTitle);
    if (YaxisTitle != "NONE" ) hData.at(0)->GetYaxis()->SetTitle(YaxisTitle);

    if(!( logY == 1 )) {
      hData.at(0)->SetMinimum(0);
      hData.at(0)->SetMaximum(hMax*1.1 );
    }
    if (histoYMin != histoYMax) hData.at(0)->GetYaxis()->SetRangeUser(histoYMin,histoYMax);
    if (histoXMin != histoXMax) hData.at(0)->GetXaxis()->SetRangeUser(histoXMin,histoXMax);

  // Global Style (TGraph) 
  } else {

    gData.at(0)->GetYaxis()->SetTitleOffset(2);
    if (XaxisTitle != "NONE" ) gData.at(0)->GetXaxis()->SetTitle(XaxisTitle);
    if (YaxisTitle != "NONE" ) gData.at(0)->GetYaxis()->SetTitle(YaxisTitle);

    if(!( logY  == 1)) {
      gData.at(0)->SetMinimum(0);
      gData.at(0)->SetMaximum(hMax*1.1 );
    }
    if (histoYMin != histoYMax) gData.at(0)->GetYaxis()->SetRangeUser(histoYMin,histoYMax);
    if (histoXMin != histoXMax) gData.at(0)->GetXaxis()->SetRangeUser(histoXMin,histoXMax);
  }

  TString opt;
  // TH1F Plot
  if ( globalHistoType == 1 ) 
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
  if ( globalHistoType == 2 )
  {
    if ( dataSetId.size() == 1 )
      if ( rData.at(0) ) hData.at(0)->Draw(global2DplotOpt);
  }

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
    if ( globalHistoType == 1 ) {
      if ( dataSetIsMc.at(iData) )  opt  = "l"; 
      else                          opt  = "p";
    } else
      opt  = "box";
    if ( dataSetLegend.at(iData) != "NONE" ) 
    { 
      if ( rData.at(iData) ) leg->AddEntry(hData.at(iData),dataSetLegend.at(iData),opt );
      else                   leg->AddEntry(gData.at(iData),dataSetLegend.at(iData),opt );
    }
  }
  leg->SetBorderSize(0);
  leg->SetFillColor(0);
  leg->Draw();

  TText* ExtLT = new TText( xLegendMin[iLegendPos] , yLegendMax[iLegendPos] + yLegendWidth , ExtLegTitle  );
  if (ExtLegTitle != "NONE") {
    ExtLT->SetNDC(kTRUE);
    ExtLT->SetTextSize(0.023);
    ExtLT->Draw();
  }

  // 
  //TText* txt=new TText(.65, 0.96, "CMS 0.9 TeV");
  TText* txt=new TText(xGlobalLabel,yGlobalLabel,globalLabel);
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

    {
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
    }
  
    
    {
     string fngif ("");
     fngif += basedir;
     fngif += "eps/"; 
     fngif += sdir;
     fngif += "/";
  
     if (!gSystem->OpenDirectory(fngif.c_str())) gSystem->mkdir(fngif.c_str(),true);
     fngif += shisto;
     fngif += "_" + ptcutstr;
     if(logY) fngif +="_logY";
     fngif += ".eps";
     c1->SaveAs(fngif.c_str(),"");
    }
  
    {
     string fngif ("");
     fngif += basedir;
     fngif += "pdf/"; 
     fngif += sdir;
     fngif += "/";
  
     if (!gSystem->OpenDirectory(fngif.c_str())) gSystem->mkdir(fngif.c_str(),true);
     fngif += shisto;
     fngif += "_" + ptcutstr;
     if(logY) fngif +="_logY";
     fngif += ".pdf";
     c1->SaveAs(fngif.c_str(),"");
    }
  
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
