#include "TPlotter.h"
#include <math.h>

ClassImp(TPlotter)


TPlotter::TPlotter(){
  nSmooth       = g_nSmooth;
  doSmoothing   = g_doSmoothing;
  smoothDrawing = g_smoothDrawing;
  smoothingXMin = g_smoothingXMin;
  smoothingXMax = g_smoothingXMax;
  binKillXMax   = g_binKillXMax;
  divideBy      = g_divideBy;
}

TPlotter::~TPlotter(){
}


