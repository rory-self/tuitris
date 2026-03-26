#ifndef TETROMINO_HPP
#define TETROMINO_HPP

#include "coordinates.hpp"
#include "colours.hpp"

#include <array>

enum class Rotation { Zero, Ninety, OneEighty, TwoSeventy };
using TilePositions = std::array<Coordinates, 4>;

class Tetromino {
protected:
  std::array<Coordinates, 3> _offsets;
  Rotation _rotation = Rotation::Zero;
  Colour _colour;

  Tetromino() = default;

  [[nodiscard]] auto rotate_anticlockwise(const Coordinates& pivot_pos) -> bool;
  [[nodiscard]] auto rotate_clockwise(const Coordinates& pivot_pos) -> bool;

public:
  virtual ~Tetromino() = default;

  [[nodiscard]] auto get_tile_positions(const Coordinates& pivot_pos) const -> TilePositions;
  [[nodiscard]] virtual auto rotate(const Coordinates& pivot_pos, const bool clockwise) -> bool;
  [[nodiscard]] auto get_colour() const noexcept -> Colour;
};

class LTetromino : public Tetromino {
public:
  LTetromino();
};

class ITetromino : public Tetromino {
public:
  ITetromino();
};

class OTetromino : public Tetromino {
public:
  OTetromino();

  [[nodiscard]] auto rotate(const Coordinates& pivot_pos, const bool clockwise) -> bool override;
};

class JTetromino : public Tetromino {
public:
  JTetromino();
};

class TTetromino : public Tetromino {
public:
  TTetromino();
};

class ZTetromino : public Tetromino {
public:
  ZTetromino();
};

class STetromino : public Tetromino {
public:
  STetromino();
};


#endif // TETROMINO_HPP
