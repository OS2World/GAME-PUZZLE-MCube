# makefile for mcube with gcc on *nix by Eric

CC=gcc
CPP=g++
LINK=g++
CFLAGS=-O2 -D__EMX__ -DOS2  -D__ST_MT_ERRNO__ -g -Wall
LFLAGS= -Zexe -Zmap -Zargs-wild -D__ST_MT_ERRNO__
RM=rm -f
INCLUDES=mcube.h config.h
OBJS=mcube.o main.o
LIBS=
DBGCFLAGS=
DBGLFLAGS=
DBGOBJS=mcube.o.dbg main.o.dbg
DBGLIBS=

all: build

debug: build-debug

build: $(OBJS)
	$(LINK) $(LFLAGS)  -o mcube  $(OBJS) $(LIBS)

build-debug: $(DBGOBJS)
	$(LINK) $(DBGLFLAGS)  -o mcube  $(OBJS) $(DBGLIBS)

clean:
	$(RM) $(OBJS)

main.o: main.cpp $(INCLUDES)
	$(CPP) $(CFLAGS) -c main.cpp

mcube.o: mcube.cpp $(INCLUDES)
	$(CPP) $(CFLAGS) -c mcube.cpp

main.o.dbg: main.cpp $(INCLUDES)
	$(CPP) $(DBGCFLAGS) -c main.cpp

mcube.o.dbg: mcube.cpp $(INCLUDES)
	$(CPP) $(DBGCFLAGS) -c mcube.cpp

dummy:
