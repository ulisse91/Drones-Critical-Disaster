include ../../Makefile.config

## Main application file
MAIN = example_top

SOURCES_EX = $(shell find -name '*.cpp')
CORE = $(shell find ../../src/core -name '*.cpp')
SIM = $(shell find ../../src/simulator -name '*.cpp')

all: $(MAIN)

$(MAIN):
	$(COMPILE_FLAGS) -o ../$(BUILD_PATH)/$(MAIN) $(CORE) $(SIM) $(SOURCES_EX) -L$(PATHLIB) $(OPTIONS)