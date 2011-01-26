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
#include "../plugins/FBCorrelwCorr.h"

bool isMC  = true;
bool debug = 0;


#include "FBacc.C"
#include "fileManager.C"
#include "FBcuts.C"
#include "NCHEvtSel.C"
#include "binningMap.C"

TString st(string input , int cut){
  stringstream out("");
  out<<cut;
  return input+"_cut"+out.str();
}

//nohup root -b -l BuildLibDico.C+ NCHrunCheck.C+"(0,7,1,100000000)" -q   > log_nchruncheck_type0_7TeV.txt &

void FBana(int type = 60 , double E = 7.0 , int iTracking = 1 , int nevt_max = 10 , int version = 0.1){
  
  TH1::AddDirectory(0);

  TTProfile::switchTH1On      = 1;
  TTProfile::switchTH2On      = 1;
  TTProfile::debug            = debug;
  FBCorrel::debug             = debug;
  FBCorrelwCorr::xCorrOn      = 0;
  FBCorrelwCorr::xyCorrOn     = 1;
  FBCorrelwCorr::trReweightOn = 1;
  
  if(debug) cout << "Getting the acceptance map ... " << endl;
  getAccMap();
  
  if(debug) cout << "Initializing classes ... " << endl;
  vector<FBCorrelwCorr*> fbcwc_INEL_evtSel_reco  (accMap.size(),0);
  vector<FBCorrel*> fbc_INEL_evtSel_reco         (accMap.size(),0);
  vector<FBCorrel*> fbc_INEL_evtSel_reco_mnMVSnP (accMap.size(),0);
  vector<FBCorrel*> fbc_INEL_evtSel_gen          (accMap.size(),0);
  vector<FBCorrel*> fbc_INEL_noEvtSel_gen        (accMap.size(),0);
  vector<FBCorrel*> fbc_INEL_noEvtSel_gen_mnMVSnP(accMap.size(),0);
  vector<FBCorrel*> fbc_noEvtSel_gen            (accMap.size(),0);
  
  vector<TH1F*> diffWeightM (accMap.size(),0);
  vector<TH1F*> diffWeightP (accMap.size(),0);
  
  vector<TH1F*> diff_diffWeight(accMap.size(),0);
  
  
  for(int acc = 0 ; acc < (signed) accMap.size() ; ++acc){
    if(debug) cout << "    ==> acceptance bin " << acc << endl;
    //vector< vector<double> > binning;
    //binning = getBins(1,0,1);//nch,pt,eta
    //binning = getBins(acc,Ebinning);//nch,pt,eta
    //baseplot->setBinning(binning);

   //----------------------  RECO  ----------------------
    fbcwc_INEL_evtSel_reco.at(acc)        = new FBCorrelwCorr(st("INEL_evtSel_reco",acc));
    fbc_INEL_evtSel_reco.at(acc)          = new FBCorrel(st("INEL_evtSel_reco",acc));
    fbc_INEL_evtSel_reco_mnMVSnP.at(acc)  = new FBCorrel(st("INEL_evtSel_reco_mnMVSnP",acc));
    fbc_INEL_evtSel_gen.at(acc)           = new FBCorrel(st("INEL_evtSel_gen",acc));
    fbc_INEL_noEvtSel_gen.at(acc)         = new FBCorrel(st("INEL_noEvtSel_gen",acc));
    fbc_INEL_noEvtSel_gen_mnMVSnP.at(acc) = new FBCorrel(st("INEL_noEvtSel_gen_mnMVSnP",acc));
    fbc_noEvtSel_gen.at(acc)             = new FBCorrel(st("noEvtSel_gen",acc));
    
    diffWeightM.at(acc) = new TH1F(st("diffWeightM",acc) , st("diffWeightM",acc) + ";(nTrReweight - nTr) / nTr; nEvts" , 600 , -3 , 3 );
    diffWeightP.at(acc) = new TH1F(st("diffWeightP",acc) , st("diffWeightP",acc) + ";(nTrReweight - nTr) / nTr; nEvts" , 600 , -3 , 3 );
    
    diff_diffWeight.at(acc) = new TH1F(st("diff_diffWeight",acc) , st("diff_diffWeight",acc) + ";#sigma_{f} - #sigma_{b}; nEvts" , 1000 , -2 , 2 );
  }
  if(debug) cout << "                          ... Done !!";
  
  
  //Getting the trm for the corrections
  for(int acc = 0 ; acc < (signed) accMap.size() ; ++acc){
    fbcwc_INEL_evtSel_reco.at(acc)->GetTRMFromFile("FBtrm1_type31_7TeV.root" , st("NSD_evtSel_nM",acc) , st("NSD_evtSel_nP",acc) );
  }
  
  //Getting the 2D Eff for the corrections
  for(int acc = 0 ; acc < (signed) accMap.size() ; ++acc){
    //fbcwc_INEL_evtSel_reco.at(acc)->GetTrEffFromFile("NCHtest1_type31_7TeV_mbTr.root" , "eff_ptVSeta_INEL_evtSel_matched" , "eff_ptVSeta_INEL_evtSel_fakes" );
    fbcwc_INEL_evtSel_reco.at(acc)->GetTrEffFromFile("NCHtest4_type60_7TeV_mbTr.root" , "eff_ptVSeta_INEL_evtSel_matched" , "eff_ptVSeta_INEL_evtSel_fakes" );
  }
  
  
  
  
  if(type==0) isMC = false;
  TString MCtype = "pythia";
  if(type == 20) MCtype = "phojet";
  if(type == 60 || type==101) MCtype = "pythia8";

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
    tree->SetBranchAddress("pixel3Vertex",&vertexToCut);
    tree->SetBranchAddress("L1Trig",&L1Trig);
    tree->SetBranchAddress("HLTrig",&HLTrig);
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

      //if(i>min(nev,nevt_max)) break;

      //Filling the variables defined setting branches
      tree->GetEntry(i);

  
      //Selection of good BX for data && MC
      //==> Not Needed anymore for 36X files
      //if(!isMC && !goodBX(evtId->Run,evtId->Bunch)) continue;

      vector<MyGenPart> gpcoll;
      pair<double,double> nCorrelGp = make_pair(0,0);
      if(isMC){
        gpcoll = *genPart;
        getPrimaryGenPart(gpcoll);
      
        //Filling Inelastic Gen Level
        for(int acc = 0 ; acc < (signed) accMap.size() ; ++acc){
          nCorrelGp = getnCorrel(gpcoll , accMap.at(acc));
          fbc_noEvtSel_gen.at(acc)->Fill(nCorrelGp.first , nCorrelGp.second);
        }
      
        //Skipping the SD events starting from here
        //==> Not Needed for INEL
	//if(isSD(*genKin , MCtype) ) continue;
      
        //Filling NSD Gen Level
        for(int acc = 0 ; acc < (signed) accMap.size() ; ++acc){
          nCorrelGp = getnCorrel(gpcoll , accMap.at(acc));
          fbc_INEL_noEvtSel_gen.at(acc)->Fill(nCorrelGp.first , nCorrelGp.second);
          fbc_INEL_noEvtSel_gen_mnMVSnP.at(acc)->Fill(nCorrelGp.second , nCorrelGp.first);
        }
      }  
      
      
      
      vector<MyTracks> trcoll = *tracks;
      if(iTracking==0) getPrimaryTracks(trcoll,vertex);
      if(iTracking==1) getPrimaryTracks(trcoll,vertex,bs);
      
      //skipping events that don't pass our event selection
      //if(!isEvtGood(E,*L1Trig , *MITEvtSel , vertexToCut)) continue;
      //if( ! ( ( (passL1HLT(E, *L1Trig, *HLTrig , 1 , isMC ) && tracks->size()<110) || (passL1HLT(E, *L1Trig, *HLTrig , 2 , isMC ) && tracks->size()>=110) ) &&
      if( ! ( passL1HLT(E, *L1Trig, *HLTrig , 3 , isMC) &&
	      passVtxQual(*MITEvtSel,E)                 &&
	      passVtx(*vertex) )
	 )
	 continue;
    
    
    
      //Filling NSD Gen Level AFTER evtSel
      if(isMC){
        for(int acc = 0 ; acc < (signed) accMap.size() ; ++acc){
          nCorrelGp = getnCorrel(gpcoll , accMap.at(acc));
          fbc_INEL_evtSel_gen.at(acc)->Fill(nCorrelGp.first , nCorrelGp.second);
        }
      }
    
      pair<double,double> nCorrelTr         = make_pair(0,0);
      pair<double,double> nCorrelTrReweight = make_pair(0,0);
      for(int acc = 0 ; acc < (signed) accMap.size() ; ++acc){
        nCorrelTr = getnCorrel(trcoll , accMap.at(acc));
        fbcwc_INEL_evtSel_reco.at(acc)->Fill(nCorrelTr.first , nCorrelTr.second);
        fbc_INEL_evtSel_reco.at(acc)->Fill(nCorrelTr.first , nCorrelTr.second);
        fbc_INEL_evtSel_reco_mnMVSnP.at(acc)->Fill(nCorrelTr.second , nCorrelTr.first);
      
        nCorrelTrReweight = getnCorrelTrReweight(trcoll , accMap.at(acc) , fbcwc_INEL_evtSel_reco.at(acc)->trEff , fbcwc_INEL_evtSel_reco.at(acc)->trFakes);
        fbcwc_INEL_evtSel_reco.at(acc)->FillTrReweight(nCorrelTrReweight.first , nCorrelTrReweight.second);
      
        if(nCorrelTr.first>0)
          diffWeightM.at(acc)->Fill( (nCorrelTrReweight.first - nCorrelTr.first) / nCorrelTr.first);
        if(nCorrelTr.second>0)
          diffWeightP.at(acc)->Fill( (nCorrelTrReweight.second - nCorrelTr.second) / nCorrelTr.second);
	
	if(nCorrelTr.first>0 && nCorrelTr.second>0)
	  diff_diffWeight.at(acc)->Fill(  (nCorrelTrReweight.second - nCorrelTr.second) / nCorrelTr.second - (nCorrelTrReweight.first - nCorrelTr.first) / nCorrelTr.first ); 
      }
    
    }//end of loop over events
    
    //Closing current files
    file->Close();

  }//end of loop over files

  
  
  //output file
  ostringstream strout("FBtest");
  strout<<"FBtest" << version << "_type"<<type<<"_"<<E<<"TeV.root";
  cout << "Output File : " << strout.str() << endl; 
  TFile* output = new TFile(strout.str().c_str(),"RECREATE");
  output->cd();
  
  for(int acc = 0 ; acc < (signed) accMap.size() ; ++acc){
    //if(isMC) fbc_INEL_noEvtSel_gen.at(acc)->ttp.write(1,1);
    //fbc_INEL_evtSel_reco.at(acc)->ttp.write(1,1);
    
    fbcwc_INEL_evtSel_reco.at(acc)->write();
    fbc_INEL_evtSel_reco.at(acc)->write();
    fbc_INEL_evtSel_reco_mnMVSnP.at(acc)->write();
    if(isMC){
      fbc_INEL_evtSel_gen.at(acc)->write();
      fbc_INEL_noEvtSel_gen.at(acc)->write();
      fbc_INEL_noEvtSel_gen_mnMVSnP.at(acc)->write();
    }
    
    
    diffWeightM.at(acc)->Write();
    diffWeightP.at(acc)->Write();
    
    diff_diffWeight.at(acc)->Write();
  }
  output->Close();
}
