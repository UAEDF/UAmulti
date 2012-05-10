#include "UEFactors.h"

#define pi 3.14159265358979323846

//double deg2rad(double);
//double mod(double);

ClassImp(UEFactors)

UEFactors::UEFactors(){
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

UEFactors::UEFactors(TString collname){
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

UEFactors::UEFactors(TString collname, vector<double> binning){
  UEcoll = collname;
  varbins = &(binning.at(0));
  nobins = binning.size()-1;
  //for (int i = 0; i < nobins+1; i++)
  //    cout<< varbins[i] << endl;
  //cout<< nBins << endl;
  this->init();
}

UEFactors::~UEFactors(){ }

void UEFactors::init(){ 
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
    
  
  //jets  
  dpt_jetmax = new TH1F("dpt_jetmax", "dpt_jetmax;dpt;nJets" , 120 , -10 , 12);
  deta_jetmax = new TH1F("deta_jetmax", "deta_jetmax;deta;nJets" , 120 , -3.0 , 3.0);
  dphi_jetmax = new TH1F("dphi_jetmax", "dphi_jetmax;dphi;nJets" , 120 , -3.2 , 3.2);
  dR_jetmax = new TH1F("dR_jetmax", "dR_jetmax;dR;nJets" , 120 , 0 , 3.2);
  dpt_jetmax_greater = new TH1F("dpt_jetmax_greater", "dpt_jetmax_greater;dpt;nJets" , 120 , -12 , 12);
  dpt_jetmax_smaller = new TH1F("dpt_jetmax_smaller", "dpt_jetmax_smaller;dpt;nJets" , 120 , -12 , 12);
  deta_jetmax_matched = new TH1F("deta_jetmax_matched", "deta_jetmax_matched;deta;nJets" , 120 , -3.0 , 3.0);
  deta_jetmax_unmatched = new TH1F("deta_jetmax_unmatched", "deta_jetmax_unmatched;deta;nJets" , 120 , -3.0 , 3.0);
  dphi_jetmax_matched = new TH1F("dphi_jetmax_matched", "dphi_jetmax_matched;dphi;nJets" , 120 , -3.2 , 3.2);
  dphi_jetmax_unmatched = new TH1F("dphi_jetmax_unmatched", "dphi_jetmax_unmatched;dphi;nJets" , 120 , -3.2 , 3.2);
  detadphi_jetmax_matched = new TH2F("detadphi_jetmax_matched", "detadphi_jetmax_matched;deta;dphi;nJets" , 120 , -3.2 , 3.2, 120, -3.2, 3.2);
  detadphi_jetmax_unmatched = new TH2F("detadphi_jetmax_unmatched", "detadphi_jetmax_unmatched;deta;dphi;nJets" , 120 , -3.2 , 3.2, 120, -3.2, 3.2);
  pt_gen_jetmax_num_unmatched = new TH1F("pt_gen_jetmax_num_unmatched", "pt_gen_jetmax_num_unmatched;TrackJet pt;nGenJets" , 200 , 0 , 100);
  eta_gen_jetmax_num_unmatched = new TH1F("eta_gen_jetmax_num_unmatched", "eta_gen_jetmax_num_unmatched;TrackJet eta;nGenJets" , 120 , -3.0 , 3.0);
  dR_matched = new TH1F("dR_matched", "dR_matched;dR;nJets" , 120 , -3.0 , 3.0);
  dR_unmatched = new TH1F("dR_unmatched", "dR_unmatched;dR;nJets" , 120 , -3.0 , 3.0);
  
  mult_jetloss = new TH1F("mult_jetloss", "mult_jetloss;Loss;Number of Events", 15, -5, 10);
  
  pt_gen_jetmax = new TH1F("pt_gen_jetmax", "pt_gen_jetmax;GenJet pt;nLeadGenJets" , 200 , 0 , 100);
  eta_gen_jetmax = new TH1F("eta_gen_jetmax", "eta_gen_jetmax;GenJet eta;nLeadGenJets" , 120 , -3.0 , 3.0);
  phi_gen_jetmax = new TH1F("phi_gen_jetmax", "phi_gen_jetmax;GenJet phi;nLeadJets" , 120 , -3.2 , 3.2);
  pt_reco_jetmax = new TH1F("pt_reco_jetmax", "pt_reco_jetmax;TrackJet pt;nLeadTrackJets" , 200 , 0 , 100);
  eta_reco_jetmax = new TH1F("eta_reco_jetmax", "eta_reco_jetmax;TrackJet eta;nLeadTrackJets" , 120 , -3.0 , 3.0);
  phi_reco_jetmax = new TH1F("phi_reco_jetmax", "phi_reco_jetmax;TrackJet phi;nLeadTrackJets" , 120 , -3.2 , 3.2);
  pt_gen_jetmax_num_found = new TH1F("pt_gen_jetmax_num_found", "pt_gen_jetmax_num_found;TrackJet pt;nGenJets" , 200 , 0 , 100);
  eta_gen_jetmax_num_found = new TH1F("eta_gen_jetmax_num_found", "eta_gen_jetmax_num_found;TrackJet eta;nGenJets" , 120 , -3.0 , 3.0);
  pt_gen_jetmax_perc_found = new TH1F("pt_gen_jetmax_perc_found", "pt_gen_jetmax_perc_found;TrackJet pt;% Jets" , 200, 0 , 100);
  eta_gen_jetmax_perc_found = new TH1F("eta_gen_jetmax_perc_found", "eta_gen_jetmax_perc_found;TrackJet eta;% Jets" , 120 , -3.0 , 3.0);
  
  PtTrackJet_PtGenJet_matched = new TH1F("PtTrackJet_PtGenJet_matched", "PtTrackJet/PtGenJet_matched;PtTrackJet/PtGenJet;Fraction of Leading TrackJets", 60, 0, 3);
  PtTrackJet_PtGenJet_unmatched = new TH1F("PtTrackJet_PtGenJet_unmatched", "PtTrackJet/PtGenJet_unmatched;PtTrackJet/PtGenJet;Fraction of Leading TrackJets", 60, 0, 3);
  profile_PtTrackJet_PtGenJet_matched = new TProfile("Profile_PtTrackJet_PtGenJet_matched", "Profile_PtTrackJet/PtGenJet_matched;PtTrackJet;PtTrackJet/PtGenJet", nobins, varbins, 0, 10000);
  profile_PtTrackJet_PtGenJet_unmatched = new TProfile("Profile_PtTrackJet_PtGenJet_unmatched", "Profile_PtTrackJet/PtGenJet_unmatched;PtTrackJet;PtTrackJet/PtGenJet", nobins, varbins, 0, 10000);
  
  //particles
  dpt_partmax = new TH1F("dpt_partmax", "dpt_partmax;dpt;nTracks" , 120 , -10 , 12);
  deta_partmax = new TH1F("deta_partmax", "deta_partmax;deta;nTracks" , 120 , -3.0 , 3.0);
  dphi_partmax = new TH1F("dphi_partmax", "dphi_partmax;dphi;nTracks" , 120 , -3.2 , 3.2);
  dR_partmax = new TH1F("dR_partmax", "dR_partmax;dR;nTracks" , 120 , 0 , 3.2);
  dpt_partmax_greater = new TH1F("dpt_partmax_greater", "dpt_partmax_greater;dpt;nTracks" , 120 , -12 , 12);
  dpt_partmax_smaller = new TH1F("dpt_partmax_smaller", "dpt_partmax_smaller;dpt;nTracks" , 120 , -12 , 12);
  
  pt_gen_partmax = new TH1F("pt_gen_partmax", "pt_gen_partmax;pt;nTracks" , 120 , 0 , 12);
  eta_gen_partmax = new TH1F("eta_gen_partmax", "eta_gen_partmax;eta;nTracks" , 120 , -3.0 , 3.0);
  phi_gen_partmax = new TH1F("phi_gen_partmax", "phi_gen_partmax;phi;nTracks" , 120 , -3.2 , 3.2);
  pt_trackmax = new TH1F("pt_trackmax", "pt_trackmax;pt;nTracks" , 120 , 0 , 12);
  eta_trackmax = new TH1F("eta_trackmax", "eta_trackmax;eta;nTracks" , 120 , -3.0 , 3.0);
  phi_trackmax = new TH1F("phi_trackmax", "phi_trackmax;phi;nTracks" , 120 , -3.2 , 3.2);
  pt_gen_partmax_num_found = new TH1F("pt_gen_partmax_num_found", "pt_gen_partmax_num_found;pt;nTracks" , 120 , 0 , 12);
  eta_gen_partmax_num_found = new TH1F("eta_gen_partmax_num_found", "eta_gen_partmax_num_found;eta;nTracks" , 120 , -3.0 , 3.0);
  pt_gen_partmax_perc_found = new TH1F("pt_gen_partmax_perc_found", "pt_gen_partmax_perc_found;pt;% Tracks" , 120 , 0 , 12);
  eta_gen_partmax_perc_found = new TH1F("eta_gen_partmax_perc_found", "eta_gen_partmax_perc_found;eta;% Tracks" , 120 , -3.0 , 3.0);
  
  //pt_corr = new TH2F("pt_corr", "pt_corr; pt_true; pt_reco", 30, 0, 30, 30, 0, 30);
  pt_corr = new TH2F("pt_corr", "pt_corr; pt_true; pt_reco", nobins, varbins, nobins, varbins);
  
  profile_corrected_matrix = new TProfile("profile_corrected_matrix_<"+UEcoll+">", "profile_corrected_matrix_<"+UEcoll+">;Leading jet pt;<"+UEcoll+">", nobins, varbins, 0, 10000);
  profile_corrected_truegengen = new TH1D("profile_corrected_truegengen_<"+UEcoll+">", "profile_corrected_truegengen_<"+UEcoll+">;Leading jet pt;<"+UEcoll+">", nobins, varbins);
  profile_corrected_genreco = new TH1D("profile_corrected_genreco_<"+UEcoll+">", "profile_corrected_genreco_<"+UEcoll+">;Leading jet pt;<"+UEcoll+">", nobins, varbins);
  gen_reco_ratio = new TH1D("gen_reco_mult_ratio_<"+UEcoll+">", "gen_reco_mult_ratio_<"+UEcoll+">;Leading jet pt;<# gen>/<# reco>", nobins, varbins);
  truegen_gen_ratio = new TH1D("truegen_gen_mult_ratio_<"+UEcoll+">", "truegen_gen_mult_ratio_<"+UEcoll+">;Leading jet pt;<# truegen>/<# gen>", nobins, varbins);
  
  iEvt = 0;
  //for (int i = 0; i < 27; i++)
  //    cout<< varbins[i] << endl;
}

void UEFactors::fill(vector<MyBaseJet>& genjet, vector<MyBaseJet>& recojet, double pt_cut, double eta_cut, double EvtWeight){
     
    iEvt++; 
    
    int ilead_gen_jet = -1;           //leading gen jet pointer
    int ilead_gen_jet_temp = 0;
    double prevgenjetpt = 0;
    //cout<< genjet.size() << " genjets:" << endl;
    for(vector<MyBaseJet>::iterator it_jet = genjet.begin() ; it_jet != genjet.end() ; ++it_jet,++ilead_gen_jet_temp)
    {                                    
        if (it_jet->Pt() == 0){cout<< "pt 0 "; continue;}
        if (it_jet->Pt() < pt_cut){cout<< "pt out of cut "; continue;}
        if (fabs(it_jet->Eta()) > eta_cut){cout<< "eta out of cut "; continue;}
        
        if (it_jet->Pt() > prevgenjetpt)
        {  
          prevgenjetpt = it_jet->Pt();
          ilead_gen_jet = ilead_gen_jet_temp;
        }
    }  

/*   
    if (ilead_gen_jet != -1)
    {
        cout<< iEvt << " " << genjet.at(ilead_gen_jet).Pt() << ", ";
        cout<< iEvt << " " << genjet.at(ilead_gen_jet).Eta() << endl;
    }
    else
        cout<< iEvt << " " << "0, ";
*/

    if((ilead_gen_jet != -1) && (fabs(genjet.at(ilead_gen_jet).Eta()) <= eta_cut) && (genjet.at(ilead_gen_jet).Pt() >= pt_cut))  
    {  
        pt_gen_jetmax->Fill(genjet.at(ilead_gen_jet).Pt(), EvtWeight);        
        eta_gen_jetmax->Fill(genjet.at(ilead_gen_jet).Eta(), EvtWeight);
        phi_gen_jetmax->Fill(genjet.at(ilead_gen_jet).Phi(), EvtWeight);          
    }
    
    double prevrecojetpt = 0;
    int ilead_reco_jet = -1;             //leading reco jet pointer
    int ilead_reco_jet_temp = 0;
    //cout<< recojet.size() << " recojets:" << endl;
    for(vector<MyBaseJet>::iterator it_jet = recojet.begin() ; it_jet != recojet.end() ; ++it_jet,++ilead_reco_jet_temp)
    {
        if (it_jet->Pt() == 0){cout<< "pt 0 "; continue;}
        if (it_jet->Pt() < pt_cut){cout<< "pt out of cut "; continue;}
        if (fabs(it_jet->Eta()) > eta_cut){cout<< "eta out of cut "; continue;}
        
        if (it_jet->Pt() > prevrecojetpt)
        {   
           prevrecojetpt = it_jet->Pt();
           ilead_reco_jet = ilead_reco_jet_temp;
        }
    }   

/*    
    if (ilead_reco_jet != -1)
    {
        cout<< iEvt << " " << recojet.at(ilead_reco_jet).Pt() << ", ";
        cout<< iEvt << " " << recojet.at(ilead_reco_jet).Eta() << endl;
    }
    else
        cout<< iEvt << " " << "0" << endl;
*/
        
    if((ilead_reco_jet != -1) && (fabs(recojet.at(ilead_reco_jet).Eta()) <= eta_cut) && (recojet.at(ilead_reco_jet).Pt() >= pt_cut))  
    {  
        pt_reco_jetmax->Fill(recojet.at(ilead_reco_jet).Pt(), EvtWeight);        
        eta_reco_jetmax->Fill(recojet.at(ilead_reco_jet).Eta(), EvtWeight);
        phi_reco_jetmax->Fill(recojet.at(ilead_reco_jet).Phi(), EvtWeight);          
    }

    int ngenjet = genjet.size();
    int nrecojet = recojet.size();
    
    mult_jetloss->Fill(ngenjet-nrecojet, EvtWeight);
    //mult_jetloss->Fill(genjet.size()-recojet.size());
/*
    if ((ilead_reco_jet != -1) && (ilead_gen_jet != -1) && (fabs(recojet.at(ilead_reco_jet).Eta()) <= eta_cut) && (recojet.at(ilead_reco_jet).Pt() >= pt_cut)
        && (fabs(genjet.at(ilead_gen_jet).Eta()) <= eta_cut) && (genjet.at(ilead_gen_jet).Pt() >= pt_cut))             
    {
        double dpt = genjet.at(ilead_gen_jet).Pt() - recojet.at(ilead_reco_jet).Pt();
        double deta = genjet.at(ilead_gen_jet).Eta() - recojet.at(ilead_reco_jet).Eta();
        double dphi = genjet.at(ilead_gen_jet).Phi() - recojet.at(ilead_reco_jet).Phi();
        double dR = sqrt(deta*deta + dphi*dphi);
        
        dpt_jetmax->Fill(dpt);        
        deta_jetmax->Fill(deta);
        dphi_jetmax->Fill(dphi);
        dR_jetmax->Fill(dR);
        
        if (dR > 0.5)
        {
           PtTrackJet_PtGenJet_unmatched->Fill((recojet.at(ilead_reco_jet).Pt())/(genjet.at(ilead_gen_jet).Pt()));
           profile_PtTrackJet_PtGenJet_unmatched->Fill(genjet.at(ilead_gen_jet).Pt(), (recojet.at(ilead_reco_jet).Pt())/(genjet.at(ilead_gen_jet).Pt()));
       
           dpt_jetmax_greater->Fill(dpt);
           deta_jetmax_unmatched->Fill(deta);
           dphi_jetmax_unmatched->Fill(dphi);
           detadphi_jetmax_unmatched->Fill(deta, dphi);
           dR_unmatched->Fill(dR);
           pt_gen_jetmax_num_unmatched->Fill(genjet.at(ilead_gen_jet).Pt());
           eta_gen_jetmax_num_unmatched->Fill(genjet.at(ilead_gen_jet).Eta());
        }
        else
        {
          PtTrackJet_PtGenJet_matched->Fill((recojet.at(ilead_reco_jet).Pt())/(genjet.at(ilead_gen_jet).Pt()));
          profile_PtTrackJet_PtGenJet_matched->Fill(genjet.at(ilead_gen_jet).Pt(), (recojet.at(ilead_reco_jet).Pt())/(genjet.at(ilead_gen_jet).Pt()));
       
          dpt_jetmax_smaller->Fill(dpt);
          deta_jetmax_matched->Fill(deta);
          dphi_jetmax_matched->Fill(dphi);
          detadphi_jetmax_matched->Fill(deta, dphi);
          dR_matched->Fill(dR);
          pt_gen_jetmax_num_found->Fill(genjet.at(ilead_gen_jet).Pt());
          eta_gen_jetmax_num_found->Fill(genjet.at(ilead_gen_jet).Eta());
          pt_gen_jetmax_perc_found->Divide(pt_gen_jetmax_num_found, pt_gen_jetmax, 1, 1);
          eta_gen_jetmax_perc_found->Divide(eta_gen_jetmax_num_found, eta_gen_jetmax, 1, 1);
          pt_corr->Fill(genjet.at(ilead_gen_jet).Pt(), recojet.at(ilead_reco_jet).Pt());         
        }
    }  
*/

    double dR_closest = -1;
    int iClosest_dR = -1;
    int iClosest_dR_temp = 0;
    if ((ilead_reco_jet != -1) && (fabs(recojet.at(ilead_reco_jet).Eta()) <= eta_cut) && (recojet.at(ilead_reco_jet).Pt() >= pt_cut))
    {
        for(vector<MyBaseJet>::iterator it_jet = genjet.begin() ; it_jet != genjet.end() ; ++it_jet,++iClosest_dR_temp)
        {          
            if (it_jet->Pt() == 0) continue;
            if (it_jet->Pt() <= pt_cut) continue;
            if (fabs(it_jet->Eta()) > eta_cut) continue;
          
            double dpt = it_jet->Pt() - recojet.at(ilead_reco_jet).Pt();
            double deta = it_jet->Eta() - recojet.at(ilead_reco_jet).Eta();
            double dphi = it_jet->Phi() - recojet.at(ilead_reco_jet).Phi();
            double dR = sqrt(deta*deta + dphi*dphi);
            
            dpt_jetmax->Fill(dpt, EvtWeight);        
            deta_jetmax->Fill(deta, EvtWeight);
            dphi_jetmax->Fill(dphi, EvtWeight);
            dR_jetmax->Fill(dR, EvtWeight);
            //cout<< dR << "  ";
            if ((dR <= dR_closest) || (dR_closest == -1))
            {   
                dR_closest = dR;  
                iClosest_dR = iClosest_dR_temp;
                //cout<< endl << "Closest: " << dR_closest << "  ";
            }             
        }
        
        int iMatched = -1;
        int iMatched_temp = 0;
        if (dR_closest <= 0.5)
           iMatched = iClosest_dR;
        for(vector<MyBaseJet>::iterator it_jet = genjet.begin() ; it_jet != genjet.end() ; ++it_jet, ++iMatched_temp)
        {
            if (it_jet->Pt() == 0) continue;
            if (it_jet->Pt() <= pt_cut) continue;
            if (fabs(it_jet->Eta()) > eta_cut) continue;
          
            double dpt = it_jet->Pt() - recojet.at(ilead_reco_jet).Pt();
            double deta = it_jet->Eta() - recojet.at(ilead_reco_jet).Eta();
            double dphi = it_jet->Phi() - recojet.at(ilead_reco_jet).Phi();
            double dR = sqrt(deta*deta + dphi*dphi);
            
            if (iMatched_temp != iMatched)
            {
               //cout<< "UN: " << dR << "  ";
               PtTrackJet_PtGenJet_unmatched->Fill((recojet.at(ilead_reco_jet).Pt())/(it_jet->Pt()), EvtWeight);
               profile_PtTrackJet_PtGenJet_unmatched->Fill(recojet.at(ilead_reco_jet).Pt(), (recojet.at(ilead_reco_jet).Pt())/(it_jet->Pt()), EvtWeight);
                
               dpt_jetmax_greater->Fill(dpt, EvtWeight);
               deta_jetmax_unmatched->Fill(deta, EvtWeight);
               dphi_jetmax_unmatched->Fill(dphi, EvtWeight);
               detadphi_jetmax_unmatched->Fill(deta, dphi, EvtWeight);
               dR_unmatched->Fill(dR, EvtWeight);
               pt_gen_jetmax_num_unmatched->Fill(recojet.at(ilead_reco_jet).Pt(), EvtWeight);
               eta_gen_jetmax_num_unmatched->Fill(recojet.at(ilead_reco_jet).Eta(), EvtWeight);
            }
            else
            {  
               //cout<< "M: " << dR << "  ";
               PtTrackJet_PtGenJet_matched->Fill((recojet.at(ilead_reco_jet).Pt())/(genjet.at(iMatched).Pt()), EvtWeight);
               profile_PtTrackJet_PtGenJet_matched->Fill(recojet.at(ilead_reco_jet).Pt(), (recojet.at(ilead_reco_jet).Pt())/(genjet.at(iMatched).Pt()), EvtWeight);
               //cout<< "reco pt: " << recojet.at(ilead_reco_jet).Pt() << " gen pt: " << genjet.at(iMatched).Pt() << " ratio: " << (recojet.at(ilead_reco_jet).Pt())/(genjet.at(iMatched).Pt()) << endl;
               dpt_jetmax_smaller->Fill(dpt, EvtWeight);
               deta_jetmax_matched->Fill(deta, EvtWeight);
               dphi_jetmax_matched->Fill(dphi, EvtWeight);
               detadphi_jetmax_matched->Fill(deta, dphi, EvtWeight);
               dR_matched->Fill(dR, EvtWeight);
               pt_gen_jetmax_num_found->Fill(recojet.at(ilead_reco_jet).Pt(), EvtWeight);
               eta_gen_jetmax_num_found->Fill(recojet.at(ilead_reco_jet).Eta(), EvtWeight);
               pt_gen_jetmax_perc_found->Divide(pt_gen_jetmax_num_found, pt_reco_jetmax, EvtWeight, 1);
               eta_gen_jetmax_perc_found->Divide(eta_gen_jetmax_num_found, eta_reco_jetmax, EvtWeight, 1);
               pt_corr->Fill(genjet.at(iMatched).Pt(), recojet.at(ilead_reco_jet).Pt(), EvtWeight);
              
            }
        }  
    }
    //cout<< endl;
}

void UEFactors::fill(vector<MyGenPart>& genpart, vector<MyTracks>& tracks, double pt_cut, double eta_cut, double EvtWeight)
{
    int ilead_gen_part = -1;           //leading gen jet pointer
    int ilead_gen_part_temp = 0;
    double prevgenpartpt = 0;
    
    for(vector<MyGenPart>::iterator it_genpart = genpart.begin() ; it_genpart != genpart.end() ; ++it_genpart,++ilead_gen_part_temp)
    {                                    
        if (it_genpart->Pt() == 0) continue;
        
        if ((it_genpart->Pt() > prevgenpartpt) && (fabs(it_genpart->Eta()) <= eta_cut))
        {  
          prevgenpartpt = it_genpart->Pt();
          ilead_gen_part = ilead_gen_part_temp;
        }
    }      
    
    if((ilead_gen_part != -1) && (fabs(genpart.at(ilead_gen_part).Eta()) <= eta_cut) && (genpart.at(ilead_gen_part).Pt() >= pt_cut))  
    {  
        pt_gen_partmax->Fill(genpart.at(ilead_gen_part).Pt());        
        eta_gen_partmax->Fill(genpart.at(ilead_gen_part).Eta());
        phi_gen_partmax->Fill(genpart.at(ilead_gen_part).Phi());          
    }
    
    double prevtrackpt = 0;
    int ilead_track = -1;             //leading reco jet pointer
    int ilead_track_temp = 0;
    
    for(vector<MyTracks>::iterator it_track = tracks.begin() ; it_track != tracks.end() ; ++it_track,++ilead_track_temp)
    {
        if (it_track->Pt() == 0) continue;
        
        if ((it_track->Pt() > prevtrackpt) && (fabs(it_track->Eta()) <= eta_cut))
        {   
           prevtrackpt = it_track->Pt();
           ilead_track = ilead_track_temp;
        }
    }   
    
    if((ilead_track != -1) && (fabs(tracks.at(ilead_track).Eta()) <= eta_cut) && (tracks.at(ilead_track).Pt() >= pt_cut))  
    {  
        pt_trackmax->Fill(tracks.at(ilead_track).Pt());        
        eta_trackmax->Fill(tracks.at(ilead_track).Eta());
        phi_trackmax->Fill(tracks.at(ilead_track).Phi());          
    }
    
    if ((ilead_track != -1) && (ilead_gen_part != -1) && (fabs(tracks.at(ilead_track).Eta()) <= eta_cut) && (tracks.at(ilead_track).Pt() >= pt_cut)
        && (fabs(genpart.at(ilead_gen_part).Eta()) <= eta_cut) && (genpart.at(ilead_gen_part).Pt() >= pt_cut))             
    {
        double dpt = genpart.at(ilead_gen_part).Pt() - tracks.at(ilead_track).Pt();
        double deta = genpart.at(ilead_gen_part).Eta() - tracks.at(ilead_track).Eta();
        double dphi = genpart.at(ilead_gen_part).Phi() - tracks.at(ilead_track).Phi();
        double dR = sqrt(deta*deta + dphi*dphi);
        
        dpt_partmax->Fill(dpt);        
        deta_partmax->Fill(deta);
        dphi_partmax->Fill(dphi);
        dR_partmax->Fill(dR);
        
        if (dR > 0.5)
          dpt_partmax_greater->Fill(dpt);
        else
        {
          dpt_partmax_smaller->Fill(dpt);
          pt_gen_partmax_num_found->Fill(genpart.at(ilead_gen_part).Pt());
          eta_gen_partmax_num_found->Fill(genpart.at(ilead_gen_part).Eta());
          pt_gen_partmax_perc_found->Divide(pt_gen_partmax_num_found, pt_gen_partmax, 1, 1);
          eta_gen_partmax_perc_found->Divide(eta_gen_partmax_num_found, eta_gen_partmax, 1, 1);
          //pt_corr->Fill(genjet.at(ilead_gen_jet).Pt(), recojet.at(ilead_reco_jet).Pt());
          
        }
    }  
}

void UEFactors::correctgenreco(TProfile* hist, TProfile* gen, TProfile* reco){
     //for (int i = 0; i < nobins; i++)
     //    cout<< varbins[i] << endl;
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
     }           
}

void UEFactors::correctgenreco(TProfile* hist){
     
     //delete profile_corrected_genreco;
     profile_corrected_genreco = hist->ProjectionX();
     profile_corrected_genreco->SetName("profile_corrected_genreco");
     profile_corrected_genreco->SetTitle("profile_corrected_genreco");
     
     for (int i = 1; i <= nobins; i++)
     {
         //cout<< gen_reco_ratio->GetBinContent(i) << "   " << profile_temp->GetBinContent(i) << endl;
         double ratiotemp = gen_reco_ratio->GetBinContent(i);
         double temptemp = profile_corrected_genreco->GetBinContent(i);
         profile_corrected_genreco->SetBinContent(i, ratiotemp*temptemp);  
     }         
}

void UEFactors::correctmatrix(TProfile* hist){
     
     double pttruefrac;
     int totalno = 0;
     TAxis *xaxis = profile_corrected_matrix->GetXaxis();
     double xval;
     
     for (int i = 1; i <= nobins; i++)
     {
         double temp = hist->GetBinContent(i);
         
         for (int j = 1; j <= nobins; j++)
             totalno += pt_corr->GetBinContent(j, i);

         //cout<< totalno << endl;
         if (totalno == 0) continue;
         
         for (int j = 1; j <= nobins; j++)
         {
             pttruefrac = pt_corr->GetBinContent(j, i)/totalno;
             //cout<< "i: " << i << "  j: " << j << "  Ptbin: " << varbins[j] << "  profileval: " << temp << "  corr: " << pt_corr->GetBinContent(j+1, i+1) << "  totalno: " << totalno << "  frac: " << pttruefrac << endl;
             //profile_corrected_matrix->Fill(varbins[j], temp, pttruefrac);    
             xval = xaxis->GetBinCenter(j);
             //cout<< "i: " << i << "  j: " << j << "  binPt: " << xval << "  profileval: " << temp << "  corr: " << pt_corr->GetBinContent(j, i) << "  totalno: " << totalno << "  frac: " << pttruefrac << endl;
             profile_corrected_matrix->Fill(xval, temp, pttruefrac); 
         }
        
         totalno = 0;
     }
     //profile_corrected_matrix->Draw();
}

void UEFactors::correcttruegengen(TProfile* hist, TProfile* truegenP, TProfile* genP){
     //truegen->Draw();
     //gen->Draw();     
     
     TH1D* truegen = truegenP->ProjectionX();
     TH1D* gen     = genP->ProjectionX();
     
     //TH1D* ratio_temp = ratio_temp->Divide(truegen, gen, 1, 1, "B");
     //truegen_gen_ratio = (TProfile*) ratio_temp->Clone("truegen_gen_ratio");
     //truegen_gen_ratio->Divide(truegenP, genP, 1, 1, "B");
     truegen_gen_ratio->Divide(truegen, gen, 1, 1, "B");
     delete profile_corrected_truegengen;
     profile_corrected_truegengen = hist->ProjectionX();
     profile_corrected_truegengen->SetName("profile_corrected_truegengen");
     profile_corrected_truegengen->SetTitle("profile_corrected_truegengen");
     
     for (int i = 1; i <= nobins; i++)
         profile_corrected_truegengen->SetBinContent(i, (truegen_gen_ratio->GetBinContent(i))*(profile_corrected_truegengen->GetBinContent(i)));           
         
     //truegen_gen_ratio->Draw();
     //profile_corrected_true->Multiply(profile_corrected, truegen_gen_ratio, 1, 1);
     //profile_corrected_true->Multiply(profile_corrected_true, truegen_gen_ratio, 1, 1);
     
}

void UEFactors::correcttruegengen(TProfile* hist){
     
     //delete profile_corrected_truegengen;
     profile_corrected_truegengen = hist->ProjectionX();
     profile_corrected_truegengen->SetName("profile_corrected_truegengen");
     profile_corrected_truegengen->SetTitle("profile_corrected_truegengen");
     
     for (int i = 1; i <= nobins; i++)
         profile_corrected_truegengen->SetBinContent(i, (truegen_gen_ratio->GetBinContent(i))*(profile_corrected_truegengen->GetBinContent(i)));           
         
     //truegen_gen_ratio->Draw();
     //profile_corrected_true->Multiply(profile_corrected, truegen_gen_ratio, 1, 1);
     //profile_corrected_true->Multiply(profile_corrected_true, truegen_gen_ratio, 1, 1);
     
}

//template<class T>
//int UEFactors::getClosestMatch(vector<T>& base, vector<T>& assoc, double pt_cut, double eta_cut)
//{
     
//}

void UEFactors::write(){
    gDirectory->cd("../macro2");
    gDirectory->mkdir("UEFactors_<"+UEcoll+">");
    gDirectory->cd("UEFactors_<"+UEcoll+">");
     
    dpt_jetmax->Write();
    deta_jetmax->Write();
    dphi_jetmax->Write();
    dR_jetmax->Write();
    dpt_jetmax_greater->Write();
    dpt_jetmax_smaller->Write();
    deta_jetmax_matched->Write();
    deta_jetmax_unmatched->Write();
    dphi_jetmax_matched->Write();
    dphi_jetmax_unmatched->Write();
    detadphi_jetmax_matched->Write();
    detadphi_jetmax_unmatched->Write();
    dR_matched->Write();
    dR_unmatched->Write();
    pt_gen_jetmax->Write();
    eta_gen_jetmax->Write();
    phi_gen_jetmax->Write();
    pt_reco_jetmax->Write();
    eta_reco_jetmax->Write();
    phi_reco_jetmax->Write();
    pt_gen_jetmax_num_found->Write();
    pt_gen_jetmax_num_unmatched->Write();
    eta_gen_jetmax_num_found->Write();
    eta_gen_jetmax_num_unmatched->Write();
    pt_gen_jetmax_perc_found->Write();
    eta_gen_jetmax_perc_found->Write();
    PtTrackJet_PtGenJet_matched->Write();
    PtTrackJet_PtGenJet_unmatched->Write();
    profile_PtTrackJet_PtGenJet_matched->Write();
    profile_PtTrackJet_PtGenJet_unmatched->Write();
    mult_jetloss->Write();
    
    dpt_partmax->Write();
    deta_partmax->Write();
    dphi_partmax->Write();
    dR_partmax->Write();
    dpt_partmax_greater->Write();
    dpt_partmax_smaller->Write();
    pt_gen_partmax->Write();
    eta_gen_partmax->Write();
    phi_gen_partmax->Write();
    pt_trackmax->Write();
    eta_trackmax->Write();
    phi_trackmax->Write();
    pt_gen_partmax_num_found->Write();
    eta_gen_partmax_num_found->Write();
    pt_gen_partmax_perc_found->Write();
    eta_gen_partmax_perc_found->Write();
    
    pt_corr->Write();
    
    profile_corrected_matrix->Write();
    profile_corrected_truegengen->Write();
    profile_corrected_genreco->Write();
    gen_reco_ratio->Write();
    truegen_gen_ratio->Write();
    
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
