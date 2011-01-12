#include "NCHDiffPlots.h"

ClassImp(NCHDiffPlots)

//_____________________________________________________________________________
NCHDiffPlots::NCHDiffPlots(){
  diffcoll = "none";
  this->init();
}

//_____________________________________________________________________________
NCHDiffPlots::NCHDiffPlots(TString collname){
  diffcoll = collname;
  this->init();
}

//_____________________________________________________________________________
NCHDiffPlots::~NCHDiffPlots(){ 
    if(isMC){   
        delete centralp_SD;
        delete centralp_DD;
        delete centralp_NSD;
        delete centralp_INEL;
    }
    delete centralp_RECO;
}

//_____________________________________________________________________________
bool NCHDiffPlots::isMC=0; 
bool NCHDiffPlots::isSD=0;
bool NCHDiffPlots::isDD=0;


//_____________________________________________________________________________
void NCHDiffPlots::init(){
 
  NCHEvtSelPlots::isMC=isMC;
 
  if(isMC) {
     centralp_SD             = new NCHCentralPlots("SD_"+diffcoll);
     centralp_DD             = new NCHCentralPlots("DD_"+diffcoll);
     centralp_NSD            = new NCHCentralPlots("NSD_"+diffcoll);
     centralp_INEL           = new NCHCentralPlots("INEL_"+diffcoll);
   }

   centralp_RECO             = new NCHCentralPlots("RECO_"+diffcoll, 1); //1 means it is not MC, see NCHCentralPlots constructor
 
}

//_____________________________________________________________________________
void NCHDiffPlots::fill(vector<MyGenPart>& gpcoll, MyGenKin& genKin, TString MCtype, vector<MyTracks>& trcoll, vector<MyVertex>& vtxcoll, vector<MyVertex>::iterator& vtxIter,
                      int vtxId, MyBeamSpot* bs, double weight){
  
  if(isMC) { 
    if(isSD) 
        centralp_SD->fill(gpcoll,trcoll,vtxcoll,vtxIter,vtxId,bs,weight); 
    else 
        centralp_NSD->fill(gpcoll,trcoll,vtxcoll,vtxIter,vtxId,bs,weight);            
    if(isDD) 
        centralp_DD->fill(gpcoll,trcoll,vtxcoll,vtxIter,vtxId,bs,weight);        

    centralp_INEL->fill(gpcoll,trcoll,vtxcoll,vtxIter,vtxId,bs,weight);            
  }
 
  centralp_RECO->fill(gpcoll,trcoll,vtxcoll,vtxIter,vtxId,bs,weight);
 
}

//_____________________________________________________________________________
void NCHDiffPlots::write(){ 
  gDirectory->mkdir("Difflvl_"+diffcoll);
  gDirectory->cd("Difflvl_"+diffcoll);

  if(isMC) {    
    centralp_SD->write();
    centralp_DD->write();
    centralp_NSD->write();
    centralp_INEL->write();
  }
  centralp_RECO->write();

  gDirectory->cd("../");
}
