#include "tile.hpp"

OccupiedTile::OccupiedTile(const Colour colour) : _colour(colour) {}

auto OccupiedTile::get_colour() const noexcept -> Colour {
  return _colour;
}

Taken::Taken(const Colour colour) : OccupiedTile(colour) {}
Falling::Falling(const Colour colour) : OccupiedTile(colour) {}

