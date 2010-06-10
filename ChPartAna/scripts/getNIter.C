int getNIter(int icut , double E){
  int niter = 4;
  
  if(E==0.9){
    if(icut==0)  niter = 7;
    if(icut==1)  niter = 7;
    if(icut==2)  niter = 7;
    if(icut==3)  niter = 7;
    if(icut==4)  niter = 4;
    if(icut==5)  niter = 4;
    if(icut==6)  niter = 4;
    if(icut==7)  niter = 7;
    if(icut==8)  niter = 4;
    if(icut==9)  niter = 4;
    if(icut==10) niter = 2;
    if(icut==11) niter = 2;
    if(icut==12) niter = 5;
    if(icut==13) niter = 2;
    if(icut==14) niter = 2;
    if(icut==15) niter = 2;
    if(icut==16) niter = 2;
    if(icut==17) niter = 2;
    if(icut==18) niter = 2;
    if(icut==19) niter = 2;
    if(icut==20) niter = 2;
    if(icut==21) niter = 2;
    if(icut==22) niter = 2;
    if(icut==23) niter = 2;
    if(icut==24) niter = 2;
    
    /*if(icut==25) niter = 3;
    if(icut==26) niter = 3;
    if(icut==27) niter = 19;*/
  }
  else if(E==2.36){
    if(icut==0)  niter = 2;
    else if(icut == 23) niter = 3;
    else niter =2;
  }
  else if(E==7.0){
    if(icut==0)  niter = 5;
    if(icut==1)  niter = 5;
    if(icut==2)  niter = 5;
    if(icut==3)  niter = 5;
    if(icut==4)  niter = 5;
    if(icut==5)  niter = 5;
    if(icut==6)  niter = 5;
    if(icut==7)  niter = 5;
    if(icut==8)  niter = 3;
    if(icut==9)  niter = 2;
    if(icut==10) niter = 5;
    if(icut==11) niter = 5;
    if(icut==12) niter = 5;
    if(icut==13) niter = 5;
    if(icut==14) niter = 5;
    if(icut==15) niter = 4;
    if(icut==16) niter = 4;
    if(icut==17) niter = 4;
    if(icut==18) niter = 2;
    if(icut==19) niter = 2;
    if(icut==20) niter = 2;
    if(icut==21) niter = 2;
    if(icut==22) niter = 2;
    if(icut==23) niter = 2;
    if(icut==24) niter = 2;


  } 
  return niter;
}
