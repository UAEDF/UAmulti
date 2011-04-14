#ifndef FBRESULTS_H
#define FBRESULTS_H

#include "FBCorrel.h"
#include "../macro/FBacc_struct.h"

class FBResults : public TObject{

  public:
  FBResults();
  FBResults(TString);
  FBResults(TString , vector<FBCorrel*>& , Double_t , Double_t);

  void Init();
  ~FBResults(){};
  
  void MakeAllPlots( vector<FBCorrel*>& , Double_t , Double_t );
  void GetbVSdEta(vector<FBCorrel*>& , int , int , TGraph& );
  void GetbVSdEtaFromFit(vector<FBCorrel*>& , int , int , TGraph& );
  void GetsigCVSeta(vector<FBCorrel*>& , int , int , TGraph& );
  void write();
  
  static Bool_t debug;
  static vector<FBacc>* accMap;
  
  TString name;
  TString dirname;
  
  TGraph bVSdEta_fromReg;
  TGraph bVSdEta_fromFit;
  TGraph sigCVSeta;
  
  private:

  ClassDef(FBResults,1);

};

#endif
