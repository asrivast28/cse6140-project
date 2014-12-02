CXX = g++
LINKER = g++
EXE	= tspsolver 

LIBS:= -lboost_filesystem\
       -lboost_program_options\
       -lboost_timer\
       -lboost_chrono\
       -lboost_system

CXXFLAGS = -Wall -std=c++0x -O3 -DNDEBUG 

SOURCES:= ProgramOptions.cpp\
          TSPInstance.cpp\
          MSTApproximation.cpp\
          TreeNode.cpp\
          BranchAndBound.cpp\
          GreedyHeuristic.cpp\
          GeneticAlgorithm.cpp\
          IteratedLocalSearch.cpp\
          driver.cpp

OBJECTS = $(patsubst %.cpp,%.o,$(SOURCES))

%.o: %.cpp 
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(EXE) : $(OBJECTS)
	$(LINKER) -o $(EXE) $(OBJECTS) $(LIBS)

all : $(OBJECTS) $(EXE)

clean:
	-rm -rf *.o tspsolver
