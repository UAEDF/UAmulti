currentdir = gDirectory;

if(syst==401){
  if(E==0.9)
    Emc = 2.36;
  else
    cout<<"!! WARNING : can't do this systematic, don't have the right energy mc"<<endl;
  ostringstream ext("");
  ext<<E<<"binning";
  TString mcfiletmp = fileManager(2,10,Emc,iTr,0,0,ext.str());
  TFile* mctmp = TFile::Open(mcfiletmp,"READ");
  if(mctmp==0){
    cout<<"!! WARNING : the following file is not present. You nead to create it. Exiting now !"<<endl;
    cout<<mcfiletmp<<endl;
    return;
  }
  matrixhist = (TH2F*) mctmp->Get(dir+st("MatrixPlots_evtSel",acc)+st("/nch_matrix_evtSel",acc));
  
  
  cout<<"++ SYSTEMATIC #"<<syst<<" ++"<<endl;
  cout<<"   ===> The MC matrix is now @ an energy of "<<Emc<<" TeV"<<endl;
  cout<<"   ===> taken from file "<<mcfiletmp<<endl;
  cout<<"++ END OF SYSTEMATIC ++"<<endl;
}

if(syst==410 || syst==411 || syst==412 || syst==413){
  
  TString mcfiletmp = fileManager(2,syst%400,Emc,iTr,0,0,"");
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
  
