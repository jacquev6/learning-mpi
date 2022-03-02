#include <iostream>

#include <gtest/gtest.h>
#include <mpi.h>


#ifdef AUTOMATED_TESTS

TEST(Foo, Bar) {
  EXPECT_EQ(1, 1);
}

#else

int main(int argc, char* argv[]) {
  MPI_Init(&argc, &argv);

  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  MPI_Finalize();
}

#endif
