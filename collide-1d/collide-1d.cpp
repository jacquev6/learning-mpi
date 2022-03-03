#include <algorithm>
#include <iostream>
#include <vector>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-function-type"
#include <mpi.h>
#pragma GCC diagnostic pop


static int rank;  // Global for convenient debug logging


struct Disc {
  Disc(const int id_, const float mass_, const float radius_, const float position_, const float velocity_) :
    id(id_),
    mass(mass_),
    radius(radius_),
    position(position_),
    velocity(velocity_)
  {
    std::cout << rank << ": initialized disc " << id << " at position " << position << ", with velocity " << velocity << std::endl;
  }

  void advance(const float delta) {
    position += delta * velocity;  // Integrate velocity
    velocity *= 0.9;  // Decelerate because of friction
    std::cout << rank << ": advanced disc " << id << " to position " << position << ", with velocity " << velocity << std::endl;
  }

  bool is_still() const {
    const bool is_still = std::abs(velocity) < 1e-2;
    std::cout << rank << ": disc " << id << " is_still: " << is_still << std::endl;
    return is_still;
  }

  float left() const {
    return position - radius;
  }

  float right() const {
    return position + radius;
  }

 private:
  const int id;
  const float mass;  // kg
  const float radius;  // m
  float position;  // m, from origin
  float velocity;  // m/s
};


struct Segment {
  Segment(const float left_position_, const float right_position_, const std::vector<Disc>& discs_) :
    left_position(left_position_),
    right_position(right_position_),
    discs(discs_)
  {
    std::cout << rank << ": initialized segment [" << left_position << ", " << right_position << "] with " << discs.size() << " discs" << std::endl;
  }

  void advance(const float delta) {
    std::cout << rank << ": advancing segment" << std::endl;
    for (Disc& disc : discs) {
      disc.advance(delta);
    }
  }

  bool is_still() {
    const bool is_still = std::all_of(discs.begin(), discs.end(), [](const Disc& disc) { return disc.is_still(); });  // @todo Skip the lambda, pass Disc::is_still directly
    std::cout << rank << ": segment is still: " << is_still << std::endl;
    return is_still;
  }

 private:
  const float left_position;
  const float right_position;
  std::vector<Disc> discs;
};


bool all_still(bool still) {
  static_assert(sizeof(bool) == 1);  // Ensure MPI_BYTE is the right type for bool
  bool all_still = false;
  MPI_Allreduce(&still, &all_still, 1, MPI_BYTE, MPI_LAND, MPI_COMM_WORLD);
  std::cout << rank << ": all segments are still: " << all_still << std::endl;
  return all_still;
}


int main(int argc, char* argv[]) {
  MPI_Init(&argc, &argv);

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  Segment segment(rank * 10, (rank + 1) * 10, { Disc(rank, 1, 1, rank * 10.f + 1, 1) });
  while (!all_still(segment.is_still())) {
    segment.advance(1e-3);
  }

  MPI_Finalize();
}
