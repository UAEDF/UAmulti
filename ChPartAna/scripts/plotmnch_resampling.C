
  cout << "Xavier :"<<endl;
  TVirtualFitter::Fitter(nchmean_all)->PrintResults(10,0.);
  int npara = 3;
  TMatrixD cov(npara,npara);
  TMatrixD cor(npara,npara);
  
  cout << endl << "Covariance Matrix ==> " << endl;
  for (int i=0 ; i<npara ; i++) {
    for (int j=0 ; j<npara ; j++) {
      cout << TVirtualFitter::Fitter(nchmean_all)->GetCovarianceMatrixElement(i,j) << "     " ;
      cov(i,j) = TVirtualFitter::Fitter(nchmean_all)->GetCovarianceMatrixElement(i,j);
    }
    cout << endl;
  }
  
  cout << endl << "Correlation Matrix ==> " << endl;
  for (int i=0 ; i<npara; i++){
    for (int j=0 ; j<npara ; j++){
      cor(i,j) = cov(i,j) / sqrt(cov(i,i) * cov(j,j));
      cout << cor(i,j) << "     " ;
    }
    cout << endl;
  }
  cout << endl;
  
  //TAKEN FROM MATLAB
  //USING THE FOLLOWING LINES:
  //C = [1   -0.996956   0.992019 ; -0.996956   1   -0.998603; 0.992019   -0.998603   1]   %(cor mtx).
  //U = chol(C)   % obtaining U such that Rcorr = R*U    (goal is to find U such that Ut * U = C , by the chelsky decomposition)
  //see http://www.sitmo.com/doc/Generating_Correlated_Random_Numbers   for more details
  
  TMatrixD U(3,3);
 
  //With 7TeV point
  U(0,0) = 1;
  U(0,1) = -0.9970;
  U(0,2) = 0.9920;
  U(1,1) = 0.0780;
  U(1,2) = -0.1232;
  U(2,2) = 0.0269;
  
  //Without 7TeV point
  U(0,0) = 1;
  U(0,1) = -0.9981;
  U(0,2) = 0.9950;
  U(1,1) = 0.0616;
  U(1,2) = -0.0973;
  U(2,2) = 0.0211;
  
  TMatrixD R(1,3);
  TMatrixD Rc(1,3);
  
  
  
  
  int npoints = 1000;
  int nresampling = 1000;
  vector<TMean> pull(npoints+2,TMean());
  vector<TH1F*> hpull;
  for(int n = 0 ; n <= npoints+1 ; ++n){
    ostringstream str("");
    str<<"hpull_"<<n;
    hpull.push_back(new TH1F(str.str().c_str() , str.str().c_str(),400,0,40));
  }
  int n_7TeV = 0;
  
  TF1* ftemp = (TF1*) f1->Clone("ftemp");
  vector <double> v_x(npoints+2,0);
  v_x.at(0) = log(f1->GetXmin());
  for(int n = 1 ; n <= npoints+1 ; ++n){
    if(v_x.at(n-1)+(log(f1->GetXmax()) - log(f1->GetXmin())) / npoints > log(7000) && v_x.at(n-1) < log(7000)){
      n_7TeV = n;
      v_x.at(n)=log(7000);//cout << v_x.at(n) << endl;
      v_x.at(n+1) = v_x.at(n-1) + (log(f1->GetXmax()) - log(f1->GetXmin())) / npoints;
      ++n;//cout << v_x.at(n) << endl;
      continue;
    }
    v_x.at(n) = v_x.at(n-1) + (log(f1->GetXmax()) - log(f1->GetXmin())) / npoints;//cout << v_x.at(n) << endl;
  } 
    //cout << "LLLLLLLLLLL  "  <<  ftemp->GetNumberFreeParameters() << endl;
  for(int i = 0 ; i <= nresampling ; ++i){
    //for(int npar = 0 ; npar < f1->GetNumberFreeParameters() ; ++npar)
    //  ftemp->SetParameter(npar,gRandom->Gaus(f1->GetParameter(npar),f1->GetParError(npar)));
    
    if(npara==3){
      R(0,0) = gRandom->Gaus(0,1);
      R(0,1) = gRandom->Gaus(0,1);
      R(0,2) = gRandom->Gaus(0,1);
      Rc = R * U;
      for(int npar = 0 ; npar < f1->GetNumberFreeParameters() ; ++npar)
        ftemp->SetParameter( npar , f1->GetParameter(npar) + Rc(0,npar) * f1->GetParError(npar) );
      
      cout << "   " << R(0,0) << "  " << R(0,1) << "  " << R(0,2) << endl;
      //cout << Rc(0,0) << "  " << Rc(0,1) << "  " << Rc(0,2) << endl;
    }
    /*if(npara==3){
      R(0,0) = gRandom->Gaus(f1->GetParameter(0),f1->GetParError(0));
      R(0,1) = gRandom->Gaus(f1->GetParameter(1),f1->GetParError(1));
      R(0,2) = gRandom->Gaus(f1->GetParameter(2),f1->GetParError(2));
      Rc = R * U;
      ftemp->SetParameters( Rc(0,0) ,Rc(0,1) , Rc(0,2) );
      cout << "   " << R(0,0) << "  " << R(0,1) << "  " << R(0,2) << endl;
      cout << Rc(0,0) << "  " << Rc(0,1) << "  " << Rc(0,2) << endl;
    }*/
    else if(npara==2){
      double prand1 = gRandom->Gaus(f1->GetParameter(0),f1->GetParError(0));
      double prand2 = gRandom->Gaus(f1->GetParameter(1),f1->GetParError(1));
      ftemp->SetParameter(1,prand1);
      ftemp->SetParameter(0,cor(0,1)*prand1 + sqrt(1-pow(cor(0,1) , 2)) * prand2) ;
      cout << "   " << prand1 << "  " << prand2 << endl;
      cout << prand1 << "  " << cor(0,1)*prand1 + sqrt(1-pow(cor(0,1) , 2)) * prand2 << endl;
      cout<<cor(1,1)<<endl;
    }
    //cout << ftemp->GetParameter(0) << endl;
    
    //double x = log(f1->GetXmin());
    for(int n = 0 ; n <= npoints+1 ; ++n){
      //cout << n << "  " << 
      pull.at(n).Add(ftemp->Eval(exp(v_x.at(n))));
      hpull.at(n)->Fill(ftemp->Eval(v_x.at(n)));
      //cout << "GGG   " << exp(x) << "  " << f1->Eval(exp(x)) << "  " << ftemp->Eval(exp(x)) << endl;
    } 
  }//end of loop over samples
  
  TGraphAsymmErrors* gf1 = new TGraphAsymmErrors(npoints+2);
  TGraphAsymmErrors* gf1_systm = new TGraphAsymmErrors(npoints+2);
  TGraphAsymmErrors* gf1_systp = new TGraphAsymmErrors(npoints+2);
  for(int n = 0 ; n <= npoints+1 ; ++n){
    //cout << pull.at(n).GetRMS() << endl;
    gf1->SetPoint( n , exp(v_x.at(n)) , f1->Eval(exp(v_x.at(n))) );
    gf1->SetPointError( n , 0 , 0 , pull.at(n).GetRMS() , pull.at(n).GetRMS() );
    
    gf1_systm->SetPoint( n , exp(v_x.at(n)) , f1->Eval(exp(v_x.at(n))) - pull.at(n).GetRMS() );
    gf1_systp->SetPoint( n , exp(v_x.at(n)) , f1->Eval(exp(v_x.at(n))) + pull.at(n).GetRMS() );
    gf1_systm->SetPointError( n , 0 , 0 , 0 , 0 );
    gf1_systp->SetPointError( n , 0 , 0 , 0 , 0 );
    
    
    cout << "    " << n << "  " << exp(v_x.at(n)) << "  " << f1->Eval(exp(v_x.at(n))) << "  " << pull.at(n).GetMean() << "  " << pull.at(n).GetRMS() << endl;
  }
  
  gf1->SetFillColor(16);
  gf1_systm->SetFillColor(16);
  gf1_systp->SetFillColor(16);
  gf1->SetMarkerStyle(kOpenCircle);
  gf1->SetFillStyle(3244);
  gf1->Draw("3same");
  gf1->Draw("Xlsame");
  //gf1_systm->Draw("cXsame");
  //gf1_systp->Draw("cXsame");
  
  //gPad->WaitPrimitive();
  
  
  TFile* f_mnch = new TFile("plotmnch_output.root","RECREATE");
  for(int n = 0 ; n <= npoints+1 ; ++n)
    hpull.at(n)->Write();
  gf1->Write();
  f_mnch->Close();
  
  //double chi2 = sqrt( pow(moments->mean->GetMean(),2) - pow(f1->Eval(exp(v_x.at(n_7TeV))),2) / ( moments->mean->_MeanSystM * pull.at(n_7TeV).GetRMS() ));
  double chi2 = (moments->mean->GetMean() - f1->Eval(exp(v_x.at(n_7TeV)))) /  sqrt( pow(moments->mean->_MeanSystM,2) + pow(pull.at(n_7TeV).GetRMS(),2));
 
  cout << endl << endl << " ====================================================================" << endl << endl;
  cout << "The 7 TeV point is at n = " << n_7TeV << " with a mean value of : " << f1->Eval(exp(v_x.at(n_7TeV))) << "  +-  " << pull.at(n_7TeV).GetRMS() << endl;
  cout << "Compared to our measurement : " << moments->mean->GetMean() << "  -  " <<  moments->mean->_MeanSystM << endl;
  cout << "Which means a Chisquare of : " << chi2;
  cout << endl << endl << " ====================================================================" << endl << endl;


  //gf1->Draw("l4same");
