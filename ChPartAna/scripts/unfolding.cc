void unfold(TH1F* unfoldedDistr,TH1F* unfoldedProb,double inputMat[][matrixsize],TH1F* auxEffect,TH1F* hypothesis, TH1F* err);
bool checkMatrix(double inputMat[][matrixsize],double inputMatNormalized[][matrixsize]);
//void specifyHypothesis(std::string curveShape, TH1F* hypothesis);
bool isGenLineVoid(double inputMat[][matrixsize],int indx_gen);
bool isRecoLineVoid(double inputMat[][matrixsize],int indx_reco);

//TH1F runalgo(double matrix[][matrixsize], TH1F* toUnfold);


//All this is defined outside, to be valid everywhere
//-------> TO FIX !
int Ngen1 = matrixsize;  
int Nreco1= matrixsize; 
typedef double matrix4dObj [matrixsize][matrixsize];
typedef double matrix4dObj_smear [matrixsize][matrixsize];//inverse from previous (!! at the indices)

//Eff distri
//TH1F* binEff=new TH1F("binEff","binEff",Ngen1,0,Ngen1);

//Generator true distribution
//TH1F* truegen=new TH1F("truegen","truegen",Ngen1,0,Ngen1);

int debug_ = 0;

using namespace std;

int iterStep=0;

TH1F runalgo(double matrix[][matrixsize], TH1F* toUnfold, TH1F* hypothesis, int niter = 5, int nsample = 0){
  //cout<<"starting the unfolding ..."<<endl;
  
  //The hypothesis distribution
  /*char* hypchar = "hypothesis_nsample%";
  sprintf(hypchar,hypchar,nsample);
  TH1F* hypothesis= new TH1F(hypchar,hypchar,Ngen1,-0.5,double(Ngen1)-0.5);
  */
  //Definition of the matrices
  matrix4dObj matrix_normalized;
  checkMatrix(matrix,matrix_normalized);

  //Load the correct hypothesis in hypothesis
  // ---> check the type !! for now, only "uniform" 
  //if(hyp==0)specifyHypothesis("gauss",hypothesis);
  
 /* char hypchar[160] = "hypothesis_nsample%d";
  sprintf(hypchar,hypchar,nsample);
  TH1F* hypothesis = (TH1F*) hyp->Clone(hypchar);
  hypothesis->SetNameTitle(hypchar,hypchar);
*/
    //Fill efficiency plot (eff vs gen var)
  //Can be used later to correct the unfolding (in case sum over gen line !=1, for trigger inef for instance)
  //Not implemented yet ....
  //fillEfficiency(binEff);
  
  //TFile* out = new TFile("output.root","UPDATE");
  //out->cd();
  //gDirectory->cd();
  
  
  vector<TH1F*>* vtoUnfold = new vector<TH1F*>(niter,new TH1F()) ;
  vector<TH1F*>* vtoUnfold_scaled = new vector<TH1F*>(niter,new TH1F());
  vector<TH1F*>* verr_obs = new vector<TH1F*>(niter,new TH1F());
  
  for(int it = 0 ; it < niter ; ++it){
    char* tmp = "toUnfold_step%d";
    char name[160] = "";
    sprintf(name,tmp,niter);
    strcat(name,"_nsample%d");
    sprintf(name,name,nsample);
    vtoUnfold->at(it)->SetNameTitle(name,name);
    vtoUnfold->at(it)->SetBins(Ngen1,-0.5,double(Ngen1)-0.5);
    //vtoUnfold->at(it)->Sumw2();
    
    tmp = "toUnfold_scaled_step%d";
    sprintf(name,tmp,niter);
    strcat(name,"_nsample%d");
    sprintf(name,name,nsample);
    vtoUnfold_scaled->at(it)->SetNameTitle(name,name);
    vtoUnfold_scaled->at(it)->SetBins(Ngen1,-0.5,double(Ngen1)-0.5);
    //vtoUnfold_scaled->at(it)->Sumw2();
    
    tmp = "err_obs_step%d";
    sprintf(name,tmp,niter);
    strcat(name,"_nsample%d");
    sprintf(name,name,nsample);
    verr_obs->at(it)->SetNameTitle(name,name);
    verr_obs->at(it)->SetBins(Ngen1,-0.5,double(Ngen1)-0.5);
    //verr_obs->at(it)->Sumw2();
    
    if(it==0)
      unfold(vtoUnfold->at(it),vtoUnfold_scaled->at(it),matrix_normalized,toUnfold,hypothesis,verr_obs->at(it));
    else
      unfold(vtoUnfold->at(it),vtoUnfold_scaled->at(it),matrix_normalized,toUnfold,vtoUnfold->at(it-1),verr_obs->at(it));
  }
  
  
  //cout<<"Finished the unfolding, returning the unfolded histo"<<endl;
  return *(vtoUnfold->at(niter-1));
  
  /*char* tmp = "toUnfold_step%d";
  char nsamplebis[60] = "";
  printf(nsamplebis,tmp,nsample);
  //1st ITERATION
  TH1F* toUnfold_step1         = new TH1F(("toUnfold_step1_"+(string)nsamplebis).c_str(),"toUnfold_step1",Ngen1,-0.5,double(Ngen1)-0.5);
  TH1F* toUnfold_scaled_step1  = new TH1F("toUnfold_scaled_step1","toUnfold_scaled_step1",Ngen1,-0.5,double(Ngen1)-0.5);
  TH1F* err_obs_step1          = new TH1F("err_obs_step1","err_obs_step1",Ngen1,-0.5,double(Ngen1)-0.5);
  toUnfold_step1->Sumw2();
  toUnfold_scaled_step1->Sumw2();
  err_obs_step1->Sumw2();
  
  unfold(toUnfold_step1,toUnfold_scaled_step1,matrix_normalized,toUnfold,hypothesis,err_obs_step1);

  //2nd ITERATION
  TH1F* toUnfold_step2         = new TH1F(("toUnfold_step2_"+(string)nsamplebis).c_str(),"toUnfold_step2",Ngen1,-0.5,double(Ngen1)-0.5);
  TH1F* toUnfold_scaled_step2  = new TH1F("toUnfold_scaled_step2","toUnfold_scaled_step2",Ngen1,-0.5,double(Ngen1)-0.5);
  TH1F* err_obs_step2          = new TH1F("err_obs_step2","err_obs_step2",Ngen1,-0.5,double(Ngen1)-0.5);
  toUnfold_step2->Sumw2();
  toUnfold_scaled_step2->Sumw2();
  err_obs_step2->Sumw2();
  
  unfold(toUnfold_step2,toUnfold_scaled_step2,matrix_normalized,toUnfold,toUnfold_scaled_step1,err_obs_step2);
  
  //3rd ITERATION
  TH1F* toUnfold_step3         = new TH1F(("toUnfold_step3_"+(string)nsamplebis).c_str(),"toUnfold_step3",Ngen1,-0.5,double(Ngen1)-0.5);
  TH1F* toUnfold_scaled_step3  = new TH1F("toUnfold_scaled_step3","toUnfold_scaled_step3",Ngen1,-0.5,double(Ngen1)-0.5);
  TH1F* err_obs_step3          = new TH1F("err_obs_step3","err_obs_step3",Ngen1,-0.5,double(Ngen1)-0.5);
  toUnfold_step3->Sumw2();
  toUnfold_scaled_step3->Sumw2();
  err_obs_step3->Sumw2();
  
  unfold(toUnfold_step3,toUnfold_scaled_step3,matrix_normalized,toUnfold,toUnfold_scaled_step2,err_obs_step3);
  
  //4th ITERATION
  TH1F* toUnfold_step4         = new TH1F(("toUnfold_step4_"+(string)nsamplebis).c_str(),"toUnfold_step4",Ngen1,-0.5,double(Ngen1)-0.5);
  TH1F* toUnfold_scaled_step4  = new TH1F("toUnfold_scaled_step4","toUnfold_scaled_step4",Ngen1,-0.5,double(Ngen1)-0.5);
  TH1F* err_obs_step4          = new TH1F("err_obs_step4","err_obs_step4",Ngen1,-0.5,double(Ngen1)-0.5);
  toUnfold_step4->Sumw2();
  toUnfold_scaled_step4->Sumw2();
  err_obs_step4->Sumw2();
  
  unfold(toUnfold_step4,toUnfold_scaled_step4,matrix_normalized,toUnfold,toUnfold_scaled_step3,err_obs_step4);
  
  //5th ITERATION
  TH1F* toUnfold_step5         = new TH1F(("toUnfold_step5_"+(string)nsamplebis).c_str(),"toUnfold_step5",Ngen1,-0.5,double(Ngen1)-0.5);
  TH1F* toUnfold_scaled_step5  = new TH1F("toUnfold_scaled_step5","toUnfold_scaled_step5",Ngen1,-0.5,double(Ngen1)-0.5);
  TH1F* err_obs_step5          = new TH1F("err_obs_step5","err_obs_step5",Ngen1,-0.5,double(Ngen1)-0.5);
  toUnfold_step5->Sumw2();
  toUnfold_scaled_step5->Sumw2();
  err_obs_step5->Sumw2();
  
  unfold(toUnfold_step5,toUnfold_scaled_step5,matrix_normalized,toUnfold,toUnfold_scaled_step4,err_obs_step5);
  
  
  //Copy previous to make more iterations
  
  toUnfold->Write();
  toUnfold_step1->Write();
  toUnfold_step2->Write();
  toUnfold_step3->Write();
  toUnfold_step4->Write();
  toUnfold_step5->Write();
  hypothesis->Write();
  
  delete toUnfold_scaled_step1;
  delete err_obs_step1;
  delete toUnfold_scaled_step2;
  delete err_obs_step2;
  delete toUnfold_scaled_step3;
  delete err_obs_step3;
  delete toUnfold_scaled_step4;
  delete err_obs_step4;
  delete toUnfold_scaled_step5;
  delete err_obs_step5;
  delete hypothesis;
  
  cout<<"Finished the unfolding, returning the unfolded histo"<<endl;
  
  if(niter == 1) return *toUnfold_step1;
  if(niter == 2) return *toUnfold_step2;
  if(niter == 3) return *toUnfold_step3;
  if(niter == 4) return *toUnfold_step4;
  if(niter == 5) return *toUnfold_step5;*/
}

void unfold(TH1F* unfoldedDistr,TH1F* unfoldedProb,double inputMat[][matrixsize],TH1F* auxEffect,TH1F* hypothesis,TH1F* err){
  ++iterStep;
  
  if (debug_){
    cout<<"Iteration "<<iterStep<<" of unfolding process"<<endl;
  }
  
  matrix4dObj_smear smearing;
  for(int j=0;j<Nreco1;j++){
  
    //If the reco line is non-void
    //if(!isRecoLineVoid(inputMat,j)){ //NOT NEEDED
      
      //Taking the reco line*hypothesis for each bin
      double sum=0.;
      for(int i=0;i<Ngen1;i++)
	sum+=inputMat[j][i]*hypothesis->GetBinContent(i+1);
	
      //Checking if the line is not void once smeared. If so, continue.
      if (sum==0 && debug_){
	cout<<"Sum of gen*hypothesis is = 0, please check the content of the matrix/hypothesis."<<endl; 
	cout<<"Now passing to the next reco line ..."<<endl;  
	//for (int i=0;i<Ngen1;i++)
	  //cout<<"matrix["<<j<<"]["<<i<<"]:"<<inputMat[j][i]<<" hypothesis:"<<hypothesis->GetBinContent(i+1)<<endl;
	  //continue;
      }
      
      //calculating inverse, * hypothesis, normalized by sum over reco line
      for(int i=0;i<Ngen1;i++){
	if(sum!=0)smearing[i][j]=inputMat[j][i]*hypothesis->GetBinContent(i+1)/sum;
        else smearing[i][j]=0;
	
        if(debug_==2){
	  cout<<" normalized matrix @ ("<<i<<" , "<<j<<") = "<<inputMat[j][i]<<endl;
	  cout<<" hypothesis @ gen line "<<i<<" = "<<hypothesis->GetBinContent(i+1)<<endl;
	  cout<<" sum gen @ reco line "<<j<<" = "<<sum<<endl;
	  cout<<" Smeared matrix @ ("<<i<<" , "<<j<<") = "<<smearing[i][j]<<endl;
        }
      }
    /*}
    else
      std::cout<<"skipped reco line "<<j<<" while creating smearing because it is void"<<std::endl;*/
  }
  

  if (debug_) std::cout<<" finished creating smearing"<<std::endl;

  //Here the efficiency of the cause is 1, ie Sum of a line is normalized to 1 (you can't lose any events).
  for (int i=0;i<Ngen1;i++){
    double auxFill=0.;
    
    //Summing over the smeared reco line (ie standard gen line)
    for (int j=0;j<Nreco1;j++){
      auxFill+=auxEffect->GetBinContent(j+1)*smearing[i][j];
      
      if(debug_==2){
        cout<<" smeared matrix @ ("<<j<<" , "<<i<<") = "<<smearing[i][j]<<endl;
	cout<<" toUnfold @ reco line "<<j<<" = "<<auxEffect->GetBinContent(j+1)<<endl;
	cout<<" sum toUnfold*smeared @ gen line "<<i<<" = "<<auxFill<<endl;
      }
      
    }
    
    //if (!auxFill)//isGenLineVoid(inputMat,i))
      unfoldedDistr->SetBinContent(i+1,auxFill);
    //else
    //  if (debug_) std::cout<<"skipped smeared gen line"<<i<<" while filling unfoldedDistr"<<std::endl;  
  }

  if (debug_) std::cout<<"     ... finished algo kernel of the iteration"<<std::endl;
   
  for (int i=1;i<=Ngen1;i++)
    unfoldedProb->SetBinContent(i,unfoldedDistr->GetBinContent(i));
  unfoldedProb->Scale(1./unfoldedProb->Integral());

  
  //CALCULATING THE ERROR: ** Not done yet
/*  
  double Ntrue=unfoldedDistr->Integral();
  // 1) contributo dell'osservazione
  matrix4dObj_V matrixV_obs;
  for (int k=1;k<=Ngen1;k++){
    for (int c=1;c<=Ngen2;c++){
      for (int l=1;l<=Ngen1;l++){
        for (int m=1;m<=Ngen2;m++){
if (DEBUG) std::cout<<"cell "<<k<<","<<c<<","<<l<<","<<m<<std::endl;
          if (k==l && c==m){
            double auxFill=0;
            for (int j=1;j<=Nreco1;j++) 
              for (int f=1;f<=Nreco2;f++) 
                auxFill+=smearing[k][c][j][f]*smearing[l][m][j][f]*auxEffect->GetBinContent(j,f)*(1.-auxEffect->GetBinContent(j,f)/Ntrue);
            for (int j=1;j<=Nreco1;j++){
              for (int f=1;f=Nreco2;f++){
                for (int a=1;a<=Nreco1;a++){
                  for (int b=1;b=Nreco2;b++){
	            if (j==a && f==b){}
		    else
                      auxFill-=smearing[k][c][j][f]*smearing[l][m][a][b]*auxEffect->GetBinContent(j,f)*auxEffect->GetBinContent(a,b)/Ntrue;//matrixM.GetBinContent(i,k)*matrixM.GetBinContent(j,l)*auxEffect->GetBinContent(j)*auxEffect->GetBinContent(i)/Ntrue;
	      }}}}
             matrixV_obs[k][c][l][m]=auxFill;
             }//end if k==l && c==m, only interesting cell
          }
        }
      }
    }
  for (int i=1;i<=err->GetNbinsX();i++)
    for (int j=1;j<=err->GetNbinsY();j++)
      err->SetBinContent(i,j,matrixV_obs[i][j][i][j]);
//*/  
}

bool checkMatrix(double inputMat[][matrixsize],double inputMatNormalized[][matrixsize]){
  
  //Check if there is a void Gen line
  //The Reco line will be checked during the algo
  /*for(int i=0;i<Ngen1;++i){
    if(isGenLineVoid(inputMat,i)){
      cout<<"Gen line "<<i<<" is void. Please check your matrix !"<<endl;
      cout<<"Now exiting ..."<<endl;
      return true;
    }
  }*/ //INFACT NOT NECESSARY
  
  //Normalizing only if no void gen lines
  for(int i=0;i<Ngen1;i++){
    double sum=0;
    for (int j=0;j<Nreco1;j++)
      sum+=(double)inputMat[j][i];	
    for (int j=0;j<Nreco1;j++){
      if(sum!=0)
	inputMatNormalized[j][i]=(double)inputMat[j][i]/(double)sum;
      else
        inputMatNormalized[j][i]=0;
      if(debug_==2){
        cout<<" matrix @ ("<<i<<" , "<<j<<") = "<<inputMat[j][i]<<endl;
        cout<<" normalized matrix @ ("<<i<<" , "<<j<<") = "<<inputMatNormalized[j][i]<<endl;
      }
    
    
    }
  }
  
  if (debug_)  std::cout<<"                ...matrix normalized"<<std::endl;
}


bool isGenLineVoid(double inputMat[][matrixsize],int indx_gen){
  double sum=0;
  for (int q=0;q<Nreco1;++q)
    sum+=inputMat[q][indx_gen];
  if (sum==0) return 1;
  else        return 0;
}

bool isRecoLineVoid(double inputMat[][matrixsize],int indx_reco){
  double sum=0;
  for (int q=0;q<Ngen1;++q)
    sum+=inputMat[indx_reco][q];
  if (sum==0) return 1;
  else        return 0;
}


