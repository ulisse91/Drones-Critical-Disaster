include ../Makefile.config

## Main application file
MAIN = main

CORE = $(shell find core -name '*.cpp')
SIM = $(shell find simulator -name '*.cpp')

all: $(MAIN)
run: mainRun

.PHONY: dirs
dirs:
	@echo "Creating directories"
	@mkdir -p $(BUILD_PATH)

$(MAIN): clean dirs
	$(COMPILE_FLAGS) -o $(BUILD_PATH)/$(MAIN) main.cpp $(CORE) $(SIM) -L$(PATHLIB) $(OPTIONS)

mainRun: $(MAIN)
	./build/$(MAIN)
	$(MAKE) clean

clean:
	@echo "Deleting $(BUILD_PATH) directory"
	rm -rf build