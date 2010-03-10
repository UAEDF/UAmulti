#include "MatrixPlots.h"

ClassImp(MatrixPlots)


void MatrixPlots::init(){
  nch_gen = new TH1F("nch_gen_"+plotsname,"nch_gen_"+plotsname+";nch_gen;# events",nch_nbin,nch_array);
  eta_gen = new TH1F("eta_gen_"+plotsname,"eta_gen_"+plotsname+";eta_gen;# events",eta_nbin,eta_array);
  pt_gen  = new TH1F("pt_gen_"+plotsname,"pt_gen_"+plotsname+";pt_gen;# events",pt_nbin,pt_array);
  
  nch_reco = new TH1F("nch_reco_"+plotsname,"nch_reco_"+plotsname+";nch_reco;# events",nch_nbin,nch_array);
  eta_reco = new TH1F("eta_reco_"+plotsname,"eta_reco_"+plotsname+";eta_reco;# events",eta_nbin,eta_array);
  pt_reco  = new TH1F("pt_reco_"+plotsname,"pt_reco_"+plotsname+";pt_reco;# events",pt_nbin,pt_array);
  
  nch_matrix = new TH2F("nch_matrix_"+plotsname,"nch_matrix_"+plotsname+";nch_gen;nch_reco",nch_nbin,nch_array,nch_nbin,nch_array);
  eta_matrix = new TH2F("eta_matrix_"+plotsname,"eta_matrix_"+plotsname+";eta_gen;eta_reco",eta_nbin,eta_array,eta_nbin,eta_array);
  pt_matrix  = new TH2F("pt_matrix_"+plotsname,"pt_matrix_"+plotsname+";pt_gen;pt_reco",pt_nbin,pt_array,pt_nbin,pt_array);
  
  nch_reco_GenBin0 = new TH1F("nch_reco_GenBin0_"+plotsname,"nch_reco_GenBin0_"+plotsname+";nch_gen;# events",nch_nbin,nch_array);
  nch_gen_RecoBin0 = new TH1F("nch_gen_RecoBin0_"+plotsname,"nch_gen_RecoBin0_"+plotsname+";nch_gen;# events",nch_nbin,nch_array);
  
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
  nbEvts = 0;
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
  ++nbEvts;
  
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

void MatrixPlots::write(bool scale){
  //TDirectory* dir = new TDirectory("MatrixPlots_"+plotsname,"");
  //dir->cd();  
  gDirectory->mkdir("MatrixPlots_"+plotsname);
  gDirectory->cd("MatrixPlots_"+plotsname);
  
  
  if(scale) nch_gen->Scale(1./nbEvts);
  nch_gen->Write();
  eta_gen->Write();
  pt_gen->Write();
  
  if(scale) nch_reco->Scale(1./nbEvts);
  nch_reco->Write();
  eta_reco->Write();
  pt_reco->Write();
  
  if(scale) nch_matrix->Scale(1./nbEvts);
  nch_matrix->Write();
  eta_matrix->Write();
  pt_matrix->Write();
  
  nch_reco_GenBin0->Write();
  nch_gen_RecoBin0->Write();
  
  gDirectory->cd("../");
}

