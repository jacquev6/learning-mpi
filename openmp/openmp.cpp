#include <cassert>

#include <mpi.h>


float one() {
  volatile float x = 3.14;
  volatile float y = 3.14;

  for (int j = 1; j != 102400; ++j) {
    x += j;
    y += j;
  }

  return x / y;
}


int main(int argc, char **argv) {
  MPI_Init(&argc, &argv);

  int mpi_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);

  int mpi_size;
  MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);

  float s = 0;

  #pragma omp parallel for reduction(+:s)
  for (int i = 0; i != 100'000 / mpi_size; ++i) {
    s += one();
  }

  float ss;
  MPI_Reduce(&s, &ss, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);

  if (mpi_rank == 0) {
    assert(ss == 100'000);
  }

  MPI_Finalize();
}
