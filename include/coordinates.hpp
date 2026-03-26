#ifndef COORDINATES_HPP
#define COORDINATES_HPP

#include <cstddef>

constexpr std::size_t game_width = 10;
constexpr std::size_t vanishing_area_height = 20;
constexpr std::size_t play_area_height = 20;
constexpr std::size_t game_height = vanishing_area_height + play_area_height;
constexpr int signed_game_height = static_cast<int>(game_height);
constexpr int signed_game_width = static_cast<int>(game_width);

using Coordinate = int;
struct Coordinates {
  Coordinate x;
  Coordinate y;
};

[[nodiscard]] auto operator+(const Coordinates& lhs, const Coordinates& rhs) -> Coordinates;

#endif // COORDINATES_HPP
