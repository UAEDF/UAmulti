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
#include <algorithm>

//STANDARD C++ INCLUDES
#include <iostream>
#include <sstream>
using namespace std;

//OUR OWN CLASSES TO READ THE TREE
#include "../mydir/MassParticles.h"
#include "../mydir/MyBaseJet.h"
#include "../mydir/MyBeamSpot.h"
#include "../mydir/MyCaloJet.h"
#include "../mydir/MyCastorDigi.h"
#include "../mydir/MyCastorJet.h"
#include "../mydir/MyCastorRecHit.h"
#include "../mydir/MyDiJet.h"
#include "../mydir/MyElectron.h"
#include "../mydir/MyEvtId.h"
#include "../mydir/MyFwdGap.h"
#include "../mydir/MyGenJet.h"
#include "../mydir/MyGenKin.h"
#include "../mydir/MyGenMet.h"
#include "../mydir/MyGenPart.h"
#include "../mydir/MyHLTrig.h"
#include "../mydir/MyJet.h"
#include "../mydir/MyL1Trig.h"
#include "../mydir/MyL1TrigOld.h"
#include "../mydir/MyMITEvtSel.h"
#include "../mydir/MyMet.h"
#include "../mydir/MyMuon.h"
#include "../mydir/MyPFCand.h"
#include "../mydir/MyPFJet.h"
#include "../mydir/MyPUSumInfo.h"
#include "../mydir/MyPart.h"
#include "../mydir/MySimVertex.h"
#include "../mydir/MyTracks.h"
#include "../mydir/MyVertex.h"

#include "../plugins/UETrackPlots.h"
#include "../plugins/GenPartPlots.h"
#include "../plugins/VertexPlots.h"
#include "../plugins/UEPlots.h"
#include "../plugins/UEFactors.h"

#include "../plugins/TOperation.h"
#include "../plugins/EfficiencyPlots.h"

bool debug = false;

//#include "cuts.C"
#include "fileManager.C"
#include "files_funcs.C"
#include "FBacc.C"
//#include "FBcuts.C"
#include "UEcuts.C"
#include "NCHEvtSel.C"
#include "binningMap.C"

int pthatcut(TString);
//TH2F Get_nevt_pthat_FromFile(TString, TString);

void UnderlyingEvent2( bool isMC  = true, const Int_t nevt_max = 100000000, bool WeighEvt = true, bool WeighTr = true, bool PtCorrection = true,
	bool PtCorrSimple = true, bool JetMatchSelCorr = true, bool EvtSelCorr = true, bool isSiSCone = true)
{
	TH1::AddDirectory(0);
  
  //UE Analysis cuts and selections
	//Reference cut: pt_cut = 0.5, eta_cut = 2.0
	//Reference selection: ImpParSig = 3, VtxNDof = 4, isHighPurity = true
  pt_cut = 0.5;
  eta_cut = 2.0; 
  int ImpParSig = 3;
  int VtxNDof = 4;
  bool isHighPurity = true;
	//Correction factor for fake rates
  double FakeCorr = 1.;

  if ((ImpParSig == 3) && (VtxNDof == 4) && isHighPurity && (FakeCorr == 1.)) cout<< endl << "Nominal Cuts and Selections" << endl;
  else cout << endl << "Systematic Cuts and Selections" << endl;
  cout<< "Pt Cut: " << pt_cut << "   " << "Eta Cut: " << eta_cut << endl;
  cout<< "ImpParSig: " << ImpParSig << "   " << "VtxNDof: " << VtxNDof << "   " << "isHighPurity: " << isHighPurity << "   " << "FakeCorr: " << FakeCorr << endl << endl;

  //TH1F* track_pt_noEvtSel = new TH1F("track_pt_noEvtSel", "pt_noEvtSel;
  //TH1F*
  
  vector<double> ptBinning = makeBins(14, 0., 14.);
  addBins(5, 2., ptBinning);
  addBins(2, 4., ptBinning);
  addBins(4, 8., ptBinning);
  addBins(1, 36., ptBinning);
  
  vector<double> trackptBinning = makeBins(20,0.,1.);
  addBins(10,.2,trackptBinning);
  addBins(4,.5,trackptBinning);
  addBins(5,1,trackptBinning);
  addBins(1,50,trackptBinning);
  
  vector<double> etaBinning = makeBins(60,-3.,3.);
  vector<double> tracketaBinning = makeBins(12,-3.,-1.8);
  addBins(18, 0.2, tracketaBinning);
  addBins(12, 0.1, tracketaBinning);
  
    
  //Binning and Efficiency plots for Tracks  
  BasePlots Binning;
  Binning.pt_nbin = trackptBinning.size()-1;
  Binning.setBinning_pt(&(trackptBinning.at(0)));
  Binning.eta_nbin = tracketaBinning.size()-1;
  Binning.setBinning_eta(&(tracketaBinning.at(0)));
  
  //UETrackPlots  trp_all = UETrackPlots("all");
  UETrackPlots  trp_sel = UETrackPlots("sel");
  GenPartPlots gpp_sel = GenPartPlots("sel");
  VertexPlots vtx_sel = VertexPlots("sel");

  UETrackPlots  trp_bfcut = UETrackPlots("bfcut");
  UETrackPlots  trp_aftcut = UETrackPlots("aftcut");
  
  //EfficiencyPlots treffp_noEvtSel("TrNoEvtSel");
  //treffp_noEvtSel.dR_max = 0.5;
  EfficiencyPlots treffp_evtSel("TrEvtSel");
  treffp_evtSel.dR_max = 0.05;
  
  EfficiencyPlots treffp_evtSel_nocuts("TrEvtSel_nocuts");
  treffp_evtSel_nocuts.dR_max = 0.05;

  EfficiencyPlots treffp_evtSel_nocuts_LeadGenMatch("TrEvtSel_nocuts_LeadGenMatch");
  treffp_evtSel_nocuts_LeadGenMatch.dR_max = 0.05;
  EfficiencyPlots treffp_evtSel_nocuts_UnMatch("TrEvtSel_nocuts_UnMatch");
  treffp_evtSel_nocuts_UnMatch.dR_max = 0.05;
  EfficiencyPlots treffp_evtSel_nocuts_NonLeadGenMatch("TrEvtSel_nocuts_NonLeadGenMatch");
  treffp_evtSel_nocuts_NonLeadGenMatch.dR_max = 0.05;
  
  //Binning and efficiency plots for Jets
  Binning.pt_nbin = ptBinning.size()-1;
  Binning.setBinning_pt(&(ptBinning.at(0)));
  Binning.eta_nbin = etaBinning.size()-1;
  Binning.setBinning_eta(&(etaBinning.at(0)));
  
  //EfficiencyPlots jeteffp_noEvtSel("JetNoEvtSel");
  //jeteffp_noEvtSel.dR_max = 0.5;
  EfficiencyPlots jeteffp_evtSel("JetEvtSel");
  jeteffp_evtSel.dR_max = 0.02;
  
  EfficiencyPlots jeteffp_evtSel_nocuts("JetEvtSel_nocuts");
  jeteffp_evtSel_nocuts.dR_max = 0.02;
  
  EfficiencyPlots leadjeteffp_evtSel("LeadJetEvtSel");
  leadjeteffp_evtSel.dR_max = 0.02;
  
  //UEPlots
  UEPlots genpart1part05 = UEPlots("genpart1part05", ptBinning);
  UEPlots genjet1part05 = UEPlots("genjet1part05", ptBinning);
  UEPlots genjet1part05_hasRecoJet = UEPlots("genjet1part05_hasRecoJet", ptBinning);
  UEPlots matchedgenjet1part05 = UEPlots("matchedgenjet1part05", ptBinning);
  UEPlots genjet1part05_vtxsel = UEPlots("genjet1part05_vtxsel", ptBinning);
  UEPlots genjet1part05_minbias = UEPlots("genjet1part05_minbias", ptBinning);
  UEPlots genjet1part05_LeadGenMatch = UEPlots("genjet1part05_LeadGenMatch", ptBinning);
  UEPlots genjet1part05_NonLeadGenMatch = UEPlots("genjet1part05_NonLeadGenMatch", ptBinning);
  UEPlots genjet1part05_UnMatch = UEPlots("genjet1part05_UnMatch", ptBinning);
    
  UEPlots truegenjet1part05 = UEPlots("truegenjet1part05", ptBinning);
  UEPlots recojet1genpart05 = UEPlots("recojet1genpart05", ptBinning);
  UEPlots genjet1recopart05 = UEPlots("genjet1recopart05", ptBinning);
  UEPlots recojet1genpart05_LeadGenMatch = UEPlots("recojet1genpart05_LeadGenMatch", ptBinning);
  UEPlots genjet1recopart05_LeadGenMatch = UEPlots("genjet1recopart05_LeadGenMatch", ptBinning);
  UEPlots recojet1genpart05_NonLeadGenMatch = UEPlots("recojet1genpart05_NonLeadGenMatch", ptBinning);
  UEPlots genjet1recopart05_NonLeadGenMatch = UEPlots("genjet1recopart05_NonLeadGenMatch", ptBinning);
  //UEPlots truegenjet1part05_recofound = UEPlots("truegenjet1part05_recofound");
  //UEPlots genjet1part05_recofound = UEPlots("genjet1part05_recofound");
  
  UEPlots recopart1part05 = UEPlots("recopart1part05", ptBinning);
  UEPlots recojet1part05 = UEPlots("recojet1part05", ptBinning);
  UEPlots recojet1part05_LeadGenMatch = UEPlots("recojet1part05_LeadGenMatch", ptBinning);
  UEPlots recojet1part05_UnMatch = UEPlots("recojet1part05_UnMatch", ptBinning);
  UEPlots recojet1part05_NonLeadGenMatch = UEPlots("recojet1part05_NonLeadGenMatch", ptBinning);
  
  //Histograms for pthat cuts
  TH1F* pthat = new TH1F("pthat", "pthat;pthat;# evt", 200, 0, 100);
  TH2F* nevt_pthat = new TH2F("nevt_pthat", "nevt_pthat;bin_pthat;pthat_cut;# evt", 4, 1, 5, 4, 1, 5);

  ////Event selection and jetpt correction efficiencies
  //TH1D* profile_nch_trans_matchcorr = new TH1D("profile_nch_trans_matchcorr", "profile_nch_trans_matchcorr;Leading jet pt;<# ch>", ptBinning.size()-1, &(ptBinning.at(0)));
  //TH1D* profile_ptsum_trans_matchcorr = new TH1D("profile_ptsum_trans_matchcorr", "profile_ptsum_trans_matchcorr;Leading jet pt;<ptsum>", ptBinning.size()-1, &(ptBinning.at(0)));
  //TH1D* profile_nch_trans_ptcorrsimple = new TH1D("profile_nch_trans_ptcorrsimple", "profile_nch_trans_ptcorrsimple;Leading jet pt;<# ch>", ptBinning.size()-1, &(ptBinning.at(0)));
  //TH1D* profile_ptsum_trans_ptcorrsimple = new TH1D("profile_ptsum_trans_ptcorrsimple", "profile_ptsum_trans_ptcorrsimple;Leading jet pt;<ptsum>", ptBinning.size()-1, &(ptBinning.at(0)));
  //TH1D* profile_nch_trans_ptcorrsimple_evtselcorr = new TH1D("profile_nch_trans_ptcorrsimple_evtselcorr", "profile_nch_trans_ptcorrsimple_evtselcorr;Leading jet pt;<# ch>", ptBinning.size()-1, &(ptBinning.at(0)));
  //TH1D* profile_ptsum_trans_ptcorrsimple_evtselcorr = new TH1D("profile_ptsum_trans_ptcorrsimple_evtselcorr", "profile_ptsum_trans_ptcorrsimple_evtselcorr;Leading jet pt;<ptsum>", ptBinning.size()-1, &(ptBinning.at(0)));
  //TH1D* profile_nch_trans_matchcorr_evtselcorr = new TH1D("profile_nch_trans_matchcorr_evtselcorr", "profile_nch_trans_matchcorr_evtselcorr;Leading jet pt;<# ch>", ptBinning.size()-1, &(ptBinning.at(0)));
  //TH1D* profile_ptsum_trans_matchcorr_evtselcorr = new TH1D("profile_ptsum_trans_matchcorr_evtselcorr", "profile_ptsum_trans_matchcorr_evtselcorr;Leading jet pt;<ptsum>", ptBinning.size()-1, &(ptBinning.at(0)));
  //TH1D* profile_nch_trans_ptcorrsimpleclosure = new TH1D("profile_nch_trans_ptcorrsimpleclosure", "profile_nch_trans_ptcorrsimpleclosure;Leading jet pt;<# ch>", ptBinning.size()-1, &(ptBinning.at(0)));
  //TH1D* profile_ptsum_trans_ptcorrsimpleclosure = new TH1D("profile_ptsum_trans_ptcorrsimpleclosure", "profile_ptsum_trans_ptcorrsimpleclosure;Leading jet pt;<ptsum>", ptBinning.size()-1, &(ptBinning.at(0)));
  
  /*TH1D* nch_evtsel_eff2 = new TH1D();
  TH1D* ptsum_evtsel_eff2 = new TH1D();
  TH1D* nch_matchtolead_eff2 = new TH1D();
  TH1D* ptsum_matchtolead_eff2 = new TH1D();
  TH1D* jetpt_nch_corr_eff2 = new TH1D();
  TH1D* jetpt_ptsum_corr_eff2 = new TH1D();*/

  //Vertex plot
  TH1D* nGoodVtx = new TH1D("nGoodVtx", "nGoodVtx;nGoodVtx;# Event", 100, 0, 100);
  TH1D* nDOFat1GdVtx = new TH1D("nDOFat1GdVtx", "nDOFat1GdVtx;nDOFat1GdVtx;# Event", 100, 0, 100);

  UEFactors multcorr = UEFactors("nch", ptBinning);
  UEFactors ptsumcorr = UEFactors("ptsum", ptBinning);
  UEFactors multcorr_LeadGenMatch = UEFactors("nch_LeadGenMatch", ptBinning);
  UEFactors multcorr_UnMatch = UEFactors("nch_UnMatch", ptBinning);
  UEFactors multcorr_NonLeadGenMatch = UEFactors("nch_NonLeadGenMatch", ptBinning);

  int nEvt_aft_sel = 0;
  
  //looping once over MC and then once over Data
  //for (int mcdata = 1; mcdata <= 1; mcdata++)
  //{
      //getting the list of files
      vector<TString>* vfiles = new vector<TString>();
	  cout<< "Getting list of files..." << endl;
      vfiles = getListOfFiles("../filelist.txt");
      cout<< "File list stored." << endl;
      //Needs the fileManager.C from UAmulti
      //vector<TString>* vfiles = new vector<TString>(1,"rfio:/castor/cern.ch/user/x/xjanssen/data/CMSSW_4_1_2/UABaseTree_412_mc_v4/__MinBias_TuneZ2_2760GeV-pythia6__Spring11-START311_V2A-v2__GEN-SIM-RECODEBUG/UABaseTree_412_mc_v4__CMSSW_4_1_2__MinBias_TuneZ2_2760GeV-pythia6__Spring11-START311_V2A-v2__GEN-SIM-RECODEBUG_1_1_yV0.root"); 
      //vfiles->push_back("UABaseTree2.root"); 
      
      //Getting event, track and jetpt weights/correction factors
      TString sMCmodel = "";
      //sMCmodel = "d6t";
      sMCmodel = "z2";
      
      //TString effdir = "../../../../../efficiencies/";
      //TString effdir = "dcap:///pnfs/iihe/cms/store/user/wywang/";
	  //TString effdir = "/localgrid/wywang/";
	  TString effdir = "";

	  TString fileSISCone, fileAk, fileEffSISCone, fileEffAk;

	  if (!isHighPurity)
	  {
		  fileSISCone = effdir + "efficiencies_siscone" + sMCmodel + "_purity.root";
		  fileAk = effdir + "efficiencies_ak" + sMCmodel + "_purity.root";
		  fileEffSISCone = effdir + "efficiencies_effsiscone" + sMCmodel + "_purity.root";
		  fileEffAk = effdir + "efficiencies_effak" + sMCmodel + "_purity.root";

		  cout<< "\n Efficiency file list for Track Selection Systematic:" << endl;
		  cout << fileSISCone << endl << fileAk << endl << fileEffSISCone << endl << fileEffAk << endl << endl;
	  }
	  if (VtxNDof != 4)
	  {
		  fileSISCone = effdir + "efficiencies_siscone" + sMCmodel + "_ndof6.root";
		  fileAk = effdir + "efficiencies_ak" + sMCmodel + "_ndof6.root";
		  fileEffSISCone = effdir + "efficiencies_effsiscone" + sMCmodel + "_ndof6.root";
		  fileEffAk = effdir + "efficiencies_effak" + sMCmodel + "_ndof6.root";

		  cout<< "\n Efficiency file list for Vertex NDOF Systematic:" << endl;
		  cout << fileSISCone << endl << fileAk << endl << fileEffSISCone << endl << fileEffAk << endl << endl;
	  }
	  if (ImpParSig != 3)
	  {
		  fileSISCone = effdir + "efficiencies_siscone" + sMCmodel + "_imp2.root";
		  fileAk = effdir + "efficiencies_ak" + sMCmodel + "_imp2.root";
		  fileEffSISCone = effdir + "efficiencies_effsiscone" + sMCmodel + "_imp2.root";
		  fileEffAk = effdir + "efficiencies_effak" + sMCmodel + "_imp2.root";

		  cout<< "\n Efficiency file list for Impact Parameter Significance Systematic:" << endl;
		  cout << fileSISCone << endl << fileAk << endl << fileEffSISCone << endl << fileEffAk << endl << endl;
	  }
	  if (isHighPurity && (VtxNDof == 4) && (ImpParSig == 3))
	  {
		  fileSISCone = effdir + "efficiencies_siscone" + sMCmodel + "_11.root";
		  fileAk = effdir + "efficiencies_ak" + sMCmodel + "_9.root";
		  fileEffSISCone = effdir + "efficiencies_effsiscone" + sMCmodel + ".root";
		  fileEffAk = effdir + "efficiencies_effak" + sMCmodel + ".root";

		  cout<< "\n Efficiency file list for Nominal cuts:" << endl;
		  cout << fileSISCone << endl << fileAk << endl << fileEffSISCone << endl << fileEffAk << endl << endl;
	  }
	  
	  if (WeighEvt && isMC)
	  {          
		  cout<< "Reading file: " << effdir + "efficiencies_siscone" + sMCmodel + ".root" << endl;
		  //nevt_pthat = &(Get_nevt_pthat_FromFile(effdir + "efficiencies_siscone" + sMCmodel + ".root", "nevt_pthat"));
		  TFile* f = TFile::Open(effdir + "efficiencies_siscone" + sMCmodel + ".root" , "READ");
		  if(f==0)
		  {
			  cout << "[UnderlyingEvent2.cc] WARNING !! Can't open file!" << endl;
		  } 
      
		  nevt_pthat = (TH2F*) f->Get("nevt_pthat");
		  if(nevt_pthat->GetEntries()==0)
		  {
			  cout << "[UnderlyingEvent2.cc] WARNING !! Couldn't fetch ptcorr " << "nevt_pthat" << " from file "<< effdir + "efficiencies_siscone" + sMCmodel + ".root" << "." << endl;
		  }				

		  f->Close();
		  delete f;
		  cout<< "Event count obtained." << endl;
	  }
	 
	  /*if (PtCorrSimple || JetMatchSelCorr || EvtSelCorr)
	  {
		  TString filename;
		  if (isSiSCone) filename = fileSISCone;
		  else filename = fileAk;

		  cout<< "Opening file: " << filename << endl;
		  TFile* f = TFile::Open(filename , "READ");
		  if(f==0)
		  {
			  cout << "[UnderlyingEvent2.cc] WARNING !! Can't open file!" << endl;
		  } 

		  cout<< "Reading Event level correction efficiencies..." << endl;
		  if (JetMatchSelCorr)
		  {
			  nch_matchtolead_eff2 = &(((TOperation<TH1D>*) f->Get("TOperation_class_nch_matchtolead_eff"))->result);
			  ptsum_matchtolead_eff2 = &(((TOperation<TH1D>*) f->Get("TOperation_class_ptsum_matchtolead_eff"))->result);
		  }
		  if (PtCorrSimple)
		  {
			  jetpt_nch_corr_eff2 = &(((TOperation<TH1D>*) f->Get("TOperation_class_jetpt_nch_corr_eff"))->result);
			  jetpt_ptsum_corr_eff2 = &(((TOperation<TH1D>*) f->Get("TOperation_class_jetpt_ptsum_corr_eff"))->result);
		  }
		  if (EvtSelCorr)
		  {
			  nch_evtsel_eff2 = &(((TOperation<TH1D>*) f->Get("TOperation_class_nch_evtsel_eff"))->result);
			  ptsum_evtsel_eff2 = &(((TOperation<TH1D>*) f->Get("TOperation_class_ptsum_evtsel_eff"))->result);
		  }
		  cout<< "Event level correction efficiencies obtained." << endl;

		  f->Close();
		  delete f;
		  cout<< "File closed." << endl;
	  }*/
      //else
      //   nevt_pthat = new TH2F("nevt_pthat", "nevt_pthat;bin_pthat;pthat_cut;# evt", 4, 1, 5, 4, 1, 5);
      /*
      TString sPtHatCut = "pt0";
      switch(pthatcut(vfiles->at(0)))
      {
          case 1:
               sPtHatCut = "pt15";
               break;
          case 2:
               sPtHatCut = "pt50";
               break;
          case 3:        
               sPtHatCut = "pt80";  
               break;                                   
      }
      */
      if (WeighTr) 
      {
		 cout<< "Storing track efficiencies and fakes to UEPlots..." << endl;
         //if (!recopart1part05.GetTrEffFromFile(fileSISCone, "EfficiencyPlots_TrEvtSel_nocuts/TOperation_class_pt_eta_eff_TrEvtSel_nocuts", "EfficiencyPlots_TrEvtSel_nocuts/TOperation_class_pt_eta_fake_TrEvtSel_nocuts", FakeCorr)) return;
         if (!recojet1part05.GetTrEffFromFile(fileSISCone, "EfficiencyPlots_TrEvtSel_nocuts/TOperation_class_pt_eta_eff_TrEvtSel_nocuts", "EfficiencyPlots_TrEvtSel_nocuts/TOperation_class_pt_eta_fake_TrEvtSel_nocuts", FakeCorr)) return;
         if (!recojet1part05_LeadGenMatch.GetTrEffFromFile(fileSISCone, "EfficiencyPlots_TrEvtSel_nocuts_LeadGenMatch/TOperation_class_pt_eta_eff_TrEvtSel_nocuts_LeadGenMatch", "EfficiencyPlots_TrEvtSel_nocuts_LeadGenMatch/TOperation_class_pt_eta_fake_TrEvtSel_nocuts_LeadGenMatch", FakeCorr)) return;
         if (!recojet1part05_UnMatch.GetTrEffFromFile(fileSISCone, "EfficiencyPlots_TrEvtSel_nocuts_UnMatch/TOperation_class_pt_eta_eff_TrEvtSel_nocuts_UnMatch", "EfficiencyPlots_TrEvtSel_nocuts_UnMatch/TOperation_class_pt_eta_fake_TrEvtSel_nocuts_UnMatch", FakeCorr)) return;
		 if (!recojet1part05_NonLeadGenMatch.GetTrEffFromFile(fileSISCone, "EfficiencyPlots_TrEvtSel_nocuts_NonLeadGenMatch/TOperation_class_pt_eta_eff_TrEvtSel_nocuts_NonLeadGenMatch", "EfficiencyPlots_TrEvtSel_nocuts_NonLeadGenMatch/TOperation_class_pt_eta_fake_TrEvtSel_nocuts_NonLeadGenMatch", FakeCorr)) return;
		 if (!genjet1recopart05.GetTrEffFromFile(fileSISCone, "EfficiencyPlots_TrEvtSel_nocuts/TOperation_class_pt_eta_eff_TrEvtSel_nocuts", "EfficiencyPlots_TrEvtSel_nocuts/TOperation_class_pt_eta_fake_TrEvtSel_nocuts", FakeCorr)) return;
		 if (!genjet1recopart05_LeadGenMatch.GetTrEffFromFile(fileSISCone, "EfficiencyPlots_TrEvtSel_nocuts_LeadGenMatch/TOperation_class_pt_eta_eff_TrEvtSel_nocuts_LeadGenMatch", "EfficiencyPlots_TrEvtSel_nocuts_LeadGenMatch/TOperation_class_pt_eta_fake_TrEvtSel_nocuts_LeadGenMatch", FakeCorr)) return;
		 if (!genjet1recopart05_NonLeadGenMatch.GetTrEffFromFile(fileSISCone, "EfficiencyPlots_TrEvtSel_nocuts_NonLeadGenMatch/TOperation_class_pt_eta_eff_TrEvtSel_nocuts_NonLeadGenMatch", "EfficiencyPlots_TrEvtSel_nocuts_NonLeadGenMatch/TOperation_class_pt_eta_fake_TrEvtSel_nocuts_NonLeadGenMatch", FakeCorr)) return;
		 cout<< "Efficiencies and fakes stored." << endl;
	  }
      
      if (PtCorrection) 
      {
		 cout<< "Storing JetPt Correction factor to UEPlots..." << endl;
         if (isSiSCone)
         {
			 if (!recojet1genpart05.GetPtCorrFromFile(fileSISCone, "UEFactors_<nch>/pt_corr", "UEFactors_<nch>/Profile_PtTrackJet_PtGenJet_matched")) return;
			if (!recojet1genpart05_LeadGenMatch.GetPtCorrFromFile(fileSISCone, "UEFactors_<nch_LeadGenMatch>/pt_corr", "UEFactors_<nch_LeadGenMatch>/Profile_PtTrackJet_PtGenJet_matched")) return;
			if (!recojet1genpart05_NonLeadGenMatch.GetPtCorrFromFile(fileSISCone, "UEFactors_<nch_NonLeadGenMatch>/pt_corr", "UEFactors_<nch_NonLeadGenMatch>/Profile_PtTrackJet_PtGenJet_matched")) return;
            if (!recojet1part05.GetPtCorrFromFile(fileSISCone, "UEFactors_<nch>/pt_corr", "UEFactors_<nch>/Profile_PtTrackJet_PtGenJet_matched")) return;
            if (!recojet1part05_LeadGenMatch.GetPtCorrFromFile(fileSISCone, "UEFactors_<nch_LeadGenMatch>/pt_corr", "UEFactors_<nch_LeadGenMatch>/Profile_PtTrackJet_PtGenJet_matched")) return;
            if (!recojet1part05_UnMatch.GetPtCorrFromFile(fileSISCone, "UEFactors_<nch_LeadGenMatch>/pt_corr", "UEFactors_<nch_UnMatch>/Profile_PtTrackJet_PtGenJet_unmatched")) return;
			if (!recojet1part05_NonLeadGenMatch.GetPtCorrFromFile(fileSISCone, "UEFactors_<nch_NonLeadGenMatch>/pt_corr", "UEFactors_<nch_NonLeadGenMatch>/Profile_PtTrackJet_PtGenJet_matched")) return;
         }
         else
         {
            if (!recojet1genpart05.GetPtCorrFromFile(fileAk, "UEFactors_<nch>/pt_corr", "UEFactors_<nch>/Profile_PtTrackJet_PtGenJet_matched")) return;
			if (!recojet1genpart05_LeadGenMatch.GetPtCorrFromFile(fileAk, "UEFactors_<nch_LeadGenMatch>/pt_corr", "UEFactors_<nch_LeadGenMatch>/Profile_PtTrackJet_PtGenJet_matched")) return;
			if (!recojet1genpart05_NonLeadGenMatch.GetPtCorrFromFile(fileAk, "UEFactors_<nch_NonLeadGenMatch>/pt_corr", "UEFactors_<nch_NonLeadGenMatch>/Profile_PtTrackJet_PtGenJet_matched")) return;
            if (!recojet1part05.GetPtCorrFromFile(fileAk, "UEFactors_<nch>/pt_corr", "UEFactors_<nch>/Profile_PtTrackJet_PtGenJet_matched")) return;
            if (!recojet1part05_LeadGenMatch.GetPtCorrFromFile(fileAk, "UEFactors_<nch_LeadGenMatch>/pt_corr", "UEFactors_<nch_LeadGenMatch>/Profile_PtTrackJet_PtGenJet_matched")) return;
            if (!recojet1part05_UnMatch.GetPtCorrFromFile(fileAk, "UEFactors_<nch_LeadGenMatch>/pt_corr", "UEFactors_<nch_UnMatch>/Profile_PtTrackJet_PtGenJet_unmatched")) return;
			if (!recojet1part05_NonLeadGenMatch.GetPtCorrFromFile(fileAk, "UEFactors_<nch_NonLeadGenMatch>/pt_corr", "UEFactors_<nch_NonLeadGenMatch>/Profile_PtTrackJet_PtGenJet_matched")) return;
         }  
		 cout<< "Correction factor stored." << endl;
      }

	  if (PtCorrSimple)
	  {
		  if (isSiSCone)
          {
			  recojet1genpart05.GetSimplePtCorrEffFromFile(fileEffSISCone, "jetpt_nch_corr_eff", "jetpt_ptsum_corr_eff");
			  recojet1part05.GetSimplePtCorrEffFromFile(fileEffSISCone, "jetpt_nch_corr_eff", "jetpt_ptsum_corr_eff");
		  }
		  else
		  {
			  recojet1genpart05.GetSimplePtCorrEffFromFile(fileEffAk, "jetpt_nch_corr_eff", "jetpt_ptsum_corr_eff");
			  recojet1part05.GetSimplePtCorrEffFromFile(fileEffAk, "jetpt_nch_corr_eff", "jetpt_ptsum_corr_eff");
		  }
	  }

	  if (JetMatchSelCorr)
	  {
		  if (isSiSCone)
          {
			  recojet1genpart05.GetMatchtoLeadEffFromFile(fileEffSISCone, "nch_matchtolead_eff", "ptsum_matchtolead_eff");
			  recojet1part05.GetMatchtoLeadEffFromFile(fileEffSISCone, "nch_matchtolead_eff", "ptsum_matchtolead_eff");
		  }
		  else
		  {
			  recojet1genpart05.GetMatchtoLeadEffFromFile(fileEffAk, "nch_matchtolead_eff", "ptsum_matchtolead_eff");
			  recojet1part05.GetMatchtoLeadEffFromFile(fileEffAk, "nch_matchtolead_eff", "ptsum_matchtolead_eff");
		  }
	  }

	  if (EvtSelCorr)
	  {
		  if (isSiSCone)
		  {
			  recojet1genpart05.GetEvtSelEffFromFile(fileEffSISCone, "nch_evtsel_eff", "ptsum_evtsel_eff");
			  genjet1recopart05.GetEvtSelEffFromFile(fileEffSISCone, "nch_evtsel_eff", "ptsum_evtsel_eff");
			  recojet1part05.GetEvtSelEffFromFile(fileEffSISCone, "nch_evtsel_eff", "ptsum_evtsel_eff");
		  }
		  else
		  {
			  recojet1genpart05.GetEvtSelEffFromFile(fileEffAk, "nch_evtsel_eff", "ptsum_evtsel_eff");
			  genjet1recopart05.GetEvtSelEffFromFile(fileEffAk, "nch_evtsel_eff", "ptsum_evtsel_eff");
			  recojet1part05.GetEvtSelEffFromFile(fileEffAk, "nch_evtsel_eff", "ptsum_evtsel_eff");
		  }
	  }
	  
      //Declaration of tree and its branches variables
      TTree* tree = new TTree("evt","");
      MyEvtId*           evtId        = NULL;
      MyHLTrig*  HLTrig               = NULL;
      MyL1Trig*  L1Trig               = NULL;
      vector<MyGenPart>* genPart      = NULL;
      vector<MyTracks>*  tracks       = NULL;
      vector<MyVertex>*  vertex       = NULL;
      MyBeamSpot*        beamSpot     = NULL;
      vector<MyBaseJet>* jet          = NULL;
      vector<MyBaseJet>* genjet       = NULL;
      vector<MyPart>* generatedpart   = new vector<MyPart>;
      vector<MyPart>* reconpart       = new vector<MyPart>;
      vector<MyPart>* reconjet        = new vector<MyPart>;
      vector<MyPart>* generatedjet    = new vector<MyPart>;
	  //vector<MyPart>* generatedjet_nocuts    = new vector<MyPart>;
      MyGenKin* genkin                = NULL;
      
	  //vector<MyVertex>* vtx_coll    = NULL;

      // etc ....
      //Put All Classes you want to read here !!
      
      double EvtWeight;
      int i_tot = 0 , nevt_tot = 0;
      //int nevt15 = 0, nevt50 = 0, nevt80 = 0;
      
      //starting Loop over files, stops at end of list of files or when reached nevt_max
      for(vector<TString>::iterator itfiles = vfiles->begin() ; itfiles != vfiles->end() && (i_tot < nevt_max||nevt_max==-1) ; ++itfiles){
      //if (i_tot == 0)
      //   for (int i = 0; i < 27; i++)
      //       cout<< multcorr.varbins[i] << endl;
		cout<< "Opening new file." << endl;
        TFile* file = TFile::Open(*itfiles,"READ");
     
        //getting the tree from the current file
	    cout<< "Getting tree from file." << endl;
        tree = (TTree*) file->Get("evt");

        //adding branches to the tree ----------------------------------------------------------------------
        tree->SetBranchStatus("*", 1);
        
        tree->SetBranchStatus("evtId", 1);
        tree->SetBranchAddress("evtId",&evtId);
        tree->SetBranchStatus("generalTracks", 1);//WeighEvt);
        tree->SetBranchAddress("generalTracks",&tracks); 
        tree->SetBranchStatus("offlinePrimaryVertices", 1);//WeighEvt);
        tree->SetBranchAddress("offlinePrimaryVertices",&vertex);
        tree->SetBranchStatus("HLTrig", 1);//WeighEvt);
        tree->SetBranchAddress("HLTrig",&HLTrig);
        tree->SetBranchStatus("L1Trig", 1);//WeighEvt);
        tree->SetBranchAddress("L1Trig",&L1Trig);
        tree->SetBranchStatus("offlineBeamSpot", 1);//WeighEvt);
        tree->SetBranchAddress("offlineBeamSpot",&beamSpot);
        if (isSiSCone)
        {
           tree->SetBranchStatus("ueSisCone5TracksJet500", 1);//WeighEvt);
           tree->SetBranchAddress("ueSisCone5TracksJet500",&jet);
        }
        else
        {
           tree->SetBranchStatus("ueAk5TracksJet500", 1);//WeighEvt);
           tree->SetBranchAddress("ueAk5TracksJet500",&jet);
        }
    
        if(isMC) { 
            tree->SetBranchStatus("genPart", 1);//WeighEvt);
            tree->SetBranchAddress("genPart",&genPart);
            if (isSiSCone)
            {
               tree->SetBranchStatus("ueSisCone5ChgGenJet500", 1);//WeighEvt);
               tree->SetBranchAddress("ueSisCone5ChgGenJet500",&genjet);
            } 
            else
            {
               tree->SetBranchStatus("ueAk5ChgGenJet500", 1);//WeighEvt);
               tree->SetBranchAddress("ueAk5ChgGenJet500",&genjet);
            }  
            tree->SetBranchStatus("genKin", 1);
            tree->SetBranchAddress("genKin", &genkin);     
        }
		cout<< "All branches set." << endl;
        
        //Calculating EvtWeight
		cout<< "Calculating Event Weight." << endl;
        EvtWeight = 1.;
		int iPtHatCut = 0;
		if (isMC)
		{
			iPtHatCut = pthatcut(*itfiles);
			if (WeighEvt)
			{
			  if (iPtHatCut == 0) 
				 EvtWeight = 1;
			  else
			  {
				 int it_iPtHatCut = iPtHatCut;
				 while(it_iPtHatCut != 0)
				 {
					EvtWeight = EvtWeight*nevt_pthat->GetBinContent(it_iPtHatCut, it_iPtHatCut)/nevt_pthat->GetBinContent(it_iPtHatCut, it_iPtHatCut+1); 
					//cout<< "pthatsmallevtno: " << nevt_pthat->GetBinContent(it_iPtHatCut, it_iPtHatCut) << " pthatbigevtno: " << nevt_pthat->GetBinContent(it_iPtHatCut, it_iPtHatCut+1) << endl;
					--it_iPtHatCut;
				 }
			  } 
			  //cout<< "EvtWeight: " << EvtWeight << endl;                                
			}
		}
		cout<< "Event Weight calculated." << endl;

        //Getting number of events
        int nev = int(tree->GetEntriesFast());
        nevt_tot += nev;
        cout <<"The current file has " << nev << " entries : "<< endl << *itfiles << endl;
             
        //starting loop over events, stops when reached end of file or nevt_max
        for(int i = 0; i < nev && (i_tot < nevt_max||nevt_max==-1); ++i , ++i_tot){
          //cout<< "Event Number: " << i_tot << endl;
          //printing the number of events done every hundred events
          //if( ((i_tot+1) % 100) == 0) cout <<int(double(i_tot+1))<<" events done"<<endl;
          //printing the % of events done every 10k evts
          if( ((i_tot+1) % 10000) == 0) cout <<int(double(i_tot+1)/1000)<<"k done"<<endl;
          
          //Filling the variables defined setting branches
          tree->GetEntry(i);
          
          if (isMC)
          {  
             if (!WeighEvt)
             {
                 if(genkin->PtHat >= 15) nevt_pthat->Fill(1, iPtHatCut+1);
                 if(genkin->PtHat >= 50) nevt_pthat->Fill(2, iPtHatCut+1);
                 if(genkin->PtHat >= 80) nevt_pthat->Fill(3, iPtHatCut+1);
                 continue;
             }
             else
             {
                 // Event Selection based on pthat cuts
                 if((genkin->PtHat >= 15) && (genkin->PtHat < 50) && (iPtHatCut != 1)) continue;
                 if((genkin->PtHat >= 50) && (genkin->PtHat < 80) && (iPtHatCut != 2)) continue;
                 if((genkin->PtHat >= 80) && (iPtHatCut != 3)) continue;   
             }
             pthat->Fill(genkin->PtHat, EvtWeight);
          }

		  //vertex vector with selected vertices
		  //vector<MyVertex>* vtxcoll = NULL;
          
          //... Get Vertex Info
          //bool hasvertex=false;
          int nvertex=0; 
          int vertexid=-1; 
		  //if (vertex->size() == 1){
          for(vector<MyVertex>::iterator v = vertex->begin() ; v != vertex->end() ; ++v)
          {
            if ( v->ndof <= VtxNDof) continue;
            if ( fabs(v->z - beamSpot->z) >= 10 ) continue;                             
            if (!v->fake)
            {
              //hasvertex=true;
    	      ++nvertex;
    	      vertexid=(v->id);
			  //*vtx_coll = *v;
            }			 
          }
          nGoodVtx->Fill(nvertex, EvtWeight);
		  if (nvertex == 1) 
			  for(vector<MyVertex>::iterator v = vertex->begin() ; v != vertex->end() ; ++v)
				  if (v->id == vertexid)
					  nDOFat1GdVtx->Fill(v->ndof, EvtWeight);
		  //}

          //... Get Trigger Info
          bool L1_veto = (      !L1Trig->fTechDecisionBefore[36]
                           && !L1Trig->fTechDecisionBefore[37]
                           && !L1Trig->fTechDecisionBefore[38]
                           && !L1Trig->fTechDecisionBefore[39] );
          bool L1_BSC  = ( (L1Trig->fPhysDecisionBefore[126] && !isMC) ||
                         (L1Trig->fTechDecisionBefore[34] && isMC) ) ;
          bool HLT_BSC = ((HLTrig->HLTmap["HLT_L1BscMinBiasORBptxPlusANDMinus_v1"] && !isMC) || isMC ) ;
          bool HLT_ZeroBias = (isMC || HLTrig->HLTmap["HLT_ZeroBias_v1"]);
    
          bool TRG_MinBias =(L1_veto && L1_BSC && HLT_BSC);
          bool TRG_ZeroBias=(HLT_ZeroBias);
          
          //Reducing the size of the track, genpart collection
          vector<MyTracks> trcoll = *tracks;
		  trp_bfcut.fill(trcoll,(vector<MyVertex>::iterator&) *vertex,vertexid,beamSpot,EvtWeight);

          getPrimaryTracks(trcoll, vertexid, vertex, ImpParSig, pt_cut, eta_cut, 0, isHighPurity);

		  trp_aftcut.fill(trcoll,(vector<MyVertex>::iterator&) *vertex,vertexid,beamSpot,EvtWeight);
          
          vector<MyTracks> trcoll_nocuts = *tracks;
          getPrimaryTracks(trcoll_nocuts, vertexid, vertex, ImpParSig, (double) 0, (double) 3, 0, isHighPurity);
          
          vector<MyBaseJet> jetcoll = *jet;
          getPrimaryJets(jetcoll, pt_cut+0.5, eta_cut);
          
          vector<MyBaseJet> jetcoll_nocuts = *jet;
          getPrimaryJets(jetcoll_nocuts, (double) 0, (double) 3);
          
          //Filling GenPart and GenJet before selection
          vector<MyGenPart> gpcoll;
          vector<MyGenPart> gpcoll_nocuts;
          vector<MyBaseJet> genjetcoll;
          vector<MyBaseJet> genjetcoll_nocuts;
          
          MyPart pp;
          MyPart jp;
             
          if (isMC)
          {
             gpcoll = *genPart;
             gpcoll_nocuts = *genPart;
             genjetcoll = *genjet;
             genjetcoll_nocuts = *genjet;
             
             getPrimaryGenPart(gpcoll, pt_cut, eta_cut);
             getPrimaryGenPart(gpcoll_nocuts, (double) 0, (double) 3);
             getPrimaryJets(genjetcoll, pt_cut+0.5, eta_cut);
             getPrimaryJets(genjetcoll_nocuts, (double) 0, (double) 3);

             //truegenjet1part05_recofound.clean(*genPart, true);
             
             generatedpart->clear();
             generatedpart->reserve(gpcoll.size());
             for(vector<MyGenPart>::iterator it_tr = gpcoll.begin() ; it_tr != gpcoll.end() ; ++it_tr){
               
               pp.charge = it_tr->charge;
               pp.SetPxPyPzE(it_tr->Px(), it_tr->Py(), it_tr->Pz(), it_tr->E());
               generatedpart->push_back(pp);                             
             }
             
             generatedjet->clear();
             generatedjet->reserve(genjetcoll.size());
             for(vector<MyBaseJet>::iterator it_jet = genjetcoll.begin() ; it_jet != genjetcoll.end() ; ++it_jet){

               jp.SetPxPyPzE(it_jet->Px(), it_jet->Py(), it_jet->Pz(), it_jet->E());
               generatedjet->push_back(jp);                             
             }
			 /*
			 generatedjet_nocuts->clear();
             generatedjet_nocuts->reserve(genjetcoll_nocuts.size());
             for(vector<MyBaseJet>::iterator it_jet = genjetcoll_nocuts.begin() ; it_jet != genjetcoll_nocuts.end() ; ++it_jet){

               jp.SetPxPyPzE(it_jet->Px(), it_jet->Py(), it_jet->Pz(), it_jet->E());
               generatedjet_nocuts->push_back(jp);                             
             }
             */
             //treffp_noEvtSel.fill(&gpcoll, &trcoll, EvtWeight);
             //jeteffp_noEvtSel.fill(&getLeading(jetcoll), &genjetcoll, EvtWeight);
             truegenjet1part05.fill(*generatedjet, *generatedpart, pt_cut+0.5, eta_cut, pt_cut, eta_cut, EvtWeight);
             
             // Fill Gen level UEPlots with different event selection conditions
             if (nvertex==1) genjet1part05_vtxsel.fill(*generatedjet, *generatedpart, pt_cut+0.5, eta_cut, pt_cut, eta_cut, EvtWeight);
             if (TRG_MinBias) genjet1part05_minbias.fill(*generatedjet, *generatedpart, pt_cut+0.5, eta_cut, pt_cut, eta_cut, EvtWeight);
            
             //truegenjet1part05_recofound.fill(*generatedjet, *generatedpart, 1, 2.0, 0.5, 2.5);
          }
          
          // ... Event Selection
          
          if ( ! TRG_MinBias ) continue;
          if ( nvertex!=1    ) continue;
          
          //if ( !passVtx(*vertex)) continue;
          //if ( (vertex->at(0)).ndof < 5) continue;
          //if ( fabs((vertex->at(0)).z - beamSpot->z) > 9 ) continue;
          
          nEvt_aft_sel++;
          //cout<< nEvt_aft_sel << endl;
    
          //-------------------------------------------------------------------------------------------------
          //filling pt distribution for the generated particles
          //ie those from pythia generator, without reconstruction
         
          reconpart->clear();
          reconpart->reserve(trcoll.size());
          for(vector<MyTracks>::iterator it_tr = trcoll.begin() ; it_tr != trcoll.end() ; ++it_tr){
        
             pp.charge = it_tr->charge;
             pp.SetPxPyPzE(it_tr->Px(), it_tr->Py(), it_tr->Pz(), it_tr->E());
             reconpart->push_back(pp);                             
          }  
        
          reconjet->clear();
          reconjet->reserve(jetcoll.size());
          for(vector<MyBaseJet>::iterator it_jet = jetcoll.begin() ; it_jet != jetcoll.end() ; ++it_jet){
         
             jp.SetPxPyPzE(it_jet->Px(), it_jet->Py(), it_jet->Pz(), it_jet->E());
             reconjet->push_back(jp);                             
          }
          
          if(isMC)
          { 
            //genpart1part05.fill(*generatedpart, 1, 2.4, 0.5, 2.4);
            
            treffp_evtSel.fill(&gpcoll, &trcoll, EvtWeight);
            treffp_evtSel_nocuts.fill(&gpcoll_nocuts, &trcoll_nocuts, EvtWeight);
            jeteffp_evtSel.fill(&getLeading(jetcoll) , &genjetcoll, EvtWeight);
            jeteffp_evtSel_nocuts.fill(&getLeading(jetcoll_nocuts), &genjetcoll_nocuts, EvtWeight);
            leadjeteffp_evtSel.fill(&getLeading(genjetcoll), &getLeading(jetcoll), EvtWeight);
            genjet1part05.fill(*generatedjet, *generatedpart, pt_cut+0.5, eta_cut, pt_cut, eta_cut, EvtWeight);
			if (!reconjet->empty())
				genjet1part05_hasRecoJet.fill(*generatedjet, *generatedpart, pt_cut+0.5, eta_cut, pt_cut, eta_cut, EvtWeight);

			vector<MyPart>* matchedgenjet = new vector<MyPart>;
			*matchedgenjet = getMatched(*reconjet, *generatedjet, 0.3);
			matchedgenjet1part05.fill(*matchedgenjet, *generatedpart, pt_cut+0.5, eta_cut, pt_cut, eta_cut, EvtWeight);
                    
			//cout<< "recojet1genpart05:" << endl;
            recojet1genpart05.fill(*reconjet, *generatedpart, pt_cut+0.5, eta_cut, pt_cut, eta_cut, EvtWeight, false, PtCorrection, PtCorrSimple, JetMatchSelCorr, EvtSelCorr);
            genjet1recopart05.fill(*generatedjet, *reconpart, pt_cut+0.5, eta_cut, pt_cut, eta_cut, EvtWeight, WeighTr, false, false, false, EvtSelCorr);
            
            if (isMatchedLeading(*reconjet, *generatedjet, 0.3))
			{
			   //cout<< "recojet1part05_LeadGenMatch:" << endl;
			   genjet1part05_LeadGenMatch.fill(*generatedjet, *generatedpart, pt_cut+0.5, eta_cut, pt_cut, eta_cut, EvtWeight);
               recojet1part05_LeadGenMatch.fill(*reconjet, *reconpart, *generatedjet, pt_cut+0.5, eta_cut, pt_cut, eta_cut, EvtWeight, WeighTr, PtCorrection);
			   recojet1genpart05_LeadGenMatch.fill(*reconjet, *generatedpart, pt_cut+0.5, eta_cut, pt_cut, eta_cut, EvtWeight, false, PtCorrection);
			   genjet1recopart05_LeadGenMatch.fill(*generatedjet, *reconpart, pt_cut+0.5, eta_cut, pt_cut, eta_cut, EvtWeight, WeighTr);
			   treffp_evtSel_nocuts_LeadGenMatch.fill(&gpcoll_nocuts, &trcoll_nocuts, EvtWeight);
			   multcorr_LeadGenMatch.fill(genjetcoll, jetcoll, pt_cut+0.5, eta_cut, EvtWeight);
			}
            else 
			{
				if (isMatched(*reconjet, *generatedjet, 0.3))
				{
					//cout<< "recojet1part05_NonLeadGenMatch:" << endl;
				   genjet1part05_NonLeadGenMatch.fill(*generatedjet, *generatedpart, pt_cut+0.5, eta_cut, pt_cut, eta_cut, EvtWeight);
				   recojet1part05_NonLeadGenMatch.fill(*reconjet, *reconpart, *generatedjet, pt_cut+0.5, eta_cut, pt_cut, eta_cut, EvtWeight, WeighTr, PtCorrection);
				   recojet1genpart05_NonLeadGenMatch.fill(*reconjet, *generatedpart, pt_cut+0.5, eta_cut, pt_cut, eta_cut, EvtWeight, false, PtCorrection);
				   genjet1recopart05_NonLeadGenMatch.fill(*generatedjet, *reconpart, pt_cut+0.5, eta_cut, pt_cut, eta_cut, EvtWeight, WeighTr);
				   treffp_evtSel_nocuts_NonLeadGenMatch.fill(&gpcoll_nocuts, &trcoll_nocuts, EvtWeight);
				   multcorr_NonLeadGenMatch.fill(genjetcoll, jetcoll, pt_cut+0.5, eta_cut, EvtWeight);
				}
				else
				{
				   //cout<< "recojet1part05_UnMatch:" << endl;
				   genjet1part05_UnMatch.fill(*generatedjet, *generatedpart, pt_cut+0.5, eta_cut, pt_cut, eta_cut, EvtWeight);
				   recojet1part05_UnMatch.fill(*reconjet, *reconpart, *generatedjet, pt_cut+0.5, eta_cut, pt_cut, eta_cut, EvtWeight, WeighTr, PtCorrection);
				   treffp_evtSel_nocuts_UnMatch.fill(&gpcoll_nocuts, &trcoll_nocuts, EvtWeight);
				   multcorr_UnMatch.fill(genjetcoll, jetcoll, pt_cut+0.5, eta_cut, EvtWeight);
				}
			}
                 
            //if ((ilead_reco_jettest != -1) && (jet->at(ilead_reco_jettest).Eta() <= 2) && (jet->at(ilead_reco_jettest).Eta() >= -2) && (jet->at(ilead_reco_jettest).Pt() >= 1.0))
            //   genjet1part05_recofound.fill(*generatedjet, *generatedpart, 1, 2.0, 0.5, 2.5);
          
            multcorr.fill(genjetcoll, jetcoll, pt_cut+0.5, eta_cut, EvtWeight);
            ptsumcorr.fill(genjetcoll, jetcoll, pt_cut+0.5, eta_cut, EvtWeight);
            
          }
          
    
          //-------------------------------------------------------------------------------------------------
          //filling pt distribution for the observed/reconstructed tracks in the detector, finding leading particle, filling dpt & deta against leading particle
          
          //recopart1part05.fill(*reconpart, pt_cut, eta_cut, pt_cut, eta_cut, EvtWeight, WeighTr);
		  //cout<< "recojet1part05:" << endl;
          recojet1part05.fill(*reconjet, *reconpart, *generatedjet, pt_cut+0.5, eta_cut, pt_cut, eta_cut, EvtWeight, WeighTr, PtCorrection, PtCorrSimple, JetMatchSelCorr, EvtSelCorr);
          //cout<< endl;
          vtx_sel.fill(*vertex, EvtWeight);

		  vector<MyVertex>::iterator goodVtx = vertex->end();
		  for(vector<MyVertex>::iterator it_vtx = vertex->begin();it_vtx != vertex->end();++it_vtx)
			  if(vertexid==it_vtx->id)
				  goodVtx = it_vtx;
		  trp_sel.fill(trcoll,goodVtx,vertexid,beamSpot,EvtWeight);
          //trp_sel.fill(trcoll,(vector<MyVertex>::iterator&) *vertex,vertexid,beamSpot,EvtWeight);
          gpp_sel.fill(gpcoll, EvtWeight);

        }//end of loop over events
        
        //Closing current files
        file->Close();
      }//end of loop over files

      //output file
      if (isMC)
      {
          TFile* outputMC = new TFile("outputMC.root","RECREATE");
          outputMC->cd();
          
          trp_sel.write();
		  trp_bfcut.write();
		  trp_aftcut.write();
          gpp_sel.write();
          vtx_sel.write();
          
          //treffp_noEvtSel.write();
          treffp_evtSel.write();
          treffp_evtSel_nocuts.write();
		  treffp_evtSel_nocuts_LeadGenMatch.write();
		  treffp_evtSel_nocuts_NonLeadGenMatch.write();
		  treffp_evtSel_nocuts_UnMatch.write();
          //jeteffp_noEvtSel.write();
          jeteffp_evtSel.write();
          jeteffp_evtSel_nocuts.write();
          leadjeteffp_evtSel.write();
    
          //genpart1part05.write();
          genjet1part05.write();
		  matchedgenjet1part05.write();
		  genjet1part05_hasRecoJet.write();
          genjet1part05_vtxsel.write();
          genjet1part05_minbias.write();
		  genjet1part05_LeadGenMatch.write();
		  genjet1part05_NonLeadGenMatch.write();
		  genjet1part05_UnMatch.write();
          
          recojet1genpart05.write();
          genjet1recopart05.write();
		  recojet1genpart05_LeadGenMatch.write();
		  genjet1recopart05_LeadGenMatch.write();
		  recojet1genpart05_NonLeadGenMatch.write();
		  genjet1recopart05_NonLeadGenMatch.write();
          truegenjet1part05.write();
          
          ///genjet1part05_recofound.write();
          //truegenjet1part05_recofound.write();
          
          //Correlation matrix
          multcorr.correctgenreco((TProfile*) recojet1part05.profile_n_mult_trans, (TProfile*) recojet1genpart05.profile_n_mult_trans, (TProfile*) recojet1part05.profile_n_mult_trans);
          ptsumcorr.correctgenreco((TProfile*) recojet1part05.profile_ptsum_trans, (TProfile*) recojet1genpart05.profile_ptsum_trans, (TProfile*) recojet1part05.profile_ptsum_trans);
          multcorr.correctmatrix((TProfile*) multcorr.profile_corrected_genreco);
          ptsumcorr.correctmatrix((TProfile*) ptsumcorr.profile_corrected_genreco);
          multcorr.correcttruegengen((TProfile*) multcorr.profile_corrected_matrix, (TProfile*) truegenjet1part05.profile_n_mult_trans, (TProfile*) genjet1part05.profile_n_mult_trans);
          ptsumcorr.correcttruegengen((TProfile*) ptsumcorr.profile_corrected_matrix, (TProfile*) truegenjet1part05.profile_ptsum_trans, (TProfile*) genjet1part05.profile_ptsum_trans);
          
          //Simple bin-by-bin
          //multcorr.correctgenreco((TProfile*) recojet1part05.profile_n_mult_trans, (TProfile*) genjet1part05.profile_n_mult_trans, (TProfile*) recojet1part05.profile_n_mult_trans);
          //ptsumcorr.correctgenreco((TProfile*) recojet1part05.profile_ptsum_trans, (TProfile*) genjet1part05.profile_ptsum_trans, (TProfile*) recojet1part05.profile_ptsum_trans);
          ////multcorr.correctmatrix((TProfile*) recojet1genpart05.profile_n_mult_trans);
          //multcorr.correcttruegengen((TProfile*) multcorr.profile_corrected_genreco, (TProfile*) truegenjet1part05.profile_n_mult_trans, (TProfile*) genjet1part05.profile_n_mult_trans);
          //ptsumcorr.correcttruegengen((TProfile*) ptsumcorr.profile_corrected_genreco, (TProfile*) truegenjet1part05.profile_ptsum_trans, (TProfile*) genjet1part05.profile_ptsum_trans);
                  
          multcorr.write();
          ptsumcorr.write();
		  multcorr_LeadGenMatch.write();
		  multcorr_NonLeadGenMatch.write();
		  multcorr_UnMatch.write();
          
          //recopart1part05.write();
          recojet1part05.write();
          recojet1part05_LeadGenMatch.write();
		  recojet1part05_NonLeadGenMatch.write();
          recojet1part05_UnMatch.write();

		  TOperation<TH1D> nch_evtsel_eff ("nch_evtsel_eff", *((truegenjet1part05.profile_n_mult_trans)->ProjectionX())  , *((genjet1part05.profile_n_mult_trans)->ProjectionX())  , "eff");
		  TOperation<TH1D> ptsum_evtsel_eff ("ptsum_evtsel_eff", *((truegenjet1part05.profile_ptsum_trans)->ProjectionX())  , *((genjet1part05.profile_ptsum_trans)->ProjectionX())  , "eff");
		  TOperation<TH1D> nch_matchtolead_eff ("nch_matchtolead_eff", *((genjet1part05.profile_n_mult_trans)->ProjectionX())  , *((matchedgenjet1part05.profile_n_mult_trans)->ProjectionX())  , "eff");
		  TOperation<TH1D> ptsum_matchtolead_eff ("ptsum_matchtolead_eff", *((genjet1part05.profile_ptsum_trans)->ProjectionX())  , *((matchedgenjet1part05.profile_ptsum_trans)->ProjectionX())  , "eff");
		  TOperation<TH1D> jetpt_nch_corr_eff ("jetpt_nch_corr_eff", *((genjet1part05.profile_n_mult_trans)->ProjectionX())  , *((recojet1genpart05.profile_n_mult_trans)->ProjectionX())  , "eff");
		  TOperation<TH1D> jetpt_ptsum_corr_eff ("jetpt_ptsum_corr_eff", *((genjet1part05.profile_ptsum_trans)->ProjectionX())  , *((recojet1genpart05.profile_ptsum_trans)->ProjectionX())  , "eff");

          nch_evtsel_eff.write(0);
		  ptsum_evtsel_eff.write(0);
		  nch_matchtolead_eff.write(0);
		  ptsum_matchtolead_eff.write(0);
		  jetpt_nch_corr_eff.write(0);
		  jetpt_ptsum_corr_eff.write(0);

		  /*if (JetMatchSelCorr)
		  {
			  profile_nch_trans_matchcorr = (TH1D*) (recojet1part05.profile_n_mult_trans_jetcorr_weighted)->ProjectionX();
			  profile_nch_trans_matchcorr->SetNameTitle("profile_nch_trans_matchcorr", "profile_nch_trans_matchcorr");
			  profile_nch_trans_matchcorr->Multiply(nch_matchtolead_eff2);
			  profile_ptsum_trans_matchcorr = (TH1D*) (recojet1part05.profile_ptsum_trans_jetcorr_weighted)->ProjectionX();
			  profile_ptsum_trans_matchcorr->SetNameTitle("profile_ptsum_trans_matchcorr", "profile_ptsum_trans_matchcorr");
			  profile_ptsum_trans_matchcorr->Multiply(ptsum_matchtolead_eff2);

			  profile_nch_trans_matchcorr->Write();
			  profile_ptsum_trans_matchcorr->Write();
		  
			  if (EvtSelCorr)
			  {
				  profile_nch_trans_matchcorr_evtselcorr = (TH1D*) profile_nch_trans_matchcorr->Clone("profile_nch_trans_matchcorr_evtselcorr");
				  profile_nch_trans_matchcorr_evtselcorr->SetNameTitle("profile_nch_trans_matchcorr_evtselcorr", "profile_nch_trans_matchcorr_evtselcorr");
				  profile_nch_trans_matchcorr_evtselcorr->Multiply(nch_evtsel_eff2);
				  profile_ptsum_trans_matchcorr_evtselcorr = (TH1D*) profile_ptsum_trans_matchcorr->Clone("profile_ptsum_trans_matchcorr_evtselcorr");
				  profile_ptsum_trans_matchcorr_evtselcorr->SetNameTitle("profile_ptsum_trans_matchcorr_evtselcorr", "profile_ptsum_trans_matchcorr_evtselcorr");
				  profile_ptsum_trans_matchcorr_evtselcorr->Multiply(ptsum_evtsel_eff2);

				  profile_nch_trans_matchcorr_evtselcorr->Write();
				  profile_ptsum_trans_matchcorr_evtselcorr->Write();
			  }
		  }
		  if (PtCorrSimple)
		  {
			  profile_nch_trans_ptcorrsimple = (TH1D*) (recojet1part05.profile_n_mult_trans_weighted)->ProjectionX();
			  profile_nch_trans_ptcorrsimple->SetNameTitle("profile_nch_trans_ptcorrsimple", "profile_nch_trans_ptcorrsimple");
			  profile_nch_trans_ptcorrsimple->Multiply(jetpt_nch_corr_eff2);
			  profile_ptsum_trans_ptcorrsimple = (TH1D*) (recojet1part05.profile_ptsum_trans_weighted)->ProjectionX();
			  profile_ptsum_trans_ptcorrsimple->SetNameTitle("profile_ptsum_trans_ptcorrsimple", "profile_ptsum_trans_ptcorrsimple");
			  profile_ptsum_trans_ptcorrsimple->Multiply(jetpt_ptsum_corr_eff2);

			  profile_nch_trans_ptcorrsimple->Write();
			  profile_ptsum_trans_ptcorrsimple->Write();
		  
			  if (EvtSelCorr)
			  {
				  profile_nch_trans_ptcorrsimple_evtselcorr = (TH1D*) profile_nch_trans_ptcorrsimple->Clone("profile_nch_trans_ptcorrsimple_evtselcorr");
				  profile_nch_trans_ptcorrsimple_evtselcorr->SetNameTitle("profile_nch_trans_ptcorrsimple_evtselcorr", "profile_nch_trans_ptcorrsimple_evtselcorr");
				  profile_nch_trans_ptcorrsimple_evtselcorr->Multiply(nch_evtsel_eff2);
				  profile_ptsum_trans_ptcorrsimple_evtselcorr = (TH1D*) profile_ptsum_trans_ptcorrsimple->Clone("profile_ptsum_trans_ptcorrsimple_evtselcorr");
				  profile_ptsum_trans_ptcorrsimple_evtselcorr->SetNameTitle("profile_ptsum_trans_ptcorrsimple_evtselcorr", "profile_ptsum_trans_ptcorrsimple_evtselcorr");
				  profile_ptsum_trans_ptcorrsimple_evtselcorr->Multiply(ptsum_evtsel_eff2);

				  profile_nch_trans_ptcorrsimple_evtselcorr->Write();
				  profile_ptsum_trans_ptcorrsimple_evtselcorr->Write();
			  }
		  }*/

          pthat->Write();
          nevt_pthat->Write();
		  nGoodVtx->Write();
		  nDOFat1GdVtx->Write();
          
          outputMC->Close();
      }
      else
      {
          TFile* outputData = new TFile("outputData.root","RECREATE");
          outputData->cd();
                  
          trp_sel.write();
          gpp_sel.write();
          vtx_sel.write();
          
          //recopart1part05.write();
          recojet1part05.write();

		  /*if (JetMatchSelCorr)
		  {
			  profile_nch_trans_matchcorr = (TH1D*) (recojet1part05.profile_n_mult_trans_jetcorr_weighted)->ProjectionX();
			  profile_nch_trans_matchcorr->SetNameTitle("profile_nch_trans_matchcorr", "profile_nch_trans_matchcorr");
			  profile_nch_trans_matchcorr->Multiply(nch_matchtolead_eff2);
			  profile_ptsum_trans_matchcorr = (TH1D*) (recojet1part05.profile_ptsum_trans_jetcorr_weighted)->ProjectionX();
			  profile_ptsum_trans_matchcorr->SetNameTitle("profile_ptsum_trans_matchcorr", "profile_ptsum_trans_matchcorr");
			  profile_ptsum_trans_matchcorr->Multiply(ptsum_matchtolead_eff2);

			  profile_nch_trans_matchcorr->Write();
			  profile_ptsum_trans_matchcorr->Write();
		  
			  if (EvtSelCorr)
			  {
				  profile_nch_trans_matchcorr_evtselcorr = (TH1D*) profile_nch_trans_matchcorr->Clone("profile_nch_trans_matchcorr_evtselcorr");
				  profile_nch_trans_matchcorr_evtselcorr->SetNameTitle("profile_nch_trans_matchcorr_evtselcorr", "profile_nch_trans_matchcorr_evtselcorr");
				  profile_nch_trans_matchcorr_evtselcorr->Multiply(nch_evtsel_eff2);
				  profile_ptsum_trans_matchcorr_evtselcorr = (TH1D*) profile_ptsum_trans_matchcorr->Clone("profile_ptsum_trans_matchcorr_evtselcorr");
				  profile_ptsum_trans_matchcorr_evtselcorr->SetNameTitle("profile_ptsum_trans_matchcorr_evtselcorr", "profile_ptsum_trans_matchcorr_evtselcorr");
				  profile_ptsum_trans_matchcorr_evtselcorr->Multiply(ptsum_evtsel_eff2);

				  profile_nch_trans_matchcorr_evtselcorr->Write();
				  profile_ptsum_trans_matchcorr_evtselcorr->Write();
			  }
		  }
		  if (PtCorrSimple)
		  {
			  profile_nch_trans_ptcorrsimple = (TH1D*) (recojet1part05.profile_n_mult_trans_weighted)->ProjectionX();
			  profile_nch_trans_ptcorrsimple->SetNameTitle("profile_nch_trans_ptcorrsimple", "profile_nch_trans_ptcorrsimple");
			  profile_nch_trans_ptcorrsimple->Multiply(jetpt_nch_corr_eff2);
			  profile_ptsum_trans_ptcorrsimple = (TH1D*) (recojet1part05.profile_ptsum_trans_weighted)->ProjectionX();
			  profile_ptsum_trans_ptcorrsimple->SetNameTitle("profile_ptsum_trans_ptcorrsimple", "profile_ptsum_trans_ptcorrsimple");
			  profile_ptsum_trans_ptcorrsimple->Multiply(jetpt_ptsum_corr_eff2);

			  profile_nch_trans_ptcorrsimpleclosure = (TH1D*) (recojet1genpart05.profile_n_mult_trans)->ProjectionX();
			  profile_nch_trans_ptcorrsimpleclosure->SetNameTitle("profile_nch_trans_ptcorrsimpleclosure", "profile_nch_trans_ptcorrsimpleclosure");
			  profile_nch_trans_ptcorrsimpleclosure->Multiply(jetpt_nch_corr_eff2);
			  profile_ptsum_trans_ptcorrsimpleclosure = (TH1D*) (recojet1genpart05.profile_ptsum_trans)->ProjectionX();
			  profile_ptsum_trans_ptcorrsimpleclosure->SetNameTitle("profile_ptsum_trans_ptcorrsimpleclosure", "profile_ptsum_trans_ptcorrsimpleclosure");
			  profile_ptsum_trans_ptcorrsimpleclosure->Multiply(jetpt_ptsum_corr_eff2);

			  profile_nch_trans_ptcorrsimpleclosure->Write();
			  profile_ptsum_trans_ptcorrsimpleclosure->Write();
			  profile_nch_trans_ptcorrsimple->Write();
			  profile_ptsum_trans_ptcorrsimple->Write();
		  
			  if (EvtSelCorr)
			  {
				  profile_nch_trans_ptcorrsimple_evtselcorr = (TH1D*) profile_nch_trans_ptcorrsimple->Clone("profile_nch_trans_ptcorrsimple_evtselcorr");
				  profile_nch_trans_ptcorrsimple_evtselcorr->SetNameTitle("profile_nch_trans_ptcorrsimple_evtselcorr", "profile_nch_trans_ptcorrsimple_evtselcorr");
				  profile_nch_trans_ptcorrsimple_evtselcorr->Multiply(nch_evtsel_eff2);
				  profile_ptsum_trans_ptcorrsimple_evtselcorr = (TH1D*) profile_ptsum_trans_ptcorrsimple->Clone("profile_ptsum_trans_ptcorrsimple_evtselcorr");
				  profile_ptsum_trans_ptcorrsimple_evtselcorr->SetNameTitle("profile_ptsum_trans_ptcorrsimple_evtselcorr", "profile_ptsum_trans_ptcorrsimple_evtselcorr");
				  profile_ptsum_trans_ptcorrsimple_evtselcorr->Multiply(ptsum_evtsel_eff2);

				  profile_nch_trans_ptcorrsimple_evtselcorr->Write();
				  profile_ptsum_trans_ptcorrsimple_evtselcorr->Write();
			  }
		  }*/

		  nGoodVtx->Write();
		  nDOFat1GdVtx->Write();
          
          outputData->Close();
              
      }
  
  //    isMC = false;
  //}//end of loop over mc and data
}

int pthatcut(TString filename)
{
    if (filename.Contains("QCD_Pt_15")) return 1;
    if (filename.Contains("QCD_Pt_50")) return 2;
    if (filename.Contains("QCD_Pt_80")) return 3;
    return 0;
}

/*
TH2F Get_nevt_pthat_FromFile(TString fstr , TString nevt_pthat_str){
  TFile* f = TFile::Open(fstr , "READ");
  if(f==0){
    cout << "[UEPlots] WARNING !! Can't open file for ptcorr & ptratio." << endl;
  }
  
  TH2F nevt_pthat = *((TH2F*) f->Get(nevt_pthat_str));
  if(nevt_pthat.GetEntries()==0){
    cout << "[UEPlots] WARNING !! Couldn't fetch ptcorr " << nevt_pthat_str << " from file "<< fstr << "." << endl;
  }
  
  return nevt_pthat;
}
*/
