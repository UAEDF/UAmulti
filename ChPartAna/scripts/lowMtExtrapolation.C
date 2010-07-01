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

void lowMtExtrapolation(int iBin = 5 , double E = 0.9 ) {

  int iTr = 1 ;
  int typeMC ; 
  if ( E == 0.9 ) typeMC = 10 ;
  if ( E == 2.36) typeMC = 10 ;
  if ( E == 7.0 ) typeMC = 31 ;

  TString filedir("../plots/");
  ostringstream datastr("");
  ostringstream mcstr("");
  datastr << "hyp" << 1 << "_niter" << 0 << "_cut" << iBin << "_DataType" << 0; 
  mcstr   << "hyp" << 1 << "_niter" << 0 << "_cut" << iBin << "_DataType" <<typeMC ; 

  TString datafile = fileManager(3,typeMC,E,iTr,0,0,datastr.str(),filedir); 
  TString mcfile   = fileManager(3,typeMC,E,iTr,0,0,mcstr.str()  ,filedir); 
 
  cout << "Data: " << datafile << endl ;
  cout << "MoCa: " << mcfile << endl ;

  TFile* data  = TFile::Open(datafile,"READ");
  TFile* mc    = TFile::Open(mcfile,"READ");

  // Pt 

  TH1* ptdata   = (TH1F*) data->Get("mt/mt_corrected");
  TH1* ptmccor  = (TH1F*) mc->Get("mt/mt_corrected");
  TH1* ptmcgen  = (TH1F*) mc->Get("mt/mt_MC_gen_afterCorrection");
  BinWidthDiv(ptdata);
  BinWidthDiv(ptmccor);
  BinWidthDiv(ptmcgen);

  ptdata->Scale(1/ptdata->Integral());
  ptmccor->Scale(1/ptmccor->Integral());
  ptmcgen->Scale(1/ptmcgen->Integral());

  TCanvas* cpt = new TCanvas("cpt","cpt");
  ptdata->GetXaxis()->SetRangeUser(0.,1.5);
  ptdata->Draw();
  ptmcgen->Draw("same"); 

  TF1* FitPtData = new TF1("FitPtData","[0] *x* exp([1]*x+[2]*x*x+0.1396*x*x*x+[4]*x*x*x*x)" , 0.04 , .2 );
  TF1* FitPtMC   = new TF1("FitPtMC"  ,"[0] *x* exp([1]*x+[2]*x*x+0.1396*x*x*x+[4]*x*x*x*x)" , 0.04 , .2 );
  FitPtData->SetParameters(100.,-4.,0.3,0.,0.);
  FitPtMC->SetParameters(100.,-4.,0.3,0.,0.);

  ptdata->Fit("FitPtData","R0");
  ptmcgen->Fit("FitPtMC","R0");

  FitPtData->Draw("same");
  FitPtMC->Draw("same");

  TF1* CloneFitPtData = (TF1*) FitPtData->Clone("CloneFitPtData");
  TF1* CloneFitPtMC   = (TF1*) FitPtMC->Clone("CloneFitPtMC");

  CloneFitPtData->SetLineStyle(2);
  CloneFitPtMC->SetLineStyle(2);
  CloneFitPtData->SetRange(0.,0.1);
  CloneFitPtMC->SetRange(0.,0.1);
  CloneFitPtData->Draw("same");
  CloneFitPtMC->Draw("same");


  Double_t DataInt   = ptdata->Integral(4,ptdata->GetNbinsX(),"width");
  Double_t MCInt     = ptmcgen->Integral(4,ptmcgen->GetNbinsX(),"width");
  Double_t MCIntALL  = ptmcgen->Integral( 1,ptmcgen->GetNbinsX(),"width");
  Double_t MCIntLow  = ptmcgen->Integral( 1,3,"width");

  Double_t DataExtra = FitPtData->Integral(0.,.04 );
  Double_t MCExtra   = FitPtMC->Integral(0.,.04 );

  cout << "DataInt   = " << DataInt << endl;
  cout << "DataExtra = " << DataExtra  << endl;
  cout << "LowPtExtapol --> " << DataExtra / (DataExtra+DataInt) << endl;

  cout << "MCInt   = " << MCInt << endl;
  cout << "MCExtra = " << MCExtra  << endl;
  cout << "LowPtExtapol --> " << MCExtra / (MCExtra+MCInt) << endl;
  cout << "MCIntALL= " << MCIntALL << endl;
  cout << "MCIntLow= " << MCIntLow << endl;
  cout << "LowPtExtapol --> " << MCIntLow / MCIntALL << endl;


}
