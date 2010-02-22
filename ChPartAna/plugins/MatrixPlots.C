
#include "MatrixPlots.h"

ClassImp(MatrixPlots)

MatrixPlots::MatrixPlots(){
  matrixcoll = "none";
  this->init();
}

MatrixPlots::MatrixPlots(TString collname){
  matrixcoll = collname;
  this->init();
}

MatrixPlots::~MatrixPlots(){ }

void MatrixPlots::init(){
  nch_gen = new TH1F("nch_gen_"+matrixcoll,"nch_gen_"+matrixcoll+";nch_gen;# events",71,-0.5,70.5);
  eta_gen = new TH1F("eta_gen_"+matrixcoll,"eta_gen_"+matrixcoll+";eta_gen;# events",60,-3.,3.);
  pt_gen  = new TH1F("pt_gen_"+matrixcoll,"pt_gen_"+matrixcoll+";pt_gen;# events",100,0,3.);
  
  nch_reco = new TH1F("nch_reco_"+matrixcoll,"nch_reco_"+matrixcoll+";nch_reco;# events",71,-0.5,70.5);
  eta_reco = new TH1F("eta_reco_"+matrixcoll,"eta_reco_"+matrixcoll+";eta_reco;# events",60,-3.,3.);
  pt_reco  = new TH1F("pt_reco_"+matrixcoll,"pt_reco_"+matrixcoll+";pt_reco;# events",100,0,3.);
  
  nch_matrix = new TH2F("nch_matrix_"+matrixcoll,"nch_matrix_"+matrixcoll+";nch_gen;nch_reco",71,-0.5,70.5,71,-0.5,70.5);
  eta_matrix = new TH2F("eta_matrix_"+matrixcoll,"eta_matrix_"+matrixcoll+";eta_gen;eta_reco",60,-3.,3.,60,-3.,3.);
  pt_matrix  = new TH2F("pt_matrix_"+matrixcoll,"pt_matrix_"+matrixcoll+";pt_gen;pt_reco",100,0,3.,100,0,3.);
  
  nch_reco_GenBin0 = new TH1F("nch_reco_GenBin0_"+matrixcoll,"nch_reco_GenBin0_"+matrixcoll+";nch_gen;# events",71,-0.5,70.5);
  nch_gen_RecoBin0 = new TH1F("nch_gen_RecoBin0_"+matrixcoll,"nch_gen_RecoBin0_"+matrixcoll+";nch_gen;# events",71,-0.5,70.5);
  
  nch_gen->Sumw2();
  eta_gen->Sumw2();
  pt_gen->Sumw2();
  nch_reco->Sumw2();
  eta_reco->Sumw2();
  pt_reco->Sumw2();
  nch_matrix->Sumw2();
  eta_matrix->Sumw2();
  pt_matrix->Sumw2();
  
  nch_gen_inEvt = 0;
  nch_reco_inEvt = 0;
  
}

void MatrixPlots::fillGen(MyPart& gen , bool accept_reco, double weight_gen ){
  if(accept_reco){
    eta_gen->Fill(gen.v.Eta() , weight_gen);
    pt_gen->Fill(gen.v.Pt() , weight_gen);
    ++nch_gen_inEvt;
  }
  //eta_matrix->Fill(gen.v.Eta() , reco.v.Eta() );
  //pt_matrix->Fill(gen.v.Pt() , reco.v.Pt() );
}

void MatrixPlots::fillReco(MyPart& reco ,bool accept_gen, double weight_reco ){
  if(accept_gen){
    eta_reco->Fill(reco.v.Eta() , weight_reco);
    pt_reco->Fill(reco.v.Pt() , weight_reco);
    ++nch_reco_inEvt;
  }
}

void MatrixPlots::nextEvent(bool accept_reco, bool accept_gen){
  if(!accept_gen){
    nch_gen_inEvt=-1;
    nch_reco_GenBin0->Fill(nch_reco_inEvt);
  }
  if(!accept_reco){
    nch_reco_inEvt=-1;
    nch_gen_RecoBin0->Fill(nch_gen_inEvt);
  }
  nch_gen->Fill(nch_gen_inEvt);
  nch_reco->Fill(nch_reco_inEvt);
  nch_matrix->Fill( nch_gen_inEvt , nch_reco_inEvt );
  nch_gen_inEvt = nch_reco_inEvt = 0;
}

void MatrixPlots::write(){
  //TDirectory* dir = new TDirectory("MatrixPlots_"+matrixcoll,"");
  //dir->cd();  
  gDirectory->mkdir("MatrixPlots_"+matrixcoll);
  gDirectory->cd("MatrixPlots_"+matrixcoll);
  
  nch_gen->Write();
  eta_gen->Write();
  pt_gen->Write();
  
  nch_reco->Write();
  eta_reco->Write();
  pt_reco->Write();
  
  nch_matrix->Write();
  eta_matrix->Write();
  pt_matrix->Write();
  
  nch_reco_GenBin0->Write();
  nch_gen_RecoBin0->Write();
  
  gDirectory->cd("../");
}

