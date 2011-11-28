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

#include "../plugins/TResponseMtx.h"
#include "../plugins/BasePlots.h"
#include "../plugins/MultiPlots.h"
#include "../plugins/GenPartPlots.h"
#include "../plugins/NCHEvtSelPlots.h"
#include "../plugins/NCHHFPlots.h"
#include "../plugins/NCHCentralPlots.h"
#include "../plugins/NCHDiffPlots.h"

#include "../plugins/TrackPlots.h"
#include "../plugins/VertexPlots.h"
//#include "../plugins/EvtSelPlots.h"
//#include "../plugins/BasePlots.h"
//#include "../plugins/MultiPlots.h"
//#include "../plugins/GenMultiPlots.h"
//#include "../plugins/MatrixPlots.h"

bool isMC = true;
bool debug = false;


#include "fileManager.C"
#include "files_funcs.C"

#include "FBacc.C"
#include "FBcuts.C"
//#include "NCHEvtSel.C"
//#include "binningMap.C"

//nohup root -b -l BuildLibDico.C+ NCHrunCheck.C+"(0,7,1,100000000)" -q   > log_nchruncheck_type0_7TeV.txt &

void NCHrunCheck(int type = 0 , double E = 7 , int iTracking = 1 , int nevt_max = 10000){
  
  if(type < 6 ) isMC = false;  
  TString MCtype = "pythia";
  if(type == 20) MCtype = "phojet";
  //if(type == 15) MCtype = "z2";
  if(type == 60 | type == 62) MCtype = "pythia8";
  
  //Detach histograms from rootfiles (they are no closed if the file is closed.)
  TH1::AddDirectory(0);
  
  TH1F* nEvts_raw              = new TH1F("nEvts_raw","nEvts_raw",100,0.,100.);
  TH1F* nEvts_b34              = new TH1F("nEvts_b34","nEvts_b34",100,0.,100.);
  TH1F* nEvts_b124             = new TH1F("nEvts_b124","nEvts_b124",100,0.,100.);
  TH1F* nEvts_b124_HLT_MB  = new TH1F("nEvts_b124_HLT_MB","nEvts_b124_HLT_MB",100,0.,100.);
  TH1F* nEvts_HLT_MB           = new TH1F("nEvts_HLT_MB","nEvts_HLT_MB",100,0.,100.);
  TH1F* nEvts_HLT_ST           = new TH1F("nEvts_HLT_ST","nEvts_HLT_ST",100,0.,100.);
  TH1F* nEvts_HLT_MB_ST        = new TH1F("nEvts_HLT_MB_ST","nEvts_HLT_MB_ST",100,0.,100.);
  
  TH1F* nEvts_b63              = new TH1F("nEvts_b63","nEvts_b63",100,0.,100.);
  TH1F* nEvts_b63_HLT_M40      = new TH1F("nEvts_b63_HLT_M40","nEvts_b63_HLT_M40",100,0.,100.);
  TH1F* nEvts_b63_HLT_M70      = new TH1F("nEvts_b63_HLT_M70","nEvts_b63_HLT_M70",100,0.,100.);
  TH1F* nEvts_b63_HLT_M85      = new TH1F("nEvts_b63_HLT_M85","nEvts_b63_HLT_M85",100,0.,100.);
  
  
  TH1F* mnch_raw               = new TH1F("mnch_raw","mnch_raw",100,0.,100.);
  TH1F* mnch_b124_HLT_MB   = new TH1F("mnch_b124_HLT_MB","mnch_b124_HLT_MB",100,0.,100.);
  TH1F* mnch_nEvts_b63_HLT_M40 = new TH1F("mnch_nEvts_b63_HLT_M40","mnch_nEvts_b63_HLT_M40",100,0.,100.);
  TH1F* mnch_nEvts_b63_HLT_M70 = new TH1F("mnch_nEvts_b63_HLT_M70","mnch_nEvts_b63_HLT_M70",100,0.,100.);
  TH1F* mnch_nEvts_b63_HLT_M85 = new TH1F("mnch_nEvts_b63_HLT_M85","mnch_nEvts_b63_HLT_M85",100,0.,100.);
  
  TH1F* mnch_raw_PV               = new TH1F("mnch_raw_PV","mnch_raw_PV",100,0.,100.);
  TH1F* mnch_b124_HLT_MB_PV   = new TH1F("mnch_b124_HLT_MB_PV","mnch_b124_HLT_MB_PV",100,0.,100.);
  TH1F* mnch_nEvts_b63_HLT_M40_PV = new TH1F("mnch_nEvts_b63_HLT_M40_PV","mnch_nEvts_b63_HLT_M40_PV",100,0.,100.);
  TH1F* mnch_nEvts_b63_HLT_M70_PV = new TH1F("mnch_nEvts_b63_HLT_M70_PV","mnch_nEvts_b63_HLT_M70_PV",100,0.,100.);
  TH1F* mnch_nEvts_b63_HLT_M85_PV = new TH1F("mnch_nEvts_b63_HLT_M85_PV","mnch_nEvts_b63_HLT_M85_PV",100,0.,100.);
  
  
  TH1F* mnvtx_raw               = new TH1F("mnvtx_raw","mnvtx_raw",100,0.,100.);
  TH1F* mnvtx_b124_HLT_MB   = new TH1F("mnvtx_b124_HLT_MB","mnvtx_b124_HLT_MB",100,0.,100.);
  TH1F* mnvtx_nEvts_b63_HLT_M40 = new TH1F("mnvtx_nEvts_b63_HLT_M40","mnvtx_nEvts_b63_HLT_M40",100,0.,100.);
  TH1F* mnvtx_nEvts_b63_HLT_M70 = new TH1F("mnvtx_nEvts_b63_HLT_M70","mnvtx_nEvts_b63_HLT_M70",100,0.,100.);
  TH1F* mnvtx_nEvts_b63_HLT_M85 = new TH1F("mnvtx_nEvts_b63_HLT_M85","mnvtx_nEvts_b63_HLT_M85",100,0.,100.);
  
  map<string,bool> runmap;
  int previousRunNumber = 0;
  
  TrackPlots               tr_raw_all("raw_all");
  map<string,TrackPlots*>  maptr_raw;
  map<string,VertexPlots*> mapvtx_raw;
  
   cout << "FILE: " << fileManager(0,type,E) << endl;
  //getting the list of files
  //vector<TString>* vfiles = getListOfFiles(fileManager(0,type,E));
  //vector<TString>* vfiles = getListOfFiles("list_test.txt");
  vector<TString>* vfiles = getListOfFiles("filelist_PU.txt");
  
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
  vector<MyVertex>::iterator  goodVtx;

  int i_tot = 0 , nevt_tot = 0;
  //starting Loop over files, stops at end of list of files or when reached nevt_max
  for(vector<TString>::iterator itfiles = vfiles->begin() ; itfiles != vfiles->end() && i_tot < nevt_max ; ++itfiles){

    TFile* file = TFile::Open(*itfiles,"READ");

    //getting the tree form the current file
    tree = (TTree*) file->Get("evt");

    //adding branches to the tree ----------------------------------------------------------------------
    tree->SetBranchAddress("evtId",&evtId);
    if(isMC) tree->SetBranchAddress("GenPart",&genPart);
    if(isMC) tree->SetBranchAddress("GenKin",&genKin);
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
    //tree->SetBranchAddress("pixel3Vertex",&vertexToCut);
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

  
      ostringstream runnumber("");
      runnumber << evtId->Run;
      
      runmap[runnumber.str()] = 1;
      if(previousRunNumber!=evtId->Run)
        cout << "New Run : " <<  evtId->Run << endl;
      previousRunNumber=evtId->Run;
      if(maptr_raw.find(runnumber.str())==maptr_raw.end()){
        maptr_raw[runnumber.str()]  = new TrackPlots("raw_run"+runnumber.str());
        mapvtx_raw[runnumber.str()] = new VertexPlots("raw_run"+runnumber.str());
      }

            //HERE
      vector<MyTracks> trcoll = *tracks;
      if(iTracking==0 || iTracking==2) getPrimaryTracks(trcoll,vertex);
      if(iTracking==1) getPrimaryTracks(trcoll,vertex,bs);

      nEvts_raw->Fill(runnumber.str().c_str(),1);
      mnch_raw->Fill(runnumber.str().c_str(),tracks->size());
      mnch_raw_PV->Fill(runnumber.str().c_str(),trcoll.size());
      mnvtx_raw->Fill(runnumber.str().c_str(),vertex->size());
      
      //MyVertex* goodVtx = GetBestVertex(vertex);
      //get good Vertex
      int bestVertexId = getBestVertex(vertex);
      goodVtx = vertex->end();
      for(vector<MyVertex>::iterator it_vtx = vertex->begin();it_vtx != vertex->end();++it_vtx) {
          if(bestVertexId==it_vtx->id)
              goodVtx = it_vtx;
      } 
      tr_raw_all.fill(*tracks, goodVtx,getBestVertex(vertex),bs);

      //maptr_raw[runnumber.str()].fill(*tracks,*vertex,getBestVertex(vertex),bs);
      maptr_raw[runnumber.str()]->fill(*tracks, goodVtx, getBestVertex(vertex),bs);
      
      if(bestVertexId >= 0) mapvtx_raw[runnumber.str()]->fill(*goodVtx);
      
      if(L1Trig->GetTechDecisionAfter(34))
        nEvts_b34->Fill(runnumber.str().c_str(),1);
	
      if(L1Trig->GetPhysDecisionAfter(124))
        nEvts_b124->Fill(runnumber.str().c_str(),1);
      
      if(L1Trig->GetPhysDecisionAfter(124) && HLTrig->HLTmap["HLT_L1_BscMinBiasOR_BptxPlusORMinus"]){
        nEvts_b124_HLT_MB->Fill(runnumber.str().c_str(),1);
	    mnch_b124_HLT_MB->Fill(runnumber.str().c_str(),tracks->size());
	    mnch_b124_HLT_MB_PV->Fill(runnumber.str().c_str(),trcoll.size());
	    mnvtx_b124_HLT_MB->Fill(runnumber.str().c_str(),vertex->size());
      }
            
      if(HLTrig->HLTmap["HLT_L1_BscMinBiasOR_BptxPlusORMinus"])
        nEvts_HLT_MB->Fill(runnumber.str().c_str(),1);
	
      if(HLTrig->HLTmap["HLT_MinBiasPixel_SingleTrack"])
        nEvts_HLT_ST->Fill(runnumber.str().c_str(),1);
	
      if(HLTrig->HLTmap["HLT_L1_BscMinBiasOR_BptxPlusORMinus"] && HLTrig->HLTmap["HLT_MinBiasPixel_SingleTrack"])
        nEvts_HLT_MB_ST->Fill(runnumber.str().c_str(),1);
  
      if(L1Trig->GetPhysDecisionAfter(63))
        nEvts_b63->Fill(runnumber.str().c_str(),1);
	
      if(L1Trig->GetPhysDecisionAfter(63) && HLTrig->HLTmap["HLT_PixelTracks_Multiplicity40"]){
        nEvts_b63_HLT_M40->Fill(runnumber.str().c_str(),1);
        mnch_nEvts_b63_HLT_M40->Fill(runnumber.str().c_str(),tracks->size());
        mnch_nEvts_b63_HLT_M40_PV->Fill(runnumber.str().c_str(),trcoll.size());
        mnvtx_nEvts_b63_HLT_M40->Fill(runnumber.str().c_str(),vertex->size());
      }
      
      if(L1Trig->GetPhysDecisionAfter(63) && HLTrig->HLTmap["HLT_PixelTracks_Multiplicity70"]){
        nEvts_b63_HLT_M70->Fill(runnumber.str().c_str(),1);
        mnch_nEvts_b63_HLT_M70->Fill(runnumber.str().c_str(),tracks->size());
        mnch_nEvts_b63_HLT_M70_PV->Fill(runnumber.str().c_str(),trcoll.size());
        mnvtx_nEvts_b63_HLT_M70->Fill(runnumber.str().c_str(),vertex->size());
      }
      
      if(L1Trig->GetPhysDecisionAfter(63) && HLTrig->HLTmap["HLT_PixelTracks_Multiplicity85"]){
        nEvts_b63_HLT_M85->Fill(runnumber.str().c_str(),1);
        mnch_nEvts_b63_HLT_M85->Fill(runnumber.str().c_str(),tracks->size());
        mnch_nEvts_b63_HLT_M85_PV->Fill(runnumber.str().c_str(),trcoll.size());
        mnvtx_nEvts_b63_HLT_M85->Fill(runnumber.str().c_str(),vertex->size());
      }
      
 
    }//end of loop over events
    cout << *itfiles << endl;
    //Closing current files
    file->Close();

  }//end of loop over files

  //filling histo with all runs, in case void
  for(map<string,bool>::iterator it = runmap.begin() ; it != runmap.end() ; ++it){
  
    nEvts_raw->Fill(it->first.c_str(),0);                         
    nEvts_b34->Fill(it->first.c_str(),0);                         
    nEvts_b124->Fill(it->first.c_str(),0);               
    nEvts_b124_HLT_MB->Fill(it->first.c_str(),0); 
    nEvts_HLT_MB->Fill(it->first.c_str(),0);                    
    nEvts_HLT_ST->Fill(it->first.c_str(),0);                    
    nEvts_HLT_MB_ST->Fill(it->first.c_str(),0);            
    
    nEvts_b63->Fill(it->first.c_str(),0);                         
    nEvts_b63_HLT_M40->Fill(it->first.c_str(),0);         
    nEvts_b63_HLT_M70->Fill(it->first.c_str(),0);         
    nEvts_b63_HLT_M85->Fill(it->first.c_str(),0);         
    
    
    mnch_raw->Fill(it->first.c_str(),0);                            
    mnch_b124_HLT_MB->Fill(it->first.c_str(),0);    
    mnch_nEvts_b63_HLT_M40->Fill(it->first.c_str(),0);
    mnch_nEvts_b63_HLT_M70->Fill(it->first.c_str(),0);
    mnch_nEvts_b63_HLT_M85->Fill(it->first.c_str(),0);
    
    mnch_raw_PV->Fill(it->first.c_str(),0);                             
    mnch_b124_HLT_MB_PV->Fill(it->first.c_str(),0);     
    mnch_nEvts_b63_HLT_M40_PV->Fill(it->first.c_str(),0); 
    mnch_nEvts_b63_HLT_M70_PV->Fill(it->first.c_str(),0); 
    mnch_nEvts_b63_HLT_M85_PV->Fill(it->first.c_str(),0); 
    
    
    mnvtx_raw->Fill(it->first.c_str(),0);                             
    mnvtx_b124_HLT_MB->Fill(it->first.c_str(),0);     
    mnvtx_nEvts_b63_HLT_M40->Fill(it->first.c_str(),0); 
    mnvtx_nEvts_b63_HLT_M70->Fill(it->first.c_str(),0); 
    mnvtx_nEvts_b63_HLT_M85->Fill(it->first.c_str(),0); 
  
  }

  //deflating and reordering the th1s labels
  
  
  nEvts_raw->LabelsDeflate("X");             
  nEvts_b34->LabelsDeflate("X");             
  nEvts_b124->LabelsDeflate("X");            
  nEvts_b124->LabelsDeflate("X");        
  nEvts_b124_HLT_MB->LabelsDeflate("X"); 
  nEvts_HLT_MB->LabelsDeflate("X");          
  nEvts_HLT_ST->LabelsDeflate("X");          
  nEvts_HLT_MB_ST->LabelsDeflate("X");      
  
  nEvts_b63->LabelsDeflate("X");             
  nEvts_b63_HLT_M40->LabelsDeflate("X");     
  nEvts_b63_HLT_M70->LabelsDeflate("X");     
  nEvts_b63_HLT_M85->LabelsDeflate("X");     
  
  
  mnch_raw->LabelsDeflate("X");              
  mnch_b124_HLT_MB->LabelsDeflate("X");  
  mnch_nEvts_b63_HLT_M40->LabelsDeflate("X");
  mnch_nEvts_b63_HLT_M70->LabelsDeflate("X");
  mnch_nEvts_b63_HLT_M85->LabelsDeflate("X");
  
  mnch_raw_PV->LabelsDeflate("X");               
  mnch_b124_HLT_MB_PV->LabelsDeflate("X");   
  mnch_nEvts_b63_HLT_M40_PV->LabelsDeflate("X"); 
  mnch_nEvts_b63_HLT_M70_PV->LabelsDeflate("X"); 
  mnch_nEvts_b63_HLT_M85_PV->LabelsDeflate("X"); 
  
  
  mnvtx_raw->LabelsDeflate("X");               
  mnvtx_b124_HLT_MB->LabelsDeflate("X");   
  mnvtx_nEvts_b63_HLT_M40->LabelsDeflate("X"); 
  mnvtx_nEvts_b63_HLT_M70->LabelsDeflate("X"); 
  mnvtx_nEvts_b63_HLT_M85->LabelsDeflate("X"); 
  
  
  
  /*nEvts_raw->LabelsOption("<","X");             
  nEvts_b34->LabelsOption("<","X");             
  nEvts_b124->LabelsOption("<","X");        
  nEvts_b124_HLT_MB->LabelsOption("<","X"); 
  nEvts_HLT_MB->LabelsOption("<","X");          
  nEvts_HLT_ST->LabelsOption("<","X");          
  nEvts_HLT_MB_ST->LabelsOption("<","X");      
  
  nEvts_b63->LabelsOption("<","X");             
  nEvts_b63_HLT_M40->LabelsOption("<","X");     
  nEvts_b63_HLT_M70->LabelsOption("<","X");     
  nEvts_b63_HLT_M85->LabelsOption("<","X");     
  
  
  mnch_raw->LabelsOption("<","X");              
  mnch_b124_HLT_MB->LabelsOption("<","X");  
  mnch_nEvts_b63_HLT_M40->LabelsOption("<","X");
  mnch_nEvts_b63_HLT_M70->LabelsOption("<","X");
  mnch_nEvts_b63_HLT_M85->LabelsOption("<","X");
  
  mnch_raw_PV->LabelsOption("<","X");               
  mnch_b124_HLT_MB_PV->LabelsOption("<","X");   
  mnch_nEvts_b63_HLT_M40_PV->LabelsOption("<","X"); 
  mnch_nEvts_b63_HLT_M70_PV->LabelsOption("<","X"); 
  mnch_nEvts_b63_HLT_M85_PV->LabelsOption("<","X"); 
  
  
  mnvtx_raw->LabelsOption("<","X");               
  mnvtx_b124_HLT_MB->LabelsOption("<","X");   
  mnvtx_nEvts_b63_HLT_M40->LabelsOption("<","X"); 
  mnvtx_nEvts_b63_HLT_M70->LabelsOption("<","X"); 
  mnvtx_nEvts_b63_HLT_M85->LabelsOption("<","X"); */
  
  
  
  ostringstream strout("");
  strout<<"TriggerTest_type"<<type<<"_"<<E<<"TeV_";
  if(iTracking==0)
    strout<<"gTr";
  if(iTracking==1)
    strout<<"mbTr";
  strout<<".root";
  cout << "Output File : " << strout.str().c_str() << endl;
  TFile* output = new TFile(strout.str().c_str(),"RECREATE");
  output->cd();




  mnch_raw->Divide(nEvts_raw);              
  mnch_b124_HLT_MB->Divide(nEvts_b124_HLT_MB);  
  mnch_nEvts_b63_HLT_M40->Divide(nEvts_b63_HLT_M40);
  mnch_nEvts_b63_HLT_M70->Divide(nEvts_b63_HLT_M70);
  mnch_nEvts_b63_HLT_M85->Divide(nEvts_b63_HLT_M85);
  
  mnch_raw_PV->Divide(nEvts_raw);              
  mnch_b124_HLT_MB_PV->Divide(nEvts_b124_HLT_MB);  
  mnch_nEvts_b63_HLT_M40_PV->Divide(nEvts_b63_HLT_M40);
  mnch_nEvts_b63_HLT_M70_PV->Divide(nEvts_b63_HLT_M70);
  mnch_nEvts_b63_HLT_M85_PV->Divide(nEvts_b63_HLT_M85);
  
  mnvtx_raw->Divide(nEvts_raw);              
  mnvtx_b124_HLT_MB->Divide(nEvts_b124_HLT_MB);  
  mnvtx_nEvts_b63_HLT_M40->Divide(nEvts_b63_HLT_M40);
  mnvtx_nEvts_b63_HLT_M70->Divide(nEvts_b63_HLT_M70);
  mnvtx_nEvts_b63_HLT_M85->Divide(nEvts_b63_HLT_M85);

  nEvts_raw->Write();             
  nEvts_b34->Write();             
  nEvts_b124->Write();       
  nEvts_b124_HLT_MB->Write(); 
  nEvts_HLT_MB->Write();          
  nEvts_HLT_ST->Write();          
  nEvts_HLT_MB_ST->Write();      
  
  nEvts_b63->Write();             
  nEvts_b63_HLT_M40->Write();     
  nEvts_b63_HLT_M70->Write();     
  nEvts_b63_HLT_M85->Write();     
  
  
  mnch_raw->Write();              
  mnch_b124_HLT_MB->Write();  
  mnch_nEvts_b63_HLT_M40->Write();
  mnch_nEvts_b63_HLT_M70->Write();
  mnch_nEvts_b63_HLT_M85->Write();
  
  mnch_raw_PV->Write();               
  mnch_b124_HLT_MB_PV->Write();   
  mnch_nEvts_b63_HLT_M40_PV->Write(); 
  mnch_nEvts_b63_HLT_M70_PV->Write(); 
  mnch_nEvts_b63_HLT_M85_PV->Write(); 
  
  
  mnvtx_raw->Write();               
  mnvtx_b124_HLT_MB->Write();   
  mnvtx_nEvts_b63_HLT_M40->Write(); 
  mnvtx_nEvts_b63_HLT_M70->Write(); 
  mnvtx_nEvts_b63_HLT_M85->Write(); 
  
  
               
  TH1F* eff_b34             = (TH1F*) nEvts_b34->Clone("eff_b34");             
  TH1F* eff_b124            = (TH1F*) nEvts_b124->Clone("eff_b124");       
  TH1F* eff_b124_HLT_MB = (TH1F*) nEvts_b124_HLT_MB->Clone("eff_b124_HLT_MB"); 
  TH1F* eff_HLT_MB          = (TH1F*) nEvts_HLT_MB->Clone("eff_HLT_MB");          
  TH1F* eff_HLT_ST          = (TH1F*) nEvts_HLT_ST->Clone("eff_HLT_ST");          
  TH1F* eff_HLT_MB_ST       = (TH1F*) nEvts_HLT_MB_ST->Clone("eff_HLT_MB_ST");      
  
  TH1F* eff_b63         = (TH1F*) nEvts_b63->Clone("eff_b63");             
  TH1F* eff_b63_HLT_M40 = (TH1F*) nEvts_b63_HLT_M40->Clone("eff_b63_HLT_M40");     
  TH1F* eff_b63_HLT_M70 = (TH1F*) nEvts_b63_HLT_M70->Clone("eff_b63_HLT_M70");     
  TH1F* eff_b63_HLT_M85 = (TH1F*) nEvts_b63_HLT_M85->Clone("eff_b63_HLT_M85"); 
  
            
  eff_b34->Divide(nEvts_raw);             
  eff_b124->Divide(nEvts_raw);        
  eff_b124_HLT_MB->Divide(nEvts_raw); 
  eff_HLT_MB->Divide(nEvts_raw);          
  eff_HLT_ST->Divide(nEvts_raw);          
  eff_HLT_MB_ST->Divide(nEvts_raw);      
  
  eff_b63->Divide(nEvts_raw);             
  eff_b63_HLT_M40->Divide(nEvts_raw);     
  eff_b63_HLT_M70->Divide(nEvts_raw);     
  eff_b63_HLT_M85->Divide(nEvts_raw);  
  
  eff_b34->Write();             
  eff_b124->Write();       
  eff_b124_HLT_MB->Write(); 
  eff_HLT_MB->Write();          
  eff_HLT_ST->Write();          
  eff_HLT_MB_ST->Write();      
  
  eff_b63->Write();             
  eff_b63_HLT_M40->Write();     
  eff_b63_HLT_M70->Write();     
  eff_b63_HLT_M85->Write();  
  

  tr_raw_all.write();
  for(map<string,TrackPlots*>::iterator it=maptr_raw.begin() ; it != maptr_raw.end() ; ++it)
    it->second->write();
  for(map<string,VertexPlots*>::iterator it=mapvtx_raw.begin() ; it != mapvtx_raw.end() ; ++it)
    it->second->write();
  
  
  output->Close();
}
