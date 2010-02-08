#ifndef __MyGenKin_H__
#define __MyGenKin_H__

#include "TObject.h"
#include "TLorentzVector.h"

class MyGenKin : public TObject {

  public :
  MyGenKin();
  virtual ~MyGenKin();

  // General Info
  Int_t    MCProcId ;   
  Double_t Scale  ;

  // Pdf Info
  Double_t x1 ;
  Double_t x2 ;
  Double_t Q ;
  Int_t    Part1Id ;
  Int_t    Part2Id ;

  // GenMet from MET Collection
  Double_t Met ; 
  Double_t MetX; 
  Double_t MetY; 
  Double_t MetPhi; 

  // GenMet form GenPart Collection (status 1 and 3)
  TLorentzVector MetGP1 ;
  TLorentzVector MetGP3 ;

  private:

  ClassDef (MyGenKin,1)
};

#endif
