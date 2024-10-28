#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int comm_sz, my_rank, local_value, received_value, step;
    int global_sum = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    // Each process starts with a value equal to its rank
    local_value = my_rank;

    for (step = 1; step < comm_sz; step *= 2) {
        if (my_rank % (2 * step) == 0) {
            // Receive from the corresponding process
            if (my_rank + step < comm_sz) {
                MPI_Recv(&received_value, 1, MPI_INT, my_rank + step, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                local_value += received_value;
            }
        } else {
            // Send and break the loop
            MPI_Send(&local_value, 1, MPI_INT, my_rank - step, 0, MPI_COMM_WORLD);
            break;
        }
    }

    if (my_rank == 0) {
        global_sum = local_value;
        printf("Global sum is %d\n", global_sum);
    }

    MPI_Finalize();
    return 0;
}
