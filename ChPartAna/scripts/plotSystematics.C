#include "TFile.h"
#include "TH1F.h"
#include "TProfile.h"
#include "TGraphAsymmErrors.h"
#include "TString.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TPad.h"
#include "TCanvas.h"
#include "TSystem.h"
#include "TStyle.h"
#include "TFrame.h"
#include "TObject.h"
#include "TROOT.h"


#include <iostream>
#include <sstream>
using namespace std;

#include "../macro/fileManager.C"

#include "../plugins/TMoments.h"


double energy = 0.9;
TString Estr = "a";
int cut = 5;
int SYST = 4;
TString plotdir = "../plots/";
bool logY = true;
TString plot = "unfolding/nch_data_corrected";
int typeMC = 10;

//void plotSystematics(TString , TString , TString , TString , bool);
double getMinSyst(double , double , double);
double getMaxSyst(double , double , double);
TH1F makeMirrorSyst(TH1F* , TH1F);


void plotSystematics(int syst , double E = 0.9 , int icut = 5 , int iMC = -1, int basesyst = 0, int iDataType = 0, int icutsyst = -1,
                     TString systleg = "NONE" , bool mirror = true){
  
  cut = icut;
  energy = E;
  SYST   = syst;
  if(E==0.9)  Estr = "a";
  if(E==2.36) Estr = "b";
  if(E==7.0)  Estr = "c";
  
  if(icutsyst<0) icutsyst = icut;
  
  typeMC = iMC;
  if(energy==7.0 && iMC==-1) typeMC = 31;
  else if(iMC==-1) typeMC = 10;
  
  plotdir = "../plots/";
  ostringstream outstr("");
  outstr << "hyp" << 1 << "_niter" << 0 << "_cut" << cut << "_DataType" << iDataType;
  
  TString tdata = "";
  if(basesyst==0)
    tdata  = fileManager(3,typeMC,energy,1,0,0,outstr.str(),plotdir);
  else
    tdata  = fileManager(3,typeMC,energy,1,basesyst,-1,outstr.str(),plotdir);
  
  outstr.str("");
  outstr << "hyp" << 1 << "_niter" << 0 << "_cut" << icutsyst << "_DataType" << iDataType;
  TString tsyst1 = "";
  TString tsyst2 = "";
  if(syst!=0){
    tsyst1 = fileManager(3,typeMC,energy,1,syst,1,outstr.str(),plotdir);
    tsyst2 = fileManager(3,typeMC,energy,1,syst,-1,outstr.str(),plotdir);
  }
  else{
    tsyst1 = fileManager(3,typeMC,energy,1,0,0,outstr.str(),plotdir);
    tsyst2 = fileManager(3,typeMC,energy,1,0,0,outstr.str(),plotdir);
  }
  
  
  TString plot = "unfolding/nch_data_corrected";
  //TString plot = "unfolding/nch_data_INC_beforeSDsub";
  //TString plot = "unfolding/nch_data_NSD_afterSDsub";
  
  
  if(syst==1000){
    outstr.str("");
    outstr << "hyp" << 0 << "_niter" << 15 << "_cut" << cut << "_DataType" << iDataType;
    tsyst1 = fileManager(3,typeMC,energy,1,0,0,outstr.str(),plotdir);
  }
  
  
 /* plotSystematics(tdata,tsyst1,tsyst2,plot,mirror);
}

void plotSystematics(TString tdata, TString tsyst1, TString tsyst2, TString plot , bool mirror){
*/
  gROOT->ProcessLine(".x cmsStyleRoot.C");
  
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
  

  Double_t* x    = new Double_t[hdata->GetNbinsX()];
  Double_t* y    = new Double_t[hdata->GetNbinsX()];
  Double_t* exl  = new Double_t[hdata->GetNbinsX()];
  Double_t* exh  = new Double_t[hdata->GetNbinsX()];
  Double_t* eyl  = new Double_t[hdata->GetNbinsX()];
  Double_t* eyh  = new Double_t[hdata->GetNbinsX()];
  
  Double_t* ry   = new Double_t[hdata->GetNbinsX()];
  Double_t* reyl = new Double_t[hdata->GetNbinsX()];
  Double_t* reyh = new Double_t[hdata->GetNbinsX()];
  double maxr = 1. , minr = 1.;
  
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
    
    //doing the ratio arrays
    ry[i-1] = 0.;
    reyl[i-1] = reyh[i-1] = 0;
    if(hdata->GetBinContent(i)!=0){
      reyl[i-1] = eyl[i-1]/hdata->GetBinContent(i);
      reyh[i-1] = eyh[i-1]/hdata->GetBinContent(i);
      if(1-reyl[i-1]<minr) minr = 0.-reyl[i-1];
      if(1+reyh[i-1]>maxr) maxr = 0.+reyh[i-1];
    }
    //cout<<eyh[i-1]<<"  "<<hdata->GetBinContent(i)<<"  "<<reyh[i-1]<<endl;
    
  }//end of loop over bins


  TGraphAsymmErrors* gsyst = new TGraphAsymmErrors(hdata->GetNbinsX(),x,y,exl,exh,eyl,eyh);
  
  double max = 0;
  for(int i=1;i<hdata->GetNbinsX();++i){
    max=x[i-1] + exh[i-1]+0.5;
    if(y[i]==0)
      break;
  }
  
  TCanvas* c_nsyst = new TCanvas("c_nsyst","c_nsyst",1000,500);
  c_nsyst->cd();
  
  gsyst->GetXaxis()->SetRangeUser(-0.5,max);
  gsyst->SetMinimum(5.);
  gsyst->GetXaxis()->SetTitle("n");//hdata->GetXaxis()->GetTitle());
  if(logY) gsyst->GetYaxis()->SetTitle("P_{n}");//hdata->GetXaxis()->GetTitle());
  else     gsyst->GetYaxis()->SetTitle("# events");
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

  
  hsyst1->SetLineColor(kBlue);
  hsyst1->Draw("same");
  if(fsyst2!=0){
    hsyst2->SetLineColor(kOrange);
    hsyst2->Draw("same");
  }


  //hdata->SetMarkerStyle(kOpenCircle);
  hdata->SetMarkerStyle(kDot);
  hdata->SetMarkerSize(0.1);
  hdata->SetLineColor(kRed);
  hdata->SetMarkerColor(kRed);
  hdata->Draw("psame");
  
  #include "../macro/acceptanceMap.C"
  ostringstream legheader("");
  legheader<< "pt > "<<accMap.at(cut).at(0)<<" GeV       |#eta| < "<<accMap.at(cut).at(1);
  TLegend* leg = new TLegend(0.20,0.2,0.40,0.40);
  leg->SetHeader(legheader.str().c_str());
  
  
  TString leg1 = "";
  TString leg2 = "";
  
  if(basesyst==0 && iDataType==0)  leg1 = "Data ";
  if(basesyst==0 && iDataType!=0)  leg1 = "MC ";
  if(basesyst==401)                leg1 += "unf=ATLAS@7 TeV";
  if(basesyst==402)                leg1 += "unf=D6T@7 TeV";
  if(basesyst==403)                leg1 += "unf=PHOJET@7 TeV";
  
  if(iDataType==11) leg1 += "DW";
  if(iDataType==12) leg1 += "P0";
  if(iDataType==13) leg1 += "ProQ20";
  if(iMC==20) leg1 += "unf=PHOJET@7 TeV";
  
                               leg2 = "N.A.";
  if((syst==0 || syst>=400) && iDataType==0)  leg2 = "Data ";
  if((syst==0 || syst>=400) && iDataType!=0)  leg2 = "MC ";
  
  if(iDataType==11) leg2 += "DW ";
  if(iDataType==12) leg2 += "P0 ";
  if(iDataType==13) leg2 += "ProQ20 ";
  
  if(syst==4)     leg2 = "tracking syst";
  if(syst==102)   leg2 = "SD sub syst";
  if(syst==200)   leg2 = "evtSel syst";
  
  if(syst==401) leg2 += "unf=ATLAS@7 TeV";
  if(syst==402) leg2 += "unf=D6T@7 TeV";
  if(syst==403) leg2 += "unf=PHOJET@7 TeV";
  if(syst==411) leg2 += "unf=DW";
  if(syst==412) leg2 += "unf=P0";
  if(syst==413) leg2 += "unf=ProQ20";
  if(syst==420) leg2 += "unf=PHOJET@7 TeV";
  if(syst==430) leg2 += "unf=D6T@7 TeV";
  if(syst==431) leg2 += "unf=ATLAS@7 TeV";
  
  if(syst==1000) leg2 += "hyp=uniform";
  
  if(syst==0)   leg2 += systleg;
   
  leg->AddEntry(hdata,leg1,"l");
  leg->AddEntry(gsyst,leg2,"f");
  
  
  leg->SetFillColor(kWhite);
  leg->Draw("same");
  
  //gPad->WaitPrimitive();
  
  //writing the files in gif
  /*ostringstream figname("");
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
  
  figname.str("");
  figname << "../figs/" << energy << "Tev/" << "systematics/";
  figname << histname << "__" <<filename.ReplaceAll(".root","");
  if(logY) figname << "_logY";
  cout<<figname.str()<<endl;
  gPad->SaveAs(TString(figname.str().c_str())+TString(".eps"),"");
  gSystem->Exec(TString("convert ")+TString(figname.str().c_str())+TString(".eps ")+TString(figname.str().c_str())+TString(".pdf"));
  
  figname.str("");
  figname << "../figs/" << energy << "Tev/" << "systematics/";
  figname << histname << "__" <<filename.ReplaceAll(".root","");
  if(logY) figname << "_logY";
  figname << ".root";
  cout<<figname.str()<<endl;
  gPad->SaveAs(figname.str().c_str(),"");
  */
  
  //---------------------
  //plotting the ratio
  TCanvas* c_rsyst = new TCanvas("c_rsyst","c_rsyst",1000,500);
  c_rsyst->cd();
  
  TGraphAsymmErrors* rsyst = new TGraphAsymmErrors(hdata->GetNbinsX(),x,ry,exl,exh,reyl,reyh);
  
  TH1F* one = new TH1F("one","one",306,-5.5,300.5);
  for(int b = 1 ; b <= one->GetNbinsX() ; ++b) one->SetBinContent(b,0.);
  one->SetLineColor(kRed);
  one->GetXaxis()->SetRangeUser(-0.5,max);
  //one->GetYaxis()->SetRangeUser(0.4,1.4);
  //one->GetYaxis()->SetRangeUser(minr*1.02,maxr*1.02);
  one->GetYaxis()->SetRangeUser(-1,1);
  cout<<minr<<"  "<<maxr<<endl;
  one->GetXaxis()->SetTitle("n");
  one->GetYaxis()->SetTitle("\\sigma_{syst}");
  one->Draw("hist");
  
  rsyst->SetFillColor(16);
  rsyst->Draw("2 same");
  
  one->Draw("histsame");
  
  ostringstream txt("");
  txt<<"CMS "<<energy<<" TeV";
  TLatex* text = new TLatex(0.4,0.8,txt.str().c_str());
  text->SetNDC(kTRUE);
  text->SetTextSize(0.06);
  text->DrawLatex(0.4,0.8,txt.str().c_str());
  
  
  
  leg = new TLegend(0.20,0.2,0.40,0.40);
  leg->SetHeader(legheader.str().c_str());
  
  
  leg1 = "";
  leg2 = "";
  
  if(basesyst==0 && iDataType==0)  leg1 = "Data ";
  if(basesyst==0 && iDataType!=0)  leg1 = "MC ";
  if(basesyst==401)                leg1 += "unf=ATLAS@7 TeV";
  if(basesyst==402)                leg1 += "unf=D6T@7 TeV";
  if(basesyst==403)                leg1 += "unf=PHOJET@7 TeV";
  
  if(iDataType==11) leg1 += "DW";
  if(iDataType==12) leg1 += "P0";
  if(iDataType==13) leg1 += "ProQ20";
  if(iMC==20) leg1 += "unf=PHOJET@7 TeV";
  
                               leg2 = "N.A.";
  if((syst==0 || syst>=400) && iDataType==0)  leg2 = "Data ";
  if((syst==0 || syst>=400) && iDataType!=0)  leg2 = "MC ";
  
  if(iDataType==11) leg2 += "DW ";
  if(iDataType==12) leg2 += "P0 ";
  if(iDataType==13) leg2 += "ProQ20 ";
  
  if(syst==4)     leg2 = "tracking syst";
  if(syst==102)   leg2 = "SD sub syst";
  if(syst==200)   leg2 = "evtSel syst";
  
  if(syst==401) leg2 += "unf=ATLAS@7 TeV";
  if(syst==402) leg2 += "unf=D6T@7 TeV";
  if(syst==403) leg2 += "unf=PHOJET@7 TeV";
  if(syst==411) leg2 += "unf=DW";
  if(syst==412) leg2 += "unf=P0";
  if(syst==413) leg2 += "unf=ProQ20";
  if(syst==420) leg2 += "unf=PHOJET@7 TeV";
  if(syst==430) leg2 += "unf=D6T@7 TeV";
  if(syst==431) leg2 += "unf=ATLAS@7 TeV";
  
  if(syst==1000) leg2 += "hyp=uniform";
  
  if(syst==0)   leg2 += systleg;
  
  leg->AddEntry(one,leg1,"l");
  leg->AddEntry(rsyst,leg2,"f");
  
  
  leg->SetFillColor(kWhite);

  leg->Draw("same");
  
  
  gPad->WaitPrimitive();
  return;
  //writing the files in gif
  logY=0;
  ostringstream figname("");
  figname << "../figs/systematics/";
  if (!gSystem->OpenDirectory(figname.str().c_str())) gSystem->mkdir(figname.str().c_str(),true);
  TString filename = fsyst1->GetName();
  TString histname = hsyst1->GetName();
  filename.Remove(0,filename.Last('/')+1);
  histname.Remove(0,histname.Last('/')+1);
  
  if(basesyst!=0) figname<<"basesyst"<<basesyst<<"_";
  if(iDataType!=0) figname<<"DataType"<<iDataType<<"_";
  figname << "syst"<<SYST<<"_cut"<<cut<<"_"<<Estr;
  if(logY) figname << "_logY";
  figname << ".gif";
  cout<<figname.str()<<endl;
  c_rsyst->SaveAs(figname.str().c_str(),"");
  
  figname.str("");
  figname << "../figs/systematics/";
  if(basesyst!=0) figname<<"basesyst"<<basesyst<<"_";
  if(iDataType!=0) figname<<"DataType"<<iDataType<<"_";
  figname << "syst"<<SYST<<"_cut"<<cut<<"_"<<Estr;
  if(logY) figname << "_logY";
  cout<<figname.str()<<endl;
  c_rsyst->SaveAs(TString(figname.str().c_str())+TString(".eps"),"");
  gSystem->Exec(TString("convert ")+TString(figname.str().c_str())+TString(".eps ")+TString(figname.str().c_str())+TString(".pdf"));
  
  figname.str("");
  figname << "../figs/systematics/";
  if(basesyst!=0) figname<<"basesyst"<<basesyst<<"_";
  if(iDataType!=0) figname<<"DataType"<<iDataType<<"_";
  figname << "syst"<<SYST<<"_cut"<<cut<<"_"<<Estr;
  if(logY) figname << "_logY";
  figname << ".root";
  cout<<figname.str()<<endl;
  c_rsyst->SaveAs(figname.str().c_str(),"");
  
  
  
  //saving the other canvas
  figname.str("");
  figname << "../figs/systematics/";
  if (!gSystem->OpenDirectory(figname.str().c_str())) gSystem->mkdir(figname.str().c_str(),true);
  
  if(basesyst!=0) figname<<"basesyst"<<basesyst<<"_";
  if(iDataType!=0) figname<<"DataType"<<iDataType<<"_";
  figname << "systcurves"<<SYST<<"_cut"<<cut<<"_"<<Estr;
  if(logY) figname << "_logY";
  figname << ".gif";
  cout<<figname.str()<<endl;
  c_nsyst->SaveAs(figname.str().c_str(),"");
  
  figname.str("");
  figname << "../figs/systematics/";
  if(basesyst!=0) figname<<"basesyst"<<basesyst<<"_";
  if(iDataType!=0) figname<<"DataType"<<iDataType<<"_";
  figname << "systcurves"<<SYST<<"_cut"<<cut<<"_"<<Estr;
  if(logY) figname << "_logY";
  cout<<figname.str()<<endl;
  c_nsyst->SaveAs(TString(figname.str().c_str())+TString(".eps"),"");
  gSystem->Exec(TString("convert ")+TString(figname.str().c_str())+TString(".eps ")+TString(figname.str().c_str())+TString(".pdf"));
  
  figname.str("");
  figname << "../figs/systematics/";
  if(basesyst!=0) figname<<"basesyst"<<basesyst<<"_";
  if(iDataType!=0) figname<<"DataType"<<iDataType<<"_";
  figname << "systcurves"<<SYST<<"_cut"<<cut<<"_"<<Estr;
  if(logY) figname << "_logY";
  figname << ".root";
  cout<<figname.str()<<endl;
  c_nsyst->SaveAs(figname.str().c_str(),"");
  
}

void finaleSystematic(double energy = 0.9, int cut = 5 , int typePlotting = 0 , bool plotPoissErr = false , bool printFig = true){
  vector<int> syst;
  syst.push_back(4);
  syst.push_back(102);
  syst.push_back(200);
  if(energy==0.9 || energy==2.36){
    syst.push_back(500);
    syst.push_back(510);
  }
  //syst.push_back(411);
  //syst.push_back(1000);//always last !!
  
  gROOT->ProcessLine(".x cmsStyleRoot.C");
  
  if(energy==7.0) typeMC = 31;
  else            typeMC = 10;
  
  vector<TH1F> syst1;
  vector<TH1F> syst2;
  
  vector<TMoments> msyst1;
  vector<TMoments> msyst2;
  
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
  
  TMoments* mdata = (TMoments*) fdata->Get("unfolding/moments/moments");
  
  //TH1F* hdata_poierr = (TH1F*) fdata->Get("unfolding/nch_resampled");
  TH1F* hdata_mtxerr = NULL;
  if(typePlotting==0) hdata_mtxerr = (TH1F*) fdata->Get("unfolding/nch_histresampled;1");
  //else               hdata_mtxerr = (TH1F*) fdata->Get("unfolding/nch_resampled;2");
  else               hdata_mtxerr = (TH1F*) fdata->Get("unfolding/nch_mtxresampledPtr");
  TH1F* eff_evtSel = (TH1F*) fdata->Get("unfolding/eff_evtSel");
  hdata_mtxerr->Divide(eff_evtSel);
  //hdata_mtxerr->Scale(1./hdata_mtxerr->Integral());
   
  TH1F* dummy = new TH1F("dummy","dummy",1,0,1);
  
  TH1F* systp = new TH1F("nch_systp","nch_systp",hdata->GetNbinsX(),hdata->GetXaxis()->GetXbins()->GetArray());
  TH1F* systm = new TH1F("nch_systm","nch_systm",hdata->GetNbinsX(),hdata->GetXaxis()->GetXbins()->GetArray());
  
  
  //Getting all histogramms
  for(int i = 0 ; i < syst.size() ; ++i){
    TString tsyst1 = "";
    TString tsyst2 = "";
    
    //hyp syst
    if(syst.at(i)==1000){
      outstr.str("");
      outstr << "hyp" << 0 << "_niter" << 0 << "_cut" << cut << "_DataType" << 31;
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
    
    
    //Getting the moments
    TMoments* msyst1_tmp = (TMoments*) fsyst1->Get("unfolding/moments/moments");
    msyst1.push_back(*msyst1_tmp);
    
    if(fsyst2==0)
      msyst2.push_back(*mdata);
    else{
      TMoments* msyst2_tmp = (TMoments*) fsyst2->Get("unfolding/moments/moments");
      msyst2.push_back(*msyst2_tmp);
    }
    //msyst1.at(i).print();
    //msyst2.at(i).print();
    
    
    /*delete fsyst1;
    delete fsyst2;
    delete hsyst1;
    delete hsyst2;*/
    
    
  }//end of loop over syst files
  
  
  //-----------------------------------
  //Making the systematic
  Double_t* x   = new Double_t[hdata->GetNbinsX()];
  Double_t* y   = new Double_t[hdata->GetNbinsX()];
  Double_t* exl = new Double_t[hdata->GetNbinsX()];
  Double_t* exh = new Double_t[hdata->GetNbinsX()];
  Double_t* eyl = new Double_t[hdata->GetNbinsX()];
  Double_t* eyh = new Double_t[hdata->GetNbinsX()];
  
  Double_t* ry   = new Double_t[hdata->GetNbinsX()];
  Double_t* reyl = new Double_t[hdata->GetNbinsX()];
  Double_t* reyh = new Double_t[hdata->GetNbinsX()];
  Double_t* seyl = new Double_t[hdata->GetNbinsX()];
  Double_t* seyh = new Double_t[hdata->GetNbinsX()];
  Double_t* poisseyl = new Double_t[hdata->GetNbinsX()];
  Double_t* poisseyh = new Double_t[hdata->GetNbinsX()];
  Double_t* mtxeyl = new Double_t[hdata->GetNbinsX()];
  Double_t* mtxeyh = new Double_t[hdata->GetNbinsX()];
  double maxr = 0. , minr = 0.;

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
    
    systm->SetBinContent(i,eyl[i-1]);
    systp->SetBinContent(i,eyh[i-1]);
    
     //doing the ratio arrays
    ry[i-1] = 0;
    reyl[i-1] = reyh[i-1] = 0;
    seyl[i-1] = seyh[i-1] = 0;
    poisseyl[i-1] = poisseyh[i-1] = 0;
    mtxeyl[i-1] = mtxeyh[i-1] = 0;
    if(hdata->GetBinContent(i)!=0){
      reyl[i-1] = eyl[i-1]/hdata->GetBinContent(i);
      reyh[i-1] = eyh[i-1]/hdata->GetBinContent(i);
      seyl[i-1] = hdata->GetBinError(i)/hdata->GetBinContent(i);
      seyh[i-1] = hdata->GetBinError(i)/hdata->GetBinContent(i);
      
      poisseyl[i-1] = sqrt(hdata->GetBinContent(i))/hdata->GetBinContent(i);
      poisseyh[i-1] = sqrt(hdata->GetBinContent(i))/hdata->GetBinContent(i);
      mtxeyl[i-1] = hdata_mtxerr->GetBinError(i)/hdata->GetBinContent(i);
      mtxeyh[i-1] = hdata_mtxerr->GetBinError(i)/hdata->GetBinContent(i);
      cout<<i<<"  "<<hdata_mtxerr->GetBinError(i)<<endl;
      if(0.-reyl[i-1]<minr) minr = 0.-reyl[i-1];
      if(0.+reyh[i-1]>maxr) maxr = reyh[i-1];
      /*cout<<i<<"  "<<hdata_mtxerr->GetBinError(i)
          <<"  "<<hdata_poierr->GetBinError(i)<<endl;
      cout<<"  "<<pow(hdata_mtxerr->GetBinError(i),2)
          <<"  "<<pow(hdata_poierr->GetBinError(i),2)<<endl;
      cout<<"  "<<sqrt(pow(hdata_mtxerr->GetBinError(i),2) + pow(hdata_poierr->GetBinError(i),2))
          <<"  "<<hdata->GetBinError(i)<<endl;*/
    }
    
    //cout<<"lllllllllllll   "<<eyl[i-1]<<"  "<<eyh[i-1]<<endl;
    //eyl[i-1] = sqrt(eyl[i-1]);
    //eyh[i-1] = sqrt(eyh[i-1]);   
  }//end of loop over bins
  
  TGraphAsymmErrors* gsyst = new TGraphAsymmErrors(hdata->GetNbinsX(),x,y,exl,exh,eyl,eyh);
  gsyst->SetName(TString("g")+hdata->GetName()+TString("_syst"));
  gsyst->SetTitle(TString("g")+hdata->GetTitle()+TString("_syst"));
  fdata->cd("unfolding/");
  cout<<"          "<<gDirectory->GetName()<<endl;
  gsyst->Write(TString("g")+hdata->GetName()+TString("_syst"),TObject::kWriteDelete);
  
  systm->Write(0,TObject::kWriteDelete);
  systp->Write(0,TObject::kWriteDelete);
  
  double max = 0;
  for(int i=3;i<hdata->GetNbinsX();++i){
    max=x[i-1] + exh[i-1]+0.5;
    if(y[i]==0)
      break;
  }
  
 // if(energy==7) max = 180.5;
  
  /*gsyst->GetXaxis()->SetRangeUser(hdata->GetXaxis()->GetXmin(),max);
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
  gStyle->SetOptTitle(kFALSE);*/

  
  /*syst1.at(0).SetLineColor(kBlue);
  syst1.at(0).Draw("same");
  if(syst2.at(0).GetName() != "dummy"){
    syst2.at(0).SetLineColor(kOrange);
    syst2.at(0).Draw("same");
  }*/


  //hdata->SetMarkerStyle(kOpenCircle);
  /*hdata->SetMarkerStyle(kDot);
  hdata->SetMarkerSize(0.1);
  hdata->SetLineColor(kRed);
  hdata->SetMarkerColor(kRed);
  hdata->Draw("psame");*/
   TCanvas* c_rsyst = new TCanvas("c_rsyst","c_rsyst",1000,500);
  c_rsyst->cd();

  TGraphAsymmErrors* rsyst = new TGraphAsymmErrors(hdata->GetNbinsX(),x,ry,exl,exh,reyl,reyh);
  TGraphAsymmErrors* ssyst = new TGraphAsymmErrors(hdata->GetNbinsX(),x,ry,exl,exh,seyl,seyh);
  TGraphAsymmErrors* psyst = new TGraphAsymmErrors(hdata->GetNbinsX(),x,ry,ry,ry,poisseyl,poisseyh);
  TGraphAsymmErrors* msyst = new TGraphAsymmErrors(hdata->GetNbinsX(),x,ry,exl,exh,mtxeyl,mtxeyh);

  TH1F* one = new TH1F("one","one",306,-5.5,300.5);
  for(int b = 1 ; b <= one->GetNbinsX() ; ++b) one->SetBinContent(b,0.);
  one->SetLineColor(kRed);
  one->GetXaxis()->SetRangeUser(-0.5,max);
  //one->GetYaxis()->SetRangeUser(0.4,1.4);
  one->GetYaxis()->SetRangeUser(-1,1);
  //one->GetYaxis()->SetRangeUser(minr*1.05,maxr*1.05);
  cout<<minr<<"  "<<maxr<<endl;
  one->GetXaxis()->SetTitle("n");
  one->GetYaxis()->SetTitle("\\sigma(P_{n}) / P_{n}");
  one->Draw("hist");

  rsyst->SetFillColor(16);
  rsyst->Draw("2 same");

  ssyst->SetFillColor(12);
  ssyst->Draw("2 same");
  
  msyst->SetFillColor(kRed);
  if(typePlotting>0)
    msyst->Draw("2 same");
  
  if(plotPoissErr)
    psyst->Draw("[] same");
  
  #include "../macro/acceptanceMap.C"
  ostringstream legheader("");
  legheader<< "pt > "<<accMap.at(cut).at(0)<<" GeV       |#eta| < "<<accMap.at(cut).at(1);
  
  
  TLegend* leg = new TLegend(0.4,0.7,0.6,0.90);
  leg->SetHeader(legheader.str().c_str());
  
  if(typePlotting==0)  leg->AddEntry(ssyst,"Statistical errors","f");
  if(typePlotting==1)  leg->AddEntry(msyst,"Evt resampling","f");
  if(typePlotting==2)  leg->AddEntry(msyst,"Mtx resampling","f");
  if(typePlotting==1)  leg->AddEntry(ssyst,"+ Mtx resampling","f");
  if(typePlotting==2)  leg->AddEntry(ssyst,"+ Evt resampling","f");
  if(plotPoissErr==1)  leg->AddEntry(psyst,"Poissonian Errors","l");
  leg->AddEntry(rsyst,"+ Systematics","f");
  leg->SetFillColor(kWhite);
  leg->Draw("same");
  
  one->Draw("histsame");

  ostringstream txt("");
  txt<<"CMS "<<energy<<" TeV";
  TLatex* text = new TLatex(0.16,0.85,txt.str().c_str());
  text->SetNDC(kTRUE);
  text->SetTextSize(0.06);
  text->DrawLatex(0.16,0.85,txt.str().c_str());

  gPad->WaitPrimitive();
  
  if(plot.Contains("kno")) return; 
  
  if(printFig){
    
    if(energy==0.9)  Estr = "a";
    if(energy==2.36) Estr = "b";
    if(energy==7.0)  Estr = "c";

    ostringstream figname("");
    figname<<"../figs/finalesyst_cut"<<cut<<"_"<<Estr;
    gPad->SaveAs(TString(figname.str().c_str())+TString(".gif"),"");
    gPad->SaveAs(TString(figname.str().c_str())+TString(".root"),"");
    gPad->SaveAs(TString(figname.str().c_str())+TString(".eps"),"");
    gSystem->Exec(TString("convert ")+TString(figname.str().c_str())+TString(".eps ")+TString(figname.str().c_str())+TString(".pdf"));
  }
  

  //-------------------------------------------------------
  //--------- Making systematic of mean & moments ---------
  //-------------------------------------------------------
  
  for(int isyst = 0 ; isyst < syst1.size() ; ++isyst){
    if(TString(syst2.at(isyst).GetName()).Contains("dummy")){
      syst2.at(isyst) = makeMirrorSyst(hdata , syst1.at(isyst));
      TH1F* systPtr = &(syst2.at(isyst));
      TMoments* msyst2_tmp = new TMoments(systPtr);
      msyst2_tmp->ComputeMoments();
      msyst2.at(isyst) = *msyst2_tmp;
    }
    
    mdata->mean->_MeanSystM += pow(getMinSyst(mdata->mean->GetMean() , msyst1.at(isyst).mean->GetMean() , msyst2.at(isyst).mean->GetMean()) , 2);
    mdata->mean->_MeanSystP += pow(getMaxSyst(mdata->mean->GetMean() , msyst1.at(isyst).mean->GetMean() , msyst2.at(isyst).mean->GetMean()) , 2);
    
    //cout<<"syst "<<isyst<<endl;
    //cout<<getMinSyst(mdata->mean->GetMean() , msyst1.at(isyst).mean->GetMean() , msyst2.at(isyst).mean->GetMean())<<endl;
    //cout<<getMaxSyst(mdata->mean->GetMean() , msyst1.at(isyst).mean->GetMean() , msyst2.at(isyst).mean->GetMean())<<endl;
    
    mdata->mean->_RMSSystM += pow(getMinSyst(mdata->mean->GetRMS() , msyst1.at(isyst).mean->GetRMS() , msyst2.at(isyst).mean->GetRMS()) , 2);
    mdata->mean->_RMSSystP += pow(getMaxSyst(mdata->mean->GetRMS() , msyst1.at(isyst).mean->GetRMS() , msyst2.at(isyst).mean->GetRMS()) , 2);
    
    for(int m =0 ; m < mdata->nmoments ; ++m){
      mdata->csystmerr->at(m) += pow(getMinSyst(mdata->cmoments->at(m) , msyst1.at(isyst).cmoments->at(m) , msyst2.at(isyst).cmoments->at(m)) , 2);
      mdata->csystperr->at(m) += pow(getMaxSyst(mdata->cmoments->at(m) , msyst1.at(isyst).cmoments->at(m) , msyst2.at(isyst).cmoments->at(m)) , 2);
    
      mdata->fsystmerr->at(m) += pow(getMinSyst(mdata->fmoments->at(m) , msyst1.at(isyst).fmoments->at(m) , msyst2.at(isyst).fmoments->at(m)) , 2);
      mdata->fsystperr->at(m) += pow(getMaxSyst(mdata->fmoments->at(m) , msyst1.at(isyst).fmoments->at(m) , msyst2.at(isyst).fmoments->at(m)) , 2);
      
      mdata->ksystmerr->at(m) += pow(getMinSyst(mdata->kmoments->at(m) , msyst1.at(isyst).kmoments->at(m) , msyst2.at(isyst).kmoments->at(m)) , 2);
      mdata->ksystperr->at(m) += pow(getMaxSyst(mdata->kmoments->at(m) , msyst1.at(isyst).kmoments->at(m) , msyst2.at(isyst).kmoments->at(m)) , 2);
      
      mdata->hsystmerr->at(m) += pow(getMinSyst(mdata->hmoments->at(m) , msyst1.at(isyst).hmoments->at(m) , msyst2.at(isyst).hmoments->at(m)) , 2);
      mdata->hsystperr->at(m) += pow(getMaxSyst(mdata->hmoments->at(m) , msyst1.at(isyst).hmoments->at(m) , msyst2.at(isyst).hmoments->at(m)) , 2);
    } 
  }
  
  
  //------------------------------------------------------------------
  //------------------    adding the stat errors    ------------------
  mdata->mean->_MeanSystM += pow(mdata->mean->GetMeanError(), 2);
  mdata->mean->_MeanSystP += pow(mdata->mean->GetMeanError(), 2);
    
  mdata->mean->_RMSSystM  += pow(mdata->mean->GetRMSError(), 2);
  mdata->mean->_RMSSystP  += pow(mdata->mean->GetRMSError(), 2);
  
  
  for(int m =0 ; m < mdata->nmoments ; ++m){
    mdata->csystmerr->at(m) += pow(mdata->cstaterr->at(m) , 2);
    mdata->csystperr->at(m) += pow(mdata->cstaterr->at(m) , 2);
   
    mdata->fsystmerr->at(m) += pow(mdata->fstaterr->at(m) , 2);
    mdata->fsystperr->at(m) += pow(mdata->fstaterr->at(m) , 2);
  
    mdata->ksystmerr->at(m) += pow(mdata->kstaterr->at(m) , 2);
    mdata->ksystperr->at(m) += pow(mdata->kstaterr->at(m) , 2);
  
    mdata->hsystmerr->at(m) += pow(mdata->hstaterr->at(m) , 2);
    mdata->hsystperr->at(m) += pow(mdata->hstaterr->at(m) , 2);
  }
  
  
  //------------------------------------------------------------------
  //------------------        taking the SQRT       ------------------
  mdata->mean->_MeanSystM = sqrt(mdata->mean->_MeanSystM);
  mdata->mean->_MeanSystP = sqrt(mdata->mean->_MeanSystP);
    
  mdata->mean->_RMSSystM  = sqrt(mdata->mean->_RMSSystM);
  mdata->mean->_RMSSystP  = sqrt(mdata->mean->_RMSSystP);
  
  
  for(int m =0 ; m < mdata->nmoments ; ++m){
    mdata->csystmerr->at(m) = sqrt(mdata->csystmerr->at(m));
    mdata->csystperr->at(m) = sqrt(mdata->csystperr->at(m));
  
    mdata->fsystmerr->at(m) = sqrt(mdata->fsystmerr->at(m));
    mdata->fsystperr->at(m) = sqrt(mdata->fsystperr->at(m));
  
    mdata->ksystmerr->at(m) = sqrt(mdata->ksystmerr->at(m));
    mdata->ksystperr->at(m) = sqrt(mdata->ksystperr->at(m));
  
    mdata->hsystmerr->at(m) = sqrt(mdata->hsystmerr->at(m));
    mdata->hsystperr->at(m) = sqrt(mdata->hsystperr->at(m));
  }
  
  fdata->cd("unfolding/moments");
  mdata->Write("moments_syst",TObject::kWriteDelete);
  mdata->print();
  //fdata->Close();
}

void mptSyst(double energy = 0.9 , bool printFig = false){
  vector<int> syst;
  syst.push_back(4);
  syst.push_back(102);
  syst.push_back(200);
  //syst.push_back(411);
  //syst.push_back(1000);//always last !!
  
  TString plotdir = "../plots/";
  TString plot = "mptVSnch";
  
  if(energy==7.0) typeMC = 31;
  else            typeMC = 10;
  
  vector<TProfile> syst1;
  vector<TProfile> syst2;
  
  
  cout<<"------------------------------ NEW SYSTEMATIC CALCULATION ------------------------------"<<endl;
  
  //Taking the data stuff
  ostringstream strout("");
  strout<<"dataType0";
  TString tdata = fileManager(5,typeMC,energy,1,0,0,strout.str(),plotdir);
  TFile*  fdata = TFile::Open(tdata,"UPDATE");
  cout<<"Trying to open data file :"<<endl;
  cout<<tdata<<endl;
  
  if(fdata==0){
    cout<<"Data file is void. Please correct it. Exiting now ..."<<endl;
    return;
  }
  
  TProfile* hdata  = (TProfile*) fdata->Get(plot);
  if(hdata==0){
    cout<<"!! ERROR : plot in data file is void. Exiting now."<<endl;
    return;
  }
     
  TProfile* dummy = new TProfile("dummy","dummy",1,0,1);
  
  TProfile* systp = new TProfile("mptVSnch_systp","mptVSnch_systp",hdata->GetNbinsX(),hdata->GetXaxis()->GetXbins()->GetArray());
  TProfile* systm = new TProfile("mptVSnch_systm","mptVSnch_systm",hdata->GetNbinsX(),hdata->GetXaxis()->GetXbins()->GetArray());
  
  
  //Getting all histogramms
  for(int i = 0 ; i < syst.size() ; ++i){
    TString tsyst1 = "";
    TString tsyst2 = "";
    
    tsyst1 = fileManager(5,typeMC,energy,1,syst.at(i),1,strout.str(),plotdir);
    tsyst2 = fileManager(5,typeMC,energy,1,syst.at(i),-1,strout.str(),plotdir);
    
    
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

    TProfile* hsyst1 = (TProfile*) fsyst1->Get(plot);
    TProfile* hsyst2 = NULL;
    if(fsyst2!=0)
      hsyst2 = (TProfile*) fsyst2->Get(plot);
  
    if(hsyst1==0)
      cout<<"!! ERROR : plot in syst1 file is void. Exiting now."<<endl;
    if(fsyst2!=0 && hsyst2==0)
      cout<<"!! ERROR : plot in syst2 file is void. Exiting now."<<endl;
    
    if(hsyst1==0 || (fsyst2!=0 && hsyst2==0))
      continue;
    
    
    syst1.push_back(*hsyst1);
    if(hsyst2==0) syst2.push_back(*dummy);
    else syst2.push_back(*hsyst2);
    
  }//end of loop over systematics
  
  
  
  
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
      
      cout<<i<<"  "<<eyl[i-1]<<"  "<<eyh[i-1]<<endl;
      cout<<binc<<"  "<<sbinc1<<"  "<<sbinc2<<endl;
    }//end of loop over syst
    //cout<<eyl[i-1]<<"  "<< eyh[i-1]<<endl;
    eyl[i-1] = sqrt(eyl[i-1] + pow(hdata->GetBinError(i),2));
    eyh[i-1] = sqrt(eyh[i-1] + pow(hdata->GetBinError(i),2));
    
    systm->SetBinContent(i,eyl[i-1]);
    systp->SetBinContent(i,eyh[i-1]);
    
    //cout<<"lllllllllllll   "<<eyl[i-1]<<"  "<<eyh[i-1]<<endl;
    //eyl[i-1] = sqrt(eyl[i-1]);
    //eyh[i-1] = sqrt(eyh[i-1]);   
  }//end of loop over bins
  
  TGraphAsymmErrors* gsyst = new TGraphAsymmErrors(hdata->GetNbinsX(),x,y,exl,exh,eyl,eyh);
  gsyst->SetName(TString("g")+hdata->GetName()+TString("_syst"));
  gsyst->SetTitle(TString("g")+hdata->GetTitle()+TString("_syst"));
  
  fdata->cd();
  gsyst->Write(0,TObject::kWriteDelete);
}










double getMinSyst(double val , double syst1 , double syst2){
  if(syst1<val && syst1<syst2) return val - syst1;
  else if(syst1<val && syst1>syst2) return val - syst2;
  else if(syst1>=val && syst2 < val) return val - syst2;
  else return 0;
}

double getMaxSyst(double val , double syst1 , double syst2){
  if(syst1>val && syst1>syst2) return syst1 - val;
  else if(syst1>val && syst1<syst2) return syst2 - val;
  else if(syst1<=val && syst2 > val) return syst2 - val;
  else return 0;
}

TH1F makeMirrorSyst(TH1F* h , TH1F syst){
  TH1F* syst2 = new TH1F("systmirror","systmirror",h->GetNbinsX(),h->GetXaxis()->GetXbins()->GetArray());
  
  for(int i = 1 ; i <= h->GetNbinsX() ; ++i){
    double diff = h->GetBinContent(i) - syst.GetBinContent(i);
    syst2->SetBinContent(i,h->GetBinContent(i) + diff);
  }
  
  return *syst2;
}
