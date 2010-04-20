#include "TMoments.h"

ClassImp(TMoments)

using namespace std;

TMoments::TMoments(){
  nmoments = 6;
  this->Init();
}

TMoments::TMoments(Int_t n):nmoments(n){
  this->Init();
}

TMoments::TMoments(TH1F* h){
  nmoments = 6;
  this->Init();
  this->Add(h);
}

void TMoments::Init(){
  
  mean = new TMean();
  
  cmoments_tmp = new vector<TMean>(nmoments,TMean());
  fmoments_tmp = new vector<TMean>(nmoments,TMean());
  
  cmoments = new vector<Double_t>(nmoments,0);
  fmoments = new vector<Double_t>(nmoments,0);
  kmoments = new vector<Double_t>(nmoments,0);
  hmoments = new vector<Double_t>(nmoments,0);
  
  cerrors = new vector<Double_t>(nmoments,0);
  ferrors = new vector<Double_t>(nmoments,0);
  kerrors = new vector<Double_t>(nmoments,0);
  herrors = new vector<Double_t>(nmoments,0);
    
}


TMoments::~TMoments(){
}


void TMoments::Add(Double_t value, Double_t weight){
  mean->Add(value,weight);
  
  for(int m = 0 ; m < nmoments ; ++m){
    cmoments_tmp->at(m).Add(pow(value,m),weight);
    //cout<<cmoments_tmp->at(m).GetN()<<"  "<<cmoments_tmp->at(m).GetSum()<<endl;
    //cout<<m<<endl;
    
    Double_t fmom = 1;
    for(int q = 0 ; q < m ; q++)
      fmom *= (value - q);
    fmoments_tmp->at(m).Add(fmom,weight);  
  }
}


void TMoments::Add(TH1F* h){
  for(int i = 1 ; i <= h->GetNbinsX() ; ++i)
    this->Add(h->GetBinCenter(i) , h->GetBinContent(i));
}

/*
void TMoments::Add(TH1F* h){
  for(int i = 0 ; i < h->GetXaxis()->GetXmax() ; ++i){
    int ibin = h->GetXaxis()->FindFixBin(i);
    this->Add(i , h->GetBinContent(ibin));
  }
}*/

void TMoments::Add(TGraph* g){
  Double_t* x = g->GetX();
  Double_t* y = g->GetY();
  for(int i = 1 ; i <= g->GetN() ; ++i)
    this->Add(x[i] , y[i]);
}

void TMoments::ComputeMoments(){
  for(int m = 0 ; m < nmoments ; ++m){
    if(mean->GetMean() != 0){
      cmoments->at(m) = cmoments_tmp->at(m).GetMean() / pow(mean->GetMean() , m) ;
      //cout<<cmoments_tmp->at(m).GetN()<<"  "<<cmoments_tmp->at(m).GetSum()<<"  "<<cmoments_tmp->at(m).GetMean()<<endl;
      
      
      fmoments->at(m) = fmoments_tmp->at(m).GetMean() / pow(mean->GetMean() , m) ;
      
      kmoments->at(m) = fmoments->at(m);
      if(m>=1)
        for(int i = 1 ; i < m ; ++i)
          kmoments->at(m) -= TMath::Binomial(m-1 , i) * kmoments->at(m-1) * fmoments->at(i);
      
      if(kmoments->at(m) != 0)
        hmoments->at(m) = fmoments->at(m) / kmoments->at(m);
    }
  }
}

void TMoments::Print(){

  cout<<" --------- MEAN ---------"<<endl;
  cout<<" Mean = "<<mean->GetMean()<<" +- "<<mean->GetMeanError()<<endl;

  cout<<" --------- C-MOMENTS ---------"<<endl;
  for(int m = 0 ; m < nmoments ; ++m)
    cout<<" C"<<m<<" = "<<cmoments->at(m)<<" +- "<<cerrors->at(m)<<endl;

  cout<<" --------- F-MOMENTS ---------"<<endl;
  for(int m = 0 ; m < nmoments ; ++m)
    cout<<" F"<<m<<" = "<<fmoments->at(m)<<" +- "<<ferrors->at(m)<<endl;

  cout<<" --------- K-MOMENTS ---------"<<endl;
  for(int m = 0 ; m < nmoments ; ++m)
    cout<<" K"<<m<<" = "<<kmoments->at(m)<<" +- "<<kerrors->at(m)<<endl;

  cout<<" --------- H-MOMENTS ---------"<<endl;
  for(int m = 0 ; m < nmoments ; ++m)
    cout<<" H"<<m<<" = "<<hmoments->at(m)<<" +- "<<herrors->at(m)<<endl;
}




void TMoments::ComputeErrorsFromResampling(TH1F* h){
  TMoments* mom_sampled = new TMoments(h);
  TH1F* sample = (TH1F*) h->Clone("resampled");
  
  TMean* mean_pull = new TMean();
  vector<TMean> cerrors_pull(nmoments , TMean());
  vector<TMean> ferrors_pull(nmoments , TMean());
  vector<TMean> kerrors_pull(nmoments , TMean());
  vector<TMean> herrors_pull(nmoments , TMean());
  
  int nsample = 1000;
  
  for(int i = 0 ; i < nsample ; ++i){
    sample->Reset();
    for(int ibin = 1 ; ibin <= h->GetNbinsX() ; ++ibin)
      sample->SetBinContent(ibin , gRandom->Gaus(h->GetBinContent(ibin) , h->GetBinError(ibin)));
    
    mom_sampled->Init();
    mom_sampled->Add(sample);
    mom_sampled->ComputeMoments();
    //mom_sampled->Print();
    
    mean_pull->Add(mom_sampled->mean->GetMean());
    for(int m = 0 ; m < nmoments ; ++m){
      cerrors_pull.at(m).Add(mom_sampled->cmoments->at(m));
      ferrors_pull.at(m).Add(mom_sampled->fmoments->at(m));
      kerrors_pull.at(m).Add(mom_sampled->kmoments->at(m));
      herrors_pull.at(m).Add(mom_sampled->hmoments->at(m));
    }
  }
  
  mean->SetMeanError(mean_pull->GetRMS());
  for(int m = 0 ; m < nmoments ; ++m){
    cerrors->at(m) = cerrors_pull.at(m).GetRMS();
    ferrors->at(m) = ferrors_pull.at(m).GetRMS();
    kerrors->at(m) = kerrors_pull.at(m).GetRMS();
    herrors->at(m) = herrors_pull.at(m).GetRMS();
  }
}

