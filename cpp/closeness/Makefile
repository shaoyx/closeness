# Build targets
SimRank_O= core/SRBenchmark.cpp \
		   core/Bfs.cpp\
           core/spt.cpp \
		   core/adaptiveheap.cpp \
           core/mytime.cpp \
		   core/Sketch.cpp \
		   core/DeltaPFS.cpp \
		   core/Kazuya.cpp \

USER_FLAGS=-Wno-unused-result -Wno-unused-but-set-variable -Wno-sign-compare
USER_LIBS=

# Compiler flags
CC  = gcc
CXX = g++
CFLAGS=$(USER_FLAGS) -O3 -fPIC -Wall -g -I. -I./include
CXXFLAGS=$(CFLAGS)
LDFLAGS=-lpthread

# The name of the excution that will be built
SimRank=CCbenchmark

all:
	$(CXX) $(CXXFLAGS) -pthread $(LDFLAGS) -o $(SimRank) $(SimRank_O) $(USER_LIBS)
	
clean:
	rm -f $(SimRank)
	find . -name '*.o' -print | xargs rm -f
