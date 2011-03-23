#ifndef __TrackPlots_H__
#define __TrackPlots_H__

#include <vector>
using namespace std;
#include "TObject.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TMath.h"
#include "TDirectory.h"
#include "./MyTracks.h"
#include "./MyVertex.h"
#include "./MyBeamSpot.h"
#include "BasePlots.h"

class TrackPlots : public BasePlots {

  public :
    TrackPlots();
    TrackPlots(TString);
    virtual ~TrackPlots();

    TString trackcoll;
    
    TH1F* charge; 
    TH1F* nch;
    TH1F* pt;
    TH1F* eta;
    TH1F* phi;
    TH1F* nhit;
    TH1F* chi2n;
    TH1F* dz;
    TH1F* edz;
    TH1F* d0;
    TH1F* d0bs;
    TH1F* ed0;
    TH1F* dzOedz;
    TH1F* dxyOed0;
    TH1F* dzOsz;
    TH1F* dxyOsxy;
    TH1F* dxybsOsxy;
    TH1F* eptOpt;
       
    TH1F* quality;  

    TH1F* fdz;
    TH1F* sz;
    TH1F* sxy;
    
    TH1F* dzOsz_old;
    TH2F* dzOszVSphi_old;
    
    TH2F* dxyOsxyVSphi;
    TH2F* dzOszVSphi;

    TH1F* dxyz;
    
    TH2F* etaVSnhit;
    
    void init();
    void fill(vector<MyTracks>& , vector<MyVertex>::iterator&, int , MyBeamSpot*, double = 1.);
    void divide(TrackPlots& , const TString);
    template <class T> void makeEff( T*, T*, const TString);
    void write();
    
  private:
    int getVtxposFromId(MyTracks&,int);
    
    ClassDef (TrackPlots,1)
};



#endif


