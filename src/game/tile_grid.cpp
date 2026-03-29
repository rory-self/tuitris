#include "game/tile_grid.hpp"

#include <algorithm>
#include <ranges>

namespace {
[[nodiscard]] auto is_taken(const Tile& tile) noexcept -> bool {
  return std::holds_alternative<Taken>(tile);
}
} // namespace

TileGrid::TileGrid() {
  for (TileRow& tile_row : _tile_grid) {
    std::ranges::fill(tile_row, Empty{});
  }
}

auto TileGrid::operator[](const Coordinates& pos) const -> const Tile& {
  const auto& [x, y] = pos;
  return _tile_grid.at(y).at(x);
}

auto TileGrid::operator[](const Coordinates& pos) -> Tile& {
  const auto& [x, y] = pos;
  return _tile_grid.at(y).at(x);
}

auto TileGrid::operator[](const std::size_t y_coord) const -> const TileRow& {
  return _tile_grid.at(y_coord);
}

auto TileGrid::operator[](const std::size_t y_coord) -> TileRow& {
  return _tile_grid.at(y_coord);
}

auto TileGrid::begin() const -> RawTileGrid::const_iterator {
  return _tile_grid.cbegin();
}

auto TileGrid::end() const -> RawTileGrid::const_iterator {
  return _tile_grid.cend();
}

auto TileGrid::is_taken_or_out_of_bounds(const Coordinates& pos) const -> bool {
  const auto [x, y] = pos;
  if (x < 0 or x >= signed_game_width or y < 0 or y >= signed_game_height) {
    return true;
  }

  return is_taken((*this)[pos]);
}

auto TileGrid::is_overflowing() const -> bool {
  constexpr std::size_t overflow_height = vanishing_area_height - 1;
  return std::ranges::any_of((*this)[overflow_height], is_taken);
}

auto TileGrid::try_remove_filled_rows(const std::unordered_set<Coordinate>& y_coords) -> std::size_t {
  const auto& row_filled = [this](const Coordinate y_coord) -> bool {
    return std::ranges::all_of((*this)[y_coord], is_taken);
  };
  auto filled_y_coords = y_coords | std::views::filter(row_filled); 

  if (filled_y_coords.empty()) {
    return 0;
  }

  const std::size_t rows_removed = std::ranges::count_if(y_coords, row_filled);
  for (const Coordinate filled_y_coord : filled_y_coords) {
    std::ranges::fill((*this)[filled_y_coord], Empty{});
  }

  fall_tiles(rows_removed, std::ranges::min(filled_y_coords));
  return rows_removed;
}

void TileGrid::fall_tiles(const std::size_t rows_removed, const Coordinate bottom_row_removed) {
  const std::size_t starting_row = bottom_row_removed - rows_removed;
  for (std::size_t y_coord = starting_row; y_coord <= starting_row; y_coord--) {
    TileRow& tile_row = (*this)[y_coord];
    TileRow& row_below = (*this)[y_coord + rows_removed];

    for (auto [tile, tile_below] : std::views::zip(tile_row, row_below)) {
      if (std::holds_alternative<Empty>(tile)) {
        continue;
      }

      tile_below = tile;
      tile = Empty{};
    }
  }
}
