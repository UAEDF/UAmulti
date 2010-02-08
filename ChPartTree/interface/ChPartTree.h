#ifndef __ChPartTree_H__
#define __ChPartTree_H__
/* 
 class          : ChPartTree
 Description    : MinBias Charged Part Analysis
 Original Author: Xavier Janssen <xjanssen@.mail.cern.ch> 
 Created        : Fri Jan 29 11:48:28 CET 2010
*/


// system include files
#include <string>
#include <vector>
using namespace std;

// ROOT
#include "TFile.h"
#include "TTree.h"

// CMSSW Include files (Minimal)
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

// ChPartTree Analysis class decleration

#include "UAmulti/ChPartTree/interface/MyPart.h"

#include "UAmulti/ChPartTree/interface/MyEvtId.h"
#include "UAmulti/ChPartTree/interface/MyL1Trig.h"

#include "UAmulti/ChPartTree/interface/MyGenKin.h"
#include "UAmulti/ChPartTree/interface/MyGenPart.h"

#include "UAmulti/ChPartTree/interface/MyVertex.h"
#include "UAmulti/ChPartTree/interface/MyTracks.h"

// MIT code
#include "UAmulti/ChPartTree/interface/MyMITEvtSel.h"
 

//#include "MitEdm/DataFormats/interface/EvtSelData.h"

//
// Some constants
//

//Double_t mpion = 139.57018;



//
// class decleration
//



class ChPartTree : public edm::EDAnalyzer {
   public:
      explicit ChPartTree(const edm::ParameterSet&);
      ~ChPartTree();


   private:
      virtual void beginJob() ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      // ----------Functions------------------------------

      virtual void GetEvtId(const edm::Event&);
      virtual void GetL1Trig(const edm::Event&, const edm::EventSetup& );

      virtual void GetGenKin(const edm::Event&);
      virtual void GetGenPart(const edm::Event&, const edm::EventSetup&);      

      virtual void GetRecoVertex(const edm::Event& , const edm::EventSetup& ,
                                 const char[60]    , vector<MyVertex>& ); 

      virtual void GetRecoTracks(const edm::Event& , const edm::EventSetup& , 
                                 const char[60]    , vector<MyTracks>& );

      virtual void GetMITEvtSel(const edm::Event&, const edm::EventSetup& );

      // ----------Config data --------------------------- 

      // MC Flag
    
      bool mcflag;

      // Modules to execute

      bool StoreGenPart;
      bool StoreGenKine;

      // Data Collection
      edm::InputTag genPartColl_ ;
      edm::InputTag hepMCColl_ ;

      // ----------Tree & File ---------------------------

      string fOutputFileName ;
      TFile*   fout;
      TTree*   tree;

      // --------- Tree Content -------------------------- 

      MyEvtId           EvtId;
      MyL1Trig          L1Trig; 

      MyGenKin          GenKin;
      vector<MyGenPart> GenPart;


      vector<MyVertex>  pixelVertex;     
      vector<MyVertex>  primaryVertex;     

      vector<MyTracks>  generalTracks;
      vector<MyTracks>  ferencTracks;


      MyMITEvtSel       MITEvtSel;

      // --------- Vtx ID --------------------------

      Int_t vtxid;
      vector<Double_t> vtxid_x;
      vector<Double_t> vtxid_y;
      vector<Double_t> vtxid_z;


};

#endif

