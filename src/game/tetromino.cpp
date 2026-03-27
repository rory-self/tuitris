#include "coordinates.hpp"
#include <game/tetromino.hpp>

#include <ranges>

auto Tetromino::get_next_rotation_pos(const bool clockwise) const noexcept -> RotationalPos {
  using enum RotationalPos;

  switch (_rotational_pos) {
    case Zero:
      return clockwise ? Ninety : TwoSeventy;
    case Ninety:
      return clockwise ? OneEighty : Zero;
    case OneEighty:
      return clockwise ? TwoSeventy : Ninety;
    case TwoSeventy:
      return clockwise ? Zero : OneEighty;
  }

  std::unreachable();
}

auto Tetromino::get_colour() const noexcept -> Colour {
  return _colour;
}

auto Tetromino::get_tile_positions(const Coordinates& pivot_pos,
    const TileOffsets& offsets) const -> TilePositions {
  TilePositions tile_positions;
  for (std::size_t i = 0; i < tile_positions.size() - 1; i++) {
    tile_positions[i] = offsets[i] + pivot_pos;
  }
  tile_positions[3] = pivot_pos;

  return tile_positions;
}

auto Tetromino::get_tile_positions(const Coordinates& pivot_pos) const -> TilePositions {
  return get_tile_positions(pivot_pos, _offsets);
}

auto Tetromino::try_rotate(const RotationParams& params, const WallKickMap& kick_map) -> bool {
  const auto& [pivot_pos, test, is_clockwise] = params;
  const RotationalPos next_rotational_pos = get_next_rotation_pos(is_clockwise);

  // TODO clean
  TileOffsets new_tile_offsets = _offsets;
  for (Coordinates& tile_offset : new_tile_offsets) {
    Coordinate& coord_to_flip = is_clockwise ? tile_offset.y : tile_offset.x;
    coord_to_flip *= -1;

    std::swap(tile_offset.x, tile_offset.y);
  }

  const KickOffsets& local_kick_offsets = kick_map[static_cast<std::size_t>(_rotational_pos)];
  const KickOffsets& next_kick_offsets = kick_map[static_cast<std::size_t>(next_rotational_pos)];

  for (const auto& [local_kick_offset, next_kick_offset] : std::views::zip(local_kick_offsets, next_kick_offsets)) {
    const Coordinates kick_translation = local_kick_offset - next_kick_offset;
    const Coordinates new_pivot_pos = pivot_pos + kick_translation;

    const TilePositions new_tile_positions = get_tile_positions(new_pivot_pos, new_tile_offsets);
    if (test(new_tile_positions)) {
      _offsets = new_tile_offsets;
      return true;
    }
  }

  return false;
}

auto GeneralTetromino::try_rotate(const RotationParams& params) -> bool {
  return Tetromino::try_rotate(params, _kick_data);
}

JTetromino::JTetromino() {
  _offsets = {{{-1, 0}, {-1, 1}, {1, 0}}};
  _colour = Colour::Blue;
}

OTetromino::OTetromino() {
  _offsets = {{{0, 1}, {1, 1}, {1, 0}}};
  _colour = Colour::Yellow;
}

LTetromino::LTetromino() {
  _offsets = {{{-1, 0}, {1, 0}, {1, 1}}};
  _colour = Colour::Orange;
}

ITetromino::ITetromino() {
  _offsets = {{{-1, 0}, {1, 0}, {2, 0}}};
  _colour = Colour::Cyan;
}

auto ITetromino::try_rotate(const RotationParams& params) -> bool {
  return Tetromino::try_rotate(params, _kick_data);
}

STetromino::STetromino() {
  _offsets = {{{-1, 0}, {0, 1}, {1, 1}}};
  _colour = Colour::Green;
}

ZTetromino::ZTetromino() {
  _offsets = {{{-1, 1}, {0, 1}, {1, 0}}};
  _colour = Colour::Red;
}

TTetromino::TTetromino() {
  _offsets = {{{-1, 0}, {0, 1}, {1, 0}}};
  _colour = Colour::Purple;
}

auto OTetromino::try_rotate(const RotationParams& params) -> bool {
  return false;
}

