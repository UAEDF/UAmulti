
#include <TROOT.h>
#include <TH1D.h>
#include <TGaxis.h>
#include <TMath.h>
using namespace TMath;
#include <TFile.h>
#include <TH1F.h>
#include <math.h>

#include <iostream>
#include <sstream>
using namespace std;

#include "../UAPlotFactory/src/cmsStyleRoot.C"
#include "../UAPlotFactory/src/UACanvas.C"
#include "../UAPlotFactory/src/UACurveStyleBase.C"
#include "../UAPlotFactory/src/UACurveStyle.C"
#include "../UAPlotFactory/src/UACurve.C"
#include "../UAPlotFactory/src/UAPlot.C"
#include "../UAPlotFactory/src/UALegend.C"

TString PlotDirectory = "../figs/";

bool switch_v25 = false;
bool switch_MC60 = false;
bool switch_sys = true;

//save plots
bool save =true;

const int colZ2=kGreen+1; //MC15
const int colP8=kRed;    //MC60
const int colP84c=kBlue;  //MC62
const int colCMS =kBlack;
//const int colOtherExp=;
//_____________________________________________________________________________
void NCHPlottingClean() {
    cout << " UAPlotting compiled "<< endl;

    PlotDirectory ="../figs/v42/";
    cout << " Plot directory set to: " << PlotDirectory << endl;
    //TGaxis::SetMaxDigits(3);
    UACurveStyleBase::g_markerStyle = kOpenCircle;
    UACurveStyleBase::g_markerSize = 0.8;
    UACurveStyleBase::g_lineWidth = 2;
    UACurveStyleBase::g_lineStyle = 1;   
}


//_____________________________________________________________________________
TString Txt(TString in, double E = 7) {
    
    TString out="CMS ";
    if(in.Contains("HF1")) out += "NSD ";
    if(in.Contains("_SD")) out = "CMS SD ";
   
    if(E==0.9 || in.Contains("0.9") || in.Contains("900") )   out+="900 GeV  ";
    else if(E==2.76 || in.Contains("2.76") || in.Contains("276") )   out+="2.76 TeV  ";
    else   out+="7 TeV  "; 
    
    if(in.Contains("nocut"))       out += "(nch>=0)      " ;
    else if(in.Contains("MBUEWG")) out += "(nch>=1  |#eta|<0.8)     " ;
    else if(in.Contains("ALICE"))  out += "(nch>=1  |#eta|<1.0)     " ;
    else if(in.Contains("ATLAS1")) out += "(nch>=1  |#eta|<2.4)     " ;
    else if(in.Contains("ATLAS2")) out += "(nch>=2  |#eta|<2.4)     " ;
    else if(in.Contains("ATLAS6")) out += "(nch>=6  |#eta|<2.4)     " ;
    
    if(in.Contains("cut0")) out += "p_{T}>0.1  |#eta|<2.4 " ;
    else if(in.Contains("cut1")) out += "p_{T}>0  |#eta|<2.4 " ;
    else if(in.Contains("cut2")) out += "p_{T}>0.5  |#eta|<2.4 " ;
    else if(in.Contains("cut3")) out += "p_{T}>1.0  |#eta|<2.4 " ;
    else if(in.Contains("cut4")) out += "p_{T}>0.1  |#eta|<1.0 " ;
    else if(in.Contains("cut5")) out += "p_{T}>0  |#eta|<1.0 " ;
    
    return out;
} 

//_____________________________________________________________________________
TString getMC(TString in) {
    TString out="";
    if      (in.Contains("10") )  out = "PYTHIA D6T";
    else if (in.Contains("15") )  out = "PYTHIA Z2";
    else if (in.Contains("31") )  out = "PYTHIA ATLAS";
    else if (in.Contains("60") )  out = "PYTHIA 8";
    else if (in.Contains("62") )  out = "PYTHIA 8 4c";
    return out;
}

//_____________________________________________________________________________
double NBD(double x, double nmean, double k){
  double p = 1. / ( (nmean / k) + 1 );
  //return Gamma(x+k)/( Gamma(x+1) * Gamma(k) ) * pow(p,k) * pow ( 1 - p , x);
  return Exp(LnGamma(x+k) - ( LnGamma(x+1) + LnGamma(k) )) * pow(p,k) * pow ( 1 - p , x);
}

Double_t nbdfunc2(Double_t* x, Double_t* par){
  return par[5] * 
(par[0]*NBD(x[0],par[1],par[2])+(1-par[0])*NBD(x[0],par[3],par[4]));
}

//_____________________________________________________________________________
TString getFigStr(TString main , TString parse , Double_t e = 0){
  if(e==0){
    if(parse.Contains("900") || parse.Contains("0.9"))
      main+="_900GeV";
    else if(parse.Contains("276") || parse.Contains("2.76"))
      main+="_2760GeV";
    else if(parse.Contains("7000") || parse.Contains("7TeV"))
      main+="_7000GeV";
  }
  else{
    if(e == 900 || e == 0.9)
      main+="_900GeV";
    else if(e == 276 || e == 2.76)
      main+="_2760GeV";
    else if(e == 7000 || e == 7)
      main+="_7000GeV";
  }

  TString wght ="_Weight";
  if (parse.Contains("NoWeight")) wght="_NoWeight";
  TString tr ="_mixedTr";
  if (parse.Contains("genTr") ) tr= "_genTr";
  if (parse.Contains("ferncTr") ) tr= "_ferncTr";
   
  main+=tr+wght;
  return main;
}

//_____________________________________________________________________________
void unfSteps(TString baseDir = "v41", TString MC="MC62", TString filename = "_partfull_HF0_MBUEWG_INEL_cut0.root", bool log = true ){
  // unfSteps("v41","MC62","_partfull_HF0_MBUEWG_INEL_cut0.root",true )
  cmsStyleRoot();
  bool norm=true;
  if(baseDir.Contains("withMC")) norm=false; 
  TString filepartStandard = "files/unfold_outputs/";
  TString extra="";
  if(baseDir.Contains("NoWeight")) extra="_noweight";
  
  TString dat_str="";
  if(baseDir.Contains("with")) {
    TString tempMC=baseDir;
    tempMC.Replace(0,12,"");
    //overwrite MC !!!
    MC=tempMC;
    
    TString tempDat=baseDir;
    tempDat.Replace(9,16,"");
    dat_str="="+getMC(tempDat.Replace(0,3,""));
  }  
  
  UACanvas* canvas = new UACanvas();
  UALegend* legend = new UALegend();
  
  UACurve curve1 = UACurve(1,filepartStandard+baseDir+"/unf_"+MC+"_"+extra+"_allEffs"+filename,"nch_mc_reco_raw");
  UACurve curve2 = UACurve(1,filepartStandard+baseDir+"/unf_"+MC+"_"+extra+"_allEffs"+filename,"nch_MC_gen_After_CentrEvtCorr");
  UACurve curve3 = UACurve(0,filepartStandard+baseDir+"/unf_"+MC+"_"+extra+"_allEffs"+filename,"nch_data_raw");
  UACurve curve4 = UACurve(0,filepartStandard+baseDir+"/unf_"+MC+"_"+extra+"_allEffs"+filename,"nch_unfoldedPtr","",1);
  UACurve curve5 = UACurve(0,filepartStandard+baseDir+"/unf_"+MC+"_"+extra+"_allEffs"+filename,"nch_data_corrected");
    
  if(norm) curve1.Norm(curve3);
  curve1.markerColor = kBlack ;
  curve1.lineColor   = kBlack ;
  curve1.Draw("");
  legend->AddLegend(curve1,"raw (MC "+getMC(MC)+")");  

  ((TH1F*) curve1.pCurve())->GetYaxis()->SetRangeUser(0, (((TH1F*) curve3.pCurve())->GetMaximum() )*1.1);
  if(log==true) ((TH1F*) curve1.pCurve())->GetYaxis()->SetRangeUser(1, (((TH1F*) curve1.pCurve())->GetMaximum() )*1.1); 
  ((TH1F*) curve1.pCurve())->GetYaxis()->SetTitle("# Events"); 
  ((TH1F*) curve1.pCurve())->GetXaxis()->SetRangeUser(0, 130);

  if(norm) curve2.Norm(curve5); 
  curve2.markerColor = kRed ;
  curve2.lineColor = kRed ;
  curve2.Draw("same");
  legend->AddLegend(curve2,"fully corrected (MC "+getMC(MC)+") ");  
  
  //curve3.Norm(curve1); 
  curve3.markerColor = kBlack ;
  curve3.lineColor = kBlack ;
  curve3.Draw("same"); 
  legend->AddLegend(curve3,"raw (Data"+ dat_str+")");
  
  //curve4.Norm(curve3); //norm to after EvtSel corr
  curve4.markerColor = kBlue ;
  //curve4.markerStyle = kFullCircle ;
  curve4.lineColor = kBlue ;
  curve4.Draw("same"); 
  legend->AddLegend(curve4,"after unfolding (Data"+ dat_str+")");

  //curve5.Norm(curve1);
  curve5.markerColor = kRed ;
  curve5.lineColor = kRed ; 
  curve5.Draw("same"); 
  legend->AddLegend(curve5,"fully corrected (Data"+ dat_str+")");
  
  legend->BuildLegend(0.22,0.45);
  canvas->AddText( Txt(baseDir+filename+MC),.22,.965,.035);
  if(log==true) canvas->GetCanvas()->SetLogy(1);
   
  gPad->Update(); 
  gPad->WaitPrimitive();
  if(save==true) canvas->Save("UnfoldingSteps_"+filename+baseDir,PlotDirectory);
  delete canvas ;  
}
                     
//_____________________________________________________________________________                    
//COMAPRE THE UNFOLDED FILES: thesis + v40 +v41   (INTERNAL PLOT)
void compVersionResult(TString baseDir = "v41_ferncTrNoWeight", TString mc= "MC62", TString mcold="MC60", TString filename = "_partfull_HF0_ATLAS1_INEL_cut2.root", TString plotname = "nch_data_corrected", 
                                                                        TString expdata ="../expdata/atlas_dsigdn_inelgt1_7000GeV_eta25_pt500.txt", TString legendStr ="_ATLAS1_E_7"){
  // compVersionResult("v41","MC62","MC60","_partfull_HF0_nocut_INEL_cut2.root","nch_data_corrected","","_nocut_E_7" )
  cmsStyleRoot();
  
  TString filepartStandard = "files/unfold_outputs/";
  TString thesisDir=baseDir;
  thesisDir.Replace(1,2,"25");
  
  TString nozerobiasDir=baseDir;
  nozerobiasDir.Replace(1,2,"40");
  TString extra="";
  if(baseDir.Contains("NoWeight")) extra="_noweight";

  UACanvas* canvas = new UACanvas();
  UALegend* legend = new UALegend();
  TString tr_oldextra="";
  if(baseDir.Contains("genTr")) tr_oldextra="_genTr";
  TString noMtxRewghtBaseDir=baseDir;
  noMtxRewghtBaseDir.ReplaceAll("41","42");
  
  UACurve curve1 = UACurve(0,filepartStandard+thesisDir+"/unf_"+mcold+extra+tr_oldextra+filename, plotname);
  UACurve curve2 = UACurve(0,filepartStandard+nozerobiasDir+"/unf_"+mc+"_"+extra+"_allEffs"+filename, plotname);
  UACurve curve3 = UACurve(0,filepartStandard+baseDir+"/unf_"+mc+"_"+extra+"_allEffs"+filename, plotname);
  UACurve curve4 = UACurve(0,filepartStandard+noMtxRewghtBaseDir+"/unf_"+mc+"_"+extra+"_allEffs"+filename, plotname);
  //expdata
  UACurve curve7 = UACurve(0,expdata,0);

  curve1.markerColor = kBlack ;
  curve1.markerStyle = kCircle;
  curve1.lineColor = kBlack ;
  curve1.Norm();
  curve1.Draw(""); 
  legend->AddLegend(curve1,thesisDir+" "+getMC(mcold));
  ((TH1F*) curve1.pCurve())->GetXaxis()->SetRangeUser(0, 200);
    
  curve2.markerColor = kMagenta ;
  //curve2.markerStyle = kDot;
  curve2.Norm();
  curve2.lineColor = kMagenta ;
  curve2.Draw("same"); 
  legend->AddLegend(curve2,nozerobiasDir+" "+getMC(mc));
  
  curve3.markerColor = kBlue ;
  //curve3.markerStyle = kDot;
  curve3.Norm();
  curve3.lineColor = kBlue ;
  curve3.Draw("same "); 
  legend->AddLegend(curve3,baseDir+" "+getMC(mc));  
  
  curve4.markerColor = kGreen+1 ;
  //curve4.markerStyle = kDot;
  curve4.Norm();
  curve4.lineColor = kGreen+1 ;
  curve4.Draw("same "); 
  legend->AddLegend(curve4,noMtxRewghtBaseDir+" "+getMC(mc)); 

  curve7.markerColor = kRed ;
  curve7.markerStyle = kOpenStar ;
  curve7.lineColor = kRed ;
  curve7.Draw("same");
  if(expdata.Contains("atlas"))
    legend->AddLegend(curve7,"ATLAS data");
  else if(expdata.Contains("alice"))
    legend->AddLegend(curve7,"ALICE data");
  
  double energy=7;
  TString testStr = filepartStandard+baseDir+"/"+filename;
  if(testStr.Contains("276"))        energy = 2.76;
  if(testStr.Contains("900"))        energy = 0.9;
  canvas->AddText( Txt(baseDir+filename+mc,energy),.22,.965,.035);
  
  legend->BuildLegend();
   
  gPad->Update();
  gPad->WaitPrimitive();
  if(save==true) canvas->Save("compVersionResult_"+plotname+"_"+legendStr+"_"+mc+"_"+baseDir,PlotDirectory);
  
  
  delete canvas ;

}

//_____________________________________________________________________________         
//COMAPRE THE UNFOLDED FILES: thesis + v40 +v41     (INTERNAL PLOT)
void final_compVersionResult(TString dir ="v41", TString mc = "MC62", TString mcold = "MC60"){ 
// final_compVersionResult("v41","MC62","MC60")
// final_compVersionResult("v40_900","MC60","MC60")
  TString plotname = "nch_data_corrected"; //"nch_unfoldedPtr";
  
  if ( !dir.Contains("900") && !dir.Contains("276") ){
    compVersionResult(dir, mc, mcold, "_partfull_HF0_MBUEWG_INEL_cut2.root", plotname, "", "_MBUEWG_E_7");
    compVersionResult(dir, mc, mcold, "_partfull_HF0_ATLAS1_INEL_cut2.root", plotname, "../expdata/atlas_dsigdn_inelgt1_7000GeV_eta25_pt500.txt", "_ATLAS1_E_7");
    compVersionResult(dir, mc, mcold, "_partfull_HF0_ATLAS2_INEL_cut0.root", plotname, "../expdata/atlas_dsigdn_inelgt2_7000GeV_eta25_pt100.txt", "_ATLAS2_E_7");
    compVersionResult(dir, mc, mcold, "_partfull_HF0_ATLAS6_INEL_cut2.root", plotname, "../expdata/atlas_dsigdn_inelgt6_7000GeV_eta25_pt500.txt", "_ATLAS6_E_7");
    compVersionResult(dir, mc, mcold,  "_partfull_HF0_ALICE_INEL_cut5.root", plotname, "../expdata/alice_dsigdn_inelgt1_7000GeV_eta10_pt0.txt"  ,  "_ALICE_E_7");  
  }
  else if (dir.Contains("276") ) {
    compVersionResult(dir, mc, mcold, "_partfull_HF0_MBUEWG_INEL_cut2.root", plotname, "", "_MBUEWG_E_276");
    compVersionResult(dir, mc, mcold, "_partfull_HF0_ATLAS1_INEL_cut2.root", plotname, "", "_ATLAS1_E_276");
    compVersionResult(dir, mc, mcold, "_partfull_HF0_ATLAS2_INEL_cut0.root", plotname, "", "_ATLAS2_E_276");
    compVersionResult(dir, mc, mcold, "_partfull_HF0_ATLAS6_INEL_cut2.root", plotname, "", "_ATLAS6_E_276");
    compVersionResult(dir, mc, mcold,  "_partfull_HF0_ALICE_INEL_cut5.root", plotname, "", "_ALICE_E_276" );      
  }
  else{
    compVersionResult(dir, mc, mcold, "_partfull_HF0_MBUEWG_INEL_cut2.root", plotname, "", "_MBUEWG_E_0.9");  
    compVersionResult(dir, mc, mcold, "_partfull_HF0_ATLAS1_INEL_cut2.root", plotname, "../expdata/atlas_dsigdn_inelgt1_900GeV_eta25_pt500.txt", "_ATLAS1_E_0.9");
    compVersionResult(dir, mc, mcold, "_partfull_HF0_ATLAS2_INEL_cut0.root", plotname, "../expdata/atlas_dsigdn_inelgt2_900GeV_eta25_pt100.txt", "_ATLAS2_E_0.9");
    compVersionResult(dir, mc, mcold, "_partfull_HF0_ATLAS6_INEL_cut2.root", plotname, "../expdata/atlas_dsigdn_inelgt6_900GeV_eta25_pt500.txt", "_ATLAS6_E_0.9");
    
  }
}


////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

//_____________________________________________________________________________         
//COMAPRE THE UNFOLDED FILES: thesis +v4x + data_corretec of MC's
void compMCResult(TString baseDir = "v41_ferncTrNoWeight", TString mcold="MC60", TString filename = "_partfull_HF0_ATLAS1_INEL_cut2.root", TString plotname = "nch_data_corrected", 
                                                                        TString expdata ="../expdata/atlas_dsigdn_inelgt1_7000GeV_eta25_pt500.txt", TString legendStr ="_ATLAS1_E_7", bool log=false, bool clean=false){
  // compMCResult("v41","MC60","_partfull_HF0_nocut_INEL_cut2.root","nch_data_raw","","_nocut_E_7" )
  cmsStyleRoot();
  TString filepartStandard = "files/unfold_outputs/";
  TString thesisDir=baseDir;
  thesisDir.Replace(1,2,"25");
  if(thesisDir.Contains("ferncTr")) thesisDir.ReplaceAll("_ferncTr","");
  if(clean==false) switch_sys=false;
  TString extra_dat="";
  if(clean==false) extra_dat="data ";
  TString extra="";
  if(baseDir.Contains("NoWeight")) extra="_noweight";
  TString tr_oldextra="";
  if(baseDir.Contains("genTr")) tr_oldextra="_genTr";

  UACanvas* canvas = new UACanvas();
  UALegend* legend = new UALegend();
  
  //thesis
  UACurve curve1 = UACurve(0,filepartStandard+thesisDir+"/unf_"+mcold+extra+tr_oldextra+filename, plotname);
  //MC's
  UACurve curve2 = UACurve(1,filepartStandard+baseDir+"/unf_MC15_"+extra+"_allEffs"+filename, plotname);
  UACurve curve3 = UACurve(1,filepartStandard+baseDir+"/unf_MC60_"+extra+"_allEffs"+filename, plotname);
  UACurve curve4 = UACurve(1,filepartStandard+baseDir+"/unf_MC62_"+extra+"_allEffs"+filename, plotname);
  UACurve curve4_base;//= UACurve(0,file1,"nch_data_corrected_new");
  UACurve curve4_sys;  
  //expdata
  UACurve curve7 = UACurve(0,expdata,0);

  
  if(!baseDir.Contains("900") && clean==false) {  
    curve2.markerColor = colZ2 ;
    curve2.markerStyle = kCircle;
    curve2.Norm();
    curve2.lineColor = colZ2 ;
    curve2.Draw();
    ((TH1F*) curve2.pCurve())->GetXaxis()->SetRangeUser(0, 200);
    legend->AddLegend(curve2,extra_dat+getMC("MC15")); 
  
    if(switch_MC60==true) {
       curve3.markerColor = colP8 ;
       //curve3.markerStyle = kDot;
       curve3.Norm();
       curve3.lineColor = colP8 ;
       curve3.Draw("same "); 
       legend->AddLegend(curve3,extra_dat+getMC("MC60"));  
     }
  }  
  
  if(clean==false){
    curve4.markerColor = colP84c ;
    //curve4.markerStyle = kDot;
    curve4.Norm();
    curve4.lineColor = colP84c ;
    if(!baseDir.Contains("900")) {  
        curve4.Draw("same "); 
     }
     else  {
        curve4.Draw(" ");  
        ((TH1F*) curve4.pCurve())->GetXaxis()->SetRangeUser(0, 200);
     }
     legend->AddLegend(curve4,extra_dat+getMC("MC62"));
  }
  TString file1 =filepartStandard+baseDir+"/unf_MC62_"+extra+"_allEffs"+filename;
  cout << file1 << endl;
  file1.ReplaceAll("unfold","sys");
  file1.ReplaceAll("unf","sys");

  if(switch_sys==true) {    
    curve4_base = UACurve(0,file1,"nch_data_corrected_new",true); //true forces it to be a TGraph
    curve4_base.Norm();
    curve4_base.markerColor = colCMS ;
    curve4_base.lineColor = colCMS ;
    curve4_base.markerStyle = kCircle;
    curve4_base.SerialBinKiller();
    if(clean==true) {
        curve4_base.Draw("E1");
        ((TH1F*) curve4_base.pCurve())->GetXaxis()->SetRangeUser(0, 200);
    }
    else curve4_base.Draw("E1same");
    legend->AddLegend(curve4_base,"CMS");
    curve4_sys = UACurve(0,file1,"Syst_Summed/gsyst");
    curve4_sys.SetStyle(curve4_base);
    curve4_sys.Norm();     
    cout << "----------------------------------------" << endl << "SERIAL BIN KILLER LOADED" << endl;
    curve4_sys.SerialBinKiller();
    curve4_sys.NoXError();
    curve4_sys.Draw("zsame"); 
  }
  if(switch_v25==true) {
    curve1.markerColor = kGreen+1 ;
    curve1.markerStyle = kCircle;
    curve1.lineColor = kGreen+1 ;
    curve1.Norm();
    curve1.Draw("same "); 
    legend->AddLegend(curve1,thesisDir+" "+getMC(mcold));
  }  
  
  curve7.markerColor = kRed ;
  curve7.markerStyle = kOpenStar ;
  curve7.lineColor = kRed ;
  curve7.Draw("same");
  if(expdata.Contains("atlas"))
    legend->AddLegend(curve7,"ATLAS");
  else if(expdata.Contains("alice"))
    legend->AddLegend(curve7,"ALICE ");
  
  double energy=7;
  TString testStr = filepartStandard+baseDir+"/"+filename;
  if(testStr.Contains("276"))        energy = 2.76;
  if(testStr.Contains("900"))        energy = 0.9;
  
  //fix SD name in plot instead of NSD when plotname = nch_SD_mc
  TString forPlot_str = baseDir+filename;
  if( plotname.Contains("nch_SD") )  forPlot_str.ReplaceAll("NSD","SD")  ;
  canvas->AddText( Txt(forPlot_str,energy),.22,.965,.035);
  
  legend->BuildLegend();
  if(log==true) canvas->GetCanvas()->SetLogy(1);
   
  gPad->Update();
  gPad->WaitPrimitive();
  if(save==true)  canvas->Save("compMCResult_"+plotname+"_"+legendStr+"_"+baseDir,PlotDirectory);
  
  
  delete canvas ;

}

//_____________________________________________________________________________         
//COMAPRE THE UNFOLDED FILES: thesis + v40 +v41
void final_compMCResult(TString dir ="v41", bool clean=true){ 
// final_compMCResult("v42",true)
// final_compMCResult("v40_900",true)
  TString mcold = "MC60";
  TString plotname = "nch_data_corrected"; //"nch_unfoldedPtr";
  
  if ( !dir.Contains("900") && !dir.Contains("276") ){
    compMCResult(dir, mcold, "_partfull_HF0_MBUEWG_INEL_cut2.root", plotname, "", "_MBUEWG_E_7",false,clean);
    compMCResult(dir, mcold, "_partfull_HF0_ATLAS1_INEL_cut2.root", plotname, "../expdata/atlas_dsigdn_inelgt1_7000GeV_eta25_pt500.txt", "_ATLAS1_E_7",false,clean);
    compMCResult(dir, mcold, "_partfull_HF0_ATLAS2_INEL_cut0.root", plotname, "../expdata/atlas_dsigdn_inelgt2_7000GeV_eta25_pt100.txt", "_ATLAS2_E_7",false,clean);
    compMCResult(dir, mcold, "_partfull_HF0_ATLAS6_INEL_cut2.root", plotname, "../expdata/atlas_dsigdn_inelgt6_7000GeV_eta25_pt500.txt", "_ATLAS6_E_7",false,clean);
    compMCResult(dir, mcold, "_partfull_HF0_ALICE_INEL_cut5.root" , plotname, "../expdata/alice_dsigdn_inelgt1_7000GeV_eta10_pt0.txt"  , "_ALICE_E_7" ,false,clean);  
  }
  else if (dir.Contains("276") ) {
    compMCResult(dir, mcold, "_partfull_HF0_MBUEWG_INEL_cut2.root", plotname, "", "_MBUEWG_E_276",false,clean);
    compMCResult(dir, mcold, "_partfull_HF0_ATLAS1_INEL_cut2.root", plotname, "", "_ATLAS1_E_276",false,clean);
    compMCResult(dir, mcold, "_partfull_HF0_ATLAS2_INEL_cut0.root", plotname, "", "_ATLAS2_E_276",false,clean);
    compMCResult(dir, mcold, "_partfull_HF0_ATLAS6_INEL_cut2.root", plotname, "", "_ATLAS6_E_276",false,clean);
    compMCResult(dir, mcold, "_partfull_HF0_ALICE_INEL_cut5.root" , plotname, "" , "_ALICE_E_276",false,clean);  
    }  
  else{ 
    compMCResult(dir, mcold, "_partfull_HF0_MBUEWG_INEL_cut2.root", plotname, "", "_MBUEWG_E_0.9",false,clean); 
    compMCResult(dir, mcold, "_partfull_HF0_ATLAS1_INEL_cut2.root", plotname, "../expdata/atlas_dsigdn_inelgt1_900GeV_eta25_pt500.txt", "_ATLAS1_E_0.9",false,clean);
    compMCResult(dir, mcold, "_partfull_HF0_ATLAS2_INEL_cut0.root", plotname, "../expdata/atlas_dsigdn_inelgt2_900GeV_eta25_pt100.txt", "_ATLAS2_E_0.9",false,clean);
    compMCResult(dir, mcold, "_partfull_HF0_ATLAS6_INEL_cut2.root", plotname, "../expdata/atlas_dsigdn_inelgt6_900GeV_eta25_pt500.txt", "_ATLAS6_E_0.9",false,clean);
    
  }
}


//_____________________________________________________________________________         
//COMAPRE THE UNFOLDED FILES: thesis +v4x + data_corretec of MC's
void compResultRawMC(TString baseDir = "v41_ferncTrNoWeight", TString filename = "_partfull_HF0_ATLAS1_INEL_cut2.root", TString expdata ="../expdata/atlas_dsigdn_inelgt1_7000GeV_eta25_pt500.txt",
                                                                         TString legendStr ="_ATLAS1_E_7", bool log=false, bool clean=false){
  // compResultRawMC("v41","_partfull_HF0_nocut_INEL_cut2.root","","_nocut_E_7" )
  TString rawMCplotname="nch_MC_gen_After_CentrEvtCorr";
  cmsStyleRoot();
  TString filepartStandard = "files/unfold_outputs/";
  
  TString extra="";
  if(baseDir.Contains("NoWeight")) extra="_noweight";
  TString tr_oldextra="";
  if(baseDir.Contains("genTr")) tr_oldextra="_genTr";

  UACanvas* canvas = new UACanvas();
  UALegend* legend = new UALegend();
  
  //MC's
  UACurve curve2 = UACurve(1,filepartStandard+baseDir+"/unf_MC15_"+extra+"_allEffs"+filename, rawMCplotname);
  UACurve curve3 = UACurve(1,filepartStandard+baseDir+"/unf_MC60_"+extra+"_allEffs"+filename, rawMCplotname);
  UACurve curve4 = UACurve(1,filepartStandard+baseDir+"/unf_MC62_"+extra+"_allEffs"+filename, rawMCplotname);
  UACurve curve4_base;//= UACurve(0,file1,"nch_data_corrected_new");
  UACurve curve4_sys;  
  //expdata
  UACurve curve7 = UACurve(0,expdata,0);

  
  if(!baseDir.Contains("900")) {  
    curve2.markerColor = colZ2 ;
    curve2.markerStyle = kCircle;
    curve2.Norm();
    curve2.lineColor = colZ2 ;
    curve2.Draw();
    ((TH1F*) curve2.pCurve())->GetXaxis()->SetRangeUser(0, 200);
    legend->AddLegend(curve2,getMC("MC15")); 
      
    curve3.markerColor = colP8 ;
    //curve3.markerStyle = kDot;
    curve3.Norm();
    curve3.lineColor = colP8 ;
    curve3.Draw("same "); 
    legend->AddLegend(curve3,getMC("MC60"));    
  }

  curve4.markerColor = colP84c ;
  //curve4.markerStyle = kDot;
  curve4.Norm();
  curve4.lineColor = colP84c ;
  if(!baseDir.Contains("900")) {  
     curve4.Draw("same "); 
  }
  else  {
     curve4.Draw(" ");  
     ((TH1F*) curve4.pCurve())->GetXaxis()->SetRangeUser(0, 200);
  }
  legend->AddLegend(curve4,getMC("MC62"));
  
  TString file1 =filepartStandard+baseDir+"/unf_MC62_"+extra+"_allEffs"+filename;
  cout << file1 << endl;
  file1.ReplaceAll("unfold","sys");
  file1.ReplaceAll("unf","sys");

  if(switch_sys==true) {    
    curve4_base = UACurve(0,file1,"nch_data_corrected_new",true); //true forces it to be a TGraph
    curve4_base.Norm();
    curve4_base.markerColor = colCMS ;
    curve4_base.lineColor = colCMS ;
    curve4_base.markerStyle = kCircle;
    curve4_base.SerialBinKiller();
    if(clean==true) {
        curve4_base.Draw("E1");
        ((TH1F*) curve4_base.pCurve())->GetXaxis()->SetRangeUser(0, 200);
    }
    else curve4_base.Draw("E1same");
    curve4_base.Draw("E1same");
    legend->AddLegend(curve4_base,"CMS");
    curve4_sys = UACurve(0,file1,"Syst_Summed/gsyst");
    curve4_sys.SetStyle(curve4_base);
    curve4_sys.Norm();   
    curve4_sys.SerialBinKiller();
    curve4_sys.NoXError();  
    curve4_sys.Draw("zsame"); 
  }
  
  curve7.markerColor = kRed ;
  curve7.markerStyle = kOpenStar ;
  curve7.lineColor = kRed ;
  curve7.Draw("same");
  if(expdata.Contains("atlas"))
    legend->AddLegend(curve7,"ATLAS");
  else if(expdata.Contains("alice"))
    legend->AddLegend(curve7,"ALICE ");
  
  double energy=7;
  TString testStr = filepartStandard+baseDir+"/"+filename;
  if(testStr.Contains("276"))        energy = 2.76;
  if(testStr.Contains("900"))        energy = 0.9;
  
  //fix SD name in plot instead of NSD when plotname = nch_SD_mc
  TString forPlot_str = baseDir+filename;
  canvas->AddText( Txt(forPlot_str,energy),.22,.965,.035);
  
  legend->BuildLegend();
  if(log==true) canvas->GetCanvas()->SetLogy(1);
   
  gPad->Update();
  gPad->WaitPrimitive();
  if(save==true) canvas->Save("compResultRawMC_data_corrected_"+legendStr+"_"+baseDir,PlotDirectory);
    
  delete canvas ;

}

//_____________________________________________________________________________         
//COMAPRE result with RAW MC's
void final_compResultRawMC(TString dir ="v42"){ 
// final_compResultRawMC("v41",true)
// final_compResultRawMC("v40_900",true)
  bool clean=false; // you want to see all the MC's
  TString mcold = "MC60";
  TString plotname = "nch_data_corrected"; //"nch_unfoldedPtr";
  
  if ( !dir.Contains("900") && !dir.Contains("276") ){
    /*compResultRawMC(dir, "_partfull_HF0_MBUEWG_INEL_cut2.root",  "", "_MBUEWG_E_7",false,clean);
    compResultRawMC(dir, "_partfull_HF0_ATLAS1_INEL_cut2.root",  "../expdata/atlas_dsigdn_inelgt1_7000GeV_eta25_pt500.txt", "_ATLAS1_E_7",false,clean);
    compResultRawMC(dir, "_partfull_HF0_ATLAS2_INEL_cut0.root",  "../expdata/atlas_dsigdn_inelgt2_7000GeV_eta25_pt100.txt", "_ATLAS2_E_7",false,clean);
    compResultRawMC(dir, "_partfull_HF0_ATLAS6_INEL_cut2.root",  "../expdata/atlas_dsigdn_inelgt6_7000GeV_eta25_pt500.txt", "_ATLAS6_E_7",false,clean);
    */compResultRawMC(dir, "_partfull_HF0_ALICE_INEL_cut5.root" ,  "../expdata/alice_dsigdn_inelgt1_7000GeV_eta10_pt0.txt"  , "_ALICE_E_7" ,false,clean);  
  }
  else if (dir.Contains("276") ) {
    /*compResultRawMC(dir, "_partfull_HF0_MBUEWG_INEL_cut2.root",  "", "_MBUEWG_E_276",false,clean);
    compResultRawMC(dir, "_partfull_HF0_ATLAS1_INEL_cut2.root",  "" , "_ATLAS1_E_276",false,clean);
    compResultRawMC(dir, "_partfull_HF0_ATLAS2_INEL_cut0.root",  "" , "_ATLAS2_E_276",false,clean);
    compResultRawMC(dir, "_partfull_HF0_ATLAS6_INEL_cut2.root",  "" , "_ATLAS6_E_276",false,clean);
    */compResultRawMC(dir, "_partfull_HF0_ALICE_INEL_cut5.root" ,  "" , "_ALICE_E_276" ,false,clean);  
    }  
  else{
    compResultRawMC(dir, "_partfull_HF0_MBUEWG_INEL_cut2.root",  "", "_MBUEWG_E_0.9",false,clean);  
    compResultRawMC(dir, "_partfull_HF0_ATLAS1_INEL_cut2.root",  "../expdata/atlas_dsigdn_inelgt1_900GeV_eta25_pt500.txt", "_ATLAS1_E_0.9",false,clean);
    compResultRawMC(dir, "_partfull_HF0_ATLAS2_INEL_cut0.root",  "../expdata/atlas_dsigdn_inelgt2_900GeV_eta25_pt100.txt", "_ATLAS2_E_0.9",false,clean);
    compResultRawMC(dir, "_partfull_HF0_ATLAS6_INEL_cut2.root",  "../expdata/atlas_dsigdn_inelgt6_900GeV_eta25_pt500.txt", "_ATLAS6_E_0.9",false,clean);    
  }
}


// COMPARE EFFICIENCIES
// compEff("v40","MC60","_partfull_HF0_nocut_INEL_cut2.root","eff_trTrigSel","","_nocut_E_7" )
// compEff("v41","MC60","_partfull_HF0_nocut_INEL_cut2.root","eff_trTrigSel","","_nocut_E_7" )
//_____________________________________________________________________________
void FixOneEfficiencyCurve(TH1F* eff_curve){
  for (int i=5;i<=eff_curve->GetNbinsX();i++) { 
    if ( eff_curve->GetBinContent(i-2)==1 && eff_curve->GetBinContent(i-1)==1 ) {
        eff_curve->SetBinContent(i,1);
        eff_curve->SetBinError(i,0);
    }     
    if ( eff_curve->GetBinContent(i-2)==1 && eff_curve->GetBinContent(i)==1 ) {
        eff_curve->SetBinContent(i-1,1);
        eff_curve->SetBinError(i-1,0);
    }       
  }
}

void compEffAll(TString baseDir = "v41_ferncTrNoWeight", TString filename = "_partfull_HF0_ATLAS1_INEL_cut2.root", TString plotname = "eff_trTrigSel "){
  // compEffAll("v40","_partfull_HF0_nocut_INEL_cut2.root","eff_trTrigSel")
  cmsStyleRoot();
  
  TString filepartStandard = "files/unfold_outputs/";
  
  TString extra="";
  if(baseDir.Contains("NoWeight")) extra="_noweight";
  TString tr_oldextra="";
  if(baseDir.Contains("genTr")) tr_oldextra="_genTr";

  TString zerobiasDir=baseDir;
  zerobiasDir.ReplaceAll("40","41");
  
  UACanvas* canvas = new UACanvas();
  UALegend* legend = new UALegend();
  
  //MC's
  UACurve curve2 = UACurve(1,filepartStandard+baseDir+"/unf_MC15_"+extra+"_allEffs"+filename, plotname);
  UACurve curve3 = UACurve(1,filepartStandard+baseDir+"/unf_MC60_"+extra+"_allEffs"+filename, plotname);
  UACurve curve4 = UACurve(1,filepartStandard+baseDir+"/unf_MC62_"+extra+"_allEffs"+filename, plotname);
  UACurve curve5 = UACurve(0,filepartStandard+zerobiasDir+"/unf_MC62_"+extra+"_allEffs"+filename, plotname);  
      
  FixOneEfficiencyCurve((TH1F*) curve2.pCurve());        
  FixOneEfficiencyCurve((TH1F*) curve3.pCurve());         
  FixOneEfficiencyCurve((TH1F*) curve4.pCurve()); 
  FixOneEfficiencyCurve((TH1F*) curve5.pCurve());
    
  curve2.markerColor = colZ2 ;
  curve2.markerStyle = kCircle;
  curve2.lineColor = colZ2 ;
  curve2.Draw();
  ((TH1F*) curve2.pCurve())->GetXaxis()->SetRangeUser(0, 200);
  legend->AddLegend(curve2,getMC("MC15")); 
  
  if(switch_MC60==true) {
    curve3.markerColor = colP8 ;
    //curve3.markerStyle = kDot;
    curve3.lineColor = colP8 ;
    curve3.Draw("same "); 
    legend->AddLegend(curve3,getMC("MC60"));  
  }
  curve4.markerColor = colP84c ;
  //curve4.markerStyle = kDot;
  curve4.lineColor = colP84c ;
  curve4.Draw("same "); 
  legend->AddLegend(curve4,getMC("MC62")); 
  
  curve5.markerColor = colCMS ;
  //curve5.markerStyle = kDot;
  curve5.lineColor = colCMS ;
  curve5.Draw("same "); 
  legend->AddLegend(curve5,"zeroBias");
  ((TH1F*) curve2.pCurve())->GetYaxis()->SetRangeUser(0.95, 1.01);
  ((TH1F*) curve2.pCurve())->GetXaxis()->SetRangeUser(0, 100);
  ((TH1F*) curve2.pCurve())->GetYaxis()->SetTitle("efficiency"); 
 
  double energy=7;
  TString testStr = filepartStandard+baseDir+"/"+filename;
  if(testStr.Contains("276"))        energy = 2.76;
  if(testStr.Contains("900"))        energy = 0.9;
  canvas->AddText( Txt(filename,energy),.22,.965,.035);
  
  legend->BuildLegend();
   
  gPad->Update();
  gPad->WaitPrimitive();
  if(save==true) canvas->Save("compEffAll_"+plotname+"_"+filename+"_"+baseDir,PlotDirectory);
  delete canvas ;
}

void compEff(TString baseDir = "v41_ferncTrNoWeight", TString mcold="MC60", TString filename = "_partfull_HF0_ATLAS1_INEL_cut2.root", TString plotname = "nch_data_corrected", 
                                                                        TString expdata ="../expdata/atlas_dsigdn_inelgt1_7000GeV_eta25_pt500.txt", TString legendStr ="_ATLAS1_E_7"){
  // compEff("v40","MC60","_partfull_HF0_nocut_INEL_cut2.root","eff_trTrigSel","","_nocut_E_7" )
  cmsStyleRoot();
  
  TString filepartStandard = "files/unfold_outputs/";
  
  TString extra="";
  if(baseDir.Contains("NoWeight")) extra="_noweight";
  TString tr_oldextra="";
  if(baseDir.Contains("genTr")) tr_oldextra="_genTr";

  TString zerobiasDir=baseDir;
  zerobiasDir.ReplaceAll("40","41");
  
  UACanvas* canvas = new UACanvas();
  UALegend* legend = new UALegend();
  
  //MC's
  UACurve curve2 = UACurve(1,filepartStandard+baseDir+"/unf_MC15_"+extra+"_allEffs"+filename, plotname);
  UACurve curve3 = UACurve(1,filepartStandard+baseDir+"/unf_MC60_"+extra+"_allEffs"+filename, plotname);
  UACurve curve4 = UACurve(1,filepartStandard+baseDir+"/unf_MC62_"+extra+"_allEffs"+filename, plotname);
  UACurve curve5 = UACurve(0,filepartStandard+zerobiasDir+"/unf_MC62_"+extra+"_allEffs"+filename, plotname);
  
  
  curve2.markerColor = colZ2 ;
  curve2.markerStyle = kCircle;
  curve2.lineColor = colZ2 ;
  curve2.Draw();
  ((TH1F*) curve2.pCurve())->GetXaxis()->SetRangeUser(0, 200);
  legend->AddLegend(curve2,getMC("MC15")); 
  
  if(switch_MC60==true) {
    curve3.markerColor = colP8 ;
    //curve3.markerStyle = kDot;
    curve3.lineColor = colP8 ;
    curve3.Draw("same "); 
    legend->AddLegend(curve3,getMC("MC60"));  
  }
  curve4.markerColor = colP84c ;
  //curve4.markerStyle = kDot;
  curve4.lineColor = colP84c ;
  curve4.Draw("same "); 
  legend->AddLegend(curve4,getMC("MC62")); 
  
  curve5.markerColor = colCMS ;
  //curve5.markerStyle = kDot;
  curve5.lineColor = colCMS ;
  curve5.Draw("same "); 
  legend->AddLegend(curve5,"zeroBias");
 ((TH1F*) curve2.pCurve())->GetYaxis()->SetRangeUser(0.95, 1.01);
 ((TH1F*) curve2.pCurve())->GetXaxis()->SetRangeUser(0, 100);
  ((TH1F*) curve2.pCurve())->GetYaxis()->SetTitle("efficiency"); 
  
    double energy=7;
  TString testStr = filepartStandard+baseDir+"/"+filename;
  if(testStr.Contains("276"))        energy = 2.76;
  if(testStr.Contains("900"))        energy = 0.9;
  
  //fix SD name in plot instead of NSD when plotname = nch_SD_mc
  TString forPlot_str = baseDir+filename;
  if( plotname.Contains("nch_SD") )  forPlot_str.ReplaceAll("NSD","SD")  ;
  canvas->AddText( Txt(forPlot_str,energy),.22,.965,.035);
  
  legend->BuildLegend();
   
  gPad->Update();
  gPad->WaitPrimitive();
  if(save==true) canvas->Save("compEff_"+plotname+"_"+legendStr+"_"+baseDir,PlotDirectory);
  
  
  delete canvas ;

}
////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////

//_____________________________________________________________________________         
//COMAPRE THE UNFOLDED FILES: thesis + v4x + trackings   (INTERNAL PLOT)
void compTrackResult(TString baseDir = "v41", TString Wght="", TString mc="MC62", TString mcold="MC60", TString filename = "_partfull_HF0_ATLAS1_INEL_cut2.root", TString plotname = "nch_data_corrected", 
                                                                        TString expdata ="../expdata/atlas_dsigdn_inelgt1_7000GeV_eta25_pt500.txt", TString legendStr ="_ATLAS1_E_7"){
  // compTrackResult("v41","NoWeight","MC62","MC60","_partfull_HF0_nocut_INEL_cut2.root","nch_data_raw","","_nocut_E_7" )
  cmsStyleRoot();
  
  TString filepartStandard = "files/unfold_outputs/";
  TString thesisDir=baseDir;
  thesisDir.Replace(1,2,"25");
  
  TString extra="";
  if(Wght.Contains("NoWeight")) extra="_noweight";

  UACanvas* canvas = new UACanvas();
  UALegend* legend = new UALegend();
  
  //thesis
  UACurve curve1  = UACurve(1,filepartStandard+thesisDir+Wght+"/unf_"+mcold+extra+filename, plotname);
  UACurve curve1b = UACurve(1,filepartStandard+thesisDir+"_genTr"+Wght+"/unf_"+mcold+extra+"_genTr"+filename, plotname);
  //Trackings
  UACurve curve2 = UACurve(0,filepartStandard+baseDir+"_genTr"  +Wght+"/unf_"+mc+"_"+extra+"_allEffs"+filename, plotname);
  UACurve curve3 = UACurve(0,filepartStandard+baseDir+"_ferncTr"+Wght+"/unf_"+mc+"_"+extra+"_allEffs"+filename, plotname);
  UACurve curve4 = UACurve(0,filepartStandard+baseDir+           Wght+"/unf_"+mc+"_"+extra+"_allEffs"+filename, plotname);
  //expdata
  UACurve curve7 = UACurve(0,expdata,0);
  

  curve2.markerColor = kMagenta ;
  //curve2.markerStyle = kDot;
  curve2.Norm();
  curve2.lineColor = kMagenta ;
  curve2.Draw("e"); 
  legend->AddLegend(curve2,baseDir+"_genTr"+Wght+" "+getMC(mc)); 
  
  curve3.markerColor = kBlue ;
  //curve3.markerStyle = kDot;
  curve3.Norm();
  curve3.lineColor = kBlue ;
  curve3.Draw("same "); 
  legend->AddLegend(curve3,baseDir+"_ferncTr"+Wght+" "+getMC(mc));  
  
  curve4.markerColor = kGreen+2 ;
  //curve4.markerStyle = kDot;
  curve4.Norm();
  curve4.lineColor = kGreen+2 ;
  curve4.Draw("same "); 
  legend->AddLegend(curve4,baseDir+Wght+getMC(mc));  
  
if( !plotname.Contains("eff"))  {  
  curve1.markerColor = kBlack ;
  curve1.markerStyle = kCircle;
  curve1.lineColor = kBlack ;
  curve1.Norm();
  curve1.Draw("same "); 
  legend->AddLegend(curve1,thesisDir+"_ferncTr"+Wght+" "+getMC(mcold));
  ((TH1F*) curve1.pCurve())->GetXaxis()->SetRangeUser(0, 200);
  //((TH1F*) curve1.pCurve())->GetYaxis()->SetTitle("efficiency");
  
  curve1b.markerColor = kCyan+1 ;
  curve1b.markerStyle = kCircle;
  curve1b.lineColor = kCyan+1 ;
  curve1b.Norm();
  curve1b.Draw("same "); 
  legend->AddLegend(curve1b,thesisDir+"_genTr"+Wght+" "+getMC(mcold));
}    

  curve7.markerColor = kRed ;
  curve7.markerStyle = kOpenStar ;
  curve7.lineColor = kRed ;
  curve7.Draw("same");
  if(expdata.Contains("atlas"))
    legend->AddLegend(curve7,"ATLAS data");
  else if(expdata.Contains("alice"))
    legend->AddLegend(curve7,"ALICE data");
  
  double energy=7;
  TString testStr = filepartStandard+baseDir+"/"+filename;
  if(testStr.Contains("276"))        energy = 2.76;
  if(testStr.Contains("900"))        energy = 0.9;
  canvas->AddText( Txt(baseDir+filename,energy),.22,.965,.035);
  
  legend->BuildLegend();
   
  gPad->Update();
  gPad->WaitPrimitive();
  if(save==true) canvas->Save("compTrackResult_"+plotname+"_"+legendStr+"_"+baseDir,PlotDirectory);
  
  
  delete canvas ;

}


//_____________________________________________________________________________         
//COMAPRE THE UNFOLDED FILES: thesis + v40 +v41     (INTERNAL PLOT)
void final_compTrackResult(TString dir ="v41", TString wght="", TString mc = "MC62", TString mcold = "MC60"){ 
// final_compTrackResult("v41","", "MC62","MC60")
// final_compTrackResult("v40_900","NoWeight", "MC60","MC60")
  TString plotname = "nch_data_corrected"; //"nch_unfoldedPtr";
  
  if ( !dir.Contains("900") && !dir.Contains("276") ){
    compTrackResult(dir, wght, mc, mcold, "_partfull_HF0_MBUEWG_INEL_cut2.root", plotname,  "", "_MBUEWG_E_7");
    compTrackResult(dir, wght, mc, mcold, "_partfull_HF0_ATLAS1_INEL_cut2.root", plotname, "../expdata/atlas_dsigdn_inelgt1_7000GeV_eta25_pt500.txt", "_ATLAS1_E_7");
    compTrackResult(dir, wght, mc, mcold, "_partfull_HF0_ATLAS2_INEL_cut0.root", plotname, "../expdata/atlas_dsigdn_inelgt2_7000GeV_eta25_pt100.txt", "_ATLAS2_E_7");
    compTrackResult(dir, wght, mc, mcold, "_partfull_HF0_ATLAS6_INEL_cut2.root", plotname, "../expdata/atlas_dsigdn_inelgt6_7000GeV_eta25_pt500.txt", "_ATLAS6_E_7");
    compTrackResult(dir, wght, mc, mcold, "_partfull_HF0_ALICE_INEL_cut5.root" , plotname, "../expdata/alice_dsigdn_inelgt1_7000GeV_eta10_pt0.txt"  , "_ALICE_E_7" );  
  }
  else if (dir.Contains("276") ) {
    compTrackResult(dir, wght, mc, mcold, "_partfull_HF0_MBUEWG_INEL_cut2.root", plotname,  "", "_MBUEWG_E_276");
    compTrackResult(dir, wght, mc, mcold, "_partfull_HF0_ATLAS1_INEL_cut2.root", plotname, "", "_ATLAS1_E_276");
    compTrackResult(dir, wght, mc, mcold, "_partfull_HF0_ATLAS2_INEL_cut0.root", plotname, "", "_ATLAS2_E_276");
    compTrackResult(dir, wght, mc, mcold, "_partfull_HF0_ATLAS6_INEL_cut2.root", plotname, "", "_ATLAS6_E_276");
    compTrackResult(dir, wght, mc, mcold, "_partfull_HF0_ALICE_INEL_cut5.root" , plotname, "", "_ALICE_E_276" );  
  }
  else{ 
    compTrackResult(dir, wght, mc, mcold, "_partfull_HF0_MBUEWG_INEL_cut2.root", plotname,  "", "_MBUEWG_E_0.9"); 
    compTrackResult(dir, wght, mc, mcold, "_partfull_HF0_ATLAS1_INEL_cut2.root", plotname, "../expdata/atlas_dsigdn_inelgt1_900GeV_eta25_pt500.txt", "_ATLAS1_E_0.9");
    compTrackResult(dir, wght, mc, mcold, "_partfull_HF0_ATLAS2_INEL_cut0.root", plotname, "../expdata/atlas_dsigdn_inelgt2_900GeV_eta25_pt100.txt", "_ATLAS2_E_0.9");
    compTrackResult(dir, wght, mc, mcold, "_partfull_HF0_ATLAS6_INEL_cut2.root", plotname, "../expdata/atlas_dsigdn_inelgt6_900GeV_eta25_pt500.txt", "_ATLAS6_E_0.9");
    
  }
}

////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////
// OUTPUTS_FULL PLOTS                       CONTROL PLOTS

////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////

//_____________________________________________________________________________       
void compMCOutput(TString baseDir = "v41_ferncTrNoWeight", TString plotname = "nch_gpp_noSel_HF0_nocut_RECO_cut0", bool log =false ){ 
  // compMCOutput("v41_ferncTr","nch_trp_noSel_HF0_nocut_RECO_cut0"   )
  // compMCOutput("v41_ferncTr","nch_trp_full_HF0_nocut_RECO_cut0"    )  
  // compMCOutput("v41_ferncTr","nch_gpp_noSel_HF0_nocut_RECO_cut0"   )
  // compMCOutput("v41_ferncTr","nch_mp_partfull_HF0_nocut_RECO_cut0" )
  //
  //900GeV
  // compMCOutput("v41_900ferncTr","nch_trp_full_HF0_nocut_RECO_cut0"  )
  cmsStyleRoot();
  bool normToData = false;
  if(plotname.Contains("pt")) normToData=true;
  TString filepartStandard = "files/outputs_full/";
  if(baseDir.Contains("41")) baseDir.Replace(1,2,"40"); //is the same set 
  
  TString tr_str="_mixedTr";
  if(baseDir.Contains("ferncTr")) tr_str="_ferncTr";
  if(baseDir.Contains("genTr")) tr_str="_genTr";
  
  TString thesisDir=baseDir;
  thesisDir.Replace(1,2,"25");
  TString thesis_tr_str=tr_str;
  if(thesisDir.Contains("ferncTr")) thesisDir.ReplaceAll("_ferncTr","");
  if(tr_str.Contains("mixedTr")) thesis_tr_str="_ferncTr";
  
  TString wght="";
  if(baseDir.Contains("NoWeight")) wght="_noweight";  
  
  TString en_str="7";
  if(baseDir.Contains("900")) en_str="0.9";
  if(baseDir.Contains("276")) en_str="2.76";
  
  UACanvas* canvas = new UACanvas();
  UALegend* legend = new UALegend();
  UACurve curve1 = UACurve(1,filepartStandard+baseDir  +"/output_MC15"+tr_str       +"_E_"+en_str+"_5000000"+wght+"_allEffs.root", plotname);
  UACurve curve2 = UACurve(1,filepartStandard+baseDir  +"/output_MC60"+tr_str       +"_E_"+en_str+"_5000000"+wght+"_allEffs.root", plotname);
  UACurve curve3 = UACurve(1,filepartStandard+baseDir  +"/output_MC62"+tr_str       +"_E_"+en_str+"_5000000"+wght+"_allEffs.root", plotname);
  UACurve curve4 = UACurve(0,filepartStandard+thesisDir+"/output_data"+thesis_tr_str+"_E_"+en_str+"_342220.root"                 , plotname);  
  UACurve curve5 = UACurve(0,filepartStandard+baseDir  +"/output_data"+tr_str       +"_E_"+en_str+"_342220.root"                 , plotname);  
  for (int i=1;i<=((TH1F* ) curve5.pCurve())->GetNbinsX();i++) {
     cout << "x " << i << "  || y "<< ((TH1F* )curve5.pCurve())->GetBinContent(i) << endl;
  } 
  
  
  curve1.markerColor = colZ2 ;
  curve1.markerStyle = kCircle;
  curve1.lineColor = colZ2 ;
  if(normToData) curve1.Norm(curve5);
  else curve1.Norm();
  curve1.Draw("hist"); 
  legend->AddLegend(curve1,getMC("MC15"));
  //((TH1F*) curve1.pCurve())->GetXaxis()->SetRangeUser(0, 200);
  
  if(switch_MC60==true) {
    curve2.markerColor = colP8 ;
    //curve2.markerStyle = kDot;
    if(normToData) curve2.Norm(curve5);
    else curve2.Norm();
    curve2.lineColor = colP8 ;
    curve2.Draw("same"); 
    legend->AddLegend(curve2,getMC("MC60"));  
  }
  
  curve3.markerColor = colP84c ;
  //curve3.markerStyle = kDot;
  if(normToData) curve3.Norm(curve5);
  else curve3.Norm();
  curve3.lineColor = colP84c ;
  curve3.Draw("same"); 
  legend->AddLegend(curve3,getMC("MC62"));
  
  if(!plotname.Contains("gpp")) {  
        if(switch_v25==true) {
            curve4.markerColor = kMagenta ;
            //curve4.markerStyle = kDot;
            if(normToData) curve4.Norm(curve5);
            else curve4.Norm();
            curve4.lineColor = kMagenta ;
            curve4.Draw("same"); 
            legend->AddLegend(curve4,"thesis data");   
        }
        curve5.markerColor = colCMS ;
        //curve5.markerStyle = kDot;
        if(!normToData) curve5.Norm();
        curve5.lineColor = colCMS ;
        curve5.Draw("same"); 
        legend->AddLegend(curve5,"data"); 
  }
  double energy=7;
  TString testStr = filepartStandard+baseDir+"/"+tr_str+"_E_"+en_str+"_5000000"+wght+"_allEffs.root";
  if(testStr.Contains("276"))        energy = 2.76;
  if(testStr.Contains("900"))        energy = 0.9;
  canvas->AddText( Txt(baseDir+tr_str+"_E_"+en_str+"_5000000"+wght+plotname,energy),.22,.965,.035);
  //canvas->AddText( plotname,.42,.965,.035);
  legend->BuildLegend();
  
  if(log==true) canvas->GetCanvas()->SetLogy(1);
  gPad->Update();
  gPad->WaitPrimitive();
 if(save==true) canvas->Save("compMCOutput_"+plotname+"_"+baseDir,PlotDirectory);
  
  delete canvas ;
}

//_____________________________________________________________________________       
//call all the controlplots
void final_compMCOutput(TString baseDir = "v41_ferncTrNoWeight", TString part="trp_full_HF0_nocut_RECO_cut0") {
   // final_compMCOutput("v41_ferncTrNoWeight", "trp_full_HF0_nocut_RECO_cut0")
    part="_"+part;
    compMCOutput(baseDir, "nch"+part,true);
    compMCOutput(baseDir, "eta"+part);
    compMCOutput(baseDir, "pt"+part,true);
    compMCOutput(baseDir, "phi"+part);
    compMCOutput(baseDir, "nhit"+part);
    compMCOutput(baseDir, "chi2n"+part);
    compMCOutput(baseDir, "dzOsz"+part);
    compMCOutput(baseDir, "dxyOsxy"+part);
    compMCOutput(baseDir, "eptOpt"+part);
    compMCOutput(baseDir, "dz"+part);
    compMCOutput(baseDir, "d0"+part);
    
    part.ReplaceAll("trp","vtxp");
    compMCOutput(baseDir, "x"+part);
    compMCOutput(baseDir, "y"+part);
    compMCOutput(baseDir, "z"+part);
    compMCOutput(baseDir, "ntracks"+part);
}


//_____________________________________________________________________________         
// SD comparision
void compSD(TString baseDir = "v41_ferncTrNoWeight", TString filename = "_partfull_HF1_ATLAS1_NSD_cut2.root", bool log=false){
  // compSD("v41","_partfull_HF1_nocut_NSD_cut2.root", false )
  cmsStyleRoot();
  TString plotname = "nch_SD_mc";
  TString filepartStandard = "files/unfold_outputs/";
  TString thesisDir=baseDir;
  thesisDir.Replace(1,2,"25");
  if(thesisDir.Contains("ferncTr")) thesisDir.ReplaceAll("_ferncTr","");
  
  TString extra="";
  if(baseDir.Contains("NoWeight")) extra="_noweight";
  TString tr_oldextra="";
  if(baseDir.Contains("genTr")) tr_oldextra="_genTr";

  UACanvas* canvas = new UACanvas();
  UALegend* legend = new UALegend();
  

  //MC's
  UACurve curve2 = UACurve(1,filepartStandard+baseDir+"/unf_MC15_"+extra+"_allEffs"+filename, plotname);
  UACurve curve3 = UACurve(1,filepartStandard+baseDir+"/unf_MC60_"+extra+"_allEffs"+filename, plotname);
  UACurve curve4 = UACurve(1,filepartStandard+baseDir+"/unf_MC62_"+extra+"_allEffs"+filename, plotname);
  
  if(!baseDir.Contains("900")) {  
    curve2.markerColor = colZ2 ;
    curve2.markerStyle = kCircle;
    curve2.Norm();
    curve2.lineColor = colZ2 ;
    curve2.Draw("");
    ((TH1F*) curve2.pCurve())->GetXaxis()->SetRangeUser(0, 50);
    legend->AddLegend(curve2,getMC("MC15")); 
  
    curve3.markerColor = colP8 ;
    //curve3.markerStyle = kDot;
    curve3.Norm();
    curve3.lineColor = colP8 ;
    curve3.Draw("same "); 
    legend->AddLegend(curve3,getMC("MC60"));  
  }  
  
  curve4.markerColor = colP84c ;
  //curve4.markerStyle = kDot;
  curve4.Norm();
  curve4.lineColor = colP84c ;
  if(!baseDir.Contains("900")) {  
     curve4.Draw("same "); 
  }
  else  {
     curve4.Draw(" ");  
     ((TH1F*) curve4.pCurve())->GetXaxis()->SetRangeUser(0, 50);
  }
  legend->AddLegend(curve4,getMC("MC62"));
  
  double energy=7;
  TString testStr = filepartStandard+baseDir+"/"+filename;
  if(testStr.Contains("276"))        energy = 2.76;
  if(testStr.Contains("900"))        energy = 0.9;
  
  //fix SD name in plot instead of NSD when plotname = nch_SD_mc
  TString forPlot_str = baseDir+filename;
  forPlot_str.ReplaceAll("NSD","SD");
  canvas->AddText( Txt(forPlot_str,energy),.22,.965,.035);
  
  legend->BuildLegend();
  if(log==true) canvas->GetCanvas()->SetLogy(1);
   
  gPad->Update();
  gPad->WaitPrimitive();
  if(save==true) canvas->Save("compSD_"+plotname+"_"+filename+"_"+baseDir,PlotDirectory);
  
  
  delete canvas ;

}    



//_______________________Compare NSD With Paper Results _________________________________
void xcheck_NSD(TString file1 , TString file2 , TString histo1 ,  TString histo2 , TString figname ){
  cmsStyleRoot();

  UACanvas* canvas = new UACanvas();
  UALegend* legend = new UALegend();
  UACurve curve1 = UACurve(1,file1,histo1);
  UACurve curve2 = UACurve(0,file2,histo2);
  
  curve1.markerColor = colCMS;
  curve1.lineColor = colCMS;
  curve1.Norm();
  //((TH1F*) curve1.pCurve())->GetYaxis()->SetRangeUser(1, ((TH1F*) curve1.pCurve())->GetYaxis()->GetXmax());
  ((TH1F*) curve1.pCurve())->GetXaxis()->SetRangeUser(0, 120);
  curve1.Draw("E1"); //no errors visible
  legend->AddLegend(curve1,"current NSD");

  curve2.markerColor = kRed ;
  curve2.lineColor = kRed ;
  curve2.Norm();
  curve2.Draw("same");
  legend->AddLegend(curve2,"paper NSD");
    
    
  legend->BuildLegend();
  double energy=7;
  if(file2.Contains("0.9TeV")) energy=0.9;
  canvas->AddText( Txt(file1,energy),.22,.965,.035);

  gPad->Update();
  gPad->WaitPrimitive();
  if(save==true) canvas->Save(figname,PlotDirectory);
  delete canvas ;
}


void xcheck_NSD_All(TString subdir ="v25", TString MC ="MC62"){
//if HF1 then pt_corr is automatically done for versions >=v22

  TString partold="";
  TString dir1="files/unfold_outputs/"+subdir+partold+"/";  
  TString dir2="../plots/systv10_18_2/";
  
  //TString plotname1="nch_data_raw";
  //TString plotname2="nch_data_NSD_afterSDsub";
  
  //TString plotname1="nch_SD_mc";
  //TString plotname2="nch_MC_gen_SD";
  
  //TString plotname1="nch_unfoldedPtr";
  //TString plotname2="nch_unfoldedPtr";
  
  TString plotname1="nch_data_corrected";
  TString plotname2="nch_data_corrected";
 
 
  TString figname = getFigStr("xcheck_NSD" , dir1);
  TString f1="";
  TString f2="";
 
  TString extra="";
  if (dir1.Contains("genTr") )   extra+="_genTr";
  if (dir1.Contains("NoWeight")) extra+="_noweight";
  if (!dir1.Contains("25")) extra="__allEffs";

  //xcheck_NSD(dir1+"unf_MC31"+extra+"_partfull_HF1_nocut_NSD_cut0.root" , dir2+"unfolding_MC_ATLAS_7.0TeV_mbTr__hyp1_niter0_cut10_DataType0.root" , plotname1 , plotname2 , figname);
  xcheck_NSD(dir1+"unf_"+MC+extra+"_partfull_HF1_nocut_NSD_cut1.root" , dir2+"unfolding_MC_ATLAS_7.0TeV_mbTr__hyp1_niter0_cut5_DataType0.root" , plotname1 , plotname2 , figname+"_cut1");
  //xcheck_NSD(dir1+"unf"+MC+extra+"_partfull_HF1_nocut_NSD_cut2.root" , dir2+"unfolding_MC_ATLAS_7.0TeV_mbTr__hyp1_niter0_cut15_DataType0.root" , plotname1 , plotname2 , figname);
  //xcheck_NSD(dir1+"unf"+MC+extra+"_partfull_HF1_nocut_NSD_cut3.root" , dir2+"unfolding_MC_ATLAS_7.0TeV_mbTr__hyp1_niter0_cut20_DataType0.root" , plotname1 , plotname2 , figname);
  //xcheck_NSD(dir1+"unf"+MC+extra+"_partfull_HF1_nocut_NSD_cut4.root" , dir2+"unfolding_MC_ATLAS_7.0TeV_mbTr__hyp1_niter0_cut13_DataType0.root" , plotname1 , plotname2 , figname);
  //xcheck_NSD(dir1+"unf_"+MC+extra+"_partfull_HF1_nocut_NSD_cut5.root" , dir2+"unfolding_MC_ATLAS_7.0TeV_mbTr__hyp1_niter0_cut8_DataType0.root" , plotname1 , plotname2 , figname+"_cut5");
   
} 
