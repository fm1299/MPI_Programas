#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int comm_sz, my_rank, local_value, received_value;
    int mask, partner, global_sum = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    local_value = my_rank;

    // Butterfly pattern for global sum
    for (mask = 1; mask < comm_sz; mask <<= 1) {
        partner = my_rank ^ mask;
        printf("partner val is %i\n",partner);
        if (partner < comm_sz) {
            MPI_Sendrecv(&local_value, 1, MPI_INT, partner, 0,
                         &received_value, 1, MPI_INT, partner, 0,
                         MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            local_value += received_value;
        }
    }

    if (my_rank == 0) {
        global_sum = local_value;
        printf("Global sum using butterfly communication is %d\n", global_sum);
    }

    MPI_Finalize();
    return 0;
}
