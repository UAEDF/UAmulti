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

//STANDARD C++ INCLUDES
#include <iostream>
#include <sstream>
using namespace std;

//OUR OWN CLASSES TO READ THE TREE
#include "../plugins/MyEvtId.h"
#include "../plugins/MyL1Trig.h"
#include "../plugins/MyGenKin.h"
#include "../plugins/MyPart.h"
#include "../plugins/MyGenPart.h"
#include "../plugins/MyTracks.h"
#include "../plugins/MyVertex.h"
#include "../plugins/MyMITEvtSel.h"
#include "../plugins/MyBeamSpot.h"

bool isMC = true;
bool debug = false;

//USED TO KNOW WHICH FILE TO GET
//NOT USEFUL IF NOT RUNNING ON THE T2
#include "fileManager.C"

//USED TO SELECT GOOD TRACKS OR GenPart
#include "cuts.C"

//USED TO SELECT GOOD EVENTS
#include "evtSel.C"

//MAIN FUNCTION
//PARAMETERS ARE :
//type : 0 for data , 10 (31) for 0.9 (7TeV) MonteCarlo    					[useful only with filemanager]
//E : energy of the collisions, 0.9 or 7							[useful only with filemanager]
//iTracking : algorithm for the tracking (general tracks = 0 , minbias tracks = 1 as default)
//nevt_max : max number of events to run on.							[50k/100k per files]
void templateTreeAna(int type = 0 , double E = 0.9 , int iTracking = 1 , int nevt_max = 100){
  
  //running on MC ?
  if(type==0) isMC = false;
  TString MCtype = "pythia";
  if(type == 20) MCtype = "phojet";
  if(type == 60 || type==101) MCtype = "pythia8";

  //Declaration of my th1s
  TH1F* pt_gen  = new TH1F("pt_gen" , "pt_gen;pt;nTracks" , 120 , 0 , 6);
  TH1F* pt_reco = new TH1F("pt_reco", "pt_reco;pt;nTracks" , 120 , 0 , 6);
  
  //getting the list of files
  vector<TString>* vfiles = getListOfFiles(fileManager(0,type,E));
  
  //Declaration of tree and its branches variables
  TTree* tree = new TTree("evt","");
  MyEvtId*           evtId        = NULL ;
  vector<MyGenPart>* genPart      = NULL;
  MyGenKin*          genKin       = NULL;
  vector<MyTracks>*  tracks       = NULL;
  vector<MyVertex>*  vertex       = NULL;
  vector<MyVertex>*  vertexToCut  = NULL;
  MyL1Trig*          L1Trig       = NULL;
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


      //AT THIS POINT ON, CAN START USING THE VARIABLES LIKE TRACKS, VERTEX ETC !


      //-------------------------------------------------------------------------------------------------
      //filling pt distribution for the generated particles
      //ie those from pythia generator, without reconstruction
      if(isMC){
        for(vector<MyGenPart>::iterator p=genPart->begin() ; p!=genPart->end() ; p++ )
          pt_gen->Fill(p->Part.v.Pt());
      }


      //Skipping the Single-Diffractive events starting from here
      //function isSD() defined in evtSel.C
      if(isMC)
        if(isSD(genKin , MCtype) ) continue;
      

      //cout << "debug5" << endl;
      //-------------------------------------------------------------------------------------------------
      //skipping events that don't pass our event selection
      //function isEvtGood() defined in evtSel.C
      if(!isEvtGood(E,*L1Trig , *MITEvtSel , vertexToCut)) continue;
    

      //-------------------------------------------------------------------------------------------------
      //filling pt distribution for the observed/reconstructed tracks in the detector
      for(vector<MyTracks>::iterator it_tr = tracks->begin() ; it_tr != tracks->end() ; ++it_tr)
        pt_reco->Fill(it_tr->Part.v.Pt());



    }//end of loop over events
    
    //Closing current files
    file->Close();
    
  }//end of loop over files
  
  
  //output file
  TFile* output = new TFile("output.root","RECREATE");
  output->cd();
  
  pt_gen->Write();
  pt_reco->Write();

  output->Close();
}
