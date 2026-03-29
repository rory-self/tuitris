#include "game/game_session.hpp"

#include <algorithm>
#include <ranges>

namespace {
[[nodiscard]] auto is_taken(const Tile& tile) noexcept -> bool {
  return std::holds_alternative<Taken>(tile);
}
} // namespace

auto GameSession::is_taken_or_out_of_bounds(const Coordinates& pos) const -> bool {
  if (pos.x < 0 or pos.x >= signed_game_width or pos.y < 0 or pos.y >= signed_game_height) {
    return true;
  }

  const Tile& tile = get_tile(pos);
  return is_taken(tile);
}


GameSession::GameSession() {
  for (auto& tile_row : _tile_data) {
    for (Tile& tile : tile_row) {
      tile = Empty{};
    }
  }
}

auto GameSession::get_tile(const Coordinates& pos) const -> const Tile& {
  const auto& [x, y] = pos;
  return _tile_data.at(y).at(x);
}

auto GameSession::get_tile(const Coordinates& pos) -> Tile& {
  const auto& [x, y] = pos;
  return _tile_data.at(y).at(x);
}

auto GameSession::get_score() const noexcept -> unsigned int {
  return _score;
}

auto GameSession::get_bag() const noexcept -> const TetrominoBag& {
  return _bag;
}

auto GameSession::try_transformation(const Transformation transformation) -> bool {
  if (not _falling_tetromino.has_value()) {
    return false;
  }

  FallingTetromino& falling_tetromino = _falling_tetromino.value();
  auto& [tetromino, tetromino_center_pos] = falling_tetromino; 
  const Colour tetromino_colour = tetromino.get_colour();

  const TilePositions old_tile_positions = tetromino.calc_tile_positions(tetromino_center_pos);
  const bool is_clockwise_rotation = transformation == Transformation::RotateClockwise;
  if (is_clockwise_rotation or transformation == Transformation::RotateAntiClockwise) {
    return try_rotate_tetromino(falling_tetromino, old_tile_positions, is_clockwise_rotation);
  }

  if (transformation == Transformation::Drop) {
    while (_falling_tetromino.has_value()) {
      tick();
    }
    return true;
  }

  int new_center_x = tetromino_center_pos.x;
  if (transformation == Transformation::Left) {
    new_center_x--;
  } else {
    new_center_x++;
  }

  const Coordinates new_center_pos = { .x = new_center_x, .y = tetromino_center_pos.y };
  const TilePositions new_tile_positions = tetromino.calc_tile_positions(new_center_pos);
  for (const Coordinates& pos : new_tile_positions) {
    if (is_taken_or_out_of_bounds(pos)) {
      return false;
    }
  }
 
  update_falling_tiles(tetromino_colour, old_tile_positions, new_tile_positions);
  tetromino_center_pos = new_center_pos;
  return true;
}

auto GameSession::try_rotate_tetromino(FallingTetromino& falling_tetromino,
  const TilePositions& curr_tile_positions, const bool clockwise) -> bool {
  auto& [tetromino, tetromino_center_pos] = falling_tetromino;

  const auto& is_invalid_placement = std::bind_front(&GameSession::is_taken_or_out_of_bounds, this);
  const auto placement_test = [&is_invalid_placement, &tetromino_center_pos](const TilePositions& new_tile_positions) -> bool { 
    if (std::ranges::none_of(new_tile_positions, is_invalid_placement)) {
      tetromino_center_pos = new_tile_positions.back();
      return true;
    }

    return false;
  };

  const bool rotated = tetromino.try_rotate(tetromino_center_pos, placement_test, clockwise);
  if (not rotated) {
    return false;
  }

  const TilePositions new_tile_positions = tetromino.calc_tile_positions(tetromino_center_pos);
  update_falling_tiles(tetromino.get_colour(), curr_tile_positions, new_tile_positions);
  return true;
}

auto GameSession::get_tile_data() const noexcept -> const TileGrid& {
  return _tile_data;
}

void GameSession::tick() {
  if (_game_over) {
    return;
  }

  if (not _falling_tetromino.has_value()) {
    drop_tetromino();
    return;
  }

  FallingTetromino& falling_tetromino = _falling_tetromino.value();
  auto& [tetromino, tetromino_center_pos] = falling_tetromino;

  const Coordinates new_center_pos = { .x = tetromino_center_pos.x, .y = tetromino_center_pos.y + 1 };
  const TilePositions curr_tile_positions = tetromino.calc_tile_positions(tetromino_center_pos);
  const TilePositions new_tile_positions = tetromino.calc_tile_positions(new_center_pos);
  const Colour tetromino_colour = tetromino.get_colour();
  for (const Coordinates& pos : new_tile_positions) {
    if (is_taken_or_out_of_bounds(pos)) {
      place_tiles(tetromino_colour, curr_tile_positions);
      return;
    }
  }

  update_falling_tiles(tetromino_colour, curr_tile_positions, new_tile_positions);
  tetromino_center_pos = new_center_pos;
}

void GameSession::update_falling_tiles(const Colour tetromino_colour,
    const TilePositions& old_tile_positions,
    const TilePositions& new_tile_positions) {

  for (const Coordinates& pos : old_tile_positions) {
    get_tile(pos) = Empty{};
  }

  for (const Coordinates& pos : new_tile_positions) {
    get_tile(pos) = Falling(tetromino_colour);
  }
}

void GameSession::drop_tetromino() {
  constexpr std::size_t drop_height = vanishing_area_height - 6;
  constexpr std::size_t drop_x = game_width / 2;
  _falling_tetromino = {{
    .tetromino = _bag.take(),
    .pivot_pos = { .x = drop_x, .y = drop_height }
  }};
}

void GameSession::place_tiles(const Colour tetromino_colour, const TilePositions& falling_tile_positions) {
  std::unordered_set<Coordinate> y_coords;
  for (const Coordinates& pos : falling_tile_positions) {
    get_tile(pos) = Taken(tetromino_colour);

    y_coords.emplace(pos.y);
  }
  _falling_tetromino = std::nullopt;

  remove_filled_rows(y_coords);
  if (is_overflowing()) {
    _game_over = true;
  }
}

auto GameSession::is_overflowing() const -> bool {
  constexpr std::size_t overflow_height = vanishing_area_height - 1;
  return std::ranges::any_of(_tile_data[overflow_height], is_taken);
}

void GameSession::remove_filled_rows(const std::unordered_set<Coordinate>& y_coords) {
  std::optional<Coordinate> bottom_row_removed;
  int rows_removed = 0;

  for (const Coordinate y_coord : y_coords) {
    TileRow& tile_row = _tile_data.at(y_coord);

    bool row_filled = true;
    for (const Tile& tile : tile_row) {
      if (not is_taken(tile)) {
        row_filled = false;
        break;
      }
    }

    if (not row_filled) {
      continue;
    }

    const Coordinate curr_bottom_row = bottom_row_removed.value_or(y_coord);
    bottom_row_removed = std::max(curr_bottom_row, y_coord);
    rows_removed++;
    for (Tile& tile : tile_row) {
      tile = Empty{};
    }
  }

  if (not bottom_row_removed.has_value()) {
    return;
  }

  // make tiles above fall
  for (Coordinate y_coord = bottom_row_removed.value() - rows_removed; y_coord >= 0; y_coord--) {
    TileRow& tile_row = _tile_data.at(y_coord);
    TileRow& row_below = _tile_data.at(y_coord + rows_removed);

    for (auto [tile, tile_below] : std::views::zip(tile_row, row_below)) {
      if (std::holds_alternative<Empty>(tile)) {
        continue;
      }

      tile_below = tile;
      tile = Empty{};
    }
  }

  score_removed_rows(rows_removed); 
}

void GameSession::score_removed_rows(const unsigned int rows_removed) {
  switch (rows_removed) {
    case 1: {
      constexpr unsigned int single_score = 40;
      _score += single_score;
      break;
    }
    case 2: {
      constexpr unsigned int double_score = 100;
      _score += double_score;
      break;
    }
    case 3: {
      constexpr unsigned int triple_score = 400;
      _score += triple_score;
      break;
    }
    case 4: {
      constexpr unsigned int tetris_score = 1200;
      _score += tetris_score;
      break;
    }
    default:
      std::unreachable();
  }
}

