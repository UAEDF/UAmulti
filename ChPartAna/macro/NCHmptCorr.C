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
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
using namespace std;

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

#include "../plugins/NCHptvsnchPlots.h"

bool debug=false;
#include "files_funcs.C"
#include "fileManager.C"
#include "NCHCuts.C"
#include "NCHEvtSel.C"
#include "NCHbinningMap.C"

#include "NCHacceptanceMap.C" 
 
 // root BuildLibDico.C+ NCHmptCorr.C+'(0, 60, 7, 2, "v52", 10000)' -q
 
//typedef map <TString, bool> MapType; in "../plugins/NCHptvsnchPlots.h"
void SetMap( MapType*, TString, bool);

//_____________________________________________________________________________
TString st(string input , int cut){
  stringstream nb("");  
  nb<<cut;
  return input+"cut"+nb.str();
}

//_____________________________________________________________________________
void NCHmptCorr(int type = 0 , int type_unf = 60, double E = 7. , int iTracking = 1, TString unfoldDir = "v52" , int nevt_max = 10000){
  
  unfoldDir="../macro/unfold_outputs/"+unfoldDir;
  TH1::AddDirectory(0); //Detach histograms from rootfiles (they are no closed if the file is closed.)

  vector<TString>* cutSel   = new vector<TString>;
  vector<TString>* centrSel = new vector<TString>;
  vector<TString>* hfSel    = new vector<TString>;
  vector<NCHptvsnchPlots*>* vec_mpt = new vector<NCHptvsnchPlots*>();
  MapType* passMap = new MapType;
  MapType::iterator map_it;
  
  // What do you want to run :
  cutSel->push_back("cut0");
  cutSel->push_back("cut1");
  cutSel->push_back("cut2");
  cutSel->push_back("cut3");
  cutSel->push_back("cut4");
  cutSel->push_back("cut5");
  
  centrSel->push_back("nocut");     
  centrSel->push_back("MBUEWG");      
  centrSel->push_back("ALICE");  
  centrSel->push_back("ATLAS1"); 
  centrSel->push_back("ATLAS2");
  centrSel->push_back("ATLAS6");
  
  hfSel->push_back("HF0");
  hfSel->push_back("HF1");
  
  stringstream mc_s("");
  mc_s << "MC" << type_unf ;
  int jj=0;
  for(vector<TString>::iterator it_cut = cutSel->begin() ; it_cut != cutSel->end() ; ++it_cut){
    for(vector<TString>::iterator it_centr = centrSel->begin() ; it_centr != centrSel->end() ; ++it_centr){
       for(vector<TString>::iterator it_hf = hfSel->begin() ; it_hf != hfSel->end() ; ++it_hf){ 
         
         NCHptvsnchPlots* ptvsnch = new NCHptvsnchPlots(mc_s.str(), *it_cut, *it_centr, *it_hf, unfoldDir);
         vec_mpt->push_back(ptvsnch);
         jj++;
       }
    } 
  }
  delete cutSel;
  delete centrSel;
  delete hfSel;
  cout <<endl << "Selections created: " << jj << endl;
 
  //EVENT SELECTION TYPE, see NCHEvtSel.C for more information 
  bool isMC = true; 
  if(type < 6 ) isMC = false;
  
  TString MCtype = "pythia";
  if(type == 20) MCtype = "phojet";
  //if(type == 15) MCtype = "z2";
  if(type == 60 || type == 62) MCtype = "pythia8";
  if(  !(iTracking==2 ||iTracking==1 || iTracking==0))  {
     cout <<"WRONG TRACKING NUMBER !!!!"<<endl; 
     return;
  } 
  TString tracking="_genTr";
  if (iTracking==1) tracking="_ferncTr";
  if (iTracking==2) tracking="_mixedTr";
  
  // BUILDING CHAIN
  //adding files to the tree chain
  cout << "FILE: " << fileManager(0,type,E) << endl;
  vector<TString>* vfiles;
  //getting the list of files
  //vector<TString>* vfiles = getListOfFiles(fileManager(0,type,E));
  // RIGHT ONE
  //vector<TString>* vfiles = getListOfFiles("../filelist.txt");
  // FOR TESTING
  if(type==0)  vfiles = getListOfFiles("filelist_PU.txt");
  if(type==62) vfiles = getListOfFiles("filelistMC62part.txt");  

  //Declaration of tree and its branches variables
  TTree* tree = new TTree("evt","");
  MyEvtId*           evtId     = NULL;
  MyL1Trig*          L1Trig    = NULL;
  MyHLTrig*          HLTrig    = NULL;
  MyMITEvtSel*       MITEvtSel = NULL;
  MyBeamSpot*        bs        = NULL;
  MyGenKin*          genKin    = NULL;
  vector<MyGenPart>* genPart   = NULL;
  vector<MyTracks>*  tracks    = NULL;
  vector<MyVertex>*  vertex    = NULL;
  int                bestVertexId = 0;
  vector<MyVertex>::iterator  goodVtx;
  vector<MyTracks>   goodtracks;
  vector<MyGenPart>  goodgenpart;
  double weight =1.;
  
  //Initialize AcceptanceMap___________________________________________________________
  InitializeAcceptanceMap();
  cout <<"-------------------------------------"<<endl;
  cout << "acceptanceMap: " << accMap->size() <<" loaded "  <<endl; 
  
  
    //get names and print
    stringstream mctype("");     mctype << type ;
    stringstream energy("");     energy << "_E_" << E ;
    TString dat="";          isMC ? dat="_MC"+mctype.str() : dat="_data";
    cout <<"-------------------------------------"<<endl;
    cout <<"Running on max: " << nevt_max     << " events"   << endl;
    cout <<"E:              " << E        << " TeV"      << endl;
    cout <<"Tracking:       " << tracking << " number: " <<  iTracking << endl;
    cout <<"Data:           " << dat      << "      isMC: "   <<  isMC      << endl;

   if (nevt_max == -1) nevt_max=100000000;
   int i_tot = 0 , nevt_tot = 0;
   //starting Loop over files, stops at end of list of files or when reached nevt_max
   for(vector<TString>::iterator itfiles = vfiles->begin() ; itfiles != vfiles->end() && i_tot < nevt_max ; ++itfiles){

    TFile* file = TFile::Open(*itfiles,"READ");
    //getting the tree form the current file
    tree = (TTree*) file->Get("evt");
    //adding branches to the tree ----------------------------------------------------------------------
    tree->SetBranchAddress("evtId",&evtId);
    tree->SetBranchAddress("L1Trig",&L1Trig);
    if(! ( type==31 ) ) 
        tree->SetBranchAddress("HLTrig",&HLTrig);
    tree->SetBranchAddress("MITEvtSel",&MITEvtSel);
    if(E != 2.76) tree->SetBranchAddress("offlineBeamSpot",&bs);   //2.76TeV has no beamspot yet
    if(isMC) {
        tree->SetBranchAddress("genPart",&genPart);
        tree->SetBranchAddress("genKin",&genKin);
    }
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
     
    // GET NUMBER OF EVENTS
    //Getting number of events
    int nev = int(tree->GetEntriesFast());
    nevt_tot += nev;
    cout <<"The current file has " << nev << " entries : "<< endl << *itfiles << endl;
   
    ////////////////////////////
    /////    LOOP     //////////
    ////////////////////////////
    //starting loop over events, stops when reached end of file or nevt_max
    if (debug) cout << "Starting the loop over the events" <<endl;
    double mpt=0;
    int goodtracks_size=0;
    
    for(int i = 0; i < nev && i_tot < nevt_max; ++i){      
        if( ((i+1) % 1000) == 0) cout <<int(double(i_tot+1)/1000)<<" k done"<<endl;    

        //Filling the variables defined setting branches
        tree->GetEntry(i);
        i_tot++; 
        
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
            if(iTracking==0 || iTracking==2) goodtracks=getPrimaryTracks(*tracks, vertex, accMap->at(acc).ptReco, accMap->at(acc).etaRecoMin, 
                                                            accMap->at(acc).etaRecoMax,accMap->at(acc).charge);     //general Tracks or mixed Tracks
            if(iTracking==1) goodtracks=getPrimaryTracks(*tracks, vertex, bs, accMap->at(acc).ptReco, accMap->at(acc).etaRecoMin, 
                                                            accMap->at(acc).etaRecoMax,accMap->at(acc).charge); // ferenc Tracks      
           goodtracks_size=goodtracks.size();
           //cout <<"loop before map"<< endl; 
           
           (*passMap)["MC"]=isMC;
           (*passMap)["SD"]=0;
           (*passMap)["MBUEWGGen"]=0;       
           (*passMap)["ALICEGen"]=0;   
           (*passMap)["ATLAS1Gen"]=0;
           (*passMap)["ATLAS2Gen"]=0;   
           (*passMap)["ATLAS6Gen"]=0;              
           
           (*passMap)["MBUEWGRECO"]=0;       
           (*passMap)["ALICERECO"]=0;   
           (*passMap)["ATLAS1RECO"]=0;
           (*passMap)["ATLAS2RECO"]=0;   
           (*passMap)["ATLAS6RECO"]=0;    
           (*passMap)["HF0"]=1; // you always pass HF0, its HF cut off     
           (*passMap)["HF1"]=0;
                   
           (*passMap)["L1"] =0;    
           (*passMap)["VtxQual"]=0;
           (*passMap)["Vtx"]=0;   
                            
           //cout << "KEY CHECK : " << passMap->find("ALICERECO")->second << endl;
                  
           if(isMC) {        
              if(isSD(*genKin,MCtype))  SetMap(passMap,"SD" , 1);
              if(passCentral(goodgenpart,"MBUEWG",accMap->at(acc).ptReco))  SetMap(passMap,"MBUEWGGen" , 1);
              if(passCentral(goodgenpart,"ALICE" ,accMap->at(acc).ptReco))  SetMap(passMap,"ALICEGen"  , 1);
              if(passCentral(goodgenpart,"ATLAS1",accMap->at(acc).ptReco))  SetMap(passMap,"ATLAS1Gen" , 1);
              if(passCentral(goodgenpart,"ATLAS2",accMap->at(acc).ptReco))  SetMap(passMap,"ATLAS2Gen" , 1);
              if(passCentral(goodgenpart,"ATLAS6",accMap->at(acc).ptReco))  SetMap(passMap,"ATLAS6Gen" , 1);          
          } 
     
          if(passCentral(goodtracks ,"MBUEWG",accMap->at(acc).ptReco))  SetMap(passMap,"MBUEWGRECO" , 1);               
          if(passCentral(goodtracks ,"ALICE" ,accMap->at(acc).ptReco))  SetMap(passMap,"ALICERECO"  , 1);
          if(passCentral(goodtracks ,"ATLAS1",accMap->at(acc).ptReco))  SetMap(passMap,"ATLAS1RECO" , 1);
          if(passCentral(goodtracks ,"ATLAS2",accMap->at(acc).ptReco))  SetMap(passMap,"ATLAS2RECO" , 1);
          if(passCentral(goodtracks ,"ATLAS6",accMap->at(acc).ptReco))  SetMap(passMap,"ATLAS6RECO" , 1);
            
          //cout << "KEY CHECK : " << passMap->find("ALICERECO")->second << endl;
          if(passHF(*MITEvtSel,1)) SetMap(passMap,"HF1" , 1);                                       
     
          if(type==31 || type ==5) {
            if(passL1(E, *L1Trig, isMC)) SetMap(passMap,"L1" , 1);
          }        
          else {
            if(passL1HLT(E, *L1Trig, *HLTrig , 1, isMC ))  
                SetMap(passMap,"L1" , 1);
          }      
          if(passVtxQual(*MITEvtSel,E)) SetMap(passMap,"VtxQual", 1);     
          if(passVtx(*vertex)) SetMap(passMap,"Vtx" , 1);        
          
          
        
          //filling
          mpt=0;
          for(vector<MyTracks>::iterator itr = goodtracks.begin() ; itr != goodtracks.end() ; ++itr){
              mpt+=itr->Pt();
          }
          mpt=mpt/goodtracks_size;
          if(goodtracks_size==0) mpt=0;
          
          stringstream acc_str("");  acc_str << "cut"<< acc; 
          for(int veci = 0 ; veci < (int) vec_mpt->size() ; ++veci){
             if( (vec_mpt->at(veci)->getCut() ).Contains(acc_str.str()) ){ //only fill the ones with the right cut
                vec_mpt->at(veci)->fill(passMap, mpt, goodtracks_size);
             }
           }
           //clearing the map
           passMap->clear();
           mpt=0;

        }//end of loop over acceptanceMap
                 
    }//end of loop over events
   
    //Closing current files
    file->Close();
   }//end of loop over files
   
   //OUTPUT  
   stringstream nEvts("");      nEvts << "_" << i_tot ;
     
   TString out_str = "mptCorr"+dat+"_unf"+mc_s.str()+tracking+energy.str()+nEvts.str()+".root";
   TFile* output = new TFile(out_str,"RECREATE");  
   output->cd();
   cout << "Writing to the file: " << endl << out_str << endl; 
   
   for(int theone = 0 ; theone < (int)vec_mpt->size() ; ++theone){
     vec_mpt->at(theone)->write();
     delete vec_mpt->at(theone);
   }    
   delete vec_mpt;

  output->Close();
  delete output;
  cout  << "-------------------------------------" << endl;         
}

//_____________________________________________________________________________
void SetMap( MapType* map, TString key, bool value) {
    MapType::iterator it_m;
    it_m=map->find(key); 
    map->erase(it_m);
    (*map)[key]=value;             
}                 
