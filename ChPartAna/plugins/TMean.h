#ifndef TMEAN_H
#define TMEAN_H

#include "TObject.h"

//#include "RootHeaders.h"

class TMean : public TObject{

  public:
  TMean();
  ~TMean();
  void Add(Double_t);
  void Add(Double_t, Double_t);
  void Substract(Double_t);
  void Substract(Double_t, Double_t);
  Double_t GetMean();
  Double_t GetRMS();
  void write();
  
  private:
  void ComputeMean();
  void ComputeRMS();
  Double_t _Mean;
  Double_t _RMS;
  Double_t _TotalNumber;
  Double_t _TotalSum;
  Double_t _TotalSumSquare;
  
  ClassDef(TMean,1);
};

#endif
