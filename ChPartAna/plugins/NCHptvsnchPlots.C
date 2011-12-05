
#include "NCHptvsnchPlots.h"

ClassImp(NCHptvsnchPlots)

//_____________________________________________________________________________
NCHptvsnchPlots::NCHptvsnchPlots(TString in_mc, TString in_cut, TString in_centr, TString in_hf, TString in_unfdir)
    : cut(in_cut), centr(in_centr), hf(in_hf), mc(in_mc), unfdir(in_unfdir)
{  
  if(hf=="HF1")
    ptvsnchcoll=hf+"_"+centr+"_NSD_"+cut;
  else
    ptvsnchcoll=hf+"_"+centr+"_INEL_"+cut;

  this->init();
}

//_____________________________________________________________________________
NCHptvsnchPlots::~NCHptvsnchPlots(){ 

  delete nch_unfolded;
  delete matrix; 
  
  delete mptVSnch_fullSel_nchCorr;   
  delete mptVSnch_noSel_nchCorr;
  
  delete mptVSnch_noSel;
  delete mptVSnch_L1;
  delete mptVSnch_HF;
  delete mptVSnch_vtxQual;
  delete mptVSnch_vtx;
  delete mptVSnch_L1_HF;
  delete mptVSnch_L1_HF_vtxQual;
  delete mptVSnch_fullSel;
  
}


//_____________________________________________________________________________
void NCHptvsnchPlots::init()
{
   TFile* _file = TFile::Open(unfdir+"/"+"unf_"+mc+"__allEffs_partfull_"+ptvsnchcoll+".root","READ");
  assert(_file);
  
  nch_unfolded = (TH1F*) _file->FindObjectAny("nch_unfoldedPtr");
  nch_unfolded->SetName("nch_unfoldedPtr"+ptvsnchcoll);
  assert(nch_unfolded);
  matrix       = (TH2F*) _file->FindObjectAny("nch_matrix");
  matrix->SetName("nch_matrix"+ptvsnchcoll);  
  assert(matrix);
  _file->Close();
  delete _file;
  
  // reweighting the matrix with the unfolded nch. + Normalizing matrix per reco line ( CHECK IF CORRECT!!!!!)
  bool multbyWidth = false;
  if(multbyWidth) this->multiplyByWidth(matrix);
  
  if(multbyWidth) this->multiplyByWidth(nch_unfolded);
  nch_unfolded->Scale(1./(nch_unfolded->Integral()));
  
  TH1D* projX = (TH1D*) (matrix->ProjectionX());
  
  for(int igen = 1 ; igen <= matrix->GetNbinsX() ; ++igen){
    double factor = 1;
    if(projX->GetBinContent(igen)!=0)
      factor = nch_unfolded->GetBinContent(igen) / projX->GetBinContent(igen);
  
    for(int ireco = 1 ; ireco <= matrix->GetNbinsY() ; ++ireco){
      matrix->SetBinContent(igen , ireco ,  matrix->GetBinContent(igen , ireco) * factor );
    }
   }
   //int bin_reco = matrix->GetYaxis()->FindFixBin(n);
  // double norm = matrix->ProjectionY()->GetBinContent(bin_reco);
  mptVSnch_noSel_nchCorr = new TProfile("mptVSnch_noSel_nchCorr_"+ptvsnchcoll,"mptVSnch_noSel_nchCorr_"+ptvsnchcoll,nch_unfolded->GetNbinsX(),nch_unfolded->GetXaxis()->GetXbins()->GetArray());
  mptVSnch_noSel_nchCorr->SetErrorOption("i"); 
  mptVSnch_fullSel_nchCorr = new TProfile("mptVSnch_fullSel_nchCorr_"+ptvsnchcoll,"mptVSnch_fullSel_nchCorr_"+ptvsnchcoll,nch_unfolded->GetNbinsX(),nch_unfolded->GetXaxis()->GetXbins()->GetArray());
  mptVSnch_fullSel_nchCorr->SetErrorOption("i"); 
  
  mptVSnch_noSel = new TProfile("mptVSnch_noSel_"+ptvsnchcoll,"mptVSnch_noSel_"+ptvsnchcoll,nch_unfolded->GetNbinsX(),nch_unfolded->GetXaxis()->GetXbins()->GetArray());
  mptVSnch_noSel->SetErrorOption("i");
  mptVSnch_L1 = new TProfile("mptVSnch_L1_"+ptvsnchcoll,"mptVSnch_L1_"+ptvsnchcoll,nch_unfolded->GetNbinsX(),nch_unfolded->GetXaxis()->GetXbins()->GetArray());
  mptVSnch_L1->SetErrorOption("i");
  mptVSnch_HF = new TProfile("mptVSnch_HF_"+ptvsnchcoll,"mptVSnch_HF_"+ptvsnchcoll,nch_unfolded->GetNbinsX(),nch_unfolded->GetXaxis()->GetXbins()->GetArray());
  mptVSnch_HF->SetErrorOption("i");
  mptVSnch_vtxQual = new TProfile("mptVSnch_vtxQual_"+ptvsnchcoll,"mptVSnch_vtxQual_"+ptvsnchcoll,nch_unfolded->GetNbinsX(),nch_unfolded->GetXaxis()->GetXbins()->GetArray());
  mptVSnch_vtxQual->SetErrorOption("i");
  mptVSnch_vtx = new TProfile("mptVSnch_vtx_"+ptvsnchcoll,"mptVSnch_vtx_"+ptvsnchcoll,nch_unfolded->GetNbinsX(),nch_unfolded->GetXaxis()->GetXbins()->GetArray());
  mptVSnch_vtx->SetErrorOption("i");
  mptVSnch_L1_HF = new TProfile("mptVSnch_L1_HF_"+ptvsnchcoll,"mptVSnch_L1_HF_"+ptvsnchcoll,nch_unfolded->GetNbinsX(),nch_unfolded->GetXaxis()->GetXbins()->GetArray());
  mptVSnch_L1_HF->SetErrorOption("i");
  mptVSnch_L1_HF_vtxQual = new TProfile("mptVSnch_L1_HF_vtxQual_"+ptvsnchcoll,"mptVSnch_L1_HF_vtxQual_"+ptvsnchcoll,nch_unfolded->GetNbinsX(),nch_unfolded->GetXaxis()->GetXbins()->GetArray());
  mptVSnch_L1_HF_vtxQual->SetErrorOption("i");
  mptVSnch_fullSel = new TProfile("mptVSnch_fullSel_"+ptvsnchcoll,"mptVSnch_fullSel_"+ptvsnchcoll,nch_unfolded->GetNbinsX(),nch_unfolded->GetXaxis()->GetXbins()->GetArray());
  mptVSnch_fullSel->SetErrorOption("i");  

}

//_____________________________________________________________________________
void NCHptvsnchPlots::fill(MapType* map_s, double meanpt, double trackcoll_size){

  if ( !goodEvent(map_s) )
    return;
  bool hf_bool=false;  
  if(hf=="HF1" && map_s->find("HF1")->second ==1)
     hf_bool==true;
  if(hf=="HF0") hf_bool=true;     

  //raw ones
  mptVSnch_noSel->Fill(trackcoll_size, meanpt);    
  if(map_s->find("L1")->second ==1)
      mptVSnch_L1->Fill(trackcoll_size, meanpt); 
       
  if(hf_bool)   
      mptVSnch_HF->Fill(trackcoll_size, meanpt);
      
  if(map_s->find("VtxQual")->second ==1)   
      mptVSnch_vtxQual->Fill(trackcoll_size, meanpt);
        
  if(map_s->find("Vtx")->second ==1)   
      mptVSnch_vtx->Fill(trackcoll_size, meanpt);   
  
  if(map_s->find("L1")->second ==1) {  
     if(hf_bool) { 
        mptVSnch_L1_HF->Fill(trackcoll_size, meanpt);
        if(map_s->find("VtxQual")->second ==1) {   
            mptVSnch_L1_HF_vtxQual->Fill(trackcoll_size, meanpt);
            if(map_s->find("Vtx")->second ==1) {
                mptVSnch_fullSel->Fill(trackcoll_size, meanpt);
            }    
        }        
     }             
  }
     
  int bin_reco = matrix->GetYaxis()->FindFixBin(trackcoll_size);
  double norm = matrix->ProjectionY()->GetBinContent(bin_reco);
  double ptcorr=1;
  if(norm==0) norm = 1;
  
  
  int bincentr_igen=0;
  double mat_norm=0;
  for(int igen = 1 ; igen <= mptVSnch_noSel->GetNbinsX() ; ++igen){ 
    bincentr_igen=mptVSnch_noSel->GetBinCenter(igen); 
    mat_norm = matrix->GetBinContent(igen, bin_reco)/norm;
    mptVSnch_noSel_nchCorr->Fill(bincentr_igen, meanpt * ptcorr,  mat_norm);     
    /*
    if(map_s->find("L1")->second ==1) 
       mptVSnch_L1_nchCorr->Fill(bincentr_igen, meanpt * ptcorr,  mat_norm);
    if(hf_bool)   
        mptVSnch_HF_nchCorr->Fill(bincentr_igen, meanpt * ptcorr,  mat_norm);
      
    if(map_s->find("VtxQual")->second ==1)   
        mptVSnch_vtxQual_nchCorr->Fill(bincentr_igen, meanpt * ptcorr,  mat_norm);
        
    if(map_s->find("Vtx")->second ==1)   
        mptVSnch_vtx_nchCorr->Fill(bincentr_igen, meanpt * ptcorr,  mat_norm);   
  */
    if(map_s->find("L1")->second ==1) {  
        if(hf_bool) { 
            //mptVSnch_L1_HF_nchCorr->Fill(bincentr_igen, meanpt * ptcorr,  mat_norm);
            if(map_s->find("VtxQual")->second ==1) {   
                //mptVSnch_L1_HF_vtxQual_nchCorr->Fill(bincentr_igen, meanpt * ptcorr,  mat_norm);
                if(map_s->find("Vtx")->second ==1) {
                    mptVSnch_fullSel_nchCorr->Fill(bincentr_igen, meanpt * ptcorr,  mat_norm);
                }    
            }        
        }             
    }
  }
  

   
  //cout << "mean pt " << meanpt << endl;
}

//_____________________________________________________________________________
bool NCHptvsnchPlots::goodEvent(MapType* map_s){

    if( map_s->find(centr+"RECO")->second ==0)
        return false;
    if(hf=="HF1" && map_s->find("MC")->second ==1)   //isMC and HF1   
        if( map_s->find("SD")->second ==1) // request HF1+NSD so :  NSD= !SD  so if SD --> return
            return false;   
    return true;   
}

//_____________________________________________________________________________
void NCHptvsnchPlots::write(){
  //TDirectory* dir = new TDirectory("NCHptvsnchPlots_"+ptvsnchcoll,"");
  //dir->cd();  
  gDirectory->mkdir("ptvsnch"+ptvsnchcoll);
  gDirectory->cd("ptvsnch"+ptvsnchcoll);
  
  nch_unfolded->Write(); 
  matrix->Write();
  
  mptVSnch_noSel_nchCorr->Write();
  mptVSnch_fullSel_nchCorr->Write();
  
  mptVSnch_noSel->Write();
  mptVSnch_L1->Write();
  mptVSnch_HF->Write();
  mptVSnch_vtxQual->Write();
  mptVSnch_vtx->Write();
  mptVSnch_L1_HF->Write();
  mptVSnch_L1_HF_vtxQual->Write();
  mptVSnch_fullSel->Write();
  
  gDirectory->cd("../");
}



//_____________________________________________________________________________
void NCHptvsnchPlots::multiplyByWidth(TH1F* hist){
  for(int i=1;i<=hist->GetNbinsX();++i){
    hist->SetBinContent(i,double(hist->GetBinContent(i))*double(hist->GetBinWidth(i)));
    hist->SetBinError(i,double(hist->GetBinError(i))*double(hist->GetBinWidth(i)));
  }
}

//_____________________________________________________________________________
void NCHptvsnchPlots::multiplyByWidth(TH2F* hist){
  double width = 1;
  for(int i=1;i<=hist->GetNbinsX();++i){
    for(int j=1;j<=hist->GetNbinsX();++j){
      width = hist->GetXaxis()->GetBinWidth(i) * hist->GetYaxis()->GetBinWidth(j);
      hist->SetBinContent(i,j,double(hist->GetBinContent(i,j))*width);
      hist->SetBinError(i,j,double(hist->GetBinError(i,j))*width);
    }
  }
}
