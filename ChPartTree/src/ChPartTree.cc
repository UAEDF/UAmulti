

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

   vtxid = 0;
   GetRecoVertex(iEvent,iSetup,"pixelVertices",pixelVertex);  
   GetRecoVertex(iEvent,iSetup,"offlinePrimaryVertices",primaryVertex);  

   GetRecoTracks(iEvent,iSetup,"generalTracks",generalTracks);
   GetRecoTracks(iEvent,iSetup,"allTracks",ferencTracks);
  
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

   tree->Branch("pixelVertex",&pixelVertex);
   tree->Branch("primaryVertex",&primaryVertex);  

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
