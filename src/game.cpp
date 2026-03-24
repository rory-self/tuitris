#include "game.hpp"

auto Game::has_started() const noexcept -> bool {
  return _data.has_value();
}

void Game::start() {
  const GameData data = { .floating_tiles = {{ .x = 0, .y = 0 }} };

  _data = { data };
}

auto Game::tick() -> TileData {
  GameData& data = _data.value();
  std::vector<Coordinates> new_floating_coords;
  for (const auto& [x, y] : data.floating_tiles) {
    if (y == game_height - 1 or data.tile_data[y + 1][x] == TileState::Taken) {
      place_tiles(data);
      return data.tile_data;
    }

    new_floating_coords.emplace_back(x, y + 1);
  }

  for (const auto& [x, y] : data.floating_tiles) {
    data.tile_data[y][x] = TileState::Empty;
    data.tile_data[y + 1][x] = TileState::Hovering;
  }
  data.floating_tiles = new_floating_coords;

  return data.tile_data;
}

void Game::place_tiles(GameData& data) {
  for (const auto& [x, y] : data.floating_tiles) {
    data.tile_data[y][x] = TileState::Taken;
  }

  data.floating_tiles.clear();
}
