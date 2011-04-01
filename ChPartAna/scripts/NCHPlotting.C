
#include <TROOT.h>
#include <TH1D.h>

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

void NCHPlotting() {
    cout << " UAPlotting compiled "<< endl;
    PlotDirectory +="v20/";
    cout << " Plot directory set to: " << PlotDirectory << endl;
    
    UACurveStyleBase::g_markerStyle = kOpenCircle;
    UACurveStyleBase::g_lineWidth = 2;
    UACurveStyleBase::g_lineStyle = 1;
    
}

TString Txt(TString in, double E = 7) {
    
    TString out="";
    if(E==7)   out="CMS 7 TeV      ";
    if(E==0.9) out="CMS 900 GeV      ";
    
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
    
    if(in.Contains("HF1")) out += "HFon ";
        
    return out;
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
  
  curve2.markerColor = kGreen ;
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
  canvas->Save("UnfCompareModels_"+legendStr,PlotDirectory);
  
  gPad->WaitPrimitive();

  delete canvas ;  
}


////////////////////////////////////////////////////////
////////////////////////////////////////////////////////


//_______________________Compare different Central Cuts during Unfolding__________________________________
void RawPlot(int mctype = 15, int acc = 0, TString hf = "HF0", TString DataOrMC = "v10", TString name ="partfull" ) {
   //Example: RawPlot(15,0,"HF0","v12")
   //MC:      RawPlot(15,0,"HF0","v12","MCcorrMC_partfull")

   TString Difftype = "RECO";  //before v11  "INEL"
   stringstream mctypestr("");     mctypestr << "_MC" << mctype ;
   stringstream cutstr("");        cutstr << "_cut" << acc ;
   //get all root files in the directory

   TString filenamenocut  = "unf"+mctypestr.str()+"_"+name+"_"+hf+"_nocut_" +Difftype+cutstr.str()+".root";
   TString filenameMBUEWG = "unf"+mctypestr.str()+"_"+name+"_"+hf+"_MBUEWG_"+Difftype+cutstr.str()+".root";
   TString filenameATLAS1 = "unf"+mctypestr.str()+"_"+name+"_"+hf+"_ATLAS1_"+Difftype+cutstr.str()+".root";
   TString filenameATLAS6 = "unf"+mctypestr.str()+"_"+name+"_"+hf+"_ATLAS6_"+Difftype+cutstr.str()+".root";
   TString filenameALICE  = "unf"+mctypestr.str()+"_"+name+"_"+hf+"_ALICE_" +Difftype+cutstr.str()+".root";
   //cout << filenamenocut<<endl;
   //cout << "unf_MC15_partfull_HF0_ALICE_INEL_cut0.root" << endl;
  
  TString dir="files/unfold_outputs/"+DataOrMC+"/";
  UAPlotting5_RawPlot(dir+filenamenocut, dir+filenameMBUEWG,dir+filenameATLAS1, dir+filenameATLAS6, dir+filenameALICE ,  "nch_data_raw"  ,  "nch_data_raw_" + mctypestr.str()+hf );
  UAPlotting5_RawPlot(dir+filenamenocut, dir+filenameMBUEWG,dir+filenameATLAS1, dir+filenameATLAS6, dir+filenameALICE ,  "nch_data_corrected" ,  "nch_data_corrected_" + mctypestr.str()+hf  );
  UAPlotting5_RawPlot(dir+filenamenocut, dir+filenameMBUEWG,dir+filenameATLAS1, dir+filenameATLAS6, dir+filenameALICE ,  "nch_resampled", "nch_resampled_" + mctypestr.str()+hf );
  UAPlotting5_RawPlot(dir+filenamenocut, dir+filenameMBUEWG,dir+filenameATLAS1, dir+filenameATLAS6, dir+filenameALICE ,  "nch_unfoldedPtr" , "nch_unfoldedPtr_"+ mctypestr.str()+hf );
  UAPlotting5_RawPlot(dir+filenamenocut, dir+filenameMBUEWG,dir+filenameATLAS1, dir+filenameATLAS6, dir+filenameALICE ,  "nch_corrected_norm","nch_corrected_norm_" + mctypestr.str()+hf   );
  UAPlotting5_RawPlot(dir+filenamenocut, dir+filenameMBUEWG,dir+filenameATLAS1, dir+filenameATLAS6, dir+filenameALICE ,  "nch_MC_gen_afterUnfolding" ,"nch_MC_gen_afterUnfolding_" + mctypestr.str()+hf  );
  UAPlotting5_RawPlot(dir+filenamenocut, dir+filenameMBUEWG,dir+filenameATLAS1, dir+filenameATLAS6, dir+filenameALICE ,  "nch_MC_gen_afterEvtSelCorrection" , "nch_MC_gen_afterEvtSelCorrection_" + mctypestr.str()+hf );
}


//_______________________Compare different MCs during Unfolding__________________________________
void UnfRawCompare(TString type = "nocut", int acc = 0, TString hf = "HF0", TString subdir = "v20", TString MCcorrMC_name ="partfull" ) {
  //Example: UnfRawCompare("nocut",0,"HF0","v20","partfull")
  //MC:      UnfRawCompare("nocut",0,"HF0","v20","MCcorrMC_partfull")

  TString Difftype = "RECO";
  if(hf =="HF1") Difftype = "NSD";
  type= "_"+type+"_";
  stringstream cutstr("");	  cutstr    << "_cut" << acc ;

  TString filenameMC10  = "unf_MC10_"+MCcorrMC_name+"_"+hf+type +Difftype+cutstr.str()+".root";
  TString filenameMC15  = "unf_MC15_"+MCcorrMC_name+"_"+hf+type +Difftype+cutstr.str()+".root";
  TString filenameMC60  = "unf_MC60_"+MCcorrMC_name+"_"+hf+type +Difftype+cutstr.str()+".root";
  TString filenameMC31  = "unf_MC31_"+MCcorrMC_name+"_"+hf+type +Difftype+cutstr.str()+".root";

  TString dir="files/unfold_outputs/"+subdir+"/";
  UAPlotting4_UnfCompare   (dir+filenameMC10, dir+filenameMC15,dir+filenameMC60, dir+filenameMC31,  "nch_data_corrected" ,"nch_diffCorrMC_"+type+cutstr.str()+hf );
  UAPlotting5_UnfRawCompare(dir+filenameMC10, dir+filenameMC15,dir+filenameMC60, dir+filenameMC31,  "nch_data_raw", "nch_mc_reco_raw" ,"nch_raw_"+type+hf+cutstr.str()   );
  UAPlotting5_UnfRawCompare(dir+filenameMC10, dir+filenameMC15,dir+filenameMC60, dir+filenameMC31,  "nch_data_corrected", "nch_MC_gen_afterEvtSelCorrection" ,"nch_corrected_"+type+hf+cutstr.str()	);
  UAPlotting4_UnfChiCompare(dir+filenameMC10, dir+filenameMC15,dir+filenameMC60, dir+filenameMC31, "chi2VSniter" ,"Chi²_"+type+hf+cutstr.str()   );  
  UAPlotting8_UnfCompare   (dir+filenameMC10, dir+filenameMC15,dir+filenameMC60, dir+filenameMC31,  "nch_unfoldedPtr", "nch_MC_gen_afterUnfolding" ,"nch_afterUnfolding_"+type+cutstr.str()+hf   );
  UAPlotting8_UnfCompare   (dir+filenameMC10, dir+filenameMC15,dir+filenameMC60, dir+filenameMC31,  "nch_data_corrected", "nch_MC_gen_afterEvtSelCorrection" ,"nch_afterCorrected_"+type+cutstr.str()+hf );
  
  //UAPlotting4_UnfChiCompare(dir+filenameMC10, dir+filenameMC15,dir+filenameMC60, dir+filenameMC31,  "nch_data_raw" ,"nch_data_raw compare"   );  
  
}
void UnfRawCompare_All(TString version = "v20", int cut = 0){
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
void UnfoldingPlot(int mctype = 31, int acc = 0, TString central= "MBUEWG", TString hf = "HF0", TString DataOrMC = "v20", TString name ="partfull", TString type="INEL", bool log = false) {
   //Example:  UnfoldingPlot(31,0,"MBUEWG","HF0","v20","partfull","RECO", false)
   //MC:       UnfoldingPlot(31,0,"nocut","HF0","v20","MCcorrMC_partfull","RECO", false)
   
   TString dir="files/unfold_outputs/"+DataOrMC+"/";
   TString Difftype = type;
   stringstream mctypestr("");        mctypestr << "_MC" << mctype ;
   stringstream cutstr("");        cutstr << "_cut" << acc ;
   //get all root files in the directory

   TString filename  = "unf"+mctypestr.str()+"_"+name+"_"+hf+"_"+central+"_" +Difftype+cutstr.str()+".root";
   TString logStr = "";
   if(name.Contains("MCcorrMC")) logStr ="_MCcorrMC";
   if (log == true) logStr +="_logy";
  
  UAPlotting5_UnfoldingPlot(dir+filename, "nch_data_raw", "nch_unfoldedPtr", "nch_data_corrected", 
                     "nch_MC_gen_After_CentrEvtCorr", "nch_mc_reco_raw", hf+"_"+central+cutstr.str()+logStr, log  ); //last field is legend name
//  UAPlotting5_UnfoldingPlot(dir+filename, "nch_data_raw", "nch_unfoldedPtr", "nch_data_corrected", 
//                     "nch_MC_gen_afterUnfolding", "nch_mc_reco_raw", central   ); //last field is legend name
}

//__________________________Compare Unfolding Steps __________________________________
void UnfAll(int mctype = 31, int acc = 0, TString hf = "HF0", TString DataOrMC = "v20", TString name ="partfull",TString type = "RECO",bool log = false) { //type ="NSD"
   //Example: UnfAll(31,0,"HF0","v20","partfull","RECO",false)
   //MC:      UnfAll(31,0,"HF0","v20","MCcorrMC_partfull","RECO",false)
   
   TString dir="files/unfold_outputs/"+DataOrMC+"/";
   stringstream mctypestr("");        mctypestr << "_MC" << mctype ;
   stringstream cutstr("");        cutstr << "_cut" << acc ;
   //get all root files in the directory
   
   UnfoldingPlot(mctype,acc,"nocut" ,hf,DataOrMC,name,type,log);
   UnfoldingPlot(mctype,acc,"MBUEWG",hf,DataOrMC,name,type,log);
   UnfoldingPlot(mctype,acc,"ALICE" ,hf,DataOrMC,name,type,log);
   UnfoldingPlot(mctype,acc,"ATLAS1",hf,DataOrMC,name,type,log);
   UnfoldingPlot(mctype,acc,"ATLAS2",hf,DataOrMC,name,type,log);
   UnfoldingPlot(mctype,acc,"ATLAS6",hf,DataOrMC,name,type,log);
}


//______________________COMPARE TrackPlots Data, MC15, MC16____________________________________
void allPlots(int nevtmc, int nevtdata, int acc = 0, TString subdir = "v20NoWeight", TString hf = "HF0", 
              TString central = "MBUEWG", TString Difftype = "RECO", bool NoWeight = false) {
   //Example: 
  //  allPlots(5000000,342220,0,"v20", "HF0","MBUEWG","RECO")

      stringstream nevt_str("");        nevt_str << "" << nevtmc  ;   
      stringstream nevt_dat("");        nevt_dat << "" << nevtdata;    
      stringstream   cutstr("");        cutstr   << "_cut" << acc  ;
  //energy
    TString Energy="7";    
    TString noweight="";
    if(NoWeight) noweight="_noweight";
   
   TString dir="files/outputs_full/"+subdir+"/"; 
   TString filenameMC10 ="output_MC10_ferncTr_E_"+Energy+"_"+nevt_str.str()+noweight+".root";
   TString filenameMC15 ="output_MC15_ferncTr_E_"+Energy+"_"+nevt_str.str()+noweight+".root";
   TString filenameMC60 ="output_MC60_ferncTr_E_"+Energy+"_"+nevt_str.str()+noweight+".root";
   TString filenameMC31 ="output_MC31_ferncTr_E_"+Energy+"_"+nevt_str.str()+noweight+".root";
   TString filenameData ="output_data_ferncTr_E_"+Energy+"_"+nevt_dat.str()+".root";            
  
   TString plot = "_full_"+hf+"_"+central+"_"+Difftype+cutstr.str();

    UAPlotting5_allPlots(dir+filenameData, dir+filenameMC10, dir+filenameMC15, dir+filenameMC60, dir+filenameMC31, "nch"  +plot   , "nch"+plot,    "nch"+plot+noweight );
    UAPlotting5_allPlots(dir+filenameData, dir+filenameMC10, dir+filenameMC15, dir+filenameMC60, dir+filenameMC31, "nch"  +plot   , "nch"+plot,    "nch"+plot+noweight+"_logy",true );
    UAPlotting5_allPlots(dir+filenameData, dir+filenameMC10, dir+filenameMC15, dir+filenameMC60, dir+filenameMC31, "eta"  +plot  , "eta"+plot  ,   "eta"+plot+noweight );    
    UAPlotting5_allPlots(dir+filenameData, dir+filenameMC10, dir+filenameMC15, dir+filenameMC60, dir+filenameMC31, "pt"   +plot  , "pt"+plot   ,    "pt"+plot+noweight );
    UAPlotting5_allPlots(dir+filenameData, dir+filenameMC10, dir+filenameMC15, dir+filenameMC60, dir+filenameMC31, "pt"   +plot  , "pt"+plot   ,    "pt"+plot+noweight+"_logy",true ,100);
    UAPlotting5_allPlots(dir+filenameData, dir+filenameMC10, dir+filenameMC15, dir+filenameMC60, dir+filenameMC31, "phi"  +plot  , "phi"+plot  ,   "phi"+plot+noweight );
    UAPlotting5_allPlots(dir+filenameData, dir+filenameMC10, dir+filenameMC15, dir+filenameMC60, dir+filenameMC31, "nhit" +plot  , "nhit"+plot ,  "nhit"+plot+noweight );
    UAPlotting5_allPlots(dir+filenameData, dir+filenameMC10, dir+filenameMC15, dir+filenameMC60, dir+filenameMC31, "chi2n"+plot  , "chi2n"+plot, "chi2n"+plot+noweight );
    
    //UAPlotting5_allPlots(dir+filenameData, dir+filenameMC10, dir+filenameMC15, dir+filenameMC60, dir+filenameMC31, "dzOsz"  +plot, "dzOsz"  +plot, "dzOsz"  plot+noweight );
    //UAPlotting5_allPlots(dir+filenameData, dir+filenameMC10, dir+filenameMC15, dir+filenameMC60, dir+filenameMC31, "dxyOsxy"+plot, "dxyOsxy"+plot, "dxyOsxy"plot+noweight );  
    //UAPlotting5_allPlots(dir+filenameData, dir+filenameMC10, dir+filenameMC15, dir+filenameMC60, dir+filenameMC31, "eptOpt" +plot, "eptOpt" +plot, "eptOpt" plot+noweight ); 
    //UAPlotting5_allPlots(dir+filenameData, dir+filenameMC10, dir+filenameMC15, dir+filenameMC60, dir+filenameMC31, "dz"     +plot, "dz"     +plot, "dz"     plot+noweight ); 
    //UAPlotting5_allPlots(dir+filenameData, dir+filenameMC10, dir+filenameMC15, dir+filenameMC60, dir+filenameMC31, "d0"     +plot, "d0"     +plot, "d0"     plot+noweight ); 
    //UAPlotting5_allPlots(dir+filenameData, dir+filenameMC10, dir+filenameMC15, dir+filenameMC60, dir+filenameMC31, "dxy"    +plot, "dxy"    +plot, "dxy"    plot+noweight ); 
    //UAPlotting5_allPlots(dir+filenameData, dir+filenameMC10, dir+filenameMC15, dir+filenameMC60, dir+filenameMC31, "x"      +plot, "x"      +plot, "x"      plot+noweight ); 
    //UAPlotting5_allPlots(dir+filenameData, dir+filenameMC10, dir+filenameMC15, dir+filenameMC60, dir+filenameMC31, "y"      +plot, "y"      +plot, "y"      plot+noweight ); 
    UAPlotting5_allPlots(dir+filenameData, dir+filenameMC10, dir+filenameMC15, dir+filenameMC60, dir+filenameMC31, "z"      +plot, "z"      +plot, "z"+plot+noweight ); 
    //UAPlotting5_allPlots(dir+filenameData, dir+filenameMC10, dir+filenameMC15, dir+filenameMC60, dir+filenameMC31, "ntracks"+plot,"ntracks" +plot, "ntracks"+plot+noweight ); 
    //UAPlotting5_allPlots(dir+filenameData, dir+filenameMC10, dir+filenameMC15, dir+filenameMC60, dir+filenameMC31, "nvertex"+plot, "nvertex"+plot, "nvertex"+plot+noweight );
   
}



//_______________________Compare Unfolding Steps _________________________________
void allPlots_allTracks(int acc = 0, TString DataOrMC = "v20", TString central="ALICE") {   // ONLY FOR nocut ALICE ATLAS6
   //Example:  allPlots_allTracks(0,"v20","ALICE")
   //MC:       allPlots_allTracks(0,"v20","ALICE")
   bool log=true;
   TString nevt="5000000";
   TString E="7";
   TString hf="_HF0";
   TString dir="files/outputs_full/"+DataOrMC+"/";
   stringstream cutstr("");        cutstr << "cut" << acc ;
   //get all root files in the directory

   TString filenameMC10  = "output_MC10_ferncTr_E_"+E+"_"+nevt+".root";
   TString filenameMC15  = "output_MC15_ferncTr_E_"+E+"_"+nevt+".root";
   TString filenameMC31  = "output_MC31_ferncTr_E_"+E+"_"+nevt+".root";
   TString filenameMC60  = "output_MC60_ferncTr_E_"+E+"_"+nevt+".root";
   TString filenameData= "output_data_ferncTr_E_7_342220.root";
   
   TString plot = "_"+central+cutstr.str();
   
   TString logStr = "";
   if (log == true) logStr ="_logy";
   UAPlotting5_allPlots(dir+filenameData, dir+filenameMC10, dir+filenameMC15, dir+filenameMC60, dir+filenameMC31, "dzOsz"  +plot, "dzOsz"  +plot, "dz0sz_"+hf+plot+logStr ,log,1000);
   UAPlotting5_allPlots(dir+filenameData, dir+filenameMC10, dir+filenameMC15, dir+filenameMC60, dir+filenameMC31, "dxyOsxy"+plot, "dxyOsxy"+plot, "dxyOsxy"+hf+plot+logStr,log,1000);
   UAPlotting5_allPlots(dir+filenameData, dir+filenameMC10, dir+filenameMC15, dir+filenameMC60, dir+filenameMC31, "eptOpt" +plot, "eptOpt" +plot, "eptOpt"+hf+plot+logStr ,log,10);
}

//_______________________Compare Unfolding Steps _________________________________
void allEffs(int acc = 0, TString DataOrMC = "v20",  TString type="RECO") {
   //Example:  allEffs(0,"v20","RECO")
   //MC:       allEffs(0,"v20","RECO")
   
   TString nevt="5000000";
   TString E="7";
   TString hf="_HF0";
   TString Difftype=type;
   
   TString dir="files/outputs_full/"+DataOrMC+"/";
   stringstream cutstr("");        cutstr << "cut" << acc ;
   //get all root files in the directory

   //TString filenameMC10  = "effs_MC10_ferncTr_E_"+E+"_"+nevt+".root";
   //TString filenameMC15  = "effs_MC15_ferncTr_E_"+E+"_"+nevt+".root";
   TString filenameMC31  = "effs_MC31_ferncTr_E_"+E+"_"+nevt+"_allEff.root";
   //TString filenameMC60  = "effs_MC60_ferncTr_E_"+E+"_"+nevt+".root";
   
   TString plot = hf+"_"+Difftype+"_"+cutstr.str();
   TString plotnocut  = "_partfull"+hf+"_nocut_" +Difftype+"_"+cutstr.str();   
   TString plotMBUEWG = "_partfull"+hf+"_MBUEWG_"+Difftype+"_"+cutstr.str();
   TString plotALICE  = "_partfull"+hf+"_ALICE_" +Difftype+"_"+cutstr.str();
   TString plotATLAS1 = "_partfull"+hf+"_ATLAS1_"+Difftype+"_"+cutstr.str();
   TString plotATLAS2 = "_partfull"+hf+"_ATLAS2_"+Difftype+"_"+cutstr.str();
   TString plotATLAS6 = "_partfull"+hf+"_ATLAS6_"+Difftype+"_"+cutstr.str();
  
   UAPlotting6_allEffs(dir+filenameMC31, "eff_total"+plotnocut  , "eff_total"+plotMBUEWG , "eff_total"+plotALICE , "eff_total"+plotATLAS1 , "eff_total"+plotATLAS2  ,"eff_total"+plotATLAS6 , "eff_total"+plot);
   UAPlotting6_allEffs(dir+filenameMC31, "L1Sel"+plotnocut      , "L1Sel"+plotMBUEWG     , "L1Sel"+plotALICE     , "L1Sel"+plotATLAS1     , "L1Sel"+plotATLAS2      ,"L1Sel"+plotATLAS6     , "L1Sel"+plot);
   UAPlotting6_allEffs(dir+filenameMC31, "hfSel"+plotnocut      , "hfSel"+plotMBUEWG     , "hfSel" +plotALICE    , "hfSel"+plotATLAS1     , "hfSel" +plotATLAS2     ,"hfSel" +plotATLAS6    , "hfSel"+plot);
   UAPlotting6_allEffs(dir+filenameMC31, "vtxSel"+plotnocut     , "vtxSel"+plotMBUEWG    , "vtxSel"+plotALICE    , "vtxSel"+plotATLAS1    , "vtxSel"+plotATLAS2     ,"vtxSel"+plotATLAS6    , "vtxSel"+plot);
   UAPlotting6_allEffs(dir+filenameMC31, "vtxqualSel"+plotnocut , "vtxqualSel"+plotMBUEWG, "vtxqualSel"+plotALICE, "vtxqualSel"+plotATLAS1, "vtxqualSel"+plotATLAS2 ,"vtxqualSel"+plotATLAS6, "vtxqualSel"+plot);
   UAPlotting6_allEffs(dir+filenameMC31, "evtSel"+plotnocut     , "evtSel"+plotMBUEWG    , "evtSel"+plotALICE    , "evtSel"+plotATLAS1    , "evtSel"+plotATLAS2     ,"evtSel"+plotATLAS6    , "evtSel"+plot);
   UAPlotting6_allEffs(dir+filenameMC31, "centrSel"+plotnocut   , "centrSel"+plotMBUEWG  , "centrSel"+plotALICE  , "centrSel"+plotATLAS1  , "centrSel"+plotATLAS2   ,"centrSel"+plotATLAS6  , "centrSel"+plot); 
   UAPlotting6_allEffs(dir+filenameMC31, "eff_total"+plotnocut  , "eff_total"+plotMBUEWG , "eff_total"+plotALICE , "eff_total"+plotATLAS1 , "eff_total"+plotATLAS2  ,"eff_total"+plotATLAS6 , "eff_total"+plot);
}

//_______________________Compare Unfolding Steps _________________________________
void oneEff(int acc = 0, TString DataOrMC = "v20",  TString type="RECO", TString hf= "_HF0") {
   //Example:  oneEff(0,"v20","RECO")
   //MC:       oneEff(0,"v20","RECO")
   
   TString nevt="5000000";
   TString E="7";
   TString Difftype=type;
   
   TString dir="files/outputs_full/"+DataOrMC+"/";
   stringstream cutstr("");        cutstr << "cut" << acc ;
   //get all root files in the directory

   //TString filenameMC10  = "effs_MC10_ferncTr_E_"+E+"_"+nevt+".root";
   //TString filenameMC15  = "effs_MC15_ferncTr_E_"+E+"_"+nevt+".root";
   TString filenameMC31  = "effs_MC31_ferncTr_E_"+E+"_"+nevt+"_allEff.root";
   //TString filenameMC60  = "effs_MC60_ferncTr_E_"+E+"_"+nevt+".root";
   
   TString plot = hf+"_"+Difftype+"_"+cutstr.str();
   TString plotnocut  = "_partfull"+hf+"_nocut_" +Difftype+"_"+cutstr.str();   
   TString plotMBUEWG = "_partfull"+hf+"_MBUEWG_"+Difftype+"_"+cutstr.str();
   TString plotALICE  = "_partfull"+hf+"_ALICE_" +Difftype+"_"+cutstr.str();
   TString plotATLAS1 = "_partfull"+hf+"_ATLAS1_"+Difftype+"_"+cutstr.str();
   TString plotATLAS2 = "_partfull"+hf+"_ATLAS2_"+Difftype+"_"+cutstr.str();
   TString plotATLAS6 = "_partfull"+hf+"_ATLAS6_"+Difftype+"_"+cutstr.str();
  
   UAPlotting6_oneEff(dir+filenameMC31, "L1Sel"  +plotnocut,  "vtxSel"  +plotnocut,"vtxqualSel"   +plotnocut,"evtSel"   +plotnocut,"centrSel"   +plotnocut,  "eff_total"  +plotnocut  , plotnocut);
   UAPlotting6_oneEff(dir+filenameMC31, "L1Sel"  +plotMBUEWG, "vtxSel"  +plotMBUEWG,"vtxqualSel"  +plotMBUEWG,"evtSel"  +plotMBUEWG,"centrSel"  +plotMBUEWG, "eff_total"  +plotMBUEWG ,plotMBUEWG);
   UAPlotting6_oneEff(dir+filenameMC31, "L1Sel"  +plotALICE,  "vtxSel"  +plotALICE,"vtxqualSel"   +plotALICE,"evtSel"   +plotALICE,"centrSel"   +plotALICE,  "eff_total"  +plotALICE  , plotALICE);
   UAPlotting6_oneEff(dir+filenameMC31, "L1Sel"  +plotATLAS1, "vtxSel"  +plotATLAS1,"vtxqualSel"  +plotATLAS1,"evtSel"  +plotATLAS1,"centrSel"  +plotATLAS1, "eff_total"  +plotATLAS1 ,plotATLAS1);
   UAPlotting6_oneEff(dir+filenameMC31, "L1Sel"  +plotATLAS2, "vtxSel"  +plotATLAS2,"vtxqualSel"  +plotATLAS2,"evtSel"  +plotATLAS2,"centrSel"  +plotATLAS2, "eff_total"  +plotATLAS2 ,plotATLAS2);
   UAPlotting6_oneEff(dir+filenameMC31, "L1Sel"  +plotATLAS6, "vtxSel"  +plotATLAS6,"vtxqualSel"  +plotATLAS6,"evtSel"  +plotATLAS6,"centrSel"  +plotATLAS6, "eff_total"  +plotATLAS6 ,plotATLAS6);
}



//_______________________Compare Final Data With Other Experiments _________________________________

void comp_other_exp(TString file1 , TString histo , TString expdata, TString legendStr){
  cmsStyleRoot();

  UACanvas* canvas = new UACanvas();
  UALegend* legend = new UALegend();
  UACurve curve1 = UACurve(0,file1,histo);
  UACurve curve2 = UACurve(1,expdata,0);
  
  ((TH1F*) curve1.pCurve())->GetXaxis()->SetRangeUser(0, 120);
  curve1.markerColor = kRed;
  curve1.lineColor = kRed;
  curve1.Norm();
  //((TH1F*) curve1.pCurve())->GetYaxis()->SetRangeUser(1, ((TH1F*) curve1.pCurve())->GetYaxis()->GetXmax());
  curve1.Draw("E1"); //no errors visible
  legend->AddLegend(curve1,"CMS");

  curve2.markerColor = kBlue ;
  curve2.lineColor = kBlue ;
  curve2.Draw("same");
  if(expdata.Contains("atlas"))
    legend->AddLegend(curve2,"ATLAS");
  else if(expdata.Contains("alice"))
    legend->AddLegend(curve2,"ALICE");
    
    
    
  cout << file1 << endl;
  file1.ReplaceAll("unfold","sys");
  file1.ReplaceAll("unf","sys");
  UACurve curve1_sys = UACurve(0,file1,"Syst_Summed/gsyst");
  curve1_sys.SetStyle(curve1);
  curve1_sys.Norm();
  curve1_sys.Draw("zsame");

  legend->BuildLegend();
  canvas->AddText( Txt(file1),.22,.965,.035);

  gPad->Update();
  gPad->WaitPrimitive();
  canvas->Save("comp_other_exp"+legendStr,PlotDirectory);
  delete canvas ;
}

void finalAll(){
   TString dir="files/unfold_outputs/v20/";

  comp_other_exp(dir+"unf_MC31_partfull_HF0_ATLAS1_RECO_cut2.root" , "nch_data_corrected" , "../expdata/atlas_dsigdn_inelgt1_7000GeV_eta25_pt500.txt","ATLAS1");
  comp_other_exp(dir+"unf_MC31_partfull_HF0_ATLAS2_RECO_cut0.root" , "nch_data_corrected" , "../expdata/atlas_dsigdn_inelgt2_7000GeV_eta25_pt100.txt","ATLAS2");
  comp_other_exp(dir+"unf_MC31_partfull_HF0_ATLAS6_RECO_cut2.root" , "nch_data_corrected" , "../expdata/atlas_dsigdn_inelgt6_7000GeV_eta25_pt500.txt","ATLAS6");
  comp_other_exp(dir+"unf_MC31_partfull_HF0_ALICE_RECO_cut4.root"  , "nch_data_corrected" , "../expdata/alice_dsigdn_inelgt1_7000GeV_eta10_pt0.txt","ALICE");
}
