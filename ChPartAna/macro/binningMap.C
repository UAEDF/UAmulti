vector<double> makeBins(const Int_t nbin , Double_t xmin , Double_t xmax){
  vector<double> array(nbin+1,0);
  Double_t step = fabs( xmax - xmin ) / double (nbin);
  for(int i=0;i<=nbin;++i)
    array[i] = xmin + i * step;
    return array;
}

void changeBins(int ibinmin , int ibinmax , double step , vector<double>& array){
  for(int i=0;i<=(ibinmax-ibinmin);++i)
    array[ibinmin+i] = array[ibinmin] + i * step;
 // return array;
}

void addBins(const int nbinnew , double step , vector<double>& array){
  int nbinold = array.size()-1;
  for(int i=1;i<=nbinnew;++i)
    array.push_back( array[nbinold] + i * step );
 // return array;
}

vector< vector<double> > getBins(int inch , int ipt , int ieta){
  vector< vector<double> > bins;
  
  //---------------- NCH BINNING ---------------
  if(inch==0){
    bins.push_back(makeBins(111,-0.5,110.5));
  }
  else if(inch==1){
    vector<double> tmp = makeBins(63,-0.5,62.5);
    addBins(3,2,tmp);
    addBins(10,5,tmp);
    bins.push_back(tmp);
  }
  
  //---------------- PT BINNING ---------------
  if(ipt==0){
    bins.push_back(makeBins(30,0.,3.));
  }
  
  //---------------- ETA BINNING ---------------
  if(ieta==0){
    bins.push_back(makeBins(60,-3.,3.));
  }
  else if(ieta==1){
    bins.push_back(makeBins(20,-2.4,2.4));
  }
  else if(ieta==2){
    bins.push_back(makeBins(12,-3.,3.));
  }
  
  return bins;
}

vector< vector<double> > getBins(int icut , double E = 0.9){
  vector< vector<double> > bins;
  
  int start = 112;
  /*int step = 4;
  if( double(icut) % 5 == 0 )
    start = 38;
  if( double(icut) % 5 == 0 )
    start = 61;
  
  start-=(icut - (icut % 5)) * step;
  if((icut - (icut % 5))>2)
    start-= 4 * ( (icut - (icut % 5)) - 2);*/
    
    
if(E==0.9){  
  if(icut==0)  start = 38;
  if(icut==1)  start = 35;
  if(icut==2)  start = 29;
  if(icut==3)  start = 22;
  if(icut==4)  start = 14;
  if(icut==5)  start = 60;
  if(icut==6)  start = 55;
  if(icut==7)  start = 45;
  if(icut==8)  start = 34;
  if(icut==9)  start = 20;
  if(icut==10) start = 58;
  if(icut==11) start = 51;
  if(icut==12) start = 43;
  if(icut==13) start = 31;
  if(icut==14) start = 18;
  if(icut==15) start = 32;
  if(icut==16) start = 31;
  if(icut==17) start = 25;
  if(icut==18) start = 19;
  if(icut==19) start = 12;
  if(icut==20) start = 16;
  if(icut==21) start = 14;
  if(icut==22) start = 12;
  if(icut==23) start = 7;
  if(icut==24) start = 5;
  if(icut==25) start = 33;
  if(icut==26) start = 19;
  if(icut==27) start = 19;
}    
    
  vector<double> tmp = makeBins(start,-0.5,start-0.5);
  addBins(1,2,tmp);
  addBins(1,3,tmp);
  addBins(6,10,tmp);
  bins.push_back(tmp);
  
  //adding default pt and eta bins 
  bins.push_back(makeBins(30,0.,3.));
  bins.push_back(makeBins(20,-2.4,2.4));
  return bins;
}
