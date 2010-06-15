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
  void clear();
  ~TMoments();
  
  void Add(Double_t, Double_t);
  void Add(TH1F*);
  void Add(TGraph*);

  void ComputeMoments();
  void ComputeErrorsFromResampling(TH1F*);
  
  void AddDataForResampling(TH1F*);
  void FinishCurrentResampling();
  
  void print();
  bool isFilled(){return kisFilled;};
  
  Int_t nmoments;
  TMean* mean;
  
  vector<TMean>* cmoments_tmp;
  vector<TMean>* fmoments_tmp;
  
  vector<Double_t>* cmoments;
  vector<Double_t>* fmoments;
  vector<Double_t>* kmoments;
  vector<Double_t>* hmoments;
  
  vector<Double_t>* cstaterr;
  vector<Double_t>* fstaterr;
  vector<Double_t>* kstaterr;
  vector<Double_t>* hstaterr;
    
  vector<Double_t>* csystmerr;
  vector<Double_t>* fsystmerr;
  vector<Double_t>* ksystmerr;
  vector<Double_t>* hsystmerr;
  
  vector<Double_t>* csystperr;
  vector<Double_t>* fsystperr;
  vector<Double_t>* ksystperr;
  vector<Double_t>* hsystperr;
  
  TMean* mean_pull;
  TMean* RMS_pull;
  vector<TMean>* cstaterr_pull;
  vector<TMean>* fstaterr_pull;
  vector<TMean>* kstaterr_pull;
  vector<TMean>* hstaterr_pull;
    
  private:
  
  bool kisFilled;

  ClassDef(TMoments,1);
};

#endif
