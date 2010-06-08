currentdir = gDirectory;
  
if(syst!=100)
  syst_val = 0.;
  
if(int(syst/100)==1 && syst!=0 && syst_sign==0){
  cout<<"!! WARNING : your systematic sign should not be 0 ! Please select 1 or -1 ..."<<endl<<"Exiting now !"<<endl;
  return;
}

//ratio SD phojet frac/ current frac (here MCD6T)
if(syst == 100 && E==0.9)
  syst_val = fabs( 1 - 4.92 / (frac_SD * 100.) );
  
if(syst == 100 && E==2.36)
  syst_val = fabs( 1 - 5.0 / (frac_SD * 100.) );
  
if(syst==100){
  cout<<"++ SYSTEMATIC #"<<syst<<" ++"<<endl;
  cout<<"   ===> The SD distri has been scaled by a factor "<<1 + syst_sign * syst_val<<endl;
      
  cout<<"++ END OF SYSTEMATIC ++"<<endl;

  nch_evtSel_SD->Scale(1 + syst_sign * syst_val);

}


//--------------------------------------------
//Using the ratio D6T(7TeV) / Phojet(7TeV) on the SD distri
if(syst==101){

  //Done using ratioSDPhojetPythia code
  TFile* fratio = TFile::Open("ratio_phojet_pythia_7TeV.root");
  
  ostringstream rname("");
  rname << "ratio_cut" << acc;
  TH1F* ratio = (TH1F*) fratio->Get(rname.str().c_str());
  
  for(int ibin = 1 ; ibin <= nch_evtSel_SD->GetNbinsX() ; ++ibin){
    double binc = nch_evtSel_SD->GetBinContent(ibin);
    double errc = nch_evtSel_SD->GetBinError(ibin);
      
    if(binc!=0 && ratio->GetBinContent(ibin)!=0 && nch_evtSel_SD->GetBinWidth(ibin)==ratio->GetBinWidth(ibin)){
      nch_evtSel_SD->SetBinContent(ibin,binc * ratio->GetBinContent(ibin));
      nch_evtSel_SD->SetBinError(ibin,errc * ratio->GetBinContent(ibin));
    }
    else if(binc!=0 && ratio->GetBinContent(ibin)!=0 && nch_evtSel_SD->GetBinWidth(ibin)!=ratio->GetBinWidth(ibin)){
      cout<<"WARNING !! The binning starts to be different at bin "<<ibin<<". Keeping the SD bins from here."<<endl;
      break;
    }
  }

  cout<<"++ SYSTEMATIC #"<<syst<<" ++"<<endl;
  cout<<"   ===> The SD distri is now with the bin-per-bin ratio of phojet @ 7TeV "<<endl;
  cout<<"   ===> taken from file "<<"ratio_phojet_pythia_7TeV.root"<<endl;
  cout<<"++ END OF SYSTEMATIC ++"<<endl;
}


//--------------------------------------------
//Using the Phojet @ 7TeV on the SD distri
if(syst==102){
  ostringstream addstr1("");
  if(addstr.Contains("binning1")==0) {addstr1.str("");addstr1<<"binningE"<<E;if(E==7.0) addstr1.str("");}
  else addstr1.str("binning1");
  TString mcfiletmp = fileManager(2,20,7.0,iTr,0,0,addstr1.str(),filedir);
  mctmp = TFile::Open(mcfiletmp,"READ");
  //TFile* mctmp = new TFile(mcfiletmp,"READ");
  if(mctmp==0){
    cout<<"!! WARNING : the following file is not present. You nead to create it. Exiting now !"<<endl;
    cout<<mcfiletmp<<endl;
    //gROOT->ProcessLine(
    return;
  }

  nch_evtSel_SD =  (TH1F*) mctmp->Get(dir+st("GenMultiPlots_evtSel_reco",acc)+st("/MultiPlots_SD_evtSel_reco",acc)+st("/nch_SD_evtSel_reco",acc));
  MultiPlots* mp_evtSel_INC_tmp =  (MultiPlots*) mctmp->Get(dir+st("GenMultiPlots_evtSel_reco",acc)+st("/MultiPlots_INC_evtSel_reco",acc)+st("/multi_class_INC_evtSel_reco",acc));
  nch_evtSel_SD->Scale(mp_INC_evtSel_reco_data->nbEvts / mp_evtSel_INC_tmp->nbEvts);
  
  if(E==0.9) nch_evtSel_SD->Scale(4.9/5.);
  
  //nch_evtSel_SD = (TH1F*) nch_evtSel_SD->Clone(nch_evtSel_SD->GetName());
  
  cout<<"++ SYSTEMATIC #"<<syst<<" ++"<<endl;
  cout<<"   ===> The SD distri is now with the phojet tune at 7 TeV "<<syst%400<<endl;
  cout<<"   ===> taken from file "<<mcfiletmp<<endl;
  cout<<"++ END OF SYSTEMATIC ++"<<endl;
}



//--------------------------------------------
//Using the SD distri from another tune @ same energy
if(syst==110 || syst==111 || syst==112 || syst==113 || syst==130 || syst==131){
  
  TString mcfiletmp = fileManager(2,syst%100,Emc,iTr,0,0,"",filedir);
  mctmp = TFile::Open(mcfiletmp,"READ");
  //TFile* mctmp = new TFile(mcfiletmp,"READ");
  if(mctmp==0){
    cout<<"!! WARNING : the following file is not present. You nead to create it. Exiting now !"<<endl;
    cout<<mcfiletmp<<endl;
    //gROOT->ProcessLine(
    return;
  }

  nch_evtSel_SD =  (TH1F*) mctmp->Get(dir+st("GenMultiPlots_evtSel_reco",acc)+st("/MultiPlots_SD_evtSel_reco",acc)+st("/nch_SD_evtSel_reco",acc));
  MultiPlots* mp_evtSel_INC_tmp =  (MultiPlots*) mctmp->Get(dir+st("GenMultiPlots_evtSel_reco",acc)+st("/MultiPlots_INC_evtSel_reco",acc)+st("/multi_class_INC_evtSel_reco",acc));
  nch_evtSel_SD->Scale(mp_INC_evtSel_reco_data->nbEvts / mp_evtSel_INC_tmp->nbEvts);
  
  cout<<"++ SYSTEMATIC #"<<syst<<" ++"<<endl;
  cout<<"   ===> The SD distri is now with the tune "<<syst%400<<endl;
  cout<<"   ===> taken from file "<<mcfiletmp<<endl;
  cout<<"++ END OF SYSTEMATIC ++"<<endl;
}

currentdir->cd();
  
