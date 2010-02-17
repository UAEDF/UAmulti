void plot (char dir[60] , char histo[60]  )
//void plot (char dir[60] , char histo[60] , char var[60] )
{
   char var[60] = "Dummy";

  TCanvas* c1 = new TCanvas("c1","c",200,10,500,500);
  c1->SetLeftMargin(0.17);
  c1->SetBottomMargin(0.10);
  c1->SetFillColor(0);
  c1->GetFrame()->SetFillColor(21);
  c1->GetFrame()->SetBorderSize(12);
  c1->SetGrid();

  //c1->SetLogy(true);

  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(kFALSE);

  TFile *data = new TFile("collectionPlotter_data_everything.root","READ");
  //TFile *data = new TFile("generalTracks_OfflinePV_data.root","READ");
  //TFile *data = new TFile("generalTracks_OfflinePV_MC900Gev_D6T.root","READ");
  data->cd();
/*
  if ( dir == "none" )
  {
    TH1F* hdata = data->Get(histo);

  } else { 
*/
    TDirectoryFile *ddata = data->Get(dir);
    ddata->cd();
    TH1F* hdata = ddata->Get(histo);
//  } 

  TFile *moca = new TFile("collectionPlotter_MC_testMulti.root","READ");
  //TFile *moca = new TFile("collectionPlotter_MC_EvtSel_Eff.root","READ");
  //TFile *moca = new TFile("generalTracks_OfflinePV_MC900Gev_D6T.root","READ");
  moca->cd();
/* 
  if ( dir == "none" )
  {
    TH1F* hmoca = moca->Get(histo);
  } else { 
*/
    TDirectoryFile *dmoca = moca->Get(dir);
    dmoca->cd();
    TH1F* hmoca = dmoca->Get(histo);
//  }

  hdata->SetMarkerStyle(20);
  hdata->GetYaxis()->SetTitleOffset(2);
  //hdata->GetXaxis()->SetTitle(var);
  hdata->GetYaxis()->SetTitle("N"); 

  hmoca->SetLineWidth(2);

//  Float_t ndata = hdata->GetSum();
//  Float_t nmoca = hmoca->GetSum();

  Float_t ndata = hdata->Integral(1, hdata->GetNbinsX() );
  Float_t nmoca = hmoca->Integral(1, hmoca->GetNbinsX() );


  cout << ndata << " " << nmoca << " " << ndata/nmoca <<  endl;
  hmoca->Scale(ndata/nmoca);
  
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

  //c1->SetLogY(true);

  hdata->Draw("e");
  hmoca->Draw("samehist"); 

  // Legend
  TLegend *leg = new TLegend (.65,.90,.90,.99);
  leg->AddEntry(hdata,"Data","p" );
  leg->AddEntry(hmoca,"PYTHIA D6T","l" );
  leg->SetBorderSize(0);
  leg->SetFillColor(0);
  leg->Draw();

  // Save Plot

  char Filename[100];
  string shisto (histo);
  string fngif ("fig/");
  fngif += shisto;
  fngif += ".gif";

  for (int j=0 ; j <= fngif.size() ; j++)
     Filename[j] = fngif[j];

  c1->SaveAs(Filename,"");


  // return 1;
}  
