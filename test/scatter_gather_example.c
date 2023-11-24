#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define ARRAY_SIZE 8

int main(int argc, char *argv[]) {
    int rank, size;
    int *send_data, *recv_data;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 2) {
        if (rank == 0) {
            printf("This example requires exactly 2 processes.\n");
        }
        MPI_Finalize();
        return 1;
    }

    // Allocate memory for the array
    send_data = (int*)malloc(sizeof(int) * ARRAY_SIZE);
    recv_data = (int*)malloc(sizeof(int) * ARRAY_SIZE / size);

    // Initialize the array in the root process
    if (rank == 0) {
        for (int i = 0; i < ARRAY_SIZE; ++i) {
            send_data[i] = i + 1;
        }
    }

    // Scatter the array from the root process
    MPI_Scatter(send_data, ARRAY_SIZE / size, MPI_INT, recv_data, ARRAY_SIZE / size, MPI_INT, 0, MPI_COMM_WORLD);

    // Perform computation on the received data (e.g., square each element)
    for (int i = 0; i < ARRAY_SIZE / size; ++i) {
        recv_data[i] = recv_data[i] * recv_data[i];
    }

    // Gather the results back to the root process
    MPI_Gather(recv_data, ARRAY_SIZE / size, MPI_INT, send_data, ARRAY_SIZE / size, MPI_INT, 0, MPI_COMM_WORLD);

    // Print the results in the root process
    if (rank == 0) {
        printf("Original array: ");
        for (int i = 0; i < ARRAY_SIZE; ++i) {
            printf("%d ", send_data[i]);
        }
        printf("\n");
    }

    // Clean up
    free(send_data);
    free(recv_data);
    MPI_Finalize();

    return 0;
}