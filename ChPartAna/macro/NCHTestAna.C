#include <TROOT.h>
#include <TH1.h>
#include <TH2.h>
#include <TProfile.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TTree.h>
#include <TFile.h>
#include <TChain.h>
#include <TDirectory.h>
#include <iostream>
#include <iomanip>
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

#include "../plugins/TResponseMtx.h"
#include "../plugins/BasePlots.h"
#include "../plugins/MultiPlots.h"


#include "../plugins/NCHEvtSelPlots.h"
#include "../plugins/NCHHFPlots.h"
#include "../plugins/NCHCentralPlots.h"
#include "../plugins/NCHDiffPlots.h"

bool debug=false;
#include "fileManager.C"
#include "NCHCuts.C"
#include "NCHEvtSel.C"
//#include "binningMap.C"

#include "NCHacceptanceMap.C"

 /////////////////////////
  //TODO LIST             //
  ///////////////////////// 
  //1) bit 124 or 63 for single pixel single track ?   
  //2) rule 227 passsCentral only use PT_RECO HERE?????
  //3) fix diffraction numbers
  ////////////////////////
  //run and close
  //   nohup root BuildLibDico.C+ NCHTestAna.C+"(60,7,1,1000000)" -q   > logMC60.txt & 
  //   nohup root BuildLibDico.C+ NCHTestAna.C+"(15,7,1,1000000)" -q   > logMC15.txt &
  //   nohup root BuildLibDico.C+ NCHTestAna.C+"(0,7,1,-1)" -q   > logData.txt & 
  //   tail -f logData.txt
  /////////////////////////
 
//_____________________________________________________________________________
TString st(string input , int cut){
  stringstream nb("");  
  nb<<cut;
  return input+"cut"+nb.str();
}


//_____________________________________________________________________________
void NCHTestAna(int type = 60 , double E = 7. , int iTracking = 1, int nevt_max = 1000){
 
  ////////////////////////////////////////////////
  //SWTICH for most of the intermediate plots:
  NCHCentralPlots::switchMBUEWGonly=0;        // 0 = only nocut and MBUEWG 
  NCHHFPlots::switchAllHF=0;                  // 0 = only HF0 and HF1
  NCHEvtSelPlots::switchIntermedPlots=1;      // 0 = only noSel and FullSel
  ////////////////////////////////////////////
  
  //EVENT SELECTION TYPE, see NCHEvtSel.C for more information 
  bool isMC = true; 
  if(type==0 || type==100) isMC = false; //100 is a fast testfile which is not on dcap
  NCHDiffPlots::isMC=isMC;
  
  TString MCtype = "pythia";
  if(type == 20) MCtype = "phojet";
  //if(type == 15) MCtype = "z2";
  if(type == 60 || type==101) MCtype = "pythia8";
  if(  !(iTracking==1 || iTracking==0))  {
     cout <<"WRONG TRACKING NUMBER !!!!"<<endl; 
     return;
  } 
  TString tracking="";   iTracking ? tracking="_ferncTr" : tracking="_genTr";
  
  
  // BUILDING CHAIN
  //adding files to the tree chain
  TH1::AddDirectory(0); 
  cout << "FILE: " << fileManager(0,type,E) << endl;
  TChain* tree = new TChain("evt","");
  tree->Add(fileManager(0,type,E));
  
  
  //adding branches to the tree 
  MyEvtId*           evtId     = NULL;
  MyL1Trig*          L1Trig    = NULL;
  MyHLTrig*          HLTrig    = NULL;
  MyMITEvtSel*       MITEvtSel = NULL;
  MyBeamSpot*        bs        = NULL;
  MyGenKin*          genKin    = NULL;
  vector<MyGenPart>* genPart   = NULL;
  vector<MyTracks>*  tracks  = NULL;
  vector<MyVertex>*  vertex  = NULL;
  int                bestVertexId = 0;
  vector<MyVertex>::iterator goodVtx;
  //vector<MyVertex>*  vertexToCut  = NULL;
  vector<MyTracks>   goodtracks;
  vector<MyGenPart>  goodgenpart;
    
  tree->SetBranchAddress("EvtId",&evtId);
  tree->SetBranchAddress("L1Trig",&L1Trig);
  tree->SetBranchAddress("HLTrig",&HLTrig);
  tree->SetBranchAddress("MITEvtSel",&MITEvtSel);
  tree->SetBranchAddress("beamSpot",&bs);   
  if(isMC) {
    tree->SetBranchAddress("GenPart",&genPart);
    tree->SetBranchAddress("GenKin",&genKin);
  }  
  if(iTracking==0){
    tree->SetBranchAddress("generalTracks",&tracks); 
    tree->SetBranchAddress("primaryVertex",&vertex);
  }
  else if(iTracking==1){
    tree->SetBranchAddress("minbiasTracks",&tracks); 
    tree->SetBranchAddress("ferencVtxFerTrk",&vertex);
  }
  //tree->SetBranchAddress("ferencVtxFerTrk",&vertexToCut);
  //tree->SetBranchAddress("pixel3Vertex",&vertexToCut);

 
  // GET NUMBER OF EVENTS
  int nev = int(tree->GetEntries());   
  int Nrun=min(nev,nevt_max);  
  if (nevt_max==-1) Nrun=nev;  //if -1 --> run on all
  
  //get names and print
  stringstream mctype("");     mctype << type ;
  stringstream energy("");     energy << "_E_" << E ;
  stringstream nEvts("");      nEvts << "_" << Nrun ;
  TString dat="";          isMC ? dat="_MC"+mctype.str() : dat="_data";
  cout <<"-------------------------------------"<<endl;
  cout <<"Number of Entries: "<< nev << endl;  
  cout <<"Running on: " << Nrun     << " events"   << endl;
  cout <<"E:             " << E        << " TeV"      << endl;
  cout <<"Tracking:      " << tracking << " number: " <<  iTracking << endl;
  cout <<"Data:          " << dat      << "      isMC: "   <<  isMC      << endl;
  cout <<"MBUEWGonly:    " << NCHCentralPlots::switchMBUEWGonly << endl;
  cout <<"AllHF:         " << NCHHFPlots::switchAllHF << endl;
  cout <<"IntermedPlots: " << NCHEvtSelPlots::switchIntermedPlots <<endl;
  
    
  //Initialize AcceptanceMap
  InitializeAcceptanceMap();
  cout << "acceptanceMap: " << accMap->size() <<" loaded "  <<endl; 
  cout <<"-------------------------------------"<<endl;
  
  //Initialising Plots tree
  if (debug) cout << "Initialising all Plots" <<endl;   
  vector<NCHDiffPlots*> nchDiffPlots (accMap->size(),0);  
  //temp, before dynamical binning, see above
  BasePlots*       baseplot    = new BasePlots("BasePlots");  
  for(int acc = 0 ; acc < (int)accMap->size() ; ++acc){
    //BINING 
    //vector< vector<double> > binning;
    //binning = getBins(1,0,1);//nch,pt,eta
    //binning = getBins(acc,Ebinning);//nch,pt,eta
    //BasePlots initialisation needed to get binning_array of MultiPlots right for nch etc
    //baseplot->setBinning(binning);     
    nchDiffPlots.at(acc)  = new NCHDiffPlots(st("",acc));
  }    
  
  
  ////////////////////////////
  /////    LOOP     //////////
  ////////////////////////////  
  if (debug) cout << "Starting the loop over the events" <<endl;
  for(int i = 0; i < nev; i++){     
    if( ((i+1) % 10000) == 0) cout <<int(double(i+1)/double(Nrun)*100.)<<" % done"<<endl;    
    if(i==Nrun) break;
    //vertexToCut=&vertex;
    tree->GetEntry(i);
    
    
    /* Test to compare with old FileManager
    if(evtId->Run!=132440     ||
       evtId->LumiSect < 158  ||
       evtId->LumiSect > 374  ) 
    { 
        continue;
    }
    */
 
    //get good Vertex
    bestVertexId = getBestVertex(vertex);
    goodVtx = vertex->end();
    for(vector<MyVertex>::iterator it_vtx = vertex->begin();it_vtx != vertex->end();++it_vtx) {
        if(bestVertexId==it_vtx->id)
            goodVtx = it_vtx;
    } 

    //Looping AcceptanceMap
    for(int acc = 0 ; acc < (int) accMap->size() ; ++acc){
        
        //GET GOOD PARTICLE AND TRACK COLLECTION
        
        if(isMC) goodgenpart=getPrimaryGenPart(*genPart,accMap->at(acc).ptGen, accMap->at(acc).etaGenMin, 
                                                            accMap->at(acc).etaGenMax,accMap->at(acc).charge);  
        if(iTracking==0) goodtracks=getPrimaryTracks(*tracks, vertex, accMap->at(acc).ptReco, accMap->at(acc).etaRecoMin, 
                                                            accMap->at(acc).etaRecoMax,accMap->at(acc).charge);     //general Tracks
        if(iTracking==1) goodtracks=getPrimaryTracks(*tracks, vertex, bs, accMap->at(acc).ptReco, accMap->at(acc).etaRecoMin, 
                                                            accMap->at(acc).etaRecoMax,accMap->at(acc).charge); // ferenc Tracks  
                   
        //RESETTING EVENT SELECTION  
        NCHDiffPlots::isSD=0;  NCHDiffPlots::isDD=0;          
        NCHCentralPlots::passMBUEWGreco=0;   NCHCentralPlots::passMBUEWGpart=0;         
        NCHCentralPlots::passALICEpart =0;   NCHCentralPlots::passALICEreco =0; 
        NCHCentralPlots::passATLAS1part=0;   NCHCentralPlots::passATLAS1reco=0;
        NCHCentralPlots::passATLAS6part=0;   NCHCentralPlots::passATLAS6reco=0;        
        NCHHFPlots::passHF1=0;     //passHF2=!passHF1
        NCHHFPlots::passHF3=0;     NCHHFPlots::passHF4=0;
        NCHHFPlots::passHF5=0;     NCHHFPlots::passHF6=0;                    
        NCHEvtSelPlots::passL1 =0;    NCHEvtSelPlots::passVtxQual=0;
        NCHEvtSelPlots::passVtx=0;    NCHEvtSelPlots::passBit40=0;                    
         

        //SETTING THE CUT BOOLEANS for this event                   
        if(isMC) {        
            if(isSD(*genKin,MCtype)) NCHDiffPlots::isSD=1;
            if(isDD(*genKin,MCtype)) NCHDiffPlots::isDD=1;
            if(passCentral(goodgenpart,"MBUEWG",accMap->at(acc).ptReco)) NCHCentralPlots::passMBUEWGpart=1;
            if(passCentral(goodgenpart,"ALICE" ,accMap->at(acc).ptReco)) NCHCentralPlots::passALICEpart =1;   //Not using PT_PART HERE !!!!!!
            if(passCentral(goodgenpart,"ATLAS1",accMap->at(acc).ptReco)) NCHCentralPlots::passATLAS1part=1;
            if(passCentral(goodgenpart,"ATLAS6",accMap->at(acc).ptReco)) NCHCentralPlots::passATLAS6part=1;
        }        
        if(passCentral(goodtracks,"MBUEWG",accMap->at(acc).ptReco)) NCHCentralPlots::passMBUEWGreco=1;                
        if(passCentral(goodtracks,"ALICE" ,accMap->at(acc).ptReco)) NCHCentralPlots::passALICEreco =1; 
        if(passCentral(goodtracks,"ATLAS1",accMap->at(acc).ptReco)) NCHCentralPlots::passATLAS1reco=1; 
        if(passCentral(goodtracks,"ATLAS6",accMap->at(acc).ptReco)) NCHCentralPlots::passATLAS6reco=1;
        
        if(passHF(*MITEvtSel,1)) NCHHFPlots::passHF1=1;
        //passHF2 = !passHF1;         
        if(passHF(*MITEvtSel,3)) NCHHFPlots::passHF3=1;
        if(passHF(*MITEvtSel,4)) NCHHFPlots::passHF4=1;
        if(passHF(*MITEvtSel,5)) NCHHFPlots::passHF5=1;
        if(passHF(*MITEvtSel,6)) NCHHFPlots::passHF6=1;
     
        //if(passL1(E, *L1Trig, isMC)) NCHEvtSelPlots::passL1=1;      
        if(passL1HLT(E, *L1Trig, *HLTrig , 1,isMC ))  NCHEvtSelPlots::passL1=1;
        if(passVtxQual(*MITEvtSel,E)) NCHEvtSelPlots::passVtxQual=1;     
        if(passVtx(*vertex)) NCHEvtSelPlots::passVtx=1;     
        if(passBit40(*L1Trig)) NCHEvtSelPlots::passBit40=1;
     
        //filling
        nchDiffPlots.at(acc)->fill(goodgenpart,*genKin,MCtype,goodtracks,*vertex,goodVtx,bestVertexId,bs, 1.);
        
    }//end of loop over acceptanceMap
                 
  }//end of loop over events
   
  //OUTPUT  
  TFile* output = new TFile("output"+dat+tracking+energy.str()+nEvts.str()+".root","RECREATE");  
  output->cd();
  cout << "Writing to the file: " << endl <<"output"+dat+tracking+energy.str()+nEvts.str()+".root" <<endl;
  for(int acc = 0 ; acc < (int)accMap->size() ; ++acc){
    nchDiffPlots.at(acc)->write();
    
    delete nchDiffPlots.at(acc);
  }
  delete baseplot;
     
  output->Close();
  delete output;
  cout  << "-------------------------------------" <<endl;         
}

