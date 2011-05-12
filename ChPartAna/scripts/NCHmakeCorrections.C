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

#include "NCHresamplings.C"
//#include "moments.C"

//TrackSYS:
#include "NCHincreaseNTracks.C"

//NiterSYS:
//is included in the code itself #include "syst_niter.C"

#include "fitting_funcs.C"

//#include "makeFakeMatrix.C"

void makeCorrectionsSten(int typeMC = 60, const TString mcfile ="", const TString datafile ="", const TString outputpart ="output", 
                         const TString dirmc="", const TString dirRECO="", const TString lastpartmc1="", const TString lastpartmc2="", 
                         const TString lastpartRECO="", const TString lastpartmc_nosel="", bool useData = true, 
                         int hyp=1, int niter=5, int syst = 0, int syst_sign = 0); 
                         //, bool drawcanv = true, float mu = 14, float sigma = 15); //double Emc = 7.0, double Edata= 7.0);


//////////////////////////////////////
//MAIN PROGRAM

// nohup root -l ../macro/BuildLibDico.C+ ../scripts/NCHmakeCorrections.C+"(\"v8\",60,0,-1,-1,true,0,7.0,7.0,0,0,true,false,false)" -q > logUnf_Data12MwithMC15.txt &
// nohup root -l ../macro/BuildLibDico.C+ ../scripts/NCHmakeCorrections.C+"(\"v0\",60,0,2000000,-2,true,0,7.0,7.0,100,1,true,false,false)" -q > logUnf_TrackSys.txt &
//_____________________________________________________________________________
void NCHmakeCorrections(TString dir_out = "", int typeMC = 60, int hf_int = 0, int Nrunmc = -1, int Nrundata = 4000000, 
         bool useData =true, int cut_tag = 0, double Emc = 7.0, double Edata = 7.0, int syst = 0, int syst_sign = 0, bool switch_memory = true, bool noweight = false, bool genTr = false, int tracking_int = 0) {
    //unfodling is only done for the central cut: centr_tag
    
    if (Nrunmc==-1) {
        if(typeMC==60)  Nrunmc =  4706282;
        if(typeMC==15)  Nrunmc = 10966400;
    }
    if(Nrunmc==-2) {
        if(typeMC==31) Nrunmc = 1886500;
        else {
            cout << "wrong Nrunmc --> exit" << endl;
            return;
        }
    }    
    if(Nrundata==-1) {
        Nrundata=12136718;
    }    
    if(Nrundata==-2) {
        Nrundata=342220;
    }
          
    ////////////////
    // SWITCH
    TString dir = "../macro/outputs_full/"+dir_out+"/";
    bool  only1test=false;
    //////////////////
    
    stringstream cut_unf("");     cut_unf     << "_cut"  << cut_tag ; 
    //Creating Filenames    
    stringstream nEvtsmc("");     nEvtsmc     << "_"  << Nrunmc ;
    stringstream nEvtsdata("");   nEvtsdata   << "_"  << Nrundata ; 
    stringstream type("");        type        << ""   << typeMC;
    stringstream energymc("");    energymc    << "_"  << Emc;
    stringstream energydata("");  energydata  << "_"  << Edata;
    
    TString usedatastr="";
    if(!useData) usedatastr="_MCcorrMC";
    
    TString noweight_str = "";
    if (noweight == true ) noweight_str = "_noweight";
    TString tracking = "_ferncTr";
    if (tracking_int == 1) tracking = "_genTr";
    TString track_out = "";
    if (genTr == true) {
        tracking ="_genTr";
        track_out="_genTr";
    }    
    TString mcstr = type.str()+tracking+"_E"+energymc.str()+nEvtsmc.str()+noweight_str;
    TString mcfile      = dir + "output_MC"+mcstr+".root";
    TString datafile    = dir + "output_data"+tracking+"_E"+energydata.str()+nEvtsdata.str()+".root";
    TString outputpart  = "../macro/unfold_outputs/"+dir_out+"/unf_MC"+type.str()+usedatastr+noweight_str+track_out;
    
    //clearing output file 
    cout << "MC   input file: " << mcfile     << endl;
    cout << "Data input file: " << datafile   << endl;
    //cout << "output dir:     " << outputpart <<endl;
    
    //Checking if both files exist
    TFile* mc   = TFile::Open(mcfile  ,"READ");  
    TFile* data = TFile::Open(datafile,"READ");  
    
    if(mc == 0)   cout<<"The MC input file doesn't exist ..."<<endl;   
    if(data == 0) cout<<"The Data input file doesn't exist ..."<<endl;
    if(mc == 0 || data == 0)
         return;
   
   
    //Make vector of all possible cut combinations   
    vector <TString> * acc = new vector <TString>;
        for(int ii=0; ii <18 ; ++ ii) {
            stringstream nb("");   nb << "_cut" << ii;
            acc->push_back(nb.str());        
        }    
    
    vector <TString> * diff = new vector <TString>;
        //diff->push_back("_INEL");     
        //diff->push_back("_DD");
        //diff->push_back("_NSD");
        //diff->push_back("_INEL");
        diff->push_back("_RECO");
    cout << "switch_memory " << switch_memory << endl;          
    vector <TString> * centr = new vector <TString>;
        if (switch_memory == true) {
            cout << "Entered switch_memory true: " << switch_memory << endl;      
            centr->push_back("_nocut");    
            centr->push_back("_MBUEWG");
            centr->push_back("_ALICE");
        }    
        if (switch_memory == false) {
            cout << "Entered switch_memory false: " << switch_memory << endl;  
            centr->push_back("_ATLAS1");
            centr->push_back("_ATLAS2");
            centr->push_back("_ATLAS6");
        }   
    vector <TString> * hf = new vector <TString>;
        for(int ii=0; ii <2 ; ++ ii) {
            if(hf_int !=ii) continue;
            stringstream nb("");    nb << "_HF" << ii;
            hf->push_back(nb.str());        
        }    
    
    if(debug_) {    
        cout << "Printing the varying filestructure" << endl;        
        for(unsigned int ii=0; ii < acc->size() ; ++ ii) {
            cout << ii << "  acc: " << acc->at(ii) <<endl;
        }
        for(unsigned int ii=0; ii <diff->size() ; ++ ii) {
            cout << ii << "  diff: " << diff->at(ii) <<endl;
        }
        for(unsigned int ii=0; ii < centr->size() ; ++ ii) {
            cout << ii << "  centr: " << centr->at(ii) <<endl;
        }
        for(unsigned int ii=0; ii <hf->size() ; ++ ii) {    
            cout << ii << "  hf: " << hf->at(ii) <<endl;
        }
    }
    
    
    //Calling the unfolding program when the directory excists        
    TString dirmc="";   
    TString dirreco="";
    TString lastpartmc1="";
    TString lastpartmc2="";
    TString lastpartmcnosel="";
    TString lastpartRECO="";
    
    TString tempdif="";
    int possibilities=0;
    int goodmc=0;
    int goodmcanddata=0;
    double nIter= 20;
    
    //to remove the message "Error in <TDirectoryFile::cd>: Unknown directory EvtSel_HF6_ATLAS6_RECO_cut17"
    int backup_gErrorIgnoreLevel= gErrorIgnoreLevel;  //=0 normally
    
    for(vector<TString>::const_iterator ac = acc->begin() ; ac != acc->end() ; ++ac){
        for(vector<TString>::const_iterator dif = diff->begin() ; dif != diff->end() ; ++dif){
            for(vector<TString>::const_iterator cen = centr->begin() ; cen != centr->end() ; ++cen){
                for(vector<TString>::const_iterator h = hf->begin() ; h != hf->end() ; ++h){
                    gErrorIgnoreLevel=3001; //to ignore errors
                    //if(*dif=="_RECO") continue; // if MC=RECO, we can not do unfolding, we need SD DD NSD or INEL
                    if(*ac!=cut_unf.str()) continue; //only do the program for 1 central cut  || bad fix to exclude the memory leaks
                    
                    //if(*cen!="_ATLAS6") continue;
                   // if(*h!="_HF1") continue;
                    
                    tempdif=*dif;
                    if(*h=="_HF1") tempdif="_NSD";   //Needed when HF=1,   then INEL-SD=NSD, so you need NSD eff, matrix,...
                    
                    possibilities++;                    
                    dirmc="Difflvl"+*ac+"/Centrlvl"+tempdif+*ac+"/HFlvl"+*cen+tempdif+*ac+"/EvtSel"+*h+*cen+tempdif+*ac+"/";
                    cout <<"dirmc    " << dirmc <<endl;                     
                    if(! mc->cd(dirmc) ) 
                        continue; //if no dir
                    goodmc++;
                    
                    dirreco="Difflvl"+*ac+"/Centrlvl_RECO"+*ac+"/HFlvl"+*cen+"_RECO"+*ac+"/EvtSel"+*h+*cen+"_RECO"+*ac+"/"; 
                    if(! data->cd(dirreco) )      
                        continue; //if no dir
                    
                    
                    goodmcanddata++;
                    lastpartmc1    ="_partfull";
                    lastpartmc2    =*h + *cen + tempdif    + *ac;
                    lastpartmcnosel="_partnoSel"+ *h + *cen + tempdif    + *ac;
                    lastpartRECO   =    "_full" + *h + *cen + "_RECO" + *ac;
                    
                    if      (*h =="_HF0" && *cen=="_nocut"  && (*ac=="_cut0" || *ac=="_cut1")) nIter = 4;
                    else if (*h =="_HF0" && *cen=="_MBUEWG" && (*ac=="_cut0" || *ac=="_cut1")) nIter = 4;
                    else if (*h =="_HF0" && *cen=="_ALICE"  && (*ac=="_cut0" || *ac=="_cut1")) nIter = 4;                  
                    else if (*h =="_HF0" && *cen=="_ATLAS1" && (*ac=="_cut0" || *ac=="_cut1")) nIter = 4;                    
                    else if (*h =="_HF0" && *cen=="_ATLAS2" && (*ac=="_cut0" || *ac=="_cut1")) nIter = 3;                   
                    else if (*h =="_HF0" && *cen=="_ATLAS6" && (*ac=="_cut0" || *ac=="_cut1")) nIter = 2;
                    
                    else if (*h =="_HF0" && *cen=="_nocut"  && *ac=="_cut2") nIter = 3;
                    else if (*h =="_HF0" && *cen=="_MBUEWG" && *ac=="_cut2") nIter = 3;
                    else if (*h =="_HF0" && *cen=="_ALICE"  && *ac=="_cut2") nIter = 2;                  
                    else if (*h =="_HF0" && *cen=="_ATLAS1" && *ac=="_cut2") nIter = 4;                    
                    else if (*h =="_HF0" && *cen=="_ATLAS2" && *ac=="_cut2") nIter = 3;                   
                    else if (*h =="_HF0" && *cen=="_ATLAS6" && *ac=="_cut2") nIter = 2;   
                    
                    else if (*h =="_HF1") nIter = 5;                                   
                    else { 
                        nIter = 3;
                        cout << "nIter was not set ! It stays: " << nIter <<endl;
                    }
                    
                    if(debug_) cout << "usedata beginning main program " << useData << endl;
                    //errors back on for the unfolding code
                    gErrorIgnoreLevel=backup_gErrorIgnoreLevel; 

                    makeCorrectionsSten(typeMC, mcfile, datafile, outputpart, dirmc, dirreco, lastpartmc1, lastpartmc2, 
                                        lastpartRECO, lastpartmcnosel, useData, 1, nIter, syst, syst_sign); //Emc, Edata);                        
                    if (only1test) break;
                }
                if (only1test) break;    
            }
            if (only1test) break;
        }
        if (only1test) break;
    }  
     
   cout <<"Good (MC+Data): " << goodmcanddata << " out of " << goodmc << " MC files out of " << possibilities << " possibilities." << endl;
     
   mc->Close();
   data->Close();
   delete acc;
   delete diff;
   delete centr;
   delete hf;
   cout << "---------------------------" << endl << "END OF NCHmakeCorrections()" << endl << "---------------------------" <<endl;       
}    /////////////////////////////////////

//_________________________________________________________________________________________________________________
void RunForEffProblem(){
    
    //cut1
    makeCorrectionsSten( 15,
    "../macro/outputs_full/v21/output_MC15_ferncTr_E_7_5000000.root",
    "../macro/outputs_full/v21/output_data_ferncTr_E_7_342220.root",
    "../macro/unfold_outputs/v21b/TESTunf_MC15",
    "Difflvl_cut2/Centrlvl_NSD_cut2/HFlvl_ATLAS2_NSD_cut2/EvtSel_HF1_ATLAS2_NSD_cut2/",
    "Difflvl_cut2/Centrlvl_RECO_cut2/HFlvl_ATLAS2_RECO_cut2/EvtSel_HF1_ATLAS2_RECO_cut2/",
    "_partfull","_HF1_ATLAS2_NSD_cut2","_full_HF1_ATLAS2_RECO_cut2","_partnoSel_HF1_ATLAS2_NSD_cut2",1,1,4,0,0);
}


   
//_________________________________________________________________________________________________________________
void RunForNCHProblem(){
    
    // ATLAS 2 cut 0
   
   /* makeCorrectionsSten( 31,
    "../macro/outputs_full/v22/output_MC31_ferncTr_E_7_5000000.root",
    "../macro/outputs_full/v22/output_data_ferncTr_E_7_342220.root",
    "../macro/unfold_outputs/v22_test/TESTunf_MC31",
    "Difflvl_cut0/Centrlvl_RECO_cut0/HFlvl_ATLAS2_RECO_cut0/EvtSel_HF0_ATLAS2_RECO_cut0/",
    "Difflvl_cut0/Centrlvl_RECO_cut0/HFlvl_ATLAS2_RECO_cut0/EvtSel_HF0_ATLAS2_RECO_cut0/",
    "_partfull","_HF0_ATLAS2_RECO_cut0","_full_HF0_ATLAS2_RECO_cut0","_partnoSel_HF0_ATLAS2_RECO_cut0",1,1,4,0,0);
    
    
    makeCorrectionsSten( 31,
    "../macro/outputs_full/v22/output_MC31_ferncTr_E_7_5000000.root",
    "../outputs_sten/output_data_ferncTr_E_7_342220_NCHCode_362_132440only_0.root",
    "../macro/unfold_outputs/v22_test/unf_362_132440only_MC31",
    "Difflvl_cut0/Centrlvl_RECO_cut0/HFlvl_ATLAS2_RECO_cut0/EvtSel_HF0_ATLAS2_RECO_cut0/",
    "Difflvl_cut0/Centrlvl_RECO_cut0/HFlvl_ATLAS2_RECO_cut0/EvtSel_HF0_ATLAS2_RECO_cut0/",
    "_partfull","_HF0_ATLAS2_RECO_cut0","_full_HF0_ATLAS2_RECO_cut0","_partnoSel_HF0_ATLAS2_RECO_cut0",1,1,4,0,0);
    
    makeCorrectionsSten( 31,
    "../macro/outputs_full/v22/output_MC31_ferncTr_E_7_5000000.root",
    "../outputs_sten/output_data_ferncTr_E_7_1063195_NCHCode_336_132440only_0.root",
    "../macro/unfold_outputs/v22_test/unf_336_132440only_MC31",
    "Difflvl_cut0/Centrlvl_RECO_cut0/HFlvl_ATLAS2_RECO_cut0/EvtSel_HF0_ATLAS2_RECO_cut0/",
    "Difflvl_cut0/Centrlvl_RECO_cut0/HFlvl_ATLAS2_RECO_cut0/EvtSel_HF0_ATLAS2_RECO_cut0/",
    "_partfull","_HF0_ATLAS2_RECO_cut0","_full_HF0_ATLAS2_RECO_cut0","_partnoSel_HF0_ATLAS2_RECO_cut0",1,1,4,0,0);
    
    makeCorrectionsSten( 31,
    "../macro/outputs_full/v22/output_MC31_ferncTr_E_7_5000000.root",
    "../macro/outputs_full/v21/output_data_ferncTr_E_7_342220.root",
    "../macro/unfold_outputs/v22_test/unf_v21_MC31",
    "Difflvl_cut0/Centrlvl_RECO_cut0/HFlvl_ATLAS2_RECO_cut0/EvtSel_HF0_ATLAS2_RECO_cut0/",
    "Difflvl_cut0/Centrlvl_RECO_cut0/HFlvl_ATLAS2_RECO_cut0/EvtSel_HF0_ATLAS2_RECO_cut0/",
    "_partfull","_HF0_ATLAS2_RECO_cut0","_full_HF0_ATLAS2_RECO_cut0","_partnoSel_HF0_ATLAS2_RECO_cut0",1,1,4,0,0);
    
    // ATLAS 6 cut 2
    
    makeCorrectionsSten( 31,
    "../macro/outputs_full/v22/output_MC31_ferncTr_E_7_5000000.root",
    "../macro/outputs_full/v22/output_data_ferncTr_E_7_342220.root",
    "../macro/unfold_outputs/v22_test/TESTunf_MC31",
    "Difflvl_cut2/Centrlvl_RECO_cut2/HFlvl_ATLAS6_RECO_cut2/EvtSel_HF0_ATLAS6_RECO_cut2/",
    "Difflvl_cut2/Centrlvl_RECO_cut2/HFlvl_ATLAS6_RECO_cut2/EvtSel_HF0_ATLAS6_RECO_cut2/",
    "_partfull","_HF0_ATLAS6_RECO_cut2","_full_HF0_ATLAS6_RECO_cut2","_partnoSel_HF0_ATLAS6_RECO_cut2",1,1,4,0,0);
    
    
    makeCorrectionsSten( 31,
    "../macro/outputs_full/v22/output_MC31_ferncTr_E_7_5000000.root",
    "../outputs_sten/output_data_ferncTr_E_7_342220_NCHCode_362_132440only_0.root",
    "../macro/unfold_outputs/v22_test/unf_362_132440only_MC31",
    "Difflvl_cut2/Centrlvl_RECO_cut2/HFlvl_ATLAS6_RECO_cut2/EvtSel_HF0_ATLAS6_RECO_cut2/",
    "Difflvl_cut2/Centrlvl_RECO_cut2/HFlvl_ATLAS6_RECO_cut2/EvtSel_HF0_ATLAS6_RECO_cut2/",
    "_partfull","_HF0_ATLAS6_RECO_cut2","_full_HF0_ATLAS6_RECO_cut2","_partnoSel_HF0_ATLAS6_RECO_cut2",1,1,4,0,0);
    
    makeCorrectionsSten( 31,
    "../macro/outputs_full/v22/output_MC31_ferncTr_E_7_5000000.root",
    "../outputs_sten/output_data_ferncTr_E_7_1063195_NCHCode_336_132440only_0.root",
    "../macro/unfold_outputs/v22_test/unf_336_132440only_MC31",
    "Difflvl_cut2/Centrlvl_RECO_cut2/HFlvl_ATLAS6_RECO_cut2/EvtSel_HF0_ATLAS6_RECO_cut2/",
    "Difflvl_cut2/Centrlvl_RECO_cut2/HFlvl_ATLAS6_RECO_cut2/EvtSel_HF0_ATLAS6_RECO_cut2/",
    "_partfull","_HF0_ATLAS6_RECO_cut2","_full_HF0_ATLAS6_RECO_cut2","_partnoSel_HF0_ATLAS6_RECO_cut2",1,1,4,0,0);
   
    makeCorrectionsSten( 31,
    "../macro/outputs_full/v22/output_MC31_ferncTr_E_7_5000000.root",
    "../macro/outputs_full/v21/output_data_ferncTr_E_7_342220.root",
    "../macro/unfold_outputs/v22_test/unf_v21_MC31",
    "Difflvl_cut2/Centrlvl_RECO_cut2/HFlvl_ATLAS6_RECO_cut2/EvtSel_HF0_ATLAS6_RECO_cut2/",
    "Difflvl_cut2/Centrlvl_RECO_cut2/HFlvl_ATLAS6_RECO_cut2/EvtSel_HF0_ATLAS6_RECO_cut2/",
    "_partfull","_HF0_ATLAS6_RECO_cut2","_full_HF0_ATLAS6_RECO_cut2","_partnoSel_HF0_ATLAS6_RECO_cut2",1,1,4,0,0);
    
    
   
    // ALICE cut 4
    
    makeCorrectionsSten( 31,
    "../macro/outputs_full/v22/output_MC31_ferncTr_E_7_5000000.root",
    "../macro/outputs_full/v22/output_data_ferncTr_E_7_342220.root",
    "../macro/unfold_outputs/v22_test/TESTunf_MC31",
    "Difflvl_cut4/Centrlvl_RECO_cut4/HFlvl_ALICE_RECO_cut4/EvtSel_HF0_ALICE_RECO_cut4/",
    "Difflvl_cut4/Centrlvl_RECO_cut4/HFlvl_ALICE_RECO_cut4/EvtSel_HF0_ALICE_RECO_cut4/",
    "_partfull","_HF0_ALICE_RECO_cut4","_full_HF0_ALICE_RECO_cut4","_partnoSel_HF0_ALICE_RECO_cut4",1,1,4,0,0);
    
    
    makeCorrectionsSten( 31,
    "../macro/outputs_full/v22/output_MC31_ferncTr_E_7_5000000.root",
    "../outputs_sten/output_data_ferncTr_E_7_342220_NCHCode_362_132440only_0.root",
    "../macro/unfold_outputs/v22_test/unf_362_132440only_MC31",
    "Difflvl_cut4/Centrlvl_RECO_cut4/HFlvl_ALICE_RECO_cut4/EvtSel_HF0_ALICE_RECO_cut4/",
    "Difflvl_cut4/Centrlvl_RECO_cut4/HFlvl_ALICE_RECO_cut4/EvtSel_HF0_ALICE_RECO_cut4/",
    "_partfull","_HF0_ALICE_RECO_cut4","_full_HF0_ALICE_RECO_cut4","_partnoSel_HF0_ALICE_RECO_cut4",1,1,4,0,0);
    
    makeCorrectionsSten( 31,
    "../macro/outputs_full/v22/output_MC31_ferncTr_E_7_5000000.root",
    "../outputs_sten/output_data_ferncTr_E_7_1063195_NCHCode_336_132440only_0.root",
    "../macro/unfold_outputs/v22_test/unf_336_132440only_MC31",
    "Difflvl_cut4/Centrlvl_RECO_cut4/HFlvl_ALICE_RECO_cut4/EvtSel_HF0_ALICE_RECO_cut4/",
    "Difflvl_cut4/Centrlvl_RECO_cut4/HFlvl_ALICE_RECO_cut4/EvtSel_HF0_ALICE_RECO_cut4/",
    "_partfull","_HF0_ALICE_RECO_cut4","_full_HF0_ALICE_RECO_cut4","_partnoSel_HF0_ALICE_RECO_cut4",1,1,4,0,0);
   
    makeCorrectionsSten( 31,
    "../macro/outputs_full/v22/output_MC31_ferncTr_E_7_5000000.root",
    "../macro/outputs_full/v21/output_data_ferncTr_E_7_342220.root",
    "../macro/unfold_outputs/v22_test/unf_v21_MC31",
    "Difflvl_cut4/Centrlvl_RECO_cut4/HFlvl_ALICE_RECO_cut4/EvtSel_HF0_ALICE_RECO_cut4/",
    "Difflvl_cut4/Centrlvl_RECO_cut4/HFlvl_ALICE_RECO_cut4/EvtSel_HF0_ALICE_RECO_cut4/",
    "_partfull","_HF0_ALICE_RECO_cut4","_full_HF0_ALICE_RECO_cut4","_partnoSel_HF0_ALICE_RECO_cut4",1,1,4,0,0);
    
  */
     makeCorrectionsSten( 31,
    "../macro/outputs_full/v22NoWeight/output_MC31_ferncTr_E_7_5000000_noweight.root",
    "../macro/outputs_full/v22NoWeight/output_data_ferncTr_E_7_342220.root",
    "../macro/unfold_outputs/v22_test/unf_v21_MC31",
    "Difflvl_cut1/Centrlvl_NSD_cut1/HFlvl_nocut_NSD_cut1/EvtSel_HF1_nocut_NSD_cut1/",
    "Difflvl_cut1/Centrlvl_RECO_cut1/HFlvl_nocut_RECO_cut1/EvtSel_HF1_nocut_RECO_cut1/",
    "_partfull","_HF1_nocut_NSD_cut1","_full_HF1_nocut_RECO_cut1","_partnoSel_HF1_nocut_NSD_cut1",0,1,4,0,0);
    
    
    
}


//_________________________________________________________________________________________________________________
void RunForPtCorrComparision(){
    
    //cut1
    makeCorrectionsSten( 31,
    "../macro/outputs_full/v21/output_MC31_ferncTr_E_7_5000000.root",
    "../macro/outputs_full/v21/output_data_ferncTr_E_7_342220.root",
    "../macro/unfold_outputs/v21_ptcorr/unf_MC31",
    "Difflvl_cut1/Centrlvl_NSD_cut1/HFlvl_nocut_NSD_cut1/EvtSel_HF1_nocut_NSD_cut1/",
    "Difflvl_cut1/Centrlvl_RECO_cut1/HFlvl_nocut_RECO_cut1/EvtSel_HF1_nocut_RECO_cut1/",
    "_partfull","_HF1_nocut_NSD_cut1","_full_HF1_nocut_RECO_cut1","_partnoSel_HF1_nocut_NSD_cut1",1,1,4,0,0);
    
    makeCorrectionsSten( 60,
    "../macro/outputs_full/v21_900/output_MC60_ferncTr_E_0.9_5000000_noweight.root",
    "../macro/outputs_full/v21_900/output_data_ferncTr_E_0.9_2250000.root",
    "../macro/unfold_outputs/v21_900_ptcorr/unf_MC60",
    "Difflvl_cut1/Centrlvl_NSD_cut1/HFlvl_nocut_NSD_cut1/EvtSel_HF1_nocut_NSD_cut1/",
    "Difflvl_cut1/Centrlvl_RECO_cut1/HFlvl_nocut_RECO_cut1/EvtSel_HF1_nocut_RECO_cut1/",
    "_partfull","_HF1_nocut_NSD_cut1","_full_HF1_nocut_RECO_cut1","_partnoSel_HF1_nocut_NSD_cut1",1,1,4,0,0);
    
    
    //cut5
    makeCorrectionsSten( 31,
    "../macro/outputs_full/v21/output_MC31_ferncTr_E_7_5000000.root",
    "../macro/outputs_full/v21/output_data_ferncTr_E_7_342220.root",
    "../macro/unfold_outputs/v21_ptcorr/unf_MC31",
    "Difflvl_cut5/Centrlvl_NSD_cut5/HFlvl_nocut_NSD_cut5/EvtSel_HF1_nocut_NSD_cut5/",
    "Difflvl_cut5/Centrlvl_RECO_cut5/HFlvl_nocut_RECO_cut5/EvtSel_HF1_nocut_RECO_cut5/",
    "_partfull","_HF1_nocut_NSD_cut5","_full_HF1_nocut_RECO_cut5","_partnoSel_HF1_nocut_NSD_cut5",1,1,4,0,0);
    
    makeCorrectionsSten( 60,
    "../macro/outputs_full/v21_900/output_MC60_ferncTr_E_0.9_5000000_noweight.root",
    "../macro/outputs_full/v21_900/output_data_ferncTr_E_0.9_2250000.root",
    "../macro/unfold_outputs/v21_900_ptcorr/unf_MC60",
    "Difflvl_cut5/Centrlvl_NSD_cut5/HFlvl_nocut_NSD_cut5/EvtSel_HF1_nocut_NSD_cut5/",
    "Difflvl_cut5/Centrlvl_RECO_cut5/HFlvl_nocut_RECO_cut5/EvtSel_HF1_nocut_RECO_cut5/",
    "_partfull","_HF1_nocut_NSD_cut5","_full_HF1_nocut_RECO_cut5","_partnoSel_HF1_nocut_NSD_cut5",1,1,4,0,0);

}


//_____________________________________________________________________________
void makeCorrectionsSten(int typeMC, const TString mcfile, const TString datafile, const TString outputpart, 
                         const TString dirmc, const TString dirRECO, const TString lastpartmc1, const TString lastpartmc2, 
                         const TString lastpartRECO, const TString lastpartmc_nosel, bool useData,
                         int hyp, int niter, int syst, int syst_sign) { //double Emc, double Edata ) {
                         //, bool drawcanv, float mu, float sigma ){
  bool drawcanv=false;
              
  bool doptcorr=true;   //it is only done for (cut1 and cut5) && (E=0.9||7) && nocut 
  
  if(debug_) cout << "usedata beginning " << useData << endl;
  //detaching cloned TH1's from their original file, so outfile->Close() will not crash the program
  TH1::AddDirectory(kFALSE);
               
  TFile* mc   = TFile::Open(mcfile  ,"READ");  
  TFile* data = TFile::Open(datafile,"READ");
  
  //Checking if both files exist
  if(mc == 0)   cout<<"The MC input file doesn't exist ..."<<endl;   
  if(data == 0) cout<<"The Data input file doesn't exist ..."<<endl;
  if(mc == 0 || data == 0)
    return;
  
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
  TString output_str =  outputpart+lastpartmc+syst_str+ptcorr_str+".root"; 
  cout << "Output file : " << output_str << endl;
      output_str.ReplaceAll("RECO","INEL");
  TFile* out = new TFile(output_str,"RECREATE");
  out->cd();  
  
   
  //------------------------------------------------------------------------------
  //---------------------------- UNFOLDING ---------------------------------------
  //------------------------------------------------------------------------------
  
  gDirectory->mkdir("unfolding"+lastpartmc);
  gDirectory->cd("unfolding"+lastpartmc);
  if (debug_!=0) cout <<"make unfolding dir" <<endl;
  //Get the unfolding matrix
  TH2F* matrixhist=0;  
  matrixhist = (TH2F*) mc->Get(dirmc+"TResponseMtx"+lastpartmc+"/mtx"+lastpartmc);
  if(debug_) cout << "matrixhist "  << dirmc+"TResponseMtx"+lastpartmc+"/mtx"+lastpartmc << endl;
  if(matrixhist==0) {
    cout << "Matrixhist is empty. Can't find: "<< dirmc+"TResponseMtx"+lastpartmc+"/mtx"+lastpartmc <<endl;
    return;
  }
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
  TH1F* nch_trueGen_afterUnfolding=0;
  nch_trueGen_afterUnfolding = (TH1F*) mc->Get(dirmc+"MultiPlots_mp"+lastpartmc+"/nch_mp"+lastpartmc);
  if(debug_) cout << "nch_trueGen_afterUnfolding  " << dirmc+"MultiPlots_mp"+lastpartmc+"/nch_mp"+lastpartmc << endl;
  if(nch_trueGen_afterUnfolding==0) {
    cout << "nch_trueGen_afterUnfolding  " << dirmc+"MultiPlots_mp"+lastpartmc+"/nch_mp"+lastpartmc << endl;
    cout << "nch_MC_gen_afterUnfoldingINEL is empty"<<endl;
    return;
  }
  nch_trueGen_afterUnfolding->SetName("nch_MC_gen_afterUnfolding");  
  if(debug_!=0) {
    cout << "nch_trueGen_afterUnfolding loaded" << endl;
    nch_trueGen_afterUnfolding->Draw();
    gPad->WaitPrimitive();
  } 


  TH1F* nch_trueGen=0;  
  TH1F* nch_trueGenAfterEvtSel=0;
  nch_trueGenAfterEvtSel = (TH1F*) mc->Get(dirmc+"MultiPlots_mp"+lastpartmc_nosel+"/nch_mp"+lastpartmc_nosel);
  if(debug_) cout << " nch_trueGenAfterEvtSel " << dirmc+"MultiPlots_mp"+lastpartmc_nosel+"/nch_mp"+lastpartmc_nosel << endl;
  if(nch_trueGenAfterEvtSel==0) {
    cout << "nch_trueGenAfterEvtSel is empty"<<endl;
    return;
  }
  nch_trueGenAfterEvtSel->SetName("nch_MC_gen_afterEvtSelCorrection");
   
  nch_trueGen = (TH1F*) mc->Get(dirmc+"GenPart_gpp_CentrGen"+lastpartmc2+"/nch_gpp_CentrGen"+lastpartmc2); 
  if(debug_) cout << "nch_trueGen " << dirmc+"GenPart_gpp_CentrGen"+lastpartmc2+"/nch_gpp_CentrGen"+lastpartmc2 << endl;
  if(nch_trueGen==0) {
    cout << "nch_trueGenINEL is empty"<<endl;
    return;
  }
  nch_trueGen->SetName("nch_MC_gen_After_CentrEvtCorr");
  
  if(debug_!=0) {
    cout << "nch_MC_Gen_afterEvtSelCorrection and nch_MC_gen_After_CentrEvtCorr loaded" << endl;
  }
  
 
  //get the nch to Unfold distribution   nchTRACKS
  TH1F* nch_REC=0;
  if(useData) {
    //the RECO Data tracks    
    nch_REC = (TH1F*)  data->Get(dirRECO+"MultiPlots_mpreco"+lastpartRECO+"/nch_mpreco"+lastpartRECO);
  }  
  else {
  // the RECO MC tracks (to check the unfolding procedure)
    nch_REC = (TH1F*)  mc->Get(dirRECO+"MultiPlots_mpreco"+lastpartRECO+"/nch_mpreco"+lastpartRECO);
  }
  if(debug_) cout << "nch_REC  " <<dirRECO+"MultiPlots_mpreco"+lastpartRECO+"/nch_mpreco"+lastpartRECO << endl;
   
  TH1F* nch_MC_RECO_RAW = (TH1F*)  mc->Get(dirRECO+"MultiPlots_mpreco"+lastpartRECO+"/nch_mpreco"+lastpartRECO);
  if(debug_) cout << "nch_MC_RECO_RAW  " << dirRECO+"MultiPlots_mpreco"+lastpartRECO+"/nch_mpreco"+lastpartRECO << endl;
  
/*    //fix bug manually: 
  //if a central cut --> you require at least 1 track --> set zerobin to 0
  //if nocut --> do nothing, stuff is allowed to go in zerobin
  if (! (dirmc.Contains("nocut")) )  {
     nch_REC->SetBinContent(1,0);
     nch_MC_RECO_RAW->SetBinContent(1,0);
     }
*/
  if(nch_REC==0) {
    cout << "nch_REC data is empty"<<endl;
    return;
  }
  if(debug_!=0) {
    cout << "nch_REC data loaded" << endl;
    cout << nch_REC->GetBinContent(1) << " bin content 0 "<< endl;
    nch_REC->Draw();
    gPad->WaitPrimitive();
  }  
  if(debug_) cout << "entries in zero bin: " << nch_REC->GetBinContent(1) <<endl;
  if(debug_) cout << useData << "  usedata " <<endl;
 
  
  
  //TRACK SYSTEMATIC  in NCHincreaceNTracks.C
  if(int(syst/100)==1) {
    double syst_val = 2.53;
    bool printSummary = true;    
    //nch_REC->Draw();
    //gPad->WaitPrimitive();
    increaseNTracks(nch_REC, syst_sign, syst_val,printSummary);    
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
    TString pathSD=dirRECO+"MultiPlots_mpreco"+lastpartRECO+"/nch_mpreco"+lastpartRECO;
    //cout << pathSD << "   lastpartmc  "<< endl;
    pathSD.ReplaceAll("RECO","SD");
    //cout << pathSD << "   lastpartedited  "<< endl;
    TString pathINEL=dirRECO+"MultiPlots_mpreco"+lastpartRECO+"/nch_mpreco"+lastpartRECO;
    pathINEL.ReplaceAll("RECO","RECO");
    

    nch_SD   = (TH1F*) mc->Get(pathSD);
    nch_INEL = (TH1F*) mc->Get(pathINEL);
    
    if(nch_SD==0 || nch_INEL==0) {
    cout << "nch_SD or nch_INEL is empty"<<endl;
    return;
    }
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
  //-------------------------                          ---------------------------
  //------------------------------------------------------------------------------   
  
  
  TH1F* eff_evtSel=0;
  TH1F* eff_centrSel=0;  
  
  //TH1F* up_evtSel= 0; up_evtSel = (TH1F*) mc->FindObjectAny("nch_partfull"+lastpartmc2);
  TH1F* up_evtSel= 0; up_evtSel = (TH1F*) mc->Get(dirmc+"MultiPlots_mp_partfull"+lastpartmc2 +"/nch_mp_partfull"+lastpartmc2);

  //TH1F* down_evtSel= 0; down_evtSel = (TH1F*) mc->FindObjectAny("nch_partnoSel"+lastpartmc2);
  TH1F* down_evtSel= 0; down_evtSel = (TH1F*) mc->Get(dirmc+"MultiPlots_mp_partnoSel"+lastpartmc2 +"/nch_mp_partnoSel"+lastpartmc2);
  
  eff_evtSel = (TH1F*) up_evtSel->Clone("eff_nch"+lastpartmc2);
  eff_evtSel->Divide(up_evtSel, down_evtSel, 1, 1, "B");
  eff_evtSel->SetMinimum(0);
  
  delete up_evtSel; delete down_evtSel;

  //TH1F* up_centrSel= 0; up_centrSel = (TH1F*) mc->FindObjectAny("nch_noSel"+lastpartmc2);
  TH1F* up_centrSel= 0; up_centrSel = (TH1F*) mc->Get(dirmc+"GenPart_gpp_noSel"+lastpartmc2 +"/nch_gpp_noSel"+lastpartmc2);
  //TH1F* down_centrSel= 0; down_centrSel = (TH1F*) mc->FindObjectAny("nch_CentrGen"+lastpartmc2);
  TH1F* down_centrSel= 0; down_centrSel = (TH1F*) mc->Get(dirmc+"GenPart_gpp_CentrGen"+lastpartmc2 +"/nch_gpp_CentrGen"+lastpartmc2);
  
  eff_centrSel = (TH1F*) up_centrSel->Clone("eff_nch_CentrEff"+lastpartmc2);
  eff_centrSel->Divide(up_centrSel, down_centrSel, 1, 1, "B");
  eff_centrSel->SetMinimum(0);
  
  delete up_centrSel; delete down_centrSel; 

  //eff_evtSel   = (TH1F*) mc->Get(dirmc+"Eff"+lastpartmc+"/eff_nch"+lastpartmc);  
  //eff_centrSel = (TH1F*) mc->Get(dirmc+"Eff_CentrEff"+lastpartmc2+"/eff_nch_CentrEff"+lastpartmc2); 
  //if(debug_) cout <<"eff_evtSel  " << dirmc+"Eff"+lastpartmc+"/eff_nch"+lastpartmc << endl;
  //if(debug_) cout <<"eff_centrSel  " << dirmc+"Eff_CentrEff"+lastpartmc2+"/eff_nch_CentrEff"+lastpartmc2 << endl;
  if(debug_) cout << "NBins eff_evtSel: " << eff_evtSel->GetNbinsX() << "  NBins eff_centrSel:   "   << eff_centrSel->GetNbinsX();
  
  if(eff_evtSel==0) {
    cout << "eff_evtSelRECO is empty"<<endl;
    return;
  }  
  if(eff_centrSel==0) {
    cout << "eff_centrSelRECO is empty"<<endl;
    return;
  }
  eff_evtSel->SetName("eff_evtSel");
  eff_centrSel->SetName("eff_centrSel");

  //put the eff to 1 after some time
  int switch_evtSel= 0;
  int switch_centrSel= 0;
  
  for (int i=1;i<=eff_evtSel->GetNbinsX();i++) { 
    if ( (i>60 && eff_evtSel->GetBinContent(i)==0) || switch_evtSel==1 ) {
        eff_evtSel->SetBinContent(i,1);
        switch_evtSel= 1;
    }    
    if ( (i>60 && eff_centrSel->GetBinContent(i)==0) || switch_centrSel==1 ) {
        eff_centrSel->SetBinContent(i,1);
        switch_centrSel= 1;
    }    
  }
  
  TH1F* eff=(TH1F*) eff_evtSel->Clone("eff_total");
  eff->Multiply(eff_centrSel,eff_evtSel,1,1);
    
  if(debug_!=0) {
    eff_evtSel->Draw();
    gPad->WaitPrimitive();
    eff_centrSel->Draw();
    gPad->WaitPrimitive();
    eff->Draw();
    gPad->WaitPrimitive();
  }
 
  
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
  else if (hyp == 2)
    hypothesis = nch_REC;       
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
        factor =  nch_REC->GetBinContent(ireco) / projY->GetBinContent(ireco);

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
 cout<<" ++++ DOING RESAMPLING & MOMENTS' STAT ERRORS ++++" <<endl;
  
  TMoments* moment = new TMoments();
  
  gDirectory->mkdir("hist_resampling");
  gDirectory->cd("hist_resampling");
  
  int niter_resampling = 50;
  if(syst!=0) niter_resampling = 0;
  if(mcfile.Contains("_genTr")) niter_resampling = 0;
  if(mcfile.Contains("_noweight")) niter_resampling = 0;
  
  cout << "WARNING !! The resampling is done with " << niter_resampling << " iterations ..." << endl;

  TH1F nch_resampled = resample(matrixhist,nch_REC,nch_REC,nch_unfoldedPtr,hypothesis,niter_resampling,0,moment,eff,false); //,nch_evtSel_SD
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
    
  TH1F nch_mtxresampled = mtxresample(matrixhist,nch_toUnfold,hypothesis,niter_resampling,moment,eff);
  TH1F* nch_mtxresampledPtr = &nch_mtxresampled;
  
  gDirectory->cd("../");
  
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
    
  nch_corrected->Divide(nch_corrected,eff,1,1);
  /*
  nch_corrected->Scale(1./nch_corrected->Integral());
  nch_trueGen->Scale(1./nch_trueGen->Integral());
  nch_trueGen_afterUnfolding->Scale(1./nch_trueGen_afterUnfolding->Integral());*/




  //------------------------------------------------------------------------------
  //--------------------------------- Pt correc ----------------------------------
  //------------------------------------------------------------------------------

  TH1F* nch_evtSelCorr = (TH1F*) nch_corrected->Clone("nch_data_evtSelCorr");
  double ptcorr_val=0;
  cout<<" ++++ DOING PT FRAC CORRECTION ++++" <<endl;
  if(doptcorr && ( lastpartmc.Contains("cut1") || lastpartmc.Contains("cut5") ) && ( dirmc.Contains("E_0.9") || dirmc.Contains("E_7") ) ){  
    cout<<" ++++ DOING PT FRAC CORRECTION ++++" <<endl;
    
    
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
        
    //if(syst==700)
    //  ptcorr_val += 1. * syst_sign;
    
    increaseNTracks(nch_corrected , +1, ptcorr_val , 1);  //+1 is the sign
  }
  else cout <<" no pt frac correction done" << endl;
  
  cout<<"Mean of multiplicity --------> "<<nch_corrected->GetMean()<<endl;
  cout<<"RMS of multiplicity  --------> "<<nch_corrected->GetRMS()<<endl;

  //------------------------------------------------------------------------------
  //--------------------------   Adding Stat Errors   ----------------------------
  //--------------------------   to the final curve   ----------------------------
  //------------------------------------------------------------------------------
  
  for(int nbin = 1 ; nbin<=nch_corrected->GetNbinsX() ; ++nbin){
    nch_corrected->SetBinError(nbin , sqrt(pow(nch_resampledPtr->GetBinError(nbin),2)+pow(nch_mtxresampledPtr->GetBinError(nbin),2)));
    //cout<<nbin<<"  "<<nch_resampledPtr->GetBinError(nbin)<<"  "<<nch_mtxresampledPtr->GetBinError(nbin)<<endl;
    //cout<<"  "<<sqrt(pow(nch_resampledPtr->GetBinError(nbin),2)+pow(nch_mtxresampledPtr->GetBinError(nbin),2))
    //    <<"  "<<nch_unfoldedPtr->GetBinError(nbin)<<endl;
  }
  
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
  cout<<" ++++ RESCALING ++++" <<endl;
  
  divideByWidth(nch_trueGen);
  divideByWidth(nch_trueGen_afterUnfolding);
  divideByWidth(nch_trueGenAfterEvtSel);
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
  cout<<" ++++ MOMENTS ++++" <<endl;  
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
  nch_trueGenAfterEvtSel->Write();
  nch_trueGen_afterUnfolding->Write();
  nch_REC->Write("nch_data_raw");
  if(lastpartmc.Contains("HF1")) nch_SD->Write("nch_SD_mc");
  nch_unfoldedPtr->Write();
  nch_corrected->Write();
  TH1F* nch_cor_norm = (TH1F*) nch_corrected->Clone("nch_corrected_norm");
  nch_cor_norm->Scale(10000*1./nch_cor_norm->Integral());    //NEED TO ADJUST with bin !=1
  nch_cor_norm->Write();
  eff_evtSel->Write();
  eff_centrSel->Write();
  eff->Write();
  nch_evtSelCorr->Write();
  nch_resampledPtr->Write();
  nch_mtxresampledPtr->Write();
  nch_MC_RECO_RAW->Write("nch_mc_reco_raw");
  matrixhist->Write();
//  nch_clone1->Write();
//  nch_clone2->Write();
  
  gDirectory->cd("../");
  mc->Close(); 
  data->Close();
  out->Close();
  
  delete hypothesis;
  delete nch_cor_norm;
  delete nch_corrected;
  delete nch_trueGen;
  if(lastpartmc.Contains("HF1")) delete nch_SD;
  delete nch_unfoldedPtr;
  delete nch_REC;
  delete nch_toUnfold;
  delete eff_evtSel;
  delete eff_centrSel;
  delete eff;
  delete nch_evtSelCorr;
  delete nch_MC_RECO_RAW;
  delete matrixhist;
  delete moment;
  delete out;
  
  

  
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
