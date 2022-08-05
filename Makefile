CC=gcc
CXX=g++
RM=rm -f
CPPFLAGS=-O2 -std=c++17
LDFLAGS=
LDLIBS=-lsfml-graphics -lsfml-window -lsfml-system

SRCS=cube.cc support.cc
OBJS=$(subst .cc,.o,$(SRCS))

all: cube

cube: $(OBJS)
	$(CXX) $(LDFLAGS) -o cube $(OBJS) $(LDLIBS)

cube.o: cube.cc support.hh

support.o: support.hh support.cc

clean:
	$(RM) $(OBJS)

distclean: clean
	$(RM) cube
