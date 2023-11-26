imageSets=(
    "P170B328_ServieresV_L3_smallest.json"
    "P170B328_ServieresV_L3_xxsmall.json"
    "P170B328_ServieresV_L3_0001-0012.json"
    "P170B328_ServieresV_L3_0001-0024.json"
    "P170B328_ServieresV_L3_0001-0064.json"
    "P170B328_ServieresV_L3_0001-0128.json"
    "P170B328_ServieresV_L3_0001-0512.json"
    "P170B328_ServieresV_L3_0001-1024.json"
    "P170B328_ServieresV_L3_0001-2500.json"
)

for element in "${imageSets[@]}"; do
    echo "$element"
done
