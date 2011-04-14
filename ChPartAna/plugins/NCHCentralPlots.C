#include "NCHCentralPlots.h"

ClassImp(NCHCentralPlots)

//_____________________________________________________________________________
NCHCentralPlots::NCHCentralPlots(){
  centralcoll = "none";
  this->init();
}

//_____________________________________________________________________________
NCHCentralPlots::NCHCentralPlots(TString collname){
  centralcoll = collname;
  this->init();
}

//_____________________________________________________________________________
NCHCentralPlots::~NCHCentralPlots(){ 
    delete hfp_nocut;
    delete hfp_MBUEWG;
    if(!switchMBUEWGonly) {
        delete hfp_ALICE;
        delete hfp_ATLAS1;
        delete hfp_ATLAS2;
        delete hfp_ATLAS6;
    }
}

//_____________________________________________________________________________
bool NCHCentralPlots::passMBUEWGGen=0;
bool NCHCentralPlots::passALICEGen=0;
bool NCHCentralPlots::passATLAS1Gen=0;
bool NCHCentralPlots::passATLAS2Gen=0;
bool NCHCentralPlots::passATLAS6Gen=0;

bool NCHCentralPlots::switchMBUEWGonly=1; 

bool NCHCentralPlots::passMBUEWGRECO=0;
bool NCHCentralPlots::passALICERECO=0;
bool NCHCentralPlots::passATLAS1RECO=0;
bool NCHCentralPlots::passATLAS2RECO=0;
bool NCHCentralPlots::passATLAS6RECO=0;
 

//_____________________________________________________________________________
void NCHCentralPlots::init(){
 
  hfp_nocut                    = new NCHHFPlots("nocut_" +centralcoll);
  hfp_MBUEWG                   = new NCHHFPlots("MBUEWG_"+centralcoll);
  if(!switchMBUEWGonly){
     hfp_ALICE                 = new NCHHFPlots("ALICE_" +centralcoll);
     hfp_ATLAS1                = new NCHHFPlots("ATLAS1_"+centralcoll);
     hfp_ATLAS2                = new NCHHFPlots("ATLAS2_"+centralcoll);
     hfp_ATLAS6                = new NCHHFPlots("ATLAS6_"+centralcoll);
  }   
 
}

//_____________________________________________________________________________
void NCHCentralPlots::fill(vector<MyGenPart>& gpcoll, vector<MyTracks>& trcoll, vector<MyVertex>& vtxcoll, vector<MyVertex>::iterator& vtxIter, int vtxId, MyBeamSpot* bs, double weight){
   //if(gpcoll.size()==6)cout << "next " << endl;
  hfp_nocut->fill(gpcoll,trcoll,vtxcoll,vtxIter,vtxId,bs,1, 1,weight);  //1= pass no cut ==always
   

   hfp_MBUEWG->fill(gpcoll,trcoll,vtxcoll,vtxIter,vtxId,bs,passMBUEWGGen, passMBUEWGRECO,weight);
  
  if(!switchMBUEWGonly){           
        hfp_ALICE->fill(gpcoll,trcoll,vtxcoll,vtxIter,vtxId,bs,passALICEGen, passALICERECO,weight); 
         

        hfp_ATLAS1->fill(gpcoll,trcoll,vtxcoll,vtxIter,vtxId,bs,passATLAS1Gen, passATLAS1RECO,weight);

        
        hfp_ATLAS2->fill(gpcoll,trcoll,vtxcoll,vtxIter,vtxId,bs,passATLAS2Gen, passATLAS2RECO,weight);
   

        hfp_ATLAS6->fill(gpcoll,trcoll,vtxcoll,vtxIter,vtxId,bs,passATLAS6Gen, passATLAS6RECO,weight);
      
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
    hfp_ATLAS2->write();  
    hfp_ATLAS6->write();
  }  
 
  gDirectory->cd("../");
}

