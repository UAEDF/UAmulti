
struct Acc {
    double ptGen;
    double ptReco;
    double charge;
    double etaGenMin;
    double etaGenMax;
    double etaRecoMin;
    double etaRecoMax;
} tmpAcc;

vector<Acc> * accMap = new vector <Acc>;


//call it in your main
void InitializeAcceptanceMap(){

    //-------PT GEN = 0 ---------
    // ptReco =.1
    // ***** 0 *****
    tmpAcc.ptGen      = 0.1;  
    tmpAcc.ptReco     = 0.1;
    tmpAcc.charge     = 0.;
    tmpAcc.etaGenMin  = 0.;
    tmpAcc.etaGenMax  = 2.4;
    tmpAcc.etaRecoMin = 0.;
    tmpAcc.etaRecoMax = 2.4;
    accMap->push_back( tmpAcc);


   // ptReco =.1 corr to 0
    // ***** 1 *****
    tmpAcc.ptGen      = 0.;    //PT correction to 0 !!!
    tmpAcc.ptReco     = 0.1;
    tmpAcc.charge     = 0.;
    tmpAcc.etaGenMin  = 0.;
    tmpAcc.etaGenMax  = 2.4;
    tmpAcc.etaRecoMin = 0.;
    tmpAcc.etaRecoMax = 2.4;
    accMap->push_back( tmpAcc);
    
    // ptReco =.5
    // ***** 2 *****
    tmpAcc.ptGen      = 0.5;
    tmpAcc.ptReco     = 0.5;
    tmpAcc.charge     = 0.;
    tmpAcc.etaGenMin  = 0.;
    tmpAcc.etaGenMax  = 2.4;
    tmpAcc.etaRecoMin = 0.;
    tmpAcc.etaRecoMax = 2.4;
    accMap->push_back( tmpAcc);

   // ptReco =1.0
    // ***** 3 *****
    tmpAcc.ptGen      = 1.;
    tmpAcc.ptReco     = 1.;
    tmpAcc.charge     = 0.;
    tmpAcc.etaGenMin  = 0.;
    tmpAcc.etaGenMax  = 2.4;
    tmpAcc.etaRecoMin = 0.;
    tmpAcc.etaRecoMax = 2.4;
    accMap->push_back( tmpAcc);
    
    // ptReco =0.1
    // ***** 4 *****
    tmpAcc.ptGen      = 0.1;
    tmpAcc.ptReco     = 0.1;
    tmpAcc.charge     = 0.;
    tmpAcc.etaGenMin  = 0.;
    tmpAcc.etaGenMax  = 1.0;
    tmpAcc.etaRecoMin = 0.;
    tmpAcc.etaRecoMax = 1.0;
    accMap->push_back( tmpAcc);  
    
    // ptReco =0.1
    // ***** 5 *****
    tmpAcc.ptGen      = 0.;
    tmpAcc.ptReco     = 0.1;
    tmpAcc.charge     = 0.;
    tmpAcc.etaGenMin  = 0.;
    tmpAcc.etaGenMax  = 1.0;
    tmpAcc.etaRecoMin = 0.;
    tmpAcc.etaRecoMax = 1.0;
    accMap->push_back( tmpAcc);          
      
/*
    //-------Eta .5 steps ---------
    // ptReco =.1
    // ***** 3 *****
    tmpAcc.ptGen      = 0.1;
    tmpAcc.ptReco     = 0.1;
    tmpAcc.charge     = 0.;
    tmpAcc.etaGenMin  = 0.;
    tmpAcc.etaGenMax  = 0.5;
    tmpAcc.etaRecoMin = 0.;
    tmpAcc.etaRecoMax = 0.5;
    accMap->push_back( tmpAcc);

    // ***** 4 *****
    tmpAcc.ptGen      = 0.1;
    tmpAcc.ptReco     = 0.1;
    tmpAcc.charge     = 0.;
    tmpAcc.etaGenMin  = 0.5;
    tmpAcc.etaGenMax  = 1.0;
    tmpAcc.etaRecoMin = 0.5;
    tmpAcc.etaRecoMax = 1.0;
    accMap->push_back( tmpAcc);

    // ***** 5 *****
    tmpAcc.ptGen      = 0.1;
    tmpAcc.ptReco     = 0.1;
    tmpAcc.charge     = 0.;
    tmpAcc.etaGenMin  = 1.;
    tmpAcc.etaGenMax  = 1.5;
    tmpAcc.etaRecoMin = 1.;
    tmpAcc.etaRecoMax = 1.5;
    accMap->push_back( tmpAcc);

    // ***** 6 *****
    tmpAcc.ptGen      = 0.1;
    tmpAcc.ptReco     = 0.1;
    tmpAcc.charge     = 0.;
    tmpAcc.etaGenMin  = 1.5;
    tmpAcc.etaGenMax  = 2.0;
    tmpAcc.etaRecoMin = 1.5;
    tmpAcc.etaRecoMax = 2.0;
    accMap->push_back( tmpAcc);

    // ***** 7 *****
    tmpAcc.ptGen      = 0.1;
    tmpAcc.ptReco     = 0.1;
    tmpAcc.charge     = 0.;
    tmpAcc.etaGenMin  = 2.0;
    tmpAcc.etaGenMax  = 2.4;
    tmpAcc.etaRecoMin = 2.0;
    tmpAcc.etaRecoMax = 2.4;
    accMap->push_back( tmpAcc);

    // ptReco =.5
    // ***** 8 *****
    tmpAcc.ptGen      = 0.5;
    tmpAcc.ptReco     = 0.5;
    tmpAcc.charge     = 0.;
    tmpAcc.etaGenMin  = 0.;
    tmpAcc.etaGenMax  = 0.5;
    tmpAcc.etaRecoMin = 0.;
    tmpAcc.etaRecoMax = 0.5;
    accMap->push_back( tmpAcc);

    // ***** 9 *****
    tmpAcc.ptGen      = 0.5;
    tmpAcc.ptReco     = 0.5;
    tmpAcc.charge     = 0.;
    tmpAcc.etaGenMin  = 0.5;
    tmpAcc.etaGenMax  = 1.0;
    tmpAcc.etaRecoMin = 0.5;
    tmpAcc.etaRecoMax = 1.;
    accMap->push_back( tmpAcc);

    // ***** 10 *****
    tmpAcc.ptGen      = 0.5;
    tmpAcc.ptReco     = 0.5;
    tmpAcc.charge     = 0.;
    tmpAcc.etaGenMin  = 1.;
    tmpAcc.etaGenMax  = 1.5;
    tmpAcc.etaRecoMin = 1.;
    tmpAcc.etaRecoMax = 1.5;
    accMap->push_back( tmpAcc);

    // ***** 11 *****
    tmpAcc.ptGen      = 0.5;
    tmpAcc.ptReco     = 0.5;
    tmpAcc.charge     = 0.;
    tmpAcc.etaGenMin  = 1.5;
    tmpAcc.etaGenMax  = 2.0;
    tmpAcc.etaRecoMin = 1.5;
    tmpAcc.etaRecoMax = 2.0;
    accMap->push_back( tmpAcc);

    // *****12 *****
    tmpAcc.ptGen      = 0.5;
    tmpAcc.ptReco     = 0.5;
    tmpAcc.charge     = 0.;
    tmpAcc.etaGenMin  = 2.;
    tmpAcc.etaGenMax  = 2.4;
    tmpAcc.etaRecoMin = 2.;
    tmpAcc.etaRecoMax = 2.4;
    accMap->push_back( tmpAcc);

    // ptReco =1.0
    // ***** 13 *****
    tmpAcc.ptGen      = 1.0;
    tmpAcc.ptReco     = 1.0;
    tmpAcc.charge     = 0.;
    tmpAcc.etaGenMin  = 0.;
    tmpAcc.etaGenMax  = 0.5;
    tmpAcc.etaRecoMin = 0.;
    tmpAcc.etaRecoMax = 0.5;
    accMap->push_back( tmpAcc);

    // ***** 14 *****
    tmpAcc.ptGen      = 1.0;
    tmpAcc.ptReco     = 1.0;
    tmpAcc.charge     = 0.;
    tmpAcc.etaGenMin  = 0.5;
    tmpAcc.etaGenMax  = 1.0;
    tmpAcc.etaRecoMin = 0.5;
    tmpAcc.etaRecoMax = 1.0;
    accMap->push_back( tmpAcc);

    // ***** 15 *****
    tmpAcc.ptGen      = 1.0;
    tmpAcc.ptReco     = 1.0;
    tmpAcc.charge     = 0.;
    tmpAcc.etaGenMin  = 1.0;
    tmpAcc.etaGenMax  = 1.5;
    tmpAcc.etaRecoMin = 1.0;
    tmpAcc.etaRecoMax = 1.5;
    accMap->push_back( tmpAcc);

    // ***** 16 *****
    tmpAcc.ptGen      = 1.0;
    tmpAcc.ptReco     = 1.0;
    tmpAcc.charge     = 0.;
    tmpAcc.etaGenMin  = 1.5;
    tmpAcc.etaGenMax  = 2.0;
    tmpAcc.etaRecoMin = 1.5;
    tmpAcc.etaRecoMax = 2.0;
    accMap->push_back( tmpAcc);

    // ***** 17 *****
    tmpAcc.ptGen      = 1.0;
    tmpAcc.ptReco     = 1.0;
    tmpAcc.charge     = 0.;
    tmpAcc.etaGenMin  = 2.;
    tmpAcc.etaGenMax  = 2.4;
    tmpAcc.etaRecoMin = 2.;
    tmpAcc.etaRecoMax = 2.4;
    accMap->push_back( tmpAcc);
*/

    if(debug) {
        if(accMap->size()>11){
            cout << "element at(10) as example: " <<endl;
            cout << "ptGen " << accMap->at(10).ptGen << endl;
            cout << "ptReco "<< accMap->at(10).ptReco << endl;
            cout << "etaRecoMax"<< accMap->at(10).etaRecoMax << endl;
            cout << "leaving acceptance function " <<endl;            
        }
        cout << "--------------------------" <<endl;
    }
}
