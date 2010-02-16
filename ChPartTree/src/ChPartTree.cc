

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

   tree->Fill();

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


}

// ------------ method called once each job just after ending the event loop  ------------
void 
ChPartTree::endJob() {

   fout->Write() ;
   fout->Close() ;

}

//define this as a plug-in
DEFINE_FWK_MODULE(ChPartTree);
