# MPIFarm
Is a project of render farm working with Open MPI.

## Requirements
Open MPI is only available on Linux.<br>
You will need to install C++ compiler and cmake.
```bash
sudo apt install g++ cmake
```
You will also need to install Open MPI components like this : 
```bash
sudo apt install libopenmpi-dev openmpi-bin
```

## Compiling
To compile, just type:
```bash
./compile
```

## Execute 
To start the program:
```bash
/usr/bin/mpirun -np 2 build/MPIFarm
```