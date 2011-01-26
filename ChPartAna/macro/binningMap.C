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
    bins.push_back(makeBins(171,-0.5,170.5));
  }
  else if(inch==1){
    vector<double> tmp = makeBins(63,-0.5,62.5);
    addBins(3,2,tmp);
    addBins(10,5,tmp);
    bins.push_back(tmp);
  }
  if(inch==2){
    bins.push_back(makeBins(271,-0.5,270.5));
  }
  
  //---------------- PT BINNING ---------------
  if(ipt==0){
    bins.push_back(makeBins(30,0.,3.));
  }
  else if(ipt == 1){
    vector<double> tmp = makeBins(200,0.,1.5);
    addBins(20,.05,tmp);
    addBins(10,.1,tmp);
    addBins(5,.2,tmp);
    addBins(15,1,tmp);
    bins.push_back(tmp);
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
  else if(ieta==3){
    bins.push_back(makeBins(75,-2.5,2.5));
  }
  
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
    tmp = makeBins(40,-0.5,39.5);
    addBins(1,2,tmp);
    addBins(1,2,tmp);
    addBins(1,5,tmp);
    addBins(67,5,tmp);
  }
  if(icut==1){
    tmp = makeBins(36,-0.5,35.5);
    addBins(1,2,tmp);
    addBins(1,3,tmp);
    addBins(1,9,tmp);
    addBins(35,9,tmp);
  }
  if(icut==2){
    tmp = makeBins(29,-0.5,28.5);
    addBins(1,2,tmp);
    addBins(1,2,tmp);
    addBins(1,7,tmp);
    addBins(45,7,tmp);
  }
  if(icut==3){
    tmp = makeBins(24,-0.5,23.5);
    addBins(1,3,tmp);
    addBins(71,5,tmp);
  }
  if(icut==4){
    tmp = makeBins(15,-0.5,14.5);
    addBins(1,2,tmp);
    addBins(73,5,tmp);
  }
  if(icut==5 || icut==35 || icut==40){
    tmp = makeBins(62,-0.5,61.5);
    addBins(1,2,tmp);
    addBins(1,2,tmp);
    addBins(1,4,tmp);
    addBins(1,10,tmp);
    addBins(30,10,tmp);
  }
  if(icut==6 || icut==30 || icut==31 || icut==32 || icut==33 || icut==34 || icut==36 || icut==41 ||icut==42){
    tmp = makeBins(57,-0.5,56.5);
    addBins(1,2,tmp);
    addBins(1,3,tmp);
    addBins(1,10,tmp);
    addBins(31,10,tmp);
  }
  if(icut==7 || icut==37){
    tmp = makeBins(46,-0.5,45.5);
    addBins(1,2,tmp);
    addBins(1,3,tmp);
    addBins(66,5,tmp);
  }
  if(icut==8 || icut==38){
    tmp = makeBins(35,-0.5,34.5);
    addBins(1,3,tmp);
    addBins(69,5,tmp);
  }
  if(icut==9 || icut==39){
    tmp = makeBins(21,-0.5,20.5);
    addBins(1,3,tmp);
    addBins(72,5,tmp);
  }
  if(icut==10){
    tmp = makeBins(58,-0.5,57.5);
    addBins(1,2,tmp);
    addBins(1,3,tmp);
    addBins(1,6,tmp);
    addBins(49,6,tmp);
  }
  if(icut==11){
    tmp = makeBins(52,-0.5,51.5);
    addBins(1,2,tmp);
    addBins(1,3,tmp);
    addBins(1,8,tmp);
    addBins(40,8,tmp);
  }
  if(icut==12){
    tmp = makeBins(43,-0.5,42.5);
    addBins(1,2,tmp);
    addBins(1,5,tmp);
    addBins(66,5,tmp);
  }
  if(icut==13){
    tmp = makeBins(32,-0.5,31.5);
    addBins(1,2,tmp);
    addBins(1,4,tmp);
    addBins(69,5,tmp);
  }
  if(icut==14){
    tmp = makeBins(19,-0.5,18.5);
    addBins(1,2,tmp);
    addBins(72,5,tmp);
  }
  if(icut==15){
    tmp = makeBins(33,-0.5,32.5);
    addBins(1,2,tmp);
    addBins(1,3,tmp);
    addBins(69,5,tmp);
  }
  if(icut==16){
    tmp = makeBins(31,-0.5,30.5);
    addBins(1,2,tmp);
    addBins(1,4,tmp);
    addBins(69,5,tmp);
  }
  if(icut==17){
    tmp = makeBins(26,-0.5,25.5);
    addBins(1,2,tmp);
    addBins(1,5,tmp);
    addBins(70,5,tmp);
  }
  if(icut==18){
    tmp = makeBins(20,-0.5,19.5);
    addBins(1,2,tmp);
    addBins(1,4,tmp);
    addBins(71,5,tmp);
  }
  if(icut==19){
    tmp = makeBins(13,-0.5,12.5);
    addBins(1,2,tmp);
    addBins(73,5,tmp);
  }
  if(icut==20){
    tmp = makeBins(17,-0.5,16.5);
    addBins(1,4,tmp);
    addBins(72,5,tmp);
  }
  if(icut==21){
    tmp = makeBins(14,-0.5,13.5);
    addBins(1,2,tmp);
    addBins(1,7,tmp);
    addBins(53,7,tmp);
  }
  if(icut==22){
    tmp = makeBins(13,-0.5,12.5);
    addBins(1,2,tmp);
    addBins(73,5,tmp);
  }
  if(icut==23){
    tmp = makeBins(11,-0.5,10.5);
    addBins(74,5,tmp);
  }
  if(icut==24){
    tmp = makeBins(8,-0.5,7.5);
    addBins(75,5,tmp);
  }
  if(icut==25){
    tmp = makeBins(33,-0.5,32.5);
    addBins(1,2,tmp);
    addBins(1,3,tmp);
    addBins(1,6,tmp);
    addBins(53,6,tmp);
  }
  if(icut==26){
    tmp = makeBins(20,-0.5,19.5);
    addBins(1,3,tmp);
    addBins(72,5,tmp);
  }
  if(icut==27){
    tmp = makeBins(20,-0.5,19.5);
    addBins(1,3,tmp);
    addBins(72,5,tmp);
  }
}
  
  
//--------------------------------------------------
//-------------      2.36 TeV      -----------------
//--------------------------------------------------
if(Ebinning==2.36){
  if(icut==0){
    tmp = makeBins(37,-0.5,36.5);
    addBins(1,2,tmp);
    addBins(1,2,tmp);
    addBins(1,2,tmp);
    addBins(1,3,tmp);
    addBins(1,5,tmp);
    addBins(66,5,tmp);
  }
  if(icut==1){
    tmp = makeBins(34,-0.5,33.5);
    addBins(1,2,tmp);
    addBins(1,2,tmp);
    addBins(1,3,tmp);
    addBins(1,4,tmp);
    addBins(67,5,tmp);
  }
  if(icut==2){
    tmp = makeBins(28,-0.5,27.5);
    addBins(1,2,tmp);
    addBins(1,2,tmp);
    addBins(1,3,tmp);
    addBins(1,4,tmp);
    addBins(69,5,tmp);
  }
  if(icut==3){
    tmp = makeBins(23,-0.5,22.5);
    addBins(1,3,tmp);
    addBins(71,5,tmp);
  }
  if(icut==4){
    tmp = makeBins(14,-0.5,13.5);
    addBins(1,3,tmp);
    addBins(73,5,tmp);
  }
  if(icut==5 || icut==35 || icut==40){
    tmp = makeBins(54,-0.5,53.5);
    addBins(1,2,tmp);
    addBins(1,2,tmp);
    addBins(1,2,tmp);
    addBins(1,2,tmp);
    addBins(1,3,tmp);
    addBins(1,3,tmp);
    addBins(1,4,tmp);
    addBins(1,5,tmp);
    addBins(61,5,tmp);
  }
  if(icut==6 || icut==30 || icut==31 || icut==32 || icut==33 || icut==34 || icut==36 || icut==41 ||icut==42){
    tmp = makeBins(49,-0.5,48.5);
    addBins(1,2,tmp);
    addBins(1,2,tmp);
    addBins(1,2,tmp);
    addBins(1,4,tmp);
    addBins(1,4,tmp);
    addBins(1,4,tmp);
    addBins(1,8,tmp);
    addBins(34,8,tmp);
  }
  if(icut==7 || icut==37){
    tmp = makeBins(40,-0.5,39.5);
    addBins(1,2,tmp);
    addBins(1,2,tmp);
    addBins(1,2,tmp);
    addBins(1,3,tmp);
    addBins(1,4,tmp);
    addBins(1,10,tmp);
    addBins(32,10,tmp);
  }
  if(icut==8 || icut==38){
    tmp = makeBins(31,-0.5,30.5);
    addBins(1,2,tmp);
    addBins(1,2,tmp);
    addBins(1,3,tmp);
    addBins(1,6,tmp);
    addBins(53,6,tmp);
  }
  if(icut==9 || icut==39){
    tmp = makeBins(19,-0.5,18.5);
    addBins(1,2,tmp);
    addBins(1,4,tmp);
    addBins(71,5,tmp);
  }
  if(icut==10){
    tmp = makeBins(48,-0.5,47.5);
    addBins(1,2,tmp);
    addBins(1,2,tmp);
    addBins(1,2,tmp);
    addBins(1,2,tmp);
    addBins(1,2,tmp);
    addBins(1,3,tmp);
    addBins(1,3,tmp);
    addBins(1,3,tmp);
    addBins(1,4,tmp);
    addBins(1,7,tmp);
    addBins(45,7,tmp);
  }
  if(icut==11){
    tmp = makeBins(46,-0.5,45.5);
    addBins(1,2,tmp);
    addBins(1,2,tmp);
    addBins(1,2,tmp);
    addBins(1,2,tmp);
    addBins(1,2,tmp);
    addBins(1,3,tmp);
    addBins(1,5,tmp);
    addBins(64,5,tmp);
  }
  if(icut==12){
    tmp = makeBins(38,-0.5,37.5);
    addBins(1,2,tmp);
    addBins(1,2,tmp);
    addBins(1,2,tmp);
    addBins(1,3,tmp);
    addBins(1,4,tmp);
    addBins(66,5,tmp);
  }
  if(icut==13){
    tmp = makeBins(30,-0.5,29.5);
    addBins(1,2,tmp);
    addBins(1,3,tmp);
    addBins(1,5,tmp);
    addBins(68,5,tmp);
  }
  if(icut==14){
    tmp = makeBins(17,-0.5,16.5);
    addBins(1,2,tmp);
    addBins(1,3,tmp);
    addBins(72,5,tmp);
  }
  if(icut==15){
    tmp = makeBins(29,-0.5,28.5);
    addBins(1,2,tmp);
    addBins(1,2,tmp);
    addBins(1,2,tmp);
    addBins(1,2,tmp);
    addBins(1,3,tmp);
    addBins(1,5,tmp);
    addBins(68,5,tmp);
  }
  if(icut==16){
    tmp = makeBins(29,-0.5,28.5);
    addBins(1,2,tmp);
    addBins(1,2,tmp);
    addBins(1,3,tmp);
    addBins(1,5,tmp);
    addBins(68,5,tmp);
  }
  if(icut==17){
    tmp = makeBins(24,-0.5,23.5);
    addBins(1,2,tmp);
    addBins(1,3,tmp);
    addBins(1,4,tmp);
    addBins(70,5,tmp);
  }
  if(icut==18){
    tmp = makeBins(17,-0.5,16.5);
    addBins(1,2,tmp);
    addBins(1,2,tmp);
    addBins(1,2,tmp);
    addBins(72,5,tmp);
  }
  if(icut==19){
    tmp = makeBins(12,-0.5,11.5);
    addBins(1,2,tmp);
    addBins(74,5,tmp);
  }
  if(icut==20){
    tmp = makeBins(15,-0.5,14.5);
    addBins(1,2,tmp);
    addBins(1,3,tmp);
    addBins(72,5,tmp);
  }
  if(icut==21){
    tmp = makeBins(14,-0.5,13.5);
    addBins(1,2,tmp);
    addBins(1,5,tmp);
    addBins(72,5,tmp);
  }
  if(icut==22){
    tmp = makeBins(11,-0.5,10.5);
    addBins(1,2,tmp);
    addBins(1,3,tmp);
    addBins(73,5,tmp);
  }
  if(icut==23){
    tmp = makeBins(9,-0.5,8.5);
    addBins(1,3,tmp);
    addBins(74,5,tmp);
  }
  if(icut==24){
    tmp = makeBins(7,-0.5,6.5);
    addBins(75,5,tmp);
  }
  if(icut==25){
    tmp = makeBins(33,-0.5,32.5);
    addBins(1,2,tmp);
    addBins(1,3,tmp);
    addBins(1,4,tmp);
    addBins(1,9,tmp);
    addBins(35,9,tmp);
  }
  if(icut==26){
    tmp = makeBins(19,-0.5,18.5);
    addBins(1,2,tmp);
    addBins(1,3,tmp);
    addBins(72,5,tmp);
  }
  if(icut==27){
    tmp = makeBins(19,-0.5,18.5);
    addBins(1,2,tmp);
    addBins(1,3,tmp);
    addBins(72,5,tmp);
  }
}
  
  
//--------------------------------------------------
//-------------      7.0  TeV      -----------------
//--------------------------------------------------
if(Ebinning==7.0 ){
  if(icut==0){
    tmp = makeBins(80,-0.5,79.5);
    addBins(1,2,tmp);
    addBins(1,2,tmp);
    addBins(1,2,tmp);
    addBins(1,4,tmp);
    addBins(1,4,tmp);
    addBins(1,10,tmp);
    addBins(28,10,tmp);
  }
  if(icut==1){
    tmp = makeBins(71,-0.5,70.5);
    addBins(1,2,tmp);
    addBins(1,2,tmp);
    addBins(1,2,tmp);
    addBins(1,2,tmp);
    addBins(1,3,tmp);
    addBins(1,9,tmp);
    addBins(30,9,tmp);
  }
  if(icut==2){
    tmp = makeBins(59,-0.5,58.5);
    addBins(1,2,tmp);
    addBins(1,3,tmp);
    addBins(1,5,tmp);
    addBins(63,5,tmp);
  }
  if(icut==3){
    tmp = makeBins(43,-0.5,42.5);
    addBins(1,2,tmp);
    addBins(1,2,tmp);
    addBins(1,5,tmp);
    addBins(66,5,tmp);
  }
  if(icut==4){
    tmp = makeBins(27,-0.5,26.5);
    addBins(1,3,tmp);
    addBins(70,5,tmp);
  }
  if(icut==5 || icut==35 || icut==40){
    tmp = makeBins(114,-0.5,113.5);
    addBins(1,2,tmp);
    addBins(1,2,tmp);
    addBins(1,2,tmp);
    addBins(1,2,tmp);
    addBins(1,2,tmp);
    addBins(1,2,tmp);
    addBins(1,4,tmp);
    addBins(1,5,tmp);
    addBins(1,10,tmp);
    addBins(24,10,tmp);
  }
  if(icut==6 || icut==30 || icut==31 || icut==32 || icut==33 || icut==34 || icut==36 || icut==41 ||icut==42){
    tmp = makeBins(101,-0.5,100.5);
    addBins(1,2,tmp);
    addBins(1,2,tmp);
    addBins(1,2,tmp);
    addBins(1,2,tmp);
    addBins(1,2,tmp);
    addBins(1,2,tmp);
    addBins(1,6,tmp);
    addBins(1,10,tmp);
    addBins(26,10,tmp);
  }
  if(icut==7 || icut==37){
    tmp = makeBins(79,-0.5,78.5);
    addBins(1,2,tmp);
    addBins(1,2,tmp);
    addBins(1,2,tmp);
    addBins(1,2,tmp);
    addBins(1,3,tmp);
    addBins(1,4,tmp);
    addBins(1,6,tmp);
    addBins(44,6,tmp);
  }
  if(icut==8 || icut==38){
    tmp = makeBins(59,-0.5,58.5);
    addBins(1,2,tmp);
    addBins(1,2,tmp);
    addBins(1,4,tmp);
    addBins(1,5,tmp);
    addBins(62,5,tmp);
  }
  if(icut==9 || icut==39){
    tmp = makeBins(36,-0.5,35.5);
    addBins(1,2,tmp);
    addBins(1,5,tmp);
    addBins(68,5,tmp);
  }
  if(icut==10){
    tmp = makeBins(110,-0.5,109.5);
    addBins(1,2,tmp);
    addBins(1,2,tmp);
    addBins(1,2,tmp);
    addBins(1,4,tmp);
    addBins(1,4,tmp);
    addBins(1,7,tmp);
    addBins(38,7,tmp);
  }
  if(icut==11){
    tmp = makeBins(97,-0.5,96.5);
    addBins(1,2,tmp);
    addBins(1,2,tmp);
    addBins(1,2,tmp);
    addBins(1,3,tmp);
    addBins(1,5,tmp);
    addBins(54,5,tmp);
  }
  if(icut==12){
    tmp = makeBins(78,-0.5,77.5);
    addBins(1,2,tmp);
    addBins(1,2,tmp);
    addBins(1,3,tmp);
    addBins(1,5,tmp);
    addBins(1,5,tmp);
    addBins(58,5,tmp);
  }
  if(icut==13){
    tmp = makeBins(57,-0.5,56.5);
    addBins(1,2,tmp);
    addBins(1,3,tmp);
    addBins(1,3,tmp);
    addBins(1,8,tmp);
    addBins(34,8,tmp);
  }
  if(icut==14){
    tmp = makeBins(34,-0.5,33.5);
    addBins(1,2,tmp);
    addBins(1,6,tmp);
    addBins(53,6,tmp);
  }
  if(icut==15){
    tmp = makeBins(70,-0.5,69.5);
    addBins(1,2,tmp);
    addBins(1,2,tmp);
    addBins(1,3,tmp);
    addBins(1,3,tmp);
    addBins(60,5,tmp);
  }
  if(icut==16){
    tmp = makeBins(61,-0.5,60.5);
    addBins(1,2,tmp);
    addBins(1,2,tmp);
    addBins(1,2,tmp);
    addBins(1,2,tmp);
    addBins(1,3,tmp);
    addBins(1,10,tmp);
    addBins(30,10,tmp);
  }
  if(icut==17){
    tmp = makeBins(52,-0.5,51.5);
    addBins(1,2,tmp);
    addBins(1,3,tmp);
    addBins(1,4,tmp);
    addBins(64,5,tmp);
  }
  if(icut==18){
    tmp = makeBins(38,-0.5,37.5);
    addBins(1,2,tmp);
    addBins(1,4,tmp);
    addBins(1,10,tmp);
    addBins(33,10,tmp);
  }
  if(icut==19){
    tmp = makeBins(24,-0.5,23.5);
    addBins(1,3,tmp);
    addBins(71,5,tmp);
  }
  if(icut==20){
    tmp = makeBins(37,-0.5,36.5);
    addBins(1,2,tmp);
    addBins(1,4,tmp);
    addBins(68,5,tmp);
  }
  if(icut==21){
    tmp = makeBins(33,-0.5,32.5);
    addBins(1,2,tmp);
    addBins(1,3,tmp);
    addBins(69,5,tmp);
  }
  if(icut==22){
    tmp = makeBins(28,-0.5,27.5);
    addBins(1,2,tmp);
    addBins(1,3,tmp);
    addBins(70,5,tmp);
  }
  if(icut==23){
    tmp = makeBins(22,-0.5,21.5);
    addBins(1,2,tmp);
    addBins(1,6,tmp);
    addBins(65,6,tmp);
  }
  if(icut==24){
    tmp = makeBins(14,-0.5,13.5);
    addBins(1,2,tmp);
    addBins(73,5,tmp);
  }
  if(icut==25){
    tmp = makeBins(67,-0.5,66.5);
    addBins(1,2,tmp);
    addBins(1,2,tmp);
    addBins(1,2,tmp);
    addBins(1,2,tmp);
    addBins(1,2,tmp);
    addBins(1,2,tmp);
    addBins(1,2,tmp);
    addBins(1,3,tmp);
    addBins(1,10,tmp);
    addBins(29,10,tmp);
  }
  if(icut==26){
    tmp = makeBins(38,-0.5,37.5);
    addBins(1,2,tmp);
    addBins(1,3,tmp);
    addBins(68,5,tmp);
  }
  if(icut==27){
    tmp = makeBins(40,-0.5,39.5);
    addBins(1,2,tmp);
    addBins(1,3,tmp);
    addBins(67,5,tmp);
  }
  if(icut==28){
    tmp = makeBins(38,-0.5,37.5);
    addBins(1,2,tmp);
    addBins(1,3,tmp);
    addBins(68,5,tmp);
  }
  if(icut==29){
    tmp = makeBins(40,-0.5,39.5);
    addBins(1,2,tmp);
    addBins(1,3,tmp);
    addBins(67,5,tmp);
  }
} 
    
    
if(icut==43){
  tmp = makeBins(127,-0.5,380.5);
}
  
if(icut==44){
  tmp = makeBins(74,-0.5,369.5);
}
    
if(icut==45){
  tmp = makeBins(55,-0.5,384.5);
}
    
if(icut==46){
  tmp = makeBins(37,-0.5,369.5);
}




  bins.push_back(tmp);
  
  //adding default pt 
  // bins.push_back(makeBins(30,0.,3.));
  tmp.clear();
  tmp = makeBins(200,0.,2.);
  addBins(80,.1,tmp);
  bins.push_back(tmp);

  // eta bins
  bins.push_back(makeBins(20,-2.4,2.4));

  // pt
  tmp.clear();
  tmp = makeBins(200,0.,2.);
  addBins(80,.1,tmp);
  bins.push_back(tmp);

  // mt
  tmp.clear();
  tmp = makeBins(200,0.,2.);
  addBins(80,.1,tmp);
  bins.push_back(tmp);

  return bins;
}


//old version
/*
vector< vector<double> > getBins(int icut , double E = 0.9){
  vector< vector<double> > bins;
  
  int start = 112;
  int step = 4;
  if( double(icut) % 5 == 0 )
    start = 38;
  if( double(icut) % 5 == 0 )
    start = 61;
  
  start-=(icut - (icut % 5)) * step;
  if((icut - (icut % 5))>2)
  
    start-= 4 * ( (icut - (icut % 5)) - 2);
    
    
if(E==0.9){  
  if(icut==0)  start = 37;
  if(icut==1)  start = 35;
  if(icut==2)  start = 29;
  if(icut==3)  start = 22;
  if(icut==4)  start = 14;
  if(icut==5)  start = 60;
  if(icut==6)  start = 55;
  if(icut==7)  start = 45;
  if(icut==8)  start = 34;
  if(icut==9)  start = 20;
  if(icut==10) start = 56;
  if(icut==11) start = 51;
  if(icut==12) start = 41;
  if(icut==13) start = 30;
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
  addBins(3,20,tmp);
  bins.push_back(tmp);
  
  //adding default pt and eta bins 
  bins.push_back(makeBins(30,0.,3.));
  bins.push_back(makeBins(20,-2.4,2.4));
  return bins;
}
*/
