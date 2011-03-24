makeLib(){
echo "Making Library"
g++ -shared -o libMySharedLib.so `root-config --ldflags` `root-config --cflags --glibs` eventdict.cxx $SRC

}

makeDic(){
echo "Making Dictionnary"
rootcint -f eventdict.cxx -c -p $LIBS LinkDef.h

}

makeCode(){
g++ hadd.cxx -o test `root-config --cflags --glibs` \
  -L/user/rougny/Ferenc_Tracking_bis/CMSSW_3_3_6_patch3/src/UAmulti/ChPartAna/plugins/ -lMySharedLib

g++ macro.C -o macro `root-config --cflags --glibs` \
  -L/user/rougny/Ferenc_Tracking_bis/CMSSW_3_3_6_patch3/src/UAmulti/ChPartAna/plugins/ -lMySharedLib
}

LIBS=`echo "\
MyPart.h \
MyBasePart.h \
MyEvtId.h \
MyL1Trig.h \
MyFwdGap.h \
MyHLTrig.h \
MyGenKin.h \
MyGenPart.h \
MyMITEvtSel.h \
MyTracks.h \
MyVertex.h \
MyBeamSpot.h \
TrackPlots.h \
VertexPlots.h \
EvtSelPlots.h \
TMean.h \
BasePlots.h \
MultiPlots.h \
GenMultiPlots.h \
MatrixPlots.h \
TMoments.h \
TPlotterBase.h \
TPlotter.h \
TTProfile.h \
TResponseMtx.h \
FBCorrel.h \
FBCorrelwCorr.h \
FBResults.h \
Syst.h \
TOperation.h \
"`

SRC=`echo "\
MyPart.C \
MyBasePart.C \
MyEvtId.C \
MyL1Trig.C \
MyFwdGap.C \
MyHLTrig.h \
MyGenKin.C \
MyGenPart.C \
MyMITEvtSel.C \
MyTracks.C \
MyVertex.C \
MyBeamSpot.C \
TrackPlots.C \
VertexPlots.C \
EvtSelPlots.C \
TMean.C \
BasePlots.C \
MultiPlots.C \
GenMultiPlots.C \
MatrixPlots.C \
TMoments.C \
TPlotterBase.C \
TPlotter.C \
TTProfile.C \
TResponseMtx.C \
FBCorrel.C \
FBCorrelwCorr.C \
FBResults.C \
Syst.C \
TOperation.h \
"`


###################
### READ option ###
###################

for xopt in $* ; do
  case $xopt in
    -dic)   makeDic  ; shift ;;
    -lib)   makeLib  ; shift ;;
    -hadd)  makeCode ; shift ;;
    *)  echo "Only -dic -lib -hadd in this order !" ;;
  esac
done

