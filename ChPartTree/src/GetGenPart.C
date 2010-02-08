// Package:     ChPartTree
// Class:       ChPartTree
// author:      Xavier Janssen
// Date:        02/12/2009 
//
// Description: Function to retrieve Generated particles from HEPEVT

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

// XJ 

// DataFormats
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Candidate/interface/CandidateFwd.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "SimGeneral/HepPDTRecord/interface/ParticleDataTable.h"

// ChPartTree Analysis class declaration
#include "UAmulti/ChPartTree/interface/ChPartTree.h"

bool GenPartDebug = false ;

void ChPartTree::GetGenPart(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace std;
   using namespace edm;
   using namespace reco;

   // Clear GenPart List
   // (evt->GenPart).clear();
   GenPart.clear();

   // Handle to access PDG data from iSetup
   ESHandle <ParticleDataTable> pdt;
   iSetup.getData( pdt );

   // Handle to access GenParticleCollection
   Handle<GenParticleCollection> genParticles;
   iEvent.getByLabel(genPartColl_, genParticles);
   if (!genParticles.isValid()) {
     cerr << "[ChPartTree::GetGenPart] Error: non valid GenParticleCollection " << endl;
     return;
   }

   // List for Daugther/Mother Search
   vector<const reco::Candidate *> cands;
   vector<const Candidate *>::const_iterator found = cands.begin();
   for(GenParticleCollection::const_iterator p = genParticles->begin();
     p != genParticles->end(); ++ p) {
     cands.push_back(&*p);
   }

   // Loop on generated particle and store if status=3
   if ( GenPartDebug )
     cout << "GenPart # : " << genParticles->size() << endl;
   
   for(GenParticleCollection::const_iterator p = genParticles->begin();
     p != genParticles->end(); ++ p) {
     int st = p->status();

       MyGenPart genpart;

      if ( GenPartDebug )
      {
       if(( fabs(p->pdgId())==11 || fabs(p->pdgId())== 13) && st==1) 
         cout    << p-genParticles->begin()
                 << " " << st  
                 << " " << p->pdgId()
                 << " " << (pdt->particle(p->pdgId()))->name()
                 << " " << p->pt()
                 << " " << p->eta()
                 << " " << p->phi()
                 << " " << p->charge()
                 << endl ;

       if(( fabs(p->pdgId())==11 || fabs(p->pdgId())== 13) && st==3)
         cout    << p-genParticles->begin()
                 << " " << st
                 << " " << p->pdgId()
                 << " " << (pdt->particle(p->pdgId()))->name()
                 << " " << p->pt()
                 << " " << p->eta()
                 << " " << p->phi()
                 << " " << p->charge()
                 << endl ;
       }

       // Four vector
/*
       genpart.pt   = p->pt();
       genpart.eta  = p->eta();
       genpart.phi  = p->phi();
       genpart.e    = p->energy();
       genpart.px   = p->px();
       genpart.py   = p->py();
       genpart.pz   = p->pz();
       genpart.m    = p->mass();
*/

       genpart.Part.v.SetPxPyPzE( p->px() , p->py() , p->pz() , p->energy() );

       // Extra properties

       genpart.Part.charge  = p->charge();
       genpart.pdgId   = p->pdgId();
       genpart.name    = (pdt->particle(p->pdgId()))->name();
       genpart.status  = p->status();

       // Mother Daughter relations

       int nMo = p->numberOfMothers();
       int nDa = p->numberOfDaughters();
       int iMo1 = -1 , iMo2 = -1 ; 
       int iDa1 = -1 , iDa2 = -1 ; 

       found = find(cands.begin(), cands.end(), p->mother(0));
       if(found != cands.end()) iMo1 = found - cands.begin() ;
 
       found = find(cands.begin(), cands.end(), p->mother(nMo-1));
       if(found != cands.end()) iMo2 = found - cands.begin() ;

       found = find(cands.begin(), cands.end(), p->daughter(0));
       if(found != cands.end()) iDa1 = found - cands.begin() ;
 
       found = find(cands.begin(), cands.end(), p->daughter(nDa-1));
       if(found != cands.end()) iDa2 = found - cands.begin() ;

       if ( GenPartDebug )
       {
         cout << "Mother : " << iMo1 << " " << iMo2 << endl;
         cout << "Daugther: " << iDa1 << " " << iDa2 << endl;
       }

       genpart.mo1     = iMo1 ;
       genpart.mo2     = iMo2 ;
       genpart.da1     = iDa1 ;
       genpart.da2     = iDa2 ;

       // Store




       if(st==1 && fabs(genpart.Part.charge)>0)
       GenPart.push_back(genpart);

   }

}
