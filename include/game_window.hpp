#ifndef GAME_WINDOW_HPP
#define GAME_WINDOW_HPP

#include "coordinates.hpp"
#include "game/game_session.hpp"

#include <ncurses.h>
#include <memory>

class GameWindows {
private:
  static constexpr int game_win_height = signed_game_height + 2;
  static constexpr int game_win_width = (signed_game_width + 2) * 2;
  using WindowPtr = std::unique_ptr<WINDOW, decltype(&delwin)>;

  const WindowPtr _game_window;
  const WindowPtr _piece_window;
  const WindowPtr _score_window;
  const GameSession& _game;
  const TetrominoBag& _bag;

  [[nodiscard]] static auto make_window(int height, int width, int y_pos, int x_pos) -> WindowPtr;
  static void move_print(const WindowPtr& window, const Coordinates& pos, std::string_view text);
  static void init_titled_window(const WindowPtr& window, std::string_view title);
  static void print_block(const WindowPtr& window, const Coordinates& pos, Colour colour);

  void clear() const;
  void draw_border() const;
  void init_score_window() const;
  void init_piece_window() const;
  void refresh() const;
  void update_score() const;
  void update_next_piece() const;

public:
  explicit GameWindows(const GameSession& game);

  void update() const;
};

#endif //GAME_WINDOW_HPP
