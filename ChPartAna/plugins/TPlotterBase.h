#ifndef TPLOTTERBASE_H
#define TPLOTTERBASE_H

#include "TObject.h"

//#include "RootHeaders.h"

class TPlotterBase : public TObject{

  public:
  TPlotterBase();
  ~TPlotterBase();
  
  static Int_t    g_nSmooth;
  static Bool_t   g_doSmoothing;
  static Double_t g_smoothingXMin;
  static Double_t g_smoothingXMax;
  static Bool_t   g_smoothDrawing;
  
  
  static Double_t g_binKillXMax;
  
  static Int_t g_divideBy;
  
  private:
  
  ClassDef(TPlotterBase,2);
};

#endif
