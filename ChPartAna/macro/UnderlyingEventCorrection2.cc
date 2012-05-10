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

void UnderlyingEventCorrection2(TString inputfile = "output_evtweight3.root"){
     
    vector<double> ptBinning = makeBins(14, 0., 14.);
    addBins(5, 2., ptBinning);
    addBins(2, 4., ptBinning);
    addBins(4, 8., ptBinning);
    addBins(1, 36., ptBinning);

    TH1D* truegen_gen_ratio = new TH1D("truegen_gen_mult_ratio", "truegen_gen_mult_ratio;Leading jet pt;<# truegen>/<# gen>", ptBinning.size()-1, &(ptBinning.at(0)));
    TH1D* nch_trans_recoevtselcorr = new TH1D("nch_trans_recoEvtEelCorr", "nch_trans_recoEvtSelCorr;Leading jet pt;<# ch>", ptBinning.size()-1, &(ptBinning.at(0)));
    
    TFile* input = TFile::Open(inputfile,"READ");
    input->cd();
    if(input==0)
    {
         cout << "In UnderlyingEventCorrection: Could not retrieve '"+inputfile+"' for making corrections. Exiting now ..." << endl;
         return;
    }
    
    TString histname;
    histname = "UEPlots_truegenjet1part05/profile_#ch_trans_truegenjet1part05";
    TProfile* truegenP = (TProfile*) input->Get(histname);
    if(truegenP == 0)
    {    
        cout << "In UnderlyingEventCorrection: Couldn't find '"+histname+"'. Exiting now ..." << endl;
        return;      
    }
    
    histname = "UEPlots_genjet1part05/profile_#ch_trans_genjet1part05";
    TProfile* genP = (TProfile*) input->Get(histname);
    if(genP == 0)
    {    
        cout << "In UnderlyingEventCorrection: Couldn't find '"+histname+"'. Exiting now ..." << endl;
        return;      
    }
    
    TH1D* truegen = truegenP->ProjectionX();
    TH1D* gen     = genP->ProjectionX();
    truegen_gen_ratio->Divide(truegen, gen, 1, 1, "B");
    
    histname = "UEPlots_recojet1part05/profile_#ch_trans_recojet1part05";
    TProfile* recoP = (TProfile*) input->Get(histname);
    if(recoP == 0)
    {    
        cout << "In UnderlyingEventCorrection: Couldn't find '"+histname+"'. Exiting now ..." << endl;
        return;      
    }
    
    nch_trans_recoevtselcorr->Multiply(recoP, truegen_gen_ratio);
               
    TFile* outputCorrection = new TFile("outputCorrection.root","RECREATE");
    outputCorrection->cd();
    
    truegen_gen_ratio->Write();
    nch_trans_recoevtselcorr->Write();
    
    outputCorrection->Close();
     
}
