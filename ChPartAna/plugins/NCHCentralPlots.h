#ifndef __NCHCentralPlots_H__
#define __NCHCentralPlots_H__

#include <vector>
#include <iostream>
#include "TObject.h"
#include "TDirectory.h"
#include "./NCHHFPlots.h"



class NCHCentralPlots : public TObject {

  public :
    NCHCentralPlots();
    NCHCentralPlots(TString);
    virtual ~NCHCentralPlots();

    TString centralcoll;
    
    NCHHFPlots* hfp_nocut;
    NCHHFPlots* hfp_MBUEWG;
    NCHHFPlots* hfp_ALICE;
    NCHHFPlots* hfp_ATLAS1; 
    NCHHFPlots* hfp_ATLAS2;  
    NCHHFPlots* hfp_ATLAS6;
    
    void init();
    void fill(vector<MyGenPart>&,vector<MyTracks>& , vector<MyVertex>&, vector<MyVertex>::iterator&, int, MyBeamSpot*, double =1.);//int=vtxId
    void write();
       
    static bool passMBUEWGGen;
    static bool passALICEGen ;
    static bool passATLAS1Gen;
    static bool passATLAS2Gen;
    static bool passATLAS6Gen;

    static bool switchMBUEWGonly;   
    
    static bool passMBUEWGRECO;
    static bool passALICERECO;
    static bool passATLAS1RECO;
    static bool passATLAS2RECO;
    static bool passATLAS6RECO;
    
  private:
    
    ClassDef (NCHCentralPlots,1)
};

#endif


