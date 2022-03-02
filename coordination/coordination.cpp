#include <cassert>
#include <algorithm>
#include <iostream>
#include <thread>

#include <mpi.h>


using namespace std::chrono_literals;

const int coordinator_rank = 0;

std::string receive_string_from(int rank) {
  char buf[1024];
  MPI_Recv(buf, sizeof(buf), MPI_CHAR, rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  return buf;
}

void send_string_to(const std::string& message, int rank) {
  MPI_Send(message.data(), message.length() + 1, MPI_CHAR, rank, 0, MPI_COMM_WORLD);
}

void coordinator(const int size) {
  for (int rank = 1; rank != size; ++rank) {
    assert(receive_string_from(rank) == "ready");
    std::cout << "Worker " << rank << " ready" << std::endl;
  }
  for (int i = 0; i != 10; ++i) {
    for (int rank = 1; rank != size; ++rank) {
      send_string_to("reverse me", rank);
    }
    for (int rank = 1; rank != size; ++rank) {
      assert(receive_string_from(rank) == "em esrever");
    }
    std::cout << "Iteration " << i << " OK" << std::endl;
  }
  for (int rank = 1; rank != size; ++rank) {
    send_string_to("done", rank);
  }
}

void worker() {
  send_string_to("ready", coordinator_rank);
  while (true) {
    auto message = receive_string_from(coordinator_rank);
    if (message == "done") {
      break;
    } else {
      std::this_thread::sleep_for(1s);
      std::reverse(message.begin(), message.end());
      send_string_to(message, coordinator_rank);
    }
  }
}

int main(int argc, char* argv[]) {
  MPI_Init(&argc, &argv);

  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (rank == coordinator_rank) {
    coordinator(size);
  } else {
    worker();
  }

  MPI_Finalize();
}
