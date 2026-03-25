#ifndef TETROMINO_HPP
#define TETROMINO_HPP

#include "coordinates.hpp"

#include <array>

enum class Direction { Left, Right };
enum class Shape {L, I, O, Z, S, J, T};

class Tetromino {
private:
  std::array<Coordinates, 3> _offsets;

public:
  Tetromino(const Shape shape);

  [[nodiscard]] auto get_tile_positions(const Coordinates& pivot_pos) const -> std::array<Coordinates, 4>;
};

#endif // TETROMINO_HPP
