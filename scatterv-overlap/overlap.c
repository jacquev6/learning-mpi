#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <mpi.h>


int main(int argc, char **argv) {
  MPI_Init(&argc, &argv);

  int mpi_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);

  int mpi_size;
  MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);
  assert(mpi_size == 4);

  char* sendbuf = 0;
  if (mpi_rank == 0) {
    const int sendbuf_size = 20;
    sendbuf = malloc(sendbuf_size);
    strncpy(sendbuf, "abcdefghijklmnopqrst", sendbuf_size);
  }

  const int sendcounts[4] = {14, 10, 12, 8};
  const int displs[4] = {0, 4, 6, 10};

  const int recvcount = sendcounts[mpi_rank];
  char* const recvbuf = malloc(recvcount);

  MPI_Scatterv(
    sendbuf, sendcounts, displs, MPI_CHAR,
    recvbuf, recvcount, MPI_CHAR,
    0, MPI_COMM_WORLD);

  switch (mpi_rank) {
    case 0:
      assert(strncmp(recvbuf, "abcdefghijklmn", recvcount) == 0);
      break;
    case 1:
      assert(strncmp(recvbuf, "efghijklmn", recvcount) == 0);
      break;
    case 2:
      assert(strncmp(recvbuf, "ghijklmnopqr", recvcount) == 0);
      break;
    case 3:
      assert(strncmp(recvbuf, "klmnopqr", recvcount) == 0);
      break;
  }


  if (mpi_rank == 0) {
    free(sendbuf);
  }
  free(recvbuf);

  MPI_Finalize();
}
