#ifndef COORDINATES_HPP
#define COORDINATES_HPP

#include <cstddef>

constexpr std::size_t game_width = 10;
constexpr std::size_t game_height = 20;
constexpr int signed_game_height = static_cast<int>(game_height);
constexpr int signed_game_width = static_cast<int>(game_width);

struct Coordinates {
  int x;
  int y;
};

[[nodiscard]] auto operator+(const Coordinates& lhs, const Coordinates& rhs) -> Coordinates;

#endif // COORDINATES_HPP
