#ifndef __TOperation_H
#define __TOperation_H

#include "TObject.h"
#include "TString.h"

template <class T>
class TOperation : public TObject{
  public :
    TOperation();
    TOperation(TString , T& , T&);
    TOperation(TString , T& , T& , TString);
    
    ~TOperation(){};
    
    Bool_t setOperation(TString);
    
    Bool_t Add();
    Bool_t Substract();
    Bool_t Multiply();
    Bool_t Divide();
    
    TOperation<T> operator+(const TOperation&);
    TOperation<T> Add(const TOperation<T>& c2){return *this + c2;};
    
    void write();
    
    TString name;
    
    T obj1;
    T obj2;
    T result;
    
    
  private :
    TString operation;

  ClassDef (TOperation,1)
};

#include "TOperation.C"

#endif
