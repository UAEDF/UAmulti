#ifndef _FBACC_STRUCT_
#define _FBACC_STRUCT_ 

struct FBacc{
  Double_t widthM;
  Double_t widthP;
  Double_t etaM;
  Double_t etaP;
  Double_t ptM;
  Double_t ptP;

  //Constructor
  FBacc(){
    widthM = 0.25;
    widthP = 0.25;
    etaM = 0;
    etaP = 0;
    ptM = 0.1;
    ptP = 0.1;
  };
};

#endif
