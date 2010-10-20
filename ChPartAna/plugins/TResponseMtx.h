#ifndef __TRESPONSEMTX_H__
#define __TRESPONSEMTX_H__

#include <vector>
#include <iostream>
#include "TObject.h"
#include "TH2F.h"
#include "TDirectory.h"
using namespace std;


class TResponseMtx : public TObject {

  public :
    TResponseMtx(){};
    TResponseMtx(TString s):name(s){this->Init();};
    TResponseMtx(TString s , vector<Double_t> v):name(s),binning(v){this->Init();};
    ~TResponseMtx(){};
     void makeBins(Int_t, Double_t, Double_t);

    TString  name;
    Double_t nEvts;
    Bool_t   isNormalised;
    Bool_t   isNormalisedByColumn;
    Bool_t   isNormalisedByLine;
    
    vector<Double_t> binning;
    TH2F mtx;
    
    void Init();
    void FillGen(Double_t);
    void FillReco(Double_t);
    void NextEvent(Double_t = 1);
    void Fill(Double_t , Double_t , Double_t = 1.);
    void NormalizeByLine();
    void NormalizeByColumn();
    void Normalize(Bool_t = true);
    void write(Bool_t = true);
    
  private:
  
    Double_t gen_inEvt;
    Double_t reco_inEvt;
        
    ClassDef (TResponseMtx,1)
};



#endif


