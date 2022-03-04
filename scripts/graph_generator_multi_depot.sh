#!/bin/bash

# Note that this script uses the main cpp code
make main

budget=60
main="./build/main"
pathoutput="data/graph/generated"
baseseed=100000

mkdir -p $pathoutput
p=0

for nodes in {10,20,30,50,75,100,125,150,175,200}; do # {50..200..25}
    for depots in {2,3,4}; do
        for seed in {0..19}; do # {0..19}
            $main -b $budget -q 2 -n $nodes -p $p -s $(($baseseed + $seed)) -d $depots --simulation "generate-graph-multi-depot"
            if ! [ $? -eq 0 ]; then
                echo -e "\nFailed Execution [FAILED]"
                echo -e "$main -b $budget -q 2 -n $nodes -p $p -s $(($baseseed + $seed)) --distrib $dis --simulation \"generate-graph-multi-depot\"\n"
                exit
            fi
        done
    done
done