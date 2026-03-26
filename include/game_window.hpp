#ifndef GAME_WINDOW_HPP
#define GAME_WINDOW_HPP

#include "coordinates.hpp"
#include "game/game.hpp"

#include <ncurses.h>
#include <memory>

class GameWindow {
private:
  static constexpr int game_win_height = signed_game_height + 2;
  static constexpr int game_win_width = (signed_game_width + 2) * 2;
  using WindowPtr = std::unique_ptr<WINDOW, decltype(&delwin)>;

  const WindowPtr _game_window;
  const WindowPtr _score_window;
  const GameSession& _game;

  [[nodiscard]] static auto make_game_window() -> WindowPtr;
  [[nodiscard]] static auto make_score_window() -> WindowPtr;

  void print_block(const Coordinates& pos, const Colour colour) const;
  void clear() const;
  void draw_border() const;
  void init_score_window() const;
  void refresh() const;

public:
  GameWindow(const GameSession& game);

  void update() const;
};

#endif //GAME_WINDOW_HPP
