//#################################################
//#
//#  macro to get the event selection. There are
//#  6 possible functions returning a bool:
//#
//#  passCentral  (*goodtracks  or *goodgenpart, TString, pt_cut)
//#  passBit40    (*MyL1Trig)
//#  //passL1       (Energy, *MyL1Trig)
//#  passL1HLT    (double, MyL1Trig&, MyHLTrig&, int )
//#  passHF       (*MyMITEvtSel)
//#  passHF       (const MyMITEvtSel& evtSel, int turn)
//#  passVtxQual  (*MyMITEvtSel,Energy)
//#  passVtx      (vector<MyVertex>*)

//#
//#  //beware, needs evtSelType to be defined
//#  isEvtGood       (Energy, *MyL1Trig , *MyMITEvtSel , vector<MyVertex>*)
//#  isEvtGoodNoVtx  (Energy, *MyL1Trig, *MyMITEvtSel )
//#
//#  //#
//#  isSD       (MyGenKin , MCtype)
//#  isDD       (MyGenKin , MCtype)
//#
//#
//#################################################

/*
//-----------------------------------------------------------------------------
bool goodBX(int irun, int bx){
  return true;
}
*/

//_____________________________________________________________________________
template <class T>
bool passCentral(vector<T>& coll, const TString evtsel, const double pt_cut = 0.5){
  //WARNING:
  //EVENTLEVEL CUT: because you need at least 1 particle with eta<.8 and Pt>.5 
  int nch_cut=1;
  double eta_cut=0.8;
  
  if(evtsel == "MBUEWG") {
    nch_cut=1;
    eta_cut=0.8;
  }
  else if (evtsel =="ALICE"){
    nch_cut=1;
    eta_cut=1.;
  } 
  else if (evtsel =="ATLAS1"){
    nch_cut=1;
    eta_cut=2.4;
  }  
  else if (evtsel =="ATLAS2"){
    nch_cut=2;
    eta_cut=2.4;
  }
  else if (evtsel =="ATLAS6"){
    nch_cut=6;
    eta_cut=2.4;
  }
  else {
    cout << "------------------------------------" << endl;
    cout << "Event selection type does not exist." <<endl;
    cout << "------------------------------------" << endl;
  }
  
  int count=0;
  for(typename vector<T>::const_iterator gp = coll.begin() ; gp != coll.end() ; ++gp){
          if ( fabs(gp->Part.v.Eta()) < eta_cut && gp->Part.v.Pt() > pt_cut ) {
              count++; 
              if(count==nch_cut) return true;
          }         
  }
  return false;
}


//-------- L1 TRIGGER CUT ------------------------------------------------------
inline bool passL1(const double Energy, const MyL1Trig& L1Trig, const bool ismc){

  if ( Energy == 0.9 || Energy == 2.36 ) {
    if(   !L1Trig.TechTrigWord[36]
       && !L1Trig.TechTrigWord[37]
       && !L1Trig.TechTrigWord[38]
       && !L1Trig.TechTrigWord[39]
       && L1Trig.TechTrigWord[34]
       && (L1Trig.TechTrigWord[0] || ismc) )
  //   && L1Trig.TechTrigWord[40])
    {
      return true;
    }
  }
  else if ( Energy == 7.0 ) { 
    if(   !L1Trig.TechTrigWord[36]
       && !L1Trig.TechTrigWord[37]
       && !L1Trig.TechTrigWord[38]
       && !L1Trig.TechTrigWord[39]
       && ((L1Trig.PhysTrigWord[124] && !ismc) || (L1Trig.TechTrigWord[34] && ismc))
       && (L1Trig.TechTrigWord[0] || ismc) )
    {
      return true;
    }
  }
  return false;
}


//-------- L1+HLT TRIGGER CUT ----------------------------------------------------
bool passL1HLT(double Energy, MyL1Trig& L1Trig, MyHLTrig& HLTrig , int iTrigCond, const bool ismc){
// iTrigCond = 1   bit[124]  +  HLT
//           = 2   bit[63]   +  HLT
//           = 3   OR(1,2)

   if ( Energy == 0.9 || Energy == 2.36 ) {
     if(   !L1Trig.TechTrigWord[36]
        && !L1Trig.TechTrigWord[37]
        && !L1Trig.TechTrigWord[38]
        && !L1Trig.TechTrigWord[39]
        && L1Trig.TechTrigWord[34]
        && (L1Trig.TechTrigWord[0] || ismc) ) 
     {
        return true;
     }
   }
   else if ( Energy == 7.0 ) {
     if(   !L1Trig.TechTrigWord[36]
        && !L1Trig.TechTrigWord[37]
        && !L1Trig.TechTrigWord[38]
        && !L1Trig.TechTrigWord[39]
        // && ((L1Trig.PhysTrigWord[124] && !isMC) || (L1Trig.TechTrigWord[34] && isMC))
        && (L1Trig.TechTrigWord[0] || ismc) )
     {
        bool L1HLT_BSC = ( ((L1Trig.PhysTrigWord[124] && !ismc) || (L1Trig.TechTrigWord[34] && ismc))
                           && HLTrig.HLTmap["HLT_L1_BscMinBiasOR_BptxPlusORMinus"] );
        bool L1HLT_M70 = (    L1Trig.PhysTrigWord[63] && HLTrig.HLTmap["HLT_PixelTracks_Multiplicity70"] ) ;
        bool L1HLT_M85 = (    L1Trig.PhysTrigWord[63] && HLTrig.HLTmap["HLT_PixelTracks_Multiplicity85"] ) ;
        bool L1HLT_M40 = (    L1Trig.PhysTrigWord[63] && HLTrig.HLTmap["HLT_PixelTracks_Multiplicity40"] ) ;
        //bool L1HLT_ST  = (   L1Trig.PhysTrigWord[124] && HLTrig.HLTmap["HLT_MinBiasPixel_SingleTrack"] ) ;
        if ( iTrigCond == 1 ) return L1HLT_BSC ;
        if ( iTrigCond == 2 ) return ( L1HLT_M70 || L1HLT_M85 || L1HLT_M40) ;
        if ( iTrigCond == 3 ) return ( L1HLT_BSC || L1HLT_M70 || L1HLT_M85 || L1HLT_M40) ;
        
     }
   }
   return false;
}


//-------- Bit40 TRIGGER CUT -------------------------------------------
inline bool passBit40(const MyL1Trig& L1Trig){
  return ((L1Trig.TechTrigWord[40])==1 ? true : false);
}


//------- HF CUT --------------------------------------------------------
inline bool passHF(const MyMITEvtSel& evtSel, const int turn){
  switch (turn) {
    case 1:
        if(  evtSel.nHfTowersP >= 1
           &&evtSel.nHfTowersN >= 1
           &&min(evtSel.eHfPos,evtSel.eHfNeg) >= 3. )
             return true;
        else return false;
    break;
        
    //case '2':  Not needed because case2 = !case1         
     
    case 3:  //PGapOnly
        if( evtSel.eHfPos < 3. 
          &&evtSel.nHfTowersN>=1
          &&evtSel.eHfNeg > 3. ) 
            return true;        
        else return false;
    break;
    
    case 4: //NGapOnly
        if( evtSel.eHfNeg < 3. 
          &&evtSel.nHfTowersP>=1
          &&evtSel.eHfPos > 3. ) 
            return true;        
        else return false;
    break;
      
    case 5: //Two Gaps
        if(  evtSel.eHfPos < 3. 
           &&evtSel.eHfNeg < 3. ) 
            return true;         
         else return false;
    break;
         
    case 6: //PGapOnly or NGapOnly
        if( (evtSel.eHfPos < 3. &&evtSel.nHfTowersN>=1 &&evtSel.eHfNeg > 3. )
          ||(evtSel.eHfNeg < 3. &&evtSel.nHfTowersP>=1 &&evtSel.eHfPos > 3. ) ) 
            return true;        
        else return false;
    break;
    
    // not used
    default: 
        return false;
    break;    
  }
}  
 

//----------- VERTEX QUALITY CUT -----------------------------------------
bool passVtxQual(const MyMITEvtSel& evtSel , const double Energy){
  // construct polynomial cut on cluster vertex quality vs. npixelhits

  double polyCut = 0.;
  if(Energy == 0.9)                   polyCut = 0.5  + 0.0045 * (evtSel.ePxHits);
  if(Energy == 7.0 || Energy == 2.36) polyCut = 0.   + 0.0045 * (evtSel.ePxHits);
  
  if((evtSel.ePxHits) < 150) 
     polyCut=0;             // don't use cut below nhitsTrunc_ pixel hits

  double clusterTrunc = 2.; 
  if( (Energy == 7.0 || Energy == 2.36) && polyCut > clusterTrunc && clusterTrunc > 0) 
     polyCut=clusterTrunc; 
    
  return (evtSel.eClusVtxQual >= polyCut ? true : false );
}


//-----------------------------------------------------------------------------
inline bool passVtx(vector<MyVertex>& vtxcoll){
    return (getBestVertex(&vtxcoll)==-1 ? false : true );
}



//-----------------------------------------------------------------------------
bool isEvtGood(double Energy , MyL1Trig& L1Trig , MyHLTrig& HLTrig , MyMITEvtSel& MITEvtSel, vector<MyVertex>* vtxcoll){
  if(  passL1HLT(Energy , L1Trig, HLTrig , 1,isMC )
    && passHF(MITEvtSel , 1)
    && passVtxQual(MITEvtSel , Energy)
    && passVtx(*vtxcoll) )
          return true;
	  
  return false;
}



//_____________________________________________________________________________
inline bool isSD(const MyGenKin& genKin , const TString MCtype){
  if(MCtype == "pythia" && (genKin.MCProcId == 92 || genKin.MCProcId == 93))
    return true;
  if(MCtype == "pythia8" && (genKin.MCProcId == 103 || genKin.MCProcId == 104))
    return true;
  if(MCtype == "phojet" && (genKin.MCProcId == 5 || genKin.MCProcId == 6))
    return true;
  return false;
}


//_____________________________________________________________________________
inline bool isDD(const MyGenKin& genKin , const TString MCtype){
  if(MCtype == "pythia" && genKin.MCProcId == 94)
    return true;
  if(MCtype == "pythia8" && (genKin.MCProcId == 105))
    return true;
  if(MCtype == "phojet" && (genKin.MCProcId == 4 || genKin.MCProcId == 7)) // 4 = DPE , 6 = DD
    return true;
  return false;
}
