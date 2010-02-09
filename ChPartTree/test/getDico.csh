#!/bin/bash

rm -fr *

wget http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/UAmulti/ChPartTree/interface/MyEvtId.h?view=co -O MyEvtId.h -o /dev/null
wget http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/UAmulti/ChPartTree/interface/MyGenKin.h?view=co -O MyGenKin.h -o /dev/null
wget http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/UAmulti/ChPartTree/interface/MyGenPart.h?view=co -O MyGenPart.h -o /dev/null
wget http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/UAmulti/ChPartTree/interface/MyL1Trig.h?view=co -O MyL1Trig.h -o /dev/null
wget http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/UAmulti/ChPartTree/interface/MyMITEvtSel.h?view=co -O MyMITEvtSel.h -o /dev/null
wget http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/UAmulti/ChPartTree/interface/MyPart.h?view=co -O MyPart.h -o /dev/null
wget http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/UAmulti/ChPartTree/interface/MyTracks.h?view=co -O MyTracks.h -o /dev/null
wget http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/UAmulti/ChPartTree/interface/MyVertex.h?view=co -O MyVertex.h -o /dev/null

echo '#include <vector>'>LinkDef.C
for file in `ls ./*.h`; do
  echo '#include "'$file'"'>>LinkDef.C
done

wget http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/UAmulti/ChPartTree/interface/LinkDef.h?view=co -O LinkDef.h -o /dev/null

cat LinkDef.h >> LinkDef.C
sed "s:CINT:MAKECINT:" < LinkDef.C > temp
mv temp LinkDef.C

wget http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/UAmulti/ChPartTree/src/MyEvtId.C?view=co -O MyEvtId.C -o /dev/null
wget http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/UAmulti/ChPartTree/src/MyGenKin.C?view=co -O MyGenKin.C -o /dev/null
wget http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/UAmulti/ChPartTree/src/MyGenPart.C?view=co -O MyGenPart.C -o /dev/null
wget http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/UAmulti/ChPartTree/src/MyL1Trig.C?view=co -O MyL1Trig.C -o /dev/null
wget http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/UAmulti/ChPartTree/src/MyMITEvtSel.C?view=co -O MyMITEvtSel.C -o /dev/null
wget http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/UAmulti/ChPartTree/src/MyPart.C?view=co -O MyPart.C -o /dev/null
wget http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/UAmulti/ChPartTree/src/MyTracks.C?view=co -O MyTracks.C -o /dev/null
wget http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/UAmulti/ChPartTree/src/MyVertex.C?view=co -O MyVertex.C -o /dev/null

for file in `ls .`; do
  sed "s:UAmulti/ChPartTree/interface/::"<$file> temp
  mv temp $file
done
