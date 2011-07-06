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

    PlotDirectory ="../figs/v25/";
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
   
    if(E==0.9 || in.Contains("0.9") || in.Contains("900") )   out+="900 GeV  ";
    else   out+="7 TeV  "; 
    //if(in.Contains("genTr")) out+="genTr  ";
    //if(in.Contains("NoWeight")) out+="NoWght  "; 
    
    if(in.Contains("nocut"))  out += "(nch>=0)      " ;
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
    else if(parse.Contains("7000") || parse.Contains("7TeV"))
      main+="_7000GeV";
  }
  else{
    if(e == 900 || e == 0.9)
      main+="_900GeV";
    else if(e == 7000 || e == 7)
      main+="_7000GeV";
  }

  TString wght ="_Weight";
  if (parse.Contains("NoWeight")) wght="_NoWeight";
  TString tr ="_ferncTr";
  if (parse.Contains("genTr") ) tr= "_genTr";
  
  main+=tr+wght;
  return main;
}


//_____________________________________________________________________________
void UAPlotting2_DoubleCompare (TString file1 , TString file2 , TString histoMC , TString leg1 , TString leg2 , TString legendStr ) {
  cmsStyleRoot();

  UACanvas* canvas = new UACanvas();
  UALegend* legend = new UALegend();
  UACurve curve1 = UACurve(0,file1,histoMC);
  UACurve curve2 = UACurve(0,file2,histoMC);
  
  curve1.markerColor = kRed;
  curve1.lineColor = kRed;
  //((TH1F*) curve1.pCurve())->GetYaxis()->SetRangeUser(1, ((TH1F*) curve1.pCurve())->GetYaxis()->GetXmax());
  curve1.Draw(""); //no errors visible
  legend->AddLegend(curve1,leg1);
  
  curve2.markerColor = kBlue ;
  curve2.lineColor = kBlue ;
  curve2.Draw("sameh");
  legend->AddLegend(curve2,leg2);
 
 
  legend->BuildLegend();
  canvas->AddText( Txt(legendStr),.22,.965,.035);
   
  gPad->WaitPrimitive();
  canvas->Save("DoubleCompare"+legendStr,PlotDirectory);
  delete canvas ;  
}



//_____________________________________________________________________________
void UAPlotting2_DoubleCompare2(TString file1 , TString file2 , TString leg1 , TString leg2 , TString legendStr ) {
  cmsStyleRoot();

  TString histo_raw = "nch_data_raw";
  TString histo_cor = "nch_unfoldedPtr";


  UACanvas* canvas = new UACanvas();
  UALegend* legend = new UALegend();
  UACurve curve1_raw = UACurve(0,file1,histo_raw);
  UACurve curve1_cor = UACurve(0,file1,histo_cor);
  UACurve curve2_raw = UACurve(1,file2,histo_raw);
  UACurve curve2_cor = UACurve(1,file2,histo_cor);
  
  curve1_raw.markerColor = kRed;
  curve1_raw.lineColor = kRed;
  curve1_raw.Norm();
  legend->AddLegend(curve1_raw,"data raw "+leg1);
  
  curve1_cor.markerColor = kMagenta+2;
  curve1_cor.lineColor = kMagenta+2;
  curve1_cor.markerStyle = kOpenSquare;
  curve1_cor.Norm();
  legend->AddLegend(curve1_cor,"data cor. "+leg1);
  
  curve2_raw.markerColor = kGreen+2 ;
  curve2_raw.lineColor = kGreen+2 ;
  //curve2_cor.lineStyle = 2;
  curve2_raw.Norm();
  legend->AddLegend(curve2_raw,"data raw "+leg2);
  
  curve2_cor.markerColor = kBlue ;
  curve2_cor.lineColor = kBlue ;
  curve2_cor.Norm();
  legend->AddLegend(curve2_cor,"data cor. "+leg2);
  
  
  ((TH1F*) curve2_raw.pCurve())->GetXaxis()->SetRangeUser(1 , 70);
  ((TH1F*) curve2_raw.pCurve())->GetYaxis()->SetTitle("P_{n}");
  ((TH1F*) curve2_raw.pCurve())->GetXaxis()->SetTitle("n");
  curve2_raw.Draw(""); //no errors visible
  curve2_cor.Draw("same"); //no errors visible
  curve1_raw.Draw("same");
  curve1_cor.Draw("samep");
 
 
  legend->BuildLegend(0.4,0.92);
  //canvas->AddText( Txt(legendStr),.35,.965,.035);
  canvas->AddText( Txt(legendStr),.035);
   
  gPad->WaitPrimitive();
  canvas->Save("DoubleCompare"+legendStr,PlotDirectory);
  delete canvas ;  
}
//_____________________________________________________________________________
void UAPlotting4_UnfChiCompare (TString file1 , TString file2 , TString file3, TString file4, TString histoMC ,TString legendStr ) {
  cmsStyleRoot();

  UACanvas* canvas = new UACanvas();
  UALegend* legend = new UALegend();
  UACurve curve1 = UACurve(0,file1,histoMC);
  UACurve curve2 = UACurve(0,file2,histoMC);
  UACurve curve3 = UACurve(0,file3,histoMC);
  UACurve curve4 = UACurve(0,file4,histoMC);
  
  curve1.markerColor = kRed;
  curve1.lineColor = kRed;
  //((TH1F*) curve1.pCurve())->GetYaxis()->SetRangeUser(1, ((TH1F*) curve1.pCurve())->GetYaxis()->GetXmax());
  curve1.Draw(""); //no errors visible
  legend->AddLegend(curve1,"MC10");
  
  curve2.markerColor = kGreen ;
  curve2.lineColor = kGreen ;
  curve2.Draw("sameh");
  legend->AddLegend(curve2,"MC15");
  
  curve3.markerColor = kBlue ;
  curve3.lineColor = kBlue ;
  curve3.Draw("sameh");
  legend->AddLegend(curve3,"MC60");
  
  curve4.markerColor = kMagenta ;
  curve4.lineColor = kMagenta ;
  curve4.Draw("sameh"); 
  legend->AddLegend(curve4,"MC31");

  
  legend->BuildLegend();
  canvas->AddText( Txt(legendStr),.22,.965,.035);
   
  gPad->WaitPrimitive();
  canvas->Save("UnfChiCompare"+legendStr,PlotDirectory);
  delete canvas ;  
}

//_____________________________________________________________________________
void UAPlotting6_allEffs (TString file , TString nocut, TString MBUEWG, TString ALICE, TString ATLAS1, TString ATLAS2, TString ATLAS6, TString legendStr = "") {
  cmsStyleRoot();

  UACanvas* canvas = new UACanvas();
  UALegend* legend = new UALegend();
  UACurve curve1 = UACurve(1,file,nocut);
  UACurve curve2 = UACurve(1,file,MBUEWG);
  UACurve curve3 = UACurve(1,file,ALICE);
  UACurve curve4 = UACurve(1,file,ATLAS1);
  UACurve curve5 = UACurve(1,file,ATLAS2);
  UACurve curve6 = UACurve(1,file,ATLAS6);
  
  
  curve1.markerColor = kBlack ;
  //curve1.markerStyle = kDot;
  curve1.lineColor = kBlack ;
  curve1.Draw("hist"); 
  legend->AddLegend(curve1,"nocut");
  ((TH1F*) curve1.pCurve())->GetXaxis()->SetRangeUser(0, 20);
  ((TH1F*) curve1.pCurve())->GetYaxis()->SetTitle("efficiency");
  
  curve2.markerColor = kRed ;
  //curve2.markerStyle = kDot;
  curve2.lineColor = kRed ;
  curve2.Draw("same "); 
  legend->AddLegend(curve2,"MBUEWG");
  
  curve3.markerColor = kGreen ;
  //curve3.markerStyle = kDot;
  curve3.lineColor = kGreen ;
  curve3.Draw("same "); 
  legend->AddLegend(curve3,"ALICE");
  
  curve4.markerColor = kBlue ;
  //curve4.markerStyle = kDot;
  curve4.lineColor = kBlue ;
  curve4.Draw("same "); 
  legend->AddLegend(curve4,"ATLAS1");
  
  curve5.markerColor = kMagenta ;
  //curve5.markerStyle = kDot;
  curve5.lineColor = kMagenta;
  curve5.Draw("same "); 
  legend->AddLegend(curve5,"ATLAS2");  
  
  curve6.markerColor = kCyan ;
  //curve6.markerStyle = kDot;
  curve6.lineColor = kCyan ;
  curve6.Draw("same "); 
  legend->AddLegend(curve6,"ATLAS6");
      
  legend->BuildLegend();
  canvas->AddText( Txt(legendStr),.37,.965,.035);
  
  gPad->WaitPrimitive();
  canvas->Save("allEff"+legendStr,PlotDirectory);
  delete canvas ;  
}

//_____________________________________________________________________________
void UAPlotting6_oneEff (TString file , TString one, TString three, TString four, TString five, TString six, TString seven,TString legendStr = "") {
  cmsStyleRoot();

  UACanvas* canvas = new UACanvas();
  UALegend* legend = new UALegend();
  UACurve curve1 = UACurve(0,file,one);
  UACurve curve3 = UACurve(0,file,three);
  UACurve curve4 = UACurve(0,file,four);
  UACurve curve5 = UACurve(1,file,five);
  UACurve curve6 = UACurve(1,file,six);
  UACurve curve7 = UACurve(1,file,seven);
  
  
  curve1.markerColor = kBlack ;
  //curve1.markerStyle = kDot;
  curve1.lineColor = kBlack ;
  curve1.Draw(""); 
  legend->AddLegend(curve1,"L1Sel");
  ((TH1F*) curve1.pCurve())->GetXaxis()->SetRangeUser(0, 20);
  ((TH1F*) curve1.pCurve())->GetYaxis()->SetTitle("efficiency");
  
  curve3.markerColor = kGreen ;
  //curve3.markerStyle = kDot;
  curve3.lineColor = kGreen ;
  curve3.Draw("same "); 
  legend->AddLegend(curve3,"vtxSel");
  
  curve4.markerColor = kRed ;
  //curve4.markerStyle = kDot;
  curve4.lineColor = kRed ;
  curve4.Draw("same "); 
  legend->AddLegend(curve4,"vtxqualSel");
  
  curve5.markerColor = kMagenta ;
  curve5.markerStyle = kDot;
  curve5.lineColor = kMagenta;
  curve5.Draw("same"); 
  legend->AddLegend(curve5,"evtSel");  
  
  curve6.markerColor = kCyan ;
  curve6.markerStyle = kDot;
  curve6.lineColor = kCyan ;
  curve6.Draw("same"); 
  legend->AddLegend(curve6,"centrSel"); 
  
  curve7.markerColor = kOrange ;
  //curve7.markerStyle = kDot;
  curve7.lineColor = kOrange ;
  curve7.Draw("same"); 
  legend->AddLegend(curve7,"eff_total");
      
  legend->BuildLegend();
  canvas->AddText( Txt(legendStr),.22,.965,.035);
  
  gPad->WaitPrimitive();
  canvas->Save("oneEff_"+legendStr,PlotDirectory);
  delete canvas ;  
}



//_____________________________________________________________________________
void UAPlotting5_allPlots (TString file1 , TString file2 , TString file3 ,TString file4 , TString file5, TString histodata, TString histomc, TString legendStr ="", bool log = false , Double_t ymin = 1.) {
  cmsStyleRoot();

  UACanvas* canvas = new UACanvas();
  UALegend* legend = new UALegend();
  UACurve curve1 = UACurve(0,file1,histodata);
  UACurve curve2 = UACurve(1,file2,histomc);
  UACurve curve3 = UACurve(1,file3,histomc);
  UACurve curve4 = UACurve(1,file4,histomc);
  UACurve curve5 = UACurve(1,file5,histomc); 
  
  if(histodata.Contains("nch_")){
    curve1.Scale(1.,"width");
    curve2.Scale(1.,"width");
    curve3.Scale(1.,"width");
    curve4.Scale(1.,"width");
    curve5.Scale(1.,"width");
  } 
 
 
 
  curve1.markerColor = kBlack ;
  curve1.Draw("");
  legend->AddLegend(curve1,"Data"); 
  ((TH1F*) curve1.pCurve())->GetYaxis()->SetRangeUser(0, (((TH1F*) curve1.pCurve())->GetMaximum() )*1.1);
  if(log==true) ((TH1F*) curve1.pCurve())->GetYaxis()->SetRangeUser( ymin , (((TH1F*) curve1.pCurve())->GetMaximum() )*1.1);
  
  curve2.Norm(curve1);
  curve2.markerColor = kRed ;
  //curve2.markerStyle = kDot;
  curve2.lineColor = kRed ;
  curve2.Draw("same"); 
  legend->AddLegend(curve2,"PYTHIA D6T");
  
  curve3.Norm(curve1);
  //curve3.markerStyle = kDot;
  curve3.markerColor = kGreen ;
  curve3.lineColor = kGreen ;
  curve3.Draw("same"); 
  legend->AddLegend(curve3,"PYTHIA Z2");

  curve4.Norm(curve1);
  //curve4.markerStyle = kDot;
  curve4.markerColor = kBlue ;
  curve4.lineColor = kBlue ;
  curve4.Draw("same"); 
  legend->AddLegend(curve4,"PYTHIA 8");
  
  curve5.Norm(curve1);
  //curve5.markerStyle = kDot;
  curve5.markerColor = kMagenta ;
  curve5.lineColor = kMagenta ;
  curve5.Draw("same"); 
  legend->AddLegend(curve5,"PYTHIA ATLAS");  
  legend->BuildLegend();
  if(log==true) canvas->GetCanvas()->SetLogy(1);
  
  canvas->AddText( Txt(legendStr),.22,.965,.035);
  
  gPad->WaitPrimitive();
  canvas->Save("allPlots_"+legendStr,PlotDirectory);
  delete canvas ;  
}

//_____________________________________________________________________________
void UAPlotting5_UnfRawCompare (TString file1 , TString file2 , TString file3, TString file4, TString histoData, TString histoMC ,TString legendStr ) {
  cmsStyleRoot();

  UACanvas* canvas = new UACanvas();
  UALegend* legend = new UALegend();
  UACurve curve1 = UACurve(0,file1,histoData);
  UACurve curve2 = UACurve(1,file1,histoMC);
  UACurve curve3 = UACurve(1,file2,histoMC);
  UACurve curve4 = UACurve(1,file3,histoMC);
  UACurve curve5 = UACurve(1,file4,histoMC);
  
  curve1.Draw("");
  legend->AddLegend(curve1,"Data");
  ((TH1F*) curve1.pCurve())->GetXaxis()->SetRangeUser(0, 120);
  ((TH1F*) curve1.pCurve())->GetYaxis()->SetRangeUser(2, 20000);
  canvas->GetCanvas()->SetLogy();
  ((TH1F*) curve1.pCurve())->GetYaxis()->SetTitle("# Events"); 
  
  curve2.Norm(curve1);
  curve2.markerColor = kRed ;
  curve2.lineColor = kRed ;
  curve2.Draw("same");
  legend->AddLegend(curve2,"PYTHIA D6T");
  
  curve3.Norm(curve1);
  curve3.markerColor = kGreen ;
  curve3.lineColor = kGreen ;
  curve3.Draw("same");
  legend->AddLegend(curve3,"PYTHIA Z2");
  
  curve4.Norm(curve1);
  curve4.markerColor = kBlue ;
  curve4.lineColor = kBlue ;
  curve4.Draw("same"); 
  legend->AddLegend(curve4,"PYTHIA 8");
  
  curve5.Norm(curve1);
  curve5.markerColor = kMagenta ;
  curve5.lineColor = kMagenta ;
  curve5.Draw("same"); 
  legend->AddLegend(curve5,"PYTHIA ATLAS");

  curve1.Draw("same");
  
  legend->BuildLegend(0.22,0.4);
  canvas->AddText( Txt(legendStr),.22,.965,.035);
   
  gPad->WaitPrimitive();
  canvas->Save("UnfRawCompare_"+legendStr,PlotDirectory);
  delete canvas ;  
}



//_____________________________________________________________________________
void UAPlotting5_ZeroBiasEff (TString file1 , TString file2 , TString file3 ,TString file4 , TString file5, TString histodata, TString histomc, TString legendStr ="") {
  cmsStyleRoot();

  UACanvas* canvas = new UACanvas();
  UALegend* legend = new UALegend();
  UACurve curve1 = UACurve(0,file1,histodata);
  UACurve curve2 = UACurve(1,file2,histomc);
  UACurve curve3 = UACurve(1,file3,histomc);
  UACurve curve4 = UACurve(1,file4,histomc);
  UACurve curve5 = UACurve(1,file5,histomc); 
 
  curve1.markerColor = kBlack ;
  curve1.Draw("");
  legend->AddLegend(curve1,"ZeroBias"); 
  ((TH1F*) curve1.pCurve())->GetYaxis()->SetRangeUser(0.990, 1.001);
  ((TH1F*) curve1.pCurve())->GetXaxis()->SetRangeUser(0, 60);
  
 
  curve2.markerColor = kRed ;
  //curve2.markerStyle = kDot;
  curve2.lineColor = kRed ;
  curve2.Draw("same"); 
  legend->AddLegend(curve2,"PYTHIA D6T");
  
  
  //curve3.markerStyle = kDot;
  curve3.markerColor = kGreen ;
  curve3.lineColor = kGreen ;
  curve3.Draw("same"); 
  legend->AddLegend(curve3,"PYTHIA Z2");

 
  //curve4.markerStyle = kDot;
  curve4.markerColor = kBlue ;
  curve4.lineColor = kBlue ;
  curve4.Draw("same"); 
  legend->AddLegend(curve4,"PYTHIA 8");
  

  //curve5.markerStyle = kDot;
  curve5.markerColor = kMagenta ;
  curve5.lineColor = kMagenta ;
  curve5.Draw("same"); 
  legend->AddLegend(curve5,"PYTHIA ATLAS");  
  legend->BuildLegend();

  
  canvas->AddText( Txt(legendStr),.22,.965,.035);
  
  gPad->WaitPrimitive();
  canvas->Save("allPlots_"+legendStr,PlotDirectory);
  delete canvas ;  
}
//_____________________________________________________________________________
void UAPlotting7_UnfRawCompare (TString file1 , TString file2 , TString file3, TString file4, TString file5, TString file6, TString histoData, TString histoMC , TString histoMC2, TString legendStr ) {
  cmsStyleRoot();
  
  TString fileData = file4;
  if(file1.Contains("900")) fileData = file3;

  UACanvas* canvas = new UACanvas();
  UALegend* legend = new UALegend();
  UACurve curve1 = UACurve(0,fileData,histoData);
  UACurve curve2 = UACurve(1,file1,histoMC);
  UACurve curve3 = UACurve(1,file2,histoMC);
  UACurve curve4 = UACurve(1,file3,histoMC);
  UACurve curve5 = UACurve(1,file4,histoMC);
  UACurve curve6 = UACurve(1,file5,histoMC2);
  UACurve curve7 = UACurve(1,file6,histoMC2);
  
  curve1.Draw("");
  legend->AddLegend(curve1,"Data");
  ((TH1F*) curve1.pCurve())->GetXaxis()->SetRangeUser(0, 120);
  ((TH1F*) curve1.pCurve())->GetYaxis()->SetRangeUser(2, 20000);
  if(file1.Contains("900")) 
    ((TH1F*) curve1.pCurve())->GetYaxis()->SetRangeUser(2, 400000);
  //canvas->GetCanvas()->SetLogy();
  ((TH1F*) curve1.pCurve())->GetYaxis()->SetTitle("# Events"); 
  
  curve2.Norm(curve1);
  curve2.markerColor = kRed ;
  curve2.lineColor = kRed ;
  curve2.Draw("same");
  legend->AddLegend(curve2,"PYTHIA D6T");
  
  curve3.Norm(curve1);
  curve3.markerColor = kGreen ;
  curve3.lineColor = kGreen ;
  curve3.Draw("same");
  legend->AddLegend(curve3,"PYTHIA Z2");
  
  curve4.Norm(curve1);
  curve4.markerColor = kBlue ;
  curve4.lineColor = kBlue ;
  curve4.Draw("same"); 
  legend->AddLegend(curve4,"PYTHIA 8 tune 1");
  
  curve6.Norm(curve1);
  curve6.markerColor = kOrange-6 ;
  curve6.lineColor = kOrange-6 ;
  curve6.Draw("same"); 
  legend->AddLegend(curve6,"PYTHIA 8 2C");
  
  curve7.Norm(curve1);
  curve7.markerColor = kCyan ;
  curve7.lineColor = kCyan ;
  curve7.Draw("same"); 
  legend->AddLegend(curve7,"PYTHIA 8 4C");  
  
  curve5.Norm(curve1);
  curve5.markerColor = kMagenta ;
  curve5.lineColor = kMagenta ;
  curve5.Draw("same"); 
  if(! file1.Contains("900"))legend->AddLegend(curve5,"PYTHIA ATLAS");
  
  fileData.ReplaceAll("unfold","sys");
  fileData.ReplaceAll("unf","sys");
  UACurve curve1_sys = UACurve(0,fileData,"Syst_Summed/gsyst");
  curve1_sys.SetStyle(curve1);
  curve1_sys.SerialBinKiller();
  curve1_sys.Draw("zsame");

  curve1.Draw("E1same");
  
  legend->BuildLegend(0.22,0.4);
  canvas->AddText( Txt(legendStr),.22,.965,.035);
   
  gPad->WaitPrimitive();
  canvas->Save("UnfRawCompare_"+legendStr,PlotDirectory);
  delete canvas ;  
}


//_____________________________________________________________________________
void UAPlotting5_RawPlot (TString file1 , TString file2 , TString file3, TString file4, TString file5, TString histo1, TString legendStr ) {
  cmsStyleRoot();

  UACanvas* canvas = new UACanvas();
  UALegend* legend = new UALegend();
  UACurve curve1 = UACurve(0,file1,histo1);
  UACurve curve2 = UACurve(0,file2,histo1);
  UACurve curve3 = UACurve(0,file3,histo1);
  UACurve curve4 = UACurve(0,file4,histo1);
  UACurve curve5 = UACurve(0,file5,histo1);
  
  //if(histo1.CompareTo("nch_corrected_norm") != 0 ) curve1.Norm(); 
  curve1.Draw("");
  legend->AddLegend(curve1,"nocut");

  //if(histo1.CompareTo("nch_corrected_norm") != 0 ) curve2.Norm(curve1);
  curve2.markerColor = kRed ;
  curve2.lineColor = kRed ;
  curve2.Draw("same");
  legend->AddLegend(curve2,"MBUEWG");
  
  //if(histo1.CompareTo("nch_corrected_norm") != 0 ) curve3.Norm(curve1); 
  curve3.markerColor = kGreen ;
  curve3.lineColor = kGreen ;
  curve3.Draw("same"); 
  legend->AddLegend(curve3,"ATLAS1");
  
  //if(histo1.CompareTo("nch_corrected_norm") != 0 ) curve4.Norm(curve1);
  curve4.markerColor = kBlue ;
  curve4.lineColor = kBlue ;
  curve4.Draw("same"); 
  legend->AddLegend(curve4,"ATLAS6");
  
  //if(histo1.CompareTo("nch_corrected_norm") != 0 ) curve5.Norm(curve1);
  curve5.markerColor = kMagenta ;
  curve5.lineColor = kMagenta ;
  curve5.Draw("same"); 
  legend->AddLegend(curve5,"ALICE");
  
  legend->BuildLegend();
  canvas->AddText( Txt(legendStr),.22,.965,.035);
   
  gPad->WaitPrimitive();
  canvas->Save("RawPlot_"+legendStr,PlotDirectory);
  delete canvas ;  
}

//_____________________________________________________________________________
void UAPlotting5_UnfoldingPlot (TString file1 , TString histo1 , TString histo2, TString histo3, TString histo4,
        TString histo5, TString legendStr, bool log =false ) {
  cmsStyleRoot();

  UACanvas* canvas = new UACanvas();
  UALegend* legend = new UALegend();
  UACurve curve1 = UACurve(0,file1,histo5); //changed order !!
  UACurve curve2 = UACurve(0,file1,histo4);
  UACurve curve3 = UACurve(1,file1,histo1);
  UACurve curve4 = UACurve(1,file1,histo2);
  UACurve curve5 = UACurve(1,file1,histo3);

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
  canvas->AddText( Txt(legendStr),.22,.965,.035);
  if(log==true) canvas->GetCanvas()->SetLogy(1);
   
  gPad->Update(); 
  gPad->WaitPrimitive();
  canvas->Save("UnfoldingPlot_"+legendStr,PlotDirectory);
  delete canvas ;  
}



//_____________________________________________________________________________
void UAPlotting8_UnfCompare (TString file1 , TString file2 , TString file3, TString file4, TString histoData, TString histoMC ,TString legendStr ) {
  cmsStyleRoot();

  UACanvas* canvas = new UACanvas();
  UALegend* legend = new UALegend();
  UACurve curve1 = UACurve(0,file1,histoData);
  UACurve curve2 = UACurve(0,file2,histoData); 
  UACurve curve3 = UACurve(0,file3,histoData);
  UACurve curve4 = UACurve(0,file4,histoData);   
  UACurve curve5 = UACurve(1,file1,histoMC);
  UACurve curve6 = UACurve(1,file2,histoMC);
  UACurve curve7 = UACurve(1,file3,histoMC);
  UACurve curve8 = UACurve(1,file4,histoMC);
  
  ((TH1F*) curve1.pCurve())->GetXaxis()->SetRangeUser(0, 120);
  curve1.markerColor = kRed;
  curve1.lineColor = kRed;
  curve1.Draw("");
  legend->AddLegend(curve1,"data_MC10");
  
  curve2.markerColor = kGreen ;
  curve2.lineColor = kGreen ;
  curve2.Draw("same");
  legend->AddLegend(curve2,"data_MC15");
  
  curve3.markerColor = kBlue ;
  curve3.lineColor = kBlue ;
  curve3.Draw("same");
  legend->AddLegend(curve3,"data_MC60");
  
  curve4.markerColor = kMagenta ;
  curve4.lineColor = kMagenta ;
  curve4.Draw("same"); 
  legend->AddLegend(curve4,"data_MC31");
  
  curve5.Norm(curve1);
  curve5.markerColor = kRed ;
  curve5.markerStyle = kDot ;
  curve5.lineStyle = 3;
  curve5.lineColor = kRed ;
  curve5.Draw("same"); 
  legend->AddLegend(curve5,"mc_MC10");

  curve6.Norm(curve2);
  curve6.markerColor = kGreen ;
  curve6.markerStyle = kDot ;
  curve6.lineStyle = 3;
  curve6.lineColor = kGreen ;
  curve6.Draw("same"); 
  legend->AddLegend(curve6,"mc_MC15");  
   
  curve7.Norm(curve3);
  curve7.markerColor = kBlue ;
  curve7.markerStyle = kDot ;
  curve7.lineStyle = 3;
  curve7.lineColor = kBlue ;
  curve7.Draw("same");
  legend->AddLegend(curve7,"mc_MC60"); 

  curve8.Norm(curve4);
  curve8.markerColor = kMagenta ;
  curve8.markerStyle = kDot ;
  curve8.lineStyle = 3;
  curve8.lineColor = kMagenta ;
  curve8.Draw("same"); 
  legend->AddLegend(curve8,"mc_MC31");  

  
  legend->BuildLegend();
  canvas->AddText( Txt(legendStr),.22,.965,.035);
  canvas->Save("UnfCompareModels8_"+legendStr,PlotDirectory);
  gPad->WaitPrimitive();

  delete canvas ;  
}


//_____________________________________________________________________________
void UAPlotting4_UnfCompare (TString file1 , TString file2 , TString file3, TString file4, TString histoData ,TString legendStr ) {
  cmsStyleRoot();

  UACanvas* canvas = new UACanvas();
  UALegend* legend = new UALegend();
  UACurve curve1 = UACurve(0,file1,histoData);
  UACurve curve2 = UACurve(0,file2,histoData); 
  UACurve curve3 = UACurve(0,file3,histoData);
  UACurve curve4 = UACurve(0,file4,histoData);
  
  canvas->GetCanvas()->SetLogy();
  ((TH1F*) curve1.pCurve())->GetXaxis()->SetRangeUser(0, 120);
  ((TH1F*) curve1.pCurve())->GetYaxis()->SetRangeUser(10, 10000);
  ((TH1F*) curve1.pCurve())->GetYaxis()->SetTitle("# Events"); 
  curve1.markerColor = kRed;
  curve1.lineColor = kRed;
  curve1.Draw("");
  legend->AddLegend(curve1,"Data (PYTHIA D6T)");
  
  curve2.markerColor =
 kGreen ;
  curve2.lineColor = kGreen ;
  curve2.Draw("same");
  legend->AddLegend(curve2,"Data (PYTHIA Z2)");
  
  curve3.markerColor = kBlue ;
  curve3.lineColor = kBlue ;
  curve3.Draw("same");
  legend->AddLegend(curve3,"Data (PYTHIA 8)");
  
  curve4.markerColor = kMagenta ;
  curve4.lineColor = kMagenta ;
  curve4.Draw("same"); 
  legend->AddLegend(curve4,"Data (PYTHIA ATLAS)");
  
  legend->BuildLegend(0.22,0.4);
  canvas->AddText( Txt(legendStr),.22,.965,.035);
  //canvas->AddText( Txt(legendStr),.965,.035);
  canvas->Save("UnfCompareModels_"+legendStr,PlotDirectory);
  
  gPad->WaitPrimitive();

  delete canvas ;  
}


////////////////////////////////////////////////////////
////////////////////////////////////////////////////////

//_______________________Compare different Central Cuts during Unfolding__________________________________
void DoubleCompare(int mctype = 31, int acc = 0 , TString hf = "HF0", TString subdir = "v25", TString name ="partfull" ) {
  //Example: DoubleCompare(31,0,"HF0","v25")

  TString CentrCut = "_ATLAS2_";
  TString Difftype = "INEL";  //before v11  "INEL"
  stringstream mctypestr("");	  mctypestr << "_MC" << mctype ;
  stringstream cutstr("");	  cutstr << "_cut" << acc ;
  //get all root files in the directory
  TString Energy = "_E_7";
  if (subdir.Contains("900") ) Energy = "_E_0.9";
  TString figName =  mctypestr.str() + "_" + hf + CentrCut + Difftype + cutstr.str() + Energy ;

  TString filename  = "unf"+mctypestr.str()+"_"+name+"_"+hf+CentrCut+Difftype+cutstr.str()+".root"; 
  TString filename_NoWeight  = "unf"+mctypestr.str()+"_noweight_"+name+"_"+hf+CentrCut+Difftype+cutstr.str()+".root"; 
  TString filename_genTr  = "unf"+mctypestr.str()+"_genTr_"+name+"_"+hf+CentrCut+Difftype+cutstr.str()+".root"; 
  TString dirNoWeight="files/unfold_outputs/"+subdir+"NoWeight/";
  TString dir="files/unfold_outputs/"+subdir+"/";
  TString dirgenTr="files/unfold_outputs/"+subdir+"_genTr/";
  TString dirRomain="files/unfold_outputs/v22NoWeight_Romain/";
  
  UAPlotting2_DoubleCompare2(dirNoWeight+filename_NoWeight, dir+filename, "(no z-reweight)", "(with z-reweight)", figName + "_NoWght_Wght");
  UAPlotting2_DoubleCompare2(dirgenTr+filename_genTr, dir+filename, "(general Tr.)", "(minbias Tr.)" , figName + "_gTr_fTr"); 
  UAPlotting2_DoubleCompare2(dirRomain+filename_NoWeight, dirNoWeight+filename_NoWeight, "(ept/pt = 0.05)", "(ept/pt = 0.1)" , figName + "_eptOpt"); 
 
  //UAPlotting2_DoubleCompare(dirNoWeight+filename_NoWeight, dir+filename, "nch_unfoldedPtr" ,  "nch_data_corrected_" + mctypestr.str()+hf+Difftype+cutstr.str()+Energy+"_NoWght_Wght", "NoWeight", "Weight"  );
  //UAPlotting2_DoubleCompare(dirgenTr+filename_genTr, dir+filename, "nch_unfoldedPtr" ,  "nch_unfoldedPtr" + mctypestr.str()+hf+Difftype+cutstr.str()+Energy+"_gTr_fTr", "genTr", "ferncTr"  ); 
}


//_______________________Compare different Central Cuts during Unfolding__________________________________

void RawPlot(int mctype = 31, int acc = 0, TString hf = "HF0", TString subdir = "v25", TString name ="partfull" ) {
   //Example: RawPlot(31,0,"HF0","v25")
   //MC:      RawPlot(31,0,"HF0","v20b","MCcorrMC_partfull")

   TString Difftype = "INEL";  //before v11  "INEL"
   stringstream mctypestr("");     mctypestr << "_MC" << mctype ;
   stringstream cutstr("");        cutstr << "_cut" << acc ;
   //get all root files in the directory
   
   TString Energy = "_E_7";
   if (subdir.Contains("900") ) Energy = "_E_0.9";
   TString tr ="_ferncTr";
   if (subdir.Contains("genTr") ) tr= "_genTr";
   TString wght ="_Weight";
   if (subdir.Contains("NoWeight")) wght="_NoWeight";
   
   TString extra="";
   if (subdir.Contains("genTr") )   extra+="_genTr";
   if (subdir.Contains("NoWeight")) extra+="_noweight";
   
   TString filenamenocut  = "unf"+mctypestr.str()+extra+"_"+name+"_"+hf+"_nocut_" +Difftype+cutstr.str()+".root";
   TString filenameMBUEWG = "unf"+mctypestr.str()+extra+"_"+name+"_"+hf+"_MBUEWG_"+Difftype+cutstr.str()+".root";
   TString filenameATLAS1 = "unf"+mctypestr.str()+extra+"_"+name+"_"+hf+"_ATLAS1_"+Difftype+cutstr.str()+".root";
   TString filenameATLAS6 = "unf"+mctypestr.str()+extra+"_"+name+"_"+hf+"_ATLAS6_"+Difftype+cutstr.str()+".root";
   TString filenameALICE  = "unf"+mctypestr.str()+extra+"_"+name+"_"+hf+"_ALICE_" +Difftype+cutstr.str()+".root";
   //cout << filenamenocut<<endl;
   //cout << "unf_MC15_partfull_HF0_ALICE_INEL_cut0.root" << endl;
  
  TString dir="files/unfold_outputs/"+subdir+"/";
  UAPlotting5_RawPlot(dir+filenamenocut, dir+filenameMBUEWG,dir+filenameATLAS1, dir+filenameATLAS6, dir+filenameALICE ,  "nch_data_raw"  ,  "nch_data_raw_" + mctypestr.str()+hf+Energy+tr+wght );
  UAPlotting5_RawPlot(dir+filenamenocut, dir+filenameMBUEWG,dir+filenameATLAS1, dir+filenameATLAS6, dir+filenameALICE ,  "nch_data_corrected" ,  "nch_data_corrected_" + mctypestr.str()+hf+Energy+tr+wght  );
  UAPlotting5_RawPlot(dir+filenamenocut, dir+filenameMBUEWG,dir+filenameATLAS1, dir+filenameATLAS6, dir+filenameALICE ,  "nch_resampled", "nch_resampled_" + mctypestr.str()+hf+Energy+tr+wght );
  UAPlotting5_RawPlot(dir+filenamenocut, dir+filenameMBUEWG,dir+filenameATLAS1, dir+filenameATLAS6, dir+filenameALICE ,  "nch_unfoldedPtr" , "nch_unfoldedPtr_"+ mctypestr.str()+hf+Energy+tr+wght );
  UAPlotting5_RawPlot(dir+filenamenocut, dir+filenameMBUEWG,dir+filenameATLAS1, dir+filenameATLAS6, dir+filenameALICE ,  "nch_corrected_norm","nch_corrected_norm_" + mctypestr.str()+hf+Energy+tr+wght   );
  UAPlotting5_RawPlot(dir+filenamenocut, dir+filenameMBUEWG,dir+filenameATLAS1, dir+filenameATLAS6, dir+filenameALICE ,  "nch_MC_gen_afterUnfolding" ,"nch_MC_gen_afterUnfolding_" + mctypestr.str()+hf+Energy+tr+wght  );
  UAPlotting5_RawPlot(dir+filenamenocut, dir+filenameMBUEWG,dir+filenameATLAS1, dir+filenameATLAS6, dir+filenameALICE ,  "nch_MC_gen_afterEvtSelCorrection" , "nch_MC_gen_afterEvtSelCorrection_" + mctypestr.str()+hf+Energy+tr+wght );
}


//_______________________Compare different MCs during Unfolding__________________________________
void UnfRawCompare(TString type = "nocut", int acc = 0, TString hf = "HF0", TString subdir = "v25_900", TString MCcorrMC_name ="partfull" ) {
  //Example: UnfRawCompare("nocut",0,"HF0","v25","partfull")
  //MC:      UnfRawCompare("nocut",0,"HF0","v25","MCcorrMC_partfull")

  hf="_"+hf;
  TString Difftype = "_INEL";
  if(hf =="_HF1") Difftype = "_NSD";
  type= "_"+type;
  stringstream cutstr("");	  cutstr    << "_cut" << acc ;
  
  TString Energy = "_E_7";
  if (subdir.Contains("900") ) Energy = "_E_0.9";
  TString NoWght="";
  if(subdir.Contains("v21") && subdir.Contains("900") ) NoWght="noweight_"; //for old files
  if(subdir.Contains("NoWeight")) NoWght="noweight_";
  
  TString extra="";
  if (subdir.Contains("genTr") )   extra+="_genTr";
  if (subdir.Contains("NoWeight")) extra+="_noweight";
  
  TString dir="files/unfold_outputs/"+subdir+"/";
  TString filenameMC10  = "unf_MC10"+extra+"_"+MCcorrMC_name+hf+type +Difftype+cutstr.str()+".root";
  TString filenameMC15  = "unf_MC15"+extra+"_"+MCcorrMC_name+hf+type +Difftype+cutstr.str()+".root";
  TString filenameMC60  = "unf_MC60"+extra+"_"+MCcorrMC_name+hf+type +Difftype+cutstr.str()+".root";
  TString filenameMC31  = "unf_MC31"+extra+"_"+MCcorrMC_name+hf+type +Difftype+cutstr.str()+".root";
  //TString dir2="files/outputs_full/"+subdir+"/";
  TString dir2="files/outputs_full/v22";
  if(subdir.Contains("900") ) dir2+="_900";
  dir2 +="/";
  TString filenameMC61  = "output_MC61_GenOnly"+Energy+"_5000000.root";
  TString filenameMC62  = "output_MC62_GenOnly"+Energy+"_5000000.root";
  
   TString tr ="_ferncTr";
   if (subdir.Contains("genTr") ) tr= "_genTr";
   TString wght ="_Weight";
   if (subdir.Contains("NoWeight")) wght="_NoWeight";

  //UAPlotting4_UnfCompare   (dir+filenameMC10, dir+filenameMC15,dir+filenameMC60, dir+filenameMC31,  "nch_data_corrected" ,"nch_diffCorrMC_"+hf+type+cutstr.str()+Energy+tr+wght );
  //also PYTHIA 8 v2 and v4
  //UAPlotting5_UnfRawCompare(dir+filenameMC10, dir+filenameMC15,dir+filenameMC60, dir+filenameMC31, "nch_data_raw", "nch_mc_reco_raw" ,"nch_raw_"+hf+type+cutstr.str()+Energy+tr+wght   );
  UAPlotting7_UnfRawCompare(dir+filenameMC10, dir+filenameMC15,dir+filenameMC60, dir+filenameMC31, dir2+filenameMC61, dir2+filenameMC62,  "nch_data_corrected", "nch_MC_gen_afterEvtSelCorrection" , "nch"+hf+type+cutstr.str() ,"nch_corrected_"+type+hf+cutstr.str()+Energy+tr+wght	);
  //UAPlotting5_UnfRawCompare(dir+filenameMC10, dir+filenameMC15,dir+filenameMC60, dir+filenameMC31,  "nch_data_raw", "nch_mc_reco_raw" ,"nch_raw_"+hf+type+cutstr.str()+Energy+tr+wght   );
  //UAPlotting5_UnfRawCompare(dir+filenameMC10, dir+filenameMC15,dir+filenameMC60, dir+filenameMC31,  "nch_data_corrected", "nch_MC_gen_afterEvtSelCorrection" ,"nch_corrected_"+hf+type+cutstr.str()+Energy+tr+wght	);
  //UAPlotting4_UnfChiCompare(dir+filenameMC10, dir+filenameMC15,dir+filenameMC60, dir+filenameMC31, "chi2VSniter" ,"Chi²_"+hf+type+cutstr.str()+Energy+tr+wght   );  
  //UAPlotting8_UnfCompare   (dir+filenameMC10, dir+filenameMC15,dir+filenameMC60, dir+filenameMC31,  "nch_unfoldedPtr", "nch_MC_gen_afterUnfolding" ,"nch_afterUnfolding_"+hf+type+cutstr.str()+Energy+tr+wght   );
  //UAPlotting8_UnfCompare   (dir+filenameMC10, dir+filenameMC15,dir+filenameMC60, dir+filenameMC31,  "nch_data_corrected", "nch_MC_gen_afterEvtSelCorrection" ,"nch_afterCorrected_"+hf+type+cutstr.str()+Energy+tr+wght );
  
  //UAPlotting4_UnfChiCompare(dir+filenameMC10, dir+filenameMC15,dir+filenameMC60, dir+filenameMC31,  "nch_data_raw" ,"nch_data_raw compare"+hf+type+cutstr.str()+Energy+tr+wght   );  
  
}
void UnfRawCompare_All(TString version = "v25", int cut = 0){
//  UnfRawCompare_All()

    UnfRawCompare("nocut" ,cut ,"HF0", version, "partfull");
    UnfRawCompare("MBUEWG",cut ,"HF0", version, "partfull");
    UnfRawCompare("ALICE" ,cut ,"HF0", version, "partfull");     
    UnfRawCompare("ATLAS1",cut ,"HF0", version, "partfull");
    UnfRawCompare("ATLAS2",cut ,"HF0", version, "partfull");
    UnfRawCompare("ATLAS6",cut ,"HF0", version, "partfull");   

    UnfRawCompare("nocut" ,cut ,"HF1", version, "partfull");
    UnfRawCompare("MBUEWG",cut ,"HF1", version, "partfull");
    UnfRawCompare("ALICE" ,cut ,"HF1", version, "partfull");
    UnfRawCompare("ATLAS1",cut ,"HF1", version, "partfull");
    UnfRawCompare("ATLAS2",cut ,"HF1", version, "partfull");
    UnfRawCompare("ATLAS6",cut ,"HF1", version, "partfull"); 
   
}



//_______________________Compare Unfolding Steps _________________________________
void UnfoldingPlot(int mctype = 31, int acc = 0, TString central= "MBUEWG", TString hf = "HF0", TString subdir = "v25", TString name ="partfull", TString type="INEL", bool log = false) {
   //Example:  UnfoldingPlot(31,0,"MBUEWG","HF0","v25","partfull","INEL", false)
   //MC:       UnfoldingPlot(31,0,"nocut","HF0","v25","MCcorrMC_partfull","INEL", false)
   
   TString dir="files/unfold_outputs/"+subdir+"/";
   TString Difftype = type;
   stringstream mctypestr("");  mctypestr << "_MC" << mctype ;
   stringstream cutstr("");        cutstr << "_cut" << acc ;
   TString Energy="_E_7";
   if (subdir.Contains("900") ) Energy = "_E_0.9";
   TString tr ="_ferncTr";
   if (subdir.Contains("genTr") ) tr= "_genTr";
   TString wght ="_Weight";
   if (subdir.Contains("NoWeight")) wght="_NoWeight";

   TString extra="";
   if (subdir.Contains("genTr") )   extra+="_genTr";
   if (subdir.Contains("NoWeight")) extra+="_noweight";
   TString filename  = "unf"+mctypestr.str()+extra+"_"+name+"_"+hf+"_"+central+"_" +Difftype+cutstr.str()+".root";
   TString logStr = "";
   if(name.Contains("MCcorrMC")) logStr ="_MCcorrMC";
   if (log == true) logStr +="_logy";
  
  UAPlotting5_UnfoldingPlot(dir+filename, "nch_data_raw", "nch_unfoldedPtr", "nch_data_corrected", 
                     "nch_MC_gen_After_CentrEvtCorr", "nch_mc_reco_raw", hf+"_"+central+cutstr.str()+logStr+Energy+tr+wght, log  ); //last field is legend name
   //with afterUnfolding instead of After_CentrEvtCorr                     
//  UAPlotting5_UnfoldingPlot(dir+filename, "nch_data_raw", "nch_unfoldedPtr", "nch_data_corrected", 
//                     "nch_MC_gen_afterUnfolding", "nch_mc_reco_raw", hf+"_"+central+cutstr.str()+logStr+Energy+tr+wght   ); 
}

//__________________________Compare Unfolding Steps __________________________________
void UnfAll(int mctype = 31, int acc = 0, TString hf = "HF0", TString subdir = "v25", TString name ="partfull",TString type = "INEL",bool log = false) { //type ="NSD"
   //Example: UnfAll(31,0,"HF0","v25","partfull","INEL",false)
   //MC:      UnfAll(31,0,"HF0","v25","MCcorrMC_partfull","INEL",false)
   
   TString dir="files/unfold_outputs/"+subdir+"/";
   stringstream mctypestr("");        mctypestr << "_MC" << mctype ;
   stringstream cutstr("");        cutstr << "_cut" << acc ;
   //get all root files in the directory
   
   UnfoldingPlot(mctype,acc,"nocut" ,hf,subdir,name,type,log);
   UnfoldingPlot(mctype,acc,"MBUEWG",hf,subdir,name,type,log);
   UnfoldingPlot(mctype,acc,"ALICE" ,hf,subdir,name,type,log);
   UnfoldingPlot(mctype,acc,"ATLAS1",hf,subdir,name,type,log);
   UnfoldingPlot(mctype,acc,"ATLAS2",hf,subdir,name,type,log);
   UnfoldingPlot(mctype,acc,"ATLAS6",hf,subdir,name,type,log);
}


//______________________COMPARE TrackPlots Data, MC15, MC16____________________________________
void allPlots(int nevtmc, int nevtdata, int acc = 0, TString subdir = "v25", TString hf = "HF0", TString central = "MBUEWG", TString Difftype = "RECO", bool NoWeight = false) {
  //Example: 
  //  allPlots(5000000,342220,0,"v25", "HF0","MBUEWG","INEL")
  //900: allPlots(5000000,2250000,0,"v25_900", "HF0","MBUEWG","INEL")


  stringstream nevt_str("");	    nevt_str << "" << nevtmc  ;   
  stringstream nevt_dat("");	    nevt_dat << "" << nevtdata;    
  stringstream   cutstr("");	    cutstr   << "_cut" << acc  ;
  
  //energy
  TString Energy="7";
  if(subdir.Contains("900") ) Energy="0.9";    
  TString noweight="";
  if(NoWeight) noweight="_noweight";
  TString wght ="_Weight";
  if (subdir.Contains("NoWeight")) wght="_NoWeight";
  TString tr ="_ferncTr";
  if (subdir.Contains("genTr") ) tr= "_genTr";
   
  TString dir="files/outputs_full/"+subdir+"/"; 
  TString filenameMC10 ="output_MC10"+tr+"_E_"+Energy+"_"+nevt_str.str()+noweight+".root";
  TString filenameMC15 ="output_MC15"+tr+"_E_"+Energy+"_"+nevt_str.str()+noweight+".root";
  TString filenameMC60 ="output_MC60"+tr+"_E_"+Energy+"_"+nevt_str.str()+noweight+".root";
  TString filenameMC31 ="output_MC31"+tr+"_E_"+Energy+"_"+nevt_str.str()+noweight+".root";
  TString filenameData ="output_data"+tr+"_E_"+Energy+"_"+nevt_dat.str()+".root";	     
  
  TString plot = "_trp_full_"+hf+"_"+central+"_"+Difftype+cutstr.str();

  UAPlotting5_allPlots(dir+filenameData, dir+filenameMC10, dir+filenameMC15, dir+filenameMC60, dir+filenameMC31, "nch"  +plot  , "nch"+plot  ,   "nch"+plot+"_E_"+Energy+tr+wght );
  UAPlotting5_allPlots(dir+filenameData, dir+filenameMC10, dir+filenameMC15, dir+filenameMC60, dir+filenameMC31, "nch"  +plot  , "nch"+plot  ,   "nch"+plot+"_E_"+Energy+tr+wght+"_logy",true );
/*  UAPlotting5_allPlots(dir+filenameData, dir+filenameMC10, dir+filenameMC15, dir+filenameMC60, dir+filenameMC31, "eta"  +plot  , "eta"+plot  ,   "eta"+plot+"_E_"+Energy+tr+wght );    
  UAPlotting5_allPlots(dir+filenameData, dir+filenameMC10, dir+filenameMC15, dir+filenameMC60, dir+filenameMC31, "pt"	+plot  , "pt"+plot   ,    "pt"+plot+"_E_"+Energy+tr+wght );
  UAPlotting5_allPlots(dir+filenameData, dir+filenameMC10, dir+filenameMC15, dir+filenameMC60, dir+filenameMC31, "pt"	+plot  , "pt"+plot   ,    "pt"+plot+"_E_"+Energy+tr+wght+"_logy",true ,100);
  UAPlotting5_allPlots(dir+filenameData, dir+filenameMC10, dir+filenameMC15, dir+filenameMC60, dir+filenameMC31, "phi"  +plot  , "phi"+plot  ,   "phi"+plot+"_E_"+Energy+tr+wght );
  UAPlotting5_allPlots(dir+filenameData, dir+filenameMC10, dir+filenameMC15, dir+filenameMC60, dir+filenameMC31, "nhit" +plot  , "nhit"+plot ,  "nhit"+plot+"_E_"+Energy+tr+wght );
  UAPlotting5_allPlots(dir+filenameData, dir+filenameMC10, dir+filenameMC15, dir+filenameMC60, dir+filenameMC31, "chi2n"+plot  , "chi2n"+plot, "chi2n"+plot+"_E_"+Energy+tr+wght );
*/  
  //UAPlotting5_allPlots(dir+filenameData, dir+filenameMC10, dir+filenameMC15, dir+filenameMC60, dir+filenameMC31, "dzOsz"  +plot, "dzOsz"  +plot, "dzOsz"  plot+noweight+"_E_"+Energy+tr+wght );
  //UAPlotting5_allPlots(dir+filenameData, dir+filenameMC10, dir+filenameMC15, dir+filenameMC60, dir+filenameMC31, "dxyOsxy"+plot, "dxyOsxy"+plot, "dxyOsxy"plot+noweight+"_E_"+Energy+tr+wght );  
  //UAPlotting5_allPlots(dir+filenameData, dir+filenameMC10, dir+filenameMC15, dir+filenameMC60, dir+filenameMC31, "eptOpt" +plot, "eptOpt" +plot, "eptOpt" plot+noweight+"_E_"+Energy+tr+wght ); 
  //UAPlotting5_allPlots(dir+filenameData, dir+filenameMC10, dir+filenameMC15, dir+filenameMC60, dir+filenameMC31, "dz"     +plot, "dz"     +plot, "dz"     plot+noweight+"_E_"+Energy+tr+wght ); 
  //UAPlotting5_allPlots(dir+filenameData, dir+filenameMC10, dir+filenameMC15, dir+filenameMC60, dir+filenameMC31, "d0"     +plot, "d0"     +plot, "d0"     plot+noweight+"_E_"+Energy+tr+wght ); 
  //UAPlotting5_allPlots(dir+filenameData, dir+filenameMC10, dir+filenameMC15, dir+filenameMC60, dir+filenameMC31, "dxy"    +plot, "dxy"    +plot, "dxy"    plot+noweight+"_E_"+Energy+tr+wght ); 
  //UAPlotting5_allPlots(dir+filenameData, dir+filenameMC10, dir+filenameMC15, dir+filenameMC60, dir+filenameMC31, "x"      +plot, "x"      +plot, "x"      plot+noweight+"_E_"+Energy+tr+wght ); 
  //UAPlotting5_allPlots(dir+filenameData, dir+filenameMC10, dir+filenameMC15, dir+filenameMC60, dir+filenameMC31, "y"      +plot, "y"      +plot, "y"      plot+noweight+"_E_"+Energy+tr+wght ); 
//  UAPlotting5_allPlots(dir+filenameData, dir+filenameMC10, dir+filenameMC15, dir+filenameMC60, dir+filenameMC31, "z"	  +plot, "z"	  +plot, "z"+plot+noweight+"_E_"+Energy+tr+wght ); 
  //UAPlotting5_allPlots(dir+filenameData, dir+filenameMC10, dir+filenameMC15, dir+filenameMC60, dir+filenameMC31, "ntracks"+plot,"ntracks" +plot, "ntracks"+plot+noweight+"_E_"+Energy+tr+wght ); 
  //UAPlotting5_allPlots(dir+filenameData, dir+filenameMC10, dir+filenameMC15, dir+filenameMC60, dir+filenameMC31, "nvertex"+plot, "nvertex"+plot, "nvertex"+plot+noweight+"_E_"+Energy+tr+wght );
  
}



//_______________________Compare Unfolding Steps _________________________________
void allPlots_allTracks(int acc = 0, TString subdir = "v25", TString central="ALICE") {   // ONLY FOR nocut ALICE ATLAS6
   //Example:  allPlots_allTracks(0,"v25","ALICE")
   //MC:       allPlots_allTracks(0,"v25","ALICE")

   bool log=true;
   TString nevt="5000000";
   TString Energy="7";
   if (subdir.Contains("900")) Energy="0.9";
   TString wght ="_Weight";
   if (subdir.Contains("NoWeight")) wght="_NoWeight";
   TString tr ="_ferncTr";
   if (subdir.Contains("genTr") ) tr= "_genTr";
    
   TString hf="_HF0";
   TString dir="files/outputs_full/"+subdir+"/";
   stringstream cutstr("");        cutstr << "cut" << acc ;
   //get all root files in the directory

   TString filenameMC10  = "output_MC10"+tr+"_E_"+Energy+"_"+nevt+".root";
   TString filenameMC15  = "output_MC15"+tr+"_E_"+Energy+"_"+nevt+".root";
   TString filenameMC31  = "output_MC31"+tr+"_E_"+Energy+"_"+nevt+".root";
   TString filenameMC60  = "output_MC60"+tr+"_E_"+Energy+"_"+nevt+".root";
   
   TString nevt_str="342220";
   if(subdir.Contains("900")) nevt_str="2250000";
   TString filenameData= "output_data"+tr+"_E_"+Energy+"_"+nevt_str+".root";
   
   TString plot = "_"+central+cutstr.str();
   
   TString logStr = "";
   if (log == true) logStr ="_logy";
   UAPlotting5_allPlots(dir+filenameData, dir+filenameMC10, dir+filenameMC15, dir+filenameMC60, dir+filenameMC31, "dzOsz"  +plot, "dzOsz"  +plot, "dz0sz_"+hf+plot+logStr+"_E_"+Energy+tr+wght,log,1000);
   UAPlotting5_allPlots(dir+filenameData, dir+filenameMC10, dir+filenameMC15, dir+filenameMC60, dir+filenameMC31, "dxyOsxy"+plot, "dxyOsxy"+plot, "dxyOsxy"+hf+plot+logStr+"_E_"+Energy+tr+wght,log,1000);
   UAPlotting5_allPlots(dir+filenameData, dir+filenameMC10, dir+filenameMC15, dir+filenameMC60, dir+filenameMC31, "eptOpt" +plot, "eptOpt" +plot, "eptOpt"+hf+plot+logStr+"_E_"+Energy+tr+wght,log,10);
}

//_______________________Compare Unfolding Steps _________________________________
void allEffs(int acc = 0, TString subdir = "v25",  TString type="INEL") {
   //Example:  allEffs(0,"v25","INEL")
   //MC:       allEffs(0,"v25","INEL")
   
   TString nevt="5000000";
   TString Energy="7";
   TString hf="_HF0";
   TString Difftype=type;
   if (subdir.Contains("900")) Energy="0.9";
   TString wght ="_Weight";
   if (subdir.Contains("NoWeight")) wght="_NoWeight";
   TString tr ="_ferncTr";
   if (subdir.Contains("genTr") ) tr= "_genTr";
   
   TString dir="files/outputs_full/"+subdir+"/";
   stringstream cutstr("");        cutstr << "cut" << acc ;
   //get all root files in the directory

   //TString filenameMC10  = "effs_MC10"+tr+"_E_"+Energy+"_"+nevt+".root";
   //TString filenameMC15  = "effs_MC15"+tr+"_E_"+Energy+"_"+nevt+".root";
   TString filenameMC31  = "effs_MC31"+tr+"_E_"+Energy+"_"+nevt+"_allEff.root";
   //TString filenameMC60  = "effs_MC60"+tr+"_E_"+Energy+"_"+nevt+".root";
   
   TString plot = hf+"_"+Difftype+"_"+cutstr.str();
   TString plotnocut  = "_partfull"+hf+"_nocut_" +Difftype+"_"+cutstr.str();   
   TString plotMBUEWG = "_partfull"+hf+"_MBUEWG_"+Difftype+"_"+cutstr.str();
   TString plotALICE  = "_partfull"+hf+"_ALICE_" +Difftype+"_"+cutstr.str();
   TString plotATLAS1 = "_partfull"+hf+"_ATLAS1_"+Difftype+"_"+cutstr.str();
   TString plotATLAS2 = "_partfull"+hf+"_ATLAS2_"+Difftype+"_"+cutstr.str();
   TString plotATLAS6 = "_partfull"+hf+"_ATLAS6_"+Difftype+"_"+cutstr.str();
  
   UAPlotting6_allEffs(dir+filenameMC31, "eff_total"+plotnocut  , "eff_total"+plotMBUEWG , "eff_total"+plotALICE , "eff_total"+plotATLAS1 , "eff_total"+plotATLAS2  ,"eff_total"+plotATLAS6 , "eff_total"+plot+"_E_"+Energy+tr+wght);
   UAPlotting6_allEffs(dir+filenameMC31, "L1Sel"+plotnocut      , "L1Sel"+plotMBUEWG     , "L1Sel"+plotALICE     , "L1Sel"+plotATLAS1     , "L1Sel"+plotATLAS2      ,"L1Sel"+plotATLAS6     , "L1Sel"+plot+"_E_"+Energy+tr+wght);
   UAPlotting6_allEffs(dir+filenameMC31, "hfSel"+plotnocut      , "hfSel"+plotMBUEWG     , "hfSel" +plotALICE    , "hfSel"+plotATLAS1     , "hfSel" +plotATLAS2     ,"hfSel" +plotATLAS6    , "hfSel"+plot+"_E_"+Energy+tr+wght);
   UAPlotting6_allEffs(dir+filenameMC31, "vtxSel"+plotnocut     , "vtxSel"+plotMBUEWG    , "vtxSel"+plotALICE    , "vtxSel"+plotATLAS1    , "vtxSel"+plotATLAS2     ,"vtxSel"+plotATLAS6    , "vtxSel"+plot+"_E_"+Energy+tr+wght);
   UAPlotting6_allEffs(dir+filenameMC31, "vtxqualSel"+plotnocut , "vtxqualSel"+plotMBUEWG, "vtxqualSel"+plotALICE, "vtxqualSel"+plotATLAS1, "vtxqualSel"+plotATLAS2 ,"vtxqualSel"+plotATLAS6, "vtxqualSel"+plot+"_E_"+Energy+tr+wght);
   UAPlotting6_allEffs(dir+filenameMC31, "evtSel"+plotnocut     , "evtSel"+plotMBUEWG    , "evtSel"+plotALICE    , "evtSel"+plotATLAS1    , "evtSel"+plotATLAS2     ,"evtSel"+plotATLAS6    , "evtSel"+plot+"_E_"+Energy+tr+wght);
   UAPlotting6_allEffs(dir+filenameMC31, "centrSel"+plotnocut   , "centrSel"+plotMBUEWG  , "centrSel"+plotALICE  , "centrSel"+plotATLAS1  , "centrSel"+plotATLAS2   ,"centrSel"+plotATLAS6  , "centrSel"+plot+"_E_"+Energy+tr+wght); 
   UAPlotting6_allEffs(dir+filenameMC31, "eff_total"+plotnocut  , "eff_total"+plotMBUEWG , "eff_total"+plotALICE , "eff_total"+plotATLAS1 , "eff_total"+plotATLAS2  ,"eff_total"+plotATLAS6 , "eff_total"+plot+"_E_"+Energy+tr+wght);
}

//_______________________Compare Unfolding Steps _________________________________

void oneEff(int acc = 0, TString subdir = "v25",  TString type="INEL", TString hf= "_HF0") {
   //Example:  oneEff(0,"v25","INEL")
   //MC:       oneEff(0,"v25","INEL")
   
   TString nevt="5000000";
   TString Energy="7";
   if (subdir.Contains("900")) Energy="0.9";
   TString wght ="_Weight";
   if (subdir.Contains("NoWeight")) wght="_NoWeight";
   TString tr ="_ferncTr";
   if (subdir.Contains("genTr") ) tr= "_genTr";
   TString Difftype=type;
   
   TString dir="files/outputs_full/"+subdir+"/";
   stringstream cutstr("");        cutstr << "cut" << acc ;
   //get all root files in the directory

   //TString filenameMC10  = "effs_MC10"+tr+"_E_"+Energy+"_"+nevt+".root";
   //TString filenameMC15  = "effs_MC15"+tr+"_E_"+Energy+"_"+nevt+".root";
   TString filenameMC31  = "effs_MC31"+tr+"_E_"+Energy+"_"+nevt+"_allEff.root";
   //TString filenameMC60  = "effs_MC60"+tr+"_E_"+Energy+"_"+nevt+".root";
   
   TString plot = hf+"_"+Difftype+"_"+cutstr.str();
   TString plotnocut  = "_partfull"+hf+"_nocut_" +Difftype+"_"+cutstr.str();   
   TString plotMBUEWG = "_partfull"+hf+"_MBUEWG_"+Difftype+"_"+cutstr.str();
   TString plotALICE  = "_partfull"+hf+"_ALICE_" +Difftype+"_"+cutstr.str();
   TString plotATLAS1 = "_partfull"+hf+"_ATLAS1_"+Difftype+"_"+cutstr.str();
   TString plotATLAS2 = "_partfull"+hf+"_ATLAS2_"+Difftype+"_"+cutstr.str();
   TString plotATLAS6 = "_partfull"+hf+"_ATLAS6_"+Difftype+"_"+cutstr.str();
  
   UAPlotting6_oneEff(dir+filenameMC31, "L1Sel"  +plotnocut,  "vtxSel"  +plotnocut,"vtxqualSel"   +plotnocut,"evtSel"   +plotnocut,"centrSel"   +plotnocut,  "eff_total"  +plotnocut  , plotnocut+"_E_"+Energy+tr+wght);
   UAPlotting6_oneEff(dir+filenameMC31, "L1Sel"  +plotMBUEWG, "vtxSel"  +plotMBUEWG,"vtxqualSel"  +plotMBUEWG,"evtSel"  +plotMBUEWG,"centrSel"  +plotMBUEWG, "eff_total"  +plotMBUEWG ,plotMBUEWG+"_E_"+Energy+tr+wght);
   UAPlotting6_oneEff(dir+filenameMC31, "L1Sel"  +plotALICE,  "vtxSel"  +plotALICE,"vtxqualSel"   +plotALICE,"evtSel"   +plotALICE,"centrSel"   +plotALICE,  "eff_total"  +plotALICE  , plotALICE+"_E_"+Energy+tr+wght);
   UAPlotting6_oneEff(dir+filenameMC31, "L1Sel"  +plotATLAS1, "vtxSel"  +plotATLAS1,"vtxqualSel"  +plotATLAS1,"evtSel"  +plotATLAS1,"centrSel"  +plotATLAS1, "eff_total"  +plotATLAS1 ,plotATLAS1+"_E_"+Energy+tr+wght);
   UAPlotting6_oneEff(dir+filenameMC31, "L1Sel"  +plotATLAS2, "vtxSel"  +plotATLAS2,"vtxqualSel"  +plotATLAS2,"evtSel"  +plotATLAS2,"centrSel"  +plotATLAS2, "eff_total"  +plotATLAS2 ,plotATLAS2+"_E_"+Energy+tr+wght);
   UAPlotting6_oneEff(dir+filenameMC31, "L1Sel"  +plotATLAS6, "vtxSel"  +plotATLAS6,"vtxqualSel"  +plotATLAS6,"evtSel"  +plotATLAS6,"centrSel"  +plotATLAS6, "eff_total"  +plotATLAS6 ,plotATLAS6+"_E_"+Energy+tr+wght);
}


//_______________________Compare Unfolding Steps _________________________________
void ZeroBiasEff(int acc = 0, TString subdir = "v25", TString hfcentral="_HF0_ALICE") {   // ONLY FOR nocut ALICE ATLAS6
   //Example:  ZeroBiasEff(0,"v22","ALICE")
   //MC:       ZeroBiasEff(0,"v22","ALICE")
   TString Energy="7";
   TString nevt="5000000";
   
   TString wght ="_Weight";
   if (subdir.Contains("NoWeight")) wght="_NoWeight";
   TString tr ="_ferncTr";
   if (subdir.Contains("genTr") ) tr= "_genTr";
    

   TString dir="files/outputs_full/"+subdir+"/";
   stringstream cutstr("");        cutstr << "cut" << acc ;
   //get all root files in the directory

   TString filenameMC10    = "effs_MC10"+tr+"_E_"+Energy+"_"+nevt+"_allEffs.root";
   TString filenameMC15    = "effs_MC15"+tr+"_E_"+Energy+"_"+nevt+"_allEffs.root";
   TString filenameMC31    = "effs_MC31"+tr+"_E_"+Energy+"_"+nevt+"_allEffs.root";
   TString filenameMC60    = "effs_MC60"+tr+"_E_"+Energy+"_"+nevt+"_allEffs.root";
   TString filenameZeroBias= "effs_zerobias"+tr+"_E_"+Energy+"_482270.root";
   
   TString plot = hfcentral+"_RECO_"+cutstr.str();
   
 
   UAPlotting5_ZeroBiasEff(dir+filenameZeroBias, dir+filenameMC10, dir+filenameMC15, dir+filenameMC60, dir+filenameMC31, "eff_evtSel_RECO"  +plot, "eff_evtSel_RECO"  +plot, "eff_evtSel_RECO"+plot+"_E_"+Energy+tr+wght);
}


//_______________________Compare Unfolding Steps _________________________________
void ZeroBiasEff_All(int acc = 0, TString subdir = "v22") {   // ONLY FOR nocut ALICE ATLAS6
    
    ZeroBiasEff(acc , subdir , "_HF0_nocut");
    ZeroBiasEff(acc , subdir , "_HF0_MBUEWG");
    ZeroBiasEff(acc , subdir , "_HF0_ALICE");
    ZeroBiasEff(acc , subdir , "_HF0_ATLAS1");
    ZeroBiasEff(acc , subdir , "_HF0_ATLAS2");
    ZeroBiasEff(acc , subdir , "_HF0_ATLAS6");
    
}


//_______________________Compare Final Data With Other Experiments _________________________________

void comp_other_exp(TString file1 , TString histo , TString expdata, TString legendStr){
  cmsStyleRoot();

  UACanvas* canvas = new UACanvas();
  UALegend* legend = new UALegend();
  UACurve curve1 = UACurve(0,file1,histo);
  UACurve curve2 = UACurve(0,expdata,0);
  
  ((TH1F*) curve1.pCurve())->GetXaxis()->SetRangeUser(0, 120);
  if(expdata.Contains("900GeV") ) ((TH1F*) curve1.pCurve())->GetXaxis()->SetRangeUser(0, 65);
  curve1.markerColor = kRed;
  curve1.lineColor = kRed;
  curve1.Norm();
  //((TH1F*) curve1.pCurve())->GetYaxis()->SetRangeUser(1, ((TH1F*) curve1.pCurve())->GetYaxis()->GetXmax());
  //curve1.Draw("E1"); //no errors visible
  curve1.Draw("E1");
  legend->AddLegend(curve1,"CMS data");

  curve2.markerColor = kBlue ;
  curve2.markerStyle = kOpenStar ;
  curve2.lineColor = kBlue ;
  curve2.Draw("same");
  if(expdata.Contains("atlas"))
    legend->AddLegend(curve2,"ATLAS data");
  else if(expdata.Contains("alice"))
    legend->AddLegend(curve2,"ALICE data");
    
    
    
  cout << file1 << endl;
  file1.ReplaceAll("unfold","sys");
  file1.ReplaceAll("unf","sys");
  UACurve curve1_sys = UACurve(0,file1,"Syst_Summed/gsyst");
  curve1_sys.SetStyle(curve1);
  curve1_sys.SerialBinKiller();
  curve1_sys.Norm();
  curve1_sys.Draw("zsame");

  legend->BuildLegend();
  
  double energy = 7;
  if(expdata.Contains("900GeV") ) energy=0.9;
  canvas->AddText( Txt(file1,energy),.035);
  
  canvas->GetCanvas()->SetLogy(1);
  
  gPad->Update();
  gPad->WaitPrimitive();
  canvas->Save("comp_other_exp"+legendStr,PlotDirectory);
  delete canvas ;
}


void finalAll(TString subdir ="v25"){ // 
   TString dir="files/unfold_outputs/"+subdir+"/";
   
   TString wght ="_Weight";
   if (dir.Contains("NoWeight")) wght="_NoWeight";
   TString tr ="_ferncTr";
   if (dir.Contains("genTr") ) tr= "_genTr";

  TString extra="";
  if (dir.Contains("genTr") )   extra+="_genTr";
  if (dir.Contains("NoWeight")) extra+="_noweight";
  
  comp_other_exp(dir+"unf_MC31"+extra+"_partfull_HF0_ATLAS1_INEL_cut2.root" , "nch_data_corrected" , "../expdata/atlas_dsigdn_inelgt1_7000GeV_eta25_pt500.txt","_ATLAS1_E_7"+tr+wght);
  comp_other_exp(dir+"unf_MC31"+extra+"_partfull_HF0_ATLAS2_INEL_cut0.root" , "nch_data_corrected" , "../expdata/atlas_dsigdn_inelgt2_7000GeV_eta25_pt100.txt","_ATLAS2_E_7"+tr+wght);
  comp_other_exp(dir+"unf_MC31"+extra+"_partfull_HF0_ATLAS6_INEL_cut2.root" , "nch_data_corrected" , "../expdata/atlas_dsigdn_inelgt6_7000GeV_eta25_pt500.txt","_ATLAS6_E_7"+tr+wght);
  comp_other_exp(dir+"unf_MC31"+extra+"_partfull_HF0_ALICE_INEL_cut4.root"  , "nch_data_corrected" , "../expdata/alice_dsigdn_inelgt1_7000GeV_eta10_pt0.txt","_ALICE_E_7"+tr+wght);  
 
  dir="files/unfold_outputs/"+subdir+"_900/";
     
  wght ="_Weight";
  if (dir.Contains("NoWeight")) wght="_NoWeight";
  tr ="_ferncTr";
  if (dir.Contains("genTr") ) tr= "_genTr";
  
  //to make it compatible to everything before v21
  extra="";
  if(dir.Contains("genTr")) extra+="_genTr";
  if(dir.Contains("v21") ||dir.Contains("NoWeight")) extra+="_noweight";
     
  comp_other_exp(dir+"unf_MC60"+extra+"_partfull_HF0_ATLAS1_INEL_cut2.root" , "nch_data_corrected" , "../expdata/atlas_dsigdn_inelgt1_900GeV_eta25_pt500.txt","_ATLAS1_E_0.9"+tr+wght);
  comp_other_exp(dir+"unf_MC60"+extra+"_partfull_HF0_ATLAS2_INEL_cut0.root" , "nch_data_corrected" , "../expdata/atlas_dsigdn_inelgt2_900GeV_eta25_pt100.txt","_ATLAS2_E_0.9"+tr+wght);
  comp_other_exp(dir+"unf_MC60"+extra+"_partfull_HF0_ATLAS6_INEL_cut2.root" , "nch_data_corrected" , "../expdata/atlas_dsigdn_inelgt6_900GeV_eta25_pt500.txt","_ATLAS6_E_0.9"+tr+wght);

}



//_______________________Compare NSD With Paper Results _________________________________
void xcheck_NSD(TString file1 , TString file2 , TString histo1 ,  TString histo2 , TString figname ){
  cmsStyleRoot();

  UACanvas* canvas = new UACanvas();
  UALegend* legend = new UALegend();
  UACurve curve1 = UACurve(1,file1,histo1);
  UACurve curve2 = UACurve(0,file2,histo2);
  
  curve1.markerColor = kRed;
  curve1.lineColor = kRed;
  curve1.Norm();
  //((TH1F*) curve1.pCurve())->GetYaxis()->SetRangeUser(1, ((TH1F*) curve1.pCurve())->GetYaxis()->GetXmax());
  ((TH1F*) curve1.pCurve())->GetXaxis()->SetRangeUser(0, 120);
  curve1.Draw("E1"); //no errors visible
  legend->AddLegend(curve1,"current NSD");

  curve2.markerColor = kBlue ;
  curve2.lineColor = kBlue ;
  curve2.Norm();
  curve2.Draw("same");
  legend->AddLegend(curve2,"paper NSD");
    
    
  legend->BuildLegend();
  double energy=7;
  if(file2.Contains("0.9TeV")) energy=0.9;
  canvas->AddText( Txt(file1,energy),.22,.965,.035);

  gPad->Update();
  gPad->WaitPrimitive();
  canvas->Save(figname,PlotDirectory);
  delete canvas ;
}


void xcheck_NSD_All(TString subdir ="v25"){
//if HF1 then pt_corr is automatically done for versions >=v22

  TString partold="";
  if(subdir=="v21") partold="_ptcorr";
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

  //xcheck_NSD(dir1+"unf_MC31"+extra+"_partfull_HF1_nocut_NSD_cut0.root" , dir2+"unfolding_MC_ATLAS_7.0TeV_mbTr__hyp1_niter0_cut10_DataType0.root" , plotname1 , plotname2 , figname);
  xcheck_NSD(dir1+"unf_MC31"+extra+"_partfull_HF1_nocut_NSD_cut1.root" , dir2+"unfolding_MC_ATLAS_7.0TeV_mbTr__hyp1_niter0_cut5_DataType0.root" , plotname1 , plotname2 , figname+"_cut1");
  //xcheck_NSD(dir1+"unf_MC31"+extra+"_partfull_HF1_nocut_NSD_cut2.root" , dir2+"unfolding_MC_ATLAS_7.0TeV_mbTr__hyp1_niter0_cut15_DataType0.root" , plotname1 , plotname2 , figname);
  //xcheck_NSD(dir1+"unf_MC31"+extra+"_partfull_HF1_nocut_NSD_cut3.root" , dir2+"unfolding_MC_ATLAS_7.0TeV_mbTr__hyp1_niter0_cut20_DataType0.root" , plotname1 , plotname2 , figname);
  //xcheck_NSD(dir1+"unf_MC31"+extra+"_partfull_HF1_nocut_NSD_cut4.root" , dir2+"unfolding_MC_ATLAS_7.0TeV_mbTr__hyp1_niter0_cut13_DataType0.root" , plotname1 , plotname2 , figname);
  xcheck_NSD(dir1+"unf_MC31"+extra+"_partfull_HF1_nocut_NSD_cut5.root" , dir2+"unfolding_MC_ATLAS_7.0TeV_mbTr__hyp1_niter0_cut8_DataType0.root" , plotname1 , plotname2 , figname+"_cut5");
   
  
/*  dir1="files/unfold_outputs/"+subdir+"_900"+partold+"/";
    
  extra="";
  if (dir1.Contains("genTr") )   extra+="_genTr";
  if (dir1.Contains("NoWeight")) extra+="_noweight";
  figname = getFigStr("xcheck_NSD" , dir1);
  
  //xcheck_NSD(dir1+"unf_MC60"+extra+"_partfull_HF1_nocut_NSD_cut0.root" , dir2+"unfolding_MC_D6T_0.9TeV_mbTr__hyp1_niter0_cut10_DataType0.root" , plotname1 , plotname2 , figname);
  xcheck_NSD(dir1+"unf_MC60"+extra+"_partfull_HF1_nocut_NSD_cut1.root" , dir2+"unfolding_MC_D6T_0.9TeV_mbTr__hyp1_niter0_cut5_DataType0.root" , plotname1 , plotname2 , figname);
  //xcheck_NSD(dir1+"unf"+extra+"_MC60_partfull_HF1_nocut_NSD_cut2.root" , dir2+"unfolding_MC_D6T_0.9TeV_mbTr__hyp1_niter0_cut15_DataType0.root" , plotname1 , plotname2 , figname);
  //xcheck_NSD(dir1+"unf"+extra+"_MC60_partfull_HF1_nocut_NSD_cut3.root" , dir2+"unfolding_MC_D6T_0.9TeV_mbTr__hyp1_niter0_cut20_DataType0.root" , plotname1 , plotname2 , figname);
  //xcheck_NSD(dir1+"unf"+extra+"_MC60_partfull_HF1_nocut_NSD_cut4.root" , dir2+"unfolding_MC_D6T_0.9TeV_mbTr__hyp1_niter0_cut13_DataType0.root" , plotname1 , plotname2 , figname);
  xcheck_NSD(dir1+"unf"+extra+"_MC60_partfull_HF1_nocut_NSD_cut5.root" , dir2+"unfolding_MC_D6T_0.9TeV_mbTr__hyp1_niter0_cut8_DataType0.root" , plotname1 , plotname2 , figname);

 */ 
  //  ept  xCheck
 /* dir1="files/unfold_outputs/v22NoWeight/";
  dir2="files/unfold_outputs/v25NoWeight/";
  plotname1="nch_data_corrected";
  plotname2="nch_data_corrected";
  //plotname1="nch_mc_reco_raw";
  //plotname2="nch_mc_reco_raw";
  //plotname1="nch_data_raw";
  //plotname2="nch_data_raw";
  plotname1="nch_unfoldedPtr";
  plotname2="nch_unfoldedPtr";
  //f1=dir1+"output_data_ferncTr_E_7_1063195_NCHCode_336_132440-71_0.root";
  f1=dir1+"unf_MC31_noweight_partfull_HF1_nocut_NSD_cut2.root";
  f2=dir2+"unf_MC31_noweight_partfull_HF1_nocut_NSD_cut2.root";
  xcheck_NSD(f1 , f2 , plotname1 , plotname2 , figname);
*/
  
  //  RAW xCheck
  //dir1="../outputs_sten/";
/*  dir1="files/outputs_full/v22NoWeight_Romain/";
  plotname1="nch_mpreco_full_HF1_nocut_RECO_cut1";
  //f1=dir1+"output_data_ferncTr_E_7_1063195_NCHCode_336_132440-71_0.root";
  f1=dir1+"output_data_ferncTr_E_7_342220.root";
  f2=dir2+"unfolding_MC_ATLAS_7.0TeV_mbTr__hyp1_niter0_cut5_DataType0.root";
  xcheck_NSD(f1 , f2 , plotname1 , plotname2 , figname);

  plotname1="nch_mpreco_full_HF1_nocut_RECO_cut1";
  f1=dir1+"output_data_ferncTr_E_7_1063195_NCHCode_336_132440only_0.root";
  f2=dir2+"unfolding_MC_ATLAS_7.0TeV_mbTr__hyp1_niter0_cut5_DataType0.root";
  xcheck_NSD(f1 , f2 , plotname1 , plotname2 , figname);


  plotname1="nch_mpreco_full_HF1_nocut_RECO_cut5";
  f1=dir1+"output_data_ferncTr_E_7_1063195_NCHCode_336_132440-71_0.root";
  f2=dir2+"unfolding_MC_ATLAS_7.0TeV_mbTr__hyp1_niter0_cut8_DataType0.root";
  xcheck_NSD(f1 , f2 , plotname1 , plotname2 , figname);
*/

  //  MC xCheck
  
/*    //SD paper in the 2 files
  dir1="../plots/simpleanav10/";
  plotname1="/ptGen0_etaGen2.4_ptReco0.1_etaReco2.4/GenMultiPlots_evtSel_reco_cut5/MultiPlots_SD_evtSel_reco_cut5/nch_SD_evtSel_reco_cut5";
  plotname2="nch_MC_gen_SD";
  f1=dir1+"simpleAna_MC_ATLAS_7.0TeV_mbTr__.root";
  f2=dir2+"unfolding_MC_ATLAS_7.0TeV_mbTr__hyp1_niter0_cut5_DataType0.root";
  xcheck_NSD(f1 , f2 , plotname1 , plotname2 , figname);

  dir1="files/unfold_outputs/v22NoWeight_Romain/";
  dir2="../plots/simpleanav10/";
  plotname1="nch_SD_mc";
  plotname2="/ptGen0_etaGen2.4_ptReco0.1_etaReco2.4/GenMultiPlots_evtSel_reco_cut5/MultiPlots_SD_evtSel_reco_cut5/nch_SD_evtSel_reco_cut5";
  f1=dir1+"unf_MC60_noweight_partfull_HF1_nocut_NSD_cut1.root";
  f2=dir2+"simpleAna_MC_ATLAS_7.0TeV_mbTr__.root";
  xcheck_NSD(f1 , f2 , plotname1 , plotname2 , figname);

  dir1="files/outputs_full/v22NoWeight_Romain/";
  plotname1="nch_mp_partfull_HF1_nocut_SD_cut1";
  plotname2="nch_MC_gen_SD";
  f1=dir1+"output_MC60_ferncTr_E_7_2_noweight.root";
  f2=dir2+"unfolding_MC_ATLAS_7.0TeV_mbTr__hyp1_niter0_cut5_DataType0.root";
  xcheck_NSD(f1 , f2 , plotname1 , plotname2 , figname);

  dir1="files/outputs_full/v22NoWeight_Romain/";
  dir2="../plots/simpleanav10/";
  plotname1="nch_mp_partfull_HF1_nocut_SD_cut1";
  plotname2="/ptGen0_etaGen2.4_ptReco0.1_etaReco2.4/GenMultiPlots_evtSel_reco_cut5/MultiPlots_SD_evtSel_reco_cut5/nch_SD_evtSel_reco_cut5";
  f1=dir1+"output_MC60_ferncTr_E_7_2_noweight.root";
  f2=dir2+"simpleAna_MC_ATLAS_7.0TeV_mbTr__.root";
  xcheck_NSD(f1 , f2 , plotname1 , plotname2 , figname);
*/

// xCheck after SD sub.

 /* dir1="files/unfold_outputs/v22NoWeight_Romain/";
  plotname1="nch_data_raw";
  plotname2="nch_MC_gen_SD";
  f1=dir1+"output_MC60_ferncTr_E_7_2_noweight.root";
  f2=dir2+"unfolding_MC_ATLAS_7.0TeV_mbTr__hyp1_niter0_cut5_DataType0.root";
  xcheck_NSD(f1 , f2 , plotname1 , plotname2 , figname);
*/


/*  dir1="files/outputs_full/v21b/";
  dir2="../plots/simpleanav10/";
  plotname1="nch_partfull_HF1_nocut_RECO_cut1";
  plotname2="nch_INC_evtSel_cut5";
  f1=dir1+"output_MC31_ferncTr_E_7_5000000_noweight_allEffs.root";
  f2=dir2+"simpleAna_MC_ATLAS_7.0TeV_mbTr__.root";
  xcheck_NSD(f1 , f2 , plotname1 , plotname2 , figname);
*/

} 


void mtx(TString file = "files/unfold_outputs/v25/unf_MC31_partfull_HF0_nocut_INEL_cut0.root" , TString histo = "nch_matrix"){
  cmsStyleRoot();

  UACanvas* canvas = new UACanvas();
  UALegend* legend = new UALegend();
  UACurve curve = UACurve(0,file,histo);
  curve.Norm();
  gStyle->SetPalette(1);
  ((TH2F*) curve.pCurve())->GetXaxis()->SetRangeUser(0, 180);
  ((TH2F*) curve.pCurve())->GetYaxis()->SetRangeUser(0, 180);
  ((TH2F*) curve.pCurve())->GetXaxis()->SetRangeUser(0, 180);
  ((TH2F*) curve.pCurve())->GetYaxis()->SetRangeUser(0, 180);
  ((TH2F*) curve.pCurve())->SetTitle(";n_{A};n_{B}");
  curve.Draw("colz"); //no errors visible
  
  double energy=7;
  canvas->AddText( Txt(file,energy),.22,.965,.035);
  
//  gPad->SetRightMargin(0.15);
  
  gPad->Update();
  gPad->WaitPrimitive();
  canvas->Save("mtx",PlotDirectory);
  delete canvas ;
}



void unfsteps(TString file = "files/unfold_outputs/v25/unf_MC31_partfull_HF0_nocut_INEL_cut0.root"){
  cmsStyleRoot();

  UACanvas* canvas = new UACanvas();
  UALegend* legend = new UALegend();
  UACurve curve1 = UACurve(0,file,"nch_data_raw");
  UACurve curve2 = UACurve(0,file,"nch_unfoldedPtr");
  
  curve1.Norm();
  ((TH1F*) curve1.pCurve())->GetXaxis()->SetRangeUser(0, 120);
  curve1.Draw(""); //no errors visible
  
  double energy=7;
  canvas->AddText( Txt(file,energy),.22,.965,.035);
  
  legend->AddLegend(curve1,"raw");
  legend->BuildLegend();
  
//  gPad->SetRightMargin(0.15);
  
  gPad->Update();
  gPad->WaitPrimitive();
  canvas->Save("unfsteps_1",PlotDirectory);
  
  
  canvas = new UACanvas();
  
  
  curve1 = UACurve(1,file,"nch_data_raw");
  
  ((TH1F*) curve1.pCurve())->GetXaxis()->SetRangeUser(0, 120);
  curve1.lineStyle = 2;
  curve1.lineWidth  = 2;
  curve1.Norm();
  curve1.Draw("");
  curve2.Norm();
  curve2.markerColor = kRed;
  curve2.Draw("psame");
  
  legend = new UALegend();
  legend->AddLegend(curve1,"raw");
  legend->AddLegend(curve2,"unfolded");
  legend->BuildLegend();
  
  
  canvas->AddText( Txt(file,energy),.22,.965,.035);
  
  gPad->Update();
  gPad->WaitPrimitive();
  canvas->Save("unfsteps_2",PlotDirectory);
  
  
  delete canvas ;
}




void fitting(TString file , TString histo){
  cmsStyleRoot();

  UACanvas* canvas = new UACanvas();
  UALegend* legend = new UALegend();
  UACurve curve = UACurve(0,file,histo);
  
  curve.markerColor = kBlack;
  curve.lineColor = kBlack;
  curve.Norm();
  //((TH1F*) curve1.pCurve())->GetYaxis()->SetRangeUser(1, ((TH1F*) curve1.pCurve())->GetYaxis()->GetXmax());
  ((TH1F*) curve.pCurve())->GetXaxis()->SetRangeUser(0, 120);
  curve.Draw(""); //no errors visible
  legend->AddLegend(curve,"CMS data");

    
  cout << file << endl;
  file.ReplaceAll("unfold","sys");
  file.ReplaceAll("unf","sys");
  UACurve curve_sys = UACurve(0,file,"Syst_Summed/gsyst");
  curve_sys.SetStyle(curve);
  curve_sys.Norm();
  curve_sys.Draw("zsame");
  
  //TWO NBDs
  TF1* nbd = new TF1("nbd",nbdfunc2,1,120,6);
  nbd->SetParNames("alpha","nmean1","k1","nmean2","k2","scalefactor");
  nbd->SetParameters(0.72,12,3,12+15,7,1);
  //nbd->FixParameter(5,nch_corrected->Integral());
  nbd->SetParLimits(0,0,1);
  nbd->SetParLimits(1,0.1,40);
  nbd->SetParLimits(2,0.5,20);
  nbd->SetParLimits(3,30,90);
  nbd->SetParLimits(4,0.5,80);
  
  
  ((TH1F*) curve.pCurve())->Fit("nbd","R");
  nbd->SetRange(0,120);
  nbd->SetLineWidth(2);
  nbd->Draw("same");
  cout << "Chi2/ndof is : " << nbd->GetChisquare() / nbd->GetNDF() << endl;
  
  legend->BuildLegend(0.5,0.8);
  legend->GetLegend()->AddEntry(nbd,"fit (2x NBD)","l");
  legend->GetLegend()->Draw("same");
  
  double energy=7;
  canvas->AddText( Txt(file,energy),.22,.965,.035);
  canvas->GetCanvas()->SetLogy(1);
  
  gPad->Update();
  gPad->WaitPrimitive();
  canvas->Save("fit",PlotDirectory);
  delete canvas ;

}

void fittingAll(TString subdir="v25"){
  TString dir="files/unfold_outputs/"+subdir+"/";
  TString file=dir+"unf_MC31_partfull_HF0_nocut_INEL_cut0.root";
  
  fitting(file,"nch_data_corrected");
  
}
