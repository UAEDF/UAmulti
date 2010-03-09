#include <stdio.h>
#include <iostream>
using namespace std;
#include "TString.h" 

void collectionPlotter(int, double , double , TString , int );


int main( int argc, char *argv[] )
{

  // arguments:
  int     type     = 10     ; 
  double  E        = 0.9    ; 
  double  ptcut    = 0.2    ; 
  TString filename = "NONE" ; 
  int     nevt_max = 5000   ; 

  for ( int iarg = 1 ; iarg < argc ; ++iarg)
  {
    if (iarg == 1) type     = atoi(argv[iarg]);
    if (iarg == 2) E        = atof(argv[iarg]); 
    if (iarg == 3) ptcut    = atof(argv[iarg]); 
    if (iarg == 4) filename = argv[iarg];
    if (iarg == 5) nevt_max = atoi(argv[iarg]);
  }

  cout << "type     set to: " << type  << endl;
  cout << "E        set to: " << E     << endl;
  cout << "ptcut    set to: " << ptcut << endl;
  cout << "filename set to: " << filename << endl;
  cout << "nevt_max set to: " << nevt_max << endl;

  collectionPlotter(type,E,ptcut,filename,nevt_max);
}
