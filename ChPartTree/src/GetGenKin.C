// Package:     ChPartTree
// Class:       ChPartTree
// author:      Xavier Janssen
// Date:        10/12/2009 
//
// Description: Function to retrieve Generated Kinematic et al.

//  system include files
#include <iostream>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"


// DataFormats
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Candidate/interface/CandidateFwd.h"

#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include "DataFormats/METReco/interface/GenMET.h"
#include "DataFormats/METReco/interface/GenMETCollection.h"


// ChPartTree Analysis class decleration
#include "UAmulti/ChPartTree/interface/ChPartTree.h"


bool GenKinDebug = false;

void ChPartTree::GetGenKin(const edm::Event& iEvent)
{
   using namespace std;
   using namespace edm;
   using namespace reco;

/* 
   // MC Process Id and PtHat for AOD

   Handle< int > genProcessID;
   iEvent.getByLabel( "genEventProcID", genProcessID );
   int processId = *genProcessID;
   cout<<"Process ID : "<<processId<<endl;

*/

   // MC Process Id and PtHat for RECO
   Handle<HepMCProduct> hepMCHandle;
   iEvent.getByLabel(hepMCColl_, hepMCHandle ) ;
   const HepMC::GenEvent* GenEvt = hepMCHandle->GetEvent() ;
   int processId = GenEvt->signal_process_id();
   if (GenKinDebug) cout<<"Process ID2: "<<processId<<endl; 
//   double ptHat  = GenEvt->event_scale();
//   if (GenKinDebug) cout<<"PtHat MC : "<<ptHat<<endl; 

   GenKin.MCProcId  = processId ; 

   GenKin.Scale     = 0. ;
   GenKin.x1 = 0. ;
   GenKin.x2 = 0. ;
   GenKin.Q  = 0. ;
   GenKin.Part1Id = 0 ; 
   GenKin.Part2Id = 0 ; 
 
   GenKin.Met    = 0. ;
   GenKin.MetX   = 0. ;
   GenKin.MetY   = 0. ;
   GenKin.MetPhi = 0. ;


/*
   GenKin.Scale     = ptHat ;

   // PDF Info -> x,Q, parton id's -> see HepMC manual
   const HepMC::PdfInfo* PdfInfo = GenEvt->pdf_info();
   double x1 = PdfInfo->x1();
   double x2 = PdfInfo->x2();
   double Q  = PdfInfo->scalePDF(); 

   int id1 = PdfInfo->id1();
   int id2 = PdfInfo->id2();

   GenKin.x1 = x1 ;
   GenKin.x2 = x2 ;
   GenKin.Q  = Q  ;
   GenKin.Part1Id = id1 ; 
   GenKin.Part2Id = id2 ; 

   // Gen MET: From GenMETCollection  

   Handle<GenMETCollection> genmet;
//   iEvent.getByLabel(GenMetColl_ , genmet);
   iEvent.getByLabel("genMetTrue" , genmet);

   if (GenKinDebug) 
   { 
     cout << "GenMET et()  = " << (genmet->front()).et() << endl ;
     cout << "GenMET px()  = " << (genmet->front()).px() << endl ;
     cout << "GenMET py()  = " << (genmet->front()).py() << endl ;
     cout << "GenMET phi() = " << (genmet->front()).phi() << endl;
   }

   GenKin.Met    = (genmet->front()).et() ; 
   GenKin.MetX   = (genmet->front()).px() ; 
   GenKin.MetY   = (genmet->front()).py() ; 
   GenKin.MetPhi = (genmet->front()).phi() ; 

   // Gen MET: From GenPart Neutrinos ( no SUSY !!! )

   // ... Handle to access GenParticleCollection
   Handle<GenParticleCollection> genParticles;
   iEvent.getByLabel(genPartColl_, genParticles);
   if (!genParticles.isValid()) {
     cerr << "[ChPartTree::GetGenKin] Error: non valid GenParticleCollection " << endl;
     return;
   }
 
   double met1Px = 0 ;
   double met1Py = 0 ;
   double met1Pz = 0 ;
   double met1E  = 0 ;

   double met3Px = 0 ;
   double met3Py = 0 ;
   double met3Pz = 0 ;
   double met3E  = 0 ;

   // ... Loop on stable (final) particles
   for(GenParticleCollection::const_iterator p = genParticles->begin(); p != genParticles->end(); ++ p) 
   {
     int st  = p->status();
     int pid = p->pdgId();
     if ( st == 1 && ( abs(pid)==12 || abs(pid)==14 || abs(pid)==16 ) )
     {
       met1Px += p->px();
       met1Py += p->py();
       met1Pz += p->pz();
       met1E  += p->energy();
     } 
     else if ( st == 3 && ( abs(pid)==12 || abs(pid)==14 || abs(pid)==16 ) )
     {
       met3Px += p->px();
       met3Py += p->py();
       met3Pz += p->pz();
       met3E  += p->energy();
     }
   }

   // ... Set Missing Et 4-Vector
   GenKin.MetGP1.SetPxPyPzE( met1Px , met1Py , met1Pz , met1E ) ;
   GenKin.MetGP3.SetPxPyPzE( met3Px , met3Py , met3Pz , met3E ) ;
*/

   GenKin.MetGP1.SetPxPyPzE( 0. , 0. , 0. , 0. ) ;
   GenKin.MetGP3.SetPxPyPzE( 0. , 0. , 0. , 0. ) ;  

}

