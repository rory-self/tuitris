#include "game/game_session.hpp"
#include "transformation.hpp"

#include <algorithm>
#include <utility>

GameSession::GameSession(const std::optional<int> seed): _bag(seed) {
  drop_tetromino();
}

auto GameSession::get_score() const noexcept -> unsigned int {
  return _score;
}

auto GameSession::get_bag() const noexcept -> const TetrominoBag& {
  return _bag;
}

auto GameSession::get_tile_data() const noexcept -> const TileGrid& {
  return _tile_data;
}

auto GameSession::game_over() const noexcept -> bool {
  return _game_over;
}

auto GameSession::can_not_place(const TilePositions& tile_positions) -> bool {
  const auto& is_invalid_placement = std::bind_front(&TileGrid::is_taken_or_out_of_bounds, _tile_data);
  return std::ranges::any_of(tile_positions, is_invalid_placement);
}

auto GameSession::try_transformation(const Transformation transformation) -> bool {
  if (not _falling_tetromino.has_value()) {
    return false;
  }
 
  using enum Transformation;
  switch (transformation) {
    case RotateClockwise:
    case RotateAntiClockwise:
      return try_rotate_tetromino(transformation == RotateClockwise);
    case Left:
    case Right:
      return try_move_tetromino(transformation == Right);
    case Drop:
      while (_falling_tetromino.has_value() and not _game_over) {
        tick();
      }

      drop_tetromino();
      return true;
    default:
      std::unreachable();
  }
}

auto GameSession::try_move_tetromino(const bool move_right) -> bool {
  auto& [tetromino, tetromino_center_pos] = _falling_tetromino.value();

  int new_center_x = tetromino_center_pos.x;
  if (move_right) {
    new_center_x++;
  } else {
    new_center_x--;
  }

  const Coordinates new_center_pos = { .x = new_center_x, .y = tetromino_center_pos.y };
  const TilePositions new_tile_positions = tetromino.calc_tile_positions(new_center_pos);

  if (can_not_place(new_tile_positions)) {
    return false;
  }

  const TilePositions old_tile_positions = tetromino.calc_tile_positions(tetromino_center_pos);
  update_falling_tiles(old_tile_positions, new_tile_positions);

  tetromino_center_pos = new_center_pos;
  return true;

}

auto GameSession::try_rotate_tetromino(const bool clockwise) -> bool {
  auto& [tetromino, tetromino_center_pos] = _falling_tetromino.value();

  const auto& is_invalid_placement = std::bind_front(&TileGrid::is_taken_or_out_of_bounds, _tile_data);
  const auto placement_test = [is_invalid_placement](const TilePositions& new_tile_positions) -> bool { 
    return std::ranges::none_of(new_tile_positions, is_invalid_placement);
  };

  if (not tetromino.try_rotate(tetromino_center_pos, placement_test, clockwise)) {
    return false;
  }

  const TilePositions curr_tile_positions = tetromino.calc_tile_positions(tetromino_center_pos);
  const TilePositions new_tile_positions = tetromino.calc_tile_positions(tetromino_center_pos);
  update_falling_tiles(curr_tile_positions, new_tile_positions);
  return true;
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

  const TilePositions curr_tile_positions = tetromino.calc_tile_positions(tetromino_center_pos);
  const Coordinates new_center_pos = { .x = tetromino_center_pos.x, .y = tetromino_center_pos.y + 1 };
  const TilePositions new_tile_positions = tetromino.calc_tile_positions(new_center_pos);

  if (can_not_place(new_tile_positions)) {
    place_tiles(curr_tile_positions);
    return;
  }

  update_falling_tiles(curr_tile_positions, new_tile_positions);
  tetromino_center_pos = new_center_pos;
}

void GameSession::update_falling_tiles(const TilePositions& old_positions, const TilePositions& new_positions) {
  const Colour colour = _falling_tetromino->tetromino.get_colour();

  for (const Coordinates& pos : old_positions) {
    _tile_data[pos] = Empty{};
  }

  for (const Coordinates& pos : new_positions) {
    _tile_data[pos] = Falling(colour);
  }
}

void GameSession::drop_tetromino() {
  _falling_tetromino = {{ .tetromino = _bag.take() }};

  const Tetromino& tetromino = _falling_tetromino->tetromino;
  const TilePositions tetromino_positions = tetromino.calc_tile_positions(drop_pos);
  const Colour colour = tetromino.get_colour();
  for (const Coordinates& pos : tetromino_positions) {
    _tile_data[pos] = Falling(colour);
  }
}

void GameSession::place_tiles(const TilePositions& falling_tile_positions) {
  const Colour colour = _falling_tetromino->tetromino.get_colour();

  std::unordered_set<Coordinate> y_coords;
  for (const Coordinates& pos : falling_tile_positions) {
    _tile_data[pos] = Taken(colour);

    y_coords.emplace(pos.y);
  }
  _falling_tetromino = std::nullopt;

  if (const std::size_t rows_removed = _tile_data.try_remove_filled_rows(y_coords); rows_removed > 0) {
    score_removed_rows(rows_removed);
  }

  if (_tile_data.is_overflowing()) {
    _game_over = true;
  }
}

void GameSession::score_removed_rows(const std::size_t rows_removed) {
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
      constexpr unsigned int triple_score = 300;
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

