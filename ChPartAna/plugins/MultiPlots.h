#ifndef MultiPlots_H
#define MultiPlots_H

#include <vector>
#include <iostream>
#include "TObject.h"
#include "TH1F.h"
#include "TH1.h"
#include "TProfile.h"
#include "TString.h"
#include "TObject.h"
#include "TDirectory.h"

#include "TMean.h"
#include "MyPart.h"
#include "BasePlots.h"

using namespace std;

class MultiPlots : public BasePlots {

  public:
  MultiPlots(){plotsname="none";this->init();};
  MultiPlots(TString collname){plotsname=collname;this->init();};
  ~MultiPlots();
  
  void fill(MyPart&,double = 1.);
  void nextEvent(bool = true , double = 1.);
  void makeKNO();
  void write(bool = true);
  
  //getters/setters
  vector<double> getMoments();
  void setNbOfMoments(Int_t);
  void SetEnergy(Double_t e){energy=e;};
  TH1* get(TString);
  
  TMean* nch_mean;
  TH1F*  nch;
  TH1F*  kno;
  TH1F*  rapidity;
  TH1F*  eta;
  TH1F*  pt;
  TH1F*  pt2;
  TProfile* ptmVSnch;
  TProfile* ptVSnch;
  
  Int_t nb_moments;
  vector<TMean>* moments;
  
  Double_t energy;
  Double_t nbEvts;
  Int_t nch_inEvt;
  TMean* ptm_inEvt;
  vector<Double_t> vpt_inEvt;
  
  private:
  TString multiname;
  
  void writeSummary();
  void init();
  
  ClassDef(MultiPlots,2);

};

#endif
