#include "game/tile_grid.hpp"
#include "coordinates.hpp"

#include <algorithm>
#include <ranges>
#include <queue>
#include <cassert>
#include <functional>

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

auto TileGrid::can_not_move_to(const TilePositions& tile_positions) const -> bool {
  const auto& is_invalid_placement = std::bind_front(&TileGrid::is_taken_or_out_of_bounds, this);
  return std::ranges::any_of(tile_positions, is_invalid_placement);
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

  fall_tiles();
  return rows_removed;
}

void TileGrid::drop(const TilePositions& positions, const Colour colour) {
  for (const Coordinates& pos : positions) {
    (*this)[pos] = Falling(colour);
  }
}

auto TileGrid::place(const TilePositions& positions) -> std::size_t{
  std::unordered_set<Coordinate> y_coords;
  for (const Coordinates& pos : positions) {
    Tile& tile = (*this)[pos];

    assert(std::holds_alternative<Falling>(tile) and "Attempted to place non-falling tile");

    tile = Taken(std::get<Falling>(tile));
    y_coords.emplace(pos.y);
  }

  return try_remove_filled_rows(y_coords);
}

void TileGrid::move(const TilePositions& old_positions, const TilePositions& new_positions) {
  std::array<Tile, tiles_in_tetromino> cached_tiles;

  for (const auto& [index, pos] : std::ranges::views::enumerate(old_positions)) {
    cached_tiles.at(index) = (*this)[pos];

    (*this)[pos] = Empty{};
  }

  for (const auto& [pos, tile] : std::ranges::views::zip(new_positions, cached_tiles)) {
    (*this)[pos] = tile;
  }
}

void TileGrid::fall_tiles() {
  const auto& row_occupied = [](const TileRow& row) -> bool {
    return std::ranges::any_of(row, is_taken);
  };

  std::queue<std::reference_wrapper<TileRow>> empty_rows;
  for (TileRow& curr_row : _tile_grid | std::views::reverse) {
    if (row_occupied(curr_row)) {
      if (empty_rows.empty()) {
        continue;
      }

      empty_rows.front().get() = curr_row;
      empty_rows.pop();

      std::ranges::fill(curr_row, Empty{});
    }

    empty_rows.emplace(curr_row);
  }
}
