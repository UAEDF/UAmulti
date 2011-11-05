
#include <TROOT.h>
#include <TH1D.h>
#include <TGaxis.h>
#include <TMath.h>
using namespace TMath;
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

//_____________________________________________________________________________
void NCHPlotting() {
    cout << " UAPlotting compiled "<< endl;

    PlotDirectory ="../figs/v40/";
    cout << " Plot directory set to: " << PlotDirectory << endl;
    //TGaxis::SetMaxDigits(3);
    UACurveStyleBase::g_markerStyle = kOpenCircle;
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
    //if(in.Contains("genTr")) out+="genTr  ";
    //if(in.Contains("mixedTr")) out+="mixedTr  ";
    //if(in.Contains("ferncTr")) out+="ferncTr  ";
    //if(in.Contains("NoWeight")) out+="NoWght  "; 
    
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
    else if(in.Contains("cut4")) out += "p_{T}>1.0  |#eta|<1.0 " ;
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

/*
// COMPARE EFFICIENCY v40xx vs v41xx 
void compVersionTwoEff(TString baseDir = "v41_ferncTrNoWeight", TString mc= "MC62", TString filename = "_partfull_HF0_ATLAS1_INEL_cut2.root", TString plotname = "eff_trTrigSel" ){  
  // compVersionTwoEff("v41","MC62","_partfull_HF0_nocut_INEL_cut2.root","eff_trTrigSel" )
  cmsStyleRoot();
  
  TString filepartStandard = "files/unfold_outputs/";
  
  TString nozerobiasDir=baseDir;
  nozerobiasDir.Replace(1,2,"40");
  TString extra="";
  if(baseDir.Contains("NoWeight")) extra="_noweight";

  UACanvas* canvas = new UACanvas();
  UALegend* legend = new UALegend();
  UACurve curve1 = UACurve(0,filepartStandard+nozerobiasDir+"/unf_"+mc+"_"+extra+"_allEffs"+filename, plotname);
  UACurve curve2 = UACurve(0,filepartStandard+baseDir+"/unf_"+mc+"_"+extra+"_allEffs"+filename, plotname);
  
 
  curve1.markerColor = kRed ;
  //curve1.markerStyle = kDot;
  //curve1.Norm();
  curve1.lineColor = kRed ;
  curve1.Draw(""); 
  ((TH1F*) curve1.pCurve())->GetXaxis()->SetRangeUser(0, 200);
  ((TH1F*) curve1.pCurve())->GetYaxis()->SetRangeUser(0.85, 1.01);
  legend->AddLegend(curve1,nozerobiasDir); 
  
  curve2.markerColor = kBlue ;
  //curve2.markerStyle = kDot;
  //curve2.Norm();
  curve2.lineColor = kBlue ;
  curve2.Draw("same "); 
  legend->AddLegend(curve2,baseDir);  
 
  double energy=7;
  TString testStr = filepartStandard+baseDir+"/"+filename;
  if(testStr.Contains("276"))        energy = 2.76;
  if(testStr.Contains("900"))        energy = 0.9;
  canvas->AddText( Txt(baseDir+filename+mc,energy),.22,.965,.035);
  

  legend->BuildLegend();
   
  gPad->Update();
  gPad->WaitPrimitive();
  canvas->Save("compVersionTwoEff_"+plotname+"_"+mc+filename,PlotDirectory);
  
  
  delete canvas ;

}
*/

void unfSteps(TString baseDir = "v41", TString MC="MC62", TString filename = "_partfull_HF0_MBUEWG_INEL_cut0.root", bool log = true ){
  // unfSteps("v41","MC62","_partfull_HF0_MBUEWG_INEL_cut0.root",true )
  cmsStyleRoot();
  
  TString filepartStandard = "files/unfold_outputs/";
  TString extra="";
  if(baseDir.Contains("NoWeight")) extra="_noweight";

  UACanvas* canvas = new UACanvas();
  UALegend* legend = new UALegend();
  
  UACurve curve1 = UACurve(1,filepartStandard+baseDir+"/unf_"+MC+"_"+extra+"_allEffs"+filename,"nch_data_raw");
  UACurve curve2 = UACurve(1,filepartStandard+baseDir+"/unf_"+MC+"_"+extra+"_allEffs"+filename,"nch_unfoldedPtr");
  UACurve curve3 = UACurve(0,filepartStandard+baseDir+"/unf_"+MC+"_"+extra+"_allEffs"+filename,"nch_data_corrected");
  UACurve curve4 = UACurve(0,filepartStandard+baseDir+"/unf_"+MC+"_"+extra+"_allEffs"+filename,"nch_MC_gen_After_CentrEvtCorr");
  UACurve curve5 = UACurve(0,filepartStandard+baseDir+"/unf_"+MC+"_"+extra+"_allEffs"+filename,"nch_mc_reco_raw");
  
  curve1.Norm(curve3);
  curve1.markerColor = kBlack ;
  curve1.lineColor   = kBlack ;
  curve1.Draw("");
  legend->AddLegend(curve1,"raw (MC)");  

  ((TH1F*) curve1.pCurve())->GetYaxis()->SetRangeUser(0, (((TH1F*) curve3.pCurve())->GetMaximum() )*1.1);
  if(log==true) ((TH1F*) curve1.pCurve())->GetYaxis()->SetRangeUser(1, (((TH1F*) curve1.pCurve())->GetMaximum() )*1.1); 
  ((TH1F*) curve1.pCurve())->GetYaxis()->SetTitle("# Events"); 
  ((TH1F*) curve1.pCurve())->GetXaxis()->SetRangeUser(0, 130);

  curve2.Norm(curve5); 
  curve2.markerColor = kRed ;
  curve2.lineColor = kRed ;
  curve2.Draw("same");
  legend->AddLegend(curve2,"fully corrected (MC) ");  
  
  curve3.Norm(curve1); 
  curve3.markerColor = kBlack ;
  curve3.lineColor = kBlack ;
  curve3.Draw("same"); 
  legend->AddLegend(curve3,"raw (Data)");
  
  //curve4.Norm(curve3); //norm to after EvtSel corr
  curve4.markerColor = kBlue ;
  //curve4.markerStyle = kFullCircle ;
  curve4.lineColor = kBlue ;
  curve4.Draw("same"); 
  legend->AddLegend(curve4,"after unfolding (Data)");
  
  //curve5.Norm(curve1);
  curve5.markerColor = kRed ;
  curve5.lineColor = kRed ;
  curve5.Draw("same"); 
  legend->AddLegend(curve5,"fully corrected (Data)");
  
  legend->BuildLegend(0.22,0.45);
  canvas->AddText( Txt(baseDir+filename+MC),.22,.965,.035);
  if(log==true) canvas->GetCanvas()->SetLogy(1);
   
  gPad->Update(); 
  gPad->WaitPrimitive();
  //canvas->Save("UnfoldingSteps_"+filename+baseDir,PlotDirectory);
  delete canvas ;  
}
                     
                     
//COMAPRE THE UNFOLDED FILES: thesis + v40 +v41
void compVersionResult(TString baseDir = "v41_ferncTrNoWeight", TString mc= "MC62", TString mcold="MC60", TString filename = "_partfull_HF0_ATLAS1_INEL_cut2.root", TString plotname = "nch_data_corrected", 
                                                                        TString expdata ="../expdata/atlas_dsigdn_inelgt1_7000GeV_eta25_pt500.txt", TString legendStr ="_ATLAS1_E_7"){
  // compVersionResult("v41","MC62","MC60","_partfull_HF0_nocut_INEL_cut2.root","nch_data_raw","","_nocut_E_7" )
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
  
  UACurve curve1 = UACurve(0,filepartStandard+thesisDir+"/unf_"+mcold+extra+tr_oldextra+filename, plotname);
  UACurve curve2 = UACurve(0,filepartStandard+nozerobiasDir+"/unf_"+mc+"_"+extra+"_allEffs"+filename, plotname);
  UACurve curve3 = UACurve(0,filepartStandard+baseDir+"/unf_"+mc+"_"+extra+"_allEffs"+filename, plotname);
  //expdata
  UACurve curve7 = UACurve(0,expdata,0);
  
  
  curve1.markerColor = kBlack ;
  curve1.markerStyle = kCircle;
  curve1.lineColor = kBlack ;
  curve1.Norm();
  curve1.Draw(""); 
  legend->AddLegend(curve1,thesisDir+" "+getMC(mcold));
  ((TH1F*) curve1.pCurve())->GetXaxis()->SetRangeUser(0, 200);
  //((TH1F*) curve1.pCurve())->GetYaxis()->SetTitle("efficiency");
  
  curve2.markerColor = kMagenta ;
  //curve2.markerStyle = kDot;
  curve2.Norm();
  curve2.lineColor = kMagenta ;
  //if(!problemDir.Contains("900")){
    curve2.Draw("same "); 
    legend->AddLegend(curve2,nozerobiasDir+" "+getMC(mc)); 
  //} 
  curve3.markerColor = kBlue ;
  //curve3.markerStyle = kDot;
  curve3.Norm();
  curve3.lineColor = kBlue ;
  curve3.Draw("same "); 
  legend->AddLegend(curve3,baseDir+" "+getMC(mc));  
  
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
 // canvas->Save("compVersionResult_"+plotname+"_"+legendStr+"_"+mc+"_"+baseDir,PlotDirectory);
  
  
  delete canvas ;

}

//COMAPRE THE UNFOLDED FILES: thesis + v40 +v41
void final_compVersionResult(TString dir ="v41", TString mc = "MC62", TString mcold = "MC60"){ 
// final_compVersionResult("v41","MC62","MC60")
// final_compVersionResult("v40_900","MC60","MC60")
  TString plotname = "nch_data_corrected"; //"nch_unfoldedPtr";
  
  if ( !dir.Contains("900") ){
    compVersionResult(dir, mc, mcold, "_partfull_HF0_ATLAS1_INEL_cut2.root", plotname, "../expdata/atlas_dsigdn_inelgt1_7000GeV_eta25_pt500.txt", "_ATLAS1_E_7");
    compVersionResult(dir, mc, mcold, "_partfull_HF0_ATLAS2_INEL_cut0.root", plotname, "../expdata/atlas_dsigdn_inelgt2_7000GeV_eta25_pt100.txt", "_ATLAS2_E_7");
    compVersionResult(dir, mc, mcold, "_partfull_HF0_ATLAS6_INEL_cut2.root", plotname, "../expdata/atlas_dsigdn_inelgt6_7000GeV_eta25_pt500.txt", "_ATLAS6_E_7");
    compVersionResult(dir, mc, mcold,  "_partfull_HF0_ALICE_INEL_cut4.root", plotname, "../expdata/alice_dsigdn_inelgt1_7000GeV_eta10_pt0.txt"  ,  "_ALICE_E_7");  
  }
  else{  
    compVersionResult(dir, mc, mcold, "_partfull_HF0_ATLAS1_INEL_cut2.root", plotname, "../expdata/atlas_dsigdn_inelgt1_900GeV_eta25_pt500.txt", "_ATLAS1_E_0.9");
    compVersionResult(dir, mc, mcold, "_partfull_HF0_ATLAS2_INEL_cut0.root", plotname, "../expdata/atlas_dsigdn_inelgt2_900GeV_eta25_pt100.txt", "_ATLAS2_E_0.9");
    compVersionResult(dir, mc, mcold, "_partfull_HF0_ATLAS6_INEL_cut2.root", plotname, "../expdata/atlas_dsigdn_inelgt6_900GeV_eta25_pt500.txt", "_ATLAS6_E_0.9");
    
  }
}


////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////

//COMAPRE THE UNFOLDED FILES: thesis +v4x + MC's
void compMCResult(TString baseDir = "v41_ferncTrNoWeight", TString mcold="MC60", TString filename = "_partfull_HF0_ATLAS1_INEL_cut2.root", TString plotname = "nch_data_corrected", 
                                                                        TString expdata ="../expdata/atlas_dsigdn_inelgt1_7000GeV_eta25_pt500.txt", TString legendStr ="_ATLAS1_E_7"){
  // compMCResult("v41","MC60","_partfull_HF0_nocut_INEL_cut2.root","nch_data_raw","","_nocut_E_7" )
  cmsStyleRoot();
  
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
  UACurve curve2 = UACurve(0,filepartStandard+baseDir+"/unf_MC15_"+extra+"_allEffs"+filename, plotname);
  UACurve curve3 = UACurve(0,filepartStandard+baseDir+"/unf_MC60_"+extra+"_allEffs"+filename, plotname);
  UACurve curve4 = UACurve(0,filepartStandard+baseDir+"/unf_MC62_"+extra+"_allEffs"+filename, plotname);
  //thesis
  UACurve curve1 = UACurve(0,filepartStandard+thesisDir+"/unf_"+mcold+extra+tr_oldextra+filename, plotname);
  //expdata
  UACurve curve7 = UACurve(0,expdata,0);

  
  curve2.markerColor = kMagenta ;
  curve2.markerStyle = kCircle;
  if( !plotname.Contains("eff"))  curve2.Norm();
  curve2.lineColor = kMagenta ;
  curve2.Draw();
  ((TH1F*) curve2.pCurve())->GetXaxis()->SetRangeUser(0, 200);
  legend->AddLegend(curve2,baseDir+" "+getMC("MC15")); 
  
  curve3.markerColor = kBlue ;
  //curve3.markerStyle = kDot;
  if( !plotname.Contains("eff"))  curve3.Norm();
  curve3.lineColor = kBlue ;
  curve3.Draw("same "); 
  legend->AddLegend(curve3,baseDir+" "+getMC("MC60"));  
  
  curve4.markerColor = kGreen+2 ;
  //curve4.markerStyle = kDot;
  if( !plotname.Contains("eff"))  curve4.Norm();
  curve4.lineColor = kGreen+2 ;
  curve4.Draw("same "); 
  legend->AddLegend(curve4,baseDir+" "+getMC("MC62")); 

if( !plotname.Contains("eff"))  {
  curve1.markerColor = kBlack ;
  curve1.markerStyle = kCircle;
  curve1.lineColor = kBlack ;
  if( !plotname.Contains("eff"))  curve1.Norm();
  curve1.Draw("same "); 
  legend->AddLegend(curve1,thesisDir+" "+getMC(mcold));
  ((TH1F*) curve1.pCurve())->GetXaxis()->SetRangeUser(0, 200);
  //((TH1F*) curve1.pCurve())->GetYaxis()->SetTitle("efficiency"); 
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
  
  //fix SD name in plot instead of NSD when plotname = nch_SD_mc
  TString forPlot_str = baseDir+filename;
  if( plotname.Contains("nch_SD") )  forPlot_str.ReplaceAll("NSD","SD")  ;
  canvas->AddText( Txt(forPlot_str,energy),.22,.965,.035);
  
  legend->BuildLegend();
   
  gPad->Update();
  gPad->WaitPrimitive();
  //canvas->Save("compMCResult_"+plotname+"_"+legendStr+"_"+baseDir,PlotDirectory);
  
  
  delete canvas ;

}
// COMPARE EFFICIENCIES
// compMCResult("v40","MC60","_partfull_HF0_nocut_INEL_cut2.root","eff_trTrigSel","","_nocut_E_7" )
// compMCResult("v41","MC60","_partfull_HF0_nocut_INEL_cut2.root","eff_trTrigSel","","_nocut_E_7" )

//COMAPRE THE UNFOLDED FILES: thesis + v40 +v41
void final_compMCResult(TString dir ="v41", TString mcold = "MC60"){ 
// final_compMCResult("v41","MC60")
// final_compMCResult("v40_900","MC60")
  TString plotname = "nch_data_corrected"; //"nch_unfoldedPtr";
  
  if ( !dir.Contains("900") ){
    compMCResult(dir, mcold, "_partfull_HF1_ATLAS1_NSD_cut2.root", plotname, "../expdata/atlas_dsigdn_inelgt1_7000GeV_eta25_pt500.txt", "_ATLAS1_E_7");
    compMCResult(dir, mcold, "_partfull_HF1_ATLAS2_NSD_cut0.root", plotname, "../expdata/atlas_dsigdn_inelgt2_7000GeV_eta25_pt100.txt", "_ATLAS2_E_7");
    compMCResult(dir, mcold, "_partfull_HF1_ATLAS6_NSD_cut2.root", plotname, "../expdata/atlas_dsigdn_inelgt6_7000GeV_eta25_pt500.txt", "_ATLAS6_E_7");
    compMCResult(dir, mcold, "_partfull_HF1_ALICE_NSD_cut4.root" , plotname, "../expdata/alice_dsigdn_inelgt1_7000GeV_eta10_pt0.txt"  , "_ALICE_E_7" );  
  }
  else{  
    compMCResult(dir, mcold, "_partfull_HF1_ATLAS1_NSD_cut2.root", plotname, "../expdata/atlas_dsigdn_inelgt1_900GeV_eta25_pt500.txt", "_ATLAS1_E_0.9");
    compMCResult(dir, mcold, "_partfull_HF1_ATLAS2_NSD_cut0.root", plotname, "../expdata/atlas_dsigdn_inelgt2_900GeV_eta25_pt100.txt", "_ATLAS2_E_0.9");
    compMCResult(dir, mcold, "_partfull_HF1_ATLAS6_NSD_cut2.root", plotname, "../expdata/atlas_dsigdn_inelgt6_900GeV_eta25_pt500.txt", "_ATLAS6_E_0.9");
    
  }
}


////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////

//COMAPRE THE UNFOLDED FILES: thesis + v4x + trackings
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
  //canvas->Save("compMCResult_"+plotname+"_"+legendStr+"_"+baseDir,PlotDirectory);
  
  
  delete canvas ;

}

//COMAPRE THE UNFOLDED FILES: thesis + v40 +v41
void final_compTrackResult(TString dir ="v41", TString wght="", TString mc = "MC62", TString mcold = "MC60"){ 
// final_compTrackResult("v41","", "MC62","MC60")
// final_compTrackResult("v40_900","NoWeight", "MC60","MC60")
  TString plotname = "nch_data_corrected"; //"nch_unfoldedPtr";
  
  if ( !dir.Contains("900") ){
    compTrackResult(dir, wght, mc, mcold, "_partfull_HF0_ATLAS1_INEL_cut2.root", plotname, "../expdata/atlas_dsigdn_inelgt1_7000GeV_eta25_pt500.txt", "_ATLAS1_E_7");
    compTrackResult(dir, wght, mc, mcold, "_partfull_HF0_ATLAS2_INEL_cut0.root", plotname, "../expdata/atlas_dsigdn_inelgt2_7000GeV_eta25_pt100.txt", "_ATLAS2_E_7");
    compTrackResult(dir, wght, mc, mcold, "_partfull_HF0_ATLAS6_INEL_cut2.root", plotname, "../expdata/atlas_dsigdn_inelgt6_7000GeV_eta25_pt500.txt", "_ATLAS6_E_7");
    compTrackResult(dir, wght, mc, mcold, "_partfull_HF0_ALICE_INEL_cut4.root" , plotname, "../expdata/alice_dsigdn_inelgt1_7000GeV_eta10_pt0.txt"  , "_ALICE_E_7" );  
  }
  else{  
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

void compMCOutput(TString baseDir = "v41_ferncTrNoWeight", TString plotname = "nch_gpp_noSel_HF0_nocut_RECO_cut0" ){ 
  // compMCOutput("v41_ferncTr","nch_trp_noSel_HF0_nocut_RECO_cut0"   )
  // compMCOutput("v41_ferncTr","nch_trp_full_HF0_nocut_RECO_cut0"    )  
  // compMCOutput("v41_ferncTr","nch_gpp_noSel_HF0_nocut_RECO_cut0"   )
  // compMCOutput("v41_ferncTr","nch_mp_partfull_HF0_nocut_RECO_cut0" )
  //
  //900GeV
  // compMCOutput("v41_900ferncTr","nch_trp_full_HF0_nocut_RECO_cut0"  )
  cmsStyleRoot();
  
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
  
  curve1.markerColor = kBlack ;
  curve1.markerStyle = kCircle;
  curve1.lineColor = kBlack ;
  curve1.Norm();
  curve1.Draw("hist"); 
  legend->AddLegend(curve1,baseDir+" "+getMC("MC15"));
  //((TH1F*) curve1.pCurve())->GetXaxis()->SetRangeUser(0, 200);
  
  curve2.markerColor = kBlue ;
  //curve2.markerStyle = kDot;
  curve2.Norm();
  curve2.lineColor = kBlue ;
  curve2.Draw("same"); 
  legend->AddLegend(curve2,baseDir+" "+getMC("MC60"));  
  
  curve3.markerColor = kRed ;
  //curve3.markerStyle = kDot;
  curve3.Norm();
  curve3.lineColor = kRed ;
  curve3.Draw("same"); 
  legend->AddLegend(curve3,baseDir+" "+getMC("MC62"));
  
  if(!plotname.Contains("gpp")) {  
        curve4.markerColor = kMagenta ;
        //curve4.markerStyle = kDot;
        curve4.Norm();
        curve4.lineColor = kMagenta ;
        curve4.Draw("same"); 
        legend->AddLegend(curve4,thesisDir);   
        
        curve5.markerColor = kCyan+2 ;
        //curve5.markerStyle = kDot;
        curve5.Norm();
        curve5.lineColor = kCyan+2 ;
        curve5.Draw("same"); 
        legend->AddLegend(curve5,baseDir); 
  }
  double energy=7;
  TString testStr = filepartStandard+baseDir+"/"+tr_str+"_E_"+en_str+"_5000000"+wght+"_allEffs.root";
  if(testStr.Contains("276"))        energy = 2.76;
  if(testStr.Contains("900"))        energy = 0.9;
  canvas->AddText( Txt(baseDir+tr_str+"_E_"+en_str+"_5000000"+wght+"_allEffs.root",energy),.22,.965,.035);
   canvas->AddText( plotname,.42,.965,.035);
  legend->BuildLegend();
  
  gPad->Update();
  gPad->WaitPrimitive();
 // canvas->Save("compMCOutput_"+plotname+"_"+baseDir,PlotDirectory);
  
  delete canvas ;
}


//call all the controlplots
void final_compMCOutput(TString baseDir = "v41_ferncTrNoWeight", TString part="trp_full_HF0_nocut_RECO_cut0") {
   // final_compMCOutput("v41_ferncTrNoWeight", "trp_full_HF0_nocut_RECO_cut0")
    part="_"+part;
    compMCOutput(baseDir, "nch"+part);
    compMCOutput(baseDir, "eta"+part);
    compMCOutput(baseDir, "pt"+part);
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
