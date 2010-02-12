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

// Genaral Tracks and Vertex
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/src/Vertex.cc"
#include "DataFormats/VertexReco/interface/VertexFwd.h"



// ChPartTree Analysis class decleration
#include "UAmulti/ChPartTree/interface/ChPartTree.h"

//bool L1TrigDebug = false;

void ChPartTree::GetRecoTracks(const edm::Event& iEvent, const edm::EventSetup& iSetup,
                               const char TrackCollName[60] , vector<MyTracks>& TrackVector )
{
   using namespace std;
   using namespace edm;

   Double_t mpion = 139.57018;


   TrackVector.clear(); 

   MyTracks mytrack;

   Handle<TrackCollection> tracks;
   //iEvent.getByLabel(trackTags_,tracks);

//   iEvent.getByLabel("generalTracks",tracks);
   iEvent.getByLabel(TrackCollName,tracks);

   for(TrackCollection::const_iterator tr = tracks->begin(); tr!=tracks->end(); ++tr)
   {
//   cout << tr->charge() << " " << tr->px() << " " << tr->py() << " " << tr->pz()   << endl;

     //mytrack.px = tr->px() ;
     //mytrack.py = tr->py() ;
     //mytrack.pz = tr->pz() ;
     //mytrack.eta= tr->eta() ;

     mytrack.Part.charge = tr->charge();
     mytrack.Part.v.SetPtEtaPhiM(tr->pt(),tr->eta(),tr->phi(), mpion );  

     mytrack.nhit  =  tr->recHitsSize();
     mytrack.chi2n =  tr->normalizedChi2();
     mytrack.dz    =  tr->dz();
     mytrack.d0    =  tr->d0();
     mytrack.edz   =  tr->dzError();
     mytrack.ed0   =  tr->d0Error();
     mytrack.ept   =  tr->ptError();

     mytrack.quality[0] = tr->quality(reco::TrackBase::qualityByName("loose"));
     mytrack.quality[1] = tr->quality(reco::TrackBase::qualityByName("tight"));
     mytrack.quality[2] = tr->quality(reco::TrackBase::qualityByName("highPurity"));


     // BeamSpot (id=0) and Vertex (id>0) Links
     mytrack.vtxid.clear();
     mytrack.vtxdxy.clear();
     mytrack.vtxdz.clear();

     for ( int i = 0 ; i != vtxid ; i++ )
     {
        //cout << i << " " << vtxid_xyz[i] << endl;
        //cout << "dxy: " << tr->dxy( vtxid_xyz[i] ) << endl;  
        //cout << "dz : " << tr->dz( vtxid_xyz[i] ) << endl; 
        mytrack.vtxid.push_back( i ); 
        mytrack.vtxdxy.push_back( tr->dxy( vtxid_xyz[i] ) );
        mytrack.vtxdz.push_back(  tr->dz( vtxid_xyz[i] )  );
     }


 /* 
     cout << mytrack.Part.v.Px() << " "
          << mytrack.Part.v.Py() << " "
          << mytrack.Part.v.Pt() << endl;
*/

     TrackVector.push_back(mytrack);
   }


}

