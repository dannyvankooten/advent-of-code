#!/usr/bin/env bash 

TOTAL=0
BENCHMARK_FILE="/tmp/benchmark.json"

# measure Python start-up cost
hyperfine 'python -c "1"' --warmup 10 --runs 10 --export-json $BENCHMARK_FILE --style none 2> /dev/null
PYTHON_STARTUP_COST=$(echo "$(cat $BENCHMARK_FILE | jq '.results[0].mean') * 0.80" | bc)

# run each day
for DIR in */; do
	cd $DIR 
	hyperfine 'python main.py' --warmup 3 --runs 3 --export-json $BENCHMARK_FILE --style none 2> /dev/null
    ELAPSED_TIME=$(cat $BENCHMARK_FILE | jq '.results[0].mean')
    ELAPSED_TIME=$(echo "$ELAPSED_TIME - $PYTHON_STARTUP_COST" | bc)
    TOTAL=$(echo "$TOTAL + $ELAPSED_TIME" | bc)
    printf "%-30s %.4fs\n" ${DIR::-1} $ELAPSED_TIME
	cd .. 
done

printf "%-30s %.4fs\n" "Total runtime:" $TOTAL
