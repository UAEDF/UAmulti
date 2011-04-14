#include "FBCorrelwCorr.h"

Bool_t FBCorrelwCorr::debug        = 0;
Bool_t FBCorrelwCorr::xCorrOn      = 0;
Bool_t FBCorrelwCorr::xyCorrOn     = 0;
Bool_t FBCorrelwCorr::trReweightOn = 0;

FBCorrelwCorr::FBCorrelwCorr(){
  if(FBCorrelwCorr::debug) cout << "+ [DEBUG] FBCorrelwCorr : Starting FBCorrelwCorr() {" << name << "}" << endl;
  if(FBCorrelwCorr::debug) cout << "- [DEBUG] FBCorrelwCorr : Finished FBCorrelwCorr() {" << name << "}" << endl;
}

FBCorrelwCorr::FBCorrelwCorr(TString s):name(s){
  if(FBCorrelwCorr::debug) cout << "+ [DEBUG] FBCorrelwCorr : Starting FBCorrelwCorr(TString) {" << name << "}" << endl;
  this->Init();
  if(FBCorrelwCorr::debug) cout << "- [DEBUG] FBCorrelwCorr : Finished FBCorrelwCorr(TString) {" << name << "}" << endl;
}


void FBCorrelwCorr::Init(){
  if(FBCorrelwCorr::debug) cout << "+ [DEBUG] FBCorrelwCorr : Starting Init() {" << name << "}" << endl;
  fbc_raw    = FBCorrel("raw_"   +name);
  
  Bool_t th1OnBkp      = TTProfile::switchTH1On;
  Bool_t th2OnBkp      = TTProfile::switchTH2On;
  Bool_t tmomentsOnBkp = TTProfile::switchTMomentsOn;
  
  TTProfile::switchTH1On      = 0;
  TTProfile::switchTH2On      = 0;
  TTProfile::switchTMomentsOn = 0;
  
  if(xCorrOn)
    fbc_xCorr      = FBCorrel("xCorr_"     +name);
  if(xyCorrOn)
    fbc_xyCorr     = FBCorrel("xyCorr_"    +name);
  if(trReweightOn)
    fbc_trReweight = FBCorrel("trReweight_"+name);
  
  //putting back wanted values
  TTProfile::switchTH1On      = th1OnBkp;
  TTProfile::switchTH2On      = th2OnBkp;
  TTProfile::switchTMomentsOn = tmomentsOnBkp;
  
  if(FBCorrelwCorr::debug) cout << "- [DEBUG] FBCorrelwCorr : Finished Init() {" << name << "}" << endl;
}

void FBCorrelwCorr::Fill(Double_t xval , Double_t yval , Double_t weight){
    
    fbc_raw.Fill(    xval , yval , weight );
  if(xCorrOn)
    fbc_xCorr.Fill(  xval , yval , xtrm );
  if(xyCorrOn)
    fbc_xyCorr.Fill( xval , yval , xtrm , ytrm);
}

void FBCorrelwCorr::FillTrReweight(Double_t xval , Double_t yval , Double_t weight){
  if(trReweightOn)
    fbc_trReweight.Fill( xval , yval , weight );
}

void FBCorrelwCorr::GetTRMFromFile(TString fstr , TString xtrmstr , TString ytrmstr){
  TFile* f = TFile::Open(fstr , "READ");
  if(f==0){
    cout << "[FBCorrelwCorr] WARNING !! Can't open file for xtrm & ytrm. Switching xCorrOn & xyCorrOn to 0." << endl;
    xCorrOn = xyCorrOn = 0;
    return;
  }
  
  xtrm = *( (TResponseMtx*) f->Get("TResponseMtx_" + xtrmstr + "/tresponsemtx_class_" + xtrmstr));
  if(xtrm.nEvts==0){
    cout << "[FBCorrelwCorr] WARNING !! Couldn't fetch xtrm " << xtrmstr << " from file "<< fstr << " . Switching xCorrOn & xyCorrOn to 0." << endl;
    xCorrOn = xyCorrOn = 0;
    return;
  }
  
  ytrm = *( (TResponseMtx*) f->Get("TResponseMtx_" + ytrmstr + "/tresponsemtx_class_" + ytrmstr));
  if(ytrm.nEvts==0){
    cout << "[FBCorrelwCorr] WARNING !! Couldn't fetch ytrm " << ytrmstr << " from file "<< fstr << " . Switching xyCorrOn to 0." << endl;
    xyCorrOn = 0;
    return;
  }
}


void FBCorrelwCorr::GetTrEffFromFile(TString fstr , TString treffstr , TString trfakesstr){
  TFile* f = TFile::Open(fstr , "READ");
  if(f==0){
    cout << "[FBCorrelwCorr] WARNING !! Can't open file for trEff & trFakes. Switching trReweightOn to 0." << endl;
    trReweightOn = 0;
    return;
  }
  
  trEff = *( (TH2F*) f->Get(treffstr));
  if(trEff.GetEntries()==0){
    cout << "[FBCorrelwCorr] WARNING !! Couldn't fetch trEff " << treffstr << " from file "<< fstr << " . Switching trReweightOn to 0." << endl;
    trReweightOn = 0;
    return;
  }
  
  trFakes = *( (TH2F*) f->Get(trfakesstr));
  if(trFakes.GetEntries()==0){
    cout << "[FBCorrelwCorr] WARNING !! Couldn't fetch trFakes " << trfakesstr << " from file "<< fstr << " . Switching trReweightOn to 0." << endl;
    trReweightOn = 0;
    return;
  }
}


void FBCorrelwCorr::write(){
  gDirectory->mkdir("FBCorrelwCorr_"+name);
  gDirectory->cd("FBCorrelwCorr_"+name);
  
  fbc_raw.write();
  if(xCorrOn){
    fbc_xCorr.write();
    xtrm.write();
  }
  if(xyCorrOn){
    fbc_xyCorr.write();
    if(!xCorrOn) xtrm.write();
    ytrm.write();
  }
  if(trReweightOn){
    fbc_trReweight.write();
    trEff.Write();
    trFakes.Write();
  }
  
  this->Write("fbcorrelwcorr_class_"+name);

  gDirectory->cd("../");
}
