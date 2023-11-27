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

nbRepetitions=2 # The program will execute several times each operation to get an average value
minProcesses=2 # The maximum number of processes to benchmark
maxProcesses= 6 # The maximum number of processes to benchmark
clusterSize="single" # single | mega | custom: as described in ./start.sh

fill_character=" "

# DISPLAY

console_width=$(tput cols)
sectionTerminator=""
for ((i = 0; i < $console_width; i++)); do
    sectionTerminator+=$fill_character
done

echo -ne "$(tput setab 7)"
echo -ne "$(tput setaf 7)"
echo -ne "$sectionTerminator"
echo -ne "$(tput sgr0)"
echo -ne "\nBenchmarking MPIFarm\n"
echo -ne "$(tput setab 7)"
echo -ne "$(tput setaf 7)"
echo -ne "$sectionTerminator"
echo -ne "$(tput sgr0)"

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

# You can overwrite this variable to create your own benchmark tests
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

echo -ne "$(tput setaf 8)"
echo "List of the image sets: "

totalSets=0
for imageSet in "${imageSets[@]}"; do
    echo "- $imageSet"
    totalSets=$((totalSets + 1))
done
echo -ne "$(tput sgr0)"

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
totalProcesses=0
for ((nbProcesses = $minProcesses; nbProcesses < $maxProcesses + 1; nbProcesses++)); do
    for imageSet in "${imageSets[@]}"; do
        for ((repNb = 1; repNb < $nbRepetitions + 1; repNb++)); do
            totalRuns=$((totalRuns + 1))
        done
    done
    totalProcesses=$((totalProcesses + 1))
done

# STARTING BENCHMARK
echo -ne "$(tput setaf 3)Starting benchmark...\n"
echo -ne "$(tput sgr0)"
runNb=1
procNb=1
for ((nbProcesses = $minProcesses; nbProcesses < $maxProcesses + 1; nbProcesses++)); do
    echo -ne "\n$nbProcesses" >>benchmark.csv
    echo -ne "\n$nbProcesses" >>benchmarkfull.csv
    setNb=1
    for imageSet in "${imageSets[@]}"; do
        avgTime=0
        for ((repNb = 1; repNb < $nbRepetitions + 1; repNb++)); do
            logLine="$(date +%T): $nbProcesses Processes ($procNb/$totalProcesses) - Set $setNb/$totalSets (rep $repNb/$nbRepetitions) | Run $runNb/$totalRuns - $(($runNb * 100 / $totalRuns))%"
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
    procNb=$((procNb + 1))
done

echo -ne "$(tput setaf 10)$(tput bold)End of benchmark.\n"
echo -ne "$(tput sgr0)"
echo -ne "$(tput setaf 8)Benchmark results are in benchmark.csv and benchmarkfull.csv\n"
echo -ne "$(tput sgr0)"