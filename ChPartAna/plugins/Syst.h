#ifndef __Syst_H__
#define __Syst_H__

#include "TObject.h"
#include "TFile.h"
#include "TH1F.h"
#include "TGraphAsymmErrors.h"
#include "TString.h"
#include "TMath.h"


#include <iostream>
#include <sstream>
using namespace std;

class Syst : public TObject {

  public :
    
    //Constructors
    Syst();
    Syst(TString);
    Syst(TString, TH1F*);
    Syst(TString,TH1F*,TH1F*,TH1F*);
    Syst(TString, TGraphAsymmErrors*);
    Syst(TString, TH1F* , TGraphAsymmErrors*);
    
    //Destructor
    virtual ~Syst();
    
    void init();		//sets all pointers to NULL
    void reset();		//calls init()
    
    
    //Copy
    void copy(Syst&) const ;
    Syst(const Syst&);
    Syst& operator= (const Syst&);
    
    
    //Setters
    void setMain(TH1F*);				// in : Main   out : gSyst , rSyst if syst1 is here || rsyst if gsyst is here || gsyst if rsyst is here 
    void setSyst(TH1F*,TH1F*);				// in : syst1 (syst2)   out : gSyst , rSyst if main is here
    void setSyst(TH1F*,TH1F*,TH1F*);			// in : main , syst1 , (syst2)    out : gSyst , rSyst
    void setgSyst(TGraphAsymmErrors*);			// in : gSyst 		out : rSyst
    void setgSyst(TH1F* , TGraphAsymmErrors*);		// in : main , gSyst 	out : rSyst
    void setrSyst(TGraphAsymmErrors*);			// in : rSyst 		out : gSyst if main is here
    void setrSyst(TH1F* , TGraphAsymmErrors*);		// in : main , rSyst	out : gSyst
    
    //Getters
    double getMinSyst(double , double , double);
    double getMaxSyst(double , double , double);
    
    
    //Others
    Bool_t isArSyst(TGraphAsymmErrors* );		// if y = 0 all along, then is of type rSyst and returns true. Otherwise is considered as a gSyst.
      
    void add(TGraphAsymmErrors*);
    void add(Syst&);
    void addStatToSyst();				//Can be called many times : in case already done, will do nothing.
    
    void write();  
    
    void Draw(Option_t* = "");

    
    //MEMBERS
    TString syscoll; 
    
    TH1F* mainTH1;
    TH1F* sys1TH1;
    TH1F* sys2TH1;
    TGraphAsymmErrors* gsyst;
    TGraphAsymmErrors* rsyst;
    

  private:
  
  
    void makeMain();		//set bin error to 0
    void makegSyst();
    void makegSystFromrSyst();
    void makerSyst();
    void makeSyst();
    
    Bool_t isStatIncluded;
    
    ClassDef (Syst,1)
};
    
#endif
