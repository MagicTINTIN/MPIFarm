if [ $# != 2 && $# != 3 ]; then

    echo -ne "$(tput setaf 9)"
    echo "ERROR: ./start.sh requires 2 arguments : cluster_type set_of_images.json"
    echo -ne "$(tput sgr0)"
    echo "       cluster_type can be : single mega custom"
    echo "       An optionnal last argument can be a number, the number of processes to spawn"
    exit 1
fi

processToSpawn=1

if [[ $1 == "single" ]]; then
    processToSpawn=1
elif [[ $1 == "mega" ]]; then
    processToSpawn=12
elif [[ $1 == "custom" ]]; then
    processToSpawn=8
else
    echo -ne "$(tput setaf 9)"
    echo "ERROR: cluster_type not recognized"
    echo -ne "$(tput sgr0)"
    echo "       cluster_type can be : single mega custom"
    exit 1
fi

if [[ $# == 3 ]]; then
    processToSpawn=$3
fi

echo -ne "$(tput setaf 2)"
echo "Starting $1 ($processToSpawn processes)"
echo -ne "$(tput sgr0)"
mpirun -hostfile h_$1.cfg -np build/MPIFarm $processToSpawn $2
