
#include "GenPartPlots.h"

ClassImp(GenPartPlots)

//_____________________________________________________________________________
GenPartPlots::GenPartPlots(){
  genpartcoll = "none";
  this->init();
}

//_____________________________________________________________________________
GenPartPlots::GenPartPlots(TString collname){
  genpartcoll = collname;
  this->init();
}

//_____________________________________________________________________________
GenPartPlots::~GenPartPlots(){ 
    delete charge;
    delete nch;
    delete pt;
    delete eta;
    delete phi;
}

//_____________________________________________________________________________
void GenPartPlots::init(){
  charge    = new TH1F("charge_"+genpartcoll,"charge_"+genpartcoll+";charge;# events",3,-1.5,1.5);
  nch       = new TH1F("nch_"+genpartcoll,"nch_"+genpartcoll+";n_{CH};# events",301,-0.5,300.5);
  pt        = new TH1F("pt_"+genpartcoll,"pt_"+genpartcoll+";pt [GeV];# events",100,0.,3.);
  eta       = new TH1F("eta_"+genpartcoll,"eta_"+genpartcoll+";#eta;# events",40,-3.,3.);
  phi       = new TH1F("phi_"+genpartcoll,"phi_"+genpartcoll+";#phi;# events",30,-TMath::Pi(),TMath::Pi());

  nch->Sumw2();
  pt ->Sumw2();
  eta->Sumw2();
  phi->Sumw2();
}

//_____________________________________________________________________________
void GenPartPlots::fill(vector<MyGenPart>& gpcoll, double weight){
  //get the good vertex iterator
  
  int n = 0;
  for(vector<MyGenPart>::const_iterator gp = gpcoll.begin() ; gp != gpcoll.end() ; ++gp){    
      ++n;
      charge->Fill(gp->Part.charge,weight);
      pt    ->Fill(gp->Part.v.Pt(),weight);
      eta   ->Fill(gp->Part.v.Eta(),weight);
      phi   ->Fill(gp->Part.v.Phi(),weight);
  }  
  nch->Fill(n);
}

//_____________________________________________________________________________
void GenPartPlots::write(){
  gDirectory->mkdir("GenPart_"+genpartcoll);
  gDirectory->cd("GenPart_"+genpartcoll);
  
  charge->Write();
  nch   ->Write();
  pt    ->Write();
  eta   ->Write();
  phi   ->Write();
 
  gDirectory->cd("../");
}

//_____________________________________________________________________________
void GenPartPlots::divide(GenPartPlots& trp, const TString name){
  
  gDirectory->mkdir("Eff_"+name);
  gDirectory->cd("Eff_"+name);
  
  this->makeEff( charge    , trp.charge    , "eff_charge_" + name);
  this->makeEff( nch       , trp.nch       , "eff_nch_"    + name);
  this->makeEff( pt        , trp.pt        , "eff_pt_"     + name);
  this->makeEff( eta       , trp.eta       , "eff_eta_"    + name);
  this->makeEff( phi       , trp.phi       , "eff_phi_"    + name);
  
  gDirectory->cd("../");
}

//_____________________________________________________________________________
template <class T>
void GenPartPlots::makeEff( T* num, T* denom, const TString name){
  T* eff    = (T*) num->Clone(name);
  eff->Divide(num , denom , 1 , 1 , "B");
  eff->SetMinimum(0);
  eff->Write();
}
