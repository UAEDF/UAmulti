#ifndef __GenPartPlots_H__
#define __GenPartPlots_H__

#include <vector>
using namespace std;
#include "TObject.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TMath.h"
#include "TDirectory.h"
#include "./MyGenPart.h"
#include "BasePlots.h"


class GenPartPlots : public BasePlots {

  public :
    GenPartPlots();
    GenPartPlots(TString);
    virtual ~GenPartPlots();

    TString genpartcoll;
    
    TH1F* charge; 
    TH1F* nch;
    TH1F* pt;
    TH1F* eta;
    TH1F* phi;
    
    void init();
    void fill(vector<MyGenPart>& , double = 1.);
    void divide(GenPartPlots& , const TString);
    template <class T> void makeEff( T*, T*, const TString);
    void write();
    
  private:
    
    ClassDef (GenPartPlots,1)
};

#endif

