#include "TPlotter.h"
#include <math.h>

ClassImp(TPlotter)

void TPlotter::Init(){
  nSmooth         = g_nSmooth;
  doSmoothing     = g_doSmoothing;
  smoothDrawing   = g_smoothDrawing;
  smoothingXMin   = g_smoothingXMin;
  smoothingXMax   = g_smoothingXMax;
  isMC            = g_isMC;
  style           = g_style;
  color           = g_color;
  size            = g_size;
  file            = g_file;
  dir             = g_dir;
  histo           = g_histo;
  hType           = g_hType;
  offset          = g_offset;
  factor          = g_factor;
  binKillXMax     = g_binKillXMax;
  divideBy        = g_divideBy;
  ratioLeg        = g_ratioLeg;
  ratioKeepOffset = g_ratioKeepOffset;
  ratioKeepFactor = g_ratioKeepFactor;
  leg             = g_leg;
  doFit           = g_doFit;
  tf1             = g_tf1;
  tf1FromFile     = g_tf1FromFile;
  tf1Name         = g_tf1Name;
  tf1Formula      = g_tf1Formula;
  tf1Show         = g_tf1Show;
  tf1Color        = g_tf1Color;
  tf1Style        = g_tf1Style;
}
