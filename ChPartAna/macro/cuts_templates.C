
bool isInAcceptance(MyPart& p , double pt , double eta , double charge = 0){
  if(p.v.Pt()<pt) return false;
  if(fabs(p.v.Eta())>eta) return false;
  if(charge!=0)
    if(charge!=p.charge)return false;
    
  return true;
}


//------------------------------ GET TRACKS IN ACCEPTANCE -----------------------------------------
template <class T>
vector<T> getInAcc(vector<T> v_tr , double pt = pt_cut, double eta = eta_cut,double charge = 0.){

  for(typename vector<T>::iterator it_tr = v_tr.begin() ; it_tr != v_tr.end() ; ++it_tr)
    if( !isInAcceptance(it_tr->Part,pt,eta,charge) ) v_tr.erase(it_tr--);

  return v_tr;
}

template <class T>
int getnInAcc(vector<T>* v_tr , double pt = pt_cut, double eta = eta_cut, double charge = 0.){

  int nch=0;
  for(typename vector<T>::iterator it_tr = v_tr->begin() ; it_tr != v_tr->end() ; ++it_tr)
    if( isInAcceptance(it_tr->Part,pt,eta,charge) ) ++nch;

  return nch;
}
