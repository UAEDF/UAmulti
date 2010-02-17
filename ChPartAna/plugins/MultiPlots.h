#ifndef MultiPlots_H
#define MultiPlots_H

#include <vector>
#include <iostream>
#include "TObject.h"
#include "TH1F.h"
#include "TString.h"
#include "TObject.h"
#include "TDirectory.h"

#include "TMean.h"
#include "MyPart.h"

using namespace std;

class MultiPlots : public TObject{

  public:
  MultiPlots();
  MultiPlots(TString);
  ~MultiPlots();
  
  void fill(MyPart&,double = 1.);
  void nextEvent(double = 1.);
  void makeKNO();
  void write();
  
  //getters/setters
  vector<double> getMoments();
  void setNbOfMoments(Int_t);
  void SetEnergy(Double_t e){energy=e;};
  
  TMean* nch_mean;
  TH1F*  nch;
  TH1F*  kno;
  TH1F*  rapidity;
  TH1F*  eta;
  TH1F*  pt;
  TH1F*  pt2;
  
  Int_t nb_moments;
  vector<TMean>* moments;
  
  Double_t energy;
  Double_t nbEvts;
  Int_t nch_inEvt;
  
  private:
  TString multiname;
  
  Double_t nch_width;
  Double_t rapidity_width;
  Double_t eta_width;
  Double_t pt_width;
  Double_t pt2_width;
  
  void writeSummary();
  void init();
  
  ClassDef(MultiPlots,1);

};

#endif
