#ifndef GAME_HPP
#define GAME_HPP

#include "game_session.hpp"

class Game {
private:
  std::optional<GameSession> _session = std::nullopt;

public:
  [[nodiscard]] auto get_session() -> std::optional<std::reference_wrapper<GameSession>>;
  [[nodiscard]] auto has_started() const noexcept -> bool;
  auto start() -> const GameSession&;
};

#endif // GAME_HPP
