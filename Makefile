CXX = g++ -std=c++14
OPT = -O4 -DNDEBUG
TARGET = main

OPTIONS = -lboost_program_options
# INCLUDEPATH = $(HOME)/NetworKit/include
PATHLIB = /usr/include/boost
SOURCES = $(TARGET).cpp graph.cpp

main:
	$(CXX) -o $(TARGET) $(SOURCES) $(OPT) -L$(PATHLIB) $(OPTIONS)

clean:
	rm -rf $(TARGET)

# main:	$(CXX) -o $(TARGET) -I$(INCLUDEPATH) -L$(PATHLIB) $(SOURCES) $(OPTIONS)