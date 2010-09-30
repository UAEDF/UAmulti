#include "TPlotter.h"
#include <math.h>

ClassImp(TPlotter)


TPlotter::TPlotter(){
  nSmooth       = g_nSmooth;
  doSmoothing   = g_doSmoothing;
  smoothDrawing = g_smoothDrawing;
  smoothingXMin = g_smoothingXMin;
  smoothingXMax = g_smoothingXMax;
  offset        = g_offset;
  factor        = g_factor;
  binKillXMax   = g_binKillXMax;
  divideBy      = g_divideBy;
  ratioLeg      = g_ratioLeg;
  leg           = g_leg;
  doFit         = g_doFit;
  tf1           = g_tf1;
  tf1Formula    = g_tf1Formula;
  tf1Show       = g_tf1Show;
  tf1Color      = g_tf1Color;
  tf1Style      = g_tf1Style;
}

TPlotter::~TPlotter(){
}


