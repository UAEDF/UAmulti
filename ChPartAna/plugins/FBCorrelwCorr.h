#ifndef FBCORRELWCORR_H
#define FBCORRELWCORR_H

#include "FBCorrel.h"
#include "TResponseMtx.h"
#include "TFile.h"
#include <vector>

class FBCorrelwCorr : public TObject{

  public:
  FBCorrelwCorr();
  FBCorrelwCorr(TString s);

  void Init();
  ~FBCorrelwCorr(){};
  
  
  void Fill(Double_t, Double_t, Double_t=1);
  void FillTrReweight(Double_t, Double_t, Double_t=1);
  void GetTRMFromFile(TString , TString , TString);
  void GetTrEffFromFile(TString , TString , TString);
  void write();
  
  TString name;
  static Bool_t debug;
  static Bool_t xCorrOn;
  static Bool_t xyCorrOn;
  static Bool_t trReweightOn;
  
  TResponseMtx xtrm;
  TResponseMtx ytrm;
  //TResponseMtx4D& trm4d;
  
  TH2F trEff;
  TH2F trFakes;  
  
  FBCorrel fbc_raw;
  FBCorrel fbc_xCorr;
  FBCorrel fbc_xyCorr;
  FBCorrel fbc_trReweight;
  //FBCorrel fbc_4DCorr;
  
  private:

  ClassDef(FBCorrelwCorr,1);

};

#endif
