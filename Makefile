examples: Examples

test: TestAll

Examples:
	$(MAKE) -C examples

TestAll:
	$(MAKE) run -C tests

clean:
	$(MAKE) clean -C build