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
using namespace std;

#include "../plugins/MyEvtId.h"
#include "../plugins/MyL1Trig.h"
#include "../plugins/MyHLTrig.h"
#include "../plugins/MyGenKin.h"
#include "../plugins/MyPart.h"
#include "../plugins/MyGenPart.h"
#include "../plugins/MyMITEvtSel.h"
#include "../plugins/MyBeamSpot.h"

#include "../plugins/TResponseMtx.h"
#include "../plugins/BasePlots.h"
#include "../plugins/MultiPlots.h"
#include "../plugins/GenPartPlots.h"

#include "../plugins/NCHEvtSelPlots.h"
#include "../plugins/NCHHFPlots.h"
#include "../plugins/NCHCentralPlots.h"
#include "../plugins/NCHDiffPlots.h"

bool debug=false;
#include "fileManager.C"
#include "NCHCuts.C"
#include "NCHEvtSel.C"
#include "NCHbinningMap.C"

#include "NCHacceptanceMap.C"

  ////////////////////////
  //run and close
  //   nohup root -l BuildLibDico.C+ NCHGenOnlyAna.C+"(61,7,2000000)" -q   > logMC61_7_2M.txt &
  //   nohup root -l BuildLibDico.C+ NCHGenOnlyAna.C+"(62,7,2000000)" -q   > logMC62_7_2M.txt & 
  //   nohup root -l BuildLibDico.C+ NCHGenOnlyAna.C+"(61,0.9,2000000)" -q   > logMC61_9_2M.txt & 
  //   nohup root -l BuildLibDico.C+ NCHGenOnlyAna.C+"(62,0.9,2000000)" -q   > logMC62_9_2M.txt &
  /////////////////////////
  //HF is using towers, we only have particles --> use !isSD  == NSD  to get the right particles for HF1
 
//_____________________________________________________________________________
TString st(string input , int cut){
  stringstream nb("");  
  nb<<cut;
  return input+"cut"+nb.str();
}

//_____________________________________________________________________________
void divideByWidth(TH1F* hist){
  for(int i=1;i<=hist->GetNbinsX();++i){
    hist->SetBinContent(i,double(hist->GetBinContent(i))/double(hist->GetBinWidth(i)));
    hist->SetBinError(i,double(hist->GetBinError(i))/double(hist->GetBinWidth(i)));
  }
}


//_____________________________________________________________________________
void NCHGenOnlyAna(int type = 60 , double E = 7. , int nevt_max = 1000){
  
  bool isMC = true; 
  if(type < 6 ) { 
    cout << "type must be not data and bigger than 6, type: " << type << endl;
    return;
  }  
  
  TString MCtype = "pythia";
  if(type == 20) MCtype = "phojet";
  //if(type == 15) MCtype = "z2";
  if(type == 60 || type ==61 || type ==62) MCtype = "pythia8";
  
  // BUILDING CHAIN
  //adding files to the tree chain
  TH1::AddDirectory(0); //Detach histograms from rootfiles (they are no closed if the file is closed.)
  cout << "FILE: " << fileManager(0,type,E) << endl;
  //getting the list of files
  vector<TString>* vfiles = getListOfFiles(fileManager(0,type,E));
  //vector<TString>* vfiles = getListOfFiles("../filelist.txt");
  
  //Declaration of tree and its branches variables
  TTree* tree = new TTree("evt","");
  MyGenKin*          genKin    = NULL;
  vector<MyGenPart>* genPart   = NULL;
  vector<MyGenPart>  goodgenpart;
  double weight =1.;
  
  //Initialize AcceptanceMap___________________________________________________________
  InitializeAcceptanceMap();
  cout <<"-------------------------------------"<<endl;
  cout << "acceptanceMap: " << accMap->size() <<" loaded "  <<endl; 
  
  
  //Initialising Plots tree
  if (debug) cout << "Initialising all Plots" <<endl;   
  //temp, before dynamical binning, see above
  BasePlots*       baseplot    = new BasePlots("BasePlots");
  vector<GenPartPlots*> GenPart_HF0_nocut  (accMap->size(),0); 
  vector<GenPartPlots*> GenPart_HF0_MBUEWG (accMap->size(),0); 
  vector<GenPartPlots*> GenPart_HF0_ALICE  (accMap->size(),0); 
  vector<GenPartPlots*> GenPart_HF0_ATLAS1 (accMap->size(),0); 
  vector<GenPartPlots*> GenPart_HF0_ATLAS2 (accMap->size(),0); 
  vector<GenPartPlots*> GenPart_HF0_ATLAS6 (accMap->size(),0); 
  
  vector<GenPartPlots*> GenPart_HF1_nocut  (accMap->size(),0); 
  vector<GenPartPlots*> GenPart_HF1_MBUEWG (accMap->size(),0); 
  vector<GenPartPlots*> GenPart_HF1_ALICE  (accMap->size(),0); 
  vector<GenPartPlots*> GenPart_HF1_ATLAS1 (accMap->size(),0); 
  vector<GenPartPlots*> GenPart_HF1_ATLAS2 (accMap->size(),0); 
  vector<GenPartPlots*> GenPart_HF1_ATLAS6 (accMap->size(),0); 
  
  
  
  //MultiPlots*   MultiTest = NULL;
  //GenPartPlots* GenTest = NULL;
  //BINING  
  vector< vector<double> > binning; 
  cout << "Baseplot init done" <<endl;
  for(int acc = 0 ; acc < (int)accMap->size() ; ++acc){          
       cout << "init acc: " << acc <<endl;
      //binning = getBins(1,0,0);//nch,pt,eta
      binning = getBins(acc,E);       
      //BasePlots initialisation needed to get binning_array of MultiPlots right for nch etc
      baseplot->setBinning(binning);      
      
      GenPart_HF0_nocut.at(acc)       = new GenPartPlots(st("HF0_nocut_",acc));
      GenPart_HF0_MBUEWG.at(acc)      = new GenPartPlots(st("HF0_MBUEWG_",acc));
      GenPart_HF0_ALICE.at(acc)       = new GenPartPlots(st("HF0_ALICE_",acc));
      GenPart_HF0_ATLAS1.at(acc)      = new GenPartPlots(st("HF0_ATLAS1_",acc));
      GenPart_HF0_ATLAS2.at(acc)      = new GenPartPlots(st("HF0_ATLAS2_",acc));
      GenPart_HF0_ATLAS6.at(acc)      = new GenPartPlots(st("HF0_ATLAS6_",acc));
      
      GenPart_HF1_nocut.at(acc)       = new GenPartPlots(st("HF1_nocut_",acc));
      GenPart_HF1_MBUEWG.at(acc)      = new GenPartPlots(st("HF1_MBUEWG_",acc));
      GenPart_HF1_ALICE.at(acc)       = new GenPartPlots(st("HF1_ALICE_",acc));
      GenPart_HF1_ATLAS1.at(acc)      = new GenPartPlots(st("HF1_ATLAS1_",acc));
      GenPart_HF1_ATLAS2.at(acc)      = new GenPartPlots(st("HF1_ATLAS2_",acc));
      GenPart_HF1_ATLAS6.at(acc)      = new GenPartPlots(st("HF1_ATLAS6_",acc));
      
  }    
   cout << "full init done" <<endl;
    //get names and print
    stringstream mctype("");     mctype << type ;
    stringstream energy("");     energy << "_E_" << E ;
    TString dat="";          isMC ? dat="_MC"+mctype.str() : dat="_data";
    cout <<"-------------------------------------"<<endl;
    cout <<"          NCH GEN ONLY ANA           " << endl;
    cout <<"Running on max: " << nevt_max     << " events"   << endl;
    cout <<"E:              " << E        << " TeV"      << endl;
    cout <<"Data:           " << dat      << "      isMC: "   <<  isMC      << endl;
    
  
   if (nevt_max == -1) nevt_max=100000000;
   int i_tot = 0 , nevt_tot = 0;
   //starting Loop over files, stops at end of list of files or when reached nevt_max
   for(vector<TString>::iterator itfiles = vfiles->begin() ; itfiles != vfiles->end() && i_tot < nevt_max ; ++itfiles){

    TFile* file = TFile::Open(*itfiles,"READ");
    //getting the tree form the current file
    tree = (TTree*) file->Get("evt");
    if(isMC) {
        tree->SetBranchAddress("GenPart",&genPart);
        tree->SetBranchAddress("GenKin",&genKin);
    }  
     
    // GET NUMBER OF EVENTS
    //Getting number of events
    int nev = int(tree->GetEntriesFast());
    nevt_tot += nev;
    cout <<"The current file has " << nev << " entries : "<< endl << *itfiles << endl;

    bool passMBUEWGGen=0;       
    bool passALICEGen =0;   
    bool passATLAS1Gen=0;
    bool passATLAS2Gen=0;   
    bool passATLAS6Gen=0;
    bool passNSD=0; 
               
    ////////////////////////////
    /////    LOOP     //////////
    ////////////////////////////
    //starting loop over events, stops when reached end of file or nevt_max
    if (debug) cout << "Starting the loop over the events" <<endl;
    for(int i = 0; i < nev && i_tot < nevt_max; ++i , ++i_tot){      
        if( ((i+1) % 10000) == 0) cout <<int(double(i_tot+1)/1000)<<" k done"<<endl;    

        //Filling the variables defined setting branches
        tree->GetEntry(i);
              
        //Looping AcceptanceMap
        for(int acc = 0 ; acc < (int) accMap->size() ; ++acc){
        
            //GET GOOD PARTICLE AND TRACK COLLECTION        
            if(isMC) goodgenpart=getPrimaryGenPart(*genPart,accMap->at(acc).ptGen, accMap->at(acc).etaGenMin, 
                                                            accMap->at(acc).etaGenMax,accMap->at(acc).charge);  
                    
           //RESETTING EVENT SELECTION       
           passMBUEWGGen=0;       
           passALICEGen =0;   
           passATLAS1Gen=0;
           passATLAS2Gen=0;   
           passATLAS6Gen=0; 
           passNSD=0;             
           
           //SETTING THE CUT BOOLEANS for this event                   
           if(isMC) {        
              if(passCentral(goodgenpart,"MBUEWG",accMap->at(acc).ptReco)) passMBUEWGGen=1;
              if(passCentral(goodgenpart,"ALICE" ,accMap->at(acc).ptReco)) passALICEGen =1;   //Not using PT_PART HERE !!!!!!
              if(passCentral(goodgenpart,"ATLAS1",accMap->at(acc).ptReco)) passATLAS1Gen=1;
              if(passCentral(goodgenpart,"ATLAS2",accMap->at(acc).ptReco)) passATLAS2Gen=1;
              if(passCentral(goodgenpart,"ATLAS6",accMap->at(acc).ptReco)) passATLAS6Gen=1;
          
          } 
          if(!isSD(*genKin,MCtype)) passNSD=1;
          
          GenPart_HF0_nocut.at(acc)->fill(goodgenpart, weight);
          if( passMBUEWGGen  ) GenPart_HF0_MBUEWG.at(acc)->fill(goodgenpart, weight);
          if( passALICEGen   )  GenPart_HF0_ALICE.at(acc)->fill(goodgenpart, weight);
          if( passATLAS1Gen  ) GenPart_HF0_ATLAS1.at(acc)->fill(goodgenpart, weight);      
          if( passATLAS2Gen  ) GenPart_HF0_ATLAS2.at(acc)->fill(goodgenpart, weight);
          if( passATLAS6Gen  ) GenPart_HF0_ATLAS6.at(acc)->fill(goodgenpart, weight);  
              
          if (passNSD) GenPart_HF1_nocut.at(acc)->fill(goodgenpart, weight);
          if( passMBUEWGGen && passNSD ) GenPart_HF1_MBUEWG.at(acc)->fill(goodgenpart, weight);
          if( passALICEGen  && passNSD )  GenPart_HF1_ALICE.at(acc)->fill(goodgenpart, weight);
          if( passATLAS1Gen && passNSD ) GenPart_HF1_ATLAS1.at(acc)->fill(goodgenpart, weight);      
          if( passATLAS2Gen && passNSD ) GenPart_HF1_ATLAS2.at(acc)->fill(goodgenpart, weight);
          if( passATLAS6Gen && passNSD ) GenPart_HF1_ATLAS6.at(acc)->fill(goodgenpart, weight);   
                          
        }//end of loop over acceptanceMap
                 
    }//end of loop over events
   
    //Closing current files
    file->Close();
   }//end of loop over files
   
   //OUTPUT  
   stringstream nEvts("");      nEvts << "_" << i_tot ;
   TFile* output = new TFile("output"+dat+"_GenOnly"+energy.str()+nEvts.str()+".root","RECREATE");  
   output->cd();
   cout << "Writing to the file: " << endl << "output"+dat+"_GenOnly"+energy.str()+nEvts.str()+".root" << endl;
  for(int acc = 0 ; acc < (int)accMap->size() ; ++acc){
    GenPart_HF0_nocut.at(acc)->divideByWidthAll();
    GenPart_HF0_MBUEWG.at(acc)->divideByWidthAll();
    GenPart_HF0_ALICE.at(acc)->divideByWidthAll();
    GenPart_HF0_ATLAS1.at(acc)->divideByWidthAll();
    GenPart_HF0_ATLAS2.at(acc)->divideByWidthAll();
    GenPart_HF0_ATLAS6.at(acc)->divideByWidthAll();

    GenPart_HF1_nocut.at(acc)->divideByWidthAll();
    GenPart_HF1_MBUEWG.at(acc)->divideByWidthAll();
    GenPart_HF1_ALICE.at(acc)->divideByWidthAll();
    GenPart_HF1_ATLAS1.at(acc)->divideByWidthAll();
    GenPart_HF1_ATLAS2.at(acc)->divideByWidthAll();
    GenPart_HF1_ATLAS6.at(acc)->divideByWidthAll();

    GenPart_HF0_nocut.at(acc)->write();
    GenPart_HF0_MBUEWG.at(acc)->write();
    GenPart_HF0_ALICE.at(acc)->write();
    GenPart_HF0_ATLAS1.at(acc)->write();
    GenPart_HF0_ATLAS2.at(acc)->write();
    GenPart_HF0_ATLAS6.at(acc)->write();
   
    GenPart_HF1_nocut.at(acc)->write();
    GenPart_HF1_MBUEWG.at(acc)->write();
    GenPart_HF1_ALICE.at(acc)->write();
    GenPart_HF1_ATLAS1.at(acc)->write();
    GenPart_HF1_ATLAS2.at(acc)->write();
    GenPart_HF1_ATLAS6.at(acc)->write();
    
    delete GenPart_HF0_nocut.at(acc);
    delete GenPart_HF0_MBUEWG.at(acc);
    delete GenPart_HF0_ALICE.at(acc);
    delete GenPart_HF0_ATLAS1.at(acc);
    delete GenPart_HF0_ATLAS2.at(acc);
    delete GenPart_HF0_ATLAS6.at(acc);   
    
    delete GenPart_HF1_nocut.at(acc);
    delete GenPart_HF1_MBUEWG.at(acc);
    delete GenPart_HF1_ALICE.at(acc);
    delete GenPart_HF1_ATLAS1.at(acc);
    delete GenPart_HF1_ATLAS2.at(acc);
    delete GenPart_HF1_ATLAS6.at(acc);
    
  }
  delete baseplot;

  output->Close();
  delete output;
 
  cout  << "-------------------------------------" << endl;         
}

