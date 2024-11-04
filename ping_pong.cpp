#include <stdio.h>
#include <mpi.h>
#include <time.h>

int main(int argc, char *argv[])
{
    int my_rank, n, count = 10000;
    double start, end, total_time;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    int ping_pong_data = 0;

    // Use MPI_Wtime
    if (my_rank == 0)
    {
        start = MPI_Wtime();
        for (n = 0; n < count; n++)
        {
            MPI_Send(&ping_pong_data, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
            MPI_Recv(&ping_pong_data, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &status);
        }
        end = MPI_Wtime();
        total_time = end - start;
        printf("Time taken using MPI_Wtime: %f seconds\n", total_time);
    }
    else if (my_rank == 1)
    {
        for (n = 0; n < count; n++)
        {
            MPI_Recv(&ping_pong_data, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
            MPI_Send(&ping_pong_data, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
    }

    // Use clock for timing
    if (my_rank == 0)
    {
        clock_t clock_start, clock_end;
        clock_start = clock();
        for (n = 0; n < count; n++)
        {
            MPI_Send(&ping_pong_data, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
            MPI_Recv(&ping_pong_data, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &status);
        }
        clock_end = clock();
        total_time = (double)(clock_end - clock_start) / CLOCKS_PER_SEC;
        printf("Time taken using clock(): %f seconds\n", total_time);
    }
    else if (my_rank == 1)
    {
        for (n = 0; n < count; n++)
        {
            MPI_Recv(&ping_pong_data, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
            MPI_Send(&ping_pong_data, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
    }

    MPI_Finalize();
    return 0;
}
