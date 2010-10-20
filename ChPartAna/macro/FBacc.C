
#include "FBacc_struct.h"
FBacc cut ;

//THE 2 VARIABLES
vector<FBacc> accMap;
vector< pair<double,double> > accGroups;



void getAccMap(){
  
  //-----------------------------------------
  // --           0  ==>  21               --
  //-----------------------------------------
  for(Double_t i=0;i<=2.4-cut.widthP;i+=0.1){
  //cout << "1   " << i << endl;
    cut.etaM = -i;
    cut.etaP = i;
    accMap.push_back(cut);
  }
  //cout << "ssssssss   " << accMap.size()-1<<endl;

  //-----------------------------------------
  // --           22  ==>  40              --
  //-----------------------------------------
  cut.widthM = cut.widthP = 0.5;
  for(Double_t i=0;i<=2.4-cut.widthP;i+=0.1){
 // cout <<  "2   " <<i << endl;
    cut.etaM = -i;
    cut.etaP = i;
    accMap.push_back(cut);
  }
  //cout << "ssssssss   " << accMap.size()-1<<endl;

  //-----------------------------------------
  // --           41  ==>  57              --
  //-----------------------------------------
  cut.widthM = cut.widthP = 0.75;
  for(Double_t i=0;i<=2.4-cut.widthP;i+=0.1){
  //cout << "3   " << i << endl;
    cut.etaM = -i;
    cut.etaP = i;
    accMap.push_back(cut);
  }
  //cout << "ssssssss   " << accMap.size()-1<<endl;

  //-----------------------------------------
  // --           58  ==>  71              --
  //-----------------------------------------
  cut.widthM = cut.widthP = 1.;
  for(Double_t i=0;i<=2.4-cut.widthP;i+=0.1){
  //cout << "4   " << i << endl;
    cut.etaM = -i;
    cut.etaP = i;
    accMap.push_back(cut);
  }
  //cout << "ssssssss   " << accMap.size()-1<<endl;
 
}


void getAccGroups(){
  accGroups.push_back(make_pair(0,21));
  accGroups.push_back(make_pair(22,40));
  accGroups.push_back(make_pair(41,57));
  accGroups.push_back(make_pair(58,71));
}

int getAccGroupNum(int acc){
  for(int accG = 0 ; accG < (signed) accGroups.size() ; ++accG){
    if(acc>=accGroups.at(accG).first && acc<=accGroups.at(accG).second)
      return accG;
  }
  cout << "ERROR !! The acc " << acc << " isn't in any accGroups. Exiting NOW ..." << endl;
  exit(0);
}


void printAcc(){
  cout << "Starting Print Acceptance Map ==> " << endl;
  for(int accG = 0 ; accG < (signed) accGroups.size() ; ++accG){
    cout << "Group from " << accGroups.at(accG).first << " to " << accGroups.at(accG).second << " : " << endl;
    for(int acc = accGroups.at(accG).first ; acc <= accGroups.at(accG).second ; ++acc){
      cout << "   acc " << acc  << "  :  ";
      cout << accMap.at(acc).etaM - accMap.at(acc).widthM << " < eta < " << accMap.at(acc).etaM;
      cout << "   &   " ;
      cout << accMap.at(acc).etaP << " < eta < " << accMap.at(acc).etaP + accMap.at(acc).widthP << endl;
    }
    cout << endl;
  
  }
}
