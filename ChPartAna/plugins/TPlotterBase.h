#ifndef TPLOTTERBASE_H
#define TPLOTTERBASE_H

#include "TObject.h"
#include "TF1.h"
#include "TString.h"

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
  
  static Double_t g_offset;
  static Double_t g_factor;
 
 
  static Double_t g_binKillXMax;
  
  static TString  g_ratioType;//"none","standard","fit","only"
  static Int_t    g_divideBy;
  static TString  g_ratioLeg;
  static Int_t    g_ratioLegPos;
  static Double_t g_ratioLegX;
  static Double_t g_ratioLegY;
  static Bool_t   g_useStandardFormat;
  
  static TString  g_leg;
  static Int_t    g_legPos;
  static Double_t g_legX;
  static Double_t g_legY;
  
  static bool     g_doFit;
  static TF1      g_tf1;
  static TString  g_tf1Formula;
  static Bool_t   g_tf1Show;
  static Int_t    g_tf1Color;
  static Int_t    g_tf1Style;
  
  private:
  
  ClassDef(TPlotterBase,2);
};

#endif
