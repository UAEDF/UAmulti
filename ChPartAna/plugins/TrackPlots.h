#ifndef __TrackPlots_H__
#define __TrackPlots_H__

#include <vector>
#include "TObject.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TMath.h"
#include "TDirectory.h"
#include "./MyTracks.h"
#include "./MyVertex.h"
#include "./MyBeamSpot.h"

class TrackPlots : public TObject {

  public :
    TrackPlots();
    TrackPlots(TString);
    virtual ~TrackPlots();

    TString trackcoll;
    
    TH1F* nch;
    TH1F* pt;
    TH1F* eta;
    TH1F* phi;
    TH1F* nhit;
    TH1F* chi2n;
    TH1F* dz;
    TH1F* d0;
    TH1F* edz;
    TH1F* ed0;
    TH1F* dzOedz;
    TH1F* dxyOed0;
    TH1F* dxyOsxy;
    TH1F* dzOsz;
    TH1F* eptOpt;
    
    TH1F* fdz;
    TH1F* sz;
    TH1F* sxy;
    
    TH1F* dzOsz_old;
    TH2F* dzOszVSphi_old;
    
    TH2F* dxyOsxyVSphi;
    TH2F* dzOszVSphi;
    
    void init();
    void fill(vector<MyTracks>& , vector<MyVertex>& , int , MyBeamSpot*, double = 1.);
    void write();
    
  private:
    int getVtxposFromId(MyTracks&,int);
    
    ClassDef (TrackPlots,1)
};



#endif


