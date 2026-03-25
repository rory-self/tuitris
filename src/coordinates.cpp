#include "coordinates.hpp"

auto operator+(const Coordinates& lhs, const Coordinates& rhs) -> Coordinates {
  const int new_x_pos = lhs.x + rhs.x;
  const int new_y_pos = lhs.y + rhs.y;

  return { .x = new_x_pos, .y = new_y_pos };
}
