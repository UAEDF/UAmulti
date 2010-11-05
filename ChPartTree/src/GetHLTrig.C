#include <iostream>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

// Trigger Inclides
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "DataFormats/Common/interface/TriggerResults.h"
//#include "FWCore/Framework/interface/TriggerNames.h"
//#include "FWCore/Common/interface/TriggerNames.h"

// UAHiggsTree UAHiggs class decleration
#include "UAmulti/ChPartTree/interface/ChPartTree.h"

void ChPartTree::GetHLTrig(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace std;
  using namespace edm;


  // Find HLT Data Object Name
  edm::InputTag srcTriggerResults_("TriggerResults"); 
  if (srcTriggerResults_.process().empty()) {
    edm::InputTag srcTriggerEvent("hltTriggerSummaryAOD");
    edm::Handle<trigger::TriggerEvent> triggerEvent;
    iEvent.getByLabel(srcTriggerEvent,triggerEvent);
    string hltProcName = triggerEvent.provenance()->processName();
    // cout<<"HLTriggerAnalyzer::analyze() INFO: HLT process="<<hltProcName<<endl;
    srcTriggerResults_ = edm::InputTag(srcTriggerResults_.label()+"::"+hltProcName);
  }
  // cout << srcTriggerResults_ << endl;
  // Fetch HLT Data Object
  edm::Handle<edm::TriggerResults> trgResults;
  iEvent.getByLabel(srcTriggerResults_,trgResults);
  const edm::TriggerNames& trgNames = iEvent.triggerNames(*trgResults);
 
  // Loop on requested triggers by user (config file)
  for(vector<string>::iterator requested_hlt_bit=hlt_bits.begin() ; requested_hlt_bit!=hlt_bits.end();requested_hlt_bit++){
    HLTrig.HLTmap[*requested_hlt_bit]= hasFired(*requested_hlt_bit, trgNames,*trgResults);
    // cout << (*requested_hlt_bit).c_str() << " = " << hasFired(*requested_hlt_bit, trgNames,*trgResults) << endl ;
  }

/*  CODE Working only up to 35x:
  vector<Handle<TriggerResults> > trhv;
  iEvent.getManyByType(trhv);
  const int nt(trhv.size());
  
  TriggerNames triggerNames;
  
  for(int ntt=0;ntt<nt;ntt++){
    triggerNames.init(*trhv[ntt]);
    vector<string> hltNames = triggerNames.triggerNames();
   // cout<<"hlt names size is "<<hltNames.size()<<endl;
    int n = 0;
    for(vector<string>::const_iterator i = hltNames.begin();i!= hltNames.end(); i++){
                  //     cout << "ntt,n,trigger: " << ntt<<","<<n<<","<<*i << " "
		//	    << (*trhv[ntt]).accept(n) << endl;
      for(vector<string>::iterator requested_hlt_bit=hlt_bits.begin();//hlt_bits request by user
	  requested_hlt_bit!=hlt_bits.end();requested_hlt_bit++){
	if(*i == *requested_hlt_bit){
	  HLTrig.HLTmap[*requested_hlt_bit]=(*trhv[ntt]).accept(n);
	}
      }
      n++;
    }//for i 
  }//for ntt
*/


}



bool ChPartTree::hasFired(const std::string& triggerName,
				 const edm::TriggerNames& triggerNames,
				 const edm::TriggerResults& triggerResults) const
{
  unsigned index = triggerNames.triggerIndex(triggerName);
  if (index>=triggerNames.size()) {
/*    if (unknownTriggers_.find(triggerName)==unknownTriggers_.end()) {
      cout<<"HLTriggerAnalyzer::hasFired() ERROR: "
	  <<"unknown trigger name "<<triggerName<<endl;
      unknownTriggers_.insert(triggerName);
    }
*/
    return false;
  }

  return triggerResults.accept(index);
}


