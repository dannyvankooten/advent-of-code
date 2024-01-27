#!/usr/bin/env bash 

TOTAL=0
BENCHMARK_FILE="/tmp/benchmark.json"

# run each day
for DIR in */; do
	cd $DIR 

    if [[ -e "main.rs" ]]; then
        rustc -O main.rs 
        hyperfine './main' --warmup 5 --runs 5 --export-json $BENCHMARK_FILE --style none 2> /dev/null
    elif [[ -x "main.py" ]]; then 
        hyperfine './main.py' --warmup 5 --runs 5 --export-json $BENCHMARK_FILE --style none 2> /dev/null
    else 
        hyperfine 'python main.py' --warmup 5 --runs 5 --export-json $BENCHMARK_FILE --style none 2> /dev/null
    fi
    ELAPSED_TIME=$(cat $BENCHMARK_FILE | jq '.results[0].min')
    TOTAL=$(echo "$TOTAL + $ELAPSED_TIME" | bc)
    printf "%-30s %.4fs\n" ${DIR::-1} $ELAPSED_TIME
	cd .. 
done

printf "%-30s %.4fs\n" "Total runtime:" $TOTAL
