CXX=g++
CXXFLAGS?=-Wall -pedantic -g -O0 -std=c++11
OUTFILES=MancalaEngine2

all: $(OUTFILES)

MancalaEngine2: MancalaEngine2.cpp Mancala2.cpp Mancala2.h
	$(CXX) $(CXXFLAGS) -o MancalaEngine2 MancalaEngine2.cpp Mancala2.cpp

clean:
	$(RM) $(OUTFILES) *.o
