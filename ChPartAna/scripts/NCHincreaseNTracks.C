void increaseNTracks(TH1F* nch_raw_input , int syst_sign, double syst_val , bool printSummary = false){
  if(syst_sign==0){
    cout<<"!! WARNING : your systematic sign should not be 0 ! Please select 1 or -1 ..."<<endl<<"Exiting now !"<<endl;
    return;
  }

  TH1F* nch_tmp = (TH1F*) nch_raw_input->Clone("nch_tmp");
  nch_raw_input->Reset();
  //nch_raw_input->Sumw2();
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
      nch_raw_input->Fill((int)lowerbin , addbinc * (1 - ( addntr - int(addntr))) );
      nch_raw_input->Fill((int)upperbin , addbinc * ( addntr - int(addntr)) );
    }
    if(syst_sign==-1){
      nch_raw_input->Fill((int)upperbin , addbinc * (1 - ( addntr - int(addntr))) );
      nch_raw_input->Fill((int)lowerbin , addbinc * ( addntr - int(addntr)) );
    }
  }
  
  for(int i = 1;i<=nch_raw_input->GetNbinsX();++i)
    nch_raw_input->SetBinError(i,nch_tmp->GetBinError(i));
  nch_raw_input->SetEntries(nch_tmp->GetEntries());

  if(printSummary){

    cout<<"++ NTR INCREASE #" << " ++"<<endl;
    cout<<"   ===> The #tr per event has been shifted by "<<syst_sign * syst_val<<"% for "<<100.<<"% of the events"<<endl;
    cout<<"   ===> Mean (before,after) : ( "<<nch_tmp->GetMean()<<" , "<<nch_raw_input->GetMean()<<" ) = "
      << (nch_raw_input->GetMean() - nch_tmp->GetMean()) *100. / nch_tmp->GetMean()<< "%"<<endl;
    cout<<"   ===> #tracks (before,after) : ( "<<nch_tmp->GetMean()*nch_tmp->Integral()<<" , "<<nch_raw_input->GetMean()*nch_raw_input->Integral()<<" )"
        <<"   Diff : "<<nch_raw_input->GetMean()*nch_raw_input->Integral()-nch_tmp->GetMean()*nch_tmp->Integral() << " = "
        << (nch_raw_input->GetMean()*nch_raw_input->Integral()-nch_tmp->GetMean()*nch_tmp->Integral() )/(nch_tmp->GetMean()*nch_tmp->Integral())*100.<<endl;
    cout<<"   ===> nevts (before,after) : ( "<<nch_tmp->Integral()<<" , "<<nch_raw_input->Integral()<<" )"<<endl;

    /*for(int i=1;i<=nch_REC->GetNbinsX();++i)
      if(nch_tmp->GetBinContent(i)!=nch_REC->GetBinContent(i))
        cout<<"bin "<<i<<" diff "<<nch_tmp->GetBinContent(i)-nch_REC->GetBinContent(i)<<endl;
     */ 
      
    delete nch_tmp;  
    cout<<"++ END OF NTR INCREASE ++"<<endl;
  }
  
}
