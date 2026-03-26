#ifndef TILE_HPP
#define TILE_HPP

#include "colours.hpp"

#include <variant>

class OccupiedTile {
private:
  Colour _colour;

protected:
  OccupiedTile(const Colour colour);

public:
  [[nodiscard]] auto get_colour() const noexcept -> Colour;
};

class Taken : public OccupiedTile {
public:
  Taken(const Colour colour);
};

class Falling : public OccupiedTile{
public:
  Falling(const Colour colour);
};

struct Empty {};
using Tile = std::variant<Empty, Taken, Falling>;

#endif //TILE_HPP
