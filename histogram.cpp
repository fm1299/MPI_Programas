#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void compute_histogram(int *data, int data_per_process, int *local_hist, int num_bins, int bin_size, int rank) {
    for (int i = 0; i < data_per_process; i++) {
        int bin_index = data[i] / bin_size;
        local_hist[bin_index]++;
    }
}

int main(int argc, char* argv[]) {
    int comm_sz, my_rank;
    int num_data = 600;
    int num_bins = 10;
    int bin_size = 10;
    int *data = NULL;

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    int data_per_process = num_data / comm_sz;
    int *local_data = (int*)malloc(data_per_process * sizeof(int));
    int *local_hist = (int*)calloc(num_bins, sizeof(int));


    if (my_rank == 0) {
        data = (int*)malloc(num_data * sizeof(int));
        for (int i = 0; i < num_data; i++) {
            data[i] = rand() % (num_bins * bin_size);
        }
        MPI_Scatter(data, data_per_process, MPI_INT, local_data, data_per_process, MPI_INT, 0, MPI_COMM_WORLD);
    } else {
        MPI_Scatter(NULL, data_per_process, MPI_INT, local_data, data_per_process, MPI_INT, 0, MPI_COMM_WORLD);
    }

    compute_histogram(local_data, data_per_process, local_hist, num_bins, bin_size, my_rank);

    int *global_hist = NULL;
    if (my_rank == 0) {
        global_hist = (int*)calloc(num_bins, sizeof(int));
    }
    MPI_Reduce(local_hist, global_hist, num_bins, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (my_rank == 0) {
        printf("Final histogram:\n");
        for (int i = 0; i < num_bins; i++) {
            printf("Bin %d: %d\n", i, global_hist[i]);
        }
        free(global_hist);
    }

    free(local_data);
    free(local_hist);
    if (my_rank == 0) free(data);

    MPI_Finalize();
    return 0;
}