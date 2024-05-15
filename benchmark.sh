#!/bin/bash

if [[ $1 == '' ]] ; then
    echo "Usage: ./benchmark.sh [program] <iterations>"
    echo "By default, iterations is 1. If provided, the executable will be"
    echo "executed for that many iterations and statistics reported for each"
    echo "iteration."
fi

if [[ $2 =~ ^[0-9]+$ ]] ; then
    iterations=$2
else
    iterations=1
fi

printf "Elapsed Time (ms)\tSystem Time\tUser Time\tCPU Usage\tMax Mem (kb)\tAvg Mem (kb)\n"
for i in $(seq 1 $iterations) ; do
    start_time=$(date +%s%3N)  # milliseconds since epoch
    $1  # Execute the program
    end_time=$(date +%s%3N)  # milliseconds since epoch

    elapsed_time=$((end_time - start_time))

    /usr/bin/time -f "$elapsed_time\t\t%S\t\t%U\t\t%P\t\t%M\t\t%K" $1
done
