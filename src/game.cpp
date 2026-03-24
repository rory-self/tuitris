#include "game.hpp"

auto Game::has_started() const noexcept -> bool {
  return _data.has_value();
}

void Game::start() {
  const GameData data = { .floating_tiles = {{ .x = 0, .y = 0 }} };

  _data = { data };
}

auto Game::try_move(const Input input) -> bool {
  int x_offset;
  switch (input) {
    case Input::Left:
      x_offset = -1;
      break;
    case Input::Right:
      x_offset = 1;
      break;
    default:
      return false;
  }

  auto& [tile_data, floating_tiles] = _data.value();
  std::vector<Coordinates> new_floating_coords;
  for (const auto& [x, y] : floating_tiles) {
    const int new_x_pos = x + x_offset;
    if (new_x_pos < 0 or new_x_pos > static_cast<int>(game_width) - 1) {
      return false;
    }

    if (tile_data[y][new_x_pos] == TileState::Taken) {
      return false;
    }

    new_floating_coords.emplace_back(new_x_pos, y);
  }

  for (const auto& [x, y] : floating_tiles) {
    tile_data[y][x] = TileState::Empty;
  }

  for (const auto& [x, y] : new_floating_coords) {
    tile_data[y][x] = TileState::Hovering;
  }
  floating_tiles = new_floating_coords;
  return true;
}

auto Game::get_tile_data() const -> const TileData& {
  return _data.value().tile_data;
}

void Game::tick() {
  auto& [tile_data, floating_tiles] = _data.value();
  std::vector<Coordinates> new_floating_coords;
  for (const auto& [x, y] : floating_tiles) {
    if (y == game_height - 1 or tile_data[y + 1][x] == TileState::Taken) {
      place_tiles(tile_data, floating_tiles);
      return;
    }

    new_floating_coords.emplace_back(x, y + 1);
  }

  for (const auto& [x, y] : floating_tiles) {
    tile_data[y][x] = TileState::Empty;
  }
  
  for (const auto& [x, y]: new_floating_coords) {
    tile_data[y][x] = TileState::Hovering;
  }
  floating_tiles = new_floating_coords;

  return;
}

void Game::place_tiles(TileData& data, std::vector<Coordinates>& floating_tiles) {
  for (const auto& [x, y] : floating_tiles) {
    data[y][x] = TileState::Taken;
  }

  floating_tiles = {{ .x = 0, .y = 0 }};
}
