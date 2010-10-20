#ifndef TTPROFILE_H
#define TTPROFILE_H

#include <iostream>
#include <vector>
#include <sstream>
#include "TObject.h"
#include "TDirectory.h"
#include "TMean.h"
#include "TMoments.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TProfile.h"
#include "TMath.h"
using namespace std;

class TTProfile : public TObject{

  public:
  TTProfile();
  TTProfile(TString);
  TTProfile(TString, Int_t , Double_t , Double_t);
  TTProfile(TString, vector<Double_t>);
  TTProfile(TString, Int_t , Double_t , Double_t , Int_t , Double_t , Double_t);
  TTProfile(TString, vector<Double_t> , vector<Double_t>);
  //TTProfile(Int_t , Double_t*, Int_t , Double_t*);
  
  void Init();
  void clear();
  ~TTProfile(){this->clear();};
  
  vector<double>  makeBins(Int_t, Double_t, Double_t);
  void Fill(Double_t, Double_t, Double_t=1);
  //void Add(TH1F*);
  //void Add(TGraph*);

  //void ComputeSlice();
  //void ComputeErrorsFromResampling(TH1F*);
  
  //void print();
  void makeProfile(TString = "RMS"); // For now only RMS
  void write(Bool_t = false , Bool_t = false);
  TTProfile& operator=(const TTProfile&);
  //TTProfile(const TTProfile&);
  
  //Int_t nSlice;
  Int_t nBins;
  Int_t nEvents;
  TString name;
  Bool_t profileDone;
  
  static Bool_t debug;
  
  static Bool_t    switchTH1On;
  static Bool_t    switchTH2On;
  static Bool_t    switchTMomentsOn;
  
  vector<TH1F>     vth1;
  vector<TMean>    vmean;
  vector<TMoments> vmom;
  TProfile	   tprofile;
  TH1F	           profile;
  TH1F	           nEntries;
  TH2F	           th2;
  
  vector< Double_t >         binning;
  vector< vector<Double_t> > vbinning;
  
  private:

  ClassDef(TTProfile,1);
};

#endif
