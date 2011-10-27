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
  vector<double> tmp;
  
  //---------------- NCH BINNING ---------------
  if(inch==0){
    bins.push_back(makeBins(171,-0.5,170.5));
  }
  else if(inch==1){
    tmp = makeBins(63,-0.5,62.5);
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
  
  
  // pt2
  tmp.clear();
  tmp = makeBins(200,0.,4.);
  addBins(80,.1,tmp);
  bins.push_back(tmp);

  // mt
  tmp.clear();
  tmp = makeBins(200,0.,2.);
  addBins(80,.1,tmp);
  bins.push_back(tmp);
  
  
  return bins;
}


//used with the output of getLowStatBinsv2() !!
vector< vector<double> > getBins(int icut , double Ebinning = 0.9){
  vector< vector<double> > bins;
  vector<double> tmp = makeBins(371,-0.5,370.5);
  
//--------------------------------------------------
//-------------     All bins 1     -----------------
//--------------------------------------------------
if(Ebinning==1)
  tmp = makeBins(371,-0.5,370.5);




  //!!!!!!!!!!!!! HERE USED MBTRACKS FOR BINNING
  
//--------------------------------------------------
//-------------      0.9 TeV      ------------------
//--------------------------------------------------

if(Ebinning==0.9)
{
  if(icut==0){
    tmp = makeBins(74,-0.5,73.5);
    addBins(1,2,tmp);
    addBins(1,3,tmp);
    addBins(1,4,tmp);
    addBins(1,10,tmp);
    addBins(21,10,tmp);
  }
  if(icut==1){
    tmp = makeBins(74,-0.5,73.5);
    addBins(1,2,tmp);
    addBins(1,3,tmp);
    addBins(1,4,tmp);
    addBins(1,10,tmp);
    addBins(21,10,tmp);
  }
  if(icut==2){
    tmp = makeBins(39,-0.5,38.5);
    addBins(1,2,tmp);
    addBins(1,2,tmp);
    addBins(1,10,tmp);
    addBins(25,10,tmp);
  }
  if(icut==3){
    tmp = makeBins(19,-0.5,18.5);
    addBins(1,2,tmp);
    addBins(56,5,tmp);
  }
  if(icut==4){
    tmp = makeBins(41,-0.5,40.5);
    addBins(1,2,tmp);
    addBins(1,3,tmp);
    addBins(51,5,tmp);
  }
  if(icut==5){
    tmp = makeBins(41,-0.5,40.5);
    addBins(1,2,tmp);
    addBins(1,3,tmp);
    addBins(51,5,tmp);
  }

}


//--------------------------------------------------
//-------------      2.76  TeV      -----------------
//--------------------------------------------------  
if(Ebinning==2.76)
{
  if(icut==0){
    tmp = makeBins(124,-0.5,123.5);
    addBins(1,2,tmp);
    addBins(1,2,tmp);
    addBins(1,3,tmp);
    addBins(1,5,tmp);
    addBins(1,8,tmp);
    addBins(20,8,tmp);
  }
  if(icut==1){
    tmp = makeBins(124,-0.5,123.5);
    addBins(1,2,tmp);
    addBins(1,2,tmp);
    addBins(1,3,tmp);
    addBins(1,5,tmp);
    addBins(1,8,tmp);
    addBins(20,8,tmp);
  }
  if(icut==2){
    tmp = makeBins(75,-0.5,74.5);
    addBins(1,2,tmp);
    addBins(1,3,tmp);
    addBins(1,5,tmp);
    addBins(43,5,tmp);
  }
  if(icut==3){
    tmp = makeBins(39,-0.5,38.5);
    addBins(1,2,tmp);
    addBins(1,3,tmp);
    addBins(52,5,tmp);
  }
  if(icut==4){
    tmp = makeBins(65,-0.5,64.5);
    addBins(1,3,tmp);
    addBins(1,3,tmp);
    addBins(46,5,tmp);
  }
  if(icut==5){
    tmp = makeBins(65,-0.5,64.5);
    addBins(1,3,tmp);
    addBins(1,3,tmp);
    addBins(46,5,tmp);
  }
}   
  
//--------------------------------------------------
//-------------      7.0  TeV      -----------------
//--------------------------------------------------
if(Ebinning==7.0 ){

  if(icut==0){
    tmp = makeBins(98,-0.5,97.5);
    addBins(1,2,tmp);
    addBins(1,2,tmp);
    addBins(1,2,tmp);
    addBins(1,2,tmp);
    addBins(1,2,tmp);
    addBins(1,2,tmp);
    addBins(1,3,tmp);
    addBins(1,4,tmp);
    addBins(1,5,tmp);
    addBins(1,9,tmp);
    addBins(19,9,tmp);
  }
  if(icut==1){
    tmp = makeBins(98,-0.5,97.5);
    addBins(1,2,tmp);
    addBins(1,2,tmp);
    addBins(1,2,tmp);
    addBins(1,2,tmp);
    addBins(1,2,tmp);
    addBins(1,2,tmp);
    addBins(1,3,tmp);
    addBins(1,4,tmp);
    addBins(1,5,tmp);
    addBins(1,9,tmp);
    addBins(19,9,tmp);
  }
  if(icut==2){
    tmp = makeBins(57,-0.5,56.5);
    addBins(1,2,tmp);
    addBins(1,2,tmp);
    addBins(1,2,tmp);
    addBins(1,3,tmp);
    addBins(1,6,tmp);
    addBins(38,6,tmp);
  }
  if(icut==3){
    tmp = makeBins(29,-0.5,28.5);
    addBins(1,2,tmp);
    addBins(1,2,tmp);
    addBins(1,4,tmp);
    addBins(53,5,tmp);
  }
  if(icut==4){
    tmp = makeBins(52,-0.5,51.5);
    addBins(1,2,tmp);
    addBins(1,2,tmp);
    addBins(1,2,tmp);
    addBins(1,3,tmp);
    addBins(1,8,tmp);
    addBins(29,8,tmp);
  }
  if(icut==5){
    tmp = makeBins(52,-0.5,51.5);
    addBins(1,2,tmp);
    addBins(1,2,tmp);
    addBins(1,2,tmp);
    addBins(1,3,tmp);
    addBins(1,8,tmp);
    addBins(29,8,tmp);
  }

} 

  bins.push_back(tmp);
  
  //adding default pt 
  // bins.push_back(makeBins(30,0.,3.));
  tmp.clear();
  tmp = makeBins(200,0.,4.);
//  addBins(80,.1,tmp);
  bins.push_back(tmp);

  // eta bins
  bins.push_back(makeBins(24,-2.4,2.4));

  // pt
  tmp.clear();
  tmp = makeBins(200,0.,4.);
  addBins(80,.1,tmp);
  bins.push_back(tmp);

  // mt
  tmp.clear();
  tmp = makeBins(200,0.,2.);
  addBins(80,.1,tmp);
  bins.push_back(tmp);

  return bins;
}
