#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char* argv[]) {
    int comm_sz, my_rank;
    long long int number_of_tosses, number_in_circle = 0, local_in_circle = 0;
    long long int toss;
    double x, y, distance_squared, pi_estimate;

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    if (my_rank == 0) {
        printf("Enter the number of tosses: ");
        scanf("%lld", &number_of_tosses);
    }

    // Broadcast the number of tosses to all processes
    MPI_Bcast(&number_of_tosses, 1, MPI_LONG_LONG_INT, 0, MPI_COMM_WORLD);

    long long int local_tosses = number_of_tosses / comm_sz;

    for (toss = 0; toss < local_tosses; toss++) {
        x = (double)rand() / RAND_MAX * 2 - 1;
        y = (double)rand() / RAND_MAX * 2 - 1;
        distance_squared = x * x + y * y;
        if (distance_squared <= 1) {
            local_in_circle++;
        }
    }

    // Reduce local results into global result at process 0
    MPI_Reduce(&local_in_circle, &number_in_circle, 1, MPI_LONG_LONG_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (my_rank == 0) {
        pi_estimate = 4 * (double)number_in_circle / (double)number_of_tosses;
        printf("Estimated value of Pi = %lf\n", pi_estimate);
    }

    MPI_Finalize();
    return 0;
}
