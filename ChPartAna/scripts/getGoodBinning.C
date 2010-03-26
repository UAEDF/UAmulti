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

#include "TFrame.h"

#include <iostream>
#include <fstream>
#include <sstream>



void getGoodBinning(TString filename){
  
  #include "../macro/acceptanceMap.C"
  
  cout<<"File : "<<filename<<endl;
  TFile* file = new TFile(filename,"READ");
  
  
  //Getting the good dir
  for(int acc = 0 ; acc < (signed)accMap.size() ; ++acc){
    ostringstream dir("");
    dir << "ptGen" << accMap.at(acc).at(0) << "_etaGen" << accMap.at(acc).at(1)
        << "_ptReco" << accMap.at(acc).at(2) << "_etaReco" << accMap.at(acc).at(3);
    if(accMap.at(acc).at(4)==-1)
      dir << "_chargemin";
    else if(accMap.at(acc).at(4)==1)
      dir << "_chargeplus";
      
    stringstream histname("");
    histname<<dir.str()<<"/MultiPlots_evtSel_INC_reco_cut"<<acc<<"/nch_evtSel_INC_reco_cut"<<acc;
  
    
    cout<<"  if(icut=="<<acc<<"){"<<endl;
    
    //cout<<histname.str()<<endl;
    TH1F* hist = (TH1F*) file->Get(histname.str().c_str());
    
    
    
    bool first = false;
    for(int i=1 ; i<=hist->GetNbinsX() ; ++i){
      int binwidth=hist->GetBinWidth(i);
      if(hist->GetBinContent(i)<20 && hist->GetBinContent(i)!=0 && !first && binwidth==1)
        cout<<"    tmp = makeBins("<<i-1<<",-0.5,"<<double(i)-1.5<<");"<<endl;
	
      if(hist->GetBinContent(i)>=20 && binwidth!=1 && !first){
        cout<<"    tmp = makeBins("<<i-1<<",-0.5,"<<double(i)-1.5<<");"<<endl;
	  first = true;
      }
      if(hist->GetBinContent(i)>=20 && binwidth!=1)
        cout<<"    addBins(1,"<<binwidth<<",tmp);"<<endl;
      
      if(hist->GetBinContent(i)<20 && hist->GetBinContent(i)!=0){
        first = true;
        double n = hist->GetBinContent(i);
	int j = 0;
        for(j=i+1;j<=hist->GetNbinsX() ; ++j){
          n+=hist->GetBinContent(j);
	  binwidth += hist->GetBinWidth(j);
	  if(n>20) break;
        }
        cout<<"    addBins(1,"<<j-i+1<<",tmp);"<<endl;
	i=j;
      }
    }
    cout<<"  }"<<endl;
  }//end of loop over acceptance map
}



