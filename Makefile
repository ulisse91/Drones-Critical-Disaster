examples: Examples

curr: 
	$(MAKE) -C src

Examples:
	$(MAKE) -C examples

testAll:
	$(MAKE) run -C tests

clean:
	$(MAKE) clean -C src
	$(MAKE) clean -C examples
	$(MAKE) clean -C tests