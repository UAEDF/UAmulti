#ifndef _MAKECORRECTIONS_
#define _MAKECORRECTIONS_


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

#include "../plugins/FBCorrel.h"
#include "../macro/FBacc.C"

#endif

using namespace std;
using namespace TMath;

bool debug = false;
bool doMC  = true;

TString st(string , int);
TGraph getbVSdEta(vector<FBCorrel>& , vector<FBacc>& , int , int);

void FBmakeCorrections(){
  
  //Get the MC file
  TString mcfile = "../macro/FBtest4_type31.root";
  TFile*  fmc    = TFile::Open(mcfile,"READ");
  cout<<"MC input file : "<<mcfile<<endl;
    
  //Get the data file
  TString datafile = "../macro/FBtest4_type0.root";
  TFile*  fdata    = TFile::Open(datafile,"READ");
  cout<<"Data input file : "<<datafile<<endl;
  
  //Checking if both files exist
  if(fmc == 0){
    cout<<"!! ERROR : the MC input file doesn't exist ..."<<endl;
  }
  if(fdata == 0){
    cout<<"!! ERROR : the Data input file doesn't exist ..."<<endl;
  }
  if(fmc == 0 || fdata == 0)
    return;

  //Getting the acceptance map
  getAccMap();
  //HERE would be the place to reduce the accMap
  //to run on just some of the bins
  //accMap.erase(accMap.begin()+22 , accMap.end());
  cout << "The acceptance map has " << accMap.size() << " entries. " << endl;
  
  //Getting the acceptance group map
  getAccGroups();
  while(accGroups.at(accGroups.size()-1).first>accMap.size()-1) accGroups.pop_back();
  cout << "The acceptance group map has " << accGroups.size() << " entries. " << endl;
  //printAcc();
  
  
  
  
  

  //Getting the TTProfile into vector<FBCorrel>
  //FIXME : should directly be a FBCorrel in file
  vector<FBCorrel> vmc_reco;
  vector<FBCorrel> vmc_gen;
  vector<FBCorrel> vdata;
  
  
  /*TTProfile* ttptmp = 0;
  for(int acc = 0 ; acc < (signed) accMap.size() ; ++acc){
    if(debug) cout << "Getting FBCorrel for acc " << acc << endl;
  
    //MC RECO
    ostringstream str("");
    //str << "FBCorrel_NSD_evtSel_reco_cut" << acc << "/fbcorrel_class_NSD_evtSel_reco_cut" << acc;
    str << "TTProfile_NSD_evtSel_reco_cut" << acc << "/ttprofile_class_NSD_evtSel_reco_cut" << acc;
    //ttptmp = dynamic_cast<FBCorrel*>(fmc->Get(str.str().c_str()));
    fmc->GetObject(str.str().c_str(),ttptmp);
    if(ttptmp==0){
      cout << "!! ERROR : the following FBCorrel doesn't exist ..." << endl << "  ==> " << str.str() << endl << "Exiting NOW !!"<<endl;
      return;
    }
    //fbctmp = new FBCorrel(*ttptmp);
    //vmc_reco.push_back(*fbctmp);
    vmc_reco.push_back(FBCorrel(*ttptmp));
    
    //MC GEN
    str.str("");ttptmp = 0;
    str << "TTProfile_NSD_noEvtSel_gen_cut" << acc << "/ttprofile_class_NSD_noEvtSel_gen_cut" << acc;
    fmc->GetObject(str.str().c_str(),ttptmp);
    if(ttptmp==0){
      cout << "!! ERROR : the following FBCorrel doesn't exist ..." << endl << "  ==> " << str.str() << endl << "Exiting NOW !!"<<endl;
      return;
    }
    vmc_gen.push_back(FBCorrel(*ttptmp));
    
    //DATA
    str.str("");ttptmp = 0;
    str << "TTProfile_NSD_evtSel_reco_cut" << acc << "/ttprofile_class_NSD_evtSel_reco_cut" << acc;
    fdata->GetObject(str.str().c_str(),ttptmp);
    if(ttptmp==0){
      cout << "!! ERROR : the following FBCorrel doesn't exist ..." << endl << "  ==> " << str.str() << endl << "Exiting NOW !!"<<endl;
      return;
    }
    vdata.push_back(FBCorrel(*ttptmp));
  }*/
  
  
  //-----------------------------------
  //Getting the FBCorrel Class Directly
  //if Work, comment previous !!
  FBCorrel* fbctmp = 0;
  for(int acc = 0 ; acc < (signed) accMap.size() ; ++acc){
    if(debug) cout << "Getting FBCorrel for acc " << acc << endl;
  
    //MC RECO
    ostringstream str("");
    str << "FBCorrel_NSD_evtSel_reco_cut" << acc << "/fbcorrel_class_NSD_evtSel_reco_cut" << acc;
    fmc->GetObject(str.str().c_str(),fbctmp);
    if(fbctmp==0){
      cout << "!! ERROR : the following FBCorrel doesn't exist ..." << endl << "  ==> " << str.str() << endl << "Exiting NOW !!"<<endl;
      return;
    }
    vmc_reco.push_back(*fbctmp);
    
    //MC GEN
    str.str("");fbctmp = 0;
    str << "FBCorrel_NSD_noEvtSel_gen_cut" << acc << "/fbcorrel_class_NSD_noEvtSel_gen_cut" << acc;
    fmc->GetObject(str.str().c_str(),fbctmp);
    if(fbctmp==0){
      cout << "!! ERROR : the following FBCorrel doesn't exist ..." << endl << "  ==> " << str.str() << endl << "Exiting NOW !!"<<endl;
      return;
    }
    vmc_gen.push_back(*fbctmp);
    
    //DATA
    str.str("");fbctmp = 0;
    str << "FBCorrel_NSD_evtSel_reco_cut" << acc << "/fbcorrel_class_NSD_evtSel_reco_cut" << acc;
    fdata->GetObject(str.str().c_str(),fbctmp);
    if(fbctmp==0){
      cout << "!! ERROR : the following FBCorrel doesn't exist ..." << endl << "  ==> " << str.str() << endl << "Exiting NOW !!"<<endl;
      return;
    }
    vdata.push_back(*fbctmp);
  }
  
  
  
  
  //Output File :
  TString outputfile = "FBcorrel.root" ;
  cout<<"Output file : "<<outputfile<<endl;
  TFile* out = new TFile(outputfile , "RECREATE");
  out->cd();
  
  //Do basic correction :
  //make eff from MC, including trigger + tracking eff.
  bool doBasicCorrection = true;
  vector<TH1F> eff_bc(accMap.size(),TH1F());
  if(doBasicCorrection){  
    for(int acc = 0 ; acc < (signed) accMap.size() ; ++acc){
      eff_bc.at(acc) = *( (TH1F*) vmc_reco.at(acc).ttp.profile.Clone(st("eff_bc",acc)));
      eff_bc.at(acc).Divide(&(vmc_gen.at(acc).ttp.profile));
  
      vdata.at(acc).ttp.profile.Divide(&(eff_bc.at(acc)));  
      vmc_reco.at(acc).ttp.profile.Divide(&(eff_bc.at(acc)));     
    }  
  }
  
  
  
  
  //Doing fit :
  for(int acc = 0 ; acc < (signed) accMap.size() ; ++acc){
    vdata.at(acc).Fit(1,debug);
    vmc_gen.at(acc).Fit(1,debug);
    vmc_reco.at(acc).Fit(1,debug);
  }
  
  //Computing b using corr func :
  for(int acc = 0 ; acc < (signed) accMap.size() ; ++acc){
    vdata.at(acc).ComputeB();
    vmc_gen.at(acc).ComputeB();
    vmc_reco.at(acc).ComputeB();
  }
  
  
  
  //ostringstream str("");
  //str << "bVSdEta_" << 
  //p.SetName(str.str().c_str());
  
  
  // Doing the bVSdEta plots -----------
  vector<TGraph> bVSdEta_data;
  vector<TGraph> bVSdEta_mc_gen;
  vector<TGraph> bVSdEta_mc_reco;
  for(int accG = 0 ; accG < (signed) accGroups.size() ; ++accG){
  
    bVSdEta_data.push_back( getbVSdEta( vdata , accMap , accGroups.at(accG).first , accGroups.at(accG).second) );
    ostringstream str("");
    str << "bVSdEta_data_" << accGroups.at(accG).first << "to" << accGroups.at(accG).second;
    bVSdEta_data.at(accG).SetName(str.str().c_str());  
    
    if(doMC){
      bVSdEta_mc_gen.push_back( getbVSdEta( vmc_gen , accMap , accGroups.at(accG).first , accGroups.at(accG).second) );
      ostringstream str("");
      str << "bVSdEta_mc_gen_" << accGroups.at(accG).first << "to" << accGroups.at(accG).second;
      bVSdEta_mc_gen.at(accG).SetName(str.str().c_str());  
    
      bVSdEta_mc_reco.push_back( getbVSdEta( vmc_reco , accMap , accGroups.at(accG).first , accGroups.at(accG).second) );
      str.str("");
      str << "bVSdEta_mc_reco_" << accGroups.at(accG).first << "to" << accGroups.at(accG).second;
      bVSdEta_mc_reco.at(accG).SetName(str.str().c_str());
    }
  }
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  //Writing to file :
  cout << "Writing to File ...";
  for(int accG = 0 ; accG < (signed) accGroups.size() ; ++accG){
    ostringstream str("");
    str << accGroups.at(accG).first << "to" << accGroups.at(accG).second;
    gDirectory->mkdir(str.str().c_str());
    gDirectory->cd(str.str().c_str());
    
    for(int acc = accGroups.at(accG).first ; acc <= accGroups.at(accG).second ; ++acc){
      vdata.at(acc).ttp.profile.Write();
      vdata.at(acc).fit.Write();
      
      if(doMC){
      
        vmc_gen.at(acc).ttp.profile.Write(TString(vmc_gen.at(acc).ttp.profile.GetName())+"_MC");
        vmc_gen.at(acc).fit.Write(TString(vmc_gen.at(acc).fit.GetName())+"_MC");
      
        vmc_reco.at(acc).ttp.profile.Write(TString(vmc_reco.at(acc).ttp.profile.GetName())+"_MC");
        vmc_reco.at(acc).fit.Write(TString(vmc_reco.at(acc).fit.GetName())+"_MC");
      }
      
      if(doBasicCorrection) eff_bc.at(acc).Write();
    }
    
    bVSdEta_data.at(accG).Write();
    if(doMC){
      bVSdEta_mc_gen.at(accG).Write();
      bVSdEta_mc_reco.at(accG).Write();
    }
    
    gDirectory->cd("../");
  
  }
  
  cout << "... Done" << endl;
  
  
  out->Close();
  
  //gPad->WaitPrimitive();
    
  
  fmc->Close();
  fdata->Close();

}

TGraph getbVSdEta(vector<FBCorrel>& fbc , vector<FBacc>& accMap , int accmin , int accmax){

  TGraph* p = new TGraph();
  for(int acc = accmin , i = 0 ; acc <= accmax ; ++acc , ++i){
    double dEta = accMap.at(acc).etaP - accMap.at(acc).etaM;
    p->SetPoint(i , dEta , fbc.at(acc).b );  
  }
  return *p;
}

TGraph getbVSdEtaFromFit(vector<FBCorrel>& fbc , vector<FBacc>& accMap , int accmin , int accmax){

  TGraph* p = new TGraph();
  for(int acc = accmin , i = 0 ; acc <= accmax ; ++acc , ++i){
    double dEta = accMap.at(acc).etaP - accMap.at(acc).etaM;
    p->SetPoint(i , dEta , fbc.at(acc).b_fit );  
  }
  return *p;
}













TString st(string input , int cut){
  stringstream out("");
  out<<cut;
  return input+"_cut"+out.str();
}





