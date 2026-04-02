#ifndef TILE_HPP
#define TILE_HPP

#include "colours.hpp"

#include <variant>

class OccupiedTile {
private:
  Colour _colour;

protected:
  OccupiedTile(Colour colour);

public:
  [[nodiscard]] auto get_colour() const noexcept -> Colour;
};

class Falling : public OccupiedTile{
public:
  explicit Falling(Colour colour);
};

class Taken : public OccupiedTile {
public:
  explicit Taken(const Falling& falling);
};

struct Empty {};
using Tile = std::variant<Empty, Taken, Falling>;

#endif //TILE_HPP
