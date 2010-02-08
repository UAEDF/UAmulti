#ifndef __MyTracks_H__
#define __MyTracks_H__

#include <string>
#include "TObject.h"
#include "UAmulti/ChPartTree/interface/MyPart.h"

class MyTracks : public TObject {

  public :
    MyTracks();
    virtual ~MyTracks();

    //Double_t px,py,pz,eta;

    MyPart      Part;

    Bool_t   quality[3];
    Int_t   nhit;
    Double_t chi2n;
    Double_t dz  , d0 ;
    Double_t edz , ed0 , ept ; 


  private:

  ClassDef (MyTracks,1)
};

#endif

