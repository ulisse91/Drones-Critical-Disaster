# Drones-Critical-Disaster

To run the tests: 
```bash
make TestAll
```
Compile the Examples with: 
```bash
make examples
```
Then run the examples singularly from the examples/build directory

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
- funzione su grafo che spalma peso su archi
- dividere due pesi (secondo peso con probabilità)
- porzioni di nodi
- fun: distw che comprende peso nodo
- graph random generator include seed (possibly from input)
- fun: obj func completion time

Examples: