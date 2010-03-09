#Makefile

#definition of the shell
SHELL = /bin/bash

#setting macros
COMPILER = g++ -g
LIBDIR = /home/lceard/MC_from_Xavier/HWWGenPart_lceard_GenLep
CFLAGS = $(shell root-config --cflags)
LDFLAGS = $(shell root-config --ldflags)
LIBS   = $(shell root-config --libs) -lThread -lMinuit -lHtml -lVMC -lEG -lGeom
ROOT = root -b -q
ROOTLIBDIR = /usr/lib/root

## Code dependencies ----------- Edit list below ----------------------

PLUGINDIR  = ../plugins/
PLUGINNAME = EvtSelPlots GenMultiPlots LinkDef MatrixPlots MultiPlots MyBeamSpot MyEvtId MyGenKin MyGenPart MyL1Trig MyMITEvtSel MyPart MyTracks MyVertex TrackPlots VertexPlots TMean 

PLUGINSRC  = $(addprefix $(PLUGINDIR), $(PLUGINNAME))
PLUGINSRC := $(addsuffix .C , $(PLUGINSRC) )
PLUGINOBJ  = $(addprefix $(PLUGINDIR), $(PLUGINNAME))
PLUGINOBJ := $(addsuffix .o , $(PLUGINOBJ) )
PLUGIDICO  = $(addprefix $(PLUGINDIR), $(PLUGINNAME))
PLUGIDICO := $(addsuffix _C.so , $(PLUGIDICO) )

OBJDIR  = ./
OBJNAME = collectionPlotter 

OBJSRC  = $(addprefix $(OBJDIR), $(OBJNAME))
OBJSRC := $(addsuffix .C , $(OBJSRC) )
OBJOBJ  = $(addprefix $(OBJDIR), $(OBJNAME))
OBJOBJ := $(addsuffix .o , $(OBJOBJ) )

MAINSRC = mcollectionPlotter.C

#suffix list
.SUFFIXES: .C .o

# ------------- What2do ---------------------------------------------

all: dependencies collectionPlotter

cplot: collectionPlotter

dependencies: ${OBJOBJ} ${PLUGINOBJ} dico
	echo ${OBJNAME}  ; \
	echo ${PLUGINOBJ}

collectionPlotter: 
	${COMPILER} mcollectionPlotter.C -o $@.exe  \
	${OBJOBJ} ${PLUGINOBJ} ${PLUGIDICO} \
	${CFLAGS} ${LIBS}

# ------------- Stuff you shouldn't have to change ------------------



.C.o : 
	${COMPILER} ${CFLAGS} -I$(PLUGINDIR) -c -o $*.o $<

dico:
	${ROOT} BuildLibDico.C+

clean:
	rm *.o *.so *.d ; \
	rm $(PLUGINDIR)/*.o $(PLUGINDIR)/*.so $(PLUGINDIR)/*.d
