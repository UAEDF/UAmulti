#ifndef __NCHDiffPlots_H__
#define __NCHDiffPlots_H__

#include <vector>
#include <iostream>
#include "TObject.h"
#include "TDirectory.h"
#include "TString.h"
#include "../mydir/MyGenKin.h"
#include "./NCHCentralPlots.h"
#include "./NCHEvtSelPlots.h"

class NCHDiffPlots : public TObject {

  public :
    NCHDiffPlots();
    NCHDiffPlots(TString);
    virtual ~NCHDiffPlots();

    TString diffcoll;
    
    NCHCentralPlots* centralp_SD;               
    //NCHCentralPlots* centralp_DD;              
    NCHCentralPlots* centralp_RECO;               
    NCHCentralPlots* centralp_NSD;              
    //NCHCentralPlots* centralp_INEL;                
 
    
    void init();
    void fill(vector<MyGenPart>&, MyGenKin&, TString, vector<MyTracks>&, vector<MyVertex>&, vector<MyVertex>::iterator&, int, MyBeamSpot*, double =1.);//int=vtxId
    void write();
    
    static bool isMC;
    static bool isSD;
    static bool isDD; 
  
  private:
    
    ClassDef (NCHDiffPlots,1)
};

#endif
