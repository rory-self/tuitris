#ifndef TETROMINO_HPP
#define TETROMINO_HPP

#include "coordinates.hpp"
#include "colours.hpp"

#include <array>
#include <functional>
#include <optional>

using TilePositions = std::array<Coordinates, 4>;
using TileOffsets = std::array<Coordinates, 3>;

enum class TetrominoShape { L, J, S, Z, T, I, O };

class Tetromino {
private:
  using KickOffsets = std::array<Coordinates, 5>;
  using WallKickMap = std::array<KickOffsets, 4>;

  static constexpr WallKickMap general_kick_map = {{
    {{ { 0, 0 }, {  0, 0 }, {  0,  0 }, { 0, 0 }, {  0, 0 } }}, // 0
    {{ { 0, 0 }, {  1, 0 }, {  1, -1 }, { 0, 2 }, {  1, 2 } }}, // 90
    {{ { 0, 0 }, {  0, 0 }, {  0,  0 }, { 0, 0 }, {  0, 0 } }}, // 180
    {{ { 0, 0 }, { -1, 0 }, { -1, -1 }, { 0, 2 }, { -1, 2 } }}, // 270
  }};

  static constexpr WallKickMap i_kick_map = {{
    {{ {  0, 0 }, { -1, 0 }, {  2, 0 }, { -1,  0 }, {  2,  0 } }}, // 0
    {{ { -1, 0 }, {  0, 0 }, {  0, 0 }, {  0,  1 }, {  0, -2 } }}, // 90
    {{ { -1, 1 }, {  1, 1 }, { -2, 1 }, {  1,  0 }, { -2,  0 } }}, // 180
    {{ {  0, 1 }, {  0, 1 }, {  0, 1 }, {  0, -1 }, {  0,  2 } }}  // 270
  }};
  
  enum class RotationalPos { Zero, Ninety, OneEighty, TwoSeventy };

  std::optional<std::reference_wrapper<const WallKickMap>> _kick_map;
  Colour _colour;
  TileOffsets _tile_offsets;
  RotationalPos _rotational_pos = RotationalPos::Zero;

  [[nodiscard]] auto calc_next_rotation_pos(bool clockwise) const noexcept -> RotationalPos;
  [[nodiscard]] auto calc_tile_positions(const Coordinates& pivot_pos,
      const TileOffsets& offsets) const -> TilePositions;
  [[nodiscard]] auto calc_rotated_offsets(bool clockwise) const -> TileOffsets;

public:
  explicit Tetromino(TetrominoShape shape);

  [[nodiscard]] auto get_colour() const noexcept -> Colour;
  [[nodiscard]] auto get_offsets() const noexcept -> const TileOffsets&;

  [[nodiscard]] auto calc_tile_positions(const Coordinates& pivot_pos) const -> TilePositions;
  [[nodiscard]] auto try_rotate(const Coordinates& pivot_pos,
      const std::function<bool(TilePositions)>& placement_method,
      bool clockwise) -> bool;
};

#endif // TETROMINO_HPP
