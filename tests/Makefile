include ../Makefile.config

## Main application file
MAIN = mainTest

SOURCES_TEST = $(shell find -name '*.cpp')
CORE = $(shell find ../src/core -name '*.cpp')

all: $(MAIN)
run: test

.PHONY: dirs
dirs:
	@echo "Creating directories"
	@mkdir -p $(BUILD_PATH)

$(MAIN): clean dirs
	$(COMPILE_FLAGS) -o $(BUILD_PATH)/$(MAIN) $(CORE) $(SOURCES_TEST) -L$(PATHLIB) $(OPTIONS)

test: $(MAIN)
	./build/$(MAIN)

clean:
	@echo "Deleting $(BUILD_PATH) directory"
	rm -rf build