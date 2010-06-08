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
  
  TString globaldir = ("../plots/");
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
    int ibin = hmoments.at(0)->GetXaxis()->FindFixBin(E.at(i));
      
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
      hmoments.at(m)->SetFillColor(kRed-3);
      hmoments.at(m)->SetLineColor(kRed-3);
      hmoments.at(m)->SetLineWidth(2);
      hmoments.at(m)->SetMarkerColor(kRed-3);
      if(m%2==0) hmoments.at(m)->SetMarkerStyle(markers[2]);
      else        hmoments.at(m)->SetMarkerStyle(markersopen[2]);
      hmoments.at(m)->SetMarkerSize(2);
      
      gmoments.at(m)->SetFillColor(kRed-3);
      gmoments.at(m)->SetLineColor(kRed-3);
      gmoments.at(m)->SetLineWidth(2);
      gmoments.at(m)->SetMarkerColor(kRed-3);
      if(m%2==0) gmoments.at(m)->SetMarkerStyle(markers[2]);
      else        gmoments.at(m)->SetMarkerStyle(markersopen[2]);
      gmoments.at(m)->SetMarkerSize(2);
    }
  }
  
  
  
  /*hmoments[3]->Draw("same E1");
  hmoments[4]->Draw("same E1");
  hmoments[5]->Draw("same E1");*/
  
  //Writing UA5
  TGraphErrors** ua5 = new TGraphErrors*[4];
  ua5[0] = new TGraphErrors();
  ua5[1] = new TGraphErrors();
  ua5[2] = new TGraphErrors();
  ua5[3] = new TGraphErrors();
  
  if(int(acc)%5==3){
    if(momtype.Contains("C")){
      ua5[0]->SetPoint(1,200,1.67);
      ua5[0]->SetPointError(1,0,0.03);
      ua5[0]->SetPoint(2,540,1.72);
      ua5[0]->SetPointError(2,0,0.02);
      ua5[0]->SetPoint(3,900,1.75);
      ua5[0]->SetPointError(3,0,0.04);
    
      ua5[1]->SetPoint(1,200,3.7);
      ua5[1]->SetPointError(1,0,0.1);
      ua5[1]->SetPoint(2,540,4);
      ua5[1]->SetPointError(2,0,0.1);
      ua5[1]->SetPoint(3,900,4.4);
      ua5[1]->SetPointError(3,0,0.1);
    
      ua5[2]->SetPoint(1,200,9.7);
      ua5[2]->SetPointError(1,0,1.1);
      ua5[2]->SetPoint(2,540,12.1);
      ua5[2]->SetPointError(2,0,0.7);
      ua5[2]->SetPoint(3,900,14.1);
      ua5[2]->SetPointError(3,0,1.5);
    
      ua5[3]->SetPoint(1,200,29);
      ua5[3]->SetPointError(1,0,5);
      ua5[3]->SetPoint(2,540,44);
      ua5[3]->SetPointError(2,0,5);
      ua5[3]->SetPoint(3,900,56);
      ua5[3]->SetPointError(3,0,9);
    }
    
    if(momtype.Contains("F")){
      ua5[0]->SetPoint(1,200,1.47);
      ua5[0]->SetPointError(1,0,0.02);
      ua5[0]->SetPoint(3,900,1.62);
      ua5[0]->SetPointError(3,0,0.02);
    
      ua5[1]->SetPoint(1,200,2.8);
      ua5[1]->SetPointError(1,0,0.1);
      ua5[1]->SetPoint(3,900,3.7);
      ua5[1]->SetPointError(3,0,0.1);
    
      ua5[2]->SetPoint(1,200,6.1);
      ua5[2]->SetPointError(1,0,0.5);
      ua5[2]->SetPoint(3,900,10.9);
      ua5[2]->SetPointError(3,0,0.8);
    
      ua5[3]->SetPoint(1,200,15);
      ua5[3]->SetPointError(1,0,3);
      ua5[3]->SetPoint(3,900,40.5);
      ua5[3]->SetPointError(3,0,5);
    }
  }
  else if(int(acc)%5==0){
  
    if(momtype.Contains("C")){
      ua5[0]->SetPoint(1,200,1.44);
      ua5[0]->SetPointError(1,0,0.03);
      ua5[0]->SetPoint(2,540,1.51);
      ua5[0]->SetPointError(2,0,0.01);
      ua5[0]->SetPoint(3,900,1.53);
      ua5[0]->SetPointError(3,0,0.04);
    
      ua5[1]->SetPoint(1,200,2.65);
      ua5[1]->SetPointError(1,0,0.11);
      ua5[1]->SetPoint(2,540,3);
      ua5[1]->SetPointError(2,0,0.06);
      ua5[1]->SetPoint(3,900,3.11);
      ua5[1]->SetPointError(3,0,0.13);
    
      ua5[2]->SetPoint(1,200,5.8);
      ua5[2]->SetPointError(1,0,0.5);
      ua5[2]->SetPoint(2,540,7.4);
      ua5[2]->SetPointError(2,0,0.3);
      ua5[2]->SetPoint(3,900,7.7);
      ua5[2]->SetPointError(3,0,0.6);
    
      ua5[3]->SetPoint(1,200,15);
      ua5[3]->SetPointError(1,0,2.2);
      ua5[3]->SetPoint(2,540,21);
      ua5[3]->SetPointError(2,0,1);
      ua5[3]->SetPoint(3,900,22);
      ua5[3]->SetPointError(3,0,2);
    }
    if(momtype.Contains("F")){
      ua5[0]->SetPoint(1,200,1.36);
      ua5[0]->SetPointError(1,0,0.02);
      ua5[0]->SetPoint(2,900,1.48);
      ua5[0]->SetPointError(2,0,0.01);
    
      ua5[1]->SetPoint(1,200,2.33);
      ua5[1]->SetPointError(1,0,0.09);
      ua5[1]->SetPoint(2,900,2.88);
      ua5[1]->SetPointError(2,0,0.09);
    
      ua5[2]->SetPoint(1,200,4.7);
      ua5[2]->SetPointError(1,0,0.36);
      ua5[2]->SetPoint(2,900,6.8);
      ua5[2]->SetPointError(2,0,0.4);
    
      ua5[3]->SetPoint(1,200,11);
      ua5[3]->SetPointError(1,0,1);
      ua5[3]->SetPoint(2,900,18);
      ua5[3]->SetPointError(2,0,2.23);
    }
    if(momtype.Contains("K")){
      ua5[0]->SetPoint(1,200,0.36);
      ua5[0]->SetPointError(1,0,0);
      ua5[0]->SetPoint(2,900,0.48);
      ua5[0]->SetPointError(2,0,0);
    
      ua5[1]->SetPoint(1,200,0.25);
      ua5[1]->SetPointError(1,0,0);
      ua5[1]->SetPoint(2,900,0.44);
      ua5[1]->SetPointError(2,0,0);
    
      ua5[2]->SetPoint(1,200,0.151);
      ua5[2]->SetPointError(1,0,0);
      ua5[2]->SetPoint(2,900,0.469);
      ua5[2]->SetPointError(2,0,0);
    
      ua5[3]->SetPoint(1,200,0.3);
      ua5[3]->SetPointError(1,0,0);
      ua5[3]->SetPoint(2,900,-0.112);
      ua5[3]->SetPointError(2,0,0);
    }
    if(momtype.Contains("H")){
      ua5[0]->SetPoint(1,200,0.265);
      ua5[0]->SetPointError(1,0,0);
      ua5[0]->SetPoint(2,900,0.324);
      ua5[0]->SetPointError(2,0,0);
    
      ua5[1]->SetPoint(1,200,0.107);
      ua5[1]->SetPointError(1,0,0);
      ua5[1]->SetPoint(2,900,0.153);
      ua5[1]->SetPointError(2,0,0);
    
      ua5[2]->SetPoint(1,200,0.032);
      ua5[2]->SetPointError(1,0,0);
      ua5[2]->SetPoint(2,900,0.069);
      ua5[2]->SetPointError(2,0,0);
    
      ua5[3]->SetPoint(1,200,0.027);
      ua5[3]->SetPointError(1,0,0);
      ua5[3]->SetPoint(2,900,-0.006);
      ua5[3]->SetPointError(2,0,0);
    }
  }
  
  ua5[0]->SetMarkerStyle(markers[2]);
  ua5[1]->SetMarkerStyle(markersopen[2]);
  ua5[2]->SetMarkerStyle(markers[2]);
  ua5[3]->SetMarkerStyle(markersopen[2]);
  ua5[0]->SetMarkerSize(2);
  ua5[1]->SetMarkerSize(2);
  ua5[2]->SetMarkerSize(2);
  ua5[3]->SetMarkerSize(2);
  ua5[0]->SetMarkerColor(kBlue);
  ua5[1]->SetMarkerColor(kBlue);
  ua5[2]->SetMarkerColor(kBlue);
  ua5[3]->SetMarkerColor(kBlue);
  ua5[0]->SetLineColor(kBlue);
  ua5[1]->SetLineColor(kBlue);
  ua5[2]->SetLineColor(kBlue);
  ua5[3]->SetLineColor(kBlue);
  
  
  
  TCanvas* c_mom = new TCanvas("c_mom","c_mom",700,900);
  c_mom->Divide(1,2);
  c_mom->cd(1);
  
  if(ua5[0]->GetN()==0) xmin = 800;
  hmoments[2]->GetXaxis()->SetRangeUser(xmin,xmax);
  hmoments[2]->GetYaxis()->SetRangeUser(0.,maxrange);
  hmoments[2]->SetTitle(TString(";#sqrt{s};")+momtype+TString("_{q}"));
  hmoments[2]->Draw("E1");
  
  gPad->SetLogx();
  gPad->SetLeftMargin(0.17);
  gPad->SetBottomMargin(0.01);
  gPad->SetFillColor(0);
  gPad->GetFrame()->SetFillColor(21);
  gPad->GetFrame()->SetBorderSize(0);
  gPad->SetGrid(0,0);
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(kFALSE);

  hmoments[3]->Draw("same E1");
  //hmoments[4]->Draw("same E1");
  //hmoments[5]->Draw("same E1");
  
  if(ua5[0]->GetN()) ua5[0]->Draw("samep");
  if(ua5[1]->GetN()) ua5[1]->Draw("samep");
  //ua5[2]->Draw("same");
  //ua5[3]->Draw("same");
  
  
  for(int m = 2 ; m < 4 ; ++m){
    gmoments.at(m)->Draw("same z ");
    hmoments.at(m)->Draw("same E1 ");
  }
  
  
  #include "../macro/acceptanceMap.C"
  ostringstream legheader("");
  legheader<< "pt >= "<<accMap.at(acc).at(0)<<" GeV       |#eta| < "<<accMap.at(acc).at(1);

  TLegend* leg = new TLegend(0.2,0.60,0.4,0.93);
  leg->SetHeader(legheader.str().c_str());
  leg->SetTextSize(0.05);
  leg->AddEntry(hmoments[2],momtype+TString("_{2} CMS"),"p");
  if(ua5[0]->GetN()) leg->AddEntry(ua5[0],momtype+TString("_{2} UA5"),"p");
  leg->AddEntry(hmoments[3],momtype+TString("_{3} CMS"),"p");
  if(ua5[1]->GetN()) leg->AddEntry(ua5[1],momtype+TString("_{3} UA5"),"p");
  //leg->AddEntry(hmoments[4],momtype+TString("_{4}"),"l");
  //leg->AddEntry(hmoments[5],momtype+TString("_{5}"),"l");
  //leg->AddEntry(hmoments[2],"CMS","p");
  //leg->AddEntry(ua5[0],"UA5","p");
  leg->SetFillColor(kWhite);
  leg->Draw("same");
  
  
  c_mom->cd(2);
  
  hmoments[4]->GetXaxis()->SetRangeUser(xmin,xmax);
  hmoments[4]->GetYaxis()->SetRangeUser(0.,maxrange);
  hmoments[4]->SetTitle(TString(";#sqrt{s} [GeV];")+momtype+TString("_{q}  "));
  
  hmoments[4]->Draw("E1");
  
  
  gPad->SetLogx();
  gPad->SetLeftMargin(0.17);
  gPad->SetTopMargin(0.00);
  gPad->SetBottomMargin(0.15);
  gPad->SetFillColor(0);
  gPad->GetFrame()->SetFillColor(21);
  gPad->GetFrame()->SetBorderSize(0);
  gPad->SetGrid(0,0);
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(kFALSE);
  
  hmoments[5]->Draw("same E1");
    
  if(ua5[2]->GetN()) ua5[2]->Draw("samep");
  if(ua5[3]->GetN()) ua5[3]->Draw("samep");
  
  for(int m = 4 ; m < 6 ; ++m){
    gmoments.at(m)->Draw("same z ");
    hmoments.at(m)->Draw("same E1 ");
  }
  
  leg = new TLegend(0.2,0.60,0.4,0.98);
  leg->SetHeader(legheader.str().c_str());
  leg->SetTextSize(0.05);
  leg->AddEntry(hmoments[4],momtype+TString("_{4} CMS"),"p");
  if(ua5[2]->GetN()) leg->AddEntry(ua5[2],momtype+TString("_{4} UA5"),"p");
  leg->AddEntry(hmoments[5],momtype+TString("_{5} CMS"),"p");
  if(ua5[3]->GetN()) leg->AddEntry(ua5[3],momtype+TString("_{5} UA5"),"p");
  //leg->AddEntry(hmoments[4],momtype+TString("_{4}"),"l");
  //leg->AddEntry(hmoments[5],momtype+TString("_{5}"),"l");
  //leg->AddEntry(hmoments[2],"CMS","p");
  //leg->AddEntry(ua5[0],"UA5","p");
  leg->SetFillColor(kWhite);
  leg->Draw("same");
  
  
  gPad->Update();
  cout<<"Ready to print fig !"<<endl;
  
  gPad->WaitPrimitive();
  c_mom->SaveAs(momtype+TString("moments")+basefig.str()+TString(".gif"),"");
  c_mom->SaveAs(momtype+TString("moments")+basefig.str()+TString(".eps"),"");
  c_mom->SaveAs(momtype+TString("moments")+basefig.str()+TString(".root"),"");
  gSystem->Exec(TString("convert ")+momtype+TString("moments")+basefig.str()+TString(".eps ")+momtype+TString("moments")+basefig.str()+TString(".pdf"));
  
  
  
}
