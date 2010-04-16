vector<TString>* pname = new vector<TString>();
pname->push_back("nch");
pname->push_back("pt");
pname->push_back("pt2");
pname->push_back("eta");
pname->push_back("ptVSnch");
pname->push_back("ptmVSnch");
  
  
for(vector<TString>::iterator itplot=pname->begin() ; itplot!=pname->end() ; ++itplot){
  
  TString plot = *itplot;
  
  gDirectory->mkdir(plot);
  gDirectory->cd(plot);
  
  TH1* plot_toCorrect;
  if(!useData) plot_toCorrect = (TH1*) mc->Get(dir+st("MultiPlots_evtSel_INC_reco",acc)+st("/"+plot+"_evtSel_INC_reco",acc));
  else         plot_toCorrect = (TH1*) data->Get(dir+st("MultiPlots_evtSel_INC_reco",acc)+st("/"+plot+"_evtSel_INC_reco",acc));
  
  //Protecting against non-existing histo
  if(plot_toCorrect==0){
    gDirectory->cd("../");
    continue;
  }
  
  plot_toCorrect->SetName(plot+"_data_toCorrect");
   
  
  TH1* plot_noSel_NSD_gen = (TH1F*) mc->Get(dir+st("MultiPlots_etaCut_noSel_NSD_gen",acc)+st("/"+plot+"_etaCut_noSel_NSD_gen",acc));
  TH1* plot_evtSel_INC_reco = (TH1F*) mc->Get(dir+st("MultiPlots_evtSel_INC_reco",acc)+st("/"+plot+"_evtSel_INC_reco",acc));
  plot_noSel_NSD_gen->SetName(plot+"_MC_gen_afterCorrection");
  plot_evtSel_INC_reco->SetName(plot+"_MC_reco_beforeCorrection");
    
  plot_noSel_NSD_gen->Scale(1./(mp_etaCut_noSel_NSD_gen->nbEvts
                              - scaleWbin0 * mp_etaCut_noSel_NSD_gen->nch->GetBinContent(1)),"width");
  plot_evtSel_INC_reco->Scale(1./(mp_etaCut_evtSel_INC_reco_MC->nbEvts
                              - scaleWbin0 * mp_etaCut_evtSel_INC_reco_MC->nch->GetBinContent(1)),"width");
  plot_toCorrect->Scale(1./(mp_etaCut_evtSel_INC_reco_data->nbEvts
                              - scaleWbin0 * mp_etaCut_evtSel_INC_reco_data->nch->GetBinContent(1)),"width");
  
  TH1* eff_plot = (TH1F*) plot_evtSel_INC_reco->Clone("eff_"+plot);
  eff_plot->Divide(eff_plot,plot_noSel_NSD_gen,1,1,"B");
    
  /*TCanvas* nn = new TCanvas();
  //TH1F* eff_eta_bayes = (TH1F*) eta_evtSel_INC_reco->Clone("eff_eta_bayes");
  TGraphAsymmErrors* eff_eta_bayes = new TGraphAsymmErrors(matrixsize);//eta_evtSel_INC_reco,eta_noSel_NSD_gen);
  eff_eta_bayes->BayesDivide(eta_evtSel_INC_reco,eta_noSel_NSD_gen);
  //eff_eta_bayes->SetLineColor(kBlue);*/
  
  
  TH1* plot_corrected  = (TH1*) plot_toCorrect->Clone(plot+"_corrected");
  plot_corrected->Divide(plot_corrected,eff_plot,1,1);
  
if(drawcanv){
  TCanvas* c_plot = new TCanvas("c_"+plot,"c_"+plot,1460,10,500,500);
  c_plot->SetLeftMargin(0.17);
  c_plot->SetBottomMargin(0.10);
  c_plot->SetFillColor(0);
  c_plot->GetFrame()->SetFillColor(21);
  c_plot->GetFrame()->SetBorderSize(12);
  c_plot->SetGrid();
  
  
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(kFALSE);


  //plot plot
  plot_noSel_NSD_gen->SetLineWidth(2);
  plot_toCorrect->SetLineWidth(2);
  plot_corrected->SetLineWidth(2);
  plot_noSel_NSD_gen->SetLineColor(kRed);
  plot_toCorrect->SetLineColor(kBlue);
  plot_corrected->SetLineColor(kBlack);

  
  TLegend *leg = new TLegend (.65,.90,.90,.99);
  leg->AddEntry(plot_noSel_NSD_gen,"PYTHIA D6T","l" );
  leg->AddEntry(plot_toCorrect,"Data - Raw","l" );
  leg->AddEntry(plot_corrected,"Data - Corrected","l" );
  leg->SetBorderSize(0);
  leg->SetFillColor(0);
  
  plot_corrected->Draw("hist");
  plot_toCorrect->Draw("samee");
  plot_noSel_NSD_gen->Draw("samee");
  leg->Draw();
  /*c1->SaveAs("eta_"+name+".gif","");
  c1->SaveAs("eta_"+name+".eps","");
  c1->SaveAs("eta_"+name+".root","");*/
  
  gPad->WaitPrimitive();
}
  
  plot_noSel_NSD_gen->Write();
  plot_evtSel_INC_reco->Write();
  plot_toCorrect->Write();
  eff_plot->Write();
  plot_corrected->Write();
  
  if(itplot->Contains("eta")){
    cout<<"Integral(eta) * binwidth = "<<plot_corrected->Integral()*plot_corrected->GetBinWidth(1)<<endl;
  }
  
  gDirectory->cd("../");
}  
