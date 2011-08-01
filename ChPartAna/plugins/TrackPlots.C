
#include "TrackPlots.h"

ClassImp(TrackPlots)

//_____________________________________________________________________________
TrackPlots::TrackPlots(){
  trackcoll = "none";
  this->init();
}

//_____________________________________________________________________________
TrackPlots::TrackPlots(TString collname){
  trackcoll = collname;
  this->init();
}

//_____________________________________________________________________________
TrackPlots::~TrackPlots(){ 
    delete charge;
    delete nch;
    delete pt;
    delete eta;
    delete phi;
    delete nhit;
    delete chi2n;
    delete dz;
    delete d0;
    delete d0bs;
    delete edz;
    delete ed0;
    delete dzOedz;
    delete dxyOed0;
    delete dxyOsxy;
    delete dxybsOsxy;
    delete dzOsz;
    delete eptOpt;
    delete dxyz;
    delete quality;
    delete fdz;
    delete sz;
    delete sxy;
    delete dzOsz_old;
    delete dzOszVSphi_old;
    delete dxyOsxyVSphi;
    delete dzOszVSphi;
    delete etaVSnhit;
}

//_____________________________________________________________________________
void TrackPlots::init(){
  charge    = new TH1F("charge_"+trackcoll,"charge_"+trackcoll+";charge;# events",3,-1.5,1.5);
  nch       = new TH1F("nch_"+trackcoll,"nch_"+trackcoll+";n_{CH};# events",nch_nbin,nch_array);
  pt        = new TH1F("pt_"+trackcoll,"pt_"+trackcoll+";pt [GeV];# events",pt_nbin,pt_array);
  eta       = new TH1F("eta_"+trackcoll,"eta_"+trackcoll+";#eta;# events",eta_nbin,eta_array);  
  phi       = new TH1F("phi_"+trackcoll,"phi_"+trackcoll+";#phi;# events",30,-TMath::Pi(),TMath::Pi());
  nhit      = new TH1F("nhit_"+trackcoll,"nhit_"+trackcoll+";nhit;# events",51,-0.5,50.5);
  chi2n     = new TH1F("chi2n_"+trackcoll,"chi2n_"+trackcoll+";#chi^{2}/ndof;# events",25,0.,5.);
  dz        = new TH1F("dz_"+trackcoll,"dz_"+trackcoll+";dz(vertex) [cm];# events",120,-30.,30.);
  d0        = new TH1F("d0_"+trackcoll,"d0_"+trackcoll+";d0(vertex) [cm];# events",50,-2.5,2.5);
  d0bs      = new TH1F("d0bs_"+trackcoll,"d0bs_"+trackcoll+";d0(beamspot) [cm];# events",50,-2.5,2.5);
  edz       = new TH1F("edz_"+trackcoll,"edz_"+trackcoll+";dzError(0,0,0);# events", 50,0.,5.);
  ed0       = new TH1F("ed0_"+trackcoll,"ed0_"+trackcoll+";d0Error(0,0,0);# events", 50,0.,5.);
  dzOedz    = new TH1F("dzOedz_"+trackcoll,"dzOedz_"+trackcoll+";dz(vtx)/dzError;# events",60,0,15.);
  dxyOed0   = new TH1F("dxyOed0_"+trackcoll,"dxyOed0_"+trackcoll+";d0(vtx)/d0Error;# events",60,0,15.);
  dxyOsxy   = new TH1F("dxyOsxy_"+trackcoll,"dxyOsxy_"+trackcoll+";#dxy(vtx) / #sigma_{xy};# events", 50,0,10.);
  dxybsOsxy = new TH1F("dxybsOsxy_"+trackcoll,"dxybsOsxy_"+trackcoll+";#dxy(bs) / #sigma_{xy};# events", 50,0,10.);
  dzOsz     = new TH1F("dzOsz_"+trackcoll,"dzOsz_"+trackcoll+";#dz / #sigma_{z};# events", 50,0,10.);
  eptOpt    = new TH1F("eptOpt_"+trackcoll,"eptOpt_"+trackcoll+";pTerror / p_{T};# events",80,0,0.8);
  dxyz      = new TH1F("dxyz_"+trackcoll,"dxyz_"+trackcoll+";dxyz;# events",60,0,15.);

  quality   = new TH1F("quality_"+trackcoll,"quality_"+trackcoll+";quality;# events",4,-0.5,3.5);
  
  fdz       = new TH1F("fdz_"+trackcoll,"fdz_"+trackcoll+";track.vertex.z - vertex.z [cm];# events",120,-30.,30.);
  sz        = new TH1F("sz_"+trackcoll,"sz_"+trackcoll+";#sigma_{Z};# events", 50,0.,5.);
  sxy       = new TH1F("sxy_"+trackcoll,"sxy_"+trackcoll+";#sigma_{XY};# events", 50,0.,5.);
  
  dzOsz_old      = new TH1F("dzOsz_old_"+trackcoll,"dzOsz_old_"+trackcoll+";#dz / sigma_z;# events", 50,0,10.);
  dzOszVSphi_old = new TH2F("dzOszVSphi_old_"+trackcoll,"dzOszVSphi_old_"+trackcoll+";#dz / sigma_z;#phi;# events", 50,0,10.,30,-TMath::Pi(),TMath::Pi());
  
  dxyOsxyVSphi = new TH2F("dxyOsxyVSphi_"+trackcoll,"dxyOsxyVSphi_"+trackcoll+";#dxy / sigma_xy;#phi;# events", 50,0,10.,30,-TMath::Pi(),TMath::Pi());
  dzOszVSphi   = new TH2F("dzOszVSphi_"+trackcoll,"dzOszVSphi_"+trackcoll+";#dz / sigma_z;#phi;# events", 50,0,10.,30,-TMath::Pi(),TMath::Pi());
  
  etaVSnhit = new TH2F("etaVSnhit"+trackcoll,"etaVSnhit"+trackcoll+";#eta;nhit;# events",20,-3.,3.,51,-0.5,50.5);

  nch           ->Sumw2();
  pt            ->Sumw2();
  eta           ->Sumw2();
  phi           ->Sumw2();
  nhit          ->Sumw2();
  chi2n         ->Sumw2();
  dz            ->Sumw2();
  d0            ->Sumw2();
  d0bs          ->Sumw2();
  edz           ->Sumw2();
  ed0           ->Sumw2();
  dzOedz        ->Sumw2();
  dxyOed0       ->Sumw2();
  dxyOsxy       ->Sumw2();
  dxybsOsxy     ->Sumw2();
  dzOsz         ->Sumw2();
  eptOpt        ->Sumw2();
  dzOsz_old     ->Sumw2();
  dzOszVSphi_old->Sumw2();
  dxyOsxyVSphi  ->Sumw2();
  dzOszVSphi    ->Sumw2();
  dxyz          ->Sumw2();
  etaVSnhit     ->Sumw2();
}

//_____________________________________________________________________________
void TrackPlots::fill(vector<MyTracks>& trcoll, vector<MyVertex>::iterator& goodVtx,int vtxId, MyBeamSpot* bs, double weight){
  
  int n = 0;
  for(vector<MyTracks>::iterator tr = trcoll.begin() ; tr != trcoll.end() ; ++tr){
  
    //if(! (tr->Phi()>2 || tr->Phi()<1) ) continue;
    
    int vtxnum = this->getVtxposFromId(*tr,vtxId);
    //if(vtxnum==-1) vtxnum = 0;
    ++n;
    charge  ->Fill(tr       ->charge,weight);
    pt      ->Fill(tr       ->Pt(),weight);
    eta     ->Fill(tr       ->Eta(),weight);
    phi     ->Fill(tr       ->Phi(),weight);
    nhit    ->Fill(tr       ->nhit,weight);
    chi2n   ->Fill(tr       ->chi2n,weight);
    if(vtxnum!=-1)d0        ->Fill(tr->vtxdxy.at(vtxnum),weight);
    d0bs    ->Fill(tr       ->vtxdxy.at(0),weight);
    if(vtxnum!=-1)dz        ->Fill(tr->vtxdz.at(vtxnum),weight);
    ed0     ->Fill(tr       ->ed0,weight);
    edz     ->Fill(tr       ->edz,weight);
    if(vtxnum!=-1)dxyOed0   ->Fill(fabs(tr->vtxdxy.at(vtxnum)) / tr->ed0,weight);
    if(vtxnum!=-1)dzOedz    ->Fill(fabs(tr->vtxdz.at(vtxnum)) / tr->edz,weight);
    if(vtxnum!=-1)dxyz      ->Fill(sqrt(tr->vtxdz.at(vtxnum)*tr->vtxdz.at(vtxnum)+tr->vtxdxy.at(vtxnum)*tr->vtxdxy.at(vtxnum)),weight);    
    if(vtxnum!=-1)dxyOsxy   ->Fill( fabs(tr->vtxdxy.at(vtxnum)) / sqrt( pow(tr->ed0,2) + bs->BeamWidthX * bs->BeamWidthY ) ,weight);
    dxybsOsxy->Fill( fabs(tr->vtxdxy.at(0)) / sqrt( pow(tr->ed0,2) + bs->BeamWidthX * bs->BeamWidthY ) ,weight);
    if(vtxnum!=-1)dzOsz_old ->Fill(fabs(tr->vtxdz.at(vtxnum)) / sqrt( pow(tr->edz,2) + pow(cosh(tr->Eta()),2) * bs->BeamWidthX * bs->BeamWidthY ) ,weight);
    if(vtxId!=-1)dzOsz      ->Fill( fabs(tr->vz - goodVtx->z) /sqrt( pow(tr->edz,2) + pow(cosh(tr->Eta()),2) * bs->BeamWidthX * bs->BeamWidthY ) ,weight);
  
    int iQual = 3;
    for ( ; iQual != -1 ; --iQual )
      if ( tr->quality[iQual-1] ) break; 
    quality->Fill(iQual,weight);
 
    if(vtxId!=-1)fdz    ->Fill( tr->vz - goodVtx->z ,weight);
    if(vtxId!=-1)sz     ->Fill( sqrt( pow(tr->edz,2) + pow(cosh(tr->Eta()),2) * bs->BeamWidthX * bs->BeamWidthY ) ,weight);
    if(vtxId!=-1)sxy    ->Fill(sqrt( pow(tr->ed0,2) + bs->BeamWidthX * bs->BeamWidthY ),weight);
    
    if(vtxnum!=-1)dxyOsxyVSphi  ->Fill( fabs(tr->vtxdxy.at(vtxnum)) / sqrt( pow(tr->ed0,2) + bs->BeamWidthX * bs->BeamWidthY ), tr->Phi(),weight);
    if(vtxnum!=-1)dzOszVSphi_old->Fill( fabs(tr->vtxdz.at(vtxnum)) / sqrt( pow(tr->edz,2) + pow(cosh(tr->Eta()),2) * bs->BeamWidthX * bs->BeamWidthY ), tr->Phi(),weight);
    if(vtxId!=-1)dzOszVSphi     ->Fill( fabs(tr->vz - goodVtx->z) / sqrt( pow(tr->edz,2) + pow(cosh(tr->Eta()),2) * bs->BeamWidthX * bs->BeamWidthY ), tr->Phi(),weight);
    
    eptOpt   ->Fill( tr->ept/ tr->Pt() ,weight);
    
    etaVSnhit->Fill(tr->Eta() , tr->nhit , weight);
  }
  nch->Fill(n, weight);
}

//_____________________________________________________________________________
void TrackPlots::write(){
  //TDirectory* dir = new TDirectory("TrackPlots_"+trackcoll,"");
  //dir->cd();  
  gDirectory->mkdir("Track_"+trackcoll);
  gDirectory->cd("Track_"+trackcoll);
  
  charge        ->Write();
  nch           ->Write();
  pt            ->Write();
  eta           ->Write();
  phi           ->Write();
  nhit          ->Write();
  chi2n         ->Write();
  dz            ->Write();
  d0            ->Write();
  d0bs          ->Write();
  edz           ->Write();
  ed0           ->Write();
  dzOedz        ->Write();
  dxyOed0       ->Write();
  dxyOsxy       ->Write();
  dxybsOsxy     ->Write();
  dzOsz         ->Write();
  eptOpt        ->Write();

  quality       ->Write(); 
 
  fdz           ->Write();
  sz            ->Write();
  sxy           ->Write();
  
  dzOsz_old     ->Write();
  dzOszVSphi_old->Write();
  
  dxyOsxyVSphi  ->Write();
  dzOszVSphi    ->Write();
 
  dxyz          ->Write();

  etaVSnhit     ->Write();
 
  gDirectory->cd("../");
}

//_____________________________________________________________________________
int TrackPlots::getVtxposFromId(MyTracks& tr,int goodvtxId){
  for(unsigned int i=0;i<tr.vtxid.size();++i)
    if(tr.vtxid.at(i)==goodvtxId)
      return i;
  
  return -1;
}

//_____________________________________________________________________________
void TrackPlots::divide(TrackPlots& trp, const TString name){
  
  gDirectory->mkdir("Eff_"+name);
  gDirectory->cd("Eff_"+name);
  
  this->makeEff( charge    , trp.charge    , "eff_charge_"   + name);
  this->makeEff( nch       , trp.nch       , "eff_nch_"      + name);
  this->makeEff( pt        , trp.pt        , "eff_pt_"       + name);
  this->makeEff( eta       , trp.eta       , "eff_eta_"      + name);
  this->makeEff( phi       , trp.phi       , "eff_phi_"      + name);
  this->makeEff( nhit      , trp.nhit      , "eff_nhit_"     + name);
  this->makeEff( chi2n     , trp.chi2n     , "eff_chi2n_"    + name);
  this->makeEff( dz        , trp.dz        , "eff_dz_"       + name);
  this->makeEff( d0        , trp.d0        , "eff_d0_"       + name);
  this->makeEff( d0bs      , trp.d0bs      , "eff_d0bs_"     + name);
  this->makeEff( edz       , trp.edz       , "eff_edz_"      + name);
  this->makeEff( ed0       , trp.ed0       , "eff_ed0_"      + name);
  this->makeEff( dzOedz    , trp.dzOedz    , "eff_dzOedz_"   + name);
  this->makeEff( dxyOed0   , trp.dxyOed0   , "eff_xyOed0_"   + name);
  this->makeEff( dxyOsxy   , trp.dxyOsxy   , "eff_dxyOsxy_"  + name);
  this->makeEff( dxybsOsxy , trp.dxybsOsxy , "eff_dxybsOsxy_"+ name);
  this->makeEff( dzOsz     , trp.dzOsz     , "eff_zOsz_"     + name);
  this->makeEff( eptOpt    , trp.eptOpt    , "eff_eptOpt_"   + name);

  this->makeEff( quality   , trp.quality   , "eff_quality_"  + name); 
 
  this->makeEff( fdz , trp.fdz , "eff_fdz_" + name);
  this->makeEff( sz  , trp.sz  , "eff_sz_"  + name);
  this->makeEff( sxy , trp.sxy , "eff_xy_"  + name);
  
  this->makeEff( dzOsz_old      , trp.dzOsz_old      , "eff_dzOsz_"          + name);
  this->makeEff( dzOszVSphi_old , trp.dzOszVSphi_old , "eff_dzOszVSphi_old_" + name);
  
  this->makeEff( dxyOsxyVSphi , trp.dxyOsxyVSphi , "eff_dxyOsxyVSphi_" + name);
  this->makeEff( dzOszVSphi   , trp.dzOszVSphi   , "eff_dzOszVSphi_"   + name);
 
  this->makeEff( dxyz         , trp.dxyz         , "eff_dxyz_"         + name);

  this->makeEff( etaVSnhit    , trp.etaVSnhit    , "eff_etaVSnhit_"    + name);
  
  gDirectory->cd("../");
}

//_____________________________________________________________________________
template <class T>
void TrackPlots::makeEff( T* num, T* denom, const TString name){
  T* eff    = (T*) num->Clone(name);
  eff->Divide(num , denom , 1 , 1 , "B");
  eff->SetMinimum(0);
  eff->Write();
}

