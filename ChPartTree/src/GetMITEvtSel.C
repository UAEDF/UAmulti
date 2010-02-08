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

// MIT code
#include "MitEdm/DataFormats/interface/EvtSelData.h"



// ChPartTree Analysis class decleration
#include "UAmulti/ChPartTree/interface/ChPartTree.h"

bool MITEvtSelDebug = false;

void ChPartTree::GetMITEvtSel(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace std;
   using namespace edm;
   using namespace mitedm;

   Handle<EvtSelData> EvtSel;
   iEvent.getByLabel("evtSelData",EvtSel);

    MITEvtSel.eHcalNeg = EvtSel->eHcalNeg();
    MITEvtSel.eHcalPos = EvtSel->eHcalPos();
    MITEvtSel.eHfNeg = EvtSel->eHfNeg();
    MITEvtSel.eHfPos = EvtSel->eHfPos();
    MITEvtSel.eHfNegTime = EvtSel->eHfNegTime();
    MITEvtSel.eHfPosTime = EvtSel->eHfPosTime();
    MITEvtSel.eCaNeg = EvtSel->eCastorNeg();
    MITEvtSel.eCaPos = EvtSel->eCastorPos();
    MITEvtSel.eCaNegTime = EvtSel->eCastorNegTime();
    MITEvtSel.eCaPosTime = EvtSel->eCastorPosTime();
    MITEvtSel.eZdcNeg = EvtSel->eZdcNeg();
    MITEvtSel.eZdcPos = EvtSel->eZdcPos();
    MITEvtSel.eZdcNegTime = EvtSel->eZdcNegTime();
    MITEvtSel.eZdcPosTime = EvtSel->eZdcPosTime();
    MITEvtSel.ePxbHits = EvtSel->ePxbHits();
    MITEvtSel.ePxHits = EvtSel->ePxHits();
    MITEvtSel.eClusVtxQual = EvtSel->eClusVtxQual();
    MITEvtSel.eClusVtxDiff = EvtSel->eClusVtxDiff();
    MITEvtSel.nHfNegHits = EvtSel->nHfNegHits();
    MITEvtSel.nHfPosHits = EvtSel->nHfPosHits();
    MITEvtSel.nHfTowersP = EvtSel->nHfTowersP();
    MITEvtSel.nHfTowersN = EvtSel->nHfTowersN();
    MITEvtSel.sumEsubEpPos = EvtSel->sumEsubEpPos();
    MITEvtSel.sumEaddEpPos = EvtSel->sumEaddEpPos();
    MITEvtSel.sumEsubEpNeg = EvtSel->sumEsubEpNeg();
    MITEvtSel.sumEaddEpNeg = EvtSel->sumEaddEpNeg();
    MITEvtSel.sumHfEsubEpPos = EvtSel->sumHfEsubEpPos();
    MITEvtSel.sumHfEaddEpPos = EvtSel->sumHfEaddEpPos();
    MITEvtSel.sumHfEsubEpNeg = EvtSel->sumHfEsubEpNeg();
    MITEvtSel.sumHfEaddEpNeg = EvtSel->sumHfEaddEpNeg();
    MITEvtSel.eHPTrkFrac = EvtSel->eHPTrkFrac();


}

