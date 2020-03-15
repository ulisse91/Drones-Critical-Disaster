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

If you want to do tests during developing phase you can use
```bash
make curr
```
The main file is src/main.cpp
This file should remain empty otherwise

# TO_DO
Algorithms:
- alg: KIM
- alg: top primo step + primbased resto
- alg nel secondo scenario:
-- inserire step quando si è scelto ciclo per ogni drone
-- aggiornare grafi (i.e., non eliminare tutti i nodi) eliminando solo quelli effettivamente visitati e/o porzioni

Features:
- fun: print graph to file
- fun: print solution in file
- fun: read solution from file

To-Fix / Important:
- dividere due pesi (secondo peso con probabilità)
- porzioni di nodi
- fun: obj func completion time
- ricontrollare costi funzioni obiettivo!!!

Examples: