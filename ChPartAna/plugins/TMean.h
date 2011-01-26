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
  Double_t GetMeanSquare();
  Double_t GetMeanSystP();
  Double_t GetMeanSystM();
  Double_t GetMeanTotalErrorP(){return _MeanSystP;}
  Double_t GetMeanTotalErrorM(){return _MeanSystM;}
  Double_t GetRMS();
  Double_t GetRMSError(){return _RMSError;}
  Double_t GetRMSSystP();
  Double_t GetRMSSystM();
  Double_t GetRMSTotalErrorP(){return _RMSSystP;}
  Double_t GetRMSTotalErrorM(){return _RMSSystM;}
  Double_t GetN(){return _TotalNumber;};
  Double_t GetSum(){return _TotalSum;};
  void SetMeanError(Double_t err){_MeanError = err;}
  void SetRMSError(Double_t err){_RMSError = err;}
  void Reset();
  void Scale(Double_t);
  void write();
  
  
  Double_t _MeanSystP;
  Double_t _MeanSystM;
  Double_t _RMSSystP;
  Double_t _RMSSystM;
  
  private:
  void ComputeMean();
  void ComputeRMS();
  Double_t _Mean;
  Double_t _MeanError;
  Double_t _RMS;
  Double_t _RMSError;
  Double_t _TotalNumber;
  Double_t _TotalSum;
  Double_t _TotalSumSquare;
  
  ClassDef(TMean,2);
};

#endif
