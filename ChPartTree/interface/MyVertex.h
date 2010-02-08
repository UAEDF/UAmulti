#ifndef __MyVertex_H__
#define __MyVertex_H__

#include "TObject.h"

class MyVertex : public TObject {

  public :
    MyVertex();
    virtual ~MyVertex();

    Int_t    id;

    Double_t x,y,z;
    Double_t ex,ey,ez;    

    Bool_t validity;  
    Bool_t fake    ;
    Double_t chi2n ;
   
    Int_t  ntracks ;  

    //MyPart      Part;
    //Int_t       pdgId,status,mo1,mo2,da1,da2;
    //std::string name;

  private:

  ClassDef (MyVertex,1)
};

#endif

