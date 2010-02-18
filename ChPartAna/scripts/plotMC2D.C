void plotMC2D (char dir[100] , char histo[200] , bool logY = false )
//void plot (char dir[60] , char histo[60] , char var[60] )
{
   char var[60] = "Dummy";
  cout<<histo<<endl;
  TCanvas* c1 = new TCanvas("c1","c",200,10,500,500);
  //c1->Divide(2,1);
  c1->SetLeftMargin(0.17);
  c1->SetBottomMargin(0.10);
  c1->SetFillColor(0);
  c1->GetFrame()->SetFillColor(21);
  c1->GetFrame()->SetBorderSize(12);
  c1->SetGrid();

  if(logY) c1->SetLogy(true);

  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(kFALSE);

  TFile *moca = new TFile("../macro/collectionPlotter_MC_final_test.root","READ");
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

  hmoca->SetLineWidth(2);

  hmoca->Draw("colz"); 
  
  // Legend
  TLegend *leg = new TLegend (.65,.90,.90,.99);
  //leg->AddEntry(hdata,"Data","p" );
  leg->AddEntry(hmoca,"PYTHIA D6T","l" );
  leg->SetBorderSize(0);
  leg->SetFillColor(0);
  //leg->Draw();

  // Save Plot
  string basedir ( "/user/rougny/UAPlots/" );
  string sdir   (dir);
  sdir += basedir;
  string shisto (histo);
  if(logY) shisto+="_logY";
  
  gPad->WaitPrimitive();
  
  return;
  bool save = false;
  if(save){
  {string fngif ("gif/");
  fngif += sdir;
  fngif += "/";
  if (!gSystem->OpenDirectory(fngif.c_str())) gSystem->mkdir(fngif.c_str(),true);
  fngif += shisto;
  fngif += ".gif";
  c1->SaveAs(fngif.c_str(),"");}

  
  {string fngif ("eps/");
  fngif += sdir;
  fngif += "/";
  if (!gSystem->OpenDirectory(fngif.c_str())) gSystem->mkdir(fngif.c_str(),true);
  fngif += shisto;
  fngif += ".eps";
  c1->SaveAs(fngif.c_str(),"");}

  
  {string fngif ("root/");
  fngif += sdir;
  fngif += "/";
  if (!gSystem->OpenDirectory(fngif.c_str())) gSystem->mkdir(fngif.c_str(),true);
  fngif += shisto;
  fngif += ".root";
  c1->SaveAs(fngif.c_str(),"");}
  }

  moca->Close();

  // return 1;
}  
