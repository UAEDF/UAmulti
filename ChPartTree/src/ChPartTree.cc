

// system include files
#include <memory>
#include <string>
#include <iostream>
   
// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/BeamSpot/interface/BeamSpot.h"

// ChPartTree Analysis class decleration
#include "UAmulti/ChPartTree/interface/ChPartTree.h"

int nEvtCut_ALL      = 0 ;
int nEvtCut_L1Common = 0 ;
int nEvtCut_L1Mult   = 0 ;
int nEvtCut_HLTMult  = 0 ;
int nEvtCut_HF       = 0 ;
int nEvtCut_VTX      = 0 ;
int nEvtCut_Ntracks  = 0 ;


ChPartTree::ChPartTree(const edm::ParameterSet& iConfig)

{
   //now do what ever initialization is needed

   // Get output filename
   fOutputFileName = iConfig.getUntrackedParameter<string>("fileName");

   // Modules to execute 

   StoreGenPart = iConfig.getParameter<bool>("StoreGenPart");
   StoreGenKine = iConfig.getParameter<bool>("StoreGenKine");

   // Define DATA Collection
   genPartColl_       = iConfig.getParameter<edm::InputTag>("genPartColl") ;
   hepMCColl_         = iConfig.getParameter<edm::InputTag>("hepMCColl") ;

   //Trigger inputs
   hlt_bits   = iConfig.getParameter<vector<string> >("requested_hlt_bits");

   // Calo Energy Sums
   caloTowerTag_ = iConfig.getParameter<edm::InputTag>("CaloTowerTag") ;
   energyThresholdHB_ = iConfig.getParameter<double>("EnergyThresholdHB") ;
   energyThresholdHE_ = iConfig.getParameter<double>("EnergyThresholdHE") ;
   energyThresholdHF_ = iConfig.getParameter<double>("EnergyThresholdHF") ;
   energyThresholdEB_ = iConfig.getParameter<double>("EnergyThresholdEB") ;
   energyThresholdEE_ = iConfig.getParameter<double>("EnergyThresholdEE") ;

}


ChPartTree::~ChPartTree()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to for each event  ------------
void
ChPartTree::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;

   // General Info

   GetEvtId(iEvent);
   GetL1Trig(iEvent,iSetup);
   GetHLTrig(iEvent,iSetup);

   // MC Info

   if (StoreGenKine) GetGenKin (iEvent);
   if (StoreGenPart) GetGenPart(iEvent,iSetup);

   // Data

   // Reset vtx id and vector
   vtxid = 0;
   vtxid_xyz.clear();

   // ... BeamSpot

   edm::Handle<reco::BeamSpot>      beamSpotHandle;
   iEvent.getByLabel("offlineBeamSpot", beamSpotHandle);
   const reco::BeamSpot* theBeamSpot = beamSpotHandle.product();
   beamSpot.x           = theBeamSpot->position().x();    
   beamSpot.y           = theBeamSpot->position().y();    
   beamSpot.z           = theBeamSpot->position().z();    
   beamSpot.sigmaZ      = theBeamSpot->sigmaZ();
   beamSpot.dxdz        = theBeamSpot->dxdz();
   beamSpot.dydz        = theBeamSpot->dydz();
   beamSpot.BeamWidthX  = theBeamSpot->BeamWidthX() ;
   beamSpot.BeamWidthY  = theBeamSpot->BeamWidthY() ;
   beamSpot.ex          = theBeamSpot->x0Error();
   beamSpot.ey          = theBeamSpot->y0Error();
   beamSpot.ez          = theBeamSpot->z0Error();
   beamSpot.esigmaZ     = theBeamSpot->sigmaZ0Error();
   beamSpot.edxdz       = theBeamSpot->dxdzError();
   beamSpot.edydz       = theBeamSpot->dydzError();
   beamSpot.eBeamWidthX = theBeamSpot->BeamWidthXError();
   beamSpot.eBeamWidthY = theBeamSpot->BeamWidthYError();

   // BeamSpot is vtx with id=0 
   vtxid++;
   vtxid_xyz.push_back(theBeamSpot->position());
   

   //cout << "BS: " << theBeamSpot->position().x() << " " << theBeamSpot->position().y() << " " << theBeamSpot->position().z() << endl;  

   // ... Vertex
   GetRecoVertex(iEvent,iSetup,"pixelVertices",pixelVertex);  
   GetRecoVertex(iEvent,iSetup,"offlinePrimaryVertices",primaryVertex);  
   GetRecoVertex(iEvent,iSetup,"pixel3Vertices",pixel3Vertex);  
   //GetRecoVertex(iEvent,iSetup,"generalVertices",ferencVtxGenTrk);  
   GetRecoVertex(iEvent,iSetup,"allVertices",ferencVtxFerTrk);  

   // ... Tracks
   GetRecoTracks(iEvent,iSetup,"generalTracks",generalTracks);
   GetRecoTracks(iEvent,iSetup,"allTracks",ferencTracks);
  
   // ... MIT VtxQuality
   GetMITEvtSel(iEvent,iSetup); 

   // ... FwdGap
   GetFwdGap(iEvent,iSetup); 

   // Event Selection (DATA ONLY) 
   bool badEvent = false;
   if ( EvtId.IsData ) { 

     ++nEvtCut_ALL;

     // ... Common Trigger selection: VETO + PhysBit
      if( ! (    !L1Trig.TechTrigWord[36]
              && !L1Trig.TechTrigWord[37]
              && !L1Trig.TechTrigWord[38]
              && !L1Trig.TechTrigWord[39] ) )  badEvent = true; 
//            &&  L1Trig.TechTrigWord[0]  ) )  badEvent = true;

     if ( ! badEvent ) ++nEvtCut_L1Common;

     // L1 Multiplicity
     if ( ! ( L1Trig.PhysTrigWord[124] || L1Trig.PhysTrigWord[63] ) ) badEvent = true;

     if ( ! badEvent ) ++nEvtCut_L1Mult;

     // HLT Multiplicity
     if ( ! (    HLTrig.HLTmap["HLT_L1_BscMinBiasOR_BptxPlusORMinus"] 
              || HLTrig.HLTmap["HLT_PixelTracks_Multiplicity40"]
              || HLTrig.HLTmap["HLT_PixelTracks_Multiplicity70"]
              || HLTrig.HLTmap["HLT_PixelTracks_Multiplicity85"]     ) ) badEvent = true;
  
     if ( ! badEvent ) ++nEvtCut_HLTMult;

/*
     // HF Cut 
     double hfEnergyMin = min(MITEvtSel.eHfPos,MITEvtSel.eHfNeg);
     if( ! (    MITEvtSel.nHfTowersP >= 1
             && MITEvtSel.nHfTowersN >= 1
             && hfEnergyMin >= 3. )          ) badEvent = true;

     if ( ! badEvent ) ++nEvtCut_HF;
*/

     // Vertex Cut
     bool goodVtx = false ;
     double vtxz_cut = 30.;
     for(vector<MyVertex>::iterator itvtx=primaryVertex.begin();itvtx!=primaryVertex.end();++itvtx){
       if(itvtx->validity && fabs(itvtx->z)<vtxz_cut && itvtx->ntracks>0 ) goodVtx = true;
     }
     for(vector<MyVertex>::iterator itvtx=pixel3Vertex.begin();itvtx!=pixel3Vertex.end();++itvtx){
       if(itvtx->validity && fabs(itvtx->z)<vtxz_cut && itvtx->ntracks>0 ) goodVtx = true;
     }
     for(vector<MyVertex>::iterator itvtx=ferencVtxGenTrk.begin();itvtx!=ferencVtxGenTrk.end();++itvtx){
       if(itvtx->validity && fabs(itvtx->z)<vtxz_cut && itvtx->ntracks>0 ) goodVtx = true;
     }
     if ( ! goodVtx ) badEvent = true;

     if ( ! badEvent ) ++nEvtCut_VTX;
/*
     // #Tracks Cut
     bool goodTracks = false ;
     int  nTracksCut = 0  ;
     if ( (signed) generalTracks.size() >= nTracksCut )  goodTracks = true ;
     if ( (signed) ferencTracks.size()  >= nTracksCut )  goodTracks = true ;

     if ( ! goodTracks ) badEvent = true;
 
     if ( ! badEvent ) ++nEvtCut_Ntracks;
*/

   } // End of Event Selection 

   if ( ! badEvent ) tree->Fill();
   // tree->Fill(); 
}


// ------------ method called once each job just before starting event loop  ------------
void 
ChPartTree::beginJob()
{

   fout = new TFile(fOutputFileName.c_str(), "RECREATE" ) ;
   tree = new TTree("evt","evt");
   //evt  = new MyEvent;
   // tree->Branch("MyEvent","MyEvent",&evt);

   // General Event Info 
   tree->Branch("EvtId",&EvtId);
   tree->Branch("L1Trig",&L1Trig);
   tree->Branch("HLTrig",&HLTrig);


   // MC Informations
   if (StoreGenKine) tree->Branch("GenKin",&GenKin);
   if (StoreGenPart) tree->Branch("GenPart",&GenPart);

   // RECO Information

   tree->Branch("beamSpot",&beamSpot);

   tree->Branch("pixelVertex",&pixelVertex);
   tree->Branch("primaryVertex",&primaryVertex);  
   tree->Branch("pixel3Vertex",&pixel3Vertex);  
   //tree->Branch("ferencVtxGenTrk",&ferencVtxGenTrk);
   tree->Branch("ferencVtxFerTrk",&ferencVtxFerTrk);

   tree->Branch("generalTracks",&generalTracks);
   tree->Branch("minbiasTracks",&ferencTracks);

   // MIT cuts
   tree->Branch("MITEvtSel",&MITEvtSel);

   // FwdGap

   tree->Branch("FwdGap",&FwdGap);


}

// ------------ method called once each job just after ending the event loop  ------------
void 
ChPartTree::endJob() {

   cout << " Events Passing cuts: "   << endl;
   cout << " ******************** "   << endl;
   cout << " ALL      = " << nEvtCut_ALL      << endl;
   cout << " L1Common = " << nEvtCut_L1Common << endl;
   cout << " L1Mult   = " << nEvtCut_L1Mult   << endl;
   cout << " HLTMult  = " << nEvtCut_HLTMult  << endl;
//   cout << " HF       = " << nEvtCut_HF       << endl;
   cout << " VTX      = " << nEvtCut_VTX      << endl;
//   cout << " #Tracks  = " << nEvtCut_Ntracks  << endl;

   fout->Write() ;
   fout->Close() ;

}

//define this as a plug-in
DEFINE_FWK_MODULE(ChPartTree);
