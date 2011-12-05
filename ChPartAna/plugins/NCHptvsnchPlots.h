#ifndef __NCHptvsnchPlots_H__
#define __NCHptvsnchPlots_H__

#include "TFile.h"
#include "TObject.h"
#include "TString.h"
#include "TH1F.h"
#include "TProfile.h"
#include "TH2F.h"
#include "TH1D.h"
#include "TDirectory.h"
#include "TString.h"

#include <sstream>
#include <iostream>
#include <map>
#include <cassert>
using namespace std;

typedef map <TString, bool> MapType;

class NCHptvsnchPlots : public TObject {

  public :
    NCHptvsnchPlots(TString, TString , TString , TString , TString );
    virtual ~NCHptvsnchPlots();

  // private:

    TString ptvsnchcoll; 
        
    TH1F* nch_unfolded;
    TH2F* matrix;
    
    TProfile* mptVSnch_noSel_nchCorr;
    TProfile* mptVSnch_noSel;
    TProfile* mptVSnch_L1;
    TProfile* mptVSnch_HF;
    TProfile* mptVSnch_vtxQual;
    TProfile* mptVSnch_vtx;
    TProfile* mptVSnch_L1_HF;
    TProfile* mptVSnch_L1_HF_vtxQual;
    TProfile* mptVSnch_fullSel;
    TProfile* mptVSnch_fullSel_nchCorr;
    
    void init();
    
    TString getCut(){ return cut; }
    TString getCentr(){ return centr; }
    TString getHf(){ return hf; }
    TString getMc(){ return mc; }
    TString getUnfdir(){ return unfdir; }
    
    void fill(MapType*, double, double);
    void write();
    
  private:
    
    void multiplyByWidth(TH1F* );
    void multiplyByWidth(TH2F* );
    bool goodEvent(MapType*);
    
    
    const TString cut;
    const TString centr;
    const TString hf;
    
    const TString unfdir;   
    const TString mc;
    
    ClassDef (NCHptvsnchPlots,1)
};

#endif


