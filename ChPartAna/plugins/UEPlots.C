#include "UEPlots.h"

#define pi 3.14159265358979323846

double deg2rad(double);
double mod(double);

ClassImp(UEPlots)

UEPlots::UEPlots(){
  UEcoll = "none";
  nBins = 26;
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

UEPlots::UEPlots(TString collname){
  UEcoll = collname;
  nBins = 26;
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

UEPlots::UEPlots(TString collname, vector<double> binning){
  UEcoll = collname;
  varbins = &(binning.at(0));
  nBins = binning.size()-1;
  //cout<< nBins << endl;
  this->init();
}

UEPlots::~UEPlots(){ }

void UEPlots::init(){
     TH1::SetDefaultSumw2(1);
    
    //double* varbins;
    /*varbins = new double [27];
    for (int i = 0; i < 14; i++)
        varbins[i] = i;
    for (int i = 0; i < 5; i++)
        varbins[14+i] = 14+i*2;
    for (int i = 0; i < 3; i++)
        varbins[19+i] = 24+i*4;
    for (int i = 0; i < 3; i++)
        varbins[22+i] = 40+i*8;
    varbins[25] = 64; 
    varbins[26] = 100;*/
    
    pt_lead_part = new TH1F("pt_lead_part_"+UEcoll, "pt_lead_part_"+UEcoll+";Leading part Pt;# Ch", 40, 0, 40);
    pt_lead_jet = new TH1F("pt_lead_jet_"+UEcoll, "pt_lead_jet_"+UEcoll+";Leading jet Pt;# Ch jets", 40, 0, 40);
    eta_lead_part = new TH1F("eta_lead_part_"+UEcoll, "eta_lead_part_"+UEcoll+";Leading part Eta;# Ch", 30, -3.0, 3.0);
    eta_lead_jet = new TH1F("eta_lead_jet_"+UEcoll, "eta_lead_jet_"+UEcoll+";Leading jet Eta;# Ch jets", 30, -3.0, 3.0);
    phi_lead_part = new TH1F("phi_lead_part_"+UEcoll, "phi_lead_part_"+UEcoll+";Leading part Phi;# Ch", 32, -3.2, 3.2);
    phi_lead_jet = new TH1F("phi_lead_jet_"+UEcoll, "phi_lead_jet_"+UEcoll+";Leading jet Phi;# Ch jets", 32, -3.2, 3.2); 
    pt_nonlead_part = new TH1F("pt_nonlead_part_"+UEcoll, "pt_nonlead_part_"+UEcoll+";Part Pt w/o leading part;# Ch", 40, 0, 40);
    pt_nonlead_jet = new TH1F("pt_nonlead_jet_"+UEcoll, "pt_nonlead_jet_"+UEcoll+";Jet Pt w/o leading jet;# Ch jets", 40, 0, 40);
    eta_nonlead_part = new TH1F("eta_nonlead_part_"+UEcoll, "eta_nonlead_part_"+UEcoll+";Part Eta w/o leading part;# Ch", 30, -3.0, 3.0);
    eta_nonlead_jet = new TH1F("eta_nonlead_jet_"+UEcoll, "eta_nonlead_jet_"+UEcoll+"Jet Eta w/o leading jet;# Ch jets", 30, -3.0, 3.0);
    phi_nonlead_part = new TH1F("phi_nonlead_part_"+UEcoll, "phi_nonlead_part_"+UEcoll+";Part Phi w/o leading part;# Ch", 32, -3.2, 3.2);
    phi_nonlead_jet = new TH1F("phi_nonlead_jet_"+UEcoll, "phi_nonlead_jet_"+UEcoll+";Jet Phi w/o leading jet;# Ch", 32, -3.2, 3.2); 
    dR_LeadPart_LeadJet = new TH1F("dR_LeadPart_LeadJet", "dR_LeadPart_LeadJet;dR;nLeads" , 120 , 0 , 3.2);
    deta_LeadPart_LeadJet = new TH1F("deta_LeadPart_LeadJet", "deta_LeadPart_LeadJet;deta;nLeads" , 120 , -3.2 , 3.2);
    dphi_LeadPart_LeadJet = new TH1F("dphi_LeadPart_LeadJet", "dphi_LeadPart_LeadJet;dphi;nLeads" , 120 , -3.2 , 3.2);
    detadphi_LeadPart_LeadJet = new TH2F("detadphi_LeadPart_LeadJet", "detadphi_LeadPart_LeadJet;deta;dphi;nLeads" , 120 , -3.2 , 3.2, 120 , -3.2 , 3.2);
    PtLeadPart_PtLeadJet = new TH1F("PtLeadPart_PtLeadJet", "PtLeadPart/PtLeadJet;PtLeadPart/PtLeadJet;Fraction of Leading Jets", 30, 0, 1.5);
    profile_PtLeadPart_PtLeadJet = new TProfile("Profile_PtLeadPart_PtLeadJet", "Profile_PtLeadPart/PtLeadJet;PtLeadJet;PtLeadPart/PtLeadJet", 40, 0, 20); 
    
    weight_dist = new TH1F("weight_dist_"+UEcoll, "weight_dist_"+UEcoll+";Weight;# Tracks", 300, 0, 3);
  //---Uncorrected---
  dphi = new TH1F("dphi_"+UEcoll, "dphi_"+UEcoll+";dphi;# Tracks", 240, -6.4, 6.4);   
  n_mult_forward = new TH1F("#ch_forward_"+UEcoll, "#ch_forward_"+UEcoll+";# Forward Particles;# Events", 30, 0, 30);
  n_mult_trans = new TH1F("#ch_trans_"+UEcoll, "#ch_trans_"+UEcoll+";# Transverse Particles;# Events", 30, 0, 30);
  n_mult_away = new TH1F("#ch_away_"+UEcoll, "#ch_away_"+UEcoll+";# Backward Particles;# Events", 30, 0, 30);
  n_mult_total = new TH1F("#ch_total_"+UEcoll, "#ch_total_"+UEcoll+";# All Particles;# Events", 30, 0, 30);
  ptsum_forward = new TH1F("ptsum_forward_"+UEcoll, "ptsum_forward_"+UEcoll+";Forward Pt Sum;# Events", 35, 0, 35);
  ptsum_trans = new TH1F("ptsum_trans_"+UEcoll, "ptsum_trans_"+UEcoll+";Transverse Pt Sum;# Events", 35, 0, 35);
  ptsum_away = new TH1F("ptsum_away_"+UEcoll, "ptsum_away_"+UEcoll+";Backward Pt Sum;# Events", 35, 0, 35);
  ptsum_total = new TH1F("ptsum_total_"+UEcoll, "ptsum_total_"+UEcoll+";All Pt Sum;# Events", 35, 0, 35);
  pt_spectra_forward = new TH1F("pt_spectra_forward_"+UEcoll, "pt_spectra_forward_"+UEcoll+";Forward Pt;# Ch", 15, 0, 15);
  pt_spectra_trans = new TH1F("pt_spectra_trans_"+UEcoll, "pt_spectra_trans_"+UEcoll+";Tranverse Pt;# Ch", 15, 0, 15);
  pt_spectra_away = new TH1F("pt_spectra_away_"+UEcoll, "pt_spectra_away_"+UEcoll+";Backward Pt;# Ch", 15, 0, 15);
  pt_spectra_total = new TH1F("pt_spectra_total_"+UEcoll, "pt_spectra_total_"+UEcoll+";All Pt;# Ch", 15, 0, 15);

  profile_n_mult_forward = new TProfile("profile_#ch_forward_"+UEcoll, "profile_#ch_forward_"+UEcoll+";Leading jet pt;<# ch>", nBins, varbins, 0, 10000);
  profile_n_mult_trans = new TProfile("profile_#ch_trans_"+UEcoll, "profile_#ch_trans_"+UEcoll+";Leading jet pt;<# ch>", nBins, varbins, 0, 10000);
  profile_n_mult_away = new TProfile("profile_#ch_away_"+UEcoll, "profile_#ch_away_"+UEcoll+";Leading jet pt;<# ch>", nBins, varbins, 0, 10000);
  profile_n_mult_total = new TProfile("profile_#ch_total_"+UEcoll, "profile_#ch_total_"+UEcoll+";Leading jet pt;<# ch>", nBins, varbins, 0, 10000);
  profile_ptsum_forward = new TProfile("profile_ptsum_forward_"+UEcoll, "profile_ptsum_forward_"+UEcoll+";Leading jet pt;<ptsum>", nBins, varbins, 0, 10000);
  profile_ptsum_trans = new TProfile("profile_ptsum_trans_"+UEcoll, "profile_ptsum_trans_"+UEcoll+";Leading jet pt;<ptsum>", nBins, varbins, 0, 10000);
  profile_ptsum_away = new TProfile("profile_ptsum_away_"+UEcoll, "profile_ptsum_away_"+UEcoll+";Leading jet pt;<ptsum>", nBins, varbins, 0, 10000);
  profile_ptsum_total = new TProfile("profile_ptsum_total_"+UEcoll, "profile_ptsum_total_"+UEcoll+";Leading jet pt;<ptsum>", nBins, varbins, 0, 10000);
  profile_ptsum_n_mult_trans = new TProfile("profile_ptsum_#ch_trans_"+UEcoll, "profile_ptsum_#ch_trans_"+UEcoll+";Leading jet pt;<ptsum>/<# ch>", nBins, varbins, 0, 10000);
  
  //---Corrected for Jet Pt---  
  profile_n_mult_forward_jetcorr = new TProfile("profile_#ch_forward_jetcorr_"+UEcoll, "profile_#ch_forward_jetcorr_"+UEcoll+";Leading jet pt;<# ch>", nBins, varbins, 0, 10000);
  profile_n_mult_trans_jetcorr = new TProfile("profile_#ch_trans_jetcorr_"+UEcoll, "profile_#ch_trans_jetcorr_"+UEcoll+";Leading jet pt;<# ch>", nBins, varbins, 0, 10000);
  profile_n_mult_away_jetcorr = new TProfile("profile_#ch_away_jetcorr_"+UEcoll, "profile_#ch_away_jetcorr_"+UEcoll+";Leading jet pt;<# ch>", nBins, varbins, 0, 10000);
  profile_n_mult_total_jetcorr = new TProfile("profile_#ch_total_jetcorr_"+UEcoll, "profile_#ch_total_jetcorr_"+UEcoll+";Leading jet pt;<# ch>", nBins, varbins, 0, 10000);
  profile_ptsum_forward_jetcorr = new TProfile("profile_ptsum_forward_jetcorr_"+UEcoll, "profile_ptsum_forward_jetcorr_"+UEcoll+";Leading jet pt;<ptsum>", nBins, varbins, 0, 10000);
  profile_ptsum_trans_jetcorr = new TProfile("profile_ptsum_trans_jetcorr_"+UEcoll, "profile_ptsum_trans_jetcorr_"+UEcoll+";Leading jet pt;<ptsum>", nBins, varbins, 0, 10000);
  profile_ptsum_away_jetcorr = new TProfile("profile_ptsum_away_jetcorr_"+UEcoll, "profile_ptsum_away_jetcorr_"+UEcoll+";Leading jet pt;<ptsum>", nBins, varbins, 0, 10000);
  profile_ptsum_total_jetcorr = new TProfile("profile_ptsum_total_jetcorr_"+UEcoll, "profile_ptsum_total_jetcorr_"+UEcoll+";Leading jet pt;<ptsum>", nBins, varbins, 0, 10000);
  
  //---Corrected Track efficiency and Fake rates---    
  n_mult_forward_weighted = new TH1F("#ch_forward_weighted_"+UEcoll, "#ch_forward_weighted_"+UEcoll+";# Forward Particles;# Events", 30, 0, 30);
  n_mult_trans_weighted = new TH1F("#ch_trans_weighted_"+UEcoll, "#ch_trans_weighted_"+UEcoll+";# Transverse Particles;# Events", 30, 0, 30);
  n_mult_away_weighted = new TH1F("#ch_away_weighted_"+UEcoll, "#ch_away_weighted_"+UEcoll+";# Backward Particles;# Events", 30, 0, 30);
  n_mult_total_weighted = new TH1F("#ch_total_weighted_"+UEcoll, "#ch_total_weighted_"+UEcoll+";# All Particles;# Events", 30, 0, 30);
  ptsum_forward_weighted = new TH1F("ptsum_forward_weighted_"+UEcoll, "ptsum_forward_weighted_"+UEcoll+";Forward Pt Sum;# Events", 35, 0, 35);
  ptsum_trans_weighted = new TH1F("ptsum_trans_weighted_"+UEcoll, "ptsum_trans_weighted_"+UEcoll+";Transverse Pt Sum;# Events", 35, 0, 35);
  ptsum_away_weighted = new TH1F("ptsum_away_weighted_"+UEcoll, "ptsum_away_weighted_"+UEcoll+";Backward Pt Sum;# Events", 35, 0, 35);
  ptsum_total_weighted = new TH1F("ptsum_total_weighted_"+UEcoll, "ptsum_total_weighted_"+UEcoll+";All Pt Sum;# Events", 35, 0, 35);
  pt_spectra_forward_weighted = new TH1F("pt_spectra_forward_weighted_"+UEcoll, "pt_spectra_forward_weighted_"+UEcoll+";Forward Pt;# Ch", 15, 0, 15);
  pt_spectra_trans_weighted = new TH1F("pt_spectra_trans_weighted_"+UEcoll, "pt_spectra_trans_weighted_"+UEcoll+";Tranverse Pt;# Ch", 15, 0, 15);
  pt_spectra_away_weighted = new TH1F("pt_spectra_away_weighted_"+UEcoll, "pt_spectra_away_weighted_"+UEcoll+";Backward Pt;# Ch", 15, 0, 15);
  pt_spectra_total_weighted = new TH1F("pt_spectra_total_weighted_"+UEcoll, "pt_spectra_total_weighted_"+UEcoll+";All Pt;# Ch", 15, 0, 15);

  profile_n_mult_forward_weighted = new TProfile("profile_#ch_forward_weighted_"+UEcoll, "profile_#ch_forward_weighted_"+UEcoll+";Leading jet pt;<# ch>", nBins, varbins, 0, 10000);
  profile_n_mult_trans_weighted = new TProfile("profile_#ch_trans_weighted_"+UEcoll, "profile_#ch_trans_weighted_"+UEcoll+";Leading jet pt;<# ch>", nBins, varbins, 0, 10000);
  profile_n_mult_away_weighted = new TProfile("profile_#ch_away_weighted_"+UEcoll, "profile_#ch_away_weighted_"+UEcoll+";Leading jet pt;<# ch>", nBins, varbins, 0, 10000);
  profile_n_mult_total_weighted = new TProfile("profile_#ch_total_weighted_"+UEcoll, "profile_#ch_total_weighted_"+UEcoll+";Leading jet pt;<# ch>", nBins, varbins, 0, 10000);
  profile_ptsum_forward_weighted = new TProfile("profile_ptsum_forward_weighted_"+UEcoll, "profile_ptsum_forward_weighted_"+UEcoll+";Leading jet pt;<ptsum>", nBins, varbins, 0, 10000);
  profile_ptsum_trans_weighted = new TProfile("profile_ptsum_trans_weighted_"+UEcoll, "profile_ptsum_trans_weighted_"+UEcoll+";Leading jet pt;<ptsum>", nBins, varbins, 0, 10000);
  profile_ptsum_away_weighted = new TProfile("profile_ptsum_away_weighted_"+UEcoll, "profile_ptsum_away_weighted_"+UEcoll+";Leading jet pt;<ptsum>", nBins, varbins, 0, 10000);
  profile_ptsum_total_weighted = new TProfile("profile_ptsum_total_weighted_"+UEcoll, "profile_ptsum_total_weighted_"+UEcoll+";Leading jet pt;<ptsum>", nBins, varbins, 0, 10000);
  
  //---Corrected for Jet Pt, Track efficiency and Fake rates---  
  profile_n_mult_forward_jetcorr_weighted = new TProfile("profile_#ch_forward_jetcorr_weighted_"+UEcoll, "profile_#ch_forward_jetcorr_weighted_"+UEcoll+";Leading jet pt;<# ch>", nBins, varbins, 0, 10000);
  profile_n_mult_trans_jetcorr_weighted = new TProfile("profile_#ch_trans_jetcorr_weighted_"+UEcoll, "profile_#ch_trans_jetcorr_weighted_"+UEcoll+";Leading jet pt;<# ch>", nBins, varbins, 0, 10000);
  profile_n_mult_away_jetcorr_weighted = new TProfile("profile_#ch_away_jetcorr_weighted_"+UEcoll, "profile_#ch_away_jetcorr_weighted_"+UEcoll+";Leading jet pt;<# ch>", nBins, varbins, 0, 10000);
  profile_n_mult_total_jetcorr_weighted = new TProfile("profile_#ch_total_jetcorr_weighted_"+UEcoll, "profile_#ch_total_jetcorr_weighted_"+UEcoll+";Leading jet pt;<# ch>", nBins, varbins, 0, 10000);
  profile_ptsum_forward_jetcorr_weighted = new TProfile("profile_ptsum_forward_jetcorr_weighted_"+UEcoll, "profile_ptsum_forward_jetcorr_weighted_"+UEcoll+";Leading jet pt;<ptsum>", nBins, varbins, 0, 10000);
  profile_ptsum_trans_jetcorr_weighted = new TProfile("profile_ptsum_trans_jetcorr_weighted_"+UEcoll, "profile_ptsum_trans_jetcorr_weighted_"+UEcoll+";Leading jet pt;<ptsum>", nBins, varbins, 0, 10000);
  profile_ptsum_away_jetcorr_weighted = new TProfile("profile_ptsum_away_jetcorr_weighted_"+UEcoll, "profile_ptsum_away_jetcorr_weighted_"+UEcoll+";Leading jet pt;<ptsum>", nBins, varbins, 0, 10000);
  profile_ptsum_total_jetcorr_weighted = new TProfile("profile_ptsum_total_jetcorr_weighted_"+UEcoll, "profile_ptsum_total_jetcorr_weighted_"+UEcoll+";Leading jet pt;<ptsum>", nBins, varbins, 0, 10000);

  /*ntracks_precut = new TH1F("ntracks_precut" , "ntracks_precut;nTracks;nEvents" , 200 , 0 ,200 );
  ntracks_cut = new TH1F("ntracks_cut" , "ntracks_cut;nTracks;nEvents" , 200 , 0 ,200 );
  Tracks_chi2noriginal = new TH1F("Tracks_chi2noriginal" , "Tracks_chi2noriginal;chi2n;nTracks" , 120 , 0 ,40 );
  Tracks_vtxdz = new TH1F("Tracks_vtxdz", "vtxdxy;dz;nTracks" , 120 , -5 , 5); 
  Tracks_vtxdxy = new TH1F("Tracks_vtxdxy", "vtxdxy;dxy;nTracks" , 120 , -5 , 5);
  Tracks_vtxed0 = new TH1F("Tracks_vtxed0", "vtxed0;ed0;nTracks" , 120 , 0 , 5);
  Tracks_dxy = new TH1F("Tracks_dxy", "dxy;dxy;nTracks" , 120 , -20 , 20);
  Tracks_dz = new TH1F("Tracks_dz", "dz;dz;nTracks" , 120 , -20 , 20);
  Tracks_vtxedz = new TH1F("Tracks_vtxedz", "vtxedz;edz;nTracks" , 120 , 0 , 10);
  Tracks_xyquality = new TH1F("Tracks_xyquality", "track_xyquality;xy_quality;nTracks" , 2 , 0 , 2);
  Tracks_zquality = new TH1F("Tracks_zquality", "track_zquality;z_quality;nTracks" , 2 , 0 , 2);
  Tracks_2quality = new TH1F("Tracks_2quality", "track_2quality;2_quality;nTracks" , 2 , 0 , 2);
  Tracks_Quality = new TH1F("Tracks_Quality" , "Tracks_Quality;Quality;nTracks" , 2 , 0 ,2 );
  
  pt_reco = new TH1F("pt_reco", "pt_reco;pt;nTracks" , 120 , 0 , 6);  
  eta_reco = new TH1F("eta_reco", "eta_reco;eta;nTracks" , 40 , -5 , 5);
  Tracks_charge = new TH1F("Tracks_charge" , "Tracks_charge;charge;nTracks" , 40 , -2 ,2 );
  Tracks_nhit = new TH1F("Tracks_nhit" , "Tracks_nhit;nhit;nTracks" , 51 , 0 ,50 );
  Tracks_pixelhit = new TH1F("Tracks_pixelhit" , "Tracks_pixelhit;pixelhit;nTracks" , 31 , 0 ,30 );
  Tracks_striphit = new TH1F("Tracks_striphit" , "Tracks_striphit;striphit;nTracks" , 51 , 0 ,50 );
  Tracks_chi2n = new TH1F("Tracks_chi2n" , "Tracks_chi2n;chi2n;nTracks" , 120 , 0 ,40 );
  Tracks_phi = new TH1F("Tracks_phi" , "Tracks_phi;phi;nTracks" , 120 , -4 ,4 );
  test = new TH1F("test", "test;test;nTracks" , 2 , 0 , 2);
  test2 = new TH1F("test2", "test2;ntracks;nVertices" , 200 , 0 , 200);*/
}

//void UEPlots::clean(vector<MyTracks>& tracks, vector<MyVertex>& vertex){
//     bool hasvertex=false;
//     int nvertex=0; 
//     int vertexid=0; 
//     for(vector<MyVertex>::iterator v = vertex.begin() ; v != vertex.end() ; ++v){
//       if (!v->fake){
//         hasvertex=true;
//	     ++nvertex;
//         vertexid=(v->id);
//       }			 
//     }
//      
//     goodtracks.clear();
//     ntracks_precut->Fill(tracks.size());
//	 int gdtracks_counter = 0;
//     
//     for(vector<MyTracks>::iterator it_tr = tracks.begin() ; it_tr != tracks.end() ; ++it_tr){
//	  
//	    Tracks_chi2noriginal->Fill(it_tr->chi2n);
//	    
//		bool trackeptquality = false;
//		if (((it_tr->ept)/(it_tr->Pt())) < .1){
//			trackeptquality = true;
//			}
//	  
//        Tracks_vtxdz->Fill((it_tr->vtxdz).at(vertexid));
//		Tracks_vtxdxy->Fill((it_tr->vtxdxy).at(vertexid));
//		Tracks_vtxed0->Fill(it_tr->ed0);
//		double trackdxy;
//		trackdxy = (((it_tr->vtxdxy).at(vertexid))/(it_tr->ed0));
//		Tracks_dxy->Fill(trackdxy);
//		bool trackxyquality=false;
//		if (trackdxy < 5) {
//		     if (trackdxy > -5) {
//			    trackxyquality=true;
//				}
//			}
//		
//		Tracks_xyquality->Fill(trackxyquality);
//		
//		Tracks_vtxedz->Fill(it_tr->edz);
//		double trackdz;
//		trackdz = (((it_tr->vtxdz).at(vertexid))/(it_tr->edz));
//		Tracks_dz->Fill(trackdz);
//		bool trackzquality=false;
//		if (trackdz < 5) {
//		     if (trackdz > -5) {
//			    trackzquality=true;
//				}
//			}
//		
//		Tracks_zquality->Fill(trackzquality);
//		
//		Tracks_2quality->Fill(it_tr->quality[2]);
//		/*
//		bool trackptcut=false;
//		if ((it_tr->Pt()) > 0.5){
//			trackptcut=true;
//			}
//		*/
//		bool trackquality=false;
//		
//		if (trackxyquality==true){
//		    if (trackzquality==true){
//			    if ((it_tr->quality[2])==true){
//					if (trackeptquality==true){
//						//if (trackptcut==true){
//							trackquality=true;
//							//}
//						}
//					}
//				}
//			}
//		
//		Tracks_Quality->Fill(trackquality);	
//		
//		if (trackquality==true){
//		    goodtracks.push_back(1);
//			test->Fill(1);
//			++gdtracks_counter;
//			}
//		else {
//		    goodtracks.push_back(0);
//			test->Fill(0);
//			}
//		
//		
//		
//		
//		if (trackquality==false) continue;
//		//track stats only for good tracks
//		pt_reco->Fill(it_tr->Pt());
//		eta_reco->Fill(it_tr->PseudoRapidity());
//		Tracks_charge->Fill(it_tr->charge);
//		Tracks_nhit->Fill(it_tr->nhit);
//		Tracks_pixelhit->Fill(it_tr->nValidPixelHits);
//		Tracks_striphit->Fill(it_tr->nValidStripHits);
//		Tracks_chi2n->Fill(it_tr->chi2n);
//		Tracks_phi->Fill(it_tr->Phi());		
//		
//		}
//		
//		test2->Fill(goodtracks.size());
//		
//		ntracks_cut->Fill(gdtracks_counter);	 
//}
//
//void UEPlots::clean(vector<MyGenPart>& genpart, bool clean){
//     
//     goodtracks.clear();
//     int gdtracks_counter = 0;
//     
//     for(vector<MyGenPart>::iterator it_tr = genpart.begin() ; it_tr != genpart.end() ; ++it_tr){
//		if (clean==true)
//        {
//           //if ((it_tr->charge != 0) && (it_tr->Eta() < 2.5) && (it_tr->Eta() > -2.5)) 
//           {
//              goodtracks.push_back(1);
//              ++gdtracks_counter;
//           }
//           //else                    
//           //   goodtracks.push_back(0); 
//		}
//		else {
//		    goodtracks.push_back(0);
//			}	
//     } 
//     
//     ntracks_cut->Fill(gdtracks_counter);
//}

void UEPlots::fill(vector<MyPart>& part, double pt_lead_cut, double eta_lead_cut, double pt_assoc_cut, double eta_assoc_cut, double EvtWeight, bool WeighTr, bool PtCorrection){
     
     double prevpartpt = 0;
     int ilead_part = -1;           //leading particle pointer
     int ilead_part_temp = 0;
     int trackcounter=0;

     for(vector<MyPart>::iterator it_part = part.begin() ; it_part != part.end() ; ++it_part,++ilead_part_temp,++trackcounter)
     {
       //if (goodtracks[trackcounter]==0) continue;                           
       if (it_part->Pt() == 0) continue;
       if (it_part->charge == 0) continue;
       if (it_part->Pt() < pt_lead_cut) continue;
       if (fabs(it_part->Eta()) > eta_lead_cut) continue;
              
       if (it_part->Pt() > prevpartpt)
       {   
           prevpartpt = it_part->Pt();
           ilead_part = ilead_part_temp;
       }
     }  
     
     double npartforward = 0;
     double nparttrans = 0;
     double npartaway = 0;
     double nparttotal = 0;
     double ptsumforward = 0;
     double ptsumtrans = 0;
     double ptsumaway = 0;
     double ptsumtotal = 0;
     int dphitemp = 0;
     
     if((ilead_part != -1) && (fabs(part.at(ilead_part).Eta()) <= eta_lead_cut) && (part.at(ilead_part).Pt() >= pt_lead_cut))
     {
        ilead_part_temp = 0;     
        trackcounter = 0;                       
        for(vector<MyPart>::iterator p=part.begin() ; p!=part.end() ; p++, ilead_part_temp++, trackcounter++)
        {
           //if (goodtracks[trackcounter]==0) continue; 
           if ((p->charge != 0) && (p->Pt() >= pt_assoc_cut) && (fabs(p->Eta()) <= eta_assoc_cut) && (ilead_part_temp != ilead_part))
           {
              pt_nonlead_part->Fill(p->Pt());
              eta_nonlead_part->Fill(p->Eta());
              phi_nonlead_part->Fill(p->Phi());
              
              dphi->Fill(p->Phi() - part.at(ilead_part).Phi());
              dphitemp = mod(p->Phi() - part.at(ilead_part).Phi());
                 if ((dphitemp <= deg2rad(60)) || (dphitemp >= deg2rad(300))){
                           ++npartforward;
                           ptsumforward += p->Pt();
                           pt_spectra_forward->Fill(p->Pt());
                           }
                 if (((dphitemp > deg2rad(60)) && (dphitemp < deg2rad(120))) || ((dphitemp > deg2rad(240)) && (dphitemp < deg2rad(300)))){
                           ++nparttrans;
                           ptsumtrans += p->Pt();
                           pt_spectra_trans->Fill(p->Pt());
                           }
                 if ((dphitemp >= deg2rad(120)) && (dphitemp <= deg2rad(240))){
                           ++npartaway;
                           ptsumaway += p->Pt();
                           pt_spectra_away->Fill(p->Pt());
                           }
                 ++nparttotal;
                 ptsumtotal += p->Pt();
                 pt_spectra_total->Fill(p->Pt());
           }
           else if ((p->charge != 0) && (p->Pt() >= pt_assoc_cut) && (fabs(p->Eta()) <= eta_assoc_cut) && (ilead_part_temp == ilead_part))
           {
                pt_lead_part->Fill(p->Pt());
                eta_lead_part->Fill(p->Eta());
                phi_lead_part->Fill(p->Phi());                  
           }
        }
       //if (!((npartforward != 0) && (ptsumforward == 0)))
       {
           n_mult_forward->Fill(npartforward);
           ptsum_forward->Fill(ptsumforward);
           profile_n_mult_forward->Fill(part.at(ilead_part).Pt(), npartforward/(2*eta_lead_cut*deg2rad(120)));
           profile_ptsum_forward->Fill(part.at(ilead_part).Pt(), ptsumforward/(2*eta_lead_cut*deg2rad(120)));
       }
       
       //if (!((nparttrans != 0) && (ptsumtrans == 0)))
       {
           n_mult_trans->Fill(nparttrans);
           ptsum_trans->Fill(ptsumtrans);
           profile_n_mult_trans->Fill(part.at(ilead_part).Pt(), nparttrans/(2*eta_lead_cut*deg2rad(120)));
           profile_ptsum_trans->Fill(part.at(ilead_part).Pt(), ptsumtrans/(2*eta_lead_cut*deg2rad(120)));
       }
       
       //if (!((npartaway != 0) && (ptsumaway == 0)))
       {
           n_mult_away->Fill(npartaway);
           ptsum_away->Fill(ptsumaway);
           profile_n_mult_away->Fill(part.at(ilead_part).Pt(), npartaway/(2*eta_lead_cut*deg2rad(120)));
           profile_ptsum_away->Fill(part.at(ilead_part).Pt(), ptsumaway/(2*eta_lead_cut*deg2rad(120)));
       }
       //if (!((nparttotal != 0) && (ptsumtotal == 0)))
       {
           n_mult_total->Fill(nparttotal);
           ptsum_total->Fill(ptsumtotal);
           profile_n_mult_total->Fill(part.at(ilead_part).Pt(), nparttotal/(2*eta_lead_cut*deg2rad(120)));
           profile_ptsum_total->Fill(part.at(ilead_part).Pt(), ptsumtotal/(2*eta_lead_cut*deg2rad(120)));   
       }  
     }
}

void UEPlots::fill(vector<MyPart>& lead_jetpart, vector<MyPart>& assoc_jetpart, double pt_lead_cut, double eta_lead_cut, double pt_assoc_cut, double eta_assoc_cut, double EvtWeight, bool WeighTr, bool PtCorrection){
     double weight = 1;
     
     //cout<< endl << "jet " << pt_lead_cut << " particles " << pt_assoc_cut << endl;
     double prevjetpartpt = 0;
     int ilead_jetpart = -1;           //leading jet or particle pointer
     int ilead_jetpart_temp = 0;
     int trackcounter = 0;
     
     for(vector<MyPart>::iterator it_jetpart = lead_jetpart.begin() ; it_jetpart != lead_jetpart.end() ; ++it_jetpart,++ilead_jetpart_temp)
     {                           
       if (it_jetpart->Pt() == 0) continue;
       if (it_jetpart->Pt() < pt_lead_cut) continue;
       if (fabs(it_jetpart->Eta()) > eta_lead_cut) continue;
       
       if (it_jetpart->Pt() > prevjetpartpt)
       {   
           prevjetpartpt = it_jetpart->Pt();
           ilead_jetpart = ilead_jetpart_temp;
       }
     }  
     
     //--------------------------------------------------------------------------------
     double prevpartpt = 0;
     int ilead_part = -1;
     int ilead_part_temp = 0;
     
     for(vector<MyPart>::iterator it_part = assoc_jetpart.begin() ; it_part != assoc_jetpart.end() ; ++it_part,++ilead_part_temp)
     {                           
       if (it_part->Pt() == 0) continue;
       if (it_part->Pt() < pt_lead_cut) continue;
       if (fabs(it_part->Eta()) > eta_lead_cut) continue;
       
       if (it_part->Pt() > prevpartpt)
       {   
           prevpartpt = it_part->Pt();
           ilead_part = ilead_part_temp;
       }
     }
          
     if ((ilead_part != -1) && (ilead_jetpart != -1) && (lead_jetpart.at(ilead_jetpart).Pt() >= pt_lead_cut) && (fabs(lead_jetpart.at(ilead_jetpart).Eta()) <= eta_lead_cut) && (assoc_jetpart.at(ilead_part).Pt() >= pt_lead_cut) && (fabs(assoc_jetpart.at(ilead_part).Eta()) <= eta_lead_cut))
     {
        if (WeighTr) weight = GetWeight(assoc_jetpart.at(ilead_part).Pt(), assoc_jetpart.at(ilead_part).Eta());
        
        double dpt = assoc_jetpart.at(ilead_part).Pt() - lead_jetpart.at(ilead_jetpart).Pt();
        double deta = assoc_jetpart.at(ilead_part).Eta() - lead_jetpart.at(ilead_jetpart).Eta();
        double dphi = assoc_jetpart.at(ilead_part).Phi() - lead_jetpart.at(ilead_jetpart).Phi();
        double dR = sqrt(deta*deta + dphi*dphi);
        
        dR_LeadPart_LeadJet->Fill(dR, weight*EvtWeight);
        deta_LeadPart_LeadJet->Fill(deta, weight*EvtWeight);
        dphi_LeadPart_LeadJet->Fill(dphi, EvtWeight);
        detadphi_LeadPart_LeadJet->Fill(deta, dphi, weight*EvtWeight);
        
        if (dR < 0.5)
        {
           PtLeadPart_PtLeadJet->Fill((assoc_jetpart.at(ilead_part).Pt())/(lead_jetpart.at(ilead_jetpart).Pt()), weight*EvtWeight);
           profile_PtLeadPart_PtLeadJet->Fill(lead_jetpart.at(ilead_jetpart).Pt(), (assoc_jetpart.at(ilead_part).Pt())/(lead_jetpart.at(ilead_jetpart).Pt()), weight*EvtWeight);
        }                
     }
     //----------------------------------------------------------------------------------
          
     ilead_jetpart_temp = 0;
     for(vector<MyPart>::iterator it_jetpart = lead_jetpart.begin() ; it_jetpart != lead_jetpart.end() ; ++it_jetpart,++ilead_jetpart_temp)
     {
         if((ilead_jetpart != -1) && (fabs(lead_jetpart.at(ilead_jetpart).Eta()) <= eta_lead_cut) && (lead_jetpart.at(ilead_jetpart).Pt() >= pt_lead_cut))  
         {
             if (ilead_jetpart_temp == ilead_jetpart)
             {
                pt_lead_jet->Fill(it_jetpart->Pt(), EvtWeight);
                eta_lead_jet->Fill(it_jetpart->Eta(), EvtWeight);
                phi_lead_jet->Fill(it_jetpart->Phi(), EvtWeight);   
             }
             else
             {
                 pt_nonlead_jet->Fill(it_jetpart->Pt(), EvtWeight);
                 eta_nonlead_jet->Fill(it_jetpart->Eta(), EvtWeight);
                 phi_nonlead_jet->Fill(it_jetpart->Phi(), EvtWeight);  
             }            
         }
     }
    
    double npartforward = 0;
    double nparttrans = 0;
    double npartaway = 0;
    double nparttotal = 0;
    double ptsumforward = 0;
    double ptsumtrans = 0;
    double ptsumaway = 0;
    double ptsumtotal = 0;
    double npartforward_weighted = 0;
    double nparttrans_weighted = 0;
    double npartaway_weighted = 0;
    double nparttotal_weighted = 0;
    double ptsumforward_weighted = 0;
    double ptsumtrans_weighted = 0;
    double ptsumaway_weighted = 0;
    double ptsumtotal_weighted = 0;
    int dphitemp = 0;
       
    if((ilead_jetpart != -1) && (fabs(lead_jetpart.at(ilead_jetpart).Eta()) <= eta_lead_cut) && (lead_jetpart.at(ilead_jetpart).Pt() >= pt_lead_cut))  
    {                           
       for(vector<MyPart>::iterator p=assoc_jetpart.begin() ; p!=assoc_jetpart.end() ; p++, trackcounter++)
       { 
         if (WeighTr) weight = GetWeight(p->Pt(), p->Eta());
         
         //if (goodtracks[trackcounter] == 0) continue;                           
         if ((p->charge != 0) && (p->Pt() >= pt_assoc_cut) && (fabs(p->Eta()) < eta_assoc_cut))
         {
            int bin;
            double ptBinContent = 0;
            
            dphi->Fill(p->Phi() - lead_jetpart.at(ilead_jetpart).Phi(), EvtWeight);
            dphitemp = p->Phi() - lead_jetpart.at(ilead_jetpart).Phi();
            while (dphitemp > pi) dphitemp -= 2*pi;
            while (dphitemp <= -pi) dphitemp += 2*pi;
            dphitemp = fabs(dphitemp);
            //if ((dphitemp <= deg2rad(60)) || (dphitemp >= deg2rad(300))){
            if (dphitemp <= deg2rad(60)){
                           npartforward++;
                           npartforward_weighted += weight;
                           ptsumforward += p->Pt();
                           ptsumforward_weighted += ( (p->Pt()) * weight);
                           pt_spectra_forward->Fill(p->Pt(), EvtWeight);
                           pt_spectra_forward_weighted->Fill(p->Pt(), weight*EvtWeight);
                           }
            //if (((dphitemp > deg2rad(60)) && (dphitemp < deg2rad(120))) || ((dphitemp > deg2rad(240)) && (dphitemp < deg2rad(300)))){
            if ((dphitemp > deg2rad(60)) && (dphitemp < deg2rad(120))){
                           nparttrans++;
                           nparttrans_weighted += weight;
                           ptsumtrans += p->Pt();
                           ptsumtrans_weighted += ( (p->Pt()) * weight);
                           pt_spectra_trans->Fill(p->Pt(), EvtWeight);
                           pt_spectra_trans_weighted->Fill(p->Pt(), weight*EvtWeight);
                           }
            //if ((dphitemp >= deg2rad(120)) && (dphitemp <= deg2rad(240))){
            if (dphitemp >= deg2rad(120)){
                           npartaway++;
                           npartaway_weighted += weight;
                           ptsumaway += p->Pt();
                           ptsumaway_weighted += ( (p->Pt()) * weight);
                           pt_spectra_away->Fill(p->Pt(), EvtWeight);
                           pt_spectra_away_weighted->Fill(p->Pt(), weight*EvtWeight);
                           }      
            nparttotal++;
            nparttotal_weighted += weight;
            ptsumtotal += p->Pt();
            ptsumtotal_weighted += ( (p->Pt()) * weight);
            pt_spectra_total->Fill(p->Pt(), EvtWeight);
            pt_spectra_total_weighted->Fill(p->Pt(), weight*EvtWeight);
            
            weight_dist->Fill(weight);
         }
       }
       double jetPt = lead_jetpart.at(ilead_jetpart).Pt();
       
       //---Uncorrected---
       n_mult_forward->Fill(npartforward, EvtWeight);
       ptsum_forward->Fill(ptsumforward, EvtWeight);
       profile_n_mult_forward->Fill(jetPt, npartforward/(2*eta_lead_cut*deg2rad(120)), EvtWeight);
       profile_ptsum_forward->Fill(jetPt, ptsumforward/(2*eta_lead_cut*deg2rad(120)), EvtWeight);

       n_mult_trans->Fill(nparttrans, EvtWeight);
       ptsum_trans->Fill(ptsumtrans, EvtWeight);
       profile_n_mult_trans->Fill(jetPt, nparttrans/(2*eta_lead_cut*deg2rad(120)), EvtWeight);
       profile_ptsum_trans->Fill(jetPt, ptsumtrans/(2*eta_lead_cut*deg2rad(120)), EvtWeight);
  
       n_mult_away->Fill(npartaway, EvtWeight);
       ptsum_away->Fill(ptsumaway, EvtWeight);
       profile_n_mult_away->Fill(jetPt, npartaway/(2*eta_lead_cut*deg2rad(120)), EvtWeight);
       profile_ptsum_away->Fill(jetPt, ptsumaway/(2*eta_lead_cut*deg2rad(120)), EvtWeight);
   
       n_mult_total->Fill(nparttotal, EvtWeight);
       ptsum_total->Fill(ptsumtotal, EvtWeight);
       profile_n_mult_total->Fill(jetPt, nparttotal/(2*eta_lead_cut*deg2rad(120)), EvtWeight);
       profile_ptsum_total->Fill(jetPt, ptsumtotal/(2*eta_lead_cut*deg2rad(120)), EvtWeight);  
       
       //---Corrected Track efficiency and Fake rates--- 
       n_mult_forward_weighted->Fill(npartforward_weighted, EvtWeight);
       ptsum_forward_weighted->Fill(ptsumforward_weighted, EvtWeight);
       profile_n_mult_forward_weighted->Fill(jetPt, npartforward_weighted/(2*eta_lead_cut*deg2rad(120)), EvtWeight);
       profile_ptsum_forward_weighted->Fill(jetPt, ptsumforward_weighted/(2*eta_lead_cut*deg2rad(120)), EvtWeight);

       n_mult_trans_weighted->Fill(nparttrans_weighted, EvtWeight);
       ptsum_trans_weighted->Fill(ptsumtrans_weighted, EvtWeight);
       profile_n_mult_trans_weighted->Fill(jetPt, nparttrans_weighted/(2*eta_lead_cut*deg2rad(120)), EvtWeight);
       profile_ptsum_trans_weighted->Fill(jetPt, ptsumtrans_weighted/(2*eta_lead_cut*deg2rad(120)), EvtWeight);
  
       n_mult_away_weighted->Fill(npartaway_weighted, EvtWeight);
       ptsum_away_weighted->Fill(ptsumaway_weighted, EvtWeight);
       profile_n_mult_away_weighted->Fill(jetPt, npartaway_weighted/(2*eta_lead_cut*deg2rad(120)), EvtWeight);
       profile_ptsum_away_weighted->Fill(jetPt, ptsumaway_weighted/(2*eta_lead_cut*deg2rad(120)), EvtWeight);
   
       n_mult_total_weighted->Fill(nparttotal_weighted, EvtWeight);
       ptsum_total_weighted->Fill(ptsumtotal_weighted, EvtWeight);
       profile_n_mult_total_weighted->Fill(jetPt, nparttotal_weighted/(2*eta_lead_cut*deg2rad(120)), EvtWeight);
       profile_ptsum_total_weighted->Fill(jetPt, ptsumtotal_weighted/(2*eta_lead_cut*deg2rad(120)), EvtWeight);
       
       
       
       if (PtCorrection) jetPt = GetCorrectedptratio(jetPt);
       
       //---Corrected for Jet Pt--- 
       profile_n_mult_forward_jetcorr->Fill(jetPt, npartforward/(2*eta_lead_cut*deg2rad(120)), EvtWeight);
       profile_ptsum_forward_jetcorr->Fill(jetPt, ptsumforward/(2*eta_lead_cut*deg2rad(120)), EvtWeight);
       
       profile_n_mult_trans_jetcorr->Fill(jetPt, nparttrans/(2*eta_lead_cut*deg2rad(120)), EvtWeight);
       profile_ptsum_trans_jetcorr->Fill(jetPt, ptsumtrans/(2*eta_lead_cut*deg2rad(120)), EvtWeight);
       
       profile_n_mult_away_jetcorr->Fill(jetPt, npartaway/(2*eta_lead_cut*deg2rad(120)), EvtWeight);
       profile_ptsum_away_jetcorr->Fill(jetPt, ptsumaway/(2*eta_lead_cut*deg2rad(120)), EvtWeight);
       
       profile_n_mult_total_jetcorr->Fill(jetPt, nparttotal/(2*eta_lead_cut*deg2rad(120)), EvtWeight);
       profile_ptsum_total_jetcorr->Fill(jetPt, ptsumtotal/(2*eta_lead_cut*deg2rad(120)), EvtWeight); 
        
       //---Corrected for Jet Pt and Track efficiency and Fake rates---  
       profile_n_mult_forward_jetcorr_weighted->Fill(jetPt, npartforward_weighted/(2*eta_lead_cut*deg2rad(120)), EvtWeight);
       profile_ptsum_forward_jetcorr_weighted->Fill(jetPt, ptsumforward_weighted/(2*eta_lead_cut*deg2rad(120)), EvtWeight);
       
       profile_n_mult_trans_jetcorr_weighted->Fill(jetPt, nparttrans_weighted/(2*eta_lead_cut*deg2rad(120)), EvtWeight);
       profile_ptsum_trans_jetcorr_weighted->Fill(jetPt, ptsumtrans_weighted/(2*eta_lead_cut*deg2rad(120)), EvtWeight);
       
       profile_n_mult_away_jetcorr_weighted->Fill(jetPt, npartaway_weighted/(2*eta_lead_cut*deg2rad(120)), EvtWeight);
       profile_ptsum_away_jetcorr_weighted->Fill(jetPt, ptsumaway_weighted/(2*eta_lead_cut*deg2rad(120)), EvtWeight);
       
       profile_n_mult_total_jetcorr_weighted->Fill(jetPt, nparttotal_weighted/(2*eta_lead_cut*deg2rad(120)), EvtWeight);
       profile_ptsum_total_jetcorr_weighted->Fill(jetPt, ptsumtotal_weighted/(2*eta_lead_cut*deg2rad(120)), EvtWeight); 
       
       //cout<< lead_jetpart.at(ilead_jetpart).Pt() << "  " << ptsumtrans << endl;
    }
}
     
void UEPlots::write(){
    gDirectory->cd("../macro2");
    gDirectory->mkdir("UEPlots_"+UEcoll);
    gDirectory->cd("UEPlots_"+UEcoll);
    
    pt_lead_part->Write();
    pt_lead_jet->Write();
    eta_lead_part->Write();
    eta_lead_jet->Write();
    phi_lead_part->Write();
    phi_lead_jet->Write();
  
    pt_nonlead_part->Write();
    pt_nonlead_jet->Write();
    eta_nonlead_part->Write();
    eta_nonlead_jet->Write();
    phi_nonlead_part->Write();
    phi_nonlead_jet->Write();
    
    dR_LeadPart_LeadJet->Write();
    deta_LeadPart_LeadJet->Write();
    dphi_LeadPart_LeadJet->Write();
    detadphi_LeadPart_LeadJet->Write();
    PtLeadPart_PtLeadJet->Write();
    profile_PtLeadPart_PtLeadJet->Write();
    
    weight_dist->Write();
    
    //---Uncorrected---
    dphi->Write();
    n_mult_forward->Write();
    n_mult_trans->Write();
    n_mult_away->Write();
    n_mult_total->Write();
    ptsum_forward->Write();
    ptsum_trans->Write();
    ptsum_away->Write();
    ptsum_total->Write();
    pt_spectra_forward->Write();
    pt_spectra_trans->Write();
    pt_spectra_away->Write();
    pt_spectra_total->Write();
    profile_n_mult_forward->Write();
    profile_n_mult_trans->Write(); 
    profile_n_mult_away->Write();
    profile_n_mult_total->Write();
    profile_ptsum_forward->Write();
    profile_ptsum_trans->Write();
    profile_ptsum_away->Write();
    profile_ptsum_total->Write();
    
    profile_ptsum_n_mult_trans->Divide(profile_ptsum_trans, profile_n_mult_trans, 1, 1);
    profile_ptsum_n_mult_trans->Write();
    
    //---Corrected for Jet Pt--- 
    profile_n_mult_forward_jetcorr->Write();
    profile_n_mult_trans_jetcorr->Write();
    profile_n_mult_away_jetcorr->Write();
    profile_ptsum_forward_jetcorr->Write();
    profile_ptsum_trans_jetcorr->Write();
    profile_ptsum_away_jetcorr->Write();
    profile_n_mult_total_jetcorr->Write();
    profile_ptsum_total_jetcorr->Write();

    //---Corrected Track efficiency and Fake rates---
    n_mult_forward_weighted->Write();
    n_mult_trans_weighted->Write();
    n_mult_away_weighted->Write();
    n_mult_total_weighted->Write();
    ptsum_forward_weighted->Write();
    ptsum_trans_weighted->Write();
    ptsum_away_weighted->Write();
    ptsum_total_weighted->Write();
    pt_spectra_forward_weighted->Write();
    pt_spectra_trans_weighted->Write();
    pt_spectra_away_weighted->Write();
    pt_spectra_total_weighted->Write();
    
    profile_n_mult_forward_weighted->Write();
    profile_n_mult_trans_weighted->Write();
    profile_n_mult_away_weighted->Write();
    profile_ptsum_forward_weighted->Write();
    profile_ptsum_trans_weighted->Write();
    profile_ptsum_away_weighted->Write();
    profile_n_mult_total_weighted->Write();
    profile_ptsum_total_weighted->Write();

    //---Corrected for Jet Pt and Track efficiency and Fake rates--- 
    profile_n_mult_forward_jetcorr_weighted->Write();
    profile_n_mult_trans_jetcorr_weighted->Write();
    profile_n_mult_away_jetcorr_weighted->Write();
    profile_ptsum_forward_jetcorr_weighted->Write();
    profile_ptsum_trans_jetcorr_weighted->Write();
    profile_ptsum_away_jetcorr_weighted->Write();
    profile_n_mult_total_jetcorr_weighted->Write();
    profile_ptsum_total_jetcorr_weighted->Write();
     
    /*gDirectory->cd("../");
    gDirectory->mkdir("cleanplots_"+UEcoll);
    gDirectory->cd("cleanplots_"+UEcoll);
    
    ntracks_precut->Write();
    ntracks_cut->Write();
    Tracks_chi2noriginal->Write();
    Tracks_vtxdz->Write();
    Tracks_vtxdxy->Write();
    Tracks_vtxed0->Write();
    Tracks_dxy->Write();
    Tracks_dz->Write();
    Tracks_vtxedz->Write();
    Tracks_xyquality->Write();
    Tracks_zquality->Write();
    Tracks_2quality->Write();
    Tracks_Quality->Write();
    
    pt_reco->Write();
    eta_reco->Write();
    Tracks_charge->Write();
    Tracks_nhit->Write();
    Tracks_pixelhit->Write();
    Tracks_striphit->Write();
    Tracks_chi2n->Write();
    Tracks_phi->Write();
    test->Write();
    test2->Write();*/
    
    gDirectory->cd("../../plugins");  
}

bool UEPlots::GetTrEffFromFile(TString fstr , TString treffstr , TString trfakesstr){
  TFile* f = TFile::Open(fstr , "READ");
  if(f==0){
    cout << "[UEPlots] WARNING !! Can't open file for trEff & trFakes." << endl;
    //trReweightOn = 0;
    return false;
  }
  
  TOperation<TH2F> trEff_temp = *( (TOperation<TH2F>*) f->Get(treffstr));
  trEff = trEff_temp.result;
  if(trEff.GetEntries()==0){
    cout << "[UEPlots] WARNING !! Couldn't fetch trEff " << treffstr << " from file "<< fstr << "." << endl;
    //trReweightOn = 0;
    return false;
  }
  
  TOperation<TH2F> trFakes_temp = *( (TOperation<TH2F>*) f->Get(trfakesstr));
  trFakes = trFakes_temp.result;
  if(trFakes.GetEntries()==0){
    cout << "[UEPlots] WARNING !! Couldn't fetch trFakes " << trfakesstr << " from file "<< fstr << "." << endl;
    //trReweightOn = 0;
    return false;
  }
  return true;
}

bool UEPlots::GetPtCorrFromFile(TString fstr , TString ptcorrstr , TString ptratiostr){
  TFile* f = TFile::Open(fstr , "READ");
  if(f==0){
    cout << "[UEPlots] WARNING !! Can't open file for ptcorr & ptratio." << endl;
    //trReweightOn = 0;
    return false;
  }
  
  //TOperation<TH2F> ptcorr_temp = *( (TOperation<TH2F>*) f->Get(ptcorrstr));
  ptcorr = *((TH2F*) f->Get(ptcorrstr));
  //ptcorr = ptcorr_temp.result;
  if(ptcorr.GetEntries()==0){
    cout << "[UEPlots] WARNING !! Couldn't fetch ptcorr " << ptcorrstr << " from file "<< fstr << "." << endl;
    //trReweightOn = 0;
    return false;
  }
  
  //TOperation<TProfile> ptratio_temp = *( (TOperation<TProfile>*) f->Get(ptratiostr));
  ptratio = *((TProfile*) f->Get(ptratiostr));
  //ptratio = ptratio_temp.result;
  if(ptratio.GetEntries()==0){
    cout << "[UEPlots] WARNING !! Couldn't fetch ptratio " << ptratiostr << " from file "<< fstr << "." << endl;
    //trReweightOn = 0;
    return false;
  }
  return true;
}

double UEPlots::GetFake(double pt, double eta){
       if(&trFakes==0) return 0;
       int xbin = trFakes.GetXaxis()->FindFixBin( pt ) ;
       int ybin = trFakes.GetYaxis()->FindFixBin( eta  ) ;
       double trFakes_val = trFakes.GetBinContent( xbin , ybin );
       if( xbin<1 || ybin<1 || xbin>trFakes.GetNbinsX() || ybin>trFakes.GetNbinsY() || trFakes_val == 0 ) return 0.;
       //cout<< "Fake: " << trFakes_val << " ";
       return trFakes_val;
}

double UEPlots::GetEff(double pt, double eta){
       if(&trEff==0) return 1;
       int xbin = trEff.GetXaxis()->FindFixBin( pt ) ;
       int ybin = trEff.GetYaxis()->FindFixBin( eta  ) ;
       double trEff_val = trEff.GetBinContent( xbin , ybin );
       if( xbin<1 || ybin<1 || xbin>trEff.GetNbinsX() || ybin>trEff.GetNbinsY() || trEff_val == 0 ) return 1.;
       //cout<< "Efficiency: " << trEff_val << " ";
       return trEff_val;
}

double UEPlots::GetWeight(double pt, double eta){
       double fake = GetFake(pt, eta);
       double efficiency = GetEff(pt, eta);
       /*if ((1-fake)/efficiency > 2){
       cout<< "Pt: " << pt << " " << "Eta: " << eta << endl;
       cout<< "Fake: " << fake << " ";
       cout<< "Efficiency: " << efficiency << " ";
       cout<< "Weight: " << (1-fake)/efficiency << endl;}*/
       return (1-fake)/efficiency;
}

//double UEPlots::GetCorrectedptcorr()

double UEPlots::GetCorrectedptratio(double pt)
{
       int xbin = ptratio.GetXaxis()->FindFixBin( pt ) ;
       //if (pt<14) cout<< "pt: " << pt << " Bincontent: " << ptratio.GetBinContent(xbin) << " ptratio: " << 1/(ptratio.GetBinContent(xbin)) << endl; 
       if (pt < 14) return ( pt/(ptratio.GetBinContent(xbin)) );
       
       double average = 0;
       //int iNoBins = (int) ((TH1) ptratio).GetXaxis()->GetNbins();
       int iNoBins = ptratio.GetNbinsX();
       for (int i = 15; i <= iNoBins; i++)
           average += ptratio.GetBinContent(i);
       if (average == 0) return pt;
       average = average/(iNoBins-14);
       //cout<< "pt: " << pt << " " << "1/ave: " << 1/average << endl;
       return (pt/average);
}

double deg2rad(double deg){
  return (deg * pi / 180);
}

double mod(double num){
  if (num>=0) return num;
  else return -num;
}
