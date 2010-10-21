#include "TTProfile.h"

ClassImp(TTProfile)

using namespace std;

Bool_t TTProfile::debug            = false;
Bool_t TTProfile::switchTH1On      = true;
Bool_t TTProfile::switchTH2On      = false;
Bool_t TTProfile::switchTMomentsOn = false;

/*TTProfile::TTProfile():name("none"){
  cout << "GGGGGGGGG"<<endl;
  binning = this->makeBins(301,0,300);
  this->Init();
}*/
TTProfile::TTProfile(){
  if(debug) cout << "+ [DEBUG] TTProfile : Starting TTProfile() {" << name << "}" << endl;
  binning = vector<Double_t>();
  nBins = 0;
  name = "";
  profileDone = 0;
  vth1 = vector<TH1F>();
  vmean = vector<TMean>();
  vmom = vector<TMoments>();
  tprofile = TProfile();
  profile = TH1F();
  th2 = TH2F();
  binning = vector<Double_t>();
  vbinning = vector< vector<Double_t> >();  
  if(debug) cout << "- [DEBUG] TTProfile : Finished TTProfile() {" << name << "}" << endl;
  
  //this->Init();
}

TTProfile::TTProfile(TString s){
  if(debug) cout << "+ [DEBUG] TTProfile : Starting TTProfile(TString) {" << name << "}" << endl;
  binning = this->makeBins(101,-0.5,100.5);
  name = s;
  this->Init();
  if(debug) cout << "- [DEBUG] TTProfile : Finished TTProfile(TString) {" << name << "}" << endl;
}

TTProfile::TTProfile(TString s, Int_t nbin , Double_t xmin , Double_t xmax):name(s){
  binning = this->makeBins(nbin , xmin , xmax);
  this->Init();
}

TTProfile::TTProfile(TString s, vector<Double_t> v):name(s){
  binning = v;
  this->Init();
}

TTProfile::TTProfile(TString s, Int_t nbin1, Double_t xmin1, Double_t xmax1, Int_t nbin2, Double_t xmin2, Double_t xmax2):name(s){
  binning = this->makeBins(nbin1 , xmin1 , xmax1);
  vector<Double_t> tmp = this->makeBins(nbin2 , xmin2 , xmax2);
  vbinning = vector< vector<Double_t> >(binning.size() - 1 , tmp);
  this->Init();
}

TTProfile::TTProfile(TString s, vector<Double_t> v , vector<Double_t> w):name(s){
  binning = v;
  vbinning = vector< vector<Double_t> >(binning.size() - 1 , w);
  this->Init();
}

void TTProfile::Init(){
  if(debug) cout << "+ [DEBUG] TTProfile : Starting Init() {" << name << "}" << endl;
  nBins = binning.size() - 1;
  nEvents = 0;
  profileDone = false;
  if(vbinning.size()==0 && (switchTH1On || switchTH2On) ){
    vector<Double_t> tmp = this->makeBins(101 , -0.5 , 100.5);
    vbinning = vector< vector<Double_t> >(nBins , tmp);
  }
  
  vmean = vector<TMean>(nBins,TMean());
  if(switchTMomentsOn) vmom  = vector<TMoments>(nBins,TMoments());
  if(switchTH2On)      th2   = TH2F(TString("th2_")+name,TString("th2_")+name,nBins,&(binning[0]),vbinning[0].size()-1,&(vbinning[0][0]));
  tprofile = TProfile(TString("tprofile_")+name,TString("tprofile_")+name,nBins,&(binning[0]));
  profile  = TH1F(TString("profile_")+name,TString("profile_")+name,nBins,&(binning[0]));
  nEntries = TH1F(TString("nEntries_")+name,TString("nEntries_")+name,nBins,&(binning[0]));
  if(switchTH1On){
    for(int i = 0 ; i < nBins ; ++i){
      ostringstream str("distri_");
      str<<name<<"_bin"<<i;
      vth1.push_back(TH1F(str.str().c_str(),str.str().c_str(),vbinning[i].size()-1,&(vbinning[i][0])));
    }
  }
  if(debug) cout << "- [DEBUG] TTProfile : Finished Init() {" << name << "}" << endl;
}

void TTProfile::clear(){
  vth1.clear();
  vmean.clear();
  vmom.clear();
  vth1.clear();
  vbinning.clear();
  
  //this->Init();
}

vector<double> TTProfile::makeBins(Int_t nbin , Double_t xmin , Double_t xmax){
  vector<double> array(nbin+1,0);
  Double_t step = fabs( xmax - xmin ) / double (nbin);
  for(int i=0 ; i<=nbin ; ++i)
    array[i] = xmin + i * step;
  return array;
}


void TTProfile::Fill(Double_t xval , Double_t yval , Double_t weight){
  int i = 0;
  nEvents++;
  nEntries.Fill(xval , weight);
  for(;i<nBins;++i)
    if(xval>=binning.at(i) && xval<binning.at(i+1)) break;
  if(i<nBins){
    if(switchTH1On)      vth1.at(i).Fill(yval,weight);
    if(switchTH2On)      th2.Fill(xval,yval,weight);
                         vmean.at(i).Add(yval,weight);
    if(switchTMomentsOn) vmom.at(i).Add(yval,weight);
                         tprofile.Fill(xval,yval,weight);
  }
  /*else{
    cout << xval << " is outside the TTProfile range ..." << endl;
  }*/
  
  
}

void TTProfile::makeProfile(TString opt){
  for(int i=0 ; i<nBins ; ++i){
    if(vmean.at(i).GetRMS()<0) continue;

    profile.SetBinContent(i+1,vmean.at(i).GetMean());
    
    if(opt.Contains("RMS"))
      profile.SetBinError(i+1,vmean.at(i).GetRMS());
  
  }
  
  if(!profileDone) profileDone = true;
}

void TTProfile::write(Bool_t writeAllBins , Bool_t writeClass){
  gDirectory->mkdir("TTProfile_"+name);
  gDirectory->cd("TTProfile_"+name);
  
  if(!profileDone) makeProfile();

  if(writeAllBins){
    for(int i=0 ; i<nBins ; ++i){
      stringstream str("_bin");str<<i;
      if(switchTH1On)      vth1.at(i).Write();
      if(switchTH2On)      th2.Write();
                           vmean.at(i).Write(TString("mean_")+name+str.str());
      if(switchTMomentsOn) vmom.at(i).Write(TString("moments_")+name+str.str());
    }
  }
  tprofile.Write();
  profile.Write();
  nEntries.Write();

  if(writeClass)
    this->Write("ttprofile_class_"+name);

  gDirectory->cd("../");
}

TTProfile& TTProfile::operator=(const TTProfile& ttp){
  if(debug) cout << "+ [DEBUG] TTProfile : Starting operator=() {" << name << "}" << endl;
  nBins       = ttp.nBins;
  nEvents     = ttp.nEvents;
  name        = ttp.name;
  profileDone = ttp.profileDone;
  
  vth1        = ttp.vth1;
  th2         = ttp.th2;
  vmean       = ttp.vmean;
  vmom        = ttp.vmom;
  tprofile    = ttp.tprofile;
  profile     = ttp.profile;
  nEntries    = ttp.nEntries;
  binning     = ttp.binning;
  vbinning    = ttp.vbinning;
  if(debug) cout << "- [DEBUG] TTProfile : Finished operator=() {" << name << "}" << endl;
  
  return *this;
}

/*TTProfile::TTProfile(const TTProfile& ttp){
  if(debug) cout << "+ [DEBUG] TTProfile : Starting Copy() {" << name << "}" << endl;
  nBins       = ttp.nBins;
  nEvents     = ttp.nEvents;
  name        = ttp.name;
  profileDone = ttp.profileDone;
  
  vth1        = ttp.vth1;
  th2         = ttp.th2;
  vmean       = ttp.vmean;
  vmom        = ttp.vmom;
  tprofile    = ttp.tprofile;
  profile     = ttp.profile;
  nEntries    = ttp.nEntries;
  binning     = ttp.binning;
  vbinning    = ttp.vbinning;
  if(debug) cout << "+ [DEBUG] TTProfile : Finished Copy() {" << name << "}" << endl;
  
}*/

