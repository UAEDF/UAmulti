#include "MultiPlots.h"
using namespace std;

ClassImp(MultiPlots)

//Destructor
MultiPlots::~MultiPlots(){
  delete moments;
  delete nch;
  delete nch_mean;
  delete kno;
  delete rapidity;
  delete eta;
  delete pt;
  delete pt2;
  delete mt;
  delete ptmVSnch;
  delete ptmVSnch_2D;
  delete ptVSnch;
  delete ptVSnch_2D;
  
  delete ptm_inEvt;
}


void MultiPlots::init(){
  nb_moments = 5;
  moments = new vector<TMean>(nb_moments,TMean());
  nch_mean = new TMean();
  
  

  
  nch      = new TH1F("nch_"+plotsname,"nch_"+plotsname+";N_{ch};fraction of events",nch_nbin,nch_array);
  kno      = new TH1F(); //TO BE SAFE
  rapidity = new TH1F("rapidity_"+plotsname,"rapidity_"+plotsname+";y;#frac{1}{N} #frac{d#sigma_{ch}}{dy}",eta_nbin,eta_array);
  eta      = new TH1F("eta_"+plotsname,"eta_"+plotsname+";#eta;#frac{1}{N} #frac{d#sigma_{ch}}{d#eta}",eta_nbin,eta_array);
  pt       = new TH1F("pt_"+plotsname,"pt_"+plotsname+";p_{T} [GeV];#frac{1}{N} #frac{d#sigma_{ch}}{dp_{T}}",pt_nbin,pt_array);
  //pt2      = new TH1F("pt2_"+plotsname,"pt2_"+plotsname+";p_{T}^{2} [GeV^{2}];#frac{1}{N} #frac{d#sigma_{ch}}{dp_{T}^{2}}",100,0.,9.);
  pt2      = new TH1F("pt2_"+plotsname,"pt2_"+plotsname+";p_{T}^{2} [GeV^{2}];#frac{1}{N} #frac{d#sigma_{ch}}{dp_{T}^{2}}",pt2_nbin,pt2_array);
  mt       = new TH1F("mt_"+plotsname,"mt_"+plotsname+";p_{T}^{2} [GeV^{2}];#frac{1}{N} #frac{d#sigma_{ch}}{dp_{T}^{2}}",mt_nbin,mt_array);
  ptmVSnch = new TProfile("ptmVSnch_"+plotsname,"ptmVSnch_"+plotsname+";N_{ch};< <p_{t}>^{evt} >",nch_nbin,nch_array);
  ptVSnch  = new TProfile("ptVSnch_"+plotsname,"ptVSnch_"+plotsname+";N_{ch};<p_{t}>",nch_nbin,nch_array);
  
  ptmVSnch_2D = new TH2F("ptmVSnch_2D_"+plotsname,"ptmVSnch_2D_"+plotsname+";N_{ch};<p_{t}>;# evts",nch_nbin,nch_array,mpt_nbin,mpt_array);
  ptVSnch_2D = new TH2F("ptVSnch_2D_"+plotsname,"ptVSnch_2D_"+plotsname+";N_{ch};p_{t};# evts",nch_nbin,nch_array,pt_nbin,pt_array);
  
  nch->Sumw2();
  rapidity->Sumw2();
  eta->Sumw2();
  pt->Sumw2();
  pt2->Sumw2();
  mt->Sumw2();
  //ptmVSnch->Sumw2();
  //ptVSnch->Sumw2();
  ptmVSnch_2D->Sumw2();
  ptVSnch_2D->Sumw2();
    
  ptmVSnch->SetErrorOption("i");
  ptVSnch->SetErrorOption("i");
    
  energy    = 900;
  nbEvts    = 0;
  nch_inEvt = 0;
  ptm_inEvt = new TMean();
  //vpt_inEvt = new vector<double>();
  
  

}

void MultiPlots::fill(MyPart& p , double weight){
  ++nch_inEvt;
  rapidity->Fill( p.Rapidity() , weight );
  eta->Fill( p.Eta() , weight );
  pt->Fill( p.Pt() , weight );
  pt2->Fill( pow(p.Pt(),2) , weight );
  mt->Fill( sqrt(pow(p.Pt(),2)+pow(0.1396,2))-0.1396 ); 
 
  ptm_inEvt->Add( p.Pt() , weight );
  vpt_inEvt.push_back( p.Pt() * weight );
}

void MultiPlots::nextEvent(bool laccept , double weight){
  ++nbEvts;
  if(laccept){
    nch->Fill( nch_inEvt , weight );
    nch_mean->Add( nch_inEvt , weight );
    for(int i = 0 ; unsigned(i) < moments->size() ; ++i)
      moments->at(i).Add( pow(double(nch_inEvt),i+1) , weight );
    ptmVSnch->Fill(nch_inEvt,ptm_inEvt->GetMean());
    ptmVSnch_2D->Fill(nch_inEvt,ptm_inEvt->GetMean() , weight);
    for(unsigned int i=0;i<vpt_inEvt.size();++i){
      ptVSnch->Fill(nch_inEvt,vpt_inEvt.at(i));
      ptVSnch_2D->Fill(nch_inEvt,vpt_inEvt.at(i) , weight);
    }
  }
  
  //re-initializing nch & <pt>
  nch_inEvt = 0;
  ptm_inEvt->Reset();
  vpt_inEvt.clear();
}

void MultiPlots::makeKNO(){
  kno = new TH1F("kno_"+plotsname,"kno_"+plotsname+";z = n_{ch} / < n_{ch} >;#psi(z)",nch->GetNbinsX(),0.,double(nch->GetXaxis()->GetXmax()/nch_mean->GetMean()));
  kno->Sumw2();
  for( int k = 1 ; k <= nch->GetNbinsX() ; ++k){
    kno->SetBinContent(k , nch_mean->GetMean() * nch->GetBinContent(k));
    kno->SetBinError(k , nch_mean->GetMean() * nch->GetBinError(k));
  }
}

void MultiPlots::write(bool scale){

  gDirectory->mkdir("MultiPlots_"+plotsname);
  gDirectory->cd("MultiPlots_"+plotsname);
  
  if(scale) nch->Scale( 1. / nbEvts , "width" );
  nch->Write();
  
  makeKNO();
  kno->Write();
  
  if(scale) rapidity->Scale(1./nbEvts , "width");
  rapidity->Write();
  
  if(scale) eta->Scale(1./nbEvts , "width");
  eta->Write();
  
  if(scale) pt->Scale(1./nbEvts , "width");
  pt->Write();
  
  if(scale) pt2->Scale(1./nbEvts , "width");
  pt2->Write();

  if(scale) mt->Scale(1./nbEvts , "width");
  mt->Write();

  if(scale) ptmVSnch->Scale(1./nbEvts , "width");
  ptmVSnch->Write();
  
  if(scale) ptVSnch->Scale(1./nbEvts , "width");
  ptVSnch->Write();
  
  if(scale) ptmVSnch_2D->Scale(1./nbEvts , "width");
  ptmVSnch_2D->Write();
  
  if(scale) ptVSnch_2D->Scale(1./nbEvts , "width");
  ptVSnch_2D->Write();
  
  this->Write("multi_class_"+plotsname);
  
  gDirectory->cd("../");
  
  //this->writeSummary();
}

TH1* MultiPlots::get(TString plot){
  if(plot.Contains("nch"))              return nch;
  else if(plot.Contains("kno") )        return kno;
  else if(plot.Contains("rapidity"))    return rapidity;
  else if(plot.Contains("eta"))         return eta;
  else if(plot.Contains("pt"))          return pt;
  else if(plot.Contains("pt2"))         return pt2;
  else if(plot.Contains("mt"))          return mt;
  else if(plot.Contains("ptmVSnch"))    return ptmVSnch;
  else if(plot.Contains("ptVSnch"))     return ptVSnch;
  else if(plot.Contains("ptmVSnch_2D")) return ptmVSnch_2D;
  else if(plot.Contains("ptVSnch_2D"))  return ptVSnch_2D;
  return NULL;  
}

void MultiPlots::writeSummary(){
  cout<<"Data @ "<<energy<<" GeV  ( "<<plotsname<<" ) :"<<endl;
  cout<<" - #evts              : "<<nbEvts<<endl;     
  cout<<" - Multiplicity Mean  : "<<nch_mean->GetMean()<<endl;
  cout<<" - KNO Mean           : "<<kno->GetMean()<<endl;
  for(int i=0;i<nb_moments;i++)
    cout<<" - Moment "<<i<<"           : "<<this->getMoments().at(i)<<endl;
  cout<<" - eta bin 0          : "<<eta->GetBinContent(eta->FindBin(0.))<<endl;
}

vector<double> MultiPlots::getMoments(){
  vector<double> mom;
  for(int i=0;unsigned(i)<moments->size();i++){
    if(nch_mean->GetMean() != 0)
      mom.push_back(moments->at(i).GetMean()/pow(nch_mean->GetMean(),i+1));
    else mom.push_back(0);
  }
  return mom;
}

//Setters
void MultiPlots::setNbOfMoments(int NbOfMoments){
  if(nbEvts>0){
    cout<<"MultiPlots::setNbOfMoments :"<<endl
        << "This setting can only be changed before filling events !"<<endl;
    return;
  }

  delete moments;
  
  nb_moments = NbOfMoments;
  moments = new vector<TMean>(nb_moments,TMean());
}


//_____________________________________________________________________________
void MultiPlots::divide(MultiPlots& mppart, const TString name){
  
  gDirectory->mkdir("Eff_"+name);
  gDirectory->cd("Eff_"+name);
  
  //TH1F
  this->makeEff( nch         , mppart.nch         , "eff_nch_"     + name);
  //this->makeEff( kno         , mppart.kno         , "eff_kno_"     + name);
  this->makeEff( rapidity    , mppart.rapidity    , "eff_rapidity_"+ name);
  this->makeEff( eta         , mppart.eta         , "eff_eta_"     + name);
  this->makeEff( pt          , mppart.pt          , "eff_pt_"      + name);
  this->makeEff( pt2         , mppart.pt2         , "eff_pt2_"     + name);
  this->makeEff( mt          , mppart.mt          , "eff_mt_"      + name);
  
  //TH2F
  this->makeEff( ptmVSnch_2D    , mppart.ptmVSnch_2D   , "eff_ptmVSnch_2D_"   + name);
  this->makeEff( ptVSnch_2D     , mppart.ptVSnch_2D    , "eff_ptVSnch_2D_"    + name);
  
  //TProfile
  this->makeEff( ptmVSnch_2D    , mppart.ptmVSnch_2D    , "eff_ptmVSnch_2D"   + name);
  this->makeEff( ptVSnch_2D     , mppart.ptVSnch_2D     , "eff_ptVSnch_2D_"   + name);
  
  
  gDirectory->cd("../");
}

 
//_____________________________________________________________________________
template <class T> void MultiPlots::makeEff( T* num, T* denom, const TString name){
  T* eff    = (T*) num->Clone(name);
  eff->Divide(num , denom , 1 , 1 , "B");
  eff->SetMinimum(0);
  eff->Write();
}
