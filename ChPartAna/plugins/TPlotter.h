#ifndef TPLOTTER_H
#define TPLOTTER_H

#include "TPlotterBase.h"

//#include "RootHeaders.h"

class TPlotter : public TPlotterBase{

  public:
  TPlotter(){this->Init();};
  ~TPlotter(){};
  void Init();
  
  Int_t    nSmooth;
  Bool_t   doSmoothing;
  Double_t smoothingXMin;
  Double_t smoothingXMax;
  Bool_t   smoothDrawing;
  
  Bool_t   isMC;
  Int_t    style;
  Int_t    color;
  Int_t    size;
  TString  file;
  TString  dir;
  TString  histo;
  Int_t    hType;
  Double_t offset;
  Double_t factor;
  
  Double_t binKillXMax;
  Int_t    divideBy;
  TString  ratioLeg;
  Bool_t   ratioKeepOffset;
  Bool_t   ratioKeepFactor;
  
  TString  leg;
  
  bool     doFit;
  TF1      tf1;
  TString  tf1FromFile;
  TString  tf1Name;
  TString  tf1Formula;
  Bool_t   tf1Show;
  Int_t    tf1Color;
  Int_t    tf1Style;
  
  private:
  
  ClassDef(TPlotter,1);
};

#endif
