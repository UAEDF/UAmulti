#ifndef __BasePlots_H__
#define __BasePlots_H__

#include <vector>
#include "TObject.h"
#include "TString.h"

class BasePlots : public TObject {

  public :
    BasePlots();
    BasePlots(TString);
    virtual ~BasePlots();

  // private:

    TString plotsname;
    
     static Int_t     nch_nbin;
     static Double_t  nch_xmin;
     static Double_t  nch_xmax;
     static Double_t* nch_array;
    
     static Int_t     pt_nbin;
     static Double_t  pt_xmin;
     static Double_t  pt_xmax;
     static Double_t* pt_array;
    
     static Int_t     eta_nbin;
     static Double_t  eta_xmin;
     static Double_t  eta_xmax;
     static Double_t* eta_array;
    
    void setBinning_nch(Int_t nbin , Double_t xmin , Double_t xmax){nch_nbin=nbin;nch_xmin=xmin;nch_xmax=xmax;makeBins(nbin , xmin , xmax , nch_array);};
    void setBinning_pt( Int_t nbin , Double_t xmin , Double_t xmax){pt_nbin =nbin;pt_xmin=xmin;pt_xmax=xmax;this->makeBins(nbin , xmin , xmax , pt_array);};
    void setBinning_eta(Int_t nbin , Double_t xmin , Double_t xmax){eta_nbin=nbin;eta_xmin=xmin;eta_xmax=xmax;this->makeBins(nbin , xmin , xmax , eta_array);};
    
    void setBinning_nch(Double_t* array){ nch_array = array ;};
    void setBinning_pt(Double_t* array) { pt_array  = array ;};
    void setBinning_eta(Double_t* array){ eta_array = array ;};
    
    void setBinning(vector< vector<double> >&);
    
    void makeBins(const Int_t , Double_t , Double_t , Double_t*);
    
  private:

    
    ClassDef (BasePlots,1)
};



#endif


