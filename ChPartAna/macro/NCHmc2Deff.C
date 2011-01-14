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

#include "../../../UAHiggs/UAHiggsAna/src/MatchTools.C"

bool isMC = true;
bool debug = false;


#include "fileManager.C"
#include "cuts.C"
#include "evtSel.C"
#include "binningMap.C"

TString st(string input , int cut){
  stringstream out("");
  out<<cut;
  return input+"_cut"+out.str();
}


void NCHmc2Deff(int type = 31 , double E = 7. , int iTracking = 1 , int nevt_max = 900000){
  
  
  if(type==0) isMC = false;
  TString MCtype = "pythia";
  if(type == 20) MCtype = "phojet";
    
  vector< vector<double> > binning;
  binning = getBins(2,1,3);//nch,pt,eta
    
  TH2F* nTr_ptVSeta_NSD_evtSel_reco  = new TH2F("nTr_ptVSeta_NSD_evtSel_reco" , "nTr_ptVSeta_NSD_evtSel_reco ;#eta;pt" , binning.at(2).size()-1 , &(binning.at(2).at(0)) ,  binning.at(1).size()-1 , &(binning.at(1).at(0)) );   
  TH2F* nTr_ptVSeta_NSD_evtSel_reco_gt4hit  = new TH2F("nTr_ptVSeta_NSD_evtSel_reco_gt4hit" , "nTr_ptVSeta_NSD_evtSel_reco_gt4hit ;#eta;pt" , binning.at(2).size()-1 , &(binning.at(2).at(0)) ,  binning.at(1).size()-1 , &(binning.at(1).at(0)) );   
  TH2F* nTr_ptVSeta_NSD_evtSel_gen   = new TH2F("nTr_ptVSeta_NSD_evtSel_gen"  , "nTr_ptVSeta_NSD_evtSel_gen  ;#eta;pt" , binning.at(2).size()-1 , &(binning.at(2).at(0)) ,  binning.at(1).size()-1 , &(binning.at(1).at(0)) );   
  TH2F* nTr_ptVSeta_NSD_noEvtSel_gen = new TH2F("nTr_ptVSeta_NSD_noEvtSel_gen", "nTr_ptVSeta_NSD_noEvtSel_gen;#eta;pt" , binning.at(2).size()-1 , &(binning.at(2).at(0)) ,  binning.at(1).size()-1 , &(binning.at(1).at(0)) );   
  nTr_ptVSeta_NSD_evtSel_reco ->Sumw2();
  nTr_ptVSeta_NSD_evtSel_reco_gt4hit->Sumw2();
  nTr_ptVSeta_NSD_evtSel_gen  ->Sumw2();
  nTr_ptVSeta_NSD_noEvtSel_gen->Sumw2();
  
  TH2F* nTr_ptVSeta_NSD_evtSel_matched_reco  = new TH2F("nTr_ptVSeta_NSD_evtSel_matched_reco" , "nTr_ptVSeta_NSD_evtSel_matched_reco ;#eta;pt" , binning.at(2).size()-1 , &(binning.at(2).at(0)) ,  binning.at(1).size()-1 , &(binning.at(1).at(0)) );   
  TH2F* nTr_ptVSeta_NSD_evtSel_fakes_reco   = new TH2F("nTr_ptVSeta_NSD_evtSel_fakes_reco"  , "nTr_ptVSeta_NSD_evtSel_fakes_reco  ;#eta;pt" , binning.at(2).size()-1 , &(binning.at(2).at(0)) ,  binning.at(1).size()-1 , &(binning.at(1).at(0)) );   
  TH2F* nTr_ptVSeta_NSD_evtSel_matched_gen   = new TH2F("nTr_ptVSeta_NSD_evtSel_matched_gen"  , "nTr_ptVSeta_NSD_evtSel_matched_gen  ;#eta;pt" , binning.at(2).size()-1 , &(binning.at(2).at(0)) ,  binning.at(1).size()-1 , &(binning.at(1).at(0)) );   
  nTr_ptVSeta_NSD_evtSel_matched_reco->Sumw2();
  nTr_ptVSeta_NSD_evtSel_fakes_reco ->Sumw2();
  nTr_ptVSeta_NSD_evtSel_matched_gen ->Sumw2();
  
  cout << "Finished initialization of TH2s " << endl;
  
  //adding files to the tree chain
  TChain* tree = new TChain("evt","");
  tree->Add(fileManager(0,type,E));
  
  
  //adding branches to the tree
  
  MyEvtId* evtId      = NULL ;
  tree->SetBranchAddress("EvtId",&evtId);
  
  vector<MyGenPart>* genPart = NULL;
  if(isMC) tree->SetBranchAddress("GenPart",&genPart);
  
  MyGenKin* genKin = NULL;
  if(isMC) tree->SetBranchAddress("GenKin",&genKin);
  
  vector<MyTracks>*  tracks  = NULL;
  vector<MyVertex>*  vertex  = NULL;
  vector<MyVertex>*  vertexToCut  = NULL;
  MyL1Trig*     L1Trig    = NULL;
  MyMITEvtSel*  MITEvtSel = NULL;
  MyBeamSpot*   bs        = NULL;
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
  
  //Getting number of events
  int nev = int(tree->GetEntries());
  std::cout <<"number of entries is : "<< nev << std::endl;
  cout<<"Running on: "<<min(nev,nevt_max)<<" events"<<endl;
  // Event TYPE counting --> Weights
  
  //starting loop over events
  for(int i = 0; i < nev; i++){
     
    if( ((i+1) % 10000) == 0) cout <<int(double(i+1)/double(min(nev,nevt_max))*100.)<<" % done"<<endl;
    
    if(i>min(nev,nevt_max)) break;
    
    tree->GetEntry(i);
    
    if(i==0) vertexToCut = vertex;
    
    //Selection of good BX for data && MC
    if(!isMC && !goodBX(evtId->Run,evtId->Bunch)) continue;
    
    //Skipping the SD events starting from here
    if(isMC)
      if(isSD(genKin , MCtype) ) continue;
    
    
    if(isMC){
      for(vector<MyGenPart>::iterator p=genPart->begin() ; p!=genPart->end() ; p++ )
        if ( isGenPartGood(*p) && isInAcceptance(p->Part,0.05,2.4,0) )
	  nTr_ptVSeta_NSD_noEvtSel_gen->Fill(p->Part.v.Eta() , p->Part.v.Pt());
    }
    
    
    //skipping events that don't pass our event selection
    if(!isEvtGood(E,*L1Trig , *MITEvtSel , vertexToCut)) continue;
    
    if(isMC){
      for(vector<MyGenPart>::iterator p=genPart->begin() ; p!=genPart->end() ; p++ )
        if ( isGenPartGood(*p) && isInAcceptance(p->Part,0.05,2.4,0) )
	  nTr_ptVSeta_NSD_evtSel_gen->Fill(p->Part.v.Eta() , p->Part.v.Pt()); 
    }
    
    
    int vtxId = getBestVertex(vertex);
    vector<MyVertex>::iterator goodVtx = vertex->end();
    for(vector<MyVertex>::iterator it_vtx = vertex->begin();it_vtx != vertex->end();++it_vtx)
      if(vtxId==it_vtx->id)
        goodVtx = it_vtx;
    
    vector<MyTracks> trcoll;
    if(iTracking==0) trcoll = getPrimaryTracks(*tracks,vertex,0.05,2.4,0);
    if(iTracking==1) trcoll = getPrimaryTracks(*tracks,vertex,bs,0.05,2.4,0);
    for(vector<MyTracks>::iterator it_tr = trcoll.begin() ; it_tr != trcoll.end() ; ++it_tr){
      nTr_ptVSeta_NSD_evtSel_reco->Fill(it_tr->Part.v.Eta() , it_tr->Part.v.Pt());
      if(it_tr->nhit>4)
        nTr_ptVSeta_NSD_evtSel_reco_gt4hit->Fill(it_tr->Part.v.Eta() , it_tr->Part.v.Pt());
    }
    
    vector<MyGenPart>* gpcoll = new vector<MyGenPart>();
    if(isMC){
      for(vector<MyGenPart>::iterator p=genPart->begin() ; p!=genPart->end() ; p++ )
        if ( isGenPartGood(*p) && isInAcceptance(p->Part,0.05,2.4,0) )
	  gpcoll->push_back(*p);
	  
      //cout << "gp size : " << gpcoll->size() << endl;
    
      vector< pair<MyGenPart* , MyTracks*> > match;
      vector<MyTracks*>                      fakes;
      vector<MyGenPart*>                     gp_notMatched;

      GetMatch2(gpcoll , &trcoll , &match , &fakes , &gp_notMatched , 0.4);

      for(vector< pair<MyGenPart* , MyTracks*> >::iterator mpair = match.begin() ; mpair != match.end() ; ++mpair)
        nTr_ptVSeta_NSD_evtSel_matched_reco->Fill(mpair->second->Part.v.Eta() , mpair->second->Part.v.Pt());
    
      for(vector<MyTracks*>::iterator it_fake = fakes.begin() ; it_fake != fakes.end() ; ++it_fake)
        nTr_ptVSeta_NSD_evtSel_fakes_reco->Fill((*it_fake)->Part.v.Eta() , (*it_fake)->Part.v.Pt());
    
    }
    
    
  }//end of loop over events
  
  
  //output file
  ostringstream strout("FBtest");
  strout<<"NCHtest2_type"<<type<<"_"<<E<<"TeV_";
  if(iTracking==0)
    strout<<"gTr";
  if(iTracking==1)
    strout<<"mbTr";
  strout<<".root";
  TFile* output = new TFile(strout.str().c_str(),"RECREATE");
  output->cd();
        
  nTr_ptVSeta_NSD_evtSel_reco ->Write();
  nTr_ptVSeta_NSD_evtSel_reco_gt4hit->Write();
  nTr_ptVSeta_NSD_evtSel_gen  ->Write();
  nTr_ptVSeta_NSD_noEvtSel_gen->Write();
  
  nTr_ptVSeta_NSD_evtSel_matched_reco->Write();
  nTr_ptVSeta_NSD_evtSel_fakes_reco->Write();
  
  TH2F* eff_ptVSeta_NSD_evtSel_matched = (TH2F*) nTr_ptVSeta_NSD_evtSel_matched_reco ->Clone("eff_ptVSeta_NSD_evtSel_matched");
  TH2F* eff_ptVSeta_NSD_evtSel_fakes   = (TH2F*) nTr_ptVSeta_NSD_evtSel_fakes_reco   ->Clone("eff_ptVSeta_NSD_evtSel_fakes");
  TH2F* eff_ptVSeta_NSD_evtSel         = (TH2F*) nTr_ptVSeta_NSD_evtSel_reco         ->Clone("eff_ptVSeta_NSD_evtSel");
  TH2F* eff_ptVSeta_NSD_evtSel_gt4hit  = (TH2F*) nTr_ptVSeta_NSD_evtSel_reco_gt4hit  ->Clone("eff_ptVSeta_NSD_evtSel_gt4hit");
  
  eff_ptVSeta_NSD_evtSel_matched->Divide(nTr_ptVSeta_NSD_evtSel_gen);
  eff_ptVSeta_NSD_evtSel_fakes  ->Divide(nTr_ptVSeta_NSD_evtSel_gen);
  eff_ptVSeta_NSD_evtSel        ->Divide(nTr_ptVSeta_NSD_evtSel_gen);
  eff_ptVSeta_NSD_evtSel_gt4hit ->Divide(nTr_ptVSeta_NSD_evtSel_gen);

  eff_ptVSeta_NSD_evtSel_matched->Write();
  eff_ptVSeta_NSD_evtSel_fakes  ->Write();
  eff_ptVSeta_NSD_evtSel        ->Write();
  eff_ptVSeta_NSD_evtSel_gt4hit ->Write();


  output->Close();
}
