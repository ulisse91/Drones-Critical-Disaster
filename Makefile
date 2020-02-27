# path #
SRC_PATH = src/core
SRC_PATH_TEST = tests
BUILD_PATH = build

# executable # 
TARGET = main
TARGET_TEST = mainTest

SOURCES = $(shell find $(SRC_PATH) -name '*.cpp')
SOURCES_TEST = $(shell find $(SRC_PATH_TEST) -name '*.cpp')

# flags #
COMPILE_FLAGS = g++ -std=c++11 -O4 # -DNDEBUG
OPTIONS = -lboost_program_options
PATHLIB = /usr/include/boost

.PHONY: dirs
dirs:
	@echo "Creating directories"
	@mkdir -p $(BUILD_PATH)

main: clean dirs
	$(COMPILE_FLAGS) -o $(BUILD_PATH)/$(TARGET) src/main.cpp $(SOURCES) -L$(PATHLIB) $(OPTIONS)

test: clean dirs
	$(COMPILE_FLAGS) -o $(BUILD_PATH)/$(TARGET_TEST) $(SOURCES) $(SOURCES_TEST) -L$(PATHLIB) $(OPTIONS)

.PHONY: clean
clean:
	@echo "Deleting $(BUILD_PATH) directory"
	@$(RM) -r $(BUILD_PATH)