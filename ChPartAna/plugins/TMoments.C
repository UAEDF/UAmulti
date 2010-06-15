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
  
  kisFilled = false;
  
  mean = new TMean();
  
  cmoments_tmp = new vector<TMean>(nmoments,TMean());
  fmoments_tmp = new vector<TMean>(nmoments,TMean());
  
  cmoments = new vector<Double_t>(nmoments,0);
  fmoments = new vector<Double_t>(nmoments,0);
  kmoments = new vector<Double_t>(nmoments,0);
  hmoments = new vector<Double_t>(nmoments,0);
  
  cstaterr = new vector<Double_t>(nmoments,0);
  fstaterr = new vector<Double_t>(nmoments,0);
  kstaterr = new vector<Double_t>(nmoments,0);
  hstaterr = new vector<Double_t>(nmoments,0);
  
  csystmerr = new vector<Double_t>(nmoments,0);
  fsystmerr = new vector<Double_t>(nmoments,0);
  ksystmerr = new vector<Double_t>(nmoments,0);
  hsystmerr = new vector<Double_t>(nmoments,0);
  
  csystperr = new vector<Double_t>(nmoments,0);
  fsystperr = new vector<Double_t>(nmoments,0);
  ksystperr = new vector<Double_t>(nmoments,0);
  hsystperr = new vector<Double_t>(nmoments,0);
    
  mean_pull = new TMean();
  RMS_pull  = new TMean();
  cstaterr_pull = new vector<TMean>(nmoments,TMean());
  fstaterr_pull = new vector<TMean>(nmoments,TMean());
  kstaterr_pull = new vector<TMean>(nmoments,TMean());
  hstaterr_pull = new vector<TMean>(nmoments,TMean());
    
}

void TMoments::clear(){

  kisFilled = false;

  delete mean;

  delete cmoments_tmp;
  delete fmoments_tmp;

  delete cmoments;
  delete fmoments;
  delete kmoments;
  delete hmoments;

  delete cstaterr;
  delete fstaterr;
  delete kstaterr;
  delete hstaterr;

  delete csystmerr;
  delete fsystmerr;
  delete ksystmerr;
  delete hsystmerr;

  delete csystperr;
  delete fsystperr;
  delete ksystperr;
  delete hsystperr;
  
  
  delete mean_pull;
  delete RMS_pull;
  delete cstaterr_pull;
  delete fstaterr_pull;
  delete kstaterr_pull;
  delete hstaterr_pull;
    
  this->Init();

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
  kisFilled = true;
  for(int i = 1 ; i <= h->GetNbinsX() ; ++i)
    this->Add(h->GetBinCenter(i) , h->GetBinContent(i));
}


/*void TMoments::Add(TH1F* h){
  kisFilled = true;
  for(int i = 1 ; i < h->GetXaxis()->GetXmax() ; ++i){
    int ibin = h->GetXaxis()->FindFixBin(i);
    this->Add(i , h->GetBinContent(ibin));
  }
}*/

void TMoments::Add(TGraph* g){
  kisFilled = true;
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
          kmoments->at(m) -= TMath::Binomial(m-1 , i) * kmoments->at(m-i) * fmoments->at(i);
      
      if(kmoments->at(m) != 0)
        hmoments->at(m) = kmoments->at(m) / fmoments->at(m);
    }
  }
}

void TMoments::print(){

  cout<<" --------- MEAN ---------"<<endl;
  cout<<" Mean = "<<mean->GetMean()<<" +- "<<mean->GetMeanError()<<" + "<<mean->_MeanSystP<<" - "<<mean->_MeanSystM<<endl;
  cout<<" D2 = "<<mean->GetRMS()<<" +- "<<mean->GetRMSError()<<" + "<<mean->_RMSSystP<<" - "<<mean->_RMSSystM<<endl;

  cout<<" --------- C-MOMENTS ---------"<<endl;
  for(int m = 0 ; m < nmoments ; ++m)
    cout<<" C"<<m<<" = "<<cmoments->at(m)<<" +- "<<cstaterr->at(m)<<" + "<<csystperr->at(m)<<" - "<<csystmerr->at(m)<<endl;

  cout<<" --------- F-MOMENTS ---------"<<endl;
  for(int m = 0 ; m < nmoments ; ++m)
    cout<<" F"<<m<<" = "<<fmoments->at(m)<<" +- "<<fstaterr->at(m)<<" + "<<fsystperr->at(m)<<" - "<<fsystmerr->at(m)<<endl;

  cout<<" --------- K-MOMENTS ---------"<<endl;
  for(int m = 0 ; m < nmoments ; ++m)
    cout<<" K"<<m<<" = "<<kmoments->at(m)<<" +- "<<kstaterr->at(m)<<" + "<<ksystperr->at(m)<<" - "<<ksystmerr->at(m)<<endl;

  cout<<" --------- H-MOMENTS ---------"<<endl;
  for(int m = 0 ; m < nmoments ; ++m)
    cout<<" H"<<m<<" = "<<hmoments->at(m)<<" +- "<<hstaterr->at(m)<<" + "<<hsystperr->at(m)<<" - "<<hsystmerr->at(m)<<endl;
}

void TMoments::ComputeErrorsFromResampling(TH1F* h){
  //TMoments* mom_sampled = new TMoments(h);
  //mom_sampled->print();
  TH1F* sample = (TH1F*) h->Clone("resampled");
    
  int nsample = 1000;
  
  for(int i = 0 ; i < nsample ; ++i){
    sample->Reset();
    for(int ibin = 1 ; ibin <= h->GetNbinsX() ; ++ibin)
      sample->SetBinContent(ibin , gRandom->Gaus(h->GetBinContent(ibin) , h->GetBinError(ibin)));
    
     this->AddDataForResampling(sample);
  }
  
  this->FinishCurrentResampling();
}

void TMoments::AddDataForResampling(TH1F* h){
  TMoments* mom_sampled = new TMoments(h);
  mom_sampled->ComputeMoments();
  
  mean_pull->Add(mom_sampled->mean->GetMean());
  RMS_pull->Add(mom_sampled->mean->GetRMS());
  for(int m = 0 ; m < nmoments ; ++m){
    cstaterr_pull->at(m).Add(mom_sampled->cmoments->at(m));
    fstaterr_pull->at(m).Add(mom_sampled->fmoments->at(m));
    kstaterr_pull->at(m).Add(mom_sampled->kmoments->at(m));
    hstaterr_pull->at(m).Add(mom_sampled->hmoments->at(m));
  }
}

void TMoments::FinishCurrentResampling(){
  mean->SetMeanError(sqrt( pow(mean_pull->GetRMS(),2) + pow(mean->GetMeanError(),2) ));
  mean->SetRMSError( sqrt( pow(RMS_pull->GetRMS(),2)  + pow(mean->GetRMSError(),2)  ));
  for(int m = 0 ; m < nmoments ; ++m){
    cstaterr->at(m) = sqrt( pow(cstaterr_pull->at(m).GetRMS(),2) + pow(cstaterr->at(m),2) );
    fstaterr->at(m) = sqrt( pow(fstaterr_pull->at(m).GetRMS(),2) + pow(fstaterr->at(m),2) );
    kstaterr->at(m) = sqrt( pow(kstaterr_pull->at(m).GetRMS(),2) + pow(kstaterr->at(m),2) );
    hstaterr->at(m) = sqrt( pow(hstaterr_pull->at(m).GetRMS(),2) + pow(hstaterr->at(m),2) );
  }
  
  mean_pull->Reset();
  RMS_pull->Reset();
  for(int m = 0 ; m < nmoments ; ++m){
    cstaterr_pull->at(m).Reset();
    fstaterr_pull->at(m).Reset();
    kstaterr_pull->at(m).Reset();
    hstaterr_pull->at(m).Reset();
  }
}



