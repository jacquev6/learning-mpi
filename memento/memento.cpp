#include <cassert>
#include <iostream>
#include <numeric>
#include <vector>

#include <mpi.h>


int main(int argc, char* argv[]) {
  // Usual boilerplate
  MPI_Init(&argc, &argv);


  // Preliminary checks
  static_assert(sizeof(int) == 4);
  int size_of_mpi_int = 0;
  MPI_Type_size(MPI_INT, &size_of_mpi_int);
  assert(size_of_mpi_int == 4);


  // Get size of, and rank in, the "World" communicator
  int size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  assert (size >= 3);


  // One-to-one
  if (rank == 0) {
    int data = 0xAABBCCDD;
    MPI_Send(
      // data, count, type: 'count' is the number of elements of type 'type' to be sent,
      // so the actual number of bytes sent is 'count * MPI_Type_size(type)'
      &data, 1, MPI_INT,
      // destination, tag, communicator:
      // 'source' must match, unless MPI_ANY_SOURCE is used
      // 'tag's on both sides must match, unless MPI_ANY_TAG is used
      1, 57, MPI_COMM_WORLD);

    // Data has not been modified
    assert(data == 0xAABBCCDD);
  } else if (rank == 1) {
    int data = 0;
    MPI_Status status;
    MPI_Recv(
      // data, count, type
      &data, 1, MPI_INT,
      // source, tag, communicator
      0, 57, MPI_COMM_WORLD,
      // status: one can use MPI_STATUS_IGNORE if not interested
      &status);

    // 'status' arguably has little interest with (this simple use of) 'MPI_Recv':
    // its attributes are equal to the arguments passed in 'MPI_Recv'
    assert(status.MPI_SOURCE == 0);
    assert(status.MPI_TAG == 57);
    int count;
    MPI_Get_count(&status, MPI_INT, &count);
    assert(count == 1);

    // And finally the data!
    assert(data == 0xAABBCCDD);
  }


  // One-to-one, dynamic size
  if (rank == 0) {
    std::vector<int> data;
    data.resize(64 + rand() % 64);
    std::iota(data.begin(), data.end(), 0);
    MPI_Send(data.data(), data.size(), MPI_INT, 1, 58, MPI_COMM_WORLD);
  } else if (rank == 1) {
    MPI_Status status;
    MPI_Probe(0, 58, MPI_COMM_WORLD, &status);
    int count;
    MPI_Get_count(&status, MPI_INT, &count);
    assert(count >= 64);
    assert(count < 128);
    std::vector<int> data;
    data.resize(count);
    MPI_Recv(data.data(), data.size(), MPI_INT, 0, 58, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    for (int i = 0; i != data.size(); ++i) {
      assert(data[i] == i);
    }
  }


  // One-to-one, dynamic tag and source
  if (rank < 2) {
    int data = 1000 * rank;
    int tag = 2000 * rank;
    MPI_Send(&data, 1, MPI_INT, 2, tag, MPI_COMM_WORLD);
  } else {
    for (int i = 0; i != 2; ++i) {
      MPI_Status status;
      MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
      assert(status.MPI_TAG == 2000 * status.MPI_SOURCE);
      int data = 0;
      MPI_Recv(&data, 1, MPI_INT, status.MPI_SOURCE, status.MPI_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      assert(data == 1000 * status.MPI_SOURCE);
    }
  }


  // One-to-all, same data
  {
    int data = 0;
    if (rank == 0) {
      data = 42;
    }
    MPI_Bcast(
      &data, 1, MPI_INT,
      // root, communicator: the 'root' is the sender, and all others are receivers
      0, MPI_COMM_WORLD);
    assert(data == 42);
  }


  // One-to-all, same data, dynamic size
  {
    std::vector<int> data;
    int count;
    if (rank == 0) {
      count = 64 + rand() % 64;
      data.resize(count);
      std::iota(data.begin(), data.end(), 0);
    }
    MPI_Bcast(&count, 1, MPI_INT, 0, MPI_COMM_WORLD);
    assert(count >= 64);
    assert(count < 128);
    if (rank != 0) {
      data.resize(count);
    }
    MPI_Bcast(data.data(), data.size(), MPI_INT, 0, MPI_COMM_WORLD);
    for (int i = 0; i != data.size(); ++i) {
      assert(data[i] == i);
    }
  }


  // Usual boilerplate
  MPI_Finalize();
}
