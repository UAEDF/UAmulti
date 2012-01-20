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

#include "../plugins/UECorrection.h"

bool debug = false;

//#include "cuts.C"
#include "fileManager.C"
#include "files_funcs.C"
#include "FBacc.C"
#include "FBcuts.C"
#include "NCHEvtSel.C"
#include "binningMap.C"

void UnderlyingEventCorrection(TString inputfile = "output_noweightsnopt.root"){
  
    vector<double> ptBinning = makeBins(14, 0., 14.);
    addBins(5, 2., ptBinning);
    addBins(2, 4., ptBinning);
    addBins(4, 8., ptBinning);
    addBins(1, 36., ptBinning);

    TFile* input = TFile::Open(inputfile,"READ");
    input->cd();
    if(input==0)
    {
         cout << "In UnderlyingEventCorrection: Could not retrieve '"+inputfile+"' for making corrections. Exiting now ..." << endl;
         return;
    }
    
    TString histname;
    histname = "UEPlots_recojet1part05/profile_#ch_trans_recojet1part05";
    TProfile* histNch = (TProfile*) input->Get(histname);
    if(histNch == 0)
    {    
        cout << "In UnderlyingEventCorrection: Couldn't find '"+histname+"'. Exiting now ..." << endl;
        return;      
    }
    
    histname = "UEPlots_recojet1part05/profile_ptsum_trans_recojet1part05";
    TProfile* histptsum = (TProfile*) input->Get(histname);
    if(histptsum == 0)
    {    
        cout << "In UnderlyingEventCorrection: Couldn't find '"+histname+"'. Exiting now ..." << endl;
        return;      
    }
    
    UECorrection corrNch = UECorrection("#ch", ptBinning);
    UECorrection corrptsum = UECorrection("ptsum", ptBinning);
    UECorrection corrNch2 = UECorrection("#ch", ptBinning);
    UECorrection corrptsum2 = UECorrection("ptsum", ptBinning);
    
    //Corrections for <#ch>
    //bool isCorrectGenReco = corrNch.CorrectGenReco(histNch);
    //cout<< "isCorrectGenReco: " << isCorrectGenReco << endl;
    //if (!isCorrectGenReco) return;
    if (!corrNch.CorrectMatrix(histNch, inputfile)) return;
    //if (!corrNch.CorrectGenReco((TProfile*) corrNch.profile_corrected_matrix, inputfile)) return;
    if (!corrNch.CorrectEvtSel((TProfile*) corrNch.profile_corrected_matrix, inputfile)) return;
    if (!corrNch2.CorrectMatrix2(histNch, inputfile)) return;
    //if (!corrNch2.CorrectGenReco((TProfile*) corrNch2.profile_corrected_matrix, inputfile)) return;
    if (!corrNch2.CorrectEvtSel((TProfile*) corrNch2.profile_corrected_matrix, inputfile)) return;
    
    //Corrections for <ptsum>
    if (!corrptsum.CorrectMatrix(histptsum, inputfile)) return;
    //if (!corrptsum.CorrectGenReco((TProfile*) corrNch.profile_corrected_matrix, inputfile)) return;
    if (!corrptsum.CorrectEvtSel((TProfile*) corrNch.profile_corrected_matrix, inputfile)) return;
    if (!corrptsum2.CorrectMatrix2(histptsum, inputfile)) return;
    //if (!corrptsum2.CorrectGenReco((TProfile*) corrNch2.profile_corrected_matrix, inputfile)) return;
    if (!corrptsum2.CorrectEvtSel((TProfile*) corrNch2.profile_corrected_matrix, inputfile)) return;
               
    TFile* outputCorrection = new TFile("outputCorrection.root","RECREATE");
    outputCorrection->cd();
    
    corrNch.write();
    corrNch2.write("2");
    corrptsum.write();
    corrptsum2.write("2");
    
    outputCorrection->Close();
     
}
