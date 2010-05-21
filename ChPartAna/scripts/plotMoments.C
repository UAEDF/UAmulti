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
#include "../plugins/TMoments.C"

void plotMoments(int acc = 5 , TString momtype = "C"){

  gROOT->ProcessLine(".x cmsStyleRoot.C");
  
  TString globaldir = ("../plots/systv10_bis/");
  const int nenergy = 3;
  
  int colors[]  = {1,1,1,2,4,kMagenta};
  int markers[] = {1,1,kFullCircle,kFullStar,kFullSquare,kFullTriangleUp};
  int markersopen[] = {1,1,kOpenCircle,kOpenStar,kOpenSquare,kOpenTriangleUp};
  
  int nbin = 1500;
  double xmin = 150;
  double xmax = 8500;
  
  double maxrange = 80;
  
  double hyp      = 1;
  double niter    = 0;
  //double acc      = 5;
  double typeData = 0;
  ostringstream outstr("");
  outstr << "hyp" << hyp << "_niter" << niter << "_cut" << acc << "_DataType" << typeData;
  
  ostringstream basefig("");
  basefig<<"_cut"<<acc;
  
  vector<double> E(3,0);
  E.at(0) = 900.;
  E.at(1) = 2360.;
  E.at(2) = 7000;
  
  int typeMC[] = {10,10,31};
  int iTr    = 1;
  
  vector<TFile*> files(nenergy,new TFile());
  for(int i = 0 ; i < nenergy ; ++i){
    TString filename = fileManager(3,typeMC[i],E.at(i)/1000,iTr,0,0,outstr.str(),globaldir);
    cout<<"Opening file "<<filename<<endl;
    files.at(i) = TFile::Open( filename , "READ");
    if(files.at(i)==0){
      cout<<"At least one file is missing. Exiting now ..."<<endl;
      return;
    }
  }
  
  const int nmoments = 6;
  
  TH1F* mnch               = new TH1F("mnch","mnch",nbin,xmin,xmax);
  TGraphAsymmErrors* gmnch = new TGraphAsymmErrors(nenergy);
  vector<TH1F*> hmoments(nmoments,new TH1F());
  vector<TGraphAsymmErrors*> gmoments(nmoments,new TGraphAsymmErrors());
  
  //instantiating the histos
  for(int m = 0 ; m < nmoments ; ++m){
    ostringstream momname("");
    momname<<"cmoment_"<<m;
    hmoments.at(m) = new TH1F(momname.str().c_str(),momname.str().c_str(),nbin,xmin,xmax);
    gmoments.at(m) = new TGraphAsymmErrors(nenergy);
  }
  
  //filling the histos/graphs
  for(int i = 0 ; i < nenergy ; ++i){
    TMoments* moments = (TMoments*) files.at(i)->Get("unfolding/moments/moments_syst");
    int ibin = mnch->GetXaxis()->FindFixBin(E.at(i));
    
    mnch->SetBinContent(ibin , moments->mean->GetMean());
    mnch->SetBinError(ibin , moments->mean->GetMeanError());
    gmnch->SetPoint(i,E.at(i),moments->mean->GetMean());
    gmnch->SetPointError(i,0,0,moments->mean->_MeanSystM,moments->mean->_MeanSystP);
      
    for(int m = 0 ; m < nmoments ; ++m){
      if(momtype.Contains("C")){
        hmoments.at(m)->SetBinContent(ibin , moments->cmoments->at(m));
        hmoments.at(m)->SetBinError(ibin , moments->cstaterr->at(m));
        gmoments.at(m)->SetPoint(i,E.at(i),moments->cmoments->at(m));
        gmoments.at(m)->SetPointError(i,0,0,moments->csystmerr->at(m),moments->csystperr->at(m));
	//cout<<m<<"  "<<i<<"  "<<E.at(i)<<"  "<<gmoments.at(m)->GetErrorYlow(i)<<"  "<<gmoments.at(m)->GetErrorYhigh(i)<<endl;
	//cout<<m<<"  "<<i<<"  "<<E.at(i)<<"  "<<moments->csystmerr->at(m)<<"  "<<moments->csystperr->at(m)<<endl;
      }
      if(momtype.Contains("F")){
        hmoments.at(m)->SetBinContent(ibin , moments->fmoments->at(m));
        hmoments.at(m)->SetBinError(ibin , moments->fstaterr->at(m));
        gmoments.at(m)->SetPoint(i,E.at(i),moments->fmoments->at(m));
        gmoments.at(m)->SetPointError(i,0,0,moments->fsystmerr->at(m),moments->fsystperr->at(m));
      }
      if(momtype.Contains("K")){
        hmoments.at(m)->SetBinContent(ibin , moments->kmoments->at(m));
        hmoments.at(m)->SetBinError(ibin , moments->kstaterr->at(m));
        gmoments.at(m)->SetPoint(i,E.at(i),moments->kmoments->at(m));
        gmoments.at(m)->SetPointError(i,0,0,moments->ksystmerr->at(m),moments->ksystperr->at(m));
      }
      if(momtype.Contains("H")){
        hmoments.at(m)->SetBinContent(ibin , moments->hmoments->at(m));
        hmoments.at(m)->SetBinError(ibin , moments->hstaterr->at(m));
        gmoments.at(m)->SetPoint(i,E.at(i),moments->hmoments->at(m));
        gmoments.at(m)->SetPointError(i,0,0,moments->hsystmerr->at(m),moments->hsystperr->at(m));
      }
      hmoments.at(m)->SetFillColor(colors[m]);
      hmoments.at(m)->SetLineColor(colors[m]);
      hmoments.at(m)->SetLineWidth(2);
      hmoments.at(m)->SetMarkerColor(colors[m]);
      hmoments.at(m)->SetMarkerStyle(markers[m]);
      
      gmoments.at(m)->SetFillColor(colors[m]);
      gmoments.at(m)->SetLineColor(colors[m]);
      gmoments.at(m)->SetLineWidth(2);
      gmoments.at(m)->SetMarkerColor(colors[m]);
      gmoments.at(m)->SetMarkerStyle(markers[m]);
    }
  }
  
  hmoments[2]->GetYaxis()->SetRangeUser(0.,maxrange);
  hmoments[2]->SetTitle(TString(";#sqrt{S};")+momtype+TString("-moments"));
  hmoments[2]->Draw("E1");
  
  gPad->SetLogx();
  gPad->SetLeftMargin(0.17);
  gPad->SetBottomMargin(0.10);
  gPad->SetFillColor(0);
  gPad->GetFrame()->SetFillColor(21);
  gPad->GetFrame()->SetBorderSize(12);
  gPad->SetGrid(0,1);
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(kFALSE);

  hmoments[3]->Draw("same E1");
  hmoments[4]->Draw("same E1");
  hmoments[5]->Draw("same E1");
  
  for(int m = 2 ; m < nmoments ; ++m){
    gmoments.at(m)->Draw("same z ");
    hmoments.at(m)->Draw("same E1 ");
  }
  
  /*hmoments[3]->Draw("same E1");
  hmoments[4]->Draw("same E1");
  hmoments[5]->Draw("same E1");*/
  
  //Writing UA5
  TH1F** ua5 = new TH1F*[4];
  ua5[0] = new TH1F("ua5_c2","ua5_c2",nbin,xmin,xmax);
  ua5[1] = new TH1F("ua5_c3","ua5_c3",nbin,xmin,xmax);
  ua5[2] = new TH1F("ua5_c4","ua5_c4",nbin,xmin,xmax);
  ua5[3] = new TH1F("ua5_c5","ua5_c5",nbin,xmin,xmax);
  int bin200 = ua5[0]->GetXaxis()->FindFixBin(200);
  int bin500 = ua5[0]->GetXaxis()->FindFixBin(540);
  int bin900 = ua5[0]->GetXaxis()->FindFixBin(900);
  
  if(int(acc)%5==3){
    if(momtype.Contains("C")){
      ua5[0]->SetBinContent(bin200,1.67);
      ua5[0]->SetBinError(bin200,0.03);
      ua5[0]->SetBinContent(bin500,1.72);
      ua5[0]->SetBinError(bin500,0.02);
      ua5[0]->SetBinContent(bin900,1.75);
      ua5[0]->SetBinError(bin900,0.04);
    
      ua5[1]->SetBinContent(bin200,3.7);
      ua5[1]->SetBinError(bin200,0.1);
      ua5[1]->SetBinContent(bin500,4);
      ua5[1]->SetBinError(bin500,0.1);
      ua5[1]->SetBinContent(bin900,4.4);
      ua5[1]->SetBinError(bin900,0.1);
    
      ua5[2]->SetBinContent(bin200,9.7);
      ua5[2]->SetBinError(bin200,1.1);
      ua5[2]->SetBinContent(bin500,12.1);
      ua5[2]->SetBinError(bin500,0.7);
      ua5[2]->SetBinContent(bin900,14.1);
      ua5[2]->SetBinError(bin900,1.5);
    
      ua5[3]->SetBinContent(bin200,29);
      ua5[3]->SetBinError(bin200,5);
      ua5[3]->SetBinContent(bin500,44);
      ua5[3]->SetBinError(bin500,5);
      ua5[3]->SetBinContent(bin900,56);
      ua5[3]->SetBinError(bin900,9);
    }
    
    if(momtype.Contains("F")){
      ua5[0]->SetBinContent(bin200,1.47);
      ua5[0]->SetBinError(bin200,0.02);
      ua5[0]->SetBinContent(bin900,1.62);
      ua5[0]->SetBinError(bin900,0.02);
    
      ua5[1]->SetBinContent(bin200,2.8);
      ua5[1]->SetBinError(bin200,0.1);
      ua5[1]->SetBinContent(bin900,3.7);
      ua5[1]->SetBinError(bin900,0.1);
    
      ua5[2]->SetBinContent(bin200,6.1);
      ua5[2]->SetBinError(bin200,0.5);
      ua5[2]->SetBinContent(bin900,10.9);
      ua5[2]->SetBinError(bin900,0.8);
    
      ua5[3]->SetBinContent(bin200,15);
      ua5[3]->SetBinError(bin200,3);
      ua5[3]->SetBinContent(bin900,40.5);
      ua5[3]->SetBinError(bin900,5);
    }
  }
  else if(int(acc)%5==0){
  
    if(momtype.Contains("C")){
      ua5[0]->SetBinContent(bin200,1.44);
      ua5[0]->SetBinError(bin200,0.03);
      ua5[0]->SetBinContent(bin500,1.51);
      ua5[0]->SetBinError(bin500,0.01);
      ua5[0]->SetBinContent(bin900,1.53);
      ua5[0]->SetBinError(bin900,0.04);
    
      ua5[1]->SetBinContent(bin200,2.65);
      ua5[1]->SetBinError(bin200,0.11);
      ua5[1]->SetBinContent(bin500,3);
      ua5[1]->SetBinError(bin500,0.06);
      ua5[1]->SetBinContent(bin900,3.11);
      ua5[1]->SetBinError(bin900,0.13);
    
      ua5[2]->SetBinContent(bin200,5.8);
      ua5[2]->SetBinError(bin200,0.5);
      ua5[2]->SetBinContent(bin500,7.4);
      ua5[2]->SetBinError(bin500,0.3);
      ua5[2]->SetBinContent(bin900,7.7);
      ua5[2]->SetBinError(bin900,0.6);
    
      ua5[3]->SetBinContent(bin200,15);
      ua5[3]->SetBinError(bin200,2.2);
      ua5[3]->SetBinContent(bin500,21);
      ua5[3]->SetBinError(bin500,1);
      ua5[3]->SetBinContent(bin900,22);
      ua5[3]->SetBinError(bin900,2);
    }
  }
  
  ua5[0]->SetMarkerStyle(markersopen[2]);
  ua5[1]->SetMarkerStyle(markersopen[3]);
  ua5[2]->SetMarkerStyle(markersopen[4]);
  ua5[3]->SetMarkerStyle(markersopen[5]);
  ua5[0]->SetMarkerColor(colors[2]);
  ua5[1]->SetMarkerColor(colors[3]);
  ua5[2]->SetMarkerColor(colors[4]);
  ua5[3]->SetMarkerColor(colors[5]);
  ua5[0]->SetLineColor(colors[2]);
  ua5[1]->SetLineColor(colors[3]);
  ua5[2]->SetLineColor(colors[4]);
  ua5[3]->SetLineColor(colors[5]);
  
  ua5[0]->Draw("same");
  ua5[1]->Draw("same");
  ua5[2]->Draw("same");
  ua5[3]->Draw("same");
  
  TLegend* leg = new TLegend(0.2,0.65,0.35,0.85);
  leg->AddEntry(hmoments[2],momtype+TString("_{2}"),"l");
  leg->AddEntry(hmoments[3],momtype+TString("_{3}"),"l");
  leg->AddEntry(hmoments[4],momtype+TString("_{4}"),"l");
  leg->AddEntry(hmoments[5],momtype+TString("_{5}"),"l");
  leg->AddEntry(hmoments[2],"CMS","p");
  leg->AddEntry(ua5[0],"UA5","p");
  leg->SetFillColor(kWhite);
  leg->Draw("same");
  
  gPad->Update();
  
  gPad->WaitPrimitive();
  return;
  gPad->SaveAs(momtype+TString("moments")+basefig.str()+TString(".gif"),"");
  gPad->SaveAs(momtype+TString("moments")+basefig.str()+TString(".eps"),"");
  gPad->SaveAs(momtype+TString("moments")+basefig.str()+TString(".root"),"");
  gSystem->Exec(TString("convert ")+momtype+TString("moments")+basefig.str()+TString(".eps ")+momtype+TString("moments")+basefig.str()+TString(".pdf"));
  
  
  
  //---------------------------------------------------------------
  //----------------------- <Nch> VS sqrt(S) ----------------------
  //---------------------------------------------------------------
  
  
  TCanvas* c_mnch = new TCanvas("c_mnch","c_mnch",200,510,500,500);
  c_mnch->cd();
  
  
  
  mnch->SetLineColor(kRed-3);
  mnch->SetMarkerColor(kRed-3);
  mnch->SetMarkerStyle(8);
  mnch->SetTitle(";#sqrt{S};<n>");
  mnch->Draw("E1");
  gmnch->Draw("same z");
  
  
  
  gPad->SetLogx();
  gPad->SetLeftMargin(0.17);
  gPad->SetBottomMargin(0.10);
  gPad->SetFillColor(0);
  gPad->GetFrame()->SetFillColor(21);
  gPad->GetFrame()->SetBorderSize(12);
  gPad->SetGrid(0,1);
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(kFALSE);
  
  TGraphErrors* UA5mean = new TGraphErrors();
  if(acc==0 || acc==5){
    UA5mean->SetPoint(1,200.,13.2);
    UA5mean->SetPointError(1,0,0.3);
    UA5mean->SetPoint(2,546,15.9);
    UA5mean->SetPointError(2,0,0.1);
    UA5mean->SetPoint(3,900,18.8);
    UA5mean->SetPointError(3,0,0.7);
  }
  else if(acc==3 || acc==8){
    UA5mean->SetPoint(1,200,5.17);
    UA5mean->SetPointError(1,0,0.13);
    UA5mean->SetPoint(2,546,6.17);
    UA5mean->SetPointError(2,0,0.06);
    UA5mean->SetPoint(3,900,7.38);
    UA5mean->SetPointError(3,0,0.28);
  }
  if(acc==2 || acc==7){
    UA5mean->SetPoint(1,24,4.49);
    UA5mean->SetPointError(1,0,0.06);
    UA5mean->SetPoint(2,31,5.00);
    UA5mean->SetPointError(2,0,0.08);
    UA5mean->SetPoint(3,45,5.49);
    UA5mean->SetPointError(3,0,0.06);
    UA5mean->SetPoint(4,53,5.775);
    UA5mean->SetPointError(4,0,0.06);
    UA5mean->SetPoint(5,63,6.16);
    UA5mean->SetPointError(5,0,0.06);
  
    UA5mean->SetPoint(6,200,7.94);
    UA5mean->SetPointError(6,0,0.23);
    UA5mean->SetPoint(7,546,9.49);
    UA5mean->SetPointError(7,0,0.08);
    UA5mean->SetPoint(8,900,11.02);
    UA5mean->SetPointError(8,0,0.32);
  }
  
  UA5mean->SetLineColor(kBlue);
  UA5mean->SetMarkerColor(kBlue);
  UA5mean->SetMarkerStyle(kOpenCross);
  UA5mean->Draw("same p");
  
  
  TGraphAsymmErrors* nchmean_all = new TGraphAsymmErrors(UA5mean->GetN()+nenergy);
  for(int i = 0 ; i < UA5mean->GetN() ; ++i){
    double x = 0 , y = 0;
    UA5mean->GetPoint(i,x,y);
    nchmean_all->SetPoint(i,x,y);
    nchmean_all->SetPointError(i,0,0,UA5mean->GetErrorYlow(i),UA5mean->GetErrorYhigh(i));
  }
  for(int i =  UA5mean->GetN() ; i <  nchmean_all->GetN() ; ++i){
    double x = 0 , y = 0;
    gmnch->GetPoint(i-UA5mean->GetN(),x,y);
    nchmean_all->SetPoint(i,x,y);
    nchmean_all->SetPointError(i,0,0,gmnch->GetErrorYlow(i-UA5mean->GetN()),gmnch->GetErrorYhigh(i-UA5mean->GetN()));
  }
  
  //TF1* fua5mnch = new TF1("fua5mnch","[0] + [1] * sqrt(x) + [2] * x",5,15000);
  //TF1* fua5mnch = new TF1("fua5mnch","[0] + [1] * x + [2] * x * x",5,15000);
  TF1* fua5mnch = new TF1("fua5mnch","[0] + [1] * log(x*x) + [2] * log(x*x) * log(x*x)",5,15000);
  fua5mnch->SetLineWidth(1);
  fua5mnch->SetLineColor(kBlack);
  nchmean_all->Fit("fua5mnch");
  fua5mnch->Draw("same");
  
  leg = new TLegend(0.3,0.7,0.45,0.85);
  if(acc==7)leg->AddEntry(UA5mean,"ISR+UA5","p");
  else      leg->AddEntry(UA5mean,"UA5","p");
  leg->AddEntry(mnch,"CMS","p");
  leg->SetFillColor(kWhite);
  
  leg->Draw("SAME");
  mnch->Draw("SAME");
  UA5mean->Draw("same p");
  
  ostringstream func("");
  func<<fixed<<setprecision(3)<<fua5mnch->GetParameter(0)<<" + "<<
        fua5mnch->GetParameter(1)<<" ln(s) + "<<
	fua5mnch->GetParameter(2)<<" ln(s)^{2}";
  cout<<func.str().c_str()<<endl;

  TLatex* textnchmean = new TLatex(0.3,0.15,func.str().c_str());
  textnchmean->SetNDC(kTRUE);
  textnchmean->SetTextSize(0.03);
  textnchmean->DrawLatex(0.45,0.15,func.str().c_str());
  
  gPad->WaitPrimitive();
  gPad->SaveAs(TString("nchmean")+basefig.str()+TString(".gif"),"");
  gPad->SaveAs(TString("nchmean")+basefig.str()+TString(".eps"),"");
  gPad->SaveAs(TString("nchmean")+basefig.str()+TString(".root"),"");
  gSystem->Exec(TString("convert ")+TString("nchmean")+basefig.str()+TString(".eps ")+TString("nchmean")+basefig.str()+TString(".pdf"));
  
  
  
  
}
