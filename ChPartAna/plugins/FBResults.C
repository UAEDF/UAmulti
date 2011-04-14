#include "FBResults.h"

ClassImp(FBResults)

Bool_t         FBResults::debug            = 0;
vector<FBacc>* FBResults::accMap           = 0;

FBResults::FBResults(){
  if(FBResults::debug) cout << "+ [DEBUG] FBResults : Starting FBResults() {" << name << "}" << endl;
  if(FBResults::debug) cout << "- [DEBUG] FBResults : Finished FBResults() {" << name << "}" << endl;
}

FBResults::FBResults(TString s):name(s){
  if(FBResults::debug) cout << "+ [DEBUG] FBResults : Starting FBResults(TString) {" << name << "}" << endl;
  if(FBResults::debug) cout << "- [DEBUG] FBResults : Finished FBResults(TString) {" << name << "}" << endl;
}

FBResults::FBResults(TString s , vector<FBCorrel*>& vfbc , Double_t accmin , Double_t accmax):name(s){
  this->Init();
  this->MakeAllPlots(vfbc , accmin , accmax);
}

void FBResults::Init(){
  if(accMap==0) cout << "[FBResults] Warning !! Please provide a FBResults::accMap pointer before running this code ..." << endl;
  bVSdEta_fromReg = TGraph();
  bVSdEta_fromFit = TGraph();
}



void FBResults::MakeAllPlots( vector<FBCorrel*>& vfbc , Double_t accmin , Double_t accmax ){
  if(FBResults::debug) cout << "+ [DEBUG] FBResults : Starting MakeAllPlots() {" << name << "}" << endl;

  for(int acc = accmin ; acc <= accmax ; ++acc){
    vfbc.at(acc)->Fit(1 , int(FBResults::debug) );
  }

  ostringstream str("");
  str << accmin << "to" << accmax;
  dirname = TString(str.str());
  
  //b VS dEta From Reg
  str.str("");
  str << "bVSdEta_fromReg_" << name << "_" << dirname;
  bVSdEta_fromReg.SetName(str.str().c_str());
  this->GetbVSdEta(vfbc , accmin , accmax , bVSdEta_fromReg );
  
  //b VS dEta From Fit
  str.str("");
  str << "bVSdEta_fromFit_" << name << "_" << dirname;
  bVSdEta_fromFit.SetName(str.str().c_str());
  this->GetbVSdEtaFromFit(vfbc , accmin , accmax , bVSdEta_fromFit );
  
  //sigC VS eta
  str.str("");
  str << "sigCVSeta_" << name << "_" << dirname;
  sigCVSeta.SetName(str.str().c_str());
  this->GetsigCVSeta(vfbc , accmin , accmax , sigCVSeta );
}



void FBResults::GetbVSdEta(vector<FBCorrel*>& vfbc , int accmin , int accmax , TGraph& p ){

  for(int acc = accmin , i = 0 ; acc <= accmax ; ++acc , ++i){
    double dEta = accMap->at(acc).etaP - accMap->at(acc).etaM;
    p.SetPoint(i , dEta , vfbc.at(acc)->b );  
  }
}

void FBResults::GetbVSdEtaFromFit(vector<FBCorrel*>& vfbc , int accmin , int accmax , TGraph& p ){

  for(int acc = accmin , i = 0 ; acc <= accmax ; ++acc , ++i){
    double dEta = accMap->at(acc).etaP - accMap->at(acc).etaM;
    p.SetPoint(i , dEta , vfbc.at(acc)->b_fit );  
  }
}


void FBResults::GetsigCVSeta(vector<FBCorrel*>& vfbc , int accmin , int accmax , TGraph& p ){

  for(int acc = accmin , i = 0 ; acc <= accmax ; ++acc , ++i){
    if(! (accMap->at(acc).etaP == - accMap->at(acc).etaM && accMap->at(acc).widthP == accMap->at(acc).widthM) ){
      cout << "[FBResults] Warning !! the accMap eta or width are not identical on both sides. TGraph content will be void" << endl;
      return;
    }
    
    double eta = accMap->at(acc).etaP + accMap->at(acc).widthP / 2.;
    p.SetPoint(i , eta , vfbc.at(acc)->mC.GetRMS());  
  }
}


void FBResults::write(){
  if(! (gDirectory->Cd(dirname) || gDirectory->GetName()==dirname) ){
    gDirectory->mkdir(dirname);
    gDirectory->cd(dirname);
  }
  
  gDirectory->mkdir("FBResults_"+name+"_"+dirname);
  gDirectory->cd("FBResults_"+name+"_"+dirname);
  
  bVSdEta_fromReg.Write();
  bVSdEta_fromFit.Write();
  sigCVSeta.Write();
  //cout << gDirectory->GetName() << "  " << "FBResults_class_"+name+"_"+dirname << endl;
  this->Write("FBResults_class_"+name+"_"+dirname);

  gDirectory->cd("../..");
}
