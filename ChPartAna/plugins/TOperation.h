#ifndef __TOperation_H
#define __TOperation_H

#include "TNamed.h"
#include "TCollection.h"
#include "TString.h"
#include "TBrowser.h"
#include "TDirectory.h"
#include "TCanvas.h"

#include <iostream>
using namespace std;

template <class T>
class TOperation : public TNamed{
  public :
    TOperation();
    TOperation(T& , T&);
    TOperation(TString , T& , T&);
    TOperation(T& , T& , TString);
    TOperation(TString , T& , T& , TString);
    
    //~TOperation(){};
    
    Bool_t setOperation(TString);
    
    Bool_t Add();
    Bool_t Substract();
    Bool_t Multiply();
    Bool_t Divide();
    
    TOperation<T> operator+(const TOperation&);
    TOperation<T> Add(const TOperation<T>& c2){return *this + c2;};
    Long64_t Merge(TCollection* list);
    
    void write(Bool_t = 0);
    void Draw(Option_t* = "");
    void Browse(TBrowser *b);
    
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
