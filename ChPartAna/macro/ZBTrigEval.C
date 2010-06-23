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

void ZBTrigEval(int = 10 , double = 0.9 , TString = "test" , int = 0 , int = 0 , int = 0 );

void ZBTrigEval(int type , double E , TString filename , int nevt_max , int iTracking , int irun)
{

  #include "acceptanceMap.C" 

  if(type < 10) isMC = false;

  TChain* tree = new TChain("evt","");
  tree->Add(fileManager(0,type,E));
  cout <<  fileManager(0,type,E) << endl;
  if ( type == 5 ) tree->Add(fileManager(0,type,2.36));
  if ( type == 5 ) tree->Add(fileManager(0,type,7.0));

  MyEvtId* evtId      = NULL ;
  tree->SetBranchAddress("EvtId",&evtId);
    
  vector<MyGenPart>* genPart = NULL;
  if(isMC) tree->SetBranchAddress("GenPart",&genPart);

  MyGenKin* genKin = NULL;   
  if(isMC) tree->SetBranchAddress("GenKin",&genKin);

  vector<MyTracks>*  tracks  = NULL;
  vector<MyVertex>*  vertex  = NULL;
  if(iTracking==0){
    tree->SetBranchAddress("generalTracks",&tracks);
    tree->SetBranchAddress("primaryVertex",&vertex);
  } 
  else if(iTracking==1){
    tree->SetBranchAddress("minbiasTracks",&tracks); 
    tree->SetBranchAddress("ferencVtxFerTrk",&vertex);
  } 

  vector<MyVertex>*  vertexToCut  = NULL;
  MyL1Trig*     L1Trig    = NULL;
  MyMITEvtSel*  MITEvtSel = NULL;
  MyBeamSpot*   bs        = NULL;
  tree->SetBranchAddress("pixel3Vertex",&vertexToCut);
  tree->SetBranchAddress("L1Trig",&L1Trig);
  tree->SetBranchAddress("MITEvtSel",&MITEvtSel);
  tree->SetBranchAddress("beamSpot",&bs);
    
  int nev = int(tree->GetEntries());
  std::cout <<"number of entries is : "<< nev << std::endl;
  if ( nevt_max == 0 ) nevt_max = nev;
  cout<<"Running on: "<<min(nev,nevt_max)<<" events"<<endl;

  float nev_tot     = 0;
  float nev_passBX  = 0;
  float nev_passL1  = 0;
  float nev_passvQ  = 0;
  float nev_passHF  = 0;
  float nev_passVZ  = 0;

  TH1F* nchBase = new TH1F("nchBase","nchBase",50,0,50);   
  TH1F* nchL134 = new TH1F("nchL134","nchL134",50,0,50);   
  TH1F* nchL140 = new TH1F("nchL140","nchL140",50,0,50);   
  TH1F* nchL1VT = new TH1F("nchL1VT","nchL1VT",50,0,50);   
  TH1F* nchHF   = new TH1F("nchHF"  ,"nchHF"  ,50,0,50);   
  TH1F* nchL1HF = new TH1F("nchL1HF","nchL1HF",50,0,50);

  nchBase->Sumw2();
  nchL134->Sumw2();
  nchL140->Sumw2();
  nchL1VT->Sumw2();
  nchHF->Sumw2();
  nchL1HF->Sumw2();

  for(int i = 0; i < min(nev,nevt_max) ; i++){

    if( ((i+1) % 10000) == 0) cout <<int(double(i+1)/double(min(nev,nevt_max))*100.)<<" % done"<<endl;
    tree->GetEntry(i);

    ++nev_tot;

    if( !isMC && !goodBX(evtId->Run,evtId->Bunch)) continue;
    ++nev_passBX;

    if( ! passVtxQual(*MITEvtSel , E) ) continue;
    ++nev_passvQ;

    //if( ! passVtx(vertexToCut) ) continue;
    if( ! passVtx(vertex) ) continue;
    ++nev_passVZ;

    if( ! (L1Trig->TechTrigWord[0] || isMC) ) continue; 
    ++nev_passL1;

    int acc = 5 ;
    int nch = 0 ;
    if(iTracking==0) nch = getnPrimaryTracks(tracks,vertex,accMap[acc].at(2),accMap[acc].at(3),accMap[acc].at(4));
    if(iTracking==1) nch = getnPrimaryTracks(tracks,vertex,bs,accMap[acc].at(2),accMap[acc].at(3),accMap[acc].at(4));  

    nchBase->Fill(nch);
    
    if (    !L1Trig->TechTrigWord[36]
         && !L1Trig->TechTrigWord[37]
         && !L1Trig->TechTrigWord[38]
         && !L1Trig->TechTrigWord[39] ) nchL1VT->Fill(nch);
    if ( E < 7. ) {
      if ( L1Trig->TechTrigWord[34] ) nchL134->Fill(nch);
    } else {
      if (  isMC && L1Trig->TechTrigWord[34] ) nchL134->Fill(nch);
      if ( !isMC && L1Trig->PhysTrigWord[124]) nchL134->Fill(nch); 
    }
    if ( L1Trig->TechTrigWord[40] ) nchL140->Fill(nch);
    if ( passHF(*MITEvtSel)       ) nchHF->Fill(nch);


    bool b34 = false;
    if ( E < 7. ) {
         b34 = L1Trig->TechTrigWord[34] ;
     } else {
        if (  isMC ) b34 = L1Trig->TechTrigWord[34] ;
        if ( !isMC ) b34 = L1Trig->PhysTrigWord[124];  
     }

    if (    b34 
         && !L1Trig->TechTrigWord[36]
         && !L1Trig->TechTrigWord[37]
         && !L1Trig->TechTrigWord[38]
         && !L1Trig->TechTrigWord[39]
         && passHF(*MITEvtSel)        ) nchL1HF->Fill(nch);   

/*
    if( ! passL1(E, *L1Trig)) continue;
    ++nev_passL1;

    if( ! passHF(*MITEvtSel) ) continue;
    ++nev_passHF;
*/



  } //end of loop over entries from tree

  cout << "nev_tot    = " << nev_tot << endl;
  cout << "nev_passBX = " << nev_passBX << endl;
  cout << "nev_passvQ = " << nev_passvQ << endl;
  cout << "nev_passVZ = " << nev_passVZ << endl;
  cout << "nev_passL1 = " << nev_passL1 << endl;
  cout << "nev_passHF = " << nev_passHF << endl;

  cout << "nev_passBX % " << nev_passBX/nev_tot << endl;
  cout << "nev_passvQ % " << nev_passvQ/nev_tot << endl;
  cout << "nev_passVZ % " << nev_passVZ/nev_tot << endl;
  cout << "nev_passL1 % " << nev_passL1/nev_tot << endl;
  cout << "nev_passHF % " << nev_passHF/nev_tot << endl;

  TH1F* effL134 = (TH1F*) nchL134->Clone("effL134");
  TH1F* effL140 = (TH1F*) nchL140->Clone("effL140");
  TH1F* effL1VT = (TH1F*) nchL1VT->Clone("effL1VT");
  TH1F* effHF   = (TH1F*) nchHF->Clone("effHF");
  TH1F* effL1HF = (TH1F*) nchL1HF->Clone("effL1HF");

  effL134->Divide(effL134,nchBase,1,1,"B");
  effL140->Divide(effL140,nchBase,1,1,"B");
  effL1VT->Divide(effL1VT,nchBase,1,1,"B");
  effHF->Divide(effHF,nchBase,1,1,"B");
  effL1HF->Divide(effL1HF,nchBase,1,1,"B");

  //TFile* file2=new TFile("ZBfile.root","RECREATE");
  TFile* file2=new TFile(fileManager(4,type,E,0,0,0),"RECREATE");
  file2->cd();
  gDirectory->mkdir("zbeff");
  gDirectory->cd("zbeff");

  nchBase->Write();
  nchL134->Write();
  nchL140->Write();
  nchL1VT->Write();
  nchHF->Write();
 
  effL134->Write();
  effL140->Write();
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


