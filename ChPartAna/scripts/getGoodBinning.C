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
    
    
    int lastwidth = 1;
    bool first = false;
    for(int i=1 ; i<=hist->GetNbinsX() ; ++i){
      int binwidth = int(hist->GetBinWidth(i));
      
      //making first bins with width 1
      if(hist->GetBinContent(i)<20 && hist->GetBinContent(i)!=0 && !first && binwidth==1){
        cout<<"    tmp = makeBins("<<i-1<<",-0.5,"<<double(i)-1.5<<");"<<endl;
        first=true;
      }
      
      //making first bins with width=1,then others with width!=1 && stat>20
      if(hist->GetBinContent(i)>=20 && binwidth!=1 && !first){
        cout<<"    tmp = makeBins("<<i-1<<",-0.5,"<<double(i)-1.5<<");"<<endl;
	first = true;
      }
      
      if(hist->GetBinContent(i)>=20 && first)
        cout<<"    addBins(1,"<<binwidth<<",tmp);"<<endl;
      
      //making bins with stat<20
      if(hist->GetBinContent(i)<20){
        first = true;
        double n = hist->GetBinContent(i);
	int j = 0;
	bool useLastBin = false;
	int nnull = 0;
        for(j=i+1;j<=hist->GetNbinsX() ; ++j){
	  if(hist->GetBinContent(j)==0) ++nnull;
          n+=hist->GetBinContent(j);
	  binwidth += int(hist->GetBinWidth(j));
	  
	  if(nnull>5){useLastBin=true;break;}
	  if(n>20) break;
        }
	
	if(!useLastBin && binwidth<=10){
          cout<<"    addBins(1,"<<binwidth<<",tmp);"<<endl;
	  i=j;
	}
	
	if(!useLastBin && binwidth>10){
          cout<<"    addBins(1,10,tmp);"<<endl;
	  binwidth=10;
	  i+=10;
	}
	if(useLastBin){
	  if(lastwidth<=5)
	    binwidth=5;
	  else
	    binwidth=lastwidth;
	    
	  int nbin = int((300-i)/binwidth);
	  cout<<"    addBins("<<nbin+1<<","<<binwidth<<",tmp);"<<endl;
	  break;
	}
	
      }
      lastwidth = binwidth;
    }
    cout<<"  }"<<endl;
  }//end of loop over acceptance map
}



