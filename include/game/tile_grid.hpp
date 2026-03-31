#ifndef TILE_GRID_HPP
#define TILE_GRID_HPP

#include "coordinates.hpp"
#include "tile.hpp"

#include <array>
#include <unordered_set>

using TileRow = std::array<Tile, game_width>;

class TileGrid {
private:
  using RawTileGrid = std::array<TileRow, game_height>;
  RawTileGrid _tile_grid;

  void fall_tiles();

public:
  TileGrid();

  [[nodiscard]] auto operator[](const Coordinates& pos) const -> const Tile&;
  [[nodiscard]] auto operator[](const Coordinates& pos) -> Tile&;
  [[nodiscard]] auto operator[](std::size_t y_coord) const -> const TileRow&;
  [[nodiscard]] auto operator[](std::size_t y_coord) -> TileRow&;

  [[nodiscard]] auto is_overflowing() const -> bool;
  [[nodiscard]] auto is_taken_or_out_of_bounds(const Coordinates& pos) const -> bool;

  [[nodiscard]] auto begin() const -> RawTileGrid::const_iterator;
  [[nodiscard]] auto end() const -> RawTileGrid::const_iterator;

  [[nodiscard]] auto try_remove_filled_rows(const std::unordered_set<Coordinate>& filled_row_y_coords) -> std::size_t;
  void move(const TilePositions& old_positions, const TilePositions& new_positions);
};

#endif // TILE_GRID_HPP
