CXX=g++
CC=gcc
CFLAGS=-O2 -Wall
LDFLAGS=-Llib
PRFFLAGS=-lProof
THRDFLAGS=-lThread
INS=-I$(ROOTSYS)/include
INS2=-I$(ROOFITSYS)/include
INS3=-I$(ROOSTATSYS)/include
INSS=-I./include
INSSS=-I.

LD1=-L$(ROOFITSYS)/lib

CFLAGS += `root-config --cflags`
LIBS += `root-config --ldflags --glibs`
LIBS += -L$(ROOFITSYS)/lib -lRooFit -lRooFitCore -lRooStats
#LDFLAGS=`root-config –-ldflags –glibs` -lRooFit –lRooFitCore -lMinuit

LDa=-lRooFitCore
LDb=-lRooFit

OBJ=pull.o

.PHONY: clean all main test

all: pull

pull: pull.o
	$(CXX) -o pull.exe $(OBJ) $(LIBS)

clean:
	@rm *.o *.exe

##############RULES##############
.cc.o:
	$(CXX) $(CFLAGS) $(INS) $(INS2) $(INS3) $(INSS) ${INSSS} -c $<
.cpp.o:
	$(CXX) $(CFLAGS) $(INS) -c $<

