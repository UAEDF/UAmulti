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

bool isMC = true;
bool debug = false;


#include "fileManager.C"
#include "cuts.C"
#include "evtSel.C"
#include "binningMap.C"

void divideByWidth(TH1F*);


void smallCode(int type = 10 , double E = 0.9 , int iTracking = 1 , int nevt_max = 100){
  
  
  if(type==0) isMC = false;
  TString MCtype = "pythia";
  if(type == 20) MCtype = "phojet";
  
  
  vector< vector<double> > binning;
  binning = getBins(5,E);
  
  TH1F* nch_gen_NSD_noSel       = new TH1F("nch_gen_NSD_noSel","nch_gen_NSD_noSel",binning.at(0).size()-1,&binning[0][0]);
  TH1F* nch_gen_NSD_noSel_ptCut = new TH1F("nch_gen_NSD_noSel_ptCut","nch_gen_NSD_noSel_ptCut",binning.at(0).size()-1,&binning[0][0]);
  
  TProfile* mptVSnchreco_reco       = new TProfile("mptVSnchreco_reco","mptVSnchreco_reco",binning.at(0).size()-1,&(binning.at(0).at(0)));
  TProfile* mptVSnchreco_reco_ptCut = new TProfile("mptVSnchreco_reco_ptCut","mptVSnchreco_reco_ptCut",binning.at(0).size()-1,&(binning.at(0).at(0)));
  TProfile* mptVSnchreco_gen        = new TProfile("mptVSnchreco_gen","mptVSnchreco_gen",binning.at(0).size()-1,&(binning.at(0).at(0)));
  TProfile* mptVSnchreco_gen_ptCut  = new TProfile("mptVSnchreco_gen_ptCut","mptVSnchreco_gen_ptCut",binning.at(0).size()-1,&(binning.at(0).at(0)));
  
  
  TProfile* mptVSnchgen_gen       = new TProfile("mptVSnchgen_gen","mptVSnchgen_gen",binning.at(0).size()-1,&(binning.at(0).at(0)));
  TProfile* mptVSnchgen_gen_ptCut = new TProfile("mptVSnchgen_gen_ptCut","mptVSnchgen_gen_ptCut",binning.at(0).size()-1,&(binning.at(0).at(0)));
  //TProfile* mptVSnchgen_reco = new TProfile("mptVSnchgen_reco","mptVSnchgen_reco",binning.at(0).size()-1,&(binning.at(0).at(0)));
  
  TH1F* pt_gen        = new TH1F("pt_gen","pt_gen",200,0,4);
  TH1F* pt_gen_ptCut  = new TH1F("pt_gen_ptCut","pt_gen_ptCut",200,0,4);
  TH1F* pt_reco       = new TH1F("pt_reco","pt_reco",200,0,4);
  TH1F* pt_reco_ptCut = new TH1F("pt_reco_ptCut","pt_reco_ptCut",200,0,4);
  
  
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
    
    
    //if(i==0 && iTracking==1 && E==7.0) vertexToCut = vertex;
    
    //Skipping the SD events starting from here
    if(isMC)
      if(isSD(genKin , MCtype) ) continue;
    
    int n_gen_allPt = getnPrimaryGenPart(genPart,0,2.4);
    nch_gen_NSD_noSel->Fill(n_gen_allPt);
    
    int n_gen_ptCut = getnPrimaryGenPart(genPart,0.5,2.4);
    nch_gen_NSD_noSel_ptCut->Fill(n_gen_ptCut);
    
    double mpt_gen_noSel = 0;
    for(vector<MyGenPart>::iterator it_tr = genPart->begin() ; it_tr != genPart->end() ; ++it_tr)
      if( isGenPartGood(*it_tr) && isInAcceptance(it_tr->Part,0.,2.4) )
        mpt_gen_noSel += it_tr->Part.v.Pt();
    int n_gen_noSel = getnPrimaryGenPart(genPart,0.,2.4);
    if(n_gen_noSel!=0) mpt_gen_noSel/=n_gen_noSel;
    mptVSnchgen_gen->Fill(n_gen_noSel,mpt_gen_noSel);
    
    //skipping events that don't pass our event selection
    if(!isEvtGood(E,*L1Trig , *MITEvtSel , vertexToCut)) continue;
    
    
    //RECO NCH
    vector<MyTracks> trcoll;
    if(iTracking==0) trcoll = getPrimaryTracks(*tracks,vertex,0.1,2.4);
    if(iTracking==1) trcoll = getPrimaryTracks(*tracks,vertex,bs,0.1,2.4);
    int n = trcoll.size();

    double mpt_reco = 0;
    for(vector<MyTracks>::iterator itr = trcoll.begin() ; itr != trcoll.end() ; ++itr){
      mpt_reco+=itr->Part.v.Pt();
      pt_reco->Fill(itr->Part.v.Pt());
    }
    if(n!=0) mpt_reco/=n;
    mptVSnchreco_reco->Fill(n,mpt_reco);
    
    
    double mpt_gen = 0;
    for(vector<MyGenPart>::iterator it_tr = genPart->begin() ; it_tr != genPart->end() ; ++it_tr){
      if( isGenPartGood(*it_tr) && isInAcceptance(it_tr->Part,0.,2.4) ){
        mpt_gen += it_tr->Part.v.Pt();
	pt_gen->Fill(it_tr->Part.v.Pt());
      }
    }
    int n_gen = getnPrimaryGenPart(genPart,0.,2.4);
    if(n_gen!=0) mpt_gen/=n_gen;
    mptVSnchreco_gen->Fill(n,mpt_gen);
    
    //mptrecoVSgen->Fill(mpt_gen,mpt_reco);
    
    
  }//end of loop over events
  
  
  //output file
  ostringstream strout("");
  strout<<"smallCode_MCtype"<<type<<"_"<<E<<"TeV.root";
  TFile* output = new TFile(strout.str().c_str(),"RECREATE");
  output->cd();
  
  gStyle->SetPalette(1);
  //mptrecoVSgen->Draw("colz");
  
  divideByWidth(nch_gen_NSD_noSel);
  nch_gen_NSD_noSel->Write();
  divideByWidth(nch_gen_NSD_noSel_ptCut);
  nch_gen_NSD_noSel_ptCut->Write();
  
  mptVSnchreco_reco->Write();
  mptVSnchreco_gen->Write();
  mptVSnchgen_gen->Write();
  
  mptVSnchreco_gen->Divide(mptVSnchreco_reco);
  mptVSnchreco_gen->Write("eff_mtp_genOreco");
  
  
  pt_gen->Write();
  pt_reco->Write();
  pt_gen->Add(pt_reco,-1);
  pt_gen->Write("pt_genMINUSreco");
  
  output->Close();
}

void divideByWidth(TH1F* hist){
  for(int i=1;i<=hist->GetNbinsX();++i){
    hist->SetBinContent(i,double(hist->GetBinContent(i))/double(hist->GetBinWidth(i)));
    hist->SetBinError(i,double(hist->GetBinError(i))/double(hist->GetBinWidth(i)));
  }
}



