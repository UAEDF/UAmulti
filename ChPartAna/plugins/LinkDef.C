#include <vector>
#include <utility>

#include "./TMean.h"
#include "./TMoments.h"

#include "./BasePlots.h"
#include "./EvtSelPlots.h"
#include "./GenMultiPlots.h"
#include "./MatrixPlots.h"
#include "./MultiPlots.h"
#include "./TrackPlots.h"
#include "./TResponseMtx.h"
#include "./VertexPlots.h"

#include "./GenPartPlots.h"
#include "./NCHEvtSelPlots.h"
#include "./NCHHFPlots.h"
#include "./NCHCentralPlots.h"
#include "./NCHDiffPlots.h"

#include "./Syst.h"
//#include "./TOperation.h"
//#include "TH1F.h"
//#include "TH2F.h"
#include "./EfficiencyPlots.h"

#include "./TPlotterBase.h"
#include "./TPlotter.h"

#include "./TTProfile.h"
#include "./FBCorrel.h"
#include "./FBCorrelwCorr.h"
#include "./FBResults.h"

#ifdef __MAKECINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ class pair<string,bool>+;
//#pragma link C++ class pair<string,unsigned int>


#pragma link C++ class TMean+;
#pragma link C++ class vector<TMean>+;
#pragma link C++ class TMoments+;
#pragma link C++ class vector<TMoments>+;

#pragma link C++ class BasePlots+;
#pragma link C++ class MultiPlots+;
#pragma link C++ class GenMultiPlots+;
#pragma link C++ class MatrixPlots+;
#pragma link C++ class TResponseMtx+;
#pragma link C++ class TrackPlots+;
#pragma link C++ class VertexPlots+;
#pragma link C++ class EvtSelPlots+;

#pragma link C++ class GenPartPlots+;
#pragma link C++ class NCHEvtSelPlots+;
#pragma link C++ class NCHHFPlots+;
#pragma link C++ class NCHCentralPlots+;
#pragma link C++ class NCHDiffPlots+;

#pragma link C++ class Syst+;

//typedef TH1F* pTH1F;
//#pragma link C++ class TOperation<TH1F>+;
//#pragma link C++ class TOperation<TH2F>+;
//#pragma link C++ class vector<TOperation<TH1F> >+;
//#pragma link C++ class vector<TOperation<TH2F> >+;

#pragma link C++ class EfficiencyPlots+;

#pragma link C++ class TPlotterBase+;
#pragma link C++ class TPlotter+;

#pragma link C++ class TTProfile+;
#pragma link C++ class vector<TTProfile>+;
#pragma link C++ class FBCorrel+;
#pragma link C++ class vector<FBCorrel>+;
#pragma link C++ class FBCorrelwCorr+;
#pragma link C++ class vector<FBCorrelwCorr>+;
#pragma link C++ class FBResults+;
#pragma link C++ class vector<FBResults>+;

#endif
