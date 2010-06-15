#include "TFile.h"
#include "TH1F.h"
#include "TProfile.h"
#include "TGraphAsymmErrors.h"
#include "TString.h"
#include "TPad.h"
#include "TSystem.h"
#include "TStyle.h"
#include "TFrame.h"
#include "TObject.h"


#include <iostream>
#include <iomanip>
#include <sstream>
using namespace std;

#include "../macro/fileManager.C"

#include "../plugins/TMoments.h"
#include "../plugins/TMean.h"


void printMoments(int cut = 5){
  TString plotdir = "../plots/systv10_binning1v3_2/";
  
  
  ostringstream outstr("");
  outstr << "hyp" << 1 << "_niter" << 0 << "_cut" << cut << "_DataType" << 0;
  TString tdata0  = fileManager(3,10,0.9,1,0,0,outstr.str(),plotdir);
  TString tdata1  = fileManager(3,10,2.36,1,0,0,outstr.str(),plotdir);
  TString tdata2  = fileManager(3,31,7.0,1,0,0,outstr.str(),plotdir);
  
  cout<<"Opening file :"<<tdata0<<endl;
  TFile*  fdata0  = TFile::Open(tdata0,"READ");
  TMoments* mom0  = (TMoments*) fdata0->Get("unfolding/moments/moments_syst");
  
  cout<<"Opening file :"<<tdata1<<endl;
  TFile*  fdata1  = TFile::Open(tdata1,"READ");
  TMoments* mom1  = (TMoments*) fdata1->Get("unfolding/moments/moments_syst");
  
  cout<<"Opening file :"<<tdata2<<endl;
  TFile*  fdata2  = TFile::Open(tdata2,"READ");
  TMoments* mom2  = (TMoments*) fdata2->Get("unfolding/moments/moments_syst");

  cout<<fixed <<setprecision(1);
  cout<<"----------------------------------------------------------------------"<<endl;
  cout<<"\\begin{table}[h!]"<<endl;
  cout<<"\\begin{center}"<<endl;
  cout<<"\\caption{$<n>$ and moments for $|\\eta|<"<<2.5-0.5*(cut - 5.*int(cut/5.))<<"$ .\\label{tab1}}"<<endl;
  cout<<"\\begin{tabular}{|l|c|c|c|}"<<endl;
  cout<<"Center of mass energy	& 0.9 \\TeV & 2.36 \\TeV & 7.0 \\TeV\\\\\\hline"<<endl;
  
  cout<<fixed <<setprecision(3);
  cout<<"$<n>$"<<" & $"
  	<<mom0->mean->GetMean()<<" \\pm "<<mom0->mean->GetMeanError()<<"^{+ "<<mom0->mean->_MeanSystP<<"}_{- "<<mom0->mean->_MeanSystM<<"}$ & $"
  	<<mom1->mean->GetMean()<<" \\pm "<<mom1->mean->GetMeanError()<<"^{+ "<<mom1->mean->_MeanSystP<<"}_{- "<<mom1->mean->_MeanSystM<<"}$ & $"
  	<<mom2->mean->GetMean()<<" \\pm "<<mom2->mean->GetMeanError()<<"^{+ "<<mom2->mean->_MeanSystP<<"}_{- "<<mom2->mean->_MeanSystM<<"}$ \\\\ "<<endl;
	
  cout<<"D_{2}"<<" & $"
  	<<mom0->mean->GetRMS()<<" \\pm "<<mom0->mean->GetRMSError()<<"^{+ "<<mom0->mean->_RMSSystP<<"}_{- "<<mom0->mean->_RMSSystM<<"}$ & $"
  	<<mom1->mean->GetRMS()<<" \\pm "<<mom1->mean->GetRMSError()<<"^{+ "<<mom1->mean->_RMSSystP<<"}_{- "<<mom1->mean->_RMSSystM<<"}$ & $"
  	<<mom2->mean->GetRMS()<<" \\pm "<<mom2->mean->GetRMSError()<<"^{+ "<<mom2->mean->_RMSSystP<<"}_{- "<<mom2->mean->_RMSSystM<<"}$ \\\\ "<<endl;
	
  
  for(int i = 2 ; i <=5 ; ++i){
  ostringstream mom("");
  mom<<"C_{"<<i<<"}";
  cout<<mom.str()<<" & $"
  	<<mom0->cmoments->at(i)<<" \\pm "<<mom0->cstaterr->at(i)<<"^{+ "<<mom0->csystperr->at(i)<<"}_{- "<<mom0->csystmerr->at(i)<<"}$ & $"
  	<<mom1->cmoments->at(i)<<" \\pm "<<mom1->cstaterr->at(i)<<"^{+ "<<mom1->csystperr->at(i)<<"}_{- "<<mom1->csystmerr->at(i)<<"}$ & $"
  	<<mom2->cmoments->at(i)<<" \\pm "<<mom2->cstaterr->at(i)<<"^{+ "<<mom2->csystperr->at(i)<<"}_{- "<<mom2->csystmerr->at(i)<<"}$ \\\\ "<<endl;
  }
  
  for(int i = 2 ; i <=5 ; ++i){
  ostringstream mom("");
  mom<<"F_{"<<i<<"}";
  cout<<mom.str()<<" & $"
  	<<mom0->fmoments->at(i)<<" \\pm "<<mom0->fstaterr->at(i)<<"^{+ "<<mom0->fsystperr->at(i)<<"}_{- "<<mom0->fsystmerr->at(i)<<"}$ & $"
  	<<mom1->fmoments->at(i)<<" \\pm "<<mom1->fstaterr->at(i)<<"^{+ "<<mom1->fsystperr->at(i)<<"}_{- "<<mom1->fsystmerr->at(i)<<"}$ & $"
  	<<mom2->fmoments->at(i)<<" \\pm "<<mom2->fstaterr->at(i)<<"^{+ "<<mom2->fsystperr->at(i)<<"}_{- "<<mom2->fsystmerr->at(i)<<"}$ \\\\ "<<endl;
  }
  
  for(int i = 2 ; i <=5 ; ++i){
  ostringstream mom("");
  mom<<"K_{"<<i<<"}";
  cout<<mom.str()<<" & $"
  	<<mom0->kmoments->at(i)<<" \\pm "<<mom0->kstaterr->at(i)<<"^{+ "<<mom0->ksystperr->at(i)<<"}_{- "<<mom0->ksystmerr->at(i)<<"}$ & $"
  	<<mom1->kmoments->at(i)<<" \\pm "<<mom1->kstaterr->at(i)<<"^{+ "<<mom1->ksystperr->at(i)<<"}_{- "<<mom1->ksystmerr->at(i)<<"}$ & $"
  	<<mom2->kmoments->at(i)<<" \\pm "<<mom2->kstaterr->at(i)<<"^{+ "<<mom2->ksystperr->at(i)<<"}_{- "<<mom2->ksystmerr->at(i)<<"}$ \\\\ "<<endl;
  }
  
  for(int i = 2 ; i <=5 ; ++i){
  ostringstream mom("");
  mom<<"H_{"<<i<<"}";
  cout<<mom.str()<<" & $"
  	<<mom0->hmoments->at(i)<<" \\pm "<<mom0->hstaterr->at(i)<<"^{+ "<<mom0->hsystperr->at(i)<<"}_{- "<<mom0->hsystmerr->at(i)<<"}$ & $"
  	<<mom1->hmoments->at(i)<<" \\pm "<<mom1->hstaterr->at(i)<<"^{+ "<<mom1->hsystperr->at(i)<<"}_{- "<<mom1->hsystmerr->at(i)<<"}$ & $"
  	<<mom2->hmoments->at(i)<<" \\pm "<<mom2->hstaterr->at(i)<<"^{+ "<<mom2->hsystperr->at(i)<<"}_{- "<<mom2->hsystmerr->at(i)<<"}$ \\\\ "<<endl;
  }
	
  cout<<"\\hline"<<endl;
  cout<<"\\end{tabular}"<<endl;
  cout<<"\\end{center}"<<endl;
  cout<<"\\end{table}"<<endl;
  
  
  cout<<mom0->mean->GetMean()<<"  "<<sqrt(pow(mom0->mean->GetMeanError(),2)+pow((mom0->mean->_MeanSystP+mom0->mean->_MeanSystM)/2,2))<<endl;
  cout	<<mom1->mean->GetMean()<<"  "<<sqrt(pow(mom1->mean->GetMeanError(),2)+pow((mom1->mean->_MeanSystP+mom1->mean->_MeanSystM)/2,2))<<endl;
  cout	<<mom2->mean->GetMean()<<"  "<<sqrt(pow(mom2->mean->GetMeanError(),2)+pow((mom2->mean->_MeanSystP+mom2->mean->_MeanSystM)/2,2))<<endl;
/*
\begin{table}[htbH]
\begin{center}
\caption{Event yields in each data sample after sequential trigger and
event selection cuts.\label{tab1}}
\begin{tabular}{|l|c|c|c|}
\hline
Center of mass energy	& 0.9 \TeV & 2.36 \TeV & 7.0 \TeV\\\hline
Selection        	& Nev 	& Nev & NeV\\\hline
Level 1 trigger  	& 264468 & 19214  & xxx\\
beam bg rejection  	& 254666 & 18627 & xxx\\
Forward calorimeters    & 146658 & 11991  & xxx\\
vertex presence         & 127892 & 11356  & xxx\\
\hline
\end{tabular}
\end{center}
\end{table}*/


}



