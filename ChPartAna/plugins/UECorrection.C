#include "UECorrection.h"

#define pi 3.14159265358979323846

//double deg2rad(double);
//double mod(double);

ClassImp(UECorrection)

UECorrection::UECorrection(){
  UEcoll = "none";
  nobins = 26;
  varbins = new double [27];
  for (int i = 0; i < 14; i++)
      varbins[i] = i;
  for (int i = 0; i < 5; i++)
      varbins[14+i] = 14+i*2;
  for (int i = 0; i < 3; i++)
      varbins[19+i] = 24+i*4;
  for (int i = 0; i < 3; i++)
      varbins[22+i] = 40+i*8;
  varbins[25] = 64; 
  varbins[26] = 100;
  this->init();
}

UECorrection::UECorrection(TString collname){
  UEcoll = collname;
  nobins = 26;
  varbins = new double [27];
  for (int i = 0; i < 14; i++)
      varbins[i] = i;
  for (int i = 0; i < 5; i++)
      varbins[14+i] = 14+i*2;
  for (int i = 0; i < 3; i++)
      varbins[19+i] = 24+i*4;
  for (int i = 0; i < 3; i++)
      varbins[22+i] = 40+i*8;
  varbins[25] = 64; 
  varbins[26] = 100;
  this->init();
}

UECorrection::UECorrection(TString collname, vector<double> binning){
  UEcoll = collname;
  varbins = &(binning.at(0));
  nobins = binning.size()-1;
  //cout<< nBins << endl;
  this->init();
}

UECorrection::~UECorrection(){ }

void UECorrection::init(){ 
     TH1::SetDefaultSumw2(1);

    //int no_varbins;
//    int no_bins;
//    double binup;
//    double* varbins;
//    cout<< "Please enter number of variable binnings. (enter 0 or 1 for default bin size): ";
//    cin>> no_varbins;
//    if ((no_varbins >= 0) || (no_varbins <= 100))
//    {
//       for (i = 0; i <= no_varbins; i++)
//       {
//              cout<< "Please enter the end range of bin set " << i+1 << ": ";
//              cin>> binup;
//              cout<< "Please enter the number of bins for bin set " << i+1 << ": "; 
//              cin>> no_bins;
//              if (i != no_varbins)
//              {
//                 varbins = new double[no_bins];
//                 for (int j = 0; j < no_bins; j++)
//                     varbins[i
//              else
//                  varbins = new double[no_bins+1];
//                else if (no_varbins > 100)
//                     cout<< "Invalid variable binning! Only from 0 to 100." << endl;
//                else continue;          
//              }    
  
    /*nobins = 26;
    varbins = new double [nobins+1];
    for (int i = 0; i < 14; i++)
        varbins[i] = i;
    for (int i = 0; i < 5; i++)
        varbins[14+i] = 14+i*2;
    for (int i = 0; i < 3; i++)
        varbins[19+i] = 24+i*4;
    for (int i = 0; i < 3; i++)
        varbins[22+i] = 40+i*8;
    varbins[25] = 64;                     
    varbins[nobins] = 100;*/
    
  //pt_corr = new TH2F("pt_corr", "pt_corr; pt_true; pt_reco", 30, 0, 30, 30, 0, 30);
  pt_corr = new TH2F("pt_corr", "pt_corr; pt_true; pt_reco", nobins, varbins, nobins, varbins);
  
  profile_corrected_matrix = new TH1D("profile_corrected_matrix_<"+UEcoll+">", "profile_corrected_matrix_<"+UEcoll+">;Leading jet pt;<"+UEcoll+">", nobins, varbins);
  profile_corrected_evtsel = new TH1D("profile_corrected_evtsel_<"+UEcoll+">", "profile_corrected_evtsel_<"+UEcoll+">;Leading jet pt;<"+UEcoll+">", nobins, varbins);
  profile_corrected_genreco = new TH1D("profile_corrected_genreco_<"+UEcoll+">", "profile_corrected_genreco_<"+UEcoll+">;Leading jet pt;<"+UEcoll+">", nobins, varbins);
  //profile_corrected_pteta = new TH1D("profile_corrected_pteta_<"+UEcoll+">", "profile_corrected_pteta_<"+UEcoll+">;Leading jet pt;<"+UEcoll+">", nobins, varbins);
  gen_reco_ratio = new TH1D("gen_reco_mult_ratio_<"+UEcoll+">", "gen_reco_mult_ratio_<"+UEcoll+">;Leading jet pt;<# gen>/<# reco>", nobins, varbins);
  truegen_gen_ratio = new TH1D("truegen_gen_mult_ratio_<"+UEcoll+">", "truegen_gen_mult_ratio_<"+UEcoll+">;Leading jet pt;<# truegen>/<# gen>", nobins, varbins);
  probability = new TH1D("probability_<"+UEcoll+">", "probability_<"+UEcoll+">;Leading genjet pt;probability sum", nobins, varbins);
  //profile_corrected_matrix_normalised = new TH1D("profile_corrected_matrix_normalised_<"+UEcoll+">", "profile_corrected_matrix_normalised_<"+UEcoll+">;Leading jet pt;<"+UEcoll+">", nobins, varbins);
  //eff_pt_eta = new TH2D();
  
  iEvt = 0;
}

/*bool UECorrection::CorrectPtEta(TProfile* hist){
     
     TFile* input = TFile::Open("outputMC.root","READ");
     input->cd();
     if(input==0)
     {
              cout << "In CorrectPtEta: Could not retrieve 'outputMC.root' for making corrections. Exiting now ..." << endl;
              return false;
     }
     
     TString histname;
     histname = "EfficiencyPlots_TrEvtSel_nocuts/eff_pt_eta_eff_TrEvtSel_nocuts";
     //cout<< histname << endl;
     TH2F* eff_pt_eta_temp = (TH2F*) input->Get(histname);
     if(hist == 0)
     {    
         cout << "In CorrectPtEta: Couldn't find '"+histname+"'. Exiting now ..." << endl;
         return false;      
     }
     eff_pt_eta = (TH2F*) eff_pt_eta_temp->Clone();
     
       
         
}*/

bool UECorrection::CorrectGenReco(TProfile* hist, TString inputfile){
     
    //TFile* input = TFile::Open("outputMC.root","READ");
    TFile* input = TFile::Open(inputfile,"READ");
    input->cd();
    if(input==0)
    {
             cout << "In CorrectGenReco: Could not retrieve '"+inputfile+"' for making corrections. Exiting now ..." << endl;
             return false;
    }
    
    //TProfile* gen = (TProfile*) input->Get("UEPlots_recojet1genpart05/profile_"+UEcoll+"_trans_recojet1genpart05");
    //TProfile* reco = (TProfile*) input->Get("UEPlots_recojet1part05/profile_"+UEcoll+"_trans_recojet1part05");   
    TProfile* gen = (TProfile*) input->Get("UEPlots_genjet1part05/profile_"+UEcoll+"_trans_genjet1part05");
    TProfile* reco = (TProfile*) hist->Clone();
        
    if(gen == 0 || reco == 0)
    {    
        cout << "In CorrectGenReco: Couldn't find one of the TProfiles. Exiting now ..." << endl;
        return false;      
    }
         
    TH1D* genth1d  = gen->ProjectionX();
    TH1D* recoth1d = reco->ProjectionX();
    
    //genth1d->DrawCopy();
    
    gen_reco_ratio->Divide(genth1d, recoth1d, 1, 1, "B");
    delete profile_corrected_genreco;
    profile_corrected_genreco = hist->ProjectionX();
    profile_corrected_genreco->SetName("profile_corrected_genreco");
    profile_corrected_genreco->SetTitle("profile_corrected_genreco");
    
    for (int i = 1; i <= nobins; i++)
    {
     //cout<< gen_reco_ratio->GetBinContent(i) << "   " << profile_temp->GetBinContent(i) << endl;
     double ratiotemp = gen_reco_ratio->GetBinContent(i);
     double temptemp = profile_corrected_genreco->GetBinContent(i);
     profile_corrected_genreco->SetBinContent(i, ratiotemp*temptemp); 
     //cout<< profile_corrected_genreco->GetBinContent(i) << endl;
    }   
    
    //input->Close();  
    return true;      
}

bool UECorrection::CorrectMatrix(TProfile* hist, TString inputfile){
     
     double pttruefrac;
     int totalno = 0;
     //TAxis *xaxis = profile_corrected_matrix->GetXaxis();
     //double xval;
     
     //TFile* input = TFile::Open("outputMC.root","READ");
     TFile* input = TFile::Open(inputfile,"READ");
     input->cd();
     if(input==0)
     {
        cout << "In CorrectMatrix: Could not retrieve '"+inputfile+"' for making corrections. Exiting now ..." << endl;
        return false;
     }
     
     TString histname;
     histname = "UEFactors_<"+UEcoll+">/pt_corr";
     //cout<< histname << endl;
     TH2F* pt_corr_temp = (TH2F*) input->Get(histname);
     if(hist == 0)
     {    
         cout << "In CorrectMatrix: Couldn't find '"+histname+"'. Exiting now ..." << endl;
         return false;      
     }
     pt_corr = (TH2F*) pt_corr_temp->Clone(); 

     for (int i = 1; i <= nobins; i++)
     {
         //cout<< i+1 << ": ";
         //cout<< hist->GetBinContent(i+1) << endl;
         double bin_val = hist->GetBinContent(i);
         double bin_error = hist->GetBinError(i);

         for (int j = 1; j <= nobins; j++)
             totalno += pt_corr->GetBinContent(j, i);

         //cout<< totalno << endl;
         if (totalno == 0) continue;
         
         for (int j = 1; j <= nobins; j++)
         {
             //cout<< "inside " << j << endl;
             pttruefrac = pt_corr->GetBinContent(j, i)/totalno;
             probability->SetBinContent(j, probability->GetBinContent(j) + pttruefrac);
             //cout<< varbins[j] << "  " << bin_val << "  " << pttruefrac << endl;
             //profile_corrected_matrix->Fill(varbins[j], bin_val, pttruefrac);    
             //xval = xaxis->GetBinCenter(j);
             //cout<< "i: " << i << "  j: " << j << "  binPt: " << xval << "  profileval: " << bin_val << "  corr: " << pt_corr->GetBinContent(j, i) << "  totalno: " << totalno << "  frac: " << pttruefrac << endl;
             //profile_corrected_matrix->Fill(xval, bin_val * pttruefrac); 
             profile_corrected_matrix->SetBinContent(j, profile_corrected_matrix->GetBinContent(j) + bin_val * pttruefrac);
             profile_corrected_matrix->SetBinError(j, profile_corrected_matrix->GetBinError(j) + bin_error * pttruefrac);
         }
        
         totalno = 0;
     }
     profile_corrected_matrix_normalised = (TH1D*) profile_corrected_matrix->Clone();
     profile_corrected_matrix_normalised->SetNameTitle("profile_corrected_matrix_normalised_<"+UEcoll+">", "profile_corrected_matrix_normalised_<"+UEcoll+">");
     profile_corrected_matrix_normalised->Divide(probability);
     return true;
}

bool UECorrection::CorrectMatrix2(TProfile* hist, TString inputfile){
     
     double pttruefrac;
     int totalno = 0;
     
     TFile* input = TFile::Open(inputfile,"READ");
     input->cd();
     if(input==0)
     {
        cout << "In CorrectMatrix: Could not retrieve '"+inputfile+"' for making corrections. Exiting now ..." << endl;
        return false;
     }
     
     TString histname;
     histname = "UEFactors_<"+UEcoll+">/pt_corr";
     TH2F* pt_corr_temp = (TH2F*) input->Get(histname);
     if(hist == 0)
     {    
         cout << "In CorrectMatrix: Couldn't find '"+histname+"'. Exiting now ..." << endl;
         return false;      
     }

     pt_corr = (TH2F*) pt_corr_temp->Clone(); 

     double bin_val;
     double bin_error;
     for (int i = 1; i <= nobins; i++)
     {
         for (int j = 1; j <= nobins; j++)
             totalno += pt_corr->GetBinContent(i, j);

         if (totalno == 0) continue;
         
         for (int j = 1; j <= nobins; j++)
         {
             bin_val = hist->GetBinContent(j);
             bin_error = hist->GetBinError(j);
             pttruefrac = pt_corr->GetBinContent(i, j)/totalno;
             probability->SetBinContent(i, probability->GetBinContent(i) + pttruefrac);
             profile_corrected_matrix->SetBinContent(i, profile_corrected_matrix->GetBinContent(i) + bin_val * pttruefrac);
             profile_corrected_matrix->SetBinError(i, profile_corrected_matrix->GetBinError(i) + bin_error * pttruefrac);
         }
        
         totalno = 0;
     }
     profile_corrected_matrix_normalised = (TH1D*) profile_corrected_matrix->Clone();
     profile_corrected_matrix_normalised->SetNameTitle("profile_corrected_matrix_normalised_<"+UEcoll+">", "profile_corrected_matrix_normalised_<"+UEcoll+">");
     profile_corrected_matrix_normalised->Divide(probability);
     return true;
}

bool UECorrection::CorrectEvtSel(TProfile* hist, TString inputfile){   
     //TFile* input = TFile::Open("outputMC.root","READ");
     TFile* input = TFile::Open(inputfile,"READ");
     input->cd();
     if(input==0)
     {
              cout << "In CorrectEvtSel: Could not retrieve '"+inputfile+"' for making corrections. Exiting now ..." << endl;
              return false;
     }
    
     TProfile* truegenP = (TProfile*) input->Get("UEPlots_truegenjet1part05/profile_"+UEcoll+"_trans_truegenjet1part05");
     TProfile* genP = (TProfile*) input->Get("UEPlots_genjet1part05/profile_"+UEcoll+"_trans_genjet1part05");   
        
     if(truegenP == 0 || genP == 0)
     {    
         cout << "In CorrectEvtSel: Couldn't find one of the TProfiles. Exiting now ..." << endl;
         return false;      
     }

     TH1D* truegen = truegenP->ProjectionX();
     TH1D* gen     = genP->ProjectionX();
     
     truegen_gen_ratio->Divide(truegen, gen, 1, 1, "B");
     delete profile_corrected_evtsel;
     profile_corrected_evtsel = hist->ProjectionX();
     profile_corrected_evtsel->SetName("profile_corrected_evtsel");
     profile_corrected_evtsel->SetTitle("profile_corrected_evtsel");
     
     for (int i = 1; i <= nobins; i++)
         profile_corrected_evtsel->SetBinContent(i, (truegen_gen_ratio->GetBinContent(i))*(profile_corrected_evtsel->GetBinContent(i)));           
         
     //truegen_gen_ratio->Draw();
     //profile_corrected_true->Multiply(profile_corrected, truegen_gen_ratio, 1, 1);
     //profile_corrected_true->Multiply(profile_corrected_true, truegen_gen_ratio, 1, 1);
     //input->Close();
     return true;
}

void UECorrection::write(){
    gDirectory->cd("../macro2");
    gDirectory->mkdir("UECorrection_<"+UEcoll+">");
    gDirectory->cd("UECorrection_<"+UEcoll+">");
    
    pt_corr->Write();
    
    profile_corrected_matrix->Write();
    profile_corrected_evtsel->Write();
    profile_corrected_genreco->Write();
    gen_reco_ratio->Write();
    truegen_gen_ratio->Write();
    probability->Write();
    profile_corrected_matrix_normalised->Write();
    
    gDirectory->cd("../../plugins");  
}

void UECorrection::write(TString fname){
    gDirectory->cd("../macro2");
    gDirectory->mkdir("UECorrection_<"+UEcoll+fname+">");
    gDirectory->cd("UECorrection_<"+UEcoll+fname+">");
    
    pt_corr->Write();
    
    profile_corrected_matrix->Write();
    profile_corrected_evtsel->Write();
    profile_corrected_genreco->Write();
    gen_reco_ratio->Write();
    truegen_gen_ratio->Write();
    probability->Write();
    profile_corrected_matrix_normalised->Write();
    
    gDirectory->cd("../../plugins");  
}

//double deg2rad(double deg){
//  return (deg * pi / 180);
//}
//
//double mod(double num){
//  if (num>=0) return num;
//  else return -num;
//}
