if(int(syst/100)!=2)
  syst_val = 0.;


if(int(syst/100)==2 && syst!=0 && syst_sign==0){
  cout<<"!! WARNING : your systematic sign should not be 0 ! Please select 1 or -1 ..."<<endl<<"Exiting now !"<<endl;
  return;
}

if(syst == 200)
  syst_val = 0.05;
  
if(syst == 201)
  syst_val = 0.1;
  
if(int(syst/100)==2 ){
  cout<<"++ SYSTEMATIC #"<<syst<<" ++"<<endl;
  cout<<"   ===> The evtSel Eff distri is going to be scaled by a factor "<<1. + syst_sign * syst_val<<endl;
      
  cout<<"++ END OF SYSTEMATIC ++"<<endl;
}

eff_evtSel->Scale(1. + syst_sign * syst_val);


