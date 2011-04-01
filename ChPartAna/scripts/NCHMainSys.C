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

#include "../plugins/TMoments.h"
#include "../plugins/Syst.h"
#include "../scripts/syst_funcs.C"

void TestCode();
Syst* MPSys( TString, TString, TString, TString, TString, TH1F* );
Syst* UNFSys(TString, TString, TString, TString, TH1F* );

//    rootc NCHMainSys.C+ -q

//________________________________________________________________________________
void NCHMainSys(TString filepart = "_partfull_HF0_ATLAS6_RECO_cut0") {

  if(filepart.Contains("HF1")) filepart.ReplaceAll("RECO","NSD");

  //---- Test Code ------
  //TestCode();

  // --------------- General  SYS code ---------------------
  //  first get files to do SYST on, then call the SYS function.
  //  all SYS of the same file: ex: (MC31) HF0_ATLAS6_INEL_cut0 goes
  //  into the same output file
  

  // -----INPUTS------------------------------------
  const TString BaseMC = "MC31";
  const TString dirpart="v20/";
  //const TString filepart="_partfull_HF0_ATLAS1_INEL_cut0";
 
  //------------------------------------------------
  //detaching cloned TH1's from their original file, so outfile->Close() will not crash the program
  TH1::AddDirectory(kFALSE);
  
  Syst*    Unf_Syst = NULL;   
  Syst*  NIter_Syst = NULL;   
  Syst*  Track_Syst = NULL;
  Syst* EvtSel_Syst = NULL; 
  Syst* Summed_Syst = NULL;

  const TString filedirunfold = "../macro/unfold_outputs/"+dirpart;
  const TString filediroutput = "../macro/sys_outputs/"+dirpart;
  const TString outstr        = "sys_"+BaseMC+filepart+".root";

  TFile* fileBase  =NULL; fileBase   = TFile::Open(filedirunfold+"unf_"+BaseMC+filepart+".root" ,"READ");  
  TFile* output=NULL; output = TFile::Open(filediroutput+outstr,"RECREATE");
  cout <<  "BaseMCFile : "  << filedirunfold+"unf_"+BaseMC+filepart+".root"  << " ptr: " <<  fileBase   << endl;
  cout <<  "output: " << filediroutput+outstr   << " ptr: " <<  output << endl;
  if (fileBase==0 || output ==0 ) return;

  const TString histoName="nch_data_corrected";
  TH1F* CurveBase = (TH1F*) fileBase->FindObjectAny(histoName);


  // ---------------  Unfolding SYS  -----------------------
  cout <<"---- Starting Unfolding SYS ----"<<endl;
  // Second MC file to compare it with the BaseMC
  TString TestMC = "MC60";
  Unf_Syst = UNFSys(TestMC, filepart, filedirunfold, histoName, CurveBase );
  cout <<"Created Unf_syst : " << TestMC << "  ptr: " << Unf_Syst << endl;
  if(Unf_Syst==0) return;
  

  // ---------------  NIter SYS  -----------------------
  cout <<"---- Starting NIter SYS ----"<<endl;
  //changing the number of iterations
  //CurveBase, filepart+NIterSys+filedirunfold+histoName
  TString NIterSys= "302";
  NIter_Syst = MPSys(NIterSys, BaseMC, filepart, filedirunfold, histoName, CurveBase );
  cout <<"Created NIter_syst : " << NIterSys << "  ptr: " << NIter_Syst << endl;
  if(NIter_Syst==0) return;


  // ---------------  Track SYS  -----------------------
  cout <<"---- Starting Track SYS ----"<<endl;
  //changing the number of tracks by 2.53 %
  TString TrackSys= "100";  
  Track_Syst = MPSys(TrackSys, BaseMC, filepart, filedirunfold, histoName, CurveBase );
  cout <<"Created Track_syst : " << TrackSys << "  ptr: " << Track_Syst << endl;
  if(Track_Syst==0) return;

  
  // -------------   EvtSel SYS    ---------------------
  
  

  // ------------   Making Sum SYS   -------------------
  cout <<"-- Starting Summing the SYS --"<<endl;
  vector<Syst>* Vec_Syst = new vector <Syst>;
  Vec_Syst->push_back(    *Unf_Syst); 
  Vec_Syst->push_back(  *NIter_Syst);
  Vec_Syst->push_back(  *Track_Syst);
//Vec_Syst->push_back( *EvtSel_Syst);

  vector<TString> vLeg;
  vLeg.push_back("corr. with PYTHIA 8");
  vLeg.push_back("niter #pm 2 ");
  vLeg.push_back("Track eff.");
  
  Summed_Syst = new Syst( "Summed" );
  Summed_Syst->setMain(Vec_Syst->at(0).mainTH1);
  //Summed_Syst->add(*Unf_Syst);
  
  int i = 0;
  for(vector<Syst>::iterator it=Vec_Syst->begin();it!=Vec_Syst->end();++it , ++i){ 
     draw_syst(*it, vLeg[i] , 1 , BaseMC+filepart);
    Summed_Syst->add(*it);  
  }
  
  Summed_Syst->addStatToSyst();
  
  output->cd();
  Summed_Syst->write();
  //delete Vec_Syst;
    
  draw_vsyst(*Vec_Syst, vLeg , 1 , BaseMC+filepart);

  //-----------   Closing Everything  -------------------
  delete CurveBase;
  fileBase->Close(); delete fileBase;
  output->Close();   delete output;  
  if (Unf_Syst!=0)   delete Unf_Syst;
  if (NIter_Syst!=0) delete NIter_Syst;
  if (Track_Syst!=0) delete Track_Syst;
  if(EvtSel_Syst!=0) delete EvtSel_Syst;
  if(Summed_Syst!=0) delete Summed_Syst;
  
  cout <<  "outputfile: " << filediroutput+outstr <<endl;
  cout << "----   END OF NCHMainSys   ----" <<endl;

}


//________________________________________________________________________________
Syst* UNFSys(TString TestMC, TString filepart, TString filedirunf, TString histoName, TH1F* CurveBase ) {

  //save output directory
  TDirectory* current=gDirectory;
  
  TFile* fileTest =NULL; fileTest  = TFile::Open(filedirunf+"unf_"+TestMC+filepart+".root","READ");
  cout <<  "FileTest: "  << filedirunf+"unf_"+TestMC+filepart+".root" << " ptr: " <<  fileTest  << endl;
  if (fileTest==0)  cout << "ERROR: NO fileTest" <<endl;

  TH1F* CurveTest = (TH1F*) fileTest->FindObjectAny(histoName);
  cout <<  "CurveBase : "  << CurveBase  << " CurveTest: " <<  CurveTest   << endl;

  Syst* Sys_unftest = new Syst(TestMC, CurveBase, CurveTest, 0);
  
  //recall output directory
  current->cd();
  Sys_unftest->write();
  fileTest->Close(); delete fileTest;
  delete CurveTest;
  
  return Sys_unftest;
}  


//________________________________________________________________________________
Syst* MPSys(TString Sys, TString BaseMC, TString filepart, TString filedirunf, TString histoName, TH1F* CurveBase ) {

  //save output directory
  TDirectory* current=gDirectory;

  TFile* fileM =NULL; fileM  = TFile::Open(filedirunf+"unf_"+BaseMC+filepart+"_SYS"+Sys+"M.root","READ");
  TFile* fileP =NULL; fileP  = TFile::Open(filedirunf+"unf_"+BaseMC+filepart+"_SYS"+Sys+"P.root","READ");
  cout <<  "FileM: "  << filedirunf+"unf_"+BaseMC+filepart+"_SYS"+Sys+"M.root" << " ptr: " <<  fileM  << endl;
  cout <<  "FileP: "  << filedirunf+"unf_"+BaseMC+filepart+"_SYS"+Sys+"P.root" << " ptr: " <<  fileP  << endl;
  //if (fileM==0 || fileP==0) return 0;
  if (fileM==0 || fileP==0) cout << "ERROR: NO fileM or NO fileP" <<endl;
  
  TH1F* CurveM = (TH1F*) fileM->FindObjectAny(histoName);
  TH1F* CurveP = (TH1F*) fileP->FindObjectAny(histoName);
  cout <<  "CurveM : "  << CurveM  << " CurveP: " <<  CurveP   << endl;

  Syst* Sys_test = new Syst(Sys, CurveBase, CurveM, CurveP);

  //recall output directory
  current->cd();
  Sys_test->write();
  fileM->Close();  delete fileM; 
  fileP->Close();  delete fileP;
  delete CurveM;   delete CurveP;

  return Sys_test;
}


//________________________________________________________________________________
void NCHAll() {

    NCHMainSys("_partfull_HF0_ATLAS1_RECO_cut0");
    NCHMainSys("_partfull_HF1_ATLAS1_RECO_cut0");
    NCHMainSys("_partfull_HF0_ATLAS2_RECO_cut0");
    NCHMainSys("_partfull_HF1_ATLAS2_RECO_cut0");
    NCHMainSys("_partfull_HF0_ATLAS6_RECO_cut0");
    NCHMainSys("_partfull_HF1_ATLAS6_RECO_cut0");
    NCHMainSys("_partfull_HF0_ALICE_RECO_cut0");
    NCHMainSys("_partfull_HF1_ALICE_RECO_cut0");
    NCHMainSys("_partfull_HF0_MBUEWG_RECO_cut0");
    NCHMainSys("_partfull_HF1_MBUEWG_RECO_cut0");   
    NCHMainSys("_partfull_HF0_nocut_RECO_cut0");
    NCHMainSys("_partfull_HF1_nocut_RECO_cut0");

    NCHMainSys("_partfull_HF0_ATLAS1_RECO_cut1");
    NCHMainSys("_partfull_HF1_ATLAS1_RECO_cut1");
    NCHMainSys("_partfull_HF0_ATLAS2_RECO_cut1");
    NCHMainSys("_partfull_HF1_ATLAS2_RECO_cut1");
    NCHMainSys("_partfull_HF0_ATLAS6_RECO_cut1");
    NCHMainSys("_partfull_HF1_ATLAS6_RECO_cut1");
    NCHMainSys("_partfull_HF0_ALICE_RECO_cut1");
    NCHMainSys("_partfull_HF1_ALICE_RECO_cut1");
    NCHMainSys("_partfull_HF0_MBUEWG_RECO_cut1");
    NCHMainSys("_partfull_HF1_MBUEWG_RECO_cut1");   
    NCHMainSys("_partfull_HF0_nocut_RECO_cut1");
    NCHMainSys("_partfull_HF1_nocut_RECO_cut1");

    NCHMainSys("_partfull_HF0_ATLAS1_RECO_cut2");
    NCHMainSys("_partfull_HF1_ATLAS1_RECO_cut2");
    NCHMainSys("_partfull_HF0_ATLAS2_RECO_cut2");
    NCHMainSys("_partfull_HF1_ATLAS2_RECO_cut2");
    NCHMainSys("_partfull_HF0_ATLAS6_RECO_cut2");
    NCHMainSys("_partfull_HF1_ATLAS6_RECO_cut2");
    NCHMainSys("_partfull_HF0_ALICE_RECO_cut2");
    NCHMainSys("_partfull_HF1_ALICE_RECO_cut2");
    NCHMainSys("_partfull_HF0_MBUEWG_RECO_cut2");
    NCHMainSys("_partfull_HF1_MBUEWG_RECO_cut2");   
    NCHMainSys("_partfull_HF0_nocut_RECO_cut2");
    NCHMainSys("_partfull_HF1_nocut_RECO_cut2");

    NCHMainSys("_partfull_HF0_ATLAS1_RECO_cut3");
    NCHMainSys("_partfull_HF1_ATLAS1_RECO_cut3");
    NCHMainSys("_partfull_HF0_ATLAS2_RECO_cut3");
    NCHMainSys("_partfull_HF1_ATLAS2_RECO_cut3");
    NCHMainSys("_partfull_HF0_ATLAS6_RECO_cut3");
    NCHMainSys("_partfull_HF1_ATLAS6_RECO_cut3");
    NCHMainSys("_partfull_HF0_ALICE_RECO_cut3");
    NCHMainSys("_partfull_HF1_ALICE_RECO_cut3");
    NCHMainSys("_partfull_HF0_MBUEWG_RECO_cut3");
    NCHMainSys("_partfull_HF1_MBUEWG_RECO_cut3");   
    NCHMainSys("_partfull_HF0_nocut_RECO_cut3");
    NCHMainSys("_partfull_HF1_nocut_RECO_cut3");

    NCHMainSys("_partfull_HF0_ATLAS1_RECO_cut4");
    NCHMainSys("_partfull_HF1_ATLAS1_RECO_cut4");
    NCHMainSys("_partfull_HF0_ATLAS2_RECO_cut4");
    NCHMainSys("_partfull_HF1_ATLAS2_RECO_cut4");
    NCHMainSys("_partfull_HF0_ATLAS6_RECO_cut4");
    NCHMainSys("_partfull_HF1_ATLAS6_RECO_cut4");
    NCHMainSys("_partfull_HF0_ALICE_RECO_cut4");
    NCHMainSys("_partfull_HF1_ALICE_RECO_cut4");
    NCHMainSys("_partfull_HF0_MBUEWG_RECO_cut4");
    NCHMainSys("_partfull_HF1_MBUEWG_RECO_cut4");   
    NCHMainSys("_partfull_HF0_nocut_RECO_cut4");
    NCHMainSys("_partfull_HF1_nocut_RECO_cut4");
}



////////////////////////////////////////////////////////////////////////////////////////
// TESTCODE
//________________________________________________________________________________
void TestCode(){

    TString coll = "coll1";

    TH1F* flat  = new TH1F("flat_" +coll,"flat_" +coll+";flat;# events",11,-0.5,10.5);
    TH1F* flatp = new TH1F("flatp_"+coll,"flatp_"+coll+";flat;# events",11,-0.5,10.5);
    TH1F* flatm = new TH1F("flatm_"+coll,"flatm_"+coll+";flat;# events",11,-0.5,10.5);

    for(int i = 1 ; i <= flat->GetNbinsX() ; ++i){
       flat ->SetBinContent(i,1);
        flatp->SetBinContent(i,3);
        flatm->SetBinContent(i,0.5);
    }
    Syst* test = NULL;
    test = new Syst("testing",flat,flatp,flatm);
    //test->getSyst(false);
    //test->reconstruct();
    test->add(test->gsyst);

    //output
    TFile* output = new TFile("./outputtest.root","RECREATE");  
    output->cd();
    test->write();
    cout << "Writing to the file: " << endl << "outputtest.root" << endl;
    output->Close();
    delete output;
  
    delete flat;
    delete flatp;
    delete flatm;
    delete test;

    cout << "END OF TestCode()" <<endl;
    cout << "-----------------" <<endl;
}
//TESTCODE
////////////////////////////////////////////////////////////////////////////////////
