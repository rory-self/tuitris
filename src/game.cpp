#include "game.hpp"

// Game //
auto Game::has_started() const noexcept -> bool {
  return _session.has_value();
}

void Game::start() {
  _session.emplace();
}

auto Game::get_session() -> GameSession& {
  return _session.value();
}

// GameSession //
GameSession::GameSession() {
  for (auto& tile_row : _tile_data) {
    for (Tile& tile : tile_row) {
      tile.state = TileState::Empty;
    }
  }

  refill_bag();
}

void GameSession::refill_bag() {
  _shape_bag = {Shape::L};
}

auto GameSession::try_move(const Input input) -> bool {
  if ((input != Input::Left and input != Input::Right) or not _falling_tetromino.has_value()) {
    return false;
  }

  FallingTetromino& falling_tetromino = _falling_tetromino.value();
  auto& [tetromino, tetromino_center_pos] = falling_tetromino; 
  int new_center_x = tetromino_center_pos.x;
  if (input == Input::Left) {
    new_center_x--;
  } else {
    new_center_x++;
  }

  const Coordinates new_center_pos = { .x = new_center_x, .y = tetromino_center_pos.y };
  const std::array<Coordinates, 4> old_tile_positions = tetromino.get_tile_positions(tetromino_center_pos);
  const std::array<Coordinates, 4> new_tile_positions = tetromino.get_tile_positions(new_center_pos);
  for (const auto [x, y] : new_tile_positions) {
    if (x < 0 or x >= signed_game_width or _tile_data[y][x] == TileState::Taken) {
      return false;
    }
  }
 
  update_falling_tiles(falling_tetromino, old_tile_positions, new_tile_positions);
  tetromino_center_pos = new_center_pos;
  return true;
}

auto GameSession::get_tile_data() const noexcept -> const TileData& {
  return _tile_data;
}

void GameSession::tick() {
  if (not _falling_tetromino.has_value()) {
    drop_tetromino();
    return;
  }

  FallingTetromino& falling_tetromino = _falling_tetromino.value();
  auto& [tetromino, tetromino_center_pos] = falling_tetromino;

  const Coordinates new_center_pos = { .x = tetromino_center_pos.x, .y = tetromino_center_pos.y + 1 };
  const std::array<Coordinates, 4> curr_tile_positions = tetromino.get_tile_positions(tetromino_center_pos);
  const std::array<Coordinates, 4> new_tile_positions = tetromino.get_tile_positions(new_center_pos);
  for (const auto [x, y] : new_tile_positions) {
    if (y >= signed_game_height or _tile_data[y][x] == TileState::Taken) {
      place_tiles(curr_tile_positions);
      return;
    }
  }

  update_falling_tiles(falling_tetromino, curr_tile_positions, new_tile_positions);
  tetromino_center_pos = new_center_pos;
}

void GameSession::update_falling_tiles(const FallingTetromino& falling_tetromino,
    const std::array<Coordinates, 4>& old_tile_positions,
    const std::array<Coordinates, 4>& new_tile_positions) {

  for (const auto [x, y] : old_tile_positions) {
    _tile_data[y][x].state = TileState::Empty;
  }

  for (const auto [x, y] : new_tile_positions) {
    _tile_data[y][x].state = TileState::Falling;
  }
}

void GameSession::drop_tetromino() {
  if (_shape_bag.empty()) {
    refill_bag();
  }

  const Shape shape = _shape_bag.back();
  _shape_bag.pop_back();

  const Tetromino tetromino = Tetromino(shape);
  _falling_tetromino = {{ tetromino, { 2, 2 }}};
}

void GameSession::place_tiles(const std::array<Coordinates, 4>& falling_tile_positions) {
  for (const auto& [x, y] : falling_tile_positions) {
    _tile_data[y][x].state = TileState::Taken;
  }

  _falling_tetromino = std::nullopt;
}
