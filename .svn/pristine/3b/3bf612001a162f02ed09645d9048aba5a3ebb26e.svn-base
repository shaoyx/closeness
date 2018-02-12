# Build targets
SimRank_O= core/DynamicGraph.cpp \

Dynamic_O= core/dynamicTSF.cpp \
		   core/srgs.cpp \
		   core/logmanager.cpp \
		   core/dynamicowgmanager.cpp \
		   core/mytime.cpp \
		   core/onewaygraph.cpp

USER_FLAGS=-Wno-unused-result -Wno-unused-but-set-variable -Wno-sign-compare
USER_LIBS=

# Compiler flags
CC  = gcc
CXX = g++
CFLAGS=$(USER_FLAGS) -O3 -fPIC -Wall -g -I. -I./include
CXXFLAGS=$(CFLAGS)
LDFLAGS=-lpthread

# The name of the excution that will be built
SimRank=DynamicGenerate

DynamicTest=dynamicTSF

all:
	$(CXX) $(CXXFLAGS) -pthread $(LDFLAGS) -o $(SimRank) $(SimRank_O) $(USER_LIBS)

tsf:
	$(CXX) $(CXXFLAGS) -pthread $(LDFLAGS) -o $(DynamicTest) $(Dynamic_O) $(USER_LIBS)
	
clean:
	rm -f $(SimRank) $(tsf)
	find . -name '*.o' -print | xargs rm -f
