#ifndef _NCHMAKECORRECTIONS_
#define _NCHMAKECORRECTIONS_


/// Watch out dangerous
#include "TError.h"
/////

//#include "vector>
#include "TMath.h"
#include "TH1F.h"
#include "TF1.h"
#include "TH1D.h"
#include "TH2F.h"
#include "TFile.h"
#include "TChain.h"
#include "TChainElement.h"
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

#include "../macro/files_funcs.C"

#endif
using namespace std;
using namespace TMath;

int debug_ =0;


const int matrixsize = 380; //bigger then number of bins of my matrix

TString st(TString input , int cut){
  stringstream out("");
  out<<cut;
  return input+"_cut"+out.str();
}

void transform2Matrix(TH2F* matrixhist,double matrix[][matrixsize]);
void    makeEff(TH1F*,TH1F*,TH1F*);
double  getRMS(TH1F*);
double* Divide( const TArrayD* , double );

void SaturateEfficiencyCurve(TH1F*);
void divideByWidth(TH1F*);
void divideByWidth(TH1D*);
void divideByWidth(TH2F*);
void multiplyByWidth(TH1F*);
void multiplyByWidth(TH1D*);
void multiplyByWidth(TH2F*);

TGraphAsymmErrors h2g(TH1F*);
TH1F* getEff(TFile* , TString , TString , TString );

//iFileType,iDataType,Energy,iTracking,iSystType,iSystSign,STest
#include "../macro/fileManager.C"

#include "unfolding.cc"
#include "getNIter.C"


//TrackSYS:
#include "NCHincreaseNTracks.C"

#include "NCHresamplings.C" 
//is included in the code itself #include "syst_niter.C"

#include "fitting_funcs.C"

//#include "makeFakeMatrix.C"


//_____________________________________________________________________________
void NCHmakeCorrections(int typeMC, const TString mcfile, const TString datafile, const TString outputpart, 
                         TString acc , TString cen , TString hf ,
                         bool useData, int hyp, int niter, int syst, int syst_sign, int unfVersion, bool zerobias_bool =1) { //double Emc, double Edata ) {
                         //, bool drawcanv, float mu, float sigma ){

//switch to select the good unfolding order
if( unfVersion!=0 && unfVersion!=1 && unfVersion!=2) {
    cout << "WRONG unfVersion number: " <<  unfVersion <<  " Has to be 0, 1 or2. "  << endl;
    return;
}
cout << "The switch for the unfolding order is chosen as: " << unfVersion << endl;


#include "NCHniter.C"


TString diff="RECO";
TString diffmc="NSD";

TString dirRECO="Difflvl_"+acc+"/Centrlvl_"+diff+"_"+acc+"/HFlvl_"+cen+"_"+diff+"_"+acc+"/EvtSel_"+hf+"_"+cen+"_"+diff+"_"+acc+"/";
TString dirmc=dirRECO;
if(hf=="HF1")  dirmc.ReplaceAll(diff , diffmc);

TString lastpartmc1      = "_partfull" ;
TString lastpartmc2      = "_" + hf + "_" + cen + "_" + diff + "_" + acc;
if(hf=="HF1")  lastpartmc2.ReplaceAll(diff , diffmc);
TString lastpartmc_nosel = "_partnoSel"+ lastpartmc2;
TString lastpartRECO     =    "_full_" + hf + "_" + cen + "_RECO_" + acc;

	 
  bool drawcanv=false;
              
  bool doptcorr=true;   //it is only done for (cut1 and cut5) && (E=0.9||7) && nocut 
  
  if(debug_) cout << "usedata beginning " << useData << endl;
  
  //detaching cloned TH1's from their original file, so outfile->Close() will not crash the program
  TH1::AddDirectory(kFALSE);
               
  TFile* mc   = getFile( mcfile );  
  TFile* data = 0;
  TFile* zerobias =0;
  TString mconmc_str="";
  if(useData) data = getFile( datafile );
  else {
    data = mc;
    mconmc_str="_MCcorrMC";
  }
  TString zerobias_str=datafile;
  zerobias_str.ReplaceAll("data","zerobias");
  if(zerobias_bool) zerobias = getFile (zerobias_str);
   
  
  TString lastpartmc=lastpartmc1+lastpartmc2;
  TString syst_str = "";
  if (syst !=0) { 
    stringstream sys_nb("");    sys_nb    << "_SYS"  << syst;
    syst_str = sys_nb.str();
    if(syst_sign==1) syst_str = syst_str+"P";
    if(syst_sign==-1) syst_str = syst_str+"M";
  }  
  
  TString ptcorr_str="_ptcorroff";
  if(doptcorr) ptcorr_str="";

  
  TString output_str =  outputpart+mconmc_str+lastpartmc+syst_str+ptcorr_str+".root"; 
      output_str.ReplaceAll("RECO","INEL");
  cout << "Output file : " << output_str << endl;

  TFile* out = new TFile(output_str,"RECREATE");
  out->cd();  
  
   
  //------------------------------------------------------------------------------
  //---------------------------- UNFOLDING ---------------------------------------
  //------------------------------------------------------------------------------
  
  if (debug_!=0) cout <<"[DEBUG] Making unfolding dir" <<endl;
  gDirectory->mkdir("unfolding"+lastpartmc);
  gDirectory->cd("unfolding"+lastpartmc);
  
  
  //Get the unfolding matrix
  TH2F* matrixhist = 0;
  if(unfVersion==0) matrixhist = getHist<TH2F>( *mc , dirmc+"TResponseMtx"+lastpartmc+"/mtx"+lastpartmc );
  else if(unfVersion==1) matrixhist = getHist<TH2F>( *mc , dirmc+"TResponseMtx_mtx_vtxSel"+lastpartmc2+"/mtx_mtx_vtxSel"+lastpartmc2 );
  else if(unfVersion==2) matrixhist = getHist<TH2F>( *mc , dirmc+"TResponseMtx_mtx_noSel"+lastpartmc2+"/mtx_mtx_noSel"+lastpartmc2 );
  matrixhist->SetName("nch_matrix");       
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
  TH1F* nch_trueGen_afterUnfolding = 0;
  if(unfVersion==0)        nch_trueGen_afterUnfolding = getHist<TH1F> ( *mc , dirmc+"MultiPlots_mp"+lastpartmc+"/nch_mp"+lastpartmc );
  else if (unfVersion==1)  nch_trueGen_afterUnfolding = getHist<TH1F> ( *mc , dirmc+"GenPart_gpp_vtxSel"+lastpartmc2+"/nch_gpp_vtxSel"+lastpartmc2 );
  else if (unfVersion==2)  nch_trueGen_afterUnfolding = getHist<TH1F> ( *mc , dirmc+"GenPart_gpp_noSel"+lastpartmc2+"/nch_gpp_noSel"+lastpartmc2 );
  nch_trueGen_afterUnfolding->SetName("nch_MC_gen_afterUnfolding");  
  if(debug_!=0) {
    cout << "nch_trueGen_afterUnfolding loaded" << endl;
    //nch_trueGen_afterUnfolding->Draw();
    //gPad->WaitPrimitive();
  } 

 
  TH1F* nch_trueGenAfterEvtSel = 0;
  if(unfVersion==0)  { 
      nch_trueGenAfterEvtSel = getHist<TH1F> ( *mc , dirmc+"MultiPlots_mp"+lastpartmc_nosel+"/nch_mp"+lastpartmc_nosel );
      // does not exist for version 1 and 2
      nch_trueGenAfterEvtSel->SetName("nch_MC_gen_afterEvtSelCorrection");
  }      
  if(debug_!=0) {
    cout << "nch_trueGenAfterEvtSel loaded" << endl;
    if(unfVersion==0)  {
        nch_trueGenAfterEvtSel->Draw();
        gPad->WaitPrimitive();
    }       
  } 
  
  TH1F* nch_trueGenAfterVtxSel = 0;
  if(unfVersion==1) { 
     nch_trueGenAfterVtxSel = getHist<TH1F> ( *mc , dirmc+"GenPart_gpp_vtxSel"+lastpartmc2+"/nch_gpp_vtxSel"+lastpartmc2 );
     // does not exist for version 0 and 2
     nch_trueGenAfterVtxSel->SetName("nch_MC_gen_afterVtxSelCorrection"); 
  } 
  if(debug_!=0) {
    cout << "nch_trueGenAfterVtxSel loaded" << endl;
    if(unfVersion==1) {
        nch_trueGenAfterVtxSel->Draw();
        gPad->WaitPrimitive();
    }    
  } 
  
   
  TH1F* nch_trueGen=getHist<TH1F>( *mc , dirmc+"GenPart_gpp_CentrGen"+lastpartmc2+"/nch_gpp_CentrGen"+lastpartmc2 ); 
  nch_trueGen->SetName("nch_MC_gen_After_CentrEvtCorr"); 
  if(debug_!=0) {
    cout << "nch_trueGen_After_CentrEvtCorr loaded" << endl;
    //nch_trueGen->Draw();
    //gPad->WaitPrimitive();
  } 
  
 
  //get the nch to Unfold distribution (if useData=0, data=mc)
  TH1F* nch_REC = getHist<TH1F> ( *data , dirRECO+"MultiPlots_mpreco"+lastpartRECO+"/nch_mpreco"+lastpartRECO );
  
/*    //fix bug manually: 
  //if a central cut --> you require at least 1 track --> set zerobin to 0
  //if nocut --> do nothing, stuff is allowed to go in zerobin
  if (! (dirmc.Contains("nocut")) )  {
     nch_REC->SetBinContent(1,0);
     nch_MC_RECO_RAW->SetBinContent(1,0);
     }
*/
  if(debug_!=0) {
    cout << "nch_REC data loaded" << endl;
    cout << nch_REC->GetBinContent(1) << " bin content 0 "<< endl;
    nch_REC->Draw();
    gPad->WaitPrimitive();
    
    cout << "entries in zero bin: " << nch_REC->GetBinContent(1) <<endl;
    cout << useData << "  usedata " <<endl;
  }
 
  
  TH1F* nch_MC_RECO_RAW = getHist<TH1F>( *mc , dirRECO+"MultiPlots_mpreco"+lastpartRECO+"/nch_mpreco"+lastpartRECO );
  //stays the same for all unfVersions
  if(debug_) cout << "nch_MC_RECO_RAW loaded " << endl;
  
  //TRACK SYSTEMATIC  in NCHincreaceNTracks.C
  if(int(syst/100)==1) {
    double syst_val = 2.53;
    bool printSummary = true;    
    //nch_REC->Draw();
    //gPad->WaitPrimitive();
    increaseNTracks( nch_REC , syst_sign , syst_val , printSummary );    
    //nch_REC->Draw();
    //gPad->WaitPrimitive();
  } 
     
  
  //NITER SYSTEMATIC in syst_niter.C
  double syst_val = 0.;
  #include "syst_niter.C"
  
  
  //------------------------------------------------------------------------------
  //------------------------- SD Substraction  IF HF1  ---------------------------
  //------------------------------------------------------------------------------ 
  TH1F* nch_SD=0;
  TH1F* nch_INEL=0;  
  if(lastpartmc.Contains("HF1")) {
  
    cout << " +++ SD SUBSTRACTION ==> " << endl;
  
    TString pathSD=dirRECO+"MultiPlots_mpreco"+lastpartRECO+"/nch_mpreco"+lastpartRECO;
    //cout << pathSD << "   lastpartmc  "<< endl;
    pathSD.ReplaceAll("RECO","SD");
    //cout << pathSD << "   lastpartedited  "<< endl;
    TString pathINEL=dirRECO+"MultiPlots_mpreco"+lastpartRECO+"/nch_mpreco"+lastpartRECO;
    pathINEL.ReplaceAll("RECO","RECO");
    

    nch_SD   = getHist<TH1F> ( *mc , pathSD );
    nch_INEL = getHist<TH1F> ( *mc , pathINEL);
    
    cout <<"dirRECO " << dirRECO <<endl;
    cout <<"pathSD " << pathSD << endl;
    //nch_REC->Draw();
    //gPad->WaitPrimitive();
    
    cout <<"pathINEL" << pathINEL << endl;
    //nch_INEL->Draw();
    //gPad->WaitPrimitive();   
    
    double Corr_Factor=nch_INEL->Integral() / nch_REC->Integral();
    cout <<"Corr_factor " << Corr_Factor << endl;
    nch_SD->Scale(1/Corr_Factor);
    
    nch_REC->Add(nch_SD,-1); 
    
             
  } 

  //------------------------------------------------------------------------------
  //-------------------------      Make Efficiencies   ---------------------------
  //------------------------------------------------------------------------------   
  if(debug_) cout << "calculating efficiencies " << endl;
 
  
  //tracks level for unfVersion 1 annd 2
  TH1F* eff_trEvtSel = getEff( mc , dirmc+"MultiPlots_mpreco_full"+lastpartmc2 +"/nch_mpreco_full"+lastpartmc2 ,
                             dirmc+"MultiPlots_mpreco_noSel"+lastpartmc2 +"/nch_mpreco_noSel"+lastpartmc2 , 
			     "eff_nch_trEvtEff"+lastpartmc2) ;
  TH1F* eff_trTrigSel = NULL;
  if(zerobias && !lastpartmc2.Contains("NSD") ) { 
     eff_trTrigSel = getEff( zerobias , dirmc+"Track_trp_full"+lastpartmc2 +"/nch_trp_full"+lastpartmc2 ,
                             dirmc+"Track_trp_vtxSel"+lastpartmc2 +"/nch_trp_vtxSel"+lastpartmc2 , 
			     "eff_nch_trTrigEff"+lastpartmc2) ;
  }
  else {               
    eff_trTrigSel = getEff( mc , dirmc+"Track_trp_full"+lastpartmc2 +"/nch_trp_full"+lastpartmc2 ,
                             dirmc+"Track_trp_vtxSel"+lastpartmc2 +"/nch_trp_vtxSel"+lastpartmc2 , 
			        "eff_nch_trTrigEff"+lastpartmc2) ;
   }                 
  
  if(debug_) cout << "track level efficiencies loaded" << endl;               
  //particles level
  TH1F* eff_evtSel = getEff( mc , dirmc+"MultiPlots_mp_partfull"+lastpartmc2 +"/nch_mp_partfull"+lastpartmc2 ,
                             dirmc+"MultiPlots_mp_partnoSel"+lastpartmc2 +"/nch_mp_partnoSel"+lastpartmc2 , 
			     "eff_nch_evtEff"+lastpartmc2) ;
  TH1F* eff_vtxSel = getEff( mc ,  dirmc+"GenPart_gpp_vtxSel"+lastpartmc2 +"/nch_gpp_vtxSel"+lastpartmc2 ,
                             dirmc+"GenPart_gpp_noSel"+lastpartmc2 +"/nch_gpp_noSel"+lastpartmc2 , 
			     "eff_nch_vtxEff"+lastpartmc2) ;
  TH1F* eff_centrSel = getEff( mc ,  dirmc+"GenPart_gpp_noSel"+lastpartmc2 +"/nch_gpp_noSel"+lastpartmc2 ,
                             dirmc+"GenPart_gpp_CentrGen"+lastpartmc2 +"/nch_gpp_CentrGen"+lastpartmc2 , 
			     "eff_nch_CentrEff"+lastpartmc2) ;    

  if(debug_) cout << "all efficiencies loaded" << endl;
  if(debug_) cout << "NBins eff_evtSel: " << eff_evtSel->GetNbinsX() << "  NBins eff_centrSel:   "   << eff_centrSel->GetNbinsX();
  
  if(eff_evtSel==0) {
    cout << "eff_evtSelRECO is empty"<<endl;
    return;
  }  
  if(eff_centrSel==0) {
    cout << "eff_centrSelRECO is empty"<<endl;
    return;
  }
  eff_trEvtSel->SetName("eff_trEvtSel");
  eff_trTrigSel->SetName("eff_trTrigSel");
  eff_evtSel->SetName("eff_evtSel");
  eff_vtxSel->SetName("eff_vtxSel");
  eff_centrSel->SetName("eff_centrSel");

  //put the eff to 1 after some time
  SaturateEfficiencyCurve( eff_evtSel    );
  SaturateEfficiencyCurve( eff_centrSel  );
  SaturateEfficiencyCurve( eff_vtxSel    );
  SaturateEfficiencyCurve( eff_trEvtSel  );
  SaturateEfficiencyCurve( eff_trTrigSel );
  // only useful for unfVersion 0
  //TH1F* eff = (TH1F*) eff_evtSel->Clone("eff_total");
  //eff->Multiply(eff_centrSel,eff_evtSel,1,1);
    

  
  TH1F* effBeforeUnf = 0;
  TH1F* effAfterUnf = 0;
  if( unfVersion ==0 ){
    // effBeforeUnf does not exist
    effAfterUnf = (TH1F*) eff_evtSel->Clone("eff_after_unf");
    effAfterUnf->Multiply(eff_centrSel,eff_evtSel,1,1);
  }
  else if( unfVersion == 1){
    effBeforeUnf = (TH1F*) eff_trTrigSel->Clone("eff_before_unf");
    effAfterUnf = (TH1F*) eff_centrSel->Clone("eff_after_unf");
    effAfterUnf->Multiply(eff_vtxSel,eff_centrSel);
  }  
  else if( unfVersion == 2){
    effBeforeUnf = (TH1F*) eff_trEvtSel->Clone("eff_before_unf");
    effAfterUnf = (TH1F*) eff_centrSel->Clone("eff_after_unf");
  }
  
  if(debug_!=0) {
    eff_evtSel->Draw();
    gPad->WaitPrimitive();
    eff_centrSel->Draw();
    gPad->WaitPrimitive();
    effAfterUnf->Draw();
    gPad->WaitPrimitive();
  }
  
  TH1F* nch_toUnfold = (TH1F*) nch_REC->Clone("nch_toUnfold");
  
  //unfVersion
  //----------
  //0: do nothing
  //1: do trigger eff on track lvl
  if(unfVersion==1) nch_toUnfold->Divide(nch_toUnfold,eff_trTrigSel,1,1);
  //2: do trig+vtx==evtSel on track lvl
  if(unfVersion==2) nch_toUnfold->Divide(nch_toUnfold,eff_trEvtSel,1,1);


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
  else if (hyp == 2)
    hypothesis = nch_toUnfold;  //nch_REC;       //CHANGED with unfVersion
  if(debug_!=0) {
    hypothesis->Draw();
    gPad->WaitPrimitive();
  }        
  
  
  //reweighting of the mtx
  bool reweightmtx = true;
  //if(E==7.) reweightmtx = true;
  //TH1D* projX = new TH1D();
  TH1D* projY = new TH1D();
  cout << "Reweighting the matrix : " << reweightmtx << endl;
  if(reweightmtx){

    matrixhist->Write("nch_matrix_beforeReweighting");

    projY = (TH1D*) (matrixhist->ProjectionY("_py",1,matrixhist->GetNbinsY()));
    //reweighting the reco side
    for(int ireco = 1 ; ireco <= matrixhist->GetNbinsY() ; ++ireco){
      double factor = 0;
      if(projY->GetBinContent(ireco)!=0)
        factor =  nch_toUnfold->GetBinContent(ireco) / projY->GetBinContent(ireco);

      double sumb = 0 , suma = 0;
      for(int igen = 1 ; igen <= matrixhist->GetNbinsX() ; ++igen){
        sumb += matrixhist->GetBinContent(igen , ireco);
        matrixhist->SetBinContent(igen , ireco ,  matrixhist->GetBinContent(igen , ireco) * factor );
        suma += matrixhist->GetBinContent(igen , ireco);
      }

      //cout << "bin " << ireco << "  before = " << sumb << "( " << projY->GetBinContent(ireco) << " )   after = " << suma << "( " << nch_toUnfold->GetBinContent(ireco) << " )"  << endl;

    }
  }

  
  
  //calling the unfolding.cc file
  nch_unfolded = runalgo(matrixhist,nch_toUnfold,hypothesis,niter);
   
  TH1F* nch_unfoldedPtr = (TH1F*) nch_unfolded.Clone("nch_unfoldedPtr");

  //cout<< nch_unfoldedPtr->GetBinContent(31) << "  " << nch_unfoldedPtr->GetBinError(31) << endl;
 
 
 
  //------------------------------------------------------------------------------
  //---------------------------- Resampling & moments ----------------------------
  //------------------------------------------------------------------------------  
  cout<<" ++++ DOING RESAMPLING & MOMENTS' STAT ERRORS ++++" <<endl;
  
  TMoments* moment = new TMoments();
  
  gDirectory->mkdir("hist_resampling");
  gDirectory->cd("hist_resampling");

  
  int niter_resampling = 50; //50
  if(syst != 0) niter_resampling = 0;
  //if(mcfile.Contains("_genTr")) niter_resampling = 0;
  //if(mcfile.Contains("_noweight")) niter_resampling = 0;
  
  cout << "WARNING !! The resampling is done with " << niter_resampling << " iterations ..." << endl;
  
  // already get ptcorr_val  because it is needed in resampling
  double ptcorr_val = 0;  
  if(doptcorr && ( lastpartmc.Contains("cut1") || lastpartmc.Contains("cut5") ) && ( dirmc.Contains("E_0.9") || dirmc.Contains("E_7") ) ){      
     cout << "PT corr will be done" << endl;
     if(dirmc.Contains("E_0.9")) {
        if(lastpartmc.Contains("nocut")) {
            if (lastpartmc.Contains("cut1")) ptcorr_val = 2.3;
            if (lastpartmc.Contains("cut5")) ptcorr_val = 2.0;
        }    
     }
     if(dirmc.Contains("E_7")) {
        if(lastpartmc.Contains("nocut")) {
            if (lastpartmc.Contains("cut1")) ptcorr_val = 2.6;
            if (lastpartmc.Contains("cut5")) ptcorr_val = 2.3;
        }
     }    
     if(dirmc.Contains("E_2.76")) {
        cout << "still need to insert numbers for 2.76TeV!!!" << endl;
     }               
  }
  else cout << " no pt frac correction will be done" << endl;
  
  TH1F nch_resampled = resample(matrixhist,nch_REC,nch_toUnfold,nch_unfoldedPtr,hypothesis,niter_resampling,0,moment,effBeforeUnf,effAfterUnf,false,false,nch_SD, ptcorr_val); //,nch_evtSel_SD
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
    
  TH1F nch_mtxresampled = mtxresample(matrixhist,nch_toUnfold,hypothesis,niter_resampling,moment,effAfterUnf);
  TH1F* nch_mtxresampledPtr = &nch_mtxresampled;
  
  gDirectory->cd("../");
  
  
  //WARNING : BE CAREFUL TO WAY RESAMPLING IS PERFORMED !!
 /* done below 
    
  for(int nbin = 1 ; nbin<=nch_unfoldedPtr->GetNbinsX() ; ++nbin){
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
  
  //different stuff for unfVersion  
  //0: evtSel+centrSel on particle lvl
  //1: vtxSel+centrSel on particle lvl    
  //2: centrSel on particle lvl
  nch_corrected->Divide(nch_corrected,effAfterUnf,1,1);

  /*
  nch_corrected->Scale(1./nch_corrected->Integral());
  nch_trueGen->Scale(1./nch_trueGen->Integral());
  nch_trueGen_afterUnfolding->Scale(1./nch_trueGen_afterUnfolding->Integral());*/




  //------------------------------------------------------------------------------
  //--------------------------------- Pt correc ----------------------------------
  //------------------------------------------------------------------------------

  TH1F* nch_evtSelCorr = (TH1F*) nch_corrected->Clone("nch_data_evtSelCorr");
  cout << " ++++ DOING PT FRAC CORRECTION ++++" << endl;
        
    //if(syst==700)
    //  ptcorr_val += 1. * syst_sign;
    
  increaseNTracks(nch_corrected , +1, ptcorr_val , 1);  //+1 is the sign


  
  cout << "Mean of multiplicity --------> " << nch_corrected->GetMean() << endl;
  cout << "RMS of multiplicity  --------> " << nch_corrected->GetRMS()  << endl;



  //------------------------------------------------------------------------------
  //--------------------------   Adding Stat Errors   ----------------------------
  //--------------------------   to the final curve   ----------------------------
  //------------------------------------------------------------effBeforeUnf------------------
  
  for(int nbin = 1 ; nbin<=nch_corrected->GetNbinsX() ; ++nbin){
    nch_corrected->SetBinError(nbin , sqrt(pow(nch_resampledPtr->GetBinError(nbin),2)+pow(nch_mtxresampledPtr->GetBinError(nbin),2)));
    //cout<<nbin<<"  "<<nch_resampledPtr->GetBinError(nbin)<<"  "<<nch_mtxresampledPtr->GetBinError(nbin)<<endl;
    //cout<<"  "<<sqrt(pow(nch_resampledPtr->GetBinError(nbin),2)+pow(nch_mtxresampledPtr->GetBinError(nbin),2))
    //    <<"  "<<nch_unfoldedPtr->GetBinError(nbin)<<"  "<<nch_corrected->GetBinError(nbin) << "  "<< nch_corrected->GetBinContent(nbin) <<endl;
  } 
  nch_corrected->SetDrawOption("e0");
  /*
    TCanvas* d_resample = new TCanvas("resampling","resampling",1460,510,500,500);
    d_resample->cd();
    //nch_corrected->SetLineColor(kBlack);
    nch_corrected->Draw("hist");
    nch_corrected->Draw("e0"); 
    gPad->WaitPrimitive(); */
  //BLABLABLA
  
/*  TH1F* nch_clone1 = (TH1F*) nch_corrected->Clone("clone1");  //divided by 2 eff and reclone 
  TH1F* nch_clone2 = (TH1F*) nch_corrected->Clone("clone2");  //divided by 2 eff and reclone
  
  nch_clone1->Divide(nch_clone1,eff,1,1);
  nch_clone2->Divide(nch_clone2, eff_centrSel,1,1);
  nch_clone2->Divide(nch_clone2, eff_evtSel,1,1);
  
  for(int i = 1 ; i<=nch_clone1->GetNbinsX() ; ++i){
  double test3= nch_clone1->GetBinContent(i)-nch_clone2->GetBinContent(i);
   double test2= nch_clone1->GetBinError(i)-nch_clone2->GetBinError(i);
    if (nch_clone1->GetBinContent(i) != 0.) cout <<"Bin content different !" << test3/nch_clone1->GetBinContent(i) << endl;
    if (nch_clone1->GetBinContent(i) != 0.) cout <<"Errors are different !" << test2/nch_clone1->GetBinContent(i) << endl;
  }
*/ 

 
  //------------------------------------------------------------------------------
  //--------------------------------- Rescaling ----------------------------------
  //------------------------------------------------------------------------------
  cout <<" ++++ RESCALING ++++" << endl;
  
  divideByWidth(nch_trueGen);
  divideByWidth(nch_trueGen_afterUnfolding);
  if(unfVersion==1)  divideByWidth(nch_trueGenAfterVtxSel);
  if(unfVersion==0)  divideByWidth(nch_trueGenAfterEvtSel);
  divideByWidth(nch_evtSelCorr);
  divideByWidth(nch_REC);
  divideByWidth(nch_unfoldedPtr);
  //if(doPTcorr) divideByWidth(nch_evtSelCorr);
  divideByWidth(nch_corrected);
  divideByWidth(nch_resampledPtr);
  divideByWidth(nch_mtxresampledPtr);
  divideByWidth(nch_MC_RECO_RAW);
  divideByWidth(matrixhist);
  divideByWidth(hypothesis);

  
  //divideByWidth(projX);
  //divideByWidth(projY);

  
  
  //nch_corrected->Scale(1./nch_corrected->Integral());
  
    
  
  //------------------------------------------------------------------------------
  //--------------------------------- Moments ------------------------------------
  //------------------------------------------------------------------------------     
  cout << " ++++ MOMENTS ++++" << endl;  
  gDirectory->mkdir("moments");
  gDirectory->cd("moments");

  moment->Add(nch_corrected);
  moment->ComputeMoments();
  //moment->print();
  //moment->ComputeErrorsFromResampling(nch_corrected);
  moment->print();
  moment->Write("moments");
  
  gDirectory->cd("../");
  
  
  //------------------------------------------------------------------------------
  //----------------------------------  Fits -------------------------------------
  //------------------------------------------------------------------------------  
  //cout<<" ++++ FITS ++++" <<endl;  
  //#include "fitting.C"  

  
  
  //------------------------------------------------------------------------------
  //----------------------------------  Writing to file --------------------------
  //------------------------------------------------------------------------------
  cout << " ++++ WRITING TO FILE ++++" << endl;
  hypothesis->Write("hypothesis");
  nch_trueGen->Write();
  if(unfVersion==1)  nch_trueGenAfterVtxSel->Write();
  if(unfVersion==0)  nch_trueGenAfterEvtSel->Write();
  nch_trueGen_afterUnfolding->Write();
  nch_REC->Write("nch_data_raw");
  if(lastpartmc.Contains("HF1")) nch_SD->Write("nch_SD_mc");
  nch_unfoldedPtr->Write();
  nch_corrected->Write();
  TH1F* nch_cor_norm = (TH1F*) nch_corrected->Clone("nch_corrected_norm");
  nch_cor_norm->Scale(10000*1./nch_cor_norm->Integral());    //NEED TO ADJUST with bin !=1
  nch_cor_norm->Write();
  eff_trEvtSel->Write();
  eff_trTrigSel->Write();
  eff_evtSel->Write();
  eff_vtxSel->Write();
  eff_centrSel->Write();
  if( unfVersion!=0 )  effBeforeUnf->Write();
  effAfterUnf->Write();
  nch_evtSelCorr->Write();
  nch_resampledPtr->Write();
  nch_mtxresampledPtr->Write();
  nch_MC_RECO_RAW->Write("nch_mc_reco_raw");
  matrixhist->Write();
  
  mc->Close(); 
  data->Close();
  if(zerobias) zerobias->Close();
  out->Close();
  
  cout << " ++++ DELETING PTRS ++++" << endl;
  delete hypothesis;
  delete nch_cor_norm;
  delete nch_corrected;
  delete nch_trueGen;
  if(unfVersion==1) delete nch_trueGenAfterVtxSel;
  if(unfVersion==0) delete nch_trueGenAfterEvtSel;
  if(lastpartmc.Contains("HF1")) delete nch_SD;
  delete nch_unfoldedPtr;
  delete nch_REC;
  delete nch_toUnfold;
  delete eff_evtSel;
  delete eff_centrSel;
  if( unfVersion!=0 ) delete effBeforeUnf;
  delete effAfterUnf;
  delete nch_evtSelCorr;
  delete nch_MC_RECO_RAW;
  delete matrixhist;
  delete moment;
  delete out;
  
}

//_____________________________________________________________________________
void SaturateEfficiencyCurve(TH1F* eff_curve){
  int switch_eff=0;
  for (int i=1;i<=eff_curve->GetNbinsX();i++) { 
    if ( (i>60 && eff_curve->GetBinContent(i)==0) || switch_eff==1 ) {
        eff_curve->SetBinContent(i,1);
        switch_eff= 1;
    }       
  }
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


//--------------------------------------------------------------

TH1F* getEff(TFile* f , TString upstr , TString downstr , TString out){
  TH1F* up   = getHist<TH1F>(*f , upstr);
  TH1F* down = getHist<TH1F>(*f , downstr);

  TH1F* eff = (TH1F*) up->Clone(out);
  eff->Divide(up , down , 1, 1, "B");

  eff->SetMinimum(0);

  delete up; delete down;
  return eff;
}
