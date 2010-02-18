
double pt_cut = 0.2;
double eta_cut = 2.4;

//----------------- GET THE BEST VERTEX ------------------
int getBestVertex(vector<MyVertex>* vtxcoll){

  vector<MyVertex>::iterator goodVtx;
  int ntracks = -1;
  double chi2n=999;
  
  for(vector<MyVertex>::iterator itvtx=vtxcoll->begin();itvtx!=vtxcoll->end();++itvtx){
    if(itvtx->validity){
      if(itvtx->ntracks>ntracks){
        goodVtx  =  itvtx;
	ntracks  =  itvtx->ntracks;
	chi2n    =  itvtx->chi2n;
      }
      else if(itvtx->ntracks==ntracks){
        if(chi2n>itvtx->chi2n){
	  goodVtx  =  itvtx;
	  ntracks  =  itvtx->ntracks;
	  chi2n    =  itvtx->chi2n;
	}
      }
    }
  }
  
  if(ntracks<=0)
    return -1;
  
  return goodVtx->id;
}

int getVtxposFromId(MyTracks& tr,int goodvtxId){
  for(unsigned int i=0;i<tr.vtxid.size();++i)
    if(tr.vtxid.at(i)==goodvtxId)
      return i;
  
  return -1;
}




//------------------ UE SELECTION ----------------------------------------
bool isTrackPrimary(MyTracks& tr, int goodvtxId){

  if(debug) cout<<" +-+-+ starting isTrackPrimary"<<endl;
  
  //get the number of the good vertex in the vector
  int vtxnum = getVtxposFromId(tr,goodvtxId);
  
  //Make the cuts:
  if( tr.Part.v.Pt()<pt_cut ) return false;
  if( fabs(tr.Part.v.Eta())>eta_cut) return false;
  
  //--------- PV track cut from Luca -----------
  if( fabs(tr.vtxdxy.at(vtxnum)) / tr.ed0 > 5 ) return false;
  if( fabs(tr.vtxdz.at(vtxnum)) / tr.edz > 5 ) return false;
  if( tr.ept / tr.Part.v.Pt() > 0.1 ) return false;
  
  //To use only with generalTracks, otherwise it's filled to 0 by default
  //if( ! tr.quality[2] && trackcoll=="generalTracks") return false;
  
  
  if(debug) cout<<" ** The track is primary"<<endl;
  
  return true;
}




//------------------ FERENC SELECTION ----------------------------------------
bool isTrackPrimary(MyTracks& tr, vector<MyVertex>& vtxcoll, int goodvtxId, MyBeamSpot* bs){

  if(debug) cout<<" +-+-+ starting isTrackPrimary"<<endl;
  
  //get the number of the good vertex in the vector
  int vtxnum = getVtxposFromId(tr,goodvtxId);
  
  //get the good vertex iterator
  vector<MyVertex>::iterator goodVtx = vtxcoll.end();
  for(vector<MyVertex>::iterator it_vtx = vtxcoll.begin();it_vtx != vtxcoll.end();++it_vtx)
    if(goodvtxId==it_vtx->id)
      goodVtx = it_vtx;
  
  //Make the cuts:
  if( tr.Part.v.Pt()<pt_cut ) return false;
  if( fabs(tr.Part.v.Eta())>eta_cut) return false;
  if( tr.ept / tr.Part.v.Pt() > 0.1 ) return false;
  
  
  //--------- PV track cut from Ferenc -----------
  double sigma_t = sqrt( pow(tr.ed0,2) +
                   bs->BeamWidthX *
                   bs->BeamWidthY );
  if( fabs(tr.vtxdxy.at(0)) > min(0.2 , 4 * sigma_t) ) return false;
  
  
  // Longitudinal impact parameter (4*sigma)
  double sigma_z = sqrt( pow(tr.edz,2) +
                    pow(cosh(tr.Part.v.Eta()),2) *
                    bs->BeamWidthX *
                    bs->BeamWidthY);
  //if( fabs(tr.vtxdz.at(vtxnum)) > 4 * sigma_z ) return false; // FIXME
  if( fabs(tr.vz - goodVtx->z) > 4 * sigma_z ) return false; // FIXME

  
  //To use only with generalTracks, otherwise it's filled to 0 by default
  //if( ! tr.quality[2] && trackcoll=="generalTracks") return false;
  
  
  if(debug) cout<<" ** The track is primary"<<endl;
  
  return true;
}


//---------------------- GET THE NUMBER OF PRIMARY TRACKS --------------
int getnPrimaryTracks(vector<MyTracks>* v_tr , vector<MyVertex>* vtxcoll){
  
  if(debug) cout<<" +-+-+ starting getnPrimaryTracks"<<endl;
  
  int nch = 0;
  
  //Protecting code if no vertex were found
  if(getBestVertex(vtxcoll)==-1) return 0;
  
  
  for(vector<MyTracks>::iterator it_tr = v_tr->begin() ; it_tr != v_tr->end() ; ++it_tr)
    if( isTrackPrimary(*it_tr , getBestVertex(vtxcoll)) )
      ++nch;
      
  if(debug) cout<<" ** getnPrimaryTracks() has found "<<nch<<" primary tracks"<<endl;
  
  return nch;
}

/*void getPrimaryTracks(vector<MyTracks>* v_tr , vector<MyVertex>* vtxcoll){

  if(debug) cout<<" +-+-+ starting getPrimaryTracks"<<endl;
  
  //Protecting code if no vertex were found
  if(getBestVertex(vtxcoll)==-1) v_tr->clear();
  
  
  for(vector<MyTracks>::iterator it_tr = v_tr->begin() ; it_tr != v_tr->end() ; ++it_tr)
    if( ! isTrackPrimary(*it_tr , getBestVertex(vtxcoll)) )
      v_tr->erase(it_tr--);
      
  if(debug) cout<<" ** "<<v_tr->size()<<" tracks remaining after primary selection"<<endl;
}*/


//------------------------------ GET THE PRIMARY TRACK -----------------------------------------
vector<MyTracks> getPrimaryTracks(vector<MyTracks> v_tr , int vtxId){

  if(debug) cout<<" +-+-+ starting getPrimaryTracks"<<endl;
  
  //Protecting code if no vertex were found
  if(vtxId==-1) v_tr.clear();
  
  
  for(vector<MyTracks>::iterator it_tr = v_tr.begin() ; it_tr != v_tr.end() ; ++it_tr)
    if( ! isTrackPrimary(*it_tr , vtxId) )
      v_tr.erase(it_tr--);
      
  if(debug) cout<<" ** "<<v_tr.size()<<" tracks remaining after primary selection"<<endl;
  
  return v_tr;
}

vector<MyTracks> getPrimaryTracks(vector<MyTracks> v_tr , vector<MyVertex>* vtxcoll){
  return getPrimaryTracks(v_tr , getBestVertex(vtxcoll) );
}



vector<MyTracks> getPrimaryTracks(vector<MyTracks> v_tr , vector<MyVertex>* vtxcoll, int vtxId, MyBeamSpot* bs){

  if(debug) cout<<" +-+-+ starting getPrimaryTracks using Ferenc association"<<endl;
  
  //Protecting code if no vertex were found
  if(vtxId==-1) v_tr.clear();
  
  
  for(vector<MyTracks>::iterator it_tr = v_tr.begin() ; it_tr != v_tr.end() ; ++it_tr)
    if( ! isTrackPrimary(*it_tr , *vtxcoll, vtxId, bs ) )
      v_tr.erase(it_tr--);
      
  if(debug) cout<<" ** "<<v_tr.size()<<" tracks remaining after primary selection"<<endl;
  
  return v_tr;
}

vector<MyTracks> getPrimaryTracks(vector<MyTracks> v_tr , vector<MyVertex>* vtxcoll, MyBeamSpot* bs){
  return getPrimaryTracks( v_tr, vtxcoll, getBestVertex(vtxcoll), bs );
}
