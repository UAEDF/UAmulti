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

bool isMC = true;
bool debug = false;


#include "fileManager.C"
#include "cuts.C"
#include "NCHEvtSel.C"
#include "binningMap.C"


TString st(string input , int cut){
  stringstream out("");
  out<<cut;
  return input+"_cut"+out.str();
} 

void divideByWidth(TH1F*);


void smallCode(int type = 10 , double E = 0.9 , int iTracking = 1 , int nevt_max = 100){
  
  Bool_t genOnly = true;
  cout << "genOnly set to : " << genOnly << endl;
  if(genOnly)
    cout << "    ==> No Reco level objects will be stored ! " << endl;
  
  if(type==0) isMC = false;
  TString MCtype = "pythia";
  if(type == 20) MCtype = "phojet";
  if(type == 60) MCtype = "pythia8";


  

/*  
  vector< vector<double> > binning;
  binning = getBins(5,E);
*/

/*  
  TH1F* nch_gen_NSD_noSel       = new TH1F("nch_gen_NSD_noSel","nch_gen_NSD_noSel",binning.at(0).size()-1,&binning[0][0]);
  TH1F* nch_gen_NSD_noSel_ptCut = new TH1F("nch_gen_NSD_noSel_ptCut","nch_gen_NSD_noSel_ptCut",binning.at(0).size()-1,&binning[0][0]);
  
  TProfile* mptVSnchreco_reco       = new TProfile("mptVSnchreco_reco","mptVSnchreco_reco",binning.at(0).size()-1,&(binning.at(0).at(0)));
  TProfile* mptVSnchreco_reco_ptCut = new TProfile("mptVSnchreco_reco_ptCut","mptVSnchreco_reco_ptCut",binning.at(0).size()-1,&(binning.at(0).at(0)));
  TProfile* mptVSnchreco_gen        = new TProfile("mptVSnchreco_gen","mptVSnchreco_gen",binning.at(0).size()-1,&(binning.at(0).at(0)));
  TProfile* mptVSnchreco_gen_ptCut  = new TProfile("mptVSnchreco_gen_ptCut","mptVSnchreco_gen_ptCut",binning.at(0).size()-1,&(binning.at(0).at(0)));
  
  
  TProfile* mptVSnchgen_gen       = new TProfile("mptVSnchgen_gen","mptVSnchgen_gen",binning.at(0).size()-1,&(binning.at(0).at(0)));
  TProfile* mptVSnchgen_gen_ptCut = new TProfile("mptVSnchgen_gen_ptCut","mptVSnchgen_gen_ptCut",binning.at(0).size()-1,&(binning.at(0).at(0)));
  //TProfile* mptVSnchgen_reco = new TProfile("mptVSnchgen_reco","mptVSnchgen_reco",binning.at(0).size()-1,&(binning.at(0).at(0)));
  
  TH1F* pt_gen        = new TH1F("pt_gen","pt_gen",200,0,4);
  TH1F* pt_gen_ptCut  = new TH1F("pt_gen_ptCut","pt_gen_ptCut",200,0,4);
  TH1F* pt_reco       = new TH1F("pt_reco","pt_reco",200,0,4);
  TH1F* pt_reco_ptCut = new TH1F("pt_reco_ptCut","pt_reco_ptCut",200,0,4);
*/  

  #include "acceptanceMap.C"
  
  vector<TH1F*>       nch_gen_NSD_noSel(accMap.size(),0);

  vector<TProfile*>   mptVSnchreco_reco(accMap.size(),0);
  vector<TProfile*>   mptVSnchreco_gen(accMap.size(),0);
  vector<TProfile*>   mptVSnchgen_reco(accMap.size(),0);
  vector<TProfile*>   mptVSnchgen_gen(accMap.size(),0);

  vector<TProfile*>   mpt2VSnchreco_reco(accMap.size(),0);
  vector<TProfile*>   mpt2VSnchreco_gen(accMap.size(),0);
  vector<TProfile*>   mpt2VSnchgen_reco(accMap.size(),0);
  vector<TProfile*>   mpt2VSnchgen_gen(accMap.size(),0);

  vector<TH1F*>       pt_gen(accMap.size(),0);
  vector<TH1F*>       pt_reco(accMap.size(),0);

  vector<TH1F*>       pt2_gen(accMap.size(),0);
  vector<TH1F*>       pt2_reco(accMap.size(),0);

  for(int acc = 0 ; acc < (signed) accMap.size() ; ++acc){    
    vector< vector<double> > binning;
    binning = getBins(acc,E); 

    nch_gen_NSD_noSel.at(acc)  = new TH1F(     st("nch_gen_NSD_noSel",acc) , st("nch_gen_NSD_noSel",acc) , binning.at(0).size()-1,&binning[0][0]);

    mptVSnchreco_reco.at(acc)  = new TProfile( st("mptVSnchreco_reco",acc) , st("mptVSnchreco_reco",acc) , binning.at(0).size()-1,&(binning.at(0).at(0)));
    mptVSnchreco_gen.at(acc)   = new TProfile( st("mptVSnchreco_gen",acc)  , st("mptVSnchreco_gen",acc)  , binning.at(0).size()-1,&(binning.at(0).at(0)));
    mptVSnchgen_reco.at(acc)   = new TProfile( st("mptVSnchgen_reco",acc)  , st("mptVSnchgen_reco",acc)  , binning.at(0).size()-1,&(binning.at(0).at(0)));
    mptVSnchgen_gen.at(acc)    = new TProfile( st("mptVSnchgen_gen",acc)   , st("mptVSnchgen_gen",acc)   , binning.at(0).size()-1,&(binning.at(0).at(0)));

    mpt2VSnchreco_reco.at(acc)  = new TProfile( st("mpt2VSnchreco_reco",acc), st("mpt2VSnchreco_reco",acc) , binning.at(0).size()-1,&(binning.at(0).at(0)));
    mpt2VSnchreco_gen.at(acc)   = new TProfile( st("mpt2VSnchreco_gen",acc) , st("mpt2VSnchreco_gen",acc)  , binning.at(0).size()-1,&(binning.at(0).at(0)));
    mpt2VSnchgen_reco.at(acc)    = new TProfile( st("mpt2VSnchgen_reco",acc)  , st("mpt2VSnchgen_reco",acc)   , binning.at(0).size()-1,&(binning.at(0).at(0)));
    mpt2VSnchgen_gen.at(acc)    = new TProfile( st("mpt2VSnchgen_gen",acc)  , st("mpt2VSnchgen_gen",acc)   , binning.at(0).size()-1,&(binning.at(0).at(0)));

    pt_gen.at(acc)             = new TH1F(     st("pt_gen",acc)            , st("pt_gen",acc)            , 200,0,4); 
    pt_reco.at(acc)            = new TH1F(     st("pt_reco_ptCut",acc)     , st("pt_reco_ptCut",acc)     , 200,0,4);

    pt2_gen.at(acc)             = new TH1F(     st("pt2_gen",acc)          , st("pt2_gen",acc)           , 200,0,4); 
    pt2_reco.at(acc)            = new TH1F(     st("pt2_reco_pt2Cut",acc)  , st("pt2_reco_pt2Cut",acc)   , 200,0,4);



    nch_gen_NSD_noSel.at(acc)->Sumw2();   
    pt_gen.at(acc)->Sumw2();  
    pt_reco.at(acc)->Sumw2(); 
    pt2_gen.at(acc)->Sumw2();  
    pt2_reco.at(acc)->Sumw2(); 

  }

  //getting the list of files
//  vector<TString>* vfiles = getListOfFiles(fileManager(0,type,E));
  vector<TString>* vfiles = getListOfFiles("../filelist.txt");

  TTree* tree = new TTree("evt","");
  
  MyEvtId* evtId      = NULL ;
  vector<MyGenPart>* genPart = NULL;
  MyGenKin* genKin = NULL;
  /*vector<MyTracks>*  generalTracks  = NULL;
  vector<MyTracks>*  minbiasTracks  = NULL;
  vector<MyVertex>*  offlinePV  = NULL;
  vector<MyVertex>*  ferencVtx  = NULL;
  vector<MyVertex>*  pixel3Vertex  = NULL;*/
  vector<MyTracks>*  tracks  = NULL;
  vector<MyVertex>*  vertex  = NULL;
  vector<MyVertex>*  vertexToCut  = NULL;
  MyL1Trig*     L1Trig    = NULL;
  MyHLTrig*     HLTrig    = NULL;
  MyMITEvtSel*  MITEvtSel = NULL;
  MyBeamSpot*   bs        = NULL;
  
  
  int i_tot = 0 , nevt_tot = 0;
  //starting Loop over files, stops at end of list of files or when reached nevt_max
  for(vector<TString>::iterator itfiles = vfiles->begin() ; itfiles != vfiles->end() && i_tot < nevt_max ; ++itfiles){

    TFile* file = TFile::Open(*itfiles,"READ");

    //getting the tree form the current file
    tree = (TTree*) file->Get("evt");

    if(isMC) tree->SetBranchAddress("GenPart",&genPart);
    if(isMC) tree->SetBranchAddress("GenKin",&genKin);
    
    if(!genOnly){
      tree->SetBranchAddress("EvtId",&evtId);
      /*tree->SetBranchAddress("generalTracks",&generalTracks); 
      tree->SetBranchAddress("minbiasTracks",&minbiasTracks);
      tree->SetBranchAddress("primaryVertex",&offlinePV);
      //tree->SetBranchAddress("ferencVtxFerTrk",&ferencVtx);*/
      if(iTracking==0){
        tree->SetBranchAddress("generalTracks",&tracks); 
        tree->SetBranchAddress("primaryVertex",&vertex);
      }
      else if(iTracking==1){
        tree->SetBranchAddress("minbiasTracks",&tracks); 
        tree->SetBranchAddress("ferencVtxFerTrk",&vertex);
      }
      
      tree->SetBranchAddress("pixel3Vertex",&vertexToCut);
      /*if(iTracking==0) tree->SetBranchAddress("ferencVtxFerTrk",&vertexToCut);
      else if(iTracking==1) vertexToCut = vertex;*/
      tree->SetBranchAddress("L1Trig",&L1Trig);
      tree->SetBranchAddress("HLTrig",&HLTrig);
      tree->SetBranchAddress("MITEvtSel",&MITEvtSel);
      tree->SetBranchAddress("beamSpot",&bs);
    }
 
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
      if(isMC)
        if(isSD(*genKin , MCtype) ) continue;
   
/* 
      int n_gen_allPt = getnPrimaryGenPart(genPart,0,2.4);
      nch_gen_NSD_noSel->Fill(n_gen_allPt);
      
      int n_gen_ptCut = getnPrimaryGenPart(genPart,0.5,2.4);
      nch_gen_NSD_noSel_ptCut->Fill(n_gen_ptCut);
*/
  
      for(int acc = 0 ; acc < (signed)accMap.size() ; ++acc) {
        nch_gen_NSD_noSel.at(acc)->Fill( getnPrimaryGenPart(genPart,accMap[acc].at(0),accMap[acc].at(1),accMap[acc].at(4)) ); 

        double mpt_gen_noSel = 0.;
        double mpt2_gen_noSel = 0.;
        for(vector<MyGenPart>::iterator it_tr = genPart->begin() ; it_tr != genPart->end() ; ++it_tr)
          if( isGenPartGood(*it_tr) && isInAcceptance(it_tr->Part,accMap[acc].at(0),accMap[acc].at(1),accMap[acc].at(4)) )
          { 
            mpt_gen_noSel  += it_tr->Part.v.Pt();
            mpt2_gen_noSel += pow(it_tr->Part.v.Pt(),2.) ;
          }
        int n_gen_noSel = getnPrimaryGenPart( genPart,accMap[acc].at(0),accMap[acc].at(1),accMap[acc].at(4) );
        if(n_gen_noSel!=0) { mpt_gen_noSel/=n_gen_noSel;  mpt2_gen_noSel/=n_gen_noSel; }
        mptVSnchgen_gen.at(acc)->Fill(n_gen_noSel,mpt_gen_noSel);
        mpt2VSnchgen_gen.at(acc)->Fill(n_gen_noSel,mpt2_gen_noSel);
      }
      
      if(genOnly) continue;
      
      //skipping events that don't pass our event selection
      if(!isEvtGood(E,*L1Trig , *HLTrig, *MITEvtSel , vertexToCut)) continue;
      
      
      //RECO NCH
      for(int acc = 0 ; acc < (signed)accMap.size() ; ++acc) {

        vector<MyTracks> trcoll;
        if(iTracking==0) trcoll = getPrimaryTracks(*tracks,vertex,    accMap[acc].at(2),accMap[acc].at(3),accMap[acc].at(4) );
        if(iTracking==1) trcoll = getPrimaryTracks(*tracks,vertex,bs, accMap[acc].at(2),accMap[acc].at(3),accMap[acc].at(4) );
        int n = trcoll.size();
  
        double mpt_reco = 0;
        double mpt2_reco = 0;
        for(vector<MyTracks>::iterator itr = trcoll.begin() ; itr != trcoll.end() ; ++itr){
          mpt_reco+=itr->Part.v.Pt();
          mpt2_reco+= pow(itr->Part.v.Pt(),2.);
          pt_reco.at(acc)->Fill(itr->Part.v.Pt());
          pt2_reco.at(acc)->Fill(pow(itr->Part.v.Pt(),2.));
        }
        if(n!=0) { mpt_reco/=n; mpt2_reco/=n; }
        mptVSnchreco_reco.at(acc)->Fill(n,mpt_reco);
        mpt2VSnchreco_reco.at(acc)->Fill(n,mpt2_reco);
        
        double mpt_gen = 0;
        double mpt2_gen = 0;
        for(vector<MyGenPart>::iterator it_tr = genPart->begin() ; it_tr != genPart->end() ; ++it_tr){
          if( isGenPartGood(*it_tr) && isInAcceptance( it_tr->Part,accMap[acc].at(0),accMap[acc].at(1),accMap[acc].at(4) ) ){
            mpt_gen += it_tr->Part.v.Pt();
            mpt2_gen += pow(it_tr->Part.v.Pt(),2.);
            pt_gen.at(acc)->Fill(it_tr->Part.v.Pt());
            pt2_gen.at(acc)->Fill(pow(it_tr->Part.v.Pt(),2.));
          }
        }
        int n_gen = getnPrimaryGenPart( genPart,accMap[acc].at(0),accMap[acc].at(1),accMap[acc].at(4) );
        if(n_gen!=0) { mpt_gen/=n_gen; mpt2_gen/=n_gen; }
        mptVSnchreco_gen.at(acc)->Fill(n,mpt_gen);
        mpt2VSnchreco_gen.at(acc)->Fill(n,mpt2_gen);

        mptVSnchgen_reco.at(acc)->Fill(n_gen,mpt_reco);
        mpt2VSnchgen_reco.at(acc)->Fill(n_gen,mpt2_reco);
      } 
        
      //mptrecoVSgen->Fill(mpt_gen,mpt_reco);
      
      
    }//end of loop over events

    //Closing current files
    file->Close();

  }//end of loop over files

  
  //output file
  ostringstream strout("");
  strout<<"smallCode_MCtype"<<type<<"_"<<E<<"TeV.root";
  TFile* output = new TFile(strout.str().c_str(),"RECREATE");
  output->cd();
  
  gStyle->SetPalette(1);
  //mptrecoVSgen->Draw("colz");
  
  for(int acc = 0 ; acc < (signed)accMap.size() ; ++acc) {
  
    divideByWidth(nch_gen_NSD_noSel.at(acc));
    nch_gen_NSD_noSel.at(acc)->Write();
    mptVSnchgen_gen.at(acc)->Write();
    mpt2VSnchgen_gen.at(acc)->Write();
    
    if(genOnly) continue;
    
    mptVSnchreco_reco.at(acc)->Write();
    mptVSnchreco_gen.at(acc)->Write();
    mptVSnchgen_reco.at(acc)->Write();

    mpt2VSnchreco_reco.at(acc)->Write();
    mpt2VSnchreco_gen.at(acc)->Write();
    mpt2VSnchgen_reco.at(acc)->Write();
    
    mptVSnchreco_gen.at(acc)->Divide(mptVSnchreco_gen.at(acc),mptVSnchreco_reco.at(acc),1.,1.,"B");
    mptVSnchreco_gen.at(acc)->Write(st("eff_mtpVSnchreco_genOreco",acc));

    mptVSnchgen_gen.at(acc)->Divide(mptVSnchgen_gen.at(acc),mptVSnchgen_reco.at(acc),1.,1.,"B");
    mptVSnchgen_gen.at(acc)->Write(st("eff_mtpVSnchgen_genOreco",acc));
    
    mpt2VSnchreco_gen.at(acc)->Divide(mpt2VSnchreco_gen.at(acc),mpt2VSnchreco_reco.at(acc),1.,1.,"B");
    mpt2VSnchreco_gen.at(acc)->Write(st("eff_mtp2VSnchreco_genOreco",acc));
    
    mpt2VSnchgen_gen.at(acc)->Divide(mpt2VSnchgen_gen.at(acc),mpt2VSnchgen_reco.at(acc),1.,1.,"B");
    mpt2VSnchgen_gen.at(acc)->Write(st("eff_mtp2VSnchgen_genOreco",acc));

    pt_gen.at(acc)->Write();
    pt_reco.at(acc)->Write();
    pt_gen.at(acc)->Add(pt_reco.at(acc),-1);
    pt_gen.at(acc)->Write(st("pt_genMINUSreco",acc));

    pt2_gen.at(acc)->Write();
    pt2_reco.at(acc)->Write();
    pt2_gen.at(acc)->Add(pt2_reco.at(acc),-1);
    pt2_gen.at(acc)->Write(st("pt2_genMINUSreco",acc));

  }
  
  output->Close();
}

void divideByWidth(TH1F* hist){
  for(int i=1;i<=hist->GetNbinsX();++i){
    hist->SetBinContent(i,double(hist->GetBinContent(i))/double(hist->GetBinWidth(i)));
    hist->SetBinError(i,double(hist->GetBinError(i))/double(hist->GetBinWidth(i)));
  }
}



