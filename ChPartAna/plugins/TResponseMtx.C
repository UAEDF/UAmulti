#include "TResponseMtx.h"

ClassImp(TResponseMtx)


void TResponseMtx::Init(){
  if(binning.size() == 0) this->makeBins(101,-0.5,100.5);
  mtx = TH2F( "mtx_"+name , "mtx_"+name+";gen;reco" , binning.size()-1 , &(binning.at(0)) , binning.size()-1 , &(binning.at(0)) );
  nEvts = 0;
  isNormalised = isNormalisedByColumn = isNormalisedByLine = false;
  gen_inEvt = reco_inEvt;
}

void TResponseMtx::FillGen(Double_t val ){
  gen_inEvt = val;
}

void TResponseMtx::FillReco(Double_t val){
  reco_inEvt = val;
}

void TResponseMtx::NextEvent(Double_t weight){
  this->Fill(gen_inEvt , reco_inEvt , weight);
  gen_inEvt = reco_inEvt = 0;
}

void TResponseMtx::Fill(Double_t xval , Double_t yval , Double_t weight){
  mtx.Fill(xval , yval , weight);
  gen_inEvt = reco_inEvt = 0;
  nEvts+=weight;
}

void TResponseMtx::NormalizeByLine(){
  if(isNormalisedByLine) return;
  isNormalisedByLine = true;
  if(isNormalised)         cout << "[TResponseMtx] WARNING !! Already normalized ..." << endl;
  if(isNormalisedByColumn) cout << "[TResponseMtx] WARNING !! Already normalized by column ..." << endl;
  
  for(int ny = 1 ; ny <= mtx.GetNbinsY() ; ++ny){
    Double_t sum     = 0;
    Bool_t isNotVoid = false;
    for(int nx = 1 ; nx <= mtx.GetNbinsX() ; ++nx){
      sum += mtx.GetBinContent(nx , ny);
      if( mtx.GetBinContent(nx , ny) != 0. )
        isNotVoid = true;
    }
   
    if(sum != 0){
      for(int nx = 1 ; nx <= mtx.GetNbinsX() ; ++nx){
        mtx.SetBinContent( nx , ny , mtx.GetBinContent(nx , ny) / sum );
        mtx.SetBinError( nx , ny , mtx.GetBinError(nx , ny) / sum );
      }
    }
    else if(sum == 0 && isNotVoid){
      cout << "[TResponseMtx] ERROR !! Sum of line = 0 but line is not void. " << endl << "Exiting NOW ..." << endl;
      exit(1);
    }
  }
}


void TResponseMtx::NormalizeByColumn(){
  if(isNormalisedByColumn) return;
  isNormalisedByColumn = true;
  if(isNormalisedByLine) cout << "[TResponseMtx] WARNING !! Already normalized by line ..." << endl;
  if(isNormalised)       cout << "[TResponseMtx] WARNING !! Already normalized ..." << endl;
  
  for(int nx = 1 ; nx <= mtx.GetNbinsX() ; ++nx){
    Double_t sum     = 0;
    Bool_t isNotVoid = false;
    for(int ny = 1 ; ny <= mtx.GetNbinsY() ; ++ny){
      sum += mtx.GetBinContent(nx , ny);
      if( mtx.GetBinContent(nx , ny) != 0. )
        isNotVoid = true;
    }
   
    if(sum != 0){
      for(int ny = 1 ; ny <= mtx.GetNbinsY() ; ++ny){
        mtx.SetBinContent( nx , ny , mtx.GetBinContent(nx , ny) / sum );
        mtx.SetBinError( nx , ny , mtx.GetBinError(nx , ny) / sum );
      }
    }
    else if(sum == 0 && isNotVoid){
      cout << "[TResponseMtx] ERROR !! Sum of column = 0 but column is not void. " << endl << "Exiting NOW ..." << endl;
      exit(1);
    }
  }
}


void TResponseMtx::Normalize(Bool_t countOverFlows){
  if(isNormalised) return;
  isNormalised = true;
  if(isNormalisedByLine)   cout << "[TResponseMtx] WARNING !! Already normalized by line ..." << endl;
  if(isNormalisedByColumn) cout << "[TResponseMtx] WARNING !! Already normalized by column ..." << endl;
  Double_t nev = mtx.Integral();
  if(countOverFlows)
    nev = nEvts;
  if(nev !=0 )
    mtx.Scale(1./nev);
}


void TResponseMtx::write(Bool_t writeClass){
  gDirectory->mkdir("TResponseMtx_"+name);
  gDirectory->cd("TResponseMtx_"+name);
  
  
  //nch_gen->Scale(1./nbEvts,"width");
  mtx.Write();
  
  if(writeClass) this->Write("tresponsemtx_class_"+name);
  
  gDirectory->cd("../");
}



void TResponseMtx::makeBins(Int_t nbin , Double_t xmin , Double_t xmax){
  binning.assign(nbin+1,0);
  Double_t step = fabs( xmax - xmin ) / double (nbin);
  for(int i=0 ; i<=nbin ; ++i)
    binning[i] = xmin + i * step;
}
