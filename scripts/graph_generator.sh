#!/bin/bash

# Note that this script uses the main cpp code
make main

budget=50
main="./build/main"
pathoutput="data/graph/generated"
baseseed=100000

mkdir -p $pathoutput

for dis in uniform poisson; do
    for pv in {0..0}; do # {0..75..25}
        p=$(echo "scale=2; $pv/100" | bc -q)
        for nodes in {50..200..25}; do # {50..200..25}
            for seed in {0..19}; do # {0..19}
                $main -b $budget -q 2 -n $nodes -p $p -s $(($baseseed + $seed)) --distrib $dis --simulation "generate-graph"
                if ! [ $? -eq 0 ]; then
                    echo -e "\nFailed Execution [FAILED]"
                    echo -e "$main -b $budget -q 2 -n $nodes -p $p -s $(($baseseed + $seed)) --distrib $dis --simulation \"generate-graph\"\n"
                    exit
                fi
            done
        done
    done
done