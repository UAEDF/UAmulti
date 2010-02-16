
#include "UAmulti/ChPartTree/interface/MyPart.h"

ClassImp(MyPart)

MyPart::MyPart()
{ }

MyPart::~MyPart()
{ }

TLorentzVector MyPart::vmpi()
{
  Double_t mpion = 139.57018;
  TLorentzVector vm;
  vm.SetPtEtaPhiM( this->v.Pt() , this->v.Eta() , this->v.Phi() , mpion );
  return vm;
}
