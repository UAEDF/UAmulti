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
#include "../scripts_sten2/syst_funcs.C"
#include "../macro/files_funcs.C"

void TestCode();
Syst* MPSys( TString, TString, TString, TString, TString, TH1F* );
Syst* UNFSys(TString, TString, TString, TString, TH1F* );
TH1F*  makeEffSyst(TString = "v25" , TString = "ATLAS2" , TString = "cut0");

//    rootc NCHMainSys.C+ -q

//________________________________________________________________________________
void NCHMainSys(TString filepart = "_partfull_HF0_ATLAS2_INEL_cut0", double energy = 7, TString dirpart="v25NoWeight") {

  if(filepart.Contains("HF1")) filepart.ReplaceAll("INEL","NSD");
  
  Syst::sysType = Syst::half;
  
  xmax = 120;

  //---- Test Code ------
  //TestCode();

  // --------------- General  SYS code ---------------------
  //  first get files to do SYST on, then call the SYS function.
  //  all SYS of the same file: ex: (MC31) HF0_ATLAS6_INEL_cut0 goes
  //  into the same output file
  

  // -----INPUTS------------------------------------
  TString BaseMC = "MC31";
  if (energy == 0.9) BaseMC = "MC60";
  //------------------------------------------------
  //detaching cloned TH1's from their original file, so outfile->Close() will not crash the program
  TH1::AddDirectory(kFALSE);
  
  TString cut = dynamic_cast<TObjString*>(filepart.Tokenize("_")->At(4))->GetString();
  TString ctr = dynamic_cast<TObjString*>(filepart.Tokenize("_")->At(2))->GetString();
  cout << "cut : " << cut << "  ctr : " << ctr << endl;
    
    
    
  Syst*    Unf_Syst = NULL;   
  Syst*  NIter_Syst = NULL;   
  Syst*  Track_Syst = NULL;
  Syst* Summed_Syst = NULL;

  const TString filedirunfold = "../macro/unfold_outputs/"+dirpart+"/";
  const TString filediroutput = "../macro/sys_outputs/"+dirpart+"/";
  const TString outstr        = "sys_"+BaseMC+filepart+".root";

  TFile* fileBase  =NULL; fileBase   = TFile::Open(filedirunfold+"unf_"+BaseMC+filepart+".root" ,"READ");  
  TFile* output=NULL; output = TFile::Open(filediroutput+outstr,"RECREATE");
  cout <<  "BaseMCFile : "  << filedirunfold+"unf_"+BaseMC+filepart+".root"  << " ptr: " <<  fileBase   << endl;
  cout <<  "output: " << filediroutput+outstr   << " ptr: " <<  output << endl;
  if (fileBase==0 || output ==0 ) return;

  const TString histoName="nch_data_corrected";
  TH1F* CurveBase = (TH1F*) fileBase->FindObjectAny(histoName);
  cout <<  "CurveBase: " << histoName   << " ptr: " <<  CurveBase << endl;
  if (CurveBase ==0 ) return;

 /* // ---------------  Unfolding SYS  -----------------------
  cout << endl << "---- Starting Unfolding SYS ----"<<endl;
  // Second MC file to compare it with the BaseMC
  TString TestMC = "MC60";
  if (energy == 0.9) TestMC = "MC15"; 
  Unf_Syst = UNFSys(TestMC, filepart, filedirunfold, histoName, CurveBase );
  cout <<"Created Unf_syst : " << TestMC << "  ptr: " << Unf_Syst << endl;
  if(Unf_Syst==0) return;
  

  // ---------------  NIter SYS  -----------------------
  cout << endl << "---- Starting NIter SYS ----"<<endl;
  //changing the number of iterations
  //CurveBase, filepart+NIterSys+filedirunfold+histoName
  TString NIterSys= "302";
  NIter_Syst = MPSys(NIterSys, BaseMC, filepart, filedirunfold, histoName, CurveBase );
  cout <<"Created NIter_syst : " << NIterSys << "  ptr: " << NIter_Syst << endl;
  if(NIter_Syst==0) return;
  draw_syst(*NIter_Syst , "niter #pm 2" , 1 , BaseMC+filepart);


  // ---------------  Track SYS  -----------------------
  cout << endl << "---- Starting Track SYS ----"<<endl;
  //changing the number of tracks by 2.53 %
  TString TrackSys= "100";  
  Track_Syst = MPSys(TrackSys, BaseMC, filepart, filedirunfold, histoName, CurveBase );
  cout <<"Created Track_syst : " << TrackSys << "  ptr: " << Track_Syst << endl;
  if(Track_Syst==0) return;


   // ---------------  Compare the BaseMC nch curve with for example the NoWeight - SYS  -----------------------
  cout << endl << "---- Starting Compare NW/W SYS ----"<<endl;
  TString extra = "_noweight";
  TString Testfilepart = extra+filepart;
  TString Testfiledir = "../"+dirpart+"NoWeight/";
  Syst*    NWW_Syst = NULL; 
  NWW_Syst = UNFSys(BaseMC, Testfilepart, filedirunfold+Testfiledir, histoName, CurveBase );
  cout <<"Created NW/W_syst : " << extra << "  ptr: " << NWW_Syst << endl;
  if(NWW_Syst==0) return;
  draw_syst(*NWW_Syst , "no weight" , 1 , BaseMC+filepart);
*/
  Syst*  gf_Syst = NULL; 
  Syst* eff_Syst = NULL; 
  if(energy==7) {  
  
  /*  // ---------------  Compare the BaseMC nch curve with genTr/ferTr - SYS  -----------------------
    cout << endl  <<"---- Starting Compare fer/gen SYS ----"<<endl;

    TString extrag = "_genTr";
    TString Testfilepartg = extrag+filepart;
     TString Testfiledirg = "../"+dirpart+"_genTr/";
    
    gf_Syst = UNFSys(BaseMC, Testfilepartg, filedirunfold+Testfiledirg, histoName, CurveBase );
    cout <<"Created gen/fernc_syst : " << extrag << "  ptr: " << gf_Syst << endl;
    if(gf_Syst==0) return;
    draw_syst(*gf_Syst , "general Tr." , 1 , BaseMC+filepart);
    */
    
    // ---------------  Compare the BaseMC nch curve with ept/pt = 0.05 or 0.1 - SYS  -----------------------
    cout << endl  <<"---- Starting Compare ept SYS ----"<<endl;

    Syst*    ept_Syst = NULL; 
    ept_Syst = UNFSys(BaseMC, filepart, "../macro/unfold_outputs/v22NoWeight_Romain/", histoName, CurveBase );
    cout <<"Created ept_syst, ptr: " << ept_Syst << endl;
    if(ept_Syst==0) return;
    draw_syst(*ept_Syst , "ept/pt = 0.1" , 1 , BaseMC+filepart);
    return;
    
    
  
    // -------------   EvtSel SYS    ---------------------
    cout << endl << endl  <<"---- Starting Compare ept SYS ----"<<endl;
    eff_Syst = new Syst( BaseMC , CurveBase , makeEffSyst( dirpart , ctr , cut ) , 0);
    cout <<"Created eff_syst, ptr: " <<  eff_Syst << endl;
    if(eff_Syst==0) return;
    draw_syst(*eff_Syst , "eff from 0-bias" , 1 , BaseMC+filepart);
    
  }  
    
  

  // ------------   Making Sum SYS   -------------------
  cout <<"-- Starting Summing the SYS --"<<endl;
  vector<Syst>* Vec_Syst = new vector <Syst>;
  Vec_Syst->push_back(    *Unf_Syst); //  Vec_Syst->push_back(  *NIter_Syst);
  Vec_Syst->push_back(  *Track_Syst);
  if( energy==7 && (cut=="cut2" || cut=="cut3") )
    Vec_Syst->push_back(  *gf_Syst); //Vec_Syst->push_back( *EvtSel_Syst);

  vector<TString> vLeg;
  vLeg.push_back("corr. with PYTHIA 8");
  vLeg.push_back("Track eff.");
  if( energy==7 && (cut=="cut2" || cut=="cut3") )
    vLeg.push_back("general Tr.");
  
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
  //if(eff_Syst!=0) delete EvtSel_Syst;
  //if(ept_Syst!=0) delete EvtSel_Syst;
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
void NCHCut01(TString dir = "v25", double energy = 7) {

    NCHMainSys("_partfull_HF0_ATLAS1_INEL_cut0",energy,dir);
    NCHMainSys("_partfull_HF1_ATLAS1_NSD_cut0",energy,dir);
    NCHMainSys("_partfull_HF0_ATLAS2_INEL_cut0",energy,dir);
    NCHMainSys("_partfull_HF1_ATLAS2_NSD_cut0",energy,dir);
    NCHMainSys("_partfull_HF0_ATLAS6_INEL_cut0",energy,dir);
    NCHMainSys("_partfull_HF1_ATLAS6_NSD_cut0",energy,dir);
    NCHMainSys("_partfull_HF0_ALICE_INEL_cut0",energy,dir);
    NCHMainSys("_partfull_HF1_ALICE_NSD_cut0",energy,dir);
    NCHMainSys("_partfull_HF0_MBUEWG_INEL_cut0",energy,dir);
    NCHMainSys("_partfull_HF1_MBUEWG_NSD_cut0",energy,dir);   
    NCHMainSys("_partfull_HF0_nocut_INEL_cut0",energy,dir);
    NCHMainSys("_partfull_HF1_nocut_NSD_cut0",energy,dir);

    NCHMainSys("_partfull_HF0_ATLAS1_INEL_cut1",energy,dir);
    NCHMainSys("_partfull_HF1_ATLAS1_NSD_cut1",energy,dir);
    NCHMainSys("_partfull_HF0_ATLAS2_INEL_cut1",energy,dir);
    NCHMainSys("_partfull_HF1_ATLAS2_NSD_cut1",energy,dir);
    NCHMainSys("_partfull_HF0_ATLAS6_INEL_cut1",energy,dir);
    NCHMainSys("_partfull_HF1_ATLAS6_NSD_cut1",energy,dir);
    NCHMainSys("_partfull_HF0_ALICE_INEL_cut1",energy,dir);
    NCHMainSys("_partfull_HF1_ALICE_NSD_cut1",energy,dir);
    NCHMainSys("_partfull_HF0_MBUEWG_INEL_cut1",energy,dir);
    NCHMainSys("_partfull_HF1_MBUEWG_NSD_cut1",energy,dir);   
    NCHMainSys("_partfull_HF0_nocut_INEL_cut1",energy,dir);
    NCHMainSys("_partfull_HF1_nocut_NSD_cut1",energy,dir);
}


//________________________________________________________________________________
void NCHCut23(TString dir = "v25", double energy = 7) {

    NCHMainSys("_partfull_HF0_ATLAS1_INEL_cut2",energy,dir);
    NCHMainSys("_partfull_HF1_ATLAS1_NSD_cut2",energy,dir);
    NCHMainSys("_partfull_HF0_ATLAS2_INEL_cut2",energy,dir);
    NCHMainSys("_partfull_HF1_ATLAS2_NSD_cut2",energy,dir);
    NCHMainSys("_partfull_HF0_ATLAS6_INEL_cut2",energy,dir);
    NCHMainSys("_partfull_HF1_ATLAS6_NSD_cut2",energy,dir);
    NCHMainSys("_partfull_HF0_ALICE_INEL_cut2",energy,dir);
    NCHMainSys("_partfull_HF1_ALICE_NSD_cut2",energy,dir);
    NCHMainSys("_partfull_HF0_MBUEWG_INEL_cut2",energy,dir);
    NCHMainSys("_partfull_HF1_MBUEWG_NSD_cut2",energy,dir);   
    NCHMainSys("_partfull_HF0_nocut_INEL_cut2",energy,dir);
    NCHMainSys("_partfull_HF1_nocut_NSD_cut2",energy,dir);

    NCHMainSys("_partfull_HF0_ATLAS1_INEL_cut3",energy,dir);
    NCHMainSys("_partfull_HF1_ATLAS1_NSD_cut3",energy,dir);
    NCHMainSys("_partfull_HF0_ATLAS2_INEL_cut3",energy,dir);
    NCHMainSys("_partfull_HF1_ATLAS2_NSD_cut3",energy,dir);
    NCHMainSys("_partfull_HF0_ATLAS6_INEL_cut3",energy,dir);
    NCHMainSys("_partfull_HF1_ATLAS6_NSD_cut3",energy,dir);
    NCHMainSys("_partfull_HF0_ALICE_INEL_cut3",energy,dir);
    NCHMainSys("_partfull_HF1_ALICE_NSD_cut3",energy,dir);
    NCHMainSys("_partfull_HF0_MBUEWG_INEL_cut3",energy,dir);
    NCHMainSys("_partfull_HF1_MBUEWG_NSD_cut3",energy,dir);   
    NCHMainSys("_partfull_HF0_nocut_INEL_cut3",energy,dir);
    NCHMainSys("_partfull_HF1_nocut_NSD_cut3",energy,dir);
}

//________________________________________________________________________________
void NCHCut45(TString dir = "v25", double energy = 7) {


    NCHMainSys("_partfull_HF0_ATLAS1_INEL_cut4",energy,dir);
    NCHMainSys("_partfull_HF1_ATLAS1_NSD_cut4",energy,dir);
    NCHMainSys("_partfull_HF0_ATLAS2_INEL_cut4",energy,dir);
    NCHMainSys("_partfull_HF1_ATLAS2_NSD_cut4",energy,dir);
    NCHMainSys("_partfull_HF0_ATLAS6_INEL_cut4",energy,dir);
    NCHMainSys("_partfull_HF1_ATLAS6_NSD_cut4",energy,dir);
    NCHMainSys("_partfull_HF0_ALICE_INEL_cut4",energy,dir);
    NCHMainSys("_partfull_HF1_ALICE_NSD_cut4",energy,dir);
    NCHMainSys("_partfull_HF0_MBUEWG_INEL_cut4",energy,dir);
    NCHMainSys("_partfull_HF1_MBUEWG_NSD_cut4",energy,dir);   
    NCHMainSys("_partfull_HF0_nocut_INEL_cut4",energy,dir);
    NCHMainSys("_partfull_HF1_nocut_NSD_cut4",energy,dir);
    
    NCHMainSys("_partfull_HF0_ATLAS1_INEL_cut5",energy,dir);
    NCHMainSys("_partfull_HF1_ATLAS1_NSD_cut5",energy,dir);
    NCHMainSys("_partfull_HF0_ATLAS2_INEL_cut5",energy,dir);
    NCHMainSys("_partfull_HF1_ATLAS2_NSD_cut5",energy,dir);
    NCHMainSys("_partfull_HF0_ATLAS6_INEL_cut5",energy,dir);
    NCHMainSys("_partfull_HF1_ATLAS6_NSD_cut5",energy,dir);
    NCHMainSys("_partfull_HF0_ALICE_INEL_cut5",energy,dir);
    NCHMainSys("_partfull_HF1_ALICE_NSD_cut5",energy,dir);
    NCHMainSys("_partfull_HF0_MBUEWG_INEL_cut5",energy,dir);
    NCHMainSys("_partfull_HF1_MBUEWG_NSD_cut5",energy,dir);   
    NCHMainSys("_partfull_HF0_nocut_INEL_cut5",energy,dir);
    NCHMainSys("_partfull_HF1_nocut_NSD_cut5",energy,dir);    
}


//________________________________________________________________________________
void NCHAll(TString dir = "v25", double energy = 7) {
   
   NCHCut01(dir, energy);
   NCHCut23(dir, energy);
   NCHCut45(dir, energy); 
}











//________________________________________________________________________________
TH1F*  makeEffSyst(TString dir , TString ctr , TString acc){

  TFile* f_unf = getFile("files/unfold_outputs/" + dir + "/unf_MC31_partfull_HF0_" + ctr + "_INEL_" + acc + ".root");
  TH1F* eff_ctr_gen = getHist<TH1F>(*f_unf , "eff_centrSel");
  TH1F* eff_trg_gen = getHist<TH1F>(*f_unf , "eff_evtSel");
  TH1F* unfoldedPtr = getHist<TH1F>(*f_unf , "nch_unfoldedPtr");
  
  TFile* f_mc = getFile("files/outputs_full/" + dir + "/effs_MC31_ferncTr_E_7_5000000_allEffs.root");
  TH1F* eff_trg_reco_mc = getHist<TH1F>(*f_mc , "eff_evtSel_RECO_HF0_" + ctr + "_RECO_" + acc);
  
  TFile* f_0b = getFile("files/outputs_full/" + dir + "/effs_zerobias_ferncTr_E_7_482270.root");
  TH1F* eff_trg_reco_0b = getHist<TH1F>(*f_0b , "eff_evtSel_RECO_HF0_" + ctr + "_RECO_" + acc);
  
  TH1F* eff_syst = (TH1F*) eff_trg_gen->Clone("eff_syst");
  
  for(int i = 1 ; i <= unfoldedPtr->GetNbinsX() ; ++i){
    Double_t bin = eff_trg_gen->GetBinContent(i)
                   + eff_trg_reco_0b->GetBinContent(i)
		   - eff_trg_reco_mc->GetBinContent(i);
    
    if(bin>1)      eff_syst->SetBinContent(i,1.);
    else if(bin<0) eff_syst->SetBinContent(i,0.);
    else           eff_syst->SetBinContent(i,bin);
  }
  
  TH1F* eff_tot = (TH1F*) eff_trg_gen->Clone("eff_tot");
  eff_tot->Multiply(eff_ctr_gen,eff_syst,1,1);
  
  
  TH1F* nch_data_corrected = (TH1F*) unfoldedPtr->Clone("nch_data_corrected");
  nch_data_corrected->Divide(nch_data_corrected , eff_tot , 1 , 1);
  
  delete f_unf ;
  delete eff_ctr_gen ;
  delete eff_trg_gen ;
  delete unfoldedPtr ;
  delete f_mc; 
  delete eff_trg_reco_mc;
  delete f_0b;
  delete eff_trg_reco_0b;
  delete eff_syst;
  delete eff_tot;
  
  return nch_data_corrected;

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
