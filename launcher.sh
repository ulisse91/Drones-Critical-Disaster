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
    make main
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
    for pv in {0..100..25}; do # {0..100..25}
        p=$(echo "scale=2; $pv/100" | bc -q)
        for nodes in {50..200..25}; do # {50..200..25}
            for drone in {5..20..5}; do # {5..20..5}
                for seed in {0..19}; do # {0..19}
                    $main -b $budget -d $drone -n $nodes -p $p -s $(($baseseed + $seed)) --distrib $dis >> ${pathoutput}"results"-b${budget}-d${drone}-n${nodes}-p${pv}_${dis}.txt
                    # echo "$main -b $budget -d $drone -n $nodes -p $p -s $(($baseseed + $seed)) --distrib $dis >> ${pathoutput}"results"-b${budget}-d${drone}-n${nodes}-p${pv}_${dis}.txt"
                done
                i=$(($i +1))
                echo -ne " $(echo "scale=0; $i*0.7" | bc -q)% \r"
            done
        done
    done
    
done
echo -e "Simulation terminated\n"

echo -n "Starting analysis script uniform ..."
python3 scripts/analyze_data.py "uniform" $budget
check_command "" $?

echo -n "Starting analysis script poisson ..."
python3 scripts/analyze_data.py "poisson" $budget
check_command "" $?

echo -n "Starting compression files ..."
timestamp=$(date "+%Y%m%d%H%M")
tar -czf ${pathbkp}simulations-b${budget}_${timestamp}.tar.gz ${pathoutput}
check_command "" $?

clean

echo "DONE"
