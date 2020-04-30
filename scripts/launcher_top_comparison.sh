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

if [[ $# -ne 2 ]]; then
    echo "Usage: $0 <bool:make> <budget>"
    echo "<bool:make>:: 1 == make curr, 0 will skip the compilation"
    exit
fi

if [[ "$1" -eq 1 ]]; then
    echo -e "Starting compilation ...\n"
    make main
    check_command "\nCompilation" $?
fi

if [[ "$2" -le 0 ]]; then
    echo -e "Budget leq zero\n"
    exit
fi

budget="$2"
main="./src/build/main"
pathoutput="data/output/top_comparison_log.txt"
baseseed=100000
drone=2

echo "Starting simulations ... "
i=0
for dis in uniform poisson; do
    for pv in {0..100..25}; do # {0..100..25}
        p=$(echo "scale=2; $pv/100" | bc -q)
        for nodes in {5..30}; do # {50..200..25}
            for seed in {0..5}; do # {0..19}
                $main -b $budget -d $drone -n $nodes -p $p -s $(($baseseed + $seed)) --distrib $dis >> ${pathoutput}          
                # echo $main -b $budget -d $drone -n $nodes -p $p -s $(($baseseed + $seed)) --distrib $dis
            done
            i=$(($i +1))
            echo -ne " $(echo "scale=0; $i*0.7" | bc -q)% \r"
        done
    done
    
done
echo -e "Simulation terminated\n"