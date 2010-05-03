#include "TFile.h"
#include "TH1F.h"
#include "TGraphAsymmErrors.h"
#include "TString.h"
#include "TPad.h"
#include "TSystem.h"
#include "TStyle.h"
#include "TFrame.h"
#include "TObject.h"


#include <iostream>
#include <sstream>
using namespace std;

#include "../macro/fileManager.C"


double energy = 2.36;
int cut = 5;
TString plotdir = "../plots/";
bool logY = false;
TString plot = "unfolding/nch_data_corrected";
int typeMC = 10;

void plotSystematics(TString , TString , TString , TString , bool);

void plotSystematics(int syst , bool mirror = true){
  
  
  ostringstream outstr("");
  outstr << "hyp" << 1 << "_niter" << 0 << "_cut" << cut << "_DataType" << 0;

  TString tdata  = fileManager(3,typeMC,energy,1,0,0,outstr.str(),plotdir);
  TString tsyst1 = fileManager(3,typeMC,energy,1,syst,1,outstr.str(),plotdir);
  TString tsyst2 = fileManager(3,typeMC,energy,1,syst,-1,outstr.str(),plotdir);
  
  TString plot = "unfolding/nch_data_corrected";
  //TString plot = "unfolding/nch_data_INC_beforeSDsub";
  //TString plot = "unfolding/nch_data_NSD_afterSDsub";
  
  
  if(syst==1000){
    outstr.str("");
    outstr << "hyp" << 0 << "_niter" << 0 << "_cut" << cut << "_DataType" << 0;
    tsyst1 = fileManager(3,10,energy,1,0,0,outstr.str(),plotdir);
  }
  
  
  plotSystematics(tdata,tsyst1,tsyst2,plot,mirror);
}

void plotSystematics(TString tdata, TString tsyst1, TString tsyst2, TString plot , bool mirror){

  cout<<"Trying to open files :"<<endl;
  cout<<tdata<<endl;
  cout<<tsyst1<<endl;
  cout<<tsyst2<<endl;
  
  
  TFile* fdata  = TFile::Open(tdata,"READ");
  TFile* fsyst1 = TFile::Open(tsyst1,"READ");
  TFile* fsyst2 = TFile::Open(tsyst2,"READ");
  
  if(fsyst1==0 && fsyst2!=0){
    fsyst1=fsyst2;
    fsyst2=NULL;
  }

  if(fdata==0)
    cout<<"Data file is void. Please correct it. Exiting now ..."<<endl;
  if(fsyst1==0)
    cout<<"Syst1 file is void. Please correct it. Exiting now ..."<<endl;
  if(fsyst1==0 || fdata==0)
    return;


  TH1F* hdata  = (TH1F*) fdata->Get(plot);
  TH1F* hsyst1 = (TH1F*) fsyst1->Get(plot);
  TH1F* hsyst2;
  if(fsyst2!=0)
    hsyst2 = (TH1F*) fsyst2->Get(plot);
    
  if(hdata==0)
    cout<<"!! ERROR : plot in data file is void. Exiting now."<<endl;
  if(hsyst1==0)
    cout<<"!! ERROR : plot in syst1 file is void. Exiting now."<<endl;
  if(fsyst2!=0 && hsyst2==0)
    cout<<"!! ERROR : plot in syst2 file is void. Exiting now."<<endl;
    
  if(hdata==0 || hsyst1==0 || (fsyst2!=0 && hsyst2==0))
    return;
  

  Double_t* x   = new Double_t[hdata->GetNbinsX()];
  Double_t* y   = new Double_t[hdata->GetNbinsX()];
  Double_t* exl = new Double_t[hdata->GetNbinsX()];
  Double_t* exh = new Double_t[hdata->GetNbinsX()];
  Double_t* eyl = new Double_t[hdata->GetNbinsX()];
  Double_t* eyh = new Double_t[hdata->GetNbinsX()];
  for(int i = 1 ; i <= hdata->GetNbinsX() ; ++i){
    x[i-1] = hdata->GetBinCenter(i);
    y[i-1] = hdata->GetBinContent(i);
    exl[i-1] = hdata->GetBinCenter(i) - hdata->GetXaxis()->GetBinLowEdge(i);
    exh[i-1] = hdata->GetXaxis()->GetBinUpEdge(i) - hdata->GetBinCenter(i);
    
    //calculating error bars
    if(hsyst1->GetBinContent(i)>hdata->GetBinContent(i)){
      eyh[i-1] = hsyst1->GetBinContent(i) - hdata->GetBinContent(i);
      if(fsyst2==0){
        if(mirror)
          eyl[i-1] = eyh[i-1];
	else
	  eyl[i-1] = 0;
      }
      else{
        if(hsyst2->GetBinContent(i)<=hdata->GetBinContent(i))
	  eyl[i-1] = hdata->GetBinContent(i) - hsyst2->GetBinContent(i);
	else{
	  eyl[i-1] = 0;
	  if(hsyst2->GetBinContent(i)>hsyst1->GetBinContent(i))
	    eyh[i-1] = hsyst2->GetBinContent(i) - hdata->GetBinContent(i);
	  cout<<"!! WARNING : No systematic below point for bin "<<i<<endl;
	}
      }
    }
    else{
      eyl[i-1] = hdata->GetBinContent(i) - hsyst1->GetBinContent(i);
      if(fsyst2==0){
        if(mirror)
          eyh[i-1] = eyl[i-1];
	else
	  eyh[i-1] = 0;
      }
      else{
        if(hsyst2->GetBinContent(i)>=hdata->GetBinContent(i))
	  eyh[i-1] = hsyst2->GetBinContent(i) - hdata->GetBinContent(i);
	else{
	  eyh[i-1] = 0;
	  if(hsyst2->GetBinContent(i)<hsyst1->GetBinContent(i))
	    eyl[i-1] = hdata->GetBinContent(i) - hsyst2->GetBinContent(i);
	  cout<<"!! WARNING : No systematic higher than point for bin "<<i<<endl;
	}
      }
    }
  }


  TGraphAsymmErrors* gsyst = new TGraphAsymmErrors(hdata->GetNbinsX(),x,y,exl,exh,eyl,eyh);
  
  double max = 0;
  for(;max<hdata->GetNbinsX();++max)
    if(y[int(max)]==0)
      max+=exh[int(max)]+1;
  gsyst->GetXaxis()->SetRangeUser(hdata->GetXaxis()->GetXmin(),max);
  gsyst->GetXaxis()->SetTitle("nch");//hdata->GetXaxis()->GetTitle());
  gsyst->GetYaxis()->SetTitle("# events");//hdata->GetXaxis()->GetTitle());
  gsyst->SetFillColor(16);
  gsyst->Draw("a2");
  
  gPad->SetLeftMargin(0.17);
  gPad->SetBottomMargin(0.10);
  gPad->SetFillColor(0);
  gPad->GetFrame()->SetFillColor(21);
  gPad->GetFrame()->SetBorderSize(12);
  gPad->SetGrid(0,0);
  if(logY) gPad->SetLogy(true);
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(kFALSE);

  
  /*hsyst1->SetLineColor(kBlue);
  hsyst1->Draw("same");
  if(fsyst2!=0){
    hsyst2->SetLineColor(kOrange);
    hsyst2->Draw("same");
  }*/


  //hdata->SetMarkerStyle(kOpenCircle);
  hdata->SetMarkerStyle(kDot);
  hdata->SetMarkerSize(0.1);
  hdata->SetLineColor(kRed);
  hdata->SetMarkerColor(kRed);
  hdata->Draw("psame");
  
  gPad->WaitPrimitive();
  
  //writing the files in gif
  ostringstream figname("");
  figname << "../figs/" << energy << "Tev/" << "systematics/";
  if (!gSystem->OpenDirectory(figname.str().c_str())) gSystem->mkdir(figname.str().c_str(),true);
  TString filename = fsyst1->GetName();
  TString histname = hsyst1->GetName();
  filename.Remove(0,filename.Last('/')+1);
  histname.Remove(0,histname.Last('/')+1);
  
  figname << histname << "__" <<filename.ReplaceAll(".root","");
  if(logY) figname << "_logY";
  figname << ".gif";
  cout<<figname.str()<<endl;
  gPad->SaveAs(figname.str().c_str(),"");
}

void finaleSystematic(double energy = 0.9, int cut = 10 , bool printFig = false){
  vector<int> syst;
  syst.push_back(4);
  syst.push_back(102);
  syst.push_back(200);
  //syst.push_back(411);
  //syst.push_back(1000);//always last !!
  
  if(energy==7.0) typeMC = 31;
  else            typeMC = 10;
  
  vector<TH1F> syst1;
  vector<TH1F> syst2;
  
  ostringstream outstr("");
  outstr << "hyp" << 1 << "_niter" << 0 << "_cut" << cut << "_DataType" << 0;
  
  cout<<"------------------------------ NEW SYSTEMATIC CALCULATION ------------------------------"<<endl;
  
  //Taking the data stuff
  TString tdata  = fileManager(3,typeMC,energy,1,0,0,outstr.str(),plotdir);
  TFile*  fdata  = TFile::Open(tdata,"UPDATE");
  cout<<"Trying to open data file :"<<endl;
  cout<<tdata<<endl;
  
  if(fdata==0){
    cout<<"Data file is void. Please correct it. Exiting now ..."<<endl;
    return;
  }
  
  TH1F* hdata  = (TH1F*) fdata->Get(plot);
  if(hdata==0){
    cout<<"!! ERROR : plot in data file is void. Exiting now."<<endl;
    return;
  }  
   
  TH1F* dummy = new TH1F("dummy","dummy",1,0,1);
  
  
  
  
  //Getting all histogramms
  for(int i = 0 ; i < syst.size() ; ++i){
    TString tsyst1 = "";
    TString tsyst2 = "";
    
    //hyp syst
    if(syst.at(i)==1000){
      outstr.str("");
      outstr << "hyp" << 0 << "_niter" << 0 << "_cut" << cut << "_DataType" << 0;
      tsyst1 = fileManager(3,typeMC,energy,1,0,0,outstr.str(),plotdir);
      tsyst2 = fileManager(3,typeMC,energy,1,0,0,outstr.str(),plotdir);
    }
    else {
      tsyst1 = fileManager(3,typeMC,energy,1,syst.at(i),1,outstr.str(),plotdir);
      tsyst2 = fileManager(3,typeMC,energy,1,syst.at(i),-1,outstr.str(),plotdir);
    }
    
    cout<<"Trying to open syst files :"<<endl;
    cout<<tsyst1<<endl;
    cout<<tsyst2<<endl;
  
    TFile* fsyst1 = TFile::Open(tsyst1,"READ");
    TFile* fsyst2 = TFile::Open(tsyst2,"READ");
  
    if(fsyst1==0 && fsyst2!=0){
      fsyst1=fsyst2;
      fsyst2=NULL;
    }

    if(fsyst1==0){
      cout<<"Syst1 file is void. Please correct it. Skipping it now ..."<<endl;
      continue;
    }
    if(fsyst2==0){
      cout<<"Using dummy/mirror for "<<tsyst2<<endl;
    }

    TH1F* hsyst1 = (TH1F*) fsyst1->Get(plot);
    TH1F* hsyst2 = NULL;
    if(fsyst2!=0)
      hsyst2 = (TH1F*) fsyst2->Get(plot);
  
    if(hsyst1==0)
      cout<<"!! ERROR : plot in syst1 file is void. Exiting now."<<endl;
    if(fsyst2!=0 && hsyst2==0)
      cout<<"!! ERROR : plot in syst2 file is void. Exiting now."<<endl;
    
    if(hsyst1==0 || (fsyst2!=0 && hsyst2==0))
      continue;
    
    syst1.push_back(*hsyst1);
    if(hsyst2==0) syst2.push_back(*dummy);
    else syst2.push_back(*hsyst2);
    
    /*delete fsyst1;
    delete fsyst2;
    delete hsyst1;
    delete hsyst2;*/
  }
  
  
  //-----------------------------------
  //Making the systematic
  Double_t* x   = new Double_t[hdata->GetNbinsX()];
  Double_t* y   = new Double_t[hdata->GetNbinsX()];
  Double_t* exl = new Double_t[hdata->GetNbinsX()];
  Double_t* exh = new Double_t[hdata->GetNbinsX()];
  Double_t* eyl = new Double_t[hdata->GetNbinsX()];
  Double_t* eyh = new Double_t[hdata->GetNbinsX()];
  for(int i = 1 ; i <= hdata->GetNbinsX() ; ++i){
    x[i-1] = hdata->GetBinCenter(i);
    y[i-1] = hdata->GetBinContent(i);
    exl[i-1] = hdata->GetBinCenter(i) - hdata->GetXaxis()->GetBinLowEdge(i);
    exh[i-1] = hdata->GetXaxis()->GetBinUpEdge(i) - hdata->GetBinCenter(i);
    eyl[i-1] = 0;
    eyh[i-1] = 0;
    
    double binc = hdata->GetBinContent(i);
    for(int isyst = 0 ; isyst < syst1.size() ; ++isyst){
    
      double sbinc1 = syst1.at(isyst).GetBinContent(i);
      
      bool s2IsVoid = false;
      if(TString(syst2.at(isyst).GetName()).Contains("dummy")) s2IsVoid = true;
      double sbinc2 = 0;
      if(!s2IsVoid)sbinc2 = syst2.at(isyst).GetBinContent(i);
      
      if(sbinc1 > binc){
        if(!s2IsVoid){
          if(sbinc2 > sbinc1){
	    eyh[i-1] += pow(sbinc2-binc , 2);
	    eyl[i-1] += 0;
	  }
	  else{
	    eyh[i-1] += pow(sbinc1-binc , 2);
	    if(sbinc2<binc) eyl[i-1] += pow(binc - sbinc2 , 2);
	    else eyl[i-1] += 0;
	  }
	}
	else {
	  eyl[i-1] += pow(sbinc1-binc , 2);
	  eyh[i-1] += pow(sbinc1-binc , 2);
	}
      }
      else{
        if(!s2IsVoid){
          if(sbinc2 < sbinc1){
	    eyl[i-1] += pow(sbinc2-binc , 2);
	    eyh[i-1] += 0;
	  }
	  else{
	    eyl[i-1] += pow(sbinc1-binc , 2);
	    if(sbinc2>binc) eyh[i-1] += pow(sbinc2-binc , 2);
	    else eyh[i-1] += 0;
	  }
	}
	else {
	  eyl[i-1] += pow(sbinc1-binc , 2);
	  eyh[i-1] += pow(sbinc1-binc , 2);
	}      
      }
      
      //cout<<i<<"  "<<eyl[i-1]<<"  "<<eyh[i-1]<<endl;
     // cout<<binc<<"  "<<sbinc1<<"  "<<sbinc2<<endl;
    }//end of loop over syst
    
    eyl[i-1] = sqrt(eyl[i-1] + pow(hdata->GetBinError(i),2));
    eyh[i-1] = sqrt(eyh[i-1] + pow(hdata->GetBinError(i),2));
    //cout<<"lllllllllllll   "<<eyl[i-1]<<"  "<<eyh[i-1]<<endl;
    //eyl[i-1] = sqrt(eyl[i-1]);
    //eyh[i-1] = sqrt(eyh[i-1]);   
  }//end of loop over bins
  
  TGraphAsymmErrors* gsyst = new TGraphAsymmErrors(hdata->GetNbinsX(),x,y,exl,exh,eyl,eyh);
  gsyst->SetName(TString("g")+hdata->GetName()+TString("_syst"));
  gsyst->SetTitle(TString("g")+hdata->GetTitle()+TString("_syst"));
  fdata->cd("unfolding/");
  gsyst->Write(0,6);
  
  
  double max = 0;
  for(;max<hdata->GetNbinsX();++max)
    if(y[int(max)]==0)
      max+=exh[int(max)]+1;
  gsyst->GetXaxis()->SetRangeUser(hdata->GetXaxis()->GetXmin(),max);
  gsyst->GetXaxis()->SetTitle("nch");//hdata->GetXaxis()->GetTitle());
  gsyst->GetYaxis()->SetTitle("P_{n}");//hdata->GetXaxis()->GetTitle());
  gsyst->SetFillColor(16);
  gsyst->Draw("a2");
  
  gPad->SetLeftMargin(0.17);
  gPad->SetBottomMargin(0.10);
  gPad->SetFillColor(0);
  gPad->GetFrame()->SetFillColor(21);
  gPad->GetFrame()->SetBorderSize(12);
  gPad->SetGrid(0,0);
  if(logY) gPad->SetLogy(true);
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(kFALSE);

  
  /*syst1.at(0).SetLineColor(kBlue);
  syst1.at(0).Draw("same");
  if(syst2.at(0).GetName() != "dummy"){
    syst2.at(0).SetLineColor(kOrange);
    syst2.at(0).Draw("same");
  }*/


  //hdata->SetMarkerStyle(kOpenCircle);
  hdata->SetMarkerStyle(kDot);
  hdata->SetMarkerSize(0.1);
  hdata->SetLineColor(kRed);
  hdata->SetMarkerColor(kRed);
  hdata->Draw("psame");
  
  gPad->WaitPrimitive();
  
  if(printFig){
    ostringstream figname("");
    figname<<"finalesyst_cut"<<cut;
    gPad->SaveAs(TString(figname.str().c_str())+TString(".gif"),"");
    gPad->SaveAs(TString(figname.str().c_str())+TString(".root"),"");
  }
}


