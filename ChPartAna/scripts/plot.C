void plot (TString dir , TString histo , bool logY = false , int iLegendPos = 0 )
{

  if ( dataSetId.size()==0 ) return;
   
  cout<<histo<<endl;
  TCanvas* c1 = new TCanvas("c1","c",200,10,500,500);
  c1->SetLeftMargin(0.17);
  c1->SetBottomMargin(0.10);
  c1->SetFillColor(0);
  c1->GetFrame()->SetFillColor(21);
  c1->GetFrame()->SetBorderSize(12);
  c1->SetGrid(0,0);

  if(logY) c1->SetLogy(true);

  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(kFALSE);

  Float_t hMax = 0 ;
  vector<TFile*>          fData ( dataSetId.size() , NULL );
  vector<TDirectoryFile*> dData ( dataSetId.size() , NULL );
  vector<TH1F*>           hData ( dataSetId.size() , NULL );  

  
  for(int iData = 0 ; iData < (signed) dataSetId.size() ; ++iData) { 
    // Get histo
    fData.at(iData) = new TFile(fileManager(globalFileType,dataSetId.at(iData),globalEnergy,0,0,0,ptcutstr),"READ");
    fData.at(iData)->cd();
    if ( dir == "none" )
    {
      hData.at(iData) = (TH1F*) fData.at(iData)->Get(histo);
    } else {
      dData.at(iData) = (TDirectoryFile*) fData.at(iData)->Get(dir);
      dData.at(iData)->cd();
      hData.at(iData) = (TH1F*) dData.at(iData)->Get(histo);
    }
    // Plot Style
    if ( ! dataSetIsMc.at(iData) ) {
      hData.at(iData)->SetMarkerColor(dataSetColor.at(iData));
      hData.at(iData)->SetMarkerStyle(dataSetStyle.at(iData)); 
    } else {
      hData.at(iData)->SetLineWidth(2);
      hData.at(iData)->SetLineColor(dataSetColor.at(iData));
      hData.at(iData)->SetLineStyle(dataSetStyle.at(iData));
    }
    // Normalisation
    if ( iData>0 && globalNorm ) {
      Float_t ndata = hData.at(0)->Integral(1, hData.at(0)->GetNbinsX() );
      Float_t nmoca = hData.at(iData)->Integral(1, hData.at(iData)->GetNbinsX() );
      hData.at(iData)->Scale(ndata/nmoca);
    }
    // hMax
    if ( hData.at(iData)->GetMaximum() > hMax ) hMax = hData.at(iData)->GetMaximum() ;
  }

  // Global Style
  hData.at(0)->GetYaxis()->SetTitleOffset(2);
  if(!logY) {
    hData.at(0)->SetMinimum(0);
    hData.at(0)->SetMaximum(hMax*1.1 );
  }

  // Plot
  TString opt;
  for(int iData = 0 ; iData < (signed) dataSetId.size() ; ++iData) {
    if ( dataSetIsMc.at(iData) )  opt  = "hist"; 
    else                          opt  = "e";
    if ( iData > 0 )              opt += "same";
    hData.at(iData)->Draw(opt);
  }
  for(int iData = 0 ; iData < (signed) dataSetId.size() ; ++iData) {
    if ( ! dataSetIsMc.at(iData) ) hData.at(iData)->Draw("esame"); 
  }

  // Legend

  TLegend *leg = new TLegend (xLegendMin[iLegendPos] ,
                              yLegendMax[iLegendPos] - yLegendWidth * (1+dataSetId.size()) ,
                              xLegendMin[iLegendPos] + xLegendWidth ,
                              yLegendMax[iLegendPos] );
  if ( LegendTitle != "NONE")  leg->SetHeader(LegendTitle);
  for(int iData = 0 ; iData < (signed) dataSetId.size() ; ++iData) {
    if ( dataSetIsMc.at(iData) )  opt  = "l"; 
    else                          opt  = "p"; 
    leg->AddEntry(hData.at(iData),dataSetLegend.at(iData),opt );
  }
  leg->SetBorderSize(0);
  leg->SetFillColor(0);
  leg->Draw();

  // 
  //TText* txt=new TText(.65, 0.96, "CMS 0.9 TeV");
  TText* txt=new TText(xGlobalLabel,yGlobalLabel,globalLabel);
  txt->SetNDC(kTRUE);
  txt->SetTextSize(0.04);
  txt->Draw();

  // Save Plot
  string basedir ( globalFigDir );
  stringstream out("");
  out<< globalEnergy << "Tev/";
  basedir += out.str();

  string sdir    (dir);
  string shisto  (histo);
  if(logY) shisto+="_logY";
  
  gPad->WaitPrimitive();
  
  bool save = globalSaveFig;
  if(save){
  {string fngif ("");
   fngif += basedir;
   fngif += "gif/";  
   fngif += sdir;
   fngif += "/";

  if (!gSystem->OpenDirectory(fngif.c_str())) gSystem->mkdir(fngif.c_str(),true);
  fngif += shisto;
  fngif += ".gif";
  c1->SaveAs(fngif.c_str(),"");}

  
  {string fngif ("");
   fngif += basedir;
   fngif += "eps/"; 
   fngif += sdir;
   fngif += "/";

  if (!gSystem->OpenDirectory(fngif.c_str())) gSystem->mkdir(fngif.c_str(),true);
  fngif += shisto;
  fngif += ".eps";
  c1->SaveAs(fngif.c_str(),"");}

  
  {string fngif ("");
   fngif += basedir;
   fngif += "root/";
   fngif += sdir;

  fngif += "/";
  if (!gSystem->OpenDirectory(fngif.c_str())) gSystem->mkdir(fngif.c_str(),true);
  fngif += shisto;
  fngif += ".root";
  c1->SaveAs(fngif.c_str(),"");}
  }

  for(int iData = 0 ; iData < (signed) dataSetId.size() ; ++iData) 
    fData.at(iData)->Close(); 

  //moca->Close();
  //data->Close();
  delete c1;

  // return 1;
}  
