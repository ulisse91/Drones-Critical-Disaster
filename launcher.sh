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

if [[ $# -ne 2 ]]; then
    echo "Usage: $0 <bool:make> <budget>"
    echo "<bool:make>:: 1 == make curr, 0 will skip the compilation"
    exit
fi

if [[ $1 -eq 1 ]]; then
    echo -e "Starting compilation ...\n"
    make curr
    check_command "\nCompilation" $?
fi

budget=$2
main="./src/build/main"
pathoutput="data/output/"
pathbkp="data/bkp/"
baseseed=100000

echo "Starting simulations ... "
i=0
for dis in uniform poisson; do
    for pv in {0..100..25}; do
        p=$(echo "scale=2; $pv/100" | bc -q)
        for nodes in {10..100..10}; do # 100
            for drone in {2..6..2}; do # 6
                for seed in {0..19}; do # 19
                    # $main -b $budget -d $drone -n $nodes -p $p -s $(($baseseed + $seed)) --distrib $dis >> ${pathoutput}"results"-b${budget}-d${drone}-n${nodes}-p${pv}_${dis}.txt
                    echo "$main -b $budget -d $drone -n $nodes -p $p -s $(($baseseed + $seed)) --distrib $dis >> ${pathoutput}"results"-b${budget}-d${drone}-n${nodes}-p${pv}_${dis}.txt"
                    i=$(($i +1))
                done
                echo -ne " $(( $i/60 ))% \r"
            done
        done
    done
    
done
echo -e "Simulation terminated\n"

echo -n "Starting analysis script uniform ..."
python3 scripts/analyze_data.py "uniform"
check_command "" $?

echo -n "Starting analysis script poisson ..."
python3 scripts/analyze_data.py "poisson"
check_command "" $?

echo -n "Starting compression files ..."
timestamp=$(date "+%Y%m%d%H%M")
tar -czf ${pathbkp}simulations-b${budget}_${timestamp}.tar.gz ${pathoutput}
check_command "" $?

clean

echo "DONE"