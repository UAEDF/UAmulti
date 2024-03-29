#include <TROOT.h>
#include <TH1.h>
#include <TH2.h>
#include <TProfile.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TTree.h>
#include <TFile.h>
#include <TChain.h>
//#include <TDirectory.h>
#include <iostream>
#include <sstream>
using namespace std;
//#include "TSystem.h"
//#include "myevent.h"
//#include "HWWGenPart.h"

#include "../plugins/MyEvtId.h"
#include "../plugins/MyL1Trig.h"
#include "../plugins/MyGenKin.h"
#include "../plugins/MyPart.h"
#include "../plugins/MyGenPart.h" 
#include "../plugins/MyTracks.h"
#include "../plugins/MyVertex.h"
#include "../plugins/MyMITEvtSel.h"
#include "../plugins/MyBeamSpot.h"
#include "../plugins/TrackPlots.h"
#include "../plugins/VertexPlots.h"
#include "../plugins/EvtSelPlots.h"
#include "../plugins/BasePlots.h"
#include "../plugins/MultiPlots.h"
#include "../plugins/GenMultiPlots.h"
#include "../plugins/MatrixPlots.h"

bool debug = false;

TString filename = "MC_test_test_900GeV";
bool isMC = true;

#include "fileManager.C"

#include "cuts.C"
#include "evtSel.C"
#include "binningMap.C"

void collectionPlotter(int = 10 , double = 0.9 , double = 0.2 , TString = "NONE" , int = 5000 );

void collectionPlotter(int type , double E , double ptcut , TString filename , int nevt_max )
{
  if(type==0) isMC = false;
  if(type==1) isMC = false;
  if(type==5) isMC = false;
  pt_cut = ptcut;
  
  // General variables
  int nbinmulti = 110;
  TH1F* nch_gen_noCut     = new TH1F("nch_gen_noCut","nch_gen_noCut;n_{CH};# events",(int)nbinmulti+1,-0.5,nbinmulti+0.5);
  TH1F* pt_gen_noCut      = new TH1F("pt_gen_noCut","pt_gen_noCut;pT [GeV];# events",100,0.,3.);
  TH1F* eta_gen_noCut     = new TH1F("eta_gen_noCut","eta_gen_noCut;#eta;# events",60,-3.,3.);
  TH1F* nch_gen_etaCut    = new TH1F("nch_gen_etaCut","nch_gen_etaCut;n_{CH};# events",(int)nbinmulti+1,-0.5,nbinmulti+0.5);
  TH1F* pt_gen_etaCut     = new TH1F("pt_gen_etaCut","pt_gen_etaCut;pT [GeV];# events",100,0.,3.);
  TH1F* eta_gen_etaCut    = new TH1F("eta_gen_etaCut","eta_gen_etaCut;#eta;# events",60,-3.,3.);
  TH1F* nch_gen_eta_ptCut = new TH1F("nch_gen_eta_ptCut","nch_gen_eta_ptCut;n_{CH};# events",(int)nbinmulti+1,-0.5,nbinmulti+0.5);
  TH1F* pt_gen_eta_ptCut  = new TH1F("pt_gen_eta_ptCut","pt_gen_eta_ptCut;pT [GeV];# events",100,0.,3.);
  TH1F* eta_gen_eta_ptCut = new TH1F("eta_gen_eta_ptCut","eta_gen_eta_ptCut;#eta;# events",60,-3.,3.);

 
  TH1F* beamwidthX = new TH1F("beamwidthX","beamwidthX",400,-4.,4.);
  TH1F* beamwidthY = new TH1F("beamwidthY","beamwidthY",400,-4.,4.);

  TH2F* nchVSnvtx = new TH2F("nchVSnvtx","nchVSnvtx",(int)nbinmulti+1,-0.5,nbinmulti+0.5,6,-0.5,5.5);

  //pt    = new TH1F("pt_"+trackcoll,"pt_"+trackcoll+";pt [GeV];# events",100,0.,3.);
  //eta   = new TH1F("eta_"+trackcoll,"eta_"+trackcoll+";#eta;# events",60,-3.,3.);
  
  //------------- TRACK COLLECTIONS -------------
  //generaltracks
  /*TrackPlots* trp_allTr_gTr_oVtx = new TrackPlots("allTr_gTr_oVtx");
  TrackPlots* trp_PV_gTr_oVtx    = new TrackPlots("PV_gTr_oVtx");
  TrackPlots* trp_allTr_gTr_fVtx = new TrackPlots("allTr_gTr_fVtx");
  TrackPlots* trp_PV_gTr_fVtx    = new TrackPlots("PV_gTr_fVtx");
  
  //minbiastracks
  TrackPlots* trp_allTr_mbTr_oVtx = new TrackPlots("allTr_mbTr_oVtx");
  TrackPlots* trp_PV_mbTr_oVtx    = new TrackPlots("PV_mbTr_oVtx");
  TrackPlots* trp_allTr_mbTr_fVtx = new TrackPlots("allTr_mbTr_fVtx");
  TrackPlots* trp_PV_mbTr_fVtx    = new TrackPlots("PV_mbTr_fVtx");
  */

  
   cout<<"tttttttttttt"<<endl;
   
  // Bin ZERO BG Plots  
  TrackPlots* trp_ntr0_gTr_novtx      = new TrackPlots("ntr0_gTr_novtx");
  TrackPlots* trp_ntr0_gTr_withvtx    = new TrackPlots("ntr0_gTr_withvtx"); 
  TrackPlots* trp_ntr0_mbTr_novtx     = new TrackPlots("ntr0_mbTr_novtx");
  TrackPlots* trp_ntr0_mbTr_withvtx   = new TrackPlots("ntr0_mbTr_withvtx");  
  int nevt_ntr0_gTr = 0 , nevt_ntr0_gTr_novtx = 0 , nevt_ntr0_gTr_notr = 0 , nevt_ntr0_gTr_novtxatall = 0;
  int nevt_ntr0_gTr_novtx_notr = 0 , nevt_ntr0_gTr_novtx_withtr = 0, nevt_ntr0_gTr_withvtx_notr = 0 ;
  int nevt_ntr0_mbTr = 0 , nevt_ntr0_mbTr_novtx = 0 , nevt_ntr0_mbTr_notr = 0 , nevt_ntr0_mbTr_novtxatall = 0;
  int nevt_ntr0_mbTr_novtx_notr = 0 , nevt_ntr0_mbTr_novtx_withtr = 0, nevt_ntr0_mbTr_withvtx_notr = 0 ;
  
  
  //-------------------------------------------------------
  //events with 2 vtx
  VertexPlots* vtxp_2vtx_vtx1 = new VertexPlots("2vtx_vtx1");
  VertexPlots* vtxp_2vtx_vtx2 = new VertexPlots("2vtx_vtx2");
  TH1F* nch_2vtx_vtx1         = new TH1F("nch_2vtx_vtx1","nch_2vtx_vtx1",(int)nbinmulti+1,-0.5,nbinmulti+0.5);
  TH1F* nch_2vtx_vtx2         = new TH1F("nch_2vtx_vtx2","nch_2vtx_vtx2",(int)nbinmulti+1,-0.5,nbinmulti+0.5);
  TH1F* zdiff_2vtx            = new TH1F("zdiff_2vtx","zdiff_2vtx",50,-10.,10.);
  TH1F* ratio_vtx1Ogen        = new TH1F("ratio_vtx1Ogen","ratio_vtx1Ogen;n_{rec}^{vtx1} / n_{gen};# events",40,0.,2.);
  TH1F* ratio_vtx2Ogen        = new TH1F("ratio_vtx2Ogen","ratio_vtx2Ogen;n_{rec}^{vtx2} / n_{gen};# events",40,0.,2.);
  TH2F* nch_vtx1VSgen         = new TH2F("nch_vtx1VSgen","nch_vtx1VSgen;n_{gen};n_{rec}^{vtx2}",(int)nbinmulti+1,-0.5,nbinmulti+0.5,(int)nbinmulti+1,-0.5,nbinmulti+0.5);
  TH2F* nch_vtx2VSgen         = new TH2F("nch_vtx2VSgen","nch_vtx2VSgen;n_{gen};n_{rec}^{vtx2}",(int)nbinmulti+1,-0.5,nbinmulti+0.5,(int)nbinmulti+1,-0.5,nbinmulti+0.5);
  
  
  
  
  /*
  //------------- VERTEX COLLECTIONS -------------
  //offlinePV
  VertexPlots* vtxp_allVtx_offlinePV = new VertexPlots("allVtx_oVtx");
  VertexPlots* vtxp_PV_offlinePV = new VertexPlots("PV_oVtx");
  
  //ferencVtx wrt minbiasTracks
  VertexPlots* vtxp_allVtx_ferencVtx = new VertexPlots("allVtx_fVtx");
  VertexPlots* vtxp_PV_ferencVtx = new VertexPlots("PV_fVtx");
  */
  
  
   cout<<"tttttttttttt"<<endl;
  //------------- EvtSel COLLECTION -------------
  EvtSelPlots* evtselp_PV_mbTr_fVtx = new EvtSelPlots("PV_mbTr_fVtx");
  EvtSelPlots* evtselp_PV_gTr_oVtx  = new EvtSelPlots("PV_gTr_oVtx");
  EvtSelPlots* evtselp_fPV_gTr_oVtx = new EvtSelPlots("fPV_gTr_oVtx");
  EvtSelPlots* evtselp_PV_mbTr_pVtx = new EvtSelPlots("PV_mbTr_pVtx");
  
  EvtSelPlots* evtselp_allTr_mbTr_fVtx = new EvtSelPlots("allTr_mbTr_fVtx");
  EvtSelPlots* evtselp_allTr_gTr_oVtx  = new EvtSelPlots("allTr_gTr_oVtx");
  EvtSelPlots* evtselp_allTr_mbTr_pVtx = new EvtSelPlots("allTr_mbTr_pVtx");
 
  
  EvtSelPlots* evtselp_allTr_mbTr_bs = new EvtSelPlots("allTr_mbTr_bs");
  EvtSelPlots* evtselp_allTr_gTr_bs  = new EvtSelPlots("allTr_gTr_bs");
  EvtSelPlots* evtselp_PV_mbTr_bs = new EvtSelPlots("PV_mbTr_bs");
  EvtSelPlots* evtselp_PV_gTr_bs  = new EvtSelPlots("PV_gTr_bs");

  
   cout<<"tttttttttttt"<<endl;
  
  //------------ MultiPlots ------------------
  BasePlots* baseplot = new BasePlots("BasePlots"); 
  vector< vector<double> > binning;
  binning = getBins(0,0,0);
  baseplot->setBinning(binning);

  MultiPlots* mp_PV_mbTr_fVtx    = new MultiPlots("L1_hf_VtxSel_PV_mbTr_fVtx");
  MultiPlots* mp_PV_gTr_oVtx     = new MultiPlots("L1_hf_VtxSel_PV_gTr_oVtx");
  
  GenMultiPlots* gmp_noCut_noSel = new GenMultiPlots("noCut_noSel");
  GenMultiPlots* gmp_ptCut_noSel = new GenMultiPlots("ptCut_noSel");
  GenMultiPlots* gmp_etaCut_noSel = new GenMultiPlots("etaCut_noSel");
  GenMultiPlots* gmp_pt_etaCut_noSel = new GenMultiPlots("pt_etaCut_noSel");
  
  GenMultiPlots* gmp_2vtx     = new GenMultiPlots("2vtx");

  
  
   cout<<"tttttttttttt"<<endl;
  
  //------------ MatrixPlots ------------------
  MatrixPlots* mtxp_etaGenCut_L1_hf_VtxSel_PV_mbTr_fVtx = new MatrixPlots("etaGenCut_L1_hf_VtxSel_PV_mbTr_fVtx");
  MatrixPlots* mtxp_eta_ptGenCut_L1_hf_VtxSel_PV_mbTr_fVtx = new MatrixPlots("eta_ptGenCut_L1_hf_VtxSel_PV_mbTr_fVtx");
  MatrixPlots* mtxp_etaGenCut_L1_hf_VtxSel_PV_gTr_oVtx = new MatrixPlots("etaGenCut_L1_hf_VtxSel_PV_gTr_oVtx");
  MatrixPlots* mtxp_eta_ptGenCut_L1_hf_VtxSel_PV_gTr_oVtx = new MatrixPlots("eta_ptGenCut_L1_hf_VtxSel_PV_gTr_oVtx");
  
   cout<<"tttttttttttt"<<endl;
  
  // Open Tree file 
  //TFile* file=new TFile("/user/xjanssen/cms/MinBias/CMSSW_3_3_6_patch3/src/UAmulti/ChPartTree/test/ChPartTree_2.36_simrecfile103.root","OPEN");
  //TTree* tree =(TTree*)file->Get("evt");
  
  TChain* tree = new TChain("evt","");

  cout << fileManager(0,type,E) << endl;
  
  tree->Add(fileManager(0,type,E));
  
/*  
//------------ v5 ------------
if(E == 0.9 ){
if(isMC)tree->Add("/user/xjanssen/data/CMSSW_3_3_6_patch3/ChPartTree_v005b_mc900/__MinBias__Summer09-STARTUP3X_V8K_900GeV-v1__GEN-SIM-RECO/ChPartTree_v005b_mc900__CMSSW_3_3_6_patch3__MinBias__Summer09-STARTUP3X_V8K_900GeV-v1__GEN-SIM-RECO_*.root/evt");
//if(isMC)tree->Add("/user/xjanssen/data/CMSSW_3_3_6_patch3/ChPartTree_v003_mc900/__MinBias__Summer09-STARTUP3X_V8K_900GeV-v1__GEN-SIM-RECO/ChPartTree_v003_mc900__CMSSW_3_3_6_patch3__MinBias__Summer09-STARTUP3X_V8K_900GeV-v1__GEN-SIM-RECO_1.root/evt");
if(!isMC)tree->Add("/user/xjanssen/data/CMSSW_3_3_6_patch3/ChPartTree_v005b_d900/__MinimumBias__BeamCommissioning09-Dec19thReReco_336p3_v2__RECO/ChPartTree_v005b_d900__CMSSW_3_3_6_patch3__MinimumBias__BeamCommissioning09-Dec19thReReco_336p3_v2__RECO_*.root/evt");
}

if(E == 2.36 ){
if(!isMC)tree->Add("/user/xjanssen/data/CMSSW_3_3_6_patch3/ChPartTree_v005b_d236/__MinimumBias__BeamCommissioning09-Dec19thReReco_336p3_v2__RECO/ChPartTree_v005b_d236__CMSSW_3_3_6_patch3__MinimumBias__BeamCommissioning09-Dec19thReReco_336p3_v2__RECO_*.root/evt");
//if(isMC)tree->Add("/user/xjanssen/data/CMSSW_3_3_6_patch3/ChPartTree_v003_mc900/__MinBias__Summer09-STARTUP3X_V8K_900GeV-v1__GEN-SIM-RECO/ChPartTree_v003_mc900__CMSSW_3_3_6_patch3__MinBias__Summer09-STARTUP3X_V8K_900GeV-v1__GEN-SIM-RECO_1.root/evt");
if(isMC)tree->Add("/user/xjanssen/data/CMSSW_3_3_6_patch3/ChPartTree_v005b_mc236/__MinBias__Summer09-STARTUP3X_V8L_2360GeV-v1__GEN-SIM-RECO/ChPartTree_v005b_mc236__CMSSW_3_3_6_patch3__MinBias__Summer09-STARTUP3X_V8L_2360GeV-v1__GEN-SIM-RECO_*.root/evt");
}
*/
/*
//---------- v4 --------------
if(E == 0.9 ){
if(!isMC)tree->Add("/user/xjanssen/data/CMSSW_3_3_6_patch3/ChPartTree_v004b_d900/__MinimumBias__BeamCommissioning09-Dec19thReReco_336p3_v2__RECO/ChPartTree_v004b_d900__CMSSW_3_3_6_patch3__MinimumBias__BeamCommissioning09-Dec19thReReco_336p3_v2__RECO_*.root/evt");
//if(isMC)tree->Add("/user/xjanssen/data/CMSSW_3_3_6_patch3/ChPartTree_v003_mc900/__MinBias__Summer09-STARTUP3X_V8K_900GeV-v1__GEN-SIM-RECO/ChPartTree_v003_mc900__CMSSW_3_3_6_patch3__MinBias__Summer09-STARTUP3X_V8K_900GeV-v1__GEN-SIM-RECO_1.root/evt");
if(isMC)tree->Add("/user/xjanssen/data/CMSSW_3_3_6_patch3/ChPartTree_v004_mc900/__MinBias__Summer09-STARTUP3X_V8K_900GeV-v1__GEN-SIM-RECO/ChPartTree_v004_mc900__CMSSW_3_3_6_patch3__MinBias__Summer09-STARTUP3X_V8K_900GeV-v1__GEN-SIM-RECO_*.root/evt");
}

if(E == 2.36 ){
if(!isMC)tree->Add("/user/xjanssen/data/CMSSW_3_3_6_patch3/ChPartTree_v004nomb_d236/__MinimumBias__BeamCommissioning09-Dec19thReReco_336p3_v2__RECO/ChPartTree_v004nomb_d236__CMSSW_3_3_6_patch3__MinimumBias__BeamCommissioning09-Dec19thReReco_336p3_v2__RECO_*.root/evt");
//if(isMC)tree->Add("/user/xjanssen/data/CMSSW_3_3_6_patch3/ChPartTree_v003_mc900/__MinBias__Summer09-STARTUP3X_V8K_900GeV-v1__GEN-SIM-RECO/ChPartTree_v003_mc900__CMSSW_3_3_6_patch3__MinBias__Summer09-STARTUP3X_V8K_900GeV-v1__GEN-SIM-RECO_1.root/evt");
if(isMC)tree->Add("/user/xjanssen/data/CMSSW_3_3_6_patch3/ChPartTree_v004_mc236/__MinBias__Summer09-STARTUP3X_V8L_2360GeV-v1__GEN-SIM-RECO/ChPartTree_v004_mc236__CMSSW_3_3_6_patch3__MinBias__Summer09-STARTUP3X_V8L_2360GeV-v1__GEN-SIM-RECO_*.root/evt");
}
*/
//OLD
//if(!isMC)tree->Add("/user/xjanssen/data/CMSSW_3_3_6_patch3/ChPartTree_v003_d900/__MinimumBias__BeamCommissioning09-Dec19thReReco_336p3_v2__RECO/ChPartTree_v003_d900__CMSSW_3_3_6_patch3__MinimumBias__BeamCommissioning09-Dec19thReReco_336p3_v2__RECO_2?.root/evt");
//if(isMC)tree->Add("/user/xjanssen/data/CMSSW_3_3_6_patch3/ChPartTree_v003_mc900/__MinBias__Summer09-STARTUP3X_V8K_900GeV-v1__GEN-SIM-RECO/ChPartTree_v003_mc900__CMSSW_3_3_6_patch3__MinBias__Summer09-STARTUP3X_V8K_900GeV-v1__GEN-SIM-RECO_1.root/evt");
//if(isMC)tree->Add("/user/xjanssen/cms/MinBias/CMSSW_3_3_6_patch3/src/UAmulti/ChPartTree/test/ChPartTree.root/evt");


//OLD if(isMC)  tree->Add("/user/xjanssen/data/CMSSW_3_3_6_patch3/ChPartTree_v001_mc/__MinBias__Summer09-STARTUP3X_V8K_900GeV-v1__GEN-SIM-RECO/ChPartTree_v001_mc__CMSSW_3_3_6_patch3__MinBias__Summer09-STARTUP3X_V8K_900GeV-v1__GEN-SIM-RECO_156.root/evt");
//OLD if(!isMC) tree->Add("/user/xjanssen/data/CMSSW_3_3_6_patch3/ChPartTree_v001_d900_runsel/__MinimumBias__BeamCommissioning09-Dec19thReReco_336p3_v2__RECO/ChPartTree_v001_d900_runsel__CMSSW_3_3_6_patch3__MinimumBias__BeamCommissioning09-Dec19thReReco_336p3_v2__RECO_*.root/evt");
  
  MyEvtId* evtId      = NULL ;
  tree->SetBranchAddress("EvtId",&evtId);
  
  vector<MyGenPart>* genPart = NULL;
  if(isMC) tree->SetBranchAddress("GenPart",&genPart);
  
  MyGenKin* genKin = NULL;
  if(isMC) tree->SetBranchAddress("GenKin",&genKin);
  
  vector<MyTracks>*  generalTracks  = NULL;
  vector<MyTracks>*  minbiasTracks  = NULL;
  vector<MyVertex>*  offlinePV  = NULL;
  vector<MyVertex>*  ferencVtx  = NULL;
  vector<MyVertex>*  pixel3Vertex  = NULL;
  vector<MyVertex>*  vertexToCut  = NULL;
  MyL1Trig*     L1Trig    = NULL;
  MyMITEvtSel*  MITEvtSel = NULL;
  MyBeamSpot*   bs        = NULL;
  tree->SetBranchAddress("generalTracks",&generalTracks); 
  tree->SetBranchAddress("minbiasTracks",&minbiasTracks);
  tree->SetBranchAddress("primaryVertex",&offlinePV);
  tree->SetBranchAddress("ferencVtxFerTrk",&ferencVtx);
  tree->SetBranchAddress("pixel3Vertex",&pixel3Vertex);
  tree->SetBranchAddress("L1Trig",&L1Trig);
  tree->SetBranchAddress("MITEvtSel",&MITEvtSel);
  tree->SetBranchAddress("beamSpot",&bs);
  
  int nev = int(tree->GetEntries());
  std::cout <<"number of entries is : "<< nev << std::endl;
  cout<<"Running on: "<<min(nev,nevt_max)<<" events"<<endl;
  // Event TYPE counting --> Weights
  
  for(int i = 0; i < nev; i++){
    
    if( ((i+1) % 10000) == 0) cout <<int(double(i+1)/double(min(nev,nevt_max))*100.)<<" % done"<<endl;
    
    if(i>min(nev,nevt_max)) break;
    
    tree->GetEntry(i);
    
/*    
    if(i==0) {
      if ( E < 7.0 ) 
        vertexToCut = pixel3Vertex;
      else
        vertexToCut = ferencVtx;
    } 
*/
    vertexToCut = ferencVtx;  
  
    //Selection of good BX for data && MC
    if(!isMC && !goodBX(evtId->Run,evtId->Bunch)) continue;
    
        
    //---------------- GEN -------------------
    int nchgen = 0;
    bool laccept_noCut = false;
    bool laccept_etaCut = false;
    bool laccept_ptCut = false;
    bool laccept_pt_etaCut = false;
    bool laccept_etaCut_NSD = false;
    bool laccept_pt_etaCut_NSD = false;
    
    bool laccept_reco_etaCut = false;
    
    if(isMC){
    
      for(vector<MyGenPart>::iterator p=genPart->begin() ; p!=genPart->end() ; p++ ){
        if ( isGenPartGood(*p) ){
	  ++nchgen;
	  pt_gen_noCut->Fill(p->Part.v.Pt());
	  eta_gen_noCut->Fill(p->Part.v.Eta());
	  gmp_noCut_noSel->fill(*genKin,p->Part);
	  laccept_noCut = true;
	  
	  //eta cut
	  if( fabs(p->Part.v.Eta())  < eta_cut ){
	    gmp_etaCut_noSel->fill(*genKin,p->Part);
	    laccept_etaCut = true;
	  }
	    
	  //pt cut
	  if( p->Part.v.Pt()> pt_cut ){
	    gmp_ptCut_noSel->fill(*genKin,p->Part);
	    laccept_ptCut = true;
	  }
	    
	  //eta + pt cut
	  if( fabs(p->Part.v.Eta()) < eta_cut 
	   && p->Part.v.Pt()> pt_cut ){
	    gmp_pt_etaCut_noSel->fill(*genKin,p->Part);
	    laccept_pt_etaCut = true;
	  }
	  
	  
	  //eta cut + good evt sel
	  if(isEvtGood( E,*L1Trig , *MITEvtSel , vertexToCut) && fabs(p->Part.v.Eta()) < eta_cut && !isSD(genKin)){
	    mtxp_etaGenCut_L1_hf_VtxSel_PV_mbTr_fVtx->fillGen(p->Part);
	    mtxp_etaGenCut_L1_hf_VtxSel_PV_gTr_oVtx->fillGen(p->Part);
	    laccept_etaCut_NSD = true;
	    
	    // + pt cut 
	    if(p->Part.v.Pt()> pt_cut){
	      mtxp_eta_ptGenCut_L1_hf_VtxSel_PV_mbTr_fVtx->fillGen(p->Part);
	      mtxp_eta_ptGenCut_L1_hf_VtxSel_PV_gTr_oVtx->fillGen(p->Part);
	      laccept_pt_etaCut_NSD = true;
	    }
	  }
	}
      }

/*      
        if(laccept_noCut)     gmp_noCut_noSel->nextEvent(*genKin);
        if(laccept_ptCut)     gmp_ptCut_noSel->nextEvent(*genKin);
        if(laccept_etaCut)    gmp_etaCut_noSel->nextEvent(*genKin);
        if(laccept_pt_etaCut) gmp_pt_etaCut_noSel->nextEvent(*genKin);
*/
        gmp_noCut_noSel->nextEvent(*genKin);
        gmp_ptCut_noSel->nextEvent(*genKin);
        gmp_etaCut_noSel->nextEvent(*genKin);
        gmp_pt_etaCut_noSel->nextEvent(*genKin);


    }
    if(nchgen!=0) nch_gen_noCut->Fill(nchgen);
    
    

    //---------------- RECO -------------------
    if(debug) cout<<"Starting the vertex collections ..."<<endl;
    int vtxId_offlinePV = getBestVertex(offlinePV);
    int vtxId_ferencVtx = getBestVertex(ferencVtx);
    int vtxId_pixel3Vtx = getBestVertex(pixel3Vertex);
    //vtxp_allVtx_offlinePV->fill(*offlinePV);
    //vtxp_allVtx_ferencVtx->fill(*ferencVtx);
    //if(vtxId<0) continue;
    
    /*for(vector<MyVertex>::iterator it_vtx = offlinePV->begin();it_vtx != offlinePV->end();++it_vtx)
      if(vtxId_offlinePV==it_vtx->id)
        //vtxp_PV_offlinePV->fill(*it_vtx);
    
    for(vector<MyVertex>::iterator it_vtx = ferencVtx->begin();it_vtx != ferencVtx->end();++it_vtx)
      if(vtxId_ferencVtx==it_vtx->id)
        vtxp_PV_ferencVtx->fill(*it_vtx);
    */
    
    MyVertex* beamSpot = new MyVertex();
    beamSpot->id = 0;
    beamSpot->x = bs->x;
    beamSpot->y = bs->y;
    beamSpot->z = bs->z;
    beamSpot->ex = bs->ex;
    beamSpot->ey = bs->ey;
    beamSpot->ez = bs->ez;
    beamSpot->validity = 1;
    beamSpot->fake = 0;
    beamSpot->chi2n = 0;
    beamSpot->ntracks = 0;
    offlinePV->push_back(*beamSpot);
    ferencVtx->push_back(*beamSpot);
    
    evtselp_PV_mbTr_fVtx->setSelBits(passL1(E,*L1Trig),passHF(*MITEvtSel),passVtxQual(*MITEvtSel,E),passVtx(vertexToCut),passBit40(*L1Trig));
    evtselp_PV_gTr_oVtx->setSelBits(passL1(E,*L1Trig),passHF(*MITEvtSel),passVtxQual(*MITEvtSel,E),passVtx(vertexToCut),passBit40(*L1Trig));
    evtselp_fPV_gTr_oVtx->setSelBits(passL1(E,*L1Trig),passHF(*MITEvtSel),passVtxQual(*MITEvtSel,E),passVtx(vertexToCut),passBit40(*L1Trig));
    evtselp_PV_mbTr_pVtx->setSelBits(passL1(E,*L1Trig),passHF(*MITEvtSel),passVtxQual(*MITEvtSel,E),passVtx(vertexToCut),passBit40(*L1Trig));

    evtselp_allTr_mbTr_fVtx->setSelBits(passL1(E,*L1Trig),passHF(*MITEvtSel),passVtxQual(*MITEvtSel,E),passVtx(vertexToCut),passBit40(*L1Trig));
    evtselp_allTr_gTr_oVtx->setSelBits(passL1(E,*L1Trig),passHF(*MITEvtSel),passVtxQual(*MITEvtSel,E),passVtx(vertexToCut),passBit40(*L1Trig));
    evtselp_allTr_mbTr_pVtx->setSelBits(passL1(E,*L1Trig),passHF(*MITEvtSel),passVtxQual(*MITEvtSel,E),passVtx(vertexToCut),passBit40(*L1Trig));
    
    evtselp_PV_mbTr_bs->setSelBits(passL1(E,*L1Trig),passHF(*MITEvtSel),passVtxQual(*MITEvtSel,E),passVtx(vertexToCut),passBit40(*L1Trig));
    evtselp_PV_gTr_bs->setSelBits(passL1(E,*L1Trig),passHF(*MITEvtSel),passVtxQual(*MITEvtSel,E),passVtx(vertexToCut),passBit40(*L1Trig));
    evtselp_allTr_mbTr_bs->setSelBits(passL1(E,*L1Trig),passHF(*MITEvtSel),passVtxQual(*MITEvtSel,E),passVtx(vertexToCut),passBit40(*L1Trig));
    evtselp_allTr_gTr_bs->setSelBits(passL1(E,*L1Trig),passHF(*MITEvtSel),passVtxQual(*MITEvtSel,E),passVtx(vertexToCut),passBit40(*L1Trig));
    
    if(debug) cout<<"Starting to fill all classes ..."<<endl;
    evtselp_allTr_mbTr_fVtx->fill(*minbiasTracks,*ferencVtx,bs,vtxId_ferencVtx,MITEvtSel->eClusVtxQual,MITEvtSel->ePxHits);
    evtselp_allTr_gTr_oVtx->fill(*generalTracks,*offlinePV,bs,vtxId_offlinePV,MITEvtSel->eClusVtxQual,MITEvtSel->ePxHits);
    evtselp_allTr_mbTr_pVtx->fill(*minbiasTracks,*pixel3Vertex,bs,vtxId_pixel3Vtx,MITEvtSel->eClusVtxQual,MITEvtSel->ePxHits);
    
    evtselp_allTr_mbTr_bs->fill(*minbiasTracks,*ferencVtx,bs,0,MITEvtSel->eClusVtxQual,MITEvtSel->ePxHits);
    evtselp_allTr_gTr_bs->fill(*generalTracks,*offlinePV,bs,0,MITEvtSel->eClusVtxQual,MITEvtSel->ePxHits);
    
    if(debug) cout<<"Starting to fill good oVtx ..."<<endl;
    vector<MyTracks> trcoll = getPrimaryTracks(*generalTracks,0);
    evtselp_PV_gTr_bs->fill(trcoll,*offlinePV,bs,0,MITEvtSel->eClusVtxQual,MITEvtSel->ePxHits);
    
    trcoll = getPrimaryTracks(*minbiasTracks,ferencVtx,0,bs);
    evtselp_PV_mbTr_bs->fill(trcoll,*ferencVtx,bs,0,MITEvtSel->eClusVtxQual,MITEvtSel->ePxHits);
    
    if(debug) cout<<"Starting to fill good oVtx ..."<<endl;
    
    //if(vtxId_offlinePV>=0){
      //trp_allTr_gTr_oVtx->fill(*generalTracks,*offlinePV,vtxId_offlinePV,bs);
      //trp_allTr_mbTr_oVtx->fill(*minbiasTracks,*offlinePV,vtxId_offlinePV,bs);
      
      trcoll = getPrimaryTracks(*generalTracks,offlinePV);
      //trp_PV_gTr_oVtx->fill( trcoll , *offlinePV, vtxId_offlinePV,bs);
      evtselp_PV_gTr_oVtx->fill(trcoll,*offlinePV,bs,vtxId_offlinePV,MITEvtSel->eClusVtxQual,MITEvtSel->ePxHits);
      
      trcoll = getPrimaryTracks(*generalTracks, offlinePV, bs);
      evtselp_fPV_gTr_oVtx->fill(trcoll,*offlinePV,bs,vtxId_offlinePV,MITEvtSel->eClusVtxQual,MITEvtSel->ePxHits);
      
      trcoll = getPrimaryTracks(*minbiasTracks, offlinePV);
      //trp_PV_mbTr_oVtx->fill(trcoll , *offlinePV, vtxId_offlinePV,bs);
    //}
    
    if(debug) cout<<"Starting to fill good fVtx ..."<<endl;
    
    //if(vtxId_ferencVtx>=0){
      //trp_allTr_gTr_fVtx->fill(*generalTracks, *ferencVtx, vtxId_ferencVtx,bs);
      //trp_allTr_mbTr_fVtx->fill(*minbiasTracks, *ferencVtx,vtxId_ferencVtx,bs);
    
      trcoll = getPrimaryTracks(*generalTracks,ferencVtx,bs);
      //trp_PV_gTr_fVtx->fill( trcoll , *ferencVtx, vtxId_ferencVtx , bs);
      
      trcoll = getPrimaryTracks(*minbiasTracks,ferencVtx,bs);
      //trp_PV_mbTr_fVtx->fill( trcoll , *ferencVtx, vtxId_ferencVtx , bs);
      evtselp_PV_mbTr_fVtx->fill(trcoll,*ferencVtx,bs,vtxId_ferencVtx,MITEvtSel->eClusVtxQual,MITEvtSel->ePxHits);

      trcoll = getPrimaryTracks(*minbiasTracks,pixel3Vertex,bs);
      evtselp_PV_mbTr_pVtx->fill(trcoll,*pixel3Vertex,bs,vtxId_pixel3Vtx,MITEvtSel->eClusVtxQual,MITEvtSel->ePxHits);


    //}


    //------------- ZERO BIN TRACKS -----------------------------

    //if( isEvtGood( E, *L1Trig , *MITEvtSel , vertexToCut )  ) {  
    if( isEvtGoodNoVtx( E, *L1Trig , *MITEvtSel )  ) {

      // general Tracks
      if(getnPrimaryTracks(generalTracks,offlinePV)==0){
        ++nevt_ntr0_gTr;

        // no track
        if(generalTracks->size()==0)
          ++nevt_ntr0_gTr_notr;

        // with vtx  
        if ( passVtx(vertexToCut) ) {
          trp_ntr0_gTr_withvtx->fill(*generalTracks,*offlinePV,vtxId_offlinePV,bs);
          if(generalTracks->size()==0)
            ++nevt_ntr0_gTr_withvtx_notr;

        // no vtx
        } else {
           ++nevt_ntr0_gTr_novtx;
           trp_ntr0_gTr_novtx->fill(*generalTracks,*offlinePV,0,bs);         
          if(generalTracks->size()==0)
            ++nevt_ntr0_gTr_novtx_notr;
          else
            ++nevt_ntr0_gTr_novtx_withtr;
        }
      }

      // minbias Tracks
      if(getnPrimaryTracks(minbiasTracks,ferencVtx,bs)==0){
        ++nevt_ntr0_mbTr;

        // no track
        if(minbiasTracks->size()==0)
          ++nevt_ntr0_mbTr_notr;
    
        // with vtx  
        if ( passVtx(vertexToCut) ) {
          trp_ntr0_mbTr_withvtx->fill(*minbiasTracks,*ferencVtx,vtxId_ferencVtx,bs);
          if(minbiasTracks->size()==0)
            ++nevt_ntr0_mbTr_withvtx_notr;

        // no vtx
        } else {
           ++nevt_ntr0_mbTr_novtx;
           trp_ntr0_mbTr_novtx->fill(*minbiasTracks,*ferencVtx,0,bs);
          if(minbiasTracks->size()==0)
            ++nevt_ntr0_mbTr_novtx_notr;
          else
            ++nevt_ntr0_mbTr_novtx_withtr;
        }
      }


    }
    
    //------------- FILLING MULTI && MATRIX----------------------- 
    if(debug) cout<<"Starting to fill Mtx ..."<<endl;
    
    if( isEvtGood( E, *L1Trig , *MITEvtSel , vertexToCut) ){
      
      if(isEvtGood( E,*L1Trig , *MITEvtSel , vertexToCut) && getnInAccTracks(generalTracks,0,eta_cut)!=0)
        laccept_reco_etaCut = true;
      
      trcoll = getPrimaryTracks(*minbiasTracks,ferencVtx,bs);
      for(vector<MyTracks>::iterator it_tr = trcoll.begin() ; it_tr != trcoll.end() ; ++it_tr){
        mp_PV_mbTr_fVtx->fill(it_tr->Part);
	mtxp_etaGenCut_L1_hf_VtxSel_PV_mbTr_fVtx->fillReco(it_tr->Part);
	mtxp_eta_ptGenCut_L1_hf_VtxSel_PV_mbTr_fVtx->fillReco(it_tr->Part);
      }
      mp_PV_mbTr_fVtx->nextEvent();
      mtxp_etaGenCut_L1_hf_VtxSel_PV_mbTr_fVtx->nextEvent(laccept_reco_etaCut , laccept_etaCut_NSD);
      mtxp_eta_ptGenCut_L1_hf_VtxSel_PV_mbTr_fVtx->nextEvent(laccept_reco_etaCut , laccept_pt_etaCut_NSD);
      nchVSnvtx->Fill(trcoll.size() , ferencVtx->size());
      
      
      trcoll = getPrimaryTracks(*generalTracks,offlinePV);
      for(vector<MyTracks>::iterator it_tr = trcoll.begin() ; it_tr != trcoll.end() ; ++it_tr){
        mp_PV_gTr_oVtx->fill(it_tr->Part);
	mtxp_etaGenCut_L1_hf_VtxSel_PV_gTr_oVtx->fillReco(it_tr->Part);
	mtxp_eta_ptGenCut_L1_hf_VtxSel_PV_gTr_oVtx->fillReco(it_tr->Part);
      }
      mp_PV_gTr_oVtx->nextEvent();
      mtxp_etaGenCut_L1_hf_VtxSel_PV_gTr_oVtx->nextEvent(true , laccept_etaCut_NSD);
      mtxp_eta_ptGenCut_L1_hf_VtxSel_PV_gTr_oVtx->nextEvent(laccept_reco_etaCut , laccept_pt_etaCut_NSD);
     
     
      //Checking evts with 2 vtx
      ferencVtx->pop_back();
      if(ferencVtx->size()==2){
        
	int vtxId_otherVtx = 0;
        for(vector<MyVertex>::iterator it_vtx = ferencVtx->begin();it_vtx != ferencVtx->end();++it_vtx){
          if(vtxId_ferencVtx==it_vtx->id)
            vtxp_2vtx_vtx1->fill(*it_vtx);
	  if(vtxId_ferencVtx!=it_vtx->id){
            vtxId_otherVtx = it_vtx->id;
	    vtxp_2vtx_vtx2->fill(*it_vtx);
	  }
	}
	
	int nch_vtx1  = 0 , nch_vtx2 = 0;
	for(vector<MyTracks>::iterator it_tr = minbiasTracks->begin() ; it_tr != minbiasTracks->end() ; ++it_tr){
          if( isTrackPrimary(*it_tr , *ferencVtx ,vtxId_ferencVtx , bs) && isInAcceptance(it_tr->Part,pt_cut,eta_cut,0) )
            ++nch_vtx1;
          if( isTrackPrimary(*it_tr , *ferencVtx ,vtxId_otherVtx  , bs) && isInAcceptance(it_tr->Part,pt_cut,eta_cut,0) )
            ++nch_vtx2;
      
        }
	nch_2vtx_vtx1->Fill(nch_vtx1);
	nch_2vtx_vtx2->Fill(nch_vtx2);
	
        zdiff_2vtx->Fill(ferencVtx->at(0).z - ferencVtx->at(1).z);
	if(isMC){
	  int nch_gen_2vtx = 0;
          for(vector<MyGenPart>::iterator p=genPart->begin() ; p!=genPart->end() ; p++ ){
            if ( isGenPartGood(*p) ){
	      if( fabs(p->Part.v.Eta()) < eta_cut && p->Part.v.Pt()> pt_cut ){
	        gmp_2vtx->fill(*genKin,p->Part);
		++nch_gen_2vtx;
              }
	    }
	  }
          gmp_2vtx->nextEvent(*genKin);
	  
	  
	  if(nch_gen_2vtx!=0){
	    ratio_vtx1Ogen->Fill(double(nch_vtx1) / double(nch_gen_2vtx));
	    ratio_vtx2Ogen->Fill(double(nch_vtx2) / double(nch_gen_2vtx));
	  }
	  nch_vtx1VSgen->Fill(nch_gen_2vtx , nch_vtx1 );
	  nch_vtx2VSgen->Fill(nch_gen_2vtx , nch_vtx2 );
	}
	
      }
    }
  
  
  }//end of loop over entries from tree

  //file->Close();


  if(debug) cout<<"Starting to write to file ..."<<endl;
  
  //output file
  ostringstream PT("");
  PT << "ptcut" << pt_cut;
  if(filename!="NONE") PT <<"_"<< filename;
  TString outputfilename = fileManager(1,type,E,0,0,0,PT.str());
  cout << "Output file : " << outputfilename << endl;
  TFile* file2=new TFile(outputfilename,"RECREATE");
  file2->cd();
  
  if(isMC){
    nch_gen_noCut->Write();
    pt_gen_noCut->Write();
    eta_gen_noCut->Write();
  }
  
  
  vtxp_2vtx_vtx1->write();
  vtxp_2vtx_vtx2->write();
  nch_2vtx_vtx1->Write();
  nch_2vtx_vtx2->Write();
  zdiff_2vtx->Write();
  if(isMC){
    gmp_2vtx->write();
    ratio_vtx1Ogen->Write();
    ratio_vtx2Ogen->Write();
    nch_vtx1VSgen->Write();
    nch_vtx2VSgen->Write();
  }
  
  nchVSnvtx->Write();
  //trp_allTr_gTr_oVtx->write();
  //trp_PV_gTr_oVtx->write();
  //trp_allTr_gTr_fVtx->write();
  //trp_PV_gTr_fVtx->write();
  
  //minbiastracks
  //trp_allTr_mbTr_oVtx->write();
  //trp_PV_mbTr_oVtx->write();
  //trp_allTr_mbTr_fVtx->write();
  //trp_PV_mbTr_fVtx->write();
 
  // BIN ZERO Tracks (BEFORE VTX CUT)
  trp_ntr0_gTr_novtx->write();
  trp_ntr0_gTr_withvtx->write();
  trp_ntr0_mbTr_novtx->write();
  trp_ntr0_mbTr_withvtx->write();
 
  //offlinePV
  //vtxp_allVtx_offlinePV->write();
  //vtxp_PV_offlinePV->write();
  
  //ferencVtx wrt minbiasTracks
  //vtxp_allVtx_ferencVtx->write();
  //vtxp_PV_ferencVtx->write();
  
  evtselp_PV_mbTr_fVtx->write();
  evtselp_PV_gTr_oVtx->write();
  evtselp_fPV_gTr_oVtx->write();
  evtselp_PV_mbTr_pVtx->write();
  
  evtselp_allTr_mbTr_fVtx->write();
  evtselp_allTr_gTr_oVtx->write();
  evtselp_allTr_mbTr_pVtx->write();
  
  evtselp_PV_mbTr_bs->write();
  evtselp_PV_gTr_bs->write();
  evtselp_allTr_mbTr_bs->write();
  evtselp_allTr_gTr_bs->write();
  
  evtselp_PV_mbTr_fVtx->makeEffPlots("RECO");
  evtselp_PV_gTr_oVtx->makeEffPlots("RECO");
  evtselp_fPV_gTr_oVtx->makeEffPlots("RECO");
  evtselp_allTr_mbTr_fVtx->makeEffPlots("RECO");
  evtselp_allTr_gTr_oVtx->makeEffPlots("RECO");
  evtselp_PV_mbTr_bs->makeEffPlots("RECO");
  evtselp_PV_gTr_bs->makeEffPlots("RECO");
  evtselp_allTr_mbTr_bs->makeEffPlots("RECO");
  evtselp_allTr_gTr_bs->makeEffPlots("RECO");
  
  
  //Using GEN !! only for MC
  if(isMC){
    evtselp_PV_mbTr_fVtx->setDenom(nch_gen_noCut,pt_gen_noCut,eta_gen_noCut);
    evtselp_PV_gTr_oVtx->setDenom(nch_gen_noCut,pt_gen_noCut,eta_gen_noCut);
    evtselp_fPV_gTr_oVtx->setDenom(nch_gen_noCut,pt_gen_noCut,eta_gen_noCut);
    evtselp_allTr_mbTr_fVtx->setDenom(nch_gen_noCut,pt_gen_noCut,eta_gen_noCut);
    evtselp_allTr_gTr_oVtx->setDenom(nch_gen_noCut,pt_gen_noCut,eta_gen_noCut);
    evtselp_PV_mbTr_bs->setDenom(nch_gen_noCut,pt_gen_noCut,eta_gen_noCut);
    evtselp_PV_gTr_bs->setDenom(nch_gen_noCut,pt_gen_noCut,eta_gen_noCut);
    evtselp_allTr_mbTr_bs->setDenom(nch_gen_noCut,pt_gen_noCut,eta_gen_noCut);
    evtselp_allTr_gTr_bs->setDenom(nch_gen_noCut,pt_gen_noCut,eta_gen_noCut);
  
    evtselp_PV_mbTr_fVtx->makeEffPlots("GEN");
    evtselp_PV_gTr_oVtx->makeEffPlots("GEN");
    evtselp_fPV_gTr_oVtx->makeEffPlots("GEN");
    evtselp_allTr_mbTr_fVtx->makeEffPlots("GEN");
    evtselp_allTr_gTr_oVtx->makeEffPlots("GEN");
    evtselp_PV_mbTr_bs->makeEffPlots("GEN");
    evtselp_PV_gTr_bs->makeEffPlots("GEN");
    evtselp_allTr_mbTr_bs->makeEffPlots("GEN");
    evtselp_allTr_gTr_bs->makeEffPlots("GEN");
    
    //Only for 1 set, final
    evtselp_PV_gTr_oVtx->setDenom(gmp_noCut_noSel->mp_NSD->nch,gmp_noCut_noSel->mp_NSD->pt,gmp_noCut_noSel->mp_NSD->eta);
    evtselp_PV_gTr_oVtx->makeEffPlots("GEN_NSD_noCut_noSel");
    
    evtselp_PV_gTr_oVtx->setDenom(gmp_etaCut_noSel->mp_NSD->nch,gmp_etaCut_noSel->mp_NSD->pt,gmp_etaCut_noSel->mp_NSD->eta);
    evtselp_PV_gTr_oVtx->makeEffPlots("GEN_NSD_etaCut_noSel");
    
    evtselp_PV_gTr_oVtx->setDenom(gmp_pt_etaCut_noSel->mp_NSD->nch,gmp_pt_etaCut_noSel->mp_NSD->pt,gmp_pt_etaCut_noSel->mp_NSD->eta);
    evtselp_PV_gTr_oVtx->makeEffPlots("GEN_NSD_pt_eta_noSel");
  }
  
  //-------- MULTI --------
  mp_PV_mbTr_fVtx->write();
  mp_PV_gTr_oVtx->write();
  
  if(isMC){
    gmp_noCut_noSel->write();
    gmp_ptCut_noSel->write();
    gmp_etaCut_noSel->write();
    gmp_pt_etaCut_noSel->write();
  }
  
  //------ MATRIX -------
  if(isMC){
    mtxp_etaGenCut_L1_hf_VtxSel_PV_mbTr_fVtx->write();
    mtxp_etaGenCut_L1_hf_VtxSel_PV_gTr_oVtx->write();
    mtxp_eta_ptGenCut_L1_hf_VtxSel_PV_mbTr_fVtx->write();
    mtxp_eta_ptGenCut_L1_hf_VtxSel_PV_gTr_oVtx->write();
  }


  cout<<" ---ZeroBin- SUMMARY --- gTR -----"<<endl;
  cout<<"nevts : "<<mp_PV_gTr_oVtx->nbEvts<<endl;
  cout<<"nevts with ntr assoc to PV = 0 : "<<nevt_ntr0_gTr<<"  ---> "<<double(nevt_ntr0_gTr)/double(mp_PV_gTr_oVtx->nbEvts)*100.<<" %"<<endl;
  cout<<"nevts with ntr assoc to PV = 0 & no good vtx: "<<nevt_ntr0_gTr_novtx<<"  ---> "<<double(nevt_ntr0_gTr_novtx)/double(nevt_ntr0_gTr)*100.<<" %"<<endl;
  cout<<"nevts with ntr assoc to PV = 0 & good vtx & no tr: "<<nevt_ntr0_gTr_withvtx_notr<<"  ---> "<<double(nevt_ntr0_gTr_withvtx_notr)/double(nevt_ntr0_gTr)*100.<<" %"<<endl;
  cout<<"nevts with ntr assoc to PV = 0 & no good vtx & no tr: "<<nevt_ntr0_gTr_novtx_notr<<"  ---> "<<double(nevt_ntr0_gTr_novtx_notr)/double(nevt_ntr0_gTr)*100.<<" %"<<endl;
  cout<<"nevts with ntr assoc to PV = 0 & no good vtx & with tr: "<<nevt_ntr0_gTr_novtx_withtr<<"  ---> "<<double(nevt_ntr0_gTr_novtx_withtr)/double(nevt_ntr0_gTr)*100.<<" %"<<endl;
  cout<<"nevts with ntr assoc to PV = 0 & no vtx at all: "<<nevt_ntr0_gTr_novtxatall<<"  ---> "<<double(nevt_ntr0_gTr_novtxatall)/double(nevt_ntr0_gTr)*100.<<" %"<<endl;
  cout<<"nevts with ntr assoc to PV = 0 & no tr: "<<nevt_ntr0_gTr_notr<<"  ---> "<<double(nevt_ntr0_gTr_notr)/double(nevt_ntr0_gTr)*100.<<" %"<<endl;

  cout<<" ---ZeroBin- SUMMARY ---mbTR -----"<<endl;
  cout<<"nevts : "<<mp_PV_gTr_oVtx->nbEvts<<endl;
  cout<<"nevts with ntr assoc to PV = 0 : "<<nevt_ntr0_mbTr<<"  ---> "<<double(nevt_ntr0_mbTr)/double(mp_PV_gTr_oVtx->nbEvts)*100.<<" %"<<endl;
  cout<<"nevts with ntr assoc to PV = 0 & no good vtx: "<<nevt_ntr0_mbTr_novtx<<"  ---> "<<double(nevt_ntr0_mbTr_novtx)/double(nevt_ntr0_mbTr)*100.<<" %"<<endl;
  cout<<"nevts with ntr assoc to PV = 0 & good vtx & no tr: "<<nevt_ntr0_mbTr_withvtx_notr<<"  ---> "<<double(nevt_ntr0_mbTr_withvtx_notr)/double(nevt_ntr0_mbTr)*100.<<" %"<<endl;
  cout<<"nevts with ntr assoc to PV = 0 & no good vtx & no tr: "<<nevt_ntr0_mbTr_novtx_notr<<"  ---> "<<double(nevt_ntr0_mbTr_novtx_notr)/double(nevt_ntr0_mbTr)*100.<<" %"<<endl;
  cout<<"nevts with ntr assoc to PV = 0 & no good vtx & with tr: "<<nevt_ntr0_mbTr_novtx_withtr<<"  ---> "<<double(nevt_ntr0_mbTr_novtx_withtr)/double(nevt_ntr0_mbTr)*100.<<" %"<<endl;
  cout<<"nevts with ntr assoc to PV = 0 & no vtx at all: "<<nevt_ntr0_mbTr_novtxatall<<"  ---> "<<double(nevt_ntr0_mbTr_novtxatall)/double(nevt_ntr0_mbTr)*100.<<" %"<<endl;
  cout<<"nevts with ntr assoc to PV = 0 & no tr: "<<nevt_ntr0_mbTr_notr<<"  ---> "<<double(nevt_ntr0_mbTr_notr)/double(nevt_ntr0_mbTr)*100.<<" %"<<endl;

  
  // CLOSE FILE !!  
  file2->Close();
}
