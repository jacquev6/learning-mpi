#include <stdio.h>

#include <mpi.h>


int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        for (int other_rank = 1; other_rank != size; ++other_rank) {
            MPI_Send("ok", 3, MPI_CHAR, other_rank, 0, MPI_COMM_WORLD);
        }
    } else {
        char buf[16];
        MPI_Recv(buf, sizeof(buf), MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("%i: %s\n", rank, buf);
    }

    MPI_Finalize();
}
