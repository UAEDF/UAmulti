
//=============================================
double getChi2(const TH1F* hist1 , const TH1F* hist2 , int errortype){
  double bin = 0 , error = 0 , sumbin = 0 , ndof = 0;
  
  //skip the first 2 bins + the underflow bin
  for(int i=5 ; i <= hist1->GetNbinsX() ; ++i){
    if(hist2->GetBinContent(i)!=0){
      ++ndof;
      bin = pow( hist1->GetBinContent(i) - hist2->GetBinContent(i) , 2 );
      if(errortype==0) error = fabs(hist1->GetBinContent(i));
      if(errortype==1) error = pow(hist1->GetBinError(i),2);
      if(error!=0) sumbin += bin / error ;
      else sumbin+=bin;
    }
  }
  return (ndof!=0) ? sumbin / ndof : -1;
}

//=============================================
void corr_vtx_cenSel(TH1F* toCorrect){
   toCorrect->Multiply(toCorrect, eff_afterUnf , 1 , 1);
}


//=============================================
void corr_ptCorr(TH1F* toCorrect){
   if(ptcorr_val) increaseNTracks( toCorrect , -1 , ptcorr_val , 0); 
}

//=============================================
void corr_unf(TH1F* toCorrect){

  TH1F* h = (TH1F*) toCorrect->Clone("tmp");

  //Backward unfolding
  for(int j = 0 ; j < toCorrect->GetNbinsX() ; ++j){
    double auxFill = 0.; 
    for(int i = 0 ; i < toCorrect->GetNbinsX() ; ++i)
      auxFill += h->GetBinContent(i+1) * matrix_normalized[j][i];
      	 
    toCorrect->SetBinContent(j+1,auxFill);
  }
  
  delete h;
}   

//=============================================
void corr_trigSel(TH1F* toCorrect){
  toCorrect->Multiply(toCorrect , eff_beforeUnf , 1 , 1);
}

void corr_full(TH1F* toCorrect){
  corr_vtx_cenSel( toCorrect );
  corr_ptCorr    ( toCorrect );
  corr_unf       ( toCorrect );
  corr_trigSel   ( toCorrect );
}

//=============================================
TH1F* fit2hist( TH1F* toCopy ){
  TH1F* fit = (TH1F*) toCopy->Clone("fit");
  fit->Reset();
     
  // set the curve coming from the generated values
  for ( int i = 1 ; i <= fit->GetNbinsX() ; ++i ){
    it->SetBinContent( i , fit_function( fit->GetBinCenter(i) , par) );
  } 
  
  return fit;
}



//=============================================
void calc_chi_square(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t iflag){

  //discretizing func in hist
  TH1F* fit = fit2hist( data_curve );
  
  //applying full backward correction
  corr_full( fit );
  
  //computing chi2
  f = getChi2( data_curve , fit , 1);

}


//=============================================
Double_t uncorrected_doubleNBD(Double_t* x , Double_t* par){

  //cout << par[0] << "  " << par[1] << "  " << par[2] << "  " << par[3] << "  " << par[4] << endl;
  debug_ = true; 
  
  TH1F* fit = fit2hist( data_curve );
  
  corr_ptCorr( fit );
  
  if(debug_){
    data_corrected->Draw();
    data_curve->Draw("same");
    TH1F* fit_draw1 = (TH1F*) fit->Clone("fit_draw1");
    fit_draw1->SetLineColor( kRed );
    fit_draw1->Draw("same");
    //gPad->WaitPrimitive();
  }
  
  corr_vtx_cenSel( fit );
    
  if(debug_){
    TH1F* fit_draw2 = (TH1F*) fit->Clone("fit_draw2");
    fit_draw2->SetLineColor( kBlue );
    fit_draw2->Draw("same");
    //gPad->WaitPrimitive();
  }
  
  corr_unf( fit );

  if(debug_){
    TH1F* fit_draw3 = (TH1F*) fit_reco->Clone("fit_draw3");
    fit_draw3->SetLineColor( kGreen );
    fit_draw3->Draw("same");
    //gPad->WaitPrimitive();
  }
  
  corr_trigSel( fit );
    
  if(debug_){
    TH1F* fit_draw4 = (TH1F*) fit_reco->Clone("fit_draw4");
    fit_draw4->SetLineColor( kMagenta );
    fit_draw4->Draw("same");
    gPad->WaitPrimitive();
  }
  
 
  if(debug_) {
     cout << "Chi2: " << getChi2( data_curve, fit_reco, 1) << endl;
  }
  
  Double_t val = fit->GetBinContent( fit->GetXaxis()->FindFixBin( x[0] ) );
  
  debug_ = false; 
  
  delete fit;
  
  return val;
}










