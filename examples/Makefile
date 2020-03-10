include ../Makefile.config

all: MakeAll

.PHONY: dirs
dirs:
	@echo "Creating directories"
	@mkdir -p $(BUILD_PATH)

MakeAll: clean dirs
	$(MAKE) -C top

clean:
	@echo "Deleting $(BUILD_PATH) directory"
	rm -rf build