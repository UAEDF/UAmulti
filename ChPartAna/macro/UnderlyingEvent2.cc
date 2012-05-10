//STANDARD ROOT INCLUDES
#include <TROOT.h>
#include <TH1.h>
#include <TH2.h>
#include <TProfile.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TTree.h>
#include <TFile.h>
#include <TChain.h>
#include <TChainElement.h>
#include <TDirectory.h>
#include <algorithm>

//STANDARD C++ INCLUDES
#include <iostream>
#include <sstream>
using namespace std;

//OUR OWN CLASSES TO READ THE TREE
#include "../mydir/MassParticles.h"
#include "../mydir/MyBaseJet.h"
#include "../mydir/MyBeamSpot.h"
#include "../mydir/MyCaloJet.h"
#include "../mydir/MyCastorDigi.h"
#include "../mydir/MyCastorJet.h"
#include "../mydir/MyCastorRecHit.h"
#include "../mydir/MyDiJet.h"
#include "../mydir/MyElectron.h"
#include "../mydir/MyEvtId.h"
#include "../mydir/MyFwdGap.h"
#include "../mydir/MyGenJet.h"
#include "../mydir/MyGenKin.h"
#include "../mydir/MyGenMet.h"
#include "../mydir/MyGenPart.h"
#include "../mydir/MyHLTrig.h"
#include "../mydir/MyJet.h"
#include "../mydir/MyL1Trig.h"
#include "../mydir/MyL1TrigOld.h"
#include "../mydir/MyMITEvtSel.h"
#include "../mydir/MyMet.h"
#include "../mydir/MyMuon.h"
#include "../mydir/MyPFCand.h"
#include "../mydir/MyPFJet.h"
#include "../mydir/MyPUSumInfo.h"
#include "../mydir/MyPart.h"
#include "../mydir/MySimVertex.h"
#include "../mydir/MyTracks.h"
#include "../mydir/MyVertex.h"

#include "../plugins/TrackPlots.h"
#include "../plugins/GenPartPlots.h"
#include "../plugins/VertexPlots.h"
#include "../plugins/UEPlots.h"
#include "../plugins/UEFactors.h"

#include "../plugins/EfficiencyPlots.h"

bool debug = false;

//#include "cuts.C"
#include "fileManager.C"
#include "files_funcs.C"
#include "FBacc.C"
#include "FBcuts.C"
#include "NCHEvtSel.C"
#include "binningMap.C"

int pthatcut(TString);
//TH2F Get_nevt_pthat_FromFile(TString, TString);

void UnderlyingEvent2( bool isMC  = true, const Int_t nevt_max = 100000000, bool WeighEvt = true, bool WeighTr = true, bool PtCorrection = true, bool isSiSCone = true){
     
  pt_cut = 0.5;
  eta_cut = 2.0; 
  
  //TH1F* track_pt_noEvtSel = new TH1F("track_pt_noEvtSel", "pt_noEvtSel;
  //TH1F*
  
  vector<double> ptBinning = makeBins(14, 0., 14.);
  addBins(5, 2., ptBinning);
  addBins(2, 4., ptBinning);
  addBins(4, 8., ptBinning);
  addBins(1, 36., ptBinning);
  
  vector<double> trackptBinning = makeBins(20,0.,1.);
  addBins(10,.2,trackptBinning);
  addBins(4,.5,trackptBinning);
  addBins(5,1,trackptBinning);
  addBins(1,50,trackptBinning);
  
  vector<double> etaBinning = makeBins(60,-3.,3.);
  vector<double> tracketaBinning = makeBins(2,-2.,-1.8);
  addBins(18, 0.2, tracketaBinning);
  addBins(2, 0.1, tracketaBinning);
  
    
  //Binning and Efficiency plots for Tracks  
  BasePlots Binning;
  Binning.pt_nbin = trackptBinning.size()-1;
  Binning.setBinning_pt(&(trackptBinning.at(0)));
  Binning.eta_nbin = tracketaBinning.size()-1;
  Binning.setBinning_eta(&(tracketaBinning.at(0)));
  
  TrackPlots  trp_all = TrackPlots("all");
  TrackPlots  trp_sel = TrackPlots("sel");
  GenPartPlots gpp_sel = GenPartPlots("sel");
  VertexPlots vtx_sel = VertexPlots("sel");
  
  EfficiencyPlots treffp_noEvtSel("TrNoEvtSel");
  treffp_noEvtSel.dR_max = 0.5;
  EfficiencyPlots treffp_evtSel("TrEvtSel");
  treffp_evtSel.dR_max = 0.5;
  
  EfficiencyPlots treffp_evtSel_nocuts("TrEvtSel_nocuts");
  treffp_evtSel_nocuts.dR_max = 0.5;
  
  //Binning and efficiency plots for Jets
  Binning.pt_nbin = ptBinning.size()-1;
  Binning.setBinning_pt(&(ptBinning.at(0)));
  Binning.eta_nbin = etaBinning.size()-1;
  Binning.setBinning_eta(&(etaBinning.at(0)));
  
  EfficiencyPlots jeteffp_noEvtSel("JetNoEvtSel");
  jeteffp_noEvtSel.dR_max = 0.5;
  EfficiencyPlots jeteffp_evtSel("JetEvtSel");
  jeteffp_evtSel.dR_max = 0.5;
  
  EfficiencyPlots jeteffp_evtSel_nocuts("JetEvtSel_nocuts");
  treffp_evtSel_nocuts.dR_max = 0.5;
  
  EfficiencyPlots leadjeteffp_evtSel("LeadJetEvtSel");
  leadjeteffp_evtSel.dR_max = 0.5;
  
  //UEPlots
  UEPlots genpart1part05 = UEPlots("genpart1part05", ptBinning);
  UEPlots genjet1part05 = UEPlots("genjet1part05", ptBinning);
  UEPlots genjet1part05_vtxsel = UEPlots("genjet1part05_vtxsel", ptBinning);
  UEPlots genjet1part05_minbias = UEPlots("genjet1part05_minbias", ptBinning);
    
  UEPlots truegenjet1part05 = UEPlots("truegenjet1part05", ptBinning);
  UEPlots recojet1genpart05 = UEPlots("recojet1genpart05", ptBinning);
  UEPlots genjet1recopart05 = UEPlots("genjet1recopart05", ptBinning);
  //UEPlots truegenjet1part05_recofound = UEPlots("truegenjet1part05_recofound");
  //UEPlots genjet1part05_recofound = UEPlots("genjet1part05_recofound");
  
  UEPlots recopart1part05 = UEPlots("recopart1part05", ptBinning);
  UEPlots recojet1part05 = UEPlots("recojet1part05", ptBinning);
  UEPlots recojet1part05_LeadGenMatch = UEPlots("recojet1part05_LeadGenMatch", ptBinning);
  UEPlots recojet1part05_LeadGenUnMatch = UEPlots("recojet1part05_LeadGenUnMatch", ptBinning);
  
  //Histograms for pthat cuts
  TH1F* pthat = new TH1F("pthat", "pthat;pthat;# evt", 200, 0, 100);
  TH2F* nevt_pthat = new TH2F("nevt_pthat", "nevt_pthat;bin_pthat;pthat_cut;# evt", 4, 1, 5, 4, 1, 5);
  
  UEFactors multcorr = UEFactors("#ch", ptBinning);
  //for (int i = 0; i < 27; i++)
  //    cout<< multcorr.varbins[i] << endl;
  UEFactors ptsumcorr = UEFactors("ptsum", ptBinning);
  //for (int i = 0; i < 27; i++)
      //cout<< multcorr.varbins[i] << endl;
  int nEvt_aft_sel = 0;
  
  //looping once over MC and then once over Data
  //for (int mcdata = 1; mcdata <= 1; mcdata++)
  //{
      //getting the list of files
      vector<TString>* vfiles = new vector<TString>();
      vfiles = getListOfFiles("../filelist.txt");
      //vfiles->push_back("/user/rougny/cms/MinBias/CMSSW_4_1_5/src/UATree/UABaseTree/UABaseTree_genPt0.29.root");
      //vfiles->push_back("/user/rougny/cms/MinBias/CMSSW_4_1_5/src/UATree/UABaseTree/UABaseTree_withEtaCut_withAkJets.root");
      
      //Needs the fileManager.C from UAmulti
      //vector<TString>* vfiles = new vector<TString>(1,"rfio:/castor/cern.ch/user/x/xjanssen/data/CMSSW_4_1_2/UABaseTree_412_mc_v4/__MinBias_TuneZ2_2760GeV-pythia6__Spring11-START311_V2A-v2__GEN-SIM-RECODEBUG/UABaseTree_412_mc_v4__CMSSW_4_1_2__MinBias_TuneZ2_2760GeV-pythia6__Spring11-START311_V2A-v2__GEN-SIM-RECODEBUG_1_1_yV0.root"); 
      //vfiles->push_back("UABaseTree2.root"); 
      
      //Getting event, track and jetpt weights/correction factors
      TString sMCmodel = "";
      //sMCmodel = "d6t";
      sMCmodel = "z2";
      
      //TString effdir = "../../../../../efficiencies/";
      TString effdir = "dcap:///pnfs/iihe/cms/store/user/wywang/";
      
      if (WeighEvt)
      {          
         //nevt_pthat = &(Get_nevt_pthat_FromFile(effdir + "efficiencies_siscone" + sMCmodel + ".root", "nevt_pthat"));
         TFile* f = TFile::Open(effdir + "efficiencies_siscone" + sMCmodel + "_test.root" , "READ");
         if(f==0)
         {
            cout << "[UEPlots] WARNING !! Can't open file for ptcorr & ptratio." << endl;
         }
      
         nevt_pthat = (TH2F*) f->Get("nevt_pthat");
         if(nevt_pthat->GetEntries()==0)
         {
            cout << "[UEPlots] WARNING !! Couldn't fetch ptcorr " << "nevt_pthat" << " from file "<< effdir + "efficiencies_siscone" + sMCmodel + ".root" << "." << endl;
         }
      }
      //else
      //   nevt_pthat = new TH2F("nevt_pthat", "nevt_pthat;bin_pthat;pthat_cut;# evt", 4, 1, 5, 4, 1, 5);
      /*
      TString sPtHatCut = "pt0";
      switch(pthatcut(vfiles->at(0)))
      {
          case 1:
               sPtHatCut = "pt15";
               break;
          case 2:
               sPtHatCut = "pt50";
               break;
          case 3:        
               sPtHatCut = "pt80";  
               break;                                   
      }
      */
      if (WeighTr) 
      {
         if (!recopart1part05.GetTrEffFromFile(effdir + "efficiencies_siscone" + sMCmodel + "_test2.root", "EfficiencyPlots_TrEvtSel_nocuts/TOperation_class_pt_eta_eff_TrEvtSel_nocuts", "EfficiencyPlots_TrEvtSel_nocuts/TOperation_class_pt_eta_fake_TrEvtSel_nocuts")) return;
         if (!recojet1part05.GetTrEffFromFile(effdir + "efficiencies_siscone" + sMCmodel + "_test2.root", "EfficiencyPlots_TrEvtSel_nocuts/TOperation_class_pt_eta_eff_TrEvtSel_nocuts", "EfficiencyPlots_TrEvtSel_nocuts/TOperation_class_pt_eta_fake_TrEvtSel_nocuts")) return;
         if (!recojet1part05_LeadGenMatch.GetTrEffFromFile(effdir + "efficiencies_siscone" + sMCmodel + "_test2.root", "EfficiencyPlots_TrEvtSel_nocuts/TOperation_class_pt_eta_eff_TrEvtSel_nocuts", "EfficiencyPlots_TrEvtSel_nocuts/TOperation_class_pt_eta_fake_TrEvtSel_nocuts")) return;
         if (!recojet1part05_LeadGenUnMatch.GetTrEffFromFile(effdir + "efficiencies_siscone" + sMCmodel + "_test2.root", "EfficiencyPlots_TrEvtSel_nocuts/TOperation_class_pt_eta_eff_TrEvtSel_nocuts", "EfficiencyPlots_TrEvtSel_nocuts/TOperation_class_pt_eta_fake_TrEvtSel_nocuts")) return;
      }
      
      if (PtCorrection) 
      {
         if (isSiSCone)
         {
            if (!recojet1genpart05.GetPtCorrFromFile(effdir + "efficiencies_siscone" + sMCmodel + "_test2.root", "UEFactors_<#ch>/pt_corr", "UEFactors_<#ch>/Profile_PtTrackJet_PtGenJet_matched")) return;
            if (!recojet1part05.GetPtCorrFromFile(effdir + "efficiencies_siscone" + sMCmodel + "_test2.root", "UEFactors_<#ch>/pt_corr", "UEFactors_<#ch>/Profile_PtTrackJet_PtGenJet_matched")) return;
            if (!recojet1part05_LeadGenMatch.GetPtCorrFromFile(effdir + "efficiencies_siscone" + sMCmodel + "_test2.root", "UEFactors_<#ch>/pt_corr", "UEFactors_<#ch>/Profile_PtTrackJet_PtGenJet_matched")) return;
            if (!recojet1part05_LeadGenUnMatch.GetPtCorrFromFile(effdir + "efficiencies_siscone" + sMCmodel + "_test2.root", "UEFactors_<#ch>/pt_corr", "UEFactors_<#ch>/Profile_PtTrackJet_PtGenJet_matched")) return;
         }
         else
         {
            if (!recojet1genpart05.GetPtCorrFromFile(effdir + "efficiencies_ak" + sMCmodel + "_test2.root", "UEFactors_<#ch>/pt_corr", "UEFactors_<#ch>/Profile_PtTrackJet_PtGenJet_matched")) return;
            if (!recojet1part05.GetPtCorrFromFile(effdir + "efficiencies_ak" + sMCmodel + "_test2.root", "UEFactors_<#ch>/pt_corr", "UEFactors_<#ch>/Profile_PtTrackJet_PtGenJet_matched")) return;
            if (!recojet1part05_LeadGenMatch.GetPtCorrFromFile(effdir + "efficiencies_ak" + sMCmodel + "_test2.root", "UEFactors_<#ch>/pt_corr", "UEFactors_<#ch>/Profile_PtTrackJet_PtGenJet_matched")) return;
            if (!recojet1part05_LeadGenUnMatch.GetPtCorrFromFile(effdir + "efficiencies_ak" + sMCmodel + "_test2.root", "UEFactors_<#ch>/pt_corr", "UEFactors_<#ch>/Profile_PtTrackJet_PtGenJet_matched")) return;
         }  
      }
      
      //Declaration of tree and its branches variables
      TTree* tree = new TTree("evt","");
      MyEvtId*           evtId        = NULL;
      MyHLTrig*  HLTrig               = NULL;
      MyL1Trig*  L1Trig               = NULL;
      vector<MyGenPart>* genPart      = NULL;
      vector<MyTracks>*  tracks       = NULL;
      vector<MyVertex>*  vertex       = NULL;
      MyBeamSpot*        beamSpot     = NULL;
      vector<MyBaseJet>* jet          = NULL;
      vector<MyBaseJet>* genjet       = NULL;
      vector<MyPart>* generatedpart   = new vector<MyPart>;
      vector<MyPart>* reconpart       = new vector<MyPart>;
      vector<MyPart>* reconjet        = new vector<MyPart>;
      vector<MyPart>* generatedjet    = new vector<MyPart>;
      MyGenKin* genkin                = NULL;
      
      // etc ....
      //Put All Classes you want to read here !!
      
      double EvtWeight;
      int i_tot = 0 , nevt_tot = 0;
      //int nevt15 = 0, nevt50 = 0, nevt80 = 0;
      
      //starting Loop over files, stops at end of list of files or when reached nevt_max
      for(vector<TString>::iterator itfiles = vfiles->begin() ; itfiles != vfiles->end() && (i_tot < nevt_max||nevt_max==-1) ; ++itfiles){
      //if (i_tot == 0)
      //   for (int i = 0; i < 27; i++)
      //       cout<< multcorr.varbins[i] << endl;
        TFile* file = TFile::Open(*itfiles,"READ");
     
        //getting the tree from the current file
        tree = (TTree*) file->Get("evt");

        //adding branches to the tree ----------------------------------------------------------------------
        tree->SetBranchStatus("*", 1);
        
        tree->SetBranchStatus("evtId", 1);
        tree->SetBranchAddress("evtId",&evtId);
        tree->SetBranchStatus("generalTracks", 1);//WeighEvt);
        tree->SetBranchAddress("generalTracks",&tracks); 
        tree->SetBranchStatus("offlinePrimaryVertices", 1);//WeighEvt);
        tree->SetBranchAddress("offlinePrimaryVertices",&vertex);
        tree->SetBranchStatus("HLTrig", 1);//WeighEvt);
        tree->SetBranchAddress("HLTrig",&HLTrig);
        tree->SetBranchStatus("L1Trig", 1);//WeighEvt);
        tree->SetBranchAddress("L1Trig",&L1Trig);
        tree->SetBranchStatus("offlineBeamSpot", 1);//WeighEvt);
        tree->SetBranchAddress("offlineBeamSpot",&beamSpot);
        if (isSiSCone)
        {
           tree->SetBranchStatus("ueSisCone5TracksJet500", 1);//WeighEvt);
           tree->SetBranchAddress("ueSisCone5TracksJet500",&jet);
        }
        else
        {
           tree->SetBranchStatus("ueAk5TracksJet500", 1);//WeighEvt);
           tree->SetBranchAddress("ueAk5TracksJet500",&jet);
        }
    
        if(isMC) { 
            tree->SetBranchStatus("genPart", 1);//WeighEvt);
            tree->SetBranchAddress("genPart",&genPart);
            if (isSiSCone)
            {
               tree->SetBranchStatus("ueSisCone5ChgGenJet500", 1);//WeighEvt);
               tree->SetBranchAddress("ueSisCone5ChgGenJet500",&genjet);
            } 
            else
            {
               tree->SetBranchStatus("ueAk5ChgGenJet500", 1);//WeighEvt);
               tree->SetBranchAddress("ueAk5ChgGenJet500",&genjet);
            }  
            tree->SetBranchStatus("genKin", 1);
            tree->SetBranchAddress("genKin", &genkin);     
        }
        
        //Calculating EvtWeight
        EvtWeight = 1.;
        int iPtHatCut = pthatcut(*itfiles);
        if (WeighEvt)
        {
          if (iPtHatCut == 0) 
             EvtWeight = 1;
          else
          {
             int it_iPtHatCut = iPtHatCut;
             while(it_iPtHatCut != 0)
             {
                EvtWeight = EvtWeight*nevt_pthat->GetBinContent(it_iPtHatCut, it_iPtHatCut)/nevt_pthat->GetBinContent(it_iPtHatCut, it_iPtHatCut+1); 
                cout<< "pthatsmallevtno: " << nevt_pthat->GetBinContent(it_iPtHatCut, it_iPtHatCut) << " pthatbigevtno: " << nevt_pthat->GetBinContent(it_iPtHatCut, it_iPtHatCut+1) << endl;
                --it_iPtHatCut;
             }
          } 
          cout<< "EvtWeight: " << EvtWeight << endl;                                
        }
    
        //Getting number of events
        int nev = int(tree->GetEntriesFast());
        nevt_tot += nev;
        cout <<"The current file has " << nev << " entries : "<< endl << *itfiles << endl;
             
        //starting loop over events, stops when reached end of file or nevt_max
        for(int i = 0; i < nev && (i_tot < nevt_max||nevt_max==-1); ++i , ++i_tot){
          //cout<< "Event Number: " << i_tot << endl;
          //printing the number of events done every hundred events
          //if( ((i_tot+1) % 100) == 0) cout <<int(double(i_tot+1))<<" events done"<<endl;
          //printing the % of events done every 10k evts
          if( ((i_tot+1) % 10000) == 0) cout <<int(double(i_tot+1)/1000)<<"k done"<<endl;
          
          //Filling the variables defined setting branches
          tree->GetEntry(i);
          
          if (isMC)
          {  
             if (!WeighEvt)
             {
                 if(genkin->PtHat >= 15) nevt_pthat->Fill(1, iPtHatCut+1);
                 if(genkin->PtHat >= 50) nevt_pthat->Fill(2, iPtHatCut+1);
                 if(genkin->PtHat >= 80) nevt_pthat->Fill(3, iPtHatCut+1);
                 continue;
             }
             //else
             //{
                 // Event Selection based on pthat cuts
             //    if((genkin->PtHat >= 15) && (genkin->PtHat < 50) && (iPtHatCut != 1)) continue;
             //    if((genkin->PtHat >= 50) && (genkin->PtHat < 80) && (iPtHatCut != 2)) continue;
             //    if((genkin->PtHat >= 80) && (iPtHatCut != 3)) continue;   
             //}
             pthat->Fill(genkin->PtHat, EvtWeight);
          }
          
          //... Get Vertex Info
          //bool hasvertex=false;
          int nvertex=0; 
          int vertexid=-1; 
          for(vector<MyVertex>::iterator v = vertex->begin() ; v != vertex->end() ; ++v)
          {
            if ( v->ndof <= 4) continue;
            if ( fabs(v->z - beamSpot->z) >= 10 ) continue;                             
            if (!v->fake)
            {
              //hasvertex=true;
    	      ++nvertex;
    	      vertexid=(v->id);
            }			 
          }
          
          //... Get Trigger Info
          bool L1_veto = (      !L1Trig->fTechDecisionBefore[36]
                           && !L1Trig->fTechDecisionBefore[37]
                           && !L1Trig->fTechDecisionBefore[38]
                           && !L1Trig->fTechDecisionBefore[39] );
          bool L1_BSC  = ( (L1Trig->fPhysDecisionBefore[126] && !isMC) ||
                         (L1Trig->fTechDecisionBefore[34] && isMC) ) ;
          bool HLT_BSC = ((HLTrig->HLTmap["HLT_L1BscMinBiasORBptxPlusANDMinus_v1"] && !isMC) || isMC ) ;
          bool HLT_ZeroBias = (isMC || HLTrig->HLTmap["HLT_ZeroBias_v1"]);
    
          bool TRG_MinBias =(L1_veto && L1_BSC && HLT_BSC);
          bool TRG_ZeroBias=(HLT_ZeroBias);
          
          //Reducing the size of the track, genpart collection
          vector<MyTracks> trcoll = *tracks;
          getPrimaryTracks(trcoll,vertexid,pt_cut,eta_cut);
          
          vector<MyTracks> trcoll_nocuts = *tracks;
          getPrimaryTracks(trcoll_nocuts,vertexid, (double) 0, (double) 3);
          
          vector<MyBaseJet> jetcoll = *jet;
          getPrimaryJets(jetcoll, pt_cut+0.5, eta_cut);
          
          vector<MyBaseJet> jetcoll_nocuts = *jet;
          getPrimaryJets(jetcoll_nocuts, (double) 0, (double) 3);
          
          //Filling GenPart and GenJet before selection
          vector<MyGenPart> gpcoll;
          vector<MyGenPart> gpcoll_nocuts;
          vector<MyBaseJet> genjetcoll;
          vector<MyBaseJet> genjetcoll_nocuts;
          
          MyPart pp;
          MyPart jp;
             
          if (isMC)
          {
             gpcoll = *genPart;
             gpcoll_nocuts = *genPart;
             genjetcoll = *genjet;
             genjetcoll_nocuts = *genjet;
             
             getPrimaryGenPart(gpcoll, pt_cut, eta_cut);
             getPrimaryGenPart(gpcoll_nocuts, (double) 0, (double) 3);
             getPrimaryJets(genjetcoll, pt_cut+0.5, eta_cut);
             getPrimaryJets(genjetcoll_nocuts, (double) 0, (double) 3);

             //truegenjet1part05_recofound.clean(*genPart, true);
             
             generatedpart->clear();
             generatedpart->reserve(gpcoll.size());
             for(vector<MyGenPart>::iterator it_tr = gpcoll.begin() ; it_tr != gpcoll.end() ; ++it_tr){
               
               pp.charge = it_tr->charge;
               pp.SetPxPyPzE(it_tr->Px(), it_tr->Py(), it_tr->Pz(), it_tr->E());
               generatedpart->push_back(pp);                             
             }
             
             generatedjet->clear();
             generatedjet->reserve(genjetcoll.size());
             for(vector<MyBaseJet>::iterator it_jet = genjetcoll.begin() ; it_jet != genjetcoll.end() ; ++it_jet){
               
               //pp.charge = it_tr->charge;
               jp.SetPxPyPzE(it_jet->Px(), it_jet->Py(), it_jet->Pz(), it_jet->E());
               generatedjet->push_back(jp);                             
             }
             
             treffp_noEvtSel.fill(&gpcoll, &trcoll, EvtWeight);
             jeteffp_noEvtSel.fill(&getLeading(jetcoll), &genjetcoll, EvtWeight);
             truegenjet1part05.fill(*generatedjet, *generatedpart, pt_cut+0.5, eta_cut, pt_cut, eta_cut, EvtWeight);
             
             // Fill UEPlots with different event selection conditions
             if (nvertex==1) genjet1part05_vtxsel.fill(*generatedjet, *generatedpart, pt_cut+0.5, eta_cut, pt_cut, eta_cut, EvtWeight);
             if (TRG_MinBias) genjet1part05_minbias.fill(*generatedjet, *generatedpart, pt_cut+0.5, eta_cut, pt_cut, eta_cut, EvtWeight);
            
             //truegenjet1part05_recofound.fill(*generatedjet, *generatedpart, 1, 2.0, 0.5, 2.5);
          }
          
          // ... Event Selection
          
          if ( ! TRG_MinBias ) continue;
          if ( nvertex!=1    ) continue;
          
          //if ( !passVtx(*vertex)) continue;
          //if ( (vertex->at(0)).ndof < 5) continue;
          //if ( fabs((vertex->at(0)).z - beamSpot->z) > 9 ) continue;
          
          nEvt_aft_sel++;
          //cout<< nEvt_aft_sel << endl;
    
          //-------------------------------------------------------------------------------------------------
          //filling pt distribution for the generated particles
          //ie those from pythia generator, without reconstruction
         
          reconpart->clear();
          reconpart->reserve(trcoll.size());
          for(vector<MyTracks>::iterator it_tr = trcoll.begin() ; it_tr != trcoll.end() ; ++it_tr){
        
             pp.charge = it_tr->charge;
             pp.SetPxPyPzE(it_tr->Px(), it_tr->Py(), it_tr->Pz(), it_tr->E());
             reconpart->push_back(pp);                             
          }  
        
          reconjet->clear();
          reconjet->reserve(jetcoll.size());
          for(vector<MyBaseJet>::iterator it_jet = jetcoll.begin() ; it_jet != jetcoll.end() ; ++it_jet){
         
             jp.SetPxPyPzE(it_jet->Px(), it_jet->Py(), it_jet->Pz(), it_jet->E());
             reconjet->push_back(jp);                             
          }
          
          if(isMC)
          { 
            //genpart1part05.fill(*generatedpart, 1, 2.4, 0.5, 2.4);
            
            treffp_evtSel.fill(&gpcoll, &trcoll, EvtWeight);
            treffp_evtSel_nocuts.fill(&gpcoll_nocuts, &trcoll_nocuts, EvtWeight);
            jeteffp_evtSel.fill(&getLeading(jetcoll) , &genjetcoll, EvtWeight);
            jeteffp_evtSel_nocuts.fill(&getLeading(jetcoll_nocuts), &genjetcoll_nocuts, EvtWeight);
            leadjeteffp_evtSel.fill(&getLeading(genjetcoll), &getLeading(jetcoll), EvtWeight);
            genjet1part05.fill(*generatedjet, *generatedpart, pt_cut+0.5, eta_cut, pt_cut, eta_cut, EvtWeight);
                    
            //recojet1genpart05.fill(*reconjet, *generatedpart, pt_cut+0.5, eta_cut, pt_cut, eta_cut, EvtWeight, false, PtCorrection);
            //genjet1recopart05.fill(*generatedjet, *reconpart, pt_cut+0.5, eta_cut, pt_cut, eta_cut, EvtWeight, WeighTr);
            
            if (isLeadingMatched(*reconjet, *generatedjet, 0.5))
               recojet1part05_LeadGenMatch.fill(*reconjet, *reconpart, pt_cut+0.5, eta_cut, pt_cut, eta_cut, EvtWeight, WeighTr, PtCorrection);
            else 
               recojet1part05_LeadGenUnMatch.fill(*reconjet, *reconpart, pt_cut+0.5, eta_cut, pt_cut, eta_cut, EvtWeight, WeighTr, PtCorrection);
                 
            //if ((ilead_reco_jettest != -1) && (jet->at(ilead_reco_jettest).Eta() <= 2) && (jet->at(ilead_reco_jettest).Eta() >= -2) && (jet->at(ilead_reco_jettest).Pt() >= 1.0))
            //   genjet1part05_recofound.fill(*generatedjet, *generatedpart, 1, 2.0, 0.5, 2.5);
          
            multcorr.fill(genjetcoll, jetcoll, pt_cut+0.5, eta_cut, EvtWeight);
            ptsumcorr.fill(genjetcoll, jetcoll, pt_cut+0.5, eta_cut, EvtWeight);
            
          }
          
    
          //-------------------------------------------------------------------------------------------------
          //filling pt distribution for the observed/reconstructed tracks in the detector, finding leading particle, filling dpt & deta against leading particle
          
          //recopart1part05.fill(*reconpart, pt_cut, eta_cut, pt_cut, eta_cut, EvtWeight, WeighTr);
          recojet1part05.fill(*reconjet, *reconpart, pt_cut+0.5, eta_cut, pt_cut, eta_cut, EvtWeight, WeighTr, PtCorrection);
    
          vtx_sel.fill(*vertex, EvtWeight);
          trp_sel.fill(trcoll,(vector<MyVertex>::iterator&) *vertex,vertexid,beamSpot,EvtWeight);
          gpp_sel.fill(gpcoll, EvtWeight);
         
        }//end of loop over events
        
        //Closing current files
        file->Close();
      }//end of loop over files

      //output file
      if (isMC)
      {
          TFile* outputMC = new TFile("outputMC.root","RECREATE");
          outputMC->cd();
          
          trp_sel.write();
          gpp_sel.write();
          vtx_sel.write();
          
          treffp_noEvtSel.write();
          treffp_evtSel.write();
          treffp_evtSel_nocuts.write();
          jeteffp_noEvtSel.write();
          jeteffp_evtSel.write();
          jeteffp_evtSel_nocuts.write();
          leadjeteffp_evtSel.write();
    
          //genpart1part05.write();
          genjet1part05.write();
          genjet1part05_vtxsel.write();
          genjet1part05_minbias.write();
          
          //recojet1genpart05.write();
          //genjet1recopart05.write();
          truegenjet1part05.write();
          
          ///genjet1part05_recofound.write();
          //truegenjet1part05_recofound.write();
          
          //Correlation matrix
          multcorr.correctgenreco((TProfile*) recojet1part05.profile_n_mult_trans, (TProfile*) recojet1genpart05.profile_n_mult_trans, (TProfile*) recojet1part05.profile_n_mult_trans);
          ptsumcorr.correctgenreco((TProfile*) recojet1part05.profile_ptsum_trans, (TProfile*) recojet1genpart05.profile_ptsum_trans, (TProfile*) recojet1part05.profile_ptsum_trans);
          multcorr.correctmatrix((TProfile*) multcorr.profile_corrected_genreco);
          ptsumcorr.correctmatrix((TProfile*) ptsumcorr.profile_corrected_genreco);
          multcorr.correcttruegengen((TProfile*) multcorr.profile_corrected_matrix, (TProfile*) truegenjet1part05.profile_n_mult_trans, (TProfile*) genjet1part05.profile_n_mult_trans);
          ptsumcorr.correcttruegengen((TProfile*) ptsumcorr.profile_corrected_matrix, (TProfile*) truegenjet1part05.profile_ptsum_trans, (TProfile*) genjet1part05.profile_ptsum_trans);
          
          //Simple bin-by-bin
          //multcorr.correctgenreco((TProfile*) recojet1part05.profile_n_mult_trans, (TProfile*) genjet1part05.profile_n_mult_trans, (TProfile*) recojet1part05.profile_n_mult_trans);
          //ptsumcorr.correctgenreco((TProfile*) recojet1part05.profile_ptsum_trans, (TProfile*) genjet1part05.profile_ptsum_trans, (TProfile*) recojet1part05.profile_ptsum_trans);
          ////multcorr.correctmatrix((TProfile*) recojet1genpart05.profile_n_mult_trans);
          //multcorr.correcttruegengen((TProfile*) multcorr.profile_corrected_genreco, (TProfile*) truegenjet1part05.profile_n_mult_trans, (TProfile*) genjet1part05.profile_n_mult_trans);
          //ptsumcorr.correcttruegengen((TProfile*) ptsumcorr.profile_corrected_genreco, (TProfile*) truegenjet1part05.profile_ptsum_trans, (TProfile*) genjet1part05.profile_ptsum_trans);
                  
          multcorr.write();
          ptsumcorr.write();
          
          //recopart1part05.write();
          recojet1part05.write();
          recojet1part05_LeadGenMatch.write();
          recojet1part05_LeadGenUnMatch.write();
          
          pthat->Write();
          nevt_pthat->Write();
          
          outputMC->Close();
      }
      else
      {
          TFile* outputData = new TFile("outputData.root","RECREATE");
          outputData->cd();
                  
          trp_sel.write();
          gpp_sel.write();
          vtx_sel.write();
          
          //recopart1part05.write();
          recojet1part05.write();
          
          outputData->Close();
              
      }
  
  //    isMC = false;
  //}//end of loop over mc and data
}

int pthatcut(TString filename)
{
    if (filename.Contains("QCD_Pt_15")) return 1;
    if (filename.Contains("QCD_Pt_50")) return 2;
    if (filename.Contains("QCD_Pt_80")) return 3;
    return 0;
}

/*
TH2F Get_nevt_pthat_FromFile(TString fstr , TString nevt_pthat_str){
  TFile* f = TFile::Open(fstr , "READ");
  if(f==0){
    cout << "[UEPlots] WARNING !! Can't open file for ptcorr & ptratio." << endl;
  }
  
  TH2F nevt_pthat = *((TH2F*) f->Get(nevt_pthat_str));
  if(nevt_pthat.GetEntries()==0){
    cout << "[UEPlots] WARNING !! Couldn't fetch ptcorr " << nevt_pthat_str << " from file "<< fstr << "." << endl;
  }
  
  return nevt_pthat;
}
*/
