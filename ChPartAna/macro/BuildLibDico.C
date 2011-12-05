{
  gROOT->ProcessLine(".L ../mydir/MyPart.cc+");
  gROOT->ProcessLine(".L ../mydir/MyBaseJet.cc+");
  gROOT->ProcessLine(".L ../mydir/MyJet.cc+");
  gROOT->ProcessLine(".L ../mydir/MyTracks.cc+");
  gROOT->ProcessLine(".L ../mydir/MyVertex.cc+");
  gROOT->ProcessLine(".L ../mydir/MyBeamSpot.cc+");
  gROOT->ProcessLine(".L ../mydir/MyCaloTower.cc+");
  gROOT->ProcessLine(".L ../mydir/MyCaloJet.cc+");
  gROOT->ProcessLine(".L ../mydir/MyCastorDigi.cc+");
  gROOT->ProcessLine(".L ../mydir/MyCastorJet.cc+");
  gROOT->ProcessLine(".L ../mydir/MyCastorRecHit.cc+");
  gROOT->ProcessLine(".L ../mydir/MyDiJet.cc+");
  gROOT->ProcessLine(".L ../mydir/MyElectron.cc+");
  gROOT->ProcessLine(".L ../mydir/MyEvtId.cc+");
  gROOT->ProcessLine(".L ../mydir/MyFwdGap.cc+");
  gROOT->ProcessLine(".L ../mydir/MyGenKin.cc+");
  gROOT->ProcessLine(".L ../mydir/MyGenMet.cc+");
  gROOT->ProcessLine(".L ../mydir/MyGenPart.cc+");
  gROOT->ProcessLine(".L ../mydir/MyGenJet.cc+");
  gROOT->ProcessLine(".L ../mydir/MyHLTrig.cc+");
  gROOT->ProcessLine(".L ../mydir/MyL1Trig.cc+");
  gROOT->ProcessLine(".L ../mydir/MyL1TrigOld.cc+");
  gROOT->ProcessLine(".L ../mydir/MyMITEvtSel.cc+");
  gROOT->ProcessLine(".L ../mydir/MyMet.cc+");
  gROOT->ProcessLine(".L ../mydir/MyMuon.cc+");
  gROOT->ProcessLine(".L ../mydir/MyPFCand.cc+");
  gROOT->ProcessLine(".L ../mydir/MyPFJet.cc+");
  gROOT->ProcessLine(".L ../mydir/MyPUSumInfo.cc+");
  gROOT->ProcessLine(".L ../mydir/MySimVertex.cc+");
  gROOT->ProcessLine(".L ../mydir/LinkDef.cc+");


  gROOT->ProcessLine(".L ../plugins/TOperation.h+");
  gROOT->ProcessLine(".L ../plugins/toperationLinkDef.C+");

  gROOT->ProcessLine(".L ../plugins/TMean.C+");
  gROOT->ProcessLine(".L ../plugins/TMoments.C+");
  
  gROOT->ProcessLine(".L ../plugins/BasePlots.C+");
  gROOT->ProcessLine(".L ../plugins/MultiPlots.C+");
  gROOT->ProcessLine(".L ../plugins/GenMultiPlots.C+");
  gROOT->ProcessLine(".L ../plugins/MatrixPlots.C+");
  gROOT->ProcessLine(".L ../plugins/TResponseMtx.C+");
  gROOT->ProcessLine(".L ../plugins/TrackPlots.C+");
  gROOT->ProcessLine(".L ../plugins/VertexPlots.C+");
  gROOT->ProcessLine(".L ../plugins/EvtSelPlots.C+");
  
  gROOT->ProcessLine(".L ../plugins/GenPartPlots.C+"); 
  //gROOT->ProcessLine(".L ../plugins/HFPlots.C+");
  gROOT->ProcessLine(".L ../plugins/NCHEvtSelPlots.C+");
  gROOT->ProcessLine(".L ../plugins/NCHHFPlots.C+");
  gROOT->ProcessLine(".L ../plugins/NCHCentralPlots.C+");
  gROOT->ProcessLine(".L ../plugins/NCHDiffPlots.C+");   
  
  gROOT->ProcessLine(".L ../plugins/NCHptvsnchPlots.C+"); 

  gROOT->ProcessLine(".L ../plugins/Syst.C+");
  gROOT->ProcessLine(".L ../plugins/EfficiencyPlots.h+");
  
  gROOT->ProcessLine(".L ../plugins/TPlotterBase.C+");
  gROOT->ProcessLine(".L ../plugins/TPlotter.C+");
  
  gROOT->ProcessLine(".L ../plugins/TTProfile.C+");
  gROOT->ProcessLine(".L ../plugins/FBCorrel.C+");
  gROOT->ProcessLine(".L ../plugins/FBCorrelwCorr.C+");
  gROOT->ProcessLine(".L ../plugins/FBResults.C+");
  
  gROOT->ProcessLine(".L ../plugins/LinkDef.C+");
}
