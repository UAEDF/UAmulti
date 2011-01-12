#ifndef __NCHEvtSelPlots_H__
#define __NCHEvtSelPlots_H__

#include <vector>
#include <iostream>
#include "TObject.h"
#include "TDirectory.h"
#include "./MyTracks.h"
#include "./MyVertex.h"
#include "./MyBeamSpot.h"
#include "./GenPartPlots.h"
#include "./TrackPlots.h"
#include "./VertexPlots.h"
#include "./TResponseMtx.h"
#include "./MultiPlots.h"

class NCHEvtSelPlots : public TObject {

  public :
    NCHEvtSelPlots();
    NCHEvtSelPlots(TString);
    virtual ~NCHEvtSelPlots();

    TString trackcoll;

    GenPartPlots* gpp_noSel;
    GenPartPlots* gpp_b40Sel;
    GenPartPlots* gpp_L1Sel;
    GenPartPlots* gpp_hfSel;
    GenPartPlots* gpp_vtxqualSel;
    GenPartPlots* gpp_vtxSel;
    GenPartPlots* gpp_L1_hfSel;
    GenPartPlots* gpp_L1_hf_vtxqualSel;
    //GenPartPlots* gpp_full; 

    
    TResponseMtx* mtx_full;   
    
    TrackPlots* trp_noSel;
    TrackPlots* trp_b40Sel;
    TrackPlots* trp_L1Sel;
    TrackPlots* trp_hfSel;
    TrackPlots* trp_vtxqualSel;
    TrackPlots* trp_vtxSel;
    TrackPlots* trp_L1_hfSel;
    TrackPlots* trp_L1_hf_vtxqualSel;
    TrackPlots* trp_full; 
    
    VertexPlots* vtxp_noSel;
    VertexPlots* vtxp_b40Sel;
    VertexPlots* vtxp_L1Sel;
    VertexPlots* vtxp_hfSel;
    VertexPlots* vtxp_vtxSel;
    VertexPlots* vtxp_vtxqualSel;
    VertexPlots* vtxp_L1_hfSel;
    VertexPlots* vtxp_L1_hf_vtxqualSel;
    VertexPlots* vtxp_full;
    
    MultiPlots* mppart_noSel;
    MultiPlots* mppart_full;
    MultiPlots* mpreco_full;
    
    void init();
    void fill(vector<MyGenPart>&, vector<MyTracks>&, vector<MyVertex>&, vector<MyVertex>::iterator&, int, MyBeamSpot*,double =1.); //int=vtxId
    void write();
    void makeEffPlots();
    
    static bool passL1;
    static bool passHF;
    static bool passVtxQual;
    static bool passVtx;
    static bool passBit40;
    
    static bool isMC;  
    static bool switchIntermedPlots;
    
  private:
    
    ClassDef (NCHEvtSelPlots,1)
};

#endif


