#include "game/game_session.hpp"
#include "transformation.hpp"

#include <algorithm>
#include <utility>

GameSession::GameSession(const std::optional<int> seed): _bag(seed) {
  drop_new_tetromino();
}

auto GameSession::get_score() const noexcept -> unsigned int {
  return _score;
}

auto GameSession::get_level() const noexcept -> unsigned int {
  return _level;
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

auto GameSession::try_transformation(const Transformation transformation) -> TransformationRes {
  if (not _falling_tetromino.has_value()) {
    return TransformationRes::Fail;
  }
 
  using enum Transformation;
  switch (transformation) {
    case RotateClockwise:
    case RotateAntiClockwise:
      return try_rotate_tetromino(transformation == RotateClockwise);
    case Left:
    case Right:
      return try_move_tetromino(transformation == Right);
    case HardDrop:
      return hard_drop_tetromino();
    case SoftDrop:
      return soft_drop_tetromino();
    default:
      std::unreachable();
  }
}

auto GameSession::hard_drop_tetromino() -> TransformationRes {
  while (_falling_tetromino.has_value() and not _game_over) {
    tick();
  }
  
  if (not _game_over) {
    drop_new_tetromino();
  }
  return TransformationRes::Success;
}

auto GameSession::soft_drop_tetromino() -> TransformationRes {
  tick();
  return TransformationRes::ResetTick;
}

auto GameSession::try_move_tetromino(const bool move_right) -> TransformationRes {
  auto& [tetromino, center_pos] = _falling_tetromino.value();

  const int new_center_x = move_right ? center_pos.x + 1 : center_pos.x - 1;
  const Coordinates new_center_pos = { .x = new_center_x, .y = center_pos.y };
  const TilePositions new_tile_positions = tetromino.calc_tile_positions(new_center_pos);

  if (_tile_data.can_not_move_to(new_tile_positions)) {
    return TransformationRes::Fail;
  }

  const TilePositions curr_tile_positions = tetromino.calc_tile_positions(center_pos);
  _tile_data.move(curr_tile_positions, new_tile_positions);

  center_pos = new_center_pos;
  return TransformationRes::Success;

}

auto GameSession::try_rotate_tetromino(const bool clockwise) -> TransformationRes {
  auto& [tetromino, tetromino_center_pos] = _falling_tetromino.value();

  const auto& is_invalid_placement = std::bind_front(&TileGrid::is_taken_or_out_of_bounds, _tile_data);
  const auto try_rotation = [&](const TilePositions& old_tile_positions, const TilePositions& new_tile_positions) -> bool { 
    if (std::ranges::none_of(new_tile_positions, is_invalid_placement)) {
      tetromino_center_pos = new_tile_positions.back();
      _tile_data.move(old_tile_positions, new_tile_positions);
      return true;
    }

    return false;
  };

  if (not tetromino.try_rotate(tetromino_center_pos, try_rotation, clockwise)) {
    return TransformationRes::Fail;
  }

  return TransformationRes::Success;
}

void GameSession::tick() {
  if (_game_over) {
    return;
  }

  if (not _falling_tetromino.has_value()) {
    drop_new_tetromino();
    return;
  }

  FallingTetromino& falling_tetromino = _falling_tetromino.value();
  auto& [tetromino, tetromino_center_pos] = falling_tetromino;

  const TilePositions curr_tile_positions = tetromino.calc_tile_positions(tetromino_center_pos);
  const Coordinates new_center_pos = { .x = tetromino_center_pos.x, .y = tetromino_center_pos.y + 1 };
  const TilePositions new_tile_positions = tetromino.calc_tile_positions(new_center_pos);

  if (_tile_data.can_not_move_to(new_tile_positions)) {
    place_tiles(curr_tile_positions);
    return;
  }

  _tile_data.move(curr_tile_positions, new_tile_positions);
  tetromino_center_pos = new_center_pos;
}

void GameSession::drop_new_tetromino() {
  _falling_tetromino = {{ .tetromino = _bag.take() }};

  const Tetromino& tetromino = _falling_tetromino->tetromino;
  const TilePositions tetromino_positions = tetromino.calc_tile_positions(drop_pos);
  _tile_data.drop(tetromino_positions, tetromino.get_colour());
}

void GameSession::place_tiles(const TilePositions& falling_tile_positions) {
  _falling_tetromino = std::nullopt;

  if (const std::size_t rows_removed = _tile_data.place(falling_tile_positions); rows_removed > 0) {
    calc_score_increase(rows_removed);
  }

  if (_tile_data.is_overflowing()) {
    _game_over = true;
  }
}

void GameSession::calc_score_increase(const std::size_t rows_removed) {
  const unsigned int score_diff = score_removed_rows(rows_removed);
  _score += score_diff;
  _curr_level_score += score_diff;
  
  constexpr unsigned int level_threshold_multiplier = 100;
  if (_curr_level_score >= level_threshold_multiplier * _level) {
    _curr_level_score = 0;
    _level++;
  }
}

auto GameSession::score_removed_rows(const std::size_t rows_removed) const -> unsigned int {
  switch (rows_removed) {
    case 1: {
      constexpr unsigned int single_score = 40;
      return single_score;
    }
    case 2: {
      constexpr unsigned int double_score = 100;
      return double_score;
    }
    case 3: {
      constexpr unsigned int triple_score = 300;
      return triple_score;
    }
    case 4: {
      constexpr unsigned int tetris_score = 1200;
      return tetris_score;
    }
    default:
      std::unreachable();
  }
}

