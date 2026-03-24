#include "game.hpp"

bool Game::has_started() const noexcept {
  return _tile_data.has_value();
}

void Game::start() {
  _tile_data.emplace();
}
