//#################################################
//#
//# fileManager: Returns FileName 
//# -----------
//#
//# iFileType = 0 : ChPartTree 
//#             1 : collectionPlotter
//#             2 : simpleAna 
//#             3 : unfolding
//#
//# iDataType = 0  : Data
//#             5  : Data ZeroBias
//#             10 : MC - PYTHIA D6T 
//#             11 : MC - PYTHIA DW
//#             12 : MC - PYTHIA P0 
//#             13 : MC - PYTHIA ProQ20
//#             15 : MC - PYTHIA Z2
//#             20 : MC - PHOJET
//#             30 : MC - PYTHIA D6T <--- V8P file
//#             51 : MC - PYTHIA X1
//#             52 : MC - PYTHIA X2
//#             60 : MC - PYTHIA 8
//#             61 : MC - PYTHIA 8 2C  --> GenParts Only
//#             62 : MC - PYTHIA 8 4C  --> GenParts Only
//#
//#             100 : DATA TEST
//#             101 : MC   TEST
//#
//# Energy    = 0.9  : 900 GeV  Data/MC
//#             2.36 : 2.36 TeV Data/MC
//# 
//# FOLLOWING OPTIONS ONLY FOR PLOTS:
//#
//# iTracking = 0  : general Tracks + offline Vtx
//#             1  : minbias Tracks + Ferenc Vtx
//#
//# iSystType = 0  : None
//#           > 0  : TBA
//#
//# iSystSign = +1/-1
//#
//#################################################
#include "TChain.h"
#include "TChainElement.h"
#include <iostream>
#include <fstream>

TString fileManager ( int     iFileType  = 0
                    , int     iDataType  = 0
                    , double  Energy     = 0.9
                    , int     iTracking  = 0
                    , int     iSystType  = 0
                    , int     iSystSign  = 0 
                    , TString STest      = "NONE"
                    , TString BaseDirPlIn= "NONE"
                    ) 
{

  TString FileName ;

  TString CMSSW;
  if        ( Energy == 0.9 || Energy == 2.36 )
    CMSSW       = "CMSSW_3_3_6_patch3";
  else if   ( Energy == 7.0 )
    CMSSW       = "CMSSW_3_5_6";
  if ( Energy == 2.36 && iDataType == 1  ) CMSSW       = "CMSSW_3_5_6";
  if ( Energy == 2.36 && iDataType == 40 ) CMSSW       = "CMSSW_3_5_6";

  TString BaseDirTree = "dcap:///pnfs/iihe/cms/store/user/xjanssen/data/" ;
  if(Energy == 7.0 && (iDataType == 51 || iDataType == 52))
    BaseDirTree = "/user/rougny/data/" ;
  
  //******NEW
  if( (Energy == 0.9 || Energy == 7.0) && (iDataType < 10 || iDataType == 10 || iDataType == 15 || iDataType == 60)  ){
    BaseDirTree = "dcap:///pnfs/iihe/cms/store/user/rougny/data/" ;
    CMSSW       = "CMSSW_3_6_2";
  }
  
  if( (Energy == 0.9 || Energy == 7.0) && (iDataType == 61 || iDataType == 62) ){
    BaseDirTree = "dcap:///pnfs/iihe/cms/store/user/rougny/data/" ;
    CMSSW       = "CMSSW_3_11_2";
  }
  
  if(iDataType == 100 || iDataType == 101 || iDataType == 5){
    BaseDirTree = "/user/rougny/data/";
    CMSSW       = "CMSSW_3_6_2";
    if(iDataType == 100 ) iDataType = 0 ;
    if(iDataType == 101 ) iDataType = 60;
  }
  //******END NEW
  
  TString BaseDirPlot = "../plots/";
  
  if ( BaseDirPlIn != "NONE" )
    BaseDirPlot = BaseDirPlIn ;

  BaseDirTree += CMSSW + "/" ;

  //---------------------- ChPartTree Files ---------------------------------  
  if        ( iFileType == 0 ) {

    TString TreeBase ;
    TString DataSet  ;
 
    // --- 900 GeV --- 
    if        ( Energy == 0.9 ){

      if      ( iDataType == 0 ) {
        //TreeBase = "ChPartTree_v005b_d900"; 
        //DataSet  = "__MinimumBias__BeamCommissioning09-Dec19thReReco_336p3_v2__RECO";
	
        TreeBase = "ChPartTree_36x_d900v3"; 
        DataSet  = "__MinimumBias__Commissioning10-Jun14thReReco_v1__RECO";
      }
      /*else if ( iDataType == 5  ) {
        TreeBase = "ChPartTree_v005c_d900";
        DataSet  = "__ZeroBias__BeamCommissioning09-Dec19thReReco_336p3_v2__RECO";
      }*/
      else if ( iDataType == 10 ) {
        //TreeBase = "ChPartTree_v005c_mc900";
        //DataSet  = "__MinBias__Summer09-STARTUP3X_V8K_900GeV-v1__GEN-SIM-RECO";
	
        TreeBase = "ChPartTree_36x_mcv2";
        DataSet  = "__MinBias_TuneD6T_900GeV-pythia6__Summer10-START36_V10A-v1__GEN-SIM-RECODEBUG";
      }
      else if ( iDataType == 15 ) {
        TreeBase = "ChPartTree_36x_mc900";
        DataSet  = "__MinBias_TuneZ2_900GeV-pythia6__Summer10-START36_V10A-v1__GEN-SIM-RECO";
      } 
      /*else if ( iDataType == 11 ) {
        TreeBase = "ChPartTree_v005c_mc900";
        DataSet  = "__MinBias__Summer09-STARTUP3X_V8K_900GeV_DW-v1__GEN-SIM-RECO";
      } 
      else if ( iDataType == 12 ) {
        TreeBase = "ChPartTree_v005c_mc900";
        DataSet  = "__MinBias__Summer09-STARTUP3X_V8K_900GeV_P0-v1__GEN-SIM-RECO";
      }
      else if ( iDataType == 13 ) {
        TreeBase = "ChPartTree_v005c_mc900";
        DataSet  = "__MinBias__Summer09-STARTUP3X_V8K_900GeV_ProQ20-v1__GEN-SIM-RECO";
      }
      else if ( iDataType == 20 ) {
        TreeBase = "ChPartTree_v005c_phojet_mc900";
        DataSet  = "__PhojetMB900GeV__yilmaz-PhojetMB900GeV-39e473178bd7b30aa2784bced713eb86__USER";
      }

      else if ( iDataType == 40 ) {
        TreeBase = "ChPartTree_v005c_mc900_V8P";
        DataSet  = "__MinBias__Summer09-STARTUP3X_V8P_900GeV-v1__GEN-SIM-RECO";
      }*/
      else if ( iDataType == 60 ) {
        TreeBase = "ChPartTree_36x_mcv2";
        DataSet  = "__MinBias_900GeV-pythia8__Summer10-START36_V10A-v1__GEN-SIM-RECO";
      }
      else if ( iDataType == 61 ) {
        TreeBase = "ChPartTree_311x_mc_genOnly";
        DataSet  = "__MinBias_Pythia8_Tune2c_900GeV__lucaroni-MinBias_Pythia8_Tune2c_900GeV-67756f77883499276cd347c56b952136__USER";
      }
      else if ( iDataType == 62 ) {
        TreeBase = "ChPartTree_311x_mc_genOnly";
        DataSet  = "__MinBias_Pythia8_Tune4c_900GeV__lucaroni-MinBias_Pythia8_Tune4c_900GeV-4c407344ef47bd807bc0c171afd57a01__USER";
      }


      else {
        cout << "[fileManager] Unknown DataType: " << iDataType << endl;
        return "NONE";
      }
    }


    // --- 2.36 TeV ---
    else if ( Energy == 2.36 ){

    /*  if      ( iDataType == 0 ) {
        //TreeBase = "ChPartTree_v005b_d236";
        //DataSet  = "__MinimumBias__BeamCommissioning09-Dec19thReReco_336p3_v2__RECO";
        TreeBase = "ChPartTree_v005_d236_loosevtxqual";
        DataSet  = "__MinimumBias__BeamCommissioning09-Dec19thReReco_336p3_v2__RECO";
      }
      else if ( iDataType == 1  ) {
        TreeBase = "ChPartTree_v005_d236_35X";
        DataSet  = "__MinimumBias__BeamCommissioning09-Mar24thReReco_PreProduction_v2__RECO";
      }
      else if ( iDataType == 5  ) {
        TreeBase = "ChPartTree_v005c_d236";
        DataSet  = "__ZeroBias__BeamCommissioning09-Dec19thReReco_336p3_v2__RECO";
      }
      else if ( iDataType == 10 ) {
        TreeBase = "ChPartTree_v005c_mc236";
        DataSet  = "__MinBias__Summer09-STARTUP3X_V8L_2360GeV-v1__GEN-SIM-RECO";
      }
      else if ( iDataType == 20 ) {
        TreeBase = "ChPartTree_v005c_phojet_mc236";
        DataSet  = "__Phojet2360GeV_test01__yilmaz-Phojet2360GeV_test01-a1981d928dd3baaabee57744658c3360__USER";
      }
      else if ( iDataType == 40 ) {
        TreeBase = "ChPartTree_v005_mc236_V22B";
        DataSet  = "__MinBias__Summer09-START3X_V22B_2360GeV-v1__GEN-SIM-RECO";
      }
      else */{
        cout << "[fileManager] Unknown DataType: " << iDataType << endl;
        return "NONE";
      }

    }

    // --- 7.00 TeV ---

    else if ( Energy == 7.00 ){

      if      ( iDataType == 0 ) {
        //TreeBase = "ChPartTree_v005_d7000";
        //DataSet  = "__MinimumBias__Commissioning10-Apr1ReReco-v2__RECO";
        
	TreeBase = "ChPartTree_36x_d7000v3";
        DataSet  = "__MinimumBias__Commissioning10-Jun14thReReco_v1__RECO";
      }
      else if ( iDataType == 5  ) {
        //TreeBase = "ChPartTree_v005b_d7000";
        //DataSet  = "__ZeroBias__Commissioning10-Apr1ReReco-v2__RECO";
	//cout << "[fileManager] WARNING !! Those files are old 356 files ... To use with caution :)" << endl;
	
	TreeBase = "ChPartTree_36x_d7000v3";
        DataSet  = "__ZeroBias__Commissioning10-Jun14thReReco_v1__RECO/HADD";
      }
      else if ( iDataType == 10 ) {
        //TreeBase = "ChPartTree_v005_mc7000";
        //DataSet  = "__MinBias__Spring10-START3X_V26A_356ReReco-v1__GEN-SIM-RECO";
	
        TreeBase = "ChPartTree_36x_mcv2";
        DataSet  = "__MinBias_TuneD6T_7TeV-pythia6__Summer10-START36_V10_SP10-v1__GEN-SIM-RECODEBUG";
      }
      else if ( iDataType == 15 ) {
        TreeBase = "ChPartTree_36x_mc7000";
        DataSet  = "__MinBias_TuneZ2_7TeV_pythia6__Summer10-START36_V10-v1__GEN-SIM-RECO";
      }
      /*else if ( iDataType == 20 ) {
        TreeBase = "ChPartTree_v005c_mc7000_v25b";
        DataSet  = "__MinBiasPhojet_SIM_0413__yilmaz-MinBiasPhojet_RECO_0413_v1-309b694e9ccf1df48e24b126fab6958b__USER";
      }
      else if ( iDataType == 30 ) {
        TreeBase = "ChPartTree_v005c_mc7000_v25b";
        DataSet  = "__MinBiasD6T_SIM_0334__yilmaz-MinBiasD6T_RECO_0334_v1-309b694e9ccf1df48e24b126fab6958b__USER";
      }*/
      else if ( iDataType == 31 ) {
        TreeBase = "ChPartTree_v005c_mc7000_v25b";
        DataSet  = "__MinBiasATLAS_SIM_0332__yilmaz-MinBiasATLAS_RECO_0332_v1-a68e153adb2dbe2ae110cdf8cea4b2da__USER";
      }/*
      else if ( iDataType == 51 ) {
        TreeBase = "ChPartTree_v005_mc7000_v26_FS";
        DataSet  = "__MinBias_TuneX1_7TeV-pythia6__Spring10-START3X_V26_FastSim-v1__GEN-SIM-DIGI-RECO";
      }
      else if ( iDataType == 52 ) {
        TreeBase = "ChPartTree_v005_mc7000_v26_FS";
        DataSet  = "__MinBias_TuneX2_7TeV-pythia6__Spring10-START3X_V26_FastSim-v1__GEN-SIM-DIGI-RECO";
      }*/
      else if ( iDataType == 60 ) {
        TreeBase = "ChPartTree_36x_mcv4";
        DataSet  = "__MinBias_7TeV-pythia8__Summer10-START36_V10_SP10-v1__GEN-SIM-RECODEBUG";
      }
      else if ( iDataType == 61 ) {
        TreeBase = "ChPartTree_311x_mc_genOnly";
        DataSet  = "__MinBias_Pythia8_Tune2c__lucaroni-MinBias_Pythia8_Tune2c-bf353181d671fe026eb74236c5090cda__USER";
      }
      else if ( iDataType == 62 ) {
        TreeBase = "ChPartTree_311x_mc_genOnly";
        DataSet  = "__MinBias_Pythia8_Tune4c_7TeV__lucaroni-MinBias_Pythia8_Tune4c_7TeV-1900885b4ddb1084e6f514b058c3f4b1__USER";
      }

      else {
        cout << "[fileManager] Unknown DataType: " << iDataType << endl;
        return "NONE";
      }




    }




    // ---- ??? TeV ---
    else {
       cout << "[fileManager] Unknown energy: " << Energy << endl;
       return "NONE";
    } 


    FileName =   BaseDirTree + TreeBase + "/" + DataSet + "/" 
               + TreeBase + "__" + CMSSW + DataSet + "_*.root/evt" ;
    if(DataSet.Contains("HADD"))
      FileName =   BaseDirTree + TreeBase + "/" + DataSet + "/" + "concatenatedfile_*.root/evt";

  //---------------------- collectionPlotter Files --------------------------
  } else if ( iFileType == 1 || iFileType == 2 || iFileType == 3 || iFileType == 4 || iFileType == 5) {

    TString PlotBase;
    TString DataSet  = "_Undef" ;
    TString SEnergy  = "_Undef" ;
    TString STracking= "_Undef" ; 

    if ( iFileType == 1 ) PlotBase = "collectionPlotter" ;
    if ( iFileType == 2 ) PlotBase = "simpleAna";
    if ( iFileType == 3 ) PlotBase = "unfolding";  
    if ( iFileType == 4 ) PlotBase = "ZeroBiasEff";  
    if ( iFileType == 5 ) PlotBase = "mptCorr"; 

    if ( iDataType == 0 ) DataSet = "_data" ; 
    if ( iDataType == 1 ) DataSet = "_data_35X" ; 
    if ( iDataType == 5 ) DataSet = "_data_ZeroBias" ; 
    if ( iDataType == 10) DataSet = "_MC_D6T" ; 
    if ( iDataType == 11) DataSet = "_MC_DW" ; 
    if ( iDataType == 12) DataSet = "_MC_P0" ; 
    if ( iDataType == 13) DataSet = "_MC_ProQ20" ; 
    if ( iDataType == 15) DataSet = "_MC_Z2" ;
    if ( iDataType == 20) DataSet = "_MC_PHOJET" ;
    if ( iDataType == 30) DataSet = "_MC_D6T_newBS" ; 
    if ( iDataType == 31) DataSet = "_MC_ATLAS" ; 
    if ( iDataType == 40) DataSet = "_MC_D6T_35X" ; 
    if ( iDataType == 51) DataSet = "_MC_D6T_X1" ; 
    if ( iDataType == 52) DataSet = "_MC_D6T_X2" ;
    if ( iDataType == 60) DataSet = "_MC_Pythia8" ;
    if ( iDataType == 61) DataSet = "_MC_Pythia8_2C" ;
    if ( iDataType == 62) DataSet = "_MC_Pythia8_4C" ;
    
    if ( Energy == 0.9  ) SEnergy = "_0.9TeV"  ;
    if ( Energy == 2.36 ) SEnergy = "_2.36TeV" ;
    if ( Energy == 7.0  ) SEnergy = "_7.0TeV" ;
    
    if ( iTracking == 0 ) STracking= "_gTr"  ;
    if ( iTracking == 1 ) STracking= "_mbTr" ;

    FileName  = BaseDirPlot +  PlotBase + DataSet + SEnergy ;
    if (iFileType == 2 ) FileName += STracking ;
    if (iFileType == 3 ) FileName += STracking ;
    if (iFileType == 5 ) FileName += STracking ;
    if (iSystType > 0 ) 
    {
      if ( fabs (iSystSign) == 1 ) 
      {
        std::string str;
        std::stringstream out("");
        out << iSystType;
        str = out.str();
        FileName += "_syst" + str ;
        if  (iSystSign ==  1 ) FileName += "p";
        if  (iSystSign == -1 ) FileName += "m";
      }
      else {
       cout << "[fileManager] Unknown iSystSign: " << iSystSign << endl;
       return "NONE";
      }      
    }
    if (STest != "NONE" ) FileName += "__" + STest ;
    FileName += ".root";

  }


  return FileName;

}


vector<TString>* getListOfFiles(TString strfiles){

  vector<TString>* vfiles = new vector<TString>;
  
  if(strfiles.Contains(".root")){
    TChain chain("evt","");
    chain.Add(strfiles);
    TObjArray* fileElements=chain.GetListOfFiles();
    TIter next(fileElements);
    TChainElement *chEl=0;
    while (( chEl=(TChainElement*)next() ))
      vfiles->push_back(TString(chEl->GetTitle()));
  }
  else if(strfiles.Contains(".txt")){
    ifstream txtfile;
    txtfile.open(strfiles);
    if(!txtfile) {
      cout<<"Unable to read the txt file where the rootfiles are." << endl ;
      cout << strfiles << " doesn't exist." << endl << "Aborting ...";
      exit(0);
    }
    string filename;
    while(txtfile>>filename && filename!="EOF")
      vfiles->push_back(TString(filename));
    txtfile.close();
  }
  else {
    cout << "Unknown type of input to get files. Must contain either .root or .txt extension." << endl << "Aborting ..." << endl;
    exit(0);
  }
  cout << "[getListOfFiles] Will run on " << vfiles->size() << " files" << endl;
  return vfiles;
}

