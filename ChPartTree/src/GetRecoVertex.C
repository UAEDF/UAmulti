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
//#include "DataFormats/VertexReco/src/Vertex.cc"
#include "DataFormats/VertexReco/interface/VertexFwd.h"



// ChPartTree Analysis class decleration
#include "UAmulti/ChPartTree/interface/ChPartTree.h"

bool RecoVtxDebug = false;

void ChPartTree::GetRecoVertex(const edm::Event& iEvent, const edm::EventSetup& iSetup,
                               const char VertexCollName[60] , vector<MyVertex>& VertexVector )
{
   using namespace std;
   using namespace edm;
   using namespace reco;

   VertexVector.clear();
   MyVertex myvertex;

   Handle<reco::VertexCollection> vtxcoll ;
   iEvent.getByLabel(VertexCollName,vtxcoll);

   if (RecoVtxDebug) cout  << VertexCollName << " : Id  position "<< endl ;

   for(VertexCollection::const_iterator p=vtxcoll->begin(); p!= vtxcoll->end() ; ++p)
   {

      myvertex.id        = vtxid++;
      myvertex.x         = p->x()  ;
      myvertex.y         = p->y()  ;
      myvertex.z         = p->z()  ;

      if (RecoVtxDebug)
        cout << myvertex.id << " " << p->position() << endl;

      myvertex.ex        = p->xError()  ;
      myvertex.ey        = p->yError()  ;
      myvertex.ez        = p->zError()  ;
      myvertex.validity  = p->isValid() ;
      myvertex.fake      = p->isFake()  ;
      if ( ! myvertex.fake )
        myvertex.chi2n     = p->normalizedChi2() ;
      else
        myvertex.chi2n     = -99. ;
      myvertex.ntracks   = p->tracksSize() ;

      VertexVector.push_back(myvertex);
      vtxid_xyz.push_back(p->position());

   }   

}

