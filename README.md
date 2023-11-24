# MPIFarm
Is a project of render farm working with Open MPI.

## Requirements
Open MPI is only available on Linux.<br>
You will need to install C++ compiler and cmake.
```bash
# Debian/Ubuntu
sudo apt install g++ cmake
# Arch Linux
sudo pacman -Sy g++ cmake
```
You will also need to install Open MPI and OpenCV components like this : 
```bash
# Debian/Ubuntu
sudo apt install libopenmpi-dev openmpi-bin libopencv-dev
# Arch Linux
sudo pacman -Sy openmpi opencv vtk
```

## Compiling
To compile, just type:
```bash
./cmakecompile
```

## Configure

### On master/local computer

#### Generate SSH Key Pair
```bash
ssh-keygen -t rsa -b 4096
```
> it might be created in `~/.ssh/id_rsa`

#### Copy Public Key to the Remote Machine
```bash
ssh-copy-id username@remote-machine
```
> it might be copied in `~/.ssh/authorized_keys`

### On remote machine

#### Modify SSH Configuration (if necessary)

```bash
sudo nvim /etc/ssh/sshd_config
```
You can also use `nano`` instead of nvim :p

```
PubkeyAuthentication yes
PasswordAuthentication no
ChallengeResponseAuthentication no
```

```bash
sudo systemctl restart ssh
```

#### Start SSH Agent for the User (optionnal ?)

```bash
eval "$(ssh-agent -s)"
ssh-add ~/.ssh/id_rsa
```

### Enjoy !
Now, you should be able to connect in ssh to the distant machine without password.

## Execute 
To start the program:
```bash
/usr/bin/mpirun -np 2 build/MPIFarm P170B328_ServieresV_L3_small.json
# or
mpirun -hostfile hostfile -np 2 build/MPIFarm P170B328_ServieresV_L3_small.json
```
If you have an `Invalid MIT-MAGIC-COOKIE-1 key6533927` error : 
```bash
ssh -X user@remote_machine
# and/or
xauth generate :0 . trusted # or :1, check echo $DISPLAY
```
If it is still here but cause no problem, just hide it with:
```bash
mpirun -hostfile hostfile -np 2 build/MPIFarm P170B328_ServieresV_L3_small.json 2>/dev/null
```

To compile and execute some tests, use
```bash
mpicxx -o scatterTest.o scatterTest.cpp
mpiexec -hostfile hostfile -np 5 ./scatterTest.o
```