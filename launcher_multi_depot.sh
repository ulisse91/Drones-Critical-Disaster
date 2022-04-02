#!/bin/bash

function clean {
    echo "Forcing remotion of every data files"
    rm -rf data/output
    mkdir data/output
}

function check_command {
    if [ $2 -eq 0 ]; then
        echo -e "$1 [OK]"
    else
        echo -e "$1 [FAILED]"
        clean
        exit
    fi
}

function run_simulation {
    name=$1[@]
    depots=$2
    array=("${!name}")
    main="./build/main"
    baseseed=100000
    echo "Starting simulations d $1... "
    i=0
    for val in "${array[@]}"; do
        for nodes in {10,20,30,40,50,75,100,125,150,175,200}; do #
            for seed in {0..19}; do
                timestamp=$(date +%s)
                $main --simulation "multi-depot" -f "data/graph/generated/graph_n"$nodes"_d"$depots"_s"$(($baseseed + $seed))".csv" -u "data/graph/generated/"$val >> ${pathoutput}"results_"$timestamp.txt    
                if ! [ $? -eq 0 ]; then
                    echo -e "\nFailed Execution [FAILED]"
                    echo -e "$main --simulation "multi-depot" -f "data/graph/generated/graph_n"$nodes"_d"$depots"_s"$(($baseseed + $seed))".csv" -u "data/graph/generated/"$val>> ${pathoutput}"results_"$timestamp.txt"
                    clean
                    exit
                fi
            done
            i=$(($i +1))
            echo -ne " $(echo "scale=0; $i*2.27" | bc -q)% \r"
        done
    done
    echo -e "[TERMINATED]\n"
}

function run_simulation_wadd {
    name=$1[@]
    depots=$2
    array=("${!name}")
    main="./build/main"
    baseseed=100000
    echo "Starting simulations wadd d $1... "
    i=0
    for val in "${array[@]}"; do
        for nodes in {10,20,30,40,50,75,100,125,150,175,200}; do #
            for seed in {0..19}; do
                timestamp=$(date +%s)
                $main --simulation "multi-depot" -f "data/graph/generated/graph_n"$nodes"_d"$depots"_s"$(($baseseed + $seed))"_wadd"5".csv" -u "data/graph/generated/"$val >> ${pathoutput}"results_"$timestamp.txt
                if ! [ $? -eq 0 ]; then
                    echo -e "\nFailed Execution [FAILED]"
                    echo -e "$main --simulation "multi-depot" -f "data/graph/generated/graph_n"$nodes"_d"$depots"_s"$(($baseseed + $seed))"_wadd"5".csv" -u "data/graph/generated/"$val>> ${pathoutput}"results_"$timestamp.txt"
                    clean
                    exit
                fi
            done
            i=$(($i +1))
            echo -ne " $(echo "scale=0; $i*2.27" | bc -q)% \r"
        done
    done
    echo -e "[TERMINATED]\n"
}

if [[ $# -ne 1 ]]; then
    echo "Usage: <bool:make>:: 1 == make curr, 0 will skip the compilation"
    exit
fi

if [[ "$1" -eq 1 ]]; then
    echo -e "Starting compilation ...\n"
    make main
    check_command "\nCompilation" $?
fi

pathoutput="data/output/"
pathbkp="data/bkp/"
pathplots="data/plots"

mkdir -p data/bkp/ data/plots data/output

declare -a StringArray_2=("drones_q2_d2.csv" "drones_q4_d2.csv" "drones_q6_d2.csv" "drones_q9_d2.csv")
run_simulation StringArray_2 2
run_simulation_wadd StringArray_2 2

# declare -a StringArray_3=("drones_q3_d3.csv" "drones_q6_d3.csv" "drones_q9_d3.csv" "drones_q12_d3.csv")
# run_simulation StringArray_3 3
# run_simulation_wadd StringArray_3 3

# declare -a StringArray_4=("drones_q4_d4.csv" "drones_q8_d4.csv" "drones_q12_d4.csv" "drones_q15_d4.csv")
# run_simulation StringArray_4 4
# run_simulation_wadd StringArray_4 4

echo -n "Starting analysis script uniform ..."
python3 scripts/analyze_data_multi_depot.py
check_command "" $?

echo -n "Starting compression files ..."
timestamp=$(date "+%Y%m%d%H%M")
tar -czf ${pathbkp}simulations_${timestamp}.tar.gz ${pathoutput}
check_command "" $?

clean $pathoutput

echo "DONE"
