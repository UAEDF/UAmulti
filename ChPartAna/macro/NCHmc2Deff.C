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

#include "../mydir/MyEvtId.h"
#include "../mydir/MyL1Trig.h"
#include "../mydir/MyHLTrig.h"
#include "../mydir/MyGenKin.h"
#include "../mydir/MyPart.h"
#include "../mydir/MyGenPart.h"
#include "../mydir/MyTracks.h"
#include "../mydir/MyVertex.h"
#include "../mydir/MyMITEvtSel.h"
#include "../mydir/MyBeamSpot.h"

#include "../plugins/EfficiencyPlots.h"

bool isMC = true;
bool debug = false;


#include "fileManager.C"
#include "files_funcs.C"
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

void NCHmc2Deff(int type = 62 , double E = 7. , int iTracking = 0 , int nevt_max = 2000000000 , int ver = 8){
  
  
  if(type==0) isMC = false;
  TString MCtype = "pythia";
  if(type == 20) MCtype = "phojet";
  if(type == 60 || type==101) MCtype = "pythia8";
  
  pt_cut  = 0.05;
  eta_cut = 2.5 ;

  vector< vector<double> > binning;
  binning = getBins(2,1,3);//nch,pt,eta
  
  
  BasePlots* baseplot = new BasePlots("BasePlots");
  baseplot->setBinning(binning);
  
  
  EfficiencyPlots effp_noEvtSel("noEvtSel");
  EfficiencyPlots effp_evtSel("evtSel");
  EfficiencyPlots effp_evtSel_HP("evtSel_HP");
  
  
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
    tree->SetBranchAddress("evtId",&evtId);
    if(isMC) tree->SetBranchAddress("genPart",&genPart);
    if(isMC) tree->SetBranchAddress("genKin",&genKin);
    if(iTracking==0){
      tree->SetBranchAddress("generalTracks",&tracks);
      tree->SetBranchAddress("offlinePrimaryVertices",&vertex);
    }
    else if(iTracking==1){
      tree->SetBranchAddress("allTracks",&tracks);
      tree->SetBranchAddress("allVertices",&vertex);
    }
    else if(iTracking==2){
      tree->SetBranchAddress("generalPlusMinBiasTracks",&tracks);
      tree->SetBranchAddress("offlinePrimaryVerticesWithMBTracks",&vertex);
    }
    tree->SetBranchAddress("L1Trig",&L1Trig);
    tree->SetBranchAddress("HLTrig",&HLTrig);
    tree->SetBranchAddress("MITEvtSel",&MITEvtSel);
    tree->SetBranchAddress("offlineBeamSpot",&bs);

    
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
      if(iTracking==2) getPrimaryTracks(trcoll,vertex,pt_cut,eta_cut);
    
      //If doesn't pass central requirement, skip event
      //if( ! ( passCentral(trcoll,"ATLAS1",0.5) )) continue;

      vector<MyGenPart> gpcoll_largeAcc = *genPart;
      if(isMC){
        getPrimaryGenPart(gpcoll_largeAcc , pt_cut - 0.1 , eta_cut + 0.5);
	
	effp_noEvtSel.fill(&gpcoll_largeAcc , &trcoll);
      }
    
    
      //skipping events that don't pass our event selection
      //if(!isEvtGood(E,*L1Trig , *MITEvtSel , vertexToCut)) continue;
      if( ! ( passL1HLT(E, *L1Trig, *HLTrig , 1,isMC ) &&
              passVtxQual(*MITEvtSel,E)                &&
              passVtx(*vertex) )
         )
         continue;


      if(isMC){
	
	effp_evtSel.fill( &gpcoll_largeAcc , &trcoll );
	
	//matching for High Purity Tracks
	if(iTracking == 0){
	
	  vector<MyTracks> trcoll_HP = *tracks;
          getPrimaryTracks(trcoll_HP,vertex,pt_cut,eta_cut , 0 , 1);
	
	
	  effp_evtSel_HP.fill(&gpcoll_largeAcc , &trcoll_HP);
	  
	}  
	  
    
      }
    
    
    }//end of loop over events
  
    //Closing current files
    file->Close();

  }//end of loop over files

  //output file
  ostringstream strout("FBtest");
  strout<<"NCHtest" << ver << "_type"<<type<<"_"<<E<<"TeV_";
  if(iTracking==0)
    strout<<"gTr";
  if(iTracking==1)
    strout<<"mbTr";
  if(iTracking==2)
    strout<<"mbgtTr";
  strout<<".root";
  cout << "Output file: " << strout.str() << endl;
  TFile* output = new TFile(strout.str().c_str(),"RECREATE");
  output->cd();
  
  
  effp_noEvtSel.write();
  effp_evtSel.write();
  
  if(iTracking==0){
  
    effp_evtSel_HP.write();
  
  }


  output->Close();
}
