#ifndef __NCHHFPlots_H__
#define __NCHHFPlots_H__

#include <vector>
#include <iostream>
#include "TObject.h"
#include "TDirectory.h"
#include "./NCHEvtSelPlots.h"

class NCHHFPlots : public TObject {

  public :
    NCHHFPlots();
    NCHHFPlots(TString);
    virtual ~NCHHFPlots();

    TString hfcoll;
    

    NCHEvtSelPlots* evtsel_HF0;
    NCHEvtSelPlots* evtsel_HF1;   
    NCHEvtSelPlots* evtsel_HF2;
    NCHEvtSelPlots* evtsel_HF3;
    NCHEvtSelPlots* evtsel_HF4;
    NCHEvtSelPlots* evtsel_HF5;
    NCHEvtSelPlots* evtsel_HF6;
    
    void init();
    void fill(vector<MyGenPart>&, vector<MyTracks>& , vector<MyVertex>&, vector<MyVertex>::iterator, int, MyBeamSpot*, bool, bool, double =1.);//int=vtxId
    void write();
    
    //static bool passHF0;  //not needed HF0 means cut off..
    static bool passHF1;   
    //passHF2=!passHF1  
    static bool passHF3;
    static bool passHF4;
    static bool passHF5;
    static bool passHF6;
    
    static bool switchAllHF;
    
  private:
    
    ClassDef (NCHHFPlots,1)
};

#endif


