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

class UEFactors : public TObject {

  public:
  UEFactors();
  UEFactors(TString);
  UEFactors(TString, vector<double>);
  virtual ~UEFactors();
  
  //for leading jet efficiency and pt correction
  TH1F* dpt_jetmax;
  TH1F* deta_jetmax;
  TH1F* dphi_jetmax;
  TH1F* dR_jetmax;
  TH1F* dpt_jetmax_greater;
  TH1F* dpt_jetmax_smaller;
  TH1F* deta_jetmax_matched;
  TH1F* deta_jetmax_unmatched;
  TH1F* dphi_jetmax_matched;
  TH1F* dphi_jetmax_unmatched;
  TH2F* detadphi_jetmax_matched;
  TH2F* detadphi_jetmax_unmatched;
  TH1F* pt_gen_jetmax_num_unmatched;
  TH1F* eta_gen_jetmax_num_unmatched;
  TH1F* dR_matched;
  TH1F* dR_unmatched;
  
  TH1F* mult_jetloss;
  
  TH1F* pt_gen_jetmax;
  TH1F* eta_gen_jetmax;
  TH1F* phi_gen_jetmax;
  TH1F* pt_reco_jetmax;
  TH1F* eta_reco_jetmax;
  TH1F* phi_reco_jetmax;
  TH1F* pt_gen_jetmax_num_found;
  TH1F* eta_gen_jetmax_num_found;
  TH1F* pt_gen_jetmax_perc_found;
  TH1F* eta_gen_jetmax_perc_found;
  
  TH1F* PtTrackJet_PtGenJet_matched;
  TH1F* PtTrackJet_PtGenJet_unmatched;
  TProfile* profile_PtTrackJet_PtGenJet_matched;
  TProfile* profile_PtTrackJet_PtGenJet_unmatched;
  
  //for leading track efficiency and pt correction
  TH1F* dpt_partmax;
  TH1F* deta_partmax;
  TH1F* dphi_partmax;
  TH1F* dR_partmax;
  TH1F* dpt_partmax_greater;
  TH1F* dpt_partmax_smaller;
  
  TH1F* pt_gen_partmax;
  TH1F* eta_gen_partmax;
  TH1F* phi_gen_partmax;
  TH1F* pt_trackmax;
  TH1F* eta_trackmax;
  TH1F* phi_trackmax;
  TH1F* pt_gen_partmax_num_found;
  TH1F* eta_gen_partmax_num_found;
  TH1F* pt_gen_partmax_perc_found;
  TH1F* eta_gen_partmax_perc_found;
  
  //Jet Correlations, ratios and corrections
  TH2F* pt_corr;
  
  TProfile* profile_corrected_matrix;
  TH1D* profile_corrected_truegengen;
  TH1D* profile_corrected_genreco;
  TH1D* gen_reco_ratio;
  TH1D* truegen_gen_ratio;
  
  int iEvt;
  
  TString UEcoll;
  
  virtual void init();
  virtual void fill(vector<MyBaseJet>&, vector<MyBaseJet>&, double, double, double = 1.);
  virtual void fill(vector<MyGenPart>&, vector<MyTracks>&, double, double, double = 1.);
  virtual void correctgenreco(TProfile*, TProfile*, TProfile*);
  virtual void correctgenreco(TProfile*);
  virtual void correctmatrix(TProfile*);
  virtual void correcttruegengen(TProfile*, TProfile*, TProfile*);
  virtual void correcttruegengen(TProfile*);
  virtual void write();

double* varbins;
  private:
          
  int nobins;        
  
          
  ClassDef(UEFactors,1)

};
