#!/bin/bash

# PREPARATION
echo -ne "\n\n\nSTARTING NEW BENCHMARK\n" >>benchmark.log
date -R >>benchmark.log
echo -ne "########################################\n\n" >>benchmark.log

echo -ne "\n\n\nSTARTING NEW BENCHMARK\n" >>benchmarkErrors.log
date -R >>benchmarkErrors.log
echo -ne "########################################\n\n" >>benchmarkErrors.log

rm benchmark.csv 2>/dev/null
rm benchmarkfull.csv 2>/dev/null

# SETTINGS

nbRepetitions=6
maxProcesses=24
clusterSize="mega" #mega by default

fill_character="#"

# DISPLAY

console_width=$(tput cols)
sectionTerminator=""
for ((i = 0; i < $console_width; i++)); do
    sectionTerminator+=$fill_character
done

echo -ne "$sectionTerminator\nBenchmarking MPIFarm\n$sectionTerminator\n"
echo "List of the image sets: "

# CREATE SET LIST

imageSets=(
    "imageSets/P170B328_ServieresV_L3_smallest.json"
    "imageSets/P170B328_ServieresV_L3_xxsmall.json"
    "imageSets/P170B328_ServieresV_L3_0001-0003.json"
    "imageSets/P170B328_ServieresV_L3_0001-0012.json"
    "imageSets/P170B328_ServieresV_L3_0001-0024.json"
    "imageSets/P170B328_ServieresV_L3_0001-0064.json"
    "imageSets/P170B328_ServieresV_L3_0001-0128.json"
    "imageSets/P170B328_ServieresV_L3_0001-0512.json"
    "imageSets/P170B328_ServieresV_L3_0001-1024.json"
    "imageSets/P170B328_ServieresV_L3_0001-2500.json"
)

setSizes=(
    1
    19
    3
    12
    24
    64
    128
    512
    1024
    2500
)

setLabels=(
    "1 images (9px)"
    "19 images (9px)"
    "3 images"
    "12 images"
    "24 images"
    "64 images"
    "128 images"
    "512 images"
    "1024 images"
    "2500 images"
)

# imageSets=(
#     "imageSets/P170B328_ServieresV_L3_smallest.json"
#     "imageSets/P170B328_ServieresV_L3_xxsmall.json"
# )

# setSizes=(
#     1
#     19
# )

# setLabels=(
#     "1 images (9px)"
#     "19 images (9px)"
# )

totalSets=0
for imageSet in "${imageSets[@]}"; do
    echo "- $imageSet"
    totalSets=$((totalSets + 1))
done

# PREPARING CSV FILE
echo -ne "\"Number of processes\"" >>benchmark.csv
echo -ne "\"Number of processes\"" >>benchmarkfull.csv
for label in "${setLabels[@]}"; do
    for ((repNb = 1; repNb < $nbRepetitions + 1; repNb++)); do
        echo -ne ",\"$label - round $repNb\"" >>benchmarkfull.csv
    done
    echo -ne ",\"$label\"" >>benchmark.csv
    echo -ne ",\"$label - average\"" >>benchmarkfull.csv
done

# Calculate number of runs
totalRuns=0
for ((nbProcesses = 1; nbProcesses < $maxProcesses + 1; nbProcesses++)); do
    for imageSet in "${imageSets[@]}"; do
        for ((repNb = 1; repNb < $nbRepetitions + 1; repNb++)); do
            totalRuns=$((totalRuns + 1))
        done
    done
done

# STARTING BENCHMARK
echo "Starting benchmark..."
runNb=1
for ((nbProcesses = 1; nbProcesses < $maxProcesses + 1; nbProcesses++)); do
    echo -ne "\n$nbProcesses" >>benchmark.csv
    echo -ne "\n$nbProcesses" >>benchmarkfull.csv
    setNb=1
    for imageSet in "${imageSets[@]}"; do
        avgTime=0
        for ((repNb = 1; repNb < $nbRepetitions + 1; repNb++)); do
            logLine="$(date +%T): $nbProcesses/$maxProcesses Processes - Set $setNb/$totalSets (rep $repNb/$nbRepetitions) | Run $runNb/$totalRuns - $(($runNb * 100 / $totalRuns))%"
            echo $logLine
            echo $logLine >>benchmark.log
            echo "\n\n$logLine : " >>benchmarkErrors.log
            ./start.sh $clusterSize $imageSet $nbProcesses --benchmark >>benchmark.log 2>>benchmarkErrors.log
            time=$(cat timeSession)
            echo -ne ",$time" >>benchmarkfull.csv

            #avgTime=$((avgTime + time / nbRepetitions))
            avgTime=$(awk -v avgTime="$avgTime" -v time="$time" -v nbRepetitions="$nbRepetitions" 'BEGIN { printf "%.10f\n", avgTime + time / nbRepetitions }')
            runNb=$((runNb + 1))
        done
        echo -ne ",$avgTime" >>benchmark.csv
        echo -ne ",$avgTime" >>benchmarkfull.csv
        setNb=$((setNb + 1))
    done
done
