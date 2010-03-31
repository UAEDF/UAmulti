//---------------------------------------------------------------------------------------------
//------------------------------   RESAMPLING INPUT DISTRI   ----------------------------------
//---------------------------------------------------------------------------------------------


TH1F resample(double matrix[][matrixsize], TH1F* nch_INC , TH1F* toUnfold , TH1F* nch_unfoldedPtr, TH1F* hyp, int niter = 5,
              bool doFit = false, TH1F* nch_mc_SD = NULL, bool printErrors = false){

  const int nresampling = 1000;
  
  TH1F sample[nresampling];

  //TH1F bins_beforeU[matrixsize];
  vector<TH1F> bins_beforeU(nch_INC->GetNbinsX(),TH1F());
  //TH1F bins[matrixsize];
  vector<TH1F> bins(nch_INC->GetNbinsX(),TH1F());// = new vector<TH1F>;
  char* name = "bin_%d";
  char* name_beforeU = "bin_beforeU_%d";
  char name2[60];
  for(int k=1;k<=nch_INC->GetNbinsX();++k){
    sprintf(name2,name,k);
    //cout<<name2<<endl;
    //cout<<bins.size()<<endl;
    //TH1F temp = (TH1F) nch_INC->Clone(name2);
    //(name2,name2,71,-0.5,70.5);
    //temp.SetLineColor(kRed);
    //temp.Draw();
    //gPad->WaitPrimitive();
    //if(temp==0) cout<<"pb"<<endl;
    if(nch_INC->Integral()<2)
      bins[k-1] = TH1F(name2,name2,1000,0.,1.);
    else
      bins[k-1] = TH1F(name2,name2,(int)nch_INC->GetMaximum()+951,-50.5,(int)nch_INC->GetMaximum()+900.5);
      
    //bins before U
    sprintf(name2,name_beforeU,k);
    if(nch_INC->Integral()<2)
      bins_beforeU[k-1] = TH1F(name2,name2,1000,0.,1.);
    else
      bins_beforeU[k-1] = TH1F(name2,name2,(int)nch_INC->GetMaximum()+951,-50.5,(int)nch_INC->GetMaximum()+900.5);
    //cout<<bins.size()<<"  "<<nch_INC->GetNbinsX()<<endl;
    //delete temp;
  }
  
  
  TH1F* out = (TH1F*) nch_INC->Clone("nch_resampled");
  TH1F* nch_beforeUnfolding_resampled = (TH1F*) nch_INC->Clone("nch_beforeUnfolding_resampled");
  TProfile* nch_resampled_profile = new TProfile("nch_resampled_profile","nch_resampled_profile",out->GetNbinsX(),out->GetXaxis()->GetXbins()->GetArray());
  nch_resampled_profile->SetErrorOption("s");
  //nch_resampled_profile->Sumw2(); // useless, already automatic
  
  TF1*Gauss = new TF1("Gauss","gaus");

  //TCanvas* c_kno = new TCanvas("c_kno2","c_kno2",200,510,500,500);
  //c_kno->cd();
  for(int i=0;i<nresampling;++i){
    char tmp[160] = "sample_nsample%d";
    char samplechar[160] = "";
    sprintf(samplechar,tmp,i+1);
    //cout<<samplechar<<endl;

    TH1F* nch_temp = new TH1F(samplechar,samplechar,toUnfold->GetNbinsX(),toUnfold->GetXaxis()->GetXbins()->GetArray());
    for(int n=1;n<=toUnfold->GetNbinsX();++n)
      nch_temp->SetBinContent(n,gRandom->Poisson(nch_INC->GetBinContent(n)));
      //nch_temp->SetBinContent(n,gRandom->Gaus(nch_INC->GetBinContent(n),nch_INC->GetBinError(n)));
    
    //substract SD with factor derived from resampled nch
    nch_temp->Add(nch_mc_SD,-1*(double(nch_temp->Integral())/double(nch_INC->GetEntries())));    
    
    for(int n=1;n<=toUnfold->GetNbinsX();++n) 
      bins_beforeU[n-1].Fill(nch_temp->GetBinContent(n));
      
    //TCanvas* c_kno = new TCanvas("c_kno","c_kno",200,510,500,500);
    //nch_temp->Draw();

    if(hyp==0) cout<<"PROBLEM -----> the hypothesis is void !!"<<endl;
    sample[i] = (runalgo(matrix,nch_temp,hyp,niter,i+1));
    //sample[i].SetLineColor(kRed);
    /*if(i==0)
      sample[i].Draw();
    else
      sample[i].Draw("same");*/
  
    for(int k=1;k<=toUnfold->GetNbinsX();++k){
      bins[k-1].Fill(sample[i].GetBinContent(k));
      nch_resampled_profile->Fill(double(k)-1.,sample[i].GetBinContent(k));
      //bins[k-1].SetLineColor(kBlue);
      //bins[k-1].Draw("same");
    }  
      
    //gPad->Update();
    //gPad->WaitPrimitive();
  }
  gPad->WaitPrimitive();
  gPad->Write();

  //TCanvas* c_kno = new TCanvas("c_kno","c_kno",200,510,500,500);
  for(int k=1;k<=toUnfold->GetNbinsX();++k){
    if(doFit){
      bins[k-1].Fit("Gauss","Q");
      out->SetBinContent(k,Gauss->GetParameter(1));
      out->SetBinError(k,Gauss->GetParameter(2));
      bins_beforeU[k-1].Fit("Gauss","Q");
      nch_beforeUnfolding_resampled->SetBinContent(k,Gauss->GetParameter(1));
      nch_beforeUnfolding_resampled->SetBinError(k,Gauss->GetParameter(2));
    }
    else {
      out->SetBinContent(k,bins[k-1].GetMean());
      out->SetBinError(k,bins[k-1].GetRMS());
      nch_beforeUnfolding_resampled->SetBinContent(k,bins_beforeU[k-1].GetMean());
      /*if(k<10)
        nch_beforeUnfolding_resampled->SetBinError(k,sqrt(bins_beforeU[k-1].GetMean()));
      else*/
        //nch_beforeUnfolding_resampled->SetBinError(k,bins_beforeU[k-1].GetRMS());
        nch_beforeUnfolding_resampled->SetBinError(k,getRMS(&bins_beforeU[k-1]));
	
       //cout<<k<<"  "<<getRMS(&bins[k-1])<<"  "<<bins[k-1].GetRMS()<<endl;
    }
    
    /*if(k>=0){
      bins_beforeU[k-1].Draw();
      gPad->Update();
      gPad->WaitPrimitive();   
    }*/
  }
  
  //Making plots to check all the errors:
  TH1F* error_diff_resampled_bU_aU = (TH1F*) toUnfold->Clone("error_diff_resampled_bU_aU");
  TH1F* error_diff_resampled_data_bU = (TH1F*) toUnfold->Clone("error_diff_resampled_data_bU");
  TH1F* error_diff_data_bU_aU = (TH1F*) toUnfold->Clone("error_diff_data_bU_aU");
  TH1F* error_diff_data_aU_rms_poiss = (TH1F*) toUnfold->Clone("error_diff_data_aU_rms_poiss");
  
  error_diff_resampled_bU_aU->Reset();
  error_diff_resampled_data_bU->Reset();
  error_diff_data_bU_aU->Reset();
  error_diff_data_aU_rms_poiss->Reset();
  
  for(int i=1;i<=toUnfold->GetNbinsX()*printErrors;++i){
    /*error_diff_resampled_bU_aU->SetBinContent(i,0);
    error_diff_data_bU_aU->SetBinContent(i,0);
    error_diff_resampled_data_bU->SetBinContent(i,0);
    
    error_diff_resampled_bU_aU->SetBinError(i,nch_beforeUnfolding_resampled->GetBinError(i) - out->GetBinError(i));
    error_diff_data_bU_aU->SetBinError(i,toUnfold->GetBinError(i) - out->GetBinError(i));
    error_diff_resampled_data_bU->SetBinError(i,toUnfold->GetBinError(i) - nch_beforeUnfolding_resampled->GetBinError(i));
    
    error_diff_resampled_bU_aU->SetBinContent(i,nch_beforeUnfolding_resampled->GetBinError(i) - out->GetBinError(i));
    error_diff_data_bU_aU->SetBinContent(i,toUnfold->GetBinError(i) - out->GetBinError(i));
    error_diff_resampled_data_bU->SetBinContent(i,toUnfold->GetBinError(i) - nch_beforeUnfolding_resampled->GetBinError(i));*/
    
    cout << "*** BIN " << i << " ***" << endl;
    cout<<"dbU_rbU_daU_raU (content,sqrt(content),error) : " 
    						 << toUnfold->GetBinContent(i) << " , " << sqrt(toUnfold->GetBinContent(i)) << " , " << toUnfold->GetBinError(i)
    				       <<"  |  " << nch_beforeUnfolding_resampled->GetBinContent(i) << " , " << sqrt(nch_beforeUnfolding_resampled->GetBinContent(i)) << " , " << nch_beforeUnfolding_resampled->GetBinError(i)
    				       <<"  |  " << nch_unfoldedPtr->GetBinContent(i) << " , " << sqrt(nch_unfoldedPtr->GetBinContent(i)) << " , " << out->GetBinError(i)
    				       <<"  |  " << out->GetBinContent(i) << " , " << sqrt(out->GetBinContent(i))  << " , " << out->GetBinError(i) << endl;
    
    
    double error = 0;
    if(nch_beforeUnfolding_resampled->GetBinError(i)!=0) error = (nch_beforeUnfolding_resampled->GetBinError(i) - out->GetBinError(i))/nch_beforeUnfolding_resampled->GetBinError(i);
    cout << "relative err resampled_bU_aU: " <<"  "<<error << " == " << nch_beforeUnfolding_resampled->GetBinError(i) << " - " <<out->GetBinError(i) << "/first" <<endl;
    error_diff_resampled_bU_aU->SetBinContent(i,error);
        
    if(toUnfold->GetBinError(i)!=0) error = (nch_beforeUnfolding_resampled->GetBinError(i) - toUnfold->GetBinError(i))/nch_beforeUnfolding_resampled->GetBinError(i);
    cout << "relative err resampled-data_bU: " <<"  "<<error << " == " << nch_beforeUnfolding_resampled->GetBinError(i) << " - " <<toUnfold->GetBinError(i) << "/first" <<endl;
    error_diff_resampled_data_bU->SetBinContent(i,error);
    
    
    
    if(toUnfold->GetBinError(i)!=0) error = (toUnfold->GetBinError(i) - out->GetBinError(i))/toUnfold->GetBinError(i);
    cout << "relative err data_bU_aU: " <<"  "<<error << " == " << toUnfold->GetBinError(i) << " - " <<out->GetBinError(i) << "/first" <<endl;
    error_diff_data_bU_aU->SetBinContent(i,error);
    
    if(toUnfold->GetBinError(i)!=0) error = (out->GetBinError(i)-sqrt(out->GetBinContent(i)))/out->GetBinError(i);
    cout << "relative err data_aU_rms_poiss: " <<"  "<<error << " == " << out->GetBinError(i) << " - " <<sqrt(out->GetBinContent(i)) << "/first" <<endl;
    error_diff_data_aU_rms_poiss->SetBinContent(i,error);
  
  }
  
  error_diff_resampled_bU_aU->Write();
  error_diff_data_bU_aU->Write();
  error_diff_resampled_data_bU->Write();
  nch_beforeUnfolding_resampled->Write();
  nch_resampled_profile->Write();
  //out->Draw();
  //gPad->WaitPrimitive();
  return *out;
}








//---------------------------------------------------------------------------------------------
//------------------------------   RESAMPLING INPUT MATRIX   ----------------------------------
//---------------------------------------------------------------------------------------------


TH1F resample(double matrix[][matrixsize] , TH1F* toUnfold , TH1F* hyp, int niter = 5,
              bool doFit = false, bool printErrors = false){

  const int nresampling = 1000;
  
  TH1F sample[nresampling];

  //TH1F bins[matrixsize];
  vector<TH1F> bins(toUnfold->GetNbinsX(),TH1F());// = new vector<TH1F>;
  char* name = "bin_%d";
  char name2[60];
  for(int k=1;k<=toUnfold->GetNbinsX();++k){
    sprintf(name2,name,k);
    
    if(toUnfold->Integral()<2)
      bins[k-1] = TH1F(name2,name2,1000,0.,1.);
    else
      bins[k-1] = TH1F(name2,name2,(int)toUnfold->GetMaximum()+951,-50.5,(int)toUnfold->GetMaximum()+900.5);
  }
  
  
  TH1F* out = (TH1F*) toUnfold->Clone("nch_resampled");
  TProfile* nch_resampled_profile = new TProfile("nch_resampled_profile","nch_resampled_profile",out->GetNbinsX(),out->GetXaxis()->GetXbins()->GetArray());
  nch_resampled_profile->SetErrorOption("s");
  //nch_resampled_profile->Sumw2();
  
  TF1*Gauss = new TF1("Gauss","gaus");

  //TCanvas* c_kno = new TCanvas("c_kno2","c_kno2",200,510,500,500);
  //c_kno->cd();
  for(int i=0;i<nresampling;++i){

    //creating the matrix
    matrix4dObj mtx_tmp;
    for(int n=0;n<matrixsize;++n){
      for(int m=0;m<matrixsize;++m)
        mtx_tmp[n][m] = gRandom->Poisson(matrix[n][m]);
      //nch_temp->SetBinContent(n,gRandom->Gaus(nch_INC->GetBinContent(n),nch_INC->GetBinError(n)));
    }
    
    if(hyp==0) cout<<"PROBLEM -----> the hypothesis is void !!"<<endl;
    sample[i] = (runalgo(mtx_tmp,toUnfold,hyp,niter,i+1));
    //sample[i].SetLineColor(kRed);
    /*if(i==0)
      sample[i].Draw();
    else
      sample[i].Draw("same");*/
  
    for(int k=1;k<=toUnfold->GetNbinsX();++k){
      bins[k-1].Fill(sample[i].GetBinContent(k));
      nch_resampled_profile->Fill(double(k)-1.,sample[i].GetBinContent(k));
      //bins[k-1].SetLineColor(kBlue);
      //bins[k-1].Draw("same");
    }  
      
  }

  //TCanvas* c_kno = new TCanvas("c_kno","c_kno",200,510,500,500);
  for(int k=1;k<=toUnfold->GetNbinsX();++k){
    if(doFit){
      bins[k-1].Fit("Gauss","Q");
      out->SetBinContent(k,Gauss->GetParameter(1));
      out->SetBinError(k,Gauss->GetParameter(2));
    }
    else {
      out->SetBinContent(k,bins[k-1].GetMean());
      out->SetBinError(k,bins[k-1].GetRMS());
    }
  }
  
  //Making plots to check all the errors:
   
  TH1F* error_diff_data_bU_aU = (TH1F*) toUnfold->Clone("error_diff_data_bU_aU");
  error_diff_data_bU_aU->Reset();
  
  for(int i=1;i<=toUnfold->GetNbinsX()*printErrors;++i){
     
    
    double error = 0;
         
    
    if(toUnfold->GetBinError(i)!=0) error = (toUnfold->GetBinError(i) - out->GetBinError(i))/toUnfold->GetBinError(i);
    cout << "relative err data_bU_aU: " <<"  "<<error << " == " << toUnfold->GetBinError(i) << " - " <<out->GetBinError(i) << "/first" <<endl;
    error_diff_data_bU_aU->SetBinContent(i,error);
      
  }
  
  error_diff_data_bU_aU->Write();
  nch_resampled_profile->Write();
  //out->Draw();
  //gPad->WaitPrimitive();
  return *out;
}
