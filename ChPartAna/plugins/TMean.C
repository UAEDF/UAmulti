#include "TMean.h"
#include <math.h>
#include <iostream>

using namespace std;

ClassImp(TMean)

TMean::TMean(){
  _Mean = _TotalNumber = _TotalSum = _TotalSumSquare = 0.;
  _RMS = -1.;
  _MeanError = _RMSError  = 0.;
  _MeanSystP = _MeanSystM = 0.;
  _RMSSystP  = _RMSSystM  = 0.;
  nsamples = 0;
  vsamples = vector<TMean*>();
}

TMean::~TMean(){
  for(int i = 0 ; i < nsamples ; ++i)
    delete vsamples[i];
}


void TMean::Add(Double_t value, Double_t weight){
  _TotalSum+=value*weight;
  _TotalSumSquare+=value*value*weight;
  _TotalNumber+=weight;
  
  if(nsamples > 0){
    for(int i = 0 ; i < nsamples ; ++i){
      if(resampling_type.Contains("poiss"))
        vsamples[i]->Add(gRandom->PoissonD(value));
      else if(resampling_type.Contains("gauss"))
        vsamples[i]->Add(gRandom->Gaus(value,value));
    }
  }
  
}

void TMean::Substract(Double_t value, Double_t weight){
  _TotalSum-=value*weight;
  _TotalSumSquare-=value*value*weight;
  _TotalNumber-=weight;

  if(nsamples > 0){
    for(int i = 0 ; i < nsamples ; ++i){
      if(resampling_type.Contains("poiss"))
        vsamples[i]->Substract(gRandom->PoissonD(value));
      else if(resampling_type.Contains("gauss"))
        vsamples[i]->Substract(gRandom->Gaus(value,value));
    }
    cout << "[TMean::Substract] WARNING !! Can't use this to un-add something added previously while resampling is on ..." << endl;
  }
}

void TMean::EnableErrorsFromResampling(TString type , int n){
  nsamples = n;
  resampling_type = type;
  vsamples.assign(nsamples,NULL);
  for(int i = 0 ; i < nsamples ; ++i)
    vsamples[i] = new TMean();
}

void TMean::ComputeMean(){
  if(_TotalNumber!=0) _Mean=_TotalSum/_TotalNumber;
  
  if(nsamples > 0){
    TMean sample_mean;
    TMean sample_RMS;
    for(int i = 0 ; i < nsamples ; ++i){
      sample_mean.Add(vsamples[i]->GetMean());
      sample_RMS.Add(vsamples[i]->GetRMS());
    }
    TMean::SetMeanError( sample_mean.GetRMS() );
    TMean::SetRMSError( sample_RMS.GetRMS() );
  }
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

