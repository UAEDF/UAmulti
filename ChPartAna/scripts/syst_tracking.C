//if no syst, keep as before
if(syst == 0 || int(syst/100)!=0)
  syst_val = 0;

  
if(! (syst == 0 || int(syst/100)!=0)){
  if(int(syst/100)==0 && syst!=0 && syst_sign==0){
    cout<<"!! WARNING : your systematic sign should not be 0 ! Please select 1 or -1 ..."<<endl<<"Exiting now !"<<endl;
    return;
  }

  if(syst == 1)
    syst_val = 0.5;
  if(syst == 2)
    syst_val = 1;
  if(syst == 3)
    syst_val = 1.5;
  if(syst == 4)
    syst_val = 3;
  if(syst == 5)
    syst_val = 5;
  if(syst == 6)
    syst_val = 10;
  
  TH1F* nch_tmp = (TH1F*) nch_INC->Clone("nch_tmp");
  nch_INC->Reset();
  nch_INC->Sumw2();
  double start = 0 ;
  if(syst_sign==-1) start = 1;
  else if(syst_sign==1) start = 0;

  if(syst_sign==-1) nch_INC->Fill(0.,nch_tmp->GetBinContent(1));

  for(double k = start;k <= nch_tmp->GetXaxis()->GetXmax()-0.5 ; ++k){
    int bin = nch_tmp->GetXaxis()->FindFixBin(k);
    double binc = nch_tmp->GetBinContent(bin) / nch_tmp->GetBinWidth(bin);
    double addbinc = double(binc) ;//* syst_val / 100.;
    double addntr = k * syst_val / 100.;
    /*if( gRandom->Uniform() > ( addntr - (addntr % 1)) )
      addntr = k + (addntr % 1);
    else
      addntr =  k + (addntr % 1) + 1;*/

    //nch_INC->Fill(k,binc-addbinc); //NEEDED IF CHANGE ONLY FOR FRAC EVT
  
    /*for(int nbin = 1;nbin<= int(binc-addbinc);++nbin)
      nch_INC->Fill(k);
    if((binc-addbinc)-int(binc-addbinc)!=0)
      nch_INC->Fill(k,(binc-addbinc)-int(binc-addbinc));*/
    
    double lowerbin = 0 , upperbin = 0;
    lowerbin = ( k + syst_sign * int(addntr) < k + syst_sign * int(addntr) + syst_sign )? k + syst_sign * int(addntr) : k + syst_sign * int(addntr) + syst_sign ;
    upperbin = ( k + syst_sign * int(addntr) > k + syst_sign * int(addntr) + syst_sign )? k + syst_sign * int(addntr) : k + syst_sign * int(addntr) + syst_sign ;
    
    if(syst_sign==1){
      nch_INC->Fill((int)lowerbin , addbinc * (1 - ( addntr - int(addntr))) );
      nch_INC->Fill((int)upperbin , addbinc * ( addntr - int(addntr)) );
    }
    else if(syst_sign==-1){
      nch_INC->Fill((int)upperbin , addbinc * (1 - ( addntr - int(addntr))) );
      nch_INC->Fill((int)lowerbin , addbinc * ( addntr - int(addntr)) );
    }
    
    
    if(debug_==100){
      cout<<"Filling : "<<k<<" , "<<binc<<" , "<<syst_val<<" , "<<addbinc<<" , "<<addntr<<endl;
      cout<<"( "<<k<<" , "<<binc-addbinc<<" )"<<endl;
      if(syst_sign==1){
        cout<<"( "<<lowerbin<<" , "<<addbinc * (1 - ( addntr - int(addntr)))<<" )"<<endl;
        cout<<"( "<<upperbin<<" , "<<addbinc * ( addntr - int(addntr))<<" )"<<endl;
      }
      if(syst_sign==-1){
        cout<<"( "<<upperbin<<" , "<<addbinc * (1 - ( addntr - int(addntr)))<<" )"<<endl;
        cout<<"( "<<lowerbin<<" , "<<addbinc * ( addntr - int(addntr))<<" )"<<endl;
      }
    }
  
  }

  for(int i = 1;i<=nch_INC->GetNbinsX();++i)
    nch_INC->SetBinError(i,sqrt(nch_INC->GetBinContent(i)));
  nch_INC->SetEntries(nch_tmp->GetEntries());

  if(/*syst == 0 ||*/ int(syst/100)==0){
    cout<<"++ SYSTEMATIC #"<<syst<<" ++"<<endl;
    cout<<"   ===> The #tr per event has been shifted by "<<syst_sign * syst_val<<"% for "<<100.<<"% of the events"<<endl;
    cout<<"   ===> Mean (before,after) : ( "<<nch_tmp->GetMean()<<" , "<<nch_INC->GetMean()<<" ) = "
      << (nch_INC->GetMean() - nch_tmp->GetMean()) *100. / nch_tmp->GetMean()<< "%"<<endl;
    cout<<"   ===> #tracks (before,after) : ( "<<nch_tmp->GetMean()*nch_tmp->Integral()<<" , "<<nch_INC->GetMean()*nch_INC->Integral()<<" )"
        <<"   Diff : "<<nch_INC->GetMean()*nch_INC->Integral()-nch_tmp->GetMean()*nch_tmp->Integral() << " = "
        << (nch_INC->GetMean()*nch_INC->Integral()-nch_tmp->GetMean()*nch_tmp->Integral() )/(nch_tmp->GetMean()*nch_tmp->Integral())*100.<<endl;
    cout<<"   ===> nevts (before,after) : ( "<<nch_tmp->Integral()<<" , "<<nch_INC->Integral()<<" )"<<endl;

    /*for(int i=1;i<=nch_INC->GetNbinsX();++i)
      if(nch_tmp->GetBinContent(i)!=nch_INC->GetBinContent(i))
        cout<<"bin "<<i<<" diff "<<nch_tmp->GetBinContent(i)-nch_INC->GetBinContent(i)<<endl;
     */ 
      
    cout<<"++ END OF SYSTEMATIC ++"<<endl;
  }
}

