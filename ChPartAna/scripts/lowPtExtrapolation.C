#include <TROOT.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TGraph.h>
#include <TString.h>
#include <TProfile.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TPad.h>
#include <TFrame.h>
#include <TFile.h>
#include <TDirectory.h>
#include <TLegend.h>
#include <TText.h>
#include <TMath.h>
#include <TF1.h>
#include <TGraphAsymmErrors.h>
#include <TLatex.h>

#include <TSystem.h>

#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;
using namespace TMath;

#include "../macro/fileManager.C"

void BinWidthDiv ( TH1 *&h ) {
  for ( int i =1 ; i<=h->GetNbinsX() ; ++i ) {
    h->SetBinContent(i, h->GetBinContent(i) / h->GetXaxis()->GetBinWidth(i) ) ;
    h->SetBinError(i, h->GetBinError(i) / h->GetXaxis()->GetBinWidth(i) ) ;
  }
}

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

void lowPtExtrapolation(int iBin = 5 , double E = 0.9 ) {

  int iTr = 1 ;
  int typeMC ; 
  if ( E == 0.9 ) typeMC = 10 ;
  if ( E == 2.36) typeMC = 10 ;
  if ( E == 7.0 ) typeMC = 31 ;

  TString filedir("../plots/systv16/");
  ostringstream datastr("");
  ostringstream mcstr("");
  datastr << "hyp" << 1 << "_niter" << 0 << "_cut" << iBin << "_DataType" << 0; 
  mcstr   << "hyp" << 1 << "_niter" << 0 << "_cut" << iBin << "_DataType" <<typeMC ; 

  TString datafile = fileManager(3,typeMC,E,iTr,0,0,datastr.str(),filedir); 
  TString mcfile   = fileManager(3,typeMC,E,iTr,0,0,mcstr.str()  ,filedir); 
 
  cout << "Data: " << datafile << endl ;
  cout << "MoCa: " << mcfile << endl ;

  TFile* data  = TFile::Open(datafile,"READ");
  //TFile* mc    = TFile::Open(mcfile,"READ");

  TGraphAsymmErrors* dptpub = NULL;
  if (iBin == 5)
  {
     int   n = 0; 
     const int  nmax = 50 ;
     double x[nmax] , xl[nmax] , xh[nmax] , y[nmax] , ex[nmax] , eyl[nmax] , eyh[nmax] ;
     ifstream mydata ;
     mydata.open ("../expdata/cms10_dndpt_7.0TeV");
     while (mydata >> xl[n] >> xh[n] >> y[n] >> eyh[n] >> eyl[n] ) {
        ex[n]   = (xh[n]-xl[n])/2. ;
        x[n]    = xl[n]+(xh[n]-xl[n])/2.;
        y[n]   *=  2*3.1416*x[n] ;
        eyh[n] *=  2*3.1416*x[n] ;
        eyl[n] *= -2*3.1416*x[n] ;
        n++;
     }     
     mydata.close();
     dptpub  = new TGraphAsymmErrors(n,x,y,ex,ex,eyh,eyh);  
     Double_t scale = TGIntegral(dptpub , 0 , n );
     cout << "Int: " << scale << endl;
     TGAsymScale(dptpub,1./515.);
  } 

  // Pt 

  TH1* ptdata   = (TH1F*) data->Get("pt/pt_corrected");
  //TH1* ptmccor  = (TH1F*) mc->Get("pt/pt_corrected");
  //TH1* ptmcgen  = (TH1F*) mc->Get("pt/pt_MC_gen_afterCorrection");
  TH1* ptmcgen  = (TH1F*) data->Get("pt/pt_MC_gen_afterCorrection");
  BinWidthDiv(ptdata);
  //BinWidthDiv(ptmccor);
  BinWidthDiv(ptmcgen);

  ptdata->Scale(1/ptdata->Integral());
  //ptmccor->Scale(1/ptmccor->Integral());
  ptmcgen->Scale(1/ptmcgen->Integral());

  TCanvas* cpt = new TCanvas("cpt","cpt",500,500);
  cpt->SetFillColor(0);
  cpt->GetFrame()->SetFillColor(21);
  cpt->GetFrame()->SetBorderSize(12);
  cpt->SetGrid(0,0);
  cpt->SetLeftMargin(0.17);
  cpt->cd();
  

  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(kFALSE);

  ptdata->GetYaxis()->SetTitleOffset(2.0);
  ptdata->GetYaxis()->SetTitle("d#sigma/dp_{T} (a.u.)");
  ptdata->SetMarkerStyle(20);
  ptdata->SetMarkerSize(0.6);
  ptdata->SetMarkerColor(kRed);
  ptmcgen->SetMarkerStyle(20);
  ptmcgen->SetMarkerSize(0.5);
  ptmcgen->SetLineColor(kBlack);
  


  ptdata->GetXaxis()->SetRangeUser(0.,1.5);

  ptdata->Draw("");
  //dptpub->Draw("psame");
  ptmcgen->Draw("samehist"); 

  TF1* FitPtData = new TF1("FitPtData","[0] *x* exp([1]*x+[2]*x*x+0.1396*x*x*x+[4]*x*x*x*x)" , 0.1 , .5 );
  TF1* FitPtMC   = new TF1("FitPtMC"  ,"[0] *x* exp([1]*x+[2]*x*x+0.1396*x*x*x+[4]*x*x*x*x)" , 0.1 , .5 );
  FitPtData->SetParameters(100.,-4.,0.3,0.,0.);
  FitPtMC->SetParameters(100.,-4.,0.3,0.,0.);

  ptdata->Fit("FitPtData","R0");
  ptmcgen->Fit("FitPtMC","R0");

  FitPtData->SetLineWidth(2);
  FitPtData->SetLineColor(kBlue);
  FitPtData->Draw("same");

  //FitPtMC->Draw("same");

  TF1* CloneFitPtData = (TF1*) FitPtData->Clone("CloneFitPtData");
  TF1* CloneFitPtMC   = (TF1*) FitPtMC->Clone("CloneFitPtMC");

  CloneFitPtData->SetLineStyle(2);
  CloneFitPtMC->SetLineStyle(2);
  CloneFitPtData->SetRange(0.,0.1);
  CloneFitPtMC->SetRange(0.,0.1);
  CloneFitPtData->Draw("same");
//  CloneFitPtMC->Draw("same");

  // Pt Extrapolation

  Double_t DataInt   = ptdata->Integral(11,ptdata->GetNbinsX(),"width");
  Double_t MCInt     = ptmcgen->Integral(11,ptmcgen->GetNbinsX(),"width");
  Double_t MCIntALL  = ptmcgen->Integral( 1,ptmcgen->GetNbinsX(),"width");
  Double_t MCIntLow  = ptmcgen->Integral( 1,10,"width");

  Double_t DataExtra = FitPtData->Integral(0.,.100);
  Double_t MCExtra   = FitPtMC->Integral(0.,.100);

  cout << "DataInt   = " << DataInt << endl;
  cout << "DataExtra = " << DataExtra  << endl;
  cout << "LowPtExtapol --> " << DataExtra / (DataExtra+DataInt) << endl;

  cout << "MCInt   = " << MCInt << endl;
  cout << "MCExtra = " << MCExtra  << endl;
  cout << "LowPtExtapol --> " << MCExtra / (MCExtra+MCInt) << endl;
  cout << "MCIntALL= " << MCIntALL << endl;
  cout << "MCIntLow= " << MCIntLow << endl;
  cout << "LowPtExtapol --> " << MCIntLow / MCIntALL << endl;

  cout << "Difference = " <<  DataExtra / (DataExtra+DataInt) -  MCIntLow / MCIntALL  << endl;

  // Average Pt Correction

  ptdata->GetXaxis()->SetRangeUser(0.,10.);
  TH1* ptdataextra = (TH1F*) ptdata->Clone("ptdataextra"); 
  TH1* ptmcgencut  = (TH1F*) ptmcgen->Clone("ptmcgencut");
  for ( int i =1 ; i<= 10 ; ++i ) { 
    ptdataextra->SetBinContent(i,FitPtData->Eval(ptdataextra->GetBinCenter(i))); 
    ptmcgencut->SetBinContent(i,0.);
    ptmcgencut->SetBinError(i,0.);
  }
  cout << "Data <pt> ALL = " << ptdataextra->GetMean() << endl ;
  cout << "Data <pt> CUT = " << ptdata->GetMean() << endl ;
  Double_t mpt_cor_data = ptdataextra->GetMean()  / ptdata->GetMean() ;
  cout << " --> Low Pt Corr = " << ptdataextra->GetMean()  / ptdata->GetMean() << endl ;
  cout << "MC   <pt> ALL = " << ptmcgen->GetMean() << endl ;
  cout << "MC   <pt> CUT = " << ptmcgencut->GetMean() << endl ;
  Double_t mpt_cor_mc = ptmcgen->GetMean()  / ptmcgencut->GetMean() ; 
  cout << " --> Low Pt Corr = " << ptmcgen->GetMean()  / ptmcgencut->GetMean() << endl ;
  ptdata->GetXaxis()->SetRangeUser(0.,1.5);

  // Legends

  TLatex* txt = NULL ;
  if ( E== 0.9 ) txt = new TLatex(.65, 0.85, "0.9 TeV");
  if ( E== 2.36) txt = new TLatex(.65, 0.85, "2.36 TeV");
  if ( E== 7.0 ) txt = new TLatex(.65, 0.85, "7.0 TeV");
  txt->SetNDC(kTRUE);
  txt->SetTextSize(0.04);
  txt->Draw();

  TLatex* txt2 = NULL ;
  if ( iBin == 5 )  txt2 = new TLatex(.65, 0.80, "|#eta| < 2.4");
  if ( iBin == 6 )  txt2 = new TLatex(.65, 0.80, "|#eta| < 2.0");
  if ( iBin == 7 )  txt2 = new TLatex(.65, 0.80, "|#eta| < 1.5");
  if ( iBin == 8 )  txt2 = new TLatex(.65, 0.80, "|#eta| < 1.0");
  if ( iBin == 9 )  txt2 = new TLatex(.65, 0.80, "|#eta| < 0.5");
  txt2->SetNDC(kTRUE);
  txt2->SetTextSize(0.04);
  txt2->Draw();

  TLegend *leg = new TLegend ( 0.6 , 0.60 , 0.85 , 0.75 ) ; 
  
  leg->AddEntry(ptdata,"Data","p");
  leg->AddEntry(ptdata,"MC","l");
  leg->AddEntry(FitPtData,"Fit","l");
  leg->AddEntry(CloneFitPtData,"Extrapolation","l");

  leg->SetBorderSize(0);
  leg->SetFillColor(0);
  leg->SetTextSizePixels(.3); 
  leg->Draw();

  TLatex* txt0 =  new TLatex(0.55,0.55,"Hadrons below 100 MeV/c:");
  txt0->SetNDC(kTRUE);
  txt0->SetTextSize(0.03);
  txt0->Draw();


  ostringstream func("");
  func.str("");
  func << "Data = " <<  100*(DataExtra / (DataExtra+DataInt)) << " %"; 
  TLatex* txt3 =  new TLatex(0.6,0.51,func.str().c_str());
  txt3->SetNDC(kTRUE);
  txt3->SetTextSize(0.03);
  txt3->Draw();

  func.str("");
  func << " MC  = " << 100*(MCIntLow / MCIntALL) << " %"; 
  TLatex* txt4 =  new TLatex(0.6,0.47,func.str().c_str());
  txt4->SetNDC(kTRUE);
  txt4->SetTextSize(0.03);
  txt4->Draw();

  TLatex* txt5 =  new TLatex(0.55,0.43,"<p_{T}> Correction Factor: ");
  txt5->SetNDC(kTRUE);
  txt5->SetTextSize(0.03);
  txt5->Draw();

  func.str("");
  func << "Data = " << mpt_cor_data  ;
  TLatex* txt6 =  new TLatex(0.6,0.39,func.str().c_str());
  txt6->SetNDC(kTRUE);
  txt6->SetTextSize(0.03);
  txt6->Draw();

  func.str("");
  func << " MC  = " << mpt_cor_mc    ;
  TLatex* txt7 =  new TLatex(0.6,0.35,func.str().c_str());
  txt7->SetNDC(kTRUE);
  txt7->SetTextSize(0.03); 
  txt7->Draw();

  ostringstream basefig("");
  basefig<< "../figs/ptExtrapol_" ;
  if ( E== 0.9 ) basefig << "900GeV_" ;
  if ( E== 2.36) basefig << "2360GeV_" ;
  if ( E== 7.0 ) basefig << "7000GeV_" ;
  basefig<< "bin" << iBin ;

  

  gPad->SaveAs(basefig.str()+TString(".gif"),"");

}
