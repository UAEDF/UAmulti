
#include "UAmulti/ChPartTree/interface/MyEvtId.h"

ClassImp(MyEvtId)

MyEvtId::MyEvtId()
{ }

MyEvtId::~MyEvtId()
{ }

void MyEvtId::Reset()
{
  Run =      0;
  Evt =      0;
  LumiSect = 0;
  Time     = -1;
  IsData   = false;
  ExpType  = 0;
  Bunch    = -1;
  Orbit    = -1;

}
