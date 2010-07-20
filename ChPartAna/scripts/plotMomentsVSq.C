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

void plotMomentsVSq(int acc = 5 , TString momtype = "C"){

  gROOT->ProcessLine(".x cmsStyleRoot.C");
  
  TString globaldir = ("/user/rougny/Ferenc_Tracking_bis/CMSSW_3_3_6_patch3/src/UAmulti/ChPartAna/plots/current_b1_2/");

  const int nenergy = 3;
  
  int colors[]  = {1,1,1,2,4,kMagenta};
  int markers[] = {1,1,kFullCircle,kFullSquare,kFullTriangleUp};
  int markersopen[] = {1,1,kOpenCircle,kOpenSquare,kOpenTriangleUp};
  
  int nbin = 1500;
  double xmin = 1.5;
  double xmax = 5.5;
  
  double ymin1 = 0.  , ymin2 = 0.;
  double ymax1 = 80. , ymax2 = 80;
  
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
  
  vector<TGraphErrors*> hmoments(E.size(),new TGraphErrors());
  vector<TGraphAsymmErrors*> gmoments(E.size(),new TGraphAsymmErrors());
  
  //instantiating the histos
  for(int e = 0 ; e < E.size() ; ++e){
    hmoments.at(e) = new TGraphErrors();
    gmoments.at(e) = new TGraphAsymmErrors();
  }
  
  cout<<"+++   Taking the moments from the file ..."<< endl;
  
  //filling the histos/graphs
  for(int i = 0 ; i < nenergy ; ++i){
    TMoments* moments = (TMoments*) files.at(i)->Get("unfolding/moments/moments_syst");
      
    for(int m = 2 ; m < nmoments ; ++m){
      if(momtype.Contains("C")){
        hmoments.at(i)->SetPoint(m-2, m , moments->cmoments->at(m));
        hmoments.at(i)->SetPointError(m-2, 0 , moments->cstaterr->at(m));
        gmoments.at(i)->SetPoint(m-2, m , moments->cmoments->at(m));
        gmoments.at(i)->SetPointError(i,0,0,moments->csystmerr->at(m),moments->csystperr->at(m));
	//cout<<m<<"  "<<i<<"  "<<E.at(i)<<"  "<<gmoments.at(m)->GetErrorYlow(i)<<"  "<<gmoments.at(m)->GetErrorYhigh(i)<<endl;
	//cout<<m<<"  "<<i<<"  "<<E.at(i)<<"  "<<moments->csystmerr->at(m)<<"  "<<moments->csystperr->at(m)<<endl;
      }
      if(momtype.Contains("F")){
        hmoments.at(i)->SetPoint(m-2, m , moments->fmoments->at(m));
        hmoments.at(i)->SetPointError(m-2, 0 , moments->fstaterr->at(m));
        gmoments.at(i)->SetPoint(m-2, m , moments->fmoments->at(m));
        gmoments.at(i)->SetPointError(i,0,0,moments->fsystmerr->at(m),moments->fsystperr->at(m));
      }
      if(momtype.Contains("K")){
        hmoments.at(i)->SetPoint(m-2, m , moments->cmoments->at(m));
        hmoments.at(i)->SetPointError(m-2, 0 , moments->kstaterr->at(m));
        gmoments.at(i)->SetPoint(m-2, m , moments->kmoments->at(m));
        gmoments.at(i)->SetPointError(i,0,0,moments->ksystmerr->at(m),moments->ksystperr->at(m));
      }
      if(momtype.Contains("H")){
        hmoments.at(i)->SetPoint(m-2, m , moments->hmoments->at(m));
        hmoments.at(i)->SetPointError(m-2, 0 , moments->hstaterr->at(m));
        gmoments.at(i)->SetPoint(m-2, m , moments->hmoments->at(m));
        gmoments.at(i)->SetPointError(i,0,0,moments->hsystmerr->at(m),moments->hsystperr->at(m));
      }
    }
    
    hmoments.at(i)->SetFillColor(kRed-3);
    hmoments.at(i)->SetLineColor(kRed-3);
    hmoments.at(i)->SetLineWidth(2);
    hmoments.at(i)->SetMarkerColor(kRed-3);
    hmoments.at(i)->SetMarkerSize(2);
      
    gmoments.at(i)->SetFillColor(kRed-3);
    gmoments.at(i)->SetLineColor(kRed-3);
    gmoments.at(i)->SetLineWidth(2);
    gmoments.at(i)->SetMarkerColor(kRed-3);
    gmoments.at(i)->SetMarkerSize(2);
  }
  
  cout<<"                                            ... done !"<< endl;
  
  hmoments.at(0)->SetMarkerStyle(markersopen[4]);
  hmoments.at(1)->SetMarkerStyle(markersopen[2]);
  hmoments.at(2)->SetMarkerStyle(markersopen[3]);
  
  gmoments.at(0)->SetMarkerStyle(markersopen[4]);
  gmoments.at(1)->SetMarkerStyle(markersopen[2]);
  gmoments.at(2)->SetMarkerStyle(markersopen[3]);
  
  
  /*hmoments[3]->Draw("same E1");
  hmoments[4]->Draw("same E1");
  hmoments[5]->Draw("same E1");*/
  
  
  cout<<"+++   Adding points from other exp  ..."<< endl;
  
  //Writing UA5
  TGraphErrors** ua5 = new TGraphErrors*[6];
  ua5[0] = new TGraphErrors();
  ua5[1] = new TGraphErrors();
  ua5[2] = new TGraphErrors();
  ua5[3] = new TGraphErrors();
  ua5[4] = new TGraphErrors();
  ua5[5] = new TGraphErrors();
  
  TGraphErrors* UA1  = new TGraphErrors();
  TGraphErrors* NA22 = new TGraphErrors();
  
  if(int(acc)%5==3){
    /*if(momtype.Contains("C")){
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
    }*/
  }
  else if(int(acc)%5==0){
  
  /*  if(momtype.Contains("C")){
      ua5[0]->SetPoint(1,200,1.44);
      ua5[0]->SetPointError(1,0,0.03);
      ua5[0]->SetPoint(2,540,1.51);
      ua5[0]->SetPointError(2,0,0.01);
      ua5[0]->SetPoint(3,880,1.53);
      ua5[0]->SetPointError(3,0,0.04);
    
      ua5[1]->SetPoint(1,200,2.65);
      ua5[1]->SetPointError(1,0,0.11);
      ua5[1]->SetPoint(2,540,3);
      ua5[1]->SetPointError(2,0,0.06);
      ua5[1]->SetPoint(3,880,3.11);
      ua5[1]->SetPointError(3,0,0.13);
    
      ua5[2]->SetPoint(1,200,5.8);
      ua5[2]->SetPointError(1,0,0.5);
      ua5[2]->SetPoint(2,540,7.4);
      ua5[2]->SetPointError(2,0,0.3);
      ua5[2]->SetPoint(3,880,7.7);
      ua5[2]->SetPointError(3,0,0.6);
    
      ua5[3]->SetPoint(1,200,15);
      ua5[3]->SetPointError(1,0,2.2);
      ua5[3]->SetPoint(2,540,21);
      ua5[3]->SetPointError(2,0,1);
      ua5[3]->SetPoint(3,880,22);
      ua5[3]->SetPointError(3,0,2);
      
      NA22->SetPoint(1,22,1.31);
      NA22->SetPointError(1,0,0.02);
      
      UA1->SetPoint(1,200,1.385);
      UA1->SetPointError(1,0,0.005);
      UA1->SetPoint(2,260,1.395);
      UA1->SetPointError(2,0,0.005);
      UA1->SetPoint(3,380,1.405);
      UA1->SetPointError(3,0,0.015);
      UA1->SetPoint(4,500,1.415);
      UA1->SetPointError(4,0,0.015);
      UA1->SetPoint(5,620,1.420);
      UA1->SetPointError(5,0,0.010);
      UA1->SetPoint(6,790,1.430);
      UA1->SetPointError(6,0,0.010);
      UA1->SetPoint(7,880,1.445);
      UA1->SetPointError(7,0,0.005);
      
      ymin1 = 1; ymax1 = 5;
      ymin2 = 3; ymax2 = 52;
      
    }
    if(momtype.Contains("F")){
      ua5[0]->SetPoint(1,200,1.36);
      ua5[0]->SetPointError(1,0,0.02);
      ua5[0]->SetPoint(2,880,1.48);
      ua5[0]->SetPointError(2,0,0.01);
    
      ua5[1]->SetPoint(1,200,2.33);
      ua5[1]->SetPointError(1,0,0.09);
      ua5[1]->SetPoint(2,880,2.88);
      ua5[1]->SetPointError(2,0,0.09);
    
      ua5[2]->SetPoint(1,200,4.7);
      ua5[2]->SetPointError(1,0,0.36);
      ua5[2]->SetPoint(2,880,6.8);
      ua5[2]->SetPointError(2,0,0.4);
    
      ua5[3]->SetPoint(1,200,11);
      ua5[3]->SetPointError(1,0,1);
      ua5[3]->SetPoint(2,880,18);
      ua5[3]->SetPointError(2,0,2.23);
      
      ymin1 = 1; ymax1 = 5;
      ymin2 = 2; ymax2 = 50;
    }
    if(momtype.Contains("K")){
      ua5[0]->SetPoint(1,200,0.36);
      ua5[0]->SetPointError(1,0,0);
      ua5[0]->SetPoint(2,880,0.48);
      ua5[0]->SetPointError(2,0,0);
    
      ua5[1]->SetPoint(1,200,0.25);
      ua5[1]->SetPointError(1,0,0);
      ua5[1]->SetPoint(2,880,0.44);
      ua5[1]->SetPointError(2,0,0);
    
      ua5[2]->SetPoint(1,200,0.151);
      ua5[2]->SetPointError(1,0,0);
      ua5[2]->SetPoint(2,880,0.469);
      ua5[2]->SetPointError(2,0,0);
    
      ua5[3]->SetPoint(1,200,0.3);
      ua5[3]->SetPointError(1,0,0);
      ua5[3]->SetPoint(2,880,-0.112);
      ua5[3]->SetPointError(2,0,0);
      
      ymin1 = 0.2; ymax1 = 1.05;
      ymin2 = -0.4; ymax2 = 1.65;
    }*/
    if(momtype.Contains("H")){
      ua5[0]->SetPoint(0,2,0.265);
      ua5[0]->SetPointError(0,0,0);
      ua5[1]->SetPoint(0,2,0.324);
      ua5[1]->SetPointError(0,0,0);
    
      ua5[0]->SetPoint(1,3,0.107);
      ua5[0]->SetPointError(1,0,0);
      ua5[1]->SetPoint(1,3,0.153);
      ua5[1]->SetPointError(1,0,0);
    
      ua5[0]->SetPoint(2,4,0.032);
      ua5[0]->SetPointError(2,0,0);
      ua5[1]->SetPoint(2,4,0.069);
      ua5[1]->SetPointError(2,0,0);
    
      ua5[0]->SetPoint(3,5,0.027);
      ua5[0]->SetPointError(3,0,0);
      ua5[1]->SetPoint(3,5,-0.006);
      ua5[1]->SetPointError(3,0,0);
      
      ymin1 = -0.1; ymax1 = 0.5;
    }
  }
  
  ua5[0]->SetMarkerStyle(markersopen[4]);
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
  
 /* NA22->SetLineColor(kBlue);
  NA22->SetMarkerColor(kBlue);
  NA22->SetMarkerSize(2);
  NA22->SetMarkerStyle(markers[3]);
  
  
  UA1->SetLineColor(kBlue);
  UA1->SetMarkerColor(kBlue);
  UA1->SetMarkerSize(2);
  UA1->SetMarkerStyle(markers[4]);*/
  
  cout<<"                                            ... done !"<< endl;
  
  TCanvas* c_mom = new TCanvas("c_mom","c_mom",500,700);
  c_mom->cd();
  
  TH1F* dummy1 = new TH1F("dummy","dummy",4,xmin,xmax);
  dummy1->GetYaxis()->SetRangeUser(ymin1 , ymax1);
  //dummy1->GetXaxis()->SetTitleOffset(1.2);
  dummy1->GetYaxis()->SetTitleOffset(1.05);
  dummy1->GetXaxis()->SetTitleSize(0.07);
  dummy1->GetYaxis()->SetTitleSize(0.07);
  dummy1->GetXaxis()->SetNdivisions(4);
  dummy1->SetTitle(TString(";q;")+momtype+TString("_{q}   "));
  dummy1->Draw();
  //hmoments[2]->GetXaxis()->SetRangeUser(10.,xmax);
  //hmoments[2]->GetYaxis()->SetRangeUser(ymin1 , ymax1);
  //hmoments[2]->SetTitle(TString(";#sqrt{s};")+momtype+TString("_{q}"));
  hmoments[0]->Draw("samep");


  //gPad->SetLogx();
  gPad->SetLeftMargin(0.17);
  gPad->SetTopMargin(0.02);
  gPad->SetBottomMargin(0.15);
  gPad->SetFillColor(0);
  gPad->GetFrame()->SetFillColor(21);
  gPad->GetFrame()->SetBorderSize(0);
  gPad->SetGrid(0,0);
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(kFALSE);
    

  hmoments[1]->Draw("same E1");
  hmoments[2]->Draw("same E1");
  
  if(ua5[0]->GetN()) ua5[0]->Draw("samep");
  if(ua5[1]->GetN()) ua5[1]->Draw("samep");
  
  if(NA22->GetN())   NA22->Draw("samep");
  if(UA1->GetN())    UA1->Draw("samep");
  
  for(int e = 0 ; e < E.size() ; ++e){
    gmoments.at(e)->Draw("same z ");
    hmoments.at(e)->Draw("same p ");
  }
  
  
  #include "../macro/acceptanceMap.C"
  ostringstream legheader("");
  legheader<< "   |#eta| < "<<accMap.at(acc).at(1);

  TLegend* leg = new TLegend(0.50,0.6,0.80,0.85);;
  leg->SetHeader(legheader.str().c_str());
  leg->SetTextSize(0.04);
   /*                  leg->AddEntry(hmoments[2],momtype+TString("_{2} CMS"),"p");
  if(NA22->GetN())   leg->AddEntry(NA22,momtype+TString("_{2} NA22"),"p");
  if(ua5[0]->GetN()) leg->AddEntry(ua5[0],momtype+TString("_{2} UA5"),"p");
  if(UA1->GetN())    leg->AddEntry(UA1,momtype+TString("_{2} UA1"),"p");*/
  if(ua5[0]->GetN()) leg->AddEntry(ua5[0],"UA5 @ 0.2  TeV","p");
  if(ua5[1]->GetN()) leg->AddEntry(ua5[1],"UA5 @ 0.9  TeV","p");
                     leg->AddEntry(hmoments[0],"CMS @ 0.9  TeV","p");
                     leg->AddEntry(hmoments[1],"CMS @ 2.36 TeV","p");
                     leg->AddEntry(hmoments[2],"CMS @ 7.0  TeV","p");
  
  //leg->AddEntry(hmoments[4],momtype+TString("_{4}"),"l");
  //leg->AddEntry(hmoments[5],momtype+TString("_{5}"),"l");
  //leg->AddEntry(hmoments[2],"CMS","p");
  //leg->AddEntry(ua5[0],"UA5","p");
  leg->SetFillColor(kWhite);
  leg->Draw("same");
  
  TLatex* text = new TLatex(0.45,0.90,"CMS Preliminary");
  text->SetNDC(kTRUE);
  text->SetTextSize(0.06);
  text->DrawLatex(0.45,0.90,"CMS Preliminary");

  gPad->Update();
  gPad->WaitPrimitive();
  
  TString dirfig = "../figs/";
  c_mom->SaveAs(dirfig+momtype+TString("momentsVSq")+basefig.str()+TString(".gif"),"");
  c_mom->SaveAs(dirfig+momtype+TString("momentsVSq")+basefig.str()+TString(".eps"),"");
  c_mom->SaveAs(dirfig+momtype+TString("momentsVSq")+basefig.str()+TString(".root"),"");
  gSystem->Exec(TString("convert ")+dirfig+momtype+TString("momentsVSq")+basefig.str()+TString(".eps ")+dirfig+momtype+TString("momentsVSq")+basefig.str()+TString(".pdf"));


  
}
