//###########################################################
//#
//#  macro to get the cuts. 
//*  
//*  5 constant doubles are needed:
//*
//*  const double ptReco_cut    
//*  const double eta_cut   
//*  const double charge_cut
//*  const double nsigma    
//*  const double vtxz_cut  
//#
//*  
//#  Functions:
//#  
//#  int                      getBestVertex     (vector<MyVertex>* vtxcoll)
//#  inline int               getVtxposFromId   (const MyTracks& tr, int goodvtxId)
//#  inline bool              isInAcceptance    (const MyPart* p, double pt, double etaMin, double etaMax, double charge)
//#  bool                     isTrackPrimary    (const MyTracks& tr, int goodvtxId)
//#  bool                     isTrackPrimary    (const MyTracks& tr, vector<MyVertex>& vtxcoll, int goodvtxId, const MyBeamSpot* bs)  
//#  vector<MyTracks>         getPrimaryTracks  (vector<MyTracks> v_tr, vector<MyVertex>* vtxcoll, int vtxId, MyBeamSpot* bs, double pt, double etaMin, double etaMax, double charge)
//#  inline vector<MyTracks>  getPrimaryTracks  (vector<MyTracks> v_tr, vector<MyVertex>* vtxcoll, MyBeamSpot* bs, double pt, double etaMin, double etaMax, double charge)
//#  inline vector<MyTracks>  getPrimaryTracks  (vector<MyTracks> v_tr, vector<MyVertex>* vtxcoll, double pt, double etaMin, double etaMax, double charge)
//#  vector<MyGenPart>        getPrimaryGenPart (vector<MyGenPart> v_tr, double pt, double etaMin, double etaMax, double charge)
//# 
//*
//###########################################################

//default acceptance is overwritten by the accMap  
const double charge_cut     =  0. ;
//generated
const double ptGen_cut      =  0.;
const double etaGenMin_cut  =  0.;
const double etaGenMax_cut  =  2.4;
//recostructed
const double ptReco_cut     =  0.1; 
const double etaRecoMin_cut =  0.;
const double etaRecoMax_cut =  2.4;

//don't change
const double nsigma     =  4. ;
const double vtxz_cut   = 15. ;
  
  

//----------------- GET THE BEST VERTEX ----------------------------------------
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


//----------------------------------------------------------------------------------------
inline int getVtxposFromId(const MyTracks& tr, int goodvtxId){
  for(unsigned int i=0;i<tr.vtxid.size();++i)
    if(tr.vtxid.at(i)==goodvtxId)
      return i;
  
  return -1;
}


//--------------------------- Get the Part in the good acceptance -----------------------------
inline bool isInAcceptance(const MyPart* p , double pt = ptReco_cut, double etaMin = etaRecoMin_cut, double etaMax = etaRecoMax_cut, double charge = charge_cut){
  if(p->Pt()<pt) return false;
  if( fabs(p->Eta())<=etaMin || fabs(p->Eta())>etaMax ) return false;
  if(charge!=0)  
    if(charge!=p->charge)return false; 
  return true;
}


//------------------ UE SELECTION ----------------------------------------
bool isTrackPrimary(const MyTracks& tr, int goodvtxId){

  if(debug) cout<<" +-+-+ starting isTrackPrimary"<<endl;
  
  //get the number of the good vertex in the vector
  int vtxnum = getVtxposFromId(tr,goodvtxId);
  
  //Make the cuts:
  //if( tr.Part.v.Pt()<ptReco_cut ) return false;
  //if( fabs(tr.Part.v.Eta())>eta_cut) return false;
  
  //--------- PV track cut from Luca -----------
  if( fabs(tr.vtxdxy.at(vtxnum)) / tr.ed0 > 5 ) return false;
  if( fabs(tr.vtxdz.at(vtxnum)) / tr.edz > 5 ) return false;
  if( tr.ept / tr.Pt() > 0.05 ) return false; //0.1
  
  //To use only with generalTracks, otherwise it's filled to 0 by default
  //if( ! tr.quality[2]) return false;
  
  
  if(debug) cout<<" ** The track is primary" <<endl;
  
  return true;
}


//------------------ FERENC SELECTION ----------------------------------------
bool isTrackPrimary(const MyTracks& tr, vector<MyVertex>& vtxcoll, int goodvtxId, const MyBeamSpot* bs){

  if(debug) cout<<" +-+-+ starting isTrackPrimary"<<endl;
  
  //get the number of the good vertex in the vector
  //int vtxnum = getVtxposFromId(tr,goodvtxId);
  
  //get the good vertex iterator
  vector<MyVertex>::iterator goodVtx = vtxcoll.end();
  for(vector<MyVertex>::iterator it_vtx = vtxcoll.begin();it_vtx != vtxcoll.end();++it_vtx)
    if(goodvtxId==it_vtx->id)
      goodVtx = it_vtx;
  
  //Make the cuts:
  //if( tr.Part.v.Pt()<ptReco_cut ) return false;
  //if( fabs(tr.Part.v.Eta())>eta_cut) return false;
  if( tr.ept / tr.Pt() > 0.05 ) return false;  //0.1
  
  
  //--------- PV track cut from Ferenc -----------
  double sigma_t = sqrt( pow(tr.ed0,2) +
                   bs->BeamWidthX *
                   bs->BeamWidthY );
  if( fabs(tr.vtxdxy.at(0)) > min(0.2 , nsigma * sigma_t) ) return false;
  
  
  // Longitudinal impact parameter (4*sigma)erator!= not defined for vector<MyGenPart,allocator<MyGenPart> >::iterator NCHCuts.C:223
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



//_____________________________________________________________________________
vector<MyTracks> getPrimaryTracks(vector<MyTracks> v_tr , vector<MyVertex>* vtxcoll, int vtxId, MyBeamSpot* bs, double pt = ptReco_cut, 
                                                                        double etaMin =  etaRecoMin_cut, double etaMax =  etaRecoMax_cut, double charge = charge_cut){

  if(debug) cout << " +-+-+ starting getPrimaryTracks using Ferenc association" << endl;
  
  //Protecting code if no vertex were found
  if(vtxId==-1) v_tr.clear();
  
  //going from the back is faster    Watch the iterators !
  for(vector<MyTracks>::iterator it_tr = v_tr.end()-1 ; it_tr != v_tr.begin()-1 ; --it_tr)
    if( ! isTrackPrimary(*it_tr , *vtxcoll, vtxId, bs ) || !isInAcceptance(&*it_tr,pt,etaMin,etaMax,charge) )
       v_tr.erase(it_tr);
   
   /* OLD Structure
    for(vector<MyTracks>::iterator it_tr = v_tr.begin() ; it_tr != v_tr.end() ; ++it_tr)
      if( ! isTrackPrimary(*it_tr , *vtxcoll, vtxId, bs erator!= not defined for vector<MyGenPart,allocator<MyGenPart> >::iterator NCHCuts.C:223) || !isInAcceptance(it_tr->Part,pt,eta,charge) )
      {
          cout << it_tr->Part.v.Pt()<<endl;
          v_tr.erase(it_tr--);
      }
    */
  if(debug) cout<<" ** "<<v_tr.size()<<" tracks remaining after primary selection"<<endl;
  
  return v_tr;
}


//_____________________________________________________________________________
inline vector<MyTracks> getPrimaryTracks(vector<MyTracks> v_tr , vector<MyVertex>* vtxcoll, MyBeamSpot* bs, double pt = ptReco_cut,
                                                        double etaMin =  etaRecoMin_cut, double etaMax =  etaRecoMax_cut, double charge = charge_cut){
  return getPrimaryTracks( v_tr, vtxcoll, getBestVertex(vtxcoll), bs, pt, etaMin, etaMax, charge);
}


//------------------------------ GET THE PRIMARY TRACK -----------------------------------------
vector<MyTracks> getPrimaryTracks(vector<MyTracks> v_tr, int vtxId, double pt = ptReco_cut, double etaMin = etaRecoMin_cut, double etaMax = etaRecoMax_cut, double charge = charge_cut){

  if(debug) cout << " +-+-+ starting getPrimaryTracks" << endl;
  
  //Protecting code if no vertex were found
  if(vtxId==-1) v_tr.clear();
 
  //going from the back is faster    Watch the iterators !   
  for(vector<MyTracks>::iterator it_tr = v_tr.end()-1 ; it_tr != v_tr.begin()-1 ; --it_tr)
    if( ! isTrackPrimary(*it_tr , vtxId) || !isInAcceptance(&*it_tr,pt,etaMin,etaMax,charge))
      v_tr.erase(it_tr);
      
  if(debug) cout << " ** " << v_tr.size() << " tracks remaining after primary selection" << endl;
  
  return v_tr;
}


//_____________________________________________________________________________
inline vector<MyTracks> getPrimaryTracks(vector<MyTracks> v_tr , vector<MyVertex>* vtxcoll , double pt = ptReco_cut, double etaMin = etaRecoMin_cut, double etaMax = etaRecoMax_cut, double charge = charge_cut){
  return getPrimaryTracks(v_tr, getBestVertex(vtxcoll), pt, etaMin, etaMax, charge );
}


//---------------------- GET THE NUMBER OF PRIMARY GenPart --------------
vector<MyGenPart> getPrimaryGenPart(vector<MyGenPart> v_tr , double pt = ptGen_cut,double etaMin = etaGenMin_cut, double etaMax = etaGenMax_cut, double charge = charge_cut){
   
  if(debug) cout <<"  +-+-+ starting getPrimaryGenPart" << endl;
  
 for(vector<MyGenPart>::iterator it_tr = v_tr.end()-1 ; it_tr != v_tr.begin()-1 ; --it_tr) {
    if( !isInAcceptance(&*it_tr,pt,etaMin,etaMax,charge) )
      v_tr.erase(it_tr);
 }     
 
  if(debug) cout <<" ** "<< v_tr.size() <<" genPart remaining after primary selection"<<endl;

  return v_tr;
}

