#!/bin/bash

function check_command {
    if [ $2 -eq 0 ]; then
        echo -e "$1 [OK]"
    else
        echo -e "$1 [FAILED]"
        clean
        exit
    fi
}

function clean {
    echo "Forcing remotion of every data files"
    rm -rf data/output
    mkdir data/output
}

if [[ $# -ne 3 ]]; then
    echo "Usage: $0 <bool:make> <budget> <prob_value>"
    echo "<bool:make>:: 1 == make curr, 0 will skip the compilation"
    exit
fi

if [[ $1 -eq 1 ]]; then
    echo -e "Starting compilation ...\n"
    make curr
    check_command "\nCompilation" $?
fi

budget=$2
p=$3
main="./src/build/main"
pathoutput="data/output/"
pathbkp="data/bkp/"
baseseed=100000

echo "Starting simulations ..."
for nodes in {10..100..10}; do
    for drone in {2..6..2}; do
        echo "Simulation: -b $budget -d $drone -n $nodes -p $p"
        for seed in {0..20}; do 
            $main -b $budget -d $drone -n $nodes -p $p -s $(($baseseed + $seed)) >> ${pathoutput}"results"-b${budget}-d${drone}-n${nodes}-p${p}.txt
            # echo "$main -b $budget -d $drone -n $nodes -p $p -s $(($baseseed + $seed)) > ${pathoutput}-b${budget}-d${drone}-n${nodes}-p${p}.txt"
        done
    done
done 
echo -e "Simulation terminated\n"

echo -n "Starting analysis script ..."
python3 scripts/analyze_data.py ${p} # >> ${pathoutput}analysis-b${budget}-p${p}.output
check_command "" $?

echo -n "Starting compression files ..."
tar -czf ${pathbkp}simulations-b${budget}-p${p}.tar.gz ${pathoutput}
check_command "" $?

clean

echo "DONE"