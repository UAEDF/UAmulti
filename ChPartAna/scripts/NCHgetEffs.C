 #ifndef NCHGETEFFS_
#define _NCHGETEFFS_


//#include "vector>
#include "TMath.h"
#include "TH1F.h"
#include "TF1.h"
#include "TH1D.h"
#include "TH2F.h"
#include "TFile.h"
#include "TChain.h"
#include "TChainElement.h"
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
using namespace std;

#endif
bool debug_=true;

#include "../macro/files_funcs.C"




//--------------------------------------------------------------

TH1F* getEff(TFile& f , TString str1 , TString str2 , TString out){
  TH1F* up   = getHist<TH1F>(f , str1);
  TH1F* down = getHist<TH1F>(f , str2);
  
  TH1F* eff = (TH1F*) up->Clone(out);
  eff->Divide(up , down , 1, 1, "B");
  
  eff->SetMinimum(0);
  for (int i=1;i<=eff->GetNbinsX();i++) { 
    if (i>60 && eff->GetBinContent(i)==0 )
        eff->SetBinContent(i,1);
  }

  delete up; delete down;
  return eff;
}


//--------------------------------------------------------------

void NCHgetEffsRun(TString dir="../macro/outputs_full/v21/", TString filenameMC31  = "output_MC31_ferncTr_E_7_5000000_allEff.root", TString lastpartmc2 ="_HF0_nocut_RECO_cut0") {

  TString lastpartmc1 ="_partfull";  
  
  TString eff_output=filenameMC31;
  eff_output.ReplaceAll("output","effs");
  TString lastpartmc=lastpartmc1+lastpartmc2;

  TFile* mc = getFile(dir+filenameMC31);  
  cout <<"Output in : " << dir+eff_output << endl; 
  TFile* eff_out= new TFile(dir+eff_output,"UPDATE");  
  eff_out->cd();
  
  //gen level
  TH1F* eff_evtSel  = getEff(*mc , "nch_partfull"+lastpartmc2 , "nch_partnoSel"+lastpartmc2 , "eff_evtSel"+lastpartmc);
  TH1F* eff_centrSel= getEff(*mc , "nch_noSel"+lastpartmc2 , "nch_CentrGen"+lastpartmc2 , "eff_centrSel"+lastpartmc);  
  
  TH1F* eff = (TH1F*) eff_evtSel->Clone("eff_total"+lastpartmc);
  eff->Multiply(eff_centrSel,eff_evtSel,1,1);
  
  //all effs
  TH1F* eff_L1Sel = getEff(*mc , "nch_L1Sel"+lastpartmc2 , "nch_noSel"+lastpartmc2 , "eff_nch_L1Eff"+lastpartmc2);
  TH1F* eff_vtxSel = getEff(*mc , "nch_vtxSel"+lastpartmc2 , "nch_noSel"+lastpartmc2 , "eff_nch_vtxEff"+lastpartmc2);
  TH1F* eff_vtxqualSel = getEff(*mc , "nch_vtxqualSel"+lastpartmc2 , "nch_noSel"+lastpartmc2 , "eff_nch_vtxqualEff"+lastpartmc2);
  TH1F* eff_hfSel      = getEff(*mc , "nch_hfSel"+lastpartmc2 , "nch_noSel"+lastpartmc2 , "eff_nch_hfEff"+lastpartmc2);
  
  //reco level
  TH1F* eff_evtSel_reco = getEff(*mc , "nch_full"+lastpartmc2 , "nch_noSel"+lastpartmc2 , "eff_evtSel_RECO_"+lastpartmc2);

  
  ////////////  
  gDirectory->mkdir(lastpartmc);
  gDirectory->cd(lastpartmc); 
  
  eff->Write("eff_total"+lastpartmc);
  eff_evtSel->Write("evtSel"+lastpartmc);
  eff_centrSel->Write("centrSel"+lastpartmc);
  eff_L1Sel->Write("L1Sel"+lastpartmc);
  eff_vtxSel->Write("vtxSel"+lastpartmc);
  eff_vtxqualSel->Write("vtxqualSel"+lastpartmc);
  eff_hfSel->Write("hfSel"+lastpartmc);
  eff_evtSel_reco->Write("evtSel_RECO_"+lastpartmc);
  
  gDirectory->cd("../"); 
  
  mc->Close();
  eff_out->Close();
  delete eff_out;
  delete mc;
}



//--------------------------------------------------------------

void NCHgetEffsZeroBias(TString dir="../macro/outputs_full/v21/", TString filenameMC31  = "output_zerobias_ferncTr_E_7_482270.root", TString lastpartmc2 ="_HF0_nocut_RECO_cut0") {

  TString lastpartmc1 ="_zerobias";    
  TString eff_output=filenameMC31;
  eff_output.ReplaceAll("output","effs");
  
  TString lastpartmc=lastpartmc1+lastpartmc2;

  TFile* mc = getFile(dir+filenameMC31);
  cout <<"Output in : " << dir+eff_output << endl;  
  TFile* eff_out= new TFile(dir+eff_output,"UPDATE");  
  eff_out->cd();
  
  TH1F* eff_evtSel = getEff(*mc , "nch_full"+lastpartmc2 , "nch_noSel"+lastpartmc2 , "eff_nch"+lastpartmc2);

  
////////////  
  //gDirectory->mkdir(lastpartmc);
  //gDirectory->cd(lastpartmc); 

  eff_evtSel->Write("evtSel"+lastpartmc);
  
  //gDirectory->cd("../"); 
  
  mc->Close();
  delete eff_out;
  delete mc;
  
}



//--------------------------------------------------------------

typedef void (*pFunc)(TString , TString , TString);
void run_all(pFunc func , TString dir , TString file){
  TString centrCuts[]={"nocut","MBUEWG","ALICE","ATLAS1","ATLAS2","ATLAS6"};
  TString hf[]={"HF0","HF1"};
  Int_t nacc = 6;
  
  for(int iAcc = 0 ; iAcc < nacc ; ++iAcc){
    ostringstream iAccStr("") ; iAccStr << iAcc;
    for(int iHF = 0 ; iHF < 2 ; ++iHF){
      for(int iCentrCuts = 0 ; iCentrCuts < 6 ; ++iCentrCuts){
        cout << "---------------------------------------------------------------" << endl;
	cout << "hf : " << hf[iHF] << "   centrCut : " << centrCuts[iCentrCuts] << "   acc : " << iAccStr.str() << endl;
        func(dir , file , "_" + hf[iHF] + "_" + centrCuts[iCentrCuts] + "_RECO_cut" + iAccStr.str());
      }
    }
  }
}



//--------------------------------------------------------------

void NCHgetEffs(Int_t type = 0 , TString subdir = "v21" ) {
  TString dir="../macro_sten/";
  TString filename = "";
  
  pFunc func = 0;
  
  if(type==0){
    func = NCHgetEffsRun;
    filename = "output_MC31_ferncTr_E_7_5000000_noweight_allEffs.root";
  }
  else if(type==1){
    func = NCHgetEffsZeroBias;
    filename = "output_zerobias_ferncTr_E_7_482270_allEffs.root";
  }
  
  TString eff_output=filename;
  eff_output.ReplaceAll("output","effs");
  TFile* out= new TFile(dir+eff_output,"RECREATE");
  out->Close();
  delete out;
  cout << "Removed and recreated empty "<< dir+eff_output << endl;
   
  run_all(func , dir , filename);
  
}
