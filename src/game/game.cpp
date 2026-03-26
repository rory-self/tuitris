#include "game/game.hpp"
#include "coordinates.hpp"
#include "game/tetromino.hpp"
#include "inputs.hpp"

#include <algorithm>
#include <variant>

namespace {
[[nodiscard]] auto is_taken(const Tile& tile) -> bool {
  return std::holds_alternative<Taken>(tile);
}
} // namespace

// Game //
auto Game::has_started() const noexcept -> bool {
  return _session.has_value();
}

auto Game::start() -> const GameSession& {
  _session.emplace();

  return _session.value();
}

auto Game::get_session() -> GameSession& {
  return _session.value();
}

// GameSession //
GameSession::GameSession() {
  for (auto& tile_row : _tile_data) {
    for (Tile& tile : tile_row) {
      tile = Empty{};
    }
  }

  refill_bag();
}

auto GameSession::get_score() const noexcept -> unsigned int {
  return _score;
}

void GameSession::refill_bag() {
  constexpr std::size_t bag_size = 14;
  _shape_bag.clear();
  _shape_bag.reserve(bag_size);

  constexpr std::size_t bag_layers = bag_size / 7;
  for (std::size_t i = 0; i < bag_layers; i++) {
    _shape_bag.emplace_back(std::make_unique<LTetromino>());
    _shape_bag.emplace_back(std::make_unique<ITetromino>());
    _shape_bag.emplace_back(std::make_unique<JTetromino>());
    _shape_bag.emplace_back(std::make_unique<ZTetromino>());
    _shape_bag.emplace_back(std::make_unique<OTetromino>());
    _shape_bag.emplace_back(std::make_unique<STetromino>());
    _shape_bag.emplace_back(std::make_unique<TTetromino>());
  }

  std::ranges::shuffle(_shape_bag, _bag_rng);
}

auto GameSession::try_transformation(const Transformation transformation) -> bool {
  if (not _falling_tetromino.has_value()) {
    return false;
  }

  FallingTetromino& falling_tetromino = _falling_tetromino.value();
  auto& [tetromino, tetromino_center_pos] = falling_tetromino; 
  const Colour tetromino_colour = tetromino->get_colour();

  const TilePositions old_tile_positions = tetromino->get_tile_positions(tetromino_center_pos);
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
  const TilePositions new_tile_positions = tetromino->get_tile_positions(new_center_pos);
  for (const auto [x, y] : new_tile_positions) {
    if (x < 0 or x >= signed_game_width or is_taken(_tile_data[y][x])) {
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
  const bool can_rotate = tetromino->rotate(tetromino_center_pos, clockwise);
  if (not can_rotate) {
    return false;
  }

  const TilePositions new_tile_positions = tetromino->get_tile_positions(tetromino_center_pos);
  update_falling_tiles(tetromino->get_colour(), curr_tile_positions, new_tile_positions);
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
  const TilePositions curr_tile_positions = tetromino->get_tile_positions(tetromino_center_pos);
  const TilePositions new_tile_positions = tetromino->get_tile_positions(new_center_pos);
  const Colour tetromino_colour = tetromino->get_colour();
  for (const auto [x, y] : new_tile_positions) {
    if (y >= signed_game_height or is_taken(_tile_data[y][x])) {
      if (not try_place_tiles(tetromino_colour, curr_tile_positions)) {
        _game_over = true;
      }
      return;
    }
  }

  update_falling_tiles(tetromino_colour, curr_tile_positions, new_tile_positions);
  tetromino_center_pos = new_center_pos;
}

void GameSession::update_falling_tiles(const Colour tetromino_colour,
    const TilePositions& old_tile_positions,
    const TilePositions& new_tile_positions) {

  for (const auto [x, y] : old_tile_positions) {
    _tile_data[y][x] = Empty{};
  }

  for (const auto [x, y] : new_tile_positions) {
    _tile_data[y][x] = Falling(tetromino_colour);
  }
}

void GameSession::drop_tetromino() {
  if (_shape_bag.empty()) {
    refill_bag();
  }

  constexpr std::size_t drop_height = vanishing_area_height - 6;
  constexpr std::size_t drop_x = game_width / 2;
  _falling_tetromino = {{ std::move(_shape_bag.back()), { drop_x, drop_height }}};
  _shape_bag.pop_back();
}

auto GameSession::try_place_tiles(const Colour tetromino_colour, const TilePositions& falling_tile_positions) -> bool {
  std::unordered_set<Coordinate> y_coords;
  for (const auto& [x, y] : falling_tile_positions) {
    _tile_data[y][x] = Taken(tetromino_colour);

    y_coords.emplace(y);
  }
  _falling_tetromino = std::nullopt;

  remove_filled_rows(y_coords);
  return not_overflowing();
}

auto GameSession::not_overflowing() const -> bool {
  constexpr std::size_t overflow_height = vanishing_area_height - 1;
  return std::ranges::none_of(_tile_data[overflow_height], is_taken);
}

void GameSession::remove_filled_rows(const std::unordered_set<Coordinate>& y_coords) {
  std::optional<Coordinate> bottom_row_removed;
  unsigned int rows_removed = 0;

  for (const Coordinate y : y_coords) {
    TileRow& tile_row = _tile_data[y];

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

    const Coordinate curr_bottom_row = bottom_row_removed.value_or(y);
    bottom_row_removed = std::max(curr_bottom_row, y);
    rows_removed++;
    for (Tile& tile : tile_row) {
      tile = Empty{};
    }
  }

  if (not bottom_row_removed.has_value()) {
    return;
  }

  // make tiles above fall
  for (Coordinate y = bottom_row_removed.value() - rows_removed; y >= 0; y--) {
    TileRow& tile_row = _tile_data[y];
    TileRow& row_below = _tile_data[y + rows_removed];

    for (Coordinate x = 0; x < signed_game_width; x++) {
      if (std::holds_alternative<Empty>(tile_row[x])) {
        continue;
      }

      row_below[x] = tile_row[x];
      tile_row[x] = Empty{};
    }
  }

  score_removed_rows(rows_removed); 
}

void GameSession::score_removed_rows(const unsigned int rows_removed) {
  // TODO migrate to modern scoring system
  switch (rows_removed) {
    case 1:
      _score += 40;
      break;
    case 2:
      _score += 100;
      break;
    case 3:
      _score += 400;
      break;
    case 4:
      _score += 1200;
      break;
  }
}


