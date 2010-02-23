void plotMC (TCanvas* c1, TH1F* h, char dir[100] , char histo[200] , int iter = 0 , bool logY = false)
//void plot (char dir[60] , char histo[60] , char var[60] )
{
  
  char var[60] = "Dummy";
  cout<<histo<<endl;
  if(iter == 0){
    //TCanvas* c1 = new TCanvas("c1","c",200,10,500,500);
    c1->SetLeftMargin(0.17);
    c1->SetBottomMargin(0.10);
    c1->SetFillColor(0);
    c1->GetFrame()->SetFillColor(21);
    c1->GetFrame()->SetBorderSize(12);
    c1->SetGrid(0,0);

    if(logY) c1->SetLogy(true);
 
    gStyle->SetOptStat(0);
    gStyle->SetOptTitle(kFALSE);

  }
  
  
  //TFile *moca = new TFile("../../../root/collectionPlotter_MC_finalv2_900GeV.root","READ");
  TFile *moca = new TFile("../../../root/collectionPlotter_MC_finalv2_2.36TeV.root","READ");
 

  //TFile *moca = new TFile("../../../root/collectionPlotter_MC_final_test.root","READ");
  //TFile *moca = new TFile("collectionPlotter_MC_EvtSel_Eff.root","READ");
  //TFile *moca = new TFile("generalTracks_OfflinePV_MC900Gev_D6T.root","READ");
  moca->cd();
  
  if ( dir == "none" )
  {
    TH1F* htmp = dmoca->Get(histo);
  } else { 

    TDirectoryFile *dmoca = moca->Get(dir);
    dmoca->cd();
    TH1F* htmp = dmoca->Get(histo);
  }
  h= htmp ;

  htmp->SetLineWidth(2);
  if(logY) c1->SetLogY(true);
  
  htmp->SetLineColor(iter+1);
  if(iter == 0) htmp->Draw("histe"); 
  else {
     htmp->Draw("esame");
  }

  // Legend
  TLegend *leg = new TLegend (.65,.90,.90,.99);
  //leg->AddEntry(hdata,"Data","p" );
  leg->AddEntry(htmp,"PYTHIA D6T","l" );
  leg->SetBorderSize(0);
  leg->SetFillColor(0);
  //leg->Draw();

  // 
  TText* txt=new TText(.65, 0.96, "CMS 2.36 TeV");
  //TText* txt=new TText(.65, 0.96, "CMS 0.9 TeV");
  txt->SetNDC(kTRUE);
  txt->SetTextSize(0.04);
  txt->Draw();


  // Save Plot
  //string basedir ( "/user/rougny/UAPlots/" );
  //string basedir ( "/home/xjanssen/cms/MinBias/900GevPlots/");
  string basedir ( "/home/xjanssen/cms/MinBias/236GevPlots/");
  string sdir   (dir);
  string shisto (histo);
  if(logY) shisto+="_logY";
  
  c1->Update();
  
  gPad->WaitPrimitive();
  
  bool save = true;
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
  
  if (!gSystem->OpenDirectory(fngif.c_str())) gSystem->mkdir(fngif.c_str(),true);
  fngif += shisto;
  fngif += ".eps";
  c1->SaveAs(fngif.c_str(),"");}

  
  {string fngif ("");
   fngif += basedir;
   fngif += "root/";
   fngif += sdir;
  
  if (!gSystem->OpenDirectory(fngif.c_str())) gSystem->mkdir(fngif.c_str(),true);
  fngif += shisto;
  fngif += ".root";
  c1->SaveAs(fngif.c_str(),"");}
  }

  moca->Close();

  // return 1;
}  
