#include "NCHEvtSelPlots.h"

ClassImp(NCHEvtSelPlots)

//_____________________________________________________________________________
NCHEvtSelPlots::NCHEvtSelPlots(){
  trackcoll = "none";
  this->init();
}

//_____________________________________________________________________________
NCHEvtSelPlots::NCHEvtSelPlots(TString collname){
  trackcoll = collname;
  this->init();
}
//_____________________________________________________________________________
NCHEvtSelPlots::~NCHEvtSelPlots(){ 
    if (isMC) {   
        if(switchIntermedPlots){ 
            delete gpp_b40Sel;           
            delete gpp_L1Sel;            
            delete gpp_hfSel;            
            delete gpp_vtxqualSel;       
            delete gpp_vtxSel;           
            delete gpp_L1_hfSel;         
            delete gpp_L1_hf_vtxqualSel; 
        }
        delete gpp_noSel;                
        //delete gpp_full;                 
        delete mppart_full; 
        delete mppart_noSel;             
        delete mtx_full;                 
    }

    if(switchIntermedPlots){
        delete trp_b40Sel;               
        delete trp_L1Sel;                
        delete trp_hfSel;                
        delete trp_vtxqualSel;           
        delete trp_vtxSel;               
        delete trp_L1_hfSel;             
        delete trp_L1_hf_vtxqualSel;      
        delete vtxp_b40Sel;              
        delete vtxp_L1Sel;               
        delete vtxp_hfSel;              
        delete vtxp_vtxqualSel;          
        delete vtxp_vtxSel;              
        delete vtxp_L1_hfSel;            
        delete vtxp_L1_hf_vtxqualSel;    
    }
    delete trp_noSel;                  
    delete trp_full;
    delete vtxp_noSel;                 
    delete vtxp_full;                 
    delete mpreco_full;                

}


//_____________________________________________________________________________
bool NCHEvtSelPlots::passL1=0;
bool NCHEvtSelPlots::passHF=0;
bool NCHEvtSelPlots::passVtxQual=0;
bool NCHEvtSelPlots::passVtx=0;
bool NCHEvtSelPlots::passBit40=0;

bool NCHEvtSelPlots::isMC=0;
bool NCHEvtSelPlots::switchIntermedPlots=0;


//_____________________________________________________________________________
void NCHEvtSelPlots::init(){

  if (isMC) {
  
    //Particles 
    if(switchIntermedPlots){        
        gpp_b40Sel                    = new GenPartPlots("b40Sel_"+trackcoll);
        gpp_L1Sel                     = new GenPartPlots("L1Sel_"+trackcoll);
        gpp_hfSel                     = new GenPartPlots("hfSel_"+trackcoll);
        gpp_vtxqualSel                = new GenPartPlots("vtxqualSel_"+trackcoll);
        gpp_vtxSel                    = new GenPartPlots("vtxSel_"+trackcoll);
        gpp_L1_hfSel                  = new GenPartPlots("L1_hfSel_"+trackcoll);
        gpp_L1_hf_vtxqualSel          = new GenPartPlots("L1_hf_vtxqualSel_"+trackcoll);
    }
    //needed for efficiency plots
    gpp_noSel                         = new GenPartPlots("noSel_"+trackcoll);    
    //gpp_full                          = new GenPartPlots("full_"+trackcoll);
    mppart_noSel                      = new MultiPlots("partnoSel_"+trackcoll);
    mppart_full                       = new MultiPlots("partfull_"+trackcoll);    
    //matrix
    mtx_full                          = new TResponseMtx("partfull_"+trackcoll, 
                                vector<Double_t>( mppart_full->nch_array, mppart_full->nch_array+mppart_full->nch_nbin+1) );     
  }
  
  //Reconstructed Tracks
  if(switchIntermedPlots){
    trp_b40Sel                        = new TrackPlots("b40Sel_"+trackcoll);
    trp_L1Sel                         = new TrackPlots("L1Sel_"+trackcoll);
    trp_hfSel                         = new TrackPlots("hfSel_"+trackcoll);
    trp_vtxqualSel                    = new TrackPlots("vtxqualSel_"+trackcoll);
    trp_vtxSel                        = new TrackPlots("vtxSel_"+trackcoll);
    trp_L1_hfSel                      = new TrackPlots("L1_hfSel_"+trackcoll);
    trp_L1_hf_vtxqualSel              = new TrackPlots("L1_hf_vtxqualSel_"+trackcoll);
  } 
  trp_noSel                           = new TrackPlots("noSel_"+trackcoll);  
  trp_full                            = new TrackPlots("full_"+trackcoll);
  
  if(switchIntermedPlots){ 
    vtxp_b40Sel                       = new VertexPlots("b40Sel_"+trackcoll);
    vtxp_L1Sel                        = new VertexPlots("L1Sel_"+trackcoll);
    vtxp_hfSel                        = new VertexPlots("hfSel_"+trackcoll);
    vtxp_vtxqualSel                   = new VertexPlots("vtxqualSel_"+trackcoll);
    vtxp_vtxSel                       = new VertexPlots("vtxSel_"+trackcoll);
    vtxp_L1_hfSel                     = new VertexPlots("L1_hfSel_"+trackcoll);
    vtxp_L1_hf_vtxqualSel             = new VertexPlots("L1_hf_vtxqualSel_"+trackcoll);
  }
  vtxp_noSel                          = new VertexPlots("noSel_"+trackcoll);  
  vtxp_full                           = new VertexPlots("full_"+trackcoll); 
  
  mpreco_full                         = new MultiPlots("full_"+trackcoll); 

}

//_____________________________________________________________________________
void NCHEvtSelPlots::fill(vector<MyGenPart>& gpcoll, vector<MyTracks>& trcoll, vector<MyVertex>& vtxcoll, vector<MyVertex>::iterator& goodVtx, int vtxId, MyBeamSpot* bs,double weight){  
  
  //no selection
  trp_noSel->fill(trcoll,goodVtx,vtxId,bs,weight);
  if(isMC){
        gpp_noSel->fill(gpcoll,weight);
        for(vector<MyGenPart>::iterator gp = gpcoll.begin() ; gp != gpcoll.end() ; ++gp){               
            mppart_noSel->fill(gp->Part,weight);    
        }
  }
  if(goodVtx != vtxcoll.end()) vtxp_noSel->fill(*goodVtx,weight); 
  
  
  if(switchIntermedPlots){  
    if(passBit40){
        trp_b40Sel->fill(trcoll,goodVtx,vtxId,bs,weight);
        if(isMC) gpp_b40Sel->fill(gpcoll,weight);
        if(goodVtx != vtxcoll.end()) vtxp_b40Sel->fill(*goodVtx,weight);
    }
  
    if(passL1){
        trp_L1Sel->fill(trcoll,goodVtx,vtxId,bs,weight);
        if(isMC) gpp_L1Sel->fill(gpcoll,weight);
        if(goodVtx != vtxcoll.end()) vtxp_L1Sel->fill(*goodVtx,weight);
    }
  
    if(passHF){
        trp_hfSel->fill(trcoll,goodVtx,vtxId,bs,weight);
        if(isMC) gpp_hfSel->fill(gpcoll,weight);
        if(goodVtx != vtxcoll.end()) vtxp_hfSel->fill(*goodVtx,weight);
    }
  
    if(passVtxQual){
        trp_vtxqualSel->fill(trcoll,goodVtx,vtxId,bs,weight);
        if(isMC) gpp_vtxqualSel->fill(gpcoll,weight);
        if(goodVtx != vtxcoll.end()) vtxp_vtxqualSel->fill(*goodVtx,weight);
    }
  
    if(passVtx){
        trp_vtxSel->fill(trcoll,goodVtx,vtxId,bs,weight);
        if(isMC) gpp_vtxSel->fill(gpcoll,weight);
        if(goodVtx != vtxcoll.end()) vtxp_vtxSel->fill(*goodVtx,weight);
    }
  
    if(passL1 && passHF){
        trp_L1_hfSel->fill(trcoll,goodVtx,vtxId,bs,weight);
        if(isMC) gpp_L1_hfSel->fill(gpcoll,weight);
        if(goodVtx != vtxcoll.end()) vtxp_L1_hfSel->fill(*goodVtx,weight);
    }  
  
    if(passL1 && passHF && passVtxQual){
        trp_L1_hf_vtxqualSel->fill(trcoll,goodVtx,vtxId,bs,weight);
        if(isMC) gpp_L1_hf_vtxqualSel->fill(gpcoll,weight);
        if(goodVtx != vtxcoll.end()) vtxp_L1_hf_vtxqualSel->fill(*goodVtx,weight);
    }
  }
  
  // Final selection 
  if(passL1 && passHF && passVtxQual && passVtx){
    trp_full->fill(trcoll,goodVtx,vtxId,bs,weight);    
    if(isMC) {
        //gpp_full->fill(gpcoll,weight);
        mtx_full->Fill(gpcoll.size(), trcoll.size(),weight);
        for(vector<MyGenPart>::iterator gp = gpcoll.begin() ; gp != gpcoll.end() ; ++gp){               
            mppart_full->fill(gp->Part,weight);    
        }
    }
    if(goodVtx != vtxcoll.end()) vtxp_full->fill(*goodVtx,weight);    
    for(vector<MyTracks>::iterator tr = trcoll.begin() ; tr != trcoll.end() ; ++tr){    
       mpreco_full->fill(tr->Part,weight);    
    }
  }      
  
  //going to next event for the Multiplots
  if(isMC) {
      mppart_noSel->nextEvent(1,weight); 
      mppart_full->nextEvent(1,weight);
  }    
  mpreco_full->nextEvent(1,weight);  
}  

//_____________________________________________________________________________
void NCHEvtSelPlots::write(){

  bool scale=0;
  
  gDirectory->mkdir("EvtSel_"+trackcoll);
  gDirectory->cd("EvtSel_"+trackcoll);

  if(isMC) {
     if(switchIntermedPlots){
        gpp_b40Sel->write();
        gpp_L1Sel->write();
        gpp_hfSel->write();
        gpp_vtxqualSel->write();
        gpp_vtxSel->write();
        gpp_L1_hfSel->write();
        gpp_L1_hf_vtxqualSel->write();
     }
     gpp_noSel->write();   
     //gpp_full->write();
     mtx_full->write();
  }
  
  if(switchIntermedPlots){  
    trp_b40Sel->write();
    trp_L1Sel->write();
    trp_hfSel->write();
    trp_vtxqualSel->write();
    trp_vtxSel->write();
    trp_L1_hfSel->write();
    trp_L1_hf_vtxqualSel->write();
    
    vtxp_b40Sel->write();
    vtxp_L1Sel->write();
    vtxp_hfSel->write();
    vtxp_vtxqualSel->write();
    vtxp_vtxSel->write();
    vtxp_L1_hfSel->write();
    vtxp_L1_hf_vtxqualSel->write();
  }
  trp_noSel->write();  
  trp_full->write();
  
  vtxp_noSel->write();
  vtxp_full->write(); 
    
  mpreco_full->write(scale);    
  
  if(isMC){
    mppart_full->write(scale);
    mppart_noSel->write(scale);
    
    //Efficiency Plots (only run for particles, not for tracks)
    if(!trackcoll.Contains("RECO")) makeEffPlots();
  }  
      
  gDirectory->cd("../");
}

//_____________________________________________________________________________
void NCHEvtSelPlots::makeEffPlots(){
    
//GenPartPlots
   if(switchIntermedPlots){ 
        gpp_b40Sel->divide(*gpp_noSel,"b40_"+trackcoll);
        gpp_L1Sel->divide(*gpp_noSel,"L1_"+trackcoll);
        gpp_hfSel->divide(*gpp_noSel,"hf_"+trackcoll);
        gpp_vtxqualSel->divide(*gpp_noSel,"vtxqual_"+trackcoll);
        gpp_vtxSel->divide(*gpp_noSel,"vtx_"+trackcoll);
        gpp_L1_hfSel->divide(*gpp_noSel,"L1_hf_"+trackcoll);
        gpp_L1_hf_vtxqualSel->divide(*gpp_noSel,"L1_hf_vtxqual_"+trackcoll);  
   }     
   //gpp_full->divide(*gpp_noSel,"full_"+trackcoll); 
//MultiPlots   
   mppart_full->divide(*mppart_noSel,"partfull_"+trackcoll); 
}
  
