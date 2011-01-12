#ifndef _NCHMAKECORRECTIONS_
#define _NCHMAKECORRECTIONS_

#include "TMath.h"
#include "TH1F.h"
#include "TF1.h"
#include "TH1D.h"
#include "TH2F.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TLegend.h"
#include "TPad.h"
#include "TRandom.h"
#include "TGraphAsymmErrors.h"
#include "TProfile.h"
#include "TROOT.h"
#include "TDirectory.h"

#include "TFrame.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include "../plugins/TResponseMtx.h"
#include "../plugins/MultiPlots.h"
#include "../plugins/TMean.h"
#include "../plugins/TMoments.h"

#endif
using namespace std;
using namespace TMath;

int debug_ = 0;


const int matrixsize = 320; //bigger then number of bins of my matrix
bool useData = false;

TString st(TString input , int cut){
  stringstream out("");
  out<<cut;
  return input+"_cut"+out.str();
}

void transform2Matrix(TH2F* matrixhist,double matrix[][matrixsize]);
//TH1F resample(double matrix[][matrixsize], TH1F*, TH1F* , TH1F* , TH1F*, int = 5 , bool = false , TH1F* = NULL, bool = false);
void    makeEff(TH1F*,TH1F*,TH1F*);
double  getRMS(TH1F*);
double* Divide( const TArrayD* , double );

void divideByWidth(TH1F*);
void divideByWidth(TH1D*);
void divideByWidth(TH2F*);
void multiplyByWidth(TH1F*);
void multiplyByWidth(TH1D*);
void multiplyByWidth(TH2F*);


TGraphAsymmErrors h2g(TH1F*);

//iFileType,iDataType,Energy,iTracking,iSystType,iSystSign,STest
#include "../macro/fileManager.C"
#include "unfolding.cc"
#include "getNIter.C"
//#include "ptcorr_funcs.C"
//#include "resamplings.C"
//#include "moments.C"

#include "fitting_funcs.C"

//#include "makeFakeMatrix.C"

void makeCorrectionsSten(int typeMC = 60, TString mcfile ="", TString datafile ="", TString outputfile ="output", 
                         TString dirmc="", TString dirRECO="", TString lastpartmc="", 
                         TString lastpartRECO="", TString lastpartmc_nosel="", bool useData = true, 
                         int hyp=0, int niter=20, double Emc = 7.0, double Edata= 7.0); 
                         //int syst = 0, int syst_sign = 0, bool drawcanv = true, float mu = 14, float sigma = 15);


//////////////////////////////////////
//MAIN PROGRAM

//_____________________________________________________________________________
void NCHmakeCorrections(int typeMC = 60, int Nrunmc = 1000000, int Nrundata = 500000, double Emc = 7.0, double Edata = 7.0) {
    //only for ferenc tracking
    
    stringstream nEvtsmc("");     nEvtsmc     << "_" << Nrunmc ;
    stringstream nEvtsdata("");   nEvtsdata   << "_" << Nrundata ; 
    stringstream type("");        type        << ""  << typeMC;
    stringstream energymc("");    energymc    << "_"  << Emc;
    stringstream energydata("");  energydata  << "_"  << Edata;
    
    TString mcstr =type.str()+"_ferncTr_E"+energymc.str()+nEvtsmc.str();
    TString mcfile     = "../macro/output_MC"+mcstr+".root";
    TString datafile   = "../macro/output_data_ferncTr_E"+energydata.str()+nEvtsdata.str()+".root";
    TString outputfile = "../macro/unfold_MC"+mcstr+".root";
    cout << "MC   input file : " << mcfile  << endl;
    cout << "Data input file : " << datafile << endl;
    cout << "output file:      " << outputfile <<endl;
    
    TString dirmc  ="Difflvl_cut0/Centrlvl_INEL_cut0/HFlvl_nocut_INEL_cut0/EvtSel_HF0_nocut_INEL_cut0/";
    TString dirRECO="Difflvl_cut0/Centrlvl_RECO_cut0/HFlvl_nocut_RECO_cut0/EvtSel_HF0_nocut_RECO_cut0/";
    
    TString lastpartmc="_full_HF0_nocut_INEL_cut0";
    TString lastpartmcnosel="_noSel_HF0_nocut_INEL_cut0";
    TString lastpartRECO="_full_HF0_nocut_RECO_cut0";
    
    
    useData=true;  
    makeCorrectionsSten(60, mcfile, datafile, outputfile, dirmc, dirRECO, lastpartmc, lastpartRECO, lastpartmcnosel, useData, 0, 20, Emc, Edata);

}    /////////////////////////////////////


//_____________________________________________________________________________
void makeCorrectionsSten(int typeMC, TString mcfile, TString datafile, TString outputfile, 
                         TString dirmc, TString dirRECO, TString lastpartmc, 
                         TString lastpartRECO, TString lastpartmc_nosel, bool useData,
                         int hyp, int niter, double Emc, double Edata ) {
                         //int syst, int syst_sign, bool drawcanv, float mu, float sigma ){
               
  TFile* mc   = TFile::Open(mcfile  ,"READ");  
  TFile* data = TFile::Open(datafile,"READ");
  
  //Checking if both files exist
  if(mc == 0)   cout<<"The MC input file doesn't exist ..."<<endl;   
  if(data == 0) cout<<"The Data input file doesn't exist ..."<<endl;
  if(mc == 0 || data == 0)
    return;
  
    
  cout<<"Output file : " << "nfold_MC_TEMPORARY.root" << endl;
  TFile* out = new TFile(outputfile,"RECREATE");
  out->cd();  
  TDirectory* currentdir = gDirectory;
  
   
  //------------------------------------------------------------------------------
  //---------------------------- UNFOLDING ---------------------------------------
  //------------------------------------------------------------------------------
  
  gDirectory->mkdir("unfolding");
  gDirectory->cd("unfolding");
  if (debug_!=0) cout <<"make unfolding dir" <<endl;
  //Get the unfolding matrix
  TH2F* matrixhist=0;  
  matrixhist = (TH2F*) mc->Get(dirmc+"TResponseMtx"+lastpartmc+"/mtx"+lastpartmc);
  matrixhist->SetName("nch_matrix");   
 
  if(matrixhist==0) {
    cout << "Matrixhist is empty. Can't find: "<< dirmc+"TResponseMtx"+lastpartmc+"/mtx"+lastpartmc <<endl;
    return;
  }  
  if(debug_!=0) {
    cout << "Matrixhist loaded" << endl;
    matrixhist->Draw();
    gPad->WaitPrimitive();
  }
  

  //Setting the limits from the matrix
  Ngen1  = matrixhist->GetNbinsX();   //used in unfolding.cc
  Nreco1 = matrixhist->GetNbinsX();   //used in unfolding.cc


  //Transform the TH2F into an array
  matrix4dObj matrix;
  transform2Matrix(matrixhist,matrix);

  //get the true nch distribution (from MC)
  TH1F* nch_trueGen_afterUnfolding=0;
  nch_trueGen_afterUnfolding = (TH1F*) mc->Get(dirmc+"GenPart"+lastpartmc+"/nch"+lastpartmc);
  nch_trueGen_afterUnfolding->SetName("nch_MC_gen_afterUnfolding");

  if(nch_trueGen_afterUnfolding==0) {
    cout << "nch_MC_gen_afterUnfoldingINEL is empty"<<endl;
    return;
  } 
  if(debug_!=0) {
    cout << "nch_trueGen_afterUnfolding loaded" << endl;
    nch_trueGen_afterUnfolding->Draw();
    gPad->WaitPrimitive();
  } 


  //NOT USED AT THE MOMENT
  TH1F* nch_trueGen=0;  

  nch_trueGen = (TH1F*) mc->Get(dirmc+"GenPart"+lastpartmc_nosel+"/nch"+lastpartmc_nosel);
  nch_trueGen->SetName("nch_MC_gen_afterEvtSelCorrection");

  if(nch_trueGen==0) {
    cout << "nch_trueGenINEL is empty"<<endl;
    return;
  }
  
  if(debug_!=0) {
    cout << "nch_MC_Gen_afterEvtSelCorrection loaded" << endl;
    nch_trueGen->Draw();
    gPad->WaitPrimitive();
  }
  
 
  //get the nch to Unfold distribution   nchTRACKS
  TH1F* nch_REC=0;
  if(useData) {
    //the RECO Data tracks    
    nch_REC = (TH1F*)  data->Get(dirRECO+"Track"+lastpartRECO+"/nch"+lastpartRECO);
  }  
  else {
  // the RECO MC tracks (to check the unfolding procedure)
    nch_REC = (TH1F*)  mc  ->Get(dirRECO+"Track"+lastpartRECO+"/nch"+lastpartRECO);
  }
  if(nch_REC==0) {
    cout << "nch_REC data is empty"<<endl;
    return;
  }
  if(debug_!=0) {
    cout << "nch_REC data loaded" << endl;
    nch_REC->Draw();
    gPad->WaitPrimitive();
  }  

 
 
  TH1F* eff_evtSel=0;
  
  eff_evtSel = (TH1F*) mc->Get(dirRECO+"Eff"+lastpartRECO+"/eff_nch"+lastpartRECO);  
  if(eff_evtSel==0) {
    cout << "eff_evtSelRECO is empty"<<endl;
    return;
  }
  if(debug_!=0) {
    eff_evtSel->Draw();
    gPad->WaitPrimitive();
  }
  eff_evtSel->SetName("eff_evtSel");
  
  TH1F* nch_toUnfold = 0;
  nch_toUnfold =(TH1F*) nch_REC->Clone("nch_toUnfold");



  //------------------------------------------------------------------------------
  //---------------------------- Unfolding ---------------------------------------
  //------------------------------------------------------------------------------  
  cout<<" ++++ DOING UNFOLDING ++++" <<endl;
 
  TH1F nch_unfolded("","",1,0,1);
  TH1F* hypothesis = (TH1F*) nch_trueGen->Clone("hypothesis"); //is a reset
  if   (hyp == 0){
    for (int i=1;i<=hypothesis->GetNbinsX();i++)
      hypothesis->SetBinContent(i,hypothesis->GetBinWidth(i));
  }
  else if (hyp == 1)
    hypothesis = nch_trueGen_afterUnfolding;
       
  if(debug_!=0) {
    hypothesis->Draw();
    gPad->WaitPrimitive();
  }        
  //calling the unfolding.cc file
  nch_unfolded = (runalgo(matrixhist,nch_REC,hypothesis,niter));
   
  TH1F* nch_unfoldedPtr = (TH1F*) nch_unfolded.Clone("nch_unfoldedPtr");

  cout<< nch_unfoldedPtr->GetBinContent(31) << "  " << nch_unfoldedPtr->GetBinError(31) << endl;
 
  //------------------------------------------------------------------------------
  //---------------------------- ReScaling -------------------------------------
  //------------------------------------------------------------------------------
/*  cout<<" ++++ DOING RESCALING ++++" <<endl;
  
  nch_unfoldedPtr->Scale(1./double(mp_INC_evtSel_reco_data->nbEvts));
  nch_REC->Scale(1./double(mp_INC_evtSel_reco_data->nbEvts));
  if(nch_unfoldedPtr->Integral()<2)
    MC_factor = 1. / double(mp_INC_evtSel_reco_MC->nbEvts) ;
  else
    MC_factor = double(mp_INC_evtSel_reco_data->nbEvts) / double(mp_INC_evtSel_reco_MC->nbEvts) ;
*/  
     
  //------------------------------------------------------------------------------
  //---------------------------- Resampling & moments ----------------------------
  //------------------------------------------------------------------------------  
/*  cout<<" ++++ DOING RESAMPLING & MOMENTS' STAT ERRORS ++++" <<endl;
  
  TMoments* moment = new TMoments();
  
  gDirectory->mkdir("hist_resampling");
  gDirectory->cd("hist_resampling");

  int niter_resampling = 100;
  if(syst!=0) niter_resampling = 1;
  
  cout << "WARNING !! The resampling is done with " << niter_resampling << " iterations ..." << endl;

  TH1F nch_resampled = resample(matrixhist,nch_REC,nch_NSD,nch_unfoldedPtr,hypothesis,niter_resampling,0,nch_evtSel_SD,moment,eff_evtSel,false);
  TH1F* nch_resampledPtr = &nch_resampled;
  
  gDirectory->cd("../");
  
  if(drawcanv){
    TCanvas* c_resample = new TCanvas("resampling","resampling",1460,510,500,500);
    c_resample->cd();
    nch_unfoldedPtr->SetLineColor(kBlack);
    nch_unfoldedPtr->Draw("hist");
    nch_resampledPtr->Draw("e");
    //gPad->WaitPrimitive();
  }
          
  gDirectory->mkdir("mtx_resampling");
  gDirectory->cd("mtx_resampling");
    
  TH1F nch_mtxresampled = mtxresample(matrixhist,nch_toUnfold,hypothesis,niter_resampling,moment,eff_evtSel);
  TH1F* nch_mtxresampledPtr = &nch_mtxresampled;
  
  gDirectory->cd("../");
*/  
    
/*  for(int nbin = 1 ; nbin<=nch_unfoldedPtr->GetNbinsX() ; ++nbin){
    nch_unfoldedPtr->SetBinError(nbin , sqrt(pow(nch_resampledPtr->GetBinError(nbin),2)+pow(nch_mtxresampledPtr->GetBinError(nbin),2)));
    }
*/
  
  
  //------------------------------------------------------------------------------
  //---------------------------- EvtSel Correction -------------------------------
  //------------------------------------------------------------------------------
  
  
  cout<<" ++++ DOING EVT SEL CORRECTION ++++" <<endl;
  
  TH1F* nch_corrected = (TH1F*) nch_unfoldedPtr->Clone("nch_data_corrected");
  //eff_evtSel->Sumw2();
  
  //including the systematic for evtSel Eff
  //#include "syst_evtSelEff.C"
    
  nch_corrected->Divide(nch_corrected,eff_evtSel,1,1);
  /*
  nch_corrected->Scale(1./nch_corrected->Integral());
  nch_trueGen->Scale(1./nch_trueGen->Integral());
  nch_trueGen_afterUnfolding->Scale(1./nch_trueGen_afterUnfolding->Integral());*/


  //------------------------------------------------------------------------------
  //--------------------------   Adding Stat Errors   ----------------------------
  //--------------------------   to the final curve   ----------------------------
  //------------------------------------------------------------------------------
/*  
  for(int nbin = 1 ; nbin<=nch_corrected->GetNbinsX() ; ++nbin){
    nch_corrected->SetBinError(nbin , sqrt(pow(nch_resampledPtr->GetBinError(nbin),2)+pow(nch_mtxresampledPtr->GetBinError(nbin),2)));
    //cout<<nbin<<"  "<<nch_resampledPtr->GetBinError(nbin)<<"  "<<nch_mtxresampledPtr->GetBinError(nbin)<<endl;
    //cout<<"  "<<sqrt(pow(nch_resampledPtr->GetBinError(nbin),2)+pow(nch_mtxresampledPtr->GetBinError(nbin),2))
    //    <<"  "<<nch_unfoldedPtr->GetBinError(nbin)<<endl;
  }
  
*/  
  
  //------------------------------------------------------------------------------
  //--------------------------------- Rescaling ----------------------------------
  //------------------------------------------------------------------------------
  //cout<<" ++++ RESCALING ++++" <<endl;  
  
  //------------------------------------------------------------------------------
  //--------------------------------- Moments ------------------------------------
  //------------------------------------------------------------------------------     
  //cout<<" ++++ MOMENTS ++++" <<endl;
  
  //------------------------------------------------------------------------------
  //---------------------------------- Plots -------------------------------------
  //------------------------------------------------------------------------------
  //cout<<" ++++ PLOTS ++++" <<endl;
  
  //------------------------------------------------------------------------------
  //---------------------------------- Fits -------------------------------------
  //------------------------------------------------------------------------------  
  //cout<<" ++++ FITS ++++" <<endl;  
  //#include "fitting.C"  

  
  //Unfolding
  hypothesis->Write("hypothesis");
  nch_trueGen->Write();
  nch_trueGen_afterUnfolding->Write();
  nch_REC->Write();
  nch_unfoldedPtr->Write();
  nch_corrected->Write();
  TH1F* nch_cor_norm = (TH1F*) nch_corrected->Clone("nch_corrected_norm");
  nch_cor_norm->Scale(10000*1./nch_cor_norm->Integral());    //NEED TO ADJUST with bin !=1
  nch_cor_norm->Write();
  eff_evtSel->Write();
  
  
  matrixhist->Write();
  
  gDirectory->cd("../");

  
  //------------------------------------------------------------------------------
  //-------------------------------- Corrections ---------------------------------
  //------------------------------------------------------------------------------
  //cout<<" ++++ CORRECTIONS ++++" <<endl;

}

//_____________________________________________________________________________
void transform2Matrix(TH2F* matrixhist,double matrix[][matrixsize]){
  
  //j-->RECO || i-->GEN
  for(int i=0;i<matrixsize;++i){
    for(int j=0;j<matrixsize;++j){
      if(i<matrixhist->GetNbinsX() && j<matrixhist->GetNbinsY())
        matrix[j][i] = matrixhist->GetBinContent(i+1,j+1);
      else
        matrix[j][i] = 0;
    }
  }
}

//_____________________________________________________________________________
void makeKNO(TH1F* multi,TH1F* kno){
  for( int k = 1 ; k <= multi->GetNbinsX() ; ++k){
    kno->SetBinContent(k , multi->GetMean() * multi->GetBinContent(k));
    kno->SetBinError(k , multi->GetMean() * multi->GetBinError(k));
  }
}


//_____________________________________________________________________________
void makeEff(TH1F* eff , TH1F* num , TH1F* denom){
  for(int i=1;i<=num->GetNbinsX();i++){
    double n = num->GetBinContent(i);
    double d = denom->GetBinContent(i);
    if( d!=0 ){
      double keff = n / d;
      double kerr = sqrt(keff*(1-keff)/ d);
      eff->SetBinContent(i,keff);
      eff->SetBinError(i,kerr);
    }
  }
}

//_____________________________________________________________________________
double getRMS(TH1F* in){
  double m  = 0;
  double m2 = 0;
  for(int i = 1 ; i <= in->GetNbinsX() ; ++i){
    m+=in->GetBinContent(i)*in->GetBinCenter(i);
    m2+=in->GetBinContent(i)*pow(in->GetBinCenter(i),2);
  }
  m/=double(in->Integral());
  
  double sig1 = sqrt( m2/double(in->Integral()) - pow(m,2)  );
  
  /*double sig2 = 0;
  for(int i = 1 ; i <= in->GetNbinsX() ; ++i)
    sig2+=in->GetBinContent(i)*pow(in->GetBinCenter(i)-m,2);
  sig2/=double(in->Integral());
  sig2=sqrt(sig2);
  
  cout<<"means (Mine, root): "<<m<<"  "<<in->GetMean()<<endl;;
  cout<<"RMSs  (Mine1,Mine2,root,sqrt(mean)): "<<sig1<<"  "<<sig2<<"  "<<in->GetRMS()<<"  "<<sqrt(in->GetMean())<<endl;
  */
  
  return sig1;
}

//_____________________________________________________________________________
double* Divide(const TArrayD* array , double val){
  TArrayD* temp = new TArrayD();
  array->Copy(*temp);
  for(int i = 0 ; i < temp->GetSize() ; i++)
    temp->SetAt(array->At(i) / val , i);
  
  return temp->GetArray();  
}

//_____________________________________________________________________________
void divideByWidth(TH1F* hist){
  for(int i=1;i<=hist->GetNbinsX();++i){
    hist->SetBinContent(i,double(hist->GetBinContent(i))/double(hist->GetBinWidth(i)));
    hist->SetBinError(i,double(hist->GetBinError(i))/double(hist->GetBinWidth(i)));
  }
}

//_____________________________________________________________________________
void divideByWidth(TH1D* hist){
  for(int i=1;i<=hist->GetNbinsX();++i){
    hist->SetBinContent(i,double(hist->GetBinContent(i))/double(hist->GetBinWidth(i)));
    hist->SetBinError(i,double(hist->GetBinError(i))/double(hist->GetBinWidth(i)));
  }
}

//_____________________________________________________________________________
void divideByWidth(TH2F* hist){
  double width = 1;
  for(int i=1;i<=hist->GetNbinsX();++i){
    for(int j=1;j<=hist->GetNbinsX();++j){
      width = hist->GetXaxis()->GetBinWidth(i) * hist->GetYaxis()->GetBinWidth(j);
      hist->SetBinContent(i,j,double(hist->GetBinContent(i,j))/width);
      hist->SetBinError(i,j,double(hist->GetBinError(i,j))/width);
    }
  }
}

//_____________________________________________________________________________
void multiplyByWidth(TH1F* hist){
  for(int i=1;i<=hist->GetNbinsX();++i){
    hist->SetBinContent(i,double(hist->GetBinContent(i))*double(hist->GetBinWidth(i)));
    hist->SetBinError(i,double(hist->GetBinError(i))*double(hist->GetBinWidth(i)));
  }
}

//_____________________________________________________________________________
void multiplyByWidth(TH1D* hist){
  for(int i=1;i<=hist->GetNbinsX();++i){
    hist->SetBinContent(i,double(hist->GetBinContent(i))*double(hist->GetBinWidth(i)));
    hist->SetBinError(i,double(hist->GetBinError(i))*double(hist->GetBinWidth(i)));
  }
}

//_____________________________________________________________________________
void multiplyByWidth(TH2F* hist){
  double width = 1;
  for(int i=1;i<=hist->GetNbinsX();++i){
    for(int j=1;j<=hist->GetNbinsX();++j){
      width = hist->GetXaxis()->GetBinWidth(i) * hist->GetYaxis()->GetBinWidth(j);
      hist->SetBinContent(i,j,double(hist->GetBinContent(i,j))*width);
      hist->SetBinError(i,j,double(hist->GetBinError(i,j))*width);
    }
  }
}

/*
//From websight
//_____________________________________________________________________________
double NBD(double x, double nmean, double k){
  double p = 1. / ( (nmean / k) + 1 );
  return Gamma(x+nmean)/( Gamma(x+1) * Gamma(nmean) ) * pow(p,k) * pow ( 1 - p , x);
}

//_____________________________________________________________________________
double NBD4(int x, double nmean, int k){
  double p = 1. / ( (nmean / k) + 1 );
  return Binomial(x + k -1, k - 1) * pow(p,k) * pow ( 1 - p , x);
}
*/
