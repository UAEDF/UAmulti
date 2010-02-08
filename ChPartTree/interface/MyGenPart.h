#ifndef __MyGenPart_H__
#define __MyGenPart_H__

#include <string>
#include "TObject.h"
#include "UAmulti/ChPartTree/interface/MyPart.h"

class MyGenPart : public TObject {

  public :
    MyGenPart();
    virtual ~MyGenPart();

    MyPart      Part;
    Int_t       pdgId,status,mo1,mo2,da1,da2;
    std::string name;

  private:

  ClassDef (MyGenPart,1)
};

#endif

