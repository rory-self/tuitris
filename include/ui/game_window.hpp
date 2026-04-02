#ifndef GAME_WINDOW_HPP
#define GAME_WINDOW_HPP

#include "coordinates.hpp"
#include "game/game_session.hpp"
#include "ui/window.hpp"

#include <ncurses.h>
#include <memory>

class GameWindows {
private:
  static constexpr int game_win_height = signed_game_height + 2;
  static constexpr int game_win_width = (signed_game_width + 2) * 2;
  using WindowPtr = std::unique_ptr<WINDOW, decltype(&delwin)>;

  const Window _game_window;
  const Window _piece_window;
  const Window _score_window;
  const Window _level_window;
  const GameSession& _game;
  const TetrominoBag& _bag;

  void clear() const;
  void draw_border() const;
  void init_score_window() const;
  void init_piece_window() const;
  void refresh() const;
  void update_score() const;
  void update_level() const;
  void update_next_piece() const;

public:
  explicit GameWindows(const GameSession& game);

  void update() const;
};

#endif //GAME_WINDOW_HPP
