#include "TPlotterBase.h"
#include <math.h>

ClassImp(TPlotterBase)

Int_t    TPlotterBase::g_nSmooth       = 50;
Bool_t   TPlotterBase::g_doSmoothing   = false;
Bool_t   TPlotterBase::g_smoothDrawing = false;
Double_t TPlotterBase::g_smoothingXMin = false;
Double_t TPlotterBase::g_smoothingXMax = false;

Double_t TPlotterBase::g_binKillXMax = -999.;

Double_t TPlotterBase::g_offset = 0.;
Double_t TPlotterBase::g_factor = 1.;

TString  TPlotterBase::g_ratioType         = "none";
Int_t    TPlotterBase::g_divideBy          = -1;
TString  TPlotterBase::g_ratioLeg          = "none";
Int_t    TPlotterBase::g_ratioLegPos       = 1;
Double_t TPlotterBase::g_ratioLegX         = -1;
Double_t TPlotterBase::g_ratioLegY         = -1;
Bool_t   TPlotterBase::g_useStandardFormat = true;

TString  TPlotterBase::g_leg          = "none";
Int_t    TPlotterBase::g_legPos       = -1;
Double_t TPlotterBase::g_legX         = -1;
Double_t TPlotterBase::g_legY         = -1;
  
bool     TPlotterBase::g_doFit      = 0;
TF1      TPlotterBase::g_tf1        = TF1();
TString  TPlotterBase::g_tf1Formula = "none";
Bool_t   TPlotterBase::g_tf1Show    = true;
Int_t    TPlotterBase::g_tf1Color   = kRed;
Int_t    TPlotterBase::g_tf1Style   = 1;
  
TPlotterBase::TPlotterBase(){
}

TPlotterBase::~TPlotterBase(){
}


