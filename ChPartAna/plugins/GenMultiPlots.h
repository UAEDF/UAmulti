#ifndef GenMultiPlots_H
#define GenMultiPlots_H

#include "TObject.h"
#include "TH1F.h"
#include "TString.h"
#include "TObject.h"

#include "MultiPlots.h"
#include "MyGenKin.h"

using namespace std;

class GenMultiPlots : public TObject{

  public:
  GenMultiPlots();
  GenMultiPlots(TString);
  ~GenMultiPlots();
  
  void fill(MyGenKin& , MyPart& , double = 1.);
  void nextEvent(MyGenKin& , double = 1.);
  void write();
  
  MultiPlots* mp_INC;
  MultiPlots* mp_SD;
  MultiPlots* mp_DD;
  MultiPlots* mp_ND;
  MultiPlots* mp_NSD;
  
  private:
  TString plotsname;
  void init();
  bool isSD(MyGenKin&);
  bool isDD(MyGenKin&);
  
  ClassDef(GenMultiPlots,1);

};

#endif
