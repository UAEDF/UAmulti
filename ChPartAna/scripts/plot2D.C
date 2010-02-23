void plot2D (char dir[100] , char histo[200] , bool logY = false )
//void plot (char dir[60] , char histo[60] , char var[60] )
{
   char var[60] = "Dummy";
  cout<<histo<<endl;
  TCanvas* c1 = new TCanvas("c1","c",200,10,1000,500);
  c1->Divide(2,1);
  c1->SetLeftMargin(0.17);
  c1->SetBottomMargin(0.10);
  c1->SetFillColor(0);
  c1->GetFrame()->SetFillColor(21);
  c1->GetFrame()->SetBorderSize(12);
  c1->SetGrid(0,0);

  if(logY) c1->SetLogy(true);

  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(kFALSE);

  TFile *data = new TFile("../../../root/collectionPlotter_data_testv5.root","READ");

  //TFile *data = new TFile("../../../root/collectionPlotter_data_finalv2_900GeV.root","READ");
  //TFile *data = new TFile("../../../root/collectionPlotter_data_finalv2_2.36TeV.root","READ");


  //TFile *data = new TFile("../../../root/collectionPlotter_data_final_test.root","READ");
  //TFile *data = new TFile("generalTracks_OfflinePV_data.root","READ");
  //TFile *data = new TFile("generalTracks_OfflinePV_MC900Gev_D6T.root","READ");
  data->cd();

  if ( dir == "none" )
  {
    TH2F* hdata = data->Get(histo);

  } else { 

    TDirectoryFile *ddata = data->Get(dir);
    ddata->cd();
    TH2F* hdata = ddata->Get(histo);
  } 

  TFile *moca = new TFile("../../../root/collectionPlotter_MC_testv5.root","READ");

  //TFile *moca = new TFile("../../../root/collectionPlotter_MC_finalv2_900GeV.root","READ");
  //TFile *moca = new TFile("../../../root/collectionPlotter_MC_finalv2_2.36TeV.root","READ");

  //TFile *moca = new TFile("../../../root/collectionPlotter_MC_final_test.root","READ");
  //TFile *moca = new TFile("collectionPlotter_MC_EvtSel_Eff.root","READ");
  //TFile *moca = new TFile("generalTracks_OfflinePV_MC900Gev_D6T.root","READ");
  moca->cd();
  
  if ( dir == "none" )
  {
    TH2F* hmoca = moca->Get(histo);
  } else { 

    TDirectoryFile *dmoca = moca->Get(dir);
    dmoca->cd();
    TH2F* hmoca = dmoca->Get(histo);
  }

  hdata->SetMarkerStyle(20);
  hdata->GetYaxis()->SetTitleOffset(2);
  if(!logY) hdata->SetMinimum(0);
  //hdata->GetXaxis()->SetTitle(var);
  //hdata->GetYaxis()->SetTitle("N"); 

  hmoca->SetLineWidth(2);

//  Float_t ndata = hdata->GetSum();
//  Float_t nmoca = hmoca->GetSum();

  //Float_t ndata = hdata->Integral(1, hdata->GetNbinsX() );
  //Float_t nmoca = hmoca->Integral(1, hmoca->GetNbinsX() );


  //cout << ndata << " " << nmoca << " " << ndata/nmoca <<  endl;
  //hmoca->Scale(ndata/nmoca);
  
  Float_t Max, min;
  (hdata->GetYaxis()->GetXmin()<hmoca->GetYaxis()->GetXmin())? min=hdata->GetYaxis()->GetXmin():min=hmoca->GetYaxis()->GetXmin();
  (hdata->GetYaxis()->GetXmax()>hmoca->GetYaxis()->GetXmax())? Max=hdata->GetYaxis()->GetXmax():Max=hmoca->GetYaxis()->GetXmax();
    
  cout << min << endl; 
  cout << Max << endl; 
  //hdata->SetMaximum(Max);
  //hdata->SetMinimum(min);

   

/*
  Float_t mxdata =  hdata->GetMaximun();
  Float_t mxmoca =  hmoca->GetMaximun();
  cout << mxdata << " " << mxmoca << endl;
*/


  //TText* txt=new TText(.65, 0.90, "CMS 0.9 TeV");
  TText* txt=new TText(.65, 0.90, "CMS 2.36 TeV");
  txt->SetNDC(kTRUE);
  txt->SetTextSize(0.04);
  TText* tdata=new TText(.65, 0.85, "Data");
  tdata->SetNDC(kTRUE);
  tdata->SetTextSize(0.04);
  TText* tmoca=new TText(.65, 0.85, "PYTHIA D6T");
  tmoca->SetNDC(kTRUE);
  tmoca->SetTextSize(0.04);


  //c1->SetLogY(true);
  c1->cd(1);
  gPad->SetLeftMargin(0.17);
  gPad->SetBottomMargin(0.10);
  gPad->SetFillColor(0);
  gPad->GetFrame()->SetFillColor(21);
  gPad->GetFrame()->SetBorderSize(12);
  gPad->SetGrid(0,0);
  hdata->Draw("colz");
  txt->Draw();
  tdata->Draw(); 
 
  c1->cd(2);
  gPad->SetLeftMargin(0.17);
  gPad->SetBottomMargin(0.10);
  gPad->SetFillColor(0);
  gPad->GetFrame()->SetFillColor(21);
  gPad->GetFrame()->SetBorderSize(12);
  gPad->SetGrid(0,0);
  hmoca->Draw("colz"); 
  txt->Draw();
  tmoca->Draw();  

  // Legend
  TLegend *leg = new TLegend (.65,.90,.90,.99);
  leg->AddEntry(hdata,"Data","p" );
  leg->AddEntry(hmoca,"PYTHIA D6T","l" );
  leg->SetBorderSize(0);
  leg->SetFillColor(0);
  //leg->Draw();



  // Save Plot
  //string basedir ( "/user/rougny/UAPlots/" );
  //string basedir ( "/home/xjanssen/cms/MinBias/900GevPlots/");
  string basedir ( "/home/xjanssen/cms/MinBias/236GevPlots/");

  string sdir   (dir);
  string shisto (histo);
  if(logY) shisto+="_logY";
  
  gPad->WaitPrimitive();
  

  bool save = false;
  if(save){
  {
   string fngif ("");
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

  moca->Close();
  data->Close();

  // return 1;
}  
