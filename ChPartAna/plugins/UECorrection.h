//STANDARD ROOT INCLUDES
#include <TROOT.h>
#include <TH1.h>
#include <TH2.h>
#include <TProfile.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TTree.h>
#include <TFile.h>
#include <TChain.h>
#include <TChainElement.h>
#include <TDirectory.h>
#include <vector>

#include "TObject.h"
//#include "../../UADataFormat/src/MyPart.h"
//#include "../../UADataFormat/src/MyVertex.h"
#include "../mydir/MyTracks.h"
#include "../mydir/MyGenPart.h"
#include "../mydir/MyBaseJet.h"
//#include "../macro2/UAPlotFactory/CPlot.C"

//STANDARD C++ INCLUDES
#include <iostream>
#include <sstream>

using namespace std;

class UECorrection : public TObject {

  public:
  UECorrection();
  UECorrection(TString);
  UECorrection(TString, vector<double>);
  virtual ~UECorrection();
  
  //Jet Correlations, ratios and corrections
  TH2F* pt_corr;
  
  TH1D* profile_corrected_matrix;
  TH1D* profile_corrected_evtsel;
  TH1D* profile_corrected_genreco;
  //TH1D* profile_corrected_pteta;
  TH1D* gen_reco_ratio;
  TH1D* truegen_gen_ratio;
  TH1D* probability;
  TH1D* profile_corrected_matrix_normalised;
  //TH2D* eff_pt_eta;
  
  int iEvt;
  
  TString UEcoll;
  
  virtual void init();
  //virtual bool CorrectPtEta(TProfile*);
  virtual bool CorrectGenReco(TProfile*, TString);
  virtual bool CorrectMatrix(TProfile*, TString);
  virtual bool CorrectMatrix2(TProfile*, TString);
  virtual bool CorrectEvtSel(TProfile*, TString);
  virtual void write();
  virtual void write(TString);

  private:
          
  int nobins;        
  double* varbins;
          
  ClassDef(UECorrection,1)

};
