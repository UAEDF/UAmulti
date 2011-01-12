#include "NCHHFPlots.h"

ClassImp(NCHHFPlots)

//_____________________________________________________________________________
NCHHFPlots::NCHHFPlots(){
  hfcoll = "none";
  this->init();
}

//_____________________________________________________________________________
NCHHFPlots::NCHHFPlots(TString collname){
  hfcoll = collname;
  this->init();
}

//_____________________________________________________________________________
NCHHFPlots::~NCHHFPlots(){ 
    delete evtsel_HF0;
    delete evtsel_HF1;
    if(switchAllHF){
        delete evtsel_HF2;
        delete evtsel_HF3;
        delete evtsel_HF4;
        delete evtsel_HF5;
        delete evtsel_HF6;    
    }
}

//_____________________________________________________________________________
//HF0 means cut off, so we always fill
bool NCHHFPlots::passHF1=0;
//passHF2=!passHF1
bool NCHHFPlots::passHF3=0;
bool NCHHFPlots::passHF4=0;
bool NCHHFPlots::passHF5=0;
bool NCHHFPlots::passHF6=0;

bool NCHHFPlots::switchAllHF=0;


//_____________________________________________________________________________
void NCHHFPlots::init(){

  evtsel_HF0               = new NCHEvtSelPlots("HF0_"+hfcoll);
  evtsel_HF1               = new NCHEvtSelPlots("HF1_"+hfcoll);
  if(switchAllHF){
     evtsel_HF2            = new NCHEvtSelPlots("HF2_"+hfcoll);
     evtsel_HF3            = new NCHEvtSelPlots("HF3_"+hfcoll);
     evtsel_HF4            = new NCHEvtSelPlots("HF4_"+hfcoll);
     evtsel_HF5            = new NCHEvtSelPlots("HF5_"+hfcoll);
     evtsel_HF6            = new NCHEvtSelPlots("HF6_"+hfcoll);
  }
 
}

//_____________________________________________________________________________
void NCHHFPlots::fill(vector<MyGenPart>& gpcoll, vector<MyTracks>& trcoll, vector<MyVertex>& vtxcoll, vector<MyVertex>::iterator vtxIter, int vtxId, MyBeamSpot* bs, double weight){
    
  NCHEvtSelPlots::passHF=1;  //You always pass HF0, it is means cut off..
      evtsel_HF0->fill(gpcoll,trcoll,vtxcoll,vtxIter,vtxId,bs,weight);        
  NCHEvtSelPlots::passHF=passHF1; 
      evtsel_HF1->fill(gpcoll,trcoll,vtxcoll,vtxIter,vtxId,bs,weight);  
  if(switchAllHF){
    NCHEvtSelPlots::passHF=!passHF1;  //inverse of HF1,   the events which don't pass through HF1...
        evtsel_HF2->fill(gpcoll,trcoll,vtxcoll,vtxIter,vtxId,bs,weight);  
    NCHEvtSelPlots::passHF=passHF3;
        evtsel_HF3->fill(gpcoll,trcoll,vtxcoll,vtxIter,vtxId,bs,weight);
    NCHEvtSelPlots::passHF=passHF4;
        evtsel_HF4->fill(gpcoll,trcoll,vtxcoll,vtxIter,vtxId,bs,weight);
    NCHEvtSelPlots::passHF=passHF5;
        evtsel_HF5->fill(gpcoll,trcoll,vtxcoll,vtxIter,vtxId,bs,weight);
    NCHEvtSelPlots::passHF=passHF6;
        evtsel_HF6->fill(gpcoll,trcoll,vtxcoll,vtxIter,vtxId,bs,weight);
  }
  
}

//_____________________________________________________________________________
void NCHHFPlots::write(){ 
  gDirectory->mkdir("HFlvl_"+hfcoll);
  gDirectory->cd("HFlvl_"+hfcoll);
   
  evtsel_HF0->write();
  evtsel_HF1->write();
  if(switchAllHF){  
    evtsel_HF2->write();  
    evtsel_HF3->write();
    evtsel_HF4->write();
    evtsel_HF5->write();
    evtsel_HF6->write();
  }  
  
  gDirectory->cd("../");
}

