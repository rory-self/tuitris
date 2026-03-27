#ifndef TETROMINO_HPP
#define TETROMINO_HPP

#include "coordinates.hpp"
#include "colours.hpp"

#include <array>
#include <functional>

enum class RotationalPos {
  Zero = 0,
  Ninety = 1,
  OneEighty = 2,
  TwoSeventy = 3
};
using KickOffsets = std::array<Coordinates, 5>;
using WallKickMap = std::array<KickOffsets, 4>;

using TilePositions = std::array<Coordinates, 4>;

using CollisionTest = std::function<bool(TilePositions)>;
struct RotationParams {
  const Coordinates& pivot_pos;
  const CollisionTest& test;
  const bool clockwise;
};

class Tetromino {
private:
  using TileOffsets = std::array<Coordinates, 3>;
  RotationalPos _rotational_pos = RotationalPos::Zero;

  [[nodiscard]] auto get_next_rotation_pos(const bool clockwise) const noexcept -> RotationalPos;
  [[nodiscard]] auto get_tile_positions(const Coordinates& pivot_pos,
      const TileOffsets& offsets) const -> TilePositions;

protected:
  TileOffsets _offsets;
  Colour _colour;

  Tetromino() = default;

  [[nodiscard]] auto try_rotate(const RotationParams& params, const WallKickMap& kick_map) -> bool;

public:
  virtual ~Tetromino() = default;

  [[nodiscard]] auto get_tile_positions(const Coordinates& pivot_pos) const -> TilePositions;
  [[nodiscard]] auto get_colour() const noexcept -> Colour;

  [[nodiscard]] virtual auto try_rotate(const RotationParams& params) -> bool = 0;
};

class GeneralTetromino : public Tetromino {
private:
  static constexpr WallKickMap _kick_data = {{
    {{ { 0, 0 }, {  0, 0 }, {  0,  0 }, { 0, 0 }, {  0, 0 } }}, // 0
    {{ { 0, 0 }, {  1, 0 }, {  1, -1 }, { 0, 2 }, {  1, 2 } }}, // 90
    {{ { 0, 0 }, {  0, 0 }, {  0,  0 }, { 0, 0 }, {  0, 0 } }}, // 180
    {{ { 0, 0 }, { -1, 0 }, { -1, -1 }, { 0, 2 }, { -1, 2 } }}  // 270
  }};

protected:
  GeneralTetromino() = default;

public:
  [[nodiscard]] auto try_rotate(const RotationParams& params) -> bool override;
};

class LTetromino : public GeneralTetromino {
public:
  LTetromino();
};

class ITetromino : public Tetromino {
private:
  static constexpr WallKickMap _kick_data = {{
    {{ {  0, 0 }, { -1, 0 }, {  2, 0 }, { -1,  0 }, {  2,  0 } }}, // 0
    {{ { -1, 0 }, {  0, 0 }, {  0, 0 }, {  0,  1 }, {  0, -2 } }}, // 90
    {{ { -1, 1 }, {  1, 1 }, { -2, 1 }, {  1,  0 }, { -2,  0 } }}, // 180
    {{ {  0, 1 }, {  0, 1 }, {  0, 1 }, {  0, -1 }, {  0,  2 } }}  // 270
  }};

public:
  ITetromino();

  [[nodiscard]] auto try_rotate(const RotationParams& params) -> bool override;
};

class OTetromino : public Tetromino {
public:
  OTetromino();

  [[nodiscard]] auto try_rotate(const RotationParams& params) -> bool override;
};

class JTetromino : public GeneralTetromino {
public:
  JTetromino();
};

class TTetromino : public GeneralTetromino {
public:
  TTetromino();
};

class ZTetromino : public GeneralTetromino {
public:
  ZTetromino();
};

class STetromino : public GeneralTetromino {
public:
  STetromino();
};


#endif // TETROMINO_HPP
