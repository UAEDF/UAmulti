//get first bin with width!=0
  double maxfit = 50;
  for(int i = 1 ; i <= nch_corrected->GetNbinsX() ; ++i){
    if(nch_corrected->GetBinWidth(i)>1){
      maxfit = nch_corrected->GetXaxis()->GetBinLowEdge(i);
      break;
    }
  }
  //maxfit=60;
  cout<<"---------------------------------------------------------------"<<endl;
  cout<<"-------------------------   FITTING   -------------------------"<<endl;
  cout<<"---------------------------------------------------------------"<<endl<<endl;
  cout<<"Max range for fit : "<<maxfit<<endl;
  cout<<"Scaling nbd by : "<<nch_corrected->Integral()<<endl<<endl;
  
  // ONE NBD
  /*TF1* nbd = new TF1("nbd",nbdfunc,3,maxfit,3);
  nbd->SetParNames("nmean","k","scalefactor");
  nbd->FixParameter(3,nch_corrected->Integral());
  nbd->SetParameters(14,9);
  nbd->SetParLimits(0,3,35);
  nbd->SetParLimits(1,1,12);*/
  
  //TWO NBDs
  TF1* nbd = new TF1("nbd",nbdfunc2,1,maxfit,6);
  nbd->SetParNames("alpha","nmean1","k1","nmean2","k2","scalefactor");
  nbd->SetParameters(0.72,12,3,30,7,nch_corrected->Integral());
  //nbd->FixParameter(5,nch_corrected->Integral());
  nbd->SetParLimits(0,0,1);
  nbd->SetParLimits(1,3,35);
  nbd->SetParLimits(2,1,12);
  nbd->SetParLimits(3,10,70);
  nbd->SetParLimits(4,3,80);
  
  
  //TWO NBDs bis
  /*TF1* nbd = new TF1("nbd",nbdfunc2bis,1,maxfit,4);
  nbd->FixParameter(4,nch_corrected->Integral());
  nbd->SetParNames("alpha","nmean1","k1","scalefactor");
  nbd->SetParameters(0.72,12,2);
  nbd->SetParLimits(0,0.5,1);
  nbd->SetParLimits(1,3,35);
  nbd->SetParLimits(2,1,12);*/
  
  
  
  
  //THREE NBDs
  /*TF1* nbd = new TF1("nbd",nbdfunc3,1,maxfit,9);
  nbd->SetParNames("alpha1","nmean1","k1","alpha2","nmean2","k2","nmean3","k3","scalefactor");
  nbd->FixParameter(4,nch_corrected->Integral());
  nbd->SetParameters(0.70,5,10,0.25,24,10,50,20);
  nbd->SetParLimits(0,0.5,1);
  nbd->SetParLimits(1,3,35);
  nbd->SetParLimits(2,1,12);
  nbd->SetParLimits(3,0.1,0.5);
  nbd->SetParLimits(4,10,35);
  nbd->SetParLimits(5,3,80);
  nbd->SetParLimits(6,20,80);
  nbd->SetParLimits(7,5,80);*/
  
  
  //THREE NBDs bis
  /*TF1* nbd = new TF1("nbd",nbdfunc3bis,1,maxfit,5);
  nbd->FixParameter(5,nch_corrected->Integral());
  nbd->SetParNames("alpha","nmean1","k1","alpha2","scalefactor");
  nbd->SetParameters(0.72,12,2,0.15);
  nbd->SetParLimits(0,0.5,1);
  nbd->SetParLimits(1,3,35);
  nbd->SetParLimits(2,1,12);
  nbd->SetParLimits(3,0.01,0.5);*/
  
  
  
  nch_corrected->Fit("nbd","RO0");
  nbd->SetRange(0,80);
  nbd->Draw("same");
  
  TCanvas* c_fit = new TCanvas("c_fit","c_fit",1460,10,500,500);
  c_fit->SetLeftMargin(0.17);
  c_fit->SetBottomMargin(0.10);
  c_fit->SetFillColor(0);
  c_fit->GetFrame()->SetFillColor(21);
  c_fit->GetFrame()->SetBorderSize(12);
  c_fit->SetGrid();
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(kFALSE);
  
  c_fit->SetLogy(true);
  nbd->GetYaxis()->SetRangeUser(0.1,(nbd->GetMaximum()>nch_corrected->GetMaximum())? 1.02* nbd->GetMaximum():1.02* nch_corrected->GetMaximum());
  nbd->SetLineWidth(1);
  nbd->Draw();
  nch_corrected->Draw("same");
  
  
  TCanvas* c_sigmafit = new TCanvas("c_sigmafit","c_sigmafit");
  c_sigmafit->cd();
  
  TH1F* sigmaFit = (TH1F*) nch_corrected->Clone("sigmaFit");
  sigmaFit->Divide(nbd);
  sigmaFit->GetYaxis()->SetRangeUser(-5,5);
  sigmaFit->Draw();
  
  
  nbd->Write();
  sigmaFit->Write();  
  
  cout<<"Number of free parameters : "<<nbd->GetNumberFreeParameters()<<endl;
  cout<<"Normalized chi2 : "<<nbd->GetChisquare()/nbd->GetNDF()<<endl;
  
  gPad->WaitPrimitive();
  


  //Doing an iterative fix of the bin mean for width>2
  /*TGraphAsymmErrors g_nch_corr_1 = fixBinMean(h2g(nch_corrected),nbd);

  TCanvas* c_fit2 = new TCanvas("c_fit2","c_fit2");
  c_fit2->cd();
  c_fit2->SetLogy(true);
  //g_nch_corr_1.Fit("nbd","RO0");
  g_nch_corr_1.SetLineColor(kRed);
  g_nch_corr_1.SetMarkerColor(kRed);
  g_nch_corr_1.SetMarkerStyle(kDot);
  g_nch_corr_1.Draw("ap");
  nbd->Draw("same");


  gPad->WaitPrimitive();
*/





  
  
