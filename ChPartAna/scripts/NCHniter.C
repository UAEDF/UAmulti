if      (hf =="HF0" && cen=="nocut"  && (acc=="cut0" || acc=="cut1" || acc=="cut4" || acc=="cut5")) niter = 4;
else if (hf =="HF0" && cen=="MBUEWG" && (acc=="cut0" || acc=="cut1" || acc=="cut4" || acc=="cut5")) niter = 4;
else if (hf =="HF0" && cen=="ALICE"  && (acc=="cut0" || acc=="cut1" || acc=="cut4" || acc=="cut5")) niter = 4;
else if (hf =="HF0" && cen=="ATLAS1" && (acc=="cut0" || acc=="cut1" || acc=="cut4" || acc=="cut5")) niter = 4;
else if (hf =="HF0" && cen=="ATLAS2" && (acc=="cut0" || acc=="cut1" || acc=="cut4" || acc=="cut5")) niter = 3;
else if (hf =="HF0" && cen=="ATLAS6" && (acc=="cut0" || acc=="cut1" || acc=="cut4" || acc=="cut5")) niter = 2;

else if (hf =="HF0" && cen=="nocut"  && (acc=="cut2" || acc=="cut3")) niter = 3;
else if (hf =="HF0" && cen=="MBUEWG" && (acc=="cut2" || acc=="cut3")) niter = 3;
else if (hf =="HF0" && cen=="ALICE"  && (acc=="cut2" || acc=="cut3")) niter = 2;
else if (hf =="HF0" && cen=="ATLAS1" && (acc=="cut2" || acc=="cut3")) niter = 4;
else if (hf =="HF0" && cen=="ATLAS2" && (acc=="cut2" || acc=="cut3")) niter = 3;
else if (hf =="HF0" && cen=="ATLAS6" && (acc=="cut2" || acc=="cut3")) niter = 2;

else if (hf =="HF1") niter = 5;

else {
    niter = 3;
    cout << "niter was not set ! It stays: " << niter <<endl;
}
