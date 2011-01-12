
#include "BasePlots.h"

ClassImp(BasePlots)

  Int_t     BasePlots::nch_nbin  = 201;
  Double_t  BasePlots::nch_xmin  = -0.5;
  Double_t  BasePlots::nch_xmax  = BasePlots::nch_xmin + BasePlots::nch_nbin;
  Double_t* BasePlots::nch_array;
  
  Int_t     BasePlots::pt_nbin  = 100;
  Double_t  BasePlots::pt_xmin  = 0.;
  Double_t  BasePlots::pt_xmax  = 3.;
  Double_t* BasePlots::pt_array;

  Int_t     BasePlots::pt2_nbin  = 100;
  Double_t  BasePlots::pt2_xmin  = 0.;
  Double_t  BasePlots::pt2_xmax  = 3.;
  Double_t* BasePlots::pt2_array;

  Int_t     BasePlots::mt_nbin  = 100;
  Double_t  BasePlots::mt_xmin  = 0.;
  Double_t  BasePlots::mt_xmax  = 3.;
  Double_t* BasePlots::mt_array;

  Int_t     BasePlots::mpt_nbin  = 160;
  Double_t  BasePlots::mpt_xmin  = 0.;
  Double_t  BasePlots::mpt_xmax  = 2;
  Double_t* BasePlots::mpt_array;
  
  Int_t     BasePlots::eta_nbin  = 60;
  Double_t  BasePlots::eta_xmin  = -3.;
  Double_t  BasePlots::eta_xmax  = 3.;
  Double_t* BasePlots::eta_array;

BasePlots::BasePlots(){  
  this->makeBins(nch_nbin,nch_xmin,nch_xmax,&nch_array);
  this->makeBins(pt_nbin,pt_xmin,pt_xmax,&pt_array);
  this->makeBins(pt2_nbin,pt2_xmin,pt2_xmax,&pt2_array);
  this->makeBins(mt_nbin,mt_xmin,mt_xmax,&mt_array);
  this->makeBins(mpt_nbin,mpt_xmin,mpt_xmax,&mpt_array);
  this->makeBins(eta_nbin,eta_xmin,eta_xmax,&eta_array);
  plotsname = "none";
}

BasePlots::BasePlots(TString collname){
  this->makeBins(nch_nbin,nch_xmin,nch_xmax,&nch_array);
  this->makeBins(pt_nbin,pt_xmin,pt_xmax,&pt_array);
  this->makeBins(pt2_nbin,pt2_xmin,pt2_xmax,&pt2_array);
  this->makeBins(mt_nbin,mt_xmin,mt_xmax,&mt_array);
  this->makeBins(mpt_nbin,mpt_xmin,mpt_xmax,&mpt_array);
  this->makeBins(eta_nbin,eta_xmin,eta_xmax,&eta_array);
  plotsname = collname;
}

BasePlots::~BasePlots(){ }

void BasePlots::makeBins(const Int_t nbin , Double_t xmin , Double_t xmax , Double_t** array){
  *array = new Double_t[nbin+1];
  Double_t step = fabs( xmax - xmin ) / double (nbin);
  for(int i=0;i<=nbin;++i)
    (*array)[i] = xmin + i * step;
}

void BasePlots::setBinning(vector< vector<double> >& array){
  nch_nbin = array.at(0).size()-1;
  nch_array = new Double_t[array.at(0).size()];
  for(int i = 0 ; (unsigned) i < array.at(0).size() ; ++i)
    nch_array[i] = array.at(0).at(i);

  pt_nbin = array.at(1).size()-1;
  pt_array = new Double_t[array.at(1).size()];
  for(int i = 0 ; (unsigned) i < array.at(1).size() ; ++i)
    pt_array[i] = array.at(1).at(i);

  eta_nbin = array.at(2).size()-1;
  eta_array = new Double_t[array.at(2).size()];
  for(int i = 0 ; (unsigned) i < array.at(2).size() ; ++i)
    eta_array[i] = array.at(2).at(i);

  pt2_nbin = array.at(3).size()-1;
  pt2_array = new Double_t[array.at(3).size()];
  for(int i = 0 ; (unsigned) i < array.at(3).size() ; ++i)
    pt2_array[i] = array.at(3).at(i);

  mt_nbin = array.at(4).size()-1;
  mt_array = new Double_t[array.at(4).size()];
  for(int i = 0 ; (unsigned) i < array.at(4).size() ; ++i)
    mt_array[i] = array.at(4).at(i);

}


