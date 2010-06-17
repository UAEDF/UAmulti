#ifndef _MAKECORRECTIONS_
#define _MAKECORRECTIONS_


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

#include "../plugins/MultiPlots.h"
#include "../plugins/TMean.h"
#include "../plugins/TMoments.h"

#endif

using namespace std;
using namespace TMath;

int debug_ = 0;


const int matrixsize = 320;
bool useData = false;
//TString name = "data_uniform_0.9_eta2.5_v004";
TString name = "test";

TString st(TString input , int cut){
  stringstream out("");
  out<<cut;
  return input+"_cut"+out.str();
}

void transform2Matrix(TH2F* matrixhist,double matrix[][matrixsize]);
//TH1F resample(double matrix[][matrixsize], TH1F*, TH1F* , TH1F* , TH1F*, int = 5 , bool = false , TH1F* = NULL, bool = false);
void makeEff(TH1F*,TH1F*,TH1F*);
double getRMS(TH1F*);
double* Divide( const TArrayD* , double );

void divideByWidth(TH1F*);
void divideByWidth(TH2F*);
void multiplyByWidth(TH1F*);
void multiplyByWidth(TH2F*);



TGraphAsymmErrors h2g(TH1F*);

//iFileType,iDataType,Energy,iTracking,iSystType,iSystSign,STest
#include "../macro/fileManager.C"
#include "unfolding.cc"
#include "getNIter.C"
#include "resamplings.C"
//#include "moments.C"

#include "fitting_funcs.C"

//#include "makeFakeMatrix.C"

void makeCorrections(int typeData = 0, int hyp=1 , int niter=0 , int acc = 5 , double E = 0.9 ,
                     int typeMC = 0 , int iTr = 1 , int syst = 0 , int syst_sign = 0 , double Emc = 0 ,
		     TString filename = "" , int scaleWbin0 = true , bool drawcanv = true ,
		     float mu = 14 , float sigma = 15 ){

  if(typeMC==0){
    typeMC = 10;
    if(E == 7.0)
      typeMC = 31;
  } 
   
  //gROOT->ProcessLine(".x ../macro/BuildLibDico.C+");
   
  useData = true;
  if(Emc==0) Emc = E;
  double syst_val = 0;
  if(syst==0) syst_sign=0;
       
  #include "../macro/acceptanceMap.C"
  
  TString filedir("../plots/simpleanav10/");
  //TString addstr("binning1");
  TString addstr("");
  
  if(syst==201)
    addstr = "evtSelSyst";
    
  
    
  TFile* mctmp = NULL;
  
  //Get the MC file
  TString mcfile = fileManager(2,typeMC,Emc,iTr,0,0,addstr,filedir);
  //TFile* mc = new TFile("/user/rougny/Ferenc_Tracking_bis/CMSSW_3_3_6_patch3/src/UAmulti/ChPartAna/macro/collectionPlotter_MC_finalv2_900GeV.root","READ");
  //TFile* mc = new TFile("../macro/GOODFILES/MC_v005b_2.36TeV_eta2.5_pt0.4_gTr.root","READ");
  //TFile* mc = new TFile("../macro/GOODFILES/MC_v005b_0.9TeV_eta2.5_pt0.4_gTr.root","READ");
  TFile* mc = TFile::Open(mcfile,"READ");//"newbinning","../plots/unfoldingv2/"
  cout<<"MC input file : "<<mcfile<<endl;
  
  //#include "syst_MCfile.C"
  
  //Get the data file
  TString datafile = fileManager(2,typeData,E,iTr,0,0,addstr,filedir);
  //TString datafile = fileManager(2,typeData,E,iTr,0,0,"newbinning","../plots/unfoldingv2/");
  //TFile* data = new TFile("../macro/GOODFILES/data_v005b_2.36TeV_eta2.5_pt0.4_gTr.root","READ");
  //TFile* data = new TFile("../macro/GOODFILES/data_v005b_0.9TeV_eta2.5_pt0.4_gTr.root","READ");
  TFile* data = TFile::Open(datafile,"READ");
  cout<<"Data input file : "<<datafile<<endl;
  
  
  //Checking if both files exist
  if(mc == 0){
    cout<<"The MC input file doesn't exist ..."<<endl;
  }
  if(data == 0){
    cout<<"The Data input file doesn't exist ..."<<endl;
  }
  if(mc == 0 || data == 0)
    return;

  ostringstream dirstr("");
  dirstr << "/ptGen" << accMap.at(acc).at(0) << "_etaGen" << accMap.at(acc).at(1) 
      << "_ptReco" << accMap.at(acc).at(2) << "_etaReco" << accMap.at(acc).at(3) ;
  if(accMap.at(acc).at(4)==-1)
    dirstr << "_chargemin";
  else if(accMap.at(acc).at(4)==1)
    dirstr << "_chargeplus";
  dirstr << "/";
  TString dir = dirstr.str();
  //cout<<dir<<endl;
  //return;
  
  //------------------------------------------------------------
  //Don't even create the output file if can't do the systematic.
  if(syst%100==typeMC){
    cout<<"!! WARNING : this is not a systematic, the MC tune for the SD distri is the same as for the rest. Exiting now ..."<<endl;
    return;
  }
  if( (getNIter(acc,E)-(syst%300)) < 1 && int(syst/100) == 3 && syst_sign == -1){
    cout<<"!! WARNING : can't do the negative systematic, niter is already 1. Exiting now ..."<<endl;
    return;
  }
  if((syst==401 ||syst==402 || syst==403) && E==7.0){
    cout<<"!! WARNING : can't do this systematic, don't have the right energy mc. Exiting now ..."<<endl;
    return;
  }
  if(syst%400==typeMC){
    cout<<"!! WARNING : this is not a systematic, the MC tune for the matrix is the same as for the rest. Exiting now ..."<<endl;
    return;
  }
  
  
  //------------------------------------------------------------
  //Opening the output file
  ostringstream outstr("");
  outstr << "hyp" << hyp << "_niter" << niter << "_cut" << acc << "_DataType" << typeData;
  if(E!=Emc) outstr << "_Emc"<<Emc;
  if(filename!="") outstr << "__" << filename;
  cout<<"Output file : "<<fileManager(3,typeMC,E,iTr,syst,syst_sign,outstr.str())<<endl;
  TFile* out = new TFile(fileManager(3,typeMC,E,iTr,syst,syst_sign,outstr.str()),"RECREATE");
  out->cd();
  
  TDirectory* currentdir = gDirectory;
  
  //Getting the good #iter if adaptive.
  //BEWARE : needs to be after output file naming/creation.
  if(niter==0){
    niter = getNIter(acc,E);
    cout<<"Will use "<<niter<<" iterations for the unfolding ..."<<endl;
  }
  
  //including the systematic on #iterations
  #include "syst_niter.C"
  
  
  
  
  
  
  //------------------------------------------------------------------------------
  //---------------------------- UNFOLDING ---------------------------------------
  //------------------------------------------------------------------------------
  
  gDirectory->mkdir("unfolding");
  gDirectory->cd("unfolding");
  
  //Get the Unfolding matrix
  //TFile* mc_matrix = new TFile(fileManager(2,type,0.9,iTr),"READ");
  //TH2F* matrixhist = (TH2F*) mc_matrix->Get(dir+st("MatrixPlots_evtSel",acc)+st("/nch_matrix_evtSel",acc));
  TH2F* matrixhist = (TH2F*) mc->Get(dir+st("MatrixPlots_evtSel",acc)+st("/nch_matrix_evtSel",acc));
  #include "syst_matrix.C"
  matrixhist->SetName("nch_matrix");
  
 

  /*for(int i=1;i<=matrixhist->GetNbinsX();++i){
    matrixhist->SetBinContent(i,1,0);
    //matrixhist->SetBinContent(1,i,0);
  }*/


  //Setting the limits from the matrix
  Ngen1  = matrixhist->GetNbinsX();
  Nreco1 = matrixhist->GetNbinsX();



  if(drawcanv){
    TCanvas* cm = new TCanvas("cm","c",2500,10,500,500);
    cm->cd();
    matrixhist->Draw("colz");
    //gPad->WaitPrimitive();
  }
  
  //Transform the TH2F into an array
  matrix4dObj matrix;
  transform2Matrix(matrixhist,matrix);
  
  //get the true nch distribution (from MC)
  //TH1F* nch_trueGen = (TH1F*) mc->Get("GenMultiPlots_etaCut/MultiPlots_NSD_etaCut/nch_NSD_etaCut");
  TH1F* nch_trueGen_afterUnfolding = (TH1F*) mc->Get(dir+st("MatrixPlots_evtSel",acc)+st("/nch_gen_evtSel",acc));
  nch_trueGen_afterUnfolding->SetName("nch_MC_gen_afterUnfolding");
  //TH1F* nch_trueGen = (TH1F*) mc->Get("MatrixPlots_etaGenCut_L1_hf_VtxSel_PV_gTr_oVtx/nch_gen_etaGenCut_L1_hf_VtxSel_PV_gTr_oVtx");
  TH1F* nch_trueGen = (TH1F*) mc->Get(dir+st("MultiPlots_etaCut_noSel_NSD_gen",acc)+st("/nch_etaCut_noSel_NSD_gen",acc));
  nch_trueGen->SetName("nch_MC_gen_afterEvtSelCorrection");
  
  //nch_trueGen->Scale(1./nch_trueGen->GetEntries());
  //nch_trueGen->Draw();
  //gPad->WaitPrimitive();
  
  //get the nch to Unfold distribution
  TH1F* nch_INC;
  if(!useData)
    //nch_INC = (TH1F*) mc->Get(dir+st("GenMultiPlots_evtSel_reco",acc)+st("/MultiPlots_INC_evtSel_reco",acc)+st("/nch_INC_evtSel_reco",acc));
    nch_INC = (TH1F*) mc->Get(dir+st("MultiPlots_evtSel_INC_reco",acc)+st("/nch_evtSel_INC_reco",acc));
  else
    nch_INC = (TH1F*) data->Get(dir+st("MultiPlots_evtSel_INC_reco",acc)+st("/nch_evtSel_INC_reco",acc));
    //nch_INC = (TH1F*) data->Get(dir+st("MultiPlots_evtSel_PV",acc)+st("/nch_evtSel_PV",acc));
  nch_INC->SetName("nch_data_INC_beforeSDsub");
  
  //nch_INC->SetBinContent(1,0);
  
  //insertion of systematic code:
  #include "syst_tracking.C"
  
  
  //Getting the class
  MultiPlots* mp_INC_evtSel_reco_MC;
  MultiPlots* mp_INC_evtSel_reco_data;
  mp_INC_evtSel_reco_MC = (MultiPlots*) mc->Get(dir+st("GenMultiPlots_evtSel_reco",acc)+st("/MultiPlots_INC_evtSel_reco",acc)+st("/multi_class_INC_evtSel_reco",acc));
  if(useData) mp_INC_evtSel_reco_data = (MultiPlots*) data->Get(dir+st("MultiPlots_evtSel_INC_reco",acc)+st("/multi_class_evtSel_INC_reco",acc));
  else mp_INC_evtSel_reco_data = (MultiPlots*) mc->Get(dir+st("MultiPlots_evtSel_INC_reco",acc)+st("/multi_class_evtSel_INC_reco",acc));
  
  //nch_INC->Scale(1./double(mp_INC_evtSel_reco_data->nbEvts));
  double MC_factor;
  if(nch_INC->Integral()<2)
    MC_factor = 1. / double(mp_INC_evtSel_reco_MC->nbEvts) ;
  else
    MC_factor = double(mp_INC_evtSel_reco_data->nbEvts) / double(mp_INC_evtSel_reco_MC->nbEvts) ;
  cout<<"The ratio data_INC/MC_INC is "<<MC_factor<<endl;
 
   //if( useData ) {
    //TH1F* nch_trueRec = (TH1F*) mc->Get("MatrixPlots_etaGenCut_L1_hf_VtxSel_PV_gTr_oVtx/nch_reco_etaGenCut_L1_hf_VtxSel_PV_gTr_oVtx");
     
     //nch_trueRec->Draw("same");
   //} 
   
   
   
  //------------------------------------------------------------------------------
  //---------------------------- SD Substraction ---------------------------------
  //------------------------------------------------------------------------------
  
  
  
  cout<<" ++++ DOING SD SUB ++++" <<endl;
  
  TH1F* nch_evtSel_SD =  (TH1F*) mc->Get(dir+st("GenMultiPlots_evtSel_reco",acc)+st("/MultiPlots_SD_evtSel_reco",acc)+st("/nch_SD_evtSel_reco",acc));
  TH1F* nch_evtSel_NSD = (TH1F*) mc->Get(dir+st("GenMultiPlots_evtSel_reco",acc)+st("/MultiPlots_NSD_evtSel_reco",acc)+st("/nch_NSD_evtSel_reco",acc));
  TH1F* nch_evtSel_INC = (TH1F*) mc->Get(dir+st("GenMultiPlots_evtSel_reco",acc)+st("/MultiPlots_INC_evtSel_reco",acc)+st("/nch_INC_evtSel_reco",acc));
  nch_evtSel_SD->SetName("nch_MC_gen_SD");
  nch_evtSel_NSD->SetName("nch_MC_gen_NSD");
  nch_evtSel_INC->SetName("nch_MC_gen_INC");
  nch_evtSel_SD->Scale(MC_factor);
  nch_evtSel_NSD->Scale(MC_factor);
  nch_evtSel_INC->Scale(MC_factor);
  TH1F* nch_NSD = (TH1F*) nch_INC->Clone("nch_data_NSD_afterSDsub");
  
  //Checking the integral, nentries & nevts
  if(  nch_evtSel_INC->GetEntries()!=mp_INC_evtSel_reco_MC->nbEvts
    || nch_INC->GetEntries()!=mp_INC_evtSel_reco_data->nbEvts){
    cout<<"Problem in normalization of SD MC component, please correct it or resampling will be wrong !"<<endl;
    return;
  }
  if( int(nch_INC->GetEntries() - nch_INC->Integral()) != 0 ){
    cout<<"!! WARNING : nch_INC->Integral differs from the #entries ==> there are some over/underflows ..."<<endl;
    cout<<"   ===> Integral   : "<<nch_INC->Integral()<<"  |  #Entries : "<<nch_INC->GetEntries()
        <<"  |  Diff : "<<nch_INC->Integral()-nch_INC->GetEntries()<<endl;
    cout<<"   ===> #Overflows : "<<nch_INC->GetBinContent(nch_INC->GetNbinsX()+1)<<endl;
  }
  
  
  //Current % of SD & NSD
  /*double frac_SD  = nch_evtSel_SD->Integral() / nch_evtSel_INC->GetEntries();
  double frac_NSD = nch_evtSel_NSD->Integral() / nch_evtSel_INC->GetEntries();*/
  double frac_SD  = nch_evtSel_SD->Integral() / nch_evtSel_INC->Integral();
  double frac_NSD = nch_evtSel_NSD->Integral() / nch_evtSel_INC->Integral();
  if(nch_evtSel_INC->GetBinContent(nch_evtSel_INC->GetNbinsX()+1)!=0)
    cout<<"!! WARNING : the MC INC distri has overflows ..."<<endl;
  cout<<"Fraction of SD events : "<< frac_SD * 100. << endl;
  cout<<"Fraction of NSD events : "<< frac_NSD * 100. << endl;
  
  
  //inserting the SD systematic
  #include "syst_SDsub.C"
  
  
  //Doing the SD substraction
  nch_NSD->Add(nch_evtSel_SD,-1);
  
  
  //nch_NSD->SetBinContent(1,0);
  
 /*int bintochange = 15;
 int nsigma = 3;
 nch_NSD->SetBinContent(bintochange,nch_NSD->GetBinContent(bintochange)- nsigma * nch_NSD->GetBinError(bintochange));
 //nch_NSD->SetBinContent(bintochange+1,nch_NSD->GetBinContent(bintochange+1)- nsigma * nch_NSD->GetBinError(bintochange+1));
 bintochange +=4;
 //nch_NSD->SetBinContent(bintochange,nch_NSD->GetBinContent(bintochange)- nsigma * nch_NSD->GetBinError(bintochange));
 */
 
 
 
  if(drawcanv){
    TCanvas* c_SDsub = new TCanvas("c_SDsub","c_SDsub",2500,510,500,500);
    c_SDsub->cd();
    nch_INC->SetLineColor(kGreen);
    nch_INC->Draw();
    nch_evtSel_SD->SetLineColor(kRed);
    nch_evtSel_SD->Draw("hist same");
    nch_evtSel_NSD->Draw("hist same");
    nch_NSD->SetLineColor(kBlue);
    nch_NSD->Draw("e same");
  
    TLegend *leg = new TLegend (.65,.70,.90,.99);
    leg->AddEntry(nch_INC,"data - Raw","l" );
    leg->AddEntry(nch_NSD,"Data after SD corr","l" );
    leg->AddEntry(nch_evtSel_SD,"MC SD","l" );
    leg->AddEntry(nch_evtSel_NSD,"MC NSD","l" );
    leg->Draw("same");
  }
  
  TH1F* eff_evtSel = (TH1F*) mc->Get(dir+st("eff_evtSel",acc));
  TH1F* evtSel_before = (TH1F*) mc->Get(dir+st("evtSel_before",acc));
  TH1F* evtSel_after = (TH1F*) mc->Get(dir+st("evtSel_after",acc));
  eff_evtSel->Divide(evtSel_after,evtSel_before,1,1,"B");
  
  eff_evtSel->SetName("eff_evtSel");
  TH1F* nch_toUnfold = (TH1F*) nch_NSD->Clone("nch_toUnfold");
  //nch_toUnfold->Divide(eff_evtSel);
    
  
  divideByWidth(nch_evtSel_SD);
  divideByWidth(nch_evtSel_NSD);
  divideByWidth(nch_evtSel_INC);
  nch_evtSel_SD->Write();
  nch_evtSel_NSD->Write();
  nch_evtSel_INC->Write();
  
  
  
  
  
  
  //////////////////////////////////////////////////////////////////////////////////
  /////////////////////////          TEST AREA           ///////////////////////////
  //////////////////////////////////////////////////////////////////////////////////
  
  
  ////////// TEMP TO CHECK ERRORS
  
 // makeFakeMatrix(matrixhist,nch_toUnfold,1);
 // transform2Matrix(matrixhist,matrix);
 // matrixhist->Scale(1,"width");
 // matrixhist->Write();
 
 ////////// TEMP TO CHECK ERRORS
 
 
  
  //------------------------------------------------------------------------------
  //---------------------------- Unfolding ---------------------------------------
  //------------------------------------------------------------------------------
  
  
  cout<<" ++++ DOING UNFOLDING ++++" <<endl;
  
  TH1F nch_unfolded("","",1,0,1);
  TH1F* hypothesis = (TH1F*) nch_trueGen->Clone("hypothesis");
  if   (hyp == 0){
    for (int i=1;i<=hypothesis->GetNbinsX();i++)
      hypothesis->SetBinContent(i,hypothesis->GetBinWidth(i));
  }
  else if (hyp == 1)
    hypothesis = nch_trueGen_afterUnfolding;
  else if (hyp == 2)
    hypothesis = nch_toUnfold;
  else if (hyp == 3)
  {
    hypothesis->Reset();
    for (int i=0 ; i<10000 ; ++i) hypothesis->Fill(gRandom->Gaus(mu,sigma) ) ;
  }
  else if(hyp == 4){
    hypothesis->Reset();
    for(int i=0 ; i<=hypothesis->GetNbinsX() ; ++i) hypothesis->SetBinContent(i,NBD(i , 26.9 , 7 ));
  }
  else if(hyp == 5){
    hypothesis->Reset();
    TH1F* nbd1 = (TH1F*) hypothesis->Clone("nbd1");
    for(int i=1 ; i<=hypothesis->GetNbinsX() ; ++i) nbd1->SetBinContent(i,NBD(i , 26.9 , 7 ));
    
    TH1F* nbd2 = (TH1F*) hypothesis->Clone("nbd2");
    for(int i=1 ; i<=hypothesis->GetNbinsX() ; ++i) nbd2->SetBinContent(i,NBD(i , 57.9 , 13 ));
    //hypothesis->Add(nbd1,nbd2,0.72,0.28);
    hypothesis->Add(nbd1,nbd2,1,0);
    //hypothesis->SetBinContent(1,0.1);
    TCanvas t("t","t");
    hypothesis->Draw();
    gPad->WaitPrimitive();
    
  }
  
  
  nch_unfolded = (runalgo(matrix,nch_toUnfold,hypothesis,niter));
  TH1F* nch_unfoldedPtr = (TH1F*) nch_unfolded.Clone("nch_unfoldedPtr");

  cout<< nch_unfoldedPtr->GetBinContent(31) << "  " << nch_unfoldedPtr->GetBinError(31) << endl;
 
  //------------------------------------------------------------------------------
  //---------------------------- ReScaling -------------------------------------
  //------------------------------------------------------------------------------
  
  
  /*nch_unfoldedPtr->Scale(1./double(mp_INC_evtSel_reco_data->nbEvts));
  nch_INC->Scale(1./double(mp_INC_evtSel_reco_data->nbEvts));
  if(nch_unfoldedPtr->Integral()<2)
    MC_factor = 1. / double(mp_INC_evtSel_reco_MC->nbEvts) ;
  else
    MC_factor = double(mp_INC_evtSel_reco_data->nbEvts) / double(mp_INC_evtSel_reco_MC->nbEvts) ;
  */  
  
  
  
  
  //------------------------------------------------------------------------------
  //---------------------------- Resampling & moments ----------------------------
  //------------------------------------------------------------------------------
  
  
  cout<<" ++++ DOING RESAMPLING & MOMENTS' STAT ERRORS ++++" <<endl;
  
  TMoments* moment = new TMoments();
  
  gDirectory->mkdir("hist_resampling");
  gDirectory->cd("hist_resampling");

  TH1F nch_resampled = resample(matrix,nch_INC,nch_NSD,nch_unfoldedPtr,hypothesis,100,0,nch_evtSel_SD,moment,eff_evtSel,false);
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

    
  TH1F nch_mtxresampled = mtxresample(matrix,nch_toUnfold,hypothesis,100,moment,eff_evtSel);
  TH1F* nch_mtxresampledPtr = &nch_mtxresampled;
  
  gDirectory->cd("../");
  
  
  
  for(int nbin = 1 ; nbin<=nch_unfoldedPtr->GetNbinsX() ; ++nbin){
    nch_unfoldedPtr->SetBinError(nbin , sqrt(pow(nch_resampledPtr->GetBinError(nbin),2)+pow(nch_mtxresampledPtr->GetBinError(nbin),2)));
    /*cout<<nbin<<"  "<<nch_resampledPtr->GetBinError(nbin)<<"  "<<nch_mtxresampledPtr->GetBinError(nbin)<<endl;
    cout<<"  "<<sqrt(pow(nch_resampledPtr->GetBinError(nbin),2)+pow(nch_mtxresampledPtr->GetBinError(nbin),2))
        <<"  "<<nch_unfoldedPtr->GetBinError(nbin)<<endl;*/
  }
  
  
  //------------------------------------------------------------------------------
  //---------------------------- EvtSel Correction -------------------------------
  //------------------------------------------------------------------------------
  
  
  cout<<" ++++ DOING EVT SEL CORRECTION ++++" <<endl;
  
  TH1F* nch_corrected = (TH1F*) nch_unfoldedPtr->Clone("nch_data_corrected");
  //eff_evtSel->Sumw2();
  
  //including the systematic for evtSel Eff
  #include "syst_evtSelEff.C"
  
  
  nch_corrected->Divide(nch_corrected,eff_evtSel,1,1);
  /*
  nch_corrected->Scale(1./nch_corrected->Integral());
  nch_trueGen->Scale(1./nch_trueGen->Integral());
  nch_trueGen_afterUnfolding->Scale(1./nch_trueGen_afterUnfolding->Integral());*/
  
  if(drawcanv){
    TCanvas* c_effcorr = new TCanvas("c_effcorr","c_effcorr",1980,510,500,500);
    c_effcorr->cd();
    nch_trueGen->Draw("hist");
    nch_corrected->Draw("same e");
    nch_trueGen_afterUnfolding->Draw("same hist");
    nch_unfoldedPtr->SetLineWidth(2);
    nch_unfoldedPtr->SetLineColor(kGreen);
    nch_unfoldedPtr->Draw("same e");
  
    TLegend* leg = new TLegend (.65,.70,.90,.99);
    leg->AddEntry(nch_trueGen,"MC after Correction","l" );
    leg->AddEntry(nch_corrected,"Data after Correction","l" );
    leg->AddEntry(nch_trueGen_afterUnfolding,"MC after Unfolding","l" );
    leg->AddEntry(nch_unfoldedPtr,"Data after Unfolding","l" );
    leg->Draw("same");
  }
  cout<<"Mean of multiplicity --------> "<<nch_corrected->GetMean()<<endl;
  cout<<"RMS of multiplicity  --------> "<<nch_corrected->GetRMS()<<endl;
  
  
  //------------------------------------------------------------------------------
  //--------------------------------- Rescaling ----------------------------------
  //------------------------------------------------------------------------------
  
  
  cout<<" ++++ RESCALING ++++" <<endl;
  
  divideByWidth(nch_trueGen);
  divideByWidth(nch_trueGen_afterUnfolding);
  divideByWidth(nch_INC);
  divideByWidth(nch_NSD);
  divideByWidth(nch_unfoldedPtr);
  divideByWidth(nch_corrected);
  divideByWidth(nch_resampledPtr);
  divideByWidth(nch_mtxresampledPtr);
  
  divideByWidth(matrixhist);
  divideByWidth(hypothesis);
  
  
  //nch_corrected->Scale(1./nch_corrected->Integral());
  
  //------------------------------------------------------------------------------
  //--------------------------------- Moments ------------------------------------
  //------------------------------------------------------------------------------
  
  cout<<" ++++ DOING MOMENTS ++++" <<endl;
  
  gDirectory->mkdir("moments");
  gDirectory->cd("moments");
  
  /*const int nmoments = 6;
  TH1F** moments = new TH1F*[nmoments];
  vector<double> cmom      = getCMoments(nch_corrected);
  vector<double> fmom      = getFMoments(nch_corrected);
  vector<double> cmomerror = getMomentErrors(nch_corrected);
  for(int m = 0 ; m < nmoments ; ++m){
    cout<<cmom[m]<<"  "<<cmomerror[m]<<endl;
    ostringstream momname("");
    momname << "moment_" << m;
    moments[m] = new TH1F(momname.str().c_str() , momname.str().c_str() , 1 , E*1000.-0.5 , E*1000.+0.5);
    moments[m]->SetBinContent(1, cmom[m]);
    moments[m]->SetBinError(1, cmomerror[m]);
    moments[m]->Write();
  }*/
  
    
  //TMoments* moment = new TMoments(nch_corrected);
  //moment->print();
  
  moment->Add(nch_corrected);
  moment->ComputeMoments();
  //moment->print();
  //moment->ComputeErrorsFromResampling(nch_corrected);
  moment->print();
  moment->Write("moments");
  
  
 /* TH1F* test = new TH1F("jj","jj",3,4.5,7.5);
  test->Fill(5,10);
  test->Fill(6,10);
  test->Fill(7,10);
  TH1F* test = new TH1F("jj","jj",20000,-5,20);
  for(int t=0;t<1000000;++t)
    test->Fill(gRandom->Poisson(1));
  cout<<"--------------------------"<<endl;
  getCMoments(test);
  getFMoments(test);
  
  
  TMoments* moment = new TMoments();
  for(int ll = 1 ; ll <= test->GetNbinsX() ; ++ll){
    cout<<"hjkhkjhkjhkhkjhjkhjkhjkhkjhjkhjkhjkhjkhkj   "<<ll<<endl;
    moment->Add(test->GetBinCenter(ll) , test->GetBinContent(ll));
  }
  moment->ComputeMoments();
  moment->print();
  */
  
  
  
  gDirectory->cd("../");
  
  
  
  //------------------------------------------------------------------------------
  //------------------------------------ KNO -------------------------------------
  //------------------------------------------------------------------------------
  
  
  cout<<" ++++ DOING KNO ++++" <<endl;
  
  double knomean = nch_corrected->GetMean();
  
  TString tkno = out->GetName();
  tkno.ReplaceAll("plots/","plots/systv10_binning1v3_2/");
  cout<<"Opening for kno mean the file : "<<tkno<<endl;
  TFile* fkno = TFile::Open(tkno,"READ");
  if(fkno!=0){
    TMoments* mom_kno = (TMoments*) fkno->Get("unfolding/moments/moments");
    knomean = mom_kno->mean->GetMean();
  }
  else{
    cout<<"WARNING !! The file does not exist, taking the mean of the hist instead"<<endl;
  }
  
  
  TH1F* kno = new TH1F("kno_corrected","kno_corrected;z = n_{ch} / < n_{ch} >;#psi(z)",nch_corrected->GetNbinsX(),Divide(nch_corrected->GetXaxis()->GetXbins(),knomean));
  kno->Sumw2();
  /*for( int k = 60 ; k <= nch_corrected->GetNbinsX() ; ++k)
    nch_corrected->SetBinContent(k,0);*/
  for( int k = 1 ; k <= nch_corrected->GetNbinsX() ; ++k){
    kno->SetBinContent(k , knomean * nch_corrected->GetBinContent(k) / nch_corrected->Integral());
    kno->SetBinError(k , knomean * nch_corrected->GetBinError(k) / nch_corrected->Integral());
  }
  if(drawcanv){
    TCanvas* c_kno = new TCanvas("c_kno","c_kno",200,510,500,500);
    c_kno->cd();
    kno->Draw();
  }
  
  
  TMoments* moment_gen = new TMoments(nch_trueGen);
  moment_gen->ComputeMoments();
  knomean = moment_gen->mean->GetMean();
  if(fkno!=0){
    TH1F* mom_kno_gen = (TH1F*) fkno->Get("unfolding/nch_MC_gen_afterEvtSelCorrection");
    knomean = mom_kno_gen->GetMean();
  }
  TH1F* kno_gen = new TH1F("kno_gen","kno_gen;z = n_{ch} / < n_{ch} >;#psi(z)",nch_trueGen->GetNbinsX(),Divide(nch_trueGen->GetXaxis()->GetXbins(),knomean));
  kno_gen->Sumw2();
  for( int k = 1 ; k <= nch_trueGen->GetNbinsX() ; ++k){
    kno_gen->SetBinContent(k , knomean * nch_trueGen->GetBinContent(k) / nch_trueGen->Integral());
    kno_gen->SetBinError(k , knomean * nch_trueGen->GetBinError(k) / nch_trueGen->Integral());
  }
  
  out->cd("unfolding");
  kno_gen->Write();
  
  
  
  
  //------------------------------------------------------------------------------
  //---------------------------------- Plots -------------------------------------
  //------------------------------------------------------------------------------
  
if(drawcanv){

  TCanvas* c_final = new TCanvas("c_final","c_final",1980,10,500,500);
  c_final->SetLeftMargin(0.17);
  c_final->SetBottomMargin(0.10);
  c_final->SetFillColor(0);
  c_final->GetFrame()->SetFillColor(21);
  c_final->GetFrame()->SetBorderSize(12);
  c_final->SetGrid();
  
  //Unfolding  
  nch_trueGen->SetLineWidth(2);
  nch_INC->SetLineWidth(2);
  nch_corrected->SetLineWidth(2);
  nch_trueGen->SetLineColor(kBlue);
  nch_INC->SetLineColor(kGreen);
  nch_corrected->SetLineColor(kRed);
 
  nch_INC->Draw("hist");
  nch_trueGen->Draw("same hist");
  nch_corrected->Draw("same e");
  
  /*nch_trueGen->Scale(1./nch_trueGen->Integral());
  nch_INC->Scale(1./nch_INC->Integral());
  nch_corrected->Scale(1./nch_corrected->Integral());*/
  
  
  // UA5
  bool lua5 = true;
  TGraphAsymmErrors* ua5_multi;
  if ( lua5 ) {
    ifstream ua5("/user/xjanssen/ua5_dsigdn_eta15");
    const Int_t   n = 300 ;
    Int_t   i = 0;
    Double_t xl[n], xh[n], x[n] , y[n], eyl[n] , eyh[n] , ex[n];
    //ua5.open ("/user/xjanssen/ua5_dsigdn_eta15");
    while ( ua5 >>  xl[i] >> xh[i] >> y[i] >> eyh[i] >> eyl[i] ) {
      eyl[i] = -eyl[i] ;
      x[i]  = xl[i]+(xh[i]-xl[i])/2;
      //y[i] = 3*50000 * y[i];
      //eyl[i]*=3*50000;
      //eyh[i]*=3*50000;
      //cout<<y[i]<<endl;
      ex[i] = (xh[i]-xl[i])/2;
      i++;
    }
    
    ua5.close();
    ua5_multi = new TGraphAsymmErrors(i,x,y,ex,ex,eyl,eyh);
    
    //gr->SetMarkerColor(0);
    //gr->SetMarkerStyle(24);
    ua5_multi->Draw("L");
    ua5_multi->Write("ua5");
  }
 

  TLegend* leg = new TLegend (.65,.90,.90,.99);
  leg->AddEntry(nch_trueGen,"PYTHIA D6T","l" );
  if (useData) {
    leg->AddEntry(nch_INC,"Data - Raw","l" );
    leg->AddEntry(nch_corrected,"Data - Corrected","l" );
  } else {
    leg->AddEntry(nch_INC,"MC - Raw","l" );
    leg->AddEntry(nch_corrected,"MC - Corrected","l" );
  }
  if (lua5) leg->AddEntry(ua5_multi,"UA5 #eta < 1.5","l"); 
  leg->SetBorderSize(0);
  leg->SetFillColor(0);

  leg->Draw();
  
  cout<<"bin 0 corrected:"<<nch_corrected->GetBinContent(1)<<endl;
  cout<<"Err bin 0 corrected:"<<nch_corrected->GetBinError(1)<<endl;
  cout<<"bin 0 before:"<<nch_unfoldedPtr->GetBinContent(1)<<endl;
  cout<<"Err bin 0 before:"<<nch_unfoldedPtr->GetBinError(1)<<endl;
  cout<<"bin 0 eff:"<<eff_evtSel->GetBinContent(1)<<endl;
  cout<<"Err bin 0 eff:"<<eff_evtSel->GetBinError(1)<<endl;
  
  
  //------------------------------------------------------------------------------
  //---------------------------------- Fits -------------------------------------
  //------------------------------------------------------------------------------
  
  
  cout<<" ++++ DOING FITS ++++" <<endl;
  
  #include "fitting.C"
  
}//End of if(drawcanv) for final plot
  
  //Unfolding
  hypothesis->Write();
  nch_trueGen->Write();
  nch_trueGen_afterUnfolding->Write();
  nch_INC->Write();
  nch_NSD->Write();
  nch_unfoldedPtr->Write();
  nch_corrected->Write();
  nch_resampledPtr->Write("nch_histresampled");
  nch_mtxresampledPtr->Write("nch_mtxresampledPtr");
  eff_evtSel->Write();
  
  h2g(nch_corrected).Write();
  //h2g(nch_mtxresampledPtr).Write();
  
  

  //eff_nch_L1_hf_vtxSel->Write();
  kno->Write();
  matrixhist->Write();
  
  gDirectory->cd("../");

  
  //------------------------------------------------------------------------------
  //-------------------------------- Corrections ---------------------------------
  //------------------------------------------------------------------------------
  
  
  cout<<" ++++ DOING BASIC CORRECTIONS ++++" <<endl;
  
  MultiPlots* mp_etaCut_noSel_NSD_gen         = (MultiPlots*) mc->Get(dir+st("MultiPlots_etaCut_noSel_NSD_gen",acc)+st("/multi_class_etaCut_noSel_NSD_gen",acc));
  MultiPlots* mp_etaCut_evtSel_INC_reco_MC    = (MultiPlots*) mc->Get(dir+st("MultiPlots_evtSel_INC_reco",acc)+st("/multi_class_evtSel_INC_reco",acc));
  MultiPlots* mp_etaCut_evtSel_INC_reco_data; 
  if(!useData) mp_etaCut_evtSel_INC_reco_data = (MultiPlots*) mc->Get(dir+st("MultiPlots_evtSel_INC_reco",acc)+st("/multi_class_evtSel_INC_reco",acc));
  else         mp_etaCut_evtSel_INC_reco_data = (MultiPlots*) data->Get(dir+st("MultiPlots_evtSel_INC_reco",acc)+st("/multi_class_evtSel_INC_reco",acc));
 
  cout << " nbEvts MC gen,reco : " << mp_etaCut_noSel_NSD_gen->nbEvts << "  " << mp_etaCut_evtSel_INC_reco_data->nbEvts  <<  endl;
  
  
 #include "makeBasicCorrections.C"
 
 
 
 // out->Close();
  //data->Close();
  //mc->Close();
/*
  delete c1;
  delete cm; 
  out->Close();
  data->Close();
  mc->Close();
  delete mc;
  delete data;
*/
}

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

void makeKNO(TH1F* multi,TH1F* kno){
  for( int k = 1 ; k <= multi->GetNbinsX() ; ++k){
    kno->SetBinContent(k , multi->GetMean() * multi->GetBinContent(k));
    kno->SetBinError(k , multi->GetMean() * multi->GetBinError(k));
  }
}


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



double* Divide(const TArrayD* array , double val){
  TArrayD* temp = new TArrayD();
  array->Copy(*temp);
  for(int i = 0 ; i < temp->GetSize() ; i++)
    temp->SetAt(array->At(i) / val , i);
  
  return temp->GetArray();  
}

void divideByWidth(TH1F* hist){
  for(int i=1;i<=hist->GetNbinsX();++i){
    hist->SetBinContent(i,double(hist->GetBinContent(i))/double(hist->GetBinWidth(i)));
    hist->SetBinError(i,double(hist->GetBinError(i))/double(hist->GetBinWidth(i)));
  }
}

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

void multiplyByWidth(TH1F* hist){
  for(int i=1;i<=hist->GetNbinsX();++i){
    hist->SetBinContent(i,double(hist->GetBinContent(i))*double(hist->GetBinWidth(i)));
    hist->SetBinError(i,double(hist->GetBinError(i))*double(hist->GetBinWidth(i)));
  }
}


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
double NBD(double x, double nmean, double k){
  double p = 1. / ( (nmean / k) + 1 );
  return Gamma(x+nmean)/( Gamma(x+1) * Gamma(nmean) ) * pow(p,k) * pow ( 1 - p , x);
}

double NBD4(int x, double nmean, int k){
  double p = 1. / ( (nmean / k) + 1 );
  return Binomial(x + k -1, k - 1) * pow(p,k) * pow ( 1 - p , x);
}
*/
