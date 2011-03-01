double ptcorr_val = 0.;
bool doptcorr = false;

void increaseNTracks(TH1F* nch_corrected , double syst_val , bool printSummary = false){
  
  int syst_sign=1;
  
  TH1F* nch_tmp = (TH1F*) nch_corrected->Clone("nch_tmp");
  nch_corrected->Reset();
  //nch_corrected->Sumw2();
  double start = 0 ;

  for(double k = start;k <= nch_tmp->GetXaxis()->GetXmax()-0.5 ; ++k){
    int bin = nch_tmp->GetXaxis()->FindFixBin(k);
    double binc = nch_tmp->GetBinContent(bin) / nch_tmp->GetBinWidth(bin);
    double addbinc = double(binc) ;//* syst_val / 100.;
    double addntr = k * syst_val / 100.;
    
    double lowerbin = 0 , upperbin = 0;
    lowerbin = ( k + syst_sign * int(addntr) < k + syst_sign * int(addntr) + syst_sign )? k + syst_sign * int(addntr) : k + syst_sign * int(addntr) + syst_sign ;
    upperbin = ( k + syst_sign * int(addntr) > k + syst_sign * int(addntr) + syst_sign )? k + syst_sign * int(addntr) : k + syst_sign * int(addntr) + syst_sign ;
    
    if(syst_sign==1){
      nch_corrected->Fill((int)lowerbin , addbinc * (1 - ( addntr - int(addntr))) );
      nch_corrected->Fill((int)upperbin , addbinc * ( addntr - int(addntr)) );
    }
  }
  
  for(int i = 1;i<=nch_corrected->GetNbinsX();++i)
    nch_corrected->SetBinError(i,nch_tmp->GetBinError(i));
  nch_corrected->SetEntries(nch_tmp->GetEntries());

  if(printSummary){

    cout<<"++ NTR INCREASE #" << " ++"<<endl;
    cout<<"   ===> The #tr per event has been shifted by "<<syst_sign * syst_val<<"% for "<<100.<<"% of the events"<<endl;
    cout<<"   ===> Mean (before,after) : ( "<<nch_tmp->GetMean()<<" , "<<nch_corrected->GetMean()<<" ) = "
      << (nch_corrected->GetMean() - nch_tmp->GetMean()) *100. / nch_tmp->GetMean()<< "%"<<endl;
    cout<<"   ===> #tracks (before,after) : ( "<<nch_tmp->GetMean()*nch_tmp->Integral()<<" , "<<nch_corrected->GetMean()*nch_corrected->Integral()<<" )"
        <<"   Diff : "<<nch_corrected->GetMean()*nch_corrected->Integral()-nch_tmp->GetMean()*nch_tmp->Integral() << " = "
        << (nch_corrected->GetMean()*nch_corrected->Integral()-nch_tmp->GetMean()*nch_tmp->Integral() )/(nch_tmp->GetMean()*nch_tmp->Integral())*100.<<endl;
    cout<<"   ===> nevts (before,after) : ( "<<nch_tmp->Integral()<<" , "<<nch_corrected->Integral()<<" )"<<endl;

    /*for(int i=1;i<=nch_INC->GetNbinsX();++i)
      if(nch_tmp->GetBinContent(i)!=nch_INC->GetBinContent(i))
        cout<<"bin "<<i<<" diff "<<nch_tmp->GetBinContent(i)-nch_INC->GetBinContent(i)<<endl;
     */ 
      
    cout<<"++ END OF NTR INCREASE ++"<<endl;
  }
  
}
