#ifndef TMEAN_H
#define TMEAN_H

#include "TObject.h"

//#include "RootHeaders.h"

class TMean : public TObject{

  public:
  TMean();
  ~TMean();
  void Add(Double_t, Double_t = 1.);
  void Substract(Double_t, Double_t = 1.);
  Double_t GetMean();
  Double_t GetMeanError(){return _MeanError;}
  Double_t GetRMS();
  Double_t GetN(){return _TotalNumber;};
  Double_t GetSum(){return _TotalSum;};
  void SetMeanError(Double_t err){_MeanError = err;}
  void Reset();
  void Scale(Double_t);
  void write();
  
  private:
  void ComputeMean();
  void ComputeRMS();
  Double_t _Mean;
  Double_t _MeanError;
  Double_t _RMS;
  Double_t _TotalNumber;
  Double_t _TotalSum;
  Double_t _TotalSumSquare;
  
  ClassDef(TMean,1);
};

#endif
