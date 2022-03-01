// Inspired by https://mpitutorial.com/tutorials/mpi-send-and-receive/#ring-program
// but written from scratch without looking too much at the original code.

#include <iostream>

#include <mpi.h>


int main(int argc, char* argv[]) {
  MPI_Init(&argc, &argv);

  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  if (size < 2) {
    std::cerr << "Need at least two processes in the ring" << std::endl;
    return 1;
  }

  // The message should turn around the ring 10 times
  int message = 0;
  const int last_message = 4;

  while(true) {
    if (message != 0 || rank != 0) {
      int other_rank = (rank + size - 1) % size;
      MPI_Recv(&message, 1, MPI_INT, other_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      std::cout << "Process " << rank << " received message '" << message << "' from process " << other_rank << std::endl;
    }

    if (rank == 0 && message == last_message) {
      break;
    }

    if (rank == 0) {
      ++message;
    }

    int other_rank = (rank + 1) % size;
    std::cout << "Process " << rank << " sending message '" << message << "' to process " << other_rank << std::endl;
    MPI_Send(&message, 1, MPI_INT, other_rank, 0, MPI_COMM_WORLD);

    if (rank != 0 && message == last_message) {
      break;
    }
  }

  MPI_Finalize();
}
