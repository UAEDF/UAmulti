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
#include "../plugins/MultiPlots.h"
#include "../plugins/GenMultiPlots.h"
#include "../plugins/MatrixPlots.h"

bool debug = false;

TString filename = "MC_test_test_900GeV";
bool isMC = true;


#include "fileManager.C"
#include "cuts.C"
#include "evtSel.C"

TString st(string input , int cut){
  stringstream out("");
  out<<cut;
  return input+"_cut"+out.str();
}

void SimpleAna(int = 10 , double = 0.9 , TString = "test" , int = 20000 , int = 0 , int = 0 );

void SimpleAna(int type , double E , TString filename , int nevt_max , int iTracking , int irun)
{
  if(type==0) isMC = false;
  
  int nbinmulti = 70;
  
  #include "acceptanceMap.C"
  
  //----------------------  RECO  ----------------------
  vector<GenMultiPlots*> gmp_evtSel_reco(accMap.size(),0);
  vector<MultiPlots*> mp_evtSel_INC_reco(accMap.size(),0);
  
  vector<GenMultiPlots*> gmp_evtSel(accMap.size(),0);
  vector<MultiPlots*> mp_etaCut_noSel_NSD_gen(accMap.size(),0);
  vector<GenMultiPlots*> gmp_etaCut(accMap.size(),0);
  
  vector<MultiPlots*> mp_evtSel_PV(accMap.size(),0);
  vector<TrackPlots*> trp(accMap.size(),0);
  VertexPlots* vtxp = new VertexPlots("");
  vector<MatrixPlots*> mtxp_evtSel(accMap.size(),0);
  
  vector<TH1F*> L1_before(accMap.size(),0);
  vector<TH1F*> L1_after(accMap.size(),0);
  vector<TH1F*> hf_before(accMap.size(),0);
  vector<TH1F*> hf_after(accMap.size(),0);
  vector<TH1F*> vtxqual_before(accMap.size(),0);
  vector<TH1F*> vtxqual_after(accMap.size(),0);
  vector<TH1F*> vtx_before(accMap.size(),0);
  vector<TH1F*> vtx_after(accMap.size(),0);
  vector<TH1F*> evtSel_before(accMap.size(),0);
  vector<TH1F*> evtSel_after(accMap.size(),0);
  
  for(int acc = 0 ; acc < (signed) accMap.size() ; ++acc){
    //----------------------  RECO  ----------------------
    gmp_evtSel_reco.at(acc)  = new GenMultiPlots(st("evtSel_reco",acc));
    mp_evtSel_INC_reco.at(acc)  = new MultiPlots(st("evtSel_INC_reco",acc));
  
  
    //----------------------   GEN  ----------------------
    gmp_evtSel.at(acc)           = new GenMultiPlots(st("evtSel",acc));
    mp_etaCut_noSel_NSD_gen.at(acc) = new MultiPlots(st("etaCut_noSel_NSD_gen",acc));
    gmp_etaCut.at(acc)           = new GenMultiPlots(st("etaCut",acc));
  
    //------------ MultiPlots ------------------
    mp_evtSel_PV.at(acc)     = new MultiPlots(st("evtSel_PV",acc));
    trp.at(acc) = new TrackPlots(st("",acc));  
  
  
    //------------ MatrixPlots ------------------
    mtxp_evtSel.at(acc) = new MatrixPlots(st("evtSel",acc));
  
    //-----
    L1_before.at(acc) = new TH1F(st("L1_before",acc),st("L1_before",acc),nbinmulti+1,-0.5,nbinmulti+0.5);
    L1_after.at(acc) = new TH1F(st("L1_after",acc),st("L1_after",acc),nbinmulti+1,-0.5,nbinmulti+0.5);
    L1_before.at(acc)->Sumw2();
    L1_after.at(acc)->Sumw2();
    hf_before.at(acc) = new TH1F(st("hf_before",acc),st("hf_before",acc),nbinmulti+1,-0.5,nbinmulti+0.5);
    hf_after.at(acc) = new TH1F(st("hf_after",acc),st("hf_after",acc),nbinmulti+1,-0.5,nbinmulti+0.5);
    hf_before.at(acc)->Sumw2();
    hf_after.at(acc)->Sumw2();
    vtxqual_before.at(acc) = new TH1F(st("vtxqual_before",acc),st("vtxqual_before",acc),nbinmulti+1,-0.5,nbinmulti+0.5);
    vtxqual_after.at(acc) = new TH1F(st("vtxqual_after",acc),st("vtxqual_after",acc),nbinmulti+1,-0.5,nbinmulti+0.5);
    vtxqual_before.at(acc)->Sumw2();
    vtxqual_after.at(acc)->Sumw2();
    vtx_before.at(acc) = new TH1F(st("vtx_before",acc),st("vtx_before",acc),nbinmulti+1,-0.5,nbinmulti+0.5);
    vtx_after.at(acc) = new TH1F(st("vtx_after",acc),st("vtx_after",acc),nbinmulti+1,-0.5,nbinmulti+0.5);
    vtx_before.at(acc)->Sumw2();
    vtx_after.at(acc)->Sumw2();
    evtSel_before.at(acc) = new TH1F(st("evtSel_before",acc),st("evtSel_before",acc),nbinmulti+1,-0.5,nbinmulti+0.5);
    evtSel_after.at(acc) = new TH1F(st("evtSel_after",acc),st("evtSel_after",acc),nbinmulti+1,-0.5,nbinmulti+0.5);
    evtSel_before.at(acc)->Sumw2();
    evtSel_after.at(acc)->Sumw2();
  }
  
  TChain* tree = new TChain("evt","");
  tree->Add(fileManager(0,type,E));
  
  
  
  MyEvtId* evtId      = NULL ;
  tree->SetBranchAddress("EvtId",&evtId);
  
  vector<MyGenPart>* genPart = NULL;
  if(isMC) tree->SetBranchAddress("GenPart",&genPart);
  
  MyGenKin* genKin = NULL;
  if(isMC) tree->SetBranchAddress("GenKin",&genKin);
  
  /*vector<MyTracks>*  generalTracks  = NULL;
  vector<MyTracks>*  minbiasTracks  = NULL;
  vector<MyVertex>*  offlinePV  = NULL;
  vector<MyVertex>*  ferencVtx  = NULL;
  vector<MyVertex>*  pixel3Vertex  = NULL;*/
  vector<MyTracks>*  tracks  = NULL;
  vector<MyVertex>*  vertex  = NULL;
  vector<MyVertex>*  vertexToCut  = NULL;
  MyL1Trig*     L1Trig    = NULL;
  MyMITEvtSel*  MITEvtSel = NULL;
  MyBeamSpot*   bs        = NULL;
  /*tree->SetBranchAddress("generalTracks",&generalTracks); 
  tree->SetBranchAddress("minbiasTracks",&minbiasTracks);
  tree->SetBranchAddress("primaryVertex",&offlinePV);
  //tree->SetBranchAddress("ferencVtxFerTrk",&ferencVtx);*/
  if(iTracking==0){
    tree->SetBranchAddress("generalTracks",&tracks); 
    tree->SetBranchAddress("primaryVertex",&vertex);
  }
  else if(iTracking==1){
    tree->SetBranchAddress("minbiasTracks",&tracks); 
    tree->SetBranchAddress("ferencVtxFerTrk",&vertex);
  }
  
  tree->SetBranchAddress("pixel3Vertex",&vertexToCut);
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
    
    //if(i==0) vertexToCut = pixel3Vertex;
    
    //Selection of good BX for data && MC
    if(!isMC && !goodBX(evtId->Run,evtId->Bunch)) continue;
    
    if(!isMC)
      if(irun>0)
        if(irun!=(signed)evtId->Run)
	  continue;
    
        
    //----------------------------------------------------------------------
    //----------------------- USED TO SUBSTRACT SD -------------------------
    //----------------------------------------------------------------------
    
    
    if(isEvtGood(*L1Trig , *MITEvtSel , vertex)){
      for(int acc = 0 ; acc < (signed)accMap.size() ; ++acc){
	if(isMC){
          for(vector<MyGenPart>::iterator p=genPart->begin() ; p!=genPart->end() ; p++ )
            if ( isGenPartGood(*p) && isInAcceptance(p->Part,accMap[acc].at(2),accMap[acc].at(3)) )//--->RECO acc cuts !!
              gmp_evtSel.at(acc)->fill(*genKin , p->Part);
          gmp_evtSel.at(acc)->nextEvent(*genKin);
        }
      
        //Making a GenMulti for RECO
        vector<MyTracks> trcoll;
	if(iTracking==0) trcoll = getPrimaryTracks(*tracks,vertex,accMap[acc].at(2),accMap[acc].at(3));
	if(iTracking==1) trcoll = getPrimaryTracks(*tracks,vertex,bs,accMap[acc].at(2),accMap[acc].at(3));
        for(vector<MyTracks>::iterator it_tr = trcoll.begin() ; it_tr != trcoll.end() ; ++it_tr){
          if(isMC) gmp_evtSel_reco.at(acc)->fill(*genKin , it_tr->Part);
	  mp_evtSel_INC_reco.at(acc)->fill(it_tr->Part);
        }
        if(isMC) gmp_evtSel_reco.at(acc)->nextEvent(*genKin);
        mp_evtSel_INC_reco.at(acc)->nextEvent();
      }
    }
    
    //Skipping the SD events starting from here
    if(isMC)
      if(isSD(genKin) ) continue;
        
    //----------------------------------------------------------------------
    //----------------------- DOING THE SELECTIONS -------------------------
    //-----------------------          GEN         -------------------------
    //----------------------------------------------------------------------
    
    for(int acc = 0 ; acc < (signed)accMap.size() ; ++acc){
      if(isMC){
        for(vector<MyGenPart>::iterator p=genPart->begin() ; p!=genPart->end() ; p++ )
          if ( isGenPartGood(*p) && isInAcceptance(p->Part,accMap[acc].at(0),accMap[acc].at(1)) )
            mp_etaCut_noSel_NSD_gen.at(acc)->fill(p->Part);
        mp_etaCut_noSel_NSD_gen.at(acc)->nextEvent();
      }
    
      if(isMC) evtSel_before.at(acc)->Fill(getnPrimaryGenPart(genPart,accMap[acc].at(0),accMap[acc].at(1)));
    
      //L1 CUT
      if(isMC) L1_before.at(acc)->Fill(getnPrimaryGenPart(genPart,accMap[acc].at(0),accMap[acc].at(1)));
      if(passL1(*L1Trig))
        if(isMC) L1_after.at(acc)->Fill(getnPrimaryGenPart(genPart,accMap[acc].at(0),accMap[acc].at(1)));
    
      //HF CUT
      if(isMC) hf_before.at(acc)->Fill(getnPrimaryGenPart(genPart,accMap[acc].at(0),accMap[acc].at(1)));
      if(passHF(*MITEvtSel))
        if(isMC) hf_after.at(acc)->Fill(getnPrimaryGenPart(genPart,accMap[acc].at(0),accMap[acc].at(1)));
    
      //vtxqual CUT
      if(isMC) vtxqual_before.at(acc)->Fill(getnPrimaryGenPart(genPart,accMap[acc].at(0),accMap[acc].at(1)));
      if(passVtxQual(*MITEvtSel))
        if(isMC) vtxqual_after.at(acc)->Fill(getnPrimaryGenPart(genPart,accMap[acc].at(0),accMap[acc].at(1)));
    
      //Taking events with only 1 good vertex
      if(isMC) vtx_before.at(acc)->Fill(getnPrimaryGenPart(genPart,accMap[acc].at(0),accMap[acc].at(1)));
      if(getBestVertex(vertexToCut)>-1)
        if(isMC) vtx_after.at(acc)->Fill(getnPrimaryGenPart(genPart,accMap[acc].at(0),accMap[acc].at(1)));
    
      //if(!isEvtGood(*L1Trig , *MITEvtSel) || getBestVertex(vertexToCut)==-1) continue;
      if(isEvtGood(*L1Trig , *MITEvtSel , vertex))
        if(isMC) evtSel_after.at(acc)->Fill(getnPrimaryGenPart(genPart,accMap[acc].at(0),accMap[acc].at(1)));
    
    }
    
    if(!isEvtGood(*L1Trig , *MITEvtSel , vertex)) continue;
    
    //----------------------------------------------------------------------
    //-----------------------         GEN          -------------------------
    //----------------------------------------------------------------------
    
    
    if(isMC){
      for(int acc = 0 ; acc < (signed)accMap.size() ; ++acc){
        for(vector<MyGenPart>::iterator p=genPart->begin() ; p!=genPart->end() ; p++ ){
          if ( isGenPartGood(*p) && isInAcceptance(p->Part,accMap[acc].at(0),accMap[acc].at(1))){
	    gmp_etaCut.at(acc)->fill(*genKin,p->Part);
	    mtxp_evtSel.at(acc)->fillGen(p->Part);
	  }
        }
        gmp_etaCut.at(acc)->nextEvent(*genKin);
      }
    }
    
    //----------------------------------------------------------------------
    //-----------------------         RECO         -------------------------
    //----------------------------------------------------------------------
    if(debug) cout<<"Starting the vertex collections ..."<<endl;
    int vtxId = getBestVertex(vertex);
    
    //------------- FILLING MULTI && MATRIX----------------------- 
    if(debug) cout<<"Starting to fill RECO ..."<<endl;
    
    vector<MyVertex>::iterator goodVtx = vertex->end();
    for(vector<MyVertex>::iterator it_vtx = vertex->begin();it_vtx != vertex->end();++it_vtx)
      if(vtxId==it_vtx->id)
        goodVtx = it_vtx;
    vtxp->fill(*goodVtx);
      
    //-----------------------------
    //FILLING THE MATRIX && MULTI
    
    for(int acc = 0 ; acc < (signed)accMap.size() ; ++acc){
      vector<MyTracks> trcoll;
      if(iTracking==0) trcoll = getPrimaryTracks(*tracks,vertex,accMap[acc].at(2),accMap[acc].at(3));
      if(iTracking==1) trcoll = getPrimaryTracks(*tracks,vertex,bs,accMap[acc].at(2),accMap[acc].at(3));
      for(vector<MyTracks>::iterator it_tr = trcoll.begin() ; it_tr != trcoll.end() ; ++it_tr){
	if(isMC)
	  mtxp_evtSel.at(acc)->fillReco(it_tr->Part);
	mp_evtSel_PV.at(acc)->fill(it_tr->Part);
	trp.at(acc)->fill(trcoll,*vertex,vtxId,bs);
      }
      if(isMC) mtxp_evtSel.at(acc)->nextEvent();
      mp_evtSel_PV.at(acc)->nextEvent();
    
    }
  
  }//end of loop over entries from tree

  //tree->Close();


  if(debug) cout<<"Starting to write to file ..."<<endl;
  
  //output file
  TFile* file2=new TFile(fileManager(2,type,E,iTracking,0,0,filename),"RECREATE");
  file2->cd();
  /*
  cout<<"mp : "<<mp_PV_gTr_oVtx->nbEvts<<endl;
  cout<<"mtxp : "<<mtxp_etaGenCut_L1_hf_VtxSel_PV_gTr_oVtx->nbEvts<<endl;
  cout<<"gmp NSD : "<<gmp_etaCut->mp_NSD->nbEvts<<endl<<endl;
  cout<<"gmp : "<<gmp_evtSel->mp_NSD->nbEvts<<endl;
  cout<<"gmp RECO : "<<gmp_evtSel_reco->mp_NSD->nbEvts<<endl;
  cout<<"mp INC RECO : "<<mp_evtSel_INC_reco->nbEvts<<endl;
  */
  
  for(int acc = 0 ; acc < (signed)accMap.size() ; ++acc){
    //TString dir = "";
    ostringstream dir("");
    dir << "ptGen" << accMap.at(acc).at(0) << "_etaGen" << accMap.at(acc).at(1) 
        << "_ptReco" << accMap.at(acc).at(2) << "_etaReco" << accMap.at(acc).at(3);
    if(accMap.at(acc).at(4)==-1)
      dir << "_chargemin";
    else if(accMap.at(acc).at(4)==1)
      dir << "_chargeplus";
     
    /*tmp<< accMap.[acc].at(1);
    dir+=tmp.str()+;tmp.str("");
    tmp<< accMap.[acc].at(2);
    dir+=tmp.str()+"_etaReco";tmp.str("");
    tmp<< accMap.[acc].at(3);*/
    gDirectory->mkdir(dir.str().c_str());
    gDirectory->cd(dir.str().c_str());

    bool scale = false;
    if(isMC){
      gmp_etaCut.at(acc)->write(scale);
      mtxp_evtSel.at(acc)->write(scale);
      mp_etaCut_noSel_NSD_gen.at(acc)->write(scale);
      gmp_evtSel.at(acc)->write(scale);
    
      gmp_evtSel_reco.at(acc)->write(scale);
    }
  
    mp_evtSel_INC_reco.at(acc)->write(scale);
    mp_evtSel_PV.at(acc)->write(scale);
    trp.at(acc)->write();
    vtxp->write();
  
    TH1F* eff_L1 = (TH1F*) L1_after.at(acc)->Clone(st("eff_L1",acc));
    eff_L1->Divide(L1_before.at(acc));
    L1_before.at(acc)->Write();
    L1_after.at(acc)->Write();
    eff_L1->Write();
  
    TH1F* eff_hf = (TH1F*) hf_after.at(acc)->Clone(st("eff_hf",acc));
    eff_hf->Divide(hf_before.at(acc));
    hf_before.at(acc)->Write();
    hf_after.at(acc)->Write();
    eff_hf->Write();
  
    TH1F* eff_vtxqual = (TH1F*) vtxqual_after.at(acc)->Clone(st("eff_vtxqual",acc));
    eff_vtxqual->Divide(vtxqual_before.at(acc));
    vtxqual_before.at(acc)->Write();
    vtxqual_after.at(acc)->Write();
    eff_vtxqual->Write();
  
    TH1F* eff_vtx = (TH1F*) vtx_after.at(acc)->Clone(st("eff_vtx",acc));
    eff_vtx->Divide(vtx_before.at(acc));
    vtx_before.at(acc)->Write();
    vtx_after.at(acc)->Write();
    eff_vtx->Write();
  
    TH1F* eff_evtSel = (TH1F*) evtSel_after.at(acc)->Clone(st("eff_evtSel",acc));
    eff_evtSel->Divide(evtSel_before.at(acc));
    evtSel_before.at(acc)->Write();
    evtSel_after.at(acc)->Write();
    eff_evtSel->Write();
    
    gDirectory->cd("../");

  
  }  
  
  file2->Close();
}
