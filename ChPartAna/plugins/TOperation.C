
template <class T>
TOperation<T>::TOperation(){

}


template <class T>
TOperation<T>::TOperation(T& o1 , T& o2):obj1(o1) , obj2(o2){}

template <class T>
TOperation<T>::TOperation(TString n , T& o1 , T& o2):TNamed("TOperation_class_"+n,"TOperation_class_"+n) , name(n) , obj1(o1) , obj2(o2){}


template <class T>
TOperation<T>::TOperation(T& o1 , T& o2 , TString op):obj1(o1) , obj2(o2){
  this->setOperation(op);
}

template <class T>
TOperation<T>::TOperation(TString n , T& o1 , T& o2 , TString op):TNamed("TOperation_class_"+n,"TOperation_class_"+n) , name(n) , obj1(o1) , obj2(o2){
  this->setOperation(op);
}

template <class T>
Bool_t TOperation<T>::setOperation(TString op){
  operation=op;
  result.SetName("result_"+name);
  if(op=="+")        result.Add(&obj1,&obj2,1,1);
  else if(op=="-")   result.Add(&obj1,&obj2,1,-1);
  else if(op=="*")   result.Multiply(&obj1,&obj2,1,1);
  else if(op=="/")   result.Divide(&obj1,&obj2,1,1);
  else if(op=="eff") result.Divide(&obj1,&obj2,1,1,"B");
  else
    return false;
    
  return true;

}


template <class T>
Bool_t TOperation<T>::Add(){
  return this->setOperation("+");
}

template <class T>
Bool_t TOperation<T>::Substract(){
  return this->setOperation("-");
}

template <class T>
Bool_t TOperation<T>::Multiply(){
  return this->setOperation("*");
}

template <class T>
Bool_t TOperation<T>::Divide(){
  return this->setOperation("/");
}


template <class T>
TOperation<T> TOperation<T>::operator+(const TOperation<T>& class2){
  if(this->operation != class2.operation) return TOperation<T>();
  this->obj1.Add(&(class2.obj1));
  this->obj2.Add(&(class2.obj2));
  this->setOperation(this->operation);
  return *this;
}

template <class T>
Long64_t TOperation<T>::Merge(TCollection* list){
  if (!list) return 0;
  if (list->IsEmpty()) return 0;
  
  TList coll1;
  TList coll2;
  
  TIter next(list);
  while (TObject *o = next()) {
    TOperation<T>* h = dynamic_cast<TOperation<T>* > (o);
    this->Add(*h);
  }
  next.Reset();
}
    
    
    
template <class T>
void TOperation<T>::write(){
  this->obj1.Write();
  this->obj2.Write();
  this->result.Write();
  this->Write("TOperation_class_"+name);
}

template <class T>
void TOperation<T>::Draw(Option_t* option){

  cout << "Drawing TOperation " << name << endl;
  result.Draw(option); 
}

template <class T>
void TOperation<T>::Browse(TBrowser *b){

  this->Draw(b ? b->GetDrawOption() : "");
  gPad->Update();

}
