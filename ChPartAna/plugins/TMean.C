#include "TMean.h"
#include <math.h>

ClassImp(TMean)

TMean::TMean(){
  _Mean = _TotalNumber = _TotalSum = _TotalSumSquare = 0.;
  _RMS = -1.;
  _MeanError = _RMSError  = 0.;
  _MeanSystP = _MeanSystM = 0.;
  _RMSSystP  = _RMSSystM  = 0.;
}

TMean::~TMean(){
}


void TMean::Add(Double_t value, Double_t weight){
_TotalSum+=value*weight;
_TotalSumSquare+=value*value*weight;
_TotalNumber+=weight;
}

void TMean::Substract(Double_t value, Double_t weight){
_TotalSum-=value*weight;
_TotalSumSquare-=value*value*weight;
_TotalNumber-=weight;
}

void TMean::ComputeMean(){
if(_TotalNumber!=0) _Mean=_TotalSum/_TotalNumber;
}

void TMean::ComputeRMS(){
if(_TotalNumber!=0) _RMS=_TotalSumSquare/_TotalNumber - _Mean*_Mean;
if(_RMS>=0) _RMS = sqrt(_RMS);
else _RMS = -1;
}

Double_t TMean::GetMean(){
  ComputeMean();
  return _Mean;
}

Double_t TMean::GetMeanSquare(){
  if(_TotalNumber!=0) return _TotalSumSquare/_TotalNumber;
  else return 0;
}

Double_t TMean::GetMeanSystP(){
  return sqrt( pow(_MeanSystP,2) - pow(_MeanError,2) );
}

Double_t TMean::GetMeanSystM(){
  return sqrt( pow(_MeanSystM,2) - pow(_MeanError,2) );
}

Double_t TMean::GetRMS(){
  ComputeMean();
  ComputeRMS();
  return _RMS;
}

Double_t TMean::GetRMSSystP(){
  return sqrt( pow(_RMSSystP,2) - pow(_RMSError,2) );
}

Double_t TMean::GetRMSSystM(){
  return sqrt( pow(_RMSSystM,2) - pow(_RMSError,2) );
}

void TMean::Reset(){
  _Mean = 0. ;
  _RMS = -1. ;
  _TotalNumber = 0. ;
  _TotalSum = 0. ;
  _TotalSumSquare = 0. ;
  _MeanError = _RMSError  = 0;
  _MeanSystP = _MeanSystM = 0;
  _RMSSystP  = _RMSSystM  = 0;
}

void TMean::Scale(Double_t factor){
  //_TotalNumber /= factor ;
  _TotalSum /= factor ;
  _TotalSumSquare /= factor ;
}

void TMean::write(){
  ComputeMean();
  ComputeRMS();
  this->Write();
}

