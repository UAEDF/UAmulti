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
#include "../plugins/FBCorrel.h"

bool isMC  = true;
bool debug = false;


#include "FBacc.C"
#include "fileManager.C"
#include "FBcuts.C"
#include "evtSel.C"
#include "binningMap.C"

TString st(string input , int cut){
  stringstream out("");
  out<<cut;
  return input+"_cut"+out.str();
}


void FBana(int type = 31 , double E = 7.0 , int iTracking = 1 , int nevt_max = 100){

  TTProfile::switchTH1On = 0;
  TTProfile::switchTH2On = 1;
  
  if(debug) cout << "Getting the acceptance map ... " << endl;
  getAccMap();
  
  if(debug) cout << "Initializing classes ... " << endl;
  vector<FBCorrel*> fbc_NSD_evtSel_reco_mnMVSnP (accMap.size(),0);
  vector<FBCorrel*> fbc_NSD_evtSel_reco         (accMap.size(),0);
  vector<FBCorrel*> fbc_NSD_evtSel_gen          (accMap.size(),0);
  vector<FBCorrel*> fbc_NSD_noEvtSel_gen        (accMap.size(),0);
  vector<FBCorrel*> fbc_NSD_noEvtSel_gen_mnMVSnP(accMap.size(),0);
  vector<FBCorrel*> fbc_noEvtSel_gen            (accMap.size(),0);
  
  
  for(int acc = 0 ; acc < (signed) accMap.size() ; ++acc){
    if(debug) cout << "    ==> acceptance bin " << acc << endl;
    //vector< vector<double> > binning;
    //binning = getBins(1,0,1);//nch,pt,eta
    //binning = getBins(acc,Ebinning);//nch,pt,eta
    //baseplot->setBinning(binning);

    //----------------------  RECO  ----------------------
    fbc_NSD_evtSel_reco_mnMVSnP.at(acc)  = new FBCorrel(st("NSD_evtSel_reco_mnMVSnP",acc));
    fbc_NSD_evtSel_reco.at(acc)          = new FBCorrel(st("NSD_evtSel_reco",acc));
    fbc_NSD_evtSel_gen.at(acc)           = new FBCorrel(st("NSD_evtSel_gen",acc));
    fbc_NSD_noEvtSel_gen.at(acc)         = new FBCorrel(st("NSD_noEvtSel_gen",acc));
    fbc_NSD_noEvtSel_gen_mnMVSnP.at(acc) = new FBCorrel(st("NSD_noEvtSel_gen_mnMVSnP",acc));
    fbc_noEvtSel_gen.at(acc)             = new FBCorrel(st("NSD_noEvtSel_gen",acc));
  }
  if(debug) cout << "                          ... Done !!";
  
  if(type==0) isMC = false;
  TString MCtype = "pythia";
  if(type == 20) MCtype = "phojet";
  
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



    vector<MyGenPart> gpcoll;
    pair<double,double> nCorrelGp = make_pair(0,0);
    if(isMC){
      gpcoll = getPrimaryGenPart(*genPart , 0.1 , 2.5 , 0);
      
      //Filling Inelastic Gen Level
      for(int acc = 0 ; acc < (signed) accMap.size() ; ++acc){
        nCorrelGp = getnCorrelGenPart(gpcoll , accMap.at(acc));
        fbc_noEvtSel_gen.at(acc)->Fill(nCorrelGp.first , nCorrelGp.second);
      }
      
      //Skipping the SD events starting from here
      if(isSD(genKin , MCtype) ) continue;
      
      //Filling NSD Gen Level
      for(int acc = 0 ; acc < (signed) accMap.size() ; ++acc){
        nCorrelGp = getnCorrelGenPart(gpcoll , accMap.at(acc));
        fbc_NSD_noEvtSel_gen.at(acc)->Fill(nCorrelGp.first , nCorrelGp.second);
        fbc_NSD_noEvtSel_gen_mnMVSnP.at(acc)->Fill(nCorrelGp.second , nCorrelGp.first);
      }
    }  
      
    //skipping events that don't pass our event selection
    if(!isEvtGood(E,*L1Trig , *MITEvtSel , vertexToCut)) continue;
    
    
    //Filling NSD Gen Level AFTER evtSel
    if(isMC){
      for(int acc = 0 ; acc < (signed) accMap.size() ; ++acc){
        nCorrelGp = getnCorrelGenPart(gpcoll , accMap.at(acc));
        fbc_NSD_evtSel_gen.at(acc)->Fill(nCorrelGp.first , nCorrelGp.second);
      }
    }
    
    
    vector<MyTracks> trcoll;
    if(iTracking==0) trcoll = getPrimaryTracks(*tracks,vertex,0.1,2.5,0);
    if(iTracking==1) trcoll = getPrimaryTracks(*tracks,vertex,bs,0.1,2.5,0);
    
    pair<double,double> nCorrelTr = make_pair(0,0);
    for(int acc = 0 ; acc < (signed) accMap.size() ; ++acc){
      nCorrelTr = getnCorrelTracks(trcoll , accMap.at(acc));
      fbc_NSD_evtSel_reco.at(acc)->Fill(nCorrelTr.first , nCorrelTr.second);
      fbc_NSD_evtSel_reco_mnMVSnP.at(acc)->Fill(nCorrelTr.second , nCorrelTr.first);
    }
    
  }//end of loop over events
  
  
  //output file
  ostringstream strout("FBtest");
  strout<<"FBtest_type"<<type<<"_"<<E<<"TeV.root";
  TFile* output = new TFile(strout.str().c_str(),"RECREATE");
  output->cd();
  
  for(int acc = 0 ; acc < (signed) accMap.size() ; ++acc){
    //if(isMC) fbc_NSD_noEvtSel_gen.at(acc)->ttp.write(1,1);
    //fbc_NSD_evtSel_reco.at(acc)->ttp.write(1,1);
    
    fbc_NSD_evtSel_reco.at(acc)->write();
    fbc_NSD_evtSel_reco_mnMVSnP.at(acc)->write();
    if(isMC){
      fbc_NSD_evtSel_gen.at(acc)->write();
      fbc_NSD_noEvtSel_gen.at(acc)->write();
      fbc_NSD_noEvtSel_gen_mnMVSnP.at(acc)->write();
    }
  }
  output->Close();
}
