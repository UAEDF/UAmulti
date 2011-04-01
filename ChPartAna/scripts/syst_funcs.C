#ifndef __Syst_funcs_H__
#define __Syst_funcs_H__

#include <vector>
#include <iostream>
using namespace std;

#include "makePlots.h"
#include "../plugins/Syst.h"

float standardColor_syst[]  = {16 , kBlue , kOrange , kMagenta , kGreen , kCyan};
void saveCanvas(TString , TString = "../figs/");

void draw_vsyst(vector<Syst>& vsyst , vector<TString>& vleg , Bool_t save = true , TString saveName = ""){

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
  vsyst_sum[vsyst.size()-1].rsyst->SetFillColor(kBlack);
  vsyst_sum[vsyst.size()-1].rsyst->GetXaxis()->SetRangeUser(-0.5 , 190.5);
  vsyst_sum[vsyst.size()-1].rsyst->GetYaxis()->SetRangeUser(-0.8 , 0.8);
  vsyst_sum[vsyst.size()-1].rsyst->SetTitle(";n;#sigma");
  vsyst_sum[vsyst.size()-1].rsyst->Draw("a2 9");
  for(int i = 2 ; i <= vsyst.size() ; ++i){
    vsyst_sum[vsyst.size()-i].rsyst->SetFillColor(standardColor_syst[vsyst.size()-i]);
    vsyst_sum[vsyst.size()-i].rsyst->Draw("2 same 9");
  }
  vsyst[0].rstat->SetLineColor(kRed);
  vsyst[0].rstat->Draw("zsame 9");
  
  //TLegend
  if(vsyst.size() == vleg.size()){
    
    TLegend* legend = new TLegend(0.3 , 0.99 - (vleg.size()+2) * 0.04 , 0.7 , 0.99 );
    legend->SetHeader("Errors :");
    legend->SetFillColor(0);
    legend->AddEntry(vsyst[0].rstat , "stat. " , "l");
    for(unsigned i = 0 ; i < vsyst_sum.size() ; ++i){
      legend->AddEntry(vsyst_sum[i].rsyst , "+ " + vleg[i] , "f");
    }
    
    legend->Draw("same");
  }

  gPad->Modified(1);
  gPad->Update();
  //gPad->WaitPrimitive();
  
  if(save){
    saveCanvas("syst_total_"+saveName);
  }
  
  gPad->Close();
    

}



void draw_syst(Syst& syst , TString leg , Bool_t save = true , TString saveName = ""){

  makeCanv("Systematics");

 
  //Drawing systematics
  syst.rsyst->GetXaxis()->SetRangeUser(-0.5 , 190.5);
  syst.rsyst->GetYaxis()->SetRangeUser(-0.8 , 0.8);
  syst.rsyst->SetFillColor(16);
  syst.rsyst->SetTitle(";n;#sigma");
  syst.rsyst->Draw("a2 9");
  syst.rstat->SetLineColor(kRed);
  syst.rstat->Draw("zsame 9");
  
  //TLegend
  TLegend* legend = new TLegend(0.3 , 0.85 , 0.7 , 0.99);
  legend->SetHeader("Errors :");
  legend->SetFillColor(0);
  legend->AddEntry(syst.rstat , "stat." , "l");
  legend->AddEntry(syst.rsyst , leg , "f");
  
  legend->Draw("same");
  

  gPad->Modified(1);
  gPad->Update();
  //gPad->WaitPrimitive();
  
  if(save){
    leg.ReplaceAll(".","");
    leg.ReplaceAll(" ","_");
    saveCanvas("syst_"+leg+"_"+saveName);
  }
    
  gPad->Close();

}

void saveCanvas(TString name , TString basedir ){

  string filename ("");
  filename += basedir;
  filename += "/gif/";
  if (!gSystem->OpenDirectory(filename.c_str())) gSystem->mkdir(filename.c_str(),true);
  filename += name;
  if(gPad->GetLogy()) filename +="_logY";
  filename += ".gif";
  gPad->SaveAs(filename.c_str(),"");
  cout << "[saveCanvas] Saved Canvas in gif : " << filename << endl;


  filename = "";
  filename += basedir;
  filename += "/eps/";
  if (!gSystem->OpenDirectory(filename.c_str())) gSystem->mkdir(filename.c_str(),true);
  filename += name;
  if(gPad->GetLogy()) filename +="_logY";
  filename += ".eps";
  gPad->SaveAs(filename.c_str(),"");
  cout << "[saveCanvas] Saved Canvas in eps : " << filename << endl;

  string command("convert ") ;
  command += filename + " ";
  filename = "";
  filename += basedir;
  filename += "/pdf/";
  if (!gSystem->OpenDirectory(filename.c_str())) gSystem->mkdir(filename.c_str(),true);
  filename += name;
  if(gPad->GetLogy()) filename +="_logY";
  filename += ".pdf";
  command += filename;
  gSystem->Exec(command.c_str());
  cout << "[saveCanvas] Saved Canvas in pdf : " << filename << endl;


}

#endif
