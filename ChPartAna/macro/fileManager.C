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
//#             10 : MC - PYTHIA D6T 
//#             11 : MC - PYTHIA DW
//#             12 : MC - PYTHIA P0 
//#             13 : MC - PYTHIA ProQ20
//#             20 : MC - PHOJET
//#             30 : MC - PYTHIA D6T <--- V8P file
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

  TString CMSSW       = "CMSSW_3_3_6_patch3";
  TString BaseDirTree = "/user/xjanssen/data/" ;
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
        TreeBase = "ChPartTree_v005b_d900"; 
        DataSet  = "__MinimumBias__BeamCommissioning09-Dec19thReReco_336p3_v2__RECO";
      }
      else if ( iDataType == 10 ) {
        TreeBase = "ChPartTree_v005c_mc900";
        DataSet  = "__MinBias__Summer09-STARTUP3X_V8K_900GeV-v1__GEN-SIM-RECO";
      }
      else if ( iDataType == 11 ) {
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

      else if ( iDataType == 30 ) {
        TreeBase = "ChPartTree_v005c_mc900_V8P";
        DataSet  = "__MinBias__Summer09-STARTUP3X_V8P_900GeV-v1__GEN-SIM-RECO";
      }


      else {
        cout << "[fileManager] Unknown DataType: " << iDataType << endl;
        return "NONE";
      }
    }


    // --- 2.36 TeV ---
    else if ( Energy == 2.36 ){

      if      ( iDataType == 0 ) {
        TreeBase = "ChPartTree_v005b_d236";
        DataSet  = "__MinimumBias__BeamCommissioning09-Dec19thReReco_336p3_v2__RECO";
      }
      else if ( iDataType == 10 ) {
        TreeBase = "ChPartTree_v005c_mc236";
        DataSet  = "__MinBias__Summer09-STARTUP3X_V8L_2360GeV-v1__GEN-SIM-RECO";
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

  //---------------------- collectionPlotter Files --------------------------
  } else if ( iFileType == 1 || iFileType == 2 || iFileType == 3 ) {

    TString PlotBase;
    TString DataSet  = "_Undef" ;
    TString SEnergy  = "_Undef" ;
    TString STracking= "_Undef" ; 

    if ( iFileType == 1 ) PlotBase = "collectionPlotter" ;
    if ( iFileType == 2 ) PlotBase = "simpleAna";
    if ( iFileType == 3 ) PlotBase = "unfolding";  

    if ( iDataType == 0 ) DataSet = "_data" ; 
    if ( iDataType == 10) DataSet = "_MC_D6T" ; 
    if ( iDataType == 11) DataSet = "_MC_DW" ; 
    if ( iDataType == 12) DataSet = "_MC_P0" ; 
    if ( iDataType == 13) DataSet = "_MC_ProQ20" ; 
    if ( iDataType == 20) DataSet = "_MC_PHOJET" ;
    if ( iDataType == 30) DataSet = "_MC_D6T_newBS" ; 
    
    if ( Energy == 0.9  ) SEnergy = "_0.9TeV"  ;
    if ( Energy == 2.36 ) SEnergy = "_2.36TeV" ;
    
    if ( iTracking == 0 ) STracking= "_gTr"  ;
    if ( iTracking == 1 ) STracking= "_mbTr" ;

    FileName  = BaseDirPlot +  PlotBase + DataSet + SEnergy ;
    if (iFileType == 2 ) FileName += STracking ;
    if (iFileType == 3 ) FileName += STracking ;
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
