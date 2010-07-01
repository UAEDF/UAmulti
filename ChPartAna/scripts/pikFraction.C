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

void pikFraction( ) {

  TF1* piEff = new TF1("piEff","([0]*pow(x,2)+[1]*x+[2])/(pow(x,2)+[3]*x+[4])",0.05,10.);
  TF1*  kEff = new TF1( "kEff","([0]*pow(x,2)+[1]*x+[2])/(pow(x,2)+[3]*x+[4])",0.05,10.);
  piEff->SetParameters(0.8773,-0.05145,0.0022,-0.0717,0.00864);
   kEff->SetParameters(0.8631,-0.07059,0.00272,-0.1354,0.025);

  TFile* mc    = TFile::Open("../macro/ChargedHadronSpectrum.root","READ");
  TH1* ptPion  = (TH1F*) mc->Get("GenMultiPlots_ChargedPions/MultiPlots_NSD_ChargedPions/pt_NSD_ChargedPions");
  TH1* ptKaon  = (TH1F*) mc->Get("GenMultiPlots_ChargedKaons/MultiPlots_NSD_ChargedKaons/pt_NSD_ChargedKaons");

  BinWidthDiv(ptPion);
  BinWidthDiv(ptKaon); 

  ptPion->Scale(1/ ptPion->Integral() );
  ptKaon->Scale(1/ ptKaon->Integral() );


  TCanvas* cpt = new TCanvas("cpt","cpt",500,500);
  cpt->SetFillColor(0);
  cpt->GetFrame()->SetFillColor(21);
  cpt->GetFrame()->SetBorderSize(12);
  cpt->SetGrid(0,0);
  cpt->SetLeftMargin(0.17);
  cpt->cd();
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(kFALSE);
 
//  piEff->Draw("");
//   kEff->Draw("same");

   ptPion->Draw();
   ptKaon->Draw("same");
      
   TH1* num900  = (TH1F*) ptPion->Clone("num900");
   TH1* num7000 = (TH1F*) ptPion->Clone("num7000");
   
   TH1* deno900  = (TH1F*) ptPion->Clone("deno900");
   TH1* deno7000 = (TH1F*) ptPion->Clone("deno7000");


   Double_t f900  = .086 ;
   Double_t f7000 = .096 ;

   for ( int i =1 ; i<=ptPion->GetNbinsX() ; ++i ) {
     Double_t num =   (1-f900) *  ptPion->GetBinContent(i) * piEff->Eval(ptPion->GetBinCenter(i))
                    +   f900   *  ptKaon->GetBinContent(i) *  kEff->Eval(ptKaon->GetBinCenter(i)) ;
     num900->SetBinContent(i,num); 
     num900->SetBinError(i,0.); 
     Double_t deno =  (1-f900) *  ptPion->GetBinContent(i) 
                    +   f900  *  ptKaon->GetBinContent(i)  ;
     deno900->SetBinContent(i,deno); 
     deno900->SetBinError(i,0.); 
   }  

   for ( int i =1 ; i<=ptKaon->GetNbinsX() ; ++i ) {
     Double_t num =   (1-f7000) *  ptPion->GetBinContent(i) * piEff->Eval(ptPion->GetBinCenter(i))
                    +   f7000  *  ptKaon->GetBinContent(i) *  kEff->Eval(ptKaon->GetBinCenter(i)) ;
     num7000->SetBinContent(i,num); 
     num7000->SetBinError(i,0.); 
     Double_t deno =  (1-f7000) *  ptPion->GetBinContent(i) 
                    +   f7000  *  ptKaon->GetBinContent(i)  ;
     deno7000->SetBinContent(i,deno); 
     deno7000->SetBinError(i,0.); 

   }  

   TH1* eff900  = (TH1F*) ptPion->Clone("num900");
   TH1* eff7000 = (TH1F*) ptPion->Clone("num7000");
   eff900->Divide(deno900); 
   eff7000->Divide(deno7000); 

   eff900->Draw("hist");
   eff7000->Draw("histsame");



}
