#include <TROOT.h>
#include <TH1.h>
#include <TH2.h>
#include <TGraphAsymmErrors.h>
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

bool isMC = true;
bool debug = false;


#include "fileManager.C"
#include "cuts.C"
#include "evtSel.C"
#include "binningMap.C"

void multiplyByWidth(TH1F*);
void multiplyByWidth(TH2F*);

TString st(string input , int cut){
  stringstream out("");
  out<<cut;
  return input+"_cut"+out.str();
} 

bool accNoSkip(int acc){
  if ( acc >= 5 && acc <= 9 ) return true;
  if ( acc >=15 && acc <=19 ) return true;
  return false;
} 


void mptCorrection(int type = 10 , double E = 0.9 , int iTracking = 1 , int nevt_max = 100 , int mctype = 10 ,
		   int syst = 0 , int syst_sign = 0 , bool multbyWidth = true){
  
  
  if(type==0) isMC = false;
  TString MCtype = "pythia";
  if(mctype == 20) MCtype = "phojet";
  

  #include "acceptanceMap.C"
  
  TString filedir("../plots/systv12/");
  
  
  //**********************************************
  //Taking mtx/histos from smallCode files
  
  ostringstream fname("");
  //fname<<"../plots/smallcodev4/smallCode_MCtype"<<mctype<<"_"<<E<<"TeV.root";
  fname<<"../plots/smallcodev6/smallCode_MCtype"<<mctype<<"_"<<E<<"TeV.root";
  //fname<<"../macro/smallCode_MCtype"<<mctype<<"_"<<E<<"TeV.root";
  cout<<"smallCode file : "<<fname.str()<<endl;
  TFile* smallcode = TFile::Open(fname.str().c_str(),"READ");
  
  if(smallcode==0){
    cout<<"Couldn't open file "<<smallcode<<" . Exiting ..."<<endl;
    return;
  }

  vector<TProfile*> mptVSnchreco_reco(accMap.size(),0);
  vector<TProfile*> mptVSnchreco_gen(accMap.size(),0);
  vector<TProfile*> mptVSnchgen_reco(accMap.size(),0);
  vector<TProfile*> mptVSnchgen_gen(accMap.size(),0);
  vector< TH1F* >   pt_genMINUSreco(accMap.size(),0);

  vector<TProfile*> mpt2VSnchreco_reco(accMap.size(),0);
  vector<TProfile*> mpt2VSnchreco_gen(accMap.size(),0);
  vector<TProfile*> mpt2VSnchgen_reco(accMap.size(),0);
  vector<TProfile*> mpt2VSnchgen_gen(accMap.size(),0);
  vector< TH1F* >   pt2_genMINUSreco(accMap.size(),0);

  for(int acc = 0 ; acc < (signed) accMap.size() ; ++acc){   
    if (accNoSkip(acc)){   
      mptVSnchreco_reco.at(acc)  = (TProfile*) smallcode->Get(st("mptVSnchreco_reco",acc));
      mptVSnchreco_gen.at(acc)   = (TProfile*) smallcode->Get(st("mptVSnchreco_gen",acc));
      mptVSnchgen_reco.at(acc)   = (TProfile*) smallcode->Get(st("mptVSnchgen_reco",acc));
      mptVSnchgen_gen.at(acc)    = (TProfile*) smallcode->Get(st("mptVSnchgen_gen",acc));
      pt_genMINUSreco.at(acc)    = (TH1F*)     smallcode->Get(st("pt_genMINUSreco",acc));
      
      mpt2VSnchreco_reco.at(acc) = (TProfile*) smallcode->Get(st("mpt2VSnchreco_reco",acc));
      mpt2VSnchreco_gen.at(acc)  = (TProfile*) smallcode->Get(st("mpt2VSnchreco_gen",acc));
      mpt2VSnchgen_reco.at(acc) = (TProfile*) smallcode->Get(st("mpt2VSnchgen_reco",acc));
      mpt2VSnchgen_gen.at(acc)  = (TProfile*) smallcode->Get(st("mpt2VSnchgen_gen",acc));
      pt2_genMINUSreco.at(acc)   = (TH1F*)     smallcode->Get(st("pt2_genMINUSreco",acc));
    }
  }
  
  //**********************************************
  //Taking mtx/histos from simpleana files
   
  vector<TFile*> unfoldfile(accMap.size(),0);
  vector<TH2F*>  matrixhist(accMap.size(),0);
  vector<TH1F*>  nch_data_unfolded(accMap.size(),0);
  vector<TH1F*>  nch_data_NSD_afterSDsub(accMap.size(),0);
  vector<TH1F*>  eff_evtSel(accMap.size(),0);

  for(int acc = 0 ; acc < (signed) accMap.size() ; ++acc){  
   if (accNoSkip(acc)){  
    ostringstream outstr("");
    outstr << "hyp1_niter0_cut" << acc << "_DataType" << type;
    TString sunfoldfile = fileManager(3,mctype,E,iTracking,syst,syst_sign,outstr.str(),filedir);
    cout<<"Unfolding file : "<<sunfoldfile<<endl;
    unfoldfile.at(acc) = TFile::Open(sunfoldfile,"READ");
  
    if(unfoldfile.at(acc)==0){
      cout<<"Couldn't open file "<<sunfoldfile<<" . Exiting ..."<<endl;
      return;
    }
  
  
    //Get Matrix
    matrixhist.at(acc) = (TH2F*) unfoldfile.at(acc)->Get("unfolding/nch_matrix");
    if(multbyWidth) multiplyByWidth(matrixhist.at(acc));
  
    nch_data_unfolded.at(acc) = (TH1F*) unfoldfile.at(acc)->Get("unfolding/nch_unfoldedPtr");
    if(multbyWidth) multiplyByWidth(nch_data_unfolded.at(acc));
    nch_data_unfolded.at(acc)->Scale(1./(nch_data_unfolded.at(acc)->Integral()));
  
    TH1D* projX = (TH1D*) (matrixhist.at(acc)->ProjectionX());
  
    for(int igen = 1 ; igen <= matrixhist.at(acc)->GetNbinsX() ; ++igen){
      double factor = 1;
      if(projX->GetBinContent(igen)!=0)
        factor = nch_data_unfolded.at(acc)->GetBinContent(igen) / projX->GetBinContent(igen);
    
      for(int ireco = 1 ; ireco <= matrixhist.at(acc)->GetNbinsY() ; ++ireco){
        matrixhist.at(acc)->SetBinContent(igen , ireco ,  matrixhist.at(acc)->GetBinContent(igen , ireco) * factor );
      }
    }
  
  

    nch_data_unfolded.at(acc)->Draw();
    projX = (TH1D*) matrixhist.at(acc)->ProjectionX();
    projX->Scale(1./projX->Integral());
    projX->SetLineColor(kRed);
    projX->Draw("samehist");
  
    //gPad->WaitPrimitive();
  
    nch_data_NSD_afterSDsub.at(acc) = (TH1F*) unfoldfile.at(acc)->Get("unfolding/nch_data_NSD_afterSDsub");
    if(multbyWidth) multiplyByWidth(nch_data_NSD_afterSDsub.at(acc));
    nch_data_NSD_afterSDsub.at(acc)->Scale(1./nch_data_NSD_afterSDsub.at(acc)->Integral());
    nch_data_NSD_afterSDsub.at(acc)->Draw();
  
    TH1D* projY = (TH1D*) matrixhist.at(acc)->ProjectionY();
    projY->Scale(1./projY->Integral());
    projY->SetLineColor(kRed);
    projY->Draw("samehist");
    gPad->Update();
  
    //gPad->WaitPrimitive();
  
    eff_evtSel.at(acc) = (TH1F*) unfoldfile.at(acc)->Get("unfolding/eff_evtSel");
   }
  }

  // *****************************************************
  // histo to create/fill 


  vector<TProfile*> mptVSnch(accMap.size(),0);
  vector<TProfile*> mptVSnch_noCorr(accMap.size(),0);
  vector<TProfile*> mptVSnch_nonchCorr(accMap.size(),0);
  vector<TProfile*> mptVSnch_nomptCorr(accMap.size(),0);
  vector<TProfile*> mptVSnch_noeffCorr(accMap.size(),0);
  vector<TProfile*> mptVSnch_nchCorr(accMap.size(),0);
  vector<TProfile*> mptVSnch_nch_mptCorr(accMap.size(),0);
  vector<TProfile*> mptVSnch_gen(accMap.size(),0);
  vector<TH2F*>     mptrecoVSgen(accMap.size(),0);
  vector<TH1F*>     mptrecoVSgen_diff(accMap.size(),0);
  vector<TProfile*> systp_trk(accMap.size(),0);
  vector<TProfile*> systm_trk(accMap.size(),0);
  vector<TProfile*> systp_pterr(accMap.size(),0);
  vector<TProfile*> systm_pterr(accMap.size(),0);
  
  vector<TProfile*> mpt2VSnch(accMap.size(),0);
  vector<TProfile*> mpt2VSnch_noCorr(accMap.size(),0);
  vector<TProfile*> mpt2VSnch_nonchCorr(accMap.size(),0);
  vector<TProfile*> mpt2VSnch_nomptCorr(accMap.size(),0);
  vector<TProfile*> mpt2VSnch_noeffCorr(accMap.size(),0);
  vector<TProfile*> mpt2VSnch_nchCorr(accMap.size(),0);
  vector<TProfile*> mpt2VSnch_nch_mptCorr(accMap.size(),0);
  vector<TProfile*> mpt2VSnch_gen(accMap.size(),0);
  vector<TH2F*>     mpt2recoVSgen(accMap.size(),0);
  vector<TH1F*>     mpt2recoVSgen_diff(accMap.size(),0);
  vector<TProfile*> syst2p_trk(accMap.size(),0);
  vector<TProfile*> syst2m_trk(accMap.size(),0);
  vector<TProfile*> syst2p_pterr(accMap.size(),0);
  vector<TProfile*> syst2m_pterr(accMap.size(),0);

  for(int acc = 0 ; acc < (signed) accMap.size() ; ++acc) {
   if (accNoSkip(acc)){  
  
    mptVSnch.at(acc) = new TProfile(st("mptVSnch",acc),st("mptVSnch",acc),nch_data_unfolded.at(acc)->GetNbinsX(),nch_data_unfolded.at(acc)->GetXaxis()->GetXbins()->GetArray());
    mptVSnch.at(acc)->SetErrorOption("i");
    
    mptVSnch_noCorr.at(acc) = new TProfile(st("mptVSnch_noCorr",acc),st("mptVSnch_noCorr",acc),nch_data_unfolded.at(acc)->GetNbinsX(),nch_data_unfolded.at(acc)->GetXaxis()->GetXbins()->GetArray());
    mptVSnch_noCorr.at(acc)->SetErrorOption("i");
    
    mptVSnch_nonchCorr.at(acc) = new TProfile(st("mptVSnch_nonchCorr",acc),st("mptVSnch_nonchCorr",acc),nch_data_unfolded.at(acc)->GetNbinsX(),nch_data_unfolded.at(acc)->GetXaxis()->GetXbins()->GetArray());
    mptVSnch_nonchCorr.at(acc)->SetErrorOption("i");
    
    mptVSnch_nomptCorr.at(acc) = new TProfile(st("mptVSnch_nomptCorr",acc),st("mptVSnch_nomptCorr",acc),nch_data_unfolded.at(acc)->GetNbinsX(),nch_data_unfolded.at(acc)->GetXaxis()->GetXbins()->GetArray());
    mptVSnch_nomptCorr.at(acc)->SetErrorOption("i");
    
    mptVSnch_noeffCorr.at(acc) = new TProfile(st("mptVSnch_noeffCorr",acc),st("mptVSnch_noeffCorr",acc),nch_data_unfolded.at(acc)->GetNbinsX(),nch_data_unfolded.at(acc)->GetXaxis()->GetXbins()->GetArray());
    mptVSnch_noeffCorr.at(acc)->SetErrorOption("i");
    
    mptVSnch_nchCorr.at(acc) = new TProfile(st("mptVSnch_nchCorr",acc),st("mptVSnch_nchCorr",acc),nch_data_unfolded.at(acc)->GetNbinsX(),nch_data_unfolded.at(acc)->GetXaxis()->GetXbins()->GetArray());
    mptVSnch_nchCorr.at(acc)->SetErrorOption("i");
    
    mptVSnch_nch_mptCorr.at(acc) = new TProfile(st("mptVSnch_nch_mptCorr",acc),st("mptVSnch_nch_mptCorr",acc),nch_data_unfolded.at(acc)->GetNbinsX(),nch_data_unfolded.at(acc)->GetXaxis()->GetXbins()->GetArray());
    mptVSnch_nch_mptCorr.at(acc)->SetErrorOption("i");
      
    mptVSnch_gen.at(acc) = new TProfile(st("mptVSnch_gen",acc),st("mptVSnch_gen",acc),nch_data_unfolded.at(acc)->GetNbinsX(),nch_data_unfolded.at(acc)->GetXaxis()->GetXbins()->GetArray());
    mptVSnch_gen.at(acc)->SetErrorOption("i");
    
    mptrecoVSgen.at(acc) = new TH2F(st("mptrecoVSgen",acc),st("mptrecoVSgen",acc),100,0,3,100,0,3);
    mptrecoVSgen_diff.at(acc) = new TH1F(st("mptrecoVSgen_diff",acc),st("mptrecoVSgen_diff",acc),200,-3,3);
    
    systp_trk.at(acc) =  new TProfile(st("systp_trk",acc),st("systp_trk",acc),nch_data_unfolded.at(acc)->GetNbinsX(),nch_data_unfolded.at(acc)->GetXaxis()->GetXbins()->GetArray());
    systm_trk.at(acc) =  new TProfile(st("systm_trk",acc),st("systm_trk",acc),nch_data_unfolded.at(acc)->GetNbinsX(),nch_data_unfolded.at(acc)->GetXaxis()->GetXbins()->GetArray());
    
    systp_pterr.at(acc) =  new TProfile(st("systp_pterr",acc),st("systp_pterr",acc),nch_data_unfolded.at(acc)->GetNbinsX(),nch_data_unfolded.at(acc)->GetXaxis()->GetXbins()->GetArray());
    systm_pterr.at(acc) =  new TProfile(st("systm_pterr",acc),st("systm_pterr",acc),nch_data_unfolded.at(acc)->GetNbinsX(),nch_data_unfolded.at(acc)->GetXaxis()->GetXbins()->GetArray());

    // and pt2

    mpt2VSnch.at(acc) = new TProfile(st("mpt2VSnch",acc),st("mpt2VSnch",acc),nch_data_unfolded.at(acc)->GetNbinsX(),nch_data_unfolded.at(acc)->GetXaxis()->GetXbins()->GetArray());
    mpt2VSnch.at(acc)->SetErrorOption("i");
    
    mpt2VSnch_noCorr.at(acc) = new TProfile(st("mpt2VSnch_noCorr",acc),st("mpt2VSnch_noCorr",acc),nch_data_unfolded.at(acc)->GetNbinsX(),nch_data_unfolded.at(acc)->GetXaxis()->GetXbins()->GetArray());
    mpt2VSnch_noCorr.at(acc)->SetErrorOption("i");
    
    mpt2VSnch_nonchCorr.at(acc) = new TProfile(st("mpt2VSnch_nonchCorr",acc),st("mpt2VSnch_nonchCorr",acc),nch_data_unfolded.at(acc)->GetNbinsX(),nch_data_unfolded.at(acc)->GetXaxis()->GetXbins()->GetArray());
    mpt2VSnch_nonchCorr.at(acc)->SetErrorOption("i");
    
    mpt2VSnch_nomptCorr.at(acc) = new TProfile(st("mpt2VSnch_nomptCorr",acc),st("mpt2VSnch_nomptCorr",acc),nch_data_unfolded.at(acc)->GetNbinsX(),nch_data_unfolded.at(acc)->GetXaxis()->GetXbins()->GetArray());
    mpt2VSnch_nomptCorr.at(acc)->SetErrorOption("i");
    
    mpt2VSnch_noeffCorr.at(acc) = new TProfile(st("mpt2VSnch_noeffCorr",acc),st("mpt2VSnch_noeffCorr",acc),nch_data_unfolded.at(acc)->GetNbinsX(),nch_data_unfolded.at(acc)->GetXaxis()->GetXbins()->GetArray());
    mpt2VSnch_noeffCorr.at(acc)->SetErrorOption("i");
    
    mpt2VSnch_nchCorr.at(acc) = new TProfile(st("mpt2VSnch_nchCorr",acc),st("mpt2VSnch_nchCorr",acc),nch_data_unfolded.at(acc)->GetNbinsX(),nch_data_unfolded.at(acc)->GetXaxis()->GetXbins()->GetArray());
    mpt2VSnch_nchCorr.at(acc)->SetErrorOption("i");
    
    mpt2VSnch_nch_mptCorr.at(acc) = new TProfile(st("mpt2VSnch_nch_mptCorr",acc),st("mpt2VSnch_nch_mptCorr",acc),nch_data_unfolded.at(acc)->GetNbinsX(),nch_data_unfolded.at(acc)->GetXaxis()->GetXbins()->GetArray());
    mpt2VSnch_nch_mptCorr.at(acc)->SetErrorOption("i");
      
    mpt2VSnch_gen.at(acc) = new TProfile(st("mpt2VSnch_gen",acc),st("mpt2VSnch_gen",acc),nch_data_unfolded.at(acc)->GetNbinsX(),nch_data_unfolded.at(acc)->GetXaxis()->GetXbins()->GetArray());
    mpt2VSnch_gen.at(acc)->SetErrorOption("i");
    
    mpt2recoVSgen.at(acc) = new TH2F(st("mpt2recoVSgen",acc),st("mpt2recoVSgen",acc),100,0,3,100,0,3);
    mpt2recoVSgen_diff.at(acc) = new TH1F(st("mpt2recoVSgen_diff",acc),st("mpt2recoVSgen_diff",acc),200,-3,3);
    
    syst2p_trk.at(acc) =  new TProfile(st("syst2p_trk",acc),st("syst2p_trk",acc),nch_data_unfolded.at(acc)->GetNbinsX(),nch_data_unfolded.at(acc)->GetXaxis()->GetXbins()->GetArray());
    syst2m_trk.at(acc) =  new TProfile(st("syst2m_trk",acc),st("syst2m_trk",acc),nch_data_unfolded.at(acc)->GetNbinsX(),nch_data_unfolded.at(acc)->GetXaxis()->GetXbins()->GetArray());
    
    syst2p_pterr.at(acc) =  new TProfile(st("syst2p_pterr",acc),st("syst2p_pterr",acc),nch_data_unfolded.at(acc)->GetNbinsX(),nch_data_unfolded.at(acc)->GetXaxis()->GetXbins()->GetArray());
    syst2m_pterr.at(acc) =  new TProfile(st("syst2m_pterr",acc),st("syst2m_pterr",acc),nch_data_unfolded.at(acc)->GetNbinsX(),nch_data_unfolded.at(acc)->GetXaxis()->GetXbins()->GetArray());


   } 
  }  
 
 
  //TProfile* tmp = new TProfile("tmp","tmp",nch_data_unfolded->GetNbinsX(),nch_data_unfolded->GetXaxis()->GetXbins()->GetArray());

  // ********************************* Start looping on data **************************
  
  //adding files to the tree chain
  TChain* tree = new TChain("evt","");
  tree->Add(fileManager(0,type,E));
  
  
  //adding branches to the tree
  
  MyEvtId* evtId      = NULL ;
  tree->SetBranchAddress("EvtId",&evtId);
  
  vector<MyGenPart>* genPart = NULL;
  if(isMC) tree->SetBranchAddress("GenPart",&genPart);
  
  MyGenKin* genKin = NULL;
  if(isMC) tree->SetBranchAddress("GenKin",&genKin);
  
  vector<MyTracks>*  tracks  = NULL;
  vector<MyVertex>*  vertex  = NULL;
  vector<MyVertex>*  vertexToCut  = NULL;
  MyL1Trig*     L1Trig    = NULL;
  MyMITEvtSel*  MITEvtSel = NULL;
  MyBeamSpot*   bs        = NULL;
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
  tree->SetBranchAddress("MITEvtSel",&MITEvtSel);
  tree->SetBranchAddress("beamSpot",&bs);
  
  //Getting number of events
  int nev = int(tree->GetEntries());
  std::cout <<"number of entries is : "<< nev << std::endl;
  cout<<"Running on: "<<min(nev,nevt_max)<<" events"<<endl;
  // Event TYPE counting --> Weights
  
  //starting loop over events
  for(int i = 0; i < nev; i++){
     
    if( ((i+1) % 10000) == 0) cout <<int(double(i+1)/double(min(nev,nevt_max))*100.)<<" % done"<<endl;
    
    if(i>min(nev,nevt_max)) break;
    
    tree->GetEntry(i);
   
    if(i==0) vertexToCut = vertex;

 
    //Skipping the SD events starting from here
    if(isMC)
      if(isSD(genKin , MCtype) ) continue;
    if(isMC){
      for(int acc = 0 ; acc < (signed)accMap.size() ; ++acc) {
       if (accNoSkip(acc)){  
        double mpt_gen_noSel = 0;
        double mpt2_gen_noSel = 0;
        int n_gen_noSel = 0;
        for(vector<MyGenPart>::iterator it_tr = genPart->begin() ; it_tr != genPart->end() ; ++it_tr)
          if( isGenPartGood(*it_tr) && isInAcceptance(it_tr->Part,accMap[acc].at(0),accMap[acc].at(1),accMap[acc].at(4)) ) {
  	    mpt_gen_noSel += it_tr->Part.v.Pt();
  	    mpt2_gen_noSel += pow(it_tr->Part.v.Pt(),2);
          } 
        n_gen_noSel = getnPrimaryGenPart(genPart,accMap[acc].at(0),accMap[acc].at(1),accMap[acc].at(4));
        if(n_gen_noSel!=0) { mpt_gen_noSel/=n_gen_noSel; mpt2_gen_noSel/=n_gen_noSel; }
        mptVSnch_gen.at(acc)->Fill(n_gen_noSel,mpt_gen_noSel);
        mpt2VSnch_gen.at(acc)->Fill(n_gen_noSel,mpt2_gen_noSel);
       }
      }  
    }    
 
    //skipping events that don't pass our event selection
    if(!isEvtGood(E,*L1Trig , *MITEvtSel , vertexToCut)) continue;
    
    /*double mpt_gen = 0;
    int n_gen = 0;
    if(isMC){
      for(vector<MyGenPart>::iterator it_tr = genPart->begin() ; it_tr != genPart->end() ; ++it_tr)
        if( isGenPartGood(*it_tr) && isInAcceptance(it_tr->Part,0,2.4) )
	  mpt_gen += it_tr->Part.v.Pt();
      n_gen = getnPrimaryGenPart(genPart,0,2.4);
      if(n_gen!=0) mpt_gen/=n_gen;
    }  */

    for(int acc = 0 ; acc < (signed)accMap.size() ; ++acc) {
     if (accNoSkip(acc)){  
    
      vector<MyTracks> trcoll;
      if(iTracking==0) trcoll = getPrimaryTracks(*tracks,vertex,   accMap[acc].at(2),accMap[acc].at(3),accMap[acc].at(4) );
      if(iTracking==1) trcoll = getPrimaryTracks(*tracks,vertex,bs,accMap[acc].at(2),accMap[acc].at(3),accMap[acc].at(4) );
      int n = trcoll.size();
      
      double mpt = 0;
      double mpt2 = 0;
      double mpt_systp_err = 0 , mpt_systm_err = 0 ;
      double mpt2_systp_err = 0 , mpt2_systm_err = 0 ;
      for(vector<MyTracks>::iterator itr = trcoll.begin() ; itr != trcoll.end() ; ++itr){
        mpt+=itr->Part.v.Pt();
        mpt2+=pow(itr->Part.v.Pt(),2.);
        
        mpt_systp_err+=itr->Part.v.Pt()+itr->ept;
        mpt_systm_err+=itr->Part.v.Pt()-itr->ept;
        mpt2_systp_err+=pow(itr->Part.v.Pt()+itr->ept,2.);
        mpt2_systm_err+=pow(itr->Part.v.Pt()-itr->ept,2.);

      }
      if(n!=0){
        mpt/=n;
        mpt_systp_err/=n;
        mpt_systm_err/=n;
        mpt2/=n;
        mpt2_systp_err/=n;
        mpt2_systm_err/=n;
      }
    
    //tmp->Fill(n_gen,mpt_gen);
    
    
      int bin_reco = matrixhist.at(acc)->GetYaxis()->FindFixBin(n);
      double norm = matrixhist.at(acc)->ProjectionY()->GetBinContent(bin_reco);
      if(norm==0) norm = 1;
    
      double ptcorr = 1;
      /*if(mptVSnchreco_reco.at(acc)->GetBinContent(bin_reco)!=0)
        ptcorr =   mptVSnchreco_gen.at(acc)->GetBinContent(bin_reco) 
                 / mptVSnchreco_reco.at(acc)->GetBinContent(bin_reco);*/
      if(mptVSnchgen_reco.at(acc)->GetBinContent(bin_reco)!=0)
        ptcorr =   mptVSnchgen_gen.at(acc)->GetBinContent(bin_reco) 
                 / mptVSnchgen_reco.at(acc)->GetBinContent(bin_reco);

      double pt2corr = 1;
      /*if(mpt2VSnchreco_reco.at(acc)->GetBinContent(bin_reco)!=0)
        pt2corr =   mpt2VSnchreco_gen.at(acc)->GetBinContent(bin_reco) 
                  / mpt2VSnchreco_reco.at(acc)->GetBinContent(bin_reco);*/
      if(mpt2VSnchgen_reco.at(acc)->GetBinContent(bin_reco)!=0)
        pt2corr =   mpt2VSnchgen_gen.at(acc)->GetBinContent(bin_reco) 
                  / mpt2VSnchgen_reco.at(acc)->GetBinContent(bin_reco);
	
      double eff = 1;
      if(eff_evtSel.at(acc)->GetBinContent(bin_reco)!=0)
        eff = eff_evtSel.at(acc)->GetBinContent(bin_reco);
    
      mptVSnch_noCorr.at(acc)->Fill(n , mpt);
      mptVSnch_nonchCorr.at(acc)->Fill(n , mpt * ptcorr , 1. / eff );

      mpt2VSnch_noCorr.at(acc)->Fill(n , mpt2);
      mpt2VSnch_nonchCorr.at(acc)->Fill(n , mpt2 * pt2corr , 1. / eff );

      //cout<<"event "<<i<<" , nch = "<<n<<" , mpt = "<<mpt<<" , bin_reco = "<<bin_reco<<endl;
      for(int igen = 1 ; igen <= mptVSnch.at(acc)->GetNbinsX() ; ++igen){
        double ptcorr = 1;
        double pt2corr = 1;
        /*if(mptVSnchreco_reco.at(acc)->GetBinContent(igen)!=0)
          ptcorr = mptVSnchreco_gen.at(acc)->GetBinContent(igen) / mptVSnchreco_reco.at(acc)->GetBinContent(igen);
	if(mpt2VSnchreco_reco.at(acc)->GetBinContent(igen)!=0)
          pt2corr = mpt2VSnchreco_gen.at(acc)->GetBinContent(igen) / mpt2VSnchreco_reco.at(acc)->GetBinContent(igen);*/
        if(mptVSnchgen_reco.at(acc)->GetBinContent(igen)!=0)
          ptcorr = mptVSnchgen_gen.at(acc)->GetBinContent(igen) / mptVSnchgen_reco.at(acc)->GetBinContent(igen);
	if(mpt2VSnchgen_reco.at(acc)->GetBinContent(igen)!=0)
          pt2corr = mpt2VSnchgen_gen.at(acc)->GetBinContent(igen) / mpt2VSnchgen_reco.at(acc)->GetBinContent(igen);
	


        double eff = 1;
        if(eff_evtSel.at(acc)->GetBinContent(igen)!=0)
          eff = eff_evtSel.at(acc)->GetBinContent(igen);
	
        //pt
        mptVSnch.at(acc)->Fill(mptVSnch.at(acc)->GetBinCenter(igen)    , mpt * ptcorr           , 1. / eff * matrixhist.at(acc)->GetBinContent(igen , bin_reco)/norm);
        systp_pterr.at(acc)->Fill(mptVSnch.at(acc)->GetBinCenter(igen) , mpt_systp_err * ptcorr , 1. / eff * matrixhist.at(acc)->GetBinContent(igen , bin_reco)/norm);
        systm_pterr.at(acc)->Fill(mptVSnch.at(acc)->GetBinCenter(igen) , mpt_systm_err * ptcorr , 1. / eff * matrixhist.at(acc)->GetBinContent(igen , bin_reco)/norm);
      
        mptVSnch_nomptCorr.at(acc)->Fill(mptVSnch.at(acc)->GetBinCenter(igen)   , mpt           , 1. / eff * matrixhist.at(acc)->GetBinContent(igen , bin_reco)/norm);
        mptVSnch_noeffCorr.at(acc)->Fill(mptVSnch.at(acc)->GetBinCenter(igen)   , mpt * ptcorr  , 1.       * matrixhist.at(acc)->GetBinContent(igen , bin_reco)/norm);
      
        mptVSnch_nchCorr.at(acc)->Fill(mptVSnch.at(acc)->GetBinCenter(igen)     , mpt           , matrixhist.at(acc)->GetBinContent(igen , bin_reco)/norm);
        mptVSnch_nch_mptCorr.at(acc)->Fill(mptVSnch.at(acc)->GetBinCenter(igen) , mpt * ptcorr  , matrixhist.at(acc)->GetBinContent(igen , bin_reco)/norm);

        //pt2 
        mpt2VSnch.at(acc)->Fill(mpt2VSnch.at(acc)->GetBinCenter(igen)     , mpt2 * pt2corr            , 1. / eff * matrixhist.at(acc)->GetBinContent(igen , bin_reco)/norm);
        syst2p_pterr.at(acc)->Fill(mpt2VSnch.at(acc)->GetBinCenter(igen) , mpt2_systp_err * pt2corr , 1. / eff * matrixhist.at(acc)->GetBinContent(igen , bin_reco)/norm);
        syst2m_pterr.at(acc)->Fill(mpt2VSnch.at(acc)->GetBinCenter(igen) , mpt2_systm_err * pt2corr , 1. / eff * matrixhist.at(acc)->GetBinContent(igen , bin_reco)/norm);
      
        mpt2VSnch_nomptCorr.at(acc)->Fill(mpt2VSnch.at(acc)->GetBinCenter(igen)   , mpt2            , 1. / eff * matrixhist.at(acc)->GetBinContent(igen , bin_reco)/norm);
        mpt2VSnch_noeffCorr.at(acc)->Fill(mpt2VSnch.at(acc)->GetBinCenter(igen)   , mpt2 * pt2corr  , 1.       * matrixhist.at(acc)->GetBinContent(igen , bin_reco)/norm);
      
        mpt2VSnch_nchCorr.at(acc)->Fill(mpt2VSnch.at(acc)->GetBinCenter(igen)      , mpt2            , matrixhist.at(acc)->GetBinContent(igen , bin_reco)/norm);
        mpt2VSnch_nch_mptCorr.at(acc)->Fill(mpt2VSnch.at(acc)->GetBinCenter(igen) , mpt2 * pt2corr  , matrixhist.at(acc)->GetBinContent(igen , bin_reco)/norm);


      
        //cout<<"bin "<<igen<<" , eff = "<<eff<<" mtx = "<<matrixhist->GetBinContent(igen , bin_reco)/norm<<endl;
        //cout<<"bin "<<igen<<" , eff = "<<eff<<" mtx = "<<matrixhist->GetBinContent(igen , bin_reco)/norm<<endl;
        //cout<<"n_gen "<<n_gen<<" , mpt_gen = "<<mpt_gen_noSel<<" n = "<<n<<" mpt_reco*ptcorr = "<<mpt * ptcorr<<endl;
      }
/*    
    double ptcorr = 1;
      if(mptVSnchreco_reco->GetBinContent(bin_reco)!=0)
        ptcorr = mptVSnchreco_gen->GetBinContent(bin_reco) / mptVSnchreco_reco->GetBinContent(bin_reco);
    mptrecoVSgen->Fill(mpt_gen,mpt*ptcorr);
    mptrecoVSgen_diff->Fill(mpt_gen-mpt*ptcorr)
*/    
    } 
   }

  }//end of loop over events
  
  
  vector<TGraphAsymmErrors*> gsyst(accMap.size(),0);
  vector<TGraphAsymmErrors*> gsyst2(accMap.size(),0);

  for(int acc = 0 ; acc < (signed)accMap.size() ; ++acc) {

   if (accNoSkip(acc)){  
    mptVSnch.at(acc)->Draw();
    
    mptVSnch_gen.at(acc)->SetLineColor(kRed);
    mptVSnch_gen.at(acc)->Draw("same");
    
    //tmp->SetLineColor(kBlue);
    //tmp->Draw("same");
    
    gPad->Update();
    gPad->WaitPrimitive();
    
    gStyle->SetPalette(1);
    //mptrecoVSgen->Draw("colz");
    mptrecoVSgen_diff.at(acc)->Draw();
    
    
    //Doing the trk systematics
    double syst_trk_frac = 2.58;
    double meanpt_losttrk = pt_genMINUSreco.at(acc)->GetMean();
    for(int igen = 1 ; igen <= mptVSnch.at(acc)->GetNbinsX() ; ++igen){
      if(mptVSnch.at(acc)->GetBinContent(igen)!=0){
        systp_trk.at(acc)->Fill(mptVSnch.at(acc)->GetBinCenter(igen),mptVSnch.at(acc)->GetBinContent(igen)+syst_trk_frac/100.*meanpt_losttrk);
        systm_trk.at(acc)->Fill(mptVSnch.at(acc)->GetBinCenter(igen),mptVSnch.at(acc)->GetBinContent(igen)-syst_trk_frac/100.*meanpt_losttrk);
        cout<<mptVSnch.at(acc)->GetBinContent(igen)<<"  "<<mptVSnch.at(acc)->GetBinContent(igen)-syst_trk_frac/100.*meanpt_losttrk<<endl;
      }
    }
    double meanpt2_losttrk = pt2_genMINUSreco.at(acc)->GetMean();
    for(int igen = 1 ; igen <= mpt2VSnch.at(acc)->GetNbinsX() ; ++igen){
      if(mpt2VSnch.at(acc)->GetBinContent(igen)!=0){
        syst2p_trk.at(acc)->Fill(mpt2VSnch.at(acc)->GetBinCenter(igen),mpt2VSnch.at(acc)->GetBinContent(igen)+syst_trk_frac/100.*meanpt2_losttrk);
        syst2m_trk.at(acc)->Fill(mpt2VSnch.at(acc)->GetBinCenter(igen),mpt2VSnch.at(acc)->GetBinContent(igen)-syst_trk_frac/100.*meanpt2_losttrk);
        cout<<mpt2VSnch.at(acc)->GetBinContent(igen)<<"  "<<mpt2VSnch.at(acc)->GetBinContent(igen)-syst_trk_frac/100.*meanpt2_losttrk<<endl;
      }
    }


    
    //doign the global systematic : pt
    {
      Double_t* x   = new Double_t[mptVSnch.at(acc)->GetNbinsX()];
      Double_t* y   = new Double_t[mptVSnch.at(acc)->GetNbinsX()];
      Double_t* exl = new Double_t[mptVSnch.at(acc)->GetNbinsX()];
      Double_t* exh = new Double_t[mptVSnch.at(acc)->GetNbinsX()];
      Double_t* eyl = new Double_t[mptVSnch.at(acc)->GetNbinsX()];
      Double_t* eyh = new Double_t[mptVSnch.at(acc)->GetNbinsX()];
      for(int i = 1 ; i <= mptVSnch.at(acc)->GetNbinsX() ; ++i){
        x[i-1] = mptVSnch.at(acc)->GetBinCenter(i);
        y[i-1] = mptVSnch.at(acc)->GetBinContent(i);
        exl[i-1] = mptVSnch.at(acc)->GetBinCenter(i) - mptVSnch.at(acc)->GetXaxis()->GetBinLowEdge(i);
        exh[i-1] = mptVSnch.at(acc)->GetXaxis()->GetBinUpEdge(i) - mptVSnch.at(acc)->GetBinCenter(i);
      
        eyl[i-1] = pow(systm_trk.at(acc)->GetBinContent(i)-mptVSnch.at(acc)->GetBinContent(i),2) + pow(systm_pterr.at(acc)->GetBinContent(i)-mptVSnch.at(acc)->GetBinContent(i),2) + pow(mptVSnch.at(acc)->GetBinError(i),2);
        eyl[i-1] = sqrt(eyl[i-1]);
      
        eyh[i-1] = pow(systp_trk.at(acc)->GetBinContent(i)-mptVSnch.at(acc)->GetBinContent(i),2) + pow(systp_pterr.at(acc)->GetBinContent(i)-mptVSnch.at(acc)->GetBinContent(i),2) + pow(mptVSnch.at(acc)->GetBinError(i),2);
        eyh[i-1] = sqrt(eyh[i-1]);
      }
      
      gsyst.at(acc) = new TGraphAsymmErrors(mptVSnch.at(acc)->GetNbinsX(),x,y,exl,exh,eyl,eyh);
      gsyst.at(acc)->SetName(TString("g")+mptVSnch.at(acc)->GetName()+TString("_syst"));
      gsyst.at(acc)->SetTitle(TString("g")+mptVSnch.at(acc)->GetTitle()+TString("_syst"));
      gsyst.at(acc)->GetXaxis()->SetTitle("n");
      gsyst.at(acc)->GetYaxis()->SetTitle("<p_{t}>");
    }

    //doign the global systematic : pt2
    { 
      Double_t* x   = new Double_t[mpt2VSnch.at(acc)->GetNbinsX()];
      Double_t* y   = new Double_t[mpt2VSnch.at(acc)->GetNbinsX()];
      Double_t* exl = new Double_t[mpt2VSnch.at(acc)->GetNbinsX()];
      Double_t* exh = new Double_t[mpt2VSnch.at(acc)->GetNbinsX()];
      Double_t* eyl = new Double_t[mpt2VSnch.at(acc)->GetNbinsX()];
      Double_t* eyh = new Double_t[mpt2VSnch.at(acc)->GetNbinsX()];
      for(int i = 1 ; i <= mpt2VSnch.at(acc)->GetNbinsX() ; ++i){
        x[i-1] = mpt2VSnch.at(acc)->GetBinCenter(i);
        y[i-1] = mpt2VSnch.at(acc)->GetBinContent(i);
        exl[i-1] = mpt2VSnch.at(acc)->GetBinCenter(i) - mpt2VSnch.at(acc)->GetXaxis()->GetBinLowEdge(i);
        exh[i-1] = mpt2VSnch.at(acc)->GetXaxis()->GetBinUpEdge(i) - mpt2VSnch.at(acc)->GetBinCenter(i);
      
        eyl[i-1] = pow(systm_trk.at(acc)->GetBinContent(i)-mpt2VSnch.at(acc)->GetBinContent(i),2) + pow(syst2m_pterr.at(acc)->GetBinContent(i)-mpt2VSnch.at(acc)->GetBinContent(i),2) + pow(mpt2VSnch.at(acc)->GetBinError(i),2);
        eyl[i-1] = sqrt(eyl[i-1]);
      
        eyh[i-1] = pow(systp_trk.at(acc)->GetBinContent(i)-mpt2VSnch.at(acc)->GetBinContent(i),2) + pow(syst2p_pterr.at(acc)->GetBinContent(i)-mpt2VSnch.at(acc)->GetBinContent(i),2) + pow(mpt2VSnch.at(acc)->GetBinError(i),2);
        eyh[i-1] = sqrt(eyh[i-1]);
      }
      
      gsyst2.at(acc) = new TGraphAsymmErrors(mpt2VSnch.at(acc)->GetNbinsX(),x,y,exl,exh,eyl,eyh);
      gsyst2.at(acc)->SetName(TString("g")+mpt2VSnch.at(acc)->GetName()+TString("_syst"));
      gsyst2.at(acc)->SetTitle(TString("g")+mpt2VSnch.at(acc)->GetTitle()+TString("_syst"));
      gsyst2.at(acc)->GetXaxis()->SetTitle("n");
      gsyst2.at(acc)->GetYaxis()->SetTitle("<p_{t}^{2}>");
    }

   }
  }  

  //output file
  ostringstream strout("");
  //strout<<"mptCorr_MCtype"<<type<<"_"<<E<<"TeV.root";
  strout<<"dataType"<<type;
  TString outname = fileManager(5,mctype,E,iTracking,syst,syst_sign,strout.str());
  cout<<"Output file : "<<outname<<endl;
  TFile* output = new TFile(outname,"RECREATE");
  output->cd();

  for(int acc = 0 ; acc < (signed)accMap.size() ; ++acc) {
   if (accNoSkip(acc)){  
    
    // ---- pt ------  
    mptVSnch.at(acc)->SetBinContent(1,0);mptVSnch.at(acc)->SetBinError(1,0);
    mptVSnch.at(acc)->Write();
    
    mptVSnch_noCorr.at(acc)->SetBinContent(1,0);mptVSnch_noCorr.at(acc)->SetBinError(1,0);
    mptVSnch_noCorr.at(acc)->Write();
    
    mptVSnch_nonchCorr.at(acc)->SetBinContent(1,0);mptVSnch_nonchCorr.at(acc)->SetBinError(1,0);
    mptVSnch_nonchCorr.at(acc)->Write();
    
    mptVSnch_nomptCorr.at(acc)->SetBinContent(1,0);mptVSnch_nomptCorr.at(acc)->SetBinError(1,0);
    mptVSnch_nomptCorr.at(acc)->Write();
    
    mptVSnch_noeffCorr.at(acc)->SetBinContent(1,0);mptVSnch_noeffCorr.at(acc)->SetBinError(1,0);
    mptVSnch_noeffCorr.at(acc)->Write();
    
    mptVSnch_nchCorr.at(acc)->SetBinContent(1,0);mptVSnch_nchCorr.at(acc)->SetBinError(1,0);
    mptVSnch_nchCorr.at(acc)->Write();
    
    mptVSnch_nch_mptCorr.at(acc)->SetBinContent(1,0);mptVSnch_nch_mptCorr.at(acc)->SetBinError(1,0);
    mptVSnch_nch_mptCorr.at(acc)->Write();
    
    if(isMC){
      mptVSnch_gen.at(acc)->SetBinContent(1,0);mptVSnch_gen.at(acc)->SetBinError(1,0);
      mptVSnch_gen.at(acc)->Write();
      
      mptrecoVSgen.at(acc)->SetBinContent(1,0);mptrecoVSgen.at(acc)->SetBinError(1,0);
      mptrecoVSgen.at(acc)->Write();
      
      mptVSnch_gen.at(acc)->Divide(mptVSnch_gen.at(acc),mptVSnch.at(acc),1.,1.,"B");
      mptVSnch_gen.at(acc)->Write(st("effnchreco_genOreco",acc));
      
 //     gPad->WaitPrimitive();
      
 //     mptVSnch_gen->Draw();
      
    }
    
    systp_pterr.at(acc)->SetBinContent(1,0);systp_pterr.at(acc)->SetBinError(1,0);
    systp_pterr.at(acc)->Write();
    
    systm_pterr.at(acc)->SetBinContent(1,0);systm_pterr.at(acc)->SetBinError(1,0);
    systm_pterr.at(acc)->Write();
    
    systp_trk.at(acc)->SetBinContent(1,0);systp_trk.at(acc)->SetBinError(1,0);
    systp_trk.at(acc)->Write();
    
    systm_trk.at(acc)->SetBinContent(1,0);systm_trk.at(acc)->SetBinError(1,0);
    systm_trk.at(acc)->Write();
    
    gsyst.at(acc)->SetPoint(0,0,0);
    gsyst.at(acc)->Write();


    // ---- pt2 ------  
    mpt2VSnch.at(acc)->SetBinContent(1,0);mpt2VSnch.at(acc)->SetBinError(1,0);
    mpt2VSnch.at(acc)->Write();
    
    mpt2VSnch_noCorr.at(acc)->SetBinContent(1,0);mpt2VSnch_noCorr.at(acc)->SetBinError(1,0);
    mpt2VSnch_noCorr.at(acc)->Write();
    
    mpt2VSnch_nonchCorr.at(acc)->SetBinContent(1,0);mpt2VSnch_nonchCorr.at(acc)->SetBinError(1,0);
    mpt2VSnch_nonchCorr.at(acc)->Write();
    
    mpt2VSnch_nomptCorr.at(acc)->SetBinContent(1,0);mpt2VSnch_nomptCorr.at(acc)->SetBinError(1,0);
    mpt2VSnch_nomptCorr.at(acc)->Write();
    
    mpt2VSnch_noeffCorr.at(acc)->SetBinContent(1,0);mpt2VSnch_noeffCorr.at(acc)->SetBinError(1,0);
    mpt2VSnch_noeffCorr.at(acc)->Write();
    
    mpt2VSnch_nchCorr.at(acc)->SetBinContent(1,0);mpt2VSnch_nchCorr.at(acc)->SetBinError(1,0);
    mpt2VSnch_nchCorr.at(acc)->Write();
    
    mpt2VSnch_nch_mptCorr.at(acc)->SetBinContent(1,0);mpt2VSnch_nch_mptCorr.at(acc)->SetBinError(1,0);
    mpt2VSnch_nch_mptCorr.at(acc)->Write();
    
    if(isMC){
      mpt2VSnch_gen.at(acc)->SetBinContent(1,0);mpt2VSnch_gen.at(acc)->SetBinError(1,0);
      mpt2VSnch_gen.at(acc)->Write();
      
      mpt2recoVSgen.at(acc)->SetBinContent(1,0);mpt2recoVSgen.at(acc)->SetBinError(1,0);
      mpt2recoVSgen.at(acc)->Write();
      
      mpt2VSnch_gen.at(acc)->Divide(mpt2VSnch_gen.at(acc),mpt2VSnch.at(acc),1.,1.,"B");
      mpt2VSnch_gen.at(acc)->Write(st("effnchreco_genOreco",acc));
      
 //     gPad->WaitPrimitive();
      
 //     mpt2VSnch_gen->Draw();
      
    }
    
    syst2p_pterr.at(acc)->SetBinContent(1,0);systp_pterr.at(acc)->SetBinError(1,0);
    syst2p_pterr.at(acc)->Write();
    
    syst2m_pterr.at(acc)->SetBinContent(1,0);systm_pterr.at(acc)->SetBinError(1,0);
    syst2m_pterr.at(acc)->Write();
    
    syst2p_trk.at(acc)->SetBinContent(1,0);systp_trk.at(acc)->SetBinError(1,0);
    syst2p_trk.at(acc)->Write();
    
    syst2m_trk.at(acc)->SetBinContent(1,0);systm_trk.at(acc)->SetBinError(1,0);
    syst2m_trk.at(acc)->Write();
    
    gsyst2.at(acc)->SetPoint(0,0,0);
    gsyst2.at(acc)->Write();

   }
  }
 
  output->Close();
}


void multiplyByWidth(TH1F* hist){
  for(int i=1;i<=hist->GetNbinsX();++i){
    hist->SetBinContent(i,double(hist->GetBinContent(i))*double(hist->GetBinWidth(i)));
    hist->SetBinError(i,double(hist->GetBinError(i))*double(hist->GetBinWidth(i)));
  }
}


void multiplyByWidth(TH2F* hist){
  double width = 1;
  for(int i=1;i<=hist->GetNbinsX();++i){
    for(int j=1;j<=hist->GetNbinsX();++j){
      width = hist->GetXaxis()->GetBinWidth(i) * hist->GetYaxis()->GetBinWidth(j);
      hist->SetBinContent(i,j,double(hist->GetBinContent(i,j))*width);
      hist->SetBinError(i,j,double(hist->GetBinError(i,j))*width);
    }
  }
}
