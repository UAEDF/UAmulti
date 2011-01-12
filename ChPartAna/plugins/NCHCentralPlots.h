#ifndef __NCHCentralPlots_H__
#define __NCHCentralPlots_H__

#include <vector>
#include <iostream>
#include "TObject.h"
#include "TDirectory.h"
#include "./MyGenPart.h"
#include "./MyTracks.h"
#include "./MyVertex.h"
#include "./MyBeamSpot.h"
#include "./NCHHFPlots.h"



class NCHCentralPlots : public TObject {

  public :
    NCHCentralPlots();
    NCHCentralPlots(TString,int = 0);
    virtual ~NCHCentralPlots();

    TString centralcoll;
    
    NCHHFPlots* hfp_nocut;
    NCHHFPlots* hfp_MBUEWG;
    NCHHFPlots* hfp_ALICE;
    NCHHFPlots* hfp_ATLAS1;   
    NCHHFPlots* hfp_ATLAS6;
    
    void init();
    void fill(vector<MyGenPart>&,vector<MyTracks>& , vector<MyVertex>&, vector<MyVertex>::iterator&, int, MyBeamSpot*, double =1.);//int=vtxId
    void write();
       
    static bool passMBUEWGpart;
    static bool passMBUEWGreco;
    static bool passALICEpart;
    static bool passALICEreco;
    static bool passATLAS1part;
    static bool passATLAS1reco;
    static bool passATLAS6part;
    static bool passATLAS6reco;

    static bool switchMBUEWGonly;
    
  private:
    bool isRECO;
    
    ClassDef (NCHCentralPlots,1)
};

#endif


