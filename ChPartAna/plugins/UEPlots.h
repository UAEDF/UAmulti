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
#include "../plugins/TOperation.h"  
#include "../mydir/MyPart.h"
#include "../mydir/MyVertex.h"
#include "../mydir/MyTracks.h"
#include "../mydir/MyGenPart.h"
#include "../mydir/MyBaseJet.h"
//#include "../macro2/UAPlotFactory/CPlot.C"

//STANDARD C++ INCLUDES
#include <iostream>
#include <sstream>

using namespace std;

class UEPlots : public TObject {

  public:
  UEPlots();
  UEPlots(TString);
  UEPlots(TString, vector<double>);
  virtual ~UEPlots();
  
  //General Plots
  TH1F* pt_lead_part;
  TH1F* pt_lead_jet;
  TH1F* eta_lead_part;
  TH1F* eta_lead_jet;
  TH1F* phi_lead_part;
  TH1F* phi_lead_jet;
  
  TH1F* pt_nonlead_part;
  TH1F* pt_nonlead_jet;
  TH1F* eta_nonlead_part;
  TH1F* eta_nonlead_jet;
  TH1F* phi_nonlead_part;
  TH1F* phi_nonlead_jet;
  
  TH1F* dR_LeadPart_LeadJet;
  TH1F* deta_LeadPart_LeadJet;
  TH1F* dphi_LeadPart_LeadJet;
  TH2F* detadphi_LeadPart_LeadJet;
  TH1F* PtLeadPart_PtLeadJet;
  TProfile* profile_PtLeadPart_PtLeadJet;
  
  TH2F trEff;
  TH2F trFakes;
  TH2F ptcorr;
  TProfile ptratio;
  
  TH1F* weight_dist;
  
  //UE Histograms
  //---Uncorrected---
  TH1F* dphi;
  TH1F* n_mult_forward;
  TH1F* n_mult_trans;
  TH1F* n_mult_away;
  TH1F* n_mult_total;
  TH1F* ptsum_forward;
  TH1F* ptsum_trans;
  TH1F* ptsum_away;
  TH1F* ptsum_total;
  TH1F* pt_spectra_forward;
  TH1F* pt_spectra_trans;
  TH1F* pt_spectra_away;
  TH1F* pt_spectra_total;
  
  TProfile* profile_n_mult_forward;
  TProfile* profile_n_mult_trans;
  TProfile* profile_n_mult_away;
  TProfile* profile_ptsum_forward;
  TProfile* profile_ptsum_trans;
  TProfile* profile_ptsum_away;
  TProfile* profile_ptsum_n_mult_trans;
  TProfile* profile_n_mult_total;
  TProfile* profile_ptsum_total;
  
  //---Corrected for Jet Pt---  
  TProfile* profile_n_mult_forward_jetcorr;
  TProfile* profile_n_mult_trans_jetcorr;
  TProfile* profile_n_mult_away_jetcorr;
  TProfile* profile_ptsum_forward_jetcorr;
  TProfile* profile_ptsum_trans_jetcorr;
  TProfile* profile_ptsum_away_jetcorr;
  TProfile* profile_n_mult_total_jetcorr;
  TProfile* profile_ptsum_total_jetcorr;
  
  //---Corrected Track efficiency and Fake rates---  
  TH1F* n_mult_forward_weighted;
  TH1F* n_mult_trans_weighted;
  TH1F* n_mult_away_weighted;
  TH1F* n_mult_total_weighted;
  TH1F* ptsum_forward_weighted;
  TH1F* ptsum_trans_weighted;
  TH1F* ptsum_away_weighted;
  TH1F* ptsum_total_weighted;
  TH1F* pt_spectra_forward_weighted;
  TH1F* pt_spectra_trans_weighted;
  TH1F* pt_spectra_away_weighted;
  TH1F* pt_spectra_total_weighted;
  
  TProfile* profile_n_mult_forward_weighted;
  TProfile* profile_n_mult_trans_weighted;
  TProfile* profile_n_mult_away_weighted;
  TProfile* profile_ptsum_forward_weighted;
  TProfile* profile_ptsum_trans_weighted;
  TProfile* profile_ptsum_away_weighted;
  TProfile* profile_n_mult_total_weighted;
  TProfile* profile_ptsum_total_weighted;
  
  //---Corrected for Jet Pt and Track efficiency and Fake rates--- 
  TProfile* profile_n_mult_forward_jetcorr_weighted;
  TProfile* profile_n_mult_trans_jetcorr_weighted;
  TProfile* profile_n_mult_away_jetcorr_weighted;
  TProfile* profile_ptsum_forward_jetcorr_weighted;
  TProfile* profile_ptsum_trans_jetcorr_weighted;
  TProfile* profile_ptsum_away_jetcorr_weighted;
  TProfile* profile_n_mult_total_jetcorr_weighted;
  TProfile* profile_ptsum_total_jetcorr_weighted;
  
  /*//histograms for cleaning tracks
  TH1F* ntracks_precut;
  TH1F* ntracks_cut;
  TH1F* Tracks_chi2noriginal;
  TH1F* Tracks_vtxdz; 
  TH1F* Tracks_vtxdxy;
  TH1F* Tracks_vtxed0;
  TH1F* Tracks_dxy;
  TH1F* Tracks_dz;
  TH1F* Tracks_vtxedz;
  TH1F* Tracks_xyquality;
  TH1F* Tracks_zquality;
  TH1F* Tracks_2quality;
  TH1F* Tracks_Quality;
  
  TH1F* pt_reco;
  TH1F* eta_reco;
  TH1F* Tracks_charge;
  TH1F* Tracks_nhit;
  TH1F* Tracks_pixelhit;
  TH1F* Tracks_striphit;
  TH1F* Tracks_chi2n;
  TH1F* Tracks_phi;
  TH1F* test;
  TH1F* test2;*/
  
  TString UEcoll;
  
  virtual void init();
  //virtual void clean(vector<MyTracks>&, vector<MyVertex>&);
  //virtual void clean(vector<MyGenPart>&, bool);
  virtual void fill(vector<MyPart>&, double, double, double, double, double = 1., bool = false, bool = false);
  virtual void fill(vector<MyPart>&, vector<MyPart>&, double, double, double, double, double = 1., bool = false, bool = false);
  virtual void write();
  virtual bool GetTrEffFromFile(TString, TString, TString);
  virtual bool GetPtCorrFromFile(TString, TString, TString);
  virtual double GetFake(double, double);
  virtual double GetEff(double, double);
  virtual double GetWeight(double, double);
  virtual double GetCorrectedptratio(double);
  
  private:
  double* varbins;
  int nBins;
  //vector <int> goodtracks;
          
  ClassDef(UEPlots,1)

};
