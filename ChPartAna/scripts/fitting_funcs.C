//From websight
double NBD(double x, double nmean, double k){
  double p = 1. / ( (nmean / k) + 1 );
  //return Gamma(x+k)/( Gamma(x+1) * Gamma(k) ) * pow(p,k) * pow ( 1 - p , x);
  return Exp(LnGamma(x+k) - ( LnGamma(x+1) + LnGamma(k) )) * pow(p,k) * pow ( 1 - p , x);
}

//From paper
double NBD2(double x, double nmean, double k){
  return Gamma(k+x) / ( Gamma(x+1) * Gamma(k) ) * pow(nmean,x)*pow(k,k)/pow((nmean+k),x+k);
}

Double_t nbdfunc(Double_t* x, Double_t* par){
  return par[2] * NBD(x[0],par[0],par[1]);
}

Double_t nbdfunc2(Double_t* x, Double_t* par){
  return par[5] * (par[0]*NBD(x[0],par[1],par[2])+(1-par[0])*NBD(x[0],par[3],par[4]));
}

Double_t nbdfunc2bis(Double_t* x, Double_t* par){
  return par[3] * (par[0]*NBD(x[0],par[1],par[2])+(1-par[0])*NBD(x[0],2*par[1],2*par[2]));
}

Double_t nbdfunc3(Double_t* x, Double_t* par){
  return par[8] * (par[0]*NBD(x[0],par[1],par[2])+par[3]*NBD(x[0],par[4],par[5])+(1-par[0]-par[3])*NBD(x[0],par[6],par[7]));
}

Double_t nbdfunc3bis(Double_t* x, Double_t* par){
  return par[4] * (par[0]*NBD(x[0],par[1],par[2])+par[3]*NBD(x[0],2*par[1],2*par[2])+(1-par[0]-par[3])*NBD(x[0],3*par[1],3*par[2]));
}


Double_t xfunc(Double_t* x , Double_t* par){
  return  x[0] * nbdfunc2(x,par);
}

TGraphAsymmErrors fixBinMean(TGraphAsymmErrors g , TF1* nbd , Int_t iter = 0){
  
  ++iter;

  cout<<"------------- ITER "<<iter<<" --------------"<<endl;

  /*TF1* simplex = new TF1("simplex",xfunc,nbd->GetXmin(),nbd->GetXmax());
  simplex->FixParameter(0,1);*/
  TF1* xnbd = new TF1("xnbd",xfunc,nbd->GetXmin(),nbd->GetXmax(),6);
  xnbd->SetParameters(nbd->GetParameters());
  /*for(int k = 0;k<nbd->GetNumberFreeParameters();++k)
    xnbd->SetParameter(k,nbd->GetParameter(k));*/

  Double_t* x   = g.GetX();
  Double_t* y   = g.GetY();
  //Double_t* ex  = g.GetEX();
  Double_t* exl = g.GetEXlow();
  Double_t* exh = g.GetEXhigh();
  Double_t* eyl = g.GetEYlow();
  Double_t* eyh = g.GetEYhigh();
  Int_t npoints = g.GetN();
  
  Double_t* xx = new Double_t[1];
  xx[0] = 60;
  
  /*cout<<"eeeeee   "<<xnbd->Eval(60)<<"  "<<nbd->Eval(60)<<"  "<<xfunc(xx,nbd->GetParameters())<<endl;
  for(int k = 1;k<=nbd->GetNumberFreeParameters();++k)
    cout<<xnbd->GetParameter(k)<<"  "<<nbd->GetParameter(k)<<endl;*/

  for(int i = 0 ; i < npoints ; ++i){
    if(exl[i]+exh[i]>2){
      Double_t mean = xnbd->Integral(x[i]-exl[i] , x[i]+exh[i]) / nbd->Integral(x[i]-exl[i] , x[i]+exh[i]);
      //cout<<xnbd->Integral(x[i]-exl[i] , x[i]+exh[i])<<"  "<<nbd->Integral(x[i]-exl[i] , x[i]+exh[i])<<endl;
      //cout<<"The mean of bin "<<i+1<<" is "<<mean<<endl;
      Double_t ex = exl[i]+exh[i];
      
      if(mean>x[i]-exl[i] && mean<x[i]+exh[i]){
        exl[i] = mean - (x[i]-exl[i]);
	exh[i] = ex - exl[i];
	x[i]   = mean;
      }
      else if(mean<=x[i]-exl[i]){
        x[i]   = x[i]-exl[i];
	exl[i] = 0;
	exh[i] = ex;
      }
      else if(mean>=x[i]+exh[i]){
        x[i]   = x[i]+exh[i];
	exl[i] = ex;
	exh[i] = 0;
      }
    }
  }
  //making new graph
  TGraphAsymmErrors* ng = new TGraphAsymmErrors(npoints,x,y,exl,exh,eyl,eyh);
  
  double fitrange = 110.;
  for(int i = 0 ; i < npoints ; ++i)
    if(y[i]==0) fitrange = x[i] + exh[i] + 1.;
  nbd->SetRange(0.5 , fitrange);
  
  ng->Fit("nbd","RO0");
  ostringstream name("");
  name<<"nch_data_corrected_binmoving_iter"<<iter;
  ng->SetName(name.str().c_str());
  ng->Write();
  
  cout<<"Normalized chi2 : "<<nbd->GetChisquare()/nbd->GetNDF()<<endl;
  
  if(iter>3)    
    return g;
  
  return fixBinMean((*ng),nbd,iter);
}

TGraphAsymmErrors h2g(TH1F* h){
  Double_t* x   = new Double_t[h->GetNbinsX()];
  Double_t* y   = new Double_t[h->GetNbinsX()];
  Double_t* exl = new Double_t[h->GetNbinsX()];
  Double_t* exh = new Double_t[h->GetNbinsX()];
  Double_t* eyl = new Double_t[h->GetNbinsX()];
  Double_t* eyh = new Double_t[h->GetNbinsX()];
  
  for(int i = 1 ; i <= h->GetNbinsX() ; ++i){
    x[i-1] = h->GetBinCenter(i);
    y[i-1] = h->GetBinContent(i);
    exl[i-1] = h->GetBinCenter(i) - h->GetXaxis()->GetBinLowEdge(i);
    exh[i-1] = h->GetXaxis()->GetBinUpEdge(i) - h->GetBinCenter(i);
    eyl[i-1] = eyh[i-1] = h->GetBinError(i);
  }
  TGraphAsymmErrors* ng = new TGraphAsymmErrors(h->GetNbinsX(),x,y,exl,exh,eyl,eyh);
  ng->SetName(TString("g")+h->GetName());
  ng->SetTitle(h->GetTitle());
  return *ng;
}





