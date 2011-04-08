 #ifndef NCHGETEFFS_
#define _NCHGETEFFS_


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
using namespace std;

#endif
bool debug_=true;


void NCHgetEffsRun(TString dir="../macro/outputs_full/v21/", TString filenameMC31  = "output_MC31_ferncTr_E_7_5000000_allEff.root", TString lastpartmc2 ="_HF0_nocut_RECO_cut0") {

 
  
  TString lastpartmc1 ="_partfull";  
  
  TString eff_output=filenameMC31;
  eff_output.ReplaceAll("output","effs");
  cout << eff_output <<endl;
  
  TString lastpartmc=lastpartmc1+lastpartmc2;

  TFile* mc = TFile::Open(dir+filenameMC31  ,"READ");  
  TFile* eff_out= new TFile(dir+eff_output,"UPDATE");  
  eff_out->cd();
  
  TH1F* eff_evtSel=0;
  TH1F* eff_centrSel=0;  
  
  
  TH1F* up_evtSel= 0; up_evtSel = (TH1F*) mc->FindObjectAny("nch_partfull"+lastpartmc2);
  TString lastpartmcnoSel = lastpartmc;
  lastpartmcnoSel.ReplaceAll("partfull","partnoSel");
  TH1F* down_evtSel= 0; down_evtSel = (TH1F*) mc->FindObjectAny("nch_partnoSel"+lastpartmc2);
  
 
  eff_evtSel = (TH1F*) up_evtSel->Clone("eff_nch"+lastpartmc2);
  eff_evtSel->Divide(up_evtSel, down_evtSel, 1, 1, "B");
  eff_evtSel->SetMinimum(0);

  delete up_evtSel; delete down_evtSel;

  TString lastmcCentrGen = lastpartmc;
  lastmcCentrGen.ReplaceAll("partfull","noSel");
  TH1F* up_centrSel= 0; up_centrSel = (TH1F*) mc->FindObjectAny("nch_noSel"+lastpartmc2);
  TString lastmcnoSel = lastpartmc;
  lastmcnoSel.ReplaceAll("partfull","CentrGen");
  TH1F* down_centrSel= 0; down_centrSel = (TH1F*) mc->FindObjectAny("nch_CentrGen"+lastpartmc2);
  
  eff_centrSel = (TH1F*) up_centrSel->Clone("eff_nch_CentrEff"+lastpartmc2);
  eff_centrSel->Divide(up_centrSel, down_centrSel, 1, 1, "B");
  eff_centrSel->SetMinimum(0);
  
  delete up_centrSel; delete down_centrSel; 

  
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
  
  TH1F* eff=(TH1F*) eff_evtSel->Clone("eff_total");
  eff->Multiply(eff_centrSel,eff_evtSel,1,1);
  
  
  
  //////////////////////::
  //L1
  TH1F* eff_L1Sel = 0;
  TH1F* up_L1Sel= 0; up_L1Sel = (TH1F*) mc->FindObjectAny("nch_L1Sel"+lastpartmc2);
  TH1F* down_L1Sel= 0; down_L1Sel = (TH1F*) mc->FindObjectAny("nch_noSel"+lastpartmc2);
  eff_L1Sel = (TH1F*) up_L1Sel->Clone("eff_nch_L1Eff"+lastpartmc2);
  eff_L1Sel->Divide(up_L1Sel, down_L1Sel, 1, 1, "B");
  eff_L1Sel->SetMinimum(0);
  
  delete up_L1Sel; delete down_L1Sel; 
  
  //////////////////////::
  //vtx
  TH1F* eff_vtxSel = 0;
  TH1F* up_vtxSel= 0; up_vtxSel = (TH1F*) mc->FindObjectAny("nch_vtxSel"+lastpartmc2);
  TH1F* down_vtxSel= 0; down_vtxSel = (TH1F*) mc->FindObjectAny("nch_noSel"+lastpartmc2);
  eff_vtxSel = (TH1F*) up_vtxSel->Clone("eff_nch_vtxEff"+lastpartmc2);
  eff_vtxSel->Divide(up_vtxSel, down_vtxSel, 1, 1, "B");
  eff_vtxSel->SetMinimum(0);
  
  delete up_vtxSel; delete down_vtxSel;
  
  //////////////////////::
  //vtxqual
  TH1F* eff_vtxqualSel = 0;
  TH1F* up_vtxqualSel= 0; up_vtxqualSel = (TH1F*) mc->FindObjectAny("nch_vtxqualSel"+lastpartmc2);
  TH1F* down_vtxqualSel= 0; down_vtxqualSel = (TH1F*) mc->FindObjectAny("nch_noSel"+lastpartmc2);
  eff_vtxqualSel = (TH1F*) up_vtxqualSel->Clone("eff_nch_vtxqualEff"+lastpartmc2);
  eff_vtxqualSel->Divide(up_vtxqualSel, down_vtxqualSel, 1, 1, "B");
  eff_vtxqualSel->SetMinimum(0);  
  
  //////////////////////::
  //vtxqual
  TH1F* eff_hfSel = 0;
  TH1F* up_hfSel= 0; up_hfSel = (TH1F*) mc->FindObjectAny("nch_hfSel"+lastpartmc2);
  TH1F* down_hfSel= 0; down_hfSel = (TH1F*) mc->FindObjectAny("nch_noSel"+lastpartmc2);
  eff_hfSel = (TH1F*) up_hfSel->Clone("eff_nch_hfEff"+lastpartmc2);
  eff_hfSel->Divide(up_hfSel, down_hfSel, 1, 1, "B");
  eff_hfSel->SetMinimum(0);
  
  delete up_hfSel; delete down_hfSel;
  
  
  //put the eff to 1 after some time
  for (int i=1;i<=eff->GetNbinsX();i++) { 
    if (i>90 && eff->GetBinContent(i)==0 )
        eff->SetBinContent(i,1);
    if (i>90 && eff_evtSel->GetBinContent(i)==0 )
        eff_evtSel->SetBinContent(i,1);    
    if (i>90 && eff_centrSel->GetBinContent(i)==0 )
        eff_centrSel->SetBinContent(i,1);
    if (i>90 && eff_L1Sel->GetBinContent(i)==0 )
        eff_L1Sel->SetBinContent(i,1);    
    if (i>90 && eff_vtxSel->GetBinContent(i)==0 )
        eff_vtxSel->SetBinContent(i,1);    
    if (i>90 && eff_vtxqualSel->GetBinContent(i)==0 )
        eff_vtxqualSel->SetBinContent(i,1);   
    if (i>90 && eff_hfSel->GetBinContent(i)==0 )
        eff_hfSel->SetBinContent(i,1); 
  }
  
  
////////////  
  gDirectory->mkdir(lastpartmc);
  gDirectory->cd(lastpartmc); 
  
  eff->Write("eff_total"+lastpartmc);
  eff_evtSel->Write("evtSel"+lastpartmc);
  eff_centrSel->Write("centrSel"+lastpartmc);
  eff_L1Sel->Write("L1Sel"+lastpartmc);
  eff_vtxSel->Write("vtxSel"+lastpartmc);
  eff_vtxqualSel->Write("vtxqualSel"+lastpartmc);
  eff_hfSel->Write("hfSel"+lastpartmc);
  
  gDirectory->cd("../"); 
  
  mc->Close();
  eff_out->Close();
  delete eff_out;
  delete mc;
  cout << "NCHgetEffs finished " << endl;
}

void NCHgetEffsZeroBias(TString dir="../macro/outputs_full/v21/", TString filenameMC31  = "output_zerobias_ferncTr_E_7_482270.root", TString lastpartmc2 ="_HF0_nocut_RECO_cut0") {

 
  
  TString lastpartmc1 ="_zerobias";    
  TString eff_output=filenameMC31;
  eff_output.ReplaceAll("output","effs");
  cout << eff_output <<endl;
  
  TString lastpartmc=lastpartmc1+lastpartmc2;

  TFile* mc = TFile::Open(dir+filenameMC31  ,"READ");
  cout <<" creating file in dir: " << dir+eff_output << endl;  
  TFile* eff_out= new TFile(dir+eff_output,"RECREATE");  
  eff_out->cd();
  
  TH1F* eff_evtSel=0;

  
  TH1F* up_evtSel= 0; up_evtSel = (TH1F*) mc->FindObjectAny("nch_full"+lastpartmc2);
  TString lastpartmcnoSel = lastpartmc;
  lastpartmcnoSel.ReplaceAll("full","noSel");
  TH1F* down_evtSel= 0; down_evtSel = (TH1F*) mc->FindObjectAny("nch_noSel"+lastpartmc2);
  
 
  eff_evtSel = (TH1F*) up_evtSel->Clone("eff_nch"+lastpartmc2);
  eff_evtSel->Divide(up_evtSel, down_evtSel, 1, 1, "B");
  eff_evtSel->SetMinimum(0);

  delete up_evtSel; delete down_evtSel;

  //put the eff to 1 after some time
  for (int i=1;i<=eff_evtSel->GetNbinsX();i++) { 
    if (i>90 && eff_evtSel->GetBinContent(i)==0 )
        eff_evtSel->SetBinContent(i,1);    
  }
  
  
////////////  
  gDirectory->mkdir(lastpartmc);
  gDirectory->cd(lastpartmc); 

  eff_evtSel->Write("evtSel"+lastpartmc);
  
  gDirectory->cd("../"); 
  
  mc->Close();
  eff_out->Write();
  delete eff_out;
  delete mc;
  
  cout << "NCHgetEffsZeroBias finished " << endl;
}
void NCHgetEffs(TString subdir = "v21" ) {
 TString filenameMC31  = "output_MC31_ferncTr_E_7_5000000_allEff.root";
  
 TString dir="../macro/outputs_full/"+subdir+"/";
  
  TString eff_output=filenameMC31;
  eff_output.ReplaceAll("output","effs");
  TFile* out= new TFile(dir+eff_output,"RECREATE");
  out->Close();
  delete out;
  cout << "Removed and recreated empty "<< dir+eff_output << endl;
   
  NCHgetEffsRun(dir, filenameMC31, "_HF0_nocut_INEL_cut0");
  NCHgetEffsRun(dir, filenameMC31, "_HF1_nocut_NSD_cut0");
  NCHgetEffsRun(dir, filenameMC31, "_HF0_MBUEWG_INEL_cut0");
  NCHgetEffsRun(dir, filenameMC31, "_HF1_MBUEWG_NSD_cut0");
  NCHgetEffsRun(dir, filenameMC31, "_HF0_ALICE_INEL_cut0");
  NCHgetEffsRun(dir, filenameMC31, "_HF1_ALICE_NSD_cut0");
  NCHgetEffsRun(dir, filenameMC31, "_HF0_ATLAS1_INEL_cut0");
  NCHgetEffsRun(dir, filenameMC31, "_HF1_ATLAS1_NSD_cut0");
  NCHgetEffsRun(dir, filenameMC31, "_HF0_ATLAS2_INEL_cut0");
  NCHgetEffsRun(dir, filenameMC31, "_HF1_ATLAS2_NSD_cut0");
  NCHgetEffsRun(dir, filenameMC31, "_HF0_ATLAS6_INEL_cut0");
  NCHgetEffsRun(dir, filenameMC31, "_HF1_ATLAS6_NSD_cut0");

}
