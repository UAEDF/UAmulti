#ifndef __MatrixPlots_H__
#define __MatrixPlots_H__

#include <vector>
#include "TObject.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TDirectory.h"
#include "./MyPart.h"

class MatrixPlots : public TObject {

  public :
    MatrixPlots();
    MatrixPlots(TString);
    virtual ~MatrixPlots();

  // private:

    TString matrixcoll;
    
    TH2F* nch_matrix;
    TH2F* eta_matrix;
    TH2F* pt_matrix;
    
    TH1F* nch_gen;
    TH1F* eta_gen;
    TH1F* pt_gen;
    
    TH1F* nch_reco;
    TH1F* eta_reco;
    TH1F* pt_reco;
    
    void init();
    void fillGen(MyPart& , double = 1.);
    void fillReco(MyPart& , double = 1.);
    void nextEvent();
    void write();
    
  private:
    Double_t nch_gen_inEvt;
    Double_t nch_reco_inEvt;
    
    ClassDef (MatrixPlots,1)
};



#endif


