//---------------------------------------------------------------------------------------------
//------------------------------   RESAMPLING INPUT DISTRI   ----------------------------------
//---------------------------------------------------------------------------------------------


TH1F resample(TH2F* matrixhist, TH1F* nch_INC , TH1F* toUnfold , TH1F* nch_unfoldedPtr, TH1F* hyp, int niter = 5,
              bool doFit = false, TMoments* moment = NULL, TH1F* eff_evtSel = NULL, bool writeAllBins = false , bool printErrors = false){    // , TH1F* nch_mc_SD = NULL


  const int nresampling = (niter==0)? 1 : 1000; 
  ///
  //cout << "YES BEGINNING" <<endl;
  TH1F sample[nresampling];

  //TH1F bins_beforeU[matrixsize];
  vector<TH1F> bins_beforeU(nch_INC->GetNbinsX(),TH1F());
  //TH1F bins[matrixsize];
  vector<TH1F> bins(nch_INC->GetNbinsX(),TH1F());// = new vector<TH1F>;
  ostringstream name("");
  ostringstream name_beforeU("");
  
  if(writeAllBins){
    for(int k=1;k<=nch_INC->GetNbinsX();++k){
    
      //enabling automatic rebinning
      bins[k-1].SetBit(TH1::kCanRebin);
      bins_beforeU[k-1].SetBit(TH1::kCanRebin);
      
      name.str("");
      name << "bin_" << k;
      if(nch_INC->Integral()<2)
        bins[k-1] = TH1F(name.str().c_str(),name.str().c_str(),1000,0.,1.);
      else
        bins[k-1] = TH1F(name.str().c_str(),name.str().c_str(),(int) 2 * sqrt(nch_INC->GetBinContent(k)) ,
                                                               (int) (nch_INC->GetBinContent(k) - sqrt(nch_INC->GetBinContent(k))) ,
							       (int) (nch_INC->GetBinContent(k) + sqrt(nch_INC->GetBinContent(k))));
       

      

      name_beforeU.str("");
      name_beforeU << "bin_beforeU_" << k;

      if(nch_INC->Integral()<2)
        bins_beforeU[k-1] = TH1F(name_beforeU.str().c_str(),name_beforeU.str().c_str(),1000,0.,1.);
      else 
        bins_beforeU[k-1] = TH1F(name_beforeU.str().c_str(),name_beforeU.str().c_str(),(int) 2 * sqrt(nch_INC->GetBinContent(k)),
                                                          (int) (nch_INC->GetBinContent(k) - sqrt(nch_INC->GetBinContent(k))),
                                                          (int) (nch_INC->GetBinContent(k) + sqrt(nch_INC->GetBinContent(k))));
							


       
    }
  }
  
  
  vector<TMean> pull_INC_bU(nch_INC->GetNbinsX(),TMean());
  //vector<TMean> pull_NSD_bU(nch_INC->GetNbinsX(),TMean());
  vector<TMean> pull_aU(nch_INC->GetNbinsX(),TMean()); 
  vector< vector<TMean> > mtxpull_aU(nch_INC->GetNbinsX(),vector<TMean>(nch_INC->GetNbinsX(),TMean()));
  
  TH1F* out = (TH1F*) nch_INC->Clone("nch_resampled");
  TH1F* nch_beforeUnfolding_resampled = (TH1F*) nch_INC->Clone("nch_beforeUnfolding_resampled");
  TProfile* nch_resampled_profile = new TProfile("nch_resampled_profile","nch_resampled_profile",out->GetNbinsX(),out->GetXaxis()->GetXbins()->GetArray());
  nch_resampled_profile->SetErrorOption("s");
  //nch_resampled_profile->Sumw2(); // useless, already automatic
  
  TF1*Gauss = new TF1("Gauss","gaus");

  //TCanvas* c_kno = new TCanvas("c_kno2","c_kno2",200,510,500,500);
  //c_kno->cd();
  ostringstream samplechar("");
  for(int i=0;i<nresampling;++i){
    samplechar.str("");
    samplechar << "sample_nsample" << i;

    TH1F* nch_temp = new TH1F(samplechar.str().c_str(),samplechar.str().c_str(),toUnfold->GetNbinsX(),toUnfold->GetXaxis()->GetXbins()->GetArray());
    for(int n=1;n<=toUnfold->GetNbinsX();++n)
      nch_temp->SetBinContent(n,gRandom->Poisson(nch_INC->GetBinContent(n)));
      //nch_temp->SetBinContent(n,gRandom->Gaus(nch_INC->GetBinContent(n),nch_INC->GetBinError(n)));
    
    for(int n=1;n<=toUnfold->GetNbinsX();++n){
    
      if(writeAllBins){
        //if is out of range, will automatically rebin !!
        bins_beforeU[n-1].Fill(nch_temp->GetBinContent(n));
      }
      
      pull_INC_bU.at(n-1).Add(nch_temp->GetBinContent(n));
    }

    //substract SD with factor derived from resampled nch
  //  nch_temp->Add(nch_mc_SD,-1);//*(double(nch_temp->Integral())/double(nch_INC->GetEntries())));    
    
    //for(int n=1;n<=toUnfold->GetNbinsX();++n)
    //  pull_NSD_bU.at(n-1).Add(nch_temp->GetBinContent(n));
    
    
    //TCanvas* c_kno = new TCanvas("c_kno","c_kno",200,510,500,500);
    //nch_temp->Draw();

    if(hyp==0) cout<<"PROBLEM -----> the hypothesis is void !!"<<endl;
    sample[i] = (runalgo(matrixhist,nch_temp,hyp,niter,i+1));
    delete nch_temp;
    //sample[i].SetLineColor(kRed);
    /*if(i==0)
      sample[i].Draw();
    else
      sample[i].Draw("same");*/
    
    sample[i].Divide(&(sample[i]),eff_evtSel,1,1);
   // if(doptcorr) increaseNTracks(&(sample[i]) , ptcorr_val);
    
    for(int k=1;k<=toUnfold->GetNbinsX();++k){
      if(writeAllBins)
        bins[k-1].Fill(sample[i].GetBinContent(k));
      nch_resampled_profile->Fill(nch_resampled_profile->GetBinCenter(k) , sample[i].GetBinContent(k));
      pull_aU.at(k-1).Add(sample[i].GetBinContent(k));
      
      for(int l=1;l<=toUnfold->GetNbinsX();++l){
        mtxpull_aU.at(k-1).at(l-1).Add(sample[i].GetBinContent(k) * sample[i].GetBinContent(l) );
      }
      
      //bins[k-1].SetLineColor(kBlue);
      //bins[k-1].Draw("same");
    }  
     ////
   //cout << "AFTER FOR LOOP 3 line 114" <<endl;     
    //gPad->Update();
    //gPad->WaitPrimitive();
  
    divideByWidth(&(sample[i]));
    //sample[i].Scale(1./sample[i].Integral());
    moment->AddDataForResampling(&(sample[i]));
  
  }//end of loop over samples
  
  moment->FinishCurrentResampling();
  
  //gPad->WaitPrimitive();
  //gPad->Write();

  //TCanvas* c_kno = new TCanvas("c_kno","c_kno",200,510,500,500);
  //TH1F* allpull = new TH1F("allpull","allpull",50,-5,5);
  
  TH2F* hmtxpull_aU = new TH2F("hmtxpull_aU","hmtxpull_aU;n_{i};n_{j};<P_{i}P_{j}>",toUnfold->GetNbinsX(),toUnfold->GetXaxis()->GetXbins()->GetArray(),toUnfold->GetNbinsX(),toUnfold->GetXaxis()->GetXbins()->GetArray());
  TH2F* covmtx      = new TH2F("covmtx"     ,"covmtx;n_{i};n_{j};#sigma^{2}"       ,toUnfold->GetNbinsX(),toUnfold->GetXaxis()->GetXbins()->GetArray(),toUnfold->GetNbinsX(),toUnfold->GetXaxis()->GetXbins()->GetArray());
  TH2F* cormtx      = new TH2F("cormtx"     ,"cormtx;n_{i};n_{j};#rho"             ,toUnfold->GetNbinsX(),toUnfold->GetXaxis()->GetXbins()->GetArray(),toUnfold->GetNbinsX(),toUnfold->GetXaxis()->GetXbins()->GetArray());
  
  for(int k=1;k<=toUnfold->GetNbinsX();++k){
    
    double mean_aU = 0 , rms_aU = 0;
    double mean_bU = 0 , rms_bU = 0;
    
    if(doFit && writeAllBins){
      bins[k-1].Fit("Gauss","Q");
      mean_aU = Gauss->GetParameter(1);
      rms_aU  = Gauss->GetParameter(2);
      
      bins_beforeU[k-1].Fit("Gauss","Q");
      mean_bU = Gauss->GetParameter(1);
      rms_bU  = Gauss->GetParameter(2);
    }
    else {
      //mean_aU = bins[k-1].GetMean();
      //rms_aU  = bins[k-1].GetRMS();
      mean_aU = pull_aU[k-1].GetMean();
      rms_aU  = pull_aU[k-1].GetRMS();
      
      mean_bU = pull_INC_bU[k-1].GetMean();
      rms_bU  = pull_INC_bU[k-1].GetRMS(); 
    }
    
    out->SetBinContent(k,mean_aU);
    out->SetBinError(k,rms_aU);
    nch_beforeUnfolding_resampled->SetBinContent(k,mean_bU);
    nch_beforeUnfolding_resampled->SetBinError(k,rms_bU);
    
    //making the pull distri
    //ostringstream pullname("");
    //pullname<<"pull_bin"<<k;
    //TH1F* pull = (TH1F*) allpull->Clone(pullname.str().c_str());
    //pull->Reset();
    /*for(int ibin = 1 ; ibin <= bins[k-1].GetNbinsX() ; ++ibin){
      allpull->Fill( (bins[k-1].GetBinCenter(ibin)-mean) / rms , bins[k-1].GetBinContent(ibin) / bins[k-1].Integral() );
      pull->Fill( (bins[k-1].GetBinCenter(ibin)-mean) / rms , bins[k-1].GetBinContent(ibin) / bins[k-1].Integral() );
    }*/
    //pull->Write();
    //delete pull;
        
    /*if(k>=0){
      bins_beforeU[k-1].Draw();
      gPad->Update();
      gPad->WaitPrimitive();   
    }*/
    
    
    //--------------------------------------------------------------------------------------------------
    //----------------------------------   making the covariance mtx   ---------------------------------
    //--------------------------------------------------------------------------------------------------
    
    for(int l=1;l<=toUnfold->GetNbinsX();++l){
      hmtxpull_aU->SetBinContent(k , l , mtxpull_aU.at(k-1).at(l-1).GetMean() );
      hmtxpull_aU->SetBinError(k , l , mtxpull_aU.at(k-1).at(l-1).GetRMS() );
      
      covmtx->SetBinContent(k , l , (mtxpull_aU.at(k-1).at(l-1).GetMean() - pull_aU[k-1].GetMean() * pull_aU[l-1].GetMean() ) );
      covmtx->SetBinError  (k , l ,  0 );
      
      if( pull_aU[k-1].GetRMS() * pull_aU[l-1].GetRMS() != 0 )
        cormtx->SetBinContent(k , l , (mtxpull_aU.at(k-1).at(l-1).GetMean() - pull_aU[k-1].GetMean() * pull_aU[l-1].GetMean()) / (pull_aU[k-1].GetRMS() * pull_aU[l-1].GetRMS()) );
      cormtx->SetBinError  (k , l ,  0 );
    
    }
    
    
  
  }
  
  //allpull->Fit("Gauss","Q");
  //allpull->Write();
  
  //Making plots to check all the errors:
  TH1F* error_diff_resampled_bU_aU = (TH1F*) toUnfold->Clone("error_diff_resampled_bU_aU");
  TH1F* error_diff_resampled_data_bU = (TH1F*) toUnfold->Clone("error_diff_resampled_data_bU");
  TH1F* error_diff_data_bU_aU = (TH1F*) toUnfold->Clone("error_diff_data_bU_aU");
  TH1F* error_diff_data_aU_rms_poiss = (TH1F*) toUnfold->Clone("error_diff_data_aU_rms_poiss");
  
  error_diff_resampled_bU_aU->Reset();
  error_diff_resampled_data_bU->Reset();
  error_diff_data_bU_aU->Reset();
  error_diff_data_aU_rms_poiss->Reset();
  
  for(int i=1;i<=toUnfold->GetNbinsX();++i){
    /*error_diff_resampled_bU_aU->SetBinContent(i,0);
    error_diff_data_bU_aU->SetBinContent(i,0);
    error_diff_resampled_data_bU->SetBinContent(i,0);
    
    error_diff_resampled_bU_aU->SetBinError(i,nch_beforeUnfolding_resampled->GetBinError(i) - out->GetBinError(i));
    error_diff_data_bU_aU->SetBinError(i,toUnfold->GetBinError(i) - out->GetBinError(i));
    error_diff_resampled_data_bU->SetBinError(i,toUnfold->GetBinError(i) - nch_beforeUnfolding_resampled->GetBinError(i));
    
    error_diff_resampled_bU_aU->SetBinContent(i,nch_beforeUnfolding_resampled->GetBinError(i) - out->GetBinError(i));
    error_diff_data_bU_aU->SetBinContent(i,toUnfold->GetBinError(i) - out->GetBinError(i));
    error_diff_resampled_data_bU->SetBinContent(i,toUnfold->GetBinError(i) - nch_beforeUnfolding_resampled->GetBinError(i));*/
    
    if(printErrors){
    cout << "*** BIN " << i << " ***" << endl;
    cout<<"dbU_rbU_daU_raU (content,sqrt(content),error) : " 
    						 << nch_INC->GetBinContent(i) << " , " << sqrt(nch_INC->GetBinContent(i)) << " , " << nch_INC->GetBinError(i)
    				       <<"  |  " << nch_beforeUnfolding_resampled->GetBinContent(i) << " , " << sqrt(nch_beforeUnfolding_resampled->GetBinContent(i)) << " , " << nch_beforeUnfolding_resampled->GetBinError(i)
    				       <<"  |  " << nch_unfoldedPtr->GetBinContent(i) << " , " << sqrt(nch_unfoldedPtr->GetBinContent(i)) << " , " << out->GetBinError(i)
    				       <<"  |  " << out->GetBinContent(i) << " , " << sqrt(out->GetBinContent(i))  << " , " << out->GetBinError(i) << endl;
    }
    
    double error = 0;
    if(nch_beforeUnfolding_resampled->GetBinError(i)!=0) error = (nch_beforeUnfolding_resampled->GetBinError(i) - out->GetBinError(i))/nch_beforeUnfolding_resampled->GetBinError(i);
    if(printErrors)cout << "relative err resampled_bU_aU: " <<"  "<<error << " == " << nch_beforeUnfolding_resampled->GetBinError(i) << " - " <<out->GetBinError(i) << "/first" <<endl;
    error_diff_resampled_bU_aU->SetBinContent(i,error);
    
    error = 0;
    if(nch_beforeUnfolding_resampled->GetBinError(i)!=0) error = (nch_beforeUnfolding_resampled->GetBinError(i) - nch_INC->GetBinError(i))/nch_beforeUnfolding_resampled->GetBinError(i);
    if(printErrors)cout << "relative err resampled-data_bU: " <<"  "<<error << " == " << nch_beforeUnfolding_resampled->GetBinError(i) << " - " <<nch_INC->GetBinError(i) << "/first" <<endl;
    error_diff_resampled_data_bU->SetBinContent(i,error);
    
    
    error = 0;
    if(nch_INC->GetBinError(i)!=0) error = (nch_INC->GetBinError(i) - out->GetBinError(i))/nch_INC->GetBinError(i);
    if(printErrors)cout << "relative err data_bU_aU: " <<"  "<<error << " == " << nch_INC->GetBinError(i) << " - " <<out->GetBinError(i) << "/first" <<endl;
    error_diff_data_bU_aU->SetBinContent(i,error);
    
    error = 0;
    if(out->GetBinError(i)!=0) error = (out->GetBinError(i)-sqrt(out->GetBinContent(i)))/out->GetBinError(i);
    if(printErrors)cout << "relative err data_aU_rms_poiss: " <<"  "<<error << " == " << out->GetBinError(i) << " - " <<sqrt(out->GetBinContent(i)) << "/first" <<endl;
    error_diff_data_aU_rms_poiss->SetBinContent(i,error);
    
    
    if(printErrors){
    
    cout<<nch_INC->GetBinContent(i)<<" , "<<nch_INC->GetBinError(i)<<"  |  "
        <<toUnfold->GetBinContent(i)<<" , "<<toUnfold->GetBinError(i)<<"  |  "
        <<nch_unfoldedPtr->GetBinContent(i)    <<" , "<<nch_unfoldedPtr->GetBinError(i)
	<<endl;
    
    cout<<pull_INC_bU.at(i-1).GetMean()<<" , "<<pull_INC_bU.at(i-1).GetRMS()<<"  |  "
   //     <<pull_NSD_bU.at(i-1).GetMean()<<" , "<<pull_NSD_bU.at(i-1).GetRMS()<<"  |  "
        <<pull_aU.at(i-1).GetMean()    <<" , "<<pull_aU.at(i-1).GetRMS()
	<<endl;
	
    cout<<(pull_INC_bU.at(i-1).GetMean() - nch_INC->GetBinContent(i)) / pull_INC_bU.at(i-1).GetMean() <<" , "<<(pull_INC_bU.at(i-1).GetRMS() - nch_INC->GetBinError(i)) / pull_INC_bU.at(i-1).GetRMS()<<"  |  "
    //    <<(pull_NSD_bU.at(i-1).GetMean() - toUnfold->GetBinContent(i)) / pull_NSD_bU.at(i-1).GetMean() <<" , "<<(pull_NSD_bU.at(i-1).GetRMS() - toUnfold->GetBinError(i)) / pull_NSD_bU.at(i-1).GetRMS()<<"  |  "
        <<(pull_aU.at(i-1).GetMean() - nch_unfoldedPtr->GetBinContent(i)) / pull_aU.at(i-1).GetMean()  <<" , "<<(pull_aU.at(i-1).GetRMS() - nch_unfoldedPtr->GetBinError(i)) / pull_aU.at(i-1).GetRMS()
	<<endl;
	
    cout<<(pull_INC_bU.at(i-1).GetRMS() / pull_INC_bU.at(i-1).GetMean() - nch_INC->GetBinError(i) / nch_INC->GetBinContent(i)) / (pull_INC_bU.at(i-1).GetRMS() / pull_INC_bU.at(i-1).GetMean())<<"  |  "
   //     <<(pull_NSD_bU.at(i-1).GetRMS() / pull_NSD_bU.at(i-1).GetMean() - toUnfold->GetBinError(i) / toUnfold->GetBinContent(i)) / (pull_NSD_bU.at(i-1).GetRMS() / pull_NSD_bU.at(i-1).GetMean())<<"  |  "
        <<(pull_aU.at(i-1).GetRMS() / pull_aU.at(i-1).GetMean() - nch_unfoldedPtr->GetBinError(i) / nch_unfoldedPtr->GetBinContent(i)) / (pull_aU.at(i-1).GetRMS() / pull_aU.at(i-1).GetMean())
	<<endl;
	
    }
	
  }
  
  if(writeAllBins){
    for(int n=1;n<=toUnfold->GetNbinsX();++n){
      bins[n-1].Write();
      bins_beforeU[n-1].Write();
    }   
  }
  error_diff_resampled_bU_aU->Write();
  error_diff_data_bU_aU->Write();
  error_diff_resampled_data_bU->Write();
  nch_beforeUnfolding_resampled->Write();
  nch_resampled_profile->Write();

  error_diff_data_aU_rms_poiss->Write();
  hmtxpull_aU->Write();
  covmtx->Write();
  out->Write();
  cormtx->Write();
  //out->Draw();
  //gPad->WaitPrimitive();
  cout<<"hist "<< out->GetBinError(31) << endl;
  
  delete nch_resampled_profile;
  delete Gauss;
  delete hmtxpull_aU;
  delete covmtx;
  delete cormtx;
  
  delete error_diff_resampled_bU_aU;
  delete error_diff_resampled_data_bU;
  delete error_diff_data_bU_aU;
  delete error_diff_data_aU_rms_poiss;
    
  return *out;
}








//---------------------------------------------------------------------------------------------
//------------------------------   RESAMPLING INPUT MATRIX   ----------------------------------
//---------------------------------------------------------------------------------------------


TH1F mtxresample(TH2F* matrixhist , TH1F* toUnfold , TH1F* hyp , int niter = 5,
              TMoments* moment = NULL , TH1F* eff_evtSel = NULL , bool doFit = false,
	      bool writeAllBins = false , bool printErrors = false){
  
  const int nresampling = (niter==0)? 1 : 1000; 
  
  TH1F sample[nresampling];
  
  //TH1F bins[matrixsize];
  vector<TH1F> bins(toUnfold->GetNbinsX(),TH1F());// = new vector<TH1F>;
  if(writeAllBins){
    ostringstream name("");
    for(int k=1;k<=toUnfold->GetNbinsX();++k){   
      name.str("");
      name << "bin_" << k;
      if(toUnfold->Integral()<2){     
        bins[k-1] = TH1F(name.str().c_str(),name.str().c_str(),1000,0.,1.);       
      }  
      else {     
        bins[k-1] = TH1F(name.str().c_str(),name.str().c_str(),(int) 2 * sqrt(toUnfold->GetBinContent(k)) ,
                                                               (int) (toUnfold->GetBinContent(k) - sqrt(toUnfold->GetBinContent(k))) ,
							       (int) (toUnfold->GetBinContent(k) + sqrt(toUnfold->GetBinContent(k))));
      }                             
       
      //enabling automatic rebinning
      bins[k-1].SetBit(TH1::kCanRebin);
       
    }
  } 
  TH1F* out = (TH1F*) toUnfold->Clone("nch_resampled");
  TProfile* nch_resampled_profile = new TProfile("nch_resampled_profile","nch_resampled_profile",out->GetNbinsX(),out->GetXaxis()->GetXbins()->GetArray());
  nch_resampled_profile->SetErrorOption("s");
  //nch_resampled_profile->Sumw2();
  
  vector<TMean> pull_aU(toUnfold->GetNbinsX(),TMean()); 
  vector< vector<TMean> > mtxpull_aU(toUnfold->GetNbinsX(),vector<TMean>(toUnfold->GetNbinsX(),TMean()));
  TF1*Gauss = new TF1("Gauss","gaus");
  //TCanvas* c_kno = new TCanvas("c_kno2","c_kno2",200,510,500,500);
  //c_kno->cd();
  for(int i=0;i<nresampling;++i){   
    //creating the matrix
    TH2F* mtx_tmp = (TH2F*) matrixhist->Clone("mtx_tmp");
    for(int n=0;n<matrixsize;++n){
      for(int m=0;m<matrixsize;++m){
        //mtx_tmp[n][m] = gRandom->Poisson(matrix[n][m]);
        mtx_tmp->SetBinContent(n,m, gRandom->Poisson(matrixhist->GetBinContent(n,m)));
      //nch_temp->SetBinContent(n,gRandom->Gaus(nch_INC->GetBinContent(n),nch_INC->GetBinError(n)));
      }
    }
    
    if(hyp==0) cout<<"PROBLEM -----> the hypothesis is void !!"<<endl;
  
    sample[i] = (runalgo(mtx_tmp,toUnfold,hyp,niter,i+1));
    //sample[i].SetLineColor(kRed);
    //if(i==0)
    //  sample[i].Draw();
    //else
    //  sample[i].Draw("same");   
    sample[i].Divide(&(sample[i]),eff_evtSel,1,1);
  //  if(doptcorr) increaseNTracks(&(sample[i]) , ptcorr_val);   
    for(int k=1;k<=toUnfold->GetNbinsX();++k){
      if(writeAllBins)
        bins[k-1].Fill(sample[i].GetBinContent(k));   
      nch_resampled_profile->Fill(double(k)-1.,sample[i].GetBinContent(k));
      //bins[k-1].SetLineColor(kBlue);
      //bins[k-1].Draw("same");     
      pull_aU.at(k-1).Add(sample[i].GetBinContent(k));     
      for(int l=1;l<=toUnfold->GetNbinsX();++l){
         
        mtxpull_aU.at(k-1).at(l-1).Add(sample[i].GetBinContent(k) * sample[i].GetBinContent(l) );
      }
      
    }    
    divideByWidth(&(sample[i]));
    //sample[i].Scale(1./sample[i].Integral());
    moment->AddDataForResampling(&(sample[i]));
  
  delete mtx_tmp;
  }//end of loop over samples 
  moment->FinishCurrentResampling();

  TH2F* hmtxpull_aU = new TH2F("hmtxpull_aU","hmtxpull_aU;n_{i};n_{j};<P_{i}P_{j}>",toUnfold->GetNbinsX(),toUnfold->GetXaxis()->GetXbins()->GetArray(),toUnfold->GetNbinsX(),toUnfold->GetXaxis()->GetXbins()->GetArray());
  TH2F* covmtx      = new TH2F("covmtx"     ,"covmtx;n_{i};n_{j};#sigma^{2}"       ,toUnfold->GetNbinsX(),toUnfold->GetXaxis()->GetXbins()->GetArray(),toUnfold->GetNbinsX(),toUnfold->GetXaxis()->GetXbins()->GetArray());
  TH2F* cormtx      = new TH2F("cormtx"     ,"cormtx;n_{i};n_{j};#rho"             ,toUnfold->GetNbinsX(),toUnfold->GetXaxis()->GetXbins()->GetArray(),toUnfold->GetNbinsX(),toUnfold->GetXaxis()->GetXbins()->GetArray());
   
  //TCanvas* c_kno = new TCanvas("c_kno","c_kno",200,510,500,500);
  for(int k=1;k<=toUnfold->GetNbinsX();++k){
    
    double mean_aU = 0 , rms_aU = 0;
    
    if(doFit && writeAllBins){     
      bins[k-1].Fit("Gauss","Q");
      mean_aU = Gauss->GetParameter(1);
      rms_aU  = Gauss->GetParameter(2);
    }
    else {
      //mean_aU = bins[k-1].GetMean();
      //rms_aU  = bins[k-1].GetRMS();
      mean_aU = pull_aU[k-1].GetMean();
      rms_aU  = pull_aU[k-1].GetRMS();
    }
    
    out->SetBinContent(k,mean_aU);
    out->SetBinError(k,rms_aU);
    
    //--------------------------------------------------------------------------------------------------
    //----------------------------------   making the covariance mtx   ---------------------------------
    //-------------------------------------------------------------------------------------------------- 
    for(int l=1;l<=toUnfold->GetNbinsX();++l){
      hmtxpull_aU->SetBinContent(k , l , mtxpull_aU.at(k-1).at(l-1).GetMean() );
      hmtxpull_aU->SetBinError(k , l , mtxpull_aU.at(k-1).at(l-1).GetRMS() );
      
      covmtx->SetBinContent(k , l , mtxpull_aU.at(k-1).at(l-1).GetMean() - pull_aU[k-1].GetMean() * pull_aU[l-1].GetMean() );
      covmtx->SetBinError  (k , l ,  0 );
      if(pull_aU[k-1].GetRMS() * pull_aU[l-1].GetRMS() != 0)
        cormtx->SetBinContent(k , l , (mtxpull_aU.at(k-1).at(l-1).GetMean() - pull_aU[k-1].GetMean() * pull_aU[l-1].GetMean()) / (pull_aU[k-1].GetRMS() * pull_aU[l-1].GetRMS()) );
      cormtx->SetBinError  (k , l ,  0 );
      //covmtx->SetBinError  (k , l ,  mtxpull_aU.at(k-1).at(l-1).GetRMS()  - pull_aU[k-1].GetRMS()  * pull_aU[l-1].GetRMS() );
    
    }
  }
  
  //Making plots to check all the errors:
   
  TH1F* error_diff_data_bU_aU = (TH1F*) toUnfold->Clone("error_diff_data_bU_aU");
  error_diff_data_bU_aU->Reset();
  
  for(int i=1;i<=toUnfold->GetNbinsX();++i){
     
    
    double error = 0;
         
    
    if(toUnfold->GetBinError(i)!=0) error = (toUnfold->GetBinError(i) - out->GetBinError(i))/toUnfold->GetBinError(i);
    if(printErrors)cout << "relative err data_bU_aU: " <<"  "<<error << " == " << toUnfold->GetBinError(i) << " - " <<out->GetBinError(i) << "/first" <<endl;
    error_diff_data_bU_aU->SetBinContent(i,error);
      
  }
  
  error_diff_data_bU_aU->Write();
  nch_resampled_profile->Write();
  hmtxpull_aU->Write();
  covmtx->Write();
  out->Write();
  cormtx->Write();
  
  cout<<"mtx "<< out->GetBinError(31) << endl;
  
  delete error_diff_data_bU_aU;
  delete nch_resampled_profile;
  delete Gauss;
  delete hmtxpull_aU;
  delete covmtx;     
  delete cormtx;     

  return *out;
}

