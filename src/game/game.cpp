#include "game/game.hpp"

// Game //
auto Game::has_started() const noexcept -> bool {
  return _session.has_value();
}

auto Game::start() -> const GameSession& {
  _session.emplace();

  return _session.value();
}

auto Game::get_session() -> std::optional<std::reference_wrapper<GameSession>> {
  if (_session.has_value()) {
    return _session.value();
  }

  return std::nullopt;
}

// GameSession //

