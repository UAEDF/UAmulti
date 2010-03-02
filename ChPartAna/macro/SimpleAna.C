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
#include "../plugins/MultiPlots.h"
#include "../plugins/GenMultiPlots.h"
#include "../plugins/MatrixPlots.h"

bool debug = false;

TString filename = "MC_test_test_900GeV";
bool isMC = true;



#include "evtSel.C"
#include "cuts.C"
void SimpleAna(bool = true , double = 0.9 , TString = "MC_test_900GeV" , int = 20000 , int = 0 );

void SimpleAna(bool ismc , double E , TString filename , int nevt_max , int irun)
{
  isMC = ismc;
  
  int nbinmulti = 70;
  
  //----------------------  RECO  ----------------------
  GenMultiPlots* gmp_evtSel_reco  = new GenMultiPlots("evtSel_reco");
  MultiPlots* mp_evtSel_INC_reco  = new MultiPlots("evtSel_INC_reco");
  
  
  //----------------------   GEN  ----------------------
  GenMultiPlots* gmp_evtSel           = new GenMultiPlots("evtSel");
  MultiPlots* mp_etaCut_noSel_NSD_gen = new MultiPlots("etaCut_noSel_NSD_gen");
  GenMultiPlots* gmp_etaCut           = new GenMultiPlots("etaCut");
  
  //------------ MultiPlots ------------------
  MultiPlots* mp_PV_mbTr_fVtx    = new MultiPlots("evtSel_PV_mbTr_fVtx");
  MultiPlots* mp_PV_gTr_oVtx     = new MultiPlots("evtSel_PV_gTr_oVtx");
    
  TrackPlots* trp = new TrackPlots("");
  VertexPlots* vtxp = new VertexPlots("");
  
  
  
  //------------ MatrixPlots ------------------
  MatrixPlots* mtxp_etaGenCut_L1_hf_VtxSel_PV_gTr_oVtx = new MatrixPlots("etaGenCut_L1_hf_VtxSel_PV_gTr_oVtx");
  
  
  
  //------------- Checking when no PV assoc tracks -------------
  TrackPlots* trp_ntr0_novtx = new TrackPlots("ntr0_novtx");
  TrackPlots* trp_ntr0_withvtx = new TrackPlots("ntr0_withvtx");
  VertexPlots* vtxp_ntr0 = new VertexPlots("ntr0");
  int nevt_ntr0 = 0 , nevt_ntr0_novtx = 0 , nevt_ntr0_notr = 0 , nevt_ntr0_novtxatall = 0;
  int nevt_ntr0_novtx_notr = 0 , nevt_ntr0_novtx_withtr = 0, nevt_ntr0_withvtx_notr = 0 ;
  
  //-----
  TH1F* L1_before = new TH1F("L1_before","L1_before",nbinmulti+1,-0.5,nbinmulti+0.5);
  TH1F* L1_after = new TH1F("L1_after","L1_after",nbinmulti+1,-0.5,nbinmulti+0.5);
  L1_before->Sumw2();
  L1_after->Sumw2();
  TH1F* hf_before = new TH1F("hf_before","hf_before",nbinmulti+1,-0.5,nbinmulti+0.5);
  TH1F* hf_after = new TH1F("hf_after","hf_after",nbinmulti+1,-0.5,nbinmulti+0.5);
  hf_before->Sumw2();
  hf_after->Sumw2();
  TH1F* vtxqual_before = new TH1F("vtxqual_before","vtxqual_before",nbinmulti+1,-0.5,nbinmulti+0.5);
  TH1F* vtxqual_after = new TH1F("vtxqual_after","vtxqual_after",nbinmulti+1,-0.5,nbinmulti+0.5);
  vtxqual_before->Sumw2();
  vtxqual_after->Sumw2();
  TH1F* vtx_before = new TH1F("vtx_before","vtx_before",nbinmulti+1,-0.5,nbinmulti+0.5);
  TH1F* vtx_after = new TH1F("vtx_after","vtx_after",nbinmulti+1,-0.5,nbinmulti+0.5);
  vtx_before->Sumw2();
  vtx_after->Sumw2();
  TH1F* evtSel_before = new TH1F("evtSel_before","evtSel_before",nbinmulti+1,-0.5,nbinmulti+0.5);
  TH1F* evtSel_after = new TH1F("evtSel_after","evtSel_after",nbinmulti+1,-0.5,nbinmulti+0.5);
  evtSel_before->Sumw2();
  evtSel_after->Sumw2();
  
  TChain* tree = new TChain("evt","");
  
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
  //tree->SetBranchAddress("ferencVtxFerTrk",&ferencVtx);
  tree->SetBranchAddress("pixel3Vertex",&pixel3Vertex);
  tree->SetBranchAddress("L1Trig",&L1Trig);
  tree->SetBranchAddress("MITEvtSel",&MITEvtSel);
  tree->SetBranchAddress("beamSpot",&bs);
  
  
  int nev = int(tree->GetEntries());
  std::cout <<"number of entries is : "<< nev << std::endl;
  cout<<"Running on: "<<min(nev,nevt_max)<<" events"<<endl;
  // Event TYPE counting --> Weights
  
  
  TH1F* nch_test = new TH1F("test","test",71,-0.5,70.5);
  
  for(int i = 0; i < nev; i++){
     
    if( ((i+1) % 10000) == 0) cout <<int(double(i+1)/double(min(nev,nevt_max))*100.)<<" % done"<<endl;
    
    if(i>min(nev,nevt_max)) break;
    
    tree->GetEntry(i);
    
    
    if(i==0) vertexToCut = pixel3Vertex;
    
    //Selection of good BX for data && MC
    if(!isMC && !goodBX(evtId->Run,evtId->Bunch)) continue;
    
    if(!isMC)
      if(irun>0)
        if(irun!=evtId->Run)
	  continue;
    
    
    //test
    if(isMC){
      if(!isSD(genKin)){
        int nch = 0;
	bool laccept = false;
        for(vector<MyGenPart>::iterator p=genPart->begin() ; p!=genPart->end() ; p++ ){
	  if(isGenPartGood(*p)) laccept = true;
          if(isGenPartGood(*p) && fabs(p->Part.v.Eta())  < eta_cut )
	    ++nch;
	}
        if(laccept) nch_test->Fill(nch);
      //nch_test->Fill(getnPrimaryGenPart(genPart,0,eta_cut));
	//cout<<getnPrimaryGenPart(genPart,0,eta_cut)<<endl;
      }
    }
    
    //----------------------------------------------------------------------
    //----------------------- USED TO SUBSTRACT SD -------------------------
    //----------------------------------------------------------------------
    
    if(isEvtGood(*L1Trig , *MITEvtSel) && getBestVertex(vertexToCut)>-1){
      if(isMC){
        for(vector<MyGenPart>::iterator p=genPart->begin() ; p!=genPart->end() ; p++ )
          if ( isGenPartGood(*p) && fabs(p->Part.v.Eta())  < eta_cut && p->Part.v.Pt() > pt_cut )
            gmp_evtSel->fill(*genKin , p->Part);
        gmp_evtSel->nextEvent(*genKin);
      }
      
        //Making a GenMulti for RECO
      vector<MyTracks> trcoll = getPrimaryTracks(*generalTracks,offlinePV);
      for(vector<MyTracks>::iterator it_tr = trcoll.begin() ; it_tr != trcoll.end() ; ++it_tr){
        if(isMC) gmp_evtSel_reco->fill(*genKin , it_tr->Part);
	mp_evtSel_INC_reco->fill(it_tr->Part);
      }
      if(isMC) gmp_evtSel_reco->nextEvent(*genKin);
      mp_evtSel_INC_reco->nextEvent();
    }
    
    
    //Skipping the SD events starting from here
    if(isMC)
      if(isSD(genKin) ) continue;
    
    /*
    
    //----------------------------------------------------------------------
    //----------------------- DOING THE SELECTIONS -------------------------
    //-----------------------          RECO        -------------------------
    //----------------------------------------------------------------------
    
    if(isMC) evtSel_before->Fill(getnPrimaryTracks(generalTracks,offlinePV));
    
    //L1 CUT
    if(isMC) L1_before->Fill(getnPrimaryTracks(generalTracks,offlinePV));
    if(passL1(*L1Trig))
      if(isMC) L1_after->Fill(getnPrimaryTracks(generalTracks,offlinePV));
    
    //HF CUT
    if(isMC) hf_before->Fill(getnPrimaryTracks(generalTracks,offlinePV));
    if(passHF(*MITEvtSel))
      if(isMC) hf_after->Fill(getnPrimaryTracks(generalTracks,offlinePV));
    
    //vtxqual CUT
    if(isMC) vtxqual_before->Fill(getnPrimaryTracks(generalTracks,offlinePV));
    if(passVtxQual(*MITEvtSel))
      if(isMC) vtxqual_after->Fill(getnPrimaryTracks(generalTracks,offlinePV));
    
    //Taking events with only 1 good vertex
    if(isMC) vtx_before->Fill(getnPrimaryTracks(generalTracks,offlinePV));
    if(getBestVertex(vertexToCut)>-1)
      if(isMC) vtx_after->Fill(getnPrimaryTracks(generalTracks,offlinePV));
    
    //if(!isEvtGood(*L1Trig , *MITEvtSel) || getBestVertex(vertexToCut)==-1) continue;
    if(isEvtGood(*L1Trig , *MITEvtSel) && getBestVertex(vertexToCut)>-1)
      if(isMC) evtSel_after->Fill(getnPrimaryTracks(generalTracks,offlinePV));
    
    */
    
    
    
    //----------------------------------------------------------------------
    //----------------------- DOING THE SELECTIONS -------------------------
    //-----------------------          GEN         -------------------------
    //----------------------------------------------------------------------
    
    if(isMC){
      for(vector<MyGenPart>::iterator p=genPart->begin() ; p!=genPart->end() ; p++ )
        if ( isGenPartGood(*p) && fabs(p->Part.v.Eta())  < eta_cut )
          mp_etaCut_noSel_NSD_gen->fill(p->Part);
      mp_etaCut_noSel_NSD_gen->nextEvent();
    }
    
    if(isMC) evtSel_before->Fill(getnPrimaryGenPart(genPart,0,eta_cut));
    
    //L1 CUT
    if(isMC) L1_before->Fill(getnPrimaryGenPart(genPart,0,eta_cut));
    if(passL1(*L1Trig))
      if(isMC) L1_after->Fill(getnPrimaryGenPart(genPart,0,eta_cut));
    
    //HF CUT
    if(isMC) hf_before->Fill(getnPrimaryGenPart(genPart,0,eta_cut));
    if(passHF(*MITEvtSel))
      if(isMC) hf_after->Fill(getnPrimaryGenPart(genPart,0,eta_cut));
    
    //vtxqual CUT
    if(isMC) vtxqual_before->Fill(getnPrimaryGenPart(genPart,0,eta_cut));
    if(passVtxQual(*MITEvtSel))
      if(isMC) vtxqual_after->Fill(getnPrimaryGenPart(genPart,0,eta_cut));
    
    //Taking events with only 1 good vertex
    if(isMC) vtx_before->Fill(getnPrimaryGenPart(genPart,0,eta_cut));
    if(getBestVertex(vertexToCut)>-1)
      if(isMC) vtx_after->Fill(getnPrimaryGenPart(genPart,0,eta_cut));
    
    if(!isEvtGood(*L1Trig , *MITEvtSel) || getBestVertex(vertexToCut)==-1) continue;
    //if(isEvtGood(*L1Trig , *MITEvtSel) && getBestVertex(vertexToCut)>-1)
    if(isMC) evtSel_after->Fill(getnPrimaryGenPart(genPart,0,eta_cut));
    
    
    
    
    
    //----------------------------------------------------------------------
    //-----------------------         GEN          -------------------------
    //----------------------------------------------------------------------
    
    
    int nchgen = 0;
    bool laccept_etaCut = false;
    bool laccept_etaCut_NSD = false;
    
    bool laccept_reco_etaCut = false;
    
    if(isMC){
    
      
      for(vector<MyGenPart>::iterator p=genPart->begin() ; p!=genPart->end() ; p++ ){
        if ( isGenPartGood(*p)){
	  ++nchgen;
	  
	  
	  //eta cut
	  if( fabs(p->Part.v.Eta())  < eta_cut ){
	    gmp_etaCut->fill(*genKin,p->Part);
	    laccept_etaCut = true;
	  }
	  
	  //eta cut + good evt sel
	  if(fabs(p->Part.v.Eta()) < eta_cut && !isSD(genKin)){
	    mtxp_etaGenCut_L1_hf_VtxSel_PV_gTr_oVtx->fillGen(p->Part);
	    laccept_etaCut_NSD = true;
	    
	  }
	}
      }
      
      //gmp_etaCut->nextEvent(*genKin , laccept_etaCut_NSD);
      gmp_etaCut->nextEvent(*genKin);
    }
    



    //----------------------------------------------------------------------
    //-----------------------         RECO         -------------------------
    //----------------------------------------------------------------------
    if(debug) cout<<"Starting the vertex collections ..."<<endl;
    int vtxId_offlinePV = getBestVertex(offlinePV);
    
    //------------- FILLING MULTI && MATRIX----------------------- 
    if(debug) cout<<"Starting to fill RECO ..."<<endl;
    
    {
      
      //Check Vertex ntracks for nPrimaryInAcc = 1
      vector<MyVertex>::iterator goodVtx = offlinePV->end();
      for(vector<MyVertex>::iterator it_vtx = offlinePV->begin();it_vtx != offlinePV->end();++it_vtx)
        if(vtxId_offlinePV==it_vtx->id)
          goodVtx = it_vtx;
      vtxp->fill(*goodVtx);
      //Check for ntracks in evt when PV tracks==1
      /*if(getnPrimaryTracks(generalTracks,offlinePV)==1 && goodVtx!= offlinePV->end()){
        cout<<"Best valid Vtx, after L1+HF+VtxQual, with 1 track assoc in eta & pt acceptance: "<<endl;
        cout<<"---->  #Tracks associated to the chosen vertex: "<<goodVtx->ntracks<<endl;
        cout<<"---->  #Tracks associated to PV (no pT/eta cut): "<<getnPrimaryTracks(generalTracks,offlinePV,0,10)<<endl;
      }*/
      
      
      //------------------------
      //RECO BIN 0
      if(getnPrimaryTracks(generalTracks,offlinePV)==0){
        ++nevt_ntr0;
	if(generalTracks->size()==0)
	  ++nevt_ntr0_notr;
	  
	bool hasValidVtx = false;
	for(vector<MyVertex>::iterator it_vtx = offlinePV->begin();it_vtx != offlinePV->end();++it_vtx)
	  if(it_vtx->validity && it_vtx->ntracks!=0)
	    hasValidVtx = true;
	if(!hasValidVtx)
	  ++nevt_ntr0_novtxatall;
	  
	if(vtxId_offlinePV==-1){
	  ++nevt_ntr0_novtx;
	  trp_ntr0_novtx->fill(*generalTracks,*offlinePV,0,bs);
	  if(generalTracks->size()==0)
	    ++nevt_ntr0_novtx_notr;
	  else
	    ++nevt_ntr0_novtx_withtr;
	}
	else{
          trp_ntr0_withvtx->fill(*generalTracks,*offlinePV,vtxId_offlinePV,bs);
          vtxp_ntr0->fill(*goodVtx);
	  if(generalTracks->size()==0)
	    ++nevt_ntr0_withvtx_notr;
	}
      }
      
      //-----------------------------
      //FILLING THE MATRIX && MULTI
      
      if(getnInAccTracks(generalTracks,0,eta_cut)!=0)
        laccept_reco_etaCut = true;
	
      vector<MyTracks> trcoll = getPrimaryTracks(*generalTracks,offlinePV);
      for(vector<MyTracks>::iterator it_tr = trcoll.begin() ; it_tr != trcoll.end() ; ++it_tr){
	if(isMC)
	  if(!isSD(genKin))
	    mtxp_etaGenCut_L1_hf_VtxSel_PV_gTr_oVtx->fillReco(it_tr->Part);
	mp_PV_gTr_oVtx->fill(it_tr->Part);
	trp->fill(trcoll,*offlinePV,vtxId_offlinePV,bs);
      }
            
      //mtxp_etaGenCut_L1_hf_VtxSel_PV_gTr_oVtx->nextEvent(laccept_reco_etaCut , laccept_etaCut_NSD);
      if(isMC) mtxp_etaGenCut_L1_hf_VtxSel_PV_gTr_oVtx->nextEvent();//true,!isSD(genKin));
      //mp_PV_gTr_oVtx->nextEvent(laccept_reco_etaCut);
      mp_PV_gTr_oVtx->nextEvent();
    
    }//end of old if(isEvtGood) !!REMOVED
  
  
  }//end of loop over entries from tree

  //file->Close();


  if(debug) cout<<"Starting to write to file ..."<<endl;
  
  //output file
  TFile* file2=new TFile("collectionPlotter_"+filename+".root","RECREATE");
  file2->cd();
  
  cout<<"mp : "<<mp_PV_gTr_oVtx->nbEvts<<endl;
  cout<<"mtxp : "<<mtxp_etaGenCut_L1_hf_VtxSel_PV_gTr_oVtx->nbEvts<<endl;
  cout<<"gmp NSD : "<<gmp_etaCut->mp_NSD->nbEvts<<endl<<endl;
  cout<<"gmp : "<<gmp_evtSel->mp_NSD->nbEvts<<endl;
  cout<<"gmp RECO : "<<gmp_evtSel_reco->mp_NSD->nbEvts<<endl;
  cout<<"mp INC RECO : "<<mp_evtSel_INC_reco->nbEvts<<endl;
  
  
  //------ MATRIX -------
  bool scale = false;
  if(isMC){
    gmp_etaCut->write(scale);
    mtxp_etaGenCut_L1_hf_VtxSel_PV_gTr_oVtx->write(scale);
    mp_etaCut_noSel_NSD_gen->write(scale);
    gmp_evtSel->write(scale);
    
    gmp_evtSel_reco->write(scale);
  }
  
  mp_evtSel_INC_reco->write(scale);
  mp_PV_gTr_oVtx->write(scale);
  trp->write();
  vtxp->write();
  
  trp_ntr0_novtx->write();
  trp_ntr0_withvtx->write();
  vtxp_ntr0->write();
  
  
  TH1F* eff_L1 = (TH1F*) L1_after->Clone("eff_L1");
  eff_L1->Divide(L1_before);
  L1_before->Write();
  L1_after->Write();
  eff_L1->Write();
  
  TH1F* eff_hf = (TH1F*) hf_after->Clone("eff_hf");
  eff_hf->Divide(hf_before);
  hf_before->Write();
  hf_after->Write();
  eff_hf->Write();
  
  TH1F* eff_vtxqual = (TH1F*) vtxqual_after->Clone("eff_vtxqual");
  eff_vtxqual->Divide(vtxqual_before);
  vtxqual_before->Write();
  vtxqual_after->Write();
  eff_vtxqual->Write();
  
  TH1F* eff_vtx = (TH1F*) vtx_after->Clone("eff_vtx");
  eff_vtx->Divide(vtx_before);
  vtx_before->Write();
  vtx_after->Write();
  eff_vtx->Write();
  
  TH1F* eff_evtSel = (TH1F*) evtSel_after->Clone("eff_evtSel");
  eff_evtSel->Divide(evtSel_before);
  evtSel_before->Write();
  evtSel_after->Write();
  eff_evtSel->Write();
  
  
  
  nch_test->Write();
  
  cout<<" ----------- SUMMARY -------------"<<endl;
  cout<<"nevts : "<<mp_PV_gTr_oVtx->nbEvts<<endl;
  cout<<"nevts with ntr assoc to PV = 0 : "<<nevt_ntr0<<"  ---> "<<double(nevt_ntr0)/double(mp_PV_gTr_oVtx->nbEvts)*100.<<" %"<<endl;
  cout<<"nevts with ntr assoc to PV = 0 & no good vtx: "<<nevt_ntr0_novtx<<"  ---> "<<double(nevt_ntr0_novtx)/double(nevt_ntr0)*100.<<" %"<<endl;
  cout<<"nevts with ntr assoc to PV = 0 & good vtx & no tr: "<<nevt_ntr0_withvtx_notr<<"  ---> "<<double(nevt_ntr0_withvtx_notr)/double(nevt_ntr0)*100.<<" %"<<endl;
  cout<<"nevts with ntr assoc to PV = 0 & no good vtx & no tr: "<<nevt_ntr0_novtx_notr<<"  ---> "<<double(nevt_ntr0_novtx_notr)/double(nevt_ntr0)*100.<<" %"<<endl;
  cout<<"nevts with ntr assoc to PV = 0 & no good vtx & with tr: "<<nevt_ntr0_novtx_withtr<<"  ---> "<<double(nevt_ntr0_novtx_withtr)/double(nevt_ntr0)*100.<<" %"<<endl;
  cout<<"nevts with ntr assoc to PV = 0 & no vtx at all: "<<nevt_ntr0_novtxatall<<"  ---> "<<double(nevt_ntr0_novtxatall)/double(nevt_ntr0)*100.<<" %"<<endl;
  cout<<"nevts with ntr assoc to PV = 0 & no tr: "<<nevt_ntr0_notr<<"  ---> "<<double(nevt_ntr0_notr)/double(nevt_ntr0)*100.<<" %"<<endl;
  
  
  
  //file2->Close();
}
