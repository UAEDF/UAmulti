//#include "TMean.cc"
//#include <vector>

#include "MultiPlots.h"
using namespace std;

ClassImp(MultiPlots)

//Default Constructor
MultiPlots::MultiPlots(){
  multiname = "none";
  this->init();
}

MultiPlots::MultiPlots(TString name){
  multiname = name;
  this->init();
}

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
}


void MultiPlots::init(){
  nb_moments = 5;
  moments = new vector<TMean>(nb_moments,TMean());
  nch_mean = new TMean();
  
  nch = new TH1F("nch_"+multiname,"nch_"+multiname+"N_{ch};fraction of events",46,-0.5,45.5);
  //kno = new TH1F("kno_"+multiname,"kno_"+multiname+"n / < n >;< n >   P_{n}",nch->GetNbinsX(),0.,double(nch->GetXaxis()->GetXmax()/nch_mean->GetMean()));
  kno = new TH1F(); //TO BE SAFE
  rapidity = new TH1F("rapidity_"+multiname,"rapidity_"+multiname+";y;#frac{1}{#sigma_{tot}} #frac{d#sigma_{ch}}{y}",60,-3.,3.);
  eta = new TH1F("eta_"+multiname,"eta_"+multiname+";#eta;#frac{1}{#sigma_{tot}} #frac{d#sigma_{ch}}{#eta}",60,-3.,3.);
  //eta = new TH1F("eta_"+multiname,"eta_"+multiname+";#eta;#frac{1}{#sigma_{tot}} #frac{d#sigma_{ch}}{#eta}",80,-10.,10.);
  pt  = new TH1F("pt_"+multiname,"pt_"+multiname+";pT [GeV];#frac{1}{#sigma_{tot}} #frac{d#sigma_{ch}}{pT}",100,0.,3.);
  pt2 = new TH1F("pt2_"+multiname,"pt2_"+multiname+";pT^{2} [GeV^{2}];#frac{1}{#sigma_{tot}} #frac{d#sigma_{ch}}{pT^{2}}",100,0.,9.);
  
  nch_width = nch->GetXaxis()->GetBinWidth(1);
  rapidity_width = rapidity->GetXaxis()->GetBinWidth(1);
  eta_width = eta->GetXaxis()->GetBinWidth(1);
  pt_width  = pt->GetXaxis()->GetBinWidth(1);
  pt2_width = pt2->GetXaxis()->GetBinWidth(1);
  
  nch->Sumw2();
  rapidity->Sumw2();
  eta->Sumw2();
  pt->Sumw2();
  pt2->Sumw2();
  
  energy    = 900;
  nbEvts    = 0;
  nch_inEvt = 0;
}

void MultiPlots::fill(MyPart& p , double weight){
  ++nch_inEvt;
  rapidity->Fill( p.v.Rapidity() , weight / rapidity_width );
  eta->Fill( p.v.Eta() , weight / eta_width );
  pt->Fill( p.v.Pt() , weight / pt_width );
  pt2->Fill( pow(p.v.Pt(),2) , weight / pt2_width );
}

void MultiPlots::nextEvent(double weight){
  ++nbEvts;
  nch->Fill( nch_inEvt , weight / nch_width );
  nch_mean->Add( nch_inEvt , weight );
  for(int i = 0 ; unsigned(i) < moments->size() ; ++i)
    moments->at(i).Add( pow(double(nch_inEvt),i+1) , weight );
    
  //re-initializing nch
  nch_inEvt = 0; 
}

void MultiPlots::makeKNO(){
  kno = new TH1F("kno_"+multiname,"kno_"+multiname+";n / < n >;< n >   P_{n}",nch->GetNbinsX(),0.,double(nch->GetXaxis()->GetXmax()/nch_mean->GetMean()));
  for( int k = 1 ; k <= nch->GetNbinsX() ; ++k)
    kno->SetBinContent(k , nch_mean->GetMean() * nch->GetBinContent(k));
}

void MultiPlots::write(){

  gDirectory->mkdir("MultiPlots_"+multiname);
  gDirectory->cd("MultiPlots_"+multiname);
  
  nch->Scale( 1. / nbEvts );
  nch->Write();
  
  makeKNO();
  kno->Write();
  
  rapidity->Scale(1./nbEvts);
  rapidity->Write();
  
  eta->Scale(1./nbEvts);
  eta->Write();
  
  pt->Scale(1./nbEvts);
  pt->Write();
  
  pt2->Scale(1./nbEvts);
  pt2->Write();

  this->Write("multi_class"+multiname);
  
  gDirectory->cd("../");
  
  this->writeSummary();
}

void MultiPlots::writeSummary(){
  cout<<"Data @ "<<energy<<" GeV  ( "<<multiname<<" ) :"<<endl;
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