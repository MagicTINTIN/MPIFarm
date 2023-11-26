#!/bin/bash

# PREPARATION
rm benchmark.csv

# SETTINGS

nbRepetitions=4
maxProcesses=40
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

imageSets=(
    "imageSets/P170B328_ServieresV_L3_smallest.json"
    "imageSets/P170B328_ServieresV_L3_xxsmall.json"
)

setSizes=(
    1
    19
)

setLabels=(
    "1 images (9px)"
    "19 images (9px)"
)

for element in "${imageSets[@]}"; do
    echo "- $element"
done

# PREPARING CSV FILE

# STARTING BENCHMARK

for ((nbProcesses = 1; nbProcesses < $maxProcesses; nbProcesses++)); do
    for element in "${imageSets[@]}"; do
        echo "- $element ($nbProcesses)"
    done
done
