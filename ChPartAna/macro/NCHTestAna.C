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
#include "../plugins/MyTracks.h"
#include "../plugins/MyVertex.h"
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
  //   nohup root -l BuildLibDico.C+ NCHTestAna.C+"(10,7,1,2000000)" -q   > logMC10v5b_2M.txt &
  //   nohup root -l BuildLibDico.C+ NCHTestAna.C+"(60,7,1,2000000)" -q   > logMC60v5b_2M.txt & 
  //   nohup root -l BuildLibDico.C+ NCHTestAna.C+"(31,7,1,2000000)" -q   > logMC31v5b_2M.txt & 
  //   nohup root -l BuildLibDico.C+ NCHTestAna.C+"(15,7,1,2000000)" -q   > logMC15v5b_2M.txt &
  //   nohup root -l BuildLibDico.C+ NCHTestAna.C+"(0,7,1,-1)"       -q   > logData12Mv5b.txt & 
  //   nohup root -l BuildLibDico.C+ NCHTestAna.C+"(5,7,1,-1)"       -q   > logZeroBias1M.txt & 
  //   tail -f logData.txt
  /////////////////////////
 
//_____________________________________________________________________________
TString st(string input , int cut){
  stringstream nb("");  
  nb<<cut;
  return input+"cut"+nb.str();
}


//_____________________________________________________________________________
void NCHTestAna(int type = 60 , double E = 7. , int iTracking = 1, int nevt_max = 1000, bool use_weight = true, bool allEff = false){
 
  ////////////////////////////////////////////////
  //SWTICH for most of the intermediate plots:
  NCHCentralPlots::switchMBUEWGonly=0;        // 0 = only nocut and MBUEWG 
  NCHHFPlots::switchAllHF=0;                  // 0 = only HF0 and HF1
  NCHEvtSelPlots::switchIntermedPlots=allEff;      // 0 = only noSel and FullSel
  //if type=31, program will launch old code
  ////////////////////////////////////////////
 
  //EVENT SELECTION TYPE, see NCHEvtSel.C for more information 
  bool isMC = true; 
  if(type < 6 ) isMC = false; 
  if(!isMC) use_weight=false;
  NCHDiffPlots::isMC=isMC;
  
  TString MCtype = "pythia";
  if(type == 20) MCtype = "phojet";
  //if(type == 15) MCtype = "z2";
  if(type == 60) MCtype = "pythia8";
  if(  !(iTracking==1 || iTracking==0))  {
     cout <<"WRONG TRACKING NUMBER !!!!"<<endl; 
     return;
  } 
  TString tracking="";   iTracking ? tracking="_ferncTr" : tracking="_genTr";
  
  
  // BUILDING CHAIN
  //adding files to the tree chain
  TH1::AddDirectory(0); //Detach histograms from rootfiles (they are no closed if the file is closed.)
  cout << "FILE: " << fileManager(0,type,E) << endl;
  //getting the list of files
  //vector<TString>* vfiles = getListOfFiles(fileManager(0,type,E));
  vector<TString>* vfiles = getListOfFiles("../filelist.txt");
  if(type==0 && E==7) vfiles = getListOfFiles("list_filesRun132440.txt"); 
  if(type==5) vfiles = getListOfFiles(fileManager(0,type,E));
  
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
  //vector<MyVertex>*  vertexToCut  = NULL;
  vector<MyTracks>   goodtracks;
  vector<MyGenPart>  goodgenpart;
  double weight =1.;
  
  //Initialize AcceptanceMap___________________________________________________________
  InitializeAcceptanceMap();
  cout <<"-------------------------------------"<<endl;
  cout << "acceptanceMap: " << accMap->size() <<" loaded "  <<endl; 
  
  
  //Initialising Plots tree
  if (debug) cout << "Initialising all Plots" <<endl;   
  vector<NCHDiffPlots*> nchDiffPlots (accMap->size(),0);  
  //temp, before dynamical binning, see above
  BasePlots*       baseplot    = new BasePlots("BasePlots");
  vector<TrackPlots*> Track_Full_nocut  (accMap->size(),0); 
  vector<TrackPlots*> Track_Full_ALICE  (accMap->size(),0); 
  vector<TrackPlots*> Track_Full_ATLAS6 (accMap->size(),0); 
  
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
      
      Track_Full_nocut.at(acc)      = new TrackPlots(st("nocut_",acc));
      Track_Full_ALICE.at(acc)      = new TrackPlots(st("ALICE_",acc));
      Track_Full_ATLAS6.at(acc)     = new TrackPlots(st("ATLAS6_",acc));
      nchDiffPlots.at(acc)  = new NCHDiffPlots(st("",acc));
      
  }    
   cout << "full init done" <<endl;
    //get names and print
    stringstream mctype("");     mctype << type ;
    stringstream energy("");     energy << "_E_" << E ;
    TString dat="";          isMC ? dat="_MC"+mctype.str() : dat="_data";
    if (type ==5) dat.ReplaceAll("data","zerobias");
    cout <<"-------------------------------------"<<endl;
    cout <<"Running on max: " << nevt_max     << " events"   << endl;
    cout <<"E:              " << E        << " TeV"      << endl;
    cout <<"Tracking:       " << tracking << " number: " <<  iTracking << endl;
    cout <<"Data:           " << dat      << "      isMC: "   <<  isMC      << endl;
    cout <<"MBUEWGonly:     " << NCHCentralPlots::switchMBUEWGonly << endl;
    cout <<"AllHF:          " << NCHHFPlots::switchAllHF << endl;
    cout <<"IntermedPlots:  " << NCHEvtSelPlots::switchIntermedPlots <<endl;
    
    // REWEIGHT with the Z Vertex --------------------------
    // get MC and Data vertex out of already runned >1M+ files
    // normalise then
    // get efficiency data/MC
    //weight = eff->GetBinContent(z);    
    cout  << " ---- Doing The Reweighting with the Z vertex ----" << endl;
    TString nevt_oldrun_strData="342220";
    if (E == 0.9) nevt_oldrun_strData="2250000";
    TString nevt_oldrun_strMC="5000000";
    //if( type == 31 ) nevt_oldrun_strMC = "1886500";
    TString oldrundir="dcap:///pnfs/iihe/cms/store/user/sluyckx/v21NoWeight";
    if( E == 0.9) oldrundir="dcap:///pnfs/iihe/cms/store/user/sluyckx/v21_900NoWeight";
    TFile* older_runnedfileData = NULL;
    TFile* older_runnedfileMC   = NULL;
    TH1F* Weight_Z_Corr = NULL; 
    
    //only on MC 
    if ( isMC && use_weight==true) {
        if (iTracking == 0) cout << "genTracking will be used" << endl;
        else cout << "ferencTracking will be used" << endl;
        older_runnedfileData = TFile::Open(oldrundir+"/output_data"+tracking+energy.str()+"_"+nevt_oldrun_strData+".root","READ");   
        older_runnedfileMC   = TFile::Open(oldrundir+"/output"+dat+tracking+energy.str()+"_"+nevt_oldrun_strMC  +"_noweight.root","READ"); 
        cout << "Reading the old files from: " <<  oldrundir << endl;
        cout << "Reading the old files: " << oldrundir+"/output_data"+tracking+energy.str()+"_"+nevt_oldrun_strData+".root"            << " ptr: " << older_runnedfileData << endl;
        cout << "Reading the old files: " << oldrundir+"/output"+dat+tracking+energy.str()+"_"+nevt_oldrun_strMC  +"_noweight.root"    << " ptr: " << older_runnedfileMC   << endl;
        if (older_runnedfileData == 0 || older_runnedfileMC == 0 ) return; 
        
        
        const TString histoName="z_full_HF0_nocut_RECO_cut0";
        TH1F* CurveZData = NULL; CurveZData = (TH1F*) older_runnedfileData->FindObjectAny(histoName);
        TH1F* CurveZMC   = NULL; CurveZMC   = (TH1F*) older_runnedfileMC  ->FindObjectAny(histoName);
        cout << "CurveZData: " << CurveZData   <<  "  CurveZMC: "  << CurveZMC <<endl;
        if ( CurveZData == 0 || CurveZMC == 0 ) return;         
        
        //normalise both
        CurveZData->Scale(1./CurveZData->Integral());    //NEED TO ADJUST with bin !=1
        CurveZMC  ->Scale(1./CurveZMC  ->Integral());
        
        Weight_Z_Corr = (TH1F*) CurveZData->Clone("weight_Z_Corr");
        Weight_Z_Corr->Divide(CurveZData, CurveZMC, 1, 1, "B");
        Weight_Z_Corr->SetBinContent(0, 1.);

        Weight_Z_Corr->SetBinContent(  Weight_Z_Corr->GetNbinsX()+1, 1.);
        Weight_Z_Corr->SetMinimum(0);
        
        delete older_runnedfileData;  delete older_runnedfileMC;
        delete CurveZData;  delete CurveZMC;
        cout << "succesfull reweighting " <<endl;
    }    
    cout << "reweighting passed" <<endl;

 
    // REWEIGHTING finished --------------------------------
    
    
    
    
  
   if (nevt_max == -1) nevt_max=100000000;
   int i_tot = 0 , nevt_tot = 0;
   //starting Loop over files, stops at end of list of files or when reached nevt_max
   for(vector<TString>::iterator itfiles = vfiles->begin() ; itfiles != vfiles->end() && i_tot < nevt_max ; ++itfiles){

    TFile* file = TFile::Open(*itfiles,"READ");
    //getting the tree form the current file
    tree = (TTree*) file->Get("evt");

    //adding branches to the tree ----------------------------------------------------------------------
    tree->SetBranchAddress("EvtId",&evtId);
    tree->SetBranchAddress("L1Trig",&L1Trig);
    if(! ( type==31 ) ) 
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
    //Getting number of events
    int nev = int(tree->GetEntriesFast());
    nevt_tot += nev;
    cout <<"The current file has " << nev << " entries : "<< endl << *itfiles << endl;

    
    ////////////////////////////
    /////    LOOP     //////////
    ////////////////////////////
    //starting loop over events, stops when reached end of file or nevt_max
    if (debug) cout << "Starting the loop over the events" <<endl;
    for(int i = 0; i < nev && i_tot < nevt_max; ++i , ++i_tot){      
        if( ((i+1) % 10000) == 0) cout <<int(double(i_tot+1)/1000)<<" k done"<<endl;    

        //Filling the variables defined setting branches
        tree->GetEntry(i);
        
        //only run on NONE PILE-UP DATA
        if(!isMC && E==7 && type ==0)
           if(132440!=(signed)evtId->Run)
	    continue;
    
    
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
           
            // Z WEIGHTING
            if(use_weight==1 && isMC) {
                weight = Weight_Z_Corr->GetBinContent( Weight_Z_Corr->GetXaxis()->FindFixBin( goodVtx-> z ) ) ;
                //cout << "i: " << i << "weight: " << weight << endl;
            }    
           
                   
           //RESETTING EVENT SELECTION  
           NCHDiffPlots::isSD=0;  NCHDiffPlots::isDD=0;          
           NCHCentralPlots::passMBUEWGGen=0;       
           NCHCentralPlots::passALICEGen =0;   
           NCHCentralPlots::passATLAS1Gen=0;
           NCHCentralPlots::passATLAS2Gen=0;   
           NCHCentralPlots::passATLAS6Gen=0;              
           
           NCHCentralPlots::passMBUEWGRECO=0;       
           NCHCentralPlots::passALICERECO =0;   
           NCHCentralPlots::passATLAS1RECO=0;
           NCHCentralPlots::passATLAS2RECO=0;   
           NCHCentralPlots::passATLAS6RECO=0;     
           NCHHFPlots::passHF1=0;     //passHF2=!passHF1
           NCHHFPlots::passHF3=0;     NCHHFPlots::passHF4=0;
           NCHHFPlots::passHF5=0;     NCHHFPlots::passHF6=0;                    
           NCHEvtSelPlots::passL1 =0;    NCHEvtSelPlots::passVtxQual=0;
           NCHEvtSelPlots::passVtx=0;    NCHEvtSelPlots::passBit40=0;                    
         

           //SETTING THE CUT BOOLEANS for this event                   
           if(isMC) {        
              if(isSD(*genKin,MCtype)) NCHDiffPlots::isSD=1;
              if(isDD(*genKin,MCtype)) NCHDiffPlots::isDD=1;
              if(passCentral(goodgenpart,"MBUEWG",accMap->at(acc).ptReco)) NCHCentralPlots::passMBUEWGGen=1;
              if(passCentral(goodgenpart,"ALICE" ,accMap->at(acc).ptReco)) NCHCentralPlots::passALICEGen =1;   //Not using PT_PART HERE !!!!!!
              if(passCentral(goodgenpart,"ATLAS1",accMap->at(acc).ptReco)) NCHCentralPlots::passATLAS1Gen=1;
              if(passCentral(goodgenpart,"ATLAS2",accMap->at(acc).ptReco)) NCHCentralPlots::passATLAS2Gen=1;
              if(passCentral(goodgenpart,"ATLAS6",accMap->at(acc).ptReco)) NCHCentralPlots::passATLAS6Gen=1;
          
          } 
                 
          if(passCentral(goodtracks ,"MBUEWG",accMap->at(acc).ptReco)) NCHCentralPlots::passMBUEWGRECO=1;                
          if(passCentral(goodtracks ,"ALICE" ,accMap->at(acc).ptReco)) NCHCentralPlots::passALICERECO =1; 
          if(passCentral(goodtracks ,"ATLAS1",accMap->at(acc).ptReco)) NCHCentralPlots::passATLAS1RECO=1; 
          if(passCentral(goodtracks ,"ATLAS2",accMap->at(acc).ptReco)) NCHCentralPlots::passATLAS2RECO=1; 
          if(passCentral(goodtracks ,"ATLAS6",accMap->at(acc).ptReco)) NCHCentralPlots::passATLAS6RECO=1;
            
        
          if(passHF(*MITEvtSel,1)) NCHHFPlots::passHF1=1;
          //passHF2 = !passHF1;         
          if(passHF(*MITEvtSel,3)) NCHHFPlots::passHF3=1;
          if(passHF(*MITEvtSel,4)) NCHHFPlots::passHF4=1;
          if(passHF(*MITEvtSel,5)) NCHHFPlots::passHF5=1;
          if(passHF(*MITEvtSel,6)) NCHHFPlots::passHF6=1;
     
          if(type==31 ) { 
            if(passL1(E, *L1Trig, isMC)) NCHEvtSelPlots::passL1=1;
          }        
          else {
            if(passL1HLT(E, *L1Trig, *HLTrig , 1,isMC ))  
                NCHEvtSelPlots::passL1=1;
          }      
          if(passVtxQual(*MITEvtSel,E)) NCHEvtSelPlots::passVtxQual=1;     
          if(passVtx(*vertex)) NCHEvtSelPlots::passVtx=1;     
          //if(passBit40(*L1Trig)) NCHEvtSelPlots::passBit40=1;
          
          
          //extra tracks with full sell (noHF)
          if( NCHEvtSelPlots::passL1 && NCHEvtSelPlots::passVtx && NCHEvtSelPlots::passVtxQual ) {
            Track_Full_nocut.at(acc)->fill(*tracks, goodVtx, bestVertexId,bs, weight);
            if( NCHCentralPlots::passALICERECO  ) Track_Full_ALICE.at(acc) ->fill(*tracks, goodVtx, bestVertexId,bs, weight);
            if( NCHCentralPlots::passATLAS6RECO ) Track_Full_ATLAS6.at(acc)->fill(*tracks, goodVtx, bestVertexId,bs, weight);         
          }
          
          
          //filling
          nchDiffPlots.at(acc)->fill(goodgenpart,*genKin,MCtype,goodtracks,*vertex,goodVtx,bestVertexId,bs, weight);
        
        }//end of loop over acceptanceMap
                 
    }//end of loop over events
   
    //Closing current files
    file->Close();
   }//end of loop over files
   
   //OUTPUT  
   stringstream nEvts("");      nEvts << "_" << i_tot ;
   TString weight_str = "";
   if (use_weight==false && isMC==true) weight_str = "_noweight";
   if (allEff ==true ) weight_str +="_allEffs";
   TString out_str = "output"+dat+tracking+energy.str()+nEvts.str()+weight_str+".root";
   TFile* output = new TFile(out_str,"RECREATE");  
   output->cd();
   cout << "Writing to the file: " << endl << out_str << endl;
  for(int acc = 0 ; acc < (int)accMap->size() ; ++acc){
    nchDiffPlots.at(acc)->write();
    Track_Full_nocut.at(acc)->write();
    Track_Full_ALICE.at(acc)->write();
    Track_Full_ATLAS6.at(acc)->write();
    
    delete nchDiffPlots.at(acc);
    delete Track_Full_nocut.at(acc);
    delete Track_Full_ALICE .at(acc);
    delete Track_Full_ATLAS6.at(acc);
    
  }
  delete baseplot;
  if ( isMC && use_weight == 1 && E==7 ) { 
    Weight_Z_Corr->Write();
    delete Weight_Z_Corr; 

  }  
  output->Close();
  delete output;
 
  cout  << "-------------------------------------" << endl;         
}

