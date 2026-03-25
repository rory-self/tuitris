#include <tetromino.hpp>

Tetromino::Tetromino(const Shape shape) {
  switch (shape) {
    case Shape::L:
      _offsets = {{{-1, 0}, {1, 0}, {-1, 1}}};
      break;
    case Shape::I:
    case Shape::O:
    case Shape::Z:
    case Shape::S:
    case Shape::J:
    case Shape::T:
      break;
  }
}

auto Tetromino::get_tile_positions(const Coordinates& pivot_pos) const -> std::array<Coordinates, 4> {
  std::array<Coordinates, 4> tile_positions;
  for (std::size_t i = 0; i < tile_positions.size() - 1; i++) {
    tile_positions[i] = _offsets[i] + pivot_pos;
  }
  tile_positions[3] = pivot_pos;

  return tile_positions;
}
