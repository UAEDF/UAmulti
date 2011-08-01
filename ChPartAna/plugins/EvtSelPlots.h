#ifndef __EvtSelPlots_H__
#define __EvtSelPlots_H__

#include <vector>
#include <iostream>
#include "TObject.h"
#include "TH1F.h"
#include "TDirectory.h"
#include "../mydir/MyTracks.h"
#include "../mydir/MyVertex.h"
#include "../mydir/MyBeamSpot.h"
#include "./TrackPlots.h"
#include "./VertexPlots.h"

class EvtSelPlots : public TObject {

  public :
    EvtSelPlots();
    EvtSelPlots(TString);
    virtual ~EvtSelPlots();

    TString trackcoll;
    TH1F* nch_noSel;
    TH1F* nch_b40Sel;
    TH1F* nch_L1Sel;
    TH1F* nch_hfSel;
    TH1F* nch_vtxqualSel;
    TH1F* nch_vtxSel;
    TH1F* nch_L1_hfSel;
    TH1F* nch_L1_hf_vtxqualSel;
    TH1F* nch_L1_hf_vtxqual_vtxSel;
    TH1F* nch_L1_b40_vtxqualSel;
    TH1F* nch_denom;
    TH1F* pt_denom;
    TH1F* eta_denom;
    
    TH2F* vtxqual_noSel;
    TH2F* vtxqual_b40Sel;
    TH2F* vtxqual_L1Sel;
    TH2F* vtxqual_hfSel;
    TH2F* vtxqual_vtxqualSel;
    TH2F* vtxqual_vtxSel;
    TH2F* vtxqual_L1_hfSel;
    TH2F* vtxqual_L1_hf_vtxqualSel;
    TH2F* vtxqual_L1_hf_vtxqual_vtxSel;
    TH2F* vtxqual_L1_b40_vtxqualSel;
    
    TrackPlots* trp_noSel;
    TrackPlots* trp_b40Sel;
    TrackPlots* trp_L1Sel;
    TrackPlots* trp_hfSel;
    TrackPlots* trp_vtxqualSel;
    TrackPlots* trp_vtxSel;
    TrackPlots* trp_L1_hfSel;
    TrackPlots* trp_L1_hf_vtxqualSel;
    TrackPlots* trp_L1_hf_vtxqual_vtxSel;
    TrackPlots* trp_L1_vtxqual_vtxSel;
    TrackPlots* trp_L1_b40_vtxqualSel;
    
    VertexPlots* vtxp_noSel;
    VertexPlots* vtxp_b40Sel;
    VertexPlots* vtxp_L1Sel;
    VertexPlots* vtxp_hfSel;
    VertexPlots* vtxp_vtxSel;
    VertexPlots* vtxp_vtxqualSel;
    VertexPlots* vtxp_L1_hfSel;
    VertexPlots* vtxp_L1_hf_vtxqualSel;
    VertexPlots* vtxp_L1_hf_vtxqual_vtxSel;
    VertexPlots* vtxp_L1_vtxqual_vtxSel;
    VertexPlots* vtxp_L1_b40_vtxqualSel;
  
    // To look at #vtx and there quality --> Ferenc vtx problem
    TH1F* nvtx_evtSel;
    TH1F* nvtx_ntrneq0_evtSel;
    TrackPlots*  trp_nvtxeq1_evtSel;
    TrackPlots*  trp_nvtxgt1_evtSel;
    VertexPlots* vtx1_evtSel;
    VertexPlots* vtx2_evtSel;
    VertexPlots* vtx3_evtSel;
 
    // Look for tracks close to vtx=(0,0,0) --> Eta problem ?

    TrackPlots*  trp_smallzvtx_evtSel;
    TrackPlots*  trp_mediumzvtx_evtSel;
    TrackPlots*  trp_largezvtx_evtSel;

    void init();
    void setSelBits(bool, bool, bool, bool, bool);
    void fill(vector<MyTracks>& , vector<MyVertex>&, MyBeamSpot*, int, double, int);//vtxId, vtxQual, npixhits
    void write();
    void setDenom(TH1F*, TH1F*,  TH1F*);
    void makeEffPlots(TString);
    
  private:
    bool passL1, passHF, passVtxQual, passVtx, passBit40;
    void makeEffVSvar(TrackPlots*);
    void makeEff(TH1F*,TH1F*);
    void makeEff2(TH1F*,TH1F*);
    
    ClassDef (EvtSelPlots,1)
};



#endif


