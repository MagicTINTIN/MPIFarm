#include <mpi.h>
#include <iostream>
#include <vector>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    std::cout << std::endl;
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int arraySize = 25;

    if (size != 5) {
        if (rank == 0) {
            std::cerr << "This example requires exactly 5 processes." << std::endl;
        }
        MPI_Finalize();
        return 1;
    }

    if (rank == 0) {
        // The root process initializes the array
        std::vector<int> originalArray(arraySize);
        for (int i = 0; i < arraySize; ++i) {
            originalArray[i] = i;
        }

        // Scatter the array to all processes
        std::vector<int> localArray(arraySize / size);
        MPI_Scatter(originalArray.data(), arraySize / size, MPI_INT, localArray.data(), arraySize / size, MPI_INT, 0, MPI_COMM_WORLD);

        // The root process prints the local portion it has
        std::cout << "Root (rank 0) has local array: ";
        for (int i : localArray) {
            std::cout << i << " ";
        }
        std::cout << std::endl;
    } else {
        // Other processes receive their local portion
        std::vector<int> localArray(arraySize / size);
        MPI_Scatter(nullptr, arraySize / size, MPI_INT, localArray.data(), arraySize / size, MPI_INT, 0, MPI_COMM_WORLD);

        // Each non-root process prints its local portion
        std::cout << "Process " << rank << " has local array: ";
        for (int i : localArray) {
            std::cout << i << " ";
        }
        std::cout << std::endl;
    }

    MPI_Finalize();
    return 0;
}