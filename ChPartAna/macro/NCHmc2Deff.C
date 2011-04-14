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
#include "../plugins/MyHLTrig.h"
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
#include "FBacc.C"
#include "FBcuts.C"
#include "NCHEvtSel.C"
#include "binningMap.C"

TString st(string input , int cut){
  stringstream out("");
  out<<cut;
  return input+"_cut"+out.str();
}

//nohup root -b -l BuildLibDico.C+ NCHmc2Deff.C+"(60,7,1,100000000)" -q > log_nchmc2deff_type60_7TeV.txt &

void NCHmc2Deff(int type = 31 , double E = 7. , int iTracking = 1 , int nevt_max = 900000000){
  
  
  if(type==0) isMC = false;
  TString MCtype = "pythia";
  if(type == 20) MCtype = "phojet";
  if(type == 60 || type==101) MCtype = "pythia8";
  
  pt_cut  = 0.05;
  eta_cut = 2.5 ;

  vector< vector<double> > binning;
  binning = getBins(2,1,3);//nch,pt,eta
    
  TH2F* nTr_ptVSeta_INEL_evtSel_reco  = new TH2F("nTr_ptVSeta_INEL_evtSel_reco" , "nTr_ptVSeta_INEL_evtSel_reco ;#eta;pt" , binning.at(2).size()-1 , &(binning.at(2).at(0)) ,  binning.at(1).size()-1 , &(binning.at(1).at(0)) );   
  TH2F* nTr_ptVSeta_INEL_evtSel_reco_gt4hit  = new TH2F("nTr_ptVSeta_INEL_evtSel_reco_gt4hit" , "nTr_ptVSeta_INEL_evtSel_reco_gt4hit ;#eta;pt" , binning.at(2).size()-1 , &(binning.at(2).at(0)) ,  binning.at(1).size()-1 , &(binning.at(1).at(0)) );   
  TH2F* nTr_ptVSeta_INEL_evtSel_gen   = new TH2F("nTr_ptVSeta_INEL_evtSel_gen"  , "nTr_ptVSeta_INEL_evtSel_gen  ;#eta;pt" , binning.at(2).size()-1 , &(binning.at(2).at(0)) ,  binning.at(1).size()-1 , &(binning.at(1).at(0)) );   
  TH2F* nTr_ptVSeta_INEL_noEvtSel_gen = new TH2F("nTr_ptVSeta_INEL_noEvtSel_gen", "nTr_ptVSeta_INEL_noEvtSel_gen;#eta;pt" , binning.at(2).size()-1 , &(binning.at(2).at(0)) ,  binning.at(1).size()-1 , &(binning.at(1).at(0)) );   
  nTr_ptVSeta_INEL_evtSel_reco ->Sumw2();
  nTr_ptVSeta_INEL_evtSel_reco_gt4hit->Sumw2();
  nTr_ptVSeta_INEL_evtSel_gen  ->Sumw2();
  nTr_ptVSeta_INEL_noEvtSel_gen->Sumw2();
  
  TH2F* nTr_ptVSeta_INEL_evtSel_matched_reco  = new TH2F("nTr_ptVSeta_INEL_evtSel_matched_reco" , "nTr_ptVSeta_INEL_evtSel_matched_reco ;#eta;pt" , binning.at(2).size()-1 , &(binning.at(2).at(0)) ,  binning.at(1).size()-1 , &(binning.at(1).at(0)) );   
  TH2F* nTr_ptVSeta_INEL_evtSel_fakes_reco    = new TH2F("nTr_ptVSeta_INEL_evtSel_fakes_reco"   , "nTr_ptVSeta_INEL_evtSel_fakes_reco   ;#eta;pt" , binning.at(2).size()-1 , &(binning.at(2).at(0)) ,  binning.at(1).size()-1 , &(binning.at(1).at(0)) );   
  TH2F* nTr_ptVSeta_INEL_evtSel_matched_gen   = new TH2F("nTr_ptVSeta_INEL_evtSel_matched_gen"  , "nTr_ptVSeta_INEL_evtSel_matched_gen  ;#eta;pt" , binning.at(2).size()-1 , &(binning.at(2).at(0)) ,  binning.at(1).size()-1 , &(binning.at(1).at(0)) );   
  nTr_ptVSeta_INEL_evtSel_matched_reco->Sumw2();
  nTr_ptVSeta_INEL_evtSel_fakes_reco ->Sumw2();
  nTr_ptVSeta_INEL_evtSel_matched_gen ->Sumw2();
   
  TH2F* nTr_ptVSeta_INEL_evtSel_matched_reco_HP  = new TH2F("nTr_ptVSeta_INEL_evtSel_matched_reco_HP" , "nTr_ptVSeta_INEL_evtSel_matched_reco_HP ;#eta;pt" , binning.at(2).size()-1 , &(binning.at(2).at(0)) ,  binning.at(1).size()-1 , &(binning.at(1).at(0)) );   
  TH2F* nTr_ptVSeta_INEL_evtSel_fakes_reco_HP    = new TH2F("nTr_ptVSeta_INEL_evtSel_fakes_reco_HP"   , "nTr_ptVSeta_INEL_evtSel_fakes_reco_HP   ;#eta;pt" , binning.at(2).size()-1 , &(binning.at(2).at(0)) ,  binning.at(1).size()-1 , &(binning.at(1).at(0)) );   
  TH2F* nTr_ptVSeta_INEL_evtSel_matched_gen_HP   = new TH2F("nTr_ptVSeta_INEL_evtSel_matched_gen_HP"  , "nTr_ptVSeta_INEL_evtSel_matched_gen_HP  ;#eta;pt" , binning.at(2).size()-1 , &(binning.at(2).at(0)) ,  binning.at(1).size()-1 , &(binning.at(1).at(0)) );   
  nTr_ptVSeta_INEL_evtSel_matched_reco_HP->Sumw2();
  nTr_ptVSeta_INEL_evtSel_fakes_reco_HP ->Sumw2();
  nTr_ptVSeta_INEL_evtSel_matched_gen_HP ->Sumw2();
  
  cout << "Finished initialization of TH2s " << endl;
  
  //getting the list of files
  vector<TString>* vfiles = getListOfFiles(fileManager(0,type,E));

  //Declaration of tree and its branches variables
  TTree* tree = new TChain("evt","");
  MyEvtId*           evtId        = NULL ;
  vector<MyGenPart>* genPart      = NULL;
  MyGenKin*          genKin       = NULL;
  vector<MyTracks>*  tracks       = NULL;
  vector<MyVertex>*  vertex       = NULL;
  vector<MyVertex>*  vertexToCut  = NULL;
  MyL1Trig*          L1Trig       = NULL;
  MyHLTrig*          HLTrig       = NULL;
  MyMITEvtSel*       MITEvtSel    = NULL;
  MyBeamSpot*        bs           = NULL;

  int i_tot = 0 , nevt_tot = 0;
  //starting Loop over files, stops at end of list of files or when reached nevt_max
  for(vector<TString>::iterator itfiles = vfiles->begin() ; itfiles != vfiles->end() && i_tot < nevt_max ; ++itfiles){

    TFile* file = TFile::Open(*itfiles,"READ");

    //getting the tree form the current file
    tree = (TTree*) file->Get("evt");

    //adding branches to the tree ----------------------------------------------------------------------
    tree->SetBranchAddress("EvtId",&evtId);
    if(isMC) tree->SetBranchAddress("GenPart",&genPart);
    if(isMC) tree->SetBranchAddress("GenKin",&genKin);
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
    tree->SetBranchAddress("HLTrig",&HLTrig);
    tree->SetBranchAddress("MITEvtSel",&MITEvtSel);
    tree->SetBranchAddress("beamSpot",&bs);

    
    //Just to put the good collection of vertex in vertexToCut
    vertexToCut = vertex;

    //Getting number of events
    int nev = int(tree->GetEntriesFast());
    nevt_tot += nev;
    cout <<"The current file has " << nev << " entries : "<< endl << *itfiles << endl;
    //cout<<"Running on: "<<min(nev,nevt_max)<<" events"<<endl;

    //starting loop over events, stops when reached end of file or nevt_max
    for(int i = 0; i < nev && i_tot < nevt_max; ++i , ++i_tot){

      //printing the % of events done every 10k evts
      if( ((i_tot+1) % 10000) == 0) cout <<int(double(i_tot+1)/1000)<<"k done"<<endl;

      //if(i>min(nev,nevt_max)) break;

      //Filling the variables defined setting branches
      tree->GetEntry(i);


    
      //Skipping the SD events starting from here
      //if(isMC) if(isSD(genKin , MCtype) ) continue;
    
      
      vector<MyTracks> trcoll = *tracks;
      if(iTracking==0) getPrimaryTracks(trcoll,vertex,pt_cut,eta_cut);
      if(iTracking==1) getPrimaryTracks(trcoll,vertex,bs,pt_cut,eta_cut);
    
      //If doesn't pass central requirement, skip event
      //if( ! ( passCentral(trcoll,"ATLAS1",0.5) )) continue;

      vector<MyGenPart> gpcoll = *genPart;
      if(isMC){
        getPrimaryGenPart(gpcoll);
        for(vector<MyGenPart>::iterator p=gpcoll.begin() ; p!=gpcoll.end() ; p++ )
	    nTr_ptVSeta_INEL_noEvtSel_gen->Fill(p->Part.v.Eta() , p->Part.v.Pt());
      }
    
    
      //skipping events that don't pass our event selection
      //if(!isEvtGood(E,*L1Trig , *MITEvtSel , vertexToCut)) continue;
      if( ! ( passL1HLT(E, *L1Trig, *HLTrig , 1,isMC ) &&
              passVtxQual(*MITEvtSel,E)                &&
              passVtx(*vertex) )
         )
         continue;

      if(isMC){
        for(vector<MyGenPart>::iterator p=gpcoll.begin() ; p!=gpcoll.end() ; p++ )
	    nTr_ptVSeta_INEL_evtSel_gen->Fill(p->Part.v.Eta() , p->Part.v.Pt()); 
      }
    
      for(vector<MyTracks>::iterator it_tr = trcoll.begin() ; it_tr != trcoll.end() ; ++it_tr){
        nTr_ptVSeta_INEL_evtSel_reco->Fill(it_tr->Part.v.Eta() , it_tr->Part.v.Pt());
        if(it_tr->nhit>4)
          nTr_ptVSeta_INEL_evtSel_reco_gt4hit->Fill(it_tr->Part.v.Eta() , it_tr->Part.v.Pt());
      }
    
      if(isMC){
	  
        vector<MyGenPart> gpcoll_largeAcc = *genPart;
	getPrimaryGenPart(gpcoll_largeAcc , pt_cut - 0.1 , eta_cut + 0.5);
	//cout << "gp size : " << gpcoll->size() << endl;
        
        vector< pair<MyGenPart* , MyTracks*> > match;
        vector<MyTracks*>                      fakes;
        vector<MyGenPart*>                     gp_notMatched;

        GetMatch2(&gpcoll_largeAcc , &trcoll , &match , &fakes , &gp_notMatched , 0.04);

        //fill with pt & eta of gen to correct for acceptancy
        for(vector< pair<MyGenPart* , MyTracks*> >::iterator mpair = match.begin() ; mpair != match.end() ; ++mpair)
          nTr_ptVSeta_INEL_evtSel_matched_reco->Fill(mpair->first->Part.v.Eta() , mpair->first->Part.v.Pt());
    
        for(vector<MyTracks*>::iterator it_fake = fakes.begin() ; it_fake != fakes.end() ; ++it_fake)
          nTr_ptVSeta_INEL_evtSel_fakes_reco->Fill((*it_fake)->Part.v.Eta() , (*it_fake)->Part.v.Pt());
	  
	
	//matching for High Purity Tracks
	if(iTracking == 0){
	
	  vector<MyTracks> trcoll_HP = *tracks;
          getPrimaryTracks(trcoll_HP,vertex,pt_cut,eta_cut , 0 , 1);
	
          vector< pair<MyGenPart* , MyTracks*> > match_HP;
          vector<MyTracks*>                      fakes_HP;
          vector<MyGenPart*>                     gp_notMatched_HP;

          GetMatch2(&gpcoll_largeAcc , &trcoll_HP , &match_HP , &fakes_HP , &gp_notMatched_HP , 0.04);

          //fill with pt & eta of gen to correct for acceptancy
          for(vector< pair<MyGenPart* , MyTracks*> >::iterator mpair = match_HP.begin() ; mpair != match_HP.end() ; ++mpair)
            nTr_ptVSeta_INEL_evtSel_matched_reco_HP->Fill(mpair->first->Part.v.Eta() , mpair->first->Part.v.Pt());
    
          for(vector<MyTracks*>::iterator it_fake = fakes_HP.begin() ; it_fake != fakes_HP.end() ; ++it_fake)
            nTr_ptVSeta_INEL_evtSel_fakes_reco_HP->Fill((*it_fake)->Part.v.Eta() , (*it_fake)->Part.v.Pt());
	  
	}  
	  
    
      }
    
    
    }//end of loop over events
  
    //Closing current files
    file->Close();

  }//end of loop over files

  //output file
  ostringstream strout("FBtest");
  strout<<"NCHtest5_type"<<type<<"_"<<E<<"TeV_";
  if(iTracking==0)
    strout<<"gTr";
  if(iTracking==1)
    strout<<"mbTr";
  strout<<".root";
  TFile* output = new TFile(strout.str().c_str(),"RECREATE");
  output->cd();
        
  nTr_ptVSeta_INEL_evtSel_reco ->Write();
  nTr_ptVSeta_INEL_evtSel_reco_gt4hit->Write();
  nTr_ptVSeta_INEL_evtSel_gen  ->Write();
  nTr_ptVSeta_INEL_noEvtSel_gen->Write();
  
  nTr_ptVSeta_INEL_evtSel_matched_reco->Write();
  nTr_ptVSeta_INEL_evtSel_fakes_reco->Write();
  
  TH2F* eff_ptVSeta_INEL_evtSel_matched = (TH2F*) nTr_ptVSeta_INEL_evtSel_matched_reco ->Clone("eff_ptVSeta_INEL_evtSel_matched");
  TH2F* eff_ptVSeta_INEL_evtSel_fakes   = (TH2F*) nTr_ptVSeta_INEL_evtSel_fakes_reco   ->Clone("eff_ptVSeta_INEL_evtSel_fakes");
  TH2F* eff_ptVSeta_INEL_evtSel         = (TH2F*) nTr_ptVSeta_INEL_evtSel_reco         ->Clone("eff_ptVSeta_INEL_evtSel");
  TH2F* eff_ptVSeta_INEL_evtSel_gt4hit  = (TH2F*) nTr_ptVSeta_INEL_evtSel_reco_gt4hit  ->Clone("eff_ptVSeta_INEL_evtSel_gt4hit");
  
  eff_ptVSeta_INEL_evtSel_matched->Divide(nTr_ptVSeta_INEL_evtSel_gen);
  eff_ptVSeta_INEL_evtSel_fakes  ->Divide(nTr_ptVSeta_INEL_evtSel_gen);
  eff_ptVSeta_INEL_evtSel        ->Divide(nTr_ptVSeta_INEL_evtSel_gen);
  eff_ptVSeta_INEL_evtSel_gt4hit ->Divide(nTr_ptVSeta_INEL_evtSel_gen);

  eff_ptVSeta_INEL_evtSel_matched->Write();
  eff_ptVSeta_INEL_evtSel_fakes  ->Write();
  eff_ptVSeta_INEL_evtSel        ->Write();
  eff_ptVSeta_INEL_evtSel_gt4hit ->Write();
  
  
  if(iTracking==0){
  
    nTr_ptVSeta_INEL_evtSel_matched_reco_HP->Write();
    nTr_ptVSeta_INEL_evtSel_fakes_reco_HP->Write();
  
    TH2F* eff_ptVSeta_INEL_evtSel_matched_HP = (TH2F*) nTr_ptVSeta_INEL_evtSel_matched_reco_HP ->Clone("eff_ptVSeta_INEL_evtSel_matched_HP");
    TH2F* eff_ptVSeta_INEL_evtSel_fakes_HP   = (TH2F*) nTr_ptVSeta_INEL_evtSel_fakes_reco_HP   ->Clone("eff_ptVSeta_INEL_evtSel_fakes_HP");
  
    eff_ptVSeta_INEL_evtSel_matched_HP->Divide(nTr_ptVSeta_INEL_evtSel_gen);
    eff_ptVSeta_INEL_evtSel_fakes_HP  ->Divide(nTr_ptVSeta_INEL_evtSel_gen);

    eff_ptVSeta_INEL_evtSel_matched_HP->Write();
    eff_ptVSeta_INEL_evtSel_fakes_HP  ->Write();
  
  
  }


  output->Close();
}
