//#include "TMean.cc"
//#include <vector>

#include "GenMultiPlots.h"
using namespace std;

ClassImp(GenMultiPlots)

//Default Constructor
GenMultiPlots::GenMultiPlots(){
  plotsname = "none";
  this->init();
}

GenMultiPlots::GenMultiPlots(TString name){
  plotsname = name;
  this->init();
}

//Destructor
GenMultiPlots::~GenMultiPlots(){
}


void GenMultiPlots::init(){
  mp_INC = new MultiPlots("INC_"+plotsname);
  mp_SD  = new MultiPlots("SD_"+plotsname);
  mp_DD  = new MultiPlots("DD_"+plotsname);
  mp_ND  = new MultiPlots("ND_"+plotsname);
  mp_NSD = new MultiPlots("NSD_"+plotsname);
}

void GenMultiPlots::fill(MyGenKin& genKin , MyPart& p , double weight , TString MCtype){
   
	                                                       mp_INC->fill(p);
  if(this->isSD(genKin,MCtype))                                mp_SD->fill(p);
  if(this->isDD(genKin,MCtype))                                mp_DD->fill(p);
  if(!this->isSD(genKin,MCtype) && !this->isDD(genKin,MCtype)) mp_ND->fill(p);
  if(!this->isSD(genKin,MCtype))                               mp_NSD->fill(p);
}

void GenMultiPlots::nextEvent(MyGenKin& genKin , TString MCtype , bool laccept , double weight){
   				                     mp_INC->nextEvent();
 /* if(this->isSD(genKin)) */                        mp_SD->nextEvent(this->isSD(genKin,MCtype));
 /* if(this->isDD(genKin))     */                    mp_DD->nextEvent(this->isDD(genKin,MCtype));
 /* if(!this->isSD(genKin) && !this->isDD(genKin))*/ mp_ND->nextEvent(!this->isSD(genKin,MCtype) && !this->isDD(genKin,MCtype));
 /* if(!this->isSD(genKin))              */          mp_NSD->nextEvent(!this->isSD(genKin,MCtype));
}

void GenMultiPlots::write(bool scale){

  gDirectory->mkdir("GenMultiPlots_"+plotsname);
  gDirectory->cd("GenMultiPlots_"+plotsname);
  
  mp_INC->write(scale);
  mp_SD->write(scale);
  mp_DD->write(scale);
  mp_ND->write(scale);
  mp_NSD->write(scale);
  
  gDirectory->cd("../");
}


bool GenMultiPlots::isSD(MyGenKin& genKin , TString MCtype){
  if(MCtype == "pythia" && (genKin.MCProcId == 92 || genKin.MCProcId == 93))
    return true;
  if(MCtype == "phojet" && (genKin.MCProcId == 5 || genKin.MCProcId == 6))
    return true;
  return false;
}

bool GenMultiPlots::isDD(MyGenKin& genKin , TString MCtype){
  if(MCtype == "pythia" && genKin.MCProcId == 94)
    return true;
  if(MCtype == "phojet" && (genKin.MCProcId == 4 || genKin.MCProcId == 7)) // 4 = DPE , 6 = DD
    return true;
  return false;
}
