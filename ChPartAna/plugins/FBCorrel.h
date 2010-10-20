#ifndef FBCORREL_H
#define FBCORREL_H

#include "TTProfile.h"
#include "TF1.h"

class FBCorrel : public TObject{

  public:
  FBCorrel();
  FBCorrel(TTProfile& t){ttp = t;this->Init();};
  FBCorrel(TString s);

  void Init();
  ~FBCorrel(){};
  
  static Bool_t debug;
  
  void Fill(Double_t, Double_t, Double_t=1);
  void Fit(Bool_t = 1 , Int_t = 0);
  void ComputeB();
  void write();

  TTProfile ttp;
  TF1 fit;
  TF1 func;
  Double_t b_fit;
  Double_t b;
  
  TMean mn1n2;
  TMean mn1;
  TMean mn2;
  
  TMean mC;
  TH1F  hC;
  
  private:

  ClassDef(FBCorrel,1);

};

#endif
