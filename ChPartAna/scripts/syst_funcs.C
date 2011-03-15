#ifndef __Syst_funcs_H__
#define __Syst_funcs_H__

#include <vector>
#include <iostream>
using namespace std;

#include "makePlots.h"
#include "../plugins/Syst.h"



void draw_vsyst(vector<Syst>& vsyst , vector<TString> vleg){

  makeCanv("Systematics");

  if(vsyst.size() == 0){
    cout << "[draw_syst] Error ! Input vector of systematic is void. Nothing to do ... " << endl;
    return;
  }
  //TCanvas c1

  //Adding systematics
  vector<Syst> vsyst_sum(vsyst.size() , Syst());
  vsyst_sum[0] = vsyst[0];
  vsyst_sum[0].addStatToSyst();
  for(unsigned i = 1 ; i < vsyst.size() ; ++i){
    vsyst_sum[i] = vsyst_sum[i-1];
    vsyst_sum[i].add(vsyst[i]);
  }

  //Drawing systematics
  vsyst_sum[vsyst.size()-1].rsyst->SetFillColor(16);
  vsyst_sum[vsyst.size()-1].rsyst->Draw("a2");
  for(int i = 2 ; i <= vsyst.size() ; ++i){
    vsyst_sum[vsyst.size()-i].rsyst->SetFillColor(standardColor[i]);
    vsyst_sum[vsyst.size()-i].rsyst->Draw("2 same");
  }
  vsyst[0].rstat->SetLineColor(kRed);
  vsyst[0].rstat->Draw("zsame");
  
  //TLegend
  if(vsyst.size() == vleg.size()){
    
    TLegend* legend = new TLegend(0.8 , 0.99 - (vleg.size()+2) * 0.04 , 0.99 , 0.99 );
    legend->SetHeader("Errors :");
    legend->SetFillColor(0);
    legend->AddEntry(vsyst[0].rstat , "stat." , "l");
    for(unsigned i = 0 ; i < vsyst_sum.size() ; ++i){
      legend->AddEntry(vsyst_sum[i].rsyst , vleg[i] , "f");
    }
    
    legend->Draw("same");
  }

  gPad->Modified(1);
  gPad->Update();
  gPad->WaitPrimitive();

}



void draw_syst(Syst& syst , TString leg){

  makeCanv("Systematics");

 
  //Drawing systematics
  syst.rsyst->SetFillColor(16);
  syst.rsyst->Draw("a2");
  syst.rstat->SetLineColor(kRed);
  syst.rstat->Draw("zsame");
  
  //TLegend
  TLegend* legend = new TLegend(0.8 , 0.85 , 0.99 , 0.99);
  legend->SetHeader("Errors :");
  legend->SetFillColor(0);
  legend->AddEntry(syst.rstat , "stat." , "l");
  legend->AddEntry(syst.rsyst , leg , "f");
  
  legend->Draw("same");
  

  gPad->Modified(1);
  gPad->Update();
  gPad->WaitPrimitive();

}



#endif
