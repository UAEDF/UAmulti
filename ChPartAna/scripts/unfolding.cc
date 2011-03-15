void unfold(TH1F* unfoldedDistr,TH1F* unfoldedProb,TH1F* hyp_reco,double inputMat[][matrixsize],TH1F* auxEffect,TH1F* hypothesis, TH1F* err);
bool checkMatrix(double inputMat[][matrixsize],double inputMatNormalized[][matrixsize]);
//void specifyHypothesis(std::string curveShape, TH1F* hypothesis);
bool isGenLineVoid(double inputMat[][matrixsize],int indx_gen);
bool isRecoLineVoid(double inputMat[][matrixsize],int indx_reco);
double getChi2(TH1F*,TH1F*,int = 0);


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

using namespace std;

int iterStep=0;

TH1F runalgo(TH2F* matrixhist, TH1F* toUnfold, TH1F* hypothesis, int niter = 5, int nsample = 0, bool smooth = false , bool smoothInputDistr = false/* , bool reweightmtx = true*/){
//TH1F runalgo(double matrix[][matrixsize], TH1F* toUnfold, TH1F* hypothesis, int niter = 5, int nsample = 0, bool smooth = true , bool smoothInputDistr = false){
  //cout<<"starting the unfolding ..."<<endl;
  
  //The hypothesis distribution
  /*char* hypchar = "hypothesis_nsample%";
  sprintf(hypchar,hypchar,nsample);
  TH1F* hypothesis= new TH1F(hypchar,hypchar,Ngen1,-0.5,double(Ngen1)-0.5);
  */
  
  
 /* if(reweightmtx){
  
    TH1D* projY = (TH1D*) (matrixhist->ProjectionY());
    //reweighting the reco side
    for(int ireco = 1 ; ireco <= matrixhist->GetNbinsY() ; ++ireco){
      double factor = 0;
      if(projY->GetBinContent(ireco)!=0)
        factor =  toUnfold->GetBinContent(ireco)  / projY->GetBinContent(ireco);
      for(int igen = 1 ; igen <= matrixhist->GetNbinsX() ; ++igen){
        matrixhist->SetBinContent(igen , ireco ,  matrixhist->GetBinContent(igen , ireco) * factor );
      }
    }
    
    
  
  }*/
  //protection
  if(niter < 1 ) niter = 1;
  
  if(debug_!=0) {
    matrixhist->Draw();
    gPad->WaitPrimitive();
  } 
  
  if(debug_!=0) {
    toUnfold->Draw();
    gPad->WaitPrimitive();
  }
  if(debug_!=0) {
    hypothesis->Draw();
    gPad->WaitPrimitive();
  }
  //Definition of the matrices
  matrix4dObj matrix;
  matrix4dObj matrix_normalized;
  transform2Matrix(matrixhist,matrix);
  checkMatrix(matrix,matrix_normalized);
  
  if(smoothInputDistr){
    //toUnfold->Smooth(2);
    toUnfold->GetXaxis()->SetRangeUser(0.5,toUnfold->GetXaxis()->GetXmax());
    divideByWidth(toUnfold);
    toUnfold->Smooth(5,"R");
    multiplyByWidth(toUnfold);
    toUnfold->GetXaxis()->SetRangeUser(-0.5,toUnfold->GetXaxis()->GetXmax());
  }
  
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
  
  if(nsample == 0){
    gDirectory->mkdir("iterations");
    gDirectory->cd("iterations");
  }
  
  TH1F* chi2VSniter = NULL;
  if(nsample == 0) chi2VSniter = new TH1F("chi2VSniter","chi2VSniter;# iter;#chi^2",niter-1,0.5,niter-0.5);
  
  vector<TH1F*>* vtoUnfold        = new vector<TH1F*>(niter,new TH1F());
  vector<TH1F*>* vtoUnfold_scaled = new vector<TH1F*>(niter,new TH1F());
  vector<TH1F*>* verr_obs         = new vector<TH1F*>(niter,new TH1F());
  vector<TH1F*>* vhyp_reco        = new vector<TH1F*>(niter,new TH1F());
  
  for(int it = 0 ; it < niter ; ++it){
    char* tmp = (char*) "toUnfold_step%d";
    char name[160] = "";
    sprintf(name,tmp,it);
    strcat(name,"_nsample%d");
    sprintf(name,name,nsample);
    vtoUnfold->at(it)=(TH1F*) toUnfold->Clone(name);
    //vtoUnfold->at(it)->SetNameTitle(name,name);
    //vtoUnfold->at(it)->SetBins(toUnfold->GetNbinsX(),toUnfold->GetXaxis()->GetXbins()->GetArray());
    vtoUnfold->at(it)->Reset("M");
    //vtoUnfold->at(it)->Sumw2();
    
    if(debug_) cout << "step further"<<endl;
    tmp = (char*) "toUnfold_scaled_step%d";
    sprintf(name,tmp,it);
    strcat(name,"_nsample%d");
    sprintf(name,name,nsample);
     vtoUnfold_scaled->at(it)=(TH1F*) toUnfold->Clone(name);
    //vtoUnfold_scaled->at(it)->SetNameTitle(name,name);
    //vtoUnfold_scaled->at(it)->SetBins(toUnfold->GetNbinsX(),toUnfold->GetXaxis()->GetXbins()->GetArray());
    //vtoUnfold_scaled->at(it)->Sumw2();
    vtoUnfold_scaled->at(it)->Reset("M");
    
    tmp = (char*) "err_obs_step%d";
    sprintf(name,tmp,it);
    strcat(name,"_nsample%d");
    sprintf(name,name,nsample);
    verr_obs->at(it)=(TH1F*) toUnfold->Clone(name);
    //verr_obs->at(it)->SetNameTitle(name,name);
    //verr_obs->at(it)->SetBins(toUnfold->GetNbinsX(),toUnfold->GetXaxis()->GetXbins()->GetArray());
    verr_obs->at(it)->Reset("M");
    
    tmp = (char*) "recoHypothesis_step%d";
    sprintf(name,tmp,it);
    strcat(name,"_nsample%d");
    sprintf(name,name,nsample);
    vhyp_reco->at(it)=(TH1F*) toUnfold->Clone(name);
    //vhyp_reco->at(it)->SetNameTitle(name,name);
    //vhyp_reco->at(it)->SetBins(toUnfold->GetNbinsX(),toUnfold->GetXaxis()->GetXbins()->GetArray());
    //verr_obs->at(it)->Sumw2();
    vhyp_reco->at(it)->Reset("M");
   
    if(debug_) cout << "after cloning" <<endl;
    
    if(it==0)
      unfold(vtoUnfold->at(it),vtoUnfold_scaled->at(it),vhyp_reco->at(it),matrix_normalized,toUnfold,hypothesis,verr_obs->at(it));
    else
      unfold(vtoUnfold->at(it),vtoUnfold_scaled->at(it),vhyp_reco->at(it),matrix_normalized,toUnfold,vtoUnfold->at(it-1),verr_obs->at(it));
  
    if(nsample==0){
      vtoUnfold->at(it)->Write();
      vhyp_reco->at(it)->Write();
      if(it!=0){
        cout<<"iter "<<it<<"  chi2 : "<<getChi2(vhyp_reco->at(it) , toUnfold ,1)<<endl;
        chi2VSniter->SetBinContent(it , getChi2(vhyp_reco->at(it) , toUnfold ,1));
      }
    }
    
    //smoothing the hypothesis
    if(smooth && it!=niter-1){
      vtoUnfold->at(it)->GetXaxis()->SetRangeUser(0.5,vtoUnfold->at(it)->GetXaxis()->GetXmax());
      divideByWidth(vtoUnfold->at(it));
      vtoUnfold->at(it)->Smooth(5,"R");
      multiplyByWidth(vtoUnfold->at(it));
      vtoUnfold->at(it)->GetXaxis()->SetRangeUser(-0.5,vtoUnfold->at(it)->GetXaxis()->GetXmax());
      if(nsample==0)
        vtoUnfold->at(it)->Write(vtoUnfold->at(it)->GetName()+TString("_smoothed"));
    }
    
    
    
    
    
  }
  
  if(nsample==0){
    chi2VSniter->Write();  
    gDirectory->cd("../");
  }
  

  for(unsigned int jj=vtoUnfold->size()-1; jj==0;jj--){
       delete vtoUnfold_scaled->at(jj);
       delete verr_obs->at(jj);
       delete vhyp_reco->at(jj);    
  }
  delete vtoUnfold_scaled;
  delete verr_obs;
  delete vhyp_reco;
  
  //cout<<"Finished the unfolding, returning the unfolded histo"<<endl;
  return *(vtoUnfold->at(niter-1));
  
  
}

void unfold(TH1F* unfoldedDistr,TH1F* unfoldedProb,TH1F* hyp_reco,double inputMat[][matrixsize],TH1F* auxEffect,TH1F* hypothesis,TH1F* err){
  ++iterStep;
  
  err->GetName();
  
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
  
  
  //Doing the hyp_reco
  for(int i=0;i<Nreco1;i++){
    double auxFill = 0.;
    for(int j=0;j<Ngen1;j++)
      auxFill+=hypothesis->GetBinContent(j+1) * inputMat[i][j];
      
    hyp_reco->SetBinContent(i+1,auxFill);
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
  return true;
}

double getChi2(TH1F* hist1 , TH1F* hist2 , int errortype){
  double bin = 0 , error = 0 , sumbin = 0 , ndof = 0;
  for(int i=1;i<=hist1->GetNbinsX();++i){
    if(hist2->GetBinContent(i)!=0){
      ++ndof;
      bin = pow( hist1->GetBinContent(i) - hist2->GetBinContent(i) , 2 );
      if(errortype==0) error = fabs(hist2->GetBinContent(i));
      if(errortype==1) error = pow(hist2->GetBinError(i),2);
      if(error!=0) sumbin += bin / error ;
      else sumbin+=bin;
    }
  }
  return (ndof!=0) ? sumbin / ndof : -1;
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


