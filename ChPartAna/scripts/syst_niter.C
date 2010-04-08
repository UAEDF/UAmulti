if(int(syst/100)!=3)
  syst_val = 0.;
  
if(int(syst/100)==3 && syst_sign==0){
  cout<<"!! WARNING : your systematic sign should not be 0 ! Please select 1 or -1 ..."<<endl<<"Exiting now !"<<endl;
  return;
}

if(int(syst/100)==3)
  syst_val = syst%300;

  
if(int(syst/100)==3){
  cout<<"++ SYSTEMATIC #"<<syst<<" ++"<<endl;
  cout<<"   ===> The number of iterations is going to be increased by "<< syst_sign * syst_val <<endl;
      
  cout<<"++ END OF SYSTEMATIC ++"<<endl;
}

niter+= syst_sign * int(syst_val);


