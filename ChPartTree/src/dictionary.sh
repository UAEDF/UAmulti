#!/bin/bash
rootcint -f eventdict.cc -c -I${PWD}/../../.. \
         -p \
	    UAmulti/ChPartTree/interface/MyGenKin.h \
    	    UAmulti/ChPartTree/interface/MyEvtId.h \
    	    UAmulti/ChPartTree/interface/MyL1Trig.h \
    	    UAmulti/ChPartTree/interface/MyHLTrig.h \
    	    UAmulti/ChPartTree/interface/MyBeamSpot.h \
    	    UAmulti/ChPartTree/interface/MyVertex.h \
    	    UAmulti/ChPartTree/interface/MyPart.h \
    	    UAmulti/ChPartTree/interface/MyTracks.h \
    	    UAmulti/ChPartTree/interface/MyGenPart.h \
    	    UAmulti/ChPartTree/interface/MyMITEvtSel.h \
    	    UAmulti/ChPartTree/interface/MyFwdGap.h \
            UAmulti/ChPartTree/interface/LinkDef.h
