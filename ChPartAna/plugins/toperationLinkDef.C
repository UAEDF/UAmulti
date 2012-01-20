#include "./TOperation.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TProfile.h"

#ifdef __MAKECINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

//typedef TH1F* pTH1F;
#pragma link C++ class TOperation<TH1F>+;
#pragma link C++ class TOperation<TH2F>+;
#pragma link C++ class TOperation<TProfile>+;
#pragma link C++ class vector<TOperation<TH1F> >+;
#pragma link C++ class vector<TOperation<TH2F> >+;
#pragma link C++ class vector<TOperation<TProfile> >+;

#endif
