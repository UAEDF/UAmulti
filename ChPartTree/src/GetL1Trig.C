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

// Trigger Inclides
#include "CondFormats/L1TObjects/interface/L1GtTriggerMenu.h"
#include "CondFormats/DataRecord/interface/L1GtTriggerMenuRcd.h"
#include "CondFormats/L1TObjects/interface/L1GtTriggerMenuFwd.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutSetupFwd.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutSetup.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerObjectMapRecord.h"
#include "L1Trigger/GlobalTriggerAnalyzer/interface/L1GtUtils.h"


// ChPartTree Analysis class decleration
#include "UAmulti/ChPartTree/interface/ChPartTree.h"

bool L1TrigDebug = false;

void ChPartTree::GetL1Trig(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace std;

  // Tests
/*
  if (L1TrigDebug)
  {  
   edm::ESHandle<L1GtTriggerMenu> menuRcd;
   iSetup.get<L1GtTriggerMenuRcd>().get(menuRcd) ;
   const L1GtTriggerMenu* menu = menuRcd.product();

   for (CItAlgo algo = menu->gtAlgorithmMap().begin(); 
                algo!=menu->gtAlgorithmMap().end(); 
                ++algo) 
   {
     cout << "Name: " << (algo->second).algoName() 
          << " Alias: " << (algo->second).algoAlias() 
          << " Bit: " << (algo->second).algoBitNumber()
           << " Result: " << l1AlgorithmResult(iEvent, iSetup, (algo->second).algoName() )
          << std::endl;
   }
  }
*/

   edm::Handle<L1GlobalTriggerReadoutRecord> L1GTRR;
   iEvent.getByLabel("gtDigis",L1GTRR);
   for (int i=0 ; i <128 ; i++) 
   {
     if (L1TrigDebug) cout << "PhysicsTriggerWord :" << i << " " << L1GTRR->decisionWord()[i] << endl;
     L1Trig.PhysTrigWord[i] = L1GTRR->decisionWord()[i];
   }
   for (int i=0 ; i <64  ; i++)
   {
    if (L1TrigDebug) cout << "technicalTriggerWord :" << i << " " << L1GTRR->technicalTriggerWord()[i] << endl;
     L1Trig.TechTrigWord[i] = L1GTRR->technicalTriggerWord()[i]; 
   }
}

