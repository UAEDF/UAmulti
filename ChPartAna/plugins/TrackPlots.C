
#include "TrackPlots.h"

ClassImp(TrackPlots)

TrackPlots::TrackPlots(){
  trackcoll = "none";
  this->init();
}

TrackPlots::TrackPlots(TString collname){
  trackcoll = collname;
  this->init();
}

TrackPlots::~TrackPlots(){ }

void TrackPlots::init(){
  charge   = new TH1F("charge_"+trackcoll,"charge_"+trackcoll+";charge;# events",3,-1.5,1.5);
  nch   = new TH1F("nch_"+trackcoll,"nch_"+trackcoll+";n_{CH};# events",71,-0.5,70.5);
  pt    = new TH1F("pt_"+trackcoll,"pt_"+trackcoll+";pt [GeV];# events",100,0.,3.);
  eta   = new TH1F("eta_"+trackcoll,"eta_"+trackcoll+";#eta;# events",60,-3.,3.);
  phi   = new TH1F("phi_"+trackcoll,"phi_"+trackcoll+";#phi;# events",30,-TMath::Pi(),TMath::Pi());
  nhit  = new TH1F("nhit_"+trackcoll,"nhit_"+trackcoll+";nhit;# events",51,-0.5,50.5);
  chi2n = new TH1F("chi2n_"+trackcoll,"chi2n_"+trackcoll+";#chi^{2}/ndof;# events",50,0.,5.);
  dz    = new TH1F("dz_"+trackcoll,"dz_"+trackcoll+";dz(vertex) [cm];# events",120,-30.,30.);
  d0    = new TH1F("d0_"+trackcoll,"d0_"+trackcoll+";d0(vertex) [cm];# events",50,-2.5,2.5);
  d0bs  = new TH1F("d0bs_"+trackcoll,"d0bs_"+trackcoll+";d0(beamspot) [cm];# events",50,-2.5,2.5);
  edz   = new TH1F("edz_"+trackcoll,"edz_"+trackcoll+";dzError(0,0,0);# events",100,0.,5.);
  ed0   = new TH1F("ed0_"+trackcoll,"ed0_"+trackcoll+";d0Error(0,0,0);# events",100,0.,5.);
  dzOedz    = new TH1F("dzOedz_"+trackcoll,"dzOedz_"+trackcoll+";dz(vtx)/dzError;# events",60,0,15.);
  dxyOed0   = new TH1F("dxyOed0_"+trackcoll,"dxyOed0_"+trackcoll+";d0(vtx)/d0Error;# events",60,0,15.);
  dxyOsxy   = new TH1F("dxyOsxy_"+trackcoll,"dxyOsxy_"+trackcoll+";#dxy(vtx) / #sigma_{xy};# events",100,0,10.);
  dxybsOsxy = new TH1F("dxybsOsxy_"+trackcoll,"dxybsOsxy_"+trackcoll+";#dxy(bs) / #sigma_{xy};# events",100,0,10.);
  dzOsz     = new TH1F("dzOsz_"+trackcoll,"dzOsz_"+trackcoll+";#dz / #sigma_{z};# events",100,0,10.);
  eptOpt    = new TH1F("eptOpt_"+trackcoll,"eptOpt_"+trackcoll+";pTerror / p_{T};# events",80,0,0.8);
  
  fdz    = new TH1F("fdz_"+trackcoll,"fdz_"+trackcoll+";track.vertex.z - vertex.z [cm];# events",120,-30.,30.);
  sz     = new TH1F("sz_"+trackcoll,"sz_"+trackcoll+";#sigma_{Z};# events",100,0.,5.);
  sxy    = new TH1F("sxy_"+trackcoll,"sxy_"+trackcoll+";#sigma_{XY};# events",100,0.,5.);
  
  dzOsz_old = new TH1F("dzOsz_old_"+trackcoll,"dzOsz_old_"+trackcoll+";#dz / sigma_z;# events",100,0,10.);
  dzOszVSphi_old = new TH2F("dzOszVSphi_old_"+trackcoll,"dzOszVSphi_old_"+trackcoll+";#dz / sigma_z;#phi;# events",100,0,10.,30,-TMath::Pi(),TMath::Pi());
  
  dxyOsxyVSphi = new TH2F("dxyOsxyVSphi_"+trackcoll,"dxyOsxyVSphi_"+trackcoll+";#dxy / sigma_xy;#phi;# events",100,0,10.,30,-TMath::Pi(),TMath::Pi());
  dzOszVSphi = new TH2F("dzOszVSphi_"+trackcoll,"dzOszVSphi_"+trackcoll+";#dz / sigma_z;#phi;# events",100,0,10.,30,-TMath::Pi(),TMath::Pi());

  nch->Sumw2();
  pt->Sumw2();
  eta->Sumw2();
  phi->Sumw2();
  nhit->Sumw2();
  chi2n->Sumw2();
  dz->Sumw2();
  d0->Sumw2();
  d0bs->Sumw2();
  edz->Sumw2();
  ed0->Sumw2();
  dzOedz->Sumw2();
  dxyOed0->Sumw2();
  dxyOsxy->Sumw2();
  dxybsOsxy->Sumw2();
  dzOsz->Sumw2();
  eptOpt->Sumw2();
  dzOsz_old->Sumw2();
  dzOszVSphi_old->Sumw2();
  dxyOsxyVSphi->Sumw2();
  dzOszVSphi->Sumw2();
}

void TrackPlots::fill(vector<MyTracks>& trcoll, vector<MyVertex>& vtxcoll, int vtxId, MyBeamSpot* bs, double weight){
  //get the good vertex iterator
  vector<MyVertex>::iterator goodVtx = vtxcoll.end();
  for(vector<MyVertex>::iterator it_vtx = vtxcoll.begin();it_vtx != vtxcoll.end();++it_vtx)
    if(vtxId==it_vtx->id)
      goodVtx = it_vtx;
  
  int n = 0;
  for(vector<MyTracks>::iterator tr = trcoll.begin() ; tr != trcoll.end() ; ++tr){
  
    //if(! (tr->Part.v.Phi()>2 || tr->Part.v.Phi()<1) ) continue;
    
    int vtxnum = this->getVtxposFromId(*tr,vtxId);
    //if(vtxnum==-1) vtxnum = 0;
    ++n;
    charge->Fill(tr->Part.charge,weight);
    pt->Fill(tr->Part.v.Pt(),weight);
    eta->Fill(tr->Part.v.Eta(),weight);
    phi->Fill(tr->Part.v.Phi(),weight);
    nhit->Fill(tr->nhit,weight);
    chi2n->Fill(tr->chi2n,weight);
    if(vtxnum!=-1)d0->Fill(tr->vtxdxy.at(vtxnum),weight);
    d0bs->Fill(tr->vtxdxy.at(0),weight);
    if(vtxnum!=-1)dz->Fill(tr->vtxdz.at(vtxnum),weight);
    ed0->Fill(tr->ed0,weight);
    edz->Fill(tr->edz,weight);
    if(vtxnum!=-1)dxyOed0->Fill(fabs(tr->vtxdxy.at(vtxnum)) / tr->ed0,weight);
    if(vtxnum!=-1)dzOedz->Fill(fabs(tr->vtxdz.at(vtxnum)) / tr->edz,weight);
    
    if(vtxnum!=-1)dxyOsxy->Fill( fabs(tr->vtxdxy.at(vtxnum)) / sqrt( pow(tr->ed0,2) + bs->BeamWidthX * bs->BeamWidthY ) ,weight);
    dxybsOsxy->Fill( fabs(tr->vtxdxy.at(0)) / sqrt( pow(tr->ed0,2) + bs->BeamWidthX * bs->BeamWidthY ) ,weight);
    if(vtxnum!=-1)dzOsz_old->Fill(fabs(tr->vtxdz.at(vtxnum)) / sqrt( pow(tr->edz,2) + pow(cosh(tr->Part.v.Eta()),2) * bs->BeamWidthX * bs->BeamWidthY ) ,weight);
    if(vtxId!=-1)dzOsz->Fill( fabs(tr->vz - goodVtx->z) /sqrt( pow(tr->edz,2) + pow(cosh(tr->Part.v.Eta()),2) * bs->BeamWidthX * bs->BeamWidthY ) ,weight);
    
    if(vtxId!=-1)fdz->Fill( tr->vz - goodVtx->z ,weight);
    if(vtxId!=-1)sz->Fill( sqrt( pow(tr->edz,2) + pow(cosh(tr->Part.v.Eta()),2) * bs->BeamWidthX * bs->BeamWidthY ) ,weight);
    if(vtxId!=-1)sxy->Fill(sqrt( pow(tr->ed0,2) + bs->BeamWidthX * bs->BeamWidthY ),weight);
    
    if(vtxnum!=-1)dxyOsxyVSphi->Fill( fabs(tr->vtxdxy.at(vtxnum)) / sqrt( pow(tr->ed0,2) + bs->BeamWidthX * bs->BeamWidthY ), tr->Part.v.Phi(),weight);
    if(vtxnum!=-1)dzOszVSphi_old->Fill( fabs(tr->vtxdz.at(vtxnum)) / sqrt( pow(tr->edz,2) + pow(cosh(tr->Part.v.Eta()),2) * bs->BeamWidthX * bs->BeamWidthY ), tr->Part.v.Phi(),weight);
    if(vtxId!=-1)dzOszVSphi->Fill( fabs(tr->vz - goodVtx->z) / sqrt( pow(tr->edz,2) + pow(cosh(tr->Part.v.Eta()),2) * bs->BeamWidthX * bs->BeamWidthY ), tr->Part.v.Phi(),weight);
    
    eptOpt->Fill( tr->ept/ tr->Part.v.Pt() ,weight);
  }
  
  //nch->Fill(trcoll.size());
  nch->Fill(n);
}

void TrackPlots::write(){
  //TDirectory* dir = new TDirectory("TrackPlots_"+trackcoll,"");
  //dir->cd();  
  gDirectory->mkdir("TrackPlots_"+trackcoll);
  gDirectory->cd("TrackPlots_"+trackcoll);
  
  charge->Write();
  nch->Write();
  pt->Write();
  eta->Write();
  phi->Write();
  nhit->Write();
  chi2n->Write();
  dz->Write();
  d0->Write();
  d0bs->Write();
  edz->Write();
  ed0->Write();
  dzOedz->Write();
  dxyOed0->Write();
  dxyOsxy->Write();
  dxybsOsxy->Write();
  dzOsz->Write();
  eptOpt->Write();
  
  fdz->Write();
  sz->Write();
  sxy->Write();
  
  dzOsz_old->Write();
  dzOszVSphi_old->Write();
  
  dxyOsxyVSphi->Write();
  dzOszVSphi->Write();
  
  gDirectory->cd("../");
}

int TrackPlots::getVtxposFromId(MyTracks& tr,int goodvtxId){
  for(unsigned int i=0;i<tr.vtxid.size();++i)
    if(tr.vtxid.at(i)==goodvtxId)
      return i;
  
  return -1;
}

