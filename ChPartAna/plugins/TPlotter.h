#ifndef TPLOTTER_H
#define TPLOTTER_H

#include "TPlotterBase.h"

//#include "RootHeaders.h"

class TPlotter : public TPlotterBase{

  public:
  TPlotter();
  ~TPlotter();
  
  Int_t    nSmooth;
  Bool_t   doSmoothing;
  Double_t smoothingXMin;
  Double_t smoothingXMax;
  Bool_t   smoothDrawing;
  
  Double_t offset;
  Double_t factor;
  
  Double_t binKillXMax;
  Int_t    divideBy;
  TString  ratioLeg;
  
  TString  leg;
  
  bool     doFit;
  TF1      tf1;
  TString  tf1Formula;
  Bool_t   tf1Show;
  Int_t    tf1Color;
  Int_t    tf1Style;
  
  private:
  
  ClassDef(TPlotter,1);
};

#endif
