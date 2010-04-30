#include "TMath.h"
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
#include "TProfile.h"
#include "TROOT.h"
#include "TDirectory.h"

#include "TFrame.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "../macro/fileManager.C"
#include "../plugins/TMoments.C"

void plotMoments(TString momtype = "C"){

  TString globaldir = ("../plots/");
  const int nenergy = 3;
  
  int colors[]  = {1,1,1,2,4,kMagenta};
  int markers[] = {1,1,kFullCircle,kFullStar,kFullSquare,kFullTriangleUp};
  int markersopen[] = {1,1,kOpenCircle,kOpenStar,kOpenSquare,kOpenTriangleUp};
  
  int nbin = 1500;
  double xmin = 100;
  double xmax = 8500;
  
  double maxrange = 80;
  
  double hyp      = 1;
  double niter    = 0;
  double acc      = 5;
  double typeData = 0;
  ostringstream outstr("");
  outstr << "hyp" << hyp << "_niter" << niter << "_cut" << acc << "_DataType" << typeData;
  
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
  vector<double> mean(nenergy,0);
  vector<double> meanerr(nenergy,0);
  vector< vector<double> > moments;
  vector< vector<double> > errors;
  TMoments* tmp;
  for(int m = 0 ; m < nmoments ; ++m){
    vector<double> moment(nenergy,0);
    vector<double> error(nenergy,0);
    for(int i = 0 ; i < nenergy ; ++i){
      tmp = (TMoments*) files.at(i)->Get("unfolding/moments/moments");
      if(tmp==0){
        cout<<"Was not able to take moments from file @ "<<E.at(i)<<"GeV. Exiting now ..."<<endl;
	return;
      }
      
      if(momtype.Contains("C")){
        moment.at(i)  = tmp->cmoments->at(m);
        error.at(i)   = tmp->cerrors->at(m);
      }
      if(momtype.Contains("F")){
        moment.at(i)  = tmp->fmoments->at(m);
        error.at(i)   = tmp->ferrors->at(m);
      }
      if(momtype.Contains("K")){
        moment.at(i)  = tmp->kmoments->at(m);
        error.at(i)   = tmp->kerrors->at(m);
      }
      if(momtype.Contains("H")){
        moment.at(i)  = tmp->hmoments->at(m);
        error.at(i)   = tmp->herrors->at(m);
      }
      
      mean.at(i)    = tmp->mean->GetMean();
      meanerr.at(i) = tmp->mean->GetMeanError();
      tmp->print();
    }
    moments.push_back(moment);
    errors.push_back(error);  
  }
  
  
  vector<TH1F*> hmoments(nmoments,new TH1F());
  for(int m = 0 ; m < nmoments ; ++m){
    ostringstream momname("");
    momname<<"cmoment_"<<m;
    hmoments.at(m) = new TH1F(momname.str().c_str(),momname.str().c_str(),nbin,xmin,xmax);
    for(int i = 0 ; i < nenergy ; ++i){
      int ibin = hmoments.at(m)->GetXaxis()->FindFixBin(E.at(i));
      hmoments.at(m)->SetBinContent(ibin , moments.at(m).at(i));
      hmoments.at(m)->SetBinError(ibin , errors.at(m).at(i));
    }
    hmoments.at(m)->SetFillColor(colors[m]);
    hmoments.at(m)->SetLineColor(colors[m]);
    hmoments.at(m)->SetLineWidth(2);
    hmoments.at(m)->SetMarkerColor(colors[m]);
    hmoments.at(m)->SetMarkerStyle(markers[m]);
  }
  
  
  
  
  hmoments[2]->GetYaxis()->SetRangeUser(0.,maxrange);
  hmoments[2]->SetTitle(";#sqrt{S};moments");
  hmoments[2]->Draw();
  
  gPad->SetLogx();
  gPad->SetLeftMargin(0.17);
  gPad->SetBottomMargin(0.10);
  gPad->SetFillColor(0);
  gPad->GetFrame()->SetFillColor(21);
  gPad->GetFrame()->SetBorderSize(12);
  gPad->SetGrid(0,1);
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(kFALSE);

  hmoments[3]->Draw("same");
  hmoments[4]->Draw("same");
  hmoments[5]->Draw("same");
  
  
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
  
  gPad->WaitPrimitive();
  gPad->SaveAs("Cmoments.gif","");
  gPad->SaveAs("Cmoments.root","");
  
  
  TCanvas* c_mnch = new TCanvas("c_mnch","c_mnch",200,510,500,500);
  c_mnch->cd();
  
  TH1F* mnch = new TH1F("mnch","mnch",nbin,xmin,xmax);
  for(int i = 0 ; i < nenergy ; ++i){
      int ibin = mnch->GetXaxis()->FindFixBin(E.at(i));
      mnch->SetBinContent(ibin , mean.at(i));
      mnch->SetBinError(ibin , meanerr.at(i));
  }
  
  mnch->SetLineColor(kRed-3);
  mnch->SetMarkerColor(kRed-3);
  mnch->SetMarkerStyle(8);
  mnch->SetTitle(";#sqrt{S};<N_{ch}>");
  mnch->Draw();
  
  
  
  gPad->SetLogx();
  gPad->SetLeftMargin(0.17);
  gPad->SetBottomMargin(0.10);
  gPad->SetFillColor(0);
  gPad->GetFrame()->SetFillColor(21);
  gPad->GetFrame()->SetBorderSize(12);
  gPad->SetGrid(0,1);
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(kFALSE);
  
  TH1F* UA5mean = new TH1F("UA5mean","UA5mean",nbin,xmin,xmax);
  UA5mean->Reset();
  if(acc==0 || acc==5){
    UA5mean->SetBinContent(bin200,13.2);
    UA5mean->SetBinError(bin200,0.3);
    UA5mean->SetBinContent(bin500,15.9);
    UA5mean->SetBinError(bin500,0.1);
    UA5mean->SetBinContent(bin900,18.8);
    UA5mean->SetBinError(bin900,0.7);
  }
  if(acc==3 || acc==8){
    UA5mean->SetBinContent(bin200,5.17);
    UA5mean->SetBinError(bin200,0.13);
    UA5mean->SetBinContent(bin500,6.17);
    UA5mean->SetBinError(bin500,0.06);
    UA5mean->SetBinContent(bin900,7.38);
    UA5mean->SetBinError(bin900,0.28);
  }
  
  UA5mean->SetLineColor(kBlue);
  UA5mean->SetMarkerColor(kBlue);
  UA5mean->SetMarkerStyle(kMultiply);
  UA5mean->Draw("same err");
  
  TH1F* nchmean_all = (TH1F*) UA5mean->Clone("nchmean_all");
  for(int i = 0 ; i < nenergy ; ++i){
    int ibin = mnch->GetXaxis()->FindFixBin(E.at(i));
    nchmean_all->SetBinContent(ibin , mean.at(i));
    nchmean_all->SetBinError(ibin , meanerr.at(i));
  }
  
  //TF1* fua5mnch = new TF1("fua5mnch","[0] + [1] * sqrt(x) + [2] * x",5,15000);
  //TF1* fua5mnch = new TF1("fua5mnch","[0] + [1] * x + [2] * x * x",5,15000);
  TF1* fua5mnch = new TF1("fua5mnch","[0] + [1] * log(x) + [2] * log(x) * log(x)",5,15000);
  fua5mnch->SetLineWidth(1);
  fua5mnch->SetLineColor(kBlack);
  nchmean_all->Fit("fua5mnch");
  fua5mnch->Draw("same");
  
  leg = new TLegend(0.3,0.7,0.45,0.85);
  leg->AddEntry(UA5mean,"UA5","p");
  leg->AddEntry(mnch,"CMS","p");
  leg->SetFillColor(kWhite);
  
  leg->Draw("SAME");
  mnch->Draw("SAME");
  UA5mean->Draw("same err");
  
  
  gPad->WaitPrimitive();
  gPad->SaveAs("nchmean.gif","");
  gPad->SaveAs("nchmean.root","");
  
  
  
  
  
}
