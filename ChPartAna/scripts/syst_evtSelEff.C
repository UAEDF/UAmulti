if(int(syst/100)!=2)
  syst_val = 0.;


if(int(syst/100)==2 && syst!=0 && syst_sign==0){
  cout<<"!! WARNING : your systematic sign should not be 0 ! Please select 1 or -1 ..."<<endl<<"Exiting now !"<<endl;
  return;
}

if(syst == 250)
  syst_val = 0.05;
  
if(syst == 251)
  syst_val = 0.1;
  
if(syst==250 || syst==251 ){
  cout<<"++ SYSTEMATIC #"<<syst<<" ++"<<endl;
  cout<<"   ===> The evtSel Eff distri is going to be scaled by a factor "<<1. + syst_sign * syst_val<<endl;
      
  cout<<"++ END OF SYSTEMATIC ++"<<endl;

  eff_evtSel->Scale(1. + syst_sign * syst_val);
}

if(syst==200){

  double maxerr = 0.04;
  if(syst_sign<0) maxerr = 0.06;
  double minerr = 0.01;

  int firstbin = 1;
  int lastbin  = 2;
  for(; lastbin <= eff_evtSel->GetNbinsX() - 1  ; ++lastbin){
    if(eff_evtSel->GetBinContent(lastbin)==1 && (eff_evtSel->GetBinContent(lastbin+1)==1 || eff_evtSel->GetBinContent(lastbin+1)==0 )){
      break;
    }
  }
  lastbin+=5;
  
  for(int ibin = 1 ; ibin <= eff_evtSel->GetNbinsX() ; ++ibin){
    double val = eff_evtSel->GetBinContent(ibin) + syst_sign * ( maxerr - (double(ibin)-double(firstbin))/(double(lastbin)-double(firstbin)) * maxerr + minerr);
    if(ibin>=lastbin) val = eff_evtSel->GetBinContent(ibin) + syst_sign * minerr;
    if(val<=0) val = 2./3. * eff_evtSel->GetBinContent(ibin);
    if(ibin>=lastbin && eff_evtSel->GetBinContent(ibin)==0) val=1. + syst_sign * minerr;
    if(val>1) val = 1.;
    cout<<eff_evtSel->GetBinContent(ibin)<<"  "<<val<<"  "<<(val-eff_evtSel->GetBinContent(ibin))<<endl;
    eff_evtSel->SetBinContent(ibin,val);
  }


  cout<<"++ SYSTEMATIC #"<<syst<<" ++"<<endl;
  cout<<"   ===> The evtSel Eff distri is going to be scaled by an decreasing factor of"<<syst_sign * syst_val<<endl;
      
  cout<<"++ END OF SYSTEMATIC ++"<<endl;

}
