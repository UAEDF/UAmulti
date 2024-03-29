//
//  This example has been taken from the ROOT tutorials website
//  and has been modified by P.Castro 14th July, 2003
//
//   Example of a program to fit non-equidistant data points
//   =======================================================
//
//   The fitting function fcn is a simple chisquare function
//   The data consists of 5 data points (arrays x,y,z) + the errors in errorsz
//   More details on the various functions or parameters for these functions
//   can be obtained in an interactive ROOT session with:
//    Root > TMinuit *minuit = new TMinuit(10);
//    Root > minuit->mnhelp("*")  to see the list of possible keywords
//    Root > minuit->mnhelp("SET") explains most parameters
//

#include <TROOT.h>
#include <TMinuit.h>
#include "TDirectory.h"
#include "TFrame.h"
#include "TCanvas.h"
#include "TLegend.h"

#include "TH1F.h"
#include "TF1.h"
#include "TH2F.h"
#include "TFile.h"

#include <iostream>
using namespace std;
//#include <math.h>
#include "TMath.h"
using namespace TMath;

#include "NCHmptPlotting.C"
#include "../macro/files_funcs.C"
#include "NCHgetPtCorrFactors.C"
#include "NCHincreaseNTracks.C"


bool debug_=false;
const bool normalize=false;

static Double_t p0=0;
static Double_t p1=1;
static Double_t p3=3;
static Double_t p5=5;
static Double_t p6=6;
   
 
const int matrixsize = 380; //bigger then number of bins of my matrix
double matrix_normalized[matrixsize][matrixsize];
void transform2Matrix(const TH2F* ,double matrix[][matrixsize]);
bool checkMatrix(double inputMat[][matrixsize],double inputMatNormalized[][matrixsize]);
double getChi2(const TH1F*, const TH1F*, int errortype);

const int iNum = 127;   //=== GetNBinsX() on the TH1
double ptcorr_val = 0.;
TH1F* data_curve = NULL; 
TH1F* data_corrected = NULL; 
TH1F* eff_beforeUnf = NULL; 
TH1F* eff_afterUnf = NULL; 

// this is the function used for the fit
//   par: vector with the fit parameters
//_____________________________________________________________________________
double NBD(double xx, double nmean, double k) {
  double p = 1. / ( (nmean / k) + 1 );
  return Exp(LnGamma(xx+k) - ( LnGamma(xx+1) + LnGamma(k) )) * pow(p,k) * pow ( 1 - p , xx);
}

//_____________________________________________________________________________
Double_t fit_function(double xx, double* par){
  return par[5]*(par[0]*NBD(xx,par[1],par[2])+(1-par[0])*NBD(xx,par[3],par[4]));
}
//_____________________________________________________________________________
Double_t doubleNBD(double* xx, double* par){
  return par[5]*(par[0]*NBD(xx[0],par[1],par[2])+(1-par[0])*NBD(xx[0],par[3],par[4]));
}

//_____________________________________________________________________________
Double_t uncorrected_doubleNBD(Double_t* x , Double_t* par){

  //cout << par[0] << "  " << par[1] << "  " << par[2] << "  " << par[3] << "  " << par[4] << endl;
   debug_ = true; 
   TH1F* fit = (TH1F*) data_curve->Clone("fit");
   fit->Reset();
   TH1F* fit_reco =(TH1F*) data_curve->Clone("fit_reco");
   fit_reco->Reset();
   
   //optimize this
   //TAxis* xAxis = (TAxis*) data_curve->GetXaxis();
   const double nbins = data_curve->GetNbinsX();
   
   // set the curve coming from the generated values
   for ( int i = 3 ; i <= nbins ; ++i ){
       fit->SetBinContent( i , fit_function( fit->GetBinCenter(i) , par) );
   }  
   if(normalize==true) fit->Scale(1/fit->Integral());
   
   if(debug_){
     data_corrected->Draw();
     data_curve->Draw("same");
     TH1F* fit_draw1 = (TH1F*) fit->Clone("fit_draw1");
     fit_draw1->SetLineColor( kRed );
     fit_draw1->Draw("same");
     //gPad->WaitPrimitive();
   }
   
   fit->Multiply(fit, eff_afterUnf , 1 , 1);
   
   if(debug_){
     TH1F* fit_draw2 = (TH1F*) fit->Clone("fit_draw2");
     fit_draw2->SetLineColor( kBlue );
     fit_draw2->Draw("same");
     //gPad->WaitPrimitive();
   }
   
   //Backward unfolding
   for(int j = 0 ; j < nbins ; ++j){
     double auxFill = 0.; 
     //double norm = 0;
     for(int i = 0 ; i < nbins ; ++i){
       auxFill += fit->GetBinContent(i+1) * matrix_normalized[j][i];   
       //norm += matrix_normalized[j][i];
     }
            
     fit_reco->SetBinContent(j+1,auxFill);
     //cout << j << "  " << fit->GetBinContent(j+1) << "  " << norm << "  " << auxFill << endl;  
   }
   
   if(debug_){
     TH1F* fit_draw3 = (TH1F*) fit_reco->Clone("fit_draw3");
     fit_draw3->SetLineColor( kGreen );
     fit_draw3->Draw("same");
     //gPad->WaitPrimitive();
   }
   
   fit_reco->Multiply(fit_reco , eff_beforeUnf , 1 , 1);
   
   if(debug_){
     TH1F* fit_draw4 = (TH1F*) fit_reco->Clone("fit_draw4");
     fit_draw4->SetLineColor( kMagenta );
     fit_draw4->Draw("same");
     gPad->WaitPrimitive();
   }
   
   if(normalize==true) fit_reco->Scale(data_curve->Integral()/fit_reco->Integral());    
   
   if(debug_) {
      cout << "Chi2: " << getChi2( data_curve, fit_reco, 1) << endl;
   }
    
  Double_t val = fit_reco->GetBinContent( fit_reco->GetXaxis()->FindFixBin( x[0] ) );
  
  debug_ = false; 
  
  delete fit;
  delete fit_reco;
  
  return val;
}

//_____________________________________________________________________________
void calc_chi_square(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t iflag){
    
   TH1F* fit = (TH1F*) data_curve->Clone("fit");
   fit->Reset();
   TH1F* fit_reco =(TH1F*) data_curve->Clone("fit_reco");
   fit_reco->Reset();
   
   //optimize this
   //TAxis* xAxis = (TAxis*) data_curve->GetXaxis();
   const double nbins = data_curve->GetNbinsX();
   
   // set the curve coming from the generated values
   for ( int i = 1 ; i <= nbins ; ++i ){
       fit->SetBinContent( i , fit_function( fit->GetBinCenter(i) , par) );
   } 
   
   if(ptcorr_val) increaseNTracks( fit , -1 , ptcorr_val , 0); 
   
   if(normalize==true) fit->Scale(1/fit->Integral());
   
   fit->Multiply(fit, eff_afterUnf , 1 , 1);
   
   //Backward unfolding
   for(int j = 0 ; j < nbins ; ++j){
      double auxFill = 0.; 
      for(int i = 0 ; i < nbins ; ++i)
          auxFill += fit->GetBinContent(i+1) * matrix_normalized[j][i];      
      fit_reco->SetBinContent(j+1,auxFill);
   }
   
   fit_reco->Multiply(fit_reco , eff_beforeUnf , 1 , 1);
   
   if(normalize==true) fit_reco->Scale(data_curve->Integral()/fit_reco->Integral());    
   
   if(debug_) {
      cout << "Chi2: " << getChi2( data_curve, fit_reco, 1) << endl;
      TH1F* data_curve_clone = (TH1F*) data_curve->Clone("data_curve"); //data_curve is const, otherwise not able to Draw()
      fit_reco->SetLineColor(kRed);
      fit_reco->SetMarkerColor(kRed);
      fit_reco->Draw();
      data_curve_clone->Draw("same");     
      gPad->WaitPrimitive();
    }
   f = getChi2( data_curve, fit_reco, 1);
 
  delete fit;
  delete fit_reco;
  return;
}

//_____________________________________________________________________________
double getChi2(const TH1F* hist1 , const TH1F* hist2 , int errortype){
  double bin = 0 , error = 0 , sumbin = 0 , ndof = 0;
  
  //skip the first 2 bins + the underflow bin
  for(int i=5 ; i <= hist1->GetNbinsX() ; ++i){
    if(hist2->GetBinContent(i)!=0){
      ++ndof;
      bin = pow( hist1->GetBinContent(i) - hist2->GetBinContent(i) , 2 );
      if(errortype==0) error = fabs(hist1->GetBinContent(i));
      if(errortype==1) error = pow(hist1->GetBinError(i),2);
      if(error!=0) sumbin += bin / error ;
      else sumbin+=bin;
    }
  }
  return (ndof!=0) ? sumbin / ndof : -1;
}





//_____________________________________________________________________________
void NCHMinuitNBD_f(TString file_str = "files/unfold_outputs/v62/unf_MC60__allEffs_partfull_HF0_nocut_INEL_cut0.root"){

  TH1::AddDirectory(0); //Detach histograms from rootfiles (they are no closed if the file is closed.)
  
  TFile* file = getFile( file_str ); 

  TH2F* matrixhist = getHist<TH2F>( *file , "nch_matrix");
  TH1F* curve      = getHist<TH1F>( *file , "nch_data_raw"); 
  TH1F* eff_before = getHist<TH1F>( *file , "eff_before_unf"); 
  TH1F* eff_after  = getHist<TH1F>( *file , "eff_after_unf"); 
  data_corrected   = getHist<TH1F>( *file , "nch_data_corrected");
  delete file;
 
  //Putting to global var, so that they are accessible from within other funcs
  data_curve=curve; 
  eff_beforeUnf=eff_before;
  eff_afterUnf=eff_after;
  
  //Transform the TH2F into an array
  double matrix [matrixsize][matrixsize];
  transform2Matrix( matrixhist, matrix);
  
  //don't normalize
  checkMatrix(matrix, matrix_normalized);
  delete matrixhist;
  
  ptcorr_val = getPtCorrFactor("nch");
  if(ptcorr_val) cout << "Using pt correction factor:  " << ptcorr_val << endl;

  TMinuit *ptMinuit = new TMinuit(6);  //initialize TMinuit with a maximum of 5 params
  
  //
  //  select verbose level:
  //    default :     (58 lines in this test)
  //    -1 : minimum  ( 4 lines in this test)
  //     0 : low      (31 lines)
  //     1 : medium   (61 lines)
  //     2 : high     (89 lines)
  //     3 : maximum (199 lines in this test)
  //
  ptMinuit->SetPrintLevel(-1);
  
  // set the user function that calculates chi_square (the value to minimize)
  ptMinuit->SetFCN(calc_chi_square);

  Double_t arglist[10];
  Int_t ierflg = 0;

  //ptMinuit->mnexcm("SET ERR", &p1 ,1,ierflg);
  cout << "Return of mnexcm() func:  " << ierflg << endl;

  // Set starting values and step sizes for parameters
  ptMinuit->mnparm(0, "alpha"  , 0.65 , 0.001   , 0 , 0 , ierflg);
  ptMinuit->mnparm(1, "nmean1" , 14   , 0.001   , 0 , 0 , ierflg);
  ptMinuit->mnparm(2, "k1"     , 2   , 0.001   , 0 , 0 , ierflg);
  ptMinuit->mnparm(3, "nmean2" , 50  , 0.001   , 0 , 0 , ierflg);
  ptMinuit->mnparm(4, "k2"     , 5 , 0.001   , 0 , 0 , ierflg);
  ptMinuit->mnparm(5, "scaling", data_curve->Integral(2, data_curve->GetNbinsX())  , 0.1  , 0 ,0, ierflg);

  ptMinuit->mnexcm("CALL FCN", &p1 ,1,ierflg);

  cout << "Fix parameter 5: " << data_curve->Integral(2, data_curve->GetNbinsX() ) << endl;
  ptMinuit->mnexcm("FIX", &p6 , 1 ,ierflg);
  
  // Now ready for minimization step
  arglist[0] = 0; // maxcalls
  arglist[1] = 0.1; //tolerance, standard 0.1
  ptMinuit->mnexcm("MIGRAD", arglist ,0,ierflg);
  ptMinuit->mnexcm("MINOS", &p0 ,1,ierflg);  //an error analysis on all parameters
  //ptMinuit->mnexcm("RELEASE", &p6 ,1,ierflg);
  //ptMinuit->mnexcm("MIGRAD", &p0 ,0,ierflg);
  //ptMinuit->mnexcm("MINOS", &p0 ,1,ierflg);
  ptMinuit->mnexcm("CALL FCN", &p3 , 1,ierflg);
  
  // Print results
  cout << "\nPrint results from minuit\n";
  double fParamVal;
  double fParamErr;
  ptMinuit->GetParameter(0,fParamVal,fParamErr);
  cout << "alpha="  << fParamVal << "+-"<<fParamErr <<"\n";
  ptMinuit->GetParameter(1,fParamVal,fParamErr);
  cout << "nmean1=" << fParamVal <<"+-" <<fParamErr<< "\n";
  ptMinuit->GetParameter(2,fParamVal,fParamErr);
  cout << "k1="     << fParamVal << "+-"<<fParamErr<< "\n";
  ptMinuit->GetParameter(3,fParamVal,fParamErr);
  cout << "nmean2=" << fParamVal << "+-"<<fParamErr<< "\n";
  ptMinuit->GetParameter(4,fParamVal,fParamErr);
  cout << "k2="     << fParamVal <<"+-" <<fParamErr<< "\n";
  ptMinuit->GetParameter(5,fParamVal,fParamErr);
  cout << "scaling="<< fParamVal <<"+-" <<fParamErr<< "\n";
  
  static Double_t  parval[10];
  static Double_t  parerr[10];
  for ( int iPar = 0 ; iPar != 6 ; ++iPar ) 
     ptMinuit->GetParameter( iPar , parval[iPar] , parerr [iPar] );
     
  // if you want to access to these parameters, use:
  Double_t amin,edm,errdef;
  Int_t nvpar,nparx,icstat;
  ptMinuit->mnstat(amin,edm,errdef,nvpar,nparx,icstat);
  //void mnstat(Double_t &fmin, Double_t &fedm, Double_t &errdef, Int_t &npari, Int_t &nparx, Int_t &istat) 
  //*-*-*-*-*Returns concerning the current status of the minimization*-*-*-*-*
  //*-*      =========================================================
  //*-*       User-called
  //*-*          Namely, it returns:
  //*-*        FMIN: the best function value found so far
  //*-*        FEDM: the estimated vertical distance remaining to minimum
  //*-*        ERRDEF: the value of UP defining parameter uncertainties
  //*-*        NPARI: the number of currently variable parameters
  //*-*        NPARX: the highest (external) parameter number defined by user
  //*-*        ISTAT: a status integer indicating how good is the covariance
  //*-*           matrix:  0= not calculated at all
  //*-*                    1= approximation only, not accurate
  //*-*                    2= full matrix, but forced positive-definite
  //*-*                    3= full accurate covariance matrix
  //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
  cout << "\n";
  cout << " Minimum chi square = " << amin << "\n";
  cout << " Estimated vert. distance to min. = " << edm << "\n";
  cout << " Number of variable parameters = " << nvpar << "\n";
  cout << " Highest number of parameters defined by user = " << nparx << "\n";
  cout << " Status of covariance matrix = " << icstat << "\n";

  cout << "\n";
  ptMinuit->mnprin(3,amin);
  //*-*-*-*Prints the values of the parameters at the time of the call*-*-*-*-*
  //*-*    ===========================================================
  //*-*        also prints other relevant information such as function value,
  //*-*        estimated distance to minimum, parameter errors, step sizes.
  //*-*
  //*-*         According to the value of IKODE, the printout is:
  //*-*    IKODE=INKODE= 0    only info about function value
  //*-*                  1    parameter values, errors, limits
  //*-*                  2    values, errors, step sizes, internal values
  //*-*                  3    values, errors, step sizes, first derivs.
  //*-*                  4    values, parabolic errors, MINOS errors
  //*-*    when INKODE=5, MNPRIN chooses IKODE=1,2, or 3, according to ISW(2)
  //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 
  
  //==================================================
  //simple fit on final curve
  
  cout << endl << endl << " ====> Making simple fit on corrected data" << endl << endl;
  //TWO NBDs
  TF1* nbd = new TF1("nbd",uncorrected_doubleNBD,1.5,300,6);
  //TF1* nbd = new TF1("nbd",doubleNBD,1.5,300,6);
  nbd->SetParNames("alpha","nmean1","k1","nmean2","k2","scalefactor");
  nbd->SetParameters( 0.64 , 14.8 , 2.18 , 50 , 5.03 , data_corrected->Integral());
  nbd->FixParameter(5,data_corrected->Integral());
  /*nbd->SetParLimits(0 , 0   , 1  );
  nbd->SetParLimits(1 , 0.1 , 80 );
  nbd->SetParLimits(2 , 0.  , 80 );
  nbd->SetParLimits(3 , 0   , 120 );
  nbd->SetParLimits(4 , 0.  , 80 );*/
  
  data_corrected->Fit("nbd","RO0");
  //curve->Fit("nbd","RO0");



  //==================================================  Drawing ====================
  
  TLegend * leg = new TLegend(0.05, 0.75, 0.35, 0.875); 
  TH1* hNDBGen = (TH1F*) data_curve->Clone("hNDBGen");
  TH1F* data_curve_clone = (TH1F*) data_curve->Clone("data_curve"); //data_curve is const, otherwise not able to Draw()
  
  TCanvas* cCorr = new TCanvas("cCorr","cCorr",200,10,500,500);  
  cCorr->SetLogy(true); 
  
  data_corrected->Draw();
  leg->AddEntry(data_corrected,"data corr","p");
  
  data_curve_clone->SetMarkerStyle(24);
  data_curve_clone->Draw("same");
  leg->AddEntry(data_curve_clone,"data raw","p");
  
  TF1 *fnbd = new TF1("fndb", doubleNBD , 1 , data_curve_clone->GetBinCenter(data_curve_clone->GetNbinsX()) , 6 );
  fnbd->SetParameters(parval[0],parval[1] ,parval[2] , parval[3] , parval[4] , parval[5]  ); 
  fnbd->SetLineColor(2); 
  fnbd->Draw("same"); 
  leg->AddEntry(fnbd,"complicated fit","l");
  
  nbd->SetLineColor( kBlue ); 
  nbd->Draw("same");
  leg->AddEntry(nbd,"simple fit","l");
  
  leg->Draw();
  gPad->WaitPrimitive();
  
  delete leg;
  delete cCorr;


}

//_____________________________________________________________________________
void transform2Matrix(const TH2F* matrixhist,double mtx[][matrixsize]){  
  //j-->RECO || i-->GEN
  for(int i=0;i<matrixsize;++i){
    for(int j=0;j<matrixsize;++j){
      if(i<matrixhist->GetNbinsX() && j<matrixhist->GetNbinsY())
        mtx[j][i] = matrixhist->GetBinContent(i+1,j+1);
      else
        mtx[j][i] = 0;
    }
  }
}

//_____________________________________________________________________________
bool checkMatrix(double inputMat[][matrixsize],double inputMatNormalized[][matrixsize]){
  // TURNED AROUND if you compare this to the one in unfolding.cc 
  int Nreco1=matrixsize;
  int Ngen1=matrixsize;
  for(int j=0;j<Nreco1;j++){
    double sum=0;
    for (int i=0;i<Ngen1;i++)
      sum+=(double)inputMat[j][i];	
    for (int i=0;i<Ngen1;i++){
      if(sum!=0)
	    inputMatNormalized[j][i]=(double)inputMat[j][i]/(double)sum;
      else
        inputMatNormalized[j][i]=0; 
    }
  }  
  if (debug_)  std::cout<<"                ...matrix normalized"<<std::endl;
  return true;
}



//_____________________________________________________________________________
void NCHMinuitNBD(){
  d.v = "v62";
  
  d.fill_file_data(0 , 62 , 7 , 2);
  d.fill_cuts_data("ATLAS2" , "HF0" , 0);
  TString s_f = "files/unfold_outputs/" + unf_getFileName();
  NCHMinuitNBD_f( s_f );
}



