vector< vector<double> > accMap;

//ptGen , etaGen , ptReco , etaReco
vector<double> tmp(5,0);

//-------PT GEN = 0 ---------
//***** 0 *****
tmp.at(0) = 0;
tmp.at(1) = 2.4;
tmp.at(2) = 0.4;
tmp.at(3) = 2.4;
accMap.push_back(tmp);

//***** 1 *****
tmp.at(0) = 0;
tmp.at(1) = 2.;
tmp.at(2) = 0.4;
tmp.at(3) = 2.;
accMap.push_back(tmp);

//***** 2 *****
tmp.at(0) = 0;
tmp.at(1) = 1.5;
tmp.at(2) = 0.4;
tmp.at(3) = 1.5;
accMap.push_back(tmp);

//***** 3 *****
tmp.at(0) = 0;
tmp.at(1) = 1.;
tmp.at(2) = 0.4;
tmp.at(3) = 1.;
accMap.push_back(tmp);

//***** 4 *****
tmp.at(0) = 0;
tmp.at(1) = 0.5;
tmp.at(2) = 0.4;
tmp.at(3) = 0.5;
accMap.push_back(tmp);



//+++++++
//***** 5 *****
tmp.at(0) = 0;
tmp.at(1) = 2.4;
tmp.at(2) = 0.10;
tmp.at(3) = 2.4;
accMap.push_back(tmp);

//***** 6 *****
tmp.at(0) = 0;
tmp.at(1) = 2.;
tmp.at(2) = 0.10;
tmp.at(3) = 2.;
accMap.push_back(tmp);

//***** 7 *****
tmp.at(0) = 0;
tmp.at(1) = 1.5;
tmp.at(2) = 0.10;
tmp.at(3) = 1.5;
accMap.push_back(tmp);

//***** 8 *****
tmp.at(0) = 0;
tmp.at(1) = 1.;
tmp.at(2) = 0.10;
tmp.at(3) = 1.;
accMap.push_back(tmp);

//***** 9 *****
tmp.at(0) = 0;
tmp.at(1) = 0.5;
tmp.at(2) = 0.10;
tmp.at(3) = 0.5;
accMap.push_back(tmp);




//-------PT GEN = 0.1 ---------
//***** 10 *****
tmp.at(0) = 0.10;
tmp.at(1) = 2.4;
tmp.at(2) = 0.10;
tmp.at(3) = 2.4;
accMap.push_back(tmp);

//***** 11 *****
tmp.at(0) = 0.10;
tmp.at(1) = 2.;
tmp.at(2) = 0.10;
tmp.at(3) = 2.;
accMap.push_back(tmp);

//***** 12 *****
tmp.at(0) = 0.10;
tmp.at(1) = 1.5;
tmp.at(2) = 0.10;
tmp.at(3) = 1.5;
accMap.push_back(tmp);

//***** 13 *****
tmp.at(0) = 0.10;
tmp.at(1) = 1.;
tmp.at(2) = 0.10;
tmp.at(3) = 1.;
accMap.push_back(tmp);

//***** 14 *****
tmp.at(0) = 0.10;
tmp.at(1) = 0.5;
tmp.at(2) = 0.10;
tmp.at(3) = 0.5;
accMap.push_back(tmp);





//-------PT GEN = 0.5 ---------
//***** 15 *****
tmp.at(0) = 0.5;
tmp.at(1) = 2.4;
tmp.at(2) = 0.5;
tmp.at(3) = 2.4;
accMap.push_back(tmp);

//***** 16 *****
tmp.at(0) = 0.5;
tmp.at(1) = 2.;
tmp.at(2) = 0.5;
tmp.at(3) = 2.;
accMap.push_back(tmp);

//***** 17 *****
tmp.at(0) = 0.5;
tmp.at(1) = 1.5;
tmp.at(2) = 0.5;
tmp.at(3) = 1.5;
accMap.push_back(tmp);

//***** 18 *****
tmp.at(0) = 0.5;
tmp.at(1) = 1.;
tmp.at(2) = 0.5;
tmp.at(3) = 1.;
accMap.push_back(tmp);

//***** 19 *****
tmp.at(0) = 0.5;
tmp.at(1) = 0.5;
tmp.at(2) = 0.5;
tmp.at(3) = 0.5;
accMap.push_back(tmp);




//-------PT GEN = 1 ---------
//***** 20 *****
tmp.at(0) = 1;
tmp.at(1) = 2.4;
tmp.at(2) = 1;
tmp.at(3) = 2.4;
accMap.push_back(tmp);

//***** 21 *****
tmp.at(0) = 1;
tmp.at(1) = 2.;
tmp.at(2) = 1;
tmp.at(3) = 2.;
accMap.push_back(tmp);

//***** 22 *****
tmp.at(0) = 1;
tmp.at(1) = 1.5;
tmp.at(2) = 1;
tmp.at(3) = 1.5;
accMap.push_back(tmp);

//***** 23 *****
tmp.at(0) = 1;
tmp.at(1) = 1.;
tmp.at(2) = 1;
tmp.at(3) = 1.;
accMap.push_back(tmp);

//***** 24 *****
tmp.at(0) = 1;
tmp.at(1) = 0.5;
tmp.at(2) = 1;
tmp.at(3) = 0.5;
accMap.push_back(tmp);




//-------PT GEN = 0.5 --------- eta2.5 for internal comparison
//***** 25 *****
tmp.at(0) = 0.1;
tmp.at(1) = 2.5;
tmp.at(2) = 0.1;
tmp.at(3) = 2.5;
accMap.push_back(tmp);


//-------PT GEN = 0.5 --------- negative charge
//***** 26 *****
tmp.at(0) = 0.1;
tmp.at(1) = 2.4;
tmp.at(2) = 0.1;
tmp.at(3) = 2.4;
tmp.at(4) = -1;
accMap.push_back(tmp);


//-------PT GEN = 0.5 --------- positive charge
//***** 27 *****
tmp.at(0) = 0.1;
tmp.at(1) = 2.4;
tmp.at(2) = 0.5;
tmp.at(3) = 2.1;
tmp.at(4) = 1;
accMap.push_back(tmp);
