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
#include "../plugins/MatrixPlots.h"

bool debug = false;

TString filename = "MC_everything";
bool isMC = true;



#include "evtSel.C"
#include "cuts.C"


void collectionPlotter()
{
  // General variables
  int nbinmulti = 45;
  TH1F* nch_gen = new TH1F("nch_gen","nch_gen;n_{CH};# events",(int)nbinmulti+1,-0.5,nbinmulti+0.5);
  TH1F* pt_gen  = new TH1F("pt_gen","pt_gen;pT [GeV];# events",100,0.,3.);
  TH1F* eta_gen = new TH1F("eta_gen","eta_gen;#eta;# events",60,-3.,3.);

  //pt    = new TH1F("pt_"+trackcoll,"pt_"+trackcoll+";pt [GeV];# events",100,0.,3.);
  //eta   = new TH1F("eta_"+trackcoll,"eta_"+trackcoll+";#eta;# events",60,-3.,3.);
  
  //------------- TRACK COLLECTIONS -------------
  //generaltracks
  TrackPlots* trp_allTr_gTr_oVtx = new TrackPlots("allTr_gTr_oVtx");
  TrackPlots* trp_PV_gTr_oVtx    = new TrackPlots("PV_gTr_oVtx");
  TrackPlots* trp_allTr_gTr_fVtx = new TrackPlots("allTr_gTr_fVtx");
  TrackPlots* trp_PV_gTr_fVtx    = new TrackPlots("PV_gTr_fVtx");
  
  //minbiastracks
  TrackPlots* trp_allTr_mbTr_oVtx = new TrackPlots("allTr_mbTr_oVtx");
  TrackPlots* trp_PV_mbTr_oVtx    = new TrackPlots("PV_mbTr_oVtx");
  TrackPlots* trp_allTr_mbTr_fVtx = new TrackPlots("allTr_mbTr_fVtx");
  TrackPlots* trp_PV_mbTr_fVtx    = new TrackPlots("PV_mbTr_fVtx");
  
  
  //------------- VERTEX COLLECTIONS -------------
  //offlinePV
  VertexPlots* vtxp_allVtx_offlinePV = new VertexPlots("allVtx_oVtx");
  VertexPlots* vtxp_PV_offlinePV = new VertexPlots("PV_oVtx");
  
  //ferencVtx wrt minbiasTracks
  VertexPlots* vtxp_allVtx_ferencVtx = new VertexPlots("allVtx_fVtx");
  VertexPlots* vtxp_PV_ferencVtx = new VertexPlots("PV_fVtx");
  
  
  //------------- EvtSel COLLECTION -------------
  EvtSelPlots* evtselp_PV_mbTr_fVtx = new EvtSelPlots("PV_mbTr_fVtx");
  EvtSelPlots* evtselp_PV_gTr_oVtx  = new EvtSelPlots("PV_gTr_oVtx");
  EvtSelPlots* evtselp_fPV_gTr_oVtx = new EvtSelPlots("fPV_gTr_oVtx");
  
  EvtSelPlots* evtselp_allTr_mbTr_fVtx = new EvtSelPlots("allTr_mbTr_fVtx");
  EvtSelPlots* evtselp_allTr_gTr_oVtx  = new EvtSelPlots("allTr_gTr_oVtx");
  
  EvtSelPlots* evtselp_allTr_mbTr_bs = new EvtSelPlots("allTr_mbTr_bs");
  EvtSelPlots* evtselp_allTr_gTr_bs  = new EvtSelPlots("allTr_gTr_bs");
  EvtSelPlots* evtselp_PV_mbTr_bs = new EvtSelPlots("PV_mbTr_bs");
  EvtSelPlots* evtselp_PV_gTr_bs  = new EvtSelPlots("PV_gTr_bs");
  
  //------------ MultiPlots ------------------
  MultiPlots* mp_PV_mbTr_fVtx    = new MultiPlots("L1_hf_VtxSel_PV_mbTr_fVtx");
  MultiPlots* mp_PV_gTr_oVtx     = new MultiPlots("L1_hf_VtxSel_PV_gTr_oVtx");
  
  MultiPlots* mp_gen_INC = new MultiPlots("gen_INC");
  MultiPlots* mp_gen_SD  = new MultiPlots("gen_SD");
  MultiPlots* mp_gen_DD  = new MultiPlots("gen_DD");
  MultiPlots* mp_gen_ND  = new MultiPlots("gen_ND");
  MultiPlots* mp_gen_NSD = new MultiPlots("gen_NSD");
  
  
  //------------ MatrixPlots ------------------
  MatrixPlots* mtxp_L1_hf_VtxSel_PV_mbTr_fVtx = new MatrixPlots("L1_hf_VtxSel_PV_mbTr_fVtx");
  MatrixPlots* mtxp_L1_hf_VtxSel_PV_gTr_oVtx = new MatrixPlots("L1_hf_VtxSel_PV_gTr_oVtx");
    
  
  // Open Tree file 
  //TFile* file=new TFile("/user/xjanssen/cms/MinBias/CMSSW_3_3_6_patch3/src/UAmulti/ChPartTree/test/ChPartTree_2.36_simrecfile103.root","OPEN");
  //TTree* tree =(TTree*)file->Get("evt");
  
  TChain* tree = new TChain("evt","");

if(!isMC)tree->Add("/user/xjanssen/data/CMSSW_3_3_6_patch3/ChPartTree_v004b_d900/__MinimumBias__BeamCommissioning09-Dec19thReReco_336p3_v2__RECO/ChPartTree_v004b_d900__CMSSW_3_3_6_patch3__MinimumBias__BeamCommissioning09-Dec19thReReco_336p3_v2__RECO_*.root/evt");
//if(isMC)tree->Add("/user/xjanssen/data/CMSSW_3_3_6_patch3/ChPartTree_v003_mc900/__MinBias__Summer09-STARTUP3X_V8K_900GeV-v1__GEN-SIM-RECO/ChPartTree_v003_mc900__CMSSW_3_3_6_patch3__MinBias__Summer09-STARTUP3X_V8K_900GeV-v1__GEN-SIM-RECO_1.root/evt");
if(isMC)tree->Add("/user/xjanssen/data/CMSSW_3_3_6_patch3/ChPartTree_v004_mc900/__MinBias__Summer09-STARTUP3X_V8K_900GeV-v1__GEN-SIM-RECO/ChPartTree_v004_mc900__CMSSW_3_3_6_patch3__MinBias__Summer09-STARTUP3X_V8K_900GeV-v1__GEN-SIM-RECO_9?.root/evt");

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
  MyL1Trig*     L1Trig    = NULL;
  MyMITEvtSel*  MITEvtSel = NULL;
  MyBeamSpot*   bs        = NULL;
  tree->SetBranchAddress("generalTracks",&generalTracks); 
  tree->SetBranchAddress("minbiasTracks",&minbiasTracks);
  tree->SetBranchAddress("primaryVertex",&offlinePV);
  tree->SetBranchAddress("ferencVtxFerTrk",&ferencVtx);
  tree->SetBranchAddress("L1Trig",&L1Trig);
  tree->SetBranchAddress("MITEvtSel",&MITEvtSel);
  tree->SetBranchAddress("beamSpot",&bs);
  
  int nev = int(tree->GetEntries());
  std::cout <<"number of entries is : "<< nev << std::endl;

  // Event TYPE counting --> Weights
  
  for(int i = 0; i < nev; i++){
    
    //if( (i%10000.) == 0) cout <<int(double(nev-i)/double(nev))<<" % left"<<endl;
    
    tree->GetEntry(i);
    
    //Selection of good BX for data && MC
    if(!isMC && !goodBX(evtId->Run,evtId->Bunch)) continue;
    
    
    
    //---------------- GEN -------------------
    int nchgen = 0;
    if(isMC){
      for(vector<MyGenPart>::iterator p=genPart->begin() ; p!=genPart->end() ; p++ ){
        if (  fabs(p->Part.charge) >0
          &&  fabs(p->Part.v.Eta())  < eta_cut
          &&  p->Part.v.Pt()> pt_cut
          &&  p->status==1 ){
           ++nchgen;
	   pt_gen->Fill(p->Part.v.Pt());
	   eta_gen->Fill(p->Part.v.Eta());
	   
	                                      mp_gen_INC->fill(p->Part);
	   if(isSD(genKin))                   mp_gen_SD->fill(p->Part);
	   if(isDD(genKin))                   mp_gen_DD->fill(p->Part);
	   if(!isSD(genKin) && !isDD(genKin)) mp_gen_ND->fill(p->Part);
	   if(!isSD(genKin))                  mp_gen_NSD->fill(p->Part);
	   
	   if(isEvtGood(*L1Trig , *MITEvtSel)){
	     mtxp_L1_hf_VtxSel_PV_mbTr_fVtx->fillGen(p->Part);
	     mtxp_L1_hf_VtxSel_PV_gTr_oVtx->fillGen(p->Part);
	   }
	}
      }
      
      mp_gen_INC->nextEvent();
      mp_gen_SD->nextEvent();
      mp_gen_DD->nextEvent();
      mp_gen_ND->nextEvent();
      mp_gen_NSD->nextEvent();
      
    }
    if(nchgen!=0) nch_gen->Fill(nchgen);
    
    

    //---------------- RECO -------------------
    if(debug) cout<<"Starting the vertex collections ..."<<endl;
    int vtxId_offlinePV = getBestVertex(offlinePV);
    int vtxId_ferencVtx = getBestVertex(ferencVtx);
    vtxp_allVtx_offlinePV->fill(*offlinePV);
    vtxp_allVtx_ferencVtx->fill(*ferencVtx);
    //if(vtxId<0) continue;
    
    for(vector<MyVertex>::iterator it_vtx = offlinePV->begin();it_vtx != offlinePV->end();++it_vtx)
      if(vtxId_offlinePV==it_vtx->id)
        vtxp_PV_offlinePV->fill(*it_vtx);
    
    for(vector<MyVertex>::iterator it_vtx = ferencVtx->begin();it_vtx != ferencVtx->end();++it_vtx)
      if(vtxId_ferencVtx==it_vtx->id)
        vtxp_PV_ferencVtx->fill(*it_vtx);
	
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
    
    evtselp_PV_mbTr_fVtx->setSelBits(passL1(*L1Trig),passHF(*MITEvtSel),passVtxQual(*MITEvtSel),passBit40(*L1Trig));
    evtselp_PV_gTr_oVtx->setSelBits(passL1(*L1Trig),passHF(*MITEvtSel),passVtxQual(*MITEvtSel),passBit40(*L1Trig));
    evtselp_fPV_gTr_oVtx->setSelBits(passL1(*L1Trig),passHF(*MITEvtSel),passVtxQual(*MITEvtSel),passBit40(*L1Trig));
    evtselp_allTr_mbTr_fVtx->setSelBits(passL1(*L1Trig),passHF(*MITEvtSel),passVtxQual(*MITEvtSel),passBit40(*L1Trig));
    evtselp_allTr_gTr_oVtx->setSelBits(passL1(*L1Trig),passHF(*MITEvtSel),passVtxQual(*MITEvtSel),passBit40(*L1Trig));
    
    evtselp_PV_mbTr_bs->setSelBits(passL1(*L1Trig),passHF(*MITEvtSel),passVtxQual(*MITEvtSel),passBit40(*L1Trig));
    evtselp_PV_gTr_bs->setSelBits(passL1(*L1Trig),passHF(*MITEvtSel),passVtxQual(*MITEvtSel),passBit40(*L1Trig));
    evtselp_allTr_mbTr_bs->setSelBits(passL1(*L1Trig),passHF(*MITEvtSel),passVtxQual(*MITEvtSel),passBit40(*L1Trig));
    evtselp_allTr_gTr_bs->setSelBits(passL1(*L1Trig),passHF(*MITEvtSel),passVtxQual(*MITEvtSel),passBit40(*L1Trig));
    
    if(debug) cout<<"Starting to fill all classes ..."<<endl;
    evtselp_allTr_mbTr_fVtx->fill(*minbiasTracks,*ferencVtx,bs,vtxId_ferencVtx,MITEvtSel->eClusVtxQual,MITEvtSel->ePxHits);
    evtselp_allTr_gTr_oVtx->fill(*generalTracks,*offlinePV,bs,vtxId_offlinePV,MITEvtSel->eClusVtxQual,MITEvtSel->ePxHits);
    
    evtselp_allTr_mbTr_bs->fill(*minbiasTracks,*ferencVtx,bs,0,MITEvtSel->eClusVtxQual,MITEvtSel->ePxHits);
    evtselp_allTr_gTr_bs->fill(*generalTracks,*offlinePV,bs,0,MITEvtSel->eClusVtxQual,MITEvtSel->ePxHits);
    
    if(debug) cout<<"Starting to fill good oVtx ..."<<endl;
    vector<MyTracks> trcoll = getPrimaryTracks(*generalTracks,0);
    evtselp_PV_gTr_bs->fill(trcoll,*offlinePV,bs,0,MITEvtSel->eClusVtxQual,MITEvtSel->ePxHits);
    
    trcoll = getPrimaryTracks(*minbiasTracks,ferencVtx,0,bs);
    evtselp_PV_mbTr_bs->fill(trcoll,*ferencVtx,bs,0,MITEvtSel->eClusVtxQual,MITEvtSel->ePxHits);
    
    if(debug) cout<<"Starting to fill good oVtx ..."<<endl;
    
    //if(vtxId_offlinePV>=0){
      trp_allTr_gTr_oVtx->fill(*generalTracks,*offlinePV,vtxId_offlinePV,bs);
      trp_allTr_mbTr_oVtx->fill(*minbiasTracks,*offlinePV,vtxId_offlinePV,bs);
      
      trcoll = getPrimaryTracks(*generalTracks,offlinePV);
      trp_PV_gTr_oVtx->fill( trcoll , *offlinePV, vtxId_offlinePV,bs);
      evtselp_PV_gTr_oVtx->fill(trcoll,*offlinePV,bs,vtxId_offlinePV,MITEvtSel->eClusVtxQual,MITEvtSel->ePxHits);
      
      trcoll = getPrimaryTracks(*generalTracks, offlinePV, bs);
      evtselp_fPV_gTr_oVtx->fill(trcoll,*offlinePV,bs,vtxId_offlinePV,MITEvtSel->eClusVtxQual,MITEvtSel->ePxHits);
      
      trcoll = getPrimaryTracks(*minbiasTracks, offlinePV);
      trp_PV_mbTr_oVtx->fill(trcoll , *offlinePV, vtxId_offlinePV,bs);
    //}
    
    if(debug) cout<<"Starting to fill good fVtx ..."<<endl;
    
    //if(vtxId_ferencVtx>=0){
      trp_allTr_gTr_fVtx->fill(*generalTracks, *ferencVtx, vtxId_ferencVtx,bs);
      trp_allTr_mbTr_fVtx->fill(*minbiasTracks, *ferencVtx,vtxId_ferencVtx,bs);
    
      trcoll = getPrimaryTracks(*generalTracks,ferencVtx,bs);
      trp_PV_gTr_fVtx->fill( trcoll , *ferencVtx, vtxId_ferencVtx , bs);
      
      trcoll = getPrimaryTracks(*minbiasTracks,ferencVtx,bs);
      trp_PV_mbTr_fVtx->fill( trcoll , *ferencVtx, vtxId_ferencVtx , bs);
      evtselp_PV_mbTr_fVtx->fill(trcoll,*ferencVtx,bs,vtxId_ferencVtx,MITEvtSel->eClusVtxQual,MITEvtSel->ePxHits);
    //}
    
    //------------- FILLING MULTI && MATRIX----------------------- 
    if( isEvtGood( *L1Trig , *MITEvtSel) ){
      trcoll = getPrimaryTracks(*minbiasTracks,ferencVtx,bs);
      for(vector<MyTracks>::iterator it_tr = trcoll.begin() ; it_tr != trcoll.end() ; ++it_tr){
        mp_PV_mbTr_fVtx->fill(it_tr->Part);
	mtxp_L1_hf_VtxSel_PV_mbTr_fVtx->fillReco(it_tr->Part);
      }
      mp_PV_mbTr_fVtx->nextEvent();
      mtxp_L1_hf_VtxSel_PV_mbTr_fVtx->nextEvent();
      
      trcoll = getPrimaryTracks(*generalTracks,offlinePV);
      for(vector<MyTracks>::iterator it_tr = trcoll.begin() ; it_tr != trcoll.end() ; ++it_tr){
        mp_PV_gTr_oVtx->fill(it_tr->Part);
	mtxp_L1_hf_VtxSel_PV_gTr_oVtx->fillReco(it_tr->Part);
      }
      mp_PV_gTr_oVtx->nextEvent();
      mtxp_L1_hf_VtxSel_PV_gTr_oVtx->nextEvent();
      
    }
  
  
  }//end of loop over entries from tree

  //file->Close();


  
  //output file
  TFile* file2=new TFile("collectionPlotter_"+filename+".root","RECREATE");
  file2->cd();
  
  if(isMC){
    nch_gen->Write();
    pt_gen->Write();
    eta_gen->Write();
  }
  
  trp_allTr_gTr_oVtx->write();
  trp_PV_gTr_oVtx->write();
  trp_allTr_gTr_fVtx->write();
  trp_PV_gTr_fVtx->write();
  
  //minbiastracks
  trp_allTr_mbTr_oVtx->write();
  trp_PV_mbTr_oVtx->write();
  trp_allTr_mbTr_fVtx->write();
  trp_PV_mbTr_fVtx->write();
  
  //offlinePV
  vtxp_allVtx_offlinePV->write();
  vtxp_PV_offlinePV->write();
  
  //ferencVtx wrt minbiasTracks
  vtxp_allVtx_ferencVtx->write();
  vtxp_PV_ferencVtx->write();
  
  evtselp_PV_mbTr_fVtx->write();
  evtselp_PV_gTr_oVtx->write();
  evtselp_fPV_gTr_oVtx->write();
  
  evtselp_allTr_mbTr_fVtx->write();
  evtselp_allTr_gTr_oVtx->write();
  
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
    evtselp_PV_mbTr_fVtx->setDenom(nch_gen,pt_gen,eta_gen);
    evtselp_PV_gTr_oVtx->setDenom(nch_gen,pt_gen,eta_gen);
    evtselp_fPV_gTr_oVtx->setDenom(nch_gen,pt_gen,eta_gen);
    evtselp_allTr_mbTr_fVtx->setDenom(nch_gen,pt_gen,eta_gen);
    evtselp_allTr_gTr_oVtx->setDenom(nch_gen,pt_gen,eta_gen);
    evtselp_PV_mbTr_bs->setDenom(nch_gen,pt_gen,eta_gen);
    evtselp_PV_gTr_bs->setDenom(nch_gen,pt_gen,eta_gen);
    evtselp_allTr_mbTr_bs->setDenom(nch_gen,pt_gen,eta_gen);
    evtselp_allTr_gTr_bs->setDenom(nch_gen,pt_gen,eta_gen);
  
    evtselp_PV_mbTr_fVtx->makeEffPlots("GEN");
    evtselp_PV_gTr_oVtx->makeEffPlots("GEN");
    evtselp_fPV_gTr_oVtx->makeEffPlots("GEN");
    evtselp_allTr_mbTr_fVtx->makeEffPlots("GEN");
    evtselp_allTr_gTr_oVtx->makeEffPlots("GEN");
    evtselp_PV_mbTr_bs->makeEffPlots("GEN");
    evtselp_PV_gTr_bs->makeEffPlots("GEN");
    evtselp_allTr_mbTr_bs->makeEffPlots("GEN");
    evtselp_allTr_gTr_bs->makeEffPlots("GEN");
  }
  
  //-------- MULTI --------
  mp_PV_mbTr_fVtx->write();
  mp_PV_gTr_oVtx->write();
  
  if(isMC){
    mp_gen_INC->write();
    mp_gen_SD->write();
    mp_gen_DD->write();
    mp_gen_ND->write();
    mp_gen_NSD->write();
  }
  
  //------ MATRIX -------
  if(isMC){
    mtxp_L1_hf_VtxSel_PV_mbTr_fVtx->write();
    mtxp_L1_hf_VtxSel_PV_gTr_oVtx->write();
  }
  
  
  
  
  
  
  
  
  
  //file2->Close();
}
