#include <sstream>

#include <mpi.h>


int main(int argc, char* argv[]) {
  MPI_Init(&argc, &argv);

  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  MPI_File file;
  MPI_File_open(MPI_COMM_WORLD, "file.txt", MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &file);

  std::ostringstream oss;
  oss << "Rank " << rank;
  std::string data = oss.str();
  data += std::string(10 - data.length(), '.') + '\n';

  MPI_File_seek(file, rank * data.size(), MPI_SEEK_SET);
  MPI_File_write_all(file, data.data(), data.size(), MPI_BYTE, MPI_STATUS_IGNORE);

  MPI_File_close(&file);

  MPI_Finalize();
}
