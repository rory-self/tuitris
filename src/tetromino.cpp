#include "coordinates.hpp"
#include <tetromino.hpp>

auto Tetromino::get_tile_positions(const Coordinates& pivot_pos) const -> TilePositions {
  TilePositions tile_positions;
  for (std::size_t i = 0; i < tile_positions.size() - 1; i++) {
    tile_positions[i] = _offsets[i] + pivot_pos;
  }
  tile_positions[3] = pivot_pos;

  return tile_positions;
}

auto Tetromino::rotate_anticlockwise(const Coordinates& pivot_pos) -> bool {
  for (Coordinates& offset : _offsets) {
    offset = {offset.y, -offset.x};
  }

  switch (_rotation) {
    case Rotation::Zero:
      _rotation = Rotation::TwoSeventy;
      break;
    case Rotation::Ninety:
      _rotation = Rotation::Zero;
      break;
    case Rotation::OneEighty:
      _rotation = Rotation::Ninety;
      break;
    case Rotation::TwoSeventy:
      _rotation = Rotation::OneEighty;
      break;
  }

  return true;
}

auto Tetromino::rotate_clockwise(const Coordinates& pivot_pos) -> bool {
  for (Coordinates& offset : _offsets) {
    offset = {-offset.y, offset.x};
  }

  switch (_rotation) {
    case Rotation::Zero:
      _rotation = Rotation::Ninety;
      break;
    case Rotation::Ninety:
      _rotation = Rotation::OneEighty;
      break;
    case Rotation::OneEighty:
      _rotation = Rotation::TwoSeventy;
      break;
    case Rotation::TwoSeventy:
      _rotation = Rotation::Zero;
      break;
  }

  return true;
}

JTetromino::JTetromino() {
  _offsets = {{{-1, 0}, {-1, 1}, {1, 0}}};
}

OTetromino::OTetromino() {
  _offsets = {{{0, 1}, {1, 1}, {1, 0}}};
}

LTetromino::LTetromino() {
  _offsets = {{{-1, 0}, {1, 0}, {1, 1}}};
}

ITetromino::ITetromino() {
  _offsets = {{{-1, 0}, {1, 0}, {2, 0}}};
}

STetromino::STetromino() {
  _offsets = {{{-1, 0}, {0, 1}, {1, 1}}};
}

ZTetromino::ZTetromino() {
  _offsets = {{{-1, 1}, {0, 1}, {1, 0}}};
}

TTetromino::TTetromino() {
  _offsets = {{{-1, 0}, {0, 1}, {1, 0}}};
}

auto OTetromino::rotate_clockwise(const Coordinates& pivot_pos) -> bool {
  return false;
}

auto OTetromino::rotate_anticlockwise(const Coordinates& pivot_pos) -> bool {
  return false;
}
