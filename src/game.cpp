#include "game.hpp"
#include "tetromino.hpp"

#include <algorithm>
#include <variant>

namespace {
[[nodiscard]] auto is_taken(const Tile& tile) -> bool {
  return std::holds_alternative<Taken>(tile);
}
} // namespace

// Tile //
auto OccupiedTile::get_colour() const noexcept -> Colour {
  return _colour;
}
OccupiedTile::OccupiedTile(const Colour colour) : _colour(colour) {}
Taken::Taken(const Colour colour) : OccupiedTile(colour) {}
Falling::Falling(const Colour colour) : OccupiedTile(colour) {}

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
      tile = Empty{};
    }
  }

  refill_bag();
}

void GameSession::refill_bag() {
  _shape_bag.clear();
  _shape_bag.reserve(7);

  _shape_bag.emplace_back(std::make_unique<LTetromino>());
  _shape_bag.emplace_back(std::make_unique<ITetromino>());
  _shape_bag.emplace_back(std::make_unique<JTetromino>());
  _shape_bag.emplace_back(std::make_unique<ZTetromino>());
  _shape_bag.emplace_back(std::make_unique<OTetromino>());
  _shape_bag.emplace_back(std::make_unique<STetromino>());
  _shape_bag.emplace_back(std::make_unique<TTetromino>());

  std::ranges::shuffle(_shape_bag, _bag_rng);
}

auto GameSession::try_move(const Input input) -> bool {
  if (not _falling_tetromino.has_value()) {
    return false;
  }

  if (input != Input::Left and input != Input::Right and input != Input::RotateClockwise) {
    return false;
  }

  FallingTetromino& falling_tetromino = _falling_tetromino.value();
  auto& [tetromino, tetromino_center_pos] = falling_tetromino; 
  const Colour tetromino_colour = tetromino->get_colour();

  if (input == Input::RotateClockwise) {
    const std::array<Coordinates, 4> old_tile_positions = tetromino->get_tile_positions(tetromino_center_pos);
    bool _ = tetromino->rotate_clockwise(tetromino_center_pos);
    const std::array<Coordinates, 4> new_tile_positions = tetromino->get_tile_positions(tetromino_center_pos);
    update_falling_tiles(tetromino_colour, old_tile_positions, new_tile_positions);
    return true;
  }

  int new_center_x = tetromino_center_pos.x;
  if (input == Input::Left) {
    new_center_x--;
  } else {
    new_center_x++;
  }

  const Coordinates new_center_pos = { .x = new_center_x, .y = tetromino_center_pos.y };
  const std::array<Coordinates, 4> old_tile_positions = tetromino->get_tile_positions(tetromino_center_pos);
  const std::array<Coordinates, 4> new_tile_positions = tetromino->get_tile_positions(new_center_pos);
  for (const auto [x, y] : new_tile_positions) {
    if (x < 0 or x >= signed_game_width or is_taken(_tile_data[y][x])) {
      return false;
    }
  }
 
  update_falling_tiles(tetromino_colour, old_tile_positions, new_tile_positions);
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
  const std::array<Coordinates, 4> curr_tile_positions = tetromino->get_tile_positions(tetromino_center_pos);
  const std::array<Coordinates, 4> new_tile_positions = tetromino->get_tile_positions(new_center_pos);
  const Colour tetromino_colour = tetromino->get_colour();
  for (const auto [x, y] : new_tile_positions) {
    if (y >= signed_game_height or is_taken(_tile_data[y][x])) {
      place_tiles(tetromino_colour, curr_tile_positions);
      return;
    }
  }

  update_falling_tiles(tetromino_colour, curr_tile_positions, new_tile_positions);
  tetromino_center_pos = new_center_pos;
}

void GameSession::update_falling_tiles(const Colour tetromino_colour,
    const std::array<Coordinates, 4>& old_tile_positions,
    const std::array<Coordinates, 4>& new_tile_positions) {

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

  _falling_tetromino = {{ std::move(_shape_bag.back()), { 2, 2}}};
  _shape_bag.pop_back();
}

void GameSession::place_tiles(const Colour tetromino_colour, const std::array<Coordinates, 4>& falling_tile_positions) {
  for (const auto& [x, y] : falling_tile_positions) {
    _tile_data[y][x] = Taken(tetromino_colour);
  }

  _falling_tetromino = std::nullopt;
}


