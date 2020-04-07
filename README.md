# Drones-Critical-Disaster

To run the tests: 
```bash
make testAll
```
Compile the Examples with: 
```bash
make examples
```
Then run the examples singularly from the examples/build directory

The main file is src/main.cpp
```bash
make curr
```

# TO_DO
Algorithms:

Features:
- fun: print solution in file
- fun: read solution from file
- seed in metric-k-center
- input from file

To-Fix / Important:
- generate graph with zipf distribution
- clean cycle no cycles with 0-0
- simulator save csv: number of total cycles, avg (and max) budget spent per cycle (do not include last cycle)
- check function finishing time G_max G_med G_min

Examples:

Tests:
- test obj func with sigma prime
- test check_cycle_sigma_prime_cycle