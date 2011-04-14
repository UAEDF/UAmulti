#include "FBCorrel.h"

Bool_t FBCorrel::debug            = 0;

FBCorrel::FBCorrel(){
  if(debug) cout << "+ [DEBUG] FBCorrel : Starting FBCorrel() {" << ttp.name << "}" << endl;
  ttp   = TTProfile();
  b_fit = b = 0;
  fit   = TF1();
  func  = TF1();
  mn1n2 = TMean();
  mn1   = TMean();
  mn2   = TMean();
  mC    = TMean();
  hC    = TH1F();
  if(debug) cout << "- [DEBUG] FBCorrel : Finished FBCorrel() {" << ttp.name << "}" << endl;
}

FBCorrel::FBCorrel(TString s){
  if(debug) cout << "+ [DEBUG] FBCorrel : Starting FBCorrel(TString) {" << ttp.name << "}" << endl;
  ttp = TTProfile(s);
  this->Init();
  if(debug) cout << "- [DEBUG] FBCorrel : Finished FBCorrel(TString) {" << ttp.name << "}" << endl;
}


void FBCorrel::Init(){
  if(debug) cout << "+ [DEBUG] FBCorrel : Starting Init() {" << ttp.name << "}" << endl;
  b     = 0;
  b_fit = 0;
  fit   = TF1("fit_"+ttp.name,"[0] + [1] * x" , ttp.binning.at(0) , ttp.binning.at(ttp.nBins) );
  func  = TF1("func_"+ttp.name,"[0] + [1] * x" , ttp.binning.at(0) , ttp.binning.at(ttp.nBins) );
  mn1n2 = TMean();
  mn1   = TMean();
  mn2   = TMean();
  mC    = TMean();
  hC    = TH1F("hC_"+ttp.name,"hC_"+ttp.name, 500 , -20. , 20.);
  if(debug) cout << "- [DEBUG] FBCorrel : Finished Init() {" << ttp.name << "}" << endl;
}

void FBCorrel::Fill(Double_t xval , Double_t yval , Double_t weight){
  ttp.Fill(xval , yval , weight);
  
  mn1n2.Add(xval*yval , weight);
  mn1.Add(xval , weight);
  mn2.Add(yval , weight);
  
  double C = 0;
  if(xval + yval != 0) C = ( xval - yval ) / sqrt( xval + yval );
  mC.Add(C , weight);
  hC.Fill(C , weight);
  
}

void FBCorrel::Fill(Double_t xval , Double_t yval , TResponseMtx& xtrm){
  xtrm.NormalizeByLine();
  Int_t xreco = xtrm.mtx.GetYaxis()->FindFixBin(xval);
  for(int i = 1 ; i <= xtrm.mtx.GetNbinsX() ; ++i)
    if(xtrm.mtx.GetBinContent(i,xreco)!=0)
      this->Fill( xtrm.mtx.GetXaxis()->GetBinCenter(i) , yval , xtrm.mtx.GetBinContent(i,xreco) );  
}


void FBCorrel::Fill(Double_t xval , Double_t yval , TResponseMtx& xtrm , TResponseMtx& ytrm){
  xtrm.NormalizeByLine();
  ytrm.NormalizeByLine();
  
  //Redistributing xval using xtrm
  Int_t xreco = xtrm.mtx.GetYaxis()->FindFixBin(xval);
  for(int i = 1 ; i <= xtrm.mtx.GetNbinsX() ; ++i){
    if(xtrm.mtx.GetBinContent(i,xreco)!=0){
      
      //Doing same on yval using ytrm
      Int_t yreco = ytrm.mtx.GetYaxis()->FindFixBin(yval);
      for(int j = 1 ; j <= ytrm.mtx.GetNbinsX() ; ++j)
        if(ytrm.mtx.GetBinContent(j,yreco)!=0)
          this->Fill( xtrm.mtx.GetXaxis()->GetBinCenter(i) , ytrm.mtx.GetXaxis()->GetBinCenter(j) , xtrm.mtx.GetBinContent(i,xreco) * ytrm.mtx.GetBinContent(j,yreco) );
    }
  }  
}

void FBCorrel::Fit(Bool_t autoRange , Int_t verbosity){
  if(! ttp.profileDone) ttp.makeProfile();
  
  Double_t min = ttp.binning.at(0) , max = ttp.binning.at(ttp.nBins) ;
  if(autoRange){
    for(int i=0 ; i<ttp.nBins-1 ; ++i){
      if(ttp.profile.GetBinContent(i+1)==0. && ttp.profile.GetBinContent(i+2)==0.){
        max = ttp.binning.at(i+1);
	break;
      }
    }
    fit.SetRange(min,max*0.75);
  }
  
  ttp.profile.Fit(&fit,"RQN");
  b_fit       = fit.GetParameter(1);
  if(verbosity)
    cout << ttp.name << " fitted with pol1 , b = " << b_fit << "  & chi2/ndof = " << fit.GetChisquare() / fit.GetNDF() << " in range ( " <<  min << " , " << max*(1-0.25*autoRange) << " ) " << endl;
    
  this->ComputeB();
  func.SetParameters(fit.GetParameter(0) , b);  
}

void FBCorrel::ComputeB(){
  b = ( mn1n2.GetMean() - mn1.GetMean() * mn2.GetMean() ) / ( mn1.GetMeanSquare() - mn1.GetMean() * mn1.GetMean() );
  //b = ( mn1n2.GetMean() - mn1.GetMean() * mn2.GetMean() ) / ( mn1.GetRMS() * mn1.GetRMS() );

}

void FBCorrel::write(){
  gDirectory->mkdir("FBCorrel_"+ttp.name);
  gDirectory->cd("FBCorrel_"+ttp.name);
  
  ttp.write(1,1);
  ttp.profile.Write();
  fit.Write();
  
  
  this->Write("fbcorrel_class_"+ttp.name);

  gDirectory->cd("../");
}
