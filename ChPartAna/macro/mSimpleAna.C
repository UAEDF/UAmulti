#include <stdio.h>
#include <iostream>
using namespace std;
#include "TString.h"

void SimpleAna(int, double, TString ,int ,int ,int );


int main( int argc, char *argv[] )
{

  // arguments:
  int     type     = 10     ;
  double  E        = 0.9    ;
  TString filename = "NONE" ;
  int     nevt_max = 5000   ;
  int     iTracking= 0      ; 
  int     irun     = 0      ;


  for ( int iarg = 1 ; iarg < argc ; ++iarg)
  {
    if (iarg == 1) type     = atoi(argv[iarg]);
    if (iarg == 2) E        = atof(argv[iarg]);
    if (iarg == 3) filename = argv[iarg];
    if (iarg == 4) nevt_max = atoi(argv[iarg]);
    if (iarg == 5) iTracking= atoi(argv[iarg]);
    if (iarg == 6) irun     = atoi(argv[iarg]);
  }

  cout << "type      set to: " << type      << endl;
  cout << "E         set to: " << E         << endl;
  cout << "filename  set to: " << filename  << endl;
  cout << "nevt_max  set to: " << nevt_max  << endl;
  cout << "iTracking set to: " << iTracking << endl;
  cout << "irun      set to: " << irun      << endl; 

  SimpleAna (type, E, filename, nevt_max, iTracking, irun );
} 
