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

void plotMoments(){

  TString globaldir = ("../plots/systv3/");
  const int nenergy = 3;
  
  int colors[]  = {1,1,1,2,4,kOrange};
  int markers[] = {1,1,kOpenCircle,kPlus,kFullSquare,kFullCircle};
  
  
  double hyp      = 1;
  double niter    = 0;
  double acc      = 8;
  double typeData = 0;
  ostringstream outstr("");
  outstr << "hyp" << hyp << "_niter" << niter << "_cut" << acc << "_DataType" << typeData;
  
  vector<double> E(nenergy,0);
  E.at(0) = 900.;
  E.at(1) = 2360.;
  E.at(2) = 7000;
  
  int typeMC = 10;
  int iTr    = 1;
  
  vector<TFile*> files(nenergy,new TFile());
  for(int i = 0 ; i < nenergy ; ++i){
    files.at(i) = TFile::Open(fileManager(3,typeMC,E.at(i)/1000,iTr,0,0,outstr.str(),globaldir) , "READ");
    if(files.at(i)==0){
      cout<<"At least one file is missing. Exiting now ..."<<endl;
      return;
    }
  }
  
  const int nmoments = 6;
  vector< vector<double> > moments;
  vector< vector<double> > errors;
  TH1F* tmp;
  for(int m = 0 ; m < nmoments ; ++m){
    ostringstream momname("");
    momname << "unfolding/moments/moment_" << m;
    vector<double> moment(nenergy,0);
    vector<double> error(nenergy,0);
    for(int i = 0 ; i < nenergy ; ++i){
      tmp = (TH1F*) files.at(i)->Get(momname.str().c_str());
      moment.at(i) = tmp->GetBinContent(1);
      error.at(i) = tmp->GetBinError(1);
    }
    moments.push_back(moment);
    errors.push_back(error);  
  }
  
  
  vector<TH1F*> hmoments(nmoments,new TH1F());
  for(int m = 0 ; m < nmoments ; ++m){
    ostringstream momname("");
    momname<<"cmoment_"<<m;
    hmoments.at(m) = new TH1F(momname.str().c_str(),momname.str().c_str(),1500,100,8500);
    for(int i = 0 ; i < nenergy ; ++i){
      int ibin = hmoments.at(m)->GetXaxis()->FindFixBin(E.at(i));
      hmoments.at(m)->SetBinContent(ibin , moments.at(m).at(i));
      hmoments.at(m)->SetBinError(ibin , errors.at(m).at(i));
    }
    hmoments.at(m)->SetFillColor(colors[m]);
    hmoments.at(m)->SetLineColor(colors[m]);
    hmoments.at(m)->SetMarkerColor(colors[m]);
    hmoments.at(m)->SetMarkerStyle(markers[m]);
  }
  
  //Writing UA5
  TH1F** ua5 = new TH1F*[4];
  ua5[0] = new TH1F("ua5_c2","ua5_c2",1500,100,8500);
  ua5[1] = new TH1F("ua5_c3","ua5_c3",1500,100,8500);
  ua5[2] = new TH1F("ua5_c4","ua5_c4",1500,100,8500);
  ua5[3] = new TH1F("ua5_c5","ua5_c5",1500,100,8500);
  int bin200 = ua5[0]->GetXaxis()->FindFixBin(200);
  int bin500 = ua5[0]->GetXaxis()->FindFixBin(540);
  int bin900 = ua5[0]->GetXaxis()->FindFixBin(900);
  
  if(int(acc)%5==3){
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
  else if(int(acc)%5==0){
  
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
  
  
  
  
  
  
  hmoments[2]->GetYaxis()->SetRangeUser(0.,60.);
  hmoments[2]->Draw();
  gPad->SetLogx();
  hmoments[3]->Draw("same");
  hmoments[4]->Draw("same");
  hmoments[5]->Draw("same");
  
  
  ua5[0]->SetMarkerStyle(kMultiply);
  ua5[1]->SetMarkerStyle(kMultiply);
  ua5[2]->SetMarkerStyle(kMultiply);
  ua5[3]->SetMarkerStyle(kMultiply);
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
}
