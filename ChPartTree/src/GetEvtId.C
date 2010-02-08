// Package:     ChPartTree
// Class:       ChPartTree
// author:      Xavier Janssen
// Date:        10/12/2009 
//
// Description: Function to retrieve Evt Id information

//  system include files
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

bool EvtIdDebug = false;

void ChPartTree::GetEvtId(const edm::Event& iEvent)
{
   using namespace std;

   if ( EvtIdDebug )
   {
     cout << "XJ: EvdId: " << iEvent.id() << endl ;
     cout << "XJ: Run  : " << iEvent.id().run() << endl ;
     cout << "XJ: Evt  : " << iEvent.id().event() << endl ;
     cout << "XJ: Lumbl: " << iEvent.luminosityBlock() << endl ;
//   cout << "XJ: proID: " << iEvent.processHistoryID() << endl;
//   cout << "XJ: GUID : " << iEvent.processGUID() << endl;
     edm::Timestamp Time=iEvent.time();
     cout << "XJ: time : " << Time.value() << endl;
     cout << "XJ: isDa : " << iEvent.isRealData() << endl;
     cout << "XJ: expTy: " << iEvent.experimentType() << endl ;
     cout << "XJ: Bx   : " << iEvent.bunchCrossing() << endl;
     cout << "XJ: Orbit: " << iEvent.orbitNumber() << endl;
//   cout << "XJ: Store: " << iEvent.storeNumber() << endl; 
   }

   EvtId.Reset();

   EvtId.Run       = iEvent.id().run() ;   
   EvtId.Evt       = iEvent.id().event() ;
   EvtId.LumiSect  = iEvent.luminosityBlock();
   edm::Timestamp Time=iEvent.time();
   EvtId.Time      = Time.value(); 
   EvtId.IsData    = iEvent.isRealData();
   EvtId.ExpType   = iEvent.experimentType();
   EvtId.Bunch     = iEvent.bunchCrossing();
   EvtId.Orbit     = iEvent.orbitNumber();

}

