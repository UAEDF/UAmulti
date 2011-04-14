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
#include "../plugins/MyFwdGap.h"
#include "../plugins/MyGenKin.h"
#include "../plugins/MyPart.h"
#include "../plugins/MyGenPart.h"
#include "../plugins/MyTracks.h"
#include "../plugins/MyVertex.h"
#include "../plugins/MyMITEvtSel.h"
#include "../plugins/MyBeamSpot.h"

bool debug = false;

TString filename = "MC_test_test_900GeV";
bool isMC = true;


#include "fileManager.C"
#include "cuts.C"
#include "evtSel.C"
#include "binningMap.C"

TString st(string input , int cut){
  stringstream out("");
  out<<cut;
  return input+"_cut"+out.str();
}

//nohup root -b -l BuildLibDico.C+ ZBTrigEval.C+"(5,7,1,100000000)" -q > log_zbtrigeval_type5_7TeV.txt &

void ZBTrigEval(int = 5 , double = 7 , int = 1 , int = 1000000000  );

void ZBTrigEval(int type , double E , int iTracking , int nevt_max )
{

  //#include "acceptanceMap.C" 
  
  TH1::AddDirectory(0);
  
  if(type < 10) isMC = false;
  TString MCtype = "pythia";
  if(type == 20) MCtype = "phojet";
  if(type == 60 || type==101) MCtype = "pythia8";

  float nev_tot     = 0;
  float nev_passBX  = 0;
  float nev_passb0  = 0;
  float nev_passb4  = 0;
  float nev_passb8  = 0;
  float nev_passb9  = 0;
  float nev_passb10 = 0;
  float nev_passb34 = 0;
  float nev_passb40 = 0;
  float nev_passvQ  = 0;
  float nev_passHF  = 0;
  float nev_passVZ  = 0;


  int nbin = 70;
  TH1F* nchb0   = new TH1F("nchb0","nchb0",nbin,-0.5,nbin-1);     
  TH1F* nchb4   = new TH1F("nchb4","nchb4",nbin,-0.5,nbin-1);     
  TH1F* nchb8   = new TH1F("nchb8","nchb8",nbin,-0.5,nbin-1);  
  TH1F* nchb9   = new TH1F("nchb9","nchb9",nbin,-0.5,nbin-1);   
  TH1F* nchb10  = new TH1F("nchb10","nchb10",nbin,-0.5,nbin-1); 
  TH1F* nchb34  = new TH1F("nchb34","nchb34",nbin,-0.5,nbin-1);     
  TH1F* nchb40  = new TH1F("nchb40","nchb40",nbin,-0.5,nbin-1);      
  TH1F* nchL1VT = new TH1F("nchL1VT","nchL1VT",nbin,-0.5,nbin-1);   
  TH1F* nchHF   = new TH1F("nchHF"  ,"nchHF"  ,nbin,-0.5,nbin-1);   
  TH1F* nchL1HF = new TH1F("nchL1HF","nchL1HF",nbin,-0.5,nbin-1);

  nchb0->Sumw2();
  nchb4->Sumw2();
  nchb8->Sumw2();
  nchb9->Sumw2();
  nchb10->Sumw2();
  nchb34->Sumw2();
  nchb40->Sumw2();
  nchL1VT->Sumw2();
  nchHF->Sumw2();
  nchL1HF->Sumw2();



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
    //tree->SetBranchAddress("pixel3Vertex",&vertexToCut);
    tree->SetBranchAddress("L1Trig",&L1Trig);
    //tree->SetBranchAddress("HLTrig",&HLTrig);
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
     
      tree->GetEntry(i);

      ++nev_tot;

      //if( !isMC && !goodBX(evtId->Run,evtId->Bunch)) continue; NOT NEEDED IF b0 IS USED
      ++nev_passBX;

      if( ! passVtxQual(*MITEvtSel , E) ) continue;
      ++nev_passvQ;

      //if( ! passVtx(vertexToCut) ) continue;
      if( ! passVtx(vertex) ) continue;
      ++nev_passVZ;

      if( (L1Trig->TechTrigWord[4] && !isMC) ) 
        ++nev_passb4;

      if( ! (L1Trig->TechTrigWord[0] || isMC) ) continue;
        ++nev_passb0;
      
      
      int nch = 0 ;
      if(iTracking==0) nch = getnPrimaryTracks(tracks,vertex);
      if(iTracking==1) nch = getnPrimaryTracks(tracks,vertex);  

      nchb0->Fill(nch);
    
      if (    !L1Trig->TechTrigWord[36]
           && !L1Trig->TechTrigWord[37]
           && !L1Trig->TechTrigWord[38]
           && !L1Trig->TechTrigWord[39] )
	nchL1VT->Fill(nch);
      
      //copy of b0 = BPTX AND
      if ( L1Trig->TechTrigWord[4]  && !isMC) nchb4->Fill(nch);
      
      //BSC OR
      if ( L1Trig->TechTrigWord[40] )         nchb40->Fill(nch);
      
      //HF Combination
      if ( L1Trig->TechTrigWord[8]  && !isMC) nchb8->Fill(nch);
      if ( L1Trig->TechTrigWord[9]  && !isMC) nchb9->Fill(nch);
      if ( L1Trig->TechTrigWord[10] && !isMC) nchb10->Fill(nch);
      
      //HF AND
      if ( passHF(*MITEvtSel)       ) nchHF->Fill(nch);


      bool passb34 = false;
      if ( E < 7. ) {
           passb34 = L1Trig->TechTrigWord[34] ;
      }
      else {
        if (  isMC ) passb34 = L1Trig->TechTrigWord[34] ;
        if ( !isMC ) passb34 = L1Trig->PhysTrigWord[124];  
      }
      if(passb34) nchb34->Fill(nch);
      
      if (    passb34 
           && !L1Trig->TechTrigWord[36]
           && !L1Trig->TechTrigWord[37]
           && !L1Trig->TechTrigWord[38]
           && !L1Trig->TechTrigWord[39]
           && passHF(*MITEvtSel)        )
	 nchL1HF->Fill(nch);   

      /*if( ! passL1(E, *L1Trig)) continue;
      ++nev_passL1;

      if( ! passHF(*MITEvtSel) ) continue;
      ++nev_passHF;
      */



    }//end of loop over events

    //Closing current files
    file->Close();

  }//end of loop over files


  cout << "nev_tot     = " << nev_tot << endl;
  cout << "nev_passBX  = " << nev_passBX << endl;
  cout << "nev_passvQ  = " << nev_passvQ << endl;
  cout << "nev_passVZ  = " << nev_passVZ << endl;
  cout << "nev_passb0  = " << nev_passb0 << endl;
  cout << "nev_passb4  = " << nev_passb4 << endl;
  cout << "nev_passb8  = " << nev_passb8 << endl;
  cout << "nev_passb9  = " << nev_passb9 << endl;
  cout << "nev_passb10 = " << nev_passb10 << endl;
  cout << "nev_passb34 = " << nev_passb34 << endl;
  cout << "nev_passb40 = " << nev_passb40 << endl;
  cout << "nev_passHF  = " << nev_passHF << endl;

  cout << "nev_passBX  % " << nev_passBX/nev_tot << endl;
  cout << "nev_passvQ  % " << nev_passvQ/nev_tot << endl;
  cout << "nev_passVZ  % " << nev_passVZ/nev_tot << endl;
  cout << "nev_passb0  % " << nev_passb0/nev_tot << endl;
  cout << "nev_passb4  % " << nev_passb4/nev_tot << endl;
  cout << "nev_passb8  % " << nev_passb8/nev_tot << endl;
  cout << "nev_passb9  % " << nev_passb9/nev_tot << endl;
  cout << "nev_passb10 % " << nev_passb10/nev_tot << endl;
  cout << "nev_passb34 % " << nev_passb34/nev_tot << endl;
  cout << "nev_passb40 % " << nev_passb40/nev_tot << endl;
  cout << "nev_passHF  % " << nev_passHF/nev_tot << endl;

  TH1F* effb4 = (TH1F*)  nchb4->Clone("effb4");
  TH1F* effb8 = (TH1F*)  nchb8->Clone("effb8");
  TH1F* effb9 = (TH1F*)  nchb9->Clone("effb9");
  TH1F* effb10 = (TH1F*) nchb10->Clone("effb10");
  TH1F* effb34 = (TH1F*) nchb34->Clone("effb34");
  TH1F* effb40 = (TH1F*) nchb40->Clone("effb40");
  TH1F* effL1VT = (TH1F*) nchL1VT->Clone("effL1VT");
  TH1F* effHF   = (TH1F*) nchHF->Clone("effHF");
  TH1F* effL1HF = (TH1F*) nchL1HF->Clone("effL1HF");

  effb4->Divide(effb4,nchb0,1,1,"B");
  effb8->Divide(effb8,nchb0,1,1,"B");
  effb9->Divide(effb9,nchb0,1,1,"B");
  effb10->Divide(effb10,nchb0,1,1,"B");
  effb34->Divide(effb34,nchb0,1,1,"B");
  effb40->Divide(effb40,nchb0,1,1,"B");
  effL1VT->Divide(effL1VT,nchb0,1,1,"B");
  effHF->Divide(effHF,nchb0,1,1,"B");
  effL1HF->Divide(effL1HF,nchb0,1,1,"B");

  //TFile* file2=new TFile("ZBfile.root","RECREATE");
  TFile* file2=new TFile(fileManager(4,type,E,0,1,1),"RECREATE");
  file2->cd();
  gDirectory->mkdir("zbeff");
  gDirectory->cd("zbeff");

  nchb0->Write();
  nchb4->Write();
  nchb8->Write();
  nchb9->Write();
  nchb10->Write();
  nchb34->Write();
  nchb40->Write();
  nchL1VT->Write();
  nchHF->Write();
 
  effb4->Write();
  effb8->Write();
  effb9->Write();
  effb10->Write();
  effb34->Write();
  effb40->Write();
  effL1VT->Write();
  effHF->Write();
  effL1HF->Write();

  if (type > 9) {

    TH1F* effL1HFup = (TH1F*) effL1HF->Clone("effL1HFup");
    TH1F* effL1HFdo = (TH1F*) effL1HF->Clone("effL1HFdo");

    double maxerrup = 0.04 ;
    double maxerrdo = 0.06 ;
    double minerr = 0.01 ;

    int firstbin = 1;
    int lastbin  = 25;
/*
    for(; lastbin <= effL1HF->GetNbinsX() - 1  ; ++lastbin){
      if(effL1HF->GetBinContent(lastbin)==1 && (effL1HF->GetBinContent(lastbin+1)==1 || effL1HF->GetBinContent(lastbin+1)==0 )){
        break;
      }
    } 
*/

    int syst_sign = 1;
    for(int ibin = 1 ; ibin <= effL1HF->GetNbinsX() ; ++ibin){
      double val = effL1HF->GetBinContent(ibin) + syst_sign * ( maxerrup - (double(ibin)-double(firstbin))/(double(lastbin)-double(firstbin)) * maxerrup + minerr);
      if(ibin>=lastbin && syst_sign == -1) val = effL1HF->GetBinContent(ibin)  -minerr;
      if(ibin>=lastbin && syst_sign ==  1) val = effL1HF->GetBinContent(ibin)  +minerr;
      if(val>1) val = 1.;
   //cout<<effL1HF->GetBinContent(ibin)<<"  "<<val<<"  "<<(val-effL1HF->GetBinContent(ibin))/effL1HF->GetBinContent(ibin)<<endl;
      effL1HFup->SetBinContent(ibin,val);
    }

    syst_sign = -1;
    for(int ibin = 1 ; ibin <= effL1HF->GetNbinsX() ; ++ibin){
      double val = effL1HF->GetBinContent(ibin) + syst_sign * ( maxerrdo - (double(ibin)-double(firstbin))/(double(lastbin)-double(firstbin)) * maxerrdo + minerr);
      if(ibin>=lastbin && syst_sign == -1) val = effL1HF->GetBinContent(ibin) - minerr;
      if(ibin>=lastbin && syst_sign ==  1) val = effL1HF->GetBinContent(ibin) + minerr;
      if(val>1) val = 1.;
   //cout<<effL1HF->GetBinContent(ibin)<<"  "<<val<<"  "<<(val-effL1HF->GetBinContent(ibin))/effL1HF->GetBinContent(ibin)<<endl;
      effL1HFdo->SetBinContent(ibin,val);
    } 

    effL1HFup->Write();
    effL1HFdo->Write();

  }
 
  file2->Close();
}


