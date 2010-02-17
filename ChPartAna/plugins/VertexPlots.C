
#include "VertexPlots.h"

ClassImp(VertexPlots)

VertexPlots::VertexPlots(){
  vertexcoll = "none";
  this->init();
}

VertexPlots::VertexPlots(TString collname){
  vertexcoll = collname;
  this->init();
}

VertexPlots::~VertexPlots(){ }

void VertexPlots::init(){
  x = new TH1F("x_"+vertexcoll,"x_"+vertexcoll+";x;# events",100,-1.,1.);
  y = new TH1F("y_"+vertexcoll,"y_"+vertexcoll+";y;# events",100,-1.,1.);
  z = new TH1F("z_"+vertexcoll,"z_"+vertexcoll+";z;# events",150,-15.,15.);
  z_wide = new TH1F("z_wide_"+vertexcoll,"z_wide_"+vertexcoll+";z;# events",200,-100.,100.);
  ex = new TH1F("ex_"+vertexcoll,"ex_"+vertexcoll+";ex;# events",100,0.,2.);
  ey = new TH1F("ey_"+vertexcoll,"ey_"+vertexcoll+";ey;# events",100,0.,2.);
  ez = new TH1F("ez_"+vertexcoll,"ez_"+vertexcoll+";ez;# events",100,0.,10.);
  chi2n = new TH1F("chi2n_"+vertexcoll,"chi2n_"+vertexcoll+";#chi^{2}/ndof;# events",80,0.,8.);
  ntracks = new TH1F("ntracks_"+vertexcoll,"ntracks_"+vertexcoll+";ntracks;# events",31,-0.5,30.5);
  
  xy = new TH2F("xy_"+vertexcoll,"xy_"+vertexcoll+";x;y;# events",100,-2.,2.,100,-2.,2.);
  xz = new TH2F("xz_"+vertexcoll,"xz_"+vertexcoll+";x;z;# events",100,-2.,2.,100,-10.,10.);
  yz = new TH2F("yz_"+vertexcoll,"yz_"+vertexcoll+";y;z;# events",100,-2.,2.,100,-10.,10.);
}

void VertexPlots::fill(vector<MyVertex>& vtxcoll , double weight){
  for(vector<MyVertex>::iterator vtx = vtxcoll.begin() ; vtx != vtxcoll.end() ; ++vtx)
    this->fill(*vtx,weight);
}

void VertexPlots::fill(MyVertex& vtx , double weight){
  x->Fill(vtx.x, weight);
  y->Fill(vtx.y, weight);
  z->Fill(vtx.z, weight);
  z_wide->Fill(vtx.z, weight);
  ex->Fill(vtx.ex, weight);
  ey->Fill(vtx.ey, weight);
  ez->Fill(vtx.ez, weight);
  chi2n->Fill(vtx.chi2n, weight);
  ntracks->Fill(vtx.ntracks, weight);
  xy->Fill(vtx.x,vtx.y, weight);
  xz->Fill(vtx.x,vtx.z, weight);
  yz->Fill(vtx.y,vtx.z, weight);
}

void VertexPlots::write(){
  //TDirectory* dir = new TDirectory("VertexPlots_"+vertexcoll,"");
  //dir->cd();  
  gDirectory->mkdir("VertexPlots_"+vertexcoll);
  gDirectory->cd("VertexPlots_"+vertexcoll);
  
  x->Write();
  y->Write();
  z->Write();
  z_wide->Write();
  ex->Write();
  ey->Write();
  ez->Write();
  chi2n->Write();
  ntracks->Write();
  xy->Write();
  xz->Write();
  yz->Write();
  
  gDirectory->cd("../");
}

