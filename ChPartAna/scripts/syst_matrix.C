currentdir = gDirectory;

if(syst==401){
  ostringstream ext("");
  ext<<"binningE"<<E;
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

if(syst==402){
  ostringstream ext("");
  ext<<"binningE"<<E;
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

if(syst==403){
  ostringstream ext("");
  ext<<"binningE"<<E;
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
  
  TString mcfiletmp = fileManager(2,syst%400,Emc,iTr,0,0,"",filedir);
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

currentdir->cd();
  
