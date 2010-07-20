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

#include "../plugins/TMoments.h"


#include "../macro/fileManager.C"

using namespace std;
using namespace TMath;

double getEdgeLastFilledBin(TH1*);
double* Divide(const TArrayD* , double);

void syst_MCpt(double E = 0.9 , int cut = 5){
  
  int typeMC = 10;
  
  if(E==7){
    cout<<"This systematic can't be applied to 7 TeV Data. Exiting now ..."<<endl;
    return;
  }
 
  
  ostringstream outstr("");
  outstr << "hyp" << 1 << "_niter" << 0 << "_cut" << cut << "_DataType" << 0;
  
  TString tbase = fileManager(3,typeMC,E,1,0,0,outstr.str());
  TFile* fbase = TFile::Open(tbase,"READ");
  
  cout<<"Base file : "<<tbase<<endl;
  
  if(fbase==0){
    cout<<"ERROR !! The base file is not present. Please run the code after making the base files ! Exiting now ..."<<endl;
    return;  
  }
  
  TH1F* hbase = (TH1F*) fbase->Get("unfolding/nch_data_corrected");
  if(hbase==0){
    cout<<"ERROR !! The base plot is void. Exiting now ..."<<endl;
    return;
  }
  
  
  //------------------------------------------------------------------------------------
  //-------------------   Doing the eta dependence systematic  -------------------------
  //------------------------------------------------------------------------------------
  //   base = D6T , syst = 413 (mtx=ProQ20)
  
  
  //getting the syst base file
  outstr.str("");
  outstr << "hyp" << 1 << "_niter" << 0 << "_cut" << cut << "_DataType" << 13;
  
  TString tsyst1 = fileManager(3,13,0.9,1,0,0,outstr.str());
  TFile* fsyst1 = TFile::Open(tsyst1,"READ");
  
  cout<<"syst1 file : "<<tsyst1<<endl;
  
  if(fsyst1==0){
    cout<<"ERROR !! The syst1 file is not present. Please run the code after making the syst1 files ! Exiting now ..."<<endl;
    return;  
  }
  
  TH1F* hsyst1 = (TH1F*) fsyst1->Get("unfolding/nch_data_corrected");
  if(hsyst1==0){
    cout<<"ERROR !! The syst1 plot is void. Exiting now ..."<<endl;
    return;
  }
  
  //getting the syst syst file
  outstr.str("");
  outstr << "hyp" << 1 << "_niter" << 0 << "_cut" << cut << "_DataType" << 13;
  
  TString tsyst2 = fileManager(3,13,0.9,1,412,-1,outstr.str());
  TFile* fsyst2 = TFile::Open(tsyst2,"READ");
  
  cout<<"syst2 file : "<<tsyst2<<endl;
  
  if(fsyst2==0){
    cout<<"ERROR !! The syst2 file is not present. Please run the code after making the syst2 files ! Exiting now ..."<<endl;
    return;  
  }
  
  TH1F* hsyst2 = (TH1F*) fsyst2->Get("unfolding/nch_data_corrected");
  if(hsyst2==0){
    cout<<"ERROR !! The syst2 plot is void. Exiting now ..."<<endl;
    return;
  }
  
  TH1F* factor = new TH1F("factor","factor",hbase->GetNbinsX(),hbase->GetXaxis()->GetXbins()->GetArray());
  if(E==0.9){
    for(int i=1;i<=hbase->GetNbinsX();++i){
      if(hsyst1->GetBinContent(i)!=0)
        factor->SetBinContent(i,hsyst2->GetBinContent(i) / hsyst1->GetBinContent(i));
      else
        factor->SetBinContent(i,1.);
    }
  }
  else if(E==2.36){
    double start = -0.5;
    double ratio = 1;
    if((getEdgeLastFilledBin(hsyst1) - start)!=0)
      ratio = (getEdgeLastFilledBin(hbase) - start) / (getEdgeLastFilledBin(hsyst1) - start);
    
    //part with no stretching
    for( int i = 1 ; i < hbase->GetXaxis()->FindFixBin(start) ; ++i ){
      if(hsyst1->GetBinContent(i)!=0)
        factor->SetBinContent(i,hsyst2->GetBinContent(i) / hsyst1->GetBinContent(i));
      else
        factor->SetBinContent(i,1.);
    }
    
    //part with stretching
    for( int i = hbase->GetXaxis()->FindFixBin(start) ; i <= hbase->GetNbinsX() ; ++i ){
      int bin = hsyst1->GetXaxis()->FindFixBin( (hbase->GetBinCenter(i)-start) / ratio  +  start);
      
      if(hsyst1->GetBinContent(bin)!=0)
        factor->SetBinContent(i,hsyst2->GetBinContent(bin) / hsyst1->GetBinContent(bin));
      else
        factor->SetBinContent(i,1.);
    }
  }
  
  TH1F* hsyst = (TH1F*) hbase->Clone("eta_syst");
  hsyst->Multiply(factor);
  
  TMoments* msyst = new TMoments(hsyst);
  msyst->ComputeMoments();
  msyst->print();
  
  
  //Making output file
  
  outstr.str("");
  outstr << "hyp" << 1 << "_niter" << 0 << "_cut" << cut << "_DataType" << 0;
  
  TString toutput = fileManager(3,typeMC,E,1,510,-1,outstr.str());
  TFile* foutput  = TFile::Open(toutput,"RECREATE");
  cout<<"Output file : " << toutput << endl;
  
  
  //Making kno plot
  double knomean = hsyst->GetMean();
  
  TString tkno = toutput;
  tkno.ReplaceAll("plots/","plots/current_b1_2/");
  cout<<"Opening for kno mean the file : "<<tkno<<endl;
  TFile* fkno = TFile::Open(tkno,"READ");
  if(fkno!=0){
    TMoments* mom_kno = (TMoments*) fkno->Get("unfolding/moments/moments");
    knomean = mom_kno->mean->GetMean();
  }
  else{
    cout<<"WARNING !! The file does not exist, taking the mean of the hist instead"<<endl;
  }
  
  
  TH1F* kno = new TH1F("kno_corrected","kno_corrected;z = n_{ch} / < n_{ch} >;#psi(z)",hsyst->GetNbinsX(),Divide(hsyst->GetXaxis()->GetXbins(),knomean));
  kno->Sumw2();
  /*for( int k = 60 ; k <= nch_corrected->GetNbinsX() ; ++k)
    nch_corrected->SetBinContent(k,0);*/
  for( int k = 1 ; k <= hsyst->GetNbinsX() ; ++k){
    kno->SetBinContent(k , knomean * hsyst->GetBinContent(k) / hsyst->Integral());
    kno->SetBinError(k , knomean * hsyst->GetBinError(k) / hsyst->Integral());
  }
  
  
  
  foutput->cd();
  foutput->mkdir("unfolding");
  foutput->cd("unfolding");
  factor->Write();
  
  hsyst->Write("nch_data_corrected");
  kno->Write();
  
  gDirectory->mkdir("moments");
  gDirectory->cd("moments");
  msyst->Write("moments");
  
  foutput->Close();
  
}




double getEdgeLastFilledBin(TH1* h){
  for(int i=3;i<=h->GetNbinsX();++i){
    if(h->GetBinContent(i)==0 && h->GetBinContent(i-1)==0) return h->GetXaxis()->GetBinUpEdge(i-2);
  }
  return h->GetXaxis()->GetXmax();
}



double* Divide(const TArrayD* array , double val){
  TArrayD* temp = new TArrayD();
  array->Copy(*temp);
  for(int i = 0 ; i < temp->GetSize() ; i++)
    temp->SetAt(array->At(i) / val , i);
  
  return temp->GetArray();  
}
