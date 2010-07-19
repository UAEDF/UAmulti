#include "TPlotterBase.h"
#include <math.h>

ClassImp(TPlotterBase)

Int_t    TPlotterBase::g_nSmooth = 50;
Bool_t   TPlotterBase::g_doSmoothing = false;
Bool_t   TPlotterBase::g_smoothDrawing = false;
Double_t TPlotterBase::g_smoothingXMin = false;
Double_t TPlotterBase::g_smoothingXMax = false;

Double_t TPlotterBase::g_binKillXMax = -999.;
Int_t    TPlotterBase::g_divideBy = -1;
  
  
TPlotterBase::TPlotterBase(){
}

TPlotterBase::~TPlotterBase(){
}


