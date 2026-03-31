#include "coordinates.hpp"

auto CoordinatesHasher::operator()(const Coordinates& coords) const -> std::size_t {
  return coords.x ^ (coords.y << 1);
}

auto operator+(const Coordinates& lhs, const Coordinates& rhs) -> Coordinates {
  return { .x = lhs.x + rhs.x, .y = lhs.y + rhs.y };
}

auto operator-(const Coordinates& lhs, const Coordinates& rhs) -> Coordinates {
  return { .x = lhs.x - rhs.x, .y = lhs.y - rhs.y };
}
