#ifndef __MatchTools_C__
#define __MatchTools_C__

/*bool isMatched ( MyGenPart *gp1 , MyGenPart *gp2 , double dRCut = 0.1 ) {
  double dR = deltaR( gp1->eta , gp2->eta , gp1->phi , gp2->phi  ) ;
  if ( dR <= dRCut ) return true;
  return false;
} 

bool isMatched ( MyGenPart *gp , MyElectron *ele , double dRCut = 0.1 ) {
  double dR = deltaR( gp->eta , ele->eta , gp->phi , ele->phi  ) ;
  if ( dR <= dRCut ) return true;
  return false;
} 

bool isMatched ( MyGenPart *gp , MyMuon *muo , double dRCut = 0.1 ) {
  double dR = deltaR( gp->eta , muo->eta , gp->phi , muo->phi  ) ;
  if ( dR <= dRCut ) return true;
  return false;
} 

bool isMatched ( TVector3 vp ,  TVector3 vr , double dRCut = 0.1 ) {
  double dR = deltaR( vp.Eta() , vr.Eta() , vp.Phi() , vr.Phi()  ) ;
  if ( dR <= dRCut ) return true;
  return false;
} 



double GenPartMatch( vector<MyGenPart*> vgpref , vector<MyGenPart*> vgppro ) {

  double mindR = 999. ;

  for(vector<MyGenPart*>::iterator itr = vgpref.begin() ; itr != vgpref.end() ; ++itr){
     for(vector<MyGenPart*>::iterator itp = vgppro.begin() ; itp != vgppro.end() ; ++itp){
        double dR = deltaR( (*itr)->eta , (*itp)->eta , (*itr)->phi , (*itp)->phi  ) ;
        if ( fabs(dR) <  fabs(mindR) )  mindR = dR ;
     }
  }
  return mindR;

}

double RecoElectronMatch( vector<MyGenPart*> vgpref , vector<MyElectron*> velepro ) {

  double mindR = 999. ;

  for(vector<MyGenPart*>::iterator itr = vgpref.begin() ; itr != vgpref.end() ; ++itr){
    for(vector<MyElectron*>::iterator itp = velepro.begin() ; itp != velepro.end() ; ++itp){
        double dR = deltaR( (*itr)->eta , (*itp)->eta , (*itr)->phi , (*itp)->phi  ) ;
        if ( fabs(dR) <  fabs(mindR) )  mindR = dR ;
    }
  }
  return mindR;

}

double RecoMuonMatch( vector<MyGenPart*> vgpref , vector<MyMuon*> velepro ) {

  double mindR = 999. ;

  for(vector<MyGenPart*>::iterator itr = vgpref.begin() ; itr != vgpref.end() ; ++itr){
    for(vector<MyMuon*>::iterator itp = velepro.begin() ; itp != velepro.end() ; ++itp){
        double dR = deltaR( (*itr)->eta , (*itp)->eta , (*itr)->phi , (*itp)->phi  ) ;
        if ( fabs(dR) <  fabs(mindR) )  mindR = dR ;
    }
  }
  return mindR;

}*/
#ifndef M_PI
#define M_PI 3.1415
#endif

double deltaR(double eta1, double eta2, double phi1, double phi2){
  double dphi = fabs(phi1-phi2);
  if(dphi > M_PI)
    dphi = (2*M_PI - dphi);
      
  double deta = fabs(eta1-eta2);
  return sqrt(dphi*dphi + deta*deta);
}

double deltaPt(double pt1 , double pt2){
  return fabs(pt1 - pt2) / pt2;
}



/*
template <class T>
bool isMatched(T *part1 , T *part2 , double dRCut = 0.1 , double dPtCut = -1) {
  double dPt = fabs(part1->pt - part2->pt) / part1->pt ;
  double dR = deltaR( part1->eta , part2->eta , part1->phi , part2->phi  ) ;
  if ( dR <= dRCut && dPtCut > 0 && dPt <= dPtCut) ) return true;
  if ( dR <= dRCut && dPtCut < 0                   ) return true;
  return false;
} 

template <class T , class U>
double Match( vector<T*> vp1 , vector<U*> vp2 ) {

  double mindR = 999. ;

  for(vector<T*>::iterator it1 = vp1.begin() ; it1 != vp1.end() ; ++it1){
    for(vector<U*>::iterator it2 = vp2.begin() ; it2 != vp2.end() ; ++it2){
        double dR = deltaR( (*it1)->eta , (*it2)->eta , (*it1)->phi , (*it2)->phi  ) ;
        if ( fabs(dR) <  fabs(mindR) )  mindR = dR ;
    }
  }
  return mindR;
}*/

template <class T , class U>
bool isMatched(T *part1 , U *part2 , double dRCut = 0.1 , double dPtCut = -1) {
  double dPt = deltaPt(part1->Pt() , part2->Pt());
  double dR = deltaR( part1->Eta() , part2->Eta() , part1->Phi() , part2->Phi() ) ;
  if ( dR <= dRCut && dPtCut > 0 && dPt <= dPtCut) return true;
  if ( dR <= dRCut && dPtCut < 0                 ) return true;
  return false;
}

template <class T , class U>
double matchValue(T *part1 , U *part2 , double dRCut = 0.1 , double dPtCut = -1){
  if(!isMatched(part1 , part2 , dRCut , dPtCut)) return 1000.;
  double val = pow(deltaR( part1->Eta() , part2->Eta() , part1->Phi() , part2->Phi() ) , 2);
  if(dPtCut > 0) val += pow(deltaPt(part1->Pt() , part2->Pt()) , 2);
  return sqrt(val);
}

template <class T , class U>
double Match( vector<T*> vp1 , vector<U*> vp2 ) {

  double mindR = 999. ;

  for(typename vector<T*>::iterator it1 = vp1.begin() ; it1 != vp1.end() ; ++it1){
    for(typename vector<U*>::iterator it2 = vp2.begin() ; it2 != vp2.end() ; ++it2){
        double dR = deltaR( (*it1)->Eta() , (*it2)->Eta() , (*it1)->Phi() , (*it2)->Phi()  ) ;
        if ( fabs(dR) <  fabs(mindR) )  mindR = dR ;
    }
  }
  return mindR;
}

//the same U can be assigned to 2 T base different ==> ambiguous, see GetMatch2 for unambiguous matching
template <class T , class U>
void GetMatch(vector<T>* vbase , vector<U>* vtoMatch , vector< pair<T*,U*> >* vmatched ,vector<U*>* vfakes ,
              vector<T*>* vbaseNotMatched , double dRCut = 0.1 , double dPtCut = -1){
  double val = 999;
  vector<bool> visMatched(vtoMatch->size() , 0);
  
  for(int i = 0 ; i < vbase->size() ; ++i){
    double minval = 999;
    int matchedTo = -1;
    
    for(int j = 0 ; j < vtoMatch->size() ; ++j){
      val = matchValue( vbase->at(i) , vtoMatch->at(j) , dRCut , dPtCut );
      if(val < minval){
        minval = val;
	matchedTo = j;
      }
    }
    
    if(matchedTo > -1){
      vmatched->push_back( make_pair( &(vbase->at(i)) , &(vtoMatch->at(matchedTo)) ) );
      visMatched.at(matchedTo) = 1;
    }
    else
      vbaseNotMatched->push_back( &(vbase->at(i)) );
  }
  
  for(int j = 0 ; j < vtoMatch->size() ; ++j){
    if(! visMatched.at(j) )
      vfakes->push_back( &(vtoMatch->at(j)) );
  }
}

pair<int , int> getMinPos( vector< vector<double> >& val ){
  double minval = 10000;
  int imin = -1 , jmin = -1;
  for(unsigned int i = 0 ; i < val.size() ; ++i){
    //if(val[i][0] > 9999) continue;
    for(unsigned int j = 0 ; j < val[i].size() ; ++j){
      if(val[i][j] < minval){
        imin = i;
	jmin = j;
	minval = val[i][j];
      }
    }
  }
  
  //cout << "minval , pos = " << minval << "  " << imin << "  " << jmin << endl;
  return make_pair( imin , jmin );
}

//Unambiguous matching, but slower
template <class T , class U>
void GetMatch2(vector<T>* vbase , vector<U>* vtoMatch , vector<pair<T*,U*> >* vmatched ,vector<U*>* vfakes ,
               vector<T*>* vbaseNotMatched, double dRCut = 0.4 , double dPtCut = -1 , bool print = 0){
	       
  if(print) cout << endl << " ++++++++   NEW EVENT   ++++++++" << endl << endl ;
  
  vector< vector<double> > val( vbase->size() , vector<double>(vtoMatch->size() , 999) );
  vector<bool> vbaseIsMatched(vbase->size() , 0);
  vector<bool> vtoMatchIsMatched(vtoMatch->size() , 0);
  
  
  for(unsigned int i = 0 ; i < vbase->size() ; ++i){
    for(unsigned int j = 0 ; j < vtoMatch->size() ; ++j){
      val[i][j] = matchValue( & vbase->at(i) , & vtoMatch->at(j) , dRCut , dPtCut );
      if(print) cout << val[i][j] << "  ";
    }
    if(print)   cout << endl;
  }
  if(print)     cout << endl;
  
  
  pair<int , int> minPos = make_pair(0,0);
  while( 1 ){
    minPos = getMinPos(val);
    //cout << "minPos = " << minPos.first << endl;
    
    //break if all vbase done
    if(minPos.first < 0 ) break;
    
    //break if no more match is available
    if(val[minPos.first][minPos.second] > 998) break;
  
    //making the actual matching
    vmatched->push_back( make_pair( &(vbase->at(minPos.first)) , &(vtoMatch->at(minPos.second)) ) );
    
    //keeping track of who is matched
    vbaseIsMatched.at(minPos.first)     = 1;
    vtoMatchIsMatched.at(minPos.second) = 1;
    
    //clearing the matched columns/lines
    val[minPos.first] = vector<double>(val[minPos.first].size() , 10000);
    for(unsigned int i = 0 ; i < val.size() ; ++i)
      val[i][minPos.second] = 10000;
  
  }
  
  
  for(unsigned int i = 0 ; i < vbase->size() ; ++i){
    for(unsigned int j = 0 ; j < vtoMatch->size() ; ++j){
      if(print) cout << val[i][j] << "  ";
    }
    if(print)   cout << endl;
  }
  if(print)     cout << endl;
  
  
  for(unsigned int i = 0 ; i < vbase->size() ; ++i){
    if(! vbaseIsMatched.at(i) )
      vbaseNotMatched->push_back( &(vbase->at(i)) );
  }
  
  for(unsigned int j = 0 ; j < vtoMatch->size() ; ++j){
    if(! vtoMatchIsMatched.at(j) )
      vfakes->push_back( &(vtoMatch->at(j)) );
  }
  
  if(print){
    cout << "Number of " << vbase->at(0).ClassName() << " : " << vbase->size() << endl;
    cout << "Number of " << vtoMatch->at(0).ClassName() << " : " << vtoMatch->size() << endl;
    cout << endl << endl;
    
    cout << "All matched pairs : " << vmatched->size() << endl;
    for(unsigned int i=0; i < vmatched->size() ; ++i){
      cout << " ==> matched with dR = "
           << deltaR(vmatched->at(i).first->Eta() , vmatched->at(i).second->Eta() , vmatched->at(i).first->Phi() , vmatched->at(i).second->Phi())
           << " and dPt = "
	   << deltaPt(vmatched->at(i).first->Pt() , vmatched->at(i).second->Pt())
	   << endl;
    }
  
    cout << "All fake " << vtoMatch->at(0).ClassName() << " : " << vfakes->size() << endl << endl;
    cout << "All not matched " << vbase->at(0).ClassName() << " : " << vbaseNotMatched->size() << endl << endl;
    
  }
  
  
  
}

#endif



