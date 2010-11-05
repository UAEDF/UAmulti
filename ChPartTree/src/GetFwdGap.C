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

#include "DataFormats/CaloTowers/interface/CaloTower.h" 
#include "DataFormats/CaloTowers/interface/CaloTowerFwd.h"
#include "DataFormats/CaloTowers/interface/CaloTowerDetId.h"
#include "DataFormats/HcalDetId/interface/HcalDetId.h"
#include "DataFormats/HcalDetId/interface/HcalSubdetector.h"
#include "DataFormats/EcalDetId/interface/EcalSubdetector.h"


// ChPartTree Analysis class decleration
#include "UAmulti/ChPartTree/interface/ChPartTree.h"

bool FwdGapDebug = false;

void ChPartTree::GetFwdGap(const edm::Event& iEvent , const edm::EventSetup& iSetup )
{
   using namespace std;


  int nTowersHF_plus = 0;
  int nTowersHF_minus = 0;
  int nTowersHE_plus = 0;
  int nTowersHE_minus = 0;
  int nTowersHB_plus = 0;
  int nTowersHB_minus = 0;
  int nTowersEE_plus = 0;
  int nTowersEE_minus = 0;
  int nTowersEB_plus = 0;
  int nTowersEB_minus = 0; 
  //Sum(E)
  double sumEHF_plus = 0.;
  double sumEHF_minus = 0.;
  double sumEHE_plus = 0.;
  double sumEHE_minus = 0.;
  double sumEHB_plus = 0.;
  double sumEHB_minus = 0.;
  double sumEEE_plus = 0.;
  double sumEEE_minus = 0.;
  double sumEEB_plus = 0.;
  double sumEEB_minus = 0.;
  // Sum(ET)
  double sumETHF_plus = 0.;
  double sumETHF_minus = 0.;
  double sumETHE_plus = 0.;
  double sumETHE_minus = 0.;
  double sumETHB_plus = 0.;
  double sumETHB_minus = 0.;
  double sumETEE_plus = 0.;
  double sumETEE_minus = 0.;
  double sumETEB_plus = 0.;
  double sumETEB_minus = 0.;

  FwdGap.Reset();

  // Calo tower collection from event
  edm::Handle<CaloTowerCollection> towerCollectionH;
  iEvent.getByLabel(caloTowerTag_,towerCollectionH);
  const CaloTowerCollection& towerCollection = *towerCollectionH;

  // Loop over calo towers
  CaloTowerCollection::const_iterator calotower = towerCollection.begin();
  CaloTowerCollection::const_iterator calotowers_end = towerCollection.end();
  for(; calotower != calotowers_end; ++calotower) {
     bool hasHCAL = false;
     bool hasHF = false;
     bool hasHE = false;
     bool hasHB = false;
     bool hasHO = false;
     bool hasECAL = false;
     bool hasEE = false;
     bool hasEB = false;     
     for(size_t iconst = 0; iconst < calotower->constituentsSize(); iconst++){
        DetId detId = calotower->constituent(iconst);
        if(detId.det()==DetId::Hcal){
           hasHCAL = true;
           HcalDetId hcalDetId(detId);
           if(hcalDetId.subdet()==HcalForward) hasHF = true;
           else if(hcalDetId.subdet()==HcalEndcap) hasHE = true;
           else if(hcalDetId.subdet()==HcalBarrel) hasHB = true;
           else if(hcalDetId.subdet()==HcalOuter) hasHO = true;  
        } else if(detId.det()==DetId::Ecal){
           hasECAL = true;
           EcalSubdetector ecalSubDet = (EcalSubdetector)detId.subdetId();
           if(ecalSubDet == EcalEndcap) hasEE = true;
           else if(ecalSubDet == EcalBarrel) hasEB = true;
        }
     }

     int zside = calotower->zside();
     double caloTowerEnergy = calotower->energy();
     // FIXME
     //double caloTowerET = calotower->et(primVtx.position());
     //double caloTowerET = calotower->et(primVtx.z());
     double caloTowerET = calotower->et();

     // HCAL: Towers made of at least one component from HB,HE,HF
     if( hasHF && !hasHE ){
        if( caloTowerEnergy >= energyThresholdHF_ ){
           if(zside >= 0){
              ++nTowersHF_plus;
              sumEHF_plus += caloTowerEnergy; 
              sumETHF_plus += caloTowerET;
           } else{
              ++nTowersHF_minus;
              sumEHF_minus += caloTowerEnergy;
              sumETHF_minus += caloTowerET;
           } 
        }
     } else if( hasHE && !hasHF && !hasHB ){
        if( caloTowerEnergy >= energyThresholdHE_ ){
           if(zside >= 0){
              ++nTowersHE_plus;
              sumEHE_plus += caloTowerEnergy;
              sumETHE_plus += caloTowerET;
           } else{
              ++nTowersHE_minus;
              sumEHE_minus += caloTowerEnergy;
              sumETHE_minus += caloTowerET;
           }
        }
     } else if( hasHB && !hasHE ){
        if( caloTowerEnergy >= energyThresholdHB_ ){
           if(zside >= 0){
              ++nTowersHB_plus;
              sumEHB_plus += caloTowerEnergy;
              sumETHB_plus += caloTowerET;
           } else{
              ++nTowersHB_minus;
              sumEHB_minus += caloTowerEnergy;
              sumETHB_minus += caloTowerET;
           }
        }
     }

     // ECAL: Towers made of at least one component from EB,EE
     if( hasEE && !hasEB ){
        if( caloTowerEnergy >= energyThresholdEE_ ){
           if(zside >= 0){
              ++nTowersEE_plus;
              sumEEE_plus += caloTowerEnergy;
              sumETEE_plus += caloTowerET;
           } else{
              ++nTowersEE_minus;
              sumEEE_minus += caloTowerEnergy;
              sumETEE_minus += caloTowerET;
           }
        }
     } else if( hasEB && !hasEE ){
        if( caloTowerEnergy >= energyThresholdEB_ ){
           if(zside >= 0){
              ++nTowersEB_plus;
              sumEEB_plus += caloTowerEnergy;
              sumETEB_plus += caloTowerET;
           } else{
              ++nTowersEB_minus;
              sumEEB_minus += caloTowerEnergy;
              sumETEB_minus += caloTowerET;
           }
        }
     }
  }

   FwdGap.nTowersHF_plus = nTowersHF_plus ;
   FwdGap.nTowersHF_minus = nTowersHF_minus ;
   FwdGap.nTowersHE_plus = nTowersHE_plus ;
   FwdGap.nTowersHE_minus = nTowersHE_minus ;
   FwdGap.nTowersHB_plus = nTowersHB_plus ;
   FwdGap.nTowersHB_minus = nTowersHB_minus ;
   FwdGap.nTowersEE_plus = nTowersEE_plus ;
   FwdGap.nTowersEE_minus = nTowersEE_minus ;
   FwdGap.nTowersEB_plus = nTowersEB_plus ;
   FwdGap.nTowersEB_minus = nTowersEB_minus ;
   FwdGap.sumEHF_plus = sumEHF_plus ;
   FwdGap.sumEHF_minus = sumEHF_minus ;
   FwdGap.sumEHE_plus = sumEHE_plus ;
   FwdGap.sumEHE_minus = sumEHE_minus ;
   FwdGap.sumEHB_plus = sumEHB_plus ;
   FwdGap.sumEHB_minus = sumEHB_minus ;
   FwdGap.sumEEE_plus = sumEEE_plus ;
   FwdGap.sumEEE_minus = sumEEE_minus ;
   FwdGap.sumEEB_plus = sumEEB_plus ;
   FwdGap.sumEEB_minus = sumEEB_minus ;
   FwdGap.sumETHF_plus = sumETHF_plus ;
   FwdGap.sumETHF_minus = sumETHF_minus ;
   FwdGap.sumETHE_plus = sumETHE_plus ;
   FwdGap.sumETHE_minus = sumETHE_minus ;
   FwdGap.sumETHB_plus = sumETHB_plus ;
   FwdGap.sumETHB_minus = sumETHB_minus ;
   FwdGap.sumETEE_plus = sumETEE_plus ;
   FwdGap.sumETEE_minus = sumETEE_minus ;
   FwdGap.sumETEB_plus = sumETEB_plus ;
   FwdGap.sumETEB_minus = sumETEB_minus ;

}

