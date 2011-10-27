#ifndef __VertexPlots_H__
#define __VertexPlots_H__

#include <vector>
using namespace std;
#include "TObject.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TDirectory.h"
#include "../mydir/MyVertex.h"

class VertexPlots : public TObject {

  public :
    VertexPlots();
    VertexPlots(TString);
    virtual ~VertexPlots();

  // private:

    TString vertexcoll;
    
    TH1F* x;
    TH1F* y;
    TH1F* z;
    TH1F* z_wide;
    TH1F* ex;
    TH1F* ey;
    TH1F* ez;
    TH1F* ndof;
    TH1F* chi2n;
    TH1F* chi2;
    TH1F* ntracks;
    TH1F* nvertex;
    
    TH2F* xy;
    TH2F* xz;
    TH2F* yz;
    
    void init();
    void fill(vector<MyVertex>&, double = 1.);
    void fill(MyVertex&, double = 1., int = 1);
    void write();
    
  private:
    
    ClassDef (VertexPlots,1)
};



#endif


