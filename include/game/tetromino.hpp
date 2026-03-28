#ifndef TETROMINO_HPP
#define TETROMINO_HPP

#include "coordinates.hpp"
#include "colours.hpp"

#include <array>
#include <functional>
#include <optional>

using TilePositions = std::array<Coordinates, 4>;
using TileOffsets = std::array<Coordinates, 3>;

enum class TetrominoShape : std::uint8_t { L, J, S, Z, T, I, O };

class Tetromino {
private:
  static constexpr std::size_t num_kick_tests = 5;
  using KickOffsets = std::array<Coordinates, num_kick_tests>;
  using WallKickMap = std::array<KickOffsets, 4>;

  static constexpr WallKickMap general_kick_map = {{
    {{ { .x = 0, .y = 0 }, { .x =  0, .y = 0 }, { .x =  0, .y =  0 }, { .x = 0, .y = 0 }, { .x =  0, .y = 0 } }}, // 0
    {{ { .x = 0, .y = 0 }, { .x =  1, .y = 0 }, { .x =  1, .y = -1 }, { .x = 0, .y = 2 }, { .x =  1, .y = 2 } }}, // 90
    {{ { .x = 0, .y = 0 }, { .x =  0, .y = 0 }, { .x =  0, .y =  0 }, { .x = 0, .y = 0 }, { .x =  0, .y = 0 } }}, // 180
    {{ { .x = 0, .y = 0 }, { .x = -1, .y = 0 }, { .x = -1, .y = -1 }, { .x = 0, .y = 2 }, { .x = -1, .y = 2 } }}, // 270
  }};

  static constexpr WallKickMap i_kick_map = {{
    {{ { .x =  0, .y = 0 }, { .x = -1, .y = 0 }, { .x =  2, .y = 0 }, { .x = -1, .y =  0 }, { .x =  2, .y =  0 } }}, // 0
    {{ { .x = -1, .y = 0 }, { .x =  0, .y = 0 }, { .x =  0, .y = 0 }, { .x =  0, .y =  1 }, { .x =  0, .y = -2 } }}, // 90
    {{ { .x = -1, .y = 1 }, { .x =  1, .y = 1 }, { .x = -2, .y = 1 }, { .x =  1, .y =  0 }, { .x = -2, .y =  0 } }}, // 180
    {{ { .x =  0, .y = 1 }, { .x =  0, .y = 1 }, { .x =  0, .y = 1 }, { .x =  0, .y = -1 }, { .x =  0, .y =  2 } }}  // 270
  }};
  
  enum class RotationalPos : std::uint8_t { Zero, Ninety, OneEighty, TwoSeventy };

  std::optional<std::reference_wrapper<const WallKickMap>> _kick_map;
  Colour _colour;
  TileOffsets _tile_offsets;
  RotationalPos _rotational_pos = RotationalPos::Zero;

  [[nodiscard]] auto calc_next_rotation_pos(bool clockwise) const noexcept -> RotationalPos;
  [[nodiscard]] static auto calc_tile_positions(const Coordinates& pivot_pos,
      const TileOffsets& offsets) -> TilePositions;
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
