currentdir = gDirectory;
  
if(syst!=100)
  syst_val = 0.;
  
if(int(syst/100)==1 && syst!=0 && syst_sign==0){
  cout<<"!! WARNING : your systematic sign should not be 0 ! Please select 1 or -1 ..."<<endl<<"Exiting now !"<<endl;
  return;
}

//ratio SD phojet frac/ current frac (here MCD6T)
if(syst == 100 && E==0.9)
  syst_val = fabs( 1 - 4.92 / (frac_SD * 100.) );
  
if(syst == 100 && E==2.36)
  syst_val = fabs( 1 - 5.0 / (frac_SD * 100.) );
  
if(syst==100){
  cout<<"++ SYSTEMATIC #"<<syst<<" ++"<<endl;
  cout<<"   ===> The SD distri has been scaled by a factor "<<1 + syst_sign * syst_val<<endl;
      
  cout<<"++ END OF SYSTEMATIC ++"<<endl;

  nch_evtSel_SD->Scale(1 + syst_sign * syst_val);

}

if(syst==110 || syst==111 || syst==112 || syst==113){
  
  TString mcfiletmp = fileManager(2,syst%100,Emc,iTr,0,0,"");
cout<<"tttttt33333333"<<endl;
  //TFile* mctmp = TFile::Open(mcfiletmp,"READ");
  TFile* mctmp = new TFile(mcfiletmp,"READ");
cout<<"tttttt33333333"<<endl;
  if(mctmp==0){
    cout<<"!! WARNING : the following file is not present. You nead to create it. Exiting now !"<<endl;
    cout<<mcfiletmp<<endl;
    return;
  }
  cout<<"tttttt"<<endl;
  nch_evtSel_SD =  (TH1F*) mctmp->Get(dir+st("GenMultiPlots_evtSel_reco",acc)+st("/MultiPlots_SD_evtSel_reco",acc)+st("/nch_SD_evtSel_reco",acc));
  nch_evtSel_SD->Scale(MC_factor);
  
  cout<<"++ SYSTEMATIC #"<<syst<<" ++"<<endl;
  cout<<"   ===> The SD distri is now with the tune "<<syst%400<<endl;
  cout<<"   ===> taken from file "<<mcfiletmp<<endl;
  cout<<"++ END OF SYSTEMATIC ++"<<endl;
}

currentdir->cd();
  
