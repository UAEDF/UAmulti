#include <TROOT.h>
#include <TH1.h>
#include <TH2.h>
#include <TGraphAsymmErrors.h>
#include <TProfile.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TTree.h>
#include <TFile.h>
#include <TChain.h>
//#include <TDirectory.h>
#include <iostream>
#include <sstream>
using namespace std;
//#include "TSystem.h"
//#include "myevent.h"
//#include "HWWGenPart.h"

#include "../plugins/MyEvtId.h"
#include "../plugins/MyL1Trig.h"
#include "../plugins/MyGenKin.h"
#include "../plugins/MyPart.h"
#include "../plugins/MyGenPart.h"
#include "../plugins/MyTracks.h"
#include "../plugins/MyVertex.h"
#include "../plugins/MyMITEvtSel.h"
#include "../plugins/MyBeamSpot.h"
#include "../plugins/TrackPlots.h"
#include "../plugins/VertexPlots.h"
#include "../plugins/EvtSelPlots.h"
#include "../plugins/BasePlots.h"
#include "../plugins/MultiPlots.h"
#include "../plugins/GenMultiPlots.h"
#include "../plugins/MatrixPlots.h"

bool isMC = true;
bool debug = false;


#include "fileManager.C"
#include "cuts.C"
#include "evtSel.C"
#include "binningMap.C"

void multiplyByWidth(TH1F*);
void multiplyByWidth(TH2F*);

void mptCorrection(int type = 10 , double E = 0.9 , int iTracking = 1 , int nevt_max = 100 , int mctype = 10 ,
		   int syst = 0 , int syst_sign = 0 , bool multbyWidth = true){
  
  
  if(type==0) isMC = false;
  TString MCtype = "pythia";
  if(mctype == 20) MCtype = "phojet";
  
  
  TString filedir("../plots/systv9/");
  
  
  //**********************************************
  //Taking mtx/histos from smallCode files
  
  ostringstream fname("");
  fname<<"../plots/smallcodev4/smallCode_MCtype"<<mctype<<"_"<<E<<"TeV.root";
  //fname<<"../macro/smallCode_MCtype"<<mctype<<"_"<<E<<"TeV.root";
  cout<<"smallCode file : "<<fname.str()<<endl;
  TFile* smallcode = TFile::Open(fname.str().c_str(),"READ");
  
  if(smallcode==0){
    cout<<"Couldn't open file "<<smallcode<<" . Exiting ..."<<endl;
    return;
  }
  
  TProfile* mptVSnchreco_reco = (TProfile*) smallcode->Get("mptVSnchreco_reco");
  TProfile* mptVSnchreco_gen  = (TProfile*) smallcode->Get("mptVSnchreco_gen");
  
  TH1F* pt_genMINUSreco = (TH1F*) smallcode->Get("pt_genMINUSreco");
  
  
  //**********************************************
  //Taking mtx/histos from simpleana files
    
  
  ostringstream outstr("");
  outstr << "hyp1_niter0_cut5_DataType"<<type;
  TString sunfoldfile = fileManager(3,mctype,E,iTracking,syst,syst_sign,outstr.str(),filedir);
  cout<<"Unfolding file : "<<sunfoldfile<<endl;
  TFile* unfoldfile = TFile::Open(sunfoldfile,"READ");
  
  if(unfoldfile==0){
    cout<<"Couldn't open file "<<sunfoldfile<<" . Exiting ..."<<endl;
    return;
  }
  
  
  //Get Matrix
  TH2F* matrixhist = (TH2F*) unfoldfile->Get("unfolding/nch_matrix");
  if(multbyWidth) multiplyByWidth(matrixhist);
  
  TH1F* nch_data_unfolded = (TH1F*) unfoldfile->Get("unfolding/nch_unfoldedPtr");
  if(multbyWidth) multiplyByWidth(nch_data_unfolded);
  nch_data_unfolded->Scale(1./nch_data_unfolded->Integral());
  
  TH1D* projX = (TH1D*) matrixhist->ProjectionX();
  
  for(int igen = 1 ; igen <= matrixhist->GetNbinsX() ; ++igen){
    double factor = 1;
    if(projX->GetBinContent(igen)!=0)
      factor = nch_data_unfolded->GetBinContent(igen) / projX->GetBinContent(igen);
    
    for(int ireco = 1 ; ireco <= matrixhist->GetNbinsY() ; ++ireco){
      matrixhist->SetBinContent(igen , ireco ,  matrixhist->GetBinContent(igen , ireco) * factor );
    }
  }
  
  
  nch_data_unfolded->Draw();
  projX = (TH1D*) matrixhist->ProjectionX();
  projX->Scale(1./projX->Integral());
  projX->SetLineColor(kRed);
  projX->Draw("samehist");
  
  //gPad->WaitPrimitive();
  
  TH1F* nch_data_NSD_afterSDsub = (TH1F*) unfoldfile->Get("unfolding/nch_data_NSD_afterSDsub");
  if(multbyWidth) multiplyByWidth(nch_data_NSD_afterSDsub);
  nch_data_NSD_afterSDsub->Scale(1./nch_data_NSD_afterSDsub->Integral());
  nch_data_NSD_afterSDsub->Draw();
  
  TH1D* projY = (TH1D*) matrixhist->ProjectionY();
  projY->Scale(1./projY->Integral());
  projY->SetLineColor(kRed);
  projY->Draw("samehist");
  gPad->Update();
  
  //gPad->WaitPrimitive();
  
  TH1F* eff_evtSel = (TH1F*) unfoldfile->Get("unfolding/eff_evtSel");
  
  
  TProfile* mptVSnch = new TProfile("mptVSnch","mptVSnch",nch_data_unfolded->GetNbinsX(),nch_data_unfolded->GetXaxis()->GetXbins()->GetArray());
  mptVSnch->SetErrorOption("i");
  
  
  
  TProfile* mptVSnch_noCorr = new TProfile("mptVSnch_noCorr","mptVSnch_noCorr",nch_data_unfolded->GetNbinsX(),nch_data_unfolded->GetXaxis()->GetXbins()->GetArray());
  mptVSnch_noCorr->SetErrorOption("i");
  
  TProfile* mptVSnch_nonchCorr = new TProfile("mptVSnch_nonchCorr","mptVSnch_nonchCorr",nch_data_unfolded->GetNbinsX(),nch_data_unfolded->GetXaxis()->GetXbins()->GetArray());
  mptVSnch_noCorr->SetErrorOption("i");
  
  TProfile* mptVSnch_nomptCorr = new TProfile("mptVSnch_nomptCorr","mptVSnch_nomptCorr",nch_data_unfolded->GetNbinsX(),nch_data_unfolded->GetXaxis()->GetXbins()->GetArray());
  mptVSnch_noCorr->SetErrorOption("i");
  
  TProfile* mptVSnch_noeffCorr = new TProfile("mptVSnch_noeffCorr","mptVSnch_noeffCorr",nch_data_unfolded->GetNbinsX(),nch_data_unfolded->GetXaxis()->GetXbins()->GetArray());
  mptVSnch_noCorr->SetErrorOption("i");
  
  
  
  TProfile* mptVSnch_nchCorr = new TProfile("mptVSnch_nchCorr","mptVSnch_nchCorr",nch_data_unfolded->GetNbinsX(),nch_data_unfolded->GetXaxis()->GetXbins()->GetArray());
  mptVSnch_noCorr->SetErrorOption("i");
  
  TProfile* mptVSnch_nch_mptCorr = new TProfile("mptVSnch_nch_mptCorr","mptVSnch_nch_mptCorr",nch_data_unfolded->GetNbinsX(),nch_data_unfolded->GetXaxis()->GetXbins()->GetArray());
  mptVSnch_noCorr->SetErrorOption("i");
    
  
  TProfile* mptVSnch_gen = new TProfile("mptVSnch_gen","mptVSnch_gen",nch_data_unfolded->GetNbinsX(),nch_data_unfolded->GetXaxis()->GetXbins()->GetArray());
  mptVSnch_gen->SetErrorOption("i");
  
  TH2F* mptrecoVSgen = new TH2F("mptrecoVSgen","mptrecoVSgen",100,0,3,100,0,3);
  TH1F* mptrecoVSgen_diff = new TH1F("mptrecoVSgen_diff","mptrecoVSgen_diff",200,-3,3);
  
  
  TProfile* systp_trk =  new TProfile("systp_trk","systp_trk",nch_data_unfolded->GetNbinsX(),nch_data_unfolded->GetXaxis()->GetXbins()->GetArray());
  TProfile* systm_trk =  new TProfile("systm_trk","systm_trk",nch_data_unfolded->GetNbinsX(),nch_data_unfolded->GetXaxis()->GetXbins()->GetArray());
  
  TProfile* systp_pterr =  new TProfile("systp_pterr","systp_pterr",nch_data_unfolded->GetNbinsX(),nch_data_unfolded->GetXaxis()->GetXbins()->GetArray());
  TProfile* systm_pterr =  new TProfile("systm_pterr","systm_pterr",nch_data_unfolded->GetNbinsX(),nch_data_unfolded->GetXaxis()->GetXbins()->GetArray());
  
  
  
  
  
  //TProfile* tmp = new TProfile("tmp","tmp",nch_data_unfolded->GetNbinsX(),nch_data_unfolded->GetXaxis()->GetXbins()->GetArray());
  
  //adding files to the tree chain
  TChain* tree = new TChain("evt","");
  tree->Add(fileManager(0,type,E));
  
  
  //adding branches to the tree
  
  MyEvtId* evtId      = NULL ;
  tree->SetBranchAddress("EvtId",&evtId);
  
  vector<MyGenPart>* genPart = NULL;
  if(isMC) tree->SetBranchAddress("GenPart",&genPart);
  
  MyGenKin* genKin = NULL;
  if(isMC) tree->SetBranchAddress("GenKin",&genKin);
  
  vector<MyTracks>*  tracks  = NULL;
  vector<MyVertex>*  vertex  = NULL;
  vector<MyVertex>*  vertexToCut  = NULL;
  MyL1Trig*     L1Trig    = NULL;
  MyMITEvtSel*  MITEvtSel = NULL;
  MyBeamSpot*   bs        = NULL;
  if(iTracking==0){
    tree->SetBranchAddress("generalTracks",&tracks); 
    tree->SetBranchAddress("primaryVertex",&vertex);
  }
  else if(iTracking==1){
    tree->SetBranchAddress("minbiasTracks",&tracks); 
    tree->SetBranchAddress("ferencVtxFerTrk",&vertex);
  }
  
  tree->SetBranchAddress("pixel3Vertex",&vertexToCut);
  tree->SetBranchAddress("L1Trig",&L1Trig);
  tree->SetBranchAddress("MITEvtSel",&MITEvtSel);
  tree->SetBranchAddress("beamSpot",&bs);
  
  //Getting number of events
  int nev = int(tree->GetEntries());
  std::cout <<"number of entries is : "<< nev << std::endl;
  cout<<"Running on: "<<min(nev,nevt_max)<<" events"<<endl;
  // Event TYPE counting --> Weights
  
  //starting loop over events
  for(int i = 0; i < nev; i++){
     
    if( ((i+1) % 10000) == 0) cout <<int(double(i+1)/double(min(nev,nevt_max))*100.)<<" % done"<<endl;
    
    if(i>min(nev,nevt_max)) break;
    
    tree->GetEntry(i);
    
    //Skipping the SD events starting from here
    if(isMC)
      if(isSD(genKin , MCtype) ) continue;
      
    
    double mpt_gen_noSel = 0;
    int n_gen_noSel = 0;
    if(isMC){
      for(vector<MyGenPart>::iterator it_tr = genPart->begin() ; it_tr != genPart->end() ; ++it_tr)
        if( isGenPartGood(*it_tr) && isInAcceptance(it_tr->Part,0,2.4) )
	  mpt_gen_noSel += it_tr->Part.v.Pt();
      n_gen_noSel = getnPrimaryGenPart(genPart,0,2.4);
      if(n_gen_noSel!=0) mpt_gen_noSel/=n_gen_noSel;
      mptVSnch_gen->Fill(n_gen_noSel,mpt_gen_noSel);
    }  
    
    //skipping events that don't pass our event selection
    if(!isEvtGood(E,*L1Trig , *MITEvtSel , vertexToCut)) continue;
    
    /*double mpt_gen = 0;
    int n_gen = 0;
    if(isMC){
      for(vector<MyGenPart>::iterator it_tr = genPart->begin() ; it_tr != genPart->end() ; ++it_tr)
        if( isGenPartGood(*it_tr) && isInAcceptance(it_tr->Part,0,2.4) )
	  mpt_gen += it_tr->Part.v.Pt();
      n_gen = getnPrimaryGenPart(genPart,0,2.4);
      if(n_gen!=0) mpt_gen/=n_gen;
    }  */
    
    vector<MyTracks> trcoll;
    if(iTracking==0) trcoll = getPrimaryTracks(*tracks,vertex,0.1,2.4);
    if(iTracking==1) trcoll = getPrimaryTracks(*tracks,vertex,bs,0.1,2.4);
    int n = trcoll.size();
    
    double mpt = 0;
    double mpt_systp_err = 0 , mpt_systm_err = 0 ;
    for(vector<MyTracks>::iterator itr = trcoll.begin() ; itr != trcoll.end() ; ++itr){
      mpt+=itr->Part.v.Pt();
      
      mpt_systp_err+=itr->Part.v.Pt()+itr->ept;
      mpt_systm_err+=itr->Part.v.Pt()-itr->ept;
    }
    if(n!=0){
      mpt/=n;
      mpt_systp_err/=n;
      mpt_systm_err/=n;
    }
    
    //tmp->Fill(n_gen,mpt_gen);
    
    
    int bin_reco = matrixhist->GetYaxis()->FindFixBin(n);
    double norm = matrixhist->ProjectionY()->GetBinContent(bin_reco);
    if(norm==0) norm = 1;
    
    double ptcorr = 1;
    if(mptVSnchreco_reco->GetBinContent(bin_reco)!=0)
      ptcorr = mptVSnchreco_gen->GetBinContent(bin_reco) / mptVSnchreco_reco->GetBinContent(bin_reco);
	
    double eff = 1;
    if(eff_evtSel->GetBinContent(bin_reco)!=0)
      eff = eff_evtSel->GetBinContent(bin_reco);
    
    mptVSnch_noCorr->Fill(n , mpt);
    mptVSnch_nonchCorr->Fill(n , mpt * ptcorr , 1. / eff );
    
    //cout<<"event "<<i<<" , nch = "<<n<<" , mpt = "<<mpt<<" , bin_reco = "<<bin_reco<<endl;
    for(int igen = 1 ; igen <= mptVSnch->GetNbinsX() ; ++igen){
      double ptcorr = 1;
      if(mptVSnchreco_reco->GetBinContent(igen)!=0)
        ptcorr = mptVSnchreco_gen->GetBinContent(igen) / mptVSnchreco_reco->GetBinContent(igen);
	
      double eff = 1;
      if(eff_evtSel->GetBinContent(igen)!=0)
        eff = eff_evtSel->GetBinContent(igen);
	
      mptVSnch->Fill(mptVSnch->GetBinCenter(igen) , mpt * ptcorr , 1. / eff * matrixhist->GetBinContent(igen , bin_reco)/norm);
      systp_pterr->Fill(mptVSnch->GetBinCenter(igen) , mpt_systp_err * ptcorr , 1. / eff * matrixhist->GetBinContent(igen , bin_reco)/norm);
      systm_pterr->Fill(mptVSnch->GetBinCenter(igen) , mpt_systm_err * ptcorr , 1. / eff * matrixhist->GetBinContent(igen , bin_reco)/norm);
      
      mptVSnch_nomptCorr->Fill(mptVSnch->GetBinCenter(igen) , mpt , 1. / eff * matrixhist->GetBinContent(igen , bin_reco)/norm);
      mptVSnch_noeffCorr->Fill(mptVSnch->GetBinCenter(igen) , mpt * ptcorr , 1. * matrixhist->GetBinContent(igen , bin_reco)/norm);
      
      mptVSnch_nchCorr->Fill(mptVSnch->GetBinCenter(igen) , mpt , matrixhist->GetBinContent(igen , bin_reco)/norm);
      mptVSnch_nch_mptCorr->Fill(mptVSnch->GetBinCenter(igen) , mpt * ptcorr , matrixhist->GetBinContent(igen , bin_reco)/norm);
      
      
      
      //cout<<"bin "<<igen<<" , eff = "<<eff<<" mtx = "<<matrixhist->GetBinContent(igen , bin_reco)/norm<<endl;
      //cout<<"bin "<<igen<<" , eff = "<<eff<<" mtx = "<<matrixhist->GetBinContent(igen , bin_reco)/norm<<endl;
      //cout<<"n_gen "<<n_gen<<" , mpt_gen = "<<mpt_gen_noSel<<" n = "<<n<<" mpt_reco*ptcorr = "<<mpt * ptcorr<<endl;
    }
    
    /*double ptcorr = 1;
      if(mptVSnchreco_reco->GetBinContent(bin_reco)!=0)
        ptcorr = mptVSnchreco_gen->GetBinContent(bin_reco) / mptVSnchreco_reco->GetBinContent(bin_reco);
    mptrecoVSgen->Fill(mpt_gen,mpt*ptcorr);
    mptrecoVSgen_diff->Fill(mpt_gen-mpt*ptcorr);*/
    
    
  }//end of loop over events
  
  
  mptVSnch->Draw();
  
  mptVSnch_gen->SetLineColor(kRed);
  mptVSnch_gen->Draw("same");
  
  //tmp->SetLineColor(kBlue);
  //tmp->Draw("same");
  
  gPad->Update();
  gPad->WaitPrimitive();
  
  gStyle->SetPalette(1);
  //mptrecoVSgen->Draw("colz");
  mptrecoVSgen_diff->Draw();
  
  
  //Doing the trk systematics
  double syst_trk_frac = 2.58;
  double meanpt_losttrk = pt_genMINUSreco->GetMean();
  for(int igen = 1 ; igen <= mptVSnch->GetNbinsX() ; ++igen){
    if(mptVSnch->GetBinContent(igen)!=0){
      systp_trk->Fill(mptVSnch->GetBinCenter(igen),mptVSnch->GetBinContent(igen)+syst_trk_frac/100.*meanpt_losttrk);
      systm_trk->Fill(mptVSnch->GetBinCenter(igen),mptVSnch->GetBinContent(igen)-syst_trk_frac/100.*meanpt_losttrk);
      cout<<mptVSnch->GetBinContent(igen)<<"  "<<mptVSnch->GetBinContent(igen)-syst_trk_frac/100.*meanpt_losttrk<<endl;
    }
  }
  
  //doign the global systematic
  Double_t* x   = new Double_t[mptVSnch->GetNbinsX()];
  Double_t* y   = new Double_t[mptVSnch->GetNbinsX()];
  Double_t* exl = new Double_t[mptVSnch->GetNbinsX()];
  Double_t* exh = new Double_t[mptVSnch->GetNbinsX()];
  Double_t* eyl = new Double_t[mptVSnch->GetNbinsX()];
  Double_t* eyh = new Double_t[mptVSnch->GetNbinsX()];
  for(int i = 1 ; i <= mptVSnch->GetNbinsX() ; ++i){
    x[i-1] = mptVSnch->GetBinCenter(i);
    y[i-1] = mptVSnch->GetBinContent(i);
    exl[i-1] = mptVSnch->GetBinCenter(i) - mptVSnch->GetXaxis()->GetBinLowEdge(i);
    exh[i-1] = mptVSnch->GetXaxis()->GetBinUpEdge(i) - mptVSnch->GetBinCenter(i);
  
    eyl[i-1] = pow(systm_trk->GetBinContent(i)-mptVSnch->GetBinContent(i),2) + pow(systm_pterr->GetBinContent(i)-mptVSnch->GetBinContent(i),2) + pow(mptVSnch->GetBinError(i),2);
    eyl[i-1] = sqrt(eyl[i-1]);
  
    eyh[i-1] = pow(systp_trk->GetBinContent(i)-mptVSnch->GetBinContent(i),2) + pow(systp_pterr->GetBinContent(i)-mptVSnch->GetBinContent(i),2) + pow(mptVSnch->GetBinError(i),2);
    eyh[i-1] = sqrt(eyh[i-1]);
  }
  
  TGraphAsymmErrors* gsyst = new TGraphAsymmErrors(mptVSnch->GetNbinsX(),x,y,exl,exh,eyl,eyh);
  gsyst->SetName(TString("g")+mptVSnch->GetName()+TString("_syst"));
  gsyst->SetTitle(TString("g")+mptVSnch->GetTitle()+TString("_syst"));
  gsyst->GetXaxis()->SetTitle("n");
  gsyst->GetYaxis()->SetTitle("<p_{t}>");
  
  //output file
  ostringstream strout("");
  //strout<<"mptCorr_MCtype"<<type<<"_"<<E<<"TeV.root";
  strout<<"dataType"<<type;
  TString outname = fileManager(5,mctype,E,iTracking,syst,syst_sign,strout.str());
  cout<<"Output file : "<<outname<<endl;
  TFile* output = new TFile(outname,"RECREATE");
  output->cd();
    
  mptVSnch->SetBinContent(1,0);mptVSnch->SetBinError(1,0);
  mptVSnch->Write();
  
  mptVSnch_noCorr->SetBinContent(1,0);mptVSnch_noCorr->SetBinError(1,0);
  mptVSnch_noCorr->Write();
  
  mptVSnch_nonchCorr->SetBinContent(1,0);mptVSnch_nonchCorr->SetBinError(1,0);
  mptVSnch_nonchCorr->Write();
  
  mptVSnch_nomptCorr->SetBinContent(1,0);mptVSnch_nomptCorr->SetBinError(1,0);
  mptVSnch_nomptCorr->Write();
  
  mptVSnch_noeffCorr->SetBinContent(1,0);mptVSnch_noeffCorr->SetBinError(1,0);
  mptVSnch_noeffCorr->Write();
  
  mptVSnch_nchCorr->SetBinContent(1,0);mptVSnch_nchCorr->SetBinError(1,0);
  mptVSnch_nchCorr->Write();
  
  mptVSnch_nch_mptCorr->SetBinContent(1,0);mptVSnch_nch_mptCorr->SetBinError(1,0);
  mptVSnch_nch_mptCorr->Write();
  
  if(isMC){
    mptVSnch_gen->SetBinContent(1,0);mptVSnch_gen->SetBinError(1,0);
    mptVSnch_gen->Write();
    
    mptrecoVSgen->SetBinContent(1,0);mptrecoVSgen->SetBinError(1,0);
    mptrecoVSgen->Write();
    
    mptVSnch_gen->Divide(mptVSnch);
    mptVSnch_gen->Write("effnchreco_genOreco");
    
    /*gPad->WaitPrimitive();
    
    mptVSnch_gen->Draw();*/
    
  }
  
  systp_pterr->SetBinContent(1,0);systp_pterr->SetBinError(1,0);
  systp_pterr->Write();
  
  systm_pterr->SetBinContent(1,0);systm_pterr->SetBinError(1,0);
  systm_pterr->Write();
  
  systp_trk->SetBinContent(1,0);systp_trk->SetBinError(1,0);
  systp_trk->Write();
  
  systm_trk->SetBinContent(1,0);systm_trk->SetBinError(1,0);
  systm_trk->Write();
  
  gsyst->SetPoint(0,0,0);
  gsyst->Write();
  
  
  output->Close();
}


void multiplyByWidth(TH1F* hist){
  for(int i=1;i<=hist->GetNbinsX();++i){
    hist->SetBinContent(i,double(hist->GetBinContent(i))*double(hist->GetBinWidth(i)));
    hist->SetBinError(i,double(hist->GetBinError(i))*double(hist->GetBinWidth(i)));
  }
}


void multiplyByWidth(TH2F* hist){
  double width = 1;
  for(int i=1;i<=hist->GetNbinsX();++i){
    for(int j=1;j<=hist->GetNbinsX();++j){
      width = hist->GetXaxis()->GetBinWidth(i) * hist->GetYaxis()->GetBinWidth(j);
      hist->SetBinContent(i,j,double(hist->GetBinContent(i,j))*width);
      hist->SetBinError(i,j,double(hist->GetBinError(i,j))*width);
    }
  }
}
