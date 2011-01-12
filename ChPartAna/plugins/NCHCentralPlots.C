#include "NCHCentralPlots.h"

ClassImp(NCHCentralPlots)

//_____________________________________________________________________________
NCHCentralPlots::NCHCentralPlots(){
  centralcoll = "none";
  isRECO = 0;
  this->init();
}

//_____________________________________________________________________________
NCHCentralPlots::NCHCentralPlots(TString collname, int isreco){
  centralcoll = collname;
  isRECO = isreco;
  this->init();
}

//_____________________________________________________________________________
NCHCentralPlots::~NCHCentralPlots(){ 
    delete hfp_nocut;
    delete hfp_MBUEWG;
    if(!switchMBUEWGonly) {
        delete hfp_ALICE;
        delete hfp_ATLAS1;
        delete hfp_ATLAS6;
    }
}

//_____________________________________________________________________________
bool NCHCentralPlots::passMBUEWGreco=0;
bool NCHCentralPlots::passMBUEWGpart=0;
bool NCHCentralPlots::passALICEpart=0;
bool NCHCentralPlots::passALICEreco=0;
bool NCHCentralPlots::passATLAS1part=0;
bool NCHCentralPlots::passATLAS1reco=0;
bool NCHCentralPlots::passATLAS6part=0;
bool NCHCentralPlots::passATLAS6reco=0;

bool NCHCentralPlots::switchMBUEWGonly=1;  

//_____________________________________________________________________________
void NCHCentralPlots::init(){
 
  hfp_nocut                 = new NCHHFPlots("nocut_" +centralcoll);
  hfp_MBUEWG                = new NCHHFPlots("MBUEWG_"+centralcoll);
  if(!switchMBUEWGonly){
     hfp_ALICE                 = new NCHHFPlots("ALICE_" +centralcoll);
     hfp_ATLAS1                = new NCHHFPlots("ATLAS1_"+centralcoll);
     hfp_ATLAS6                = new NCHHFPlots("ATLAS6_"+centralcoll);
  }   
 
}

//_____________________________________________________________________________
void NCHCentralPlots::fill(vector<MyGenPart>& gpcoll, vector<MyTracks>& trcoll, vector<MyVertex>& vtxcoll, vector<MyVertex>::iterator& vtxIter, int vtxId, MyBeamSpot* bs, double weight){
 
  hfp_nocut->fill(gpcoll,trcoll,vtxcoll,vtxIter,vtxId,bs,weight);
   
  if( (passMBUEWGpart && !isRECO ) || (passMBUEWGreco && isRECO) )
      hfp_MBUEWG->fill(gpcoll,trcoll,vtxcoll,vtxIter,vtxId,bs,weight);
  
  if(!switchMBUEWGonly){           
    if( (passALICEpart && !isRECO ) || (passALICEreco && isRECO) )
        hfp_ALICE->fill(gpcoll,trcoll,vtxcoll,vtxIter,vtxId,bs,weight); 
         
    if( (passATLAS1part && !isRECO ) || (passATLAS1reco && isRECO) )
        hfp_ATLAS1->fill(gpcoll,trcoll,vtxcoll,vtxIter,vtxId,bs,weight);
      
    if( (passATLAS6part && !isRECO ) || (passATLAS6reco && isRECO) )
        hfp_ATLAS6->fill(gpcoll,trcoll,vtxcoll,vtxIter,vtxId,bs,weight);
  }
  
}

//_____________________________________________________________________________
void NCHCentralPlots::write(){ 
  gDirectory->mkdir("Centrlvl_"+centralcoll);
  gDirectory->cd("Centrlvl_"+centralcoll);
  
  hfp_nocut->write();  
  hfp_MBUEWG->write();
  
  if(!switchMBUEWGonly){
    hfp_ALICE->write();
    hfp_ATLAS1->write();  
    hfp_ATLAS6->write();
  }  
 
  gDirectory->cd("../");
}

