#include <cassert>
#include <complex>
#include <vector>

#include <mpi.h>

using namespace std::complex_literals;


static_assert(sizeof(float) == 4);
typedef std::complex<float> complex;
static_assert(sizeof(complex) == 8);


void add_complex(void* invec_, void* inoutvec_, int* len, MPI_Datatype* datatype) {
  complex* invec = reinterpret_cast<complex*>(invec_);
  complex* inoutvec = reinterpret_cast<complex*>(inoutvec_);
  for (int i = 0; i != *len; ++i) {
    *inoutvec += *invec;
    ++inoutvec;
    ++invec;
  }
}

int main(int argc, char* argv[]) {
  MPI_Init(&argc, &argv);

  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  std::vector<complex> data_to_send { 1., float(rank) * 1.if, float(rank) + 1.if };
  std::vector<complex> data_to_recv(data_to_send.size());

  // Reducing complexes as floats, possible because addition distributes on the complex and imaginary parts
  MPI_Allreduce(data_to_send.data(), data_to_recv.data(), 2 * data_to_send.size(), MPI_FLOAT, MPI_SUM, MPI_COMM_WORLD);

  assert(data_to_recv[0] == complex(size));
  assert(data_to_recv[1] == float((size - 1) * size / 2) * 1.if );
  assert(data_to_recv[2] == float((size - 1) * size / 2)  + float(size) * 1.if );

  data_to_recv[0] = data_to_recv[1] = data_to_recv[2] = 0;

  // Define a new MPI type
  MPI_Datatype cmplx;
  MPI_Type_contiguous(2, MPI_FLOAT, &cmplx);
  MPI_Type_commit(&cmplx);

  // Define the addition operation on complex
  MPI_Op cmplx_sum;
  MPI_Op_create(&add_complex, true, &cmplx_sum);

  // Reduce as custom complex type
  MPI_Allreduce(data_to_send.data(), data_to_recv.data(), data_to_send.size(), cmplx, cmplx_sum, MPI_COMM_WORLD);

  assert(data_to_recv[0] == complex(size));
  assert(data_to_recv[1] == float((size - 1) * size / 2) * 1.if );
  assert(data_to_recv[2] == float((size - 1) * size / 2)  + float(size) * 1.if );

  MPI_Finalize();
}
