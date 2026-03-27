#ifndef TETROMINO_HPP
#define TETROMINO_HPP

#include "coordinates.hpp"
#include "colours.hpp"

#include <array>
#include <functional>
#include <optional>

enum class TetrominoShape {
  L = 0,
  J = 1,
  S = 2,
  Z = 3,
  T = 4,
  I = 5,
  O = 6,
};

enum class RotationalPos {
  Zero = 0,
  Ninety = 1,
  OneEighty = 2,
  TwoSeventy = 3
};

using KickOffsets = std::array<Coordinates, 5>;
using WallKickMap = std::array<KickOffsets, 4>;
using TilePositions = std::array<Coordinates, 4>;

struct RotationParams {
  const Coordinates& pivot_pos;
  const std::function<bool(TilePositions)>& placement_method;
  const bool clockwise;
};

class Tetromino {
private:
  using TileOffsets = std::array<Coordinates, 3>;
  using OptionalKickMap = std::optional<std::reference_wrapper<const WallKickMap>>;

  OptionalKickMap _kick_map;
  Colour _colour;
  TileOffsets _tile_offsets;
  RotationalPos _rotational_pos = RotationalPos::Zero;

  [[nodiscard]] auto get_next_rotation_pos(const bool clockwise) const noexcept -> RotationalPos;
  [[nodiscard]] auto get_tile_positions(const Coordinates& pivot_pos,
      const TileOffsets& offsets) const -> TilePositions;
  [[nodiscard]] auto get_rotated_offsets(const bool clockwise) const -> TileOffsets;

public:
  explicit Tetromino(const TetrominoShape shape);

  [[nodiscard]] auto get_tile_positions(const Coordinates& pivot_pos) const -> TilePositions;
  [[nodiscard]] auto get_colour() const noexcept -> Colour;

  [[nodiscard]] auto try_rotate(const RotationParams& params) -> bool;
};

#endif // TETROMINO_HPP
