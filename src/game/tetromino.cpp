#include "coordinates.hpp"
#include <game/tetromino.hpp>

#include <ranges>

namespace {
  constexpr WallKickMap general_kick_map = {{
    {{ { 0, 0 }, {  0, 0 }, {  0,  0 }, { 0, 0 }, {  0, 0 } }}, // 0
    {{ { 0, 0 }, {  1, 0 }, {  1, -1 }, { 0, 2 }, {  1, 2 } }}, // 90
    {{ { 0, 0 }, {  0, 0 }, {  0,  0 }, { 0, 0 }, {  0, 0 } }}, // 180
    {{ { 0, 0 }, { -1, 0 }, { -1, -1 }, { 0, 2 }, { -1, 2 } }}, // 270
  }};

  constexpr WallKickMap i_kick_map = {{
    {{ {  0, 0 }, { -1, 0 }, {  2, 0 }, { -1,  0 }, {  2,  0 } }}, // 0
    {{ { -1, 0 }, {  0, 0 }, {  0, 0 }, {  0,  1 }, {  0, -2 } }}, // 90
    {{ { -1, 1 }, {  1, 1 }, { -2, 1 }, {  1,  0 }, { -2,  0 } }}, // 180
    {{ {  0, 1 }, {  0, 1 }, {  0, 1 }, {  0, -1 }, {  0,  2 } }}  // 270
  }};
}

Tetromino::Tetromino(const TetrominoShape shape) {
    using enum TetrominoShape;
    using enum Colour;

    switch (shape) {
      case L:
        _tile_offsets = {{ {-1, 0}, {1, 0}, {1, -1} }};
        _kick_map = general_kick_map;
        _colour = Orange;
        break;
      case J:
        _tile_offsets = {{ {-1, 0}, {-1, -1}, {1, 0} }};
        _kick_map = general_kick_map;
        _colour = Blue;
        break;
      case T:
        _tile_offsets = {{{-1, 0}, {0, 1}, {1, 0}}};
        _kick_map = general_kick_map;
        _colour = Purple;
        break;
      case S:
        _tile_offsets = {{{-1, 0}, {0, 1}, {1, 1}}};
        _kick_map = general_kick_map;
        _colour = Green;
        break;
      case Z:
        _tile_offsets = {{{-1, 1}, {0, 1}, {1, 0}}};
        _kick_map = general_kick_map;
        _colour = Red;
        break;
      case I:
        _tile_offsets = {{{-1, 0}, {1, 0}, {2, 0}}};
        _kick_map = i_kick_map;
        _colour = Cyan;
        break;
      case O:
        _tile_offsets = {{{0, 1}, {1, 1}, {1, 0}}};
        _kick_map = std::nullopt;
        _colour = Yellow;
        break;
    }
  }

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
  return get_tile_positions(pivot_pos, _tile_offsets);
}

auto Tetromino::get_rotated_offsets(const bool clockwise) const -> TileOffsets {
  TileOffsets rotated_tile_offsets = _tile_offsets;
  for (Coordinates& tile_offset : rotated_tile_offsets) {
    Coordinate& coord_to_flip = clockwise ? tile_offset.y : tile_offset.x;
    coord_to_flip *= -1;

    std::swap(tile_offset.x, tile_offset.y);
  }

  return rotated_tile_offsets;
}

auto Tetromino::try_rotate(const RotationParams& params) -> bool {
  if (not _kick_map.has_value()) {
    return false;
  }

  const auto& [pivot_pos, placement_test, is_clockwise] = params;
  const RotationalPos next_rotational_pos = get_next_rotation_pos(is_clockwise);

  const TileOffsets new_tile_offsets = get_rotated_offsets(is_clockwise);

  const WallKickMap& kick_map = _kick_map.value().get();
  const KickOffsets& local_kick_offsets = kick_map[static_cast<std::size_t>(_rotational_pos)];
  const KickOffsets& next_kick_offsets = kick_map[static_cast<std::size_t>(next_rotational_pos)];

  for (const auto& [local_kick_offset, next_kick_offset] : std::views::zip(local_kick_offsets, next_kick_offsets)) {
    const Coordinates kick_translation = local_kick_offset - next_kick_offset;
    const Coordinates new_pivot_pos = pivot_pos + kick_translation;

    const TilePositions new_tile_positions = get_tile_positions(new_pivot_pos, new_tile_offsets);
    if (placement_test(new_tile_positions)) {
      _tile_offsets = new_tile_offsets;
      _rotational_pos = next_rotational_pos;
      return true;
    }
  }

  return false;
}

