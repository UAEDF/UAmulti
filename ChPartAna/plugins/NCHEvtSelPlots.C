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
        delete mtx_noSel; 
        delete mtx_vtxSel; 
        delete gpp_CentralGen;              
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
        gpp_b40Sel                    = new GenPartPlots("gpp_b40Sel_"+trackcoll);
        gpp_L1Sel                     = new GenPartPlots("gpp_L1Sel_"+trackcoll);
        gpp_hfSel                     = new GenPartPlots("gpp_hfSel_"+trackcoll);
        gpp_vtxqualSel                = new GenPartPlots("gpp_vtxqualSel_"+trackcoll);
        gpp_vtxSel                    = new GenPartPlots("gpp_vtxSel_"+trackcoll);
        gpp_L1_hfSel                  = new GenPartPlots("gpp_L1_hfSel_"+trackcoll);
        gpp_L1_hf_vtxqualSel          = new GenPartPlots("gpp_L1_hf_vtxqualSel_"+trackcoll);
    }
    //needed for efficiency plots
    gpp_noSel                         = new GenPartPlots("gpp_noSel_"+trackcoll);    
    //gpp_full                          = new GenPartPlots("full_"+trackcoll);
    mppart_noSel                      = new MultiPlots("mp_partnoSel_"+trackcoll);
    mppart_full                       = new MultiPlots("mp_partfull_"+trackcoll);    
    //matrix
    mtx_full                          = new TResponseMtx("partfull_"+trackcoll, 
                                vector<Double_t>( mppart_full->nch_array, mppart_full->nch_array+mppart_full->nch_nbin+1) );  
    mtx_noSel                         = new TResponseMtx("mtx_noSel_"+trackcoll, 
                                vector<Double_t>( mppart_full->nch_array, mppart_full->nch_array+mppart_full->nch_nbin+1) );    
    mtx_vtxSel                        = new TResponseMtx("mtx_vtxSel_"+trackcoll, 
                                vector<Double_t>( mppart_full->nch_array, mppart_full->nch_array+mppart_full->nch_nbin+1) );  
                                
    gpp_CentralGen                    = new GenPartPlots("gpp_CentrGen_"+trackcoll);         
  }
  
  //Reconstructed Tracks
  if(switchIntermedPlots){
    trp_b40Sel                        = new TrackPlots("trp_b40Sel_"+trackcoll);
    trp_L1Sel                         = new TrackPlots("trp_L1Sel_"+trackcoll);
    trp_hfSel                         = new TrackPlots("trp_hfSel_"+trackcoll);
    trp_vtxqualSel                    = new TrackPlots("trp_vtxqualSel_"+trackcoll);
    trp_vtxSel                        = new TrackPlots("trp_vtxSel_"+trackcoll);
    trp_L1_hfSel                      = new TrackPlots("trp_L1_hfSel_"+trackcoll);
    trp_L1_hf_vtxqualSel              = new TrackPlots("trp_L1_hf_vtxqualSel_"+trackcoll);
  } 
  trp_noSel                           = new TrackPlots("trp_noSel_"+trackcoll);  
  trp_full                            = new TrackPlots("trp_full_"+trackcoll);
  
  if(switchIntermedPlots){ 
    vtxp_b40Sel                       = new VertexPlots("vtxp_b40Sel_"+trackcoll);
    vtxp_L1Sel                        = new VertexPlots("vtxp_L1Sel_"+trackcoll);
    vtxp_hfSel                        = new VertexPlots("vtxp_hfSel_"+trackcoll);
    vtxp_vtxqualSel                   = new VertexPlots("vtxp_vtxqualSel_"+trackcoll);
    vtxp_vtxSel                       = new VertexPlots("vtxp_vtxSel_"+trackcoll);
    vtxp_L1_hfSel                     = new VertexPlots("vtxp_L1_hfSel_"+trackcoll);
    vtxp_L1_hf_vtxqualSel             = new VertexPlots("vtxp_L1_hf_vtxqualSel_"+trackcoll);
  }
  vtxp_noSel                          = new VertexPlots("vtxp_noSel_"+trackcoll);  
  vtxp_full                           = new VertexPlots("vtxp_full_"+trackcoll); 
  
  mpreco_noSel                        = new MultiPlots("mpreco_noSel_"+trackcoll); 
  mpreco_full                         = new MultiPlots("mpreco_full_"+trackcoll); 
  

}

//_____________________________________________________________________________
void NCHEvtSelPlots::fill(vector<MyGenPart>& gpcoll, vector<MyTracks>& trcoll, vector<MyVertex>& vtxcoll, vector<MyVertex>::iterator& goodVtx, 
                                            int vtxId, MyBeamSpot* bs,bool passCentralGen, bool passCentralRECO, double weight){  
  
  if(passCentralRECO) {
   
    //no selection
    trp_noSel->fill(trcoll,goodVtx,vtxId,bs,weight);	 //in passCentralRECO, because we use it for data
    for(vector<MyTracks>::iterator tr = trcoll.begin() ; tr != trcoll.end() ; ++tr){	
       mpreco_noSel->fill(*(dynamic_cast<MyPart*>(&(*tr))),weight);    
    }  
    mpreco_noSel->nextEvent(1,weight);

    if(isMC){
      gpp_noSel->fill(gpcoll,weight);
      mtx_noSel->Fill(gpcoll.size(), trcoll.size(),weight);

      for(vector<MyGenPart>::iterator gp = gpcoll.begin() ; gp != gpcoll.end() ; ++gp){ 	      
        mppart_noSel->fill(*(dynamic_cast<MyPart*>(&(*gp))),weight);    
      }
      //going to next event for the Multiplots
      mppart_noSel->nextEvent(1,weight);   
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
          if(isMC) mtx_vtxSel->Fill(gpcoll.size(), trcoll.size(),weight);
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
              mppart_full->fill(*(dynamic_cast<MyPart*>(&(*gp))),weight);    
          }
      }
      if(goodVtx != vtxcoll.end()) vtxp_full->fill(*goodVtx,weight);    
      for(vector<MyTracks>::iterator tr = trcoll.begin() ; tr != trcoll.end() ; ++tr){    
         mpreco_full->fill(*(dynamic_cast<MyPart*>(&(*tr))),weight);    
      }
    
         
      //going to next event for the Multiplots
      if(isMC) mppart_full->nextEvent(1,weight); 
      mpreco_full->nextEvent(1,weight);
    }      
  
  }  
        
  //to get the efficiency of the Central cut 
  if(isMC){
      if(passCentralGen)
  	  gpp_CentralGen->fill(gpcoll,weight);
      //noSel = gpp_CentralRECO  
  
  }
  
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
     mtx_full->write(0);  //0 will not write the class
     mtx_noSel->write(0);
     mtx_vtxSel->write(0);
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
    
  mpreco_noSel->write(scale);  
  mpreco_full->write(scale);    
  
  if(isMC){
    mppart_full->write(scale);
    mppart_noSel->write(scale);
    gpp_CentralGen->write();
    
    //Efficiency Plots (only run for particles, not for tracks)
    //if(!trackcoll.Contains("RECO")) makeEffPlots();
  }  
      
  gDirectory->cd("../");
}

//_____________________________________________________________________________
void NCHEvtSelPlots::makeEffPlots(){
    
//GenPartPlots
   if(switchIntermedPlots){ 
        gpp_b40Sel->divide(*gpp_noSel,"gpp_b40_"+trackcoll);
        gpp_L1Sel->divide(*gpp_noSel,"gpp_L1_"+trackcoll);
        gpp_hfSel->divide(*gpp_noSel,"gpp_hf_"+trackcoll);
        gpp_vtxqualSel->divide(*gpp_noSel,"gpp_vtxqual_"+trackcoll);
        gpp_vtxSel->divide(*gpp_noSel,"gpp_vtx_"+trackcoll);
        gpp_L1_hfSel->divide(*gpp_noSel,"gpp_L1_hf_"+trackcoll);
        gpp_L1_hf_vtxqualSel->divide(*gpp_noSel,"gpp_L1_hf_vtxqual_"+trackcoll);  
   }     
   //gpp_full->divide(*gpp_noSel,"full_"+trackcoll); 
//MultiPlots   
   mppart_full->divide(*mppart_noSel,"mp_partfull_"+trackcoll); 
   gpp_noSel->divide(*gpp_CentralGen,"gpp_CentrEff_"+trackcoll);
}
  
