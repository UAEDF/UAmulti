#include "EvtSelPlots.h"

ClassImp(EvtSelPlots)

EvtSelPlots::EvtSelPlots(){
  trackcoll = "none";
  this->init();
}

EvtSelPlots::EvtSelPlots(TString collname){
  trackcoll = collname;
  this->init();
}

EvtSelPlots::~EvtSelPlots(){ }

void EvtSelPlots::init(){

  // -------- Multi Histos ---------
  int nbinmulti = 25;
  /*nch_noSel  = new TH1F("nch_noSel_"+trackcoll,"nch_noSel;n_{CH};# events",(int)nbinmulti+1,-0.5,nbinmulti+0.5);
  nch_b40Sel  = new TH1F("nch_b40Sel_"+trackcoll,"nch_b40Sel;n_{CH};# events",(int)nbinmulti+1,-0.5,nbinmulti+0.5);
  nch_L1Sel  = new TH1F("nch_L1Sel_"+trackcoll,"nch_L1Sel;n_{CH};# events",(int)nbinmulti+1,-0.5,nbinmulti+0.5);
  nch_hfSel  = new TH1F("nch_hfSel_"+trackcoll,"nch_hfSel;n_{CH};# events",(int)nbinmulti+1,-0.5,nbinmulti+0.5);
  nch_vtxSel = new TH1F("nch_vtxSel_"+trackcoll,"nch_vtxSel;n_{CH};# events",(int)nbinmulti+1,-0.5,nbinmulti+0.5);
  nch_L1_hfSel     = new TH1F("nch_L1_hfSel_"+trackcoll,"nch_L1_hfSel;n_{CH};# events",(int)nbinmulti+1,-0.5,nbinmulti+0.5);
  nch_L1_hf_vtxSel = new TH1F("nch_L1_hf_vtxSel_"+trackcoll,"nch_L1_hf_vtxSel;n_{CH};# events",(int)nbinmulti+1,-0.5,nbinmulti+0.5);
  nch_L1_b40_vtxSel = new TH1F("nch_L1_b40_vtxSel_"+trackcoll,"nch_L1_b40_vtxSel;n_{CH};# events",(int)nbinmulti+1,-0.5,nbinmulti+0.5);
  nch_noSel->Sumw2();
  nch_b40Sel->Sumw2();
  nch_L1Sel->Sumw2();
  nch_hfSel->Sumw2();
  nch_vtxSel->Sumw2();
  nch_L1_hfSel->Sumw2();
  nch_L1_hf_vtxSel->Sumw2();
  nch_L1_b40_vtxSel->Sumw2();*/
  
  
  trp_noSel                = new TrackPlots("noSel_"+trackcoll);
  trp_b40Sel               = new TrackPlots("b40Sel_"+trackcoll);
  trp_L1Sel                = new TrackPlots("L1Sel_"+trackcoll);
  trp_hfSel                = new TrackPlots("hfSel_"+trackcoll);
  trp_vtxqualSel           = new TrackPlots("vtxqualSel_"+trackcoll);
  trp_vtxSel               = new TrackPlots("vtxSel_"+trackcoll);
  trp_L1_hfSel             = new TrackPlots("L1_hfSel_"+trackcoll);
  trp_L1_hf_vtxqualSel     = new TrackPlots("L1_hf_vtxqualSel_"+trackcoll);
  trp_L1_hf_vtxqual_vtxSel = new TrackPlots("L1_hf_vtxqual_vtxSel_"+trackcoll);
  trp_L1_vtxqual_vtxSel    = new TrackPlots("L1_vtxqual_vtxSel_"+trackcoll);
  trp_L1_b40_vtxqualSel    = new TrackPlots("L1_b40_vtxqualSel_"+trackcoll);
  
  //init of eff denom with noSelection histos
  nch_denom = trp_noSel->nch;
  pt_denom  = trp_noSel->pt;
  eta_denom = trp_noSel->eta;
  nch_denom->Sumw2();
  pt_denom->Sumw2();
  eta_denom->Sumw2();
  
  vtxp_noSel                = new VertexPlots("noSel_"+trackcoll);
  vtxp_b40Sel               = new VertexPlots("b40Sel_"+trackcoll);
  vtxp_L1Sel                = new VertexPlots("L1Sel_"+trackcoll);
  vtxp_hfSel                = new VertexPlots("hfSel_"+trackcoll);
  vtxp_vtxqualSel           = new VertexPlots("vtxqualSel_"+trackcoll);
  vtxp_vtxSel               = new VertexPlots("vtxSel_"+trackcoll);
  vtxp_L1_hfSel             = new VertexPlots("L1_hfSel_"+trackcoll);
  vtxp_L1_hf_vtxqualSel     = new VertexPlots("L1_hf_vtxqualSel_"+trackcoll);
  vtxp_L1_hf_vtxqual_vtxSel = new VertexPlots("L1_hf_vtxqual_vtxSel_"+trackcoll);
  vtxp_L1_vtxqual_vtxSel    = new VertexPlots("L1_vtxqual_vtxSel_"+trackcoll);
  vtxp_L1_b40_vtxqualSel    = new VertexPlots("L1_b40_vtxqualSel_"+trackcoll);

  nvtx_evtSel    = new TH1F("nvtx_evtSel_"+trackcoll,"nvtx_evtSel_",21,-0.5,20.5);
  nvtx_ntrneq0_evtSel  = new TH1F("nvtx_ntrneq0_evtSel_"+trackcoll,"nvtx_ntrneq0_evtSel_",21,-0.5,20.5);
  trp_nvtxeq1_evtSel = new TrackPlots("nvtxeq1_evtSel_"+trackcoll); 
  trp_nvtxgt1_evtSel = new TrackPlots("nvtxgt1_evtSel_"+trackcoll); 
  vtx1_evtSel    = new VertexPlots("vtx1_evtSel_"+trackcoll);
  vtx2_evtSel    = new VertexPlots("vtx2_evtSel_"+trackcoll);
  vtx3_evtSel    = new VertexPlots("vtx3_evtSel_"+trackcoll);

  trp_smallzvtx_evtSel = new TrackPlots("smallzvtx_evtSel_"+trackcoll); 
  trp_mediumzvtx_evtSel = new TrackPlots("mediumzvtx_evtSel_"+trackcoll);
  trp_largezvtx_evtSel = new TrackPlots("largezvtx_evtSel_"+trackcoll);
  
  vtxqual_noSel  = new TH2F("vtxqual_noSel_"+trackcoll,"vtxqual_noSel",100,0.,2000.,20,0.,10.);
  vtxqual_b40Sel = new TH2F("vtxqual_b40Sel_"+trackcoll,"vtxqual_b40Sel",100,0.,2000.,20,0.,10.);
  vtxqual_L1Sel  = new TH2F("vtxqual_L1Sel_"+trackcoll,"vtxqual_L1Sel",100,0.,2000.,20,0.,10.);
  vtxqual_hfSel  = new TH2F("vtxqual_hfSel_"+trackcoll,"vtxqual_hfSel",100,0.,2000.,20,0.,10.);
  vtxqual_vtxqualSel = new TH2F("vtxqual_vtxqualSel_"+trackcoll,"vtxqual_vtxqualSel",100,0.,2000.,20,0.,10.);
  vtxqual_vtxSel = new TH2F("vtxqual_vtxSel_"+trackcoll,"vtxqual_vtxSel",100,0.,2000.,20,0.,10.);
  vtxqual_L1_hfSel      = new TH2F("vtxqual_L1_hfSel_"+trackcoll,"vtxqual_L1_hfSel",100,0.,2000.,20,0.,10.);
  vtxqual_L1_hf_vtxqualSel  = new TH2F("vtxqual_L1_hf_vtxqualSel_"+trackcoll,"vtxqual_L1_hf_vtxqualSel",100,0.,2000.,20,0.,10.);
  vtxqual_L1_hf_vtxqual_vtxSel  = new TH2F("vtxqual_L1_hf_vtxqual_vtxSel_"+trackcoll,"vtxqual_L1_hf_vtxqual_vtxSel",100,0.,2000.,20,0.,10.);
  vtxqual_L1_b40_vtxqualSel = new TH2F("vtxqual_L1_b40_vtxqualSel_"+trackcoll,"vtxqual_L1_b40_vtxqualSel",100,0.,2000.,20,0.,10.);
  vtxqual_noSel->Sumw2();
  vtxqual_b40Sel->Sumw2();
  vtxqual_L1Sel->Sumw2();
  vtxqual_hfSel->Sumw2();
  vtxqual_vtxqualSel->Sumw2();
  vtxqual_vtxSel->Sumw2();
  vtxqual_L1_hfSel->Sumw2();
  vtxqual_L1_hf_vtxqualSel->Sumw2();
  vtxqual_L1_hf_vtxqual_vtxSel->Sumw2();
  vtxqual_L1_b40_vtxqualSel->Sumw2();
}

void EvtSelPlots::fill(vector<MyTracks>& trcoll, vector<MyVertex>& vtxcoll, MyBeamSpot* bs, int vtxId, double vtxQual, int npixhits){
  
  //std::cout<<trackcoll<<std::endl;
  
  vector<MyVertex>::iterator goodVtx = vtxcoll.end();
  for(vector<MyVertex>::iterator it_vtx = vtxcoll.begin();it_vtx != vtxcoll.end();++it_vtx)
    if(vtxId==it_vtx->id)
      goodVtx = it_vtx;
  
  //nch_noSel->Fill(trcoll.size());
  trp_noSel->fill(trcoll,vtxcoll,vtxId,bs);
  if(goodVtx != vtxcoll.end()) vtxp_noSel->fill(*goodVtx);
  vtxqual_noSel->Fill(npixhits,vtxQual);
  
  
  if(passBit40){
    //nch_b40Sel->Fill(trcoll.size());
    trp_b40Sel->fill(trcoll,vtxcoll,vtxId,bs);
    if(goodVtx != vtxcoll.end()) vtxp_b40Sel->fill(*goodVtx);
    vtxqual_b40Sel->Fill(npixhits,vtxQual);
  }
  
  if(passL1){
    //nch_L1Sel->Fill(trcoll.size());
    trp_L1Sel->fill(trcoll,vtxcoll,vtxId,bs);
    if(goodVtx != vtxcoll.end()) vtxp_L1Sel->fill(*goodVtx);
    vtxqual_L1Sel->Fill(npixhits,vtxQual);
  }
  
  if(passHF){
    //nch_hfSel->Fill(trcoll.size());
    trp_hfSel->fill(trcoll,vtxcoll,vtxId,bs);
    if(goodVtx != vtxcoll.end()) vtxp_hfSel->fill(*goodVtx);
    vtxqual_hfSel->Fill(npixhits,vtxQual);
  }
  
  if(passVtxQual){
    //nch_vtxSel->Fill(trcoll.size());
    trp_vtxqualSel->fill(trcoll,vtxcoll,vtxId,bs);
    if(goodVtx != vtxcoll.end()) vtxp_vtxqualSel->fill(*goodVtx);
    vtxqual_vtxqualSel->Fill(npixhits,vtxQual);
  }
  
  if(passVtx){
    //nch_vtxSel->Fill(trcoll.size());
    trp_vtxSel->fill(trcoll,vtxcoll,vtxId,bs);
    if(goodVtx != vtxcoll.end()) vtxp_vtxSel->fill(*goodVtx);
    vtxqual_vtxSel->Fill(npixhits,vtxQual);
  }
  
  if(passL1 && passHF){
    //nch_L1_hfSel->Fill(trcoll.size());
    trp_L1_hfSel->fill(trcoll,vtxcoll,vtxId,bs);
    if(goodVtx != vtxcoll.end()) vtxp_L1_hfSel->fill(*goodVtx);
    vtxqual_L1_hfSel->Fill(npixhits,vtxQual);
  }  
  
  if(passL1 && passHF && passVtxQual){
    //nch_L1_hf_vtxSel->Fill(trcoll.size());
    trp_L1_hf_vtxqualSel->fill(trcoll,vtxcoll,vtxId,bs);
    if(goodVtx != vtxcoll.end()) vtxp_L1_hf_vtxqualSel->fill(*goodVtx);
    vtxqual_L1_hf_vtxqualSel->Fill(npixhits,vtxQual);
  }
 
  // Final selection 
  if(passL1 && passHF && passVtxQual && passVtx){
    //nch_L1_hf_vtxSel->Fill(trcoll.size());
    trp_L1_hf_vtxqual_vtxSel->fill(trcoll,vtxcoll,vtxId,bs);
    if(goodVtx != vtxcoll.end()) vtxp_L1_hf_vtxqual_vtxSel->fill(*goodVtx);
    vtxqual_L1_hf_vtxqual_vtxSel->Fill(npixhits,vtxQual);
    nvtx_evtSel->Fill(vtxcoll.size());
    int nvtx_ntrneq0 = 0;
    for(vector<MyVertex>::iterator vtx = vtxcoll.begin() ; vtx != vtxcoll.end() ; ++vtx){
      if (vtx-vtxcoll.begin() == 0) vtx1_evtSel->fill( *vtx );
      if (vtx-vtxcoll.begin() == 1) vtx2_evtSel->fill( *vtx );
      if (vtx-vtxcoll.begin() == 2) vtx3_evtSel->fill( *vtx );
      if (vtx->ntracks > 0 ) ++nvtx_ntrneq0;   
    }
    nvtx_ntrneq0_evtSel->Fill(nvtx_ntrneq0);
    if ( nvtx_ntrneq0 == 1 ) trp_nvtxeq1_evtSel->fill(trcoll,vtxcoll,vtxId,bs);
    if ( nvtx_ntrneq0 >  1 ) trp_nvtxgt1_evtSel->fill(trcoll,vtxcoll,vtxId,bs);

    if(goodVtx != vtxcoll.end()) {
      if ( fabs(goodVtx->z) < 1 ) trp_smallzvtx_evtSel->fill(trcoll,vtxcoll,vtxId,bs);
      if ( fabs(goodVtx->z) < 5 && fabs(goodVtx->z) >=1 )
                                   trp_mediumzvtx_evtSel->fill(trcoll,vtxcoll,vtxId,bs);
      if ( fabs(goodVtx->z) >= 5 ) trp_largezvtx_evtSel->fill(trcoll,vtxcoll,vtxId,bs);
    }
  }

  // Final selection but HF 
  if(passL1 && passVtxQual && passVtx ){
    //nch_L1_b40_vtxSel->Fill(trcoll.size());
    trp_L1_vtxqual_vtxSel->fill(trcoll,vtxcoll,vtxId,bs);
    if(goodVtx != vtxcoll.end()) vtxp_L1_vtxqual_vtxSel->fill(*goodVtx);
    // vtxqual_L1_b40_vtxqualSel->Fill(npixhits,vtxQual);
  }
 
  if(passL1 && passBit40 && passVtxQual){
    //nch_L1_b40_vtxSel->Fill(trcoll.size());
    trp_L1_b40_vtxqualSel->fill(trcoll,vtxcoll,vtxId,bs);
    if(goodVtx != vtxcoll.end()) vtxp_L1_b40_vtxqualSel->fill(*goodVtx);
    vtxqual_L1_b40_vtxqualSel->Fill(npixhits,vtxQual);
  }
}

void EvtSelPlots::write(){
  //TDirectory* dir = new TDirectory("EvtSelPlots_"+trackcoll,"");
  //dir->cd();  
  gDirectory->mkdir("EvtSelPlots_"+trackcoll);
  gDirectory->cd("EvtSelPlots_"+trackcoll);
  
  /*nch_noSel->Write();
  nch_b40Sel->Write();
  nch_L1Sel->Write();
  nch_hfSel->Write();
  nch_vtxSel->Write();
  nch_L1_hfSel->Write();
  nch_L1_hf_vtxSel->Write();
  nch_L1_b40_vtxSel->Write();*/
  /*
  this->makeEff(nch_noSel);
  this->makeEff(nch_b40Sel);
  this->makeEff(nch_L1Sel);
  this->makeEff(nch_hfSel);
  this->makeEff(nch_vtxSel);
  this->makeEff(nch_L1_hfSel);
  this->makeEff(nch_L1_hf_vtxSel);
  this->makeEff(nch_L1_b40_vtxSel);*/
  
  vtxqual_noSel->Write();
  vtxqual_b40Sel->Write();
  vtxqual_L1Sel->Write();
  vtxqual_hfSel->Write();
  vtxqual_vtxqualSel->Write();
  vtxqual_vtxSel->Write();
  vtxqual_L1_hfSel->Write();
  vtxqual_L1_hf_vtxqualSel->Write();
  vtxqual_L1_hf_vtxqual_vtxSel->Write();
  vtxqual_L1_b40_vtxqualSel->Write();
  
  trp_noSel->write();
  trp_b40Sel->write();
  trp_L1Sel->write();
  trp_hfSel->write();
  trp_vtxSel->write();
  trp_vtxqualSel->write();
  trp_L1_hfSel->write();
  trp_L1_hf_vtxqualSel->write();
  trp_L1_hf_vtxqual_vtxSel->write();
  trp_L1_vtxqual_vtxSel->write();
  trp_L1_b40_vtxqualSel->write();
  
  
  vtxp_noSel->write();
  vtxp_b40Sel->write();
  vtxp_L1Sel->write();
  vtxp_hfSel->write();
  vtxp_vtxqualSel->write();
  vtxp_vtxSel->write();
  vtxp_L1_hfSel->write();
  vtxp_L1_hf_vtxqualSel->write();
  vtxp_L1_hf_vtxqual_vtxSel->write();
  vtxp_L1_vtxqual_vtxSel->write();
  vtxp_L1_b40_vtxqualSel->write();
 
  nvtx_evtSel->Write();
  nvtx_ntrneq0_evtSel->Write();
  trp_nvtxeq1_evtSel->write();
  trp_nvtxgt1_evtSel->write();
  vtx1_evtSel->write();
  vtx2_evtSel->write();
  vtx3_evtSel->write();

  trp_smallzvtx_evtSel->write();
  trp_mediumzvtx_evtSel->write();
  trp_largezvtx_evtSel->write();

  gDirectory->cd("../");
}

void EvtSelPlots::setSelBits(bool passl1, bool passhf, bool passvtxqual, bool passvtx, bool passb40){
  passL1      = passl1;
  passHF      = passhf;
  passVtxQual = passvtxqual;
  passVtx     = passvtx;
  passBit40   = passb40;
}

void EvtSelPlots::makeEffPlots(TString denomName){
  gDirectory->cd("EvtSelPlots_"+trackcoll);
  gDirectory->mkdir("Eff_"+denomName+"_"+trackcoll);
  gDirectory->cd("Eff_"+denomName+"_"+trackcoll);
  
  this->makeEffVSvar(trp_noSel);
  this->makeEffVSvar(trp_b40Sel);
  this->makeEffVSvar(trp_L1Sel);
  this->makeEffVSvar(trp_hfSel);
  this->makeEffVSvar(trp_vtxqualSel);
  this->makeEffVSvar(trp_vtxSel);
  this->makeEffVSvar(trp_L1_hfSel);
  this->makeEffVSvar(trp_L1_hf_vtxqualSel);
  this->makeEffVSvar(trp_L1_hf_vtxqual_vtxSel);
  this->makeEffVSvar(trp_L1_vtxqual_vtxSel);
  this->makeEffVSvar(trp_L1_b40_vtxqualSel);
  
  gDirectory->cd("../../");
  
}

void EvtSelPlots::makeEffVSvar(TrackPlots* trp){
  this->makeEff2(trp->nch,nch_denom);
  this->makeEff2(trp->pt,pt_denom);
  this->makeEff2(trp->eta,eta_denom);
}

void EvtSelPlots::makeEff(TH1F* num,TH1F* denom){
  TH1F* clone  = (TH1F*) num->Clone("eff_"+(TString)num->GetName());
  clone->GetYaxis()->SetTitle("efficiency");
  clone->Divide(denom);
  clone->GetYaxis()->SetRangeUser(0.,1.1);
  clone->Write();
}

void EvtSelPlots::makeEff2(TH1F* num, TH1F* denom){
  TH1F* clone  = (TH1F*) num->Clone("eff_"+(TString)num->GetName());
  //clone->Scale(1./double(clone->GetXaxis()->GetBinWidth(1)));
  for(int i=1;i<=num->GetNbinsX();i++){
    double n = double( clone->GetBinContent(i) ) / double(clone->GetXaxis()->GetBinWidth(i));
    double d = denom->GetBinContent(i);
    if( d!=0 ){
      double eff = n / d;
      double err = sqrt(eff*(1-eff)/ d);
      clone->SetBinContent(i,eff);
      clone->SetBinError(i,err);
    }
  }
  clone->GetYaxis()->SetRangeUser(0.,1.1);
  clone->Write();
}

void EvtSelPlots::setDenom(TH1F* nch, TH1F* pt ,  TH1F* eta ){
  nch_denom=nch;pt_denom=pt;eta_denom=eta;
}
    
