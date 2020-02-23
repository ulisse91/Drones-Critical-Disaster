CXX = g++ -std=c++14
OPT = -O4 # -DNDEBUG
TARGET = main
TARGET_TEST = mainTest

OPTIONS = -lboost_program_options
# INCLUDEPATH = $(HOME)/NetworKit/include
PATHLIB = /usr/include/boost
SOURCES = graph.cpp simulator.cpp

main:
	$(CXX) -o $(TARGET) $(TARGET).cpp $(SOURCES) $(OPT) -L$(PATHLIB) $(OPTIONS)

test:
	$(CXX) -o $(TARGET_TEST) $(TARGET_TEST).cpp $(SOURCES) test.cpp $(OPT) -L$(PATHLIB) $(OPTIONS)

clean:
	rm -rf $(TARGET) $(TARGET_TEST)