//#################################################
//#
//#  macro to get the event selection. There are
//#  6 possible functions returning a bool:
//#
//#  passBit40    (*MyL1Trig)
//#  passL1       (*MyL1Trig)
//#  passHF       (*MyMITEvtSel)
//#  passVtxQual  (*MyMITEvtSel)
//#  passVtx      (vector<MyVertex>*)
//#
//#  isEvtGood    (*MyL1Trig , *MyMITEvtSel , vector<MyVertex>*)
//#
//#  goodBX       (irun,bx)
//#
//#  isSD       (MyGenKin)
//#  isDD       (MyGenKin)
//#
//#
//#################################################


//-------- L1 TRIGGER CUT --------
bool passL1(MyL1Trig& L1Trig){
  if(   !L1Trig.TechTrigWord[36]
     && !L1Trig.TechTrigWord[37]
     && !L1Trig.TechTrigWord[38]
     && !L1Trig.TechTrigWord[39]
     && L1Trig.TechTrigWord[34]
     && (L1Trig.TechTrigWord[0] || isMC) )
  //   && L1Trig.TechTrigWord[40])
  {
    return true;
  }
  return false;
}

//-------- Bit40 TRIGGER CUT --------
bool passBit40(MyL1Trig& L1Trig){
  if( L1Trig.TechTrigWord[40] )
    return true;
    
  return false;
}
  
//------- HF CUT ----------
bool passHF(MyMITEvtSel& evtSel){
  double hfEnergyMin = min(evtSel.eHfPos,evtSel.eHfNeg);
  if(  evtSel.nHfTowersP >= 1
    && evtSel.nHfTowersN >= 1
    && hfEnergyMin >= 3. )
  {
    return true;
  }
  return false;
}

//----------- VERTEX QUALITY CUT ---------
bool passVtxQual(MyMITEvtSel& evtSel){
  // construct polynomial cut on cluster vertex quality vs. npixelhits
  double polyCut = 0.5 + 0.0045 * (evtSel.ePxHits);
  
  if((evtSel.ePxHits) < 150) 
    polyCut=0;             // don't use cut below nhitsTrunc_ pixel hits
    
  if( evtSel.eClusVtxQual >= polyCut )
    return true;
    
  return false;
}    

bool passVtx(vector<MyVertex>* vtxcoll){
  if(getBestVertex(vtxcoll)==-1) return false;
  else return true;
}

//----------- GLOBAL CUT ---------
bool isEvtGoodNoVtx(MyL1Trig& L1Trig, MyMITEvtSel& evtSel){
  if(  passL1(L1Trig)
    && passHF(evtSel)
    && passVtxQual(evtSel)
    )
      return true;

   return false;
}


//----------- GLOBAL CUT ---------
bool isEvtGood(MyL1Trig& L1Trig, MyMITEvtSel& evtSel, vector<MyVertex>* vtxcoll){
  if(  passL1(L1Trig)
    && passHF(evtSel)
    && passVtxQual(evtSel)
    && passVtx(vtxcoll) )
      return true;
   
   return false;
}

//----------- BX CUT ---------
bool goodBX(int irun,int bx){

  bool accepted = false;
  int type_=1;  //1 collision 2 non collision
  if (irun==123592 || irun==123596 || irun==123615) {
    if (type_==1) {
      if ((bx==51) || (bx==2724))
        accepted=true;
    }else if (type_==2) {
      if ((bx==2276) || (bx==3170))
        accepted=true;
    }
  }

  else if (irun==123732) {
    if (type_==1) {
      if ((bx==3487) || (bx==2596))
        accepted=true;
    } else if (type_==2) {
      if ((bx==2148) || (bx==3042))
        accepted=true;
    }
  }

else if (irun==123815 || irun==123818) {
    if (type_==1) {
      if ((bx==2724) || (bx==51))
        accepted=true;
    } else if (type_==2) {
      if ((bx==2276) || (bx==3170))
        accepted=true;
    }
  }

else if (irun==123906 || irun==123908) {
    if (type_==1) {
      if ((bx==51))
        accepted=true;
    } else if (type_==2) {
      if ((bx==2276) || (bx==2724) || (bx==1830) || (bx==1833))
        accepted=true;
    }
  }

  else if (irun==124006 || irun==124008 || irun==124009 || irun==124020 || irun==124022 || irun==124023 || irun==124024 || irun==124025 ||irun==124027 || irun==124030 ) {
    if (type_==1) {
      if ((bx==2824) || (bx==151) || (bx==51))
        accepted=true;
    } else if (type_==2) {
      if ((bx==1365) || (bx==474) ||(bx==2259) || (bx==1930) )
        accepted=true;
    }
  }
  
  else if (irun==124120) {
    if (type_==1) {
      if ((bx==51))
        accepted=true;
    } else if (type_==2) {
      if ((bx==1836) || (bx==942))
        accepted=true;
    }
  }

   
 else if (irun==124230) {
    if (type_==1) {
      if (bx==51 || (bx==151) || bx==232 || bx==1042 || bx==1123 || bx==1933 || bx==2014 || bx==2824 || bx==2905)
        accepted=true;
    } else if (type_==2) {
      if (bx==13)
        accepted=true;
    }
  }

  return accepted;

}

bool isSD(MyGenKin* genKin){
  if(genKin->MCProcId == 92 || genKin->MCProcId == 93)
    return true;
  return false;
}

bool isDD(MyGenKin* genKin){
  if(genKin->MCProcId == 94)
    return true;
  return false;
}


