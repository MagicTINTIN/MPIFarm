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
# or
mpirun -hostfile hostfile -np 6 build/MPIFarm 
```
If you have an `Invalid MIT-MAGIC-COOKIE-1 key6533927` error : 
```bash
ssh -X user@remote_machine
# and/or
xauth generate :0 . trusted # or :1, check echo $DISPLAY
```

To compile and execute some tests, use
```bash
mpicxx -o scatterTest.o scatterTest.cpp
mpiexec -hostfile hostfile -np 5 ./scatterTest.o
```