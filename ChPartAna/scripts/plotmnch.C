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
//#include "TFitResult.h"
#include "TVirtualFitter.h"
#include "TMatrixD.h"

#include "TFrame.h"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>

#include "../macro/fileManager.C"
#include "../plugins/TMoments.C"

void plotmnch(double acc = 5){

  #include "../macro/acceptanceMap.C"

  gROOT->ProcessLine(".x cmsStyleRoot.C");
  
  //TString globaldir = ("../plots/systv10_/");
  TString globaldir = ("/user/rougny/Ferenc_Tracking_bis/CMSSW_3_3_6_patch3/src/UAmulti/ChPartAna/plots/systv10_binning1v6_2/");

  const int nenergy = 3;
  
  int colors[]  = {1,1,1,2,4,kMagenta};
  int markers[] = {1,1,kFullCircle,kFullStar,kFullSquare,kFullTriangleUp};
  int markersopen[] = {1,1,kOpenCircle,kOpenStar,kOpenSquare,kOpenTriangleUp};
  
  int nbin = 1500;
  double xmin = 15;
  double xmax = 8500;
  double ymin = 20;
    
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
  
  //instantiating the histos
  
  //filling the histos/graphs
  TMoments* moments = NULL;
  for(int i = 0 ; i < nenergy ; ++i){
    moments = (TMoments*) files.at(i)->Get("unfolding/moments/moments_syst");
    //moments->print();
    
    int ibin = mnch->GetXaxis()->FindFixBin(E.at(i));
    
    mnch->SetBinContent(ibin , moments->mean->GetMean());
    mnch->SetBinError(ibin , moments->mean->GetMeanError());
    gmnch->SetPoint(i,E.at(i),moments->mean->GetMean());
    gmnch->SetPointError(i,0,0,moments->mean->_MeanSystM,moments->mean->_MeanSystP);
  }
  
  //---------------------------------------------------------------
  //----------------------- <Nch> VS sqrt(S) ----------------------
  //---------------------------------------------------------------
  
  TString legheader = "";
  double minleg = 0.6; 
 
  TCanvas* c_mnch = new TCanvas("c_mnch","c_mnch",600,600);
  c_mnch->cd();
  
  
  
  mnch->SetLineColor(kRed-3);
  mnch->SetMarkerColor(kRed-3);
  mnch->SetMarkerStyle(kFullCircle);
  mnch->SetMarkerSize(2);
  mnch->SetTitle(";#sqrt{s} [GeV];<n>");
  mnch->GetXaxis()->SetTitleSize(0.07);
  
  // Old experiments

  TGraphErrors* UA5mean = new TGraphErrors();
  TGraphErrors* ISR     = new TGraphErrors();
  TGraphErrors* H1      = new TGraphErrors();
  TGraphErrors* UA1     = new TGraphErrors();
  TGraphErrors* NA22    = new TGraphErrors();
  TGraphErrors* CDF     = new TGraphErrors();
  TGraphErrors* ALICE   = new TGraphErrors();
 
  // Theory:
  TGraphErrors* Likhoded = new TGraphErrors();
  TGraphErrors* Levin    = new TGraphErrors();
  TGraphErrors* QGSM     = new TGraphErrors();
  TGraphErrors* Gotsman  = new TGraphErrors();
 
  if(acc==0 || acc==5){
    ymin = 4.;
    legheader = " |#eta| < 2.4";
  
    NA22->SetPoint(0,22.,7.2);
    NA22->SetPointError(0,0,0.1);
  
    UA1->SetPoint(0,200,14.4);
    UA1->SetPointError(0,0,0.4);
    UA1->SetPoint(1,260,15.4);
    UA1->SetPointError(1,0,0.5);
    UA1->SetPoint(2,380,16.4);
    UA1->SetPointError(2,0,0.6);
    UA1->SetPoint(3,500,17.);
    UA1->SetPointError(3,0,0.6);
    UA1->SetPoint(4,620,17.7);
    UA1->SetPointError(4,0,0.6);
    UA1->SetPoint(5,790,18.8);
    UA1->SetPointError(5,0,0.8);
    UA1->SetPoint(6,900,19.1);
    UA1->SetPointError(6,0,0.6);
    
    UA5mean->SetPoint(0,200.,13.2);
    UA5mean->SetPointError(0,0,0.3);
    UA5mean->SetPoint(1,546,15.9);
    UA5mean->SetPointError(1,0,0.1);
    UA5mean->SetPoint(2,900,18.8);
    UA5mean->SetPointError(2,0,0.7);
  }
  if(acc==2 || acc==7){
    ymin = 2.;
    legheader = " |#eta| < 1.5";
    minleg = 0.5; 
 
    NA22->SetPoint(0,22.,4.61);
    NA22->SetPointError(0,0,0.08);
    
    
    ISR->SetPoint(0,24,4.49);
    ISR->SetPointError(0,0,0.06);
    ISR->SetPoint(1,31,5.00);
    ISR->SetPointError(1,0,0.08);
    ISR->SetPoint(2,45,5.49);
    ISR->SetPointError(2,0,0.06);
    ISR->SetPoint(3,53,5.775);
    ISR->SetPointError(3,0,0.06);
    ISR->SetPoint(4,63,6.16);
    ISR->SetPointError(4,0,0.06);
    
    
    UA1->SetPoint(0,540,9.8);
    UA1->SetPointError(0,0,0.7);
  
    UA5mean->SetPoint(0,200,7.94);
    UA5mean->SetPointError(0,0,0.23);
    UA5mean->SetPoint(1,546,9.49);
    UA5mean->SetPointError(1,0,0.08);
    UA5mean->SetPoint(2,900,11.02);
    UA5mean->SetPointError(2,0,0.32);
    
    CDF->SetPoint(0,630.,10.3);
    CDF->SetPointError(0,0,1.2);
    CDF->SetPoint(1,1800.,13.1);
    CDF->SetPointError(1,0,1.7);
  }
  else if(acc==9){
    legheader = " |#eta| < 0.5";
    ymin = 0.75;
    minleg = 0.5; 

    NA22->SetPoint(0,22.,1.52);
    NA22->SetPointError(0,0,0.05);
    
    H1->SetPoint(0,96.9,2.46);
    H1->SetPointError(0,0,0.1);
    H1->SetPoint(1,132.,2.5);
    H1->SetPointError(1,0,0.12);
    H1->SetPoint(2,166.8,2.63);
    H1->SetPointError(2,0,0.18);
    H1->SetPoint(3,201.9,2.66);
    H1->SetPointError(3,0,0.18);
    
    
    UA1->SetPoint(0,200,2.65);
    UA1->SetPointError(0,0,0.08);
    UA1->SetPoint(1,260,2.71);
    UA1->SetPointError(1,0,0.08);
    UA1->SetPoint(2,380,2.94);
    UA1->SetPointError(2,0,0.09);
    UA1->SetPoint(3,500,3.05);
    UA1->SetPointError(3,0,0.09);
    UA1->SetPoint(4,620,3.15);
    UA1->SetPointError(4,0,0.09);
    UA1->SetPoint(5,790,3.41);
    UA1->SetPointError(5,0,0.1);
    UA1->SetPoint(6,900,3.48);
    UA1->SetPointError(6,0,0.1);
  
    //UA1->SetPoint(1,540,3.3);
    //UA1->SetPointError(1,0,0.2);
/*    
    UA5mean->SetPoint(1,200,2.5);
    UA5mean->SetPointError(1,0,0.09);
    UA5mean->SetPoint(2,546,3.01);
    UA5mean->SetPointError(2,0,0.03);
    UA5mean->SetPoint(3,900,3.61);
    UA5mean->SetPointError(3,0,0.13);
*/

    // Same as dN/deta paper
    UA5mean->SetPoint(0,53, 1.93 );
    UA5mean->SetPointError(0,0,0.10);
    UA5mean->SetPoint(1,200,2.5);
    UA5mean->SetPointError(1,0,0.09);
    UA5mean->SetPoint(2,546,3.01);
    UA5mean->SetPointError(2,0,0.03);
    UA5mean->SetPoint(3,900,3.61);
    UA5mean->SetPointError(3,0,0.13);


/*
    UA5mean->SetPoint(2,200,2.48);
    UA5mean->SetPointError(2,0,0.08);
    UA5mean->SetPoint(3,546,3.05);
    UA5mean->SetPointError(3,0,0.09);
    UA5mean->SetPoint(4,900,3.48);
    UA5mean->SetPointError(4,0,0.12);
*/    
    
    ALICE->SetPoint(0,900,3.58);
    ALICE->SetPointError(0,0,0.12);
    ALICE->SetPoint(1,2360,4.43);
    ALICE->SetPointError(1,0,0.15);
  }
  
  
  TGraphAsymmErrors* nchmean_all = new TGraphAsymmErrors(UA5mean->GetN()+NA22->GetN()+CDF->GetN()+UA1->GetN()+ISR->GetN()+nenergy+1);
  int np = 0;
  
  //Setting point 0,0
  nchmean_all->SetPoint(0,0,0);
  nchmean_all->SetPointError(0,0,0,0,0);
  ++np;  
  
  for(int i = 0 ; i < ISR->GetN() ; ++i){
    double x = 0 , y = 0;
    ISR->GetPoint(i,x,y);
    nchmean_all->SetPoint(i,x,y);
    nchmean_all->SetPointError(i,0,0,ISR->GetErrorYlow(i),ISR->GetErrorYhigh(i));
  }
  np+=ISR->GetN();
  
  for(int i = np ; i < NA22->GetN() + np ; ++i){
    double x = 0 , y = 0;
    NA22->GetPoint(i - np,x,y);
    nchmean_all->SetPoint(i,x,y);
    nchmean_all->SetPointError(i,0,0,NA22->GetErrorYlow(i- np),NA22->GetErrorYhigh(i- np));
  }
  np+=NA22->GetN();
/*  
  for(int i = np ; i < H1->GetN() + np ; ++i){
    double x = 0 , y = 0;
    H1->GetPoint(i - np,x,y);
    nchmean_all->SetPoint(i,x,y);
    nchmean_all->SetPointError(i,0,0,H1->GetErrorYlow(i- np),H1->GetErrorYhigh(i- np));
  }
  np+=H1->GetN();
*/  

  /*for(int i = np ; i < UA1->GetN() + np ; ++i){
    double x = 0 , y = 0;
    UA1->GetPoint(i - np,x,y);
    nchmean_all->SetPoint(i,x,y);
    nchmean_all->SetPointError(i,0,0,UA1->GetErrorYlow(i- np),UA1->GetErrorYhigh(i- np));
  }
  np+=UA1->GetN();*/
  
  for(int i = np ; i < UA5mean->GetN() + np ; ++i){
    double x = 0 , y = 0;
    UA5mean->GetPoint(i - np,x,y);
    nchmean_all->SetPoint(i,x,y);
    nchmean_all->SetPointError(i,0,0,UA5mean->GetErrorYlow(i- np),UA5mean->GetErrorYhigh(i- np));
  }
  np+=UA5mean->GetN();
  
  for(int i = np ; i < CDF->GetN() + np ; ++i){
    double x = 0 , y = 0;
    CDF->GetPoint(i - np,x,y);
    nchmean_all->SetPoint(i,x,y);
    nchmean_all->SetPointError(i,0,0,CDF->GetErrorYlow(i- np),CDF->GetErrorYhigh(i- np));
  }
  np+=CDF->GetN();
  
  for(int i = np ; i < ALICE->GetN() + np ; ++i){
    double x = 0 , y = 0;
    ALICE->GetPoint(i - np,x,y);
    nchmean_all->SetPoint(i,x,y);
    nchmean_all->SetPointError(i,0,0,ALICE->GetErrorYlow(i- np),ALICE->GetErrorYhigh(i- np));
  }
  np+=ALICE->GetN();
  
  for(int i =  np; i <  nchmean_all->GetN() ; ++i){
    double x = 0 , y = 0;
    gmnch->GetPoint(i-np,x,y);
    
    //skip last point in the fits
    if(x==7000) continue;
    
    nchmean_all->SetPoint(i,x,y);
    nchmean_all->SetPointError(i,0,0,gmnch->GetErrorYlow(i-np),gmnch->GetErrorYhigh(i-np));
  }
  
  // Read ../theorydata/nchmean_sqrts_Likhoded 
  int   n = 0;
  const int  nmax = 400 ;
  double s[nmax],s1[nmax],s2[nmax],s3[nmax],s4[nmax],s5[nmax],s6[nmax],s7[nmax],s8[nmax],s9[nmax],s10[nmax];
  double zero[nmax];
  ifstream mydata ;


  mydata.open ("../theorydata/nchmean_sqrts_Likhoded");
  while (mydata >>  s[n] >> s1[n] >> s2[n] >> s3[n] >> s4[n] >> s5[n] >> s6[n] >> s7[n] >> s8[n] >> s9[n] >> s10[n] ) { zero [n] = 0. ; n++; } 
  mydata.close();

  if ( acc == 5 ) Likhoded = new TGraphErrors(n,s,s5,zero,zero);   
  if ( acc == 6 ) Likhoded = new TGraphErrors(n,s,s4,zero,zero);   
  if ( acc == 7 ) Likhoded = new TGraphErrors(n,s,s3,zero,zero);   
  if ( acc == 8 ) Likhoded = new TGraphErrors(n,s,s2,zero,zero);   
  if ( acc == 9 ) Likhoded = new TGraphErrors(n,s,s1,zero,zero);   
  ymin = 0.;


  if ( acc == 5 ) {
    double x[3] , y[3] ;
    n=3;
    x[0]=900  ; y[0] = 3.672*4.8 ;
    x[1]=2360 ; y[1] = 4.672*4.8 ;
    x[2]=7000 ; y[2] = 6.008*4.8 ;
    Levin = new TGraphErrors(n,x,y ,zero,zero);
  }
 
  if ( acc == 7 ) {
    n=3;
    double x[3] , y[3] ;
    x[0]=900 ; y[0] = 3.621*3 ;
    x[1]=2360; y[1] = 4.574*3 ;
    x[2]=7000; y[2] = 5.858*3 ;
    Levin = new TGraphErrors(n,x,y ,zero,zero);
  }


  if ( acc == 9 ) {
    n = 0;
    mydata.open ("../theorydata/nchmena_sqrts_levin_eta0.5");
    while (mydata >>  s[n] >> s1[n] ) { zero [n] = 0. ; n++; } 
    mydata.close();
    Levin = new TGraphErrors(n,s,s1,zero,zero);
  }
  
  if( acc == 5 ){
    n = 3;
    double x[n] , y[n] ;
    x[0]=900  ; y[0] = 18*2.4/2.5;
    x[1]=7000 ; y[1] = 25.7*2.4/2.5 ;
    x[2]=10000; y[2] = 27.1*2.4/2.5 ;
    QGSM = new TGraphErrors(n,x,y ,zero,zero);
  }

  
  if( acc == 5 ){
    n = 7;
    double x[n] , y[n] ;
    x[0]=200  ; y[0] = 5.9638*2;
    x[1]=546  ; y[1] = 7.9721*2;
    x[2]=900  ; y[2] = 8.8706*2;
    x[3]=1800 ; y[3] = 10.3884*2;
    x[4]=2360 ; y[4] = 10.9615*2;
    x[5]=7000 ; y[5] = 12.8045*2;
    x[6]=14000; y[6] = 14.2964*2;
    Gotsman = new TGraphErrors(n,x,y ,zero,zero);
  }

  //mnch->GetXaxis()->SetRangeUser(xmin,xmax);
  mnch->GetYaxis()->SetRangeUser(ymin,gmnch->GetYaxis()->GetXmax()*1.02);
  //cout<<gmnch->GetYaxis()->GetXmin()<<"  "<<gmnch->GetYaxis()->GetXmax()*1.03<<endl;
  mnch->GetXaxis()->SetTitleOffset(1.0);
  
  c_mnch->cd();
  mnch->Draw("E1");
  //gPad->WaitPrimitive();
  
  
  // Theory 

  Likhoded->SetLineColor(kMagenta+1);
  Likhoded->SetLineStyle(3);
  Likhoded->SetLineWidth(3);
  Likhoded->Draw("same l"); 
  

  if ( acc == 9 || acc == 7 || acc == 5) {
    Levin->SetLineColor(kGreen+1);
    Levin->SetLineWidth(3);
    Levin->SetLineStyle(7);
    Levin->Draw("same l");
  }
  
  if ( acc == 5) {
    QGSM->SetLineColor(kOrange-5);
    QGSM->SetLineWidth(3);
    QGSM->SetLineStyle(10);
    QGSM->Draw("same l");
  }
  
  if ( acc == 5) {
    Gotsman->SetLineColor(kBlue);
    Gotsman->SetLineWidth(4);
    Gotsman->SetLineStyle(6);
    Gotsman->Draw("same l");
  }
  
  gmnch->Draw("same z");
    
  gPad->SetLogx();
  gPad->SetLeftMargin(0.17);
  gPad->SetBottomMargin(0.15);
  gPad->SetFillColor(0);
  gPad->GetFrame()->SetFillColor(21);
  gPad->GetFrame()->SetBorderSize(12);
  gPad->SetGrid(0,0);
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(kFALSE);
  
  // Fits 
  
  //TF1* fua5mnch = new TF1("fua5mnch","[0] + [1] * sqrt(x) + [2] * x",5,15000);
  //TF1* fua5mnch = new TF1("fua5mnch","[0] + [1] * x + [2] * x * x",5,15000);
  //TF1* f1 = new TF1("f1","[0] + [1] * log(x*x) + [2] * log(x*x) * log(x*x) + [3]*pow(log(x*x),3)",5,15000);
  //TF1* f1 = new TF1("f1","[0] + [1] * log(x*x)  + [2]*pow(log(x*x),3)",5,15000);
  TF1* f1 = new TF1("f1","[0] + [1] * log(x*x) + [2] * log(x*x) * log(x*x) ",5,10000);
  //TF1* f1 = new TF1("f1","[0] + 0 * log(x*x) + [1] * log(x*x) * log(x*x) ",5,15000);
  f1->SetLineWidth(1);
  f1->SetLineColor(kBlack);
  f1->SetFillColor(16);
  f1->SetFillStyle(4100);
  f1->SetLineStyle(1);
  f1->SetParameters(2.5,-.5,0.05);
  //f1->SetParameters(2.5,0.05);
  //f1->SetParameter(0,2.5);
  nchmean_all->Fit("f1","R");
    
  
  f1->SetLineWidth(2);
  //f1->Draw("same");
  
  #include "plotmnch_resampling.C"
  
  
  TF1* f2 = new TF1("f2","[0] + [1] * pow(x*x,[2])",5,15000);
  f2->SetParameters(0,1.,0.5);
  f2->SetLineWidth(1);
  f2->SetLineStyle(2);
  f2->SetLineColor(kRed);
  nchmean_all->Fit("f2");
  //f2->Draw("same");
  
  TF1* f3 = new TF1("f3","[0] + [1] * exp([2]*sqrt(log(x*x)))",5,15000);
  f3->SetParameters(4,7.,0.5);
  f3->SetLineWidth(1);
  f3->SetLineColor(kBlue);
  f3->SetLineStyle(3);
  nchmean_all->Fit("f3");
  //f3->Draw("same");
  
  TF1* f4 = new TF1("f4","[0]*pow(x*x,-1*[1]) + [2] * pow(x*x,[3])",5,15000);
  f4->SetParameters(1,2.5,1,0.17);
  f4->SetParameters(4,7.,0.5);
  f4->SetLineWidth(1);
  f4->SetLineColor(kMagenta);
  nchmean_all->Fit("f4");
  //f4->Draw("same");
  
  TF1* fdndeta = new TF1("dndeta","[0] + [1] * log(x*x) + [2] * log(x*x) * log(x*x) ",50,15000);
  fdndeta->SetParameters(2.716,-0.307,0.0267);
  //fdndeta->Draw("same");
 
 
  //gf1->Draw("3same");
  Likhoded->Draw("same l"); 
  if ( acc == 9 || acc == 7 || acc == 5)
    Levin->Draw("same l");
  if ( acc == 5 )
    QGSM->Draw("same l");
  if ( acc == 5 )
    Gotsman->Draw("same l");
  
  
  UA5mean->SetLineColor(kBlue);
  UA5mean->SetMarkerColor(kBlue);
  UA5mean->SetMarkerSize(2);
  UA5mean->SetMarkerStyle(kOpenCross);
  if(UA5mean->GetN()!=0) UA5mean->Draw("same p");
  
  
  ISR->SetLineColor(kBlue);
  ISR->SetMarkerColor(kBlue);
  ISR->SetMarkerSize(1);
  ISR->SetMarkerStyle(kOpenSquare);
  if(ISR->GetN()!=0) ISR->Draw("same p");
  
  NA22->SetLineColor(kBlue);
  NA22->SetMarkerColor(kBlue);
  NA22->SetMarkerSize(2);
  NA22->SetMarkerStyle(kOpenTriangleUp);
  if(NA22->GetN()!=0) NA22->Draw("same p");
  
  H1->SetLineColor(kBlue);
  H1->SetMarkerColor(kBlue);
  H1->SetMarkerSize(2);
  H1->SetMarkerStyle(kOpenCircle);
  if(H1->GetN()!=0) H1->Draw("same p");
  
  UA1->SetLineColor(kBlue);
  UA1->SetMarkerColor(kBlue);
  UA1->SetMarkerSize(2);
  UA1->SetMarkerStyle(kOpenStar);
  if(UA1->GetN()!=0) UA1->Draw("same p");
  
  CDF->SetLineColor(kBlue);
  CDF->SetMarkerColor(kBlue);
  CDF->SetMarkerSize(2);
  CDF->SetMarkerStyle(kOpenCircle);
  if(CDF->GetN()!=0) CDF->Draw("same p");
    
  ALICE->SetLineColor(kBlue);
  ALICE->SetMarkerColor(kBlue);
  ALICE->SetMarkerSize(2);
  ALICE->SetMarkerStyle(kOpenDiamond);
  if(ALICE->GetN()!=0) ALICE->Draw("same p");
  
  
  //Charged Hadron fits
  TF1* pubFit = new TF1();
  if(acc == 9) pubFit = new TF1("pubFit","2.716 - 0.307*log(x*x) + 0.0267*log(x*x)*log(x*x)",50,7500);
  if(acc == 7) pubFit = new TF1("pubFit","8.148 - 0.921*log(x*x) + 0.0801*log(x*x)*log(x*x)",50,7500);
  if(acc == 5) pubFit = new TF1("pubFit","13.0368 - 1.4736*log(x*x) + 0.12816*log(x*x)*log(x*x)",50,7500);
  //pubFit->SetParameter(0,0);
  pubFit->SetLineColor(kRed);
  pubFit->SetLineWidth(2);
  /*
  cout<<"GGGGGGGGGGGGGGGGGGG           "<<(*pubFit)(100)<<endl;
  
  TCanvas* cc = new TCanvas("cc","cc",500,500);
  cc->cd();*/
  //gPad->WaitPrimitive();
  
  
  
  TLegend* leg = new TLegend(0.25,minleg,0.45,0.90);
  leg->SetHeader(legheader);
  if(NA22->GetN()!=0)    leg->AddEntry(NA22,"NA22","p");
  if(ISR->GetN()!=0)     leg->AddEntry(ISR,"ISR","p");
  if(H1->GetN()!=0)      leg->AddEntry(H1,"H1","p");
  if(UA1->GetN()!=0)     leg->AddEntry(UA1,"UA1","p");
  if(UA5mean->GetN()!=0) leg->AddEntry(UA5mean,"UA5","p");
  if(CDF->GetN()!=0)     leg->AddEntry(CDF,"CDF","p");
  if(ALICE->GetN()!=0)   leg->AddEntry(ALICE,"ALICE","p");
  leg->AddEntry(mnch,"CMS","p");
  leg->SetFillColor(kWhite);
  
  leg->Draw("SAME");
  
  mnch->Draw("SAME");
  //if(UA5mean->GetN()) UA5mean->Draw("same p");

  TLatex* text = new TLatex(0.50,0.87,"CMS");
  text->SetNDC(kTRUE);
  text->SetTextSize(0.05);
  text->DrawLatex(0.50,0.87,"CMS");

  double ylegmin = 0.2 , ylegmax = 0.3;
  if(acc==5){
    ylegmin = 0.18;
    ylegmax = 0.32;
  }
  TLegend* legfunc = new TLegend(0.35,ylegmin,0.90,ylegmax);
 
  legfunc->AddEntry(Likhoded,"Likhoded et al.","l");
  if ( acc == 9 || acc == 7 || acc == 5 ) 
    legfunc->AddEntry(Levin,"Levin et al.","l");
  if ( acc == 5 ) 
    legfunc->AddEntry(QGSM,"Kaidalov and Poghosyan.","l");
  if ( acc == 5 ) 
    legfunc->AddEntry(Gotsman,"Gotsman et al.","l");
 
  ostringstream func("");
  func<<fixed<<setprecision(3)<<f1->GetParameter(0);
  if ( f1->GetParameter(1) >= 0 ) func <<" + " << f1->GetParameter(1) ;
  else                            func <<" - " << -1*(f1->GetParameter(1));  
  func <<" ln(s) + "<<f1->GetParameter(2)<<" ln^{2}(s)";
  cout<<func.str().c_str()<<endl;
  
  legfunc->AddEntry(gf1,func.str().c_str(),"lf");
  
  func.str("");
  func<<fixed<<setprecision(3)<<f2->GetParameter(0)<<" + "<<
        f2->GetParameter(1)<<" s^{"<<
	f2->GetParameter(2)<<"}";
  cout<<func.str().c_str()<<endl;
  
//  legfunc->AddEntry(f2,func.str().c_str(),"l");
  
  
  func.str("");
  func<<fixed<<setprecision(3)<<f3->GetParameter(0)<<" + "<<
        f3->GetParameter(1)<<" e^{"<<
	f3->GetParameter(2)<<" #sqrt{ln(s)}}";
  cout<<func.str().c_str()<<endl;
  
//  legfunc->AddEntry(f3,func.str().c_str(),"l");
  
  
  legfunc->SetFillColor(kWhite);
  legfunc->Draw("same");

  TLatex* textnchmean = new TLatex(0.3,0.15,func.str().c_str());
  textnchmean->SetNDC(kTRUE);
  textnchmean->SetTextSize(0.03);
  //textnchmean->DrawLatex(0.45,0.15,func.str().c_str());
  
  /*TCanvas* c_all = new TCanvas("c_all","c_all",500,500);
  c_all->cd();
  
  nchmean_all->Draw("ap");*/
  
  
  //f1->Draw("fcsame");
  
  //pubFit->Draw("same");
  
  gPad->WaitPrimitive();
  gPad->SaveAs(TString("../figs/nchmean")+basefig.str()+TString(".gif"),"");
  gPad->SaveAs(TString("../figs/nchmean")+basefig.str()+TString(".eps"),"");
  gPad->SaveAs(TString("../figs/nchmean")+basefig.str()+TString(".root"),"");
  gSystem->Exec(TString("convert ")+TString("../figs/nchmean")+basefig.str()+TString(".eps ")+TString("../figs/nchmean")+basefig.str()+TString(".pdf"));
  
}
