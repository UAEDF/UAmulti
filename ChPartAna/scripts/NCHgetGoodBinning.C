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

//_____________________________________________________________________________
TString st(string input , int cut){
  stringstream nb("");  
  nb<<cut;
  return input+"cut"+nb.str();
}

// root -l NCHgetGoodBinning.C+"(\"../macro/outputs_full/v21/output_data_ferncTr_E_7_3422minEvtInBin.root\")" -q
// root -l NCHgetGoodBinning.C+"(\"../macro/outputs_full/v21/output_data_ferncTr_E_7_342220.root\")" -q
void NCHgetGoodBinning(TString filename){
  double minEvtInBin = 40;  
  #include "../macro/acceptanceMap.C"
  
  cout<<"File : "<<filename<<endl;
  TFile* file = new TFile(filename,"READ");
  

  //Getting the good dir
  for(int acc = 0 ; acc < (signed)accMap.size() ; ++acc){

    TString histname = "nch_mpreco_full_HF0_nocut_RECO";

    cout<<"  if(icut=="<<acc<<"){"<<endl;
    
    //cout<<histname.str()<<endl;
    TH1F* hist = NULL;
    hist = (TH1F*) file->FindObjectAny(histname+st("_",acc));
    if (hist==0) {
        cout << " histogram does not exist for: acc " << acc << endl;
        return;
    }    
    
    int lastwidth = 1;
    bool first = false;
    for(int i=1 ; i<=hist->GetNbinsX() ; ++i){
      int binwidth = int(hist->GetBinWidth(i));
      
      //making first bins with width 1
      if(hist->GetBinContent(i)<minEvtInBin && hist->GetBinContent(i)!=0 && !first && binwidth==1){
        cout<<"    tmp = makeBins("<<i-1<<",-0.5,"<<double(i)-1.5<<");"<<endl;
        first=true;
      }
      
      //making first bins with width=1,then others with width!=1 && stat>minEvtInBin
      if(hist->GetBinContent(i)>=minEvtInBin && binwidth!=1 && !first){
        cout<<"    tmp = makeBins("<<i-1<<",-0.5,"<<double(i)-1.5<<");"<<endl;
	first = true;
      }
      
      if(hist->GetBinContent(i)>=minEvtInBin && first){
        while(binwidth<lastwidth){
	  i++;
	  binwidth+=int(hist->GetBinWidth(i));
	}
        cout<<"    addBins(1,"<<binwidth<<",tmp);"<<endl;
      }
      //making bins with stat<minEvtInBin
      if(hist->GetBinContent(i)<minEvtInBin){
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
	  if(n>minEvtInBin) break;
        }
        
	if(!useLastBin && binwidth<=10){
	  while(binwidth<lastwidth){
	    j++;
	    binwidth+=int(hist->GetBinWidth(j));
	  }
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



