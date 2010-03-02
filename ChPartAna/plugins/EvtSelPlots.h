#ifndef __EvtSelPlots_H__
#define __EvtSelPlots_H__

#include <vector>
#include <iostream>
#include "TObject.h"
#include "TH1F.h"
#include "TDirectory.h"
#include "./MyTracks.h"
#include "./MyVertex.h"
#include "./MyBeamSpot.h"
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
    VertexPlots* vtxp_L1_b40_vtxqualSel;
    
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


