#include "Syst.h"
 
ClassImp(Syst)


Int_t Syst::sysType = Syst::half;


//--------------------------------------------------------------------------
//----------------------------   CONSTRUCTORS   ---------------------------- 
//--------------------------------------------------------------------------

//_____________________________________________________________________________
Syst::Syst(){
  syscoll = "none";
  this->init();
}


//_____________________________________________________________________________
Syst::Syst(TString collname){
  syscoll = collname;
  this->init();
}

//_____________________________________________________________________________
Syst::Syst(TString collname , TH1F* main){
  syscoll = collname;
  this->init();
  this->setMain(main); 
}

//_____________________________________________________________________________
Syst::Syst(TString collname , TH1F* main , TH1F* sys1 , TH1F* sys2){
  syscoll = collname;
  this->init();
  
  this->setMain(main);
  this->setSyst(sys1,sys2);
}

//_____________________________________________________________________________
Syst::Syst(TString collname, TGraphAsymmErrors* ginput){ //reconstruct main and rsyst out of gsyst
  syscoll = collname;
  this->init();
  this->setgSyst(ginput);
}


//_____________________________________________________________________________
Syst::Syst(TString collname, TH1F* main , TGraphAsymmErrors* input){ //reconstruct either gsyst or rsyst out of main & input
  syscoll = collname;
  this->init();
  if(this->isArSyst(input))
    this->setrSyst(main , input);
  else
    this->setgSyst(main , input);
}




//--------------------------------------------------------------------------
//----------------------------    DESTRUCTOR    ---------------------------- 
//--------------------------------------------------------------------------


//_____________________________________________________________________________
Syst::~Syst(){
  if(gsyst!=0)    delete gsyst; 
  if(rsyst!=0)    delete rsyst;
  if(rstat!=0)    delete rstat;
  if(mainTH1!=0)  delete mainTH1;
  if(sys1TH1!=0)  delete sys1TH1;
  if(sys2TH1!=0)  delete sys2TH1;	
}


//_____________________________________________________________________________
void Syst::init(){
  gsyst    = NULL;
  rsyst    = NULL;
  rstat    = NULL;
  mainTH1  = NULL;
  sys1TH1  = NULL;
  sys2TH1  = NULL;
  
  isStatIncluded = false;
}

//_____________________________________________________________________________
void Syst::reset(){
  this->init();
}



//--------------------------------------------------------------------------
//------------------------------      COPY      ---------------------------- 
//--------------------------------------------------------------------------


void Syst::copy(Syst& newSyst) const {

  newSyst.TObject::Copy(*((TObject*)this));

  newSyst.syscoll = this->syscoll;
  
  if(this->mainTH1!=0)
    newSyst.mainTH1 = (TH1F*) this->mainTH1->Clone();
  else
    newSyst.mainTH1 = NULL;
    
  if(this->sys1TH1!=0)
    newSyst.sys1TH1 = (TH1F*) this->sys1TH1->Clone();
  else
    newSyst.sys1TH1 = NULL;
  
  if(this->sys2TH1!=0)
    newSyst.sys2TH1 = (TH1F*) this->sys2TH1->Clone();
  else
    newSyst.sys2TH1 = NULL;
  
  if(this->gsyst!=0)
    newSyst.gsyst   = (TGraphAsymmErrors*) this->gsyst->Clone();
  else
    newSyst.gsyst = NULL;
  
  if(this->rsyst!=0)
    newSyst.rsyst   = (TGraphAsymmErrors*) this->rsyst->Clone();
  else
    newSyst.rsyst = NULL;
  
  if(this->rstat!=0)
    newSyst.rstat   = (TGraphAsymmErrors*) this->rstat->Clone();
  else
    newSyst.rstat = NULL;
  
  newSyst.isStatIncluded  = this->isStatIncluded;
}


Syst::Syst(const Syst& in):TObject(in){
  in.copy(*this);
}

Syst::Syst(TString n , const Syst& in):TObject(in){
  in.copy(*this);
  syscoll = n;
}


Syst& Syst::operator= (const Syst& in){
  in.copy(*this);
  return *this;
}


//--------------------------------------------------------------------------
//----------------------------      SETTERS     ---------------------------- 
//--------------------------------------------------------------------------


//_____________________________________________________________________________
void Syst::setMain(TH1F* main){
  if(mainTH1 != 0) delete mainTH1;
  mainTH1 = (TH1F*) main->Clone("Main TH1F");
  if(mainTH1->GetSumw2() == 0) mainTH1->Sumw2();
  this->makerStat();
  
  if(sys1TH1 != 0)    makeSyst();
  else if(gsyst != 0) makerSyst();
  else if(rsyst != 0) makegSyst();  
}


//_____________________________________________________________________________
void Syst::setSyst(TH1F* sys1, TH1F* sys2){
  if(sys1TH1 != 0) delete sys1TH1;
  if(sys2TH1 != 0) delete sys2TH1;
  
  sys1TH1 = (TH1F*) sys1->Clone("syst1_TH1F");
  if(sys2!=0) sys2TH1 = (TH1F*) sys2->Clone("syst2_TH1F");
  else cout << "Sys2 is empty ! " << endl;
  if(mainTH1 != 0) makeSyst();
}


//_____________________________________________________________________________
void Syst::setSyst(TH1F* main , TH1F* sys1, TH1F* sys2){
  this->reset();
  this->setMain(main);
  this->setSyst(sys1 , sys2);
}



//_____________________________________________________________________________
void Syst::setgSyst(TGraphAsymmErrors* ginput){
  if(gsyst != 0) delete gsyst;
  gsyst = (TGraphAsymmErrors*) ginput->Clone("gsyst");
  if(mainTH1 == 0)
    this->makeMain();
  this->makerSyst();
}

//_____________________________________________________________________________
void Syst::setgSyst(TH1F* main , TGraphAsymmErrors* ginput){
  this->reset();
  this->setMain(main);
  this->setgSyst(ginput);
}


//_____________________________________________________________________________
void Syst::setrSyst(TGraphAsymmErrors* rinput){
  if(rsyst != 0) delete rsyst;
  rsyst = (TGraphAsymmErrors*) rinput->Clone("rsyst");
  if(mainTH1 != 0) this->makegSystFromrSyst();
}

//_____________________________________________________________________________
void Syst::setrSyst(TH1F* main , TGraphAsymmErrors* rinput){
  this->reset();
  this->setMain(main);
  this->setrSyst(rinput);
}



//--------------------------------------------------------------------------
//----------------------------      GETTERS     ---------------------------- 
//--------------------------------------------------------------------------

     
//_____________________________________________________________________________
double Syst::getMinSyst(double val , double syst1 , double syst2){
  if(syst1<val && syst1<syst2) return val - syst1;
  else if(syst1<val && syst1>syst2) return val - syst2;
  else if(syst1>=val && syst2 < val) return val - syst2;
  else return 0;
}


//_____________________________________________________________________________
double Syst::getMaxSyst(double val , double syst1 , double syst2){
  if(syst1>val && syst1>syst2) return syst1 - val;
  else if(syst1>val && syst1<syst2) return syst2 - val;
  else if(syst1<=val && syst2 > val) return syst2 - val;
  else return 0;
}




//--------------------------------------------------------------------------
//----------------------------   OTHER FUNCTIONS  --------------------------
//--------------------------------------------------------------------------

//_____________________________________________________________________________
Bool_t Syst::isArSyst(TGraphAsymmErrors* input){
  for(int i = 0 ; i < input->GetN() ; ++i){
    Double_t x;
    Double_t y;
    input->GetPoint(i,x,y);
    if(y != 0) return false;
  }
  return true;
}



//_____________________________________________________________________________
void Syst::add(TGraphAsymmErrors* ginput){
  if(gsyst==0) setgSyst(ginput);
  else {
    //here  go through all points and do   /sqrt(gsyst1²+gsyst2²)=gsyst
    for(int i = 0 ; i < gsyst->GetN() ; ++i){ 
      gsyst->SetPointEYlow (i,sqrt( pow(gsyst->GetErrorYlow (i),2) + pow(ginput->GetErrorYlow (i),2) ));
      gsyst->SetPointEYhigh(i,sqrt( pow(gsyst->GetErrorYhigh(i),2) + pow(ginput->GetErrorYhigh(i),2) ));
    }	     
  }
  
  this->makerSyst();    
}


//_____________________________________________________________________________
void Syst::add(Syst& sysclass){
  this->add(sysclass.gsyst);
}


//_____________________________________________________________________________
void Syst::addStatToSyst(){
  if(mainTH1==0){
    cout << "[Syst::addStatToSyst] {" << syscoll << "} Error ! You need to provide a mainTH1 with the stat errors !" << endl;
    return;
  }
  if(!isStatIncluded){
    //here  go through all points and do   /sqrt(mainError²+gsyst²)=gsyst
    for(int i = 0 ; i < gsyst->GetN() ; ++i){
      gsyst->SetPointEYlow (i,sqrt( pow(gsyst->GetErrorYlow (i),2) + pow(mainTH1->GetBinError(i+1),2) ));
      gsyst->SetPointEYhigh(i,sqrt( pow(gsyst->GetErrorYhigh(i),2) + pow(mainTH1->GetBinError(i+1),2) ));
    }
    
    this->makerSyst();
    isStatIncluded = true; 	   
  }
  
}


//_____________________________________________________________________________
void Syst::write(){
    gDirectory->mkdir("Syst_"+syscoll);
    gDirectory->cd("Syst_"+syscoll);
    
    if(mainTH1!=0) mainTH1->Write();
    if(sys1TH1!=0) sys1TH1->Write();
    if(sys2TH1!=0) sys2TH1->Write();
    if(gsyst!=0)   gsyst->Write();
    if(rsyst!=0)   rsyst->Write();
    this->Write("syst_class_"+syscoll);
    
    gDirectory->cd("../");
}



//_____________________________________________________________________________
void Syst::Draw(Option_t* option){
  gsyst->Draw(option);
}





//---------------------------------------------------------------------------------------
//				PRIVATE FUNCTIONS
  
  
  
    
//--------------------------------------------------------------------------
//----------------------------      MAKERS     ---------------------------- 
//--------------------------------------------------------------------------


//_____________________________________________________________________________
void Syst::makeMain(){

  cout << "[Syst::makeMain] {" << syscoll << "} Making Main ..." << endl;
  
  if(gsyst == 0){
    cout << "[Syst::makeMain] {" << syscoll << "} ERROR !! Can't make mainTH1 , gsyst is void. " << endl;
    return;
  }
  
  
  Double_t* xarray = new Double_t[gsyst->GetN()+1];
  Double_t x , y;
  for(int i = 0 ; i < gsyst->GetN() ; ++i){
    gsyst->GetPoint(i,x,y);
    xarray[i] = x - gsyst->GetErrorXlow(i);
  }
  xarray[gsyst->GetN()] = x + gsyst->GetErrorXhigh(gsyst->GetN()-1);
    
  if(mainTH1!=0) delete mainTH1;
  mainTH1 = new TH1F("mainTH1","mainTH1",gsyst->GetN(),xarray);
  mainTH1->Sumw2();
  
  for(int i = 0 ; i < gsyst->GetN() ; ++i){
    Double_t x , y;
    gsyst->GetPoint(i,x,y);
    mainTH1->SetBinContent(i+1 , y);
    mainTH1->SetBinError(i+1 , 0);
  }
  
  
  delete[] xarray;
}     


/*
//_____________________________________________________________________________
void Syst::makegSyst(){

  Bool_t mirror = true;
  
  cout << "[Syst::makegSyst] {" << syscoll << "} Making gSyst ..." << endl;
  
  if(mainTH1==0)
    cout<<"[Syst::makegSyst] {" << syscoll << "} ERROR !! Please provide a mainTH1."<<endl;
  if(sys1TH1==0)
    cout<<"[Syst::makegSyst] {" << syscoll << "} ERROR !! Please provide a sys1TH1."<<endl;
  if(sys2TH1==0)
    cout<<"[Syst::makegSyst] {" << syscoll << "} WARNING !! There is no sys2TH1, so sys1 will be mirrored."<<endl;
  if(mainTH1==0 || sys1TH1==0 )
    return; 
        
  Double_t* x    = new Double_t[mainTH1->GetNbinsX()];
  Double_t* y    = new Double_t[mainTH1->GetNbinsX()];
  Double_t* exl  = new Double_t[mainTH1->GetNbinsX()];
  Double_t* exh  = new Double_t[mainTH1->GetNbinsX()];
  Double_t* eyl  = new Double_t[mainTH1->GetNbinsX()];
  Double_t* eyh  = new Double_t[mainTH1->GetNbinsX()];
  
  
  for(int i = 1 ; i <= mainTH1->GetNbinsX() ; ++i){
    x[i-1] = mainTH1->GetBinCenter(i);
    y[i-1] = mainTH1->GetBinContent(i);
    exl[i-1] = mainTH1->GetBinCenter(i) - mainTH1->GetXaxis()->GetBinLowEdge(i);
    exh[i-1] = mainTH1->GetXaxis()->GetBinUpEdge(i) - mainTH1->GetBinCenter(i);
    
    //calculating error bars
    if(sys1TH1->GetBinContent(i)>mainTH1->GetBinContent(i)){
      eyh[i-1] = sys1TH1->GetBinContent(i) - mainTH1->GetBinContent(i);
      if(sys2TH1==0){
        if(mirror)
          eyl[i-1] = eyh[i-1];
	    else
	      eyl[i-1] = 0;
      }
      else{
        if(sys2TH1->GetBinContent(i)<=mainTH1->GetBinContent(i))
	    eyl[i-1] = mainTH1->GetBinContent(i) - sys2TH1->GetBinContent(i);
	else{
	  eyl[i-1] = 0;
	  if(sys2TH1->GetBinContent(i)>sys1TH1->GetBinContent(i))
	    eyh[i-1] = sys2TH1->GetBinContent(i) - mainTH1->GetBinContent(i);
	  cout<<"!! WARNING : No systematic below point for bin "<<i<<endl;
	}
      }
    }
    else{
      eyl[i-1] = mainTH1->GetBinContent(i) - sys1TH1->GetBinContent(i);
      if(sys2TH1==0){
        if(mirror)
          eyh[i-1] = eyl[i-1];
	    else
	      eyh[i-1] = 0;
      }
      else{
        if(sys2TH1->GetBinContent(i)>=mainTH1->GetBinContent(i))
	    eyh[i-1] = sys2TH1->GetBinContent(i) - mainTH1->GetBinContent(i);
	else{
	  eyh[i-1] = 0;
	  if(sys2TH1->GetBinContent(i)<sys1TH1->GetBinContent(i))
	    eyl[i-1] = mainTH1->GetBinContent(i) - sys2TH1->GetBinContent(i);
	  cout<<"!! WARNING : No systematic higher than point for bin "<<i<<endl;
	}
      }
    }
           
        
  }//end of loop over bins  
  
  if(gsyst!=0) delete gsyst;
  gsyst = new TGraphAsymmErrors(mainTH1->GetNbinsX(), x, y,  exl, exh,  eyl,  eyh);
  gsyst->SetName("gsyst");

  delete x;   
  delete y;  
  delete exl; 
  delete exh; 
  delete eyl; 
  delete eyh;   
}     

*/

//_____________________________________________________________________________
void Syst::makegSyst(){
  
  cout << "[Syst::makegSyst] {" << syscoll << "} Making gSyst ..." << endl;
  
  if(mainTH1==0)
    cout<<"[Syst::makegSyst] {" << syscoll << "} ERROR !! Please provide a mainTH1."<<endl;
  if(sys1TH1==0)
    cout<<"[Syst::makegSyst] {" << syscoll << "} ERROR !! Please provide a sys1TH1."<<endl;
  if(sys2TH1==0)
    cout<<"[Syst::makegSyst] {" << syscoll << "} WARNING !! There is no sys2TH1, so sys1 will be mirrored."<<endl;
  if(mainTH1==0 || sys1TH1==0 )
    return; 
        
  Double_t* x    = new Double_t[mainTH1->GetNbinsX()];
  Double_t* y    = new Double_t[mainTH1->GetNbinsX()];
  Double_t* exl  = new Double_t[mainTH1->GetNbinsX()];
  Double_t* exh  = new Double_t[mainTH1->GetNbinsX()];
  Double_t* eyl  = new Double_t[mainTH1->GetNbinsX()];
  Double_t* eyh  = new Double_t[mainTH1->GetNbinsX()];
  
  
  for(int i = 1 ; i <= mainTH1->GetNbinsX() ; ++i){
    x[i-1] = mainTH1->GetBinCenter(i);
    y[i-1] = mainTH1->GetBinContent(i);
    exl[i-1] = mainTH1->GetBinCenter(i) - mainTH1->GetXaxis()->GetBinLowEdge(i);
    exh[i-1] = mainTH1->GetXaxis()->GetBinUpEdge(i) - mainTH1->GetBinCenter(i);
    
    
    if(sys2TH1!=0){
      eyl[i-1] = this->getMinSyst(y[i-1] , sys1TH1->GetBinContent(i) , sys2TH1->GetBinContent(i) );
      eyh[i-1] = this->getMaxSyst(y[i-1] , sys1TH1->GetBinContent(i) , sys2TH1->GetBinContent(i) );
    }
    else{
    
      //none
      eyl[i-1] = this->getMinSyst(y[i-1] , sys1TH1->GetBinContent(i) , y[i-1] );
      eyh[i-1] = this->getMaxSyst(y[i-1] , sys1TH1->GetBinContent(i) , y[i-1] );
      
      //mirror
      if(sysType == Syst::mirror || sysType == Syst::half ){
        if(eyl[i-1] == 0) eyl[i-1] = eyh[i-1];
        if(eyh[i-1] == 0) eyh[i-1] = eyl[i-1];
      }
      
      //half
      if(sysType == Syst::half ){
        eyl[i-1] /= 2.;
      	eyh[i-1] /= 2.;
      }
    }
    
        
  }//end of loop over bins  
  
  if(gsyst!=0) delete gsyst;
  gsyst = new TGraphAsymmErrors(mainTH1->GetNbinsX(), x, y,  exl, exh,  eyl,  eyh);
  gsyst->SetName("gsyst");

  delete x;   
  delete y;  
  delete exl; 
  delete exh; 
  delete eyl; 
  delete eyh;   
}     

//_____________________________________________________________________________
void Syst::makegSystFromrSyst(){  //reconstructs rsyst. Needs gsyst. 
  if(mainTH1==0)
    cout << "[Syst::makegSystFromrSyst] {" << syscoll << "} ERROR !! Can't make gSyst , please provide a mainTH1. " << endl;
  
  if(gsyst != 0) delete gsyst;
  gsyst = (TGraphAsymmErrors*) rsyst->Clone("gsyst");
  
  for(int i = 0 ; i < rsyst->GetN() ; ++i){
    Double_t x;
    Double_t y;
    rsyst->GetPoint(i,x,y);
    gsyst->SetPoint(i,x,mainTH1->GetBinContent(i+1));
    gsyst->SetPointEYlow (i, rsyst->GetErrorYlow (i) * mainTH1->GetBinContent(i+1));
    gsyst->SetPointEYhigh(i, rsyst->GetErrorYhigh(i) * mainTH1->GetBinContent(i+1));
  }
}


//_____________________________________________________________________________
void Syst::makerSyst(){  //reconstructs rsyst. Needs gsyst. 

  cout << "[Syst::makerSyst] {" << syscoll << "} Making rSyst ..." << endl;	
  
  if(gsyst==0){
    cout << "[Syst::makerSyst] Error ! gsyst is not present. Can't reconstruct rsyst ..." << endl;
    return;
  }
  
  if(rsyst != 0) delete rsyst;
  rsyst = (TGraphAsymmErrors*) gsyst->Clone("rsyst");
  
  for(int i = 0 ; i < gsyst->GetN() ; ++i){
    Double_t x;
    Double_t y;
    gsyst->GetPoint(i,x,y);
    rsyst->SetPoint(i,x,0);
    if(y != 0){
      rsyst->SetPointEYlow (i, gsyst->GetErrorYlow (i)/y);
      rsyst->SetPointEYhigh(i, gsyst->GetErrorYhigh(i)/y);
    }
    else{
      rsyst->SetPointEYlow (i, 0);
      rsyst->SetPointEYhigh(i, 0);
    }
  } 

}


//_____________________________________________________________________________
void Syst::makeSyst(){
  makegSyst();
  makerSyst();
}    


//_____________________________________________________________________________
void Syst::makerStat(){

  cout << "[Syst::makerStat] {" << syscoll << "} Making rStat ..." << endl;	
  
  if(mainTH1==0){
    cout << "[Syst::makerStat] Error ! mainTH1 is not present. Can't reconstruct rstat ..." << endl;
    return;
  }
  
  if(rstat != 0) delete rstat;
  rstat = new TGraphAsymmErrors(mainTH1->GetNbinsX());
  rstat->SetName("rstat");
  
  for(int i = 0 ; i < rstat->GetN() ; ++i){
    rstat->SetPoint(i,mainTH1->GetBinCenter(i+1),0);
    rstat->SetPointEXlow (i , mainTH1->GetBinWidth(i+1) / 2. );
    rstat->SetPointEXhigh(i , mainTH1->GetBinWidth(i+1) / 2. );
    if(mainTH1->GetBinContent(i+1) != 0){
      rstat->SetPointEYlow (i, mainTH1->GetBinError(i+1)/ mainTH1->GetBinContent(i+1));
      rstat->SetPointEYhigh(i, mainTH1->GetBinError(i+1)/ mainTH1->GetBinContent(i+1));
    }
    else{
      rstat->SetPointEYlow (i, 0);
      rstat->SetPointEYhigh(i, 0);
    }
  } 

}

