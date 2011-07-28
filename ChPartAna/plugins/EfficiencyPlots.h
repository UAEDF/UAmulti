#ifndef __EfficiencyPlots_H__
#define __EfficiencyPlots_H__

#include <vector>
#include <iostream>
using namespace std;
#include "TObject.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TMath.h"
#include "TDirectory.h"
#include "MatchTools.C"
#include "TOperation.h"
#include "BasePlots.h"

class EfficiencyPlots : public BasePlots {

  public :
    EfficiencyPlots();
    EfficiencyPlots(TString);
    virtual ~EfficiencyPlots();

    TString Recocoll;
    
    TH1F* pt_gen; 
    TH1F* eta_gen; 
    TH1F* phi_gen; 
    TH2F* pt_eta_gen;
    
    TH1F* pt_rec_matched;
    TH1F* eta_rec_matched;
    TH1F* phi_rec_matched;
    TH2F* pt_eta_rec_matched;
 
    TH1F* pt_rec_fake;
    TH1F* eta_rec_fake;
    TH1F* phi_rec_fake;
    TH2F* pt_eta_rec_fake;
    

    TH1F* nMatched;
    TH1F* dR;
    TH1F* dPt;
    TH1F* deta;
    TH1F* dphi;
         
    //TH1F* pt_eff;
    //TH1F* eta_eff;
    //TH1F* phi_eff;
    //TH2F* pt_eta_eff;
    
    Double_t deltaR;
    Double_t deltaPt;
    
    void init();
 
    template<class T , class U>
    void fill(vector<T>* , vector<U>* , double = 1.);
    
       
    void write();
    
    ClassDef (EfficiencyPlots,1)
};


template<class T , class U>
void EfficiencyPlots::fill(vector<T>* v_gen, vector<U>* v_reco, double weight){

  vector< pair<T* , U*> > match;
  vector<U*>	          fakes;
  vector<T*>	          gen_notMatched;

  GetMatch2(v_gen , v_reco , &match , &fakes , &gen_notMatched , this->deltaR , this->deltaPt);
  
  for(typename vector< pair<T* , U*> >::iterator mpair = match.begin() ; mpair != match.end() ; ++mpair){
    pt_rec_matched->Fill(mpair->first->Pt() , weight);
    eta_rec_matched->Fill(mpair->first->Eta() , weight);
    phi_rec_matched->Fill(mpair->first->Phi() , weight);
    pt_eta_rec_matched->Fill(mpair->first->Pt() , mpair->first->Eta() , weight);
    
    dR->Fill ( deltaR( mpair->first->Eta() , mpair->second->Eta() , mpair->first->Phi() , mpair->second->Phi() ) , weight ); 
    dPt->Fill( mpair->first->Pt() - mpair->second->Pt() , weight ); 
    deta->Fill( mpair->first->Eta() - mpair->second->Eta() , weight ); 
    dphi->Fill( mpair->first->Phi() - mpair->second->Phi() , weight ); 
  }
  
  for(typename vector<U*>::iterator it_fake = fakes.begin() ; it_fake != fakes.end() ; ++it_fake){
    pt_rec_fake->Fill(it_fake->Pt() , weight);
    eta_rec_fake->Fill(it_fake->Eta() , weight);
    phi_rec_fake->Fill(it_fake->Phi() , weight);
    pt_eta_rec_fake->Fill(it_fake->Pt() , it_fake->Eta() , weight);
  }
  
  for(typename vector<U*>::iterator it_gen = v_gen->begin() ; it_gen != v_gen->end() ; ++it_gen){
    pt_gen->Fill(it_gen->Pt() , weight);
    eta_gen->Fill(it_gen->Eta() , weight);
    phi_gen->Fill(it_gen->Phi() , weight);
    pt_eta_gen->Fill(it_gen->Pt() , it_gen->Eta() , weight);
  } 

}


EfficiencyPlots::EfficiencyPlots(){
  Recocoll = "none";
  this->init();
}

EfficiencyPlots::EfficiencyPlots(TString collname){
  Recocoll = collname;
  this->init();
}

EfficiencyPlots::~EfficiencyPlots(){ }


void EfficiencyPlots::init(){
 
// Particle basic properties
    
    TH1::SetDefaultSumw2(1);
    
    pt_gen  = new TH1F("pt_gen_"+Recocoll,"pt_gen_"+Recocoll+";pt;#events",pt_nbin,pt_array); 
    eta_gen = new TH1F("eta_gen_"+Recocoll,"eta_gen_"+Recocoll+";eta;#events",eta_nbin , eta_array); 
    phi_gen = new TH1F("phi_gen_"+Recocoll,"phi_gen_"+Recocoll+";phi;#events",30,-TMath::Pi(),TMath::Pi());
    pt_eta_gen = new TH2F("pt_eta_gen_"+Recocoll,"pt_eta_gen_"+Recocoll+";pt;eta;#events",pt_nbin,pt_array,eta_nbin,eta_array); 
    
    pt_rec_matched  = new TH1F("pt_rec_matched_"+Recocoll,"pt_rec_matched_"+Recocoll+";pt;#events",pt_nbin,pt_array); 
    eta_rec_matched = new TH1F("eta_rec_matched_"+Recocoll,"eta_rec_matched_"+Recocoll+";eta;#events",eta_nbin , eta_array); 
    phi_rec_matched = new TH1F("phi_rec_matched_"+Recocoll,"phi_rec_matched_"+Recocoll+";phi;#events",30,-TMath::Pi(),TMath::Pi());
    pt_eta_rec_matched = new TH2F("pt_eta_rec_matched_"+Recocoll,"pt_eta_rec_matched_"+Recocoll+";pt;eta;#events",pt_nbin,pt_array,eta_nbin,eta_array);
    
    pt_rec_fake  = new TH1F("pt_rec_fake_"+Recocoll,"pt_rec_fake_"+Recocoll+";pt;#events",pt_nbin,pt_array); 
    eta_rec_fake = new TH1F("eta_rec_fake_"+Recocoll,"eta_rec_fake_"+Recocoll+";eta;#events",eta_nbin , eta_array); 
    phi_rec_fake = new TH1F("phi_rec_fake_"+Recocoll,"phi_rec_fake_"+Recocoll+";phi;#events",30,-TMath::Pi(),TMath::Pi());
    pt_eta_rec_fake = new TH2F("pt_eta_rec_fake_"+Recocoll,"pt_eta_rec_fake_"+Recocoll+";pt;eta;#events",pt_nbin,pt_array,eta_nbin,eta_array);  
    
    /*pt_eff  = new TH1F("pt_eff_"+Recocoll,"pt_eff_"+Recocoll+";pt;#events",pt_nbin,pt_array); 
    eta_eff = new TH1F("eta_eff_"+Recocoll,"eta_eff_"+Recocoll+";eta;#events",eta_nbin , eta_array); 
    phi_eff = new TH1F("phi_eff_"+Recocoll,"phi_eff_"+Recocoll+";phi;#events",30,-TMath::Pi(),TMath::Pi());
    pt_eta_eff = new TH2F("pt_eta_eff_"+Recocoll,"pt_eta_eff_"+Recocoll+";pt;eta;#events",pt_nbin,pt_array,eta_nbin,eta_array); 
*/
    nMatched = new TH1F("nMatched_"+Recocoll,"nMatched_"+Recocoll+";nMatched;#events",10,0,10);
    dR       = new TH1F("dR_"+Recocoll,"dR_"+Recocoll+";Delta R ;#events",50,0.,0.5);
    dPt      = new TH1F("dPt_"+Recocoll,"dPt_"+Recocoll+";Delta Pt;#events",40,-10.,10.);
    deta      = new TH1F("deta_"+Recocoll,"deta_"+Recocoll+";Delta eta;#events",50,-0.5,0.5);
    dphi      = new TH1F("dphi_"+Recocoll,"dphi_"+Recocoll+";Delta phi;#events",50,-0.5,0.5);

 
}




void EfficiencyPlots::write(){
  
  gDirectory->mkdir("EfficiencyPlots_"+Recocoll);
  gDirectory->cd("EfficiencyPlots_"+Recocoll);
  
  pt_gen->Write(); 
  eta_gen->Write(); 
  phi_gen->Write(); 
  pt_eta_gen->Write();

  pt_rec_matched->Write();
  eta_rec_matched->Write();
  phi_rec_matched->Write();
  pt_eta_rec_matched->Write();

  pt_rec_fake->Write();
  eta_rec_fake->Write();
  phi_rec_fake->Write();
  pt_eta_rec_fake->Write();


  nMatched->Write();
  dR->Write();
  dPt->Write();
  deta->Write();
  dphi->Write();

  TOperation<TH1F> pt_eff ("pt_eff_" +Recocoll , *pt_rec_matched  , *pt_gen  , "eff");
  TOperation<TH1F> eta_eff("eta_eff_"+Recocoll , *eta_rec_matched , *eta_gen , "eff");
  TOperation<TH1F> phi_eff("phi_eff_"+Recocoll , *phi_rec_matched , *phi_gen , "eff");
  TOperation<TH2F> pt_eta_eff("pt_eta_eff_" +Recocoll , *pt_eta_rec_matched  , *pt_eta_gen  , "eff");
 
  TOperation<TH1F> pt_fake ("pt_fake_" +Recocoll , *pt_rec_fake  , *pt_gen  , "eff");
  TOperation<TH1F> eta_fake("eta_fake_"+Recocoll , *eta_rec_fake , *eta_gen , "eff");
  TOperation<TH1F> phi_fake("phi_fake_"+Recocoll , *phi_rec_fake , *phi_gen , "eff");
  TOperation<TH2F> pt_eta_fake("pt_eta_fake_" +Recocoll , *pt_eta_rec_fake  , *pt_eta_gen  , "eff");
 
  pt_eff  .write(1); 
  eta_eff .write(1); 
  phi_eff .write(1); 
  pt_eta_eff.write(1); 

  pt_fake  .write(1);
  eta_fake .write(1);
  phi_fake .write(1);
  pt_eta_fake.write(1);
  
  
  gDirectory->cd("../");
}
#endif

















