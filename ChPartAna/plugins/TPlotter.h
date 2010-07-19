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
  
  Double_t binKillXMax;
  Int_t    divideBy;
  
  private:
  
  ClassDef(TPlotter,1);
};

#endif
