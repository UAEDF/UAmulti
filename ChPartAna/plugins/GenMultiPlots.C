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

void GenMultiPlots::fill(MyGenKin& genKin , MyPart& p , double weight){
   
	                                         mp_INC->fill(p);
  if(this->isSD(genKin))                         mp_SD->fill(p);
  if(this->isDD(genKin))                         mp_DD->fill(p);
  if(!this->isSD(genKin) && !this->isDD(genKin)) mp_ND->fill(p);
  if(!this->isSD(genKin))                        mp_NSD->fill(p);
}

void GenMultiPlots::nextEvent(MyGenKin& genKin , double weight){
   				                 mp_INC->nextEvent();
  if(this->isSD(genKin))                         mp_SD->nextEvent();
  if(this->isDD(genKin))                         mp_DD->nextEvent();
  if(!this->isSD(genKin) && !this->isDD(genKin)) mp_ND->nextEvent();
  if(!this->isSD(genKin))                        mp_NSD->nextEvent();
}

void GenMultiPlots::write(){

  gDirectory->mkdir("GenMultiPlots_"+plotsname);
  gDirectory->cd("GenMultiPlots_"+plotsname);
  
  mp_INC->write();
  mp_SD->write();
  mp_DD->write();
  mp_ND->write();
  mp_NSD->write();
  
  gDirectory->cd("../");
}


bool GenMultiPlots::isSD(MyGenKin& genKin){
  if(genKin.MCProcId == 92 || genKin.MCProcId == 93)
    return true;
  return false;
}

bool GenMultiPlots::isDD(MyGenKin& genKin){
  if(genKin.MCProcId == 94)
    return true;
  return false;
}
