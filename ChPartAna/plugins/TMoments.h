#ifndef TMOMENTS_H
#define TMOMENTS_H

#include "TObject.h"
#include "TRandom.h"
#include "TMean.h"
#include "TH1F.h"
#include "TGraph.h"
#include "TMath.h"
#include <iostream>

class TMoments : public TObject{

  public:
  TMoments();
  TMoments(Int_t);
  TMoments(TH1F*);
  
  void Init();
  ~TMoments();
  
  void Add(Double_t, Double_t);
  void Add(TH1F*);
  void Add(TGraph*);

  void ComputeMoments();
  void ComputeErrorsFromResampling(TH1F*);
  
  void Print();
  
  Int_t nmoments;
  TMean* mean;
  
  vector<TMean>* cmoments_tmp;
  vector<TMean>* fmoments_tmp;
  
  vector<Double_t>* cmoments;
  vector<Double_t>* fmoments;
  vector<Double_t>* kmoments;
  vector<Double_t>* hmoments;
  
  vector<Double_t>* cerrors;
  vector<Double_t>* ferrors;
  vector<Double_t>* kerrors;
  vector<Double_t>* herrors;
    
  private:

  ClassDef(TMoments,1);
};

#endif
