currentdir = gDirectory;

if(syst==401 || syst==481){
  ostringstream ext("");
  ext<<"binningE"<<E;
  if(E==7.0) ext.str("");
  TString mcfiletmp = fileManager(2,31,7.0,iTr,0,0,ext.str(),filedir);
  TFile* mctmp = TFile::Open(mcfiletmp,"READ");
  if(mctmp==0){
    cout<<"!! WARNING : the following file is not present. You nead to create it. Exiting now !"<<endl;
    cout<<mcfiletmp<<endl;
    return;
  }
  matrixhist = (TH2F*) mctmp->Get(dir+st("MatrixPlots_evtSel",acc)+st("/nch_matrix_evtSel",acc));
  
  
  cout<<"++ SYSTEMATIC #"<<syst<<" ++"<<endl;
  cout<<"   ===> The MC matrix is now @ an energy of 7 TeV using atlas tune"<<endl;
  cout<<"   ===> taken from file "<<mcfiletmp<<endl;
  cout<<"++ END OF SYSTEMATIC ++"<<endl;
}

if(syst==402 || syst==482){
  ostringstream ext("");
  ext<<"binningE"<<E;
  if(E==7.0) ext.str("");
  TString mcfiletmp = fileManager(2,30,7.0,iTr,0,0,ext.str(),filedir);
  TFile* mctmp = TFile::Open(mcfiletmp,"READ");
  if(mctmp==0){
    cout<<"!! WARNING : the following file is not present. You nead to create it. Exiting now !"<<endl;
    cout<<mcfiletmp<<endl;
    return;
  }
  matrixhist = (TH2F*) mctmp->Get(dir+st("MatrixPlots_evtSel",acc)+st("/nch_matrix_evtSel",acc));
  
  
  cout<<"++ SYSTEMATIC #"<<syst<<" ++"<<endl;
  cout<<"   ===> The MC matrix is now @ an energy of 7 TeV using D6T tune"<<endl;
  cout<<"   ===> taken from file "<<mcfiletmp<<endl;
  cout<<"++ END OF SYSTEMATIC ++"<<endl;
}

if(syst==403 || syst==483){
  ostringstream ext("");
  ext<<"binningE"<<E;
  if(E==7.0) ext.str("");
  TString mcfiletmp = fileManager(2,20,7.0,iTr,0,0,ext.str(),filedir);
  TFile* mctmp = TFile::Open(mcfiletmp,"READ");
  if(mctmp==0){
    cout<<"!! WARNING : the following file is not present. You nead to create it. Exiting now !"<<endl;
    cout<<mcfiletmp<<endl;
    return;
  }
  matrixhist = (TH2F*) mctmp->Get(dir+st("MatrixPlots_evtSel",acc)+st("/nch_matrix_evtSel",acc));
  
  
  cout<<"++ SYSTEMATIC #"<<syst<<" ++"<<endl;
  cout<<"   ===> The MC matrix is now @ an energy of 7 TeV using phojet tune"<<endl;
  cout<<"   ===> taken from file "<<mcfiletmp<<endl;
  cout<<"++ END OF SYSTEMATIC ++"<<endl;
}

if(syst==410 || syst==411 || syst==412 || syst==413 || syst==420 || syst==430 || syst==431){
  
  TString mcfiletmp = fileManager(2,syst%400,Emc,iTr,0,0,addstr,filedir);
  TFile* mctmp = TFile::Open(mcfiletmp,"READ");
  if(mctmp==0){
    cout<<"!! WARNING : the following file is not present. You nead to create it. Exiting now !"<<endl;
    cout<<mcfiletmp<<endl;
    return;
  }
  matrixhist = (TH2F*) mctmp->Get(dir+st("MatrixPlots_evtSel",acc)+st("/nch_matrix_evtSel",acc));
  
  
  cout<<"++ SYSTEMATIC #"<<syst<<" ++"<<endl;
  cout<<"   ===> The MC matrix is now with the tune "<<syst%400<<endl;
  cout<<"   ===> taken from file "<<mcfiletmp<<endl;
  cout<<"++ END OF SYSTEMATIC ++"<<endl;
}

if(syst>=480 && syst<500){
  TString mcfiletmp = fileManager(3,typeMC,E,iTr,0,0,outstr.str());
  TFile* mctmp = TFile::Open(mcfiletmp,"READ");
  if(mctmp==0){
    cout<<"!! WARNING : the following file is not present. You nead to create it. Exiting now !"<<endl;
    cout<<mcfiletmp<<endl;
    return;
  }
  
  TH1F* unfoldedtmp = (TH1F*) mctmp->Get("unfolding/nch_data_corrected");
  multiplyByWidth(unfoldedtmp);
  unfoldedtmp->Scale(1./unfoldedtmp->Integral());

  TH1D* projX = (TH1D*) matrixhist->ProjectionX();

  for(int igen = 1 ; igen <= matrixhist->GetNbinsX() ; ++igen){
    double factor = 1;
    if(projX->GetBinContent(igen)!=0)
      factor = unfoldedtmp->GetBinContent(igen) / projX->GetBinContent(igen);

    for(int ireco = 1 ; ireco <= matrixhist->GetNbinsY() ; ++ireco){
      matrixhist->SetBinContent(igen , ireco ,  matrixhist->GetBinContent(igen , ireco) * factor );
    }
  }


  cout<<"++ SYSTEMATIC #"<<syst<<" ++"<<endl;
  cout<<"   ===> The MC matrix is now reweighted with unfolded data"<<endl;
  cout<<"   ===> taken from file "<<mcfiletmp<<endl;
  cout<<"++ END OF SYSTEMATIC ++"<<endl;

}

currentdir->cd();
  
