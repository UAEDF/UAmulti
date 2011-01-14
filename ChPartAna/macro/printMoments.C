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
#include <fstream>

using namespace std;

#include "../macro/fileManager.C"

#include "../plugins/TMoments.h"
#include "../plugins/TMean.h"

//-----------------------------------------------------------
//          !!!    WARNING   !!!
//
// the syst for the moments are the total errors !!!
// was fixed only for the mean
//-----------------------------------------------------------



void printMoments(int cut = 5){
  TString plotdir = "../plots/current_b1_2/";
  
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
  
  cout<<fixed <<setprecision(1);
  cout<<"$<n>$"<<" & $"
  	<<mom0->mean->GetMean()<<" \\pm "<<mom0->mean->GetMeanError()<<"^{+ "<<mom0->mean->GetMeanSystP()<<"}_{- "<<mom0->mean->GetMeanSystM()<<"}$ & $"
  	<<mom1->mean->GetMean()<<" \\pm "<<mom1->mean->GetMeanError()<<"^{+ "<<mom1->mean->GetMeanSystP()<<"}_{- "<<mom1->mean->GetMeanSystM()<<"}$ & $"
  	<<mom2->mean->GetMean()<<" \\pm "<<mom2->mean->GetMeanError()<<"^{+ "<<mom2->mean->GetMeanSystP()<<"}_{- "<<mom2->mean->GetMeanSystM()<<"}$ \\\\ "<<endl;
	
  cout<<"D_{2}"<<" & $"
  	<<mom0->mean->GetRMS()<<" \\pm "<<mom0->mean->GetRMSError()<<"^{+ "<<mom0->mean->_RMSSystP<<"}_{- "<<mom0->mean->_RMSSystM<<"}$ & $"
  	<<mom1->mean->GetRMS()<<" \\pm "<<mom1->mean->GetRMSError()<<"^{+ "<<mom1->mean->_RMSSystP<<"}_{- "<<mom1->mean->_RMSSystM<<"}$ & $"
  	<<mom2->mean->GetRMS()<<" \\pm "<<mom2->mean->GetRMSError()<<"^{+ "<<mom2->mean->_RMSSystP<<"}_{- "<<mom2->mean->_RMSSystM<<"}$ \\\\ "<<endl;
	
  cout<<fixed <<setprecision(3);
  
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
  
  
  cout<<mom0->mean->GetMean()<<"  "<<sqrt(pow(mom0->mean->GetMeanError(),2)+pow((mom0->mean->GetMeanSystP()+mom0->mean->GetMeanSystM())/2,2))<<endl;
  cout	<<mom1->mean->GetMean()<<"  "<<sqrt(pow(mom1->mean->GetMeanError(),2)+pow((mom1->mean->GetMeanSystP()+mom1->mean->GetMeanSystM())/2,2))<<endl;
  cout	<<mom2->mean->GetMean()<<"  "<<sqrt(pow(mom2->mean->GetMeanError(),2)+pow((mom2->mean->GetMeanSystP()+mom2->mean->GetMeanSystM())/2,2))<<endl;


  //------------------------------------------
  //------------------------------------------
  //--                                      --
  //--        Dumping in text file          --
  //--                                      --
  //------------------------------------------
  //------------------------------------------
  
  #include "acceptanceMap.C"


  vector<TMoments*> vmom;
  vmom.push_back(mom0);
  vmom.push_back(mom1);
  vmom.push_back(mom2);
  vector<double> E;
  E.push_back(0.9);
  E.push_back(2.36);
  E.push_back(7);
  
  ostringstream txtstr("");
  txtstr << "../txt/Cmoments_eta" << accMap.at(cut).at(1) << ".txt";
  ofstream txtout(txtstr.str().c_str());
  cout << " ++    dumping points in : " << txtstr.str() << endl;
  
  txtout << "Fig. 6 (";
  if(cut==5) txtout << "a)" << endl << endl;
  if(cut==9) txtout << "b)" << endl << endl;
  
  for(int imom = 0 ; imom < vmom.size() ; ++imom){
    txtout.unsetf(ios_base::floatfield);
    txtout << "  Cmoments at sqrt(s) = " << setprecision(3) << setw(4) << E.at(imom) << "TeV for | eta | < " << accMap.at(cut).at(1) << endl << endl;
    txtout << left 
           << setw(5) << "q"
           << setw(10) << "C_q"
           << setw(10) << "stat-"
           << setw(10) << "stat+"
           << setw(10) << "syst-"
           << setw(10) << "syst+"
	   << endl << endl;
	   
    for(int q = 2 ; q <= 5 ; ++q){
    
      //precision
      int precision = 1;
      for(int ipow = 1 ; ipow <=10 ; ++ipow){
        precision = ipow+1;
        if(vmom.at(imom)->cmoments->at(q) > pow(10.,-ipow)){
          break;
        }
      }

      txtout << left 
             << setw(5) << q
	     << setprecision(precision) << fixed
             << setw(10) << vmom.at(imom)->cmoments->at(q)
             << setw(10) << vmom.at(imom)->cstaterr->at(q)
             << setw(10) << vmom.at(imom)->cstaterr->at(q)
             << setw(10) << sqrt( pow(vmom.at(imom)->csystmerr->at(q),2) - pow(vmom.at(imom)->cstaterr->at(q),2) )
             << setw(10) << sqrt( pow(vmom.at(imom)->csystperr->at(q),2) - pow(vmom.at(imom)->cstaterr->at(q),2) )
	     << endl;
    }
    txtout << endl << endl << endl;
  }

  txtout.close();

}



