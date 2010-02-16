#ifndef __MyPart_H__
#define __MyPart_H__

#include "TObject.h"
#include "TLorentzVector.h"

class MyPart : public TObject {

  public :
    MyPart();
    virtual ~MyPart();

  // private:

    Double_t       charge;
    TLorentzVector v;

    TLorentzVector vmpi();

  private:

    ClassDef (MyPart,1)
};



#endif


