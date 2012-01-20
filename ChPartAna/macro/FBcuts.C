double pt_cut   = 0.15;
double eta_cut  = 2.4;
double vtxz_cut = 10.;
double nsigma   = 4.;

//----------------- GET THE BEST VERTEX ------------------
int getBestVertex(vector<MyVertex>* vtxcoll){

  vector<MyVertex>::iterator goodVtx;
  int ntracks = -1;
  double chi2n=999;
  
  for(vector<MyVertex>::iterator itvtx=vtxcoll->begin();itvtx!=vtxcoll->end();++itvtx){
    if(itvtx->validity && fabs(itvtx->z)<vtxz_cut){
      if(itvtx->ntracks>ntracks){
        goodVtx  =  itvtx;
	ntracks  =  itvtx->ntracks;
	chi2n    =  itvtx->chi2n();
      }
      else if(itvtx->ntracks==ntracks){
        if(chi2n>itvtx->chi2n()){
	  goodVtx  =  itvtx;
	  ntracks  =  itvtx->ntracks;
	  chi2n    =  itvtx->chi2n();
	}
      }
    }
  }
  
  return (ntracks<=0 ? -1 : goodVtx->id);
}

MyVertex* GetBestVertex(vector<MyVertex>* vtxcoll , bool return_null = 1){

  vector<MyVertex>::iterator goodVtx;
  int ntracks = -1;
  double chi2n=999;

  for(vector<MyVertex>::iterator itvtx=vtxcoll->begin();itvtx!=vtxcoll->end();++itvtx){
    if(itvtx->validity && fabs(itvtx->z)<vtxz_cut){
      if(itvtx->ntracks>ntracks){
        goodVtx  =  itvtx;
        ntracks  =  itvtx->ntracks;
        chi2n    =  itvtx->chi2n();
      }
      else if(itvtx->ntracks==ntracks){
        if(chi2n>itvtx->chi2n()){
          goodVtx  =  itvtx;
          ntracks  =  itvtx->ntracks;
          chi2n    =  itvtx->chi2n();
        }
      }
    }
  }

  if(ntracks<=0 && return_null) return 0;
  else if(ntracks<=0)		return new MyVertex();
  else          		return &(*goodVtx);
}


inline int getVtxposFromId(const MyTracks& tr,int goodvtxId){
  for(unsigned int i=0;i<tr.vtxid.size();++i)
    if(tr.vtxid.at(i)==goodvtxId)
      return i;
  
  return -1;
}
 
//--------------------------- Get the Part in the good acceptance -----------------------------

template <class T>
bool isInAcceptance(const T& p , double pt , double eta , double charge = 0){
  if(p.Pt()<pt) return false;
  if(fabs(p.Eta())>eta) return false;
  if(charge!=0)
    if(charge!=p.charge)return false;
  return true;
}


//------------------ UE SELECTION ----------------------------------------
bool isTrackPrimary(const MyTracks& tr, int goodvtxId){

  if(debug) cout<<" +-+-+ starting isTrackPrimary"<<endl;
  
  //get the number of the good vertex in the vector
  int vtxnum = getVtxposFromId(tr,goodvtxId);
  
  //Make the cuts:
  //if( tr.Pt()<pt_cut ) return false;
  //if( fabs(tr.Eta())>eta_cut) return false;
  
  //--------- PV track cut from Luca -----------
  if( fabs(tr.vtxdxy.at(vtxnum)) / tr.ed0 > 5 ) return false;
  if( fabs(tr.vtxdz.at(vtxnum)) / tr.edz > 5 ) return false;
  if( tr.ept / tr.Pt() > 0.1 ) return false;
  
  //To use only with generalTracks, otherwise it's filled to 0 by default
  //if( ! tr.quality[2]) return false;
  
  
  if(debug) cout<<" ** The track is primary"<<endl;
  
  return true;
}




//------------------ FERENC SELECTION ----------------------------------------
bool isTrackPrimary(MyTracks& tr, vector<MyVertex>& vtxcoll, int goodvtxId, MyBeamSpot* bs){

  if(debug) cout<<" +-+-+ starting isTrackPrimary"<<endl;
  
  //get the number of the good vertex in the vector
  //int vtxnum = getVtxposFromId(tr,goodvtxId);
  
  //get the good vertex iterator
  vector<MyVertex>::iterator goodVtx = vtxcoll.end();
  for(vector<MyVertex>::iterator it_vtx = vtxcoll.begin();it_vtx != vtxcoll.end();++it_vtx)
    if(goodvtxId==it_vtx->id)
      goodVtx = it_vtx;
  
  //Make the cuts:
  //if( tr.Pt()<pt_cut ) return false;
  //if( fabs(tr.Eta())>eta_cut) return false;
  if( tr.ept / tr.Pt() > 0.1 ) return false;
  
  
  //--------- PV track cut from Ferenc -----------
  double sigma_t = sqrt( pow(tr.ed0,2) +
                   bs->BeamWidthX *
                   bs->BeamWidthY );
  if( fabs(tr.vtxdxy.at(0)) > min(0.2 , nsigma * sigma_t) ) return false;
  
  
  // Longitudinal impact parameter (4*sigma)
  double sigma_z = sqrt( pow(tr.edz,2) +
                    pow(cosh(tr.Eta()),2) *
                    bs->BeamWidthX *
                    bs->BeamWidthY);
  //if( fabs(tr.vtxdz.at(vtxnum)) > 4 * sigma_z ) return false; // FIXME
  if( fabs(tr.vz - goodVtx->z) > nsigma * sigma_z ) return false; // FIXME

  
  //To use only with generalTracks, otherwise it's filled to 0 by default
  //if( ! tr.quality[2] && trackcoll=="generalTracks") return false;
  
  
  if(debug) cout<<" ** The track is primary"<<endl;
  
  return true;
}


bool isTrackPrimary_highNCH(const MyTracks& tr, int goodvtxId , double dzMax = 999){

  int vtxnum = getVtxposFromId(tr,goodvtxId);
  
  if( fabs(tr.vtxdxy.at(vtxnum)) / tr.ed0 > 3 ) return false;
  if( fabs(tr.vtxdz.at(vtxnum)) / tr.edz > 3 ) return false;
  //if( tr.ept / tr.Pt() > 0.05 ) return false; //0.1
  if( fabs(tr.vtxdz.at(vtxnum)) > dzMax ) return false;
  return true;


}



//---------------------- GET THE NUMBER OF PRIMARY TRACKS ---UEEVENTS --
int getnPrimaryTracks(vector<MyTracks>* v_tr , vector<MyVertex>* vtxcoll , double pt = pt_cut, double eta = eta_cut, double charge = 0){
  
  if(debug) cout<<" +-+-+ starting getnPrimaryTracks"<<endl;
  
  int nch = 0;
  
  //Protecting code if no vertex were found
  if(getBestVertex(vtxcoll)==-1) return 0;
  
  
  for(vector<MyTracks>::iterator it_tr = v_tr->begin() ; it_tr != v_tr->end() ; ++it_tr)
    if( isTrackPrimary(*it_tr , getBestVertex(vtxcoll)) && isInAcceptance(*it_tr,pt,eta,charge) )
      ++nch;
      
  if(debug) cout<<" ** getnPrimaryTracks() has found "<<nch<<" primary tracks"<<endl;
  
  return nch;
}


//---------------------- GET THE NUMBER OF PRIMARY TRACKS -- FERENC ----
int getnPrimaryTracks(vector<MyTracks>* v_tr , vector<MyVertex>* vtxcoll, MyBeamSpot* bs , double pt = pt_cut, double eta = eta_cut, double charge = 0){

  if(debug) cout<<" +-+-+ starting getnPrimaryTracks"<<endl;

  int nch = 0;

  //Protecting code if no vertex were found
  if(getBestVertex(vtxcoll)==-1) return 0;


  for(vector<MyTracks>::iterator it_tr = v_tr->begin() ; it_tr != v_tr->end() ; ++it_tr)
    if( isTrackPrimary(*it_tr , *vtxcoll , getBestVertex(vtxcoll), bs) && isInAcceptance(*it_tr,pt,eta,charge) )
      ++nch;

  if(debug) cout<<" ** getnPrimaryTracks() has found "<<nch<<" primary tracks"<<endl;

  return nch;
}




//------------------------------ GET TRACKS IN ACCEPTANCE -----------------------------------------
template <class T>
vector<T> getInAcc(vector<T> v , double pt = pt_cut, double eta = eta_cut,double charge = 0.){
    
  for(typename vector<T>::iterator it = v.end()-1 ; it != v.begin()-1 ; --it)
    if( !isInAcceptance(it,pt,eta,charge) )  v.erase(it);

  return v;

}


//------------------------------ GET NUMBER OF TRACKS IN ACCEPTANCE -----------------------------------------
template <class T>
int getnInAcc(vector<T>* v , double pt = pt_cut, double eta = eta_cut, double charge = 0.){

  int nch=0;
  for(typename vector<T>::iterator it = v->begin() ; it != v->end() ; ++it)
    if( isInAcceptance(*it,pt,eta,charge) ) ++nch;

  return nch;

}

//------------------------------ GET THE PRIMARY gTR -----------------------------------------
void getPrimaryTracks(vector<MyTracks>& v_tr , int vtxId , double pt = pt_cut, double eta = eta_cut, double charge = 0, bool highPurityOnly = false){

  if(debug) cout<<" +-+-+ starting getPrimaryTracks"<<endl;
  
  //Protecting code if no vertex were found
  if(vtxId==-1) v_tr.clear();
  
  
  for(vector<MyTracks>::iterator it_tr = v_tr.end()-1 ; it_tr != v_tr.begin()-1 ; --it_tr)
    if( ! isTrackPrimary(*it_tr , vtxId) || !isInAcceptance(*it_tr,pt,eta,charge) || (highPurityOnly && !it_tr->quality[2]) )
      v_tr.erase(it_tr);
      
  if(debug) cout<<" ** "<<v_tr.size()<<" tracks remaining after primary selection"<<endl;
}

inline void getPrimaryTracks(vector<MyTracks>& v_tr , vector<MyVertex>* vtxcoll ,  double pt = pt_cut, double eta = eta_cut, double charge = 0 , bool highPurityOnly = false){
  getPrimaryTracks(v_tr , getBestVertex(vtxcoll) , pt , eta , charge , highPurityOnly);
}



//------------------------------ GET THE PRIMARY gTR highNCH -----------------------------------------
void getPrimaryTracks_highNCH(vector<MyTracks>& v_tr , int vtxId , double pt = pt_cut, double eta = eta_cut,
                   double charge = 0, bool highPurityOnly = false , double dzMax = 999){

  if(debug) cout<<" +-+-+ starting getPrimaryTracks"<<endl;
  
  //Protecting code if no vertex were found
  if(vtxId==-1) v_tr.clear();

   
  for(vector<MyTracks>::iterator it_tr = v_tr.end()-1 ; it_tr != v_tr.begin()-1 ; --it_tr){
    /*int vtxnum = getVtxposFromId(*it_tr,vtxId);
    cout << it_tr->Pt() << "  "
         << it_tr->Eta() << "  "
         << it_tr->vtxdz.at(vtxnum) << "  "
         << fabs(it_tr->vtxdxy.at(vtxnum)) / it_tr->ed0 << "  "
         << fabs(it_tr->vtxdz.at(vtxnum)) / it_tr->edz << endl;
	 
	 
  
    cout << isTrackPrimary_highNCH(*it_tr , vtxId , dzMax) << "  " 
         << isInAcceptance(*it_tr,pt,eta,charge) << "  " << (highPurityOnly && (!it_tr->quality[2]))  << endl;*/
    if( (!isTrackPrimary_highNCH(*it_tr , vtxId , dzMax)) || (!isInAcceptance(*it_tr,pt,eta,charge)) || ((highPurityOnly && !it_tr->quality[2])) )
      v_tr.erase(it_tr);
  }
      
  if(debug) cout<<" ** "<<v_tr.size()<<" tracks remaining after primary selection"<<endl;
}


//------------------------------ GET THE PRIMARY mbTR -----------------------------------------
void getPrimaryTracks(vector<MyTracks>& v_tr , vector<MyVertex>* vtxcoll, int vtxId, MyBeamSpot* bs, double pt = pt_cut, double eta = eta_cut, double charge = 0){

  if(debug) cout<<" +-+-+ starting getPrimaryTracks using Ferenc association"<<endl;
  
  //Protecting code if no vertex were found
  if(vtxId==-1) v_tr.clear();
  
  for(vector<MyTracks>::iterator it_tr = v_tr.end()-1 ; it_tr != v_tr.begin()-1 ; --it_tr)
    if( ! isTrackPrimary(*it_tr , *vtxcoll, vtxId, bs ) || !isInAcceptance(*it_tr,pt,eta,charge) )
       v_tr.erase(it_tr);
      
  if(debug) cout<<" ** "<<v_tr.size()<<" tracks remaining after primary selection"<<endl;
}

inline void getPrimaryTracks(vector<MyTracks>& v_tr , vector<MyVertex>* vtxcoll, MyBeamSpot* bs, double pt = pt_cut, double eta = eta_cut, double charge = 0){
  getPrimaryTracks( v_tr, vtxcoll, getBestVertex(vtxcoll), bs , pt , eta , charge);
}



//--------------------------- IS THE GenPart GOOD ------------------
bool isGenPartGood(const MyGenPart& p){
  if ( fabs(p.charge) <=0 ) return false;
  if ( p.status != 1 )          return false;

  //Count only Stable Hadrons (and not the leptons) *** NOT LONGER NEEDED FOR MBUEWG
  //if(fabs(p.pdgId) == 11 )  return false;
  //if(fabs(p.pdgId) == 13 )  return false;
  //if(fabs(p.pdgId) == 15 )  return false;
  
  return true;
}


//---------------------- GET THE NUMBER OF PRIMARY GenPart --------------
int getnPrimaryGenPart(vector<MyGenPart>* v_tr , double pt = pt_cut, double eta = eta_cut, double charge = 0){
  
  int nch = 0;
  
  for(vector<MyGenPart>::iterator it_tr = v_tr->begin() ; it_tr != v_tr->end() ; ++it_tr)
    if( isGenPartGood(*it_tr) && isInAcceptance(*it_tr,pt,eta,charge) )
      ++nch;
      
  if(debug) cout<<" ** getnPrimaryTracks() has found "<<nch<<" primary tracks"<<endl;
  
  return nch;
}

//---------------------- GET THE PRIMARY GenPart --------------
void getPrimaryGenPart(vector<MyGenPart>& v_tr , double pt = pt_cut, double eta = eta_cut, double charge = 0){
  
  for(vector<MyGenPart>::iterator it_tr = v_tr.end()-1 ; it_tr != v_tr.begin()-1 ; --it_tr)
    if( !(isGenPartGood(*it_tr) && isInAcceptance(*it_tr,pt,eta,charge)) )
      v_tr.erase(it_tr);
  
  if(debug) cout<<" ** "<<v_tr.size()<<" GenPart remaining after primary selection"<<endl;
  
}


//---------------------- GET THE PRIMARY Jets --------------
void getPrimaryJets(vector<MyBaseJet>& v_jet, double pt = pt_cut, double eta = eta_cut, double charge = 0){

     for(vector<MyBaseJet>::iterator it_jet = v_jet.end()-1; it_jet != v_jet.begin()-1; --it_jet)
        if(!isInAcceptance(*it_jet, pt, eta, charge))
           v_jet.erase(it_jet);

     if(debug) cout<< " ** " << v_jet.size() << " Jets remaining after primary selection" << endl;

}


template<class T>
vector<T> getLeading(vector<T> coll)
{
    if (coll.size() == 0) return coll;

    double pt_max = 0;
    int pt_max_pointer = 0;
    int pt_max_pointer_temp = 0;

    for(typename vector<T>::iterator it_coll = coll.begin() ; it_coll != coll.end() ; ++it_coll, ++pt_max_pointer_temp)
    {
        if (it_coll->Pt() >= pt_max)
        {
           pt_max = it_coll->Pt();
           pt_max_pointer = pt_max_pointer_temp;
        }
        //if (coll.size() > 1) cout<< it_coll->Pt() << " ";
    }
    //if (coll.size() > 1) cout<< endl << coll.at(pt_max_pointer).Pt() << " "; 
    vector<T> coll_lead;
    coll_lead.push_back(coll.at(pt_max_pointer));    
    //if (coll.size() > 1) cout<< coll_lead.at(0).Pt() << endl;
    return coll_lead;
}

template<class T>
bool isLeadingMatched(vector<T> coll_base, vector<T> coll_assoc, double dR_cut)
{
    if ((coll_base.size() == 0) || (coll_assoc.size() == 0)) return false;

    vector<T> coll_base_lead = getLeading(coll_base);
    vector<T> coll_assoc_lead = getLeading(coll_assoc);

    double deta = coll_base_lead.at(0).Eta() - coll_assoc_lead.at(0).Eta();
    double dphi = coll_base_lead.at(0).Phi() - coll_assoc_lead.at(0).Phi();
    double dR = sqrt(deta*deta + dphi*dphi);

    if (dR <= dR_cut) return true;
    else return false;
}


template <class T>
pair<Double_t,Double_t> getnCorrel(vector<T>& v_tr , FBacc& cut){
  Int_t nTrM = 0 , nTrP = 0;
  for(typename vector<T>::iterator it_tr = v_tr.begin() ; it_tr != v_tr.end() ; ++it_tr){
    if( it_tr->Eta() < cut.etaM && it_tr->Eta() > cut.etaM - cut.widthM ) nTrM++;
    if( it_tr->Eta() > cut.etaP && it_tr->Eta() < cut.etaP + cut.widthP ) nTrP++;
  }
  if(debug) cout << "[getnCorrel] ntrM , nTrP : " << nTrM << "  " << nTrP << endl;
  return make_pair(nTrM , nTrP);
}


template <class T>
pair<Double_t,Double_t> getnCorrelTrReweight(vector<T>& v_tr , FBacc& cut , TH2F& trEff , TH2F& trFakes=0){
  Double_t nTrM = 0 , nTrP = 0;
  Int_t xbin = 0 , ybin = 0;
  Double_t weight = 1 , trEff_val = 1 , trFakes_val = 1;
  for(typename vector<T>::iterator it_tr = v_tr.begin() ; it_tr != v_tr.end() ; ++it_tr){
    xbin = trEff.GetXaxis()->FindFixBin( it_tr->Eta() ) ;
    ybin = trEff.GetYaxis()->FindFixBin( it_tr->Pt()  ) ;
    trEff_val = trEff.GetBinContent( xbin , ybin );
    trFakes_val =  1. - trFakes.GetBinContent( xbin , ybin );
    if(&trFakes==0) trFakes_val = 1;

    if( xbin<1 || ybin<1 || xbin>trEff.GetNbinsX() || ybin>trEff.GetNbinsY() || trEff_val == 0 ) weight = 1.;
    else					                                                 weight = trFakes_val / trEff_val;

    if(debug && weight > 2){
      cout << "[getnCorrelTrReweight] trEff_val , trFakes_val , weight : " << trEff_val << "  " << trFakes_val << "  " << weight << endl;
      cout << "[getnCorrelTrReweight] pt , eta                         : " << it_tr->Pt() << "  " << it_tr->Eta() << endl;
    }

    if( it_tr->Eta() < cut.etaM && it_tr->Eta() > cut.etaM - cut.widthM ) nTrM+=weight;
    if( it_tr->Eta() > cut.etaP && it_tr->Eta() < cut.etaP + cut.widthP ) nTrP+=weight;
  }
  if(debug) cout << "[getnCorrelTrReweight] ntrM , nTrP : " << nTrM << "  " << nTrP << endl;
  return make_pair(nTrM , nTrP);
}









