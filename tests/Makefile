include ../Makefile.config

MAIN = mainTest
SRC_PATH = "../src/"
OBJ += test.o
$(DEPS) += test.h

%.o: %.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) -c -o $@ $< $(LDFLAGS)

$(MAIN): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(BUILD_PATH)/$@ $^ $(LDFLAGS)

run: dirs $(MAIN)
	./../build/$(MAIN)

.PHONY: dirs test run
dirs:
	@echo "Creating $(BUILD_PATH) directory"
	@- mkdir -p $(BUILD_PATH)
