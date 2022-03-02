#include <iostream>
#include <optional>
#include <queue>

#include <gtest/gtest.h>
#include <mpi.h>


struct Disc {
  std::string id;
  float mass;  // kg
  float radius;  // m

  // location(t) = location0 + (t - t0) * speed0
  float t0;  // s
  float location0;  // m, from origin
  float speed0;  // m/s
};

struct Collision {
  float t;  // s
  Disc left;
  Disc right;
};

std::optional<Collision> collision(const Disc& left, const Disc& right) {
  // Solve for t:
  //     left.location(t) + left.radius == right.location(t) - right.radius
  // <=> left.location0 + (t - left.t0) * left.speed0 + left.radius == right.location0 + (t - right.t0) * right.speed0 - right.radius
  // <=>  (t - left.t0) * left.speed0 - (t - right.t0) * right.speed0 == right.location0 - left.location0 - right.radius - left.radius
  // <=>  t * left.speed0 - t * right.speed0 == right.location0 - left.location0 + left.t0 * left.speed0 - right.t0 * right.speed0 - right.radius - left.radius
  // <=> t * (left.speed0 - right.speed0) == right.location0 - left.location0 + left.t0 * left.speed0 - right.t0 * right.speed0 - right.radius - left.radius
  if (left.speed0 == right.speed0) {
    return std::nullopt;
  } else {
    const float t = (right.location0 - left.location0 + left.t0 * left.speed0 - right.t0 * right.speed0 - right.radius - left.radius) / (left.speed0 - right.speed0);
    return Collision {t, left, right};
  }
}

std::optional<Collision> next_collision(float t, const Disc& left, const Disc& right) {
  auto coll = collision(left, right);
  if (coll && coll->t > t) {
    return coll;
  } else {
    return std::nullopt;
  }
}

#ifdef AUTOMATED_TESTS

TEST(NextCollision, Simple) {
  Disc left { "left", 1, 1, 0, -11, 1 };
  Disc right { "right", 1, 1, 0, 11, -1 };

  auto collision = next_collision(0, left, right);
  ASSERT_TRUE(collision.has_value());
  ASSERT_EQ(collision->t, 10);
}

#endif

struct Event {
  float t;
};

bool compare_events_by_date(std::shared_ptr<Event> a, std::shared_ptr<Event> b) {
  return a->t < b->t;
}

class Segment {
 public:
  Segment(float start, float stop, std::vector<Disc> discs) :
      _start(start),
      _stop(stop),
      _discs(discs),
      _events(&compare_events_by_date)
  {
    for (auto& disc : _discs) {
      assert(disc.t0 == 0);
      assert(disc.location0 >= _start);
      assert(disc.location0 <= _stop);
    }
  }

 private:
  float _start;
  float _stop;
  std::vector<Disc> _discs;

  std::priority_queue<std::shared_ptr<Event>, std::vector<std::shared_ptr<Event>>, bool(*)(std::shared_ptr<Event>, std::shared_ptr<Event>)> _events;
};

#ifndef AUTOMATED_TESTS

int main(int argc, char* argv[]) {
  MPI_Init(&argc, &argv);

  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  MPI_Finalize();
}

#endif
