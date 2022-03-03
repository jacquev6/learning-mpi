#include <algorithm>
#include <cassert>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

#include <cairommconfig.h>
#include <cairomm/context.h>
#include <cairomm/surface.h>
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-function-type"
#include <mpi.h>
#pragma GCC diagnostic pop


static int rank;  // Global for convenient debug logging


struct Disc {
  Disc(const int id, const float mass, const float radius, const float position, const float velocity) :
    _id(id),
    _mass(mass),
    _radius(radius),
    _position(position),
    _velocity(velocity)
  {
    // std::cout << rank << ": initialized disc " << _id << " at position " << _position << ", with velocity " << _velocity << std::endl;
  }

  void advance(const float delta) {
    _position += delta * _velocity;  // Integrate _velocity
    _velocity *= 0.9995;  // Decelerate because of friction
    // std::cout << rank << ": advanced disc " << _id << " to position " << _position << ", with velocity " << _velocity << std::endl;
  }

  bool is_still() const {
    const bool is_still = std::abs(_velocity) < 1e-3;
    // std::cout << rank << ": disc " << _id << " is still: " << is_still << std::endl;
    return is_still;
  }

  float center() const {
    return _position;
  }

  float left() const {
    return _position - _radius;
  }

  float right() const {
    return _position + _radius;
  }

  float radius() const {
    return _radius;
  }

 private:
  const int _id;
  const float _mass;  // kg
  const float _radius;  // m
  float _position;  // m, from origin
  float _velocity;  // m/s
};


struct Segment {
  Segment(const float left_position, const float right_position, const std::vector<Disc>& discs) :
    _left_position(left_position),
    _right_position(right_position),
    _discs(discs)
  {
    std::cout << rank << ": initialized segment [" << _left_position << ", " << _right_position << "] with " << _discs.size() << " _discs" << std::endl;
  }

  void advance(const float delta) {
    std::cout << rank << ": advancing segment" << std::endl;
    for (Disc& disc : _discs) {
      disc.advance(delta);
    }
  }

  bool is_still() {
    const bool is_still = std::all_of(_discs.begin(), _discs.end(), [](const Disc& disc) { return disc.is_still(); });  // @todo Skip the lambda, pass Disc::is_still directly
    // std::cout << rank << ": segment is still: " << is_still << std::endl;
    return is_still;
  }

  void draw(const std::string& file_name) {
    std::cout << rank << ": drawing segment to " << file_name << std::endl;

    auto surface = Cairo::ImageSurface::create(Cairo::Format::FORMAT_ARGB32, (_right_position - _left_position) * 10, 60);
    auto cr = Cairo::Context::create(surface);

    if (rank % 2) {
      cr->set_source_rgb(1, 0.8, 0.8);
    } else {
      cr->set_source_rgb(0.8, 1, 0.8);
    }
    cr->paint();

    cr->scale(10, 10);
    cr->translate(-_left_position, 3);

    cr->set_source_rgb(0, 0, 0);
    cr->move_to(_left_position, 0);
    cr->line_to(_right_position, 0);
    cr->set_line_width(3);
    cr->save();
    cr->set_identity_matrix();
    cr->stroke();
    cr->restore();

    cr->set_source_rgba(0, 0, 0, 0.5);
    for (const auto& disc : _discs) {
      cr->arc(disc.center(), 0, disc.radius(), 0, 2 * M_PI);
    }
    cr->fill();

    surface->write_to_png(file_name);
  }

 private:
  const float _left_position;
  const float _right_position;
  std::vector<Disc> _discs;
};


bool all_still(bool still) {
  static_assert(sizeof(bool) == 1);  // Ensure MPI_BYTE is the right type for bool
  bool all_still = false;
  MPI_Allreduce(&still, &all_still, 1, MPI_BYTE, MPI_LAND, MPI_COMM_WORLD);
  // std::cout << rank << ": all segments are still: " << all_still << std::endl;
  return all_still;
}


std::string make_filename(const std::string& output_directory, const int rank, const int frame_index) {
  std::ostringstream oss;
  oss << output_directory
      << "/"
      << std::setw(8) << std::setfill('0') << rank
      << '-'
      << std::setw(8) << std::setfill('0') << frame_index
      << ".png";
  return oss.str();
}


int main(int argc, char* argv[]) {
  MPI_Init(&argc, &argv);
  if (argc < 3) return 1;
  std::string output_directory(argv[1]);
  int frames_count = 25 * std::atoi(argv[2]);

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  Segment segment(rank * 10, (rank + 1) * 10, { Disc(rank, 1, 1 + rank / 3.f, rank * 10.f + 3, 7 + rank) });
  segment.draw(make_filename(output_directory, rank, 0));

  for (int frame_index = 1; frame_index != frames_count; ++frame_index) {
    for (int instant_index = 0; instant_index != 40; ++instant_index) {
      // Advance time by 1ms, 40 times per frame == 40ms / frame == 25 fps
      segment.advance(1e-3);
    }
    segment.draw(make_filename(output_directory, rank, frame_index));
  }

  MPI_Finalize();
}
