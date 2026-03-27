#include "coordinates.hpp"

auto operator+(const Coordinates& lhs, const Coordinates& rhs) -> Coordinates {
  return { .x = lhs.x + rhs.x, .y = lhs.y + rhs.y };
}

auto operator-(const Coordinates& lhs, const Coordinates& rhs) -> Coordinates {
  return { .x = lhs.x - rhs.x, .y = lhs.y - rhs.y };
}
