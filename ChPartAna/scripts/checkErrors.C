#include "TMath.h"
#include "TLatex.h"
#include "TSystem.h"
#include "TText.h"
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
#include "TGraphErrors.h"
#include "TProfile.h"
#include "TROOT.h"
#include "TDirectory.h"

#include "TFrame.h"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>

#include "../macro/fileManager.C"

void checkErrors(double E = 0.9 , int acc = 5 , int typeData = 0 , TString addfigname= ""){

  gROOT->ProcessLine(".x cmsStyleRoot.C");
  
  TString globaldir = "../plots/systv10_niter/";
  //TString globaldir = "../plots/systv10_niter_InSmoothed/";
  
  //TString plot = "unfolding/nch_data_corrected";
  TString plot = "unfolding/nch_histresampled";
  //TString plot = "unfolding/nch_mtxresampledPtr";

  int hyp = 1;
  //int acc = 5;
  //int typeData = 10;
  int typeMC = 10;
  if(E==7.) typeMC = 31;
  
  //syst file
  int niter = 0;
  ostringstream outstr("");
  outstr << "hyp" << hyp << "_niter" << niter << "_cut" << acc << "_DataType" << 0;
  TString filename = fileManager(3,typeMC, E,1,0,0,outstr.str(),"../plots/systv10_niter100_2/");
  cout<<"Opening file : "<<filename<<endl;
  TFile* fsyst = TFile::Open(filename,"READ");
  TGraphAsymmErrors*  hsyst = (TGraphAsymmErrors*) fsyst->Get("unfolding/gnch_data_corrected_syst");
  
  niter = 0;
  outstr.str("");
  outstr << "hyp" << hyp << "_niter" << niter << "_cut" << acc << "_DataType" << typeData;
  filename = fileManager(3,typeMC, E,1,0,0,outstr.str(),globaldir);
  cout<<"Opening file : "<<filename<<endl;
  TFile* f3 = TFile::Open(filename,"READ");
  TH1F*  h3 = NULL;
  if(f3!=0) h3 = (TH1F*) f3->Get(plot);
  
  niter = 20;
  outstr.str("");
  outstr << "hyp" << hyp << "_niter" << niter << "_cut" << acc << "_DataType" << typeData;
  filename = fileManager(3,typeMC, E,1,0,0,outstr.str(),globaldir);
  cout<<"Opening file : "<<filename<<endl;
  TFile* f5 = TFile::Open(filename,"READ");
  TH1F*  h5 = NULL;
  if(f5!=0) h5 = (TH1F*) f5->Get(plot);
  
  niter = 50;
  outstr.str("");
  outstr << "hyp" << hyp << "_niter" << niter << "_cut" << acc << "_DataType" << typeData;
  filename = fileManager(3,typeMC, E,1,0,0,outstr.str(),globaldir);
  cout<<"Opening file : "<<filename<<endl;
  TFile* f10 = TFile::Open(filename,"READ");
  TH1F*  h10 = NULL;
  if(f10!=0) h10 = (TH1F*) f10->Get(plot);
  
  
  niter = 100;
  outstr.str("");
  outstr << "hyp" << hyp << "_niter" << niter << "_cut" << acc << "_DataType" << typeData;
  filename = fileManager(3,typeMC, E,1,0,0,outstr.str(),globaldir);
  cout<<"Opening file : "<<filename<<endl;
  TFile* f50 = TFile::Open(filename,"READ");
  TH1F*  h50 = NULL;
  if(f50!=0) h50 = (TH1F*) f50->Get(plot);
  
  
  niter = 500;
  outstr.str("");
  outstr << "hyp" << hyp << "_niter" << niter << "_cut" << acc << "_DataType" << typeData;
  filename = fileManager(3,typeMC, E,1,0,0,outstr.str(),globaldir);
  cout<<"Opening file : "<<filename<<endl;
  TFile* f100 = TFile::Open(filename,"READ");
  TH1F*  h100 = NULL;
  if(f100!=0) h100 = (TH1F*) f100->Get(plot);
  
  niter = 1000;
  outstr.str("");
  outstr << "hyp" << hyp << "_niter" << niter << "_cut" << acc << "_DataType" << typeData;
  filename = fileManager(3,typeMC, E,1,0,0,outstr.str(),globaldir);
  cout<<"Opening file : "<<filename<<endl;
  TFile* f1000 = TFile::Open(filename,"READ");
  TH1F*  h1000 = NULL;
  if(f1000!=0) h1000 = (TH1F*) f1000->Get(plot);
  
  
  
  Double_t* x  = new Double_t[h3->GetNbinsX()];
  Double_t* y  = new Double_t[h3->GetNbinsX()];
  Double_t* ex = new Double_t[h3->GetNbinsX()];
  
  Double_t* eyn     = new Double_t[h3->GetNbinsX()];
  Double_t* ysyst   = hsyst->GetY();
  Double_t* eylsyst = hsyst->GetEYlow();
  Double_t* eyhsyst = hsyst->GetEYhigh();
  Double_t* ey3     = new Double_t[h3->GetNbinsX()];
  Double_t* ey5     = new Double_t[h3->GetNbinsX()];
  Double_t* ey10    = new Double_t[h3->GetNbinsX()];
  Double_t* ey50    = new Double_t[h3->GetNbinsX()];
  Double_t* ey100   = new Double_t[h3->GetNbinsX()];
  Double_t* ey1000  = new Double_t[h3->GetNbinsX()];
  
  for(int i = 1 ; i <= h3->GetNbinsX() ; ++i){
    x[i-1] = h3->GetBinCenter(i);
    y[i-1] = 0;
    ex[i-1] = h3->GetBinCenter(i) - h3->GetXaxis()->GetBinLowEdge(i);
    
    eyn[i-1] = ey3[i-1] = ey5[i-1] = ey10[i-1] = ey50[i-1] = ey100[i-1] = 0;
    
    
    if(h3!=0 && h3->GetBinContent(i)!=0)       eyn[i-1]     = sqrt(h3->GetBinContent(i))/(h3->GetBinContent(i) * sqrt(h3->GetBinWidth(i)));
    if(h3!=0 && h3->GetBinContent(i)!=0)       ey3[i-1]     = h3->GetBinError(i)        /h3->GetBinContent(i);
    if(h5!=0 && h5->GetBinContent(i)!=0)       ey5[i-1]     = h5->GetBinError(i)        /h5->GetBinContent(i);
    if(h10!=0 && h10->GetBinContent(i)!=0)     ey10[i-1]    = h10->GetBinError(i)       /h10->GetBinContent(i);
    if(h50!=0 && h50->GetBinContent(i)!=0)     ey50[i-1]    = h50->GetBinError(i)       /h50->GetBinContent(i);
    if(h100!=0 && h100->GetBinContent(i)!=0)   ey100[i-1]   = h100->GetBinError(i)      /h100->GetBinContent(i);   
    if(h1000!=0 && h1000->GetBinContent(i)!=0) ey1000[i-1]  = h1000->GetBinError(i)     /h1000->GetBinContent(i);  
    if(ysyst[i-1]!=0 && hsyst!=0)              eylsyst[i-1] = eylsyst[i-1]              /ysyst[i-1];
    if(ysyst[i-1]!=0 && hsyst!=0)              eyhsyst[i-1] = eyhsyst[i-1]              /ysyst[i-1];
    
  }
  
  double max = 0;
  for(int i=1;i<h3->GetNbinsX();++i){
    max=x[i-1] + ex[i-1]+0.5;
    if(h3->GetBinContent(i)==0)
      break;
  }
  
  TGraphErrors* gn    = new TGraphErrors(h3->GetNbinsX(),x,y,ex,eyn);
  TGraphAsymmErrors* gsyst = new TGraphAsymmErrors(h3->GetNbinsX(),x,y,ex,ex,eylsyst,eyhsyst);
  TGraphErrors* g3    = new TGraphErrors(h3->GetNbinsX(),x,y,ex,ey3);
  TGraphErrors* g5    = new TGraphErrors(h3->GetNbinsX(),x,y,ex,ey5);
  TGraphErrors* g10   = new TGraphErrors(h3->GetNbinsX(),x,y,ex,ey10);
  TGraphErrors* g50   = new TGraphErrors(h3->GetNbinsX(),x,y,ex,ey50);
  TGraphErrors* g100  = new TGraphErrors(h3->GetNbinsX(),x,y,ex,ey100);
  TGraphErrors* g1000 = new TGraphErrors(h3->GetNbinsX(),x,y,ex,ey1000);
  
  
  gn->SetMarkerStyle(kDot);
  
  gn->SetFillColor(kBlack);
  g3->SetFillColor(kBlue);
  g5->SetFillColor(kRed);
  g10->SetFillColor(kOrange);
  g50->SetFillColor(kMagenta);
  g100->SetFillColor(kGreen);
  g1000->SetFillColor(kBlack);
  gsyst->SetFillColor(16);
  
  gsyst->GetYaxis()->SetRangeUser(-0.3,0.3);
  gsyst->GetXaxis()->SetRangeUser(-0.5,max);
  gsyst->SetTitle(";n;#sigma(P_{n}) / P_{n}");
  
               gsyst->Draw("a2");
  if(h1000!=0) g1000->Draw("2 same");
  if(h100!=0)  g100->Draw("2 same");
  g50->Draw("2 same");
  g10->Draw("2 same");
  g5->Draw(" 2same");
  g3->Draw("2 same");
  gn->Draw("p same");
  
  TString baseLeg = "Data ";
  if(typeData!=0) baseLeg = "MC ";
  
  #include "../macro/acceptanceMap.C"

  TLegend* leg = new TLegend(0.20,0.75,0.50,0.99);
  ostringstream legheader("");
  legheader<< "pt > "<<accMap.at(acc).at(0)<<" GeV       |#eta| < "<<accMap.at(acc).at(1);
  leg->SetHeader(legheader.str().c_str());
  leg->AddEntry(gn,"err=#sqrt{N}","l");
  if(h3!=0)  leg->AddEntry(g3,baseLeg+TString("standard iter"),"f");
  if(h5!=0)  leg->AddEntry(g5,baseLeg+TString("20 iter"),"f");
  if(h10!=0)  leg->AddEntry(g10,baseLeg+TString("50 iter"),"f");
  if(h50!=0)  leg->AddEntry(g50,baseLeg+TString("100 iter"),"f");
  if(h100!=0)  leg->AddEntry(g100,baseLeg+TString("500 iter"),"f");
  if(h1000!=0) leg->AddEntry(g1000,baseLeg+TString("1000 iter"),"f");
               leg->AddEntry(gsyst,"Data default stat + syst","f");
  //leg->AddEntry(gsyst,baseLeg+TString("default stat + syst"),"f");
  leg->SetFillColor(kWhite);

  
  leg->Draw("same");
  
  ostringstream txt("");
  txt<<"CMS "<<E<<" TeV";
  TLatex* text = new TLatex(0.2,0.4,txt.str().c_str());
  text->SetNDC(kTRUE);
  text->SetTextSize(0.06);
  text->DrawLatex(0.5,0.95,txt.str().c_str());
  

  gPad->WaitPrimitive();
  
  ostringstream figbase("");
  figbase<<"../figs/err_niter_"<<E<<"TeV_dataType"<<typeData<<"_cut"<<acc;
  gPad->SaveAs(TString(figbase.str())+addfigname+TString(".gif"),"gif");
  gPad->SaveAs(TString(figbase.str())+addfigname+TString(".eps"),"eps");

}
